#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <math.h>
#include <time.h>

using namespace cv;

Mat down_image(Mat, int); //��ƭ��Y�p�Ϥ�[�˱�],�Ѽ�(��J�Ϥ�,�Y�p���v(���))
Mat nearest_neighbor(Mat, float); //�D��ƭ���j�Ϥ�[nearest neighbor],�Ѽ�(��J�Ϥ��A��j���v)
Mat bilinear(Mat, float); //�D��ƭ���j�Ϥ�[bilinear],�Ѽ�(��J�Ϥ��A��j���v)
Mat bicubic(Mat, float); //�D��ƭ���j�Ϥ�[bicubic],�Ѽ�(��J�Ϥ��A��j���v)

void bicubic_function(float w_x[4],float w_y[4], float u, float v); //bicubic���

int main2_1_3()
{
	int width = 256;
	int height = 256;
	int size = width * height;
	char OriFile[] = "img/lena_256.raw"; 
	FILE* lenaFile;
	lenaFile = fopen(OriFile, "rb");
	unsigned char* lenai = new unsigned char[size];
	fread(lenai, 1, size, lenaFile);

	Mat lenaMat = Mat(height, width, CV_8UC1, lenai); //unsign 8bit channel 1(�Ƕ�)
	/*-----------------------------------------star-----------------------------------*/
	clock_t t1, t2; //�Ψӭp���O�ɶ�

	//GUI����
	int star = 1;
	while (star) {
		
		int input;
		int range;

		printf("\n�п�ܨϥΤ�k\n\
			\t�Ʀr1:nearest neighbor\n\
			\t�Ʀr2:bilinear\n\
			\t�Ʀr3:bicubic\n\
			\n\
			\t�Ʀr0:�h�^�D�����\n");
		scanf("%d", &input);
		if (input == 0){
			star = 0;
			continue;
		}
		printf("\n�п�ܩ�j�Y�p���v\n\
			\t�Ʀr1:��j2.5���A�Y�p2��\n\
			\t�Ʀr2:�Y�p2���A��j2.5��\n\
			\t�Ʀr3:��j1.25��\n");
		scanf("%d", &range);
		switch (range) {
		case 1:
			printf("\n��j2.5���A�Y�p2���C");
			break;
		case 2:
			printf("\n�Y�p2���A��j2.5���C");

			break;
		case 3:
			printf("\n��j1.25���C");

			break;
		case 0:
			star = 0;
			continue;
			break;
		default:
			printf("\n�Ш̷ӻ�����J�Ʀr\n");
			continue;
			break;
		}
		Mat lenaMat_results;
		switch (input) {
		case 1:
			printf("�ϥ�nearest neigbor\n");
			if (range == 1) {
				t1 = clock();
				Mat lenaMat_nearest_Up25 = nearest_neighbor(lenaMat, 2.5);
				Mat lenaMat_nearest_Up25_down2 = down_image(lenaMat_nearest_Up25, 2);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("nearest neigbor zoom2.5 shrink2", lenaMat_nearest_Up25_down2);
				waitKey(0);
			}
			if (range == 2) {
				t1 = clock();
				Mat lenaMat_nearest_down2 = down_image(lenaMat, 2);
				Mat lenaMat_nearest_down2_Up25 = nearest_neighbor(lenaMat_nearest_down2, 2.5);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("nearest neigbor shrink2 zoom2.5", lenaMat_nearest_down2_Up25);
				waitKey(0);
			}
			if (range == 3){
				t1 = clock();
				Mat lenaMat_nearest_Up125 = nearest_neighbor(lenaMat, 1.25);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("nearest neigbor zoom 1.25", lenaMat_nearest_Up125);//display Image
				waitKey(0);
			}
			break;
		case 2:
			printf("�ϥ�bilinear\n");
			if (range == 1) {
				t1 = clock();
				Mat lenaMat_bilinear_Up25 = bilinear(lenaMat, 2.5);
				Mat lenaMat_bilinear_Up25_down2 = down_image(lenaMat_bilinear_Up25, 2);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("bilinear zoom2.5 shrink2", lenaMat_bilinear_Up25_down2);
				waitKey(0);
			}
			if (range == 2) {
				t1 = clock();
				Mat lenaMat_bilinear_down2 = down_image(lenaMat, 2);
				Mat lenaMat_bilinear_down2_Up25 = bilinear(lenaMat_bilinear_down2, 2.5);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("bilinearshrink2zoom25", lenaMat_bilinear_down2_Up25);
				waitKey(0);
			}
			if (range == 3) {
				t1 = clock();
				Mat lenaMat_bilinear_Up125 = bilinear(lenaMat, 1.25);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("bilinearzoom125", lenaMat_bilinear_Up125);
				waitKey(0);
			}
			break;
		case 3:
			printf("�ϥ�bicubic\n");
			if (range == 1) {
				t1 = clock();
				Mat lenaMat_bicubic_Up25 = bicubic(lenaMat, 2.5);
				Mat lenaMat_bicubic_UP25_down2 = down_image(lenaMat_bicubic_Up25, 2);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("bicubic zoom2.5 shrink2", lenaMat_bicubic_UP25_down2);
				waitKey(0);
			}
			if (range == 2) {
				t1 = clock();
				Mat lenaMat_bicubic_down2 = down_image(lenaMat, 2);
				Mat lenaMat_bicubic_down2_Up25 = bicubic(lenaMat_bicubic_down2, 2.5);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("bicubic shrink2 zoom2.5", lenaMat_bicubic_down2_Up25);
				waitKey(0);
			}
			if (range == 3) {
				t1 = clock();
				Mat lenaMat_bicubic_Up125 = bicubic(lenaMat, 1.25);
				t2 = clock();
				printf("%lfs\n", (t2 - t1) / (double)(CLOCKS_PER_SEC));
				imshow("bicubic zoom 1.25", lenaMat_bicubic_Up125);
				waitKey(0);
			}
			break;
		case 0:
			star = 0;
			continue;
			break;
		default:
			printf("\n�Ш̷ӻ�����J�Ʀr\n");
			continue;
			break;
		}
}
	fclose(lenaFile);
	delete lenai;
	destroyWindow("lenaFile");

	return 0;
}
Mat down_image(Mat lenaMat, int times) {
	Mat lenaMatdown = Mat(lenaMat.rows / times, lenaMat.cols / times, CV_8UC1);
	//�Ī����˥h
	for (int i = 0; i < lenaMat.rows; i++)
	{
		for (int j = 0; j < lenaMat.cols; j++)
		{
			if (i % times == 0 && j % times == 0)
				lenaMatdown.at<uchar>(i / times, j / times) = lenaMat.at<uchar>(i, j);
		}
	}
	return lenaMatdown;
}
//�ھڭ��4��pixel�ھڳ̪�Z������
Mat nearest_neighbor(Mat lenaMat, float times) {
	//lenaMat���,lemaMatup��j��
	int height = lenaMat.rows;
	int width = lenaMat.cols;
	Mat lenaMatup = Mat(height * times, width * times, CV_8UC1);

	for (int i = 0; i < lenaMatup.rows; i++)
	{
		for (int j = 0; j < lenaMatup.cols; j++)
		{
			//��t�@��Anearest neighbor�����ɳ̪񪺭�
			if (i >= (lenaMatup.rows - times) && j >= (lenaMatup.cols - times)) {
				lenaMatup.at<uchar>(i, j) = lenaMatup.at<uchar>(i - 1, j - 1);
			}
			else if (i >= (lenaMatup.rows - times)) {
				lenaMatup.at<uchar>(i, j) = lenaMatup.at<uchar>(i, j);
			}
			else if (j >= (lenaMatup.cols - times)) {
				lenaMatup.at<uchar>(i,j) = lenaMatup.at<uchar>(i, j - 1);
			}
			else {
				//X,Y��Ϯy�СAi,j��j�Ϯy��
				int X = i / times;
				int Y = j / times;

				int center[2] = { (2 * X + 1) * times / 2,(2 * Y + 1) * times / 2 }; //�����4�Ӭ۾Fpixel������j�ϫ᪺�����I�A�n�M�w�n�����ӭ�

				//�ھ�4�ӶH���A���O���J��������Ϯy��
				if (i >= center[0] && j >= center[1])
					lenaMatup.at<uchar>(i, j) = lenaMat.at<uchar>(X + 1, Y + 1);
				if (i >= center[0] && j < center[1])
					lenaMatup.at<uchar>(i, j) = lenaMat.at<uchar>(X + 1, Y);
				if (i < center[0] && j < center[1])
					lenaMatup.at<uchar>(i, j) = lenaMat.at<uchar>(X, Y);
				if (i < center[0] && j >= center[1])
					lenaMatup.at<uchar>(i, j) = lenaMat.at<uchar>(X, Y + 1);
			}
		}
	}	

	return lenaMatup;
}
//�ھڭ��4��pxiel���j�������ʴ���
Mat bilinear(Mat lenaMat, float times) {
	int height = lenaMat.rows;
	int width = lenaMat.cols;
	Mat lenaMatup = Mat(height * times, width * times, CV_8UC1);

	//��t�L�k�ɨ�A���D�[�Jpadding
	for (int i = 0; i < lenaMatup.rows - times; i++)
	{
		for (int j = 0; j < lenaMatup.cols - times; j++)
		{
			//X,Y��Ϯy�СAi,j��j�Ϯy��
			int X = i / times;
			int Y = j / times;

			//���4��pixel���Ƕ���
			float F11 = lenaMat.at<uchar>(X, Y);
			float F21 = lenaMat.at<uchar>(X + 1, Y);
			float F12 = lenaMat.at<uchar>(X, Y + 1);
			float F22 = lenaMat.at<uchar>(X + 1, Y + 1);

			//���4��pixel������j�Ϫ��y��
			float x1 = X*times;
			float x2 = (X+1)*times;
			float y1 = Y*times;
			float y2 = (Y+1)*times;

			//�a�J���u�ʴ��Ȥ�{��
			lenaMatup.at<uchar>(i, j) = int(1 / ((x2 - x1) * (y2 - y1)) * (F11 * (x2 - i) * (y2 - j) + F21 * (i - x1) * (y2 - j) + F12 * (x2 - i) * (j - y1) + F22 * (i - x1) * (j - y1)));
		}
	}
	return lenaMatup;
}
//�ھڭ��16��pxiel���j�������ʴ���
Mat bicubic(Mat lenaMat, float times) {
	int height = lenaMat.rows;
	int width = lenaMat.cols;

	Mat lenaMatup = Mat(height * times, width * times, CV_8UC1);

	//��t�L�k�ɨ�A���D�[�Jpadding
	for (int i = ceil(times); i < lenaMatup.rows - (ceil(times))*2; i++)
	{
		for (int j = ceil(times); j < lenaMatup.cols - (ceil(times))*2; j++)
		{
			int x = i / times;
			int y = j / times;

			//X,Y�@�w�n>=1
			int X = (int)x;
			int Y = (int)y;

			//��X�ѩ�j�᪺�j�ϡA������Ӥp�Ϯy�ЩҲ��ͪ��p���I�ƭ�
			float u = x - X;
			float v = y - Y;

			//�p��X16�ӦǶ��ȹ������v���Ax�Py��V�U�|��
			float Wx[4], Wy[4];
			bicubic_function(Wx,Wy, u,v);

			lenaMatup.at<uchar>(i, j) = 0;
			for (int a = 0; a < 4; a++) {
				for (int b = 0; b < 4; b++)
				{
					lenaMatup.at<uchar>(i, j) += lenaMat.at<uchar>(X-1+a,Y-1+b)*Wx[a]*Wy[b]; //���ȵ��G��16�I�Ƕ��ȻP�����v�����u�ʲզX
				}
			}
		}
	}

	return lenaMatup;
}
//�p���Ӥ�V�U4���v��
void bicubic_function(float Wx[4],float Wy[4], float u, float v) {
	float a = -0.5;
	//u,v����O������x,y��V��4�Ӱϰ쪺�v��
	//----for abs(x)<=1----//
	//u|v
	Wx[1] = (a + 2) * abs(pow(u,3)) - (a + 3) * pow(u, 2) + 1;
	Wy[1] = (a + 2) * abs(pow(v, 3)) - (a + 3) * pow(v, 2) + 1;
	//1-u|v
	Wx[2] = (a + 2) * abs(pow(1-u,3)) - (a + 3) * pow(1-u, 2) + 1;
	Wy[2] = (a + 2) * abs(pow(1 - v, 3)) - (a + 3) * pow(1 - v, 2) + 1;

	//---for 1<=abs(x)<=2---//
	//u|v+1
	Wx[0] = a * abs(pow(u + 1, 3)) - 5 * a * pow(u + 1, 2) + 8 * a * abs(u + 1) - 4 * a;
	Wy[0] = a * abs(pow(v + 1, 3)) - 5 * a * pow(v + 1, 2) + 8 * a * abs(v + 1) - 4 * a;
	//2-u|v
	Wx[3] = a * abs(pow(2-u,3)) - 5 * a * pow(2-u, 2) + 8 * a * abs(2-u) - 4 * a;
	Wy[3] = a * abs(pow(2 - v, 3)) - 5 * a * pow(2 - v, 2) + 8 * a * abs(2 - v) - 4 * a;
}

