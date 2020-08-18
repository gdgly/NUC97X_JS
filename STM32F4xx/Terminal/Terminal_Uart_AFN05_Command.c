
//UART ��������
#include "Project.h"
#include "Terminal_Uart_AFN05_Command.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_Contrl_Remote.h"

#include "Terminal_Statistic.h"
#include "../Device/RTC.h"


extern TerminalCommandUartLib_TypeDef	TerminalCommandUartLib[];


void Uart_AFN05(u32 PORTn)//��������
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 f;
	
	u8 * p8s;
	u8 * p8d;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8* p8;

  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

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
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
//PW��֤
	i=Uart_376_PW(PORTn);//��Ϣ��֤���ֶ�PW;���PW��ַ,����0��Ч,1��Ч,4�ѷ�����֡
	if(i==4)
	{
		return;
	}
	if(i!=0x0)
	{
err:
		//ȫ������
		Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		return;
	}
/*
//������������ÿ���
	if(PORTn==IRPORT)
	{
    if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_MeterFMTimer,2)==0))
		{
			goto err;
		}
	}
*/
	
	f=0;//f=0���ȫ�������Ƿ�ȫ����Ч,f=1д��
loop:
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
			Pn=i&0xffff;
			p8s=(u8 *)(RXaddr);
			if(Fn==0x0)
			{
				goto err;
			}
		#if ((USER/100)==5)//�Ϻ�
			if(Fn==201)
			{
				goto AFN05F201;
			}
		#endif
			if(Fn>Get_AFN05CommandLib_MaxFn())
			{
				goto err;
			}
			if(Pn>TerminalCommandUartLib[Fn-1].PnMax)
			{
				goto err;
			}
			if(TerminalCommandUartLib[Fn-1].PnType!=0x0)
			{
				if(Pn==0x0)
				{
					goto err;
				}
			}
			if((TerminalCommandUartLib[Fn-1].project&(1<<(Project/100)))==0x0)
			{//�������޴�Fn����
				goto err;
			}
			switch(Fn)//Fn
			{
				case 31://��ʱ����
			#if ((USER/100)==5)//�Ϻ�
				AFN05F201:
			#endif
					if(f==0x0)
					{
					#if ((USER/100)==5)//�Ϻ�
						if(Fn==31)
						{
							if((DIlen-4)<6)
							{
								goto err;
							}
						}
						if(Fn==201)
						{
							if((DIlen-4)<8)
							{
								goto err;
							}
						}
					#else
						if((DIlen-4)<6)
						{
							goto err;
						}
					#endif
					}
					else
					{
						p8d=(u8 *)(ADDR_RTCBUFF);
						p8d[0]=p8s[0];
						p8d[1]=p8s[1];
						p8d[2]=p8s[2];
						p8d[3]=p8s[4]>>5;
						if(p8d[3]==7)
						{
							p8d[3]=0;
						}
						p8d[4]=p8s[3];
						p8d[5]=p8s[4]&0x1f;
						p8d[6]=p8s[5];
						if(Check_HMS(MRR((u32)p8d,3))!=0)//ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
						{
							goto err;
						}
						if(Check_YMD(MRR(((u32)p8d)+4,3))!=0)//�����պϷ����,����0�Ϸ�,1�Ƿ�
						{
							goto err;
						}
						
//            WriteLPCRTC(ADDR_RTCBUFF);//д�ڲ�RTC
//            SyncLPCRTC();//ͬ���ڲ�RTC(���ڲ�д�ⲿ)
						WriteRTC(ADDR_RTCBUFF);//д�ⲿӲʵʱ��
						
						Comm_Ram->MS_Task=0;//�²�����RTC
						Comm_Ram->ReadRTCTimer=0;//25 ��ERTCÿ10MS��1��ʱ��
						Terminal_Ram->I2SUBTask1=0;//�²�����ʵʱ����RTC
						
						//��ʱ�¼���¼
						MWR(0,ADDR_DATABUFF+2,2);//�������
						MR(ADDR_DATABUFF+4,ADDR_AFN0CF2,6);//��ʱǰʱ��
						MR(ADDR_DATABUFF+10,(u32)p8s,6);//��ʱ��ʱ��
						ERC41_Event(ADDR_DATABUFF);//��ʱ�¼���¼;���:ADDR_BUFF�ĵ�2�ֽڿ�ʼ�ֱ�Ϊ2BYTE�������,6BYTE��ʱǰʱ��,6BYTE��ʱ��ʱ��
						
						//��վ�·���ʱ������ն�ʱ���ն�Ӧ��������ʱ��εĵ�ѹͳ��Ϊ�ϸ񣻶�ʱ������ʱ���Ӧ������ʱ��
						MR(ADDR_DATABUFF,ADDR_DATABUFF+4,12);//��ʱǰʱ��,��ʱ��ʱ��
						p8d=(u8 *)(ADDR_DATABUFF);
						p8d[4]&=0x1f;//��ʱǰʱ��ȥ���ϵ�����
						p8d[6+4]&=0x1f;//��ʱ��ʱ��ȥ���ϵ�����
						x=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
						if((x>>31)==0)
						{//��ֵ
							i=MRR(ADDR_DATABUFF+3,3);
							i=YMD_Add1D(i);//��ʱǰʱ�������ռ�1��,���ؼ�1���������
							MC(0,ADDR_DATABUFF+12,3);
							MWR(i,ADDR_DATABUFF+15,3);
							i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+12);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
							if(x>i)
							{
								x=i;//�õ��յ�24���
							}
							x/=60;//���Ϊ��
							for(i=0;i<AFN0DF27MaxPn;i++)
							{
							//A���ѹ�ϸ����ۼ�ʱ��
								y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+8,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+8,2);
							//B���ѹ�ϸ����ۼ�ʱ��
								y=MRR(ADDR_AFN04F25+(i*11)+10,1);//ȡ��������������ĵ�Դ���߷�ʽ
								if(y!=1)
								{//������������
									y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+18,2);
									y+=x;
									if(y>0xffff)
									{
										y=0xffff;
									}
									MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+18,2);
								}
							//C���ѹ�ϸ����ۼ�ʱ��
								y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+28,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+28,2);
							}
							x=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
							i=MRR(ADDR_DATABUFF+4,2);
							i=YM_Add1M(i);//���¼�1��,���ؼ�1�������
							MC(0,ADDR_DATABUFF+12,3);
							MWR(1,ADDR_DATABUFF+15,1);
							MWR(i,ADDR_DATABUFF+16,2);
							i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+12);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
							if(x>i)
							{
								x=i;//�õ���δ(30��31��)24���
							}
							x/=60;//���Ϊ��
							for(i=0;i<AFN0DF27MaxPn;i++)
							{
							//A���ѹ�ϸ����ۼ�ʱ��
								y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+8,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+8,2);
							//B���ѹ�ϸ����ۼ�ʱ��
								y=MRR(ADDR_AFN04F25+(i*11)+10,1);//ȡ��������������ĵ�Դ���߷�ʽ
								if(y!=1)
								{//������������
									y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+18,2);
									y+=x;
									if(y>0xffff)
									{
										y=0xffff;
									}
									MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+18,2);
								}
							//C���ѹ�ϸ����ۼ�ʱ��
								y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+28,2);
								y+=x;
								if(y>0xffff)
								{
									y=0xffff;
								}
								MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+28,2);
							}
							for(i=0;i<AFN0DF27MaxPn;i++)
							{
								AFN0DF27F35_Computer();//�ն�ͳ��.��ѹͳ������
							}
						}
						
					}
				#if ((USER/100)==5)//�Ϻ�
					if(Fn==31)
					{
						RXaddr+=6;
						DIlen-=6;
					}
					if(Fn==201)
					{
						if(f==1)//f=0���ȫ�������Ƿ�ȫ����Ч,f=1д��
						{
							MW(RXaddr,ADDR_AFN05F201+1,8);
							i=0;
	 						//RS485-1
							if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_UART_PORT_SET+(1*LEN_UART_PORT_SET),1)!=0))//�ն˳���
							{
								i|=1;
							}
							//RS485-2
							if(MRR(ADDR_UART_PORT_SET+(2*LEN_UART_PORT_SET),1)!=0)//�ն˳���
							{
		 						i|=(1<<1);
							}
	
						#if ((Project/100))==2//������
							//�ز�
	 						i|=(1<<3);
						#endif
							MWR(i,ADDR_AFN05F201,1);
						}
						RXaddr+=8;
						DIlen-=8;
					}
				#else
					RXaddr+=6;
					DIlen-=6;
				#endif
					continue;
				case 32://������Ϣ
					if(f==0x0)
					{
						if((DIlen-4)<(p8s[1]+2))
						{
							goto err;
						}
					}
					else
					{
						MW(RXaddr,TerminalCommandUartLib[Fn-1].Addr+(((p8s[0]>>4)&0x1)*202*16)+((p8s[0]&0x0f)*202),p8s[1]+2);	
					}
					RXaddr+=p8s[1]+2;
					DIlen-=p8s[1]+2;
					continue;
				case 49://����ָ��ͨ�Ŷ˿���ͣ����
				case 50://����ָ��ͨ�Ŷ˿ڻָ�����
				case 51://����ָ��ͨ�Ŷ˿����³���
				case 52://��ʼ��ָ��ͨ�Ŷ˿��µ�ȫ���м�·����Ϣ
				case 53://ɾ��ָ��ͨ�Ŷ˿��µ�ȫ�����
					if(f==0x0)
					{
						if((DIlen-4)<1)
						{
							goto err;
						}
						if(p8s[0]>31)
						{
							goto err;
						}
						if(p8s[0]==0x0)
						{
							goto err;
						}
					}
					else
					{
						MC(TerminalCommandUartLib[Fn-1].Command,TerminalCommandUartLib[Fn-1].Addr+p8s[0]-1,1);
					}
					RXaddr+=1;
					DIlen-=1;
					continue;
				case 57://ɾ��ָ���¼�����ERC�µ�ȫ���¼�
					if(f==0x0)
					{
						if((DIlen-4)<1)
						{
							goto err;
						}
					}
					else
					{
						if(p8s[0]==0)
						{//n=0������������¼�����ERC ��Ӧ���¼�
							MC(0,ADDR_AFN0EF3,MaxERC*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT)));
						}
						else
						{
							for(i=0;i<p8s[0];i++)
							{
								x=p8s[1+i];//Ҫ��0��ERC����
								if((x==0)||(x>MaxERC))
								{
									continue;
								}
								x--;
								MC(0,ADDR_AFN0EF3+(x*(2+(MaxClassEventNum*LEN_TERMINAL_EVENT))),2+(MaxClassEventNum*LEN_TERMINAL_EVENT));
							}
						}
//----�ն��¼������������㣬���Ժ����������������ж�AFN0CF245---
						p8=(u8*)(ADDR_AFN0CF245);//�ն��¼�����
						x=0;//�ն˵�ǰ�¼���������n
						for(i=0;i<MaxERC;i++)
						{
							y=ADDR_AFN0EF3+(i*(2+(15*LEN_TERMINAL_EVENT)));
							y=MRR(y,2);
							if(y>15)
							{
								y=15;
							}
							if(y!=0)
							{
								p8[1+(2*x)+0]=i+1;//�¼�����ERC
								p8[1+(2*x)+1]=y;//�¼�����
								x++;//�ն˵�ǰ�¼���������n
							}
						}
						p8[0]=x;//����n
//---�ն��¼������������� END---
					}
					i=p8s[0];
					i++;
					RXaddr+=i;
					DIlen-=i;
					continue;
					
				default:
					i=TerminalCommandUartLib[Fn-1].Len;
					break;
			}
			if(TerminalCommandUartLib[Fn-1].Addr==0x0)
			{
				goto err;
			}
			if(TerminalCommandUartLib[Fn-1].Command!=0x0)
			{//���ǽ�������ж�����
				if((DIlen-4)<i)
				{
					goto err;
				}
			}
			if(f==0x1)
			{
				p8s=(u8 *)(RXaddr);
				p8d=(u8 *)(ADDR_DATABUFF);
				p8d[0]=TerminalCommandUartLib[Fn-1].Command;
				if(TerminalCommandUartLib[Fn-1].Command!=0x0)
				{//���ǽ������
					for(x=0;x<i;x++)
					{
						p8d[1+x]=p8s[x];
					}
				}
				else
				{//�ǽ������
					for(x=0;x<i;x++)
					{
						p8d[1+x]=0;
					}
				}
				if(Pn==0x0)
				{
					MW(ADDR_DATABUFF,TerminalCommandUartLib[Fn-1].Addr,i+1);
				}
				else
				{
					MW(ADDR_DATABUFF,TerminalCommandUartLib[Fn-1].Addr+((Pn-1)*(i+1)),i+1);
				}
				switch(Fn)
				{
					case 1://ң����բ(���Ժ����ʱ���������ֻ��1����ʱ����F2�����բ,�ն˿���ѭ����ֻ��1�벻�ܼ�ʱ����,���ڴ������м����ܼ�¼ң����բ����)
						for(x=0;x<4;x++)
						{
							Terminal_Remote_Control();//�ն˿���-ң����բ
						}
						break;
				#if (USER/100)==6//�û�:����
					case 103://��������������δ֪���(����ϵ��չ)
						x=MRR(ADDR_AFN05F103+3,1);
						if(x&1)//ִ�б�־��D0λ=1����ϴ��������,=0�����ϴ��������
						{
							MC(0,ADDR_AFN0CF13,2);//�������ĵ����Ϣ
						}
						//�޸�AFN05F149���Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
						if(x&2)//ִ�б�־��D1λ=1δ֪���ֱ�Ӽ���������,=0���Զ�����
						{
							MWR(0x555555550100,ADDR_AFN05F149,6);
							
						}
						else
						{
							MWR(0x555555550200,ADDR_AFN05F149,6);
						}
						break;
				#endif
					case 149://���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�(�����˿���ֱ���Խ���)
				#if (USER/100)==6//�û�:����
						MWR((12*60)<<8,ADDR_AFN05F103,3);//��F103������־,ͬʱ��������������ʱ��=12*60��
				#endif
						x=MRR(ADDR_AFN05F149+1,1);
						if((x==1)||(x==2))//�Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
						{
							x=0x55555555;
							MC(0,ADDR_AFN0CF13,2);//�������ĵ����Ϣ
						}
						else
						{
							x=0;
						}
						MWR(x,ADDR_AFN05F149+2,4);
						break;
				}
			}
			if(TerminalCommandUartLib[Fn-1].Command!=0x0)
			{//���ǽ�����������
				RXaddr+=i;
				DIlen-=i;
			}
		}
	}
	if(f==0x0)
	{
		f=1;
		goto loop;
	}
	else
	{
		//ȫ��ȷ��
		Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		BUZZ_Generate(2);//��������,��ڷ�����
	}
}



