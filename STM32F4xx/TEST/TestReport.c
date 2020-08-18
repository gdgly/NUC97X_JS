

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../TEST/TestReport.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/ascii.h"
#include "../OS/OS.h"




//POWER测试报告
char *TestReport_POWER(char *p8str,u32 ADDR_BASE)//POWER测试报告
{
	u32 i;
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***POWER***\r\n");
//VDD5V0
	p8=(u8*)ADDR_BASE+OFFSET_POWER_Characteristics;
	p8str=StrCopy(p8str,"VDD5V0  Min=4.80V  Typ=4.90V  Max=5.00V     Test=");
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
	}
	else
	{
		p8str+=BCD_ASCII(p8[1],(u32)p8str);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		p8str=StrCopy(p8str,".");
		p8str=Byte_ascii(p8[0],p8str);
		p8str=StrCopy(p8str,"V");
		i=p8[0]+(p8[1]<<8);
		if((i<0x480)||(i>0x500))
		{
			p8str=StrCopy(p8str,"   *FAIL*\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"   OK\r\n");
		}
	}
//RTC_BATTERY3V0
	p8=(u8*)ADDR_BASE+OFFSET_POWER_Characteristics+6;
	p8str=StrCopy(p8str,"RTC3V0  Min=3.10V  Typ=3.20V  Max=3.40V     Test=");
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
	}
	else
	{
		p8str+=BCD_ASCII(p8[1],(u32)p8str);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		p8str=StrCopy(p8str,".");
		p8str=Byte_ascii(p8[0],p8str);
		p8str=StrCopy(p8str,"V");
		i=p8[0]+(p8[1]<<8);
		if((i<0x310)||(i>0x340))
		{
			p8str=StrCopy(p8str,"   *FAIL*\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"   OK\r\n");
		}
	}
//GPRS4V0
	p8=(u8*)ADDR_BASE+OFFSET_POWER_Characteristics+10;
	p8str=StrCopy(p8str,"GPRS4V0  Min=3.90V  Typ=4.00V  Max=4.10V     Test=");
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
	}
	else
	{
		p8str+=BCD_ASCII(p8[1],(u32)p8str);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		p8str=StrCopy(p8str,".");
		p8str=Byte_ascii(p8[0],p8str);
		p8str=StrCopy(p8str,"V");
		i=p8[0]+(p8[1]<<8);
		if((i<0x390)||(i>0x410))
		{
			p8str=StrCopy(p8str,"   *FAIL*\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"   OK\r\n");
		}
	}
//ETH3V3
	p8=(u8*)ADDR_BASE+OFFSET_POWER_Characteristics+12;
	p8str=StrCopy(p8str,"ETH3V3  Min=3.20V  Typ=3.30V  Max=3.40V     Test=");
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
	}
	else
	{
		p8str+=BCD_ASCII(p8[1],(u32)p8str);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		p8str=StrCopy(p8str,".");
		p8str=Byte_ascii(p8[0],p8str);
		p8str=StrCopy(p8str,"V");
		i=p8[0]+(p8[1]<<8);
		if((i<0x320)||(i>0x340))
		{
			p8str=StrCopy(p8str,"   *FAIL*\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"   OK\r\n");
		}
	}
	
	return p8str;
}

//POWER_DOWN测试报告
char *TestReport_POWER_DOWN(char *p8str,u32 ADDR_BASE)//POWER_DOWN测试报告
{
	u32 i;
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***OFF POWER***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_POWER_DOWN_Characteristics;
	if((p8[0]==0xff)||(p8[0]==0x55))
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//1BYTE 4K(BKPSRAM)掉电保持,0=OK,1=ERR,0xff=没测试,0x55=进行测试
	p8str=StrCopy(p8str,"BKPSRAM     ");
	if(p8[0])
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
//1BYTE 电源掉电数据恢复,0=OK,1=ERR,0xff=没测试,0x55=进行测试
	p8str=StrCopy(p8str,"Data recovery     ");
	if(p8[1])
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
//1BYTE 电源掉电时数据保存时间ms
	p8str=StrCopy(p8str,"Data save = ");
	i=p8[2];
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"ms\r\n");
//2BYTE 电源掉电维持工作时间ms
	p8str=StrCopy(p8str,"Run keep  Min=1000ms  Typ=-  Max=-     Test=");
	i=p8[3]+(p8[4]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"ms     ");
	if(i<1000)
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	return p8str;
}

char *TestReport_STM32F4xx(char *p8str,u32 ADDR_BASE)//STM32F4xx测试报告
{
	u32 i;
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***STM32F4xx CLK=168MHz***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_STM32F4xx_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//ISRAM
	p8str=StrCopy(p8str,"SRAM     ");
	if(p8[0])
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
//ECC
	p8str=StrCopy(p8str,"ECC  Min=256Byte  Typ=-  Max=8192Byte     Test=256-8192Byte     ");
	if(p8[1])
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
//CRC
	p8str=StrCopy(p8str,"CRC     ");
	if(p8[2])
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
//fLSE
	p8str=StrCopy(p8str,"LSE  Min=-  Typ=32768Hz  Max=-     Test=");
	if(p8[3])
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
//fHSE
	p8str=StrCopy(p8str,"fHSE  Min=-200PPM  Typ=16000000Hz  Max=+200PPM     Test=");
	if(p8[3]!=0)
	{
		p8str=StrCopy(p8str,"Not test");
	}
	else
	{
		i=p8[4]+(p8[5]<<8)+(p8[6]<<16)+(p8[7]<<24);
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		p8str=StrCopy(p8str,"Hz");
		//if((i<15998400)||(i>16001600))//100ppm
		if((i<15996800)||(i>16003200))//200ppm
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
	}
//fLSI
	p8str=StrCopy(p8str,"fLSI  Min=17kHz  Typ=32kHz  Max=47kHz     Test=");
	i=p8[8]+(p8[9]<<8)+(p8[10]<<16)+(p8[11]<<24);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"Hz");
	if((i<17000)||(i>47000))
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
//
	
	
	return p8str;
}


char *TestReport_SDRAM(char *p8str,u32 ADDR_BASE)//SDRAM测试报告
{
	u32 i;
	u8 *p8;
	
#if IC_MT48LC4M16==1
	p8str=StrCopy(p8str,"\r\n***MT48LC4M16 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif
#if IC_MT48LC8M16==1
	p8str=StrCopy(p8str,"\r\n***MT48LC8M16 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif
#if IC_MT48LC16M16==1
	p8str=StrCopy(p8str,"\r\n***MT48LC16M16 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif
#if IC_MT48LC32M16==1
	p8str=StrCopy(p8str,"\r\n***MT48LC32M16 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif
	
#if IC_MT48LC8M8==1
	p8str=StrCopy(p8str,"\r\n***MT48LC8M8 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif
#if IC_MT48LC16M8==1
	p8str=StrCopy(p8str,"\r\n***MT48LC16M8 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif
#if IC_MT48LC32M8==1
	p8str=StrCopy(p8str,"\r\n***MT48LC32M8 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif
#if IC_MT48LC64M8==1
	p8str=StrCopy(p8str,"\r\n***MT48LC64M8 CLK=168MHz  SDCLK=CLK/2  1SDCLK=11.9ns  CAS=2(SDCLK)***\r\n");
#endif

	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_SDRAM_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//CAS
	
//加载模式寄存器到激活 (Load Mode Register to Active)
	p8str=StrCopy(p8str,"LOAD MODE REGISTER command to ACTIVE or REFRESH command(tMRD)  Min=-  Typ=2(SDCLK)  Max=-     Test=");
	i=p8[1];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(SDCLK)");
	p8str=StrCopy(p8str,"  OK\r\n");
	//退出自刷新延迟 (Exit Self-refresh delay)
	p8str=StrCopy(p8str,"Exit SELF REFRESH-to-ACTIVE command(tXSR)  Min=67ns  Typ=-  Max=-     Test=");
	i=p8[2];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(SDCLK)");
	if(i<=6)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//自刷新时间 (Self refresh time)
	p8str=StrCopy(p8str,"ACTIVE-to-PRECHARGE command(tRAS)  Min=42ns  Typ=-  Max=120000ns     Test=");
	i=p8[3];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(SDCLK)");
	if(i<=4)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//行循环延迟 (Row cycle delay)
	p8str=StrCopy(p8str,"ACTIVE-to-ACTIVE command period(tRC)  Min=60ns  Typ=-  Max=-     Test=");
	i=p8[4];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(SDCLK)");
	if(i<=6)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//恢复延迟 (Recovery delay)
	p8str=StrCopy(p8str,"WRITE recovery time(tWR)  Min=1(SDCLK)+6ns  Typ=-  Max=-     Test=");
	i=p8[5];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(SDCLK)");
	if(i<=4)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//行预充电延迟 (Row precharge delay)
	p8str=StrCopy(p8str,"PRECHARGE command period(tRP)  Min=18ns  Typ=-  Max=-     Test=");
	i=p8[6];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(SDCLK)");
	if(i<=2)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//行到列延迟 (Row to column delay)
	p8str=StrCopy(p8str,"ACTIVE-to-READ or WRITE delay(tRCD)  Min=18ns  Typ=-  Max=-     Test=");
	i=p8[7];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(SDCLK)");
	if(i<=2)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	
	return p8str;
}


#if IC_S34MLxG==1
char *TestReport_S34MLxG_NAND(char *p8str,u32 ADDR_BASE)//NAND测试报告
{
	u32 i;
	u8 *p8;

#if IC_S34ML01G2==1//0=没,1=有(128 Mbyte)
	p8str=StrCopy(p8str,"\r\n***S34ML01G1 CLK=168MHz 1CLK=5.95ns***\r\n");
#endif
#if IC_S34ML02G2==1//0=没,1=有(256 Mbyte)
	p8str=StrCopy(p8str,"\r\n***S34ML02G1 CLK=168MHz 1CLK=5.95ns***\r\n");
#endif
#if IC_S34ML04G2==1//0=没,1=有(512 Mbyte)
	p8str=StrCopy(p8str,"\r\n***S34ML04G1 CLK=168MHz 1CLK=5.95ns***\r\n");
#endif
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_NAND_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//hnand.Init.TCLRSetupTime = 0;//;CPU值(0-15);CLE 到 RE 的延迟	
	p8str=StrCopy(p8str,"CLE to RE# delay(tCLR)  Min=10ns  Typ=-  Max=-     Test=");
	i=p8[0];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=2)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//hnand.Init.TARSetupTime = 0;//;CPU值(0-15);ALE 到 RE 的延迟 
	p8str=StrCopy(p8str,"ALE to RE# delay(tAR)  Min=10ns  Typ=-  Max=-     Test=");
	i=p8[1];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=2)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.SetupTime = 8-1;//1-255;CPU值(0-255)通用存储器建立时间
	p8str=StrCopy(p8str,"CE# setup time(tCS)  Min=20ns  Typ=-  Max=-     Test=");
	i=p8[2];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=4)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.WaitSetupTime = 4-1;//2-255;CPU值(1-255)通用存储器等待时间
	p8str=StrCopy(p8str,"RE# access time(tREA)  Min=-  Typ=-  Max=20ns     Test=");
	i=p8[3];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=4)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.HoldSetupTime = 2;//1-254;CPU值(1-255)通用存储器保持时间
	p8str=StrCopy(p8str,"RE# low to output hold(tRHOH)  Min=15ns  Typ=-  Max=-     Test=");
	i=p8[4];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=3)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.HiZSetupTime = 1-1;//1-255;CPU值(0-255)通用存储器数据总线高阻态时间
/*
	p8str=StrCopy(p8str,"WE# high hold time(tWH)  Min=10ns  Typ=-  Max=-     Test=");
	i=p8[5];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<4)
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
*/
//Block Erase Time
	p8str=StrCopy(p8str,"Block Erase Time(tBERS)  Min=-  Typ=3.5ms  Max=10ms     Test=");
	i=p8[6]+(p8[7]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"ms");
	if(i<10)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//Program Time(us)
	p8str=StrCopy(p8str,"Program Time(tPROG)  Min=-  Typ=200us  Max=700us     Test=");
	i=p8[8]+(p8[9]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"us");
	if(i<700)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//Page Read Time(us)
	p8str=StrCopy(p8str,"Random access  Min=-  Typ=-  Max=25us     Test=");
	i=p8[10]+(p8[11]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"us");
	if(i<25)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//错误块数
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_NAND_ERROR_BLOCKS;
	i=p8[0]+(p8[1]<<8);
#if IC_S34ML01G2==1//0=没,1=有(128 Mbyte)
	p8str=StrCopy(p8str,"Number of error blocks  Min=0  Typ=-  Max=20     Test=");
	if(i==0xffff)
	{
		p8str=StrCopy(p8str," Not test\r\n");
	}
	else
	{
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		if(i<=20)
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
	}
#endif
#if IC_S34ML02G2==1//0=没,1=有(256 Mbyte)
	p8str=StrCopy(p8str,"Number of error blocks  Min=0  Typ=-  Max=40     Test=");
	if(i==0xffff)
	{
		p8str=StrCopy(p8str," Not test\r\n");
	}
	else
	{
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		if(i<=40)
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
	}
#endif
#if IC_S34ML04G2==1//0=没,1=有(512 Mbyte)
	p8str=StrCopy(p8str,"Number of error blocks  Min=0  Typ=-  Max=80     Test=");
	if(i==0xffff)
	{
		p8str=StrCopy(p8str," Not test\r\n");
	}
	else
	{
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		if(i<=80)
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
	}
#endif
	return p8str;
}
#endif

#if IC_MT29FxG==1
char *TestReport_MT29FxG_NAND(char *p8str,u32 ADDR_BASE)//NAND测试报告
{
	u32 i;
	u8 *p8;

#if IC_MT29F1G==1//0=没,1=有(128 Mbyte)
	p8str=StrCopy(p8str,"\r\n***MT29F1G CLK=168MHz 1CLK=5.95ns***\r\n");
#endif
#if IC_MT29F2G==1//0=没,1=有(256 Mbyte)
	p8str=StrCopy(p8str,"\r\n***MT29F2G CLK=168MHz 1CLK=5.95ns***\r\n");
#endif
#if IC_MT29F4G==1//0=没,1=有(512 Mbyte)
	p8str=StrCopy(p8str,"\r\n***MT29F4G CLK=168MHz 1CLK=5.95ns***\r\n");
#endif
#if IC_MT29F8G==1//0=没,1=有(512 Mbyte)
	p8str=StrCopy(p8str,"\r\n***MT29F8G CLK=168MHz 1CLK=5.95ns***\r\n");
#endif
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_NAND_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//hnand.Init.TCLRSetupTime = 0;//;CPU值(0-15);CLE 到 RE 的延迟	
	p8str=StrCopy(p8str,"CLE to RE# delay(tCLR)  Min=10ns  Typ=-  Max=-     Test=");
	i=p8[0];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=2)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//hnand.Init.TARSetupTime = 0;//;CPU值(0-15);ALE 到 RE 的延迟 
	p8str=StrCopy(p8str,"ALE to RE# delay(tAR)  Min=10ns  Typ=-  Max=-     Test=");
	i=p8[1];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=2)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.SetupTime = 8-1;//1-255;CPU值(0-255)通用存储器建立时间
	p8str=StrCopy(p8str,"CE# setup time(tCS)  Min=20ns  Typ=-  Max=-     Test=");
	i=p8[2];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=4)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.WaitSetupTime = 4-1;//2-255;CPU值(1-255)通用存储器等待时间
	p8str=StrCopy(p8str,"RE# access time(tREA)  Min=-  Typ=-  Max=20ns     Test=");
	i=p8[3];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=2)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.HoldSetupTime = 6-1;//1-254;CPU值(1-255)通用存储器保持时间
	p8str=StrCopy(p8str,"RE# low to output hold(tRHOH)  Min=15ns  Typ=-  Max=-     Test=");
	i=p8[4];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<=4)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ComSpaceTiming.HiZSetupTime = 1-1;//1-255;CPU值(0-255)通用存储器数据总线高阻态时间
/*
	p8str=StrCopy(p8str,"WE# high hold time(tWH)  Min=10ns  Typ=-  Max=-     Test=");
	i=p8[5];
	i++;
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"(CLK)");
	if(i<4)
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
*/
//Block Erase Time
	p8str=StrCopy(p8str,"Block Erase Time(tBERS)  Min=-  Typ=700us  Max=3000us     Test=");
	i=p8[6]+(p8[7]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"us");
	if(i<3000)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//Program Time(us)
	p8str=StrCopy(p8str,"Program Time(tPROG)  Min=-  Typ=220us  Max=600us     Test=");
	i=p8[8]+(p8[9]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"us");
	if(i<600)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//Page Read Time(us)
	p8str=StrCopy(p8str,"Random access  Min=-  Typ=45us  Max=70us     Test=");
	i=p8[10]+(p8[11]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"us");
	if(i<70)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//错误块数
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_NAND_ERROR_BLOCKS;
	i=p8[0]+(p8[1]<<8);
#if IC_MT29F1G==1//0=没,1=有(128 Mbyte)
	p8str=StrCopy(p8str,"Number of error blocks  Min=0  Typ=-  Max=20     Test=");
	if(i==0xffff)
	{
		p8str=StrCopy(p8str," Not test\r\n");
	}
	else
	{
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		if(i<=20)
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
	}
#endif
#if IC_MT29F2G==1//0=没,1=有(256 Mbyte)
	p8str=StrCopy(p8str,"Number of error blocks  Min=0  Typ=-  Max=40     Test=");
	if(i==0xffff)
	{
		p8str=StrCopy(p8str," Not test\r\n");
	}
	else
	{
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		if(i<=40)
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
	}
#endif
#if IC_MT29F4G==1//0=没,1=有(512 Mbyte)
	p8str=StrCopy(p8str,"Number of error blocks  Min=0  Typ=-  Max=80     Test=");
	if(i==0xffff)
	{
		p8str=StrCopy(p8str," Not test\r\n");
	}
	else
	{
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		if(i<=80)
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
	}
#endif
#if IC_MT29F8G==1//0=没,1=有(1024 Mbyte)
	p8str=StrCopy(p8str,"Number of error blocks  Min=0  Typ=-  Max=160     Test=");
	if(i==0xffff)
	{
		p8str=StrCopy(p8str," Not test\r\n");
	}
	else
	{
		p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		if(i<=160)
		{
			p8str=StrCopy(p8str,"  OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"  *FAIL*\r\n");
		}
	}
#endif
	return p8str;
}
#endif

char *TestReport_90E36(char *p8str,u32 ADDR_BASE)//90E36测试报告
{
	u32 i;
	u32 x;
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***M90E36 CLK=168MHz***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_90E36_Characteristics;
	i=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);
	if(i==0xffffffff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//通信速率
	p8str=StrCopy(p8str,"SCLK  Min=100Hz  Typ=-  Max=1200kHz     Test=");
	x=i;
	p8str+=HEX_ASCII(x/1000,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	x%=1000;
	if(x!=0)
	{
		p8str=StrCopy(p8str,".");
		p8str+=HEX_ASCII(x,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	}
	p8str=StrCopy(p8str,"kHz");
	if(i>=1200000)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//复位
	p8str=StrCopy(p8str,"RESET  Min=-  Typ=2us  Max=-     Test=");
	i=p8[1];
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"us");
	if(i<100)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	return p8str;
}

char *TestReport_RTC(char *p8str,u32 ADDR_BASE)//RTC测试报告
{
	u32 i;
	u8 *p8;
	
#if IC_DS3231==1//0=没,1=有
	p8str=StrCopy(p8str,"\r\n***DS3231 SCK=333kHz***\r\n");
#endif
#if IC_RX8025T==1//0=没,1=有
	p8str=StrCopy(p8str,"\r\n***RX8025T SCK=333kHz***\r\n");
#endif
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_RTC_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//设置时钟99-12-31 6 23:59:59读出比较:0=正确,1=错误
	p8str=StrCopy(p8str,"Set RTC   99-12-31 6 23:59:59");
	if(p8[0]==0)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//1秒后读时钟值检查进位00-01-01 0 00:00:00 ;0=正确,1=错误
	p8str=StrCopy(p8str,"After 1 second read RTC  00-01-01 0 00:00:00");
	if(p8[1]==0)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//I2C总线复位间隔时间
	p8str=StrCopy(p8str,"tRESET  Min=900ms  Typ=950ms  Max=1000ms     Test=");
	i=p8[2]+(p8[3]<<8);
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"ms");
	if((i<900)||(i>1000))
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
//1Hz输出(0=OK,1=ERROR)
	
	
//32kHz输出(0=OK,1=ERROR)
	
	return p8str;
}

char *TestReport_LAN8720A(char *p8str,u32 ADDR_BASE)//LAN8720A测试报告
{
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***LAN8720Ai CLK=168MHz MDC=CLK/102=1.647MHz(Max=2.5MHz)***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_LAN8720A_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//SMI
	p8str=StrCopy(p8str,"SMI  ");
	if(p8[0]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
//Auto-Negotiation
	p8str=StrCopy(p8str,"Auto-Negotiation  ");
	if(p8[1]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
//REF_CLK
	p8str=StrCopy(p8str,"REF_CLK  ");
	if(p8[2]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
	}
	else
	{
		if(p8[2]==0)
		{
			p8str=StrCopy(p8str,"OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	}
//tRESET
	p8str=StrCopy(p8str,"tRESET  Min=100us  Typ=-  Max=-     Test=");
	p8str+=HEX_ASCII(p8[3],(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"us");
	if(p8[3]>=110)
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	
	return p8str;
}


char *TestReport_UART_BPS(char *p8str,u32 bps)//UART测试报告_波特率
{
	switch(bps)//波特率,O～11依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
	{
		case 0:
			p8str=StrCopy(p8str,"300");
			break;
		case 1:
			p8str=StrCopy(p8str,"600");
			break;
		case 2:
			p8str=StrCopy(p8str,"1200");
			break;
		case 3:
			p8str=StrCopy(p8str,"2400");
			break;
		case 4:
			p8str=StrCopy(p8str,"4800");
			break;
		case 5:
			p8str=StrCopy(p8str,"7200");
			break;
		case 6:
			p8str=StrCopy(p8str,"9600");
			break;
		case 7:
			p8str=StrCopy(p8str,"19200");
			break;
		case 8:
			p8str=StrCopy(p8str,"38400");
			break;
		case 9:
			p8str=StrCopy(p8str,"57600");
			break;
		case 10:
			p8str=StrCopy(p8str,"115200");
			break;
		case 11:
			p8str=StrCopy(p8str,"230400");
			break;
	}
	return p8str;
}
char *TestReport_UART(char *p8str,u32 ADDR_BASE)//UART测试报告
{
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***UART CLK=168MHz***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_UART_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
//RS485
#if MainProtocol!=CSG
	p8str=StrCopy(p8str,"RS485-1 RS485-2  Min=300  Typ=2400  Max=9600     Test=300-");
#else
	p8str=StrCopy(p8str,"RS485-1 RS485-2  RS485-3  Min=300  Typ=2400  Max=9600     Test=300-");
#endif
	p8str=TestReport_UART_BPS(p8str,p8[0]);//UART测试报告_波特率
	if(p8[0]>=6)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//IR
	p8str=StrCopy(p8str,"IR  Min=300  Typ=1200  Max=2400     Test=300-");
	p8str=TestReport_UART_BPS(p8str,p8[1]);//UART测试报告_波特率
	if(p8[1]>=3)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//RS232
	p8str=StrCopy(p8str,"RS232  Min=1200  Typ=9600  Max=115200     Test=1200-");
	p8str=TestReport_UART_BPS(p8str,p8[2]);//UART测试报告_波特率
	if(p8[2]>=10)
	{
		p8str=StrCopy(p8str,"  OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//ETH
	p8str=StrCopy(p8str,"Ethernet  Min=10Mbps  Typ=-  Max=100Mbps     Test=");
	if(p8[3]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
//GPRS
	p8str=StrCopy(p8str,"GPRS Module     Test=");
	if(p8[4]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
//本地载波模块A相
	p8str=StrCopy(p8str,"Local Moduole A Phase       Test=");
	if(p8[5]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
//本地载波模块B相
	p8str=StrCopy(p8str,"Local Moduole B Phase       Test=");
	if(p8[6]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
//本地载波模块C相
	p8str=StrCopy(p8str,"Local Moduole C Phase       Test=");
	if(p8[7]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
//控制模块输出1
	p8str=StrCopy(p8str,"Contrl Module OUT1       Test=");
	if(p8[5]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
//控制模块输出2
	p8str=StrCopy(p8str,"Contrl Module OUT2       Test=");
	if(p8[6]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
//控制模块报警
	p8str=StrCopy(p8str,"Contrl Module Alarm       Test=");
	if(p8[7]==0)
	{
		p8str=StrCopy(p8str,"OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"*FAIL*\r\n");
	}
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=三合一终端,500=集中器Ⅱ型,600=通信模块,700=
	if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
	{
	//本地载波模块A相
		p8str=StrCopy(p8str,"Local Moduole A Phase       Test=");
		if(p8[5]==0)
		{
			p8str=StrCopy(p8str,"OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	//本地载波模块B相
		p8str=StrCopy(p8str,"Local Moduole B Phase       Test=");
		if(p8[6]==0)
		{
			p8str=StrCopy(p8str,"OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	//本地载波模块C相
		p8str=StrCopy(p8str,"Local Moduole C Phase       Test=");
		if(p8[7]==0)
		{
			p8str=StrCopy(p8str,"OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	}
	else
	{
	//控制模块输出1
		p8str=StrCopy(p8str,"Contrl Module OUT1       Test=");
		if(p8[5]==0)
		{
			p8str=StrCopy(p8str,"OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	//控制模块输出2
		p8str=StrCopy(p8str,"Contrl Module OUT2       Test=");
		if(p8[6]==0)
		{
			p8str=StrCopy(p8str,"OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	//控制模块报警
		p8str=StrCopy(p8str,"Contrl Module Alarm       Test=");
		if(p8[7]==0)
		{
			p8str=StrCopy(p8str,"OK\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	}
#endif
	return p8str;
}


char *Data_ASCII(u8 *p8,char *p8str,u32 Byte)//先填写低字节,字节间空格
{
	u32 i;
	
	while(Byte--)
	{
		i=p8[0];
		i>>=4;
		if(i<10)
		{
			i+=0x30;
		}
		else
		{
			i+=55;
		}
		p8str[0]=i;
		i=p8[0];
		i&=0xf;
		if(i<10)
		{
			i+=0x30;
		}
		else
		{
			i+=55;
		}
		p8str[1]=i;
		p8str[2]=' ';
		p8str+=3;
		p8++;
	}
	return p8str;
}
char *TestReport_ESAM(char *p8str,u32 ADDR_BASE)//ESAM测试报告
{
	u32 i;
	u8 *p8;

//TESAM
	p8str=StrCopy(p8str,"\r\n***ESAM CLK=168MHz***\r\n");
	p8str=StrCopy(p8str,"T-ESAM SCK Min=1MHz  Typ=-  Max=2MHz     Test=");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_ESAM_Characteristics;
	i=p8[0]+(p8[1]<<8)+(p8[2]<<16)+(p8[3]<<24);
	if(i==0xffffffff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
	p8str=StrCopy(p8str,"500000-");
	p8str+=HEX_ASCII(i,(u32)p8str);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
	p8str=StrCopy(p8str,"Hz");
	if(i>2000000)
	{
		p8str=StrCopy(p8str," OK\r\n");
		
		p8str=StrCopy(p8str,"T-ESAM serial number:");
		p8+=4;
		p8str=Data_ASCII(p8,p8str,8);//先填写低字节,字节间空格
		p8str=StrCopy(p8str,"\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
//MESAM
	
	return p8str;
}

char *TestReport_24AA02E48(char *p8str,u32 ADDR_BASE)//24AA02E48测试报告
{
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***24AA02E48 SCK=333kHz***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_24AA02E48_Characteristics;
	if(p8[0]==0xff)
	{
		p8str=StrCopy(p8str,"Not test\r\n");
		return p8str;
	}
	if(p8[0]==0)
	{
		p8str=StrCopy(p8str,"MAC:");
		p8+=1;
		p8str=Data_ASCII(p8,p8str,6);//先填写低字节,字节间空格
		p8str=StrCopy(p8str," OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	
	return p8str;
}

char *TestReport_KEY(char *p8str,u32 ADDR_BASE)//KEY测试报告
{
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***KEY CLK=168MHz***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_KEY_Characteristics;
	if(p8[0]==0)
	{
		p8str=StrCopy(p8str," OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	return p8str;
}

char *TestReport_IN(char *p8str,u32 ADDR_BASE)//开关量输入测试报告
{
	u8 *p8;
	
	p8str=StrCopy(p8str,"\r\n***Switch input CLK=168MHz***\r\n");
	p8=(u8*)ADDR_BASE;
	p8+=OFFSET_SWIN_Characteristics;
	p8str=StrCopy(p8str,"IN1     Test=");
	if(p8[0]==0)
	{
		p8str=StrCopy(p8str," OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	p8str=StrCopy(p8str,"IN2     Test=");
	if(p8[1]==0)
	{
		p8str=StrCopy(p8str," OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	p8str=StrCopy(p8str,"IN3     Test=");
	if(p8[2]==0)
	{
		p8str=StrCopy(p8str," OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	p8str=StrCopy(p8str,"IN4     Test=");
	if(p8[3]==0)
	{
		p8str=StrCopy(p8str," OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	p8str=StrCopy(p8str,"DOOR     Test=");
	if(p8[4]==0)
	{
		p8str=StrCopy(p8str," OK\r\n");
	}
	else
	{
		p8str=StrCopy(p8str,"  *FAIL*\r\n");
	}
	return p8str;
}

char *Report_txt(char *p8str,u32 ADDR_BASE)//生成测试报告文件
{
	u32 i;
	u8 *p8;

//测试总结论
	p8str=StrCopy(p8str,"\r\n\r\n");
	p8=(u8*)ADDR_BASE+(OFFSET_TestResult_Characteristics);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
	if(p8[0]!=0xff)
	{//已测试
		//测试时温度
		p8=(u8*)ADDR_BASE+(OFFSET_Test_Temperature);
		i=p8[0]+(p8[1]<<8);
		if(i&0x8000)
		{//负
			p8str=StrCopy(p8str,"-");
		}
		i&=0x7fff;
		i>>=4;
		p8str+=BCD_ASCII(i,(u32)p8str);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		p8str=StrCopy(p8str,"℃");
		
		p8=(u8*)ADDR_BASE+(OFFSET_TestResult_Characteristics);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
		p8str=StrCopy(p8str,"  TEST RESULT: ");
		if(p8[0]==0)
		{
			p8str=StrCopy(p8str,"PASS\r\n");
		}
		else
		{
			
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
	//POWER测试报告
		p8str=TestReport_POWER(p8str,ADDR_BASE);//POWER测试报告
	//POWER_DOWN测试报告
		p8str=TestReport_POWER_DOWN(p8str,ADDR_BASE);//POWER_DOWN测试报告
	//STM32F4xx测试报告
		p8str=TestReport_STM32F4xx(p8str,ADDR_BASE);//STM32F4xx测试报告
	//SDRAM测试报告
		p8str=TestReport_SDRAM(p8str,ADDR_BASE);//SDRAM测试报告
	//NAND测试报告
#if IC_S34MLxG==1
		p8str=TestReport_S34MLxG_NAND(p8str,ADDR_BASE);//NAND测试报告
#endif
#if IC_MT29FxG==1
		p8str=TestReport_MT29FxG_NAND(p8str,ADDR_BASE);//NAND测试报告
#endif
	//90E36测试报告
		p8str=TestReport_90E36(p8str,ADDR_BASE);//90E36测试报告
	//RTC测试报告
		p8str=TestReport_RTC(p8str,ADDR_BASE);//RTC测试报告
	//LAN8720A
		p8str=TestReport_LAN8720A(p8str,ADDR_BASE);//LAN8720A测试报告
	//UART测试报告
		p8str=TestReport_UART(p8str,ADDR_BASE);//UART测试报告
	//ESAM测试报告
		p8str=TestReport_ESAM(p8str,ADDR_BASE);//ESAM测试报告
	//24AA02E48
		p8str=TestReport_24AA02E48(p8str,ADDR_BASE);//24AA02E48测试报告
	//KEY
		p8str=TestReport_KEY(p8str,ADDR_BASE);//KEY测试报告
	//SWIN
		p8str=TestReport_IN(p8str,ADDR_BASE);//开关量输入测试报告
	//
	
	}
	return p8str;
}




u32 TestReport(void)//生成测试报告文件;返回:文件长度(生成测试报告文件在ADDR_128KWRITE_BUFF，并写入os文件系统)
{
	char *p8str;
	FILE *fsfile;
	os_Type *os;
	
	p8str=(char *)ADDR_128KWRITE_BUFF;
//常温测试
	p8str=Report_txt(p8str,ADDR_Characteristics_BASE1);//生成测试报告文件
//低温测试
	p8str=Report_txt(p8str,ADDR_Characteristics_BASE2);//生成测试报告文件
//高温测试
	p8str=Report_txt(p8str,ADDR_Characteristics_BASE3);//生成测试报告文件

	os=Get_os();
	fsfile=os->fopen("N0:/TEST/TEST.txt","w");
	if(fsfile!=NULL)
	{
		os->fwrite((void *)(ADDR_128KWRITE_BUFF),1,((u32)p8str)-(ADDR_128KWRITE_BUFF),fsfile);
		os->fclose(fsfile);
	}
	return ((u32)p8str)-(ADDR_128KWRITE_BUFF);
}




