#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<vector>

using namespace cv;
using namespace std;
#define PI 3.14159265359

//用來回傳實部、需部的vector
struct Icomplex {
	vector<vector<double>> Ireal, Iimag;
	Mat image;
};

Icomplex dft_func(Mat);
Mat idft_func(Icomplex);
vector<double> MSE_PSNR(Mat, Mat);

int main5()
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
	Icomplex IPoint_rect = dft_func(rect);
	//IDFT
	Mat result_rect = idft_func(IPoint_rect);
	vector<double> rect_mp = MSE_PSNR(rect, result_rect);
	printf("rect{MSE:%f,PSNR:%f}\n",rect_mp[0],rect_mp[1]);
	imshow("freq_rect.png", IPoint_rect.image);
	imshow("result_rect.png", result_rect);

	//---circle---//
	//DFT
	Icomplex IPoint_circle = dft_func(circle);
	//IDFT
	Mat result_circle = idft_func(IPoint_circle);
	vector<double> circle_mp = MSE_PSNR(circle, result_circle);
	printf("circle{MSE:%f,PSNR:%f}\n", circle_mp[0], circle_mp[1]);
	imshow("freq_circle.png", IPoint_circle.image);
	imshow("result_circle.png", result_circle);

	//---Square---//
	//DFT
	Icomplex IPoint_square = dft_func(square);
	//IDFT
	Mat result_square = idft_func(IPoint_square);
	vector<double> square_mp = MSE_PSNR(square, result_square);
	printf("square{MSE:%f,PSNR:%f}\n", square_mp[0], square_mp[1]);
	imshow("freq_square.png", IPoint_square.image);
	imshow("result_square.png", result_square);

	//---Square Rotation---//
	//DFT
	Icomplex IPoint_Rsquare = dft_func(Rsquare);
	//IDFT
	Mat result_Rsquare = idft_func(IPoint_Rsquare);
	vector<double> Rsquare_mp = MSE_PSNR(Rsquare, result_Rsquare);
	printf("Rsquare{MSE:%f,PSNR:%f}\n", Rsquare_mp[0], Rsquare_mp[1]);
	imshow("freq_Rsquare.png", IPoint_Rsquare.image);
	imshow("result_Rsquare.png", result_Rsquare);

	waitKey(0);
	return 0;
}
Icomplex dft_func(Mat I) {
	int M = I.rows, N = I.cols;
	Mat result_Mat(M, N, CV_8UC1);
	double temp, Nmin = 255.0, Nmax = 0.0;
	int move;

	//轉為F(x,v)
	vector<vector<double>> real_v(M, vector<double>(N));
	vector<vector<double>> imag_v(M, vector<double>(N));
	for (int x = 0; x < M; x++) {
		for (int v = 0; v < N; v++) {
			for (int y = 0; y < N; y++) {
				move = (x + y) % 2 == 0 ? 1 : -1; //DC shift

				temp = (double)v * y / (double)N;
				real_v[x][v] += (I.at<uchar>(x, y) * cos(-2 * PI * temp) * move) / (double)N;
				imag_v[x][v] += (I.at<uchar>(x, y) * sin(-2 * PI * temp) * move) / (double)N;
			}
		}
	}

	//轉為F(u,v)
	vector<vector<double>> result(M, vector<double>(N));
	vector<vector<double>> real(M, vector<double>(N));
	vector<vector<double>> imag(M, vector<double>(N));
	for (int u = 0; u < M; u++) {
		for (int v = 0; v < N; v++) {
			for (int x = 0; x < M; x++) {
				temp = (double)u * x / (double)M;
				real[u][v] += (1 / (double)M) * real_v[x][v] * cos(-2 * PI * temp);
				imag[u][v] += (1 / (double)M) * real_v[x][v] * sin(-2 * PI * temp);

			}
			result[u][v] = sqrt(pow(real[u][v], 2) + pow(imag[u][v], 2));
			Nmin = result[u][v] < Nmin ? result[u][v] : Nmin;
			Nmax = result[u][v] > Nmax ? result[u][v] : Nmax;
		}
	}
	//printf("%f,%f", Nmin, Nmax);
	//normalize
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < N; j++) {
			result_Mat.at<uchar>(i, j) = round((result[i][j] - Nmin) / (Nmax - Nmin) * 255);
		}
	}
	//imshow("result", result_Mat);

	struct Icomplex IPoint = { real,imag,result_Mat };

	return IPoint;
}
Mat idft_func(Icomplex IPoint) {
	int M = IPoint.Ireal.size(), N = IPoint.Ireal[0].size();
	Mat result_Mat(M, N, CV_8UC1);
	double pixel, temp;
	int move;

	//轉為F(u,y)
	vector<vector<double>> real_v(M, vector<double>(N));
	vector<vector<double>> imag_v(M, vector<double>(N));

	for (int u = 0; u < M; u++) {
		for (int y = 0; y < N; y++) {
			real_v[u][y] = 0;
			imag_v[u][y] = 0;
			for (int v = 0; v < N; v++) {
				temp = (double)v * y / (double)N;
				real_v[u][y] += IPoint.Ireal[u][v] * cos(2 * PI * temp);
				imag_v[u][y] += IPoint.Iimag[u][v] * sin(2 * PI * temp);


			}
		}
	}

	//轉為F(u,v)
	vector<vector<int>> result(M, vector<int>(N));
	for (int x = 0; x < M; x++) {
		for (int y = 0; y < N; y++) {
			pixel = 0;
			for (int u = 0; u < M; u++) {
				move = (x + y) % 2 == 0 ? 1 : -1; //DC shift

				temp = (double)u * x / (double)M;
				pixel += real_v[u][y] * cos(2 * PI * temp) * move - imag_v[u][y] * sin(2 * PI * temp) * move;
			}
			result_Mat.at<uchar>(x, y) = max(min(int(pixel), 255), 0);
		}
	}
	//imshow("", result_Mat);

	return result_Mat;
}
vector<double> MSE_PSNR(Mat original, Mat image) {
	int height = original.rows, width = original.cols;
	vector<double> mp(2);
	//---MSE---//
	double squE = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			squE = squE + pow(original.at<uchar>(i, j) - image.at<uchar>(i, j), 2);
		}
	}
	mp[0] = squE / (original.rows * original.cols);

	//---PSNR---//
	mp[1] = 20 * log10(255 / sqrt(mp[0]));

	return mp;
}