
#include "cmsis_os.h"
#include "stdio.h"

/*单次定时器相关定义*/
void onc_timer_callback(void const *argument);

osTimerDef(once_timer,onc_timer_callback);	//定义单次定时器
osTimerId  once_timer_ID;    //单次定时器ID

//单次定时器回掉函数
void onc_timer_callback(void const *argument)
{
   printf("thread1 print is onc_timer,3s.\r\n");
}


/*周期定时器相关定义*/
void periodic_timer_callback(void const *argument);

osTimerDef(periodic_timer,periodic_timer_callback);//定义周期定时器
osTimerId  periodic_timer_ID;  //周期定时器ID

//周期定时器回掉函数
void periodic_timer_callback(void const *argument)
{
   printf("thread2 print is periodic_timer,1s.\r\n");
}


int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口  
	//创建定时器1，单次定时器，不传递参数
	if((once_timer_ID = osTimerCreate(osTimer(once_timer),osTimerOnce,NULL))==NULL)
		printf("timer1 create failed!\r\n");
	printf("timer1 create OK!\r\n");
	//创建定时器2，周期定时器，不传递参数
	if((periodic_timer_ID = osTimerCreate(osTimer(periodic_timer),osTimerPeriodic,NULL)) == NULL)
		printf("timer2 create failed!\r\n");
	printf("timer2 create OK!\r\n");
	osTimerStart(once_timer_ID,3000);//启动单次定时器，周期为3s
	osTimerStart(periodic_timer_ID,1000);//启动周期定时器，周期为1s
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
}
	
