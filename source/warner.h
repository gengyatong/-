#ifndef _WARNER_H
#define _WARNER_H
#include "GPIO.h" 
#include "main.h"

using namespace Msp430GPIO;

//设置蜂鸣器鸣叫一次，有多少个PWM波
#define BeeperOnce 30
//设置程序运行一轮蜂鸣器鸣叫多少次
#define BeepPerCircle 5  

class Warner
{
    public :
        Warner();
        ~Warner();
        //由数据处理模块设置开始报警提示
        void SetWarning();
        //由数据处理模块设置停止报警
        void ClearWarning();
        //设置报警过程中的按键检测或其他回调函数，以提高报警过程中对其他输入的相应频率
        void SetCallBackFunc( void(*func)() );
 
    private :
        //报警状态开关
        unsigned char warningSw_ ;
        //需要执行的回调函数指针
        void (*func_) () ;
        //蜂鸣器响动
        void BeeperGo();
        //声明led和蜂鸣器IO
        Gpio *Led;
        Gpio *Beeper;
        //实现报警功能
        void Warning( );
};

#endif