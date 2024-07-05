#include "alarm.h"
#include "oled.h"
#include "ds1302.h"
#include "keyscan.h"
#include "delay.h"
#include "mp3.h"
#include "bmp.h"

uchar alarm_suspend = 0;
Alarm alarm;

void alarm_init(){
	alarm.hour = get_integer_hour();
	alarm.minute = get_integer_minute();
	alarm.ringtone = 1;
	alarm.status = 0;
	alarm_suspend = 0;
}

char adjust_12(char month_num){
	if(month_num > 12) month_num = 1;
	return month_num;
}

char adjust_30(uint year_num, char month_num, char day_num, uchar keep_max){
	if(month_num == 2){
		if(isLeapYear(year_num)){
			if(day_num > 29) day_num = keep_max? 29:1;
		}
		else {
			if(day_num > 28) day_num = keep_max? 28:1;
		}
	}
	else if(month_num == 4 || month_num == 6 || month_num == 9 || month_num == 11){
		if(day_num > 30) day_num = keep_max? 30:1;
	}
	else{
		if(day_num > 31) day_num = keep_max? 31:1;
	}
	return day_num;
}

char adjust_24(char hour_num){
	if(hour_num >= 24) hour_num = 0;
	return hour_num;
}

char adjust_60(char minute_num){
	if(minute_num >= 60) minute_num = 0;
	return minute_num;
}

void set_alarm(char hour, char minute, char music, uchar status){
	alarm.hour = hour;
	alarm.minute = minute;
	alarm.ringtone = music;
	alarm.status = status;
	alarm_suspend = 0;
}

void page_alarm(){
	char title_chinese[] = {0,1,19,36}, ringtone_chinese[] = {17,96}, repeat_chinese[] = {97,98}, saved_chinese[] = {0,1,13,14,15};
	char disabled_chinese[] = {105,33,9}, once_chinese[] = {99,16,77,100}, everyday_chinese[] = {101,4}, workday_chinese[] = {103,77,102,103,104};
//	char title[] = "Alarm1";
	char alarm_hour[3] = "00";
	char alarm_minute[3] = "00";
	char alarm_music[] = "Music ";
	uchar step = 1;
	Alarm alarm_original;
	OLED_Clear();
	alarm_original = alarm;
	double_digit_to_string(alarm.hour, alarm_hour);
	double_digit_to_string(alarm.minute, alarm_minute);
	alarm_music[5] = Char(alarm.ringtone);
	OLED_ShowChineseString(32,0,0,title_chinese,4);
//	OLED_ShowString(32,0,title,16);
	OLED_ShowChar(60,2,':',16);
	OLED_ShowChineseString(0,4,0,ringtone_chinese,2);
	OLED_ShowChar(36,4,'|',16);
	OLED_ShowChineseString(0,6,0,repeat_chinese,2);
	OLED_ShowChar(36,6,'|',16);
	while(1){
		if(step == 1) OLED_ShowString_Reverse(44,2,alarm_hour,16);
		else OLED_ShowString(44,2,alarm_hour,16);
		if(step == 2) OLED_ShowString_Reverse(68,2,alarm_minute,16);
		else OLED_ShowString(68,2,alarm_minute,16);
		if(step == 3) OLED_ShowString_Reverse(64,4,alarm_music,16);
		else OLED_ShowString(64,4,alarm_music,16);
		if(step == 4){
			if(alarm.status == 0) OLED_ShowChineseString_Reverse(64,6,0,disabled_chinese,3);
			else if(alarm.status == 1) OLED_ShowChineseString_Reverse(56,6,0,once_chinese,4);
			else if(alarm.status == 2) OLED_ShowChineseString_Reverse(72,6,0,everyday_chinese,2);
			else if(alarm.status == 3) OLED_ShowChineseString_Reverse(48,6,0,workday_chinese,5);
		}
		else{
			if(alarm.status == 0) OLED_ShowChineseString(64,6,0,disabled_chinese,3);
			else if(alarm.status == 1) OLED_ShowChineseString(56,6,0,once_chinese,4);
			else if(alarm.status == 2) OLED_ShowChineseString(72,6,0,everyday_chinese,2);
			else if(alarm.status == 3) OLED_ShowChineseString(48,6,0,workday_chinese,5);
		}
		if(getKey() == 1){
			switch(step){
				case 1: {
					alarm.hour++;
					alarm.hour = adjust_24(alarm.hour);
					double_digit_to_string(alarm.hour, alarm_hour);
					break;
				}
				case 2: {
					alarm.minute++;
					alarm.minute = adjust_60(alarm.minute);
					double_digit_to_string(alarm.minute, alarm_minute);
					break;
				}
				case 3: {
					alarm.ringtone++;
					if(alarm.ringtone >= 10) alarm.ringtone = 1;
					alarm_music[5] = Char(alarm.ringtone);
					break;
				}
				case 4: {
					alarm.status++;
					if(alarm.status > 3) alarm.status = 0;
					OLED_ShowString(48,6,"          ",16);
					break;
				}
				default: break;
			}
		}
		else if(getKey() == 2){
			step++;
			if(step > 4) step = 1;
		}
		else if(getKey() == 3){
			OLED_Clear();
			alarm = alarm_original;
			break;
		}
		else if(getKey() == 4){
			OLED_Clear();
			alarm_suspend = 0;
			OLED_DrawBMP(0, 0, 128, 4, success_icon);
			OLED_ShowChineseString(24,4,0,saved_chinese,5);
//			OLED_ShowString(32,2,"Saved!",16);
			delay_ms(2000);
			OLED_Clear();
			break;
		}
	}
}

void alarm_tick_tock(){
	if(!alarm.status) return;
	else if(alarm.status == 3 && get_integer_weekday() > 5) return;
	if(get_integer_hour() == alarm.hour && get_integer_minute() == alarm.minute){
		if(!alarm_suspend) page_ring();
	}
	else{
		if(alarm_suspend) alarm_suspend = 0;
	}
}

void page_ring(){
	uchar alarm_going_off_chinese[] = {0,1,16,17,18}, press_key_cancel_chinese[] = {58,59,60,61,62,63};
	OLED_Clear();
	OLED_DrawBMP(0, 0, 128, 4, alarm_icon);
	OLED_ShowChineseString(24,4,0,alarm_going_off_chinese,5);
	OLED_ShowChineseString(16,6,0,press_key_cancel_chinese,6);
//	OLED_ShowString(0,2,"Alarm going off",16);
	set_single_loop(1);
	playmusic(alarm.ringtone,1);
	while(1){
		if(getKey() || get_integer_minute() == alarm.minute + 3){
			stopmusic();
			set_single_loop(0);
			alarm_suspend = 1;
			if(alarm.status == 1){
				alarm.status = 0;
				alarm_suspend = 0;
			}
			OLED_Clear();
			break;
		}
	}
}
