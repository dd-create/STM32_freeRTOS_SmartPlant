#include "water_level.h"

static ADC_HandleTypeDef *w_hadc = NULL;
static WaterLevelState_t w_state = WATER_LEVEL_NORMAL;

void WaterLevel_Init(ADC_HandleTypeDef *hadc)
{
	w_hadc = hadc;
}

WaterLevelEvent_t WaterLevel_Read(uint32_t *raw_level)
{
	WaterLevelEvent_t event = WATER_EVENT_NONE;

	HAL_ADC_Start(w_hadc);
	if(HAL_ADC_PollForConversion(w_hadc, 10) != HAL_OK)
	{
		return WATER_EVENT_NONE;
	}
	uint32_t level = HAL_ADC_GetValue(w_hadc);
	if(raw_level) *raw_level = level;

	if (w_state == WATER_LEVEL_NORMAL && level >= WATER_LEVEL_ALARM_ON)
	{
		w_state = WATER_LEVEL_HIGH;
		event = WATER_EVENT_BECAME_HIGH;
	}
	else if (w_state == WATER_LEVEL_HIGH && level <= WATER_LEVEL_ALARM_OFF)
	{
		w_state = WATER_LEVEL_NORMAL;
		event = WATER_EVENT_BECAME_NORMAL;
	}

	return event;
}

WaterLevelState_t WaterLevel_GetState(void)
{
    return w_state;
}
