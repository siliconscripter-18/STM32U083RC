/******************************************************************************
 * @file    display.c
 * @brief   Display interface for the Visitor Counter application.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "display.h"
#include "ssd1306.h"
#include "SysTick.h"
#include <stdio.h>

#define SPLASH_SCREEN_TIME_MS      2000U
#define DISPLAY_VALUE_BUFFER_SIZE     6U

/******************************************************************************
 * @brief Displays the startup splash screen.
 ******************************************************************************/
void SSD1306_ShowSplash(void)
{
    SSD1306_Clear();

    /* Display project title */
    SSD1306_SetCursor(32, 3);
    SSD1306_DrawStringScaled("Visitor");

    SSD1306_SetCursor(32, 5);
    SSD1306_DrawStringScaled("Counter");

    /* Keep the splash screen visible */
    Delay_ms(SPLASH_SCREEN_TIME_MS);
}

/******************************************************************************
 * @brief Displays the static visitor counter layout.
 ******************************************************************************/
void SSD1306_ShowLayout(void)
{
    SSD1306_Clear();

    /* Display title */
    SSD1306_SetCursor(24, 0);
    SSD1306_DrawString("Visitor Counter");

    /* Display field labels */
    SSD1306_SetCursor(10, 3);
    SSD1306_DrawString("IN:");

    SSD1306_SetCursor(10, 5);
    SSD1306_DrawString("OUT:");

    SSD1306_SetCursor(10, 7);
    SSD1306_DrawString("ROOM:");
}

/******************************************************************************
 * @brief Updates the visitor counts on the OLED display.
 *
 * @param InCount  Total entry count.
 * @param OutCount Total exit count.
 * @param Present  Current room occupancy.
 ******************************************************************************/
void Display_UpdateVisitorCounts(uint16_t InCount,
                                 uint16_t OutCount,
                                 uint16_t Present)
{
    char buffer[DISPLAY_VALUE_BUFFER_SIZE];

    /* Update IN count */
    snprintf(buffer, sizeof(buffer), "%u", InCount);
    SSD1306_SetCursor(30, 3);
    SSD1306_DrawString(buffer);

    /* Update OUT count */
    snprintf(buffer, sizeof(buffer), "%u", OutCount);
    SSD1306_SetCursor(36, 5);
    SSD1306_DrawString(buffer);

    /* Update ROOM occupancy */
    snprintf(buffer, sizeof(buffer), "%u", Present);
    SSD1306_SetCursor(42, 7);
    SSD1306_DrawString("     ");
    SSD1306_SetCursor(42, 7);
    SSD1306_DrawString(buffer);
}
