
//��������(��չ)
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"






void Uart_AFN255(u32 PORTn)//��������(��չ)
{
	u32 i;
	u32 DIlen;
	u32 Fn;
//	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 *p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<12)
	{
err:
		//ȫ������
		Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		return;
ok:
		//ȫ��ȷ��
		Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		BUZZ_Generate(2);//��������,��ڷ�����
		return;
	}
//Tpʱ���ǩ��Ч�ж�

//PW��֤

	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//�û������ݱ�ʶ�Ͳ������ݳ���
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	if(DIlen<4)
	{
		goto err;
	}
//����(����վ������10������1�����ݣ�����Ӧ�ò�����ȷ�ϣ�CON=1������·����)
	DIaddr=((u32)p8rxbuff)+14;
	RXaddr=((u32)p8rxbuff)+14+4;
	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		if(i==0x0)//���ݱ�ʶ�ֽ�ΪFnPn
		{
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
		}
		else
		{
			Fn=(i>>16);
			if((Fn!=32)&&(Fn!=37))
			{//���Ƕ�
				if(Comm_Ram->Factory!=0x55)//0x55=����״̬
				{
					goto err;
				}
			}
			//Pn=i&0xffff;
			switch(Fn)//Fn
			{
				case 14://�Բ�ģʽ1(Ӳ������)
					if(FileDownload->File&(1<<4))//�ļ���־(��λ��ʾOK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=Font_ttf,B7=Module,...B31=
					{
						p32=(u32*)ADDR_Menu_ENTRY;
						if(p32[128]==8)
						{//�����:0=MS,1=OS,2=Meter,3=Terminal,4=Test,5=Font,6=PACK,7=Router,8=TEST
							//ȫ��ȷ��
							Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
							DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
							BUZZ_Generate(2);//��������,��ڷ�����
							while(1)
							{
								p32=(u32*)ADDR_MS_ENTRY;
								(*(void(*)())p32[115])();//Main_Thread; 115://���߳�
								if(UARTCtrl->Task==0)
								{
									break;
								}
							}
							p32=(u32*)ADDR_Menu_ENTRY;
							(*(void(*)())p32[115])();//115://�������ܲ���
							goto ok;
						}
					}
					goto err;
				case 15://�Բ�ģʽ2(FLASH��д����)

					goto err;
				case 16://�Բ�ģʽ3(FLASH������������)

					goto err;
				case 17://�Բ�ģʽ3(FLASH�������Լ���)

					goto err;
				case 32://���Լ���Ϣ
					MC(0,((u32)p8txbuff)+18,128);
					//4�ֽڷ�����
					MWR(Project,((u32)p8txbuff)+18,4);
					//4�ֽ����ģ������־
					MR(((u32)p8txbuff)+18+4,(u32)&FileDownload->File,4);
//					//4�ֽ�IC��Ӳ�������־
//					 //B8=MESAM ���ܱ���ESAMоƬ(��Ӳ������1 ʱ��)
//					 //B9=TESAM �ն���ESAMоƬ(��Ӳ������1 ʱ��)
//					i=MRR(ADDR_MeterError_IEEPROM_Start+64+28,4);
//					i&=0x300;
//					Comm_Ram->ErrorIC|=i;
//					MR(((u32)p8txbuff)+18+8,(u32)&Comm_Ram->ErrorIC,4);
//					//4�ֽ�IS61LV25616�������ߴ����־
//					MR(((u32)p8txbuff)+18+12,ADDR_MeterError_IEEPROM_Start+64+32,4);
//					//4�ֽ�IS61LV25616��ַ���ߴ����־
//					MR(((u32)p8txbuff)+18+16,ADDR_MeterError_IEEPROM_Start+64+32+4,4);
//					//4�ֽ�SST39VF6401B�������ߴ����־
//					MR(((u32)p8txbuff)+18+20,ADDR_MeterError_IEEPROM_Start+64+32+8,4);
//					//4�ֽ�SST39VF6401B��ַ���ߴ����־
//					MR(((u32)p8txbuff)+18+24,ADDR_MeterError_IEEPROM_Start+64+32+12,4);
//					//4�ֽ�MX29GLxxx�������ߴ����־
//					MR(((u32)p8txbuff)+18+28,ADDR_MeterError_IEEPROM_Start+64+32+16,4);
//					//4�ֽ�MX29GLxxx��ַ���ߴ����־
//					MR(((u32)p8txbuff)+18+32,ADDR_MeterError_IEEPROM_Start+64+32+20,4);
//					//4�ֽ���������Բ��־:(��λ��ʾ����,ע:��IRAM�б�־��λ��ʾOK�෴)
//					i=MRR(ADDR_MeterError_IEEPROM_Start+64+32+24,4);
//					i=~i;
//					MWR(i,((u32)p8txbuff)+18+36,4);
//					//4�ֽ��Բ��־:(��λ��ʾ����)
//					MR(((u32)p8txbuff)+18+40,ADDR_MeterError_IEEPROM_Start+64+32+28,4);
//				

					//�����ܽ���
					i=MRR(ADDR_Characteristics_BASE1+OFFSET_TestResult_Characteristics,1);//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
					if(i)
					{
						MC(0xff,((u32)p8txbuff)+18+40,4);
					}
					
					i=4+128;
					i+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
					p8txbuff[1]=i;
					p8txbuff[2]=i>>8;
					Uart_376_NoAUX(PORTn);//û������Ϣ
					Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
					break;
				case 37://���а汾��Ϣ
					MC(0,((u32)p8txbuff)+18,256);
					//4*8�ֽ�I0�汾
					i=MRR(ADDR_MS_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18,i,32);
					//4*8�ֽ�I1�汾
					i=MRR(ADDR_OS_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+32,i,32);
					//4*8�ֽ�I2�汾
					i=MRR(ADDR_Meter_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+64,i,32);
					//4*8�ֽ�E1�汾
					i=MRR(ADDR_Terminal_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+96,i,32);
					//4*8�ֽ�E2�汾
					i=MRR(ADDR_Menu_ENTRY+(4*114),4);
					MR(((u32)p8txbuff)+18+128,i,32);
					
					//
					i=4+256;
					i+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
					p8txbuff[1]=i;
					p8txbuff[2]=i>>8;
					Uart_376_NoAUX(PORTn);//û������Ϣ
					Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
					break;
				case 101://����2�ֽ�������ַ��2�ֽ��ն˵�ַ��8�ֽ��豸���,6�ֽ�MAC��ַ
					MW(RXaddr,ADDR_AREACODE,18);
					MW(RXaddr+4,ADDR_AFN0CF1+4,8);//�ն˰汾��Ϣ�е��豸���
					MR(((u32)p8txbuff)+7,ADDR_AREACODE,4);//���ն�ʵ��ַ���뷢����
					goto ok;
				case 102://���ù���ģʽ��ʱʱ��(����ģʽ��Ч�ֶ�ʱ��hex)
		#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
					MW(RXaddr,ADDR_MeterFMTimer+14,2);
		#else
					MW(RXaddr,ADDR_TerminalFMTimer+14,2);
		#endif
					goto ok;
				default:
					goto err;
			}
		}
	}
}




