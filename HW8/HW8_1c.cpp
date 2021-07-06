#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include<math.h>
#include<cmath>
#include <vector>

using namespace cv;
using namespace std;

Mat myerode1(Mat, int);
Mat mydilate1(Mat, int);
Mat convexHull(Mat, int[][3], vector<vector<int>>);
vector<vector<int>> Connected_Components(Mat, Mat, int);
Mat threshold(Mat, int);

int main8_1c()
{
	int width = 512;
	int height = 512;
	int size = width * height;
	char adipF[] = "adip.raw";
	FILE* adipFile;
	adipFile = fopen(adipF, "rb");
	unsigned char* adipi = new unsigned char[size];
	fread(adipi, 1, size, adipFile);
	Mat adip = Mat(height, width, CV_8UC1, adipi);
	//---------------------star----------------------//
	Mat adip_b = threshold(adip, 254);
	Mat adip_3(height, width, CV_8UC3);
	for(int i=0;i<height;i++)
		for (int j = 0; j < width; j++) {
			int value = adip.at<uchar>(i, j);
			adip_3.at<Vec3b>(i, j)[0] = value;
			adip_3.at<Vec3b>(i, j)[1] = value;
			adip_3.at<Vec3b>(i, j)[2] = value;
		}
	vector<vector<int>> box = Connected_Components(adip_b, adip_3, 0);
	imshow("adip bounding box", adip_3);

	//(c) convex hull 
	int B1[][3] = { {1,0,0},{1,0,0},{1,0,0} };
	int B2[][3] = { {1,1,1},{0,0,0},{0,0,0} };
	int B3[][3] = { {0,0,1},{0,0,1},{0,0,1} };
	int B4[][3] = { {0,0,0},{0,0,0},{1,1,1} };
	Mat convexB1 = convexHull(adip,B1, box);
	Mat convexB2 = convexHull(adip, B2, box);
	Mat convexB3 = convexHull(adip, B3, box);
	Mat convexB4 = convexHull(adip, B4, box);

	int cB1, cB2, cB3, cB4;
	Mat result = Mat::zeros(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cB1 = convexB1.at<uchar>(i, j);
			cB2 = convexB2.at<uchar>(i, j);
			cB3 = convexB3.at<uchar>(i, j);
			cB4 = convexB4.at<uchar>(i, j);
			if (cB1 != 0 || cB2 != 0 || cB3 != 0 || cB4 != 0)
				result.at<uchar>(i, j) = 255;
		}
	}
	imshow("ori", adip);
	imshow("B1", convexB1);

	imshow("B2", convexB2);

	imshow("B3", convexB3);

	imshow("B4", convexB4);

	imshow("result", result);

	waitKey(0);


	return 0;
}
Mat myerode1(Mat image, int size) {
	int height = image.rows, width = image.cols;
	Mat himage(height, width, CV_8UC1);

	int M_half = (size - 1) / 2;

	int Vmin;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmin = 255;
			for (int a = i - M_half; a <= i + M_half; a++) {
				for (int b = j; b <= j; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (image.at<uchar>(a, b) == 0) {
						Vmin = 0;
						goto here;
					}
				}
			}
		here: himage.at<uchar>(i, j) = Vmin;
		}
	}

	Mat wimage(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmin = 255;
			for (int a = i; a <= i; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (himage.at<uchar>(a, b) == 0) {
						Vmin = 0;
						goto here2;
					}
				}
			}
		here2: wimage.at<uchar>(i, j) = Vmin;
		}
	}

	return wimage;
}
Mat mydilate1(Mat image, int size) {
	int height = image.rows, width = image.cols;
	Mat himage(height, width, CV_8UC1);

	int M_half = (size - 1) / 2;

	int Vmax;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmax = 0;
			for (int a = i - M_half; a <= i + M_half; a++) {
				for (int b = j; b <= j; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (image.at<uchar>(a, b) == 255) {
						Vmax = 255;
						goto here;
					}
				}
			}
		here: himage.at<uchar>(i, j) = Vmax;
		}
	}

	Mat wimage(height, width, CV_8UC1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vmax = 0;
			for (int a = i; a <= i; a++) {
				for (int b = j - M_half; b <= j + M_half; b++) {
					if (a < 0 || b < 0 || a >= height || b >= width) continue;
					if (himage.at<uchar>(a, b) == 255) {
						Vmax = 255;
						goto here2;
					}
				}
			}
		here2: wimage.at<uchar>(i, j) = Vmax;
		}
	}

	return wimage;
}
Mat convexHull(Mat image,int B[][3], vector<vector<int>> box) {
	int h = image.rows, w = image.cols;

	Mat temp(h, w, CV_8UC1);
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			temp.at<uchar>(i, j) = image.at<uchar>(i, j);
	Mat convexImg = Mat::zeros(h, w, CV_8UC1);

	int M_half = (3 - 1) / 2;

	while(true){
		for (int i = M_half; i < h - M_half; i++) {
			for (int j = M_half; j < w - M_half; j++) {
				int conv_sum = 0;
				for (int a = i - M_half; a <= i + M_half; a++) {
					for (int b = j - M_half; b <= j + M_half; b++) {
						
						int x = a - (i - M_half);
						int y = b - (j - M_half);
						if (temp.at<uchar>(a, b) * B[x][y] != 0)
							conv_sum++;
					}
				}
				if (conv_sum == 3)
					convexImg.at<uchar>(i, j) = 255;
				else
					convexImg.at<uchar>(i, j) = temp.at<uchar>(i, j);
			}
		}
		int flag = 0;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (temp.at<uchar>(i, j) == convexImg.at<uchar>(i, j))
					flag++;
				temp.at<uchar>(i, j) = convexImg.at<uchar>(i, j);
			}
		}
		if (flag == h * w)
			break;
	}
	int inbox=0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			for (int a = 0; a < box.size(); a++)
				if (i > box[a][0] && i<box[a][2] && j>box[a][1] && j < box[a][3])
					goto here;
			temp.at<uchar>(i, j) = 0;
		here:continue;
		}
	}
	return temp;
}
Mat threshold(Mat image, int threshold) {
	int h = image.rows, w = image.cols;
	Mat binary(h, w, CV_8UC1);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			binary.at<uchar>(i, j) = image.at<uchar>(i, j) > threshold ? 255 : 0;
		}
	}
	return binary;
}
vector<vector<int>> Connected_Components(Mat image, Mat original, int color) {
	int height = image.rows, width = image.cols;

	vector<vector<int> > eq_table(100, vector<int>(2)); //先預開100*2的衝突表
	vector<int> table_w(2);
	int table_add = 10;

	int way[2][2] = { {-1,0},{0,-1} }; //copy 標籤方向
	int way_label[2];
	int label = 1; //標籤數字
	int count = 0; //table表大小
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (image.at<uchar>(i, j) == 0) continue;
			for (int k = 0; k < 2; k++) {
				int ni = i + way[k][0];
				int nj = j + way[k][1];
				//過邊界，設值為0
				if (ni < 0 || ni >= height || nj < 0 || nj >= width) {
					way_label[k] = 0;
					continue;
				}
				way_label[k] = image.at<uchar>(ni, nj);
			}
			int min_label = min(way_label[0], way_label[1]);
			int max_label = max(way_label[0], way_label[1]);
			//---狀況1:建立新label---//
			if (max_label == 0) {
				image.at<uchar>(i, j) = label;
				label++;
			}
			//---狀況2:直接複製---//
			else if (min_label == 0 || min_label == max_label) {
				image.at<uchar>(i, j) = max_label;
			}
			//---狀況3:處理衝突點---//
			else {
				image.at<uchar>(i, j) = min_label;

				//衝突狀況放入table表
				eq_table[count][0] = min_label;
				eq_table[count][1] = max_label;
				count++;
				//確認table表不重複

				if (count >= eq_table.size())
					eq_table.resize(eq_table.size() + table_add, table_w);

				for (int a = 0; a < count - 1; a++) {
					if (eq_table[a][0] == min_label && eq_table[a][1] == max_label) {
						count--;
						break;
					}
				}
			}
		}
	}

	//printf("label:%d,count:%d\n", label - 1, count);
	//複製一份equivalent table

	vector<vector<int>> eq_copy(count, vector<int>(2));
	for (int i = 0; i < count; i++) {
		eq_copy[i][0] = eq_table[i][0];
		eq_copy[i][1] = eq_table[i][1];

	}

	//將eq_copy整理分類
	for (int i = 0; i < count; i++) {
		int target = eq_copy[i][0];
		int change = eq_copy[i][1];
		for (int j = 0; j < count; j++) {
			if (eq_copy[j][0] == change) eq_copy[j][0] = target;
			if (eq_copy[j][1] == change) eq_copy[j][1] = target;
		}
	}

	vector<vector<int>> eq_relation(10, vector<int>(count));
	vector<int> relation_w(count);
	int relation_add = 3;
	int hc = 0;
	int Nchange = 0;

	for (int i = 0; i < count; i++) {
		if (hc >= eq_relation.size())
			eq_relation.resize(eq_relation.size() + relation_add, relation_w);
		for (int j = 0; j < hc; j++) {
			if (eq_copy[i][0] == eq_relation[j][0]) {
				Nchange = 1;
				break;
			}
		}
		if (Nchange == 1) {
			Nchange = 0;
			continue;
		}
		eq_relation[hc][0] = eq_copy[i][0];
		hc++;
	}
	int* rc_count = (int*)calloc(count, sizeof(int));

	//用開頭數字下去，找到對應的值。
	for (int i = 0; i < hc; i++) {
		int rc = 1;
		for (int j = 0; j < count; j++) {
			if (eq_relation[i][0] == eq_copy[j][0]) {
				eq_relation[i][rc] = eq_table[j][0];
				rc++;
				for (int k = 0; k < rc - 1; k++) {
					if (eq_relation[i][rc - 1] == eq_relation[i][k]) {
						rc--;
						break;
					}
				}
				eq_relation[i][rc] = eq_table[j][1];
				rc++;
				for (int k = 0; k < rc - 1; k++) {
					if (eq_relation[i][rc - 1] == eq_relation[i][k]) {
						rc--;
						break;
					}
				}
			}
		}
		rc_count[i] = rc;
	}
	vector<int> self_label(label - 1);
	int label_count = 1;
	for (int i = 0; i < label - 1; i++) {
		self_label[i] = label_count;
		label_count++;
	}

	for (int i = 0; i < hc; i++) {
		for (int j = 0; j < rc_count[i]; j++) {
			int eq_num = eq_relation[i][j] - 1;
			self_label[eq_num] = 0;
		}
	}
	for (int i = 0; i < label - 1; i++) {
		if (hc >= eq_relation.size())
			eq_relation.resize(eq_relation.size() + relation_add, relation_w);
		if (self_label[i] != 0) {
			eq_relation[hc][0] = self_label[i];
			rc_count[hc] = 1;
			hc++;
		}
	}
	int Clabel;
	vector<vector<int>> draw_point(hc, vector<int>(4));
	//int** draw_point = ge_matrix(hc, 4);
	for (int i = 0; i < hc; i++) {
		for (int j = 0; j < 4; j++) {
			if (j < 2)
				draw_point[i][j] = height;
			else
				draw_point[i][j] = 0;
		}
	}
	vector<int> area(hc, 0);
	vector<vector<int>> centroid(hc, vector<int>(2, 0));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Clabel = image.at<uchar>(i, j);
			if (Clabel == 0) continue;
			for (int x = 0; x < hc; x++) {
				for (int y = 0; y < rc_count[x]; y++) {
					if (eq_relation[x][y] == Clabel) {
						image.at<uchar>(i, j) = (x + 1);
						//Bounding box
						draw_point[x][0] = min(i, draw_point[x][0]);
						draw_point[x][1] = min(j, draw_point[x][1]);
						draw_point[x][2] = max(i, draw_point[x][2]);
						draw_point[x][3] = max(j, draw_point[x][3]);

						//property analysis
						area[x]++;
						centroid[x][0] += i;
						centroid[x][1] += j;
						goto here;
					}
				}
			}
		here:;
		}
	}

	Scalar box_color;
	box_color = color == 0 ? Scalar(0, 0, 255) : Scalar(255, 0, 0);


	for (int i = 0; i < hc; i++) {
		rectangle(original, Point(draw_point[i][1], draw_point[i][0]), Point(draw_point[i][3], draw_point[i][2]), box_color, 2);
		centroid[i][0] /= (float)area[i];
		centroid[i][1] /= (float)area[i];
		circle(original, Point(centroid[i][1], centroid[i][0]), 2, box_color, FILLED);
	}

	return draw_point;
}