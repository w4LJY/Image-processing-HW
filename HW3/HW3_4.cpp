#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;
void histogram(Mat,int*);
Mat histogram_eq(Mat,int*);

int main3_4()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char livingroomF[] = "img/livingroom_d512.raw";
	char cameramanF[] = "img/cameraman_b512.raw";

	FILE* livingroomFile, * cameramanFile;
	livingroomFile = fopen(livingroomF, "rb");
	cameramanFile = fopen(cameramanF, "rb");

	unsigned char* livingroomi = new unsigned char[size];
	unsigned char* cameramani = new unsigned char[size];
	fread(livingroomi, 1, size, livingroomFile);
	fread(cameramani, 1, size, cameramanFile);

	Mat livingroom = Mat(height, width, CV_8UC1, livingroomi);
	Mat cameraman = Mat(height, width, CV_8UC1, cameramani);

	//------------star-------------//
	
	int star = 1;

	int hist_livingroom[256] = { 0 };
	int hist_livingroom_eq[256] = { 0 };
	Mat livingroom_eq;
	int hist_cameraman[256] = { 0 };
	int hist_cameraman_eq[256] = { 0 };
	Mat cameraman_eq;

	while (star) {
		int input;
		printf("請輸入數字對應圖案\n\
				\t數字1:livingroom\n\
				\t數字2:cameraman\n");
		scanf("%d", &input);
		switch (input) {
		case 1:
			//livingroom
			printf("\n計算直方圖\n");
			histogram(livingroom, hist_livingroom);
			printf("\n直方圖等化\n");
			livingroom_eq = histogram_eq(livingroom, hist_livingroom);
			printf("\n直方圖等化後的直方圖\n");
			histogram(livingroom_eq, hist_livingroom_eq);
			break;
		case 2:
			//cameraman
			printf("\n計算直方圖\n");
			histogram(cameraman, hist_cameraman);
			printf("\n直方圖等化\n");
			cameraman_eq = histogram_eq(cameraman, hist_cameraman);
			printf("\n直方圖等化後的直方圖\n");
			histogram(cameraman_eq, hist_cameraman_eq);
			break;
		case 0:
			star = 0;
			continue;
			break;
		}
	}
	return 0;
}
void histogram(Mat image,int* hist) {
	
	int height = image.rows, width = image.cols;
	//計算直方圖
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			hist[r]++; //每個顏色出現次數
		}
	}
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
		line(histogram_image, Point(i, hist_H-1), Point(i, (hist_H-1) - round(nomalize[i]*(hist_H-1))), Scalar(255));//座標起始點是左上角，因此畫圖時要用高度去減

	imshow("",histogram_image);
	waitKey(0);
}
Mat histogram_eq(Mat image,int* hist) {
	double hist_cdf[256] = { 0.0 };
	int size = image.rows * image.cols;

	//計算hist的CDF
	for (int k = 0; k < 256; k++) {
		for (int j = 0; j <= k; j++) {
			hist_cdf[k] += hist[j]/(float)size; //hist[j]/size是rj的pdf
		}
	}
	//將CDF*255找出對應新的顏色位置
	Mat image_eq = Mat(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			image_eq.at<uchar>(i, j) = round(hist_cdf[r] * 255);  //CDF對應該pixel的新灰階值，但要將0~1的數值，拉伸到0~255
		}
	}
	imshow("", image_eq);
	waitKey(0);
	return  image_eq;
}