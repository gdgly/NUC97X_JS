
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../MS/SoftwareUpdate.h"
#include "../MS/SoftwareCheck.h"
#include "../Device/NAND.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_FLASH.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../OS/OS.h"
#include "../Calculate/ascii.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/Data_Save.h"


u32 SoftwareUpdateRW(char *FileName,u32 *p32s,u32 *p32d,u32 LEN_Program_Max,u32 Same,u32 SoftwareNo)//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
{
	u32 i;
	u32 LEN_SourFile;
	u32 LEN_File;
	FILE *file;
	u8 *p8;
	os_Type *os;
	fsStatus fs;

	
	WWDT_Enable_Feed(32000);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	os=Get_os();
	LEN_File=0;
	file=os->fopen(FileName,"r");
	if(file!=NULL)
	{
		LEN_SourFile=os->fread((void *)p32s,1,LEN_Program_Max,file);
		os->fclose(file);
		if((p32s[0]&0xff)==0)
		{//非压缩
			MW((u32)p32s,(u32)p32d,LEN_SourFile);
			LEN_File=LEN_SourFile;
		}
		else
		{//Lzma压缩
			p8=(u8*)p32s;
			i=p8[5]+(p8[6]<<8)+(p8[7]<<16)+(p8[8]<<24);
			if(i<=LEN_Program_Max)
			{
				LEN_File=os->RAM_LzmaDecode((u32)p32s,LEN_SourFile,(u32)p32d,i);//RAM->RAM的Lzma解压缩算法;返回:解压缩后的长度
				if(LEN_File!=i)
				{
					LEN_File=0;
				}
			}
		}
		if(LEN_File>(4*132))
		{
			i=SoftwareCheck(p32d);//软件检查;入口:软件入口地址指针,返回:0=错误,1=正确
			if(i==0)//软件检查;入口:软件入口地址指针,返回:0=错误,1=正确
			{
				LEN_File=0;
			}
			else
			{
				if(p32d[128]==SoftwareNo)//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
				{
					if(p32d[125]!=0)//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
					{
						Same&=p32d[125];
						if(Same!=p32d[125])
						{
							return 1;//还没同时下载完成,返回1表示有本文件
						}
					}
					os->fchdir("N0:/Program Files");
					file=os->fopen(FileName,"w");
					if(file!=NULL)
					{
						os->fwrite((void *)p32s,1,LEN_SourFile,file);
						os->fclose(file);
					}
					
					if(SoftwareNo<=1)//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
					{//是MS,OS更新
						//MW((u32)p32d,ADDR_Download_ENTRY,LEN_File);//在备用区保存
						RePlace_Write_NAND((u32)p32d,ADDR_Download_ENTRY-ADDR_EFLASH_Start,LEN_File);//NAND_FLASH写;若错误则块替换
						//MS,OS更新需马上重启动,先保存各文件标志
						Same|=(1<<SoftwareNo);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
						RTC->BKP1R=Same;//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
						//先删除文件
						os->fchdir("N0:/Download");
						os->fdelete(FileName,NULL);//删除文件
						if(SoftwareNo==0)//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
						{//MS在BOOT更新
							Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"MS                OK");
							i=p32d[124];//124//SVER;//软件版本号
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=0;//语言选择
							p8[21]=0;//结束符
							p8+=13;
							p8[0]='V';
							p8[1]=(i>>8)+0x30;
							p8[2]='.';
							p8[3]=((i>>4)&0xf)+0x30;
							DisplayStringAutoUp(0,9,(u8*)ADDR_DATABUFF);//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
						}
						else
						{//OS
							// Disable SysTick timer                                               
							SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
							//USB-HS,ETHERNET 复位
							RCC->AHB1RSTR=(1<<29)+(1<<25);
							//USB-FS 复位
							RCC->AHB2RSTR=(1<<7);
							//清复位
							RCC->AHB1RSTR=0;
							RCC->AHB2RSTR=0;

							__disable_irq();//关中断
							Write_STM32F4xx_FLASH((u32)p32d,p32d[110],LEN_File);//FLASH写;使用ADDR_DATABUFF,ADDR_4KWRITE_BUFF,0x10089FE0-0x10089FFF=32Byte
							__enable_irq();//允许中断
							//MC(0,ADDR_Download_ENTRY,4*132);//清备份区
							RePlace_Clr_NAND(0,ADDR_Download_ENTRY-ADDR_EFLASH_Start,4*32);//NAND_FLASH清0;若错误则块替换
							Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"OS                OK");
							i=p32d[124];//124//SVER;//软件版本号
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=0;//语言选择
							p8[21]=0;//结束符
							p8+=13;
							p8[0]='V';
							p8[1]=(i>>8)+0x30;
							p8[2]='.';
							p8[3]=((i>>4)&0xf)+0x30;
							p8[4]=(i&0xf)+0x30;
							DisplayStringAutoUp(0,9,(u8*)ADDR_DATABUFF);//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
							SoftDelayMS(1000);//软件ms延时
							
							(*(void(*)())p32d[113])();//软件模块更新初始化入口
							ReSTART_OS();//重启OS
						}
						return 2;//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
					}
					if(SoftwareNo!=7)//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
					{//内部FLASH
						__disable_irq();//关中断
						Write_STM32F4xx_FLASH((u32)p32d,p32d[110],LEN_File);//FLASH写;使用ADDR_DATABUFF,ADDR_4KWRITE_BUFF,0x10089FE0-0x10089FFF=32Byte
						__enable_irq();//允许中断
					}
					else
					{//外部FLASH或SDRAM
						MW((u32)p32d,p32d[110],LEN_File);
					}
					FileDownload->File|=(1<<SoftwareNo);//文件标志(置位表示OK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,...B31=
					Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
					switch(SoftwareNo)
					{
						case 2:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Meter             OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Meter Update      OK");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
							break;
						case 3:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Terminal          OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Terminal Update   OK");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
							break;
						case 4:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Menu              OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Menu Update       OK");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
							break;
						case 5:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Font              OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Font Update       OK");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
							break;
						case 7:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Module            OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Module Update     OK");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
							break;
						case 8:
							StrCopy((char *)ADDR_DATABUFF+1,(char *)"Test              OK");
							//DisplayStringAutoUp(0,9,(u8*)"\x0""Test Update       OK");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
							break;
					}
					i=p32d[124];//124//SVER;//软件版本号
					p8=(u8*)ADDR_DATABUFF;
					p8[0]=0;//语言选择
					p8[21]=0;//结束符
					p8+=13;
					p8[0]='V';
					p8[1]=(i>>8)+0x30;
					p8[2]='.';
					p8[3]=((i>>4)&0xf)+0x30;
					p8[4]=(i&0xf)+0x30;
					DisplayStringAutoUp(0,9,(u8*)ADDR_DATABUFF);//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				}
				else
				{
					LEN_File=0;
				}
			}
		}
	}
	fs=os->fchdir("N0:/Download");
	fs=fs;
	fs=os->fdelete(FileName,NULL);//删除文件
	fs=fs;
	
	if(LEN_File==0)
	{
		Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
		switch(SoftwareNo)
		{
			case 0:
				DisplayStringAutoUp(0,9,(u8*)"\x0""MS Update       FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
			case 1:
				DisplayStringAutoUp(0,9,(u8*)"\x0""OS Update       FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
			case 2:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Meter Update    FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
			case 3:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Terminal Update FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
			case 4:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Menu Update     FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
			case 5:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Font Update     FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
			case 7:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Module Update   FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
			case 8:
				DisplayStringAutoUp(0,9,(u8*)"\x0""Test Update     FAIL");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
				break;
		}
	}
	return LEN_File;
}

fsStatus FileFind (const char *FileFactory,const char *FileUser ,fsFileInfo *info)
{
	os_Type *os;
	
	os=Get_os();
	info->fileID = 0; 
	if(os->ffind(FileFactory,info)==fsOK)
	{
		return fsOK;
	}
	info->fileID = 0; 
	if(os->ffind(FileUser,info)==fsOK)
	{
		return fsOK;
	}
	return fsFileNotFound;
}




void SoftwareUpdate(void)//软件更新
{
	u32 i;
	u32 x;
	u32 y;
	u32 Same;//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
	u32 Reset;//更新后复位标志
	fsFileInfo info;
	FILE *file;
	u32 LEN_SourFile;
	u32 LEN_File;
	u8* p8;
	u32 *p32s;
	u32 *p32d;
	os_Type *os;
	UARTCtrl_TypeDef *UARTCtrl;
	
	os=Get_os();

	i=RTC->BKP0R&0xff;//FileDownload->USB0Task;//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
	//USB0主机
	if(((i==2)||(i==3)))//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
	{
		return;//USB0主机正在操作
	}
	//USB0设备
	if((i==0)||(i>=6))//任务:0=空,1=等待插入,2=主机插入初始化等待,3=主机插入,4=主机拨出,5=主机结束,6=设备插入,7=设备拨出
	{//6=设备插入
		return;
	}

	if(FileDownload->New==0)//有新文件载入标志:0=有,1=没
	{
		i=FileDownload->PORTn;
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(i);
		if(UARTCtrl->Task!=0)
		{//不是空闲
			return;
		}
		p32s=ms_malloc((size_t)1024*1024);//动态分配我的内存堆
		p32d=ms_malloc((size_t)2048*1024);//动态分配我的内存堆
		if((p32s==0)||(p32d==0))
		{
			Init_my_heap();//初始化与内核RTOS无关的我的内存堆
			p32s=ms_malloc((size_t)1024*1024);//动态分配我的内存堆
			p32d=ms_malloc((size_t)2048*1024);//动态分配我的内存堆
		}
		
		os->fchdrive ("N0:");
		os->fchdir("N0:/Download");
		
//若有打包文件则先解压文件
		info.fileID=0;
		i=os->ffind(FileFactory_PACK,&info);
		if(i==fsOK)
		{
			file=os->fopen((char*)&info.name,"r");
			if(file!=NULL)
			{
				LEN_SourFile=os->fread((void *)p32s,1,1024*1024,file);
				LEN_File=0;
				os->fclose(file);
				if(p32s[0]&0xff)
				{//Lzma压缩
					p8=(u8*)p32s;
					i=p8[5]+(p8[6]<<8)+(p8[7]<<16)+(p8[8]<<24);
					if(i<=(2*1024*1024))
					{
						LEN_File=os->RAM_LzmaDecode((u32)p32s,LEN_SourFile,(u32)p32d,i);//RAM->RAM的Lzma解压缩算法;返回:解压缩后的长度
						if(LEN_File!=i)
						{
							LEN_File=0;
						}
					}
				}
				if(LEN_File>(4*132))
				{
					for(i=0;i<32;i++)
					{
						y=p32d[2+(2*i)];
						x=p32d[3+(2*i)];
						if((x>=0x00200000)||(y>=0x00200000)||(x<y))
						{
							continue;
						}
						x-=y;
						if(x>(4*132))
						{
							y>>=1;
							y<<=1;
							y+=(u32)p32d;
					#if MainProtocol==376
							switch(i)
							{
								case 0:
									file=os->fopen("MS.bin","w");
									break;
								case 1:
									file=os->fopen("OS.bin","w");
									break;
								case 2:
									file=os->fopen("Meter.bin","w");
									break;
								case 3:
									file=os->fopen("Terminal.bin","w");
									break;
								case 4:
									file=os->fopen("Menu.bin","w");
									break;
								case 5:
									file=os->fopen("Font.bin","w");
									break;
								case 6:
									file=os->fopen("Test.bin","w");
									break;
								default:
									file=NULL;
									break;
							}
					#endif
					#if MainProtocol==698
							switch(i)
							{
								case 0:
									file=os->fopen("MS698.bin","w");
									break;
								case 1:
									file=os->fopen("OS698.bin","w");
									break;
								case 2:
									file=os->fopen("Meter698.bin","w");
									break;
								case 3:
									file=os->fopen("Terminal698.bin","w");
									break;
								case 4:
									file=os->fopen("Menu698.bin","w");
									break;
								case 5:
									file=os->fopen("Font.bin","w");
									break;
								case 6:
									file=os->fopen("Test698.bin","w");
									break;
								default:
									file=NULL;
									break;
							}
					#endif
					#if MainProtocol==CSG
							switch(i)
							{
								case 0:
									file=os->fopen("MSCSG.bin","w");
									break;
								case 1:
									file=os->fopen("OSCSG.bin","w");
									break;
								case 2:
									file=os->fopen("MeterCSG.bin","w");
									break;
								case 3:
									file=os->fopen("TerminalCSG.bin","w");
									break;
								case 4:
									file=os->fopen("MenuCSG.bin","w");
									break;
								case 5:
									file=os->fopen("Font.bin","w");
									break;
								case 6:
									file=os->fopen("TestCSG.bin","w");
									break;
								default:
									file=NULL;
									break;
							}
					#endif
							if(file!=NULL)
							{
								os->fwrite((void *)y,1,x,file);
								os->fclose(file);
							}
						}
					}
				}
				os->fdelete((char*)&info.name,NULL);//删除文件
			}
		}
		
//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=PACK,7=Router,8=Test
		Same=RTC->BKP1R;//已更新的需同时更新文件:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
		Reset=0;//更新后复位标志
//第1次扫描MS,OS,Meter,Terminal,Menu,Font
		//Font
		if(FileFind(FileFactory_Font,FileUser_Font,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Font,Same,5);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<5);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
			}
		}
		//Menu
		if(FileFind(FileFactory_Menu,FileUser_Menu,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Menu,Same,4);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<4);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				}
			}
		}
		//Terminal
		if(FileFind(FileFactory_Terminal,FileUser_Terminal,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Terminal,Same,3);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<3);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				#if MainProtocol==698
					(*(void(*)())p32d[117])();//终端版本变更事件
				#endif
				}
			}
		}
		//Meter
		if(FileFind(FileFactory_Meter,FileUser_Meter,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Meter,Same,2);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<2);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				}
			}
		}
		//OS
		if(FileFind(FileFactory_OS,FileUser_OS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_OS,Same,1);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				//OS更新时重启了OS，不会返回到下代码
				Same|=(1<<1);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				}
			}
		}
		//MS
		if(FileFind(FileFactory_MS,FileUser_MS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_MS,Same,0);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{//MS在BOOT更新
				Same|=(1<<0);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					SoftDelayMS(2000);//软件ms延时
					p32s=(u32 *)(0x08000000);
					(*(void(*)())p32s[110])((u32)p32d,p32d[111]-p32d[110]);
				}
			}
		}
//第2次扫描MS,OS,Meter,Terminal,Menu,Test,Font
		//Font
		if(FileFind(FileFactory_Font,FileUser_Font,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Font,Same,5);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<5);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
			}
		}
		//Menu
		if(FileFind(FileFactory_Menu,FileUser_Menu,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Menu,Same,4);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<4);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				}
			}
		}
		//Test
		if(FileFind(FileFactory_Test,FileFactory_Test,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Menu,Same,8);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<4);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				}
			}
		}
		//Terminal
		if(FileFind(FileFactory_Terminal,FileUser_Terminal,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Terminal,Same,3);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<3);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				#if MainProtocol==698
					(*(void(*)())p32d[117])();//终端版本变更事件
				#endif
				}
			}
		}
		//Meter
		if(FileFind(FileFactory_Meter,FileUser_Meter,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Meter,Same,2);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				Same|=(1<<2);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				}
			}
		}
		//OS
		if(FileFind(FileFactory_OS,FileUser_OS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_OS,Same,1);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{
				//OS更新时重启了OS，不会返回到下代码
				Same|=(1<<1);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					Reset|=p32d[129];//更新后复位标志
					(*(void(*)())p32d[113])();//软件模块更新初始化入口
				}
			}
		}
		//MS
		if(FileFind(FileFactory_MS,FileUser_MS,&info)==fsOK)
		{
			i=SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_MS,Same,0);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			if(i!=0)
			{//MS在BOOT更新
				Same|=(1<<0);//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
				if(i>1)
				{//其他值=更新
					SoftDelayMS(2000);//软件ms延时
					p32s=(u32 *)(0x08000000);
					(*(void(*)())p32s[110])((u32)p32d,p32d[111]-p32d[110]);
				}
			}
		}
		
	//PLC,总是在MS,OS后检查，因MS,OS更新会先暂存到相同的备用区保存(ADDR_Backup_ENTRY),同时以上更新可能需重启而中断与模块的数据传输
	#if (((Project/100)==2)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=,500=集中器Ⅱ型,600=,700=
		switch(Comm_Ram->RouterType)////路由器类型:0=保留,1=正泰,2=鼎信,3=东软,4=晓晨,5=瑞斯康,6=力合微,7=讯能,8=钱龙,100=保留,101=麦希,102=友讯达,103=有方,104=桑锐
		{
			case 1:
				i=FileFind(FileFactory_PLC_50,FileUser_PLC_50,&info);
				break;
			case 2:
				i=FileFind(FileFactory_PLC_TC,FileUser_PLC_TC,&info);
				break;
			case 3:
				i=FileFind(FileFactory_PLC_ES,FileUser_PLC_ES,&info);
				break;
			case 4:
				i=FileFind(FileFactory_PLC_CX,FileUser_PLC_CX,&info);
				break;
			case 5:
				i=FileFind(FileFactory_PLC_01,FileUser_PLC_01,&info);
				break;
			case 6:
				i=FileFind(FileFactory_PLC_LH,FileUser_PLC_LH,&info);
				break;
			case 7:
				i=FileFind(FileFactory_PLC_NX,FileUser_PLC_NX,&info);
				break;
			case 8:
				i=FileFind(FileFactory_PLC_QL,FileUser_PLC_QL,&info);
				break;
			case 101:
				i=FileFind(FileFactory_PLC_MX,FileUser_PLC_MX,&info);
				break;
			case 102:
				i=FileFind(FileFactory_PLC_FC,FileUser_PLC_FC,&info);
				break;
			case 103:
				i=FileFind(FileFactory_PLC_NW,FileUser_PLC_NW,&info);
				break;
			case 104:
				i=FileFind(FileFactory_PLC_SR,FileUser_PLC_SR,&info);
				break;
			default:
				i=fsFileNotFound;
				break;
		}
		if(i==fsOK)
		{
			SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Download,Same,7);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
		}
		else
		{
			i=FileFind(FileFactory_Modele,FileUser_MODULE,&info);
			if(i==fsOK)
			{
				SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Download,Same,7);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
			}
		}
	#endif
	#if (((Project/100)==3)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=,500=集中器Ⅱ型,600=,700=
		i=FileFind(FileFactory_Modele,FileUser_MODULE,&info);
		if(i==fsOK)
		{
			SoftwareUpdateRW((char*)&info.name,p32s,p32d,LENmax_Download,Same,7);//软件更新读写;返回:0=没更新,1=文件正确但需同时更新,其他值=更新
		}
	#endif
		RTC->BKP1R=0;//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
		FileDownload->New=1;//有新文件载入标志:0=有,1=没
		//更新后复位
		Reset>>=8;
		Reset&=0xff;
		if(Reset!=0)//更新后复位标志
		{
			SoftDelayMS(2000);//软件ms延时
			WWDT_RESET();//立即使能看门狗复位
		}
		ms_free(p32s);//释放我的内存堆分配
		ms_free(p32d);//释放我的内存堆分配
		
	}
}

























