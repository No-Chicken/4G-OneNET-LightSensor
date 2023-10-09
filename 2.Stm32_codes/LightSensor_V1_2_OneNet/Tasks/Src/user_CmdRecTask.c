/* Private includes -----------------------------------------------------------*/
//includes
#include "string.h"
#include "stm32f1xx_it.h"

#include "user_TasksInit.h"
#include "user_CmdRecTask.h"

#include "onenet_MQTT.h"
#include "BL24C02.h"
#include "lcd.h"
#include "lcd_init.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

float OFFSET_k = 1.0;
int16_t OFFSET_b = 0;

/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  Key press check task
  * @param  argument: Not used
  * @retval None
  */
void CmdRecTask(void *argument)
{
	
	uint8_t strsend[55];
	uint8_t keystr;
	uint8_t strlcd[15];
	int8_t sendbuf[3];


	while(1)
	{
		//use key set the light offset
		if(osMessageQueueGet(Key_MessageQueue,&keystr,NULL,1)==osOK)
		{
			if(keystr == 1)
			{
				OFFSET_k += 0.1;
				if(OFFSET_k > K_MAX)
				{OFFSET_k = K_MIN;}
			}
			else if(keystr == 2)
			{
				OFFSET_b += 100;
				if(OFFSET_b > B_MAX)
				{OFFSET_b = B_MIN;}
			}
			sendbuf[0] = 0x55;
			sendbuf[1] = OFFSET_k*10;
			sendbuf[2] = OFFSET_b/100;
			vTaskSuspendAll();
			BL24C02_Write(0x00,3,sendbuf);
			xTaskResumeAll();
			sprintf(strlcd,"k = %.1f   ",OFFSET_k);
			LCD_ShowString(0,60,strlcd,WHITE,BLACK,24,0);
			sprintf(strlcd,"b = %d   ",OFFSET_b);
			LCD_ShowString(0,90,strlcd,WHITE,BLACK,24,0);
		}

		//recieve the CMD from onenet, to set the offset
		
		if(HardInt_uart_flag == 1)
		{
			HardInt_uart_flag = 0;
			if(HardInt_receive_str[2] == '+' && HardInt_receive_str[3] == 'M' && HardInt_receive_str[4] == 'S' && HardInt_receive_str[5] == 'U' && HardInt_receive_str[6] == 'B' && HardInt_receive_str[7] == ':')
			{
				
				char *CMDdata;
				char CmdID[50];
				CMDdata = strtok(HardInt_receive_str, "/");
		    CMDdata = strtok(NULL,"/");
		    CMDdata = strtok(NULL,"/");
		    CMDdata = strtok(NULL,"/");
		    CMDdata = strtok(NULL,"/");
		    CMDdata = strtok(NULL,"\"");//CmdID
				sprintf(CmdID,"%s",CMDdata);
		    CMDdata = strtok(NULL,",");
    		CMDdata = strtok(NULL,"\r");//data

    		//OFFSET data process
    		if(CMDdata[0] == 'k' && CMDdata[1] == '=' && CMDdata[3] == '.' && CMDdata[5] == 'b' && CMDdata[6] == '=')
    		{
    			float tempk;
    			int16_t tempb;
    			memset(strlcd,0,15);
    			sprintf(strlcd,"%s",CMDdata);
    			tempb = (strlcd[2]-'0')*10+strlcd[4]-'0';
    			tempk = tempb;
    			tempk /= 10;
    			tempb = 0;
    			if(strlcd[12] != 0)
    			{
    				onenet_CmdResponse_MPUB(CmdID,"Format_Erro");
    			}
    			else
    			{
    				//format is:k=1.0b=-1000
	    			if(strlcd[7] == '-')
	    			{
	    				uint8_t index=8;
	    				while(strlcd[index]!=0)
	    				{
	    					tempb *= 10;
	    					tempb += strlcd[index]-'0';
	    					index++;
	    				}
	    				tempb = -tempb;
	    			}
	    			//format is:k=1.0b=100
	    			else
	    			{
	    				uint8_t index=7;
	    				while(strlcd[index]!=0)
	    				{
	    					tempb *= 10;
	    					tempb += strlcd[index]-'0';
	    					index++;
	    				}
	    			}
	    			//check the value is between max and min
	    			if(tempk<=K_MAX && tempk>=K_MIN && tempb<=B_MAX && tempb>=B_MIN)
	    			{
	    				OFFSET_k = tempk;
	    				OFFSET_b = tempb;
	    				sprintf(strsend,"Format_OK,k=%.1f,b=%d",OFFSET_k,OFFSET_b);
	    				onenet_CmdResponse_MPUB(CmdID,strsend);
	    				sendbuf[0] = 0x55;
							sendbuf[1] = OFFSET_k*10;
							sendbuf[2] = OFFSET_b/100;
							vTaskSuspendAll();
							BL24C02_Write(0x00,3,sendbuf);
							xTaskResumeAll();
							sprintf(strlcd,"k = %.1f   ",OFFSET_k);
							LCD_ShowString(0,60,strlcd,WHITE,BLACK,24,0);
							sprintf(strlcd,"b = %d   ",OFFSET_b);
							LCD_ShowString(0,90,strlcd,WHITE,BLACK,24,0);
	    			}
	    			else
	    			{
	    				sprintf(strsend,"Format_OK,value_erro");
	    				onenet_CmdResponse_MPUB(CmdID,strsend);
	    			}
    			}
    		}
    		else
    		{
    			onenet_CmdResponse_MPUB(CmdID,"Format_Erro");
    		}
    		memset(CmdID,0,50);
			}
		}
		
		osDelay(100);
	}
}
