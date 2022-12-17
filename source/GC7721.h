#ifndef _GC7721_H
#define _GC7721_H
#include "uart.h"
extern "C"{
#include  "msp430x14x.h"
}
class DataProc;

#define  DMMDATALEN 14

class GC7721:public Uart
{
private:
    /* data */
    unsigned char front;
    unsigned char datanum;
    //用于存放GC7721传来的数据串
    unsigned char rdat[DMMDATALEN];
    //传入需要通知的数据处理模块指针；
    DataProc* dataProc_;
public:
    GC7721(uartConfig * uartCfg);
    ~GC7721();
    //数据接收完成后的回调函数指针
    void (*ReiceiveDoneCallback)(unsigned char * );
    //从GC7721接收桢数据,存储到rdat缓冲区中
    void receive_frame_GC7721();
    //关闭中断方法，在处理数据的时候需要关闭中断，以免数据出现前后段错乱
    static void DisableInterupt();
    //完成数据处理后打开中断
    static void EnableInterupt();
};













#endif