
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../Terminal698/Terminal_Init.h"
#include "../DL698/DL698TEST.h"
#include "../DL698/Info_3762.h"

#include "../DL698/DL698_JSON.h"


/*
#define ADDR_6034     ADDR_6032+LENmax_6032//�ɼ������ؼ�
//0 �ɼ������ص�Ԫ��=structure
//{
//2 ����ID       unsigned
//4 ����ִ��״̬  enum
//{
//	δִ�У�0��
//	ִ���У�1��
//	��ִ�У�2��
//}��
//6 ����ִ�п�ʼʱ��   date_time_s��//�ø��˿�ʱ��ֵС��
//14 ����ִ�н���ʱ��   date_time_s��//�ø��˿�ʱ��ֵ���
//22 �ɼ�������         long-unsigned��
//25 �ɼ��ɹ�����       long-unsigned��
//28 �ѷ��ͱ�������     long-unsigned��
//31 �ѽ��ձ�������     long-unsigned
//}
//˵�����ɼ��ɹ��������ѷ��ͱ����������ѽ��ձ���������ÿ������ִ�������㡣
#define LENper_6034     34
#define NUMmax_6034     NUMmax_6012_2
#define LENmax_6034     (LENper_6034*NUMmax_6034)
*/


//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//���ȼ�:1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����,��չ14=�ն��¼�(���ϱ�),15=�ն��¼�(�ϱ�)
//��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�
//��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��





u32 RM_Addr(u32 PORTn,u16* pAddr)//ָ��ָ���ͨ�ŵ�ַ�Ƿ��ѳ�;����:0=û��,1=�ѳ�
{
	u32 i;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	RMokfailLIST_TypeDef *rmOKFAILlist;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;
	if(i>=(RMTASKmax+ExtRMTASKmax))
	{
		return 0;//����:0=û��,1=�ѳ�
	}
	rmOKFAILlist=UARTCtrl->rmOKFAILlist[i];
	i=UARTCtrl->NUMrmOK[i];//�������Ѳɼ���ɵ��ܱ�����
	if(i>NUM_RMmax)
	{
		i=NUM_RMmax;
	}
	Addr0=pAddr[0];
	Addr1=pAddr[1];
	Addr2=pAddr[2];
	while(i--)
	{
		if(Addr0==rmOKFAILlist->Addr0)
		{
			if(Addr1==rmOKFAILlist->Addr1)
			{
				if(Addr2==rmOKFAILlist->Addr2)
				{
					return 1;//����:0=û��,1=�ѳ�
				}
			}
		}
		rmOKFAILlist++;
	}
	return 0;
}

u32 RM_MeterCount(u32 PORTn)//���ܱ����+1;����:0=����,1=�����
{
	u32 i;
	u32 Nmax;
	u32 id;
	UARTCtrl_TypeDef *UARTCtrl;
	u16* p16Data;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->CONNECT=0;//DL698���ܱ�����:0-100���ӹ�����,100=����,101=
	p16Data=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data[3]=0;//����=ʱ��+�Ѳɼ���CSD
	p16Data+=LEN_RM_DI_CSD/2;
	p16Data[0]=0;//����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
	p16Data[1]=0;//����=����д��buff����CSD�ĳ���(����������2�ֽ�)
	
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return 1;//����:0=����,1=�����
	}
	id>>=8;
	id&=0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return 1;//����:0=����,1=�����
	}
#if RM_HPLC_Concurrency==1//0=��,1=��;����ز�������������������
	if(PORTn==RS485_4PORT)
	{
		if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
		{//�ǿ��
			//UARTCtrl->MeterCount[id]=UARTCtrl->SourMeterCount_id;//��������ʱ��ǰ����id��ԭʼ������ܱ����
		}
	}
#endif
	i=UARTCtrl->MeterCount[id];//�������ѳ����ܱ����
	i++;
	UARTCtrl->MeterCount[id]=i;
	
	Nmax=NUM_RMmax;
	switch(id)
	{
		case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
			Nmax=PowerEvent->NUMrm;//ͣ�ϵ��¼��賭��������
			if(Nmax>NUM_RMmax)
			{
				Nmax=NUM_RMmax;
			}
			if(i>=Nmax)
			{
				i=UARTCtrl->RMLOOP[id];
				if(i!=0xff)
				{//RMTASKmax byte ȫ������һ��ı�������
					i++;
					UARTCtrl->RMLOOP[id]=i;
				}
			}
			break;
	}
	if(i>=Nmax)
	{
		UARTCtrl->MeterCount[id]=0;//�������ѳ����ܱ����
//		i=UARTCtrl->TaskTime[id].StdTimeCount;//ÿ��Ƶ�ʵ��׼ʱ�����+1,ÿִ��һ��-1
//		if(i)
//		{
//			i--;
//			UARTCtrl->TaskTime[id].StdTimeCount=i;
//		}
	}
#if RM_HPLC_Concurrency==1//0=��,1=��;����ز�������������������
	if(PORTn==RS485_4PORT)
	{
		if(Terminal_Router->RouterRunMode_1==2)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
		{//�ǿ��
			UARTCtrl->SourMeterCount_id=UARTCtrl->MeterCount[id];//��������ʱ��ǰ����id��ԭʼ������ܱ����
		}
	}
#endif
	return 0;//����:0=����,1=�����
}


u16* MeterSetNoToCollectorAddr(u32 MeterSetNo)//ȡ�ɼ�����������ŵĲɼ�����ַ;���:��0��ʼ�����;����:�ɼ�����ַָ��,0=��Ч
{
	u8* p8;
	
	p8=(u8*)ADDR_6000_SDRAM+(MeterSetNo*LENper_6000);
	p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
	if(p8==0)
	{
		return 0;
	}
	return (u16*)(p8+3);
}

void NUMrm(u32 PORTn)//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
{
	u32 TaskID;
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 id;
	u32 OADPORTn;
	u32 NUMrm;
	u32 filename;
	u8* pMS;
	u8 *p8s;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	ms_Type *ms;
	msFILE_TypeDef *file;
	RecordFileHead_TypeDef* RecordFileHead;//�����¼�ļ�ͷ
	RMokfailLIST_TypeDef *rmOKFAILlist;

	
	ms=Get_ms();
//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//���ȼ�:1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����,��չ14=�ն��¼�(���ϱ�),15=�ն��¼�(�ϱ�)
//��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�,4=����
//��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	TaskID=UARTCtrl->TaskID;
	if(TaskID==0)
	{
		return;
	}
	id=(TaskID>>8)&0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return;
	}
	UARTCtrl->NUMrmTOTAL[id]=0;//������ɼ�����
	UARTCtrl->NUMrmOK[id]=0;//�������Ѳɼ���ɵ��ܱ���
	UARTCtrl->NUMrmBAD[id]=0;//�������Ѳɼ���ɵĵ��ܱ����������ñ����ԭ�����賭������
	//UARTCtrl->NUMrmFAIL[id]=0;//�������Ѳɼ�ʧ�ܵ��ܱ���

	
//���˿��ڵ��������õ��Ǳ�����
	NUMrm=0;//���õ��Ǳ�����
	OADPORTn=PORTntoOAD(PORTn);
	for(i=0;i<NUM_RMmax;i++)
	{
		p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
		if(p8s[0]==DataType_structure)
		{
			//�˿�
			p8d=Get_Element(p8s+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8d)
			{
				x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
				if(x==OADPORTn)
				{
					NUMrm++;//���õ��Ǳ�����
				}
			}
		}
	}
	UARTCtrl->NUM_SetMeter=NUMrm;//���˿��ڵ��������õ��Ǳ�����
//������
	switch(id)
	{
		case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
			NUMrm=0;//��ɼ�����
			OADPORTn=PORTntoOAD(PORTn);
			for(i=0;i<PowerEvent->NUMrm;i++)
			{
				x=PowerEvent->setno[i];
				p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
				//�˿�
				p8d=Get_Element(p8s+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8d)
				{
					x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
					if(x==OADPORTn)
					{
						NUMrm++;//��ɼ�����
					}
				}
			}
			UARTCtrl->NUMrmTOTAL[id]=NUMrm;//������ɼ�����
			return;
	}
	
	i=TaskID&0xff;//�������
	switch((TaskID>>24)&0xf)//��������
	{
		case 1://��ͨ�ɼ�����
			i*=LENper_6014;
			i+=ADDR_6014_SDRAM;
			p8s=(u8*)i;
			if(p8s[0]!=DataType_structure)
			{
				return;
			}
			pMS=Get_Element(p8s,5,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(pMS==0)
			{
				return;
			}
/*
			//��¼��ѡ��CSD, ��������DateSaveTI()Ҫ��
			p8d=Get_Element(p8s,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8d==0)
			{
				return;
			}
			i=UARTCtrl->RMCSDCount;//����CSD����
			p8d=Get_Element(p8d,i+1,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8d==0)
			{
				return;
			}
			i=(p8d[2]<<8)|(p8d[3]<<0);//�ɼ����ݵ�OI
			UARTCtrl->RMCSD[2]=i>>8;//OI
			UARTCtrl->RMCSD[3]=i;//OI
			switch(i)
			{
				case 0x5002://���Ӷ���,����������,��Ჹ����96�����������Чʱ��û��ͬ�ļ���ʹ�ѳ���Ϊ0,��RM_TxDATA.c�л�ÿһ�����Ƿ��ѳ�
					UARTCtrl->RMCSD[2]=0xff;//��Ч�Ĳɼ����ݵ�OI,��DateSaveTI()Ҫ��
					UARTCtrl->RMCSD[3]=0xff;//��Ч�Ĳɼ����ݵ�OI,��DateSaveTI()Ҫ��
					break;
//				case 0x5004://�ն���
//				case 0x5006://�¶���
//				default:
//					break;
			}
*/
/*
			UARTCtrl->RMCSD[2]=0xff;//��Ч�Ĳɼ����ݵ�OI,��DateSaveTI()Ҫ��
			UARTCtrl->RMCSD[3]=0xff;//��Ч�Ĳɼ����ݵ�OI,��DateSaveTI()Ҫ��
*/
			break;
		case 2://�¼��ɼ�����
			i*=LENper_6016;
			i+=ADDR_6016_SDRAM;
			p8s=(u8*)i;
			if(p8s[0]!=DataType_structure)
			{
				return;
			}
			pMS=Get_Element(p8s,3,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(pMS==0)
			{
				return;
			}
			break;
		case 3://͸������
#if LENper_6018>LEN_128KDATABUFF
	#error
#endif
//#define ADDR_6018     ADDR_6016+LENmax_6016//͸��������(��ʼ��ֵ0x00)
//����2��͸����������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	�������     unsigned��
//	�������ݼ�   array �������ݣ�
			i*=LENper_6018;
			i+=ADDR_6018;
			MR(ADDR_128KDATABUFF,i,8);
			p8s=(u8*)ADDR_128KDATABUFF;
			if(p8s[0]!=DataType_structure)
			{
				return;
			}
			p8s+=4;
			x=p8s[1];
			if(x&0x80)
			{
				x=(p8s[2]<<8)+p8s[3];
				p8s+=4;
			}
			else
			{
				p8s+=2;
			}
			if(x>NUM_RMmax)
			{
				x=0;
			}
			UARTCtrl->NUMrmTOTAL[id]=x;//������ɼ�����
			//���ļ��ѳ���ַ�б�,ԭ�Ѳɼ������
			i=Get_FileID(PORTn);//UART�������FileID
			Comm_Ram->msFILESearchCount=0;
			filename=Get_RecordFileName(i,0,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename!=0xffffffff)
			{
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				y=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
				if(y>NUM_RMmax)
				{
					y=0;
				}
				i=UARTCtrl->NUMrmFAIL[id];//�������Ѳɼ�ʧ�ܵ��ܱ���
				if((y+i)>NUM_RMmax)
				{
					UARTCtrl->NUMrmFAIL[id]=NUM_RMmax-y;//�������Ѳɼ�ʧ�ܵ��ܱ���
				}
				UARTCtrl->NUMrmOK[id]=y;//ԭ�Ѳɼ������
				ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
				RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
				rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
				for(i=0;i<y;i++)
				{
					rmOKFAILlist->Addr0=RecordFileHead->Addr0;
					rmOKFAILlist->Addr1=RecordFileHead->Addr1;
					rmOKFAILlist->Addr2=RecordFileHead->Addr2;
					rmOKFAILlist++;
					RecordFileHead++;
				}
			}
			//����ԭ�Ѳɼ�������еķǷ���
			p8d=p8s;//ָ�����ݼ�
			y=UARTCtrl->NUMrmOK[id];//ԭ�Ѳɼ������
			if(y>=NUM_RMmax)
			{
				y=0;
			}
			NUMrm=0;//�ɼ�������еķǷ���
			rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
			while(y--)
			{
				p8s=p8d;//�ָ�p8s
				//x=���ݼ��������
				for(i=0;i<x;i++)
				{
					OADPORTn=p8s[4]|(p8s[5]<<8);
					if(rmOKFAILlist->Addr0==OADPORTn)
					{
						OADPORTn=p8s[6]|(p8s[7]<<8);
						if(rmOKFAILlist->Addr1==OADPORTn)
						{
							OADPORTn=p8s[8]|(p8s[9]<<8);
							if(rmOKFAILlist->Addr2==OADPORTn)
							{
								break;
							}
						}
					}
					OADPORTn=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					p8s+=OADPORTn;
				}
				if(i>=x)
				{//û�ҵ���ͬ��ַ
					NUMrm++;//�ɼ�������еķǷ���
				}
				rmOKFAILlist++;
			}
			UARTCtrl->NUMrmBAD[id]=NUMrm;//�ɼ�������еķǷ���
			//ɾ���Ѳɼ�ʧ�����еķǷ���
			rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
			rmOKFAILlist--;
			y=UARTCtrl->NUMrmFAIL[id];//ԭ�Ѳɼ�ʧ����
			if(y>=NUM_RMmax)
			{
				y=0;
			}
			for(i=0;i<y;i++)
			{
				x=AddrToMeterSetNo((u16*)rmOKFAILlist);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
				if(x<NUM_RMmax)
				{
					p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
					if(p8s[0]==DataType_structure)
					{
						p8d=Get_Element(p8s+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						if(p8d)
						{
							n=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
							if(n==OADPORTn)
							{
								x=Compare_SETNO_MS((u8 *)ADDR_6000_SDRAM,x,pMS);//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
								if(x)
								{
									rmOKFAILlist--;
									continue;
								}
							}
						}
					}
				}
				#if (LEN_RMokfailLIST*NUM_RMmax)>LEN_128KDATABUFF
					#error
				#endif
				MR(ADDR_128KDATABUFF,((u32)rmOKFAILlist)-((y-(i+1))*sizeof(RMokfailLIST_TypeDef)),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
				MW(ADDR_128KDATABUFF,(u32)rmOKFAILlist-((y-(i+1))*sizeof(RMokfailLIST_TypeDef))+sizeof(RMokfailLIST_TypeDef),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
				y--;
				rmOKFAILlist--;
			}
			UARTCtrl->NUMrmFAIL[id]=y;//ԭ�Ѳɼ�ʧ����
			return;
		case 4://�ϱ�����
		case 5://�ű�����
		default:
			break;
	}
	i=(TaskID>>24)&0xf;
	if(i<=2)
	{//1=��ͨ�ɼ�����,2=�¼��ɼ�����;(3=͸�����������б�)
		pMS++;//ָ���������ͺ���ֽ�
		NUMrm=0;//��ɼ�����
		OADPORTn=PORTntoOAD(PORTn);
		p8s=(u8*)ADDR_6000_SDRAM;
		for(i=0;i<NUMmax_6000;i++)
		{
			if(p8s[0]==DataType_structure)
			{
				p8d=Get_Element(p8s+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8d)
				{
					x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
					if(x==OADPORTn)
					{
						x=Compare_SETNO_MS((u8*)ADDR_6000_SDRAM,i,pMS);//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
						if(x)
						{
							NUMrm++;//��ɼ�����
						}
					}
				}
			}
			p8s+=LENper_6000;
		}
		UARTCtrl->NUMrmTOTAL[id]=NUMrm;//2byte������ɼ�����
		//���ļ��ѳ���ַ�б�,ԭ�Ѳɼ������
		Comm_Ram->msFILESearchCount=0;
		i=(TaskID>>24)&0xf;
		if(i==1)
		{//1=��ͨ�ɼ�����
			DateSaveTI(PORTn,UARTCtrl->date_time_s);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
			i=Get_FileID(PORTn);//UART�������FileID
			filename=Get_RecordFileName(i,0xffffffff,UARTCtrl->date_time_s);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
		}
		else
		{//2=�¼��ɼ�����
			DateSaveTI(PORTn,UARTCtrl->date_time_s);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
			i=Get_FileID(PORTn);//UART�������FileID
			filename=Get_RecordFileName(i,0,UARTCtrl->date_time_s);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
		}
		if(filename!=0xffffffff)
		{
			if(filename<FILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			y=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
			if(y>NUM_RMmax)
			{
				y=0;
			}
			i=UARTCtrl->NUMrmFAIL[id];//�������Ѳɼ�ʧ�ܵ��ܱ���
			if((y+i)>NUM_RMmax)
			{
				UARTCtrl->NUMrmFAIL[id]=NUM_RMmax-y;//�������Ѳɼ�ʧ�ܵ��ܱ���
			}
			UARTCtrl->NUMrmOK[id]=y;//ԭ�Ѳɼ������
			ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
			rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
			for(i=0;i<y;i++)
			{
				rmOKFAILlist->Addr0=RecordFileHead->Addr0;
				rmOKFAILlist->Addr1=RecordFileHead->Addr1;
				rmOKFAILlist->Addr2=RecordFileHead->Addr2;
				rmOKFAILlist++;
				RecordFileHead++;
			}
		}
		NUMrm=0;//�ɼ�������еķǷ���
		//����ԭ�Ѳɼ�������еķǷ���
		y=UARTCtrl->NUMrmOK[id];//ԭ�Ѳɼ������
		if(y>NUM_RMmax)
		{
			y=0;
		}
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
		for(i=0;i<y;i++)
		{
			x=AddrToMeterSetNo((u16*)rmOKFAILlist);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
			if(x<NUM_RMmax)
			{
				p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
				if(p8s[0]==DataType_structure)
				{
					p8d=Get_Element(p8s+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8d)
					{
						n=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
						if(n==OADPORTn)
						{
							x=Compare_SETNO_MS((u8 *)ADDR_6000_SDRAM,x,pMS);//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
							if(x)
							{
								rmOKFAILlist++;
								continue;
							}
						}
					}
				}
			}
			rmOKFAILlist++;
			NUMrm++;//�ɼ�������еķǷ���
		}
		UARTCtrl->NUMrmBAD[id]=NUMrm;//�Ƿ���
		//ɾ���Ѳɼ�ʧ�����еķǷ���
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
		rmOKFAILlist--;
		y=UARTCtrl->NUMrmFAIL[id];//ԭ�Ѳɼ�ʧ����
		if(y>=NUM_RMmax)
		{
			y=0;
		}
		for(i=0;i<y;i++)
		{
			x=AddrToMeterSetNo((u16*)rmOKFAILlist);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
			if(x<NUM_RMmax)
			{
				p8s=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
				if(p8s[0]==DataType_structure)
				{
					p8d=Get_Element(p8s+5,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8d)
					{
						n=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
						if(n==OADPORTn)
						{
							x=Compare_SETNO_MS((u8 *)ADDR_6000_SDRAM,x,pMS);//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
							if(x)
							{
								rmOKFAILlist--;
								continue;
							}
						}
					}
				}
			}
			#if (LEN_RMokfailLIST*NUM_RMmax)>LEN_128KDATABUFF
				#error
			#endif
			MR(ADDR_128KDATABUFF,((u32)rmOKFAILlist)-((y-(i+1))*sizeof(RMokfailLIST_TypeDef)),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
			MW(ADDR_128KDATABUFF,(u32)rmOKFAILlist-((y-(i+1))*sizeof(RMokfailLIST_TypeDef))+sizeof(RMokfailLIST_TypeDef),(y-(i+1))*sizeof(RMokfailLIST_TypeDef));
			y--;
			rmOKFAILlist--;
		}
		UARTCtrl->NUMrmFAIL[id]=y;//ԭ�Ѳɼ�ʧ����
	}
	
//FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע
//FileID�ɼ����� 3=��ʱ�����ɼ�(��������)
//TaskID�ɼ����� 4=����
	i=Get_FileID(PORTn);//UART�������FileID
	if((((i>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
	{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
		UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
	}
	
}

void RM_OK_FAIL(u32 PORTn,u32 OK)//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�,2=�ɹ�������Ϊ��
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 id;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	EventSour_typedef* EventSour;
	RMokfailLIST_TypeDef *rmOKFAILlist;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return;
	}
	id>>=8;
	id&=0xff;
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return;
	}
	Addr0=UARTCtrl->RMAddr[0];
	Addr1=UARTCtrl->RMAddr[1];
	Addr2=UARTCtrl->RMAddr[2];
	if(OK==1)
	{//�ɹ�
		n=UARTCtrl->NUMrmOK[id];
		if(n>=NUM_RMmax)
		{
			return;
		}
		//��ԭ�Ƿ�ɹ���¼
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
		for(i=0;i<n;i++)
		{
			if(rmOKFAILlist->Addr0==Addr0)
			{
				if(rmOKFAILlist->Addr1==Addr1)
				{
					if(rmOKFAILlist->Addr2==Addr2)
					{
						break;
					}
				}
			}
			rmOKFAILlist++;
		}
		if(i>=n)
		{//û
			rmOKFAILlist->Addr0=Addr0;
			rmOKFAILlist->Addr1=Addr1;
			rmOKFAILlist->Addr2=Addr2;
			n++;
			UARTCtrl->NUMrmOK[id]=n;
			//��ԭ�Ƿ�ʧ�ܼ�¼
			m=UARTCtrl->NUMrmFAIL[id];
			if((n+m)>NUM_RMmax)
			{
				m=NUM_RMmax-n;//�������Ѳɼ�ʧ�ܵ��ܱ���
			}
			rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
			rmOKFAILlist--;
			for(i=0;i<m;i++)
			{
				if(rmOKFAILlist->Addr0==Addr0)
				{
					if(rmOKFAILlist->Addr1==Addr1)
					{
						if(rmOKFAILlist->Addr2==Addr2)
						{
							break;
						}
					}
				}
				rmOKFAILlist--;
			}
			if(i<m)
			{//��
				#if (LEN_RMokfailLIST*NUM_RMmax)>LEN_128KDATABUFF
					#error
				#endif
				MR(ADDR_128KDATABUFF,((u32)rmOKFAILlist)-((m-(i+1))*sizeof(RMokfailLIST_TypeDef)),(m-(i+1))*sizeof(RMokfailLIST_TypeDef));
				MW(ADDR_128KDATABUFF,(u32)rmOKFAILlist-((m-(i+1))*sizeof(RMokfailLIST_TypeDef))+sizeof(RMokfailLIST_TypeDef),(m-(i+1))*sizeof(RMokfailLIST_TypeDef));
				m--;
				UARTCtrl->NUMrmFAIL[id]=m;
			}
		}
		//�ն˳���ʧ���¼�
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
		{
			i=MRR(ADDR_310F_6+5,1);//�����ɼ������
			if(i==id)
			{
				EventSour=GET_EventSour((u16*)UARTCtrl->RMAddr);//�õ��¼�ԭֵ����ָ�룬���:�����ַ, ����:����ָ��,0=����
				if(EventSour)
				{
					EventSour->Re=0;//����ʧ��(310F)�����ִ�
					if((EventSour->Occur&1)==1)//b0=����ʧ��(310F)
					{
						EventSour->Occur&=~1;//b0=����ʧ��(310F)
						p8=(u8*)ADDR_128KDATABUFF+(64*1024);
						p8[0]=0;//�¼�����ǰ�����й��ܵ���=��
						p8[1024]=0;//�¼������������й��ܵ���=��
						p8+=512;
						p8[0]=DataType_TSA;//����Դ
						p8[1]=7;
						p8[2]=5;
						p8[3]=Addr0;
						p8[4]=Addr0>>8;
						p8[5]=Addr1;
						p8[6]=Addr1>>8;
						p8[7]=Addr2;
						p8[8]=Addr2>>8;
						Event_OAD(0x310F0200,p8,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					}
				}
			}
		}
		//RS485�豸�����¼���¼�ָ�
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
		{
			if(UARTCtrl->EVENT_FAIL==1)//�豸�����¼���¼(485���ز�ͨ��),0=�ջ�ָ�,1=����
			{
				UARTCtrl->EVENT_FAIL=0;//�豸�����¼���¼(485���ز�ͨ��),0=�ջ�ָ�,1=����
				p8=(u8*)ADDR_128KDATABUFF+(64*1024);
				p8[0]=0;//�¼�����ǰ�����й��ܵ���=��
				p8[1024]=0;//�¼������������й��ܵ���=��
				p8+=512;
				p8[0]=DataType_enum;//����Դ
				p8[1]=3;//485�������    ��3��
				Event_OAD(0x310A0200,p8,0,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
			}
		}
	}
	else
	{//ʧ�ܻ������
		//��ԭ�Ƿ�ɹ���¼
		n=UARTCtrl->NUMrmOK[id];
		if(n>=NUM_RMmax)
		{
			return;
		}
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
		for(i=0;i<n;i++)
		{
			if(rmOKFAILlist->Addr0==Addr0)
			{
				if(rmOKFAILlist->Addr1==Addr1)
				{
					if(rmOKFAILlist->Addr2==Addr2)
					{
						return;
					}
				}
			}
			rmOKFAILlist++;
		}
		//��ԭ�Ƿ�ʧ�ܼ�¼
		m=UARTCtrl->NUMrmFAIL[id];
		if((n+m)>NUM_RMmax)
		{
			m=NUM_RMmax-n;//�������Ѳɼ�ʧ�ܵ��ܱ���
		}
		rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
		rmOKFAILlist--;
		for(i=0;i<m;i++)
		{
			if(rmOKFAILlist->Addr0==Addr0)
			{
				if(rmOKFAILlist->Addr1==Addr1)
				{
					if(rmOKFAILlist->Addr2==Addr2)
					{
						rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;
						rmOKFAILlist->SpaceData=OK;//0=ʧ��,1=,2=��������������Ϊ�ձ�־ʧ��(̨����Բɼ���ؼ�Ϊ����ʧ��)
						break;
					}
				}
			}
			rmOKFAILlist--;
		}
		if(i>=m)
		{//û
			if((n+m)>=NUM_RMmax)
			{
				return;
			}
			rmOKFAILlist->Addr0=Addr0;
			rmOKFAILlist->Addr1=Addr1;
			rmOKFAILlist->Addr2=Addr2;
			rmOKFAILlist->ReRMCount=UARTCtrl->ReRMCount;
			rmOKFAILlist->SpaceData=OK;//0=ʧ��,1=,2=��������������Ϊ�ձ�־ʧ��(̨����Բɼ���ؼ�Ϊ����ʧ��)
			m++;
			UARTCtrl->NUMrmFAIL[id]=m;
			//�ն˳���ʧ���¼�
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{
				i=MRR(ADDR_310F_6+3,3);//�����ִ�,�����ɼ������
				if((i>>16)==id)
				{
					i&=0xff;//�����е������ִ�
					EventSour=GET_EventSour((u16*)UARTCtrl->RMAddr);//�õ��¼�ԭֵ����ָ�룬���:�����ַ, ����:����ָ��,0=����
					if(EventSour)
					{
						if(EventSour->Re>=i)
						{
							if((EventSour->Occur&1)==0)//b0=����ʧ��(310F)
							{
								EventSour->Occur|=1;//b0=����ʧ��(310F)
								p8=(u8*)ADDR_128KDATABUFF+(64*1024);
								p8[0]=0;//�¼�����ǰ�����й��ܵ���=��
								p8[1024]=0;//�¼������������й��ܵ���=��
								p8+=512;
								p8[0]=DataType_TSA;//����Դ
								p8[1]=7;
								p8[2]=5;
								p8[3]=Addr0;
								p8[4]=Addr0>>8;
								p8[5]=Addr1;
								p8[6]=Addr1>>8;
								p8[7]=Addr2;
								p8[8]=Addr2>>8;
								Event_OAD(0x310F0200,p8,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
							}
						}
						if(EventSour->Re!=0xff)//����ʧ��(310F)�����ִ�
						{
							EventSour->Re++;//����ʧ��(310F)�����ִ�
						}
					}
				}
			}
		}
		//RS485�豸�����¼���¼����
		if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
		{
			if(UARTCtrl->EVENT_FAIL==0)//�豸�����¼���¼(485���ز�ͨ��),0=�ջ�ָ�,1=����
			{
			#if LEN_DATABUFF<NUM_RMmax
				#error
			#endif
				MC(0,ADDR_DATABUFF,NUM_RMmax);
				p8=(u8*)ADDR_DATABUFF;
				n=0;//����ʧ�ܱ���
				for(id=1;id<RMTASKmax;id++)
				{
					//ͳ��ʧ�ܼ�¼�ĸ�����
					m=UARTCtrl->NUMrmFAIL[id];
					if(m>NUM_RMmax)
					{
						m=NUM_RMmax;
					}
					rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
					rmOKFAILlist--;
					for(i=0;i<m;i++)
					{
						x=AddrToMeterSetNo((u16*)rmOKFAILlist);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
						if(x<NUM_RMmax)
						{
							if(p8[x]==0)
							{
								p8[x]=1;
								n++;//����ʧ�ܱ���+1
							}
						}
						rmOKFAILlist--;
					}
				}
				if(n>=UARTCtrl->NUM_SetMeter)//���˿��ڵ��������õ��Ǳ�����
				{
					UARTCtrl->EVENT_FAIL=1;//�豸�����¼���¼(485���ز�ͨ��),0=�ջ�ָ�,1=����
					p8=(u8*)ADDR_128KDATABUFF+(64*1024);
					p8[0]=0;//�¼�����ǰ�����й��ܵ���=��
					p8[1024]=0;//�¼������������й��ܵ���=��
					p8+=512;
					p8[0]=DataType_enum;//����Դ
					p8[1]=3;//485�������    ��3��
					Event_OAD(0x310A0200,p8,1,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			}
		}
	}
}

void RM_ADDR_Display(u32 PORTn)//�����ַ��ʾ
{
	u32 i;
	u32 x;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8=(u8*)ADDR_DATABUFF;
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{
		CopyString((u8 *)"485     ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		if(PORTn==RS485_1PORT)
		{
			i=1;
		}
		else
		{
			i=2;
		}
		p8[3]=i;
	}
	else
	{
		if(PORTn==ACSAMPLEPORT)
		{//����
			CopyString((u8 *)"����     ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		}
		else
		{
			CopyString((u8 *)"�ز�     ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		}
	}
//�㲥Уʱ����
	if((UARTCtrl->BroadcastTimeTask&0x7f)>=2)//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
	{//����ִ�й㲥Уʱ
		CopyString((u8 *)"�㲥Уʱ",(u8*)ADDR_DATABUFF+6);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		p8[14]=0;
		WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
		return;
	}
//�����	
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;
	i=hex_bcd(i);
	if(i<0x10)
	{
		p8[5]=i+0x30;
	}
	else
	{
		if(i<0x100)
		{
			p8[5]=(i>>4)+0x30;
			p8[6]=(i&0xf)+0x30;
		}
		else
		{
			p8[5]=((i>>8)&0xf)+0x30;
			p8[6]=((i>>4)&0xf)+0x30;
			p8[7]=(i&0xf)+0x30;
		}
	}
//���ܱ������ϱ�����
	if(UARTCtrl->AutoReportTask>=2)//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
	{
		CopyString((u8 *)"�ϱ�",(u8*)ADDR_DATABUFF+4);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
	}
//12λ��ַ
	p8+=8;
	for(i=0;i<3;i++)
	{
		x=UARTCtrl->RMAddr[i];
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
		x>>=8;
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
	}
	p8[0]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
}

void PROXY_ADDR_Display(u32 PORTn)//�����ַ��ʾ
{
	u32 i;
	u32 x;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlProxy;
	
	if(PORTn==ACSAMPLEPORT)
	{//����
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8=(u8*)ADDR_DATABUFF;
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{
		CopyString((u8 *)"485  ת  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		if(PORTn==RS485_1PORT)
		{
			i=1;
		}
		else
		{
			i=2;
		}
		p8[3]=i;
	}
	else
	{
		CopyString((u8 *)"�ز� ת  ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
	}
	UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(UARTCtrl->ProxyPORTn);
	p8+=8;
	for(i=0;i<3;i++)
	{
		x=UARTCtrlProxy->RMAddr[i];
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
		x>>=8;
		if(((x>>4)&0xf)<10)
		{
			p8[0]=((x>>4)&0xf)+0x30;
		}
		else
		{	
			p8[0]=((x>>4)&0xf)+55;
		}
		p8++;
		if((x&0xf)<10)
		{
			p8[0]=(x&0xf)+0x30;
		}
		else
		{
			p8[0]=(x&0xf)+55;
		}
		p8++;
	}
	p8[0]=0;
	WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
}



void RM_NUM_Display(u32 PORTn)//����������ʾ
{
	u32 i;
	u32 x;
	u32 id;
	u8* p8;
	u32 TaskID;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//id=UARTCtrl->LastTaskID;//����ϴ���Ч�ɼ�����ID,������ʾ��
	id=UARTCtrl->TaskID;
	id>>=8;
	id&=0xff;
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{
		//CopyString((u8 *)"485x____��____��____",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		CopyString((u8 *)"485x____ �ɹ�____",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		p8=(u8*)(ADDR_DATABUFF);
		if(PORTn==RS485_1PORT)
		{
			i=1;
		}
		else
		{
			i=2;
		}
		p8[3]=i;
	}
	else
	{
		if(PORTn==ACSAMPLEPORT)
		{//����
			CopyString((u8 *)"����____ �ɹ�____",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		}
		else
		{
			//CopyString((u8 *)"�ز�____��____��____",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			CopyString((u8 *)"�ز�____ �ɹ�____",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
		}
	}
	//����
	p8=(u8*)(ADDR_DATABUFF+4+3);
	p8[0]='0';
	i=UARTCtrl->NUMrmTOTAL[id];
	i=hex_bcd(i);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	//�ɹ���
	p8=(u8*)(ADDR_DATABUFF+13+3);
	p8[0]='0';
	TaskID=UARTCtrl->TaskID;
	i=Get_FileID(PORTn);//UART�������FileID
	if((((i>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
	{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
		//UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
		i=UARTCtrl->NUMrmOK[id];
	}
	else
	{
		i=UARTCtrl->NUMrmOK[id];
		x=UARTCtrl->NUMrmBAD[id];
		if(i>=x)
		{
			i-=x;
		}
		else
		{
			i=0;
		}
	}
	i=hex_bcd(i);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	p8=(u8*)(ADDR_DATABUFF+17);
	p8[0]=0;
	//ʧ����
/*
	p8=(u8*)(ADDR_DATABUFF+16+3);
	p8[0]='0';
	p8[1]=0;
	i=UARTCtrl->NUMrmFAIL[id];
	i=hex_bcd(i);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
*/
	if(PORTn==RS485_4PORT)
	{//�ز���
		WarningOccur(100,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
	}
	else
	{
		WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
	}
}


void RM_Monitor_Init(u32 ID)//�ɼ������س�ʼ��;���:ID=�����
{
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl_1;
	UARTCtrl_TypeDef *UARTCtrl_2;
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	UARTCtrl_TypeDef *UARTCtrl_4;
#endif
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	UARTCtrl_1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl_2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	UARTCtrl_4=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
#endif
	if(UARTCtrl_1->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	{
		return;
	}
	if(UARTCtrl_2->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	{
		return;
	}
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(UARTCtrl_4->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	{
		return;
	}
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		if(UARTCtrl_4->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
		{
			return;
		}
	}
#endif
	UARTCtrl_1->TaskTime[ID].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	UARTCtrl_2->TaskTime[ID].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	UARTCtrl_4->TaskTime[ID].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
#endif
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
//0 �ɼ������ص�Ԫ��=structure
	p8[0]=DataType_structure;
	p8[1]=8;
//{
//2 ����ID       unsigned
	p8[2]=DataType_unsigned;
	p8[3]=ID;
//4 ����ִ��״̬  enum
//{
//	δִ�У�0��
//	ִ���У�1��
//	��ִ�У�2��
//}��
	p8[4]=DataType_enum;
	p8[5]=0;
//6 ����ִ�п�ʼʱ��   date_time_s��//�ø��˿�ʱ��ֵС��
	p8[6]=DataType_date_time_s;
	p8[7]=0xff;
	p8[8]=0xff;
	p8[9]=0xff;
	p8[10]=0xff;
	p8[11]=0xff;
	p8[12]=0xff;
	p8[13]=0xff;
//14 ����ִ�н���ʱ��   date_time_s��//�ø��˿�ʱ��ֵ���
	p8[14]=DataType_date_time_s;
	p8[15]=0xff;
	p8[16]=0xff;
	p8[17]=0xff;
	p8[18]=0xff;
	p8[19]=0xff;
	p8[20]=0xff;
	p8[21]=0xff;
//22 �ɼ�������         long-unsigned��
	p8[22]=DataType_long_unsigned;
	p8[23]=0;
	p8[24]=0;
//25 �ɼ��ɹ�����       long-unsigned��
	p8[25]=DataType_long_unsigned;
	p8[26]=0;
	p8[27]=0;
//28 �ѷ��ͱ�������     long-unsigned��
	p8[28]=DataType_long_unsigned;
	p8[29]=0;
	p8[30]=0;
//31 �ѽ��ձ�������     long-unsigned
	p8[31]=DataType_long_unsigned;
	p8[32]=0;
	p8[33]=0;
//}
}

void RM_Monitor_Init_PORTn(u32 PORTn,u32 ID)//�ɼ������س�ʼ��(���˿ں�);���:PORTn=�˿ں�,ID=�����
{
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl_1;
	UARTCtrl_TypeDef *UARTCtrl_2;
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	UARTCtrl_TypeDef *UARTCtrl_4;
#endif
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	UARTCtrl_1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	UARTCtrl_2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	UARTCtrl_4=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
#endif
	if(UARTCtrl_1->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	{
		return;
	}
	if(UARTCtrl_2->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	{
		return;
	}
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(UARTCtrl_4->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	{
		return;
	}
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		if(UARTCtrl_4->TaskTime[ID].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
		{
			return;
		}
	}
#endif
	if(PORTn==RS485_1PORT)
	{
		UARTCtrl_1->TaskTime[ID].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	}
	if(PORTn==RS485_2PORT)
	{
		UARTCtrl_2->TaskTime[ID].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	}
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(PORTn==RS485_4PORT)
	{
		UARTCtrl_4->TaskTime[ID].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
	}
#endif
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
//0 �ɼ������ص�Ԫ��=structure
	p8[0]=DataType_structure;
	p8[1]=8;
//{
//2 ����ID       unsigned
	p8[2]=DataType_unsigned;
	p8[3]=ID;
//4 ����ִ��״̬  enum
//{
//	δִ�У�0��
//	ִ���У�1��
//	��ִ�У�2��
//}��
	p8[4]=DataType_enum;
	p8[5]=0;
//6 ����ִ�п�ʼʱ��   date_time_s��//�ø��˿�ʱ��ֵС��
	p8[6]=DataType_date_time_s;
	p8[7]=0xff;
	p8[8]=0xff;
	p8[9]=0xff;
	p8[10]=0xff;
	p8[11]=0xff;
	p8[12]=0xff;
	p8[13]=0xff;
//14 ����ִ�н���ʱ��   date_time_s��//�ø��˿�ʱ��ֵ���
	p8[14]=DataType_date_time_s;
	p8[15]=0xff;
	p8[16]=0xff;
	p8[17]=0xff;
	p8[18]=0xff;
	p8[19]=0xff;
	p8[20]=0xff;
	p8[21]=0xff;
//22 �ɼ�������         long-unsigned��
	p8[22]=DataType_long_unsigned;
	p8[23]=0;
	p8[24]=0;
//25 �ɼ��ɹ�����       long-unsigned��
	p8[25]=DataType_long_unsigned;
	p8[26]=0;
	p8[27]=0;
//28 �ѷ��ͱ�������     long-unsigned��
	p8[28]=DataType_long_unsigned;
	p8[29]=0;
	p8[30]=0;
//31 �ѽ��ձ�������     long-unsigned
	p8[31]=DataType_long_unsigned;
	p8[32]=0;
	p8[33]=0;
//}
}



void RM_Monitor(u32 ID,u32 NUM_Tx,u32 NUM_Rx)//�ɼ�������;���:ID=�����,NUM_Tx=��������,NUM_Rx=��������
{
	u32 i;
	u32 x;
	u32 y;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl_1;
	UARTCtrl_TypeDef *UARTCtrl_2;
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	UARTCtrl_TypeDef *UARTCtrl_4;
#endif
	u32 NUM_OK;
	u32 SourTaskID;
	u32 TaskID;
	u32 FileID;
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
	i=MRR(ADDR_6012_2_SDRAM+(ID*LENper_6012_2),1);
	if(i!=DataType_structure)
	{
		p8[0]=0;//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
		return;
	}
	UARTCtrl_1=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
	if(UARTCtrl_1->Function==0)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
	{
		UARTCtrl_1->TaskTime[ID].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	}
	UARTCtrl_2=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
	if(UARTCtrl_2->Function==0)//�ڹ��� 0=�ն�ά����,!=0�ն˳����
	{
		UARTCtrl_2->TaskTime[ID].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	}
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	UARTCtrl_4=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
#endif
//0 �ɼ������ص�Ԫ��=structure
	p8[0]=DataType_structure;
	p8[1]=8;
//{
//2 ����ID       unsigned
	p8[2]=DataType_unsigned;
	p8[3]=ID;
//4 ����ִ��״̬  enum
//{
//	δִ�У�0��
//	ִ���У�1��
//	��ִ�У�2��
//}��
	p8[4]=DataType_enum;
	p8[5]=0;
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1)||(UARTCtrl_4->TaskTime[ID].Run==1))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	{
		p8[5]=1;
	}
	else
	{
		if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2)&&(UARTCtrl_4->TaskTime[ID].Run==2))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
		{
			p8[5]=2;
		}
	}
#endif
#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	{
		p8[5]=1;
	}
	else
	{
		if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
		{
			p8[5]=2;
		}
	}
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1)||(UARTCtrl_4->TaskTime[ID].Run==1))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
		{
			p8[5]=1;
		}
		else
		{
			if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2)&&(UARTCtrl_4->TaskTime[ID].Run==2))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
			{
				p8[5]=2;
			}
		}
	}
	else
	{
		if((UARTCtrl_1->TaskTime[ID].Run==1)||(UARTCtrl_2->TaskTime[ID].Run==1))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
		{
			p8[5]=1;
		}
		else
		{
			if((UARTCtrl_1->TaskTime[ID].Run==2)&&(UARTCtrl_2->TaskTime[ID].Run==2))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
			{
				p8[5]=2;
			}
		}
	}
#endif
//6 ����ִ�п�ʼʱ��   date_time_s��//�ø��˿�ʱ��ֵС��
	p8[6]=DataType_date_time_s;
	p8[7]=0xff;
	p8[8]=0xff;
	p8[9]=0xff;
	p8[10]=0xff;
	p8[11]=0xff;
	p8[12]=0xff;
	p8[13]=0xff;
//	if(UARTCtrl_1->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i=UARTCtrl_1->TaskTime[ID].StartTime[2];
		if((i!=0)&&(i!=0xff))
		{
			MR((u32)p8+7,(u32)&UARTCtrl_1->TaskTime[ID].StartTime,7);
		}
//	}
//	if(UARTCtrl_2->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i=UARTCtrl_2->TaskTime[ID].StartTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+7,(u8 *)&UARTCtrl_2->TaskTime[ID].StartTime,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+7,(u32)&UARTCtrl_2->TaskTime[ID].StartTime,7);
			}
		}
//	}
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//	if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i=UARTCtrl_4->TaskTime[ID].StartTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+7,(u8 *)&UARTCtrl_4->TaskTime[ID].StartTime,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+7,(u32)&UARTCtrl_4->TaskTime[ID].StartTime,7);
			}
		}
//	}
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
//		if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//		{
			i=UARTCtrl_4->TaskTime[ID].StartTime[2];
			if((i!=0)&&(i!=0xff))
			{
				i=Compare_DL698DataString(p8+7,(u8 *)&UARTCtrl_4->TaskTime[ID].StartTime,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(i==2)
				{//p1<p2
					MR((u32)p8+7,(u32)&UARTCtrl_4->TaskTime[ID].StartTime,7);
				}
			}
//		}
	}
#endif
//14 ����ִ�н���ʱ��   date_time_s��//�ø��˿�ʱ��ֵ���
	p8[14]=DataType_date_time_s;
	p8[15]=0;
	p8[16]=0;
	p8[17]=0;
	p8[18]=0;
	p8[19]=0;
	p8[20]=0;
	p8[21]=0;
//	if(UARTCtrl_1->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i=UARTCtrl_1->TaskTime[ID].EndTime[2];
		if((i!=0)&&(i!=0xff))
		{
			MR((u32)p8+15,(u32)&UARTCtrl_1->TaskTime[ID].EndTime,7);
		}
//	}
//	if(UARTCtrl_2->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i=UARTCtrl_2->TaskTime[ID].EndTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+15,(u8 *)&UARTCtrl_2->TaskTime[ID].EndTime,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+15,(u32)&UARTCtrl_2->TaskTime[ID].EndTime,7);
			}
		}
//	}
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//	if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i=UARTCtrl_4->TaskTime[ID].EndTime[2];
		if((i!=0)&&(i!=0xff))
		{
			i=Compare_DL698DataString(p8+15,(u8 *)&UARTCtrl_4->TaskTime[ID].EndTime,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i==2)
			{//p1<p2
				MR((u32)p8+15,(u32)&UARTCtrl_4->TaskTime[ID].EndTime,7);
			}
		}
//	}
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
//		if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//		{
			i=UARTCtrl_4->TaskTime[ID].EndTime[2];
			if((i!=0)&&(i!=0xff))
			{
				i=Compare_DL698DataString(p8+15,(u8 *)&UARTCtrl_4->TaskTime[ID].EndTime,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(i==2)
				{//p1<p2
					MR((u32)p8+15,(u32)&UARTCtrl_4->TaskTime[ID].EndTime,7);
				}
			}
//		}
	}
#endif
//22 �ɼ�������         long-unsigned��
	p8[22]=DataType_long_unsigned;
//	p8[23]=0;
//	p8[24]=0;
	i=0;
//	if(UARTCtrl_1->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i=UARTCtrl_1->NUMrmTOTAL[ID];
//	}
//	if(UARTCtrl_2->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
//	{
		i+=UARTCtrl_2->NUMrmTOTAL[ID];
//	}
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
//	if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
//	{
		i+=UARTCtrl_4->NUMrmTOTAL[ID];
//	}
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��
		{
			i+=UARTCtrl_4->NUMrmTOTAL[ID];
		}
	}
#endif
	p8[23]=i>>8;
	p8[24]=i;
//25 �ɼ��ɹ�����       long-unsigned��
	p8[25]=DataType_long_unsigned;
//	p8[26]=0;
//	p8[27]=0;
	i=0;
	if(UARTCtrl_1->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	{
		SourTaskID=UARTCtrl_1->TaskID;
		TaskID=IDtoTaskID(ID);//���:�����id;���ڣ�TaskID
		UARTCtrl_1->TaskID=TaskID;
		FileID=Get_FileID(RS485_1PORT);//UART�������FileID
		UARTCtrl_1->TaskID=SourTaskID;
		if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
		{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
			//UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
			x=UARTCtrl_1->NUMrmOK[ID];
		}
		else
		{
			x=UARTCtrl_1->NUMrmOK[ID];
			y=UARTCtrl_1->NUMrmBAD[ID];
			if(x>=y)
			{
				x-=y;
			}
			else
			{
				x=0;
			}
		}
		i=x;
	}
	if(UARTCtrl_2->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	{
		SourTaskID=UARTCtrl_2->TaskID;
		TaskID=IDtoTaskID(ID);//���:�����id;���ڣ�TaskID
		UARTCtrl_2->TaskID=TaskID;
		FileID=Get_FileID(RS485_2PORT);//UART�������FileID
		UARTCtrl_2->TaskID=SourTaskID;
		if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
		{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
			//UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
			x=UARTCtrl_2->NUMrmOK[ID];
		}
		else
		{
			x=UARTCtrl_2->NUMrmOK[ID];
			y=UARTCtrl_2->NUMrmBAD[ID];
			if(x>=y)
			{
				x-=y;
			}
			else
			{
				x=0;
			}
		}
		i+=x;
	}
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	{
		SourTaskID=UARTCtrl_4->TaskID;
		TaskID=IDtoTaskID(ID);//���:�����id;���ڣ�TaskID
		UARTCtrl_4->TaskID=TaskID;
		FileID=Get_FileID(RS485_4PORT);//UART�������FileID
		UARTCtrl_4->TaskID=SourTaskID;
		if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
		{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
			//UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
			x=UARTCtrl_4->NUMrmOK[ID];
		}
		else
		{
			x=UARTCtrl_4->NUMrmOK[ID];
			y=UARTCtrl_4->NUMrmBAD[ID];
			if(x>=y)
			{
				x-=y;
			}
			else
			{
				x=0;
			}
		}
		i+=x;
	}
#endif
#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		if(UARTCtrl_4->TaskTime[ID].Run)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
		{
			SourTaskID=UARTCtrl_4->TaskID;
			TaskID=IDtoTaskID(ID);//���:�����id;���ڣ�TaskID
			UARTCtrl_4->TaskID=TaskID;
			FileID=Get_FileID(RS485_4PORT);//UART�������FileID
			UARTCtrl_4->TaskID=SourTaskID;
			if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
			{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
				//UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
				x=UARTCtrl_4->NUMrmOK[ID];
			}
			else
			{
				x=UARTCtrl_4->NUMrmOK[ID];
				y=UARTCtrl_4->NUMrmBAD[ID];
				if(x>=y)
				{
					x-=y;
				}
				else
				{
					x=0;
				}
			}
			i+=x;
		}
	}
#endif
	p8[26]=i>>8;
	p8[27]=i;
	NUM_OK=i;
//28 �ѷ��ͱ�������     long-unsigned��
	p8[28]=DataType_long_unsigned;
	i=p8[29]<<8;
	i|=p8[30];
	i+=NUM_Tx;
	if(i<NUM_OK)
	{//�ɼ��ɹ�����Ϊ0�������ͺͽ���֡��=0,�ڳ���ɹ����д����������������TaskID=0����Ϊ����ִ�е����˲ɼ������س�ʼ��
		//׼ȷ����=�ɹ���*����������
		i=NUM_OK;//����Ϊ�ɹ���
	}
	p8[29]=i>>8;
	p8[30]=i;
//31 �ѽ��ձ�������     long-unsigned
	p8[31]=DataType_long_unsigned;
	i=p8[32]<<8;
	i|=p8[33];
	i+=NUM_Rx;
	if(i<NUM_OK)
	{//�ɼ��ɹ�����Ϊ0�������ͺͽ���֡��=0,�ڳ���ɹ����д����������������TaskID=0����Ϊ����ִ�е����˲ɼ������س�ʼ��
		//׼ȷ����=�ɹ���*����������
		i=NUM_OK;//����Ϊ�ɹ���
	}
	p8[32]=i>>8;
	p8[33]=i;
}

void RM_Monitor_TxNUM(u32 ID,u32 NUM_Tx)//�ɼ�������;���:ID=�����,NUM_Tx=��������
{
	u32 i;
	u8* p8;
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
	i=p8[29]<<8;
	i|=p8[30];
	i+=NUM_Tx;
	p8[29]=i>>8;
	p8[30]=i;
}

void RM_Monitor_RxNUM(u32 ID,u32 NUM_Rx)//�ɼ�������;���:ID=�����,NUM_Rx=��������
{
	u32 i;
	u8* p8;
	
	if(ID>=RMTASKmax)
	{
		return;
	}
	p8=(u8*)ADDR_6034+(ID*LENper_6034);
	i=p8[32]<<8;
	i|=p8[33];
	i+=NUM_Rx;
	p8[32]=i>>8;
	p8[33]=i;
}

void RM_State_SetUpDate(void)//�ɼ�״̬,�������ʱ����(��Ӧ�������)
{
	u32 i;
	u32 x;
	u8* p8s;
	u8* p8d;

	p8s=(u8*)(ADDR_6000_SDRAM);
	p8d=(u8*)(ADDR_6032);
	for(i=0;i<NUM_RMmax;i++)
	{
		if(p8s[0])
		{
			if(p8d[0])
			{
				for(x=0;x<6;x++)
				{
					if(p8s[10+x]!=p8d[5+x])
					{
						break;
					}
				}
				if(x<6)
				{
					p8d[0]=0;//��Ч
				}
			}
		}
		else
		{
			p8d[0]=0;
		}
		p8s+=LENper_6000;
		p8d+=LENper_6032;
	}
}

void RM_State_RMUpDate(u32 PORTn,u32 OK,u32 SetNo,u32 Class,u32 Phase)//�ɼ�״̬,ÿ�γ�������:���:OK=0ʧ��=1�ɹ�,OK=1ʱClass=�м̼���,Phase=��λ
{
	u32 i;
	u32 offset;
#if ((USER/100)==9)//����		
	u32 j;
	u8* p8s1;
#endif
	u8* p8d;

	p8d=(u8*)(ADDR_6032);
	p8d+=SetNo*LENper_6032;
	if(p8d[0]==0)
	{
		if(OK==0)
		{
			return;
		}
//һ���ɼ�״̬��=structure 
		p8d[0]=DataType_structure;
		p8d[1]=8;
//{ 
//1.ͨ�ŵ�ַ              TSA�� 
		p8d[2]=DataType_TSA;
		p8d[3]=7;
		p8d[4]=5;
		MR((u32)p8d+5,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
		offset=11;
//2.�м̼���              unsigned�� 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=Class;
		offset+=2;
//3.�м̵�ַ              TSA�� 
		p8d[offset]=0;
		offset+=1;
//4.�˿�                  OAD�� 
		p8d[offset]=DataType_OAD;
		i=PORTntoOAD(PORTn);//ͨ�ſں�תΪOADֵ
		p8d[offset+1]=i>>24;
		p8d[offset+2]=(i>>16)&0xff;
		p8d[offset+3]=(i>>8)&0xff;
		p8d[offset+4]=i;
		offset+=5;
//5.���һ�βɼ��ɹ�ʱ��  date_time_s�� 
		p8d[offset]=DataType_date_time_s;
		Get_date_time_s(p8d+offset+1);//�õ�date_time_s��7�ֽ�
		offset+=8;
//6.�ɼ�ʧ�ܴ���          unsigned�� 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=0;
		offset+=2;
//7.��λ enum{δ֪��0����A �ࣨ1����B �ࣨ2����C �ࣨ3��}��
		p8d[offset]=DataType_enum;
#if ((USER/100)==9)//����	
		Data_Inverse((u32)p8d+5,6); // ת�ɱȽϲ���
		p8s1=(u8*)ADDR_AFN10F31;
		for(j=0;j<NUM_RMmax;j++)
		{
			if(Compare_DL698DataString(p8s1 + 2 + 8 * j ,p8d+5, 6) == 0)
			{
				break;
			}
		}
		Data_Inverse((u32)p8d+5,6);	// �ٴλ�ת
		Phase = 0;
		if(j < NUM_RMmax)
		{
			if(p8s1[ 2+ 8*j + 6]&0x01)
			{
				Phase = 1;
			}
			
			if(p8s1[ 2+ 8*j + 6]&0x02)
			{
				Phase = 2;
			}
	
			if(p8s1[ 2+ 8*j + 6]&0x04)
			{
				Phase = 3;
			}
		}			
#endif
		p8d[offset+1]=Phase;
		offset+=2;
//8.�����쳣 enum{������0����LN ���ף�1����������2��} 
		p8d[offset]=DataType_enum;
		p8d[offset+1]=0;
//}
		return;
	}
	if(OK==0)
	{//ʧ��
		//6.�ɼ�ʧ�ܴ���          unsigned�� 
		p8d=Get_Element(p8d,6,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(p8d)
		{
			if(p8d[1]!=0xff)
			{
				p8d[1]++;
			}
		}
	}
	else
	{//�ɹ�
		//2.�м̼���              unsigned�� 
		p8d[12]=Class;
		//5.���һ�βɼ��ɹ�ʱ��  date_time_s�� 
		p8d=Get_Element(p8d,5,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		if(p8d)
		{
			Get_date_time_s(p8d+1);//�õ�date_time_s��7�ֽ�
			//7.��λ enum{δ֪��0����A �ࣨ1����B �ࣨ2����C �ࣨ3��}��
			p8d+=10;
			p8d[1]=Phase;
		}
	}
}
/*
void RM_State_UpDate(u32 PORTn,u32 SetNo,u32 Phase,u32 Err)//�ɼ�״̬����:���:SetNo=���ú�,Phase=��λ,Err=��·�쳣
{
	u32 i;
	u32 offset;
	u8* p8d;
	
	p8d=(u8*)(ADDR_6032);
	p8d+=SetNo*LENper_6032;
	if(p8d[0]==0)
	{
//һ���ɼ�״̬��=structure 
		p8d[0]=DataType_structure;
		p8d[1]=8;
//{ 
//1.ͨ�ŵ�ַ              TSA�� 
		p8d[2]=DataType_TSA;
		p8d[3]=7;
		p8d[4]=5;
		MR((u32)p8d+5,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
		offset=11;
//2.�м̼���              unsigned�� 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=0;
		offset+=2;
//3.�м̵�ַ              TSA�� 
		p8d[offset]=0;
		offset+=1;
//4.�˿�                  OAD�� 
		p8d[offset]=DataType_OAD;
		i=PORTntoOAD(PORTn);//ͨ�ſں�תΪOADֵ
		p8d[offset+1]=i>>24;
		p8d[offset+2]=(i>>16)&0xff;
		p8d[offset+3]=(i>>8)&0xff;
		p8d[offset+4]=i;
		offset+=5;
//5.���һ�βɼ��ɹ�ʱ��  date_time_s�� 
		p8d[offset]=DataType_date_time_s;
		MC(0xff,(u32)p8d+offset+1,7);
		offset+=8;
//6.�ɼ�ʧ�ܴ���          unsigned�� 
		p8d[offset]=DataType_unsigned;
		p8d[offset+1]=0;
		offset+=2;
//7.��λ enum{δ֪��0����A �ࣨ1����B �ࣨ2����C �ࣨ3��}��
		p8d[offset]=DataType_enum;
		p8d[offset+1]=Phase;
		offset+=2;
//8.�����쳣 enum{������0����LN ���ף�1����������2��} 
		p8d[offset]=DataType_enum;
		p8d[offset+1]=Err;
//}
		return;
	}
//7.��λ enum{δ֪��0����A �ࣨ1����B �ࣨ2����C �ࣨ3��}��
	p8d=Get_Element(p8d,7,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
	if(p8d)
	{
		p8d[1]=Phase;
//8.�����쳣 enum{������0����LN ���ף�1����������2��} 
		p8d[3]=Err;
	}
}

void RM_State_ReadRouterAllUpDate(u32 PORTn)//�ɼ�״̬,��ȫ��·����Ϣ����
{
	u32 i;
	u32 n;
	u32 StartNo;
	u32 SetNo;
	u32 Phase;
	u32 Err;
	u32 NUMmax;
	u8* p8;
	
	StartNo=0;//��ʼ���
	while(1)
	{
		UART_3762_AFN03F101(PORTn,StartNo,15,(u8*)ADDR_128KDATABUFF);//(����)��ѯ�ڵ�̨����Ϣ(ֻ��ͣ·��,û�ָ�);���:StartNo=�ڵ���ʼ�����֡��0��ʼ,Num=�ڵ�����(���֧��15��),p8Buff=���ݷ��û���
//̨���ڵ�������	BIN	2
//�����ϱ��ڵ�����n(n<=15)	BIN	1
//�ڵ�1��ַ	BCD	6
//b7	b6	b5-3	b2-1	b0	BS	1
//����	��·�쳣	����	̨��״̬	����		
//�ڵ�1̨����ַ	BCD	6
//��
		p8=(u8*)ADDR_128KDATABUFF;
		NUMmax=p8[0]|(p8[1]<<8);//̨���ڵ�������
		n=p8[2];//�����ϱ��ڵ�����
		p8+=3;
		for(i=0;i<n;i++)
		{
			MR(ADDR_DATABUFF,(u32)p8,6);
			Data_Inverse(ADDR_DATABUFF,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			StartNo=AddrToMeterSetNo((u16*)ADDR_DATABUFF);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
			if(StartNo<NUM_RMmax)
			{//�ҵ����
				Phase=p8[6];
				Err=Phase;
				Phase>>=3;
				Phase&=0x03;
				switch(Phase)
				{
					case 1:
						Phase=1;//��λ enum{δ֪��0����A �ࣨ1����B �ࣨ2����C �ࣨ3��}
						break;
					case 2:
						Phase=2;
						break;
					case 4:
						Phase=3;
						break;
					default:
						Phase=0;
						break;
				}
				Err>>=6;//�����쳣 enum{������0����LN ���ף�1����������2��} 
				Err&=1;
				if(Err)
				{
					Err=2;//�����쳣 enum{������0����LN ���ף�1����������2��} 
				}
				RM_State_UpDate(RS485_4PORT,SetNo,Phase,Err);//�ɼ�״̬����:���:SetNo=���ú�,Phase=��λ,Err=��·�쳣
			}
			p8+=13;
		}
		StartNo+=n;
		if(StartNo>=NUMmax)
		{
			break;
		}
	}
}
*/


u32 DateSaveTI(u32 PORTn,u8* pOUT)//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:ʱ������1=����ʼʱ��,2=��Ե���0��0��,3=...,6=;pOUT=date_time_s
{
	u32 i;
	u32 x;
	u32 id;
	u32 YYMD;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//b31-b28���ȼ�:1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��
	id=UARTCtrl->TaskID;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	switch((id>>16)&0xf)//��ͨ�ɼ������Ĵ洢ʱ��
	{
		default://(ʹ������ʼʱ��,����ʧ�ܵ�ַ�洢ʱ��Ҫ�洢ʱ��)
		case 0://δ����(ʹ������ʼʱ��)
		case 1://����ʼʱ��
			if(pOUT)
			{
				i=id;
				i>>=20;
				i&=0xf;
				switch(i)//��ͨ�ɼ������Ĳɼ�����
				{
					default:
					//case 0://�ɼ���ǰ����,
					//case 1://�ɼ��ϵ�N��,
					//case 2://������ʱ��ɼ�,
					//case 3://��ʱ�����ɼ�
						id>>=8;
						id&=0xff;//�����
						if(id<(RMTASKmax+ExtRMTASKmax))
						{
							for(i=0;i<7;i++)
							{
								pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];
							}
						}
						break;
					case 4://RetryMetering ����
						id>>=8;
						id&=0xff;//�����
						YYMD=(UARTCtrl->TaskTime[id].StdTime[0]<<24)+(UARTCtrl->TaskTime[id].StdTime[1]<<16)+(UARTCtrl->TaskTime[id].StdTime[2]<<8)+UARTCtrl->TaskTime[id].StdTime[3];
						i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						}
						pOUT[0]=YYMD>>24;
						pOUT[1]=YYMD>>16;
						pOUT[2]=YYMD>>8;
						pOUT[3]=YYMD;
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
				}
			}
			return 1;
		case 2://��Ե���0��0��
			i=(id>>8)&0xff;//�����
			i=MRR(ADDR_6012_2_SDRAM+(i*LENper_6012_2)+5,3);
			YYMD=(i&0xff00)+(i>>16);
			i&=0xff;
			switch(i)//�ɼ�Ƶ��
			{
				case 0://��      ��0����
					break;
				case 1://��      ��1����
					YYMD*=60;
					break;
				case 2://ʱ      ��2����
					YYMD*=60*60;
					break;
				case 3://��      ��3����
					YYMD*=60*60*24;
				case 4://��      ��4����
				case 5://��      ��5��
				default:
					YYMD*=60*60*24;
					break;
			}
#if((USER/100)==0||(USER/100)==15)//���Ժ����
			if(YYMD<(60*60*24))
			{
				//�ɼ����С��1��,�洢ʱ��������ʼʱ��(ע:���Ժ��������ɼ��¼��ȼ����������Ե���0��0�֣�
				if(pOUT)
				{
					id=(UARTCtrl->TaskID>>8)&0xff;
					if(id<(RMTASKmax+ExtRMTASKmax))
					{
						for(i=0;i<7;i++)
						{
							pOUT[i]=UARTCtrl->TaskTime[id].StartTime[i];
						}
					}
				}
				return 1;
			}
#endif
			if(pOUT)
			{
				i=UARTCtrl->TaskID;
				i>>=20;
				i&=0xf;
				switch(i)//��ͨ�ɼ������Ĳɼ�����
				{
					default:
					//case 0://�ɼ���ǰ����,
					//case 1://�ɼ��ϵ�N��,
					//case 2://������ʱ��ɼ�,
					//case 3://��ʱ�����ɼ�
						for(i=0;i<4;i++)
						{
							pOUT[i]=Comm_Ram->DL698YMDHMS[i];
						}
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
					case 4://RetryMetering ����
						YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
						i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						}
						pOUT[0]=YYMD>>24;
						pOUT[1]=YYMD>>16;
						pOUT[2]=YYMD>>8;
						pOUT[3]=YYMD;
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
				}
			}
			return 2;
		case 3://�������23��59��
			if(pOUT)
			{
				YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
				i=UARTCtrl->TaskID;
				i>>=20;
				i&=0xf;
				switch(i)//��ͨ�ɼ������Ĳɼ�����
				{
					default:
					//case 0://�ɼ���ǰ����,
					//case 1://�ɼ��ϵ�N��,
					//case 2://������ʱ��ɼ�,
					//case 3://��ʱ�����ɼ�
						YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						break;
					case 4://RetryMetering ����
						YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						}
						break;
				}
				pOUT[0]=YYMD>>24;
				pOUT[1]=YYMD>>16;
				pOUT[2]=YYMD>>8;
				pOUT[3]=YYMD>>0;
				pOUT[4]=23;
				pOUT[5]=59;
				pOUT[6]=0;
			}
			return 3;
		case 4://�������0��0��
			if(pOUT)
			{
				YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
				i=UARTCtrl->TaskID;
				i>>=20;
				i&=0xf;
				switch(i)//��ͨ�ɼ������Ĳɼ�����
				{
					default:
					//case 0://�ɼ���ǰ����,
					//case 1://�ɼ��ϵ�N��,
					//case 2://������ʱ��ɼ�,
					//case 3://��ʱ�����ɼ�
						YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						break;
					case 4://RetryMetering ����
						YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
						while(i--)
						{
							YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
						}
						break;
				}
				pOUT[0]=YYMD>>24;
				pOUT[1]=YYMD>>16;
				pOUT[2]=YYMD>>8;
				pOUT[3]=YYMD>>0;
				pOUT[4]=0;
				pOUT[5]=0;
				pOUT[6]=0;
			}
			return 4;
		case 5://��Ե���1��0��0��
			if(pOUT)
			{
				for(i=0;i<3;i++)
				{
					pOUT[i]=Comm_Ram->DL698YMDHMS[i];
				}
				pOUT[3]=1;
				pOUT[4]=0;
				pOUT[5]=0;
				pOUT[6]=0;
			}
			return 5;
		case 6://���ݶ���ʱ��
//b31-b28���ȼ�:1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��
//b15-b8�����
//b7-b0�������
			if(pOUT)
			{
				i=UARTCtrl->TaskID&0xff;//������
				p8=(u8*)(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
				p8=Get_Element(p8,4,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				i=0;
				if(p8)
				{
					i=(p8[0]<<24)|(p8[1]<<16)|(p8[2]<<8)|p8[3];
				}
				//i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
				switch(i)
				{
					case 0x5B015002://���Ӷ���(����������)
						i=UARTCtrl->TaskID;
						i>>=20;
						i&=0xf;
						switch(i)//��ͨ�ɼ������Ĳɼ�����
						{
							default:
							case 0://�ɼ���ǰ����,
								id>>=8;
								id&=0xff;//�����
								if(id<(RMTASKmax+ExtRMTASKmax))
								{
									for(i=0;i<7;i++)
									{
										pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];//������ʼ��׼ʱ��
									}
								}
								break;
							case 1://�ɼ��ϵ�N��,
								//��1�ε���10��
								id>>=8;
								id&=0xff;//�����
								if(id>=(RMTASKmax+ExtRMTASKmax))
								{
									break;
								}
								for(i=0;i<7;i++)
								{
									pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];//����ʼ��׼ʱ��
								}
								p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
								x=(p8[6]<<8)+p8[7];//ȡƵ��ֵ
								if(x==0)
								{
									x=1;//��Сֵ=1
								}
								switch(p8[5])//ִ��Ƶ��   TI
								{
									case 0://��      ��0����
										break;
									case 1://��      ��1����
										x*=60;
										break;
									case 2://ʱ      ��2����
										x*=60*60;
										break;
									case 3://��      ��3����
										x*=60*60*24;
										break;
									case 4://��      ��4����
									case 5://��      ��5��
										x*=60*60*24*31;
										break;
								}
								i=UARTCtrl->RM_DI_N;//�ɼ��ϵ�N��
								if(i)
								{
									i--;
								}
								x*=i;
								pYYMDHMS_SubS_hex(pOUT,x);//��������ʱ�����S��,���ؼ�S������������ʱ����
								break;
							case 2://������ʱ��ɼ�,
								id>>=8;
								id&=0xff;//�����
								if(id<(RMTASKmax+ExtRMTASKmax))
								{
									for(i=0;i<7;i++)
									{
										pOUT[i]=UARTCtrl->TaskTime[id].StdTime[i];//������ʼ��׼ʱ��
									}
								}
								break;
							case 3://��ʱ�����ɼ�
								id>>=8;
								id&=0xff;//�����
								if(id>=(RMTASKmax+ExtRMTASKmax))
								{
									break;
								}
							#if (USER/100)==9//����,5���ӵ�ѹ����
								if(PORTn==ACSAMPLEPORT)
								{//��������
									MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
									return 6;
								}
							#endif
								switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
								{
									case 1://DL/T645-1997��1��
										//������ʵʱ���ݴ����ñ�׼����ʼʱ��
										MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
										return 6;
									case 2://DL/T645-2007��2��
										switch(UARTCtrl->RM_DI)
										{
											case 0x06000001://ȫ��6�ฺ������
											case 0x06010001://1�ฺ������(��ѹ��������Ƶ��)
											case 0x06020001://2�ฺ������(���޹�����)
											case 0x06030001://3�ฺ������(��������)
											case 0x06040001://4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
											case 0x06050001://5�ฺ������(1-4�����޹��ܵ���)
											case 0x06060001://6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
												break;
											default:
												//����ʵʱ���ݴ����ñ�׼����ʼʱ��
												MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
												return 6;
										}
										break;
									case 3://DL/T698.45��3��
										break;
									case ProtocolNO_ABB://ABB��ͨ��Э����
									case ProtocolNO_Landis://Landis��ͨ��Э����(IEC1107 ģʽC)
										//������ʵʱ���ݴ����ñ�׼����ʼʱ��
										MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
										return 6;
									default:
										break;
								}
								//������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����
								//����ִ�м��ʱ��
								p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
								i=pTItoSECOND(p8+5);//TIתΪ��ֵ,���:pTI����������;����:hex��
								if(PORTn==RS485_4PORT)
								{//���ز���
								#if (CurveTaskTImax)
									if(i<CurveTaskTImax)//(6*60*60)//���߳�������ִ��Ƶ�����ֵ(��),��ֵ�������ɼ������ɼ����ʱ�����ǰ���������ߵ���
									{
										i=CurveTaskTImax;
									}
								#endif
								}
								//�ɼ������ɼ����ʱ��
								x=pTItoSECOND(UARTCtrl->RM_DI_TI);//TIתΪ��ֵ,���:pTI����������;����:hex��
								
								//#if (USER/100)==17
								//if(UARTCtrl->RM_DI_TI_Count==0)
								//{
									//UARTCtrl->RM_DI_TI_Count=1;
								//}
								//x*=(UARTCtrl->RM_DI_TI_Count-1);
								//#else
								//x*=UARTCtrl->RM_DI_TI_Count;
								//#endif
								x*=UARTCtrl->RM_DI_TI_Count;
								if(i>=x)
								{
									i-=x;
								}
								else
								{
									i=0;
								}
								MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
								pYYMDHMS_SubS_hex(pOUT,i);//��������ʱ�����S��,���ؼ�S������������ʱ����
								break;
							case 4://RetryMetering ����
								id>>=8;
								id&=0xff;//�����
								if(id>=(RMTASKmax+ExtRMTASKmax))
								{
									break;
								}
								MR((u32)pOUT,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
								i=pTItoSECOND(UARTCtrl->RM_DI_TI);//TIתΪ��ֵ,���:pTI����������;����:hex��
								//#if (USER/100)==17
								//if(UARTCtrl->RM_DI_TI_Count==0)
								//{
									//UARTCtrl->RM_DI_TI_Count=1;
								//}
								//i*=(UARTCtrl->RM_DI_TI_Count-1);
								//#else
								//i*=UARTCtrl->RM_DI_TI_Count;
								//#endif
								i*=UARTCtrl->RM_DI_TI_Count;
								pYYMDHMS_SubS_hex(pOUT,i);//��������ʱ�����S��,���ؼ�S������������ʱ����
								return 6;
						}
						break;
					case  0x5B015004://�ն���
						i=id;
						i>>=20;
						i&=0xf;
						switch(i)//��ͨ�ɼ������Ĳɼ�����
						{
							default:
							case 0://�ɼ���ǰ����,
							case 1://�ɼ��ϵ�N��,
							case 2://������ʱ��ɼ�,
							case 3://��ʱ�����ɼ�
								for(i=0;i<4;i++)
								{
									pOUT[i]=Comm_Ram->DL698YMDHMS[i];
								}
								pOUT[4]=0;
								pOUT[5]=0;
								pOUT[6]=0;
								break;
							case 4://RetryMetering ����
								YYMD=(Comm_Ram->DL698YMDHMS[0]<<24)+(Comm_Ram->DL698YMDHMS[1]<<16)+(Comm_Ram->DL698YMDHMS[2]<<8)+Comm_Ram->DL698YMDHMS[3];
								i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
								while(i--)
								{
									YYMD=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
								}
								pOUT[0]=YYMD>>24;
								pOUT[1]=YYMD>>16;
								pOUT[2]=YYMD>>8;
								pOUT[3]=YYMD;
								pOUT[4]=0;
								pOUT[5]=0;
								pOUT[6]=0;
								break;
						}
						break;
					case 0x5B015006://�¶���
						for(i=0;i<4;i++)
						{
							pOUT[i]=Comm_Ram->DL698YMDHMS[i];
						}
						pOUT[3]=0x01;//�̶�Ϊ1��
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						break;
					default:
						for(i=0;i<4;i++)
						{
							pOUT[i]=Comm_Ram->DL698YMDHMS[i];
						}
						pOUT[4]=0;
						pOUT[5]=0;
						pOUT[6]=0;
						id=UARTCtrl->TaskID;
						id>>=8;
						id&=0xff;//�����
						p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
						switch(p8[5])//ִ��Ƶ��   TI
						{
							case 0://��      ��0����
								break;
							case 1://��      ��1����
								break;
							case 2://ʱ      ��2����
								pOUT[4]=Comm_Ram->DL698YMDHMS[4];//ʱ
								break;
							case 3://��      ��3����
								break;
							case 4://��      ��4����
								pOUT[3]=0x01;//�̶�Ϊ1��
								break;
							case 5://��      ��5��
								break;
						}
						break;
				}
			}
			return 6;
		case 7://���������ĩ23��59��
			if(pOUT)
			{
				YYMD=(Comm_Ram->DL698YMDHMS[0]<<16)+(Comm_Ram->DL698YMDHMS[1]<<8)+(Comm_Ram->DL698YMDHMS[2]<<0);
				YYMD=YYM_SubM_hex(YYMD,1);//�����¼�m��,���ؼ�m�º��������
				pOUT[0]=YYMD>>16;
				pOUT[1]=YYMD>>8;
				pOUT[2]=YYMD>>0;
				YYMD=YYM_D_hex(YYMD);//ȡָ�����µ�����
				pOUT[3]=YYMD;
				pOUT[4]=23;
				pOUT[5]=59;
				pOUT[6]=0;
			}
			return 7;
	}
}

void RM_UpDate(void)//�����������;����:�ж�,���ɼ��������ñ��flash��sdram,��λ��PORTn���������ؼ������賭�����ɹ�����
{
	u32 i;
	u32 x;
	u32 n;
	u32 m;
	u32 PORTs;
	u32 PORTd;
	u32 UpDate;
	u8* p8s;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	
	if(Terminal_Ram->SET6000==0x55)//�ɼ��������ñ������ñ�־:0=û����,0x55=����
	{
		Terminal_Ram->SET6000=0;//�ɼ��������ñ������ñ�־:0=û����,0x55=����
		UpDate=0;//�仯��־
		n=0;
		while(n<NUMmax_6000)
		{
			m=LEN_128KDATABUFF/LENper_6000;
			if(m>(NUMmax_6000-n))
			{
				m=NUMmax_6000-n;
			}
			MR(ADDR_128KDATABUFF,ADDR_6000,m*LENper_6000);
			for(i=0;i<m;i++)
			{
				//ɾ��
				p8s=(u8*)ADDR_128KDATABUFF+(i*LENper_6000);
				p8d=(u8*)ADDR_6000_SDRAM+((n+i)*LENper_6000);
				if(p8s[0]!=p8d[0])
				{
					break;
				}
				//��ַ
				p8s+=8;
				p8d+=8;
				//x=Compare_octet_string(p8s,p8d);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				Compare_TSA_string(p8s,p8d);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(x)
				{
					break;
				}
				//�˿�
				x=p8s[0];
				x++;
				x+=5;
				p8s+=x;
				p8d+=x;
				PORTs=(p8s[0]<<24)|(p8s[1]<<16)|(p8s[2]<<8)|p8s[3];
				PORTd=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
				if(PORTs!=PORTd)
				{
					//0xF2090201://RS485_4ͨ�ſں�(�������ز�,ר�����ģ��)
					break;
				}
			}
			if(i<m)
			{
				UpDate=1;//�仯��־
				break;
			}
			n+=m;
		}
		if(UpDate)//�仯��־
		{
			MR(ADDR_6000_SDRAM,ADDR_6000,LENmax_6000);
			Terminal_Ram->SETTask=0xff;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
			UARTCtrl->SetChange6000=0x55;//�ɼ��������ñ��б仯��־:0=û�仯,0x55=�仯
			Terminal_Ram->RouterSynDelay_S_Timer=10;//·�ɵ���ͬ����ʱִ���붨ʱ��
			RM_State_SetUpDate();//�ɼ�״̬,�������ʱ����(��Ӧ�������)
			
		}
		return;
	}
	if(Terminal_Ram->FLASHtoSDRAM!=Comm_Ram->DL698YMDHMS[4])//����FLASHtoSDRAM�ϴν��е�ʵʱ��ʱ(������)
	{
		Terminal_Ram->FLASHtoSDRAM=Comm_Ram->DL698YMDHMS[4];//����FLASHtoSDRAM�ϴν��е�ʵʱ��ʱ(������)
		FLASHtoSDRAM();//����FLASHtoSDRAM
	}
}








