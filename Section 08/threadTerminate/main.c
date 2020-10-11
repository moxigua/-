
#include "cmsis_os.h"
#include "stdio.h"

//定义线程ID号
osThreadId  thread1_ID,thread2_ID;

//定义线程的函数原型
void Thread_Print(void const *arg)
{
	if(*(char *)arg == '1'){//线程1运行
	  while(1){
			printf("thread 1 running...\r\n");
			osDelay(100); //延时100ms执行一次
		}
	}else if(*(char *)arg == '2'){//线程2运行
	  if(osThreadTerminate(thread1_ID) == osOK){
			printf("terminate thread 1 success.\r\n");
		}
	}
}///结束

//定义线程
osThreadDef(Thread_Print,osPriorityNormal,2,0);

int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	thread1_ID=osThreadCreate(osThread(Thread_Print),"1");//创建thread1 
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
	osDelay(1000);        //1s后终止线程1
	thread2_ID=osThreadCreate(osThread(Thread_Print),"2");//创建thread2
}
	
