#include "mp3.h"
#include "oled.h"
#include "keyscan.h"
#include "delay.h"
#include "ds1302.h"

char vol = 15;

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
	char vol_init = vol, title[] = "Music1", vol_to_show[] = "Volume:  ";
	OLED_Clear();
	set_volume(vol);
	playmusic(1);
	while(1){
		OLED_ShowString(32,0,title,16);
		vol_to_show[8] = (char)(vol / 5 + '0');
		OLED_ShowString(20,2,vol_to_show,16);
		if(getKey() == 1){
			set_volume(vol+5);
			vol_to_show[8] = (char)(vol / 5 + '0');
		}
		else if(getKey() == 2){
			set_volume(vol-5);
			vol_to_show[8] = (char)(vol / 5 + '0');
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
			title[5] = (char)(selection + '0');
			playmusic(selection);
		}
	}
}
