#include "GPIO.h"
namespace Msp430GPIO
{
  
 //单个gpio对象
  Gpio::Gpio ( GpioConfig *Gpio )
  {
    //保存port设置
    GpioConfig_ = *Gpio;
    
    //记录port方向设置寄存器地址
    unsigned int *PxDIR;
  
    //记录port功能设置寄存器地址
    unsigned int *PxSEL;

    switch((Gpio)->PortNum)
      {
        case Port1:
        {PxDIR = (unsigned int *)P1DIR_;PxSEL = (unsigned int *)P1SEL_;break;}
        case Port2:
        {PxDIR = (unsigned int *)P2DIR_;PxSEL = (unsigned int *)P2SEL_;break;}
        case Port3:
        {PxDIR = (unsigned int *)P3DIR_;PxSEL = (unsigned int *)P3SEL_;break;}        
        case Port4:
        {PxDIR = (unsigned int *)P4DIR_;PxSEL = (unsigned int *)P4SEL_;break;}    
        case Port5:
        {PxDIR = (unsigned int *)P5DIR_;PxSEL = (unsigned int *)P5SEL_;break;}
        case Port6:
        {PxDIR = (unsigned int *)P6DIR_;PxSEL = (unsigned int *)P6SEL_;break;}
      }
      //如果io功能为io，则对应寄存器bit位为0
      if(Gpio->FuncSel == iofunc)
        *(unsigned char*)PxSEL = *(unsigned char*)PxSEL &(~(0x01 << Gpio->PinNum));
      ////如果io功能为外设，则对应寄存器bit位为1
      else
        *(unsigned char*)PxSEL = *(unsigned char*)PxSEL |( (0x01 << Gpio->PinNum));
   
      //如果io方向为输入，则对应寄存器bit位为0
      if(Gpio->Direction == ioInput)
        *(unsigned char*)PxDIR = *(unsigned char*)PxDIR &(~(0x01 << Gpio->PinNum));
      //如果io方向为输出，则对应寄存器bit位为0
      else     
        *(unsigned char*)PxDIR = *(unsigned char*)PxDIR|( (0x01 << Gpio->PinNum));     
  }  
  
  void Gpio::operator=(unsigned char value)
  {
    unsigned int * PxOUT ;
    switch(GpioConfig_.PortNum)
      {
        case Port1:
                PxOUT = (unsigned int *)P1OUT_; 
                break;
        case Port2:
                PxOUT = (unsigned int *)P2OUT_;
                break;
        case Port3:
                PxOUT = (unsigned int *)P3OUT_;
                break;
        case Port4:
                PxOUT = (unsigned int *)P4OUT_;
                break;
        case Port5:
                PxOUT = (unsigned int *)P5OUT_;
                break;
      case Port6:
                PxOUT = (unsigned int *)P6OUT_;
                break;
      }  
      if(value == 0)
      {
        *(unsigned char*)PxOUT = *(unsigned char*)PxOUT &  ( ~( 0x01 << GpioConfig_.PinNum ) );
      }
      if(value == 1)
      {
        *(unsigned char*)PxOUT = *(unsigned char*)PxOUT |  ( ( 0x01 << GpioConfig_.PinNum ) ) ;      
      }
  } 
  
  Gpio::~Gpio()
  {
  //  delete [] GpioGroup_ ;    
  }
  
  
}