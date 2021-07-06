#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#define HEIGHT 540
#define WIDTH 960
int sum_frame[HEIGHT][WIDTH];

//�ǥѥ������覡�Ӯ������T
int main4_2() {

	int input[3] = {10,200,9999}; //���O��10�i�A200�i�A����
	for (int i = 0; i < 3; i++) {
		if(input[i]>500)
			printf("���Ҧ��v���������G\n");
		else
			printf("���e%d�i�������G\n", input[i]);
		for (int a = 0; a < HEIGHT; a++)
			for (int b = 0; b < WIDTH; b++)
				sum_frame[a][b] = 0;
		//int input = 200;
		VideoCapture cap("img/street.avi");
		if (!cap.isOpened()) {
			cout << "Error opening video" << endl;
			return -1;
		}
		Mat ave_video = Mat(HEIGHT, WIDTH, CV_8UC1);
		int number = 0;
		while (1) {
			Mat frame;
			cap >> frame;
			if (frame.empty() || number >= input[i])
				break;

			cvtColor(frame, frame, COLOR_BGR2GRAY);

			//�p��pixel�`�M
			for (int i = 0; i < frame.rows; i++) {
				for (int j = 0; j < frame.cols; j++) {
					sum_frame[i][j] += frame.at<uchar>(i, j);
				}
			}
			number++;
		}
		cap.release();
		destroyAllWindows();
		//�p��pixel����
		for (int i = 0; i < HEIGHT; i++) {
			for (int j = 0; j < WIDTH; j++) {
				ave_video.at<uchar>(i, j) = sum_frame[i][j] / number;
			}
		}
		imshow("", ave_video);
		waitKey(0);
	}
	return 0;
}
