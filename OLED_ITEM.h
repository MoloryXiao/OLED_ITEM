#ifndef __OLED_ITEM_H
#define __OLED_ITEM_H	  

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define  u8 unsigned char 	// 0~255
#define  u16 unsigned int 	// 0~65535
#define  u32 unsigned long	// 0~4294967295

class OLED_SPI
{
public:
	/* 构造函数 初始化引脚 */
	OLED_SPI(u8 D0,u8 D1,u8 RES,u8 DC,u8 CS);
	
	/* 基础成员函数 */			   		    
	void Init();					//引脚模式初始化 屏幕参数初始化
	void WR_Byte(u8 dat,u8 cmd);	//根据cmd值发送数据或命令  
	void Display_On();				//开启OLED显示  
	void Display_Off();				//关闭OLED显示  
	void Clear();					//清屏
	void Set_Pos(u8 x,u8 y);		//定位 x:0~127 y:0~7

	/* 各种绘字函数 */
	//绘制一个点 x:0~63 y:0~7
	void Print_Point(u8 x, u8 y,u8 checkedState);
	//绘制一条线 从(x,y)向右打印count次像素点 count:0~63
	void Print_ALine(u8 x,u8 y,u8 count,u8 checkedState);
	//打印一个英文'A' x:0~15 y:0~7
	void Print_AEN(u8 x, u8 y,u8 chr,u8 checkedState);	
	//打印一串英文"ABCD" x:0~15 y:0~7 char *title,char *words
	void Print_ENString(u8 x,u8 y,char *p,u8 checkedState,u8 middleState);	
	//打印一串数字 x:0~15 y:0~3 len>=num.length
	void Print_Num(u8 x,u8 y,u32 num,u8 len,u8 checkedState,u8 middleState);	
	//绘制一张图
	void Draw_BMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);			

	/* 其他函数 */
	u16  pow(u8 m,u8 n);		//计算m^n
	void Dele_Row(u8 row);		//删除一整行 row(0~3)
	//void fun();

	/* 页面函数 */
	void ENStr_Page(char *words);
	void Comfirm_Page(char *title,char *words);
	void Switch_Page(char *title,char *words,unsigned char state,unsigned char clr);
	void Adjust_Page(char *title,unsigned char param,unsigned char maxValue,unsigned char cls);

	/* 引脚电平变化函数 */
	void OLED_CS_Set(u8 staus);	
	void OLED_RES_Set(u8 staus);
	void OLED_DC_Set(u8 staus);
	void OLED_SCL_Set(u8 staus);
	void OLED_SDA_Set(u8 staus);

private:
	u8 OLED_SCL;	//D0
	u8 OLED_SDA; 	//D1
	u8 OLED_RES;	//RES
	u8 OLED_DC;		//DC
	u8 OLED_CS;		//CS
};
#endif  
	 



