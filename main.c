#include "reg52.h"
#include "oled.h"
#include "bmp.h"
#include "dht11.h"
#include "ds1302.h"
#include "keyscan.h"
#include "mp3.h"
#include "alarm.h"
#include "settings.h"
#include "delay.h"
#include "lunar.h"
#include "mq2.h"
#include "hc08.h"
#include "weather.h"
#include "event.h"

extern Alarm alarm;

uchar hour_type = 24;

void switch_hour_type(){
	if(hour_type == 12) hour_type = 24;
	else if(hour_type == 24) hour_type = 12;
}

void function3(){
	uchar selection = 0;
	OLED_Clear();
	while(1){
		OLED_ShowString(24,0,"Alarm",16);
		OLED_ShowString(24,2,"Music",16);
		OLED_ShowString(24,4,"Weather",16);
		OLED_ShowString(24,6,"Calendar",16);
		OLED_ShowChar(16,selection*2,'>',16);
		if(getKey() == 1){
			OLED_ShowChar(16,selection*2,' ',16);
			if(selection == 0) selection = 3;
			else selection -= 1;
		}
		else if(getKey() == 2){
			OLED_ShowChar(16,selection*2,' ',16);
			if(selection == 3) selection = 0;
			else selection += 1;
		}
		else if(getKey() == 3){
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			switch(selection){
				case 0: {page_alarm(); break;}
				case 1: {page_music(); break;}
				case 2: {page_weather_all(); break;}
				case 3: {/*page_calendar();*/ break;}
				default: break;
			}
		}
	}
}

void function2(){
	uint year = get_integer_year();
	uchar month = get_integer_month(), day = get_integer_day();
	OLED_Clear();
	while(1){
		print_date_any_time(0, 2, year, month, day);
		print_lunar_and_term_any_time(12,4, year, month, day);
		if(getKey() == 1){
			day++;
			if(day>28) day = 28;
		}
		else if(getKey() == 2){
			day--;
			if(day<1) day = 1;
		}
		else if(getKey() == 3){
			OLED_Clear();
			break;
		}
	}
}

void function4(){
	page_settings();
}

/*******************************Main*******************************/
void main(){
	Delay_50ms(1);//等待系统稳定
	ds1302_init();  //DS1302初始化
	UsartConfiguration();
	OLED_Init();
	OLED_Display_On();
	OLED_Clear();
	OLED_DrawBMP(0, 0, 128, 8, start_up);
	Delay_50ms(25);
	OLED_Clear();
	alarm_init();
	weather_init();
	event_init();
	while(1){
		print_time_now(0,3,hour_type);
		print_date_now(0,1);
//		print_weather_base_now(80, 2);
		print_temp_and_hum(44, 0);
		switch(getKey()){
			case 1: {switch_hour_type(); break;}
			case 2: {function2(); break;}
			case 3: {function3(); break;}
			case 4: {function4(); break;}
			default: break;
		}
		alarm_tick_tock();
		mq2_tick_tock();
		event_tick_tock(0,6);
		if(!is_event_about_to_happen()) print_lunar_and_term_now(12, 6);
	}
}