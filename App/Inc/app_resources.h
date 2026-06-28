#ifndef APP_RESOURCES_H
#define APP_RESOURCES_H

#include <stdint.h>
#include "cmsis_os.h"

#define ALARM_EVT_ON    (1UL << 0)
#define ALARM_EVT_OFF   (1UL << 1)
#define ALARM_EVT_TOGGLE   (1UL << 2)
#define ALARM_EVT_ALL   (ALARM_EVT_ON | ALARM_EVT_OFF | ALARM_EVT_TOGGLE)

typedef enum {
    ALARM_STATE_IDLE,
    ALARM_STATE_ACTIVE,
    ALARM_STATE_MUTED
} AlarmState_t;

typedef enum {
    SENSOR_DHT11,
    SENSOR_WATER
} SensorType_t;

typedef struct {
    SensorType_t type;
    union {
        struct {
            float temperature;
            float humidity;
        } dht11;
        struct {
            uint32_t level;
        } water;
    } data;
} SensorMsg_t;

extern osSemaphoreId_t ButtonBinarySemHandle;
extern osEventFlagsId_t AlarmEventHandle;
extern osMutexId_t UartMutexHandle;
extern osMessageQueueId_t LCDQueueHandle;

void StartButtonTask(void *argument);
void StartDebugTask(void *argument);
void StartWSTask(void *argument);
void StartDHT11Task(void *argument);
void StartLCDTask(void *argument);
void StartAlarmTask(void *argument);

#endif
