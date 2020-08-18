

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698TEST.h"
#include "../Device/MEMRW.h"
#include "../Device/RTC.h"
#include "../Calculate/Calculate.h"
#include "../Terminal698/Terminal_Init.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../MS/MS.h"
#include "../Display/Display.h"
#include "../Display/Warning.h"







#if RUN_DL698TEST_1==1//0��Ч,1=����;ȫ��������Ϊ�ڲ����ɲ������ݴ洢������

const u8 InitData_6000_2_TEST[]=//�ɼ���������
{
//����2��Acquisition document definition����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
	DataType_structure,
	4,
//{
//�������  long-unsigned��
	DataType_long_unsigned,
	0,1,
//������Ϣ  Basic_object��
	
//��չ��Ϣ  Extended_object��
	
//������Ϣ  Annex_object
//}

//Basic_object��=structure
	DataType_structure,
	10,
//{
//ͨ�ŵ�ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
	DataType_enum,
	255,
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//}��
	DataType_enum,
	3,
//�˿�      OAD��
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //��������ͨ�ſں�(����)
//ͨ������  octet-string��
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//���ʸ���  unsigned��
	DataType_unsigned,
	4,
//�û�����  unsigned��
	DataType_unsigned,
	101,
//���߷�ʽ  enum
//{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//}
	DataType_enum,
	3,
//���ѹ    long-unsigned(����-1����λV),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//�����    long-unsigned(����-1����λA)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object��=structure
	DataType_structure,
	4,
//{
//�ɼ�����ַ  TSA��
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//�ʲ���      octet-string��
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned��
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object��=array structure
	DataType_array,
	0,
//{
//������������  OAD��
//����ֵ        Data
//}
};


void DL698TEST_1(void)//ȫ��������Ϊ�ڲ����ɲ������ݴ洢������
{
	u32 i;
	u32 x;
	u64 d64;
	u8* p8;
	u32 filename;
	msFILE_TypeDef *file;
//	ms_Type *ms;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
	p8=(u8*)ADDR_TEST;
	i=p8[0];
	switch(i)
	{
		case 0:
			d64=d64;
/*
			//��ʼ������
			DL698_INIT(3);//��ʼ��,���Command:0=��,1=Ӳ����ʼ��,2=��������ʼ��,3=������ȫ����������ʼ��(���ָ�����������),4/5=����(����ϵͳ��վͨ���йص�)��ȫ����������ʼ��,6=�����ݲ�����ʼ��
			//�嵵��
			MC(0,ADDR_6000,LENper_6000*NUMmax_6000);
			//��ʼ������
			MR(ADDR_DATABUFF,(u32)InitData_6000_2_TEST,sizeof(InitData_6000_2_TEST));
			d64=0;//ͨ�ŵ�ַ
			//for(i=0;i<NUMmax_6000;i++)
			for(i=0;i<10;i++)
			{
				d64=hex_bcd16(d64);//HEX��תΪ16λѹ��BCD��			
				d64=R_Inverse(d64,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
				MWR(d64,ADDR_DATABUFF+10,6);
				MW(ADDR_DATABUFF,ADDR_6000+(i*LENper_6000),LENper_6000);
				d64=R_Inverse(d64,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
				d64=bcd16_hex(d64);//16λѹ��BCD��תΪHEX
				d64++;
			}
			FLASHtoSDRAM();//����FLASHtoSDRAM
*/
			p8[0]++;
			Comm_Ram->DisplayAllErrorTimer=0;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��

			break;
		case 1:
			//NAND���ļ������͵�ǰ��
			p8=(u8*)ADDR_DATABUFF;
			CopyString((u8 *)"NAND               ",p8);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			p8+=6;
			i=FILENAMEmax_FLASH;
			p8+=HEX_ASCII(i,(u32)p8);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
			p8++;
			i=0;
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					i++;
				}
				file++;
			}
			p8+=HEX_ASCII(i,(u32)p8);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
			p8[0]=0;
			WarningOccur(RS485_1PORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			
			//RAM���ļ������͵�ǰ��
			p8=(u8*)ADDR_DATABUFF;
			CopyString((u8 *)"RAM               ",p8);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			p8+=6;
			i=FILENAMEmax_RAM;
			p8+=HEX_ASCII(i,(u32)p8);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
			p8++;
			i=0;
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			for(filename=0;filename<recordFILENAMEmax_RAM;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					i++;
				}
				file++;
			}
			p8+=HEX_ASCII(i,(u32)p8);//HEXתΪASCII�����Addr������Ԫ,������Ч0,������ЧASCII�ַ���
			p8[0]=0;
			WarningOccur(RS485_2PORT,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			
			//��ʱ��
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
			if(UARTCtrl->TaskID!=0)
			{
				break;
			}
			for(i=0;i<60;i++)
			{
				TASK(ACSAMPLEPORT,1);//����;���:RM=0�ϱ�����,RM=1�ɼ�����
				if(UARTCtrl->TaskID!=0)
				{
					break;
				}
			}
			if(i<60)
			{
				return;
			}
					//u8 DL698YMDHMS[10];//����(�ȸ��ֽں���ֽ�),��,��,ʱ,��,��,�ٷ���,��
					x=Comm_Ram->RTCBuff[1];//��
					x=bcd_hex(x);
					i=(Comm_Ram->RTCBuff[1]<<8)+Comm_Ram->RTCBuff[0];//����
					
					if(Comm_Ram->RTCBuff[0]<0x58)
					{
						x/=5;
						x*=5;
						x+=4;
						x=hex_bcd(x);
						Comm_Ram->RTCBuff[1]=x;//��
						Comm_Ram->RTCBuff[0]=0x58;//��
						WriteRTC(ADDR_RTCBUFF);//дʵʱ��
					}
					
/*
						if(i<0x5958)
						{
							Comm_Ram->RTCBuff[2]=0x23;//ʱ
							Comm_Ram->RTCBuff[1]=0x59;//��
							Comm_Ram->RTCBuff[0]=0x58;//��
							WriteRTC(ADDR_RTCBUFF);//дʵʱ��
						}
*/
/*
					switch(i/15)
					{
						case 0:
							if(i<0x1458)
							{
								Comm_Ram->RTCBuff[1]=0x14;//��
								Comm_Ram->RTCBuff[0]=0x58;//��
								WriteRTC(ADDR_RTCBUFF);//дʵʱ��
							}
							break;
						case 1:
							if(i<0x2958)
							{
								Comm_Ram->RTCBuff[1]=0x29;//��
								Comm_Ram->RTCBuff[0]=0x58;//��
								WriteRTC(ADDR_RTCBUFF);//дʵʱ��
							}
							break;
						case 2:
							if(i<0x4458)
							{
								Comm_Ram->RTCBuff[1]=0x44;//��
								Comm_Ram->RTCBuff[0]=0x58;//��
								WriteRTC(ADDR_RTCBUFF);//дʵʱ��
							}
							break;
						case 3:
							if(i<0x5958)
							{
								Comm_Ram->RTCBuff[1]=0x59;//��
								Comm_Ram->RTCBuff[0]=0x58;//��
								WriteRTC(ADDR_RTCBUFF);//дʵʱ��
							}
							break;
					}
*/
					
					
			break;
		
			
			
		default:
			break;
	}
	
	
	
}
#endif



























