#ifndef __PID_H__
#define __PID_H__


typedef struct struct_PID{   
	float SetPoint;     //输入：给定值
	float Kp;  			//比例常数
	float Ti;  			//积分时间
	float Td;  			//微分时间
	float T;  			//采样时间
	float a0;  			//系数1：a0 = Kp(1 + T/Ti + Td/T)
	float a1;  			//系数2: a1 = Kp(1 + 2Td/T)
	float a2;  			//系数3：a2 = Kp*Td/T
	float Ek;    		//当前误差
	float Ek_1;  		//前一次误差
	float Ek_2;  		//第二次误差
	float Output;  		//输出值
	float Last_Output; //上一次输出值
	float Increment;   //增量值
	float OutMax;      //输出限制最大值
	float OutMin;      //输出限制最小值
}PID_TypeDef;


extern void PID_init ( PID_TypeDef *p);
extern void PID_clear(PID_TypeDef *p);
extern float PID_Calc( PID_TypeDef *p, float feedback, float ref);

#endif
