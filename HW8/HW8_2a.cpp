#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;

Mat gray_erode(Mat, int, int);
Mat gray_dilate(Mat, int, int);
int main8_2a()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char wbF[] = "walkbridge.raw";
	FILE* wbFile;
	wbFile = fopen(wbF, "rb");
	unsigned char* wbi = new unsigned char[size];
	fread(wbi, 1, size, wbFile);
	Mat wb = Mat(height, width, CV_8UC1, wbi);
	//---------------------star----------------------//
	//(a) gray-level dilation erosion
	Mat gd = gray_dilate(wb, 5, 10);
	Mat ge = gray_erode(wb, 5, 10);

	imshow("ori", wb);
	imshow("gray dilate", gd);
	imshow("gray erode", ge);
	waitKey(0);


	return 0;
}
Mat gray_erode(Mat image, int size, int value) {
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
					Vmin = max(min(image.at<uchar>(a, b) - value, Vmin), 0);

				}
			}
			himage.at<uchar>(i, j) = Vmin;
		}
	}

	Mat wimage(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmin = 255;
			for (int a = i; a <= i; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					Vmin = max(min(himage.at<uchar>(a, b) - value, Vmin), 0);
				}
			}
			wimage.at<uchar>(i, j) = Vmin;
		}
	}

	return wimage;
}
Mat gray_dilate(Mat image, int size, int value) {
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
					Vmax = min(max(image.at<uchar>(a, b) + value, Vmax), 255);
				}
			}
			himage.at<uchar>(i, j) = Vmax;
		}
	}

	Mat wimage(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmax = 0;
			for (int a = i; a <= i; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					Vmax = min(max(himage.at<uchar>(a, b) + value, Vmax), 255);
				}
			}
			wimage.at<uchar>(i, j) = Vmax;
		}
	}

	return wimage;
}