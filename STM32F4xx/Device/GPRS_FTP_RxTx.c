
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#endif
#include "GPRS_FTP_RxTx.h"
#include "../Terminal/ATString.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/ascii.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../OS/My_Heap.h"


u32 GPRS_FTP_Tx(void)//GPRS_FTP_Tx发送;返回发送字节数
{
	u32 i;
	u32 x;
	u32 y;
	u32 TxByte;
	u16 *p16;
	u8 *p8tx;
	u32 FTPFileCount;//FTP文件下载个数计数
#if MainProtocol==698
	u8* p8;
#endif
	
	p8tx=(u8*)(ADDR_ATTx);
	TxByte=0;
	FTPFileCount=MRR(ADDR_FTPFileFlags+4,1);
	switch(UART1Ctrl->LinkTask2)//通道2链接任务(FTP命令)
	{
		case 0://不论如何先关闭文件服务
			TxByte=CopyString((u8 *)"AT$MYFTPCLOSE\r",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			break;
		case 1://启动FTP文件服务
			TxByte=CopyString((u8 *)"AT$MYFTPOPEN=0,",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
		#if MainProtocol==376
			//目的IP 地址,目的端口号
			MR(ADDR_DATABUFF,ADDR_AFN0FF1+11,LEN_AFN0FF1-11);
			i=WinDataComp((u32)(u8 *)"//",ADDR_DATABUFF,2,64);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
			if(i==0)
			{//无IP
				UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				return 0;
			}
			i+=2;
			y=WinDataComp((u32)(u8 *)",",i,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
			if(y==0)
			{//无IP和目录
				UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				return 0;
			}
			x=WinDataComp((u32)(u8 *)"/",i,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
			if(x>y)
			{//无目录
				x=y;
			}
			MC(0,x,1);//字符'/'改为串结束符0
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)i,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//账号
			i=WinDataComp((u32)(u8 *)",",x,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
			if(i==0)
			{//无目录
				i=x;
			}
			i+=1;
			x=WinDataComp((u32)(u8 *)"/",i,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
			if(x==0)
			{//无账号
				UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				return 0;
			}
			MC(0,x,1);//字符'/'改为串结束符0
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)i,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//口令
			p8tx[TxByte]='"';
			TxByte++;
			TxByte+=CopyString((u8 *)(x+1),p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='1';//0表示主动模式；1表示被动模式（默认被动模式）
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='3';//5-180s，FTP命令或者数据空闲超时设置，单位秒；默认为30秒
			TxByte++;
			p8tx[TxByte]='0';//
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='0';//0表示二进制模式 (默认)；1表示文本模式
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
		#else
			//MainProtocol=698
//属性4（服务器信息）∷=structure
//{
//IP地址  octet-string，
//端口    long-unsigned，
//用户名  visible-string，
//密码    visible-string
//}
			//目的IP 地址,目的端口号
			MR(ADDR_DATABUFF,ADDR_F002_4,LENmax_F002_4);
			p8=Get_Element((u8*)ADDR_DATABUFF,1,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8==0)
			{//无IP
				UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				return 0;
			}
			if(p8[1]!=4)
			{
				UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			IP4_HEX_ASCII(p8+2,(u8 *)ADDR_DATABUFF+1024);
			TxByte+=CopyString((u8 *)ADDR_DATABUFF+1024,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8=Get_Element((u8*)ADDR_DATABUFF,2,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8)
			{//有端口设置
				if(p8[0]==DataType_long_unsigned)
				{
					i=(p8[1]<<8)|p8[2];
					if(i)
					{//端口号!=0
						p8tx[TxByte]=':';
						TxByte+=1;
						TxByte+=HEX_ASCII(i,(u32)p8tx+TxByte);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					}
				}
			}
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//账号
			p8=Get_Element((u8*)ADDR_DATABUFF,3,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8==0)
			{//无账号
				UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			i=p8[1];
			MR((u32)p8tx+TxByte,(u32)p8+2,i);
			TxByte+=i;
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			//口令
			p8=Get_Element((u8*)ADDR_DATABUFF,4,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8==0)
			{//无口令
				UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			i=p8[1];
			MR((u32)p8tx+TxByte,(u32)p8+2,i);
			TxByte+=i;
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='1';//0表示主动模式；1表示被动模式（默认被动模式）
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='3';//5-180s，FTP命令或者数据空闲超时设置，单位秒；默认为30秒
			TxByte++;
			p8tx[TxByte]='0';//
			TxByte++;
			p8tx[TxByte]=',';
			TxByte++;
			p8tx[TxByte]='0';//0表示二进制模式 (默认)；1表示文本模式
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
		#endif
			break;
		case 2://获取FTP文件大小
			TxByte=CopyString((u8 *)"AT$MYFTPSIZE=",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='/';
			TxByte++;
		#if MainProtocol==376
			MC(0,ADDR_DATABUFF,512);
			MR(ADDR_DATABUFF,ADDR_AFN0FF1+11,LEN_AFN0FF1-11);
			x=WinDataComp((u32)(u8 *)"/",ADDR_DATABUFF+12,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
			if(x!=0)
			{//有目录
				x+=1;
				y=WinDataComp((u32)(u8 *)",",x,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
				if(y!=0)
				{//有目录
					MC(0,y,1);
					p8tx[TxByte]='/';
					TxByte++;
					TxByte+=CopyString((u8 *)x,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				}
			}
			
		#else
			//MainProtocol==698
			p8=Get_Element((u8*)ADDR_F002_7,1,1,LENmax_F002_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8)
			{//找到文件路径及名称
				i=p8[1];
				if(i)
				{
					p8+=2;
					if(p8[0]=='/')
					{//有文件路径
						//计算仅路径的字符个数（不含文件名）
						while(i)
						{
							i--;
							if(p8[i]=='/')
							{
								break;
							}
						}
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
					}
				}
			}
		#endif
			TxByte+=CopyString((u8 *)FileUserDIR,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			p8tx[TxByte]='/';
			TxByte++;
			switch(FTPFileCount)//FTP文件下载个数计数
			{
				case 0:
					//TxByte+=CopyString((u8 *)FileFTP_MS,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 1:
					//TxByte+=CopyString((u8 *)FileFTP_OS,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 2:
					//TxByte+=CopyString((u8 *)FileFTP_Meter,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 3:
					//TxByte+=CopyString((u8 *)FileFTP_Terminal,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 4:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					//TxByte+=CopyString((u8 *)FileFTP_Menu,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
					return 0;
				#endif
				case 5:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					//TxByte+=CopyString((u8 *)FileFTP_Font,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
					return 0;
				#endif
				case 6:
					//TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 7:
			#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=			
					switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
					{
						case 1:
							TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 2:
							TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 3:
							TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 4:
							TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 5:
							TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 6:
							TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 7:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 8:
							TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 101:
							TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 102:
							TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 103:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 104:
							TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
							
						default:
							MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
							MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
							UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
							UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
							return 0;
					}
					break;
			#endif
			#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=			
					TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
			#endif
			#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
					{
						switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
						{
							case 1:
								TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 2:
								TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 3:
								TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 4:
								TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 5:
								TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 6:
								TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 7:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 8:
								TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 101:
								TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 102:
								TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 103:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 104:
								TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
								
							default:
								MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
								MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
								UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
								UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
								return 0;
						}
						break;
					}
					else
					{
						TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					}
			#endif
			#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
					return 0;
			#endif
				default:
					UART1Ctrl->LinkTask2=6;//通道2链接任务(FTP命令)//6=关闭FTP服务
					return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case 3://得到文件数据(命令）
			TxByte=CopyString((u8 *)"AT$MYFTPGET=",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			p8tx[TxByte]='"';
			TxByte++;
			p8tx[TxByte]='/';
			TxByte++;
		#if MainProtocol==376
			MC(0,ADDR_DATABUFF,512);
			MR(ADDR_DATABUFF,ADDR_AFN0FF1+11,LEN_AFN0FF1-11);
			x=WinDataComp((u32)(u8 *)"/",ADDR_DATABUFF+12,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
			if(x!=0)
			{//有目录
				x+=1;
				y=WinDataComp((u32)(u8 *)",",x,1,128);//窗口数据比较,返回与DataAddr1相同数据的DataAddr2开始地址,若=0在后移TByte字节内无相同
				if(y!=0)
				{//有目录
					MC(0,y,1);
					p8tx[TxByte]='/';
					TxByte++;
					TxByte+=CopyString((u8 *)x,p8tx+TxByte);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				}
			}
		#else
			//MainProtocol==698
			p8=Get_Element((u8*)ADDR_F002_7,1,1,LENmax_F002_7);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8)
			{//找到文件路径及名称
				i=p8[1];
				if(i)
				{
					p8+=2;
					if(p8[0]=='/')
					{//有文件路径
						//计算仅路径的字符个数（不含文件名）
						while(i)
						{
							i--;
							if(p8[i]=='/')
							{
								break;
							}
						}
						MR((u32)p8tx+TxByte,(u32)p8,i);
						TxByte+=i;
					}
				}
			}
		#endif
			TxByte+=CopyString((u8 *)FileUserDIR,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			p8tx[TxByte]='/';
			TxByte++;
			switch(FTPFileCount)//FTP文件下载个数计数
			{
				case 0:
					//TxByte+=CopyString((u8 *)FileFTP_MS,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 1:
					//TxByte+=CopyString((u8 *)FileFTP_OS,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 2:
					//TxByte+=CopyString((u8 *)FileFTP_Meter,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 3:
					//TxByte+=CopyString((u8 *)FileFTP_Terminal,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 4:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					//TxByte+=CopyString((u8 *)FileFTP_Menu,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
					return 0;
				#endif
				case 5:
				#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					//TxByte+=CopyString((u8 *)FileFTP_Font,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				#else
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
					return 0;
				#endif
				case 6:
					//TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					TxByte+=CopyString((u8 *)FileFTP_PACK,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
				case 7:
			#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=			
					switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
					{
						case 1:
							TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 2:
							TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 3:
							TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 4:
							TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 5:
							TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 6:
							TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 7:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 8:
							TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 101:
							TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 102:
							TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 103:
							TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						case 104:
							TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
							break;
						default:
							MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
							MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
							UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
							UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
							return 0;
					}
					break;
			#endif
			#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=			
					TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
					break;
			#endif
			#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
					{
						switch(Comm_Ram->RouterType)//路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=
						{
							case 1:
								TxByte+=CopyString((u8 *)FileFTP_PLC_50,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 2:
								TxByte+=CopyString((u8 *)FileFTP_PLC_TC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 3:
								TxByte+=CopyString((u8 *)FileFTP_PLC_ES,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 4:
								TxByte+=CopyString((u8 *)FileFTP_PLC_CX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 5:
								TxByte+=CopyString((u8 *)FileFTP_PLC_01,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 6:
								TxByte+=CopyString((u8 *)FileFTP_PLC_LH,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 7:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 8:
								TxByte+=CopyString((u8 *)FileFTP_PLC_QL,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 101:
								TxByte+=CopyString((u8 *)FileFTP_PLC_MX,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 102:
								TxByte+=CopyString((u8 *)FileFTP_PLC_FC,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 103:
								TxByte+=CopyString((u8 *)FileFTP_PLC_NW,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							case 104:
								TxByte+=CopyString((u8 *)FileFTP_PLC_SR,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
								break;
							default:
								MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
								MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
								UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
								UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
								return 0;
						}
						break;
					}
					else
					{
						TxByte+=CopyString((u8 *)FileFTP_MODULE,p8tx+TxByte);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
						break;
					}
			#endif
			#if ((Project/100)<2)||((Project/100)>4)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=采集器,600=通信模块,700=
					MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
					MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
					return 0;
			#endif
				default:
					UART1Ctrl->FTPErr=2;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
					return 0;
			}
			p8tx[TxByte]='"';
			TxByte++;
			//文件数据的偏移量
			p8tx[TxByte]=',';
			TxByte++;
			i=MRR((u32)&UART1Ctrl->FTPByte,4);
			i=hex_bcd(i);
			TxByte+=BCD_ASCII(i,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
			//读取文件数据的长度
			p8tx[TxByte]=',';
			TxByte++;
			x=MRR((u32)&UART1Ctrl->FTPFileLen,4);//FTP当前文件总长度
			y=MRR((u32)&UART1Ctrl->FTPByte,4);//FTP当前文件已完成下载字节数
			if(x<=y)
			{
				UART1Ctrl->LinkTask2=2;//通道2链接任务(FTP命令)//2=获取FTP文件大小
				return 0;
			}
			x-=y;
			if(x>LEN_DATABUFF)
			{
				x=LEN_DATABUFF;
			}
			x=hex_bcd(x);
			TxByte+=BCD_ASCII(x,((u32)p8tx)+TxByte);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
			p8tx[TxByte]='\r';
			TxByte++;
			break;
		case 4://得到文件数据
			p16=(u16*)(ADDR_UART1IntATRx);
			if(p16[0]==p16[1])
			{
				Terminal_Ram->AT_MS_Timer=40000/10;//AT接收第一个字节超时定时值(秒)
			}
			UART1Ctrl->ATTask=3;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			return 0;
		case 5://一个文件传输完成，等待更新完成
//		  p32=(u32 *)(ADDR_DATABUFF);
//		  MR(ADDR_DATABUFF,ADDR_Download_ENTRY+(4*64),8);
//			if(p32[0]==0)
//			{//文件已更新
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)//2=获取FTP文件大小
//			}
			break;
		case 6://关闭FTP服务
			TxByte=CopyString((u8 *)"AT$MYFTPCLOSE\r",p8tx);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			break;
		default:
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
			break;
	}
	return TxByte;
}


void GPRS_FTP_Rx(void)//GPRS_FTP_Rx接收
{
	u32 i;
	u32 x;
	u32 y;
	u32 RxByte;
	u32 Addr;
	u8 *p8s;
	u8 *p8d;
	u16 *p16fifo;
	u32 *p32;
	u32 p1;
	u32 FTPFileCount;//FTP文件下载个数计数
	os_Type *os;	
	ms_Type* ms;
#ifdef IC_STM32F4xx
	FILE *file;
#endif
#ifdef IC_NUC9xx
	int yaffs;
	char *pFileName;
#endif
	
	p16fifo=(u16*)(ADDR_UART1IntATRx);
	FTPFileCount=MRR(ADDR_FTPFileFlags+4,1);
	switch(UART1Ctrl->LinkTask2)//通道2链接任务(FTP命令)
	{
		case 0://不论如何先关闭文件服务
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			else
			{
				UART1Ctrl->LinkSubTask++;//尝试AT发送计数
				if(UART1Ctrl->LinkSubTask<=2)//尝试AT发送计数
				{
					return;
				}
			}
	#if MainProtocol==376
			i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
	#else	
			i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
			if(i)
			{
				i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
				i=(i>>8)+((i&0xff)<<8);
				i/=60;
			}
			else
			{
				i=0;
			}
	#endif
			if(i<=1)
			{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
				Terminal_Ram->GPRSWaitTx_S_Timer=15;//等待启动FTP文件服务秒定时器
			}
			else
			{
				Terminal_Ram->GPRSWaitTx_S_Timer=30;//等待启动FTP文件服务秒定时器
			}
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->LinkTask2++;//通道2链接任务(FTP命令)
			break;
		case 1://启动FTP文件服务
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->LinkTask2++;//通道2链接任务(FTP命令)
				return;
			}
			if(Terminal_Ram->GPRSWaitTx_S_Timer==0)//等待启动FTP文件服务秒定时器
			{
		#if MainProtocol==376
				i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
		#else	
				i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
				if(i)
				{
					i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
					i=(i>>8)+((i&0xff)<<8);
					i/=60;
				}
				else
				{
					i=0;
				}
		#endif
				if(i<=1)
				{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
					Terminal_Ram->GPRSWaitTx_S_Timer=15;//等待启动FTP文件服务秒定时器
				}
				else
				{
					Terminal_Ram->GPRSWaitTx_S_Timer=30;//等待启动FTP文件服务秒定时器
				}
				UART1Ctrl->LinkSubTask++;//尝试AT发送计数
				if(UART1Ctrl->LinkSubTask>2)//尝试AT发送计数
				{
					UART1Ctrl->FTPErr=1;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				}
			}
			break;
		case 2://获取FTP文件大小
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"$MYFTPSIZE: "));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				MC(0,ADDR_DATABUFF,64);
				i=GetATRxString(ADDR_DATABUFF);//取AT返回当前p1指针开始以0x0d0x0a结束的字符串,入口字符串存放地址;返回:不含0x0d0x0a的字符串长度,同时修改p1指向下字符串开始
				Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
				if(Addr!=0)
				{
					p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				}
				if(i!=0)
				{
					i=ASCII_HEX(ADDR_DATABUFF);//10进制的ASCII码转为HEX,例"1234"->0x04d2,入口ASCII码开始地址非0-9ASCII码为结束
					if((i>LENmax_Download)||(i==0))
					{//文件长度错误
						MWR(5,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
						MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
						UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
					}
					else
					{//文件长度OK
						MWR(1,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
						MWR(i,(u32)&UART1Ctrl->FTPFileLen,4);//FTP当前文件总长度
						MWR(0,(u32)&UART1Ctrl->FTPByte,4);//FTP当前文件已完成下载字节数
						UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
						UART1Ctrl->LinkTask2++;//通道2链接任务(FTP命令)
					}
					return;
				}
			}
			UART1Ctrl->LinkSubTask++;//尝试AT发送计数
			if(UART1Ctrl->LinkSubTask>2)//尝试AT发送计数
			{
				MWR(6,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
				MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
				//NextATCount();//下1个AT
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->ATCount++;
			}
			break;
		case 3://得到文件数据(命令）
			Addr=ATRxSearch((u32)((u8*)"CONNECT\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				if(p16fifo[1]>=(LEN_UART1IntATRx-10))
				{
					p16fifo[1]-=(LEN_UART1IntATRx-10);
				}
				UART1Ctrl->ATTask=1;//AT任务:1=发送;避免中间空闭心跳等插入错误
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->LinkTask2++;//通道2链接任务(FTP命令)
				return;
			}
			UART1Ctrl->LinkSubTask++;//尝试AT发送计数
			if(UART1Ctrl->LinkSubTask>2)//尝试AT发送计数
			{
				MWR(5,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
				MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
			}
			break;
		case 4://得到文件数据
			p1=p16fifo[1];//原p1
			//读取字节长度
			RxByte=MRR((u32)&UART1Ctrl->FTPFileLen,4);//FTP当前文件总长度
			y=MRR((u32)&UART1Ctrl->FTPByte,4);//FTP当前文件已完成下载字节数
			if(RxByte<=y)
			{
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
				return;
			}
			RxByte-=y;
			if(RxByte>LEN_DATABUFF)
			{
				RxByte=LEN_DATABUFF;
			}
			i=GetATRxByte();//中断接收,计算当前p0对应p1还有未处理字节数
			if(i<(RxByte+6))
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
					p16fifo[1]=p1;//原p1
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
				}
				else
				{
					UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					UART1Ctrl->LinkTask2=3;//得到文件数据(命令）
					//NextATCount();//下1个AT
					UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
					UART1Ctrl->SubTask=0;//子任务
					UART1Ctrl->ATCount++;
				}
				return;
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			//接收数据线性放ADDR_DATABUFF
			x=p16fifo[1];
			while(x>=(LEN_UART1IntATRx-10))
			{
				x-=(LEN_UART1IntATRx-10);
			}
			p8s=(u8*)(ADDR_UART1IntATRx+10);
			p8d=(u8*)(ADDR_DATABUFF);
			for(i=0;i<RxByte;i++)
			{
				if(x==p16fifo[0])
				{
					break;
				}
				p8d[i]=p8s[x];
				x++;
				if(x>=(LEN_UART1IntATRx-10))
				{
					x=0;
				}
			}
			p16fifo[1]=x;//修改已读指针
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
				//写
				if((y+RxByte)>LENmax_Download)//FTP当前文件已完成下载字节数
				{
					goto FileErr;
				}
				else
				{
					MW(ADDR_DATABUFF,ADDR_Download_ENTRY+y,RxByte);
					y+=RxByte;
					MWR(y,(u32)&UART1Ctrl->FTPByte,4);

					x=MRR((u32)&UART1Ctrl->FTPFileLen,4);//FTP当前文件总长度
					if(y>=x)
					{//当前已读取长度>=FTP当前文件总长度,下载完成
						if(y!=x)
						{
							goto FileErr;
						}
						os=Get_os();
						ms=Get_ms();
					#ifdef IC_STM32F4xx
						i=os->ffree("N0:");
						if(i<LENmax_Download)
						{//空间不够
							//删除TEST文件夹
							os->fdelete("N0:/TEST/",NULL);//删除文件
							i=os->ffree("N0:");
							if(i<LENmax_Download)
							{//空间还不够
								//删除Program Files文件夹
								os->fdelete("N0:/Program Files/",NULL);//删除文件
							}
						}
					#endif
					#ifdef IC_NUC9xx
						i=os->yaffs_freespace("user/Update");
						if(i<LENmax_Download)
						{//空间不够
							//删除TEST文件夹
								os->yaffs_unlinkdir("user/TEST/");
								i=os->yaffs_freespace("user/Update");
								if(i<LENmax_Download)
								{//空间还不够
									//删除Program Files文件夹
									os->yaffs_unlinkdir("user/Program Files/");
								}
						}
					#endif
						p8s=ms->ms_malloc((size_t)LENmax_Download);//动态分配我的内存堆
						p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//动态分配我的内存堆
						if((p8s==0)||(p8d==0))
						{
							ms->Init_my_heap();
							p8s=ms->ms_malloc((size_t)LENmax_Download);//动态分配我的内存堆
							p8d=ms->ms_malloc((size_t)(2*LENmax_Download));//动态分配我的内存堆
						}
						i=MRR(ADDR_Download_ENTRY,1);
						if(i==0)
						{//非压缩
							MR((u32)p8s,ADDR_Download_ENTRY,LENmax_Download);
							p32=(u32*)p8s;
							//x=p32[111]-p32[110];//原始文件长度
							if(x>LENmax_Download)
							{
							FileErr:
								ms->ms_free(p8s);//释放我的内存堆分配
								ms->ms_free(p8d);//释放我的内存堆分配
								MWR(5,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
								MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
								UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
								UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
								return;
							}
					#ifdef IC_STM32F4xx
						#if MainProtocol==376
							switch(p32[128])//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS.bin","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS.bin","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter.bin","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal.bin","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu.bin","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.bin","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.bin","w");
									break;
								default:
									goto FileErr;
							}
						#else
							switch(p32[128])//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS698.bin","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS698.bin","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter698.bin","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal698.bin","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu698.bin","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.bin","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.bin","w");
									break;
								default:
									goto FileErr;
							}
						#endif
					#endif
						}
						else
						{//Lzma压缩
							MR((u32)p8s,ADDR_Download_ENTRY,LENmax_Download);
							i=p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);
							//x=原始文件长度
							if((i>(2*LENmax_Download))||(x>LENmax_Download))
							{
								goto FileErr;
							}
							y=os->RAM_LzmaDecode((u32)p8s,x,(u32)p8d,i);//RAM->RAM的Lzma解压缩算法;返回:解压缩后的长度
							if(y!=i)
							{
								goto FileErr;
							}
							p32=(u32*)p8d;
					#ifdef IC_STM32F4xx
						#if MainProtocol==376
							switch(p32[128])//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS.zip","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS.zip","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter.zip","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal.zip","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu.zip","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.zip","w");
									break;
								case 6:
									file=os->fopen("N0:/Download/PACK.zip","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.zip","w");
									break;
								default:
									goto FileErr;
							}
						#else
							switch(p32[128])//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
							{
								case 0:
									file=os->fopen("N0:/Download/MS698.zip","w");
									break;
								case 1:
									file=os->fopen("N0:/Download/OS698.zip","w");
									break;
								case 2:
									file=os->fopen("N0:/Download/Meter698.zip","w");
									break;
								case 3:
									file=os->fopen("N0:/Download/Terminal698.zip","w");
									break;
								case 4:
									file=os->fopen("N0:/Download/Menu698.zip","w");
									break;
								case 5:
									file=os->fopen("N0:/Download/Font.zip","w");
									break;
								case 6:
									file=os->fopen("N0:/Download/PACK698.zip","w");
									break;
								case 7:
									file=os->fopen("N0:/Download/Module.zip","w");
									break;
								default:
									goto FileErr;
							}
						#endif
						}
						if(file==NULL)
						{
							goto FileErr;
						}
						os->fwrite(p8s,1,x,file);
						os->fclose(file);
				#endif
				#ifdef IC_NUC9xx
						pFileName=(char*)ADDR_DATABUFF;
						pFileName=StrCopy(pFileName,"user/Update/");
						switch(p32[128])//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
						{
							case 0:
								pFileName=StrCopy(pFileName,FileFactory_MS);
								break;
							case 1:
								pFileName=StrCopy(pFileName,FileFactory_OS);
								break;
							case 2:
								pFileName=StrCopy(pFileName,FileFactory_Meter);
								break;
							case 3:
								pFileName=StrCopy(pFileName,FileFactory_Terminal);
								break;
							case 4:
								pFileName=StrCopy(pFileName,FileFactory_Menu);
								break;
							case 5:
								pFileName=StrCopy(pFileName,FileFactory_Font);
								break;
							case 6:
								pFileName=StrCopy(pFileName,FileFactory_PACK);
								break;
							case 7:
								pFileName=StrCopy(pFileName,FileFactory_Modele);
								break;
							default:
								goto FileErr;
						}
					}
					pFileName[0]=0;
					yaffs=os->yaffs_open((char*)ADDR_DATABUFF,O_CREAT | O_RDWR | O_TRUNC,S_IREAD | S_IWRITE);
					if(yaffs<0)
					{
						goto FileErr;
					}
					os->yaffs_WRITE(yaffs,p8s,x);
					//os->yaffs_fsync(yaffs);
					os->yaffs_close(yaffs);
				#endif
						ms->ms_free(p8s);//释放我的内存堆分配
						ms->ms_free(p8d);//释放我的内存堆分配
						FileDownload->New=0;//有新文件载入标志:0=有,1=没
						//OK
						if(FTPFileCount<7)
						{//PACK
							MC(2,ADDR_FTPFileFlags+5,7);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
							MWR(7,ADDR_FTPFileFlags+4,1);//下载文件计数+1
							UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
							UART1Ctrl->LinkTask2++;//通道2链接任务(FTP命令)//一个文件传输完成，等待更新完成
						}
						else
						{
							MWR(2,ADDR_FTPFileFlags+5+FTPFileCount,1);//0=没操作,1=正在读专用,2=专用完成,3=正在读通用,4=通用完成,5=文件错误,6=文件没找到
							MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
							UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
							UART1Ctrl->LinkTask2++;//通道2链接任务(FTP命令)//一个文件传输完成，等待更新完成
						}
						return;
					}
				}
				UART1Ctrl->LinkTask2=3;//得到文件数据(命令）
				//NextATCount();//下1个AT
				UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
				UART1Ctrl->SubTask=0;//子任务
				UART1Ctrl->ATCount++;
				return;
			}
			if(Terminal_Ram->AT_MS_Timer!=0)
			{//接收没完成
				p16fifo[1]=p1;//原p1
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
				if(i<=1)
				{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
					if(Terminal_Ram->AT_MS_Timer>(15000/10))
					{
						Terminal_Ram->AT_MS_Timer=(15000/10);
					}
				}
				return;
			}
			UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
			//NextATCount();//下1个AT
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->SubTask=0;//子任务
			UART1Ctrl->ATCount++;
			break;
		//case 5://一个文件传输完成，等待更新完成
		//	break;
		case 6://关闭FTP服务
			Addr=ATRxSearch((u32)((u8*)"OK\r\n"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr==0)
			{
				if(Terminal_Ram->AT_MS_Timer!=0)
				{//接收没完成
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
					if(i<=1)
					{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
						if(Terminal_Ram->AT_MS_Timer>(15000/10))
						{
							Terminal_Ram->AT_MS_Timer=(15000/10);
						}
					}
					return;
				}
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			Addr=ATRxSearch((u32)((u8*)"OK"));//AT命令返回数据搜索;入口:ADDR_String为要找的以0结束的字符串或数据串地址,返回:0=没找到,!=0为找到相同串后的第1个字节地址
			if(Addr!=0)
			{
				p16fifo[1]=Addr-(ADDR_UART1IntATRx+10);//修改已读指针
			}
			MWR(FTPFileCount+1,ADDR_FTPFileFlags+4,1);//下载文件计数+1
			break;
		default:
			if(Terminal_Ram->AT_MS_Timer!=0)
			{//接收没完成
			#if MainProtocol==376
					i=MRR(ADDR_AFN04F1+5,1);//心跳周期 BIN min  1字节
			#else	
					i=(u32)Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//计算元素地址,使用ADDR_128K_DATABUFF
					if(i)
					{
						i=MRR(i+1,2);;//心跳周期(秒)  long-unsigned
						i=(i>>8)+((i&0xff)<<8);
						i/=60;
					}
					else
					{
						i=0;
					}
			#endif
				if(i<=1)
				{//心跳周期=1分(60秒),等待延时用15秒,共发3次为45秒,否则当3次时间大于心跳周期时重链接会清LinkSubTask=0进入死循环无法退出
					if(Terminal_Ram->AT_MS_Timer>(15000/10))
					{
						Terminal_Ram->AT_MS_Timer=(15000/10);
					}
				}
				return;
			}
			UART1Ctrl->ATTask=0;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			UART1Ctrl->LinkSubTask=0;//尝试AT发送计数
			UART1Ctrl->LinkTask2=0;//通道2链接任务(FTP命令)
			break;
	}
}


