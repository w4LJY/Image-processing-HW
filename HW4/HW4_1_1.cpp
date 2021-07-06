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
	printf("match���G��\n");
	imshow("match", match_image);//display Image
	waitKey(0);

	return 0;
}
void histogram(Mat image, int* hist,const char* Wname) {

	int height = image.rows, width = image.cols;
	//�p�⪽���
	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			hist[image.at<uchar>(i, j)]++;
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
	printf("house�����\n");
	histogram(image, I_hist,"house");
	printf("lena�����\n");
	histogram(match, M_hist,"lena");

	double I_cdf[256] = { 0.0 };
	double M_cdf[256] = { 0.0 };
	//�p��hist��CDF
	for (int k = 0; k < 256; k++) {
		for (int j = 0; j <= k; j++) {
			I_cdf[k] += I_hist[j] / (float)size;
			M_cdf[k] += M_hist[j] / (float)size;
		}
	}
	//��Ximage�Pmatch�������t��
	float dif_cdf[256][256];
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			dif_cdf[i][j] = fabs(M_cdf[j] - I_cdf[i]); // G(zk) - sk
		}
	}
	//�إ�sk�Pzk�����M�g
	//��k��s���M�gzk�A���Psk�۴��t�ȳ̤p��z
	int lut[256] = { 0 }; //s��z���M�g��
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
	//�ھڬM�g�N��ϰ�image matching
	Mat image_match = Mat(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int s = image.at<uchar>(i, j);
			image_match.at<uchar>(i, j) = lut[s];
		}
	}

	int match_hist[256] = { 0 };
	printf("match�᪺�����\n");
	histogram(image_match, match_hist,"match");

	return  image_match;
}