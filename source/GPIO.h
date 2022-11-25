extern "C"{
#include "msp430x14x.h" 
}
#include <stdlib.h>
#ifndef _GPIO_H
#define _GPIO_H

//对应bit为0，则该io为输入方向
#define ioInput  0x00
//对应bit为1，则该io为输出方向
#define ioOutput 0x01

//对应bit为0，则该IO为普通IO用途；
#define iofunc 0x00
//对应bit为1，则该IO为外设用途
#define pmfunc 0x01

namespace Msp430GPIO
{
  
  typedef enum 
  {
    Port1,
    Port2,
    Port3,
    Port4,
    Port5,
    Port6
  } PortNumDef;  
  
  //每用到1个pin,都声明一个针对这个PIN的结构体
  typedef struct gpioConfig
  {
    PortNumDef     PortNum;     //port 分组编号
    unsigned char  PinNum;      //pin   编号
    unsigned char  FuncSel;     //功能选择
    unsigned char  Direction;   //输入/输出方向设置
  }GpioConfig;
  
class Gpio
  {
    private :
         
    public :  
        //指针指向需要设置的IO口 GpioConfig数组
        GpioConfig *GpioGroup;
        void SetGpio(GpioConfig *Gpio  );  
        //构造函数，设定使用的IO口数量
        Gpio(unsigned char pinNumNeed);
        //析构函数，用于释放Gpio设置IO端口时所占用分配的内存
        ~Gpio();
    protected:
       unsigned char PinNumNeed; 
  };
   
}

#endif