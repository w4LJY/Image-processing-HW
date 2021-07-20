# HW3

 
# 1.	Bit Plane
## (1).	將lena與baboon兩張圖合成為lena_with_baboon，並隱藏baboon。最後以MSE與PSNR評估影像損失。
將問題分成取出bit plane與合成圖像。
### a.	取出bit plane
取出Bit plane的動作其實就是灰階圖分割，能將256色共8個bit的圖像分為0~7bit。其中對於圖像價值最高的為7bit，最低為0bit。

要將bit plane取出，只要**設定對應的遮罩後，用C語言中的&按位運算，就能得到結果**，如下圖7bit的取法。

![](https://i.imgur.com/HfmsxRC.png)


但取出之後，得到的二值影像不是0與255，而是0與2n-1，n為要取的bit數。所以**先歸一化為0與1，在拉伸到0與255**，就能得到一張只有黑跟白的bit plane。

  

公式中的a(x,y)為原本的灰階值，b(x,y)為取bit plane後的灰階值，n為要取的bit數。

程式演算法為：
```c=
bit_image.at<uchar>(i, j) = (image.at<uchar>(i,j) & byte)*((float)255/(byte));
```

下面顯示lena_256 的7bit~0bit圖像。

* A.	lena_256 7bit
 
    ![](https://i.imgur.com/ZLYRawX.png)

* B.	lena_256 6bit
 
    ![](https://i.imgur.com/ZL6VlSJ.png)

* C.	lena_256 5bit
 
    ![](https://i.imgur.com/AUEOqdQ.png)

* D.	lena_256 4bit
 
    ![](https://i.imgur.com/ZYacTv0.png)

* E.	lena_256 3bit
 
    ![](https://i.imgur.com/ile6waN.png)

* F.	lena_256 2bit
 
    ![](https://i.imgur.com/qWzL6tn.png)

* G.	lena_256 1bit
 
    ![](https://i.imgur.com/7ZRVc6b.png)

* H.	lena_256 0bit
 
    ![](https://i.imgur.com/2sBn59h.png)

### b.	合成圖像

合成圖像就是將兩張圖各取幾個bit plane然後合起來，但重點是能使合成後的lena_with_baboon與lena有著夠高的相似度，並能保持lena和baboon不會損失太多。

經過我多次試驗後發現，分別取lena_256與baboon_256的7~4bit也就是價值最高的4bit來合成，對兩者的損失最小。

而合成的順序為lena_256的4個bit放在7~4bit的位置，baboon_256的4個bit放在3~0bit的位置。這樣能很好的隱藏baboon_256，並且與lena有著最高的相似度。

將bit plane合成為圖像時需注意，需將**歸一化後拉伸的0~255的灰階值，還原成0 ~ 2^n**，n為放入的bit數。

 
![](https://i.imgur.com/IcwXybo.png)

公式中A為合成後的影像，b(n)為要放在第n個bit的bit  plane影像，n為放回的bit數

程式演算法為：
```c=
int sort_lena[8] = {8,7,6,5};
int sort_baboon[8] = {4,3,2,1};
```
```c=
for (int i = 0; i < 8; i++) {
//merge前，同樣需要將bit plane還原成0~2^n
//sort_lena與sort_baboon創建矩陣只定義前4個值，後4個值為0，不會有任何動作
lena_merge += bit_plane(lena, 8-i ) / 255 * (int)(pow(2, sort_lena[i]-1));
baboon_merge += bit_plane(baboon, 8-i) / 255 * (int)(pow(2, sort_baboon[i]-1));
}
merge_image = lena_merge + baboon_merge;
```

將合成的圖像與lena_256進行MSE與PSNR的評估。

* A.	lena_with_baboon
與lena_256的MSE: 28.01 PSNR: 33.66

    ![](https://i.imgur.com/pwd7uXq.png)

 

## (2).	將baboon_256由lena_with_baboon中拆出，並評估其MSE與PSNR，目的是能在合成時盡可能減少baboon的損失。

我將baboon與lena都從lena_with_baboon拆出做分析，拆出bit需要知道放進去時是哪幾個bit，lena就是lena_with_baboon的7~4bit，baboon就是lena_with_baboon的3~0bit，接著使用第一題的取出bit plane與合成影像，就能拆處baboon與lena。

但因為合成時各使用7~4bit的關係，我失去了後4bit的圖像，因此我在還原lena與baboon的過程，**重複將lena與baboon的後4bit放入前4bit的影像**，增加其相似性。
```c=
int sort_lena[8] = { 8,7,6,5 };
int sort_baboon[8] = { 4,3,2,1 };
```
```c=
for (int i = 0; i < 4; i++) {
    lena_splite += bit_plane(merge, sort_lena[i]) / 255 * (int)(pow(2, 7 - i));
    baboon_splite += bit_plane(merge, sort_baboon[i]) / 255 * (int)(pow(2, 7 - i ));
}
for (int i = 0; i < 4; i++) {
    lena_splite += bit_plane(merge, sort_lena[i]) / 255 * (int)(pow(2, 3 - i));
    baboon_splite += bit_plane(merge, sort_baboon[i]) / 255 * (int)(pow(2, 3 - i));
}
```
 
* A.	合成圖拆出的lena
與lena的MSE：33.74 PSNR：32.85 

    ![](https://i.imgur.com/mykLuo0.png)

* B.	合成圖拆出的baboon
與lena的MSE：29.47 PSNR：33.43

    ![](https://i.imgur.com/blwVh9M.png)


# 2.	Bit-plane and Negative
將8個bit-plane隨機打亂並在其中幾張加上負片，要求還原成原圖像。

---
要將8個bit-plane還原成原圖，首先要找出8個bit-plane的順序，用肉眼來判斷的話，可以依據信息量來判斷，**越高位所攜帶的信息量越高，越清晰，而越低位則逐漸遞減，甚至到後面看起來如同雜訊**。

經過我的觀察後將a~h 8張圖排列順序由0~7bit排成b,g,h,c,a,e,d,f。

接著要觀察哪幾張圖片有經過負片處理，老實說一開始看了半天，只有發現h的招牌顏色反了，之後對後四個bit分別做了負片處理來比對在圖像上天空的真實程度，才又找出了e,d。所以最後我對了d,e,h做了負片處理。

負片公式為：

![](https://i.imgur.com/qQr8aVy.png)

 
r是原圖的灰階值，L是灰階範圍，s是經負片轉換後的灰階值。

程式演算法為：

```c=
//正確順序
int sort[8] = {5,3,4,0,2,7,6,1};

//將負片圖轉回來
Image_Negatives(image[3]);
Image_Negatives(image[4]);
Image_Negatives(image[7]);

for (int i = 0; i < 8; i++) 
    merge_image += image[sort[i]] / 255 * (int)(pow(2, i));
```
 

* A.	還原後的圖像

    ![](https://i.imgur.com/rrPql0j.png)


# 3.	Gray Level Transformation

根據下列方式，修正暗圖livingroom_d512，亮圖cameraman_b512
## (1).	 Log轉換
 log轉換根據方程式![](https://i.imgur.com/Bi4a7ae.png)
 
```c=
int r = image.at<uchar>(i, j);
log_image.at<uchar>(i, j) = c *log10(1+r);
```
Log轉換主要功能是**改善暗部**，而與他對應的Inverse log則是改善亮部。

因此log transformation使用於暗圖livingroom_d512較為適合，而倍率c的調整，在c>1時能增加log的陡峭程度，c<0則相反。

但依照**題目給定的c=20 與 c=100都還是太小，無法將暗部拉得夠亮**，但過我的試驗，c=500時效果還不錯。

![](https://i.imgur.com/rgBpR3Y.png)

   而將cameraman使用log轉換，我認為是沒什麼效果的，cameraman的問題是過亮，用log只會讓他再變更亮而已

* A.	Livingroom (log轉換，c=20)

    ![](https://i.imgur.com/KDhtjQz.png)

 
* B.	Livingroom (log轉換，c=100)

    ![](https://i.imgur.com/CiOmw1O.png)

* C.	Cameraman(log轉換，c=20)

    ![](https://i.imgur.com/iRxfpbc.png)

* D.	Cameraman(log轉換，c=100)

    ![](https://i.imgur.com/g8IGnHz.png)

 
## (2).	Power-law轉換
Power-law增強暗部還是亮部取決於γ，**當γ>0時為增強亮部，當γ<0時為增強暗部，而c同樣是決定power-law曲線的曲度，c>0曲線變陡峭，c<0相反**。Power-law的轉換方程式為![](https://i.imgur.com/twg9q29.png)
。

```c=
int r = image.at<uchar>(i, j);
powerlaw_image.at<uchar>(i, j) = c*pow(r,gamma);
```
 
因為power-law暗部與亮部皆有辦法處理，因此對livingroom_d512與cameraman_b512都能有所改善。
    其中livingroom_d512為暗圖，因此在**γ<0時能有所改善**，若增強幅度不夠能將γ調小，或是加上>0的倍率c。
    
而我在調整livingroom_d512時，以γ=0.2下去調，發現暗部增強的不夠，圖像還是偏暗，因此將倍率c調整至345，得到較好的效果。

Cameraman_d512為亮圖，因此在**γ>0時能有所改善**，若增強幅度不夠能將γ調大，或是加上>0的倍率c。

而我在調整cameraman_b512時，以γ=10下去調，發現亮部改善幅度過大，因此將倍率c調小為1e-22。若不想將c調整的那麼小，也可以用較小的γ來調整，也能獲得類似的結果。

* A.	Livingroom(power-law轉換 γ=0.2 c=345)

    ![](https://i.imgur.com/jYLBNOh.png)

 
* B.	Livingroom(power-law轉換 γ=10 c= 1e-11)
 
     ![](https://i.imgur.com/Io94KcH.png)

* C.	Cameraman(power-law轉換 γ=0.2 c=75)

    ![](https://i.imgur.com/CxlGPAS.png)

 
* D.	Cameraman(power-law轉換 γ=10 c=1e-22)

    ![](https://i.imgur.com/fvHu19N.png)

 

# 4.	Histogram Equalization
## (1).	繪製livingroom_d512與cameraman_b512的直方圖，畫圖部分可以使用opencv。並探討直方圖間的差異性。

直方圖的計算就是**計算每個灰階值出現的次數**，並將其做成圖表，而**圖表的橫軸就是灰階範圍0~255，圖表縱軸則可以是灰階值出現次數或機率等可看出次數差異的表示法**。

程式演算法-計算直方圖
```c=
//計算直方圖
for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
        int r = image.at<uchar>(i, j);
        hist[r]++; //每個顏色出現次數
    }
}
```
有了直方圖，能夠很直觀的看出灰階分布，圖像亮暗等資訊。
		   
畫直方圖時，縱軸可以用PDF的機率顯示，也可以直接用次數。但我使用機率方式顯示發現機率小的灰階值看不太出分別。

將機率做為直方圖縱軸的程式演算法。
```c=
int size = width * height;
```
```c=
float nomalize[256];
for (int i = 0; i < 256; i++) 
    nomalize[i] = hist[i] / (float)max;
```
 

Cameraman_b512的直方圖，因為縱軸為PDF，不方便看出灰階出現次數差異性。

![](https://i.imgur.com/A7NV9ig.png)

 

因此改為用次數來顯示，但因為要方便觀看，不可能真的將真實的灰階出現次數印出，如livingroom_d512中最大灰階值出現次數高達80000多，因此我先**將灰階值的出現次數根據直方圖中的最大值歸一化**。

```c=
//找到直方圖中的最大值
int max = 0;
for (int i = 0; i < 256; i++) {
    if (hist[i] > max)
        max = hist[i];
}
//將直方圖根據直方圖中的最大值歸一化
float nomalize[256];
for (int i = 0; i < 256; i++) 
    nomalize[i] = hist[i] / (float)max;
```

Cameraman_b512的直方圖，縱軸為對直方圖最大值歸一化結果，明顯更方便於觀看。

![](https://i.imgur.com/KsrbBdT.png)

 
接著在畫圖時，將**圖像高度乘上歸一化結果**，就能清楚的看到直方圖結果。



我使用來畫直方圖的函式為cv::line，比較重要的參數除了輸入圖像外，就是線段的起點與終點，只要注意圖像的起點是左上角就不會畫錯了。

程式演算法-畫直方圖
```c=
int hist_H = 200;//縱軸的高度
Mat histogram_image(hist_H, 256, CV_8UC1);
//初始化histogram_image為0
for (int i = 0; i < hist_H; i++)
    for (int j = 0; j < 256; j++)
        histogram_image = 0;

for (int i = 0; i < 256; i++)
    line(histogram_image, Point(i, hist_H-1), Point(i, (hist_H-1) - round(nomalize[i]*(hist_H-1))), Scalar(255));
```
 

* A.	livingroom_d512直方圖

    ![](https://i.imgur.com/tFCjSFt.png)

 
* B.	cameraman_b512直方圖

    ![](https://i.imgur.com/z9RthKo.png)

 
根據兩個直方圖的灰階分布，可以看出，livingroom_d512的亮度集中於暗部。Cameraman_b512集中於亮部。且兩者的顏色對比度都很低。

## (2).	將livingroom_d512與cameraman_b512做直方圖等化，並將等化結果的直方圖畫出，並與(1)來做比較。

直方圖等化的原理就是增強直方圖對比度，將直方圖由**原先集中於某個灰度區間變為在所有灰度範圍內均勻分布**。
    要能完成直方圖等化需要先算出直方圖的CDF，CDF就是PDF的積分，因此我們可以對將**各個灰階出現次數除上總pixel數後得到PDF，接著積分得到CDF，但因為數位影像是離散的，所以要用累加的方式**。 
    有了CDF後，將**CDF的值對應為等化後直方圖的橫軸，找出原直方圖的舊灰階值對應到的新灰階值位置**。但須要注意的是，CDF的機率是0~1要拉伸為0~255才是對應到正確的灰階值，因此最後還要**乘上255**。
    
程式演算法-直方圖等化
```c=
//計算hist的CDF
for (int k = 0; k < 256; k++) {
    for (int j = 0; j <= k; j++) {
        hist_cdf[k] += hist[j]/(float)size; //hist[j]/size是rj的pdf
    }
}
//將CDF*255找出對應新的顏色位置
Mat image_eq = Mat(image.rows, image.cols, CV_8UC1);
for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
        int r = image.at<uchar>(i, j);
        image_eq.at<uchar>(i, j) = round(hist_cdf[r] * 255);  //CDF對應該pixel的新灰階值，但要將0~1的數值，拉伸到0~255
    }
}
```
 

而最後將等化前與等化後的直方圖做比較，能發現雖然灰階值間，出現次數仍有差異，但相較於等化前灰階值的**分布變得平均，不在局部集中於某幾個區塊，增強了對比度後，也讓我們能更清楚的看到圖像內的資訊**。

* A.	livingroom_d512直方圖等化
 
    ![](https://i.imgur.com/kIeOSua.png)

* B.	cameraman_b512 直方圖等化

    ![](https://i.imgur.com/2ZOTkfi.png)

* C.	livingroom_d512 等化後直方圖

    ![](https://i.imgur.com/JtUVOqA.png)

* D.	cameraman_b512 等化後直方圖

    ![](https://i.imgur.com/5hcFTkO.png)


比較直方圖等化前與後的直方圖，能發現其實長的線段還是長的，短的線段也仍是短的，灰階次數並沒有被改變，而是因為根據CDF平均攤開後，0~255間的灰階分布變得更加平均，這也是讓我們能更方便用眼睛觀察的形式。
    
