

#ifndef __ka_h__
#define __ka_h__


#define KA_Count 4

#define KA1_Port  GPIOF
#define KA1_Pin   GPIO_Pin_8
#define KA1_Port_RCC RCC_APB2Periph_GPIOF
#define KA1_Open() GPIO_SetBits(KA1_Port, KA1_Pin)
#define KA1_Close() GPIO_ResetBits(KA1_Port, KA1_Pin)
#define KA1_IsOpen() GPIO_ReadOutputDataBit(KA1_Port, KA1_Pin)
#define KA1_Toggle()  (KA1_IsOpen())?(KA1_Close()):(KA1_Open())


#define KA2_Port  GPIOF
#define KA2_Pin   GPIO_Pin_9
#define KA2_Port_RCC RCC_APB2Periph_GPIOF
#define KA2_Open() GPIO_SetBits(KA2_Port, KA2_Pin)
#define KA2_Close() GPIO_ResetBits(KA2_Port, KA2_Pin)
#define KA2_IsOpen() GPIO_ReadOutputDataBit(KA2_Port, KA2_Pin)
#define KA2_Toggle()  (KA2_IsOpen())?(KA2_Close()):(KA2_Open())


#define KA3_Port  GPIOF
#define KA3_Pin   GPIO_Pin_10
#define KA3_Port_RCC RCC_APB2Periph_GPIOF
#define KA3_Open() GPIO_SetBits(KA3_Port, KA3_Pin)
#define KA3_Close() GPIO_ResetBits(KA3_Port, KA3_Pin)
#define KA3_IsOpen() GPIO_ReadOutputDataBit(KA3_Port, KA3_Pin)
#define KA3_Toggle()  (KA3_IsOpen())?(KA3_Close()):(KA3_Open())


#define KA4_Port  GPIOF
#define KA4_Pin   GPIO_Pin_11
#define KA4_Port_RCC RCC_APB2Periph_GPIOF
#define KA4_Open() GPIO_SetBits(KA4_Port, KA4_Pin)
#define KA4_Close() GPIO_ResetBits(KA4_Port, KA4_Pin)
#define KA4_IsOpen() GPIO_ReadOutputDataBit(KA4_Port, KA4_Pin)
#define KA4_Toggle()  (KA4_IsOpen())?(KA4_Close()):(KA4_Open())



extern int KA_Init(void);
extern int KA_Open(int ka);
extern int KA_Close(int ka);
extern int KA_GetCount(void);


#endif
