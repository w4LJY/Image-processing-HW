int main8_1ab(), main8_1c(), main8_1d(), main8_2a(), main8_2b();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("��J�Ʀr�����D��\n\
		\t�Ʀr1:(1)-ab �κA����t, region filling�� HW7\n\
		\t�Ʀr2:(1)-c ���Ҧ��r����convex hull \n\
		\t�Ʀr3:(1)-d �h�����T \n\
		\t�Ʀr4:(2)-a Gray level morphology \n\
		\t�Ʀr5:(1)-d �κA�Ǥ��ΰϰ� \n\
		\n\
		\t�Ʀr0:�����{��\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main8_1ab();
			break;
		case 2:
			printf("�ЧU�еy��\n");
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