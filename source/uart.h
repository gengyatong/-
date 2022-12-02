#ifndef _UART_H
#define _UART_H

extern "C" {
#include "msp430x14x.h" 
}
#include "GPIO.h"

using namespace Msp430GPIO;

//串口编号，串口0/串口1
typedef enum
{
    uart0,
    uart1,
}UartOderNum; 

//波特率设置
typedef enum
{
    Bps115200,  //115200 波特率用于esp8266
    Bps2400,    //2400波特率用于gc7721
}Bps;

/*(暂未使用)设置串口所必要的寄存器值内容
typedef struct uartRegistList
{   
    //输入输出使能寄存器内容
    unsigned int* ME;
    //数据位数寄存器内容
    unsigned int* UCTL;
    //时钟源选择寄存器(UCLK = SMCLK = 1M)
    unsigned int* UTCTL;
    //串口时钟分频比寄存器内容
    unsigned int* UBR0;
    unsigned int* UBR1;
    //Modulation
    unsigned int* UMCTL;
    //中断寄存器
    unsigned int* IE ; 
}UartRegistList;*/

typedef struct uartConfig
{
    //数据位默认为8bit，停止位1bit，没有校验位，
    //所以下面结构中不再进行这些向的设置

    //使用的串口编号 串口0/串口1
    UartOderNum UartId;
    //波特率设置
    Bps         BpsSet;

} UartConfig;

class Uart
{
private:
    void InitUART1(void);
    void InitUART0(void);
    Gpio *uartTx;
    Gpio *uartRx;
    UartConfig uartConfigSave;
public:
    Uart(UartConfig *UartCfg);
    ~Uart();
    unsigned char  UartRxData();
    void UartTxData(unsigned char* ,unsigned char length );
};

/*
#define DMMDATALEN 14
extern unsigned char rdat[DMMDATALEN];
extern unsigned char num_deci[4];
extern unsigned char Data_Ready ;

static unsigned char front=0;
static unsigned char datanum=0;
static unsigned char DataReady=0;


void InitUART1(void);
void InitUART0(void);

void SendData(unsigned char *Data ,unsigned char length );

void receive_frame_UART1(unsigned char *p);


#define cloud_up1   0x01
#define cloud_down1 0x02
#define cloud_up2   0x03
#define cloud_down2 0x04
#define cloud_warningOn 0x05
#define cloud_warningOff 0x06
#define cloud_hold  0x07
*/

#endif
