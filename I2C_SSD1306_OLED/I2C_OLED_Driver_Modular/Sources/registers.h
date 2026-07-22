#ifndef SOURCES_REGISTERS_H_
#define SOURCES_REGISTERS_H_

#include <stdint.h>

/* --- RCC Registers --- */
#define RCC_IOPENR             (*(volatile uint32_t *)0x4002104C)
#define RCC_APBENR1            (*(volatile uint32_t *)0x40021058)


/* --- GPIOB Registers --- */
#define GPIOB_MODER            (*(volatile uint32_t *)0x50000400)
#define GPIOB_OTYPER           (*(volatile uint32_t *)0x50000404)
#define GPIOB_SPEEDR           (*(volatile uint32_t *)0x50000408)
#define GPIOB_AFRH             (*(volatile uint32_t *)0x50000424)


/* --- I2C Registers --- */
#define I2C1_CR1                (*(volatile uint32_t *)0x40005400)
#define I2C1_CR2                (*(volatile uint32_t *)0x40005404)
#define I2C1_TIMINGR            (*(volatile uint32_t *)0x40005410)
#define I2C1_ISR                (*(volatile uint32_t *)0x40005418)
#define I2C1_ICR                (*(volatile uint32_t *)0x4000541C)
#define I2C1_RXDR               (*(volatile uint32_t *)0x40005424)
#define I2C1_TXDR               (*(volatile uint32_t *)0x40005428)

/* --- RCC Peripheral Enable Bits --- */
#define GPIOB_EN               1
#define I2C1_EN                21


/* --- GPIO Pin Numbers --- */
#define PB8                    8
#define PB9                    9

/* --- Alternate Function Selection --- */
#define AF4                    4

/* --- I2C Bit Positions --- */
#define PE 				       0
#define SADD                   0
#define RD_WRN                 10
#define START                  13
#define STOP                   14
#define NBYTES                 16
#define TXIS                   1
#define RXNE                   2
#define TC                     6
#define BUSY                   15
#define NACKF                  4
#define NACKCF                 4
#define STOPCF				   5
#define STOPF				   5

#define I2C_TIMEOUT 		   100000U
#endif /* SOURCES_REGISTERS_H_ */
