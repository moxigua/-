
#include "Driver_USART.h"

//定义USART驱动结构体
extern  ARM_DRIVER_USART Driver_USART1;

void UserPage(void)
{
	Driver_USART1.Send("->command\r\n",11);
	while(Driver_USART1.GetStatus().tx_busy);
	Driver_USART1.Send("  0:close the led.\r\n",20);
	while(Driver_USART1.GetStatus().tx_busy);
	Driver_USART1.Send("  1:open the led.\r\n",19);
	while(Driver_USART1.GetStatus().tx_busy);
	
}

int main(void)
{
	uint8_t buffer[1024] = {0}; //接受数据存放的数组
	/*初始化USART1外设 */
	Driver_USART1.Initialize(NULL);
	/*USART1上电 */
	Driver_USART1.PowerControl(ARM_POWER_FULL);
	/*配置USART1为异步通信，波特率115200，字长8位，停止位为1，没有奇偶校验 */
	Driver_USART1.Control(ARM_USART_MODE_ASYNCHRONOUS |
							ARM_USART_DATA_BITS_8 |
							ARM_USART_PARITY_NONE |
							ARM_USART_STOP_BITS_1 |
							ARM_USART_FLOW_CONTROL_NONE, 115200);

	/* 使能串口发送和接受*/
	Driver_USART1.Control (ARM_USART_CONTROL_TX, 1);
	Driver_USART1.Control (ARM_USART_CONTROL_RX, 1);
	/* 打印提示页面 */
	UserPage();
	while(1){
		
		Driver_USART1.Receive(buffer,1);
		while(Driver_USART1.GetStatus().rx_busy);
		 if(Driver_USART1.GetRxCount()){ //如果接受缓冲区有数据
			
			switch(buffer[0]){
				case 0:
					Driver_USART1.Send("->led has closed.\r\n",20);//模拟关闭Led
					while(Driver_USART1.GetStatus().tx_busy);
				break;
				case 1:
					Driver_USART1.Send("->led has openned.\r\n",21);//模拟开启led
					while(Driver_USART1.GetStatus().tx_busy);
				break;
				default:
					Driver_USART1.Send("->command error!!.\r\n",20);//提示无效命令
					UserPage(); //打印提示页面
				break;	
			}
		}
	}
}

