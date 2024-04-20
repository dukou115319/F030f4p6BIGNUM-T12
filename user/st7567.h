#ifndef _ST7567_H
#define _ST7567_H

#include "stm32f0xx.h"
#include "delay.h"

#define SDA_L  GPIO_ResetBits(GPIOF,GPIO_Pin_0)
#define SDA_H  GPIO_SetBits(GPIOF,GPIO_Pin_0)

#define SCL_L  GPIO_ResetBits(GPIOF,GPIO_Pin_1)
#define SCL_H  GPIO_SetBits(GPIOF,GPIO_Pin_1)

#define CD_L   GPIO_ResetBits(GPIOA,GPIO_Pin_0)
#define CD_H   GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define RST_L  GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define RST_H  GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define CS_L   GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#define CS_H   GPIO_SetBits(GPIOA,GPIO_Pin_9)

void ST7567_GPIO_Init(void);
void ST7567_WriteCommand(uint8_t  COM);
void ST7567_WriteData(uint8_t date);
void HDReset(void);
void st7567_ClearScreen(void);
void ST7567_Init(void);
void st7567_Refresh_Gram(void);
void ST7567_draw_point(uint8_t x,uint8_t y,uint8_t t);
void st7567_DrawPlusSign(uint8_t x,uint8_t y);
void st7567_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void st7567_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
uint32_t mypow(uint8_t m,uint8_t n);
void st7567_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);
void st7567_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t size,uint8_t mode);
void st7567_ShowChinese(uint16_t x,uint16_t y,uint8_t index,uint8_t size,uint8_t mode);
void OLED_DrawPointNum(uint8_t x,uint8_t y,uint8_t index,uint8_t mode);
#endif


