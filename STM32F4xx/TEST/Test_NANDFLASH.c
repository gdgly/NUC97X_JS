
#include "Project.h"
#include "../Display/Display.h"
#include "../Periph/S34MLxxG2.h"
#include "../LPC18xx43xx/LPC18xx43xx_EEPROM.h"
#include "../Calculate/Calculate.h"
#include "../LPC18xx43xx/LPC18xx43xx_IO.h"
#include "../LPC18xx43xx/Dealy.h"
#include "../OS/My_Heap.h"
#include "../Periph/MEMRW.h"



void Test_NANDFLASH(void)
{
	u32 i;
	u32 x;
	u32 y;
	u32 BLOCK_COUNT;
	u32 ERROR_COUNT;
	u32 *p32;
	u16 *pStrBuff;
	u8 *p8EEPROM;
	
	
	DisplayStringAutoUp(0,9,(u8*)"\x0""NAND FLASH TEST");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	i=ID_S34MLxxG2();//������ID(1Gb*8=0x01F1801D,1Gb*16=0x01C1805D)
	if(i!=0x01F1801D)
	{
		DisplayStringAutoUp(0,9,(u8*)"\x0""Error!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
		return;
	}

	DisplayStringAutoUp(0,9,(u8*)"\x0""Total Blocks:1024");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0""Error Blocks:0");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	DisplayStringAutoUp(0,9,(u8*)"\x0"" ");//����������ʾ��
	
	Clr_LPC18xx43xx_EEPROM(0xFD,ADDR_NANDFLASH_BLOCK_REPLACE,NAND_BLOCK_COUNT);//EEPROM���
	p32=(u32*)(ADDR_128KWRITE_BUFF);
	p8EEPROM=(u8*)ADDR_NANDFLASH_BLOCK_REPLACE;
	
	y=(u32)Myalloc(128*1024);//��̬�����ҵ��ڴ��
	for(BLOCK_COUNT=0;BLOCK_COUNT<NAND_BLOCK_COUNT;BLOCK_COUNT++)
	{
		//����ԭ����
		READ_S34MLxxG2(y,BLOCK_COUNT*128*1024,128*1024);//NAND_FLASH S34MLxxG2û�滻��
		//д0xff
		for(i=0;i<((128*1024)/4);i++)
		{
			p32[i]=0xffffffff;
		}
		i=BLOCKWRITE_S34MLxxG2(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			Clr_LPC18xx43xx_EEPROM(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
		//��д˳����
		for(i=0;i<((128*1024)/4);i++)
		{
			p32[i]=i;
		}
		i=BLOCKWRITE_S34MLxxG2(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			Clr_LPC18xx43xx_EEPROM(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
		//д0
		for(i=0;i<((128*1024)/4);i++)
		{
			p32[i]=0;
		}
		i=BLOCKWRITE_S34MLxxG2(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		if(i!=0)
		{
			Clr_LPC18xx43xx_EEPROM(0xFE,ADDR_NANDFLASH_BLOCK_REPLACE+BLOCK_COUNT,1);//EEPROM���
		}
		//�ָ�ԭ����
		MW(y,ADDR_128KWRITE_BUFF,128*1024);
		BLOCKWRITE_S34MLxxG2(BLOCK_COUNT*128*1024);//��дADDR_128KWRITE_BUFF�����ݵ�FLASH��,ͬʱ�����Ƚ�;����;0=��ȷ,1=����
		
		//��ʾ�������
		ERROR_COUNT=0;
		for(i=0;i<NAND_BLOCK_COUNT;i++)
		{
			x=p8EEPROM[i];
			if(x==0xFE)
			{
				ERROR_COUNT++;
				continue;
			}
			if((x>=1)&&(x<=NAND_REPLACE_BLOCK_COUNT))
			{
				ERROR_COUNT++;
			}
		}
		pStrBuff=(u16 *)(ADDR_STRINGBUFF+((AutoDisplayRowCol->Row-2)*84)+4+(13*2));
		for(i=0;i<4;i++)
		{
			pStrBuff[i]=0;
		}
		x=hex_bcd(ERROR_COUNT);
		DisplayData_OffZero(x,4,0,pStrBuff);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
		//��ʾ��ʱ(S)
		
		//��ʾ����
		i=(BLOCK_COUNT*100)/NAND_BLOCK_COUNT;
		DisplayRATE(AutoDisplayRowCol->Row-1,i);//��ʾ����;���:Ln=��ʾ��,Rate=����(hex,0-100%)
	}
	Myfree((void *)y);//�ͷ��ҵ��ڴ�ѷ���
	
	//���
	AutoDisplayRowCol->Row--;
	DisplayClrRow(AutoDisplayRowCol->Row,AutoDisplayRowCol->Row);//�������ʾ��
	if(ERROR_COUNT<=20)
	{
		i=MRR(ADDR_ICERROR_IEEPROM,4);
		i&=(~0x08);
		MWR(i,ADDR_ICERROR_IEEPROM,4);
		DisplayStringAutoUp(0,9,(u8*)"\x0""OK!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	}
	else
	{
		i=MRR(ADDR_ICERROR_IEEPROM,4);
		i|=0x08;
		MWR(i,ADDR_ICERROR_IEEPROM,4);
		DisplayStringAutoUp(0,9,(u8*)"\x0""ERROR!");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	}
	
}









