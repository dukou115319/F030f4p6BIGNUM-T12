#ifndef _TIM_H
#define _TIM_H
#include "stm32f0xx.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

//void pwm_io_init(void);
void TIM3_Int_Init(uint16_t arr,uint16_t psc);
void TIM16_Init(uint16_t arr, uint16_t psc);
//void TIM14_PWM_Init(uint16_t period, uint16_t prescaler);

#endif
