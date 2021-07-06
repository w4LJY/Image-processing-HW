#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>

using namespace cv;
using namespace std;
#define PI 3.14159265358979323846

struct cv_complex {
	Mat compleximg;
	Mat image;
};
cv_complex fft_func(Mat);
Mat ifft_func(Mat);

int main5_openCV()
{
	int width = 256;
	int height = 256;
	int size = width * height;
	char rectF[] = "rect256.raw";
	char circleF[] = "circle256.raw";
	char squareF[] = "square256.raw";
	char RsquareF[] = "square256_rota.raw";
	FILE* rectFile, * circleFile, * squareFile, * RsquareFile;
	rectFile = fopen(rectF, "rb");
	circleFile = fopen(circleF, "rb");
	squareFile = fopen(squareF, "rb");
	RsquareFile = fopen(RsquareF, "rb");
	unsigned char* recti = new unsigned char[size];
	unsigned char* circlei = new unsigned char[size];
	unsigned char* squarei = new unsigned char[size];
	unsigned char* Rsquarei = new unsigned char[size];
	fread(recti, 1, size, rectFile);
	fread(circlei, 1, size, circleFile);
	fread(squarei, 1, size, squareFile);
	fread(Rsquarei, 1, size, RsquareFile);
	Mat rect = Mat(height, width, CV_8UC1, recti);
	Mat circle = Mat(height, width, CV_8UC1, circlei);
	Mat square = Mat(height, width, CV_8UC1, squarei);
	Mat Rsquare = Mat(height, width, CV_8UC1, Rsquarei);
	//---------------------star----------------------//
	//DFT
	cv_complex IPoint_rect = fft_func(rect);
	//IDFT
	Mat result_rect = ifft_func(IPoint_rect.compleximg);
	imshow("freq_rect.png", IPoint_rect.image);
	imshow("result_rect.png", result_rect);

	//---circle---//
	//DFT
	cv_complex IPoint_circle = fft_func(circle);
	//IDFT
	Mat result_circle = ifft_func(IPoint_circle.compleximg);
	imshow("freq_circle.png", IPoint_circle.image);
	imshow("result_circle.png", result_circle);

	//---Square---//
	//DFT
	cv_complex IPoint_square = fft_func(square);
	//IDFT
	Mat result_square = ifft_func(IPoint_square.compleximg);
	imshow("freq_square.png", IPoint_square.image);
	imshow("result_square.png", result_square);

	//---Square Rotation---//
	//DFT
	cv_complex IPoint_Rsquare = fft_func(Rsquare);
	//IDFT
	Mat result_Rsquare = ifft_func(IPoint_Rsquare.compleximg);
	imshow("freq_Rsquare.png", IPoint_Rsquare.image);
	imshow("result_Rsquare.png", result_Rsquare);

	waitKey(0);
	return 0;
}
cv_complex fft_func(Mat I) {
	//---��o�̨�DFT�ؤo�A��2������---//
	Mat padded;
	int M = getOptimalDFTSize(I.rows); //�Ө�ƪ�^���w�V�q�ؤo���ť߸����u�ؤo�j�p�C���F���������ť߸��ܴ����B��t�סA�ݭn�X�R�Ϲ��C                      
	int N = getOptimalDFTSize(I.cols);
	//�X�R�Ϲ���ɬ�M*N
	copyMakeBorder(I, padded, 0, M - I.rows, 0, N - I.cols, BORDER_CONSTANT, Scalar::all(0));

	//---�N�v���B�z���곡�P�곡
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) }; //�����곡�P�곡
	Mat complexImg;
	merge(planes, 2, complexImg); //�X�ֹ�곡

	//---�ֳt�ť߸��ഫ---//
	dft(complexImg, complexImg); // FFT�ܴ�

	//---�N�ť߸����G�ѽƼ���T��---//
	split(complexImg, planes);  //�����q�D�A planes[0] ����Ƴ����Aplanes[1]����Ƴ���
	Mat mag;
	magnitude(planes[0], planes[1], mag);

	//---��K�[�ݪ���Ƥث��ܤ�---//
	//mag += Scalar::all(1);
	//log(mag, mag);

	//---�첾�Ϲ�---��DC�I���Ϲ�����---//
	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));  //�O�Ұ��ƪ�����A�Y���_�Ʀ�i�����
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	//����4�Ӱ϶����s�ƦC�A�q����k�A�q�W��U ���O��q0, q1, q2, q3                                                                                                           
	Mat tmp;
	Mat q0(mag, Rect(0, 0, cx, cy));
	Mat q1(mag, Rect(cx, 0, cx, cy));
	Mat q2(mag, Rect(0, cy, cx, cy));
	Mat q3(mag, Rect(cx, cy, cx, cy));
	//���q0�Mq3
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	//���q1�Mq2
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(mag, mag, 0, 1, NORM_MINMAX); //�W�d�ƭȨ� 0~1 ��ܹϤ����ݭn

	cv_complex Point = { complexImg,mag };

	return Point;
}
Mat ifft_func(Mat I) {
	Mat ifft;
	idft(I, ifft, DFT_REAL_OUTPUT);
	normalize(ifft, ifft, 0, 1, NORM_MINMAX);
	return ifft;
}