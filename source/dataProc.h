#ifndef _DATAPROC_H
#define _DATAPROC_H

#include <msp430.h>
#include "GC7721.h"
#include "key.h"

class DataProc
{
private:
    /* data */
    //缓存GC7721的原始数据值
    unsigned char GC7721FrameData_[DMMDATALEN];
    //报警功能开启标志
    unsigned char WarningSW_; 
    //缓存需要在屏幕显示的字符串
    unsigned char data_display[13];
    //存储计算出的数据变量
    float res_cal;
    //记录需要显示的初始电阻值
    unsigned char initRes_display[13];
    //存储初始电阻值
    float res_init;
    //方法间耦合变量变量
    unsigned char num [4] ;
    unsigned char dp[3] ;
    unsigned char unit[2] ;
    //高低阈值
    unsigned char thresh_1;
    unsigned char thresh_2;
    //根据获取到的按键或者云端指令，修改阈值
    void ManageThresh(unsigned char);
    //根据按键设置是否开关报警功能
    void ManageSW(unsigned char);
    //记录需要显示的电阻值字符串
    void RecordResDisplay();
    //记录初始电阻值
    void RecordResValue();
    //一帧数据接收完成标志
    unsigned char dataReady;
public:
    //需要设置初始显示的电阻值字符串，以及初始电阻值
    DataProc(unsigned char * );
    ~DataProc();
    //读取按键值
    void GetKeyValue( unsigned char );
    //获取缓冲区数据,传入7721帧数据缓冲区指针，并将显示的数据保存在本类的缓冲区中
    void GetGC7721Frame( const unsigned char * );
    //将GC7721数据转换成显示字符串,存入data_display数组
    unsigned char * GetDisplayString();
    //返回需要显示的init栏显示的电阻数值字符串指针
    unsigned char * GetRecordDisplayString();
    //返回高低两个阈值
    unsigned char GetThread1Value();
    unsigned char GetThread2Value();

    //返回报警功能是否开关
    const char * GetWarningSW();

    //将GC7721传来的数据计算成十进制
   // float GetDecRes(const unsigned char *num , const unsigned char *dp );
     float GetDecRes();
    //产生是否报警的标志
    //返回值bit[0]  1：需要LED报警：   返回值：0不需要LED报警
    //返回值bit[1]  1: 需要蜂鸣器报警； 返回值：0不需要蜂鸣器报警
    unsigned char WarningFlag();

};

#endif