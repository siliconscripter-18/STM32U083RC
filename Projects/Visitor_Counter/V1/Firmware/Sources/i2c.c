/******************************************************************************
 * @file    i2c.c
 * @brief   I2C1 driver for STM32U083RC.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>
#include "i2c.h"
#include "stm32u083xx.h"

#define I2C_TIMEOUT    100000U

/******************************************************************************
 * @brief Initializes I2C1 for standard mode (100 kHz).
 ******************************************************************************/
void I2C1_Init(void)
{
    /* Enable GPIOB and I2C1 peripheral clocks */
    RCC->IOPENR  |= RCC_IOPENR_GPIOBEN;
    RCC->APBENR1 |= RCC_APBENR1_I2C1EN;

    /* Configure PB8 as Alternate Function 4 (SCL) */
    GPIOB->MODER &= ~GPIO_MODER_MODE8_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE8_1;

    /* Configure PB9 as Alternate Function 4 (SDA) */
    GPIOB->MODER &= ~GPIO_MODER_MODE9_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE9_1;

    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL8_Msk;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_2;

    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL9_Msk;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2;

    /* Configure pins as open-drain */
    GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;

    /* Configure low-speed operation */
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_0 |
                      GPIO_OSPEEDR_OSPEED9_0;

    /* Configure I2C timing for 100 kHz */
    I2C1->TIMINGR = 0x00303D5B;

    /* Enable I2C peripheral */
    I2C1->CR1 |= I2C_CR1_PE;
}

/******************************************************************************
 * @brief Checks whether an I2C slave device acknowledges its address.
 *
 * @param slaveaddr 7-bit I2C slave address.
 *
 * @retval 1 Device acknowledged.
 * @retval 0 Device did not respond or timeout occurred.
 ******************************************************************************/
uint8_t I2C1_CheckDevice(uint8_t slaveaddr)
{
    uint32_t timeout = I2C_TIMEOUT;

    /* Wait until the I2C bus becomes idle */
    while (I2C1->ISR & I2C_ISR_BUSY)
    {
        if (--timeout == 0U)
        {
            return 0;
        }
    }

    /* Configure the slave address */
    I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
    I2C1->CR2 |= (slaveaddr << 1);

    /* Configure write transfer with zero data bytes */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;

    /* Generate the START condition */
    I2C1->CR2 |= I2C_CR2_START;

    timeout = I2C_TIMEOUT;

    /* Wait for transfer complete or NACK */
    while (!(I2C1->ISR & (I2C_ISR_TC | I2C_ISR_NACKF)))
    {
        if (--timeout == 0U)
        {
            I2C1->CR2 |= I2C_CR2_STOP;
            return 0;
        }
    }

    /* Handle NACK response */
    if (I2C1->ISR & I2C_ISR_NACKF)
    {
        I2C1->ICR |= I2C_ICR_NACKCF;
        I2C1->CR2 |= I2C_CR2_STOP;

        while (!(I2C1->ISR & I2C_ISR_STOPF))
        {
            if (--timeout == 0U)
            {
                return 0;
            }
        }

        I2C1->ICR |= I2C_ICR_STOPCF;

        return 0;
    }

    /* Generate the STOP condition */
    I2C1->CR2 |= I2C_CR2_STOP;

    while (!(I2C1->ISR & I2C_ISR_STOPF))
    {
        if (--timeout == 0U)
        {
            return 0;
        }
    }

    I2C1->ICR |= I2C_ICR_STOPCF;

    /* Device acknowledged */
    return 1;
}

/******************************************************************************
 * @brief Sends data to an I2C slave device.
 *
 * @param slaveaddr 7-bit I2C slave address.
 * @param buffer    Pointer to transmit buffer.
 * @param len       Number of bytes to transmit.
 *
 * @retval 1 Data transmitted successfully.
 * @retval 0 Transmission failed or timeout occurred.
 ******************************************************************************/
uint8_t I2C1_Send(uint8_t slaveaddr, uint8_t *buffer, uint8_t len)
{
    uint32_t timeout = I2C_TIMEOUT;

    /* Wait until the I2C bus becomes idle */
    while (I2C1->ISR & I2C_ISR_BUSY)
    {
        if (--timeout == 0U)
        {
            return 0;
        }
    }

    /* Configure the slave address */
    I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
    I2C1->CR2 |= (slaveaddr << 1);

    /* Configure write transfer */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos);

    /* Generate the START condition */
    I2C1->CR2 |= I2C_CR2_START;

    /* Transmit all bytes */
    for (uint8_t i = 0; i < len; i++)
    {
        timeout = I2C_TIMEOUT;

        /* Wait until TXDR is ready or NACK is received */
        while (!(I2C1->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)))
        {
            if (--timeout == 0U)
            {
                I2C1->CR2 |= I2C_CR2_STOP;
                return 0;
            }
        }

        /* Handle NACK response */
        if (I2C1->ISR & I2C_ISR_NACKF)
        {
            I2C1->ICR |= I2C_ICR_NACKCF;
            I2C1->CR2 |= I2C_CR2_STOP;

            timeout = I2C_TIMEOUT;

            while (!(I2C1->ISR & I2C_ISR_STOPF))
            {
                if (--timeout == 0U)
                {
                    return 0;
                }
            }

            I2C1->ICR |= I2C_ICR_STOPCF;

            return 0;
        }

        /* Write one byte to the transmit data register */
        I2C1->TXDR = buffer[i];
    }

    /* Wait until the transfer completes */
    timeout = I2C_TIMEOUT;

    while (!(I2C1->ISR & I2C_ISR_TC))
    {
        if (--timeout == 0U)
        {
            I2C1->CR2 |= I2C_CR2_STOP;
            return 0;
        }
    }

    /* Generate the STOP condition */
    I2C1->CR2 |= I2C_CR2_STOP;

    timeout = I2C_TIMEOUT;

    while (!(I2C1->ISR & I2C_ISR_STOPF))
    {
        if (--timeout == 0U)
        {
            return 0;
        }
    }

    I2C1->ICR |= I2C_ICR_STOPCF;

    /* Transmission completed successfully */
    return 1;
}

/******************************************************************************
 * @brief Writes data to an I2C slave, then reads data back.
 *
 * @param slaveaddr 7-bit I2C slave address.
 * @param txbuf     Pointer to transmit buffer.
 * @param txlen     Number of bytes to transmit.
 * @param rxbuf     Pointer to receive buffer.
 * @param rxlen     Number of bytes to receive.
 *
 * @retval 1 Write-read operation completed successfully.
 * @retval 0 Communication failed or timeout occurred.
 ******************************************************************************/
uint8_t I2C1_WriteRead(uint8_t slaveaddr,
                       uint8_t *txbuf,
                       uint8_t txlen,
                       uint8_t *rxbuf,
                       uint8_t rxlen)
{
    uint32_t timeout = I2C_TIMEOUT;

    /* Wait until the I2C bus becomes idle */
    while (I2C1->ISR & I2C_ISR_BUSY)
    {
        if (--timeout == 0U)
        {
            return 0;
        }
    }

    /* Configure the slave address */
    I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
    I2C1->CR2 |= (slaveaddr << 1);

    /* Configure write transfer */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= (txlen << I2C_CR2_NBYTES_Pos);

    /* Generate the START condition */
    I2C1->CR2 |= I2C_CR2_START;

    /* Transmit all bytes */
    for (uint8_t i = 0; i < txlen; i++)
    {
        timeout = I2C_TIMEOUT;

        /* Wait until TXDR is ready or NACK is received */
        while (!(I2C1->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)))
        {
            if (--timeout == 0U)
            {
                I2C1->CR2 |= I2C_CR2_STOP;
                return 0;
            }
        }

        /* Handle NACK response */
        if (I2C1->ISR & I2C_ISR_NACKF)
        {
            I2C1->ICR |= I2C_ICR_NACKCF;
            I2C1->CR2 |= I2C_CR2_STOP;

            while (!(I2C1->ISR & I2C_ISR_STOPF))
            {
                if (--timeout == 0U)
                {
                    return 0;
                }
            }

            I2C1->ICR |= I2C_ICR_STOPCF;

            return 0;
        }

        /* Write one byte to the transmit data register */
        I2C1->TXDR = txbuf[i];
    }

    /* Wait until the write transfer completes */
    timeout = I2C_TIMEOUT;

    while (!(I2C1->ISR & I2C_ISR_TC))
    {
        if (--timeout == 0U)
        {
            I2C1->CR2 |= I2C_CR2_STOP;
            return 0;
        }
    }

    /* Configure read transfer */
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= (rxlen << I2C_CR2_NBYTES_Pos);
    I2C1->CR2 |= I2C_CR2_RD_WRN;

    /* Generate the repeated START condition */
    I2C1->CR2 |= I2C_CR2_START;

    /* Receive all bytes */
    for (uint8_t i = 0; i < rxlen; i++)
    {
        timeout = I2C_TIMEOUT;

        /* Wait until receive data is available or NACK is received */
        while (!(I2C1->ISR & (I2C_ISR_RXNE | I2C_ISR_NACKF)))
        {
            if (--timeout == 0U)
            {
                I2C1->CR2 |= I2C_CR2_STOP;
                return 0;
            }
        }

        /* Handle NACK response */
        if (I2C1->ISR & I2C_ISR_NACKF)
        {
            I2C1->ICR |= I2C_ICR_NACKCF;
            I2C1->CR2 |= I2C_CR2_STOP;

            while (!(I2C1->ISR & I2C_ISR_STOPF))
            {
                if (--timeout == 0U)
                {
                    return 0;
                }
            }

            I2C1->ICR |= I2C_ICR_STOPCF;

            return 0;
        }

        /* Read one byte from the receive data register */
        rxbuf[i] = I2C1->RXDR;
    }

    /* Wait until the read transfer completes */
    timeout = I2C_TIMEOUT;

    while (!(I2C1->ISR & I2C_ISR_TC))
    {
        if (--timeout == 0U)
        {
            I2C1->CR2 |= I2C_CR2_STOP;
            return 0;
        }
    }

    /* Generate the STOP condition */
    I2C1->CR2 |= I2C_CR2_STOP;

    while (!(I2C1->ISR & I2C_ISR_STOPF))
    {
        if (--timeout == 0U)
        {
            return 0;
        }
    }

    I2C1->ICR |= I2C_ICR_STOPCF;

    /* Write-read operation completed successfully */
    return 1;
}
