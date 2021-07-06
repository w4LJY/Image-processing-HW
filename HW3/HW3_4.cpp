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
		printf("�п�J�Ʀr�����Ϯ�\n\
				\t�Ʀr1:livingroom\n\
				\t�Ʀr2:cameraman\n");
		scanf("%d", &input);
		switch (input) {
		case 1:
			//livingroom
			printf("\n�p�⪽���\n");
			histogram(livingroom, hist_livingroom);
			printf("\n����ϵ���\n");
			livingroom_eq = histogram_eq(livingroom, hist_livingroom);
			printf("\n����ϵ��ƫ᪺�����\n");
			histogram(livingroom_eq, hist_livingroom_eq);
			break;
		case 2:
			//cameraman
			printf("\n�p�⪽���\n");
			histogram(cameraman, hist_cameraman);
			printf("\n����ϵ���\n");
			cameraman_eq = histogram_eq(cameraman, hist_cameraman);
			printf("\n����ϵ��ƫ᪺�����\n");
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
	//�p�⪽���
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			hist[r]++; //�C���C��X�{����
		}
	}
	//��쪽��Ϥ����̤j��
	int max = 0;
	for (int i = 0; i < 256; i++) {
		if (hist[i] > max)
			max = hist[i];
	}
	//�N����Ϯھڪ���Ϥ����̤j���k�@��
	float nomalize[256];
	for (int i = 0; i < 256; i++) 
		nomalize[i] = hist[i] / (float)max;

	//-----�}�l�e�����-----//
	int hist_H = 200;//�a�b������
	Mat histogram_image(hist_H, 256, CV_8UC1);
	//��l��histogram_image��0
	for (int i = 0; i < hist_H; i++)
		for (int j = 0; j < 256; j++)
			histogram_image = 0;

	for (int i = 0; i < 256; i++)
		line(histogram_image, Point(i, hist_H-1), Point(i, (hist_H-1) - round(nomalize[i]*(hist_H-1))), Scalar(255));//�y�а_�l�I�O���W���A�]���e�Ϯɭn�ΰ��ץh��

	imshow("",histogram_image);
	waitKey(0);
}
Mat histogram_eq(Mat image,int* hist) {
	double hist_cdf[256] = { 0.0 };
	int size = image.rows * image.cols;

	//�p��hist��CDF
	for (int k = 0; k < 256; k++) {
		for (int j = 0; j <= k; j++) {
			hist_cdf[k] += hist[j]/(float)size; //hist[j]/size�Orj��pdf
		}
	}
	//�NCDF*255��X�����s���C���m
	Mat image_eq = Mat(image.rows, image.cols, CV_8UC1);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int r = image.at<uchar>(i, j);
			image_eq.at<uchar>(i, j) = round(hist_cdf[r] * 255);  //CDF������pixel���s�Ƕ��ȡA���n�N0~1���ƭȡA�Ԧ���0~255
		}
	}
	imshow("", image_eq);
	waitKey(0);
	return  image_eq;
}