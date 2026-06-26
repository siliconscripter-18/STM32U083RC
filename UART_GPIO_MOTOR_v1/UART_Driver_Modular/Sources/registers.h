#ifndef SOURCES_REGISTERS_H_
#define SOURCES_REGISTERS_H_

#include <stdint.h>

#define RCC_IOPENR  (*(volatile uint32_t*)0x4002104C)
#define RCC_APBENR1 (*(volatile uint32_t*)0x40021058)

#define GPIOA_MODER (*(volatile uint32_t*)0x50000000)
#define GPIOA_ODR   (*(volatile uint32_t*)0x50000014)

#define GPIOC_MODER (*(volatile uint32_t*)0x50000800)
#define GPIOC_PUPDR (*(volatile uint32_t*)0x5000080c)
#define GPIOC_IDR   (*(volatile uint32_t*)0x50000810)

#define GPIOA_AFRL   (*(volatile uint32_t*)0x50000020)

#define USART2_CR1   (*(volatile uint32_t*)0x40004400)
#define USART2_BRR   (*(volatile uint32_t*)0x4000440C)
#define USART2_ISR   (*(volatile uint32_t*)0x4000441C)
#define USART2_TDR   (*(volatile uint32_t*)0x40004428)
#define USART2_RDR   (*(volatile uint32_t*)0x40004424)

#define GPIOAEN 0
#define GPIOCEN 2

#define USART2EN 17

#define PA2 2
#define PA3 3
#define PA5 5
#define PA8 8
#define PA9 9

#define PC13 13

#define AF7 7


#define UE 0
#define RE 2
#define TE 3

#define TXFNF 7
#define RXFNE 5



#endif /* SOURCES_REGISTERS_H_ */
