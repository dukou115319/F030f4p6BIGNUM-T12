#ifndef _ADC_H
#define _ADC_H

#include "stm32f0xx.h"
#include "delay.h"
#include "stm32f0xx_adc.h"

void ADC_GPIO_Init(void);
uint16_t ADC1_Init(uint16_t ADC_tondao);  
//uint16_t Get_Average(uint16_t ch,uint8_t times);
uint16_t get_adc(uint16_t ch);

#endif
