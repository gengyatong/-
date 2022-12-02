#include "main.h"
#include  "msp430x14x.h"
#define CPU_CLOCK       8000000
#define delay_ms(ms)    __delay_cycles(CPU_CLOCK/1000*(ms))
#define delay_us(us)    __delay_cycles(CPU_CLOCK/1000000*(us))
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

//using namespace Msp430GPIO;

void LedFlash()
{
       for(int i= 0; i<10; i++ ) {
        delay_ms(150);
        P5OUT = P5OUT | 0x10;   
        delay_ms(150);
        P5OUT = P5OUT & 0xEF ;} 
       return;
}

 // UartConfig GC7721Cfg = { uart1,Bps2400  };
 // GC7721  *gc7721;




void main(void)
{ 
  close_watch_dog();
  clk_init();
 // Warner warnner;
 // Key key;
 //fs9721对应管脚控制
  P2DIR = 0XFF;
  P2OUT = 0X03;     //拉高TYPE_COM 管脚，使9721切换到其他测量模式
  delay_ms(100);
  P2OUT = 0X01;     //拉低TYPE_COM 管脚，使9721重新回到电阻测量模式
 // gc7721 = new GC7721(&GC7721Cfg);

    P3SEL |= 0xC0;                            // P3.6,7 = USART1 TXD/RXD
    P3DIR &= 0x7f;                            // rx�ܽ�����Ϊ����
    ME2 |= URXE1 + UTXE1;                     // Enable USART1 T/RXD
    UCTL1 |= CHAR;                            // 8-bit character
    UTCTL1 |= 0x30;                           // UCLK = SMCLK = 1M
    UBR01 = 0xA0;                             // 1M/2400 = 208.333
    UBR11 = 0x01;                             //
    UMCTL1 = 0x89;                            // Modulation
    UCTL1 &= ~SWRST;                          // Initialize USART state machine
    IE2 |= URXIE1;                            // ʹ��USART1�Ľ����ж�
    _EINT();

 // warnner.SetCallBackFunc( LedFlash);
  while(1)
  {
      delay_ms(20);
//      if(key.KeyDetect()!= 0)
 //     {
 //     LedFlash();
 //     }
    
  }
}
  //串口0中断处理函数
#pragma  vector = UART1RX_VECTOR
extern "C"    __interrupt void UART1_RXISR(void)
{
  int i =- 1;
//  gc7721->receive_frame_GC7721();
}
