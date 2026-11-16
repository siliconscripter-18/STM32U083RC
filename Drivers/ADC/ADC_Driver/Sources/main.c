/******************************************************************************
 * @file    main.c
 * @brief   ADC voltage measurement and UART transmission.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include <stdio.h>
#include <stdlib.h>
#include "adc.h"
#include "uart.h"
#include "tim1.h"

uint8_t wholevolts;
uint16_t decimalparts;
uint16_t adc_value;
uint32_t voltage_mv;

char buffer[50];

/******************************************************************************
 * @brief Main application entry point.
 *
 * Initializes TIM1, ADC and UART peripherals. Continuously reads the ADC
 * value, calculates the corresponding input voltage, and transmits the
 * ADC reading and voltage through UART at one-second intervals.
 ******************************************************************************/
int main(void)
{
    TIM1_Init();
    ADC_Init();
    UART_Init();

    while(1)
    {
        /* Read ADC conversion result */
        adc_value = ADC_Read();

        /* Convert ADC value into millivolts */
        voltage_mv = ((adc_value * 3300) / 4095);

        /* Extract whole and fractional voltage parts */
        wholevolts = voltage_mv / 1000;
        decimalparts = voltage_mv % 1000;

        /* Format ADC value and voltage into a string */
        snprintf(buffer, sizeof(buffer),"ADC_Value: %d\tVoltage: %d.%03d\r\n", adc_value, wholevolts, decimalparts);

        /* Transmit ADC reading and voltage through UART */
        UART_SendString(buffer);
      
        /* Wait for one second before the next reading */
        delay_ms(1000);
    }
}