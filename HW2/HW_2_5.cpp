#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <math.h>
using namespace cv;
#define WIDTH 20
#define HEIGHT 20
int minstep = 10000;
int trace_step[WIDTH][HEIGHT] = {0};
int trace_minstep[WIDTH][HEIGHT] = { 0 };

void D4(int, int, int, Mat, int range1, int range2 = 80, int range3 = 80);
void D8(int,int,int,Mat,int range1,int range2 = 80,int range3 = 80);
void Dm(int, int, int, Mat, int range1, int range2 = 80, int range3 = 80);



int main2_5() {

	char OriFile[] = "img/map.raw"; 
	FILE* mapFile;
	mapFile = fopen(OriFile, "rb");
	unsigned char* mapi = new unsigned char[WIDTH*HEIGHT];
	fread(mapi, 1, WIDTH * HEIGHT, mapFile);

	/*-----------------------------------------star-----------------------------------*/
	Mat mapMat = Mat(HEIGHT, WIDTH, CV_8UC1, mapi); 
	
	//顯示map原圖
	printf("------map原圖------\n");
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++) {
			printf("%4d", mapMat.data[i*mapMat.cols+ j]);
		}
		printf("\n");
	}
	

	int star = 1;

	Mat min_map = Mat(HEIGHT, WIDTH, CV_8UC1);
	while (star) {
		minstep = 10000;

		int input;
		int range;
		int R1 = 80;
		int R2 = 80;
		int R3 = 80;
		printf("\n請輸入走迷宮的方法\n\
			\t數字1:D4\n\
			\t數字2:D8\n\
			\t數字3:Dm\n\
			\n\
			\t數字0:退回題號選擇\n");
		scanf("%d", &input);
		if (input == 0)
		{
			star = 0;
			continue;
		}
		printf("\n請輸入允許走的範圍\n\
			\t數字1:{80}\n\
			\t數字2:{80,160}\n\
			\t數字3:{80,160,255}\n");
		scanf("%d", &range);


		switch (range) {
		case 1:
			printf("\n允許範圍{80}");
			break;
		case 2:
			printf("\n允許範圍{80,160}");
			R2 = 160;
			break;
		case 3:
			printf("\n允許範圍{80,160,255}");
			R2 = 160;
			R3 = 255;
			break;
		default:
			printf("\n請依照說明輸入數字\n");
			continue;
			break;
		}

		trace_step[0][0] = 1;
		switch (input) {
		case 1:
			printf("使用D4\n");
			D4(0, 0, 0, mapMat, R1, R2, R3);
			break;
		case 2:
			printf("使用D8\n");
			printf("\n請助教稍等30秒. . . . . .\n");
			D8(0, 0, 0, mapMat, R1, R2, R3);
			break;
		case 3:
			printf("使用Dm\n");
			Dm(0, 0, 0, mapMat, R1, R2, R3);
			break;
		default:
			printf("\n請依照說明輸入數字\n");
			continue;
			break;
		}
		printf("\n最短路徑是%d\n", minstep);
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++) {
				min_map.data[i * WIDTH + j] = 0;
				if (trace_minstep[i][j] == 1) {
					trace_minstep[i][j] = 255;
					min_map.data[i * WIDTH + j] = trace_minstep[i][j];
				}
			}
		}
		
		namedWindow("min step", 0); //視窗名
		resizeWindow("min step", 200, 200); //視窗大小
		imshow("min step", min_map);
		waitKey(0);
	}
	return 0;
}
//4鄰方法走迷宮
void D4(int x, int y, int step, Mat map, int range1, int range2, int range3) {
	int way[][2] = {{0,1},{0,-1},{1,0},{-1,0} };
	//到達終點，step>minstep則是為了節省時間，超過當前最小步數就放棄步走了
	if (x == HEIGHT - 1 && y == WIDTH - 1 || step > minstep) {
		//紀錄當前的最短路徑與路線圖，並沒有終止條件，為窮舉所有可能
		if (step < minstep) {
			minstep = step;
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					trace_minstep[i][j] = trace_step[i][j];
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			int nx, ny;//下一步(x,y)
			nx = x + way[i][0];
			ny = y + way[i][1];

			//邊界條件
			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH)
				continue;
			//走過的地方設置為1
			if (trace_step[nx][ny] == 1)
				continue;
			//range1,2,3為可以走的值，題目為80,160,255
			if (map.data[nx * HEIGHT + ny] == range1 || map.data[nx * HEIGHT + ny] == range2 || map.data[nx * HEIGHT + ny] == range3) {
				trace_step[nx][ny] = 1; //走過的地方設置為障礙物，防止再次走到
				D4(nx, ny, step + 1, map, range1, range2, range3); //遞迴函式
				trace_step[nx][ny] = 0; //不能走退回來或走到終點，因此要歸0，之後路過是可以走的
			}
		}
	}
}
//8鄰方法走迷宮
void D8(int x, int y, int step, Mat map,int range1 ,int range2,int range3) {
	int way[][2] = { {1,1},{-1,-1},{1,-1},{-1,1},{0,1},{0,-1},{1,0},{-1,0} };
	if (x == HEIGHT - 1 && y == WIDTH - 1  || step>minstep) {
		if (step < minstep) {
			minstep = step;
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					trace_minstep[i][j] = trace_step[i][j];
		}
	}
	else {
		//與D4只差在方向變為8個
		for (int i = 0; i < 8; i++) {
			int nx, ny;//下一個(x,y)
			nx = x + way[i][0];
			ny = y + way[i][1];

			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH)
				continue;
			if (trace_step[nx][ny] == 1) 
				continue;
			if (map.data[nx * HEIGHT + ny] == range1 || map.data[nx * HEIGHT + ny] == range2 || map.data[nx * HEIGHT + ny] == range3) {
				trace_step[nx][ny] = 1;
				D8(nx, ny, step + 1, map, range1, range2, range3);
				trace_step[nx][ny] = 0;
			}
		}
	}
}
//m鄰方法走迷宮，能4鄰就走4鄰，都沒有才走8鄰
void Dm(int x, int y, int step, Mat map, int range1, int range2, int range3) {
	int way[][2] = { {1,1},{-1,-1},{1,-1},{-1,1},{0,1},{0,-1},{1,0},{-1,0} };
	if (x == HEIGHT - 1 && y == WIDTH - 1 || step > minstep) {
		if (step < minstep) {
			minstep = step;
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					trace_minstep[i][j] = trace_step[i][j];
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			int nx, ny;//下一個(x,y)
			nx = x + way[i][0];
			ny = y + way[i][1];
			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH)
				continue;
			if (trace_step[nx][ny] == 1)
				continue;
			//當8鄰附近的兩個4鄰都不能走時，才能走8鄰
			if (way[i][0] * way[i][1] != 0) { //斜走
				//另外兩邊不得符合指定值
				int R1_boolean = map.data[nx * HEIGHT + y] == range1 || map.data[x * HEIGHT + ny] == range1;
				int R2_boolean = map.data[nx * HEIGHT + y] == range2 || map.data[x * HEIGHT + ny] == range2;
				int R3_boolean = map.data[nx * HEIGHT + y] == range3 || map.data[x * HEIGHT + ny] == range3;
				if (R1_boolean || R2_boolean || R3_boolean) {
					continue;
				}
			}
			if (map.data[nx * HEIGHT + ny] == range1 || map.data[nx * HEIGHT + ny] == range2 || map.data[nx * HEIGHT + ny] == range3) {
				trace_step[nx][ny] = 1;
				Dm(nx, ny, step + 1, map, range1, range2, range3);
				trace_step[nx][ny] = 0;
			}
		}
	}
}