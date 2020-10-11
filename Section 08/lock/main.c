
#include "cmsis_os.h"
#include "stdio.h"

//定义Mutex
osMutexDef(lock);
//MutexID
osMutexId MutexID;
//函数原型申明
void Thread_Mutex(void const *arg);
//定义线程
osThreadDef(Thread_Mutex,osPriorityNormal,2,0);

//定义线程的函数原型
void Thread_Mutex(void const *arg)
{
	if(*(char *)arg == '1'){ //线程1部分
		printf("thread 1 attemp to obtain the Mutex......\r\n");
		if(osMutexWait(MutexID,osWaitForever)) //线程1尝试获取Mutex
			printf("thread 1 got Mutex.\r\n");
		printf("thread 1 got Mutex success!\r\n");
		while(1);                              //线程1一直占用线程Mutex
	}else if(*(char *)arg == '2'){//线程2部分
		while(1){
			int ret;
			printf("%d",osMutexRelease(MutexID));
			printf("thread 2 attemp to obtain the Mutex......\r\n");
			ret = osMutexWait(MutexID,3000);	//线程2尝试获取Mutex，超时时间3s
			if(ret == osOK)  //线程2获取Mutex成功
				printf("thread 2 got Mutes.\r\n");
			else if(ret == osErrorTimeoutResource) //线程2获取Mutex超时
				printf("thread 2 obtain Mutex timeout.\r\n");
			else                                   //线程2获取Mutex其它错误
				printf("thread 2 obtain Mutex occured errors.\r\n");
		}
	}else {
		printf("arg error.\r\n");  //只支持'1','2'两个参数来启动两个线程
	}
}///结束

int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	osThreadCreate(osThread(Thread_Mutex),"1"); //创建线程输出a
	osThreadCreate(osThread(Thread_Mutex),"2"); //创建线程输出b
	MutexID = osMutexCreate(osMutex(lock));     //创建Mutex
	if(MutexID == NULL)
		printf("Mutex Create failed.\r\n");       //创建Mutex 失败
	printf("Mutex Create success.\r\n");        //创建Mutex 成功
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
	
	
}
	
