\# I2C SSD1306 OLED Driver



Register-level I2C and SSD1306 OLED driver developed for the STM32 NUCLEO-U083RC without using HAL.



\## Features



\- I2C1 register-level initialization

\- I2C master transmit implementation

\- BUSY, TXIS, NACKF and TC flag handling

\- SSD1306 OLED initialization

\- OLED screen clear

\- Cursor positioning

\- 5x7 full printable ASCII font table

\- Character rendering

\- String rendering



\## Hardware



\- STM32 NUCLEO-U083RC

\- SSD1306 128x64 I2C OLED



\## I2C Configuration



\- I2C Peripheral: I2C1

\- SCL: PB8

\- SDA: PB9

\- Alternate Function: AF4

\- OLED I2C Address: 0x3C

\- I2C Timing Register: 0x00303D5B



\## Project Structure



\- `i2c.c / i2c.h` - Register-level I2C driver

\- `ssd1306.c / ssd1306.h` - SSD1306 OLED driver

\- `font5x7.c / font5x7.h` - 5x7 printable ASCII font table

\- `registers.h` - STM32 peripheral register definitions

\- `main.c` - Application entry point



\## Implementation



The project directly configures STM32 registers for GPIO and I2C operation. The SSD1306 driver sends command and display data through the custom I2C transmit function.



No STM32 HAL is used.

