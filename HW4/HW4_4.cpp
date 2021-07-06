#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>

#define PI 3.14159265359
using namespace cv;

double** ge_matrix(int); //取得二維動態記憶體
void re_matrix(double**,int); //釋放二維動態記憶體
Mat gaussianfilter(Mat, int, double); //gaussian濾波 輸入影像，大小，sigma
Mat DoG(Mat, int, double, double); //difference of Gaussians
Mat mi_padd(Mat, int); //鏡射補值

int main4_4()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char turtleF[] = "img/turtle512.raw";
	FILE* turtleFile;
	turtleFile = fopen(turtleF, "rb");
	unsigned char* turtlei = new unsigned char[size];
	fread(turtlei, 1, size, turtleFile);
	Mat turtle = Mat(height, width, CV_8UC1, turtlei);
	//---------------------star----------------------//

	//----第一小題 高斯濾波----//
	//gaussian filter sigma = 0.8 size=5x5
	printf("gaussian filter sigma=0.8\n");
	Mat G_08 = gaussianfilter(turtle, 5, 0.8);
	namedWindow("G_08", 0);
	resizeWindow("G_08", height, width);
	imshow("G_08", G_08);

	//gaussian filter sigma = 1.3 size=5x5
	printf("gaussian filter sigma=1.3\n");
	Mat G_13 = gaussianfilter(turtle, 5, 1.3);
	namedWindow("G_13", 0);
	resizeWindow("G_13", height, width);
	imshow("G_13", G_13);

	//gaussian filter sigma = 2 size=5x5
	printf("gaussian filter sigma=2\n");
	Mat G_20 = gaussianfilter(turtle, 5, 2);
	namedWindow("G_20", 0);
	resizeWindow("G_20", height, width);
	imshow("G_20", G_20);
	waitKey(0);
	//----第二小題 DoG----//
	//DoG sigma1=0.5 sigma2=1.5 size=5x5
	printf("DoG結果\n");
	Mat image_DoG = DoG(turtle, 5, 0.5, 1.5);
	namedWindow("image_DoG", 0);
	resizeWindow("image_DoG", height, width);
	imshow("image_DoG", image_DoG);



	waitKey(0);

	return 0;
}
double** ge_matrix(int size) {
	double** ptr;
	ptr = (double**)malloc(sizeof(double*) * size);
	for (int i = 0; i < size; i++)
		ptr[i] = (double*)malloc(sizeof(double) * size);
	return ptr;
}
void re_matrix(double** ptr,int size) {
	for (int i = 0; i < size; i++)
		free(ptr[i]);
	free(ptr);
}
Mat gaussianfilter(Mat image, int size, double sigma) {

	//生成高斯filter
	double** Gfilter = ge_matrix(size); //存儲高斯filter
	double Gsum = 0.0; //存儲高斯filter的總和
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			//根據Gaussian的方程式產生的filter
			Gfilter[i][j] = (1 / (2 * PI * pow(sigma, 2))) * exp(-((pow(i - size / 2, 2) + (pow(j - size / 2, 2))) / (2 * pow(sigma, 2))));
			Gsum += Gfilter[i][j];
		}
	}
	//開始進行高斯濾波
	Mat image_p = mi_padd(image, size); //鏡射補值

	int height = image.rows, width = image.cols;
	int M_half = (size - 1) / 2;

	Mat image_conv = Mat(height, width, CV_8UC1);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double conv_sum = 0.0;
			for (int a = i - M_half; a <= i + M_half; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					int x = a + M_half - i;
					int y = b + M_half - j;
					conv_sum += image_p.at<uchar>(a + M_half, b + M_half) * (Gfilter[x][y] / Gsum); //高斯卷積
				}
			}
			image_conv.at<uchar>(i, j) = max(min((int)conv_sum, 255), 0);
		}
	}
	re_matrix(Gfilter,size);

	return image_conv;
}
//將小的sigmal得到的Gaussian濾波結果減去大的sigmal得到的Gaussian濾波結果能得到具高頻邊緣的邊緣
Mat DoG(Mat image, int size, double g1, double g2) {
	int height = image.rows, width = image.cols;
	Mat image_DoG = Mat(height, width, CV_8UC1);
	Mat G_g1 = gaussianfilter(image, size, g1);
	Mat G_g2 = gaussianfilter(image, size, g2);
	
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image_DoG.at<uchar>(i, j) = max(min(G_g1.at<uchar>(i, j) - G_g2.at<uchar>(i, j),255),0); //高斯濾波sigma0.5 - sigma1.5
		}
	}
	imwrite("out/Dog.png", image_DoG);
	return image_DoG;
}
Mat mi_padd(Mat image, int size) {
	int height = image.rows, width = image.cols;
	int s_half = (size - 1) / 2;
	Mat image_pad(height + 2 * s_half, width + 2 * s_half, CV_8UC1);
	int p_height = image_pad.rows, p_width = image_pad.cols;

	for (int i = 0; i < p_height; i++) {
		for (int j = 0; j < p_width; j++) {
			int a = i < s_half;
			int b = i > p_height - s_half - 1;
			int c = j < s_half;
			int d = j > p_width - s_half - 1;
			image_pad.at<uchar>(i, j) = image.at<uchar>(abs(s_half - i + (2 * height - 1) * b) - a * 1, abs(s_half - j + (2 * width - 1) * d) - c * 1);
		}
	}
	return image_pad;
}

