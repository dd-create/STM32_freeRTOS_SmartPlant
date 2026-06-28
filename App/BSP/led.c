#include "led.h"
#include "main.h"

void Led_set(uint8_t on)
{
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
