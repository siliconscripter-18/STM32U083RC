/******************************************************************************
 * @file    display.h
 * @brief   Display interface for the Visitor Counter application.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

/* Displays the startup splash screen */
void SSD1306_ShowSplash(void);

/* Displays the static OLED layout */
void SSD1306_ShowLayout(void);

/* Updates the visitor counts on the OLED display */
void Display_UpdateVisitorCounts(uint16_t InCount, uint16_t OutCount, uint16_t Present);

#endif /* DISPLAY_H */
