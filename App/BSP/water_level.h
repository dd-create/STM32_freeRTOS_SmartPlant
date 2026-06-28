#ifndef WATER_LEVEL_H
#define WATER_LEVEL_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define WATER_LEVEL_ALARM_ON   1300U
#define WATER_LEVEL_ALARM_OFF  1000U

typedef enum {
    WATER_LEVEL_NORMAL = 0,
    WATER_LEVEL_HIGH   = 1
} WaterLevelState_t;

typedef enum {
    WATER_EVENT_NONE = 0,
    WATER_EVENT_BECAME_HIGH,
    WATER_EVENT_BECAME_NORMAL
} WaterLevelEvent_t;

void WaterLevel_Init(ADC_HandleTypeDef *hadc);
WaterLevelEvent_t WaterLevel_Read(uint32_t *raw_level);
WaterLevelState_t WaterLevel_GetState(void);

#endif
