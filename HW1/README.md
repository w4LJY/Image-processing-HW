
HW1
===

# 1. 	Raw檔I/O
## 1.1.	Raw檔處理
---

### a.	下載raw檔
### b.	用raw檔檢視軟體顯示lena256，並截圖，且說明過程
\
我選擇XnView作為raw檔的的檢視器，若要正確的讀取raw檔需將載入格式的大小與通道設置正確。

而lena256這張圖片的高與寬都為256，並且是灰階影像，因此要將格式設定如下圖：

![](https://i.imgur.com/DGNSq2q.png)
&nbsp;




## 1.2.	Raw image file I/O

### a.	使用C/C++編程，並安裝OpenCV
### b.	下載範例檔來開啟lena256.raw


#### (1). lena.raw圖中，第123row,234col的強度值
`Sol. 157`

用at函式可以讀取Mat矩陣指定行列值的強度值，如`Image.at<uchar>(r,c)`，需要注意的是，CV_8UC1為灰階圖對應`<uchar>`，CV_8UC3為彩色圖對應`<Vec3b>`


#### (2). lena.raw圖中，第5487的強度值，與其對應的行列
`Sol. 119，(21,111)`

配合兩個for迴圈先列後行的方式進行讀取，並設置變數計數，數到第5487個後，即可以at函數讀出當前強度值，且對應的兩個for迴圈指標就是轉換後的行列值(i,j)


### c.	顯示原圖

![](https://i.imgur.com/j3M3Dch.png)

### d.	翻轉lena256
翻轉就是將每個像素點的行與列相互調換。

![](https://i.imgur.com/xuMVcfC.png)

### e.	將lena256分為16等分，並隨機擺放。需顯示兩種不同的隨機狀況，並說明演算法。
 
#### (1)	切割
將原來256x256的大圖切割為16張64x64的小圖，方程式為：
```c
lena_random[k].at<uchar>(i,j)=lena.at<uchar>(i+(k/4)*r16,j+(k%4)*c16);
``` 
其中i,j是任意點的行與列，k是小圖的編號(0~15)共16個 ，r16與c16為小圖的長與寬(也就是64)，lena是大圖，lena_random[k]是切割過後的第k張小圖。 
將每個k丟入兩個for迴圈經由方程式遍歷過整張大圖(256x256)，便可得到切割後的16張圖。

#### (2)	隨機
以時間作為亂數種子，將16個數字重新排列。

#### (3)	還原
將16張64x64的小圖，依據隨機出來的排列順序來還原成256x256的大圖，方程式為：
```c
random_final[image_count-1].at<uchar>(i+(k/4*r16,j+(k%4)*c16)=lena_random[poke[k]-1].at<uchar>(i,j);
```

其中random_final是還原後的大圖，poker[k]是第k個重新排列的值。

同樣是將每個k丟入兩個for迴圈經由方程式遍歷過整張小圖(64x64)，方程式和切割的方程式大體上十分相像，就是方程式左右對調而已。
![](https://i.imgur.com/EJ1ot6w.png)

 
### f.	將p01,p02,p03,p04等四張圖合為一張大圖
#### (1)	如何得出合成圖的大小
四張原圖大小都為256x256，而合成的大圖，長與寬是相等的，由此可知，大圖最後的大小為512x512

![](https://i.imgur.com/YKU8mvD.png)

#### (2)	將四張小圖合為一張大圖
四張256x256小圖將合成一張512x512大圖，方程式為：
```c
Mat_mix.at<uchar>(i+(k/2)*height,j+(k%2)*width)=PMat[k].at<uchar(i,j)
```
其中Mat_mix是合成後的大圖，PMat[k]是第k塊小圖。可以發現其實這就是1.2-e中的還原方程式。

#### (3)	儲存為raw檔，並在Xnview中顯示結果。

![](https://i.imgur.com/m5tzLJK.png)


 
## 1.3.	亮度
### a.	將lena256的每個像素強度值加上50
當像素強度值大於(255-50)的將其都設定為205，最後在將圖上像素值都加上50，這樣就不會發生像素值超過255的狀況。

![](https://i.imgur.com/nZww3AK.png)

 
### b.	將lena256的每個像素值隨機加上-50~50
以時間作為隨機種子，取得-50~50間的隨機數，接著將原圖加上其值。
需要注意的是除了上限，也有可能突破下限，因此多設置了，若隨機值為負數，且像素強度值小於(0+負數隨機值)，則設定值為負數隨機值取絕對值。

 ![](https://i.imgur.com/f25Y831.png)

### c.	a與b小題需要注意甚麼
亮度值的範圍為0~255，因此在增加或減少時，都要注意會不會超過這個範圍，在程式中，我以條件式限制，將所有會超過255的強度值固定在255，會低於0的強度值固定在0。
# 2. 	將haha.raw這張圖左上角寫上自己的學號，並在右下角寫上任何想說的話。
使用opencv中的函式putText就能夠很輕易的寫上資訊，需注意的是圖檔大小為512x512，讀錯大小的話，熊貓會只剩下兩隻耳朵。

![](https://i.imgur.com/1c7e9pj.png)




 

###### tags: `github` `ImaageProcessing`



