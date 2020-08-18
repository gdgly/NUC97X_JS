
//�����¼�ļ�
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CONGEAL_Record.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Device/MEMRW.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_REPORT.h"
#include "../Calculate/Calculate.h"
#include "../DL698/CALC_TAG.h"



u32 Get_CongealFileID(u32 OI)//�õ������ļ���FileID
{
	OI<<=8;
	OI|=1;//�ļ�����
	return OI;
}

u32 Get_CongealFileName(u32 OI,u32 FileCount,u8* pTime)//���������ļ���:���:OI,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
{
	u32 i;
	msFILE_TypeDef *file;
	u32 SearchCount;
	
	OI=Get_CongealFileID(OI);//�õ������ļ���FileID
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
			if(file->FILEID!=OI)
			{
				file++;
				continue;
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
			//�洢ʱ��
			if(pTime)
			{
				for(i=0;i<6;i++)//�벻�Ƚ�
				{
					if(pTime[i]!=file->save_date_time_s[i])
					{
						break;
					}
				}
				if(i<6)
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

u32 ADDR_PARAMETER_50xx(u32 OI)//���ض��������ַ,0=��Ч
{
	switch(OI)
	{
		case 0x5000://˲ʱ����
			return ADDR_5000;
		case 0x5001://�붳��
			return ADDR_5001;
		case 0x5002://���Ӷ���
			return ADDR_5002;
		case 0x5003://Сʱ����
			return ADDR_5003;
		case 0x5004://�ն���
			return ADDR_5004;
		case 0x5005://�����ն���
			return ADDR_5005;
		case 0x5006://�¶���
			return ADDR_5006;
		case 0x5007://�궳��
			return ADDR_5007;
		case 0x5008://ʱ�����л�����
			return ADDR_5008;
		case 0x5009://��ʱ�α��л�����
			return ADDR_5009;
		case 0x500A://���ʵ���л�����
			return ADDR_500A;
		case 0x500B://�����л�����
			return ADDR_500B;
		case 0x5011://���ݽ��㶳��
			return ADDR_5011;
		default:
			return 0;
	}
}

u32 DEPTH_CongealFile(u32 OI)//�����ļ��洢���;����:�洢���(��OAD������ֵ)
{
	u32 i;
	u32 addr;
	u32 array;
	u32 depth;
	u8* p8;
	
	addr=ADDR_PARAMETER_50xx(OI);//���ض��������ַ,0=��Ч
	if(addr==0)
	{
		return 0;
	}
#if LENmax_50xx>LEN_DATABUFF
	#error
#endif
	MR(ADDR_DATABUFF,addr,LENmax_50xx);
	p8=(u8*)ADDR_DATABUFF;
	if(p8[0]==DataType_array)
	{
		array=p8[1];//ԭ��������
		if(array>127)
		{
			array=0;
		}
	}
	else
	{
		array=0;
	}
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
	p8+=2;
	depth=0;
	while(array--)
	{
		i=(p8[11]<<8)|p8[12];
		if(i>depth)
		{
			depth=i;
		}
		p8+=13;
	}
	if(depth==0)
	{
		depth=1;//��Сֵ
	}
	switch(OI)
	{
		case 0x5000://˲ʱ����
			i=1;//Ĭ��������
			break;
		case 0x5001://�붳��
			i=1;//Ĭ��������
			break;
		case 0x5002://���Ӷ���
			i=96*2;//Ĭ��������
			break;
		case 0x5003://Сʱ����
			i=24;//Ĭ��������
			break;
		case 0x5004://�ն���
			i=62;//Ĭ��������
			break;
		case 0x5005://�����ն���
			i=1;//Ĭ��������
			break;
		case 0x5006://�¶���
			i=12;//Ĭ��������
			break;
		case 0x5007://�궳��
			i=1;//Ĭ��������
			break;
		case 0x5008://ʱ�����л�����
			i=1;//Ĭ��������
			break;
		case 0x5009://��ʱ�α��л�����
			i=1;//Ĭ��������
			break;
		case 0x500A://���ʵ���л�����
			i=1;//Ĭ��������
			break;
		case 0x500B://�����л�����
			i=1;//Ĭ��������
			break;
		case 0x5011://���ݽ��㶳��
			i=1;//Ĭ��������
			break;
	}
	if(depth>i)
	{
		depth=i;
	}
	return depth;
}

void CountInc_CongealFile(u32 OI)//�����ļ�����=0ʱ,ȫ�������ļ��ļ�����+1,ɾ���ﵽ��¼��ȵ��ļ�
{
	u32 i;
	u32 FileID;
	u32 set_depth;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
	FileID=Get_CongealFileID(OI);//�õ������ļ���FileID
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			if(file->FILECOUNT==0)
			{
				if(FileID==file->FILEID)
				{
					if(file->LEN_DATA)
					{
						break;
					}
				}
			}
		}
		file++;
	}
	if(filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
	{//�м���=0���ļ�
		ms=Get_ms();
		set_depth=DEPTH_CongealFile(OI);//�����ļ��洢���;����:�洢���(��OAD������ֵ)
		if(set_depth==0)
		{
			set_depth=1;//��Сֵ
		}
		if(set_depth>0xfffe)
		{
			set_depth=0xfffe;//���ֵ
		}
		set_depth--;
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
		{
			if(filename==recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			}
			if(file->BLOCK_ENTRY)
			{
				if(FileID==file->FILEID)
				{
					if(file->FILECOUNT>=set_depth)
					{
						i=filename;
						if(i>=recordFILENAMEmax_FLASH)
						{
							i-=recordFILENAMEmax_FLASH;
							i+=FILENAMEmax_FLASH;
						}
						ms->msfile_Delete(i);
					}
					else
					{
						file->FILECOUNT++;
					}
				}
			}
			file++;
		}
	}
}

void CongealFile_Time(u32 OI,u8* pOUT)//�����ļ�ʱ��(����������)
{
	u32 i;
	
	for(i=0;i<7;i++)
	{
		pOUT[i]=Comm_Ram->DL698YMDHMS[i];
	}
	switch(OI)
	{
		case 0x5000://˲ʱ����
			break;
		case 0x5001://�붳��
			break;
		case 0x5002://���Ӷ���
			//��2000��1��1��0ʱ0��0��ÿ15�ֵı�׼ʱ��
			MR((u32)pOUT,ADDR_TerminalCongeal_Time,7);
			break;
		case 0x5003://Сʱ����
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5004://�ն���
			pOUT[4]=0;
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5005://�����ն���
			break;
		case 0x5006://�¶���
			pOUT[3]=1;
			pOUT[4]=0;
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5007://�궳��
			pOUT[2]=1;
			pOUT[3]=1;
			pOUT[4]=0;
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5008://ʱ�����л�����
			pOUT[6]=0;
			break;
		case 0x5009://��ʱ�α��л�����
			pOUT[6]=0;
			break;
		case 0x500A://���ʵ���л�����
			pOUT[6]=0;
			break;
		case 0x500B://�����л�����
			pOUT[6]=0;
			break;
		case 0x5011://���ݽ��㶳��
			pOUT[6]=0;
			break;
	}
}


u32 Get_CongealFileData_RCSD(u8 *pRCSD,u8 *pRecord,u8* pTime,u32 NUM_DATA,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//��������ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pRecordָ�����ݿ�,pTimeָ�򶳽�ʱ��(����������),NUM_DATA���������б���,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
{
	u32 i;
	u32 x;
	u32 n;
	u32 OADn;
	u32 OADm;
	u32 Len;
	u32 Offset;
	u8* p8;
	CongealFileHead_TypeDef* CongealFileHead;//�����¼�ļ�ͷ�ļ�
	u32 fDATA;//����ѡ��OAD���ݱ�־
	
	
	fDATA=0;//����ѡ��OAD���ݱ�־
	n=pRCSD[0];//RCSD��SEQUENCE OF����
	pRCSD++;
	if(NUM_DATA>NUMmax_50xx)//��OAD��¼�б����
	{
		NUM_DATA=NUMmax_50xx;
	}
	Len=0;
	while(n--)
	{
//��������Լ��
//2byte �����ݸ���
//7byte date_time_s ���ݶ���ʱ��
//1byte 
//NUMmax_50xx*sizeof(CongealFileHead_TypeDef) byte
//����
		
		OADn=(pRCSD[1]<<24)+(pRCSD[2]<<16)+(pRCSD[3]<<8)+pRCSD[4];
		switch(OADn)
		{
			case 0x40010200://ͨ�ŵ�ַ
			case 0x202A0200://Ŀ���������ַ
				if(LEN_DATAmax<9)
				{
					return 0;
				}
				pout[Len]=DataType_TSA;
				pout[Len+1]=7;
				pout[Len+2]=5;
				MR((u32)pout+Len+3,ADDR_4001+2,6);
				Len+=9;
				LEN_DATAmax-=9;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x20210200://���ݶ���ʱ��
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pTime,7);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
		}
		if(pRCSD[0]==0)
		{//OAD
			CongealFileHead=(CongealFileHead_TypeDef*)(pRecord);
			Offset=0;
			for(i=0;i<NUM_DATA;i++)
			{
				OADm=(CongealFileHead->OAD[0]<<24)|(CongealFileHead->OAD[1]<<16)|(CongealFileHead->OAD[2]<<8)|(CongealFileHead->OAD[3]<<0);
				if(OADn==OADm)
				{
					break;
				}
				if((OADm&0xff)==0)
				{//����ʱΪȫ��Ԫ��
					if(OADn&0xff)
					{//��ȡ1��Ԫ��
						if((OADn>>8)==(OADm>>8))
						{
							break;
						}
					}
				}
				Offset+=CongealFileHead->LEN_data[0]+(CongealFileHead->LEN_data[1]<<8);
				CongealFileHead++;
			}
			if(i>=NUM_DATA)
			{//û�ҵ�
				if(LEN_DATAmax<1)
				{
					return 0;
				}
				pout[Len]=0;//NULL
				Len+=1;
				LEN_DATAmax-=1;
			}
			else
			{//�ҵ�
				x=CongealFileHead->LEN_data[0]+(CongealFileHead->LEN_data[1]<<8);
				p8=pRecord+(NUMmax_50xx*sizeof(CongealFileHead_TypeDef))+Offset;
				if((OADm&0xff)==0)
				{//����ʱΪȫ��Ԫ��
					if(OADn&0xff)
					{//��ȡ1��Ԫ��
						p8=Get_Element(p8,OADn&0xff,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
						if(p8)
						{//�ҵ�Ԫ��
							x=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
						else
						{//û�ҵ�Ԫ��
							if(LEN_DATAmax<1)
							{
								return 0;
							}
							pout[Len]=0;//NULL
							Len+=1;
							LEN_DATAmax-=1;
							continue;
						}
					}
				}
				if(LEN_DATAmax<x)
				{
					return 0;
				}
				MR((u32)pout+Len,(u32)p8,x);
				Len+=x;
				LEN_DATAmax-=x;
				fDATA=1;//����ѡ��OAD���ݱ�־
			}
			pRCSD+=5;
		}
		else
		{//ROAD
			x=pRCSD[5];//SEQUENCE OF����
			pRCSD+=6+(x*4);
			if(LEN_DATAmax<(2+x))
			{
				return 0;
			}
			pout[Len]=DataType_array;
			pout[Len+1]=x;
			for(i=0;i<x;i++)
			{
				pout[Len+2+i]=0;//NULL
			}
			Len+=2+x;
			LEN_DATAmax-=2+x;
		}
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

u32 Get_CongealFileData_Fixed(u32 OI,u32 FileCount,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ������ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
{
	u32 i;
	u32 n;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
	
	ms=(ms_Type *)Get_ms();
	filename=Get_CongealFileName(OI,FileCount,0);//���������ļ���:���:OI,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	if(filename==0xffffffff)
	{
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
	if(file->BLOCK_ENTRY==0)
	{
		return 0;
	}
	switch(pRSD[0])
	{
		case 0:
//��ѡ��       [0]  NULL
			break;
		case 1:
//Selector1��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ֵ            Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://���ݶ���ʱ��
					pRSD+=4;
					if(pRSD[0]!=DataType_date_time_s)
					{//��ֵData=��
						return 0;
					}
					pRSD+=1;
					//��ֵData=date_time_s
					for(i=0;i<6;i++)
					{//�벻�Ƚ�
						if(pRSD[i]!=file->save_date_time_s[i])
						{
							return 0;
						}
					}
					break;

				default:
					return 0;
			}
			break;
		case 2:
//Selector2��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ʼֵ          Data��
//  ����ֵ          Data��
//  ���ݼ��        Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://���ݶ���ʱ��
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=0)
					{//ʱ��!=��ʼֵ
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=2)
					{//ʱ��>=����ֵ
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 3:
//Selector3��=SEQUENCE OF Selector2
			pRSD+=2;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://���ݶ���ʱ��
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=0)
					{//ʱ��!=��ʼֵ
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=2)
					{//ʱ��>=����ֵ
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 4:
//Selector4��=SEQUENCE
//{
//  �ɼ�����ʱ��  date_time_s��
//  ���ܱ���    MS
//}
			return 0;
		case 5:
//Selector5��=SEQUENCE
//{
//  �ɼ��洢ʱ��  date_time_s��
//  ���ܱ���    MS
//}
			return 0;
		case 6:
//Selector6��=SEQUENCE
//{
//  �ɼ�����ʱ����ʼֵ  date_time_s��
//  �ɼ�����ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			return 0;
		case 7:
//Selector7��=SEQUENCE
//{
//  �ɼ��洢ʱ����ʼֵ  date_time_s��
//  �ɼ��洢ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			return 0;
		case 8:
//Selector8��=SEQUENCE
//{
//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			return 0;
		case 9:
//Selector9��=SEQUENCE
//{
//  �ϵ�n�μ�¼  unsigned
//}
			break;//����ʱ���ļ��������ҵ�����Ч
		case 10:
//Selector10��=SEQUENCE
//{
//��n����¼  unsigned��
//���ܱ���  MS
//}
			return 0;
		default:
			return 0;
	}
		
	n=file->NUM_DATA;
	i=file->LEN_DATA;
	if(i>LEN_128KDATABUFF)
	{
		return 0;
	}
	ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,i);
	i=Get_CongealFileData_RCSD(pRCSD,(u8*)ADDR_128KDATABUFF,file->save_date_time_s,n,pOUT,LEN_OUTmax,ALLDATANULLSEL);//��������ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pRecordָ��OAD�б�����ݿ鿪ʼ(��ʼ2�ֽ�Ϊ���б���),p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
	return i;
}

u32 Get_CongealFileData(u32 OI,u32 FileCount,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ������ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
{
	u32 LEN;
	Comm_Ram->msFILESearchCount=0;
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
	{
		LEN=Get_CongealFileData_Fixed(OI,FileCount,pRSD,pRCSD,pOUT,LEN_OUTmax);//�õ������ļ�����
		if(LEN)
		{
			return LEN;
		}
	}
	return 0;
}



void CongealFile(u32 OI)//�����ļ�
{
	u32 i;
	u32 x;
	u32 Addr;
	u32 array;
	u32 OAD;
	u32 FileID;
	u32 filename;
	u32 NUM_DATA;
	u32 LEN_DATA;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u8* pPARAs;
	u8* pPARAd;
	u8* pDATA;
	CongealFileHead_TypeDef* CongealFileHead;
	
	Addr=ADDR_PARAMETER_50xx(OI);//���ض��������ַ,0=��Ч
	if(Addr==0)
	{
		return;
	}
	Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
	ms=Get_ms();
	FileID=Get_CongealFileID(OI);//�õ������ļ���FileID
	Comm_Ram->msFILESearchCount=0;
	CongealFile_Time(OI,(u8*)ADDR_DATABUFF);//�����ļ�ʱ��(����������)
	filename=Get_CongealFileName(OI,0xffffffff,(u8*)ADDR_DATABUFF);//���������ļ���:���:OI,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	if(filename!=0xffffffff)
	{//�Ѵ���
		//ɾ���ļ�
		if(filename>=recordFILENAMEmax_FLASH)
		{
			filename-=recordFILENAMEmax_FLASH;
			filename+=FILENAMEmax_FLASH;
		}
		ms->msfile_Delete(filename);
		filename=0xffffffff;//û�ҵ�
	}
	if(filename==0xffffffff)
	{//û�ҵ�
		CountInc_CongealFile(OI);//�����ļ�����=0ʱ,ȫ����¼�ļ��ļ�����+1,ɾ���ﵽ��¼��ȵ��ļ�
		//��FLASH���ļ�
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
		CongealFile_Time(OI,file->save_date_time_s);//�����ļ�ʱ��(����������)
	}
//���ļ�����
	NUM_DATA=0;
	LEN_DATA=NUMmax_50xx*sizeof(CongealFileHead_TypeDef);
	//д����
//�����������Ա��=array һ�������Ķ�������//ɾ��ʱarray�����ݵ�Ԫ��Ϊ0(NULL)
//һ�������Ķ������ԡ�=structure
//{
//  ��������  long-unsigned��
//  ������������������  OAD��
//  �洢���  long-unsigned
//}
#if LENmax_50xx>(4*1024)
	#error
#endif
	pPARAs=(u8*)ms->ms_malloc(4*1024);
	if(pPARAs==0)
	{
		return;
	}
	pPARAd=pPARAs;
	pDATA=(u8*)ms->ms_malloc(128*1024);
	if(pDATA==0)
	{
		ms->ms_free(pPARAs);
		return;
	}
	MR((u32)pPARAd,Addr,LENmax_50xx);
	if(pPARAd[0]!=DataType_array)
	{
		ms->ms_free(pPARAd);
		ms->ms_free(pDATA);
		return;
	}
	array=pPARAd[1];
	pPARAd+=2;
	CongealFileHead=(CongealFileHead_TypeDef*)(pDATA);
	while(array--)
	{
		if(LEN_DATA>=(128*1024))
		{
			break;
		}
		OAD=(pPARAd[6]<<24)|(pPARAd[7]<<16)|(pPARAd[8]<<8)|pPARAd[9];
		i=GET_OAD_DATA(ACSAMPLEPORT,OAD,(u8*)pDATA+LEN_DATA,(128*1024)-LEN_DATA);
		CongealFileHead->LEN_data[0]=i;
		CongealFileHead->LEN_data[1]=i>>8;
		CongealFileHead->OAD[0]=OAD>>24;
		CongealFileHead->OAD[1]=(OAD>>16)&0xff;
		CongealFileHead->OAD[2]=(OAD>>8)&0xff;
		CongealFileHead->OAD[3]=(OAD>>0)&0xff;
		LEN_DATA+=i;
		NUM_DATA++;
		CongealFileHead++;
		pPARAd+=13;
	}
//д�ļ�
	while(1)
	{
		x=ms->msfile_write(filename,0,(u8*)pDATA,LEN_DATA);
		if(x!=LEN_DATA)
		{//д����
			x=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(x)
			{//��ɾ��
				continue;
			}
			ms->ms_free(pPARAs);
			ms->ms_free(pDATA);
			return;
		}
		break;
	}
	ms->ms_free(pPARAs);
	ms->ms_free(pDATA);
//��
	file->LEN_DATA=LEN_DATA;
	file->NUM_DATA=NUM_DATA;
	
//�����ϱ�
	i=MRR(ADDR_4300_8+1,1);
	if(i==0)
	{//�����������ϱ�
		for(i=0;i<NUMmax_601C;i++)
		{
		#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
			file->NUM_GPRSreport[i]=NUM_DATA;
			file->NUM_ENETreport[i]=NUM_DATA;
		#else
			file->NUM_GPRSorENETreport[i]=NUM_DATA;
		#endif
		}
	}
	else
	{//���������ϱ�
	//�ϱ�ͨ��
		REPORT_PORTn(file);//ͨ�������ϱ�ʱ�����ϱ����ݸ���=��ǰ���ݸ�����ʹ��ADDR_DATABUFF
	}
}

extern const u8 InitData_2131[];
extern const u8 InitData_2140[];
extern const u8 Init_EnergyData_double_long_unsigned[];
void CONGEAL_Record(void)//����
{
	u32 i;
	u64 YYMDHMSs;
	u64 YYMDHMSc;
	u64 d64;
	u32 CongealFlag;//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
	u8* p8;
	
	p8=(u8*)ADDR_TerminalCongeal_Time;
	if(p8[7]==Comm_Ram->DL698YMDHMS[6])
	{
		return;
	}
	p8[7]=Comm_Ram->DL698YMDHMS[6];
	CongealFlag=0;//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
	YYMDHMSs=MRR(ADDR_TerminalCongeal_Time,7);
	YYMDHMSc=MRR(ADDR_DL698YMDHMS,7);
	
	if((YYMDHMSs&0xffffffff)!=(YYMDHMSc&0xffffffff))
	{//�ձ仯
		CongealFlag|=1;//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
	}
	if((YYMDHMSs&0xffffff)!=(YYMDHMSc&0xffffff))
	{//�±仯
		CongealFlag|=2;//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
	}

	//�ֱ仯(���߶���)
	//��ǰʱ���2000��1��1��0ʱ0��0��
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=2000>>8;//��
	p8[1]=2000&0xff;
	p8[2]=1;//��
	p8[3]=1;//��
	p8[4]=0;//ʱ
	p8[5]=0;//��
	p8[6]=0;//��
	i=YYMDHMS_Sub_hex(p8,(u8*)ADDR_DL698YMDHMS);//hexʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
	if(i)
	{
		if(i>>31)
		{//��
			i=~i;
			i++;
		}
		i/=60;//��Ϊ��
		i/=CongealTime_5002;//�ֶ���(����)���ʱ���
		i*=CongealTime_5002;
		i*=60;
		pYYMDHMS_AddS_hex(p8,i);//��������ʱ�����S��,���ؼ�S������������ʱ����
		d64=MRR(ADDR_DATABUFF,7);
		if((YYMDHMSs&0xffffffffffff)!=(d64&0xffffffffffff))
		{//�ֱ仯(���߶���)
			YYMDHMSc=d64;
			CongealFlag|=4;//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
		}
	}

	
	if(CongealFlag==0)
	{
		return;
	}
	MWR(YYMDHMSc,ADDR_TerminalCongeal_Time,7);
	
//�ֶ���(���߶���)
	if(CongealFlag&4)//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
	{//�ֶ���
		CongealFile(0x5002);//�����ļ�
		
//�ֶ������0
	}
	
	
//�ն���
	if(CongealFlag&1)//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
	{//�ն���
		CongealFile(0x5004);//�����ļ�
		
		
//�ն������0
		MC(0,ADDR_BroadcasTimeFlag,1);//���ܱ��չ㲥Уʱ��־
//��ѹ�ϸ���
		MR(ADDR_2131,(u32)InitData_2131,2+23);//A��
		MR(ADDR_2132,(u32)InitData_2131,2+23);//B��
		MR(ADDR_2133,(u32)InitData_2131,2+23);//C��

//����й����ʼ�����ʱ��
		MR(ADDR_2140,(u32)InitData_2140,12);
//ͨ������
		MC(0,ADDR_2200+3,4);
//����ʱ��
		MC(0,ADDR_2203+3,4);
//��λ�ۼƴ���
		MC(0,ADDR_2204+3,2);
//����������յ�����
	#if NUMmax_IMPIN!=0
		for(i=0;i<NUMmax_IMPIN;i++)
		{
			MW((u32)Init_EnergyData_double_long_unsigned,ADDR_impEnergy_Day+(i*LENper_impEnergy),LENper_impEnergy);
		}
	#endif
//�ܼ���
		Init_Data230x_D();//��ʼ���ܼ���������
		
	}
	
//�¶���
	if(CongealFlag&2)//�����־:B0=�ն���,B1=�¶���,B2=�ֶ��ᣬB3...
	{//�¶���
		CongealFile(0x5006);//�����ļ�
		
		
//�¶������0
//��ѹ�ϸ���
		MR(ADDR_2131+2+23,(u32)InitData_2131+2+23,23);//A��
		MR(ADDR_2132+2+23,(u32)InitData_2131+2+23,23);//B��
		MR(ADDR_2133+2+23,(u32)InitData_2131+2+23,23);//C��
//����й����ʼ�����ʱ��
		MR(ADDR_2141,(u32)InitData_2140,12);
//ͨ������
		MC(0,ADDR_2200+8,4);
		MC(0,ADDR_2200_EventRecordFags,1);//ͨ�����������¼��Ѽ�¼��־,�¸���ʱ��0
//����ʱ��
		MC(0,ADDR_2203+8,4);
//��λ�ۼƴ���
		MC(0,ADDR_2204+6,2);
//����������µ�����
	#if NUMmax_IMPIN!=0
		for(i=0;i<NUMmax_IMPIN;i++)
		{
			MW((u32)Init_EnergyData_double_long_unsigned,ADDR_impEnergy_Month+(i*LENper_impEnergy),LENper_impEnergy);
		}
	#endif
//�ܼ���
		Init_Data230x_M();//��ʼ���ܼ���������
		
	}
}














