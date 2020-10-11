#include <string.h>
#include "W5500_functions.h"
#include "stm32f10x.h"
#include "socket.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "DHCP/DHCP.h"

#define Debug

/***********************************W5500默认配置*********************************/
/*定义W5500的配置字*/
wiz_NetInfo ConfigMsg;

//接受数据缓存
uint8_t buffer[2048];												// 定义一个2KB的数组，用来存放Socket的通信数据

/*是否需要DHCP*/
//#define GetIPFromDHCP 

/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
uint8_t macAddress[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*定义默认IP信息*/
uint8_t local_ip[4]={192,168,1,99};												/*定义W5500默认IP地址*/
uint8_t subnet[4]={255,255,255,0};												/*定义W5500默认子网掩码*/
uint8_t gateway[4]={192,168,1,1};													/*定义W5500默认网关*/
uint8_t dns_server[4]={114,114,114,114};									/*定义W5500默认DNS*/

uint8_t server_tcp_ip[4]={192,168,1,111};												/*定义W5500 TCP服务器IP地址*/
uint32_t server_tcp_port = 5000 ;												/*定义W5500 tcp服务器端口*/
uint16_t local_port=5942;	                       					/*定义本地端口*/

/***********************************SPI的配置*********************************/ 
//PG15是复位引脚
#define SPI_W5500_Port  GPIOG
#define W5500_Reset_Pin   GPIO_Pin_15
//PG8是中断输入引脚
#define W5500_INT_Pin  GPIO_Pin_8

//定义SPI驱动
extern ARM_DRIVER_SPI Driver_SPI1;
ARM_DRIVER_SPI* SPIdrv;

/***********************************DHCP的定义*********************************/
#define SOCK_DHCP			7          //使用Socket7 进行DHCP的开发
#define MY_MAX_DHCP_RETRY	3      

uint8_t dhcp_ok=0;																				/*dhcp成功获取IP*/

//定义一个1s的定时器，供DHCP使用
osTimerId DHCP_TimerID;
void DHCP_Timer_Function(void const *arg);
osTimerDef(DHCP_Timer,DHCP_Timer_Function);

/***********************************W5500回掉函数*********************************/
osSemaphoreDef (SampleSemaphore);            // semaphore object
static osSemaphoreId sid_Thread_Semaphore;   // semaphore id
//初始化锁
int Init_Lock (void) {

  sid_Thread_Semaphore = osSemaphoreCreate (osSemaphore(SampleSemaphore), 1);
  if (!sid_Thread_Semaphore) {
    ; // Semaphore object not created, handle failure
  }
  return(0);
}

void W5500_Lock(void)
{
    osSemaphoreWait (sid_Thread_Semaphore, osWaitForever);  
}

void W5500_Unlock(void)
{
	  osSemaphoreRelease(sid_Thread_Semaphore);
}
//W5500片选信号拉低
void W5500_CS_Enable(void)
{
	 /* SS line = ACTIVE = LOW */
   SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);
}
//W5500片选信号拉高
void W5500_CS_Disable(void)
{
	/* SS line = INACTIVE = HIGH */
  SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);
}
//W5500发送一个字节
void W5500_SPI_sendByte(uint8_t wb)
{
	uint8_t b;
	SPIdrv->Transfer(&wb,&b,1);
	while(SPIdrv->GetStatus().busy);
}
//W5500接受一个字节
uint8_t W5500_SPI_recByte(void)
{
	uint8_t a = 0,b;
	SPIdrv->Transfer(&a,&b,1);
	while(SPIdrv->GetStatus().busy);
	return b;
}
//W5500复位
void W5500_Reset(void)
{
	GPIO_ResetBits(SPI_W5500_Port,W5500_Reset_Pin);
	osDelay(100);
	GPIO_SetBits(SPI_W5500_Port,W5500_Reset_Pin);
	osDelay(100);
}
//W5500外设的配置
void W5500_GPIO_Config(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);
	//配置W5500 Reset
	GPIO_InitStructure.GPIO_Pin = W5500_Reset_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_W5500_Port,&GPIO_InitStructure);
	//配置W5500 INT
	GPIO_InitStructure.GPIO_Pin = W5500_INT_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(SPI_W5500_Port,&GPIO_InitStructure);
	//配置SPI
	 SPIdrv = &Driver_SPI1;
	// Initialize the SPI driver 
	SPIdrv->Initialize(NULL);
	// Power up the SPI peripheral 
	SPIdrv->PowerControl(ARM_POWER_FULL);
	// Configure the SPI to Master, 8-bit mode @10000 kBits/sec 
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL0_CPHA0 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 36000000);
}
void W5500_FuctionRegist(void)
{
	//初始化锁
	Init_Lock();
	//进入和退出临界状态
	reg_wizchip_cris_cbfunc(W5500_Lock,W5500_Unlock);
	//配置片选信号的回掉函数
	reg_wizchip_cs_cbfunc(W5500_CS_Enable,W5500_CS_Disable);
	//配置SPI读写的回掉函数
	reg_wizchip_spi_cbfunc( W5500_SPI_recByte ,W5500_SPI_sendByte);    
}
/*W5500初始化配置*/
void W5500_Ini(void)
{
	uint8_t tmp;	
	uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
	//把默认的配置 MAC 地址 和IP 地址之类的 赋值到ConfigMsg这个结构体中
	memcpy(ConfigMsg.mac, macAddress, 6);
	memcpy(ConfigMsg.ip,local_ip,4);
	memcpy(ConfigMsg.sn,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);	
	//配置W5500中socket的缓存区的大小
	if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
	{
		 printf("WIZCHIP Initialized fail.\r\n");
		 while(1);
	}
	//等待网线连接好了
	do
	{
		 if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
				printf("Unknown PHY Link stauts.\r\n");
	}while(tmp == PHY_LINK_OFF);
	//配置W5500
	ctlnetwork(CN_SET_NETINFO, (void*)&ConfigMsg);
	#ifdef Debug
		printf("\r\n=== NET CONF ===\r\n");
		printf("  ip is :%d.%d.%d.%d\r\n",ConfigMsg.ip[0],ConfigMsg.ip[1],ConfigMsg.ip[2],ConfigMsg.ip[3] );
		printf("  gw is :%d.%d.%d.%d\r\n",ConfigMsg.gw[0],ConfigMsg.gw[1],ConfigMsg.gw[2],ConfigMsg.gw[3] );
		printf("  mac is :%d.%d.%d.%d.%d.%d\r\n",ConfigMsg.mac[0],ConfigMsg.mac[1],ConfigMsg.mac[2],ConfigMsg.mac[3],ConfigMsg.mac[4],ConfigMsg.mac[5] );
	#endif
}


/*************************************

DHCP测试，要进行这个测试

**********************************************/
void DHCP_Timer_Function(void const *arg)
{
	DHCP_time_handler(); //触发DHCP里面的一个函数,时间计数器+1
}

 
void W5500_DHCP_Config(void)
{	
	uint8_t tmpstr[5] = {0};
  #ifdef Debug
	printf("use ip from DHCP.");
	#endif
	ctlnetwork(CN_SET_NETINFO, (void*)&ConfigMsg);
	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	#ifdef Debug
		ctlnetwork(CN_GET_NETINFO, (void*)&ConfigMsg);
		printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
		printf("  ip is :%d.%d.%d.%d\r\n",ConfigMsg.ip[0],ConfigMsg.ip[1],ConfigMsg.ip[2],ConfigMsg.ip[3] );
		printf("  gw is :%d.%d.%d.%d\r\n",ConfigMsg.gw[0],ConfigMsg.gw[1],ConfigMsg.gw[2],ConfigMsg.gw[3] );
		printf("  mac is :%d.%d.%d.%d.%d.%d\r\n",ConfigMsg.mac[0],ConfigMsg.mac[1],ConfigMsg.mac[2],ConfigMsg.mac[3],ConfigMsg.mac[4],ConfigMsg.mac[5] );
	#endif
	//wizchip_init(txsize,rxsize);
}

void my_ip_assign(void)
{
   getIPfromDHCP(ConfigMsg.ip);
   getGWfromDHCP(ConfigMsg.gw);
   getSNfromDHCP(ConfigMsg.sn);
   getDNSfromDHCP(ConfigMsg.dns);
   ConfigMsg.dhcp = NETINFO_DHCP;
   /* Network initialization */
   W5500_DHCP_Config();      // apply from dhcp
	//DHCP分配的地址是有时间限制的，所以如果DHCP过去需要重新的分配
#ifdef Debug
   printf("DHCP LEASED TIME : %ld Sec.\r\n", getDHCPLeasetime());
#endif
}

void my_ip_conflict(void)
{
#ifdef Dubug
	printf("CONFLICT IP from DHCP\r\n");
#endif
   //halt or reset or any...
   while(1); // this example is halt.
}
 
void W5500_DHCP_Timer(void)
{
	 osStatus status;                                              // function return status
	DHCP_TimerID = osTimerCreate(osTimer(DHCP_Timer), osTimerPeriodic,NULL); 
	//每秒钟触发一次
	if(DHCP_TimerID != NULL)
	{
		status = osTimerStart(DHCP_TimerID,1000);              // function return status
		if(status != osOK)
		{
			#ifdef Debug
			printf("osTimer Started failed.");
			#endif
		}
	}
	else
	{
		 #ifdef Debug
			printf("osTimer Started failed.");
			#endif
	}
}	

void W5500_DHCP_Init(void)
{
	//初始化DHCP时间基准
	W5500_DHCP_Timer();
	//在使用DHCP服务之前得设置mac地址
	setSHAR(macAddress);
  //初始化使用DHCP
	DHCP_init(SOCK_DHCP, buffer);
	//注册回掉函数，有ip分配和更新回掉函数，ip冲突回掉函数
	//这里ip分配和更新都调用 my_ip_assign 函数
	reg_dhcp_cbfunc(my_ip_assign, my_ip_assign, my_ip_conflict);
}

void W5500_DHCP_Loop(void)
{
  uint8_t my_dhcp_retry = 0;
	while(1)
	{
		switch(DHCP_run())
		{
			case DHCP_IP_ASSIGN:
			case DHCP_IP_CHANGED:
				break;
			case DHCP_IP_LEASED:
				//说明IP地址已经以使用了
				dhcp_ok = 1;
				break;
			case DHCP_FAILED:
				dhcp_ok = 0;
				my_dhcp_retry++;
				if(my_dhcp_retry > MY_MAX_DHCP_RETRY)
				{
					#ifdef Debug
					printf(">> DHCP %d Failed\r\n", my_dhcp_retry);
					#endif
					my_dhcp_retry = 0;
					DHCP_stop();   
					DHCP_init(SOCK_DHCP, buffer);   //DHCP获取失败则重新初始化
				}
				break;
			default:
				break;
		}
		osDelay(500);
	} // end of Main loop
}

