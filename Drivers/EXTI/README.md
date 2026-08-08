# EXTI_DRIVER

## Overview

This project demonstrates a **register-level External Interrupt (EXTI) driver** for the **STM32 NUCLEO-U083RC** development board.

The project configures **PC13** as an external interrupt input and detects a **falling edge** using the STM32 EXTI peripheral.

When the interrupt occurs, the `EXTI4_15_IRQHandler()` interrupt service routine clears the pending flag and toggles the LED connected to **PA5**.

The project uses **CMSIS device headers** for STM32U083RC peripheral register and bit definitions without using the STM32 HAL library.

> **This project demonstrates external GPIO interrupt configuration and interrupt handling using direct peripheral register programming.**

---

# Hardware Used

- STM32 NUCLEO-U083RC
- On-board User Button
- On-board LED
- USB Type-C Cable

---

# Features

- Bare-Metal C
- CMSIS Device Definitions
- Direct Peripheral Register Access
- No STM32 HAL
- GPIOA Clock Configuration
- GPIOC Clock Configuration
- PA5 GPIO Output Configuration
- PC13 GPIO Input Configuration
- Internal Pull-Up Configuration
- EXTI13 GPIO Source Selection
- Falling-Edge Trigger Configuration
- EXTI Interrupt Unmasking
- NVIC Interrupt Enable
- EXTI Pending Flag Detection
- EXTI Pending Flag Clearing
- Interrupt Service Routine
- LED Toggle on External Interrupt

---

# GPIO Configuration

| Pin | Configuration | Function |
| --- | --- | --- |
| PA5 | General-purpose output | On-board LED |
| PC13 | Input with pull-up | User Button / EXTI13 |

---

# EXTI Configuration

| Configuration | Value |
| --- | --- |
| EXTI Line | EXTI13 |
| GPIO Source | PC13 |
| Trigger | Falling Edge |
| Interrupt Vector | `EXTI4_15_IRQn` |
| Interrupt Handler | `EXTI4_15_IRQHandler()` |

---

# EXTI Driver Operation

The initialization sequence is:

    Enable GPIOC Clock
            ↓
    Configure PC13 as Input
            ↓
    Configure PC13 Pull-Up
            ↓
    Select GPIOC as EXTI13 Source
            ↓
    Enable Falling-Edge Trigger
            ↓
    Unmask EXTI13 Interrupt
            ↓
    Enable EXTI4_15 in NVIC

When the user button is pressed:

    PC13 Falling Edge
            ↓
    EXTI13 Triggered
            ↓
    EXTI4_15_IRQHandler()
            ↓
    Check EXTI13 Pending Flag
            ↓
    Clear Pending Flag
            ↓
    Toggle PA5 LED

---

# Interrupt Handling

The STM32U083 uses the `EXTI4_15_IRQn` interrupt vector for EXTI lines 4 through 15.

The interrupt service routine checks the EXTI13 pending flag before handling the interrupt.

The pending flag is cleared inside the ISR before the LED is toggled.

---

# CMSIS Usage

The project uses the STM32U083 CMSIS device header:

    #include "stm32u083xx.h"

Peripheral registers and bit definitions are accessed through the CMSIS device definitions.

Examples include:

    RCC->IOPENR
    GPIOA->MODER
    GPIOC->MODER
    GPIOC->PUPDR
    EXTI->EXTICR[3]
    EXTI->FTSR1
    EXTI->IMR1
    EXTI->FPR1
    GPIOA->ODR

No manually defined peripheral addresses are used.

No STM32 HAL library is used.

---

# Project Structure

EXTI/
│
├── README.md
│
└── EXTI_DRIVER/
    │
    ├── Sources/
    │   ├── main.c
    │   ├── gpio.c
    │   ├── gpio.h
    │   ├── EXTI.c
    │   └── EXTI.h
    │
    ├── Startup/
    │   └── startup_stm32u083rctx.s
    │
    ├── CMakeLists.txt
    ├── CMakePresets.json
    ├── cubide-gcc.cmake
    └── STM32U083RCTX_FLASH.ld
    
    Build-generated files such as `Debug/` and IDE configuration files are excluded from version control where applicable.

---

# Software Used

- STM32CubeIDE
- ARM GCC Toolchain
- CMSIS
- CMake
- Git
- GitHub

---

# Development Approach

- Bare-Metal C
- Direct Peripheral Register Access
- CMSIS Device Definitions
- No STM32 HAL
- Separate GPIO and EXTI source/header files
- Interrupt-driven GPIO event handling

---

# Notes

- PC13 is configured as an input with an internal pull-up resistor.
- EXTI13 is configured for falling-edge detection.
- EXTI13 is handled by the `EXTI4_15_IRQn` interrupt vector.
- The EXTI13 pending flag is cleared inside the ISR.
- PA5 is toggled as a visible indication that the interrupt was received.
- GPIO initialization and EXTI initialization are implemented as separate drivers.
- This project serves as a reusable EXTI driver reference for future STM32U083RC projects.