#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;


int main2_1()
{
	int width = 256;
	int height = 256;
	int size = width * height;
	char OriFile[] = "img/lena_256.raw";  

	FILE* lenaFile;
	lenaFile = fopen(OriFile, "rb");

	unsigned char* lenai = new unsigned char[size];
	fread(lenai, 1, size, lenaFile);

	//-----------------------star-----------------------//
	//Mat type
	Mat lenaMat = Mat(height, width, CV_8UC1, lenai); //unsign 8bit channel 1(灰階)

	//將眼睛的ROI區域切出來
	int eyes[4] = { 140,120,184,120 }; //手動取得眼睛座標的rows範圍、cols範圍
	Mat lena_eyes = Mat(eyes[0]-eyes[1], eyes[2]-eyes[3], CV_8UC1);
	for (int i = 0; i < lenaMat.rows; i++)
	{
		for (int j = 0; j < lenaMat.cols; j++)
		{
			if (i < eyes[0] && i >= eyes[1] && j < eyes[2] && j >= eyes[3])
			{
				lena_eyes.data[(i - eyes[1])*lena_eyes.cols+( j - eyes[3])] = lenaMat.data[i * lenaMat.cols + j];
			}

		}
	}
	//將眼睛部分放大兩倍
	Mat lena_eyesX2 = Mat(lena_eyes.rows * 2, lena_eyes.cols * 2, CV_8UC1);

	//採用重複填四次(2x2)的方式放大兩倍
	for (int i = 0; i < lena_eyes.rows; i++)
	{
		for (int j = 0; j < lena_eyes.cols; j++)
		{
			lena_eyesX2.data[(i * 2) * lena_eyesX2.cols + (j * 2)] = lena_eyes.data[i*lena_eyes.cols+ j];
			lena_eyesX2.data[(i * 2) * lena_eyesX2.cols + (j * 2 + 1)] = lena_eyes.data[i * lena_eyes.cols + j];
			lena_eyesX2.data[(i * 2 + 1)* lena_eyesX2.cols + (j * 2)] = lena_eyes.data[i * lena_eyes.cols + j];
			lena_eyesX2.data[(i * 2 + 1)* lena_eyesX2.cols + (j * 2 + 1)] = lena_eyes.data[i * lena_eyes.cols + j];
		}
	}

	//將眼睛部份放回原圖lena
	int eyesX2_star[2] = {(eyes[0]+eyes[1])/2-lena_eyesX2.rows/2,(eyes[2]+eyes[3])/2-lena_eyesX2.cols/2}; //根據原來眼睛的中心位置與變換後的寬高，計算出放大眼睛的起始位置
	
	for (int i = 0; i < lena_eyesX2.rows; i++)
	{
		for (int j = 0; j < lena_eyesX2.cols; j++)
		{
			lenaMat.data[(i+eyesX2_star[0])*lenaMat.cols+ j+eyesX2_star[1]] = lena_eyesX2.data[i*lena_eyesX2.cols+ j];
		}
	}

	//將完成的圖縮小兩倍
	Mat lenaMat_d2 = Mat(lenaMat.rows / 2, lenaMat.cols / 2, CV_8UC1);
	//縮小採直接捨去pixel
	for (int i = 0; i < lenaMat.rows; i++)
	{
		for (int j = 0; j < lenaMat.cols; j++)
		{
			if (i % 2 == 0 && j % 2 == 0)
				lenaMat_d2.data[i / 2*lenaMat_d2.cols + j / 2] = lenaMat.data[i*lenaMat.cols+ j]; 
		}
	}
	imshow("lenaFile", lenaMat);//display Image	
	imshow("lenaFILE D2", lenaMat_d2);
	waitKey(0);

	fclose(lenaFile);
	delete lenai;
	destroyWindow("lenaFile");

	return 0;
}