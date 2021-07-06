#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;
#define PI 3.14159265358979323846

Mat fft_func2(Mat);
Mat ifft_func2(Mat);
Mat ifft_show2(Mat);
Mat idealH_func(Mat, float);
void mse_psnr2(Mat, Mat);

int main6_2a()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char houseF[] = "aerialcity512.raw";
	FILE* aerialcityFile;
	aerialcityFile = fopen(houseF, "rb");
	unsigned char* aerialcityi = new unsigned char[size];
	fread(aerialcityi, 1, size, aerialcityFile);
	Mat aerialcity = Mat(height, width, CV_8UC1, aerialcityi);
	//---------------------star----------------------//

	Mat result5 = idealH_func(aerialcity, 5);
	imshow("ideal LPF D0=5", result5);
	printf("ideal LPF D0=5:");
	mse_psnr2(result5, aerialcity);
	waitKey(0);

	Mat result25 = idealH_func(aerialcity, 25);
	imshow("ideal LPF D0=25", result25);
	printf("ideal LPF D0=25:");
	mse_psnr2(result25, aerialcity);
	waitKey(0);

	Mat result125 = idealH_func(aerialcity, 125);
	imshow("ideal LPF D0=125", result125);
	printf("ideal LPF D0=125:");
	mse_psnr2(result125, aerialcity);
	waitKey(0);


	return 0;
}
Mat fft_func2(Mat I) {
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
Mat ifft_show2(Mat I) {
	Mat planes[] = { Mat_<float>(I), Mat::zeros(I.size(), CV_32FC1) }; //分為實部與虛部
	split(I, planes);  //分離通道， planes[0] 為實數部分，planes[1]為虛數部分

	Mat mag;
	magnitude(planes[0], planes[1], mag);//將傅立葉結果由複數轉幅值
	mag += Scalar::all(1);
	log(mag, mag); //方便觀看的對數尺度變化
	normalize(mag, mag, 0, 1, NORM_MINMAX); //規範化值到 0~1 顯示圖片的需要

	return mag;
}
Mat ifft_func2(Mat I) {
	Mat ifft;

	idft(I, ifft, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	//DC點位移
	for (int i = 0; i < I.rows; i++)
		for (int j = 0; j < I.cols; j++)
			ifft.at<float>(i, j) = ifft.at<float>(i, j) * pow(-1, i + j);

	return ifft;
}
Mat idealH_func(Mat I, float D0) {
	int H = I.rows, W = I.cols;
	Mat logI = Mat(I.rows, I.cols, CV_32FC1);

	//dft
	Mat dftI = fft_func2(I);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			float D = sqrt(pow(i - H / 2, 2) + pow(j - W / 2, 2));//找到與dc點的距離
			dftI.at<Vec2f>(i, j)[0] = D >= D0 ? 0 : dftI.at<Vec2f>(i, j)[0];
			dftI.at<Vec2f>(i, j)[1] = D >= D0 ? 0 : dftI.at<Vec2f>(i, j)[1];
		}
	}
	
	//show ideal後的dft
	Mat mag = ifft_show2(dftI);
	imshow("magnitude spectrum", mag);

	//idft
	Mat idftI = ifft_func2(dftI);

	idftI.convertTo(idftI, CV_8UC1);

	return idftI;
}
void mse_psnr2(Mat I, Mat original) {
	int H = I.rows, W = I.cols;
	double MSE = 0;
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			MSE += pow(I.at<uchar>(i, j) - original.at<uchar>(i, j), 2);
	MSE = MSE / (H * W);

	double PSNR = 20 * log10(255 / sqrt(MSE));

	printf("MSE:%f,PSNR:%f\n", MSE, PSNR);
}