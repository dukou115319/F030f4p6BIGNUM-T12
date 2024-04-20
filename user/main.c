#include "main.h"

 uint8_t   setflag;
 //uint8_t   heat;		 //LCD屏幕烙铁加热指示
 uint16_t  zhouqijishu;  //加热周期200ms计数
 uint16_t  jiareshu;     //每200ms加热周期内需要加热的次数(一次等于1ms,相当于加热占空比)
 uint16_t  t12wendu;     //T12烙铁头的实际温度(非热电偶的温差)(同样为10倍温度)
 uint16_t  t12dianya;    //T12烙铁头的实际电压
 uint16_t  shedingwendu; //设定温度(范围200~450度)
 uint16_t  wencha;       //T12两个周期间的温差
 uint32_t  shurudianya;  //PD充电头输入电压
/*****************激进PID*******************/
 float aggkp=10;    //比例参数，设置调节力度
 float aggki=6;		  //积分参数T/Ti，可以消除稳态误差
 float aggkd=1;		  //微分参数Td/T，可以预测误差的变化，做到提前决策
/*****************保守的PID参数*************/
 float consKp=6;    //保守的PID参数
 float consKi=4;
 float consKd=3;
 
 int error1=0,error2=0,error3=0;
 
 /****************LCD显示**************/
void lcd_show(uint16_t a)
{
	  uint8_t bai,shi,ge;
	  st7567_ShowString(0,0,(uint8_t *)"STemp",12,1);
	  st7567_ShowNum(30,0,shedingwendu,3,12,1);
		st7567_ShowString(70,0,(uint8_t *)"Heat",12,1);
	  st7567_ShowNum(99,0,jiareshu/2,3,12,1);
		st7567_ShowChar(120,0,'%',12,1);
		bai=a/100;
		shi=a%100/10;
		ge =a%10;
		OLED_DrawPointNum(20,12,bai*6,1);
		OLED_DrawPointNum(45,12,shi*6,1);
		OLED_DrawPointNum(70,12,ge*6,1);		
	  st7567_ShowChinese(95,20,0,16,1);
	  st7567_ShowString(0,50,(uint8_t *)"Vin",12,1);
	  st7567_ShowNum(100,50,shurudianya/10,3,12,1);
		st7567_ShowChar(118,50,'V',12,1);
		st7567_Refresh_Gram();
}
/****************PID函数*****************/
 int PID(void)
 {
	 int out;
	 error3 = shedingwendu - t12wendu;
	 if(error3>50) out=200;
	 else if(error3>4)
	 {
		out = aggkp*(error3-error2)+aggki*(error3)+aggkd*(error3-2*error2+error1);
	 }
	 else
	 {
		out =consKp*(error3-error2)+consKi*(error3)+consKd*(error3-2*error2+error1);
	 }
	 error1=error2;    //这部分是迭代，因为上次的误差就是上次的当前误差
     error2=error3;
	 if(t12wendu>shedingwendu)
		out=0;
   return out;
 }
 
/*******************控制输出**********************************/
void Control_out(void)
{
		zhouqijishu++;//加热周期计数
		if(jiareshu>=200)//最多加热50ms
		  jiareshu=200;
		if(zhouqijishu<=jiareshu)//如果当前计数小于等于加热数
		{T12_H;}
		else
		{T12_L;}
		if(t12wendu>500)//如果t12温度为500,说明没有插入烙铁头或参数严重错误
			jiareshu=0;
		if(zhouqijishu==220)//t12停止加热后10ms再检测温度
		{
		 jiareshu=0;
		 t12wendu=get_adc(ADC_Channel_3);
		 zhouqijishu=0;//重新开始加热周期计数    
		}
		 jiareshu=PID();
}
void IWDG_init(uint16_t wt)
{
    IWDG->KR = 0x5555;			  //	Key for unlock timer register
    IWDG->PR = 7;				      //	Clock divider 256 (40kHz/256)
    IWDG->RLR = (wt*40)>>8;		//	Reload register update
    IWDG->KR = 0xAAAA;			  //	Reload
    IWDG->KR = 0xCCCC;			  //	Start
}
int main (void)
{
	 delay_init();
	 encode_io_init();
	 T12_CRL_io_init();
	 ADC_GPIO_Init();
	 ST7567_Init();	
   TIM3_Int_Init(479,99);   //1/10ms
	 TIM16_Init(199,479);
	 //TIM14_PWM_Init(239, 0);  //TIM时钟48M，2399+1=2400分频后=20KHz（PWM频率）,PWM脉冲宽度2400个计数（占空比）为100%,24个计数（占空比）为1% 		 
   beep_number(500,100);
	 shedingwendu=120;
	 IWDG_init(2000);
	while(1)
	{
		  IWDG_ReloadCounter();
	    shurudianya=ADC1_Init(ADC_Channel_2)/3;
//			if(t12wendu>=500)
//			{
//				st7567_Fill(0,0,127,63,0);
//				st7567_ShowString(30,20,(uint8_t *)"ERROR",12,1);
//				t12wendu=get_adc(ADC_Channel_3);
//			}
//			else
		   lcd_show(t12wendu);
	}
}

/***********TIM中断函数*******************/
void TIM3_IRQHandler(void)     
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  // 清TIM3溢出中断标志位
		Control_out();
	}
	
void TIM16_IRQHandler(void)                         //TIM16中断函数
	{
		TIM_ClearITPendingBit(TIM16,TIM_IT_Update); // 清TIM16溢出中断标志位
		 encode();                                                              
	}
