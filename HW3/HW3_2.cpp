#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

void Image_Negatives(Mat); //負片
Mat merge_plane(Mat*); //將8個bit排序與處理後合成


int main3_2()
{
	int width = 512;
	int height = 512;
	int size = width * height;

	char aF[] = "img/a512x512.raw";
	char bF[] = "img/b512x512.raw";
	char cF[] = "img/c512x512.raw";
	char dF[] = "img/d512x512.raw";
	char eF[] = "img/e512x512.raw";
	char fF[] = "img/f512x512.raw";
	char gF[] = "img/g512x512.raw";
	char hF[] = "img/h512x512.raw";

	FILE* File[8];
	File[0] = fopen(aF, "rb");
	File[1] = fopen(bF, "rb");
	File[2] = fopen(cF, "rb");
	File[3] = fopen(dF, "rb");
	File[4] = fopen(eF, "rb");
	File[5] = fopen(fF, "rb");
	File[6] = fopen(gF, "rb");
	File[7] = fopen(hF, "rb");

	unsigned char* imagei[8];
	Mat image[8];
	for (int i = 0; i < 8; i++) {
		imagei[i] = new unsigned char[size];
		fread(imagei[i], 1, size, File[i]);
		image[i] = Mat(height, width, CV_8UC1, imagei[i]);
	}
	//---------start----------//
	printf("\n8bit plane修正後的合成圖\n");
	Mat merge_image = merge_plane(image);

	namedWindow("lenaFile", 0); //視窗名
	resizeWindow("lenaFile", height, width); //視窗大小
	imshow("lenaFile", merge_image);//display Image	
	waitKey(0);


	return 0;
}
//將有負片效果的影像還原
void Image_Negatives(Mat image) {
	int L = 256;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			image.at<uchar>(i, j) = L - 1 - r;
		}
	}
}
//將8個bit plane根據觀察得到的順序進行還原
Mat merge_plane(Mat* image) {
	int height = image[0].cols, width = image[0].cols;
	Mat merge_image = Mat(height, width, CV_8UC1);
	//將圖像初始為0
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			merge_image.at<uchar>(i, j) = 0;

	//正確順序
	int sort[8] = {5,3,4,0,2,7,6,1};
	
	//將負片圖轉回來
	Image_Negatives(image[3]);
	Image_Negatives(image[4]);
	Image_Negatives(image[7]);

	for (int i = 0; i < 8; i++) 
		merge_image += image[sort[i]] / 255 * (int)(pow(2, i));

	return merge_image;
}
