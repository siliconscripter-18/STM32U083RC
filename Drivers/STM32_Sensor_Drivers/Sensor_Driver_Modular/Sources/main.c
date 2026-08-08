#include <stdint.h>
#include <stdio.h>
#include "registers.h"
#include "uart.h"
#include "timer3.h"
#include "button.h"
#include "dht22.h"
#include "hc-sr04.h"

int main(void)
{
    float humidity, temp;
    uint16_t distance = 0;
    uint8_t buttonpressed = 0;
    char buffer[50];

    UART_Init();
    TIM3_Init();
    Button_Init();
    DHT22_Init();
    Ultrasonic_Init();

    UART_SendString("Press Ext.Push Button1 (PA6) for DHT22 and Ext.Push Button2 (PA7) for HC-SR04 Data(s)...\r\n");

    while (1)
    {
        buttonpressed = 0; // reset each loop

        /* --- Check buttons --- */
        if (!(GPIOA_IDR & (1 << PA6)))
        {
            delay_ms(50); // debounce
            if (!(GPIOA_IDR & (1 << PA6))) // confirm still pressed
            {
                buttonpressed = 1;
            }
        }

        else if (!(GPIOA_IDR & (1 << PA7)))
        {
            delay_ms(50); // debounce
            if (!(GPIOA_IDR & (1 << PA7))) // confirm still pressed
            {
                buttonpressed = 2;
            }
        }

        /* --- Branch by button --- */
        switch (buttonpressed)
        {


        case 1: if (DHT22_Read(&humidity, &temp))
                {
                    snprintf(buffer, sizeof(buffer),"Humidity: %.1f %%  Temperature: %.1f C\r\n", humidity, temp); // @suppress("Float formatting support")
                }

                else
                {
                    snprintf(buffer, sizeof(buffer),"DHT22 Read Error\r\n");
                }

                UART_SendString(buffer);

                while (!(GPIOA_IDR & (1 << PA6))); // wait release
                delay_ms(200);
                break;


        case 2: if (UltraSonicMeasure(&distance))
                {
                    snprintf(buffer, sizeof(buffer),"Distance: %u cm\r\n", distance);
                }

                else
                {
                    snprintf(buffer, sizeof(buffer),"HC-SR04 Measurement Error\r\n");
                }

                UART_SendString(buffer);

                while (!(GPIOA_IDR & (1 << PA7))); // wait release
                delay_ms(200);
                break;

        default:   break;

        }
    }
}
