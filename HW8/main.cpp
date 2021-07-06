int main8_1ab(), main8_1c(), main8_1d(), main8_2a(), main8_2b();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("輸入數字對應題號\n\
		\t數字1:(1)-ab 形態學邊緣, region filling填滿 HW7\n\
		\t數字2:(1)-c 找到所有字元的convex hull \n\
		\t數字3:(1)-d 去除雜訊 \n\
		\t數字4:(2)-a Gray level morphology \n\
		\t數字5:(1)-d 形態學分割區域 \n\
		\n\
		\t數字0:結束程序\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main8_1ab();
			break;
		case 2:
			printf("請助教稍等\n");
			main8_1c();
			break;
		case 3:
			main8_1d();
			break;
		case 4:
			main8_2a();
			break;
		case 5:
			main8_2b();
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