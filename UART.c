#include "UART.h"
bit Rs232_Rece_Error,Start_Rxd,Rs232_Flag;
unsigned char  Rs_buf[Rs232_Input_LEN],Rs_buf1[Rs232_Input_LEN],Rs_buf2[Rs232_Input_LEN],g_cReceNum,buf_num=0;
unsigned int   Rs_bufsum=0;
void UARTinit(void)				//���ڳ�ʼ��
{
 EA=0;          //��ʱ�ر��ж�
 TMOD&=0x0F;    //��ʱ��1ģʽ�����ڸ�4λ
 TMOD|=0x20;    //��ʱ��1������ģʽ2���Զ���װģʽ
 SCON=0x50;     //���ڹ�����ģʽ1
 TH1=256-FOSC/(Baud*12*16);  //���㶨ʱ����װֵ
 TL1=256-FOSC/(Baud*12*16);
 PCON|=0x80;    //���ڲ����ʼӱ�   PCON=0x00;�����ʲ�����
 ES=1;          //�����ж�����
 TR1=1;         //������ʱ��1
 REN=1;         //������� 
 EA=1;          //�����ж�
}
void UARTsend(unsigned char d)		  //����һ���ֽڵ����ݣ��β�d��Ϊ���������ݡ�
{
 SBUF=d; //������д�뵽���ڻ���
 sending=1;	 //���÷��ͱ�־
 while(sending); //�ȴ��������
}
void UARTsendc(unsigned char * str)
{	 	
    int i;  
	if(str!=xintiao)
	{ 
	str[TX_NUM]=ZZ++;
	if(str[TX_NUM]==0){str[TX_NUM]=1;}
	str[TX_CRC]=Tx_CRC(str);
	str[target]=targetN;
	}
	for(i=0;i<28;i++)
	{
		UARTsend(str[i]);
	}
}
void UARTsendc1(unsigned char * str)
{
   int i;
   str[TX_NUM]=ZZ++;
   if(str[TX_NUM]==0){str[TX_NUM]=1;}
   str[TX_CRC]=Tx_CRC(str);
   	for(i=0;i<28;i++)
	{
		UARTsend(str[i]);
	}

}


/********************************************/
/****������һ��������Ϊ������־�Ĳ�������****/
/********************************************/
void RS232(void) interrupt 4   using 2
{   
   if(RI)
   {  
      unsigned char Rece_data = SBUF;  
	  
	   ES                     = False;  
       RI                     = 0;
		  if (Start_Rxd) 
		  {
		             g_cReceNum ++;			 
				 if ((g_cReceNum == (Rs232_Input_LEN-1))) 
				 {
				       if(Rece_data==Rx_end_data)
				           { 
						     Rs_buf[g_cReceNum] = Rece_data;
				   			 Start_Rxd = False;
							 Rs232_Flag = True;
						     g_cReceNum = 0;
							 goto Rs232_end ;
							}
				 }
				 if(g_cReceNum==2)
				 {
				 	if(Rece_data==1)
					{buf_num=1;}
					if(Rece_data==2)
					{buf_num=2;}				 
				 }
	             if (g_cReceNum<(Rs232_Input_LEN-1))
				 {
				         Rs_buf[g_cReceNum] = Rece_data;
					 if(buf_num==1)Rs_buf1[g_cReceNum] = Rece_data;
				     if(buf_num==2)Rs_buf2[g_cReceNum] = Rece_data;  
				 }
				   else 
				 {
				         
						  g_cReceNum = 0;
						  Rs232_Rece_Error = True;
						  Start_Rxd = False;
				 }												  	
	
			} 
		  else
	      {        
		       if (Rece_data == Rx_start_data)
	           {    
	           		 Start_Rxd = True;
	                 g_cReceNum = 0;
	                 Rs_buf[g_cReceNum]  = Rece_data;
	           }          
	
	       }
	
		Rs232_end:     			    
	       ES = True;
	}
	else      //������һ�ֽ�����
	{
	  TI=0;
	  sending=0;  //�����ڷ��ͱ�־
	}
}
unsigned char Rx_CRC()
{
 unsigned int Rx_data=0,k;
 for( k=0;k<(Rs232_Input_LEN-1);k++){Rx_data += Rs_buf[k];}
 Rx_data=Rx_data&0x00FF;
 Rx_data=(unsigned char)(~Rx_data+1);
 return Rx_data;
}
unsigned char Tx_CRC(unsigned char Txarray[Rs232_Output_LEN])
{
 unsigned int Tx_data=0,k;
 for( k=0;k<(Rs232_Output_LEN-1);k++){Tx_data += Txarray[k];}
 Tx_data=Tx_data&0x00FF;
 Tx_data=(unsigned char)(~Tx_data+1);
 return Tx_data;
}
	  

