/******************************************************************************
 * @file    ssd1306.c
 * @brief   SSD1306 OLED display driver for the Visitor Counter application.
 *
 *          Provides low-level functions for initializing the SSD1306 OLED,
 *          clearing the display, positioning the cursor, and rendering ASCII
 *          characters and strings using the 5x7 font.
 *
 *          Communication with the display is performed through I2C1.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>

#include "ssd1306.h"
#include "i2c.h"
#include "font5x7.h"


/******************************************************************************
 * @brief Initializes the SSD1306 OLED display.
 *
 *        Sends the required initialization command sequence, clears the
 *        display RAM, and finally enables the OLED display.
 ******************************************************************************/
void SSD1306_Init(void)
{
    /* SSD1306 initialization command sequence */
    uint8_t cmds[][3] =
    {
        {0x00, 0xAE},          /* Display OFF */
        {0x00, 0xD5, 0x80},    /* Display clock divide ratio */
        {0x00, 0xA8, 0x3F},    /* Multiplex ratio */
        {0x00, 0xD3, 0x00},    /* Display offset */
        {0x00, 0x40},          /* Display start line */
        {0x00, 0x8D, 0x14},    /* Charge pump enable */
        {0x00, 0x20, 0x00},    /* Horizontal addressing mode */
        {0x00, 0xA1},          /* Segment remap */
        {0x00, 0xC8},          /* COM scan direction */
        {0x00, 0xDA, 0x12},    /* COM pins configuration */
        {0x00, 0x81, 0x7F},    /* Contrast */
        {0x00, 0xA4},          /* Resume RAM display */
        {0x00, 0xA6},          /* Normal display */
    };


    /* Send initialization commands */
    for (int i = 0; i < 13; i++)
    {
        int len = (cmds[i][1] == 0xD5 ||
                   cmds[i][1] == 0xA8 ||
                   cmds[i][1] == 0xD3 ||
                   cmds[i][1] == 0x8D ||
                   cmds[i][1] == 0x20 ||
                   cmds[i][1] == 0xDA ||
                   cmds[i][1] == 0x81) ? 3 : 2;

        I2C1_Send(SSD1306_ADDR, cmds[i], len);
    }


    /* Clear display memory before enabling the display */
    SSD1306_SetCursor(0, 0);
    SSD1306_Clear();


    /* Turn the display ON */
    uint8_t display_on[] = {0x00, 0xAF};

    I2C1_Send(SSD1306_ADDR, display_on, 2);
}


/******************************************************************************
 * @brief Clears the entire OLED display.
 *
 *        Writes zero data to all eight display pages.
 ******************************************************************************/
void SSD1306_Clear(void)
{
    /* Control byte followed by 128 bytes of display data */
    uint8_t clear[129];

    clear[0] = 0x40;

    for (int i = 1; i < 129; i++)
    {
        clear[i] = 0x00;
    }


    /* Clear all eight 8-pixel display pages */
    for (int page = 0; page < 8; page++)
    {
        uint8_t set_page[] =
        {
            0x00,
            (0xB0 | page),
            0x00,
            0x00
        };

        I2C1_Send(SSD1306_ADDR, set_page, 4);
        I2C1_Send(SSD1306_ADDR, clear, 129);
    }
}


/******************************************************************************
 * @brief Sets the OLED cursor position and display addressing range.
 *
 * @param col  Starting display column (0-127).
 * @param page Starting display page (0-7).
 ******************************************************************************/
void SSD1306_SetCursor(uint8_t col, uint8_t page)
{
    uint8_t cmds[] =
    {
        0x00,
        0x21,
        col,
        0x7F,
        0x22,
        page,
        0x07
    };

    I2C1_Send(SSD1306_ADDR, cmds, sizeof(cmds));
}


/******************************************************************************
 * @brief Draws a single ASCII character.
 *
 * @param c Character to display.
 *
 *        Characters outside the supported printable ASCII range are replaced
 *        with a blank space.
 ******************************************************************************/
void SSD1306_DrawChar(char c)
{
    /* Replace unsupported characters with a space */
    if (c < 32 || c > 126)
    {
        c = ' ';
    }


    /* Get the 5x7 bitmap for the character */
    const uint8_t *bitmap = font5x7[c - 32];

    /* Control byte + five font columns */
    uint8_t buf[6];

    buf[0] = 0x40;

    for (int i = 0; i < 5; i++)
    {
        buf[i + 1] = bitmap[i];
    }

    I2C1_Send(SSD1306_ADDR, buf, 6);


    /* Add one blank column between characters */
    uint8_t space[2] = {0x40, 0x00};

    I2C1_Send(SSD1306_ADDR, space, 2);
}


/******************************************************************************
 * @brief Draws a horizontally scaled ASCII character.
 *
 * @param c Character to display.
 *
 *        Each font column is transmitted twice, producing a character that
 *        is approximately twice as wide as the standard 5x7 character.
 ******************************************************************************/
void SSD1306_DrawCharScaled(char c)
{
    /* Replace unsupported characters with a space */
    if (c < 32 || c > 126)
    {
        c = ' ';
    }


    /* Get the 5x7 bitmap for the character */
    const uint8_t *bitmap = font5x7[c - 32];

    /* Control byte + ten duplicated font columns */
    uint8_t buf[11];

    buf[0] = 0x40;

    int i;
    int j;

    for (i = 0, j = 1; i < 5; i++)
    {
        buf[j]     = bitmap[i];
        buf[j + 1] = bitmap[i];

        j += 2;
    }

    I2C1_Send(SSD1306_ADDR, buf, 11);


    /* Add one blank column after the scaled character */
    uint8_t space[2] = {0x40, 0x00};

    I2C1_Send(SSD1306_ADDR, space, 2);
}


/******************************************************************************
 * @brief Draws a null-terminated ASCII string.
 *
 * @param s Pointer to the null-terminated string.
 ******************************************************************************/
void SSD1306_DrawString(const char *s)
{
    while (*s)
    {
        SSD1306_DrawChar(*s++);
    }
}


/******************************************************************************
 * @brief Draws a horizontally scaled null-terminated ASCII string.
 *
 * @param s Pointer to the null-terminated string.
 ******************************************************************************/
void SSD1306_DrawStringScaled(const char *s)
{
    while (*s)
    {
        SSD1306_DrawCharScaled(*s++);
    }
}
