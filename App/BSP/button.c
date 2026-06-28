#include "button.h"
#include "main.h"

uint8_t ButtonPressed(void)
{
	return (HAL_GPIO_ReadPin(BTN_EXTI10_GPIO_Port, BTN_EXTI10_Pin) == GPIO_PIN_RESET)? 1 : 0;
}

