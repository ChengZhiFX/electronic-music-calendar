#ifndef _LUNAR_H_
#define _LUNAR_H_

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

uchar get_lunar_calendar(uint year, uchar month, uchar day, uchar lunar_date[2]);
uchar get_solar_term_num(uint year, uchar month, uchar day);

#endif

