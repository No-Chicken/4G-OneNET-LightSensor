/* Private includes -----------------------------------------------------------*/
//includes
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f1xx_it.h"
#include "user_TasksInit.h"
#include "user_CmdRecTask.h"
#include "key.h"
#include "Air724ug.h"
#include "onenet_MQTT.h"
#include "lcd.h"
#include "lcd_init.h"
#include "Max44009.h"
#include "BL24C02.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  measure the light and send to the cloud
  * @param  argument: Not used
  * @retval None
  */
void HardwaresInitTask(void *argument)
{
	while(1)
	{
		vTaskSuspendAll();
		//lcd
		LCD_Init();
		LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
		//Key
		Key_Port_Init();
		//Max44009
		uint8_t strlcd[10];
		uint8_t i=5;
		uint8_t MAX_Erro = 1;
		while(MAX_Erro && i)
		{
			i--;
			MAX_Erro = MAX44009_Init();
			sprintf(strlcd,"max erro:%d",MAX_Erro);
			LCD_ShowString(0,0,strlcd,WHITE,BLACK,24,0);
		}
		//EEPROM
		BL24C02_Init();

		//EEPROM read the data
		int8_t recbuf[3];
		BL24C02_Read(0x00,3,recbuf);
		if(recbuf[0] == 0x55)
		{
			if(recbuf[1]<=(K_MAX*10) && recbuf[1]>=(K_MIN*10) && recbuf[2]<=(B_MAX/100) && recbuf[2]>=(B_MIN/100))
			{
				OFFSET_k = recbuf[1];
				OFFSET_k /= 10;
				OFFSET_b = recbuf[2];
				OFFSET_b *= 100;
			}
		}
		else
		{
			recbuf[0] = 0x55;
			recbuf[1] = OFFSET_k*10;
			recbuf[2] = OFFSET_b/100;
			BL24C02_Write(0x00,3,recbuf);
		}
		LCD_ShowString(0,30,"lux=kx+b",WHITE,BLACK,24,0);
		sprintf(strlcd,"k = %.1f   ",OFFSET_k);
		LCD_ShowString(0,60,strlcd,WHITE,BLACK,24,0);
		sprintf(strlcd,"b = %d   ",OFFSET_b);
		LCD_ShowString(0,90,strlcd,WHITE,BLACK,24,0);
		
		//Air724
		LCD_ShowString(0,120,"Air724...",WHITE,BLACK,24,0);
		Air724_Init();
		//PDP
		LCD_ShowString(0,120,"PDP...   ",WHITE,BLACK,24,0);
		Air724_PDPact();
		//MQTT
		LCD_ShowString(0,120,"MQTT...  ",WHITE,BLACK,24,0);
		onenet_MQTTConnect();
		LCD_ShowString(0,120,"         ",WHITE,BLACK,24,0);
		//set the rct
		Air724_GetTime();
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
			//订阅CMD主题
			printf("AT+MSUB=\"$sys/%s/%s/cmd/request/+\",0\r\n",ProductID,DeviceName);
			delay_ms(400);
		}
		
		xTaskResumeAll();
		vTaskDelete(NULL);
		osDelay(1000);
	}
	
}
