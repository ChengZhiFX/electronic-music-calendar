#ifndef _HC08_H_
#define _HC08_H_

#include "reg52.h"

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//--����ʹ�õ�IO--//

sbit LED=P0^0;

//--����ȫ�ֺ���--//   
void sendData(uchar *datas);
void UsartConfiguration();

#endif