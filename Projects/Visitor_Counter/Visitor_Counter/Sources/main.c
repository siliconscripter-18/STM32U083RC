#include <stdint.h>
#include "stm32u083xx.h"
#include "SysTick.h"
#include "EXTI.h"
#include "gpio.h"
#include "i2c.h"
#include "ssd1306.h"
#include "display.h"
#include "EEPROM.h"

#define EEPROM_IN_COUNT_ADDR     0x0000
#define EEPROM_OUT_COUNT_ADDR    0x0004
#define SPLASH_SCREEN_TIME_MS 	 2000U

volatile uint8_t InLock = 0;
volatile uint8_t OutLock = 0;

volatile uint16_t InCount  = 0;
volatile uint16_t OutCount = 0;
volatile uint16_t Present  = 0;


int main(void)
{
    GPIO_Init();
    SysTick_Init();
    EXTI_Init();
    I2C1_Init();
    SSD1306_Init();

    EEPROM_ReadUint16(EEPROM_IN_COUNT_ADDR, &InCount);
    EEPROM_ReadUint16(EEPROM_OUT_COUNT_ADDR, &OutCount);
    Present = InCount - OutCount;

    SSD1306_ShowSplash();
    SSD1306_ShowLayout();
    Display_UpdateVisitorCounts(InCount,OutCount,Present);


    while(1)
    {

    		if (InFlag)
    		{
    		    InFlag = 0;

    		    if(InLock==0)
    		    {
    		    	InLock = 1;

    		    InCount++;
    		    Present++;
    		    Buzzer_Beep();
    		    EEPROM_WriteUint16(EEPROM_IN_COUNT_ADDR,InCount);
    		    Display_UpdateVisitorCounts(InCount,OutCount,Present);

    		    }
    		}

    		/* Unlock only after beam is clear */
    		if ((GPIOA->IDR & GPIO_IDR_ID0 ) != 0)
    		{
    		    InLock = 0;
    		}

    		if (OutFlag)
    		{
    		    OutFlag = 0;

    		    if(OutLock==0)
    		    {
    		    	OutLock = 1;

    		    if (Present > 0)
    		    {
    		        OutCount++;
    		        Present--;
    		        EEPROM_WriteUint16(EEPROM_OUT_COUNT_ADDR,OutCount);
    		        Display_UpdateVisitorCounts(InCount,OutCount,Present);

    		    }

    		    }
    		}

    		/* Unlock only after beam is clear */
    			if ((GPIOA->IDR & GPIO_IDR_ID1) != 0)
    			{
    				OutLock = 0;
    			}

    			if(Present>0)
    		    {
    		         LED_On();
    		    }

    		    else
    		    {
    		         LED_Off();
    		    }

    }
}








