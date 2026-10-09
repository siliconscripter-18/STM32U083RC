/******************************************************************************
 * @file    uart.c
 * @brief   USART2 UART driver for STM32U083RC.
 *
 *          PA2 -> USART2_TX
 *          PA3 -> USART2_RX
 *
 *          USART2 is connected to the ST-LINK Virtual COM Port.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "uart.h"

/******************************************************************************
 * @brief Initializes USART2 for UART communication.
 *
 *          Baud rate : 115200
 *          Data      : 8 bits
 *          Parity    : None
 *          Stop bits : 1
 ******************************************************************************/
void UART_Init(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Configure PA2 and PA3 as Alternate Function */
    GPIOA->MODER &= ~(GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE3_Msk);
    GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;

    /* Select AF7 for USART2 */
    GPIOA->AFR[0] &= ~((0xFU << 8) | (0xFU << 12));
    GPIOA->AFR[0] |= (7U << 8) | (7U << 12);

    /* Enable USART2 clock */
    RCC->APBENR1 |= RCC_APBENR1_USART2EN;

    /* Configure baud rate for 115200 baud at 4 MHz */
    USART2->BRR = 35U;

    /* Enable transmitter and USART */
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

uint8_t UART_Available(void)
{
    return (USART2->ISR & USART_ISR_RXNE_RXFNE) ? 1U : 0U;
}

char UART_ReadChar(void)
{
    while (!(USART2->ISR & USART_ISR_RXNE_RXFNE));

    return (char)USART2->RDR;
}

uint8_t UART_ProcessCommand(char *buffer)
{
    static uint8_t index = 0U;
    char c;

    if (!UART_Available())
    {
        return 0U;
    }

    c = UART_ReadChar();

    if (c == '\r')
    {
        buffer[index] = '\0';

        UART_SendString("\r\n");

        for (uint8_t i = 0U; buffer[i] != '\0'; i++)
        {
            if (buffer[i] >= 'a' && buffer[i] <= 'z')
            {
                buffer[i] -= 32;
            }
        }

        index = 0U;

        return 1U;
    }

    if (c == '\n')
    {
        return 0U;
    }

    if (index >= MAX_CMD_LEN - 1U)
    {
        UART_SendString("\r\nERROR: Command too long\r\n");

        index = 0U;

        return 1U;
    }

    buffer[index++] = c;

    UART_SendChar(c);

    return 0U;
}
/******************************************************************************
 * @brief Sends one character through USART2.
 *
 * @param c Character to transmit.
 ******************************************************************************/
void UART_SendChar(char c)
{
    /* Wait until transmit data register is ready */
    while (!(USART2->ISR & (1U << 7)))
    {
    }

    /* Write character to transmit data register */
    USART2->TDR = c;
}

/******************************************************************************
 * @brief Sends a null-terminated string through USART2.
 *
 * @param msg Pointer to the string to transmit.
 ******************************************************************************/
void UART_SendString(const char *msg)
{
    while (*msg)
    {
        UART_SendChar(*msg++);
    }
}
