#ifndef LCD_H
#define LCD_H

#include <stdint.h>

void LCD_Init();
void LCD_ShowTempHumidity(float temp, float humid);
void LCD_ShowWaterLevel(uint32_t level);

#endif
