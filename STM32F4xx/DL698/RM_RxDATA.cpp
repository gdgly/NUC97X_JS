
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_TxDATA.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_Uart.h"

#include "../DL698/DL698_JSON.h"


u32 RM_Rx(u32 PORTn)//�������֡;���:PORTn;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
	if(i>=2)
	{
		i=UARTCtrl->AutoReportRMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	}
	else
	{
		i=UARTCtrl->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	}
	switch(i)
	{
		default://δ֪ (0)
			return RM_Rx_Unknown(PORTn);//δ֪��Լ����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
		case 1://DL/T645-1997
		case 2://DL/T645-2007
			return RM_Rx_DL645(PORTn);//UART_645����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
		case 3://DL/T698.45
			return Uart_698_Rx(PORTn);//UART_698����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
		case 4://CJ/T 188-2004
			return RM_Rx_Unknown(PORTn);//δ֪��Լ����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
		
		case ProtocolNO_ABB://ABB
			return RM_Rx_ABB(PORTn);//UART_645����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
		case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
			return RM_Rx_Landis(PORTn);//UART_645����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ

	}
}

u32 RM_RxDATA_CHECK(u32 PORTn)//֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����:0=��ȷ,1=��ַ��,2=�������,3=���ݱ�ʶ��
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
	if(i>=2)
	{
		i=UARTCtrl->AutoReportRMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	}
	else
	{
		i=UARTCtrl->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	}
	switch(i)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	{
		default://δ֪ (0)
			return 0;
		case 1://DL/T645-1997
			return RM_RxDATA_DL1997_CHECK(PORTn);//DL645_1997֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
		case 2://DL/T645-2007
			return RM_RxDATA_DL2007_CHECK(PORTn);//DL645_2007֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
		case 3://DL/T698.45
			return RM_RxDATA_DL698_CHECK(PORTn);//DL698֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
		case 4://CJ/T 188-2004
			return 0;
		
		case ProtocolNO_ABB://ABB
			return RM_RxDATA_ABB_CHECK(PORTn);//ABB֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
		case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
			return RM_RxDATA_Landis_CHECK(PORTn);//Landis֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��

	}
}





//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     1024
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������,ȫ������Ϊ����˳��������������)

//���������ϲ������ݻ���1
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29 �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������,ȫ������Ϊ����˳��������������)

void RM_RxDATA_START(u32 PORTn)//����������ݴ���-��ʼ
{
	u32 id;
	u8* p8;
	u16* p16;
  UARTCtrl_TypeDef * UARTCtrl;
	RecordFileHead_TypeDef* RecordFileHead;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	RecordFileHead=(RecordFileHead_TypeDef*)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
	
	MR((u32)&RecordFileHead->Addr0,(u32)&UARTCtrl->RMAddr,6);//�ɼ�ͨ�ŵ�ַ
	RecordFileHead->LEN_DATA=23;//����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
	
	id=UARTCtrl->TaskID;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	id>>=8;
	id&=0xff;//�����
	MR(((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef),(u32)&UARTCtrl->TaskTime[id].StdTime,7);//�ɼ�����ʱ��(����ʼ��׼ʱ��)  date_time_s(����������)
//	DateSaveTI(PORTn,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14);//�ɼ��洢ʱ��
	p8=(u8*)RecordFileHead;
	p8[sizeof(RecordFileHead_TypeDef)+21]=DataType_array;//�ɼ���CSD  array(����������)
	p8[sizeof(RecordFileHead_TypeDef)+22]=0;
	
	p16=(u16*)RecordFileHead;
	p16[LEN_RM_DI_CSD/2]=0;//����=����д��buff����û����һ��CSD�ĳ���
	p16[(LEN_RM_DI_CSD/2)+1]=0;//����=����д��buff����CSD�ĳ���(����������2�ֽ�)
	//p16[(LEN_RM_DI_CSD/2)+2]=DataType_array;//�ɼ�������(����������)
}

#if (USER/100)==15//����
void RM_RxDATA_END(u32 PORTn)//����������ݴ���-����
{
	u32 len1;
	u32 len2;
	u16* p16;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 i;
	u8* pDATA;
	u32 x;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p16=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
	RecordFileHead=(RecordFileHead_TypeDef*)p16;
	MR((u32)p16+sizeof(RecordFileHead_TypeDef)+7,ADDR_DL698YMDHMS,7);//�ɼ��ɹ�ʱ��  date_time_s(����������)
	DateSaveTI(PORTn,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14);//�ɼ��洢ʱ��
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;//�����
	if((i==2)&&(UARTCtrl->RMprotocol==2))
	{
		MR(ADDR_4KWRITE_BUFF,((u32)p16)+LEN_RM_DI_CSD+4,8);//����ʱ��
		pDATA=(u8*)ADDR_4KWRITE_BUFF;
		x=Compare_DL698DataString(pDATA+1,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14,7);//�������ݶ���ʱ��Ͳɼ��ɹ�ʱ������ڲ������������������
		if(x!=0)
		{
			len2=15;
			MC(0,((u32)p16)+LEN_RM_DI_CSD+4,15);
		}
		else
		{
			len2=p16[(LEN_RM_DI_CSD/2)+1];
		}
		len1=RecordFileHead->LEN_DATA;
		RecordFileHead->LEN_DATA=len1+len2;
		MR(((u32)p16)+sizeof(RecordFileHead_TypeDef)+len1,((u32)p16)+LEN_RM_DI_CSD+4,len2);
	}
	else
	{
		//�ϲ�����
		len1=RecordFileHead->LEN_DATA;
		len2=p16[(LEN_RM_DI_CSD/2)+1];
		RecordFileHead->LEN_DATA=len1+len2;
		MR(((u32)p16)+sizeof(RecordFileHead_TypeDef)+len1,((u32)p16)+LEN_RM_DI_CSD+4,len2);
	}
}

#else
void RM_RxDATA_END(u32 PORTn)//����������ݴ���-����
{
	u32 len1;
	u32 len2;
	u16* p16;
	RecordFileHead_TypeDef* RecordFileHead;
	
	p16=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
	RecordFileHead=(RecordFileHead_TypeDef*)p16;
	MR((u32)p16+sizeof(RecordFileHead_TypeDef)+7,ADDR_DL698YMDHMS,7);//�ɼ��ɹ�ʱ��  date_time_s(����������)
	DateSaveTI(PORTn,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14);//�ɼ��洢ʱ��
	//�ϲ�����
	len1=RecordFileHead->LEN_DATA;
	len2=p16[(LEN_RM_DI_CSD/2)+1];
	RecordFileHead->LEN_DATA=len1+len2;
	MR(((u32)p16)+sizeof(RecordFileHead_TypeDef)+len1,((u32)p16)+LEN_RM_DI_CSD+4,len2);
}

#endif


u32 RM_One_CSD(u32 PORTn)//��鳭���Ƿ����1��������CSD������;����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
{
	u32 i;
	u32 n;
	u32 OAD;
	u32 RMOADCount;
	u32 OADCountOK;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u8 *p8CSD;
	u16 *p16Data;
	UARTCtrl_TypeDef * UARTCtrl;
	u8* p8CSDlast;
	
	n=n;
	OAD=OAD;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���(��DL698����ʱ)
	if(i>=UARTCtrl->RMCSDSubCountMax)//����CSD���ӳ������(��DL698�����Լ���趨)
	{//1��OAD����
		//�ɼ���CSD  array
		if(UARTCtrl->RMROADCountMax<=1)
		{//Ҫ����CSDΪOAD��ROAD���ֶ��OAD��
	#if LEN_RecordFileHead<8
		#error
	#endif
			//һ������OAD�����ݳ���
			p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
			p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
			LEN_Data0=p16Data[0];
			LEN_Data1=p16Data[1];
			if(LEN_Data0==0)
			{//������
				return 0;
			}
			LEN_Data1+=LEN_Data0;
			p16Data[1]=LEN_Data1;
			p16Data[0]=0;
			//
			p16Data=(u16*)(p8CSD+6);
			LEN_Data1=p16Data[0];
			LEN_Data0=Get_DL698DataLen_S(UARTCtrl->RMCSD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if((LEN_Data1+LEN_Data0)>(LEN_RM_DI_CSD-sizeof(RecordFileHead_TypeDef)))
			{
				return 1;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
//		#if (USER==901)//���ϰ�
			//�������ǡ�00100201���͡�00200201������վ�á�00100200���͡�00200200���ٶ������ʵ��������ǿգ�ֻ���ܵ������ؾ�����
			i=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|(UARTCtrl->RMCSD[5]<<0);
			if(i==0x50020200)
			{//���Ӷ���(����)
				i=UARTCtrl->RMCSD[6];
				for(n=0;n<i;n++)
				{
					OAD=(UARTCtrl->RMCSD[7+(4*n)]<<24)|(UARTCtrl->RMCSD[7+(4*n)+1]<<16)|(UARTCtrl->RMCSD[7+(4*n)+2]<<8)|(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
					switch(OAD)
					{
						case 0x00100201://�����й�������
						case 0x00200201://�����й�������
						case 0x00300201://����޹�1������
						case 0x00400201://����޹�2������
						case 0x00500201://��1�����޹�������
						case 0x00600201://��2�����޹�������
						case 0x00700201://��3�����޹�������
						case 0x00800201://��4�����޹�������
						case 0x20000201://A���ѹ
						case 0x20010201://A�����
						case 0x20040202://A���й�����
						case 0x20050202://A���޹�����
						case 0x200A0202://A�๦������
							UARTCtrl->RMCSD[7+(4*n)+3]=0;
							break;
					}
				}
			}
//		#endif
			p8CSDlast=p8CSD;
			p8CSDlast+=sizeof(RecordFileHead_TypeDef)+21;//ָ��ɼ���CSD  array CSD(����������)
			i=p8CSDlast[1];//ȡ������
			if((i==0)||(UARTCtrl->RMCSD[1]==0))
			{//������=0�򱾴�д�벻��ROAD
				MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,(u32)&UARTCtrl->RMCSD,LEN_Data0);
				LEN_Data1+=LEN_Data0;
				p16Data[0]=LEN_Data1;
				p8CSD+=sizeof(RecordFileHead_TypeDef)+21;
				i=p8CSD[1];//array CSD����
				i++;
				p8CSD[1]=i;
			}
			else
			{//������>=1
				p8CSDlast[1]=i-1;//ȡ������-1,���������γ������ݵ�OAD��ROAD��ַ�����ڱȽ��Ƿ��뱾����ͬ
				n=Get_DL698DataLen_S(p8CSDlast,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				p8CSDlast[1]=i;//�ָ�������
				p8CSDlast+=n;
				for(n=0;n<6;n++)
				{
					if(p8CSDlast[n]!=UARTCtrl->RMCSD[n])
					{
						break;
					}
				}
				if(n<6)
				{//���ϴβ���ͬ
					MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,(u32)&UARTCtrl->RMCSD,LEN_Data0);
					LEN_Data1+=LEN_Data0;
					p16Data[0]=LEN_Data1;
					p8CSD+=sizeof(RecordFileHead_TypeDef)+21;
					i=p8CSD[1];//array CSD����
					i++;
					p8CSD[1]=i;
				}
				else
				{//���ϴ���ͬ
					if(LEN_Data0>=7)
					{
						LEN_Data0-=7;
					}
					MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,((u32)&UARTCtrl->RMCSD)+7,LEN_Data0);
					LEN_Data1+=LEN_Data0;
					p16Data[0]=LEN_Data1;
					i=UARTCtrl->RMCSD[6];//����ROAD�е�OAD����
					n=p8CSDlast[6];//ԭROAD�е�OAD����
					i+=n;
					if(i>127)
					{
						i=127;
					}
					p8CSDlast[6]=i;
				}
			}
		}
		else
		{//ROAD�ֶ��OAD��
			//һ������OAD�����ݳ���
			p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
			p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
			LEN_Data0=p16Data[0];
			LEN_Data1=p16Data[1];
			if(LEN_Data0==0)
			{//������
				return 0;
			}
			LEN_Data1+=LEN_Data0;
			p16Data[1]=LEN_Data1;
			p16Data[0]=0;
			//
			RMOADCount=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
			OADCountOK=UARTCtrl->RMROADCountOK;//�������ݻ���ROADʱ�����������OAD���ݸ�������
			if(RMOADCount>=UARTCtrl->RMROADCountMax)
			{
				return 6;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			if(OADCountOK>RMOADCount)
			{
				return 6;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			p16Data=(u16*)(p8CSD+6);
			LEN_Data1=p16Data[0];
			
			p8CSDlast=p8CSD;
			p8CSDlast+=sizeof(RecordFileHead_TypeDef)+21;//ָ��ɼ���CSD  array CSD(����������)
			i=p8CSDlast[1];//ȡ������
			if((i==0)||(UARTCtrl->RMCSD[1]==0))
			{//������=0�򱾴�д�벻��ROAD
				p8CSDlast=0;//�����
			}
			else
			{//������>=1
				p8CSDlast[1]=i-1;//ȡ������-1,���������γ������ݵ�OAD��ROAD��ַ�����ڱȽ��Ƿ��뱾����ͬ
				n=Get_DL698DataLen_S(p8CSDlast,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				p8CSDlast[1]=i;//�ָ�������
				p8CSDlast+=n;
				for(n=0;n<6;n++)
				{
					if(p8CSDlast[n]!=UARTCtrl->RMCSD[n])
					{
						break;
					}
				}
				if(n<6)
				{//���ϴβ���ͬ
					p8CSDlast=0;//�����
				}
				else
				{//���ϴ���ͬ
					
				}
			}
			
			if((OADCountOK==0)&&(p8CSDlast==0))
			{//����=0,��дROAD��ǰ7�ֽ�
				if((LEN_Data1+7)>(LEN_RM_DI_CSD-sizeof(RecordFileHead_TypeDef)))
				{
					return 1;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,(u32)&UARTCtrl->RMCSD,7);
				LEN_Data1+=7;
			}
			if((LEN_Data1+4)>(LEN_RM_DI_CSD-sizeof(RecordFileHead_TypeDef)))
			{
				return 1;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
//		#if (USER==901)//���ϰ�
			//�������ǡ�00100201���͡�00200201������վ�á�00100200���͡�00200200���ٶ������ʵ��������ǿգ�ֻ���ܵ������ؾ�����
			i=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|(UARTCtrl->RMCSD[5]<<0);
			if(i==0x50020200)
			{//���Ӷ���(����)
				i=UARTCtrl->RMCSD[6];
				for(n=0;n<i;n++)
				{
					OAD=(UARTCtrl->RMCSD[7+(4*n)]<<24)|(UARTCtrl->RMCSD[7+(4*n)+1]<<16)|(UARTCtrl->RMCSD[7+(4*n)+2]<<8)|(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
					switch(OAD)
					{
						case 0x00100201://�����й�������
						case 0x00200201://�����й�������
						case 0x00300201://����޹�1������
						case 0x00400201://����޹�2������
						case 0x00500201://��1�����޹�������
						case 0x00600201://��2�����޹�������
						case 0x00700201://��3�����޹�������
						case 0x00800201://��4�����޹�������
						case 0x20000201://A���ѹ
						case 0x20010201://A�����
						case 0x20040202://A���й�����
						case 0x20050202://A���޹�����
						case 0x200A0202://A�๦������
							UARTCtrl->RMCSD[7+(4*n)+3]=0;
							break;
					}
				}
			}
//		#endif
			MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,((u32)&UARTCtrl->RMCSD)+7+(4*RMOADCount),4);
			LEN_Data1+=4;
			p16Data[0]=LEN_Data1;
			OADCountOK++;
			UARTCtrl->RMROADCountOK=OADCountOK;//�������ݻ���ROADʱ�����������OAD���ݸ�������
			if(p8CSDlast)
			{//���
				i=p8CSDlast[6];//ROAD�е�OAD����+1
				if(i<127)
				{
					i++;
				}
				p8CSDlast[6]=i;
			}
			else
			{//�����
				p8CSD+=sizeof(RecordFileHead_TypeDef)+LEN_Data1;
				p8CSD-=(4*OADCountOK);
				p8CSD-=1;
				p8CSD[0]=OADCountOK;//SEQUENCE OF OAD����
			}
			if((OADCountOK==1)&&(p8CSDlast==0))
			{//array CSD����
				p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
				p8CSD+=sizeof(RecordFileHead_TypeDef)+21;
				i=p8CSD[1];//array CSD����
				i++;
				p8CSD[1]=i;
			}

		}
	}
	return 0;
}





/*
u32 Uart_645_CS(u8 *p8)//DL645֡CSֵ����,�Ƚ�,����дCSֵ,ͬʱ�ڽ�β��0x16;����0=��ȷ��1=����
{
	u32 i;
	u32 x;
	u32 cs;

	x=p8[9];//����
	//cs
	cs=0;
	x+=10;
	for(i=0;i<x;i++)
	{
		cs+=p8[i];
	}
	cs&=0xff;
	if(cs==p8[x])
	{
		i=0;//0=��ȷ��1=����
	}
	else
	{
		i=1;//0=��ȷ��1=����
	}
	p8[x]=cs;
	//��β0x16
	p8[x+1]=0x16;
	return i;
}
*/

u32 RM_Rx_Unknown(u32 PORTn)//δ֪��Լ����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
{
	u32 i;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
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
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
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
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//��ʱ
		if(RxByte)
		{
			return 1;//1=����OK
		}
		else
		{
			return 2;//2=���ճ�ʱ
		}
	}
	return 0;
}

u32 RM_Rx_DL645(u32 PORTn)//UART_645����;����:0=���ڽ���,1=���1֡����,2=���ճ�ʱ
{
	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
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
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
		}
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
			if(RxByte==8)
			{
				if(i!=0x68)
				{
					RxByte=0;
					continue;
				}
			}
			else
			{
				if(RxByte>=12)
				{
					if(RxByte>=(p8rxbuff[9]+12))
					{
						if(i==0x16)
						{//�������
							//��У�����
							Pn=0;
							for(i=0;i<(p8rxbuff[9]+10);i++)
							{
								Pn+=p8rxbuff[i];
							}
							Pn&=0xff;
							if(Pn==p8rxbuff[i])
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
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//��ʱ
		return 2;//2=���ճ�ʱ
	}
	return 0;
}



void RM_RxDATA(u32 PORTn)//����������ݴ���
{
	u32 Err;
  UARTCtrl_TypeDef * UARTCtrl;
	u16* p16timer;
	u8* p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Err=UARTCtrl->CONNECT;
	switch(Err)//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
	{
		case 1://���ӹ���1
		case 2://���ӹ���2
		case 102://�ر�����
			switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
			{
				default://δ֪ (0)
				case 1://DL/T645-1997
				case 2://DL/T645-2007
				case 3://DL/T698.45
				case 4://CJ/T 188-2004
					UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
					break;;
				case ProtocolNO_ABB://ABB
					RM_RxDATA_ABB(PORTn);//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					break;;
				case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
					RM_RxDATA_Landis(PORTn);//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					break;;
			}
			return;
		
		case 101://101=������
			break;
		default:
			UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������,102=�ر�����
			return;
	}
	switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	{
		default://δ֪ (0)
			return;
		case 1://DL/T645-1997
			Err=RM_RxDATA_DL1997(PORTn);//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			break;
		case 2://DL/T645-2007
			Err=RM_RxDATA_DL2007(PORTn);//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:Err=%d\n",(u32)&__func__,(u32)__LINE__,Err);
			#endif
			break;
		case 3://DL/T698.45
			Err=RM_RxDATA_DL698(PORTn);//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			break;
		case 4://CJ/T 188-2004
			return;
		case ProtocolNO_ABB://ABB
			while(1)
			{
				Err=RM_RxDATA_ABB(PORTn);//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				if(Err)
				{
					break;
				}
				UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
				Err=RM_RxDATA_ABB(PORTn);//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				if(Err!=2)
				{
					UARTCtrl->CONNECT = 102; 
					return;
				}
			}	
			break;
		case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
			while(1)
			{
				Err=RM_RxDATA_Landis(PORTn);//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				if(Err)
				{
					break;
				}
				UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
				Err=RM_TxDATA_Manage(PORTn);//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				if(Err!=2)
				{
					return;
				}
			}	
			break;
	}
	if(Err)
	{
		switch(Err)
		{
			case 1:
				Err=CopyString((u8 *)"����1���Ȳ���",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 2:
				Err=CopyString((u8 *)"����2���Ȳ���",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 3:
				Err=CopyString((u8 *)"�������йس���ֵ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 4:
				Err=CopyString((u8 *)"�޶�Ӧ�ӿ����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 5:
				Err=CopyString((u8 *)"����������127",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
			case 6:
				Err=CopyString((u8 *)"����������������",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				break;
		}
		p8=(u8*)ADDR_DATABUFF;
		p8[Err]=0;
		WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
		p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
		p16timer[0]=1000/10;//��֡����������ʱ,����ʾ������
	}
}






