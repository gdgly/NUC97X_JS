
//����ת��
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../Device/IC_BUZZ.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"




void Forwarding(u32 PORTn);//����ת���������
void Uart_AFN10(u32 PORTn)//����ת��
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Fn;
	u32 DIaddr;
	u32 DIlen;
	u32 TXaddr;
	u32 TXlen;
	u32 LEN_UARTnTx;
	u64 d64a;
	u64 d64b;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u32 *p32;

	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<(12+16))
	{
		goto err;
	}
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+16+6))
		{
			goto err;
		}
		if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
		{
			i=((u32)p8rxbuff)+6+DIlen-6;
			i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
			if(i!=0x0)
			{
				return;//�����ñ���
			}
		}
	}
//PW��֤
	i=Uart_376_PW(PORTn);//��Ϣ��֤���ֶ�PW;����0��Ч,1��Ч,4�ѷ�����֡
	if(i==4)
	{
		return;
	}
	if(i!=0x0)
	{
err:
		//ȫ������
		p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
		Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		return;
	}

//������������ÿ���
//#if (USER/100)==5//�Ϻ�
//	if(PORTn==IRPORT)
//	{
//		if(Comm_Ram->SelfTestCommand!=0x55555555)//�Բ�������0x55555555����1(Ӳ��),0xAAAAAAAA����2(FLASH),0x5555AAAA����3(FLASH��������)
//		{
//			goto err;
//		}
//	}
//#endif

	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//�û������ݱ�ʶ�Ͳ������ݳ���
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		DIlen-=(8+16+6);
	}
	else
	{
		DIlen-=(8+16);
	}
	if(DIlen<=4)
	{
		goto err;
	}
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
	Fn=(i>>16);
	switch(Fn)
	{
		case 1://F1͸��ת��
			if(DIlen<(4+6))
			{//���ݵ�Ԫ��С6+1�ֽ�
				goto err;
			}
			i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//ת���ֽ���L
			if(i==0x0)
			{//ת���ֽ���L=0��
				goto err;
			}
//	if(p8rxbuff[18]==2)
//	{
//		p8rxbuff[18]=31;//��ģ��̨�����ز���ESAM�ڲ�������
//	}
			switch(p8rxbuff[18])
			{
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
				case 1://�˿ں�=1,�ڲ����������ӿ�
#endif
				case 2://�˿ں�=2,RS485-1
				case 3://�˿ں�=3,RS485-2
#if (((Project/100)==2)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
				case 4://�˿ں�=4,RS485-3
	#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
				case 31://�˿ں�=31,�ز�
	#endif			
#endif
					if(p8rxbuff[18]!=31)
					{//RS485��
						p16timer[0]=5000/10;//�ȴ�����ת���ڿ��ж�ʱ5S
					}
					else
					{//�ز���
						p16timer[0]=120000/10;//�ȴ�����ת���ڿ��ж�ʱ120S
					}
					UARTCtrl->Task=8;//8=�ȴ�ͨ������������ת��
					break;
				default:
					goto err;
			}
			break;
		case 9://F9ת����վֱ�ӶԵ��ĳ�����������
		case 10://F10ת����վֱ�ӶԵ�������������������
			switch(p8rxbuff[18])
			{
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
				case 1://�˿ں�=1,�ڲ����������ӿ�
#endif
				case 2://�˿ں�=2,RS485-1
				case 3://�˿ں�=3,RS485-2
#if ((Project/100)==5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
				case 4://�˿ں�=4,RS485-3
#endif
#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
				case 31://�˿ں�=31,�ز�
#endif			
					if(p8rxbuff[18]!=31)
					{//RS485��
						p16timer[0]=5000/10;//�ȴ�����ת���ڿ��ж�ʱ5S
					}
					else
					{//�ز���
						p16timer[0]=120000/10;//�ȴ�����ת���ڿ��ж�ʱ120S
					}
//----F9תΪF10��ͬ�����ʽ-----
					if(Fn==9)
					{
						i=p8rxbuff[19];//ת���м̼���n
						if(i==0xff)
						{
							i=0;
						}
						i*=6;//�м̼����͵�ַ�ֽڳ���
						x=MRR(((u32)p8rxbuff)+20+i+6+1,4);//ת��ֱ�ӳ��������ݱ�ʶ
						p8rxbuff[20+i+6+1]=1;//��ʶ����=1
						MWR(x,((u32)p8rxbuff)+20+i+6+2,4);//ת��ֱ�ӳ��������ݱ�ʶ
					}
//------------------------------			
					UARTCtrl->Task=8;//8=�ȴ�ͨ������������ת��
					break;
				default:
					goto err;
			}
			break;

		case 12://F12����������·���������
			z=p8rxbuff[18+2];//��������
			if(z>=MaxAFN10F12TaskType)//���ת��F12����������
			{//��������>=MaxAFN10F12TaskType
				goto err;
			}
			y=MRR(ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData,2);//ԭ�����õ������n
			i=(p8rxbuff[18+3]+(p8rxbuff[18+4]<<8));//�����������ݳ���Ln
			x=p8rxbuff[18+5+i]+(p8rxbuff[18+5+i+1]<<8);//�������õ������n
			TXlen=5+i+2+(x*40);//�������ܳ���
			if(DIlen!=(TXlen+4))
			{
				goto err;
			}
			DIaddr=(u32)p8rxbuff+18+5+i+2;//��1�����õ�ַ
			//ɾ�����������ڵ���ͬ���
			for(i=0;i<x;i++)
			{
				d64a=MRR(DIaddr+(i*40),8);
				for(Fn=0;Fn<x;Fn++)
				{
					d64b=MRR(DIaddr+(Fn*40),8);
					if(d64a==d64b)
					{//��ͬ��ַ
						if(i!=Fn)
						{//�����Լ�
							MR(DIaddr+(Fn*40),DIaddr+((Fn+1)*40),(x-Fn)*40);
							x--;//�������õ������-1
							Fn--;
						}
					}
				}
				p32=(u32 *)(ADDR_MS_ENTRY);
				(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
			}
			//ɾ����ԭ������ͬ���
			for(i=0;i<y;i++)
			{
				d64a=MRR(ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData+2+(i*40),8);
				for(Fn=0;Fn<x;Fn++)
				{
					d64b=MRR(DIaddr+(Fn*40),8);
					if(d64a==d64b)
					{//��ͬ��ַ
						MR(DIaddr+(Fn*40),DIaddr+((Fn+1)*40),(x-Fn)*40);
						x--;//�������õ������-1
						Fn--;
					}
				}
				p32=(u32 *)(ADDR_MS_ENTRY);
				(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
			}
			if((5+LEN_AFN10F12TaskData+2+((y+x)*40))>LEN_AFN10F12PerTask)
			{//�洢�ռ䲻��
				goto err;
			}
			//д�����ʽ	BIN	2
			//д��������	BIN	1
			//д�������ݳ���Ln	BIN	2
			//д��������	BIN	LEN_AFN10F12TaskData
			MW(((u32)p8rxbuff)+18,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask),5+LEN_AFN10F12TaskData);
			//д�������õĵ������
			MW(DIaddr,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData+2+(y*40),x*40);
			//д�������ܵ����
			MWR(y+x,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData,2);
			
			//ȫ��ȷ��
			BUZZ_Generate(2);//��������,��ڷ�����
			Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			break;
		case 13://F13��ѯ����������·������ִ��״̬
			z=p8rxbuff[18];//��ѯ����������
			if(z>=MaxAFN10F12TaskType)//���ת��F12����������
			{
				goto err;
			}
			y=MRR(ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData,2);//���õ������n
			TXaddr=(u32)p8txbuff+18;
			TXlen=4;
			MWR(y,TXaddr+1,2);//����ִ�жԵ��������е��������
			TXaddr+=5;
			TXlen+=5;
			Fn=0;//����ִ�жԵ����������ѳɹ��ĵ������n
			for(;UARTCtrl->MulFrameGPCR<y;UARTCtrl->MulFrameGPCR++)//��֡ʱͨ�ü����Ĵ���
			{
				if(MRR(ADDR_AFN10F13+(z*MaxRS485AddCarrierPn)+UARTCtrl->MulFrameGPCR,1)==1)
				{//0=û���,1=���,2=����
					if((TXlen+14+8+2+(8))>LEN_UARTnTx)
					{
						goto MulTx;//����-���к���֡
					}
					MR(TXaddr,ADDR_AFN10F12+(z*LEN_AFN10F12PerTask)+5+LEN_AFN10F12TaskData+2+(UARTCtrl->MulFrameGPCR*40),8);//���
					TXaddr+=8;
					TXlen+=8;
					Fn++;//����ִ�жԵ����������ѳɹ��ĵ������n
				}
			}
			MWR(Fn,(u32)p8txbuff+18+3,2);//�ѳɹ��ĵ������n
			
			TXlen+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
			p8txbuff[1]=TXlen;
			p8txbuff[2]=TXlen>>8;
			if(UARTCtrl->TxFrameCount!=0)//�Ĵ��֡����ʱ�ѷ���֡����
			{
				p8txbuff[13]&=0x9f;//����֡Ϊ��֡�м�֡TpV,FIR,FIN,CON;PSEQ|RSEQ
				p8txbuff[13]|=0x20;//����֡��Ϊ��֡����֡TpV,FIR,FIN,CON;PSEQ|RSEQ
			}
			UARTCtrl->TxFrameCount++;//�Ĵ��֡����ʱ�ѷ���֡����
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ,�ݵȴ���֡���ݷ���
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return;
		MulTx://����-���к���֡
			MWR(Fn,(u32)p8txbuff+18+3,2);//�ѳɹ��ĵ������n
			TXlen+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
			p8txbuff[1]=TXlen;
			p8txbuff[2]=TXlen>>8;
			p8txbuff[13]&=0x9f;//����֡Ϊ��֡�м�֡TpV,FIR,FIN,CON;PSEQ|RSEQ
			if(UARTCtrl->TxFrameCount==0)//�Ĵ��֡����ʱ�ѷ���֡����
			{
				p8txbuff[13]|=0x40;//����֡Ϊ��֡��ʼ֡TpV,FIR,FIN,CON;PSEQ|RSEQ
			}
			UARTCtrl->TxFrameCount++;//�Ĵ��֡����ʱ�ѷ���֡����
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ,�ݵȴ���֡���ݷ���
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			//����Ӧ֡���RSEQ+1
			i=p8rxbuff[13];
			x=i;
			i&=0xf0;
			x++;
			x&=0x0f;
			i|=x;
			p8rxbuff[13]=i;
			UARTCtrl->Task=5;//5=�ȴ���֡���ݷ���
			return;
		default:
			goto err;
	}
}
