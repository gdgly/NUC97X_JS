
//uart��3����(�¼�)
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"






void Uart_AFN0E(u32 PORTn)//��3����(�¼�)
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
	u32 Addr;
  u32 LEN_UARTnTx;
  u64 d64a;
  u64 d64b;
	
	u8 * p8s;
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;

  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+6))
		{
err:
			//ȫ������
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//��Ӧ������ 9������
			Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//�û������ݱ�ʶ����
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	if(DIlen<6)
	{
		goto err;
	}
//����(����վ������11������2�����ݣ�����Ӧ�ò��������ݵ���·����)
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	RXaddr=Get_ADDR_UARTnRx(PORTn)+14+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=Get_ADDR_UARTnTx(PORTn)+14+4;
	TXlen=0;
	f=0;//������Ч���������־
	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	p8d=(u8 *)(TXDIaddr);
	p8d[0]=0;
	p8d[1]=0;
	p8d[2]=0;
	p8d[3]=0;
	d64a=MRR(ADDR_AFN0CF8,8);//F8�ն��¼���־״̬p0
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		if(i==0x0)//���ݱ�ʶ�ֽ�ΪFnPn
		{
//DIerr:
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if((TXlen+14+8+2)>LEN_UARTnTx)
				{
					goto err;
				}
				f=0;
			}
			CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
			p8d=(u8 *)(TXDIaddr);
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
			p8d[3]=0;
		}
		else
		{
			Fn=(i>>16);
			Pn=i&0xffff;
			if(Pn!=0x0)
			{
				DIlen-=2;
				RXaddr+=2;
				continue;	
			}
			switch(Fn)
			{
				case 1://������Ҫ�¼�
					Addr=ADDR_AFN0EF1;
					break;
				case 2://����һ���¼�
					Addr=ADDR_AFN0EF2;
					break;
				case 3://�����¼�
					p8s=(u8 *)(RXaddr);
					p8d=(u8 *)(TXaddr);
					i=p8s[0];//�����¼�����ERC
					i--;
					if(i>(MaxERC-1))
					{//ERC�����
						DIlen-=2;
						RXaddr+=2;
						continue;
					}
					Addr=ADDR_AFN0EF3+(i*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)));
					i=p8s[1];//��������������¼�����n
					if(i==0)
					{//������¼�����ERC�������¼�
						i=MaxClassEventNum;
					}
					x=MRR(Addr,2);//ȡ�Ѵ��¼���¼��
					Addr+=2;
					if(x>MaxClassEventNum)
					{
						x=MaxClassEventNum;
					}
					if(i>x)
					{
						i=x;
					}
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8d[0]=i;//��֡���Ĵ��͵��¼�����n
					x-=i;//��ʼ���ͼ�¼ָ��
					TXlen+=1;
					TXaddr+=1;
					for(y=0;y<i;y++)
					{
						f=MRR(Addr+(x*LEN_TERMINAL_EVENT)+1,1);//ȡ�¼�����
						f+=2;
						if((TXlen+4+14+8+2+f)>LEN_UARTnTx)
						{
							goto err;
						}
						MR(TXaddr,Addr+(x*LEN_TERMINAL_EVENT),f);
						TXlen+=f;
						TXaddr+=f;
						x++;
					}
					DIlen-=2;
					RXaddr+=2;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					f=1;
					continue;

				default:
					DIlen-=2;
					RXaddr+=2;
					continue;
			}
			MR(TXaddr,ADDR_AFN0CF7_FM,2);//�ȶ�2�ֽ�EC����
			p8s=(u8 *)(RXaddr);
			p8d=(u8 *)(TXaddr+2);
			p8d[0]=p8s[0];//��ʼָ��
			p8d[1]=p8d[0];//��ǰ����ָ��=��ʼָ��
			TXlen+=4;
			TXaddr+=4;
			while(p8d[1]!=p8s[1])
			{//��ǰ����ָ��!=�������ָ��
				x=Addr+(p8d[1]*LEN_TERMINAL_EVENT);
				//ȡERC��,�����Ѷ���־
				i=MRR(x,1);
				i-=1;
				if(i<=63)
				{//ERC�Ϸ�
					d64b=1;
					d64b<<=i;
					d64b=~d64b;
					d64a&=d64b;
				}
				//ȡ�¼�
				i=MRR(x+1,1);
				if(i==0x0)
				{//�¼���¼����Le=0,���¼�
					if(TXlen==4)
					{//���¼�����,ֻ��2�ֽ�EC����2�ֽ�ָ��
						p8d[0]+=1;//��ʼָ��+1
						p8d[1]=p8d[0];//��ǰ����ָ��=��ʼָ��
						continue;
					}
					else
					{
						break;
					}
				}
				i+=2;
				if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
				{
					goto err;
				}
				MR(TXaddr,x,i);
				TXlen+=i;
				TXaddr+=i;
				p8d[1]+=1;
			}
			
			DIlen-=2;
			RXaddr+=2;
			if(TXlen==4)
			{//���¼�����,ֻ��2�ֽ�EC����2�ֽ�ָ��
				TXlen-=4;
				TXaddr-=4;
				continue;
			}
			FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
			f=1;

			switch(Fn)
			{//�����ݺ���������ݵ�Fn
				case 1://������Ҫ�¼�
					Addr=ADDR_AFN0EF1;
					if(PORTn==GPRSPORT)
					{
						MWR(0,ADDR_ADCFlags,1);//��Ҫ�¼��ȴ�����0=û,1=��
					}
		#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
					if(PORTn==ENETPORT)
					{
						MWR(0,ADDR_ADCFlags+1,1);//��Ҫ�¼��ȴ�����0=û,1=��
					}
		#else
					if(PORTn==ENETPORT)
					{
						MWR(0,ADDR_ADCFlags,1);//��Ҫ�¼��ȴ�����0=û,1=��
					}
		#endif			
					if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
					{
						MWR(0,ADDR_ADCFlags+2,1);//��Ҫ�¼��ȴ�����0=û,1=��
					}
					break;
			}
		}
	}
	if(TXlen==0x0)
	{
		goto err;
	}
	MWR(d64a,ADDR_AFN0CF8,8);//F8�ն��¼���־״̬p0
	TXlen+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ

}










