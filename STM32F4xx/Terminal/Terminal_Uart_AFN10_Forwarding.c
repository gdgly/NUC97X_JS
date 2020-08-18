
//����ת���������
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../Device/IC_BUZZ.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"


	#if(USER/100)==0
#define MaxReRMCount     2//1//�����ط�����(���ط�����,�����ʼ1�εķ���);�������ز�=1,΢��������=2
	#else
#define MaxReRMCount     0//�����ط�����(���ط�����,�����ʼ1�εķ���)
	#endif


void Forwarding(u32 PORTn)//����ת���������
{
	u32 i;
	u32 Fn;
	u32 DIaddr;
	u32 n;
	u32 DI;
	u64 CommAddr;
	u32 PORTRS485;
	u32 x;
	u32 y;
	u32 z;
	u32 Protocol;
	
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u16 * p16;
	u32 * p32;
#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
	u64 d64;
//	#if (USER/100)==0//���Ժ����
//	u8 *p8d;
//	#endif
#endif
	

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//�õ�UART��������ַ
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	
#if (USER/100)==0//���Ժ����
//�ز�ͨ������������

#else
 #if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
//	if(Terminal_Ram->PowerTask<=10)//ͣ���¼�����
//	{
//		if(Comm_Ram->SelfTestCommand!=0x55555555)//�Բ�������0x55555555����1(Ӳ��),0xAAAAAAAA����2(FLASH),0x5555AAAA����3(FLASH��������)
//		{
//			goto err;
//		}
//	}
 #endif
#endif//���Ժ����

	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
	Fn=(i>>16);
//if(p8rxbuff[18]==2)
//{
//	p8rxbuff[18]=31;//��ģ��̨�����ز���ESAM�ڲ�������
//}
	switch(p8rxbuff[18])
	{
		case 1://�ڲ����������ӿ�
			PORTRS485=ACSAMPLEPORT;
		case 2://RS485-1
			PORTRS485=RS485_1PORT;
			break;
		case 3://RS485-2
			PORTRS485=RS485_2PORT;
			break;
		case 4://RS485-3
			PORTRS485=RS485_3PORT;
			break;
		case 31://�˿ں�=31,�ز�
			PORTRS485=RS485_4PORT;
			break;
		default:
			UARTCtrl->Task=0;
			return;
	}
	switch(Fn)
	{
		case 1://F1͸��ת��
			switch(UARTCtrl->Task)
			{
				case 8://8=�ȴ�ͨ������������ת��
					switch(p8rxbuff[18])
					{
						case 1://�ڲ����������ӿ�
							p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//ת���ֽ���
							p8=(u8 *)((u32)p8rxbuff+18+6);
							//��Ϊ��ȷ֡����ǰ��0xfeȥ��,��0x68��ʼ
							while(i!=0x0)
							{
								if(p8[0]==0x68)
								{
									break;
								}
								p8++;
								i--;
							}
							if(i!=0x0)
							{
								if(i>Get_LEN_UARTnRx(ACSAMPLEPORT))
								{//ת������̫����
									goto err;
								}
								MW((u32)p8,Get_ADDR_UARTnRx(ACSAMPLEPORT),i);
								MC(0xaa,Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,6);//ͨ�ŵ�ַ��0xaa
								i=UARTCtrlRS485->Task;//ԭ����Ĵ�
								UARTCtrlRS485->Task=1;//1=�������
							  if(FileDownload->File&0x04)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
							  {
									p32=(u32 *)(ADDR_Meter_ENTRY);
							    (*(void(*)())p32[130])((u32)ACSAMPLEPORT);//����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
								}
								UARTCtrlRS485->Task=i;//ԭ����ָ�
							}
							i=UARTCtrlRS485->TxByte;//�����ֽ���
//							if(i==0x0)
//							{//û�յ�����
//								goto err;
//							}
							MW(Get_ADDR_UARTnTx(ACSAMPLEPORT),((u32)p8txbuff)+21,i);
							p8txbuff[19]=i;
							p8txbuff[20]=i>>8;
							i+=7;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,2�ֽڳ���
							i+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
							p8txbuff[1]=i;
							p8txbuff[2]=i>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
							break;
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							if(p16timer[0]==0)//�ȴ�����ת���ڿ��ж�ʱ5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
								if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
								{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrl->ReRMCount=0;//�ط�����
									UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ
								}
							}
							break;
#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
						case 31://�˿ں�=31,�ز�
							//�˿ں�=31,ת��ֻ�ܶ�DL645-1997��2007����
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//������(��ʱ����·��ģ���������,��ÿ֡����OKʱ��ʼ��Ϊ0)
							{
								case 0://�ȴ��ڿձ�ʱ����
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
									DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
									if(p16[0]!=p16[1])
									{//�ѽ��յ���֡
										goto err;
									}
								#endif
									if(p16timer[0]==0)//�ȴ�����ת���ڿ��ж�ʱ120S
									{
										goto err;
									}
									if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
									{
										break;
									}
									if(Terminal_Router->UpDateTask!=0)//�����������
									{
										p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)	
										goto err;
									}
									if(UARTCtrlRS485->Lock==0x55)
									{
										goto err;//�����ڱ�ռ��
									}
									else
									{//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
										{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
											p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
											UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
											UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
										}
									}
									break;
								case 1://��ͣ·��
									if(Terminal_Router->RouterCtrl!=0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//��ͣ·�ɱ�־
									Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
									break;
								case 2://��ͣ·�ɽ���
									if(Terminal_Router->RouterCtrl==0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									{
										UARTCtrl->ReRMCount=0;//�ط�����
										UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
									}
									break;
								case 3://·��ת������
									x=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//ת���ֽ���
									if((x<14)||(x>(128-2)))
									{//DL645-1997��С14�ֽ�,DL645-2007��С16�ֽ�,���ת��(128-2)�ֽ�
										Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
										Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
										UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										goto err;
									}
									y=0;//��ǰ��0xfe����
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									if((p8rxbuff[18+6+y]!=0x68)||(p8rxbuff[18+6+y+7]!=0x68))
									{//����DL645��׼֡
										Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
										Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
										UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										goto err;
									}
									for(i=0;i<6;i++)
									{
										if(p8rxbuff[18+6+y+1+i]!=0x99)
										{
											break;
										}
									}
									if(i==6)
									{//�ǹ㲥����
										UARTCtrl->SubTask=5;//������(��ʱ����·��ģ������)
										break;
									}
								#if(USER/100)==7//�㽭�û�
									if(p8rxbuff[18+6+y+8]==0x08)
									{//��ָ�����ַУʱ����
										UARTCtrl->SubTask=10;//������(��ʱ����·��ģ������)
										break;
									}
								#endif
//								#if(USER/100)==0//��������
//									if(p8rxbuff[18+6+y+8]==0x14)
//									{//д����
//										i=MRR(((u32)p8rxbuff)+18+6+y+8+2,4);
//										if((i==0x3733343F)||(i==0x37333435))
//										{
//											UARTCtrl->SubTask=10;//������(��ʱ����·��ģ������)
//											break;
//										}
//									}
//								#endif
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
									{
										p8[3]=0x43;//������;����ʽ·���ز�ͨ��
									}
									else
									{
										if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
										{
											p8[3]=0x4a;//������;΢��������ͨ��
										}
										else
										{
											p8[3]=0x41;//������;����ʽ·���ز�ͨ��
										}
									}
									p8[4]=4;//ͨ��ģ���ʶ=1
									p8[5]=0;
									p8[6]=0;//Ԥ��Ӧ���ֽ���
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
									p8[9]=UARTCtrlRS485->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
									MR(((u32)p8)+16,((u32)p8rxbuff)+18+6+y+1,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[18+6+y+8];//ȡ������
									if((i&0x10)==0x10)
									{
										if(i==0x10)
										{//1997�������������
											p8[25]=1;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
										}
										else
										{
											p8[25]=2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
										}
									}
									else
									{
										p8[25]=1;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
									}
									p8[26]=0;//ͨ����ʱ��ر�־
									p8[27]=0;//�����ڵ���=0
									
									p8[28]=x-y;//DL645���ĳ���(ǰ��0xfe������)
									MR(((u32)p8)+29,((u32)p8rxbuff)+18+6+y,x-y);//ת������(ǰ��0xfe������)
									
									i=29+p8[28]+2;//���ܷ����ֽ���
									p8[1]=i;
									p8[2]=i>>8;
									if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
									{//΢��������,������ַ�òɼ�����ַ
										CommAddr=MRR(((u32)p8)+16,6);
										x=Check_AFN04F10_Addr(31,CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
										if(x!=0)
										{
											d64=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
											if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
											{
												MWR(d64,((u32)p8)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
											}
										}
									}
									Uart_3762_CS((u32)p8);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
									UARTCtrlRS485->TxByte=i;//�����ֽ���(���ֶ���)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
									CopyString((u8 *)"�ز�F1  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(18+6+y+1+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ�?
									break;
								case 4://ת������
									if(UARTCtrlRS485->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=p8[1]+(p8[2]<<8);
									if((UARTCtrlRS485->Task!=1)||(i<=16))
									{//��ʱ����ϻش�
										y=0;//ת���������ݳ���
									}
									else
									{
										y=p8[28];//����645֡�ܳ���
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											UARTCtrlRS485->Task=5;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
											UARTCtrlRS485->RxByte=0;//�����ֽ���(���ֶ���)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(i==0x0200)
											{//AFN=00,Fn=2ֱ�ӷ���֡(΢���������д����)
												p16[0]=6000/10;//6�볬ʱʱ��
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
											}
											return;
										}
										//��ַ�Ƚ�
										//�鵱ǰ����ǰ��0xfe����
										for(i=0;i<14;i++)
										{
											if(p8[29+i]!=0xfe)
											{
												break;
											}
										}
										i=DataComp(((u32)p8)+29+1+i,Get_ADDR_UARTnTx(PORTRS485)+29+1,6);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
										if(i!=0)
										{//����ͬ
											UARTCtrlRS485->Task=5;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
											UARTCtrlRS485->RxByte=0;//�����ֽ���(���ֶ���)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(y==0)
											{//û�յ�����
												p16[0]=6000/10;//6�볬ʱʱ��
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
											}
											return;
										}
									}
									if(y==0x0)
									{//û�յ�����,�ز�·�������ݷ��ؼ�Ϊ��׼645֡�����鷵��֡�Ϸ���
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//�ط�����
										{
										#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
											p16=(u16 *)Get_ADDR_UARTn(PORTn);
											DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
											if(p16[0]==p16[1])
											{//û���յ���֡
												UARTCtrl->ReRMCount++;//�ط�����
												UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
												return;
											}
										#else
											UARTCtrl->ReRMCount++;//�ط�����
											UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
											return;
										#endif
										}
									#endif
									}
						BroadcastESC:
									Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
									Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									UARTCtrlRS485->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									MW(((u32)p8)+28+1,((u32)p8txbuff)+21,y);
									p8txbuff[19]=y;
									p8txbuff[20]=y>>8;
									y+=7;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,2�ֽڳ���
									y+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
									p8txbuff[1]=y;
									p8txbuff[2]=y>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
									Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
									break;
								case 5://�ز��㲥ת��
									y=0;//��ǰ��0xfe����
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									if(p8rxbuff[18+6+y+8]==0x08)
									{//�ǹ㲥Уʱ
										UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
									i=p8rxbuff[18+6+y+8];//ȡ������
									if((i&0x10)==0x10)
									{
										if(i==0x10)
										{//1997�������������
											p8[0]=1;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
										}
										else
										{
											p8[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
										}
									}
									else
									{
										p8[0]=1;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
									}
									i=p8rxbuff[18+4];
									p8[1]=i;//���ĳ���
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,i);
									Uart_3762_TxData(PORTRS485,0x05,3,2+i);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
									UARTCtrl->SubTask=8;//������(��ʱ����·��ģ������)
									if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
									{
										Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//���㲥��ʱʱ����(���ֶ���)
									}
									WarningOccur(PORTRS485,(u8 *)"�ز�ת���㲥����");//�澯����,��ڸ澯����͸澯�ַ���
									break;
								case 6://�㲥Уʱ.ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
									x=p8rxbuff[18+4];//ת���ֽ���
									if(x>(128-2))
									{
										x=(128-2);
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
								#if (USER/100)<=1
									p8[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
								#else
									p8[0]=0;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
								#endif
									p8[1]=x;//���ĳ���
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,x);
									Uart_3762_TxData(PORTRS485,0x03,9,2+x);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)	
									WarningOccur(PORTRS485,(u8 *)"�ز�ת���㲥Уʱ");//�澯����,��ڸ澯����͸澯����
									break;
								case 7://�㲥Уʱ.����ʱ����ѯ,���㲥Уʱ
									if(UARTCtrlRS485->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=MRR(((u32)p8)+10,3);
									if((i!=0x010103)||(UARTCtrlRS485->Task!=1))
									{
										Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
										Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
										UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										goto err;
									}
									i=MRR(((u32)p8)+13,2);//�㲥ͨ���ӳ�ʱ����
									y=0;//��ǰ��0xfe����
									for(x=0;x<14;x++)
									{
										if(p8rxbuff[18+6+x]!=0xfe)
										{
											break;
										}
										y++;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
								#if (USER/100)<=1
									p8[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
								#else
									p8[0]=0;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
								#endif
									p8[1]=y+18;//���ĳ���
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,y+18);
									for(x=0;x<6;x++)//ԭ�㲥ʱ���0x33
									{
										p8[2+y+10+x]-=0x33;
									}
									d64=MRR(((u32)p8)+2+y+10,6);
									d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
									MWR(d64,((u32)p8)+2+y+10,6);
									for(i=0;i<6;i++)
									{
										p8[2+y+10+i]+=0x33;
									}
									x=0;
									for(i=0;i<16;i++)
									{
										x+=p8[2+y+i];
									}
									p8[y+18]=x;
									p8[y+19]=0x16;
									Uart_3762_TxData(PORTRS485,0x05,3,y+20);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
									if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
									{
										Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//���㲥��ʱʱ����(���ֶ���)
									}
									break;
								case 8://�ز��㲥ת��.����·��ȷ��֡
									if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
									{//0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ���
										switch(UARTCtrlRS485->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
										{
											case 1://1=���1֡����
												if(Get_Uart_3762_RxData_AFN_Fn(PORTRS485)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
												{
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													y=0;//ת���������ݳ���
													goto BroadcastESC;
												}
												else
												{
													UARTCtrlRS485->RxByte=0;//RX����(�ѽ����ֽ���)=0
													UARTCtrlRS485->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
												}
												break;
											case 0://0=����
											case 2://2=���ճ�ʱ(�����)
												if(Terminal_Ram->RouterCheck_S_Timer==0)//���㲥��ʱʱ����(���ֶ���)
												{
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													y=0;//ת���������ݳ���
													goto BroadcastESC;
												}
												else
												{
													UARTCtrlRS485->RxByte=0;//RX����(�ѽ����ֽ���)=0
													UARTCtrlRS485->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
												}
												break;
										}
									}
									else
									{//1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
										switch(UARTCtrlRS485->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
										{
											case 1://1=���1֡����
												if(Get_Uart_3762_RxData_AFN_Fn(PORTRS485)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
												{
													//ȡ�ȴ�ʱ��
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													i=p8[17];
													i+=p8[18]<<8;
													i*=1000/10;
													if(i>0xffff)
													{
														i=0xffff;
													}
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=i;
													UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
													break;
												}
											case 0://0=����
											case 2://2=���ճ�ʱ(�����)
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=0;//ת���������ݳ���
												goto BroadcastESC;
										}
									}
									break;
								case 9://�ȴ��㲥��ʱ
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
									if(p16timer[0]==0)
									{
										p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
										y=0;//ת���������ݳ���
										goto BroadcastESC;
									}
									break;
						#if (USER/100)==7//�㽭�û�
								case 10://ָ�����ַУʱ.ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
									x=p8rxbuff[18+4];//ת���ֽ���
									if(x>(128-2))
									{
										x=(128-2);
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[4]=0x04;
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
									y=0;//��ǰ��0xfe����
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									for(i=0;i<6;i++)
									{
										p8[16+i]=p8rxbuff[18+6+y+1+i];
									}

									p8+=128;
									p8[0]=0;//ͨ��Э�����ͣ�00HΪ͸�����䣻01HΪ DL/T 645-1997��02HΪ DL/T 645-2007��03H��FFH����
									p8[1]=1;//										
									p8[2]=0;//										
									p8[3]=x;//���ĳ���
									MR(((u32)p8)+4,((u32)p8rxbuff)+18+6,x);
									Uart_3762_TxData(PORTRS485,0x13,1,4+x);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)	
									WarningOccur(PORTRS485,(u8 *)"�ز�ת��Уʱ");//�澯����,��ڸ澯����͸澯����
									break;
								case 11://ָ�����ַУʱ.����ʱ����ѯ,���㲥Уʱ
									if(UARTCtrlRS485->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=MRR(((u32)p8)+10,3);
									if((i!=0x000414)||(UARTCtrlRS485->Task!=1))
									{
										Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
										Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
										UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										goto err;
									}
									i=MRR(((u32)p8)+19,2);//�㲥ͨ���ӳ�ʱ����
									y=0;//��ǰ��0xfe����
									for(x=0;x<14;x++)
									{
										if(p8rxbuff[18+6+x]!=0xfe)
										{
											break;
										}
										y++;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
									p8[0]=y+18;//���ĳ���
									MR(((u32)p8)+1,((u32)p8rxbuff)+18+6,y+18);
									for(x=0;x<6;x++)//ԭ�㲥ʱ���0x33
									{
										p8[1+y+10+x]-=0x33;
									}
									d64=MRR(((u32)p8)+1+y+10,6);
									d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
									MWR(d64,((u32)p8)+1+y+10,6);
									for(i=0;i<6;i++)
									{
										p8[1+y+10+i]+=0x33;
									}
									x=0;
									for(i=0;i<16;i++)
									{
										x+=p8[1+y+i];
									}
									p8[y+17]=x;
									p8[y+18]=0x16;
									Uart_3762_TxData(PORTRS485,0x14,3,y+19);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
									Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//�ӽڵ������ʱʱ����
									break;
								case 12://�ز��㲥ת��.����·��ȷ��֡
									switch(UARTCtrlRS485->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									{
										case 1://1=���1֡����
											p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
											i=MRR(((u32)p8)+22,3);
											if(i==0x000113)
											{
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=p8[28];//����645֡�ܳ���
												goto BroadcastESC;
											}
											else
											{
												UARTCtrlRS485->RxByte=0;//RX����(�ѽ����ֽ���)=0
												UARTCtrlRS485->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
												p16timer[0]=RouterTxtoRxOverTimer;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
											}
											break;
										case 0://0=����
										case 2://2=���ճ�ʱ(�����)
											if(Terminal_Ram->RouterCheck_S_Timer==0)//�ӽڵ������ʱʱ����
											{
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=0;//ת���������ݳ���
												goto BroadcastESC;
											}
											else
											{
												UARTCtrlRS485->RxByte=0;//RX����(�ѽ����ֽ���)=0
												UARTCtrlRS485->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
												p16timer[0]=RouterTxtoRxOverTimer;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
											}
											break;
									}
									break;
						#endif
//						#if(USER/100)==0//��������
//								case 10://ָ�����ַУʱ.ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
//									x=p8rxbuff[18+4];//ת���ֽ���
//									if(x>(128-2))
//									{
//										x=(128-2);
//									}
//									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
//									p8[4]=0x04;
//									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
//									y=0;//��ǰ��0xfe����
//									for(i=0;i<14;i++)
//									{
//										if(p8rxbuff[18+6+i]!=0xfe)
//										{
//											break;
//										}
//										y++;
//									}
//									for(i=0;i<6;i++)
//									{
//										p8[16+i]=p8rxbuff[18+6+y+1+i];
//									}
//
//									p8+=128;
//									p8[0]=0;//ͨ��Э�����ͣ�00HΪ͸�����䣻01HΪ DL/T 645-1997��02HΪ DL/T 645-2007��03H��FFH����
//									p8[1]=1;//										
//									p8[2]=0;//										
//									p8[3]=x;//���ĳ���
//									MR(((u32)p8)+4,((u32)p8rxbuff)+18+6,x);
//									Uart_3762_TxData(PORTRS485,0x13,1,4+x);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
//									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)	
//									WarningOccur(PORTRS485,(u8 *)"�ز�ת��F1");//�澯����,��ڸ澯����͸澯����
//									break;
//								case 11://ָ�����ַУʱ.����ʱ����ѯ,���㲥Уʱ
//									if(UARTCtrlRS485->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
//									{
//										break;
//									}
//									p8d=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//									i=MRR(((u32)p8d)+10,3);
//									if((i!=0x000414)||(UARTCtrlRS485->Task!=1))
//									{
//										Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
//										Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
//										UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
//										UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
//										goto err;
//									}
//									i=MRR(((u32)p8d)+19,2);//�㲥ͨ���ӳ�ʱ����
//									y=0;//��ǰ��0xfe����
//									for(x=0;x<14;x++)
//									{
//										if(p8rxbuff[18+6+x]!=0xfe)
//										{
//											break;
//										}
//										y++;
//									}
//									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
//									p8+=128;
//									p8[0]=MRR(((u32)p8d)+21,1);//���ĳ���
//									MR(((u32)p8)+1,((u32)p8rxbuff)+18+6,p8[0]);
//
//									x=MRR(((u32)p8d)+18+6+y+8+2,4);
//									if(x==0x3733343F)
//									{
//										for(x=0;x<7;x++)//ԭʱ���0x33
//										{
//											p8[1+y+22+x]-=0x33;
//										}
//										d64=MRR(((u32)p8)+1+y+22,3);
//										d64+=(MRR(((u32)p8)+1+y+22+4,3)<<24);
//										d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
//										MWR(d64,((u32)p8)+1+y+22,3);
//										MWR((d64>>24),((u32)p8)+1+y+22+4,3);
//										for(i=0;i<7;i++)
//										{
//											p8[1+y+22+i]+=0x33;
//										}
//									}
//									else
//									{//if(x==0x37333435)
//										for(x=0;x<3;x++)//ԭʱ���0x33
//										{
//											p8[1+y+22+x]-=0x33;
//										}
//										d64=MRR(((u32)p8)+1+y+22,3);
//										d64+=0x010101000000;
//										d64=YMDHMS_AddS(d64,i);//������ʱ�����S��,���ؼ�S����������ʱ����
//										MWR(d64,((u32)p8)+1+y+22,3);
//										for(i=0;i<3;i++)
//										{
//											p8[1+y+22+i]+=0x33;
//										}
//									}
//									x=0;
//									for(i=0;i<(p8[0]-y-2);i++)
//									{
//										x+=p8[1+y+i];
//									}
//									p8[p8[0]-1]=x;//cs
//									p8[p8[0]]=0x16;
//									
//									Uart_3762_TxData(PORTRS485,0x14,3,p8[0]+1);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
//									UARTCtrl->SubTask++;//������(��ʱ����·��ģ������)
//									Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//�ӽڵ������ʱʱ����
//									break;
//								case 12://ָ�����ַУʱ.����·��ȷ��֡
//									switch(UARTCtrlRS485->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
//									{
//										case 1://1=���1֡����
//											p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//											i=MRR(((u32)p8)+22,3);
//											if(i==0x000113)
//											{
//												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//												y=p8[28];//����645֡�ܳ���
//												goto BroadcastESC;
//											}
//											else
//											{
//												UARTCtrlRS485->RxByte=0;//RX����(�ѽ����ֽ���)=0
//												UARTCtrlRS485->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
//												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
//												p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
//											}
//											break;
//										case 0://0=����
//										case 2://2=���ճ�ʱ(�����)
//											if(Terminal_Ram->RouterCheck_S_Timer==0)//�ӽڵ������ʱʱ����
//											{
//												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//												y=0;//ת���������ݳ���
//												goto BroadcastESC;
//											}
//											else
//											{
//												UARTCtrlRS485->RxByte=0;//RX����(�ѽ����ֽ���)=0
//												UARTCtrlRS485->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
//												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
//												p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
//											}
//											break;
//									}
//									break;
//						#endif
						
								default:
									UARTCtrl->Task=0;
									Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
									Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=�ȴ���֡����������ʱ(ԭ��Ϊ�����ʱ��Ϊ��ֵ��
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2.RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//�õ�UART��ʱ����ַ
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//ת���ֽ���
								MW(((u32)p8rxbuff)+18+6,Get_ADDR_UARTnTx(PORTRS485),i);
								UARTCtrlRS485->BpsCtrl=p8rxbuff[19];
								Init_UARTn(PORTRS485);//��ʼ��UART��,���ͨ�ſ�����������
								UARTCtrlRS485->RxByte=0;//�����ֽ���
								UARTCtrlRS485->TxByte=i;//�����ֽ���
								//p16=(u16 *)Get_ADDR_UARTn(PORTRS485);//�õ�UART�����׵�ַ
								//p16[0]=0;//FIFOָ��
								//p16[1]=0;//FIFO�Ѷ�ָ��
								Start_UARTn_Tx(PORTRS485);//����UART�ڷ���
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
					#endif
								UARTCtrl->Task=10;//10=����ͨ�������ڷ�������
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=����ͨ�������ڷ�������
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								i=p8rxbuff[20];//͸��ת�����յȴ����ĳ�ʱʱ��
								if((i&0x80)==0x0)
								{//10ms
									i*=10;
								}
								else
								{//s
									i&=0x7f;
									i*=1000;
									if(i>0xffff)
									{
										i=0xffff;
									}
								}
								if(i<1000)
								{
									i=1000;//��С1000ms
								}
								if(i>ReadDL645OverTimer)
								{
									i=ReadDL645OverTimer;//���ReadDL645OverTimer
								}
								i/=10;
								p16[0]=i;
								UARTCtrl->Task=11;//11=����ͨ�������ڽ�������
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=����ͨ�������ڽ�������
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
							p16=(u16 *)Get_ADDR_UARTn(PORTRS485);
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							DMA_UARTn_RX_Pointer(PORTRS485);//����DMA_UARTn����ָ��ֵ(p0)
							if(p16[0]!=p16[1])
							{//���յ��ֽ�
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
					#endif
								if(p16[0]>p16[1])
								{
									i=p16[0]-p16[1];
									p16[1]=p16[0];
									UARTCtrlRS485->RxByte+=i;///�����ֽ����Ĵ�
									i=p8rxbuff[21];//͸��ת�����յȴ��ֽڳ�ʱʱ�� NIB 10MS
									if(i<(50/10))
									{
										i=50/10;//��С50ms
									}
									if(i>(1000/10))
									{
										i=1000/10;//���1000ms
									}
									p16timer[0]=i;
								}
								else
								{
									UARTCtrlRS485->RxByte=0;//������ֽ����Ĵ�
									UARTCtrlRS485->Lock=0;//�������
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									UARTCtrlRS485->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									goto err;
								}
							}
							else
							{
								if(p16timer[0]==0)
								{//����ʱ�䵽
									y=0;//��ת��֡ǰ��0xfe����
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									i=UARTCtrlRS485->RxByte;//�յ����ݳ���
									if(i==0x0)
									{//û�յ�����
			F1_RS485_ReTx:
										for(i=0;i<6;i++)
										{//���Ƿ�㲥��ַ
											if(p8rxbuff[18+6+y+1+i]!=0x99)
											{
												break;
											}
										}
										if(i!=6)
										{//���ǹ㲥��ַ,�ط�
											if(UARTCtrl->ReRMCount<2)//�ط�����
											{
												UARTCtrl->ReRMCount++;//�ط�����
												p16timer[0]=2000/10;//�̶���ʱ2����ط�,����ģ�������Ӧ��ʱ
												UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ
												return;
											}
										}
									}
									else
									{//�������յ�
										//�������֡
										x=0;//��ת������֡ǰ��0xfe����
										p8=(u8*)(Get_ADDR_UARTn(PORTRS485)+10);
										for(i=0;i<14;i++)
										{
											if(p8[i]!=0xfe)
											{
												break;
											}
											x++;
										}
										//֡ͷ0x68�Ƚ�
										if((p8[x]!=0x68)||(p8[x+7]!=0x68))
										{
											goto F1_RS485_ReTx;
										}
										//��ַ�Ƚ�
										for(i=0;i<6;i++)
										{
											if(p8rxbuff[18+6+y+1+i]!=0xaa)
											{
												if(p8rxbuff[18+6+y+1+i]!=p8[x+1+i])
												{
													break;
												}
											}
										}
										if(i!=6)
										{
											goto F1_RS485_ReTx;
										}
									}
									
									i=UARTCtrlRS485->RxByte;//�յ����ݳ���
									UARTCtrlRS485->Lock=0;//�������
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									UARTCtrlRS485->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									UARTCtrlRS485->RxByte=0;//������ֽ����Ĵ�
									MW(Get_ADDR_UARTn(PORTRS485)+10,((u32)p8txbuff)+21,i);
									p8txbuff[19]=i;
									p8txbuff[20]=i>>8;
									i+=7;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,2�ֽڳ���
									i+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
									p8txbuff[1]=i;
									p8txbuff[2]=i>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
									Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
								}
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
err:
					//ȫ������
					Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
					i=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
					Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
					break;
				default:
					UARTCtrl->Task=0;
					break;
			}
			break;
/*
		case 9://F9ת����վֱ�ӶԵ��ĳ�����������
			switch(UARTCtrl->Task)
			{
				case 8://8=�ȴ�ͨ������������ת��
					switch(p8rxbuff[18])
					{
						case 1://�ڲ����������ӿ�
							p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							n=p8rxbuff[19];//ת���м̼���n
							if(n==0xff)
							{
								n=0;//��ָ���м�·��
							}
							n*=6;//�м̼����͵�ַ�ֽڳ���
							n++;
							Protocol=p8rxbuff[19+n+6];//ת��ֱ�ӳ��������ݱ�ʶ����
							Protocol&=3;
							if(Protocol==0)
							{//0������DL/T645-1997�� 1������DL/T645-2007
								goto err;
							}
							CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//ת��Ŀ���ַ
							DI=MRR(((u32)p8rxbuff)+19+n+6+1,4);//ת��ֱ�ӳ��������ݱ�ʶ
							Read_DL645_2007_TxData(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
							i=UARTCtrlRS485->Task;//ԭ����Ĵ�
							UARTCtrlRS485->Task=1;//1=�������
						  if((Comm_Ram->SoftModule&(1<<1))==0)
						  {
								p32=(u32 *)(ADDR_I1_ENTRY);
						    (*(void(*)())p32[70])((u32)ACSAMPLEPORT);//����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
							}
							UARTCtrlRS485->Task=i;//ԭ����ָ�
							p8=(u8*)Get_ADDR_UARTnTx(ACSAMPLEPORT);
							if((p8[8]&0x40)!=0)
							{
								x=4;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
								y=4;//ת���������ݳ���
								MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//ת��ֱ�ӳ��������ݱ�ʶ
							}
							else
							{
								x=5;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
								y=p8[9];//ת���������ݳ���
								for(i=0;i<y;i++)
								{//�������0x33
									p8[10+i]-=0x33;
								}
							}
							MWR(CommAddr,((u32)p8txbuff)+19,6);//ת��Ŀ���ַ
							p8txbuff[25]=x;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
							p8txbuff[26]=y;//ת��ֱ�ӳ��������������ֽ���k+2
							MR(((u32)p8txbuff)+27,((u32)p8)+10,y);//ת��ֱ�ӳ�������������
							y+=13;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־,1�ֽ����������ֽ���
							y+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
							p8txbuff[1]=y;
							p8txbuff[2]=y>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
							break;
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							if(p16timer[0]==0)//�ȴ�����ת���ڿ��ж�ʱ5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
								if(UARTCtrlRS485->Task==0)
								{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrl->ReRMCount=0;//�ط�����
									UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ
								}
							}
							break;
#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
						case 31://�˿ں�=31,�ز�
							//�˿ں�=31,ת��ֻ�ܶ�DL645-1997��2007����
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//������(��ʱ����·��ģ���������,��ÿ֡����OKʱ��ʼ��Ϊ0)
							{
								case 0://�ȴ��ڿձ�ʱ����
									n=p8rxbuff[19];//ȡת���м̼���
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
									if(p16[0]!=p16[1])
									{//�ѽ��յ���֡
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										y=0;//ת���������ݳ���
										goto F9err;
									}
									if(p16timer[0]==0)//�ȴ�����ת���ڿ��ж�ʱ120S
									{
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										y=0;//ת���������ݳ���
										goto F9err;
									}
									if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
									{
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										y=0;//ת���������ݳ���
										p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
										goto F9err;
									}
									if(Terminal_Router->UpDateTask!=0)//�����������
									{
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										y=0;//ת���������ݳ���
										p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
										goto F9err;
									}
									if(UARTCtrlRS485->Lock!=0x55)
									{//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										//if(UARTCtrlRS485->Task==0)
										if(UARTCtrlRS485->Task!=4)
										{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
											p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
											UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
											UARTCtrl->SubTask=1;//������(��ʱ����·��ģ������)
										}
									}
									break;
								case 1://��ͣ·��
									if(Terminal_Router->RouterCtrl!=0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//��ͣ·��
									Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrl->SubTask=2;//������(��ʱ����·��ģ������)
									break;
								case 2://��ͣ·�ɽ���
									if(Terminal_Router->RouterCtrl==0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									{
										UARTCtrl->ReRMCount=0;//�ط�����
										UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
									}
									break;
								case 3://·��ת������
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1!=3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
									{
										p8[3]=0x41;//������;����ʽ·���ز�ͨ��
									}
									else
									{
										p8[3]=0x4a;//������;΢��������ͨ��
									}
									p8[4]=4;//ͨ��ģ���ʶ=1
									p8[5]=0;
									p8[6]=0;//Ԥ��Ӧ���ֽ���
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
									p8[9]=UARTCtrlRS485->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
									n=p8rxbuff[19];//ȡת���м̼���
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									MR(((u32)p8)+16,((u32)p8rxbuff)+18+2+n,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
									CommAddr=MRR(((u32)p8)+16,6);//ͨ�ŵ�ַ
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[18+2+n+6];//ת�����ݱ�ʶ����
									i++;
									p8[25]=i;
									DI=MRR(((u32)p8rxbuff)+18+2+n+6+1,4);//ȡ���ݱ�ʶ
									p8[26]=0;//ͨ����ر�־
									p8[27]=0;//�����ڵ���=0
									if(i==0x1)
									{//DL/T645-1997
										p8[28]=14;//DL645���ĳ���
										Read_DL645_1997_TxData(((u32)p8)+29,DI,CommAddr);//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
									}
									else
									{
										p8[28]=16;//DL645���ĳ���
										Read_DL645_2007_TxData(((u32)p8)+29,DI,CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
									}
									i=31+p8[28];//���ܷ����ֽ���
									p8[1]=i;
									p8[2]=i>>8;
									Uart_3762_CS((u32)p8);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
									UARTCtrlRS485->TxByte=i;//�����ֽ���(���ֶ���)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
									UARTCtrl->SubTask=4;//������(��ʱ����·��ģ������)
									//WarningOccur(PORTRS485,(u8 *)"�ز�ת��ֱ�ӳ���F9");//�澯����,��ڸ澯����͸澯�ַ���
									CopyString((u8 *)"�ز�F9  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(18+2+n+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ�
									break;
								case 4://ת������
									if(UARTCtrlRS485->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									{
										break;
									}
							
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									if(UARTCtrlRS485->Task!=1)
									{
										x=1;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										y=0;//ת���������ݳ���
									}
									else
									{
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											x=4;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
											y=0;//ת���������ݳ���
										}
										else
										{
											x=5;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
											y=p8[28];//���������򳤶�
											if(y==0)
											{//�������ݳ���=0
												x=4;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
												y=0;//ת���������ݳ���
											}
											else
											{
												for(i=0;i<y;i++)
												{//ȥǰ��0xfe
													if(p8[29+i]!=0xfe)
													{
														if(i!=0)
														{
															MR(((u32)p8)+29,((u32)p8)+29+i,y-i);
														}
														break;
													}
												}
												y=p8[29+9];//���������򳤶�
												p8+=29;
												//������
												if((p8[8]&0x40)!=0x0)
												{//��վ�쳣�ش�
													x=4;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
													y=0;//ת���������ݳ���
												}
												else
												{
													//CSУ��
													Fn=0;
													for(i=0;i<(y+10);i++)
													{
														Fn+=p8[i];
													}
													Fn&=0xff;
													if(Fn!=p8[i])
													{
														x=2;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
														y=0;//ת���������ݳ���
													}
													//�������0x33
													p8=(u8 *)(Get_ADDR_UARTnRx(PORTRS485)+29+10);
													for(i=0;i<y;i++)
													{
														p8[i]-=0x33;
													}
												}
											}
										}
									}
									Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
									Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									n=p8rxbuff[19];//ȡת���м̼���
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									//Ŀ�ĵ�ַ�Ƚ�
									if(x==5)
									{//5=ת����������
					 					if(DataComp(((u32)p8rxbuff)+18+2+n,Get_ADDR_UARTnRx(PORTRS485)+29+1,6)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
										{//��ͬ
											x=2;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
											y=0;//ת���������ݳ���
										}
									}
									if(y==0x0)
									{//û�յ�����,�ز�·�������ݷ��ؼ�Ϊ��׼645֡�����鷵��֡�Ϸ���
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//�ط�����
										{
											p16=(u16 *)Get_ADDR_UARTn(PORTn);
											if(p16[0]==p16[1])
											{//û���յ���֡
												UARTCtrl->ReRMCount++;//�ط�����
												UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
												return;
											}
										}
									#endif
									}
							F9err:
									UARTCtrlRS485->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									MR(((u32)p8txbuff)+18+1,((u32)p8rxbuff)+18+2+n,6);//ת��Ŀ���ַ
									p8txbuff[18+7]=x;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
									if(y==0x0)
									{//�޷���
										i=p8rxbuff[18+2+n+6];//ȡת�����ݱ�ʶ����
										if(i==0x0)
										{//1997
											y=2;
										}
										else
										{//2007
											y=4;
										}
										p8txbuff[18+8]=y;//ת��ֱ�ӳ��������������ֽ���k+2
										MR(((u32)p8txbuff)+18+9,((u32)p8rxbuff)+18+2+n+6+1,y);//ת��ֱ�ӳ��������ݱ�ʶ
									}
									else
									{//�з���
										p8txbuff[18+8]=y;//ת��ֱ�ӳ��������������ֽ���k+2
										MR(((u32)p8txbuff)+18+9,Get_ADDR_UARTnRx(PORTRS485)+29+10,y);
									}
									i=9+y+12;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ+4�ֽڱ�ʶ=12�ֽ�
									p8txbuff[1]=i;
									p8txbuff[2]=i>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
									Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
									break;
								default:
									Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
									Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrl->Task=0;
									UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=�ȴ���֡����������ʱ(ԭ��Ϊ�����ʱ��Ϊ��ֵ��
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//�õ�UART��ʱ����ַ
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								n=p8rxbuff[19];//ת���м̼���n
								if(n==0xff)
								{
									n=0;//��ָ���м�·��
								}
								n*=6;//�м̼����͵�ַ�ֽڳ���
								n++;
								CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//ת��Ŀ���ַ
								DI=MRR(((u32)p8rxbuff)+19+n+6+1,4);//ת��ֱ�ӳ��������ݱ�ʶ
								i=p8rxbuff[19+n+6];//ת��ֱ�ӳ��������ݱ�ʶ����
								i&=3;
								MC(0xfe,Get_ADDR_UARTnTx(PORTRS485),4);
								if(i==0)
								{//0������DL/T645-1997
									UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//Ĭ��1200
									Read_DL645_1997_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
									UARTCtrlRS485->TxByte=4+14;//�����ֽ���
								}
								else
								{//1������DL/T645-2007
									UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//Ĭ��2400
									Read_DL645_2007_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
									UARTCtrlRS485->TxByte=4+16;//�����ֽ���
								}
								i=Check_AFN04F10_Addr(p8rxbuff[18],CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
								if(i!=0)
								{//�ҵ�
									//ͨ������
									i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
									if((i&0xe0)!=0x0)
									{//����Ĭ��
										UARTCtrlRS485->BpsCtrl=(i&0xe0)+0x0b;
									}
								}
								Init_UARTn(PORTRS485);//��ʼ��UART��,���ͨ�ſ�����������
								Start_UARTn_Tx(PORTRS485);//����UART�ڷ���
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
					#endif
								UARTCtrl->Task=10;//10=����ͨ�������ڷ�������
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=����ͨ�������ڷ�������
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								p16[0]=ReadDL645OverTimer/10;//ת�����յȴ����ĳ�ʱʱ��
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								UARTCtrlRS485->RxByte=0;//�����ֽ���
								UARTCtrl->Task=11;//11=����ͨ�������ڽ�������
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=����ͨ�������ڽ�������
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							i=DL645_Rx(PORTRS485);//DL645֡����;����0û���,1���,2��ʱ
							if(i==0)
							{
								break;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							n=p8rxbuff[19];//ת���м̼���n
							if(n==0xff)
							{
								n=0;//��ָ���м�·��
							}
							n*=6;//�м̼����͵�ַ�ֽڳ���
							n++;
							Protocol=p8rxbuff[19+n+6];//ת��ֱ�ӳ��������ݱ�ʶ����
							Protocol&=3;//0=����DL/T645-1997,1=����DL/T645-2007
							if(i==2)
							{//2=��ʱ
								if(UARTCtrl->ReRMCount<2)//�ط�����
								{
									UARTCtrl->ReRMCount++;//�ط�����
									UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ
									break;
								}
							}
							if(i==1)
							{//1���
								if(Protocol==0)
								{//0������DL/T645-1997
									x=DL645_1997_RxData(PORTRS485);//DL645_1997֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
								}
								else
								{//1������DL/T645-2007
									x=DL645_2007_RxData(PORTRS485);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
								}
								if(x!=0)
								{
									if(x!=2)
									{
										//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
										//��������
										p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
										p16[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
										UARTCtrlRS485->RxByte=0;//������ֽ����Ĵ�
										break;
									}
								}
							}
							
							UARTCtrlRS485->Lock=0;//�������
							UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
							UARTCtrlRS485->RxByte=0;//������ֽ����Ĵ�
							UARTCtrlRS485->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
							if(i!=1)
							{
								x=1;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
								if(Protocol==0x0)
								{//1997
									y=2;//ת���������ݳ���
								}
								else
								{//2007
									y=4;//ת���������ݳ���
								}
								MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//ת��ֱ�ӳ��������ݱ�ʶ
							}
							else
							{
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
					#endif
								if((p8[8]&0x40)!=0)
								{
									x=4;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
									if(Protocol==0x0)
									{//1997
										y=2;//ת���������ݳ���
									}
									else
									{//2007
										y=4;//ת���������ݳ���
									}
									MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//ת��ֱ�ӳ��������ݱ�ʶ
								}
								else
								{
									x=5;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
									y=p8[9];//ת���������ݳ���
								}
							}
							CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//ת��Ŀ���ַ
							MWR(CommAddr,((u32)p8txbuff)+19,6);//ת��Ŀ���ַ
							p8txbuff[25]=x;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
							p8txbuff[26]=y;//ת��ֱ�ӳ��������������ֽ���k+2
							MR(((u32)p8txbuff)+27,((u32)p8)+10,y);//ת��ֱ�ӳ�������������
							y+=13;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־,1�ֽ����������ֽ���
							y+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
							p8txbuff[1]=y;
							p8txbuff[2]=y>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־?
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
			}
			break;
*/
	
		case 9://F9ת����վֱ�ӶԵ��ĳ�����������
		case 10://F10ת����վֱ�ӶԵ�������������������
			switch(UARTCtrl->Task)
			{
				case 8://8=�ȴ�ͨ������������ת��
					switch(p8rxbuff[18])
					{
						case 1://�ڲ����������ӿ�
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							n=p8rxbuff[19];//ת���м̼���n
							if(n==0xff)
							{
								n=0;//��ָ���м�·��
							}
							n*=6;//�м̼����͵�ַ�ֽڳ���
							CommAddr=MRR(((u32)p8rxbuff)+20+n,6);//ת��Ŀ���ַ
							Protocol=p8rxbuff[20+n+6];//ת��ֱ�ӳ��������ݱ�ʶ����
							Protocol&=3;
							if(Protocol==0)
							{//0:����DL/T645-1997,1:����DL/T645-2007
								goto err;
							}
							Protocol=Get_LEN_UARTnTx(PORTn);
							
							UARTCtrl->F10ReadDICount=0;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
							UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
							p8txbuff[26]=0;//������ת��ֱ�ӳ��������ݱ�ʶ����
							while(UARTCtrl->F10ReadDICount<p8rxbuff[20+n+7])
							{
								i=UARTCtrl->F10ReadDICount;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
								DI=MRR(((u32)p8rxbuff)+20+n+8+(i*4),4);//ת��ֱ�ӳ��������ݱ�ʶ
								if(UARTCtrl->SubsequentFrames==0)
								{
									Read_DL645_2007_TxData(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
								}
								else
								{
									Read_DL645_2007_TxData_SubsequentFrames(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa,UARTCtrl->SubsequentFrames);//DL645-2007����������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ,����֡��
								}
								x=UARTCtrlRS485->Task;//ԭ����Ĵ�
								UARTCtrlRS485->Task=1;//1=�������
							  if(FileDownload->File&0x04)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
							  {
									p32=(u32 *)(ADDR_Meter_ENTRY);
							    (*(void(*)())p32[130])((u32)ACSAMPLEPORT);//����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
								}
								UARTCtrlRS485->Task=x;//ԭ����ָ�
								p8=(u8*)Get_ADDR_UARTnTx(ACSAMPLEPORT);
								if((p8[8]&0x40)!=0)
								{
									if(UARTCtrl->SubsequentFrames==0)
									{
										y=4;//ת���������ݳ���
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(i*4),4);//ת��ֱ�ӳ��������ݱ�ʶ
									}
									else
									{
										y=0;//ת���������ݳ���
									}
								}
								else
								{
									y=p8[9];//ת���������ݳ���
									for(x=0;x<y;x++)
									{//�������0x33
										p8[10+x]-=0x33;
									}
									z=MRR(((u32)p8)+10,4);//���ص����ݱ�ʶ
									if(z!=DI)
									{
										y=0;
									}
								}
								x=0;//�������ݳ���
								for(z=0;z<p8txbuff[26];z++)
								{
									x+=p8txbuff[27+x];
									x++;
								}
								if(UARTCtrl->SubsequentFrames==0)
								{//��1֡
									if((27+(x+1+y)+8+2)>Protocol)
									{
										UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										i++;
										if(i>=256)
										{
											break;
										}
										UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
									}
									else
									{
										MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//ת��ֱ�ӳ�������������
										p8txbuff[27+x]=y;//ת��ֱ�ӳ��������������ֽ���k+4
										if((p8[8]&0x20)&&(y!=0))
										{//�к���֡
											UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
										}
										else
										{//�޺���֡
											UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
											p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
											i++;
											if(i>=256)
											{
												break;
											}
											UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
										}
									}
								}
								else
								{//����֡
									if(y>5)
									{//4�ֽڱ�ʶ,1�ֽ�seq
										y-=5;
									}
									else
									{
										y=0;
									}
									z=p8txbuff[27+x];//����ʶ�Ѷ��볤��
									z=255-z;//�������
									if(y>z)
									{
										y=z;
									}
									if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
									{
										UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										i++;
										if(i>=256)
										{
											break;
										}
										UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
									}
									else
									{
										MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//ת��ֱ�ӳ�������������
										p8txbuff[27+x]+=y;//ת��ֱ�ӳ��������������ֽ���k+4
										if((p8[8]&0x20)&&(y!=0))
										{//�к���֡
											UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
										}
										else
										{//�޺���֡
											UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
											p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
											i++;
											if(i>=256)
											{
												break;
											}
											UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
										}
									}
								}
							}
							MWR(CommAddr,((u32)p8txbuff)+19,6);//ת��Ŀ���ַ
							x=0;//�������ݳ���
							for(z=0;z<p8txbuff[26];z++)
							{
								x+=p8txbuff[27+x];
								x++;
							}
							if(x>(p8txbuff[26]*5))//ת��ֱ�ӳ��������ݱ�ʶ����
							{
								p8txbuff[25]=5;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
							}
							else
							{
								p8txbuff[25]=2;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
							}
							if(Fn==9)
							{
								MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
								//x+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
								//x+=12;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־
								x+=20;
							}
							else
							{
								//x+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
								//x+=13;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־,1�ֽ����ݱ�ʶ����
								x+=21;
							}
							p8txbuff[1]=x;
							p8txbuff[2]=x>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
							Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
							break;
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							if(p16timer[0]==0)//�ȴ�����ת���ڿ��ж�ʱ5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
								if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
								{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									p16timer[0]=50/10;//�ָ�20ms����(����̨�����ģ����ٶ�����50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrl->ReRMCount=0;//�ط�����
									UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ
									
									UARTCtrl->F10ReadDICount=0;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
									UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									p8txbuff[26]=0;//������ת��ֱ�ӳ��������ݱ�ʶ����
								}
							}
							break;
#if ((Project/100)==2)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
						case 31://�˿ں�=31,�ز�
							//�˿ں�=31,ת��ֻ�ܶ�DL645-1997��2007����
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//������(��ʱ����·��ģ���������,��ÿ֡����OKʱ��ʼ��Ϊ0)
							{
								case 0://�ȴ��ڿձ�ʱ����
									p8txbuff[26]=0;//������ת��ֱ�ӳ��������ݱ�ʶ����
									
									n=p8rxbuff[19];//ȡת���м̼���
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
									DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
									if(p16[0]!=p16[1])
									{//�ѽ��յ���֡
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										goto F10err;
									}
								#endif
									if(p16timer[0]==0)//�ȴ�����ת���ڿ��ж�ʱ120S
									{
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										goto F10err;
									}
									if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
									{
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										goto F10err;
									}
									if(Terminal_Router->UpDateTask!=0)//�����������
									{
										x=0;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										goto F10err;
									}
									if(UARTCtrlRS485->Lock!=0x55)
									{//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
										{//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
											UARTCtrlRS485->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
											UARTCtrl->SubTask=1;//������(��ʱ����·��ģ������)
										}
									}
									break;
								case 1://��ͣ·��
									if(Terminal_Router->RouterCtrl!=0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//��ͣ·��
									Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrl->SubTask=2;//������(��ʱ����·��ģ������)
									break;
								case 2://��ͣ·�ɽ���
									if(Terminal_Router->RouterCtrl==0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									{
										UARTCtrl->ReRMCount=0;//�ط�����
										UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
										
										UARTCtrl->F10ReadDICount=0;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
										UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									}
									break;
								case 3://·��ת������
									n=p8rxbuff[19];//ȡת���м̼���
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
									DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
									if((UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])||(p16[0]!=p16[1]))
									{//F10��ת��ֱ�ӳ��������ݱ�ʶ����>=������������յ���֡
								#else
									if(UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])
									{//F10��ת��ֱ�ӳ��������ݱ�ʶ����>=�������
								#endif
									F10End:
										Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
										Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
										
										x=0;//�������ݳ���
										for(z=0;z<p8txbuff[26];z++)
										{
											x+=p8txbuff[27+x];
											x++;
										}
										if(x>(p8txbuff[26]*5))//ת��ֱ�ӳ��������ݱ�ʶ����
										{
											x=5;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										}
										else
										{
											x=2;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										}
									F10err:
										UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
										UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										p8txbuff[25]=x;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
										
										n=p8rxbuff[19];//ȡת���м̼���
										if(n==0xff)
										{
											n=0;
										}
										n*=6;
										MR(((u32)p8txbuff)+19,((u32)p8rxbuff)+20+n,6);//ת��Ŀ���ַ
										x=0;//�������ݳ���
										for(z=0;z<p8txbuff[26];z++)
										{
											x+=p8txbuff[27+x];
											x++;
										}
										if(Fn==9)
										{
											MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
											//x+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
											//x+=12;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־
											x+=20;
										}
										else
										{
											//x+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
											//x+=13;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־,1�ֽ����ݱ�ʶ����
											x+=21;
										}
										p8txbuff[1]=x;
										p8txbuff[2]=x>>8;
										i=p8rxbuff[1]+(p8rxbuff[2]<<8);
										Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
										Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־�
										return;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
									{
										p8[3]=0x43;//������;����ʽ·���ز�ͨ��
									}
									else
									{
										if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
										{
											p8[3]=0x4a;//������;΢��������ͨ��
										}
										else
										{
											p8[3]=0x41;//������;����ʽ·���ز�ͨ��
										}
									}
									p8[4]=4;//ͨ��ģ���ʶ=1
									p8[5]=0;
									p8[6]=0;//Ԥ��Ӧ���ֽ���
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
									p8[9]=UARTCtrlRS485->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
									MR(((u32)p8)+16,((u32)p8rxbuff)+20+n,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
									CommAddr=MRR(((u32)p8)+16,6);//ͨ�ŵ�ַ
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[20+n+6];//ת�����ݱ�ʶ����
									i++;
									p8[25]=i;
									DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ��������ݱ�ʶ
									p8[26]=0;//ͨ����ر�־
									p8[27]=0;//�����ڵ���=0
									if(UARTCtrl->SubsequentFrames==0)
									{
										if(i==0x1)
										{//DL/T645-1997
											p8[28]=14;//DL645���ĳ���
											Read_DL645_1997_TxData(((u32)p8)+29,DI,CommAddr);//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
										}
										else
										{
											p8[28]=16;//DL645���ĳ���
											Read_DL645_2007_TxData(((u32)p8)+29,DI,CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
										}
									}
									else
									{
										if(i==0x1)
										{//DL/T645-1997
											p8[28]=14;//DL645���ĳ���
											Read_DL645_1997_TxData_SubsequentFrames(((u32)p8)+29,DI,CommAddr);//DL645-1997����������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
										}
										else
										{
											p8[28]=17;//DL645���ĳ���
											Read_DL645_2007_TxData_SubsequentFrames(((u32)p8)+29,DI,CommAddr,UARTCtrl->SubsequentFrames);//DL645-2007����������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ,����֡��
										}
									}
									i=31+p8[28];//���ܷ����ֽ���
									p8[1]=i;
									p8[2]=i>>8;
									if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
									{//΢��������,������ַ�òɼ�����ַ
										x=Check_AFN04F10_Addr(31,CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
										if(x!=0)
										{
											d64=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
											if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
											{
												MWR(d64,((u32)p8)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
											}
										}
									}
									Uart_3762_CS((u32)p8);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
									UARTCtrlRS485->TxByte=i;//�����ֽ���(���ֶ���)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
									UARTCtrl->SubTask=4;//������(��ʱ����·��ģ������)
									if(Fn==9)
									{
										CopyString((u8 *)"�ز�F9  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									}
									else
									{
										CopyString((u8 *)"�ز�F10 ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
									}
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(20+n+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ�
									break;
								case 4://ת������
									if(UARTCtrlRS485->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
									{
										break;
									}
									n=p8rxbuff[19];//ȡת���м̼���
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=p8[1]+(p8[2]<<8);
									if((UARTCtrlRS485->Task!=1)||(i<=16))
									{//��ʱ����ϻش�
										y=0;//ת���������ݳ���
									}
									else
									{
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											UARTCtrlRS485->Task=5;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
											UARTCtrlRS485->RxByte=0;//�����ֽ���(���ֶ���)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(i==0x0200)
											{//AFN=00,Fn=2ֱ�ӷ���֡(΢���������д����)
												p16[0]=6000/10;//6�볬ʱʱ��
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
											}
											return;
										}
										else
										{
											y=p8[28];//����645֡�ܳ���
											if(y!=0)
											{
												for(i=0;i<y;i++)
												{//ȥǰ��0xfe
													if(p8[29+i]!=0xfe)
													{
														if(i!=0)
														{
															MR(((u32)p8)+29,((u32)p8)+29+i,y-i);
														}
														break;
													}
												}
												//������
												if((p8[29+8]&0x40)!=0x0)
												{//��վ�쳣�ش�
													y=0;//ת���������ݳ���
												}
												else
												{
													y=p8[29+9];//����645֡�����򳤶�
													//CSУ��
													z=0;
													for(i=0;i<(y+10);i++)
													{
														z+=p8[29+i];
													}
													z&=0xff;
													if(z!=p8[29+i])
													{
														y=0;//ת���������ݳ���
													}
													else
													{
													#if (USER/100)==11//���ְ�
														//���������򲻼�0x33
														//���ݱ�ʶ��0x33
														for(i=0;i<4;i++)
														{
															p8[29+10+i]-=0x33;
														}
													#else
														//�������0x33
														for(i=0;i<y;i++)
														{
															p8[29+10+i]-=0x33;
														}
													#endif
														//Ŀ�ĵ�ַ�Ƚ�
									 					if(DataComp(((u32)p8rxbuff)+20+n,Get_ADDR_UARTnRx(PORTRS485)+29+1,6)!=0x0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
														{//��ͬ
															UARTCtrlRS485->Task=5;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
															UARTCtrlRS485->RxByte=0;//�����ֽ���(���ֶ���)
															p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
															p16[0]=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
															return;
														}
														//DI�Ƚ�
														DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ��������ݱ�ʶ
														z=MRR(((u32)p8)+29+10,4);//���ص����ݱ�ʶ
														i=p8rxbuff[20+n+6];//ת�����ݱ�ʶ����
														if(i==0)
														{//1997
															DI&=0xffff;
															z&=0xffff;
														}
														if(DI!=z)
														{
															if(z==0x04001501)
															{//���յ������ϱ�֡
//															#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//																d64=MRR(((u32)p8rxbuff)+20+n,6);
//																Write_AutoReportWord(31,d64,((u32)p8)+29+14);//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
//															#endif
																UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
																p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
																i=UARTCtrl->F10ReadDICount;//F10��ת��ֱ�ӳ��������ݱ�ʶ
																i++;
																if(i>=256)
																{
																	goto F10End;
																}
																UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ
																UARTCtrl->ReRMCount=0;//�ط�����
																UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
																return;
															}
															UARTCtrlRS485->Task=5;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
															UARTCtrlRS485->RxByte=0;//�����ֽ���(���ֶ���)
															p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
															p16[0]=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
															return;
														}
														else
														{
															y=p8[29+9];//���������򳤶�
															if(i==0)
															{//1997
																//2�ֽڱ�ʶ��Ϊ4�ֽ�
																if(y<2)
																{
																	y=0;
																}
																else
																{
																	y-=2;
																	MR(ADDR_DATABUFF,((u32)p8)+29+12,y);
																	MR(((u32)p8)+29+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ���������
																	MR(((u32)p8)+29+14,ADDR_DATABUFF,y);
																	y+=4;
																}
															}
														}
													}
												}
											}
										}
									}
									
									p8=(u8 *)(Get_ADDR_UARTnRx(PORTRS485)+29);
									if(y==0x0)
									{//û�յ�����
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//�ط�����
										{
											UARTCtrl->ReRMCount++;//�ط�����
											UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
											return;
										}
									#endif
										y=4;//ת���������ݳ���
										p8[8]=0;//ȥ����֡��־
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ��������ݱ�ʶ
									}
									Protocol=Get_LEN_UARTnTx(PORTn);
									x=0;//�������ݳ���
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									i=UARTCtrl->F10ReadDICount;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
									if(UARTCtrl->SubsequentFrames==0)
									{//��1֡
										if((27+(x+1+y)+8+2)>Protocol)
										{
											UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
											i++;
											if(i>=256)
											{
												goto F10End;
											}
											UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
										}
										else
										{
											MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//ת��ֱ�ӳ�������������
											p8txbuff[27+x]=y;//ת��ֱ�ӳ��������������ֽ���k+4
											if((p8[8]&0x20)&&(y!=0))
											{//�к���֡
												UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
											}
											else
											{//�޺���֡
												UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
												p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
												i++;
												if(i>=256)
												{
													goto F10End;
												}
												UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
											}
										}
									}
									else
									{//����֡
										i=p8rxbuff[20+n+6];//ת�����ݱ�ʶ����
										if(i==0)
										{//1997
											if(y>4)
											{//4�ֽڱ�ʶ
												y-=4;
											}
											else
											{
												y=0;
											}
										}
										else
										{//2007
											if(y>5)
											{//4�ֽڱ�ʶ,1�ֽ�seq
												y-=5;
											}
											else
											{
												y=0;
											}
										}
										z=p8txbuff[27+x];//����ʶ�Ѷ��볤��
										z=255-z;//�������
										if(y>z)
										{
											y=z;
										}
										if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
										{
											UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
											i++;
											if(i>=256)
											{
												goto F10End;
											}
											UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
										}
										else
										{
											MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//ת��ֱ�ӳ�������������
											p8txbuff[27+x]+=y;//ת��ֱ�ӳ��������������ֽ���k+4
											if((p8[8]&0x20)&&(y!=0))
											{//�к���֡
												UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
											}
											else
											{//�޺���֡
												UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
												p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
												i++;
												if(i>=256)
												{
													goto F10End;
												}
												UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
											}
										}
									}
									UARTCtrl->ReRMCount=0;//�ط�����
									UARTCtrl->SubTask=3;//������(��ʱ����·��ģ������)
									break;
								default:
									Terminal_Router->StopFlags&=~(1<<2);//�ָ�·��
									Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
									UARTCtrl->Task=0;
									UARTCtrlRS485->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=�ȴ���֡����������ʱ(ԭ��Ϊ�����ʱ��Ϊ��ֵ��
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//�õ�UART��ʱ����ַ
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								n=p8rxbuff[19];//ת���м̼���n
								if(n==0xff)
								{
									n=0;//��ָ���м�·��
								}
								n*=6;//�м̼����͵�ַ�ֽڳ���
								
								p16=(u16 *)Get_ADDR_UARTn(PORTn);
							#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
								DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
								if((UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])||(p16[0]!=p16[1]))
								{//F10��ת��ֱ�ӳ��������ݱ�ʶ����>=������������յ���֡
							#else
								if(UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])
								{//F10��ת��ֱ�ӳ��������ݱ�ʶ����>=�������
							#endif
								F10End_2:
									x=0;//�������ݳ���
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									if(x>(p8txbuff[26]*5))//ת��ֱ�ӳ��������ݱ�ʶ����
									{
										x=5;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
									}
									else
									{
										x=2;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
									}
								//F10err_2:
									p8txbuff[25]=x;//ת�������־:0=����ִ��ת��,1=ת�����ճ�ʱ,2=ת�����մ���,3=ת������ȷ��,4=ת�����շ���,5=ת����������,����ֵ��Ч
									UARTCtrlRS485->Lock=0;//�������
									UARTCtrlRS485->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									UARTCtrlRS485->RxByte=0;//������ֽ����Ĵ�
									UARTCtrlRS485->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���


									n=p8rxbuff[19];//ȡת���м̼���
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									MR(((u32)p8txbuff)+19,((u32)p8rxbuff)+20+n,6);//ת��Ŀ���ַ
									x=0;//�������ݳ���
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									if(Fn==9)
									{
										MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
										//x+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
										//x+=12;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־
										x+=20;
									}
									else
									{
										//x+=8;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ=8�ֽ�
										//x+=13;//4�ֽڱ�ʶ,1�ֽڶ˿ں�,6�ֽ�ת��Ŀ���ַ,1�ֽ�ת�������־,1�ֽ����ݱ�ʶ����
										x+=21;
									}
									p8txbuff[1]=x;
									p8txbuff[2]=x>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ
									Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־�
									return;
								}
								CommAddr=MRR(((u32)p8rxbuff)+20+n,6);//ת��Ŀ���ַ
								DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ��������ݱ�ʶ
								i=p8rxbuff[20+n+6];//ת��ֱ�ӳ��������ݱ�ʶ����
								i&=3;
								MC(0xfe,Get_ADDR_UARTnTx(PORTRS485),4);
								if(UARTCtrl->SubsequentFrames==0)
								{//��1֡
									if(i==0)
									{//0������DL/T645-1997
										UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//Ĭ��1200
										Read_DL645_1997_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
										UARTCtrlRS485->TxByte=4+14;//�����ֽ���
									}
									else
									{//1������DL/T645-2007
										UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//Ĭ��2400
										Read_DL645_2007_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
										UARTCtrlRS485->TxByte=4+16;//�����ֽ���
									}
								}
								else
								{//����֡
									if(i==0)
									{//0������DL/T645-1997
										UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//Ĭ��1200
										Read_DL645_1997_TxData_SubsequentFrames(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
										UARTCtrlRS485->TxByte=4+14;//�����ֽ���
									}
									else
									{//1������DL/T645-2007
										UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//Ĭ��2400
										Read_DL645_2007_TxData_SubsequentFrames(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr,UARTCtrl->SubsequentFrames);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
										UARTCtrlRS485->TxByte=4+17;//�����ֽ���
									}
								}
								i=Check_AFN04F10_Addr(p8rxbuff[18],CommAddr);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
								if(i!=0)
								{//�ҵ�
									//ͨ������
									i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
									if((i&0xe0)!=0x0)
									{//����Ĭ��
										UARTCtrlRS485->BpsCtrl=(i&0xe0)+0x0b;
									}
								}
								Init_UARTn(PORTRS485);//��ʼ��UART��,���ͨ�ſ�����������
								Start_UARTn_Tx(PORTRS485);//����UART�ڷ���
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
					#endif
								UARTCtrl->Task=10;//10=����ͨ�������ڷ�������
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=����ͨ�������ڷ�������
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								p16[0]=ReadDL645OverTimer/10;//ת�����յȴ����ĳ�ʱʱ��
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								UARTCtrlRS485->RxByte=0;//�����ֽ���
								UARTCtrl->Task=11;//11=����ͨ�������ڽ�������
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=����ͨ�������ڽ�������
					switch(p8rxbuff[18])
					{
						case 2://�˿ں�=2,RS485-1
						case 3://�˿ں�=3,RS485-2
						case 4://�˿ں�=4,RS485-3
							i=DL645_Rx(PORTRS485);//DL645֡����;����0û���,1���,2��ʱ
							if(i==0)
							{
								break;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							n=p8rxbuff[19];//ת���м̼���n
							if(n==0xff)
							{
								n=0;//��ָ���м�·��
							}
							n*=6;//�м̼����͵�ַ�ֽڳ���
							
							Protocol=p8rxbuff[20+n+6];//ת��ֱ�ӳ��������ݱ�ʶ����
							Protocol&=3;//0=����DL/T645-1997,1=����DL/T645-2007
							if(i==2)
							{//2=��ʱ
								if(UARTCtrl->ReRMCount<2)//�ط�����
								{
									UARTCtrl->ReRMCount++;//�ط�����
									UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ
									break;
								}
							}
							if(i==1)
							{//1���
								if(Protocol==0)
								{//0������DL/T645-1997
									x=DL645_1997_RxData(PORTRS485);//DL645_1997֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
								}
								else
								{//1������DL/T645-2007
									x=DL645_2007_RxData(PORTRS485);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
								}
								if(x!=0)
								{
									if(x==3)
									{//3=���ݱ�ʶ��
										p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
										//��0x33
										for(z=0;z<p8[9];z++)
										{
											p8[10+z]-=0x33;
										}
										z=MRR(((u32)p8)+10,4);//���ص����ݱ�ʶ
										if(z==0x04001501)//if(z==0x37334834)
										{//���յ������ϱ�֡
//										#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
//											d64=MRR(((u32)p8)+1,6);
//											Write_AutoReportWord(p8rxbuff[18],d64,((u32)p8)+14);//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
//										#endif
											UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
											p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
											i=UARTCtrl->F10ReadDICount;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
											i++;
											if(i>=256)
											{
												goto F10End_2;
											}
											UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
											UARTCtrl->ReRMCount=0;//�ط�����
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//�õ�UART��ʱ����ַ
											p16[0]=0;
											UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ(ԭ��Ϊ�����ʱ��Ϊ��ֵ��
											return;
										}
									}
									if(x!=2)
									{
										//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
										//��������
										p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
										p16[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
										UARTCtrlRS485->RxByte=0;//������ֽ����Ĵ�
										break;
									}
								}
							}
							p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
							if(i!=1)
							{
								y=4;//ת���������ݳ���
								p8[8]=0;//ȥ����֡��־
								MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ���������
							}
							else
							{
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
								}
					#endif
								if((p8[8]&0x40)!=0)
								{
									if(UARTCtrl->SubsequentFrames==0)
									{
										y=4;//ת���������ݳ���
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ��������ݱ�ʶ
									}
									else
									{
										y=0;//ת���������ݳ���
									}
								}
								else
								{
									y=p8[9];//ת���������ݳ���
									if(Protocol==0)
									{//1997
										//2�ֽڱ�ʶ��Ϊ4�ֽ�
										if(y<2)
										{
											y=0;
										}
										else
										{
											y-=2;
											MR(ADDR_DATABUFF,((u32)p8)+12,y);
											MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//ת��ֱ�ӳ���������
											MR(((u32)p8)+14,ADDR_DATABUFF,y);
											y+=4;
										}
									}
								}
							}
							Protocol=Get_LEN_UARTnTx(PORTn);
							x=0;//�������ݳ���
							for(z=0;z<p8txbuff[26];z++)
							{
								x+=p8txbuff[27+x];
								x++;
							}
							i=UARTCtrl->F10ReadDICount;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
							if(UARTCtrl->SubsequentFrames==0)
							{//��1֡
								if((27+(x+1+y)+8+2)>Protocol)
								{
									UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									i++;
									if(i>=256)
									{
										goto F10End_2;
									}
									UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
								}
								else
								{
									MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//ת��ֱ�ӳ�������������
									p8txbuff[27+x]=y;//ת��ֱ�ӳ��������������ֽ���k+4
									if((p8[8]&0x20)&&(y!=0))
									{//�к���֡
										UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
									}
									else
									{//�޺���֡
										UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
										i++;
										if(i>=256)
										{
											goto F10End_2;
										}
										UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
									}
								}
							}
							else
							{//����֡
								i=p8rxbuff[20+n+6];//ת�����ݱ�ʶ����
								if(i==0)
								{//1997
									if(y>4)
									{//4�ֽڱ�ʶ
										y-=4;
									}
									else
									{
										y=0;
									}
								}
								else
								{//2007
									if(y>5)
									{//4�ֽڱ�ʶ,1�ֽ�seq
										y-=5;
									}
									else
									{
										y=0;
									}
								}
								z=p8txbuff[27+x];//����ʶ�Ѷ��볤��
								z=255-z;//�������
								if(y>z)
								{
									y=z;
								}
								if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
								{
									UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
									i++;
									if(i>=256)
									{
										goto F10End_2;
									}
									UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
								}
								else
								{
									MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//ת��ֱ�ӳ�������������
									p8txbuff[27+x]+=y;//ת��ֱ�ӳ��������������ֽ���k+4
									if((p8[8]&0x20)&&(y!=0))
									{//�к���֡
										UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
									}
									else
									{//�޺���֡
										UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
										p8txbuff[26]++;//������ת��ֱ�ӳ��������ݱ�ʶ����
										i++;
										if(i>=256)
										{
											goto F10End_2;
										}
										UARTCtrl->F10ReadDICount=i;//F10��ת��ֱ�ӳ��������ݱ�ʶ����
									}
								}
							}
							UARTCtrl->ReRMCount=0;//�ط�����
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//�õ�UART��ʱ����ַ
							p16[0]=0;
							UARTCtrl->Task=9;//9=�ȴ���֡����������ʱ(ԭ��Ϊ�����ʱ��Ϊ��ֵ��
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
			}
			break;
		default:
			UARTCtrl->Task=0;
			break;
	}
}












