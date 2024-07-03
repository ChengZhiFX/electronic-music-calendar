#include "weather.h"
#include "oled.h"
#include "keyscan.h"
#include "delay.h"
#include "ds1302.h"

Weather weather[4];

void weather_init(){
	uchar i;
	for(i=0;i<4;i++){
		weather[i].month = 0;
		weather[i].day = 0;
		weather[i].dayWeather = 0;
		weather[i].dayTemp = 0;
		weather[i].nightWeather = 0;
		weather[i].nightTemp = 0;
	}
}

void set_weather_from_str(uchar num, char weather_str[13]){
	weather[num].month = char_to_double_digit(weather_str[0], weather_str[1]);
	weather[num].day = char_to_double_digit(weather_str[2], weather_str[3]);
	weather[num].dayWeather = char_to_double_digit(weather_str[4], weather_str[5]);
	weather[num].dayTemp = char_to_double_digit(weather_str[6], weather_str[7]);
	weather[num].nightWeather = char_to_double_digit(weather_str[8], weather_str[9]);
	weather[num].nightTemp = char_to_double_digit(weather_str[10], weather_str[11]);
}

void page_weather_error(){
	char connect_ble_chinese[] = {43,48,49,50,51}, waiting_chinese[] = {52,53,54,55,4,5,56,57}, press_key_cancel_chinese[] = {58,59,60,61,62,63};
	OLED_Clear();
	OLED_ShowChineseString(24,0,0,connect_ble_chinese,5);
	OLED_ShowChineseString(0,2,0,waiting_chinese,8);
	OLED_ShowChineseString(16,6,0,press_key_cancel_chinese,6);
//	OLED_ShowString(0,0,"Waiting for App",16);
//	OLED_ShowString(0,4,"Press any key",16);
//	OLED_ShowString(0,6,"to cancel",16);
	while(1){
		if(weather[0].month != 0 && weather[0].day != 0) break;
		else if(getKey()){
			OLED_Clear();
			break;
		}
	}
}

void print_weather_all(uchar page){
	char day_chinese[] = {64,4}, night_chinese[] = {65,25};
	uchar dayWeather_str[] = {0,1}, nightWeather_str[] = {0,1};
	char date_to_show[] = "--/--", dayTemp_to_show[] = "--C", nightTemp_to_show[] = "--C";
	date_to_show[0] = Char(weather[page-1].month / 10);
	date_to_show[1] = Char(weather[page-1].month % 10);
	date_to_show[3] = Char(weather[page-1].day / 10);
	date_to_show[4] = Char(weather[page-1].day % 10);
	dayWeather_str[0] = weather[page-1].dayWeather*2;
	dayWeather_str[1] = weather[page-1].dayWeather*2 + 1;
	nightWeather_str[0] = weather[page-1].nightWeather*2;
	nightWeather_str[1] = weather[page-1].nightWeather*2 + 1;
	dayTemp_to_show[0] = Char(weather[page-1].dayTemp / 10);
	dayTemp_to_show[1] = Char(weather[page-1].dayTemp % 10);
	nightTemp_to_show[0] = Char(weather[page-1].nightTemp / 10);
	nightTemp_to_show[1] = Char(weather[page-1].nightTemp % 10);
	OLED_ShowString(32,0,date_to_show,16);
	OLED_ShowChineseString(0,2,0,day_chinese,2);
	OLED_ShowString(32,2,":",16);
	OLED_ShowChineseString(48,2,3,dayWeather_str,2);
	OLED_ShowString(80,2,dayTemp_to_show,16);
	OLED_ShowChineseString(0,4,0,night_chinese,2);
	OLED_ShowString(32,4,":",16);
	OLED_ShowChineseString(48,4,3,nightWeather_str,2);
	OLED_ShowString(80,4,nightTemp_to_show,16);
}

//void print_weather_base_now(uchar x, uchar y){
//	uchar i, weather_str[2] = {0,0};
//	for(i=0;i<4;i++){
//		if(get_integer_month() == weather[i].month && get_integer_day() == weather[i].day){
//			if(get_integer_hour() < 6){
//				if(i-1>=0 && i-1<4){
//					weather_str[0] = weather[i-1].nightWeather*2;
//					weather_str[1] = weather[i-1].nightWeather*2 + 1;
//				}
//			}
//			else if(get_integer_hour() >= 18){
//				weather_str[0] = weather[i].nightWeather*2;
//				weather_str[1] = weather[i].nightWeather*2 + 1;
//			}
//			else{
//				weather_str[0] = weather[i].dayWeather*2;
//				weather_str[1] = weather[i].dayWeather*2 + 1;
//			}
//			OLED_ShowChineseString(x,y,3,weather_str,2);
//		}
//	}
//}

void page_weather_all(){
	uchar page = 1;
	if(weather[0].month == 0 && weather[0].day == 0){
		page_weather_error();
		if(weather[0].month == 0 && weather[0].day == 0) return;
	}
	OLED_Clear();
	while(1){
		print_weather_all(page);
		if(getKey() == 1){
			page--;
			if(page < 1) page = 1;
		}
		else if(getKey() == 2){
			page++;
			if(page > 4) page = 4;
		}
		else if(getKey() == 3){
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			OLED_Clear();
			break;
		}
	}
}
