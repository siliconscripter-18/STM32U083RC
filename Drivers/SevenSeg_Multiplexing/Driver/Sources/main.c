
/******************************************************************************
 * @file    main.c
 * @brief   Demonstrates a two-digit multiplexed seven-segment display.
 *
 *          Initializes the SysTick timer and seven-segment display drivers.
 *          Counts from 0 to 10 at one-second intervals and repeats
 *          continuously while refreshing the display through multiplexing.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "SysTick.h"
#include "SevenSeg.h"


/******************************************************************************
 * @brief  Application entry point.
 *
 *         Initializes the system tick and seven-segment display GPIOs.
 *         The main loop refreshes the display continuously and increments
 *         the displayed number every second.
 ******************************************************************************/
int main(void)
{
    /* Initialize SysTick to generate a 1 ms periodic interrupt */
    SysTick_Init();

    /* Initialize segment GPIOs and digit control pins */
    SevenSeg_Init();
    BaseSwitch_Init();

    /* Initialize the displayed number and counting timestamp */
    uint8_t number = 0;
    uint32_t last_count_tick = 0;

    while (1)
    {
        /* Refresh the active digit to maintain display multiplexing */
        SevenSeg_MultiplexTask();

        /* Check whether one second has elapsed */
        if ((uint32_t)(SysTick_ms - last_count_tick) >= 1000U)
        {
            /* Record the timestamp of the current count update */
            last_count_tick = SysTick_ms;

            /* Increment the displayed number */
            number++;

            /* Reset the count after reaching 99 */
            if (number > 99U)
            {
                number = 0;
            }

            /* Update the number displayed by the seven-segment driver */
            SevenSeg_SetNumber(number);
        }
    }
}
