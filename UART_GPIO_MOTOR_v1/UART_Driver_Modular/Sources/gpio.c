/*
 * gpio.c
 * GPIO Driver Implementation
 */

#include "gpio.h"
#include "registers.h"
#include "uart.h"

/* GPIO Initialization */
void GPIO_Init(void)
{
    /* Enable clock for GPIOA and GPIOC */
    RCC_IOPENR |= (1<<GPIOAEN);
    RCC_IOPENR |= (1<<GPIOCEN);

    /* Configure PA1 as input (External Button) */
    GPIOA_MODER &= ~(3<<(PA1*2));

    /* Enable internal pull-up for PA1 */
    GPIOA_PUPDR &= ~(3<<(PA1*2));
    GPIOA_PUPDR |=  (1<<(PA1*2));

    /* Configure PA5 as output (On-board LD4) */
    GPIOA_MODER &= ~(3<<(PA5*2));
    GPIOA_MODER |=  (1<<(PA5*2));

    /* Configure PA7 as output (External LED1) */
    GPIOA_MODER &= ~(3<<(PA7*2));
    GPIOA_MODER |=  (1<<(PA7*2));

    /* Configure PA8 as output (External LED2) */
    GPIOA_MODER &= ~(3<<(PA8*2));
    GPIOA_MODER |=  (1<<(PA8*2));

    /* Configure PA9 as output (External LED3) */
    GPIOA_MODER &= ~(3<<(PA9*2));
    GPIOA_MODER |=  (1<<(PA9*2));

    /* Configure PA10 as output (External LED4) */
    GPIOA_MODER &= ~(3<<(PA10*2));
    GPIOA_MODER |=  (1<<(PA10*2));

    /* Configure PC13 as input (On-board User Button) */
    GPIOC_MODER &= ~(3<<(PC13*2));

    /* Enable internal pull-up for PC13 */
    GPIOC_PUPDR &= ~(3<<(PC13*2));
    GPIOC_PUPDR |=  (1<<(PC13*2));

    /* Configure PA15 as output (Buzzer) */
    GPIOA_MODER &= ~(3<<(PA15*2));
    GPIOA_MODER |=  (1<<(PA15*2));

    /* Turn buzzer OFF (Active LOW) */
    GPIOA_BSRR = (1<<PA15);
}

/* On-board LED (LD4) Functions */
void LD4_On(void)
{
    GPIOA_BSRR = (1<<PA5);
}

void LD4_Off(void)
{
    GPIOA_BSRR = (1<<(PA5+16));
}

void LD4_Status(void)
{
	if(GPIOA_ODR & (1<<PA5))
    {
        UART_SendString("LED(LD4) Status:ON\r\n");
    }
    else
    {
        UART_SendString("LED(LD4) Status:OFF\r\n");
    }
}

/* External LED Functions */
void LED1_On(void)
{
    GPIOA_BSRR = (1<<PA7);
}

void LED1_Off(void)
{
    GPIOA_BSRR = (1<<(PA7+16));
}

void LED2_On(void)
{
    GPIOA_BSRR = (1<<PA8);
}

void LED2_Off(void)
{
    GPIOA_BSRR = (1<<(PA8+16));
}

void LED3_On(void)
{
    GPIOA_BSRR = (1<<PA9);
}

void LED3_Off(void)
{
    GPIOA_BSRR = (1<<(PA9+16));
}

void LED4_On(void)
{
    GPIOA_BSRR = (1<<PA10);
}

void LED4_Off(void)
{
    GPIOA_BSRR = (1<<(PA10+16));
}

/* External LED Group Functions */
void AllLED_On(void)
{
    LD4_On();
    LED1_On();
    LED2_On();
    LED3_On();
    LED4_On();
}

void AllLED_Off(void)
{
    LD4_Off();
    LED1_Off();
    LED2_Off();
    LED3_Off();
    LED4_Off();
}

/* Buzzer Functions */
void Buzzer_On(void)
{
    GPIOA_BSRR = (1<<(PA15+16));
}

void Buzzer_Off(void)
{
    GPIOA_BSRR = (1<<PA15);
}

/* Button Functions */
void UserButton_Read(void)
{
    if(GPIOC_IDR & (1<<PC13))
    {
        UART_SendString("USER BUTTON RELEASED\r\n");
    }
    else
    {
        UART_SendString("USER BUTTON PRESSED\r\n");
    }
}

void ExternalButton_Read(void)
{
    if(GPIOA_IDR & (1<<PA1))
    {
        UART_SendString("EXTERNAL BUTTON RELEASED\r\n");
    }
    else
    {
        UART_SendString("EXTERNAL BUTTON PRESSED\r\n");
    }
}

