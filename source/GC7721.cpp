#include "GC7721.h"

GC7721::GC7721(uartConfig * uartCfg):Uart( uartCfg )
{
   //定义处理方法指针默认为空
   GC7721Str2Proc = NULL;
}

GC7721::~GC7721()
{
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
      if(lastdata==0xe0)
      {
      //datanum=0;
       front=0;
       //数据接收完成,调用其他模块的方法进行处理
       if(GC7721Str2Proc != NULL)
       {
         GC7721Str2Proc(rdat);
       }
      }
   }   


}