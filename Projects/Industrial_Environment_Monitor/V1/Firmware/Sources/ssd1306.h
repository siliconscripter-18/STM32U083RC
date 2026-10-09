/******************************************************************************
 * @file    ssd1306.h
 * @brief   SSD1306 OLED display driver interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

/* SSD1306 I2C slave address */
#define SSD1306_ADDR    0x3C

/* Initializes the OLED display */
void SSD1306_Init(void);

/* Clears the OLED display */
void SSD1306_Clear(void);

/* Sets the display cursor position */
void SSD1306_SetCursor(uint8_t col, uint8_t page);

/* Draws a single ASCII character */
void SSD1306_DrawChar(char c);

/* Draws a horizontally scaled ASCII character */
void SSD1306_DrawCharScaled(char c);

/* Draws a null-terminated string */
void SSD1306_DrawString(const char *s);

/* Draws a horizontally scaled null-terminated string */
void SSD1306_DrawStringScaled(const char *s);

#endif /* SSD1306_H */
