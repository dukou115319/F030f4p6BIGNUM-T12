#include "st7567.h"
#include "ascii.h"

uint16_t OLED_GRAM[128][8];
///****************************************************************************/
//��������:ST7567_GPIO_Init(void)
//��������:LCD�ӿڳ�ʼ������
//�������:��
//�� �� ֵ:��
///****************************************************************************/
void ST7567_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOF, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
}
///****************************************************************************/
//��������:ST7567_WriteCommand(uint8_t  COM)
//��������:д�����
//�������:uint8_t  COM
//�� �� ֵ:��
///****************************************************************************/
void ST7567_WriteCommand(uint8_t  COM)
{
	uint8_t bit8;
	CD_L;CS_L;
	for(bit8=0;bit8<8;bit8++)
	{
		SCL_L;
		if((COM&0x80)==0x80)
		SDA_H;
		else		
		SDA_L;
		SCL_H;
		COM=(COM<<1);
	}		
}
///****************************************************************************/
//��������:ST7567_WriteData(uint8_t date)
//��������:д���ݺ���
//�������:uint8_t date
//�� �� ֵ:��
///****************************************************************************/
void ST7567_WriteData(uint8_t date)
{   
	uint8_t bit8;
	CD_H;		CS_L;		
	for(bit8=0;bit8<8;bit8++)
	{
		SCL_L;
		if((date&0x80)==0x80)
		SDA_H;
		else
		SDA_L;
		SCL_H;
		date=(date<<1);
	}
}
///****************************************************************************/
//��������:HDReset(void)
//��������:Ӳ����λ����
//�������:��
//�� �� ֵ:��
///****************************************************************************/
void HDReset(void)
{
	RST_L;
	delay_ms(2);
	RST_H;
	delay_ms(4);
}
///****************************************************************************/
//��������:Lcd12864_ClearScreen(void)
//��������:��������
//�������:��
//�� �� ֵ:��
///****************************************************************************/
void st7567_ClearScreen(void)
{
		uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		ST7567_WriteCommand(0xb0+i);    //����ҳ��ַ��0~7��
		ST7567_WriteCommand(0x00);      //������ʾλ�á��е͵�ַ
		ST7567_WriteCommand(0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
		ST7567_WriteData(0); 
	} //������ʾ
}
///****************************************************************************/
//��������:Init_ST7567()
//��������:��ʼ��Һ����
//�������:��
//�� �� ֵ:��
//TIPS:�������ST7567�����ֲ�
///****************************************************************************/

void ST7567_Init()
{
		ST7567_GPIO_Init();
		SCL_H;
		SDA_H;
		HDReset();//Ӳ��λ
		delay_ms(400);
    ST7567_WriteCommand(0xe2);//��λ
    ST7567_WriteCommand(0xaf);//0xaf��ʾ����
    ST7567_WriteCommand(0x2f);//0x2f��ѹ��·,��ѹ�����·, 	   
    ST7567_WriteCommand(0x25);//0x20-0x27ΪV5��ѹ�ڲ������������ 
    ST7567_WriteCommand(0x81);// SET EV ���Աȶ�81
    ST7567_WriteCommand(0x18);//0x01-0x3f�����Ĵ�������ģʽ
    ST7567_WriteCommand(0xa0);//0xa0ΪSegment����,0xa1 ΪSegment����	   
    ST7567_WriteCommand(0xc0);//0xc0����ɨ��,0xc8����ɨ��
    ST7567_WriteCommand(0xa6);//0xa6������ʾ,0xa7������ʾ
    ST7567_WriteCommand(0xa4);//0xa4������ʾ,0xa5ȫ������
    ST7567_WriteCommand(0xF8);//��ѹ������F8
    ST7567_WriteCommand(0x00);//00--10
		st7567_ClearScreen();   //����
}
///****************************************************************************/
//��������: OLED_Refresh_Gram(void)
//��������: �����Դ浽OLED
//�������: ��
//�� �� ֵ:��
///****************************************************************************/	
void st7567_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		ST7567_WriteCommand (0xb0+i);    //����ҳ��ַ��0~7��
		ST7567_WriteCommand (0x00);      //������ʾλ�á��е͵�ַ
		ST7567_WriteCommand (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)
		ST7567_WriteData(OLED_GRAM[n][i]); 
	}   
}

///****************************************************************************/
//��������: ST7567_Set_Pos(uint8_t x,uint8_t y,uint8_t t)
//��������: ���㺯��
//�������: x:0~127;y:0~63	
//�� �� ֵ:��
///****************************************************************************/	
void ST7567_draw_point(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;

}
///****************************************************************************/
//��������: st7567_DrawPlusSign(uint8_t x,uint8_t y)
//��������: ���Ӻ�
//�������: x:1~126�����������x; 1~62�����������y
//�� �� ֵ:��
///****************************************************************************/
void st7567_DrawPlusSign(uint8_t x,uint8_t y)
{
	ST7567_draw_point(x,y-1,1);
	ST7567_draw_point(x,y,1);
	ST7567_draw_point(x,y+1,1);
	ST7567_draw_point(x-1,y,1);
	ST7567_draw_point(x+1,y,1);
}
///****************************************************************************/
//��������: OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
//��������: �������ĶԽ�����
//�������: ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63;dot:0,���;1,���
//�� �� ֵ:��
///****************************************************************************/ 
void st7567_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)ST7567_draw_point(x,y,dot);
	}													    
	//st7567_Refresh_Gram();//������ʾ
}

///****************************************************************************/
//��������: st7567_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
//��������: ��ָ��λ����ʾһ���ַ�,���������ַ�
//�������: x:0~127;y:0~63;mode:0,������ʾ;1,������ʾ;size:ѡ������ 12/16/24
//�� �� ֵ:��
///****************************************************************************/
void st7567_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		//else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)ST7567_draw_point(x,y,mode);
			else ST7567_draw_point(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }  st7567_Refresh_Gram();         
}
//m^n����
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}
///****************************************************************************/
//��������: st7567_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
//��������: ��ʾ2������
//�������: x,y :�������;len :���ֵ�λ��;size:�����С12/16/24;mode:ģʽ	0,���ģʽ;1,����ģʽ;num:��ֵ(0~4294967295);
//�� �� ֵ:��
///****************************************************************************/ 
void st7567_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				st7567_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	st7567_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	} 
} 
///****************************************************************************/
//��������:  st7567_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t size,uint8_t mode)
//��������: ��ʾһ���ַ���
//�������: x,y:�������;size:�����С12/16/24;*p:�ַ�����ʼ��ַ;mode:0,������ʾ;1,������ʾ
//�� �� ֵ:��
///****************************************************************************/ 	   		   
void st7567_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t size,uint8_t mode)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;st7567_ClearScreen();}
        st7567_ShowChar(x,y,*p,size,mode);	 
        x+=size/2;
        p++;
    }  
}
/****************************************************************************/
//��������:  st7567_ShowChines(uint16_t x,uint16_t y,uint8_t  index,uint8_t size,uint8_t mode)
//��������: ��ָ��λ�ã���ʾһ��16*16��С�ĺ���
//�������: x,y:�������;Index:���ֱ��;msize:ѡ������ 12/16/24;mode:0,������ʾ;1,������ʾ
//�� �� ֵ:��
/****************************************************************************/ 
void st7567_ShowChinese(uint16_t x,uint16_t y,uint8_t index,uint8_t size,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t *dzk;   
	uint8_t csize=(size/8+((size%8)?1:0))*size; //һ��24*24�ĺ���72�ֽ� 
	if(size == 12) dzk=(uint8_t*)Chinese_1212[index];        //�õ����ֱ�Ŷ�Ӧ�ĵ���� 
	else if(size == 16)	dzk=(uint8_t*)Chinese_1616[index];   //�õ����ֱ�Ŷ�Ӧ�ĵ���� 
	else dzk=(uint8_t*)Chinese_2424[index];   //�õ����ֱ�Ŷ�Ӧ�ĵ���� 
	for(t=0;t<csize;t++)
	{                                                                                                      
		temp=dzk[t];                                //�õ���������                          
		for(t1=0;t1<8;t1++)                        //���մӸ�λ����λ��˳�򻭵�
		{
			if(temp&0x80)ST7567_draw_point(x,y,mode);
			else ST7567_draw_point(x,y,!mode); 
			temp<<=1;
			y++;
			if((y-y0)==size)                        //��y����Ĵ�����y�������ʼ�����24�����ص㣬x�����1
			{
				y=y0;
				x++;
				break;
			}
		}           
	}	  
}
//��ָ��λ�ã��Ի���ķ�ʽ��ʾ44x22���������
//x,y:��ʼ������x�ķ�Χ0~84��yΪҳ�ķ�Χ0~20
//index��������num_4422�е�����
//mode:0,������ʾ;1,������ʾ
void OLED_DrawPointNum(uint8_t x,uint8_t y,uint8_t index,uint8_t mode)
{
	uint16_t temp,t,t1;
    uint8_t y0=y;
	for(t=0;t<176;t++)
	{   
		temp=num_4422[index][t];                            
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)ST7567_draw_point(x,y,mode);
			else ST7567_draw_point(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==50)
			{
				y=y0;
				x++;
				break;
			}
		}    
	}
}
