#include "key.h"
#include "delay.h"
#include "user_TasksInit.h"

//KEY1
#define KEY1_PORT	GPIOA
#define KEY1_PIN	GPIO_PIN_4
#define KEY1 HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN)
//KEY_Wake
#define KEY2_PORT	GPIOA
#define KEY2_PIN	GPIO_PIN_5
#define KEY2 HAL_GPIO_ReadPin(KEY2_PORT,KEY2_PIN)

void Key_Port_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = KEY1_PIN|KEY2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStruct);
	
}

uint8_t KeyScan(uint8_t mode)
{
	static uint8_t key_up = 1;
	uint8_t keyvalue=0;
	if(mode) key_up = 1;
	if( key_up && ((!KEY1) || !KEY2))
	{
		//delay_ms(3);//ensure the key is down
		osDelay(3);
		if(!KEY1) keyvalue = 1;
		else if(!KEY2) keyvalue = 2;
		if(keyvalue) key_up = 0;
	}
	else
	{
		//delay_ms(3);//ensure the key is up
		osDelay(3);
		if(KEY1 && (KEY2))
			key_up = 1;
	}
	return keyvalue;
}

