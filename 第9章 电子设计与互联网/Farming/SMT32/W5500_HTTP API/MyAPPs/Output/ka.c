

#include "output/ka.h"
#include "stm32f10x.h"


int  KA_Init(void)
{			    		   
	GPIO_InitTypeDef GPIO_InitStructure;
	//使能RCC
	RCC_APB2PeriphClockCmd(KA1_Port_RCC | KA2_Port_RCC | KA3_Port_RCC |KA4_Port_RCC,ENABLE);
  //KA1
	GPIO_InitStructure.GPIO_Pin = KA1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(KA1_Port, &GPIO_InitStructure);
	//KA2
	GPIO_InitStructure.GPIO_Pin = KA2_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(KA2_Port, &GPIO_InitStructure);
	//KA3
	GPIO_InitStructure.GPIO_Pin = KA3_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(KA3_Port, &GPIO_InitStructure);
	//KA4
	GPIO_InitStructure.GPIO_Pin = KA4_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(KA4_Port, &GPIO_InitStructure);
	
	KA_Close(1);
	KA_Close(2);
	KA_Close(3);
	KA_Close(4);
	
	return 0;
}

int KA_Open(int ka)
{
	switch(ka)
	{
	
		case 1:
			KA1_Open();
			break;
		case 2:
			KA2_Open();
			break;
		case 3:
			KA3_Open();
			break;
		case 4:
			KA4_Open();
			break;
		default:
			return -1;
	}
	return 0;
}

int KA_Close(int ka)
{
	switch(ka)
	{
	
		case 1:
			KA1_Close();
			break;
		case 2:
			KA2_Close();
			break;
		case 3:
			KA3_Close();
			break;
		case 4:
			KA4_Close();
			break;
		default:
			return -1;
	}
	return 0;
}


int KA_GetCount(void)
{
	return KA_Count;
}
