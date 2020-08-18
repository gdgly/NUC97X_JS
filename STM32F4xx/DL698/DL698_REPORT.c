

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_REPORT.h"
#include "../DL698/TASK.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/RM_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"


void RM_Monitor(u32 ID,u32 NUM_Tx,u32 NUM_Rx);//�ɼ�������;���:ID=�����,NUM_Tx=��������,NUM_Rx=��������

//REPORT-Notification��=CHOICE
//{
//�ϱ����ɸ���������         [1] ReportNotificationList��
//�ϱ����ɸ���¼�Ͷ�������   [2] ReportNotificationRecordList
//}


void REPORT_PORTn(msFILE_TypeDef *file)//ͨ�������ϱ�ʱ�����ϱ����ݸ���=��ǰ���ݸ�����ʹ��ADDR_DATABUFF
{
/*
	u32 i;
	u32 n;
	u32 OI;
	u32 NUM_DATA;
	u8* p8;
	
	NUM_DATA=file->NUM_DATA;
	MR(ADDR_DATABUFF,ADDR_4300_10,LENmax_4300_10);//�ϱ�ͨ��
	p8=(u8*)ADDR_DATABUFF;
	n=p8[1];//������
	p8+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8[1]<<8)|p8[2];
		if(OI==0x4500)//GPRS
		{
			break;
		}
		p8+=5;
	}
	if(i>=n)
	{//GPRS���ϱ�ͨ��
		for(i=0;i<NUMmax_601C;i++)
		{
			file->NUM_GPRSreport[i]=NUM_DATA;
		}
	}
	p8=(u8*)ADDR_DATABUFF;
	p8+=2;
	for(i=0;i<n;i++)
	{
		OI=(p8[1]<<8)|p8[2];
		if(OI==0x4510)//ENET
		{
			break;
		}
		p8+=5;
	}
	if(i>=n)
	{//ENET���ϱ�ͨ��
		for(i=0;i<NUMmax_601C;i++)
		{
			file->NUM_ENETreport[i]=NUM_DATA;
		}
	}
*/
}
	
void REPORT_complete(u32 PORTn)//�����ϱ���ɺ���(��ʱ����)
{
	u32 i;
	u32 x;
	u32 n;
	u32 id;
	u32 filename;
	msFILE_TypeDef *file;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	x=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
	UARTCtrl->REPORT_NUM_File=0;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
	
	if(UARTCtrl->HostACK==5)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
	{
		id=UARTCtrl->TaskID;
		id&=0xff;//�ϱ�������
		if(id<NUMmax_601C)
		{
			if((x!=0)&&(x<=REPORT_NUMFILEmax))
			{
				Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
				for(i=0;i<x;i++)
				{
					filename=UARTCtrl->REPORT_filename[i];//��ǰ�����ϱ����ݵ��ļ���,0xffff��ʾ��Ч�ļ���
					if(filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
					{
						if(filename<recordFILENAMEmax_FLASH)
						{
							file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
						}
						else
						{
							file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
						}
						n=UARTCtrl->REPORT_NUM_Data[i];
					#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
						if(PORTn==GPRSPORT)
						{
							file->NUM_GPRSreport[id]+=n;//��ǰ�����ϱ����ݵ��ļ��а����е��ܱ�ĸ������¼�����
						}
						else
						{
							file->NUM_ENETreport[id]+=n;//��ǰ�����ϱ����ݵ��ļ��а����е��ܱ�ĸ������¼�����
						}
					#else
						file->NUM_GPRSorENETreport[id]+=n;//��ǰ�����ϱ����ݵ��ļ��а����е��ܱ�ĸ������¼�����
					#endif
					}
				}
			}
		}
		UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
	}
}

void REPORT_END(u32 PORTn)//�����ϱ��������ʱ��(��ʱ����)
{
	u32 i;
	u32 id;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	id=(i>>8)&0xff;//�����
	if(id>=NUMmax_6012_2)
	{
		return;
	}
	UARTCtrl->TaskTime[id].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
	for(i=0;i<7;i++)
	{
		UARTCtrl->TaskTime[id].EndTime[i]=Comm_Ram->DL698YMDHMS[i];//����ִ�н���ʱ��YYMDHMS
	}
}

//#define ADDR_601C     ADDR_6018+LENmax_6018///�ϱ�������(��ʼ��ֵ0x00)
//����2���ϱ����� report plan����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//�������          unsigned��
//�ϱ�ͨ��          array OAD��
//�ϱ���Ӧ��ʱʱ��  TI��
//����ϱ�����      unsigned��
//�ϱ�����          strcuture
//{
//  ����  unsigned��
//  ����  Data
//}
//}
//�ϱ����ݣ�
//�ϱ�����	  �ϱ�����	��ʾ
// 0	      OAD	������������
// 1	      RecordData	�ϱ���¼�Ͷ�������
//RecordData��=structure
//{
//  ����������������      OAD��
//	��¼�Ͷ�������������  RCSD��
//	��ѡ��                RSD
//}
//#define LENper_601C     512
#include "../DL698/DL698_JSON.h"
void REPORT_Notification(u32 PORTn)//�ϱ�֪ͨ(��ʱ����)
{
	u32 i;
	u32 x;
	u32 n;
	u32 TaskID;
	u32 id;
	u32 reportid;
	u32 fileid;
	u32 OAD;
	u32 LEN_Tx;
	//u32 TxByte;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//��󻹿ɷ��͵��ֽ���
	u8* p8;
	u32 NUM_RCSD;
	u32 LEN_RCSD;
	u8* pRCSD;
	u8* pRSD;
	u32 fRSD;//��ѡ����Ч��־
	u32 FILECOUNT;
	u32 FILE_OFFSET;
	u32 fDATA;//�����ݱ�־
	u32 NUM_READFILE;//���ļ��ܴ���
	u16* p16time;
	u8* p8tx;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//�����¼�ļ�ͷ�ļ�
	u32 ProjectType;//��������
	
	
	if((PORTn!=GPRSPORT)&&(PORTn!=ENETPORT))
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Task!=0)
	{
		return;
	}
	if(UARTCtrl->LinkTask!=101)
	{//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
		return;
	}
	//myprintf("111111111111111111111111111\n", 0, 0, 0);

//---��RS232�ڲ����ϱ�---
	
//	if(PORTn!=RS232PORT)
//	{
//		return;
//	}
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//	if(UARTCtrl->Task!=0)
//	{
//		return;
//	}
	
//---��RS232�ڲ����ϱ� END---
	

	//myprintf("UARTCtrl->HostACK=%d\n", UARTCtrl->HostACK, 0, 0);
	if((UARTCtrl->HostACK)&&((UARTCtrl->HostACK!=5)&&(UARTCtrl->HostACK!=8)))//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
	{//�����������ϱ��ȴ�ȷ�Ϸ���
		return;
	}
	i=Comm_Ram->DL698YMDHMS[6];
	if(UARTCtrl->REPORT_RTCS==i)//����ʱ�����ϱ���RTC��
	{
		//myprintf("����ʱ�����ϱ���RTC��\n", 0, 0, 0);
		return;
	}
	if(i&1)
	{
		return;//��ǰʱ��������,�������¼�,ż������ʱ����
	}
	UARTCtrl->REPORT_RTCS=i;//����ʱ�����ϱ���RTC��
	i=MRR(ADDR_4300_8+1,1);//���������ϱ�
	if(i==0)
	{//�����������ϱ�
		UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
		//���������ϱ����ļ������ϱ���������
		/*
		Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯(1=�������ݱ���,2=���������ϱ�����)
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
		{
			if(filename==recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			}
			if(file->BLOCK_ENTRY)
			{
				x=file->NUM_DATA;
				for(i=0;i<RMTASKmax;i++)
				{
					file->NUM_REPORT[i]=x;
				}
			}
			file++;
		}
		*/
		return;
	}
	switch(PORTn)
	{
		case GPRSPORT:
			p16time=(u16*)&Terminal_Ram->GPRS_WaitHostACK_S_Timer;//29 GPRS�����ϱ��ȴ���վȷ���붨ʱ��
			break;
		case ENETPORT:
			p16time=(u16*)&Terminal_Ram->ENET_WaitHostACK_S_Timer;//30 ENET�����ϱ��ȴ���վȷ���붨ʱ��
			break;
		default:
		case RS232PORT:
			p16time=(u16*)&Terminal_Ram->RS232_WaitHostACK_S_Timer;//31 RS232�����ϱ��ȴ���վȷ���붨ʱ��
			break;
	}
	if(p16time[0]!=0)
	{//�ϱ���Ӧ��ʱʱ��û��
		return;
	}
	i=UARTCtrl->REPORT_NUMmax;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
	if(i)
	{
		i--;
		UARTCtrl->REPORT_NUMmax=i;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
		p16time[0]=UARTCtrl->REPORT_OVER_SECOND;//�ϱ���Ӧ��ʱʱ��Ĵ�
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
		i=Get_LEN_UARTnTx(PORTn);
		p8+=i;
		LEN_Tx=p8[1]|(p8[2]<<8);
		LEN_Tx+=2;
		if(LEN_Tx<=i)
		{
			MR((u32)p8tx,(u32)p8,LEN_Tx);
			UARTCtrl->TxByte=LEN_Tx;
			UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
		}
		return;
	}
	REPORT_complete(PORTn);//��ǰ�����ϱ���ɺ���(��ʱ����)
	
#if(Project/100==2)&&(USER/100==17)//���ռ�����
	if(Terminal_Router->MinuteCollect!=0)
	{
		i=Report_JsonFile(p16time,PORTn);
		if(i)
		{
			return;
		}
	}
#endif
//��������
	if(PORTn==GPRSPORT)
	{
		if(Terminal_Ram->GPRSHeartBeat_S_Timer==0)//GPRS�����붨ʱ��
		{
			#if(Project/100==2)&&(USER/100==17)//���ռ�����
			if(UARTCtrl->HostACK==8)//����
			{
				UARTCtrl->HostACK=3;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
			}
			#endif
			return;
		}
	}
	if(PORTn==ENETPORT)
	{
		if(Terminal_Ram->ENETHeartBeat_S_Timer==0)//��̫�������붨ʱ��
		{
			#if(Project/100==2)&&(USER/100==17)//���ռ�����
			if(UARTCtrl->HostACK==8)//����
			{
				UARTCtrl->HostACK=3;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
			}
			#endif
			return;
		}
	}
	
	

	TASK(PORTn,0);//����;���:RM=0�ϱ�����,RM=1�ɼ�����
	TaskID=UARTCtrl->TaskID;
	id=(TaskID>>8)&0xff;//�����
	#if (USER/100)==15//����
	if(id==16)
	{
		n=2;//����ID=2
		RM_Monitor(2,0,0);
		p8=(u8*)ADDR_6034+(n*LENper_6034);
		p8=Get_Element(p8,2,0,0);
		if(p8)
		{
			if(p8[0]!=0x16||p8[1]!=0x02)
			{
				return;
			}
		}
		else
			return;
	}
	#endif
	if(TaskID)
	{//���ϱ�����
//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//b31-b28���ȼ�:1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�,4=����
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��
		if(id>=NUMmax_6012_2)
		{
			return;
		}
		if(UARTCtrl->TaskTime[id].Run==2)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
		{
			return;
		}
		reportid=TaskID&0xff;//�ϱ�������
		if(reportid>=NUMmax_601C)
		{//������ų�
			REPORT_END(PORTn);//�����ϱ��������ʱ��
			return;
		}
		p8=(u8*)ADDR_601C_SDRAM+((TaskID&0xff)*LENper_601C);
		if(p8[0]!=DataType_structure)
		{//ɾ��
			REPORT_END(PORTn);//�����ϱ��������ʱ��
			return;
		}
		//�ϱ������е��ϱ�ͨ��
		x=PORTntoOAD(PORTn);//ͨ�ſں�תΪOADֵ
		p8+=4;
		n=p8[1];//������
		p8+=2;
		x>>=16;
		for(i=0;i<n;i++)
		{
			OAD=(p8[1]<<8)|p8[2];
			if(OAD==x)
			{
				break;
			}
			p8+=5;
		}
		if(i>=n)
		{//���ϱ��˿�
/*
			if(reportid!=0)//�ϱ�������
			{
				Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
				for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
				{
					if(filename==recordFILENAMEmax_FLASH)
					{
						file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
					}
					if(file->BLOCK_ENTRY)
					{
						x=file->NUM_DATA;
						if(PORTn==GPRSPORT)
						{
							file->NUM_GPRSreport[reportid]=x;
						}
						else
						{
							file->NUM_ENETreport[reportid]=x;
						}
					}
					file++;
				}
			}
*/
			REPORT_END(PORTn);//�����ϱ��������ʱ��
			return;//���ϱ�ͨ��
		}
		//
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
		LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
	#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
		ClientConnectInfo=ClientConnectInfo;
	#else
		//�ͻ������ջ��峤��
		ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
		i=ClientConnectInfo->LEN_Rx;
		i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
		if(i<2048)
		{
			i=2048;
		}
		if(i>(TxAPDUOffset+1))
		{//��Ϊ0
			i-=(TxAPDUOffset+1);
			if(LENmax_TxSPACE>i)
			{
				LENmax_TxSPACE=i;
			}
		}
	#endif
		//�ϱ�����
		p8=(u8*)ADDR_601C_SDRAM+((TaskID&0xff)*LENper_601C);
		p8=Get_Element(p8,5,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
		if(p8==0)
		{
			REPORT_END(PORTn);//�����ϱ��������ʱ��
			return;
		}
		if(p8[3]==0)
		{//0=OAD	������������
			REPORT_END(PORTn);//�����ϱ��������ʱ��(��ʱ����)
			p8+=4;
			if(p8[0]==DataType_array)
			{
				n=p8[1];
				p8+=2;
			}
			else
			{
				n=1;
			}
		//APDU��88 01 01 01 40 00 02 00 01 1C 07 E0 0B 0E 00 02 00 00 00 
			p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=0x88;//�ϱ�֪ͨ REPORT-Notification
			p8tx[1]=0x01;//�ϱ����ɸ���������         [1] ReportNotificationList
									//�ϱ����ɸ���¼�Ͷ�������   [2] ReportNotificationRecordList
			i=UARTCtrl->REPORT_PIID;
			i++;
			i&=0x3f;
			UARTCtrl->REPORT_PIID=i;
			p8tx[2]=i;//PIID
			p8tx[3]=0;//SEQUENCE OF OAD������=?
			LEN_TxAPDU=4;
			LENmax_TxSPACE-=LEN_TxAPDU+2;//����2byte(1byteʱ���ǩ,1byte�ϱ���ϢFollowReport OPTIONAL��
			
			while(n--)
			{
				if(LENmax_TxSPACE<6)
				{
					break;
				}
				MR((u32)p8tx+LEN_TxAPDU,(u32)p8+1,4);
				LENmax_TxSPACE-=4;
				UARTCtrl->NEXT=0;//DL698��֡��־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡
				i=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
				p8+=5;
				i=GET_OAD_DATA(PORTn,i,p8tx+LEN_TxAPDU+4+1,LENmax_TxSPACE-1);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
				if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
				{
					break;
				}
				LEN_TxAPDU+=4;
				if(UARTCtrl->DAR!=0)//���ݷ��ʽ��
				{
					p8tx[LEN_TxAPDU+0]=0;//Get-Result��=CHOICE,������Ϣ  [0]   DAR
					p8tx[LEN_TxAPDU+1]=UARTCtrl->DAR;
					LEN_TxAPDU+=2;
					LENmax_TxSPACE-=2;
				}
				else
				{
					p8tx[LEN_TxAPDU]=1;//Get-Result��=CHOICE,  [1]   DATA
					LEN_TxAPDU+=i+1;
					LENmax_TxSPACE-=i+1;
				}
				p8tx[3]++;//SEQUENCE OF OAD������=?
			}
			p8tx[LEN_TxAPDU+0]=0;//ʱ���ǩ
			p8tx[LEN_TxAPDU+1]=0;//�ϱ���ϢFollowReport OPTIONAL
			LEN_TxAPDU+=2;
			
		}
		else
		{//1=RecordData	�ϱ���¼�Ͷ�������
			//�ȼ����ϱ��������ڳ�������,Ȼ���ɷ�����ȷ������������ļ�����.
			p8+=6;
//RecordData��=structure
//{
//  ����������������      OAD��
//	��¼�Ͷ�������������  RCSD��
//	��ѡ��                RSD
//}
//02 03
//51 60 12 03 00 ����������������      OAD
//60 01 RCSD��=SEQUENCE OF CSD
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 
//5a 00 ��ѡ��                RSD
			//���������������� OAD
			OAD=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
			switch(OAD)
			{
				case 0x60120300:
					ms=(ms_Type *)Get_ms();
					pRCSD=p8+5;//��¼�Ͷ�������������  RCSD
					i=Get_DL698DataLen_S(pRCSD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����)
				
					pRSD=pRCSD+i;//��ѡ�� RSD
					fileid=RCSDtoFileID(pRCSD);//��¼��ѡ��RCSD���Ӧ��FileID,����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO�ֶ�=0xff;RCSD��ֻ��ͨ�ŵ�ַ�ȷǳ���������ʱ����0xffffffff
					if((fileid==0)||(fileid==0xffffffff))
					{//0��ʾû�ҵ�,��ЧFileID
						UARTCtrl->TaskTime[id].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
						return;
					}
					ProjectType=fileid>>28;//��������
					if(ProjectType==1)
					{//��ͨ�ɼ�����
						fileid>>=20;
					}
					else
					{
						fileid>>=12;
						fileid&=0xff;
					}
					//���ļ��������ϱ�
//TxAPDU��
//88 �ϱ�֪ͨ
//02 �ϱ����ɸ���¼�Ͷ�������
//02 PIID
//01 SEQUENCE OF A-ResultNormal
//60 12 03 00  OAD
//01 RCSD��SEQUENCE OF����=1
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 //RCSD
//01 ��¼����
//02 M����¼��M=2
//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
//00 00 
					p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
					p8tx[0]=0x88;//�ϱ�֪ͨ REPORT-Notification
					p8tx[1]=0x02;//�ϱ����ɸ���������[1] ReportNotificationList,�ϱ����ɸ���¼�Ͷ�������   [2] ReportNotificationRecordList
					i=UARTCtrl->REPORT_PIID;
					i++;
					i&=0x3f;
					UARTCtrl->REPORT_PIID=i;
					p8tx[2]=i;//PIID
					//SEQUENCE OF A-ResultNormal
					//A-ResultNormal��=SEQUENCE
					//{
					//  ��������������  OAD��
					//  ������        Get-Result
					//}
					//Get-Result��=CHOICE
					//{
					//  ������Ϣ  [0]   DAR��
					//  ����      [1]   Data
					//}
					p8tx[3]=1;//SEQUENCE OF A-ResultNormal
					p8tx[4]=0x60;
					p8tx[5]=0x12;
					p8tx[6]=0x03;
					p8tx[7]=0x00;
					//RCSD�ܳ���
					NUM_RCSD=pRCSD[1];//RCSD��SEQUENCE OF����
					LEN_RCSD=0;
					for(n=0;n<NUM_RCSD;n++)
					{
						x=pRCSD[2+LEN_RCSD];
						if(x==0)
						{//OAD
							LEN_RCSD+=5;
						}
						else
						{//ROAD
							x=pRCSD[2+LEN_RCSD+5];
							x*=4;
							x+=6;
							LEN_RCSD+=x;
						}
					}
					p8tx[8]=NUM_RCSD;
					MR((u32)p8tx+9,(u32)pRCSD+2,LEN_RCSD);
					p8tx[9+LEN_RCSD]=1;//��¼����
					p8tx[10+LEN_RCSD]=0;//M����¼
					
					LEN_TxAPDU=11+LEN_RCSD;
					LENmax_TxSPACE-=LEN_TxAPDU+2;//����2byte(1byteʱ���ǩ,1byte�ϱ���ϢFollowReport OPTIONAL��
					
					fDATA=0;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
					NUM_READFILE=0;//���ļ��ܴ���
					UARTCtrl->REPORT_NUM_File=0;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
					FILECOUNT=96;//���ϱ��ļ������������
					
					while(1)
					{
						if(UARTCtrl->REPORT_NUM_File>=REPORT_NUMFILEmax)//�ϱ����ݵĲ�ͬ�ļ������
						{
							break;
						}
						if(fDATA==2)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
						{
							break;
						}
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
						for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
						{
							if(fDATA==2)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
							{
								break;
							}
							if(filename==recordFILENAMEmax_FLASH)
							{
								file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
							}
							if(file->BLOCK_ENTRY)
							{
								if(file->FILECOUNT==FILECOUNT)
								{//��������ͬ
									i=file->FILEID;
									if((i&0xff)==0)
									{//�ļ�����=0
										if(ProjectType==1)//��������
										{//��ͨ�ɼ�����
											i>>=20;
										#if (USER/100)==0//�ǵ��Ժ����
										//---���Ժ����������ϱ���������Ϊ���ϸ�����ʱ������---
											if(((i>>4)&0xf)==3)
											{//�ɼ�����=3,��������
												p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
												p8+=4+1;//ִ��Ƶ��   TI
												n=pTItoSECOND(p8);//TIתΪ��ֵ,���:pTI����������,����:hex��
												MR(ADDR_DATABUFF,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
												pYYMDHMS_SubS_hex((u8*)ADDR_DATABUFF,n);//��������ʱ�����S��,���ؼ�S������������ʱ����
												n=Compare_DL698DataString(file->save_date_time_s,(u8*)ADDR_DATABUFF,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
												if(n!=2)
												{//û���ϱ�ʱ��,�ݲ���
													i=~fileid;//ʹi!=fileid
												}
											}
										//---���Ժ����������ϱ���������... RND---
										#endif
										}
										else
										{
											i>>=12;
											i&=0xff;
										}
										if(i==fileid)
										{//�ɼ���ͬ
										#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
											if(PORTn==GPRSPORT)
											{
												n=file->NUM_GPRSreport[reportid];
											}
											else
											{
												n=file->NUM_ENETreport[reportid];
											}
										#else
											n=file->NUM_GPRSorENETreport[reportid];
										#endif
											if(file->NUM_DATA>n)
											{//���������ϱ�
												//����¼ͷ
												ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,NUM_RMmax*sizeof(RecordFileHead_TypeDef));
												RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
												FILE_OFFSET=0;//�������ݵ�ַ
												for(i=0;i<n;i++)
												{
													FILE_OFFSET+=RecordFileHead->LEN_DATA;
													RecordFileHead++;
												}
												i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
												if(i>=REPORT_NUMFILEmax)//�ϱ����ݵĲ�ͬ�ļ������
												{
													break;
												}
												UARTCtrl->REPORT_filename[i]=filename;//1֡�����ϱ����ݲ�ͬ�ļ����б�
												UARTCtrl->REPORT_NUM_Data[i]=0;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
												i++;
												UARTCtrl->REPORT_NUM_File=i;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
												while(1)
												{
													i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
													if(i)
													{
														i--;
													}
													n=UARTCtrl->REPORT_NUM_Data[i];
												#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
													if(PORTn==GPRSPORT)
													{
														n+=file->NUM_GPRSreport[reportid];
													}
													else
													{
														n+=file->NUM_ENETreport[reportid];
													}
												#else
													n+=file->NUM_GPRSorENETreport[reportid];
												#endif
													if(file->NUM_DATA<=n)
													{//û�������ϱ�
														break;
													}
													i=RecordFileHead->LEN_DATA;
													NUM_READFILE++;//���ļ��ܴ���
													if(NUM_READFILE>127)//���ļ��ܴ���
													{//���ļ��ܴ���������ÿ������ʱ��
														break;
													}
													if(p8tx[10+LEN_RCSD]>=127)//M����¼
													{
														fDATA=2;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
														break;
													}
													//ȡ��¼����
													ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+(64*1024),i);
													fRSD=0;//��ѡ����Ч��־
													switch(pRSD[1])//��ѡ��
													{
														case 0:
												//��ѡ��       [0]  NULL
															fRSD=1;//��ѡ����Ч��־
															break;
														case 1:
												//Selector1��=SEQUENCE
												//{
												//  ��������������  OAD��
												//  ��ֵ            Data
												//}
															fRSD=0;//��ѡ����Ч��־
															break;
														case 2:
												//Selector2��=SEQUENCE
												//{
												//  ��������������  OAD��
												//  ��ʼֵ          Data��
												//  ����ֵ          Data��
												//  ���ݼ��        Data
												//}
															fRSD=0;//��ѡ����Ч��־
															break;
														case 3:
												//Selector3��=SEQUENCE OF Selector2
															fRSD=0;//��ѡ����Ч��־
															break;
														case 4:
												//Selector4��=SEQUENCE
												//{
												//  �ɼ�����ʱ��  date_time_s��
												//  ���ܱ���    MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//���ݵĵ��ܱ��ַ
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+7);//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
															break;
														case 5:
												//Selector5��=SEQUENCE
												//{
												//  �ɼ��洢ʱ��  date_time_s��
												//  ���ܱ���    MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//���ݵĵ��ܱ��ַ
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+7);//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
															break;
														case 6:
												//Selector6��=SEQUENCE
												//{
												//  �ɼ�����ʱ����ʼֵ  date_time_s��
												//  �ɼ�����ʱ�����ֵ  date_time_s��
												//  ʱ����            TI��
												//  ���ܱ���          MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//���ݵĵ��ܱ��ַ
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+17);//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
															break;
														case 7:
												//Selector7��=SEQUENCE
												//{
												//  �ɼ��洢ʱ����ʼֵ  date_time_s��
												//  �ɼ��洢ʱ�����ֵ  date_time_s��
												//  ʱ����            TI��
												//  ���ܱ���          MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//���ݵĵ��ܱ��ַ
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+17);//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
															break;
														case 8:
												//Selector8��=SEQUENCE
												//{
												//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
												//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
												//  ʱ����            TI��
												//  ���ܱ���          MS
												//}
															p8=(u8*)ADDR_DATABUFF;
															p8[0]=DataType_TSA;
															p8[1]=7;
															p8[2]=5;
															MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//���ݵĵ��ܱ��ַ
															fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+17);//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
															break;
														case 9:
												//Selector9��=SEQUENCE
												//{
												//  �ϵ�n�μ�¼  unsigned
												//}
															if(file->FILECOUNT<=1)
															{//�ϱ����1��
																fRSD=1;//��ѡ����Ч��־
															}
															else
															{
																fRSD=0;//��ѡ����Ч��־
															}
															break;
														case 10:
												//Selector10��=SEQUENCE
												//{
												//��n����¼  unsigned��
												//���ܱ���  MS
												//}
															if(file->FILECOUNT<=1)
															{//�ϱ����1��
																p8=(u8*)ADDR_DATABUFF;
																p8[0]=DataType_TSA;
																p8[1]=7;
																p8[2]=5;
																MR(ADDR_DATABUFF+3,ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)),6);//���ݵĵ��ܱ��ַ
																fRSD=Compare_TSA_MS((u8 *)ADDR_6000_SDRAM,p8+1,pRSD+2+1);//�Ƚ�TSA�Ƿ����MS����,���:pADDR_6000����RAM,pTSA,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
															}
															else
															{
																fRSD=0;//��ѡ����Ч��־
															}
															break;
														default:
															fRSD=0;//��ѡ����Ч��־
															break;
													}
													if(fRSD)//��ѡ����Ч��־
													{//��ѡ������������ϱ�
														p8=(u8*)(ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)));//���ݵĵ��ܱ��ַ
														i=Get_RecordFileData_RCSD(pRCSD+1,p8,(u8*)ADDR_128KDATABUFF+(64*1024),(u8*)ADDR_DATABUFF,LEN_DATABUFF,0);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
														if(i)
														{
															if(i<=LENmax_TxSPACE)
															{
																MR((u32)p8tx+LEN_TxAPDU,ADDR_DATABUFF,i);
																LEN_TxAPDU+=i;
																LENmax_TxSPACE-=i;
																p8tx[10+LEN_RCSD]++;//M����¼
																fDATA=1;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
															}
															else
															{
																if(fDATA)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
																{
																	fDATA=2;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
																	break;
																}
																//���������ݻ��岻��������������ϱ�
																
															}
														}
													}
													i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
													if(i)
													{
														i--;
													}
													UARTCtrl->REPORT_NUM_Data[i]++;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
													FILE_OFFSET+=RecordFileHead->LEN_DATA;
													RecordFileHead++;
												}
											}
										}
									}
								}
							}
							file++;
						}
						if(FILECOUNT==0)
						{
							break;
						}
						FILECOUNT--;
					}
					if(fDATA==0)//�����ݱ�־
					{//û�ϱ�����
						if(UARTCtrl->REPORT_NUM_File==0)//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
						{
							REPORT_END(PORTn);//�����ϱ��������ʱ��
						}
						else
						{
							//�ļ����Ѵ���������ݸ���
							UARTCtrl->HostACK=5;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
							REPORT_complete(PORTn);//��ǰ�����ϱ���ɺ���(��ʱ����)
						}
						return;
					}				
					p8tx[LEN_TxAPDU+0]=0;//ʱ���ǩ
					p8tx[LEN_TxAPDU+1]=0;//�ϱ���ϢFollowReport OPTIONAL
					LEN_TxAPDU+=2;
					break;
				default:
					UARTCtrl->TaskTime[id].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=ִ��δ���
					return;
			}
		}
		
		i=MRR(ADDR_F101_2+1,1);//��ȡ��ȫģʽ����
		if(i==0)
		{//0�������ð�ȫģʽ������Ĭ�ϰ�ȫģʽ����
			p8tx-=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=0x68;
			//2byte������L
			p8tx[3]=0x83;//������C
			p8tx[4]=5;//TSA
			MR((u32)p8tx+5,ADDR_4001+2,6);
			Data_Inverse((u32)p8tx+5,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			p8tx[11]=0;//�ͻ�����ַCA
			//2byte֡ͷУ��HCS
			MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
			//2byteУ��
			LEN_Tx=14+LEN_TxAPDU+2;
			p8tx[LEN_Tx]=0x16;
			LEN_Tx++;
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
		}
		else
		{//1�����ð�ȫģʽ������Ĭ�ϰ�ȫģʽ����
			p8tx-=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
			p8tx[0]=0x68;
			//2byte������L
			p8tx[3]=0x83;//������C
			p8tx[4]=5;//TSA
			MR((u32)p8tx+5,ADDR_4001+2,6);
			Data_Inverse((u32)p8tx+5,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			p8tx[11]=0;//�ͻ�����ַCA
			//2byte֡ͷУ��HCS
			p8tx[14]=0x10;//0x10,0x00,0x13,//��ȫ����
			p8tx[15]=0x00;//��ȫ���
			//���ĳ���
			if(LEN_TxAPDU<127)
			{
				p8tx[16]=LEN_TxAPDU;
				LEN_Tx=17;
			}
			else
			{
				p8tx[16]=0x82;
				p8tx[17]=LEN_TxAPDU>>8;
				p8tx[18]=LEN_TxAPDU;
				LEN_Tx=19;
			}
			i=TESAM_CMD_TxRx(0x80140103,LEN_TxAPDU,(u32)p8tx+TxAPDUOffset);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			if(i)
			{//ESAM��֤ʧ��
				UARTCtrl->REPORT_NUM_File=0;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
				return;
			}
			MR((u32)p8tx+LEN_Tx,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
			LEN_Tx+=LEN_TxAPDU;
			//02 0C 82 07 EF 22 99 AD 50 09 00 00 00 0F 04 FB 43 41 D0
			p8tx[LEN_Tx]=2;//�����+����MAC  [2]  RN_MAC
			p8tx[LEN_Tx+1]=12;
			MR((u32)p8tx+LEN_Tx+2,ADDR_UART7Rx+5,12);
			p8tx[LEN_Tx+2+12]=4;
			MR((u32)p8tx+LEN_Tx+15,ADDR_UART7Rx+5+12,4);
			LEN_Tx+=19;
			LEN_Tx+=3;//2byteУ��,1byte 0x16
			UARTCtrl->TxByte=LEN_Tx;
			LEN_Tx-=2;
			p8tx[1]=LEN_Tx;
			p8tx[2]=LEN_Tx>>8;
		}
		
		Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
		UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
		//����֡������DataBuff+LEN_UARTnTx,�����ط�
		i=Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
		i+=Get_LEN_UARTnTx(PORTn);
		MW((u32)p8tx,i,LEN_Tx+2);
		//�ϱ���Ӧ��ʱʱ��  TI
		p16time[0]=WaitHostACKMinTimer;//Ĭ����Сֵ
		UARTCtrl->REPORT_OVER_SECOND=WaitHostACKMinTimer;//�ϱ���Ӧ��ʱʱ��Ĵ�
		UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
		UARTCtrl->HostACK=5;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
		p8=(u8*)ADDR_601C_SDRAM+((TaskID&0xff)*LENper_601C);
		p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
		if(p8)
		{
			i=MRR((u32)p8+1,3);
			i=TItoSECOND(i);//TIתΪ��ֵ,����:hex��
			if(i>WaitHostACKMaxTimer)
			{
				i=WaitHostACKMaxTimer;
			}
			if(i<WaitHostACKMinTimer)
			{
				i=WaitHostACKMinTimer;
			}
			p16time[0]=i;
			UARTCtrl->REPORT_OVER_SECOND=i;//�ϱ���Ӧ��ʱʱ��Ĵ�
		//����ϱ�����
			i=p8[5];
			if(i>=1)
			{
				i--;
				UARTCtrl->REPORT_NUMmax=i;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
				if(i==0)
				{
					p16time[0]=0;
				}
			}
		}

	}
}




void EVENT_REPORT_complete(u32 PORTn,u32 ACK)//�����ϱ���ɺ���(�¼�),���:ACK=0ûȷ��,1=ȷ��
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 filename;
	msFILE_TypeDef *file;
	UARTCtrl_TypeDef *UARTCtrl;
	EventFileHead_TypeDef* EventFileHead;
	ms_Type *ms;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	x=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
	UARTCtrl->REPORT_NUM_File=0;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
	
	if(UARTCtrl->HostACK==4)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
	{
		ms=(ms_Type *)Get_ms();
		if((x!=0)&&(x<=REPORT_NUMFILEmax))
		{
			Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
			for(i=0;i<x;i++)
			{
				filename=UARTCtrl->REPORT_filename[i];//��ǰ�����ϱ����ݵ��ļ���,0xffff��ʾ��Ч�ļ���
				if(filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
				{
					if(filename<recordFILENAMEmax_FLASH)
					{
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
					}
					else
					{
						file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
					}
					n=UARTCtrl->REPORT_NUM_Data[i];
					if(n)
					{
					#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
						if(PORTn==GPRSPORT)
						{
							z=file->NUM_GPRSreport[0];//���ϱ���
							file->NUM_GPRSreport[0]=z+n;//��ǰ�����ϱ����ݵ��ļ��а����е��ܱ�ĸ������¼�����
						}
						else
						{
							z=file->NUM_ENETreport[0];//���ϱ���
							file->NUM_ENETreport[0]=z+n;//��ǰ�����ϱ����ݵ��ļ��а����е��ܱ�ĸ������¼�����
						}
					#else
							z=file->NUM_GPRSorENETreport[0];//���ϱ���
							file->NUM_GPRSorENETreport[0]=z+n;//��ǰ�����ϱ����ݵ��ļ��а����е��ܱ�ĸ������¼�����
					#endif
						if((file->FILEID&0xff)!=2)
						{//���¼���¼�ļ�2,��0=ȫ�¼��ɼ���¼��5=DL645���ܱ������ϱ�
							continue;
						}
						//�¼����ϱ���־
						y=file->NUM_DATA;
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,y*sizeof(EventFileHead_TypeDef));
						EventFileHead=(EventFileHead_TypeDef*)(ADDR_128KDATABUFF+(z*sizeof(EventFileHead_TypeDef)));
						while(n--)
						{
							if(z<y)
							{
								if(EventFileHead->REPORT&1)//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
								{
									if(PORTn==GPRSPORT)
									{
										EventFileHead->REPORT|=2;//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
									}
									else
									{
										EventFileHead->REPORT|=4;//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
									}
									if(ACK)
									{
										EventFileHead->ACK=ACK;//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
									}
								}
								EventFileHead++;
								z++;
							}
						}
						ms->msfile_write(filename,0,(u8*)ADDR_128KDATABUFF,y*sizeof(EventFileHead_TypeDef));
					}
				}
			}
		}
		UARTCtrl->HostACK=0;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
	}
}


void EVENT_REPORT_Notification(u32 PORTn)//�ϱ�֪ͨ(�¼�)
{
	u32 i;
	u32 x;
	u32 n;
	u32 fileid;
	u32 OI;
	u32 LEN_Tx;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;//��󻹿ɷ��͵��ֽ���
	u8* p8;
//	u32 FILECOUNT;
	u32 FILE_OFFSET;
	u32 fDATA;//�����ݱ�־
	u32 NUM_READFILE;//���ļ��ܴ���
	u16* p16time;
	u8* p8tx;
	UARTCtrl_TypeDef *UARTCtrl;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//�ͻ���������Ϣ
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	EventFileHead_TypeDef* EventFileHead;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 NUM_RCSD;
	u32 LEN_RCSD;
	u8* pRCSD;
	u32 NUMmax_DATA;
	#if(USER/100)==15//����
	u8* p8s;
	u8* p8SOUR;
	u16* p16s;
	u8* p8Data;
	u32 j;
	u32 len;
	u32 Len;
	u8* p8t;
	DL698_SETLIB_TypeDef *pLib;
	#endif

#define TEST_REPORT_PORTn_RS232     0//0=��,1=��;�������ϱ���ΪRS232
#if TEST_REPORT_PORTn_RS232==0
	if((PORTn!=GPRSPORT)&&(PORTn!=ENETPORT))
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Task!=0)
	{
		return;
	}
	if(UARTCtrl->LinkTask!=101)
	{//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼,104=�ػ�
		return;
	}
#else
//---��RS232�ڲ����ϱ�---
	if(PORTn!=RS232PORT)
	{
		return;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Task!=0)
	{
		return;
	}
//---��RS232�ڲ����ϱ� END---
#endif
	
	if((UARTCtrl->HostACK)&&(UARTCtrl->HostACK!=4))//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�
	{//�����������ϱ��ȴ�ȷ�Ϸ���
		return;
	}
	i=Comm_Ram->DL698YMDHMS[6];
	if(UARTCtrl->REPORT_RTCS==i)//����ʱ�����ϱ���RTC��
	{
		return;
	}
	if((i&1)==0)
	{
		return;//��ǰʱ��������,�������¼�,ż������ʱ����
	}
	UARTCtrl->REPORT_RTCS=i;//����ʱ�����ϱ���RTC��
	i=MRR(ADDR_4300_8+1,1);//���������ϱ�
	if(i==0)
	{//�����������ϱ�
		UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
		//���������ϱ����ļ������ϱ���������
		/*
		Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯(1=�������ݱ���,2=���������ϱ�����)
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
		{
			if(filename==recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			}
			if(file->BLOCK_ENTRY)
			{
				
			}
			file++;
		}
		*/
		return;
	}
	switch(PORTn)
	{
		case GPRSPORT:
			p16time=(u16*)&Terminal_Ram->GPRS_WaitHostACK_S_Timer;//29 GPRS�����ϱ��ȴ���վȷ���붨ʱ��
			break;
		case ENETPORT:
			p16time=(u16*)&Terminal_Ram->ENET_WaitHostACK_S_Timer;//30 ENET�����ϱ��ȴ���վȷ���붨ʱ��
			break;
		default:
		case RS232PORT:
			p16time=(u16*)&Terminal_Ram->RS232_WaitHostACK_S_Timer;//31 RS232�����ϱ��ȴ���վȷ���붨ʱ��
			break;
	}
	if(p16time[0]!=0)
	{//�ϱ���Ӧ��ʱʱ��û��
		return;
	}
	i=UARTCtrl->REPORT_NUMmax;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
	if(i)
	{
		i--;
		UARTCtrl->REPORT_NUMmax=i;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
		p16time[0]=UARTCtrl->REPORT_OVER_SECOND;//�ϱ���Ӧ��ʱʱ��Ĵ�
		//����֡������DataBuff+LEN_UARTnTx,�����ط�
		p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
		p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
		i=Get_LEN_UARTnTx(PORTn);
		p8+=i;
		LEN_Tx=p8[1]|(p8[2]<<8);
		LEN_Tx+=2;
		if(LEN_Tx<=i)
		{
			MR((u32)p8tx,(u32)p8,LEN_Tx);
			UARTCtrl->TxByte=LEN_Tx;
			UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
		}
		return;
	}
	ms=(ms_Type *)Get_ms();
	EVENT_REPORT_complete(PORTn,0);//�����ϱ���ɺ���(�¼�),���:ACK=0ûȷ��,1=ȷ��
	
//��������
	if(PORTn==GPRSPORT)
	{
		if(Terminal_Ram->GPRSHeartBeat_S_Timer==0)//GPRS�����붨ʱ��
		{
			return;
		}
	}
	if(PORTn==ENETPORT)
	{
		if(Terminal_Ram->ENETHeartBeat_S_Timer==0)//��̫�������붨ʱ��
		{
			return;
		}
	}
//
	fDATA=0;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
	NUM_READFILE=0;//���ļ��ܴ���
	UARTCtrl->REPORT_NUM_File=0;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(UARTCtrl->REPORT_NUM_File>=REPORT_NUMFILEmax)//�ϱ����ݵĲ�ͬ�ļ������
		{
			break;
		}
		if(fDATA==2)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
		{
			break;
		}
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			fileid=file->FILEID;
			if((fileid&0xff)==2)
			{//�ļ�����=2���¼���¼
				x=file->NUM_DATA;
			#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
				if(PORTn==GPRSPORT)
				{
					n=file->NUM_GPRSreport[0];
				}
				else
				{
					n=file->NUM_ENETreport[0];
				}
			#else
				n=file->NUM_GPRSorENETreport[0];
			#endif
				if(x>n)
				{//���¼����ϱ�
					i=x*sizeof(EventFileHead_TypeDef);
					if(i<=LEN_128KDATABUFF)
					{
						if(NUM_READFILE>127)//���ļ��ܴ���
						{//���ļ��ܴ���������ÿ������ʱ��
							break;
						}
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,i);
						NUM_READFILE++;//���ļ��ܴ���
						EventFileHead=(EventFileHead_TypeDef*)(ADDR_128KDATABUFF);
						FILE_OFFSET=0;//�������ݵ�ַ
						for(i=0;i<n;i++)
						{
							FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
							EventFileHead++;
						}
						i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
						if(i>=REPORT_NUMFILEmax)//�ϱ����ݵĲ�ͬ�ļ������
						{
							break;
						}
						UARTCtrl->REPORT_filename[i]=filename;//1֡�����ϱ����ݲ�ͬ�ļ����б�
						UARTCtrl->REPORT_NUM_Data[i]=0;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
						i++;
						UARTCtrl->REPORT_NUM_File=i;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
						while(1)
						{
							i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
							if(i)
							{
								i--;
							}
							n=UARTCtrl->REPORT_NUM_Data[i];
						#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
							if(PORTn==GPRSPORT)
							{
								n+=file->NUM_GPRSreport[0];
							}
							else
							{
								n+=file->NUM_ENETreport[0];
							}
						#else
							n+=file->NUM_GPRSorENETreport[0];
						#endif
							if(file->NUM_DATA<=n)
							{//û�������ϱ�
								break;
							}
							if(EventFileHead->REPORT&1)//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
							{//���ϱ�
								if(fDATA==0)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
								{
									p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
									LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
									LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
								#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
									ClientConnectInfo=ClientConnectInfo;
								#else
									//�ͻ������ջ��峤��
									ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
									i=ClientConnectInfo->LEN_Rx;
									i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
									if(i<2048)
									{
										i=2048;
									}
									if(i>(TxAPDUOffset+1))
									{//��Ϊ0
										i-=(TxAPDUOffset+1);
										if(LENmax_TxSPACE>i)
										{
											LENmax_TxSPACE=i;
										}
									}
								#endif
//APDU:88 02 01 
//01  SEQUENCE OF OAD������=?
//31 04 02 00  OAD
//06 OAD����
//00 20 22 02 00 00 20 1e 02 00 00 20 20 02 00 00 20 24 02 00 00 33 00 02 00 00 f2 03 42 01  OAD�б�
//01 Get-Result��=CHOICE, [1]   DATA
//01 ��¼��m						
//06 00 00 00 31 1c 07 e1 03 10 0c 18 00 1c 07 e1 03 10 0c 18 00 00 01 02 02 02 51 45 00 00 00 16 00 02 02 51 45 10 00 00 16 00 02 02 11 00 11 00 00 00
									p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
									p8tx[0]=0x88;//�ϱ�֪ͨ REPORT-Notification
									p8tx[1]=0x02;//�ϱ����ɸ���������         [1] ReportNotificationList
															//�ϱ����ɸ���¼�Ͷ�������   [2] ReportNotificationRecordList
									i=UARTCtrl->REPORT_PIID;
									i++;
									i&=0x3f;
									UARTCtrl->REPORT_PIID=i;
									p8tx[2]=i;//PIID
									p8tx[3]=0;//SEQUENCE OF OAD������=?
									LEN_TxAPDU=4;
									LENmax_TxSPACE-=LEN_TxAPDU+2;//����2byte(1byteʱ���ǩ,1byte�ϱ���ϢFollowReport OPTIONAL��
								}
								//�¼�OAD
								if(LENmax_TxSPACE<5)
								{
									if(fDATA)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
									{
										fDATA=2;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
										break;
									}
									//�������ݴ��ڻ��峤�ȶ����������ϱ�
									i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
									if(i)
									{
										i--;
									}
									UARTCtrl->REPORT_NUM_Data[i]++;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
									FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
									EventFileHead++;
									continue;
								}
								OI=file->FILEID;
								NUMmax_DATA=DEPTH_EventFile(OI>>16);//�¼��ļ��洢���;����:�洢���
								OI>>=8;
								p8tx[LEN_TxAPDU+0]=OI>>16;
								p8tx[LEN_TxAPDU+1]=OI>>8;
								p8tx[LEN_TxAPDU+2]=OI;
								p8tx[LEN_TxAPDU+3]=0x00;
								p8tx[LEN_TxAPDU+4]=0;//OAD����=?
								LEN_Tx=5;
								LENmax_TxSPACE-=5;
								//OAD�б�
								i=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								ms->msfile_read(filename,((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef)),i);
								i=EventFileHead->LEN_event[0]+(EventFileHead->LEN_event[1]<<8);
								p8=(u8*)ADDR_128KDATABUFF+((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+i;
								n=p8[1];
								p8tx[LEN_TxAPDU+4]=n;//OAD����=?
								n*=5;
								if(LENmax_TxSPACE<(n+2))
								{
									if(fDATA)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
									{
										fDATA=2;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
										break;
									}
									//�������ݴ��ڻ��峤�ȶ����������ϱ�
									i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
									if(i)
									{
										i--;
									}
									UARTCtrl->REPORT_NUM_Data[i]++;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
									FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
									EventFileHead++;
									continue;
								}
								MR((u32)p8tx+LEN_TxAPDU+LEN_Tx,(u32)p8+2,n);
								p8tx[LEN_TxAPDU+LEN_Tx+n]=1;//Get-Result��=CHOICE, [1]   DATA
								p8tx[LEN_TxAPDU+LEN_Tx+n+1]=1;//��¼��m
								LEN_Tx+=n+2;
								LENmax_TxSPACE-=n+2;
								
								i=EventFileHead->LEN_event[0]+(EventFileHead->LEN_event[1]<<8);
								i-=2;
								if(LENmax_TxSPACE<i)
								{
									if(fDATA)//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
									{
										fDATA=2;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
										break;
									}
									//���ݴ������������,�������ݴ��ڻ��峤�ȶ����������ϱ�
									i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
									if(i)
									{
										i--;
									}
									UARTCtrl->REPORT_NUM_Data[i]++;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
									FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
									EventFileHead++;
									continue;
								}
								MR((u32)p8tx+LEN_TxAPDU+LEN_Tx,ADDR_128KDATABUFF+((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+2,i);
								LEN_Tx+=i;
								LENmax_TxSPACE-=i;
								p8tx[3]++;//SEQUENCE OF OAD������=?
								LEN_TxAPDU+=LEN_Tx;
								#if(USER/100)==15//����
								if(OI==0x00310602)
								{
									p8t=Get_Element(p8tx+LEN_TxAPDU-i,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
									if(p8t==0)
										return;
								}
								if(OI==0x00310602&&p8t[1]==0)//ͣ���¼��ϱ����ݴ���
								{
									LEN_Tx=0;
									Len=0;
									for(i=0;i<NUMmax_6012_2;i++)
									{
										p8s=(u8*)ADDR_6012_2_SDRAM+(i*LENper_6012_2);//�������ñ�
										if(p8s[0]==DataType_structure)
										{
											if(p8s[9]==1)
											{//��ͨ�ɼ�����
												pLib=Get_DL698_SET_pLib(0x31060B00);//�õ�DATA_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
												if(pLib==0)
													break;
												MR(ADDR_DATABUFF,pLib->Addr,4);
												p8t=(u8*)ADDR_DATABUFF;
												if(p8s[11]==p8t[3])
												{//�������
													p8SOUR=(u8*)ADDR_6000_SDRAM;
													for(i=0;i<NUMmax_6000;i++)
													{
														if(p8SOUR[0]==DataType_structure)
														{
															p16s=(u16*)(p8SOUR+10);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
															len=Get_LastRecordFileDataHasID(54,p16s);
															if(len!=0)//�����ݼ�¼�����и�TSA������
															{
																LEN_RCSD=0;
																if(Len==0)
																{
																	p8tx[LEN_TxAPDU+0]=0x60;
																	p8tx[LEN_TxAPDU+1]=0x12;
																	p8tx[LEN_TxAPDU+2]=0x03;
																	p8tx[LEN_TxAPDU+3]=0x00;
																	
																	p8Data=(u8*)ADDR_128KDATABUFF;
																	NUM_RCSD = p8Data[22];
																	p8tx[LEN_TxAPDU+4]=NUM_RCSD+1;
																	j=0;
																	for(n=0;n<NUM_RCSD;n++)
																	{
																		x=5;
																		MR((u32)p8tx+LEN_TxAPDU+5+LEN_RCSD,(u32)p8Data+j+24,x);
																		LEN_RCSD+=x;
																		j+=x+1;
																	}
																	p8tx[LEN_TxAPDU+5+LEN_RCSD]=0;//Ŀ���������ַOAD
																	p8tx[LEN_TxAPDU+6+LEN_RCSD]=0x20;
																	p8tx[LEN_TxAPDU+7+LEN_RCSD]=0x2A;
																	p8tx[LEN_TxAPDU+8+LEN_RCSD]=0x02;
																	p8tx[LEN_TxAPDU+9+LEN_RCSD]=0x00;
																	LEN_Tx+=10+LEN_RCSD;
																	LEN_TxAPDU+=10+LEN_RCSD;
																	LENmax_TxSPACE-=10+LEN_RCSD;
																	p8tx[LEN_TxAPDU]=1;
																	p8tx[LEN_TxAPDU+1]=0;
																}
																len-=23+j;
																p8tx[LEN_TxAPDU+1]++;									
																MR((u32)p8tx+LEN_TxAPDU+2+Len,(u32)p8Data+23+j,len);														
																p8tx[LEN_TxAPDU+2+len+Len]=0x55;//Ŀ���������ַ����
																p8tx[LEN_TxAPDU+3+len+Len]=0x07;
																p8tx[LEN_TxAPDU+4+len+Len]=0x05;
																MR((u32)p8tx+LEN_TxAPDU+5+len+Len,(u32)p16s,6);
																Len+=9+len;
															}
														}
														p8SOUR+=LENper_6000;
													}
												}
											}
										}
									}
									if(Len>0)
									{
										p8tx[3]++;
										LEN_Tx+=Len+2;
										LENmax_TxSPACE-=Len+2;
										LEN_TxAPDU+=Len+2;
									}
								}
								#endif
								fDATA=1;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
							}	
							i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
							if(i)
							{
								i--;
							}
							UARTCtrl->REPORT_NUM_Data[i]++;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
							FILE_OFFSET+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
							EventFileHead++;
							if(fDATA)
							{
								break;//�ļ����¼����ϱ�,��ͬ���ݼ�¼֡��ʽ��ͬ!
							}
						}
					}
				}
			}
	//���ܱ�ȫ�¼��ɼ��ϱ�
			if((fileid>>28)==2)
			{//�¼��ɼ�����
				x=file->NUM_DATA;
			#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
				if(PORTn==GPRSPORT)
				{
					n=file->NUM_GPRSreport[0];
				}
				else
				{
					n=file->NUM_ENETreport[0];
				}
			#else
				n=file->NUM_GPRSorENETreport[0];
			#endif
				if(x>n)
				{//���¼����ϱ�
					i=x*sizeof(RecordFileHead_TypeDef);
					if(i<=LEN_128KDATABUFF)
					{
						if(NUM_READFILE>127)//���ļ��ܴ���
						{//���ļ��ܴ���������ÿ������ʱ��
							break;
						}
						NUM_READFILE++;//���ļ��ܴ���
						i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
						if(i>=REPORT_NUMFILEmax)//�ϱ����ݵĲ�ͬ�ļ������
						{
							break;
						}
						UARTCtrl->REPORT_filename[i]=filename;//1֡�����ϱ����ݲ�ͬ�ļ����б�
						UARTCtrl->REPORT_NUM_Data[i]=0;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
						i++;
						UARTCtrl->REPORT_NUM_File=i;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���

						//����¼ͷ
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,x*sizeof(RecordFileHead_TypeDef));
						RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
						FILE_OFFSET=0;//�������ݵ�ַ
						for(i=0;i<n;i++)
						{
							FILE_OFFSET+=RecordFileHead->LEN_DATA;
							RecordFileHead++;
						}
						//ȡ��¼����
						i=RecordFileHead->LEN_DATA;
						ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+(64*1024),i);
	//TxAPDU��
	//88 �ϱ�֪ͨ
	//02 �ϱ����ɸ���¼�Ͷ�������
	//02 PIID
	//01 SEQUENCE OF A-ResultNormal
	//60 12 03 00  OAD
	//01 RCSD��SEQUENCE OF����=1
	//01 50 04 02 00 02 00 10 02 00 00 20 02 00 //RCSD
	//01 ��¼����
	//02 M����¼��M=2
	//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
	//01 02 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 01 05 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 06 00 00 00 00 
	//00 00 
						p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
						LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
						LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
					#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
						ClientConnectInfo=ClientConnectInfo;
					#else
						//�ͻ������ջ��峤��
						ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
						i=ClientConnectInfo->LEN_Rx;
						i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
						if(i<2048)
						{
							i=2048;
						}
						if(i>(TxAPDUOffset+1))
						{//��Ϊ0
							i-=(TxAPDUOffset+1);
							if(LENmax_TxSPACE>i)
							{
								LENmax_TxSPACE=i;
							}
						}
					#endif
						p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
						p8tx[0]=0x88;//�ϱ�֪ͨ REPORT-Notification
						p8tx[1]=0x02;//�ϱ����ɸ���������[1] ReportNotificationList,�ϱ����ɸ���¼�Ͷ�������   [2] ReportNotificationRecordList
						i=UARTCtrl->REPORT_PIID;
						i++;
						i&=0x3f;
						UARTCtrl->REPORT_PIID=i;
						p8tx[2]=i;//PIID
						p8tx[3]=1;//SEQUENCE OF A-ResultNormal
						p8tx[4]=0x60;
						p8tx[5]=0x12;
						p8tx[6]=0x03;
						p8tx[7]=0x00;
						
						LEN_RCSD=0;//�����͵�RCSD����
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x60;//0x60400200://�ɼ�����ʱ��
						p8tx[11+LEN_RCSD]=0x40;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x60;//0x60410200://�ɼ��ɹ�ʱ��
						p8tx[11+LEN_RCSD]=0x41;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x60;//0x60420200://�ɼ��洢ʱ��
						p8tx[11+LEN_RCSD]=0x42;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;
						p8tx[9+LEN_RCSD]=0;//OAD
						p8tx[10+LEN_RCSD]=0x20;//0x202A0200://Ŀ���������ַ
						p8tx[11+LEN_RCSD]=0x2A;
						p8tx[12+LEN_RCSD]=0x02;
						p8tx[13+LEN_RCSD]=0x00;
						LEN_RCSD+=5;

						pRCSD=(u8*)ADDR_128KDATABUFF+(64*1024)+21;
						NUM_RCSD=pRCSD[1];//RCSD��SEQUENCE OF����
						p8tx[8]=NUM_RCSD+(LEN_RCSD/5);//+Ŀ���������ַ��
						//�ļ���������RCSDתΪ�����͵�RCSD
						i=2;//�ļ��������͵�RCSD����
						for(n=0;n<NUM_RCSD;n++)
						{
							if(pRCSD[i+1]==0)
							{//OAD
								x=5;
								MR((u32)p8tx+9+LEN_RCSD,(u32)pRCSD+i+1,x);
								LEN_RCSD+=x;
								i+=x+1;
							}
							else
							{//ROAD
								x=pRCSD[i+6];
								x*=4;
								x+=6;
								MR((u32)p8tx+9+LEN_RCSD,(u32)pRCSD+i+1,x);
								LEN_RCSD+=x;
								i+=x+1;
							}
						}
						p8tx[9+LEN_RCSD]=1;//��¼����
						p8tx[10+LEN_RCSD]=0;//M����¼
						
						LEN_TxAPDU=11+LEN_RCSD;
						LENmax_TxSPACE-=LEN_TxAPDU+2;//����2byte(1byteʱ���ǩ,1byte�ϱ���ϢFollowReport OPTIONAL��	
					#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
						if(PORTn==GPRSPORT)
						{
							n=file->NUM_GPRSreport[0];
						}
						else
						{
							n=file->NUM_ENETreport[0];
						}
					#else
						n=file->NUM_GPRSorENETreport[0];
					#endif
						p8=(u8*)(ADDR_128KDATABUFF+(n*sizeof(RecordFileHead_TypeDef)));//���ݵĵ��ܱ��ַ
						i=Get_RecordFileData_RCSD(p8tx+8,p8,(u8*)ADDR_128KDATABUFF+(64*1024),(u8*)ADDR_DATABUFF,LEN_DATABUFF,0);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
						if(i)
						{
							if(i<=LENmax_TxSPACE)
							{
								MR((u32)p8tx+LEN_TxAPDU,ADDR_DATABUFF,i);
								LEN_TxAPDU+=i;
								LENmax_TxSPACE-=i;
								p8tx[10+LEN_RCSD]++;//M����¼
								fDATA=1;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
							}
						}
						i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
						if(i)
						{
							i--;
						}
						UARTCtrl->REPORT_NUM_Data[i]++;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
						if(fDATA)
						{
							break;//�ļ����¼����ϱ�,��ͬ���ݼ�¼֡��ʽ��ͬ!
						}

					}
				}
			}
	//DL645���ܱ������ϱ�
			if((fileid&0xff)==5)
			{//�ļ�����=5
				x=file->NUM_DATA;
			#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
				if(PORTn==GPRSPORT)
				{
					n=file->NUM_GPRSreport[0];
				}
				else
				{
					n=file->NUM_ENETreport[0];
				}
			#else
				n=file->NUM_GPRSorENETreport[0];
			#endif
				if(x>n)
				{//���¼����ϱ�
					i=x*sizeof(RecordFileHead_TypeDef);
					if(i<=LEN_128KDATABUFF)
					{
						if(NUM_READFILE>127)//���ļ��ܴ���
						{//���ļ��ܴ���������ÿ������ʱ��
							break;
						}
						NUM_READFILE++;//���ļ��ܴ���
						i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
						if(i>=REPORT_NUMFILEmax)//�ϱ����ݵĲ�ͬ�ļ������
						{
							break;
						}
						UARTCtrl->REPORT_filename[i]=filename;//1֡�����ϱ����ݲ�ͬ�ļ����б�
						UARTCtrl->REPORT_NUM_Data[i]=0;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
						i++;
						UARTCtrl->REPORT_NUM_File=i;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���

						//����¼ͷ
						ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,x*sizeof(RecordFileHead_TypeDef));
						RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
						FILE_OFFSET=0;//�������ݵ�ַ
						for(i=0;i<n;i++)
						{
							FILE_OFFSET+=RecordFileHead->LEN_DATA;
							RecordFileHead++;
						}
						//ȡ��¼����
						i=RecordFileHead->LEN_DATA;
						ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+FILE_OFFSET,(u8*)ADDR_128KDATABUFF+(64*1024),i);
	//ReportNotificationTransData��=SEQUENCE 
	//{ 
	//	�������-���ȼ�-ACD PIID-ACD�� 
	//	������Դ�˿ں� OAD�� 
	//	͸������ SEQUENCE OF octet-string 
	//}
	//TxAPDU��
	//88 �ϱ�֪ͨ
	//03 �ϱ�͸������ [3] ReportNotificationTransData
	//02 �������-���ȼ�-ACD PIID-ACD
	//F2 01 02 01  ������Դ�˿ں� OAD
	//01 ͸������ SEQUENCE OF octet-string 
						p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
						LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//��󻹿ɷ��͵��ֽ���
						LENmax_TxSPACE-=TxAPDUOffset+1;//1byte�ϱ���ϢFollowReport OPTIONAL=0 ��ʾû��
					#if USE_ClientConnectInfo==0//ʹ�ÿͻ���������Ϣ�Ŀͻ����������ֽ�:0=��ʹ��,1=ʹ��
						ClientConnectInfo=ClientConnectInfo;
					#else
						//�ͻ������ջ��峤��
						ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//�ͻ���������Ϣ
						i=ClientConnectInfo->LEN_Rx;
						i=R_Inverse(i,2);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
						if(i<2048)
						{
							i=2048;
						}
						if(i>(TxAPDUOffset+1))
						{//��Ϊ0
							i-=(TxAPDUOffset+1);
							if(LENmax_TxSPACE>i)
							{
								LENmax_TxSPACE=i;
							}
						}
					#endif
						p8tx+=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
						p8tx[0]=0x88;//�ϱ�֪ͨ REPORT-Notification
						p8tx[1]=0x03;//�ϱ�͸������ [3] ReportNotificationTransData
						i=UARTCtrl->REPORT_PIID;
						i++;
						i&=0x3f;
						UARTCtrl->REPORT_PIID=i;
						p8tx[2]=i;//PIID
						LEN_TxAPDU=3;
						LENmax_TxSPACE-=LEN_TxAPDU+2;//����2byte(1byteʱ���ǩ,1byte�ϱ���ϢFollowReport OPTIONAL��	
						
						i=RecordFileHead->LEN_DATA;
						if(i<=LENmax_TxSPACE)
						{
							MR((u32)p8tx+LEN_TxAPDU,ADDR_128KDATABUFF+(64*1024),i);
							LEN_TxAPDU+=i;
							LENmax_TxSPACE-=i;
							fDATA=1;//�����ݱ�־;0=û,1=��,2=��ʾ���ͻ��岻��
						}

						i=UARTCtrl->REPORT_NUM_File;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
						if(i)
						{
							i--;
						}
						UARTCtrl->REPORT_NUM_Data[i]++;//1֡�����ϱ����ݲ�ͬ�ļ������Ѵ������ݸ����б�,����û�κ���ѡ�еĸ���
						if(fDATA)
						{
							break;//�ļ����¼����ϱ�,��ͬ���ݼ�¼֡��ʽ��ͬ!
						}
					}
				}
			}
			//
			
		}
		file++;
	}
	if(fDATA==0)//�����ݱ�־
	{//û�ϱ�����
		if(UARTCtrl->REPORT_NUM_File)//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
		{
			//�ļ����Ѵ���������ݸ���
			UARTCtrl->HostACK=4;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
			EVENT_REPORT_complete(PORTn,0);//�����ϱ���ɺ���(�¼�),���:ACK=0ûȷ��,1=ȷ��
		}
		return;
	}		
	p8tx[LEN_TxAPDU+0]=0;//ʱ���ǩ
	p8tx[LEN_TxAPDU+1]=0;//�ϱ���ϢFollowReport OPTIONAL
	LEN_TxAPDU+=2;
			
	i=MRR(ADDR_F101_2+1,1);//��ȡ��ȫģʽ����
	if(i==0)
	{//0�������ð�ȫģʽ������Ĭ�ϰ�ȫģʽ����
		p8tx-=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
		p8tx[0]=0x68;
		//2byte������L
		p8tx[3]=0x83;//������C
		p8tx[4]=5;//TSA
		MR((u32)p8tx+5,ADDR_4001+2,6);
		Data_Inverse((u32)p8tx+5,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
		p8tx[11]=0;//�ͻ�����ַCA
		//2byte֡ͷУ��HCS
		MR((u32)p8tx+14,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
		//2byteУ��
		LEN_Tx=14+LEN_TxAPDU+2;
		p8tx[LEN_Tx]=0x16;
		LEN_Tx++;
		UARTCtrl->TxByte=LEN_Tx;
		LEN_Tx-=2;
		p8tx[1]=LEN_Tx;
		p8tx[2]=LEN_Tx>>8;
	}
	else
	{//1�����ð�ȫģʽ������Ĭ�ϰ�ȫģʽ����
		p8tx-=TxAPDUOffset;//TxAPDU��ſ�ʼ��ַ
		p8tx[0]=0x68;
		//2byte������L
		p8tx[3]=0x83;//������C
		p8tx[4]=5;//TSA
		MR((u32)p8tx+5,ADDR_4001+2,6);
		Data_Inverse((u32)p8tx+5,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
		p8tx[11]=0;//�ͻ�����ַCA
		//2byte֡ͷУ��HCS
		p8tx[14]=0x10;//0x10,0x00,0x13,//��ȫ����
		p8tx[15]=0x00;//��ȫ���
		//���ĳ���
		if(LEN_TxAPDU<127)
		{
			p8tx[16]=LEN_TxAPDU;
			LEN_Tx=17;
		}
		else
		{
			p8tx[16]=0x82;
			p8tx[17]=LEN_TxAPDU>>8;
			p8tx[18]=LEN_TxAPDU;
			LEN_Tx=19;
		}
		i=TESAM_CMD_TxRx(0x80140103,LEN_TxAPDU,(u32)p8tx+TxAPDUOffset);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
		if(i)
		{//ESAM��֤ʧ��
			UARTCtrl->REPORT_NUM_File=0;//1֡�����ϱ��������Բ�ͬ�ļ��ĸ���
			return;
		}
		MR((u32)p8tx+LEN_Tx,(u32)p8tx+TxAPDUOffset,LEN_TxAPDU);
		LEN_Tx+=LEN_TxAPDU;
		//02 0C 82 07 EF 22 99 AD 50 09 00 00 00 0F 04 FB 43 41 D0
		p8tx[LEN_Tx]=2;//�����+����MAC  [2]  RN_MAC
		p8tx[LEN_Tx+1]=12;
		MR((u32)p8tx+LEN_Tx+2,ADDR_UART7Rx+5,12);
		p8tx[LEN_Tx+2+12]=4;
		MR((u32)p8tx+LEN_Tx+15,ADDR_UART7Rx+5+12,4);
		LEN_Tx+=19;
		LEN_Tx+=3;//2byteУ��,1byte 0x16
		UARTCtrl->TxByte=LEN_Tx;
		LEN_Tx-=2;
		p8tx[1]=LEN_Tx;
		p8tx[2]=LEN_Tx>>8;
	}
	
	Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
	UARTCtrl->Task=2;//2=�ȴ���֡���ݷ���
	//����֡������DataBuff+LEN_UARTnTx,�����ط�
	i=Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
	i+=Get_LEN_UARTnTx(PORTn);
	MW((u32)p8tx,i,LEN_Tx+2);
	//�ϱ���Ӧ��ʱʱ��  TI
	p16time[0]=WaitHostACKMinTimer;//Ĭ����Сֵ
	UARTCtrl->REPORT_OVER_SECOND=WaitHostACKMinTimer;//�ϱ���Ӧ��ʱʱ��Ĵ�
	UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
	UARTCtrl->HostACK=4;//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
	if(PORTn==GPRSPORT)
	{//����ͨ��ģ��1
		p8=Get_Element((u8 *)ADDR_4500_2,11,0,LENmax_4500_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
		
	}
	else
	{//��̫��ͨ��ģ��1,������
		p8=Get_Element((u8 *)ADDR_4510_2,7,0,LENmax_4510_2);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
	}
	if(p8)
	{
		x=p8[1];//bit7~bit2����ʱʱ�䣨�룩��bit1~bit0���ط�����
		i=x>>2;
		if(i>WaitHostACKMaxTimer)
		{
			i=WaitHostACKMaxTimer;
		}
		if(i<WaitHostACKMinTimer)
		{
			i=WaitHostACKMinTimer;
		}
		p16time[0]=i;
		UARTCtrl->REPORT_OVER_SECOND=i;//�ϱ���Ӧ��ʱʱ��Ĵ�
		//����ϱ�����
		i=x&0x3;
		if(i>=1)
		{
			i--;
			UARTCtrl->REPORT_NUMmax=i;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
			if(i==0)
			{
				p16time[0]=0;
			}
		}
	}
	
//u32 Event_3106(u8* pOccurSour,u32 Occur);//�ն�ͣ/�ϵ��¼���¼;���:f=���Ա�־,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
//Event_3106(0,1);//�ն�ͣ/�ϵ��¼���¼
	
	
}


//REPORT-Response��=CHOICE
//{
//�ϱ����ɸ��������Ե���Ӧ      	 [1] ReportResponseList��
//�ϱ����ɸ���¼�Ͷ������Ե���Ӧ	 [2] ReportResponseRecordList
//}
void REPORT_Response(u32 PORTn)//�ϱ�Ӧ��
{
	u32 LEN_SA;
	u8* p8rx;
	UARTCtrl_TypeDef *UARTCtrl;
	u16 *p16time;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_SA=p8rx[4];//ȡ��ַ�ֽ���
	LEN_SA&=0x0f;
	LEN_SA++;
	switch(p8rx[LEN_SA+8+1])
	{
		case 1://�ϱ����ɸ��������Ե���Ӧ      	 [1] ReportResponseList

			break;
		case 2://�ϱ����ɸ���¼�Ͷ������Ե���Ӧ	 [2] ReportResponseRecordList
			
			break;
	}
	switch(PORTn)
	{
		case GPRSPORT:
			p16time=(u16*)&Terminal_Ram->GPRS_WaitHostACK_S_Timer;//29 GPRS�����ϱ��ȴ���վȷ���붨ʱ��
			break;
		case ENETPORT:
			p16time=(u16*)&Terminal_Ram->ENET_WaitHostACK_S_Timer;//30 ENET�����ϱ��ȴ���վȷ���붨ʱ��
			break;
		default:
		case RS232PORT:
			p16time=(u16*)&Terminal_Ram->RS232_WaitHostACK_S_Timer;//31 RS232�����ϱ��ȴ���վȷ���붨ʱ��
			break;
	}
	if(UARTCtrl->HostACK==4)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
	{
		p16time[0]=0;
		UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
		UARTCtrl->REPORT_RTCS=0xff;//����ʱ�����ϱ���RTC��
		EVENT_REPORT_complete(PORTn,1);//�����ϱ���ɺ���(�¼�),���:ACK=0ûȷ��,1=ȷ��
	}
	if(UARTCtrl->HostACK==5)//�����ϱ�����վȷ��֡���:0=��,1=��¼֡,2=�˳���¼֡,3=����֡,4=��Ҫ�¼�,5=��ʱ����,6=���ܱ��¼�,7=�ն�������վʱ��
	{
		p16time[0]=0;
		UARTCtrl->REPORT_NUMmax=0;//����վûӦ��������ط��ϱ�����,ÿ�ϱ�1�μ�1
		UARTCtrl->REPORT_RTCS=0xff;//����ʱ�����ϱ���RTC��
		REPORT_complete(PORTn);//�����ϱ���ɺ���(��ʱ����)
	}

}








