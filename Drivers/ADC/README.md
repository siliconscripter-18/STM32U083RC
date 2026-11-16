# ADC Driver — STM32U083RC

Register-level ADC driver for the **STM32 NUCLEO-U083RC**, written in bare-metal C using CMSIS device headers, without STM32 HAL.

Configures **PC0 (ADC_IN0)** as an analog input and performs 12-bit ADC conversions. A 10k potentiometer is used to vary the input voltage, with ADC readings and calculated voltage transmitted through UART.

---

## What It Demonstrates

- GPIO analog mode configuration
- ADC peripheral clock and internal regulator enable
- ADC calibration
- ADC channel selection and initialization
- 12-bit single conversion using polling
- End-of-conversion (EOC) flag checking
- ADC voltage calculation and UART transmission

---

## Configuration

| Item | Value |
|---|---|
| Board | STM32 NUCLEO-U083RC |
| Analog input | PC0 (ADC_IN0) |
| ADC resolution | 12-bit |
| ADC range | 0–4095 |
| Reference voltage | 3.3V (assumed) |
| Input | 10k potentiometer |
| Output | USART2 via ST-LINK VCP |
| Baud rate | 115200 |

---

## Hardware Connections

| Potentiometer | NUCLEO-U083RC |
|---|---|
| Outer pin 1 | 3.3V |
| Outer pin 2 | GND |
| Middle pin (wiper) | PC0 |

---

## How It Works

```text
ADC Initialization
       ↓
Enable ADC clock and regulator
       ↓
ADC calibration
       ↓
Select ADC channel 0
       ↓
Enable ADC
       ↓
Start conversion
       ↓
Wait for EOC flag
       ↓
Read ADC data register
       ↓
Calculate voltage
       ↓
Transmit ADC value and voltage through UART
```

---

## Project Structure

```text
ADC/
├── ADC_Driver/
│   ├── .settings/
│   ├── Sources/
│   │   ├── main.c
│   │   ├── adc.c
│   │   ├── adc.h
│   │   ├── tim1.c
│   │   ├── tim1.h
│   │   ├── uart.c
│   │   └── uart.h
│   ├── Startup/
│   ├── .cproject
│   ├── .project
│   ├── ADC Debug.launch
│   ├── CMakeLists.txt
│   ├── CMakePresets.json
│   ├── cubeide-gcc.cmake
│   └── STM32U083RCTX_FLASH.ld
│
└── README.md
```

**Dependencies:** TIM1 delay driver · UART driver

**Tools:** STM32CubeIDE · ARM GCC · Git