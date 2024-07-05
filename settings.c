#include "ds1302.h"
#include "settings.h"
#include "mp3.h"
#include "alarm.h"
#include "keyscan.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"
#include "dht11.h"

void page_set_date_and_time(){
	char set_date_and_time_chinese[] = {21,22,23,24,25,19,20}, saved_chinese[] = {19,20,13,14,15}, cancel_chinese[] = {62,63}, save_chinese[] = {14,15};
	char year_to_show[5] = "1970", month_to_show[3] = "01", day_to_show[3] = "01", hour_to_show[3] = "00", minute_to_show[3] = "00", second_to_show[3] = "00";
	uchar weekday_char[2] = {16, 15};
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
	OLED_ShowChineseString(8,0,0,set_date_and_time_chinese,7);
//	OLED_ShowString(8,0,"Set Date&Time",16);
	if(calculate_week_day(year, month, day) == 7) weekday_char[1] = 17;
	else weekday_char[1] = calculate_week_day(year, month, day);
	OLED_ShowChineseString(0,6,0,cancel_chinese,2);
	OLED_ShowChineseString(48,6,0,save_chinese,2);
	while(1){
		if(step == 1) OLED_ShowString_Reverse(0,2,year_to_show,16);
		else OLED_ShowString(0,2,year_to_show,16);
		OLED_ShowChar(32,2,'/',16);
		if(step == 2) OLED_ShowString_Reverse(40,2,month_to_show,16);
		else OLED_ShowString(40,2,month_to_show,16);
		OLED_ShowChar(56,2,'/',16);
		if(step == 3) OLED_ShowString_Reverse(64,2,day_to_show,16);
		else OLED_ShowString(64,2,day_to_show,16);
		OLED_ShowChineseString(90,2,1,weekday_char,2);
		
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
					day = adjust_30(year, month, day, 0);
					double_digit_to_string(day, day_to_show);
					if(calculate_week_day(year, month, day) == 7) weekday_char[1] = 17;
					else weekday_char[1] = calculate_week_day(year, month, day);
					break;
				}
				case 2: {
					month++;
					month = adjust_12(month);
					double_digit_to_string(month, month_to_show);
					day = adjust_30(year, month, day, 0);
					double_digit_to_string(day, day_to_show);
					if(calculate_week_day(year, month, day) == 7) weekday_char[1] = 17;
					else weekday_char[1] = calculate_week_day(year, month, day);
					break;
				}
				case 3: {
					day++;
					day = adjust_30(year, month, day, 0);
					double_digit_to_string(day, day_to_show);
					if(calculate_week_day(year, month, day) == 7) weekday_char[1] = 17;
					else weekday_char[1] = calculate_week_day(year, month, day);
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
			OLED_DrawBMP(0, 0, 128, 4, success_icon);
			OLED_ShowChineseString(24,4,0,saved_chinese,5);
//			OLED_ShowString(32,2,"Saved!",16);
			delay_ms(2000);
			OLED_Clear();
			break;
		}
	}
}

void page_set_notification(){
	char title_chinese[] = {26,27,28,29,30}, notification_vol_chinese[] = {26,27,2,10}, media_vol_chinese[] = {108,109,2,10}, alert_vol_chinese[] = {110,111,2,10};
	uchar step = 1;
	OLED_Clear();
	OLED_ShowChineseString(24,0,0,title_chinese,5);
	OLED_ShowChineseString(20,2,0,notification_vol_chinese,4);
	OLED_ShowChar(84,2,':',16);
	OLED_ShowChineseString(20,4,0,media_vol_chinese,4);
	OLED_ShowChar(84,4,':',16);
	OLED_ShowChineseString(20,6,0,alert_vol_chinese,4);
	OLED_ShowChar(84,6,':',16);
	while(1){
		if(step == 1) OLED_ShowChar_Reverse(100,2,Char(get_notification_volume() / 5),16);
		else OLED_ShowChar(100,2,Char(get_notification_volume() / 5),16);
		if(step == 2) OLED_ShowChar_Reverse(100,4,Char(get_media_volume() / 5),16);
		else OLED_ShowChar(100,4,Char(get_media_volume() / 5),16);
		if(step == 3) OLED_ShowChar_Reverse(100,6,Char(get_alert_volume() / 5),16);
		else OLED_ShowChar(100,6,Char(get_alert_volume() / 5),16);
		if(getKey() == 1){
			stopmusic();
			if(step == 1){
				set_notification_volume(get_notification_volume() + 5);
				playmusic(10,1);
			}
			else if(step == 2){
				set_media_volume(get_media_volume() + 5);
				playmusic(17,2);
			}
			else if(step == 3){
				set_alert_volume(get_alert_volume() + 5);
				playmusic(19,3);
			}
		}
		else if(getKey() == 2){
			stopmusic();
			if(step == 1){
				set_notification_volume(get_notification_volume() - 5);
				playmusic(10,1);
			}
			else if(step == 2){
				set_media_volume(get_media_volume() - 5);
				playmusic(17,2);
			}
			else if(step == 3){
				set_alert_volume(get_alert_volume() - 5);
				playmusic(19,3);
			}
		}
		else if(getKey() == 3){
			stopmusic();
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			step++;
			if(step > 3) step = 1;
		}
	}
}

void page_mod_switch(){
	char temp_and_hum_sensor_chinese[] = {89,90,91,92,93}, audio_out_chinese[] = {2,94,95,63}, bluetooth_chinese[] = {50,51}, smog_sensor_chinese[] = {41,42,92,93};
	char enabled_chinese[] = {9,12}, disabled_chinese[] = {11,12}, saved_chinese[] = {19,20,13,14,15};
	char unable_to_boot_chinese[] = {115,31,32,71,116,9,117}, check_connection_chinese[] = {43,38,46,48,49,118,97,119};
	extern uchar dht11_enabled, mp3_enabled, hc08_enabled, mq2_enabled;
	uchar step = 1, dht11_temp = dht11_enabled, mp3_temp = mp3_enabled, hc08_temp = hc08_enabled, mq2_temp = mq2_enabled;
	uchar dht11_old = dht11_enabled, mp3_old = mp3_enabled, mq2_old = mq2_enabled;
	OLED_Clear();
	OLED_ShowChineseString(0,0,0,temp_and_hum_sensor_chinese,5);
	OLED_ShowChar(80,0,'|',16);
	OLED_ShowChineseString(0,2,0,audio_out_chinese,4);
	OLED_ShowChar(80,2,'|',16);
	OLED_ShowChineseString(0,4,0,bluetooth_chinese,2);
	OLED_ShowChar(80,4,'|',16);
	OLED_ShowChineseString(0,6,0,smog_sensor_chinese,4);
	OLED_ShowChar(80,6,'|',16);
	while(1){
		if(dht11_old != dht11_enabled){dht11_old = dht11_enabled; dht11_temp = dht11_enabled;}
		if(mp3_old != mp3_enabled){mp3_old = mp3_enabled; mp3_temp = mp3_enabled;}
		if(mq2_old != mq2_enabled){mq2_old = mq2_enabled; mq2_temp = mq2_enabled;}
		if(step == 1){
			if(dht11_temp) OLED_ShowChineseString_Reverse(96,0,0,enabled_chinese,2);
			else OLED_ShowChineseString_Reverse(96,0,0,disabled_chinese,2);
		}
		else{
			if(dht11_temp) OLED_ShowChineseString(96,0,0,enabled_chinese,2);
			else OLED_ShowChineseString(96,0,0,disabled_chinese,2);
		}
		if(step == 2){
			if(mp3_temp) OLED_ShowChineseString_Reverse(96,2,0,enabled_chinese,2);
			else OLED_ShowChineseString_Reverse(96,2,0,disabled_chinese,2);
		}
		else{
			if(mp3_temp) OLED_ShowChineseString(96,2,0,enabled_chinese,2);
			else OLED_ShowChineseString(96,2,0,disabled_chinese,2);
		}
		if(step == 3){
			if(hc08_temp) OLED_ShowChineseString_Reverse(96,4,0,enabled_chinese,2);
			else OLED_ShowChineseString_Reverse(96,4,0,disabled_chinese,2);
		}
		else{
			if(hc08_temp) OLED_ShowChineseString(96,4,0,enabled_chinese,2);
			else OLED_ShowChineseString(96,4,0,disabled_chinese,2);
		}
		if(step == 4){
			if(mq2_temp) OLED_ShowChineseString_Reverse(96,6,0,enabled_chinese,2);
			else OLED_ShowChineseString_Reverse(96,6,0,disabled_chinese,2);
		}
		else{
			if(mq2_temp) OLED_ShowChineseString(96,6,0,enabled_chinese,2);
			else OLED_ShowChineseString(96,6,0,disabled_chinese,2);
		}
		if(getKey() == 1){
			if(step == 1) dht11_temp = dht11_temp? 0:1;
			else if(step == 2) mp3_temp = mp3_temp? 0:1;
			else if(step == 3) hc08_temp = hc08_temp? 0:1;
			else if(step == 4) mq2_temp = mq2_temp? 0:1;
		}
		else if(getKey() == 2){
			step++;
			if(step > 4) step = 1;
		}
		else if(getKey() == 3){
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			OLED_Clear();
			if(dht11_temp && !dht11_try_catch_data()){
				OLED_ShowChineseString(24,0,0,temp_and_hum_sensor_chinese,5);
				OLED_ShowChineseString(8,4,0,unable_to_boot_chinese,7);
				OLED_ShowChineseString(0,6,0,check_connection_chinese,8);
				delay_ms(2000);
				OLED_Clear();
				OLED_ShowChineseString(0,0,0,temp_and_hum_sensor_chinese,5);
				OLED_ShowChar(80,0,'|',16);
				OLED_ShowChineseString(0,2,0,audio_out_chinese,4);
				OLED_ShowChar(80,2,'|',16);
				OLED_ShowChineseString(0,4,0,bluetooth_chinese,2);
				OLED_ShowChar(80,4,'|',16);
				OLED_ShowChineseString(0,6,0,smog_sensor_chinese,4);
				OLED_ShowChar(80,6,'|',16);
			}
			else{
				dht11_enabled = dht11_temp;
				mp3_enabled = mp3_temp;
				hc08_enabled = hc08_temp;
				mq2_enabled = mq2_temp;
				if(!mp3_enabled) stopmusic();
				OLED_DrawBMP(0, 0, 128, 4, success_icon);
				OLED_ShowChineseString(24,4,0,saved_chinese,5);
				delay_ms(2000);
				OLED_Clear();
				break;
			}
		}
	}
	//HC08, MQ-2, DHT11, JQ8900
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
	uchar date_and_time_chinese[] = {21,22,23,24,25}, notification_chinese[] = {26,27,28,29,30}, module_chinese[] = {31,32,87,88,68}, about_chinese[] = {34,35,19,37};
	uchar selection = 0;
	OLED_Clear();
	while(1){
		OLED_ShowChineseString(24,0,0,date_and_time_chinese,5);
		OLED_ShowChineseString(24,2,0,notification_chinese,5);
		OLED_ShowChineseString(24,4,0,module_chinese,5);
		OLED_ShowChineseString(32,6,0,about_chinese,4);
//		OLED_ShowString(24,0,"Date&Time",16);
//		OLED_ShowString(24,2,"Notification",16);
//		OLED_ShowString(24,4,"Module",16);
//		OLED_ShowString(24,6,"About",16);
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
				case 2: {page_mod_switch(); break;}
				case 3: {page_about(); break;}
				default: break;
			}
		}
	}
}