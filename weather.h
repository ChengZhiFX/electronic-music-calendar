#ifndef __WEATHER_H_
#define __WEATHER_H_

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

typedef struct weather{
	uchar month;
	uchar day;
	uchar dayWeather;
	uchar dayTemp;
	uchar nightWeather;
	uchar nightTemp;
}Weather;

void weather_init();
void set_weather_from_str(uchar num, char weather_str[13]);
void print_weather_base_now(uchar x, uchar y);
void page_weather_all();

#endif