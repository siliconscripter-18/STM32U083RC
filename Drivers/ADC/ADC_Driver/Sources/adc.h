/******************************************************************************
 * @file    ADC.h
 * @brief   ADC driver function declarations.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_ADC_H_
#define SOURCES_ADC_H_

/******************************************************************************
 * @brief ADC initialization and read functions.
 ******************************************************************************/

void ADC_Init(void);

uint16_t ADC_Read(void);

#endif /* SOURCES_ADC_H_ */