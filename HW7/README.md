# HW7

# 1.	Periodic Noise
## (a).	使用Notch filtering去除週期性雜訊，顯示去除雜訊後的結果。
Notch filter的功用是去除週期性雜訊，週期性雜訊若經過傅立葉轉換後會根據其在影像空間的雜訊方向在頻域圖上產生對稱的兩個點。只要能將這兩個點抑制就能成功去除雜訊。

Notch filter的方法是精確地找出著兩個對稱點的位置，並將這個範圍的傅立葉值設置為0，也就是去除這個雜訊所帶來的高頻變化。

實作上很簡單，一共需要知道3個參數，分別是這個雜訊距離DC點的x,y距離，和這個雜訊點的範圍(用半徑或是高斯變異數等來設計)。

因此可以使用ideal、butterworth、Gussian等方法來實現，差別只是在於對於抑制雜訊的邊緣處理的陡峭程度，將從濾波強度與是否會產生水波紋間做取捨。

我這裡使用butterworth的方式，他的除雜訊強度介於ideal，與butterworth之間，只要找得點足夠精準，範圍也剛好，就能很好的去除週期性雜訊。

* A.	Notch filte x=16,y=81,w=50
    * I.	頻域圖

        ![image](https://user-images.githubusercontent.com/87016529/126637413-2e0f8a2d-f394-40ea-8fa4-c170deb8f48d.png)

 
    * II.	結果圖

        ![image](https://user-images.githubusercontent.com/87016529/126637494-be80f810-cddd-4b10-bb7b-9679baedd92e.png)

 
## (b).	使用band reject filtering 去除週期性雜訊，顯示去除雜訊後的結果。
Band reject 與Notch差別在於，Notch是精確地找的兩個對應點，而Band reject是直接消除雜訊點出現的一整圈範圍，因此要知道的參數也比較少，只要知道DC點與雜訊的直線距離，與雜訊的寬度就能很好的去除。

與ideal的做法很像，像是找兩個圓，並將兩個圓不重疊的區域設為0。

這題我用ideal的方法來進行實作，果然還是會有水波紋的產生，牽扯到傅立葉轉換時，對於這種會產生方波的動作還是要小心處理。

* A.	band reject filtering 
    * I.	頻域圖
    * 
        ![image](https://user-images.githubusercontent.com/87016529/126637594-4ff19667-b8c7-4165-bc7d-037e3fbe1236.png)

    * II.	結果圖
    * 
        ![image](https://user-images.githubusercontent.com/87016529/126637611-cbb7cba6-716e-48fa-bd01-87a62368913c.png)

 

## (c).	討論與比較不同方法的效果
若是以這題為例，Notch filter的處理效果會比band reject filter好很多，因為這題就只有一對雜訊點，band reject去除一整圈是沒有必要的，還會因此損失掉部分的細節。

但如果說有很多對雜訊點在與DC點半徑相近的區域的話。用Notch filter就會非常麻煩，每對雜訊都要三個參數才有辦法決定。因此這個時候選擇總共只要兩個參數設定的band reject filter就是一個很好的方式。

# 2.	Debluring
對motion_flower.raw分別以inverse filter與Wiener filter來復原。給出一個g(x,y)函數，它是由於運動造成圖像模糊。顯示輸出圖象與原圖間的視覺差異。

---

雜訊分為摺積式雜訊與加成性雜訊，在這題中，對motion_flower.raw造成雜訊的是摺積式雜訊。通常會用觀察法、實驗法、數學法等方式來去除。而這題老師有給我們早成其雜訊的數學模型g(x,y)，因此我們只需要將g(x,y)推導到傅立葉，就能知道G(u,v)，該除上的H(x,y)是多少，因為完整的g(x,y)內所有數值都已經有了，所以只要對應推導公式就能得到H的實部與虛部： 

![image](https://user-images.githubusercontent.com/87016529/126637733-7bb485c0-311c-4237-b38e-f1ad1aeed732.png)

但並不是有了H之後就能只接復原影像，直接除上H會放大加成性雜訊  

 ![image](https://user-images.githubusercontent.com/87016529/126637751-22bae614-0111-4c8a-b6e1-6cbbcf22729e.png)


因此這裡使用兩種方法來避免加成性雜訊被過度放大
(a).	Inverse filter: Inverse filter的方式就是直接將G(u,v)除上H(u,v)，而加成性雜訊放大抑制的方式是用類似ideal的方式限制在低頻的區域才做這類運算，高頻雜訊過多不做，當然一樣是可以使用butterworth或是Gaussian來限制範圍。

直接將G(u,v)除上H(u,v)可以得到:

![image](https://user-images.githubusercontent.com/87016529/126637803-ad099f48-f465-4ec5-a4e8-30f27b2a55ef.png)

  
在還原F(u,v)時，為了避免在H(u,v)很小時，產生過大的數值，因此在輸出F(u,v)時我會限制他的範圍，若超過指定範圍值，就保持原本的值不改動。
做完後的效果差強人意，雖然我從頻譜圖看得出來差異，也都有按照老師給的方程式g(x,y)下去除雜訊，但是結果圖很難看出有什麼改善。我試過非常多的方法，如將g(x,y)換方向去濾除或是重新推導H(u,v)的式子，結果都沒辦法更好。
* A.	Inverse filter設置radius為100，限制F(u,v)值<4000
    * I.	頻率圖

        ![image](https://user-images.githubusercontent.com/87016529/126637835-5d02b762-4769-46bc-8ad0-04bce5ab8bd0.png)

    * II.	結果圖
    
        ![image](https://user-images.githubusercontent.com/87016529/126637857-e088d688-fb7d-4c37-b2bd-5aca0c622645.png)

 
## (b).	Wiener filter: 考慮上訊雜比的因素k，隨著加成性函數的多寡，調整k的大小，同樣能避免加成性函數被過分放大。
加上K後G(u,v)與F(u,v)的轉換公式變成:

![image](https://user-images.githubusercontent.com/87016529/126637892-275f72f6-e254-45bf-85f3-0f534ab19ed4.png)

因為![image](https://user-images.githubusercontent.com/87016529/126637908-605859cf-c7fc-4ab5-81a1-776667c2e671.png)  是個常數項所以:

![image](https://user-images.githubusercontent.com/87016529/126637935-75518751-1b30-4d37-a79c-bbb2668532e5.png)

 
因為這張圖並沒有太多的加成性雜訊，因此K並不需要太大，但同樣的結果圖並沒有很好的復原，畢竟Wiener filter與Inverse filter的差別是對加成性雜訊的處理，並不會使圖像恢復得更完美。 

* B.	Wiener filter 設置K為0.005，限制F(u,v)值<4000
    * I.	頻率圖
    
        ![image](https://user-images.githubusercontent.com/87016529/126638021-e083fb64-0ba9-4a7d-9f09-1e20b74a6d56.png)

 
    * II.	結果圖
    
        ![image](https://user-images.githubusercontent.com/87016529/126637988-80828220-8763-48a2-bb1f-fc3d5e1bfd09.png)

 

