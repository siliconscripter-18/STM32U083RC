# STM32U083RC Register-Level Projects

Bare-metal embedded systems projects developed using the **STM32 NUCLEO-U083RC** development board.

All projects are implemented using **direct peripheral register programming** without the STM32 HAL library. Each project is organized using a modular driver architecture to demonstrate reusable embedded firmware design.

---

# Projects

## UART GPIO Motor Driver

Register-level UART, GPIO, button, and DC motor control implementation.

### Features

- USART2 transmit and receive
- UART command processing
- GPIO LED control
- Push-button status reading
- L298N DC motor forward, reverse, and stop control
- Modular driver structure

**Project folder**

`UART_GPIO_MOTOR_v1`

---

## I2C SSD1306 OLED Driver

Register-level I2C1 driver and SSD1306 OLED display implementation.

### Features

- I2C1 master transmission
- I2C status flag handling
- SSD1306 initialization
- OLED screen clearing
- Cursor positioning
- 5×7 printable ASCII font table
- Character and string rendering
- Modular I2C and OLED drivers

**Project folder**

`I2C_SSD1306_OLED`

---

## STM32 Sensor Drivers

Register-level sensor drivers demonstrating reusable firmware architecture using shared peripheral drivers.

### Features

- Generic TIM3 microsecond timer driver
- DHT22 temperature and humidity sensor driver
- HC-SR04 ultrasonic distance sensor driver
- GPIO input/output mode switching
- DHT22 checksum validation
- HC-SR04 timeout protection
- Push-button triggered sensor measurements
- UART data transmission
- Modular driver structure

**Project folder**

`STM32_Sensor_Drivers`

---

# Development Board

- STM32 NUCLEO-U083RC

---

# Development Approach

- Bare-metal C
- Direct register access
- No STM32 HAL
- Modular peripheral drivers
- Register-level peripheral programming
- Reusable firmware architecture

---

# Tools

- STM32CubeIDE
- ARM GCC Toolchain
- Tera Term
- Git
- GitHub

---

# Planned Projects

The following register-level projects are planned as part of this repository:

- 7-Segment Display Driver
- AT24Cxx EEPROM Driver
- External Interrupt (EXTI) Driver
- Timer Input Capture
- PWM Driver
- SPI Driver
- ADC Driver
- RTC Driver

---

# Repository Goal

This repository documents my journey of learning embedded systems by implementing peripherals and sensors from scratch using register-level programming. Each project focuses on understanding the underlying hardware, writing reusable drivers, and building firmware without relying on vendor abstraction libraries.