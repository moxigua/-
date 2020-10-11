

#include "Buttons.h"


void Buttons_Inti(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(Button1_Port_RCC | Button2_Port_RCC |Button3_Port_RCC |Button4_Port_RCC, ENABLE);
		//Button1
		GPIO_InitStructure.GPIO_Pin = Button1_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(Button1_Port, &GPIO_InitStructure);
	//Button2
		GPIO_InitStructure.GPIO_Pin = Button2_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(Button2_Port, &GPIO_InitStructure);
	//Button3
		GPIO_InitStructure.GPIO_Pin = Button3_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(Button3_Port, &GPIO_InitStructure);
	//Button4
		GPIO_InitStructure.GPIO_Pin = Button4_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(Button4_Port, &GPIO_InitStructure);
}

uint32_t  Buttons_Read(void)
{
	uint32_t keys;
	keys |= (0x01 && Button1_IsPressed())<<1;	
	keys |= (0x01 && Button2_IsPressed())<<2;	
	keys |= (0x01 && Button3_IsPressed())<<3;	
	keys |= (0x01 && Button4_IsPressed())<<4;	
	return keys;
}


uint8_t Buttongs_Count(void)
{
	return  Buttons_Count;
}

