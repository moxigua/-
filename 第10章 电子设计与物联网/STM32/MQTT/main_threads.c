

#include "cmsis_os.h"
#include "Button/buttons.h"
#include "Output/led.h"
#include "W5500_Functions.h"
#include "W5500_MQTT_Functions.h"
#include "main_threads.h"
#include "stm32f10x.h"

void  ButtonThread(void const *arg);
osThreadDef(ButtonThread,osPriorityNormal,1,0);

int ButtonThread_Inti(void)
{
	Buttons_Inti();
	if(osThreadCreate(osThread(ButtonThread),NULL) == NULL)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/*
  button1 -->  connect 
	button1 -->  publish
	button2 -->  disconnect

*/

void ButtonThread(void const *arg)
{
	int count = 0;
	int flag = 0;
	while(1)
	{
		 count = count + 1;
		 if (count > 5)
		 {
			 count = 0;
			 if (flag == 0)
			 {
				 flag = 1;
				 Led1_Open();
			 }
			 else
			 {
				 flag = 0;
				 Led1_Close();
			 }
			 
		 }
		 if((Button1_IsPressed()) && (!IsConnnected) && dhcp_ok)
		 {
			 osDelay(100);
			 if(Button1_IsPressed())
			 {
					while(Button1_IsPressed());
          	//MQTTClient初始化
					
			 }
		 }
		 if(Button2_IsPressed())
		 {
			 osDelay(100);
			 if(Button2_IsPressed())
			 {
				while(Button2_IsPressed());
				if(IsConnnected)
				 {
						if(MQTTclient_Subscribe(DEFAULT_SUBSCRIBE_TOPIC,MQTT_QOS_0)) 
						{
							IsConnnected = 0;
						}
				 }
			 }
		 }
		  if(Button3_IsPressed() )
		  {
			  osDelay(100);
				if(Button3_IsPressed())
				{
					while(Button3_IsPressed());
					if(IsConnnected)
					 {
							if(MQTTClient_Publish(DEFAULT_PUBLIC_TOPIC,MQTT_QOS_0,"stm32_MQTTClient publish temperature",37)) 
							{
								IsConnnected = 0;
							}
					 }
				}
				
		  }
		 if(Button4_IsPressed() )
		  {
				osDelay(100);
//				if(Button4_IsPressed())
//				{
//					while(Button4_IsPressed());
//					 if(IsConnnected)
//					 {
//							MQTTclient_Disconnect();
//							IsConnnected = 0;
//					 }
//				}
		  }
			osDelay(100);
	}
}
