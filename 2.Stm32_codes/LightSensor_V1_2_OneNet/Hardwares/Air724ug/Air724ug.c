#include "Air724ug.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "stm32f1xx_it.h"

/**
  ******************************************************************************
  * @file         Air724ug.c
  * @brief        这个文件用于合宙的Air724ug模块，使用串口发送AT指令. 
  * 			  
  * @author	      Kingham Xu              
  ******************************************************************************
  * @attention
  *
  * 由于本文件中串口发送指令全部用了 printf, 在编译的时候要打开 use microlib .
  * printf 重定向在 usart.c 里面定义了.
  * 
  ******************************************************************************
  */


void Air724_Reset(void)
{
	printf("AT+RESET\r\n");
}


/**
  * @brief  初始化Air724,包括：
  * 		1.匹配波特率
  * 		2.检测SIM卡是否插好
  * 		3.信号质量(17-31信号才良好)
  * 		4.网络注册状态(正常第二位返回1)
  * 		5.查询网络状态GPRS附着状态(正常返回1,停机返回0)
  * 		6.设置APN(获取IP失败)
  * 		7.激活网络(初始只能激活一次)
  * 		8.查询IP地址是否有了
  */
void Air724_Init(void)
{
  //匹配波特率
	while(strcmp(HardInt_receive_str,"AT\r\n\r\nOK\r\n"))
	{
		//memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
		printf("AT\r\n");
		delay_ms(100);
	}
  //检测SIM卡是否插好
  while(strcmp(HardInt_receive_str,"AT+CPIN?\r\n\r\n+CPIN: READY\r\n\r\nOK\r\n"))
  {
    //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
    printf("AT+CPIN?\r\n");
    delay_ms(200);
  }
  //信号质量

  //网络注册状态
  while(strcmp(HardInt_receive_str,"AT+CREG?\r\n\r\n+CREG: 0,1\r\n\r\nOK\r\n"))
  {
    //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
    printf("AT+CREG?\r\n");
    delay_ms(200);
  }
  //GPRS附着状态
  while(strcmp(HardInt_receive_str,"AT+CGATT?\r\n\r\n+CGATT: 1\r\n\r\nOK\r\n"))
  {
    //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
    printf("AT+CGATT?\r\n");
    delay_ms(200);
  }
  //查询IP
  //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
  printf("AT+CIFSR\r\n");
  delay_ms(200);
  while(!strcmp(HardInt_receive_str,"AT+CIFSR\r\n\r\n+CME ERROR: 3\r\n\r\n"))
  {
    //设置APN
    printf("AT+CSTT=\"\",\"\",\"\"\r\n");
    delay_ms(200);
    //激活网络
    printf("AT+CIICR\r\n");
    delay_ms(200);
    //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
    printf("AT+CIFSR\r\n");
    delay_ms(200);
  }
}


/**
  * @brief  激活PDP,包括：
  * 		1.打开承载,激活PDP
  * 		2.设置GPRS承载场景ID
  * 		3.同步网络时间
  */
void Air724_PDPact(void)
{
  
  //设置GPRS承载场景ID
  while(strcmp(HardInt_receive_str,"AT+CNTPCID=1\r\n\r\nOK\r\n"))
  {
    //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
    printf("AT+CNTPCID=1\r\n");
    delay_ms(200);
  }
  //激活PDP
  printf("AT+SAPBR=1,1\r\n");
  delay_ms(200);
  //同步网络时间
  while(HardInt_receive_str[9]!='1')//获取正常为: \r\n+CNTP: 1\r\n
  {
    //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
    printf("AT+CNTP\r\n");
    delay_ms(600);
  }
}

void Air724_GetTime(void)
{
  printf("AT+CCLK?\r\n");
  delay_ms(200);
  if(HardInt_receive_str[12] == '+' 
    && HardInt_receive_str[13] == 'C' 
    && HardInt_receive_str[14] == 'C' 
    && HardInt_receive_str[15] == 'L' 
    && HardInt_receive_str[16] == 'K')
  {
    uint8_t hour,min,sec;
    hour = (HardInt_receive_str[29]-'0')*10 + HardInt_receive_str[30]-'0';
    min = (HardInt_receive_str[32]-'0')*10 + HardInt_receive_str[33]-'0';
    sec = (HardInt_receive_str[35]-'0')*10 + HardInt_receive_str[36]-'0';
    if(hour>=0 && hour <=23 && min>=0 && min<=59 && sec>=0 && sec<=59)
      RTC_SetTime(hour,min,sec);
  }
  
}

/**
  * @brief  Air724进入飞行模式
  */
void Air724_FlightMode(void)
{
  printf("AT+CFUN=4\r\n");
  delay_ms(400);
}
