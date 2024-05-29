#ifndef __SETTINGS_H_
#define __SETTINGS_H_

#include<reg52.h>
#include<intrins.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

void page_set_date_and_time();
void page_settings();

#endif