
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Test/LOG.h"
#include "../../STM32F4xx/OS/OS.h"
#include "../../STM32F4xx/Calculate/ascii.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_UART.h"

#include "../../STM32F4xx/MS/MS.h"


#ifndef LINUX
void Init_LOG(void)//LOG上电初始化(在FS初始化后进行)
{
/*
	os_Type *os;
	
	if(Comm_Ram->Init_LOG!=0)//上电后LOG已初始化标志:0=没，!=0已初始化
	{
		return;
	}
	Comm_Ram->Init_LOG=0xff;//上电后LOG已初始化标志:0=没，!=0已初始化
	os=Get_os();
#if LOG_698==1
#else
	os->yaffs_unlink("user/TEST/DL698.log");//删除文件
	os->yaffs_unlink("user/TEST/DL698-old.log");//删除文件
#endif
#if LOG_1376_1==1//0=没,1=有
#else
	os->yaffs_unlink("user/TEST/GDW1376-1.log");//删除文件
	os->yaffs_unlink("user/TEST/GDW1376-1-old.log");//删除文件
#endif
#if LOG_1376_2==1//0=没,1=有
#else
	os->yaffs_unlink("user/TEST/GDW1376-2.log");//删除文件
	os->yaffs_unlink("user/TEST/GDW1376-2-old.log");//删除文件
#endif
#if LOG_1376_3==1//0=没,1=有
#else
	os->yaffs_unlink("user/TEST/GDW1376-3.log");//删除文件
	os->yaffs_unlink("user/TEST/GDW1376-3-old.log");//删除文件
#endif
#if LOG_645_1997==1//0=没,1=有
#else
	os->yaffs_unlink("user/TEST/DL645-1997.log");//删除文件
	os->yaffs_unlink("user/TEST/DL645-1997-old.log");//删除文件
#endif
#if LOG_645_2007==1//0=没,1=有
#else
	os->yaffs_unlink("user/TEST/DL645-2007.log");//删除文件
	os->yaffs_unlink("user/TEST/DL645-2007-old.log");//删除文件
#endif
*/
}

void LOG(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
	u32 i;
	u32 x;
	char *p8str;
	
	if(Byte>(LEN_128KDATABUFF/4))
	{//长度太长错
		return;
	}
	p8str=(char*)ADDR_128KDATABUFF;
	switch(TR)
	{
		case 0://0接收
			p8str=StrCopy(p8str,"\r\n\r\nR");
			break;
		case 1://1发送
			p8str=StrCopy(p8str,"\r\nT");
			break;
		case 2://2接收(解密后)
			p8str=StrCopy(p8str,"\r\nR");
			break;
		default:
			p8str=StrCopy(p8str,"\r\n\r\n-");
			break;
	}
	if((protocol==698)||(protocol==13761)||(protocol==6451997)||(protocol==6452007))
	{
		switch(PORTn)
		{
			case RS232PORT:
				p8str=StrCopy(p8str,"(232) ");
				break;
			case GPRSPORT:
				p8str=StrCopy(p8str,"(G) ");
				break;
			case RS485_1PORT:
				p8str=StrCopy(p8str,"(1) ");
				break;
			case RS485_2PORT:
				p8str=StrCopy(p8str,"(2) ");
				break;
		#if MainProtocol!=CSG
			case RS485_4PORT:
				p8str=StrCopy(p8str,"(M) ");
				break;
		#else
			case PLC_PORT:
				p8str=StrCopy(p8str,"(M) ");
				break;
		#endif
			case IRPORT:
				p8str=StrCopy(p8str,"(IR) ");
				break;
			case ENETPORT:
				p8str=StrCopy(p8str,"(E) ");
				break;
			case SMSPORT:
				p8str=StrCopy(p8str,"(SMS) ");
				break;
			default:
				return;
		}
	}
	else
	{
		p8str=StrCopy(p8str," ");
	}
//ascii数据
	//时间-月
	p8str=Byte_ascii(Comm_Ram->RTCBuff[5],p8str);//RTCBuff[8];//秒,分,时,周,日,月,年,NC
	p8str=StrCopy(p8str,"/");
	//时间-日
	p8str=Byte_ascii(Comm_Ram->RTCBuff[4],p8str);
	p8str=StrCopy(p8str," ");
	//时间-时
	p8str=Byte_ascii(Comm_Ram->RTCBuff[2],p8str);
	p8str=StrCopy(p8str,":");
	//时间-分
	p8str=Byte_ascii(Comm_Ram->RTCBuff[1],p8str);
	p8str=StrCopy(p8str,":");
	//时间-秒
	p8str=Byte_ascii(Comm_Ram->RTCBuff[0],p8str);
	p8str=StrCopy(p8str,"\r\n");
	//帧数据
	if(ascii==0)
	{//ascii=0为hex码
		for(i=0;i<Byte;i++)
		{
			x=pBUF[i];
			x>>=4;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[0]=x;
			x=pBUF[i];
			x&=0xf;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[1]=x;
			p8str[2]=' ';
			p8str+=3;
		}
	}
	else
	{//=1本身为ascii码
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//是AT发送
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//是AT接收
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	i=((u32)p8str)-(ADDR_128KDATABUFF);
	LOG_FILE(protocol,i);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;nand中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
}

void LOG_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
	u32 i;
	u32 x;
	char *p8str;
	
	p8str=(char*)ADDR_128KDATABUFF;
	if(Byte>(LEN_128KDATABUFF/4))
	{
		return;
	}
//ascii数据
	//帧数据
	if(ascii==0)
	{//ascii=0为hex码
		for(i=0;i<Byte;i++)
		{
			x=pBUF[i];
			x>>=4;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[0]=x;
			x=pBUF[i];
			x&=0xf;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[1]=x;
			p8str[2]=' ';
			p8str+=3;
		}
		p8str=StrCopy(p8str,"\r\n");
	}
	else
	{//=1本身为ascii码
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//是AT发送
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//是AT接收
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	
	i=((u32)p8str)-(ADDR_128KDATABUFF);
	LOG_FILE(protocol,i);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;nand中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
}





void LOG_ATRx(u32 link)//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
{
	u32 i;
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	
	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p1>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p0>=p1)
	{
		Byte=p0-p1;
	}
	else
	{
		Byte=(p0+(LEN_UART1IntATRx-10))-p1;
	}
	p8s=(u8*)(ADDR_UART1IntATRx+10);
	p8d=(u8*)(ADDR_DATABUFF);
	if(Byte>8096)//4K字节FLASH写缓冲(上紧接ADDR_DATABUFF,特殊使用时LEN_DATABUFF至少达到8K)
	{
		Byte=8096;
	}
	for(i=0;i<Byte;i++)
	{
		if(p1>=(LEN_UART1IntATRx-10))
		{
			p1=0;
		}
		p8d[i]=p8s[p1];
		p1++;
	}
	if(link==0)
	{//新log
		LOG(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
	}
	else
	{//接上log
		LOG_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
	}
}

void LOG_FILE(u32 protocol,u32 len)//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
{
	os_Type *os;
	int yaffs;
	struct yaffs_stat stat;
	u32 LEN_SourFile;
	char pFileName[256];
	char* p8;
	
//写入文件
	os=Get_os();
	switch(protocol)
	{
		case 13761:
			p8=(char*)pFileName;
			p8=StrCopy(p8,"user/TEST/GDW1376-1.log");
			p8[0]=0;
			yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_APPEND,S_IREAD | S_IWRITE);
			if(yaffs<0) return;
			os->yaffs_fstat(yaffs, &stat);
			LEN_SourFile=stat.st_size;
			if(LEN_SourFile>LENmax_LOG)
			{//每个文件最大尺寸限制
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/GDW1376-1-old.log");//删除文件
				os->yaffs_rename("user/TEST/GDW1376-1.log","user/TEST/GDW1376-1-old.log");
				yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
				if(yaffs<0) return;
			}
			break;
		case 13762:
			p8=(char*)pFileName;
			p8=StrCopy(p8,"user/TEST/GDW1376-2.log");
			p8[0]=0;
			yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_APPEND,S_IREAD | S_IWRITE);
			if(yaffs<0)
			{
				return;
			}
			os->yaffs_fstat(yaffs, &stat);
			LEN_SourFile=stat.st_size;
			if(LEN_SourFile>LENmax_LOG)
			{//每个文件最大尺寸限制
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/GDW1376-2-old.log");//删除文件
				os->yaffs_rename("user/TEST/GDW1376-2.log","user/TEST/GDW1376-2-old.log");
				yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
				if(yaffs<0) return;
			}
			break;
		case 13763:
			p8=(char*)pFileName;
			p8=StrCopy(p8,"user/TEST/GDW1376-3.log");
			p8[0]=0;
			yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_APPEND,S_IREAD | S_IWRITE);
			if(yaffs<0) return;
			os->yaffs_fstat(yaffs, &stat);
			LEN_SourFile=stat.st_size;
			if(LEN_SourFile>LENmax_LOG)
			{//每个文件最大尺寸限制
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/GDW1376-3-old.log");//删除文件
				os->yaffs_rename("user/TEST/GDW1376-3.log","user/TEST/GDW1376-3-old.log");
				yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
				if(yaffs<0) return;
			}
			break;
		case 6451997:
			p8=(char*)pFileName;
			p8=StrCopy(p8,"user/TEST/DL645-1997.log");
			p8[0]=0;
			yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_APPEND,S_IREAD | S_IWRITE);
			if(yaffs<0) return;
			os->yaffs_fstat(yaffs, &stat);
			LEN_SourFile=stat.st_size;
			if(LEN_SourFile>LENmax_LOG)
			{//每个文件最大尺寸限制
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/DL645-1997-old.log");//删除文件
				os->yaffs_rename("user/TEST/DL645-1997.log","user/TEST/DL645-1997-old.log");
				yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
				if(yaffs<0) return;
			}
			break;
		case 6452007:
			p8=(char*)pFileName;
			p8=StrCopy(p8,"user/TEST/DL645-2007.log");
			p8[0]=0;
			yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_APPEND,S_IREAD | S_IWRITE);
			if(yaffs<0) return;
			os->yaffs_fstat(yaffs, &stat);
			LEN_SourFile=stat.st_size;
			if(LEN_SourFile>LENmax_LOG)
			{//每个文件最大尺寸限制
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/DL645-2007-old.log");//删除文件
				os->yaffs_rename("user/TEST/DL645-2007.log","user/TEST/DL645-2007-old.log");
				yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
				if(yaffs<0) return;
			}
			break;
		case 698:
			p8=(char*)pFileName;
			p8=StrCopy(p8,"user/TEST/DL698.log");
			p8[0]=0;
			yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_APPEND,S_IREAD | S_IWRITE);
			if(yaffs<0) return;
			os->yaffs_fstat(yaffs, &stat);
			LEN_SourFile=stat.st_size;
			if(LEN_SourFile>LENmax_LOG)
			{//每个文件最大尺寸限制
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/DL698-old.log");//删除文件
				os->yaffs_rename("user/TEST/DL698.log","user/TEST/DL698-old.log");
				yaffs=os->yaffs_open(pFileName,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
				if(yaffs<0) return;
			}
			break;
		default:
			return;
	}
	os->yaffs_WRITE(yaffs,(void*)ADDR_128KDATABUFF,len);
	//os->yaffs_fsync(yaffs);
	os->yaffs_close(yaffs);
	
}

#else//#ifndef LINUX

void Init_LOG(void)//LOG上电初始化(在FS初始化后进行)
{
}
void LOG(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
}
void LOG_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
}
void LOG_ATRx(u32 link)//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
{
}
void LOG_FILE(u32 protocol,u32 len)//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
{
}

#endif//#ifdef LINUX










//打印到RS232
#if DEBUG_PORTtoRS232!=0
void LOGwaitRS232(void)//等待RS232原发送完成
{
	while(UART0Ctrl->Task!=0)//0=正在接收,1=接收完成,2=等待单帧数据发送,3=单帧数据正在发送
	{
		switch(UART0Ctrl->Task)
		{
			case 1://1=接收完成
				UART0Ctrl->Task=0;//0=正在接收
				break;
			case 2://2=等待单帧数据发送
				UART0Ctrl->Task=3;//正在发送标志
				Start_UARTn_Tx(RS232PORT);//启动UART口发送
				break;
			case 3://3=单帧数据正在发送
				if(Wait_UARTn_Tx(RS232PORT)==0)//等待UART口发送,返回0=发送完成,1=发送没完成
				{			
					UART0Ctrl->Task=0;//0=正在接收
					UART0Ctrl->RxByte=0;//rx计数=0
				}
				break;
		}
	}
}

void LOGRS232(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
	u32 i;
	u32 x;
	char *p8str;
	if(Byte>(((LEN_UART0TxMax+LEN_UART0FnDataBuff)/3)))
	{//长度太长错
		return;
	}
	LOGwaitRS232();//等待RS232原发送完成
	p8str=(char *)Get_ADDR_UARTnTx(RS232PORT);
	switch(TR)
	{
		case 0://0接收
			p8str=StrCopy(p8str,"\r\n\r\nR");
			break;
		case 1://1发送
			p8str=StrCopy(p8str,"\r\nT");
			break;
		case 2://2接收(解密后)
			p8str=StrCopy(p8str,"\r\nR");
			break;
		default:
			p8str=StrCopy(p8str,"\r\n\r\n-");
			break;
	}
	switch(protocol)//未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	{
		case 1:
			protocol=6451997;
			break;
		case 2:
			protocol=6452007;
			break;
		case 3:
			protocol=698;
			break;
	}
	
	if((protocol==698)||(protocol==13761)||(protocol==6451997)||(protocol==6452007))
	{
		switch(PORTn)
		{
			case RS232PORT:
				p8str=StrCopy(p8str,"(232) ");
				break;
			case GPRSPORT:
				p8str=StrCopy(p8str,"(G) ");
				break;
			case RS485_1PORT:
				p8str=StrCopy(p8str,"(485-1) ");
				break;
			case RS485_2PORT:
				p8str=StrCopy(p8str,"(485-2) ");
				break;
		#if MainProtocol!=CSG
			case RS485_4PORT:
				p8str=StrCopy(p8str,"(PLC) ");
				break;
		#else
			case PLC_PORT:
				p8str=StrCopy(p8str,"(PLC) ");
				break;
		#endif
			case IRPORT:
				p8str=StrCopy(p8str,"(IR) ");
				break;
			case ENETPORT:
				p8str=StrCopy(p8str,"(E) ");
				break;
			case SMSPORT:
				p8str=StrCopy(p8str,"(SMS) ");
				break;
			default:
				return;
		}
	}
	else
	{
		p8str=StrCopy(p8str," ");
	}
//ascii数据
	//时间-月
	p8str=Byte_ascii(Comm_Ram->RTCBuff[5],p8str);//RTCBuff[8];//秒,分,时,周,日,月,年,NC
	p8str=StrCopy(p8str,"/");
	//时间-日
	p8str=Byte_ascii(Comm_Ram->RTCBuff[4],p8str);
	p8str=StrCopy(p8str," ");
	//时间-时
	p8str=Byte_ascii(Comm_Ram->RTCBuff[2],p8str);
	p8str=StrCopy(p8str,":");
	//时间-分
	p8str=Byte_ascii(Comm_Ram->RTCBuff[1],p8str);
	p8str=StrCopy(p8str,":");
	//时间-秒
	p8str=Byte_ascii(Comm_Ram->RTCBuff[0],p8str);
	p8str=StrCopy(p8str,"\r\n");
	//帧数据
	if(ascii==0)
	{//ascii=0为hex码
		for(i=0;i<Byte;i++)
		{
			x=pBUF[i];
			x>>=4;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[0]=x;
			x=pBUF[i];
			x&=0xf;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[1]=x;
			p8str[2]=' ';
			p8str+=3;
		}
	}
	else
	{//=1本身为ascii码
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//是AT发送
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//是AT接收
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	
	i=((u32)p8str)-(Get_ADDR_UARTnTx(RS232PORT));
	UART0Ctrl->TxByte=i;
	Init_UARTn(RS232PORT);//初始化UART口,入口通信控制字已填入
	Start_UARTn_Tx(RS232PORT);//启动UART口发送
	UART0Ctrl->Task=3;//3=单帧数据正在发送
	return;
}

void LOGRS232_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
	u32 i;
	u32 x;
	char *p8str;
	
	if(Byte>(((LEN_UART0TxMax+LEN_UART0FnDataBuff)/3)))
	{//长度太长错
		return;
	}
	LOGwaitRS232();//等待RS232原发送完成
	p8str=(char *)Get_ADDR_UARTnTx(RS232PORT);

//ascii数据
	//帧数据
	if(ascii==0)
	{//ascii=0为hex码
		for(i=0;i<Byte;i++)
		{
			x=pBUF[i];
			x>>=4;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[0]=x;
			x=pBUF[i];
			x&=0xf;
			if(x<10)
			{
				x+=0x30;
			}
			else
			{
				x+=55;
			}
			p8str[1]=x;
			p8str[2]=' ';
			p8str+=3;
		}
		p8str=StrCopy(p8str,"\r\n");
	}
	else
	{//=1本身为ascii码
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//是AT发送
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//是AT接收
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	
	i=((u32)p8str)-(Get_ADDR_UARTnTx(RS232PORT));
	UART0Ctrl->TxByte=i;
	Init_UARTn(RS232PORT);//初始化UART口,入口通信控制字已填入
	Start_UARTn_Tx(RS232PORT);//启动UART口发送
	UART0Ctrl->Task=3;//3=单帧数据正在发送
}


void LOGRS232_ATRx(u32 link)//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
{
	u32 i;
	u32 Byte;
	u32 p0;
	u32 p1;
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	
	p16=(u16*)(ADDR_UART1IntATRx);
	p0=p16[0];
	p1=p16[1];
	if(p0>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p1>=(LEN_UART1IntATRx-10))
	{
		return;
	}
	if(p0>=p1)
	{
		Byte=p0-p1;
	}
	else
	{
		Byte=(p0+(LEN_UART1IntATRx-10))-p1;
	}
	p8s=(u8*)(ADDR_UART1IntATRx+10);
	p8d=(u8*)(ADDR_DATABUFF);
	if(Byte>8096)//4K字节FLASH写缓冲(上紧接ADDR_DATABUFF,特殊使用时LEN_DATABUFF至少达到8K)
	{
		Byte=8096;
	}
	for(i=0;i<Byte;i++)
	{
		if(p1>=(LEN_UART1IntATRx-10))
		{
			p1=0;
		}
		p8d[i]=p8s[p1];
		p1++;
	}
	if(link==0)
	{//新log
		LOGRS232(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
	}
	else
	{//接上log
		LOGRS232_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
	}
}
#endif//#if DEBUG_PORTtoRS232!=0




