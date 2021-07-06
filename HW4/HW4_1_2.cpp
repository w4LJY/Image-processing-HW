#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<stdlib.h>
using namespace cv;

void histogram2(Mat, int*, const char*); //輸入圖像，儲存hist的陣列，
void local_histeq(Mat, int);
Mat mi_padd2(Mat, int);

int main4_1_2()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char houseF[] = "img/house512.raw";
	FILE* houseFile;
	houseFile = fopen(houseF, "rb");
	unsigned char* housei = new unsigned char[size];
	fread(housei, 1, size, houseFile);
	Mat house = Mat(height, width, CV_8UC1, housei);

	//--------------------star--------------------//

	printf("local histogram equalization mask大小3x3\n");
	local_histeq(house, 3);
	printf("local histogram equalization mask大小9x9\n");
	local_histeq(house, 9);
	printf("local histogram equalization mask大小27x27\n");
	local_histeq(house, 27);
	
	waitKey(0);

	return 0;
}
void histogram2(Mat image, int* hist, const char* Wname) {

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
}
void local_histeq(Mat image, int mask) {
	int height = image.rows, width = image.cols;

	Mat image_p = mi_padd2(image, mask); //採用鏡射的padding方式
	Mat local_eq = Mat(height, width, CV_8UC1);
	int M_half = (mask - 1) / 2;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			//計算直方圖
			int local_hist[256] = {0};
			//對每一個pixel周圍mask*mask的大小做直方圖等化
			for (int a = i - M_half; a <= i + M_half; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					local_hist[image_p.at<uchar>(a+M_half, b+M_half)]++;
				}
			}
			//計算CDF
			double local_cdf[256] = {0.0};
			for (int k = 0; k < 256; k++) {
				for (int j = 0; j <= k; j++) {
					local_cdf[k] += local_hist[j] / (float)(mask*mask);
				}
			}
			//直方圖等化(只對中心點，有就是當前迭代到的點進行直方圖等化)
			int r = image.at<uchar>(i, j);
			local_eq.at<uchar>(i, j) = round(local_cdf[r] * 255);
			//printf("%d->%d,", r,local_eq.at<uchar>(i, j));
		}
	}
	int local_hist[256] = {0};
	histogram2(local_eq, local_hist, "local");
	char name_window[50];
	sprintf(name_window, "local_eq%d", mask);
	imshow(name_window, local_eq);
	waitKey(0);
}
Mat mi_padd2(Mat image, int size) {
	int height = image.rows, width = image.cols;
	int s_half = (size - 1) / 2;
	Mat image_pad(height + 2 * s_half, width + 2 * s_half, CV_8UC1);
	int p_height = image_pad.rows, p_width = image_pad.cols;

	for (int i = 0; i < p_height; i++) {
		for (int j = 0; j < p_width; j++) {
			int a = i < s_half;
			int b = i > p_height - s_half - 1;
			int c = j < s_half;
			int d = j > p_width - s_half - 1;
			image_pad.at<uchar>(i, j) = image.at<uchar>(abs(s_half - i + (2 * height - 1) * b) - a * 1, abs(s_half - j + (2 * width - 1) * d) - c * 1);
		}
	}
	return image_pad;
}
