#include <stdint.h>
#include "ssd1306.h"
#include "i2c.h"
#include "font5x7.h"

// --------------------------------------------------------
// SSD1306 init sequence
void SSD1306_Init(void) {

    uint8_t cmds[][3] = {
        {0x00, 0xAE},          // Display OFF
        {0x00, 0xD5, 0x80},    // Clock
        {0x00, 0xA8, 0x3F},    // Multiplex
        {0x00, 0xD3, 0x00},    // Offset
        {0x00, 0x40},          // Start line
        {0x00, 0x8D, 0x14},    // Charge pump
        {0x00, 0x20, 0x00},    // Horizontal addressing
        {0x00, 0xA1},          // Segment remap
        {0x00, 0xC8},          // COM scan dir
        {0x00, 0xDA, 0x12},    // COM pins
        {0x00, 0x81, 0x7F},    // Contrast
        {0x00, 0xA4},          // Resume
        {0x00, 0xA6},          // Normal
        {0x00, 0xAF}           // Display ON
    };

    for(int i=0;i<14;i++) {

        int len = (cmds[i][1]==0xD5 || cmds[i][1]==0xA8 || cmds[i][1]==0xD3 ||
                   cmds[i][1]==0x8D || cmds[i][1]==0x20 || cmds[i][1]==0xDA ||
                   cmds[i][1]==0x81) ? 3 : 2;

        I2C1_Send(SSD1306_ADDR, cmds[i], len);
    }
}

// --------------------------------------------------------
// Clear screen
void SSD1306_Clear(void) {

    uint8_t clear[129];
    clear[0] = 0x40;

    for(int i=1; i<129; i++){
    	clear[i] = 0x00;
    }

    for(int page=0; page<8; page++) {
        uint8_t set_page[] = {0x00, (0xB0 | page), 0x00, 0x00};
        I2C1_Send(SSD1306_ADDR, set_page, 4);
        I2C1_Send(SSD1306_ADDR, clear, 129);
    }
}

// --------------------------------------------------------
// Set cursor position
void SSD1306_SetCursor(uint8_t col, uint8_t page) {

    uint8_t cmds[] = {0x00, 0x21, col, 0x7F, 0x00, 0x22, page, 0x07};
    I2C1_Send(SSD1306_ADDR, cmds, sizeof(cmds));
}

// --------------------------------------------------------
// Draw a single character
void SSD1306_DrawChar(char c) {

    if(c < 32 || c > 126)
    {
    	c = ' '; // if below 32 and above 126 char not matched with font table so show space instead of garbage
    }

    const uint8_t *bitmap = font5x7[c - 32];
    uint8_t buf[6];
    buf[0] = 0x40;

    for(int i=0;i<5;i++)
    {
    	buf[i+1] = bitmap[i];
    }

    I2C1_Send(SSD1306_ADDR, buf, 6);
    uint8_t space[2] = {0x40, 0x00}; // spacing column
    I2C1_Send(SSD1306_ADDR, space, 2);
}

// --------------------------------------------------------
// Draw a string
void SSD1306_DrawString(const char *s) {
    while(*s) {
        SSD1306_DrawChar(*s++);
    }
}
