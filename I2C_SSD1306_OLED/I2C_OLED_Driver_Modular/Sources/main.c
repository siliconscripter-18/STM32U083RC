#include "i2c.h"
#include "ssd1306.h"
#include "font5x7.h"

int main(void)
{
    I2C1_Init();

    if (I2C1_CheckDevice(SSD1306_ADDR))
    {
        SSD1306_Init();
        SSD1306_Clear();

        SSD1306_SetCursor(0, 0);
        SSD1306_DrawString("STM32 ROCKS 123!");
    }

    while (1)
    {
    }
}
