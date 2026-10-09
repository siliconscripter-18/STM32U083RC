/******************************************************************************
 * @file    dht22.c
 * @brief   DHT22 temperature and humidity sensor driver.
 *
 *          Provides initialization, communication, data acquisition, and
 *          checksum validation for the DHT22 sensor.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "dht22.h"
#include "tim1.h"

/******************************************************************************
 * @brief Initializes GPIO required for DHT22 communication.
 ******************************************************************************/
void DHT22_Init(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
}

/******************************************************************************
 * @brief Configures the DHT22 data pin as an open-drain output.
 ******************************************************************************/
static void DHT22_Output(void)
{
    GPIOA->MODER &= ~(3U << (DHT22_PIN * 2));
    GPIOA->MODER |= (1U << (DHT22_PIN * 2));
    GPIOA->OTYPER |= (1U << DHT22_PIN);
}

/******************************************************************************
 * @brief Configures the DHT22 data pin as an input.
 ******************************************************************************/
static void DHT22_Input(void)
{
    GPIOA->MODER &= ~(3U << (DHT22_PIN * 2));
}

/******************************************************************************
 * @brief Sends the start signal to the DHT22 sensor.
 ******************************************************************************/
static void DHT22_Start(void)
{
    DHT22_Output();

    /* Pull data line LOW for at least 18 ms */
    GPIOA->BSRR = (1U << (DHT22_PIN + 16));
    delay_us(18000);

    /* Release data line */
    DHT22_Input();
}

/******************************************************************************
 * @brief Waits for the DHT22 response sequence.
 *
 * @return 1 if the sensor responds correctly, otherwise 0.
 ******************************************************************************/
static uint8_t DHT22_Response(void)
{
    uint16_t start;

    /* Wait for sensor to pull the line LOW */
    start = TIM1->CNT;

    while (GPIOA->IDR & (1U << DHT22_PIN))
    {
        if ((uint16_t)(TIM1->CNT - start) > 1000U)
            return 0U;
    }

    /* Wait for sensor to pull the line HIGH */
    start = TIM1->CNT;

    while (!(GPIOA->IDR & (1U << DHT22_PIN)))
    {
        if ((uint16_t)(TIM1->CNT - start) > 1000U)
            return 0U;
    }

    /* Wait for sensor to pull the line LOW again */
    start = TIM1->CNT;

    while (GPIOA->IDR & (1U << DHT22_PIN))
    {
        if ((uint16_t)(TIM1->CNT - start) > 1000U)
            return 0U;
    }

    return 1U;
}

/******************************************************************************
 * @brief Reads a single data bit from the DHT22.
 *
 * @return 1 for a logic HIGH bit, otherwise 0.
 ******************************************************************************/
static uint8_t DHT22_ReadBit(void)
{
    uint16_t start;
    uint16_t pulsewidth;

    /* Wait for the beginning of the HIGH pulse */
    start = TIM1->CNT;

    while (!(GPIOA->IDR & (1U << DHT22_PIN)))
    {
        if ((uint16_t)(TIM1->CNT - start) > 1000U)
            return 0U;
    }

    start = TIM1->CNT;

    /* Measure the HIGH pulse width */
    while (GPIOA->IDR & (1U << DHT22_PIN))
    {
        if ((uint16_t)(TIM1->CNT - start) > 1000U)
            return 0U;
    }

    pulsewidth = (uint16_t)(TIM1->CNT - start);

    /* Long HIGH pulse represents logic HIGH */
    return (pulsewidth > 50U) ? 1U : 0U;
}

/******************************************************************************
 * @brief Reads all 40 bits of DHT22 sensor data.
 *
 * @param data Pointer to the five-byte data buffer.
 *
 * @return 1 when all data bits are read successfully.
 ******************************************************************************/
static uint8_t DHT22_Data(uint8_t *data)
{
    for (uint8_t i = 0U; i < 5U; i++)
    {
        uint8_t byte = 0U;

        for (uint8_t j = 0U; j < 8U; j++)
        {
            uint8_t bit = DHT22_ReadBit();
            byte = (byte << 1) | bit;
        }

        data[i] = byte;
    }

    return 1U;
}

/******************************************************************************
 * @brief Converts raw humidity data to percentage.
 *
 * @param data Pointer to the five-byte DHT22 data buffer.
 *
 * @return Relative humidity in percent.
 ******************************************************************************/
static float GetHumidity(uint8_t *data)
{
    uint16_t humidityraw = ((uint16_t)data[0] << 8) | data[1];

    return humidityraw / 10.0f;
}

/******************************************************************************
 * @brief Converts raw temperature data to degrees Celsius.
 *
 * @param data Pointer to the five-byte DHT22 data buffer.
 *
 * @return Temperature in degrees Celsius.
 ******************************************************************************/
static float GetTemperature(uint8_t *data)
{
    uint16_t tempraw = ((uint16_t)data[2] << 8) | data[3];

    /* Check sign bit */
    if (tempraw & (1U << 15))
    {
        tempraw &= ~(1U << 15);
        return -(tempraw / 10.0f);
    }

    return tempraw / 10.0f;
}

/******************************************************************************
 * @brief Reads and validates a complete DHT22 measurement.
 *
 * @param humidity Pointer to store the measured relative humidity.
 * @param temp     Pointer to store the measured temperature in Celsius.
 *
 * @return 1 if the measurement is valid, otherwise 0.
 ******************************************************************************/
uint8_t DHT22_Read(float *humidity, float *temp)
{
    uint8_t data[5];
    uint16_t checksum;

    DHT22_Start();

    /* Check sensor response */
    if (!DHT22_Response())
        return 0U;

    /* Read 40-bit sensor data */
    if (!DHT22_Data(data))
        return 0U;

    /* Validate checksum */
    checksum = data[0] + data[1] + data[2] + data[3];

    if ((checksum & 0xFFU) != data[4])
        return 0U;

    /* Convert and return sensor values */
    *humidity = GetHumidity(data);
    *temp = GetTemperature(data);

    return 1U;
}
