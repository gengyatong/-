#include "GPIO.h"
namespace Msp430GPIO
{
  
  //���캯�������ø�PIN�ķ����Ƿ��ã�
  Gpio::Gpio ( GpioConfig *Gpio )
  {
    //�����IO�ڶ�Ӧ�Ļ���������
    GpioConfig_ = *Gpio;
    
    //�����ݴ��ӦPortNum����Ҫ��Ӧ�������������Ĵ��� 
    unsigned int *PxDIR;
  
    //�����ݴ��ӦPortNum����Ҫ��Ӧ�Ĺ���ѡ��Ĵ�������ͨIO�������������io��
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
      //�����ӦpinΪͨ��IO�ӿڣ���PxSEL�еĶ�Ӧbit����
      if(Gpio->FuncSel == iofunc)
        *(unsigned char*)PxSEL = *(unsigned char*)PxSEL &(~(0x01 << Gpio->PinNum));
      //�����򽫶�Ӧbit��Ϊ1
      else
        *(unsigned char*)PxSEL = *(unsigned char*)PxSEL |( (0x01 << Gpio->PinNum));
   
      //�����ӦpinΪ���룬��PxDIR�еĶ�Ӧbit����
      if(Gpio->Direction == ioInput)
        *(unsigned char*)PxDIR = *(unsigned char*)PxDIR &(~(0x01 << Gpio->PinNum));
      //�����򽫶�Ӧbit��Ϊ1
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