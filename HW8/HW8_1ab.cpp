#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>

using namespace cv;
using namespace std;

Mat myerode(Mat, int);
Mat mydilate(Mat, int);
Mat region_fill(Mat, int, int); //輸入圖像，seed座標x,y (cols,rows)
int main8_1ab()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char adipF[] = "adip.raw";
	FILE* adipFile;
	adipFile = fopen(adipF, "rb");
	unsigned char* adipi = new unsigned char[size];
	fread(adipi, 1, size, adipFile);
	Mat adip = Mat(height, width, CV_8UC1, adipi);
	//---------------------star----------------------//
	//(a) boundary
	Mat d = mydilate(adip, 5);
	Mat e = myerode(adip, 5);
	Mat d_img = d - adip;
	Mat img_e = adip - e;
	Mat d_e = d - e;
	imshow("膨脹-原圖", d_img);
	imshow("原圖-侵蝕", img_e);
	imshow("膨脹-侵蝕", d_e);
	waitKey(0);
	//(b) region fill HW7
	printf("請助教稍等\n");
	Mat fill_H = region_fill(adip, 162, 268);
	Mat fill_W = region_fill(adip, 274, 241);
	Mat fill_7 = region_fill(adip, 392, 238);
	Mat image_fill = (fill_H + fill_W + fill_7);
	//remove
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (image_fill.at<uchar>(i, j) != 0)
				adip.at<uchar>(i, j) = 0;
	imshow("remove HW7", adip);

	waitKey(0);


	return 0;
}
Mat myerode(Mat image, int size) {
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
Mat mydilate(Mat image, int size) {
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
Mat region_fill(Mat image, int seedx, int seedy) {
	int h = image.rows, w = image.cols;
	Mat image_fill = Mat::zeros(h, w, CV_8UC1);
	Mat image_d = Mat::zeros(h, w, CV_8UC1);

	image_d.at<uchar>(seedy, seedx) = 255;
	int stop = 1;
	while (true) {
		image_fill = mydilate(image_d, 5);
		image_fill = image_fill & image;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				int before = image_d.at<uchar>(i, j);
				int after = image_fill.at<uchar>(i, j);
				if (before-after != 0)
					goto here;
			}
		}
		break;
	here:image_d = image_fill; 	
	}
	return image_fill;
}