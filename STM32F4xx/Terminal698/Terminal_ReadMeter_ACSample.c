

//�ڲ���������
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Terminal/Terminal_ReadMeter_ACSample.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"
#include "Terminal_ReadMeter_SaveFnData.h"




void GetACSample(u32 DI)//�õ��ڲ���������,���ͨ�����ݱ�ʶ,����645֡��ʽ��ͨ�ſڻ���ķ�����
{
	u32 i;
	u32 Task;
	u32 *p32;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(ACSAMPLEPORT);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(ACSAMPLEPORT);
	Task=UARTCtrl->Task;//ԭ����Ĵ�
	UARTCtrl->Task=1;//1=�������
	UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	MWR(DI,((u32)p8rxbuff)+10,4);
	//��0x33
	for(i=0;i<4;i++)
	{
		p8rxbuff[10+i]+=0x33;
	}
	//ͨ�ŵ�ַ
	MC(0xaa,((u32)p8rxbuff)+1,6);
	p8rxbuff[8]=0x11;//������
	p8rxbuff[9]=0x4;
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
  if(FileDownload->File&0x4)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
  {
		p32=(u32 *)(ADDR_Meter_ENTRY);
    (*(void(*)())p32[130])((u32)ACSAMPLEPORT);//����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
		//��33H
		for(i=0;i<p8txbuff[9];i++)
		{
			p8txbuff[10+i]-=0x33;
		}
	}
	else
	{
		MC(0,Get_ADDR_UARTnTx(ACSAMPLEPORT),Get_LEN_UARTnTx(ACSAMPLEPORT));
	}
#else//#if ((Project/100)<5)
	MC(0,Get_ADDR_UARTnTx(ACSAMPLEPORT),Get_LEN_UARTnTx(ACSAMPLEPORT));
#endif//#if ((Project/100)<5)
	UARTCtrl->Task=Task;//ԭ����ָ�
}




/*
extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
void Terminal_ReadMeter_ACSample(u32 PORTn)//�ն˳���,��������
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u8 *p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	u32 *p32;

	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
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
#if RMM_RS485_PassageZero==1//RS485ͨ��������0��;0=����ͣ,1=��ͣ
			i=MRR(ADDR_TYMDHMS,3);
			if((i>0x235955)||(i<0x000005))
			{//�ڲ�������ÿ��0�㸽����ͣ����
				return;
			}
#endif
			UARTCtrl->Task=3;//��������
			break;
		case 1://1=���1֡����
			UARTCtrl->Task=0;//����1��
			break;
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			UARTCtrl->RMCount+=1;//�����������+1
			Fn=MRR(DL645_2007_RMLib[UARTCtrl->FnCount],1);
			switch(Fn)
			{
				case 0xE1://��չ1 ERC8���ܱ���������¼�������
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
					break;
			}
			break;
		case 3://3=��������
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*27)+2,2);
			if(Pn==0x0)
			{//����ŵĵ��ܱ�/��������װ��ɾ��
				UARTCtrl->Task=0;//0=����
				UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
				break;
			}
			if(UARTCtrl->FnCount>=Get_RM_DL645_2007_MaxFn())
			{//Fn����>=���Fn����,�����������
				UARTCtrl->Task=0;//0=����
				UARTCtrl->FnCount=0;//Fn����=0
				UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->RMError=0;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־
				break;
			}
			if(DL645_2007_RMLib[UARTCtrl->FnCount]==0x0)
			{//��Fn�޳���
NEXT_Fn:
				UARTCtrl->Task=1;//���1֡����
				UARTCtrl->FnCount+=1;//Fn����+1
				UARTCtrl->RMCount=0;//�����������=0
				break;	
			}
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x1)==0x0)
			{//��Fn�����ڽ�����������
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];
			if(p8RMLib[1]==0x0)
			{//��Fn����������=0
			#if AFN0CF33MaxPn==0
				goto NEXT_Fn;
			#else
				switch(Fn)
				{
//F33 ��ǰ������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//F34 ��ǰ������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//F35 ����������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//F36 ���·�����/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//F37 ���£���һ�����գ�������/�޹�������޹�1������ʾֵ��һ/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//F38 ���£���һ�����գ�������/�޹�������޹�2������ʾֵ����/�������޹�����ʾֵ���ܡ�����1��M��1��M��12��
//F39 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
//F40 ���£���һ�����գ�������/�޹��������������ʱ�䣨�ܡ�����1��M��1��M��12��
					case 33:
					case 34:
					case 35:
					case 36:
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
			#endif
			}
			
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//�����������>=Fn�跢������;��Fn�������
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
				goto NEXT_Fn;
			}
			
			if(UARTCtrl->RMCount==0x0)
			{//�����������=0,��Fn���ݻ���
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//�ն�DL645-2007������������
			MR((u32)p8rxbuff,((u32)p8txbuff)+4,16);
			UARTCtrl->Task=1;//1=�������
			UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
		  if(FileDownload->File&0x04)//�ļ���־(��λ��ʾOK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
		  {
				p32=(u32 *)(ADDR_Meter_ENTRY);
	      (*(void(*)())p32[130])((u32)PORTn);//����void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645���շ���
			}
			else
			{
				UARTCtrl->Task=1;//���1֡����
				UARTCtrl->RMCount+=1;//�����������+1
				break;
			}
			MR((u32)p8rxbuff,((u32)p8txbuff),p8txbuff[9]+12);
			//������
			if((p8rxbuff[8]&0x40)!=0x0)
			{//��վ�쳣�ش�
				UARTCtrl->Task=1;//���1֡����
				UARTCtrl->RMCount+=1;//�����������+1
				break;
			}
			//��0x33
			for(i=0;i<p8rxbuff[9];i++)
			{
				p8rxbuff[10+i]-=0x33;
			}
			ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007����������������Fn���ݻ���
			UARTCtrl->Task=1;//���1֡����
			UARTCtrl->RMCount+=1;//�����������+1
			break;
	}

}
*/





