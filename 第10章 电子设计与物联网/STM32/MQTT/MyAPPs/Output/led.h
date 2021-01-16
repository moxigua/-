


#ifndef __led_h__
#define __led_h__


#define Led_Count 3

#define Led1_Port  GPIOE
#define Led1_Pin   GPIO_Pin_2
#define Led1_Port_RCC RCC_APB2Periph_GPIOE
#define Led1_Close() GPIO_SetBits(Led1_Port, Led1_Pin)
#define Led1_Open() GPIO_ResetBits(Led1_Port, Led1_Pin)


#define Led2_Port  GPIOE
#define Led2_Pin   GPIO_Pin_3
#define Led2_Port_RCC RCC_APB2Periph_GPIOE
#define Led2_Close() GPIO_SetBits(Led2_Port, Led2_Pin)
#define Led2_Open() GPIO_ResetBits(Led2_Port, Led2_Pin)


#define Led3_Port  GPIOE
#define Led3_Pin   GPIO_Pin_4
#define Led3_Port_RCC RCC_APB2Periph_GPIOE
#define Led3_Close() GPIO_SetBits(Led3_Port, Led3_Pin)
#define Led3_Open() GPIO_ResetBits(Led3_Port, Led3_Pin)


extern int Led_Init(void);
extern int Led_GetCount(void);


#endif


