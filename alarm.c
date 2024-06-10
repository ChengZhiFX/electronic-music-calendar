#include "alarm.h"
#include "oled.h"
#include "ds1302.h"
#include "keyscan.h"
#include "delay.h"
#include "mp3.h"

uchar alarm_suspend = 0;
Alarm alarm;
//Alarm alarm_delay;

void alarm_init(){
	alarm.hour = get_integer_hour();
	alarm.minute = get_integer_minute();
	alarm.ringtone = 1;
	alarm.enable = 0;
	alarm_suspend = 0;
	set_volume(15);
}

char adjust_12(char month_num){
	if(month_num > 12) month_num = 1;
	return month_num;
}

char adjust_30(uint year_num, char month_num, char day_num){
	if(month_num == 2){
		if(isLeapYear(year_num)){
			if(day_num > 29) day_num = 1;
		}
		else {
			if(day_num > 28) day_num = 1;
		}
	}
	else if(month_num == 4 || month_num == 6 || month_num == 9 || month_num == 11){
		if(day_num > 30) day_num = 1;
	}
	else{
		if(day_num > 31) day_num = 1;
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

void set_alarm(char hour, char minute, char music, uchar enable){
	alarm.hour = hour;
	alarm.minute = minute;
	alarm.ringtone = music;
	alarm.enable = enable;
	alarm_suspend = 0;
}

void page_alarm(){
	char title[] = "Alarm1";
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
	while(1){
		OLED_ShowString(32,0,title,16);
		if(step == 1) OLED_ShowString_Reverse(32,2,alarm_hour,16);
		else OLED_ShowString(32,2,alarm_hour,16);
		OLED_ShowChar(48,2,':',16);
		if(step == 2) OLED_ShowString_Reverse(56,2,alarm_minute,16);
		else OLED_ShowString(56,2,alarm_minute,16);
		if(step == 3) OLED_ShowString_Reverse(32,4,alarm_music,16);
		else OLED_ShowString(32,4,alarm_music,16);
		if(step == 4){
			if(alarm.enable) OLED_ShowString_Reverse(32,6,"Enabled",16);
			else OLED_ShowString_Reverse(32,6,"Disable",16);
		}
		else{
			if(alarm.enable) OLED_ShowString(32,6,"Enabled",16);
			else OLED_ShowString(32,6,"Disable",16);
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
					if(alarm.enable) alarm.enable = 0;
					else alarm.enable = 1;
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
			OLED_ShowString(32,2,"Saved!",16);
			delay_ms(2000);
			OLED_Clear();
			break;
		}
	}
}

void alarm_tick_tock(){
	if(alarm.enable){
		if(get_integer_hour() == alarm.hour && get_integer_minute() == alarm.minute){
			if(!alarm_suspend) page_ring();
		}
		else{
			if(alarm_suspend) alarm_suspend = 0;
		}
	}
}

void page_ring(){
	OLED_Clear();
	OLED_ShowString(0,2,"Alarm going off",16);
	set_single_loop(1);
	playmusic(alarm.ringtone);
	while(1){
		if(getKey() || get_integer_minute() == alarm.minute + 3){
			stopmusic();
			set_single_loop(0);
			alarm_suspend = 1;
			OLED_Clear();
			break;
		}
	}
}
