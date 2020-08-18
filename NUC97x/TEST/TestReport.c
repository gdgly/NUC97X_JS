
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../STM32F4xx/TEST/TestReport.h"
#include "../../STM32F4xx/Calculate/Calculate.h"
#include "../../STM32F4xx/Calculate/ascii.h"
#include "../../STM32F4xx/OS/OS.h"



char *Report_txt(char *p8str,u32 ADDR_BASE)//生成测试报告文件
{
	u32 i;
	u8 *p8;

//测试总结论
	p8str=StrCopy(p8str,"\r\n\r\n");
	p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_TestResult_Characteristics);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
	if(p8[0]!=0xff)
	{//已测试
		//测试时温度
		p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_Test_Temperature);
		i=p8[0]+(p8[1]<<8);
		if(i&0x8000)
		{//负
			p8str=StrCopy(p8str,"-");
		}
		i&=0x7fff;
		i>>=4;
		p8str+=BCD_ASCII(i,(u32)p8str);//BCD转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
		p8str=StrCopy(p8str,"℃");
		
		p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_TestResult_Characteristics);//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
		p8str=StrCopy(p8str,"  TEST RESULT: ");
		if(p8[0]==0)
		{
			p8str=StrCopy(p8str,"PASS\r\n");
		}
		else
		{
			p8str=StrCopy(p8str,"*FAIL*\r\n");
		}
		if(p8[0])
		{//只列出没通过的项
	//POWER测试报告
			
	//POWER_DOWN测试报告
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_POWER_DOWN_Characteristics);
			if(p8[1]==1)
			{
				p8str=StrCopy(p8str,"POWER_DOWN  *FAIL*\r\n");
			}
	//STM32F4xx测试报告

	//SDRAM测试报告
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_SDRAM_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"DDR2  *FAIL*\r\n");
			}
	//NAND测试报告
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_NAND_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"NAND_FLASH  *FAIL*\r\n");
			}
	//90E36测试报告
		#if IC_90E36==1
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_90E36_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"ATM90E36  *FAIL*\r\n");
			}
		#endif
	//ATT7022测试报告
		#if IC_ATT7022==1
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_90E36_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"ATT7022  *FAIL*\r\n");
			}
		#endif
	//RTC测试报告
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_RTC_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"RX8025  *FAIL*\r\n");
			}
	//IP101GRI
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_LAN8720A_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"IP101GRI  *FAIL*\r\n");
			}
	//UART测试报告
			//RS485-1,RS485-2
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"RS485-1,RS485-2  *FAIL*\r\n");
			}
			//红外
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+1);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"IR  *FAIL*\r\n");
			}
			//RS232(RS485-1,RS485-3)
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+2);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"RS485-1,RS485-3  *FAIL*\r\n");
			}
			//Ethernet
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+3);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"Ethernet  *FAIL*\r\n");
			}
			//GPRS测试报告
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+4);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"GPRS  *FAIL*\r\n");
			}
			//本地模块
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+5);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"Local Moduole  *FAIL*\r\n");
			}
			//控制模块
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+6);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"Contrl Module  *FAIL*\r\n");
			}
	//ESAM测试报告
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_ESAM_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"TESAM  *FAIL*\r\n");
			}
	//24AA02E48
			
	//SWIN
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_SWIN_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"SWIN1  *FAIL*\r\n");
			}
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_SWIN_Characteristics+1);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"SWIN2  *FAIL*\r\n");
			}
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_SWIN_Characteristics+2);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"SWIN3  *FAIL*\r\n");
			}
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_SWIN_Characteristics+3);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"SWIN4  *FAIL*\r\n");
			}
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_SWIN_Characteristics+4);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"SWIN5  *FAIL*\r\n");
			}
	//OUT
			
	//KEY
			
		}
	}
	return p8str;
}


u32 TestReport(void)//生成测试报告文件;返回:文件长度(生成测试报告文件在ADDR_128KWRITE_BUFF)
{
	char *p8str;
//	os_Type *os;
	
	p8str=(char *)ADDR_128KWRITE_BUFF;
//常温测试
	p8str=Report_txt(p8str,ADDR_Characteristics_BASE1);//生成测试报告文件
//低温测试
	//p8str=Report_txt(p8str,ADDR_Characteristics_BASE2);//生成测试报告文件
//高温测试
	//p8str=Report_txt(p8str,ADDR_Characteristics_BASE3);//生成测试报告文件

	
//	os=Get_os();
//	fsfile=os->fopen("N0:/TEST/TEST.txt","w");
//	if(fsfile!=NULL)
//	{
//		os->fwrite((void *)(ADDR_128KWRITE_BUFF),1,((u32)p8str)-(ADDR_128KWRITE_BUFF),fsfile);
//		os->fclose(fsfile);
//	}
	return ((u32)p8str)-(ADDR_128KWRITE_BUFF);
}












