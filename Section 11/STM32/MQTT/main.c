#include "cmsis_os.h"
#include "rtc.h"
#include "stdio.h"
#include <wolfssl/wolfcrypt/md5.h>
#include "MQTTClient.h"
#include "W5500_functions.h"
#include "W5500_MQTT_Functions.h"
#include "Output/led.h"
#include "main_threads.h"

#define adp_type   "thingidp"
#define IoTCoreID  "asyfdev"   // 申请时随机生成
#define DeviceKey  "ForBooks"
//#define DeviceKey  "ssl_test"
#define DeviceSecret "qvEOnrVOnuRcUAtN"
#define timestamp   "0"
#define algorithm_type   "MD5"

#define getUserName() adp_type"@"IoTCoreID"|"DeviceKey"|" timestamp "|" algorithm_type
#define BrokerCommonPart ".iot.gz.baidubce.com"
#define getBrokerAddress() IoTCoreID""BrokerCommonPart
#define getPWDString() DeviceKey"&"timestamp"&"algorithm_type""DeviceSecret

#define MD5_MAX_Length  250

int getPassword(char * array)
{
	wc_Md5 myMD5;   // md5结构体
	int ret =0,i;
	byte *p;
	char PWMString[1024] = {0};
	sprintf(PWMString, "%s",getPWDString());
	p = malloc(MD5_MAX_Length);
	if(p == NULL){
		return -1;
	}
	if ((ret = wc_InitMd5(&myMD5)) != 0) {
   printf("wc_Initmd5 failed");
		return -1;
	}
	ret = wc_Md5Update(&myMD5, PWMString, strlen(PWMString));
	//	ret = wc_Md5Update(&myMD5, "ForBooks&0&MD5qvEOnrVOnuRcUAtN", strlen(getPWDString()));
	if (ret != 0) {
		printf("Md5 Update Error Case\r\n");
	} 
	ret = wc_Md5Final(&myMD5, p);
	if (ret != 0) {
		printf("Md5 Final Error Case\r\n");
	}
	//*array = '\0';
	for(i=0;i<MD5_MAX_Length;i++)
	{
		char temp[2] = {0};
		if (p[i] == '\0')
			break;
		sprintf(temp,"%02x",p[i]);
		strcat(array,temp);
	}
	wc_Md5Free(&myMD5);
	free(p);
	return ret;
}







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

int main(void)
{
	char pwd[100] = {0};
	osKernelInitialize(); //初始化CMSIS_OS
	RTC_Init();
	RTC_Set(2020,10,06,15,19,10);
	stdout_init(); 				//初始化调试串口

	// 获取连接信息
	printf(" MD5 test \r\n");
	printf("%s\r\n",getBrokerAddress());
	printf("%s\r\n",getUserName());
	printf("%s\r\n",getPWDString());
	getPassword(pwd);
	printf("%s\r\n",pwd);
	
	/***********   MQTT相关  ****************/
	W5500_Setup();   //初始化W5500
	Init_MQTT_Lock();				//MQTT操作之间互锁
	Led_Init();
	ButtonThread_Inti();    //Button按钮读取线程
	//W5500_Ini();
	W5500_DHCP_Init();      //初始化DHCP
	RTC_Init();
	//设置RTC时间，证书是有有效期的
	RTC_Set(2020,10,05,18,9,45);

	osThreadCreate(osThread(DHCPThread),NULL);  //DHCP循环线程
	DNS_Timer_Init();				//DNS定时器
	osKernelStart();        //CMSIS_OS启动
	//while(1);

	while(!dhcp_ok);				//等待DHCP完成
	//MQTTClient初始化
	MQTTClient_Init();

	if(!MQTTClient_Connect_With_Name(getBrokerAddress(),getUserName(),pwd)){
		IsConnnected = 1;
	}
	else{
		IsConnnected = 0;
	}
	while(1)
	{
		if(IsConnnected)
		{
			MQTTclient_Subscribe(DEFAULT_SUBSCRIBE_TOPIC,MQTT_QOS_0);
			MQTTClient_Publish(DEFAULT_PUBLIC_TOPIC,MQTT_QOS_0, "Current temperature is 23.", strlen("Current temperature is 23."));
		  MQTTclient_ReadLoop();	
			
		}
		else
		{
			osDelay(300);
		} 
	}
}



//void  DHCPThread(void const *arg);
//osThreadDef(DHCPThread,osPriorityNormal,1,0);

//void DHCPThread(void const *arg)
//{
//	while(1)
//	{
//		W5500_DHCP_Loop();
//	}
//}

//void W5500_Setup(void)
//{
//	W5500_GPIO_Config();     //外设初始化
//  W5500_FuctionRegist();   //注册回调函数
//	W5500_Reset();           //复位W5500
//	W5500_Ini();						 //配置W5500的网络信息
//}

//int main(void)
//{
//	RTC_Init();
//	RTC_Set(2020,10,06,15,19,10);
//	osKernelInitialize(); //初始化CMSIS_OS
//	stdout_init(); 				//初始化调试串口
//	W5500_Setup(); 				//W5500初始化
//	W5500_DHCP_Init();  	//初始化DHCP
//	osThreadCreate(osThread(DHCPThread),NULL);  //DHCP循环线程
//	osKernelStart();      //CMSIS_OS启动
//	while(!dhcp_ok);			//等待DHCP完成
//	while(1)
//	{
//		loopback_tcps(1,buffer,8086); //TcpServer进行测试
//	}
//}

