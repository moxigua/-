
#include "cmsis_os.h"
#include "stdio.h"
#include "math.h"

/*Memory Pool相关申明*/
typedef struct {    //定义在Memory Pool中存放的内存单元(对象)
float voltage;      //采集的电压值
float current;      //采集的电流值
uint32_t counter;        //采集的次数
}elec_message;

osPoolDef(elecMessagePool,16,elec_message);   //定义Memory Pool
osPoolId  poolID;    //定义Memory Pool 的ID

/*Message Queue相关申明*/
osMessageQDef(message_q, 5, elec_message); // 定义 message queue
osMessageQId  messageID;   //定义 messageID
osEvent messageEvent;      //存放message信息


/*线程的相关定义*/
//函数原型申明
void Thread_Message(void const *arg);
//定义线程
osThreadDef(Thread_Message,osPriorityNormal,2,0);

//线程的函数原型
void Thread_Message(void const *arg)
{
	if(*(char *)arg == '1'){ //线程1部分，发送message
		uint32_t count = 1;
		while(1){
			elec_message message1;
			elec_message message2;
			message1 = *(elec_message *)osPoolAlloc(poolID);	
			message1.counter = count++;  //采集的次数
			message1.voltage = 100*sin(count);      //电压为sin
			message1.current = 100*cos(count);      //电流为cos
			osMessagePut(messageID,(uint32_t)&message1,osWaitForever);//发送数据count且count自加1  
			message2 = *(elec_message *)osPoolAlloc(poolID);	
			message2.counter = count++;  //采集的次数
			message2.voltage = 100*sin(count);      //电压为sin
			message2.current = 100*cos(count);      //电流为cos
			osMessagePut(messageID,(uint32_t)&message2,osWaitForever);//发送数据count且count自加1
			osDelay(1000);  //每秒钟发送一次数据
		}
	}else if(*(char *)arg == '2'){//线程2部分，接受message
		elec_message message;
		while(1){
		  messageEvent = osMessageGet(messageID,osWaitForever); //接受数据
			if(messageEvent.status == osEventMessage){  //确定此事件是message到达事件
				message = *(elec_message *)messageEvent.value.p;
				printf("recieved message-->\r\n  ");
				printf("count:%d,voltage:%f V,current:%f A.\r\n",message.counter,message.voltage,message.current);
				osPoolFree(poolID,&message);
			}
		}
	}else{
		printf("arg error.\r\n"); //只支持'1'和'2'
	}
}///结束

/*主函数，主要初始化CMSIS，Memory Pool和Message Queue*/
int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	osThreadCreate(osThread(Thread_Message),"1"); //启动线程1
	osThreadCreate(osThread(Thread_Message),"2"); //启动线程2
	if((messageID = osMessageCreate(osMessageQ(message_q),NULL)) == NULL){
		 printf("create Message Queue failed!\r\n"); //创建Message Queue失败
	}
	printf("create Message Queue OK!\r\n"); //创建Message Queue成功
	if(osPoolCreate(osPool(elecMessagePool)) == NULL){  //创建Memory Pool
		 printf("create Memoey Pool failed!\r\n"); //创建Memoey Pool失败
	}
	printf("create Memoey Pool OK!\r\n"); //创建Memoey Pool成功
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
}
	
