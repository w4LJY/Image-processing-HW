int main4_1_1(), main4_1_2(), main4_2(), main4_3(), main4_4();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("��J�Ʀr�����D��\n\
		\t�Ʀr1:��1�D(1)�p�D\n\
		\t�Ʀr2:��1�D(2)�p�D\n\
		\t�Ʀr3:��2�D\n\
		\t�Ʀr4:��3�D\n\
		\t�Ʀr5:��4�D\n\
		\n\
		\t�Ʀr0:�����{��\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main4_1_1();
			break;
		case 2:
			printf("�ЧU�еy��......\n�o�D�n�ܤ[\n");
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