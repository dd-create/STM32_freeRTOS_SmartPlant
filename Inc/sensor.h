#ifndef SENSOR_H
#define SENSOR_H

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

#include "main.h"

typedef enum {
    SENSOR_DHT11 = 0,
    SENSOR_WATER = 1
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

void microDelay (uint16_t delay);
void DHT11_SendStart(void);
uint8_t DHT11_ReadByte(void);
uint8_t DHT11_ReceiveAll(uint8_t *rhi, uint8_t *rhd,
                          uint8_t *tci, uint8_t *tcd, uint8_t *sum);

#endif
