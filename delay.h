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

void delay_ms(uint ms);
void delay_us(uint us);
void delay_for_mq2();
char Char(char tinyint);
char Tinyint(char ch);
void quadruple_digit_to_string(uint num, char str[5]);
void double_digit_to_string(char num, char str[3]);
uint char_to_quadruple_digit(char ch4, char ch3, char ch2, char ch1);
char char_to_double_digit(char ch2, char ch1);

#endif