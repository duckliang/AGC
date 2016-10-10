#ifndef _UART_H_
#define _UART_H_
#include "stc12c5a60s2.h"
#include <intrins.h>
#include <string.h>
#define FOSC     11059200UL			 /*使用22.1184M晶体*/	 
#define Baud   9600UL		                 /*波特率定义为9600*/

#define  True              1
#define  False             0
#define  Rs232_Input_LEN   16
#define  Rs232_Output_LEN  28
#define  Rx_start_data     0xF5
#define  TX_CRC			   Rs232_Output_LEN-1
#define  TX_NUM            3
#define  TX_list           21
#define  target            1
#define  Rx_end_data	  Rx_CRC()  
/**************************/
/*******串口接收设置*******/
extern bit Rs232_Rece_Error,Start_Rxd,Rs232_Flag;   
extern unsigned char  Rs_buf[Rs232_Input_LEN],Rs_buf1[Rs232_Input_LEN],Rs_buf2[Rs232_Input_LEN],g_cReceNum,ZZ,buf_num;   
extern unsigned int   Rs_bufsum;
extern unsigned char a,k,zifuchuan[];
extern unsigned char  xintiao[];
extern volatile unsigned char sending,taking;
extern unsigned char targetN,startN;
void UARTinit(void);
void UARTsend(unsigned char d);
void UARTsendc(unsigned char * str);
void UARTsendc1(unsigned char * str);
unsigned char Rx_CRC();
unsigned char Tx_CRC(unsigned char Txarray[Rs232_Output_LEN]);

#endif
