#ifndef _ENCODE_H
#define _ENCODE_H
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "stm32f0xx_gpio.h"
#include "beep.h"


void encode_readio(void);
void encode_io_init(void);
void encode(void);

#endif
