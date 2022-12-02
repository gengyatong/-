#include "uart.h"
extern "C"
{
    #include "msp430x14x.h"
}


Uart::Uart(UartConfig *UartCfg )
{
    //保存UART相关设置，主要是uart0还是uart1
    uartConfigSave = *UartCfg;
    if(UartCfg->UartId == uart0)
        InitUART0();
    else if (UartCfg->UartId == uart1) 
        InitUART1();
}

Uart::~Uart()
{
    //删除相关io口对象
    delete uartRx;
    delete uartTx;
}


//串口的初始化代码调用;这段代码是从旧的工程抄过来的，除了io口初始化的相关指令以外
//其他部分代码没有改动
extern "C" {
    void Uart::InitUART0()
    {
        GpioConfig uartRxcfg =  {Port3,Pin5,pdfunc,ioInput  }; 
        uartRx = new Gpio(&uartRxcfg);
        GpioConfig uartTxcfg =  {Port3,Pin4,pdfunc,ioOutput};
        uartTx = new Gpio(&uartTxcfg);
        ME1 |= URXE0 + UTXE0;                     // Enable USART1 T/RXD
        UCTL0 |= CHAR;                            // 8-bit character
        UTCTL0 |= 0x30;                           // UCLK = SMCLK = 1M
        UBR00 = 0x08;                             // 1M/115200 = 8.6
        UBR10 = 0x00;                             //
        UMCTL0 = 0xAB;                            // Modulation
        UCTL0 &= ~SWRST;                          // Initialize USART state machine
        IE1 |= URXIE0; 
        _EINT();
    }

    void Uart:: InitUART1(void)
    {
        GpioConfig uartRxcfg =  {Port3,Pin7,pdfunc,ioInput  }; 
        uartRx = new Gpio(&uartRxcfg);
        GpioConfig uartTxcfg =  {Port3,Pin6,pdfunc,ioOutput};
        uartTx = new Gpio(&uartTxcfg);
        ME2 |= URXE1 + UTXE1;                     // Enable USART1 T/RXD
        UCTL1 |= CHAR;                            // 8-bit character
        UTCTL1 |= 0x30;                           // UCLK = SMCLK = 1M
        UBR01 = 0xA0;                             // 1M/2400 = 416.666
        UBR11 = 0x01;                             //
        UMCTL1 = 0x89;                            // Modulation
        UCTL1 &= ~SWRST;                          // Initialize USART state machine
        IE2 |= URXIE1;                            // ʹ��USART1�Ľ����ж�
        _EINT();
    }

//两个串口的发送函数分开处理
    void Uart:: UartTxData(unsigned char *Data, unsigned char length )
    {
        unsigned char i ;
        if( uartConfigSave.UartId == uart0)
            for( i = 0;i<length;i++)
            {  
                while ( !(IFG1&UTXIFG0) );
                TXBUF0 = *(Data+i);
            }
        else
            for( i = 0;i<length;i++)
            {
                while ( !(IFG2&UTXIFG1) );
                TXBUF1 = *(Data+i);
            }
    }

//串口数据接收
    unsigned char  Uart::UartRxData()
    {
        unsigned char dataRx;
        //当串口编号不同时，分别读取不同的串口buffer内的值
        if(uartConfigSave.UartId == uart1)
            dataRx = U1RXBUF;
        else
            dataRx = U0RXBUF;
        return dataRx;  
    }

}

