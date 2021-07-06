#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

#include<math.h>
#include<cmath>

using namespace cv;

Mat Glevel(Mat,Mat,int);
int main2_2()
{
	int width = 256;
	int height = 256;
	int size = width * height;
	char lenaFile[] = "img/lena_256.raw";
	char babooFile[] = "img/baboon_256.raw"; 

	FILE* lenaF,* babooF;
	lenaF = fopen(lenaFile, "rb");
	babooF = fopen(babooFile, "rb");

	unsigned char* lenai = new unsigned char[size];
	unsigned char* babooi = new unsigned char[size];

	fread(lenai, 1, size, lenaF);
	fread(babooi, 1, size, babooF);

	/*-----------------------------------------star-----------------------------------*/
	Mat lenaMat = Mat(height, width, CV_8UC1, lenai); //unsign 8bit channel 1(灰階)
	Mat babooMat = Mat(height, width, CV_8UC1, babooi);
	int star = 1;
	while (star) {
		int input;
		printf("\n輸入量化bit數1~8\n數字0:退回題號選擇\n");
		scanf("%d", &input);
		if (input == 0)
		{
			star = 0;
			continue;
		}
		if (input <= 0 || input > 8) {
			printf("\n請重新輸入");
			continue;
		}

		Mat lenaMat_level_1 = Glevel(lenaMat,babooMat, input);
	}
	fclose(lenaF);
	delete lenai;
	destroyWindow("lenaF");

	return 0;
}
//根據level為多少bit，決定量化後的結果，1~7bit，8bit是原圖(2^n)
//HW3有另一種方法能夠達成
Mat Glevel(Mat lenaMat,Mat babooMat, int level) {
	Mat lenaMat_level = Mat(lenaMat.rows, lenaMat.cols, CV_8UC1);
	Mat babooMat_level = Mat(babooMat.rows, babooMat.cols, CV_8UC1);
	
	for (int i = 0; i < lenaMat.rows; i++)
	{
		for (int j = 0; j < lenaMat.cols; j++)
		{
			//採用壓縮後還原的方式，去除掉損失的pixel就是量化結果
			//lena
			lenaMat_level.at<uchar>(i, j) = round(lenaMat.at<uchar>(i, j) * (float)(pow(2,level)-1) / 255);
			lenaMat_level.at<uchar>(i, j) = round(lenaMat_level.at<uchar>(i, j)  / (float)(pow(2, level)-1) * 255);
			//baboon
			babooMat_level.at<uchar>(i, j) = round(babooMat.at<uchar>(i, j) * (float)(pow(2, level) - 1) / 255);
			babooMat_level.at<uchar>(i, j) = round(babooMat_level.at<uchar>(i, j) / (float)(pow(2, level) - 1) * 255);
		}
	}
	printf("");
	//---MSE---//
	//每個pixel的誤差平方和除以總pixel做平均，越小越好
	double squE = 0;
	double squE2 = 0;
	for (int i = 0; i < lenaMat.rows; i++)
	{
		for (int j = 0; j < lenaMat.cols; j++)
		{
			squE=squE+pow(lenaMat.at<uchar>(i, j) - lenaMat_level.at<uchar>(i, j), 2);
			squE2 = squE2+ pow(babooMat.at<uchar>(i, j) - babooMat_level.at<uchar>(i, j), 2);
		}
	}
	double mean_squE = squE / (lenaMat.rows * lenaMat.cols);
	double mean_squE2 = squE2 / (babooMat.rows * babooMat.cols);

	//---PSNR---//
	//根據圖像灰階值範圍與MSE能夠得到，越大越好
	double PSNR = 20 * log10(255 / sqrt(mean_squE));
	double PSNR2 = 20 * log10(255 / sqrt(mean_squE2));
	
	printf("\n量化lena_256.raw 結果\n");
	printf("MSE:%f\n", mean_squE);
	printf("PSNR:%f\n ", PSNR);
	namedWindow("lenadown", 0); //視窗名
	resizeWindow("lenadown", lenaMat_level.rows, lenaMat_level.cols); //視窗大小
	moveWindow("lenadown", 500, 100); //將視窗左上角放在螢幕指定位置
	imshow("lenadown", lenaMat_level);//display Image	

	printf("\n量化baboon_256.raw 結果\n");
	printf("MSE:%f\n", mean_squE2);
	printf("PSNR:%f\n ", PSNR2);
	namedWindow("baboodown", 0); //視窗名
	resizeWindow("baboodown", babooMat_level.rows, babooMat_level.cols); //視窗大小
	moveWindow("baboodown", 1000, 100); //將視窗左上角放在螢幕指定位置
	imshow("baboodown", babooMat_level);//display Image	

	waitKey(0);

	return lenaMat_level;
}
