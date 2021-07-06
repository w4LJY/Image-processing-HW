#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h> //c�y��
#include <iostream> //C++

//opencv
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

#include<stdlib.h> //�üƬ������
#include<time.h> //�ɶ��������

using namespace cv;
using namespace std;

//---�� C/C++---//
void read_pixel_rowcol(Mat); //Ū�����w��Cpixel���Ƕ���
void read_pixel_flag(Mat); //Ū�����w���Xpixel���Ƕ���
void display_on_screen(Mat); //��ܹϤ�
void rotate_and_display(Mat); //����P��ܹϤ�
void random_16(Mat); //������16�����A���H���Ƨ�
void assembled_4image(); //�X�֥|�i�v��
void brightness_add50(Mat); //��i�ϫG�׼W�[50
void brightness_random50(Mat); //�C��pixel�G���H���W�[/���0~50

//---opencv---//
void text_opencv(); //�Hopencv�禡�w�A���L��r��Ϲ��W

int main()
{

	int height = 256; //�Ϥ���rows
	int width = 256; //�Ϥ��ecols
	int size = width * height; //�Ϥ��`�����ƶq
	char OriFile[] = "img/lena256.raw";    //Input Image name


	//-----------------------1. Read File-----------------------//
	FILE* lenaFile;
	lenaFile = fopen(OriFile, "rb");

	//�Ϥ��}�ҥ���
	if (lenaFile == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}

	unsigned char* lenai = new unsigned char[size]; //�إߤ@�ӰO�����}�A��ʹ�����Mat�}�C�A
	fread(lenai, 1, size, lenaFile);

	//-----------------------2. Transfer data type to OpenCV-----------------------//
	//Mat type
	Mat lenaMat = Mat(height, width, CV_8UC1, lenai); //Set lenai data into lenaMat,8��1�q�D�ǫ׹Ϲ� ,Mat�Oopencv�����x�}���A


	//GUI
	int select;
	int run = 1;
	printf("�ھ��D����J\n�D�������G\
			\n\t1.2-b ��J 1\
			\n\t1.2-c ��J 2\
			\n\t1.2-d ��J 3\
			\n\t1.2-e ��J 4\
			\n\t1.2-f ��J 5\
			\n\t1.3-a ��J 6\
			\n\t1.3-b ��J 7\
			\n\t2     ��J 8\
			\n\t�����{�ǿ�J 0\n\n");
	while(run)
	{
		printf("��J�D�������Ʀr:");
		scanf("%d", &select);
		switch (select)
		{
			case 1:
				/*---1.2(b1)---*/
				printf("-------------------------1.2(b1)----------------------------------\n");
				read_pixel_rowcol(lenaMat);

				/*---1.2(b2)---*/
				printf("-------------------------1.2(b2)----------------------------------\n");
				read_pixel_flag(lenaMat);
				break;

			case 2:
				/*---1.2(c)---*/
				printf("-------------------------1.2(c)----------------------------------\n");
				display_on_screen(lenaMat);
				break;

			case 3:
				/*---1.2(d)---*/
				printf("-------------------------1.2(d)----------------------------------\n");
				rotate_and_display(lenaMat);
				break;

			case 4:
				/*---1.2(e)---*/
				printf("-------------------------1.2(e)----------------------------------\n");
				random_16(lenaMat);
				break;

			case 5:
				/*---1.2(f)---*/
				printf("-------------------------1.2(f)----------------------------------\n");
				assembled_4image();
				break;

			case 6:
				/*---1.3(a)---*/
				printf("-------------------------1.3(a)----------------------------------\n");
				brightness_add50(lenaMat);
				break;

			case 7:
				/*---1.3(b)---*/
				printf("-------------------------1.3(b)----------------------------------\n");
				brightness_random50(lenaMat);
				break;

			case 8:
				/*---  2   ---*/
				printf("---------------------------2-------------------------------------\n");
				text_opencv();
				break;
			case 0:
				run = 0;
				break;

			default:
				printf("�Ы��ӹ����D����J\n");
				break;
		}
	
	}
	//-----------------------5. Release memory-----------------------//
	fclose(lenaFile);
	delete lenai;
	destroyWindow("lenaFile");

}
void read_pixel_rowcol(Mat lena)
{
	printf("��123�C234�檺�����Ȭ�%d\n", lena.at<uchar>(123, 234));
}

void read_pixel_flag(Mat lena)
{
	int count = 0;
	for (int i = 0; i < lena.rows; i++)
	{
		for (int j = 0; j < lena.cols; j++)
		{
			if (count == 5487)
			{
				printf("��5487�Ӥ����Ȭ�%d\n", lena.at<uchar>(i, j)); //�]�i�H����lena.at<uchar>(5487) �N�i�H���
				printf("�����y��(%d,%d)\n",i,j);
			}
			count++;
		}
	}
}

void display_on_screen(Mat lena)
{
	printf("��ܭ��\n");
	namedWindow("lena_original", 0); //�����W
	resizeWindow("lena_original", 256, 256); //�����j�p
	moveWindow("lena_original", 100, 100); //�N�������W����b�ù����w��m
	imshow("lena_original", lena);//display Image	
	waitKey(0);
}

void rotate_and_display(Mat lena)
{
	FILE* resultFile;
	char OutFile[] = "out/rotate_lena.raw";
	resultFile = fopen(OutFile, "wb");
	printf("����Ϲ�\n");
	Mat r_lena = Mat(lena.rows, lena.cols, CV_8UC1);
	for (int i = 0; i < lena.rows; i++)
	{
		for (int j = 0; j < lena.cols; j++)
		{
			r_lena.at<uchar>(j, i) = lena.at<uchar>(i, j); //½��Y����C����
		}
	}
	namedWindow("lena_rotate", 0); //�����W
	resizeWindow("lena_rotate", 256, 256); //�����j�p
	moveWindow("lena_rotate", 100, 100); //�N�������W����b�ù����w��m
	imshow("lena_rotate", r_lena);//display Image	
	waitKey(0);

	fwrite(r_lena.ptr(0, 0), 1, 256*256, resultFile);
	fclose(resultFile);
}

void random_16(Mat lena)
{	
	FILE* resultFile,* resultFile2;
	char OutFile[] = "out/random_16_1.raw";
	char OutFile2[] = "out/random_16_2.raw";
	resultFile = fopen(OutFile, "wb");
	resultFile2 = fopen(OutFile2, "wb");

	printf("�N�Ϥ���16�Ӥp���A�H���\��\n");
	//�إ�16�Ӥj�p�����1/16���x�}�A�Ӥ��έ��
	Mat lena_random[16]; //�n�W�߫ŧi
	for (int i = 0; i < 16; i++)
	{
		lena_random[i]=Mat(lena.rows/4,lena.cols/4, CV_8UC1);
	}
	int r16 = lena.rows / 4; //���Ϋ�C������
	int c16 = lena.cols / 4; //���Ϋ�檺����
	
	//�N��ϩ16��
	for (int k = 0; k < 16; k++)
	{
		for (int i = 0; i < lena.rows / 4; i++)
		{
			for (int j = 0; j < lena.cols / 4; j++)
			{
				//��k/4�Ak%4�o�Xk�b4x4�y�Ф���m����A�pk=11(0~15),k/4=2,k%4=3,�]���y�Ь�2,3(0~3,0~3)
				//�]��(k/4)*r16�P(k%4)*c16�N��lena_random i,j��0�ɡAlena i,j�������_�l�y��
				//�]���H��k���ܤơA������o�����ϰ쪺�y���I
				lena_random[k].at<uchar>(i, j) = lena.at<uchar>(i + (k / 4) * r16, j + (k % 4) * c16); //���ά�16���A������ھ�k���ƭȤ��P�A�|�������P�϶�
			}
		}
	}
	//opencv�禡�����Τ�k
	//for (int i = 0; i < 16; i++)
	//{
	//	lena_random[i] = lena(Range(r16 * (i / 4), r16 * (i / 4 + 1)), Range(c16 * (i % 4), c16 * (i % 4 + 1)));
	//}

	//show�X16�i���Ϋ᪺�϶�
	//for (int j = 0; j < 16; j++)
	//{
	//	namedWindow("lenafile", 0); //�����W 
	//	resizeWindow("lenafile", 256 / 4, 256 / 4); //�����j�p 
	//	moveWindow("lenafile", 100, 100); //�N�������W����b�ù����w��m 
	//	imshow("lenafile", lena_random[j]);//display image	
	//	waitKey(0); 
	//}

	//�}�l�[�J�ü�

	int Size = 16;
	int i, poker[16], temp, pos;
	srand(time(NULL));
	//��J�Ʀr
	poker[0] = 16;
	//
	Mat random_final[2];
	random_final[0] = Mat(lena.rows, lena.cols, CV_8UC1);
	random_final[1] = Mat(lena.rows, lena.cols, CV_8UC1);

	for (int image_count = 1; image_count <= 2; image_count++) //�i��⦸�H��
	{
		for (i = 1; i < Size; i++)
			poker[i] = i;

		//�~�P�A�N1~16���s�Ƨ�
		for (i = 0; i < Size; i++) {
			pos = (16 - 1) * rand() / RAND_MAX;
			temp = poker[i];
			poker[i] = poker[pos];
			poker[pos] = temp;
		}
		//�L�X 0~15���H����
		printf("�H���ƦC��(%d)����random number: ",image_count);
		for (i = 0; i < 16; i++)
			printf("%d ", poker[i] - 1);

		//�N�H���Ϲ�random�����^���
		for (int k = 0; k < 16; k++)
		{
			for (int i = 0; i < lena.rows / 4; i++)
			{
				for (int j = 0; j < lena.cols / 4; j++)
				{
					random_final[image_count-1].at<uchar>(i + (k / 4) * r16, j + (k % 4) * c16) = lena_random[poker[k] - 1].at<uchar>(i, j); //�P��}�������k����
				}
			}
		}
		printf("\n");
		namedWindow("lenaFile3", 0); //�����W
		resizeWindow("lenaFile3", 256, 256); //�����j�p
		moveWindow("lenaFile3", 100, 100); //�N�������W����b�ù����w��m
		imshow("lenaFile3", random_final[image_count-1]);//display Image	
		waitKey(0);

	}
	fwrite(random_final[0].ptr(0, 0), 1, 256*256, resultFile);
	fwrite(random_final[1].ptr(0, 0), 1, 256 * 256, resultFile2);
	fclose(resultFile);
	fclose(resultFile2);
}

void assembled_4image() 
{

	int width = 256;
	int height = 256;
	int size = width * height;
	char OriFile01[] = "img/p01.raw";    //Input Image name
	char OriFile02[] = "img/p02.raw";
	char OriFile03[] = "img/p03.raw";
	char OriFile04[] = "img/p04.raw";

	char OutFile[] = "out/assmbled_4image.raw";

	//-----------------------1. Read File-----------------------//
	FILE* P01File, * P02File, * P03File, * P04File, * resultFile;
	P01File = fopen(OriFile01, "rb");
	P02File = fopen(OriFile02, "rb");
	P03File = fopen(OriFile03, "rb");
	P04File = fopen(OriFile04, "rb");

	resultFile = fopen(OutFile, "wb");

	unsigned char* P01i = new unsigned char[size];
	unsigned char* P02i = new unsigned char[size];
	unsigned char* P03i = new unsigned char[size];
	unsigned char* P04i = new unsigned char[size];
	fread(P01i, 1, size, P01File);
	fread(P02i, 1, size, P02File);
	fread(P03i, 1, size, P03File);
	fread(P04i, 1, size, P04File);

	//-----------------------2. Transfer data type to OpenCV-----------------------//
	//-----------------------You can use "Mat"-----------------------//
	//Mat type
	//�ھ��\�񶶧ǩ�J�x�}
	Mat PMat[4];
	PMat[3] = Mat(height, width, CV_8UC1, P01i); //unsign 8bit channel 1(�Ƕ�)
	PMat[2] = Mat(height, width, CV_8UC1, P02i); //unsign 8bit channel 1(�Ƕ�)
	PMat[0] = Mat(height, width, CV_8UC1, P03i); //unsign 8bit channel 1(�Ƕ�)
	PMat[1] = Mat(height, width, CV_8UC1, P04i); //unsign 8bit channel 1(�Ƕ�)

	//cv:Mat lenaMat(height, width, CV_8UC1, lenai); #��k2
	//Set lenai data into lenaMat

	//-----------------------3. Create window and show your Image-----------------------//

	printf("�N�|���Ϲ�����\n");
	Mat Mat_mix = Mat(height * 2, width * 2, CV_8UC1);

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				Mat_mix.at<uchar>(i + (k / 2) * height, j + (k % 2) * width) = PMat[k].at<uchar>(i, j); //�Prandom_16���������@�Ҥ@�ˡA���F�x�}��4x4�ܬ�2x2
			}
		}
	}
	namedWindow("lenaFile4", 0); //�����W
	resizeWindow("lenaFile4", 256, 256); //�����j�p
	moveWindow("lenaFile4", 100, 100); //�N�������W����b�ù����w��m
	imshow("lenaFile4", Mat_mix);//display Image	
	waitKey(0);

	//-----------------------5. Release memory-----------------------//
	fclose(P01File);
	fclose(P02File);
	fclose(P03File);
	fclose(P04File);

	delete P01i;
	delete P02i;
	delete P03i;
	delete P04i;

	destroyWindow("P01File");
	destroyWindow("P02File");
	destroyWindow("P03File");
	destroyWindow("P04File");

	fwrite(Mat_mix.ptr(0, 0), 1, 512*512, resultFile);
	fclose(resultFile);


}

void brightness_add50(Mat lena)
{
	char OutFile[] = "out/brightness_add50.raw";
	FILE* resultFile;
	resultFile = fopen(OutFile, "wb");

	Mat lena_add50 = Mat(lena.rows, lena.cols, CV_8UC1);

	printf("�N�Ҧ�������+50\n");
	for (int i = 0; i < lena.rows; i++)
	{
		for (int j = 0; j < lena.cols; j++)
		{
			if (lena.at<uchar>(i, j) > (255 - 50)) //�N�[�W50��|>255���Ʀr����h50�A�Ϲ��N���|�[�L�Y
				lena.at<uchar>(i, j) = (255 - 50);
			lena_add50.at<uchar>(i, j) = lena.at<uchar>(i, j)+50;
		}
	}
	namedWindow("lenafile_add50", 0); //�����W 
	resizeWindow("lenafile_add50", 256, 256); //�����j�p 
	moveWindow("lenafile_add50", 100, 100); //�N�������W����b�ù����w��m 
	imshow("lenafile_add50", lena_add50);//display image	
	waitKey(0);

	fwrite(lena_add50.ptr(0, 0), 1, 256 * 256, resultFile);
	fclose(resultFile);
}

void brightness_random50(Mat lena)
{
	char OutFile[] = "out/brightness_random50.raw";
	FILE* resultFile;
	resultFile = fopen(OutFile, "wb");

	Mat lena_random50 = Mat(lena.rows, lena.cols, CV_8UC1);
	srand(time(NULL));

	int min = -50;
	int max = 50;


	printf("�N�Ҧ��������H��+(-50~50)\n");
	int r_count = rand() % (max - min + 1) + min; //-50~50�����H����
	printf("�H����%d\n", r_count);
	for (int i = 0; i < lena.rows; i++)
	{
		for (int j = 0; j < lena.cols; j++)
		{
			
			if (r_count >= 0)
			{
				//����[�W�L255
				if (lena.at<uchar>(i, j) > (255 - r_count))
					lena.at<uchar>(i, j) = (255 - r_count);
			}
			else
			{
				//�����p��0
				if (lena.at<uchar>(i, j) < (-r_count))
					lena.at<uchar>(i, j) = (-r_count);
			}
			lena_random50.at<uchar>(i, j) = lena.at<uchar>(i, j) + r_count;

		}
	}
	namedWindow("lenafile_random50", 0); //�����W 
	resizeWindow("lenafile_random50", 256, 256); //�����j�p 
	moveWindow("lenafile_random50", 100, 100); //�N�������W����b�ù����w��m 
	imshow("lenafile_random50", lena_random50);//display image	
	waitKey(0);

	fwrite(lena_random50.ptr(0, 0), 1, 256 * 256, resultFile);
	fclose(resultFile);
}

void text_opencv() 
{
	int height = 256*2;
	int width = 256*2;
	int size = height * width;
	char haha_OriFile[] = "img/haha.raw";
	FILE* hahaFile;
	hahaFile = fopen(haha_OriFile, "rb");

	unsigned char* hahai = new unsigned char[size];

	fread(hahai, 1, size, hahaFile);

	Mat hahaMat = Mat(height, width, CV_8UC1, hahai);

	string text = "109318054";
	string text2 = "LOL";
	int font_face = cv::FONT_HERSHEY_COMPLEX;
	double font_scale = 1;

	Point origin;
	origin.x = hahaMat.cols / 8;
	origin.y = hahaMat.rows / 8;
	putText(hahaMat, text, origin, font_face, font_scale, Scalar(0, 0, 0));

	Point origin2;
	origin2.x = hahaMat.cols / 8 *7;
	origin2.y = hahaMat.rows / 8 *7;
	cv::putText(hahaMat, text2, origin2, font_face, font_scale, Scalar(0, 0, 0));

	printf("�bhaha�ϤW�g�W�Ǹ��P�Q�g���r\n");
	namedWindow("lenaFile", 0); //�����W
	resizeWindow("lenaFile", 512, 512); //�����j�p
	moveWindow("lenaFile", 100, 100); //�N�������W����b�ù����w��m
	imshow("lenaFile", hahaMat);//display Image	
	waitKey(0);

	imwrite("out/text_opencv.png",hahaMat);
}
