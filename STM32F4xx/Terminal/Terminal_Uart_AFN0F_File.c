
//文件传输
#include "Project.h"
#include "Terminal_Uart_AFN0F_File.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../Device/IC_BUZZ.h"
#include "../OS/OS.h"
#include "../MS/MS.h"
#include "../OS/My_Heap.h"



void WriteFileSegmentFlag(u32 seg)//写收到文件段标志,入口:当前段数
{
	u32 i;
	u32 Addr;
	
	if(seg>=1024)
	{
		return;
	}
	Addr=seg/8;
	Addr+=(ADDR_AFN0CF14+4);
	seg%=8;
	seg=1<<seg;
	seg=(~seg);
	i=MRR(Addr,1);
	i&=seg;
	MWR(i,Addr,1);
}

u32 CheckFileSegmentFlag(u32 seg)//检查收到文件段标志,入口:总段数;返回:0没收到任何段，1有收到，2全部收到
{
	u32 i;
	u32 x;
	u32 f;
	u8 *p8;
	
	if(seg==0)
	{
		return 2;
	}
	if(seg>=1024)
	{
		return 0;
	}
	f=0;//b0=1有段没收到,b1=1有段收到
	MR(ADDR_DATABUFF,ADDR_AFN0CF14+4,128);
	p8=(u8*)ADDR_DATABUFF;
	x=0;
	while(1)
	{
		for(i=0;i<8;i++)
		{
			if((p8[x]&(1<<i))!=0)
			{
				f|=1;//有段没收到
			}
			else
			{
				f|=2;//有段收到
			}
			seg--;
			if(seg==0)
			{
				switch(f)
				{
					case 1:
						return 0;
					case 2:
						return 2;
					case 3:
						return 1;
				}
			}
		}
		x++;
	}
}




void Uart_AFN0F(u32 PORTn)//文件传输
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 DIlen;
	u32 Fn;
	u32 DIaddr;
	
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 * p32;
	u8 *p8s;
	u8 *p8d;
	FILE *file;
	os_Type *os;
	ms_Type* ms;
#if (USER/100)==5//上海
	u8 * p8;
#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)(Get_ADDR_UARTnRx(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
#if (USER/100)==5//上海
	if(DIlen<(12+0))
	{//F98读FTP升级文件传输结果不需PW
		goto ERR;
	}
#else
	if(DIlen<(12+16))
	{
		goto ERR;
	}
#endif

//PW认证
	if(p8rxbuff[16]!=2)
	{//不是F2自定义文件传输和F98上海FTP升级文件传输结果
		i=Uart_376_PW(PORTn);//消息认证码字段PW;入口PW首址,出口0有效,1无效,4已返错误帧
		if(i==4)
		{
			return;
		}
		if(i!=0x0)
		{
			goto ERR;
		}
	}
/*
//若红外口需设置开关
	if(PORTn==IRPORT)
	{
    if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_MeterFMTimer,2)==0))
		{
			goto ERR;
		}
	}
*/
//用户区数据标识和参数数据长度
	if(p8rxbuff[16]!=2)
	{//不是F2自定义文件传输和F98上海FTP升级文件传输结果
		if((p8rxbuff[13]&0x80)!=0x0)
		{//有Tp
			DIlen-=(8+16+6);
		}
		else
		{
			DIlen-=(8+16);
		}
	}
	else
	{//是F2自定义文件传输和F98上海FTP升级文件传输结果
		if((p8rxbuff[13]&0x80)!=0x0)
		{//有Tp
			DIlen-=(8+6);
		}
		else
		{
			DIlen-=(8);
		}
	}

//数据(启动站功能码10（请求1级数据）用于应用层请求确认（CON=1）的链路传输)
	DIaddr=((u32)p8rxbuff)+14;
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	if(DIlen<4)
	{
	ERR:
		//全部否认
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		Uart_376_NoAUX(PORTn);//没附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		return;
	}
	else
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		Fn=(i>>16);
		switch(Fn)
		{
			case 1://传输方式1
				if(DIlen<(4+11))
				{
					goto ERR;
				}
				if(p8rxbuff[18+0]==0)
				{//文件标识=0,清除下装文件,恢复到升级前状态。
					UARTCtrl->LinkTask2=0;//通道2链接任务(FTP命令)
					UARTCtrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
					MC(0,ADDR_AFN0CF14,4);//文件下装未收到数据段:组号
					MC(0xff,ADDR_AFN0CF14+4,128);//文件下装未收到数据段:组内各数据段未收到标志
					MC(0xff,ADDR_FTPFileFlags,5+16);//清FTP文件下载标志
					MC(0,ADDR_AFN0FF1,LEN_AFN0FF1);
					MC(0,((u32)p8rxbuff)+18+5,4);
				F1RET:
					MR(((u32)p8txbuff)+18,((u32)p8rxbuff)+18+5,4);//返回当前段标识4字节
					i=4+4;//返回长度
					i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
					p8txbuff[1]=i;
					p8txbuff[2]=i>>8;
					DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
					Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				}
				else
				{
					switch(p8rxbuff[18+2])//文件指令
					{
						case 0://报文方式下装
						/*
							if(UARTCtrl->AddrType>=2)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
							{//本帧是组地址或广播地址
								i=MRR(((u32)p8rxbuff)+18+3,2);//总段数
								i=CheckFileSegmentFlag(i);//检查收到文件段标志,入口:总段数;返回:0没收到任何段，1有收到，2全部收到
								if(i==0)
								{
									return;//没启动
								}
							}
						*/
						case 2://启动组地址升级
							x=MRR(((u32)p8rxbuff)+18+3,2);//总段数
							i=MRR(((u32)p8rxbuff)+18+5,4);//第i段
							if(i==0)
							{
								UARTCtrl->LinkTask2=0;//通道2链接任务(FTP命令)
								UARTCtrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
								MC(0,ADDR_AFN0CF14,4);//文件下装未收到数据段:组号
								MC(0xff,ADDR_AFN0CF14+4,128);//文件下装未收到数据段:组内各数据段未收到标志
								MC(0xff,ADDR_FTPFileFlags,5+16);//清FTP文件下载标志
								MC(0,ADDR_AFN0FF1,LEN_AFN0FF1);
							}
							if((x>1024)||(x==0))
							{//总段数错
								goto F1ERR;
							}
							if(i>=x)
							{//第i段>=总段数错
						F1ERR:
								if(UARTCtrl->AddrType>1)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
								{//本帧是组地址或广播地址
									return;
								}
								MC(0xff,((u32)p8rxbuff)+18+5,4);//返回当前段标识4字节为文件错误
								goto F1RET;
							}
							y=MRR(((u32)p8rxbuff)+18+9,2);//第i段数据长度Lf
							if((x*y)>LENmax_Download)
							{//大于512K错
								goto F1ERR;
							}
							z=Get_LEN_UARTnRx(PORTn);//得到UART接收缓冲长度
							if((18+11+y+2)>z)
							{//第i段数据长度Lf>接收缓冲长度错
								goto F1ERR;
							}
						#if (USER/100)!=7//不是浙江用户
							Data_Inverse(((u32)p8rxbuff)+18+11,y);//数据倒序(高低字节调换),入口Le>=2
						#endif
							z=MRR(ADDR_AFN0FF1+9,2);//取保存的第i段数据长度Lf
							if((i+1)==x)
							{//是最后段
								//计算总文件长度
								MWR((i*z)+y,ADDR_LEN_AFN0FF1,4);//376.1 F1下载文件的总长度计算后寄存
							}
							else
							{
								if(z!=0)
								{//已有第i段数据长度Lf
									if(z!=y)
									{
										goto F1ERR;
									}
								}
								z=y;//第i 段数据长度Lf
								MWR(y,ADDR_AFN0FF1+9,2);//保存的第i段数据长度Lf
							}
							MW(((u32)p8rxbuff)+18+11,ADDR_Download_ENTRY+(i*z),y);
							WriteFileSegmentFlag(i);//写收到文件段标志,入口:当前段数
							MW(((u32)p8rxbuff)+18,ADDR_AFN0FF1,1);//写文件标识(显示用到)
							i=CheckFileSegmentFlag(x);//检查收到文件段标志,入口:总段数;返回:0没收到任何段，1有收到，2全部收到
							if(i==2)
							{//2全部收到
								MC(0,ADDR_AFN0FF1,LEN_AFN0FF1);//清文件标识(显示用到)
								//
								os=Get_os();
								ms=Get_ms();
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
									x=p32[111]-p32[110];//原始文件长度
									if(x>LENmax_Download)
									{
										ms->ms_free(p8s);//释放我的内存堆分配
										ms->ms_free(p8d);//释放我的内存堆分配
										goto F1ERR;
									}
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
											ms->ms_free(p8s);//释放我的内存堆分配
											ms->ms_free(p8d);//释放我的内存堆分配
											goto F1ERR;
									}
								}
								else
								{//Lzma压缩
									MR((u32)p8s,ADDR_Download_ENTRY,LENmax_Download);
									i=p8s[5]+(p8s[6]<<8)+(p8s[7]<<16)+(p8s[8]<<24);
									x=MRR(ADDR_LEN_AFN0FF1,4);//原始文件长度
									if((i>(2*LENmax_Download))||(x>LENmax_Download))
									{
										ms->ms_free(p8s);//释放我的内存堆分配
										ms->ms_free(p8d);//释放我的内存堆分配
										goto F1ERR;
									}
									y=os->RAM_LzmaDecode((u32)p8s,x,(u32)p8d,i);//RAM->RAM的Lzma解压缩算法;返回:解压缩后的长度
									if(y!=i)
									{
										ms->ms_free(p8s);//释放我的内存堆分配
										ms->ms_free(p8d);//释放我的内存堆分配
										goto F1ERR;
									}
									p32=(u32*)p8d;
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
											ms->ms_free(p8s);//释放我的内存堆分配
											ms->ms_free(p8d);//释放我的内存堆分配
											goto F1ERR;
									}
								}
								if(file==NULL)
								{
									ms->ms_free(p8s);//释放我的内存堆分配
									ms->ms_free(p8d);//释放我的内存堆分配
									goto F1ERR;
								}
								os->fwrite(p8s,1,x,file);
								os->fclose(file);
								ms->ms_free(p8s);//释放我的内存堆分配
								ms->ms_free(p8d);//释放我的内存堆分配
								FileDownload->New=0;//有新文件载入标志:0=有,1=没
								FileDownload->PORTn=PORTn;//完成下载的端口号
							}
							if(UARTCtrl->AddrType>=2)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
							{//本帧是组地址或广播地址
								return;
							}
							goto F1RET;
							
						case 1://FTP 方式下装
							//连接任务
							UARTCtrl->LinkTask2=0;//通道2链接任务(FTP命令)
							UARTCtrl->FTPLinkCount=0;//FTP尝试连接计数
							UARTCtrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
							//FTP文件下载标志
							MC(0,ADDR_FTPFileFlags+4,1+16);
							//文件标识,指令，IP地址等
							MC(0,ADDR_DATABUFF,LEN_AFN0FF1);
							i=DIlen-4;
							if(i>LEN_AFN0FF1)
							{
								i=LEN_AFN0FF1;
							}
							MR(ADDR_DATABUFF,((u32)p8rxbuff)+18,i);
							MW(ADDR_DATABUFF,ADDR_AFN0FF1,LEN_AFN0FF1);
							//暂不返回
							//FTP接收帧的前256字节保存
							MW((u32)p8rxbuff,ADDR_AFN0FF1_Rx,256);
							//FTP返回帧的前18字节保存
							MW((u32)p8txbuff,ADDR_AFN0FF1_Tx,18);
						#if (USER/100)==5//上海
							//清F97FTP命令标志
							MC(0,ADDR_AFN0FF97,1);
						#endif
							return;
						default:
							goto ERR;
					}
				}
				return;
		#if (USER/100)==5//上海
			case 97://FTP升级文件传输
				if(DIlen<(4+126))
				{
					goto ERR;
				}
				//信息保存
				p8rxbuff[17]=1;//1byte 命令发起标志:0=不是F97命令,1=是F97命令发起
				MW(((u32)p8rxbuff)+17,ADDR_AFN0FF97,127);
				//连接任务
				UARTCtrl->LinkTask2=0;//通道2链接任务(FTP命令)
				UARTCtrl->FTPLinkCount=0;//FTP尝试连接计数
				UARTCtrl->FTPErr=0;//FTP错误标志:0=空,1=FTP服务器无法登录,2=找不到指定文件目录
				//FTP文件下载标志
				MC(0,ADDR_FTPFileFlags+4,1+16);
				//转换IP路径等为F1文件标识,指令，IP地址等
				MC(0,ADDR_DATABUFF,LEN_AFN0FF1);
				p8=(u8*)(ADDR_DATABUFF);
				//文件标识	BIN	1
				//文件属性	BIN	1
				p8[1]=1;//文件指令	BIN	1
				//总段数n	BIN	2
				//第i段标识或偏移（i=0～n-1）	BIN	4
				//第i段数据长度Lf	BIN	2
				//文件数据	BIN	Lf(ftp://文件路径,账号/口令的方式填写)
					//ip
				i=11;
				i+=CopyString((u8 *)"ftp://",p8+i);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				for(x=0;x<4;x++)
				{
					y=hex_bcd(p8rxbuff[18+x]);//HEX数转为8位压缩BCD数
					i+=BCD_ASCII(y,((u32)p8)+i);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					p8[i]='.';
					i++;
				}
					//端口号
				i--;
				p8[i]=':';
				i++;
				y=hex_bcd(p8rxbuff[18+6]+(p8rxbuff[18+7]<<8));//HEX数转为8位压缩BCD数
				i+=BCD_ASCII(y,((u32)p8)+i);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
					//文件路径
				p8[i]='/';
				i++;
				MC(0,ADDR_DATABUFF+256,21);
				MR(ADDR_DATABUFF+256,((u32)p8rxbuff)+18+36,20);
				i+=CopyString((u8 *)(ADDR_DATABUFF+256),p8+i);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					//账号
				p8[i]=',';
				i++;
				MC(0,ADDR_DATABUFF+256,11);
				MR(ADDR_DATABUFF+256,((u32)p8rxbuff)+18+16,10);
				i+=CopyString((u8 *)(ADDR_DATABUFF+256),p8+i);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					//口令
				p8[i]='/';
				i++;
				MC(0,ADDR_DATABUFF+256,11);
				MR(ADDR_DATABUFF+256,((u32)p8rxbuff)+18+26,10);
				i+=CopyString((u8 *)(ADDR_DATABUFF+256),p8+i);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
				MW(ADDR_DATABUFF,ADDR_AFN0FF1,LEN_AFN0FF1);
				
				//清F98 FTP升级文件传输结果数据
					//升级前终端软件版本号	ASCII	10
				MC(0,ADDR_DATABUFF,20);
				p8[0]='Z';
				p8[1]='Z';
				p8[2]=((SVER>>4)&0xf)+0x30;
				p8[3]=((SVER>>0)&0xf)+0x30;
					//升级后终端软件版本号	ASCII	10
					//发生时间：分时日月年	见附录A.15	5
				MC(0xee,ADDR_DATABUFF+20,5);
				MW(ADDR_DATABUFF,ADDR_AFN0FF98,25);
				//全部确认
				Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
				return;
			case 98://FTP升级文件传输结果
				MR(((u32)p8txbuff)+18,ADDR_AFN0FF98,25);//返回当前段标识4字节
				i=4+25;//返回长度
				i+=8;//1字节控制域+5字节地址域+1字节AFN+1字节SEQ=8字节
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
				Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
				return;
		#endif
/*
			case 2://传输方式2
				//DATABUFF开始32字节使用
				//1BYTE 信道0=RS232,1=GPRS或RJ45
				//1BYTE 保留
				//1BYTE 文件传输控制	BIN	1
				//1BYTE 文件编号(0-255)	BIN	1
				//16BYTE 文件名称	ASCII	16
				//2BYTE 文件总段数n(n>=1)	BIN	2
				//2BYTE 文件数据当前段数号（i=1~n）	BIN	2
				//2BYTE 文件每段字节长度(L)	BIN	2
				//4BYTE 已下载总字节数
				//2BYTE 保留
				if(DIlen<(4+24))
				{
					goto ERR;
				}
				p8=(u8 *)(ADDR_DATABUFF);
				MEMRead(ADDR_DATABUFF,ADDR_FileDown+(p8rxbuff[19]*32),32);
				if(p8rxbuff[19]<8)//文件编号
				{//文件编号=0-7
					if(p8rxbuff[18]<8)
					{//文件传输控制=0-7
						if(p8rxbuff[18]==0)
						{//文件传输控制=0传输启动命令
							MEMClr(0,ADDR_DATABUFF,32);
							if(PORTn!=RS232PORT)
							{//UART1
								p8[0]=1;
							}
							MEMRead(ADDR_DATABUFF+2,((u32)p8rxbuff)+18,20);
							MEMWrite(ADDR_DATABUFF,ADDR_FileDown+(p8rxbuff[19]*32),32);
RET_OK:
							Terminal_Ram->RequestFileTimer=0;//请求文件传输MS定时器
							//全部确认
							Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
							Uart_376_NoAUX(PORTn);//没附加信息
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
							return; 
						}
						if(p8rxbuff[18]==1)
						{//文件传输控制=1暂停
							MEMClr(1,ADDR_FileDown+2+(p8rxbuff[19]*32),1);
							goto RET_OK;
						}
			 			if(p8rxbuff[18]==2)
						{//文件传输控制=2继续
 							MEMClr(2,ADDR_FileDown+2+(p8rxbuff[19]*32),1);
							goto RET_OK;
						}
						if(p8rxbuff[18]==3)
						{//文件传输控制=3取消
 							MEMClr(3,ADDR_FileDown+2+(p8rxbuff[19]*32),1);
							goto RET_OK;
						}
						if(p8rxbuff[18]==4)
						{//文件传输控制=4传输完成
							goto RET_OK;
 						}
						if(p8rxbuff[18]==5)
						{//文件传输控制=5读传输信息
							MEMRead(((u32)p8txbuff)+18,ADDR_FileDown+2+(p8rxbuff[19]*32),24);
							p8txbuff[19]=p8rxbuff[19];//文件编号
							p8txbuff[1]=8+4+24+(4*6);
							p8txbuff[3]=8+4+24+(4*6);
							p8txbuff[2]=0;
							p8txbuff[4]=0;
							p8txbuff+=18;
							p8txbuff[22]=4*6;//文件每段字节长度(L)=4*版本号数
							p8txbuff[23]=0;
							MEMClr(' ',((u32)p8txbuff)+24,4*10);
							//硬件版本号
							p8txbuff[24+0]=(HV>>8)+0x30;
							p8txbuff[24+1]='.';
							p8txbuff[24+2]=((HV>>4)&0xf)+0x30;
							p8txbuff[24+3]=(HV&0xf)+0x30;
							//软件模块I0版本号
							p32=(u32 *)(ADDR_DATABUFF);
							MEMRead(ADDR_DATABUFF,ADDR_I0_ENTRY+(4*68),4);
							MEMRead(ADDR_DATABUFF,p32[0]+16,4);
							i=p32[0];
							p8txbuff[28+0]=(i>>8)+0x30;
							p8txbuff[28+1]='.';
							p8txbuff[28+2]=((i>>4)&0xf)+0x30;
							p8txbuff[28+3]=(i&0xf)+0x30;
							//软件模块I1版本号
							MEMRead(ADDR_DATABUFF,ADDR_I1_ENTRY+(4*68),4);
							if((p32[0]>ADDR_I1_ENTRY)&&(p32[0]<(ADDR_I1_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[32+0]=(i>>8)+0x30;
								p8txbuff[32+1]='.';
								p8txbuff[32+2]=((i>>4)&0xf)+0x30;
								p8txbuff[32+3]=(i&0xf)+0x30;
							}
							//软件模块I2版本号
							MEMRead(ADDR_DATABUFF,ADDR_I2_ENTRY+(4*68),4);
							if((p32[0]>ADDR_I2_ENTRY)&&(p32[0]<(ADDR_I2_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[36+0]=(i>>8)+0x30;
								p8txbuff[36+1]='.';
								p8txbuff[36+2]=((i>>4)&0xf)+0x30;
								p8txbuff[36+3]=(i&0xf)+0x30;
							}
							//软件模块E1版本号
							MEMRead(ADDR_DATABUFF,ADDR_E1_ENTRY+(4*68),4);
							if((p32[0]>ADDR_E1_ENTRY)&&(p32[0]<(ADDR_E1_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[40+0]=(i>>8)+0x30;
								p8txbuff[40+1]='.';
								p8txbuff[40+2]=((i>>4)&0xf)+0x30;
								p8txbuff[40+3]=(i&0xf)+0x30;
							}
							//软件模块E2版本号
							MEMRead(ADDR_DATABUFF,ADDR_E2_ENTRY+(4*68),4);
							if((p32[0]>ADDR_E2_ENTRY)&&(p32[0]<(ADDR_E2_ENTRY+(256*1024))))
							{
								MEMRead(ADDR_DATABUFF,p32[0]+16,4);
								i=p32[0];
								p8txbuff[44+0]=(i>>8)+0x30;
								p8txbuff[44+1]='.';
								p8txbuff[44+2]=((i>>4)&0xf)+0x30;
								p8txbuff[44+3]=(i&0xf)+0x30;
							}
							//软件模块E4版本号
							
							
							p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
							Uart_376_NoAUX(PORTn);//没附加信息
							Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
							return; 
						}
						if(p8rxbuff[18]==6)
						{//文件传输控制=6主站对终端请求数据的响应或终端向主站请求数据
							if((p8rxbuff[36]+(p8rxbuff[37]<<8))<(p8rxbuff[38]+(p8rxbuff[39]<<8)))
							{//总段数<当前段数号
								goto NOACK;
							}
							if((p8rxbuff[36]+(p8rxbuff[37]<<8))==0x0)
							{//总段数=0
								goto NOACK;
							}
							if((p8rxbuff[38]+(p8rxbuff[39]<<8))==0x0)
							{//当前段数号=0
								goto NOACK;
							}
							if((p8rxbuff[40]+(p8rxbuff[41]<<8))==0x0)
							{//当前段长度==0
								goto NOACK;
							}
							if((p8rxbuff[38]+(p8rxbuff[39]<<8))!=(p8[22]+(p8[23]<<8)+1))
							{//当前段数号!=请求段号
								if(PORTn==ENETPORT)
								{
									Terminal_Ram->RequestFileTimer=5000/10;//等5秒后再发请求帧,免传输延时多次请求相同帧而进入重复循环
								}
								else
								{
									if(PORTn==GPRSPORT)
									{
										Terminal_Ram->RequestFileTimer=10000/10;//等10秒后再发请求帧,免传输延时多次请求相同帧而进入重复循环
									}
									else
									{
										if(PORTn==SMSPORT)
										{
											Terminal_Ram->RequestFileTimer=60000/10;//等60秒后再发请求帧,免传输延时多次请求相同帧而进入重复循环
										}
										else
										{//其他通道5S
											Terminal_Ram->RequestFileTimer=5000/10;//等5秒后再发请求帧,免传输延时多次请求相同帧而进入重复循环
										}
									}
								}
								return;
							}
							p8[22]=p8rxbuff[38];
							p8[23]=p8rxbuff[39];
							x=p8[26]+(p8[27]<<8)+(p8[28]<<16)+(p8[29]<<24);//已下载总字节数
							i=p8rxbuff[40]+(p8rxbuff[41]<<8);//当前段字节数
							if((x+i)>(256*1024))
							{//(已下载总字节数+当前段字节数)>((256*1024)-4)
								goto NOACK;
							}
							if(DIlen!=4+24+i)
							{//数据单元标识和数据的长度!=4+24+L
								goto NOACK;
							}
							MEMWrite(((u32)p8rxbuff)+42,ADDR_Download_ENTRY+x,i);//写当前段数据
							//总段数,当前段数号,每段字节长度,已下载总字节数
							x+=i;
							p8[26]=x;
							p8[27]=x>>8;
							p8[28]=x>>16;
							p8[29]=x>>24;
							MEMWrite(ADDR_DATABUFF,ADDR_FileDown+(p8rxbuff[19]*32),32);
							if((p8[20]+(p8[21]<<8))==(p8[22]+(p8[23]<<8)))
							{//当前段数=总段数,完成
								MEMClr(4,ADDR_FileDown+2+(p8rxbuff[19]*32),1);//4=下载完成标志
								//Terminal_Ram->RequestFileTimer=2000/10;//请求文件传输延时2秒等待写入后再请求传输
								//return;
							}
NOACK:
							//返回处理,使马上能发出下帧,加快传输速度
							Terminal_Ram->RequestFileTimer=0;//请求文件传输MS定时器
							
						}
						if(p8rxbuff[18]==7)
						{//文件传输控制=7返回主站IP
							MEMWrite(((u32)p8rxbuff)+20,ADDR_AFN04F3,12);//主站IP地址和端口
							WWDT_RESET();//立即使能看门狗复位
							while(1)
							{
							}
						}
						
					}
				}
				break;
*/
			default:
				goto ERR;
		}
	}
}

/*
const u8 Request_File_Example[]=//请求文件帧
{
	0x68,36,0,36,0,0x68,
	0xCA,//控制码;D7(DIR传输方向位),D6(PRM启动标志位),D5(帧计数位FCB/要求访问位ACD),D4(帧计数有效位FCV/空),D3-D0(功能码)
	0,0,0,0,//任意终端地址和区域码
	0,//单地址
	0x0F,//AFN=0X0F
	0x60,//帧序列域SEQ;D7(TpV时间标签有效位),R6(FIR首帧),D5(FIN末帧),D4(CON确认标志位),D3-D0(PSEQ/RSEQ)
	0,0,2,0,//数据单元标识P0-F2
	6,//文件传输控制
	0,//文件编号
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,//文件名称	ASCII	16
	0,0,//文件总段数n(n>=1)
	0,0,//文件数据当前段数号（i=1~n)
	0,0,//文件每段字节长度(L)
};
void Request_File_Tx(u32 PORTn)//发送请求文件传输帧
{
	u32 i;
	u8 * p8txbuff;
	u8 * p8;
  UARTCtrl_TypeDef * UARTCtrl;

	p8=(u8 *)(ADDR_DATABUFF);
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	MEMRead((u32)p8txbuff,(u32)Request_File_Example,42);
  //取本终端地址
	MEMRead(((u32)p8txbuff)+7,ADDR_AREACODE,4);
	//取编号,名称,总段数,当前段数,每段字节长度
	MEMRead(((u32)p8txbuff)+19,ADDR_DATABUFF+3,23);
	//当前段数+1
	i=p8[22]+(p8[23]<<8);
	i++;
	p8txbuff[38]=i;
	p8txbuff[39]=i>>8;
	p8txbuff[13]|=(UARTCtrl->TxSEQ&0xf);//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ
	UARTCtrl->TxSEQ+=1;//启动帧序号PSEQ+1
	
	Uart_376_NoAUX(PORTn);//没附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}
*/

void Request_File(void)//请求文件传输
{
/*
	u32 PORTn;
	u32 i;
	u32 FileParaAddr;
	u8 * p8;
	u16 * p16timer;
	u32 *p32;
  UARTCtrl_TypeDef * UARTCtrl;


  p32=(u32 *)(ADDR_DATABUFF);
  MEMRead(ADDR_DATABUFF,ADDR_Download_ENTRY+(4*64),8);//存储器读
	if((p32[1]>p32[0])&&((p32[1]-p32[0])<=0x40000))
	{
		MEMRead(ADDR_DATABUFF,ADDR_Download_ENTRY+(p32[1]-p32[0]-4),4);//存储器读
		if(p32[0]==0xA55A5AA5)
		{
			return;//原下载没更新
		}
	}

	FileParaAddr=ADDR_FileDown;
	p8=(u8 *)(ADDR_DATABUFF);
	for(i=0;i<8;i++)
	{
		MEMRead(ADDR_DATABUFF,FileParaAddr,32);
		if((p8[2]==0x0)||(p8[2]==0x2))
		{//启动或继续
			if(p8[0]==0x0)
			{//RS232
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(RS232PORT));
				if(UARTCtrl->Task==0x0)
				{//0=接收
					p16timer=(u16 *)(Get_ADDR_UARTnTimer(RS232PORT));
					if(p16timer[0]==0x0)
					{//超时定时=0
						if(Terminal_Ram->RequestFileTimer==0x0)//请求文件传输MS定时器
						{
							Terminal_Ram->RequestFileTimer=1000/10;//若没回答1000MS后重发
							Request_File_Tx(RS232PORT);//发送请求文件传输帧
						}
					}
				}
			}
			else
			{//无线或以太网
				PORTn=ENETPORT;
				UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
				if(UARTCtrl->LinkTask!=101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
				{
					PORTn=GPRSPORT;
					UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(GPRSPORT));
					if(UARTCtrl->LinkTask!=101)//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					{
						return;//没登录
					}
				}
				if(PORTn==GPRSPORT)
				{
					if(UART1Ctrl->ATTask!=0)//AT任务:0=空闲,1=发送,2=正在发送,3=接收
					{
						return;
					}
				}
				//登录
					if(UARTCtrl->Task==0x0)
					{//0=接收
						p16timer=(u16 *)(Get_ADDR_UARTnTimer(PORTn));
						if(p16timer[0]==0x0)
						{//超时定时=0
							if(Terminal_Ram->RequestFileTimer==0x0)//请求文件传输MS定时器
							{
								if(PORTn==ENETPORT)//模块类型
								{//以太网
									Terminal_Ram->RequestFileTimer=5000/10;//若没回答5S后重发
								}
								else
								{//无线模块
									Terminal_Ram->RequestFileTimer=5000/10;//若没回答10S后重发
								}
								Request_File_Tx(PORTn);//发送请求文件传输帧
							}
						}
					}
				
			}
		}
		FileParaAddr+=32;
	}
*/
}
















