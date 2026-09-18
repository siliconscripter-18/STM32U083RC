/******************************************************************************
 * @file    tim1.c
 * @brief   TIM1 timer driver for microsecond and millisecond delays.
 *
 *          Configures TIM1 to provide a 1 us timer resolution and provides
 *          blocking delay functions for precise timing requirements such as
 *          DHT22 communication.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"


/******************************************************************************
 * @brief Initializes TIM1 for 1 us timer resolution.
 *
 *        Assumes a 4 MHz timer clock. The prescaler divides the clock by
 *        4, resulting in a 1 MHz timer frequency.
 ******************************************************************************/
void TIM1_Init(void)
{
    /* Enable TIM1 peripheral clock */
    RCC->APBENR2 |= RCC_APBENR2_TIM1EN;

    /* Disable counter and configure up-counting mode */
    TIM1->CR1 &= ~(TIM_CR1_CEN | TIM_CR1_DIR | TIM_CR1_ARPE);

    /* Prescaler for 1 us timer resolution */
    TIM1->PSC = 3U;

    /* Maximum auto-reload value */
    TIM1->ARR = 0xFFFFU;

    /* Generate update event to load prescaler and ARR */
    TIM1->EGR |= TIM_EGR_UG;

    /* Clear update flag */
    TIM1->SR &= ~TIM_SR_UIF;

    /* Enable counter */
    TIM1->CR1 |= TIM_CR1_CEN;
}


/******************************************************************************
 * @brief Provides a blocking delay in microseconds using TIM1.
 *
 * @param us Delay duration in microseconds.
 ******************************************************************************/
void delay_us(uint16_t us)
{
    uint16_t startCnt = TIM1->CNT;

    while ((uint16_t)(TIM1->CNT - startCnt) < us)
    {
    }
}


/******************************************************************************
 * @brief Provides a blocking delay in milliseconds using TIM1.
 *
 * @param ms Delay duration in milliseconds.
 ******************************************************************************/
void delay_ms(uint16_t ms)
{
    while (ms--)
    {
        delay_us(1000U);
    }
}
