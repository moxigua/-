
#include "cmsis_os.h"
#include "stdio.h"

//线程ID
osThreadId Thread_Receive_ID , Thread_Send_ID;

//接受Signal线程原型
void Thread_ReceiveSignal(void const *arg)
{
	while(1){
		osEvent even ;
		printf("-> waiting for signal(thread_rec).\r\n");
		even = osSignalWait(0x01,osWaitForever);//等待指定的信号值
		switch(even.status){
			case osOK: //等待时间为0返回osOK
				printf("  osOK.\r\n");
				break;
			case osEventTimeout://在超时时间内没有收到指定信号
				printf("  osEventTimeout.\r\n");
				break;
			case osEventSignal: //收到了指定的信号
				printf("  osEventSignal--%d.\r\n",even.value.signals);
				break;
			case osErrorValue:
				printf("  osErrorValue.\r\n");
				break;
			case osErrorISR:
				printf("  osErrorISR.\r\n");
				break;
			default:
				
				  break;
		}
	}
}///结束

//发送Signal线程原型
void Thread_SendSignal(void const *arg)
{
	  while(1){
			osSignalSet(Thread_Receive_ID,0x01); //想接受线程发送0x01的信号值
			printf("-> set the signal(thread_send).\r\n");
			osDelay(2000); //两秒钟发送一次
		}
} 	

//定义线程Thread_ReceiveSignal与 Thread_SendSignal
osThreadDef(Thread_ReceiveSignal,osPriorityNormal,1,0);
osThreadDef(Thread_SendSignal,osPriorityNormal,1,0);

int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
	Thread_Receive_ID = osThreadCreate(osThread(Thread_ReceiveSignal),NULL); //创建接受Signal线程
	osThreadCreate(osThread(Thread_SendSignal),NULL); //创建发送Signal线程
}
	
