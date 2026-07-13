#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>

// SSD1306 I2C address
#define SSD1306_ADDR 0x3C

// Initialize the OLED display
void SSD1306_Init(void);

// Clear the entire screen
void SSD1306_Clear(void);

// Set cursor position (column 0–127, page 0–7)
void SSD1306_SetCursor(uint8_t col, uint8_t page);

// Draw a single character
void SSD1306_DrawChar(char c);

// Draw a string
void SSD1306_DrawString(const char *s);



#endif // SSD1306_H
