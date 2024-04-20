#ifndef _BEEP_H
#define _BEEP_H
#include "delay.h"
#include "stm32f0xx.h"

#define beep_H     GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define beep_L   GPIO_ResetBits(GPIOA,GPIO_Pin_1)

void beep_io_init(void);
void beep_number(uint16_t pinlv,uint8_t cishu);



#endif

