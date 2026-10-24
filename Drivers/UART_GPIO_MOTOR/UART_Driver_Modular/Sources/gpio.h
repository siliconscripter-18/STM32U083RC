#ifndef SOURCES_GPIO_H_
#define SOURCES_GPIO_H_

/* GPIO Initialization */
void GPIO_Init(void);

/* On-board LED (LD4) */
void LD4_On(void);
void LD4_Off(void);
void LD4_Status(void);

/* External LEDs */
void LED1_On(void);
void LED1_Off(void);

void LED2_On(void);
void LED2_Off(void);

void LED3_On(void);
void LED3_Off(void);

void LED4_On(void);
void LED4_Off(void);

/* External LED Group */
void AllLED_On(void);
void AllLED_Off(void);

/* Buzzer */
void Buzzer_On(void);
void Buzzer_Off(void);

/* Buttons */
void UserButton_Read(void);      // PC13 (On-board User Button)
void ExternalButton_Read(void);  // PA1 (External Button)

#endif /* SOURCES_GPIO_H_ */
