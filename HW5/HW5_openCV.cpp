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
	//---獲得最佳DFT尺寸，為2的次方---//
	Mat padded;
	int M = getOptimalDFTSize(I.rows); //該函數返回給定向量尺寸的傅立葉最優尺寸大小。為了提高離散傅立葉變換的運行速度，需要擴充圖像。                      
	int N = getOptimalDFTSize(I.cols);
	//擴充圖像邊界為M*N
	copyMakeBorder(I, padded, 0, M - I.rows, 0, N - I.cols, BORDER_CONSTANT, Scalar::all(0));

	//---將影像處理為實部與虛部
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) }; //分為實部與虛部
	Mat complexImg;
	merge(planes, 2, complexImg); //合併實虛部

	//---快速傅立葉轉換---//
	dft(complexImg, complexImg); // FFT變換

	//---將傅立葉結果由複數轉幅值---//
	split(complexImg, planes);  //分離通道， planes[0] 為實數部分，planes[1]為虛數部分
	Mat mag;
	magnitude(planes[0], planes[1], mag);

	//---方便觀看的對數尺度變化---//
	//mag += Scalar::all(1);
	//log(mag, mag);

	//---位移圖像---使DC點位於圖像中心---//
	mag = mag(Rect(0, 0, mag.cols & -2, mag.rows & -2));  //保證偶數的邊長，若有奇數行進行裁剪
	int cx = mag.cols / 2;
	int cy = mag.rows / 2;

	//切為4個區塊重新排列，從左到右，從上到下 分別為q0, q1, q2, q3                                                                                                           
	Mat tmp;
	Mat q0(mag, Rect(0, 0, cx, cy));
	Mat q1(mag, Rect(cx, 0, cx, cy));
	Mat q2(mag, Rect(0, cy, cx, cy));
	Mat q3(mag, Rect(cx, cy, cx, cy));
	//對調q0和q3
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	//對調q1和q2
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	normalize(mag, mag, 0, 1, NORM_MINMAX); //規範化值到 0~1 顯示圖片的需要

	cv_complex Point = { complexImg,mag };

	return Point;
}
Mat ifft_func(Mat I) {
	Mat ifft;
	idft(I, ifft, DFT_REAL_OUTPUT);
	normalize(ifft, ifft, 0, 1, NORM_MINMAX);
	return ifft;
}