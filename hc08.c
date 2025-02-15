#include<reg52.h>
#include<intrins.h>
#include <string.h>
#include "hc08.h"
#include "delay.h"
#include "oled.h"
#include "ds1302.h"
#include "alarm.h"
#include "mp3.h"
#include "weather.h"
#include "event.h"
#include "keyscan.h"

uchar hc08_enabled = 1;
uchar receive_data, flag=0, index=0;
uchar rcv_buffer[20];

void switch_hour_type();

void set_date_via_bt(){
	if(receive_data!='&'){
		if(receive_data=='?'){
			write_date(char_to_quadruple_digit(rcv_buffer[0],rcv_buffer[1],rcv_buffer[2],rcv_buffer[3]), char_to_double_digit(rcv_buffer[4],rcv_buffer[5]), char_to_double_digit(rcv_buffer[6],rcv_buffer[7]));
			flag = 0;
			index = 0;
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void set_time_via_bt(){
	if(receive_data!='@'){
		if(receive_data=='?'){
			write_time(char_to_double_digit(rcv_buffer[0],rcv_buffer[1]), char_to_double_digit(rcv_buffer[2],rcv_buffer[3]), get_integer_second());
			flag = 0;
			index = 0;
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void sync_time_via_bt(){
	if(receive_data!='$'){
		if(receive_data=='?'){
			write_date(char_to_quadruple_digit(rcv_buffer[0],rcv_buffer[1],rcv_buffer[2],rcv_buffer[3]), char_to_double_digit(rcv_buffer[4],rcv_buffer[5]), char_to_double_digit(rcv_buffer[6],rcv_buffer[7]));
			write_time(char_to_double_digit(rcv_buffer[8],rcv_buffer[9]), char_to_double_digit(rcv_buffer[10],rcv_buffer[11]), char_to_double_digit(rcv_buffer[12],rcv_buffer[13]));
			flag = 0;
			index = 0;
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void set_alarm_via_bt(){
	if(receive_data!='%'){
		if(receive_data=='?'){
			set_alarm(char_to_double_digit(rcv_buffer[0],rcv_buffer[1]), char_to_double_digit(rcv_buffer[2],rcv_buffer[3]), Tinyint(rcv_buffer[4]), Tinyint(rcv_buffer[5]));
			flag = 0;
			index = 0;
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void set_volume_via_bt(){
	if(receive_data!='#'){
		if(receive_data=='?'){
			if(rcv_buffer[0] == '1'){
				set_notification_volume(5*Tinyint(rcv_buffer[1]));
				playmusic(10,1);
			}
			else if(rcv_buffer[0] == '2'){
				set_media_volume(5*Tinyint(rcv_buffer[1]));
				send_volume(2);
			}
			else if(rcv_buffer[0] == '3'){
				set_alert_volume(5*Tinyint(rcv_buffer[1]));
				playmusic(19,3);
			}
			flag = 0;
			index = 0;
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void rcv_weather_all_via_bt(){
	static uchar step=0;
	char weather_str[13];
	if(receive_data!='^'){
		if(receive_data=='?'){
			strncpy(weather_str, rcv_buffer, 12);
			weather_str[12] = 0;
			set_weather_from_str(step, weather_str);
			flag = 0;
			index = 0;
			step = 0;
			return;
		}
		else if(receive_data=='.'){
			strncpy(weather_str, rcv_buffer, 12);
			weather_str[12] = 0;
			set_weather_from_str(step, weather_str);
			flag = 0;
			index = 0;
			step++;
			sendData("WeatherReady");
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void set_event_via_bt(){
	if(receive_data!='`'){
		if(receive_data=='}'){
			rcv_buffer[index-1] = 0;
			set_event_content(rcv_buffer);
			flag = 0;
			index = 0;
			return;
		}
		else if(receive_data=='{'){
			set_event_control(char_to_quadruple_digit(rcv_buffer[0],rcv_buffer[1],rcv_buffer[2],rcv_buffer[3]), char_to_double_digit(rcv_buffer[4],rcv_buffer[5]), char_to_double_digit(rcv_buffer[6],rcv_buffer[7]), Tinyint(rcv_buffer[8]));
			flag = 0;
			index = 0;
			sendData("EventReady");
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void play_music_via_bt(){
	if(receive_data!='*'){
		if(receive_data=='?'){
			if(rcv_buffer[1] == '1'){
				playmusic(Tinyint(rcv_buffer[0]),2);
			}
			else if(rcv_buffer[1] == '0'){
				stopmusic();
			}
			flag = 0;
			index = 0;
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

void switch_mod_via_bt(){
	extern uchar dht11_enabled, mp3_enabled, mq2_enabled;
	if(receive_data!='~'){
		if(receive_data=='?'){
			if(rcv_buffer[0] == '1') dht11_enabled = Tinyint(rcv_buffer[1]);
			else if(rcv_buffer[0] == '2') mp3_enabled = Tinyint(rcv_buffer[1]);
			else if(rcv_buffer[0] == '4') mq2_enabled = Tinyint(rcv_buffer[1]);
			if(!mp3_enabled) stopmusic();
			flag = 0;
			index = 0;
			return;
		}
		else rcv_buffer[index-1] = receive_data;
	}
	index++;
}

/********************************************************************
* 名称 : Com_Int()
* 功能 : 串口中断子函数
* 输入 : 无 * 输出 : 无
***********************************************************************/
void Com_Int(void) interrupt 4 {
	uchar i;
  EA = 0;
  if(RI == 1){//当硬件接收到一个数据时，RI会置位
		RI = 0;
		receive_data = SBUF;//接收到的数据
		if(hc08_enabled){
			LED = 0;
			if(flag == 0){
				flag = receive_data;
				index = 0;
				strcpy(rcv_buffer, "");
			}
			switch(flag){
				case 0x3A:{
					switch_hour_type();
					flag=0;
					index=0;
					break;
				}
				case '&':{
					set_date_via_bt();
					break;
				}
				case '@':{
					set_time_via_bt();
					break;
				}
				case '$':{
					sync_time_via_bt();
					break;
				}
				case '%':{
					set_alarm_via_bt();
					break;
				}
				case '#':{
					set_volume_via_bt();
					break;
				}
				case '^':{
					rcv_weather_all_via_bt();
					break;
				}
				case '`':{
					set_event_via_bt();
					break;
				}
				case '*':{
					play_music_via_bt();
					break;
				}
				case '~':{
					switch_mod_via_bt();
					break;
				}
				default:{
					break;
				}
			}
		}
		LED = 1;
	}
	EA = 1;
}

void sendData(uchar *datas){
	uchar i=0;
	if(!hc08_enabled) return;
	while(datas[i]!='\0')
    {	SBUF=datas[i];		  //???????????????
    	while(!TI);       //????????
    	TI=0;
		i++;}			  //?????????
}

/*******************************************************************************
* 函 数 名       : UsartConfiguration()
* 函数功能		   : 设置串口
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/

void UsartConfiguration()
{	
	SCON=0X50;			//设置为工作方式1,8位数据，可变波特率
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X00;			//波特率不加倍
	TH1=0XFd;		    //计数器初始值设置，9600@11.0592MHz
	TL1=0XFd;
	TR1=1;					//打开计数器
	ES = 1;         //开串口中断
  EA = 1;         //开总中断
}
