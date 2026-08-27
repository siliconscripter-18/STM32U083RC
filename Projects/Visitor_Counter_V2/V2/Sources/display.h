/******************************************************************************
 * @file    display.h
 * @brief   OLED display interface for the Visitor Counter application.
 *
 *          Declares the application-level display functions used to show
 *          the startup screen, static interface layout, and live visitor
 *          statistics.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define SPLASH_SCREEN_TIME_MS    2000U


/******************************************************************************
 * @brief Displays the startup splash screen.
 ******************************************************************************/
void SSD1306_ShowSplash(void);


/******************************************************************************
 * @brief Displays the static OLED interface layout.
 ******************************************************************************/
void SSD1306_ShowLayout(void);


/******************************************************************************
 * @brief Updates the visitor statistics on the OLED display.
 *
 * @param InCount   Total number of recorded entries.
 * @param OutCount  Total number of recorded exits.
 * @param Present   Current number of visitors inside the room.
 ******************************************************************************/
void Display_UpdateVisitorCounts(uint16_t InCount,
                                 uint16_t OutCount,
                                 uint16_t Present);

#endif /* DISPLAY_H */
