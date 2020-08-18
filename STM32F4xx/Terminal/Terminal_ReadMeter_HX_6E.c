
//�ն˳���RS485��_DL645_2007
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "Terminal_ReadMeter_HX_6E.h"

#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_HX.h"  //	�˺�������ļ���Ҫ�޸�  ��Ӧ645.c
#include "Terminal_ReadMeter_HX_6E_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_HX_6E_Lib.h"
#include "Terminal_ReadMeter_Event.h"
#include "../terminal/airthmetic.h"


#if ((Project/100)==3)&&((USER/100)==12)//�Ĵ�ר��
extern const u32 HX_6E_RMLib[];
u32 Get_RM_HX_6E_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];


void Terminal_ReadMeter_HX_6E(u32 PORTn)//�ն˳���RS485��_HX
{
	u32 i,x,Fn,Pn;
	u8 * p8RMLib;
    UARTCtrl_TypeDef * UARTCtrl;
#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	u32 z;
#endif
	u8 * p8rxbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
//	u32 ClassNo;
//	u32 y;
//	u8 temp[8];
#if Extern_ReadMeter_Fn_YesNo!=0//�ⲿ����:0=I2,1=E1,����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)
	u32 *p32;
#endif

//#if ((USER/100) == 12)
//	u32 *pdata;
//#endif

	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);



	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
				Fn=MRR(HX_6E_RMLib[UARTCtrl->FnCount],1);
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
						UARTCtrl->RMError|=2;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
						break;      							
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
			if(UARTCtrl->FnCount>=Get_RM_HX_6E_MaxFn())
			{//Fn����>=���Fn����,���������
				if(UARTCtrl->RMError==0)
				{//����ɹ�
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
					ReadMeterNote_OK(PORTn,Pn,0);//���г���������Ϣ.�ɹ�
					DateRMFlags(Pn,0);//�ճ����־;���:f=0�ɹ�,=1ʧ��
				}
				else
				{//����ʧ��
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//�Ĵ泭��ɹ�ʧ�ܱ�־:0=��,1=������1�����ݱ�ʶ����ʧ��,2=�ɹ�
					ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
					DateRMFlags(Pn,1);//�ճ����־;���:f=0�ɹ�,=1ʧ��
				}
	END_Fn:
				UARTCtrl->Task=0;//0=����
				UARTCtrl->FnCount=0;//Fn����=0
				UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				UARTCtrl->RMError=0;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				break;
			}
			if(HX_6E_RMLib[UARTCtrl->FnCount]==0x0)
			{//��Fn�޳���
	NEXT_Fn:
				UARTCtrl->FnCount+=1;//Fn����+1		  
				UARTCtrl->Task=1;//���1֡����
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				UARTCtrl->RMError&=0xfe;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				break;
			}
			p8RMLib=(u8 *)HX_6E_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//��Fn����RS485����
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];
//			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���
//		#if Extern_ReadMeter_Fn_YesNo==0//�ⲿ����:0=I2,1=E1,����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)	
//				y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,30);//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������
//		#else
//			y=0;
//			if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
//			{
//				p32=(u32 *)(ADDR_E1_ENTRY);
//				y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)UARTCtrl->RMCount,(u32)30);//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������
//			}
//		#endif
//			if(y==0x0)		
//			{//�������
//				if(p8RMLib[1]==0x0)
//				{//��Fn����������=0
//					goto NEXT_Fn;
//				}
//				if(UARTCtrl->RMCount==0)//�������ݱ�ʶ����
//				{//�������ݱ�ʶ����=0,��Fn���ݻ���
//					UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
//					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
//				}
//				UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����,��Ϊʧ����ȫû��ʱ����������
//				UARTCtrl->RMCount++;//�������ݱ�ʶ����
//				UARTCtrl->ReRMCount=0;//�ط�����
//				if(UARTCtrl->RMCount>=p8RMLib[1])
//				{//�����������>=Fn�跢������;��Fn�������
//				#if(RMM_RS485_FailData_0xEE==0)//RS485����ʧ��������0xee;0=��,1=��
//					if(p8RMLib[1]>UARTCtrl->RMCountErr)
//					{//����ȫ����ʶ����
//						goto OK_Fn;
//					}
//					goto NEXT_Fn;
//				#else
//					goto OK_Fn;
//				#endif
//				}
//				return;
//			}
			if(UARTCtrl->RMTaskC==0)
			{//�����û������
				UARTCtrl->RMTaskC=(MaxRMTask-1);//(MaxRMTask-1)=ѭ������;
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
					//F40 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M?2?
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
//			OK_Fn:   //�����￪ʼҪ������ϸ�ļ��
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
//				ReadMeterNote_Fail(PORTn,Pn);//���г���������Ϣ.ʧ��
//				DateRMFlags(Pn,1);//�ճ����־;���:f=0�ɹ�,=1ʧ��
				goto END_Fn;//����ת��NEXT_Fn���¸�Fn���������ó���Ϊ�ɹ���־
			}
			Terminal_ReadMeter_HX_6E_TxData(PORTn,p8RMLib);//�ն�HX����������	
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
			UARTCtrl->BpsCtrl &=0xfff7; 
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
			MWR(PORT485,ADDR_DATABUFF+3,1);
			MR(((u32)p8rxbuff),ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
			for(i=0;i<6;i++)
			{
				x=p8rxbuff[5-i];
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
			i=HX_Rx(PORTn);//DL645֡����;����0û���,1���,2��ʱ
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
			i=HX_6E_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if(i!=0)
			{//��վ�쳣�ش�
				UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
				break;
			}
			UARTCtrl->Task=1;//���1֡����
			p8RMLib=(u8 *)HX_6E_RMLib[UARTCtrl->FnCount];
			ReadMeter_HX_6E_FnData(PORTn,p8RMLib);//���������������Fn���ݻ���
//			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
//			Fn=p8RMLib[0];
			UARTCtrl->RMCount+=1;//�����������+1
			UARTCtrl->ReRMCount=0;//���������ط�����=0
			break;
	}
}


void Terminal_ReadMeter_HX_6E_Fn(u32 PORTn,u8 *p8RMLib)//����1��Fn>=129������д��FnDataBuff;Task=0=����(��Fn�������)
{
	u32 i;
	u32 x;
	u32 Pn;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
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
				goto END_Fn;
			}
			Terminal_ReadMeter_HX_6E_TxData(PORTn,p8RMLib);//�ն˳���������		
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
			UARTCtrl->BpsCtrl &=0xfff7; 
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
			p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
			CopyString((u8 *)"485 ʵ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			
			MWR(Pn,ADDR_DATABUFF+3,1);
			MR(((u32)p8rxbuff),ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
			for(i=0;i<6;i++)
			{
				x=p8rxbuff[5-i];
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
			i=HX_Rx(PORTn);//DL645֡����;����0û���,1���,2��ʱ
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
			i=HX_6E_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if(i!=0)
			{//��վ�쳣�ش�
				UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
				break;
			}
	
			UARTCtrl->Task=1;//���1֡����
			ReadMeter_HX_6E_FnData(PORTn,p8RMLib);//DL645_2007���������������Fn���ݻ���
			UARTCtrl->RMCount+=1;//�����������+1
			UARTCtrl->ReRMCount=0;//���������ط�����=0
			break;
	}
}
#endif










