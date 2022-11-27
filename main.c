#include "msp430x14x.h"
#include "key.h"
#include "uart.h"
#include "data_proc.h"
#include "oled.h"
#include "GPIO.h"

extern unsigned char Data_Ready ;
extern unsigned char Data_Ready_0 ;
extern unsigned char CloudCmd ;

void clk_init()
{

  char i;
  BCSCTL1 &= ~XT2OFF;           //使用外部8M晶振
  BCSCTL2 |= (SELM_2 + SELS + DIVM_0 + DIVS_3 ) ; //主时钟为外部8M晶振 mclk 与 smclk 均设置为1M
  do 
  {
    IFG1 &= ~OFIFG;
    for( i=0 ;i<100;i++ );
  }
  while ( (IFG1&OFIFG) != 0 );
}

void led_init()
{
  //蜂鸣器及LED灯IO口控制
  P5DIR = 0XFF;
  P5OUT = 0x00;
  
  //fs9721对应管脚控制
  P2DIR = 0XFF;
  P2OUT = 0X03;     //拉高TYPE_COM 管脚，使9721切换到其他测量模式
  delay_ms(100);
  P2OUT = 0X01;     //拉低TYPE_COM 管脚，使9721重新回到电阻测量模式
}

void analogSWinit()
{
  P1DIR = P1DIR|0x20;  //p1.5管脚置为输出管脚
  P1DIR = P1DIR&0xBF;   //,P1.6管脚设置为输入管脚
  P1OUT = P1OUT&0xBF;  //设置1.6脚默认输出值为0
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

void main(void)
{
  
  GpioConfig LedGpioSet= { Port5,Pin4,iofunc,ioOutput};
  Gpio Led(&LedGpioSet);
  Led = 1;
  GpioConfig BeeperGpioSet= { Port5,Pin3,iofunc,ioOutput};
  Gpio Beeper(&BeeperGpioSet);
  
  while(1)
  {

  }
  
}
