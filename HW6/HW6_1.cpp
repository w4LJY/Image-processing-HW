#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;
#define PI 3.14159265358979323846

Mat fft_func(Mat);
Mat ifft_func(Mat);
Mat ifft_show(Mat);
Mat HomomorphicFilter(Mat);

int main6_1()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char houseF[] = "house512.raw";
	FILE* houseFile;
	houseFile = fopen(houseF, "rb");
	unsigned char* housei = new unsigned char[size];
	fread(housei, 1, size, houseFile);
	Mat house = Mat(height, width, CV_8UC1, housei);
	//---------------------star----------------------//
	Mat result = HomomorphicFilter(house);

	imshow("original", house);
	imshow("homorphic result", result);

	waitKey(0);


	return 0;
}
Mat fft_func(Mat I) {
	//�Ϲ��ഫ���B�I��//
	Mat complexImg;
	I.convertTo(I, CV_32FC1);

	//DC�I�첾
	for (int i = 0; i < I.rows; i++)
		for (int j = 0; j < I.cols; j++)
			I.at<float>(i, j) = I.at<float>(i, j) * pow(-1, i + j);

	//�ֳt�ť߸��ഫ//
	dft(I, complexImg, DFT_COMPLEX_OUTPUT); // FFT�ܴ�

	return complexImg;
}
Mat ifft_show(Mat I) {
	Mat planes[] = { Mat_<float>(I), Mat::zeros(I.size(), CV_32FC1) }; //�����곡�P�곡
	split(I, planes);  //�����q�D�A planes[0] ����Ƴ����Aplanes[1]����Ƴ���

	Mat mag;
	magnitude(planes[0], planes[1], mag);//�N�ť߸����G�ѽƼ���T��
	mag += Scalar::all(1);
	log(mag, mag); //��K�[�ݪ���Ƥث��ܤ�
	normalize(mag, mag, 0, 1, NORM_MINMAX); //�W�d�ƭȨ� 0~1 ��ܹϤ����ݭn

	return mag;
}
Mat ifft_func(Mat I) {
	Mat ifft;

	idft(I, ifft, DFT_REAL_OUTPUT + DFT_INVERSE + DFT_SCALE);
	//DC�I�첾
	for (int i = 0; i < I.rows; i++)
		for (int j = 0; j < I.cols; j++)
			ifft.at<float>(i, j) = ifft.at<float>(i, j) * pow(-1, i + j);

	return ifft;
}
Mat HomomorphicFilter(Mat I) {
	int H = I.rows, W = I.cols;
	Mat logI = Mat(I.rows, I.cols, CV_32FC1);
	//log
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			logI.at<float>(i, j) = log(1 + I.at<uchar>(i, j));
		}
	}
	//dft
	Mat dftI = fft_func(logI);

	//show Homorphic�e��dft
	Mat mag = ifft_show(dftI);
	//imshow("1", mag);

	//Homomorphic filter
	H = dftI.rows;
	W = dftI.cols;
	float rH = 7;
	float rL = 0.005;
	float c = 10000;
	float D0 = 1000;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			float D = pow(i - H / (float)2, 2) + pow(j - W / (float)2, 2);
			dftI.at<Vec2f>(i, j)[0] = dftI.at<Vec2f>(i, j)[0] * ((rH - rL) * (1 - exp(-c * (D / pow(D0, 2)))) + rL);
			dftI.at<Vec2f>(i, j)[1] = dftI.at<Vec2f>(i, j)[1] * ((rH - rL) * (1 - exp(-c * (D / pow(D0, 2)))) + rL);
		}
	}

	//show Homorphic�᪺dft
	mag = ifft_show(dftI);
	//imshow("2", mag);

	//idft
	Mat idftI = ifft_func(dftI);

	//exp
	for (int i = 0; i < idftI.rows; i++)
		for (int j = 0; j < idftI.cols; j++)
			idftI.at<float>(i, j) = round(exp(idftI.at<float>(i, j)) - 1);

	idftI.convertTo(idftI, CV_8UC1);

	return idftI;
}