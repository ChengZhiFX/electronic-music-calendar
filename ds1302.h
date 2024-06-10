#ifndef _DS1302_H_
#define _DS1302_H_

#include <reg52.h>
#include <intrins.h>
#include "oled.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint  unsigned int
#endif

sbit RST=P3^6;
sbit IO=P3^5;
sbit SCK=P3^4;
//DS1302地址定义
#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

//STC89C52为1T单片机,即1个时钟/机器周期,速度为AT89C52的12倍
//DS1302初始化函数
void ds1302_init(void);

//向DS1302写入一字节数据
void ds1302_write_byte(uchar addr, uchar d);

//从DS1302读出一字节数据
uchar ds1302_read_byte(uchar addr);

//向DS1302写入时钟数据
void ds1302_write_time(void);

//从DS1302读出时钟数据
void ds1302_read_time(void);

void print_time_now(uchar x, uchar y, uchar hour_type);
void print_date_any_time(uchar x, uchar y, uint year, uchar month, uchar day);
void print_date_now(uchar x, uchar y);
void print_lunar_and_term_now(uchar x,uchar y);
void print_lunar_and_term_any_time(uchar x,uchar y, uint year, uchar month, uchar day);
void read_discrete_time();

char isLeapYear(uint year);
uint get_integer_year();
uchar get_integer_month();
uchar get_integer_day();
uchar get_integer_hour();
uchar get_integer_minute();
uchar get_integer_second();
uchar get_integer_weekday();
uchar calculate_week_day(uint year, uchar month, uchar day);
void write_date_and_time(uint year, uchar month, uchar day, uchar hour, uchar minute, uchar second);
void write_date(uint year, uchar month, uchar day);
void write_time(uchar hour, uchar minute, uchar second);

#endif
