

#ifndef __W5500_functions_H__
#define __W5500_functions_H__

#include "Driver_SPI.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "wizchip_conf.h" //包含配置文件
#include "DHCP/DHCP.h"
#include "DNS/DNS.h"
#include "SNTP/sntp.h"

#define Debug

//接受数据缓存
extern uint8_t buffer[2048];												// 定义一个2KB的数组，用来存放Socket的通信数据
//W5500配置结构体
extern wiz_NetInfo ConfigMsg;

/*定义MAC地址,如果多块W5500网络适配板在同一现场工作，请使用不同的MAC地址*/
extern uint8_t macAddress[6];

/*定义默认IP信息*/
extern uint8_t local_ip[4];							/*定义W5500默认IP地址*/
extern uint8_t subnet[4];								/*定义W5500默认子网掩码*/
extern uint8_t gateway[4];							/*定义W5500默认网关*/
extern uint8_t dns_server[4];						/*定义W5500默认DNS*/

extern uint8_t server_tcp_ip[4];				/*定义W5500 TCP服务器IP地址*/
extern uint32_t server_tcp_port;				/*定义W5500 tcp服务器端口*/
extern uint16_t local_port;	            /*定义本地端口*/



//W5500配置默认的IP地址 mac 之类的，供给没有DHCP服务的使用使用的
extern void W5500_Ini(void);
//W5500硬件复位
extern void W5500_Reset(void);
//stm32 GPIO的配置
extern void W5500_GPIO_Config(void);
//注册官方给的需要我们自己写的函数，例如spi的读写，片选信号之类的
extern void W5500_FuctionRegist(void);

/**********DHCP******************/
extern uint8_t dhcp_ok;
extern void W5500_DHCP_Timer(void);
extern void W5500_DHCP_Init(void);
extern void W5500_DHCP_Loop(void);

/***********DNS********************/
#define Default_SOCK_DNS			6          //使用Socket6 进行dns的开发
extern  int DNS_Timer_Init(void);
extern  int8_t DNS(uint8_t SOCK_DNS,  char *hostName, uint8_t *IP);
/*SNTP*/
#define SNTP_Socket 5  //默认使用W5500的Socket 6
extern uint8_t SNTP_Server[4];	//"cn.pool.ntp.org"
extern datetime NowDatetime;
extern uint64_t timeSeconds;

#endif
