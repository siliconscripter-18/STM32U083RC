#include "gpio.h"
#include "motor.h"
#include "uart.h"

char c;

int main(void)
{
    UART_Init();

    LED_Init();

    Button_Init();

    Motor_Init();

    Help();


    while(1)
    {

        c = UART_ReadChar();

        if(c =='L' || c =='l'){

        	LED_On();
        }

        else if(c =='O' || c =='o'){

        	LED_Off();
        }

        else if(c =='s' || c =='S'){

        	LED_Status();
        }

        else if(c =='b' || c =='B'){

        	User_Button();
        }

        else if(c == 'F' || c == 'f'){

        	Motor_Forward();

        }

        else if(c == 'R' || c == 'r'){

            Motor_Reverse();
        }

        else if(c== 'X' || c == 'x'){

        	Motor_Stop();
        }

        else if(c == 'h' || c == 'H'){

        	Help();
        }

        else{

        	UART_SendString("INVALID COMMAND\r\n");

        }

    }
}
