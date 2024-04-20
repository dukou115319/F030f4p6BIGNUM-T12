#include "st7567.h"
#include "ascii.h"

uint16_t OLED_GRAM[128][8];
///****************************************************************************/
//函数名称:ST7567_GPIO_Init(void)
//函数功能:LCD接口初始化函数
//输入参数:无
//返 回 值:无
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
//函数名称:ST7567_WriteCommand(uint8_t  COM)
//函数功能:写命令函数
//输入参数:uint8_t  COM
//返 回 值:无
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
//函数名称:ST7567_WriteData(uint8_t date)
//函数功能:写数据函数
//输入参数:uint8_t date
//返 回 值:无
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
//函数名称:HDReset(void)
//函数功能:硬件复位函数
//输入参数:无
//返 回 值:无
///****************************************************************************/
void HDReset(void)
{
	RST_L;
	delay_ms(2);
	RST_H;
	delay_ms(4);
}
///****************************************************************************/
//函数名称:Lcd12864_ClearScreen(void)
//函数功能:清屏函数
//输入参数:无
//返 回 值:无
///****************************************************************************/
void st7567_ClearScreen(void)
{
		uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		ST7567_WriteCommand(0xb0+i);    //设置页地址（0~7）
		ST7567_WriteCommand(0x00);      //设置显示位置—列低地址
		ST7567_WriteCommand(0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
		ST7567_WriteData(0); 
	} //更新显示
}
///****************************************************************************/
//函数名称:Init_ST7567()
//函数功能:初始化液晶屏
//输入参数:无
//返 回 值:无
//TIPS:命令符见ST7567数据手册
///****************************************************************************/

void ST7567_Init()
{
		ST7567_GPIO_Init();
		SCL_H;
		SDA_H;
		HDReset();//硬复位
		delay_ms(400);
    ST7567_WriteCommand(0xe2);//软复位
    ST7567_WriteCommand(0xaf);//0xaf显示器开
    ST7567_WriteCommand(0x2f);//0x2f升压电路,电压管理电路, 	   
    ST7567_WriteCommand(0x25);//0x20-0x27为V5电压内部电阻调整设置 
    ST7567_WriteCommand(0x81);// SET EV 调对比度81
    ST7567_WriteCommand(0x18);//0x01-0x3f电量寄存器设置模式
    ST7567_WriteCommand(0xa0);//0xa0为Segment正向,0xa1 为Segment反向	   
    ST7567_WriteCommand(0xc0);//0xc0正向扫描,0xc8反射扫描
    ST7567_WriteCommand(0xa6);//0xa6正向显示,0xa7反向显示
    ST7567_WriteCommand(0xa4);//0xa4正常显示,0xa5全屏点亮
    ST7567_WriteCommand(0xF8);//背压比设置F8
    ST7567_WriteCommand(0x00);//00--10
		st7567_ClearScreen();   //清屏
}
///****************************************************************************/
//函数名称: OLED_Refresh_Gram(void)
//函数功能: 更新显存到OLED
//输入参数: 无
//返 回 值:无
///****************************************************************************/	
void st7567_Refresh_Gram(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		ST7567_WriteCommand (0xb0+i);    //设置页地址（0~7）
		ST7567_WriteCommand (0x00);      //设置显示位置—列低地址
		ST7567_WriteCommand (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
		ST7567_WriteData(OLED_GRAM[n][i]); 
	}   
}

///****************************************************************************/
//函数名称: ST7567_Set_Pos(uint8_t x,uint8_t y,uint8_t t)
//函数功能: 画点函数
//输入参数: x:0~127;y:0~63	
//返 回 值:无
///****************************************************************************/	
void ST7567_draw_point(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;

}
///****************************************************************************/
//函数名称: st7567_DrawPlusSign(uint8_t x,uint8_t y)
//函数功能: 画加号
//输入参数: x:1~126，中心坐标的x; 1~62，中心坐标的y
//返 回 值:无
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
//函数名称: OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
//函数功能: 填充区域的对角坐标
//输入参数: 确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63;dot:0,清空;1,填充
//返 回 值:无
///****************************************************************************/ 
void st7567_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)ST7567_draw_point(x,y,dot);
	}													    
	//st7567_Refresh_Gram();//更新显示
}

///****************************************************************************/
//函数名称: st7567_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
//函数功能: 在指定位置显示一个字符,包括部分字符
//输入参数: x:0~127;y:0~63;mode:0,反白显示;1,正常显示;size:选择字体 12/16/24
//返 回 值:无
///****************************************************************************/
void st7567_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		//else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
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
//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}
///****************************************************************************/
//函数名称: st7567_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
//函数功能: 显示2个数字
//输入参数: x,y :起点坐标;len :数字的位数;size:字体大小12/16/24;mode:模式	0,填充模式;1,叠加模式;num:数值(0~4294967295);
//返 回 值:无
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
//函数名称:  st7567_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t size,uint8_t mode)
//函数功能: 显示一个字符串
//输入参数: x,y:起点坐标;size:字体大小12/16/24;*p:字符串起始地址;mode:0,反白显示;1,正常显示
//返 回 值:无
///****************************************************************************/ 	   		   
void st7567_ShowString(uint8_t x,uint8_t y, uint8_t *p,uint8_t size,uint8_t mode)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;st7567_ClearScreen();}
        st7567_ShowChar(x,y,*p,size,mode);	 
        x+=size/2;
        p++;
    }  
}
/****************************************************************************/
//函数名称:  st7567_ShowChines(uint16_t x,uint16_t y,uint8_t  index,uint8_t size,uint8_t mode)
//函数功能: 在指定位置，显示一个16*16大小的汉字
//输入参数: x,y:起点坐标;Index:汉字编号;msize:选择字体 12/16/24;mode:0,反白显示;1,正常显示
//返 回 值:无
/****************************************************************************/ 
void st7567_ShowChinese(uint16_t x,uint16_t y,uint8_t index,uint8_t size,uint8_t mode)
{
	uint8_t temp,t,t1;
	uint16_t y0=y;
	uint8_t *dzk;   
	uint8_t csize=(size/8+((size%8)?1:0))*size; //一个24*24的汉字72字节 
	if(size == 12) dzk=(uint8_t*)Chinese_1212[index];        //得到汉字编号对应的点阵库 
	else if(size == 16)	dzk=(uint8_t*)Chinese_1616[index];   //得到汉字编号对应的点阵库 
	else dzk=(uint8_t*)Chinese_2424[index];   //得到汉字编号对应的点阵库 
	for(t=0;t<csize;t++)
	{                                                                                                      
		temp=dzk[t];                                //得到点阵数据                          
		for(t1=0;t1<8;t1++)                        //按照从高位到低位的顺序画点
		{
			if(temp&0x80)ST7567_draw_point(x,y,mode);
			else ST7567_draw_point(x,y,!mode); 
			temp<<=1;
			y++;
			if((y-y0)==size)                        //对y坐标的处理，当y坐标距起始坐标差24个像素点，x坐标加1
			{
				y=y0;
				x++;
				break;
			}
		}           
	}	  
}
//在指定位置，以画点的方式显示44x22点阵的数字
//x,y:起始点坐标x的范围0~84，y为页的范围0~20
//index：数字在num_4422中的索引
//mode:0,反白显示;1,正常显示
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
