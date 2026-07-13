#include <stdint.h>
#include "i2c.h"
#include "registers.h"


void I2C1_Init(void) {
    // Enable GPIOB and I2C1 clocks
    RCC_IOPENR |= (1<<1);
    RCC_APBENR1 |= (1<<21);

    // Configure PB8/PB9 as AF4 open-drain
    GPIOB_MODER &= ~(3<<(PB8*2));
    GPIOB_MODER |= (2<<(PB8*2));
    GPIOB_MODER &= ~(3<<(PB9*2));
    GPIOB_MODER |= (2<<(PB9*2));

    GPIOB_AFRH &= ~(0xF<<((PB8-8)*4));
    GPIOB_AFRH |= (AF4<<((PB8-8)*4));
    GPIOB_AFRH &= ~(0xF<<((PB9-8)*4));
    GPIOB_AFRH |= (AF4<<((PB9-8)*4));

    GPIOB_OTYPER  |= (1<<PB8)|(1<<PB9);

    GPIOB_SPEEDR |= (1<<(PB8*2))|(1<<(PB9*2));

    // Configure timing (~100kHz safe default)
    I2C_TIMINGR = 0x00303D5B;

    I2C_CR1 |= 1; // Enable I2C1
}

uint8_t I2C1_Send(uint8_t addr, uint8_t *buffer, uint8_t len) {
    while(I2C_ISR & (1<<BUSY));

    I2C_CR2 &= ~(0x3FF << SADD);
    I2C_CR2 |= (addr << (SADD+1));

    I2C_CR2 &= ~(1<<RD_WRN);

    I2C_CR2 &= ~(0xFF << NBYTES);
    I2C_CR2 |= (len << NBYTES);

    I2C_CR2 |= (1<<START);

    while(!(I2C_ISR & ((1<<TXIS) | (1<<NACKF))));

    if(I2C_ISR & (1<<NACKF)) {
        I2C_ICR |= (1<<NACKCF);
        I2C_CR2 |= (1<<STOP);
        return 0;
    }

    for(uint8_t i=0;i<len;i++) {
        while(!(I2C_ISR & (1<<TXIS)));
        I2C_TXDR = buffer[i];
    }

    while(!(I2C_ISR & (1<<TC)));
    I2C_CR2 |= (1<<STOP);

    return 1;
}
