#include "delay.h"
#include "mq2.h"
#include "mp3.h"
#include "oled.h"
#include "keyscan.h"
#include "hc08.h"

uchar smog_alarm_enable = 1;
uchar smog_alarm_suspend = 0;

uchar isSmoking(){
	if(mq2 == 0){
		delay_for_mq2();
		if(mq2 == 0) return 1;
	}
	return 0;
}

void page_smog_alarm(){
	char vol_init;
	OLED_Clear();
	OLED_ShowString(0,2,"Smog Detected!",16);
	vol_init = get_volume();
	set_volume(30);
	sendData("FIRE");
	set_single_loop(1);
	playmusic(19);
	while(1){
		if(getKey() || !isSmoking()){
			stopmusic();
			set_single_loop(0);
			set_volume(vol_init);
			smog_alarm_suspend = 1;
			OLED_Clear();
			break;
		}
	}
}

void mq2_tick_tock(){
	if(smog_alarm_enable){
		if(isSmoking()){
			if(!smog_alarm_suspend) page_smog_alarm();
		}
		else{
			if(smog_alarm_suspend) smog_alarm_suspend = 0;
		}
	}
}
