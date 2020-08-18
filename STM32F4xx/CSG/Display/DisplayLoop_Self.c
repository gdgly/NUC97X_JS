
//�ն˱�����Ϣ��ʾ
#include "../Hcsg/ProjectCSG.h"

#include "../Display/DisplayLoop_Info.h"
#include "../Display/Display.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"





u32 DisplayLoop_Info(u32 Row)//�ն���Ϣ��ʾ,��������
{
/*
	u32 i;
	u32 x;
	u32 data;
	u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
	
	switch(TerminalDisplayLoop->Count)
	{
		case 0://��վIP���˿ںš���������
			DisplayMenuString(Row,0,2,(u8 *)
																			"��վͨ�Ų���:\n\r"
																			"��IP=\n\r"
																			"���˿ں�=\n\r"
																			"��IP=\n\r"
																			"���˿ں�=\n\r"
																			"�������� =\n\r"
																			"��ǰ���� =\n\r"
																			);//��ʾ�˵��ַ���
			//��IP
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*5));
			for(i=0;i<4;i++)
			{
				data=MRR(ADDR_AFN04F3+i,1);
				data=hex_bcd(data);
				p16=DisplayData_OffZero(data,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				p16[0]='.'+0x2000;
				p16++;
			}
			p16--;
			p16[0]=0;
			//���˿ں�
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
			data=MRR(ADDR_AFN04F3+4,2);
			data=hex_bcd(data);
			DisplayData_OffZero(data,5,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��IP
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*5));
			for(i=0;i<4;i++)
			{
				data=MRR(ADDR_AFN04F3+6+i,1);
				data=hex_bcd(data);
				p16=DisplayData_OffZero(data,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				p16[0]='.'+0x2000;
				p16++;
			}
			p16--;
			p16[0]=0;
			//���˿ں�
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
			data=MRR(ADDR_AFN04F3+10,2);
			data=hex_bcd(data);
			DisplayData_OffZero(data,5,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��������
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			data=MRR(ADDR_AFN04F1+5,1);
			data=hex_bcd(data);
			DisplayData_OffZero(data,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��ǰ����
			Row++;
			i=0;
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
			if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{
				if(UARTCtrl->LinkCount<=2)
				{//��ip
					i=1;
				}
				else
				{//����ip
					i=2;
				}
			}
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
			if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{
				if(UARTCtrl->LinkCount<2)
				{//��ip
					i=1;
				}
				else
				{//����ip
					i=2;
				}
			}
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			switch(i)
			{
				case 0:
					DispalyStringContinue(p16,(u8 *)"��");//��ʾ�ַ�������
					break;
				case 1:
					DispalyStringContinue(p16,(u8 *)"��IP");//��ʾ�ַ�������
					break;
				case 2:
					DispalyStringContinue(p16,(u8 *)"��IP");//��ʾ�ַ�������
					break;
			}
			break;
		case 1://APN������ר���û���������
			DisplayMenuString(Row,0,2,(u8 *)
																			"����ͨ�Ų���:\n\r"
																			"APN=\n\r"
																			" \n\r"
																			"�û���=\n\r"
																			" \n\r"
																			"����=\n\r"
																			);//��ʾ�˵��ַ���
			//APN
			Row++;
			MR(ADDR_DATABUFF,ADDR_AFN04F3+12,16);
			MC(0,ADDR_DATABUFF+16,1);
			DisplayString(Row,4,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
			//����ר���û���
			Row+=2;
			MR(ADDR_DATABUFF,ADDR_AFN04F16,32);
			MC(0,ADDR_DATABUFF+32,1);
			DisplayString(Row,7,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
			//����
			Row+=2;
			MR(ADDR_DATABUFF,ADDR_AFN04F16+32,32);
			MC(0,ADDR_DATABUFF+32,1);
			DisplayString(Row,6,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
			break;
		case 2://��վ�ֻ����롢�������ĺ���
			DisplayMenuString(Row,0,2,(u8 *)
																			"��վͨ�ź���:\n\r"
																			"�绰\n\r"
																			"����\n\r"
																			"����ͨ�ź���:\n\r"
																			"�绰\n\r"
																			"IMSI\n\r"
																			);//��ʾ�˵��ַ���
			//��վ�ֻ�����
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*4));
			for(i=0;i<8;i++)
			{
				data=MRR(ADDR_AFN04F4+i,1);
				x=data>>4;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
				x=data&0xf;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
			}
			//�������ĺ���
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*4));
			for(i=0;i<8;i++)
			{
				data=MRR(ADDR_AFN04F4+8+i,1);
				x=data>>4;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
				x=data&0xf;
				if(x==0xf)
				{
					break;
				}
				if(x==0xa)
				{
					p16[0]=','+0x2000;
				}
				else
				{
					if(x==0xb)
					{
						p16[0]='#'+0x2000;
					}
					else
					{
						p16[0]=x+0x30+0x2000;
					}
				}
				p16++;
			}
			//��������
			Row+=2;
			DispalyStringOnlyOneRow(Row,4,(u8*)ADDR_MobilePhoneNumber);//��ʾ�ַ�������1����
			//����IMSI
			Row++;
			DisplayString(Row,5,0,(u8 *)ADDR_MobilePhoneNumber+20);//��ʾ�ַ���
			break;
		case 3://�������롢�߼���ַ�����̴��롢�豸��š���Ӳ���汾��
			DisplayMenuString(Row,0,2,(u8 *)
																			"�ն���Ϣ:\n\r"
																			"�������� = \n\r"
																			"�߼���ַ = \n\r"
																			"���̴��� = \n\r"
																			"�豸��� = \n\r"
																			"����汾 = \n\r"
																			"Ӳ���汾 = \n\r"
																			);//��ʾ�˵��ַ���
			//��������
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			data=MRR(ADDR_AREACODE,2);
			DisplayData(data,4,0,p16);//������ʾ,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//�߼���ַ
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
#if ((USER/100)==11)//���ּ�����
			if(Comm_Ram->Factory==0x55)
			{//����״̬
				data=MRR(ADDR_TERMINAL,2);
				data=hex_bcd(data);
				DisplayData_OffZero(data,5,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			}
			else
			{
				data=MRR(ADDR_04000401,6);
				DisplayData_OffZero(data,8,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			}
#else
			data=MRR(ADDR_TERMINAL,2);
			data=hex_bcd(data);
			DisplayData_OffZero(data,5,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
#endif
			//���̴���
			Row++;
			MR(ADDR_DATABUFF,ADDR_AFN0CF1,4);
			MC(0,ADDR_DATABUFF+4,1);
			DisplayString(Row,11,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
			//�豸���
			Row++;
			MR(ADDR_DATABUFF,ADDR_AFN0CF1+4,8);
			MC(0,ADDR_DATABUFF+8,1);
			DisplayString(Row,11,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
			//����汾
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
		#if (USER/100)==5//�Ϻ�
			p16[0]='Z'+0x2000;
			p16[1]='Z'+0x2000;
			p16[2]=((SVER>>4)&0xf)+0x2030;
			p16[3]=((SVER>>0)&0xf)+0x2030;
		#else
			p16[0]='V'+0x2000;
			p16[1]=((SVER>>8)&0xf)+0x30+0x2000;
			p16[2]='.'+0x2000;
			p16[3]=((SVER>>4)&0xf)+0x30+0x2000;
			//p16[4]=((SVER>>0)&0xf)+0x30+0x2000;
		#endif
			//Ӳ���汾
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			p16[0]='V'+0x2000;
			p16[1]=((HVER>>8)&0xf)+0x30+0x2000;
			p16[2]='.'+0x2000;
			p16[3]=((HVER>>4)&0xf)+0x30+0x2000;
			//p16[4]=((HVER>>0)&0xf)+0x30+0x2000;
			break;
//		case 4://
//			break;
	}
*/
	return Row;
}










