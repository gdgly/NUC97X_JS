
//事件记录文件
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




#if CompareMeterEventCount==1//全事件采集抄表是否比较发生次数:0=不比较(全部抄表数据写入文件),1=比较(只有次数变化的事件写入文件)
void RM_MeterEventAutoInc0x20220200(u32 PORTn)//电能表事件抄读列选择若无发生次数(事件记录序号)则自动加入
{
	u32 i;
	u32 n;
	u32 OAD;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	n=UARTCtrl->RMCSD[6];
	if(n==0)
	{//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”
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
	{//没找到
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

u32 Get_RxDATA_DL698_MeterEventCount(u32 PORTn)//从698读事件记录接收帧中取出电能表事件发生次数(事件记录序号);返回:发生次数,没找到时返回0xffffffff
{
	u32 y;
	u8* p8;
	u32 NUM_RCSDrx;
	u32 OADrx;
	u8* p8rx;

	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	NUM_RCSDrx=p8rx[21];//RCSD，SEQUENCE OF个数
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
	{//没找到列选择OAD
		return 0xffffffff;//返回:发生次数,没找到时返回0xffffffff
	}
	else
	{//找到列选择OAD
		p8=p8rx+22+(5*NUM_RCSDrx)+1;
		if(p8[0]==0)
		{//记录个数=0
			return 0xffffffff;//返回:发生次数,没找到时返回0xffffffff
		}
		p8++;
		while(y--)
		{
			p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		}
		if(p8[0]!=DataType_double_long_unsigned)
		{//空
			return 0xffffffff;//返回:发生次数,没找到时返回0xffffffff
		}
		y=p8[1]<<24;
		y|=p8[2]<<16;
		y|=p8[3]<<8;
		y|=p8[4];
		return y;
	}
}


u32 CompSourMeterEventCount(u32 SetNo,u32 OAD,u32 Count)//比较原电能表事件次数;入口:SetNo=档案配置号(0开始),Count=当前次数;返回:0=初始值,1=次数相同,2=次数不同,同时修改原计数,使用ADDR_DATABUFF
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
		return 0;//返回:0=初始值,1=次数相同,2=次数不同
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
	{//地址不同,初始化
		MR(ADDR_DATABUFF,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
		p16[0]=1;//列表个数=1
		p32[0]=OAD;
		p32[1]=Count;
		MW(ADDR_DATABUFF,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount),16);
		return 2;//返回:0=初始值,1=次数相同,2=次数不同
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
	{//没找到相同OAD
		n++;
		p16[0]=n;
		p32[0]=OAD;
		p32[1]=Count;
		//更新列表个数
		MWR(n,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount)+6,2);
		//增加列表
		MW((u32)p32,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount)+6+2+(8*i),8);
		return 2;//返回:0=初始值,1=次数相同,2=次数不同
	}
	//找到相同OAD
	x=p32[1];
	if(x==Count)
	{
		return 1;//返回:0=初始值,1=次数相同,2=次数不同
	}
	//更新事件计数值
	MWR(Count,ADDR_SourMeterEventCount+(SetNo*LENper_SourMeterEventCount)+6+2+(8*i)+4,4);
	return 2;//返回:0=初始值,1=次数相同,2=次数不同
}
#endif


u32 GET_EventFileName(u32 OAD,u32 FileCount)//搜索事件记录文件名:入口:OAD,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
{
	u32 FileID;
	msFILE_TypeDef *file;
	u32 SearchCount;
	
	if(OAD!=0xffffffff)
	{
		OAD&=0xffffff00;
		OAD|=2;//文件分类
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
			//文件分类
			if((FileID&0xff)!=2)
			{
				file++;
				continue;
			}
			//文件ID
			if(OAD!=0xffffffff)
			{
				if(FileID!=OAD)
				{
					file++;
					continue;
				}
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


u32 DEPTH_EventFile(u32 OI)//事件文件存储深度;入口:OI;返回:存储深度
{
	switch(OI)
	{
		case 0x3111://发现未知电能表事件
		case 0x3112://跨台区电能表事件
		case 0x3030://模块ID变更事件
	#if ((3*NUM_RMmax)*LEN_EventFileHead)>LEN_128KDATABUFF
		#error
	#endif
			return NUM_RMmax;//最大抄表数
		default:
			return NUMmax_Event;//最大事件记录数
	}
}


u32 EventRecord(u32 OAD,u8 *pbuf,u32 byte,u32 Occur,u32 Report)//事件记录文件;入口:Report=0不上报1=上报;返回:写入字节数
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
	NUMmax_DATA=DEPTH_EventFile(OAD>>16);//事件文件存储深度;入口:OI;返回:存储深度
	filename=GET_EventFileName(OAD,0);//搜索事件记录文件名:入口:OAD,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
	if(filename==0xffffffff)
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
				return 0;
			}
			break;
		}
	}
	else
	{//找到
		//读最后记录
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		i=file->LEN_DATA;//文件总长度
		x=file->NUM_DATA;//事件个数
		if((i>((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef)))&&(x!=0))
		{//有记录
			x--;
			x*=sizeof(EventFileHead_TypeDef);
			ms->msfile_read(filename,x,pbuf+3,4);
			EventFileHead=(EventFileHead_TypeDef*)(pbuf+3);
			x=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);//最后事件记录长度
			if(i<x)
			{
				pbuf[3]=0;
				pbuf[4]=0;
				pbuf[5]=0;
				pbuf[6]=0;
			}
			else
			{
				//读最后记录的记录号
				i-=x;
				ms->msfile_read(filename,i+3,pbuf+3,4);
				//若恢复事件则读上事件产生时间
				if(Occur==0)//产生恢复:0=恢复,1=产生,2=无
				{
					//ms->msfile_read(filename,i+2+5+1,pbuf+2+5+1,7);
					if(pbuf[23]==DataType_TSA)
					{//事件有发生源=TSA
						i=GET_LastEventTime_Occur(OAD,pbuf+23,pbuf+2+5);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
						if(i==1)
						{//1byte空填入
							pbuf[2+5]=DataType_date_time_s;//固定长度8byte，不能为空
						}
					}
					else
					{
						i=GET_LastEventTime_Occur(OAD,0,pbuf+2+5);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
						if(i==1)
						{//1byte空填入
							pbuf[2+5]=DataType_date_time_s;//固定长度8byte，不能为空
						}
					}
				}
			}
		}
	}
	//事件记录号+1
	i=(pbuf[3]<<24)|(pbuf[4]<<16)|(pbuf[5]<<8)|(pbuf[6]<<0);
	i++;
	pbuf[3]=i>>24;
	pbuf[4]=i>>16;
	pbuf[5]=i>>8;
	pbuf[6]=i>>0;
	
	if(file->BLOCK_ENTRY==0)
	{//空文件
		ms->msfile_lenclear(filename);//清0文件相关的长度次数,0-(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)表示文件在RAM
		file->FILEID=OAD|0x2;
		file->FILECOUNT=0;
		Get_date_time_s(file->save_date_time_s);//MW(ADDR_DL698YMDHMS,(u32)&file->date_time_s,7);//文件创建时间
	}
//写文件
	x=file->NUM_DATA;
	if(((x>=NUMmax_DATA)&&(Occur))||(x>=(2*NUMmax_DATA)))
	{//记在新文件，每个文件记录条数>=定义数,且是产生事件,恢复事件不记在新文件
		Comm_Ram->msFILESearchCount=0;
		while(1)
		{
			filename=GET_EventFileName(OAD,0xffffffff);////搜索事件记录文件名:入口:OAD,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
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
			{//文件计数+1
				file->FILECOUNT=1;
			}
			else
			{//删除
				i=filename;
				if(i>=recordFILENAMEmax_FLASH)
				{
					i-=recordFILENAMEmax_FLASH;
					i+=FILENAMEmax_FLASH;
				}
				ms->msfile_Delete(i);
			}
		}
		//找空文件
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
				return 0;
			}
			break;
		}
		if(file->BLOCK_ENTRY==0)
		{//空文件
			ms->msfile_lenclear(filename);//清0文件相关的长度次数,0-(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)表示文件在RAM
			file->FILEID=OAD|0x2;
			file->FILECOUNT=0;
			MW(ADDR_DL698YMDHMS,(u32)&file->save_date_time_s,7);//文件创建时间
		}
		x=0;//数据数
	}
	//写数据
	i=file->LEN_DATA;
	if(i<((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef)))
	{
		i=((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef));
	}
	while(1)
	{
		y=ms->msfile_write(filename,i,pbuf,byte);
		if(y!=byte)
		{//写错误
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y)
			{//有删除
				continue;
			}
			return 0;
		}
		break;
	}
	i+=byte;
	//写关联对象属性表∷=array OAD,数据在ADDR_128KDATABUFF+96k
	pbuf=(u8*)ADDR_128KDATABUFF+(96*1024);
	LEN_OAD=pbuf[1];
	LEN_OAD*=5;
	LEN_OAD+=2;
	while(1)
	{
		y=ms->msfile_write(filename,i,pbuf,LEN_OAD);
		if(y!=LEN_OAD)
		{//写错误
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y)
			{//有删除
				continue;
			}
			return 0;
		}
		break;
	}
	//写长度
	EventFileHead=(EventFileHead_TypeDef*)pbuf;
	EventFileHead->LEN_data[0]=(byte+LEN_OAD);
	EventFileHead->LEN_data[1]=(byte+LEN_OAD)>>8;
	EventFileHead->LEN_event[0]=byte;
	EventFileHead->LEN_event[1]=byte>>8;
	EventFileHead->REPORT=Report;//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
	EventFileHead->OCCUR=Occur;//产生恢复:0=恢复,1=产生,2=无
	EventFileHead->ACK=0;//上报确认:0=没确认,1=确认
	ms->msfile_write(filename,x*sizeof(EventFileHead_TypeDef),pbuf,sizeof(EventFileHead_TypeDef));
	//数据数+1
	x++;
	file->NUM_DATA=x;
	
//主动上报
	i=MRR(ADDR_4300_8+1,1);
	if(i==0)
	{//不允许主动上报
		for(i=0;i<NUMmax_601C;i++)
		{
		#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
			file->NUM_GPRSreport[i]=x;
			file->NUM_ENETreport[i]=x;
		#else
			file->NUM_GPRSorENETreport[i]=x;
		#endif
		}
	}
	else
	{//允许主动上报
		//上报通道
		REPORT_PORTn(file);//通道不需上报时置已上报数据个数=当前数据个数；使用ADDR_DATABUFF
	}

	return byte;
}


u32 GET_EventRecord_RCSD(u32 OAD,u8 *pRCSD,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
{
	u32 i;
	u32 n;
	u32 m;
	u32 OADn;
	u32 OADm;
	u8* pOAD;
	u32 Len;
	u8* p8;
	u32 fDATA;//有列选择OAD数据标志
	
	i=Get_DL698DataLen_S(pRecord,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
	pOAD=pRecord+i;
	n=pRCSD[0];//RCSD，SEQUENCE OF个数
	m=pOAD[1];//记录中列表的OAD个数
	pOAD+=2;
	if(n==0)
	{
		//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”。
		n=m;
		pRCSD=(u8*)ADDR_1kDATABUFF;
		pRCSD[0]=n;//RCSD，SEQUENCE OF个数
		MR((u32)pRCSD+1,(u32)pOAD,5*n);//接收缓冲在前,5*m最大=5*256极限也只能超出接收缓冲封皮入发送区但不会超出发送区
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
		{//找到
			i++;//元素号
			p8=Get_Element(pRecord,i,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8)
			{
				i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				if(LEN_DATAmax<i)
				{
					return 0;
				}
				MR((u32)pout+Len,(u32)p8,i);
				Len+=i;
				LEN_DATAmax-=i;
				fDATA=1;//有列选择OAD数据标志
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

u32 GET_EventRecord(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE,u32 ALL)//得到事件记录;入口:ALL=0只事件无OAD列表,ALL=1有OAD列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
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
	
	NUMmax_DATA=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	ms=Get_ms();
	Comm_Ram->msFILESearchCount=0;//文件搜索计数
	filename=GET_EventFileName(OAD&0xffffff00,0);//搜索事件记录文件名:入口:OI,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
	if(filename==0xffffffff)
	{//没找到
		UARTCtrl->DAR=6;//对象不存在           （6）
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
	{//单个元素(最近1个事件为元素1）
		NUM=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
		if(i>NUM)
		{
			UARTCtrl->DAR=6;//对象不存在 （6）
			return 0;
		}
		NUM=file->NUM_DATA;//事件个数
		if(i>NUM)
		{//在文件计数1文件中
			i-=NUM;
			Comm_Ram->msFILESearchCount=0;//文件搜索计数
			filename=GET_EventFileName(OAD&0xffffff00,1);//搜索事件记录文件名:入口:OI,FileCount值为0xffffffff表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
			if(filename==0xffffffff)
			{//没找到
				UARTCtrl->DAR=6;//对象不存在           （6）
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
			NUM=file->NUM_DATA;//事件个数
			if(i>NUM)
			{//文件不在计数1文件中
				UARTCtrl->DAR=6;//对象不存在           （6）
				return 0;
			}
		}
		NUM-=i;
		Len=(NUM+1)*sizeof(EventFileHead_TypeDef);
		if(Len>LEN_128KDATABUFF)
		{
			UARTCtrl->DAR=6;//对象不存在           （6）
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
		if(ALL==0)//ALL=0只事件无OAD列表,ALL=1有OAD列表
		{
			Len=EventFileHead->LEN_event[0]+(EventFileHead->LEN_event[1]<<8);
		}
		if(Len>LENmax_TxSPACE)
		{//分帧
			if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
			{
				UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
			}
			else
			{
				UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
			}
			return 0;
		}
		ms->msfile_read(filename,((2*NUMmax_DATA)*sizeof(EventFileHead_TypeDef))+i,p8tx,Len);
		//事件上报状态
//标准事件记录单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    instance-specific，
//  事件上报状态  array 通道上报状态，

//  第1个关联对象属性的数据  Data，
//  …
//  第n个关联对象属性的数据  Data
//}
//  事件上报状态  array 通道上报状态，
	//通道上报状态∷=structure
	//{
	//通道     OAD，
	//上报状态 unsigned
	//bit0:事件发生上报标识，0—未上报，1—已上报；
	//bit1:事件发生上报确认标识，0—未确认，1—已确认；
	//bit2:事件结束（恢复）上报标识，0—未上报，1—已上报；
	//bit3:事件结束（恢复）上报确认标识，0—未确认，1—已确认。
	//}
		p8tx=Get_Element(p8tx,5,0,0);//计算元素地址,使用ADDR_128KDATABUFF
		if(p8tx)
		{
			if(p8tx[0]==DataType_array)
			{
				p8tx+=2;
				i=EventFileHead->REPORT;
				if(i&2)//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
				{//已上报GPRS
					switch(EventFileHead->OCCUR)//产生恢复:0=恢复,1=产生,2=无
					{
						case 0://恢复
							if(EventFileHead->ACK)//上报确认:0=没确认,1=确认
							{
								p8tx[8]|=3<<2;
							}
							else
							{
								p8tx[8]|=1<<2;
							}
							break;
						case 1://产生
						default://无
							if(EventFileHead->ACK)//上报确认:0=没确认,1=确认
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
				if(i&4)//b0=上报标志0不需上报1需上报;b1=已上报GPRS,b2=已上报以太网,b3=
				{//已上报以太网
					switch(EventFileHead->OCCUR)//产生恢复:0=恢复,1=产生,2=无
					{
						case 0://恢复
							if(EventFileHead->ACK)//上报确认:0=没确认,1=确认
							{
								p8tx[8+9]|=3<<2;
							}
							else
							{
								p8tx[8+9]|=1<<2;
							}
							break;
						case 1://产生
						default://无
							if(EventFileHead->ACK)//上报确认:0=没确认,1=确认
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
	{//全部元素
		UARTCtrl->DAR=6;//对象不存在           （6）
		return 0;
	}
}


u32 GET_LastEventOccurRecovery(u32 OAD,u8* pTSA)//得到最近次事件产生恢复标志;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:0=恢复,1=产生;使用ADDR_DATABUFF
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
	
	OAD|=2;//文件分类
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
				//文件ID
				if(file->FILEID!=OAD)
				{
					file++;
					continue;
				}
				//文件计数
				if(file->FILECOUNT!=filecount)
				{
					file++;
					continue;
				}
				i=file->NUM_DATA;//事件个数
				if(i==0)
				{
					file++;
					continue;
				}
				if(pTSA==0)
				{//没发生源=抄表地址
					ms=Get_ms();
					i--;
					i*=sizeof(EventFileHead_TypeDef);
					ms->msfile_read(filename,i,(u8*)ADDR_DATABUFF,sizeof(EventFileHead_TypeDef));
					EventFileHead=(EventFileHead_TypeDef*)(ADDR_DATABUFF);
					i=EventFileHead->OCCUR;//产生恢复:0=恢复,1=产生,2=无
					if(i!=1)
					{
						i=0;
					}
					return i;//返回:0=恢复,1=产生
				}
				else
				{//有发生源=抄表地址
					ms=Get_ms();
					i=file->LEN_DATA;
					p8=ms->ms_malloc(i);//动态分配我的内存堆
					if(p8)
					{
						ms->ms_free(p8);//释放我的内存堆分配
						ms->msfile_read(filename,0,p8,i);
						NUM_DATA=file->NUM_DATA;//事件个数
						n=NUM_DATA;
						while(n--)
						{
							EventFileHead=(EventFileHead_TypeDef*)p8;
							offset=DEPTH_EventFile(OAD>>16);//事件文件存储深度;入口:OI;返回:存储深度
							offset*=2;
							offset*=sizeof(EventFileHead_TypeDef);
							for(i=0;i<n;i++)
							{
								offset+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								EventFileHead++;
							}
							if(offset>=file->LEN_DATA)
							{//偏移错
								return 1;//返回:0=恢复,1=产生
							}
							if(p8[offset+2+5+8]==DataType_date_time_s)
							{//有结束时间
								i=Compare_DL698DataString(pTSA,p8+offset+23,3+6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							}
							else
							{//开始时间总=有,结束时间=NULL
								i=Compare_DL698DataString(pTSA,p8+offset+(23-7),3+6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							}
							if(i==0)
							{//相同
								i=EventFileHead->OCCUR;//产生恢复:0=恢复,1=产生,2=无
								if(i!=1)
								{
									i=0;
								}
								return i;//返回:0=恢复,1=产生
							}
						}
					}
				}
			}
			file++;
		}
	}
	return 0;//返回:0=恢复,1=产生
}

u32 GET_LastEventTime_OccurRecovery(u32 OAD,u8* pTSA,u8* pOut,u32 OccurRecovery)//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
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
	
	OAD|=2;//文件分类
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
				//文件ID
				if(file->FILEID!=OAD)
				{
					file++;
					continue;
				}
				//文件计数
				if(file->FILECOUNT!=filecount)
				{
					file++;
					continue;
				}
				i=file->NUM_DATA;//事件个数
				if(i==0)
				{
					file++;
					continue;
				}
				if(pTSA==0)
				{//没发生源=抄表地址
					ms=Get_ms();
					i=file->LEN_DATA;
					p8=ms->ms_malloc(i);//动态分配我的内存堆
					if(p8)
					{
						ms->ms_free(p8);//释放我的内存堆分配
						ms->msfile_read(filename,0,p8,i);
						NUM_DATA=file->NUM_DATA;//事件个数
						n=NUM_DATA;
						while(n--)
						{
							EventFileHead=(EventFileHead_TypeDef*)p8;
							offset=DEPTH_EventFile(OAD>>16);//事件文件存储深度;入口:OI;返回:存储深度
							offset*=2;
							offset*=sizeof(EventFileHead_TypeDef);
							for(i=0;i<n;i++)
							{
								offset+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								EventFileHead++;
							}
							if(offset>=file->LEN_DATA)
							{//偏移错
								pOut[0]=0;
								return 1;
							}
							i=EventFileHead->OCCUR;//产生恢复:0=恢复,1=产生,2=无
							if((i==OccurRecovery)||(i==2))
							{
								if(OccurRecovery==1)
								{//最近次产生时间
									if(p8[offset+2+5]!=DataType_date_time_s)
									{
										pOut[0]=0;
										return 1;
									}
									MR((u32)pOut,(u32)p8+offset+2+5,8);//取发生时间
									return 8;
								}
								else
								{//最近次恢复时间
									if(p8[offset+2+5+8]!=DataType_date_time_s)
									{
										pOut[0]=0;
										return 1;
									}
									MR((u32)pOut,(u32)p8+offset+2+5+8,8);//取发生时间
									return 8;
								}
							}
						}
					}
				}
				else
				{//有发生源=抄表地址
					ms=Get_ms();
					i=file->LEN_DATA;
					p8=ms->ms_malloc(i);//动态分配我的内存堆
					if(p8)
					{
						ms->ms_free(p8);//释放我的内存堆分配
						ms->msfile_read(filename,0,p8,i);
						NUM_DATA=file->NUM_DATA;//事件个数
						n=NUM_DATA;
						while(n--)
						{
							EventFileHead=(EventFileHead_TypeDef*)p8;
							offset=DEPTH_EventFile(OAD>>16);//事件文件存储深度;入口:OI;返回:存储深度
							offset*=2;
							offset*=sizeof(EventFileHead_TypeDef);
							for(i=0;i<n;i++)
							{
								offset+=EventFileHead->LEN_data[0]+(EventFileHead->LEN_data[1]<<8);
								EventFileHead++;
							}
							if(offset>=file->LEN_DATA)
							{//偏移错
								pOut[0]=0;
								return 1;
							}
							if(p8[offset+2+5+8]==DataType_date_time_s)
							{//有结束时间
								i=Compare_DL698DataString(pTSA,p8+offset+23,3+6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							}
							else
							{//开始时间总=有,结束时间=NULL
								i=Compare_DL698DataString(pTSA,p8+offset+(23-7),3+6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							}
							if(i==0)
							{//相同
								i=EventFileHead->OCCUR;//产生恢复:0=恢复,1=产生,2=无
								if((i==OccurRecovery)||(i==2))
								{
									if(OccurRecovery==1)
									{//最近次产生时间
										if(p8[offset+2+5]!=DataType_date_time_s)
										{
											pOut[0]=0;
											return 1;
										}
										MR((u32)pOut,(u32)p8+offset+2+5,8);//取发生时间
										return 8;
									}
									else
									{//最近次恢复时间
										if(p8[offset+2+5+8]!=DataType_date_time_s)
										{
											pOut[0]=0;
											return 1;
										}
										MR((u32)pOut,(u32)p8+offset+2+5+8,8);//取发生时间
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
u32 GET_LastEventTime_Occur(u32 OAD,u8* pTSA,u8* pOut)//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
{
	return GET_LastEventTime_OccurRecovery(OAD,pTSA,pOut,1);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
}
u32 GET_LastEventTime_Recovery(u32 OAD,u8* pTSA,u8* pOut)//得到最近次事件恢复时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
{
	return GET_LastEventTime_OccurRecovery(OAD,pTSA,pOut,0);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
}



EventSour_typedef* GET_EventSour(u16* pRMAddr)//得到事件原值数据指针，入口:抄表地址, 返回:数据指针,0=错误
{
	u32 i;
	EventSour_typedef* EventSour;
	
	i=AddrToMeterSetNo(pRMAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
	if(i>=NUM_RMmax)
	{
		return 0;//无效指针
	}
	EventSour=(EventSour_typedef*)(ADDR_Event_Sour+(i*sizeof(EventSour_typedef)));
	for(i=0;i<3;i++)
	{
		if(EventSour->RMAddr[i]!=pRMAddr[i])
		{//地址不同,清事件原数据
			for(i=0;i<3;i++)
			{
				EventSour->RMAddr[i]=pRMAddr[i];
			}
			EventSour->res=0;
			EventSour->Re=0;//抄表失败(310F)重试轮次
			EventSour->Occur=0;//事件产生恢复,bit=0恢复=1产生
			EventSour->TimeCount=0;//电能表停走时间计时(秒)
			break;
		}
	}
	return EventSour;
}


void CLR_EventRecord(u32 OI)//清相应OI的事件记录
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
			//文件分类
			if((FileID&0xff)!=2)
			{
				file++;
				continue;
			}
			//文件OI
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
		//清事件当前值记录表
			pLib=Get_DL698_EVENT_pLib(OI<<16);//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
			if(pLib)
			{
				if(pLib->Addr_Currentvalue)
				{
					MC(0,pLib->Addr_Currentvalue,LENper_EventCurrentvalue);
				}
			}
		//清相关的已记录标志
			if(OI==0x3110)
			{//通信流量超限事件
				MC(0,ADDR_2200_EventRecordFags,1);//通信流量超限事件已记录标志,月更新时清0
			}
		}
		file++;
	}
}



u32 EVENT_HeadData(u32 OAD,u8* pOccurSour,u32 Occur)//标准事件头数据;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* pOAD;
	u8* p8;
	u32 ADDR_EventOAD;
	DL698_EVENTLIB_TypeDef * pLib;
//	UARTCtrl_TypeDef *UARTCtrl;
	
//事件缓冲使用约定
//ADDR_DATABUFF事件编辑存放
//ADDR_128KDATABUFF  32KBYTE 保留给DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE 事件参数,关联对象属性表等
//ADDR_128KDATABUFF+64K 32KBYTE 产生事件用临时数据
//ADDR_128KDATABUFF+96K 32KBYTE 事件中各项数据的相应OAD顺序表array OAD
	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD

	p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
	p8[0]=DataType_array;
	p8[1]=0;
	pLib=Get_DL698_EVENT_pLib(OAD);//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
	if(pLib==0)
	{
		return 0;//返回:0=没记录,1=记录
	}
	ADDR_EventOAD=pLib->Addr_EventOAD;
#if (4+LENmax_EventOAD)>(32*1024)
	#error
#endif
	MR(ADDR_128KDATABUFF+(32*1024),ADDR_EventOAD,4+LENmax_EventOAD);
	pOAD=(u8*)ADDR_128KDATABUFF+(32*1024);
	//有效标识
	if(pOAD[0]!=DataType_bool)
	{
		return 0;//返回:b0-b15总长度,b16=上报标识0=不上报1=上报
	}
	if(pOAD[1]==0)
	{
		return 0;//返回:b0-b15总长度,b16=上报标识0=不上报1=上报
	}
	Report=pOAD[3];//上报标识
	switch(Report)
	{
		default:
		case 0://不上报
			break;
		case 1://事件发生上报
			if(Occur==0)
			{
				Report=0;
			}
			break;
		case 2://事件恢复上报
			if(Occur==1)
			{
				Report=0;
			}
			break;
		case 3://事件发生恢复均上报
			Report=1;
			break;
	}
	
	p8=(u8*)ADDR_DATABUFF;
//标准事件记录单元∷=structure
//{
	p8[0]=DataType_structure;
	p8[1]=5;//结构体元素数
//  事件记录序号  double-long-unsigned，
	p8[2]=DataType_double_long_unsigned;
	p8[3]=0;
	p8[4]=0;
	p8[5]=0;
	p8[6]=0;
	switch(Occur)
	{
		case 0://恢复
	//  事件发生时间  date_time_s，
			p8[7]=DataType_date_time_s;
			p8[8]=0;//0xff;
			p8[9]=0;//0xff;
			p8[10]=0;//0xff;
			p8[11]=0;//0xff;
			p8[12]=0;//0xff;
			p8[13]=0;//0xff;
			p8[14]=0;//0xff;
	//  事件结束时间  date_time_s，
			p8[15]=DataType_date_time_s;
			Get_date_time_s(p8+16);//得到date_time_s共7字节
			Len=23;
			break;
		case 1://产生
	//  事件发生时间  date_time_s，
			p8[7]=DataType_date_time_s;
			Get_date_time_s(p8+8);//得到date_time_s共7字节
	//  事件结束时间  date_time_s，
			p8[15]=0;//NULL
			Len=16;
			break;
		default://无产生恢复
	//  事件发生时间  date_time_s，
			p8[7]=DataType_date_time_s;
#if (USER/100)==15//安徽
		if(OAD==0x31140200)
			MR((u32)p8+8,ADDR_128KDATABUFF+(64*1024),7);
		else
			Get_date_time_s(p8+8);//得到date_time_s共7字节
#else
			Get_date_time_s(p8+8);//得到date_time_s共7字节
#endif
	//  事件结束时间  date_time_s，
			p8[15]=DataType_date_time_s;
			Get_date_time_s(p8+16);//得到date_time_s共7字节
			Len=23;
			break;
	}
//  事件发生源    instance-specific，
	if(pOccurSour==0)
	{
		p8[Len]=0;//NULL
		Len++;
	}
	else
	{
		i=Get_DL698DataLen_S(pOccurSour,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		if(i>(LEN_DATABUFF-1024))
		{
			return 0;//错误
		}
		MR((u32)p8+Len,(u32)pOccurSour,i);
		Len+=i;
	}
//  事件上报状态  array 通道上报状态，
#if(USER/100)==17//江苏系
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
#if(USER/100)==17//江苏系
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
	//bit0:事件发生上报标识，0—未上报，1—已上报；
	//bit1:事件发生上报确认标识，0—未确认，1—已确认；
	//bit2:事件结束（恢复）上报标识，0—未上报，1—已上报；
	//bit3:事件结束（恢复）上报确认标识，0—未确认，1—已确认。
	p8[Len+7]=DataType_unsigned;
#if(USER/100)==17//江苏系
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
	//bit0:事件发生上报标识，0—未上报，1—已上报；
	//bit1:事件发生上报确认标识，0—未确认，1—已确认；
	//bit2:事件结束（恢复）上报标识，0—未上报，1—已上报；
	//bit3:事件结束（恢复）上报确认标识，0—未确认，1—已确认。
	p8[Len+7]=DataType_unsigned;
	p8[Len+8]=0;
	Len+=9;
#endif
//  第1个关联对象属性的数据  Data，
//  …
//  第n个关联对象属性的数据  Data
//}

	p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
	p8[0]=DataType_array;
	p8[1]=5;//5个
	p8[2]=0;//DataType_OAD;//事件记录序号
	p8[3]=0x20;
	p8[4]=0x22;
	p8[5]=0x02;
	p8[6]=0x00;
	p8[7]=0;//DataType_OAD;//事件发生时间
	p8[8]=0x20;
	p8[9]=0x1e;
	p8[10]=0x02;
	p8[11]=0x00;
	p8[12]=0;//DataType_OAD;//事件结束时间
	p8[13]=0x20;
	p8[14]=0x20;
	p8[15]=0x02;
	p8[16]=0x00;
	p8[17]=0;//DataType_OAD;//事件发生源
	p8[18]=0x20;
	p8[19]=0x24;
	p8[20]=0x02;
	p8[21]=0x00;
	p8[22]=0;//DataType_OAD;//事件上报状态
	p8[23]=0x33;
	p8[24]=0x00;
	p8[25]=0x02;
	p8[26]=0x00;
	return (Len<<8)|Report;
}

u32 EVENT_OADData(u32 Len,u32 OccurkWh)//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,OccurkWh=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
{
	u32 i;
	u32 NUM_OAD;
	u8* pOAD;
	u8* p8;
	u8* pALLOAD;
	
	p8=(u8*)ADDR_DATABUFF;
	pOAD=(u8*)ADDR_128KDATABUFF+(32*1024)+4;
	NUM_OAD=pOAD[1];//关联OAD数
	pOAD+=2;
	while(NUM_OAD--)
	{
		if(pOAD[0]==DataType_OAD)
		{//OAD
			if(Len>(LEN_DATABUFF-1))
			{
				return 0;//错误
			}
			p8[1]++;//结构体元素数
			//事件中各项数据的相应OAD顺序表array OAD
			pALLOAD=(u8*)ADDR_128KDATABUFF+(96*1024);
			i=pALLOAD[1];
			pALLOAD[1]++;//个数+1
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
				case 0x201E4200://事件发生时间
				case 0x20204200://事件结束时间
					p8[Len]=DataType_date_time_s;
					Get_date_time_s(p8+Len+1);//得到date_time_s共7字节
					Len+=8;
					break;
				case 0x00102200://00102201事件发生前正向有功总电能及费率
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024),0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024),i);
					Len+=i;
					break;
				case 0x00102201://00102201事件发生前正向有功总电能
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+2,i);
					Len+=i;
					break;
				case 0x00106200://00102201事件结束前正向有功总电能及费率
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024),0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024),i);
					Len+=i;
					break;
				case 0x00106201://00102201事件结束前正向有功总电能
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+2,i);
					Len+=i;
					break;
				case 0x00104200://00102201事件发生后正向有功总电能及费率
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024,i);
					Len+=i;
					break;
				case 0x00104201://00102201事件发生后正向有功总电能
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+2,i);
					Len+=i;
					break;
				case 0x00108200://00108201事件结束后正向有功总电能及费率
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024,i);
					Len+=i;
					break;
				case 0x00108201://00108201事件结束后正向有功总电能
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+2,i);
					Len+=i;
					break;
					
				case 0x00202200://00202201事件发生前反向有功总电能及费率
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128,i);
					Len+=i;
					break;
				case 0x00202201://00202201事件发生前反向有功总电能
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128+2,i);
					Len+=i;
					break;
				case 0x00206200://00202201事件结束前反向有功总电能及费率
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128,i);
					Len+=i;
					break;
				case 0x00206201://00202201事件结束前反向有功总电能
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+128+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+128+2,i);
					Len+=i;
					break;
				case 0x00204200://00202201事件发生后反向有功总电能及费率
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128,i);
					Len+=i;
					break;
				case 0x00204201://00202201事件发生后反向有功总电能
					if(OccurkWh!=1)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128+2,i);
					Len+=i;
					break;
				case 0x00208200://00208201事件结束后反向有功总电能及费率
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128,i);
					Len+=i;
					break;
				case 0x00208201://00208201事件结束后反向有功总电能
					if(OccurkWh!=0)//occur=0恢复,=1产生,=2无
					{
						p8[Len]=0;
						Len+=1;
						break;
					}
					i=Get_DL698DataLen_S((u8*)ADDR_128KDATABUFF+(64*1024)+1024+128+2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					MR((u32)p8+Len,ADDR_128KDATABUFF+(64*1024)+1024+128+2,i);
					Len+=i;
					break;
				
				case 0x40002200://对时事件发生前对时时间
					p8[Len]=DataType_date_time_s;
					MR((u32)p8+Len+1,ADDR_128KDATABUFF+(64*1024),7);
					Len+=8;
					break;
				case 0x40008200://对时事件结束后对时时间
					p8[Len]=DataType_date_time_s;
					MR((u32)p8+Len+1,ADDR_DL698YMDHMS,7);
					Len+=8;
					break;

				case 0x22004202://事件发生后当月通信流量
					MR((u32)p8+Len,ADDR_2200+2+5,5);
					Len+=5;
					break;
				
				default:
					i=GET_OAD_DATA(ACSAMPLEPORT,i,p8+Len,LEN_DATABUFF-Len);//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
					if(i==0)
					{//无数据填入
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
			i=Get_DL698DataLen_S(pOAD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			pOAD+=i;
		}
	}
	return Len;
}

void Event_OAD(u32 OAD,u8* pOccurSour,u32 Occur,u32 OccurkWh)//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 Len;
	u32 Report;
	
	Len=EVENT_HeadData(OAD,pOccurSour,Occur);//标准事件头数据;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		Len=EVENT_OADData(Len,OccurkWh);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(OAD,(u8*)ADDR_DATABUFF,Len,Occur,Report);//事件记录文件;返回:写入字节数
	}
}


void Event_30300200(u8* pDATA, u8* pOccurSour,u32 FramLen)//模块ID变更事件记录;入口:pDATA=模块ID变更属性;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x30300200,pOccurSour,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=3;//结构体元素数
		MR(ADDR_DATABUFF+Len,(u32)pDATA,FramLen);
		Len+=FramLen;

		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=3;//个数+3
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//模块ID变更事件单元-6
		p8[1]=0x33;
		p8[2]=0x12;
		p8[3]=0x02;
		p8[4]=6;
		
		p8[5]=0;//DataType_OAD;//模块ID变更事件单元-7
		p8[6]=0x33;
		p8[7]=0x12;
		p8[8]=0x02;
		p8[9]=7;

		p8[10]=0;//DataType_OAD;//模块ID变更事件单元-8
		p8[11]=0x33;
		p8[12]=0x12;
		p8[13]=0x02;
		p8[14]=8;

		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x30300200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}

void Event_31010200(void)//终端版本变更事件
{
	Event_OAD(0x31010200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
}


//电能表时钟超差记录单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    TSA，
//  事件上报状态  array 通道上报状态，
//  电能表时钟    DATETIME_S，
//  终端当前时钟  DATETIME_S，
//第1个关联对象属性的数据   Data，
//  …
//  第n个关联对象属性的数据   Data
//}

void Event_31050200(u8* pOccurSour,u32 Occur,u8* p1)//电能表时钟超差记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	Len=EVENT_HeadData(0x31050200,pOccurSour,Occur);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=2;//结构元素数
//  电能表时钟    DATETIME_S，
		MR(ADDR_DATABUFF+Len,(u32)p1,8);//产生事件用临时数据
		Len+=8;
//  终端当前时钟  DATETIME_S，
		p8=(u8*)ADDR_DATABUFF+Len;
		p8[0]=DataType_date_time_s;
		MR(ADDR_DATABUFF+Len+1,ADDR_DL698YMDHMS,7);
		Len+=8;
//	事件中各项数据的相应OAD顺序表array OAD
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=2;//个数+2
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD
		p8[1]=0x33;//电能表时钟超差记录单元,元素6,电能表时钟    DATETIME_S
		p8[2]=0x13;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD
		p8[6]=0x33;//电能表时钟超差记录单元,元素7,终端当前时钟  DATETIME_S
		p8[7]=0x13;
		p8[8]=0x02;
		p8[9]=7;
		
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x31050200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//事件记录文件;返回:写入字节数
	}
}

//停/上电事件记录单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    enum{停电(0)，上电(1)}，
//  事件上报状态  array 通道上报状态，  
//属性标志     bit-string（SIZE(8)），
//  第1个关联对象属性的数据   Data，
//  …
//  第n个关联对象属性的数据   Data
//}
//属性标志：
//bit0置“1”：事件正常，bit0置“0”：事件异常，此处是对停电时间有效间隔的判断；
//bit1置“1”：事件有效，bit1置“0”：事件无效，此处是对停电时间偏差限值的判断；
//bit2～bit7备用。
void Event_31060200(u8* pOccurSour,u32 Occur)//终端停/上电事件记录;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

//事件发生源    enum{停电(0)，上电(1)}
	pOccurSour=(u8*)ADDR_128KDATABUFF+(64*1024);
	pOccurSour[0]=DataType_enum;
	if(Occur)
	{
		pOccurSour[1]=0;//停电(0)
	}
	else
	{
		pOccurSour[1]=1;//上电(1)
	}
	Len=EVENT_HeadData(0x31060200,pOccurSour,Occur);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		if(Occur==1)
		{//产生
			//修改产生时间
			MR((u32)ADDR_DATABUFF+8,(u32)&PowerEvent->Tstarttime,7);
		}
		else
		{//恢复
			//修改产生时间
			MR((u32)ADDR_DATABUFF+8,(u32)&PowerEvent->Tstarttime,7);
			//修改恢复时间
			MR((u32)ADDR_DATABUFF+16,(u32)&PowerEvent->Tstoptime,7);
		}
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//结构体元素数
		p8[Len]=DataType_bit_string;
		p8[Len+1]=8;
		p8[Len+2]=PowerEvent->bit;//属性标志：
		Len+=3;
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]++;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//停/上电事件记录单元,元素6
		p8[1]=0x33;
		p8[2]=0x09;
		p8[3]=0x02;
		p8[4]=6;

		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x31060200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//事件记录文件;返回:写入字节数
	}
}	
void Event_310A0200(u8* pOccurSour,u32 Occur,u8* pTSA)//
{
	u32 Len;
	u32 Report;
	u8* p8;
	u32 i;
	Len=EVENT_HeadData(0x310A0200,pOccurSour,Occur);//标准事件头数据;入口:pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//结构体元素数
		p8[Len]=DataType_TSA;
		p8[Len+1]=7;
		p8[Len+2]=5;
		MR((u32)p8+Len+3,(u32)pTSA,6);
		Data_Inverse((u32)p8+Len+3,6);
		Len+=9;
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]++;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//目标服务器地址
		p8[1]=0x20;
		p8[2]=0x2A;
		p8[3]=0x02;
		p8[4]=0;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x310A0200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//事件记录文件;返回:写入字节数
	}
}

//拒绝从节点入网记录单元∷=structure
//{
//		事件记录序号 double-long-unsigned，
//		事件发生时间 date_time_s，
//		事件结束时间 date_time_s，
//		事件发生源 TSA，
//		事件上报状态 array 通道上报状态，
//		拒绝从节点列表 array octet-string，
//		第 1 个关联对象属性的数据 Data，
//		...
//		第 n 个关联对象属性的数据 Data
//}
void Event_311F0200(u8* pDATA,u8* pOccurSour,u32 FramLen)//拒绝从节点入网记录:入口:pDATA=1个拒绝从节点入网记录单元;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x311F0200,pOccurSour,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{		
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=1;//结构体元素数
		MR(ADDR_DATABUFF+Len,(u32)pDATA,FramLen);
		Len+=FramLen;

		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=1;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//拒绝从节点入网记录单元-6
		p8[1]=0x33;
		p8[2]=0x1B;
		p8[3]=0x02;
		p8[4]=6;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x311F0200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}


//发现未知电能表事件∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    NULL，
//  事件上报状态  array 通道上报状态，
//  搜表结果      array 一个搜表结果，
//}
void Event_31110200(u8* pDATA)//发现未知电能表事件记录;入口:pDATA=1个搜表结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x31110200,0,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//结构体元素数
		p8[Len]=DataType_array;
		p8[Len+1]=1;
		Len+=2;
		i=Get_DL698DataLen_S(pDATA,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		MR(ADDR_DATABUFF+Len,(u32)pDATA,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]++;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//发现未知电能表事件单元-6
		p8[1]=0x33;
		p8[2]=0x03;
		p8[3]=0x02;
		p8[4]=6;
		//Len=EVENT_OADData(Len);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x31110200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}
//跨台区电能表事件单元∷=structure
//{
//  事件记录序号    double-long-unsigned，
//  事件发生时间    date_time_s，
//  事件结束时间    date_time_s，
//  事件发生源      NULL，
//  事件上报状态    array 通道上报状态，
//  跨台区搜表结果  array  一个跨台区结果，
//  第1个关联对象属性的数据  Data，
//  …
//  第n个关联对象属性的数据  Data
//}
void Event_31120200(u8* pDATA)//跨台区电能表事件记录;入口:pDATA=一个跨台区结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x31120200,0,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//结构体元素数
		p8[Len]=DataType_array;
		p8[Len+1]=1;
		Len+=2;
		i=Get_DL698DataLen_S(pDATA,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		MR(ADDR_DATABUFF+Len,(u32)pDATA,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]++;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//跨台区电能表事件单元-6
		p8[1]=0x33;
		p8[2]=0x04;
		p8[3]=0x02;
		p8[4]=6;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x31120200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}




//遥控事件记录单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    OAD，
//  事件上报状态  array 通道上报状态，
//  控后2分钟总加组功率 array long64，
//  第1个关联对象属性的数据   Data，
//  …
//  第n个关联对象属性的数据   Data
//}
void Event_31150200(u8* pOAD,u32 Occur)//遥控事件记录(控后2分钟),入口:pOAD=事件发生源OAD（继电器单元）,occur=0恢复=1产生=2无
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	Len=EVENT_HeadData(0x31150200,pOAD,Occur);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=1;//结构体元素数
		//控后2分钟总加组功率 array long64
		p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
		p8[0]=DataType_array;
		p8[1]=1;
		MR((u32)p8+2,ADDR_230x_3,9);
		i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)p8,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=1;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//编程对象列表
		p8[1]=0x33;
		p8[2]=0x0A;
		p8[3]=0x02;
		p8[4]=6;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x31150200,(u8*)ADDR_DATABUFF,Len,Occur,Report);//事件记录文件;返回:写入字节数
	}
}




//编程记录事件单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    NULL，
//  事件上报状态  array 通道上报状态，
//  编程对象列表  array OAD，
//  第1个关联对象属性的数据  Data，
//  …
//  第n个关联对象属性的数据  Data
//}
void Event_31180200(u8* pOAD)//终端编程事件记录;入口:pOAD=设置的OAD数组指针;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	Len=EVENT_HeadData(0x31180200,0,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//结构体元素数
		i=Get_DL698DataLen_S(pOAD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		MR(ADDR_DATABUFF+Len,(u32)pOAD,i);
		Len+=i;
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]++;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//编程对象列表
		p8[1]=0x33;
		p8[2]=0x02;
		p8[3]=0x02;
		p8[4]=6;
		
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x31180200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
		
	}
}






//电能表数据变更监控记录单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    TSA，
//  事件上报状态  array 通道上报状态，
//  监控数据对象  CSD，
//  变化前数据    Data，
//  变化后数据    Data
//}
//事件发生时间：为监控数据发生变化的时刻；
//事件结束时间：无效，各字段填FF。
void Event_311C0200(u32 PORTn,u8* pCSD,u8* pDATA1,u8* pDATA2)//电能表数据变更监控记录,入口:pCSD=监控的CSD,pDATA1=变更前数据,pDATA2=变更后数据
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
	Len=EVENT_HeadData(0x311C0200,UARTCtrl->TSA,1);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=3;//结构体元素数
		i=Get_DL698DataLen_S(pCSD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)pCSD,i);
		Len+=i;
		i=Get_DL698DataLen_S(pDATA1,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)pDATA1,i);
		Len+=i;
		i=Get_DL698DataLen_S(pDATA2,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		if((Len+i)>LEN_DATABUFF)
		{
			return;
		}
		MR(ADDR_DATABUFF+Len,(u32)pDATA2,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=3;//个数+3
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//编程对象列表
		p8[1]=0x33;
		p8[2]=0x0F;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//编程对象列表
		p8[6]=0x33;
		p8[7]=0x0F;
		p8[8]=0x02;
		p8[9]=7;
		p8[10]=0;//DataType_OAD;//编程对象列表
		p8[11]=0x33;
		p8[12]=0x0F;
		p8[13]=0x02;
		p8[14]=8;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x311C0200,(u8*)ADDR_DATABUFF,Len,1,Report);//事件记录文件;返回:写入字节数
	}
}


//3305 功控跳闸记录单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    OI，
//  事件上报状态  array 通道上报状态，
//  事件发生后2分钟功率  long64(单位：W，换算-1)，
//  控制对象      OI，
//  跳闸轮次      bit-string(SIZE(8))，
//	功控定值      long64（单位：kW，换算-4），
//	跳闸发生前总加有功功率    long64（单位：kW，换算-4），
//  第1个关联对象属性的数据  Data，
//  …
//  第n个关联对象属性的数据  Data
//}
void Event_32000200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst,u8* pAfter)//功控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	//入口:pOccurSour=事件发生源    OI
	//OI=控制对象      OI
	//bit=跳闸轮次      bit-string(SIZE(8))
	//pFixedValue=功控定值      long64（单位：kW，换算-4）
	//pFirst=跳闸发生前总加有功功率    long64（单位：kW，换算-4）
	//pAfter=事件发生后2分钟功率  long64(单位：W，换算-1)
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
	p8[0]=DataType_OI;
	p8[1]=pOccurSour>>8;
	p8[2]=pOccurSour;
	Len=EVENT_HeadData(0x32000200,p8,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=5;//结构体元素数
//  事件发生后2分钟功率  long64(单位：W，换算-1)，
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pAfter,8);
		Len+=9;
//  控制对象      OI，
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_OI;
		p8[Len+1]=OI>>8;
		p8[Len+2]=OI;
		Len+=3;
//  跳闸轮次      bit-string(SIZE(8))，
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_bit_string;
		p8[Len+1]=8;
		p8[Len+2]=bit;
		Len+=3;
//	功控定值      long64（单位：kW，换算-4），
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFixedValue,8);
		Len+=9;
//	跳闸发生前总加有功功率    long64（单位：kW，换算-4），
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFirst,8);
		Len+=9;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=5;//个数+5
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//事件发生后2分钟功率
		p8[1]=0x33;
		p8[2]=0x05;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//控制对象      OI
		p8[6]=0x33;
		p8[7]=0x05;
		p8[8]=0x02;
		p8[9]=7;
		p8[10]=0;//DataType_OAD;//跳闸轮次
		p8[11]=0x33;
		p8[12]=0x05;
		p8[13]=0x02;
		p8[14]=8;
		p8[15]=0;//DataType_OAD;//功控定值
		p8[16]=0x33;
		p8[17]=0x05;
		p8[18]=0x02;
		p8[19]=9;
		p8[20]=0;//DataType_OAD;//跳闸发生前总加有功功率
		p8[21]=0x33;
		p8[22]=0x05;
		p8[23]=0x02;
		p8[24]=10;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x32000200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}


//3306 电控跳闸记录单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    OI，
//  事件上报状态  array 通道上报状态，
//  控制对象      OI，
//  跳闸轮次      bit-string(SIZE(8))，
//电控定值      long64（单位：kWh，换算-4），
//跳闸发生时总加电能量  long64（单位：kwh/元，换算-4），

//  第1个关联对象属性的数据  Data，
//  …
//  第n个关联对象属性的数据  Data
//}
//说明：
//当事件发生源类型为月电控8108，跳闸时总加电能量为总加月电能量；
//当事件发生源类型为购电控8107，跳闸时总加电能量为剩余电能量/费。
void Event_32010200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst)//电控跳闸记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	//入口:pOccurSour=事件发生源    OI
	//OI=控制对象      OI
	//bit=跳闸轮次      bit-string(SIZE(8))
	//pFixedValue=电控定值 long64（单位：kWh，换算-4）
	//pFirst=跳闸发生时总加电能量  long64（单位：kwh/元，换算-4）
//当事件发生源类型为月电控8108，跳闸时总加电能量为总加月电能量；
//当事件发生源类型为购电控8107，跳闸时总加电能量为剩余电能量/费。
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
	p8[0]=DataType_OI;
	p8[1]=pOccurSour>>8;
	p8[2]=pOccurSour;
	Len=EVENT_HeadData(0x32010200,p8,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=4;//结构体元素数
//  控制对象      OI，
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_OI;
		p8[Len+1]=OI>>8;
		p8[Len+2]=OI;
		Len+=3;
//  跳闸轮次      bit-string(SIZE(8))，
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_bit_string;
		p8[Len+1]=8;
		p8[Len+2]=bit;
		Len+=3;
//	电控定值      long64（单位：kW，换算-4），
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFixedValue,8);
		Len+=9;
//	跳闸发生时总加电能量  long64（单位：kW，换算-4），
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFirst,8);
		Len+=9;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=4;//个数+5
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//控制对象      OI
		p8[1]=0x33;
		p8[2]=0x06;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//跳闸轮次
		p8[6]=0x33;
		p8[7]=0x06;
		p8[8]=0x02;
		p8[9]=7;
		p8[10]=0;//DataType_OAD;//电控定值
		p8[11]=0x33;
		p8[12]=0x06;
		p8[13]=0x02;
		p8[14]=8;
		p8[15]=0;//DataType_OAD;//跳闸发生时总加电能量
		p8[16]=0x33;
		p8[17]=0x06;
		p8[18]=0x02;
		p8[19]=9;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x32010200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}

//3307 电控告警事件单元∷=structure
//{
//  事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    OI，
//  事件上报状态  array 通道上报状态，
//  控制对象      OI，
//  电控定值      long64（单位：kWh，换算-4），
//  第1个关联对象属性的数据  Data，
//  …
//  第n个关联对象属性的数据  Data
//}
void Event_32030200(u32 pOccurSour,u32 OI,u8* pFixedValue)//电控告警事件记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	//入口:pOccurSour=事件发生源    OI
	//OI=控制对象      OI
	//pFixedValue=电控定值 long64（单位：kWh，换算-4）
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;
	
	p8=(u8*)(ADDR_128KDATABUFF+(64*1024));
	p8[0]=DataType_OI;
	p8[1]=pOccurSour>>8;
	p8[2]=pOccurSour;
	Len=EVENT_HeadData(0x32030200,p8,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]+=2;//结构体元素数
//  控制对象      OI，
		if((Len+3)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_OI;
		p8[Len+1]=OI>>8;
		p8[Len+2]=OI;
		Len+=3;
//	电控定值 long64（单位：kW，换算-4），
		if((Len+9)>LEN_DATABUFF)
		{
			return;
		}
		p8[Len]=DataType_long64;
		MR(ADDR_DATABUFF+Len+1,(u32)pFixedValue,8);
		Len+=9;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]+=2;//个数+5
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//控制对象OI
		p8[1]=0x33;
		p8[2]=0x06;
		p8[3]=0x02;
		p8[4]=6;
		p8[5]=0;//DataType_OAD;//电控定值
		p8[6]=0x33;
		p8[7]=0x06;
		p8[8]=0x02;
		p8[9]=7;
		Len=EVENT_OADData(Len,2);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x32030200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}


#if (USER/100)==9//河南
//电表接线错误信息单元∷=structure
//{
//事件记录序号  double-long-unsigned，
//  事件发生时间  date_time_s，
//  事件结束时间  date_time_s，
//  事件发生源    NULL，
//  事件上报状态  array 通道上报状态，
//一个电表接线错误结果 array电表接线错误结果
//}

//电表接线错误结果∷=structure
//{
//台区节点总数 unsigned，
//节点地址 octet-string，
//节点信息 bit-string(SIZE(8))，
//节点台区地址 octet-string
//}
//节点信息：b0为表档案，b1-2为台区状态，b3-5为相序，b6为线路异常，b7为保留
void Event_34110200(u8* pDATA)//电表接线错误事件记录;入口:pDATA=1个错误结果;使用ADDR_DATABUFF和ADDR_128KDATABUFF
{
	u32 i;
	u32 Len;
	u32 Report;
	u8* p8;

	Len=EVENT_HeadData(0x34110200,0,2);//标准事件头数据;入口:OI=事件对象,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无;返回:b0-b7上报标识0=不上报1=上报,b8-b23总长度;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	Report=Len&0xff;
	Len>>=8;
	if(Len)
	{
		p8=(u8*)ADDR_DATABUFF;
		p8[1]++;//结构体元素数
		p8[Len]=DataType_array;
		p8[Len+1]=1;
		Len+=2;
		i=Get_DL698DataLen_S(pDATA,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		MR(ADDR_DATABUFF+Len,(u32)pDATA,i);
		Len+=i;
		
		p8=(u8*)ADDR_128KDATABUFF+(96*1024);//事件中各项数据的相应OAD顺序表array OAD
		i=p8[1];
		p8[1]++;//个数+1
		i*=5;
		i+=2;
		p8+=i;
		p8[0]=0;//DataType_OAD;//发现未知电能表事件单元-6
		p8[1]=0x33;
		p8[2]=0x03;
		p8[3]=0x02;
		p8[4]=6;
		//Len=EVENT_OADData(Len);//标准事件关联OAD数据;入口:关联OAD参数已读入到ADDR_128KDATABUFF+(32*1024),Len=事件已有长度,Occur=0恢复=1产生=2无(当=2时同时表示正反有功电量没读入ADDR_128KDATABUFF+(64*1024)和ADDR_128KDATABUFF+(64*1024)+1024);返回:总长度
		EventRecord(0x34110200,(u8*)ADDR_DATABUFF,Len,2,Report);//事件记录文件;返回:写入字节数
	}
}
#endif






