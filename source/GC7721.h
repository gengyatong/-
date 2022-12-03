#ifndef _GC7721_H
#define _GC7721_H
#include "uart.h"
extern "C"{
#include  "msp430x14x.h"
}
#define  DMMDATALEN 14

class GC7721:public Uart
{
private:
    /* data */
    unsigned char front;
    unsigned char datanum;
    //用于存放GC7721传来的数据串
    unsigned char rdat[DMMDATALEN];

public:
    GC7721(uartConfig * uartCfg);
    ~GC7721();
    //用于调用接收完成后其他模块的处理方法,将缓冲器数组的指针返回给其他方法
    void (*GC7721Str2Proc)( unsigned char * );
    //从GC7721接收桢数据,存储到rdat缓冲区中
    void receive_frame_GC7721();
};













#endif