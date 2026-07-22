#include "hc-sr04.h"
#include "timer3.h"
#include "registers.h"

void Ultrasonic_Init(void)
{
    RCC_IOPENR |= (1 << GPIOC_EN);

    /* PC2 - TRIG Output */
    GPIOC_MODER &= ~(3 << (PC2 * 2));
    GPIOC_MODER |=  (1 << (PC2 * 2));

    /* PC3 - ECHO Input */
    GPIOC_MODER &= ~(3 << (PC3 * 2));

    /* Internal Pull-down */
    GPIOC_PUPDR &= ~(3 << (PC3 * 2));
    GPIOC_PUPDR |=  (2 << (PC3 * 2));
}

static void Ultrasonic_Trigger(void)
{
    /* Ensure LOW */
    GPIOC_BSRR = (1 << (PC2 + 16));
    delay_us(2);

    /* 10 us HIGH pulse */
    GPIOC_BSRR = (1 << PC2);
    delay_us(10);

    /* Back LOW */
    GPIOC_BSRR = (1 << (PC2 + 16));
}

static uint8_t Ultrasonic_Echo(uint16_t *pulsewidth)
{
    uint16_t start;
    uint16_t echo_start;
    uint16_t echo_end;

    /* Wait for ECHO HIGH */
    start = TIM3_CNT;

    while (!(GPIOC_IDR & (1 << PC3)))
    {
        if ((uint16_t)(TIM3_CNT - start) > 30000)
        {
            return 0;
        }
    }

    echo_start = TIM3_CNT;

    /* Wait for ECHO LOW */
    while (GPIOC_IDR & (1 << PC3))
    {
        if ((uint16_t)(TIM3_CNT - echo_start) > 30000)
        {
            return 0;
        }
    }

    echo_end = TIM3_CNT;

    *pulsewidth = (uint16_t)(echo_end - echo_start);

    return 1;
}

/* Convert echo pulse width (us) to distance (cm) */
static uint16_t GetDistance(uint16_t pulsewidth)
{
    return pulsewidth / 58;
}

uint8_t UltraSonicMeasure(uint16_t *distance)
{
    uint16_t pulsewidth;

    Ultrasonic_Trigger();

    if (!Ultrasonic_Echo(&pulsewidth))
    {
        return 0;
    }

    *distance = GetDistance(pulsewidth);

    return 1;
}
