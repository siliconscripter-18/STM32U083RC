/******************************************************************************
 * @file    gpio.c
 * @brief   GPIO driver for IR sensors, warning LEDs, and buzzer.
 *
 *          Configures the four IR sensor inputs and the GPIO outputs used for
 *          entry/exit warning indication and the audible buzzer.
 *
 *          Sensor mapping:
 *          PA0 -> S1 (Entry sensor)
 *          PA1 -> S2 (Entry sensor)
 *          PA6 -> S3 (Exit sensor)
 *          PA7 -> S4 (Exit sensor)
 *
 *          Output mapping:
 *          PB4 -> Entry warning LED
 *          PB5 -> Exit warning LED
 *          PB6 -> Buzzer
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "gpio.h"
#include "SysTick.h"


/******************************************************************************
 * @brief Initializes GPIO pins used by the Visitor Counter.
 *
 *          The IR sensors are configured as inputs with internal pull-up
 *          resistors. Warning LEDs are active-high outputs, while the buzzer
 *          is controlled as an active-low output.
 ******************************************************************************/
void GPIO_Init(void)
{
    /* Enable GPIOA and GPIOB peripheral clocks */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;


    /*
     * Configure IR sensor inputs.
     *
     * PA0 -> S1
     * PA1 -> S2
     * PA6 -> S3
     * PA7 -> S4
     */
    GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
    GPIOA->MODER &= ~GPIO_MODER_MODE1_Msk;
    GPIOA->MODER &= ~GPIO_MODER_MODE6_Msk;
    GPIOA->MODER &= ~GPIO_MODER_MODE7_Msk;


    /* Enable internal pull-ups for all IR sensor inputs */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD0_0;

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_0;

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD6_0;

    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD7_0;


    /*
     * Configure warning LEDs and buzzer as GPIO outputs.
     *
     * PB4 -> Entry warning LED
     * PB5 -> Exit warning LED
     * PB6 -> Buzzer
     */
    GPIOB->MODER &= ~GPIO_MODER_MODE4_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE4_0;

    GPIOB->MODER &= ~GPIO_MODER_MODE5_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE5_0;

    GPIOB->MODER &= ~GPIO_MODER_MODE6_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE6_0;


    /* Ensure all warning outputs start in the inactive state */
    GPIOB->BSRR = GPIO_BSRR_BR4;
    GPIOB->BSRR = GPIO_BSRR_BR5;
    GPIOB->BSRR = GPIO_BSRR_BS6;
}


/******************************************************************************
 * @brief Turns on the entry warning LED.
 ******************************************************************************/
void Entry_Warning_LED_On(void)
{
    GPIOB->BSRR = GPIO_BSRR_BS4;
}


/******************************************************************************
 * @brief Turns off the entry warning LED.
 ******************************************************************************/
void Entry_Warning_LED_Off(void)
{
    GPIOB->BSRR = GPIO_BSRR_BR4;
}


/******************************************************************************
 * @brief Turns on the exit warning LED.
 ******************************************************************************/
void Exit_Warning_LED_On(void)
{
    GPIOB->BSRR = GPIO_BSRR_BS5;
}


/******************************************************************************
 * @brief Turns off the exit warning LED.
 ******************************************************************************/
void Exit_Warning_LED_Off(void)
{
    GPIOB->BSRR = GPIO_BSRR_BR5;
}


/******************************************************************************
 * @brief Turns the buzzer on.
 *
 *        The buzzer module is controlled as an active-low output.
 ******************************************************************************/
void Buzzer_On(void)
{
    GPIOB->BSRR = GPIO_BSRR_BR6;
}


/******************************************************************************
 * @brief Turns the buzzer off.
 ******************************************************************************/
void Buzzer_Off(void)
{
    GPIOB->BSRR = GPIO_BSRR_BS6;
}


/******************************************************************************
 * @brief Generates a blocking buzzer beep for the specified duration.
 *
 * @param duration Beep duration in milliseconds.
 ******************************************************************************/
void Buzzer_Beep(uint32_t duration)
{
    Buzzer_On();

    Delay_ms(duration);

    Buzzer_Off();
}
