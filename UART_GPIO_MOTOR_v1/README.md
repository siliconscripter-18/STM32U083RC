# UART_GPIO_MOTOR_v1

## Overview

This project demonstrates a **modular register-level firmware implementation** on the **STM32 NUCLEO-U083RC** development board.

The firmware provides a UART-based command-line interface to control the on-board LED, read the user button status, and control a DC motor through an **L298N motor driver**.

All peripheral configuration is performed through **direct register programming** without using the STM32 HAL library.

---

## Hardware Used

* STM32 NUCLEO-U083RC
* L298N Motor Driver
* DC Motor
* 12V DC Adapter
* USB Type-C (Programming + UART Terminal)

---

## Features

* Register-level programming
* Modular driver architecture
* UART Transmit
* UART Receive
* UART Command Interface
* LED ON/OFF Control
* LED Status
* User Button Status
* Motor Forward
* Motor Reverse
* Motor Stop

---

## UART Commands

| Command | Function          |
| ------- | ----------------- |
| H       | Display Help Menu |
| L       | LED ON            |
| O       | LED OFF           |
| S       | LED Status        |
| B       | Button Status     |
| F       | Motor Forward     |
| R       | Motor Reverse     |
| X       | Motor Stop        |

> Commands are **case-insensitive**.

---

## Pin Connections

### STM32 → L298N

| STM32 Pin    | Function      | L298N Pin |
| ------------ | ------------- | --------- |
| CN9 D7 (PA8) | Motor Control | IN1       |
| CN9 D8 (PA9) | Motor Control | IN2       |
| GND          | Common Ground | GND       |

### External Power Supply

| Adapter | L298N |
| ------- | ----- |
| +12V    | 12V   |
| GND     | GND   |

> **Important:** The STM32 and the L298N must share a common ground.

---

## Project Structure

UART_GPIO_MOTOR_v1
├── Images/
│   ├── hardware_setup.jpg
│   ├── help_menu.png
│   ├── led_button_demo.png
│   ├── motor_control.png
│   └── cubeide_project.png
├── README.md
├── .gitignore
└── UART_Driver_Modular/
    ├── .settings/
    ├── Sources/
    │   ├── gpio.c
    │   ├── gpio.h
    │   ├── main.c
    │   ├── motor.c
    │   ├── motor.h
    │   ├── uart.c
    │   └── uart.h
    ├── Startup/
    ├── .project
    ├── .cproject
    ├── CMakeLists.txt
    ├── CMakePresets.json
    ├── cubeide-gcc.cmake
    └── STM32U083RCTX_FLASH.ld


---

## Project Images

### Hardware Setup

[Hardware Setup](Images/hardware_setup.jpg)

---

### UART Help Menu

[Help Menu](Images/help_menu.png)

---

### LED and Button Demonstration

[LED and Button](Images/led_button_demo.png)

---

### Motor Control Demonstration

[Motor Control](Images/motor_control.png)

---

### STM32CubeIDE Project

[CubeIDE](Images/cubeide_project.png)

---

## Software

* STM32CubeIDE
* Tera Term
* Git
* GitHub

---

## Notes

* Register-level peripheral programming
* USART2 used for UART communication
* Modular driver implementation
* No HAL library used
* Tested on STM32 NUCLEO-U083RC

---

## Future Improvements

* Centralize register definitions into a common header
* Interrupt-driven UART reception
* PWM motor speed control
* SSD1306 OLED display
* I2C driver
* External sensor integration

