#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;

Mat myerode2(Mat, int);
Mat mydilate2(Mat, int);
int main8_1d()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char adipF[] = "adip_noise.raw";
	FILE* adipFile;
	adipFile = fopen(adipF, "rb");
	unsigned char* adipi = new unsigned char[size];
	fread(adipi, 1, size, adipFile);
	Mat adip = Mat(height, width, CV_8UC1, adipi);
	//---------------------star----------------------//
	//(a) remove noise
	Mat adip_c = 255 - adip;
	Mat e = myerode2(adip_c, 3);
	Mat d = mydilate2(e, 3);
	Mat result = 255 - d;


	imshow("", result);
	waitKey(0);


	return 0;
}
Mat myerode2(Mat image, int size) {
	int height = image.rows, width = image.cols;
	Mat himage(height, width, CV_8UC1);

	int M_half = (size - 1) / 2;

	int Vmin;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmin = 255;
			for (int a = i - M_half; a <= i + M_half; a++) {
				for (int b = j; b <= j; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (image.at<uchar>(a, b) == 0) {
						Vmin = 0;
						goto here;
					}
				}
			}
		here: himage.at<uchar>(i, j) = Vmin;
		}
	}

	Mat wimage(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmin = 255;
			for (int a = i; a <= i; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (himage.at<uchar>(a, b) == 0) {
						Vmin = 0;
						goto here2;
					}
				}
			}
		here2: wimage.at<uchar>(i, j) = Vmin;
		}
	}

	return wimage;
}
Mat mydilate2(Mat image, int size) {
	int height = image.rows, width = image.cols;
	Mat himage(height, width, CV_8UC1);

	int M_half = (size - 1) / 2;

	int Vmax;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmax = 0;
			for (int a = i - M_half; a <= i + M_half; a++) {
				for (int b = j; b <= j; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (image.at<uchar>(a, b) == 255) {
						Vmax = 255;
						goto here;
					}
				}
			}
		here: himage.at<uchar>(i, j) = Vmax;
		}
	}

	Mat wimage(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmax = 0;
			for (int a = i; a <= i; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (himage.at<uchar>(a, b) == 255) {
						Vmax = 255;
						goto here2;
					}
				}
			}
		here2: wimage.at<uchar>(i, j) = Vmax;
		}
	}

	return wimage;
}