#include "servo.h"
#include "usart.h"   
#include "tim.h"      
void servo_run(void)
{
	 static uint8_t angle = 0;
	 static uint8_t ch;	 
	HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY);     
	if(ch == '\r')	
	  {
		  return;
  	}		
  if (angle <= 180)
	  {
		  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1000 + angle * 100 / 18);	     
			angle = 0;	
	  }				 
	else
	{
		angle = angle * 10 + (ch - '0');
	}
}
		
