#include "dataProc.h"

dataProc::dataProc(unsigned char * initDisplay ,float resInit ) 
{
  //默认报警功能开关开启
  WarningSW_ = 0XFF;
  //默认初始电阻值为1000(K)
  res_init = resInit;
  //初始化显示字符串
  for(int i=0;i<13;i++)
  {
    initRes_display[i] = initDisplay[i];
  }
}

dataProc::~dataProc()
{
}
//获取GC7721帧数据
void dataProc:: GetGC7721Frame( const unsigned char * frameData )
{
    //如果指针不为0，则说明缓冲区存满了一帧数据，这时候要赋值到本地存储区
    if(frameData!=NULL)
        {
        //在赋值数据的时候，需要关闭GC7721对应的串口中断，以免发生数据不完整  
        IE2 &= ~URXIE1; 
          for(int i = 0 ;i<14;i++)
          {
            GC7721FrameData_[i] = frameData[i];
          }
        //复制完成后，重新打开中断
        IE2 |= URXIE1;
        }    
      else
      {
        for(int i = 0 ;i<14;i++)
          {
            GC7721FrameData_[i] = 0;
          }
      }
}
//获取按键键值
void dataProc:: GetKeyValue( unsigned char keyValue)
{
    ManageThresh(keyValue);
    ManageSW(keyValue);
    //如果按键为保持功能
    if(keyValue==key_hold)
    {
      RecordResDisplay();
      RecordResValue();
    }
}
//获取显示数据 dataIn  7721传入的数据
unsigned char * dataProc:: GetDisplayString(const unsigned char *dataIn)
{
  unsigned char temp[4]           = {0,0,0,0};
  unsigned char num_deci_char [4] = { 0,0,0,0};
  unsigned char dp_char[3]        = {0,0,0};
  unsigned char unit_char[2]  = {0,0};
  //将GC7721发送的数码管数据恢复成可以识别的数据
  temp[0]=(((dataIn[1]&0x01)<<6)|(dataIn[1]&0x02)|(dataIn[1]&0x04)|                                        //��������λ
	    ((dataIn[2]&0x01)<<5)|((dataIn[2]&0x02)>>1)|((dataIn[2]&0x04)<<2)|(dataIn[2]&0x08));

	temp[1]=(((dataIn[3]&0x01)<<6)|(dataIn[3]&0x02)|(dataIn[3]&0x04)|                  
		((dataIn[4]&0x01)<<5)|((dataIn[4]&0x02)>>1)|((dataIn[4]&0x04)<<2)|(dataIn[4]&0x08));

	temp[2]=(((dataIn[5]&0x01)<<6)|(dataIn[5]&0x02)|(dataIn[5]&0x04)|
		((dataIn[6]&0x01)<<5)|((dataIn[6]&0x02)>>1)|((dataIn[6]&0x04)<<2)|(dataIn[6]&0x08));

	temp[3]=(((dataIn[7]&0x01)<<6)|(dataIn[7]&0x02)|(dataIn[7]&0x04)|                                        //��������λ
		((dataIn[8]&0x01)<<5)|((dataIn[8]&0x02)>>1)|((dataIn[8]&0x04)<<2)|(dataIn[8]&0x08));      
    
	for(int j=0; j<4; j++)
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
                case 0x0E : {num[j]=10; num_deci_char[j] = 'L';}   ;break;       
                default : { num_deci_char[j] = ' ';}               ;break;
		}
	}   
//查看是否存在小数点，及小数点位置
          if( dataIn[3]&0x8 )                                                                   
          {
            dp[0] = 1 ;
            dp_char[0] = '.';
          }
          else 
          {
            dp[0] = 0 ;
            dp_char[0] = ' ';
          }
          if(dataIn[5]&0x8 )
          {
            dp[1] = 1 ;
            dp_char[1] = '.';
          }
          else
          {
            dp[1] = 0 ;
            dp_char[1] = ' ';
          }
          if(dataIn[7]&0x8)                                                                      //���λС����
          { 
            dp[2] = 1 ;
            dp_char[2] = '.';
          }
          else
          {
            dp[2] = 0 ;
            dp_char[2] = ' ' ;      
          }
   
//查看单位（K/M）
          if( dataIn[9]&0x2 )                                                                    
          {
            unit[0] = 1 ;         
            unit_char[0] = 'K';
          }
          else 
          {
            unit[0] = 0 ;
            unit_char[0] = ' ';
          }
          if (dataIn[10]&0x2 )
          {  
            unit[1] = 1 ;
            unit_char[1] = 'M';
          }
          else
          {
            unit[1] = 0 ;
            unit_char[1] = ' ';
          }     
      //保存要显示的字符串     
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
       data_display[12] = '\0'; 
       return data_display;
}
//返回需要显示的init栏显示的电阻数值字符串指针
unsigned char * dataProc:: GetRecordDisplayString()
{
  return initRes_display;
}
//获取检测到的电阻值  num：数字部分  dp 小数点部分
float dataProc:: GetDecRes(const unsigned char *num , const unsigned char *dp )
{  
       if( num[2] == 10 )
       {
         res_cal = 50000;      //如果出现了0.L，则认为超载，直接复制50M 
         return res_cal;
       }
       
       //计算单位为M 欧姆时的阻值（计算结果以K为单位）
       if ( (unit[1]==1) && (dp[0] == 1) )           
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;        
       }
       
       else if ( (unit[1]==1) && (dp[1] == 1) )          
       {
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
         res_cal = res_cal *10;
       } 
       else if ( (unit[1]==1) && (dp[2] == 1) )            
       {
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
         res_cal = res_cal *100;
       
       }
       else if ( unit[1]==1 &&( (dp[0]|dp[1]|dp[2]) ==0) )             
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;
         res_cal = res_cal*1000;
       }
              
       //计算单位为K 欧姆时的阻值（计算结果以K为单位）
       if ( (unit[0]==1) && ( dp[0] == 1 ) )              
      { res_cal = num[0]*1 + num[1]*0.1 + num[2]*0.01 + num[3]*0.001 ; }
       
       
       else if ( (unit[0]==1) && (dp[1] == 1 ) )         
       { res_cal = num[0]*10 + num[1]*1 + num[2]*0.1 + num[3]*0.01 ; }
       
       else if ( (unit[0]==1) && (dp[2] == 1 ) )                   
      
       { res_cal = num[0]*100 + num[1]*10 + num[2]*1 + num[3]*0.1 ; }
      
       else if ( unit[0]==1 &&( (dp[0]|dp[1]|dp[2])==0) )             
       { 
         res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ;
  
       }     
       
       ////计算单位为欧姆时的阻值（计算结果以K为单位）
       if ( ( (unit[0]| unit[1]) ==0 ) &&  dp[0] == 1 )            
       {
          res_cal = num[0] ;
          res_cal = res_cal/1000; 
       }
       
       else if ( ( (unit[0]| unit[1]) ==0 ) && (dp[1] == 1) )         
       {
         res_cal = num[0]*10 + num[1] ;
         res_cal = res_cal/1000;
       } 
       else if ( ( (unit[0]| unit[1]) ==0 ) && (dp[2] == 1) )            
       {
         res_cal = num[0]*100 + num[1]*10 + num[2] ;
         res_cal = res_cal /1000;
       }
       else if ( (( unit[0]| unit[1]) ==0) &&  ( (dp[0]|dp[1]|dp[2])==0) )             
       { 
          res_cal = num[0]*1000 + num[1]*100 + num[2]*10 + num[3] ; 
          res_cal = res_cal/1000;
       }
      return res_cal;
}
//产生是否报警的标志，
//返回值bit[0]  1：需要LED报警：   返回值：0不需要LED报警
//返回值bit[1]  1: 需要蜂鸣器报警； 返回值：0不需要蜂鸣器报警
 unsigned char dataProc:: WarningFlag()
 {   
  unsigned char  WarningFlag;
  float thresh_2_value_H;
  float thresh_2_value_L;
  float thresh_1_value_H;
  float thresh_1_value_L;
  //如果报警开关没有打开，则不会报警
  if(WarningSW_ == 0X00)
    {
      WarningFlag = 0x00;
      return WarningFlag;
    } 
  else
  {
  thresh_2_value_H = (float)(100+thresh_2)/100*res_init;
  thresh_2_value_L = (float)(100-thresh_2)/100*res_init;
  thresh_1_value_H = (float)(100+thresh_1)/100*res_init; 
  thresh_1_value_L = (float)(100-thresh_1)/100*res_init; 

  if (((thresh_1_value_L > res_cal)||(thresh_1_value_H < res_cal))&&(res_cal != 50000) )  
    WarningFlag = WarningFlag |0x01;      
  if (((thresh_2_value_L > res_cal)||(thresh_2_value_H < res_cal))&&(res_cal != 50000))      
    WarningFlag = WarningFlag |0x10;
  }
  return WarningFlag;   
 }
//管理阈值
void dataProc::ManageThresh(unsigned char KeyValue)
{
   switch( KeyValue )
      {
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
	}  



}
//管理报警阈值标志
void dataProc::ManageSW(unsigned char KeyValue)
{
  if(KeyValue == key_warning)
      WarningSW_ = ~ WarningSW_;
}
//更新初始电阻值其显示字符串
void dataProc::RecordResDisplay()
{
  for(int i=0;i<13;i++)
    {
      initRes_display[i] = data_display[i];
    }
}
//更新初始电阻值
void dataProc:: RecordResValue()
{
  res_init = res_cal;
}
