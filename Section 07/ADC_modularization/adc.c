
/**************  以下为【adc.h】文件中的内容  **********************/
#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

extern uint16_t Get_Adc(uint8_t ch);
extern void ADC_Inti(void);

#endif

/**************  以下为【adc.c】文件中的内容  **********************/
#include "adc.h"

void ADC_RCC_Init(void)
{ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
	RCC_APB2Periph_ADC1 , ENABLE );   //使能ADC1 通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置ADC 分频因子6,72M/6=12,ADC 最大时间不能超过14M
}

void ADC_GPIO_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;//PA1 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA1
}
 
void ADC_Mode_Config(void)
{ 
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1); //复位ADC1
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 		//单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的ADC 通道的数目
	ADC_Init(ADC1, &ADC_InitStructure); 	 //根据指定的参数初始化外设ADCx
	ADC_Cmd(ADC1, ENABLE); //使能指定的ADC1
	ADC_ResetCalibration(ADC1); 					//开启复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); 	//等待复位校准结束
	ADC_StartCalibration(ADC1); 					//开启AD 校准
	while(ADC_GetCalibrationStatus(ADC1)); 			//等待校准结束
}

uint16_t Get_Adc(uint8_t ch)
{
	//设置指定ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );//通道1,规则采样顺序值为1,采样时间为239.5 周期
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能指定的ADC1 的软件转换功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1); 	 //返回最近一次ADC1 规则组的转换结果
}

void ADC_Inti()
{ 
	ADC_RCC_Init();    	// 初始化RCC
	ADC_GPIO_Init();   	// 初始化GPIO
	ADC_Mode_Config();	// 初始化ADC模式
}
