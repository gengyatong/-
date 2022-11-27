extern "C"{
#include "msp430x14x.h" 
}
#include <stdlib.h>
#ifndef _GPIO_H
#define _GPIO_H

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
  
  typedef enum 
  {
    Pin0 =  0x00 ,
    Pin1,
    Pin2,
    Pin3,
    Pin4,
    Pin5,
    Pin6,
    Pin7
  } PinNumDef;  
  
  typedef enum
  {
    //对应bit为0，则该io为输入方向
    ioInput  = 0x00,
    //对应bit为1，则该io为输出方向
    ioOutput = 0x01
  }DirectDef;
  
  typedef enum
  {
    //对应bit为0，则该IO为普通IO用途；
    iofunc  = 0x00,
    //对应bit为1，则该IO为外设用途(peripheral device )
    pdfunc = 0x01
  }FuncSelDef;
    
    
  //每用到1个pin,都声明一个针对这个PIN的结构体
  typedef struct gpioConfig
  {
    PortNumDef  PortNum;     //port 分组编号
    PinNumDef   PinNum;      //pin   编号
    FuncSelDef  FuncSel;     //功能选择
    DirectDef   Direction;   //输入/输出方向设置
  }GpioConfig;
  
class Gpio
  {
  public :  
        
      void operator=(unsigned char value); 
        
        //构造函数，设定使用的IO口数量，该方法将会使GpioGroup指针指向分配的数组
      Gpio( GpioConfig *Gpio);
        
        //析构函数，用于释放Gpio设置IO端口时所占用分配的内存
       ~Gpio();
        
  private:
        //保存该IO口对应的基础设置项
      GpioConfig GpioConfig_;
        
  protected:
        //暂未用到
       unsigned char PinNumNeed_; 
  };
   
}

#endif