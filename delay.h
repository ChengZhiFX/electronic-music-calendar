#ifndef __DELAY_H_
#define __DELAY_H_

//---包含头文件---//
#include<reg52.h>
#include<intrins.h>


#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

void Delay_50ms(uint Del_50ms);
void delay_ms(uint ms);
void delay_us(uint us);
char Char(char tinyint);

#endif