#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#define HEIGHT 540
#define WIDTH 960
int sum_frame[HEIGHT][WIDTH];

//藉由平均的方式來消除雜訊
int main4_2() {

	int input[3] = {10,200,9999}; //分別選10張，200張，全部
	for (int i = 0; i < 3; i++) {
		if(input[i]>500)
			printf("取所有影像平均結果\n");
		else
			printf("取前%d張平均結果\n", input[i]);
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

			//計算pixel總和
			for (int i = 0; i < frame.rows; i++) {
				for (int j = 0; j < frame.cols; j++) {
					sum_frame[i][j] += frame.at<uchar>(i, j);
				}
			}
			number++;
		}
		cap.release();
		destroyAllWindows();
		//計算pixel平均
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
