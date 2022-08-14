// 公共头文件
#include "../Common/STC15F2K60S2.h"
#include "../Common/definition.h"
#include "../Common/delay.h"

// 硬件
#include "key.h"

// 软件

// 宏定义
#define PRESSED 0   //低电平按键有效
#define UNPRESSED 1

/*行*/
#define ROW_1 P2_0
#define ROW_2 P2_1
#define ROW_3 P2_2
#define ROW_4 P2_3

/*列*/
#define COL_1 P2_4
#define COL_2 P2_5 
#define COL_3 P2_6
#define COL_4 P2_7

/*
原键盘对应的返回值
********--10--7--4--1--*********
********--0 --8--5--2--*********
********--11--9--6--3--*********
********--15--14-13-12-*********
修改后键盘值
********--1--2--3--12--********
********--4--5--6--13--********
********--7--8--9--14--********
********--10-0--11-15--********
键盘实际显示
********--1--2--3--↑--*********
********--4--5--6--↓-*********
********--7--8--9--←--*********
********-存--0-取--ok--*********
*/

//行1
#define PRESS1_1    12   
#define PRESS1_2    13   
#define PRESS1_3    14    
#define PRESS1_4    15
//行2
#define PRESS2_1    3   
#define PRESS2_2    6   
#define PRESS2_3    9    
#define PRESS2_4    11  
//行3  
#define PRESS3_1    2   
#define PRESS3_2    5    
#define PRESS3_3    8  
#define PRESS3_4    0  
//行4
#define PRESS4_1    1   
#define PRESS4_2    4    
#define PRESS4_3    7  
#define PRESS4_4    10 

// 蜂鸣器 0响
#define BEEP_RING   0
#define BEEP_UNRING 1

// 全局变量

// 外部变量
extern bool g_breakKey; /* 按键中断标志*/

// 内部变量
sbit beep = P5^5;
//sbit beep = P4^0;  // 蜂鸣器 测试时屏蔽

// 静态变量

uint8_t getKey() {
    int8_t keyVal = -1;
    do {
        //使用定时器跳出该等待函数
        if (g_breakKey == true) {
            g_breakKey = false;
            break;
        }

        ROW_1 = PRESSED;
        ROW_2 = ROW_3 = ROW_4 = UNPRESSED;
        if (ROW_1 == PRESSED && COL_1 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_1 == PRESSED);
            
            return keyVal = PRESS1_1;
        }
        else if (ROW_1 == PRESSED && COL_2 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_2 == PRESSED);
            return keyVal = PRESS1_2;
        }
        else if (ROW_1 == PRESSED && COL_3 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_3 == PRESSED);
            return keyVal = PRESS1_3;
        }
        else if (ROW_1 == PRESSED && COL_4 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_4 == PRESSED);
            return keyVal = PRESS1_4;
        }
        ROW_2 = PRESSED;
        ROW_1 = ROW_3 = ROW_4 = UNPRESSED;
        if (ROW_2 == PRESSED && COL_1 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_1 == PRESSED);
            return keyVal = PRESS2_1;
        }
        else if (ROW_2 == PRESSED && COL_2 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_2 == PRESSED);
            return keyVal = PRESS2_2;
        }
        else if (ROW_2 == PRESSED && COL_3 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_3 == PRESSED);
            return keyVal = PRESS2_3;
        }
        else if (ROW_2 == PRESSED && COL_4 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_4 == PRESSED);
            return keyVal = PRESS2_4;
        }
        ROW_3 = PRESSED;
        ROW_1 = ROW_2 = ROW_4 = UNPRESSED;
        if (ROW_3 == PRESSED && COL_1 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_1 == PRESSED);
            return keyVal = PRESS3_1;
        }
        else if (ROW_3 == PRESSED && COL_2 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_2 == PRESSED);
            return keyVal = PRESS3_2;
        }
        else if (ROW_3 == PRESSED && COL_3 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_3 == PRESSED);
            return keyVal = PRESS3_3;
        }
        else if (ROW_3 == PRESSED && COL_4 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_4 == PRESSED);
            return keyVal = PRESS3_4;
        }
        ROW_4 = PRESSED;
        ROW_1 = ROW_2 = ROW_3 = UNPRESSED;
        if (ROW_4 == PRESSED && COL_1 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            DelayTimeMs(3);
            while (COL_1 == PRESSED);
            return keyVal = PRESS4_1;
        }
        else if (ROW_4 == PRESSED && COL_2 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_2 == PRESSED);
            return keyVal = PRESS4_2;
        }
        else if (ROW_4 == PRESSED && COL_3 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_3 == PRESSED);
            return keyVal = PRESS4_3;
        }
        else if (ROW_4 == PRESSED && COL_4 == PRESSED) {
            beep = BEEP_RING;
            DelayTimeMs(3);
            beep = BEEP_UNRING;
            while (COL_4 == PRESSED);
            return keyVal = PRESS4_4;
        }
    }while (keyVal == -1);
    ROW_1 = ROW_2 = ROW_3 = ROW_4 = UNPRESSED;
	return keyVal;
}

