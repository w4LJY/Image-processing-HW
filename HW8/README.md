# HW8

# 1.	Binary morphological processing
## (a).	用形態學取得adip.raw上字元的邊緣
形態學的邊緣分別有三種取得方式，分別是(膨脹-原圖)、(原圖-侵蝕)、(膨脹-侵蝕)，這幾種方法能依據形態學取得邊緣的原理是因為二值化影像形態學中的dilation與erosion都會使圖像白色區域大小產生改變，相減就能夠得到邊緣。

這三種方法得到的邊緣有所差異，(膨脹-原圖)的邊緣會在字元的外圍，因為膨脹是使目標變胖，(原圖-侵蝕)的邊緣會在字元的內圈，因為侵蝕是使目標變瘦，(膨脹-侵蝕)的邊緣是最寬的，相當於(膨脹-原圖)與(原圖-膨脹)結果的聯集。

Binary image的dilation與erosion我都是使用正方形的mask，並且為了加快運算速度，將mask由nxn拆分成兩個nx1去對影像作形態學。

* A.	邊緣(膨脹-原圖) mask大小5x5

    ![image](https://user-images.githubusercontent.com/87016529/126638374-d5433205-6160-4d72-ac88-306e2a551555.png)
 
* B.	邊緣(原圖-侵蝕) mask大小5x5

    ![image](https://user-images.githubusercontent.com/87016529/126638401-a0bf8365-9a94-4fd5-8e47-28b920599542.png)
 
* C.	邊緣(膨脹-侵蝕) mask大小5x5

    ![image](https://user-images.githubusercontent.com/87016529/126638423-87dbc36a-2f31-49c1-9a45-d00a35bb70bc.png)

 
## (b).	用region filling 將字元”HW7”填滿去除，可以手動設定seeds
要進行region filling前須要先找出seeds，因為題目要求填滿的字元一共有三個，所以要分別找出三個seeds，這裡我是以小畫家來查找座標。

![image](https://user-images.githubusercontent.com/87016529/126638448-d2a6edf2-9335-48ab-b4f2-92d6fffdd95e.png)

 

找到座標以後根據演算法，將seeds持續dilation並與原影像補集取交集，直觀來看就是將起始點往外擴張，並限制擴張範圍。

最後需要設置停止條件，也就是圖像不再改變(無法繼續擴張範圍)，就結束運算，可以得到:
 
![image](https://user-images.githubusercontent.com/87016529/126638501-2cf7872c-eccb-43b3-819b-55fa1cbde11d.png)

有了填補後的字元後，就能輕鬆從adip.raw中去除”HW7”:
 
![image](https://user-images.githubusercontent.com/87016529/126638514-eb459886-9a49-4a98-8bf6-9c530ff85ea9.png)

## (c).	找到每個字元的convex hull
Convex hull 凸包，就是讓目標包成完全是凸的，也就是兩點間必有直線能連接。

Convex hull的演算法需要分別對4個方向的結構元素進行運算，運算的方式是判斷所掃到的區域是否符合結構定義，符合的話，將掃到位置設為255，否則為0。也就是根據結構形狀，向某個方向進行凸包擴張，重複執行到圖像不再發生改變。最後將4個結構的最終結果聯集，就能得到凸包。

* A.	四個結構分別的結果(未限制範圍)

    ![image](https://user-images.githubusercontent.com/87016529/126638586-ff593d53-b78e-4c29-b2fd-41541cd2652c.png)
    
    ![image](https://user-images.githubusercontent.com/87016529/126638610-927a66ec-0af9-4787-8d69-5f67bdbbbe4d.png)
    
    ![image](https://user-images.githubusercontent.com/87016529/126638692-829c2669-5c18-40c7-91be-3b783c9bfcdf.png)
    
    ![image](https://user-images.githubusercontent.com/87016529/126638704-c239c42d-2a82-45eb-afee-ab35e6a9caaa.png)


  
* B.	字元凸包(未限制範圍)

    ![image](https://user-images.githubusercontent.com/87016529/126638722-176c5c56-38a9-4413-8a0b-ef7f61f0fa15.png)

 
    

通常凸包不會直接使用以上演算法的結果，以上結果的凸包其實超過凸包所要求的最小尺寸，因此會用原目標大小的水平與垂直尺寸限制增長範圍。
不過adip.raw這張圖總共有11個目標字元，因此需要分別限制11個目標的範圍，為了得知這11個目標的範圍我加上了Connected Components去找出11個字元的bounding box。

![image](https://user-images.githubusercontent.com/87016529/126638763-8b959b3b-0681-496a-a33f-fb293246a3f9.png)

找出bounding box後就能夠根據bounding box的範圍進行增長限制，得到正確的凸包結果。

* A.	四個結構的分別的結果

    ![image](https://user-images.githubusercontent.com/87016529/126638828-746b888a-10db-47b2-973e-380c5d0b9896.png)
    
    ![image](https://user-images.githubusercontent.com/87016529/126638847-5332bf23-f136-4df0-bfcf-8731b69e8d8b.png)
    
    ![image](https://user-images.githubusercontent.com/87016529/126638860-fe3212ed-e6bc-451d-982e-16a692a149c1.png)
    
    ![image](https://user-images.githubusercontent.com/87016529/126638870-e716cf5e-50f4-4e35-9e09-adf56d8a9b74.png)

* B.	字元凸包

    ![image](https://user-images.githubusercontent.com/87016529/126638893-9a71e7c2-7038-4f6c-a54d-dcaa0cc7731f.png)

 

## (d).	使用形態學去除adpi_noise.raw上的雜訊
圖片上的細線，小點這類雜訊非常適合用opening來去除雜訊，因為opening會先以erosion清掉雜訊，dilation在將被erosion而變小的字元放大成原本的大小，去除雜訊結果為:

![image](https://user-images.githubusercontent.com/87016529/126638935-6f3bd533-69ca-4737-b18f-ce0640f602ed.png)

 

# 2.	Gray-level morphology
## (a).	用灰階影像的dilation與erosion對walkbridge.raw做處理。
灰階影像dilation演算法為將結構元素反向後與原圖相加，而最大值就是輸出，erosion演算法為將結構元素與原圖相減，而最小值就是輸出。

灰階影像與二值化影像的形態學效果類似，由原來0與1的變化轉換到了亮暗的變化。因此dilation後會使較亮(白色)區域範圍擴大，且整體影像變亮，erosion會使較亮(白色)區域範圍縮小，且整體影像變暗。

觀察walkbridge.raw的dilation結果能發現，橋上的小孔擴大，且整體影像變亮。Erosion結果，橋上的小孔不見了，且影像整體變暗。
* A.	原圖橋上小孔示意圖

    ![image](https://user-images.githubusercontent.com/87016529/126638990-eaf801ba-4c5a-48fa-bae5-ea64eadd8204.png)
 
* B.	Dilation結果 mask大小5x5 數值皆為10的方形

    ![image](https://user-images.githubusercontent.com/87016529/126639019-efa9b014-efd4-47f3-bb43-35a3bca90b7f.png) 

* C.	Erosion結果 mask大小5x5 數值皆為10的方形

    ![image](https://user-images.githubusercontent.com/87016529/126639043-2865aa60-b794-4f2a-b7fb-e4ed75f3a864.png)
 
## (b).	使用形態學將gra__493x277.raw根據types/sizes分割成三個區域。
觀察圖案可以發現主要分為三種區域，分別是(大石子，白色)、(大石子，灰色)、(小石子、灰色)，因此我決定分為兩個任務，分別是依據顏色將灰與白分開，與依據石頭大小將大與小石頭分開。
 	
要將灰與白分開，我先使用gray-level closing讓白色石頭能連接在一起，接著使用閥值取出大部分的白色石頭區域，與其他的雜訊，在反覆使用binary 的opening與closing就能成功將白石頭部分的遮罩做出來。
 	
要將大小石頭分開，我直接先以閥值轉為二值化影像來進行binary的形態學，closing後取與原圖差值方便觀察，接著持續用binary的opening與closing，將較小的石頭侵蝕掉，並盡量保持大的石頭不被侵蝕掉，就能成功分開大小石頭。
 	
最後我將三個區域分別設置為不同的顏色，(大石子，白色)區域設置為灰色，(大石子，灰色)區域設置為白色，(小石子、灰色)區域設置為黑色。結果如下:
* A.	 原圖

    ![image](https://user-images.githubusercontent.com/87016529/126639129-f8282346-1614-402c-b7e7-42bb0c054888.png) 

* B.	分割成三個區域

    ![image](https://user-images.githubusercontent.com/87016529/126639146-96edf9e2-4e7f-4a68-b87f-257238349e33.png)

 
