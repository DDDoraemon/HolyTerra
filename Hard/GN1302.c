#include <intrins.h>
#include "../Common/STC15F2K60S2.h"
#include "../Common/definition.h"

#include "GN1302.h"

#define WRITE_SEC_CMD 	0x80
#define WRITE_MIN_CMD 	0x82
#define WRITE_HOUR_CMD 	0x84
#define WRITE_DAY_CMD 	0x86
#define WRITE_MON_CMD 	0x88
#define WRITE_WEEK_CMD 	0x8A
#define WRITE_YEAR_CMD 	0x8C

#define READ_SEC_CMD 	0x81
#define READ_MIN_CMD 	0x83
#define READ_HOUR_CMD 	0x85
#define READ_DAY_CMD 	0x87
#define READ_MON_CMD 	0x89
#define READ_WEEK_CMD 	0x8B
#define READ_YEAR_CMD 	0x8D

#define INIT_SEC	0
#define INIT_MIN	0
#define INIT_HOUR	0
#define INIT_DAY	1
#define INIT_MON	1
#define INIT_WEEK	7
#define INIT_YEAR	22


sbit gn1302Sclk = P1^2;	//串行时钟输入端
sbit gn1302Io   = P1^3;	//双向输入线（数据线）
sbit gn1302Ce   = P1^4;	//使能端

/**
 * @brief 静态函数 数据转为BCD码 例如 (369)10 = (0011 0110 1001)8421
 * @param date 待转换的十进制数
 * @return uint8_t 转换后的BCD码
 */
static uint8_t DateToBCD(uint8_t date); 

/**
 * @brief 静态函数 BCD转十进制
 * @param date 待转换的BCD
 * @return uint8_t 转换后的十进制
 */
static uint8_t BcdToDate(uint8_t date);

/**
 * @brief 静态函数 向GN1302里写一个字节
 * @param cmd 数据地址
 * @param date 要写的数据
 */
static void WriteGN1302(uint8_t cmd,uint8_t date); 

/**
 * @brief 静态函数从GN1302里读一个字节
 * @param cmd 数据地址
 * @return uint8_t 读到的数据
 */
static uint8_t ReadGN1302(uint8_t cmd);

static uint8_t DateToBCD(uint8_t date)//数据转BCD码
{
	uint8_t dat1,dat2;
	dat1 = date / 10;
	dat2 = date % 10;
	dat2 = dat2 + dat1 * 16;
	return dat2;
}

static uint8_t BcdToDate(uint8_t date)//BCD码转数据
{
	uint8_t dat1,dat2;
	dat1 = date /16;
	dat2 = date % 16;
	dat2 = dat2 + dat1 * 10;
	return dat2;
}

static void WriteGN1302(uint8_t cmd,uint8_t date)//写一个字节
{
	uint8_t i;
	gn1302Ce = 0;//拉低使能端
	gn1302Sclk = 0;//拉低数据总线
	gn1302Ce = 1;//使能端产生上升沿
	for(i = 0; i < 8; i++)//写命令
	{
		gn1302Sclk = 0;
		gn1302Io = cmd & 0x01;//最低位先传
		gn1302Sclk = 1;//数据总线产生上升沿，数据被DS1302读走
		cmd >>= 1;//右移1位
		_nop_();
	}	
	for(i = 0; i < 8; i++)//写数据
	{
		gn1302Sclk = 0;
		gn1302Io = date & 0x01;//最低位先传
		gn1302Sclk = 1;//数据总线产生上升沿，数据被DS1302读走
		date >>= 1;//右移1位
		_nop_();
	}
	gn1302Ce = 0;//拉低使能端，关闭读写功能
}

static uint8_t ReadGN1302(uint8_t cmd)//读取数据
{
	uint8_t i, data = 0;
	gn1302Ce = 0;//拉低使能端
	gn1302Sclk = 0;//拉低数据总线
	gn1302Ce = 1;//使能端产生上升沿
	for(i = 0; i < 8; i ++)//传命令
	{
		gn1302Sclk = 0;
		gn1302Io = cmd & 0x01;//最低位先传
		gn1302Sclk = 1;//数据总线产生上升沿，数据被DS1302读走
		cmd >>= 1;//右移1位
	}
	for(i = 0;i < 8; i ++)//读数据
	{
		gn1302Sclk = 0;//拉低数据总线
		data >>= 1;
		if(gn1302Io)
		{
			data |= 0x80;
		}
		gn1302Sclk = 1;//数据总线产生上升沿，数据被DS1302读走
	}
	gn1302Ce = 0;//拉低使能端，关闭读写功能
	
	//重要
	_nop_();
	gn1302Sclk = 1;
	_nop_();
	gn1302Io = 0;
	_nop_();
	gn1302Io = 1;
	_nop_();
	
	return data;
}

bool GN1302Init() {
	time t_InitTime;

	//数据是以BCD码存储的 取出时不用转为数据 若转为数据 则不能通过位与来判断是否已经初始化
	t_InitTime.m_sec = BcdToDate(ReadGN1302(READ_SEC_CMD));
	if (t_InitTime.m_sec & (0x01 << 7)) {  //秒寄存器（81H、80H）bit7定义为时钟暂停标志（CH）当该位置为1时，时钟振荡器停止，DS1302处于低功耗状态 当该位置为0时，时钟开始运行
		t_InitTime.m_year = INIT_YEAR;
		t_InitTime.m_month = INIT_MON;
		t_InitTime.m_day = INIT_DAY;

		t_InitTime.m_hour = INIT_HOUR;
		t_InitTime.m_min = INIT_MIN;
		t_InitTime.m_sec = INIT_SEC;

		t_InitTime.m_week = INIT_WEEK;

		writeTime(t_InitTime);
	}
	else {  //已经初始化过
		return false;
	}

	return true;
}

void ReadTime(time *currentTime) {
	WriteGN1302(0x8E, 0);  //清除写保护
	currentTime->m_year = BcdToDate(ReadGN1302(READ_YEAR_CMD));
	currentTime->m_month = BcdToDate(ReadGN1302(READ_MON_CMD));
	currentTime->m_day = BcdToDate(ReadGN1302(READ_DAY_CMD));

	currentTime->m_hour = BcdToDate(ReadGN1302(READ_HOUR_CMD));
	currentTime->m_min = BcdToDate(ReadGN1302(READ_MIN_CMD));
	currentTime->m_sec = BcdToDate(ReadGN1302(READ_SEC_CMD));
	currentTime->m_week = BcdToDate(ReadGN1302(READ_WEEK_CMD));
	WriteGN1302(0x8E, 0x80);  //开写保护
}

void WriteTime(time *setTime) {
	WriteGN1302(0x8E, 0);  //清除写保护
	WriteGN1302(WRITE_SEC_CMD, DateToBCD(setTime->m_sec));
	WriteGN1302(WRITE_MIN_CMD, DateToBCD(setTime->m_min));
	WriteGN1302(WRITE_HOUR_CMD, DateToBCD(setTime->m_hour));
	WriteGN1302(WRITE_DAY_CMD, DateToBCD(setTime->m_day));
	WriteGN1302(WRITE_MON_CMD, DateToBCD(setTime->m_month));
	WriteGN1302(WRITE_YEAR_CMD, DateToBCD(setTime->m_year));
	WriteGN1302(WRITE_WEEK_CMD, DateToBCD(setTime->m_week));
	WriteGN1302(0x8E, 0x80);  //开写保护
}



