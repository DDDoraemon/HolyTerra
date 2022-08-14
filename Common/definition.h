#ifndef _DEFINITION_H_
#define _DEFINITION_H_

#define OK  0
#define ERR 1

/*!< Signed integer types  */
//51中 short和int都是两个字节长 long是4个字节长
typedef signed char     int8_t;     /* -128~127 */
typedef signed int    	int16_t;    /* -32768~32767 */
typedef signed long     int32_t;    /* -2147483648 ~ 2147483647*/
 
/*!< Unsigned integer types  */
typedef unsigned char   uint8_t;    /* 0 ~ 255 */
typedef unsigned int    uint16_t;   /* 0 ~ 65535 */
typedef unsigned long   uint32_t;   /* 0 ~ 4294967295*/

/** 
 * @brief 定义布尔类型 1字节
 */
typedef enum Bool {
    false = 0,
    true = 1
}bool;

#endif // !_DEFINITION_H_