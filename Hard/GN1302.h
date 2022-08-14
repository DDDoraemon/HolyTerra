#ifndef __GN13002_H__
#define __GN13002_H__

/**
 * @brief 时间结构体 7字节
 */
typedef struct Time {
    uint8_t m_sec;    /* seconds 00-59*/
    uint8_t m_min;    /* minutes 00-59*/
    uint8_t m_hour;   /* hours 1-12/0-23*/
    uint8_t m_day;    /* days 1-31*/
    uint8_t m_month;  /* months 1-12*/
    uint8_t m_year;   /* years 00-99*/
    uint8_t m_week;   /* weeks 1-7*/
}time;

/**
 * @brief GN1302初始化 若初试过后断电重启 则跳过初始化
 * @return bool 初始化成功返回true 失败返回false;
 */
bool GN1302Init(void);

// 读取当前时间 当前时间仅有一个全局指针 不再设置其他
void ReadTime(time *currentTime);

// 用来修改GN1302的内部时间
void WriteTime(time *setTime);

#endif