#include "mp3.h"
#include "oled.h"
#include "keyscan.h"
#include "delay.h"
#include "ds1302.h"

uchar mp3_enabled = 1;
char vol_notification = 20, vol_media = 15, vol_alert = 30; //"vol" equals to "vol_notification"

void Send ( uchar addr )//发送函数。
{
    uchar i;

    EA = 0;/*发送时关掉中断，防止中断影响时序  */

    sda = 1;/*开始拉高  */
    delay_us ( 1000 );
    sda = 0;/*开始引导码*/
    delay_us ( 2200 );/*此处延时最少要大于2ms，此参数延时为310ms  */

    for ( i = 0; i < 8; i++ ) /*总共8位数据  */
    {
        sda = 1;
        if ( addr & 0x01 ) /*3:1表示数据位1,每个位用两个脉冲表示  */
        {
            delay_us ( 500 );
            sda = 0;
            delay_us ( 210 );
        }
        else              /*1：3表示数据位0 ,每个位用两个脉冲表示  */
        {
            delay_us ( 210 );
            sda = 0;
            delay_us ( 500 );
        }
        addr >>= 1;
    }
    sda = 1;
    EA = 1;//恢复中断
}

void playmusic(uchar num){
	uchar num_h,num_l;
	if(!mp3_enabled) return;
	if(num >= 10){
		num_h = num / 10;
		num_l = num % 10;
		Send(0x0a);
		Send(num_h);
		Send(num_l);
		Send(0x0b);
	}
	else {
		Send(0x0a);
		Send(num);
		Send(0x0b);
	}
}

void stopmusic(){
	Send(0x13);
}

void set_single_loop(uchar is_loop){
	if(is_loop){
		Send(0x0a);
		Send(0x01);
		Send(0x0e);
	}
	else{
		Send(0x0a);
		Send(0x02);
		Send(0x0e);
	}
}

void send_volume(uchar vol_type){
	char vol_H, vol_L, vol;
	if(vol_type == 1) vol = vol_notification;
	else if(vol_type == 2) vol = vol_media;
	else if(vol_type == 3) vol = vol_alert;
	vol_H = vol / 10;
	vol_L = vol % 10;
	Send(0x0a);
	Send(vol_H);
	Send(vol_L);
	Send(0x0c);
}

void mp3_init(){
	set_notification_volume(20);
	send_volume(1);
	playmusic(20);
}

char get_notification_volume(){
	return vol_notification;
}

char get_media_volume(){
	return vol_media;
}

char get_alert_volume(){
	return vol_alert;
}

void set_notification_volume(char vol_temp){
	if(vol_temp > 30) vol_notification = 30;
	else if(vol_temp < 0) vol_notification = 0;
	else vol_notification = vol_temp;
}

void set_media_volume(char vol_temp){
	if(vol_temp > 30) vol_media = 30;
	else if(vol_temp < 0) vol_media = 0;
	else vol_media = vol_temp;
}

void set_alert_volume(char vol_temp){
	if(vol_temp > 30) vol_alert = 30;
	else if(vol_temp < 0) vol_alert = 0;
	else vol_alert = vol_temp;
}

void page_music(){
	uchar selection = 1;
	char title_chinese[] = {2,3,66,67,68}, media_vol_chinese[] = {108,109,2,10}, cancel_chinese[] = {62,63}, next_chinese[] = {106,77,107};
	char music_playing[] = "Music1", vol_to_show[] = ":  ";
	OLED_Clear();
	send_volume(2);
	playmusic(1);
	OLED_ShowChineseString(24,0,0,title_chinese,5);
	OLED_ShowChineseString(20,4,0,media_vol_chinese,4);
	OLED_ShowChineseString(0,6,0,cancel_chinese,2);
	OLED_ShowChineseString(40,6,0,next_chinese,3);
	while(1){
		OLED_ShowString(40,2,music_playing,16);
		vol_to_show[2] = Char(get_media_volume() / 5);
		OLED_ShowString(84,4,vol_to_show,16);
		if(getKey() == 1){
			set_media_volume(get_media_volume() + 5);
			send_volume(2);
			vol_to_show[2] = Char(get_media_volume() / 5);
		}
		else if(getKey() == 2){
			set_media_volume(get_media_volume() - 5);
			send_volume(2);
			vol_to_show[2] = Char(get_media_volume() / 5);
		}
		else if(getKey() == 3){
			stopmusic();
			send_volume(1);
			OLED_Clear();
			break;
		}
		else if(getKey() == 4){
			stopmusic();
			selection++;
			if(selection >= 10) selection = 1;
			music_playing[5] = Char(selection);
			playmusic(selection);
		}
	}
}
