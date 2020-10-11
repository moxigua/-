
#include "stm32f10x.h" //使用STM32标准函数库需要添加此头文件
#include "stdio.h"
#include "stdout_USART.h"
#include "adc.h"


//毫秒级的延时
void delay_ms(u16 time)
{    
   u16 i=0;  
   while(time--)
   {
      i=12000;  //自己定义
      while(i--) ;    
   }
}

int main(void)
{
	int sum; u8 i,j; float ADC_Value[3] = {0};//用来保存经过转换得到的电压值
	
	stdout_init();  // 串口输出调试初始化
	ADC_Inti();     // ADC初始化
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//开始采集

	while(1){
		for(i=0;i<3;i++){
			sum=0;
			for(j=0;j<10;j++)
				sum+=ADCConvertedValue[j][i];
			ADC_Value[i]=(float)sum/(10*4096)*3.3;//求平均值并转换成电压值
		}
		printf("value of ADC_Channel_1 is %f\r\n",ADC_Value[0]);
		printf("value of ADC_Channel_2 is %f\r\n",ADC_Value[1]);
		printf("value of ADC_Channel_3 is %f\r\n",ADC_Value[2]);
		delay_ms(1000);
	}
}



//#include "adc.h"
//#include "stm32f10x.h"
// 
//extern __IO uint16_t ADC_ConvertedValue;
//float ADC_ConvertedValueLocal;

//int main(void)
//{  
//  stdout_init();  // 串口输出调试初始化	
//  ADC_Inti();
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//开始采集
//  printf("******This is a ADC test******\n");
//		
//  while(1)
//    {
//        ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3;
//        printf("The current AD value =0x%04X\n",ADC_ConvertedValue);
//	printf("The current AD value =%f V\n",ADC_ConvertedValueLocal);
//		
//	delay_ms(1000);
//    }
//  return 0;	
//}
