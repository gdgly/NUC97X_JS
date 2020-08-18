
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



char *Report_txt(char *p8str,u32 ADDR_BASE)//���ɲ��Ա����ļ�
{
	u32 i;
	u8 *p8;

//�����ܽ���
	p8str=StrCopy(p8str,"\r\n\r\n");
	p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_TestResult_Characteristics);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	if(p8[0]!=0xff)
	{//�Ѳ���
		//����ʱ�¶�
		p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_Test_Temperature);
		i=p8[0]+(p8[1]<<8);
		if(i&0x8000)
		{//��
			p8str=StrCopy(p8str,"-");
		}
		i&=0x7fff;
		i>>=4;
		p8str+=BCD_ASCII(i,(u32)p8str);//BCDתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
		p8str=StrCopy(p8str,"��");
		
		p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_TestResult_Characteristics);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
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
		{//ֻ�г�ûͨ������
	//POWER���Ա���
			
	//POWER_DOWN���Ա���
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_POWER_DOWN_Characteristics);
			if(p8[1]==1)
			{
				p8str=StrCopy(p8str,"POWER_DOWN  *FAIL*\r\n");
			}
	//STM32F4xx���Ա���

	//SDRAM���Ա���
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_SDRAM_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"DDR2  *FAIL*\r\n");
			}
	//NAND���Ա���
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_NAND_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"NAND_FLASH  *FAIL*\r\n");
			}
	//90E36���Ա���
		#if IC_90E36==1
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_90E36_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"ATM90E36  *FAIL*\r\n");
			}
		#endif
	//ATT7022���Ա���
		#if IC_ATT7022==1
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_90E36_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"ATT7022  *FAIL*\r\n");
			}
		#endif
	//RTC���Ա���
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
	//UART���Ա���
			//RS485-1,RS485-2
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"RS485-1,RS485-2  *FAIL*\r\n");
			}
			//����
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
			//GPRS���Ա���
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+4);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"GPRS  *FAIL*\r\n");
			}
			//����ģ��
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+5);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"Local Moduole  *FAIL*\r\n");
			}
			//����ģ��
			p8=(u8*)ADDR_BASE-ADDR_IEEPROM_START+ADDR_SPI_MAP+(OFFSET_UART_Characteristics+6);
			if(p8[0]==1)
			{
				p8str=StrCopy(p8str,"Contrl Module  *FAIL*\r\n");
			}
	//ESAM���Ա���
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


u32 TestReport(void)//���ɲ��Ա����ļ�;����:�ļ�����(���ɲ��Ա����ļ���ADDR_128KWRITE_BUFF)
{
	char *p8str;
//	os_Type *os;
	
	p8str=(char *)ADDR_128KWRITE_BUFF;
//���²���
	p8str=Report_txt(p8str,ADDR_Characteristics_BASE1);//���ɲ��Ա����ļ�
//���²���
	//p8str=Report_txt(p8str,ADDR_Characteristics_BASE2);//���ɲ��Ա����ļ�
//���²���
	//p8str=Report_txt(p8str,ADDR_Characteristics_BASE3);//���ɲ��Ա����ļ�

	
//	os=Get_os();
//	fsfile=os->fopen("N0:/TEST/TEST.txt","w");
//	if(fsfile!=NULL)
//	{
//		os->fwrite((void *)(ADDR_128KWRITE_BUFF),1,((u32)p8str)-(ADDR_128KWRITE_BUFF),fsfile);
//		os->fclose(fsfile);
//	}
	return ((u32)p8str)-(ADDR_128KWRITE_BUFF);
}












