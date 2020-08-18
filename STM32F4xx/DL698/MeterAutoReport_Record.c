

//电能表主动上报记录文件
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


void MeterAutoReport_Record(u32 PORTn,u8* pDATA,u32 FileID)//电能表主动上报记录文件;入口:pDATA=数据指针(分别为RecordFileHead+时标+帧个数和长度+数据)
{
	u32 i;
	u32 x;
	u32 n;
	u32 Len;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	
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
			n=file->NUM_DATA;
		#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
			i=MRR(ADDR_4300_10+1,1);//上报通道数
			if(i==1)
			{//上报通道数=1
				if((file->NUM_GPRSreport[0]>=n)||(file->NUM_ENETreport[0]>=n))
				{
					ms->msfile_Delete(filename);
					filename=recordFILENAMEmax_FLASH;//没找到
				}
			}
			else
			{//上报通道数=2
				if((file->NUM_GPRSreport[0]>=n)&&(file->NUM_ENETreport[0]>=n))
				{//二个口均已上报,删除文件
					ms->msfile_Delete(filename);
					filename=recordFILENAMEmax_FLASH;//没找到
				}
				else
				{
					if((file->NUM_GPRSreport[0]>=n)||(file->NUM_ENETreport[0]>=n))
					{//其中有一个口已上报
						if(n>=NUM_RMmax)
						{//删除文件
							ms->msfile_Delete(filename);
							filename=recordFILENAMEmax_FLASH;//没找到
						}
					}
				}
			}
		#else
			if(file->NUM_GPRSorENETreport[0]>=n)
			{
				ms->msfile_Delete(filename);
				filename=recordFILENAMEmax_FLASH;//没找到
			}
		#endif
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
	n=file->NUM_DATA;
	if(n>=NUM_RMmax)
	{
		return;
	}

//写文件
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
	ms->msfile_write(filename,n*sizeof(RecordFileHead_TypeDef),pDATA,sizeof(RecordFileHead_TypeDef));
//数据数+1
	n++;
	file->NUM_DATA=n;
//主动上报
	//始终允许

}

//抄表结束后合并在数据缓冲1
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29 采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型,全部数据为单独顺序排列无总数组)
void MeterAutoReport_RM_Record_3011(u32 PORTn,u8* pbuf)//电能表停电主动上报记录成抄表文件;入口:pbuf=数据指针
{
	u32 i;
	u32 x;
	u32 y;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;

//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//优先级:1=首要,2=必要,3=需要,4=可能
//方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集,4=补抄
//普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标
	Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
	ms=Get_ms();
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	FileID=MeterAutoReportFileID_3011;
	Comm_Ram->msFILESearchCount=0;
	filename=Get_RecordFileName(FileID,0,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
	if(filename!=0xffffffff)
	{//找到
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		i=file->NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
//		x=file->NUM_GPRSreport[0];
//		y=file->NUM_ENETreport[0];
//		if((x>=i)||(y>=i)||(i>=NUM_RMmax))
		if(i>=NUM_RMmax)
		{//存贮满
			x=filename;//寄存文件名准备改为计数1
			//删除原文件计数1文件
			Comm_Ram->msFILESearchCount=0;
			filename=Get_RecordFileName(FileID,1,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
			if(filename!=0xffffffff)
			{//找到
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
			//改为计数1
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
			filename=0xffffffff;//没找到
		}
	}
	if(filename==0xffffffff)
	{//没找到
		AutoDelete_ErrID_RecordFile();//自动删除任务已不存在的记录文件;返回:0=没删除,1=有删除
		//找空文件
		//FLASH文件
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
		MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//采集存储时标
		MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//采集启动时间
	}
//写文件
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//错
		ms->msfile_Delete(filename);
		return;
	}
	//写数据
	if(filename>=FILENAMEmax_FLASH)
	{//RAM中文件
		i=ms->ms_malloc_sizemax();//最大可分配的尺寸(字节数)
		if(i<(1024*1024))
		{//空间<1M,保留至少1M空间用于记录数据读取
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y==0)
			{//没删除
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
		{//写错误
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y)
			{//有删除
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//写地址列表
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//数据数+1
	x++;
	file->NUM_DATA=x;
	
//主动上报
	//始终允许
	
}










const u8 InitData_MeterAutoReport_3011_CSD[]=
{
//采集内容 Data
	DataType_array,
	1,
	DataType_CSD,
	1,0x30,0x11,0x02,0x00,//存储事件为电能表掉电事件
	5,
	0x20,0x22,0x02,0x00,//事件记录序号
	0x20,0x1e,0x02,0x00,//事件发生时间
	0x20,0x20,0x02,0x00,//事件结束时间
	0x20,0x24,0x02,0x00,//事件发生源
	0x33,0x00,0x02,0x00,//事件上报状态
};
void MeterAutoReport_3011(u8* p8rx,u32 Occur)//载波口上报从节点事件(电能表停电事件);入口:pAddr=从路由模块接收到的接收缓冲中的6字节地址(电表或采集器地址)
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
	
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗
//抄表结束后合并在数据缓冲1
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29 采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型,全部数据为单独顺序排列无总数组)
	PORTn=ACSAMPLEPORT;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲长度
	if(LEN_RxDataBuff>2000)
	{
		LEN_RxDataBuff=2000;//为上报时每数据处理方便限长度
	}
	p8=(u8*)Get_ADDR_UARTnFnDataBuff(PORTn);
//0 6byte 电能表地址
	MR((u32)p8,(u32)p8rx,6);
	Data_Inverse((u32)p8,6);//数据倒序(高低字节调换),入口Le>=2
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
	Get_date_time_s(p8+8);//得到date_time_s共7字节
//15 7byte 采集成功时标  date_time_s(无数据类型)
	Get_date_time_s(p8+15);//得到date_time_s共7字节
//22 7byte 采集存储时标  date_time_s(无数据类型)
	Get_date_time_s(p8+22);//得到date_time_s共7字节
//29 采集的CSD  array CSD(有数据类型)
	offset=29;
	MR((u32)p8+offset,(u32)InitData_MeterAutoReport_3011_CSD,sizeof(InitData_MeterAutoReport_3011_CSD));
	offset+=sizeof(InitData_MeterAutoReport_3011_CSD);
//n  采集的数据(有数据类型,全部数据为单独顺序排列无总数组)
	//事件记录序号
	i=MRR(ADDR_MeterAutoReport_3011_Count,4);//电能表停电主动上报事件记录序号（顺序生成）
	i++;
	MWR(i,ADDR_MeterAutoReport_3011_Count,4);//电能表停电主动上报事件记录序号（顺序生成）
	p8[offset+0]=DataType_double_long_unsigned;
	p8[offset+1]=i>>24;
	p8[offset+2]=(i>>16)&0xff;
	p8[offset+3]=(i>>8)&0xff;
	p8[offset+4]=i&0xff;
	offset+=5;
	if(Occur==1)
	{//停电发生
		//事件发生时间(河南:停电时，事件发生时间有效，以集中器收到事件的时间为准，事件结束时间null；复电时，事件发生和结束事件均有效)
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//得到date_time_s共7字节
		offset+=8;
		//事件结束时间
		p8[offset]=0;
		offset+=1;
	}
	else
	{//停电恢复
		//事件发生时间(河南:停电时，事件发生时间有效，以集中器收到事件的时间为准，事件结束时间null；复电时，事件发生和结束事件均有效)
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//得到date_time_s共7字节
		offset+=8;
		
		ms=Get_ms();
		FileID=MeterAutoReportFileID_3011;
		for(filecount=0;filecount<2;filecount++)
		{
			f=0;
			Comm_Ram->msFILESearchCount=0;
			filename=Get_RecordFileName(FileID,filecount,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
			if(filename!=0xffffffff)
			{//找到
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
				p8d=(u8*)ms->ms_malloc(i);//动态分配我的内存堆
				if(p8d)
				{
					ms->ms_free(p8d);//释放我的内存堆分配
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
									{//找到记录的发生时间
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
		
		//事件结束时间
		p8[offset]=DataType_date_time_s;
		Get_date_time_s(p8+offset+1);//得到date_time_s共7字节
		offset+=8;
	}
	//事件发生源
	p8[offset]=DataType_OAD;
	p8[offset+1]=0xf2;//F2090201
	p8[offset+2]=0x09;
	p8[offset+3]=0x02;
	p8[offset+4]=0x01;
	offset+=5;
	//事件上报状态
	p8[offset]=DataType_array;
	p8[offset+1]=1;
	offset+=2;
	//OAD_通道为载波/微功率无线     	F2090201
	p8[offset]=DataType_structure;
	p8[offset+1]=2;
	p8[offset+2]=DataType_OAD;
	p8[offset+3]=0xf2;
	p8[offset+4]=0x09;
	p8[offset+5]=0x02;
	p8[offset+6]=0x01;
	//bit0:事件发生上报标识，0—未上报，1—已上报；
	//bit1:事件发生上报确认标识，0—未确认，1—已确认；
	//bit2:事件结束（恢复）上报标识，0—未上报，1—已上报；
	//bit3:事件结束（恢复）上报确认标识，0—未确认，1—已确认。
	p8[offset+7]=DataType_unsigned;
	p8[offset+8]=1;//已上报
	offset+=9;

	i=offset;
	i-=8;
	p8[6]=i;//2byte 长度=时标+已采集的CSD(不包括本身2字节)
	p8[7]=i>>8;
	MeterAutoReport_RM_Record_3011(PORTn,p8);//电能表停电主动上报记录成抄表文件;入口:pDATA=数据指针
	UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中
}





