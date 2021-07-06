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
	
	//���map���
	printf("------map���------\n");
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
		printf("\n�п�J���g�c����k\n\
			\t�Ʀr1:D4\n\
			\t�Ʀr2:D8\n\
			\t�Ʀr3:Dm\n\
			\n\
			\t�Ʀr0:�h�^�D�����\n");
		scanf("%d", &input);
		if (input == 0)
		{
			star = 0;
			continue;
		}
		printf("\n�п�J���\�����d��\n\
			\t�Ʀr1:{80}\n\
			\t�Ʀr2:{80,160}\n\
			\t�Ʀr3:{80,160,255}\n");
		scanf("%d", &range);


		switch (range) {
		case 1:
			printf("\n���\�d��{80}");
			break;
		case 2:
			printf("\n���\�d��{80,160}");
			R2 = 160;
			break;
		case 3:
			printf("\n���\�d��{80,160,255}");
			R2 = 160;
			R3 = 255;
			break;
		default:
			printf("\n�Ш̷ӻ�����J�Ʀr\n");
			continue;
			break;
		}

		trace_step[0][0] = 1;
		switch (input) {
		case 1:
			printf("�ϥ�D4\n");
			D4(0, 0, 0, mapMat, R1, R2, R3);
			break;
		case 2:
			printf("�ϥ�D8\n");
			printf("\n�ЧU�еy��30��. . . . . .\n");
			D8(0, 0, 0, mapMat, R1, R2, R3);
			break;
		case 3:
			printf("�ϥ�Dm\n");
			Dm(0, 0, 0, mapMat, R1, R2, R3);
			break;
		default:
			printf("\n�Ш̷ӻ�����J�Ʀr\n");
			continue;
			break;
		}
		printf("\n�̵u���|�O%d\n", minstep);
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
		
		namedWindow("min step", 0); //�����W
		resizeWindow("min step", 200, 200); //�����j�p
		imshow("min step", min_map);
		waitKey(0);
	}
	return 0;
}
//4�F��k���g�c
void D4(int x, int y, int step, Mat map, int range1, int range2, int range3) {
	int way[][2] = {{0,1},{0,-1},{1,0},{-1,0} };
	//��F���I�Astep>minstep�h�O���F�`�ٮɶ��A�W�L��e�̤p�B�ƴN���B���F
	if (x == HEIGHT - 1 && y == WIDTH - 1 || step > minstep) {
		//������e���̵u���|�P���u�ϡA�èS���פ����A���a�|�Ҧ��i��
		if (step < minstep) {
			minstep = step;
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++)
					trace_minstep[i][j] = trace_step[i][j];
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			int nx, ny;//�U�@�B(x,y)
			nx = x + way[i][0];
			ny = y + way[i][1];

			//��ɱ���
			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH)
				continue;
			//���L���a��]�m��1
			if (trace_step[nx][ny] == 1)
				continue;
			//range1,2,3���i�H�����ȡA�D�ج�80,160,255
			if (map.data[nx * HEIGHT + ny] == range1 || map.data[nx * HEIGHT + ny] == range2 || map.data[nx * HEIGHT + ny] == range3) {
				trace_step[nx][ny] = 1; //���L���a��]�m����ê���A����A������
				D4(nx, ny, step + 1, map, range1, range2, range3); //���j�禡
				trace_step[nx][ny] = 0; //���ਫ�h�^�өΨ�����I�A�]���n�k0�A������L�O�i�H����
			}
		}
	}
}
//8�F��k���g�c
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
		//�PD4�u�t�b��V�ܬ�8��
		for (int i = 0; i < 8; i++) {
			int nx, ny;//�U�@��(x,y)
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
//m�F��k���g�c�A��4�F�N��4�F�A���S���~��8�F
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
			int nx, ny;//�U�@��(x,y)
			nx = x + way[i][0];
			ny = y + way[i][1];
			if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH)
				continue;
			if (trace_step[nx][ny] == 1)
				continue;
			//��8�F���񪺨��4�F�����ਫ�ɡA�~�ਫ8�F
			if (way[i][0] * way[i][1] != 0) { //�ר�
				//�t�~���䤣�o�ŦX���w��
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