/******************************************************************************
 * @file    ssd1306.h
 * @brief   SSD1306 OLED display driver interface for the Visitor Counter.
 *
 *          Provides the public interface for initializing, clearing, positioning
 *          and rendering text on the SSD1306 OLED display over I2C1.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>


/* SSD1306 7-bit I2C slave address */
#define SSD1306_ADDR    0x3C


/******************************************************************************
 * @brief Initializes the SSD1306 OLED display.
 ******************************************************************************/
void SSD1306_Init(void);


/******************************************************************************
 * @brief Clears the entire OLED display.
 ******************************************************************************/
void SSD1306_Clear(void);


/******************************************************************************
 * @brief Sets the OLED display cursor position.
 *
 * @param col  Starting display column (0-127).
 * @param page Starting display page (0-7).
 ******************************************************************************/
void SSD1306_SetCursor(uint8_t col, uint8_t page);


/******************************************************************************
 * @brief Draws a single ASCII character using the standard 5x7 font.
 *
 * @param c Character to display.
 ******************************************************************************/
void SSD1306_DrawChar(char c);


/******************************************************************************
 * @brief Draws a horizontally scaled ASCII character.
 *
 * @param c Character to display.
 ******************************************************************************/
void SSD1306_DrawCharScaled(char c);


/******************************************************************************
 * @brief Draws a null-terminated ASCII string.
 *
 * @param s Pointer to the string to display.
 ******************************************************************************/
void SSD1306_DrawString(const char *s);


/******************************************************************************
 * @brief Draws a horizontally scaled null-terminated ASCII string.
 *
 * @param s Pointer to the string to display.
 ******************************************************************************/
void SSD1306_DrawStringScaled(const char *s);


#endif /* SSD1306_H */
