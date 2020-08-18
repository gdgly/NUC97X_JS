
//�¼���¼�ļ�
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/RM_Record.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Device/MEMRW.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_REPORT.h"




#if CompareMeterEventCount==1//ȫ�¼��ɼ������Ƿ�ȽϷ�������:0=���Ƚ�(ȫ����������д���ļ�),1=�Ƚ�(ֻ�д����仯���¼�д���ļ�)
void RM_MeterEventAutoInc0x20220200(u32 PORTn)//���ܱ��¼�������ѡ�����޷�������(�¼���¼���)���Զ�����
{
	u32 i;
	u32 n;
	u32 OAD;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	n=UARTCtrl->RMCSD[6];
	if(n==0)
	{//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ����
		return;
	}
	if(n>((LEN_RMCSD-7)/4))
	{
		n=(LEN_RMCSD-7)/4;
	}
	for(i=0;i<n;i++)
	{
		OAD=UARTCtrl->RMCSD[7+(i*4)+0]<<24;
		OAD|=UARTCtrl->RMCSD[7+(i*4)+1]<<16;
		OAD|=UARTCtrl->RMCSD[7+(i*4)+2]<<8;
		OAD|=UARTCtrl->RMCSD[7+(i*4)+3]<<0;
		if(OAD==0x20220200)
		{
			break;
		}
	}
	if(i>=n)
	{//û�ҵ�
		if(n>=((LEN_RMCSD-7)/4))
		{
			n--;
		}
		else
		{
			UARTCtrl->RMCSD[6]=n+1;
		}
		UARTCtrl->RMCSD[7+(n*4)+0]=0x20;
		UARTCtrl->RMCSD[7+(n*4)+1]=0x22;
		UARTCtrl->RMCSD[7+(n*4)+2]=0x02;
		UARTCtrl->RMCSD[7+(n*4)+3]=0x00;
	}
}

u32 Get_RxDATA_DL698_MeterEventCount(u32 PORTn)//��698���¼���¼����֡��ȡ�����ܱ��¼���������(�¼���¼���);����:��������,û�ҵ�ʱ����0xffffffff
{
	u32 y;
	u8* p8;
	u32 NUM_RCSDrx;
	u32 OADrx;
	u8* p8rx;

	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	NUM_RCSDrx=p8rx[21];//RCSD��SEQUENCE OF����
	for(y=0;y<NUM_RCSDrx;y++)
	{
		if(p8rx[22+(y*5)+0]==0)
		{
			OADrx=p8rx[22+(y*5)+1]<<24;
			OADrx|=p8rx[22+(y*5)+2]<<16;
			OADrx|=p8rx[22+(y*5)+3]<<8;
			OADrx|=p8rx[22+(y*5)+4];
			if(OADrx==0x20220200)
			{
				break;
			}
		}
	}
	if(y>=NUM_RCSDrx)
	{//û�ҵ���ѡ��OAD
		return 0xffffffff;//����:��������,û�ҵ�ʱ����0xffffffff
	}
	else
	{//�ҵ���ѡ��OAD
		p8=p8rx+22+(5*NUM_RCSDrx)+1;
		if(p8[0]==0)
		{//��¼����=0
			return 0xffffffff;//����:��������,û�ҵ�ʱ����0xffffffff
		}
		p8++;
		while(y--)
		{
			p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		}
		if(p8[0]!=DataType_double_long_unsigned)
		{//��
			return 0xffffffff;//����:��������,û�ҵ�ʱ����0xffffffff
		}
		y=p8[1]<<24;
		y|=p8[2]<<16;
		y|=p8[3]<<8;
		y|=p8[4];
		return y;
	}
}


u32 CompSourMeterEventCount(u32 SetNo,u32 OAD,u32 Count)//�Ƚ�ԭ���ܱ��¼�����;���:SetNo=�������ú�(0��ʼ),Count=��ǰ����;����:0=��ʼֵ,1=������ͬ,2=������ͬ,ͬʱ�޸�ԭ����,ʹ��ADDR_DATABUFF
{
	u32 i;
	u32 n;
	u32 x;
	u16* p16;
	u32* p32;
	u8* p8s;
	u8* p8d;
	
	if(SetNo>=NUM_RMmax)
	{
		return 0;//����:0=��ʼֵ,1=������ͬ,2=������ͬ
	}
#if LENper_SourMeterEventCount>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount),LENper_SourMeterEventCount);
	p16=(u16*)(ADDR_DATABUFF+6);
	p32=(u32*)(ADDR_DATABUFF+8);
	p8s=(u8*)ADDR_6000_SDRAM+(SetNo*LENper_6000)+10;
	p8d=(u8*)ADDR_DATABUFF;
	for(i=0;i<6;i++)
	{
		if(p8s[i]!=p8d[i])
		{
			break;
		}
	}
	if(i<6)
	{//��ַ��ͬ,��ʼ��
		MR(ADDR_DATABUFF,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
		p16[0]=1;//�б����=1
		p32[0]=OAD;
		p32[1]=Count;
		MW(ADDR_DATABUFF,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount),16);
		return 2;//����:0=��ʼֵ,1=������ͬ,2=������ͬ
	}
	n=p16[0];
	if(n>=((LENper_SourMeterEventCount-8)/8))
	{
		n=0;
	}
	for(i=0;i<n;i++)
	{
		x=p32[0];
		if(x==OAD)
		{
			break;
		}
		p32+=2;
	}
	if(i>=n)
	{//û�ҵ���ͬOAD
		n++;
		p16[0]=n;
		p32[0]=OAD;
		p32[1]=Count;
		//�����б����
		MWR(n,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount)+6,2);
		//�����б�
		MW((u32)p32,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount)+6+2+(8*i),8);
		return 2;//����:0=��ʼֵ,1=������ͬ,2=������ͬ
	}
	//�ҵ���ͬOAD
	x=p32[1];
	if(x==Count)
	{
		return 1;//����:0=��ʼֵ,1=������ͬ,2=������ͬ
	}
	//�����¼�����ֵ
	MWR(Count,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount)+6+2+(8*i)+4,4);
	return 2;//����:0=��ʼֵ,1=������ͬ,2=������ͬ
}
#endif


u32 GET_EventFileName(u32 OAD,u32 FileCount)//�����¼���¼�ļ���:���:OAD,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
{
	u32 FileID;
	msFILE_TypeDef *file;
	u32 SearchCount;
	
	if(OAD!=0xffffffff)
	{
		OAD&=0xffffff00;
		OAD|=2;//�ļ�����
	}
	SearchCount=Comm_Ram->msFILESearchCount;
	if(SearchCount<recordFILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(SearchCount*sizeof(msFILE_TypeDef)));
	}
	else
	{
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((SearchCount-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	for(;SearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);SearchCount++)
	{
		if(SearchCount==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			//�ļ�����
			if((FileID&0xff)!=2)
			{
				file++;
				continue;
			}
			//�ļ�ID
			if(OAD!=0xffffffff)
			{
				if(FileID!=OAD)
				{
					file++;
					continue;
				}
			}
			//�ļ�����
			if(FileCount!=0xffffffff)//�ļ�����
			{
				if(FileCount!=file->FILECOUNT)
				{
					file++;
					continue;
				}
			}
			Comm_Ram->msFILESearchCount=SearchCount+1;//�ļ���������
			if(SearchCount>=recordFILENAMEmax_FLASH)
			{
				SearchCount-=recordFILENAMEmax_FLASH;
				SearchCount+=FILENAMEmax_FLASH;
			}
			return SearchCount;
		}
		file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//�ļ���������
	return 0xffffffff;
}


u32 DEPTH_EventFile(u32 OI)//�¼��ļ��洢���;���:OI;����:�洢���
{
	switch(OI)
	{
		case 0x3111://����δ֪���ܱ��¼�
		case 0x3112://��̨�����ܱ��¼�
		case 0x3030://ģ��ID����¼�
	#if ((3*NUM_RMmax)*LEN_EventFileHead)>LEN_128KDATABUFF
		#error
	#endif
			return NUM_RMmax;//��󳭱���
		default:
			return NUMmax_Event;//����¼���¼��
	}
}


u32 EventRecord(u32 OAD,u8 *pbuf,u32 byte,u32 Occur,u32 Report)//�¼���¼�ļ�;���:Report=0���ϱ�1=�ϱ�;����:д���ֽ���
{
	u32 i;
	u32 x;
	u32 y;
	u32 LEN_OAD;
	ms_Type *ms;
	u32 filename;
	msFILE_TypeDef *file;
	EventFileHead_TypeDef* EventFileHead;
	u32 NUMmax_DATA;
	
	ms=Get_ms();
	Comm_Ram->msFILESearchCount=0;
	NUMmax_DATA=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;���:OI;����:�洢���
	filename=GET_EventFileName(OAD,0);//�����¼���¼�ļ���:���:OAD,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	if(filename==0xffffffff)
	{//û�ҵ�
		while(1)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
			{
				if(file->BLOCK_ENTRY==0)
				{
					break;
				}
				file++;
			}
			if(filename>=recordFILENAMEmax_FLASH)
			{//û��
				i=AutoDeleteFile(0);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
				if(i)
				{//��ɾ��
					continue;
				}
				return 0;
			}
			break;
		}
	}
	else
	{//�ҵ�
		//������¼
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		i=file->LEN_DATA;//�ļ��ܳ���
		x=file->NUM_DATA;//�¼�����
		if((i>((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef)))&&(x!=0))
		{//�м�¼
			x--;
			x*=sizeof(EventFileHead_TypeDef);
			ms->msfile_read(filename,x,pbuf+3,4);
			EventFileHead=(EventFileHead_TypeDef*)(pbuf+3);
			x=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);//����¼���¼����
			if(i<x)
			{
				pbuf[3]=0;
				pbuf[4]=0;
				pbuf[5]=0;
				pbuf[6]=0;
			}
			else
			{
				//������¼�ļ�¼��
				i-=x;
				ms->msfile_read(filename,i+3,pbuf+3,4);
				//���ָ��¼�������¼�����ʱ��
				if(Occur==0)//�����ָ�:0=�ָ�,1=����,2=��
				{
					//ms->msfile_read(filename,i+2+5+1,pbuf+2+5+1,7);
					if(pbuf[23]==DataType_TSA)
					{//�¼��з���Դ=TSA
						i=GET_LastEventTime_Occur(OAD,pbuf+23,pbuf+2+5);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
						if(i==1)
						{//1byte������
							pbuf[2+5]=DataType_date_time_s;//�̶�����8byte������Ϊ��
						}
					}
					else
					{
						i=GET_LastEventTime_Occur(OAD,0,pbuf+2+5);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
						if(i==1)
						{//1byte������
							pbuf[2+5]=DataType_date_time_s;//�̶�����8byte������Ϊ��
						}
					}
				}
			}
		}
	}
	//�¼���¼��+1
	i=(pbuf[3]<<24)|(pbuf[4]<<16)|(pbuf[5]<<8)|(pbuf[6]<<0);
	i++;
	pbuf[3]=i>>24;
	pbuf[4]=i>>16;
	pbuf[5]=i>>8;
	pbuf[6]=i>>0;
	
	if(file->BLOCK_ENTRY==0)
	{//���ļ�
		ms->msfile_lenclear(filename);//��0�ļ���صĳ��ȴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
		file->FILEID=OAD|0x2;
		file->FILECOUNT=0;
		Get_date_time_s(file->save_date_time_s);//MW(ADDR_DL698YMDHMS,(u32)&file->date_time_s,7);//�ļ�����ʱ��
	}
//д�ļ�
	x=file->NUM_DATA;
	if(((x>=NUMmax_DATA)&&(Occur))||(x>=(2*NUMmax_DATA)))
	{//�������ļ���ÿ���ļ���¼����>=������,���ǲ����¼�,�ָ��¼����������ļ�
		Comm_Ram->msFILESearchCount=0;
		while(1)
		{
			filename=GET_EventFileName(OAD,0xffffffff);////�����¼���¼�ļ���:���:OAD,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename==0xffffffff)
			{
				break;
			}
			if(filename<FILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			if(file->FILECOUNT==0)
			{//�ļ�����+1
				file->FILECOUNT=1;
			}
			else
			{//ɾ��
				i=filename;
				if(i>=recordFILENAMEmax_FLASH)
				{
					i-=recordFILENAMEmax_FLASH;
					i+=FILENAMEmax_FLASH;
				}
				ms->msfile_Delete(i);
			}
		}
		//�ҿ��ļ�
		while(1)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
			{
				if(file->BLOCK_ENTRY==0)
				{
					break;
				}
				file++;
			}
			if(filename>=recordFILENAMEmax_FLASH)
			{//û��
				i=AutoDeleteFile(0);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
				if(i)
				{//��ɾ��
					continue;
				}
				return 0;
			}
			break;
		}
		if(file->BLOCK_ENTRY==0)
		{//���ļ�
			ms->msfile_lenclear(filename);//��0�ļ���صĳ��ȴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
			file->FILEID=OAD|0x2;
			file->FILECOUNT=0;
			MW(ADDR_DL698YMDHMS,(u32)&file->save_date_time_s,7);//�ļ�����ʱ��
		}
		x=0;//������
	}
	//д����
	i=file->LEN_DATA;
	if(i<((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef)))
	{
		i=((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef));
	}
	while(1)
	{
		y=ms->msfile_write(filename,i,pbuf,byte);
		if(y!=byte)
		{//д����
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y)
			{//��ɾ��
				continue;
			}
			return 0;
		}
		break;
	}
	i+=byte;
	//д�����������Ա��=array OAD,������ADDR_128KDATABUFF+96k
	pbuf=(u8*)ADDR_128KDATABUFF+(96*1024);
	LEN_OAD=pbuf[1];
	LEN_OAD*=5;
	LEN_OAD+=2;
	while(1)
	{
		y=ms->msfile_write(filename,i,pbuf,LEN_OAD);
		if(y!=LEN_OAD)
		{//д����
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y)
			{//��ɾ��
				continue;
			}
			return 0;
		}
		break;
	}
	//д����
	EventFileHead=(EventFileHead_TypeDef*)pbuf;
	EventFileHead->LEN_data[0]=(byte+LEN_OAD);
	EventFileHead->LEN_data[1]=(byte+LEN_OAD)>>8;
	EventFileHead->LEN_event[0]=byte;
	EventFileHead->LEN_event[1]=byte>>8;
	EventFileHead->REPORT=Report;//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
	EventFileHead->OCCUR=Occur;//�����ָ�:0=�ָ�,1=����,2=��
	EventFileHead->ACK=0;//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
	ms->msfile_write(filename,x*sizeof(EventFileHead_TypeDef),pbuf,sizeof(EventFileHead_TypeDef));
	//������+1
	x++;
	file->NUM_DATA=x;
	
//�����ϱ�
	i=MRR(ADDR_4300_8+1,1);
	if(i==0)
	{//�����������ϱ�
		for(i=0;i<NUMmax_601C;i++)
		{
		#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
			file->NUM_GPRSreport[i]=x;
			file->NUM_ENETreport[i]=x;
		#else
			file->NUM_GPRSorENETreport[i]=x;
		#endif
		}
	}
	else
	{//���������ϱ�
		//�ϱ�ͨ��
		REPORT_PORTn(file);//ͨ�������ϱ�ʱ�����ϱ����ݸ���=��ǰ���ݸ�����ʹ��ADDR_DATABUFF
	}

	return byte;
}


u32 GET_EventRecord_RCSD(u32 OAD,u8 *pRCSD,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
{
	u32 i;
	u32 n;
	u32 m;
	u32 OADn;
	u32 OADm;
	u8* pOAD;
	u32 Len;
	u8* p8;
	u32 fDATA;//����ѡ��OAD���ݱ�־
	
	i=Get_DL698DataLen_S(pRecord,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	pOAD=pRecord+i;
	n=pRCSD[0];//RCSD��SEQUENCE OF����
	m=pOAD[1];//��¼���б��OAD����
	pOAD+=2;
	if(n==0)
	{
		//����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ������
		n=m;
		pRCSD=(u8*)ADDR_1kDATABUFF;
		pRCSD[0]=n;//RCSD��SEQUENCE OF����
		MR((u32)pRCSD+1,(u32)pOAD,5*n);//���ջ�����ǰ,5*m���=5*256����Ҳֻ�ܳ������ջ����Ƥ�뷢���������ᳬ��������
	}
	pRCSD++;
	Len=0;
	fDATA=0;
	while(n--)
	{
		OADn=(pRCSD[1]<<24)+(pRCSD[2]<<16)+(pRCSD[3]<<8)+pRCSD[4];
		pRCSD+=5;
		for(i=0;i<m;i++)
		{
			OADm=(pOAD[(i*5)+1]<<24)+(pOAD[(i*5)+2]<<16)+(pOAD[(i*5)+3]<<8)+pOAD[(i*5)+4];
			if(OADn==OADm)
			{
				break;
			}
		}
		if(i<m)
		{//�ҵ�
			i++;//Ԫ�غ�
			p8=Get_Element(pRecord,i,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8)
			{
				i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				if(LEN_DATAmax<i)
				{
					return 0;
				}
				MR((u32)pout+Len,(u32)p8,i);
				Len+=i;
				LEN_DATAmax-=i;
				fDATA=1;//����ѡ��OAD���ݱ�־
				continue;
			}
		}
		if(LEN_DATAmax<1)
		{
			return 0;
		}
		pout[Len]=0;//NULL
		Len+=1;
		LEN_DATAmax-=1;
	}
	if(fDATA)//����ѡ��OAD���ݱ�־
	{
		return Len;
	}
	if(DATANULLSEL)//ȫ�����ݿ�ʱ��0=�޷���,1=�ÿ�����0
	{
		return Len;
	}
	return 0;
}

u32 GET_EventRecord(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE,u32 ALL)//�õ��¼���¼;���:ALL=0ֻ�¼���OAD�б�,ALL=1��OAD�б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 Len;
	u32 NUM;
	UARTCtrl_TypeDef *UARTCtrl;
	ms_Type *ms;
	u32 filename;
	msFILE_TypeDef *file;
	EventFileHead_TypeDef* EventFileHead;
	u32 NUMmax_DATA;
	
	NUMmax_DATA=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	ms=Get_ms();
	Comm_Ram->msFILESearchCount=0;//�ļ���������
	filename=GET_EventFileName(OAD&0xffffff00,0);//�����¼���¼�ļ���:���:OI,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	if(filename==0xffffffff)
	{//û�ҵ�
		UARTCtrl->DAR=6;//���󲻴���           ��6��
		return 0;
	}
	if(filename<FILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
	}
	else
	{
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	i=OAD&0xff;
	if(i)
	{//����Ԫ��(���1���¼�ΪԪ��1��
		NUM=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
		if(i>NUM)
		{
			UARTCtrl->DAR=6;//���󲻴��� ��6��
			return 0;
		}
		NUM=file->NUM_DATA;//�¼�����
		if(i>NUM)
		{//���ļ�����1�ļ���
			i-=NUM;
			Comm_Ram->msFILESearchCount=0;//�ļ���������
			filename=GET_EventFileName(OAD&0xffffff00,1);//�����¼���¼�ļ���:���:OI,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename==0xffffffff)
			{//û�ҵ�
				UARTCtrl->DAR=6;//���󲻴���           ��6��
				return 0;
			}
			if(filename<FILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			NUM=file->NUM_DATA;//�¼�����
			if(i>NUM)
			{//�ļ����ڼ���1�ļ���
				UARTCtrl->DAR=6;//���󲻴���           ��6��
				return 0;
			}
		}
		NUM-=i;
		Len=(NUM+1)*sizeof(EventFileHead_TypeDef);
		if(Len>LEN_128KDATABUFF)
		{
			UARTCtrl->DAR=6;//���󲻴���           ��6��
			return 0;
		}
		ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,Len);
		EventFileHead=(EventFileHead_TypeDef*)(ADDR_128KDATABUFF);
		i=0;
		for(Len=0;Len<NUM;Len++)
		{
			i+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
			EventFileHead++;
		}
		Len=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
		if(ALL==0)//ALL=0ֻ�¼���OAD�б�,ALL=1��OAD�б�
		{
			Len=EventFileHead->LEN_event[0]+(EventFileHead->LEN_event[1]<<8);
		}
		if(Len>LENmax_TxSPACE)
		{//��֡
			if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
			{
				UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
			}
			else
			{
				UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
			}
			return 0;
		}
		ms->msfile_read(filename,((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+i,p8tx,Len);
		//�¼��ϱ�״̬
//��׼�¼���¼��Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    instance-specific��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��

//  ��1�������������Ե�����  Data��
//  ��
//  ��n�������������Ե�����  Data
//}
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
	//ͨ���ϱ�״̬��=structure
	//{
	//ͨ��     OAD��
	//�ϱ�״̬ unsigned
	//bit0:�¼������ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit1:�¼������ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϣ�
	//bit2:�¼��������ָ����ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit3:�¼��������ָ����ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϡ�
	//}
		p8tx=Get_Element(p8tx,5,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
		if(p8tx)
		{
			if(p8tx[0]==DataType_array)
			{
				p8tx+=2;
				i=EventFileHead->REPORT;
				if(i&2)//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
				{//���ϱ�GPRS
					switch(EventFileHead->OCCUR)//�����ָ�:0=�ָ�,1=����,2=��
					{
						case 0://�ָ�
							if(EventFileHead->ACK)//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
							{
								p8tx[8]|=3<<2;
							}
							else
							{
								p8tx[8]|=1<<2;
							}
							break;
						case 1://����
						default://��
							if(EventFileHead->ACK)//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
							{
								p8tx[8]|=3;
							}
							else
							{
								p8tx[8]|=1;
							}
							break;
					}
				}
				if(i&4)//b0=�ϱ���־0�����ϱ�1���ϱ�;b1=���ϱ�GPRS,b2=���ϱ���̫��,b3=
				{//���ϱ���̫��
					switch(EventFileHead->OCCUR)//�����ָ�:0=�ָ�,1=����,2=��
					{
						case 0://�ָ�
							if(EventFileHead->ACK)//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
							{
								p8tx[8+9]|=3<<2;
							}
							else
							{
								p8tx[8+9]|=1<<2;
							}
							break;
						case 1://����
						default://��
							if(EventFileHead->ACK)//�ϱ�ȷ��:0=ûȷ��,1=ȷ��
							{
								p8tx[8+9]|=3;
							}
							else
							{
								p8tx[8+9]|=1;
							}
							break;
					}
				}
			}
		}
		return Len;
	}
	else
	{//ȫ��Ԫ��
		UARTCtrl->DAR=6;//���󲻴���           ��6��
		return 0;
	}
}


u32 GET_LastEventOccurRecovery(u32 OAD,u8* pTSA)//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
{
	u32 i;
	msFILE_TypeDef *file;
	u32 filename;
	EventFileHead_TypeDef* EventFileHead;
	ms_Type *ms;
	u32 filecount;
	u32 NUM_DATA;
	u32 n;
	u32 offset;
	u8* p8;
	
	OAD|=2;//�ļ�����
	for(filecount=0;filecount<2;filecount++)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(filename==recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			}
			if(file->BLOCK_ENTRY)
			{
				//�ļ�ID
				if(file->FILEID!=OAD)
				{
					file++;
					continue;
				}
				//�ļ�����
				if(file->FILECOUNT!=filecount)
				{
					file++;
					continue;
				}
				i=file->NUM_DATA;//�¼�����
				if(i==0)
				{
					file++;
					continue;
				}
				if(pTSA==0)
				{//û����Դ=�����ַ
					ms=Get_ms();
					i--;
					i*=sizeof(EventFileHead_TypeDef);
					ms->msfile_read(filename,i,(u8*)ADDR_DATABUFF,sizeof(EventFileHead_TypeDef));
					EventFileHead=(EventFileHead_TypeDef*)(ADDR_DATABUFF);
					i=EventFileHead->OCCUR;//�����ָ�:0=�ָ�,1=����,2=��
					if(i!=1)
					{
						i=0;
					}
					return i;//����:0=�ָ�,1=����
				}
				else
				{//�з���Դ=�����ַ
					ms=Get_ms();
					i=file->LEN_DATA;
					p8=ms->ms_malloc(i);//��̬�����ҵ��ڴ��
					if(p8)
					{
						ms->ms_free(p8);//�ͷ��ҵ��ڴ�ѷ���
						ms->msfile_read(filename,0,p8,i);
						NUM_DATA=file->NUM_DATA;//�¼�����
						n=NUM_DATA;
						while(n--)
						{
							EventFileHead=(EventFileHead_TypeDef*)p8;
							offset=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;���:OI;����:�洢���
							offset*=2;
							offset*=sizeof(EventFileHead_TypeDef);
							for(i=0;i<n;i++)
							{
								offset+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								EventFileHead++;
							}
							if(offset>=file->LEN_DATA)
							{//ƫ�ƴ�
								return 1;//����:0=�ָ�,1=����
							}
							if(p8[offset+2+5+8]==DataType_date_time_s)
							{//�н���ʱ��
								i=Compare_DL698DataString(pTSA,p8+offset+23,3+6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							}
							else
							{//��ʼʱ����=��,����ʱ��=NULL
								i=Compare_DL698DataString(pTSA,p8+offset+(23-7),3+6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							}
							if(i==0)
							{//��ͬ
								i=EventFileHead->OCCUR;//�����ָ�:0=�ָ�,1=����,2=��
								if(i!=1)
								{
									i=0;
								}
								return i;//����:0=�ָ�,1=����
							}
						}
					}
				}
			}
			file++;
		}
	}
	return 0;//����:0=�ָ�,1=����
}

u32 GET_LastEventTime_OccurRecovery(u32 OAD,u8* pTSA,u8* pOut,u32 OccurRecovery)//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
{
	u32 i;
	msFILE_TypeDef *file;
	u32 filename;
	EventFileHead_TypeDef* EventFileHead;
	ms_Type *ms;
	u32 filecount;
	u32 NUM_DATA;
	u32 n;
	u32 offset;
	u8* p8;
	
	OAD|=2;//�ļ�����
	for(filecount=0;filecount<2;filecount++)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(filename==recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			}
			if(file->BLOCK_ENTRY)
			{
				//�ļ�ID
				if(file->FILEID!=OAD)
				{
					file++;
					continue;
				}
				//�ļ�����
				if(file->FILECOUNT!=filecount)
				{
					file++;
					continue;
				}
				i=file->NUM_DATA;//�¼�����
				if(i==0)
				{
					file++;
					continue;
				}
				if(pTSA==0)
				{//û����Դ=�����ַ
					ms=Get_ms();
					i=file->LEN_DATA;
					p8=ms->ms_malloc(i);//��̬�����ҵ��ڴ��
					if(p8)
					{
						ms->ms_free(p8);//�ͷ��ҵ��ڴ�ѷ���
						ms->msfile_read(filename,0,p8,i);
						NUM_DATA=file->NUM_DATA;//�¼�����
						n=NUM_DATA;
						while(n--)
						{
							EventFileHead=(EventFileHead_TypeDef*)p8;
							offset=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;���:OI;����:�洢���
							offset*=2;
							offset*=sizeof(EventFileHead_TypeDef);
							for(i=0;i<n;i++)
							{
								offset+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								EventFileHead++;
							}
							if(offset>=file->LEN_DATA)
							{//ƫ�ƴ�
								pOut[0]=0;
								return 1;
							}
							i=EventFileHead->OCCUR;//�����ָ�:0=�ָ�,1=����,2=��
							if((i==OccurRecovery)||(i==2))
							{
								if(OccurRecovery==1)
								{//����β���ʱ��
									if(p8[offset+2+5]!=DataType_date_time_s)
									{
										pOut[0]=0;
										return 1;
									}
									MR((u32)pOut,(u32)p8+offset+2+5,8);//ȡ����ʱ��
									return 8;
								}
								else
								{//����λָ�ʱ��
									if(p8[offset+2+5+8]!=DataType_date_time_s)
									{
										pOut[0]=0;
										return 1;
									}
									MR((u32)pOut,(u32)p8+offset+2+5+8,8);//ȡ����ʱ��
									return 8;
								}
							}
						}
					}
				}
				else
				{//�з���Դ=�����ַ
					ms=Get_ms();
					i=file->LEN_DATA;
					p8=ms->ms_malloc(i);//��̬�����ҵ��ڴ��
					if(p8)
					{
						ms->ms_free(p8);//�ͷ��ҵ��ڴ�ѷ���
						ms->msfile_read(filename,0,p8,i);
						NUM_DATA=file->NUM_DATA;//�¼�����
						n=NUM_DATA;
						while(n--)
						{
							EventFileHead=(EventFileHead_TypeDef*)p8;
							offset=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;���:OI;����:�洢���
							offset*=2;
							offset*=sizeof(EventFileHead_TypeDef);
							for(i=0;i<n;i++)
							{
								offset+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								EventFileHead++;
							}
							if(offset>=file->LEN_DATA)
							{//ƫ�ƴ�
								pOut[0]=0;
								return 1;
							}
							if(p8[offset+2+5+8]==DataType_date_time_s)
							{//�н���ʱ��
								i=Compare_DL698DataString(pTSA,p8+offset+23,3+6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							}
							else
							{//��ʼʱ����=��,����ʱ��=NULL
								i=Compare_DL698DataString(pTSA,p8+offset+(23-7),3+6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							}
							if(i==0)
							{//��ͬ
								i=EventFileHead->OCCUR;//�����ָ�:0=�ָ�,1=����,2=��
								if((i==OccurRecovery)||(i==2))
								{
									if(OccurRecovery==1)
									{//����β���ʱ��
										if(p8[offset+2+5]!=DataType_date_time_s)
										{
											pOut[0]=0;
											return 1;
										}
										MR((u32)pOut,(u32)p8+offset+2+5,8);//ȡ����ʱ��
										return 8;
									}
									else
									{//����λָ�ʱ��
										if(p8[offset+2+5+8]!=DataType_date_time_s)
										{
											pOut[0]=0;
											return 1;
										}
										MR((u32)pOut,(u32)p8+offset+2+5+8,8);//ȡ����ʱ��
										return 8;
									}
								}
							}
						}
					}
				}
			}
			file++;
		}
	}
	pOut[0]=0;
	return 1;
}
u32 GET_LastEventTime_Occur(u32 OAD,u8* pTSA,u8* pOut)//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
{
	return GET_LastEventTime_OccurRecovery(OAD,pTSA,pOut,1);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
}
u32 GET_LastEventTime_Recovery(u32 OAD,u8* pTSA,u8* pOut)//�õ�������¼��ָ�ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
{
	return GET_LastEventTime_OccurRecovery(OAD,pTSA,pOut,0);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
}



EventSour_typedef* GET_EventSour(u16* pRMAddr)//�õ��¼�ԭֵ����ָ�룬���:�����ַ, ����:����ָ��,0=����
{
	u32 i;
	EventSour_typedef* EventSour;
	
	i=AddrToMeterSetNo(pRMAddr);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
	if(i>=NUM_RMmax)
	{
		return 0;//��Чָ��
	}
	EventSour=(EventSour_typedef*)(ADDR_Event_Sour+(i*sizeof(EventSour_typedef)));
	for(i=0;i<3;i++)
	{
		if(EventSour->RMAddr[i]!=pRMAddr[i])
		{//��ַ��ͬ,���¼�ԭ����
			for(i=0;i<3;i++)
			{
				EventSour->RMAddr[i]=pRMAddr[i];
			}
			EventSour->res=0;
			EventSour->Re=0;//����ʧ��(310F)�����ִ�
			EventSour->Occur=0;//�¼������ָ�,bit=0�ָ�=1����
			EventSour->TimeCount=0;//���ܱ�ͣ��ʱ���ʱ(��)
			break;
		}
	}
	return EventSour;
}


void CLR_EventRecord(u32 OI)//����ӦOI���¼���¼
{
	u32 i;
	u32 FileID;
	msFILE_TypeDef *file;
	u32 filename;
	ms_Type *ms;
	DL698_EVENTLIB_TypeDef *pLib;
	
	ms=Get_ms();
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			//�ļ�����
			if((FileID&0xff)!=2)
			{
				file++;
				continue;
			}
			//�ļ�OI
			FileID>>=16;
			if(FileID!=OI)
			{
				file++;
				continue;
			}
			i=filename;
			if(i>=recordFILENAMEmax_FLASH)
			{
				i-=recordFILENAMEmax_FLASH;
				i+=FILENAMEmax_FLASH;
			}
			ms->msfile_Delete(i);
		//���¼���ǰֵ��¼��
			pLib=Get_DL698_EVENT_pLib(OI<<16);//�õ�EVENT_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
			if(pLib)
			{
				if(pLib->Addr_Currentvalue)
				{
					MC(0,pLib->Addr_Currentvalue,LENper_EventCurrentvalue);
				}
			}
		//����ص��Ѽ�¼��־
			if(OI==0x3110)
			{//ͨ�����������¼�
				MC(0,ADDR_2200_EventRecordFags,1);//ͨ�����������¼��Ѽ�¼��־,�¸���ʱ��0
			}
		}
		file++;
	}
}



u32 EVENT_HeadData(u32 OAD,u8* pOccurSour,u32 Occur)//��׼�¼�ͷ����;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* pOAD;
	u8* p8;
	u32 ADDR_EventOAD;
	DL698_EVENTLIB_TypeDef * pLib;
//	UARTCtrl_TypeDef *UARTCtrl;
	
//�¼�����ʹ��Լ��
//ADDR_DATABUFF�¼��༭���
//ADDR_128KDATABUFF  32KBYTE ������DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE �¼�����,�����������Ա��
//ADDR_128KDATABUFF+64K 32KBYTE �����¼�����ʱ����
//ADDR_128KDATABUFF+96K 32KBYTE �¼��и������ݵ���ӦOAD˳���array OAD
	
//��Ч��ʶ��=bool
//�ϱ���ʶ��=enum
//{
//���ϱ���0����
//�¼������ϱ���1����
//�¼��ָ��ϱ���2����
//�¼������ָ����ϱ���3��
//}
//�����������Ա��=array OAD

	p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
	p8[0]=DataType_array;
	p8[1]=0;
	pLib=Get_DL698_EVENT_pLib(OAD);//�õ�EVENT_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
	if(pLib==0)
	{
		return 0;//����:0=û��¼,1=��¼
	}
	ADDR_EventOAD=pLib->Addr_EventOAD;
#if (4+LENmax_EventOAD)>(32*1024)
	#error
#endif
	MR(ADDR_128KDATABUFF+(32*1024),ADDR_EventOAD,4+LENmax_EventOAD);
	pOAD=(u8*)ADDR_128KDATABUFF+(32*1024);
	//��Ч��ʶ
	if(pOAD[0]!=DataType_bool)
	{
		return 0;//����:b0-b15�ܳ���,b16=�ϱ���ʶ0=���ϱ�1=�ϱ�
	}
	if(pOAD[1]==0)
	{
		return 0;//����:b0-b15�ܳ���,b16=�ϱ���ʶ0=���ϱ�1=�ϱ�
	}
	Report=pOAD[3];//�ϱ���ʶ
	switch(Report)
	{
		default:
		case 0://���ϱ�
			break;
		case 1://�¼������ϱ�
			if(Occur==0)
			{
				Report=0;
			}
			break;
		case 2://�¼��ָ��ϱ�
			if(Occur==1)
			{
				Report=0;
			}
			break;
		case 3://�¼������ָ����ϱ�
			Report=1;
			break;
	}
	
	p8=(u8*)ADDR_DATABUFF;
//��׼�¼���¼��Ԫ��=structure
//{
	p8[0]=DataType_structure;
	p8[1]=5;//�ṹ��Ԫ����
//  �¼���¼���  double-long-unsigned��
	p8[2]=DataType_double_long_unsigned;
	p8[3]=0;
	p8[4]=0;
	p8[5]=0;
	p8[6]=0;
	switch(Occur)
	{
		case 0://�ָ�
	//  �¼�����ʱ��  date_time_s��
			p8[7]=DataType_date_time_s;
			p8[8]=0;//0xff;
			p8[9]=0;//0xff;
			p8[10]=0;//0xff;
			p8[11]=0;//0xff;
			p8[12]=0;//0xff;
			p8[13]=0;//0xff;
			p8[14]=0;//0xff;
	//  �¼�����ʱ��  date_time_s��
			p8[15]=DataType_date_time_s;
			Get_date_time_s(p8+16);//�õ�date_time_s��7�ֽ�
			Len=23;
			break;
		case 1://����
	//  �¼�����ʱ��  date_time_s��
			p8[7]=DataType_date_time_s;
			Get_date_time_s(p8+8);//�õ�date_time_s��7�ֽ�
	//  �¼�����ʱ��  date_time_s��
			p8[15]=0;//NULL
			Len=16;
			break;
		default://�޲����ָ�
	//  �¼�����ʱ��  date_time_s��
			p8[7]=DataType_date_time_s;
#if (USER/100)==15//����
		if(OAD==0x31140200)
			MR((u32)p8+8,ADDR_128KDATABUFF+(64*1024),7);
		else
			Get_date_time_s(p8+8);//�õ�date_time_s��7�ֽ�
#else
			Get_date_time_s(p8+8);//�õ�date_time_s��7�ֽ�
#endif
	//  �¼�����ʱ��  date_time_s��
			p8[15]=DataType_date_time_s;
			Get_date_time_s(p8+16);//�õ�date_time_s��7�ֽ�
			Len=23;
			break;
	}
//  �¼�����Դ    instance-specific��
	if(pOccurSour==0)
	{
		p8[Len]=0;//NULL
		Len++;
	}
	else
	{
		i=Get_DL698DataLen_S(pOccurSour,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		if(i>(LEN_DATABUFF-1024))
		{
			return 0;//����
		}
		MR((u32)p8+Len,(u32)pOccurSour,i);
		Len+=i;
	}
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
#if(USER/100)==17//����ϵ
	if(OAD==0x310A0200)
	{
		p8[Len]=DataType_array;
		p8[Len+1]=1;
		Len+=2;
		p8[Len]=DataType_structure;
		p8[Len+1]=2;
		p8[Len+2]=DataType_OAD;
		p8[Len+3]=0xF2;
		p8[Len+4]=0x09;
		p8[Len+5]=0x02;
		p8[Len+6]=0x01;
		p8[Len+7]=DataType_unsigned;
		p8[Len+8]=1;
		Len+=9;
	}
	else
	{
		p8[Len]=DataType_array;
		p8[Len+1]=2;
		Len+=2;
		p8[Len]=DataType_structure;
		p8[Len+1]=2;
		p8[Len+2]=DataType_OAD;
		p8[Len+3]=0x45;
		p8[Len+4]=0x00;
		p8[Len+5]=0x02;
		p8[Len+6]=0x00;
		p8[Len+7]=DataType_unsigned;
#if(USER/100)==17//����ϵ
		p8[Len+8]=1;
#else
		p8[Len+8]=0;
#endif
		Len+=9;
		p8[Len]=DataType_structure;
		p8[Len+1]=2;
		p8[Len+2]=DataType_OAD;
		p8[Len+3]=0x45;
		p8[Len+4]=0x10;
		p8[Len+5]=0x02;
		p8[Len+6]=0x00;
		p8[Len+7]=DataType_unsigned;
		p8[Len+8]=0;
		Len+=9;
	}
#else
	p8[Len]=DataType_array;
	p8[Len+1]=2;
	Len+=2;
	//OAD_GPRS     	0x45000200
	p8[Len]=DataType_structure;
	p8[Len+1]=2;
	p8[Len+2]=DataType_OAD;
	p8[Len+3]=0x45;
	p8[Len+4]=0x00;
	p8[Len+5]=0x02;
	p8[Len+6]=0x00;
	//bit0:�¼������ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit1:�¼������ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϣ�
	//bit2:�¼��������ָ����ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit3:�¼��������ָ����ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϡ�
	p8[Len+7]=DataType_unsigned;
#if(USER/100)==17//����ϵ
	p8[Len+8]=1;
#else
	p8[Len+8]=0;
#endif
	
	Len+=9;
	//OAD_ENET     	0x45100200
	p8[Len]=DataType_structure;
	p8[Len+1]=2;
	p8[Len+2]=DataType_OAD;
	p8[Len+3]=0x45;
	p8[Len+4]=0x10;
	p8[Len+5]=0x02;
	p8[Len+6]=0x00;
	//bit0:�¼������ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit1:�¼������ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϣ�
	//bit2:�¼��������ָ����ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit3:�¼��������ָ����ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϡ�
	p8[Len+7]=DataType_unsigned;
	p8[Len+8]=0;
	Len+=9;
#endif
//  ��1�������������Ե�����  Data��
//  ��
//  ��n�������������Ե�����  Data
//}

	p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
	p8[0]=DataType_array;
	p8[1]=5;//5��
	p8[2]=0;//DataType_OAD;//�¼���¼���
	p8[3]=0x20;
	p8[4]=0x22;
	p8[5]=0x02;
	p8[6]=0x00;
	p8[7]=0;//DataType_OAD;//�¼�����ʱ��
	p8[8]=0x20;
	p8[9]=0x1e;
	p8[10]=0x02;
	p8[11]=0x00;
	p8[12]=0;//DataType_OAD;//�¼�����ʱ��
	p8[13]=0x20;
	p8[14]=0x20;
	p8[15]=0x02;
	p8[16]=0x00;
	p8[17]=0;//DataType_OAD;//�¼�����Դ
	p8[18]=0x20;
	p8[19]=0x24;
	p8[20]=0x02;
	p8[21]=0x00;
	p8[22]=0;//DataType_OAD;//�¼��ϱ�״̬
	p8[23]=0x33;
	p8[24]=0x00;
	p8[25]=0x02;
	p8[26]=0x00;
	return (Len<<8)|Report;
}

u32 EVENT_OADData(u32 Len,u32 OccurkWh)//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,OccurkWh=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
{
	u32 i;
	u32 NUM_OAD;
	u8* pOAD;
	u8* p8;
	u8* pALLOAD;
	
	p8=(u8*)ADDR_DATABUFF;
	pOAD=(u8*)ADDR_128KDATABUFF+(32*1024)+4;
	NUM_OAD=pOAD[1];//����OAD��
	pOAD+=2;
	while(NUM_OAD--)
	{
		if(pOAD[0]==DataType_OAD)
		{//OAD
			if(Len>(LEN_DATABUFF-1))
			{
				return 0;//����
			}
			p8[1]++;//�ṹ��Ԫ����
			//�¼��и������ݵ���ӦOAD˳���array OAD
			pALLOAD=(u8*)ADDR_128KDATABUFF+(96*1024);
			i=pALLOAD[1];
			pALLOAD[1]++;//����+1
			i*=5;
			i+=2;
			pALLOAD+=i;
			pALLOAD[0]=0;//DataType_OAD;
			pALLOAD[1]=pOAD[1];
			pALLOAD[2]=pOAD[2];
			pALLOAD[3]=pOAD[3];
			pALLOAD[4]=pOAD[4];
			
			i=(pOAD[1]<<24)|(pOAD[2]<<16)|(pOAD[3]<<8)|(pOAD[4]<<0);
			pOAD+=5;
			switch(i)
			{
				case 0x201E4200://�¼�����ʱ��
				case 0x20204200://�¼�����ʱ��
					p8[Len]=DataType_date_time_s;
					Get_date_time_s(p8+Len+1);//�õ�date_time_s��7�ֽ�
					Len+=8;
					break;
				case 0x00102200://00102201�¼�����ǰ�����й��ܵ��ܼ�����
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024),0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024),i);
					Len+=i;
					break;
				case 0x00102201://00102201�¼�����ǰ�����й��ܵ���
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+2,i);
					Len+=i;
					break;
				case 0x00106200://00102201�¼�����ǰ�����й��ܵ��ܼ�����
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024),0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024),i);
					Len+=i;
					break;
				case 0x00106201://00102201�¼�����ǰ�����й��ܵ���
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+2,i);
					Len+=i;
					break;
				case 0x00104200://00102201�¼������������й��ܵ��ܼ�����
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024,i);
					Len+=i;
					break;
				case 0x00104201://00102201�¼������������й��ܵ���
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+2,i);
					Len+=i;
					break;
				case 0x00108200://00108201�¼������������й��ܵ��ܼ�����
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024,i);
					Len+=i;
					break;
				case 0x00108201://00108201�¼������������й��ܵ���
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+2,i);
					Len+=i;
					break;
					
				case 0x00202200://00202201�¼�����ǰ�����й��ܵ��ܼ�����
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128,i);
					Len+=i;
					break;
				case 0x00202201://00202201�¼�����ǰ�����й��ܵ���
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128+2,i);
					Len+=i;
					break;
				case 0x00206200://00202201�¼�����ǰ�����й��ܵ��ܼ�����
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128,i);
					Len+=i;
					break;
				case 0x00206201://00202201�¼�����ǰ�����й��ܵ���
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128+2,i);
					Len+=i;
					break;
				case 0x00204200://00202201�¼����������й��ܵ��ܼ�����
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128,i);
					Len+=i;
					break;
				case 0x00204201://00202201�¼����������й��ܵ���
					if(OccurkWh!=1)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128+2,i);
					Len+=i;
					break;
				case 0x00208200://00208201�¼����������й��ܵ��ܼ�����
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128,i);
					Len+=i;
					break;
				case 0x00208201://00208201�¼����������й��ܵ���
					if(OccurkWh!=0)//occur=0�ָ�,=1����,=2��
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128+2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128+2,i);
					Len+=i;
					break;
				
				case 0x40002200://��ʱ�¼�����ǰ��ʱʱ��
					p8[Len]=DataType_date_time_s;
					MR((u32)p8+Len+1,ADDR_128KDATABUFF+(64*1024),7);
					Len+=8;
					break;
				case 0x40008200://��ʱ�¼��������ʱʱ��
					p8[Len]=DataType_date_time_s;
					MR((u32)p8+Len+1,ADDR_DL698YMDHMS,7);
					Len+=8;
					break;

				case 0x22004202://�¼���������ͨ������
					MR((u32)p8+Len,ADDR_2200+2+5,5);
					Len+=5;
					break;
				
				default:
					i=GET_OAD_DATA(ACSAMPLEPORT,i,p8+Len,LEN_DATABUFF-Len);//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
					if(i==0)
					{//����������
						p8[Len]=0;//NULL
						Len++;
						break;
					}
					Len+=i;
					break;
			}
		}
		else
		{//ROAD
			i=Get_DL698DataLen_S(pOAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			pOAD+=i;
		}
	}
	return Len;
}

void Event_OAD(u32 OAD,u8* pOccurSour,u32 Occur,u32 OccurkWh)//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 Len;
	u32 Report;
	
	Len=EVENT_HeadData(OAD,pOccurSour,Occur);//��׼�¼�ͷ����;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		Len=EVENT_OADData(Len,OccurkWh);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(OAD,(u8*)ADDR_DATABUFF,Len,Occur,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}


void Event_30300200(u8* pDATA, u8* pOccurSour,u32 FramLen)//ģ��ID����¼���¼;���:pDATA=ģ��ID�������;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x30300200,pOccurSour,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=3;//�ṹ��Ԫ����
		MR(ADDR_DATABUFF+Len,(u32)pDATA,FramLen);
		Len+=FramLen;

		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=3;//����+3
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//ģ��ID����¼���Ԫ-6
		p8[1]=0x33;
		p8[2]=0x12;
		p8[3]=0x02;
		p8[4]=6;
		
		p8[5]=0;//DataType_OAD;//ģ��ID����¼���Ԫ-7
		p8[6]=0x33;
		p8[7]=0x12;
		p8[8]=0x02;
		p8[9]=7;

		p8[10]=0;//DataType_OAD;//ģ��ID����¼���Ԫ-8
		p8[11]=0x33;
		p8[12]=0x12;
		p8[13]=0x02;
		p8[14]=8;

		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x30300200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}

void Event_31010200(void)//�ն˰汾����¼�
{
	Event_OAD(0x31010200,0,2,2);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;����:0=û��¼,1=��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
}


//���ܱ�ʱ�ӳ����¼��Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    TSA��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  ���ܱ�ʱ��    DATETIME_S��
//  �ն˵�ǰʱ��  DATETIME_S��
//��1�������������Ե�����   Data��
//  ��
//  ��n�������������Ե�����   Data
//}

void Event_31050200(u8* pOccurSour,u32 Occur,u8* p1)//���ܱ�ʱ�ӳ����¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	Len=EVENT_HeadData(0x31050200,pOccurSour,Occur);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=2;//�ṹԪ����
//  ���ܱ�ʱ��    DATETIME_S��
		MR(ADDR_DATABUFF+Len,(u32)p1,8);//�����¼�����ʱ����
		Len+=8;
//  �ն˵�ǰʱ��  DATETIME_S��
		p8=(u8*)ADDR_DATABUFF+Len;
		p8[0]=DataType_date_time_s;
		MR(ADDR_DATABUFF+Len+1,ADDR_DL698YMDHMS,7);
		Len+=8;
//	�¼��и������ݵ���ӦOAD˳���array OAD
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=2;//����+2
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD
		p8[1]=0x33;//���ܱ�ʱ�ӳ����¼��Ԫ,Ԫ��6,���ܱ�ʱ��    DATETIME_S
		p8[2]=0x13;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD
		p8[6]=0x33;//���ܱ�ʱ�ӳ����¼��Ԫ,Ԫ��7,�ն˵�ǰʱ��  DATETIME_S
		p8[7]=0x13;
		p8[8]=0x02;
		p8[9]=7;
		
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x31050200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}

//ͣ/�ϵ��¼���¼��Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    enum{ͣ��(0)���ϵ�(1)}��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��  
//���Ա�־     bit-string��SIZE(8)����
//  ��1�������������Ե�����   Data��
//  ��
//  ��n�������������Ե�����   Data
//}
//���Ա�־��
//bit0�á�1�����¼�������bit0�á�0�����¼��쳣���˴��Ƕ�ͣ��ʱ����Ч������жϣ�
//bit1�á�1�����¼���Ч��bit1�á�0�����¼���Ч���˴��Ƕ�ͣ��ʱ��ƫ����ֵ���жϣ�
//bit2��bit7���á�
void Event_31060200(u8* pOccurSour,u32 Occur)//�ն�ͣ/�ϵ��¼���¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

//�¼�����Դ    enum{ͣ��(0)���ϵ�(1)}
	pOccurSour=(u8*)ADDR_128KDATABUFF+(64*1024);
	pOccurSour[0]=DataType_enum;
	if(Occur)
	{
		pOccurSour[1]=0;//ͣ��(0)
	}
	else
	{
		pOccurSour[1]=1;//�ϵ�(1)
	}
	Len=EVENT_HeadData(0x31060200,pOccurSour,Occur);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		if(Occur==1)
		{//����
			//�޸Ĳ���ʱ��
			MR((u32)ADDR_DATABUFF+8,(u32)&PowerEvent->Tstarttime,7);
		}
		else
		{//�ָ�
			//�޸Ĳ���ʱ��
			MR((u32)ADDR_DATABUFF+8,(u32)&PowerEvent->Tstarttime,7);
			//�޸Ļָ�ʱ��
			MR((u32)ADDR_DATABUFF+16,(u32)&PowerEvent->Tstoptime,7);
		}
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//�ṹ��Ԫ����
		p8[Len]=DataType_bit_string;
		p8[Len+1]=8;
		p8[Len+2]=PowerEvent->bit;//���Ա�־��
		Len+=3;
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]++;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//ͣ/�ϵ��¼���¼��Ԫ,Ԫ��6
		p8[1]=0x33;
		p8[2]=0x09;
		p8[3]=0x02;
		p8[4]=6;

		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x31060200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}	
void Event_310A0200(u8* pOccurSour,u32 Occur,u8* pTSA)//
{
	u32 Len;
	u32 Report;
	u8* p8;
	u32 i;
	Len=EVENT_HeadData(0x310A0200,pOccurSour,Occur);//��׼�¼�ͷ����;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//�ṹ��Ԫ����
		p8[Len]=DataType_TSA;
		p8[Len+1]=7;
		p8[Len+2]=5;
		MR((u32)p8+Len+3,(u32)pTSA,6);
		Data_Inverse((u32)p8+Len+3,6);
		Len+=9;
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]++;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//Ŀ���������ַ
		p8[1]=0x20;
		p8[2]=0x2A;
		p8[3]=0x02;
		p8[4]=0;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x310A0200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}

//�ܾ��ӽڵ�������¼��Ԫ��=structure
//{
//		�¼���¼��� double-long-unsigned��
//		�¼�����ʱ�� date_time_s��
//		�¼�����ʱ�� date_time_s��
//		�¼�����Դ TSA��
//		�¼��ϱ�״̬ array ͨ���ϱ�״̬��
//		�ܾ��ӽڵ��б� array octet-string��
//		�� 1 �������������Ե����� Data��
//		...
//		�� n �������������Ե����� Data
//}
void Event_311F0200(u8* pDATA,u8* pOccurSour,u32 FramLen)//�ܾ��ӽڵ�������¼:���:pDATA=1���ܾ��ӽڵ�������¼��Ԫ;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x311F0200,pOccurSour,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{		
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=1;//�ṹ��Ԫ����
		MR(ADDR_DATABUFF+Len,(u32)pDATA,FramLen);
		Len+=FramLen;

		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=1;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//�ܾ��ӽڵ�������¼��Ԫ-6
		p8[1]=0x33;
		p8[2]=0x1B;
		p8[3]=0x02;
		p8[4]=6;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x311F0200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}


//����δ֪���ܱ��¼���=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    NULL��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  �ѱ���      array һ���ѱ�����
//}
void Event_31110200(u8* pDATA)//����δ֪���ܱ��¼���¼;���:pDATA=1���ѱ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x31110200,0,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//�ṹ��Ԫ����
		p8[Len]=DataType_array;
		p8[Len+1]=1;
		Len+=2;
		i=Get_DL698DataLen_S(pDATA,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		MR(ADDR_DATABUFF+Len,(u32)pDATA,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]++;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//����δ֪���ܱ��¼���Ԫ-6
		p8[1]=0x33;
		p8[2]=0x03;
		p8[3]=0x02;
		p8[4]=6;
		//Len=EVENT_OADData(Len);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x31110200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}
//��̨�����ܱ��¼���Ԫ��=structure
//{
//  �¼���¼���    double-long-unsigned��
//  �¼�����ʱ��    date_time_s��
//  �¼�����ʱ��    date_time_s��
//  �¼�����Դ      NULL��
//  �¼��ϱ�״̬    array ͨ���ϱ�״̬��
//  ��̨���ѱ���  array  һ����̨�������
//  ��1�������������Ե�����  Data��
//  ��
//  ��n�������������Ե�����  Data
//}
void Event_31120200(u8* pDATA)//��̨�����ܱ��¼���¼;���:pDATA=һ����̨�����;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x31120200,0,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//�ṹ��Ԫ����
		p8[Len]=DataType_array;
		p8[Len+1]=1;
		Len+=2;
		i=Get_DL698DataLen_S(pDATA,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		MR(ADDR_DATABUFF+Len,(u32)pDATA,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]++;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//��̨�����ܱ��¼���Ԫ-6
		p8[1]=0x33;
		p8[2]=0x04;
		p8[3]=0x02;
		p8[4]=6;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x31120200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}




//ң���¼���¼��Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    OAD��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  �غ�2�����ܼ��鹦�� array long64��
//  ��1�������������Ե�����   Data��
//  ��
//  ��n�������������Ե�����   Data
//}
void Event_31150200(u8* pOAD,u32 Occur)//ң���¼���¼(�غ�2����),���:pOAD=�¼�����ԴOAD���̵�����Ԫ��,occur=0�ָ�=1����=2��
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	Len=EVENT_HeadData(0x31150200,pOAD,Occur);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=1;//�ṹ��Ԫ����
		//�غ�2�����ܼ��鹦�� array long64
		p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
		p8[0]=DataType_array;
		p8[1]=1;
		MR((u32)p8+2,ADDR_230x_3,9);
		i=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)p8,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=1;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//��̶����б�
		p8[1]=0x33;
		p8[2]=0x0A;
		p8[3]=0x02;
		p8[4]=6;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x31150200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}




//��̼�¼�¼���Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    NULL��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  ��̶����б�  array OAD��
//  ��1�������������Ե�����  Data��
//  ��
//  ��n�������������Ե�����  Data
//}
void Event_31180200(u8* pOAD)//�ն˱���¼���¼;���:pOAD=���õ�OAD����ָ��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	Len=EVENT_HeadData(0x31180200,0,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//�ṹ��Ԫ����
		i=Get_DL698DataLen_S(pOAD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		MR(ADDR_DATABUFF+Len,(u32)pOAD,i);
		Len+=i;
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]++;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//��̶����б�
		p8[1]=0x33;
		p8[2]=0x02;
		p8[3]=0x02;
		p8[4]=6;
		
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x31180200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
		
	}
}






//���ܱ����ݱ����ؼ�¼��Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    TSA��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  ������ݶ���  CSD��
//  �仯ǰ����    Data��
//  �仯������    Data
//}
//�¼�����ʱ�䣺Ϊ������ݷ����仯��ʱ�̣�
//�¼�����ʱ�䣺��Ч�����ֶ���FF��
void Event_311C0200(u32 PORTn,u8* pCSD,u8* pDATA1,u8* pDATA2)//���ܱ����ݱ����ؼ�¼,���:pCSD=��ص�CSD,pDATA1=���ǰ����,pDATA2=���������
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->TSA[0]=DataType_TSA;
	UARTCtrl->TSA[1]=7;
	UARTCtrl->TSA[2]=5;
	Len=EVENT_HeadData(0x311C0200,UARTCtrl->TSA,1);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=3;//�ṹ��Ԫ����
		i=Get_DL698DataLen_S(pCSD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)pCSD,i);
		Len+=i;
		i=Get_DL698DataLen_S(pDATA1,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)pDATA1,i);
		Len+=i;
		i=Get_DL698DataLen_S(pDATA2,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)pDATA2,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=3;//����+3
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//��̶����б�
		p8[1]=0x33;
		p8[2]=0x0F;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//��̶����б�
		p8[6]=0x33;
		p8[7]=0x0F;
		p8[8]=0x02;
		p8[9]=7;
		p8[10]=0;//DataType_OAD;//��̶����б�
		p8[11]=0x33;
		p8[12]=0x0F;
		p8[13]=0x02;
		p8[14]=8;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x311C0200,(u8*)ADDR_DATABUFF,Len,1,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}


//3305 ������բ��¼��Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    OI��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  �¼�������2���ӹ���  long64(��λ��W������-1)��
//  ���ƶ���      OI��
//  ��բ�ִ�      bit-string(SIZE(8))��
//	���ض�ֵ      long64����λ��kW������-4����
//	��բ����ǰ�ܼ��й�����    long64����λ��kW������-4����
//  ��1�������������Ե�����  Data��
//  ��
//  ��n�������������Ե�����  Data
//}
void Event_32000200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst,u8* pAfter)//������բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	//���:pOccurSour=�¼�����Դ    OI
	//OI=���ƶ���      OI
	//bit=��բ�ִ�      bit-string(SIZE(8))
	//pFixedValue=���ض�ֵ      long64����λ��kW������-4��
	//pFirst=��բ����ǰ�ܼ��й�����    long64����λ��kW������-4��
	//pAfter=�¼�������2���ӹ���  long64(��λ��W������-1)
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
	p8[0]=DataType_OI;
	p8[1]=pOccurSour>>8;
	p8[2]=pOccurSour;
	Len=EVENT_HeadData(0x32000200,p8,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=5;//�ṹ��Ԫ����
//  �¼�������2���ӹ���  long64(��λ��W������-1)��
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pAfter,8);
		Len+=9;
//  ���ƶ���      OI��
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_OI;
		p8[Len+1]=OI>>8;
		p8[Len+2]=OI;
		Len+=3;
//  ��բ�ִ�      bit-string(SIZE(8))��
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_bit_string;
		p8[Len+1]=8;
		p8[Len+2]=bit;
		Len+=3;
//	���ض�ֵ      long64����λ��kW������-4����
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFixedValue,8);
		Len+=9;
//	��բ����ǰ�ܼ��й�����    long64����λ��kW������-4����
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFirst,8);
		Len+=9;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=5;//����+5
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//�¼�������2���ӹ���
		p8[1]=0x33;
		p8[2]=0x05;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//���ƶ���      OI
		p8[6]=0x33;
		p8[7]=0x05;
		p8[8]=0x02;
		p8[9]=7;
		p8[10]=0;//DataType_OAD;//��բ�ִ�
		p8[11]=0x33;
		p8[12]=0x05;
		p8[13]=0x02;
		p8[14]=8;
		p8[15]=0;//DataType_OAD;//���ض�ֵ
		p8[16]=0x33;
		p8[17]=0x05;
		p8[18]=0x02;
		p8[19]=9;
		p8[20]=0;//DataType_OAD;//��բ����ǰ�ܼ��й�����
		p8[21]=0x33;
		p8[22]=0x05;
		p8[23]=0x02;
		p8[24]=10;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x32000200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}


//3306 �����բ��¼��Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    OI��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  ���ƶ���      OI��
//  ��բ�ִ�      bit-string(SIZE(8))��
//��ض�ֵ      long64����λ��kWh������-4����
//��բ����ʱ�ܼӵ�����  long64����λ��kwh/Ԫ������-4����

//  ��1�������������Ե�����  Data��
//  ��
//  ��n�������������Ե�����  Data
//}
//˵����
//���¼�����Դ����Ϊ�µ��8108����բʱ�ܼӵ�����Ϊ�ܼ��µ�������
//���¼�����Դ����Ϊ�����8107����բʱ�ܼӵ�����Ϊʣ�������/�ѡ�
void Event_32010200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst)//�����բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	//���:pOccurSour=�¼�����Դ    OI
	//OI=���ƶ���      OI
	//bit=��բ�ִ�      bit-string(SIZE(8))
	//pFixedValue=��ض�ֵ long64����λ��kWh������-4��
	//pFirst=��բ����ʱ�ܼӵ�����  long64����λ��kwh/Ԫ������-4��
//���¼�����Դ����Ϊ�µ��8108����բʱ�ܼӵ�����Ϊ�ܼ��µ�������
//���¼�����Դ����Ϊ�����8107����բʱ�ܼӵ�����Ϊʣ�������/�ѡ�
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
	p8[0]=DataType_OI;
	p8[1]=pOccurSour>>8;
	p8[2]=pOccurSour;
	Len=EVENT_HeadData(0x32010200,p8,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=4;//�ṹ��Ԫ����
//  ���ƶ���      OI��
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_OI;
		p8[Len+1]=OI>>8;
		p8[Len+2]=OI;
		Len+=3;
//  ��բ�ִ�      bit-string(SIZE(8))��
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_bit_string;
		p8[Len+1]=8;
		p8[Len+2]=bit;
		Len+=3;
//	��ض�ֵ      long64����λ��kW������-4����
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFixedValue,8);
		Len+=9;
//	��բ����ʱ�ܼӵ�����  long64����λ��kW������-4����
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFirst,8);
		Len+=9;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=4;//����+5
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//���ƶ���      OI
		p8[1]=0x33;
		p8[2]=0x06;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//��բ�ִ�
		p8[6]=0x33;
		p8[7]=0x06;
		p8[8]=0x02;
		p8[9]=7;
		p8[10]=0;//DataType_OAD;//��ض�ֵ
		p8[11]=0x33;
		p8[12]=0x06;
		p8[13]=0x02;
		p8[14]=8;
		p8[15]=0;//DataType_OAD;//��բ����ʱ�ܼӵ�����
		p8[16]=0x33;
		p8[17]=0x06;
		p8[18]=0x02;
		p8[19]=9;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x32010200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}

//3307 ��ظ澯�¼���Ԫ��=structure
//{
//  �¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    OI��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//  ���ƶ���      OI��
//  ��ض�ֵ      long64����λ��kWh������-4����
//  ��1�������������Ե�����  Data��
//  ��
//  ��n�������������Ե�����  Data
//}
void Event_32030200(u32 pOccurSour,u32 OI,u8* pFixedValue)//��ظ澯�¼���¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	//���:pOccurSour=�¼�����Դ    OI
	//OI=���ƶ���      OI
	//pFixedValue=��ض�ֵ long64����λ��kWh������-4��
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
	p8[0]=DataType_OI;
	p8[1]=pOccurSour>>8;
	p8[2]=pOccurSour;
	Len=EVENT_HeadData(0x32030200,p8,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=2;//�ṹ��Ԫ����
//  ���ƶ���      OI��
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_OI;
		p8[Len+1]=OI>>8;
		p8[Len+2]=OI;
		Len+=3;
//	��ض�ֵ long64����λ��kW������-4����
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFixedValue,8);
		Len+=9;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]+=2;//����+5
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//���ƶ���OI
		p8[1]=0x33;
		p8[2]=0x06;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//��ض�ֵ
		p8[6]=0x33;
		p8[7]=0x06;
		p8[8]=0x02;
		p8[9]=7;
		Len=EVENT_OADData(Len,2);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x32030200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}


#if (USER/100)==9//����
//�����ߴ�����Ϣ��Ԫ��=structure
//{
//�¼���¼���  double-long-unsigned��
//  �¼�����ʱ��  date_time_s��
//  �¼�����ʱ��  date_time_s��
//  �¼�����Դ    NULL��
//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
//һ�������ߴ����� array�����ߴ�����
//}

//�����ߴ�������=structure
//{
//̨���ڵ����� unsigned��
//�ڵ��ַ octet-string��
//�ڵ���Ϣ bit-string(SIZE(8))��
//�ڵ�̨����ַ octet-string
//}
//�ڵ���Ϣ��b0Ϊ������b1-2Ϊ̨��״̬��b3-5Ϊ����b6Ϊ��·�쳣��b7Ϊ����
void Event_34110200(u8* pDATA)//�����ߴ����¼���¼;���:pDATA=1��������;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x34110200,0,2);//��׼�¼�ͷ����;���:OI=�¼�����,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;����:b0-b7�ϱ���ʶ0=���ϱ�1=�ϱ�,b8-b23�ܳ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//�ṹ��Ԫ����
		p8[Len]=DataType_array;
		p8[Len+1]=1;
		Len+=2;
		i=Get_DL698DataLen_S(pDATA,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		MR(ADDR_DATABUFF+Len,(u32)pDATA,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//�¼��и������ݵ���ӦOAD˳���array OAD
		i=p8[1];
		p8[1]++;//����+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//����δ֪���ܱ��¼���Ԫ-6
		p8[1]=0x33;
		p8[2]=0x03;
		p8[3]=0x02;
		p8[4]=6;
		//Len=EVENT_OADData(Len);//��׼�¼�����OAD����;���:����OAD�����Ѷ��뵽ADDR_128KDATABUFF+(32*1024),Len=�¼����г���,Occur=0�ָ�=1����=2��(��=2ʱͬʱ��ʾ�����й�����û����ADDR_128KDATABUFF+(64*1024)��ADDR_128KDATABUFF+(64*1024)+1024);����:�ܳ���
		EventRecord(0x34110200,(u8*)ADDR_DATABUFF,Len,2,Report);//�¼���¼�ļ�;����:д���ֽ���
	}
}
#endif






