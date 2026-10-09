/******************************************************************************
 * @file    gpio.c
 * @brief   GPIO driver for IR sensors, LED, and buzzer.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "gpio.h"
#include "SysTick.h"

/******************************************************************************
 * @brief Initializes GPIO pins for IR sensors, LED, and buzzer.
 ******************************************************************************/
void GPIO_Init(void)
{
    /* Enable GPIOA and GPIOB peripheral clocks */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Configure PA0 and PA1 as input pins */
    GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
    GPIOA->MODER &= ~GPIO_MODER_MODE1_Msk;

    /* Enable pull-up resistor for PA0 */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;

    /* Enable pull-up resistor for PA1 */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_0;

    /* Configure PB4 as LED output */
    GPIOB->MODER &= ~GPIO_MODER_MODE4_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE4_0;

    /* Configure PB5 as buzzer output */
    GPIOB->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE5_0;

    /* Keep buzzer OFF initially */
    GPIOB->BSRR = GPIO_BSRR_BS5;
}

/******************************************************************************
 * @brief Turns the status LED ON.
 ******************************************************************************/
void LED_On(void)
{
    GPIOB->BSRR = GPIO_BSRR_BS4;
}

/******************************************************************************
 * @brief Turns the status LED OFF.
 ******************************************************************************/
void LED_Off(void)
{
    GPIOB->BSRR = GPIO_BSRR_BR4;
}

/******************************************************************************
 * @brief Generates a buzzer beep.
 ******************************************************************************/
void Buzzer_Beep(void)
{
    /* Turn the buzzer ON */
    GPIOB->BSRR = GPIO_BSRR_BR5;

    /* Keep the buzzer active for 500 ms */
    Delay_ms(500);

    /* Turn the buzzer OFF */
    GPIOB->BSRR = GPIO_BSRR_BS5;
}
