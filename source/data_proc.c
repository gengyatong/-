#include "msp430x14x.h"
#include "data_proc.h "
#include "uart.h"

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
   float res_cal = 0;          //将用于显示的字符型电阻值转换为十进制字符
  
  unsigned char data_shadow[14];
  for( i=0;i<14;i++ )
    data_shadow[i] = data_in[i];
    
  for(j=0; j<len; j++)
 	  data_shadow[j]&=0x0F;
 	  
    //将接收到的数据转换为8段数码管显示的字符
 	temp[0]=(((data_shadow[1]&0x01)<<6)|(data_shadow[1]&0x02)|(data_shadow[1]&0x04)|                                        //数码管最高位
	    ((data_shadow[2]&0x01)<<5)|((data_shadow[2]&0x02)>>1)|((data_shadow[2]&0x04)<<2)|(data_shadow[2]&0x08));

	temp[1]=(((data_shadow[3]&0x01)<<6)|(data_shadow[3]&0x02)|(data_shadow[3]&0x04)|                  
		((data_shadow[4]&0x01)<<5)|((data_shadow[4]&0x02)>>1)|((data_shadow[4]&0x04)<<2)|(data_shadow[4]&0x08));

	temp[2]=(((data_shadow[5]&0x01)<<6)|(data_shadow[5]&0x02)|(data_shadow[5]&0x04)|
		((data_shadow[6]&0x01)<<5)|((data_shadow[6]&0x02)>>1)|((data_shadow[6]&0x04)<<2)|(data_shadow[6]&0x08));

	temp[3]=(((data_shadow[7]&0x01)<<6)|(data_shadow[7]&0x02)|(data_shadow[7]&0x04)|                                        //数码管最低位
		((data_shadow[8]&0x01)<<5)|((data_shadow[8]&0x02)>>1)|((data_shadow[8]&0x04)<<2)|(data_shadow[8]&0x08));      
    
    //将数码管显示的字符转换为10进制字符
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
                case 0x0E : {num[j]=10; num_deci_char[j] = 'L';}   ;break;       //如果该字符为10，则表明电阻测量过大，超出量程
                default : { num_deci_char[j] = ' ';}               ;break;
		}
	}
    //确定哪个小数点要显示,dp的对应位为1，则说明该小数点需要显示    

          if( data_in[3]&0x8 )                                                                    //最高位小数点
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
          if(data_in[7]&0x8)                                                                      //最低位小数点
          { 
            dp[2] = 1 ;
            dp_char[2] = '.';
          }
          else
          {
            dp[2] = 0 ;
            dp_char[2] = ' ' ;      
          }
   //确定电阻显示的单位，K /M    

          if( data_in[9]&0x2 )                                                                    
          {
            unit[0] = 1 ;         //unit[0]对应 K欧姆  unit[1] 对应M欧姆
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
       
//将显示用的字符换算成十进制电阻值
       
       if( num[2] == 10 )
       {
         res_cal = 50000;       //当超出电阻测量量程的时候，设置测试电阻值为50_000K
         Data_Ready = 0 ;       //完成一次数据处理后将Data_ready置0， 使串口重新接收下一次数据
         return res_cal;
       }
       
       //如果后缀单位为M
       if ( (unit[1]==1) && (dp[0] == 1) )           
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;        
       }
       
       else if ( (unit[1]==1) && (dp[1] == 1) )          //如果后缀单位为M,且第二位小数点有效
       {
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
         res_cal = res_cal *10;
       } 
       else if ( (unit[1]==1) && (dp[2] == 1) )            //如果后缀单位为M,且第三位小数点有效
       {
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
         res_cal = res_cal *100;
       
       }
       else if ( unit[1]==1 &&( (dp[0]|dp[1]|dp[2]) ==0) )             //如果后缀单位为K,没有小数点
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;
         res_cal = res_cal*1000;
       }
       
       
       
       //如果后缀单位为K
       if ( (unit[0]==1) && ( dp[0] == 1 ) )               //如果后缀单位为K,且第一位小数点有效
      { res_cal = num[0]*1 + num[1]*0.1 + num[2]*0.01 + num[3]*0.001 ; }
       
       
       else if ( (unit[0]==1) && (dp[1] == 1 ) )         //如果后缀单位为K,且第二位小数点有效
       { res_cal = num[0]*10 + num[1]*1 + num[2]*0.1 + num[3]*0.01 ; }
       
       else if ( (unit[0]==1) && (dp[2] == 1 ) )            //如果后缀单位为K,且第三位小数点有效         
      
       { res_cal = num[0]*100 + num[1]*10 + num[2]*1 + num[3]*0.1 ; }
      
       else if ( unit[0]==1 &&( (dp[0]|dp[1]|dp[2])==0) )             //如果后缀单位为K,没有小数点
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;
  
       }     
       
       //如果后缀单位为欧姆
       if ( ( (unit[0]| unit[1]) ==0 ) &&  dp[0] == 1 )            //如果后缀单位为欧姆,且第一位小数点有效
       {
          res_cal = num[0] ;
          res_cal = res_cal/1000; 
       }
       
       else if ( ( (unit[0]| unit[1]) ==0 ) && (dp[1] == 1) )         //如果后缀单位为欧姆,且第一位小数点有效
       {
         res_cal = num[0]*10 + num[1] ;
         res_cal = res_cal/1000;
       } 
       else if ( ( (unit[0]| unit[1]) ==0 ) && (dp[2] == 1) )            //如果后缀单位为欧姆,且第一位小数点有效
       {
         res_cal = num[0]*100 + num[1]*10 + num[2] ;
         res_cal = res_cal /1000;
       }
       else if ( (( unit[0]| unit[1]) ==0) &&  ( (dp[0]|dp[1]|dp[2])==0) )             //如果后缀单位为欧姆,且第一位小数点有效
       { 
          res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
          res_cal = res_cal/1000;
       }
       
       Data_Ready = 0 ;       //完成一次数据处理后将Data_ready置0， 使串口重新接收下一次数据
       return res_cal;

}

