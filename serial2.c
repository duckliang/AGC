/* 宏定义无符号字符型、整形、长整形 */
#define S2RI 0x01	//串口2接收中断请求标志位
#define S2TI 0x02	//串口2发送中断请求标志位
#define uchar unsigned char			   //8位
#define ulint unsigned long			   //32位
#define uint unsigned int			   //16位

uchar idata Send_buf[30];			   //发送数据数组
uchar idata Receive_buf[30];		   //接收数据数组
uint  idata disp[8]={0,0,0,0,0,0,0,0}; //字接收显示数组，用于存储6x1-6x8的十六位接收数据
uchar count=0,time=0;				   //count为收到数据的次数，time应用于发送03代码时，中间的字数量是多少个数值
uchar flag_finish=0,flag_led=0;		   //是否接收完成位，flag_led暂时不用
uint  address_num=0;				   //接收到字的数值
uint  begin_address=0,address_leg=0;   //数值字开始位，数值字长度
uchar sign=0,signal=0x00;

sbit K0=P0^0;						  
sbit K1=P0^1;
sbit K2=P0^2;
sbit K3=P0^3;
sbit K4=P0^4;
sbit K5=P0^5;
sbit K6=P0^6;
sbit K7=P0^7;
sbit K8=P2^0;
sbit K9=P2^1;
sbit K10=P2^2;						  
sbit K11=P2^3;
sbit K12=P2^4;
sbit K13=P2^5;
sbit K14=P2^6;
sbit K15=P2^7;
sbit K16=P1^0;
sbit K17=P1^1;
/*****16位CRC检验表，低位在前，高位在后*******/

//////////////*高位表*///////////////////
uchar code auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

//////////////*低位表*///////////////////
uchar code auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;

////////*16位CRC校验函数,查表法*////////////////
uint crc16(uchar *puchMsg,uchar usDataLen) 	
{ 											
	uchar uchCRCHi = 0xFF ; 				
	uchar uchCRCLo = 0xFF ; 				
	uint uIndex ; 							
	while (usDataLen--) 					
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; 	
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (((uint)(uchCRCLo) << 8) | uchCRCHi) ;
}

/////////*延时子程序，延迟N us*//////////
void delay_us(int n)
{
	int i;
	for(i=n;n<0;n--)
	_nop_();
}

/////////*延时子程序，延迟N ms*//////////
void delay_ms(int n)
{
	int i,j;
	for(i=n;i>0;i--)
		for(j=141;j>0;j--);
}



/*************串口初始化函数***************/
/********设定的参数为9600，N，8，1*********/

void init_uart(void) 	//9600bps@11.0592MHz
{
	AUXR &= 0xF7;		//波特率不倍速
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xFB;		//独立波特率发生器时钟为Fosc/12,即12T
	BRT = 0xFD;		//设定独立波特率发生器重装值
	IE2 =0x01;	//开串口2中断  ES2=1
    EA =1;	//开总中断
	AUXR |= 0x10;		//启动独立波特率发生器
}

/******************发送函数******************/
/***********发送一个字符即一个字节***********/

void send(uchar num)
{
	S2BUF=num;		   //把字符放到发送数据缓存区SBUF，num可以是字符也可以是一个字节数据
    while(!(S2CON&S2TI));  //若S2TI=0，在此等待
    S2CON&=~S2TI;	  //S2TI=0
} 

/**********************发送函数***********************/
/***********发送字符串或者一连串的字节数据************/
/****mydata表示要发送的字符串数组，num表示数组长度****/

void send_word(uchar *mydata,uchar num) 
{ 										
	int i;
	for(i=0;i<num;i++)
	{
		send(*(mydata+i));
	}
	count=0;
}

/********************中断接收函数**********************/
/****当数据接收完时，进入串口中断将接收的值放入数组****/

void comm() interrupt 8 using 3
{
	if(S2CON&S2RI)						//当接收完一个字节的数据后，RI会自动置位，即RI=1
	{  	
	    uchar a;
	    S2CON&=~S2RI;					//接受中断标志软件清零					
		a=S2BUF;					//把缓存在单片机的数据给a
		Receive_buf[count]=a;	//将接受数据放入预置数组
		count++;				//数组自动递增，用于存储下一个数据
		if(count==8)			//当接收完8个MODBUS的功能码之后，自动清0，从新接收（支持的01、05、03、06命令读取或者写入每次都发送8个字节）
		{
			count=0;
			flag_finish=1;		//接受完8个MODBUS功能码的标志位
		}
	}
}


/**************************MODBUS功能码处理***************************/
/********目前只能识别01，03，05，06功能码，而且只能挂载单台***********/

void command(void)
{
	uint crc_end,legg;										  //CEC校验值和03功能码发送的位的长度

		if(Receive_buf[1] == 0x01)							  //01功能码
		{
			crc_end=crc16(Receive_buf,6);					  //校验
			if(crc_end==Receive_buf[6]<<8 | Receive_buf[7])	  //当校验一致时
			{
				Send_buf[0]=Receive_buf[0];					  //站号
				Send_buf[1]=Receive_buf[1];					  //功能码
				Send_buf[2]=0x04;							  //字节数
//				P0=0XFF;
//				P2=0XFF;
				Send_buf[3]=~P0;						  //低位状态,即LED开关情况
				Send_buf[4]=~P2;						  //高位状态
				Send_buf[5]=~((P1&0x01)|signal);
				Send_buf[6]=0X00;
				crc_end=crc16(Send_buf,7);					  //校验
				Send_buf[7]=crc_end%256;					  //校验低位
				Send_buf[8]=crc_end/256;					  //校验高位
				send_word(Send_buf,9);						  //发送返回屏
			}
		}
		if(Receive_buf[1] == 0x05)							  //05功能码
		{
			begin_address=Receive_buf[3];					  //开始地址
			crc_end=crc16(Receive_buf,6);					  //校验
			if(crc_end==Receive_buf[6]<<8|Receive_buf[7])	  //校验正确时
			{
				if(Receive_buf[4]==0xff)					  //当为强制打开时
				 {
					 switch(begin_address)					 //对应的地址
					 {
						 case 0x00: K0=0; sign=1; break;
						 case 0x01: K1=0; sign=2; break;
						 case 0x02: K2=0; sign=3; break;
						 case 0x03: K3=0; sign=4; break;
						 case 0x04: K4=0; sign=5; break;
						 case 0x05: K5=0; sign=6; break;
						 case 0x06: K6=0; sign=7; break;
						 case 0x07: K7=0; sign=8; break;
						 case 0x08: K8=0; sign=9; break;
						 case 0x09: K9=0; sign=10; break;
						 case 0x0A: K10=0; sign=11; break;
						 case 0x0B: K11=0; sign=12; break;
						 case 0x0C: K12=0; sign=13; break;
						 case 0x0D: K13=0; sign=14; break;
						 case 0x0E: K14=0; sign=15; break;
						 case 0x0F: K15=0; sign=16; break;
						 case 0x10: K16=0; sign=17; break;
                         case 0x11: K17=0; sign=18; break;
					 }					 
				 }
				else										  //当强制为OFF时
				 {
					switch(begin_address)
					{
						 case 0x00: K0=1;  break;
						 case 0x01: K1=1;  break;
						 case 0x02: K2=1;  break;
						 case 0x03: K3=1;  break;
						 case 0x04: K4=1;  break;
						 case 0x05: K5=1;  break;
						 case 0x06: K6=1;  break;
						 case 0x07: K7=1;  break;
						 case 0x08: K8=1;  break;
						 case 0x09: K9=1;  break;
						 case 0x0A: K10=1;  break;
						 case 0x0B: K11=1;  break;
						 case 0x0C: K12=1;  break;
						 case 0x0D: K13=1;  break;
						 case 0x0E: K14=1;  break;
						 case 0x0F: K15=1;  break;
						 case 0x10: K16=1;  break;
                         case 0x11: K17=1;  break;
					}
				 }					  
				Send_buf[0]=Receive_buf[0];					//站号
				Send_buf[1]=Receive_buf[1];					//功能码
				Send_buf[2]=Receive_buf[2];					//起始高位
				Send_buf[3]=Receive_buf[3];					//起始低位
				Send_buf[4]=Receive_buf[4];					//返回状态
				Send_buf[5]=Receive_buf[5];					//返回状态
				crc_end=crc16(Send_buf,6);					//校验
				Send_buf[6]=crc_end%256;					//校验低位
				Send_buf[7]=crc_end/256;					//校验高位
				send_word(Send_buf,8);			 			//发送返回屏
			}
		}

}
  

