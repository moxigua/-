
/**************  以下为【adc.h】文件中的内容  **********************/
#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

extern uint16_t Get_Adc(uint8_t ch);
extern void ADC_Inti(void);

extern volatile uint16_t ADCConvertedValue[10][3];

#endif
