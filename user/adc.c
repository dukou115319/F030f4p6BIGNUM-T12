#include "adc.h"


float adc_data; //采集值变量，电压显示值变量
#define FILTER4_N 4
uint16_t filter_buf[FILTER4_N + 1];
//******************************************************************************************************************* 
//函数名称: ADC_GPIO_Init(void)
//函数功能: 配置ADC输入引叫
//输入参数: 无
//返 回 值:无
//***************************  dukou **************************************************************************** 
void ADC_GPIO_Init(void)                                          //ADC-GPIO输入引脚配置,在此可以设置9路外部输入通道
{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);        //使能GPIOA和GPIOB时钟
		GPIO_InitTypeDef GPIO_InitStructure;                      //声明结构变量
			
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;   	  //定义，PA2脚为电压AD输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;          	  //IO口为模拟输入模式
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       	  //AD口设置为浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);                 	  //按以上参数设置A口 
}
/**********************************************************************************/
//函数功能：配置ADC参数及采集
//输    入：ADC_TONGDAO
//输    出：adc_data
/**********************************************************************************/
	uint16_t ADC1_Init(uint16_t ADC_tondao)                             //配置ADC参数及采集
	{
		ADC_GPIO_Init();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);               //使能AHB预分频器到外设ADC1的开关
		RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4) ;                           //时钟分频48M/4=12M 最大时钟不超过14M

		ADC_DeInit(ADC1);                                                  //ADC复位
		ADC_DMACmd(ADC1, DISABLE);                                         //禁止DMA
		ADC_InitTypeDef ADC_InitStuctrue;                                  //声明ADC结构变量
		ADC_StructInit(&ADC_InitStuctrue);                                 //根据ADC_InitStuctrue中指定参数初始化ADC1的寄存器
		ADC_InitStuctrue.ADC_Resolution=ADC_Resolution_10b;                //采集设为10位精度即4095
		ADC_InitStuctrue.ADC_ContinuousConvMode=DISABLE;                   //禁止持续ADC，设为单次ADC采集
		ADC_InitStuctrue.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None; 
																																			 //不用外部触发，软件触发转换
		ADC_InitStuctrue.ADC_DataAlign=ADC_DataAlign_Right;                //数据右对齐
		ADC_InitStuctrue.ADC_ScanDirection=ADC_ScanDirection_Backward;     //数据覆盖/浏览方向
		ADC_Init(ADC1,&ADC_InitStuctrue);                                  //按以上参数设置ADC1
		ADC_ChannelConfig(ADC1,ADC_tondao,ADC_SampleTime_7_5Cycles);       //配置ADC1通道1即PA1，
																																			 //采样周期7.5个周期
		ADC_GetCalibrationFactor(ADC1);                                    //校准ADC1
		ADC_Cmd(ADC1,ENABLE);                                              //使能ADC1
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADEN)==RESET);               //等待ADC准备
		ADC_StartOfConversion(ADC1);                                       //软件启动ADC转换
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);                //等待ADC完成

		adc_data=ADC_GetConversionValue(ADC1);  //所得数据即为ADC数据
		return adc_data;
}
	
uint16_t get_adc(uint16_t ch) 
{
  int i;
  int filter_sum = 0;
  filter_buf[FILTER4_N] = ADC1_Init(ch);		
  for(i = 0; i < FILTER4_N; i++) 
	{
    filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，低位仍掉
    filter_sum += filter_buf[i];
  }
//	printf("%d\n",filter_sum / FILTER4_N);
  return (int)(filter_sum / FILTER4_N);
}
//uint16_t Get_Average(uint16_t ch,uint8_t times)
//{
//	uint32_t temp_val=0;
//	uint8_t t;
//	for(t=0;t<times;t++)
//	{ 
//		temp_val+=ADC1_Init(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//}
