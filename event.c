#include "event.h"
#include "ds1302.h"
#include "keyscan.h"
#include "alarm.h"
#include "delay.h"

Date today;
Event event;
uchar redraw = 0, event_about_to_happen_old = 0, event_about_to_happen_new = 0, event_status_temp = 0;

void event_init(){
	event.year = 0;
	event.month = 1;
	event.day = 2;
	event.year_of_last_day = 0;
	event.month_of_last_day = 1;
	event.day_of_last_day = 1;
	event.status = 0;
}

Date date_addition_adjust(uint year_num, char month_num, char day_num){
	Date correct_date;
	day_num = adjust_30(year_num, month_num, day_num, 0);
	if(day_num == 1) month_num++;
	if (month_num > 12) {
		month_num = 1;
		year_num++;
	}
	correct_date.year = year_num;
	correct_date.month = month_num;
	correct_date.day = day_num;
	return correct_date;
}

Date date_subtraction_adjust(uint year_num, char month_num, char day_num){
	Date correct_date;
	if(day_num < 1) month_num--;
	if(month_num < 1){
		year_num--;
		month_num = 12;
	}
	if(day_num < 1) day_num = adjust_30(year_num, month_num, 32, 1);
	correct_date.year = year_num;
	correct_date.month = month_num;
	correct_date.day = day_num;
	return correct_date;
}

uchar is_event_about_to_happen(){
	today.year = get_integer_year();
	today.month = get_integer_month();
	today.day = get_integer_day();
	if(today.year == event.year && today.month == event.month && today.day == event.day) return 1;
	if(event.status == 2 && today.year == event.year_of_last_day && today.month == event.month_of_last_day && today.day == event.day_of_last_day) return 1;
	return 0;
}

void set_event_control(uint year, char month, char day, uchar status){
	Date last_day;
	event.year = year;
	event.month = month;
	event.day = day;
	event.status = status;
	last_day = date_subtraction_adjust(event.year, event.month, event.day - 1);
	event.year_of_last_day = last_day.year;
	event.month_of_last_day = last_day.month;
	event.day_of_last_day = last_day.day;
	event_status_temp = event.status;
}

void set_event_content(char content[20]){
	uchar i;
	for(i=0;content[i]!=0 && i<16;i++) event.content[i] = content[i];
	if(i<16) event.content[i] = 0;
	else event.content[16] = 0;
	redraw = 1;
}

void event_tick_tock(uchar x, uchar y){
	event_about_to_happen_new = is_event_about_to_happen();
	if(event_about_to_happen_new != event_about_to_happen_old){
		redraw = 1;
		event_about_to_happen_old = event_about_to_happen_new;
	}
	if(redraw){
		OLED_ShowString(x,y,"                ",16);
		redraw = 0;
	}
	if(event.status && event_about_to_happen_new){
		OLED_ShowString(x,y,event.content,16);
	}
	else{
		print_lunar_and_term_now(12, y);
	}
}

void page_event_null(){
	char ble_add_chinese[] = {43,48,49,50,51,72,73}, no_event_chinese[] = {71,29,30,69,70}, press_key_cancel_chinese[] = {58,59,60,61,62,63};
	OLED_Clear();
	OLED_ShowChineseString(24,0,0,no_event_chinese,5);
	OLED_ShowChineseString(8,2,0,ble_add_chinese,7);
	OLED_ShowChineseString(16,6,0,press_key_cancel_chinese,6);
	while(1){
		if(event.year || getKey()){
			OLED_Clear();
			break;
		}
	}
}

void page_event_view(){
	char remind_chinese[] = {29,30}, only_same_chinese[] = {80,79,4}, same_and_in_advance_chinese[] = {79,4,23,29,76,77,4};
	char saved_chinese[] = {29,30,13,14,15}, if_sure_to_clear_chinese[] = {81,36,82,83,69,70,84}, cancel_chinese[] = {85,86}, sure_chinese[] = {81,36};
	uchar weekday_char[2] = {16, 15};
	OLED_Clear();
	while(1){
		if(!event.year){
			page_event_null();
			if(!event.year) return;
		}
		if(redraw){
			OLED_Clear();
			redraw = 0;
		}
		OLED_ShowString(0,0,event.content,16);
		print_date_any_time(0, 2, event.year, event.month, event.day);
		OLED_ShowChineseString(0,4,0,remind_chinese,2);
		OLED_ShowChar(32,4,':',16);
		if(event_status_temp == 0) OLED_ShowChinese_Reverse(48,4,0,71);
		else OLED_ShowChinese(48,4,0,71);
		if(event_status_temp == 1) OLED_ShowChineseString_Reverse(80,4,0,only_same_chinese,3);
		else OLED_ShowChineseString(80,4,0,only_same_chinese,3);
		if(event_status_temp == 2) OLED_ShowChineseString_Reverse(8,6,0,same_and_in_advance_chinese,7);
		else OLED_ShowChineseString(8,6,0,same_and_in_advance_chinese,7);
		if(getKey() == 1){
			OLED_Clear();
			OLED_ShowChineseString(8,2,0,if_sure_to_clear_chinese,7);
			OLED_ShowChineseString(0,6,0,cancel_chinese,2);
			OLED_ShowChineseString(48,6,0,sure_chinese,2);
			while(1){
				if(getKey() == 3) break;
				else if(getKey() == 4){
					event_init();
					break;
				}
			}
			OLED_Clear();
		}
		else if(getKey() == 2){
			event_status_temp++;
			if(event_status_temp > 2) event_status_temp = 0;
		}
		else if(getKey() == 3){
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			OLED_Clear();
			event.status = event_status_temp;
			OLED_ShowChineseString(24,2,0,saved_chinese,5);
//			OLED_ShowString(32,2,"Saved!",16);
			delay_ms(2000);
			OLED_Clear();
			break;
		}
	}
}
