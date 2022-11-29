extern "C" {
#include "msp430x14x.h"
}
#include "key.h"
#include "uart.h"
#include "GPIO.h"
#include "warner.h"

extern unsigned char Data_Ready ;
extern unsigned char Data_Ready_0 ;
extern unsigned char CloudCmd ;
  
void clk_init()
{

  char i;
  BCSCTL1 &= ~XT2OFF;           //ʹ���ⲿ8M����
  BCSCTL2 |= (SELM_2 + SELS + DIVM_0 + DIVS_3 ) ; //��ʱ��Ϊ�ⲿ8M���� mclk �� smclk ������Ϊ1M
  do 
  {
    IFG1 &= ~OFIFG;
    for( i=0 ;i<100;i++ );
  }
  while ( (IFG1&OFIFG) != 0 );
}

void led_init()
{
  //��������LED��IO�ڿ���
  P5DIR = 0XFF;
  P5OUT = 0x00;
  
  //fs9721��Ӧ�ܽſ���
  P2DIR = 0XFF;
  P2OUT = 0X03;     //����TYPE_COM �ܽţ�ʹ9721�л�����������ģʽ
  delay_ms(100);
  P2OUT = 0X01;     //����TYPE_COM �ܽţ�ʹ9721���»ص��������ģʽ
}

void analogSWinit()
{
  P1DIR = P1DIR|0x20;  //p1.5�ܽ���Ϊ����ܽ�
  P1DIR = P1DIR&0xBF;   //,P1.6�ܽ�����Ϊ����ܽ�
  P1OUT = P1OUT&0xBF;  //����1.6��Ĭ�����ֵΪ0
}



void close_watch_dog()
{
  WDTCTL = WDTPW + WDTHOLD;
}


void strcopy( unsigned char end[]  ,unsigned char source[]  , unsigned char length  )
{
  unsigned char i;
  for(i= 0; i<length;i++ )
  {
    end[i] = source[i];
    
  }
}

using namespace Msp430GPIO;

void LedFlash()
{
       for(int i= 0; i<10; i++ ) {
        delay_ms(150);
        P5OUT = P5OUT | 0x10;   
        delay_ms(150);
        P5OUT = P5OUT & 0xEF ;} 
       return;
}




void main(void)
{ 
  close_watch_dog();
  clk_init();
  Warner warnner;
  Key key;
  warnner.SetCallBackFunc( LedFlash);
  while(1)
  {
      delay_ms(20);
      if(key.KeyDetect()!= 0)
      {
      LedFlash();
      }
    
  }
  
}
