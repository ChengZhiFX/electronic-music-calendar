#include "event.h"
#include "ds1302.h"
#include "keyscan.h"

Event event;

void event_init(){
	event.year = get_integer_year();
	event.month = get_integer_month();
	event.day = get_integer_day();
	event.changed = 0;
}

uchar is_event_about_to_happen(){
	if(get_integer_year() == event.year && get_integer_month() == event.month && get_integer_day() == event.day - 1)
		return 1;
	else return 0;
}

void set_event_control(uint year, char month, char day, uchar enable){
	event.year = year;
	event.month = month;
	event.day = day;
}

void set_event_content(char content[20]){
	uchar i;
	for(i=0;content[i]!=0 && i<16;i++) event.content[i] = content[i];
	if(i<16) event.content[i] = 0;
	else event.content[16] = 0;
	event.changed = 1;
}

void event_tick_tock(uchar x, uchar y){
	if(event.changed){
		OLED_ShowString(x,y,"                ",16);
		event.changed = 0;
	}
	if(is_event_about_to_happen()) OLED_ShowString(x,y,event.content,16);
}

//void page_reminder(){
//	OLED_Clear();
//	OLED_ShowString(0,2,event.content,16);
//	while(1){
//		if(getKey()){
//			event.enable = 0;
//			OLED_Clear();
//			break;
//		}
//	}
//}
