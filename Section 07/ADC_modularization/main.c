
#include "stm32f10x.h" //使用STM32标准函数库需要添加此头文件
#include "stdio.h"
#include "adc.h"
#include "stdout_USART.h"

int main(void)
{
	 uint16_t adcx;float real_v;
	
	 stdout_init();  //  初始化printf()函数
	 while(1){
		 ADC_Inti();     // ADC初始化
		 adcx = Get_Adc(1); // 获得AD数据
		 real_v=(float)adcx*(2.5/4096); // 转换成真实电压值
		 printf("get value of adc is :%d\r\n",adcx);
		 printf("real v is :%f\r\n",real_v);
	 }
}




