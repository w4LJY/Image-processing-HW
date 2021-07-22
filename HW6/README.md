# HW6

# 1.	Homomorphic filter
## (a).	使用Homomorphic filter可增強house512.raw的細節，使用不同的參數改善house512.raw。比較並討論不同參數下的效果
Homomorphic的原理是先用自然對數將圖像拆為illumination乘上reflection，並在傅立葉時將相乘轉為相加，並藉由調低illumination以減少光源對圖像的影響，調高reflection提升圖像中細節部分。H公式為![image](https://user-images.githubusercontent.com/87016529/126635411-b4c2f088-04d0-4743-8798-5dcfb092a98d.png) ，和高斯的原理類似，而要調整圖片除了提升![image](https://user-images.githubusercontent.com/87016529/126635444-76b860fb-b34f-4906-b3a5-35e6ec778e8c.png)  來強調細節，降低![image](https://user-images.githubusercontent.com/87016529/126635497-462f880b-ebb2-486a-aa4d-60a065ace27c.png)減少亮度造成的影響外，也可以調整c來調整高斯的斜率，與調整截止頻濾![image](https://user-images.githubusercontent.com/87016529/126635576-8f80a39e-fc92-45cb-8a4f-5949a8f2a702.png)。最後在反傅立葉後，需要用exp來補償一開始所取的自然對數。
* A. ![image](https://user-images.githubusercontent.com/87016529/126635614-569e3fcc-2991-4e65-836d-3a6b1be9ec88.png)
    
    ![image](https://user-images.githubusercontent.com/87016529/126635648-8f3a91fc-dd09-4bd3-a5a6-156993d9ad22.png)

 
# 2.	Filter in Frequency domain
## (a).	分別使用參數為5,25,125的ideal LPF，對aerialcity512.raw的頻域做低通濾波，顯示濾波輸出結果與其頻域圖。討論比較每個輸出圖像的視覺差異。用原始影像來計算他們的MSE與PSNR
Ideal Low pass Filter是一種在頻域上的濾波的手段，相較於在空間域的濾波來說，傅立葉後的頻域在濾波上更加直觀，並且可以使用到一些較特別的方法。

因為調整過DC點在中心的頻域圖，其中心點為低頻，越往外越高頻，ideal就是用最暴力的方法，找出一個中心點為圓心的圓，根據選取的半徑大小決定通過的高頻數量。在園內的保留，圓外的都將值設為0就能有效抑制高頻。

由此可見，當半徑越小的時候，能通過的高頻就越少，模糊效果就越強，如半徑![image](https://user-images.githubusercontent.com/87016529/126635793-9a618adc-c9af-4872-bb08-613ac4289d19.png) 時整個影像都模糊不清，隨著![image](https://user-images.githubusercontent.com/87016529/126635837-d8fcf3bf-e242-41d9-905a-21dde6871501.png) 增加，圖像的模糊越弱。而且可以發現，雖然ideal的效果很強，但是因為方波在傅立葉後會產生弦波，所有會有水波紋，因此在方法選用上就需要再效果與避免水波紋兩者間取平衡，也因此會用到Butterworth filter與Gaussian filter等方法來處理。
* A.	magnitude spectrum  ![image](https://user-images.githubusercontent.com/87016529/126635873-23c8f7d2-fdee-4b74-a6a7-f1c53c12060b.png) 

    ![image](https://user-images.githubusercontent.com/87016529/126636400-71c72643-1845-447b-8739-251725f32add.png)
 
* B.	magnitude spectrum   ![image](https://user-images.githubusercontent.com/87016529/126635897-2d911ebd-a24b-4b75-860d-c0b5937a60b9.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636520-dc9958a8-bd01-4a9c-9b11-fb3aefd507b9.png)
 
* C.	magnitude spectrum   ![image](https://user-images.githubusercontent.com/87016529/126635941-d7468a83-691e-4ef0-a737-990ef8ce7476.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636544-933a8506-6ea5-4b0e-be24-c43afe634e8c.png)

* A.	 output image  ![image](https://user-images.githubusercontent.com/87016529/126635873-23c8f7d2-fdee-4b74-a6a7-f1c53c12060b.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636566-9204f781-14ec-414b-b02b-ee4420b22686.png)

* B.	output image  ![image](https://user-images.githubusercontent.com/87016529/126635897-2d911ebd-a24b-4b75-860d-c0b5937a60b9.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636579-a7d90522-a2ba-406d-83ab-0f5455bee9ef.png)

* C.	output image   ![image](https://user-images.githubusercontent.com/87016529/126635941-d7468a83-691e-4ef0-a737-990ef8ce7476.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636586-540e46e2-8035-473a-bf3e-9b9a9792a4da.png)

* A.	 ![image](https://user-images.githubusercontent.com/87016529/126635873-23c8f7d2-fdee-4b74-a6a7-f1c53c12060b.png)的MSE與PSNR
    
    > MSE:1196.099
    > PSNR:17.353

* B.	  ![image](https://user-images.githubusercontent.com/87016529/126635897-2d911ebd-a24b-4b75-860d-c0b5937a60b9.png)的MSE與PSNR

    > MSE:1196.099
    > PSNR:17.353

* C.	 ![image](https://user-images.githubusercontent.com/87016529/126635941-d7468a83-691e-4ef0-a737-990ef8ce7476.png)的MSE與PSNR

    > MSE:173.642
    > PSNR:25.734

## (b).	分別使用參數為5,25,125的Gaussian HPF，對aerialcity512.raw的頻域做高通濾波，顯示濾波輸出結果與其頻域圖。討論比較每個輸出圖像的視覺差異。用原始影像來計算他們的MSE與PSNR
Gaussian filter與ideal類似，與ideal相比，他的效果沒有那麼好，但能夠有效避免水波紋。他所調整的不是圓的半徑，是Gaussian的variance，variance越大，模糊效果越強。
但這裡使用的是高通濾波器，也就是只有高頻可以通過，因此要將1減去高斯的結果![image](https://user-images.githubusercontent.com/87016529/126636108-70081955-6cdf-4a87-9e9a-ec85edc77500.png) ，所以變成當variance ![image](https://user-images.githubusercontent.com/87016529/126636146-85fae31b-e50b-4926-99ba-4eefb061d611.png)  越大，能夠通過的低頻越少，只剩下變化性強的高頻部分。
* A.	magnitude spectrum    ![image](https://user-images.githubusercontent.com/87016529/126635873-23c8f7d2-fdee-4b74-a6a7-f1c53c12060b.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636843-ece6d24e-68bd-4f91-be5f-f15cd31a0308.png)
 
* B.	magnitude spectrum   ![image](https://user-images.githubusercontent.com/87016529/126635897-2d911ebd-a24b-4b75-860d-c0b5937a60b9.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636852-0ef90117-df1c-4a3c-bffe-3720ae3422bd.png)

* C.	magnitude spectrum   ![image](https://user-images.githubusercontent.com/87016529/126635941-d7468a83-691e-4ef0-a737-990ef8ce7476.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636859-0dbe12ce-11bd-4e48-a861-0a9e1a7b1279.png)

* A.	 output image  ![image](https://user-images.githubusercontent.com/87016529/126635873-23c8f7d2-fdee-4b74-a6a7-f1c53c12060b.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636870-c88167ed-515f-41b3-95ef-e0201ce1b952.png)

* B.	output image  ![image](https://user-images.githubusercontent.com/87016529/126635897-2d911ebd-a24b-4b75-860d-c0b5937a60b9.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636883-db3eef85-8367-405e-ae5d-c8ae6a33b403.png)

* C.	output image   ![image](https://user-images.githubusercontent.com/87016529/126635941-d7468a83-691e-4ef0-a737-990ef8ce7476.png)

    ![image](https://user-images.githubusercontent.com/87016529/126636895-c6c0f56a-7e4f-4179-ad6f-c6a4fad86acd.png)

* A.	![image](https://user-images.githubusercontent.com/87016529/126635873-23c8f7d2-fdee-4b74-a6a7-f1c53c12060b.png) 的MSE與PSNR

    > MSE:8708.29
    > PSNR:8.7315

* B.	![image](https://user-images.githubusercontent.com/87016529/126635897-2d911ebd-a24b-4b75-860d-c0b5937a60b9.png)的MSE與PSNR
    
    > MSE:9986.59
    > PSNR:8.1367

* C.	 ![image](https://user-images.githubusercontent.com/87016529/126635941-d7468a83-691e-4ef0-a737-990ef8ce7476.png) 的MSE與PSNR

    > MSE:10987.25
    > PSNR:7.722
 
