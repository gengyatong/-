#ifndef _MAIM_H
#define _MAIM_H

extern "C" {
#include <msp430.h>
}
#include "key.h"
#include "GC7721.h"
#include "uart.h"
#include "GPIO.h"
#include "warner.h"
#include "interupt.h"
#include "delay.h"
#include "dataProc.h"
//用于定义GC7721一帧数据接收完成后的处理函数
void DataProcGetString(unsigned char *);


#endif
