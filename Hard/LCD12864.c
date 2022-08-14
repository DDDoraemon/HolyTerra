#include "../common/chinesecharlib.h"
#include "LCD12864.h"

extern uint8_t g_Status;        /* 全局状态 */
extern uint8_t g_DoorNum;       //可修改的柜门数量
extern door g_Cabinet[];        //柜门数组
extern uint8_t g_Station;       //可修改的站点号

extern uint8_t g_Allot;         //分配
extern uint8_t g_EffectTime;    //有效时间
extern uint8_t g_MinMinute;     //最小时间
extern uint8_t g_Money;         //投币
extern uint8_t g_FreeTime;      //免费时长
extern uint8_t g_Voice;         //语音
extern uint8_t g_QuZhi;         //取纸
extern uint8_t g_NongDu;        //浓度
extern uint8_t g_QieZhi;        //切纸

static void Busy(void)
{
	uint8_t temp;
    RS=0;
    RW=1;
	DATABUS=0xff;
	while(1)
    {
        EN=1;
        temp=DATABUS; 					
        EN=0;
        if ((temp&0x80)==0) 
        break;  				    
    }	
}

static void writeLCD(uint8_t rs, uint8_t lr, uint8_t cd) {
    Busy(); 

    RW = 0;
    RS = rs;

    //选择半屏
    if (lr == LEFT) {
        CSL = 1;
        CSR = 0;
    }
    else if (lr == RIGHT) {
        CSL = 0;
        CSR = 1;
    }
    else if(lr == ALL) {
        CSL = 1;
        CSR = 1;
    }

    DATABUS = cd;

    EN = 1;
    EN = 0;
}

void clearLCD() {
    uint8_t i, j;
    writeLCD(CMD, ALL, Z);
    writeLCD(CMD, ALL, X);
    writeLCD(CMD, ALL, Y);
    
    for (i = 0; i < 8; i++) {  //行扫描

        writeLCD(CMD, ALL, i + X);

        for (j = 0; j < 64; j++) {  //列扫描
            writeLCD(DAT, ALL, CLEAR);
        }
    }
}

void LCDInit() {
    writeLCD(CMD, ALL, OFF);    //左右屏设置为关
    writeLCD(CMD, ALL, ON);     //左右屏设置为开
    writeLCD(CMD, ALL, X);
    writeLCD(CMD, ALL, Y);
    writeLCD(CMD, ALL, Z);
    clearLCD();
}

void showNumber(uint8_t xx, uint8_t yy, uint8_t num) {
    showWord(xx, yy, numbers + (24 * num), 12);
}

void showWord(uint8_t xx, uint8_t yy, uint8_t* word, uint8_t length) {
    uint8_t i, j;
    for (i = 0; i < 2; i++) {  //充填行
        writeLCD(CMD, ALL, Z);  
        writeLCD(CMD, ALL, X + xx + i);

        if (yy < 64) {
            writeLCD(CMD, LEFT, Y + yy);  //充填列
            for (j = 0; j < length; j++) {
                writeLCD(DAT, LEFT, *word);        
                word++;
            }
        }
        else {
            writeLCD(CMD, RIGHT, Y + (yy - 64));
            for (j = 0; j < length; j++) {
                writeLCD(DAT, RIGHT, *word);              
                word++;
            }
        }
        
    }
}

void showSpecialWord(uint8_t xx, uint8_t special, uint8_t* word, uint8_t length) {
    uint8_t i, j;
    for (i = 0; i < 2; i++) {  //充填行
        writeLCD(CMD, ALL, Z);  
        writeLCD(CMD, ALL, X + xx + i);
        writeLCD(CMD, LEFT, Y + (64 - special));  //充填列
        for (j = 0; j < special; j++) {
            writeLCD(DAT, LEFT, *word);        
            word++;
        }

        writeLCD(CMD, RIGHT, Y);
        for (j = 0; j < length - special; j++) {
            writeLCD(DAT, RIGHT, *word);        
            word++;
        }
    }
}

// void showUnderLine(uint8_t xx, uint8_t yy, uint8_t* word) {
//     uint8_t i, j;
//     for (i = 0; i < 2; i++) {  //充填行
//         writeLCD(CMD, ALL, Z);  
//         writeLCD(CMD, ALL, X + xx + i);

//         if (yy < 64) {
//             writeLCD(CMD, LEFT, Y + yy);  //充填列
//             for (j = 0; j < 12; j++) {
//                 writeLCD(DAT, LEFT, *word);        
//                 word++;
//             }
//         }
//         else {
//             writeLCD(CMD, RIGHT, Y + (yy - 64));
//             for (j = 0; j < 12; j++) {
//                 writeLCD(DAT, RIGHT, *word);              
//                 word++;
//             }
//         }    
//     }
// }

void showCenter(uint8_t xx, uint8_t size, uint8_t* word) {
    uint8_t i, j;
    
    for (i = 0; i < 2; i++) {
        writeLCD(CMD, ALL, X + xx + i);

        writeLCD(CMD, LEFT, Y + 58);  //设置左屏
        for (j = 0; j < (size / 2); j++) {
            writeLCD(DAT, LEFT, *word);
            word++;
        }

        writeLCD(CMD, RIGHT, Y);    //设置右屏
        for (j = 0; j < (size / 2); j++) {
            writeLCD(DAT, RIGHT, *word);
            word++;
        }
    }
}

void showTXT(uint8_t xx, uint8_t yy, uint8_t* word, uint8_t num, uint8_t length, uint8_t skip) {
	uint8_t i = 0;
	for (i; i < num; i++) {
		showWord(xx, yy + 12 * i, word + skip * i, length);
	}
}

void showMain() {
    writeLCD(CMD, ALL, Z);  //设置初始行
    
    showTXT(0, 34, cun, 2, 12, 24);
    showCenter(0, 12, an);
    showTXT(0, 70, jian, 2, 12, 24);

    showTXT(2, 34, qu, 2, 12, 24);
    showCenter(2, 12, miao);
    showTXT(2, 70, tiao, 2, 12, 24);

    showNumber(4, 52, g_Station);
    showWord(4, 66, gui, 12);

    showDoor();
}

void showNum() {
    showWord(0, 0, shuzi, 12);    
    showWord(0, 48, shuzi + 24, 12);
    showWord(0, 93, shuzi + 48, 12);

    showWord(2, 0, shuzi + 72, 12);    
    showWord(2, 48, shuzi + 96, 12);
    showWord(2, 93, shuzi + 120, 12);

    showWord(4, 0, shuzi + 144, 12);    
    showWord(4, 48, shuzi + 168, 12);

    showTXT(6, 0, tuichu, 4, 12, 24);

    if(g_Status == S_admin_1) {
        showWord(4, 93, N9, 12);
        showTXT(6, 92, xiaye, 3, 12, 24);
    }
    else if (g_Status == S_admin_2) {
        showTXT(6, 92, shangye, 3, 12, 24);
    }
}

//void showMenu(uint8_t * menu) {
void showMenu(uint8_t opt) {
    uint8_t *menu = 0;
    if (opt == 1) {
        menu = kaixiang;
    }
    else {
        menu = shijian;
    }
        
    showTXT(0, 10, menu, 2, 12, 24); 
    showCenter(0, 12, menu + 48);
    showWord(0, 70, menu + 72, 12);  
    showTXT(0, 104, menu + 96, 2, 12, 24);

    showTXT(2, 10, menu + 144, 2, 12, 24); 
    showCenter(2, 12, menu + 192);
    showWord(2, 70, menu + 216, 12);  
    showTXT(2, 104, menu + 240, 2, 12, 24); 

    showTXT(4, 10, menu + 288, 2, 12, 24); 
    showCenter(4, 12, menu + 336);
    showWord(4, 70, menu + 360, 12);  
    if(g_Status == S_admin_1) {
        showTXT(4, 104, menu + 384, 2, 12, 24);  
    }     
}

void showBlock(uint8_t num, uint8_t* graph) {
/*柜门编号排布
    1   7   13  19
    2   8   14  20
    3   9   15  21 
    4   10  16  22  
    5   11  17  23  
    6   12  18  24

    原程序显示 
    2:  1    1
    4:  2    2
    5:  5
    6:  6
    8:  4    4
    9:  6    3
    10: 5    5
    12: 6    6
    14: 7    7
    15: 55   5
    16: 44   44
    18: 66   6
    20: 55   55
    21: 77    7
    24: 66   66
    其他情况均无显示
*/
    uint8_t i, xx, yy;

    switch (g_DoorNum) {
        case 2:
            xx = 0;
            if (num == 1) yy = 0;
            else yy = 122;
        break;
        
        case 4:
            if (num == 1 || num == 3) xx = 0;
            else if (num == 2 || num == 4) xx = 1;

            if (num <= 2) yy = 0;
            else yy = 122;
        break;
        
        case 8:
            if (num == 1 || num == 5) xx = 0;
            else if (num == 2 || num == 6) xx = 1;
            else if (num == 3 || num == 7) xx = 2;
            else if (num == 4 || num == 8) xx = 3;

            if (num <= 4) yy = 0;
            else yy = 122;
        break;

        case 9:
            if (num == 1 || num == 7) xx = 0;
            else if (num == 2 || num == 8) xx = 1;
            else if (num == 3 || num == 9) xx = 2;
            else if (num == 4) xx = 3;
            else if (num == 5) xx = 4;
            else if (num == 6) xx = 5;

            if (num <= 6) yy = 0;
            else yy = 122;
        break;

        case 10:
            if (num == 1 || num == 6) xx = 0;
            else if (num == 2 || num == 7) xx = 1;
            else if (num == 3 || num == 8) xx = 2;
            else if (num == 4 || num == 9) xx = 3;
            else if (num == 5 || num == 10) xx = 4;

            if (num <= 5) yy = 0;
            else yy = 122;
        break;

        case 12:
            if (num == 1 || num == 7) xx = 0;
            else if (num == 2 || num == 8) xx = 1;
            else if (num == 3 || num == 9) xx = 2;
            else if (num == 4 || num == 10) xx = 3;
            else if (num == 5 || num == 11) xx = 4;
            else if (num == 6 || num == 12) xx = 5;

            if (num <= 6) yy = 0;
            else yy = 122;
        break;

        case 14:
            if (num == 1 || num == 8) xx = 0;
            else if (num == 2 || num == 9) xx = 1;
            else if (num == 3 || num == 10) xx = 2;
            else if (num == 4 || num == 11) xx = 3;
            else if (num == 5 || num == 12) xx = 4;
            else if (num == 6 || num == 13) xx = 5;
            else if (num == 7 || num == 14) xx = 6;

            if (num <= 7) yy = 0;
            else yy = 122;
        break;

        //15: 55   5
        case 15: 
            if (num == 1 || num == 6 || num == 11) xx = 0;
            else if (num == 2 || num == 7 || num == 12) xx = 1;
            else if (num == 3 || num == 8 || num == 13) xx = 2;
            else if (num == 4 || num == 9 || num == 14) xx = 3;
            else if (num == 5 || num == 10 || num == 15) xx = 4;

            if (num <= 5) yy = 0;
            else if (num <= 10) yy = 6;
            else yy = 122;
        break;

        //16: 44   44
        case 16:
            if (num == 1 || num == 5 || num == 9 || num == 13) xx = 0;
            else if (num == 2 || num == 6 || num == 10 || num == 14) xx = 1;
            else if (num == 3 || num == 7 || num == 11 || num == 15) xx = 2;
            else if (num == 4 || num == 8 || num == 12 || num == 16) xx = 3;

            if (num <= 4) yy = 0;
            else if (num <= 8) yy = 6;
            else if (num <= 12) yy = 116;
            else yy = 122;
        break;

        //18: 66   6
        case 18:
            if (num == 1 || num == 7 || num == 13) xx = 0;
            else if (num == 2 || num == 8 || num == 14) xx = 1;
            else if (num == 3 || num == 9 || num == 15) xx = 2;
            else if (num == 4 || num == 10 || num == 16) xx = 3;
            else if (num == 5 || num == 11 || num == 17) xx = 4;
            else if (num == 6 || num == 12 || num == 18) xx = 5;

            if (num <= 6) yy = 0;
            else if (num <= 12) yy = 6;
            else yy = 122;
        break;

        //20: 55   55
        case 20:
            if (num == 1 || num == 6 || num == 11 || num == 16) xx = 0;
            else if (num == 2 || num == 7 || num == 12 || num == 17) xx = 1;
            else if (num == 3 || num == 8 || num == 13 || num == 18) xx = 2;
            else if (num == 4 || num == 9 || num == 14 || num == 19) xx = 3;
            else if (num == 5 || num == 10 || num == 15 || num == 20) xx = 4;

            if (num <= 5) yy = 0;
            else if (num <= 10) yy = 6;
            else if (num <= 15) yy = 116;
            else yy = 122;
        break;

        //21: 77    7
        case 21:
            if (num == 1 || num == 8 || num == 15) xx = 0;
            else if (num == 2 || num == 9 || num == 16) xx = 1;
            else if (num == 3 || num == 10 || num == 17) xx = 2;
            else if (num == 4 || num == 11 || num == 18) xx = 3;
            else if (num == 5 || num == 12 || num == 19) xx = 4;
            else if (num == 6 || num == 13 || num == 20) xx = 5;
            else if (num == 7 || num == 14 || num == 21) xx = 6;

            if (num <= 7) yy = 0;
            else if (num <= 14) yy = 6;
            else yy = 122;
        break;

        //24: 66   66
        case 24:
            if (num == 1 || num == 7 || num == 13 || num == 19) xx = 0;
            else if (num == 2 || num == 8 || num == 14 || num == 20) xx = 1;
            else if (num == 3 || num == 9 || num == 15 || num == 21) xx = 2;
            else if (num == 4 || num == 10 || num == 16 || num == 22) xx = 3;
            else if (num == 5 || num == 11 || num == 17 || num == 23) xx = 4;
            else if (num == 6 || num == 12 || num == 18 || num == 24) xx = 5;

            if (num <= 6) yy = 0;
            else if (num <= 12) yy = 6;
            else if (num <= 18) yy = 116;
            else yy = 122;
        break;

        default:
            if (num == 1 || num == 7 || num == 13 || num == 19) {
                xx = 0;
            }
            else if (num == 2 || num == 8 || num == 14 || num == 20) {
                xx = 1;
            }
            else if (num == 3 || num == 9 || num == 15 || num == 21) {
                xx = 2;
            }
            else if (num == 4 || num == 10 || num == 16 || num == 22) {
                xx = 3;
            }
            else if (num == 5 || num == 11 || num == 17 || num == 23) {
                xx = 4;
            }
            else if (num == 6 || num == 12 || num == 18 || num == 24) {
                xx = 5;
            }

            if (num >= 1 && num < 7) {
                yy = 0;
            }
            else if (num >= 7 && num < 13) {
                yy = 6;
            }
            else if (num >= 13 && num < 19) {
                yy = 116;
            }
            else if (num >= 19 && num <= 24) {
                yy = 122;
            }
    }

    writeLCD(CMD, ALL, X + xx);
    if (yy < 64) {
        writeLCD(CMD, LEFT, Y + yy);  
        for (i = 0; i < 5; i++) {
            writeLCD(DAT, LEFT, *graph);
            graph++;
        }        
    }
    else {
        writeLCD(CMD, RIGHT, Y + (yy - 64));
        for (i = 0; i < 5; i++) {
            writeLCD(DAT, RIGHT, *graph);
            graph++;
        }
    }
}

static void showDoor() {
    uint8_t i;

    for (i = 1; i <= g_DoorNum; i++) {
        if (g_Cabinet[i].m_Lock == true) {
            showBlock(i, locked);
        }
        else if (g_Cabinet[i].m_Status == true) {
            showBlock(i, block);
        }
        else {
            showBlock(i, box);
        }
    }
}

void showOkBack(bool hide) {
    showTXT(6, 79, fanhui, 4, 12, 24);  //返回
    if (!hide) {
        showTXT(6, 0, ok, 4, 12, 24);  //OK
    }
}

void showDoorNum(uint8_t index, uint8_t show) {
    showTXT(0, 27, qingshuru, 3, 12, 24); //请输入
    showTXT(0, 64, qingshuru + (index * 24), 4, 12, 24); //厢门号+3 原密码+12 新密码+16
    if (show) {
        showTXT(2, 16, shixiao, 4, 12, 24); //未取的箱
        showTXT(2, 64, shixiao + (24 * 4), 4, 12, 24); //门将失效
    }
    showOkBack(false);
}

void showPass() {
    showTXT(2, 21, qingshuru, 3, 12, 24); //请输入
    showCenter(2, 12, qingshuru + (24 *7));
    showTXT(2, 71, qingshuru + (24 * 8), 4, 12, 24);    
    showOkBack(false);
}

void showDoorHave(uint8_t num, uint8_t index) {
    clearLCD();

    showNumber(2, 33,num);

    showWord(2, 45, menyi, 12);
    showCenter(2, 12, menyi + 24);
    showTXT(2, 71, menyi + (24 * index), 2, 12, 24); 
}

void showPleaseCloseDoor() {
    showTXT(4, 15, qingguanmen, 4, 12, 24);
    showTXT(4, 64, qingguanmen + (24 * 4), 4, 12, 24);
}

void showErr() {
    showTXT(4, 16, shurucuowu, 4, 12, 24);
    showTXT(4, 64, shurucuowu + (24 * 5), 5, 12, 24);
}

void showIf(uint8_t index) {
    showTXT(0, 22, shifou, 2, 12, 24); //是否
    if (index == 0) {
        showWord(0, 46, shifou + (24 * 2), 12); //开
        showCenter(0, 12, shifou + (24 * 3)); //启
    }
    else if (index == 1) {
        showWord(0, 46, shifou + (24 * 7), 12); //清
        showCenter(0, 12, shifou + (24 * 8)); //空
        showTXT(2, 16, shixiao, 4, 12, 24); //未取的箱
        showTXT(2, 64, shixiao + (24 * 4), 4, 12, 24); //门将失效
    }
    showTXT(0, 70, shifou + (24 * 4), 3, 12, 24); //所有箱

    showOkBack(false);
}

void showDoing(uint8_t index) {
    clearLCD();
    showTXT(3, 28, zhengzai, 2, 12, 24);
    if (index == 0) {
        showWord(3, 52, zhengzai + (24 * 2), 12);
        showWord(3, 64, zhengzai + (24 * 3), 12);
    }
    else if (index == 1) {
        showWord(3, 52, zhengzai + (24 * 5), 12);
        showWord(3, 64, zhengzai + (24 * 6), 12);
    }

    showWord(3, 88, zhengzai + (24 * 4), 12);
}

void showAllDoorHave(uint8_t index) {
    clearLCD();
    showTXT(3, 28, suoyou, 3, 12, 24);
    showTXT(3, 64, suoyou, 1, 12, 24);
    showTXT(3, 76, suoyou + 24 * index, 2, 12, 24);
    
}

void showTime(uint8_t opt) {
	time t_CurrentTime;
    uint8_t yy;
	t_CurrentTime = readTime();

    if (opt == 6) {
        yy = 21;
	    showCenter(opt, 12, numbers + (24 * t_CurrentTime.m_day));
    }
    else {
        yy = 15;
        showNumber(opt, yy + 36, t_CurrentTime.m_day);
        showOkBack(false);
        showWord(opt, 106, colon, 6);
        showNumber(opt, 112, t_CurrentTime.m_sec);
    }

    showNumber(opt, yy, t_CurrentTime.m_year);
    showWord(opt, yy + 12, line, 6);
    showNumber(opt, yy + 18, t_CurrentTime.m_month);
    showWord(opt, yy + 30, line, 6);
	showNumber(opt, 76, t_CurrentTime.m_hour);
	showWord(opt, 88, colon, 6);
	showNumber(opt, 94, t_CurrentTime.m_min);
}

void showHaveFull() {
    showTXT(3, 34, yiman, 2, 12, 24);
    showCenter(3, 12, yiman + (24 * 2));
    showTXT(3, 70, yiman + (24 * 3), 2, 12, 24);
}

void showGuiMen() {
    showTXT(1, 16, guimen, 4, 12, 24);
    showTXT(1, 64, guimen + (24 * 4), 3, 12, 24);
    showWord(1, 100, colon, 6);

    showTXT(3, 16, guimen, 4, 12, 24);
    showTXT(3, 64, guimen + (24 * 7), 3, 12, 24);
    showWord(3, 100, colon, 6);

    showOkBack(false);
}

void showSingleNum(uint8_t num, uint8_t xx, uint8_t yy, bool under) {
    uint8_t i, j;
    uint8_t *word = num0_9 + (12 * num);
    if (under) {    
        for (i = 0; i < 2; i++) {  //充填行
            writeLCD(CMD, ALL, Z);  
            writeLCD(CMD, ALL, X + xx + i);

            if (yy < 64) {
                writeLCD(CMD, LEFT, Y + yy);  //充填列
                for (j = 0; j < 6; j++) {
                    if (i == 1) writeLCD(DAT, LEFT, ((*word) | 0x08));  //在数字下面加下划线
                    else writeLCD(DAT, LEFT, *word);        
                    word++;
                }
            }
            else {
                writeLCD(CMD, RIGHT, Y + (yy - 64));
                for (j = 0; j < 6; j++) {
                    if (i == 1) writeLCD(DAT, RIGHT, ((*word) | 0x08));
                    else writeLCD(DAT, RIGHT, *word);              
                    word++;
                }
            }
        }
    }
    else {
        showWord(xx, yy, num0_9 + (12 * num), 6);
    }
}

void showQuerenxinmima() {
    clearLCD();
    showOkBack(false);
    showTXT(0, 33, xinmima, 2, 12, 24); //确认
    showCenter(0, 12, xinmima + (24 * 2)); //新
    showTXT(0, 70, xinmima + (24 * 3), 2, 12, 24); //密码
}

void showXiugaijieguo(uint8_t success) {
    if (success) {
        showTXT(4, 28, xiugaichenggong, 3, 12, 24);
        showTXT(4, 64, xiugaichenggong + (24 * 3), 3, 12, 24);
        Delay10ms(100);
    }
    else {
        showTXT(4, 21, xinmima + (24 * 5), 3, 12, 24); //两次密
        showCenter(4, 12, xinmima + (24 * 8)); //码
        showTXT(4, 70, xinmima + (24 * 9), 3, 12, 24); //不一致
        Delay10ms(100);
    }
}

void showCunqu() {
    uint8_t i = 0;
    for (i = 0; i < 4; i++) {
        showTXT(2 * i, 4, qingshuru + 24, 2, 12, 24);
    }

    showTXT(0, 28, cunqu, 3, 12, 24);
    showTXT(0, 64, cunqu + (24 * 3), 3, 12, 24);
    showWord(0, 116, cunqu + (24 * 6), 12);

    showTXT(2, 28, cunqu + (24 * 7), 3, 12, 24);
    showTXT(2, 64, cunqu + (24 * 10), 3, 12, 24);
    showWord(2, 116, cunqu + (24 * 13), 12);

    showTXT(4, 28, cunqu + (24 * 14), 3, 12, 24);
    showTXT(4, 64, cunqu + (24 * 17), 3, 12, 24);
    showWord(4, 116, cunqu + (24 * 20), 12);

    showTXT(6, 28, cunqu + (24 * 21), 3, 12, 24);
    showTXT(6, 64, cunqu + (24 * 24), 3, 12, 24);
    showWord(6, 116, cunqu + (24 * 27), 12);
}

void showChaxun() {
    uint8_t i = 0;
    for (i; i < 3; i++) {
        showWord(2 * i, 0, shuzi + (24 * i), 12);
        showTXT(2 * i, 13, chaxun + (48 * i), 2, 12, 24);
        showTXT(2 * i, 37, chaxun + 144, 2, 12, 24);
    }
    showOkBack(true);
}

void showCunquchaxun(uint8_t flag) {
    if (flag) {
        showTXT(0, 0, cunqujilu, 2, 12, 24); //箱号
        showTXT(2, 0, cunqujilu + 48, 2, 12, 24); //密码
        showWord(4, 12, cunqujilu + (24 * 4), 12); //月
        showWord(4, 36, cunqujilu + (24 * 5), 12); //日
        showWord(4, 64, cunqujilu + (24 * 6), 12); //时
        showWord(4, 88, cunqujilu + (24 * 7), 12); //分
    }
    else {
        showWord(3, 45, jilu + (24 * 4), 12); //无
        showSpecialWord(3, 6, jilu + (24 * 2), 12); //记
        //showCenter(3, 12, jilu + (24 * 2)); 
        showWord(3, 71, jilu + (24 * 3), 12); //录
    }

}

void showGuanlichaxun(record opt) {
    showTXT(0, 0, guanlijilu, 4, 12, 24); //管理记录
    showWord(2, 12, cunqujilu + (24 * 4), 12); //月
    showWord(2, 36, cunqujilu + (24 * 5), 12); //日
    showWord(2, 64, cunqujilu + (24 * 6), 12); //时
    showWord(2, 88, cunqujilu + (24 * 7), 12); //分
    
    switch(opt) {
        case O_enterAdmin:
            showTXT(4, 0, guanlijilu + (4 * 24), 5, 12, 24); //进入管理系
            showSpecialWord(4, 3, guanlijilu + (9 * 24), 12); //统
        break;

        case O_openOne:
            showTXT(4, 0, guanlijilu + (10 * 24), 2, 12, 24); //开启
            showWord(4, 36, guanlijilu + (20 * 24), 12); //箱
        break;

        case O_allOpen:
            showTXT(4, 0, guanlijilu + (10 * 24), 2, 12, 24); //开启
            showTXT(4, 24, guanlijilu + (18 * 24), 2, 12, 24); //所有
            showWord(4, 48, guanlijilu + (20 * 24), 12); //箱
        break;

        case O_clearOne:
            showTXT(4, 0, guanlijilu + (12 * 24), 2, 12, 24); //清空
            showWord(4, 36, guanlijilu + (20 * 24), 12); //箱
        break;

        case O_allClear:
            showTXT(4, 0, guanlijilu + (12 * 24), 2, 12, 24); //清空
            showTXT(4, 24, guanlijilu + (18 * 24), 2, 12, 24); //所有
            showWord(4, 48, guanlijilu + (20 * 24), 12); //箱
        break;

        case O_lock:
            showTXT(4, 0, guanlijilu + (14 * 24), 2, 12, 24); //锁定
            showWord(4, 36, guanlijilu + (20 * 24), 12); //箱
        break;

        case O_unlock:
            showTXT(4, 0, guanlijilu + (16 * 24), 2, 12, 24); //解锁
            showWord(4, 36, guanlijilu + (20 * 24), 12); //箱
        break;
    }
}

void showJilushumu(uint8_t num) {
    showWord(3, 33, jilu, 12);
    showNumber(3, 45, num);
    showCenter(3, 12, jilu + (24 * 1));
    showTXT(3, 70, jilu + (24 * 2), 2, 12, 24);
}

void showFenpei() {
    uint8_t i = 0;
    for (i; i < 2; i++) {
        showWord(2 * i, 0, shuzi + (24 * i), 12);
        showTXT(2 * i, 13, fenpei + (48 * i), 2, 12, 24);
        showTXT(2 * i, 37, fenpei + 96, 1, 12, 24);
    }
    if (g_Allot == 1) {
        showTXT(3, 64, fenpei, 2, 12, 24);
    }
    else {
        showTXT(3, 64, fenpei + 48, 2, 12, 24);
    }
    showWord(3, 88, fenpei + 96, 12);

    showOkBack(false);
}

void showYuyin() {
    uint8_t i = 0;
    for (i; i < 2; i++) {
        showWord(2 * i, 0, shuzi + (24 * i), 12);
    }
    showTXT(0, 13, menyi + 192, 2, 12, 24);
    showTXT(2, 13, menyi + 144, 2, 12, 24);

    if (g_Voice == 1) {
        showTXT(3, 64, menyi + 192, 2, 12, 24);
    }
    else {
        showTXT(3, 64, menyi + 144, 2, 12, 24);
    }
    showOkBack(false);
}

void showQuzhi() {
    uint8_t i = 0;
    for (i; i < 2; i++) {
        showWord(2 * i, 0, shuzi + (24 * i), 12);
    }
    showTXT(0, 13, menyi + 192, 2, 12, 24);
    showTXT(2, 13, menyi + 144, 2, 12, 24);

    if (g_QuZhi == 1) {
        showTXT(3, 64, menyi + 192, 2, 12, 24);
    }
    else {
        showTXT(3, 64, menyi + 144, 2, 12, 24);
    }
    showOkBack(false);
}

void showHuifu() {
    showTXT(3, 16, huifu, 4, 12, 24);
    showTXT(3, 64, huifu + 96, 4, 12, 24);
    showOkBack(false);
}

void showHuifuzhong() {
    showTXT(3, 33, huifu + (24 * 2), 2, 12, 24);
    showCenter(3, 12, huifu + (24 * 6));
    showTXT(3, 70, huifu + (24 * 7), 2, 12, 24);
}

void showNongdu() {
    showOkBack(true);

    showTXT(2, 10, nongdu, 4, 12, 24);
    showCenter(2, 12, nongdu + (24 * 4));
    showTXT(2, 70, nongdu + (24 * 5), 4, 12, 24);
}

void showQiezhi() {
    uint8_t i = 0;
    for (i; i < 2; i++) {
        showWord(2 * i, 0, shuzi + (24 * i), 12);
        showTXT(2 * i, 13, qiezhi + (48 * i), 2, 12, 24);
    }
    if (g_QieZhi == 1) {
        showTXT(3, 64, qiezhi, 2, 12, 24);
    }
    else {
        showTXT(3, 64, qiezhi + 48, 2, 12, 24);
    }
    showOkBack(true);
}

void showHideOne() {
    showOkBack(true);

    showTXT(0, 0, hide1, 5, 12, 24);
    showTXT(0, 72, hide1 + (24 * 5), 5, 12, 24);
    showTXT(2, 0, hide1 + (24 * 10), 5, 12, 24);
    showTXT(2, 72, hide1 + (24 * 15), 5, 12, 24);
    showTXT(4, 0, hide1 + (24 * 20), 5, 12, 24);
    showTXT(4, 66, hide1 + (24 * 25), 5, 12, 24);
}

void showHideTwo() {
    uint8_t i = 0;
    for (i; i < 2; i++) {
        showWord(2 * i, 0, shuzi + (24 * i), 12);
        showTXT(2 * i, 13, hide2 + (48 * i), 2, 12, 24);
    }
    // if (g_QieZhi == 1) {
    //     showTXT(3, 64, qiezhi, 2, 12, 24);
    // }
    // else {
    //     showTXT(3, 64, qiezhi + 48, 2, 12, 24);
    // }
    showOkBack(true);
}

