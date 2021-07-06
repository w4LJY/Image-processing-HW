#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;

Mat fft_func(Mat);
Mat ifft_func(Mat);
Mat ifft_show(Mat);
Mat Bandreject_func(Mat, float, float);

int main7_1b()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char kirbyF[] = "kirby512.raw";
	FILE* kirbyFile;
	kirbyFile = fopen(kirbyF, "rb");
	unsigned char* kirbyi = new unsigned char[size];
	fread(kirbyi, 1, size, kirbyFile);
	Mat kirby = Mat(height, width, CV_8UC1, kirbyi);
	//---------------------star----------------------//
	Mat result = Bandreject_func(kirby, 81, 20);
	imshow("result", result);


	waitKey(0);


	return 0;
}
Mat fft_func(Mat I) {
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
Mat ifft_show(Mat I) {
	Mat planes[] = { Mat_<float>(I), Mat::zeros(I.size(), CV_32FC1) }; //分為實部與虛部
	split(I, planes);  //分離通道， planes[0] 為實數部分，planes[1]為虛數部分

	Mat mag;
	magnitude(planes[0], planes[1], mag);//將傅立葉結果由複數轉幅值
	mag += Scalar::all(1);
	log(mag, mag); //方便觀看的對數尺度變化
	normalize(mag, mag, 0, 1, NORM_MINMAX); //規範化值到 0~1 顯示圖片的需要

	return mag;
}
Mat ifft_func(Mat I) {
	Mat ifft;

	idft(I, ifft, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	//DC點位移
	for (int i = 0; i < I.rows; i++)
		for (int j = 0; j < I.cols; j++)
			ifft.at<float>(i, j) = ifft.at<float>(i, j) * pow(-1, i + j);

	return ifft;
}
Mat Bandreject_func(Mat I, float D0, float w) {
	int H = I.rows, W = I.cols;

	//dft
	Mat dftI = fft_func(I);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			float D = sqrt(pow(i - H / 2, 2) + pow(j - W / 2, 2));//找到與dc點的距離
			dftI.at<Vec2f>(i, j)[0] = D >= (D0 - w / 2) && D <= (D0 + w / 2) ? 0 : dftI.at<Vec2f>(i, j)[0];
			dftI.at<Vec2f>(i, j)[1] = D >= (D0 - w / 2) && D <= (D0 + w / 2) ? 0 : dftI.at<Vec2f>(i, j)[1];
		}
	}

	//show bandreject後的dft
	Mat mag = ifft_show(dftI);
	imshow("spectrum", mag);

	//idft
	Mat idftI = ifft_func(dftI);
	idftI.convertTo(idftI, CV_8UC1);

	return idftI;
}
