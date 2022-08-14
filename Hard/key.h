#ifndef __KEY_H__
#define __KEY_H__

//键值
#define KEY_1   press4_1     
#define KEY_2   press3_1      
#define KEY_3   press2_1     
#define KEY_4   press4_2     
#define KEY_5   press3_2     
#define KEY_6   press2_2     
#define KEY_7   press4_3    
#define KEY_8   press3_3    
#define KEY_9   press2_3    
#define KEY_0   press3_4    
//功能键
#define KEY_S   press4_4    //存
#define KEY_F   press2_4    //取 
#define KEY_U   press1_1    //上
#define KEY_D   press1_2    //下
#define KEY_L   press1_3    //左
#define KEY_OK  press1_4    //ok键  

/**
 * @brief 获取键值
 * @param 无
 * @return 键值
 */
uint8_t getKey();

#endif // !__KEY_H__
