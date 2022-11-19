#include "msp430x14x.h"
#include "key.h"
#include "uart.h"
#include "data_proc.h"
#include "oled.h"

extern unsigned char Data_Ready ;
extern unsigned char Data_Ready_0 ;
extern unsigned char CloudCmd ;

void clk_init()
{

  char i;
  BCSCTL1 &= ~XT2OFF;           //使用外部8M晶振
  BCSCTL2 |= (SELM_2 + SELS + DIVM_0 + DIVS_3 ) ; //主时钟为外部8M晶振 mclk 与 smclk 均设置为1M
  do 
  {
    IFG1 &= ~OFIFG;
    for( i=0 ;i<100;i++ );
  }
  while ( (IFG1&OFIFG) != 0 );
}

void led_init()
{
  //蜂鸣器及LED灯IO口控制
  P5DIR = 0XFF;
  P5OUT = 0x00;
  
  //fs9721对应管脚控制
  P2DIR = 0XFF;
  P2OUT = 0X03;     //拉高TYPE_COM 管脚，使9721切换到其他测量模式
  delay_ms(100);
  P2OUT = 0X01;     //拉低TYPE_COM 管脚，使9721重新回到电阻测量模式
}

void analogSWinit()
{
  P1DIR = P1DIR|0x20;  //p1.5管脚置为输出管脚
  P1DIR = P1DIR&0xBF;   //,P1.6管脚设置为输入管脚
  P1OUT = P1OUT&0xBF;  //设置1.6脚默认输出值为0
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


void main(void)
{

  unsigned char  thresh_2 = 20 ;
  unsigned char  thresh_1 = 10;
 
  int key_value = 0 ;       //检测当前按键值
  int key_value_reg = 0 ;   //锁存上一次按键值
  int key_effect = 0 ;      //获得边沿检测后的有效值
  char waring_sw = 0x00 ;   //报警功能开关
  
  unsigned char data_display_reg[13] = { '1','0','0','0','K',' ',' ',' ',' ',' ',' ',' ','\0' };
  
  float res_save = 1000 ;     //默认初始电阻值为1M
  float  res_now = 1000 ;     //记录当前电阻值
  
  float thresh_2_value_H = 0;
  float thresh_2_value_L = 0;
  
  float thresh_1_value_H = 0;
  float thresh_1_value_L = 0;
  
  unsigned char packHeader[2] = {0x55,0xAA};
  unsigned char packEnder[2] = {0xAA,0X55};
  
  unsigned char  DeviceState = 0x00 ;  //设备状态寄存器  第0位：报警开关是否打开；第1位：阈值1报警开关；第2位：阈值2报警开关；第3位：命令接收回复握手
  
  struct DataToSend {
  unsigned char header[2] ; 
  unsigned char dataInit[9];
  unsigned char dataNow[9];
  unsigned char thread1;
  unsigned char thread2;
  unsigned char deviceState ;
  unsigned char ender[2];
  
  };
  struct DataToSend dataToSend ;
  
  close_watch_dog();
  clk_init();
  led_init();
  key_init();

  OLED_Init();	//初始化oled
  analogSWinit();
  OLED_ShowString(0,0, "Init:" ,16); 
  
  OLED_ShowString(0,16, "Th1:" ,16);        //显示固定字符
  OLED_ShowChar(56,16,'%',16,1);

  OLED_ShowString(0,32, "Th2:" ,16);        //显示固定字符
  OLED_ShowChar(56,32,'%',16,1);  
  
  OLED_ShowString(80,20, "WarnSW:" ,12);
  
  OLED_Refresh_Gram();       //刷新屏幕显示
  
  InitUART1();                //初始化串
//   _EINT();
  InitUART0();
  while(1)  {
 
//数据处理及显示刷新    
    if(Data_Ready)  
     {  
      IE2 = 0X0;                                // 关闭USART1的接收中断 
      
      res_now = data_proc (  rdat ,14, num_deci );
 
      OLED_ShowString(0,48,data_display,16);
      
      OLED_Refresh_Gram();                     //刷新屏幕显示
      
      IE2 |= URXIE1;                           // 使能USART1的接收中断    
    }  
    
    
//监测是否有云端发送来的命令   
    if(Data_Ready_0)
    {
      IE1 = 0X0;                                // 关闭USART0的接收中断 
      switch( CloudCmd )
      {
        case cloud_hold  :  { res_save = res_now;                   //锁存当前按键值
                            for(int i=0;i<12;i++)
                            data_display_reg[i] = data_display[i];     //锁存显示电阻字符值
                            };break;	
      
        case cloud_up2   :  {      
                          if (thresh_2 <100 )
                            thresh_2 = thresh_2 + 5;
                          else
                            thresh_2 = 100;                         
                          };break;                         
                          
	case cloud_down2 : {      
                          if (thresh_2 > 5 )
                            thresh_2 = thresh_2 - 5;
                          else
                            thresh_2 = 5 ;                         
                          };break; 
                          
        case cloud_up1   :  {      
                          if (thresh_1 <100 )
                            thresh_1 = thresh_1 + 5;
                          else
                            thresh_1 = 100;                         
                          };break;
	  
        case cloud_down1 : {      
                          if (thresh_1 > 5 )
                            thresh_1 = thresh_1 - 5;
                          else
                            thresh_1 = 5 ;                         
                          };break; 
                          
        case cloud_warningOn : {
                         waring_sw = 0xFF   ;      
                          };break;   
                          
        case cloud_warningOff : {
                         waring_sw = 0x00   ;      
                          };break;                   
	}  
      
      DeviceState = DeviceState|0x08;          //拉高第3位
      Data_Ready_0 = 0;
      IE1 |= URXIE0;                           // 使能USART0的接收中断  
    
    }
    
//按键值判断，根据按键值修改阈值设置    
   
    key_value = key_detect();         //获得当前按键值
    key_effect = key_value & (~ key_value_reg ) & 0x3F ;    //获取有效按键值  
    
    switch( key_effect )
      {
        case key_hold  :  { res_save = res_now;                   //锁存当前按键值
                            for(int i=0;i<12;i++)
                            data_display_reg[i] = data_display[i];     //锁存显示电阻字符值
                                                };break;	
      
        case key_up2   :  {      
                          if (thresh_2 <100 )
                            thresh_2 = thresh_2 + 5;
                          else
                            thresh_2 = 100;                         
                          };break;                         
                          
	case key_down2 : {      
                          if (thresh_2 > 5 )
                            thresh_2 = thresh_2 - 5;
                          else
                            thresh_2 = 5 ;                         
                          };break; 
                          
        case key_up1   :  {      
                          if (thresh_1 <100 )
                            thresh_1 = thresh_1 + 5;
                          else
                            thresh_1 = 100;                         
                          };break;
	  
        case key_down1 : {      
                          if (thresh_1 > 5 )
                            thresh_1 = thresh_1 - 5;
                          else
                            thresh_1 = 5 ;                         
                          };break;                               
        case key_warning : {
                         waring_sw = ~waring_sw   ;      
                          };break;   
	}      
    
    OLED_ShowNum(32,16,thresh_1,3,16  );        //显示阈值1
    OLED_ShowNum(32,32,thresh_2,3,16  );        //显示阈值2
    OLED_ShowString(48,4,data_display_reg,12);  //显示锁存的字符值  
    
    if(waring_sw == 0xFF)  
    {
      OLED_ShowString(90,36, "ON " ,12);
      DeviceState |= 0x01;   //拉高状态寄存器第0位
    }
    else           
    {
      OLED_ShowString(90,36, "OFF" ,12);
      DeviceState &= 0xfe;   //拉低状态寄存器第0位
    } 
     key_value_reg = key_value;        //锁存本次按键检测值
    
     //判断模拟二选一开关的选择位置
     if((P1IN &0x40) == 0)  //为0则说明USB连接线没有插入，此时将开关播到单片机的串口管脚
       P1OUT = P1OUT&0xDF ;
     else                   //否则说明有连接线插入，将模拟开关切换到CH340输入管脚
       P1OUT = P1OUT|0X20;

//延迟，并且根据阈值大小点亮LED或者鸣叫蜂鸣器
   
    thresh_2_value_H = (float)(100+thresh_2)/100*res_save;
    thresh_2_value_L = (float)(100-thresh_2)/100*res_save;
    
    thresh_1_value_H = (float)(100+thresh_1)/100*res_save; 
    thresh_1_value_L = (float)(100-thresh_1)/100*res_save; 
    
    if (( (thresh_1_value_L > res_now)||(thresh_1_value_H < res_now)  )&&(waring_sw == 0xFF))      {  
       P5OUT |= 0x10;   //点亮LED灯
       DeviceState = DeviceState |0x02;
    } else {
       P5OUT &= 0xEF;
       DeviceState = DeviceState &0xfD;
    }
    
   if (( (thresh_2_value_L > res_now)||(thresh_2_value_H < res_now)  )&&(waring_sw == 0xFF))      {  
       for(int i= 0; i<150; i++ ) {
        delay_us(150);
        P5OUT = P5OUT | 0x08;   
        delay_us(150);
        P5OUT = P5OUT & 0xF7 ;} 
        DeviceState = DeviceState |0x04;
    }else
    {
      DeviceState = DeviceState &0xfB;
      P5OUT = P5OUT & 0xF7 ;
      delay_ms(30);
    }
   
     strcopy(dataToSend.dataInit,data_display_reg,9);
     strcopy(dataToSend.dataNow,data_display,9);
     strcopy(dataToSend.header,packHeader,2);
     strcopy(dataToSend.ender,packEnder,2);
     dataToSend.thread1 = thresh_1;
     dataToSend.thread2 = thresh_2;
     dataToSend.deviceState = DeviceState;
     

     SendData( (unsigned char*) &dataToSend ,25 );
     //发送完成后，拉低命令握手回复位
     DeviceState = DeviceState&0xF7;          //拉低第3位
     
  }

}
