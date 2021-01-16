
#include "cmsis_os.h"
#include "stdio.h"

osSemaphoreDef(my_semaphore);//定义semaphore
osSemaphoreId my_semaphore_ID; //semaphoreID
//函数原型申明
void Thread_Semaphore(void const *arg);
//定义线程
osThreadDef(Thread_Semaphore,osPriorityNormal,3,0);

//定义线程的函数原型
void Thread_Semaphore(void const *arg)
{
	if(*(char *)arg == '1'){ //线程1部分
		int semaphore_count=0; //smeaphore令牌总数
		/*thread1中获取两个semaphore不释放*/
		if((semaphore_count = osSemaphoreWait(my_semaphore_ID,osWaitForever)) != -1)   
			printf("thread1 get semaphore success.count of semaphore:%d\r\n",semaphore_count);  
		if((semaphore_count = osSemaphoreWait(my_semaphore_ID,osWaitForever)) != -1)
			printf("thread1 get semaphore success.count of semaphore:%d\r\n",semaphore_count);
		while(1)
			osThreadYield(); //切换至同优先级其它线程运行
	}else if(*(char *)arg == '2'){//线程2部分
		int semaphore_count=0;  //smeaphore令牌总数
		/*thread1中获取一个semaphore不释放*/
		if((semaphore_count = osSemaphoreWait(my_semaphore_ID,osWaitForever)) != -1)
			printf("thread2 get semaphore success.count of semaphore:%d\r\n",semaphore_count);
		while(1)
			osThreadYield(); //切换至同优先级其它线程运行
	}else if(*(char *)arg == '3'){//线程3部分
		int semaphore_count=0; //smeaphore令牌总数
		osDelay(100); //等待thread1和thread2获取semaphore
    if((semaphore_count = osSemaphoreWait(my_semaphore_ID,3000)) != -1){ //尝试获取semaphore
				uint8_t i=0;
				printf("thread3 get semaphore failed!count of semaphore:%d\r\n",semaphore_count);
				for(i=0;i<6;i++){
				if(osSemaphoreRelease(my_semaphore_ID) == osOK)//尝试释放6个semaphore
					printf("thread3 releases semaphore success.\r\n");
				}	
				if((semaphore_count =osSemaphoreWait(my_semaphore_ID,3000)) != -1) //再尝试获取semaphore
					printf("thread3 get semaphore success.count of semaphore:%d\r\n",semaphore_count);
				else
					printf("thread3 get semaphore failed!count of semaphore:%d\r\n",semaphore_count);
		}
		else{                      //获取semaphore失败
		    printf("thread3 get semaphore failed!count of semaphore:%d\r\n",semaphore_count);
		}
		while(1); //thread3停止在这
	}else{
		printf("arg error.\r\n");
	}
}///结束

int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	osThreadCreate(osThread(Thread_Semaphore),"1"); //创建线程输出a
	osThreadCreate(osThread(Thread_Semaphore),"2"); //创建线程输出b
	osThreadCreate(osThread(Thread_Semaphore),"3"); //创建线程输出b
	if((my_semaphore_ID = osSemaphoreCreate(osSemaphore(my_semaphore),3)) == NULL){
		 printf("create semaphore failed!\r\n");
	}
	printf("create semaphore OK!\r\n");
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
}
	
