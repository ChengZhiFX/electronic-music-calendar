#include "ds1302.h"
#include "settings.h"
#include "mp3.h"
#include "alarm.h"
#include "keyscan.h"
#include "delay.h"
#include "oled.h"

extern uchar time_buf[8];
extern uchar smog_alarm_enable;

void page_set_date_and_time(){
	char year_to_show[5] = "1970";
	char month_to_show[3] = "01";
	char day_to_show[3] = "01";
	char hour_to_show[3] = "00";
	char minute_to_show[3] = "00";
	char second_to_show[3] = "00";
	uint year = get_integer_year();
	uchar month = get_integer_month(), day = get_integer_day(), weekday = get_integer_weekday();
	uchar hour = get_integer_hour(), minute = get_integer_minute(), second = get_integer_second(), step = 1;
	OLED_Clear();
	quadruple_digit_to_string(year, year_to_show);
	double_digit_to_string(month, month_to_show);
	double_digit_to_string(day, day_to_show);
	double_digit_to_string(hour, hour_to_show);
	double_digit_to_string(minute, minute_to_show);
	double_digit_to_string(second, second_to_show);
	OLED_ShowString(8,0,"Set Date&Time",16);
	while(1){
		OLED_ShowChar(104,2,Char(weekday),16);
		if(step == 1) OLED_ShowString_Reverse(16,2,year_to_show,16);
		else OLED_ShowString(16,2,year_to_show,16);
		OLED_ShowChar(48,2,'/',16);
		if(step == 2) OLED_ShowString_Reverse(56,2,month_to_show,16);
		else OLED_ShowString(56,2,month_to_show,16);
		OLED_ShowChar(72,2,'/',16);
		if(step == 3) OLED_ShowString_Reverse(80,2,day_to_show,16);
		else OLED_ShowString(80,2,day_to_show,16);
		if(step == 4) OLED_ShowString_Reverse(32,4,hour_to_show,16);
		else OLED_ShowString(32,4,hour_to_show,16);
		OLED_ShowChar(48,4,':',16);
		if(step == 5) OLED_ShowString_Reverse(56,4,minute_to_show,16);
		else OLED_ShowString(56,4,minute_to_show,16);
		OLED_ShowChar(72,4,':',16);
		if(step == 6) OLED_ShowString_Reverse(80,4,second_to_show,16);
		else OLED_ShowString(80,4,second_to_show,16);
		if(getKey() == 1){
			switch(step){
				case 1: {
					year++;
					if(year >= 10000) year = 0;
					quadruple_digit_to_string(year, year_to_show);
					day = adjust_30(year, month, day);
					double_digit_to_string(day, day_to_show);
					weekday = calculate_week_day(year, month, day);
					break;
				}
				case 2: {
					month++;
					month = adjust_12(month);
					double_digit_to_string(month, month_to_show);
					day = adjust_30(year, month, day);
					double_digit_to_string(day, day_to_show);
					weekday = calculate_week_day(year, month, day);
					break;
				}
				case 3: {
					day++;
					day = adjust_30(year, month, day);
					double_digit_to_string(day, day_to_show);
					weekday = calculate_week_day(year, month, day);
					break;
				}
				case 4: {
					hour++;
					hour = adjust_24(hour);
					double_digit_to_string(hour, hour_to_show);
					break;
				}
				case 5: {
					minute++;
					minute = adjust_60(minute);
					double_digit_to_string(minute, minute_to_show);
					break;
				}
				case 6: {
					second++;
					second = adjust_60(second);
					double_digit_to_string(second, second_to_show);
					break;
				}
				default: break;
			}
		}
		else if(getKey() == 2){
			step++;
			if(step > 6) step = 1;
		}
		else if(getKey() == 3){
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			OLED_Clear();
			write_date_and_time(year, month, day, hour, minute, second);
			OLED_ShowString(32,2,"Saved!",16);
			delay_ms(2000);
			OLED_Clear();
			break;
		}
	}
}

void page_set_notification(){
	char vol_to_show[] = "Volume:  ";
	vol_to_show[8] = (char)(get_volume() / 5 + '0');
	OLED_Clear();
	OLED_ShowString(16,0,"Notification",16);
	
	while(1){
		OLED_ShowString(20,2,vol_to_show,16);
		if(smog_alarm_enable) OLED_ShowString(32,6,"Enabled",16);
		else OLED_ShowString(32,6,"Disable",16);
		if(getKey() == 1){
			stopmusic();
			set_volume(get_volume()+5);
			vol_to_show[8] = (char)(get_volume() / 5 + '0');
			playmusic(10);
		}
		else if(getKey() == 2){
			stopmusic();
			set_volume(get_volume()-5);
			vol_to_show[8] = (char)(get_volume() / 5 + '0');
			playmusic(10);
		}
		else if(getKey() == 3){
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			if(smog_alarm_enable) smog_alarm_enable = 0;
			else smog_alarm_enable = 1;
		}
	}
}

void page_set_display(){
	
}

void page_about(){
	OLED_Clear();
	while(1){
		OLED_ShowString(16,0,"Copyright(C)",16);
		OLED_ShowString(8,2,"Zhang Jiacheng",16);
		OLED_ShowString(16,4,"202200120145",16);
		OLED_ShowString(0,6,"Chongxin College",16);
		if(getKey()){
			OLED_Clear();
			break;
		}
	}
}

void page_settings(){
	uchar selection = 0;
	OLED_Clear();
	while(1){
		OLED_ShowString(24,0,"Date&Time",16);
		OLED_ShowString(24,2,"Notification",16);
		OLED_ShowString(24,4,"Display",16);
		OLED_ShowString(24,6,"About",16);
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
				case 0: {page_set_date_and_time(); break;}
				case 1: {page_set_notification(); break;}
				case 2: {page_set_display(); break;}
				case 3: {page_about(); break;}
				default: break;
			}
		}
	}
}