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
  BCSCTL1 &= ~XT2OFF;           //ʹ���ⲿ8M����
  BCSCTL2 |= (SELM_2 + SELS + DIVM_0 + DIVS_3 ) ; //��ʱ��Ϊ�ⲿ8M���� mclk �� smclk ������Ϊ1M
  do 
  {
    IFG1 &= ~OFIFG;
    for( i=0 ;i<100;i++ );
  }
  while ( (IFG1&OFIFG) != 0 );
}

void led_init()
{
  //��������LED��IO�ڿ���
  P5DIR = 0XFF;
  P5OUT = 0x00;
  
  //fs9721��Ӧ�ܽſ���
  P2DIR = 0XFF;
  P2OUT = 0X03;     //����TYPE_COM �ܽţ�ʹ9721�л�����������ģʽ
  delay_ms(100);
  P2OUT = 0X01;     //����TYPE_COM �ܽţ�ʹ9721���»ص��������ģʽ
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


void main(void)
{

  unsigned char  thresh_2 = 20 ;
  unsigned char  thresh_1 = 10;
 
  int key_value = 0 ;       //��⵱ǰ����ֵ
  int key_value_reg = 0 ;   //������һ�ΰ���ֵ
  int key_effect = 0 ;      //��ñ��ؼ������Чֵ
  char waring_sw = 0x00 ;   //�������ܿ���
  
  unsigned char data_display_reg[13] = { '1','0','0','0','K',' ',' ',' ',' ',' ',' ',' ','\0' };
  
  float res_save = 1000 ;     //Ĭ�ϳ�ʼ����ֵΪ1M
  float  res_now = 1000 ;     //��¼��ǰ����ֵ
  
  float thresh_2_value_H = 0;
  float thresh_2_value_L = 0;
  
  float thresh_1_value_H = 0;
  float thresh_1_value_L = 0;
  
  unsigned char packHeader[2] = {0x55,0xAA};
  unsigned char packEnder[2] = {0xAA,0X55};
  
  unsigned char  DeviceState = 0x00 ;  //�豸״̬�Ĵ���  ��0λ�����������Ƿ�򿪣���1λ����ֵ1�������أ���2λ����ֵ2�������أ���3λ��������ջظ�����
  
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

  OLED_Init();	//��ʼ��oled
  analogSWinit();
  OLED_ShowString(0,0, "Init:" ,16); 
  
  OLED_ShowString(0,16, "Th1:" ,16);        //��ʾ�̶��ַ�
  OLED_ShowChar(56,16,'%',16,1);

  OLED_ShowString(0,32, "Th2:" ,16);        //��ʾ�̶��ַ�
  OLED_ShowChar(56,32,'%',16,1);  
  
  OLED_ShowString(80,20, "WarnSW:" ,12);
  
  OLED_Refresh_Gram();       //ˢ����Ļ��ʾ
  
  InitUART1();                //��ʼ����
//   _EINT();
  InitUART0();
  while(1)  {
 
//���ݴ�����ʾˢ��    
    if(Data_Ready)  
     {  
      IE2 = 0X0;                                // �ر�USART1�Ľ����ж� 
      
      res_now = data_proc (  rdat ,14, num_deci );
 
      OLED_ShowString(0,48,data_display,16);
      
      OLED_Refresh_Gram();                     //ˢ����Ļ��ʾ
      
      IE2 |= URXIE1;                           // ʹ��USART1�Ľ����ж�    
    }  
    
    
//����Ƿ����ƶ˷�����������   
    if(Data_Ready_0)
    {
      IE1 = 0X0;                                // �ر�USART0�Ľ����ж� 
      switch( CloudCmd )
      {
        case cloud_hold  :  { res_save = res_now;                   //���浱ǰ����ֵ
                            for(int i=0;i<12;i++)
                            data_display_reg[i] = data_display[i];     //������ʾ�����ַ�ֵ
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
      
      DeviceState = DeviceState|0x08;          //���ߵ�3λ
      Data_Ready_0 = 0;
      IE1 |= URXIE0;                           // ʹ��USART0�Ľ����ж�  
    
    }
    
//����ֵ�жϣ����ݰ���ֵ�޸���ֵ����    
   
    key_value = key_detect();         //��õ�ǰ����ֵ
    key_effect = key_value & (~ key_value_reg ) & 0x3F ;    //��ȡ��Ч����ֵ  
    
    switch( key_effect )
      {
        case key_hold  :  { res_save = res_now;                   //���浱ǰ����ֵ
                            for(int i=0;i<12;i++)
                            data_display_reg[i] = data_display[i];     //������ʾ�����ַ�ֵ
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
    
    OLED_ShowNum(32,16,thresh_1,3,16  );        //��ʾ��ֵ1
    OLED_ShowNum(32,32,thresh_2,3,16  );        //��ʾ��ֵ2
    OLED_ShowString(48,4,data_display_reg,12);  //��ʾ������ַ�ֵ  
    
    if(waring_sw == 0xFF)  
    {
      OLED_ShowString(90,36, "ON " ,12);
      DeviceState |= 0x01;   //����״̬�Ĵ�����0λ
    }
    else           
    {
      OLED_ShowString(90,36, "OFF" ,12);
      DeviceState &= 0xfe;   //����״̬�Ĵ�����0λ
    } 
     key_value_reg = key_value;        //���汾�ΰ������ֵ
    
     //�ж�ģ���ѡһ���ص�ѡ��λ��
     if((P1IN &0x40) == 0)  //Ϊ0��˵��USB������û�в��룬��ʱ�����ز�����Ƭ���Ĵ��ڹܽ�
       P1OUT = P1OUT&0xDF ;
     else                   //����˵���������߲��룬��ģ�⿪���л���CH340����ܽ�
       P1OUT = P1OUT|0X20;

//�ӳ٣����Ҹ�����ֵ��С����LED�������з�����
   
    thresh_2_value_H = (float)(100+thresh_2)/100*res_save;
    thresh_2_value_L = (float)(100-thresh_2)/100*res_save;
    
    thresh_1_value_H = (float)(100+thresh_1)/100*res_save; 
    thresh_1_value_L = (float)(100-thresh_1)/100*res_save; 
    
    if (( (thresh_1_value_L > res_now)||(thresh_1_value_H < res_now)  )&&(waring_sw == 0xFF))      {  
       P5OUT |= 0x10;   //����LED��
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
     //������ɺ������������ֻظ�λ
     DeviceState = DeviceState&0xF7;          //���͵�3λ
     
  }

}
