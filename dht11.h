#ifndef _DHT11_H_
#define _DHT11_H_

#include<reg52.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit Data = P0^2;

typedef struct sensor
{	uchar temper;
	uchar humid;
}Sensor;

void getdata(uchar *sensordata);
void print_temp_and_hum(uchar x, uchar y);

#endif 