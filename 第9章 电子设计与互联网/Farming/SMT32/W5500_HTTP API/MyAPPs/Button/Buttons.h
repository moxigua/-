

#ifndef __Buttons_H__
#define __Buttons_H__

#include "stm32f10x.h"


#define Buttons_Count 4

#define Button1_Port  GPIOA
#define Button1_Pin   GPIO_Pin_0
#define Button1_Port_RCC RCC_APB2Periph_GPIOA
#define Button1_IsPressed() (GPIO_ReadInputDataBit(Button1_Port, Button1_Pin))

#define Button2_Port  GPIOE
#define Button2_Pin   GPIO_Pin_5
#define Button2_Port_RCC RCC_APB2Periph_GPIOE
#define Button2_IsPressed() (!GPIO_ReadInputDataBit(Button2_Port, Button2_Pin))

#define Button3_Port  GPIOE
#define Button3_Pin   GPIO_Pin_6
#define Button3_Port_RCC RCC_APB2Periph_GPIOE
#define Button3_IsPressed() (!GPIO_ReadInputDataBit(Button3_Port, Button3_Pin))

#define Button4_Port  GPIOF
#define Button4_Pin   GPIO_Pin_6
#define Button4_Port_RCC RCC_APB2Periph_GPIOF
#define Button4_IsPressed() (!GPIO_ReadInputDataBit(Button4_Port, Button4_Pin))


extern void Buttons_Inti(void);
extern uint32_t  Buttons_Read(void);
extern uint8_t Buttongs_Count(void);

#endif


