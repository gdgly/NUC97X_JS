

//�û���ʶ:����ϵ
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_SETLIB.h"
#endif
#include "../terminal/Terminal_USER11xx.h"
#include "../terminal/Terminal_Uart_3761.h"
#include "../terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"



#if MainProtocol==376
#if (USER/100)==11//����ϵ

u32 USER11xx_RxAddr(u32 PORTn)//����ϵ����֡�ն˵�ַ�Ƚ�,4�ֽڳ��ȱ�Ϊ��׼֡��2�ֽ�;����:0=��ַ��ͬ,1=��ַ��ͬ
{
	u32 i;
	u32 x;
	u16 * p16;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	
	if(Comm_Ram->Factory==0x55)//0x55=����״̬
	{
		return 1;
	}
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);

//������������ͨ�ŵ�ַ
		i=MRR(((u32)p8rxbuff)+7,4);
		if(i==0xffffffff)
		{//�㲥��ַ
			//�޳�Ͷ��
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
				if(p8rxbuff[12+2]!=5)
				{//AFN!=5
					return 0;
				}
				if((p8rxbuff[14+2]+(p8rxbuff[15+2]<<8))!=0)
				{//Pn!=0
					return 0;
				}
				if((p8rxbuff[16+2]+(p8rxbuff[17+2]<<8))!=0x340)
				{//Fn!=31��ʱ����
					return 0;
				}
			}
			UARTCtrl->AddrType=3;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
			goto AddrjlOK;
		}
//������������ͨ�ŵ�ַ

	MR(ADDR_DATABUFF,ADDR_AREACODE,2);//������������
	MR(ADDR_DATABUFF+2,ADDR_04000401,4);//�����ַ��Ϊ�ն˵�ַ
	i=p8rxbuff[9]+(p8rxbuff[10]<<8);//��ַ��
	if(i==0xffff)
	{//�㲥��ַ
		if(p8rxbuff[12+2]!=15)//Ӧ�ò㹦����AFN
		{//�����ļ�����
			if((p8rxbuff[11+2]&0x1)==0x0)
			{//�������ַ��־��
				return 0;
			}
			//�޳�Ͷ��
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
				if(p8rxbuff[12+2]!=5)
				{//AFN!=5
					return 0;
				}
				if((p8rxbuff[14+2]+(p8rxbuff[15+2]<<8))!=0)
				{//Pn!=0
					return 0;
				}
				if((p8rxbuff[16+2]+(p8rxbuff[17+2]<<8))!=0x340)
				{//Fn!=31��ʱ����
					return 0;
				}
			}
			//����������Ƚ�
			if((p8rxbuff[7]+p8rxbuff[8])!=0x0)
			{//ע:����������=0ʱ�Զ�����Ϊ�㲥�����������ն˵�ַͨ������
				if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
				{//�����������
					return 0;
				}
			}
			UARTCtrl->AddrType=3;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
		}
	}
	else
	{//�ǹ㲥��ַ
		if((p8rxbuff[11+2]&0x1)==0x0)
		{//����ַ�Ƚ�
			//���ն�����������͵�ַ�Ƚ�
			if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,6)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			{//���Ǳ��ն�����������͵�ַ
				return 0;
			}
		}
		else
		{//��ַΪ���ַ
			//�޳�Ͷ��
			i=MRR(ADDR_AFN05F28,1);
			if(i==0x55)
			{//�޳�Ͷ��,ֻ�ж�ʱ���������Ч,����ȫ����Ч
				if(p8rxbuff[12+2]!=5)
				{//AFN!=5
					return 0;
				}
				if((p8rxbuff[14+2]+(p8rxbuff[15+2]<<8))!=0)
				{//Pn!=0
					return 0;
				}
				if((p8rxbuff[16+2]+(p8rxbuff[17+2]<<8))!=0x340)
				{//Fn!=31��ʱ����
					return 0;
				}
			}
			//����������Ƚ�
			if(DataComp(((u32)p8rxbuff)+7,ADDR_DATABUFF,2)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			{//�����������
				return 0;
			}
			p16=(u16 *)(ADDR_DATABUFF);
			p16[0]=p8rxbuff[9]+(p8rxbuff[10]<<8);//��ַ��
			if(p16[0]==0)
			{//���ַ=0Ϊ��Ч���ַ
				return 0;
			}
			MR(ADDR_DATABUFF+2,ADDR_AFN04F6,16);
			for(i=0;i<8;i++)
			{
				if(p16[0]==p16[1+i])
				{
					break;
				}
			}
			if(i==8)
			{//���ַ����ͬ
				return 0;
			}
			UARTCtrl->AddrType=2;//����֡�ĵ�ַ����:0=��,1=����ַ,2=���ַ,3=�㲥��ַ
		}
	}
	//�ƶ�����֡�ն˵�ַ6�ֽڳ���Ϊ��׼֡��4�ֽڳ���
AddrjlOK:	
	x=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(x<5)
	{
		return 0;
	}
	i=x-2;
	p8rxbuff[1]=i;
	p8rxbuff[2]=i>>8;
	x-=5;//���ƶ����ֽ���
	for(i=0;i<x;i++)
	{
		p8rxbuff[6+5+i]=p8rxbuff[6+5+2+i];
	}
	return 1;
}

//	0x68,
//	12,0,
//	12,0,
//	0x68,
//	0xc9,//������(DIR=1����,������־λPRM=1,������9=����M��Ӧ֡	��·����)
//	0,0,//����������
//	0,0,//�ն˵�ַ

//	0,//����վ��ַ(�ն���������֡��MSAӦΪ��)
//	2,//AFN=2
//	0x70,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ
//	0,0,//Pn=0
//	0x04,0,//Fn=1����,2�˳�,3����
//	0,//cs
//	0x16,


void USER11xx_TxAddr(u32 PORTn)//����ϵ����֡�ն˵�ַ2�ֽڳ��ȱ�Ϊ4�ֽ�,�����¼���֡У���,�跢�͵�֡�ֽ���+2
{
	u32 i;
	u32 x;
	u32 len;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8txbuff;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	if(Comm_Ram->Factory==0x55)//0x55=����״̬
	{
		return;
	}
	x=p8txbuff[1]+(p8txbuff[2]<<8);//�û�����������;ÿ֡�ַ���Ϊ�û����ݳ���L+8

	x>>=2;
	len=x;
	if(x<5)
	{
		return;
	}
	x-=5;//���ƶ����ֽ���
	for(i=0;i<x;i++)
	{
		p8txbuff[(5+len+2)-i]=p8txbuff[(5+len)-i];
	}
	MR(((u32)p8txbuff)+9,ADDR_04000401,4);//�����ַ��Ϊ�ն˵�ַ
	len+=2;
	p8txbuff[1]=len;
	p8txbuff[2]=len>>8;
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
	UARTCtrl->TxByte+=2;//�跢���ֽ���
}


#endif
#endif
