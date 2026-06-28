#include "app_resources.h"
#include "led.h"
#include "buzzer.h"
#include "water_level.h"
#include <stdio.h>

#define MUTE_TIMEOUT_MS  10000U

void StartAlarmTask(void *argument)
{
	AlarmState_t state = ALARM_STATE_IDLE;
	for(;;)
	{
		uint32_t timeout = (state == ALARM_STATE_MUTED) ? MUTE_TIMEOUT_MS : osWaitForever;
		uint32_t flags = osEventFlagsWait(AlarmEventHandle, ALARM_EVT_ALL, osFlagsWaitAny, timeout);

		if ((flags & 0x80000000UL) != 0)
		{
			if (state == ALARM_STATE_MUTED)
			{
		        if ((WaterLevel_GetState() == WATER_LEVEL_HIGH))
		        {
		        	state = ALARM_STATE_ACTIVE;
					Led_set(1);
					Buzzer_set(1);
		        }
		        else
				{
					state = ALARM_STATE_IDLE;
				}
			}
			continue;
		}

		if (flags & ALARM_EVT_ON)
		{
			state = ALARM_STATE_ACTIVE;
			Led_set(1);
			Buzzer_set(1);
		}

		if (flags & ALARM_EVT_OFF)
		{
			state = ALARM_STATE_IDLE;
			Led_set(0);
			Buzzer_set(0);
		}
		if (flags & ALARM_EVT_TOGGLE)
		{
			if (state == ALARM_STATE_ACTIVE)
			{
				state = ALARM_STATE_MUTED;
				Led_set(0);
				Buzzer_set(0);
			}
			else if (state == ALARM_STATE_MUTED)
			{
				state = ALARM_STATE_ACTIVE;
				Led_set(1);
				Buzzer_set(1);
			}
			else
			{
				state = ALARM_STATE_ACTIVE;
				Led_set(1);
				Buzzer_set(1);
			}
		}
	}
}

void StartDebugTask(void *argument)
{
	for(;;)
	{
		uint32_t flags = osEventFlagsWait(AlarmEventHandle,
										   ALARM_EVT_ALL,
										   osFlagsWaitAny,
										   osWaitForever);

		if ((flags & 0x80000000UL) != 0)
		{
			continue;
		}

		osMutexAcquire(UartMutexHandle, osWaitForever);
		if (flags & ALARM_EVT_ON)
		{
			printf("[WARNING] Water level HIGH!\r\n");
		}
		if (flags & ALARM_EVT_OFF)
		{
			printf("[INFO] Water level normalized.\r\n");
		}
		if (flags & ALARM_EVT_TOGGLE)
		{
			printf("[INFO] Button pressed. Alarm state toggled.\r\n");
		}
		osMutexRelease(UartMutexHandle);
	}
}
