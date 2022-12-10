#include "main.h"
#include <msp430.h>
#include "Display.h"
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


GC7721  *gc7721;
DataProc * dataProc;
//定义初始显示电阻值
unsigned char initDisplay[13] = { '1','0','0','0','K',' ',' ',' ',' ',' ',' ',' ','\0' };

void main(void)
{ 
  //关闭看门狗
  close_watch_dog();
  //时钟初始化
  clk_init();
/****初始化相关模块*******/
//初始化报警器
  Warner warnner;
//初始化按键
  Key key;
//初始化显示模块
  Display *display = new Display();
  //初始化数据处理模块
  dataProc =  new DataProc(initDisplay);
//初始化7721
  UartConfig GC7721Cfg = { uart1,Bps2400  };
  gc7721 =new GC7721(&GC7721Cfg); 
  //设定7721收到消息后给到数据处理模块
  gc7721->SetDataProcModule(dataProc);

  
  unsigned char keyGet;
  //用于显示当前电阻值的字符串
  unsigned char * ResNowDisplay;
  //用于显示原始电阻值的字符串
  unsigned char * ResInitDisplay;
  while(1)
  {
    //检测按键值
    keyGet = key.KeyDetect();
    //将按键值输入到数据处理模块
    dataProc->GetKeyValue(keyGet);
    //获取需要显示的当前阻值字符串
    ResNowDisplay = dataProc->GetDisplayString();
    //获取需要显示的记录阻值字符串
    ResInitDisplay = dataProc->GetRecordDisplayString();
    display->ShowString(48,4,ResInitDisplay,12);
    display->ShowString(0,48,ResNowDisplay,16);
    //刷新显存，将内容搬运到显示屏幕上
    display->RefreshScreen();
    delay_ms(10);

  }

}


