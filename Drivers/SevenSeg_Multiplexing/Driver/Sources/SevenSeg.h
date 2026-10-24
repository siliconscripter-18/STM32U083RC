
/******************************************************************************
 * @file    SevenSeg.h
 * @brief   Function declarations for the seven-segment display driver.
 *
 *          Provides interfaces for initializing the segment GPIOs and digit
 *          control pins, blanking the display, setting the displayed number,
 *          and refreshing the display through multiplexing.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SEVENSEG_H
#define SEVENSEG_H

#include <stdint.h>


/******************************************************************************
 * @brief Initializes GPIO pins PB0-PB6 for seven-segment control.
 ******************************************************************************/
void SevenSeg_Init(void);


/******************************************************************************
 * @brief Initializes GPIO pins PB11 and PB12 for digit selection.
 ******************************************************************************/
void BaseSwitch_Init(void);


/******************************************************************************
 * @brief Switches off all seven-segment outputs.
 ******************************************************************************/
void SevenSeg_Blank(void);


/******************************************************************************
 * @brief Sets the number to be displayed.
 *
 * @param number Number to display, in the range 0-99.
 ******************************************************************************/
void SevenSeg_SetNumber(uint8_t number);


/******************************************************************************
 * @brief Refreshes one digit of the multiplexed display.
 *
 *        Must be called continuously from the main loop to maintain
 *        the display.
 ******************************************************************************/
void SevenSeg_MultiplexTask(void);

#endif /* SEVENSEG_H */
