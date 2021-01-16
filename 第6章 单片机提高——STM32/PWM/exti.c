
/**************  以下为【exti.c】文件中的内容  **********************/
#include "exti.h"
#include "led.h"
//#include "delay.h"
#include "stm32f10x.h"

void Key_Init(void);

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE); //使能PORTA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //初始化WK_UP-->GPIOA.0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0 设置成输入，上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA.0
}

void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 第(1)步 配置IO为输入*/
	Key_Init();  
	/* 第(2)步 开启IO 口复用时钟，设置IO 口与中断线的映射关系*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); // PA0
	/* 第(3)步 初始化线上中断，设置触发条件等*/
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;                   	// EXTI0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        	// 中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 	//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					// 使能
	EXTI_Init(&EXTI_InitStructure); //初始化中断线参数
	/* 第(4)步 配置中断分组（NVIC），并使能中断*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 	//使能按键外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级2，
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; 	//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		//使能外部中断通道
	
	NVIC_Init(&NVIC_InitStructure);

}

/* 第(5)步 编写中断服务函数*/
void EXTI0_IRQHandler(void)
{
	//delay_ms(10); //消抖
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0){ // PA.0是否被按下
		ToggleLed1();// 切换灯的状态
	}
	while( GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0) // 等待按钮释放
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE2 上的中断标志位
}

