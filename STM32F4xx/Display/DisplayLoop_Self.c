
//�ն˱�����Ϣ��ʾ
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#endif
#include "../Display/DisplayLoop_Info.h"
#include "../Display/Display.h"

#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"




#if MainProtocol==376
u32 DisplayLoop_Info(u32 Row)//�ն���Ϣ��ʾ,��������
{
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
			#if (USER/100)==12//�Ĵ�
				p16[0]='1'+0x2000;
				p16[1]='6'+0x2000;
				p16[2]='C'+0x2000;
				p16[3]='4'+0x2000;
			#else
				p16[0]='V'+0x2000;
				p16[1]=((SVER>>8)&0xf)+0x30+0x2000;
				p16[2]='.'+0x2000;
				p16[3]=((SVER>>4)&0xf)+0x30+0x2000;
				//p16[4]=((SVER>>0)&0xf)+0x30+0x2000;
			#endif
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
	return Row;
}
#endif


#if MainProtocol==698
#include "../MS/MS.h"
u32 DisplayLoop_Info(u32 Row)//�ն���Ϣ��ʾ,��������
{

	u32 i;
	u32 x;
	u32 y;
	u32 data;
	u8* p8;
	u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
	
//ms_Type * ms;
//ms=Get_ms();
	
	
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
			data=MRR(ADDR_4500_3+6,4);
			for(i=0;i<4;i++)
			{
				x=data&0xff;
				data>>=8;
				x=hex_bcd(x);
				p16=DisplayData_OffZero(x,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				p16[0]='.'+0x2000;
				p16++;
			}
			p16--;
			p16[0]=0;
			//���˿ں�
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
			p8=Get_Element((u8*)ADDR_4500_3+2,2,0,LENmax_4500_3);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				x=(p8[1]<<8)|p8[2];
			}
			else
			{
				x=0;
			}
			x=hex_bcd(x);
			DisplayData_OffZero(x,5,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��IP
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*5));
			p8=Get_Element((u8*)ADDR_4500_3,2,0,LENmax_4500_3);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				data=MRR((u32)p8+4,4);
			}
			else
			{
				data=0;
			}
			for(i=0;i<4;i++)
			{
				x=data&0xff;
				data>>=8;
				x=hex_bcd(x);
				p16=DisplayData_OffZero(x,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
				p16[0]='.'+0x2000;
				p16++;
			}
			p16--;
			p16[0]=0;
			//���˿ں�
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*9));
			if(p8)
			{
				p8=Get_Element(p8,2,0,LENmax_4500_3);//����Ԫ�ص�ַ(�����ַָ��)
				if(p8)
				{
					x=(p8[1]<<8)|p8[2];
				}
				else
				{
					x=0;
				}
			}
			else
			{
				x=0;
			}
			x=hex_bcd(x);
			DisplayData_OffZero(x,5,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��������
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*11));
			p8=Get_Element((u8*)ADDR_4500_2,12,0,LENmax_4500_2);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				x=(p8[1]<<8)|p8[2];
			}
			else
			{
				x=0;
			}
			x=hex_bcd(x);
			DisplayData_OffZero(x,3,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
			//��ǰ����
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*10));
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
			if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
			{
				if(UARTCtrl->LinkCount<=2)
				{//��ip
					p16=DispalyStringContinue(p16,(u8 *)"������IP ");//��ʾ�ַ�������
				}
				else
				{//����ip
					p16=DispalyStringContinue(p16,(u8 *)"���߱�IP ");//��ʾ�ַ�������
				}
				Row++;
				p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*10));
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					if(UARTCtrl->LinkCount<2)
					{//��ip
						p16=DispalyStringContinue(p16,(u8 *)"��̫����IP");//��ʾ�ַ�������
					}
					else
					{//����ip
						p16=DispalyStringContinue(p16,(u8 *)"��̫����IP");//��ʾ�ַ�������
					}
				}
			}
			else
			{
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{
					if(UARTCtrl->LinkCount<2)
					{//��ip
						p16=DispalyStringContinue(p16,(u8 *)"��̫����IP");//��ʾ�ַ�������
					}
					else
					{//����ip
						p16=DispalyStringContinue(p16,(u8 *)"��̫����IP");//��ʾ�ַ�������
					}
				}
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
			p8=Get_Element((u8*)ADDR_4500_2,6,0,LENmax_4500_2);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				x=p8[1];
				if(x>32)
				{
					x=32;
				}
				p8[2+x]=0;
				DisplayString(Row,4,0,p8+2);//��ʾ�ַ���
			}
			//����ר���û���
			Row+=2;
			p8=Get_Element((u8*)ADDR_4500_2,7,0,LENmax_4500_2);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				x=p8[1];
				if(x>32)
				{
					x=32;
				}
				p8[2+x]=0;
				DisplayString(Row,7,0,p8+2);//��ʾ�ַ���
			}
			//����
			Row+=2;
			p8=Get_Element((u8*)ADDR_4500_2,8,0,LENmax_4500_2);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				x=p8[1];
				if(x>32)
				{
					x=32;
				}
				p8[2+x]=0;
				DisplayString(Row,5,0,p8+2);//��ʾ�ַ���
			}
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
//#define ADDR_4500_4     ADDR_4500_3+LENmax_4500_3//����ͨ��ģ��1;4����ͨ�Ų���
//����ͨ�Ų�������=structure
//{
//�������ĺ���     visible-string(SIZE(16))��
//��վ����         array visible-string(SIZE(16))
//����֪ͨĿ�ĺ��� array visible-string(SIZE(16))
//}
			p8=Get_Element((u8*)ADDR_4500_4,2,0,LENmax_4500_4);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				x=p8[1];
				if(x>16)
				{
					x=16;
				}
				p8[2+x]=0;
				DisplayString(Row,4,0,p8+2);//��ʾ�ַ���
			}
			//�������ĺ���
			Row++;
			p16=(u16 *)(ADDR_STRINGBUFF+(84*Row)+4+(2*4));
			p8=Get_Element((u8*)ADDR_4500_4,1,0,LENmax_4500_4);//����Ԫ�ص�ַ(�����ַָ��)
			if(p8)
			{
				x=p8[1];
				if(x>16)
				{
					x=16;
				}
				p8[2+x]=0;
				DisplayString(Row,4,0,p8+2);//��ʾ�ַ���
			}

			//��������
			Row+=2;
			DispalyStringOnlyOneRow(Row,4,(u8*)ADDR_MobilePhoneNumber);//��ʾ�ַ�������1����
			//����IMSI
			Row++;
			DisplayString(Row,4,0,(u8 *)ADDR_MobilePhoneNumber+20);//��ʾ�ַ���

			break;
		case 3://�߼���ַ�����̴��롢�豸��š���Ӳ���汾��
			DisplayMenuString(Row,0,2,(u8 *)
																			"�ն���Ϣ:\n\r"
																			"�ն˵�ַ = \n\r"
																			"\n\r"
																			"�ͻ���� = \n\r"
																			"\n\r"
																			"���̴��� = \n\r"
																			"����汾 = \n\r"
																			"Ӳ���汾 = \n\r"
																			);//��ʾ�˵��ַ���
			//�ն˵�ַ
			Row+=2;
			MR(ADDR_DATABUFF,ADDR_4001+1,17);
			p8=(u8*)ADDR_DATABUFF;
			y=p8[0];
			if(y>10)
			{
				y=10;
			}
			p8++;
			for(i=0;i<y;i++)
			{
				x=p8[i];
				x>>=4;
				if(x<10)
				{
					x+=0x30;
				}
				else
				{
					x+=55;
				}
				p8[17+(2*i)]=x;
				p16++;
				x=p8[i];
				x&=0xf;
				if(x<10)
				{
					x+=0x30;
				}
				else
				{
					x+=55;
				}
				p8[17+(2*i)+1]=x;
			}
			p8[17+(2*i)]=0;
			DisplayString(Row,0,0,p8+17);//��ʾ�ַ���
			//�ͻ����
			Row+=2;
			MR(ADDR_DATABUFF,ADDR_4003+1,17);
			p8=(u8*)ADDR_DATABUFF;
			y=p8[0];
			if(y>10)
			{
				y=10;
			}
			p8++;
			for(i=0;i<y;i++)
			{
				x=p8[i];
				x>>=4;
				if(x<10)
				{
					x+=0x30;
				}
				else
				{
					x+=55;
				}
				p8[17+(2*i)]=x;
				p16++;
				x=p8[i];
				x&=0xf;
				if(x<10)
				{
					x+=0x30;
				}
				else
				{
					x+=55;
				}
				p8[17+(2*i)+1]=x;
			}
			p8[17+(2*i)]=0;
			DisplayString(Row,0,0,p8+17);//��ʾ�ַ���
			//���̴���
			Row++;
	#if (USER/100)==0
			
	#else
		#if (USER/100)==9//����
			p8=(u8 *)ADDR_DATABUFF;
			p8[0]='Z';
			p8[1]='J';
			p8[2]='Z';
			p8[3]='T';
			p8[4]=0;
			DisplayString(Row,11,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
		#else
			p8=(u8 *)ADDR_DATABUFF;
			p8[0]='C';
			p8[1]='H';
			p8[2]='I';
			p8[3]='N';
			p8[4]='T';
			p8[5]=0;
			DisplayString(Row,11,0,(u8 *)ADDR_DATABUFF);//��ʾ�ַ���
		#endif
	#endif
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
	return Row;
}
#endif






