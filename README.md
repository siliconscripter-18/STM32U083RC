\# STM32U083RC Register-Level Projects



Bare-metal embedded systems projects developed using the STM32 NUCLEO-U083RC.



The projects focus on direct peripheral register programming without using the STM32 HAL.



\## Projects



\### UART GPIO Motor Driver



Register-level UART, GPIO, button and DC motor control implementation.



Features include:



\- USART2 transmit and receive

\- UART command processing

\- GPIO LED control

\- Push-button status reading

\- L298N DC motor forward, reverse and stop control

\- Modular driver structure



Project folder:



`UART\_GPIO\_MOTOR\_v1`



\### I2C SSD1306 OLED Driver



Register-level I2C1 driver and SSD1306 OLED display implementation.



Features include:



\- I2C1 master transmission

\- I2C status flag handling

\- SSD1306 initialization

\- OLED screen clearing

\- Cursor positioning

\- 5x7 printable ASCII font table

\- Character and string rendering

\- Modular I2C and OLED drivers



Project folder:



`I2C\_SSD1306\_OLED`



\## Development Board



STM32 NUCLEO-U083RC



\## Development Approach



\- Bare-metal C

\- Direct register access

\- No STM32 HAL

\- Modular peripheral drivers

\- Hardware-level peripheral learning



\## Tools



\- STM32CubeIDE

\- ARM GCC toolchain

\- Git and GitHub

