int main2_1(), main2_1_3(), main2_2(), main2_3(), main2_5();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("��J�Ʀr�����D��\n\
		\t�Ʀr1:��1�D(1)(2)�p�D\n\
		\t�Ʀr2:��1�D(3)�p�D\n\
		\t�Ʀr3:��2�D\n\
		\t�Ʀr4:��3�D\n\
		\t�Ʀr5:��5�D\n\
		\n\
		\t�Ʀr0:�����{��\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main2_1();
			break;
		case 2:
			main2_1_3();
			break;
		case 3:
			main2_2();
			break;
		case 4:
			main2_3();
			break;
		case 5:
			main2_5();
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