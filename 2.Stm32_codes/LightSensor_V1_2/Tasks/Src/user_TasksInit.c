/* Private includes -----------------------------------------------------------*/
//includes
//sys
#include "main.h"
#include "iwdg.h"
#include "sys.h"
#include "stdio.h"
//tasks
#include "user_TasksInit.h"
#include "user_HardwaresInitTask.h"
#include "user_KeyTask.h"
#include "user_CmdRecTask.h"
#include "user_MeasureSendTask.h"

#include "lcd.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/

/* Timers --------------------------------------------------------------------*/
osTimerId_t IdleTimerHandle;

/* Tasks ---------------------------------------------------------------------*/
//HardwaresInitTask
osThreadId_t HardwaresInitTaskHandle;
const osThreadAttr_t HardwaresInitTask_attributes = {
  .name = "HardwaresInitTask",
  .stack_size = 128 * 5,
  .priority = (osPriority_t) osPriorityHigh1,
};

//WDOG task
osThreadId_t WDOGTaskHandle;
const osThreadAttr_t WDOGTask_attributes = {
  .name = "WDOGTask",
  .stack_size = 128 * 1,
  .priority = (osPriority_t) osPriorityHigh,
};

//LED task
osThreadId_t LEDTaskHandle;
const osThreadAttr_t LEDTask_attributes = {
  .name = "LEDTask",
  .stack_size = 128 * 1,
  .priority = (osPriority_t) osPriorityLow,
};

//Key task
osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
  .name = "KeyTask",
  .stack_size = 128 * 1,
  .priority = (osPriority_t) osPriorityNormal,
};

//CMD Recieve Task
osThreadId_t CmdRecTaskHandle;
const osThreadAttr_t CmdRecTask_attributes = {
  .name = "CmdRecTask",
  .stack_size = 128 * 10,
  .priority = (osPriority_t) osPriorityLow2,
};

//MeasureSend task
osThreadId_t MeasureSendTaskHandle;
const osThreadAttr_t MeasureSendTask_attributes = {
  .name = "MeasureSendTask",
  .stack_size = 128 * 10,
  .priority = (osPriority_t) osPriorityLow1,
};

//ConCheck task
osThreadId_t ConCheckTaskHandle;
const osThreadAttr_t ConCheckTask_attributes = {
  .name = "ConCheckTask",
  .stack_size = 128 * 5,
  .priority = (osPriority_t) osPriorityLow1,
};

/* Message queues ------------------------------------------------------------*/
osMessageQueueId_t Key_MessageQueue;


/* Private function prototypes -----------------------------------------------*/
void WDOGFeedTask(void *argument);
void LEDTask(void *argument);


/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void User_Tasks_Init(void) 
{
  /* add mutexes, ... */

  /* add semaphores, ... */

  /* start timers, add new ones, ... */
	
  /* add queues, ... */
	Key_MessageQueue  = osMessageQueueNew(1, 1, NULL);
	
	/* add threads, ... */
  HardwaresInitTaskHandle = osThreadNew(HardwaresInitTask, NULL, &HardwaresInitTask_attributes);
  WDOGTaskHandle          = osThreadNew(WDOGFeedTask, NULL, &WDOGTask_attributes);
	LEDTaskHandle           = osThreadNew(LEDTask, NULL, &LEDTask_attributes);
  KeyTaskHandle           = osThreadNew(KeyTask, NULL, &KeyTask_attributes);
  MeasureSendTaskHandle   = osThreadNew(MeasureSendTask, NULL, &MeasureSendTask_attributes);
  CmdRecTaskHandle        = osThreadNew(CmdRecTask, NULL, &CmdRecTask_attributes);
  ConCheckTaskHandle      = osThreadNew(ConCheckTask, NULL, &ConCheckTask_attributes);

  /* add events, ... */
	
	
	/* add  others ... */
	
	
}


/**
  * @brief  FreeRTOS Tick Hook
  * @param  None
  * @retval None
  */
void vApplicationTickHook()
{
	
}


/**
  * @brief  Feed the WatchDog
  * @param  None
  * @retval None
  */
void WDOGFeedTask(void *argument)
{
	MX_IWDG_Init();
  while(1)
  {
    HAL_IWDG_Refresh(&hiwdg);
    osDelay(100);
  }
}

/**
  * @brief  LED Task, to show the sys is not choken
  * @param  None
  * @retval None
  */
void LEDTask(void *argument)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
		osDelay(500);
	}
}


