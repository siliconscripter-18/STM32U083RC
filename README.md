# STM32U083RC Register-Level Projects

Bare-metal embedded systems projects developed using the **STM32 NUCLEO-U083RC** development board.

This repository documents my hands-on learning and implementation of STM32 peripherals, communication interfaces, sensors, interrupts, and embedded applications using **direct peripheral register programming**.

All projects avoid the STM32 HAL library and focus on understanding the underlying hardware, peripheral registers, interrupt mechanisms, timing, and firmware structure.

---

# Repository Structure

    STM32U083RC/
    │
    ├── Drivers/
    │   ├── EXTI/
    │   │   ├── EXTI_DRIVER/
    │   │   └── README.md
    │   │
    │   ├── I2C_SSD1306_OLED/
    │   │   ├── I2C_OLED_Driver_Modular/
    │   │   ├── Images/
    │   │   └── README.md
    │   │
    │   ├── STM32_Sensor_Drivers/
    │   │   ├── Sensor_Driver_Modular/
    │   │   ├── Images/
    │   │   └── README.md
    │   │
    │   └── UART_GPIO_MOTOR_v1/
    │       ├── UART_Driver_Modular/
    │       ├── Images/
    │       └── README.md
    │
    ├── Projects/
    │   └── Visitor_Counter/
    │       ├── Visitor_Counter/
    │       ├── Images/
    │       └── README.md
    │
    ├── .gitignore
    └── README.md

The repository is divided into two main sections:

- **Drivers/** — standalone peripheral, communication, and sensor driver implementations.
- **Projects/** — complete embedded applications that combine multiple drivers and peripherals.

Each individual driver or application contains its own README describing its implementation, hardware, configuration, and operation.

---

# Drivers

## EXTI Driver

A register-level external interrupt driver for the STM32U083.

The current implementation uses **CMSIS device definitions** for peripheral register and bit-field access without using the STM32 HAL library.

### Features

- External interrupt configuration
- EXTI13 configuration
- PC13 GPIO source selection
- Falling-edge trigger
- EXTI interrupt unmasking
- NVIC interrupt configuration
- Pending flag detection
- Pending flag clearing
- Interrupt service routine
- PA5 LED indication

**Project folder**

`Drivers/EXTI/`

---

## UART GPIO Motor Driver

Register-level UART, GPIO, button, and DC motor control implementation.

### Features

- USART2 transmit and receive
- UART command processing
- GPIO LED control
- Push-button status reading
- L298N DC motor forward, reverse, and stop control
- Driver-based firmware structure

**Project folder**

`Drivers/UART_GPIO_MOTOR_v1/`

---

## I2C SSD1306 OLED Driver

Register-level I2C1 and SSD1306 OLED display implementation.

### Features

- I2C1 master transmission
- I2C slave address configuration
- I2C transfer byte count configuration
- START and STOP condition generation
- BUSY flag handling
- TXIS flag handling
- NACKF error detection
- Transfer Complete handling
- SSD1306 initialization
- OLED screen clearing
- Cursor positioning
- 5×7 printable ASCII font table
- Character rendering
- String rendering
- Scaled character and string rendering

**Project folder**

`Drivers/I2C_SSD1306_OLED/`

---

## STM32 Sensor Drivers

Register-level sensor drivers demonstrating reusable peripheral and sensor interfaces.

### Features

- TIM3 microsecond timer
- DHT22 temperature and humidity driver
- HC-SR04 ultrasonic distance driver
- GPIO input/output mode switching
- DHT22 checksum validation
- HC-SR04 timeout protection
- Push-button triggered measurements
- UART data transmission
- Reusable driver structure

**Project folder**

`Drivers/STM32_Sensor_Drivers/`

---

# Projects

## Visitor Counter

A complete embedded application built around the STM32 NUCLEO-U083RC.

The current **Version 1.0** implementation combines GPIO, EXTI, SysTick, I2C, SSD1306 OLED, EEPROM, and sensor-related functionality to implement a visitor counting system.

### Features

- IR sensor based visitor detection
- External interrupt based event detection
- Entry and exit counting
- Visitor count persistence using EEPROM
- SSD1306 128×64 OLED display
- Visitor count display
- Entry count display
- Exit count display
- Current room occupancy display
- Splash screen
- Display layout
- Buzzer indication
- Status LED
- SysTick-based millisecond timing
- Register-level peripheral programming
- Separate driver and application functionality

### Version 1.0

Version 1.0 focuses on getting the complete visitor counter application working and documenting the hardware, drivers, and firmware implementation.

The current Version 1.0 code is intentionally kept as the working implementation.

**Project folder**

`Projects/Visitor_Counter/`

### Version 2.0

Version 2.0 will refactor the Visitor Counter application around a **Finite State Machine (FSM)** architecture.

The goal is to improve application flow, state handling, scalability, and separation between hardware drivers and application logic while reusing the existing peripheral drivers.

Planned Version 2.0 direction:

    Hardware Inputs
          ↓
    Driver Layer
          ↓
    Application Logic
          ↓
    FSM
          ↓
    State Transitions
          ↓
    Display / Buzzer / EEPROM

---

# Development Board

- STM32 NUCLEO-U083RC
- STM32U083RC microcontroller

---

# Development Approach

The projects in this repository are developed with an emphasis on understanding the MCU at the register level.

### Core principles

- Bare-metal C
- Direct peripheral register access
- CMSIS device definitions where applicable
- No STM32 HAL library
- Separate `.c` and `.h` files
- Reusable peripheral drivers
- Explicit peripheral configuration
- Interrupt-driven event handling where required
- Hardware-oriented firmware design

The repository also evolves from individual driver experiments toward complete embedded applications.

---

# Peripherals and Interfaces Covered

The repository currently includes implementations involving:

- GPIO
- EXTI
- SysTick
- TIM3
- UART / USART
- I2C
- SSD1306 OLED
- DHT22
- HC-SR04
- EEPROM
- DC motor control

Future projects will extend this into additional STM32 peripherals and interfaces.

---

# Hardware Used

Hardware used across the projects includes:

- STM32 NUCLEO-U083RC
- SSD1306 128×64 I2C OLED
- DHT22 / AM2302
- HC-SR04 ultrasonic sensor
- IR obstacle sensor
- AT24Cxx EEPROM
- L298N motor driver
- DC motor
- LEDs
- Buzzer
- 7-segment displays
- Push buttons
- Breadboard
- Jumper wires
- USB Type-C cable

---

# Software Used

- STM32CubeIDE
- ARM GCC Toolchain
- CMSIS
- CMake
- Tera Term
- Git
- GitHub

---

# Learning Progression

The repository follows a progression from individual peripheral experiments to larger embedded applications.

    GPIO
      ↓
    UART
      ↓
    I2C
      ↓
    OLED
      ↓
    Timers
      ↓
    Sensors
      ↓
    EXTI
      ↓
    EEPROM
      ↓
    Complete Application
      ↓
    FSM-based Application Architecture

Each project is intended to build on the concepts learned in the previous projects.

---

# Planned Projects

The following register-level projects are planned for future development:

- 7-Segment Display Driver
- Timer Input Capture
- PWM Driver
- SPI Driver
- ADC Driver
- RTC Driver
- Additional STM32U083 peripheral drivers

---

# Repository Goal

This repository documents my journey of learning embedded systems by implementing peripherals, communication interfaces, sensors, and complete applications from the hardware level upward.

The primary goal is to understand:

- MCU peripheral registers
- Peripheral initialization
- GPIO configuration
- Interrupt handling
- Timer operation
- Communication protocols
- Sensor interfacing
- Driver design
- Application architecture
- State-machine based firmware design

The projects are developed without relying on vendor abstraction libraries so that the underlying STM32 hardware and firmware behavior remain explicit and understandable.

---

# Future Direction

The long-term goal is to evolve the repository from individual register-level peripheral implementations into structured embedded firmware projects with:

- Reusable drivers
- Clear application layers
- Interrupt-driven operation
- Finite State Machines
- Hardware abstraction where appropriate
- Error handling
- Persistent data storage
- Scalable application architecture