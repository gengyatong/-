#include "GPIO.h"
namespace Msp430GPIO
{
    //���ö�Ӧ��GPIO����
  void Gpio:: SetGpio(GpioConfig *Gpio  )
  {
    //����ѡ���ӦPortNum����Ҫ��Ӧ�������������Ĵ���
    unsigned int PxDIR;
    //����ѡ���ӦPortNum����Ҫ��Ӧ�Ĺ���ѡ��Ĵ���
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
      //�����ӦpinΪͨ��IO�ӿڣ���PxSEL�еĶ�Ӧbit����
      if((Gpio+i)->FuncSel == iofunc)
        PxSEL = PxSEL &(~(0x01 << (Gpio+i)->PinNum));
      //�����򽫶�Ӧbit��Ϊ1
      else
        PxSEL = PxSEL |( (0x01 << (Gpio+i)->PinNum));
        
      //�����ӦpinΪ���룬��PxDIR�еĶ�Ӧbit����
      if((Gpio+i)->Direction == ioInput)
        PxDIR = PxDIR &(~(0x01 << (Gpio+i)->PinNum));
      //�����򽫶�Ӧbit��Ϊ1
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