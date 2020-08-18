
//������������
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"


extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
//#define Class2Data_MaxFn	218//��2�������Fn
u32 Get_Class2Data_MaxFn(void);








void Uart_AFN0B(u32 PORTn)//������������
{
	u32 i;
	u32 x;
	u32 y;
	u32 t;
	u32 Fn;
	u32 Pn;
	u32 DIlen;
	u32 DIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 SourFn;
	u32 ALLPn;//ȫ����Ч�����־��0=��,1=��
  u32 LEN_UARTnTx;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	
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
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	TXaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXlen=0;
	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//�����ݱ�ʶΪȫ����Ч������
		ALLPn=0;//ȫ����Ч�����־��0=��,1=��
	}
	else
	{
		ALLPn=1;//ȫ����Ч�����־��0=��,1=��
	}
	SourFn=0;
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		if(i==0x0)//���ݱ�ʶ�ֽ�ΪFnPn
		{
			switch(SourFn)
			{
				case 1:
					DIlen-=4;
					DIaddr+=4;
					break;
				case 2:
					if(ALLPn==0x0)
					{//�����ݱ�ʶΪȫ����Ч������,����ʱ���ݱ�ʶ���������
						DIlen-=4;
						DIaddr+=4;
					}
					else
					{
						DIlen-=9;
						DIaddr+=9;
					}
					break;
				default:
					goto err;
			}
			CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼�����

			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//�����ݱ�ʶΪȫ����Ч������
				ALLPn=0;//ȫ����Ч�����־��0=��,1=��
			}
			else
			{
				ALLPn=1;//ȫ����Ч�����־��0=��,1=��
			}
			continue;
		}
		Fn=(i>>16);
		Pn=i&0xffff;
		SourFn=Fn;
		if(Pn==0x0)
		{
			goto err;
		}
		if(Pn>MaxTimerTask)
		{//ȫ��Pn��
			if(ALLPn!=0x0)
			{//�����ݱ�ʶΪȫ����Ч������,����ʱ���ݱ�ʶ���������
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			goto err;
		}
		Pn--;
		switch(Fn)
		{
			case 1://����ʱ�ϱ�1����������
				i=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Pn)+8,1);//
				i*=4;
				if((TXlen+32+i)>LEN_UARTnTx)
				{
					goto err;
				}
				MR(TXaddr,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Pn)+9,i);
				TXaddr+=i;
				TXlen+=i;
				break;
			case 2://����ʱ�ϱ�2����������
				if(DIlen<9)
				{
					goto err;
				}
				x=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+8,1);//
				for(i=0;i<x;i++)
				{
					y=BitLoca(MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i)+2,1));
					y+=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i)+3,1)*8;
					if((y>=Get_Class2Data_MaxFn())||(y==0x0))
					{//Fn��Чʱ��װ��,��ʱ�곤�Ȳ���֪
						if(ALLPn==0)//ȫ����Ч�����־��0=��,1=��
						{
							DIlen-=5;
							DIaddr+=5;
						}
						continue;
					}
					if((TXlen+32+4)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i),4);//ȡDI
					TXaddr+=4;
					TXlen+=4;
					switch(Terminal_Class2DataLib[y].CongealType)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
					{
						case 0://�ն���
							if((TXlen+32+3)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+2,TXaddr,3);
							TXaddr+=3;
							TXlen+=3;
							break;
						case 1://�����ն���
							if((TXlen+32+3)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+2,TXaddr,3);
							TXaddr+=3;
							TXlen+=3;
							break;
						case 2://�¶���(�ϱ���1��)
							if((TXlen+32+2)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+3,TXaddr,2);
							TXaddr+=2;
							TXlen+=2;
							break;
						case 3://����(�ϱ���1�����յ���)
/*
#define CurveCongealTime     15//���߶�����ʱ��(�ܶ�)0=������;15=15,30,45,0;30=30,0;60=0;5=5,10,15...0;1=1,2,3...0
���߳�ȡ���ݱ���R��ȡֵ��Χ1��96
�类��ȡ�����ݵĶ����ܶ�m=2����ÿ30���Ӷ���һ��ֵ����ô��R=2ʱ����ʾ��60���ӳ�ȡ��R=1ʱ����ʾ�԰�30���ӳ�ȡ
���ݶ����ܶ�m			
0	������	��
1	15	15�֡�30�֡�45�֡�0��
2	30	30�֡�0�֣�
3	60	0��
254	5	5�֡�10�֡�15��......��0��
255	1	1�֡�2�֡�3�֡�......0��
���ݵ���n
*/
							if((TXlen+32+7)>LEN_UARTnTx)
							{
								goto err;
							}
							MR(TXaddr,DIaddr+4,5);
							//���߶�����ʱ��*��ȡ���ݱ���
							t=CurveCongealTime*MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+7,1);
							switch(t)
							{
								case 15:
									y=1;//Ҫ��ȡ���ߵ������ܶ�m
									break;
								case 30:
									y=2;//Ҫ��ȡ���ߵ������ܶ�m
									break;
								case 60:
									y=3;//Ҫ��ȡ���ߵ������ܶ�m
									break;
								case 5:
									y=254;//Ҫ��ȡ���ߵ������ܶ�m
									break;
								case 1:
									y=255;//Ҫ��ȡ���ߵ������ܶ�m
									break;
								default:
									t=CurveCongealTime;
									switch(t)
									{
										case 15:
											y=1;//Ҫ��ȡ���ߵ������ܶ�m
											break;
										case 30:
											y=2;//Ҫ��ȡ���ߵ������ܶ�m
											break;
										case 60:
											y=3;//Ҫ��ȡ���ߵ������ܶ�m
											break;
										case 5:
											y=254;//Ҫ��ȡ���ߵ������ܶ�m
											break;
										case 1:
											y=255;//Ҫ��ȡ���ߵ������ܶ�m
											break;
									}
									break;
							}
							MC(y,TXaddr+5,1);//���ݶ����ܶ�m
							t=(24*60)/t;//��1�����
							MC(t,TXaddr+6,1);//���ݶ����ܶ�m
							TXaddr+=7;
							TXlen+=7;
							break;
					}
				}
				if(ALLPn==0)//ȫ����Ч�����־��0=��,1=��
				{
					DIlen-=5;
					DIaddr+=5;
				}
				break;
			default:
				goto err;
		}
	}
	if(TXlen==0)
	{
		goto err;
	}
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		i=((u32)p8rxbuff)+6+DIlen-6;
		MR(TXaddr,i,6);
		TXaddr+=6;
		TXlen+=6;
	}
	TXlen+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	TXlen+=8;
	i=Get_LEN_UARTnRx(PORTn);
	if(TXlen>i)
	{
		goto err;
	}
	MR((u32)p8rxbuff,(u32)p8txbuff,TXlen+8);
	if(Fn==1)
	{
		UARTCtrl->LockFlags|=0x40;//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7
		Uart_AFN0C(PORTn);//��1����
	}
	else
	{
		Uart_AFN0D(PORTn);//��2����
	}
}







