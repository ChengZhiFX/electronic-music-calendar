#include "oled.h"
#include "oledfont.h"
#include "delay.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			  

/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	OLED_SCLK_Set() ;
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
}

void IIC_Wait_Ack()
{
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(uchar IIC_Byte)
{
	uchar i;
	uchar m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}
}

/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(uchar IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(uchar IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	 IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	 IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	 IIC_Wait_Ack();	
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		Write_IIC_Data(dat);
	}
	else 
	{	
		Write_IIC_Command(dat);
	}
}

/********************************************
// fill_Picture
********************************************/
//void fill_picture(uchar fill_Data)
//{
//	uchar m,n;
//	for(m=0;m<8;m++)
//	{
//		OLED_WR_Byte(0xb0+m,0);		//page0-page1
//		OLED_WR_Byte(0x00+0x02,0);		//low column start address
//		OLED_WR_Byte(0x10,0);		//high column start address
//		for(n=0;n<128;n++)
//			{
//				OLED_WR_Byte(fill_Data,1);
//			}
//	}
//}


//坐标设置
void OLED_Set_Pos(uchar x, uchar y) 
{ 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)+0x02,OLED_CMD); 
} 

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
//void OLED_Display_Off(void)
//{
//	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
//	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
//	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
//}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uchar i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00+0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}

//void OLED_On(void)  
//{  
//	uchar i,n;		    
//	for(i=0;i<8;i++)  
//	{  
//		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
//		OLED_WR_Byte (0x00+0x02,OLED_CMD);      //设置显示位置—列低地址
//		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
//		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
//	} //更新显示
//}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar Char_Size){          
        uchar c=0,i=0;    
//        x+=2;
        c=chr-' ';//???????            
        if(x>Max_Column-1){x=0;y=y+2;}
        if(Char_Size ==16){
            OLED_Set_Pos(x,y);    
            for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
        }
        else if(Char_Size == 12){    
            OLED_Set_Pos(x,y);
            for(i=0;i<6;i++)
            OLED_WR_Byte(F6x8[c][i],OLED_DATA);
        }
        else if(Char_Size == 24){    
            OLED_Set_Pos(x,y);    
            for(i=0;i<12;i++)
                OLED_WR_Byte(F12X24[c*36+i],OLED_DATA);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<12;i++)
                OLED_WR_Byte(F12X24[c*36+i+12],OLED_DATA);
            OLED_Set_Pos(x,y+2);
            for(i=0;i<12;i++)
                OLED_WR_Byte(F12X24[c*36+i+24],OLED_DATA);
				}
}
void OLED_ShowChar_Reverse(uchar x,uchar y,uchar chr,uchar Char_Size){          
        uchar c=0,i=0;    
//        x+=2;
        c=chr-' ';//???????            
        if(x>Max_Column-1){x=0;y=y+2;}
        if(Char_Size ==16)
            {
            OLED_Set_Pos(x,y);    
            for(i=0;i<8;i++)
            OLED_WR_Byte(~F8X16[c*16+i],OLED_DATA);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<8;i++)
            OLED_WR_Byte(~F8X16[c*16+i+8],OLED_DATA);
            }
            else if(Char_Size == 12){    
                OLED_Set_Pos(x,y);
                for(i=0;i<6;i++)
                OLED_WR_Byte(~F6x8[c][i],OLED_DATA);
                
            }
        else if(Char_Size == 24){    
            OLED_Set_Pos(x,y);    
            for(i=0;i<12;i++)
                OLED_WR_Byte(~F12X24[c*36+i],OLED_DATA);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<12;i++)
                OLED_WR_Byte(~F12X24[c*36+i+12],OLED_DATA);
            OLED_Set_Pos(x,y+2);
            for(i=0;i<12;i++)
                OLED_WR_Byte(~F12X24[c*36+i+24],OLED_DATA);
				}
}
//m^n函数
//uint oled_pow(uchar m,uchar n)
//{
//	uint result=1;	 
//	while(n--)result*=m;    
//	return result;
//}			

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
//void OLED_ShowNum(uchar x,uchar y,uint num,uchar len,uchar size2)
//{
//	uchar t,temp;
//	uchar enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/oled_pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
//				continue;
//			}else enshow=1;
//		}
//	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
//	}
//}

//显示一个字符号串
void OLED_ShowString(uchar x,uchar y,uchar *chr,uchar Char_Size){
	uchar j=0;
//    x+=2;
	while (chr[j]!='\0'){
		OLED_ShowChar(x,y,chr[j],Char_Size);
		x+=Char_Size/2;
//		if(x>120){x=0;y+=2;}
		j++;
	}
}

void OLED_ShowString_Reverse(uchar x,uchar y,uchar *chr,uchar Char_Size){
	uchar j=0;
//    x+=2;
	while (chr[j]!='\0'){
		OLED_ShowChar_Reverse(x,y,chr[j],Char_Size);
		x+=Char_Size/2;
//		if(x>120){x=0;y+=2;}
		j++;
	}
}

//显示汉字
void OLED_ShowChinese(uchar x, uchar y, uchar sel_lib, uchar num){
	uchar t, adder=0;
	uchar (*lib)[32];
	switch(sel_lib){
		case 0: {lib = Hzk; break;}
		case 1: {lib = Lunar; break;}
		case 2: {lib = SolarTermLib; break;}
		case 3: {lib = WeatherLib; break;}
		default: {OLED_ShowChar_Reverse(x,y,'?',16); return;}
	}
	OLED_Set_Pos(x,y);
	for(t=0;t<16;t++){
		OLED_WR_Byte(lib[2*num][t],OLED_DATA);
		adder+=1;
	}
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++){	
		OLED_WR_Byte(lib[2*num+1][t],OLED_DATA);
		adder+=1;
	}
}

void OLED_ShowChinese_Reverse(uchar x, uchar y, uchar sel_lib, uchar num){
	uchar t, adder=0;
	uchar (*lib)[32];
	switch(sel_lib){
		case 0: {lib = Hzk; break;}
		case 1: {lib = Lunar; break;}
		case 2: {lib = SolarTermLib; break;}
		case 3: {lib = WeatherLib; break;}
		default: {OLED_ShowChar_Reverse(x,y,'?',16); return;}
	}
	OLED_Set_Pos(x,y);
	for(t=0;t<16;t++){
		OLED_WR_Byte(~lib[2*num][t],OLED_DATA);
		adder+=1;
	}
	OLED_Set_Pos(x,y+1);	
	for(t=0;t<16;t++){	
		OLED_WR_Byte(~lib[2*num+1][t],OLED_DATA);
		adder+=1;
	}
}

void OLED_ShowChineseString(uchar x,uchar y,uchar sel_lib,uchar chr[],uchar length)
{
    uchar j=0;
//    x+=2;
    for(j=0;j<length;j++){
			OLED_ShowChinese(x, y, sel_lib, chr[j]);
      x+=16;
      if(x>112){x=0;y+=2;}
    }
}

void OLED_ShowChineseString_Reverse(uchar x,uchar y,uchar sel_lib,uchar chr[],uchar length)
{
    uchar j=0;
//    x+=2;
    for(j=0;j<length;j++){
			OLED_ShowChinese_Reverse(x, y, sel_lib, chr[j]);
      x+=16;
      if(x>112){x=0;y+=2;}
    }
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0~128，y为页的范围0~8*****************/
void OLED_DrawBMP(uchar x0, uchar y0,uchar x1, uchar y1,uchar BMP[])
{ 	
	unsigned int j=0;
	uchar x,y;
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 

//初始化SSD1306					    
void OLED_Init(void)
{ 	
 
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
	OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  
