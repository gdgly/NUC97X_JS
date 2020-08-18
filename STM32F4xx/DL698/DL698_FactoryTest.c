
//工厂测试
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_FactoryTest.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/ascii.h"
#include "../TEST/TestReport.h"
#include "../Device/IC_BUZZ.h"
#include "../MS/MS.h"





//方法127进入工厂自测发送帧(OAD=0xFFFF7F00,地址兼容0xAAAAAAAAAAAA通配地址)
//68 18 00 43 05 aa aa aa aa aa aa 10 67 26 07 01 01 ff ff 7f 00 00 00 90 18 16
//返回帧
//68 1A 00 C3 05 01 00 00 00 00 00 10 9F 85 87 01 01 FF FF 7F 00 00 00 00 00 C8 18 16 (正确)
//68 1A 00 C3 05 01 00 00 00 00 00 10 9F 85 87 01 01 FF FF 7F 00 06 00 00 00 52 53 16 (错误,终端当前非工厂模式或无安装自测软件等等)

//方法128退出工厂模式发送帧(OAD=0xFFFF8000,地址兼容0xAAAAAAAAAAAA通配地址)
//68 18 00 43 05 aa aa aa aa aa aa 10 67 26 07 01 01 ff ff 80 00 00 00 42 dd 16
//返回帧
//68 1a 00 c3 05 01 00 00 00 00 00 10 9f 85 87 01 01 ff ff 80 00 00 00 00 00 e2 e5 16 (正确)

//读工厂自测结果发送帧(OAD=0xFFFF0200,地址兼容0xAAAAAAAAAAAA通配地址)
//68 17 00 43 05 aa aa aa aa aa aa 10 2b 3a 05 01 01 ff ff 02 00 00 d7 ea 16
//返回帧(返回错误信息:当无错误即全正确时返回空数据,当有错误时返回数据类型Visible String)
//68 1A 00 C3 05 01 00 00 00 00 00 10 9F 85 85 01 01 FF FF 02 00 01 00 00 00 8F 5F 16(无错误即全正确）
//68 24 00 c3 05 01 00 00 00 00 00 10 8d 5f 85 01 01 ff ff 02 00 01 0a 82 00 07 4e 4f 20 54 45 53 54 00 00 a5 80 16  (4e 4f 20 54 45 53 54的ASII"NO TEST")
//68 29 0D C3 05 01 00 00 00 00 00 10 71 1D 85 01 01 FF FF 02 00 01 0A 82 0D 0C ......    (ASCII码的测试报告文档,文档长度接近4k)

//读可充电电池插入状态错误信息发送帧(OAD=0xFFFF0300,地址兼容0xAAAAAAAAAAAA通配地址)
//68 17 00 43 05 aa aa aa aa aa aa 10 2b 3a 05 01 01 ff ff 03 00 00 0b b0 16
//返回帧(返回错误信息:当无错误即全正确时返回空数据,当有错误时返回数据类型Visible String)
//68 1a 00 c3 05 01 00 00 00 00 00 10 9f 85 85 01 01 ff ff 03 00 01 00 00 00 a4 5b 16(无错误即全正确）
//68 22 00 C3 05 01 00 00 00 00 00 10 FA AA 85 01 01 FF FF 03 00 01 0A 82 00 05 45 52 52 4F 52 00 00 CE 18 16   (45 52 52 4F 52的ASII"ERROR")




u32 GET_OAD_FactoryTest(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//读工厂测试结果;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	switch(OAD)
	{
		case 0xFFFF0200://测试总结论
			//错误信息
			p8tx[0]=DataType_visible_string;
			p8tx[1]=0x82;
			p8tx[2]=0;
			p8tx[3]=0;
			Len=4;
			//没测试
		#ifdef IC_STM32F4xx
			p8=(u8*)ADDR_Characteristics_BASE1+(OFFSET_TestResult_Characteristics);
		#endif
		#ifdef IC_NUC9xx
			p8=(u8*)((ADDR_Characteristics_BASE1)-(ADDR_IEEPROM_START))+ADDR_SPI_MAP;//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
			p8+=OFFSET_TestResult_Characteristics;
		#endif
			switch(p8[0])
			{
				case 0://合格
					p8tx[0]=0;
					return 1;
				case 1://不合格
					if(LENmax_TxSPACE<4)
					{
						UARTCtrl->DAR=11;//不处于分帧传输状态   （11）
						return 0;
					}
					Len=TestReport();//生成测试报告文件;返回:文件长度(生成测试报告文件在ADDR_128KWRITE_BUFF，并写入os文件系统)
					if(Comm_Ram->Factory==0x55)//工厂状态
					{
						LENmax_TxSPACE+=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲长度
					}
					if(Len>(LENmax_TxSPACE-4))
					{
						Len=LENmax_TxSPACE-4;
					}
					MR((u32)p8tx+4,ADDR_128KWRITE_BUFF,Len);
					Len+=4;
					break;
				default://没测试
					CopyString((u8*)"NO TEST",p8tx+Len);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
					Len+=7;
					break;
			}
			p8tx[2]=(Len-4)>>8;
			p8tx[3]=(Len-4);
			return Len;
		case 0xFFFF0300://读可充电电池插入状态
			i=MRR(ADDR_UBACKUP,2);
			if(i)
			{//有充电电池
				p8tx[0]=0;
				return 1;
			}
			p8tx[0]=DataType_visible_string;
			p8tx[1]=0x82;
			p8tx[2]=0;
			p8tx[3]=0;
			Len=4;
			CopyString((u8*)"ERROR",p8tx+Len);//拷贝字符串,字符串ps拷贝到pd,返回拷贝字符串的字节数
			Len+=5;
			p8tx[2]=(Len-4)>>8;
			p8tx[3]=(Len-4);
			return Len;
		default:
			UARTCtrl->DAR=6;//数据访问结果
			return 0;
	}
}

u32 ACTION_FFFF(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 LEN_TxAPDU;
	u32 *p32;
	UARTCtrl_TypeDef *UARTCtrl;
	#ifdef LINUX
	ms_Type *ms;//得到ms表地址
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(p8rx[2])
	{
		case 127://方法127：自检
			if(Comm_Ram->Factory==0x55)//工厂状态
			{
				if(FileDownload->File&(1<<4))//文件标志(置位表示OK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,...B31=
				{
					p32=(u32*)ADDR_Menu_ENTRY;
					if(p32[128]==8)
					{//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Test,5=Font,6=PACK,7=Router,8=TEST
						LEN_TxAPDU=9;
						Uart_698_TxResponse(PORTn,0,LEN_TxAPDU);//DL698发送响应数据,数据已在p8tx+TxAPDUOffset的开始地址中
						//BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
						while(1)
						{
						#ifndef LINUX
							p32=(u32*)ADDR_MS_ENTRY;
							(*(void(*)())p32[115])();//Main_Thread; 115://主线程
						#else
							ms=(ms_Type *)Get_ms();
							ms->Main_Thread();
						#endif
							if(UARTCtrl->Task==0)
							{
								break;
							}
						}
						UARTCtrl->Lock=0;
						p32=(u32*)ADDR_Menu_ENTRY;
					#ifndef LINUX
						(*(void(*)())p32[115])();//115://电气性能测试重启OS前
					#else
						(*(void(*)())p32[116])();//116://电气性能测试
					#endif
					}
				}
			}
			UARTCtrl->DAR=6;//对象不存在（6）
			return 0;
		case 128://方法128：退出工厂模式
			MWR(0,ADDR_MeterFMTimer+14,2);
			MWR(0,ADDR_TerminalFMTimer+14,2);
			//工厂状态退出时，当前通信地址保存为出厂通信地址
			MR(ADDR_DATABUFF,ADDR_4001,LENmax_4001_Factory);
			MW(ADDR_DATABUFF,ADDR_4001_Factory,LENmax_4001_Factory);
			return 0;
	}
	UARTCtrl->DAR=6;//对象不存在（6）
	return 0;
}



