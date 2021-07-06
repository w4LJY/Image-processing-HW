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
Mat Bandreject_func(Mat I, float D0, float w) {
	int H = I.rows, W = I.cols;

	//dft
	Mat dftI = fft_func(I);

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			float D = sqrt(pow(i - H / 2, 2) + pow(j - W / 2, 2));//���Pdc�I���Z��
			dftI.at<Vec2f>(i, j)[0] = D >= (D0 - w / 2) && D <= (D0 + w / 2) ? 0 : dftI.at<Vec2f>(i, j)[0];
			dftI.at<Vec2f>(i, j)[1] = D >= (D0 - w / 2) && D <= (D0 + w / 2) ? 0 : dftI.at<Vec2f>(i, j)[1];
		}
	}

	//show bandreject�᪺dft
	Mat mag = ifft_show(dftI);
	imshow("spectrum", mag);

	//idft
	Mat idftI = ifft_func(dftI);
	idftI.convertTo(idftI, CV_8UC1);

	return idftI;
}
