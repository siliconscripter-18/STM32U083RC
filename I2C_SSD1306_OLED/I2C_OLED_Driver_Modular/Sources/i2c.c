#include <stdint.h>
#include "i2c.h"
#include "registers.h"

void I2C1_Init(void) {
    // Enable GPIOB and I2C1 clocks
    RCC_IOPENR |= (1<<GPIOB_EN);
    RCC_APBENR1 |= (1<<I2C1_EN);

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
    I2C1_TIMINGR = 0x00303D5B;

    I2C1_CR1 |= (1 << PE); // Enable I2C1
}


uint8_t I2C1_CheckDevice(uint8_t slaveaddr)
{
    uint32_t timeout;

    timeout = I2C_TIMEOUT;
    while(I2C1_ISR & (1<<BUSY)) {
        if(--timeout == 0)
            return 0;
    }

    I2C1_CR2 &= ~(0x3FF<<SADD);
    I2C1_CR2 |=  (slaveaddr<<(SADD+1));

    I2C1_CR2 &= ~(1<<RD_WRN);

    I2C1_CR2 &= ~(0xFF<<NBYTES);

    I2C1_CR2 |= (1<<START);

    timeout = I2C_TIMEOUT;
    while(!(I2C1_ISR & ((1<<TC) | (1<<NACKF)))) {
        if(--timeout == 0) {
            I2C1_CR2 |= (1<<STOP);
            return 0;
        }
    }

    if(I2C1_ISR & (1<<NACKF)) {
        I2C1_ICR |= (1<<NACKCF);
        I2C1_CR2 |= (1<<STOP);

        timeout = I2C_TIMEOUT;
        while(!(I2C1_ISR & (1<<STOPF))) {
            if(--timeout == 0)
                return 0;
        }

        I2C1_ICR |= (1<<STOPCF);
        return 0;
    }

    I2C1_CR2 |= (1<<STOP);

    timeout = I2C_TIMEOUT;
    while(!(I2C1_ISR & (1<<STOPF))) {
        if(--timeout == 0)
            return 0;
    }

    I2C1_ICR |= (1<<STOPCF);

    return 1;
}


uint8_t I2C1_Send(uint8_t slaveaddr, uint8_t *buffer, uint8_t len)
{
    uint32_t timeout;

    timeout = I2C_TIMEOUT;
    while(I2C1_ISR & (1<<BUSY)) {
        if(--timeout == 0)
            return 0;
    }

    I2C1_CR2 &= ~(0X3FF<<SADD);
    I2C1_CR2 |= (slaveaddr<<(SADD+1));

    I2C1_CR2 &= ~(1<<RD_WRN);

    I2C1_CR2 &= ~(0XFF<<NBYTES);

    I2C1_CR2 |=  (len<<NBYTES);

    I2C1_CR2 |= (1<<START);

    for(uint8_t i=0; i<len; i++) {
        timeout = I2C_TIMEOUT;
        while(!(I2C1_ISR & ((1<<TXIS) | (1<<NACKF)))) {
            if(--timeout == 0) {
                I2C1_CR2 |= (1<<STOP);
                return 0;
            }
        }

        if(I2C1_ISR & (1<<NACKF)) {
            I2C1_ICR |= (1<<NACKCF);
            I2C1_CR2 |= (1<<STOP);

            timeout = I2C_TIMEOUT;
            while(!(I2C1_ISR & (1<<STOPF))) {
                if(--timeout == 0)
                    return 0;
            }

            I2C1_ICR |= (1<<STOPCF);
            return 0;
        }

        I2C1_TXDR = buffer[i];
    }

    timeout = I2C_TIMEOUT;
    while(!(I2C1_ISR & (1<<TC))) {
        if(--timeout == 0) {
            I2C1_CR2 |= (1<<STOP);
            return 0;
        }
    }

    I2C1_CR2 |= (1<<STOP);

    timeout = I2C_TIMEOUT;
    while(!(I2C1_ISR & (1<<STOPF))) {
        if(--timeout == 0)
            return 0;
    }

    I2C1_ICR |= (1<<STOPCF);

    return 1;
}

