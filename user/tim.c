#include "tim.h"
//��ʼ����ʱ�����ж�?
void TIM3_Int_Init(uint16_t psc,uint16_t arr)
	{
		
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);         //ʱ��ʹ��
		
		//��ʱ��TIM3��ʼ��?
		TIM_TimeBaseStructure.TIM_Period = arr;                      //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ		
		TIM_TimeBaseStructure.TIM_Prescaler =psc;                    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ?
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ?
	 
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	 
		//?????NVIC??
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPriority = 1;  //��ռ���ȼ�0��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	 
		TIM_Cmd(TIM3, ENABLE); //ʹ��TIMx				 
	}

//********************************************************************************************
void TIM16_Init(uint16_t psc, uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;              //����TIM_TimeBaseStructure�ṹ����
	NVIC_InitTypeDef NVIC_InitStructure;                        //����NVIC_InitTypeDef �����ṹ����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);       //ʹ��AHBԤ��Ƶ����TIM14
	/* ��ʱ����������*/    
	TIM_TimeBaseStructure.TIM_Period = arr;                     //����TIM14�Զ���װ����ֵ=47999+1=48000
	TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //��ʱ����ʱ��Ƶ��=TIM_Prescaler+1=40��
																														  //Ƶ��=48M/40/48000=25��ʱ��=1/25=40ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 1;                //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;  //���ϼ���
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);            //�����ϲ�����ʼ��TIM14
	//************
	// TIM16�ж�Ƕ������
	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;           //ѡ��TIM16��IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;            //IRQͨ�����ȼ�=0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //ʹ��IRQͨ��
	NVIC_Init(&NVIC_InitStructure);                            //���Բ���������

	TIM_ARRPreloadConfig(TIM16, DISABLE);                      //��ֹARRԤװ�ػ�����
	TIM_ITConfig(TIM16,TIM_IT_Update, ENABLE);                 //ʹ��TIM16����ж�
	TIM_Cmd(TIM16, ENABLE);                                    //ʹ��TIM16 ���ж�
}

//void pwm_io_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;                       //����GPIO_InitStructure�ṹ����

//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);         //ʹ��AHBԤ��Ƶ�����˿�A�Ŀ���

//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_4);         //A4�ڹ��ܸ���ѡ��AF_4��Tim14_CH1)

//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               //ָ��ģʽΪ���ܸ���
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             //ָ�����Ϊ�������
//  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //�ڲ��������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //�趨�˿�������50MHz

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                  //ָ����4���
//  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //�����ϲ�������A��
//}

//void TIM14_PWM_Init(uint16_t period, uint16_t prescaler)
//{
//	TIM_OCInitTypeDef TIM_OCInitStructure;                     //����TIM_OCInitStructure�ṹ����
//	GPIO_InitTypeDef GPIO_InitStructure;                       //����GPIO_InitStructure�ṹ����

//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);         //ʹ��AHBԤ��Ƶ�����˿�A�Ŀ���
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);       //ʹ��AHBԤ��Ƶ����TIM14

//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_4);         //A4�ڹ��ܸ���ѡ��AF_4��Tim14_CH1)
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               //ָ��ģʽΪ���ܸ���
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             //ָ�����Ϊ�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //�趨�˿�������50MHz
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                  //ָ����4���
//  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //�����ϲ�������A��
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;             //����TIM_TimeBaseStructure�ṹ����


//	TIM_TimeBaseStructure.TIM_Period        = period;          //����TIM14�Զ���װ����ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler     = prescaler;       //����TIM14Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;               //����ʱ�ӷָ�
//	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up; //���ϼ���
//	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);           //�����ϲ�����ʼ��TIM14

//	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;            //PWM2ģʽ
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;    //�������TIM_OCPolarity_High�����ԣ�TIM_OCPolarity_Low �����ԡ�
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//�Ƚ����ʹ��

//	TIM_OCInitStructure.TIM_Pulse=14400;                       //ռ�ձ�=240/24000*100=1%
//	TIM_OC1Init(TIM14,&TIM_OCInitStructure);                   //�����ϲ�����ʼ��TIM14CH1
//	TIM_Cmd(TIM14,ENABLE);                                     //ʹ��TIM14
//}



