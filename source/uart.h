#ifndef _UART_H
#define _UART_H

#define DMMDATALEN 14
extern unsigned char rdat[DMMDATALEN];
extern unsigned char num_deci[4];
extern unsigned char Data_Ready ;

static unsigned char front=0;
static unsigned char datanum=0;
static unsigned char DataReady=0;


void InitUART1(void);
void InitUART0(void);

void SendData(unsigned char *Data ,unsigned char length );

void receive_frame_UART1(unsigned char *p);


#define cloud_up1   0x01
#define cloud_down1 0x02
#define cloud_up2   0x03
#define cloud_down2 0x04
#define cloud_warningOn 0x05
#define cloud_warningOff 0x06
#define cloud_hold  0x07
#endif
