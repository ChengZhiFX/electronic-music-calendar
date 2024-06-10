#ifndef _MQ2_H_
#define _MQ2_H_

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit mq2 = P1^2;

uchar isSmoking();
void page_smog_alarm();
void mq2_tick_tock();

#endif