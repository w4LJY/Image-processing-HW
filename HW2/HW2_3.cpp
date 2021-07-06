#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

#include<math.h>
#include<cmath>

using namespace cv;

Mat isopreference_Glevel(Mat,Mat,int);
Mat bicubic2(Mat, float); //��j�Ϥ�,�Ѽ�(��J�Ϥ��A��j���v)
void bicubic_function2(float w_x[4], float w_y[4], float u, float v); //bicubic���

int main2_3()
{
	int width = 256;
	int height = 256;
	int size = width * height;
	char lenaFile[] = "img/lena_256.raw";

	FILE* lenaF;
	lenaF = fopen(lenaFile, "rb");

	unsigned char* lenai = new unsigned char[size];

	fread(lenai, 1, size, lenaF);

	/*-----------------------------------------star-----------------------------------*/
	Mat lenaMat = Mat(height, width, CV_8UC1, lenai); //unsign 8bit channel 1(�Ƕ�)

	Mat lenaMat3 = bicubic2(lenaMat, 3);
	printf("\n�������Ƕ��ѪR�׹��Ϲ��j�p���v�T�A������ת��v�T�bmain2_2\n");
	int star = 1;
	while (star) {
		int input;
		printf("\n��J�q��bit��1~8\n�Ʀr0:�h�^�D�����\n");
		scanf("%d", &input);
		if (input == 0)
		{
			star = 0;
			continue;
		}
		if (input <= 0 || input > 8) {
			printf("\n�Э��s��J");
			continue;
		}

		Mat lenaMat_level_1 = isopreference_Glevel(lenaMat,lenaMat3,input);
	}

	fclose(lenaF);
	delete lenai;
	destroyWindow("lenaF");

	return 0;
}
//�q�ưt�X�Y��A�ҩ�Isopreference test�o�ز{�H���s�b
Mat isopreference_Glevel(Mat lenaMat,Mat lenaMat3, int level) {
	Mat lenaMat_level = Mat(lenaMat.rows, lenaMat.cols, CV_8UC1);
	Mat lenaMat_level3 = Mat(lenaMat3.rows, lenaMat3.cols, CV_8UC1);
	for (int i = 0; i < lenaMat.rows; i++)
	{
		for (int j = 0; j < lenaMat.cols; j++)
		{
			//lena
			lenaMat_level.at<uchar>(i, j) = round(lenaMat.at<uchar>(i, j) * (float)(pow(2, level) - 1) / 255);
			lenaMat_level.at<uchar>(i, j) = round(lenaMat_level.at<uchar>(i, j) / (float)(pow(2, level) - 1) * 255);
		}
	}
	for (int i = 0; i < lenaMat3.rows; i++)
	{
		for (int j = 0; j < lenaMat3.cols; j++)
		{
			//lena zoom 4
			lenaMat_level3.at<uchar>(i, j) = round(lenaMat3.at<uchar>(i, j) * (float)(pow(2, level) - 1) / 255);
			lenaMat_level3.at<uchar>(i, j) = round(lenaMat_level3.at<uchar>(i, j) / (float)(pow(2, level) - 1) * 255);
		}
	}

	printf("\n�q��lena_256.raw ���G\n");
	namedWindow("lenadown", 0); //�����W
	resizeWindow("lenadown", lenaMat_level.rows, lenaMat_level.cols); //�����j�p
	moveWindow("lenadown", 500, 100); //�N�������W����b�ù����w��m
	imshow("lenadown", lenaMat_level);//display Image	

	printf("\n�q��4��lena_256.raw ���G\n");
	namedWindow("lenaMat_level4", 0); //�����W
	resizeWindow("lenaMat_level4", lenaMat_level3.rows, lenaMat_level3.cols); //�����j�p
	moveWindow("lenaMat_level4", 1000, 100); //�N�������W����b�ù����w��m
	imshow("lenaMat_level4", lenaMat_level3);//display Image

	waitKey(0);
	return lenaMat_level;
}
Mat bicubic2(Mat lenaMat, float times) {
	int height = lenaMat.rows;
	int width = lenaMat.cols;

	Mat lenaMatup = Mat(height * times, width * times, CV_8UC1);

	for (int i = ceil(times); i < lenaMatup.rows - (ceil(times)) * 2; i++)
	{
		for (int j = ceil(times); j < lenaMatup.cols - (ceil(times)) * 2; j++)
		{
			int x = i / times;
			int y = j / times;

			//X,Y�@�w�n>=1
			int X = (int)x;
			int Y = (int)y;

			float u = x - X;
			float v = y - Y;

			float Wx[4], Wy[4];
			bicubic_function2(Wx, Wy, u, v);

			lenaMatup.at<uchar>(i, j) = 0;
			for (int a = 0; a < 4; a++) {
				for (int b = 0; b < 4; b++)
				{
					lenaMatup.at<uchar>(i, j) += lenaMat.at<uchar>(X - 1 + a, Y - 1 + b) * Wx[a] * Wy[b];
				}
			}
		}
	}
	//namedWindow("UP_bicubic", 0); //�����W
	//resizeWindow("UP_bicubic", lenaMatup.rows, lenaMatup.cols); //�����j�p
	////moveWindow("UP_bicubic", 100, 100); //�N�������W����b�ù����w��m
	//imshow("UP_bicubic", lenaMatup);//display Image

	return lenaMatup;
}
void bicubic_function2(float Wx[4], float Wy[4], float u, float v) {
	float a = -0.5;
	//----for abs(x)<=1----//
	//u|v
	Wx[1] = (a + 2) * abs(pow(u, 3)) - (a + 3) * pow(u, 2) + 1;
	Wy[1] = (a + 2) * abs(pow(v, 3)) - (a + 3) * pow(v, 2) + 1;
	//1-u|v
	Wx[2] = (a + 2) * abs(pow(1 - u, 3)) - (a + 3) * pow(1 - u, 2) + 1;
	Wy[2] = (a + 2) * abs(pow(1 - v, 3)) - (a + 3) * pow(1 - v, 2) + 1;

	//---for 1<=abs(x)<=2---//
	//u|v+1
	Wx[0] = a * abs(pow(u + 1, 3)) - 5 * a * pow(u + 1, 2) + 8 * a * abs(u + 1) - 4 * a;
	Wy[0] = a * abs(pow(v + 1, 3)) - 5 * a * pow(v + 1, 2) + 8 * a * abs(v + 1) - 4 * a;
	//2-u|v
	Wx[3] = a * abs(pow(2 - u, 3)) - 5 * a * pow(2 - u, 2) + 8 * a * abs(2 - u) - 4 * a;
	Wy[3] = a * abs(pow(2 - v, 3)) - 5 * a * pow(2 - v, 2) + 8 * a * abs(2 - v) - 4 * a;
}
