#include "delay.h"
#include "mq2.h"
#include "mp3.h"
#include "oled.h"
#include "keyscan.h"
#include "hc08.h"
#include "bmp.h"

uchar mq2_enabled = 1;
uchar smog_alarm_suspend = 0;

uchar isSmoking(){
	if(!mq2_enabled) return 0;
	if(mq2 == 0){
		delay_for_mq2();
		if(mq2 == 0) return 1;
	}
	return 0;
}

void page_smog_alarm(){
	char smog_detected_chinese[] = {38,39,40,41,42,43,46,47}, press_key_cancel_chinese[] = {58,59,60,61,62,63};
	OLED_Clear();
	OLED_DrawBMP(0, 0, 128, 4, fire_icon);
	OLED_ShowChineseString(0,4,0,smog_detected_chinese,8);
	OLED_ShowChineseString(16,6,0,press_key_cancel_chinese,6);
//	OLED_ShowString(0,2,"Smog Detected!",16);
	send_volume(get_alert_volume());
	sendData("FIRE");
	set_single_loop(1);
	playmusic(19);
	while(1){
		if(getKey() || !isSmoking()){
			stopmusic();
			set_single_loop(0);
			send_volume(get_notification_volume());
			smog_alarm_suspend = 1;
			OLED_Clear();
			break;
		}
	}
}

void mq2_tick_tock(){
	if(!mq2_enabled) return;
	if(isSmoking()){
		if(!smog_alarm_suspend) page_smog_alarm();
	}
	else{
		if(smog_alarm_suspend) smog_alarm_suspend = 0;
	}
}
