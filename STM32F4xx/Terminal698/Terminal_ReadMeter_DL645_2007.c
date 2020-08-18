
//�ն˳���RS485��_DL645_2007
#include "Project.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"

	#if (USER/100)==8//�û���ʶ��ͨ��
#include "../Driver/DMW.h"
	#endif

extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];


void Terminal_ReadMeter_DL645_2007(u32 PORTn)//�ն˳���RS485��_DL645_2007
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	u32 z;
#endif
	u32 Fn;
	u32 Pn;
	u8 * p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
	u32 ClassNo;
	u64 YMDHM;
#if Extern_ReadMeter_Fn_YesNo!=0//�ⲿ����:0=I2,1=E1,����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)
	u32 *p32;
#endif
#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
	u8 *p8;
#endif

//	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	
	
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			if(UARTCtrl->BatchTask>=1)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			{
				return;
			}
/*
#if RMM_RS485_PassageZero==1//RS485ͨ�������0��;0=����ͣ,1=��ͣ
			i=MRR(ADDR_TYMDHMS+1,2);
			if((i>=0x2355)||(i<RMM_StartHM))
			{//RS485����ÿ��0�㸽����ͣ����
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						PORT485=1;//RS485PORT=1;
						break;
					case RS485_2PORT://RS485-2
						PORT485=2;//RS485PORT=2;
						break;
					case RS485_3PORT://RS485-3
						PORT485=3;//RS485PORT=3;
						break;
					default:
						return;
				}
				MC(0,ADDR_DATABUFF,21);
				CopyString((u8 *)"485 ͣ���ȴ�ʱ�ӹ�0",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				MWR(PORT485,ADDR_DATABUFF+3,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				return;
			}
#endif
*/
			if(p16timer[0]==0x0)
			{//��֡����������ʱ
				UARTCtrl->Task=3;//��������
			}
			break;
		case 1://1=���1֡����
			UARTCtrl->Task=0;//����1��
			break;
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			if(UARTCtrl->ReRMCount<2)
			{
				UARTCtrl->ReRMCount+=1;//���������ط�����+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//�����������+1
				UARTCtrl->ReRMCount=0;//���������ط�����
				Fn=MRR(DL645_2007_RMLib[UARTCtrl->FnCount],1);
				if(Fn==3)
				{
					UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
					UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
					UARTCtrl->RMDI=0xffffffff;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
				}
				if(Fn!=0xE1)
				{
					UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				}
				switch(Fn)
				{
					default:
						UARTCtrl->RMError|=1;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
						break;
					case 129://F129 ��ǰ�����й�����ʾֵ
					case 28://�������״̬�ּ����λ��־
					case 0xE3://��չ3 ���г����ն����������޹�,4�������޹�����ʾֵ���ܡ�����1��M��
						UARTCtrl->RMError|=2;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
						break;
					case 0xE1://��չ1 ERC8���ܱ��������¼�������
						i=UARTCtrl->RMCount;
						if(i<2)//�����������
						{//��1�׵�1-2��ʱ�α�
							UARTCtrl->RMCount=2;
							break;
						}
						if(i<4)//�����������
						{//��2�׵�1-2��ʱ�α�
							UARTCtrl->RMCount=4;
						}
						return;
				#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					case 2://�����ܱ������ϱ��¼�
						//���¼���־,����ѭ��
						MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
						return;
				#endif
				}
			}
			break;
		case 3://3=��������
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//����ŵĵ��ܱ�/��������װ��ɾ��
				goto END_Fn;//����
			}
			if(AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]>=2)//����������ɱ�־:0=û���,1=��ɵ�������1�����ݱ�ʶ����ʧ��,2=�ɹ����,3=���賭
			{
				goto END_Fn;//�ѳɹ�����
			}
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					PORT485=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					PORT485=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					PORT485=3;//RS485PORT=3;
					break;
				default://�ڲ������������ز���
					return;
			}
#if (((Project/100)==2)||((Project/100)==5))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//�ն˳������в�������
			if((i&0x02)!=0x0)
			{//D1��"1"Ҫ���ն�ֻ���ص����"0"Ҫ���ն˳����б�
				y=MRR(ADDR_AFN04F35,1);//̨�����г����ص㻧����
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MRR(ADDR_AFN04F35+1+(2*i),2)==UARTCtrl->MeterNo)
					{
						break;
					}
				}
				if(i==y)
				{//�����ص㻧
					goto END_Fn;//����
				}
			}
			i=MRR(ADDR_AFN04F30+(Pn-1),1);//̨�����г���ͣ��/Ͷ������(�������)
			if(i!=0)
			{//ͣ��
				goto END_Fn;//����
			}
#endif
			if(UARTCtrl->FnCount>=Get_RM_DL645_2007_MaxFn())
			{//Fn����>=���Fn����,���������
				if(UARTCtrl->RMError==0)
				{//����ɹ�
				#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
				#else
					if(UARTCtrl->RMMeterEvent==0)//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
					{
						AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
					}
				#endif
					ReadMeterNote_OK(PORTn,Pn,0);//���г���������Ϣ.�ɹ�
					DateRMFlags(Pn,0);//�ճ����־;���:f=0�ɹ�,=1ʧ��
				}
				else
				{//����ʧ��
				#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
				#else
					if(UARTCtrl->RMMeterEvent==0)//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
					{
						AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
					}
				#endif
					ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
					DateRMFlags(Pn,1);//�ճ����־;���:f=0�ɹ�,=1ʧ��
				}
	END_Fn:
				UARTCtrl->Task=0;//0=����
				UARTCtrl->FnCount=0;//Fn����=0
				UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
				UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
				UARTCtrl->RMDI=0xffffffff;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
				UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				UARTCtrl->RMError=0;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				UARTCtrl->RMMeterEvent=0;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
				break;
			}
			if(DL645_2007_RMLib[UARTCtrl->FnCount]==0x0)
			{//��Fn�޳���
	NEXT_Fn:
				UARTCtrl->Task=1;//���1֡����
				UARTCtrl->FnCount+=1;//Fn����+1
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				UARTCtrl->RMError&=0xfe;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				
	#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
				if((UARTCtrl->RMMeterEvent&1)==0)//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
				{//0=��
					UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
					UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
					UARTCtrl->RMDI=0xffffffff;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
					if(UARTCtrl->RMTaskC!=0)//��ǰ���������0-3:0=�ϵ�����,1=ÿ������,2=ÿ������,3=��ͣ���¼,(MaxRMTask-1)=ѭ������
					{//��������
						i=Check_Meter_Event_Save();//�����ܱ��¼������ռ�;����0=�洢�M(û�ϱ�),1-255�����ɴ洢������
						if(i>=30)
						{//�ռ�ɴ�>=30��,ÿ��10��
							i=Check_AutoReportWord(PORTn);//�����ܱ������ϱ�״̬��;����:0=��,!=0 B31-B16Ϊ�������(1��ʼ),B15-B0ΪҪ��ȡ�������ϱ�״̬��λ��(0-95)
							i>>=16;
							if(i!=0)
							{//�������ϱ���Ҫ��
								UARTCtrl->RMMeterEvent|=1;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
								UARTCtrl->MeterNoBackupOfAutoReport=UARTCtrl->MeterNo;//485������ܱ�/��������װ�����(���ֶ���)(ԭ���泭���װ����żĴ�)
								UARTCtrl->FnCountBackupOfAutoReport=UARTCtrl->FnCount;//485����Fn����(ԭ���泭���Fn�����Ĵ�)
								UARTCtrl->MeterNo=i;
								UARTCtrl->FnCount=2;
							}
						}
						if((UARTCtrl->RMMeterEvent&1)==0)//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
						{
							p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
							if(p8RMLib[0]==2)//F2
							{
								UARTCtrl->FnCount++;//Fn����+1;�������¼���������
							}
						}
					}
				}
				else
				{//��
					UARTCtrl->RMMeterEvent&=0xfe;//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
					UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485������ܱ�/��������װ�����(���ֶ���)(ԭ���泭���װ����żĴ�)
					UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfAutoReport;//485����Fn����(ԭ���泭���Fn�����Ĵ�)
				}
	#endif
	#endif
				break;
			}
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//��Fn����RS485����
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];

			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
		#if Extern_ReadMeter_Fn_YesNo==0//�ⲿ����:0=I2,1=E1,����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)	
			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,30);//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������
		#else
			y=0;
			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				p32=(u32 *)(ADDR_E1_ENTRY);
				y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)UARTCtrl->RMCount,(u32)30);//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������
			}
		#endif
	#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
			if(UARTCtrl->RMMeterEvent!=0)//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
			{
				y=1;//���������ϱ���������
			}
	#endif
	#endif
			if(Fn==26)
			{
			#if RMM_AFN0CF26==0//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
				if(UARTCtrl->RMCount==12)//�������ݱ�ʶ����
			#endif
			#if RMM_AFN0CF26==1//AFN0CF26�������ݳ���;0=���������¼����,1=�����ʧѹ��¼����
				if(UARTCtrl->RMCount==16)//�������ݱ�ʶ����
			#endif
				{//�����������16�����Ǽ�������,ֻ�в��ɹ�ʱ�ų�
					for(i=0;i<(1+61);i++)
					{
						if(p8FnData[i]!=0xee)
						{
							break;
						}
					}
					if(i!=(1+61))
					{//�г��ɹ�
						y=0;//����
						UARTCtrl->RMCount=100;//�������ݱ�ʶ����
					}
				}
			}
		#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
			if(y!=0)
			{
			//��չ14 ������2���ն�������
			//��չ15 ������3���ն�������
				if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
				{
					y=0;//û���1��ѭ������ʱ����
					x=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰ������
					x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
					if(p8RMLib[0]==0xEE)
					{
						y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
					}
					else
					{
						x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
						y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
					}
					y=MRR(y,3);//ȡ��?�ղ���ʱ��
					if(x==y)
					{//����
						goto NEXT_Fn;
					}
					x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
					i=GetDateCongealNo(x);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
					if(i!=0xffffffff)
					{//�ҵ�
						z=UARTCtrl->RMCount;//�����������
						if((z>0)&&(z<9))
						{
							z+=160;//�����������1-8��ӦFn=161-168
							z=GetClass2DataOfficeAddr(z,Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
							if(z!=0)
							{//�ҵ�
								y=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰ������
								z=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+z+2,3);//ȡԭ����ʱ��������
								if((z!=0xeeeeee)&&(z!=0xffffff))
								{
									if((z>x)&&(z<=y))
									{//ԭ����ʱ��>��2��ʱ��,ͬʱ<=��ǰʱ��
										goto NEXT_Fn;//����
									}
								}
							}
						}
					}
					while(1)
					{
						i=UARTCtrl->RMCount;//�������ݱ�ʶ����
						y=1;//�������ݱ�ʶ����=0��9����Ҫ��
						if(i>9)
						{
							break;
						}
						if((i>0)&&(i<9))
						{
							i+=160;//Ҫ����Fn=161-168
							y=Class2Data_Fn(ClassNo,i);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��		
						}
						if(y==0)
						{
							UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ����������
							UARTCtrl->RMCount++;//�������ݱ�ʶ����
						}
						else
						{
							break;
						}
					}
					if(UARTCtrl->RMCount>=p8RMLib[1])
					{//�������ݱ�ʶ����>=Fn�跢������;��Fn�������
						if(p8RMLib[1]>UARTCtrl->RMCountErr)
						{//����ȫ����ʶȫ������ʧ��
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
						}
						goto NEXT_Fn;
					}
				}
			}
		#endif
			if(y!=0)
			{
				if((p8RMLib[0]==0xE3)||(p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
				{//����1-3���ն�������
				#if ERC39==1
					if(p8RMLib[0]==0xEF)
					{
						if(UARTCtrl->RMCount<=1)
						{
							ERC39_Event(Pn);//����ʧ���¼���¼
						}
					}
				#endif
				#if (USER/100)==4//�û���ʶ:����
					if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
					{
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//���ܷ��ʸ���
						if(i==1)
						{//����2-3���ն�������
							y=0;//����ũ�������ն���
						}
					}
				#endif
					if(UARTCtrl->RMCount==9)
					{//�ٴ�ȷ���ն���ʱ��YYMMDDhhmm
						//ʡ�Բ���,�򳭱��ڹ�0�����ʱ����,�ڴ��жϲ��޸ĳ��������
						y=0;
					}
				}
			}
			if(y==0x0)
			{//�������
				if(p8RMLib[1]==0x0)
				{//��Fn����������=0
					goto NEXT_Fn;
				}
				if(UARTCtrl->RMCount==0)//�������ݱ�ʶ����
				{//�������ݱ�ʶ����=0,��Fn���ݻ���
					UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ����������
				UARTCtrl->RMCount++;//�������ݱ�ʶ����
				UARTCtrl->ReRMCount=0;//�ط�����
				if(UARTCtrl->RMCount>=p8RMLib[1])
				{//�����������>=Fn�跢������;��Fn�������
				#if(RMM_RS485_FailData_0xEE==0)//RS485����ʧ��������0xee;0=��,1=��
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//����ȫ����ʶ����
						goto OK_Fn;
					}
					goto NEXT_Fn;
				#else
					goto OK_Fn;
				#endif
				}
				return;
			}

			if(p8RMLib[1]==0x0)
			{//��Fn����������=0
			#if AFN0CF33MaxPn==0
				goto NEXT_Fn;
			#else
				if((p8RMLib[4]&0x01)!=0)//���ݷ���:B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				{//1������,ʵʱ
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
					ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
					goto NEXT_Fn;
				}
			#endif
			}

			if(UARTCtrl->RMCount==0x0)
			{//�����������=0,��Fn���ݻ���
				UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//�����������>=Fn�跢������;��Fn�������
			OK_Fn:
				if(Fn==129)//F129 ��ǰ�����й�����ʾֵ
				{
					if((UARTCtrl->RMError&0x2)==0)//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
					{
				#if(RMM_RS485_FailData_0xEE==1)//RS485����ʧ��������0xee;0=��,1=��
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				#else
						if(p8RMLib[1]>UARTCtrl->RMCountErr)
						{//����ȫ����ʶ��û��
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
						}
				#endif
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,0);//���ܱ���ɹ�ʧ�ܼ����ͱ�־,���UART�����ַ,���ܱ����,��־f=0�ɹ�,f=1ʧ��
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,0);//�ն�485����ʧ���¼�,��ڲ������,f=1����=0�ָ�
						#endif
						MC(0,ADDR_ERC21_Flag,1);//��ERC21 485��������¼���¼��־
					}
					else
					{
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,1);//���ܱ���ɹ�ʧ�ܼ����ͱ�־,���UART�����ַ,���ܱ����,��־f=0�ɹ�,f=1ʧ��
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,1);//�ն�485����ʧ���¼�,��ڲ������,f=1����=0�ָ�
						#endif
						#if ERC21==1
						ERC21_Event();//ERC21 485��������¼�
						#endif
				#if(RMM_RS485_FailData_0xEE==1)//RS485����ʧ��������0xee;0=��,1=��
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
				#else
						ClrERC29SOUR(Pn);//���г���ʧ����ERC29���ܱ�����¼�ԭʼ����
				#endif
					}
					goto NEXT_Fn;
				}
		#if(RMM_RS485_FailData_0xEE==1)//RS485����ʧ��������0xee;0=��,1=��
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
		#else
				if(UARTCtrl->RMError==0)//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				{
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//����ȫ����ʶ��û��
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
					}
				}
		#endif
				goto NEXT_Fn;
			}
			if(UARTCtrl->RMError!=0x0)
			{//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
		#if(RMM_RS485_FailData_0xEE==1)//RS485����ʧ��������0xee;0=��,1=��
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
		#endif
				//����ת��NEXT_Fn���¸�Fn���������ó���Ϊ�ɹ���־
			#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
				AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
			#else
				if(UARTCtrl->RMMeterEvent==0)//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
				{
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
				}
			#endif
				ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
				DateRMFlags(Pn,1);//�ճ����־;���:f=0�ɹ�,=1ʧ��
				goto END_Fn;//����ת��NEXT_Fn���¸�Fn���������ó���Ϊ�ɹ���־
			}

			//��չ13 ��1-24��������������й�����ʾֵ
			if(p8RMLib[0]==0xED)
			{
				//Fn101���й�����ʾֵ��������
				y=Class2Data_Fn(ClassNo,101);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				//Fn103���й�����ʾֵ��������
				y|=Class2Data_Fn(ClassNo,103);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				if(y==0)
				{
					goto NEXT_Fn;
				}
				if(UARTCtrl->RMCount==0)
				{//�����������=0ʱ�б�,��������ʱ��
					KeyUserPnList();//���г����ص㻧���߶���Pn�б�
				}
				i=MRR(ADDR_TYMDHMS+2,4);//ȡ��ǰ������ʱ
				YMDHM=YMDH_SubH(i,UARTCtrl->RMCount);//������ʱ��hʱ,���ؼ�hʱ���������ʱ
				YMDHM<<=8;//��=0
				i=CheckCurveCongeal(YMDHM,101,Pn);//������߶���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
				if(i!=0)
				{//�Ѷ����û�ҵ������
					UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
					UARTCtrl->RMCount++;//�������ݱ�ʶ����+1
					UARTCtrl->ReRMCount=0;//�ط�����
					return;
				}
			}
	#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
		//EventProject==0
			Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//�ն�DL645-2007����������
		#if (USER/100)==4//�û���ʶ:����
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//���ܷ��ʸ���
			if((i==1)&&(p8RMLib[0]==0xE3))
			{//�任�ն������ݱ�ʶ
				switch(UARTCtrl->RMCount)
				{
					default://�ն���ʱ��YYMMDDhhmm
						UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
						UARTCtrl->RMCount++;//�������ݱ�ʶ����+1
						UARTCtrl->ReRMCount=0;//�ط�����
						return;
					case 1://��1���������й�����ʾֵ���ܡ�����1��M��
						i=0x00010000;//��ǰ�����й�����ʾֵ���ܡ�����1��M��
						break;
				}
				MWR(i,((u32)p8txbuff)+4+10,4);
				//DI��0x33
				for(i=0;i<4;i++)
				{
					p8txbuff[4+10+i]+=0x33;
				}
				//cs
				p8txbuff[4+14]=0;
				for(i=0;i<14;i++)
				{
					p8txbuff[4+14]+=p8txbuff[4+i];
				}
			}
		#endif
			Fn=p8RMLib[0];
			if((Fn>=0xe4)&&(Fn<=0xe9))
			{
			#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�ϵ������
				//����ǰ������0�����ߵ�
				YMDHM=MRR(ADDR_TYMDHMS+1,5);//ȡ��ǰʱ��������ʱ��
				YMDHM&=0xffffffff00;
				i=UARTCtrl->RMCount*60;//ÿ60�ּ��
				YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
				i=Fn;
				i-=0xe4;
				i<<=16;
				i+=0x06000001;
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//�ն�DL645-2007�����͸�����������
			#else
				//����ǰ������0�����ߵ�
				YMDHM=MRR(ADDR_TYMDHMS+1,5);//ȡ��ǰʱ��������ʱ��
				YMDHM=YMDHM_Int15(YMDHM);//������ʱ�ֻ�Ϊ��15��,��00ʱ18�ֻ�Ϊ00ʱ15��
				i=UARTCtrl->RMCount*15;//ÿ15�ּ��
				YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
				i=Fn;
				i-=0xe4;
				i<<=16;
				i+=0x06000001;
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//�ն�DL645-2007�����͸�����������
			#endif
			}
	#else//#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
		//EventProject==1
			if(UARTCtrl->SubsequentFrames==0)//0=�޶�����֡,1-255=DL645����֡��֡��
			{//�Ǻ���֡
				Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//�ն�DL645-2007����������
			#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
				if(p8RMLib[0]==2)//F2 ���ܱ������ϱ��¼�
				{
					switch(UARTCtrl->RMCount)
					{
						case 0:
						case 1://�������ϱ�״̬��(��������ģ��ʱ)
							UARTCtrl->RMCount++;
							return;
						case (2+(96*2))://��λ�����ϱ�״̬��
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							MWR(YMDHM,((u32)p8txbuff)+4+1,6);
							p8txbuff[4+8]=0x14;
							p8txbuff[4+9]=24;
							MWR(0x04001503,((u32)p8txbuff)+4+10,4);
							MC(0,((u32)p8txbuff)+4+14,8);//����,�����ߴ���
							MR(((u32)p8txbuff)+4+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//��λ�����ϱ�״̬�ֵ�λ��־
							for(i=0;i<12;i++)
							{//ȡ����־λ
								x=p8txbuff[4+22+i];
								x=~x;
								p8txbuff[4+22+i]=x;
							}
							DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
							UARTCtrl->TxByte=4+36;//�����ֽ���
							break;
						case (2+(96*2)+1)://�������ϱ�״̬��
							break;
						default://2-97���������¼�
							//ȡDI	
							x=(u32)p8RMLib;
							x+=10+(5*UARTCtrl->RMCount);
							x=MRR(x,4);
							if(x==0)
							{//����
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-2)/2),1);
								UARTCtrl->RMCount++;
								if(UARTCtrl->RMCount&1)
								{
									UARTCtrl->RMCount++;
								}
								UARTCtrl->ReRMCount=0;
								return;
							}
							i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-2)/2),1);
							if(i==0)
							{//�����
								UARTCtrl->RMCount++;
								if(UARTCtrl->RMCount&1)
								{
									UARTCtrl->RMCount++;
								}
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(UARTCtrl->RMCount&1)
							{//��1-10���¼�
								if(i>10)
								{
									i=10;
								}
								i--;
								x+=i;
								YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
								i=Get_ADDR_UARTnTx(PORTn);
								Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
							}
							MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//��1֡��־
							break;
					}
				}
			#endif//#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
				if(p8RMLib[0]==3)//F3 ���ڳ������ܱ��¼�
				{
				#if ((USER/100)==5)//�Ϻ��û�
					switch(UARTCtrl->RMCount)
					{
					#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
						case ((60)*3)://�������ϱ�״̬��
							break;
						case ((60)*3+1)://��λ�����ϱ�״̬��
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							MWR(YMDHM,((u32)p8txbuff)+4+1,6);
							p8txbuff[4+8]=0x14;
							p8txbuff[4+9]=24;
							MWR(0x04001503,((u32)p8txbuff)+4+10,4);
							MC(0,((u32)p8txbuff)+4+14,8);//����,�����ߴ���
							MR(((u32)p8txbuff)+4+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//��λ�����ϱ�״̬�ֵ�λ��־

							for(i=0;i<12;i++)
							{
								p8txbuff[4+22+i]=0xff;
							}
							//ȡ����־λbit5��bit7
							x=p8txbuff[4+22];
							x=(~(x&0xa0));
							p8txbuff[4+22]=x;
							DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
							UARTCtrl->TxByte=4+36;//�����ֽ���
							break;
						case ((60)*3+3)://���������״̬��
							break;
					#endif
						default:
							i=UARTCtrl->RMCount;
							if((i%3)!=0)
							{//��1-10���¼�
								i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/3))+3,1);//��������(��ʼ��ֵ0xff)
								if(i==0xff)
								{
									i=0;
								}
								if(i==0)
								{//�����
									UARTCtrl->RMCount++;
									UARTCtrl->ReRMCount=0;
									return;
								}
								if(i>10)
								{
									i=10;
								}
								i--;
								//ȡDI	
								x=(u32)p8RMLib;
								x+=10+(5*UARTCtrl->RMCount);
								x=MRR(x,4);
								x+=i;
								YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
								i=Get_ADDR_UARTnTx(PORTn);
								Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn)+((UARTCtrl->RMCount%3)-1)*2,2);//��1֡��־
							}
							break;
					}
				#else//#if ((USER/100)==5)//�Ϻ��û�
				//!!!2015-7-29�����¼������޸�
					if(UARTCtrl->RMDI==0xffffffff)//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
					{//UARTCtrl->RMDI��Ч
						i=UARTCtrl->RMCount;
						if((i&1)!=0)
						{//��1-10���¼�
							i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//��������(��ʼ��ֵ0xff)
							if(i==0xff)
							{
								i=0;
							}
							if(i==0)
							{//�����
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(i>10)
							{
								i=10;
							}
							i--;
							//ȡDI	
							x=(u32)p8RMLib;
							x+=10+(5*UARTCtrl->RMCount);
							x=MRR(x,4);
							x+=i;
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
							MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//��1֡��־
						}
					}
					else
					{//UARTCtrl->RMDI��Ч
						i=UARTCtrl->RMCount;
						if((i&1)==0)
						{//������(���¼�������)
							x=UARTCtrl->RMDI;//�������ݱ�ʶ(���������¼�����),0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
							if(UARTCtrl->RMDICount==0)//�����¼������ѳ�DI����
							{
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//��1֡��־
							}
						}
						else
						{//��1-10���¼�,ָֻ�������ݱ�ʶ
							i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//��������(��ʼ��ֵ0xff)
							if(i==0xff)
							{
								i=0;
							}
							if(i==0)
							{//�����
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(i>10)
							{
								i=10;
							}
							i--;
							//ȡDI	
							x=UARTCtrl->RMDI;
							x&=0xffffff00;
							x|=0x01;
							x+=i;
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
							if(UARTCtrl->RMDICount==0)//�����¼������ѳ�DI����
							{
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//��1֡��־
							}
						}
					}
				#endif
				}
			#if (USER/100)==4//�û���ʶ:����
				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//���ܷ��ʸ���
				if((i==1)&&(p8RMLib[0]==0xE3))
				{//�任�ն������ݱ�ʶ
					switch(UARTCtrl->RMCount)
					{
						default://�ն���ʱ��YYMMDDhhmm
							UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
							UARTCtrl->RMCount++;//�������ݱ�ʶ����+1
							UARTCtrl->ReRMCount=0;//�ط�����
							return;
						case 1://��1���������й�����ʾֵ���ܡ�����1��M��
							i=0x00010000;//��ǰ�����й�����ʾֵ���ܡ�����1��M��
							break;
					}
					MWR(i,((u32)p8txbuff)+4+10,4);
					//��0x33
					for(i=0;i<4;i++)
					{
						p8txbuff[4+10+i]+=0x33;
					}
					//cs
					p8txbuff[4+14]=0;
					for(i=0;i<14;i++)
					{
						p8txbuff[4+14]+=p8txbuff[4+i];
					}
				}
			#endif
				Fn=p8RMLib[0];
				if((Fn>=0xe4)&&(Fn<=0xe9))
				{
				#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�ϵ������
					//����ǰ������0�����ߵ�
					YMDHM=MRR(ADDR_TYMDHMS+1,5);//ȡ��ǰʱ��������ʱ��
					YMDHM&=0xffffffff00;
					i=UARTCtrl->RMCount*60;//ÿ60�ּ��
					YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
					i=Fn;
					i-=0xe4;
					i<<=16;
					i+=0x06000001;
					PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//�ն�DL645-2007�����͸�����������
				#else
					//����ǰ������0�����ߵ�
					YMDHM=MRR(ADDR_TYMDHMS+1,5);//ȡ��ǰʱ��������ʱ��
					YMDHM=YMDHM_Int15(YMDHM);//������ʱ�ֻ�Ϊ��15��,��00ʱ18�ֻ�Ϊ00ʱ15��
					i=UARTCtrl->RMCount*15;//ÿ15�ּ��
					YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
					i=Fn;
					i-=0xe4;
					i<<=16;
					i+=0x06000001;
					PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//�ն�DL645-2007�����͸�����������
				#endif
				}
			}
			else
			{//�Ǻ���֡
				p8txbuff[4+8]=0x12;
				p8txbuff[4+9]=5;
				p8txbuff[4+14]=UARTCtrl->SubsequentFrames+0x33;
				p8txbuff[4+15]=0;//cs
				for(i=0;i<15;i++)
				{
					p8txbuff[4+15]+=p8txbuff[4+i];
				}
				p8txbuff[4+16]=0x16;
				UARTCtrl->TxByte=4+17;//�����ֽ���
			}
	#endif//#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
	
			//ͨ������
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ĭ��
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			//��ʾ�����ַ
			switch(UARTCtrl->RMTaskC)
			{
				case 0://�ϵ�
					CopyString((u8 *)"485 ����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 1://�ճ�
					if(UARTCtrl->DateReRMCount<=1)//ÿ������(������������)��ִ���ܴ���;0=��(�ϵ���ո���),1-255=�ܴ�������
					{
						CopyString((u8 *)"485 �ճ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					}
					else
					{
						CopyString((u8 *)"485 ����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					}
					break;
				case 2://�ܳ�
					CopyString((u8 *)"485 ʱ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					//CopyString((u8 *)"485 �ܳ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 3://����
					CopyString((u8 *)"485 ����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case (MaxRMTask-2)://���ڳ������ܱ��¼�
					switch(UARTCtrl->RMEventTaskC)//��ǰ�������ܱ��¼�����:0=��,1-3=1-3��
					{
						case 1:
							CopyString((u8 *)"485  1��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 2:
							CopyString((u8 *)"485  2��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
						case 3:
							CopyString((u8 *)"485  3��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
							break;
					}
					break;
				default://ѭ��
					CopyString((u8 *)"485 ѭ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
			}
		#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			if(UARTCtrl->RMMeterEvent&1)//���ڳ������ܱ��¼���־;b0=�����ϱ��¼�,b1=
			{
				CopyString((u8 *)"485 �ϱ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			}
		#endif
			MWR(PORT485,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8txbuff[10-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			}
			break;
		case 5://5=���ڽ���
			i=DL645_Rx(PORTn);//DL645֡����;����0û���,1���,2��ʱ
			switch(i)
			{
				case 0://0û���
					return;
				case 1://1���
					break;
				default://2��ʱ
					UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
					return;
			}	
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			p16timer[0]=30/10;//��֡����������ʱ20MS��
			
	#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			if(UARTCtrl->SubsequentFrames!=0)//0=�޶�����֡,1-255=DL645����֡��֡��
			{//�Ǻ���֡
				if((p8RMLib[0]<2)||(p8RMLib[0]>3))
				{//���Ƕ������ϱ��¼������ڶ����ܱ��¼�
					//��0x33
					for(i=0;i<p8rxbuff[9];i++)
					{
						p8rxbuff[10+i]-=0x33;
					}
					i=MRR(((u32)p8rxbuff)+10,4);//���ص����ݱ�ʶ
					if(i==0x04001501)
					{//���յ������ϱ�֡
						if((UARTCtrl->RMCount+1)>=p8RMLib[1])//�����������
						{//ֻ��1��,���������ϱ����¼�����ʱÿFn���ж����������ʱ��Ƕ��.
							WriteAutoReportWord(PORTn,((u32)p8rxbuff));//д���ܱ������ϱ�״̬��;���:�˿ں�(1-31),ͨ�ŵ�ַ,�Զ��ϱ�״̬�ֵ�ַ
						}
						UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
						UARTCtrl->Task=0;//����1��
						UARTCtrl->ReRMCount=0;//���������ط�����
						return;
					}
					if(p8rxbuff[8]&0x20)
					{//�к���֡
						UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
					}
					else
					{//�޺���֡
						UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
					}
					UARTCtrl->Task=0;//����1��
					UARTCtrl->ReRMCount=0;//���������ط�����
					return;
				}
			}
			if(p8rxbuff[8]&0x20)
			{//�к���֡
			#if EnMeterAutoAutoReport==1//������ܱ������ϱ��¼�:0=������,1=����
				UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
			#else
				if((p8RMLib[0]==2)||(p8RMLib[0]==3))
				{//�ز�ֻ���������¼������ڶ����ܱ��¼�ʱ���������֡
					UARTCtrl->SubsequentFrames++;//0=�޶�����֡,1-255=DL645����֡��֡��
				}
			#endif
			}
			else
			{//�޺���֡
				UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
			}
	#endif
			//�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33
			i=DL645_2007_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��

	#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			if(i==3)
			{//3���ݱ�ʶ��
				//����ʱû��0x33
				for(x=0;x<p8rxbuff[9];x++)
				{
					p8rxbuff[10+x]-=0x33;
				}
				x=MRR(((u32)p8rxbuff)+10,4);
				if((p8RMLib[0]==2)&&((UARTCtrl->RMCount==(2+(96*2)))||(x==0x04001501)))
				{//F2 ���ܱ������ϱ��¼��ĸ�λ�����ϱ�״̬�ֻش�����֡
					i=0;//��ȷ
				}
				if((p8RMLib[0]==3)&&(x==0x04001501))
				{//F3 ���ڶ����ܱ��¼��������ϱ�״̬�ֺ���֡
					i=0;//��ȷ
				}
			#if ((USER/100)==5)//�Ϻ��û�
			#if EnMeterAutoAutoReport==0//������ܱ������ϱ��¼�:0=������,1=����
				if((p8RMLib[0]==3)&&(UARTCtrl->RMCount==((60)*3)+1))
				{//F3 ���ڶ����ܱ��¼��ĸ�λ�����ϱ�״̬�ֻش�
					i=0;//��ȷ
				}
			#endif
			#endif
			}
	#endif
			if(i!=0)
			{
				if(i==2)
				{//��վ�쳣�ش�
					UARTCtrl->Task=1;//���1֡����
					UARTCtrl->RMCount+=1;//�����������+1
					UARTCtrl->ReRMCount=0;//���������ط�����
					UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ����������
					break;
				}
				//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
				//��������
				p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=���ڽ���
				break;
			}
			//���س���
			if(p8rxbuff[9]==4)
			{//���Ժ����������¼���¼������ʱ,ֻ����4�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
				UARTCtrl->Task=1;//���1֡����
				UARTCtrl->RMCount+=1;//�����������+1
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ����������
				break;
			}
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			Fn=p8RMLib[0];
			if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
			{//���Ƕ��������
				p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
			}
			else
			{//�Ƕ��������
				p8RMLib+=10+4;
			}
	#if EventProject==0//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
			if(p8RMLib[0]!=0x0)
	#else
			if((p8RMLib[0]!=0x0)&&(p8rxbuff[8]!=0x92)&&(p8rxbuff[8]!=0xB2))
			//Ӧ���س��Ȳ�Ϊ0 ͬʱ ���Ǻ���֡
	#endif
			{
			//!!!2015-7-29�����¼������޸�
				if((Fn==3)&&(UARTCtrl->RMDI!=0xffffffff))
				{
					goto SKIP;//�����¼������ǿ�������ʱ�޷��س��ȿɱȽ�
				}
				if(p8rxbuff[9]!=(p8RMLib[0]+4))
				{
				#if EventProject==1//�¼���¼�ɼ�����:0=ԭ����̨���׼,1=�������ܵ��ܱ��ɼ��ն��¼���¼�ɼ�����
					if(p8RMLib[0]==2)
					{//F2 ���ܱ������ϱ��¼�
						if(p8rxbuff[9]>=4)
						{
							goto SKIP;//���Ƚϳ���
						}
					}
				#endif
					if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
					{//������������0xaa
						UARTCtrl->Task=0;//����1��
						UARTCtrl->RMCount+=1;//�����������+1
						UARTCtrl->ReRMCount=0;//���������ط�����
						UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ���������
						break;
					}
				}
			}
SKIP:
			UARTCtrl->Task=1;//���1֡����
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007���������������Fn���ݻ���
			
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			Fn=p8RMLib[0];
			//�����1-6�ฺ������
			if((Fn>=0xe4)&&(Fn<=0xea))
			{
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				UARTCtrl->RMCountErr=255;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����=���ֵ����δ洢����
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			//��չ13 ��1-24��������������й�����ʾֵ
			if(Fn==0xED)
			{
				YMDHM=MRR(((u32)p8FnData)+6+1,4);//ȡ����صĶ���ʱ��������ʱ
				YMDHM<<=8;
				AddCurveCongeal(YMDHM,101,Pn,((u32)p8FnData)+6+1+4,4);//�����߶��������
				AddCurveCongeal(YMDHM,103,Pn,((u32)p8FnData)+6+1+4+4,4);//�����߶��������
			#if (USER/100)==11//����ϵ
				//����̨�����������㶳������
				YMDHM=YMDHM_SubM(YMDHM,30);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
				AddCurveCongeal(YMDHM,101,Pn,((u32)p8FnData)+6+1+4,4);//�����߶��������
			#endif
			}
#if (USER/100)==11//����ϵ
			if(Fn==0xFB)
			{
				YMDHM=MRR(((u32)p8FnData)+6+1,4);//ȡ���Ķ���ʱ��������ʱ
				YMDHM<<=8;
				AddCurveCongeal(YMDHM,89,Pn,((u32)p8FnData)+6+1+4,2);//�����߶��������
				AddCurveCongeal(YMDHM+0x15,89,Pn,((u32)p8FnData)+6+1+4+2,2);//�����߶��������
				AddCurveCongeal(YMDHM+0x30,89,Pn,((u32)p8FnData)+6+1+4+2+2,2);//�����߶��������
				AddCurveCongeal(YMDHM+0x45,89,Pn,((u32)p8FnData)+6+1+4+2+2+2,2);//�����߶��������
			}
			if(Fn==0xFA)
			{
				if(UARTCtrl->RMCount==2)
				{
					i=MRR(ADDR_TYMDHMS+4,2);//ȡ��ǰʱ������
					i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
					AddMonthCongeal(i,35,Pn,((u32)p8FnData)+6+1,93);//���¶��������
				}
			}
#endif
	#if ((USER/100)==8)||((USER/100)==7)//�û���ʾ��ͨ�á��㽭
			if((Fn==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))
			{
		 		MR(ADDR_DATABUFF,((u32)p8FnData)+1+6,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				p8=(u8 *)(ADDR_DATABUFF);
				if(p8[0]!=0xee)
				{
					p8[4]&=0x1f;//ȥ�������ֽ��ϵ�����
					p8[10]&=0x1f;//ȥ�������ֽ��ϵ�����
					i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//������ʱ����Ϸ����,����0�Ϸ�,1�Ƿ�
					if(i==0)
					{//���ܱ�ʱ�ӺϷ�
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								if((i&0x02)==0)
								{
									i|=0x02;
								}
								break;
							case RS485_2PORT://RS485-2
								if((i&0x04)==0)
								{
									i|=0x04;
								}
								break;
						}
						MWR(i,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
						if((i>>31)!=0x0)
						{//��
							i=~i;
							i++;
						}
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								x=0;
								break;
							case RS485_2PORT://RS485-2
								x=1;
								break;
						}
						x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
						if(x<4)
						{
							MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
							MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//���ܱ��ʱǰ�����
							MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//���ܱ��ʱ������
							MWR(2,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//���ܱ��ʱ��Ľ��
						}
						if(x==4)
						{
							x=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//���ܱ��ʱǰ�����
							if(x>MRR(ADDR_AFN04F298_Ext+7,1))
							{
								MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
								MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//���ܱ��ʱ������
								if(i<=MRR(ADDR_AFN04F298_Ext+7,1))
								{//�ɹ�
									MWR(1,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//���ܱ��ʱ��Ľ��
								}
								else
								{//ʧ��
									MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//���ܱ��ʱ��Ľ��
								}
								//ERC51
								p8=(u8 *)ADDR_DATABUFF;
								//���ܱ��ʱǰ�����
								MWR(x,ADDR_DATABUFF+9,2);
								//���ܱ��ʱ������
								MWR(i,ADDR_DATABUFF+11,2);
								p8[13]=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);
								ERC51_Event(Pn+(1<<15),ADDR_DATABUFF);//Уʱ��¼�¼�;���:Pn�������(��0��ʼ)
							#if (USER/100)==8//�û���ʶ��ͨ��
								p8=(u8 *)ADDR_TYMDHMS;
								MR((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1),3);
								x=DataComp((u32)ADDR_DATABUFF,(u32)ADDR_TYMDHMS+3,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
								if(x==0)
								{
									x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);
									if(x==0xffffff)
									{
										DMWR(i,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);//�Ĵ���ֱ��д���洢��(<=8�ֽ�)(�޲���)
									}
								}
								//ʱ�䳬�� 5min ����������
								if(i>300)
								{
									x=MRR(ADDR_AFN0DF321_Ext_Source,2);
									x++;
									MWR(x,ADDR_AFN0DF321_Ext_Source,2);
								}
								//����ʱ����ֵδ����5min����������
								if((i>MRR(ADDR_AFN04F298_Ext+7,1))&&(i<300))
								{
									x=MRR(ADDR_AFN0DF321_Ext_Source+2,2);
									x++;
									MWR(x,ADDR_AFN0DF321_Ext_Source+2,2);
								}
							#endif
							}
							x=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),1);//���ܱ��ʱ����
							if(x<31)
							{
								x++;
							}
							MWR(x,ADDR_DATABUFF,1);
							MR(ADDR_DATABUFF+1,ADDR_AFN0CF250_Ext1+10*(Pn-1),10);
							if(x>=31)
							{
								x=30;
							}
							MR(ADDR_DATABUFF+11,ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1)+1,10*x);
							MR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),ADDR_DATABUFF,1+10*x+10);
						}
					}
				}
			}
	#endif
			UARTCtrl->RMCount+=1;//�����������+1
			UARTCtrl->ReRMCount=0;//���������ط�����=0
			break;
	}
}


void Terminal_ReadMeter_DL645_2007_Fn(u32 PORTn,u8 *p8RMLib)//����1��Fn>=129������д��FnDataBuff;Task=0=����(��Fn�������)
{
	u32 i;
	u32 x;
	u32 Pn;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	u64 YMDHM;
	
//	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			break;
		case 1://1=���1֡����
			UARTCtrl->Task=3;
			break;
		case 2://2=���ճ�ʱ(�����)
			if(UARTCtrl->ReRMCount<1)
			{
				UARTCtrl->ReRMCount+=1;//���������ط�����+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//�����������+1
				UARTCtrl->ReRMCount=0;//���������ط�����
			}
			UARTCtrl->Task=3;
			break;
		case 3://3=��������
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//����ŵĵ��ܱ�/��������װ��ɾ��
			END_Fn:
				UARTCtrl->Task=0;//����
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				break;
			}
			if((p8RMLib[3]&0x2)==0x0)
			{//��Fn����RS485����
				goto END_Fn;
			}
//			Fn=p8RMLib[0];

			if(p8RMLib[1]==0x0)
			{//��Fn����������=0
				goto END_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//�����������=0,��Fn���ݻ���
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//�����������>=Fn�跢������;��Fn�������
				if(p8RMLib[0]==28)
				{//F28 �������״̬�ּ����λ��־
					ReadMeter_SaveFnData(PORTn,p8RMLib[0],Pn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				}
				goto END_Fn;
			}
			
			//������֡
			if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
			{//���Ƕ��������
				Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//�ն�DL645-2007����������
			}
			else
			{//���������
				i=MRR(((u32)p8RMLib)+10,4);
				YMDHM=MRR((u32)&UARTCtrl->RMCommandYMDHM[0],5);//���ߵ�ʱ��������ʱ��
			#if (((Project/100)==2)&&((USER/100)==5))//�Ϻ�ϵ������
				YMDHM&=0xffffffff00;
			#endif
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//�ն�DL645-2007�����͸�����������
			}
			
			//ͨ������
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ĭ��2400
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					Pn=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					Pn=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					Pn=3;//RS485PORT=3;
					break;
				default:
					Pn=0;//RS485PORT=0;
					break;
			}
			//��ʾ�����ַ
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			CopyString((u8 *)"485 ʵ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			MWR(Pn,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8rxbuff[10-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
				UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			}
			break;
		case 5://5=���ڽ���
			i=DL645_Rx(PORTn);//DL645֡����;����0û���,1���,2��ʱ
			switch(i)
			{
				case 0://0û���
					return;
				case 1://1���
					break;
				default://2��ʱ
					UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
					return;
			}	
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			p16timer[0]=30/10;//��֡����������ʱ20MS��

			//�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33
			i=DL645_2007_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if(i!=0)
			{
				if(i==2)
				{//��վ�쳣�ش�
					UARTCtrl->Task=1;//���1֡����
					UARTCtrl->RMCount+=1;//�����������+1
					UARTCtrl->ReRMCount=0;//���������ط�����
					break;
				}
				//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
				//��������
				p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=���ڽ���
				break;
			}
	
			//���س���
			p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
			if(p8RMLib[0]!=0x0)
			{
				if(p8rxbuff[9]!=(p8RMLib[0]+4))
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
					{//������������0xaa
						if(p8rxbuff[9]==4)
						{//���Ժ����������¼���¼������ʱ,ֻ����4�ֽ����ݱ�ʶ,ͬ��վ�쳣�ش�
							UARTCtrl->Task=1;//���1֡����
							UARTCtrl->RMCount+=1;//�����������+1
							UARTCtrl->ReRMCount=0;//���������ط�����
						}
						else
						{
							UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
						}
						break;
					}
				}
			}
			p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
			UARTCtrl->Task=1;//���1֡����
			
			ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007���������������Fn���ݻ���
			UARTCtrl->RMCount+=1;//�����������+1
			UARTCtrl->ReRMCount=0;//���������ط�����=0
			break;
	}
}











