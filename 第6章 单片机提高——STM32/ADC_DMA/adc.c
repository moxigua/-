
#include "adc.h"

volatile uint16_t ADCConvertedValue[10][3];//用来存放ADC转换结果，也是DMA的目标地址,3通道，每通道采集10次后面取平均数

void ADC_RCC_Init(void)
{ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
	RCC_APB2Periph_ADC1 , ENABLE );   //使能ADC1 通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置ADC 分频因子6,72M/6=12,ADC 最大时间不能超过14M
}

void ADC_GPIO_Init(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 | GPIO_Pin_2 |GPIO_Pin_3 ;//PA1,2,3 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIOA1
}
 
void ADC_Mode_Config(void)
{ 
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1); //复位ADC1
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC 独立模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC 数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 3; //要转换的通道数目
	ADC_Init(ADC1, &ADC_InitStructure); 	 //根据指定的参数初始化外设ADCx
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);//通道一转换结果保存到ADCConvertedValue[0~10][0]
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,2,ADC_SampleTime_55Cycles5);//通道二转换结果保存到ADCConvertedValue[0~10][1]
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,3,ADC_SampleTime_55Cycles5);//通道三转换结果保存到ADCConvertedValue[0~10][2]
	
	
	ADC_DMACmd(ADC1,ENABLE); // 使能数据传输
	ADC_Cmd(ADC1, ENABLE); //使能指定的ADC1
	ADC_ResetCalibration(ADC1); 					//开启复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); 	//等待复位校准结束
	ADC_StartCalibration(ADC1); 					//开启AD 校准
	while(ADC_GetCalibrationStatus(ADC1)); 			//等待校准结束
}

void ADC_DMA_Config(void){
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);  // 开启DMA1的时钟
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//ADC外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue; //内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //方向(从外设到内存)
	DMA_InitStructure.DMA_BufferSize = 3*10; //传输内容的大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自加1
	DMA_InitStructure.DMA_PeripheralDataSize= 	DMA_PeripheralDataSize_HalfWord ; // 16 位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//16位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ; // DMA模式：循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High ; //优先级：高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //禁止内存到内存的传输
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //配置DMA1的1通道
	DMA_Cmd(DMA1_Channel1,ENABLE);
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
	ADC_DMA_Config();   // DMA 配置
	ADC_Mode_Config();	// 初始化ADC模式
}
