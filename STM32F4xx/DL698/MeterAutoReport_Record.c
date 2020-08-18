

//���ܱ������ϱ���¼�ļ�
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/MeterAutoReport_Record.h"
#include "../DL698/RM_Record.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/RM_RxDATA.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"


void MeterAutoReport_Record(u32 PORTn,u8* pDATA,u32 FileID)//���ܱ������ϱ���¼�ļ�;���:pDATA=����ָ��(�ֱ�ΪRecordFileHead+ʱ��+֡�����ͳ���+����)
{
	u32 i;
	u32 x;
	u32 n;
	u32 Len;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	
	Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
	ms=Get_ms();
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			//b7-b0�ļ�����
			if(file->FILEID!=FileID)
			{
				file++;
				continue;
			}
			//�ҵ�
			n=file->NUM_DATA;
		#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
			i=MRR(ADDR_4300_10+1,1);//�ϱ�ͨ����
			if(i==1)
			{//�ϱ�ͨ����=1
				if((file->NUM_GPRSreport[0]>=n)||(file->NUM_ENETreport[0]>=n))
				{
					ms->msfile_Delete(filename);
					filename=recordFILENAMEmax_FLASH;//û�ҵ�
				}
			}
			else
			{//�ϱ�ͨ����=2
				if((file->NUM_GPRSreport[0]>=n)&&(file->NUM_ENETreport[0]>=n))
				{//�����ھ����ϱ�,ɾ���ļ�
					ms->msfile_Delete(filename);
					filename=recordFILENAMEmax_FLASH;//û�ҵ�
				}
				else
				{
					if((file->NUM_GPRSreport[0]>=n)||(file->NUM_ENETreport[0]>=n))
					{//������һ�������ϱ�
						if(n>=NUM_RMmax)
						{//ɾ���ļ�
							ms->msfile_Delete(filename);
							filename=recordFILENAMEmax_FLASH;//û�ҵ�
						}
					}
				}
			}
		#else
			if(file->NUM_GPRSorENETreport[0]>=n)
			{
				ms->msfile_Delete(filename);
				filename=recordFILENAMEmax_FLASH;//û�ҵ�
			}
		#endif
			break;
		}
		file++;
	}
	if(filename>=recordFILENAMEmax_FLASH)
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
				return;
			}
			break;
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//���ļ�
		ms->msfile_lenclear(filename);//��0�ļ���صĳ��ȴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;
	}
	n=file->NUM_DATA;
	if(n>=NUM_RMmax)
	{
		return;
	}

//д�ļ�
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	}
	RecordFileHead=(RecordFileHead_TypeDef*)pDATA;
	Len=RecordFileHead->LEN_DATA;
	while(1)
	{
		x=ms->msfile_write(filename,i,pDATA+sizeof(RecordFileHead_TypeDef),Len);
		if(x!=Len)
		{//д����
			x=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(x)
			{//��ɾ��
				continue;
			}
			return;
		}
		break;
	}
	i+=Len;
	file->LEN_DATA=i;
//д��ַ�б�
	ms->msfile_write(filename,n*sizeof(RecordFileHead_TypeDef),pDATA,sizeof(RecordFileHead_TypeDef));
//������+1
	n++;
	file->NUM_DATA=n;
//�����ϱ�
	//ʼ������

}

//���������ϲ������ݻ���1
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29 �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������,ȫ������Ϊ����˳��������������)
void MeterAutoReport_RM_Record_3011(u32 PORTn,u8* pbuf)//���ܱ�ͣ�������ϱ���¼�ɳ����ļ�;���:pbuf=����ָ��
{
	u32 i;
	u32 x;
	u32 y;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;

//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//���ȼ�:1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�,4=����
//��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��
	Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
	ms=Get_ms();
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	FileID=MeterAutoReportFileID_3011;
	Comm_Ram->msFILESearchCount=0;
	filename=Get_RecordFileName(FileID,0,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	if(filename!=0xffffffff)
	{//�ҵ�
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		i=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
//		x=file->NUM_GPRSreport[0];
//		y=file->NUM_ENETreport[0];
//		if((x>=i)||(y>=i)||(i>=NUM_RMmax))
		if(i>=NUM_RMmax)
		{//������
			x=filename;//�Ĵ��ļ���׼����Ϊ����1
			//ɾ��ԭ�ļ�����1�ļ�
			Comm_Ram->msFILESearchCount=0;
			filename=Get_RecordFileName(FileID,1,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename!=0xffffffff)
			{//�ҵ�
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				i=filename;
				if(i>=recordFILENAMEmax_FLASH)
				{
					i-=recordFILENAMEmax_FLASH;
					i+=FILENAMEmax_FLASH;
				}
				ms->msfile_Delete(i);
			}
			//��Ϊ����1
			filename=x;
			if(filename<FILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			file->FILECOUNT=1;
			filename=0xffffffff;//û�ҵ�
		}
	}
	if(filename==0xffffffff)
	{//û�ҵ�
		AutoDelete_ErrID_RecordFile();//�Զ�ɾ�������Ѳ����ڵļ�¼�ļ�;����:0=ûɾ��,1=��ɾ��
		//�ҿ��ļ�
		//FLASH�ļ�
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
					return;
				}
				break;
			}
	}
	if(file->BLOCK_ENTRY==0)
	{//���ļ�
		ms->msfile_lenclear(filename);//��0�ļ���صĳ��ȴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;
		MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
		MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
	}
//д�ļ�
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//��
		ms->msfile_Delete(filename);
		return;
	}
	//д����
	if(filename>=FILENAMEmax_FLASH)
	{//RAM���ļ�
		i=ms->ms_malloc_sizemax();//���ɷ���ĳߴ�(�ֽ���)
		if(i<(1024*1024))
		{//�ռ�<1M,��������1M�ռ����ڼ�¼���ݶ�ȡ
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y==0)
			{//ûɾ��
				return;
			}
		}
	}
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	}
	RecordFileHead=(RecordFileHead_TypeDef*)pbuf;
	while(1)
	{
		y=RecordFileHead->LEN_DATA;
		y=ms->msfile_write(filename,i,pbuf+sizeof(RecordFileHead_TypeDef),y);
		if(y!=RecordFileHead->LEN_DATA)
		{//д����
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y)
			{//��ɾ��
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//д��ַ�б�
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//������+1
	x++;
	file->NUM_DATA=x;
	
//�����ϱ�
	//ʼ������
	
}










const u8 InitData_MeterAutoReport_3011_CSD[]=
{
//�ɼ����� Data
	DataType_array,
	1,
	DataType_CSD,
	1,0x30,0x11,0x02,0x00,//�洢�¼�Ϊ���ܱ�����¼�
	5,
	0x20,0x22,0x02,0x00,//�¼���¼���
	0x20,0x1e,0x02,0x00,//�¼�����ʱ��
	0x20,0x20,0x02,0x00,//�¼�����ʱ��
	0x20,0x24,0x02,0x00,//�¼�����Դ
	0x33,0x00,0x02,0x00,//�¼��ϱ�״̬
};
void MeterAutoReport_3011(u8* p8rx,u32 Occur)//�ز����ϱ��ӽڵ��¼�(���ܱ�ͣ���¼�);���:pAddr=��·��ģ����յ��Ľ��ջ����е�6�ֽڵ�ַ(����ɼ�����ַ)
{
	u32 PORTn;
	u32 i;
	u32 n;
	u32 offset;
	u32 LEN_RxDataBuff;
	u8* p8;
	u8* p8d;
	u32 FileID;
	u32 filename;
	u32 filecount;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	UARTCtrl_TypeDef* UARTCtrl;
	u32 f;
	
	WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��
//���������ϲ������ݻ���1
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29 �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������,ȫ������Ϊ����˳��������������)
	PORTn=ACSAMPLEPORT;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ��峤��
	if(LEN_RxDataBuff>2000)
	{
		LEN_RxDataBuff=2000;//Ϊ�ϱ�ʱÿ���ݴ������޳���
	}
	p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);
//0 6byte ���ܱ��ַ
	MR((u32)p8,(u32)p8rx,6);
	Data_Inverse((u32)p8,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
	Get_date_time_s(p8+8);//�õ�date_time_s��7�ֽ�
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
	Get_date_time_s(p8+15);//�õ�date_time_s��7�ֽ�
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
	Get_date_time_s(p8+22);//�õ�date_time_s��7�ֽ�
//29 �ɼ���CSD  array CSD(����������)
	offset=29;
	MR((u32)p8+offset,(u32)InitData_MeterAutoReport_3011_CSD,sizeof(InitData_MeterAutoReport_3011_CSD));
	offset+=sizeof(InitData_MeterAutoReport_3011_CSD);
//n  �ɼ�������(����������,ȫ������Ϊ����˳��������������)
	//�¼���¼���
	i=MRR(ADDR_MeterAutoReport_3011_Count,4);//���ܱ�ͣ�������ϱ��¼���¼��ţ�˳�����ɣ�
	i++;
	MWR(i,ADDR_MeterAutoReport_3011_Count,4);//���ܱ�ͣ�������ϱ��¼���¼��ţ�˳�����ɣ�
	p8[offset+0]=DataType_double_long_unsigned;
	p8[offset+1]=i>>24;
	p8[offset+2]=(i>>16)&0xff;
	p8[offset+3]=(i>>8)&0xff;
	p8[offset+4]=i&0xff;
	offset+=5;
	if(Occur==1)
	{//ͣ�緢��
		//�¼�����ʱ��(����:ͣ��ʱ���¼�����ʱ����Ч���Լ������յ��¼���ʱ��Ϊ׼���¼�����ʱ��null������ʱ���¼������ͽ����¼�����Ч)
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//�õ�date_time_s��7�ֽ�
		offset+=8;
		//�¼�����ʱ��
		p8[offset]=0;
		offset+=1;
	}
	else
	{//ͣ��ָ�
		//�¼�����ʱ��(����:ͣ��ʱ���¼�����ʱ����Ч���Լ������յ��¼���ʱ��Ϊ׼���¼�����ʱ��null������ʱ���¼������ͽ����¼�����Ч)
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//�õ�date_time_s��7�ֽ�
		offset+=8;
		
		ms=Get_ms();
		FileID=MeterAutoReportFileID_3011;
		for(filecount=0;filecount<2;filecount++)
		{
			f=0;
			Comm_Ram->msFILESearchCount=0;
			filename=Get_RecordFileName(FileID,filecount,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename!=0xffffffff)
			{//�ҵ�
				if(filename<FILENAMEmax_FLASH)
				{
					file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
				}
				else
				{
					file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
				}
				i=file->LEN_DATA;
				if(i>(NUM_RMmax*sizeof(RecordFileHead_TypeDef)))
				{
					i-=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
				}
				p8d=(u8*)ms->ms_malloc(i);//��̬�����ҵ��ڴ��
				if(p8d)
				{
					ms->ms_free(p8d);//�ͷ��ҵ��ڴ�ѷ���
					ms->msfile_read(filename,NUM_RMmax*sizeof(RecordFileHead_TypeDef),p8d,i);
					n=file->NUM_DATA;
					if((n*sizeof(RecordFileHead_TypeDef))>LEN_128KDATABUFF)
					{
						n=0;
					}
					ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,n*sizeof(RecordFileHead_TypeDef));
					RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
					Addr0=p8[0]|(p8[1]<<8);
					Addr1=p8[2]|(p8[3]<<8);
					Addr2=p8[4]|(p8[5]<<8);
					for(i=0;i<n;i++)
					{
						if(RecordFileHead->Addr0==Addr0)
						{
							if(RecordFileHead->Addr1==Addr1)
							{
								if(RecordFileHead->Addr2==Addr2)
								{
									offset-=8;
									if(p8d[offset-sizeof(RecordFileHead_TypeDef)]==DataType_date_time_s)
									{//�ҵ���¼�ķ���ʱ��
										f=1;
										MR((u32)p8+offset,(u32)p8d+offset-sizeof(RecordFileHead_TypeDef),8);
									}
									offset+=8;
								}
							}
						}
						p8d+=RecordFileHead->LEN_DATA;
						RecordFileHead++;
					}
				}
			}
			if(f)
			{
				break;
			}
		}
		
		//�¼�����ʱ��
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//�õ�date_time_s��7�ֽ�
		offset+=8;
	}
	//�¼�����Դ
	p8[offset]=DataType_OAD;
	p8[offset+1]=0xf2;//F2090201
	p8[offset+2]=0x09;
	p8[offset+3]=0x02;
	p8[offset+4]=0x01;
	offset+=5;
	//�¼��ϱ�״̬
	p8[offset]=DataType_array;
	p8[offset+1]=1;
	offset+=2;
	//OAD_ͨ��Ϊ�ز�/΢��������     	F2090201
	p8[offset]=DataType_structure;
	p8[offset+1]=2;
	p8[offset+2]=DataType_OAD;
	p8[offset+3]=0xf2;
	p8[offset+4]=0x09;
	p8[offset+5]=0x02;
	p8[offset+6]=0x01;
	//bit0:�¼������ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit1:�¼������ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϣ�
	//bit2:�¼��������ָ����ϱ���ʶ��0��δ�ϱ���1�����ϱ���
	//bit3:�¼��������ָ����ϱ�ȷ�ϱ�ʶ��0��δȷ�ϣ�1����ȷ�ϡ�
	p8[offset+7]=DataType_unsigned;
	p8[offset+8]=1;//���ϱ�
	offset+=9;

	i=offset;
	i-=8;
	p8[6]=i;//2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
	p8[7]=i>>8;
	MeterAutoReport_RM_Record_3011(PORTn,p8);//���ܱ�ͣ�������ϱ���¼�ɳ����ļ�;���:pDATA=����ָ��
	UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������
}





