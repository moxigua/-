
#include "stm32f10x.h" //使用STM32标准函数库需要添加此头文件
#include "led.h"

void MY_TIM3_Init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   // 初始化结构体
	/*  1.分配时钟  */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	/* 2.初始化定时器相关配置  */
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	/* 这个TIM_ClockDivision 是设置与进行输入捕获相关的分频 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	/* 3.打开定时器  */
	TIM_Cmd(TIM3,ENABLE);
}

int main(void){
	Led_Init();
	MY_TIM3_Init(7199,9999);  //定时器初始化
	while(1){
		if(TIM_GetFlagStatus(TIM3,TIM_IT_Update)){  //检测更新标志位
			TIM_ClearFlag(TIM3,TIM_IT_Update);  //清除标志位
			ToggleLed1();
		}
	}
}
