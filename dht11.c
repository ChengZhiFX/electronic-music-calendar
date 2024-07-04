#include "dht11.h"//ͷ�ļ�
#include "oled.h"
#include "delay.h"

const unsigned char code C8x8[] = {0x00,0x06,0x38,0x44,0x42,0x42,0x04,0x00};
uchar dht11_enabled = 1;
Sensor sensor;
sbit LED=P0^0;

//дʱ��ĺ���
void DHT11_delay_us(uchar n)
{
    while (--n);
}

void DHT11_delay_ms(uint z)
{
    uint i, j;
    for (i = z; i > 0; i--)
        for (j = 122; j > 0; j--);
}

/*------------------------------------------------
              DHT11��ʼ�ź�
------------------------------------------------*/
void DHT11_start() {
	Data = 1;
	DHT11_delay_us(2);
	Data = 0;
	DHT11_delay_ms(20);   //��ʱ18ms����
	Data = 1;
	DHT11_delay_us(30);
}

/*------------------------------------------------
              ���հ�λ������
------------------------------------------------*/
uchar DHT11_rec_byte()
{
    uchar i, dat = 0;
    for (i = 0; i < 8; i++)
    {
        while (!Data);
        DHT11_delay_us(8);
        dat <<= 1;
        if (Data == 1)
            dat += 1;
        while (Data);
    }
    return dat;
}

/*------------------------------------------------
              ����40bit����
------------------------------------------------*/
uchar wait_ms_for_data(uint ms){
	uchar j;
	while(ms--){	
		for(j=0;j<122;j++) if(Data) return 1;
		if(Data) return 1;
	}
	return 0;
}
	
uchar getdata(uchar *sensordata) {    //����40λ������
	uchar R_H, R_L, T_H, T_L, revise, i;
	uint ms = 2000;
//	if(!dht11_enabled) return;
	DHT11_start();
	if (Data == 0) {
		if(!wait_ms_for_data(2000)) return 0;
//		while (Data == 0); //�ȴ�����
		DHT11_delay_us(40);  //���ߺ���ʱ80us
		R_H = DHT11_rec_byte();  //����ʪ�ȸ߰�λ
		R_L = DHT11_rec_byte();  //����ʪ�ȵͰ�λ
		T_H = DHT11_rec_byte();  //�����¶ȸ߰�λ
		T_L = DHT11_rec_byte();  //�����¶ȵͰ�λ
		revise = DHT11_rec_byte(); //����У��λ

		DHT11_delay_us(25);    //����

		sensordata[0] = T_H;
		sensordata[1] = R_H;
		return 1;
	}
	return 0;
}

uchar dht11_try_catch_data(){
	if(getdata((uchar *)&sensor)){
		dht11_enabled = 1;
		return 1;
	}
	else{
		dht11_enabled = 0;
		return 0;
	}
}

void print_temp_and_hum(uchar x, uchar y){
	char temper_to_show[3] = "--", humid_to_show[] = "--%";
	uchar i=0;
	if(!dht11_enabled) return;
	if(!dht11_try_catch_data()){
		OLED_ShowString(x,y,"         ",12);
		return;
	}
	double_digit_to_string(sensor.temper, temper_to_show);
	humid_to_show[0] = Char(sensor.humid / 10);
	humid_to_show[1] = Char(sensor.humid % 10);
	humid_to_show[2] = '%';
	humid_to_show[3] = 0;
	OLED_ShowString(x,y, temper_to_show,12);
	OLED_Set_Pos(x+12,y);
	for(i=0;i<8;i++) OLED_WR_Byte(C8x8[i],OLED_DATA);
	OLED_ShowString(x+30,y, humid_to_show,12);
}
