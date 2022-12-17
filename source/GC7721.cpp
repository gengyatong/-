#include "GC7721.h"
#include "dataProc.h"
#include "GPIO.h"
#include "delay.h"
GC7721::GC7721(uartConfig * uartCfg):Uart( uartCfg )
{
   dataProc_ = NULL;
   //初始化7721对应的IO口，并控制IO口输出将7721调整到电阻测量模式
   GpioConfig modeSelectCfg = {Port2,Pin0,iofunc,ioOutput};
   Gpio * modeSelect = new Gpio(&modeSelectCfg);
   GpioConfig typeComCfg = {Port2,Pin1,iofunc,ioOutput};
   Gpio * typeCom = new Gpio(&typeComCfg);
   *modeSelect = 1;
   *typeCom = 1;
   delay_ms(1);
   *modeSelect = 1;
   *typeCom = 0;
   //完成7721配置后就可以删除这两个IO对象了
   delete modeSelect;
   delete typeCom;
}

GC7721::~GC7721()
{
}

void GC7721::DisableInterupt()
{
   IE2 &= ~URXIE1; 
}

void GC7721::EnableInterupt()
{
   IE2 |= URXIE1;
}

void GC7721::receive_frame_GC7721()
{
    unsigned char temp,lastdata,flag;
   
   temp=UartRxData();
   flag=temp&0xf0;
   if(flag==0x10)
   {
     front=1;
     datanum=0;
   }
   if(front)
   {
      rdat[datanum++]=temp;
      lastdata= temp&0xf0;
      if(datanum==14&&lastdata==0xe0)
      {
      if(lastdata==0xe0)
      {
      //datanum=0;
       front=0;
       //数据接收完成,调用其他模块的方法,将收到的帧数据传输给其他模块
       ReiceiveDoneCallback(rdat);
      }
       }
   }   


}