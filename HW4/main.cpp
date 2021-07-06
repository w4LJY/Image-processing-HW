int main4_1_1(), main4_1_2(), main4_2(), main4_3(), main4_4();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("輸入數字對應題號\n\
		\t數字1:第1題(1)小題\n\
		\t數字2:第1題(2)小題\n\
		\t數字3:第2題\n\
		\t數字4:第3題\n\
		\t數字5:第4題\n\
		\n\
		\t數字0:結束程序\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main4_1_1();
			break;
		case 2:
			printf("請助教稍等......\n這題要很久\n");
			main4_1_2();
			break;
		case 3:
			main4_2();
			break;
		case 4:
			main4_3();
			break;
		case 5:
			main4_4();
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