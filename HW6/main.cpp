int main6_1(), main6_2a(), main6_2b();
#define _CRT_SECURE_NO_DEPRECATE
#include<stdio.h>
#include<stdlib.h>

using namespace std;

int main() {
	int star = 1;

	while (star) {
		printf("块计癸莱肈腹\n\
		\t计1:(1)   Homomorphic filtering\n\
		\t计2:(2)-a ideal LPF\n\
		\t计3:(2)-b Gaussian HPF\n\
		\n\
		\t计0:挡祘\n");
		int input;
		scanf("%d", &input);

		switch (input) {
		case 1:
			main6_1();
			break;
		case 2:
			main6_2a();
			break;
		case 3:
			main6_2b();
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