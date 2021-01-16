
#include "cmsis_os.h"
#include "stdio.h"
#include "math.h"

/*Mail Queue相关申明*/
typedef struct {    //定义Mail Queue中存放的内存单元(对象)
float voltage;      //采集的电压值
float current;      //采集的电流值
uint32_t counter;   //采集的次数
}elec_message;

osMailQDef(mail,16,elec_message);    //定义Mail Queue
osMailQId mailID;                    //定义Mail Queue ID


/*线程的相关定义*/
//函数原型申明
void Thread_Message(void const *arg);
//定义线程
osThreadDef(Thread_Message,osPriorityNormal,2,0);

//线程的函数原型
void Thread_Message(void const *arg)
{
	if(*(char *)arg == '1'){ //线程1部分，发送Mail
		uint32_t count = 1;
		while(1){
			elec_message message1;  
			elec_message message2;
			message1 = *(elec_message *) osMailAlloc(mailID,100); //申请空间填写邮件信息
			message1.counter = count++;
			message1.voltage = 100 * sin(count);   
			message1.current = cos(count);
			osMailPut(mailID,&message1);   //发送邮件
			message2 = *(elec_message *) osMailAlloc(mailID,100);//申请空间填写邮件信息
			message2.counter = count++;
			message2.voltage = 100 * sin(count);
			message2.current = cos(count);
			osMailPut(mailID,&message2);  //发送邮件
			osDelay(1000);     //没秒钟发送一次
		}
	}else if(*(char *)arg == '2'){//线程2部分，接受Mail消息
		elec_message message;
		while(1){
		   osEvent event = osMailGet(mailID,500);  //等待邮件到达
			if(event.status == osEventMail){ 	//判断是否是邮件到达
				message = *(elec_message *)event.value.p; //将邮件中的消息内容赋值给message
				printf("receieved message-->\r\n  ");
				printf("count:%d,voltage:%f V,current:%f A.\r\n",message.counter,message.voltage,message.current);
			}
		}
	}else{
		printf("arg error.\r\n"); //只支持'1'和'2'
	}
}///结束

/*主函数，主要初始化CMSIS，Mail Queue*/
int main(void)             
{
	osKernelInitialize(); //初始化CMSIS-RTOS内核
	stdout_init();        //重定向printf函数到串口
	osThreadCreate(osThread(Thread_Message),"1"); //启动线程1
	osThreadCreate(osThread(Thread_Message),"2"); //启动线程2
	if((mailID = osMailCreate(osMailQ(mail),NULL)) == NULL)
		printf("create Mail Queue failed!\r\n");  
	printf("create Mail Queue OK!\r\n");
	osKernelStart();      //启动CMSIS-RTOS内核，开始线程调度
}
	
