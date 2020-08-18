
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET_EVENT.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/EVENT_Record.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../MS/msFILE.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"





u32 GET_EVENT_OAD(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 DataAddr;
	u32 Len;
	u32 Array;
	DL698_EVENTLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	u8 *p8;
	u32 filename;
	msFILE_TypeDef *file;
	
	
	Len=0;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	pLib=Get_DL698_EVENT_pLib(OAD);//得到EVENT_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//对象不存在           （6）
		return 0;
	}
	if(OAD&0xff00)
	{//单个属性
		if((OAD&0xff00)==0x0100)
		{//读逻辑名
			if(LENmax_TxSPACE<4)
			{//分帧
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_octet_string;//long-unsigned
			p8tx[1]=2;
			p8tx[2]=OAD>>24;
			p8tx[3]=OAD>>16;
			return 4;
		}
		if((OAD&0xff)==0)
		{//全部元素
			if(pLib->IC==7)
			{
				switch((OAD>>8)&0xff)
				{
					case 2://2．事件记录表	（dyn.）
#if(USER/100)==17//江苏系
						n=0;
						p8tx[0]=DataType_array;
						p8tx[1]=0;
						Len=2;
					 	LENmax_TxSPACE-=2;
						Array=0;
					 	i=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
					 	while(1)
						{
							if(i>=NUMmax_Event)
							{
							break;
							}
							if(LENmax_TxSPACE<400)
							{
								if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								{
									UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								UARTCtrl->NEXTOADSubNum=i;//DL698分帧传输OAD内子计数
								p8tx[1]=Array;//填入配置组数
								UARTCtrl->DAR=0;
								return Len;
							}
							OAD&=0xffffff00;
							OAD|=(i+1);
							n=GET_EventRecord(PORTn,OAD,p8tx+Len,LENmax_TxSPACE,0);//得到事件记录;入口:ALL=0只事件无OAD列表,ALL=1有OAD列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
							if(n!=0)
							{
								Len+=n;
								LENmax_TxSPACE-=n;
								Array++;//填入配置组数
								i++;
							}
							else
							{
								p8tx[1]=Array;
								UARTCtrl->DAR=0;
								return Len;
							}
						}
#else
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
#endif				
					case 3://3．关联对象属性表	（static）
						DataAddr=pLib->Addr_EventOAD+4;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							Len>>=8;
						}
						break;
					case 4://4．当前记录数	（dyn.）
						Len=0;
						OAD&=0xffff0000;
						OAD|=0x0202;
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
						for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
						{
							if(file->BLOCK_ENTRY)
							{
								i=file->FILEID;
								//文件分类
								if((i&0xff)!=2)
								{
									file++;
									continue;
								}
								//OI
								if(i!=OAD)
								{
									file++;
									continue;
								}
								Len+=file->NUM_DATA;
							}
							file++;
						}
						i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
						if(Len>i)
						{
							Len=i;
						}
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						p8[1]=Len>>8;
						p8[2]=Len;
						Len=3;
						break;
					case 5://5．最大记录数	（static）
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
						p8[1]=i>>8;
						p8[2]=i;
						Len=3;
						break;
					case 6://6．配置参数	（static）
						DataAddr=pLib->Addr;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return 0;
							}
							Len>>=8;
						}
						break;
					case 7://7．当前值记录表	（dyn.）
				//当前值记录表∷=array 当前值
				//当前值∷=structure
				//{
				//事件发生源  instance-specific，
				//累计时间及发生次数  structure
				//{
				//事件发生次数 double-long-unsigned，
				//事件累计时间 double-long-unsigned
				//}
				//}
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
					case 8://8．上报标识	（static）
						DataAddr=pLib->Addr_EventOAD+2;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 9://9．有效标识	（static）
						DataAddr=pLib->Addr_EventOAD;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 10://时间状态记录表
					//时间状态记录表∷=array 当前值 
					//当前值∷=structure 
					//{ 
					//事件发生源 instance-specific， 
					//累计时间及发生次数 structure 
					//{
					//最近一次发生时间 datetime_s，
					//最近一次结束时间 datetime_s 
					//} 
					//} 
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_array;
						p8[1]=1;
						p8[2]=DataType_structure;
						p8[3]=2;
						p8[4]=0;//事件发生源
						p8[5]=DataType_structure;
						p8[6]=2;
						Len=7;
					//最近一次发生时间 datetime_s，
						i=OAD&0xffff0000;
						i|=0x0200;//属性2事件记录表
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:1=找到事件取时间到pOut,0=没找到没填任何数据
					//最近一次结束时间 datetime_s 
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:1=找到事件取时间到pOut,0=没找到没填任何数据
						break;
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
			}
			else
			{
				if(pLib->IC==24)
				{
					switch((OAD>>8)&0xff)
					{
						case 2://2．关联对象属性表	（static）
							DataAddr=pLib->Addr_EventOAD+4;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
								Len>>=8;
							}
							break;
						case 3://3．当前记录数	（dyn.）
							Len=0;
							OAD|=0x2;
							file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
							for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
							{
								if(file->BLOCK_ENTRY)
								{
									i=file->FILEID;
									//文件分类
									if((i&0xff)!=2)
									{
										file++;
										continue;
									}
									//OI
									if(i!=OAD)
									{
										file++;
										continue;
									}
									Len+=file->NUM_DATA;
								}
								file++;
							}
							i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
							if(Len>i)
							{
								Len=i;
							}
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							p8[1]=Len>>8;
							p8[2]=Len;
							Len=3;
							break;
						case 4://4．最大记录数	（static）
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
							p8[1]=i>>8;
							p8[2]=i;
							Len=3;
							break;
						case 5://5．配置参数	（static）
							DataAddr=pLib->Addr;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//类型不匹配           （7）
									return 0;
								}
								Len>>=8;
							}
							break;
						case 6://6．事件记录表1	（dyn.）
						case 7://7．事件记录表2	（dyn.）
						case 8://8．事件记录表3	（dyn.）
						case 9://9．事件记录表4	（dyn.）
							UARTCtrl->DAR=6;//对象不存在           （6）
							return 0;
						case 10://10．当前值记录表	（dyn.）
							DataAddr=pLib->Addr_Currentvalue;
							if(DataAddr==0)
							{
								UARTCtrl->DAR=6;//对象不存在           （6）
								return 0;
							}
							Len=Get_DL698DataLen_S((u8*)DataAddr,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
							break;
						case 11://11．上报标识	（static）
							DataAddr=pLib->Addr_EventOAD+2;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 12://12．有效标识	（static）
							DataAddr=pLib->Addr_EventOAD;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 14://时间状态记录表
						//时间状态记录表∷=array structure 
						//{ 
						//最近一次发生时间 datetime_s， 
						//最近一次结束时间 datetime_s 
						//} 
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_array;
							p8[1]=4;
							Len=2;
							for(n=0;n<4;n++)
							{
								p8[Len+0]=DataType_structure;
								p8[Len+1]=2;
								Len+=2;
							//最近一次发生时间 datetime_s，
								i=OAD&0xffff0000;
								i|=(n<<8)+0x0600;//6．事件记录表1
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
							//最近一次结束时间 datetime_s 
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
							}
							break;
							
						default:
							UARTCtrl->DAR=6;//对象不存在           （6）
							return 0;
					}
				}
			}
		}
		else
		{//单个元素
			if(pLib->IC==7)
			{
				switch((OAD>>8)&0xff)
				{
					case 2://2．事件记录表	（dyn.）
						Len=GET_EventRecord(PORTn,OAD,p8tx,LENmax_TxSPACE,0);//得到事件记录;入口:ALL=0只事件无OAD列表,ALL=1有OAD列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
						return Len;
					case 3://3．关联对象属性表	（static）
						DataAddr=pLib->Addr_EventOAD+4;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							Len>>=8;
						}
						break;
					case 4://4．当前记录数	（dyn.）
						Len=0;
						OAD&=0xffff0000;
						OAD|=0x0202;
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
						for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
						{
							if(file->BLOCK_ENTRY)
							{
								i=file->FILEID;
								//文件分类
								if((i&0xff)!=2)
								{
									file++;
									continue;
								}
								//OI
								if(i!=OAD)
								{
									file++;
									continue;
								}
								Len+=file->NUM_DATA;
							}
							file++;
						}
						i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
						if(Len>i)
						{
							Len=i;
						}
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						p8[1]=Len>>8;
						p8[2]=Len;
						Len=3;
						break;
					case 5://5．最大记录数	（static）
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
						p8[1]=i>>8;
						p8[2]=i;
						Len=3;
						break;
					case 6://6．配置参数	（static）
						DataAddr=pLib->Addr;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//类型不匹配           （7）
								return 0;
							}
							Len>>=8;
						}
						break;
					case 7://7．当前值记录表	（dyn.）
				//当前值记录表∷=array 当前值
				//当前值∷=structure
				//{
				//事件发生源  instance-specific，
				//累计时间及发生次数  structure
				//{
				//事件发生次数 double-long-unsigned，
				//事件累计时间 double-long-unsigned
				//}
				//}
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
					case 8://8．上报标识	（static）
						DataAddr=pLib->Addr_EventOAD+2;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 9://9．有效标识	（static）
						DataAddr=pLib->Addr_EventOAD;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 10://时间状态记录表
					//时间状态记录表∷=array 当前值 
					//当前值∷=structure 
					//{ 
					//事件发生源 instance-specific， 
					//累计时间及发生次数 structure 
					//{
					//最近一次发生时间 datetime_s，
					//最近一次结束时间 datetime_s 
					//} 
					//} 
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_array;
						p8[1]=1;
						p8[2]=DataType_structure;
						p8[3]=2;
						p8[4]=0;//事件发生源
						p8[5]=DataType_structure;
						p8[6]=2;
						Len=7;
					//最近一次发生时间 datetime_s，
						i=OAD&0xffff0000;
						i|=0x0200;//属性2事件记录表
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:1=找到事件取时间到pOut,0=没找到没填任何数据
					//最近一次结束时间 datetime_s 
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:1=找到事件取时间到pOut,0=没找到没填任何数据
						break;
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
			}
			else
			{
				if(pLib->IC==24)
				{
					switch((OAD>>8)&0xff)
					{
						case 2://2．关联对象属性表	（static）
							DataAddr=pLib->Addr_EventOAD+4;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
								Len>>=8;
							}
							break;
						case 3://3．当前记录数	（dyn.）
							Len=0;
							OAD|=0x2;
							file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
							for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
							{
								if(file->BLOCK_ENTRY)
								{
									i=file->FILEID;
									//文件分类
									if((i&0xff)!=2)
									{
										file++;
										continue;
									}
									//OI
									if(i!=OAD)
									{
										file++;
										continue;
									}
									Len+=file->NUM_DATA;
								}
								file++;
							}
							i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
							if(Len>i)
							{
								Len=i;
							}
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							p8[1]=Len>>8;
							p8[2]=Len;
							Len=3;
							break;
						case 4://4．最大记录数	（static）
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							i=DEPTH_EventFile(OAD>>16);//事件文件存储深度;返回:存储深度
							p8[1]=i>>8;
							p8[2]=i;
							Len=3;
							break;
						case 5://5．配置参数	（static）
							DataAddr=pLib->Addr;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//类型不匹配           （7）
									return 0;
								}
								Len>>=8;
							}
							break;
						case 6://6．事件记录表1	（dyn.）
						case 7://7．事件记录表2	（dyn.）
						case 8://8．事件记录表3	（dyn.）
						case 9://9．事件记录表4	（dyn.）
							UARTCtrl->DAR=6;//对象不存在           （6）
							return 0;
						case 10://10．当前值记录表	（dyn.）
							DataAddr=pLib->Addr_Currentvalue;
							if(DataAddr==0)
							{
								UARTCtrl->DAR=6;//对象不存在           （6）
								return 0;
							}
							Len=Get_DL698DataLen_S((u8*)DataAddr,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
							break;
						case 11://11．上报标识	（static）
							DataAddr=pLib->Addr_EventOAD+2;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 12://12．有效标识	（static）
							DataAddr=pLib->Addr_EventOAD;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 14://时间状态记录表
						//时间状态记录表∷=array structure 
						//{ 
						//最近一次发生时间 datetime_s， 
						//最近一次结束时间 datetime_s 
						//} 
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_array;
							p8[1]=4;
							Len=2;
							for(n=0;n<4;n++)
							{
								p8[Len+0]=DataType_structure;
								p8[Len+1]=2;
								Len+=2;
							//最近一次发生时间 datetime_s，
								i=OAD&0xffff0000;
								i|=(n<<8)+0x0600;//6．事件记录表1
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
							//最近一次结束时间 datetime_s 
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//得到最近次事件产生时间;入口:pTSA(有类型)若有效则需比较地址后的最近次;OccurRecovery=0恢复,=1产生;返回:8=找到事件取时间8byte到pOut,1=没找到填1byte空(0)到pOut
							}
							break;
							
						default:
							UARTCtrl->DAR=6;//对象不存在           （6）
							return 0;
					}
				}
			}
//事件缓冲使用约定
//ADDR_DATABUFF事件编辑存放
//ADDR_128KDATABUFF  32KBYTE 保留给DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE 事件参数,关联对象属性表等
//ADDR_128KDATABUFF+64K 32KBYTE 产生事件用临时数据
//ADDR_128KDATABUFF+96K 32KBYTE 事件中各项数据的相应OAD顺序表array OAD
			p8=Get_Element((u8 *)DataAddr,OAD&0xff,0,32*1024);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8==0)
			{
				UARTCtrl->DAR=6;//对象不存在
				return 0;
			}
			Len=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			DataAddr=(u32)p8;
		}
		if(Len>LENmax_TxSPACE)
		{//分帧
			GET_Request_Frame(PORTn);//分帧标志
			return 0;
		}
		MR((u32)p8tx,DataAddr,Len);
		return Len;
	}
	else
	{//0表示整个对象属性，即对象的所有属性
		UARTCtrl->DAR=6;//对象不存在           （6）
		return 0;
	}
}
	
	
	
	
	
	
	
	
	




