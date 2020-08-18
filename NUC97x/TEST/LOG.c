
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
void Init_LOG(void)//LOG�ϵ��ʼ��(��FS��ʼ�������)
{
/*
	os_Type *os;
	
	if(Comm_Ram->Init_LOG!=0)//�ϵ��LOG�ѳ�ʼ����־:0=û��!=0�ѳ�ʼ��
	{
		return;
	}
	Comm_Ram->Init_LOG=0xff;//�ϵ��LOG�ѳ�ʼ����־:0=û��!=0�ѳ�ʼ��
	os=Get_os();
#if LOG_698==1
#else
	os->yaffs_unlink("user/TEST/DL698.log");//ɾ���ļ�
	os->yaffs_unlink("user/TEST/DL698-old.log");//ɾ���ļ�
#endif
#if LOG_1376_1==1//0=û,1=��
#else
	os->yaffs_unlink("user/TEST/GDW1376-1.log");//ɾ���ļ�
	os->yaffs_unlink("user/TEST/GDW1376-1-old.log");//ɾ���ļ�
#endif
#if LOG_1376_2==1//0=û,1=��
#else
	os->yaffs_unlink("user/TEST/GDW1376-2.log");//ɾ���ļ�
	os->yaffs_unlink("user/TEST/GDW1376-2-old.log");//ɾ���ļ�
#endif
#if LOG_1376_3==1//0=û,1=��
#else
	os->yaffs_unlink("user/TEST/GDW1376-3.log");//ɾ���ļ�
	os->yaffs_unlink("user/TEST/GDW1376-3-old.log");//ɾ���ļ�
#endif
#if LOG_645_1997==1//0=û,1=��
#else
	os->yaffs_unlink("user/TEST/DL645-1997.log");//ɾ���ļ�
	os->yaffs_unlink("user/TEST/DL645-1997-old.log");//ɾ���ļ�
#endif
#if LOG_645_2007==1//0=û,1=��
#else
	os->yaffs_unlink("user/TEST/DL645-2007.log");//ɾ���ļ�
	os->yaffs_unlink("user/TEST/DL645-2007-old.log");//ɾ���ļ�
#endif
*/
}

void LOG(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
{
	u32 i;
	u32 x;
	char *p8str;
	
	if(Byte>(LEN_128KDATABUFF/4))
	{//����̫����
		return;
	}
	p8str=(char*)ADDR_128KDATABUFF;
	switch(TR)
	{
		case 0://0����
			p8str=StrCopy(p8str,"\r\n\r\nR");
			break;
		case 1://1����
			p8str=StrCopy(p8str,"\r\nT");
			break;
		case 2://2����(���ܺ�)
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
//ascii����
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[5],p8str);//RTCBuff[8];//��,��,ʱ,��,��,��,��,NC
	p8str=StrCopy(p8str,"/");
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[4],p8str);
	p8str=StrCopy(p8str," ");
	//ʱ��-ʱ
	p8str=Byte_ascii(Comm_Ram->RTCBuff[2],p8str);
	p8str=StrCopy(p8str,":");
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[1],p8str);
	p8str=StrCopy(p8str,":");
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[0],p8str);
	p8str=StrCopy(p8str,"\r\n");
	//֡����
	if(ascii==0)
	{//ascii=0Ϊhex��
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
	{//=1����Ϊascii��
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//��AT����
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//��AT����
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	i=((u32)p8str)-(ADDR_128KDATABUFF);
	LOG_FILE(protocol,i);//LOG nand��ADDR_128KDATABUFF����д���ļ�;���:protocol=13761,13762,13763,6451997,6452007;nand�г�������RTC_BKPxR��,len=ADDR_128KDATABUFF�г���(���޴�����ʱ��0)
}

void LOG_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
{
	u32 i;
	u32 x;
	char *p8str;
	
	p8str=(char*)ADDR_128KDATABUFF;
	if(Byte>(LEN_128KDATABUFF/4))
	{
		return;
	}
//ascii����
	//֡����
	if(ascii==0)
	{//ascii=0Ϊhex��
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
	{//=1����Ϊascii��
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//��AT����
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//��AT����
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	
	i=((u32)p8str)-(ADDR_128KDATABUFF);
	LOG_FILE(protocol,i);//LOG nand��ADDR_128KDATABUFF����д���ļ�;���:protocol=13761,13762,13763,6451997,6452007;nand�г�������RTC_BKPxR��,len=ADDR_128KDATABUFF�г���(���޴�����ʱ��0)
}





void LOG_ATRx(u32 link)//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
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
	if(Byte>8096)//4K�ֽ�FLASHд����(�Ͻ���ADDR_DATABUFF,����ʹ��ʱLEN_DATABUFF���ٴﵽ8K)
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
	{//��log
		LOG(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
	}
	else
	{//����log
		LOG_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
	}
}

void LOG_FILE(u32 protocol,u32 len)//LOG nand��ADDR_128KDATABUFF����д���ļ�;���:protocol=13761,13762,13763,6451997,6452007;FLASH�г�������RTC_BKPxR��,len=ADDR_128KDATABUFF�г���(���޴�����ʱ��0)
{
	os_Type *os;
	int yaffs;
	struct yaffs_stat stat;
	u32 LEN_SourFile;
	char pFileName[256];
	char* p8;
	
//д���ļ�
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
			{//ÿ���ļ����ߴ�����
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/GDW1376-1-old.log");//ɾ���ļ�
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
			{//ÿ���ļ����ߴ�����
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/GDW1376-2-old.log");//ɾ���ļ�
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
			{//ÿ���ļ����ߴ�����
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/GDW1376-3-old.log");//ɾ���ļ�
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
			{//ÿ���ļ����ߴ�����
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/DL645-1997-old.log");//ɾ���ļ�
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
			{//ÿ���ļ����ߴ�����
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/DL645-2007-old.log");//ɾ���ļ�
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
			{//ÿ���ļ����ߴ�����
				os->yaffs_close(yaffs);
				os->yaffs_unlink("user/TEST/DL698-old.log");//ɾ���ļ�
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

void Init_LOG(void)//LOG�ϵ��ʼ��(��FS��ʼ�������)
{
}
void LOG(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
{
}
void LOG_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
{
}
void LOG_ATRx(u32 link)//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
{
}
void LOG_FILE(u32 protocol,u32 len)//LOG nand��ADDR_128KDATABUFF����д���ļ�;���:protocol=13761,13762,13763,6451997,6452007;FLASH�г�������RTC_BKPxR��,len=ADDR_128KDATABUFF�г���(���޴�����ʱ��0)
{
}

#endif//#ifdef LINUX










//��ӡ��RS232
#if DEBUG_PORTtoRS232!=0
void LOGwaitRS232(void)//�ȴ�RS232ԭ�������
{
	while(UART0Ctrl->Task!=0)//0=���ڽ���,1=�������,2=�ȴ���֡���ݷ���,3=��֡�������ڷ���
	{
		switch(UART0Ctrl->Task)
		{
			case 1://1=�������
				UART0Ctrl->Task=0;//0=���ڽ���
				break;
			case 2://2=�ȴ���֡���ݷ���
				UART0Ctrl->Task=3;//���ڷ��ͱ�־
				Start_UARTn_Tx(RS232PORT);//����UART�ڷ���
				break;
			case 3://3=��֡�������ڷ���
				if(Wait_UARTn_Tx(RS232PORT)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
				{			
					UART0Ctrl->Task=0;//0=���ڽ���
					UART0Ctrl->RxByte=0;//rx����=0
				}
				break;
		}
	}
}

void LOGRS232(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
{
	u32 i;
	u32 x;
	char *p8str;
	if(Byte>(((LEN_UART0TxMax+LEN_UART0FnDataBuff)/3)))
	{//����̫����
		return;
	}
	LOGwaitRS232();//�ȴ�RS232ԭ�������
	p8str=(char *)Get_ADDR_UARTnTx(RS232PORT);
	switch(TR)
	{
		case 0://0����
			p8str=StrCopy(p8str,"\r\n\r\nR");
			break;
		case 1://1����
			p8str=StrCopy(p8str,"\r\nT");
			break;
		case 2://2����(���ܺ�)
			p8str=StrCopy(p8str,"\r\nR");
			break;
		default:
			p8str=StrCopy(p8str,"\r\n\r\n-");
			break;
	}
	switch(protocol)//δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
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
//ascii����
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[5],p8str);//RTCBuff[8];//��,��,ʱ,��,��,��,��,NC
	p8str=StrCopy(p8str,"/");
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[4],p8str);
	p8str=StrCopy(p8str," ");
	//ʱ��-ʱ
	p8str=Byte_ascii(Comm_Ram->RTCBuff[2],p8str);
	p8str=StrCopy(p8str,":");
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[1],p8str);
	p8str=StrCopy(p8str,":");
	//ʱ��-��
	p8str=Byte_ascii(Comm_Ram->RTCBuff[0],p8str);
	p8str=StrCopy(p8str,"\r\n");
	//֡����
	if(ascii==0)
	{//ascii=0Ϊhex��
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
	{//=1����Ϊascii��
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//��AT����
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//��AT����
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	
	i=((u32)p8str)-(Get_ADDR_UARTnTx(RS232PORT));
	UART0Ctrl->TxByte=i;
	Init_UARTn(RS232PORT);//��ʼ��UART��,���ͨ�ſ�����������
	Start_UARTn_Tx(RS232PORT);//����UART�ڷ���
	UART0Ctrl->Task=3;//3=��֡�������ڷ���
	return;
}

void LOGRS232_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
{
	u32 i;
	u32 x;
	char *p8str;
	
	if(Byte>(((LEN_UART0TxMax+LEN_UART0FnDataBuff)/3)))
	{//����̫����
		return;
	}
	LOGwaitRS232();//�ȴ�RS232ԭ�������
	p8str=(char *)Get_ADDR_UARTnTx(RS232PORT);

//ascii����
	//֡����
	if(ascii==0)
	{//ascii=0Ϊhex��
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
	{//=1����Ϊascii��
		for(i=0;i<Byte;i++)
		{
			p8str[i]=pBUF[i];
		}
		p8str+=Byte;
		if(TR==1)
		{//��AT����
			p8str[0]=0x0a;
			p8str++;
		}
		else
		{//��AT����
			p8str[0]=0x0d;
			p8str[1]=0x0a;
			p8str+=2;
		}
	}
	
	i=((u32)p8str)-(Get_ADDR_UARTnTx(RS232PORT));
	UART0Ctrl->TxByte=i;
	Init_UARTn(RS232PORT);//��ʼ��UART��,���ͨ�ſ�����������
	Start_UARTn_Tx(RS232PORT);//����UART�ڷ���
	UART0Ctrl->Task=3;//3=��֡�������ڷ���
}


void LOGRS232_ATRx(u32 link)//GPRSģ��AT��������LOG;���:link=0��log,=1����log;ʹ��ADDR_DATABUFF
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
	if(Byte>8096)//4K�ֽ�FLASHд����(�Ͻ���ADDR_DATABUFF,����ʹ��ʱLEN_DATABUFF���ٴﵽ8K)
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
	{//��log
		LOGRS232(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
	}
	else
	{//����log
		LOGRS232_LINK(13763,0,GPRSPORT,(u8 *)ADDR_DATABUFF,Byte,1);//���:TR=0����=1����;protocol=13761,13762,13763,6451997,6452007;ascii=0Ϊhex��=1����Ϊascii��
	}
}
#endif//#if DEBUG_PORTtoRS232!=0




