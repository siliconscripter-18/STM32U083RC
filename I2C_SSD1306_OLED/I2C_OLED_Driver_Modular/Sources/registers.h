#ifndef SOURCES_REGISTERS_H_
#define SOURCES_REGISTERS_H_


// --- RCC and GPIO registers ---
#define RCC_IOPENR   (*(volatile uint32_t*)0x4002104C)
#define RCC_APBENR1  (*(volatile uint32_t*)0x40021058)

#define GPIOB_MODER  (*(volatile uint32_t*)0x50000400)
#define GPIOB_AFRH   (*(volatile uint32_t*)0x50000424)
#define GPIOB_OTYPER (*(volatile uint32_t*)0x50000404)
#define GPIOB_SPEEDR (*(volatile uint32_t*)0x50000408)

#define I2C_CR1      (*(volatile uint32_t*)0x40005400)
#define I2C_CR2      (*(volatile uint32_t*)0x40005404)
#define I2C_TIMINGR  (*(volatile uint32_t*)0x40005410)
#define I2C_ISR      (*(volatile uint32_t*)0x40005418)
#define I2C_ICR      (*(volatile uint32_t*)0x4000541C)
#define I2C_TXDR     (*(volatile uint32_t*)0x40005428)

#define PB8 8
#define PB9 9
#define AF4 4

// Bit positions
#define SADD   0
#define RD_WRN 10
#define START  13
#define STOP   14
#define NBYTES 16
#define TXIS   1
#define TC     6
#define BUSY   15
#define NACKF  4
#define NACKCF 4



#endif /* SOURCES_REGISTERS_H_ */
