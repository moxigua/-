
#include "Driver_SPI.h"
#include "cmsis_os.h" // ARM::CMSIS:RTOS:Keil RTX

/* SPI发送线程声明 */
void mySPI_Thread(void const *argument);//发送数据函数原型
osThreadId tid_mySPI_Thread;         		//发送数据线程ID
osThreadDef (mySPI_Thread, osPriorityNormal, 1, 0);  

/* SPI 驱动结构体*/
extern ARM_DRIVER_SPI Driver_SPI1;   //引用SPI驱动结构体
ARM_DRIVER_SPI * SPIdrv = &Driver_SPI1;  


const uint8_t testdata_out[8] = { 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77 }; //需要发送的数据
uint8_t testdata_in [8];                  //接受的数据

void mySPI_callback(uint32_t event)
{
  switch (event){
	case ARM_SPI_EVENT_TRANSFER_COMPLETE:
		/* 成功：唤醒线程 */
		osSignalSet(tid_mySPI_Thread, 0x01);
		break;
	case ARM_SPI_EVENT_DATA_LOST:
		__breakpoint(0); /* 出现错误*/
		break;
	case ARM_SPI_EVENT_MODE_FAULT:
		__breakpoint(0); /*出现错误*/
		break;
	}
}

int main(void)
{
	osKernelInitialize();
	SPIdrv->Initialize(mySPI_callback); //初始化 SPI 驱动 
	SPIdrv->PowerControl(ARM_POWER_FULL); // 给SPI 外设上电 
	SPIdrv->Control(ARM_SPI_MODE_MASTER | ARM_SPI_CPOL1_CPHA1 | ARM_SPI_MSB_LSB | ARM_SPI_SS_MASTER_SW | ARM_SPI_DATA_BITS(8), 1000000);  // 配置SPI为主机模式,8字节数据位 @1000 kBits/s
	SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE); //失能片选信号，置为高电平 
	osKernelStart();
	tid_mySPI_Thread = osThreadCreate( osThread( mySPI_Thread),NULL);
}

void mySPI_Thread(void const* argument)
{
	osEvent evt;  
	while (1){
		SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_ACTIVE);//使能片选信号，置低
		SPIdrv->Send(testdata_out, sizeof(testdata_out));  //发送数据

		evt = osSignalWait(0x01, 100);  //等待发送完成
		if (evt.status == osEventTimeout) {
			__breakpoint(0); //超时
		}
		SPIdrv->Control(ARM_SPI_CONTROL_SS, ARM_SPI_SS_INACTIVE);//失能片选信号
	}
}


