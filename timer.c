#include "timer.h"
void Timer0Init()
{
    TMOD = 0x01;                    //设置定时器0为工作方式1（M1M0为01）
    TL0 = (65536-(Time0*Fosc/12000000))%256;                       
    TH0 = (65536-(Time0*Fosc/12000000))/256;              
	EA = 1;                         //开总中断
    ET0 = 1;                        //开定时器0中断
    TR0 = 1;                        //启动定时器0                  
}