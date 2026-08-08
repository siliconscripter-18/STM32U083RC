/******************************************************************************
 * @file    gpio.c
 * @brief   GPIO driver for LED and user button.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "GPIO.h"
#include "stm32u083xx.h"

/******************************************************************************
 * @brief Initializes GPIOA and GPIOC pins.
 *
 * Configures PA5 as a general-purpose output for the LED and PC13 as an
 * input with an internal pull-up resistor for the user button.
 ******************************************************************************/
void GPIO_Init(void)
{
    /* Enable GPIOA and GPIOC peripheral clocks */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    /* Configure PA5 as general-purpose output */
    GPIOA->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOA->MODER |=  GPIO_MODER_MODE5_0;

    /* Configure PC13 as input */
    GPIOC->MODER &= ~GPIO_MODER_MODE13_Msk;

    /* Configure PC13 with internal pull-up */
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
    GPIOC->PUPDR |=  GPIO_PUPDR_PUPD13_0;
}
