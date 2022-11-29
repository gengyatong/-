#ifndef _GPIO_H
#define _GPIO_H

extern "C" {
#include "msp430x14x.h" 
}
#include <stdlib.h>


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
    //io输入对应bit为0
    ioInput  = 0x00,
    //io输出对应bit为1
    ioOutput = 0x01
  }DirectDef;
  
  typedef enum
  {
    //io功能为0
    iofunc  = 0x00,
    //外设功能为1;(peripheral device )
    pdfunc = 0x01
  }FuncSelDef;
    
    
  //定义引脚配置相关结构体
  typedef struct gpioConfig
  {
    PortNumDef  PortNum;     //port 序号
    PinNumDef   PinNum;      //pin  序号
    FuncSelDef  FuncSel;     //功能选择
    DirectDef   Direction;   //方向选择
  }GpioConfig;
  
class Gpio
  {
  public :  
      //对应IO口输出（运算符重载） 
      void operator=(unsigned char value); 
      //对应IO口输入读取（暂未编写）

        //构造函数
      Gpio( GpioConfig *Gpio);
        
        //析构函数
       ~Gpio();
        
  private:
        //保存对应io设置
      GpioConfig GpioConfig_;
        
  protected:
        //暂未使用
       unsigned char PinNumNeed_; 
  };
   
}

#endif