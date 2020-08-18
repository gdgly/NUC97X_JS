
//�Զ��ѱ��¼�ļ�
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/SearchMeter_Record.h"
#include "../DL698/RM_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "../DL698/DL698_JSON.h"


void DEL_SearchMeter_Record(u32 FileID)//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
{
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
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
			ms=Get_ms();
			ms->msfile_Delete(filename);
			return;
		}
		file++;
	}
}

//void SearchMeter_Record_JS_ONE(u64 CollectorAddr,u64 MeterAddr)
//{
//	u32 i;
//	u32 y;
//	u32 x;
//	u64 d64a;
//	u64 d64b;
//	u8 *p8s;
//	u8 *p8d;
//	
//	
//	x=Terminal_Router->NoteNum1;//��֪�ܴӽڵ���
//	if((x+1)<=NUM_RMmax)
//	{
//		d64a=MeterAddr;
//		for(i=0;i<x;i++)
//		{
//			d64b=MRR((u32)&NoteList1->List1[i].Addr,6);
//			if(d64a==d64b)
//			{
//				MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//�ɼ���
//				break;//ԭ�Ѵ���
//			}
//		}
//		if(i==x)
//		{//ԭû����
//				for(i=0;i<x;i++)
//				{
//					d64b=MRR((u32)&NoteList1->List1[i].CollectorAddr,6);
//					if(d64a==d64b)
//					{
//						MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//�ɼ���
//						break;//ԭ�Ѵ���
//					}
//				}
//				if(i==x)
//				{//ԭû����
//					for(i=0;i<NUM_RMmax;i++)
//					{
//						p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
//						if(p8s[0]==DataType_structure)
//						{
//							p8d=Get_Element(p8s,2,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
//							if(p8d)
//							{
//								d64b=MRR((u32)p8d+3,6);
//								d64b=R_Inverse(d64b,6);
//								if(d64b==d64a)
//								{
//									break;
//								}
//							}
//						}
//					}
//					if(i==NUM_RMmax)
//						return;
//					MC(0,(u32)&NoteList1->List1[x],LEN_NoteList1);
//					MWR(d64a,(u32)&NoteList1->List1[x].Addr,6);
//					MWR(CollectorAddr,(u32)&NoteList1->List1[x].CollectorAddr,6);//�ɼ�����ַ
//					x++;
//					Terminal_Router->NoteNum1=x;//��ǰ·�������ܴӽڵ���
//				}
//			}
//		}
//}
#if 0
void SearchMeter_CollectorAddrRecord_JS(u64 CollectorAddr)
{
	u32 i;
	u32 y;
	u32 x;
	u64 d64a;
	u64 d64b;
	u8 *p8s;
	u8 *p8d;
	
	x=Terminal_Router->NoteNum1;//��֪�ܴӽڵ���
	if((x+1)<=NUM_RMmax)
	{
		d64a=CollectorAddr;
		for(i=0;i<x;i++)
		{
			d64b=MRR((u32)&NoteList1->List1[i].Addr,6);
			if(d64a==d64b)
			{
				MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//�ɼ���
				break;//ԭ�Ѵ���
			}
		}
		if(i==x)
		{//ԭû����
				for(i=0;i<x;i++)
				{
					d64b=MRR((u32)&NoteList1->List1[i].CollectorAddr,6);
					if(d64a==d64b)
					{
						MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//�ɼ���
						break;//ԭ�Ѵ���
					}
				}
				if(i==x)
				{//ԭû����
					for(i=0;i<NUM_RMmax;i++)
					{
						p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
						if(p8s[0]==DataType_structure)
						{
							p8d=Get_Element(p8s,2,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							if(p8d)
							{
								d64b=MRR((u32)p8d+3,6);
								d64b=R_Inverse(d64b,6);
								if(d64b==d64a)
								{
									break;
								}
							}
						}
					}
					if(i==NUM_RMmax)
						return;
					MC(0,(u32)&NoteList1->List1[x],LEN_NoteList1);
					MWR(d64a,(u32)&NoteList1->List1[x].Addr,6);
					MWR(CollectorAddr,(u32)&NoteList1->List1[x].CollectorAddr,6);//�ɼ�����ַ
					NoteList1->List1[x].user=1;
					x++;
					Terminal_Router->NoteNum1=x;//��ǰ·�������ܴӽڵ���
					myprintf("[%s:%d]:д��ģ���ӽڵ���Ϣ�ɹ�:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum1);
				}
			}
		}
}

void SearchMeter_Record_JS(u64 CollectorAddr,u32 NoteNum,u8 *p8rx)
{
	u32 i;
	u32 y;
	u32 x;
	u64 d64a;
	u64 d64b;
	u8 *p8s;
	u8 *p8d;
	
	x=Terminal_Router->NoteNum1;//��֪�ܴӽڵ���
	for(y=0;y<NoteNum;y++)
	{
		if((x+1)<=NUM_RMmax)
		{
			d64a=MRR(((u32)p8rx)+2+(7*y),6);
			for(i=0;i<x;i++)
			{
				//�����жϵ��ǵ��ĵ�ַ
				d64b=MRR((u32)&NoteList1->List1[i].Addr,6);
				if(d64a==d64b)
				{
					MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//�ɼ���
					break;//ԭ�Ѵ���
				}
			}
			if(i==x)
			{//ԭû����
					for(i=0;i<x;i++)
					{
						d64b=MRR((u32)&NoteList1->List1[i].CollectorAddr,6);
						if(d64a==d64b)
						{
							MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//�ɼ���
							break;//ԭ�Ѵ���
						}
					}
					if(i==x)
					{//ԭû����
						for(i=0;i<NUM_RMmax;i++)
						{
							p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							if(p8s[0]==DataType_structure)
							{
								p8d=Get_Element(p8s,2,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
								if(p8d)
								{
									d64b=MRR((u32)p8d+3,6);
									d64b=R_Inverse(d64b,6);
									if(d64b==d64a)
									{
										break;
									}
								}
							}
						}
						if(i==NUM_RMmax)
						{
							myprintf("[%s:%d]:���ַ�ڵ�����Ҳû���ҵ�\n",(u32)&__func__,(u32)__LINE__,0);
							break;
						}
						MC(0,(u32)&NoteList1->List1[x],LEN_NoteList1);
						MWR(d64a,(u32)&NoteList1->List1[x].Addr,6);
						MWR(CollectorAddr,(u32)&NoteList1->List1[x].CollectorAddr,6);//�ɼ�����ַ
						NoteList1->List1[x].user=1;
						x++;
						Terminal_Router->NoteNum1=x;//��ǰ·�������ܴӽڵ���
						myprintf("[%s:%d]:д��ӽڵ���Ϣ�ɹ�:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum1);
						//��ӡ��ÿ���ѱ�Ľ��
					}
				}
			}
		}
}
#else
void SearchMeter_CollectorAddrRecord_JS(u64 CollectorAddr)
{
	u32 i;
	//u32 y;
	u32 x;
	u64 d64a;
	u64 d64b;
	u8 *p8s;
	u8 *p8d;
	
	x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:д��ӽڵ���Ϣ�ɹ�:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
	#endif
	if((x+1)<=NUM_RMmax)
	{
		d64a=CollectorAddr;
		for(i=0;i<x;i++)
		{
			d64b=MRR((u32)&NoteList->List[i].Addr,6);
			if(d64a==d64b)
			{
				MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//�ɼ���
				break;//ԭ�Ѵ���
			}
		}
		if(i==x)
		{//ԭû����
				for(i=0;i<x;i++)
				{
					d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
					if(d64a==d64b)
					{
						MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//�ɼ���
						break;//ԭ�Ѵ���
					}
				}
				if(i==x)
				{//ԭû����
					for(i=0;i<NUM_RMmax;i++)
					{
						p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
						if(p8s[0]==DataType_structure)
						{
							p8d=Get_Element(p8s,2,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							if(p8d)
							{
								d64b=MRR((u32)p8d+3,6);
								d64b=R_Inverse(d64b,6);
								if(d64b==d64a)
								{
									break;
								}
							}
						}
					}
					if(i==NUM_RMmax)
						return;
					MC(0,(u32)&NoteList->List[x],LEN_NoteList);
					MWR(d64a,(u32)&NoteList->List[x].Addr,6);
					MWR(CollectorAddr,(u32)&NoteList->List[x].CollectorAddr,6);//�ɼ�����ַ
					//NoteList->List[i][x].user=1;
					x++;
					Terminal_Router->NoteNum=x;//��ǰ·�������ܴӽڵ���
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:д��ӽڵ���Ϣ�ɹ�:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
					#endif
				}
			}
		}
}

void SearchMeter_Record_JS(u64 CollectorAddr,u32 NoteNum,u8 *p8rx)
{
	u32 i;
	u32 y;
	u32 x;
	u64 d64a;
	u64 d64b;
	u8 *p8s;
	u8 *p8d;
	
	x=Terminal_Router->NoteNum;//��֪�ܴӽڵ���
	myprintf("[%s:%d]:д��ӽڵ���Ϣ�ɹ�:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
	for(y=0;y<NoteNum;y++)
	{
		if((x+1)<=NUM_RMmax)
		{
			d64a=MRR(((u32)p8rx)+2+(7*y),6);
			for(i=0;i<x;i++)
			{
				//�����жϵ��ǵ��ĵ�ַ
				d64b=MRR((u32)&NoteList->List[i].Addr,6);
				if(d64a==d64b)
				{
					MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//�ɼ���
					break;//ԭ�Ѵ���
				}
			}
			if(i==x)
			{//ԭû����
					for(i=0;i<x;i++)
					{
						d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
						if(d64a==d64b)
						{
							MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//�ɼ���
							break;//ԭ�Ѵ���
						}
					}
					if(i==x)
					{//ԭû����
						for(i=0;i<NUM_RMmax;i++)
						{
							p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							if(p8s[0]==DataType_structure)
							{
								p8d=Get_Element(p8s,2,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
								if(p8d)
								{
									d64b=MRR((u32)p8d+3,6);
									d64b=R_Inverse(d64b,6);
									if(d64b==d64a)
									{
										break;
									}
								}
							}
						}
						if(i==NUM_RMmax)
						{
							myprintf("[%s:%d]:���ַ�ڵ�����Ҳû���ҵ�\n",(u32)&__func__,(u32)__LINE__,0);
							break;
						}
						MC(0,(u32)&NoteList->List[x],LEN_NoteList);
						MWR(d64a,(u32)&NoteList->List[x].Addr,6);
						MWR(CollectorAddr,(u32)&NoteList->List[x].CollectorAddr,6);//�ɼ�����ַ
						//NoteList1->List1[x].user=1;
						x++;
						Terminal_Router->NoteNum=x;//��ǰ·�������ܴӽڵ���
						myprintf("[%s:%d]:д��ӽڵ���Ϣ�ɹ�:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
						//��ӡ��ÿ���ѱ�Ľ��
					}
				}
			}
		}
}
#endif
void SearchMeter_Record(u32 PORTn,u32 FileID,u64 MeterAddr,u32 Protocol,u64 CollectorAddr,u32 Bps,u32 Phase)//�ѱ��¼;���:FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
{
	u32 i;
#if (((USER/100)==9)||((USER/100)==17))//����	
	u32 j;
#endif
	u32 x;
	u32 n;
	u32 Len;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
#if (((USER/100)==9)||((USER/100)==17))//����		
	u8 *p8s;
	u8* p8d;
#endif		
	u32 offset;
	u32 AddrType;//ͷ�ļ��м�¼�ĵ�ַ��:0=����ַ,1=�ɼ�����ַ
	SearchMeterFileHead_TypeDef* SearchMeterFileHead;
	u32 SetNo;
	
	
	MeterAddr=R_Inverse(MeterAddr,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	CollectorAddr=R_Inverse(CollectorAddr,6);//�Ĵ������ݵ���(�ߵ��ֽڵ���),���:Data=�Ĵ���������,Byte=�赹����ֽ���;����:����������
	if(MeterAddr==0xeeeeeeeeeeee)
	{
		AddrType=1;//ͷ�ļ��м�¼�ĵ�ַ��:0=����ַ,1=�ɼ�����ַ
	}
	else
	{
		AddrType=0;//ͷ�ļ��м�¼�ĵ�ַ��:0=����ַ,1=�ɼ�����ַ
	}
	p8=(u8*)ADDR_DATABUFF+1024;
	if( FileID==3)
	{//ȫ���ѱ���
//����2�������ѱ�������=array һ���ѱ���
//һ���ѱ�����=structure
//{
//  ͨ�ŵ�ַ         TSA��
//  �����ɼ�����ַ   TSA��
//	��Լ����  enum
//	{
//  	δ֪          ��0����
//		DL/T645-1997  ��1����
//		DL/T645-2007  ��2����
//		DL/T698.45    ��3����
//		CJ/T 188-2004 ��4��
//	}��
//	��λ             enum{δ֪��0����A��1����B��2����C��3��}��
//		�ź�Ʒ��         unsigned��
//	�ѵ���ʱ��       date_time_s��
//	�ѵ��ĸ�����Ϣ   array������Ϣ
//}
//������Ϣ��=structure
//{
//	������������  OAD��
//	����ֵ        Data
//}
//һ���ѱ�����=structure
//{
		p8[0]=DataType_structure;
		p8[1]=7;
		offset=2;
//  ͨ�ŵ�ַ         TSA��
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(MeterAddr,(u32)p8+offset+3,6);
		offset+=9;
//  �����ɼ�����ַ   TSA��
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(CollectorAddr,(u32)p8+offset+3,6);
		offset+=9;
//	��Լ����  enum
//	{
//  	δ֪          ��0����
//		DL/T645-1997  ��1����
//		DL/T645-2007  ��2����
//		DL/T698.45    ��3����
//		CJ/T 188-2004 ��4��
//	}��
		//376.2��ͨ��Э�����ͣ�00H��͸�����䣻01H��DL/T645-1997��02H��DL/T645-2007��03H��FFH����
		p8[offset]=DataType_enum;
		p8[offset+1]=Protocol;
		offset+=2;
//	��λ             enum{δ֪��0����A��1����B��2����C��3��}��
		p8[offset]=DataType_enum;
		p8[offset+1]=Phase;
		offset+=2;
//		�ź�Ʒ��         unsigned��
		p8[offset]=DataType_unsigned;
		p8[offset+1]=0;
		offset+=2;
//	�ѵ���ʱ��       date_time_s��
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//�õ�date_time_s��7�ֽ�
		offset+=8;
//	�ѵ��ĸ�����Ϣ   array������Ϣ
		p8[offset]=DataType_array;
		p8[offset+1]=0;
		offset+=2;
//}
	}
	else
	{//��̨���ѱ���
//����5����̨���ѱ�������=array һ����̨�����
//һ����̨�������=structure
//{
//	ͨ�ŵ�ַ       TSA��
//	���ڵ��ַ     TSA��
//	���ʱ��       date_time_s
//}
//һ����̨�������=structure
//{
		p8[0]=DataType_structure;
		p8[1]=3;
		offset=2;
//	ͨ�ŵ�ַ       TSA��
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(MeterAddr,(u32)p8+offset+3,6);
		offset+=9;
//	���ڵ��ַ     TSA��
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(CollectorAddr,(u32)p8+offset+3,6);
		offset+=9;
//	���ʱ��       date_time_s
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//�õ�date_time_s��7�ֽ�
		offset+=8;
//}
	}
	
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
	
	
//��ԭ�Ƿ��Ѿ�����
	n=file->NUM_DATA;
	if(n>=NUM_RMmax)
	{
		return;
	}
	ms->msfile_read(filename,0,(u8*)ADDR_AddrList,n*sizeof(SearchMeterFileHead_TypeDef));
	SearchMeterFileHead=(SearchMeterFileHead_TypeDef*)(ADDR_AddrList);
	if(AddrType==0)//ͷ�ļ��м�¼�ĵ�ַ��:0=����ַ,1=�ɼ�����ַ
	{
		Addr0=(p8[6]<<8)|p8[5];
		Addr1=(p8[8]<<8)|p8[7];
		Addr2=(p8[10]<<8)|p8[9];
		for(i=0;i<n;i++)
		{
			if(SearchMeterFileHead->AddrType==0)
			{
				if(SearchMeterFileHead->Addr0==Addr0)
				{
					if(SearchMeterFileHead->Addr1==Addr1)
					{
						if(SearchMeterFileHead->Addr2==Addr2)
						{
							break;
						}
					}
				}
			}
			SearchMeterFileHead++;
		}
	}
	else
	{
		Addr0=(p8[15]<<8)|p8[14];
		Addr1=(p8[17]<<8)|p8[16];
		Addr2=(p8[19]<<8)|p8[18];
		for(i=0;i<n;i++)
		{
			if(SearchMeterFileHead->AddrType==1)
			{
				if(SearchMeterFileHead->Addr0==Addr0)
				{
					if(SearchMeterFileHead->Addr1==Addr1)
					{
						if(SearchMeterFileHead->Addr2==Addr2)
						{
							break;
						}
					}
				}
			}
			SearchMeterFileHead++;
		}
	}
	if(i<n)
	{//��ַ�Ѵ���
		return;
	}
//д�ļ�
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(SearchMeterFileHead_TypeDef));
	}
	Len=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	while(1)
	{
		x=ms->msfile_write(filename,i,p8,Len);
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
	SearchMeterFileHead=(SearchMeterFileHead_TypeDef*)(ADDR_DATABUFF);
	if(AddrType==0)//ͷ�ļ��м�¼�ĵ�ַ��:0=����ַ,1=�ɼ�����ַ
	{
		MWR(MeterAddr,(u32)&SearchMeterFileHead->Addr0,6);
	}
	else
	{
		MWR(CollectorAddr,(u32)&SearchMeterFileHead->Addr0,6);
	}
	SearchMeterFileHead->AddrType=AddrType;
	SearchMeterFileHead->LEN_DATA=Len;
	ms->msfile_write(filename,n*sizeof(SearchMeterFileHead_TypeDef),(u8*)SearchMeterFileHead,sizeof(SearchMeterFileHead_TypeDef));
//������+1
	n++;
	file->NUM_DATA=n;
//�����ϱ�
	
	 SetNo=AddrTowToMeterSetNo((u16*)ADDR_DATABUFF);//����ͨ�ŵ�ַ�ɼ�����ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
	//SetNo=AddrToMeterSetNo((u16*)ADDR_DATABUFF);//����ͨ�ŵ�ַ�ڲɼ��������ñ��е����;����:��0��ʼ���������,>=NUM_RMmax��ʾ��Ч
//�����ѱ�����¼�
	i=MRR(ADDR_6002_8+7,1);//�ѱ����,�Ƿ�����ѱ�����¼�   bool
	if(i==1)
	{//�����ѱ�����¼�
		switch(FileID)
		{
			case 3://ȫ���ѱ����ļ�
			#if (((USER/100)==9)||((USER/100)==17))//����
				if(AddrType == 0)
				{	
					p8s=(u8*)ADDR_6000_SDRAM;
					for(j=0;j<NUM_RMmax;j++)
					{
						if(p8s[0]==DataType_structure)
						{//��Ч
							p8d=p8s+5;
							p8d=Get_Element(p8d,0,0,0);//����Ԫ�ص�ַ
							if(p8d)
							{
								if(Compare_DL698DataString(&p8d[5],p8 + 5, 6) == 0)
								{
									break;
								}
							}
						}
						p8s+=LENper_6000;
					}
					if(j == NUM_RMmax)
					{
						Event_31110200(p8);//����δ֪���ܱ��¼���¼;���:pDATA=1���ѱ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
					}
					else
					{
						break;
					}
				}
				else
				{
					// �ɼ�����ʱδ�����Ȼ���
					Event_31110200(p8);//����δ֪���ܱ��¼���¼;���:pDATA=1���ѱ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			#else
				if(SetNo>=NUM_RMmax)
				{//����δ֪���ܱ�
					Event_31110200(p8);//����δ֪���ܱ��¼���¼;���:pDATA=1���ѱ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				}
			#endif
				//}
				break;
			case 4://��̨���ѱ����ļ�
				Event_31120200(p8);//��̨�����ܱ��¼���¼;���:pDATA=һ����̨�����;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
				break;
		}
	}

#if (USER/100)!=17//����ϵ
//���µ���
	if(SetNo>=NUM_RMmax)
	{//����δ֪���ܱ�
		i=MRR(ADDR_6002_8+5,1);//�ѱ����,�Զ����²ɼ�����       bool
		if(i==1)
		{//�Զ����²ɼ�����
			p8=(u8*)ADDR_6000_SDRAM+LENper_6000;
			for(SetNo=1;SetNo<NUM_RMmax;SetNo++)
			{
				if(p8[0]==0)
				{
					break;//�ҵ���λ��
				}
				p8+=LENper_6000;
			}
			if(SetNo>=NUM_RMmax)
			{
				return;//û�ҵ���λ��
			}
			Terminal_Ram->SET6000=0x55;//�ɼ��������ñ������ñ�־:0=û����,0x55=����
			//����2�����ñ�ֻ������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
			p8[0]=DataType_structure;
			p8[1]=4;
			p8+=2;
			//{
			//	�������  long-unsigned,
			p8[0]=DataType_long_unsigned;
			p8[1]=SetNo>>8;
			p8[2]=SetNo;
			p8+=3;
			//	������Ϣ  Basic_object��
			//Basic_object��=structure
			//{
			p8[0]=DataType_structure;
			p8[1]=10;
			p8+=2;
			//	ͨ�ŵ�ַ  TSA��
			p8[0]=DataType_TSA;
			p8[1]=7;
			p8[2]=5;
			MWR(MeterAddr,(u32)p8+3,6);
			p8+=9;
			//	������    enum
			//	{
			//		300bps��0����  600bps��1����    1200bps��2����
			//		2400bps��3���� 4800bps��4����   7200bps��5����
			//		9600bps��6���� 19200bps��7����  38400bps��8����
			//		57600bps��9����115200bps��10��������Ӧ��255��
			//	}��
			p8[0]=DataType_enum;
			if(PORTn==RS485_4PORT)
			{
				p8[1]=255;//����Ӧ��255��
			}
			else
			{
				p8[1]=Bps;
			}
			p8+=2;
			//	��Լ����  enum
			//	{
			//  	δ֪ (0)��
			//		DL/T645-1997��1����
			//		DL/T645-2007��2����
			//		DL/T698.45��3����
			//		CJ/T 188-2004��4��
			//	}��
			p8[0]=DataType_enum;
			p8[1]=Protocol;
			p8+=2;
			//	�˿�      OAD��
			i=PORTntoOAD(PORTn);//ͨ�ſں�תΪOADֵ
			p8[0]=DataType_OAD;
			p8[1]=i>>24;
			p8[2]=i>>16;
			p8[3]=i>>8;
			p8[4]=i;
			p8+=5;
			//	ͨ������  octet-string��
			p8[0]=DataType_octet_string;
			p8[1]=0;
			p8+=2;
			//	���ʸ���  unsigned��
			p8[0]=DataType_unsigned;
			p8[1]=4;
			p8+=2;
			//	�û�����  unsigned��
			p8[0]=DataType_unsigned;
			p8[1]=5;//5=E���ѹ�����û�
			p8+=2;
			//	���߷�ʽ  enum
			//	{
			//   δ֪��0����
			//   ���ࣨ1����
			//   �������ߣ�2����
			//   �������ߣ�3��
			//	}
			p8[0]=DataType_enum;
			p8[1]=0;//δ֪��0��
			p8+=2;
			//���ѹ    long-unsigned(����-1����λV),
			p8[0]=0;
			p8+=1;
			//�����    long-unsigned(����-1����λA)
			p8[0]=0;
			p8+=1;
			//}
			
			//	��չ��Ϣ  Extended_object��
			//Extended_object��=structure
			//{
			p8[0]=DataType_structure;
			p8[1]=4;
			p8+=2;
			//	�ɼ�����ַ  TSA��
			p8[0]=DataType_TSA;
			p8[1]=7;
			p8[2]=5;
			MWR(CollectorAddr,(u32)p8+3,6);
			p8+=9;
			//	�ʲ���      octet-string��
			p8[0]=DataType_octet_string;
			p8[1]=0;
			p8+=2;
			//	PT          long-unsigned��
			p8[0]=DataType_long_unsigned;
			p8[1]=0;
			p8[2]=0;
			p8+=3;
			//	CT          long-unsigned
			p8[0]=DataType_long_unsigned;
			p8[1]=0;
			p8[2]=0;
			p8+=3;
			//}
			//	������Ϣ  Annex_object
			//Annex_object��=array structure
			p8[0]=DataType_array,
			p8[1]=0,
			p8+=2;
			//{
			//	������������  OAD��
			//	����ֵ        Data
			//}

			//}
			//����FLASH����
			MW(ADDR_6000_SDRAM+(SetNo*LENper_6000),ADDR_6000+(SetNo*LENper_6000),LENper_6000);
		}
	}
	
	
#endif
}

































