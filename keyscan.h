#ifndef __KEYSCAN_H_
#define __KEYSCAN_H_

#include<reg52.h>
#include<intrins.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit key1=P3^0;
sbit key2=P3^1;
sbit key3=P3^2;
sbit key4=P3^3;

uchar getKey();

#endif