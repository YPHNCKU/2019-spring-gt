# Graph-Theory-Project-Chinese Postman Problem
Project of course - Graph Theory (EE6622E) in National Cheng Kung University.

## Chinese Postman Problem
郵差要從已知的路線當中，設法找到一條最短路徑，可以走過此地區所有街道，且最終能回到一開始的出發點。
中國郵差問題就是要求在最短的路徑下，經過所有的邊與區域。
在一個無向圖中，找到一個最短的封閉路徑，且此路徑須通過所有的邊至少一次。

在最理想的狀況下，一張圖可以找出一條可以一次走完的封閉路徑，但是前提是這張圖是一張歐拉圖，
歐拉圖的性質就是可以找到一條通過圖上每條邊一次的封閉路徑。所以我會先檢查這張圖是不是歐拉圖，檢查的方法就是
檢查圖上的每個點的degree是否都是偶數。如果輸入的圖片是偶數的話，我就直接找那條封閉路徑。
但是如果存在一對以上的奇數點，那就表示不存在一條邊只走過一次的封閉路徑，我們必須在圖上增加一些邊，
使其滿足歐拉圖的特性，再繼續找出封閉路徑。但是對於奇數點倆倆建立路徑卻可能會產生不同長度的結果，
因此必須找出配對組合中最短的路徑長度。


## Program
程式的輸入是fake-mininet提供的圖格式，而程式輸出的結果會包含輸入圖的dot格式文件、計算結果的dot格式文件、輸出路徑的純文字檔案
此程式有以下注意事項:
1.當一張圖的奇數degree點為10個以上，程式無法正確
2.程式無法處理一開始就包含多重邊的圖片
	

## Algorithm

### 1.歐拉化
將一張圖轉為所有點的degree都是偶數的圖
我使用 `find_paths()` 去找出兩奇數點間的路線，並選出最短的組合，這裡由於我沒有實作出兩兩個點分為一組點不重複的演算法
所以直接將奇數點所有排列可能情形都計算路徑並找出最小值，而10個奇數點就有3628800種排列方式，導致程式無法順利執行。
因此最多只能做8個奇數點的圖，最後將增加的邊加到原圖上。

### 2.找歐拉迴路 -Hierholzer's Algorithm
	Hierholzer's Algorithm可以找出一張所有點都是偶數degree的圖中的歐拉迴圈
	我使用兩個stack資料結構實現此演算法
	1.從圖上的任意點開始，將此點push進入stack1
	2.找出與起點相鄰的任意點，若找到則跳到該點並將該點push進stack1，並將前後點的連線切斷
	3.若找不到下一點，則檢查圖上是否還有邊，若沒有邊則表示所有邊都走過了，跳至第7步
	4.若圖上還有邊，則表示還未走完，將該點pop出並push到stack2，並跳到stack1的最上方那點
	5-1.檢查是否可以找到下一點，若沒有則pop出stack1，並push到stack2，並跳到stack1的最上方那點
	5-2.若找到下一點則跳到下一點，並將下一點push到stack1
	6.檢查圖上是否還有邊，若還有則重複第5步，若沒有則表示所有邊都已走過。
	7.將stack1的點pop出並push到stack2，直到stack1為空。
	8.這時將stack2 pop出的點記錄下來，則為圖上的Euler circuit


## File
* main.cc: 主程式
* example1.txt: 助教提供的測資
* Inputgraph.dot: 輸入的圖轉換成dot格式(程式執行時產生)
* Resultpath.dot: 輸出的圖轉換成dot格式(程式執行時產生)
* Result_path.txt: 運算結果輸出路徑	(程式執行時產生)
* example2.txt: 奇數degree點為8的測資

## Result

### Example1
Input graph
![](/Example1.png)

Output graph
![](/Example1_Result.png)

The result path is:a->b->c->d->e->b->e->c->a

### Example2
Input graph
![](/Example2.png)

Output graph
![](/Example2_Result.png)

The result path is: a -> b -> a -> c -> a -> d -> a -> e -> a -> f -> a -> g -> a -> h -> a -> i -> a

