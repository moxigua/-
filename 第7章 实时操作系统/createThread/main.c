
#include "cmsis_os.h"
#include "stdio.h"

//定义线程的函数原型
void Thread_Print(void const *arg)
{
	uint32_t i;
	for(i=i;i<260;i++){
		printf("%s",(char *)arg);
	}
}///结束

//定义线程Thread_Print，
//线程等级是osPriorityNorma
//允许创建3个线程实例
//使用默认堆栈大小
osThreadDef(Thread_Print,osPriorityNormal,3,0);

int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	osThreadCreate(osThread(Thread_Print),"a"); //创建线程输出a
	osThreadCreate(osThread(Thread_Print),"b"); //创建线程输出b
	osThreadCreate(osThread(Thread_Print),"c"); //创建线程输出c 
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
}
	
