

#ifndef __W5500_functions_H__
#define __W5500_functions_H__

#include "Driver_SPI.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "wizchip_conf.h" //包含配置文件

//接受数据缓存
extern uint8_t buffer[2048];												// 定义一个2KB的数组，用来存放Socket的通信数据

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

#endif
