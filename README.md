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

## Algorithm

### 1.歐拉化
	先將一張圖轉為所有點的degree都是偶數的圖
	我使用 `find_Path()` 去找出兩奇數點間的路線，並選出最短的組合，
	最後將增加的邊加到原圖上。

### 2.找歐拉迴路 -Hierholzer's Algorithm
	Hierholzer's Algorithm可以找出一張所有點都是偶數degree的圖中的歐拉迴圈
	我使用stack資料結構實現此演算法


## File
* main.cc: 主程式
* example1.txt: 助教提供的測資
* inputgraph.dot: 輸入的圖轉換成dot格式(程式執行時產生)
* Resultpath.dot: 輸出的圖轉換成dot格式(程式執行時產生)
* Result_path.txt: 運算結果輸出路徑	(程式執行時產生)

## Result

### Example1
Input graph
![Alt text](/Example1.png)

Output graph
![Alt text](/Example1_Result.png)

The result path is:a->b->c->d->e->b->e->c->a

