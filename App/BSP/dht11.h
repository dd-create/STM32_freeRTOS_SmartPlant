#ifndef DHT11_H
#define DHT11_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum {
    DHT11_OK = 0,
    DHT11_ERR_NO_RESPONSE,
    DHT11_ERR_CHECKSUM
} DHT11_Status_t;

typedef struct {
	float temperature;
	float humidity;
} DHT11_Data_t;

void DHT11_Init(TIM_HandleTypeDef *htim);
void DHT11_SendStart(void);
DHT11_Status_t DHT11_ReceiveAll(uint8_t *rhi, uint8_t *rhd, uint8_t *tci, uint8_t *tcd, uint8_t *sum);
DHT11_Status_t DHT11_Validate(uint8_t rhi, uint8_t rhd, uint8_t tci, uint8_t tcd, uint8_t sum, DHT11_Data_t *data);

#endif
