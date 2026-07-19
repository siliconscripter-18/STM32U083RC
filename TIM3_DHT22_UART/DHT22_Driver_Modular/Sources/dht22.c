#include <stdint.h>
#include "dht22.h"
#include "timer3.h"
#include "registers.h"

/* Initialize GPIO required for DHT22 communication */
void DHT22_Init(void)
{
	RCC_IOPENR |= (1 << GPIOA_EN);
}

/* Configure DHT22 data pin as open-drain output */
static void DHT22_Output(void)
{
    GPIOA_MODER &= ~(3 << (DHT22_PIN * 2));
    GPIOA_MODER |=  (1 << (DHT22_PIN * 2));

    GPIOA_OTYPER |= (1 << DHT22_PIN);
}

/* Configure DHT22 data pin as input */
static void DHT22_Input(void)
{
    GPIOA_MODER &= ~(3 << (DHT22_PIN * 2));
}

/* Send the start signal to the DHT22 */
static void DHT22_Start(void)
{

    DHT22_Output();

    GPIOA_BSRR = (1 << (DHT22_PIN + 16));

    delay_us(18000);

    DHT22_Input();

}

/* Wait for the DHT22 response sequence */
static void DHT22_Response(void)
{

    while (GPIOA_IDR & (1 << DHT22_PIN));

    while (!(GPIOA_IDR & (1 << DHT22_PIN)));

    while (GPIOA_IDR & (1 << DHT22_PIN));

    
}

/* Read a single data bit from the DHT22 */
static uint8_t DHT22_ReadBit(void)
{
    uint16_t start;
    uint16_t pulsewidth;
   
    while (!(GPIOA_IDR & (1 << DHT22_PIN)));
       
    start = TIM3_CNT;

    while (GPIOA_IDR & (1 << DHT22_PIN));
    
    pulsewidth = (uint16_t)(TIM3_CNT - start);

    return (pulsewidth > 50);
}

/* Read all 40 bits of sensor data */
static void DHT22_Data(uint8_t *data)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        uint8_t byte = 0;

        for (int j = 0; j < 8; j++)
        {
            byte = (byte << 1) | DHT22_ReadBit();
        }

        data[i] = byte;
    }
}

/* Convert raw humidity bytes to percentage */
static float GetHumidity(uint8_t *data)
{
    uint16_t humidityraw = ((uint16_t)data[0] << 8) | data[1];

    return humidityraw / 10.0f;
}

/* Convert raw temperature bytes to Celsius */
static float GetTemperature(uint8_t *data)
{
    uint16_t tempraw = ((uint16_t)data[2] << 8) | data[3];

    if (tempraw & (1 << 15))
    {
        tempraw &= ~(1 << 15);

        return -(tempraw / 10.0f);
    }

    return tempraw / 10.0f;
}

/* Read and validate a complete DHT22 measurement */
uint8_t DHT22_Read(float *humidity, float *temp)
{
    uint8_t data[5];

    DHT22_Start();

    DHT22_Response();

    DHT22_Data(data);

    uint16_t checksum = data[0] + data[1] + data[2] + data[3];

    if ((checksum & 0xFF) != data[4])
    {

        return 0;
    }

    *humidity = GetHumidity(data);
    *temp = GetTemperature(data);

    return 1;
}
