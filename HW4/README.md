# HW4

# 1.	Histogram processing
## (1).	用Histogram Matching 將house512 匹配成lena512的直方圖。

![](https://i.imgur.com/cB7IUqx.png)

 
Histogram Matching的方式，就是只讓原圖像符合match圖像直方圖的趨勢，相比於Histogram Equalization的直線趨勢，能有更多的變化。兩者的共通點在於都是找到灰階值與灰階值間的映射關係。

因此可以先將這個問題思考為找到原圖一個灰階值經過一系列計算後映射的新灰階值。

由原圖像素![](https://i.imgur.com/fJB7AvK.png)丟入直方圖均衡化公式能得到符合均衡趨勢的新強度![](https://i.imgur.com/bf2Kb63.png) ，同理match圖像像素![](https://i.imgur.com/0kOGTMO.png) 丟入直方圖均衡化公式也能得到新強度![](https://i.imgur.com/aolN6cT.png)。

而我們要找到的其實是![](https://i.imgur.com/fJB7AvK.png)與![](https://i.imgur.com/0kOGTMO.png)間的映射關係，也就是256個a (0~255)，分別對應的c值為多少(0~255)。

因此以直方圖均衡化做為橋樑，![](https://i.imgur.com/fJB7AvK.png)先經由直方圖等化得到![](https://i.imgur.com/bf2Kb63.png) ，再將![](https://i.imgur.com/bf2Kb63.png) 經過直方圖等化的**反函式**得到對應的![](https://i.imgur.com/0kOGTMO.png) ，公式可寫為 ![](https://i.imgur.com/MhNEqQP.png)
 。
問題就難在反函式計算困難，因此換個想法，對每個![](https://i.imgur.com/kxeP0ft.png)去找最相近的![](https://i.imgur.com/SkbeAB5.png)，這樣在意義上，能使![](https://i.imgur.com/z4zm8Bf.png) 與![](https://i.imgur.com/SkbeAB5.png) 最為相近的![](https://i.imgur.com/y9tUTuc.png) ，就是![](https://i.imgur.com/l6CeXXO.png) ，也就完成反矩陣的計算。

但實際上不須這麼麻煩分別做的兩次直方圖均衡化是能夠被省略的，因此只要找兩個cdf的差值![](https://i.imgur.com/9WdAueU.png) 能使結果最小的y，也就是![](https://i.imgur.com/WS48o9s.png) 即為原圖灰階值![](https://i.imgur.com/9P9QUE1.png) 所映射的新灰階值。

![](https://i.imgur.com/VpW5xpf.png)為house直方圖的pdf， ![](https://i.imgur.com/QPQsrgJ.png)為lena直方圖的pdf。

---

對house進行Histogram Equalization為 ![](https://i.imgur.com/J8S7YMV.png)

對lena  進行Histogram Equalization為![](https://i.imgur.com/rnMcQ8M.png)
 
因為Histogram Equalization後結果應為同一圖像，故![](https://i.imgur.com/T2UwEsv.png) ，因此只要找到兩者的映射關係，就能完成Histogram Matching。
也就是![](https://i.imgur.com/OFZYKjT.png) ，對於s的每個k(0~255)找到一個與其差值最小的z，這個z就是s當前的映射值。 
但程式實作上能更簡單，不需經過Equalization

![](https://i.imgur.com/D7GdQPZ.png)

  
可以知道，最後只要做出**兩者的cdf**就能夠找出**映射關係**。
* A.	house512.raw的直方圖

    ![](https://i.imgur.com/aX6PdJn.png)

 
* B.	lena512.raw的直方圖

    ![](https://i.imgur.com/TPdLUFD.png)

* C.	match結果的直方圖

    ![](https://i.imgur.com/EcgCNy2.png)

* D.	match的結果圖

    ![](https://i.imgur.com/3swqJn8.png)

 
## (2).	用Local Histogram Equalization來改善house512，使用三種不同的mask大小。
Local Histogram Equalization跟卷積的概念有點像，就是對一個個**小區塊做Histogram Equalization**。
我的方法就是對每一個pixel取出一個區塊(我設定為3x3,9x9,21x21)，接著計算區塊間的直方圖，算出直方圖的cdf後，將pixel點輸出為Histogram Equalization的結果。對整張圖的每一個pixel都經過此運算，就能得到Local Histogram Equlization的結果。
* A.	Mask大小3x3的Local Histogram Equlization 結果

    ![](https://i.imgur.com/DTicD8r.png)

 
* B.	Mask大小3x3的直方圖結果

    ![](https://i.imgur.com/Ip5zDfG.png)

 
* C.	Mask大小9x9的Local Histogram Equlization 結果

    ![](https://i.imgur.com/IDxPoN2.png)

 
* D.	Mask大小9x9的直方圖結果
 
    ![](https://i.imgur.com/9HxufX4.png)

* E.	Mask大小21x21的Local Histogram Equlization 結果
 
    ![](https://i.imgur.com/FUOUDF3.png)

* F.	Mask大小21x21的直方圖結果
    
    ![](https://i.imgur.com/77ZruNY.png)

 
# 2.	Image Averaging
## (1).	由street.avi的影片中，分別取10,200,all張圖片，加總後取平均，來消除圖片上的雜訊(車子)。
因為道路是背景的關係，影片中的每張圖片，道路部分都不會有所改變，但車子會隨著時間數量與位置都不盡相同，因此取平均有效於將車子從畫面中移出。並且取的圖片數量越多，平均起來就越能有效都去除所有車子，而不會留下殘影。
若取得圖片量有限的狀況，要盡量選取狀況**差異較大**的圖片，但因為影片中的車況流量一直都蠻穩定的，無論怎麼選效果都差不多。
* A.	選取前10張

    ![](https://i.imgur.com/BFJVnRb.png)

 
* B.	選取前200張

    ![](https://i.imgur.com/b3N8Mz6.png)

 
* C.	選取所有影像

    ![](https://i.imgur.com/kru7rVg.png)

 

從結果圖上能夠發現，只選前十張的結果，在車流量大的區域仍還有許多殘影，車流量小的區域車子就清的比較乾淨，可見**車流量越大(雜訊越多)則需要更多張影像去平均**才能完全消除。


# 3.	Edge Detection
## (1).	用兩種3x3 Laplacian filter來對walkbridge卷積，並以zero padding與replicated padding 來補值。

laplacian filter是對圖像進行二階微分，相比一階微分強度更強，輸出的邊界會更加銳化，且只會強調影像pixel中像素不連續的部分。這次使用到了兩種filter，第一種只考慮到**垂直與水平**兩個方向的梯度，第二種則是也考慮到**正負45度**梯度方向。
比較後可以發現，第二種因為考慮的方向比較多，**邊緣比第一種明顯，邊緣數量也較第一種多**。
* A.	Filter1 padding zero

    ![](https://i.imgur.com/YmXgDWN.png)

* B.	Filter1 padding replicated

    ![](https://i.imgur.com/j3D7Qdq.png)

* C.	Filter2 padding zero

    ![](https://i.imgur.com/1XStuae.png)

* D.	Filter2 padding replicated

    ![](https://i.imgur.com/gel82Zj.png)

## (2).	用A=1,2,4的3x3 Hight-boost基於(1)的兩個filter對walkbridge卷積，並以replicated padding補值。

Hight-boost的意義就是將laplacian的結果在加回到原圖，並將原圖的灰度值乘上A倍，有銳化邊緣的效果。

因為A為對原圖的倍率，A為1則代表laplacian結果加上原圖，A越大，圖像整體會越白，配合上laplacian的結果，能夠調整出更好看的影像。
* A.	A=1 Filter1

    ![](https://i.imgur.com/ED6PT1W.png)

 
* B.	A=1 Filter2

    ![](https://i.imgur.com/bN3LNzR.png)

* C.	A=2 Filter1

    ![](https://i.imgur.com/tUfiw29.png)

 
* D.	A=2 Filter2

    ![](https://i.imgur.com/dxY7cuF.png)

* E.	A=4 Filter1

    ![](https://i.imgur.com/E0OBIpw.png)

* F.	A=4 Filter2

    ![](https://i.imgur.com/ARb18aI.png)

# 4.	Gaussian filter 
## (1).	用5x5的Gaussian filters，且 ![](https://i.imgur.com/DGN1AGv.png)分別是0.8, 1.3, 2.0來對turtle卷積。

Gaussian filter與averaging filter一樣都是用來模糊影像的，但高斯函數對於目標點的權重比較強，較能保留住一些邊緣，而不像averaging filter會整張變得模糊不清。能夠調整 決定高斯函數的陡峭程度，當 越小，陡峭程度越高，也就是說， **![](https://i.imgur.com/QbnNdIK.png)越大，模糊的程度越高。**
* A.	Gaussian filter ![](https://i.imgur.com/GrBOXSK.png)=0.8

    ![](https://i.imgur.com/c8Uxy0G.png)

* B.	Gaussian filter  ![](https://i.imgur.com/tEgPLCF.png)=1.3

    ![](https://i.imgur.com/EzwFXtK.png)

* C.	Gaussian filter  ![](https://i.imgur.com/9DwhNIc.png)=2.0

    ![](https://i.imgur.com/l3cJBos.png)

## (2).	用DoG對於turtle，  ![](https://i.imgur.com/lhBpVun.png)分別為0.5與1.5。
DoG的做法就是將兩個不同 ![](https://i.imgur.com/nfKl29m.png)的Gaussian filter相減，就能得到邊緣與其他細節，大部分的邊緣檢測在微分的過程中也會增強雜訊，DoG則因為過程中的Gassian filter能去除高頻雜訊，所以非常適合於找出具有高頻雜訊圖像的邊緣。
* A.	DoG結果  ![](https://i.imgur.com/i2RMkFl.png)=0.5 -  ![](https://i.imgur.com/z1VfiHS.png)=1.5

    ![](https://i.imgur.com/bGOSGTS.png)

 
