#include "key.h"

/****************操作过程说明***********/
//按键检测程序会记录KeyDetect运行时，检测到的本次按键，
//并且将这个按键值与上一次检测的按键值进行对比，当两次
//结果不同，并且对应bit出现跳变时，则认为该按键已经按下
//按键检测程序本身不会运行多次，它完全依赖mian函数循环
//或者其他循环进行至少两次调用才会得出对应按键值
//这种做法可以不用添加单独的按键消抖等待延迟，但是
//坏处是不支持按键长时间按下检测

Key::Key(/* args */)
{   
    KeyInit();
}

Key::~Key()
{
    delete KeyHode       ;
    delete KeyUp1Config  ;
    delete KeyDown1Config;
    delete KeyUp2Config  ;
    delete KeyDown2Config;
    delete KeyWarnSWConfig;
}

void Key::KeyInit()
{
    //按键对应寄存器初始化
    lastKeyValue = 0;
    keyValue     = 0;
    //设置按键对应的IO输入寄存器地址
    keyValueInputReg = (unsigned int*)P4IN_;
    //hold 按键
    GpioConfig keyHodeConfig   = {Port4,Pin1,iofunc,ioInput } ;
    KeyHode = new Gpio(&keyHodeConfig);
    //阈值1上调按键
    GpioConfig keyUp1Config    = {Port4,Pin2,iofunc,ioInput } ;
    KeyUp1Config = new Gpio(&keyUp1Config);
    //阈值1下调按键
    GpioConfig keyDown1Config  = {Port4,Pin3,iofunc,ioInput } ;
    KeyDown1Config = new Gpio(&keyDown1Config);
    //阈值2上调按键
    GpioConfig keyUp2Config    = {Port4,Pin4,iofunc,ioInput } ;
    KeyUp2Config = new Gpio(&keyUp2Config);
    //阈值2下调按键
    GpioConfig keyDown2Config  = {Port4,Pin5,iofunc,ioInput } ;
    KeyDown2Config = new Gpio(&keyDown2Config);
    //报警开关按键
    GpioConfig keyWarnSWConfig = {Port4,Pin6,iofunc,ioInput } ;
    KeyWarnSWConfig = new Gpio(&keyWarnSWConfig);
}

char Key::KeyDetect()
{
    switch ( *((unsigned char*)keyValueInputReg) & keyValueMask )
  {                           
    case regValueHold   : keyValue = key_hold   ;     break; 
    case regValueUp1    : keyValue = key_up1    ;     break;
    case regValueDown1  : keyValue = key_down1  ;     break;
    case regValueUp2    : keyValue = key_up2    ;     break;
    case regValueDown2  : keyValue = key_down2  ;     break;
    case regValueWarnSW : keyValue = key_warning;     break;
    default             : keyValue = key_null   ;     break;    
  }   
    //按键检测不支持连按，检测到判断的按键值出现单bit上升沿，则认为该键被按下；
    EffectKeyValue = keyValue & (~lastKeyValue) & keyValueMask;
    //将本次按键键值所存
    lastKeyValue = keyValue;
    return EffectKeyValue;
}