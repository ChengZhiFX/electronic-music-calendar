#include "mp3.h"
#include "oled.h"
#include "keyscan.h"
#include "delay.h"
#include "ds1302.h"

char vol = 15;

void Send ( uchar addr )//���ͺ�����
{
    uchar i;

    EA = 0;/*����ʱ�ص��жϣ���ֹ�ж�Ӱ��ʱ��  */

    sda = 1;/*��ʼ����  */
    delay_us ( 1000 );
    sda = 0;/*��ʼ������*/
    delay_us ( 2200 );/*�˴���ʱ����Ҫ����2ms���˲�����ʱΪ310ms  */

    for ( i = 0; i < 8; i++ ) /*�ܹ�8λ����  */
    {
        sda = 1;
        if ( addr & 0x01 ) /*3:1��ʾ����λ1,ÿ��λ�����������ʾ  */
        {
            delay_us ( 500 );
            sda = 0;
            delay_us ( 210 );
        }
        else              /*1��3��ʾ����λ0 ,ÿ��λ�����������ʾ  */
        {
            delay_us ( 210 );
            sda = 0;
            delay_us ( 500 );
        }
        addr >>= 1;
    }
    sda = 1;
    EA = 1;//�ָ��ж�
}
void playmusic(uchar num)
{	
	uchar num_h,num_l;
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

char get_volume(){
	return vol;
}

void set_volume(char vol_temp){
	char vol_H, vol_L;
	if(vol_temp > 30) vol = 30;
	else if(vol_temp < 0) vol = 0;
	else vol = vol_temp;
	vol_H = vol / 10;
	vol_L = vol % 10;
	Send(0x0a);
	Send(vol_H);
	Send(vol_L);
	Send(0x0c);
}

void page_music(){
	uchar selection = 1;
	char title_chinese[] = {2,3,66,67,68}, vol_chinese[] = {2,10};
	char vol_init = vol, music_playing[] = "Music1", vol_to_show[] = ":  ";
	OLED_Clear();
	set_volume(vol);
	playmusic(1);
	while(1){
		OLED_ShowChineseString(24,0,0,title_chinese,5);
		OLED_ShowString(32,2,music_playing,16);
		vol_to_show[2] = Char(vol / 5);
		OLED_ShowChineseString(32,4,0,vol_chinese,2);
		OLED_ShowString(64,4,vol_to_show,16);
		if(getKey() == 1){
			set_volume(vol+5);
			vol_to_show[2] = Char(vol / 5);
		}
		else if(getKey() == 2){
			set_volume(vol-5);
			vol_to_show[2] = Char(vol / 5);
		}
		else if(getKey() == 3){
			stopmusic();
			vol = vol_init;
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
