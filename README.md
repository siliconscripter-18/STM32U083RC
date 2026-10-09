# STM32U083RC — Bare-Metal Embedded Systems

Bare-metal embedded firmware developed on the **STM32 NUCLEO-U083RC** using direct peripheral register programming — no STM32 HAL.

The repository progresses from individual peripheral drivers to complete multi-peripheral embedded applications.

**Approach:** Bare-metal C · Direct register access · No HAL · Modular drivers
**Tools:** STM32CubeIDE · ARM GCC · CMSIS · Tera Term · Git

---

## Projects

### Industrial Environment Monitor

Temperature, humidity, and occupancy monitoring system with PWM fan control, FSM-based alert states, and persistent event logging.

<p align="center">
  <img src="Projects/Industrial_Environment_Monitor/V2/Images/Hardware_Setup.jpg" width="700">
</p>

| Version | What it adds |
|---|---|
| V1 | DHT22 + IR occupancy + PWM fan + FSM + OLED |
| V2 | DS1307 RTC + EEPROM alert logging + UART log interface |

**Highlights:**
- Temperature FSM — Standby / Warm Alert / Thermal Alert with PWM fan response
- IR entry/exit counting drives room light control
- V2 logs every alert-state transition with RTC timestamp to EEPROM
- UART `LOG` and `CLEAR` commands for alert history retrieval

📁 [`Projects/Industrial_Environment_Monitor`](Projects/Industrial_Environment_Monitor)

---

### Visitor Counter

IR sensor based visitor counting system with FSM direction detection, EEPROM persistence, and wrong-direction warning system.

<p align="center">
  <img src="Projects/Visitor_Counter/V2/Images/Hardware_Setup_front_view.jpg" width="700">
</p>

| Version | What it adds |
|---|---|
| V1 | 2 IR sensors + EXTI counting + OLED + EEPROM persistence |
| V2 | 4 IR sensors + FSM direction detection + wrong-direction warning |

**Highlights:**
- V2 FSM validates sensor trigger sequence — S1→S2 valid entry, S2→S1 wrong direction
- Per-sensor debounce and re-arm mechanism
- 2000ms FSM timeout for incomplete sequences
- Warning LED and buzzer on wrong-direction detection

📁 [`Projects/Visitor_Counter`](Projects/Visitor_Counter)

---

## Drivers

Standalone register-level peripheral drivers developed as reusable modules before integration into projects.

| Driver | What it covers |
|---|---|
| [`UART + GPIO + Motor`](Drivers/UART_GPIO_MOTOR_v1) | USART2 transmit/receive, UART command parser, GPIO, L298N motor control |
| [`I2C + SSD1306 OLED`](Drivers/I2C_SSD1306_OLED) | I2C1 master driver, SSD1306 init and display, 5×7 ASCII font rendering |
| [`Sensor Drivers`](Drivers/STM32_Sensor_Drivers) | DHT22 and HC-SR04 with TIM3 µs timing, checksum validation, timeout protection |
| [`EXTI`](Drivers/EXTI) | EXTI configuration, GPIO-to-EXTI mapping, edge triggering, NVIC, ISR handling |

---

## Peripherals Covered

`GPIO` · `EXTI` · `NVIC` · `SysTick` · `TIM1` · `TIM2` · `TIM3` · `PWM` · `USART2` · `I²C1` · `DHT22` · `HC-SR04` · `SSD1306 OLED` · `AT24C04N EEPROM` · `DS1307 RTC` · `IR Sensors` · `L298N Motor Driver`