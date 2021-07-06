#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace cv;

void histogram(Mat, int*, const char*);
Mat histogram_match(Mat, Mat);

int main4_1_1()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char houseF[] = "img/house512.raw";    //Input Image name
	char lenaF[] = "img/lena512.raw";

	FILE* houseFile, * lenaFile;
	houseFile = fopen(houseF, "rb");
	lenaFile = fopen(lenaF, "rb");

	unsigned char* housei = new unsigned char[size];
	unsigned char* lenai = new unsigned char[size];

	fread(housei, 1, size, houseFile);
	fread(lenai, 1, size, lenaFile);

	Mat house = Mat(height, width, CV_8UC1, housei);
	Mat lena = Mat(height, width, CV_8UC1, lenai);

	//--------------------star--------------------//
	Mat match_image = histogram_match(house, lena);

	//imshow("lena", lena);
	printf("match結果圖\n");
	imshow("match", match_image);//display Image
	waitKey(0);

	return 0;
}
void histogram(Mat image, int* hist,const char* Wname) {

	int height = image.rows, width = image.cols;
	//計算直方圖
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			hist[image.at<uchar>(i, j)]++;
	//找到直方圖中的最大值
	int max = 0;
	for (int i = 0; i < 256; i++) {
		if (hist[i] > max)
			max = hist[i];
	}
	//將直方圖根據直方圖中的最大值歸一化
	float nomalize[256];
	for (int i = 0; i < 256; i++)
		nomalize[i] = hist[i] / (float)max;

	//-----開始畫直方圖-----//
	int hist_H = 200;//縱軸的高度
	Mat histogram_image(hist_H, 256, CV_8UC1);
	//初始化histogram_image為0
	for (int i = 0; i < hist_H; i++)
		for (int j = 0; j < 256; j++)
			histogram_image = 0;

	for (int i = 0; i < 256; i++)
		line(histogram_image, Point(i, hist_H - 1), Point(i, (hist_H - 1) - round(nomalize[i] * (hist_H - 1))), Scalar(255));

	char name[100];
	sprintf(name, "his_%s", Wname);
	imshow(name, histogram_image);
	waitKey(0);
}
Mat histogram_match(Mat image, Mat match) {
	int height = image.rows, width = image.cols;
	int size = height * width;

	int I_hist[256] = { 0 };
	int M_hist[256] = { 0 };
	printf("house直方圖\n");
	histogram(image, I_hist,"house");
	printf("lena直方圖\n");
	histogram(match, M_hist,"lena");

	double I_cdf[256] = { 0.0 };
	double M_cdf[256] = { 0.0 };
	//計算hist的CDF
	for (int k = 0; k < 256; k++) {
		for (int j = 0; j <= k; j++) {
			I_cdf[k] += I_hist[j] / (float)size;
			M_cdf[k] += M_hist[j] / (float)size;
		}
	}
	//算出image與match之間的差值
	float dif_cdf[256][256];
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			dif_cdf[i][j] = fabs(M_cdf[j] - I_cdf[i]); // G(zk) - sk
		}
	}
	//建立sk與zk間的映射
	//第k個s的映射zk，為與sk相減後差值最小的z
	int lut[256] = { 0 }; //s轉z的映射表
	for (int i = 0; i < 256; i++) {
		float min = dif_cdf[i][0];
		int index = 0;
		for (int j = 1; j < 256; j++) {
			if (min > dif_cdf[i][j]) {
				min = dif_cdf[i][j];
				index = j;
			}
		}
		lut[i] = index;
	}
	//根據映射將原圖做image matching
	Mat image_match = Mat(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int s = image.at<uchar>(i, j);
			image_match.at<uchar>(i, j) = lut[s];
		}
	}

	int match_hist[256] = { 0 };
	printf("match後的直方圖\n");
	histogram(image_match, match_hist,"match");

	return  image_match;
}