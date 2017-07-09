#include "OLED_ITEM.h"
#include "OLED_Font.h"

#define OLED_CMD  0		//д����
#define OLED_DATA 1		//д����

//	OLED_SPI���캯��
OLED_SPI::OLED_SPI(u8 D0,u8 D1,u8 RES,u8 DC,u8 CS)
{
	this->OLED_SCL=D0;
	this->OLED_SDA=D1; 
	this->OLED_RES=RES;
	this->OLED_DC=DC;
	this->OLED_CS=CS;
}

//��ʼ��SSD1306					    
void OLED_SPI::Init()
{
	pinMode(this->OLED_SDA,OUTPUT);    //SDA D1����Ϊ���ģʽ
    pinMode(this->OLED_SCL,OUTPUT);   //SCL D0����Ϊ���ģʽ
    pinMode(this->OLED_DC,OUTPUT);    //Data/Command����Ϊ���ģʽ
    pinMode(this->OLED_CS,OUTPUT);    //CS����Ϊ���ģʽ
    pinMode(this->OLED_RES,OUTPUT);   //RES����Ϊ���ģʽ

  	OLED_RES_Set(1);
	delay(100);	
	OLED_RES_Set(0);
	delay(100);
	OLED_RES_Set(1); 

	WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	WR_Byte(0x00,OLED_CMD);//---set low column address
	WR_Byte(0x10,OLED_CMD);//---set high column address
	WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	WR_Byte(0x81,OLED_CMD);//--set contrast control register
	WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	WR_Byte(0xA6,OLED_CMD);//--set normal display
	WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	WR_Byte(0x00,OLED_CMD);//-not offset
	WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	WR_Byte(0x12,OLED_CMD);
	WR_Byte(0xDB,OLED_CMD);//--set vcomh
	WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	WR_Byte(0x02,OLED_CMD);//
	WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	Clear();
	Set_Pos(0,0); 	
}

//	�������ܣ���SSD1306д��һ���ֽڡ�
//	dat:Ҫд�������/����
//	cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_SPI::WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set(1);		//д����
	else 
	  OLED_DC_Set(0);		//д����
	OLED_CS_Set(0);			//Ƭѡ�ȵͺ�� �����ش���
	for(i=0;i<8;i++)		//��������
	{			  
		OLED_SCL_Set(0);
		if(dat&0x80)
		{
		   OLED_SDA_Set(1);
		}
		else
		   OLED_SDA_Set(0);
		OLED_SCL_Set(1);
		dat<<=1;   
	}	 		  
	OLED_CS_Set(1);
	OLED_DC_Set(1);   	  	//����ʱ��Ĭ��Ϊд����
}  	 

//	�������ܣ�����OLED��ʾ    
void OLED_SPI::Display_On()
{
	WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	WR_Byte(0X14,OLED_CMD);  //DCDC ON
	WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//	�������ܣ��ر�OLED��ʾ     
void OLED_SPI::Display_Off()
{
	WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}

//	�������ܣ�����
void OLED_SPI::Clear()  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		WR_Byte(0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		WR_Byte(0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		WR_Byte(0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++) WR_Byte(0,OLED_DATA); 
	}
}

//	�������ܣ���������
//	x��������(0~127)
//	y��������(0~7)
void OLED_SPI::Set_Pos(u8 x, u8 y) 
{ 
	WR_Byte(0xb0+y,OLED_CMD);
	WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	WR_Byte((x&0x0f)|0x01,OLED_CMD); 
}

//	���ܽ��ܣ�����checkedStateֵ�����Ƶ�
//	�ڵ�x(0~63)�� ��y(0~7)��
void OLED_SPI::Print_Point(u8 x, u8 y,u8 checkedState)
{
  Set_Pos(x,y);
  if(checkedState == 1)
  {
  	WR_Byte(0xC0,OLED_DATA);
    WR_Byte(0xC0,OLED_DATA);
  }
  else
  {
    WR_Byte(0x00,OLED_DATA);
    WR_Byte(0x00,OLED_DATA);
  }
}
//	���ܽ��ܣ�����checkedStateֵ�����Ƶ�
//	����һ���� ��(x,y)���Ҵ�ӡcount�����ص� count:0~63
void OLED_SPI::Print_ALine(u8 x,u8 y,u8 count,u8 checkedState)
{
	unsigned char i,data;
	if(checkedState) data=0xC0;
	else data=0x00;
	Set_Pos(x,y);
	for(i=0;i<count;i++)
	{
		WR_Byte(data,OLED_DATA);
		WR_Byte(data,OLED_DATA);
	}
}
//	���ܽ��ܣ�����char�����е�16��ʮ��������ʾ����Ӣ��
//	�ڵ�x(0~15)�� ��y(0~3)��
void OLED_SPI::Print_AEN(u8 x, u8 y,u8 chr,u8 checkedState)
{
  u8 i=0;
  u8 c = chr-' ';

  Set_Pos(x*8,y);
  for(i = 0;i < 8;i++)
  {
    if(checkedState == 0)
    	WR_Byte(pgm_read_byte(&F8X16[c*16+i]),OLED_DATA);
    else 
    	WR_Byte(~pgm_read_byte(&F8X16[c*16+i]),OLED_DATA);
  }
  Set_Pos(x*8,y+1);
  for(i = 0;i < 8;i++)
  {
    if(checkedState == 0)
    	WR_Byte(pgm_read_byte(&F8X16[c*16+i+8]),OLED_DATA);
    else 
    	WR_Byte(~pgm_read_byte(&F8X16[c*16+i+8]),OLED_DATA);
  }
}

//	�������ܣ���ʾһ��Ӣ���ַ���
//	x��0~15 y��0~3
//	*chr���ַ���
void OLED_SPI::Print_ENString(u8 x,u8 y,char *chr,u8 checkedState,u8 middleState)
{
	u8 i,j=0,tabNum=0;
	u8 EN_Num=strlen(chr);
	if(middleState == 1)  			 		//����״̬�����ƫ����
    	tabNum = 16/2-(EN_Num+1)/2;   			//������������ �����㷨
	while (chr[j]!='\0')
	{
		if(checkedState)
			Print_AEN(tabNum+x,y,chr[j],1);
		else
			Print_AEN(tabNum+x,y,chr[j],0);
		x+=1;
		j++;
	}

	//ѡ��״̬ʱҪ��û�����ĵĵط��������
  	
  	if(checkedState == 1)   
  	{
    	if(middleState == 0)  //û�о��� ��ֻ���󲿷�
    	{
      		for(i=x;i<16;i++)
      		{
        		Set_Pos(i*8,y);
        		for(j=0;j<8;j++) WR_Byte(0xFF,OLED_DATA);
        		Set_Pos(i*8,y+1);
        		for(j=0;j<8;j++) WR_Byte(0xFF,OLED_DATA);
      		}
    	}else   //����״̬ �����ǰ��������
    	{ 
      		for(i=0;i<tabNum;i++)   //ǰ������� 
      		{
        		Set_Pos(i*8,y);
        		for(j=0;j<8;j++) WR_Byte(0xFF,OLED_DATA);
        		Set_Pos(i*8,y+1);
        		for(j=0;j<8;j++) WR_Byte(0xFF,OLED_DATA);
      		}
      		for(i=tabNum+EN_Num;i<16;i++)   //�󲿷����
      		{
        		Set_Pos(i*8,y);
        		for(j=0;j<8;j++) WR_Byte(0xFF,OLED_DATA);
        		Set_Pos(i*8,y+1);
        		for(j=0;j<8;j++) WR_Byte(0xFF,OLED_DATA);
      		}
    	}
    }
}

//	���ܽ��ܣ�����char�����е�32��ʮ��������ʾ��������
//	�ڵ�x(0~7)�� ��y(0~3)��
/*
void OLED_SPI::Print_ACN(u8 x, u8 y,const u8 *CNChar,u8 checkedState)
{
	u8 i;
	u16 adder=0;
	Set_Pos(x*16,y*2);
	for(i = 0;i < 16;i++)
	{
	    if(checkedState == 0)
	    	WR_Byte(CNChar[adder],OLED_DATA);
	    else 
	      	WR_Byte(~CNChar[adder],OLED_DATA);
		adder += 1;
	}
	Set_Pos(x*16,y*2+1);
	for(i = 0;i < 16;i++)
	{
		if(checkedState == 0)
	    	WR_Byte(CNChar[adder],OLED_DATA);
	    else 
	    	WR_Byte(~CNChar[adder],OLED_DATA);
		adder += 1;
	}
}
*/

//	�������ܣ���ʾ2������
//	x,y :�������	 
//	len :���ֵ�λ��
//	size:�����С
//	num:��ֵ(0~4294967295)
void OLED_SPI::Print_Num(u8 x,u8 y,u32 num,u8 len,u8 checkedState,u8 middleState)
{         	
	u8 t,temp;
	u8 enshow=0,tabNum=0;	
	if(middleState)
	{
		tabNum = 8-len/2;
	}
	for(t=0;t<len;t++)
	{
		temp=(num/pow(10,len-t-1))%10;
		if(enshow==0 && t<(len-1))
		{
			if(temp==0)
			{
				if(checkedState)
					Print_AEN(x+t+tabNum,y,' ',1);
				else 
					Print_AEN(x+t+tabNum,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
		if(checkedState)
	 		Print_AEN(x+t+tabNum,y,temp+'0',1); 
	 	else
	 		Print_AEN(x+t+tabNum,y,temp+'0',0); 
	}
} 	

//	������������ʾBMPͼƬ128��64��ʼ������(x,y)
//	x�ķ�Χ0~127��yΪҳ�ķ�Χ0~7
void OLED_SPI::Draw_BMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[])
{ 	
	u16 j=0;
	u8 x,y;

	for(y=y0;y<y1;y++)
	{
		Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//	���ܽ��ܣ�ɾ����Ļ��0-3��ĳһ��
void OLED_SPI::Dele_Row(u8 row)
{
    if(row >4 ) return; //��������
    u8 x,y;
    WR_Byte(0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
	WR_Byte(0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ 
    for(y=row*2;y<row*2+2;y++)
    {
        WR_Byte(0xb0+y,OLED_CMD);    //����ҳ��ַ��0~7��
        for(x=0;x<64;x++)
        {
            WR_Byte(0,OLED_DATA);
            WR_Byte(0,OLED_DATA);
        }
    }
}

/* ҳ�溯�� */
void OLED_SPI::ENStr_Page(char *words)
{
	Clear();
	Print_ENString(0,3,words,0,1);
}
void OLED_SPI::Comfirm_Page(char *title,char *words)
{
	Clear();
	Print_ENString(0,0,title,0,1);
	Print_ENString(0,3,words,0,1);
	Print_ENString(0,6,"L:YES",0,0);
	Print_ENString(12,6,"R:NO",0,0);
}
void OLED_SPI::Switch_Page(char *title,char *words,unsigned char state,unsigned char clr)
{
	if(clr) Clear();
	unsigned char x_add=3;	//�������۵�x����λ��
	Print_ENString(0,0,title,0,1);
	Print_ENString(x_add,3,words,0,0);
	unsigned char len=strlen(words);
	if(state) Print_ENString(x_add+len+1,3,"ON ",0,0);
	else Print_ENString(x_add+len+1,3,"OFF",0,0);
	Print_ENString(0,6,"L:DO",0,0);
	Print_ENString(11,6,"R:YES",0,0);

}
void OLED_SPI::Adjust_Page(char *title,unsigned char param,unsigned char maxValue,unsigned char numLen)
{
	Clear();
	unsigned char x_add=2,count=0;
	Print_ENString(0,0,title,0,1);
	// 48/maxValue = x/param
	count = 48*param/maxValue;	// 48����ͷȥβ�ո�������ʾ�����ص� maxValue����������ֵ
	if(count==0) count=1;		// ������ʾһ���� ���ڽ����Ѻ�
	Print_ALine(16,3,count,1);

	//����'+' '-'����
	Print_AEN(2,6,'-',0);
	Print_AEN(13,6,'+',0);
	//���Ƶײ�����
	Print_Num(0,6,param,numLen,0,1);
}

//	�������ܣ�����m^n
u16 OLED_SPI::pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--) result*=m;    
	return result;
}

/* ���ŵ�ƽ�仯���� */
void OLED_SPI::OLED_SCL_Set(u8 staus)
{
	if(staus) digitalWrite(this->OLED_SCL,HIGH);
	else digitalWrite(this->OLED_SCL,LOW);
}
void OLED_SPI::OLED_SDA_Set(u8 staus)
{
	if(staus) digitalWrite(this->OLED_SDA,HIGH); 
	else digitalWrite(this->OLED_SDA,LOW); 
}
void OLED_SPI::OLED_RES_Set(u8 staus)
{
	if(staus) digitalWrite(this->OLED_RES,HIGH); 
	else digitalWrite(this->OLED_RES,LOW); 
}
void OLED_SPI::OLED_DC_Set(u8 staus)
{
	if(staus) digitalWrite(this->OLED_DC,HIGH); 
	else digitalWrite(this->OLED_DC,LOW); 
}
void OLED_SPI::OLED_CS_Set(u8 staus)
{
	if(staus) digitalWrite(this->OLED_CS,HIGH);
	else digitalWrite(this->OLED_CS,LOW); 
}

