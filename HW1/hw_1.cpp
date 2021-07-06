#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h> //c語言
#include <iostream> //C++

//opencv
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

#include<stdlib.h> //亂數相關函數
#include<time.h> //時間相關函數

using namespace cv;
using namespace std;

//---純 C/C++---//
void read_pixel_rowcol(Mat); //讀取指定行列pixel的灰階值
void read_pixel_flag(Mat); //讀取指定號碼pixel的灰階值
void display_on_screen(Mat); //顯示圖片
void rotate_and_display(Mat); //旋轉與顯示圖片
void random_16(Mat); //切分為16等分，並隨機排序
void assembled_4image(); //合併四張影像
void brightness_add50(Mat); //整張圖亮度增加50
void brightness_random50(Mat); //每個pixel亮度隨機增加/減少0~50

//---opencv---//
void text_opencv(); //以opencv函式庫，打印文字於圖像上

int main()
{

	int height = 256; //圖片高rows
	int width = 256; //圖片寬cols
	int size = width * height; //圖片總像素數量
	char OriFile[] = "img/lena256.raw";    //Input Image name


	//-----------------------1. Read File-----------------------//
	FILE* lenaFile;
	lenaFile = fopen(OriFile, "rb");

	//圖片開啟失敗
	if (lenaFile == NULL) {
		puts("Loading File Error!");
		system("PAUSE");
		exit(0);
	}

	unsigned char* lenai = new unsigned char[size]; //建立一個記憶體位址，改動對應的Mat陣列，
	fread(lenai, 1, size, lenaFile);

	//-----------------------2. Transfer data type to OpenCV-----------------------//
	//Mat type
	Mat lenaMat = Mat(height, width, CV_8UC1, lenai); //Set lenai data into lenaMat,8位1通道灰度圖像 ,Mat是opencv中的矩陣型態


	//GUI
	int select;
	int run = 1;
	printf("根據題號輸入\n題號對應：\
			\n\t1.2-b 輸入 1\
			\n\t1.2-c 輸入 2\
			\n\t1.2-d 輸入 3\
			\n\t1.2-e 輸入 4\
			\n\t1.2-f 輸入 5\
			\n\t1.3-a 輸入 6\
			\n\t1.3-b 輸入 7\
			\n\t2     輸入 8\
			\n\t結束程序輸入 0\n\n");
	while(run)
	{
		printf("輸入題號對應數字:");
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
				printf("請按照對應題號輸入\n");
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
	printf("第123列234行的元素值為%d\n", lena.at<uchar>(123, 234));
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
				printf("第5487個元素值為%d\n", lena.at<uchar>(i, j)); //也可以直接lena.at<uchar>(5487) 就可以找到
				printf("對應座標(%d,%d)\n",i,j);
			}
			count++;
		}
	}
}

void display_on_screen(Mat lena)
{
	printf("顯示原圖\n");
	namedWindow("lena_original", 0); //視窗名
	resizeWindow("lena_original", 256, 256); //視窗大小
	moveWindow("lena_original", 100, 100); //將視窗左上角放在螢幕指定位置
	imshow("lena_original", lena);//display Image	
	waitKey(0);
}

void rotate_and_display(Mat lena)
{
	FILE* resultFile;
	char OutFile[] = "out/rotate_lena.raw";
	resultFile = fopen(OutFile, "wb");
	printf("旋轉圖像\n");
	Mat r_lena = Mat(lena.rows, lena.cols, CV_8UC1);
	for (int i = 0; i < lena.rows; i++)
	{
		for (int j = 0; j < lena.cols; j++)
		{
			r_lena.at<uchar>(j, i) = lena.at<uchar>(i, j); //翻轉即為行列互調
		}
	}
	namedWindow("lena_rotate", 0); //視窗名
	resizeWindow("lena_rotate", 256, 256); //視窗大小
	moveWindow("lena_rotate", 100, 100); //將視窗左上角放在螢幕指定位置
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

	printf("將圖切為16個小塊，隨機擺放\n");
	//建立16個大小為原圖1/16的矩陣，來切割原圖
	Mat lena_random[16]; //要獨立宣告
	for (int i = 0; i < 16; i++)
	{
		lena_random[i]=Mat(lena.rows/4,lena.cols/4, CV_8UC1);
	}
	int r16 = lena.rows / 4; //切割後列的長度
	int c16 = lena.cols / 4; //切割後行的長度
	
	//將原圖拆為16分
	for (int k = 0; k < 16; k++)
	{
		for (int i = 0; i < lena.rows / 4; i++)
		{
			for (int j = 0; j < lena.cols / 4; j++)
			{
				//由k/4，k%4得出k在4x4座標中位置為何，如k=11(0~15),k/4=2,k%4=3,因此座標為2,3(0~3,0~3)
				//因此(k/4)*r16與(k%4)*c16代表lena_random i,j為0時，lena i,j對應的起始座標
				//因此隨著k的變化，能夠取得對應區域的座標點
				lena_random[k].at<uchar>(i, j) = lena.at<uchar>(i + (k / 4) * r16, j + (k % 4) * c16); //切割為16塊，等號後根據k的數值不同，會對應不同區塊
			}
		}
	}
	//opencv函式的切割方法
	//for (int i = 0; i < 16; i++)
	//{
	//	lena_random[i] = lena(Range(r16 * (i / 4), r16 * (i / 4 + 1)), Range(c16 * (i % 4), c16 * (i % 4 + 1)));
	//}

	//show出16張切割後的圖塊
	//for (int j = 0; j < 16; j++)
	//{
	//	namedWindow("lenafile", 0); //視窗名 
	//	resizeWindow("lenafile", 256 / 4, 256 / 4); //視窗大小 
	//	moveWindow("lenafile", 100, 100); //將視窗左上角放在螢幕指定位置 
	//	imshow("lenafile", lena_random[j]);//display image	
	//	waitKey(0); 
	//}

	//開始加入亂數

	int Size = 16;
	int i, poker[16], temp, pos;
	srand(time(NULL));
	//放入數字
	poker[0] = 16;
	//
	Mat random_final[2];
	random_final[0] = Mat(lena.rows, lena.cols, CV_8UC1);
	random_final[1] = Mat(lena.rows, lena.cols, CV_8UC1);

	for (int image_count = 1; image_count <= 2; image_count++) //進行兩次隨機
	{
		for (i = 1; i < Size; i++)
			poker[i] = i;

		//洗牌，將1~16重新排序
		for (i = 0; i < Size; i++) {
			pos = (16 - 1) * rand() / RAND_MAX;
			temp = poker[i];
			poker[i] = poker[pos];
			poker[pos] = temp;
		}
		//印出 0~15的隨機數
		printf("隨機排列第(%d)次為random number: ",image_count);
		for (i = 0; i < 16; i++)
			printf("%d ", poker[i] - 1);

		//將隨機圖像random拼接回原圖
		for (int k = 0; k < 16; k++)
		{
			for (int i = 0; i < lena.rows / 4; i++)
			{
				for (int j = 0; j < lena.cols / 4; j++)
				{
					random_final[image_count-1].at<uchar>(i + (k / 4) * r16, j + (k % 4) * c16) = lena_random[poker[k] - 1].at<uchar>(i, j); //與拆開等式左右互換
				}
			}
		}
		printf("\n");
		namedWindow("lenaFile3", 0); //視窗名
		resizeWindow("lenaFile3", 256, 256); //視窗大小
		moveWindow("lenaFile3", 100, 100); //將視窗左上角放在螢幕指定位置
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
	//根據擺放順序放入矩陣
	Mat PMat[4];
	PMat[3] = Mat(height, width, CV_8UC1, P01i); //unsign 8bit channel 1(灰階)
	PMat[2] = Mat(height, width, CV_8UC1, P02i); //unsign 8bit channel 1(灰階)
	PMat[0] = Mat(height, width, CV_8UC1, P03i); //unsign 8bit channel 1(灰階)
	PMat[1] = Mat(height, width, CV_8UC1, P04i); //unsign 8bit channel 1(灰階)

	//cv:Mat lenaMat(height, width, CV_8UC1, lenai); #方法2
	//Set lenai data into lenaMat

	//-----------------------3. Create window and show your Image-----------------------//

	printf("將四塊圖像拼接\n");
	Mat Mat_mix = Mat(height * 2, width * 2, CV_8UC1);

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				Mat_mix.at<uchar>(i + (k / 2) * height, j + (k % 2) * width) = PMat[k].at<uchar>(i, j); //與random_16中的拼接一模一樣，除了矩陣由4x4變為2x2
			}
		}
	}
	namedWindow("lenaFile4", 0); //視窗名
	resizeWindow("lenaFile4", 256, 256); //視窗大小
	moveWindow("lenaFile4", 100, 100); //將視窗左上角放在螢幕指定位置
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

	printf("將所有像素值+50\n");
	for (int i = 0; i < lena.rows; i++)
	{
		for (int j = 0; j < lena.cols; j++)
		{
			if (lena.at<uchar>(i, j) > (255 - 50)) //將加上50後會>255的數字先減去50，圖像就不會加過頭
				lena.at<uchar>(i, j) = (255 - 50);
			lena_add50.at<uchar>(i, j) = lena.at<uchar>(i, j)+50;
		}
	}
	namedWindow("lenafile_add50", 0); //視窗名 
	resizeWindow("lenafile_add50", 256, 256); //視窗大小 
	moveWindow("lenafile_add50", 100, 100); //將視窗左上角放在螢幕指定位置 
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


	printf("將所有像素值隨機+(-50~50)\n");
	int r_count = rand() % (max - min + 1) + min; //-50~50間的隨機數
	printf("隨機值%d\n", r_count);
	for (int i = 0; i < lena.rows; i++)
	{
		for (int j = 0; j < lena.cols; j++)
		{
			
			if (r_count >= 0)
			{
				//防止加超過255
				if (lena.at<uchar>(i, j) > (255 - r_count))
					lena.at<uchar>(i, j) = (255 - r_count);
			}
			else
			{
				//防止減小於0
				if (lena.at<uchar>(i, j) < (-r_count))
					lena.at<uchar>(i, j) = (-r_count);
			}
			lena_random50.at<uchar>(i, j) = lena.at<uchar>(i, j) + r_count;

		}
	}
	namedWindow("lenafile_random50", 0); //視窗名 
	resizeWindow("lenafile_random50", 256, 256); //視窗大小 
	moveWindow("lenafile_random50", 100, 100); //將視窗左上角放在螢幕指定位置 
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

	printf("在haha圖上寫上學號與想寫的字\n");
	namedWindow("lenaFile", 0); //視窗名
	resizeWindow("lenaFile", 512, 512); //視窗大小
	moveWindow("lenaFile", 100, 100); //將視窗左上角放在螢幕指定位置
	imshow("lenaFile", hahaMat);//display Image	
	waitKey(0);

	imwrite("out/text_opencv.png",hahaMat);
}
