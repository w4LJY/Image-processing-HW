int main3_1(), main3_2(), main3_3(), main3_4();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("��J�Ʀr�����D��\n\
		\t�Ʀr1:��1�D\n\
		\t�Ʀr2:��2�D\n\
		\t�Ʀr3:��3�D\n\
		\t�Ʀr4:��4�D\n\
		\n\
		\t�Ʀr0:�����{��\n");
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