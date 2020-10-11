
#include "cmsis_os.h"
#include "stdio.h"

osMessageQDef(message_q, 5, uint32_t); // 定义 message queue
osMessageQId  messageID;   //定义 messageID
osEvent messageEvent;      //存放message信息
//函数原型申明
void Thread_Message(void const *arg);
//定义线程
osThreadDef(Thread_Message,osPriorityNormal,2,0);

//定义线程的函数原型
void Thread_Message(void const *arg)
{
	if(*(char *)arg == '1'){ //线程1部分，发送message
		uint32_t count;
		while(1){
			osMessagePut(messageID,count++,100);  //发送数据count且count自加1
			osDelay(1000);  //每秒钟发送一次数据
		}
	}else if(*(char *)arg == '2'){//线程2部分，接受message
		while(1){
		  messageEvent = osMessageGet(messageID,500); //接受数据
			if(messageEvent.status == osEventMessage){  //确定此事件是message到达事件
				printf("recieved data is:%d\r\n",messageEvent.value.v); //通过event.value.v读取消息的值
			}
		}
	}else{
		printf("arg error.\r\n"); //只支持'1'和'2'
	}
}///结束

int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	osThreadCreate(osThread(Thread_Message),"1"); //启动线程1
	osThreadCreate(osThread(Thread_Message),"2"); //启动线程2
	if((messageID = osMessageCreate(osMessageQ(message_q),NULL)) == NULL){
		 printf("create Message Queue failed!\r\n");
	}
	printf("create Message Queue OK!\r\n");
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
}
	
