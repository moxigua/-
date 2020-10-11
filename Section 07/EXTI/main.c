
#include "stm32f10x.h" //使用STM32标准函数库需要添加此头文件
#include "exti.h"
#include "led.h"

int main(void)
{
	Led_Init();  	// Led 初始化
	
	ToggleLed2();
	
	EXTIX_Init(); 	// 外部中断初始化

	while(1);     	// 程序停留在这等待中断
}//主函数结束


