#include "reg52.h"
#ifndef __OLED_H__
#define __OLED_H__			  	 

#define  uchar unsigned char
#define  uint unsigned int 

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define REV_DATA 2
#define OLED_MODE 0

//I2C管脚定义
sbit OLED_SCL=P1^7; //D0（SCLK） 时钟
sbit OLED_SDIN=P1^6;//D1（MOSI） 数据


#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1



//OLED模式设置
//0:4线串行模式
//1:并行8080模式

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uchar x,uchar y,uchar t);
void OLED_Fill(uchar x1,uchar y1,uchar x2,uchar y2,uchar dot);
void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar Char_Size);
void OLED_ShowChar_Reverse(uchar x,uchar y,uchar chr,uchar Char_Size);
void OLED_ShowNum(uchar x,uchar y,uint num,uchar len,uchar size);
void OLED_ShowString(uchar x,uchar y, uchar *p,uchar Char_Size);
void OLED_ShowString_Reverse(uchar x,uchar y, uchar *p,uchar Char_Size);
void OLED_Set_Pos(uchar x, uchar y);
void OLED_ShowChinese(uchar x, uchar y, uchar sel_lib, uchar num);
void OLED_ShowChineseString(uchar x,uchar y,uchar sel_lib,uchar chr[],uchar length);
void OLED_DrawBMP(uchar x0, uchar y0,uchar x1, uchar y1,uchar BMP[]);
void fill_picture(uchar fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(uchar IIC_Command);
void Write_IIC_Data(uchar IIC_Data);
void Write_IIC_Byte(uchar IIC_Byte);
void IIC_Wait_Ack();

#endif
