extern "C"{
#include "msp430x14x.h" 
}
#include <stdlib.h>
#ifndef _GPIO_H
#define _GPIO_H

//��ӦbitΪ0�����ioΪ���뷽��
#define ioInput  0x00
//��ӦbitΪ1�����ioΪ�������
#define ioOutput 0x01

//��ӦbitΪ0�����IOΪ��ͨIO��;��
#define iofunc 0x00
//��ӦbitΪ1�����IOΪ������;
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
  
  //ÿ�õ�1��pin,������һ��������PIN�Ľṹ��
  typedef struct gpioConfig
  {
    PortNumDef     PortNum;     //port ������
    unsigned char  PinNum;      //pin   ���
    unsigned char  FuncSel;     //����ѡ��
    unsigned char  Direction;   //����/�����������
  }GpioConfig;
  
class Gpio
  {
    private :
         
    public :  
        //ָ��ָ����Ҫ���õ�IO�� GpioConfig����
        GpioConfig *GpioGroup;
        void SetGpio(GpioConfig *Gpio  );  
        //���캯�����趨ʹ�õ�IO������
        Gpio(unsigned char pinNumNeed);
        //���������������ͷ�Gpio����IO�˿�ʱ��ռ�÷�����ڴ�
        ~Gpio();
    protected:
       unsigned char PinNumNeed; 
  };
   
}

#endif