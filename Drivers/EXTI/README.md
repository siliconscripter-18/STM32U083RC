# EXTI Driver — STM32U083RC

Register-level external interrupt driver for the **STM32 NUCLEO-U083RC**, written in bare-metal C using CMSIS device headers, without STM32 HAL.

Configures the on-board user button (**PC13**) as a falling-edge interrupt source. The interrupt handler clears the pending flag and toggles the on-board LED (**PA5**).

---

## What It Demonstrates

- GPIO to EXTI line mapping
- Falling-edge trigger configuration
- EXTI interrupt unmasking and NVIC enable
- Interrupt service routine with pending-flag check and clear
- Separate, reusable GPIO and EXTI modules

---

## Configuration

| Item | Value |
|---|---|
| Input | PC13, user button, internal pull-up |
| Output | PA5, on-board LED |
| EXTI line | EXTI13, source GPIOC |
| Trigger | Falling edge |
| NVIC vector | `EXTI4_15_IRQn` |
| Handler | `EXTI4_15_IRQHandler()` |

---

## How It Works

Initialization:

```
Enable GPIOC clock
      ↓
PC13 as input with pull-up
      ↓
Select GPIOC as EXTI13 source   (EXTI->EXTICR[3])
      ↓
Enable falling-edge trigger     (EXTI->FTSR1)
      ↓
Unmask EXTI13                   (EXTI->IMR1)
      ↓
Enable EXTI4_15 in NVIC
```

At runtime:

```
Button press → PC13 falling edge → EXTI13 pending
      ↓
EXTI4_15_IRQHandler()
      ↓
Check pending flag → clear it (EXTI->FPR1) → toggle PA5
```

EXTI lines 4 to 15 share one interrupt vector on the STM32U083, so the handler checks which line is pending before acting.

---

## Project Structure

```
EXTI_DRIVER/
└── Sources/
    ├── main.c
    ├── gpio.c / gpio.h
    └── EXTI.c / EXTI.h
```

---

## Notes

- The pending flag is cleared inside the ISR before the LED is toggled.
- This driver is the base for the EXTI handling used in the Visitor Counter and Industrial Environment Monitor projects.

**Tools:** STM32CubeIDE · ARM GCC · Git