
#include "Project.h"
#include "Terminal_RouterUpDate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../STM32F4xx/STM32F4xx_CRC.h"


#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void UpDateRouter(u32 PORTn)//·���������
{
	if(Comm_Ram->RouterType!=5)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
	{
		UpDateRouter_Std(PORTn);//��׼·���������
	}
	else
	{
		UpDateRouter_01(PORTn);//��˹��·���������
	}
}

void UpDateRouter_Std(u32 PORTn)//��׼·���������
{
/*
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 *p32;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32*)(ADDR_E4_ENTRY);
	
	switch(Terminal_Router->UpDateTask)//�����������
	{
		case 0://����
			i=p32[68];//�汾��Ϣ
			p32=(u32*)i;
			if(p32[5]!=Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
			{//·�������Ҵ�
				MC(0,ADDR_E4_ENTRY,4096);
				break;
			}
			if(UARTCtrl->Task<=2)
			{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				Terminal_Router->UpDateCount=0;//�������֡����
				Terminal_Router->UpDateReCount=0;//����������Դ���
				Terminal_Router->UpDateTask++;//�����������
				WarningOccur(PORTn,(u8 *)"���ڴ����ز�ģ�����");//�澯����,��ڸ澯����͸澯�ַ���
			}
			break;
		case 1://��ͣ·����
			Terminal_Router->StopFlags=(1<<16);//��ͣ·��:b16=·���������
			Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
			Terminal_Router->UpDateTask++;//�����������
			break;
		case 2://������
			if(p32[64]!=ADDR_E4_ENTRY)
			{
				Terminal_Router->UpDateTask=0;//�����������
				break;
			}
			Uart_3762_TxData(PORTn,0x15,1,0);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			z=Terminal_Router->MaxFileByte;//�ļ�����֧�ֵ���󵥸����ݰ�����
			if(z>(256-13-11-2))
			{
				z=(256-13-11-2);
			}
			p8txbuff[13]=3;//�ļ���ʶ	03H������ͨѶģ�������ļ�
			x=p32[63];
			y=p32[62];
			x-=y;
			y=(x+(z-1))/z;
			if((Terminal_Router->UpDateCount+1)<y)
			{
				p8txbuff[14]=0;//�ļ����ԣ���ʼ֡���м�֡Ϊ00H������֡Ϊ01H
			}
			else
			{
				p8txbuff[14]=1;//�ļ����ԣ���ʼ֡���м�֡Ϊ00H������֡Ϊ01H
			}
			p8txbuff[15]=0;//�ļ�ָ��	00H�����ķ�ʽ��װ
			p8txbuff[16]=y;//�ܶ���n	BIN	2
			p8txbuff[17]=y>>8;
			y=Terminal_Router->UpDateCount;//�������֡����
			MWR(y,((u32)p8txbuff)+18,4);//��i�α�ʶ��i=0~n��	BIN	4
			//�ļ�����	BIN	Lf
			y*=z;//�ѷ��ֽ���
			if(x>=y)
			{
				i=x-y;
			}
			else
			{
				i=0;
			}
			if(i>z)
			{
				i=z;
			}
			if(i==0x0)
			{//����
				UARTCtrl->Task=0;
				Terminal_Router->UpDateTask=8;//�����������
				break;
			}
			MWR(i,((u32)p8txbuff)+22,2);//��i�����ݳ���Lf	BIN	2
			MR(((u32)p8txbuff)+24,p32[62]+y,i);
			//��֡�ܳ���
			z=13+11+i+2;
			p8txbuff[1]=z;//���ֽ���
			p8txbuff[2]=z>>8;
			Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
			UARTCtrl->TxByte=z;//�����ֽ���(���ֶ���)
			UARTCtrl->Task=3;//�ȴ�����
			Terminal_Router->UpDateTask++;//�����������
			break;
		case 3://���ؽ���
			if(UARTCtrl->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				case 1://1=���1֡����
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1501)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//�����յ���ǰ�α�ʶ
						i=MRR(Get_ADDR_UARTnRx(PORTn)+13,4);
						if(i==Terminal_Router->UpDateCount)
						{//����鷢���ļ�����ʱ����0xffff
							Terminal_Router->UpDateReCount=0;//����������Դ���
							Terminal_Router->UpDateCount++;//֡��+1
							Terminal_Router->UpDateTask--;//�����������
							break;
						}
					}
				case 0://0=����
				case 2://2=���ճ�ʱ(�����)
					if(Terminal_Router->UpDateReCount<3)//����������Դ���
					{
						Terminal_Router->UpDateReCount++;//����������Դ���
						Terminal_Router->UpDateTask--;//�����������
					}
					else
					{//ʧ��
						WarningOccur(PORTn,(u8 *)"�ز�ģ���������ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
						Terminal_Ram->RouterCheck_S_Timer=5;//24 �ز�·���������붨ʱ��
						Terminal_Router->UpDateTask=9;//�����������
					}
					break;
			}
			break;
		case 8://���
			WarningOccur(PORTn,(u8 *)"�ز�ģ�����ڸ������");//�澯����,��ڸ澯����͸澯�ַ���
			Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxUpDateTime;//�����������ȴ�ʱ����
			Terminal_Router->UpDateTask++;//�����������
			break;
		case 9://��ʱ?S��λ
			if(Terminal_Ram->RouterCheck_S_Timer>60)//�����������ȴ�ʱ����
			{
				Terminal_Ram->RouterCheck_S_Timer=60;//�����������ȴ�ʱ����
			}
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 �ز�·���������붨ʱ��
			{
				MC(0,ADDR_E4_ENTRY,4096);
				Terminal_Router->UpDateTask=0;//�����������
				Terminal_Router->RouterInitTask=0;//·������ʼ������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
	}
*/
}


const u16 crc_ba[16] =
{
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
};

u32 UpDateRouter_01_CRC16(u32 Addr,u32 len)//��˹��·���������CRC16
{
	u8 da;
	u16 crc;
	u8 *ptr;
	
	crc=0xffff;
	ptr=(u8*)Addr;
	while(len--!=0)
	{
		da = ((u8)(crc/256))/16;
		crc <<= 4;
		crc ^= crc_ba[da^(*ptr/16)];
		da = ((u8)(crc/256)/16);
		crc <<= 4;
		crc ^= crc_ba[da^(*ptr&0x0f)];
		ptr++;
	}
	return (crc ^ 0xffff);
}


void UpDateRouter_01(u32 PORTn)//��˹��·���������
{
/*
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 *p32;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32*)(ADDR_E4_ENTRY);
	
	switch(Terminal_Router->UpDateTask)//�����������
	{
		case 0://����
			i=p32[68];//�汾��Ϣ
			p32=(u32*)i;
			if(p32[5]!=Comm_Ram->RouterType)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��
			{//·�������Ҵ�
				MC(0,ADDR_E4_ENTRY,4096);
				break;
			}
			if(UARTCtrl->Task<=2)
			{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				Terminal_Router->UpDateCount=0;//�������֡����
				Terminal_Router->UpDateReCount=0;//����������Դ���
				Terminal_Router->UpDateTask++;//�����������
				WarningOccur(PORTn,(u8 *)"���ڴ����ز�ģ�����");//�澯����,��ڸ澯����͸澯�ַ���
			}
			break;
		case 1://��ͣ·����
			Terminal_Router->StopFlags=(1<<16);//��ͣ·��:b16=·���������
			Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
			Terminal_Router->UpDateTask++;//�����������
			break;
		case 2://�������ļ���ʶΪ00H ���ļ�����֪֡ͨ����ͨ��ģ������洢���������ļ�
			Uart_3762_TxData(PORTn,0x15,1,0);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			p8txbuff[13]=0;//�ļ���ʶ	00H
			p8txbuff[14]=1;//�ļ����ԣ���ʼ֡���м�֡Ϊ00H������֡Ϊ01H
			p8txbuff[15]=0;//�ļ�ָ��	00H�����ķ�ʽ��װ
			p8txbuff[16]=1;//�ܶ���n	BIN	2
			p8txbuff[17]=0;
			MWR(0,((u32)p8txbuff)+18,4);//��i�α�ʶ��i=0~n��	BIN	4
			MWR(130,((u32)p8txbuff)+22,2);//��i�����ݳ���Lf	BIN	2
			
			z=UpDateRouter_01_CRC16(((u32)p8txbuff)+24,128);//��˹��·���������CRC16
			MWR(z,((u32)p8txbuff)+24+128,2);
			
			//��֡�ܳ���
			z=13+11+130+2;
			p8txbuff[1]=z;//���ֽ���
			p8txbuff[2]=z>>8;
			Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
			UARTCtrl->TxByte=z;//�����ֽ���(���ֶ���)
			UARTCtrl->Task=3;//�ȴ�����
			Terminal_Router->UpDateTask++;//�����������
			break;
		case 3:
			if(UARTCtrl->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				case 1://1=���1֡����
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1501)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//�����յ���ǰ�α�ʶ
						i=MRR(Get_ADDR_UARTnRx(PORTn)+13,4);
						if(i!=0xffff)
						{//����鷢���ļ�����ʱ����0xffff
							Terminal_Router->UpDateReCount=0;//����������Դ���
							Terminal_Router->UpDateTask++;//�����������
							break;
						}
					}
				case 0://0=����
				case 2://2=���ճ�ʱ(�����)
					if(Terminal_Router->UpDateReCount<3)//����������Դ���
					{
						Terminal_Router->UpDateReCount++;//����������Դ���
						Terminal_Router->UpDateTask--;//�����������
					}
					else
					{//ʧ��
						WarningOccur(PORTn,(u8 *)"�ز�ģ���������ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
						Terminal_Ram->RouterCheck_S_Timer=5;//24 �ز�·���������붨ʱ��
						Terminal_Router->UpDateTask=9;//�����������
					}
					break;
			}
			break;
		case 4://������
			if(p32[64]!=ADDR_E4_ENTRY)
			{
				Terminal_Router->UpDateTask=0;//�����������
				break;
			}
			Uart_3762_TxData(PORTn,0x15,1,0);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			z=128;//����ͨ��ģ�������ļ���ÿ֡�����ݳ���Lf �涨Ϊ128+2�ֽ�
			p8txbuff[13]=3;//�ļ���ʶ	03H������ͨѶģ�������ļ�
			x=p32[63];
			y=p32[62];
			x-=y;
			y=(x+(z-1))/z;
			if((Terminal_Router->UpDateCount+1)<y)
			{
				p8txbuff[14]=0;//�ļ����ԣ���ʼ֡���м�֡Ϊ00H������֡Ϊ01H
			}
			else
			{
				p8txbuff[14]=1;//�ļ����ԣ���ʼ֡���м�֡Ϊ00H������֡Ϊ01H
			}
			p8txbuff[15]=0;//�ļ�ָ��	00H�����ķ�ʽ��װ
			p8txbuff[16]=y;//�ܶ���n	BIN	2
			p8txbuff[17]=y>>8;
			y=Terminal_Router->UpDateCount;//�������֡����
			MWR(y,((u32)p8txbuff)+18,4);//��i�α�ʶ��i=0~n��	BIN	4
			//�ļ�����	BIN	Lf
			y*=z;//�ѷ��ֽ���
			if(x>=y)
			{
				i=x-y;
			}
			else
			{
				i=0;
			}
			if(i>z)
			{
				i=z;
			}
			if(i==0x0)
			{//����
				UARTCtrl->Task=0;
				Terminal_Router->UpDateTask=8;//�����������
				break;
			}
			MWR(130,((u32)p8txbuff)+22,2);//��i�����ݳ���Lf	BIN	2
			MR(((u32)p8txbuff)+24,p32[62]+y,i);
			
			z=UpDateRouter_01_CRC16(((u32)p8txbuff)+24,128);//��˹��·���������CRC16
			MWR(z,((u32)p8txbuff)+24+128,2);
			
			//��֡�ܳ���
			z=13+11+130+2;
			p8txbuff[1]=z;//���ֽ���
			p8txbuff[2]=z>>8;
			Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
			UARTCtrl->TxByte=z;//�����ֽ���(���ֶ���)
			UARTCtrl->Task=3;//�ȴ�����
			Terminal_Router->UpDateTask++;//�����������
			break;
		case 5://���ؽ���
			if(UARTCtrl->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				case 1://1=���1֡����
					if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x1501)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
					{//�����յ���ǰ�α�ʶ
						i=MRR(Get_ADDR_UARTnRx(PORTn)+13,4);
						if(i==Terminal_Router->UpDateCount)
						{//����鷢���ļ�����ʱ����0xffff
							Terminal_Router->UpDateReCount=0;//����������Դ���
							Terminal_Router->UpDateCount++;//֡��+1
							Terminal_Router->UpDateTask--;//�����������
							break;
						}
					}
				case 0://0=����
				case 2://2=���ճ�ʱ(�����)
					if(Terminal_Router->UpDateReCount<3)//����������Դ���
					{
						Terminal_Router->UpDateReCount++;//����������Դ���
						Terminal_Router->UpDateTask--;//�����������
					}
					else
					{//ʧ��
						WarningOccur(PORTn,(u8 *)"�ز�ģ���������ʧ��");//�澯����,��ڸ澯����͸澯�ַ���
						Terminal_Ram->RouterCheck_S_Timer=5;//24 �ز�·���������붨ʱ��
						Terminal_Router->UpDateTask=9;//�����������
					}
					break;
			}
			break;
		case 8://���
			WarningOccur(PORTn,(u8 *)"�ز�ģ�����ڸ������");//�澯����,��ڸ澯����͸澯�ַ���
			Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxUpDateTime;//�����������ȴ�ʱ����
			Terminal_Router->UpDateTask++;//�����������
			break;
		case 9://��ʱ?S��λ
			if(Terminal_Ram->RouterCheck_S_Timer>240)//�����������ȴ�ʱ����
			{
				Terminal_Ram->RouterCheck_S_Timer=240;//�����������ȴ�ʱ����
			}
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 �ز�·���������붨ʱ��
			{
				MC(0,ADDR_E4_ENTRY,4096);
				Terminal_Router->UpDateTask=0;//�����������
				Terminal_Router->RouterInitTask=0;//·������ʼ������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			}
			break;
	}
*/
}

#endif

//0x68
//�ܳ���L
//�ܳ���L
//������
//��Ϣ��R-1
//��Ϣ��R-2
//��Ϣ��R-3
//��Ϣ��R-4
//��Ϣ��R-5
//��Ϣ��R-6
//��ַ��A1,A2,A3
//Ӧ�ù�����AFN
//���ݵ�Ԫ��ʶDT1
//���ݵ�Ԫ��ʶDT2
//���ݵ�Ԫ
//CS
//0x16
