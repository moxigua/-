

#include "cmsis_os.h"
#include "Button/buttons.h"
#include "W5500_Functions.h"
#include "main_threads.h"
#include "stm32f10x.h"

uint8_t IsConnnected = 0;

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
	while(1)
	{
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
				 {//
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
					 {//
					 }
				}
				
		  }
		 if(Button4_IsPressed() )
		  {
				osDelay(100);
				if(Button4_IsPressed())
				{
					while(Button4_IsPressed());
					 if(IsConnnected)
					 {
							//
					 }
				}
		  }
			osDelay(100);
	}
}
