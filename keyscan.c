#include "keyscan.h"
#include "delay.h"

uchar getKey(){
	uchar key_num;
	if(key1==0){
		LED = 0;
		delay_ms(20);
		if(key1==0) key_num=1;
		//while(!key1);
		LED = 1;
	}
	else if(key2==0){
		LED = 0;
		delay_ms(20);
		if(key2==0) key_num=2;
		//while(!key2);
		LED = 1;
	}
	else if(key3==0){
		LED = 0;
		delay_ms(20);
		if(key3==0) key_num=3;
		//while(!key3);
		LED = 1;
	}
	else if(key4==0){
		LED = 0;
		delay_ms(20);
		if(key4==0) key_num=4;
		//while(!key4);
		LED = 1;
	}
	else key_num=0;
	return key_num;
}