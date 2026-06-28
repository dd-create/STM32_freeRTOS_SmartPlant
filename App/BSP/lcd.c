#include "lcd.h"
#include "liquidcrystal_i2c.h"
#include <stdio.h>

void LCD_Init()
{
	HD44780_Init(2);
	HD44780_Clear();
	HD44780_Backlight();
}

void LCD_ShowTempHumidity(float temp, float humid)
{
	char line[17];
	snprintf(line, sizeof(line), "T:%.1fC H:%.1f%%", temp, humid);
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr(line);
}

void LCD_ShowWaterLevel(uint32_t level)
{
	char line[17];
	snprintf(line, sizeof(line), "Water:%-10lu", (unsigned long)level);
	HD44780_SetCursor(0, 1);
	HD44780_PrintStr(line);
}
