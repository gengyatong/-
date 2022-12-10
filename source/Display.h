#ifndef _OLEDDISPLAY_H
#define _OLEDDISPLAY_H
#include <msp430.h>
#include "GPIO.h"


class Display
{
private:
    /* data */
    void displayInit();


public:
    Display(/* args */);
    ~Display();
    //显示字符串函数参数：  x：起始x位置  y：起始y位置  str：显示的字符串  size :显示字符大小 
    void ShowString(unsigned char x,unsigned char y,unsigned char *str ,unsigned char size);
    //显示字符串函数参数：  x：起始x位置  y：起始y位置  str：显示的字符串  size :显示字符大小 mode:阴/阳文显示
    void ShowChar(unsigned char x,unsigned char y,unsigned char str ,unsigned char size ,unsigned char mode);
    //显示数字 x：起始x位置  y：起始y位置  num：显示的字符串 len:数字长度 size :显示字符大小
    void ShowNUM(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size);
    //刷新显存，将显存内容搬运到屏幕上
    void RefreshScreen();
};

#endif