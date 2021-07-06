#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

struct Mat_2 {
	Mat im1;
	Mat im2;
};

Mat bit_plane(Mat, int); //取出圖像的bit plane
Mat merge_plane(Mat , Mat); //將lena,baboon合成
Mat_2 splite_plane(Mat);//拆開lena與baboon的合成圖

double mse_function(Mat,Mat); //將原圖與修改後的圖做PSNR


int main3_1()
{
	int width = 256;
	int height = 256;
	int size = width * height;
	char lenaF[] = "img/lena_256.raw"; 
	char baboonF[] = "img/baboon_256.raw";

	FILE* lenaFile,* baboonFile;
	lenaFile = fopen(lenaF, "rb");
	baboonFile = fopen(baboonF, "rb");

	unsigned char* lenai = new unsigned char[size];
	unsigned char* babooni = new unsigned char[size];
	fread(lenai, 1, size, lenaFile);
	fread(babooni, 1, size, baboonFile);

	Mat lenaMat = Mat(height, width, CV_8UC1, lenai); 
	Mat baboonMat = Mat(height, width, CV_8UC1, babooni);

	//-------star-------//
	//將lena與baboon合成
	Mat merge_image = merge_plane(lenaMat, baboonMat); 
	//從合成圖拆出lena與baboon
	struct Mat_2 image2 = splite_plane(merge_image);
	//三者的PSNR
	double merge_mse = mse_function(lenaMat, merge_image);
	double merge_psnr = 20 * log10(255 / sqrt(merge_mse));
	printf("\n合成圖的MSE為%f,PSNR為%f\n", merge_mse,merge_psnr);

	double lena_mse = mse_function(lenaMat, image2.im1);
	double lena_psnr = 20 * log10(255 / sqrt(lena_mse));
	printf("lena的MSE為%f,PSNR為%f\n", lena_mse,lena_psnr);

	double baboon_mse = mse_function(baboonMat, image2.im2);
	double baboon_psnr = 20 * log10(255 / sqrt(baboon_mse));
	printf("baboon的MSE為%f,PSNR為%f\n", baboon_mse,baboon_psnr);

	FILE* resultFile;
	char outFile[] = "out/lena_with_baboon.raw";
	resultFile = fopen(outFile, "wb");
	fwrite(merge_image.ptr(0, 0), 1, 256 * 256, resultFile);

	waitKey(0);

	fclose(lenaFile);
	delete lenai;
	destroyWindow("lenaFile");

	return 0;
}
Mat bit_plane(Mat image, int n) {
	Mat bit_image = Mat(image.rows, image.cols, CV_8UC1);

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int byte;
			n == 0 ? byte = 0: byte = (int)pow(2, n - 1); //製作遮罩byte
			//以&作為遮罩取出bit plane，但此時範圍為0與2^(n-1)
			//通過除以byte歸一化，接著拉伸到0和255方便觀看
			bit_image.at<uchar>(i, j) = (image.at<uchar>(i,j) & byte)*((float)255/(byte));
		}
	}

	return bit_image;
}
Mat merge_plane(Mat lena, Mat baboon) {
	int height = lena.cols, width = lena.cols;
	//lena為前四張，baboon為後4張，bit數越高對影像整體的影響越高
	int sort_lena[8] = {8,7,6,5};
	int sort_baboon[8] = {4,3,2,1};
	Mat lena_merge = Mat(height, width, CV_8UC1);
	Mat baboon_merge = Mat(height, width, CV_8UC1);
	Mat merge_image = Mat(height, width, CV_8UC1);

	//將 三個圖像初始為0
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			lena_merge.at<uchar>(i, j) = 0;
			baboon_merge.at<uchar>(i, j) = 0;
			merge_image.at<uchar>(i, j) = 0;
		}
	}

	for (int i = 0; i < 8; i++) {
		//merge前，同樣需要將bit plane還原成0~2^n
		//sort_lena與sort_baboon創建矩陣只定義前4個值，後4個值為0，不會有任何動作
		lena_merge += bit_plane(lena, 8-i ) / 255 * (int)(pow(2, sort_lena[i]-1));
		baboon_merge += bit_plane(baboon, 8-i) / 255 * (int)(pow(2, sort_baboon[i]-1));
	}
	merge_image = lena_merge + baboon_merge;

	printf("\nlena與baboon的合成圖\n");
	namedWindow("lenaFile", 0); //視窗名
	resizeWindow("lenaFile", 256, 256); //視窗大小
	imshow("lenaFile", merge_image);//display Image	
	waitKey(0);

	return merge_image;
}
Mat_2 splite_plane(Mat merge) {
	int height = merge.cols, width = merge.cols;
	int sort_lena[8] = { 8,7,6,5 };
	int sort_baboon[8] = { 4,3,2,1 };
	Mat lena_splite = Mat(height, width, CV_8UC1);
	Mat baboon_splite = Mat(height, width, CV_8UC1);

	//將 三個圖像初始為0
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			lena_splite.at<uchar>(i, j) = 0;
			baboon_splite.at<uchar>(i, j) = 0;
		}
	}

	//將圖像的前四個與後四個bit plane都放滿，盡可能還原成原圖
	for (int i = 0; i < 4; i++) {
		lena_splite += bit_plane(merge, sort_lena[i]) / 255 * (int)(pow(2, 7 - i));
		baboon_splite += bit_plane(merge, sort_baboon[i]) / 255 * (int)(pow(2, 7 - i ));
	}
	for (int i = 0; i < 4; i++) {
		lena_splite += bit_plane(merge, sort_lena[i]) / 255 * (int)(pow(2, 3 - i));
		baboon_splite += bit_plane(merge, sort_baboon[i]) / 255 * (int)(pow(2, 3 - i));
	}

	struct Mat_2 image2 = { lena_splite,baboon_splite };
	

	printf("\n合成圖拆開的lena\n");
	namedWindow("lenaFile", 0); //視窗名
	resizeWindow("lenaFile", 256, 256); //視窗大小
	imshow("lenaFile", lena_splite);//display Image	

	printf("\n合成圖拆開的baboon\n");
	namedWindow("lenaFile2", 0); //視窗名
	resizeWindow("lenaFile2", 256, 256); //視窗大小
	imshow("lenaFile2", baboon_splite);//display Image	

	return image2;

}
double mse_function(Mat original, Mat image) {
	int height = original.rows;
	int width = original.cols;
	//---MSE---//
	double squE = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			squE = squE + pow(original.at<uchar>(i, j) - image.at<uchar>(i, j), 2);
	double mean_squE = squE / (width * height);
	
	return mean_squE;
}