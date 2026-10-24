/*
 * main.c
 * UART Command-Line Interface for STM32U083RC
 */

#include <string.h>
#include "gpio.h"
#include "motor.h"
#include "uart.h"

/* Command Buffer */
char cmd[21];

int main(void)
{
    /* Initialize Peripherals */
    UART_Init();
    GPIO_Init();
    Motor_Init();

    /* Display Help Menu */
    Help();

    /* Display Command Prompt */
    UART_SendString("> ");

    while(1)
    {
        /* Read User Command */
        UART_ReadString(cmd);

        if(cmd[0] == '\0')
        {
            UART_SendString("> ");
            continue;
        }

        /* LD4 Commands */
        if(strcmp(cmd, "LD4ON") == 0)
        {
            LD4_On();
            UART_SendString("LD4 ON\r\n");
        }

        else if(strcmp(cmd, "LD4OFF") == 0)
        {
            LD4_Off();
            UART_SendString("LD4 OFF\r\n");
        }

        else if(strcmp(cmd, "LD4STATUS") == 0)
        {
            LD4_Status();
        }

        /* External LED Commands */
        else if(strncmp(cmd, "LEDON ", 6) == 0)
        {
            int led = my_atoi(&cmd[6]);

            switch(led)
            {
                case 1:
                    LED1_On();
                    UART_SendString("LED1 ON\r\n");
                    break;

                case 2:
                    LED2_On();
                    UART_SendString("LED2 ON\r\n");
                    break;

                case 3:
                    LED3_On();
                    UART_SendString("LED3 ON\r\n");
                    break;

                case 4:
                    LED4_On();
                    UART_SendString("LED4 ON\r\n");
                    break;

                default:
                    UART_SendString("INVALID LED NUMBER\r\n");
                    break;
            }
        }

        else if(strncmp(cmd, "LEDOFF ", 7) == 0)
        {
            int led = my_atoi(&cmd[7]);

            switch(led)
            {
                case 1:
                    LED1_Off();
                    UART_SendString("LED1 OFF\r\n");
                    break;

                case 2:
                    LED2_Off();
                    UART_SendString("LED2 OFF\r\n");
                    break;

                case 3:
                    LED3_Off();
                    UART_SendString("LED3 OFF\r\n");
                    break;

                case 4:
                    LED4_Off();
                    UART_SendString("LED4 OFF\r\n");
                    break;

                default:
                    UART_SendString("INVALID LED NUMBER\r\n");
                    break;
            }
        }

        else if(strcmp(cmd, "ALLLEDON") == 0)
        {
            AllLED_On();
            UART_SendString("ALL LEDs ON\r\n");
        }

        else if(strcmp(cmd, "ALLLEDOFF") == 0)
        {
            AllLED_Off();
            UART_SendString("ALL LEDs OFF\r\n");
        }

        /* Buzzer Commands */
        else if(strcmp(cmd, "BUZON") == 0)
        {
            Buzzer_On();
            UART_SendString("BUZZER ON\r\n");
        }

        else if(strcmp(cmd, "BUZOFF") == 0)
        {
            Buzzer_Off();
            UART_SendString("BUZZER OFF\r\n");
        }

        /* Button Commands */
        else if(strcmp(cmd, "USERBTN") == 0)
        {
            UserButton_Read();
        }

        else if(strcmp(cmd, "EXTBTN") == 0)
        {
            ExternalButton_Read();
        }

        /* Motor Commands */
        else if(strcmp(cmd, "MOTORFWD") == 0)
        {
            Motor_Forward();
        }

        else if(strcmp(cmd, "MOTORREV") == 0)
        {
            Motor_Reverse();
        }

        else if(strcmp(cmd, "MOTORSTOP") == 0)
        {
            Motor_Stop();
        }

        /* Help Command */
        else if(strcmp(cmd, "HELP") == 0)
        {
            Help();
        }

        /* Invalid Command */
        else
        {
            UART_SendString("INVALID COMMAND\r\n");
        }

        /* Display Command Prompt */
        UART_SendString("> ");
    }
}
