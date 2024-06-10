#ifndef _EVENT_H_
#define _EVENT_H_

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

typedef struct event
{	uint year;
	char month;
	char day;
	char content[20];
	uchar changed;
}Event;

void event_init();
uchar is_event_about_to_happen();
void set_event_control(uint year, char month, char day, uchar enable);
void set_event_content(char content[20]);
void event_tick_tock(uchar x, uchar y);
void page_reminder();

#endif