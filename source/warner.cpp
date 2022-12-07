#include "warner.h"




Warner::Warner()
{
  //回调函数初始为空
  func_  = NULL;
  //报警开关默认开启  
  warningSw_ = 0xff; 
  //添加LED和蜂鸣器对应的IO口
  GpioConfig ledConfig  = {Port5,Pin4,iofunc,ioOutput } ;
  GpioConfig bepperConfig = {Port5,Pin3,iofunc,ioOutput };
  Led = new Gpio (&ledConfig);
  Beeper = new Gpio(&bepperConfig);  
}

//析构函数
Warner::~Warner()
{
    delete Led;
    delete Beeper;
}

void Warner:: SetCallBackFunc( void(*func)() )
{
    func_ = func;
}


void Warner::SetWarning()
{
    warningSw_ = 0xff;
    Warning();
}

void Warner::ClearWarning()
{
    warningSw_ = 0x00;
    Warning(); 
}


void Warner::Warning( )
{   //如果报警功能开启,则蜂鸣器运行，Led点亮
    if(warningSw_ == 0xff)
    {   
        BeeperGo();
        *Led = 1;
    }
    //否则关闭蜂鸣器与LED
    else
    {   
        *Beeper = 0;
        *Led = 0;
    }
}

//产生PWM波，蜂鸣器发出声音
void Warner::BeeperGo()
{   for(int j = 0;j<BeepPerCircle;j++)
   {
      for(int i= 0; i<BeeperOnce; i++ ) 
       {
            delay_us(150);
            *Beeper = 1;
            delay_us(150);
            *Beeper = 0;
       }
       //如果回调函数指针不为空，则调用回调函数
       if(func_ != NULL)
       {
            (*func_)();
       }
    }

}