#include "key.h"
#include "msp430x14x.h"

void key_init (void )
{
    P4DIR = 0X00;   //设置P4端口为输入
}

char key_detect (void )
{
  char key_value ; 
  switch ( P4IN&0x7e )
  {                           //有按键为低，则说明被按下
    case 0x7c : key_value = key_hold ;     break; 
    case 0x7a : key_value = key_up1  ;     break;
    case 0x76 : key_value = key_down1;     break;
    case 0x6e : key_value = key_up2;       break;
    case 0x5e : key_value = key_down2;     break;
    case 0x3e : key_value = key_warning;   break;
    default   : key_value = key_null      ;break;     //非以上值代表无按键按下
  }          
  return key_value;
}