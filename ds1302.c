#include <reg52.h>
#include <intrins.h>
#include "ds1302.h"
#include "oled.h"
#include "delay.h"
#include "lunar.h"

uchar dis_time_buf[16]={0};

//��ʼʱ�䶨��
uchar time_buf[8] = {0x20,0x24,0x05,0x14,0x17,0x59,0x38,0x02};//��ʼʱ��2024��5��14��17��59��38�� ���ڶ�

//DS1302��ʼ������
void ds1302_init(void) 
{
	RST=0;			//RST���õ�
	SCK=0;			//SCK���õ�
}
//��DS1302д��һ�ֽ�����
void ds1302_write_byte(uchar addr, uchar d) 
{
	uchar i;
	RST=1;					//����DS1302����	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;   //���λ���㣬�Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			IO=1;
			}
		else {
			IO=0;
			}
		SCK=1;      //����ʱ��
		SCK=0;
		addr = addr >> 1;
		}	
	//д�����ݣ�d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			IO=1;
			}
		else {
			IO=0;
			}
		SCK=1;    //����ʱ��
		SCK=0;
		d = d >> 1;
		}
	RST=0;		//ֹͣDS1302����
}

//��DS1302����һ�ֽ�����
uchar ds1302_read_byte(uchar addr) {

	uchar i,temp;	
	RST=1;					//����DS1302����
	//д��Ŀ���ַ��addr
	addr = addr | 0x01;    //���λ�øߣ��Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			IO=1;
			}
		else {
			IO=0;
			}
		SCK=1;
		SCK=0;
		addr = addr >> 1;
		}	
	//������ݣ�temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (IO) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}
		SCK=1;
		SCK=0;
		}	
	RST=0;					//ֹͣDS1302����
	return temp;
}
//��DS1302д��ʱ������
void ds1302_write_time(void)
{
	ds1302_write_byte(ds1302_control_add,0x00);			//�ر�д���� 
	ds1302_write_byte(ds1302_sec_add,0x80);				//��ͣʱ�� 
	//ds1302_write_byte(ds1302_charger_add,0xa9);	    //������ 
	ds1302_write_byte(ds1302_year_add,time_buf[1]);		//�� 
	ds1302_write_byte(ds1302_month_add,time_buf[2]);	//�� 
	ds1302_write_byte(ds1302_date_add,time_buf[3]);		//�� 
	ds1302_write_byte(ds1302_hr_add,time_buf[4]);		//ʱ 
	ds1302_write_byte(ds1302_min_add,time_buf[5]);		//��
	ds1302_write_byte(ds1302_sec_add,time_buf[6]);		//��
	ds1302_write_byte(ds1302_day_add,time_buf[7]);		//�� 
	ds1302_write_byte(ds1302_control_add,0x80);			//��д����     
}
//��DS1302����ʱ������
void ds1302_read_time(void)  
{
	time_buf[1]=ds1302_read_byte(ds1302_year_add);		//�� 
	time_buf[2]=ds1302_read_byte(ds1302_month_add);		//�� 
	time_buf[3]=ds1302_read_byte(ds1302_date_add);		//�� 
	time_buf[4]=ds1302_read_byte(ds1302_hr_add);		//ʱ 
	time_buf[5]=ds1302_read_byte(ds1302_min_add);		//�� 
	time_buf[6]=(ds1302_read_byte(ds1302_sec_add))&0x7f;//�룬������ĵ�7λ�����ⳬ��59
	time_buf[7]=ds1302_read_byte(ds1302_day_add);		//�� 	
}

void print_time_now(uchar x, uchar y, uchar hour_type){
	uchar hour;
	char time_to_show[9], am_or_pm[3];
	read_discrete_time();
	if(hour_type == 12){
		hour = get_integer_hour();
		if(hour >= 12){
			am_or_pm[0] = 'P';
			am_or_pm[1] = 'M';
		}
		else{
			am_or_pm[0] = 'A';
			am_or_pm[1] = 'M';
		}
		if(hour > 12) hour -= 12;
		dis_time_buf[8] = hour / 10;
		dis_time_buf[9] = hour % 10;
	}
	else{
		am_or_pm[0] = ' ';
		am_or_pm[1] = ' ';
	}
	if(dis_time_buf[8] == 0) time_to_show[0] = ' ';
	else time_to_show[0] = Char(dis_time_buf[8]);
	time_to_show[1] = Char(dis_time_buf[9]);
	time_to_show[2] = ':';
	time_to_show[3] = Char(dis_time_buf[10]);
	time_to_show[4] = Char(dis_time_buf[11]);
	time_to_show[5] = ':';
	time_to_show[6] = Char(dis_time_buf[12]);
	time_to_show[7] = Char(dis_time_buf[13]);
	time_to_show[8] = 0;
	am_or_pm[2] = 0;
	OLED_ShowString(x,y+1,am_or_pm,16);
	OLED_ShowString(x+16,y,time_to_show,24);
}

void print_date_now(uchar x, uchar y){
	char date_to_show[11];
	uchar weekday_char[2] = {16, 15};
	read_discrete_time();
	date_to_show[0] = Char(dis_time_buf[0]);
	date_to_show[1] = Char(dis_time_buf[1]);
	date_to_show[2] = Char(dis_time_buf[2]);
	date_to_show[3] = Char(dis_time_buf[3]);
	date_to_show[4] = '/';
	date_to_show[5] = Char(dis_time_buf[4]);
	date_to_show[6] = Char(dis_time_buf[5]);
	date_to_show[7] = '/';
	date_to_show[8] = Char(dis_time_buf[6]);
	date_to_show[9] = Char(dis_time_buf[7]);
	date_to_show[10] = 0;
	OLED_ShowString(x,y,date_to_show,16);
	if(dis_time_buf[14] == 7) weekday_char[1] = 17;
	else weekday_char[1] = dis_time_buf[14];
	OLED_ShowChineseString(x+90,y,1,weekday_char,2);
}

void print_date_any_time(uchar x, uchar y, uint year, uchar month, uchar day){
	char date_to_show[11];
	uchar weekday_char[2] = {16, 15};
	quadruple_digit_to_string(year, date_to_show);
	date_to_show[4] = '/';
	double_digit_to_string(month, date_to_show+5);
	date_to_show[7] = '/';
	double_digit_to_string(day, date_to_show+8);
	date_to_show[10] = 0;
	OLED_ShowString(x,y,date_to_show,16);
	if(calculate_week_day(year, month, day) == 7) weekday_char[1] = 17;
	else weekday_char[1] = calculate_week_day(year, month, day);
	OLED_ShowChineseString(x+90,y,1,weekday_char,2);
}

void print_lunar(uchar x,uchar y,uchar month,uchar day){
	uchar lunar_date_char[4] = {0, 15, 13, 1};
	if(month>1) lunar_date_char[0] = month;
	else lunar_date_char[0] = 0;
	if(day<=10){
		lunar_date_char[2] = 13;
		lunar_date_char[3] = day;
	}
	else if(day<20){
		lunar_date_char[2] = 10;
		lunar_date_char[3] = day%10;
	}
	else if(day == 20){
		lunar_date_char[2] = 2;
		lunar_date_char[3] = 10;;
	}
	else if(day<30){
		lunar_date_char[2] = 14;
		lunar_date_char[3] = day%10;
	}
	else{
		lunar_date_char[2] = 3;
		lunar_date_char[3] = 10;
	}
	OLED_ShowChineseString(x,y,1,lunar_date_char,4);
}

void print_lunar_and_term_now(uchar x,uchar y){
	print_lunar_and_term_any_time(x, y, get_integer_year(), get_integer_month(), get_integer_day());
}

void print_lunar_and_term_any_time(uchar x,uchar y, uint year, uchar month, uchar day){
	uchar lunar_date[2], solar_term_char[2] = {0, 1}, num = get_solar_term_num(year, month, day);
	get_lunar_calendar(year, month, day, lunar_date);
	print_lunar(x+40, y, lunar_date[0], lunar_date[1]);
	
	solar_term_char[0] = num;
	solar_term_char[1] = num+1;
	OLED_ShowChineseString(x,y,2,solar_term_char,2);
}

void read_discrete_time()
{
	ds1302_read_time();  //��ȡʱ�� 
	dis_time_buf[0]=(time_buf[0]>>4); //��   
	dis_time_buf[1]=(time_buf[0]&0x0f);
   
	dis_time_buf[2]=(time_buf[1]>>4);   
	dis_time_buf[3]=(time_buf[1]&0x0f);

	dis_time_buf[4]=(time_buf[2]>>4); //��  
	dis_time_buf[5]=(time_buf[2]&0x0f);
   
	dis_time_buf[6]=(time_buf[3]>>4); //��   
	dis_time_buf[7]=(time_buf[3]&0x0f);
   
	dis_time_buf[14]=(time_buf[7]&0x07); //����
	 
	dis_time_buf[8]=(time_buf[4]>>4); //ʱ   
	dis_time_buf[9]=(time_buf[4]&0x0f);

	dis_time_buf[10]=(time_buf[5]>>4); //��   
	dis_time_buf[11]=(time_buf[5]&0x0f);   

	dis_time_buf[12]=(time_buf[6]>>4); //��   
	dis_time_buf[13]=(time_buf[6]&0x0f);
}
 
char isLeapYear(uint year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return 1;
    } else {
        return 0;
    }
}

uint get_integer_year(){
	read_discrete_time();
	return dis_time_buf[0]*1000+dis_time_buf[1]*100+dis_time_buf[2]*10+dis_time_buf[3];
}

uchar get_integer_month(){
	read_discrete_time();
	return dis_time_buf[4]*10+dis_time_buf[5];
}

uchar get_integer_day(){
	read_discrete_time();
	return dis_time_buf[6]*10+dis_time_buf[7];
}

uchar get_integer_hour(){
	read_discrete_time();
	return dis_time_buf[8]*10+dis_time_buf[9];
}

uchar get_integer_minute(){
	read_discrete_time();
	return dis_time_buf[10]*10+dis_time_buf[11];
}

uchar get_integer_second(){
	read_discrete_time();
	return dis_time_buf[12]*10+dis_time_buf[13];
}

uchar get_integer_weekday(){
	read_discrete_time();
	return dis_time_buf[14];
}

uchar calculate_week_day(uint year, uchar month, uchar day){
	uchar iWeek;
	if(month==1||month==2) {
		month+=12;
		year--;
	}
	iWeek = (day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7 + 1;
	return iWeek;
}

void write_date_and_time(uint year, uchar month, uchar day, uchar hour, uchar minute, uchar second){
	time_buf[0] = year / 1000 * 16 + (year / 100) % 10;
	time_buf[1] = ((year / 10) % 10) * 16 + year % 10;
	time_buf[2] = (month / 10) * 16 + month % 10;
	time_buf[3] = (day / 10) * 16 + day % 10;
	time_buf[4] = (hour / 10) * 16 + hour % 10;
	time_buf[5] = (minute / 10) * 16 + minute % 10;
	time_buf[6] = (second / 10) * 16 + second % 10;
	time_buf[7] = calculate_week_day(year, month, day);
	ds1302_write_time();
}

void write_date(uint year, uchar month, uchar day){
	ds1302_read_time();
	time_buf[0] = year / 1000 * 16 + (year / 100) % 10;
	time_buf[1] = ((year / 10) % 10) * 16 + year % 10;
	time_buf[2] = (month / 10) * 16 + month % 10;
	time_buf[3] = (day / 10) * 16 + day % 10;
	time_buf[7] = calculate_week_day(year, month, day);
	ds1302_write_time();
}

void write_time(uchar hour, uchar minute, uchar second){
	ds1302_read_time();
	time_buf[4] = (hour / 10) * 16 + hour % 10;
	time_buf[5] = (minute / 10) * 16 + minute % 10;
	time_buf[6] = (second / 10) * 16 + second % 10;
	ds1302_write_time();
}
