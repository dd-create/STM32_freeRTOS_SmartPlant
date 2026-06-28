#include "app_resources.h"
#include "lcd.h"

void StartLCDTask(void *argument)
{
	SensorMsg_t msg;

	for(;;)
	{
	  osStatus_t status = osMessageQueueGet(LCDQueueHandle, &msg, NULL, 500);

	  if(status == osOK)
	  {
		  if(msg.type == SENSOR_DHT11)
		  {
			  LCD_ShowTempHumidity(msg.data.dht11.temperature, msg.data.dht11.humidity);
		  }
		  else if(msg.type == SENSOR_WATER)
		  {
			  LCD_ShowWaterLevel(msg.data.water.level);

		  }
	  }
	}
}
