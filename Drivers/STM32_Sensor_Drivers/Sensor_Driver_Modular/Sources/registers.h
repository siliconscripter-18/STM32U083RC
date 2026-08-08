#ifndef SOURCES_REGISTERS_H_
#define SOURCES_REGISTERS_H_

#include <stdint.h>

/* --- RCC Registers --- */
#define RCC_IOPENR             (*(volatile uint32_t *)0x4002104C)
#define RCC_APBENR1            (*(volatile uint32_t *)0x40021058)

/* --- GPIOA Registers --- */
#define GPIOA_MODER            (*(volatile uint32_t *)0x50000000)
#define GPIOA_OTYPER           (*(volatile uint32_t *)0x50000004)
#define GPIOA_IDR              (*(volatile uint32_t *)0x50000010)
#define GPIOA_PUPDR            (*(volatile uint32_t *)0x5000000C)
#define GPIOA_BSRR             (*(volatile uint32_t *)0x50000018)
#define GPIOA_AFRL             (*(volatile uint32_t *)0x50000020)

/* --- GPIOC Registers --- */
#define GPIOC_MODER            (*(volatile uint32_t *)0x50000800)
#define GPIOC_IDR              (*(volatile uint32_t *)0x50000810)
#define GPIOC_PUPDR            (*(volatile uint32_t *)0x5000080C)
#define GPIOC_BSRR             (*(volatile uint32_t *)0x50000818)

/* --- USART2 Registers --- */
#define USART_CR1              (*(volatile uint32_t *)0x40004400)
#define USART_BRR              (*(volatile uint32_t *)0x4000440C)
#define USART_ISR              (*(volatile uint32_t *)0x4000441C)
#define USART_RDR              (*(volatile uint32_t *)0x40004424)
#define USART_TDR              (*(volatile uint32_t *)0x40004428)

/* --- TIM3 Registers --- */
#define TIM3_CR1               (*(volatile uint32_t *)0x40000400)
#define TIM3_SR                (*(volatile uint32_t *)0x40000410)
#define TIM3_EGR               (*(volatile uint32_t *)0x40000414)
#define TIM3_CNT               (*(volatile uint32_t *)0x40000424)
#define TIM3_PSC               (*(volatile uint32_t *)0x40000428)
#define TIM3_ARR               (*(volatile uint32_t *)0x4000042C)

/* --- RCC Peripheral Enable Bits --- */
#define GPIOA_EN               0
#define GPIOC_EN               2
#define TIM3_EN                1
#define USART_EN               17

/* --- GPIO Pin Numbers --- */
#define PA2                    2
#define PA3                    3
#define PA6                    6
#define PA7                    7
#define PC2                    2
#define PC3                    3
#define DHT22_PIN              10

/* --- Alternate Function Selection --- */
#define AF7                    7

/* --- USART Bits --- */
#define UE                     0
#define RE                     2
#define TE                     3
#define RXFNE                  5
#define TXFNF                  7

/* --- TIM3 Bits --- */
#define UIF                    0
#define UG                     0
#define CEN                    0
#define DIR                    4
#define ARPE                   7

#endif /* SOURCES_REGISTERS_H_ */
