# HW5
 
# 1.	2D-DFT
## (1).	寫出自己的DFT (DC點在正中央) 對rect256.raw, circle256.raw, square256.raw, square256_rotate.raw.並顯示其頻譜，討論結果 
傅立葉轉換的公式為![image](https://user-images.githubusercontent.com/87016529/126632802-a6f09607-f902-49bc-bef1-ca0806eddf16.png) 在考慮到DC點需位移乘上![image](https://user-images.githubusercontent.com/87016529/126632957-54335a4a-8709-4d24-b835-9d2ec187cd6f.png) ，因此整個公式變為 

![image](https://user-images.githubusercontent.com/87016529/126633039-b260f88e-a2e9-489a-983e-89cea77bc2b1.png)

但若直接使用這個公式for迴圈的次數會是![image](https://user-images.githubusercontent.com/87016529/126633067-d8f07630-de75-4348-9aff-d4ddd183b5e2.png) ，我的電腦需要跑18分鐘。因此我將方程式拆分為兩個一維來分別進行運算。

![image](https://user-images.githubusercontent.com/87016529/126633129-c54a480f-8a7c-42e2-a771-3aafb7b312b4.png)

![image](https://user-images.githubusercontent.com/87016529/126633138-0c342ac2-85f0-486d-b216-afbd4148ae33.png)
  
改用一維來做for迴圈次數降為![image](https://user-images.githubusercontent.com/87016529/126633169-3592bbd8-7959-46bb-8bf1-596e7a8adf7d.png) ，使原本將近20分鐘才能跑完的程式，現在幾秒鐘就能夠出來了。
要實作公式，要處理方程式中的實部與虛部，我使用尤拉公式，將 ![image](https://user-images.githubusercontent.com/87016529/126633200-4b295f78-81e3-4b6b-9d93-5c07da2bed22.png)拆分為![image](https://user-images.githubusercontent.com/87016529/126633220-25befc09-40f0-4970-a630-5b2bf0850075.png) ，得到傅立葉結果的實部與虛部後，平方相加開根號得到頻譜圖的像素強度值。

![image](https://user-images.githubusercontent.com/87016529/126633243-d14b202f-6018-4619-8463-20128c4ab42b.png)

最後出來的結果整張圖都是黑的，因此我用normalize處理圖像，方便對頻譜圖的觀察。

![image](https://user-images.githubusercontent.com/87016529/126633259-8207c423-e79d-482e-91fe-8bbbe211561d.png)
  
傅立葉後的頻譜圖，與原圖存在著對應關係。首先，傅立葉結果代表的是原圖像的高低頻變化，如果原圖是整張同一顏色的無變化，那傅立葉結果就只會有頻譜圖中心的直流dc分量有值，而隨著原圖的顏色變化越劇烈，高頻就會越多，頻譜圖結果也會往外擴散。

Rect256.raw這張圖來看的話，x方向的顏色變化沒有y方向的變化強，因此結果會像是躺著的直條圖。

Circle256.raw這張圖，每個方向的顏色變化都是一樣的，因此結果圖也是一個圓。

由square256.raw與square256_rotat.raw可以發現，隨著圖像的旋轉，對於顏色變化響應結果也會因為方向改變而改變，可以知道頻譜圖會隨著圖像旋轉而旋轉。但如果只是一般的位移，頻譜圖是不會發生改變的。

* A.	Rect256.raw原圖與傅立葉後的頻譜圖

    ![image](https://user-images.githubusercontent.com/87016529/126633342-e67b7c3c-84a9-49e6-b9c2-cf62f0e910f0.png) ![image](https://user-images.githubusercontent.com/87016529/126633352-4eefc12b-0b45-4360-930c-0e0aba98b3fd.png)


* B.	circle256.raw原圖與傅立葉後的頻譜圖 

    ![image](https://user-images.githubusercontent.com/87016529/126633443-3565a48a-dba5-4f71-a948-0a3220e48183.png) ![image](https://user-images.githubusercontent.com/87016529/126633456-cef7814e-cc83-4df5-ba2a-ba0ab0f1261a.png)

* C.	square256.raw原圖與傅立葉後的頻譜圖 

    ![image](https://user-images.githubusercontent.com/87016529/126633477-84b6bb90-725e-48c5-92fa-1f866529993e.png) ![image](https://user-images.githubusercontent.com/87016529/126633486-529b24c9-1fd7-4ad8-a3e5-d36d5b4b8971.png)

* D.	square256_rotate.raw原圖與傅立葉後的頻譜圖 

    ![image](https://user-images.githubusercontent.com/87016529/126633503-31df3c63-ea7e-4f0d-9313-dd4b76b5a247.png) ![image](https://user-images.githubusercontent.com/87016529/126633512-7bcb763d-fff4-4523-bad0-3544139f6a07.png)


## (2).	比較自己的方法與openCV的方法，並討論結果的不同
openCV的方法我參考自openCV官網上的FFT過程，一開始，為了提升傅立葉轉換的速度，擴充圖像的尺寸
```c
//---獲得最佳DFT尺寸，為2的次方---//
Mat padded;
int M = getOptimalDFTSize(I.rows); //該函數返回給定向量尺寸的傅立葉最優尺寸大小。為了提高離散傅立葉變換的運行速度，需要擴充圖像。                      
int N = getOptimalDFTSize(I.cols);
```
接著建立實部與虛部merge後的圖像
```c
//---將影像處理為實部與虛部
Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) }; //分為實部與虛部
Mat complexImg;
merge(planes, 2, complexImg); //合併實虛部
```
進行快速傅立葉轉換
```c
dft(complexImg, complexImg); // FFT變換
```
方便觀察可能會加上對比度增強
```c
//---方便觀看的對數尺度變化---//
//mag += Scalar::all(1);
//log(mag, mag);
```
 
位移使DC點在圖像中心
```c
//切為4個區塊重新排列，從左到右，從上到下 分別為q0, q1, q2, q3                                                                                                           
Mat tmp;
Mat q0(mag, Rect(0, 0, cx, cy));
Mat q1(mag, Rect(cx, 0, cx, cy));
Mat q2(mag, Rect(0, cy, cx, cy));
Mat q3(mag, Rect(cx, cy, cx, cy));
```
```c
//對調q0和q3
q0.copyTo(tmp);
q3.copyTo(q0);
tmp.copyTo(q3);
//對調q1和q2
q1.copyTo(tmp);
q2.copyTo(q1);
tmp.copyTo(q2);
```
最後進行歸一化
```c
normalize(mag, mag, 0, 1, NORM_MINMAX);
```

* A.	Rect256.raw傅立葉結果

    ![image](https://user-images.githubusercontent.com/87016529/126634110-e0a08a77-0f73-4cb3-8f86-ee29e6cc6518.png)

* B.	circle256.raw傅立葉結果

    ![image](https://user-images.githubusercontent.com/87016529/126634128-0e132bfe-5d6e-4b04-ac8a-06eae5f0af9a.png)

* C.	square256.raw傅立葉結果

    ![image](https://user-images.githubusercontent.com/87016529/126634146-15071c7d-8d14-439d-84fb-65d39a5cda9c.png)

 
* D.	square256_rotate.raw傅立葉結果

    ![image](https://user-images.githubusercontent.com/87016529/126634161-071efd72-aec3-46a7-b771-266e8501396e.png)
 

openCV與我自己的方法結果是差不多的，最大的差別在於運算時間。以rect256.raw這張圖的運算時間來比較。
* A.	我的方法對rect256.raw的運算時間

    > 12.57秒

* B.	openCV對rect256.raw的運算時間

    > 0.012秒

可以發現兩者的時間差約有1000倍，因此FFT對於改善傅立葉的速度還是非常有效的。

## (3).	寫出自己的IDFT並將反傅立葉的結果圖與原圖進行比較，算出MSE與PSNR的值。
反傅立葉的公式為 ![image](https://user-images.githubusercontent.com/87016529/126634264-e74d8f65-e88f-43eb-b47d-3f16ba412111.png)

同樣拆分為一維來加快速度，且要將DC位移值乘回去

![image](https://user-images.githubusercontent.com/87016529/126634297-3c500bf6-d192-4a75-b442-b6e90e6a3c52.png)

![image](https://user-images.githubusercontent.com/87016529/126634326-765d7169-d77f-41a8-af26-e331e6bd5cd6.png)
 
最後在將實部與虛部根據尤拉公式還原成原圖。 

![image](https://user-images.githubusercontent.com/87016529/126634338-87dd0ee9-225c-4656-b10e-253aeb12e232.png)

* A.	Rect256.raw IDFT結果與和原圖的

    ![image](https://user-images.githubusercontent.com/87016529/126634373-60ad57d7-d663-4377-99e7-3d0792ad302f.png)

    > MSE: 29.46
    > PSNR:33.44
* B.	circle256.raw IDFT結果與和原圖的
 
    ![image](https://user-images.githubusercontent.com/87016529/126634454-7a264c2f-cc91-4079-8573-f0ab74d8dad6.png)
    
    > MSE:0.027
    > PSNR:63.86
* C.	square256.raw IDFT結果與和原圖的

    ![image](https://user-images.githubusercontent.com/87016529/126634478-6550ad4e-f5a6-48ef-b1d8-7818bbca464b.png)

    > MSE:35.48
    > PSNR:32.63
* D.	square256_rotat.raw IDFT結果與和原圖的

    ![image](https://user-images.githubusercontent.com/87016529/126634504-eff72e59-ef05-4563-99bd-74294751e4e4.png)

    > MSE:21.35
    > PSNR:34.87
