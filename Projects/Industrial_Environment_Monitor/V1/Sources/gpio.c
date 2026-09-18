/******************************************************************************
 * @file    gpio.c
 * @brief   GPIO configuration for the Industrial Environment Monitor.
 *
 *          Configures IR sensor inputs, motor control outputs, and the
 *          room-light test LED.
 *
 *          PA0 -> S1 -> Entry IR sensor
 *          PA1 -> S2 -> Exit IR sensor
 *          PA6 -> Room light test LED
 *          PB1 -> L298N IN1
 *          PB2 -> L298N IN2
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "gpio.h"

/******************************************************************************
 * @brief Initializes GPIO pins used by the monitoring system.
 ******************************************************************************/
void GPIO_Init(void)
{
    /* Enable GPIOA and GPIOB clocks */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* PA0 = Entry IR sensor input with pull-up */
    GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;

    /* PA1 = Exit IR sensor input with pull-up */
    GPIOA->MODER &= ~GPIO_MODER_MODE1_Msk;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_0;

    /* PB1 = L298N IN1 */
    GPIOB->MODER &= ~GPIO_MODER_MODE1_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE1_0;

    /* PB2 = L298N IN2 */
    GPIOB->MODER &= ~GPIO_MODER_MODE2_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE2_0;

    /* Set motor direction: IN1 HIGH, IN2 LOW */
    GPIOB->BSRR = GPIO_BSRR_BS1;
    GPIOB->BSRR = GPIO_BSRR_BR2;

    /* PA6 = room light test LED output */
    GPIOA->MODER &= ~GPIO_MODER_MODE6_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE6_0;

    /* LED initially OFF */
    GPIOA->BSRR = GPIO_BSRR_BR6;
}
