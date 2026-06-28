#include "app_resources.h"
#include "button.h"
#include "main.h"

#define BUTTON_DEBOUNCE_MS  50U

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == BTN_EXTI10_Pin)
  {
    osSemaphoreRelease(ButtonBinarySemHandle);
  }
}

void StartButtonTask(void *argument)
{
	osSemaphoreAcquire(ButtonBinarySemHandle, 0);
  for(;;)
  {
	  osSemaphoreAcquire(ButtonBinarySemHandle, osWaitForever);
	  osDelay(BUTTON_DEBOUNCE_MS);

	  if(ButtonPressed())
	  {
	      osEventFlagsSet(AlarmEventHandle, ALARM_EVT_TOGGLE);

	      while(ButtonPressed())
	      {
	          osDelay(10);
	      }
	  }
  }
}
