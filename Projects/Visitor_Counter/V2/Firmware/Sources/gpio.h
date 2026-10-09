/******************************************************************************
 * @file    gpio.h
 * @brief   GPIO interface for the Visitor Counter application.
 *
 *          Declares the GPIO initialization routine and control functions
 *          for the entry/exit warning LEDs and buzzer.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>


/* Default warning indication duration */
#define WARNING_LED_TIME_MS    1000U


/******************************************************************************
 * @brief Initializes GPIO pins used by the Visitor Counter.
 ******************************************************************************/
void GPIO_Init(void);


/******************************************************************************
 * @brief Turns on the entry warning LED.
 ******************************************************************************/
void Entry_Warning_LED_On(void);


/******************************************************************************
 * @brief Turns off the entry warning LED.
 ******************************************************************************/
void Entry_Warning_LED_Off(void);


/******************************************************************************
 * @brief Turns on the exit warning LED.
 ******************************************************************************/
void Exit_Warning_LED_On(void);


/******************************************************************************
 * @brief Turns off the exit warning LED.
 ******************************************************************************/
void Exit_Warning_LED_Off(void);


/******************************************************************************
 * @brief Turns the buzzer on.
 ******************************************************************************/
void Buzzer_On(void);


/******************************************************************************
 * @brief Turns the buzzer off.
 ******************************************************************************/
void Buzzer_Off(void);


/******************************************************************************
 * @brief Generates a buzzer beep for the specified duration.
 *
 * @param duration Beep duration in milliseconds.
 ******************************************************************************/
void Buzzer_Beep(uint32_t duration);


#endif /* GPIO_H */
