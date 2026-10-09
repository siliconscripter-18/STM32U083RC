/******************************************************************************
 * @file    gpio.h
 * @brief   GPIO driver interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef GPIO_H
#define GPIO_H

/* Initializes GPIO pins */
void GPIO_Init(void);

/* Turns the status LED ON */
void LED_On(void);

/* Turns the status LED OFF */
void LED_Off(void);

/* Generates a buzzer beep */
void Buzzer_Beep(void);

#endif /* GPIO_H */
