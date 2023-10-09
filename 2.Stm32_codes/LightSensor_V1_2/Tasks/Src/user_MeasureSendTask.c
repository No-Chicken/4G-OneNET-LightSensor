/* Private includes -----------------------------------------------------------*/
//includes
#include "stdio.h"
#include "string.h"

#include "stm32f1xx_it.h"
#include "rtc.h"

#include "user_TasksInit.h"
#include "user_MeasureSendTask.h"
#include "user_KeyTask.h"
#include "user_CmdRecTask.h"

#include "Air724ug.h"
#include "onenet_MQTT.h"
#include "lcd.h"
#include "lcd_init.h"
#include "Max44009.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  measure the light and send to the cloud
  * @param  argument: Not used
  * @retval None
  */
void MeasureSendTask(void *argument)
{
	float Light=0.0;
	uint8_t strsend[55];
	uint8_t strlcd[10];
	RTC_TimeTypeDef nowtime;
			
	
	
	while(1)
	{
		//measure the Light
		vTaskSuspendAll();
		MAX_Measure(&Light);
		xTaskResumeAll();
		Light = OFFSET_k * Light +  OFFSET_b;

		//Send the Light data to OneNet 
		if(Light<200000.0 && Light>100.0)
		{
			sprintf(strsend,"{\\22id\\22: 1,\\22dp\\22: {\\22light\\22: [{\\22v\\22:%d}]}}",(uint32_t)Light);
			onenet_Data_MPUB(strsend);
			sprintf(strlcd,"lux = %.1f   ",Light);
			LCD_ShowString(0,120,strlcd,WHITE,BLACK,24,0);
		}

		HAL_RTC_GetTime(&hrtc,&nowtime,RTC_FORMAT_BIN);
		sprintf(strlcd,"%02d:%02d:%02d   ",nowtime.Hours,nowtime.Minutes,nowtime.Seconds);
		LCD_ShowString(0,150,strlcd,WHITE,BLACK,24,0);

		osDelay(500);
	}
}


void ConCheckTask(void *argument)
{
	while(1)
	{

		//check the TCP is connected or not
		if(!Connect_Check())
		{
			//断开TCP连接
			onenet_TCPDisConnect();
			//建立TCP连接
		  printf("AT+MIPSTART=\"%s\",%s\r\n",mqtturl,mqttport);
		  delay_ms(400);
		  //请求会话//是否erro
		  printf("AT+MCONNECT=1,120\r\n");
		  delay_ms(400);
			LCD_ShowString(0,150,"reconnect",WHITE,BLACK,24,0);
			delay_ms(400);
			LCD_ShowString(0,150,"         ",WHITE,BLACK,24,0);
		}
		osDelay(10000);
	}
}