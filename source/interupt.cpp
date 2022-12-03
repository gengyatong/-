extern "C"
{
#include "interupt.h"
}

extern GC7721 *gc7721;

int i = 0 ;
//串口0中断处理函数
// UART0 RX ISR will for exit from LPM3 in Mainloop
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USART1RX_VECTOR
__interrupt void usart1_rx (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USART1RX_VECTOR))) usart1_rx (void)
#else
#error Compiler not supported!
#endif
{
  gc7721->receive_frame_GC7721(); 
}