
//��ʱ����
#include "Project.h"
#include "Terminal_Uart_TimerTask.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"


extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);



void Terminal_Uart_AutoTask(void)//�����ϱ���ʱ����
{
	Terminal_TimerTask(GPRSPORT);//�����ϱ���ʱ����,���UART�˿ں�
	Terminal_TimerTask(ENETPORT);//�����ϱ���ʱ����,���UART�˿ں�
#if RS232AutoRePort==1//RS232�������ϱ�0=��ֹ,1=����
	Terminal_TimerTask(RS232PORT);//�����ϱ���ʱ����,���UART�˿ں�
#endif
}


const u8 ClassData_Example[]=
{
	0x68,
	0,0,
	0,0,
	0x68,
	0x00,//������(DIR=0����,������־λPRM=0,��������4=���ͨM�޻ش�,����Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1)10=����M��Ӧ)
	0,0,//����������
	0,0,//�ն˵�ַ
	0,//����վ��ַ(�ն���������֡��MSAӦΪ��)
	0x00,//AFN
	0x60,//SEQ=TpV,FIR,FIN,CON;PSEQ|RSEQ CON��=0
	//DI
	//cs
	//0x16
};




void Terminal_TimerTask(u32 PORTn)//�����ϱ���ʱ����,���UART�˿ں�
{
	u32 i;
	u32 x;
	u32 y;
	u32 Fn;
	u32 TaskCount;
	u32 t;
	u32 ReTxFlag;
	u64 YMDHM;
	u64 YMDHMS;
	
	u8 * p8;
	u16 * p16;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;


	if(PORTn==GPRSPORT)
	{
		if(UART1Ctrl->ATTask!=0)//AT����:0=����,1=����,2=���ڷ���,3=����
		{
			return;
		}
	}
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
  p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	if(UARTCtrl->Lock==0x55)
	{
		return;
	}
	//�����ϱ���ʱ��������ɶ�������
	if(Terminal_Ram->CongealFlags==0x1f)//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	{
		if(DataComp(((u32)Terminal_Ram->LastCongealTime),ADDR_TYMDHMS+1,5))//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		{//������ʱ�ָ���
			return;//��ʱ����Ͷ����첽����,���ڴ��ֽ����ж�
		}
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
		{
			goto LoopChack;//����Ѳ�������ϱ�
		}
		if(UARTCtrl->Task==0x0)
		{//����
			p16=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			if(p16[0]==0x0)
			{
				if(PORTn==RS232PORT)
				{
					goto LoopChack;
				}
				if(UARTCtrl->LinkTask==101)//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
				{//����
LoopChack:
					if(PORTn==GPRSPORT)
					{
						p16ACKTimer=(u16 *)((u32)&Terminal_Ram->GPRS_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
					}
					if(PORTn==ENETPORT)
					{
						p16ACKTimer=(u16 *)((u32)&Terminal_Ram->ENET_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
					}
					if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
					{
						p16ACKTimer=(u16 *)((u32)&Terminal_Ram->RS232_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
					}
					if(UARTCtrl->HostACK!=0x0)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					{	
						if(UARTCtrl->HostACK!=5)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
						{//�����������ϱ��ȴ�ȷ�Ϸ���
							return;
						}
						if(p16ACKTimer[0]!=0x0)
						{
							return;//�ȴ���վȷ���붨ʱ��!=0
						}
						UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
						i=MRR(ADDR_AFN04F1+3,1);
						i>>=4;
						i&=0x3;
						if(UARTCtrl->HostCount>i)
						{//�ط�������
							UARTCtrl->HostCount=0;//�����ϱ��ط�����
						}
						else
						{//�ط��ϼ�������
							if(UARTCtrl->TimerTaskCount!=0x0)
							{
								UARTCtrl->TimerTaskCount--;//��ʱ����ֲ�����
							}
							else
							{
								UARTCtrl->TimerTaskCount=(2*MaxTimerTask)-1;
							}
						}
					}
					if(MRR(ADDR_AFN05F29,1)!=0x55)
					{//�������ն������ϱ�
						UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
						UARTCtrl->HostCount=0;//�����ϱ��ط�����
						return;
					}
					//����֡����
					if(PORTn==GPRSPORT)
					{
						if(Terminal_Ram->GPRSHeartBeat_S_Timer==0)//5 GPRS�����붨ʱ��
						{
							return;
						}
					}
					if(PORTn==ENETPORT)
					{
						if(Terminal_Ram->ENETHeartBeat_S_Timer==0)//2 ��̫�������붨ʱ��
						{
							return;
						}
					}
					//�ϴ�
					TaskCount=UARTCtrl->TimerTaskCount;
					if(TaskCount>=(2*MaxTimerTask))
					{
						TaskCount=0;
					}
					if(MRR(ADDR_AFN04F67+TaskCount,1)==0x55)
					{//����
						if(MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+8,1)!=0x0)
						{//���ݵ�Ԫ��ʶ����!=0
							//ȡ��׼ʱ��
							MR(ADDR_DATABUFF,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+1,6);
							//ȡ�ϴ��ϱ���¼ʱ��
							if(PORTn==GPRSPORT)
							{
								MR(ADDR_DATABUFF+6,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
							}
					#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
							if(PORTn==ENETPORT)
							{
								MR(ADDR_DATABUFF+6,ADDR_ENETTimerTaskNotes+(6*TaskCount),6);
							}
					#else
							if(PORTn==ENETPORT)
							{
								MR(ADDR_DATABUFF+6,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
							}
					#endif		
							if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
							{
								MR(ADDR_DATABUFF+6,ADDR_RS232TimerTaskNote+(6*TaskCount),6);
							}
							
							p8=(u8 *)(ADDR_DATABUFF);
							if((p8[4]&0xe0)!=0x0)
							{//376.1���������׼ʱ�����ֽڰ���������
								p8[4]&=0x1f;
								MW(ADDR_DATABUFF,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+1,6);
							}
							if(DataComp(ADDR_TYMDHMS,ADDR_DATABUFF,6)<2)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
							{//��ǰʵʱ��>=���ͻ�׼ʱ��
								i=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount),1);//��ʱ����1��������������(�����)
								t=i&0x3f;//��ʱ�ϱ����ݵ�ʱ������
								if(t!=0x0)
								{//��ʱ�ϱ����ݵ�ʱ������!=0
									ReTxFlag=UARTCtrl->HostCount;
									if(ReTxFlag==0x0)
									{//�����ط�
										i>>=6;
										if(i!=3)
										{//��ʱ�ϱ����ڵ�λ=�֡�ʱ����
											switch(i)//��ʱ�ϱ�����
											{
												case 0://��
													break;
												case 1://ʱ
													t*=60;//��ʱ�ϱ����ݵ�ʱ������(ͳһΪ��)
													break;
												case 2://��
													t*=(60*24);//��ʱ�ϱ����ݵ�ʱ������(ͳһΪ��)
													break;
											}
											i=TimeDifference(ADDR_DATABUFF,ADDR_TYMDHMS);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
											if((i&0x80000000)!=0x0)
											{//��ֵ
												i=~i;
												i++;
											}
											i/=60;//�ӻ�׼ʱ�䵽��ǰʵʱ�ӵ����ʱ���
											i/=t;//������������
											t*=i;//���ζ�ʱ�ϱ�ʱ��=��׼ʱ��+�����
											YMDHMS=MRR(ADDR_DATABUFF+1,5);
											YMDHMS=YMDHM_AddM(YMDHMS,t);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
											MWR(YMDHMS,ADDR_DATABUFF+1,5);
											if(DataComp(ADDR_DATABUFF,ADDR_DATABUFF+6,6)==0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
											{//��ͬʱ�����ϱ�
												goto No_Task;
											}
										}
										else
										{//��ʱ�ϱ����ڵ�λ=��
											if(DataComp(ADDR_TYMDHMS+2,ADDR_DATABUFF+2,2)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
											{//��ǰʵʱ��ʱ��!=���ͻ�׼ʱ��ʱ��
												goto No_Task;
											}
											//��ǰʵʱ�ӵ����¶�0��1�µ�����
											p8=(u8 *)(ADDR_TYMDHMS);
											i=bcd_hex(p8[5]);
											i*=12;
											i+=bcd_hex(p8[4]);
											//��׼ʱ������¶�0��1�µ�����
											p8=(u8 *)(ADDR_DATABUFF);
											x=bcd_hex(p8[5]);
											x*=12;
											x+=bcd_hex(p8[4]);
											if(i<x)
											{
												goto No_Task;
											}
											i-=x;
											i/=t;
											t*=i;//���ζ�ʱ�ϱ�ʱ��=��׼ʱ��+�����
											p8[4]=bcd_hex(p8[4]);
											p8[5]=bcd_hex(p8[5]);
											for(;t!=0x0;t--)
											{
												p8[4]++;
												if(p8[4]>=13)
												{
													p8[4]=1;
													p8[5]++;
													if(p8[5]>=100)
													{
														p8[5]=0;
													}
												}
											}
											p8[4]=hex_bcd(p8[4]);
											p8[5]=hex_bcd(p8[5]);
											if(DataComp(ADDR_DATABUFF,ADDR_DATABUFF+6,6)==0X0)
											{//��ͬʱ�����ϱ�
												goto No_Task;
											}
										}
									}
									//ȡ��ǰ�ϱ�ʱ������2������
									if(ReTxFlag==0x0)
									{//�����ط�
										YMDHMS=MRR(ADDR_DATABUFF,6);//��ǰ�ϱ�ʱ��Ĵ�
									}
									else
									{//���ط�
										if(PORTn==GPRSPORT)
										{
											YMDHMS=MRR(ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);//�ϴζ�ʱ�ϱ�ʱ��
										}
								#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
										if(PORTn==ENETPORT)
										{
											YMDHMS=MRR(ADDR_ENETTimerTaskNotes+(6*TaskCount),6);//�ϴζ�ʱ�ϱ�ʱ��
										}
								#else
										if(PORTn==ENETPORT)
										{
											YMDHMS=MRR(ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);//�ϴζ�ʱ�ϱ�ʱ��
										}
								#endif
										if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
										{
											YMDHMS=MRR(ADDR_RS232TimerTaskNote+(6*TaskCount),6);//�ϴζ�ʱ�ϱ�ʱ��
										}
									}
									MR((u32)p8rxbuff,(u32)ClassData_Example,14);
									MR(((u32)p8rxbuff)+7,ADDR_AREACODE,4);//������,�ն˵�ַ
									p8rxbuff[13]|=(UARTCtrl->TxSEQ&0xf);//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
									if(TaskCount<MaxTimerTask)
									{//��1����
										i=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+8,1);
										i*=4;//DI�ֽ���
										MR(((u32)p8rxbuff)+14,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9,i);//ȡDI
										i+=8;
										p8rxbuff[1]=i;//�޸���Ϊ����֡����L
										p8rxbuff[2]=i>>8;
										MW((u32)p8rxbuff,(u32)p8txbuff,30);//COPY 14+16�ֽ�����ͷ��������,��Ϊ����ͷ

										UARTCtrl->TxFrameCount=0;//�Ĵ��֡����ʱ�ѷ���֡����
										UARTCtrl->MulFrameGPCR=0;//��֡ʱͨ�ü����Ĵ���
										UARTCtrl->LockFlags|=(1<<4);//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B5-B7����
										p8rxbuff[12]=0x0c;//Ӧ�ò㹦����AFN
										
										UARTCtrl->HostCount+=1;//�ط�����
										Uart_3761_Data(PORTn);//UART_376����
										if(p8txbuff[12]!=0x0C)
										{//û�ϱ�����
											UARTCtrl->Task=0;
											UARTCtrl->RxByte=0;//0=����
											UARTCtrl->TxByte=0;//�跢���ֽ���=0
											goto No_Task;
										}
									}
									else
									{//��2����
										x=0;
										for(i=0;i<MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+8,1);i++)
										{
											Fn=BitLoca(MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9+(4*i)+2,1));
											Fn+=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9+(4*i)+3,1)*8;
											if((Fn>=Get_Class2Data_MaxFn())||(Fn==0x0))
											{//Fn��Чʱ��װ��,��ʱ�곤�Ȳ���֪
												continue;
											}
											MR(((u32)p8rxbuff)+14+x,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+9+(4*i),4);//ȡDI
											x+=4;
											switch(Terminal_Class2DataLib[Fn].CongealType)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
											{
												case 0://�ն���(�ϱ���1��)
													t=YMD_Sub1D(YMDHMS>>24);//�����ռ�1��,���ؼ�1���������
													MWR(t,((u32)p8rxbuff)+14+x,3);
													x+=3;
													break;
												case 1://�����ն���
													t=YMDHMS>>24;
													MWR(t,((u32)p8rxbuff)+14+x,3);
													x+=3;
													break;
												case 2://�¶���(�ϱ���1��)
													t=YM_Sub1M(YMDHMS>>32);//���¼�1��,���ؼ�1�������
													MWR(t,((u32)p8rxbuff)+14+x,2);
													x+=2;
													break;
												case 3://����
//-----------------------------------------------------------------------
/*
													//�̶��ϱ���1�����յ���
													t=YMD_Sub1D(YMDHMS>>24);//�����ռ�1��,���ؼ�1���������
													MC(0,((u32)p8rxbuff)+14+x,2);//ʱ��==00:00
													MWR(t,((u32)p8rxbuff)+14+x+2,3);
													//���߶�����ʱ��*��ȡ���ݱ���
													t=CurveCongealTime*MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+7,1);
													switch(t)
													{
														case 15:
															Fn=1;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 30:
															Fn=2;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 60:
															Fn=3;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 5:
															Fn=254;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 1:
															Fn=255;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														default:
															t=CurveCongealTime;
															switch(t)
															{
																case 15:
																	Fn=1;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 30:
																	Fn=2;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 60:
																	Fn=3;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 5:
																	Fn=254;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 1:
																	Fn=255;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
															}
															break;
													}
													MC(hex_bcd(t),((u32)p8rxbuff)+14+x,1);//��ʼʱ���Ϊ��1�����ݵ�ķ�
													MC(Fn,((u32)p8rxbuff)+14+x+5,1);//���ݶ����ܶ�m
													t=(24*60)/t;//��1�����
													MC(t,((u32)p8rxbuff)+14+x+6,1);//�����ݵ���
													x+=7;
*/
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
													//����ʱ���������ڵ����ϱ�
													y=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount),1);//��ʱ����1��������������(�����)
													t=y&0x3f;//��ʱ�ϱ����ݵ�ʱ������
													y>>=6;
													switch(y)//��ʱ�ϱ�����
													{
														case 0://��
															break;
														case 1://ʱ
															t*=60;//��ʱ�ϱ����ݵ�ʱ������(ͳһΪ��)
															break;
														case 2://��
															t*=(60*24);//��ʱ�ϱ����ݵ�ʱ������(ͳһΪ��)
															break;
														case 3://��
															t=YMDHMS>>32;
															t=YM_Sub1M(t);//���¼�1��,���ؼ�1�������
															t=YM_D(t);//ȡָ�����µ�����
															t*=(60*24);//��ʱ�ϱ����ݵ�ʱ������(ͳһΪ��)
															break;
													}
													YMDHM=YMDHM_SubM(YMDHMS>>8,t);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
										#if (USER/100)==2//�û���ʶ:ɽ����
													if((t%60)==0)
													{//��ʱ�ϱ�����=60�ֵ�������
														//�ϱ������㶳�����������,��ǰ���㶳�����ݻ�û��
														YMDHM>>=8;
														YMDHM<<=8;
													}
										#endif										
													//���߶�����ʱ��*��ȡ���ݱ���
													y=CurveCongealTime*MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*TaskCount)+7,1);
													switch(y)
													{
														case 15:
															Fn=1;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 30:
															Fn=2;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 60:
															Fn=3;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 5:
															Fn=254;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														case 1:
															Fn=255;//Ҫ��ȡ���ߵ������ܶ�m
															break;
														default:
															y=CurveCongealTime;
															switch(y)
															{
																case 15:
																	Fn=1;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 30:
																	Fn=2;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 60:
																	Fn=3;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 5:
																	Fn=254;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
																case 1:
																	Fn=255;//Ҫ��ȡ���ߵ������ܶ�m
																	break;
															}
															break;
													}
													if((t%y)==0)
													{
														t/=y;//���ϴ�����
													}
													else
													{
														t/=y;//���ϴ�����
														t++;
													}
													if(t>96)
													{//���ֻ�����96��
														YMDHM=YMDHM_AddM(YMDHM,(t-96)*y);
														t=96;
													}
													if(t==0)
													{
														t=1;//����1����
													}
													MWR(YMDHM,((u32)p8rxbuff)+14+x,5);
													MWR(Fn,((u32)p8rxbuff)+14+x+5,1);//���ݶ����ܶ�m
													MWR(t,((u32)p8rxbuff)+14+x+6,1);//�����ݵ���
													x+=7;
//-----------------------------------------------------------------------
													break;
											}
										}
										x+=8;
										p8rxbuff[1]=x;//�޸���Ϊ����֡����L
										p8rxbuff[2]=x>>8;
										MW((u32)p8rxbuff,(u32)p8txbuff,30);//COPY 14+16�ֽ�����ͷ��������,��Ϊ����ͷ

										UARTCtrl->TxFrameCount=0;//�Ĵ��֡����ʱ�ѷ���֡����
										UARTCtrl->MulFrameGPCR=0;//��֡ʱͨ�ü����Ĵ���
										UARTCtrl->LockFlags|=(1<<5);//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B5-B7����
										p8rxbuff[12]=0x0d;//Ӧ�ò㹦����AFN
										
										UARTCtrl->HostCount+=1;//�ط�����
										Uart_3761_Data(PORTn);//UART_376����
										if(p8txbuff[12]!=0x0D)
										{//û�ϱ�����
											UARTCtrl->Task=0;
											UARTCtrl->RxByte=0;//0=����
											UARTCtrl->TxByte=0;//�跢���ֽ���=0
											goto No_Task;
										}
									}
									//�޸��ϴζ�ʱ�ϱ�ʱ��
									if(ReTxFlag==0x0)
									{//�����ط�
										if(PORTn==GPRSPORT)
										{
											MWR(YMDHMS,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
										}
								#if GPRSENETAutoRePort==0//GPRS����̫���������ϱ�0=�ֿ������ϱ�,1=�ϲ��ϱ�
										if(PORTn==ENETPORT)
										{
											MWR(YMDHMS,ADDR_ENETTimerTaskNotes+(6*TaskCount),6);
										}
								#else
										if(PORTn==ENETPORT)
										{
											MWR(YMDHMS,ADDR_GPRSTimerTaskNotes+(6*TaskCount),6);
										}
								#endif
										if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
										{
											MWR(YMDHMS,ADDR_RS232TimerTaskNote+(6*TaskCount),6);
										}
									}
									//UARTCtrl->TxSEQ+=1;//����֡������SEQ+1;(������1��2��UART���������м�1)
									i=MRR(ADDR_AFN04F1+2,2);//�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��
									i&=0xfff;
									if(i<WaitHostACKMinTimer)
									{
										i=WaitHostACKMinTimer;//�����ϱ��ȴ���վȷ����С��ʱ��
									}
									if(i>WaitHostACKMaxTimer)
									{
										i=WaitHostACKMaxTimer;//�����ϱ��ȴ���վȷ�����ʱ��
									}
									p16ACKTimer[0]=i;//�����ϱ��ȴ���վȷ���붨ʱ
									TaskCount++;//�������+1
									UARTCtrl->TimerTaskCount=TaskCount;
									return;
								}
							}
						}
					}
	No_Task:
					UARTCtrl->LockFlags&=~(3<<4);//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=����1�ඨʱ����,B5=����2�ඨʱ����,B6=�ٶ�1�ඨʱ����,B7=����
					UARTCtrl->HostACK=0x0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
					UARTCtrl->HostCount=0;//�����ϱ��ط�����
					TaskCount++;//�������+1
					UARTCtrl->TimerTaskCount=TaskCount;
				}
			}
		}
	}
}

void Uart_376_CON(u32 PORTn)//������ʱ����CON��־λ�͹�����
{
	u32 i;
	u8 * p8txbuff;
	u16 * p16ACKTimer;
  UARTCtrl_TypeDef * UARTCtrl;

  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	if(UARTCtrl->LockFlags&0x30)//ͨ�Ź��ܱ���ֹ��־:B0=����,B1=�����Զ��ѱ�,B2=�������Զ��ѱ�,B3=����,B4=������ʱ����1������,B5=������ʱ����2������,B5-B7����
	{//������ʱ����
		p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
		i=MRR(ADDR_AFN04F1+4,1);
		i&=0x03;		
		if(i)
		{//��Ҫ��վȷ�ϵ�ͨ�ŷ���CON=1���ı�־
			p8txbuff[6]&=0xf0;
			p8txbuff[6]|=0xCA;//������DIR=1����,������־λPRM=0,Ҫ�����λACD��=0;������10=����M��Ӧ֡(����1������)
			p8txbuff[13]|=0x10;//CON=1
			UARTCtrl->HostACK=5;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
			if(PORTn==GPRSPORT)
			{
				p16ACKTimer=(u16 *)((u32)&Terminal_Ram->GPRS_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
			}
			if(PORTn==ENETPORT)
			{
				p16ACKTimer=(u16 *)((u32)&Terminal_Ram->ENET_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
			}
			if((PORTn==RS232PORT)||(PORTn==RS485_1PORT)||(PORTn==RS485_2PORT)||(PORTn==RS485_3PORT))
			{
				p16ACKTimer=(u16 *)((u32)&Terminal_Ram->RS232_WaitHostACK_S_Timer);//6 �����ϱ��ȴ���վȷ���붨ʱ��
			}
			i=MRR(ADDR_AFN04F1+2,2);//�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��
			i&=0xfff;
			if(i<WaitHostACKMinTimer)
			{
				i=WaitHostACKMinTimer;//�����ϱ��ȴ���վȷ����С��ʱ��
			}
			if(i>WaitHostACKMaxTimer)
			{
				i=WaitHostACKMaxTimer;//�����ϱ��ȴ���վȷ�����ʱ��
			}
			p16ACKTimer[0]=i;//�����ϱ��ȴ���վȷ���붨ʱ
		}
		else
		{
			p8txbuff[6]&=0xf0;
			p8txbuff[6]|=0xC4;//������DIR=1����,������־λPRM=0,Ҫ�����λACD��=0;������4=���ͨM�޻ش�
			UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����
		}
	}
}


