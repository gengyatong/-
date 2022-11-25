#include "GPIO.h"
namespace Msp430GPIO
{
    //设置对应的GPIO功能
  void Gpio:: SetGpio(GpioConfig *Gpio  )
  {
    //用于选择对应PortNum所需要对应的输入输出方向寄存器
    unsigned int PxDIR;
    //用于选择对应PortNum所需要对应的功能选择寄存器
    unsigned int PxSEL;
   
    for(int i= 0 ;i<PinNumNeed;i++)
    {
      switch((Gpio+i)->PortNum)
      {
        case Port1:
        {PxDIR = P1DIR;PxSEL = P1SEL;}
        case Port2:
        {PxDIR = P2DIR;PxSEL = P2SEL;}
        case Port3:
        {PxDIR = P3DIR;PxSEL = P3SEL;}        
        case Port4:
        {PxDIR = P4DIR;PxSEL = P4SEL;}    
        case Port5:
        {PxDIR = P5DIR;PxSEL = P5SEL;}
        case Port6:
        {PxDIR = P6DIR;PxSEL = P6SEL;}
      }
      //如果对应pin为通用IO接口，则将PxSEL中的对应bit清零
      if((Gpio+i)->FuncSel == iofunc)
        PxSEL = PxSEL &(~(0x01 << (Gpio+i)->PinNum));
      //否则，则将对应bit置为1
      else
        PxSEL = PxSEL |( (0x01 << (Gpio+i)->PinNum));
        
      //如果对应pin为输入，则将PxDIR中的对应bit清零
      if((Gpio+i)->Direction == ioInput)
        PxDIR = PxDIR &(~(0x01 << (Gpio+i)->PinNum));
      //否则，则将对应bit置为1
      else
        PxDIR = PxDIR |( (0x01 << (Gpio+i)->PinNum));       
    }
  }
  
  Gpio::Gpio ( unsigned char pinNumNeed)
  {
    Gpio::PinNumNeed = pinNumNeed;
    GpioGroup = new GpioConfig[PinNumNeed] ;   
  }
  
  Gpio::~Gpio()
  {
    delete [] GpioGroup ;    
  }
  
  
}