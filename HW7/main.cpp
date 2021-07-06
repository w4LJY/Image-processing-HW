int main7_1a(), main7_1b(), main7_2();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("块计癸莱肈腹\n\
		\t计1:(1)-a Notch filter\n\
		\t计2:(1)-b band reject filter\n\
		\t计3:(2) Debluring inverse filter & Wiener filter\n\
		\n\
		\t计0:挡祘\n");
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