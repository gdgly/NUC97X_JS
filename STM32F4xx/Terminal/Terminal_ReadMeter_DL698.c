//�ն˳���RS485��_�Ϻ���Լ
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"

#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Terminal/DataConversion_SH698.h"

#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_ReadMeter_DL698_FnData.h"
#include "Terminal_ReadMeter_DL698_Lib.h"
#include "../MS/MS.h"


#if (USER/100)==5//�����Ϻ�

#define		CTRL_OFFSET			3		//�������ڱ������ƫ����
#define		AF_OFFSET			4		//��ַ��ʶ�ڱ������ƫ��
#define		SA_OFFSET			5		//��������ַ�ڱ������ƫ��
#define		SA_LEN(af)		(((af) & 0x0f) + 1)		//SA�ĳ���
#define		APDU_OFFSET(data)	(8 + SA_LEN(data[AF_OFFSET]))					//APDU���ֽ��ڱ������ƫ��


extern const u32 DL698_RMLib[];


u32 DL698_Rx(u32 PORTn)//698��Լ֡����;����0û���,1���,2��ʱ
{

	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	//u32 sa_len;
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
	p0=p16fifo[0];//�жϽ���ָ��
	p1=p16fifo[1];//��ǰ�Ѷ�ָ��
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//�жϽ���ָ��
		p16fifo[1]=0;//��ǰ�Ѷ�ָ��
		p16fifo[2]=0;//��Լ���ָ��
		p16fifo[3]=0;
		p16fifo[4]=0;
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//�����ݽ������ó�ʱ��ʱ
		p16timer[0]=500/10;//�ֽڼ䳬ʱ��ʱ
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-10))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(i!=0x68)
			{
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte>5)
			{
				if(RxByte==((p8rxbuff[4]&0X0F)+1+8))//֡ͷ�������ˣ�����У��HCS
				{
					Pn=Get_16bitfcs_code(p8rxbuff+1, RxByte-3)^0xffff;
					if(Pn!=(p8rxbuff[RxByte-2]+(p8rxbuff[RxByte-1]<<8)))
					{//HCSУ�����
						RxByte=0;
						continue;					
					}
				}
				if(RxByte==(p8rxbuff[1]+((p8rxbuff[2]&0x3f)<<8)+2))//��֡�������ˣ�����У��FCS
				{
					if(i==0x16)//����λ������0X16
					{
						Pn=Get_16bitfcs_code(p8rxbuff+1,RxByte-4)^0xffff;
						if(Pn==(p8rxbuff[RxByte-3]+(p8rxbuff[RxByte-2]<<8)))
						{//��У����ȷ
							p16fifo[1]=p1;//��ǰ�Ѷ�ָ��
							p16fifo[2]=p1;//���ָ��==��ָ��,���ʡʱ
							UARTCtrl->RxByte=RxByte;
							return 1;
						}
						else
						{//��У�����
							RxByte=0;
							continue;
						}	
					}
					else
					{
						RxByte=0;
						continue;
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//��ʱ
		return 2;//2=���ճ�ʱ
	}
	return 0;
}


u32 DL698_RxData(u32 PORTn)//698��Լ֡��������,�ȽϷ��յ�ַ��������;����0��ȷ,1��ַ��,2�������
{
	u32 i;
	u32 sa_len;
	u32 apdu_offset;
	u32 split_frame;//��֡��ʽ
	u32 fragment_len;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 block_no;//��֡��ǰ֡���
	// u32 last_block_no=0xffff;//��֡��һ֡֡���

	u8* p_save_frame;//�����֡���һ֮֡ǰ��֡��apdu
	// u32 save_fame_len=0;//�����֡��֡����
	ms_Type* ms;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	ms=Get_ms();
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	//ͨ�ŵ�ַ�Ƚ�
	//CA���ñȽ�
	sa_len=SA_LEN(p8rxbuff[4]);//,(p8rxbuff[4]&0x0f)+1;
	for(i=0;i<sa_len;i++)
	{
		if(p8rxbuff[5+i]!=p8txbuff[5+i])
		{
			break;
		}
	}
	if(i!=sa_len)
	{
		return 1;//����0��ȷ,1��ַ��,2�������
	}

	apdu_offset=APDU_OFFSET(p8rxbuff);//apdu��֡������ֽ�
	if((p8rxbuff[3]&0x20)!=0)//�з�֡
	{
		split_frame=MRR(((u32)p8rxbuff)+apdu_offset,2);//��֡��ʽ
		block_no=split_frame&0xfff;//��ǰ֡���
		fragment_len=UARTCtrl->RxByte-13-SA_LEN(p8rxbuff[4]);//APDU��ȥ��֡��ʽ֮��ĳ���
		if(UARTCtrl->Last_Block_No==block_no)//������֡ͬһ��ţ��Ǵ����
		{
			if(UARTCtrl->Addr_Split_Frame!=0)
				ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//�ͷ��ҵ��ڴ�ѷ���
			UARTCtrl->Addr_Split_Frame=0;
			UARTCtrl->Save_Split_Frame_len=0;
			UARTCtrl->Last_Block_No=0xffffffff;
			return 1;
		}
		else
		{
			if(UARTCtrl->Last_Block_No!=0xffffffff)//�Ѿ����ǵ�һ֡��,��Ҫ�ж����
			{
				if(block_no!=UARTCtrl->Last_Block_No+1)//ǰ��֡���򲻶ԣ�����
				{
					if(UARTCtrl->Addr_Split_Frame!=0)
						ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//�ͷ��ҵ��ڴ�ѷ���
					UARTCtrl->Addr_Split_Frame=0;
					UARTCtrl->Save_Split_Frame_len=0;
					UARTCtrl->Last_Block_No=0xffffffff;
					return 1;
				}
			}
			//�����˵����֡����ȷ�ķ�֡������һ֡����֡���APDU���ֿ�����p_save_frame��
			UARTCtrl->Last_Block_No=block_no;//����֡���
			if(UARTCtrl->Addr_Split_Frame==0)//��û������ռ�
			{
				p_save_frame=ms->ms_malloc(8*1024);//��̬�����ҵ��ڴ��
				UARTCtrl->Addr_Split_Frame=(u32)p_save_frame;
				if(UARTCtrl->Addr_Split_Frame==0)//����ռ�ʧ��
				{	
					return 1;
				}
				UARTCtrl->Save_Split_Frame_len=0;
			}
			if(block_no==0)//��һ֡ʱ������֡ͷ
			{
				MR(UARTCtrl->Addr_Split_Frame+UARTCtrl->Save_Split_Frame_len,(u32)p8rxbuff,14);
				UARTCtrl->Save_Split_Frame_len+=14;
			}
			//���ݷ�֡��APDU����
			MR(UARTCtrl->Addr_Split_Frame+UARTCtrl->Save_Split_Frame_len,((u32)p8rxbuff)+apdu_offset+2,fragment_len);
			UARTCtrl->Save_Split_Frame_len+=fragment_len;

			if((split_frame&0xc000)==0x4000)//��֡���һ֡��
			{
				MR((u32)p8rxbuff,UARTCtrl->Addr_Split_Frame,UARTCtrl->Save_Split_Frame_len);
				if(UARTCtrl->Addr_Split_Frame!=0)
					ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//�ͷ��ҵ��ڴ�ѷ���
				UARTCtrl->Addr_Split_Frame=0;
				UARTCtrl->Last_Block_No=0xffffffff;
				UARTCtrl->Save_Split_Frame_len=0;
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	else//���Ƿ�֡��ʱ��ȥ���֡��Ҫ�õ��Ĳ�����Ϊ��֡��׼��
	{
		UARTCtrl->Last_Block_No=0xffffffff;
		if(UARTCtrl->Addr_Split_Frame!=0)
		{
			ms->ms_free((u8*)UARTCtrl->Addr_Split_Frame);//�ͷ��ҵ��ڴ�ѷ���
			UARTCtrl->Addr_Split_Frame=0;
		}
		UARTCtrl->Save_Split_Frame_len=0;
	}
	return 0;//����0��ȷ,1��ַ��,2�������
}

void Terminal_ReadMeter_DL698(u32 PORTn)//�ն˳���RS485��_�Ϻ���Լ
{
	u32 i;
	u32 x;
	u32 Fn;
	u32 Pn;
	u8 * p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
	u32 ClassNo;

	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	u32 y;


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
				Fn=MRR(DL698_RMLib[UARTCtrl->FnCount],1);
				if(Fn!=0xe1)
				{
					UARTCtrl->RMCountErr++;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				}
				if(Fn==3)
				{
					UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
					UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
					UARTCtrl->RMDI=0xffffffff;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
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
				default:
					return;
			}
#if (((Project/100)==2)||((Project/100)==5))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��,700=
			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//�ն˳������в�������,�˿�31
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
			if(UARTCtrl->FnCount>=Get_RM_DL698_MaxFn())
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
				UARTCtrl->FnEnd=0;//485����Fn����������־,0=����,1=û����
				UARTCtrl->FnCount=0;//Fn����=0
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->SearchDICount=0;//�����¼�����AFN04F106������������
				UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
				UARTCtrl->RMDI=0xffffffff;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
				UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				UARTCtrl->RMError=0;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				break;
			}
			if(DL698_RMLib[UARTCtrl->FnCount]==0x0)
			{//��Fn�޳���
NEXT_Fn:
				UARTCtrl->Task=1;//���1֡����
				UARTCtrl->FnCount+=1;//Fn����+1
				UARTCtrl->RMCount=0;//�����������=0
				UARTCtrl->ReRMCount=0;//���������ط�����
				UARTCtrl->RMCountErr=0;//Fn���г���ʧ�ܻ򲻳������ݱ�ʶ����
				UARTCtrl->RMError&=0xfe;//�����ܱ�/��������װ�����ͨ��ʧ�ܱ�־,B0=ʧ��,B1=F129ʧ��
				UARTCtrl->RMDICount=0;//�����¼������ѳ�DI����
				UARTCtrl->RMDI=0xffffffff;//�����¼����������ݱ�ʶ,0xffffffff��ʾ��Ч,����ֵ��ʾҪ�������ݱ�ʶ�����ÿ��еĿ鳭��ʶ�ͷ��¼���ĳ������ݱ�ʶ
				
				break;	
			}
			p8RMLib=(u8 *)DL698_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//��Fn����RS485����
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//ȡ��С���			
			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,3);//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������
			i=UARTCtrl->RMCount;
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
				goto NEXT_Fn;
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
				if(p8RMLib[1]>UARTCtrl->RMCountErr)
				{//����ȫ����ʶ��û��
					ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����;��ֻ�洢ʵʱ����u8 *p8RMLib=0
				}
//				goto NEXT_Fn;//�Ϻ���Լ�����û��0x2A���һ���������㷢��ʱ�̣��������κ�����֡����������һ��Fn����F27û�����ݡ�
			}

			Terminal_ReadMeter_DL698_TxData(PORTn,p8RMLib);//�ն��Ϻ���Լ����������

			if(UARTCtrl->TxByte==0)//��֡ʧ��,Terminal_ReadMeter_DL698_TxData_TESAM���п���ʧ��
				goto NEXT_Fn;
				
			//ͨ������
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ĭ��1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
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
					CopyString((u8 *)"485 �ܳ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 3://����
					CopyString((u8 *)"485 ����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
					break;
				case 5://�¼�
					CopyString((u8 *)"485 �¼�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
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
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MWR(PORT485,ADDR_DATABUFF+3,1);
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
			i=DL698_Rx(PORTn);//698.45��Լ֡����;����0û���,1���,2��ʱ
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
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
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
			
			//�ȽϷ��յ�ַ�������롢��0x33
			i=DL698_RxData(PORTn);//698.45��Լ֡��������,�ȽϷ��յ�ַ�������롢��0x33;����0��ȷ,1��ַ��,2�������
			if(i!=0)
			{
				UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
				break;
			}
			
			UARTCtrl->Task=1;//���1֡����
			p8RMLib=(u8 *)DL698_RMLib[UARTCtrl->FnCount];
			ReadMeter_DL698_FnData(PORTn,p8RMLib);//698.45��Լ���������������Fn���ݻ���
			UARTCtrl->RMCount+=1;//�����������+1
			i=UARTCtrl->RMCount;
			UARTCtrl->ReRMCount=0;//���������ط�����=0
			break;
	}
}


void Terminal_ReadMeter_DL698_Fn(u32 PORTn,u8 *p8RMLib)//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
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
	Log_Dumphex_Level(LOG_LEVEL_DEBUG,"in Terminal_ReadMeter_DL698_Fn",0,0);	
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
//				ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//��������Fn����
				goto END_Fn;
			}
			
			Terminal_ReadMeter_DL698_TxData(PORTn,p8RMLib);//�ն��Ϻ���Լ����������
			
			//ͨ������
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ĭ��1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
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
				x=p8rxbuff[7-i];
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
			i=DL698_Rx(PORTn);//�Ϻ���Լ֡����;����0û���,1���,2��ʱ
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
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
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
			
			//�ȽϷ��յ�ַ�������롢��0x33
			i=DL698_RxData(PORTn);//�Ϻ���Լ֡��������,�ȽϷ��յ�ַ�������롢��0x33;����0��ȷ,1��ַ��,2�������
			if(i!=0)
			{
				UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
				break;
			}
			UARTCtrl->Task=1;//���1֡����
			
			ReadMeter_DL698_FnData(PORTn,p8RMLib);//�Ϻ���Լ���������������Fn���ݻ���
			UARTCtrl->RMCount+=1;//�����������+1
			UARTCtrl->ReRMCount=0;//���������ط�����=0
			break;
	}
}

#endif
