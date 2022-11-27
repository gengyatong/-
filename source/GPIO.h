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
    //��ӦbitΪ0�����ioΪ���뷽��
    ioInput  = 0x00,
    //��ӦbitΪ1�����ioΪ�������
    ioOutput = 0x01
  }DirectDef;
  
  typedef enum
  {
    //��ӦbitΪ0�����IOΪ��ͨIO��;��
    iofunc  = 0x00,
    //��ӦbitΪ1�����IOΪ������;(peripheral device )
    pdfunc = 0x01
  }FuncSelDef;
    
    
  //ÿ�õ�1��pin,������һ��������PIN�Ľṹ��
  typedef struct gpioConfig
  {
    PortNumDef  PortNum;     //port ������
    PinNumDef   PinNum;      //pin   ���
    FuncSelDef  FuncSel;     //����ѡ��
    DirectDef   Direction;   //����/�����������
  }GpioConfig;
  
class Gpio
  {
  public :  
        
      void operator=(unsigned char value); 
        
        //���캯�����趨ʹ�õ�IO���������÷�������ʹGpioGroupָ��ָ����������
      Gpio( GpioConfig *Gpio);
        
        //���������������ͷ�Gpio����IO�˿�ʱ��ռ�÷�����ڴ�
       ~Gpio();
        
  private:
        //�����IO�ڶ�Ӧ�Ļ���������
      GpioConfig GpioConfig_;
        
  protected:
        //��δ�õ�
       unsigned char PinNumNeed_; 
  };
   
}

#endif