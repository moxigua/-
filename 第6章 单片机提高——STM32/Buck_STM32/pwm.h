#ifndef __PWM_H__
#define __PWM_H__

extern int PWM_Period;

extern void MY_PWM_Init(void);
extern void Update_PWM(int d);
extern void Stop_PWM(void);
extern void Restart_PWM(int d);

#endif

