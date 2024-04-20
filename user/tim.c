#include "tim.h"
//初始化定时器和中断?
void TIM3_Int_Init(uint16_t psc,uint16_t arr)
	{
		
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);         //时钟使能
		
		//定时器TIM3初始化?
		TIM_TimeBaseStructure.TIM_Period = arr;                      //设置在下一个更新事件装入活动的自动重装载寄存器周期的值		
		TIM_TimeBaseStructure.TIM_Prescaler =psc;                    //设置用来作为TIMx时钟频率除数的预分频值?
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              //根据指定的参数初始化TIMx的时间基数单位?
	 
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	 
		//?????NVIC??
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPriority = 1;  //先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	 
		TIM_Cmd(TIM3, ENABLE); //使能TIMx				 
	}

//********************************************************************************************
void TIM16_Init(uint16_t psc, uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;              //声明TIM_TimeBaseStructure结构变量
	NVIC_InitTypeDef NVIC_InitStructure;                        //声明NVIC_InitTypeDef 向量结构变量
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);       //使能AHB预分频器到TIM14
	/* 定时器基础设置*/    
	TIM_TimeBaseStructure.TIM_Period = arr;                     //设置TIM14自动重装周期值=47999+1=48000
	TIM_TimeBaseStructure.TIM_Prescaler = psc;                  //定时器的时钟频率=TIM_Prescaler+1=40；
																														  //频率=48M/40/48000=25，时间=1/25=40ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 1;                //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;  //向上计数
	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);            //按以上参数初始化TIM14
	//************
	// TIM16中断嵌套设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;           //选择TIM16的IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;            //IRQ通道优先级=0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能IRQ通道
	NVIC_Init(&NVIC_InitStructure);                            //按以参数上设置

	TIM_ARRPreloadConfig(TIM16, DISABLE);                      //禁止ARR预装载缓冲器
	TIM_ITConfig(TIM16,TIM_IT_Update, ENABLE);                 //使能TIM16溢出中断
	TIM_Cmd(TIM16, ENABLE);                                    //使能TIM16 总中断
}

//void pwm_io_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;                       //声明GPIO_InitStructure结构变量

//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);         //使能AHB预分频器到端口A的开关

//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_4);         //A4口功能复用选择AF_4（Tim14_CH1)

//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               //指定模式为功能复用
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             //指定输出为推挽输出
//  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //内部上拉输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //设定端口最快输出50MHz

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                  //指定脚4输出
//  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //按以上参数设置A口
//}

//void TIM14_PWM_Init(uint16_t period, uint16_t prescaler)
//{
//	TIM_OCInitTypeDef TIM_OCInitStructure;                     //声明TIM_OCInitStructure结构变量
//	GPIO_InitTypeDef GPIO_InitStructure;                       //声明GPIO_InitStructure结构变量

//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);         //使能AHB预分频器到端口A的开关
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);       //使能AHB预分频器到TIM14

//  GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_4);         //A4口功能复用选择AF_4（Tim14_CH1)
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;               //指定模式为功能复用
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             //指定输出为推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;          //设定端口最快输出50MHz
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                  //指定脚4输出
//  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //按以上参数设置A口
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;             //声明TIM_TimeBaseStructure结构变量


//	TIM_TimeBaseStructure.TIM_Period        = period;          //设置TIM14自动重装周期值
//	TIM_TimeBaseStructure.TIM_Prescaler     = prescaler;       //设置TIM14预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;               //设置时钟分割
//	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up; //向上计数
//	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);           //按以上参数初始化TIM14

//	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;            //PWM2模式
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;    //输出极性TIM_OCPolarity_High正极性；TIM_OCPolarity_Low 负极性。
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//比较输出使能

//	TIM_OCInitStructure.TIM_Pulse=14400;                       //占空比=240/24000*100=1%
//	TIM_OC1Init(TIM14,&TIM_OCInitStructure);                   //按以上参数初始化TIM14CH1
//	TIM_Cmd(TIM14,ENABLE);                                     //使能TIM14
//}



