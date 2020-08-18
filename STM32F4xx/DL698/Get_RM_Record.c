
//得到抄表记录
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_Record.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


#include "../DL698/DL698_JSON.h"


u32 SomeTaskID(u32 id)//找是否有相同抄表OAD或ROAD的任务;入口:任务id号;返回:0=没相同,1=有相同
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 m;
	u32 NUM_CSD;
	u32 OAD;
	u32 ROAD;
	u32 NUM_OAD;
	u8* pCSD;
	u8* p8;
	
	p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
	if(p8[0]!=DataType_structure)
	{
		return 0;//返回:0=没相同,1=有相同
	}
	id=p8[11];//方案编号0-127
	p8=(u8*)ADDR_6014_SDRAM+(id*LENper_6014);
	p8=Get_Element(p8,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
	if(p8==0)
	{
		return 0;//返回:0=没相同,1=有相同
	}
	pCSD=p8;
	
	NUM_CSD=pCSD[1];//RCSD∷=SEQUENCE OF CSD
	pCSD+=2;
	while(NUM_CSD--)
	{
		if(pCSD[1]==0)
		{//OAD
			OAD=(pCSD[2]<<24)|(pCSD[3]<<16)|(pCSD[4]<<8)|pCSD[5];
			switch(OAD)
			{
				case 0x40010200://通信地址
				case 0x202A0200://目标服务器地址
				case 0x60400200://采集启动时标
				case 0x60410200://采集成功时标
				case 0x60420200://采集存储时标
					pCSD+=6;
					continue;
			}
			for(i=0;i<NUMmax_6014;i++)
			{
				if(i==id)
				{
					continue;
				}
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//普通采集方案
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//列选择  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==0)
							{
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if((y>>8)==(OAD>>8))
								{//找到普通采集方案号
									return 1;//返回:0=没相同,1=有相同
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
					}
				}
			}
			pCSD+=6;
		}
		else
		{//ROAD
			ROAD=(pCSD[2]<<24)|(pCSD[3]<<16)|(pCSD[4]<<8)|pCSD[5];
			for(i=0;i<NUMmax_6014;i++)
			{
				if(i==id)
				{
					continue;
				}
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//普通采集方案
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//列选择  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==1)
							{//ROAD
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if(y==ROAD)
								{
									NUM_OAD=pCSD[6];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pCSD[7+(j*4)]<<24)|(pCSD[7+(j*4)+1]<<16)|(pCSD[7+(j*4)+2]<<8)|pCSD[7+(j*4)+3];
										m=p8[6];
										for(z=0;z<m;z++)
										{
											y=(p8[7+(z*4)]<<24)|(p8[7+(z*4)+1]<<16)|(p8[7+(z*4)+2]<<8)|p8[7+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												return 1;//返回:0=没相同,1=有相同
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
					}
				}
			}
			pCSD+=Get_DL698DataLen_S(pCSD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
		}			
	}
	return 0;//返回:0=没相同,1=有相同
}

u32 FileID_ChangeTaskNO(u32 FileID)//文件ID按宏定义更改任务号,任务无关的固定为0xFE
{
	u32 ProjectType;//方案类型
	u32 CollectionType;//采集类型
	u32 TaskNo;//任务号
	
	TaskNo=(FileID>>12)&0xff;//任务号
	ProjectType=SomeTaskID(TaskNo);//找是否有相同抄表OAD或ROAD的任务;入口:任务id号;返回:0=没相同,1=有相同
	if(ProjectType)
	{
		ProjectType=FileID>>28;//方案类型
		CollectionType=(FileID>>24)&0xf;//采集类型
		switch(ProjectType)//方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
		{
			case 1://1=普通采集方案
				switch(CollectionType)//采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集,4=补抄
				{
					default:
					case 0://0=采集当前数据
					#if FileID_10x==0//0=否,1=是;方案类型_采集类型_存储时标类型=10x(普通采集方案,采集当前数据)文件标识区分任务ID号
						TaskNo=0xFE;//任务号
					#endif
						break;
					//1=采集上第N次
					case 2://2=按冻结时标采集(日冻结数据)
						//日冻结
					#if FileID_12xD==0//0=否,1=是;方案类型_采集类型_存储时标类型=12x(普通采集方案,按冻结时标采集日冻结)文件标识区分任务ID号
						TaskNo=0xFE;//任务号
					#endif
						break;
					case 3://3=按时标间隔采集(曲线数据)
					#if FileID_136==0//0=否,1=是;方案类型_采集类型_存储时标类型=136(普通采集方案,按时标间隔采集曲线)文件标识区分任务ID号
						TaskNo=0xFE;//任务号
					#endif
						break;
					//4=补抄
					case 14://扩展(月冻结数据)
						//月冻结
					#if FileID_12xM==0//0=否,1=是;方案类型_采集类型_存储时标类型=12x(普通采集方案,按冻结时标采集月冻结)文件标识区分任务ID号
						TaskNo=0xFE;//任务号
					#endif
						break;
				}
				break;
			case 2://2=事件采集方案
			#if FileID_2xx==0//0=否,1=是;方案类型_采集类型_存储时标类型=2xx(事件采集方案)文件标识区分任务ID号
				TaskNo=0xFE;//任务号
			#endif
				break;
			//3=透明方案,4=上报方案,5=脚本方案
		}
		FileID&=~(0xff<<12);
		FileID|=TaskNo<<12;
	}	
	return FileID;
}

u32 Get_FileID(u32 PORTn)//UART各任务的FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
{
	u32 i;
	u32 FileID;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	FileID=UARTCtrl->TaskID;//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	if(((FileID>>24)&0xf)==1)
	{//普通采集方案
		i=FileID&0xff;//方案号
		p8=(u8*)(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
		p8=Get_Element(p8,4,1,0);//计算元素地址,使用ADDR_128KDATABUFF
		if(p8)
		{
			i=(p8[0]<<24)|(p8[1]<<16)|(p8[2]<<8)|p8[3];
			switch(i)
			{
				case 0x5B015002://分钟冻结(曲线)
					FileID&=~(0xf<<20);
					FileID|=3<<20;//3=按时标间隔采集(曲线数据)
					break;
				case 0x5B015004://日冻结
					FileID&=~(0xf<<20);
					FileID|=2<<20;//2=按冻结时标采集(日冻结数据)
					break;
				case 0x5B015006://月冻结
					FileID&=~(0xf<<20);
					FileID|=14<<20;//14=扩展(月冻结数据)
					break;
			#if (USER/100)==17
				case 0x5B015005://结算日冻结
					FileID&=~(0xf<<20);
					FileID|=15<<20;//15=扩展(月冻结数据)
					break;
			#endif
			}
		}
	}
	
//#if(USER/100)==17//江苏系
//	switch(UARTCtrl->res23)
//	{
//		case 3:
//		case 4:
//			FileID&=~(0xf<<20);
//			FileID|=3<<20;//3=按时标间隔采集(曲线数据)
//			FileID>>=8;
//			FileID<<=12;
//		  FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff22);
//			FileID<<=12;
//			break;
//		case 1:
//		case 6:
//			FileID&=~(0xf<<20);
//			FileID|=2<<20;//2=按冻结时标采集(日冻结数据)
//			FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff16);
//			FileID<<=12;
//			break;
//		case 2:
//		case 7:
//			FileID&=~(0xf<<20);
//			FileID|=14<<20;//14=扩展(月冻结数据)
//			FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff18);
//			FileID<<=12;
//			break;
//		case 8:
//		case 9:
//			FileID&=~(0xf<<20);
//			FileID|=15<<20;//15=扩展(其他分钟冻结)
//			FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff19);
//			FileID<<=12;
//			break;
//		default:
//			break;	
//	}
//#else
	FileID>>=8;
	FileID<<=12;
	FileID=FileID_ChangeTaskNO(FileID);//文件ID按宏定义更改任务号,任务无关的固定为0xFE
//#endif
	FileID|=0xf<<20;//存储时标类型不用
	return FileID;
}

u32 IDtoFileID(u32 id)//入口:任务号id,返回:文件存储FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类;无效id返回=0
{
	u32 i;
	u32 FileID;
	u8* p8;
	
	p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
	if(p8[0]!=DataType_structure)
	{
		return 0;//无效id
	}
	i=p8[9];//方案类型(1-5)
	FileID=i<<28;
	switch(i)
	{
		case 1://1=普通采集方案,
			i=p8[11];//方案编号0-127
			p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
			if(p8[0]!=DataType_structure)
			{
				return 0;//无效id
			}
			//采集类型
			FileID|=(p8[10]&0xf)<<24;
			p8=Get_Element(p8,4,1,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8==0)
			{
				return 0;//无效id
			}
			i=(p8[0]<<24)|(p8[1]<<16)|(p8[2]<<8)|p8[3];
			switch(i)
			{
				case 0x5B015002://分钟冻结(曲线)
					FileID&=~(0xf<<24);
					FileID|=3<<24;//3=按时标间隔采集(曲线数据)
					break;
				case 0x5B015004://日冻结
					FileID&=~(0xf<<24);
					FileID|=2<<24;//2=按冻结时标采集(日冻结数据)
					break;
				case 0x5B015006://月冻结
					FileID&=~(0xf<<24);
					FileID|=14<<24;//14=扩展(月冻结数据)
					break;
			#if (USER/100)==17
				case 0x5B015005://结算日冻结
					FileID&=~(0xf<<24);
					FileID|=15<<24;//15=扩展(结算日冻结)
					break;
			#endif
			}
			//存储时标类型
			FileID|=0xf<<20;//存储时标类型不用
			//
			FileID|=id<<12;//任务号
			FileID|=0xf<<8;//端口号
			//FileID|=0;//文件分类
			break;
		case 2://2=事件采集方案,
			i=p8[11];//方案编号0-127
			p8=(u8*)ADDR_6016_SDRAM+(i*LENper_6016);
			if(p8[0]!=DataType_structure)
			{
				return 0;//无效id
			}
			FileID|=0xf<<24;//采集类型
			FileID|=0xf<<20;//存储时标
			FileID|=id<<12;//任务号
			FileID|=0xf<<8;//端口号
			//FileID|=0;//文件分类
			break;
		case 3://3=透明方案,
		case 4://4=上报方案,
		case 5://5=脚本方案
		default:
			FileID|=0xf<<24;//采集类型
			FileID|=0xf<<20;//存储时标
			FileID|=id<<12;//任务号
			FileID|=0xf<<8;//端口号
			//FileID|=0;//文件分类
			break;
	}
	FileID=FileID_ChangeTaskNO(FileID);//文件ID按宏定义更改任务号,任务无关的固定为0xFE
	return FileID;
}

u32 IDtoTaskID(u32 id)//入口:任务号id;出口：TaskID
{
	u32 x;
	u32 TaskID;//当前采集任务ID;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	u8* p8s;
	u8* p8d;
	
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return 0;
	}
	p8s=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
	TaskID=p8s[33];//优先级
	if(TaskID>15)
	{
		TaskID=15;
	}
	TaskID<<=4;
	x=p8s[9];//方案类型(1-5)
	if(x>=4)
	{
		return 0;
	}
	x&=0xf;
	TaskID|=x;
	switch(x)//方案类型
	{
		case 1://普通采集方案 （1）
			//普通方案时采集类型
			TaskID<<=4;
			x=p8s[11];//方案编号0-127
			p8d=(u8*)ADDR_6014_SDRAM+(x*LENper_6014);
			TaskID|=p8d[10]&0xf;
			//普通方案时存储时标
			TaskID<<=4;
			p8d=Get_Element(p8d,6,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8d)
			{
				TaskID|=p8d[1]&0xf;
			}
			break;
		case 2://事件采集方案 （2）
			//事件采集方案时采集类型
			TaskID<<=4;
			x=p8s[11];//方案编号0-127
			p8d=(u8*)ADDR_6016_SDRAM+(x*LENper_6016);
			x=p8d[7]&0xf;
			TaskID|=x;
			//事件采集方案时存储时标=0
			TaskID<<=4;
			break;
		//csae 3://透明方案     （3）
		//case 4://上报方案     （4）
		//case 5://脚本方案     （5）
		default:
			//采集类型=0
			//存储时标=0
			TaskID<<=8;
			break;
	}
	TaskID<<=8;
	TaskID|=id;//任务号
	TaskID<<=8;
	x=p8s[11];//方案编号0-127
	TaskID|=x;//方案编号
		
	return TaskID;
}


u32 RCSDtoID(u8* pRCSD)//记录列选择RCSD相对应的任务号,返回:数据在采集任务的任务号,>=RMTASKmax表示无效任务号
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 m;
	u32 offset;
	u32 NUM_CSD;
	u32 OAD;
	u32 ROAD;
	u32 NUM_OAD;
	u8* p8;
	
//X RecordData∷=structure
//{
//X  主对象属性描述符      OAD，
//	记录型对象属性描述符  RCSD，
//	行选择                RSD
//}
//X 02 03
//X 51 60 12 03 00 主对象属性描述符      OAD
//60 01 RCSD∷=SEQUENCE OF CSD
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 
//5a 00 行选择                RSD
	NUM_CSD=pRCSD[1];//RCSD∷=SEQUENCE OF CSD
	pRCSD+=2;
//先普通方案列表
	offset=0;
	while(NUM_CSD--)
	{
		if(pRCSD[offset]==0)
		{//OAD
			OAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			switch(OAD)
			{
				case 0x40010200://通信地址
				case 0x202A0200://目标服务器地址
				case 0x60400200://采集启动时标
				case 0x60410200://采集成功时标
				case 0x60420200://采集存储时标
					offset+=5;
					continue;
			}
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//普通采集方案
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//记录列选择  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==0)
							{
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if((y>>8)==(OAD>>8))
								{//找到普通采集方案号
									
									//普通方案列表转为任务号
									for(j=0;j<NUMmax_6012_2;j++)
									{
										p8=(u8*)ADDR_6012_2_SDRAM+(j*LENper_6012_2);//任务配置表
										if(p8[0]==DataType_structure)
										{
											if(p8[9]==1)
											{//普通采集方案
												if(p8[11]==i)
												{//方案编号
													return j;//任务配置号
												}
											}
										}
									}
									return RMTASKmax;
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
					}
				}
			}
			offset+=5;
		}
		else
		{//ROAD
			ROAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//普通采集方案
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//记录列选择  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==1)
							{//ROAD
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if(y==ROAD)
								{
									NUM_OAD=pRCSD[offset+5];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pRCSD[offset+6+(j*4)]<<24)|(pRCSD[offset+6+(j*4)+1]<<16)|(pRCSD[offset+6+(j*4)+2]<<8)|pRCSD[offset+6+(j*4)+3];
										m=p8[6];
										for(z=0;z<m;z++)
										{
											y=(p8[7+(z*4)]<<24)|(p8[7+(z*4)+1]<<16)|(p8[7+(z*4)+2]<<8)|p8[7+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												
												//普通方案列表转为任务号
												for(j=0;j<NUMmax_6012_2;j++)
												{
													p8=(u8*)ADDR_6012_2_SDRAM+(j*LENper_6012_2);//任务配置表
													if(p8[0]==DataType_structure)
													{
														if(p8[9]==1)
														{//普通采集方案
															if(p8[11]==i)
															{//方案编号
																return j;//任务配置号
															}
														}
													}
												}
												return RMTASKmax;
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
					}
				}
			}
			i=pRCSD[offset+5];
			i*=4;
			i+=6;
			offset+=6;
		}			
	}
	return RMTASKmax;
}

u32 RCSDtoFileID(u8* pRCSD)//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff;RCSD中只有通信地址等非抄表类数据时返回0xffffffff
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 m;
	u32 k;
	u32 offset;
	u32 NUM_CSD;
	u32 OAD;
	u32 ROAD;
	u32 NUM_OAD;
	u8* p8;
	u32 ProjectID;//采集方案号
	u32 NUM_ProjectID;//至少有的相同采集方案个数
	u32 Data_Com;//有通信地址等公共数据
	ProjectID=0;//采集方案号
	NUM_ProjectID=0;//至少有的相同采集方案个数
	Data_Com=0;//RCSD中有通信地址等非抄表类数据
//X RecordData∷=structure
//{
//X  主对象属性描述符      OAD，
//	记录型对象属性描述符  RCSD，
//	行选择                RSD
//}
//X 02 03
//X 51 60 12 03 00 主对象属性描述符      OAD
//60 01 RCSD∷=SEQUENCE OF CSD
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 
//5a 00 行选择                RSD
	NUM_CSD=pRCSD[1];//RCSD∷=SEQUENCE OF CSD
	pRCSD+=2;
//先普通方案列表
	offset=0;
	k=0;
	while(NUM_CSD--)
	{
		if(pRCSD[offset]==0)
		{//OAD
			OAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			switch(OAD)
			{
				case 0x40010200://通信地址
				case 0x202A0200://目标服务器地址
				case 0x60400200://采集启动时标
				case 0x60410200://采集成功时标
				case 0x60420200://采集存储时标
					Data_Com=0xffffffff;//RCSD中有通信地址等非抄表类数据
					offset+=5;
					continue;
			}
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//普通采集方案
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//记录列选择  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==0)
							{
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if((y>>8)==(OAD>>8))
								{//找到普通采集方案号
									NUM_ProjectID++;//至少有的相同采集方案个数
									if(NUM_ProjectID>=2)//至少有的相同采集方案个数
									{
										break;
									}
									ProjectID=i;//采集方案号
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
					}
				}
			}
			offset+=5;
		}
		else
		{//ROAD
			ROAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			if(Terminal_Router->MinuteCollect==1)//支持分钟级采集
			//if(1)//支持分钟级采集
			{
				if(ROAD==0x50040200)
				{
					return 0x13f16000;
				}
				if(ROAD==0x50060200)
				{
					return 0x13f18000;
				}
			}
			k=0;
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//普通采集方案
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//记录列选择  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==1)
							{//ROAD
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if(y==ROAD)
								{
									NUM_OAD=pRCSD[offset+5];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pRCSD[offset+6+(j*4)]<<24)|(pRCSD[offset+6+(j*4)+1]<<16)|(pRCSD[offset+6+(j*4)+2]<<8)|pRCSD[offset+6+(j*4)+3];
										m=p8[6];
										for(z=0;z<m;z++)
										{
											y=(p8[7+(z*4)]<<24)|(p8[7+(z*4)+1]<<16)|(p8[7+(z*4)+2]<<8)|p8[7+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												NUM_ProjectID++;//至少有的相同采集方案个数
												if(NUM_ProjectID>=2)//至少有的相同采集方案个数
												{
													break;
												}
												ProjectID=i;//采集方案号
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
					}
				}
			}
		#if(USER/100)==17//江苏系
			if(NUM_ProjectID==0)
			{
				for(i=0;i<NUMmax_6016;i++)
				{
				p8=(u8*)ADDR_6016_SDRAM+(i*LENper_6016);//事件采集方案
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,2,2,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8)
					{//记录列选择  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[0]==0x52)
							{//ROAD
								y=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
								if(y==ROAD)
								{
									NUM_OAD=pRCSD[offset+5];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pRCSD[offset+6+(j*4)]<<24)|(pRCSD[offset+6+(j*4)+1]<<16)|(pRCSD[offset+6+(j*4)+2]<<8)|pRCSD[offset+6+(j*4)+3];
										m=p8[5];
										for(z=0;z<m;z++)
										{
											y=(p8[6+(z*4)]<<24)|(p8[6+(z*4)+1]<<16)|(p8[6+(z*4)+2]<<8)|p8[6+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												NUM_ProjectID++;//至少有的相同采集方案个数
												if(NUM_ProjectID>=2)//至少有的相同采集方案个数
												{
													break;
												}
												ProjectID=i;//采集方案号
												k=1;
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						}
					}
				}
			}
			}
			#endif
			
//		#if(USER/100)==17//江苏系
//			if(ROAD==0x30110200)
//				return 0x23000000;
//		#endif
			i=pRCSD[offset+5];
			i*=4;
			i+=6;
			offset+=i;
			//offset+=6;
		}			
	}
	if(NUM_ProjectID==0)//至少有的相同采集方案个数
	{
		if(Data_Com)//RCSD中有通信地址等非抄表类数据
		{
			return Data_Com;
		}
		return 0;//返回:0表示没找到无效FileID
	}
	//普通方案列表转为任务号
	for(j=0;j<NUMmax_6012_2;j++)
	{
		p8=(u8*)ADDR_6012_2_SDRAM+(j*LENper_6012_2);//任务配置表
		if(p8[0]==DataType_structure)
		{
			if((k==0)&&(p8[9]==1))
			{//普通采集方案
				if(p8[11]==ProjectID)
				{//方案编号
					i=IDtoFileID(j);//入口:任务号id,返回:文件存储FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类;无效id返回=0
					if(NUM_ProjectID>=2)//至少有的相同采集方案个数
					{
						i|=0xff<<12;
					}
					return i;//FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
				}
			}
			else if((k==1)&&(p8[9]==2))
			{
				if(p8[11]==ProjectID)
				{//方案编号
					i=IDtoFileID(j);//入口:任务号id,返回:文件存储FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类;无效id返回=0
					if(NUM_ProjectID>=2)//至少有的相同采集方案个数
					{
						i|=0xff<<12;
					}
					return i;//FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
				}
			}
		}
	}
	return 0;//返回:0表示没找到无效FileID
}


u32 Get_RecordFileName(u32 FileID,u32 FileCount,u8* pTime)//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
{
	u32 i;
	msFILE_TypeDef *file;
	u32 ProjectType;//方案类型
	u32 CollectionType;//采集类型
	u32 StorageType;//存储时标类型
	u32 TaskNo;//任务号
	u32 SearchCount;
	
	ProjectType=FileID>>28;//方案类型
	CollectionType=(FileID>>24)&0xf;//采集类型
	StorageType=(FileID>>20)&0xf;//存储时标类型
	TaskNo=(FileID>>12)&0xff;//任务号

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
			//b7-b0文件分类
			if((FileID&0xff)!=0)
			{
				file++;
				continue;
			}
			//b31-b28方案类型
			if(ProjectType!=0xf)//方案类型
			{
				if(ProjectType!=(FileID>>28))
				{
					file++;
					continue;
				}
			}
			//b27-b24采集类型
			if(CollectionType!=0xf)//采集类型
			{
				if(CollectionType!=((FileID>>24)&0xf))
				{
					file++;
					continue;
				}
			}
			//b23-b20存储时标类型
			if(StorageType!=0xf)//存储时标类型
			{
				if(StorageType!=((FileID>>20)&0xf))
				{
					file++;
					continue;
				}
			}
			//b19-b12任务号
			if(TaskNo!=0xff)//任务号
			{
				if(TaskNo!=((FileID>>12)&0xff))
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
		//file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//文件搜索计数
	return 0xffffffff;
}

u32 Get_RecordFileName_JS(u32 FileID,u32 FileCount,u8* pTime)//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
{
	u32 i;
	msFILE_TypeDef *file;
	u32 ProjectType;//方案类型
	u32 CollectionType;//采集类型
	u32 StorageType;//存储时标类型
	u32 TaskNo;//任务号
	u32 SearchCount;
	
	ProjectType=FileID>>28;//方案类型
	CollectionType=(FileID>>24)&0xf;//采集类型
	StorageType=(FileID>>20)&0xf;//存储时标类型
	TaskNo=(FileID>>12)&0xff;//任务号

	SearchCount=Comm_Ram->msFILESearchCount;
	if(SearchCount<recordFILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(SearchCount*sizeof(msFILE_TypeDef)));
	}
	else
	{
		return 0xffffffff;
		//file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((SearchCount-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	
	//for(;SearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);SearchCount++)
	for(;SearchCount<(recordFILENAMEmax_FLASH);SearchCount++)
	{
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			//b7-b0文件分类
			if((FileID&0xff)!=0)
			{
				file++;
				continue;
			}
			//b31-b28方案类型
			if(ProjectType!=0xf)//方案类型
			{
				if(ProjectType!=(FileID>>28))
				{
					file++;
					continue;
				}
			}
			//b27-b24采集类型
			if(CollectionType!=0xf)//采集类型
			{
				if(CollectionType!=((FileID>>24)&0xf))
				{
					file++;
					continue;
				}
			}
			//b23-b20存储时标类型
			if(StorageType!=0xf)//存储时标类型
			{
				if(StorageType!=((FileID>>20)&0xf))
				{
					file++;
					continue;
				}
			}
			//b19-b12任务号
			if(TaskNo!=0xff)//任务号
			{
				if(TaskNo!=((FileID>>12)&0xff))
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
			//if(SearchCount>=recordFILENAMEmax_FLASH)
			//{
				//SearchCount-=recordFILENAMEmax_FLASH;
				//SearchCount+=FILENAMEmax_FLASH;
			//}
			return SearchCount;
		}
		file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//文件搜索计数
	return 0xffffffff;
}

u32 Get_LastRecordFileDataHasID(u32 ID,u16* pAddr)//
{
	u32 i;
	u32 n;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//抄表记录文件头文件
	u32 FileCount;
	u32 offset;
	
	FileID=IDtoFileID(ID);//入口:任务号id,返回:文件存储FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类;无效id返回=0
	FileID&=0xfffff0ff;
	ms=(ms_Type *)Get_ms();
	//先找文件计数0再找文件计数1
	for(FileCount=0;FileCount<2;FileCount++)
	{
		Comm_Ram->msFILESearchCount=0;
		while(1)
		{
			filename=Get_RecordFileName(FileID,FileCount,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
			if(filename==0xffffffff)
			{//没找到
				break;
			}
			if(filename<recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>LEN_128KDATABUFF)
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
			for(i=0;i<n;i++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							break;
						}
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			if(i<n)
			{//找到
				i=RecordFileHead->LEN_DATA;
				if(i>LEN_128KDATABUFF)
				{
					i=LEN_128KDATABUFF;
				}
				ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF,i);
				return i;//返回:读入到ADDR_128KDATABUFF数据长度，0=没找到
			}
		}
	}
	return 0;//返回:0=没找到,1=找到并读入到ADDR_128KDATABUFF
}
	
u32 Get_LastRecordFileData(u32 ID,u16* pAddr)//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
{
	u32 i;
	u32 n;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//抄表记录文件头文件
	u32 FileCount;
	u32 offset;
	
	FileID=IDtoFileID(ID);//入口:任务号id,返回:文件存储FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类;无效id返回=0
	FileID|=0xff<<12;//任务号无关
	ms=(ms_Type *)Get_ms();
	//先找文件计数0再找文件计数1
	for(FileCount=0;FileCount<2;FileCount++)
	{
		Comm_Ram->msFILESearchCount=0;
		while(1)
		{
			filename=Get_RecordFileName(FileID,FileCount,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
			if(filename==0xffffffff)
			{//没找到
				break;
			}
			if(filename<recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>LEN_128KDATABUFF)
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
			for(i=0;i<n;i++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							break;
						}
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			if(i<n)
			{//找到
				i=RecordFileHead->LEN_DATA;
				if(i>LEN_128KDATABUFF)
				{
					i=LEN_128KDATABUFF;
				}
				ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF,i);
				return i;//返回:读入到ADDR_128KDATABUFF数据长度，0=没找到
			}
		}
	}
	return 0;//返回:0=没找到,1=找到并读入到ADDR_128KDATABUFF
}

/*
u32 Get_RecordFileData_RCSD(u8 *pRCSD,u8 *pAddr,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
{
	u32 i;
	u32 x;
	u32 Offset;
	u32 Len;
	u32 Nrcsd;
	u32 OADn;
	u32 Nroad;
	u32 Mcsd;
	u32 OADm;
	u32 Mroad;
	u32 OADcount;
	u8 *pData;
	u32 fDATA;//有列选择OAD数据标志
	
	fDATA=0;//有列选择OAD数据标志
	//计算记录数据开始指针
	Offset=21;
	Mcsd=pRecord[Offset+1];
	Offset+=2;
	for(i=0;i<Mcsd;i++)
	{
		if(pRecord[Offset+1]==0)
		{//OAD
			
			Offset+=6;
		}
		else
		{//ROAD
			x=pRecord[Offset+6];//关联OAD个数
			
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=(u8*)pRecord+Offset;
	//
	Nrcsd=pRCSD[0];
	if(Nrcsd==0)
	{
		//RCSD∷=SEQUENCE OF CSD 当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”
		Nrcsd=Mcsd;
		Offset-=21+2;
		pRCSD=(u8*)ADDR_1kDATABUFF;
		pRCSD[0]=Nrcsd;//RCSD，SEQUENCE OF个数
		MR((u32)pRCSD+1,(u32)pRecord+21+2,Offset);//接收缓冲在前,5*m最大=5*256极限也只能超出接收缓冲封皮入发送区但不会超出发送区
		//去数据存贮时的数据前类型
		pRCSD++;
		Len=0;
		Offset=0;
		for(i=0;i<Mcsd;i++)
		{
			if(pRCSD[Offset+1]==0)
			{//OAD
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,5);
				Len+=5;
				Offset+=6;
			}
			else
			{//ROAD
				x=pRecord[Offset+6];//关联OAD个数
				x*=4;
				x+=7;
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,x-1);
				Len+=x-1;
				Offset+=x;
			}
		}
	}
	Len=0;
	pRCSD++;
	while(Nrcsd--)
	{
//电能表数据块区,长度按实际写入数
//0 7byte 采集启动时标  date_time_s(无数据类型)
//7 7byte 采集成功时标  date_time_s(无数据类型)
//14 7byte 采集存储时标  date_time_s(无数据类型)
//21  采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型)(ROAD时不写入总关联的OAD个数数组!!!)
		OADn=(pRCSD[1]<<24)+(pRCSD[2]<<16)+(pRCSD[3]<<8)+pRCSD[4];
		switch(OADn)
		{
			case 0x40010200://通信地址
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_octet_string;
				pout[Len+1]=6;
				MR((u32)pout+Len+2,(u32)pAddr,6);
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
			case 0x202A0200://目标服务器地址
				if(LEN_DATAmax<9)
				{
					return 0;
				}
				pout[Len]=DataType_TSA;
				pout[Len+1]=7;
				pout[Len+2]=5;
				MR((u32)pout+Len+3,(u32)pAddr,6);
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
			case 0x60400200://采集启动时标
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord,7);
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
			case 0x60410200://采集成功时标
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+7,7);
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
			case 0x60420200://采集存储时标
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+14,7);
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
		//找相同OAD在记录中的OAD计数
		Offset=21;
		Mcsd=pRecord[Offset+1];
		Offset+=2;
		OADcount=0;//相同OAD在记录中的OAD计数
		for(i=0;i<Mcsd;i++)
		{
			OADm=(pRecord[Offset+2]<<24)+(pRecord[Offset+3]<<16)+(pRecord[Offset+4]<<8)+pRecord[Offset+5];
			if(OADn==OADm)
			{
				break;
			}
			if(pRecord[Offset+1]==0)
			{//OAD
				OADcount++;
				Offset+=6;
			}
			else
			{//ROAD
				x=pRecord[Offset+6];//关联OAD个数
				OADcount+=x;
				x*=4;
				x+=7;
				Offset+=x;
			}
		}
		if(i>=Mcsd)
		{//没找到抄表任务的CSD,用0=NULL数据
			if(pRCSD[0]==0)
			{//OAD
				pRCSD+=5;
				if(LEN_DATAmax<1)
				{
					return 0;
				}
				pout[Len]=0;//NULL
				Len+=1;
				LEN_DATAmax-=1;
			}
			else
			{//ROAD
				Nroad=pRCSD[5];//SEQUENCE OF个数
				pRCSD+=6+(Nroad*4);
				if(LEN_DATAmax<(2+Nroad))
				{
					return 0;
				}
				pout[Len]=DataType_array;
				pout[Len+1]=Nroad;
				for(i=0;i<Nroad;i++)
				{
					pout[Len+2+i]=0;//NULL
				}
				Len+=2+Nroad;
				LEN_DATAmax-=2+Nroad;
			}
			continue;
		}
		if(pRCSD[0]==0)
		{//OAD
			pRCSD+=5;
			x=OADcount;
			OADn=0;//数据偏移
			while(x--)
			{
				OADm=Get_DL698DataLen_S(pData+OADn,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				OADn+=OADm;
			}
			x=Get_DL698DataLen_S(pData+OADn,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if(LEN_DATAmax<x)
			{
				return 0;
			}
			MR((u32)pout+Len,(u32)pData+OADn,x);
			Len+=x;
			LEN_DATAmax-=x;
			fDATA=1;//有列选择OAD数据标志
		}
		else
		{//ROAD
			Nroad=pRCSD[5];//SEQUENCE OF个数
			pRCSD+=6;
			if(pRecord[Offset+1]==0)
			{//记录数据非ROAD
				return 0;
			}
			if(LEN_DATAmax<2)
			{
				return 0;
			}
			pout[Len]=DataType_array;
			pout[Len+1]=Nroad;
			Len+=2;
			LEN_DATAmax-=2;
			Mroad=pRecord[Offset+6];//SEQUENCE OF个数
			Offset+=7;
			for(i=0;i<Nroad;i++)
			{
				OADn=(pRCSD[0]<<24)+(pRCSD[1]<<16)+(pRCSD[2]<<8)+pRCSD[3];
				for(x=0;x<Mroad;x++)
				{
					OADm=(pRecord[Offset+(4*x)+0]<<24)+(pRecord[Offset+(4*x)+1]<<16)+(pRecord[Offset+(4*x)+2]<<8)+pRecord[Offset+(4*x)+3];
					if(OADn==OADm)
					{
						x+=OADcount;
						OADn=0;//数据偏移
						while(x--)
						{
							OADm=Get_DL698DataLen_S(pData+OADn,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
							OADn+=OADm;
						}
						x=Get_DL698DataLen_S(pData+OADn,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						if(LEN_DATAmax<x)
						{
							return 0;
						}
						MR((u32)pout+Len,(u32)pData+OADn,x);
						Len+=x;
						LEN_DATAmax-=x;
						x=0;
						fDATA=1;//有列选择OAD数据标志
						break;
					}
				}
				if(x>=Mroad)
				{//没找到,用0=NULL数据
					if(LEN_DATAmax<1)
					{
						return 0;
					}
					pout[Len]=0;//NULL
					Len+=1;
					LEN_DATAmax-=1;
				}
				pRCSD+=4;
			}
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
*/

u32 Get_RecordFileData_RCSD(u8 *pRCSD,u8 *pAddr,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
{
	u32 i;
	u32 x;
	u32 Offset;
	u32 Len;
	u32 Nrcsd;
	u32 Nroad;
	u32 OADrcsd;
	u32 OADroad;
	u32 Mcsd;
	u32 Mroad;
	u32 OAD;
	u32 OADcount;
	u32 OADoffset;
	u8 *pData;
	u32 fDATA;//有列选择OAD数据标志
	u8* p8;
	
	fDATA=0;//有列选择OAD数据标志
	//计算记录数据开始指针
	Offset=21;
	Mcsd=pRecord[Offset+1];
	Offset+=2;
	for(i=0;i<Mcsd;i++)
	{
		if(pRecord[Offset+1]==0)
		{//OAD
			Offset+=6;
		}
		else
		{//ROAD
			x=pRecord[Offset+6];//关联OAD个数
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=(u8*)pRecord+Offset;
	//
	Nrcsd=pRCSD[0];//RCSD∷=SEQUENCE OF CSD 当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”
	if(Nrcsd==0)
	{
		Nrcsd=Mcsd;
		Offset-=21+2;
		pRCSD=(u8*)ADDR_1kDATABUFF;
		pRCSD[0]=Nrcsd;//RCSD，SEQUENCE OF个数
		if(Offset>(LEN_1kDATABUFF-1))
		{
			Offset=(LEN_1kDATABUFF-1);
		}
		MR((u32)pRCSD+1,(u32)pRecord+21+2,Offset);//接收缓冲在前,5*m最大=5*256极限也只能超出接收缓冲封皮入发送区但不会超出发送区
		//去数据存贮时的数据前类型
		pRCSD++;
		Len=0;
		Offset=0;
		for(i=0;i<Mcsd;i++)
		{
			if(pRCSD[Offset+1]==0)
			{//OAD
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,5);
				Len+=5;
				Offset+=6;
			}
			else
			{//ROAD
				x=pRecord[Offset+6];//关联OAD个数
				x*=4;
				x+=7;
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,x-1);
				Len+=x-1;
				Offset+=x;
			}
		}
	}
	Len=0;
	pRCSD++;
//电能表数据块区,长度按实际写入数
//0 7byte 采集启动时标  date_time_s(无数据类型)
//7 7byte 采集成功时标  date_time_s(无数据类型)
//14 7byte 采集存储时标  date_time_s(无数据类型)
//21  采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型)(ROAD时不写入总关联的OAD个数数组!!!)
	while(Nrcsd--)
	{
		OADrcsd=(pRCSD[1]<<24)+(pRCSD[2]<<16)+(pRCSD[3]<<8)+pRCSD[4];
		switch(OADrcsd)
		{
			case 0x40010200://通信地址
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_octet_string;
				pout[Len+1]=6;
				MR((u32)pout+Len+2,(u32)pAddr,6);
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
			case 0x202A0200://目标服务器地址
				if(LEN_DATAmax<9)
				{
					return 0;
				}
				pout[Len]=DataType_TSA;
				pout[Len+1]=7;
				pout[Len+2]=5;
				MR((u32)pout+Len+3,(u32)pAddr,6);
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
			case 0x60400200://采集启动时标
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord,7);
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
			case 0x60410200://采集成功时标
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+7,7);
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
			case 0x60420200://采集存储时标
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+14,7);
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
		//在记录中找相同OAD数据
		if(pRCSD[0]==0)
		{//OAD
			Offset=21;
			Mcsd=pRecord[Offset+1];
			Offset+=2;
			OADcount=0;//相同OAD在记录中的OAD计数
			for(i=0;i<Mcsd;i++)
			{
				OAD=(pRecord[Offset+2]<<24)+(pRecord[Offset+3]<<16)+(pRecord[Offset+4]<<8)+pRecord[Offset+5];
				if((OADrcsd>>8)==(OAD>>8))
				{
					pRCSD+=5;
					x=OADcount;
					OADoffset=0;//数据偏移
					while(x--)
					{
						OADoffset+=Get_DL698DataLen_S(pData+OADoffset,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					}
					p8=pData+OADoffset;
					if(((OAD&0xff)==0)&&((OADrcsd&0xff)!=0))
					{//存贮时全部元素,取其中的元素
						p8=Get_Element(p8,OADrcsd&0xff,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
						if(p8==0)
						{//没找到,用0=NULL数据
							if(LEN_DATAmax<1)
							{
								return 0;
							}
							pout[Len]=0;//NULL
							Len+=1;
							LEN_DATAmax-=1;
							break;
						}
					}
					else
					{
						if(OADrcsd!=OAD)
						{
							continue;
						}
					}
					x=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					if(LEN_DATAmax<x)
					{
						return 0;
					}
					MR((u32)pout+Len,(u32)p8,x);
					Len+=x;
					LEN_DATAmax-=x;
					fDATA=1;//有列选择OAD数据标志
					break;
				}
				if(pRecord[Offset+1]==0)
				{//OAD
					OADcount++;
					Offset+=6;
				}
				else
				{//ROAD
					x=pRecord[Offset+6];//关联OAD个数
					OADcount+=x;
					x*=4;
					x+=7;
					Offset+=x;
				}
			}
			if(i>=Mcsd)
			{//没找到抄表任务的CSD,用0=NULL数据
				pRCSD+=5;
				if(LEN_DATAmax<1)
				{
					return 0;
				}
				pout[Len]=0;//NULL
				Len+=1;
				LEN_DATAmax-=1;
			}
		}
		else
		{//ROAD
		
			Nroad=pRCSD[5];//SEQUENCE OF个数
			pRCSD+=6;
			if(LEN_DATAmax<2)
			{
				return 0;
			}
			pout[Len]=DataType_array;
			pout[Len+1]=Nroad;
			Len+=2;
			LEN_DATAmax-=2;
			while(Nroad--)
			{
				OADroad=(pRCSD[0]<<24)+(pRCSD[1]<<16)+(pRCSD[2]<<8)+pRCSD[3];
				pRCSD+=4;
				Offset=21;
				Mcsd=pRecord[Offset+1];
				Offset+=2;
				OADcount=0;//相同OAD在记录中的OAD计数
				for(i=0;i<Mcsd;i++)
				{
					if(pRecord[Offset+1]==0)
					{//OAD
						OADcount++;
						Offset+=6;
						continue;
					}
					else
					{//ROAD
						OAD=(pRecord[Offset+2]<<24)+(pRecord[Offset+3]<<16)+(pRecord[Offset+4]<<8)+pRecord[Offset+5];
						if(OADrcsd!=OAD)
						{
							x=pRecord[Offset+6];//关联OAD个数
							OADcount+=x;
							x*=4;
							x+=7;
							Offset+=x;
							continue;
						}
						Mroad=pRecord[Offset+6];//关联OAD个数
						Offset+=7;
						for(x=0;x<Mroad;x++)
						{
							OAD=(pRecord[Offset+0]<<24)+(pRecord[Offset+1]<<16)+(pRecord[Offset+2]<<8)+pRecord[Offset+3];
							if((OADroad>>8)==(OAD>>8))
							{
								if(((OAD&0xff)==0)&&((OADroad&0xff)!=0))
								{//存贮时全部元素,取其中的元素
									
								}
								else
								{
									if(OADroad!=OAD)
									{
										OADcount++;
										Offset+=4;
										continue;
									}
								}
								break;
							}
							OADcount++;
							Offset+=4;
						}
						if(x<Mroad)
						{//找到
							x=OADcount;
							OADoffset=0;//数据偏移
							while(x--)
							{
								OADoffset+=Get_DL698DataLen_S(pData+OADoffset,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
							}
							p8=pData+OADoffset;
							if(((OAD&0xff)==0)&&((OADroad&0xff)!=0))
							{//存贮时全部元素,取其中的元素
								p8=Get_Element(p8,OADroad&0xff,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
								if(p8==0)
								{//没找到,用0=NULL数据
									if(LEN_DATAmax<1)
									{
										return 0;
									}
									pout[Len]=0;//NULL
									Len+=1;
									LEN_DATAmax-=1;
									break;
								}
							}
							x=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
							if(LEN_DATAmax<x)
							{
								return 0;
							}
							MR((u32)pout+Len,(u32)p8,x);
							Len+=x;
							LEN_DATAmax-=x;
							fDATA=1;//有列选择OAD数据标志
							break;
						}
					}
					
				}
				if(i>=Mcsd)
				{//没找到抄表任务的CSD,用0=NULL数据
					if(LEN_DATAmax<1)
					{
						return 0;
					}
					pout[Len]=0;//NULL
					Len+=1;
					LEN_DATAmax-=1;
				}
			}
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





