
//UART���ò���
#include "Project.h"
#include "Terminal_Uart_AFN04_Paramter.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../Terminal/Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_Init.h"
#include "Terminal_AFN0D_SourceData.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "Terminal_AFN0C_RealTimeData_Init.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "../Device/RTC.h"

	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "Terminal_Statistic.h"
	#endif


extern TerminalParamterLib_TypeDef     TerminalParamterLib[];


void Uart_AFN04F10_ERR_READDR(u32 PORTn)//AFN04F10���ܱ����õ�ַ�ظ����󷵻�֡
{
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8txbuff[12]=0;//AFN=0
	
	p8txbuff[14]=0;//Pn=0
	p8txbuff[15]=0;
	p8txbuff[16]=4;//Fn=3(���յ������е�ȫ�����ݵ�Ԫ��ʶ�������ȷ��/����),Pn=0
	p8txbuff[17]=0;
	p8txbuff[18]=0x04;//Ҫ��ȷ�ϵı��ĵ�AFN=04H
	p8txbuff[19]=0;//Pn=0
	p8txbuff[20]=0;
	p8txbuff[21]=2;//Fn=10
	p8txbuff[22]=1;
	p8txbuff[23]=2;//ERR��=2(���ַ�ظ�)

	p8txbuff[1]=18;//���ݳ���
	p8txbuff[2]=0;
	p8txbuff[6]&=0x0f;
	p8txbuff[6]|=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}


//��������
void Uart_AFN04(u32 PORTn)//��������
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 f;
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
//	u32 *p32;
#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
	u32 *p32;
	u64 d64a;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
#endif
	
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
	if(DIlen<=4)
	{
		goto err;
	}
//����(����վ������10������1�����ݣ�����Ӧ�ò�����ȷ�ϣ�CON=1������·����)
	DIaddr=((u32)p8rxbuff)+14;
	RXaddr=((u32)p8rxbuff)+14+4;
	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	p8rxbuff[0]=1;//����ERC3���������¼��DI��ʶ��������
	while(DIlen>4)
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		if(i==0x0)//���ݱ�ʶ�ֽ�ΪFnPn
		{
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
			i=p8rxbuff[0]*4;
			if(f!=0x0)
			{
				MR(((u32)p8rxbuff)+14+(p8rxbuff[0]*4),DIaddr,4);//����ERC3���������¼��DI��ʶ����
				p8rxbuff[0]+=1;//����ERC3���������¼��DI��ʶ��������+1
			}
		}
		else
		{
			Fn=(i>>16);
			Pn=i&0xffff;
			p8=(u8 *)(RXaddr);
		#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
			if((Fn==297)&&(Pn==0))
			{
				goto AFN04F297;	
			}
			if((Fn==298)&&(Pn==0))
			{
				goto AFN04F298;	
			}
			if((Fn==299)&&(Pn==0))
			{
				goto AFN04F299;	
			}
		#endif
		#if (USER/100)==12//�Ĵ��û�
			if(Fn==151)
			{
				if((DIlen-4)<1)
				{
					goto err;
				}
				goto AFN04F151;
			}
		#endif
			if(Fn>Get_AFN04ParamterLib_MaxFn())
			{
				goto err;
			}
			if(Fn==0x0)
			{
				goto err;
			}
			if(Pn>TerminalParamterLib[Fn-1].PnMax)
			{
				goto err;
			}
			if(TerminalParamterLib[Fn-1].PnType!=0x0)
			{
				if(Pn==0x0)
				{
					goto err;
				}
			}
			if((TerminalParamterLib[Fn-1].project&(1<<(Project/100)))==0x0)
			{//�������޴�Fn����
				goto err;
			}
			switch(Fn)//Fn
			{
				case 0:
					goto err;
				case 2://�ն�����ͨ�ſ������м�ת������
					i=(p8[0]&0x7f);
					if(i>16)
					{
						goto err;
					}
					i=(i*2)+1;
					break;
				case 7://�ն�IP��ַ�Ͷ˿�����
					i=p8[20];
					if(i>20)//�û�������m(0��20)
					{
						goto err;
					}
					i=20+1+i;
					if(p8[i]>20)//���볤��n(0��20)
					{
						goto err;
					}
					i+=(1+p8[i]+2);
					break;
				case 10://�ն˵��ܱ���������װ�����ò�������
					if(f==0x0)
					{
					#if ((USER/100)==5)//�Ϻ��û�
						for(i=0;i<(p8[0]+(p8[1]<<8));i++)
						{
							if(p8[7+(i*LEN_AFN04F10_Pn)]==21)
							{//�Ϻ���Լ
								//��3�ֽڣ����쳧���룬1�ֽ�ASCII�룬��P����ó���֡��Ϊ50H����վ�·����ն�Ϊ80H(BCD��)
								p8[11+(i*LEN_AFN04F10_Pn)]=bcd_hex(p8[11+(i*LEN_AFN04F10_Pn)]);//8λѹ��BCD��תΪHEX	
							}
						}
					#endif
						i=p8[0]+(p8[1]<<8);
						if((DIlen-4)<((i*LEN_AFN04F10_Pn)+2))
						{
							goto err;
						}
						//ͬʱ���ö��ʱ��������ظ����
						for(x=0;x<i;x++)
						{
							p8=(u8 *)(RXaddr+(x*LEN_AFN04F10_Pn)+4);
							a64=p8[0]+(p8[1]<<8);
							if(a64==0)
							{//�������=0���Ƚ�
								continue;
							}
							z=0;
							p8=(u8 *)(RXaddr+4);
							for(y=0;y<i;y++)
							{
								b64=p8[0]+(p8[1]<<8);
								if(a64==b64)
								{
									z++;
								}
								p8+=LEN_AFN04F10_Pn;
							}
							if(z>1)
							{
								goto err;
							}
						}
						//ͬʱ���ö��ʱͨ�ŵ�ַ�ظ����
						for(x=0;x<i;x++)
						{
							p8=(u8 *)(RXaddr+(x*LEN_AFN04F10_Pn)+4);
							w=p8[0]+(p8[1]<<8);
							if(w==0)
							{//�������=0
								continue;
							}
							w=p8[2];//�˿ں�
							w&=0x1f;
							a64=MRR(((u32)p8)+4,6);
							a64=(p8[4+5]<<24)+(p8[4+4]<<16)+(p8[4+3]<<8)+p8[4+2];
							a64<<=16;
							a64+=(p8[4+1]<<8)+p8[4];
							z=0;
							for(y=0;y<i;y++)
							{
								p8=(u8 *)(RXaddr+(y*LEN_AFN04F10_Pn)+4);
								if((p8[0]+(p8[1]<<8))==0)
								{//�������=0
									continue;
								}
								if(w!=(p8[2]&0x1f))
								{//�˿ںŲ�ͬ
									continue;
								}
								b64=(p8[4+5]<<24)+(p8[4+4]<<16)+(p8[4+3]<<8)+p8[4+2];
								b64<<=16;
								b64+=(p8[4+1]<<8)+p8[4];
								if(a64==b64)
								{
									z++;
								}
							}
							if(z>1)
							{
								Uart_AFN04F10_ERR_READDR(PORTn);//AFN04F10���ܱ����õ�ַ�ظ����󷵻�֡
								return;
							}
						}
						//
						p8=(u8 *)(RXaddr);
						i=p8[0]+(p8[1]<<8);
						DIlen-=((i*LEN_AFN04F10_Pn)+2);
						RXaddr+=((i*LEN_AFN04F10_Pn)+2);
						p8+=2;
						for(;i!=0x0;i--)
						{
							y=p8[0]+(p8[1]<<8);
							if(y==0x0)
							{//���=0
								goto err;
							}
							if(y>AFN04F10MaxPn)
							{//���>Max
								goto err;
							}
							x=p8[2]+(p8[3]<<8);
							if(x>MaxRS485AddCarrierPn)
							{//���Ե�ų�ֵ��
								goto err;
							}
							if(x!=0)
							{
								z=p8[4];
								z&=0x1f;
								a64=MRR(((u32)p8)+6,6);
								z=Check_AFN04F10_Addr(z,a64);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0����ͬ��ַ
								if(z!=0)
								{//����ͬͨ�ŵ�ַ
									if(z!=y)
									{//������Ų�ͬ
										//goto err;
										Uart_AFN04F10_ERR_READDR(PORTn);//AFN04F10���ܱ����õ�ַ�ظ����󷵻�֡
										return;
									}
								}
							}

							x=Check_AFN04F10_Pn(x);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
							if(x==y)
							{//���������ͬ
								x=0;
							}
							x|=Check_AFN04F11_Pn(p8[2]+(p8[3]<<8));//����ն��������ò���ָ���Ĳ������;����0��,1��
							x|=Check_AFN04F13_Pn(p8[2]+(p8[3]<<8));//����ն˵�ѹ/����ģ�������ò���ָ���Ĳ������;����0��,1��
							if(x!=0)
							{//��������ظ���
								goto err;
							}
							p8+=LEN_AFN04F10_Pn;
						}
					}
					else
					{
						RXaddr+=2;
						DIlen-=2;
						for(i=0;i<(p8[0]+(p8[1]<<8));i++)
						{
							p8d=(u8 *)(ADDR_AFN04F10+(((p8[2+(i*LEN_AFN04F10_Pn)]+(p8[1+2+(i*LEN_AFN04F10_Pn)]<<8))-1)*LEN_AFN04F10_Pn));
							x=MRR(((u32)p8d)+2,2);//ԭ�������
							y=MRR(RXaddr+2,2);//���õĲ������
						#if (USER/100)==13//������
							if(x==1)
							{//ԭ�������
								z=MRR(((u32)p8d)+4,1);//ԭ�˿ں�
								z&=0x1f;
								if(z==1)
								{//ԭ�˿ں�
									if(MRR(((u32)p8d)+5,1)==2)
									{//ԭͨ��Э������ BIN 1 (����0����ʾ�ն�����Ա���ŵĵ��ܱ�/��������װ�ý��г���1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э�飻����������)
										w=MRR(RXaddr+4,1);//���õĶ˿ں�
										w&=0x1f;
										if((y!=1)||(w!=1)||(MRR(RXaddr+5,1)!=2))
										{//���õĲ������!=1�����õĶ˿ں�!=1�����õ�ͨ��Э�����Ͳ��ǽ���
											goto err;
										}
									}
								}
							}
						#endif
					#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
							//ͨ�ŵ�ַ�仯��ͬ��·�ɵ���
						#if ((USER/100)==11)||((USER/100)==12)//�û���ʾ������//������ˮ��̨����ز�����ת������������ͬ�� �Ĵ�͸���������Ҫ��������ͬ��
							Terminal_Ram->RouterSynDelay_S_Timer=0;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
						#else
							Terminal_Ram->RouterSynDelay_S_Timer=30;//29 ·�ɵ���ͬ����ʱִ���붨ʱ��
						#endif			
							if(Terminal_Router->AFN04F10==0)//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
							{
								z=MRR(((u32)p8d)+4,1);//ԭ�˿ں�
								z&=0x1f;
								w=MRR(RXaddr+4,1);//���õĶ˿ں�
								w&=0x1f;
								if((z==31)||(w==31))
								{//���ز������
									if(x!=y)
									{//������ű仯
										Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
									}
									if((x!=0)||(y!=0))
									{//�����������
										if(z!=w)
										{//�˿ںű仯
											Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
										}
									}
									if((y!=0)&&(x!=0))
									{//ԭ�������õĲ�����ž���Ч
										a64=MRR(((u32)p8d)+6,6);//ԭͨ�ŵ�ַ
										b64=MRR(RXaddr+6,6);//���õ�ͨ�ŵ�ַ
										if(a64!=b64)
										{//ͨ�ŵ�ַ�仯
											Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
										}
										a64=MRR(((u32)p8d)+20,6);//ԭ�ɼ�����ַ
										b64=MRR(RXaddr+20,6);//���õĲɼ�����ַ
										if(a64!=b64)
										{//�ɼ�����ַ�仯
											Terminal_Router->AFN04F10=1;//AFN04F10���ñ仯��־0=û,1=�仯;���ڵ���ͬ��
										}
									}
								}
							}
							//�����ý�1-31�ճ�����Ϊȫ�ɹ�,��ɾ��
							z=MRR(ADDR_AFN05F149+1,1);//�Զ�ά��״̬��00���رգ�01�����ò�����F10��02�����õ�������F10��ȱʡΪ�ر�
							if(z==0x1)
							{	
								if(y!=0)
								{
									MC(0,ADDR_RMDateFlags+(4*(y-1)),4);//�������ճ���ɹ�ʧ�ܱ�־;B0=����,B1-B31�ֱ��ʾ1-31�ճ���ɹ�ʧ�ܱ�־,λ0=�ɹ�1=ʧ��(��ʼ��ʱΪ0)
								}
							}
					#endif
							if(x!=y)
							{//������ű仯
								Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
								//Init_Class1Data(pnType,x);//��1���ݳ�ʼ��(ָ��Pn����)
								Init_Class1Data(pnType,y);//��1���ݳ�ʼ��(ָ��Pn����)
								//Init_Class2Data(x);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
								Init_Class2Data(y);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
								Init_Class3Data(y);//��3����(�¼�)��ʼ��(ָ���Ĳ���������,Pn=1~n)
								//Init_EnergyInc_Souser_Pn(x);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
								Init_EnergyInc_Souser_Pn(y);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
								//Init_Energy_Pn(x);//���������µ�������Ϊ0(ָ����Pn����)
								Init_Energy_Pn(y);//���������µ�������Ϊ0(ָ����Pn����)
							}
							//MW(RXaddr,TerminalParamterLib[Fn-1].Addr+(((p8[2+(i*LEN_AFN04F10_Pn)]+(p8[1+2+(i*LEN_AFN04F10_Pn)]<<8))-1)*LEN_AFN04F10_Pn),LEN_AFN04F10_Pn);
							RXaddr+=LEN_AFN04F10_Pn;
							DIlen-=LEN_AFN04F10_Pn;
						}
						//д��
						x=p8[2]+(p8[3]<<8);//д�뿪ʼ�������
						y=0;//������Ÿ���
						z=((u32)p8)+2;//д��Ļ��忪ʼ
						for(i=0;i<(p8[0]+(p8[1]<<8));i++)
						{
							w=p8[2+(i*LEN_AFN04F10_Pn)]+(p8[1+2+(i*LEN_AFN04F10_Pn)]<<8);//��ǰ�������
							if(w!=(x+y))
							{//������
								MW(z,ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn),y*LEN_AFN04F10_Pn);
								x=w;//д�뿪ʼ�������
								y=0;//������Ÿ���
								z=((u32)p8)+2+(i*LEN_AFN04F10_Pn);//д��Ļ��忪ʼ
							}
							y++;
						}
						MW(z,ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn),y*LEN_AFN04F10_Pn);
					#if ((USER/100)==5)//�Ϻ�     //������ⷢ�ֲ�������1·485��ֱ���޸Ĳ�����˿�Ϊ��һ��485����F129ȥ����������ݣ�������Чֵ���跢�Ͷ˿����³�������ſ���
            if(MRR(ADDR_AFN05F50+1,1)!=0x55)
						{
                MC(0x55,ADDR_AFN05F51+2-1,1);
            }
            if(MRR(ADDR_AFN05F50+2,1)!=0x55)
						{
                MC(0x55,ADDR_AFN05F51+3-1,1);
            }
        #endif							
						PnVSAFN04F10SetNoList();//�������Pn��ӦAFN04F10��������б�
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
						AFN04F150_Computer();//��������Ч��־����
						Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
						Terminal_Ram->F246PnListUpDate=0;//Ҫ�����ĵ����¼����Pn�б��Ѹ��±�־,0=Ҫ����,!=0�Ѹ���
					}
					continue;
				case 11://�ն��������ò�������
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*5)+1))
						{
							goto err;
						}
						//������������ͬ������ź����Լ��
						for(x=0;x<i;x++)
						{
							p8=(u8 *)(RXaddr+(x*5)+2);
							a64=p8[0];
							if(a64==0)
							{//�������=0���Ƚ�
								continue;
							}
							a64+=(p8[1]<<8);
							z=0;
							p8=(u8 *)(RXaddr+2);
							for(y=0;y<i;y++)
							{
								b64=MRR((u32)p8,2);
								if(a64==b64)
								{
									z++;
								}
								p8+=5;
							}
							if(z>1)
							{
								goto err;
							}
						}
						//
						p8=(u8 *)(RXaddr);
						i=p8[0];
						DIlen-=((i*5)+1);
						RXaddr+=((i*5)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							y=p8[0];
							if(y==0x0)
							{//���=0
								goto err;
							}
							if(y>AFN04F11MaxPn)
							{//���>Max
								goto err;
							}
							y=p8[1];
							if(y>MaxRS485AddCarrierPn)
							{//���Ե�ų�ֵ��
								goto err;
							}
							x=Check_AFN04F10_Pn(y);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
							//x|=Check_AFN04F11_Pn(y);//����ն��������ò���ָ���Ĳ������;����0��,1��
							x|=Check_AFN04F13_Pn(y);//����ն˵�ѹ/����ģ�������ò���ָ���Ĳ������;����0��,1��
							if(x!=0)
							{//��������ظ���
								goto err;
							}
							if(y!=0)
							{//�������!=0
								y+=(p8[2]<<8);
								z=AFN04F11MaxPn;
								for(x=0;x<z;x++)
								{
									if(y==MRR(ADDR_AFN04F11+(5*x)+1,2))
									{//��ͬ������ź�����
										if(p8[0]!=(x+1))
										{//��ͬ�˿ں�
											goto err;
										}
									}
								}
							}
							p8+=5;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							x=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//ԭ������ź���������
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5),5);
							y=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//���ú������ź���������
							if(x!=y)
							{//������Ż��������Ա仯
								//��˿�ԭ��¼����������˿ڵ���
								MC(0,ADDR_Terminal_impIn_Energy+((4+(5*(TMaxTariff+1)))*(p8[1+(i*5)]-1)),4+(5*(TMaxTariff+1)));
								//��˿�ԭ��¼������˿ڵ������������ʱ��
								MC(0,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*(p8[1+(i*5)]-1)),7*(TMaxTariff+1));
								//
								x&=0xff;
								y&=0xff;
								Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
								Init_Class1Data(pnType,x);//��1���ݳ�ʼ��(ָ��Pn����)
								Init_Class1Data(pnType,y);//��1���ݳ�ʼ��(ָ��Pn����)
								Init_Class2Data(x);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
								Init_Class2Data(y);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
								Init_EnergyInc_Souser_Pn(x);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
								Init_EnergyInc_Souser_Pn(y);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
								Init_Energy_Pn(x);//���������µ�������Ϊ0(ָ����Pn����)
								Init_Energy_Pn(y);//���������µ�������Ϊ0(ָ����Pn����)
							}
							RXaddr+=5;
							DIlen-=5;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
						AFN04F150_Computer();//��������Ч��־����
					}
					continue;
				case 13://�ն˵�ѹ/����ģ�������ò�������
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*3)+1))
						{
							goto err;
						}
						DIlen-=((i*3)+1);
						RXaddr+=((i*3)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							y=p8[0];
							if(y==0x0)
							{//���=0
								goto err;
							}
							if(y>AFN04F13MaxPn)
							{//���>Max
								goto err;
							}
							y=p8[1];
							if(y>MaxRS485AddCarrierPn)
							{//���Ե�ų�ֵ��
								goto err;
							}
							x=Check_AFN04F10_Pn(y);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
							x|=Check_AFN04F11_Pn(y);//����ն��������ò���ָ���Ĳ������;����0��,1��
							//x|=Check_AFN04F13_Pn(y);//����ն˵�ѹ/����ģ�������ò���ָ���Ĳ������;����0��,1��
							if(x!=0)
							{//��������ظ���
								goto err;
							}
							p8+=3;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							x=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*3)]-1)*3)+1,1);//ԭ�������
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*3)]-1)*3),3);
							y=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*3)]-1)*3)+1,1);//���ú�������
							if(x!=y)
							{//������ű仯
								Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
								Init_Class1Data(pnType,x);//��1���ݳ�ʼ��(ָ��Pn����)
								Init_Class1Data(pnType,y);//��1���ݳ�ʼ��(ָ��Pn����)
								Init_Class2Data(x);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
								Init_Class2Data(y);//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
								Init_EnergyInc_Souser_Pn(x);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
								Init_EnergyInc_Souser_Pn(y);//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
								Init_Energy_Pn(x);//���������µ�������Ϊ0(ָ����Pn����)
								Init_Energy_Pn(y);//���������µ�������Ϊ0(ָ����Pn����)
							}
							RXaddr+=3;
							DIlen-=3;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
						AFN04F150_Computer();//��������Ч��־����
					}
					continue;
				case 14://�ն��ܼ������ò�������
					if(f==0x0)
					{
						x=1;
						for(i=0;i<p8[0];i++)
						{
							x+=p8[x+1]+2;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							if(p8[x]==0x0)
							{
								goto err;
							}
							if(p8[x]>AFN04F14MaxPn)
							{
								goto err;
							}
							for(y=0;y<p8[x+1];y++)
							{//�����ܼӵĲ�����ż��
								if((p8[x+2+y]&0x3f)>=MaxRS485User)
								{
									goto err;
								}
							}
							x+=p8[x+1]+2;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[x]-1)*66),p8[x+1]+2);
							RXaddr+=p8[x+1]+2;
							DIlen-=p8[x+1]+2;
							x+=p8[x+1]+2;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;	
				case 15://�й��ܵ������Խ���¼���������
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*9)+1))
						{
							goto err;
						}
						DIlen-=((i*9)+1);
						RXaddr+=((i*9)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							if(p8[0]==0x0)
							{
								goto err;
							}
							if(p8[0]>AFN04F15MaxPn)
							{
								goto err;
							}
							p8+=9;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*9)]-1)*9),9);
							RXaddr+=9;
							DIlen-=9;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;
				case 22://�ն˵�������������
					i=p8[0];
					if(i>48)
					{
						goto err;
					}
					i=(i*4)+1;
					break;
				case 31://�ز��ӽڵ㸽���ڵ��ַ����
					i=p8[0];
					if(i>20)
					{
						goto err;
					}
					i=(i*6)+1;
					break;
				case 33://�ն˳������в�������
					if(f==0x0)
					{
						x=1;
						for(i=0;i<p8[0];i++)
						{
							x+=(p8[x+13]*4)+14;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							if(p8[x]==0x0)
							{
								goto err;
							}
							if(p8[x]>31)
							{
								goto err;
							}
							x+=(p8[x+13]*4)+14;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[x]-1)*(14+(4*24))),(p8[x+13]*4)+14);
							RXaddr+=(p8[x+13]*4)+14;
							DIlen-=(p8[x+13]*4)+14;
							x+=(p8[x+13]*4)+14;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;
				case 34://����������ͨ��ģ��Ĳ�������
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*6)+1))
						{
							goto err;
						}
						DIlen-=((i*6)+1);
						RXaddr+=((i*6)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							if(p8[0]==0x0)
							{
								goto err;
							}
							if(p8[0]>31)
							{
								goto err;
							}
							p8+=6;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*6)]-1)*6),6);
							RXaddr+=6;
							DIlen-=6;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;
				case 35://̨�����г����ص㻧����
					i=p8[0];
				#if (MaxKeyUser<=20)
					if(i>MaxKeyUser)
					{
						goto err;
					}
				#else
					if(i>20)
					{
						goto err;
					}
				#endif
					i=(i*2)+1;
					Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35���ò������±�־,!=0û����
					Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
					break;
				case 37://�ն˼���ͨ�Ų�������
					i=p8[6]&0x0f;
					if(i>3)
					{
						goto err;
					}
					i=(i*4)+7;
					//���Ǳ����������ʼ������
//					if(p8[0]==2)
//					{//RS485-1
//						if((p8[6]&0x80)!=0x0)
//						{//������
//							UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
//							UARTCtrl->BpsCtrl=p8[1];
//							Init_UARTn(RS485_1PORT);//��ʼ��UART��,���ͨ�ſ�����������
//						}
//					}
//					if(p8[0]==3)
//					{//RS485-2
//						if((p8[6]&0x80)!=0x0)
//						{//������
//							UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
//							UARTCtrl->BpsCtrl=p8[1];
//							Init_UARTn(RS485_2PORT);//��ʼ��UART��,���ͨ�ſ�����������
//						}
//					}
//					if(p8[0]==4)
//					{//RS485-3
//						if((p8[6]&0x80)!=0x0)
//						{//������
//							UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
//							UARTCtrl->BpsCtrl=p8[1];
//							Init_UARTn(RS485_3PORT);//��ʼ��UART��,���ͨ�ſ�����������
//						}
//					}
					break;
				case 38://1��������������
				case 39://2��������������
					if(f==0x0)
					{
//						if(p8[0]==0)
//						{//�����0:ȱʡ����,��ֹ��������
//							goto err;
//						}
						if(p8[0]>15)
						{//�����0-15
							goto err;
						}
						if(p8[1]==0x0)
						{//����������=0
							goto err;
						}
						if(p8[1]>16)
						{//����������>16
							goto err;
						}
						x=2;
						for(i=0;i<p8[1];i++)
						{
							if(p8[x]>15)
							{//�û�С���0-15
								goto err;
							}
							if(p8[x+1]>31)
							{//��Ϣ������0-31
								goto err;
							}
							x+=p8[x+1]+2;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
					}
					else
					{
						x=2;
						RXaddr+=2;
						DIlen-=2;
						for(i=0;i<p8[1];i++)
						{
							MW(RXaddr+1,TerminalParamterLib[Fn-1].Addr+(p8[0]*32*16)+(p8[x]*32),p8[x+1]+1);
							RXaddr+=p8[x+1]+2;
							DIlen-=p8[x+1]+2;
							x+=p8[x+1]+2;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;
				case 41://ʱ�ι��ض�ֵ����
					if(f==0x0)
					{
						if(p8[0]>7)
						{//������־D0-D2
							goto err;
						}
						x=1;
						for(i=1;i<7;)
						{
							if((p8[0]&i)!=0x0)
							{
								x+=(BitNum(p8[x])*2)+1;//������λΪ1�ĸ���(0-32)
							}
							i<<=1;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
					}
					else
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3))),1);
						x=1;
						RXaddr+=1;
						DIlen-=1;
						for(i=1;i<7;)
						{
							if((p8[0]&i)!=0x0)
							{
								switch(i)
								{
									case 1:
										MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1,(BitNum(p8[x])*2)+1);
										break;
									case 2:
										MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+17,(BitNum(p8[x])*2)+1);
										break;
									case 4:
										MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+34,(BitNum(p8[x])*2)+1);
										break;
								}
								RXaddr+=(BitNum(p8[x])*2)+1;//������λΪ1�ĸ���(0-32)
								DIlen-=(BitNum(p8[x])*2)+1;//������λΪ1�ĸ���(0-32)
								x+=(BitNum(p8[x])*2)+1;//������λΪ1�ĸ���(0-32)
							}
							i<<=1;
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;
				case 47://���������ѣ��ز���
					if(f==0x0)
					{
						if((DIlen-4)<17)
						{
							goto err;
						}
						p8d=(u8 *)(TerminalParamterLib[Fn-1].Addr+((Pn-1)*17));
						for(i=0;i<4;i++)
						{
							if(p8[i]!=p8d[i])
							{
								break;
							}
						}
						if(i==4)
						{
//#if SoftBug4==0//0=��������,1==376��վ�������BUG.���絥��	
						#if ( ((USER/100)==0)||((USER/100)==10) )//���Ժ�ͼ�,�ӱ�ϵ							
							if(p8[4]==0x55)//�ӱ�141226Ҫ��ˢ��ʱ���Ƚϵ���
							{
								goto err;//���絥����ͬ
							}	
						#else  //����ϵ	
							goto err;//���絥����ͬ
						#endif							
//#endif
						}
						if(p8[4]!=0x55)
						{
							if(p8[4]!=0xaa)
							{
								goto err;
							}
						}
					}
					else
					{
						x=MRR(ADDR_AFN0CF23+(4*(Pn-1)),4);//���ι���ǰʣ����������ѣ�
						if(p8[4]==0x55)
						{//׷��
							i=MRR(ADDR_AFN0CF23+(4*(Pn-1)),4);//����ǰʣ�����(��)���ݸ�ʽ03
							a64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
							b64=MRR(ADDR_AFN0CF23_DP+(2*(Pn-1)),2);//����ǰʣ�������λG=1ʱ(MWH)С���������,��ʽhex=0.000
							if((i&0x10000000)!=0x0)
							{//���ݸ�ʽ03Ϊ��ֵ
								b64=(~b64)+1;
							}
							a64+=b64;
							i=MRR(RXaddr+5,4);
							b64=Data03_kWh(i);//���ݸ�ʽ03תΪ��λͳһΪkwh��hex����
							a64+=b64;
							a64=kWh_Data03(a64);//��λͳһΪkwh��hex����תΪ���ݸ�ʽ03,��ת����λΪMWhʱС������(0.000MWh)hex��ʽ�ڷ��صĸ�4�ֽ�
							MWR(a64,ADDR_AFN0CF23+(4*(Pn-1)),4);//д�Ĵ������ݵ�������,Byte<=8
							a64>>=32;
							MWR(a64,ADDR_AFN0CF23_DP+(2*(Pn-1)),2);//д�Ĵ������ݵ�������,Byte<=8
						}
						else
						{//ˢ��
							MW(RXaddr+5,ADDR_AFN0CF23+(4*(Pn-1)),4);//ˢ���ն˵�ǰʣ�����(��)(�ܼ����)
							MC(0x0,ADDR_AFN0CF23_DP+(2*(Pn-1)),2);//�嵱ǰʣ�������λG=1ʱ(MWH)С���������,��ʽBCD=0.000
						}
						i=MRR(ADDR_AFN0CF23+(4*(Pn-1)),4);//���ι����ʣ����������ѣ�
				#if ERC19==1
						ERC19_Event(Pn,RXaddr,x,i);//����������ü�¼
				#endif
				#if (((USER/100)==5) || ((USER/100)==13))//�Ϻ���������
						//��ʾ�ù���ǰ��ʣ����������ѣ�
						a64=i;
						a64<<=32;
						a64|=x;
						MWR(a64,ADDR_AFN04F47_Ext+(8*(Pn-1)),8);
				#endif
					}
					i=TerminalParamterLib[Fn-1].Len;
					break;
				case 65://��ʱ�ϱ�1��������������
				case 66://��ʱ�ϱ�2��������������		
					if(f==0x0)
					{
						if(p8[8]>MaxTimerTaskDI)//���ʱ�������ݱ�ʶ����
						{
							goto err;
						}
						x=(p8[8]*4)+9;
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
					}
					else
					{
						x=(p8[8]*4)+9;
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(9+(4*MaxTimerTaskDI))),x);
						DIlen-=x;
						RXaddr+=x;
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;
				case 84://�ն�ͨ�Ŷ˿����ò���
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*15)+1))
						{
							goto err;
						}
						DIlen-=((i*15)+1);
						RXaddr+=((i*15)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							if(p8[0]==0x0)
							{//�������=0
								goto err;
							}
							if(p8[0]>AFN04F84MaxPn)
							{
								goto err;
							}
							p8+=15;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*15)]-1)*15),15);
							RXaddr+=15;
							DIlen-=15;
							//ͨ������ͬ��
							p8d=(u8 *)(RXaddr);
							x=p8d[1]&0x1f;
							y=p8d[1]>>5;//D7~D5 �����ʾͨ�Ŷ˿ڵ�ͨ�Ų����ʣ���λΪbps��1~7 ���α�ʾ600��1200��2400��4800��7200��9600��19200��0����ʾ�������û�ʹ��Ĭ��ֵ
							if(x==2)
							{//RS485-1
								if(y==0x0)
								{
									y=3;//Ĭ��ֵ3=2400
								}
//								MWR(y,UART_PORT_SET+4+1,1);//д�Ĵ������ݵ�������,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET+4);
//								if(p8d[0]!=0x0)
//								{//���ն���
//									p8d=(u8 *)(ADDR_AFN04F37);//�ն˼���ͨ�Ų���
//									if(p8d[0]==2)
//									{//����ͨ�Ŷ˿�
//										if((p8d[6]&0x80)!=0x0)
//										{//������
//											uart2bps((y<<5)|0x0b);//uart2������,���ͨ�ſ�����ͬ���ɹ���2005��Լ(��չ1λD8���ʱ�ʾ)
//										}
//									}
//								}
							}
							if(x==3)
							{//RS485-2
								if(y==0x0)
								{
									y=3;//Ĭ��ֵ3=2400
								}
//								MWR(y,UART_PORT_SET+8+1,1);//д�Ĵ������ݵ�������,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET+8);
//								if(p8d[0]!=0x0)
//								{//���ն���
//									p8d=(u8 *)(ADDR_AFN04F37);//�ն˼���ͨ�Ų���
//									if(p8d[0]==3)
//									{//����ͨ�Ŷ˿�
//										if((p8d[6]&0x80)!=0x0)
//										{//������
//											uart3bps((y<<5)|0x0b);//uart3������,���ͨ�ſ�����ͬ���ɹ���2005��Լ(��չ1λD8���ʱ�ʾ)
//										}
//									}
//								}
							}
							if(x==4)
							{//RS485-3
								if(y==0x0)
								{
									y=3;//Ĭ��ֵ3=2400
								}
//								MWR(y,UART_PORT_SET+12+1,1);//д�Ĵ������ݵ�������,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET+12);
//								if(p8d[0]!=0x0)
//								{//���ն���
//									p8d=(u8 *)(ADDR_AFN04F37);//�ն˼���ͨ�Ų���
//									if(p8d[0]==4)
//									{//����ͨ�Ŷ˿�
//										if((p8d[6]&0x80)!=0x0)
//										{//������
//											uart4bps((y<<5)|0x0b);//uart4������,���ͨ�ſ�����ͬ���ɹ���2005��Լ(��չ1λD8���ʱ�ʾ)
//										}
//									}
//								}
							}
							if(x==5)
							{//rs232
								if(y==0x0)
								{
									y=10;//Ĭ��ֵ10=115200
								}
//								MWR(y,UART_PORT_SET+1,1);//д�Ĵ������ݵ�������,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET);
//								if(p8d[0]!=0x0)
//								{//���ն���
//									uart0bps((y<<5)|0x0b);//uart0������,���ͨ�ſ�����ͬ���ɹ���2005��Լ(��չ1λD8���ʱ�ʾ)
//								}
							}
							
						}
						Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					}
					continue;
				case 97://F97ͣ�����ݲɼ����ò���
					i=p8[3]+(p8[4]<<8);
					if(i>MaxRS485AddCarrierPn)
					{//�������������
						goto err;
					}
					if((p8[0]&0x02)==0)
					{//D1����"1"���ѡ������㣬��"0"ֻ�ɼ����õĲ����㣻��"1"ʱ����д�������1���������n�������ܱ����n���ѡ��n��������
						for(x=0;x<i;x++)
						{//������ż��
							y=p8[5+(2*x)]+(p8[6+(2*x)]<<8);
							if(y>MaxRS485AddCarrierPn)
							{
								goto err;
							}
						}
					}
					i=5+(i*2);
					break;
				case 104://���߶����ܶȲ���(�������)
					if((DIlen-4)<2)
					{
						goto err;
					}
					i=p8[0];//����Fn����
					x=p8[1];
					if(f==0x0)
					{
						if(i!=0)
						{
							i=Get_CurveSetFnListNo(i);//�õ����߶����ܶȲ���Fn�б��;����:�б��0-n,0xffffffff��ʾ��
							if(i==0xffffffff)
							{
								goto err;
							}
						}
						RXaddr+=2;
						DIlen-=2;
						continue;
					}
					if(i==0)
					{//������Fn ����Ϊ0 ʱ����ʾ�����ö�����������Ч
						MC(x,ADDR_AFN04F104,LEN_AFN04F104_PerPn*AFN04F104MaxPn);
					}
					else
					{
						i=Get_CurveSetFnListNo(i);//�õ����߶����ܶȲ���Fn�б��;����:�б��0-n,0xffffffff��ʾ��
						if(i!=0xffffffff)
						{
							MWR(x,ADDR_AFN04F104+((Pn-1)*LEN_AFN04F104_PerPn)+i,1);
						}
					}
					Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
					continue;
				case 106://���ܱ����ݷּ�����(���ݷּ����)
					x=p8[0];//�������õĵ��ܱ����ݸ���n
					if((DIlen-4)<(1+(x*5)))
					{
						goto err;
					}
					if(f==0x0)
					{
						RXaddr+=(1+(x*5));
						DIlen-=(1+(x*5));
						continue;
					}
					MR(ADDR_DATABUFF,ADDR_AFN04F106+((Pn-1)*5*NUM_AFN04F106_PerC),5*NUM_AFN04F106_PerC);
					if(x>NUM_AFN04F106_PerC)
					{
						x=NUM_AFN04F106_PerC;//ÿ�ּ���洢�ĵ��ܱ����ݱ�ʶ����
					}
					for(i=0;i<x;i++)
					{
						y=MRR(RXaddr+1+(i*5),4);
						for(z=0;z<NUM_AFN04F106_PerC;z++)
						{
							f=MRR(ADDR_DATABUFF+(z*5),4);
							if(y==f)
							{//�ҵ�ԭ����λ��
								p8=(u8*)(RXaddr+1+(i*5)+4);
								p8d=(u8*)(ADDR_DATABUFF+(z*5)+4);
								p8d[0]=p8[0];
								break;
							}
						}
						if(z==NUM_AFN04F106_PerC)
						{//û�ҵ�ԭ����λ��,��1����λ�ô洢
							for(z=0;z<NUM_AFN04F106_PerC;z++)
							{
								p8d=(u8*)(ADDR_DATABUFF+4+(z*5));
								if(p8d[0]==0)
								{//�ҵ���λ�ô洢
									MW(RXaddr+1+(i*5),ADDR_DATABUFF+(z*5),5);
									break;
								}
							}
						}
					}
					MW(ADDR_DATABUFF,ADDR_AFN04F106+((Pn-1)*5*NUM_AFN04F106_PerC),5*NUM_AFN04F106_PerC);
					f=1;
					RXaddr+=(1+(x*5));
					DIlen-=(1+(x*5));
					continue;
				case 107://�ּ��¼��ɼ����ڲ���
					//Ϊ�˵��Ժ̨����Էּ��¼��ϱ�˳������,����������ʱ��ʾ���Կ�ʼ,�峭����ʱ
					//ԭʱ������Чʱ��01��01��01��00ʱ00��
					for(i=0;i<4;i++)
					{
						MWR(0x0101010000,((u32)&UARTRMEvent_4851->YMDHMS)+(6*i)+1,5);//485-1
					}
					for(i=0;i<4;i++)
					{
						MWR(0x0101010000,((u32)&UARTRMEvent_4852->YMDHMS)+(6*i)+1,5);//485-2
					}
					for(i=0;i<4;i++)
					{
						MWR(0x0101010000,((u32)&UARTRMEvent_4854->YMDHMS)+(6*i)+1,5);//485-4(�ز�)
					}
					i=TerminalParamterLib[Fn-1].Len;
					break;
				case 150://������״̬(ֻ������)
					goto err;
			#if (USER/100)==12//�Ĵ��û�
/*
��������AFN04 ��չF151 485�ڹ������á�����1�ֽڣ�D0~D7y���α�ʾ485I��485II����
��1����ʾ485����ͨ�ſڡ���0��ʾΪ485����ڡ�
*/
				case 151:
				AFN04F151:
					if(Comm_Ram->Factory!=0x55)
					{//0x55=����ģʽ
						MWR((~p8[0])&0x1,ADDR_UART_PORT_SET+LEN_UART_PORT_SET,1);
					}
					MWR(((~p8[0])>>1)&0x1,ADDR_UART_PORT_SET+2*LEN_UART_PORT_SET,1);
					RXaddr+=1;
					DIlen-=1;
					continue;
			#endif
			#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
				case 297://�������ݡ��㽭�ն˶�ʱ����
			AFN04F297:
					i=10;
					break;	
				case 298://�������ݡ��㽭����ʱ����
			AFN04F298:
					i=8;
					break;	
				case 299://�������ݡ��㽭�ն�������վʱ��
			AFN04F299:
					if(UARTCtrl->HostACK==7)//������������վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					{
						UARTCtrl->HostACK=0;//����վȷ��֡���:0=��
						UARTCtrl->HostCount=0;//�����ϱ��ط�����(��������������)
						if(Terminal_Ram->PowerTask==0)//ͣ���¼�����
						{
							Terminal_Ram->RMDelay_S_Timer=0;
							Terminal_Ram->PowerTask=1;//ͣ���¼�����

							p8rxbuff[21+4]&=0x1f;//ȥ�������ֽ��ϵ�����
							p8rxbuff[30+4]&=0x1f;//ȥ�������ֽ��ϵ�����
							i=Check_YMDHMS(MRR((u32)p8rxbuff+21,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
							if(i==0)
							{//ʱ�ӺϷ�
								i=Check_YMDHMS(MRR((u32)p8rxbuff+30,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
								if(i==0)
								{//ʱ�ӺϷ�
									i=TimeDifference((u32)p8rxbuff+21,(u32)p8rxbuff+30);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
						 			if((i>>31)!=0x0)
									{//��
										i=~i;
										i++;
										if(i>=300)
										{
											d64a=MRR(ADDR_ERC14SOUR+22,6);
											d64a=YMDHMS_SubS(d64a,i);//������ʱ�����S��,���ؼ�S����������ʱ����
											MWR(d64a,ADDR_ERC14SOUR+22,6);
		
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_SubS(d64a,i);//������ʱ�����S��,���ؼ�S����������ʱ����
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									else
									{
										if(i>=300)
										{
											d64a=MRR(ADDR_ERC14SOUR+22,6);
											d64a=YMDHMS_AddS(d64a,i);//������ʱ�����S��,���ؼ�S����������ʱ����
											MWR(d64a,ADDR_ERC14SOUR+22,6);
		
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_AddS(d64a,i);//������ʱ�����S��,���ؼ�S����������ʱ����
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									if(i>=300)
									{
//										WriteLPCRTC(ADDR_RTCBUFF);//д�ڲ�RTC
//										SyncLPCRTC();//ͬ���ڲ�RTC(���ڲ�д�ⲿ)
										WriteRTC(ADDR_RTCBUFF);//д�ⲿӲʵʱ��
										Comm_Ram->MS_Task=0;//�²�����RTC
										Comm_Ram->ReadRTCTimer=0;//25 ��ERTCÿ10MS��1��ʱ��
										Terminal_Ram->I2SUBTask1=0;//�²�����ʵʱ����RTC
										//��ʱ�¼���¼
										MWR(0,ADDR_DATABUFF+2,2);//�������
										MR(ADDR_DATABUFF+4,ADDR_AFN0CF2,6);//��ʱǰʱ��
										MR(ADDR_DATABUFF+10,(u32)p8rxbuff+30,6);//��ʱ��ʱ��
									WriteTerminalTime:
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
								}
							}
						}
						else
						{
							p8rxbuff[21+4]&=0x1f;//ȥ�������ֽ��ϵ�����
							p8rxbuff[30+4]&=0x1f;//ȥ�������ֽ��ϵ�����
							p8rxbuff[39+4]&=0x1f;//ȥ�������ֽ��ϵ�����
							i=Check_YMDHMS(MRR((u32)p8rxbuff+21,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
							x=Check_YMDHMS(MRR((u32)p8rxbuff+30,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
							y=Check_YMDHMS(MRR((u32)p8rxbuff+39,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
							if((i==0)&&(x==0)&&(y==0))
							{
								i=MRR(ADDR_AFN04F299_Ext+28,1);
								i&=0x7f;
								i++;
								MWR(i|0x80,ADDR_AFN04F299_Ext+28,1);
								//V=T4-T3
								MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
								p8=(u8 *)ADDR_DATABUFF;
								p8[4]&=0x1f;//ȥ�������ֽ��ϵ�����
								y=TimeDifference((u32)p8rxbuff+39,(u32)ADDR_DATABUFF);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
								//U=T2-T1
								x=TimeDifference((u32)p8rxbuff+21,(u32)p8rxbuff+30);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
								//K=(U-V)/2
					 			if(((y>>31)!=0x0)&&((x>>31)!=0x0))
								{//������
									y=~y;
									y++;
									x=~x;
									x++;
									if(x>y)
									{
										z=(x-y);
										z/=2;
										z=~z;
										z++;
									}
									else
									{
										z=(y-x);
										z/=2;
									}
								}
								else
								{
						 			if(((y>>31)!=0x0)&&((x>>31)==0x0))
									{//������
										y=~y;
										y++;
										z=(x+y);
										z/=2;
									}
									else
									{
							 			if(((y>>31)==0x0)&&((x>>31)!=0x0))
										{//������
											x=~x;
											x++;
											z=(x+y);
											z/=2;
											z=~z;
											z++;
										}
										else
										{
								 			if(((y>>31)==0x0)&&((x>>31)==0x0))
											{//������
												if(x>y)
												{
													z=(x-y);
													z/=2;
												}
												else
												{
													z=(y-x);
													z/=2;
													z=~z;
													z++;
												}
											}
										}
									}
								}
								MWR(z,ADDR_AFN04F299_Ext+28+1+(i-1)*4,4);
								if(i==24)
								{
									MR(ADDR_AFN04F299_Ext+28+1+4*25+4+1,ADDR_TYMDHMS,6);
									MR(ADDR_DATABUFF,ADDR_AFN04F299_Ext+28+1,96);
									p32=(u32 *)ADDR_DATABUFF;
	
									//��������
									x=0;
									z=0;
									for(y=0;y<24;y++)
									{
										
										if((p32[y]>>31)!=0)
										{//��
											p32[47-z]=p32[y];
											p32[47-z]=~p32[47-z];
											p32[47-z]++;
											z++;
										}
										else
										{
											p32[x+24]=p32[y];
											x++;
										}
									}
									//��������
									p32[48]=x;
									//�����Ӵ�С����
									if(p32[48]!=0)
									{
										for(x=0;x<p32[48]-1;x++) 
										{ 
											for (i=0;i<p32[48]-1-x;i++)
											{
												if (p32[24+i]<p32[24+i+1]) 
												{ 
													y=p32[24+i]; 
													p32[24+i]=p32[24+i+1]; 
													p32[24+i+1]=y;
												} 
											} 
										} 
									}
									//�����Ӵ�С����
									if(p32[48]!=24)
									{
										for(x=0;x<(23-p32[48]);x++) 
										{ 
											for (i=0;i<(23-p32[48])-x;i++)
											{
												if (p32[47-i]<p32[47-(i+1)]) 
												{ 
													y=p32[47-i]; 
													p32[47-i]=p32[47-(i+1)]; 
													p32[47-(i+1)]=y;
												} 
											} 
										}
									}
									//ѡȡ20��Kֵ��С�����ݣ���ȡ��ʱƽ��ֵ
									if(p32[48]<=4)
									{
										i=0;
										for(x=0;x<20;x++)
										{
											i+=p32[24+4+x];
										}
										i/=20;
										i=~i;
										i++;
									}
									else
									{
										i=0;
										for(x=0;x<(p32[48]-4);x++)
										{
											i+=p32[24+4+x];
										}
										y=0;
										for(x=0;x<(24-p32[48]);x++)
										{
											y+=p32[47-x];
										}
										if(i>y)
										{
											i-=y;
											i/=20;
										}
										else
										{
											i=(y-i);
											i/=20;
											i=~i;
											i++;
										}
									}
									//��ʱǰ���
									MWR(i,ADDR_AFN04F299_Ext+28+1+4*25,4);
									if((i>>31)!=0)
									{//��
										i=~i;
										i++;
										if(i>MRR(ADDR_AFN04F297_Ext+8,1))
										{
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_SubS(d64a,i);//������ʱ�����S��,���ؼ�S����������ʱ����
											MWR(d64a,ADDR_TYMDHMS,6);
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									else
									{
										if(i>MRR(ADDR_AFN04F297_Ext+8,1))
										{
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_AddS(d64a,i);//������ʱ�����S��,���ؼ�S����������ʱ����
											MWR(d64a,ADDR_TYMDHMS,6);
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									if(i>MRR(ADDR_AFN04F297_Ext+8,1))
									{
										WriteRTC(ADDR_RTCBUFF);//д�ⲿӲʵʱ��
										Comm_Ram->MS_Task=0;//�²�����RTC
										Comm_Ram->ReadRTCTimer=0;//25 ��ERTCÿ10MS��1��ʱ��
										Terminal_Ram->I2SUBTask1=0;//�²�����ʵʱ����RTC
										//��ʱ�¼���¼
										MWR(0,ADDR_DATABUFF+2,2);//�������
										MR(ADDR_DATABUFF+4,ADDR_AFN0CF2,6);//��ʱǰʱ��
										MR(ADDR_DATABUFF+10,ADDR_TYMDHMS,6);//��ʱ��ʱ��
										goto WriteTerminalTime;
									}
									else
									{
										MWR(0,ADDR_AFN04F299_Ext+28,1);
										MWR(0x80,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										//F249
										MR(ADDR_AFN0CF249_Ext,ADDR_TYMDHMS+1,5);
										MWR(i,ADDR_AFN0CF249_Ext+5,0);
										MWR(0,ADDR_AFN0CF249_Ext+7,0);
										MWR(2,ADDR_AFN0CF249_Ext+9,1);
									}
								}
								else
								{
									if(i==25)
									{
										//F249
										MR(ADDR_AFN0CF249_Ext,ADDR_TYMDHMS+1,5);
										//ERC51
										p8=(u8 *)ADDR_DATABUFF;
										i=MRR(ADDR_AFN04F299_Ext+28+1+25*4,4);//Уʱǰ���
										if((i>>31)!=0)
										{
											i=~i;
											i++;
										}
										MWR(i,ADDR_AFN0CF249_Ext+5,2);
										MWR(i,ADDR_DATABUFF+9,2);
										if(MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1)==0xff)//���ܱ��ʱ��Ľ��
										{
											i&=0xffff;
											y=MRR(ADDR_AFN0CF250_Ext1+10*x+5,2);//���ܱ��ʱǰ�����
											y-=i;
											MWR(y,ADDR_AFN0CF250_Ext1+10*x+5,2);//���ܱ��ʱǰ�����
										}
										i=MRR(ADDR_AFN04F299_Ext+28+1+24*4,4);//Уʱ�����
										if((i>>31)!=0)
										{
											i=~i;
											i++;
										}
										MWR(i,ADDR_AFN0CF249_Ext+7,2);
										MWR(i,ADDR_DATABUFF+11,2);
										if(i>MRR(ADDR_AFN04F297_Ext+8,1))
										{//Уʱʧ��
											p8[13]=0;
											MWR(0,ADDR_AFN0CF249_Ext+9,1);
										}
										else
										{//Уʱ�ɹ�
											p8[13]=1;
											MWR(1,ADDR_AFN0CF249_Ext+9,1);
										}
										ERC51_Event(0+(1<<15),ADDR_DATABUFF);//Уʱ��¼�¼�;���:Pn�������(��0��ʼ)
										//��¼�ڼ��ζ�ʱ
										i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										i&=0x7f;
										i++;
										i|=(p8[13]<<7);
										MWR(i,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										if(DataComp(ADDR_AFN04F297_Ext+4,ADDR_TYMDHMS+1,2)==2)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
										{
											i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
											MWR(i&0x80,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										}
									}
								}
							}
							}
					}
					i=28;
					break;	
			#endif
			#if (USER/100)==11//����ϵ
				case 6://
					if(Comm_Ram->Factory==0x55)//0x55=����״̬
					{
						i=16;
					}
					else
					{
						i=TerminalParamterLib[Fn-1].Len;
					}
					break;
			#endif
				default:
					i=TerminalParamterLib[Fn-1].Len;
					break;
			}
			if(i==0x0)
			{
				goto err;
			}
			if(Fn!=97)
			{//F97ͣ�����ݲɼ����ò���ʱ����֡����
				if((DIlen-4)<i)
				{
					goto err;
				}
			}
			if(f==0x1)
			{
			#if (USER/100)==7//�㽭�û�
				if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
				{
					if((UARTCtrl->LinkTask>=100)&&(UARTCtrl->LinkTask<=101))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
					{//����
						if(Fn==3)
						{
							if(DataComp(RXaddr,TerminalParamterLib[Fn-1].Addr,i))//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							{
								UARTCtrl->LinkTask=102;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
								if(PORTn==GPRSPORT)
								{
									Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS�����붨ʱ��
								}
								if(PORTn==ENETPORT)
								{
									Terminal_Ram->ENETHeartBeat_S_Timer=0;//��̫�������붨ʱ��
								}
							}
						}
						if(Fn==8)
						{
							if(DataComp(RXaddr,TerminalParamterLib[Fn-1].Addr,1))//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							{
								UARTCtrl->LinkTask=102;//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
								if(PORTn==GPRSPORT)
								{
									Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS�����붨ʱ��
								}
								if(PORTn==ENETPORT)
								{
									Terminal_Ram->ENETHeartBeat_S_Timer=0;//��̫�������붨ʱ��
								}
							}
						}
					}
				}
			#endif
			#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
				if(Pn==0x0)
				{
					if((Fn!=297)||(Fn!=298))
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
					if(Fn==297)
					{
						MW(RXaddr,ADDR_AFN04F297_Ext,i);
					}
					if(Fn==298)
					{
						MW(RXaddr,ADDR_AFN04F298_Ext,i);
					}
				}
			#else
				#if (USER/100)==11//����
				if(Pn==0x0)
				{
					if(Fn==89)
					{
						if(Comm_Ram->Factory==0x55)//0x55=����״̬
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
						else
						{// �ǹ���ģʽ������F89���ط���
							goto err;
						}
					}
					else
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
				}
				#else
					#if (USER/100)==12//�Ĵ�
					if(Pn==0x0)
					{
						if(Fn==245)
						{
							x=MRR(RXaddr,4);
							y=BitNum(x);//������λΪ1�ĸ���(0-32)
							if(y<=5)
							{//�Ƽ��������ɼ�����Ϊ5�죬��ǰ��5����"1"��Ч
								MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
							}
							else
							{
								y=0;
								for(i=0;i<32;i++)
								{
									if((x&(1<<i))!=0x0)
									{
										y+=1;
									}
									if(y==5)
									{
										break;
									}
								}
								for(z=0;z<(31-i);z++)
								{
									x&=(~(1<<(31-z)));
								}
								MWR(x,TerminalParamterLib[Fn-1].Addr,4);
							}
						}
						else
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
					}
					#else
					if(Pn==0x0)
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
					#endif					
				#endif	
			#endif	
				else
				{
					MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*i),i);
				}
				Terminal_AFN0CF3_Computer(Fn);//�ն���1����F3,�ն˲���״̬
				if(Fn==97)
				{//F97ͣ�����ݲɼ����ò���
					Terminal_Ram->F246PnListUpDate=0;//Ҫ�����ĵ����¼����Pn�б��Ѹ��±�־,0=Ҫ����,!=0�Ѹ���
				}
			}
			RXaddr+=i;
			if(DIlen>=i)
			{
				DIlen-=i;
			}
			else
			{
				DIlen=0;
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
	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
		if(Fn!=299)
		{
			ERC3_Event((u32)p8rxbuff);//���������¼,��ڽ��ջ�����ַ
		}
	#else
		ERC3_Event((u32)p8rxbuff);//���������¼,��ڽ��ջ�����ַ
	#endif
		Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		BUZZ_Generate(2);//��������,��ڷ�����
	}
}

//������ѯ
void Uart_AFN0A(u32 PORTn)//������ѯ
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
  u32 LEN_UARTnTx;
	u8 * p8s;
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
	u16 *p16;
	u32 crc;
#endif
#if ((USER/100)==13)&&((Project/100)==3)//������ר��
	u32 z;
#endif
#if (USER/100)==6//�û���ʶ:����
	u32 k;
#endif


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
	if(DIlen<4)
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
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		if(i==0x0)//���ݱ�ʶ�ֽ�ΪFnPn
		{
DIerr:
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if((TXlen+14+8+2)>Get_LEN_UARTnRx(PORTn))
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
		#if (USER/100)==12//�Ĵ��û�
			if(Fn==151)
			{
				goto AFN0AF151;
			}
		#endif
		#if (USER/100)==6//����
			if(Fn==200)
			{
				goto AFN0AF200;
			}
		#endif
		#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
			p8s=(u8 *)(ADDR_DATABUFF);
			if((Fn==296)&&(Pn==0))
			{
				if(DIlen<6)
				{
					goto err;
				}
				else
				{
					goto AFN0AF296;	
				}
			}
			if((Fn==297)&&(Pn==0))
			{
				goto AFN0AF297;	
			}
			if((Fn==298)&&(Pn==0))
			{
				goto AFN0AF298;	
			}
		#endif
			if(Fn>Get_AFN04ParamterLib_MaxFn())
			{
				goto DIerr;
			}
			if(Fn==0x0)
			{
				goto DIerr;
			}
			if(Pn>TerminalParamterLib[Fn-1].PnMax)
			{
				goto err;
			}
			if(TerminalParamterLib[Fn-1].PnType!=0x0)
			{
				if(Pn==0x0)
				{
					goto err;
				}
			}
			if((TerminalParamterLib[Fn-1].project&(1<<(Project/100)))==0x0)
			{//�������޴�Fn����
				goto err;
			}
			p8s=(u8 *)(ADDR_DATABUFF);
			switch(Fn)//Fn
			{
				case 0:
					goto DIerr;
				case 2://�ն�����ͨ�ſ������м�ת����ѯ
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0]&0x7f;
					if(i>16)
					{
						i=16;
					}
					i*=2;
					i+=1;
					break;
				case 7://�ն�IP��ַ�Ͷ˿ڲ�ѯ
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,64);
					i=20+1+p8s[20];
					i+=(1+p8s[i]+2);
					break;
				case 10://�ն˵��ܱ���������װ�����ò�����ѯ
					if((TXlen+4+14+8+2+2)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0]+(p8s[1]<<8);
					if((DIlen-4)<(2+(i*2)))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					p8txbuff[1]=0;
					TXlen+=2;
					TXaddr+=2;
					for(i=0;i<(p8s[0]+(p8s[1]<<8));i++)
					{
						if((TXlen+4+14+8+2+LEN_AFN04F10_Pn)>LEN_UARTnTx)
						{
							goto err;
						}
						x=p8s[2+(i*2)]+(p8s[2+(i*2)+1]<<8);
						if(x==0x0)
						{
							continue;	
						}
						if(x>AFN04F10MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((x-1)*LEN_AFN04F10_Pn),LEN_AFN04F10_Pn);
						p8d=(u8 *)(TXaddr);
						if(x==(p8d[0]+(p8d[1]<<8)))
						{//�����ͬ
						#if ((USER/100)==5)//�Ϻ��û�
							if(p8d[5]==21)
							{//�Ϻ���Լ
								//��3�ֽڣ����쳧���룬1�ֽ�ASCII�룬��P����ó���֡��Ϊ50H����վ�·����ն�Ϊ80H(BCD��)
								p8d[9]=hex_bcd(p8d[9]);	
							}
						#endif
							TXlen+=LEN_AFN04F10_Pn;
							TXaddr+=LEN_AFN04F10_Pn;
							f=p8txbuff[0]+(p8txbuff[1]<<8);//���β�ѯ����+1
							f+=1;
							p8txbuff[0]=f;
							p8txbuff[1]=f>>8;
						}
					}
					RXaddr+=2+(i*2);
					DIlen-=2+(i*2);
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 11://�ն��������ò�����ѯ
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+5)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F11MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*5),5);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//�����ͬ
							TXlen+=5;
							TXaddr+=5;
							p8txbuff[0]+=1;//���β�ѯ����+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 13://�ն˵�ѹ/����ģ�������ò�����ѯ
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+3)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F13MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*3),3);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//�����ͬ
							TXlen+=3;
							TXaddr+=3;
							p8txbuff[0]+=1;//���β�ѯ����+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 14://�ն��ܼ������ò�����ѯ
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F14MaxPn)
						{
							continue;
						}
						MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*66)+1,1);
						p8d=(u8 *)(ADDR_DATABUFF);
						x=p8d[0]+2;
						if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
						{
							goto err;
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*66),x);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//�����ͬ
							TXlen+=x;
							TXaddr+=x;
							p8txbuff[0]+=1;//���β�ѯ����+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 15://�й��ܵ������Խ���¼�������ѯ
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+9)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F15MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*9),9);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//�����ͬ
							TXlen+=9;
							TXaddr+=9;
							p8txbuff[0]+=1;//���β�ѯ����+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 22://�ն˵��������ʲ�ѯ
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0];
					if(i>48)
					{
						i=48;
					}
					i*=4;
					i+=1;
					break;
				case 31://�ز��ӽڵ㸽���ڵ��ַ��ѯ
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0];
					if(i>20)
					{
						i=20;
					}
					i*=6;
					i+=1;
					break;
				case 33://�ն˳������в�����ѯ
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>31)
						{
							continue;
						}
						MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*(14+(4*24)))+13,1);
						p8d=(u8 *)(ADDR_DATABUFF);
						x=(p8d[0]*4)+14;
						if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
						{
							goto err;
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*(14+(4*24))),x);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//�����ͬ
							TXlen+=x;
							TXaddr+=x;
							p8txbuff[0]+=1;//���β�ѯ����+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;	
				case 34://����������ͨ��ģ��Ĳ�����ѯ
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+6)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>31)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*6),6);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//�����ͬ
							TXlen+=6;
							TXaddr+=6;
							p8txbuff[0]+=1;//���β�ѯ����+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;	
				case 35://̨�����г����ص㻧��ѯ
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0];
				#if (MaxKeyUser<=20)
					if(i>MaxKeyUser)
					{
						i=MaxKeyUser;
					}
				#else
					if(i>20)
					{
						i=20;
					}
				#endif
					i*=2;
					i+=1;
					break;	
				case 37://�ն˼���ͨ�Ų�����ѯ
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+6,1);
					i=p8s[0]&0x0f;
					if(i>3)
					{
						i=3;
					}
					i*=4;
					i+=7;
					break;	
				case 38://1���������ò�ѯ
				case 39://2���������ò�ѯ
					if((TXlen+4+14+8+2+2)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[1];
					if((DIlen-4)<(2+i))
					{
						goto err;
					}
					if(p8s[0]>15)
					{//���β�ѯ���û������>15
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=p8s[0];//���β�ѯ���û������
					p8txbuff[1]=0;//���β�ѯ����=0
					TXlen+=2;
					TXaddr+=2;
					for(i=0;i<p8s[1];i++)
					{
						if(p8s[2+i]>15)
						{//С���>15
							goto err;
						}
						p8d=(u8 *)(ADDR_DATABUFF);
						MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+(p8s[0]*32*16)+(p8s[2+i]*32),1);
						x=p8d[0];
						if(x>31)
						{
							x=31;
						}
						if((TXlen+4+14+8+2+x+2)>LEN_UARTnTx)
						{
							goto err;
						}
						p8d=(u8 *)(TXaddr);
						p8d[0]=p8s[2+i];//С���
						MR(TXaddr+1,TerminalParamterLib[Fn-1].Addr+(p8s[0]*32*16)+(p8s[2+i]*32),x+1);
						TXlen+=x+2;
						TXaddr+=x+2;
						p8txbuff[1]+=1;//���β�ѯ����+1
					}
					RXaddr+=2+i;
					DIlen-=2+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 41://ʱ�ι��ض�ֵ��ѯ
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3))),1);
					p8s[0]&=0x7;
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=p8s[0];//������־
					TXlen+=1;
					TXaddr+=1;
					for(i=1;i<7;)
					{
						if((p8s[0]&i)!=0x0)
						{
							switch(i)
							{
								case 1:
									MR(ADDR_DATABUFF+1,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1,1);
									x=(BitNum(p8s[1])*2)+1;//������λΪ1�ĸ���(0-32)
									if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
									{
										goto err;
									}
									MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1,x);
									break;
								case 2:
									MR(ADDR_DATABUFF+1,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+17,1);
									x=(BitNum(p8s[1])*2)+1;//������λΪ1�ĸ���(0-32)
									if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
									{
										goto err;
									}
									MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+17,x);
									break;
								case 4:
									MR(ADDR_DATABUFF+1,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+34,1);
									x=(BitNum(p8s[1])*2)+1;//������λΪ1�ĸ���(0-32)
									if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
									{
										goto err;
									}
									MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+34,x);
									break;
							}
							TXlen+=x;
							TXaddr+=x;
						}
						i<<=1;
					}
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 65://��ʱ�ϱ�1��������������
				case 66://��ʱ�ϱ�2��������������				
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(9+(4*MaxTimerTaskDI)))+8,1);
					x=(p8s[0]*4)+9;
					if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(9+(4*MaxTimerTaskDI))),x);
					TXlen+=x;
					TXaddr+=x;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 84://�ն�ͨ�Ŷ˿����ò���
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//���β�ѯ����=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+15)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F84MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*15),15);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//�����ͬ
							TXlen+=15;
							TXaddr+=15;
							p8txbuff[0]++;//���β�ѯ����+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 97://F97ͣ�����ݲɼ����ò���
					i=MRR(ADDR_AFN04F97+3,2);
					if(i>MaxRS485AddCarrierPn)
					{
						i=MaxRS485AddCarrierPn;
					}
					i=5+(i*2);
					break;
				case 104://���߶����ܶȲ���(�������)
					y=0;//�����ܶȷ�Ĭ��ֵ�����߸���=0
					for(i=1;i<=248;i++)
					{
						x=Get_CurveSetFnListNo(i);//�õ����߶����ܶȲ���Fn�б��;����:�б��0-n,0xffffffff��ʾ��
						if(x!=0xffffffff)
						{
							x=MRR(ADDR_AFN04F104+((Pn-1)*LEN_AFN04F104_PerPn)+x,1);
							if(x!=127)
							{//ֻ���ظ��Ĺ������ܶȵ����ߣ��������ܶ����ݷǡ�127��������
								if((TXlen+4+14+8+2+(1+((y+1)*2)))>LEN_UARTnTx)
								{
									goto err;
								}
								x<<=8;
								x|=i;
								MWR(x,TXaddr+1+(y*2),2);
								y++;//
							}
						}
					}
					MWR(y,TXaddr,1);
					TXlen+=(1+(y*2));
					TXaddr+=(1+(y*2));
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
				case 106://���ܱ����ݷּ�����(���ݷּ����)
					y=0;//�����õĵ��ܱ����ݸ���n
					x=ADDR_AFN04F106+((5*NUM_AFN04F106_PerC)*(Pn-1));
					for(i=0;i<NUM_AFN04F106_PerC;i++)
					{
						f=MRR(x+4,1);
						if(f!=0)
						{
							if((TXlen+4+14+8+2+(1+((y+1)*5)))>LEN_UARTnTx)
							{
								goto err;
							}
							MR(TXaddr+1+(y*5),x,5);
							y++;
						}
						x+=5;
					}
					MWR(y,TXaddr,1);
					TXlen+=(1+(y*5));
					TXaddr+=(1+(y*5));
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
					
				case 150://��ѯ������״̬
					//���������յ���ѯ������״̬���F150��n���ӣ�n=0~20��һ��ȡ10���ڲ�������F10�ն˵��ܱ�/��������װ�����ò������Զ�����
					Terminal_Ram->AutoUpdataAFN04F10_S_Timer=10*60;//25 �Զ�����F10�붨ʱ��;���������յ���ѯ������״̬����(F150)n����(n=0~20,һ��ȡ10)�ڲ�������F10�ն˵��ܱ�/��������װ�����ò������Զ�����
					i=TerminalParamterLib[Fn-1].Len;
					break;
			#if (USER/100)==12//�Ĵ��û�
/*
��������AFN04 ��չF151 485�ڹ������á�����1�ֽڣ�D0~D7y���α�ʾ485I��485II����
��1����ʾ485����ͨ�ſڡ���0��ʾΪ485����ڡ�
*/
				case 151:
				AFN0AF151:
					if(Comm_Ram->Factory==0x55)
					{//0x55=����ģʽ
						i=0;
					}
					else
					{
						i=MRR(ADDR_UART_PORT_SET+LEN_UART_PORT_SET,1);
						if(i!=0)
						{
							i=1;
						}
					}
					x=MRR(ADDR_UART_PORT_SET+2*LEN_UART_PORT_SET,1);
					if(x!=0)
					{
						x=1;
					}
					i|=(x<<1);
					i=((~i)&0x3);
					MWR(i,TXaddr,1);
					TXlen+=1;
					TXaddr+=1;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
					continue;
			#endif
			#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
				case 160://���������Աȶ�
					crc = 0;
					p8d=(u8*)(ADDR_DATABUFF);
					p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//�������Pn��ӦAFN04F10�������,0Ϊ��Ч
					for(x=2;x<=AFN04F10MaxPn;x++)
			    {
						y=p16[x];//pn��ӦAFN04F10�������,0Ϊ��Ч
						if(y!=0)
						{
							y--;
							p8d[0]=x;
							p8d[1]=x>>8;
							MR(ADDR_DATABUFF+2,ADDR_AFN04F10+6+(LEN_AFN04F10_Pn*y),6);
						#if ((Project/100)==2)//������
							if(MRR(ADDR_AFN04F10+5+(LEN_AFN04F10_Pn*y),1)==21)
							{//�Ϻ���Լ
								//��3�ֽڣ����쳧���룬1�ֽ�ASCII�룬��P����ó���֡��Ϊ50H����վ�·����ն�Ϊ80H(BCD��)
								p8d[5]=hex_bcd(p8d[5]);	
							}
						#endif
							for(y=0;y<8;y++)
							{
				        crc = crc ^ (p8d[y] << 8);
				        for (i = 0; i < 8; i++)
				        {
				            if (crc & 0x8000)
				                crc = (crc << 1) ^ 0x1021;
				            else
				                crc = crc << 1;
				        }
							}
						}
			    }
					p8d[0]=crc;
					p8d[1]=crc>>8;
					MR(ADDR_DATABUFF+2,ADDR_AFN04F150,256);
					i=2+256;
					break;
			#endif
			#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
				case 296://�������ݡ��㽭����ʱ���״̬
			AFN0AF296:
					i=256;
					break;	
				case 297://�������ݡ��㽭�ն˶�ʱ����
			AFN0AF297:
					i=10;
					break;	
				case 298://�������ݡ��㽭����ʱ����
			AFN0AF298:
					i=8;
					break;
			#endif
			#if (USER/100)==11//����ϵ
				case 6://
					if(Comm_Ram->Factory==0x55)//0x55=����״̬
					{
						i=16;
					}
					else
					{
						i=TerminalParamterLib[Fn-1].Len;
					}
					break;
				case 89://
					if(Comm_Ram->Factory==0x55)//0x55=����״̬
					{
						i=TerminalParamterLib[Fn-1].Len;
					}
					else
					{
						i=6;
					}
					break;
			#endif
			#if (USER/100)==6//����ϵ
				case 200://�ն˵绰�����SIM������
			AFN0AF200:				
					i=29;
					break;
			#endif
				default:
					i=TerminalParamterLib[Fn-1].Len;
					break;
			}
			if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
			{
				goto err;
			}
		#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
			if(Pn==0x0)
			{
				if((Fn!=296)||(Fn!=297)||(Fn!=298))
				{
					MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
				}
				if(Fn==296)
				{
					MC(0,ADDR_AFN0AF296_Ext,i);
					p8s=(u8 *)(RXaddr);
					for(x=1;x<=MaxRS485AddCarrierPn;x++)
					{
						if(PnSet(x)!=0)//��Ч������ż��;����0=��Ч,1=��Ч
						{
							y=MRR(ADDR_AFN0CF250_Ext1+10*(x-1)+5,2);
							if((y>>15)!=0x0)
							{//��
								y=~y;
								y++;
							}
							if(y>=(p8s[0]+(p8s[1]<<8)))
							{
								y=MRR(ADDR_AFN0AF296_Ext+(x/8),1);
								y|=(1<<(x%8));
								MWR(y,ADDR_AFN0AF296_Ext+(x/8),1);
							}
						}
					}
					MR(TXaddr,ADDR_AFN0AF296_Ext,i);
					RXaddr+=2;
				}
				if(Fn==297)
				{
					MR(TXaddr,ADDR_AFN04F297_Ext,i);
				}
				if(Fn==298)
				{
					MR(TXaddr,ADDR_AFN04F298_Ext,i);
				}
			}
		#else
			#if ((USER/100)==13)&&((Project/100)==3)//������ר��
			if(Pn==0x0)
			{
				MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
				if(Fn==150)
				{//F150���������������
					//�ն��������ò���
					p8txbuff=(u8 *)(TXaddr);
					x=ADDR_AFN04F11+1;
					for(y=1;y<=AFN04F11MaxPn;y++)
					{
						z=MRR(x,1);
						if(z!=0)
						{
					#if AFN04F150Dn==0//AFN04F150��������Ч��־Dnλ����Ĳ������;1�Ų�����0=Dn-1,1=Dn
							z--;
					#endif
							p8txbuff[z/8]&=(~(1<<(z%8)));
						}
						x+=5;
					}
				}
			}
			#else
				#if (USER/100)==11//����
				if(Pn==0x0)
				{
					if(Fn==89)
					{
						if(Comm_Ram->Factory==0x55)//0x55=����״̬
						{
							MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
						else
						{
							MR(TXaddr,TerminalParamterLib[Fn-1].Addr,2);
							MR(TXaddr+2,ADDR_04000401,4);
						}
					}
					else
					{
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
				}
				#else
					#if (USER/100)==6//����ϵ
					if(Pn==0x0)
					{
						if(Fn==200)
						{
							if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
							{
								goto err;
							}
							//8���ֽ��ն˵绰���룬P0��P15��Ӧ�绰�����һλ����ĩλ������δ����������FH
							MC(0xFF,TXaddr,8);
							p8s=(u8 *)ADDR_MobilePhoneNumber;
							p8d=(u8 *)TXaddr;
							for(x=0;x<20;x++)
							{
								if(p8s[x]==0)
								{
									break;
								}
							}
							if(x!=0)
							{
							 	if(x>=16)
								{
									y=ASCII_BCD(ADDR_MobilePhoneNumber+8,8);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
									MWR(y,ADDR_DATABUFF,4);//д�ն˵绰����
									y=ASCII_BCD(ADDR_MobilePhoneNumber,8);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
									MWR(y,ADDR_DATABUFF+4,4);//д�ն˵绰����
									Data_Inverse(ADDR_DATABUFF,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
									MW(ADDR_DATABUFF,TXaddr,8);
								}
								else
								{//x<16
									if(x>=8)
									{
										if(x&1)
										{//����
											y=ASCII_BCD(ADDR_MobilePhoneNumber+(x-7),7);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
											y<<=4;
											MWR(y,ADDR_DATABUFF,4);//д�ն˵绰����
											y=ASCII_BCD(ADDR_MobilePhoneNumber,(x-7));//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
											MWR(y,ADDR_DATABUFF+4,(x-7)/2);//д�ն˵绰����
											Data_Inverse(ADDR_DATABUFF,(x+1)/2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
											MW(ADDR_DATABUFF,TXaddr,(x+1)/2);
											p8d[(x-1)/2]|=0x0F;
										}
										else
										{//ż��
											y=ASCII_BCD(ADDR_MobilePhoneNumber+(x-8),8);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
											MWR(y,ADDR_DATABUFF,4);//д�ն˵绰����
											y=ASCII_BCD(ADDR_MobilePhoneNumber,(x-8));//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
											MWR(y,ADDR_DATABUFF+4,(x-8)/2);//д�ն˵绰����
											Data_Inverse(ADDR_DATABUFF,x/2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
											MW(ADDR_DATABUFF,TXaddr,x/2);
										}
									}
									else
									{//x<8
										if(x&1)
										{//����
											y=ASCII_BCD(ADDR_MobilePhoneNumber,x);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
											y<<=((8-x)*4);
											MWR(y,ADDR_DATABUFF,(x+1)/2);//д�ն˵绰����
											if(((x+1)/2)>=2)
											{
												Data_Inverse(ADDR_DATABUFF,(x+1)/2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
											}
											MW(ADDR_DATABUFF,TXaddr,(x+1)/2);
											p8d[(x-1)/2]|=0x0F;
										}
										else
										{//ż��
											y=ASCII_BCD(ADDR_MobilePhoneNumber,x);//16���Ƶ�ASCII��תΪBCD,��"12AB"->0x12AB,���ASCII�뿪ʼ��ַ��ASCII���ֽ���,����=0xFFFFFFFF��ʾ����ASCII���0-9,A-Fֵ
											MWR(y,ADDR_DATABUFF,x/2);//д�ն˵绰����
											if((x/2)>=2)
											{
												Data_Inverse(ADDR_DATABUFF,x/2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
											}
											MW(ADDR_DATABUFF,TXaddr,x/2);
										}
									}
								}
							}
							//SIM������(SM0~SM19��ӦSIM�����ŵ�һλ����ĩλ����ASCII�룬��ĸҪ��д)
							MR(TXaddr+8,ADDR_AFN09F9+26,20);
							p8d=(u8 *)(TXaddr+8);
							for(x=0;x<20;x++)
							{
								if((p8d[x]>=0x61)&&(p8d[x]<=0x7a))
								{//0x61~0x7a��ӦСд��ĸa~z��0x41~0x5a��Ӧ��д��ĸA~Z
									p8d[x]-=0x20;
								}
							}
							k=Terminal_Ram->SignaldBm;
							if(k==0x99)
							{
								k=0;
							}
							k=bcd_hex(k);//8λѹ��BCD��תΪHEX
							MWR(k,TXaddr+8+20,1);
 						}
						else
						{
							MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
					}				
					#else
					if(Pn==0x0)
					{
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
					#endif
				#endif
			#endif
		#endif	
			else
			{
				MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*i),i);
			}
			FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn���Ϊ���ݱ�ʶ
			TXlen+=i;
			TXaddr+=i;
			if(i!=0x0)
			{
				f=1;
			}
		}
	}
	if(TXlen==0x0)
	{
		goto err;
	}
	TXlen+=8;
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}













