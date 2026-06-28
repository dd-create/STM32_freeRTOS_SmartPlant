#include "buzzer.h"
#include "main.h"

void Buzzer_set(uint8_t on)
{
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
