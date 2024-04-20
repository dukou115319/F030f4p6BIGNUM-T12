#include "beep.h"

void beep_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //声明GPIO_InitStructure结构变量
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);        //使能AHB预分频器到端口A的开关

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;            //指定模式为功能复用
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //指定输出为推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //设定端口最快输出50MHz
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;               //指定脚9输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //按以上参数设置A口
}
void beep_number(uint16_t pinlv,uint8_t cishu)
{
	beep_io_init();
	while(cishu--)
	{
		beep_H;delay_us(pinlv);beep_L;delay_us(pinlv);
	}

}
