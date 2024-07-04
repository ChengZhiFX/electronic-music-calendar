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
static uchar event_chinese[] = {69,70,29,30}, music_chinese[] = {2,3,66,67,68}, weather_chinese[] = {4,5}, calendar_chinese[] = {6,7,8};
uchar hour_type = 24;

void switch_hour_type(){
	if(hour_type == 12) hour_type = 24;
	else if(hour_type == 24) hour_type = 12;
}

void page_calendar(){
	char press_key_turn_page_chinese[] = {58,112,106,61,113,114};
	Date date;
	date.year = get_integer_year();
	date.month = get_integer_month();
	date.day = get_integer_day();
	OLED_Clear();
	OLED_ShowChineseString(40,0,0,calendar_chinese,3);
	OLED_ShowChineseString(16,6,0,press_key_turn_page_chinese,6);
	while(1){
		print_date_any_time(0, 2, date.year, date.month, date.day);
		print_lunar_and_term_any_time(12,4, date.year, date.month, date.day);
		if(getKey() == 1) date = date_addition_adjust(date.year, date.month, date.day + 1);
		else if(getKey() == 2) date = date_subtraction_adjust(date.year, date.month, date.day - 1);
		else if(getKey() == 3 || getKey() == 4){
			OLED_Clear();
			break;
		}
	}
}

void page_function_menu(){
	uchar selection = 0;
	OLED_Clear();
	while(1){
		OLED_ShowChineseString(24,0,0,music_chinese,5);
		OLED_ShowChineseString(40,2,0,calendar_chinese,3);
		OLED_ShowChineseString(48,4,0,weather_chinese,2);
		OLED_ShowChineseString(32,6,0,event_chinese,4);
//		OLED_ShowString(24,0,"Alarm",16);
//		OLED_ShowString(24,2,"Music",16);
//		OLED_ShowString(24,4,"Weather",16);
//		OLED_ShowString(24,6,"Calendar",16);
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
				case 0: {page_music(); break;}
				case 1: {page_calendar(); break;}
				case 2: {page_weather_all(); break;}
				case 3: {page_event_view(); break;}
				default: break;
			}
		}
	}
}

/*******************************Main*******************************/
void main(){
	uchar index = 1;
	delay_ms(50);//等待系统稳定
	ds1302_init();  //DS1302初始化
	UsartConfiguration();
	OLED_Init();
	OLED_Display_On();
	OLED_Clear();
	OLED_DrawBMP(0, 0, 128, 8, start_up);
	alarm_init();
	weather_init();
	event_init();
	dht11_try_catch_data();
	mp3_init();
	delay_ms(2000);
	OLED_Clear();
	while(1){
		print_time_now(0,3,hour_type);
		print_date_now(0,1);
		if(index){print_temp_and_hum(38, 0); index = 0;}
		else{mq2_tick_tock(); index = 1;}
		event_tick_tock(0,6);
		alarm_tick_tock();
		switch(getKey()){
			case 1: {switch_hour_type(); break;}
			case 2: {page_alarm(); break;}
			case 3: {page_function_menu(); break;}
			case 4: {page_settings(); break;}
			default: break;
		}
	}
}