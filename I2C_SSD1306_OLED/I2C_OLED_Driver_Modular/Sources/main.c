#include "i2c.h"
#include "ssd1306.h"
#include "font5x7.h"

int main(void) {
    // Initialize I2C1 hardware
    I2C1_Init();

    // Initialize OLED
    SSD1306_Init();
    SSD1306_Clear();

    // Position cursor at column 0, page 0
    SSD1306_SetCursor(0, 0);

    // Draw any string you like
    SSD1306_DrawString("STM32 ROCKS 123!");

    while(1) {
        // main loop
    }
}
