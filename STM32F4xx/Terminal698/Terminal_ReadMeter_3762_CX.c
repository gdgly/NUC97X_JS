
//�����ز�ģ��
//����֧��:010-68459012-8053 �ι�
//010-68466652(����)����ϼ
//68459012-8011(�绰)����ϼ


#include "AllHead.h"
#include "Terminal_ReadMeter_3762_CX.h"
#include "Terminal_ReadMeter_3762.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../LPC177x8x/lpc177x8x_UART.h"
#include "../LPC177x8x/lpc177x8x_IO.h"
#include "../Calculate/Calculate.h"
#include "../Driver/MEMRW.h"
#include "../Display/Warning.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"



#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��

#define RouterCheckTimerVal     (60*10)//�´��ز�·��������붨ʱֵ

void Terminal_ReadMeter_3762_CX(u32 PORTn)//�����ز�ģ��
{
	u32 i;
	u8 * p8rxbuff;
//	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
//	u16 * p16timer;
//	u32 * p32;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
//	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(Terminal_Router->RouterInitTask)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
	{
		case 100://����·������ģʽ
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 101://����·������
			if(Terminal_Router->NoteNum!=0)//��ǰ·��������֪�ܴӽڵ���
			{
				//���ѧϰʱ�����
				i=Terminal_Router->NoteNum;
				i*=30;//ƽ��ÿ��ѧϰʱ��30��
				if(i>0xffff)
				{
					i=0xffff;
				}
				Terminal_Ram->RouterCheck_S_Timer=i;
				Terminal_Router->RouterCtrl=1;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				Terminal_Router->RouterState=1;//·������ǰ״̬0=��ͣ,1=����
			}
			Terminal_Router->RouterInitSubTask=0;//������
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 102://��ѯ·���������
			if(Terminal_Router->NoteNum==0)//��ǰ·��������֪�ܴӽڵ���
			{
				Terminal_Router->RouterInitSubTask=0;//������
				Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
				break;
			}
			
			if(MEMReadReg(ADDR_AFN05F49+30,1)==0x55)//����ָ��ͨ�Ŷ˿���ͣ����
			{
				WarningOccur(PORTn,(u8 *)"�ز�������ͣ����");//�澯����,��ڸ澯����͸澯�ַ���
				if(Terminal_Router->RouterState==1)//·������ǰ״̬0=��ͣ,1=����
				{
					Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
					Terminal_Router->RouterState=0;//·������ǰ״̬0=��ͣ,1=����
				}
				break;
			}
			i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//�ն˳������в�������,�˿�31
			if((i&0x01)!=0x0)
			{//D0��"1"�������Զ�������"0" Ҫ���ն˸��ݳ���ʱ���Զ�����
				WarningOccur(PORTn,(u8 *)"�ز��������Զ�����");//�澯����,��ڸ澯����͸澯�ַ���
				if(Terminal_Router->RouterState==1)//·������ǰ״̬0=��ͣ,1=����
				{
					Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
					Terminal_Router->RouterState=0;//·������ǰ״̬0=��ͣ,1=����
				}
				break;
			}
			if(UARTCtrl->RMFlag==0)//1=���������,0=��������ͣ
			{
				WarningOccur(PORTn,(u8 *)"�ز��ǳ���ʱ��");//�澯����,��ڸ澯����͸澯�ַ���
				if(Terminal_Router->RouterState==1)//·������ǰ״̬0=��ͣ,1=����
				{
					Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
					Terminal_Router->RouterState=0;//·������ǰ״̬0=��ͣ,1=����
				}
				break;
			}
			if(Terminal_Router->RouterState==0)//·������ǰ״̬0=��ͣ,1=����
			{
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				Terminal_Router->RouterState=1;//·������ǰ״̬0=��ͣ,1=����
				break;
			}
		
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					Uart_3762_TxData(PORTn,0x10,4,0);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
					Terminal_Router->RouterInitSubTask=1;
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=׼������
					{
						case 1://1=���1֡����
							UARTCtrl->Task=0;//0=����,����ת���Ȳ������
							i=p8rxbuff[13];
							if(i&1)
							{//B0·����ɱ�־:1Ϊ·��ѧϰ���,0Ϊδ���		
								Terminal_Router->RouterInitSubTask=0;//������
								Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							}
							else
							{
								if(Terminal_Ram->RouterCheck_S_Timer==0)
								{//���ѧϰʱ�䵽
									Terminal_Router->RouterInitSubTask=0;//������
									Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
									break;
								}
								if(Terminal_Router->AFN04F10==0)//AFN04F10���ñ仯��־0=û,1=�仯
								{
									Terminal_Router->RouterInitSubTask=0;//������
								}
								else
								{
									Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
								}
							}
							break;
						case 0://0=����
						case 2://2=���ճ�ʱ(�����)
							Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
			}
			break;
		case 103://ֹͣ·������,���г���
			Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
			Terminal_Router->RouterState=0;//·������ǰ״̬0=��ͣ,1=����
			Terminal_Router->RouterInitSubTask=0;//������
			Terminal_Router->RouterInitTask++;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		case 104:
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//�´��ز�·��������붨ʱֵ
			UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=׼������
			Terminal_Router->RouterInitTask=200;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
		
		
		case 200://>=200·�ɳ���
//			p32=(u32*)(ADDR_E4_ENTRY);
//			if((p32[64]==ADDR_E4_ENTRY)||(Terminal_Router->UpDateTask!=0))//�����������
//			{
//				UpData_CX(PORTn);//·���������
//				break;
//			}

			if(UARTCtrl->Task==0)//0=����
			{
				if(Terminal_Router->AFN04F10!=0)//AFN04F10���ñ仯��־0=û,1=�仯
				{
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
					break;
				}
				
				if(Terminal_Ram->RouterCheck_S_Timer==0)//24 �ز�·��������붨ʱ��
				{
					Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//�´��ز�·��������붨ʱ��
					Terminal_Router->RouterCtrl=4;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
					break;
				}
				if(MEMReadReg(ADDR_AFN05F49+30,1)==0x55)//����ָ��ͨ�Ŷ˿���ͣ����
				{
					WarningOccur(PORTn,(u8 *)"�ز�������ͣ����");//�澯����,��ڸ澯����͸澯�ַ���
					break;
				}
				i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//�ն˳������в�������,�˿�31
				if((i&0x01)!=0x0)
				{//D0��"1"�������Զ�������"0" Ҫ���ն˸��ݳ���ʱ���Զ�����
					WarningOccur(PORTn,(u8 *)"�ز��������Զ�����");//�澯����,��ڸ澯����͸澯�ַ���
					break;
				}
				if(UARTCtrl->RMFlag==0)//1=���������,0=��������ͣ
				{
					WarningOccur(PORTn,(u8 *)"�ز��ǳ���ʱ��");//�澯����,��ڸ澯����͸澯�ַ���
					break;
				}
			}
			if(Terminal_Router->NoteNum==0)
			{//�ڵ���
				DisplayRouterVer(PORTn);//��ʾ·�ɳ��ҺͰ汾��
				break;
			}
			
			if(UARTCtrl->RMKeyUser==0)//�����ص㻧��־,0=���泭��,1=�ص㻧����
			{
				GeneralReadMeter_3762_CX(PORTn);//�ز�ģ��,���泭��
			}
			else
			{
				KeyUserReadMeter_3762_CX(PORTn);//�ز�ģ��,�ص㻧����
			}
			break;
		default:
			Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			break;
	}
}


extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const u32 DL645_1997_RMLib[];
u32 Get_RM_DL645_1997_MaxFn(void);

void GeneralReadMeter_3762_CX(u32 PORTn)//�ز�ģ��,���泭��
{
	u32 i;
	u32 x;
	u32 y;
	u32 ClassNo;
	u32 ListNo;
	u32 Fn;
	u32 Pn;
	u32 Protocol;
	u32 AddrAFN04F10;
	u64 d64a;
	u64 d64b;
	
	u8 * p8RMLib;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=׼������
	{
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			break;
		case 0://0=����
			i=MEMReadReg(ADDR_YMDHMS+1,2);
			if((i>0x2350)||(i<0x0010))
			{//��ÿ��0�㸽����ͣ����
				UARTCtrl->Task=0;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}

			ListNo=UARTCtrl->GRMCount;//�������ܱ����(���̺���̩�ز�)
			if(ListNo>=Terminal_Router->NoteNum)
			{//�������>=��ǰ·�������ܴӽڵ���
				ListNo=0;
				UARTCtrl->GRMCount=0;
				break;
			}
			x=Terminal_Note->List[ListNo].MeterNo;//ȡ��Ӧ���ܱ�/��������װ�����(��1��ʼ)
			if((x==0)||(x>AFN04F10MaxPn))
			{//��Ŵ�
		RMOK:
				Terminal_Note->List[ListNo].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
				goto RMEND;
			}
			i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//�ն˳������в�������,�˿�31
			if((i&0x02)!=0x0)
			{//D1��"1"Ҫ���ն�ֻ���ص����"0"Ҫ���ն˳����б�
				y=MEMReadReg(ADDR_AFN04F35,1);//̨�����г����ص㻧����
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MEMReadReg(ADDR_AFN04F35+1+(2*i),2)==x)
					{
						break;
					}
				}
				if(i==y)
				{//�����ص㻧
					goto RMOK;//����,ֱ�ӷ��س����ɹ�
				}
			}
			x--;
			
			AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
			d64a=MEMReadReg(AddrAFN04F10+6,6);//ȡ���õ�ͨ�ŵ�ַ
			Pn=MEMReadReg(AddrAFN04F10+2,2);//ȡ���õĲ������
			if(Pn==0)
			{//����ɾ��
				goto RMOK;//����,ֱ�ӷ��س����ɹ�
			}
			i=MEMReadReg(ADDR_AFN04F30+(Pn-1),1);//̨�����г���ͣ��/Ͷ������(�������)
			if(i!=0)
			{//ͣ��
				goto RMOK;//����,ֱ�ӷ��س����ɹ�
			}
			if(Terminal_Note->List[ListNo].ReCount>=1)
			{//�����ط�����>=1
				Terminal_Note->List[ListNo].RMErr=1;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
				ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
				goto RMEND;//��������
			}
			Terminal_Note->List[ListNo].ReCount++;//�����ط�����+1
			Protocol=MEMReadReg(AddrAFN04F10+5,1);//ȡͨ��Э������:1=DL/T645-1997,30=DL/T645-2007
			ClassNo=MEMReadReg(AddrAFN04F10+26,1);//ȡ��С���
			
			while(1)
			{
				if(Protocol==1)
				{//DL/T645-1997
					i=Get_RM_DL645_1997_MaxFn();
				}
				else
				{//DL/T645-2007
					i=Get_RM_DL645_2007_MaxFn();
				}
				if(Terminal_Note->List[ListNo].FnCount>=i)
				{//Fn����>=���Fn����,���ֳ������
			RMEND:
					DisplayRouterVer(PORTn);//��ʾ·�ɳ��ҺͰ汾��
					Terminal_Note->List[ListNo].RMFlag=2;//�����־;0=�ϵ���ո���,1=����ǿ���س�,2=���һ�ֳ���
			 		Terminal_Note->List[ListNo].FnCount=0;//Fn����
					Terminal_Note->List[ListNo].DICount=0;//�������ݱ�ʶ����
					Terminal_Note->List[ListNo].ReCount=0;//�����ط�����
					if(Terminal_Note->List[ListNo].RMErr==0)//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
					{
						Terminal_Note->List[ListNo].OldRMErr=2;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
					}
					else
					{
						if(Terminal_Note->List[ListNo].OldRMErr<2)//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
						{
							Terminal_Note->List[ListNo].OldRMErr=1;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
						}
					}
					Terminal_Note->List[ListNo].RMErr=0;//��ǰ����ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��
					UARTCtrl->GRMCount++;//�������ܱ����(���̺���̩�ز�)
					return;
				}

				if(Protocol==1)
				{//DL/T645-1997
					i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
				}
				else
				{//DL/T645-2007
					i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
				}
				if(i==0)
				{//��Fn�ǿ�
					goto NEXT_Fn;
				}
				p8RMLib=(u8 *)i;//�����
				if((p8RMLib[3]&0x04)==0)//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
				{
					goto NEXT_Fn;
				}
				
				
				y=0;
				if((p8RMLib[4]&0x01)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				{//1������,ʵʱ
					Fn=p8RMLib[0];//ȡʵʱFn
					y=Class1Data_Fn(ClassNo,Fn);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
					//i=GetClass1DataAddr(Fn,Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
					//if(i!=0)
					//{
					//}
				}
				x=0;
				if((p8RMLib[4]&0x08)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				{//2������B3=�ն���
					Fn=p8RMLib[7];//ȡ�ն���Fn
					i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
					if(i!=0)
					{
						MEMRead(ADDR_DATABUFF,i,5);//ȡԭ����ʱ��������ʱ��
						x|=DataComp(ADDR_DATABUFF+2,ADDR_YMDHMS+3,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
					}
				}
				if((p8RMLib[4]&0x20)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				{//2������B5=�¶���
					Fn=p8RMLib[9];//ȡ�¶���Fn
					i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
					if(i!=0)
					{
						MEMRead(ADDR_DATABUFF,i,5);//ȡԭ����ʱ��������ʱ��
						x|=DataComp(ADDR_DATABUFF+3,ADDR_YMDHMS+4,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
					}
				}
				if(x==0)
				{//����ʱ����ͬ
					if(Terminal_Note->List[ListNo].RMFlag==1)//�����־;0=�ϵ���ո���,1=����ǿ���س�,2=���
					{
						y|=Class2Data_Fn(ClassNo,Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
					}
				}
				else
				{
					y|=Class2Data_Fn(ClassNo,Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
				if(y==0x0)
				{//��Fn����
		NEXT_Fn:
					Terminal_Note->List[ListNo].FnCount++;//Fn����+1
					Terminal_Note->List[ListNo].DICount=0;//�������ݱ�ʶ����
					Terminal_Note->List[ListNo].ReCount=0;//�ط�����
					continue;
				}
				if(p8RMLib[1]==0x0)
				{//��Fn����������=0
					if((p8RMLib[4]&0x01)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
					{//1������,ʵʱ
						Fn=p8RMLib[0];//ȡʵʱFn
						switch(Fn)
						{
						//F33 ��ǰ������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						//F34 ��ǰ������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						//F35 ����������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						//F36 ���·�����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
							case 33:
							case 34:
							case 35:
							case 36:
						//F37 ���£���һ�����գ�������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						//F38 ���£���һ�����գ�������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
						//F39 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
						//F40 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M�12�
							case 37:
							case 38:
							case 39:
							case 40:
								break;
							default:
								goto NEXT_Fn;
						}
						p8FnData[0]=0x53;//ǿ�Ƴ���ɹ�
						ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
						goto NEXT_Fn;
					}
				}
				if(Terminal_Note->List[ListNo].DICount==0)//�������ݱ�ʶ����
				{//�������ݱ�ʶ����=0,��Fn���ݻ���
					MEMClr(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				if(Terminal_Note->List[ListNo].DICount>=p8RMLib[1])
				{//�������ݱ�ʶ����>=Fn�跢������;��Fn�������
					ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
					goto NEXT_Fn;
				}
				
				
				p8txbuff[0]=0x68;
				p8txbuff[3]=0x41;//������Ϊ0x41
				p8txbuff[4]=4;//ͨ��ģ���ʶ=1
				p8txbuff[5]=0;
				p8txbuff[6]=0;
				p8txbuff[7]=0;
				p8txbuff[8]=0;
				p8txbuff[9]=0;
				MEMClr(0,((u32)p8txbuff)+10,6);
				MEMRead(((u32)p8txbuff)+10,ADDR_TERMINALADDR,2);;//���ڵ��ַ(Դ��ַ)(ͬ�ն˵�ַ)
				MEMWriteReg(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
				p8txbuff[22]=0x13;//AFN=13H
				p8txbuff[23]=1;//Fn=1
				p8txbuff[24]=0;
				if(Protocol==1)
				{//dl645-1997
					p8txbuff[25]=1;//2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
					p8txbuff[26]=0;//�����ڵ���=0
					p8txbuff[27]=14;//DL645���ĳ���
					p8txbuff[28]=0x68;
					MEMWriteReg(d64a,((u32)p8txbuff)+29,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
					p8txbuff[35]=0x68;
					p8txbuff[36]=0x01;
					p8txbuff[37]=2;
					p8txbuff[38]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)]+0x33;//ȡDIͬʱ��0x33
					p8txbuff[39]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+1]+0x33;
					p8txbuff[40]=0;
					p8txbuff[41]=0x16;
					for(i=0;i<12;i++)
					{
						p8txbuff[40]+=p8txbuff[28+i];
					}
				}
				else
				{//dl645-2007
					i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)];
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]<<8;
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]<<16;
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]<<24;
					switch(i)
					{//����������̫֡������
						case 0x03300101://F27 ���һ�ε�����㷢��ʱ��
						case 0x03300201://F27 ���һ���������㷢��ʱ��
						case 0x03300501://F166 ���һ�ε��ܱ�ʱ�β����޸�ʱ��
//						case 0x03300001://(����̩)F27 ���һ�α�̷���ʱ��
//						case 0x1E0002FF://(����̩)F161 ��1-10�κ�բ�����ߴ���
//						case 0x1E0001FF://(����̩)F161 ��1-10�κ�բ����ʱ��
//						case 0x1D0002FF://(����̩)F161 ��1-10����բ�����ߴ���
//						case 0x1D0001FF://(����̩)F161 ��1-10����բ����ʱ��
							Terminal_Note->List[ListNo].ReCount=0;//�����ط�����=0
							Terminal_Note->List[ListNo].DICount++;//�����������
							return;
					}
					p8txbuff[25]=2;//2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
					p8txbuff[26]=0;//�����ڵ���=0
					p8txbuff[27]=16;//DL645���ĳ���
					p8txbuff[28]=0x68;
					MEMWriteReg(d64a,((u32)p8txbuff)+29,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
					p8txbuff[35]=0x68;
					p8txbuff[36]=0x11;
					p8txbuff[37]=4;
					p8txbuff[38]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)]+0x33;//ȡDIͬʱ��0x33
					p8txbuff[39]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]+0x33;
					p8txbuff[40]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]+0x33;
					p8txbuff[41]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]+0x33;
					p8txbuff[42]=0;
					p8txbuff[43]=0x16;
					for(i=0;i<14;i++)
					{
						p8txbuff[42]+=p8txbuff[28+i];
					}
				}
				i=28+p8txbuff[27]+2;//���ܷ����ֽ���
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
				UARTCtrl->TxByte=i;//�����ֽ���(���ֶ���)
				UARTCtrl->Task=3;//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				UARTCtrl->RouterTimeOut=60000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)

				//��ʾ�����ַ
				CopyString((u8 *)"�ز��ڳ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				for(i=0;i<6;i++)
				{
					x=p8txbuff[16+5-i];
					MEMWriteReg((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
					MEMWriteReg((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
				}
				MEMWriteReg(0,ADDR_DATABUFF+8+12,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				return;
			}
			//break;
		case 1://1=���1֡����
			UARTCtrl->Task=0;//����1��
			ListNo=UARTCtrl->GRMCount;//�������ܱ����(���̺���̩�ز�)
			if(ListNo>=Terminal_Router->NoteNum)
			{//�������>=��ǰ·�������ܴӽڵ���
				ListNo=0;
				UARTCtrl->GRMCount=0;
			}
			x=Terminal_Note->List[ListNo].MeterNo;//ȡ��Ӧ���ܱ�/��������װ�����(��1��ʼ)
			if((x==0)||(x>AFN04F10MaxPn))
			{//��Ŵ�
		rxerr:
				break;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if(Pn==0)
			{//������Ŵ�
				goto rxerr;
			}
			i=MEMReadReg(x+3,1);//ͨ��Э������ BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//ͨ��Э�鲻֪
				goto rxerr;
			}
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
			}
			p8RMLib=(u8 *)i;//�����
			
			d64a=MEMReadReg(x+4,6);//ȡ���õ�ͨ�ŵ�ַ
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//���ϻش�
				goto rxerr;
//				Terminal_Note->List[ListNo].ReCount=0;//�����ط�����=0
//				Terminal_Note->List[ListNo].DICount++;//�������ݱ�ʶ+1
//				break;
			}
			y=p8rxbuff[26];//���������򳤶�
			MEMRead((u32)p8rxbuff,((u32)p8rxbuff)+26+1,y);
			
			d64b=MEMReadReg(((u32)p8rxbuff)+1,6);//���صĵ�ַ
			if(d64a!=d64b)
			{//���ַ����ͬ
				goto rxerr;
			}
			x=0;//��У�����
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//��У���
				goto rxerr;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//��վ�쳣�ش�,�޴�������,��������
				Terminal_Note->List[ListNo].ReCount=0;//�����ط�����=0
				Terminal_Note->List[ListNo].DICount++;//�������ݱ�ʶ+1
				break;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//�������0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//DL/T645-1997
				x=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)];
				x|=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+1]<<8;
				y=MEMReadReg(((u32)p8rxbuff)+10,2);
				if(x!=y)
				{//DI����
					goto rxerr;
				}
				i=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+2];
				if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
				{//���س�����ͬ
					UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
					UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//����Fn����
					UARTCtrl->RMCount=Terminal_Note->List[ListNo].DICount;//�������ݱ�ʶ����
					ReadMeter_DL645_1997_FnData(PORTn);//DL645_1997���������������Fn���ݻ���
					Terminal_Note->List[ListNo].DICount=UARTCtrl->RMCount;//���صĳ������ݱ�ʶ����
					ReadMeterNote_OK(PORTn,Pn);//���г���������Ϣ.�ɹ�
				}
				Terminal_Note->List[ListNo].ReCount=0;//�����ط�����=0
				Terminal_Note->List[ListNo].DICount++;//�������ݱ�ʶ+1
			}
			else
			{//DL/T645-2007
				x=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)];
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]<<8;
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]<<16;
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]<<24;
				y=MEMReadReg(((u32)p8rxbuff)+10,4);
				if(x!=y)
				{//DI����
					goto rxerr;
				}
				i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+4];
				if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
				{//���س�����ͬ
					UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//��Ӧ���ܱ��������(���ֶ���)(��1��ʼ)
					UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//����Fn����
					UARTCtrl->RMCount=Terminal_Note->List[ListNo].DICount;//�������ݱ�ʶ����
					ReadMeter_DL645_2007_FnData(PORTn);//DL645_2007���������������Fn���ݻ���
					Terminal_Note->List[ListNo].DICount=UARTCtrl->RMCount;//���صĳ������ݱ�ʶ����
					ReadMeterNote_OK(PORTn,Pn);//���г���������Ϣ.�ɹ�
				}
				Terminal_Note->List[ListNo].ReCount=0;//�����ط�����=0
				Terminal_Note->List[ListNo].DICount++;//�������ݱ�ʶ+1
			}
			
			break;
	}
}

void KeyUserReadMeter_3762_CX(u32 PORTn)//�ز�ģ��,�ص㻧����
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Pn;
	u64 d64a;
	u64 d64b;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p8FnData=(u8 *)ADDR_DATABUFF;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterKeyUserTask)//�ص㻧��������
	{
		case 0://��ͣ·��
			if(UARTCtrl->Task==0)
			{
				if(Terminal_Router->RouterStop==0)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=...
				{
					Terminal_Router->RouterStop=1;//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=...
					Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				}
				Terminal_Router->RouterKeyUserTask++;//�ص㻧��������
			}
			else
			{
				GeneralReadMeter_3762_CX(PORTn);//�ز�ģ��,���泭��
			}
			break;
		case 1://AFN13 F1 ·������ת��:����ز��ӽڵ�
			i=MEMReadReg(ADDR_AFN04F35,1);//̨�����г����ص㻧����
			if(i>MaxKeyUser)
			{
				i=MaxKeyUser;
			}
			if(UARTCtrl->KeyUserRMCount>=i)//�ص㻧�������ܱ����
			{//ȫ���ص㻧����
				UARTCtrl->KeyUserRMCount=0;//�ص㻧�������ܱ����
				Terminal_Router->RouterKeyUserTask=0;//�ص㻧��������
				UARTCtrl->KeyUserRMOKCountLast=UARTCtrl->KeyUserRMOKCount;//�ϴ��ص㻧�����ɹ�����
				UARTCtrl->KeyUserRMOKCount=0;//��ǰ�ص㻧�����ɹ�����
				if(Terminal_Router->RouterStop==1)//��ͣ·�ɱ�ʶ:0=��,1=���ص㻧,2=����ת��,3=UART��·����Ϣ,4=�㲥Уʱ,5=�㲥����,6=ͨ�ϵ����,7=...
				{
					Terminal_Router->RouterCtrl=3;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				}
				UARTCtrl->RMKeyUser=0;//�����ص㻧��־,0=���泭��,1=�ص㻧����
				//Сʱ�������ص㻧���������AFN04F10���ñ仯���ʼ��·����
				if(Terminal_Router->AFN04F10!=0)//AFN04F10���ñ仯��־0=û,1=�仯
				{
					Terminal_Router->AFN04F10=0;//AFN04F10���ñ仯��־0=û,1=�仯
					Terminal_Router->RouterInitTask=0;//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
				}
				return;
			}
			x=MEMReadReg(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//�ص㻧�ĵ��ܱ�/��������װ�����
			if((x==0)||(x>AFN04F10MaxPn))
			{//װ����Ŵ�
				UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
				break;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//������Ŵ�
				UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
				break;
			}
			i=MEMReadReg(x+2,1);
			i&=0x1f;
			if(i!=31)
			{//�˿ڲ����ز�
				UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
				break;
			}
			i=MEMReadReg(x+3,1);//ͨ��Э������ BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//ͨ��Э�鲻֪
				UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
				break;
			}

			p8txbuff[0]=0x68;
			p8txbuff[3]=0x41;//������Ϊ0x41
			p8txbuff[4]=4;//ͨ��ģ���ʶ=1
			p8txbuff[5]=0;
			p8txbuff[6]=0;
			p8txbuff[7]=0;
			p8txbuff[8]=0;
			p8txbuff[9]=0;
			MEMClr(0,((u32)p8txbuff)+10,6);
			MEMRead(((u32)p8txbuff)+10,ADDR_TERMINALADDR,2);;//���ڵ��ַ(Դ��ַ)(ͬ�ն˵�ַ)
			MEMRead(((u32)p8txbuff)+16,x+4,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
			p8txbuff[22]=0x13;//AFN=13H
			p8txbuff[23]=1;//Fn=1
			p8txbuff[24]=0;
			if(Protocol==1)
			{//dl645-1997
				p8txbuff[25]=1;//2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
				p8txbuff[26]=0;//�����ڵ���=0
				p8txbuff[27]=14;//DL645���ĳ���
				p8txbuff[28]=0x68;
				MEMRead(((u32)p8txbuff)+29,x+4,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
				p8txbuff[35]=0x68;
				p8txbuff[36]=0x01;
				p8txbuff[37]=2;
				p8txbuff[38]=0x0f+0x33;//DI=0x901f
				p8txbuff[39]=0x90+0x33;
				p8txbuff[40]=0;
				p8txbuff[41]=0x16;
				for(i=0;i<12;i++)
				{
					p8txbuff[40]+=p8txbuff[28+i];
				}
			}
			else
			{//dl645-2007
				p8txbuff[25]=2;//2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
				p8txbuff[26]=0;//�����ڵ���=0
				p8txbuff[27]=16;//DL645���ĳ���
				p8txbuff[28]=0x68;
				MEMRead(((u32)p8txbuff)+29,x+4,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
				p8txbuff[35]=0x68;
				p8txbuff[36]=0x11;
				p8txbuff[37]=4;
				p8txbuff[38]=0x00+0x33;//DI=0x0001FF00
				p8txbuff[39]=0x32;//0xff+0x33;
				p8txbuff[40]=0x01+0x33;
				p8txbuff[41]=0x00+0x33;
				p8txbuff[42]=0;
				p8txbuff[43]=0x16;
				for(i=0;i<14;i++)
				{
					p8txbuff[42]+=p8txbuff[28+i];
				}
			}
			i=28+p8txbuff[27]+2;//���ܷ����ֽ���
			p8txbuff[1]=i;
			p8txbuff[2]=i>>8;
			Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
			UARTCtrl->TxByte=i;//�����ֽ���(���ֶ���)
			UARTCtrl->Task=3;
			UARTCtrl->RouterTimeOut=60000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			//��ʾ�����ַ
			CopyString((u8 *)"���ص㻧",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[16+5-i];
				MEMWriteReg((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MEMWriteReg((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MEMWriteReg(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			Terminal_Router->RouterKeyUserTask++;//�ص㻧��������
			break;
		case 2://����
			if(UARTCtrl->Task>=3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			x=MEMReadReg(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//�ص㻧�ĵ��ܱ�/��������װ�����
			if((x==0)||(x>AFN04F10MaxPn))
			{//װ����Ŵ�
				goto rxerr;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if((Pn==0)||(Pn>AFN0CF129KeyUserMaxPn))
			{//������Ŵ�
				goto rxerr;
			}
			i=MEMReadReg(x+3,1);//ͨ��Э������ BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//ͨ��Э�鲻֪
				goto rxerr;
			}
			d64a=MEMReadReg(x+4,6);//ȡ���õ�ͨ�ŵ�ַ
			MEMClr(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
			MEMRead(((u32)p8FnData)+1,ADDR_YMDHMS+1,5);//����ʱ��
			
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//���ϻش�
		rxerr:
				UARTCtrl->Task=0;//����1��
				UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
				Terminal_Router->RouterKeyUserTask--;//�ص㻧��������
				break;
			}
			y=p8rxbuff[26];//���������򳤶�
			if(UARTCtrl->Task!=1)
			{
				y=0;//�������ݳ���
			}
			else
			{
				i=MEMReadReg(((u32)p8rxbuff)+22,3);
				if(i!=0x113)
				{//AFN=13H,Fn=1
					y=0;//�������ݳ���
				}
			}
			if(y==0)
			{//�������ݳ���=0
				goto rxerr;
			}
			MEMRead((u32)p8rxbuff,((u32)p8rxbuff)+26+1,y);
			
			d64b=MEMReadReg(((u32)p8rxbuff)+1,6);//���صĵ�ַ
			if(d64a!=d64b)
			{//���ַ����ͬ
				goto rxerr;
			}
			x=0;//��У�����
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//��У���
				goto rxerr;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//�������վ�ش��쳣
				goto rxerr;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//�������0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//1997
				if(p8rxbuff[9]<=2)
				{//������
					goto rxerr;
				}
				i=MEMReadReg(((u32)p8rxbuff)+10,2);
				if(i!=0x901f)
				{
					goto rxerr;
				}
			}
			else
			{//2007
				if(p8rxbuff[9]<=4)
				{//������
					goto rxerr;
				}
				i=MEMReadReg(((u32)p8rxbuff)+10,4);
				if(i!=0x0001FF00)
				{
					goto rxerr;
				}
			}
			for(i=0;i<p8rxbuff[9];i++)
			{
				if(p8rxbuff[10+i]==0xff)
				{
					p8rxbuff[10+i]=0;//��������Ч������0xff
				}
			}
			if(Protocol==1)
			{//1997
				i=p8rxbuff[9];
				if(i>=6)
				{
					i-=6;
					i/=4;
					if(i>MaxTariff)
					{
						i=MaxTariff;
					}
					p8FnData[6]=i;
					i+=1;
					p8FnData+=7;
					p8rxbuff+=12;
					for(;i!=0x0;i--)
					{
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						p8FnData+=5;
						p8rxbuff+=4;
					}
				}
			}
			else
			{//2007
				i=p8rxbuff[9];
				if(i>=8)
				{
					i-=8;
					i/=4;
					if(i>MaxTariff)
					{
						i=MaxTariff;
					}
					p8FnData[6]=i;
					i+=1;
					p8FnData+=7;
					p8rxbuff+=14;
					for(;i!=0x0;i--)
					{
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						p8FnData+=5;
						p8rxbuff+=4;
					}
				}
			}
			//SaveClass1Data(129,Pn,((u32)p8FnData)+1);//������1����
			MEMWrite(((u32)p8FnData)+1,ADDR_AFN0CF129_KeyUser+((6+(5*(1+MaxTariff)))*(Pn-1)),6+(5*(1+MaxTariff)));
			
			UARTCtrl->Task=0;//����1��
			UARTCtrl->KeyUserRMOKCount++;//��ǰ�ص㻧�����ɹ�����
			UARTCtrl->KeyUserRMCount++;//�ص㻧�������ܱ����
			Terminal_Router->RouterKeyUserTask--;//�ص㻧��������
			break;
		default:
			Terminal_Router->RouterKeyUserTask=0;//�ص㻧������ʼ������
			break;
	}	
}


void UpData_CX(u32 PORTn)//����·���������
{

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






