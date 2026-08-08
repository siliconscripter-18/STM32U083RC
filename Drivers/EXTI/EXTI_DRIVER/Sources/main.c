/******************************************************************************
 * @file    main.c
 * @brief   EXTI driver test application.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "EXTI.h"
#include "GPIO.h"

/******************************************************************************
 * @brief Main application entry point.
 ******************************************************************************/
int main(void)
{
    /* Initialize GPIO pins */
    GPIO_Init();

    /* Initialize EXTI interrupt */
    EXTI_Init();

    /* Main application loop */
    while (1)
    {
    }
}