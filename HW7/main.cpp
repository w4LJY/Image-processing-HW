int main7_1a(), main7_1b(), main7_2();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("��J�Ʀr�����D��\n\
		\t�Ʀr1:(1)-a Notch filter\n\
		\t�Ʀr2:(1)-b band reject filter\n\
		\t�Ʀr3:(2) Debluring inverse filter & Wiener filter\n\
		\n\
		\t�Ʀr0:�����{��\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main7_1a();
			break;
		case 2:
			main7_1b();
			break;
		case 3:
			main7_2();
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