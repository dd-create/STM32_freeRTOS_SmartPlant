#include "dht11.h"

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_1

static TIM_HandleTypeDef *dhtTim = NULL;

static void microDelay (uint16_t delay)
{
  __HAL_TIM_SET_COUNTER(dhtTim, 0);
  while (__HAL_TIM_GET_COUNTER(dhtTim) < delay);
}

void DHT11_Init(TIM_HandleTypeDef *htim)
{
    dhtTim = htim;
    HAL_TIM_Base_Start(dhtTim);
}

void DHT11_SendStart(void)
{
    GPIO_InitTypeDef g = {0};
    g.Pin   = DHT11_PIN;
    g.Mode  = GPIO_MODE_OUTPUT_OD;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    g.Pull  = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PORT, &g);

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 0);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 1);
}

static uint8_t DHT11_ReadByte(void)
{
    uint8_t a, b = 0;
    for (a = 0; a < 8; a++)
    {
        uint32_t t = HAL_GetTick();
        while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
        {
            if (HAL_GetTick() - t >= 2) break;
        }
        microDelay(40);

        if (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
            b &= ~(1 << (7 - a));
        else
            b |= (1 << (7 - a));

        t = HAL_GetTick();
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
        {
            if (HAL_GetTick() - t >= 2) break;
        }
    }
    return b;
}

DHT11_Status_t DHT11_ReceiveAll(uint8_t *rhi, uint8_t *rhd, uint8_t *tci, uint8_t *tcd, uint8_t *sum)
{
    uint8_t response = 0;

    GPIO_InitTypeDef g = {0};
    g.Pin  = DHT11_PIN;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PORT, &g);

    microDelay(30);

    if (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
    {
        microDelay(80);
        if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) response = 1;
    }

    uint32_t t = HAL_GetTick();
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
    {
        if (HAL_GetTick() - t >= 2) break;
    }

    if (!response) return DHT11_ERR_NO_RESPONSE;

    *rhi = DHT11_ReadByte();
    *rhd = DHT11_ReadByte();
    *tci = DHT11_ReadByte();
    *tcd = DHT11_ReadByte();
    *sum = DHT11_ReadByte();

    return DHT11_OK;
}

DHT11_Status_t DHT11_Validate(uint8_t rhi, uint8_t rhd, uint8_t tci, uint8_t tcd, uint8_t sum, DHT11_Data_t *data)
{
	if((uint8_t)rhi + rhd + tci + tcd != sum)
	{
		return DHT11_ERR_CHECKSUM;
	}
	data->temperature = (float)tci + (tcd / 10.0f);
	data->humidity = (float)rhi + (rhd / 10.0f);
	return DHT11_OK;
}
