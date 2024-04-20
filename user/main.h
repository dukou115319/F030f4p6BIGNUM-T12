#ifndef _MAIN_H
#define _MAIN_H
#include "delay.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "tim.h"
#include "beep.h"
#include "st7567.h"
#include "Encode.h"
#include "adc.h"
#include "stm32f0xx_iwdg.h"
#define T12_H		GPIO_SetBits(GPIOA,GPIO_Pin_4 )
#define T12_L		GPIO_ResetBits( GPIOA,GPIO_Pin_4 )

void T12_CRL_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
}

#endif
