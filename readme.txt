在工程中添加.C文件
使用方法：在主函数C文件里写上头文件#inculde "UART.h"
          在主函数C文件里UARTinit();）;即可初始化串口
主函数前：unsigned char zifuchuan[]="天津工业大学理学院!";//待显示字符。
          volatile unsigned char sending,taking,recieve;


注意：本串口使用定时器1，工作方式为2自动重装载8位初值给串口提供波特率
      这里串口在工作方式1；smod=1，波特率加倍；波特率在头文件中的宏定义改变

发送函数：
       直接调用UARTsend（）；或UARTsendc（）；
接收函数：
       在中断中将接收标志位taking=1；并recieve=SBUF;但此接收仅一个字符。
      