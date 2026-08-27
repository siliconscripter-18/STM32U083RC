# STM32U083RC Register-Level Projects

Bare-metal embedded systems projects developed using the **STM32 NUCLEO-U083RC** development board.

This repository documents my hands-on learning and implementation of STM32 peripherals, communication interfaces, sensors, interrupts, timing, EEPROM, display interfaces, and complete embedded applications using **direct peripheral register programming**.

All projects avoid the STM32 HAL library and focus on understanding the underlying hardware, peripheral registers, interrupt mechanisms, timing, driver structure, and application architecture.

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
    │   ├── Visitor_Counter/
    │   │   ├── Visitor_Counter/
    │   │   ├── Images/
    │   │   └── README.md
    │   │
    │   └── Visitor_Counter_V2/
    │       ├── Images/
    │       ├── V2/
    │       │   ├── Sources/
    │       │   ├── Startup/
    │       │   ├── Debug/
    │       │   ├── .settings/
    │       │   ├── .project
    │       │   ├── .cproject
    │       │   ├── CMakeLists.txt
    │       │   ├── CMakePresets.json
    │       │   ├── cubeide-gcc.make
    │       │   ├── STM32U083RC_TX_FLASH.ld
    │       │   └── Visitor_Counter_Debug.launch
    │       │
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

The implementation uses **CMSIS device definitions** for peripheral register and bit-field access without using the STM32 HAL library.

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

## Visitor Counter — V1

A complete embedded visitor counting application built around the **STM32 NUCLEO-U083RC**.

Version 1 combines GPIO, EXTI, SysTick, I2C, SSD1306 OLED, EEPROM, IR sensors, buzzer, and status indication to implement a working visitor counter.

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

Version 1 focuses on getting the complete visitor counter application working and establishing the foundation for the later FSM-based version.

The V1 implementation is retained separately as the original working version.

**Project folder**

`Projects/Visitor_Counter/`

---

# Visitor Counter — V2

Version 2 extends the Visitor Counter into a more structured application using a **Finite State Machine (FSM)** for direction detection and sensor-sequence validation.

The system uses four IR sensors:

- **S1 and S2** — Entry detection
- **S3 and S4** — Exit detection

The order in which sensors are triggered determines whether the movement is a valid entry, valid exit, or wrong-direction event.

### Features

- Four IR sensors
- EXTI falling-edge interrupts
- Independent sensor debounce
- Sensor re-arm mechanism
- FSM-based direction detection
- Valid entry detection
- Valid exit detection
- Wrong entry detection
- Wrong exit detection
- FSM sequence timeout
- SSD1306 128×64 I2C OLED
- AT24C04 EEPROM persistence
- Entry warning LED
- Exit warning LED
- Active buzzer
- Room occupancy tracking
- 1 ms SysTick time base
- Modular driver architecture
- Register-level STM32 programming
- Interrupt-driven sensor event capture

### Sensor Mapping

| Sensor | STM32 Pin | Function |
|---|---|---|
| S1 | PA0 | Entry sensor 1 |
| S2 | PA1 | Entry sensor 2 |
| S3 | PA6 | Exit sensor 1 |
| S4 | PA7 | Exit sensor 2 |

### Output Mapping

| Function | STM32 Pin |
|---|---|
| Entry Warning LED | PB4 |
| Exit Warning LED | PB5 |
| Active Buzzer | PB6 |
| I2C1 SCL | PB8 |
| I2C1 SDA | PB9 |

### Direction Detection

    S1 → S2
        ↓
    Valid Entry

    S2 → S1
        ↓
    Wrong Entry

    S3 → S4
        ↓
    Valid Exit

    S4 → S3
        ↓
    Wrong Exit

### FSM States

The application uses five states:

    FSM_IDLE
    FSM_ENTRY_S1
    FSM_ENTRY_S2
    FSM_EXIT_S3
    FSM_EXIT_S4

The general application flow is:

    Sensor Event
         ↓
    EXTI Interrupt
         ↓
    Sensor Flag
         ↓
    FSM Processing
         ↓
    Direction Decision
         ↓
    Counter / Warning Action

An incomplete sensor sequence returns to `FSM_IDLE` after the configured FSM timeout.

### V2 Project Structure

    Visitor_Counter_V2/
    │
    ├── Images/
    │   ├── Block_Diagram.png
    │   ├── Hardware_Setup.jpg
    │   ├── OLED_Count.jpg
    │   └── OLED_Splash.jpg
    │
    └── V2/
        ├── Sources/
        │   ├── display.c
        │   ├── display.h
        │   ├── EEPROM.c
        │   ├── EEPROM.h
        │   ├── EXTI.c
        │   ├── EXTI.h
        │   ├── font5x7.c
        │   ├── font5x7.h
        │   ├── FSM.c
        │   ├── FSM.h
        │   ├── gpio.c
        │   ├── gpio.h
        │   ├── i2c.c
        │   ├── i2c.h
        │   ├── main.c
        │   ├── ssd1306.c
        │   ├── ssd1306.h
        │   ├── SysTick.c
        │   └── SysTick.h
        │
        ├── Startup/
        ├── Debug/
        ├── .settings/
        ├── .project
        ├── .cproject
        ├── CMakeLists.txt
        ├── CMakePresets.json
        ├── cubeide-gcc.make
        ├── STM32U083RC_TX_FLASH.ld
        └── Visitor_Counter_Debug.launch

**Project folder**

`Projects/Visitor_Counter_V2/`

---

# Visitor Counter V1 → V2

| V1 | V2 |
|---|---|
| Basic visitor counting | FSM-based direction detection |
| Sensor event handling | Sensor sequence validation |
| Entry/exit counting | Valid and wrong-direction detection |
| Basic sensor handling | Debounce + sensor re-arm |
| OLED + EEPROM | OLED + EEPROM + warning system |
| Basic application flow | Explicit application states |
| Single application flow | FSM-driven movement logic |
| Working foundation | More structured application architecture |

V2 builds on the working V1 hardware and peripheral drivers rather than replacing the entire project.

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
- Hardware/application separation
- State-machine based application logic where appropriate

The repository evolves from individual peripheral experiments toward complete embedded applications.

---

# Peripherals and Interfaces Covered

The repository currently includes implementations involving:

- GPIO
- EXTI
- NVIC
- SysTick
- TIM3
- UART / USART
- I2C
- SSD1306 OLED
- DHT22
- HC-SR04
- EEPROM
- DC motor control
- IR sensors
- Finite State Machines

Future projects will extend this into additional STM32 peripherals and interfaces.

---

# Hardware Used

Hardware used across the projects includes:

- STM32 NUCLEO-U083RC
- SSD1306 128×64 I2C OLED
- DHT22 / AM2302
- HC-SR04 ultrasonic sensor
- IR obstacle sensors
- AT24Cxx EEPROM
- L298N motor driver
- DC motor
- LEDs
- Buzzer
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
- Finite State Machines
- Persistent data storage
- Hardware/application separation

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
- More complex multi-peripheral applications

---

# Author

**Balaji M**

Target: **STM32U083RC**

Repository: **STM32U083RC Register-Level Projects**