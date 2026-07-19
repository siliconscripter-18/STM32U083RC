#include "timer3.h"
#include "registers.h"

/* Timer counter value used by delay_us() */
static volatile uint16_t startCnt;

/* Configure TIM3 to generate a 1 µs timer tick. */
void TIM3_Init(void)
{
    /* Enable TIM3 peripheral clock */
    RCC_APBENR1 |= (1 << TIM3_EN);

    /* Up-counter, no auto-reload preload, counter disabled */
    TIM3_CR1 &= ~((1 << CEN) | (1 << DIR) | (1 << ARPE));

    /* Prescaler for 1 µs timer resolution */
    TIM3_PSC = 3;

    /* Maximum auto-reload value */
    TIM3_ARR = 65535;

    /* Load PSC and ARR into active registers */
    TIM3_EGR |= (1 << UG);

    /* Clear update flag */
    TIM3_SR &= ~(1 << UIF);

    /* Enable counter */
    TIM3_CR1 |= (1 << CEN);
}

/* Generate a blocking delay in microseconds. */
void delay_us(uint16_t us)
{
    startCnt = TIM3_CNT;

    while ((uint16_t)(TIM3_CNT - startCnt) < us);
}

/* Generate a blocking delay in milliseconds. */
void delay_ms(uint16_t ms)
{
    while (ms--)
    {
        delay_us(1000);
    }
}
