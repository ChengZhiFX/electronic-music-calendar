#ifndef __ALARM_H_
#define __ALARM_H_

//---包含头文件---//
#include<reg52.h>
#include<intrins.h>


#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

typedef struct alarm
{	char minute;
	char hour;
	char ringtone;
	uchar enable;
}Alarm;

char adjust_12(char month_num);
char adjust_30(uint year_num, char month_num, char day_num);
char adjust_24(char hour_num);
char adjust_60(char minute_num);
void alarm_init();
void page_alarm();
void alarm_tick_tock();
void page_ring();

#endif