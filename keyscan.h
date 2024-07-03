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

sbit key1=P2^2;
sbit key2=P2^3;
sbit key3=P1^1;
sbit key4=P1^0;

uchar getKey();

#endif