#ifndef _MP3_H_
#define _MP3_H_

#include<reg52.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif
sbit sda = P1^0;
void Send( uchar addr );
char get_volume();
void set_volume(char vol_temp);
void playmusic(uchar num);
void stopmusic();
void set_single_loop(uchar is_loop);
void page_music();

#endif 