#include "app_resources.h"
#include "water_level.h"
#include "dht11.h"
#include <stdio.h>

void StartWSTask(void *argument)
{
    SensorMsg_t msg;
    msg.type = SENSOR_WATER;

    for (;;)
    {
        uint32_t level = 0;
        WaterLevelEvent_t event = WaterLevel_Read(&level);

        msg.data.water.level = level;

        if (event == WATER_EVENT_BECAME_HIGH)
        {
            osEventFlagsSet(AlarmEventHandle, ALARM_EVT_ON);
        }
        else if (event == WATER_EVENT_BECAME_NORMAL)
        {
            osEventFlagsSet(AlarmEventHandle, ALARM_EVT_OFF);
        }

        osMessageQueuePut(LCDQueueHandle, &msg, 0, 0);

        osDelay(500);
    }
}

void StartDHT11Task(void *argument)
{
	osDelay(1000);
    SensorMsg_t msg;
    msg.type = SENSOR_DHT11;

    for(;;)
    {
    	DHT11_Data_t data;
        uint8_t rhi, rhd, tci, tcd, sum;

        DHT11_SendStart();

        taskENTER_CRITICAL();
        DHT11_Status_t status = DHT11_ReceiveAll(&rhi, &rhd, &tci, &tcd, &sum);
        taskEXIT_CRITICAL();

        if (status == DHT11_OK)
        {
        	status = DHT11_Validate(rhi, rhd, tci, tcd, sum, &data);
        }

        switch (status)
        {
        case DHT11_OK:
        	msg.data.dht11.temperature = data.temperature;
        	msg.data.dht11.humidity    = data.humidity;
			osMessageQueuePut(LCDQueueHandle, &msg, 0, 0);
			break;
        case DHT11_ERR_CHECKSUM:
        	printf("에러: 체크섬 오류\r\n");
        	break;
        case DHT11_ERR_NO_RESPONSE:
        	printf("에러: DHT11 응답 없음\r\n");
        	break;
        }
        osDelay(pdMS_TO_TICKS(2000));
    }
}
