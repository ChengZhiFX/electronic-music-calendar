#ifndef _EVENT_H_
#define _EVENT_H_

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

typedef struct date
{	uint year;
	char month;
	char day;
}Date;

typedef struct event
{	uint year;
	char month;
	char day;
	char content[20];
	uchar status;
	uint year_of_last_day;
	char month_of_last_day;
	char day_of_last_day;
}Event;

void event_init();
Date date_addition_adjust(uint year_num, char month_num, char day_num);
Date date_subtraction_adjust(uint year_num, char month_num, char day_num);
uchar is_event_about_to_happen();
void set_event_control(uint year, char month, char day, uchar enable);
void set_event_content(char content[20]);
void event_tick_tock(uchar x, uchar y);
void page_event_view();

#endif