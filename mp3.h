#ifndef _MP3_H_
#define _MP3_H_

#include<reg52.h>

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

sbit sda = P3^7;

void Send( uchar addr );
void playmusic(uchar num, uchar vol_type);
void stopmusic();
void set_single_loop(uchar is_loop);
void send_volume(uchar vol_type);
void mp3_init();
char get_notification_volume();
char get_media_volume();
char get_alert_volume();
void set_notification_volume(char vol_temp);
void set_media_volume(char vol_temp);
void set_alert_volume(char vol_temp);
void page_music();

#endif 