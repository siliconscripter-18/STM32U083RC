# STM32U083RC Register-Level Projects

Bare-metal embedded systems projects developed using the **STM32 NUCLEO-U083RC** development board.

The projects focus on direct peripheral register programming without using the STM32 HAL library.

---

## Projects

### UART GPIO Motor Driver

Register-level UART, GPIO, button, and DC motor control implementation.

Features include:

- USART2 transmit and receive
- UART command processing
- GPIO LED control
- Push-button status reading
- L298N DC motor forward, reverse, and stop control
- Modular driver structure

Project folder:

`UART_GPIO_MOTOR_v1`

---

### I2C SSD1306 OLED Driver

Register-level I2C1 driver and SSD1306 OLED display implementation.

Features include:

- I2C1 master transmission
- I2C status flag handling
- SSD1306 initialization
- OLED screen clearing
- Cursor positioning
- 5×7 printable ASCII font table
- Character and string rendering
- Modular I2C and OLED drivers

Project folder:

`I2C_SSD1306_OLED`

---

### TIM3 DHT22 UART Driver

Register-level DHT22 temperature and humidity sensor implementation using TIM3 and USART2.

Features include:

- TIM3 microsecond delay driver
- DHT22 single-wire communication
- GPIO input/output mode switching
- DHT22 start signal generation
- Sensor response detection
- 40-bit data acquisition
- Checksum validation
- Temperature and humidity conversion
- Push-button triggered measurements
- UART data transmission
- Modular driver structure

Project folder:

`TIM3_DHT22_UART`

---

## Development Board

STM32 NUCLEO-U083RC

---

## Development Approach

- Bare-metal C
- Direct register access
- No STM32 HAL
- Modular peripheral drivers
- Hardware-level peripheral learning

---

## Tools

- STM32CubeIDE
- ARM GCC Toolchain
- Tera Term
- Git
- GitHub

---

## Planned Projects

The following register-level projects are planned as part of this repository:

- 7-Segment Display Driver
- HC-SR04 Ultrasonic Sensor Driver
- AT24Cxx EEPROM Driver
- External Interrupt (EXTI) Driver
- Timer Input Capture
- PWM Motor Control
- SPI Driver
- ADC Driver
- RTC Driver