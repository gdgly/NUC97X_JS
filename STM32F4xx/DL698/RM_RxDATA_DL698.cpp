
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"
#include "../Device/IC_TESAM.h"
#include "../DL698/RM_Record.h"

#include "../MS/MS.h"
//void my_printf(char *pStr,u32 param1,u32 param2,u32 param3)
//{
//	ms_Type *ms;
//	ms=Get_ms();
//	ms->link_printf((char *)pStr,param1,param2,param3);
//}

 #include "../DL698/DL698_JSON.h"
u32 RM_RxDATA_DL698_CHECK(u32 PORTn)//DL698֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
{
	u32 i;
	u32 x;
	u32 n;
	u8* p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
#if RM_SECURITY_CompMAC==1//0=��,1=��,��ȫģʽ����ʱ����MACУ����Ƚ�
	u8* p8;
	u32 DL698ByteTypeLenByte;
#endif
	u64 d64a;
	u64 d64b;
	u32 y;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	if((p8rx[4]&0x0f)!=5)
	{
		return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
/*
#if ((Project/100)==2)//������
#if SEL_RouterRunMode3ADDR==1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
	if((PORTn==RS485_4PORT)&&(Terminal_Router->RouterRunMode_1==3))//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
	{//΢��������,Ŀ���ַ�òɼ�����ַ
		i=UARTCtrl->TaskID;
		i>>=8;
		i&=0xff;
		switch(i)
		{
			case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
				i=UARTCtrl->MeterCount[i];
				i=PowerEvent->setno[i];
				break;
			default:
				i=UARTCtrl->MeterCount[i];
				break;
		}
		p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
		p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ
		if(p8)
		{//�ɼ�����ȡ
			p8+=5;
			x=p8[4]|(p8[5]<<8);
			i=(p8rx[5]<<8)|p8rx[6];
			if(i!=x)
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			x=p8[2]|(p8[3]<<8);
			i=(p8rx[7]<<8)|p8rx[8];
			if(i!=x)
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			x=p8[0]|(p8[1]<<8);
			i=(p8rx[9]<<8)|p8rx[10];
			if(i!=x)
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
		}
		else
		{
			return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
		}
	}
	else
#endif
#endif
*/
	#if (USER/100)==17//����ϵ
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;
	if(i==0x22)
	{
		if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
		//if(((UARTCtrl->TaskID>>20)&0xf)==5)
		{
			switch(i)
			{
				case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
					i=UARTCtrl->MeterCount[i];
					i=PowerEvent->setno[i];
					break;
				default:
					i=UARTCtrl->MeterCount[i];
					break;
			}
			d64a=MRR((u32)p8rx+5,6);
			#if 0
			x=Terminal_Router->NoteNum1;//��֪�ܴӽڵ���
			for(y=0;y<x;y++)
			{
				d64b=MRR((u32)&NoteList1->List1[y].CollectorAddr,6);
				if(d64a==d64b)
				{
					break;//ԭ�Ѵ���
				}
			}
			if(y==x)
			{
				x=Terminal_Router->NoteNum1;//��֪�ܴӽڵ���
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList1->List1[y].Addr,6);
					if(d64a==d64b)
					{
						break;//ԭ�Ѵ���
					}
				}
				if(x==y)
				{
					i=(p8rx[5]<<8)|p8rx[6];
					if(i!=UARTCtrl->RMAddr[2])
					{
						myprintf("[%s:%d]:��ַ��\n",(u32)&__func__,(u32)__LINE__,0);
						return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
					}
					i=(p8rx[7]<<8)|p8rx[8];
					if(i!=UARTCtrl->RMAddr[1])
					{
						myprintf("[%s:%d]:��ַ��\n",(u32)&__func__,(u32)__LINE__,0);
						return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
					}
					i=(p8rx[9]<<8)|p8rx[10];
					if(i!=UARTCtrl->RMAddr[0])
					{
						myprintf("[%s:%d]:��ַ��\n",(u32)&__func__,(u32)__LINE__,0);
						return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
					}
				}	
			}
			#else
			x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
			for(y=0;y<x;y++)
			{
				d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
				if(d64a==d64b)
				{
					break;//ԭ�Ѵ���
				}
			}
			if(y==x)
			{
				x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList->List[y].Addr,6);
					if(d64a==d64b)
					{
						break;//ԭ�Ѵ���
					}
				}
				if(x==y)
				{
					i=(p8rx[5]<<8)|p8rx[6];
					if(i!=UARTCtrl->RMAddr[2])
					{
						return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
					}
					i=(p8rx[7]<<8)|p8rx[8];
					if(i!=UARTCtrl->RMAddr[1])
					{
						return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
					}
					i=(p8rx[9]<<8)|p8rx[10];
					if(i!=UARTCtrl->RMAddr[0])
					{
						return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
					}
				}	
			}
			#endif
		}
		else
		{
			i=(p8rx[5]<<8)|p8rx[6];
			if(i!=UARTCtrl->RMAddr[2])
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			i=(p8rx[7]<<8)|p8rx[8];
			if(i!=UARTCtrl->RMAddr[1])
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			i=(p8rx[9]<<8)|p8rx[10];
			if(i!=UARTCtrl->RMAddr[0])
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
		}
	}
	#else
	{
		i=(p8rx[5]<<8)|p8rx[6];
		if(i!=UARTCtrl->RMAddr[2])
		{
			return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
		}
		i=(p8rx[7]<<8)|p8rx[8];
		if(i!=UARTCtrl->RMAddr[1])
		{
			return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
		}
		i=(p8rx[9]<<8)|p8rx[10];
		if(i!=UARTCtrl->RMAddr[0])
		{
			return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
		}
	}
	#endif
#if (USER/100)==17//����ϵ
	if((p8rx[3]&0x0f)!=0x03)
	{
		return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
#else
	//������
	if(p8rx[3]!=0xc3)
	{
		return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
#endif
	//����
	i=p8rx[1]+(p8rx[2]<<8);
	if(i<=(13+8+3))
	{
		return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
	
	//��ȫģʽ��ȡ
	if(p8rx[14]==0x90)
	{
		//����+MAC��ʽ��֤��������
//68 72 00 C3 05 01 00 00 00 00 00 00 3B 06 
//90 ��ȫ��Ӧ   		[144]	SECURITY-Response
//00 ����Ӧ�����ݵ�Ԫ   [0]  octet-string
//59 ����89
//85 03 05 50 04 02 00 03 00 20
//21 02 00 00 00 10 02 00 00 00 
//20 02 00 01 01 1C 07 E2 09 07 
//00 00 00 01 05 06 00 00 03 E8 
//06 00 00 01 90 06 00 00 01 2C 
//06 00 00 00 C8 06 00 00 00 64 
//01 05 06 00 00 03 E8 06 00 00 
//01 90 06 00 00 01 2C 06 00 00 
//00 C8 06 00 00 00 64 00 00 
//01 ������֤��Ϣ  CHOICE   OPTIONAL=1
//00 ����MAC    [0]  MAC
//04 24 80 09 E0
//AA AC 16
		
 #if RM_SECURITY_CompMAC==1//0=��,1=��,��ȫģʽ����ʱ����MACУ����Ƚ�
	#if RM_SECURITY_MeterNo==0//0=��,1=��;DL698��ȫģʽ����ʱ�ȳ�������� �� ESAM���к�
		#error
	#endif
	//ע!!!:���ܱ���������Կ���¹������ܱ��ź���ܱȶ�ͨ��
	if(PORTn!=RS485_4PORT)
	{//ע!!!:�ز��ڲ�698�¼������ϱ�������ʱ������MAC
		//���õ��ܱ��űȽ�
		MR(ADDR_UART7Tx+7,((u32)&UARTCtrl->RM_Rx_date_40020200)+2,8);
		//16byte Rand
		MR(ADDR_UART7Tx+7+8,(u32)&UARTCtrl->Rand,16);
		//Data2(����APDU)
		DL698ByteTypeLenByte=Get_DL698ByteTypeLenByte(p8rx+16);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
		x=Get_DL698ByteTypeLenValue(p8rx+16);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
		if((x+2+6+16+4)>(LEN_UART7Tx-7))
		{
			return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
		}
		MR(ADDR_UART7Tx+7+8+16,(u32)p8rx+16+DL698ByteTypeLenByte,x);
		//4byte MAC
		n=Get_DL698ByteTypeLenByte(p8rx+16+DL698ByteTypeLenByte+x+2);//�õ�DL698�ֽ�����������ʾ���ȵ�Ԫ���ֽ���
		MR(ADDR_UART7Tx+7+8+16+x,(u32)p8rx+16+DL698ByteTypeLenByte+x+2+n,4);
		x+=8+16+4;
		//���ͣ� 800E4887+LC+�����+Rand+Data2+MAC
		//���أ� 9000+0000
		i=TESAM_CMD_TxRx(0x800E4887,x,ADDR_UART7Tx+7);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
		if(i)
		{
			//����8byte ͨ�ŵ�ַ(̨����������ͨ�ŵ�ַ����)
			p8=(u8*)ADDR_UART7Tx+7;
			p8[0]=0;
			p8[1]=0;
			MR(ADDR_UART7Tx+7+2,(u32)&UARTCtrl->RMAddr,6);
			i=TESAM_CMD_TxRx(0x800E4887,x,ADDR_UART7Tx+7);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{
				return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
		}
	}
 #endif
		//
		x=p8rx[16];
		if(x<128)
		{
			MR((u32)p8rx+14,(u32)p8rx+17,x);
		}
		else
		{
			x&=0x3f;//��ʾ���ȵ��ֽ���
			if(x==1)
			{//1byte����
				x=p8rx[17];
				MR((u32)p8rx+14,(u32)p8rx+18,x);
			}
			else
			{//2byte����
				x=(p8rx[17]<<8)|p8rx[18];
				if(x>(LEN_UART2Rx-14))
				{
					return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
				}
				MR((u32)p8rx+14,(u32)p8rx+19,x);
			}
		}
		x+=14+2;//(������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���))
		x--;
		p8rx[1]=x;
		p8rx[2]=x>>8;
		Uart_698_HCSFCS(p8rx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	}
#if (USER/100)==17//����ϵ
	if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
	//if(((UARTCtrl->TaskID>>20)&0xf)==5)
	{
		return 0;
	}
#endif

	//OAD�Ƚ�
	i=p8rx[15];
	switch(i)
	{
		default:
			return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	#if (USER/100)==0//���Ժ
		//�����Բ���,�Զ��ϱ��ն˳���698�����¼��ķ���֡��BUG(85 01��Ϊ85 10)
		case 0x10:
	#endif
		case 0x01://GetRequestNormal
/*
			//ע:�����������ʵʱ����
			i=UARTCtrl->RMCSD[2+0]<<24;
			i|=UARTCtrl->RMCSD[2+1]<<16;
			i|=UARTCtrl->RMCSD[2+2]<<8;
			i|=UARTCtrl->RMCSD[2+3];
			if(i==0x50020200)
			{//���Ӷ���
				i=UARTCtrl->TaskID;
				i=(i>>8)&0xff;//�����
				i=UARTCtrl->MeterCount[i];//ȡ������
				p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
				p8=Get_Element(p8,2,8,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8)
				{
					i=p8[1];//ȡ���߷�ʽ
					if(i==1)//δ֪��0�������ࣨ1�����������ߣ�2�����������ߣ�3��
					{//���������,��ʵʱ����
						i=p8rx[17+0]<<24;
						i|=p8rx[17+1]<<16;
						i|=p8rx[17+2]<<8;
						i|=p8rx[17+3]<<0;
						if(i!=UARTCtrl->RM_DI)
						{
							return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
						}
						break;
					}
				}
			}
*/
			for(i=0;i<4;i++)
			{
				if(p8rx[17+i]!=UARTCtrl->RMCSD[2+i])
				{
					break;
				}
			}
			if(i<4)
			{
				return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			if(p8rx[21]!=1)
			{//0=����1=��¼����
				return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			break;
	#if (USER/100)==17//����ϵ
		case 2://GetRequestNormalList
		case 6://ReportNotificationSimplifyRecord
			return 0;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	#endif
//		case 2://GetRequestNormalList
//			return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
		case 0x03://GeResponseRecord
			for(i=0;i<4;i++)
			{
				if(p8rx[17+i]!=UARTCtrl->RMCSD[2+i])
				{
					return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
				}
			}
			i=p8rx[21];//RCSD��SEQUENCE OF����
			i*=5;
			if(p8rx[22+i]!=1)
			{//01 ���� ��¼����
				return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			if(p8rx[22+i+1]==0)
			{//��¼���ݸ���=0
				return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			i=UARTCtrl->TaskID;
			switch((i>>24)&0xf)//��������
			{
				case 2://�¼��ɼ�����
					return 0;
				default://��ͨ�ɼ�����
					//�Ƚ϶���ʱ��(�����)
		//68 68 00 C3 05 01 00 00 00 00 00 00 F6 60 85 03 1D    17byte
		//50 02 02 00 03 00 20 21 02 00 00 00 10 02 00 00 00 20 02 00 01 01 1C 07 E1 04 19 00 0F 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 00 00 4D 92 16
					i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
					switch(i)
					{
						case 0x5002://���Ӷ���(����������)
						//case 0x5004://�ն���
						//case 0x5006://�¶���
							switch((UARTCtrl->TaskID>>20)&0xf)//�ɼ�����
							{
								default:
								case 0://0=�ɼ���ǰ����
								case 1://1=�ɼ��ϵ�N��
									//���Ƚ�
									return 0;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
								case 2://2=������ʱ��ɼ�
								case 3://3=��ʱ�����ɼ�
									break;
								case 4://4=RetryMetering ����
									break;
							}
							n=UARTCtrl->RMCSD[6];//RCSD��SEQUENCE OF����
							for(i=0;i<n;i++)
							{
								x=(UARTCtrl->RMCSD[7+(i*4)]<<8)|UARTCtrl->RMCSD[7+(i*4)+1];
								if(x==0x2021)
								{//�����ݶ���ʱ����ѡ����
									n=p8rx[21];//RCSD��SEQUENCE OF����
									for(i=0;i<n;i++)
									{
										x=p8rx[22+(i*5)+1]<<8;
										x|=p8rx[22+(i*5)+2];
										if(x==0x2021)
										{//�ҵ����ն���ʱ����ѡ��
											p8rx+=22+(5*n)+2;
											while(i--)
											{
												p8rx+=Get_DL698DataLen_S(p8rx,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
											}
									#if CompareTime_5002==0//�������Ӷ���(����������)�Ƿ�Ƚ϶���ʱ��:0=���Ƚ�,1=�Ƚ�
											p8rx++;
											for(i=0;i<7;i++)//�벻�Ƚ�
											{
												p8rx[i]=UARTCtrl->date_time_s[i];
											}
											return 0;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
									#else
											if(p8rx[0]!=DataType_date_time_s)
											{//����Ϊ��
												return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
											}
											p8rx++;
											for(i=0;i<6;i++)//�벻�Ƚ�
											{
												if(p8rx[i]!=UARTCtrl->date_time_s[i])
												{
													break;
												}
											}
											if(i>=6)
											{//����ʱ����ͬ
												return 0;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
											}
											return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
									#endif
										}
									}
									//û�ҵ����ն���ʱ����ѡ��
									return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
								}
							}
							break;
					}
					break;
			}
			break;
//		case 4://GetRequestRecordList
//			return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
//		case 5://GetRequestNext
//			
//			break;
	}
	return 0;
}


//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     512
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������)(ROADʱ��д���ܹ�����OAD��������!!!)

u32 RM_RxDATA_DL698(u32 PORTn)//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
{
	//u32 iii;
	u32 iRes;
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 OAD;
	u32 ROAD;
	u32 NUM_RCSD;
	u32 OADrx;
	u32 NUM_RCSDrx;
	u8* p8;
//	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
//	u8* p8Array;
	u32 NUM_RecordRow;
	u32 NUM_AddrData;
	u8* p8RMADDR;
	u32 NUM_ADDR;
	u8* p8Data;
	u16* p16Data;
	u8* p8rx;
	u32 m;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);

	
#if(USER/100)==17//����ϵ
	i=p8rx[14];
	if(i==0x88)//�ɼ����������ϱ�
	{
		UARTCtrl->res23=0;//һ��ʼ��ֵΪ0
		p8Data=(u8 *)ADDR_DATABUFF;
		i=p8rx[15];
		//UARTCtrl->PIID=p8rx[16];
		UARTCtrl->PIID=(p8rx[16]&0x3f);
		x=0;
		switch(i)
		{
			case 6://ReportNotificationSimplifyRecord�ϱ���ʽ
				if(p8rx[21]==7)
				{
					p8RMADDR=(u8*)ADDR_4KWRITE_BUFF;
					if(p8rx[39]==3)//MS:һ���û���ַ
					{
						NUM_ADDR=p8rx[40];
						for(i=0;i<NUM_ADDR;i++)
						{
							MR((u32)p8RMADDR+i*6,(u32)p8rx+43+i*8,6);//
						}
					}                      
					
					MR((u32)p8Data+8,ADDR_DL698YMDHMS,7);//�ɼ��ɹ�ʱ��
					MR((u32)p8Data+15,ADDR_DL698YMDHMS,7);//�ɼ��ɹ�ʱ��
					MR((u32)p8Data+22,(u32)p8rx+22,7);//�ɼ��洢ʱ��
					
					NUM_RCSD=1;
					NUM_RCSDrx=p8rx[45+NUM_ADDR*8];
					
					p8Data[29]=1;
					p8Data[30]=1;
					p8Data[31]=0x5B;
					p8Data[32]=1;
					MR((u32)p8Data+33,(u32)p8rx+41+NUM_ADDR*8,5+NUM_RCSDrx*4);
					MR((u32)UARTCtrl->RMCSD,(u32)p8Data+31,7+NUM_RCSDrx*4);					
					if(p8rx[41+5+NUM_RCSDrx*4+NUM_ADDR*8]==1)//��Ӧ����choice��[1]A-SimplifyRecord
					{
						if(p8rx[38]==1)
						{
							UARTCtrl->res23=3;//1������������
						}
						else
						{
							UARTCtrl->res23=8;//����������������
						}
					}
					else if(p8rx[41+5+NUM_RCSDrx*4+NUM_ADDR*8]==0xc8)//DAR=200��ʾ�޺���֡
					{
						if(p8rx[38]==1)
						{
							UARTCtrl->res23=4;//1������������
						}
						else
						{
							UARTCtrl->res23=9;//����������������
						}
					}
					
					LEN_Data0=0;//��ʾ���յ������ݵĳ���					
					NUM_AddrData=p8rx[47+NUM_RCSDrx*4+NUM_RCSDrx+NUM_ADDR*8+LEN_Data0];
					NUM_AddrData=(NUM_AddrData/NUM_ADDR);
					for(i=0;i<NUM_ADDR;i++)
					{
						MR((u32)p8Data,(u32)p8RMADDR+i*6,6);
						//LEN_Data1=30+NUM_RCSDrx*4+NUM_ADDR*8;//��ʾ�洢���ݵĳ���				
						z=NUM_RCSDrx;
						for(j=0;j<NUM_AddrData;j++)
						{
							LEN_Data1=30+NUM_RCSDrx*4+8;//��ʾ�洢���ݵĳ���
							for(x=0;x<NUM_RCSDrx;x++)//ROAD��OAD������
							{
								OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
								OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
								OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
								OAD|=UARTCtrl->RMCSD[10+(4*x)];
								if(OAD==0x20210200)
								{
									z=x;
									break;
								}
							}
							MR((u32)p8Data+22,(u32)p8rx+22,7);//�ɼ��洢ʱ��
							y=j*p8rx[38]*60;
							if(x==NUM_RCSDrx)
							{
								pYYMDHMS_AddS_hex(p8Data+22,y);//�ɼ��洢ʱ��
							}
							for(x=0;x<NUM_RCSDrx;x++)
							{
								y=p8rx[42+5+NUM_RCSDrx*4+x+NUM_ADDR*8];
								p8Data[LEN_Data1]=y;
								y=Get_DL698DataLen_S(p8rx+47+NUM_RCSDrx*4+x+NUM_ADDR*8,0);
								m=Deal_DataNull_JS(p8Data[LEN_Data1],p8rx+42+5+NUM_RCSDrx*4+NUM_ADDR*8+NUM_RCSDrx+LEN_Data0+1);
								if(m==1)
								{
									p8Data[LEN_Data1]=0;
									LEN_Data0+=(y-1);
									LEN_Data1+=1;
								}
								else
								{
									MR((u32)p8Data+LEN_Data1+1,(u32)p8rx+42+5+NUM_RCSDrx*4+NUM_ADDR*8+NUM_RCSDrx+LEN_Data0+1,y-1);
									if(z!=NUM_RCSDrx)//����ʱ��洢�ɲɼ��洢ʱ��
									{
										MR((u32)p8Data+22,(u32)p8rx+42+5+NUM_ADDR*8+NUM_RCSDrx*4+NUM_RCSDrx+LEN_Data0+1,y-1);
										z=NUM_RCSDrx;
									}
									LEN_Data0+=(y-1);
									LEN_Data1+=y;
								}
							}
							p8Data[6]=(LEN_Data1-8)&0xff;
							p8Data[7]=(LEN_Data1-8)>>8;
							RM_RecordFile(PORTn,p8Data);
							iRes=UARTCtrl->res23;
							switch(iRes)
							{
								case 3:
								case 4:
									UARTCtrl->res23=12;
									RM_RecordFile(PORTn,p8Data);
									UARTCtrl->res23=iRes;
									break;
								case 8:
								case 9:
									UARTCtrl->res23=13;
									RM_RecordFile(PORTn,p8Data);
									UARTCtrl->res23=iRes;
									break;
								default:
									return 0;	
							}
						
						}
					}
					return 0;
				}
				break;
			case 2://�ϱ����ɸ���¼�Ͷ�������
				//UARTCtrl->res23=2;
				if(p8rx[22]!=1)
					return 1;
				MR((u32)p8Data,(u32)p8rx+5,6);
				Data_Inverse((u32)p8Data,6);
				if((p8rx[24]==0x50)&&(p8rx[25]==0x04))
				{
					UARTCtrl->res23=1;//�к���֡���ն���
					i=p8rx[28];
					if(p8rx[i*4+28+1]==0)
					{
						UARTCtrl->res23=6;//�޺���֡���ն���
					}
				}
				else if((p8rx[24]==0x50)&&(p8rx[25]==0x06))
				{
					UARTCtrl->res23=2;//�к���֡���¶���
					i=p8rx[28];
					if(p8rx[i*4+28+1]==0)
					{
						UARTCtrl->res23=7;//�޺���֡���¶���
					}
				}
				else
					return 1;	
				
				NUM_RCSD=p8rx[23];
				p8Data[29]=1;
				p8Data[30]=1;
				p8Data[31]=0x5B;
				p8Data[32]=1;
				NUM_RCSDrx=p8rx[28];
				MR((u32)p8Data+33,(u32)p8rx+24,5+NUM_RCSDrx*4);
				MR((u32)p8Data+8,ADDR_DL698YMDHMS,7);//�ɼ�����ʱ��
				MR((u32)p8Data+15,ADDR_DL698YMDHMS,7);//�ɼ��ɹ�ʱ��
				MR((u32)p8Data+22,ADDR_DL698YMDHMS,7);//�ɼ��ɹ�ʱ��

				UARTCtrl->RMCSD[1]=p8rx[23];
				if(UARTCtrl->RMCSD[1]==1)
				{//ROAD
					MR((u32)UARTCtrl->RMCSD,(u32)p8Data+31,7+NUM_RCSDrx*4);
				}	
				NUM_RecordRow=p8rx[NUM_RCSDrx*4+28+2];
				p8=p8rx+24+(4*NUM_RCSDrx)+7;
				LEN_Data1=0;
				//p8+=2;
				while(NUM_RecordRow--)//��¼����
				{
					y=p8[1];
					LEN_Data1=38+NUM_RCSDrx*4;//��ʾ�洢���ݵĳ���
					LEN_Data0=2;
					for(x=0;x<y;x++)//ROAD��OAD������
					{
						OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
						OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
						OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
						OAD|=UARTCtrl->RMCSD[10+(4*x)];
						if(OAD==0x20210200)
						{
							i=Get_DL698DataLen_S(p8+LEN_Data0,0);
							MR((u32)p8Data+22,(u32)p8+1+LEN_Data0,7);
							MR((u32)p8Data+LEN_Data1,(u32)p8+LEN_Data0,8);
							pYYMDHMS_SubS_hex(p8Data+22,60);
							pYYMDHMS_SubS_hex(p8Data+LEN_Data1+1,60);
							LEN_Data0+=i;
							LEN_Data1+=i;
							continue;
						}
						if(OAD==0x10100200)
						{
							if((UARTCtrl->res23==1)||(UARTCtrl->res23==6))
								UARTCtrl->res23=10;
							else if((UARTCtrl->res23==2)||(UARTCtrl->res23==7))
								UARTCtrl->res23=11;
						}
						i=Get_DL698DataLen_S(p8+LEN_Data0,0);
						MR((u32)p8Data+LEN_Data1,(u32)p8+LEN_Data0,i);
						LEN_Data1+=i;
						LEN_Data0+=i;
					}
					p8Data[6]=(LEN_Data1-8)&0xff;
					p8Data[7]=(LEN_Data1-8)>>8;
					RM_RecordFile(PORTn,p8Data);
					p8+=LEN_Data0;
				}			
				return 0;
		}
	}
	else if(i==0x87)
	{
		if(p8rx[21]==0x06)//�ɼ�����������Ӧ
		{
			UARTCtrl->res23=5;
			return 0;
		}
		if(p8rx[21]==0)
		{
			UARTCtrl->res23=5;
			return 0;
		}
	}
#endif
	
	
//	p8Array=(u8*)p8Data;
//	LEN_Data0=p16Data[0];
//	if(LEN_RxDataBuff<LEN_Data0)
//	{//��
//		return 1;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
//	}
//	LEN_RxDataBuff-=LEN_Data0;
	
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ��峤��
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8Data+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//��
		return 3;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data+=LEN_RM_DI_CSD+4+LEN_Data1;
	
	//������(������)
	if(UARTCtrl->RM_Rx_date_NULL!=0)//������շ���:0=������,!=0������(������)
	{
		if(UARTCtrl->RMCSD[1]==0)
		{//OAD
			LEN_Rx=1;
		}
		else
		{//ROAD
			LEN_Rx=UARTCtrl->RMCSD[6];
		}	
		if(LEN_RxDataBuff<LEN_Rx)
		{//���峤�Ȳ���
			return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		for(i=0;i<LEN_Rx;i++)
		{
			p8Data[i]=0;
		}
		p16Data[0]=LEN_Rx;
		return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	
	LEN_Rx=p8rx[1]+(p8rx[2]<<8);
	#if(USER/100)==15//����
	if(LEN_Rx<(13+8+3))
	{//����<x
		return 0;
	}
	if(LEN_Rx==(13+8+3))
	{//����==x ������
		if(UARTCtrl->RMCSD[1]==0)
		{//OAD
			LEN_Rx=1;
		}
		else
		{//ROAD
			LEN_Rx=UARTCtrl->RMCSD[6];
		}	
		if(LEN_RxDataBuff<LEN_Rx)
		{//���峤�Ȳ���
			return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		for(i=0;i<LEN_Rx;i++)
		{
			p8Data[i]=0;
		}
		p16Data[0]=LEN_Rx;
		return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������

	}
	#else
	if(LEN_Rx<=(13+8+3))
	{//����<x
		return 0;
	}
	#endif
	
	i=p8rx[15];
	switch(i)
	{
		default:
			if(UARTCtrl->RMCSD[1]==0)
			{//OAD
				LEN_Rx=1;
			}
			else
			{//ROAD
				LEN_Rx=UARTCtrl->RMCSD[6];
			}	
			if(LEN_RxDataBuff<LEN_Rx)
			{//���峤�Ȳ���
				return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			for(i=0;i<LEN_Rx;i++)
			{
				p8Data[i]=0;
			}
			p16Data[0]=LEN_Rx;
			return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		case 1://GetRequestNormal
			LEN_Rx=Get_DL698DataLen_S(p8rx+22,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if(LEN_RxDataBuff<LEN_Rx)
			{//���峤�Ȳ���
				return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			p8rx+=22;
			if(UARTCtrl->RM_Rx_date_40020200[0]==1)//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
			{
//68 23 00 C3 05 01 00 00 00 00 00 00 EA EF 85 01 01 40 02 02 00 01 09 08 00 00 00 00 00 00 00 00 00 00 66 5F 16
				MC(0,((u32)&UARTCtrl->RM_Rx_date_40020200)+2,8);
				i=p8rx[1];//��ų���
				if(i>8)
				{
					i=8;
				}
				UARTCtrl->RM_Rx_date_40020200[1]=i;
				x=8-i;
				MR(((u32)&UARTCtrl->RM_Rx_date_40020200)+2+x,(u32)p8rx+2,i);
				UARTCtrl->RM_Rx_date_40020200[0]=2;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
				UARTCtrl->RMCSDSubCount--;//����CSD�Ӽ���
				return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			for(i=0;i<LEN_Rx;i++)
			{
				p8Data[i]=p8rx[i];
			}
			p16Data[0]=LEN_Rx;
			
			//�������ϱ�
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{
				//���ɸ��������Լ�������        [1]  SEQUENCE OF A-ResultNormal��
				//���ɸ���¼�Ͷ������Լ�������  [2]  SEQUENCE OF A-ResultRecord
				p8rx+=LEN_Rx;
				if(p8rx[0]!=0)
				{//�и����ϱ�
				#if EnMeterAutoReportEvent==1//������ܱ������ϱ��¼�:0=������,1=����
					if(UARTCtrl->AutoReportTask==0)//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
					{
						UARTCtrl->AutoReportTask=1;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
						UARTCtrl->AutoReportRMprotocol=3;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
						MW((u32)&UARTCtrl->RMAddr,(u32)&UARTCtrl->AutoReportRMAddr,6);
					}
				#endif
				}
			}
			break;
//		#if (USER/100)==17//����ϵ
//			case 2://GetResponseNormalList-----���ճ��������ݲ���GetResponseNormalList��ʽӦ��
//				ROAD=UARTCtrl->RMCSD[1];
//				if(ROAD==1)
//				{//ROAD
//					ROAD=UARTCtrl->RMCSD[2+0]<<24;
//					ROAD|=UARTCtrl->RMCSD[2+1]<<16;
//					ROAD|=UARTCtrl->RMCSD[2+2]<<8;
//					ROAD|=UARTCtrl->RMCSD[2+3];
//				}
//				NUM_RCSD=UARTCtrl->RMCSD[6];
//				NUM_RCSDrx=p8rx[17];//RCSD��SEQUENCE OF����
//				p16Data[0]=0;
//				for(x=0;x<NUM_RCSD;x++)
//				{
//					OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
//					OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
//					OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
//					OAD|=UARTCtrl->RMCSD[10+(4*x)];
//					LEN_Data0=0;
//					for(y=0;y<NUM_RCSDrx;y++)
//					{
//							OADrx=p8rx[18+LEN_Data0]<<24;
//							OADrx|=p8rx[18+LEN_Data0+1]<<16;
//							OADrx|=p8rx[18+LEN_Data0+2]<<8;
//							OADrx|=p8rx[18+LEN_Data0+3];
//							if(OAD==OADrx)
//							{
//								break;
//							}
//							i=p8rx[18+LEN_Data0+4];
//							if(i==0)
//								LEN_Data0+=2;
//							else
//							{
//								i=Get_DL698DataLen_S(p8rx+18+LEN_Data0+5,0);
//								LEN_Data0+=i;
//							}
//								
//					}
//					if(y>=NUM_RCSDrx)
//					{//û�ҵ���ѡ��OAD
//						if(LEN_RxDataBuff<1)
//						{//���峤�Ȳ���
//							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
//						}
//						p8Data[0]=0;
//						p8Data+=1;
//						LEN_RxDataBuff-=1;
//						p16Data[0]+=1;
//					}
//					else
//					{
//						i=p8rx[18+LEN_Data0+4];
//						if(i==0)
//							LEN_Data0+=2;
//						else
//						{
//							i=Get_DL698DataLen_S(p8rx+18+LEN_Data0+5,0);
//							MW((u32)p8rx+18+LEN_Data0+5,(u32)p8Data,i);
//							p8Data+=i;
//							LEN_RxDataBuff-=i;
//							p16Data[0]+=i;
//						}
//					}
//				}
//				//�������ϱ�
//				if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
//				{
//					p8=p8rx+18+LEN_Data0;
//					while(NUM_RCSDrx--)
//					{
//						i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
//						p8+=i;
//					}
//					if(p8[0]!=0)
//					{//�и����ϱ�
//					#if EnMeterAutoReportEvent==1//������ܱ������ϱ��¼�:0=������,1=����
//						if(UARTCtrl->AutoReportTask==0)//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
//						{
//							UARTCtrl->AutoReportTask=1;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
//							UARTCtrl->AutoReportRMprotocol=3;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
//							MW((u32)&UARTCtrl->RMAddr,(u32)&UARTCtrl->AutoReportRMAddr,6);
//						}
//					#endif
//					}
//				}
//				break;
//		#endif
		case 2://GetRequestNormalList

			break;
		case 3://GeResponseRecord
		#if CompareMeterEventCount==1//ȫ�¼��ɼ������Ƿ�ȽϷ�������:0=���Ƚ�(ȫ����������д���ļ�),1=�Ƚ�(ֻ�д����仯���¼�д���ļ�)
			i=UARTCtrl->TaskID;
			if(((i>>24)&0xf)==2)
			{//�¼��ɼ�����
				i=Get_RxDATA_DL698_MeterEventCount(PORTn);//��698���¼���¼����֡��ȡ�����ܱ��¼���������(�¼���¼���);����:��������,û�ҵ�ʱ����0xffffffff
				if(i==0xffffffff)
				{
					return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				OAD=UARTCtrl->RMCSD[2+0]<<24;
				OAD|=UARTCtrl->RMCSD[2+1]<<16;
				OAD|=UARTCtrl->RMCSD[2+2]<<8;
				OAD|=UARTCtrl->RMCSD[2+3];
				x=UARTCtrl->TaskID;
				x>>=8;
				x&=0xff;
				x=UARTCtrl->MeterCount[x];//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
				i=CompSourMeterEventCount(x,OAD,i);//�Ƚ�ԭ���ܱ��¼�����;���:SetNo=�������ú�(0��ʼ),Count=��ǰ����;����:0=��ʼֵ,1=������ͬ,2=������ͬ,ͬʱ�޸�ԭ����,ʹ��ADDR_DATABUFF
				if(i!=2)
				{
					return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
			}
		#endif
			ROAD=UARTCtrl->RMCSD[1];
			if(ROAD==1)
			{//ROAD
				ROAD=UARTCtrl->RMCSD[2+0]<<24;
				ROAD|=UARTCtrl->RMCSD[2+1]<<16;
				ROAD|=UARTCtrl->RMCSD[2+2]<<8;
				ROAD|=UARTCtrl->RMCSD[2+3];
			}
			NUM_RCSD=UARTCtrl->RMCSD[6];
			NUM_RCSDrx=p8rx[21];//RCSD��SEQUENCE OF����
			if(NUM_RCSD==0)
			{//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ����
				//Ϊ������㽫���յ�CSD������Ϊ����ѡ���CSD
				x=NUM_RCSDrx*4;
				if((x+7)<sizeof(UARTCtrl->RMCSD))
				{
					UARTCtrl->RMCSD[6]=NUM_RCSDrx;
					for(y=0;y<NUM_RCSDrx;y++)
					{
						UARTCtrl->RMCSD[7+(4*y)]=p8rx[22+(y*5)+1];
					}
				}
			}
			p16Data[0]=0;
			for(x=0;x<NUM_RCSD;x++)
			{
				OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
				OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
				OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
				OAD|=UARTCtrl->RMCSD[10+(4*x)];
				for(y=0;y<NUM_RCSDrx;y++)
				{
					if(p8rx[22+(y*5)+0]==0)
					{
						OADrx=p8rx[22+(y*5)+1]<<24;
						OADrx|=p8rx[22+(y*5)+2]<<16;
						OADrx|=p8rx[22+(y*5)+3]<<8;
						OADrx|=p8rx[22+(y*5)+4];
						if(OAD==OADrx)
						{
							break;
						}
					}
				}
				if(y>=NUM_RCSDrx)
				{//û�ҵ���ѡ��OAD
					if(LEN_RxDataBuff<1)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data[0]=0;
					p8Data+=1;
					LEN_RxDataBuff-=1;
					p16Data[0]+=1;
				}
				else
				{//�ҵ���ѡ��OAD
					p8=p8rx+22+(5*NUM_RCSDrx)+2;
					while(y--)
					{
						i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						p8+=i;
					}
					y=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
//				#if CompareTime_5004==1//�����ն����Ƿ�Ƚ϶���ʱ��:0=���Ƚ�,1=�Ƚ�
//						if(ROAD==0x50040200)
//						{//�ն���
//							if(OAD==0x20210200)
//							{//���ݶ���ʱ��
//								for(i=0;i<4;i++)
//								{
//									if(p8[1+i]!=Comm_Ram->DL698YMDHMS[i])
//									{
//										break;
//									}
//								}
//								if(i<4)
//								{//�����ղ�ͬ
//									p16Data[0]=0;
//									return 0;
//								}
//							}
//						}
//				#endif

				//#if (USER==901)//���ϰ�
					if(ROAD==0x50020200)
					{//���Ӷ���(����)
						switch(OAD)
						{
							//�������ǡ�00100201���͡�00200201������վ�á�00100200���͡�00200200���ٶ������ʵ��������ǿգ�ֻ���ܵ������ؾ�����
							case 0x00100201://�����й�������
							case 0x00200201://�����й�������
							case 0x00300201://����޹�1������
							case 0x00400201://����޹�2������
							case 0x00500201://��1�����޹�������
							case 0x00600201://��2�����޹�������
							case 0x00700201://��3�����޹�������
							case 0x00800201://��4�����޹�������
								UARTCtrl->RMCSD[10+(4*x)]=0;
								if(LEN_RxDataBuff<2)
								{//���峤�Ȳ���
									return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Data[0]=DataType_array;
								p8Data[1]=5;
								p8Data+=2;
								LEN_RxDataBuff-=2;
								p16Data[0]+=2;
								
								if(LEN_RxDataBuff<(y+4))
								{//���峤�Ȳ���
									return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								if(y<=1)
								{//������
									if(LEN_RxDataBuff<5)
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data[0]=0;
									y=1;
								}
								else
								{
									for(i=0;i<y;i++)
									{
										p8Data[i]=p8[i];
									}
								}
								p8Data[y]=0;//����1
								p8Data[y+1]=0;//����2
								p8Data[y+2]=0;//����3
								p8Data[y+3]=0;//����4
								p8Data+=y+4;
								LEN_RxDataBuff-=y+4;
								p16Data[0]+=y+4;
								continue;
							case 0x20000201://A���ѹ
							case 0x20010201://A�����
								UARTCtrl->RMCSD[10+(4*x)]=0;
								if(LEN_RxDataBuff<2)
								{//���峤�Ȳ���
									return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Data[0]=DataType_array;
								p8Data[1]=3;
								p8Data+=2;
								LEN_RxDataBuff-=2;
								p16Data[0]+=2;
								
								if(LEN_RxDataBuff<(y+2))
								{//���峤�Ȳ���
									return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								if(y<=1)
								{//������
									if(LEN_RxDataBuff<3)
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data[0]=0;
									y=1;
								}
								else
								{
									for(i=0;i<y;i++)
									{
										p8Data[i]=p8[i];
									}
								}
								p8Data[y]=0;//B���
								p8Data[y+1]=0;//C���
								p8Data+=y+2;
								LEN_RxDataBuff-=y+2;
								p16Data[0]+=y+2;
								continue;
							case 0x20040202://A���й�����
							case 0x20050202://A���޹�����
							case 0x200A0202://A�๦������
								UARTCtrl->RMCSD[10+(4*x)]=0;
								if(LEN_RxDataBuff<2)
								{//���峤�Ȳ���
									return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Data[0]=DataType_array;
								p8Data[1]=1;
								p8Data+=2;
								LEN_RxDataBuff-=2;
								p16Data[0]+=2;
								
								if(LEN_RxDataBuff<y)
								{//���峤�Ȳ���
									return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								if(y<=1)
								{//������
									if(LEN_RxDataBuff<1)
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data[0]=0;
									y=1;
								}
								else
								{
									for(i=0;i<y;i++)
									{
										p8Data[i]=p8[i];
									}
								}
								p8Data+=y;
								LEN_RxDataBuff-=y;
								p16Data[0]+=y;
								continue;
						}
					}
				//#endif
					if(y<=1)
					{//������
						if(LEN_RxDataBuff<1)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_RxDataBuff-=1;
						p16Data[0]+=1;
						continue;
					}
					if(LEN_RxDataBuff<y)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					for(i=0;i<y;i++)
					{
						p8Data[i]=p8[i];
					}
					p8Data+=y;
					LEN_RxDataBuff-=y;
					p16Data[0]+=y;
				}
			}
			
			//�������ϱ�
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{
				p8=p8rx+22+(5*NUM_RCSDrx)+2;
				while(NUM_RCSDrx--)
				{
					i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					p8+=i;
				}
				if(p8[0]!=0)
				{//�и����ϱ�
				#if EnMeterAutoReportEvent==1//������ܱ������ϱ��¼�:0=������,1=����
					if(UARTCtrl->AutoReportTask==0)//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
					{
						UARTCtrl->AutoReportTask=1;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
						UARTCtrl->AutoReportRMprotocol=3;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
						MW((u32)&UARTCtrl->RMAddr,(u32)&UARTCtrl->AutoReportRMAddr,6);
					}
				#endif
				}
			}
			break;
//		case 4://GetRequestRecordList
//			
//			break;
//		case 5://GetRequestNext
//			
//			break;
	}
	return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
}























