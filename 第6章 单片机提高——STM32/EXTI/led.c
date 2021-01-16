


#include "led.h"
#include "stm32f10x.h"

int Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Led1_Port_RCC | Led2_Port_RCC |Led3_Port_RCC , ENABLE);
	//Led1
	GPIO_InitStructure.GPIO_Pin = Led1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Led1_Port, &GPIO_InitStructure);
	//Led2
	GPIO_InitStructure.GPIO_Pin = Led2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Led2_Port, &GPIO_InitStructure);
	//Led3
	GPIO_InitStructure.GPIO_Pin = Led3_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(Led3_Port, &GPIO_InitStructure);
	
	Led1_Close();
	Led2_Close();
	Led3_Close();
	return 0;
}


int Led_GetCount(void)
{
	return Led_Count;
}



void ToggleLed1(void)
{
	if(!GPIO_ReadOutputDataBit(Led1_Port, Led1_Pin))
	{
		Led1_Close();
	}
	else
	{
		Led1_Open();
	}
		
}

void ToggleLed2(void)
{
	if(!GPIO_ReadOutputDataBit(Led2_Port, Led2_Pin))
	{
		Led2_Close();
	}
	else
	{
		Led2_Open();
	}
		
}

void ToggleLed3(void)
{
	if(!GPIO_ReadOutputDataBit(Led3_Port, Led3_Pin))
	{
		Led3_Close();
	}
	else
	{
		Led3_Open();
	}
		
}

