int main3_1(), main3_2(), main3_3(), main3_4();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("輸入數字對應題號\n\
		\t數字1:第1題\n\
		\t數字2:第2題\n\
		\t數字3:第3題\n\
		\t數字4:第4題\n\
		\n\
		\t數字0:結束程序\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main3_1();
			break;
		case 2:
			main3_2();
			break;
		case 3:
			main3_3();
			break;
		case 4:
			main3_4();
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