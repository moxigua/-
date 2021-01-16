#include "cmsis_os.h"
#include "loopback.h"
#include "W5500_functions.h"
#include "main_threads.h"
#include "RTC/rtc.h"
#include "http.h"
#include "stdio.h"
#include "string.h"


void  DHCPThread(void const *arg);
osThreadDef(DHCPThread,osPriorityNormal,1,0);

void DHCPThread(void const *arg)
{
	while(1)
	{
		W5500_DHCP_Loop();
	}
}

void W5500_Setup(void)
{
	W5500_GPIO_Config();    //外设初始化
  W5500_FuctionRegist();  //注册回调函数
	W5500_Reset();          //复位W5500
	W5500_Ini();						//配置W5500的网络信息
}

char message[128] = {0};

int main(void)
{ 
	osKernelInitialize();//初始化CMSIS_OS
	stdout_init(); //初始化调试串口
	RTC_Init();    //初始化RTC实时时钟
	ButtonThread_Inti();    //Button按钮读取线程
	W5500_Setup();  //配置W5500
	W5500_DHCP_Init();      //初始化DHCP
	osThreadCreate(osThread(DHCPThread),NULL);  //DHCP循环线程
	DNS_Timer_Init();				//DNS定时器
	
	osKernelStart();        //CMSIS_OS启动
	while(!dhcp_ok);				//等待DHCP完成
	
	while(1)
	{
		
		printf("start to post data.\r\n");
		
		//{"id":id,"status":"close"}
		sprintf(message,"{\"air_temp\":%f,\"air_hum\":%f,\"soil_temp\":%f,\"soil_hum\":%f,\"light\":%d}",23.2,45.0, 22.0,86.0,1000);
		
		getHttpString(buffer, "192.168.1.106:5000", message );
		
		printf(buffer);
		SendHTTPString(6, buffer, strlen(buffer));
		
		printf("end to post data.\r\n");
		osDelay(3000);
	}
}


