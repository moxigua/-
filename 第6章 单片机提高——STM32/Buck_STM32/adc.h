
/**************  以下为【adc.h】文件中的内容  **********************/
#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"


typedef struct ADC_V_I{
	float V;
	float I;
}ADC_V_I;



extern uint16_t Get_Adc(uint8_t ch);
extern void ADC_Inti(void);
extern ADC_V_I GetOutVI(void);

#endif
