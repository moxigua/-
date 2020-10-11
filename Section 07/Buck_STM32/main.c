
#include "stm32f10x.h" //使用STM32标准函数库需要添加此头文件
#include "stdio.h"
#include "adc.h"
#include "pid.h"
#include "pwm.h"
#include "stdout_USART.h"


void delay_us(u32 i)
{
    u32 temp;
    SysTick->LOAD=9*i;         //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;         //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;                //清零计数器
    do
    {
        temp=SysTick->CTRL;           //读取当前倒计数值
    }
    while((temp&0x01)&&(!(temp&(1<<16))));     //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}


int main(void)
{
	int count =0, i = 0; // 临时变量
	PID_TypeDef pid_voltage; 	// 电压环PID控制器
	PID_TypeDef pid_current; 	// 电流环PID控制器
	ADC_V_I adc_v_i;		// 采集结果
	
	float VoutREF = 5.0; 	// 输出电压的参考值
	float PWM_Duty = 0; 	// 控制PWM
	
	stdout_init();  //  初始化printf()函数
	MY_PWM_Init();
	ADC_Inti();
	 
	pid_voltage.T = 250; 		// 250us
	pid_voltage.Kp = 0.2;
	pid_voltage.Ti = 2000;
	pid_voltage.Td = 0;
	pid_voltage.OutMax = 5;		// 限制最大增幅
	pid_voltage.OutMin = -4;	// 限制最小增幅
	PID_init(&pid_voltage);

	pid_current.T = 50;			// 50us
	pid_current.Kp = 13;
	pid_current.Ti = 90;
	pid_current.Td = 0;
	pid_current.OutMax = 0.95*PWM_Period;  // 最大更改为95%个周期
	pid_current.OutMin = 0.05*PWM_Period;  // 最小增幅为5%个周期
	PID_init(&pid_current);

//	count = 0;  
//	while(1)
//	{
//		adc_v_i = GetOutVI();   	// 获取电压与电流
//		if (count %5 == 0)				// 外环是内环频率的1/5
//		{	// 电压环调节环调节
//			PID_Calc( &pid_voltage, adc_v_i.V, VoutREF);
//		}
//		PWM_Duty = PID_Calc( &pid_current, adc_v_i.I, pid_voltage.Output);
//		Update_PWM(PWM_Duty);
//		delay_us(50);             // 内环采样周期为 50us
//		if (i % 50 == 0)
//		{  
//			// 过流等保护程序 
//		}
//		count ++;
//	}
		PWM_Duty = 100;
		Update_PWM(PWM_Duty);
}




