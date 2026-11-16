# IWDG – Independent Watchdog Driver

Register-level IWDG driver and test application for the **STM32 NUCLEO-U083RC** board. No HAL used.

## What This Project Demonstrates

- Independent Watchdog initialization and activation
- Prescaler and reload value configuration
- Watchdog refresh operation
- Watchdog reset detection using RCC reset flags
- Automatic MCU recovery when watchdog refresh stops

## Configuration

| Parameter | Configuration |
|---|---|
| Board | STM32 NUCLEO-U083RC |
| Watchdog | Independent Watchdog (IWDG) |
| Prescaler | /32 |
| Reload Value | 4000 |
| Clock Source | LSI |
| LED | PA5 |
| User Button | PC13 (Active LOW) |
| UART | USART2 – ST-LINK VCP |
| Baud Rate | 115200 |

*Estimated watchdog timeout: approximately 4 seconds, assuming an LSI frequency of 32 kHz. Actual timeout may vary.*

## How It Works

1. Initialize GPIO, TIM1 and UART.
2. Check the RCC reset flag to identify an IWDG reset.
3. Send `IWDG_RESET` through UART if a watchdog reset occurred.
4. Initialize the IWDG.
5. Toggle the LED and refresh the watchdog during normal operation.
6. Press B1 to intentionally stop watchdog refresh.
7. The watchdog times out and resets the MCU.
8. After restart, the reset cause is reported through UART.

## Project Structure

```text
IWDG/
├── IWDG_Driver/
│   ├── Sources/
│   │   ├── main.c
│   │   ├── IWDG.c
│   │   ├── IWDG.h
│   │   ├── gpio.c
│   │   ├── gpio.h
│   │   ├── tim1.c
│   │   ├── tim1.h
│   │   ├── uart.c
│   │   └── uart.h
│   ├── Startup/
│   ├── .project
│   ├── .cproject
│   └── ...
└── README.md
```

## Tools

- STM32CubeIDE
- ARM GCC
- STM32 NUCLEO-U083RC