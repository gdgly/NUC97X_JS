
//��������
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





//����127���빤���Բⷢ��֡(OAD=0xFFFF7F00,��ַ����0xAAAAAAAAAAAAͨ���ַ)
//68 18 00 43 05 aa aa aa aa aa aa 10 67 26 07 01 01 ff ff 7f 00 00 00 90 18 16
//����֡
//68 1A 00 C3 05 01 00 00 00 00 00 10 9F 85 87 01 01 FF FF 7F 00 00 00 00 00 C8 18 16 (��ȷ)
//68 1A 00 C3 05 01 00 00 00 00 00 10 9F 85 87 01 01 FF FF 7F 00 06 00 00 00 52 53 16 (����,�ն˵�ǰ�ǹ���ģʽ���ް�װ�Բ�����ȵ�)

//����128�˳�����ģʽ����֡(OAD=0xFFFF8000,��ַ����0xAAAAAAAAAAAAͨ���ַ)
//68 18 00 43 05 aa aa aa aa aa aa 10 67 26 07 01 01 ff ff 80 00 00 00 42 dd 16
//����֡
//68 1a 00 c3 05 01 00 00 00 00 00 10 9f 85 87 01 01 ff ff 80 00 00 00 00 00 e2 e5 16 (��ȷ)

//�������Բ�������֡(OAD=0xFFFF0200,��ַ����0xAAAAAAAAAAAAͨ���ַ)
//68 17 00 43 05 aa aa aa aa aa aa 10 2b 3a 05 01 01 ff ff 02 00 00 d7 ea 16
//����֡(���ش�����Ϣ:���޴���ȫ��ȷʱ���ؿ�����,���д���ʱ������������Visible String)
//68 1A 00 C3 05 01 00 00 00 00 00 10 9F 85 85 01 01 FF FF 02 00 01 00 00 00 8F 5F 16(�޴���ȫ��ȷ��
//68 24 00 c3 05 01 00 00 00 00 00 10 8d 5f 85 01 01 ff ff 02 00 01 0a 82 00 07 4e 4f 20 54 45 53 54 00 00 a5 80 16  (4e 4f 20 54 45 53 54��ASII"NO TEST")
//68 29 0D C3 05 01 00 00 00 00 00 10 71 1D 85 01 01 FF FF 02 00 01 0A 82 0D 0C ......    (ASCII��Ĳ��Ա����ĵ�,�ĵ����Ƚӽ�4k)

//���ɳ���ز���״̬������Ϣ����֡(OAD=0xFFFF0300,��ַ����0xAAAAAAAAAAAAͨ���ַ)
//68 17 00 43 05 aa aa aa aa aa aa 10 2b 3a 05 01 01 ff ff 03 00 00 0b b0 16
//����֡(���ش�����Ϣ:���޴���ȫ��ȷʱ���ؿ�����,���д���ʱ������������Visible String)
//68 1a 00 c3 05 01 00 00 00 00 00 10 9f 85 85 01 01 ff ff 03 00 01 00 00 00 a4 5b 16(�޴���ȫ��ȷ��
//68 22 00 C3 05 01 00 00 00 00 00 10 FA AA 85 01 01 FF FF 03 00 01 0A 82 00 05 45 52 52 4F 52 00 00 CE 18 16   (45 52 52 4F 52��ASII"ERROR")




u32 GET_OAD_FactoryTest(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//���������Խ��;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	switch(OAD)
	{
		case 0xFFFF0200://�����ܽ���
			//������Ϣ
			p8tx[0]=DataType_visible_string;
			p8tx[1]=0x82;
			p8tx[2]=0;
			p8tx[3]=0;
			Len=4;
			//û����
		#ifdef IC_STM32F4xx
			p8=(u8*)ADDR_Characteristics_BASE1+(OFFSET_TestResult_Characteristics);
		#endif
		#ifdef IC_NUC9xx
			p8=(u8*)((ADDR_Characteristics_BASE1)-(ADDR_IEEPROM_START))+ADDR_SPI_MAP;//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
			p8+=OFFSET_TestResult_Characteristics;
		#endif
			switch(p8[0])
			{
				case 0://�ϸ�
					p8tx[0]=0;
					return 1;
				case 1://���ϸ�
					if(LENmax_TxSPACE<4)
					{
						UARTCtrl->DAR=11;//�����ڷ�֡����״̬   ��11��
						return 0;
					}
					Len=TestReport();//���ɲ��Ա����ļ�;����:�ļ�����(���ɲ��Ա����ļ���ADDR_128KWRITE_BUFF����д��os�ļ�ϵͳ)
					if(Comm_Ram->Factory==0x55)//����״̬
					{
						LENmax_TxSPACE+=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ��峤��
					}
					if(Len>(LENmax_TxSPACE-4))
					{
						Len=LENmax_TxSPACE-4;
					}
					MR((u32)p8tx+4,ADDR_128KWRITE_BUFF,Len);
					Len+=4;
					break;
				default://û����
					CopyString((u8*)"NO TEST",p8tx+Len);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
					Len+=7;
					break;
			}
			p8tx[2]=(Len-4)>>8;
			p8tx[3]=(Len-4);
			return Len;
		case 0xFFFF0300://���ɳ���ز���״̬
			i=MRR(ADDR_UBACKUP,2);
			if(i)
			{//�г����
				p8tx[0]=0;
				return 1;
			}
			p8tx[0]=DataType_visible_string;
			p8tx[1]=0x82;
			p8tx[2]=0;
			p8tx[3]=0;
			Len=4;
			CopyString((u8*)"ERROR",p8tx+Len);//�����ַ���,�ַ���ps������pd,���ؿ����ַ������ֽ���
			Len+=5;
			p8tx[2]=(Len-4)>>8;
			p8tx[3]=(Len-4);
			return Len;
		default:
			UARTCtrl->DAR=6;//���ݷ��ʽ��
			return 0;
	}
}

u32 ACTION_FFFF(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//����;����:b31-b16�����������ݳ���(������4�ֽ�OMD),b15-b0���뷢�������践�����ݳ���(������4�ֽ�OMD),���ݷ��ʽ��UARTCtrl->DAR
{
	u32 LEN_TxAPDU;
	u32 *p32;
	UARTCtrl_TypeDef *UARTCtrl;
	#ifdef LINUX
	ms_Type *ms;//�õ�ms���ַ
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(p8rx[2])
	{
		case 127://����127���Լ�
			if(Comm_Ram->Factory==0x55)//����״̬
			{
				if(FileDownload->File&(1<<4))//�ļ���־(��λ��ʾOK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,...B31=
				{
					p32=(u32*)ADDR_Menu_ENTRY;
					if(p32[128]==8)
					{//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Test,5=Font,6=PACK,7=Router,8=TEST
						LEN_TxAPDU=9;
						Uart_698_TxResponse(PORTn,0,LEN_TxAPDU);//DL698������Ӧ����,��������p8tx+TxAPDUOffset�Ŀ�ʼ��ַ��
						//BUZZ_Generate(2);//��������,��ڷ�����
						while(1)
						{
						#ifndef LINUX
							p32=(u32*)ADDR_MS_ENTRY;
							(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
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
						(*(void(*)())p32[115])();//115://�������ܲ�������OSǰ
					#else
						(*(void(*)())p32[116])();//116://�������ܲ���
					#endif
					}
				}
			}
			UARTCtrl->DAR=6;//���󲻴��ڣ�6��
			return 0;
		case 128://����128���˳�����ģʽ
			MWR(0,ADDR_MeterFMTimer+14,2);
			MWR(0,ADDR_TerminalFMTimer+14,2);
			//����״̬�˳�ʱ����ǰͨ�ŵ�ַ����Ϊ����ͨ�ŵ�ַ
			MR(ADDR_DATABUFF,ADDR_4001,LENmax_4001_Factory);
			MW(ADDR_DATABUFF,ADDR_4001_Factory,LENmax_4001_Factory);
			return 0;
	}
	UARTCtrl->DAR=6;//���󲻴��ڣ�6��
	return 0;
}



