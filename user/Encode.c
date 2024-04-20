#include "Encode.h"

/*±àÂëÆ÷A-GPIOA-6*/
/*±àÂëÆ÷B-GPIOA-7*/
/*±àÂëÆ÷K-GPIOA-10*/
extern uint16_t shedingwendu,huancun;
uint16_t encodea,encodeb,encodek,state=0;
void encode_io_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_5;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void encode_readio()
{
	encode_io_init();
	encodea = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	encodeb = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
	encodek = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
}
	
void encode(void)
{

	if(encodea==1&&encodeb==1&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==1)
	  {
			 shedingwendu=shedingwendu-5;
			 if(shedingwendu<=100)
				 shedingwendu=100;
			   beep_number(500,10);
		}
	if(encodea==1&&encodeb==1&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==1&&GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0)
			 {
				shedingwendu=shedingwendu+5;
				 if(shedingwendu>=480)
					 shedingwendu=480;
				  beep_number(200,30);
			 }
		if(encodek==0&&state==0)
			{
				shedingwendu=300;state=1;
			}
		 else if(encodek==0&&state==1)
		 {
			 shedingwendu=100;state=0;
		 }
    encode_readio();		
}
