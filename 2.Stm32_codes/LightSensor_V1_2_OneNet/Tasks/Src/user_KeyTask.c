/* Private includes -----------------------------------------------------------*/
//includes
#include "user_TasksInit.h"
#include "key.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  Key press check task
  * @param  argument: Not used
  * @retval None
  */
void KeyTask(void *argument)
{
	uint8_t keystr=0;
	while(1)
	{
		switch(KeyScan(0))
		{
			case 1:
				keystr = 1;
				osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
				break;
				
			case 2:
				keystr = 2;
				osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
				break;
		}
		osDelay(1);
	}
}
