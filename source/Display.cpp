#include "Display.h"
extern "C"
{
#include "oled.h"
#include "oledfont.h"
}


using namespace Msp430GPIO;

Display::Display(/* args */)
{
    displayInit();
}

Display::~Display()
{

}

void Display::displayInit()
{
    //调用构造函数进行GPIO初始化
    GpioConfig cs = { Port1,Pin0,iofunc,ioOutput };
    Gpio *CS = new Gpio(&cs);

    GpioConfig dc = { Port1,Pin1,iofunc,ioOutput };
    Gpio *DC = new Gpio(&dc);

    GpioConfig res = { Port1,Pin2,iofunc,ioOutput };
    Gpio *RES = new Gpio(&res);

    GpioConfig sda = { Port1,Pin3,iofunc,ioOutput };
    Gpio *SDA = new Gpio(&sda);

    GpioConfig sck = { Port1,Pin4,iofunc,ioOutput };
    Gpio *SCK = new Gpio(&sck);
    //调用C函数
    OLED_Init(); 
    //初始化完成后，删除GPIO对象,具体的IO口操作是用C语言写的，不需要用GPIO对象操作了
    delete CS;
    delete DC;
    delete RES;
    delete SDA;
    delete SCK;

    //显示初始化使用的固定字符
    OLED_ShowString(0,0, "Init:" ,16); 
    OLED_ShowString(0,16, "Th1:" ,16);        //显示固定字符
    OLED_ShowChar(56,16,'%',16,1);
    OLED_ShowString(0,32, "Th2:" ,16);        //显示固定字符
    OLED_ShowChar(56,32,'%',16,1);  
    OLED_ShowString(80,20, "WarnSW:" ,12);
    OLED_Refresh_Gram();                      //刷新屏幕显示
}

void Display::ShowString(unsigned char x,unsigned char y,unsigned char *str ,unsigned char size )
{
    //调用C的函数
    OLED_ShowString( x,y,str,size);
}

void Display::ShowChar(unsigned char x,unsigned char y,unsigned char str ,unsigned char size ,unsigned char mode )
{
    //调用C的函数
    OLED_ShowChar(x,y,str,size,mode);
}

void Display::ShowNUM(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char size)
{
    OLED_ShowNum(x,y,num,len,size);
}

void Display::RefreshScreen()
{
    //调用C 的刷新屏幕显示
    OLED_Refresh_Gram();                      //刷新屏幕显示
}



