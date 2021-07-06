#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
using namespace cv;

void filter_conv(Mat, int, int[][3], int); //��J�Ϲ��Bmask�j�p�Bmask�ȡBpadding���k(0:zeros,1:replicated)
Mat zero_padd(Mat, int); //��J�Ϲ�,mask�j�p
Mat re_padd(Mat, int); //��J�Ϲ�,mask�j�p

int main4_3()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char wbF[] = "img/walkbridge.raw";
	FILE* wbFile;
	wbFile = fopen(wbF, "rb");
	unsigned char* wbi = new unsigned char[size];
	fread(wbi, 1, size, wbFile);
	Mat wb = Mat(height, width, CV_8UC1, wbi);
	//--------------------star--------------------//
	int mask_size = 3;
	//2�� laplacian filter
	int lap_filter1[][3] = { {0,-1,0}, {-1,4,-1}, {0,-1,0} };
	int lap_filter2[][3] = { {-1,-1,-1}, {-1,8,-1}, {-1,-1,-1} };

	//---laplacian filter----//
	printf("############laplacian filter##############\n\n");
	//filter1 padding zero
	printf("laplacian filter1 ��0\n");
	filter_conv(wb, mask_size, lap_filter1, 0);
	//filter1 padding replicated
	printf("laplacian filter2 �ɪ����\n");
	filter_conv(wb, mask_size, lap_filter1, 1);
	//filter2 padding zero
	printf("laplacian filter1 ��0\n");
	filter_conv(wb, mask_size, lap_filter2, 0);
	//filter2 padding replicated
	printf("laplacian filter2 �ɪ����\n");
	filter_conv(wb, mask_size, lap_filter2, 1);

	//---High-boost filter---//
	printf("############Height boost##############\n\n");
	//High-boost�u�ݭn�վ�filter���ƭȧY�i�AA�]�m��1��laplacian���G�[�W���
	int A[3] = { 1,2,4 };
	for (int i = 0; i < 3; i++) {
		
		int lap_filter1[][3] = { {0,-1,0}, {-1,4 + A[i],-1}, {0,-1,0} };
		int lap_filter2[][3] = { {-1,-1,-1}, {-1,8 + A[i],-1}, {-1,-1,-1} };
		
		printf("Hight boost A=%d laplacian filter1 �ɪ����\n", A[i]);
		filter_conv(wb, mask_size, lap_filter1, 1);
		printf("Hight boost A=%d laplacian filter2 �ɪ����\n", A[i]);
		filter_conv(wb, mask_size, lap_filter2, 1);
	}


	waitKey(0);

	return 0;
}
void filter_conv(Mat image, int size, int mask[][3], int padding) {
	int height = image.rows, width = image.cols;
	int M_half = (size - 1) / 2;

	Mat image_p;
	if (padding == 0)
		image_p = zero_padd(image, size);
	else
		image_p = re_padd(image, size);

	Mat image_conv = Mat(height, width, CV_8UC1);

	//���n�p��
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double conv_sum = 0;
			for (int a = i - M_half; a <= i + M_half; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					int x = a + M_half - i;
					int y = b + M_half - j;
					conv_sum += image_p.at<uchar>(a + M_half, b + M_half) * mask[x][y];
				}
			}
			image_conv.at<uchar>(i, j) = max(min((int)conv_sum, 255), 0); //������n���G��0~255
		}
	}
	namedWindow("convolution", 0);
	resizeWindow("convolution", height, width);
	imshow("convolution", image_conv);
	waitKey(0);
}
//padding�~���0
Mat zero_padd(Mat image, int size) {
	int height = image.rows, width = image.cols;
	int s_half = (size - 1) / 2;
	Mat image_pad = Mat::zeros(height + 2 * s_half, width + 2 * s_half, CV_8UC1); //padding�᪺�Ϲ��|���j

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			image_pad.at<uchar>(i + s_half, j + s_half) = image.at<uchar>(i, j); //�N�D0���ϰ���ϭ�
		}
	}
	return image_pad;
}
//padding�~��ɪ����
Mat re_padd(Mat image, int size) {
	int height = image.rows, width = image.cols;
	int s_half = (size - 1) / 2;
	Mat image_pad(height + 2 * s_half, width + 2 * s_half, CV_8UC1);
	int p_height = image_pad.rows, p_width = image_pad.cols;

	for (int i = 0; i < p_height; i++) {
		for (int j = 0; j < p_width; j++) {
			//�]�m�|�ӱ���(���O�N�P�_�O�_���n�ɪ��ϰ�)��K��z���l
			int a = !(i <= s_half);
			int b = !(i > p_height - s_half*2);
			int c = !(j <= s_half);
			int d = !(j > p_width - s_half*2);

			image_pad.at<uchar>(i, j) = image.at<uchar>(a * b * (i - s_half) + !b * (height - 1), c * d * (j - s_half) + !d * (height - 1)); //�o�����l��P�_���̸Ӹɪ����
		}
	}
	return image_pad;
}

