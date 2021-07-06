#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

void Image_Negatives(Mat); //�t��
Mat merge_plane(Mat*); //�N8��bit�ƧǻP�B�z��X��


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
	printf("\n8bit plane�ץ��᪺�X����\n");
	Mat merge_image = merge_plane(image);

	namedWindow("lenaFile", 0); //�����W
	resizeWindow("lenaFile", height, width); //�����j�p
	imshow("lenaFile", merge_image);//display Image	
	waitKey(0);


	return 0;
}
//�N���t���ĪG���v���٭�
void Image_Negatives(Mat image) {
	int L = 256;
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			image.at<uchar>(i, j) = L - 1 - r;
		}
	}
}
//�N8��bit plane�ھ��[��o�쪺���Ƕi���٭�
Mat merge_plane(Mat* image) {
	int height = image[0].cols, width = image[0].cols;
	Mat merge_image = Mat(height, width, CV_8UC1);
	//�N�Ϲ���l��0
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			merge_image.at<uchar>(i, j) = 0;

	//���T����
	int sort[8] = {5,3,4,0,2,7,6,1};
	
	//�N�t������^��
	Image_Negatives(image[3]);
	Image_Negatives(image[4]);
	Image_Negatives(image[7]);

	for (int i = 0; i < 8; i++) 
		merge_image += image[sort[i]] / 255 * (int)(pow(2, i));

	return merge_image;
}
