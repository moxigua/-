#include "pid.h"



// PID的数据初始化程序：
void PID_init ( PID_TypeDef *p)
{   
	// 计算PID公式中的参数
	p->a0 = p->Kp*(1 + 1.0*p->T/p->Ti + 1.0*p->Td/p->T);
	p->a1 = p->Kp*(1 + 2.0*p->Td/p->T);
	p->a2 = 1.0*p->Kp*p->Td/p->T;
}

void PID_clear(PID_TypeDef *p)
{ 
	// 清除误差
	p->Ek_2 = 0;
	p->Ek_1 = 0;               
	p->Last_Output = 0;
}

float PID_Calc( PID_TypeDef *p, float feedback, float ref)
{ 
	// 增量式PID算法：
	p->Ek = ref - feedback;             
	p->Increment = (p->a0*p->Ek - p->a1*p->Ek_1 + p->a2*p->Ek_2 ); 
	p->Output = p->Last_Output + p->Increment;

	if(p->Output > p->OutMax)p->Output = p->OutMax;  // 限制PWM最大值
	if(p->Output < p->OutMin)p->Output = p->OutMin;  // 限制PWM的最小值
	p->Ek_2 = p->Ek_1;
	p->Ek_1 = p->Ek;               
	p->Last_Output = p->Output;
	return p->Output;
}

