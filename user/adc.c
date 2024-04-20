#include "adc.h"


float adc_data; //�ɼ�ֵ��������ѹ��ʾֵ����
#define FILTER4_N 4
uint16_t filter_buf[FILTER4_N + 1];
//******************************************************************************************************************* 
//��������: ADC_GPIO_Init(void)
//��������: ����ADC��������
//�������: ��
//�� �� ֵ:��
//***************************  dukou **************************************************************************** 
void ADC_GPIO_Init(void)                                          //ADC-GPIO������������,�ڴ˿�������9·�ⲿ����ͨ��
{
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);        //ʹ��GPIOA��GPIOBʱ��
		GPIO_InitTypeDef GPIO_InitStructure;                      //�����ṹ����
			
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;   	  //���壬PA2��Ϊ��ѹAD����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;          	  //IO��Ϊģ������ģʽ
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       	  //AD������Ϊ��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);                 	  //�����ϲ�������A�� 
}
/**********************************************************************************/
//�������ܣ�����ADC�������ɼ�
//��    �룺ADC_TONGDAO
//��    ����adc_data
/**********************************************************************************/
	uint16_t ADC1_Init(uint16_t ADC_tondao)                             //����ADC�������ɼ�
	{
		ADC_GPIO_Init();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);               //ʹ��AHBԤ��Ƶ��������ADC1�Ŀ���
		RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4) ;                           //ʱ�ӷ�Ƶ48M/4=12M ���ʱ�Ӳ�����14M

		ADC_DeInit(ADC1);                                                  //ADC��λ
		ADC_DMACmd(ADC1, DISABLE);                                         //��ֹDMA
		ADC_InitTypeDef ADC_InitStuctrue;                                  //����ADC�ṹ����
		ADC_StructInit(&ADC_InitStuctrue);                                 //����ADC_InitStuctrue��ָ��������ʼ��ADC1�ļĴ���
		ADC_InitStuctrue.ADC_Resolution=ADC_Resolution_10b;                //�ɼ���Ϊ10λ���ȼ�4095
		ADC_InitStuctrue.ADC_ContinuousConvMode=DISABLE;                   //��ֹ����ADC����Ϊ����ADC�ɼ�
		ADC_InitStuctrue.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None; 
																																			 //�����ⲿ�������������ת��
		ADC_InitStuctrue.ADC_DataAlign=ADC_DataAlign_Right;                //�����Ҷ���
		ADC_InitStuctrue.ADC_ScanDirection=ADC_ScanDirection_Backward;     //���ݸ���/�������
		ADC_Init(ADC1,&ADC_InitStuctrue);                                  //�����ϲ�������ADC1
		ADC_ChannelConfig(ADC1,ADC_tondao,ADC_SampleTime_7_5Cycles);       //����ADC1ͨ��1��PA1��
																																			 //��������7.5������
		ADC_GetCalibrationFactor(ADC1);                                    //У׼ADC1
		ADC_Cmd(ADC1,ENABLE);                                              //ʹ��ADC1
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_ADEN)==RESET);               //�ȴ�ADC׼��
		ADC_StartOfConversion(ADC1);                                       //�������ADCת��
		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);                //�ȴ�ADC���

		adc_data=ADC_GetConversionValue(ADC1);  //�������ݼ�ΪADC����
		return adc_data;
}
	
uint16_t get_adc(uint16_t ch) 
{
  int i;
  int filter_sum = 0;
  filter_buf[FILTER4_N] = ADC1_Init(ch);		
  for(i = 0; i < FILTER4_N; i++) 
	{
    filter_buf[i] = filter_buf[i + 1]; // �����������ƣ���λ�Ե�
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
