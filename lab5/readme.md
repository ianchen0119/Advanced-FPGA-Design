# Lab5

## 原始碼分析

Cordic 演算法透過多次跌代修正目標 **theta** 對應的 Sin 與 Cos 值。
![](https://www.rfwireless-world.com/images/CORDIC-rotation-angles.jpg)

接著，考慮以下程式碼做逐步分析。
```c=
#include "cordic.h"
#define iteration_num 7
THETA_TYPE cordic_phase[iteration_num] = {45.0, 26.565, 14.036, 7.125, 3.576, 1.790, 0.895};
void cordic(THETA_TYPE theta, COS_SIN_TYPE &s, COS_SIN_TYPE &c){
	COS_SIN_TYPE current_cos = 0.60735;
	COS_SIN_TYPE current_sin = 0.0;
	COS_SIN_TYPE factor = 1.0;
    int j = 0;
    for(; j<iteration_num; j++){
        int sigma = (theta>0)?1:-1;
        COS_SIN_TYPE temp_cos = current_cos;
        current_cos = current_cos - current_sin * sigma * factor;
        current_sin = temp_cos * sigma * factor + current_sin;
        theta = theta - sigma * cordic_phase[j];
        factor = factor >> 1;
    }
    s = current_sin;
    c = current_cos;
}
```
一開始，我們先定義一個 Array `cordic_phase` 存放每次跌代偏移的角度:
```c=
THETA_TYPE cordic_phase[iteration_num] = {45.0, 26.565, 14.036, 7.125, 3.576, 1.790, 0.895};
```
投影片有提到多次跌代後在輸出時需要乘上 `1.64676` 做修正。
對處理器來說，乘法所需的  CPU 週期遠大與加法，我們直接將 cos 的初始值由 1.0 改為 `0.60735` 就可以省掉一次加法的成本:
```c=
  COS_SIN_TYPE current_cos = 0.60735;
	COS_SIN_TYPE current_sin = 0.0;
	COS_SIN_TYPE factor = 1.0;
```
> factor 為 2^-i 。

在每次跌代，我們都需要比較 Target 與當前角度的大小決定要順時針還是逆時針做 Rotate :
```c=
int sigma = (theta>0)?1:-1;
```
接著，照著演算法做矩陣運算:
```c=
current_cos = current_cos - current_sin * sigma * factor;
current_sin = temp_cos * sigma * factor + current_sin;
theta = theta - sigma * cordic_phase[j];
```
做完以後，對 factor 做 right-shift 。 
Call by reference! 更新 Cos 與 Sin 的數值:
```c=
s = current_sin;
c = current_cos;
```
## Reference
- [CORDIC--HLS实现](https://blog.csdn.net/qq_23831743/article/details/81987777)
- [課程投影片](http://www.aiotlab.org/teaching/fpga/5_CORDIC.pdf)
