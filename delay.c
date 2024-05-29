#include "delay.h"

void Delay_50ms(uint Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

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

char Char(char tinyint){
	return (char)(tinyint + '0');
}