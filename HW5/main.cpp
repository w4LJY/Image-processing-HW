int main5(), main5_openCV();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("輸入數字對應題號\n\
		\t數字1:傅立葉轉換 C++版本\n\
		\t數字2:傅立葉轉換 OpenCV版本\n\
		\n\
		\t數字0:結束程序\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			printf("請助教稍等......\n");
			main5();
			break;
		case 2:
			main5_openCV();
			break;
		case 0:
			star = 0;
			continue;
			break;
		}
		system("cls");
	}
	return 0;
}