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

uchar receive_data, flag=0, index=0;
//uchar send_buffer[36] = "���ݻ����Ϣ�Ƽ����޹�˾www.hc01.com";
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
			set_volume(5*Tinyint(rcv_buffer[0]));
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

/********************************************************************
* ���� : Com_Int()
* ���� : �����ж��Ӻ���
* ���� : �� * ��� : ��
***********************************************************************/
void Com_Int(void) interrupt 4 {
	uchar i;
//  uchar receive_data;

  EA = 0;
  if(RI == 1){//��Ӳ�����յ�һ������ʱ��RI����λ
		RI = 0;
		LED = 0;
		receive_data = SBUF;//���յ�������
		if(flag == 0){
			flag = receive_data;
			index = 0;
			strcpy(rcv_buffer, "");
		}
		switch(flag){
			case 0x2A:{
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
			default:{
				break;
			}
		}
		LED = 1;
	}
//		for(i=0; i<36; i++)
//		{
//			SBUF = send_buffer[i];   //��Ҫ���͵����ݷ��뵽���ͼĴ���
//			while(!TI);		    //�ȴ������������
//			TI=0;			        //���������ɱ�־λ
//			delay_ms(1);
//		}
	EA = 1;
}

void sendData(uchar *datas)
{	uchar i=0;
	while(datas[i]!='\0')
    {	SBUF=datas[i];		  //???????????????
    	while(!TI);       //????????
    	TI=0;
		i++;}			  //?????????
}

/*******************************************************************************
* �� �� ��       : UsartConfiguration()
* ��������		   : ���ô���
* ��    ��       : ��
* ��    ��       : ��
*******************************************************************************/

void UsartConfiguration()
{	
	SCON=0X50;			//����Ϊ������ʽ1,8λ���ݣ��ɱ䲨����
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X00;			//�����ʲ��ӱ�
	TH1=0XFd;		    //��������ʼֵ���ã�9600@11.0592MHz
	TL1=0XFd;
	TR1=1;					//�򿪼�����
	ES = 1;         //�������ж�
  EA = 1;         //�����ж�
}
