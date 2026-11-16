/******************************************************************************
 * @file    ADC.c
 * @brief   ADC driver for analog-to-digital conversion.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>
#include "stm32u083xx.h"
#include "tim1.h"
#include "adc.h"

/******************************************************************************
 * @brief Initializes the ADC peripheral.
 *
 * Configures PC0 as an analog input, enables the ADC clock and internal
 * regulator, performs ADC calibration, and enables ADC channel 0.
 ******************************************************************************/
void ADC_Init()
{
    /* Enable GPIOC peripheral clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    /* Configure PC0 as analog mode */
    GPIOC->MODER &= ~GPIO_MODER_MODE0_Msk;
    GPIOC->MODER |=  GPIO_MODER_MODE0_Msk;

    /* Enable ADC peripheral clock */
    RCC->APBENR2 |= RCC_APBENR2_ADCEN;

    /* Enable ADC internal regulator */
    ADC1->CR |= ADC_CR_ADVREGEN;

    /* Wait for regulator startup */
    delay_us(20);

    /* Start ADC calibration */
    ADC1->CR |= ADC_CR_ADCAL;

    /* Wait until calibration is complete */
    while(ADC1->CR & ADC_CR_ADCAL);

    /* Read calibration factor */
    uint32_t cal_factor = ADC1->DR & 0X7F;

    /* Convert calibration factor */
    cal_factor += 1;

    /* Store calibration factor */
    ADC1->CALFACT = cal_factor;

    /* ADC_CFGR1 default configuration:
     * RES   = 00 : 12-bit resolution
     * ALIGN = 0  : Right alignment
     * CONT  = 0  : Single conversion
     * EXTEN = 00 : Software trigger
     * DMAEN = 0  : DMA disabled
     *
     * All above settings are retained at their reset values.
     */

    /* Select ADC channel 0 */
    ADC1->CHSELR |= ADC_CHSELR_CHSEL0;

    /* Wait until channel configuration is ready */
    while(!(ADC1->ISR & ADC_ISR_CCRDY));

    /* Enable ADC peripheral */
    ADC1->CR |= ADC_CR_ADEN;

    /* Wait until ADC is ready */
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
}

/******************************************************************************
 * @brief Reads the ADC conversion result.
 *
 * Starts a single ADC conversion, waits for the end-of-conversion flag,
 * and returns the converted digital value.
 *
 * @return 16-bit ADC conversion result.
 ******************************************************************************/
uint16_t ADC_Read(void)
{
    /* Start ADC conversion */
    ADC1->CR |= ADC_CR_ADSTART;

    /* Wait until conversion is complete */
    while(!(ADC1->ISR & ADC_ISR_EOC));

    /* Read ADC conversion result */
    uint16_t adc_value = ADC1->DR;

    return adc_value;
}