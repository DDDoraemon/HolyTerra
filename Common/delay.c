// 公共头文件
#include <intrins.h>
#include "definition.h"
#include "delay.h"

void Delay10ms(uint8_t time)		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();

    if (time == 3) {
        i = 2;
        j = 67;
        k = 183;
    }
    else if (time == 30) {
        i = 13;
        j = 156;
        k = 83;
    }
    else if (time == 50) {
        i = 22;
        j = 3;
        k = 227;
    }
    else if (time == 100) {
        i = 43;
        j = 6;
        k = 203;
    }

	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void DelayTimeMs(uint8_t time) // 11.0592MHz
{
    uint8_t i, j, k;

    _nop_();
	_nop_();

    switch (time) {
        case 3:
            i = 33;
	        j = 66;
            k = 1;
            break;
        case 10:
            i = 108;
	        j = 145;
            k = 1;
            break;
        case 30:
            i = 2;
	        j = 67;
	        k = 183;
            break;
        case 50:
            i = 3;
	        j = 26;
	        k = 223;
            break;
        case 100:
            i = 5;
	        j = 52;
	        k = 195;
            break;
        default: // 默认3ms
            i = 33;
	        j = 66;
            k = 1;
            break;
    }

    do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);

}
