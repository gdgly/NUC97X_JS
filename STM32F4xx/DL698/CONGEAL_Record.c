
//冻结记录文件
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



u32 Get_CongealFileID(u32 OI)//得到冻结文件的FileID
{
	OI<<=8;
	OI|=1;//文件分类
	return OI;
}

u32 Get_CongealFileName(u32 OI,u32 FileCount,u8* pTime)//搜索冻结文件名:入口:OI,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
{
	u32 i;
	msFILE_TypeDef *file;
	u32 SearchCount;
	
	OI=Get_CongealFileID(OI);//得到冻结文件的FileID
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
			//文件计数
			if(FileCount!=0xffffffff)//文件计数
			{
				if(FileCount!=file->FILECOUNT)
				{
					file++;
					continue;
				}
			}
			//存储时间
			if(pTime)
			{
				for(i=0;i<6;i++)//秒不比较
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
			Comm_Ram->msFILESearchCount=SearchCount+1;//文件搜索计数
			if(SearchCount>=recordFILENAMEmax_FLASH)
			{
				SearchCount-=recordFILENAMEmax_FLASH;
				SearchCount+=FILENAMEmax_FLASH;
			}
			return SearchCount;
		}
		file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//文件搜索计数
	return 0xffffffff;
}

u32 ADDR_PARAMETER_50xx(u32 OI)//返回冻结参数地址,0=无效
{
	switch(OI)
	{
		case 0x5000://瞬时冻结
			return ADDR_5000;
		case 0x5001://秒冻结
			return ADDR_5001;
		case 0x5002://分钟冻结
			return ADDR_5002;
		case 0x5003://小时冻结
			return ADDR_5003;
		case 0x5004://日冻结
			return ADDR_5004;
		case 0x5005://结算日冻结
			return ADDR_5005;
		case 0x5006://月冻结
			return ADDR_5006;
		case 0x5007://年冻结
			return ADDR_5007;
		case 0x5008://时区表切换冻结
			return ADDR_5008;
		case 0x5009://日时段表切换冻结
			return ADDR_5009;
		case 0x500A://费率电价切换冻结
			return ADDR_500A;
		case 0x500B://阶梯切换冻结
			return ADDR_500B;
		case 0x5011://阶梯结算冻结
			return ADDR_5011;
		default:
			return 0;
	}
}

u32 DEPTH_CongealFile(u32 OI)//冻结文件存储深度;返回:存储深度(各OAD中最大的值)
{
	u32 i;
	u32 addr;
	u32 array;
	u32 depth;
	u8* p8;
	
	addr=ADDR_PARAMETER_50xx(OI);//返回冻结参数地址,0=无效
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
		array=p8[1];//原有数组数
		if(array>127)
		{
			array=0;
		}
	}
	else
	{
		array=0;
	}
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
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
		depth=1;//最小值
	}
	switch(OI)
	{
		case 0x5000://瞬时冻结
			i=1;//默认最大深度
			break;
		case 0x5001://秒冻结
			i=1;//默认最大深度
			break;
		case 0x5002://分钟冻结
			i=96*2;//默认最大深度
			break;
		case 0x5003://小时冻结
			i=24;//默认最大深度
			break;
		case 0x5004://日冻结
			i=62;//默认最大深度
			break;
		case 0x5005://结算日冻结
			i=1;//默认最大深度
			break;
		case 0x5006://月冻结
			i=12;//默认最大深度
			break;
		case 0x5007://年冻结
			i=1;//默认最大深度
			break;
		case 0x5008://时区表切换冻结
			i=1;//默认最大深度
			break;
		case 0x5009://日时段表切换冻结
			i=1;//默认最大深度
			break;
		case 0x500A://费率电价切换冻结
			i=1;//默认最大深度
			break;
		case 0x500B://阶梯切换冻结
			i=1;//默认最大深度
			break;
		case 0x5011://阶梯结算冻结
			i=1;//默认最大深度
			break;
	}
	if(depth>i)
	{
		depth=i;
	}
	return depth;
}

void CountInc_CongealFile(u32 OI)//当有文件计数=0时,全部冻结文件文件计数+1,删除达到记录深度的文件
{
	u32 i;
	u32 FileID;
	u32 set_depth;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
	FileID=Get_CongealFileID(OI);//得到冻结文件的FileID
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
	{//有计数=0的文件
		ms=Get_ms();
		set_depth=DEPTH_CongealFile(OI);//冻结文件存储深度;返回:存储深度(各OAD中最大的值)
		if(set_depth==0)
		{
			set_depth=1;//最小值
		}
		if(set_depth>0xfffe)
		{
			set_depth=0xfffe;//最大值
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

void CongealFile_Time(u32 OI,u8* pOUT)//冻结文件时标(无数据类型)
{
	u32 i;
	
	for(i=0;i<7;i++)
	{
		pOUT[i]=Comm_Ram->DL698YMDHMS[i];
	}
	switch(OI)
	{
		case 0x5000://瞬时冻结
			break;
		case 0x5001://秒冻结
			break;
		case 0x5002://分钟冻结
			//对2000年1月1日0时0分0秒每15分的标准时间
			MR((u32)pOUT,ADDR_TerminalCongeal_Time,7);
			break;
		case 0x5003://小时冻结
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5004://日冻结
			pOUT[4]=0;
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5005://结算日冻结
			break;
		case 0x5006://月冻结
			pOUT[3]=1;
			pOUT[4]=0;
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5007://年冻结
			pOUT[2]=1;
			pOUT[3]=1;
			pOUT[4]=0;
			pOUT[5]=0;
			pOUT[6]=0;
			break;
		case 0x5008://时区表切换冻结
			pOUT[6]=0;
			break;
		case 0x5009://日时段表切换冻结
			pOUT[6]=0;
			break;
		case 0x500A://费率电价切换冻结
			pOUT[6]=0;
			break;
		case 0x500B://阶梯切换冻结
			pOUT[6]=0;
			break;
		case 0x5011://阶梯结算冻结
			pOUT[6]=0;
			break;
	}
}


u32 Get_CongealFileData_RCSD(u8 *pRCSD,u8 *pRecord,u8* pTime,u32 NUM_DATA,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//冻结表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pRecord指向数据块,pTime指向冻结时间(无数据类型),NUM_DATA冻结数据列表数,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
{
	u32 i;
	u32 x;
	u32 n;
	u32 OADn;
	u32 OADm;
	u32 Len;
	u32 Offset;
	u8* p8;
	CongealFileHead_TypeDef* CongealFileHead;//冻结记录文件头文件
	u32 fDATA;//有列选择OAD数据标志
	
	
	fDATA=0;//有列选择OAD数据标志
	n=pRCSD[0];//RCSD，SEQUENCE OF个数
	pRCSD++;
	if(NUM_DATA>NUMmax_50xx)//总OAD记录列表个数
	{
		NUM_DATA=NUMmax_50xx;
	}
	Len=0;
	while(n--)
	{
//缓冲数据约定
//2byte 总数据个数
//7byte date_time_s 数据冻结时间
//1byte 
//NUMmax_50xx*sizeof(CongealFileHead_TypeDef) byte
//数据
		
		OADn=(pRCSD[1]<<24)+(pRCSD[2]<<16)+(pRCSD[3]<<8)+pRCSD[4];
		switch(OADn)
		{
			case 0x40010200://通信地址
			case 0x202A0200://目标服务器地址
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
					return 0;//CSD错误
				}
				continue;
			case 0x20210200://数据冻结时间
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
					return 0;//CSD错误
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
				{//冻结时为全部元素
					if(OADn&0xff)
					{//提取1个元素
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
			{//没找到
				if(LEN_DATAmax<1)
				{
					return 0;
				}
				pout[Len]=0;//NULL
				Len+=1;
				LEN_DATAmax-=1;
			}
			else
			{//找到
				x=CongealFileHead->LEN_data[0]+(CongealFileHead->LEN_data[1]<<8);
				p8=pRecord+(NUMmax_50xx*sizeof(CongealFileHead_TypeDef))+Offset;
				if((OADm&0xff)==0)
				{//冻结时为全部元素
					if(OADn&0xff)
					{//提取1个元素
						p8=Get_Element(p8,OADn&0xff,0,0);//计算元素地址,使用ADDR_128KDATABUFF
						if(p8)
						{//找到元素
							x=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
						else
						{//没找到元素
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
				fDATA=1;//有列选择OAD数据标志
			}
			pRCSD+=5;
		}
		else
		{//ROAD
			x=pRCSD[5];//SEQUENCE OF个数
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
	if(fDATA)//有列选择OAD数据标志
	{
		return Len;
	}
	if(DATANULLSEL)//全部数据空时：0=无返回,1=用空数据0
	{
		return Len;
	}
	return 0;
}

u32 Get_CongealFileData_Fixed(u32 OI,u32 FileCount,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到冻结文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
{
	u32 i;
	u32 n;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
	
	ms=(ms_Type *)Get_ms();
	filename=Get_CongealFileName(OI,FileCount,0);//搜索冻结文件名:入口:OI,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
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
//不选择       [0]  NULL
			break;
		case 1:
//Selector1∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  数值            Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://数据冻结时间
					pRSD+=4;
					if(pRSD[0]!=DataType_date_time_s)
					{//数值Data=空
						return 0;
					}
					pRSD+=1;
					//数值Data=date_time_s
					for(i=0;i<6;i++)
					{//秒不比较
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
//Selector2∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  起始值          Data，
//  结束值          Data，
//  数据间隔        Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://数据冻结时间
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=0)
					{//时间!=起始值
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=2)
					{//时间>=结束值
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 3:
//Selector3∷=SEQUENCE OF Selector2
			pRSD+=2;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://数据冻结时间
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=0)
					{//时间!=起始值
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=2)
					{//时间>=结束值
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 4:
//Selector4∷=SEQUENCE
//{
//  采集启动时间  date_time_s，
//  电能表集合    MS
//}
			return 0;
		case 5:
//Selector5∷=SEQUENCE
//{
//  采集存储时间  date_time_s，
//  电能表集合    MS
//}
			return 0;
		case 6:
//Selector6∷=SEQUENCE
//{
//  采集启动时间起始值  date_time_s，
//  采集启动时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			return 0;
		case 7:
//Selector7∷=SEQUENCE
//{
//  采集存储时间起始值  date_time_s，
//  采集存储时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			return 0;
		case 8:
//Selector8∷=SEQUENCE
//{
//  采集成功时间起始值  date_time_s，
//  采集成功时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			return 0;
		case 9:
//Selector9∷=SEQUENCE
//{
//  上第n次记录  unsigned
//}
			break;//查找时用文件计数，找到就有效
		case 10:
//Selector10∷=SEQUENCE
//{
//上n条记录  unsigned，
//电能表集合  MS
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
	i=Get_CongealFileData_RCSD(pRCSD,(u8*)ADDR_128KDATABUFF,file->save_date_time_s,n,pOUT,LEN_OUTmax,ALLDATANULLSEL);//冻结表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pRecord指向OAD列表和数据块开始(开始2字节为总列表数),p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
	return i;
}

u32 Get_CongealFileData(u32 OI,u32 FileCount,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到冻结文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
{
	u32 LEN;
	Comm_Ram->msFILESearchCount=0;
	while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
	{
		LEN=Get_CongealFileData_Fixed(OI,FileCount,pRSD,pRCSD,pOUT,LEN_OUTmax);//得到冻结文件数据
		if(LEN)
		{
			return LEN;
		}
	}
	return 0;
}



void CongealFile(u32 OI)//冻结文件
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
	
	Addr=ADDR_PARAMETER_50xx(OI);//返回冻结参数地址,0=无效
	if(Addr==0)
	{
		return;
	}
	Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
	ms=Get_ms();
	FileID=Get_CongealFileID(OI);//得到冻结文件的FileID
	Comm_Ram->msFILESearchCount=0;
	CongealFile_Time(OI,(u8*)ADDR_DATABUFF);//冻结文件时标(无数据类型)
	filename=Get_CongealFileName(OI,0xffffffff,(u8*)ADDR_DATABUFF);//搜索冻结文件名:入口:OI,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
	if(filename!=0xffffffff)
	{//已存在
		//删除文件
		if(filename>=recordFILENAMEmax_FLASH)
		{
			filename-=recordFILENAMEmax_FLASH;
			filename+=FILENAMEmax_FLASH;
		}
		ms->msfile_Delete(filename);
		filename=0xffffffff;//没找到
	}
	if(filename==0xffffffff)
	{//没找到
		CountInc_CongealFile(OI);//当有文件计数=0时,全部记录文件文件计数+1,删除达到记录深度的文件
		//找FLASH空文件
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
		CongealFile_Time(OI,file->save_date_time_s);//冻结文件时标(无数据类型)
	}
//组文件数据
	NUM_DATA=0;
	LEN_DATA=NUMmax_50xx*sizeof(CongealFileHead_TypeDef);
	//写数据
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
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
//写文件
	while(1)
	{
		x=ms->msfile_write(filename,0,(u8*)pDATA,LEN_DATA);
		if(x!=LEN_DATA)
		{//写错误
			x=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(x)
			{//有删除
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
//总
	file->LEN_DATA=LEN_DATA;
	file->NUM_DATA=NUM_DATA;
	
//主动上报
	i=MRR(ADDR_4300_8+1,1);
	if(i==0)
	{//不允许主动上报
		for(i=0;i<NUMmax_601C;i++)
		{
		#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
			file->NUM_GPRSreport[i]=NUM_DATA;
			file->NUM_ENETreport[i]=NUM_DATA;
		#else
			file->NUM_GPRSorENETreport[i]=NUM_DATA;
		#endif
		}
	}
	else
	{//允许主动上报
	//上报通道
		REPORT_PORTn(file);//通道不需上报时置已上报数据个数=当前数据个数；使用ADDR_DATABUFF
	}
}

extern const u8 InitData_2131[];
extern const u8 InitData_2140[];
extern const u8 Init_EnergyData_double_long_unsigned[];
void CONGEAL_Record(void)//冻结
{
	u32 i;
	u64 YYMDHMSs;
	u64 YYMDHMSc;
	u64 d64;
	u32 CongealFlag;//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
	u8* p8;
	
	p8=(u8*)ADDR_TerminalCongeal_Time;
	if(p8[7]==Comm_Ram->DL698YMDHMS[6])
	{
		return;
	}
	p8[7]=Comm_Ram->DL698YMDHMS[6];
	CongealFlag=0;//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
	YYMDHMSs=MRR(ADDR_TerminalCongeal_Time,7);
	YYMDHMSc=MRR(ADDR_DL698YMDHMS,7);
	
	if((YYMDHMSs&0xffffffff)!=(YYMDHMSc&0xffffffff))
	{//日变化
		CongealFlag|=1;//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
	}
	if((YYMDHMSs&0xffffff)!=(YYMDHMSc&0xffffff))
	{//月变化
		CongealFlag|=2;//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
	}

	//分变化(曲线冻结)
	//当前时间对2000年1月1日0时0分0秒
	p8=(u8*)ADDR_DATABUFF;
	p8[0]=2000>>8;//年
	p8[1]=2000&0xff;
	p8[2]=1;//月
	p8[3]=1;//日
	p8[4]=0;//时
	p8[5]=0;//分
	p8[6]=0;//秒
	i=YYMDHMS_Sub_hex(p8,(u8*)ADDR_DL698YMDHMS);//hex时钟差值计算;入口:pRTC1(年年月日时分秒);pRTC2(年年月日时分秒);出口:pRTC2-pRTC1=HEX补码时钟快或慢的差值秒,pRTC1或pRTC2非法=0,pRTC2>=pRTC1差值为正,pRTC2<pRTC1差值为负
	if(i)
	{
		if(i>>31)
		{//负
			i=~i;
			i++;
		}
		i/=60;//化为分
		i/=CongealTime_5002;//分冻结(曲线)间隔时间分
		i*=CongealTime_5002;
		i*=60;
		pYYMDHMS_AddS_hex(p8,i);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
		d64=MRR(ADDR_DATABUFF,7);
		if((YYMDHMSs&0xffffffffffff)!=(d64&0xffffffffffff))
		{//分变化(曲线冻结)
			YYMDHMSc=d64;
			CongealFlag|=4;//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
		}
	}

	
	if(CongealFlag==0)
	{
		return;
	}
	MWR(YYMDHMSc,ADDR_TerminalCongeal_Time,7);
	
//分冻结(曲线冻结)
	if(CongealFlag&4)//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
	{//分冻结
		CongealFile(0x5002);//冻结文件
		
//分冻结后清0
	}
	
	
//日冻结
	if(CongealFlag&1)//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
	{//日冻结
		CongealFile(0x5004);//冻结文件
		
		
//日冻结后清0
		MC(0,ADDR_BroadcasTimeFlag,1);//智能表日广播校时标志
//电压合格率
		MR(ADDR_2131,(u32)InitData_2131,2+23);//A相
		MR(ADDR_2132,(u32)InitData_2131,2+23);//B相
		MR(ADDR_2133,(u32)InitData_2131,2+23);//C相

//最大有功功率及发生时间
		MR(ADDR_2140,(u32)InitData_2140,12);
//通信流量
		MC(0,ADDR_2200+3,4);
//供电时间
		MC(0,ADDR_2203+3,4);
//复位累计次数
		MC(0,ADDR_2204+3,2);
//脉冲计量当日电能量
	#if NUMmax_IMPIN!=0
		for(i=0;i<NUMmax_IMPIN;i++)
		{
			MW((u32)Init_EnergyData_double_long_unsigned,ADDR_impEnergy_Day+(i*LENper_impEnergy),LENper_impEnergy);
		}
	#endif
//总加组
		Init_Data230x_D();//初始化总加组日数据
		
	}
	
//月冻结
	if(CongealFlag&2)//冻结标志:B0=日冻结,B1=月冻结,B2=分冻结，B3...
	{//月冻结
		CongealFile(0x5006);//冻结文件
		
		
//月冻结后清0
//电压合格率
		MR(ADDR_2131+2+23,(u32)InitData_2131+2+23,23);//A相
		MR(ADDR_2132+2+23,(u32)InitData_2131+2+23,23);//B相
		MR(ADDR_2133+2+23,(u32)InitData_2131+2+23,23);//C相
//最大有功功率及发生时间
		MR(ADDR_2141,(u32)InitData_2140,12);
//通信流量
		MC(0,ADDR_2200+8,4);
		MC(0,ADDR_2200_EventRecordFags,1);//通信流量超限事件已记录标志,月更新时清0
//供电时间
		MC(0,ADDR_2203+8,4);
//复位累计次数
		MC(0,ADDR_2204+6,2);
//脉冲计量当月电能量
	#if NUMmax_IMPIN!=0
		for(i=0;i<NUMmax_IMPIN;i++)
		{
			MW((u32)Init_EnergyData_double_long_unsigned,ADDR_impEnergy_Month+(i*LENper_impEnergy),LENper_impEnergy);
		}
	#endif
//总加组
		Init_Data230x_M();//初始化总加组月数据
		
	}
}














