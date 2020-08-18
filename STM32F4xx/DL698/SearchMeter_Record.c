
//自动搜表记录文件
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


void DEL_SearchMeter_Record(u32 FileID)//清空搜表结果;入口：FileID=3全部搜表结果，=4跨台区搜表结果
{
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			//b7-b0文件分类
			if(file->FILEID!=FileID)
			{
				file++;
				continue;
			}
			//找到
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
//	x=Terminal_Router->NoteNum1;//已知总从节点数
//	if((x+1)<=NUM_RMmax)
//	{
//		d64a=MeterAddr;
//		for(i=0;i<x;i++)
//		{
//			d64b=MRR((u32)&NoteList1->List1[i].Addr,6);
//			if(d64a==d64b)
//			{
//				MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//采集器
//				break;//原已存在
//			}
//		}
//		if(i==x)
//		{//原没存在
//				for(i=0;i<x;i++)
//				{
//					d64b=MRR((u32)&NoteList1->List1[i].CollectorAddr,6);
//					if(d64a==d64b)
//					{
//						MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//采集器
//						break;//原已存在
//					}
//				}
//				if(i==x)
//				{//原没存在
//					for(i=0;i<NUM_RMmax;i++)
//					{
//						p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
//						if(p8s[0]==DataType_structure)
//						{
//							p8d=Get_Element(p8s,2,1,0);//计算元素地址,使用ADDR_128KDATABUFF
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
//					MWR(CollectorAddr,(u32)&NoteList1->List1[x].CollectorAddr,6);//采集器地址
//					x++;
//					Terminal_Router->NoteNum1=x;//当前路由器中总从节点数
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
	
	x=Terminal_Router->NoteNum1;//已知总从节点数
	if((x+1)<=NUM_RMmax)
	{
		d64a=CollectorAddr;
		for(i=0;i<x;i++)
		{
			d64b=MRR((u32)&NoteList1->List1[i].Addr,6);
			if(d64a==d64b)
			{
				MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//采集器
				break;//原已存在
			}
		}
		if(i==x)
		{//原没存在
				for(i=0;i<x;i++)
				{
					d64b=MRR((u32)&NoteList1->List1[i].CollectorAddr,6);
					if(d64a==d64b)
					{
						MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//采集器
						break;//原已存在
					}
				}
				if(i==x)
				{//原没存在
					for(i=0;i<NUM_RMmax;i++)
					{
						p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
						if(p8s[0]==DataType_structure)
						{
							p8d=Get_Element(p8s,2,1,0);//计算元素地址,使用ADDR_128KDATABUFF
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
					MWR(CollectorAddr,(u32)&NoteList1->List1[x].CollectorAddr,6);//采集器地址
					NoteList1->List1[x].user=1;
					x++;
					Terminal_Router->NoteNum1=x;//当前路由器中总从节点数
					myprintf("[%s:%d]:写入模块表从节点信息成功:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum1);
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
	
	x=Terminal_Router->NoteNum1;//已知总从节点数
	for(y=0;y<NoteNum;y++)
	{
		if((x+1)<=NUM_RMmax)
		{
			d64a=MRR(((u32)p8rx)+2+(7*y),6);
			for(i=0;i<x;i++)
			{
				//上面判断的是电表的地址
				d64b=MRR((u32)&NoteList1->List1[i].Addr,6);
				if(d64a==d64b)
				{
					MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//采集器
					break;//原已存在
				}
			}
			if(i==x)
			{//原没存在
					for(i=0;i<x;i++)
					{
						d64b=MRR((u32)&NoteList1->List1[i].CollectorAddr,6);
						if(d64a==d64b)
						{
							MWR(CollectorAddr,(u32)&NoteList1->List1[i].CollectorAddr,6);//采集器
							break;//原已存在
						}
					}
					if(i==x)
					{//原没存在
						for(i=0;i<NUM_RMmax;i++)
						{
							p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							if(p8s[0]==DataType_structure)
							{
								p8d=Get_Element(p8s,2,1,0);//计算元素地址,使用ADDR_128KDATABUFF
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
							myprintf("[%s:%d]:表地址在档案中也没有找到\n",(u32)&__func__,(u32)__LINE__,0);
							break;
						}
						MC(0,(u32)&NoteList1->List1[x],LEN_NoteList1);
						MWR(d64a,(u32)&NoteList1->List1[x].Addr,6);
						MWR(CollectorAddr,(u32)&NoteList1->List1[x].CollectorAddr,6);//采集器地址
						NoteList1->List1[x].user=1;
						x++;
						Terminal_Router->NoteNum1=x;//当前路由器中总从节点数
						myprintf("[%s:%d]:写入从节点信息成功:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum1);
						//打印出每次搜表的结果
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
	
	x=Terminal_Router->NoteNum;//已知总从节点数
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:写入从节点信息成功:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
	#endif
	if((x+1)<=NUM_RMmax)
	{
		d64a=CollectorAddr;
		for(i=0;i<x;i++)
		{
			d64b=MRR((u32)&NoteList->List[i].Addr,6);
			if(d64a==d64b)
			{
				MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//采集器
				break;//原已存在
			}
		}
		if(i==x)
		{//原没存在
				for(i=0;i<x;i++)
				{
					d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
					if(d64a==d64b)
					{
						MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//采集器
						break;//原已存在
					}
				}
				if(i==x)
				{//原没存在
					for(i=0;i<NUM_RMmax;i++)
					{
						p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
						if(p8s[0]==DataType_structure)
						{
							p8d=Get_Element(p8s,2,1,0);//计算元素地址,使用ADDR_128KDATABUFF
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
					MWR(CollectorAddr,(u32)&NoteList->List[x].CollectorAddr,6);//采集器地址
					//NoteList->List[i][x].user=1;
					x++;
					Terminal_Router->NoteNum=x;//当前路由器中总从节点数
					#ifdef DEBUG_JSON
					myprintf("[%s:%d]:写入从节点信息成功:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
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
	
	x=Terminal_Router->NoteNum;//已知总从节点数
	myprintf("[%s:%d]:写入从节点信息成功:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
	for(y=0;y<NoteNum;y++)
	{
		if((x+1)<=NUM_RMmax)
		{
			d64a=MRR(((u32)p8rx)+2+(7*y),6);
			for(i=0;i<x;i++)
			{
				//上面判断的是电表的地址
				d64b=MRR((u32)&NoteList->List[i].Addr,6);
				if(d64a==d64b)
				{
					MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//采集器
					break;//原已存在
				}
			}
			if(i==x)
			{//原没存在
					for(i=0;i<x;i++)
					{
						d64b=MRR((u32)&NoteList->List[i].CollectorAddr,6);
						if(d64a==d64b)
						{
							MWR(CollectorAddr,(u32)&NoteList->List[i].CollectorAddr,6);//采集器
							break;//原已存在
						}
					}
					if(i==x)
					{//原没存在
						for(i=0;i<NUM_RMmax;i++)
						{
							p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							if(p8s[0]==DataType_structure)
							{
								p8d=Get_Element(p8s,2,1,0);//计算元素地址,使用ADDR_128KDATABUFF
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
							myprintf("[%s:%d]:表地址在档案中也没有找到\n",(u32)&__func__,(u32)__LINE__,0);
							break;
						}
						MC(0,(u32)&NoteList->List[x],LEN_NoteList);
						MWR(d64a,(u32)&NoteList->List[x].Addr,6);
						MWR(CollectorAddr,(u32)&NoteList->List[x].CollectorAddr,6);//采集器地址
						//NoteList1->List1[x].user=1;
						x++;
						Terminal_Router->NoteNum=x;//当前路由器中总从节点数
						myprintf("[%s:%d]:写入从节点信息成功:Terminal_Router->NoteNum:%d\n",(u32)&__func__,(u32)__LINE__,Terminal_Router->NoteNum);
						//打印出每次搜表的结果
					}
				}
			}
		}
}
#endif
void SearchMeter_Record(u32 PORTn,u32 FileID,u64 MeterAddr,u32 Protocol,u64 CollectorAddr,u32 Bps,u32 Phase)//搜表记录;入口:FileID=3全部搜表结果文件，=4跨台区搜表结果文件
{
	u32 i;
#if (((USER/100)==9)||((USER/100)==17))//河南	
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
#if (((USER/100)==9)||((USER/100)==17))//河南		
	u8 *p8s;
	u8* p8d;
#endif		
	u32 offset;
	u32 AddrType;//头文件中记录的地址类:0=电表地址,1=采集器地址
	SearchMeterFileHead_TypeDef* SearchMeterFileHead;
	u32 SetNo;
	
	
	MeterAddr=R_Inverse(MeterAddr,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	CollectorAddr=R_Inverse(CollectorAddr,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(MeterAddr==0xeeeeeeeeeeee)
	{
		AddrType=1;//头文件中记录的地址类:0=电表地址,1=采集器地址
	}
	else
	{
		AddrType=0;//头文件中记录的地址类:0=电表地址,1=采集器地址
	}
	p8=(u8*)ADDR_DATABUFF+1024;
	if( FileID==3)
	{//全部搜表结果
//属性2（所有搜表结果）∷=array 一个搜表结果
//一个搜表结果∷=structure
//{
//  通信地址         TSA，
//  所属采集器地址   TSA，
//	规约类型  enum
//	{
//  	未知          （0），
//		DL/T645-1997  （1），
//		DL/T645-2007  （2），
//		DL/T698.45    （3），
//		CJ/T 188-2004 （4）
//	}，
//	相位             enum{未知（0），A（1），B（2），C（3）}，
//		信号品质         unsigned，
//	搜到的时间       date_time_s，
//	搜到的附加信息   array附加信息
//}
//附加信息∷=structure
//{
//	对象属性描述  OAD，
//	属性值        Data
//}
//一个搜表结果∷=structure
//{
		p8[0]=DataType_structure;
		p8[1]=7;
		offset=2;
//  通信地址         TSA，
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(MeterAddr,(u32)p8+offset+3,6);
		offset+=9;
//  所属采集器地址   TSA，
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(CollectorAddr,(u32)p8+offset+3,6);
		offset+=9;
//	规约类型  enum
//	{
//  	未知          （0），
//		DL/T645-1997  （1），
//		DL/T645-2007  （2），
//		DL/T698.45    （3），
//		CJ/T 188-2004 （4）
//	}，
		//376.2中通信协议类型：00H＝透明传输；01H＝DL/T645-1997；02H＝DL/T645-2007；03H～FFH保留
		p8[offset]=DataType_enum;
		p8[offset+1]=Protocol;
		offset+=2;
//	相位             enum{未知（0），A（1），B（2），C（3）}，
		p8[offset]=DataType_enum;
		p8[offset+1]=Phase;
		offset+=2;
//		信号品质         unsigned，
		p8[offset]=DataType_unsigned;
		p8[offset+1]=0;
		offset+=2;
//	搜到的时间       date_time_s，
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//得到date_time_s共7字节
		offset+=8;
//	搜到的附加信息   array附加信息
		p8[offset]=DataType_array;
		p8[offset+1]=0;
		offset+=2;
//}
	}
	else
	{//跨台区搜表结果
//属性5（跨台区搜表结果）∷=array 一个跨台区结果
//一个跨台区结果∷=structure
//{
//	通信地址       TSA，
//	主节点地址     TSA，
//	变更时间       date_time_s
//}
//一个跨台区结果∷=structure
//{
		p8[0]=DataType_structure;
		p8[1]=3;
		offset=2;
//	通信地址       TSA，
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(MeterAddr,(u32)p8+offset+3,6);
		offset+=9;
//	主节点地址     TSA，
		p8[offset]=DataType_TSA;
		p8[offset+1]=7;
		p8[offset+2]=5;
		MWR(CollectorAddr,(u32)p8+offset+3,6);
		offset+=9;
//	变更时间       date_time_s
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//得到date_time_s共7字节
		offset+=8;
//}
	}
	
	Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
	ms=Get_ms();
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			//b7-b0文件分类
			if(file->FILEID!=FileID)
			{
				file++;
				continue;
			}
			//找到
			break;
		}
		file++;
	}
	if(filename>=recordFILENAMEmax_FLASH)
	{//没找到
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
			{//没空
				i=AutoDeleteFile(0);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
				if(i)
				{//有删除
					continue;
				}
				return;
			}
			break;
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//空文件
		ms->msfile_lenclear(filename);//清0文件相关的长度次数,0-(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)表示文件在RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;
	}
	
	
//查原是否已经存在
	n=file->NUM_DATA;
	if(n>=NUM_RMmax)
	{
		return;
	}
	ms->msfile_read(filename,0,(u8*)ADDR_AddrList,n*sizeof(SearchMeterFileHead_TypeDef));
	SearchMeterFileHead=(SearchMeterFileHead_TypeDef*)(ADDR_AddrList);
	if(AddrType==0)//头文件中记录的地址类:0=电表地址,1=采集器地址
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
	{//地址已存在
		return;
	}
//写文件
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(SearchMeterFileHead_TypeDef));
	}
	Len=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
	while(1)
	{
		x=ms->msfile_write(filename,i,p8,Len);
		if(x!=Len)
		{//写错误
			x=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(x)
			{//有删除
				continue;
			}
			return;
		}
		break;
	}
	i+=Len;
	file->LEN_DATA=i;
//写地址列表
	SearchMeterFileHead=(SearchMeterFileHead_TypeDef*)(ADDR_DATABUFF);
	if(AddrType==0)//头文件中记录的地址类:0=电表地址,1=采集器地址
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
//数据数+1
	n++;
	file->NUM_DATA=n;
//主动上报
	
	 SetNo=AddrTowToMeterSetNo((u16*)ADDR_DATABUFF);//搜索通信地址采集器地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
	//SetNo=AddrToMeterSetNo((u16*)ADDR_DATABUFF);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
//产生搜表相关事件
	i=MRR(ADDR_6002_8+7,1);//搜表参数,是否产生搜表相关事件   bool
	if(i==1)
	{//产生搜表相关事件
		switch(FileID)
		{
			case 3://全部搜表结果文件
			#if (((USER/100)==9)||((USER/100)==17))//河南
				if(AddrType == 0)
				{	
					p8s=(u8*)ADDR_6000_SDRAM;
					for(j=0;j<NUM_RMmax;j++)
					{
						if(p8s[0]==DataType_structure)
						{//有效
							p8d=p8s+5;
							p8d=Get_Element(p8d,0,0,0);//计算元素地址
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
						Event_31110200(p8);//发现未知电能表事件记录;入口:pDATA=1个搜表结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
					}
					else
					{
						break;
					}
				}
				else
				{
					// 采集器暂时未处理，先缓缓
					Event_31110200(p8);//发现未知电能表事件记录;入口:pDATA=1个搜表结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			#else
				if(SetNo>=NUM_RMmax)
				{//发现未知电能表
					Event_31110200(p8);//发现未知电能表事件记录;入口:pDATA=1个搜表结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				}
			#endif
				//}
				break;
			case 4://跨台区搜表结果文件
				Event_31120200(p8);//跨台区电能表事件记录;入口:pDATA=一个跨台区结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
				break;
		}
	}

#if (USER/100)!=17//江苏系
//更新档案
	if(SetNo>=NUM_RMmax)
	{//发现未知电能表
		i=MRR(ADDR_6002_8+5,1);//搜表参数,自动更新采集档案       bool
		if(i==1)
		{//自动更新采集档案
			p8=(u8*)ADDR_6000_SDRAM+LENper_6000;
			for(SetNo=1;SetNo<NUM_RMmax;SetNo++)
			{
				if(p8[0]==0)
				{
					break;//找到空位置
				}
				p8+=LENper_6000;
			}
			if(SetNo>=NUM_RMmax)
			{
				return;//没找到空位置
			}
			Terminal_Ram->SET6000=0x55;//采集档案配置表有设置标志:0=没设置,0x55=设置
			//属性2（配置表，只读）∷=structure//删除时structure的数据单元清为0(NULL)
			p8[0]=DataType_structure;
			p8[1]=4;
			p8+=2;
			//{
			//	配置序号  long-unsigned,
			p8[0]=DataType_long_unsigned;
			p8[1]=SetNo>>8;
			p8[2]=SetNo;
			p8+=3;
			//	基本信息  Basic_object，
			//Basic_object∷=structure
			//{
			p8[0]=DataType_structure;
			p8[1]=10;
			p8+=2;
			//	通信地址  TSA，
			p8[0]=DataType_TSA;
			p8[1]=7;
			p8[2]=5;
			MWR(MeterAddr,(u32)p8+3,6);
			p8+=9;
			//	波特率    enum
			//	{
			//		300bps（0），  600bps（1），    1200bps（2），
			//		2400bps（3）， 4800bps（4），   7200bps（5），
			//		9600bps（6）， 19200bps（7），  38400bps（8），
			//		57600bps（9），115200bps（10），自适应（255）
			//	}，
			p8[0]=DataType_enum;
			if(PORTn==RS485_4PORT)
			{
				p8[1]=255;//自适应（255）
			}
			else
			{
				p8[1]=Bps;
			}
			p8+=2;
			//	规约类型  enum
			//	{
			//  	未知 (0)，
			//		DL/T645-1997（1），
			//		DL/T645-2007（2），
			//		DL/T698.45（3），
			//		CJ/T 188-2004（4）
			//	}，
			p8[0]=DataType_enum;
			p8[1]=Protocol;
			p8+=2;
			//	端口      OAD，
			i=PORTntoOAD(PORTn);//通信口号转为OAD值
			p8[0]=DataType_OAD;
			p8[1]=i>>24;
			p8[2]=i>>16;
			p8[3]=i>>8;
			p8[4]=i;
			p8+=5;
			//	通信密码  octet-string，
			p8[0]=DataType_octet_string;
			p8[1]=0;
			p8+=2;
			//	费率个数  unsigned，
			p8[0]=DataType_unsigned;
			p8[1]=4;
			p8+=2;
			//	用户类型  unsigned，
			p8[0]=DataType_unsigned;
			p8[1]=5;//5=E类低压居民用户
			p8+=2;
			//	接线方式  enum
			//	{
			//   未知（0），
			//   单相（1），
			//   三相三线（2），
			//   三相四线（3）
			//	}
			p8[0]=DataType_enum;
			p8[1]=0;//未知（0）
			p8+=2;
			//额定电压    long-unsigned(换算-1，单位V),
			p8[0]=0;
			p8+=1;
			//额定电流    long-unsigned(换算-1，单位A)
			p8[0]=0;
			p8+=1;
			//}
			
			//	扩展信息  Extended_object，
			//Extended_object∷=structure
			//{
			p8[0]=DataType_structure;
			p8[1]=4;
			p8+=2;
			//	采集器地址  TSA，
			p8[0]=DataType_TSA;
			p8[1]=7;
			p8[2]=5;
			MWR(CollectorAddr,(u32)p8+3,6);
			p8+=9;
			//	资产号      octet-string，
			p8[0]=DataType_octet_string;
			p8[1]=0;
			p8+=2;
			//	PT          long-unsigned，
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
			//	附属信息  Annex_object
			//Annex_object∷=array structure
			p8[0]=DataType_array,
			p8[1]=0,
			p8+=2;
			//{
			//	对象属性描述  OAD，
			//	属性值        Data
			//}

			//}
			//更新FLASH档案
			MW(ADDR_6000_SDRAM+(SetNo*LENper_6000),ADDR_6000+(SetNo*LENper_6000),LENper_6000);
		}
	}
	
	
#endif
}

































