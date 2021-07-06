#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;
#define PI 3.14

Mat fft_func3(Mat);
Mat ifft_func3(Mat);
Mat ifft_show3(Mat);
Mat inverse_func(Mat);
Mat Wiener_func(Mat);

int main7_2()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char flowerF[] = "motion_flower.raw";
	FILE* flowerFile;
	flowerFile = fopen(flowerF, "rb");
	unsigned char* floweri = new unsigned char[size];
	fread(floweri, 1, size, flowerFile);
	Mat flower = Mat(height, width, CV_8UC1, floweri);
	//---------------------star----------------------//
	imshow("original", flower);

	Mat result = inverse_func(flower);
	imshow("inverse result", result);
	Mat result2 = Wiener_func(flower);
	imshow("Wiener result", result2);




	waitKey(0);


	return 0;
}
Mat fft_func3(Mat I) {
	//圖像轉換為浮點數//
	Mat complexImg;
	I.convertTo(I, CV_32FC1);

	//DC點位移
	for (int i = 0; i < I.rows; i++)
		for (int j = 0; j < I.cols; j++)
			I.at<float>(i, j) = I.at<float>(i, j) * pow(-1, i + j);

	//快速傅立葉轉換//
	dft(I, complexImg, DFT_COMPLEX_OUTPUT); // FFT變換

	return complexImg;
}
Mat ifft_show3(Mat I) {
	Mat planes[] = { Mat_<float>(I), Mat::zeros(I.size(), CV_32FC1) }; //分為實部與虛部
	split(I, planes);  //分離通道， planes[0] 為實數部分，planes[1]為虛數部分

	Mat mag;
	magnitude(planes[0], planes[1], mag);//將傅立葉結果由複數轉幅值
	//mag += Scalar::all(1);
	log(1 + mag, mag); //方便觀看的對數尺度變化
	normalize(mag, mag, 0, 1, NORM_MINMAX); //規範化值到 0~1 顯示圖片的需要

	return mag;
}
Mat ifft_func3(Mat I) {
	Mat ifft;

	idft(I, ifft, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	//DC點位移
	for (int i = 0; i < I.rows; i++) {
		for (int j = 0; j < I.cols; j++) {
			ifft.at<float>(i, j) = ifft.at<float>(i, j) * pow(-1, i + j);
		}
	}
	return ifft;
}
Mat inverse_func(Mat I) {
	int H = I.rows, W = I.cols;

	//dft
	Mat dftI = fft_func3(I);

	float D0 = 100;
	int n = 4000;
	for (int i = 0; i < H; i++) {
		for (int j = 1; j < W; j++) {
			float D = sqrt(pow(i - H / 2, 2) + pow(j - W / 2, 2));//找到與dc點的距離
			if (D <= D0) {
				//realH
				double Hreal = sin(0.02 * PI * j) / (0.02 * PI * j);
				//imagH
				double Himag = (1 - cos(0.02 * PI * j)) / (0.02 * PI * j);

				double Freal = (dftI.at<Vec2f>(i, j)[0] * Hreal + dftI.at<Vec2f>(i, j)[1] * Himag) / (pow(Hreal, 2) + pow(Himag, 2));
				double Fimag = (dftI.at<Vec2f>(i, j)[1] * Hreal - dftI.at<Vec2f>(i, j)[0] * Himag) / (pow(Hreal, 2) + pow(Himag, 2));

				if (!(_isnan(Freal) || abs(Freal) > n))
					dftI.at<Vec2f>(i, j)[0] = Freal;

				if (!(_isnan(Fimag) || abs(Fimag) > n))
					dftI.at<Vec2f>(i, j)[1] = Fimag;


			}
		}
	}

	//show inverse後的dft
	Mat mag = ifft_show3(dftI);
	imshow("Inverse spectrum", mag);

	//idft
	Mat idftI = ifft_func3(dftI);
	idftI.convertTo(idftI, CV_8UC1);

	return idftI;
}
Mat Wiener_func(Mat I) {
	int H = I.rows, W = I.cols;


	//dft
	Mat dftI = fft_func3(I);


	int n = 4000;
	float K = 0.005;
	for (int i = 0; i < H; i++) {
		for (int j = 1; j < W; j++) {
			float D = sqrt(pow(i - H / 2, 2) + pow(j - W / 2, 2));//找到與dc點的距離
			//real
			double Hreal = sin(0.02 * PI * j) / (0.02 * PI * j);
			//imag
			double Himag = (1 - cos(0.02 * PI * j)) / (0.02 * PI * j);

			double Freal = (dftI.at<Vec2f>(i, j)[0] * Hreal + dftI.at<Vec2f>(i, j)[1] * Himag) / (pow(Hreal, 2) + pow(Himag, 2)) * ((Hreal * Hreal + Himag * Himag) / (Hreal * Hreal + Himag * Himag + K));
			double Fimag = (dftI.at<Vec2f>(i, j)[1] * Hreal - dftI.at<Vec2f>(i, j)[0] * Himag) / (pow(Hreal, 2) + pow(Himag, 2)) * ((Hreal * Hreal + Himag * Himag) / (Hreal * Hreal + Himag * Himag + K));

			if (!(_isnan(Freal) || abs(Freal) > n))
				dftI.at<Vec2f>(i, j)[0] = Freal;

			if (!(_isnan(Fimag) || abs(Fimag) > n))
				dftI.at<Vec2f>(i, j)[1] = Fimag;

		}
	}


	//show Wiener後的dft
	Mat mag = ifft_show3(dftI);
	imshow("Wiener spectrum", mag);

	//idft
	Mat idftI = ifft_func3(dftI);
	idftI.convertTo(idftI, CV_8UC1);

	return idftI;
}

