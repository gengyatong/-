#ifndef _KEY_H
#define _KEY_H
extern "C"
{
#include "msp430x14x.h" 
}
#include "GPIO.h" 

/*************类设计说明************/
//按键类（key）里面包含了板子所需要的所有按键（6个）
//所以在类的结构设计上，并没有继承于IO类，而是例化了
//6个io对象，并读取其输入作为按键值检测；
//另外的一个设计方法是（该代码里没有采用），建立单独的
//按键类，并且继承于IO类，这样在工程里就需要例化六个
//按键对象。



using namespace Msp430GPIO;
//设定按键值判断需要的掩码（prot4一共连接了六个按键，最高位最低位未用）
#define keyValueMask 0x7e

//定义对应按键的键值
//以独热码表示，方便后续有效按键值判断
typedef enum
{
    key_null =  0x00   ,
    key_hold =  0x02   ,
    key_up1  =  0x04   ,
    key_down1=  0x08   ,
    key_up2  =  0x10   ,
    key_down2=  0x20   ,
    key_warning = 0x40 , 
} KeyValue;  

//定义对应按键按下的io输入寄存器键值
typedef enum
{
    regValueHold  =  0x7c,
    regValueUp1   =  0x7a,
    regValueDown1 =  0x76,
    regValueUp2   =  0x6e,
    regValueDown2 =  0x5e,
    regValueWarnSW = 0x3e

}RegisterValue;

class Key
{
private:
    /* data */
    Gpio *KeyHode       ;
    Gpio *KeyUp1Config  ;
    Gpio *KeyDown1Config;
    Gpio *KeyUp2Config  ;
    Gpio *KeyDown2Config;
    Gpio *KeyWarnSWConfig;
    void KeyInit();
    
    //本次检测获得的按键值
    unsigned char keyValue;
    //上一次检测获得的按键值
    unsigned char lastKeyValue; 
    //输出给其他模块的本次扫描有效键值
    unsigned char EffectKeyValue;
    //指向按键相关IO口对应的输入寄存器地址
    unsigned int * keyValueInputReg;

public:
    Key(/* args */);
    ~Key();
    //按键值检测
    char KeyDetect ();
};


#endif