#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;

Mat gray_erode2(Mat, int, int);
Mat gray_dilate2(Mat, int, int);
Mat gray_closing(Mat, int, int);
Mat my2erode(Mat, int);
Mat my2dilate(Mat, int);
Mat my2opening(Mat, int);
Mat my2closing(Mat, int);
Mat mythreshold(Mat, int);
int main8_2b()
{
	int width = 493;
	int height = 277;
	int size = width * height;
	char graF[] = "gra_493x277.raw";
	FILE* graFile;
	graFile = fopen(graF, "rb");
	unsigned char* grai = new unsigned char[size];
	fread(grai, 1, size, graFile);
	Mat gra = Mat(height, width, CV_8UC1, grai);
	//---------------------star----------------------//
	
	//sizes
	Mat binary = mythreshold(gra, 150);
	//imshow("binary", binary);

	Mat close1 = my2closing(binary, 5);
	//imshow("close1", close1);

	Mat diff = close1 - binary;
	//imshow("diff", diff);

	Mat close2 = my2closing(diff, 7);
	//imshow("close2", close2);

	Mat open1 = my2opening(close2, 7);
	//imshow("open1", open1);

	Mat close3 = my2closing(open1, 11);
	//imshow("close3", close3);

	Mat binary_result = my2opening(close3, 25);
	//imshow("binary_result", binary_result);

	//gray-level
	Mat gopen = gray_closing(gra, 5, 15);
	//imshow("gopen", gopen);

	Mat white = mythreshold(gopen, 220);
	//imshow("white", white);

	Mat wopen1 = my2opening(white, 5);
	//imshow("wopen1", wopen1);

	Mat wclose1 = my2closing(wopen1, 21);
	//imshow("wclose1", wclose1);

	Mat wopen2 = my2opening(wclose1, 31);
	//imshow("wopen2", wopen2);

	Mat wclose2 = my2closing(wopen2, 21);
	//imshow("wclose2", wclose2);

	Mat white_result = my2dilate(wclose2, 10);
	//imshow("white_result", white_result);
	//mask
	Mat mask_img(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (binary_result.at<uchar>(i, j) == 255)
				mask_img.at<uchar>(i, j) = 0;
			else
				mask_img.at<uchar>(i, j) = 255;
			if (white_result.at<uchar>(i,j) == 255)
				mask_img.at<uchar>(i, j) = 128;
		}
	}
	imshow("ori", gra);

	imshow("mask_img", mask_img);

	waitKey(0);


	return 0;
}
Mat my2erode(Mat image, int size) {
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
Mat my2dilate(Mat image, int size) {
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
Mat my2opening(Mat image, int size) {
	Mat e = my2erode(image, size);
	Mat d = my2dilate(e, size);
	return d;
}
Mat my2closing(Mat image, int size) {
	Mat d = my2dilate(image, size);
	Mat e = my2erode(d, size);
	return e;
}
Mat mythreshold(Mat image, int threshold) {
	int h = image.rows, w = image.cols;
	Mat binary(h, w, CV_8UC1);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			binary.at<uchar>(i, j) = image.at<uchar>(i, j) > threshold ? 255 : 0;
		}
	}
	return binary;
}
Mat gray_erode2(Mat image, int size, int value) {
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
Mat gray_dilate2(Mat image, int size, int value) {
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
Mat gray_closing(Mat image, int size, int value) {
	Mat gd = gray_dilate2(image, 5, 10);
	Mat ge = gray_erode2(gd, 5, 10);

	return ge;
}