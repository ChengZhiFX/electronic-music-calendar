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
//DS1302��ַ����
#define ds1302_sec_add			0x80		//�����ݵ�ַ
#define ds1302_min_add			0x82		//�����ݵ�ַ
#define ds1302_hr_add			0x84		//ʱ���ݵ�ַ
#define ds1302_date_add			0x86		//�����ݵ�ַ
#define ds1302_month_add		0x88		//�����ݵ�ַ
#define ds1302_day_add			0x8a		//�������ݵ�ַ
#define ds1302_year_add			0x8c		//�����ݵ�ַ
#define ds1302_control_add		0x8e		//�������ݵ�ַ
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

//STC89C52Ϊ1T��Ƭ��,��1��ʱ��/��������,�ٶ�ΪAT89C52��12��
//DS1302��ʼ������
void ds1302_init(void);

//��DS1302д��һ�ֽ�����
void ds1302_write_byte(uchar addr, uchar d);

//��DS1302����һ�ֽ�����
uchar ds1302_read_byte(uchar addr);

//��DS1302д��ʱ������
void ds1302_write_time(void);

//��DS1302����ʱ������
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
