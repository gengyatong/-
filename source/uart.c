//#include "uart.h"
//#include "data_proc.h"

void InitUART1(void)
{
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
}


void InitUART0(void)
{
    P3SEL |= 0x30;                            // P3.5,6 = USART0 TXD/RXD
    P3DIR &= 0x5f;                            // rx�ܽ�����Ϊ����
    ME1 |= URXE0 + UTXE0;                     // Enable USART1 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= 0x30;                           // UCLK = SMCLK = 1M
    UBR00 = 0x08;                             // 1M/115200 = 8.6
    UBR10 = 0x00;                             //
    UMCTL0 = 0xAB;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 |= URXIE0;                            // ʹ��USART1�Ľ����ж�
 //   _EINT();
}

void SendData(unsigned char *Data, unsigned char length )
{
  unsigned char i ;
  for( i = 0;i<length;i++)
  {  
    while ( !(IFG1&UTXIFG0) );
    TXBUF0 = *(Data+i);
  }
}


/*����1�����ݽ����߼������ڽ������ñ�оƬ����*/

unsigned char Data_Ready;

void receive_frame_UART1(unsigned char *p)
{
   unsigned char temp,lastdata,flag;
   
   temp=U1RXBUF;//�Ƚ�������
   flag=temp&0xf0;
   if(flag==0x10)//��⵽֡ͷ
   {
     front=1;
     datanum=0;
   }
   if(front)//�����⵽֡ͷ�ͽ���ú�����������
   {
      p[datanum++]=temp;
      lastdata= temp&0xf0;
      if(datanum==14&&lastdata==0xe0)
       //PutChar2UART0(0xab);//for test
      if(lastdata==0xe0)
      {
      //datanum=0;
       front=0;
       Data_Ready = 1;
      }
   }
}

unsigned char rdat[DMMDATALEN];

#pragma vector = UART1RX_VECTOR
__interrupt void UART1_RXISR(void)
{

    receive_frame_UART1(rdat);
}



/*����0�����ݽ����߼������ڽ����ƶ�ָ��*/

unsigned char Data_Ready_0;
unsigned char CloudCmd = 0;
unsigned char CmdBuff[4] = {0,0,0,0};
unsigned char datanum0;
unsigned char front0;

void receive_frame_UART0(void)
{
   unsigned char temp,lastdata,flag;

   
   temp=U0RXBUF;//�Ƚ�������
   flag=temp&0xff;
   if(flag==0x55)//��⵽֡ͷ
   {
     front0=1;
     datanum0=0;
   }
   if(front0)//�����⵽֡ͷ�ͽ���ú�����������
   {
      CmdBuff[datanum0]=temp;
      datanum0++;
      lastdata= temp&0xff;
      if(datanum0==3&&lastdata==0xAA)
      {
      datanum0=0;
      front0=0;
      CloudCmd = CmdBuff[1];
      Data_Ready_0 = 1;
      }
      else if (datanum0==3&&lastdata!=0xAA)   //��������żӵ�3�����ǰ�β�����ϣ������ָ���Ϊָ�����
      {
      datanum0=0;
      front0=0;
      Data_Ready_0 = 0;
      CloudCmd = 0x00;
      }
        
   }
}

#pragma vector = UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{

    receive_frame_UART0();
}
