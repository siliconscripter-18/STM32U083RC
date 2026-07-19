#include <stdint.h>
#include <stdio.h>
#include "registers.h"
#include "uart.h"
#include "timer3.h"
#include "button.h"
#include "dht22.h"

int main(void)
{
    float humidity, temp;
    char buffer[50];

    UART_Init();
    TIM3_Init();
    Button_Init();
    DHT22_Init();

    UART_SendString("Press OnBoard Push Button for Sensor Data...\r\n");

    while (1)
    {
        /* Button pressed? (PC13 is active LOW) */
        if (!(GPIOC_IDR & (1 << PC13)))
        {
            /* Debounce delay */
            delay_ms(50);

            /* Wait for button release */
            while (!(GPIOC_IDR & (1 << PC13)));

            /* Read DHT22 sensor */
            if (DHT22_Read(&humidity, &temp))
            {
                snprintf(buffer, sizeof(buffer), // @suppress("Float formatting support")
                         "Humidity: %.1f %%  Temp: %.1f C\r\n",
                         humidity, temp);
                UART_SendString(buffer);
            }
            else
            {
                UART_SendString("Checksum Error\r\n");
            }

            /* DHT22 requires at least 2 seconds between readings */
            delay_ms(2000);
        }
    }
}
