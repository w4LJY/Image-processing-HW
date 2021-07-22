# HW2


# 1.	Zooming and Shrinking
## (1).	放大眼睛區域兩倍
分為3個步驟a. 取出眼睛ROI區域、b. 將眼睛區域放大2倍、c. 將放大後的眼睛放回原圖lena_256
### a.	取出眼睛ROI區域：
根據觀察原圖眼睛位置得知其位於圖片中央，加上知道圖像大小為256*256，因此推測出眼睛座標為rows 120~140、cols 120~184，眼睛的ROI取法是兩隻眼睛一起取，如下圖

![](https://i.imgur.com/ikH1rJg.png)
  

### b.	將眼睛區域放大2倍：
因為2倍為整數倍放大，所以我並沒有使用到插值的方式，而是將原圖的**每個像素值重複填四次**到放大圖中，方程式如下
```c
for (int i = 0; i < lena_eyes.rows; i++)
{
    for (int j = 0; j < lena_eyes.cols; j++)
    {
        lena_eyesX2.data[(i * 2) * lena_eyesX2.cols + (j * 2)] = lena_eyes.data[i*lena_eyes.cols+ j];
        lena_eyesX2.data[(i * 2) * lena_eyesX2.cols + (j * 2 + 1)] = lena_eyes.data[i * lena_eyes.cols + j];
        lena_eyesX2.data[(i * 2 + 1)* lena_eyesX2.cols + (j * 2)] = lena_eyes.data[i * lena_eyes.cols + j];
        lena_eyesX2.data[(i * 2 + 1)* lena_eyesX2.cols + (j * 2 + 1)] = lena_eyes.data[i * lena_eyes.cols + j];
    }
}
```

 

而眼睛區域放大的結果如下

![](https://i.imgur.com/XiCYum2.png)



 

### c.	將放大後的眼睛放回原圖lena_256：
要將放大後的眼睛放回原圖需要先找到一個對應點，而我將對應點設定為眼睛的**左上角座標**。而**眼睛的左上角座標對應lena_256的座標就是眼睛在lena_256的中心點座標減去眼睛的長/2與寬/2**。
```c
int eyesX2_star[2] = {(eyes[0]+eyes[1])/2-lena_eyesX2.rows/2,(eyes[2]+eyes[3])/2-lena_eyesX2.cols/2};
```

有了對應座標就可以將眼睛丟回lena_256中了。
```c
for (int i = 0; i < lena_eyesX2.rows; i++)
{
    for (int j = 0; j < lena_eyesX2.cols; j++)
    {
        lenaMat.data[(i+eyesX2_star[0])*lenaMat.cols+ j+eyesX2_star[1]] = lena_eyesX2.data[i*lena_eyesX2.cols+ j];
    }
}
```
 
得到的圖像為

![](https://i.imgur.com/PEbiwnl.png)
 
## (2).	將(1)完成的影像縮小2倍:
Sol. 縮小2倍為整數倍縮小，因此我直接使用捨棄像素的方法，將圖中**偶數倍的行與列上的像素保留**，其他的都直接捨棄，便可得到縮小兩倍的影像，方法如下:

```c
for (int i = 0; i < lenaMat.rows; i++)
{
    for (int j = 0; j < lenaMat.cols; j++)
    {
        if (i % 2 == 0 && j % 2 == 0)
            lenaMat_d2.data[i / 2*lenaMat_d2.cols + j / 2] = lenaMat.data[i*lenaMat.cols+ j]; 
    }
}
```

得到的圖像為

![](https://i.imgur.com/vTcwJES.png)



## (3).	使用插值的三種方法nearest neighbor、bilinear、bicubic來放大圖片，並比較放大2.5縮小2、縮小2放大2.5、放大1.25三種縮放倍率。
插值的第一步就是要找到兩張圖之間的對應座標，而插值的意義就是依據原圖相鄰四個座標的灰階值，對應填入到大圖座標中，並將其中包含所不知道的灰階值根據所使用的方法插入。後面將詳細說明我如何使用插值方法。

![](https://i.imgur.com/AhyblHs.png)

 

### a.	Nearest neighbor:
從字面上來看就知道這個方法是插入離他最近的灰階值，而這個灰階值就是小圖對應倍率填入到大圖中的值。

![](https://i.imgur.com/Gv5GR1r.png)


 
程式設計邏輯，我是將要插的值**分為四個象限**，找到原點座標center，之後**判斷不同象限的座標，設置條件式**插入其對應的小圖灰階值。

![](https://i.imgur.com/IpxeV1N.png)

![](https://i.imgur.com/6BATzTZ.png)

 
Nearest neighbor因為要考慮2x2的矩陣，圖像邊有幾排是無法插值的，因為近鄰是考慮最近的已知灰階，我直接將灰階值參考離他最近的已知灰階點。



```c
if (i >= (lenaMatup.rows - times) && j >= (lenaMatup.cols - times)) {
    lenaMatup.at<uchar>(i, j) = lenaMatup.at<uchar>(i - 1, j - 1);
}
else if (i >= (lenaMatup.rows - times)) {
    lenaMatup.at<uchar>(i, j) = lenaMatup.at<uchar>(i, j);
}
else if (j >= (lenaMatup.cols - times)) {
    lenaMatup.at<uchar>(i,j) = lenaMatup.at<uchar>(i, j - 1);
}
```
 

Nearest neighbor方法的好處就是簡單、快速，為三者間速度最快，同時也最簡單的方法，但其插值方式不夠嚴謹，直接根據周圍四點的值來取，並不能很好的顯示出畫素間的差異性。適合用於對放大後的影像品質沒有太大的要求，但要求效率的情況下使用。

* A.	Nearest neighbor放大2.5縮小2\
花費時間：0.105s

    ![](https://i.imgur.com/IxWpRNv.png)

* B.	Nearest neighbor縮小2放大2.5\
花費時間：0.027s

    ![](https://i.imgur.com/cfrcpHD.png)


* C.	Nearest neighbor放大1.25\
花費時間：0.021s

    ![](https://i.imgur.com/0DAUkHA.png)


 

### b.	Bilinear
Bilinear同樣是根據已知的四點座標灰階值來插入其中的未知座標點，但插入值得選定與Nearest neighbor不同。
Bilinear又稱為雙線性插值，而線性插值的意思是兩點之間根據線性關係插入數值，因此雙線性插值就是在兩個方向分別進行一次的線性插值。

![](https://i.imgur.com/ROBjT0D.png)


要求解其轉換式可以根據下面這條公式，分別代入4個灰階值座標，解4條聯立方程式便可以得到a,b,c,d四個權重。

![](https://i.imgur.com/sIU5hXL.png)

 
有了這四個權重就可以得到方程式，將範圍內的值代入方程式就能得到對應的灰階值。
因為解聯立方程式過於麻煩，因此我**使用整理好的關係方程式**

![](https://i.imgur.com/WAWxbrF.png)

 
其中Q11 =(x1,y1),Q12=(x1,y2) ,Q21=(x2,y1) ,Q22=(x2,y2)為對應四個座標，而f(Q)就是該座標對應的灰階值，代入範圍內的任一座標進入方程式，即可得到該點經由bilinear所插出的數值。

```c
lenaMatup.at<uchar>(i, j) = int(1 / ((x2 - x1) * (y2 - y1)) * (F11 * (x2 - i) * (y2 - j) + F21 * (i - x1) * (y2 - j) + F12 * (x2 - i) * (j - y1) + F22 * (i - x1) * (j - y1)));
```
Bilinear考慮了點與點之間的線性關係，因此插值出來像素間的差異性較符合原圖的感覺，尤其是與nearest neighbor來相比，像素間更加平滑，效果遠勝nearest neighbor的結果。而**效率方面也沒有慢nearest neighbor多少**，若不是需要非常細緻的顯示效果，非常推薦使用該方法。

* A.	Bilinear放大2.5縮小2\
花費時間：0.225s

    ![](https://i.imgur.com/RzGpDaS.png)

* B.	Bilinear縮小2放大2.5\
花費時間0.053s

    ![](https://i.imgur.com/6VCCArY.png)


* C.	Bilinear放大1.25\
花費時間0.053s

    ![](https://i.imgur.com/6kFGo2C.png)




### c.	Bicubic:
Bicubic是三者中最常使用的方法，原理類似於bilinear插值法，但考慮到的點為周圍16點，因此方法複雜許多。
首先可以知道放大圖上未知灰階值的點是灰階值是原圖上的已知**16點灰階值與對應權重的線性組合**\
![](https://i.imgur.com/g4clyE4.png)

其中a(i,j)是小圖上16個座標點的灰階值，B(X,Y)是大圖上未知灰階值的點X,Y，W(i)為橫軸上的權重，W(j)為縱軸上的權重。
至於權重的求法，我是先將目標點X,Y轉換到小圖座標，也就是X/倍率,Y/倍率。接著將目標小數取出，b(x+u,y+v)，有了u和v之後便能找到對應16個點的u與v，如a(0,0)與b(x+u,y+v)的距離為1+u,1+v，將**x方向的四個距離1+u,u,1-u,2-u,與y方向的四個距離1+v,v,1-v,2-v分別代入bicubic的公式**，就可求出8個權重值。

![](https://i.imgur.com/fPPsY0X.png)

bicubic的公式如下
 
![](https://i.imgur.com/3kzBqaC.png)

8個權重代入bicubic的結果
```c
//----for abs(x)<=1----//
//u|v
Wx[1] = (a + 2) * abs(pow(u,3)) - (a + 3) * pow(u, 2) + 1;
Wy[1] = (a + 2) * abs(pow(v, 3)) - (a + 3) * pow(v, 2) + 1;
//1-u|v
Wx[2] = (a + 2) * abs(pow(1-u,3)) - (a + 3) * pow(1-u, 2) + 1;
Wy[2] = (a + 2) * abs(pow(1 - v, 3)) - (a + 3) * pow(1 - v, 2) + 1;

//---for 1<=abs(x)<=2---//
//u|v+1
Wx[0] = a * abs(pow(u + 1, 3)) - 5 * a * pow(u + 1, 2) + 8 * a * abs(u + 1) - 4 * a;
Wy[0] = a * abs(pow(v + 1, 3)) - 5 * a * pow(v + 1, 2) + 8 * a * abs(v + 1) - 4 * a;
//2-u|v
Wx[3] = a * abs(pow(2-u,3)) - 5 * a * pow(2-u, 2) + 8 * a * abs(2-u) - 4 * a;
Wy[3] = a * abs(pow(2 - v, 3)) - 5 * a * pow(2 - v, 2) + 8 * a * abs(2 - v) - 4 * a;
```

**有了bicubic的權重，與16個灰階值線性組合**，就可以插出放大圖後的所有值了。
```c
lenaMatup.at<uchar>(i, j) = 0;
for (int a = 0; a < 4; a++) {
    for (int b = 0; b < 4; b++)
    {
        lenaMatup.at<uchar>(i, j) += lenaMat.at<uchar>(X-1+a,Y-1+b)*Wx[a]*Wy[b]; //插值結果為16點灰階值與對應權重的線性組合
    }
}
```
 
比較需要注意的是，因為需要考慮16個點，也就是4x4的範圍，因此圖片**周圍一圈的值是無法內插**的，這也是我的結果圖會缺外面一整圈的原因。
Bicubic的複雜程度遠勝前兩種方法，用C++程式跑還須要等一會才會顯示出結果，**效率最差**，而使用16值內插的結果毫無疑問的較前兩種方法好，若不考慮效率問題，他一定會是最好的選擇。

* A.	bicubic放大2.5縮小2\
花費時間1.842

    ![](https://i.imgur.com/zlEtO4I.png)


* B.	bicubic縮小2放大2.5\
花費時間0.45s

    ![](https://i.imgur.com/aaj9mxV.png)

* C.	bicubic放大1.25\
花費時間0.443s

    ![](https://i.imgur.com/OZlJAbi.png)



# 2.	G-level resolution
## (1).	將lena_256與baboon_256結果顯示出來，並計算其MSE與PSNR

這兩張影像的灰階值範圍為0~256，以bit來說就是8bit，而量化就是將8bit由二進位的尾巴一個一個去掉，就能得到1~7bit的量化圖形。
而我的想法是將**256個像素值等比例壓縮成目標範圍(0~2^n, n為bit數)，再將其還原成0~256**，但因為壓縮過程中已經損失掉bit數了，因此還原出來的不再是原圖，而是量化結果。

![](https://i.imgur.com/gKDjKs6.png)

其中a是原圖，b是量化結果，n是要量化的bit數。

```c
lenaMat_level.at<uchar>(i, j) = round(lenaMat.at<uchar>(i, j) * (float)(pow(2,level)-1) / 255);
lenaMat_level.at<uchar>(i, j) = round(lenaMat_level.at<uchar>(i, j)  / (float)(pow(2, level)-1) * 255);
```
要注意的是，在程式中的int型態是自動向下取整，因此須配合**4捨5入**才能等比例的壓縮，量化結果才會正確。

* A.	Baboon 8bit

    ![](https://i.imgur.com/5YJGhcc.png)

* B.	Baboon 7bit
MSE:0.507782	
PSNR:51.074031

    ![](https://i.imgur.com/0L2U92Q.png)

* C.	Baboon 6bit
MSE:1.529434
PSNR:46.285496

    ![](https://i.imgur.com/EVadqFe.png)

 
* D.	Baboon 5bit
MSE:5.786575
PSNR:40.506588

    ![](https://i.imgur.com/DpZdvTA.png)

 	
* E.	Baboon 4bit
MSE:23.749008
PSNR:34.374349

    ![](https://i.imgur.com/sI7RWax.png)

 
* F.	Baboon 3bit
MSE:110.9104
PSNR:27.681081

    ![](https://i.imgur.com/qUCyhnN.png)

 
* G.	Baboon 2bit
MSE:595.600723
PSNR:20.381251

    ![](https://i.imgur.com/R8bIHqw.png)

 
* H.	Baboon 1bit
MSE:9678.368744
PSNR:8.272782

    ![](https://i.imgur.com/mZppD0m.png)

 

* A.	Lena 8bit

    ![](https://i.imgur.com/YDawUI7.png)

 
* B.	Lena 7bit
MSE:0.502884
PSNR:51.116126

    ![](https://i.imgur.com/5vP2F1D.png)

* C.	Lena 6bit
MSE:1.5625
PSNR:46.1926

    ![](https://i.imgur.com/PCDj2lT.png)


* D.	Lena 5bit
MSE:5.859085
PSNR:40.452506

    ![](https://i.imgur.com/b4ZTUOQ.png)

 
* E.	Lena 4bit
MSE:24.160583
PSNR:34.299729

    ![](https://i.imgur.com/nXR4LMw.png)

 
* F.	Lena 3bit
MSE:118.109009
PSNR:27.407973

    ![](https://i.imgur.com/fNlYwjs.png)

 
* G.	Lena 2bit
MSE:713.317139
PSNR:19.604070

    ![](https://i.imgur.com/ew1Z5iR.png)

	 
* H.	Lena 1bit
MSE:8522.232986
PSNR:8.272782

    ![](https://i.imgur.com/H4JiL5h.png)

 
# 3.	Isopreference test on gray-level resolution
Isopreference test是一種現象，說明兩件事，第一個是當圖像複雜度固定時，**越大張的圖，越不會受到灰階解析度的影響**。第二個是說當圖像大小固定時，**複雜度越高的圖，越不會受到灰階解析度的影響**。Isopreference test的趨勢如下

![](https://i.imgur.com/tFgEO2c.png)

 
而老師給我們的兩張圖lena_256與baboon_256兩張圖複雜度就差了很多，lena_256是人臉，複雜度很低，而baboon_256的狒狒圖有非常多線條，複雜度非常高。
在第2題的量化問題中，我將lena_256與baboon_256兩張圖在每種量化下一同顯示，可以發現，當量化到4bit時，lena_256中lena的臉部開始出現顏色斷層，整個圖片看起來變得非常不正常，而baboon_256中的狒狒除非很仔細看，不然仍與原圖差不多，要一直持續到2bit左右才能看到baboon_256的失真狀態。
為了瞭解圖像大小對於灰階解析度下的影響，我使用bicubic將lena圖放大3倍，接著同樣在不同量化下同時顯示lena的原圖，與放大後的lena圖，但其顯示後的結果並沒有複雜度的影像那麼嚴重，兩張圖的是在4bit左右開始產生失真，因此我認為Isopreference所講的大圖較不受灰階解析度影響是因為人在看大圖時，會專注於局部，較不會注意到像素色塊間的顏色差異。而看小圖時，一眼就能看完整張圖，哪裡有顏色斷層會看的一清二楚。
		複雜度比較由第三題可看出
* A.	Lena放大3倍圖 8bit

* ![](https://i.imgur.com/mxrPWFd.png)

 
* B.	Lena放大3倍圖 7bit

![](https://i.imgur.com/KT5Bz5x.png)

 
* C.	Lena放大3倍圖 6bit

![](https://i.imgur.com/GwBPTq9.png)

* D.	Lena放大3倍圖 5bit

![](https://i.imgur.com/ZDw3RuH.png)


* E.	Lena放大3倍圖 4bit

![](https://i.imgur.com/LsV3nYf.png)

* F.	Lena放大3倍圖 3bit
 
 ![](https://i.imgur.com/iWxE9fv.png)

* G.	Lena放大3倍圖 2bit

![](https://i.imgur.com/Jfyxlkz.png)

* H.	Lena放大3倍圖 1bit
 
![](https://i.imgur.com/mIGa8Pz.png)


# 5.	Distance and path
map是一張20x20的迷宮圖像，根據D4，D8，Dm三種走法由座標(0,0)走到(19,19)，並且依據灰階{80}，{80,160}，{80,160,255}三種數值來走，顯示最短路徑，並且畫出走法。

---
走迷宮的程式需使用到**遞迴**的演算法，我使用的是**深度搜尋法**，就是將所有可能的路徑全部走過一次，後面介紹我的演算法：
遞迴程式通常會設置一個終止條件，以防遞迴永遠不會結束，但我的方法就是要將所有可能路徑都走一遍，因此我**只設置一個判斷是否走到終點的條件式，而沒有終止條件**。
深度搜尋法就像一個樹狀結構往後延伸，而當所有的可能都計算過後便會停止遞迴。


![](https://i.imgur.com/2fiHvvm.png)

程式中需寫一個到達終點的判斷式，當到達終點時紀錄當前的路徑圖與值，若路徑較當前的最短路徑小的話，就將此路徑設為最短路徑，直到所有路徑都嘗試過一遍，就可以得知最短路徑為多少。

```c
if (x == HEIGHT - 1 && y == WIDTH - 1 || step > minstep) {
    //紀錄當前的最短路徑與路線圖，並沒有終止條件，為窮舉所有可能
    if (step < minstep) {
        minstep = step;
        for (int i = 0; i < 20; i++)
            for (int j = 0; j < 20; j++)
                trace_minstep[i][j] = trace_step[i][j];
    }
}
```
可以注意到我的終點判斷式除了會判斷是否到達終點，還會**判斷當前的步數是否大於當前的最短路徑**。這是因為深度搜尋法真得太過花時間了，尤其是D8走法，沒有個幾十分鐘是跑不完的，因此我將其重嘗試每一種可能更改為嘗試每一種小於當前最短路徑的可能，因為我的目的只是要找出最短路徑，已經確定不可能成為最短路徑的走法就沒有繼續嘗試的必要了，而這個判斷條件也大大增加了程式的效率，原先要幾十分鐘的程式現在幾秒鐘就能夠出來了。

至於遞迴如何走，並不單單只考慮到D4，D8，Dm等走法與可以走的灰階範圍{80}，{80,160}，{80,160,255}，需要**標記當前已走過的路段**，防止**回頭或是繞圈圈**出不來，永遠無法到達終點的狀況發生。而當走到終點後，會回到上一個沒走過的分支開始走，回到分支時，需要**消除分支到終點間的標記**，這樣才能確保所有可能性都能被試過一次。
下面分別敘述D4，D8，Dm三種走法，在程式上的差異性。

### a.	D4：

首先，先定義D4可以走的4個方向

int way[][2] = {{0,1},{0,-1},{1,0},{-1,0} };

找到方向後並不可以直接進入遞迴，要先判斷這個方向是否真的可以走，分別考慮要走的座標**1.是否超出圖像範圍 2.是否之前已經走過，若都沒有發生才考慮3.這個座標上的灰階值是否為指定的灰階範圍。**
確認可以走後，將這一點標記為已走過，接這進入下一遞迴。
當到達終點或是某個遞迴中所有的分支(方向)都走過了，就消除標記值，回到上一遞迴，一直持續到每個遞迴的所有分支都走完。

```c
for (int i = 0; i < 4; i++) {
    int nx, ny;//下一步(x,y)
    nx = x + way[i][0];
    ny = y + way[i][1];

    //邊界條件
    if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH)
        continue;
    //走過的地方設置為1
    if (trace_step[nx][ny] == 1)
        continue;
    //range1,2,3為可以走的值，題目為80,160,255
    if (map.data[nx * HEIGHT + ny] == range1 || map.data[nx * HEIGHT + ny] == range2 || map.data[nx * HEIGHT + ny] == range3) {
        trace_step[nx][ny] = 1; //走過的地方設置為障礙物，防止再次走到
        D4(nx, ny, step + 1, map, range1, range2, range3); //遞迴函式
        trace_step[nx][ny] = 0; //不能走退回來或走到終點，因此要歸0，之後路過是可以走的
    }
}
```
 
D4的走法是三種走法中**路徑最長**的，但其因為只考慮到四個方向，**速度也是最快的**。

### b.	D8
定義D8可以走的8個方向

int way[][2] = { {1,1},{-1,-1},{1,-1},{-1,1},{0,1},{0,-1},{1,0},{-1,0} }; 

接著與D4的完全相同，只差在每個遞迴有8個分支要走
```c
for (int i = 0; i < 8; i++) {
    int nx, ny;//下一個(x,y)
    nx = x + way[i][0];
    ny = y + way[i][1];

    if (nx < 0 || nx >= HEIGHT || ny < 0 || ny >= WIDTH)
        continue;
    if (trace_step[nx][ny] == 1) 
        continue;
    if (map.data[nx * HEIGHT + ny] == range1 || map.data[nx * HEIGHT + ny] == range2 || map.data[nx * HEIGHT + ny] == range3) {
        trace_step[nx][ny] = 1;
        D8(nx, ny, step + 1, map, range1, range2, range3);
        trace_step[nx][ny] = 0;
    }
}
```

D8因為必須考慮到8個方向，是三個走法中**最花時間**的，但**路徑也是三者之中最短**的。

### c.	Dm
Dm比較特別，上下左右一定可以走，但要走斜邊需考慮到相鄰的兩個直向是否可以走，因此比D4，D8多了一些判斷式。
```c
if (way[i][0] * way[i][1] != 0) { //斜走
    //另外兩邊不得符合指定值
    int R1_boolean = map.data[nx * HEIGHT + y] == range1 || map.data[x * HEIGHT + ny] == range1;
    int R2_boolean = map.data[nx * HEIGHT + y] == range2 || map.data[x * HEIGHT + ny] == range2;
    int R3_boolean = map.data[nx * HEIGHT + y] == range3 || map.data[x * HEIGHT + ny] == range3;
    if (R1_boolean || R2_boolean || R3_boolean) {
        continue;
    }
}
```
這條方程式式在判斷當要走斜邊(x*y!=0)時，判斷相鄰的兩個直向是否可以走，如果可以就不走斜邊。
## (1).	灰階{80}
* D4：最短路徑66

    ![](https://i.imgur.com/cAwcEsw.png)

 
* D8：最短路徑51

    ![](https://i.imgur.com/rzuY1Sk.png)

* Dm：最短路徑66

    ![](https://i.imgur.com/4V9RFqX.png)

## (2).	灰階{80,160}
* D4：最短路徑66

    ![](https://i.imgur.com/x2pwnG8.png)

* D8：最短路徑29

    ![](https://i.imgur.com/JhNMvIE.png)
 
* Dm：最短路徑35
 
     ![](https://i.imgur.com/Vdo3xpo.png)

## (3).	灰階{80,160,255}
* D4：最短路徑66

    ![](https://i.imgur.com/gBDJkMP.png)


* D8：最短路徑25
    
    ![](https://i.imgur.com/qz3nmxF.png)
 

* Dm：最短路徑30
 
    ![](https://i.imgur.com/gkaagNp.png)


###### tags: `github` `ImaageProcessing`
