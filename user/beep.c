#include "beep.h"

void beep_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //����GPIO_InitStructure�ṹ����
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);        //ʹ��AHBԤ��Ƶ�����˿�A�Ŀ���

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;            //ָ��ģʽΪ���ܸ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;            //ָ�����Ϊ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //�趨�˿�������50MHz
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;               //ָ����9���
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //�����ϲ�������A��
}
void beep_number(uint16_t pinlv,uint8_t cishu)
{
	beep_io_init();
	while(cishu--)
	{
		beep_H;delay_us(pinlv);beep_L;delay_us(pinlv);
	}

}
