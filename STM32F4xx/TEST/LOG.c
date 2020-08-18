

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif

#include "../TEST/LOG.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/NAND.h"
#include "../OS/OS.h"
#include "../OS/My_Heap.h"
#include "../Calculate/ascii.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


void BE_NAND(u32 ADDR_Dest)//NAND_FLASH 块擦除;没块替换
{
	ADDR_Dest-=ADDR_EFLASH_Start;
	BlockErase_NAND(ADDR_Dest,1);//NAND_FLASH 块擦除;没块替换;入口:Busy_Wait=1NAND忙时等待Busy_Wait=0不等待
}

//bus=16bit
#if NAND_BUS_WIDTH==16//仅OS和TEST需要,其他程序自动识别;8=8bit,16=16bit
void DR_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//nand没替换没ECC读
{
	u32 i;
	u32 PageByte;
	u16 *p16;
	u8 *p8DEST;
	u16 *p16DEST;
	
	ADDR_Sour-=ADDR_EFLASH_Start;
	p16=(u16*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	while(Byte!=0)
	{
		while(1)//掉电中断冲突重读
		{
			i=ADDR_Sour;
			i>>=1;
			p16[0x10000>>1]=0x00;//1st Cycle Page Read Command
			p16[0x20000>>1]=i;//Col.Add.1
			p16[0x20000>>1]=(i>>8)&0x03;//Col.Add.2
			p16[0x20000>>1]=(i>>10);//Row Add. 1
			p16[0x20000>>1]=(i>>18);//Row Add. 2
		#if (NAND_2G|NAND_4G|NAND_8G)
			p16[0x20000>>1]=(i>>26);//Row Add. 3
		#endif
			p16[0x10000>>1]=0x30;//2nd Cycle Cycle Page Read Command
			
			PageByte=2048-(ADDR_Sour&0x7FF);//当前Page要读字节
			if(PageByte>Byte)
			{
				PageByte=Byte;
			}
			Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
			p16[0x10000>>1]=0x00;//1st Cycle Page Read Command
			p8DEST=(u8*)ADDR_Dest;
			i=PageByte;
			if(ADDR_Sour&0x1)
			{
				*p8DEST=p16[0]>>8;
				p8DEST++;
				i--;
			}
			p16DEST=(u16*)p8DEST;
			while(i>=2)
			{
				*p16DEST=p16[0];
				p16DEST++;
				i-=2;
			}
			p8DEST=(u8*)p16DEST;
			if(i)
			{
				*p8DEST=p16[0];
				p8DEST++;
			}
			if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
			{
				Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
				continue;
			}
			break;
		}
		ADDR_Dest+=PageByte;
		ADDR_Sour+=PageByte;
		Byte-=PageByte;
	}
}
void DW_NAND(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//nand没替换没ECC写
{
	u32 i;
	u32 word;
	u32 PageByte;
	u16 *p16;
	u8 *p8SOUR;
	
	ADDR_Dest-=ADDR_EFLASH_Start;
	p16=(u16*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	while(Byte!=0)
	{
		while(1)//掉电中断冲突重读
		{
			i=ADDR_Dest;
			i>>=1;
			p16[0x10000>>1]=0x00;//1st Cycle Page Read Command
			p16[0x20000>>1]=i;//Col.Add.1
			p16[0x20000>>1]=(i>>8)&0x03;//Col.Add.2
			p16[0x20000>>1]=(i>>10);//Row Add. 1
			p16[0x20000>>1]=(i>>18);//Row Add. 2
		#if (NAND_2G|NAND_4G|NAND_8G)
			p16[0x20000>>1]=(i>>26);//Row Add. 3
		#endif
			p16[0x10000>>1]=0x30;//2nd Cycle Cycle Page Read Command
			PageByte=2048-(ADDR_Dest&0x7FF);//当前Page要读字节
			if(PageByte>Byte)
			{
				PageByte=Byte;
			}
			Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
			p16[0x10000>>1]=0x00;//1st Cycle Page Read Command
			if(ADDR_Dest&0x1)
			{
				word=p16[0]&0xff;
			}

			i=ADDR_Dest;
			i>>=1;
			p16[0x10000>>1]=0x80;//Command
			p16[0x20000>>1]=i;//Col.Add.1
			p16[0x20000>>1]=(i>>8)&0x3;//Col.Add.2
			p16[0x20000>>1]=(i>>10);//Row Add. 1
			p16[0x20000>>1]=(i>>18);//Row Add. 2
		#if (NAND_2G|NAND_4G|NAND_8G)
			p16[0x20000>>1]=(i>>26);//Row Add. 3
		#endif
			p8SOUR=(u8*)ADDR_Sour;
			i=PageByte;
			if(ADDR_Dest&0x1)
			{
				word|=(p8SOUR[0]<<8);
				p16[0]=word;
				i--;
				p8SOUR++;
			}
			while(i>=2)
			{
				p16[0]=p8SOUR[0]+(p8SOUR[1]<<8);
				p8SOUR+=2;
				i-=2;
			}
			if(i)
			{
				word=p8SOUR[0]|0xff00;
				p16[0]=word;
				p8SOUR++;
			}
			NVIC_DisableIRQ(EXTI3_IRQn);//禁止IRQ通道中断(电源下降)//NVIC_EnableIRQ(EXTI3_IRQn);//允许IRQ通道中断(电源下降)
			if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
			{
				Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
				continue;
			}
			p16[0x10000>>1]=0x10;//Command
			NVIC_EnableIRQ(EXTI3_IRQn);//允许IRQ通道中断(电源下降)
			Wait_NAND(700*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
			break;
		}
		ADDR_Sour+=PageByte;
		ADDR_Dest+=PageByte;
		Byte-=PageByte;
	}
}
#endif

//bus=8bit
#if NAND_BUS_WIDTH==8//仅OS和TEST需要,其他程序自动识别;8=8bit,16=16bit
void DR_NAND(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte)//nand没替换没ECC读
{
	u32 i;
	u32 x;
	u32 PageByte;
	u8 *p8;
	u8 *p8DEST;
	
	ADDR_Sour-=ADDR_EFLASH_Start;
	p8=(u8*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	while(Byte!=0)
	{
		while(1)//掉电中断冲突重读
		{
			i=ADDR_Sour;
			p8[0x10000]=0x00;//1st Cycle Page Read Command
			p8[0x20000]=i;//Col.Add.1
			p8[0x20000]=(i>>8)&0x07;//Col.Add.2
			p8[0x20000]=(i>>11);//Row Add. 1
			p8[0x20000]=(i>>19);//Row Add. 2
		#if (NAND_2G|NAND_4G|NAND_8G)
			p8[0x20000]=(i>>27);//Row Add. 3
		#endif
			p8[0x10000]=0x30;//2nd Cycle Cycle Page Read Command
			PageByte=2048-(ADDR_Sour&0x7FF);//当前Page要读字节
			if(PageByte>Byte)
			{
				PageByte=Byte;
			}
			Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
			p8[0x10000]=0x00;//1st Cycle Page Read Command
			p8DEST=(u8*)ADDR_Dest;
			i=PageByte;
			while(i--)
			{
				x=p8[0];
				*p8DEST=x;
				p8DEST++;
			}
			p8DEST-=PageByte;
			if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
			{
				Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
				continue;
			}
			break;
		}
		ADDR_Dest+=PageByte;
		ADDR_Sour+=PageByte;
		Byte-=PageByte;
	}
}
void DW_NAND(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte)//nand没替换没ECC写
{
	u32 i;
	u32 x;
	u32 PageByte;
	u8 *p8;
	u8 *p8SOUR;
	
	ADDR_Dest-=ADDR_EFLASH_Start;
	p8=(u8*)0x80000000;
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	while(Byte!=0)
	{
		while(1)//掉电中断冲突重读
		{
			i=ADDR_Dest;
			p8[0x10000]=0x00;//1st Cycle Page Read Command
			p8[0x20000]=i;//Col.Add.1
			p8[0x20000]=(i>>8)&0x07;//Col.Add.2
			p8[0x20000]=(i>>11);//Row Add. 1
			p8[0x20000]=(i>>19);//Row Add. 2
		#if (NAND_2G|NAND_4G|NAND_8G)
			p8[0x20000]=(i>>27);//Row Add. 3
		#endif
			p8[0x10000]=0x30;//2nd Cycle Cycle Page Read Command
			PageByte=2048-(ADDR_Dest&0x7FF);//当前Page要读字节
			if(PageByte>Byte)
			{
				PageByte=Byte;
			}
			Wait_NAND(70*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
			p8[0x10000]=0x00;//1st Cycle Page Read Command
			
			i=ADDR_Dest;
			p8[0x10000]=0x80;//1st Cycle Page Read Command
			p8[0x20000]=i;//Col.Add.1
			p8[0x20000]=(i>>8)&0x07;//Col.Add.2
			p8[0x20000]=(i>>11);//Row Add. 1
			p8[0x20000]=(i>>19);//Row Add. 2
		#if (NAND_2G|NAND_4G|NAND_8G)
			p8[0x20000]=(i>>27);//Row Add. 3
		#endif
			p8SOUR=(u8*)ADDR_Sour;
			i=PageByte;
			while(i--)
			{
				x=*p8SOUR;
				p8[0]=x;
				p8SOUR++;
			}
			NVIC_DisableIRQ(EXTI3_IRQn);//禁止IRQ通道中断(电源下降)//NVIC_EnableIRQ(EXTI3_IRQn);//允许IRQ通道中断(电源下降)
			if(Comm_Ram->IntFlags&0x08)//中断服务中使用冲突标志
			{
				Comm_Ram->IntFlags=0;//中断服务中使用冲突标志
				continue;
			}
			p8[0x10000]=0x10;//Command
			NVIC_EnableIRQ(EXTI3_IRQn);//允许IRQ通道中断(电源下降)
			Wait_NAND(700*3);//读NAND状态寄存器等待当前命令完成,返回:0=正确,!=0读取的状态字
			break;
		}
		ADDR_Sour+=PageByte;
		ADDR_Dest+=PageByte;
		Byte-=PageByte;
	}
}
#endif


void Init_LOG(void)//LOG上电初始化(在FS初始化后进行)
{
	os_Type *os;
	
	if(Comm_Ram->Init_LOG!=0)//上电后LOG已初始化标志:0=没，0x55=正在初始化,0xAA=初始化完成
	{
		return;
	}
	Comm_Ram->Init_LOG=0x55;//上电后LOG已初始化标志:0=没，0x55=正在初始化,0xAA=初始化完成
	Comm_Ram->DisplayScanTimer=5000/10;//死机最大时间// 27 显示扫描 每10MS减1定时器(终端)
	os=Get_os();
#if LOG_698==1
	LOG_FILE(698,0);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#else
	os->fdelete("N0:/TEST/DL698.log",NULL);//删除文件
	os->fdelete("N0:/TEST/DL698-old.log",NULL);//删除文件
#endif
#if LOG_1376_1==1//0=没,1=有
	LOG_FILE(13761,0);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#else
	//南网现场运行发现在下条死机
	os->fdelete("N0:/TEST/GDW1376-1.log",NULL);//删除文件
	os->fdelete("N0:/TEST/GDW1376-1-old.log",NULL);//删除文件
#endif
#if LOG_1376_2==1//0=没,1=有
	LOG_FILE(13762,0);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#else
	os->fdelete("N0:/TEST/GDW1376-2.log",NULL);//删除文件
	os->fdelete("N0:/TEST/GDW1376-2-old.log",NULL);//删除文件
#endif
#if LOG_1376_3==1//0=没,1=有
	LOG_FILE(13763,0);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#else
	os->fdelete("N0:/TEST/GDW1376-3.log",NULL);//删除文件
	os->fdelete("N0:/TEST/GDW1376-3-old.log",NULL);//删除文件
#endif
#if LOG_645_1997==1//0=没,1=有
	LOG_FILE(6451997,0);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#else
	os->fdelete("N0:/TEST/DL645-1997.log",NULL);//删除文件
	os->fdelete("N0:/TEST/DL645-1997-old.log",NULL);//删除文件
#endif
#if LOG_645_2007==1//0=没,1=有
	LOG_FILE(6452007,0);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#else
	os->fdelete("N0:/TEST/DL645-2007.log",NULL);//删除文件
	os->fdelete("N0:/TEST/DL645-2007-old.log",NULL);//删除文件
#endif
	Comm_Ram->Init_LOG=0xAA;//上电后LOG已初始化标志:0=没，0x55=正在初始化,0xAA=初始化完成
	Comm_Ram->DisplayScanTimer=0;//死机最大时间// 27 显示扫描 每10MS减1定时器(终端)
}

void LOG(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
#if (LOG_698|LOG_1376_1|LOG_1376_2|LOG_1376_3|LOG_645_1997|LOG_645_2007)
	u32 i;
	u32 x;
	u32 *RTC_BKPxR;
	u32 ADDR_NAND;
	char *p8str;
	
	if(Byte>((128*1024)/4))
	{//长度太长错
		return;
	}
	switch(protocol)
	{
		case 13761:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*4));
			ADDR_NAND=ADDR_LOG_1376_1;
			break;
		case 13762:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*5));
			ADDR_NAND=ADDR_LOG_1376_2;
			break;
		case 13763:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*6));
			ADDR_NAND=ADDR_LOG_1376_3;
			break;
		case 6451997:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*7));
			ADDR_NAND=ADDR_LOG_645_1997;
			break;
		case 6452007:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*8));
			ADDR_NAND=ADDR_LOG_645_2007;
			break;
		case 698:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*9));
			ADDR_NAND=ADDR_LOG_698;
			break;
		default:
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
	if(*RTC_BKPxR>(128*1024))
	{//计数错误
		BE_NAND(ADDR_NAND);//NAND_FLASH 块擦除;没块替换
		*RTC_BKPxR=0;
	}
	ADDR_NAND+=*RTC_BKPxR;
	i=((u32)p8str)-(ADDR_128KDATABUFF);
	if(i<=((128*1024)-(*RTC_BKPxR)))
	{
		DW_NAND(ADDR_128KDATABUFF,ADDR_NAND,i);//nand没替换没ECC写
		*RTC_BKPxR+=i;
		//DR_NAND(ADDR_128KDATABUFF,ADDR_LOG_698,128*1024);
		return;
	}
	//一起写入文件
	LOG_FILE(protocol,i);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;nand中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#endif
}

void LOG_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
#if (LOG_698|LOG_1376_1|LOG_1376_2|LOG_1376_3|LOG_645_1997|LOG_645_2007)
	u32 i;
	u32 x;
	u32 *RTC_BKPxR;
	u32 ADDR_NAND;
	char *p8str;
	
	if(Byte>((128*1024)/3))
	{//长度太长错
		return;
	}
	switch(protocol)
	{
		case 13761:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*4));
			ADDR_NAND=ADDR_LOG_1376_1;
			break;
		case 13762:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*5));
			ADDR_NAND=ADDR_LOG_1376_2;
			break;
		case 13763:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*6));
			ADDR_NAND=ADDR_LOG_1376_3;
			break;
		case 6451997:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*7));
			ADDR_NAND=ADDR_LOG_645_1997;
			break;
		case 6452007:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*8));
			ADDR_NAND=ADDR_LOG_645_2007;
			break;
		case 698:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*9));
			ADDR_NAND=ADDR_LOG_698;
			break;
		default:
			return;
	}
	p8str=(char*)ADDR_128KDATABUFF;

	if(*RTC_BKPxR>(128*1024))
	{//计数错误
		BE_NAND(ADDR_NAND);//NAND_FLASH 块擦除;没块替换
		*RTC_BKPxR=0;
	}
	ADDR_NAND+=*RTC_BKPxR;
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
	if(i<=((128*1024)-(*RTC_BKPxR)))
	{
		DW_NAND(ADDR_128KDATABUFF,ADDR_NAND,i);//nand没替换没ECC写
		*RTC_BKPxR+=i;
//		DR_NAND(ADDR_128KDATABUFF,ADDR_LOG_1376_3,128*1024);
		return;
	}
	//一起写入文件
	LOG_FILE(protocol,i);//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;nand中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
#endif
}

void LOG_ATRx(u32 link)//GPRS模块AT接收数据LOG;入口:link=0新log,=1接上log;使用ADDR_DATABUFF
{
#if (LOG_698|LOG_1376_1|LOG_1376_2|LOG_1376_3|LOG_645_1997|LOG_645_2007)
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
#endif
}



const char filename_698[]={"N0:/TEST/DL698.log"};
const char filename_698_old[]={"N0:/TEST/DL698-old.log"};
const char filename_698_new[]={"DL698-old.log"};

const char filename_1376_1[]={"N0:/TEST/GDW1376-1.log"};
const char filename_1376_1_old[]={"N0:/TEST/GDW1376-1-old.log"};
const char filename_1376_1_new[]={"GDW1376-1-old.log"};

const char filename_1376_2[]={"N0:/TEST/GDW1376-2.log"};
const char filename_1376_2_old[]={"N0:/TEST/GDW1376-2-old.log"};
const char filename_1376_2_new[]={"GDW1376-2-old.log"};

const char filename_1376_3[]={"N0:/TEST/GDW1376-3.log"};
const char filename_1376_3_old[]={"N0:/TEST/GDW1376-3-old.log"};
const char filename_1376_3_new[]={"GDW1376-3-old.log"};

const char filename_645_1997[]={"N0:/TEST/DL645-1997.log"};
const char filename_645_1997_old[]={"N0:/TEST/DL645-1997-old.log"};
const char filename_645_1997_new[]={"DL645-1997-old.log"};

const char filename_645_2007[]={"N0:/TEST/DL645-2007.log"};
const char filename_645_2007_old[]={"N0:/TEST/DL645-2007-old.log"};
const char filename_645_2007_new[]={"DL645-2007-old.log"};

void LOG_FILE(u32 protocol,u32 len)//LOG nand和ADDR_128KDATABUFF数据写入文件;入口:protocol=13761,13762,13763,6451997,6452007;FLASH中长度已在RTC_BKPxR中,len=ADDR_128KDATABUFF中长度(当无此数据时用0)
{
#if (LOG_698|LOG_1376_1|LOG_1376_2|LOG_1376_3|LOG_645_1997|LOG_645_2007)
	u32 i;
	u32 *RTC_BKPxR;
	u32 ADDR_NAND;
	char *filename;
	char *oldfilename;
	char *newname;
	FILE *fsfile;
	fsFileInfo info;
	os_Type *os;
	void *p8s;

	
	os=Get_os();
	info.fileID=0;
	info.size=0;
	switch(protocol)
	{
		case 13761:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*4));
			ADDR_NAND=ADDR_LOG_1376_1;
			os->ffind(filename_1376_1,&info);
			fsfile=os->fopen(filename_1376_1,"ab");
			filename=(char*)filename_1376_1;
			oldfilename=(char*)filename_1376_1_old;
			newname=(char*)filename_1376_1_new;
			break;
		case 13762:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*5));
			ADDR_NAND=ADDR_LOG_1376_2;
			os->ffind(filename_1376_2,&info);
			fsfile=os->fopen(filename_1376_2,"ab");
			filename=(char*)filename_1376_2;
			oldfilename=(char*)filename_1376_2_old;
			newname=(char*)filename_1376_2_new;
			break;
		case 13763:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*6));
			ADDR_NAND=ADDR_LOG_1376_3;
			os->ffind(filename_1376_3,&info);
			fsfile=os->fopen(filename_1376_3,"ab");
			filename=(char*)filename_1376_3;
			oldfilename=(char*)filename_1376_3_old;
			newname=(char*)filename_1376_3_new;
			break;
		case 6451997:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*7));
			ADDR_NAND=ADDR_LOG_645_1997;
			os->ffind(filename_645_1997,&info);
			fsfile=os->fopen(filename_645_1997,"ab");
			filename=(char*)filename_645_1997;
			oldfilename=(char*)filename_645_1997_old;
			newname=(char*)filename_645_1997_new;
			break;
		case 6452007:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*8));
			ADDR_NAND=ADDR_LOG_645_2007;
			os->ffind(filename_645_2007,&info);
			fsfile=os->fopen(filename_645_2007,"ab");
			filename=(char*)filename_645_2007;
			oldfilename=(char*)filename_645_2007_old;
			newname=(char*)filename_645_2007_new;
			break;
		case 698:
			RTC_BKPxR=(u32*)(ADDR_RTCBKPSRAM_START+(4*9));
			ADDR_NAND=ADDR_LOG_698;
			os->ffind(filename_698,&info);
			fsfile=os->fopen(filename_698,"ab");
			filename=(char*)filename_698;
			oldfilename=(char*)filename_698_old;
			newname=(char*)filename_698_new;
			break;
		default:
			return;
	}
	if(fsfile==NULL)
	{
		return;
	}
	if(*RTC_BKPxR>(128*1024))
	{//计数错误
		BE_NAND(ADDR_NAND);//NAND_FLASH 块擦除;没块替换
		*RTC_BKPxR=0;
	}
	i=info.size;
	if(i>LENmax_LOG)
	{//每个文件最大尺寸限制
		os->fclose(fsfile);
		os->fdelete(oldfilename,NULL);//删除old文件
		os->frename(filename,newname);
		fsfile=os->fopen(filename,"ab");
		if(fsfile==NULL)
		{
			return;
		}
	}
	i=os->ffree("N0:");
	if(i<((*RTC_BKPxR)+len))
	{//nand空间不够
		os->fdelete(oldfilename,NULL);//删除old文件
		i=os->ffree("N0:");
		if(i<((*RTC_BKPxR)+len))
		{//还不够
			os->fclose(fsfile);
			
			BE_NAND(ADDR_NAND);//NAND_FLASH 块擦除;没块替换
			*RTC_BKPxR=0;
			return;
		}
	}
	
	if((*RTC_BKPxR)|len)
	{
		p8s=ms_malloc(128*1024);//动态分配我的内存堆
		if(p8s)
		{
			DR_NAND((u32)p8s,ADDR_NAND,*RTC_BKPxR);//nand没替换没ECC读
			os->fseek(fsfile,0,SEEK_END);
			os->fwrite(p8s,1,*RTC_BKPxR,fsfile);
			ms_free(p8s);//释放我的内存堆分配
			os->fwrite((void*)ADDR_128KDATABUFF,1,len,fsfile);
			
			BE_NAND(ADDR_NAND);//NAND_FLASH 块擦除;没块替换
			*RTC_BKPxR=0;
		}
	}
	os->fclose(fsfile);
#endif
}











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

	if(Byte>((LEN_UART0Tx/2)))
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
}

void LOGRS232_LINK(u32 protocol,u32 TR,u32 PORTn,u8 *pBUF,u32 Byte,u32 ascii)//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
{
	u32 i;
	u32 x;
	char *p8str;
	
	if(Byte>((LEN_UART0Tx/2)))
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




