#include "reg52.h"
#include "oled.h"
//#include "bmp.h"
#include "dht11.h"
#include "ds1302.h"
#include "keyscan.h"
#include "mp3.h"
#include "alarm.h"
#include "settings.h"
#include "delay.h"
#include "lunar.h"
#include "mq2.h"

Sensor sensor;
Alarm alarm;

unsigned char str[] = {0,1,2,3,4};
char dht11_to_show[8];
uchar hour_type = 24;

void function1(){
	if(hour_type == 12) hour_type = 24;
	else if(hour_type == 24) hour_type = 12;
}

void function2(){
	uchar selection = 0;
	OLED_Clear();
	while(1){
		OLED_ShowString(22,0,"Alarm",16);
		OLED_ShowString(22,2,"Music",16);
		OLED_ShowString(22,4,"Weather",16);
		OLED_ShowString(22,6,"Settings",16);
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
				case 0: {page_alarm(); break;}
				case 1: {page_music(); break;}
				case 2: {/*page_weather();*/ break;}
				case 3: {page_settings(); break;}
				default: break;
			}
		}
	}
}

void function3(){
	
}

void function4(){
	
}

/*******************************Main*******************************/
void main(){
	Delay_50ms(1);//等待系统稳定
	ds1302_init();  //DS1302初始化
	OLED_Init();
	OLED_Display_On();
	OLED_Clear();
	OLED_Clear();
	alarm_init();
	while(1){
		print_time_now(20,2,hour_type);
		print_date_now(0,4);
		print_lunar_now(0,0);
		print_solar_term_now(80,0);
		getdata((uchar *)&sensor);
		dht11_to_show[0] = (char)(sensor.temper / 10 + '0');
		dht11_to_show[1] = (char)(sensor.temper % 10 + '0');
		dht11_to_show[2] = 'C';
		dht11_to_show[3] = ' ';
		dht11_to_show[4] = (char)(sensor.humid / 10 + '0');
		dht11_to_show[5] = (char)(sensor.humid % 10 + '0');
		dht11_to_show[6] = '%';
		dht11_to_show[7] = 0;
		OLED_ShowString(20,6, dht11_to_show,16);
		switch(getKey()){
			case 1: {function1(); break;}
			case 2: {function2(); break;}
			case 3: {function3(); break;}
			case 4: {function4(); break;}
			default: break;
		}
		alarm_tick_tock();
		mq2_tick_tock();
	}
}