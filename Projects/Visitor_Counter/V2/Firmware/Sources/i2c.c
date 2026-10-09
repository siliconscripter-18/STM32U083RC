/******************************************************************************
 * @file    i2c.c
 * @brief   I2C1 driver for STM32U083RC.
 *
 *          Provides low-level blocking I2C communication routines used by
 *          external I2C devices such as the AT24C04 EEPROM and SSD1306 OLED.
 *          The driver supports device detection, transmit operations, and
 *          combined write-read transactions with timeout and NACK handling.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>

#include "i2c.h"
#include "stm32u083xx.h"

#define I2C_TIMEOUT    100000U


/******************************************************************************
 * @brief Initializes I2C1 in standard mode.
 *
 *          PB8 and PB9 are configured as open-drain alternate-function pins
 *          for I2C1 SCL and SDA respectively. The I2C peripheral is configured
 *          for a 100 kHz bus speed.
 *
 *          Pin mapping:
 *          PB8 -> I2C1_SCL
 *          PB9 -> I2C1_SDA
 ******************************************************************************/
void I2C1_Init(void)
{
    /* Enable GPIOB and I2C1 peripheral clocks */
    RCC->IOPENR  |= RCC_IOPENR_GPIOBEN;
    RCC->APBENR1 |= RCC_APBENR1_I2C1EN;


    /* Configure PB8 as alternate function 4 (I2C1 SCL) */
    GPIOB->MODER &= ~GPIO_MODER_MODE8_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE8_1;

    /* Configure PB9 as alternate function 4 (I2C1 SDA) */
    GPIOB->MODER &= ~GPIO_MODER_MODE9_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE9_1;

    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL8_Msk;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL8_2;

    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL9_Msk;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL9_2;


    /* Configure I2C pins as open-drain */
    GPIOB->OTYPER |= GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9;

    /* Configure low-speed GPIO operation */
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8_0 |
                      GPIO_OSPEEDR_OSPEED9_0;


    /*
     * Configure I2C timing for standard-mode operation at approximately
     * 100 kHz with the selected peripheral clock.
     */
    I2C1->TIMINGR = 0x00303D5B;

    /* Enable I2C1 peripheral */
    I2C1->CR1 |= I2C_CR1_PE;
}


/******************************************************************************
 * @brief Checks whether an I2C slave acknowledges its address.
 *
 *          Performs a zero-byte write transaction to determine whether the
 *          addressed device is present and responding on the I2C bus.
 *
 * @param slaveaddr 7-bit I2C slave address.
 *
 * @retval 1 Device acknowledged.
 * @retval 0 Device did not respond or a timeout occurred.
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


    /* Configure the 7-bit slave address */
    I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
    I2C1->CR2 |= (slaveaddr << 1);


    /* Configure a zero-byte write transfer */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;


    /* Generate START condition */
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


    /* Generate STOP condition */
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

    return 1;
}


/******************************************************************************
 * @brief Sends a buffer of data to an I2C slave device.
 *
 *          The function waits for the bus to become idle, transmits all bytes,
 *          checks for NACK conditions, and generates a STOP condition after
 *          the transfer completes.
 *
 * @param slaveaddr 7-bit I2C slave address.
 * @param buffer    Pointer to the transmit buffer.
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


    /* Configure the 7-bit slave address */
    I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
    I2C1->CR2 |= (slaveaddr << 1);


    /* Configure write transfer */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= (len << I2C_CR2_NBYTES_Pos);


    /* Generate START condition */
    I2C1->CR2 |= I2C_CR2_START;


    /* Transmit all bytes */
    for (uint8_t i = 0; i < len; i++)
    {
        timeout = I2C_TIMEOUT;

        /* Wait until TXDR is ready or a NACK is received */
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


        /* Write the next byte to the transmit data register */
        I2C1->TXDR = buffer[i];
    }


    /* Wait for the complete transfer */
    timeout = I2C_TIMEOUT;

    while (!(I2C1->ISR & I2C_ISR_TC))
    {
        if (--timeout == 0U)
        {
            I2C1->CR2 |= I2C_CR2_STOP;
            return 0;
        }
    }


    /* Generate STOP condition */
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

    return 1;
}


/******************************************************************************
 * @brief Performs a combined I2C write-read transaction.
 *
 *          The function first writes the requested bytes to the slave and
 *          then performs a repeated START followed by a read transaction.
 *          This transaction structure is required by devices such as EEPROMs
 *          where a memory address must be written before reading data.
 *
 * @param slaveaddr 7-bit I2C slave address.
 * @param txbuf     Pointer to the transmit buffer.
 * @param txlen     Number of bytes to transmit.
 * @param rxbuf     Pointer to the receive buffer.
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


    /* Configure the 7-bit slave address */
    I2C1->CR2 &= ~I2C_CR2_SADD_Msk;
    I2C1->CR2 |= (slaveaddr << 1);


    /* Configure write transfer */
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= (txlen << I2C_CR2_NBYTES_Pos);


    /* Generate START condition */
    I2C1->CR2 |= I2C_CR2_START;


    /* Transmit all write bytes */
    for (uint8_t i = 0; i < txlen; i++)
    {
        timeout = I2C_TIMEOUT;

        /* Wait until TXDR is ready or a NACK is received */
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


        /* Write the next byte to the transmit data register */
        I2C1->TXDR = txbuf[i];
    }


    /* Wait until the write phase completes */
    timeout = I2C_TIMEOUT;

    while (!(I2C1->ISR & I2C_ISR_TC))
    {
        if (--timeout == 0U)
        {
            I2C1->CR2 |= I2C_CR2_STOP;
            return 0;
        }
    }


    /* Configure the read phase */
    I2C1->CR2 &= ~I2C_CR2_NBYTES;
    I2C1->CR2 |= (rxlen << I2C_CR2_NBYTES_Pos);
    I2C1->CR2 |= I2C_CR2_RD_WRN;


    /* Generate repeated START condition */
    I2C1->CR2 |= I2C_CR2_START;


    /* Receive all requested bytes */
    for (uint8_t i = 0; i < rxlen; i++)
    {
        timeout = I2C_TIMEOUT;

        /* Wait until RXDR contains data or a NACK is received */
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


        /* Read the received byte */
        rxbuf[i] = I2C1->RXDR;
    }


    /* Wait until the read phase completes */
    timeout = I2C_TIMEOUT;

    while (!(I2C1->ISR & I2C_ISR_TC))
    {
        if (--timeout == 0U)
        {
            I2C1->CR2 |= I2C_CR2_STOP;
            return 0;
        }
    }


    /* Generate STOP condition */
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

    return 1;
}
