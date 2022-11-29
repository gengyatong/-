#include "msp430x14x.h"
#include "data_proc.h "
//#include "uart.h"

unsigned char num_deci [4] = {0,0,0,0 };
unsigned char num_deci_char [4] = { 0,0,0,0};

unsigned char dp[3] = {0,0,0};
unsigned char dp_char[3] = {0,0,0};

unsigned char unit[2] = {0,0};
unsigned char unit_char[2] = {0,0};

unsigned char data_display[12] = {0,0,0,0,0,0,0,0,0,0,0,0} ;


 float data_proc (  unsigned char *data_in, unsigned char len ,unsigned char *num )
{
  char i , j = 0 ;
  unsigned char temp[4] = {0,0,0,0};
   float res_cal = 0;          //��������ʾ���ַ��͵���ֵת��Ϊʮ�����ַ�
  
  unsigned char data_shadow[14];
  for( i=0;i<14;i++ )
    data_shadow[i] = data_in[i];
    
  for(j=0; j<len; j++)
 	  data_shadow[j]&=0x0F;
 	  
    //�����յ�������ת��Ϊ8���������ʾ���ַ�
 	temp[0]=(((data_shadow[1]&0x01)<<6)|(data_shadow[1]&0x02)|(data_shadow[1]&0x04)|                                        //��������λ
	    ((data_shadow[2]&0x01)<<5)|((data_shadow[2]&0x02)>>1)|((data_shadow[2]&0x04)<<2)|(data_shadow[2]&0x08));

	temp[1]=(((data_shadow[3]&0x01)<<6)|(data_shadow[3]&0x02)|(data_shadow[3]&0x04)|                  
		((data_shadow[4]&0x01)<<5)|((data_shadow[4]&0x02)>>1)|((data_shadow[4]&0x04)<<2)|(data_shadow[4]&0x08));

	temp[2]=(((data_shadow[5]&0x01)<<6)|(data_shadow[5]&0x02)|(data_shadow[5]&0x04)|
		((data_shadow[6]&0x01)<<5)|((data_shadow[6]&0x02)>>1)|((data_shadow[6]&0x04)<<2)|(data_shadow[6]&0x08));

	temp[3]=(((data_shadow[7]&0x01)<<6)|(data_shadow[7]&0x02)|(data_shadow[7]&0x04)|                                        //��������λ
		((data_shadow[8]&0x01)<<5)|((data_shadow[8]&0x02)>>1)|((data_shadow[8]&0x04)<<2)|(data_shadow[8]&0x08));      
    
    //���������ʾ���ַ�ת��Ϊ10�����ַ�
	for(j=0; j<4; j++)
	{
		switch(temp[j])
		{
                case 0x30 : {num[j]=1;  num_deci_char[j] = '1';}   ;break;
                case 0x6d : {num[j]=2;  num_deci_char[j] = '2';}   ;break;
                case 0x79 : {num[j]=3;  num_deci_char[j] = '3';}   ;break;
                case 0x33 : {num[j]=4;  num_deci_char[j] = '4';}   ;break;
                case 0x5B : {num[j]=5;  num_deci_char[j] = '5';}   ;break;
                case 0x5F : {num[j]=6;  num_deci_char[j] = '6';}   ;break;
                case 0x70 : {num[j]=7;  num_deci_char[j] = '7';}   ;break;
                case 0x7F : {num[j]=8;  num_deci_char[j] = '8';}   ;break;
                case 0x7B : {num[j]=9;  num_deci_char[j] = '9';}   ;break;
                case 0x7E : {num[j]=0;  num_deci_char[j] = '0';}   ;break;
                case 0x0E : {num[j]=10; num_deci_char[j] = 'L';}   ;break;       //������ַ�Ϊ10�����������������󣬳�������
                default : { num_deci_char[j] = ' ';}               ;break;
		}
	}
    //ȷ���ĸ�С����Ҫ��ʾ,dp�Ķ�ӦλΪ1����˵����С������Ҫ��ʾ    

          if( data_in[3]&0x8 )                                                                    //���λС����
          {
            dp[0] = 1 ;
            dp_char[0] = '.';
          }
          else 
          {
            dp[0] = 0 ;
            dp_char[0] = ' ';
          }
          if(data_in[5]&0x8 )
          {
            dp[1] = 1 ;
            dp_char[1] = '.';
          }
          else
          {
            dp[1] = 0 ;
            dp_char[1] = ' ';
          }
          if(data_in[7]&0x8)                                                                      //���λС����
          { 
            dp[2] = 1 ;
            dp_char[2] = '.';
          }
          else
          {
            dp[2] = 0 ;
            dp_char[2] = ' ' ;      
          }
   //ȷ��������ʾ�ĵ�λ��K /M    

          if( data_in[9]&0x2 )                                                                    
          {
            unit[0] = 1 ;         //unit[0]��Ӧ Kŷķ  unit[1] ��ӦMŷķ
            unit_char[0] = 'K';
          }
          else 
          {
            unit[0] = 0 ;
            unit_char[0] = ' ';
          }
          if (data_in[10]&0x2 )
          {  
            unit[1] = 1 ;
            unit_char[1] = 'M';
          }
          else
          {
            unit[1] = 0 ;
            unit_char[1] = ' ';
          }      
       data_display[0] = num_deci_char[0];
       data_display[1] = dp_char[0];
       data_display[2] = num_deci_char[1];   
       data_display[3] = dp_char[1];
       data_display[4] = num_deci_char[2]; 
       data_display[5] = dp_char[2]; 
       data_display[6] = num_deci_char[3];
       data_display[7] = unit_char[0];
       data_display[8] = unit_char[1];
       data_display[9] = 'o';
       data_display[10]= 'h';
       data_display[11] = 'm';   
       
//����ʾ�õ��ַ������ʮ���Ƶ���ֵ
       
       if( num[2] == 10 )
       {
         res_cal = 50000;       //����������������̵�ʱ�����ò��Ե���ֵΪ50_000K
 //        Data_Ready = 0 ;       //���һ�����ݴ�����Data_ready��0�� ʹ�������½�����һ������
         return res_cal;
       }
       
       //�����׺��λΪM
       if ( (unit[1]==1) && (dp[0] == 1) )           
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;        
       }
       
       else if ( (unit[1]==1) && (dp[1] == 1) )          //�����׺��λΪM,�ҵڶ�λС������Ч
       {
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
         res_cal = res_cal *10;
       } 
       else if ( (unit[1]==1) && (dp[2] == 1) )            //�����׺��λΪM,�ҵ���λС������Ч
       {
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
         res_cal = res_cal *100;
       
       }
       else if ( unit[1]==1 &&( (dp[0]|dp[1]|dp[2]) ==0) )             //�����׺��λΪK,û��С����
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;
         res_cal = res_cal*1000;
       }
       
       
       
       //�����׺��λΪK
       if ( (unit[0]==1) && ( dp[0] == 1 ) )               //�����׺��λΪK,�ҵ�һλС������Ч
      { res_cal = num[0]*1 + num[1]*0.1 + num[2]*0.01 + num[3]*0.001 ; }
       
       
       else if ( (unit[0]==1) && (dp[1] == 1 ) )         //�����׺��λΪK,�ҵڶ�λС������Ч
       { res_cal = num[0]*10 + num[1]*1 + num[2]*0.1 + num[3]*0.01 ; }
       
       else if ( (unit[0]==1) && (dp[2] == 1 ) )            //�����׺��λΪK,�ҵ���λС������Ч         
      
       { res_cal = num[0]*100 + num[1]*10 + num[2]*1 + num[3]*0.1 ; }
      
       else if ( unit[0]==1 &&( (dp[0]|dp[1]|dp[2])==0) )             //�����׺��λΪK,û��С����
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;
  
       }     
       
       //�����׺��λΪŷķ
       if ( ( (unit[0]| unit[1]) ==0 ) &&  dp[0] == 1 )            //�����׺��λΪŷķ,�ҵ�һλС������Ч
       {
          res_cal = num[0] ;
          res_cal = res_cal/1000; 
       }
       
       else if ( ( (unit[0]| unit[1]) ==0 ) && (dp[1] == 1) )         //�����׺��λΪŷķ,�ҵ�һλС������Ч
       {
         res_cal = num[0]*10 + num[1] ;
         res_cal = res_cal/1000;
       } 
       else if ( ( (unit[0]| unit[1]) ==0 ) && (dp[2] == 1) )            //�����׺��λΪŷķ,�ҵ�һλС������Ч
       {
         res_cal = num[0]*100 + num[1]*10 + num[2] ;
         res_cal = res_cal /1000;
       }
       else if ( (( unit[0]| unit[1]) ==0) &&  ( (dp[0]|dp[1]|dp[2])==0) )             //�����׺��λΪŷķ,�ҵ�һλС������Ч
       { 
          res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
          res_cal = res_cal/1000;
       }
       
    //   Data_Ready = 0 ;       //���һ�����ݴ�����Data_ready��0�� ʹ�������½�����һ������
       return res_cal;

}

