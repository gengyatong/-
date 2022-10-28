#ifndef _DATA_PROC_H
#define _DATA_PROC_H


extern unsigned char num_deci[4] ;
extern unsigned char dp[3];
extern unsigned char unit[2] ;

extern unsigned char num_deci_char[4] ;
extern unsigned char dp_char[3];
extern unsigned char unit_char[2] ;


extern unsigned char data_display[12];

float data_proc (  unsigned char *data_in, unsigned char len ,unsigned char *num);

#endif