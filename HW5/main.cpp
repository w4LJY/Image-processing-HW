int main5(), main5_openCV();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("��J�Ʀr�����D��\n\
		\t�Ʀr1:�ť߸��ഫ C++����\n\
		\t�Ʀr2:�ť߸��ഫ OpenCV����\n\
		\n\
		\t�Ʀr0:�����{��\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			printf("�ЧU�еy��......\n");
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