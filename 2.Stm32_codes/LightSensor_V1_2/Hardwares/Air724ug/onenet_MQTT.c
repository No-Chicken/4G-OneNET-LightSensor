#include "Air724ug.h"
#include "onenet_MQTT.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
#include "stm32f1xx_it.h"

/**
  ******************************************************************************
  * @file         onenet_MQTT.c
  * @brief        这个文件用于合宙的Air724ug模块使用MQTT连接中国移动onenet.
  * @author	      Kingham Xu              
  ******************************************************************************
  * @attention
  *
  * 由于本文件中串口发送指令全部用了 printf, 在编译的时候要打开 use microlib .
  * printf 重定向在 usart.c 里面定义了.
  * 
  ******************************************************************************
  */


/**
  * @brief  onenet设备参数，每个设备都需要改下.
  * 		    sign是通过onenet的token算法计算得到的值,
  *         token工具: https://open.iot.10086.cn/doc/mqtt/book/manual/auth/tool.html
  *         1.发文res格式为products/()/devices/()
  *         括号里面填你的参数
  *         2.时间戳et大于当前时间就行
  *         3.秘钥key去设备详情复制
  */
//机3
const char * ProductID = "592681";
const char * DeviceName = "LightSensor1";
const char * sign = "version=2018-10-31&res=products%2F592693%2Fdevices%2FLightSensor3&et=1996892239&method=md5&sign=??????????????????";//md5算法生成的sign

const char * mqtturl = "mqtts.heclouds.com";
const char * mqttport= "1883";//port = 1883

//发送数据格式
//char * sendteststr = "{\\22params\\22:{\\22MeasuredIlluminance\\22:211.1}}";

/**
  * @brief 	连接MQTT,接入onenet,包括：
  * 		1.设置MQTT相关参数
  * 		2.建立TCP连接
  * 		3.请求会话(失败断开TCP重试)
  * 		4.订阅多个主题
  */
void onenet_MQTTConnect(void)
{
  //设置参数
  printf("AT+MCONFIG=\"%s\",\"%s\",\"%s\"\r\n",DeviceName,ProductID,sign);
  delay_ms(400);
  //断开连接
  onenet_TCPDisConnect();
  delay_ms(400);
  //建立TCP连接
  printf("AT+MIPSTART=\"%s\",%s\r\n",mqtturl,mqttport);
  delay_ms(400);
  //请求会话//是否erro
  printf("AT+MCONNECT=1,120\r\n");
  delay_ms(400);
  //订阅消息主题
  //printf("AT+MSUB=\"%s\",0\r\n",topicpost);
  //delay_ms(400);
  //printf("AT+MSUB=\"%s\",0\r\n",topicreply);
  //delay_ms(400);

  //订阅CMD主题
  printf("AT+MSUB=\"$sys/%s/%s/cmd/request/+\",0\r\n",ProductID,DeviceName);
	delay_ms(400);
}


/**
  * @brief  TCP断开连接
  */
void onenet_TCPDisConnect(void)
{
  //断开TCP连接
  printf("AT+MIPCLOSE\r\n");
  delay_ms(200);
}


/**
  * @brief 	向已订阅主题发布消息传输数据
  *         数据传输格式必须满足 Alink JSON 格式
  *         Air724发送消息时用\\22替换掉引号"
  */
void onenet_Data_MPUB(char * strsend)
{
  printf("AT+MPUB=\"$sys/%s/%s/dp/post/json\",0,0,\"%s\"\r\n",ProductID,DeviceName,strsend);
  delay_ms(400);
}


/**
  * @brief  向已订阅主题发布消息传输数据
  *         数据传输格式必须满足 Alink JSON 格式
  *         Air724发送消息时用\\22替换掉引号"
  */
void onenet_CmdResponse_MPUB(char *CmdID,char * strsend)
{
  printf("AT+MPUB=\"$sys/%s/%s/cmd/response/%s\",0,0,\"%s\"\r\n",ProductID,DeviceName,CmdID,strsend);
  delay_ms(400);
}


/**
  * @brief  查询MQTT连接状态
  *         1:OK
  *         0:没连上
  */
uint8_t Connect_Check(void)
{
  //memset(HardInt_receive_str,0,strlen(HardInt_receive_str));
  printf("AT+MQTTSTATU\r\n");
  delay_ms(200);
  if(!strcmp(HardInt_receive_str,"AT+MQTTSTATU\r\n\r\n+MQTTSTATU :1\r\n\r\nOK\r\n"))
  {
    return 1;
  }
  return 0;
}



