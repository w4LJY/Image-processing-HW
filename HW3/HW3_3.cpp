#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

Mat log_T(Mat, float); //log�ഫ
Mat powerlaw_T(Mat, float, double c = 10); //�����ഫ

int main3_3()
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
	//log transformation
	printf("\n�Ϲ�livingroom(log�ഫ  c = 100)\n");
	Mat livingroom_c100 = log_T(livingroom, 100);
	imshow("livingroom_c100", livingroom_c100);

	waitKey(0);

	printf("\n�Ϲ�livingroom(log�ഫ  c = 20)\n");
	Mat livingroom_c20 = log_T(livingroom, 20);
	imshow("livingroom_c20", livingroom_c20);

	waitKey(0);

	printf("\n�Ϲ�cameraman(log�ഫ  c = 100)\n");
	Mat cameraman_c100 = log_T(cameraman, 100);
	imshow("cameraman_c100", cameraman_c100);

	waitKey(0);

	printf("\n�Ϲ�cameraman(log�ഫ  c = 20)\n");
	Mat cameraman_c20 = log_T(cameraman, 20);
	imshow("cameraman_c20", cameraman_c20);

	waitKey(0);


	//power-law transformation
	printf("\n�Ϲ�livingroom(�����ഫ gamma = 0.2 c = 345)\n");
	Mat livingroom_r02 = powerlaw_T(livingroom, 0.2, 345);
	imshow("livingroom_r02", livingroom_r02);
	waitKey(0);

	printf("\n�Ϲ�livingroom(�����ഫ gamma = 10 c = 1e-11)\n");
	Mat livingroom_r10 = powerlaw_T(livingroom, 10, 0.00000000001);
	imshow("livingroom_r10", livingroom_r10);

	waitKey(0);

	printf("\n�Ϲ�cameraman(�����ഫ gamma = 0.2 c = 75)\n");
	Mat cameraman_r02 = powerlaw_T(cameraman, 0.2, 75);
	imshow("cameraman_r02", cameraman_r02);

	waitKey(0);

	printf("\n�Ϲ�cameraman(�����ഫ gamma = 10 c = 1e-22)\n");
	Mat cameraman_r10 = powerlaw_T(cameraman, 10, 0.0000000000000000000001);
	imshow("cameraman_r10", cameraman_r10);

	waitKey(0);
	
	return 0;
}
Mat log_T(Mat image, float c) {
	Mat log_image = Mat(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			log_image.at<uchar>(i, j) = c *log10(1+r); //log�ഫ���Ƭ�10,log0�O�L�N�q���A���Nr�[�W1
		}
	}
	return log_image;
}
Mat powerlaw_T(Mat image,float gamma, double c) {
	Mat powerlaw_image = Mat(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			powerlaw_image.at<uchar>(i, j) = c*pow(r,gamma);
		}
	}
	return powerlaw_image;
}
