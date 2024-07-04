#include "delay.h"

void delay_ms(uint ms)
{
	uchar j;
	while(ms--)
	{	
		for(j=0;j<122;j++);
	}
}

void delay_us(uint us)//ÑÓÊ±º¯Êý
{
	while(us--);
}

void delay_for_mq2(){
	uchar m,n,s;
	for(m=20;m>0;m--)
		for(n=20;n>0;n--)
			for(s=248;s>0;s--);
}

char Char(char tinyint){
	return (char)(tinyint + '0');
}

char Tinyint(char ch){
	return (ch - '0');
}

void quadruple_digit_to_string(uint num, char str[5]){
	if(num < 10){
		str[0] = '0';
		str[1] = '0';
		str[2] = '0';
		str[3] = Char(num);
	}
	else if(num < 100){
		str[0] = '0';
		str[1] = '0';
		str[2] = Char(num / 10);
		str[3] = Char(num % 10);
	}
	else if(num < 1000){
		str[0] = '0';
		str[1] = Char(num / 100);
		str[2] = Char((num / 10) % 10);
		str[3] = Char(num % 10);
	}
	else if(num < 10000){
		str[0] = Char(num / 1000);
		str[1] = Char((num / 100) % 10);
		str[2] = Char((num / 10) % 10);
		str[3] = Char(num % 10);
	}
	str[4] = 0;
}

void double_digit_to_string(uchar num, char str[3]){
	if(num < 10){
		str[0] = '0';
		str[1] = Char(num);
	}
	else if(num < 100){
		str[0] = Char(num / 10);
		str[1] = Char(num % 10);
	}
	str[2] = 0;
}

uint char_to_quadruple_digit(char ch4, char ch3, char ch2, char ch1){
	return Tinyint(ch4)*1000 + Tinyint(ch3)*100 + Tinyint(ch2)*10 + Tinyint(ch1);
}

char char_to_double_digit(char ch2, char ch1){
	return Tinyint(ch2)*10 + Tinyint(ch1);
}