#include <STC12C5A60S2.H>
#include "UART.h"
#include "timer.h"
#include<intrins.h>					  //nop延时头文件，在延时函数中用到，调用一个_nop_()代表延时1us。
#include "serial2.c"
unsigned char targetN=0,startN,listN;
volatile unsigned char sending,taking;
unsigned char    ZZ=0X01,tt=0;
unsigned char    xintiao[Rs232_Output_LEN]= {0XFA,0X00,0X1C,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XEA};
unsigned char   lianxian1[Rs232_Output_LEN]={0xFA,0x01,0x1C,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7};
unsigned char   lianxian2[Rs232_Output_LEN]={0xFA,0x02,0x1C,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE6};
unsigned char renwuxiada[Rs232_Output_LEN]= {0XFA,0X01,0X1C,0X00,0X02,0X01,0X0B,0X0C,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X7B};
unsigned char renwuqidon[Rs232_Output_LEN]= {0XFA,0X01,0X1C,0X00,0X02,0X01,0X0B,0X0C,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X03,0X00,0X00,0X00,0X00,0X00,0X00,0X63};
unsigned char      qidon[Rs232_Output_LEN]= {0XFA,0X01,0X1C,0X00,0X03,0X01,0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
unsigned char     qidon1[Rs232_Output_LEN]= {0XFA,0X01,0X1C,0X00,0X03,0X01,0X01,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
unsigned char     qidon2[Rs232_Output_LEN]= {0XFA,0X02,0X1C,0X00,0X03,0X01,0X01,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
unsigned char    tingzhi[Rs232_Output_LEN]= {0XFA,0X01,0X1C,0X00,0X03,0X01,0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};
sbit led0=P1^0;
sbit led1=P1^1;
void aim_array(uchar w1,uchar w2,uchar w3,uchar w4)
{ 
   renwuxiada[6]=w1;
   renwuqidon[6]=w1;
   renwuxiada[7]=w2;
   renwuqidon[7]=w2;
   renwuxiada[8]=w3;
   renwuqidon[8]=w3;
   renwuxiada[9]=w4;
   renwuqidon[9]=w4;
}
void delay(unsigned int i)
{
	unsigned char j,k;
	for(j=i;j>0;j--)
		for(k=255;k>0;k--);
}
void NVIC_init(viod)
{
IP=0X00;
IPH=0X10;
IP2=0X01;
IPH2=0X00;
}
void main()
{	NVIC_init();
  	Timer0Init();
	UARTinit();
    init_uart();				 
	while(1)
	{	 
        if(P0!=0XFF||P2!=0XFF||led0==0)
          {
		    led1=0;
		  }
		  else 
		  {led1=1;}
         if (Rs232_Flag)
          {  
		         Rs232_Flag = 0;
				  if(Rs_buf1[6]==1)
				  {targetN=1;}
				  else if(Rs_buf2[6]==1)
				  {targetN=2;}

				  if((Rs_buf1[7]==0x30)||(Rs_buf1[7]==0x31));
				  else if((Rs_buf1[7]==0x10)||(Rs_buf1[7]==0x11));
				  else
				  {
						  if(Rs_buf1[9]!=0)//1号车管制
						 {
						 	if(Rs_buf2[9]==0){qidon1[6]=Rs_buf1[9];UARTsendc1(qidon1);}
							else
							{
							  if(Rs_buf1[9]!=Rs_buf2[9])
							  {qidon1[6]=Rs_buf1[9];UARTsendc1(qidon1);}
							}			 
						 }			 			
	              }

				  if((Rs_buf2[7]==0x30)||(Rs_buf2[7]==0x31));
				  else if((Rs_buf2[7]==0x10)||(Rs_buf2[7]==0x11));
				  else
				  {
						 if(Rs_buf2[9]!=0)//2号车管制
						 {
						 	if(Rs_buf1[9]==0){qidon2[6]=Rs_buf2[9];UARTsendc1(qidon2);}
							else
							{
							  if(Rs_buf2[9]!=Rs_buf1[9])
							  {qidon2[6]=Rs_buf2[9];UARTsendc1(qidon2);}
					        }			  
				         }
		           }
			}
			 
	   while(flag_finish)		   //当接收完成时
	   {
		   command();			   //识别相应的功能码
		   flag_finish=0;		   //重新接收	
	   }
	   if(sign!=0)
	   {
	          switch(sign)
			   {
			     case 1: aim_array(2,11,12,1);  break;
				 case 2: aim_array(3,11,12,1);  break;
				 case 3: aim_array(4,11,12,1);  break;
				 case 4: aim_array(2,11,12,1);  break;
				 case 5: aim_array(3,11,12,1);  break;
				 case 6: aim_array(4,11,12,1);  break;
				 case 7: aim_array(5,11,12,1);  break;
				 case 8: aim_array(6,11,12,1);  break;
				 case 9: aim_array(7,11,12,1);  break;
				 case 10: aim_array(5,11,12,1);  break;
				 case 11: aim_array(6,11,12,1);  break;
				 case 12: aim_array(7,11,12,1);  break;
				 case 13: aim_array(8,11,12,1);  break;
				 case 14: aim_array(9,11,12,1);  break;
				 case 15: aim_array(10,11,12,1);  break;
				 case 16: aim_array(9,11,12,1);  break;
				 case 17: aim_array(10,11,12,1);  break;
			   }
			  	sign=0;	
				UARTsendc(renwuxiada);
								delay_ms(500);	
				UARTsendc(renwuqidon);	
				                delay_ms(500);	
				UARTsendc(renwuxiada);
								delay_ms(500);	
				UARTsendc(renwuqidon);		  
	    }
	  
	  
	}
}
void tm0_isr() interrupt 1 	//中断服务部分
{
    TL0 =(65536-(Time0*Fosc/12000000))%256;                     //reload timer0 low byte
    TH0 =(65536-(Time0*Fosc/12000000))/256;                //reload timer0 high byte
	tt++;
		if(tt==10)
	{
	tt=0;
	UARTsendc(xintiao);
	UARTsendc1(lianxian1);
	UARTsendc1(lianxian2);
	}
}

