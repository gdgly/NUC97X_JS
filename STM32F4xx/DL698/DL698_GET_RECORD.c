

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_RECORD.h"
#include "../DL698/RM_TASK.h"

#include "../DL698/DL698_TESAM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET_iMeter.h"
#include "../DL698/DL698_GET_6000.h"
#include "../DL698/DL698_GET_F000.h"
#include "../DL698/DL698_GET_EVENT.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../DL698/CONGEAL_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../Calculate/Calculate.h"
#include "../Terminal698/Terminal_Init.h"
#include "../DL698/DL698_REPORT.h"


#include "../DL698/DL698_JSON.h"




u32 GET_RecordOAD(u32 PORTn,u8 *p8rx)//读一个记录型对象属性;返回:b31-b16接收ROAD长度,b15-b0发送ROAD及数据字节数,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 n;
	u32 LEN_RSD;
	u32 LEN_MS;
	u32 LEN_RxOAD;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8SOUR;
	u8* pDATA;
	u8 *p8tx;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;
	u32 M_Offset;
	u32 LEN_TxData;
	u32 LEN_ReadData;
	ms_Type* ms;
	u16 *p16;
	u16* p16s;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	u32 OAD;
	DL698_SETLIB_TypeDef *pLib;
	u32 NUMmax;
	u32 MeterCount;
	u32 NUMmax_LIST;
	u64 d64;
	u32 FileID;
	u32 NUM_TIDOT;
	
#if(USER/100)==17//江苏系
	u32 filename;
	msFILE_TypeDef *file;
	u8 *p8d;
	RecordFileHead_TypeDef* RecordFileHead;
#endif
	
	p16=(u16*)(ADDR_1kDATABUFF);
	p16[0]=0;//清0，RCSD为全选时和设置事件时的OAD列表，数
	
	ms=Get_ms();
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;
	p16=(u16 *)Get_ADDR_UARTnTimer_S(PORTn);//秒定时器
	p16[0]=TimerS_UARTRAMFREE;
	OAD=(p8rx[0]<<24)+(p8rx[1]<<16)+(p8rx[2]<<8)+p8rx[3];
//GetRequestRecord∷=SEQUENCE
//{
//  服务序号-优先级         PIID，
//  读取一个记录型对象属性  GetRecord
//}
//GetRecord∷=SEQUENCE
//{
//  对象属性描述符     OAD，
//  记录选择描述符     RSD，
//  记录列选择描述符   RCSD
//}
	LEN_RxOAD=0;
	LEN_TxAPDU=0;
	UARTCtrl->LEN_TxData=0;//需发送数据总长度(包括每段前的2字节段长度)
	UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
	switch(OAD)//对象属性描述符     OAD
	{
//---60xx配置 START----
		case 0x60000200://采集档案配置表
		case 0x60120200://任务配置表
		case 0x60140200://普通采集方案集
		case 0x60160200://事件采集方案集
		case 0x60180200://透明方案集
		case 0x601E0200://采集规则库
			FLASHtoSDRAM();//参数FLASHtoSDRAM
			switch(p8rx[4])//记录选择描述符RSD
			{
				case 0:
		//不选择       [0]  NULL
					LEN_RxOAD+=4+1;
				
					pLib=Get_DL698_SET_pLib(OAD);//得到SET_pLib指针;入口:OI=对象标识,Attribute=属性号;返回:Lib指针(=0表示无相应的OI)
					if(pLib==0)
					{
						UARTCtrl->DAR=6;//对象不存在           （6）
						break;
					}
					NUMmax=pLib->LENmax/pLib->LENper;
					LENmax_TxSPACE=ms->ms_malloc_sizemax();
					pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
					if(pDATA==0)
					{
							UARTCtrl->DAR=255;
							break;
					}
					LEN_TxAPDU=0;
					switch(OAD)//对象属性描述符     OAD
					{
						case 0x60000200://采集档案配置表
							p8SOUR=(u8*)ADDR_6000_SDRAM;
							OAD=0x60010200;//标准单元
							break;
						case 0x60120200://任务配置表
							p8SOUR=(u8*)ADDR_6012_2_SDRAM;
							OAD=0x60130200;//标准单元
							break;
						case 0x60140200://普通采集方案集
							p8SOUR=(u8*)ADDR_6014_SDRAM;
							OAD=0x60150200;//标准单元
							break;
						case 0x60160200://事件采集方案集
							p8SOUR=(u8*)ADDR_6016_SDRAM;
							OAD=0x60170200;//标准单元
							break;
						case 0x60180200://透明方案集
							p8SOUR=(u8*)pLib->Addr;
							OAD=0x60190200;//标准单元
							break;
						case 0x601E0200://采集规则库
							p8SOUR=(u8*)pLib->Addr;
							OAD=0x601F0200;//标准单元
							break;
					}
					//修改列选择
					MR(ADDR_DATABUFF,(u32)p8rx,LEN_RxOAD);
					p8rx=(u8*)ADDR_DATABUFF;
					p8rx[LEN_RxOAD+0]=1;
					p8rx[LEN_RxOAD+1]=0;
					p8rx[LEN_RxOAD+2]=OAD>>24;
					p8rx[LEN_RxOAD+3]=(OAD>>16)&0xff;
					p8rx[LEN_RxOAD+4]=(OAD>>8)&0xff;
					p8rx[LEN_RxOAD+5]=(OAD>>0)&0xff;
					for(n=0;n<NUMmax;n++)
					{
						if(LENmax_TxSPACE<2)
						{
							break;
						}
						x=Get_DL698DataLen_S((u8*)p8SOUR,pLib->LENper);//得到DL698数据类型长度(包括:类型字节和数据);
						if(x>2)
						{
							x+=2;//数据块长度(包括本身2字节)
							if(x>LENmax_TxSPACE)
							{
								UARTCtrl->DAR=255;//错误代码
								break;
							}
							pDATA[LEN_TxAPDU+0]=x;
							pDATA[LEN_TxAPDU+1]=x>>8;
							MR((u32)pDATA+LEN_TxAPDU+2,(u32)p8SOUR,x-2);
							LEN_TxAPDU+=x;
							LENmax_TxSPACE-=x;
						}
						p8SOUR+=pLib->LENper;
					}
					pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
					UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
					UARTCtrl->LEN_TxData=LEN_TxAPDU;//需发送数据总长度(包括每段前的2字节段长度)
					UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
					break;
				case 1:
		//Selector1∷=SEQUENCE
		//{
		//  对象属性描述符  OAD，
		//  数值            Data
		//}
//05 03 08 
//60 00 02 00 对象属性描述符     OAD
				
//01 60 01 02 01 12 00 01 记录选择描述符RSD
//01 00 60 01 02 00 //记录列选择描述符RCSD
//00 无时间标签
					LEN_RxOAD+=4;
					pLib=Get_DL698_SET_pLib(OAD);//得到SET_pLib指针;入口:OI=对象标识,Attribute=属性号;返回:Lib指针(=0表示无相应的OI)
					if(pLib==0)
					{
						UARTCtrl->DAR=6;//对象不存在           （6）
						break;
					}
					switch(p8rx[8])//用于行选择的元素号
					{
						case 1://选择第一个元素(配置序号等)
							switch(p8rx[9])
							{
								case DataType_unsigned:
									n=p8rx[10];
									LEN_RSD=7;
									break;
								case DataType_long_unsigned:
								default:
									n=(p8rx[10]<<8)+p8rx[11];
									LEN_RSD=8;
									break;
							}
							LEN_RxOAD+=LEN_RSD;
							if(n>=(pLib->LENmax/pLib->LENper))
							{
								UARTCtrl->DAR=255;
								break;
							}
							n*=pLib->LENper;
							n+=pLib->Addr;
//							LEN_TxAPDU=Get_DL698DataLen((u8*)n,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							LEN_TxAPDU=Get_DL698DataLen((u8*)n,(Attribute_TypeDef *)0,0,pLib->LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							if(pLib->pDataType)
							{
								if((LEN_TxAPDU&0xff)==0)
								{
									UARTCtrl->DAR=7;//类型不匹配           （7）
									break;
								}
							}
							LEN_TxAPDU>>=8;
							LEN_TxAPDU+=2;//数据块长度(包括本身2字节)
							pDATA=(u8*)ms->ms_malloc(LEN_TxAPDU);
							if(pDATA==0)
							{
								UARTCtrl->DAR=255;//其它                 （255）
								break;
							}
							pDATA[0]=LEN_TxAPDU;
							pDATA[1]=LEN_TxAPDU>>8;
							MR((u32)pDATA+2,n,LEN_TxAPDU-2);
							pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
							UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
							UARTCtrl->LEN_TxData=LEN_TxAPDU;//需发送数据总长度(包括每段前的2字节段长度)
							UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
							break;
						default:
							UARTCtrl->DAR=255;//其它                 （255）
							break;
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
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 3:
		//Selector3∷=SEQUENCE OF Selector2
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 4:
		//Selector4∷=SEQUENCE
		//{
		//  采集启动时间  date_time_s，
		//  电能表集合    MS
		//}
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 5:
		//Selector5∷=SEQUENCE
		//{
		//  采集存储时间  date_time_s，
		//  电能表集合    MS
		//}
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 6:
		//Selector6∷=SEQUENCE
		//{
		//  采集启动时间起始值  date_time_s，
		//  采集启动时间结束值  date_time_s，
		//  时间间隔            TI，
		//  电能表集合          MS
		//}
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 7:
		//Selector7∷=SEQUENCE
		//{
		//  采集存储时间起始值  date_time_s，
		//  采集存储时间结束值  date_time_s，
		//  时间间隔            TI，
		//  电能表集合          MS
		//}
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 8:
		//Selector8∷=SEQUENCE
		//{
		//  采集成功时间起始值  date_time_s，
		//  采集成功时间结束值  date_time_s，
		//  时间间隔            TI，
		//  电能表集合          MS
		//}
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 9:
		//Selector9∷=SEQUENCE
		//{
		//  上第n次记录  unsigned
		//}
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 10:
		//Selector10∷=SEQUENCE
		//{
		//上n条记录  unsigned，
		//电能表集合  MS
		//}
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				default:
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
			}
			break;
			
//---60配置 END----
			
//---6034采集任务监控 START----
		case 0x60340200://采集任务监控数据
			switch(p8rx[4])//记录选择描述符RSD
			{
				default:
					UARTCtrl->DAR=6;//对象不存在           （6）
					break;
				case 1:
		//Selector1∷=SEQUENCE
		//{
		//  对象属性描述符  OAD，
		//  数值            Data
		//}
//05 03 08 
//60 00 02 00 对象属性描述符     OAD
//01 60 01 02 01 12 00 01 记录选择描述符RSD
//01 00 60 01 02 00 //记录列选择描述符RCSD
//00 无时间标签

//05 03 0e 
//60 34 02 00 对象属性描述符     OAD
//01 60 35 02 01 11 01 记录选择描述符RSD,用第一个元素任务ID unsigned进行记录行选择
//00 记录列选择描述符RCSD
//00 无时间标签
					LEN_RxOAD+=4;
					//LEN_RSD=8;
					//LEN_RxOAD+=LEN_RSD;
					
					switch(p8rx[8])//用于行选择的元素号
					{
						case 1://任务ID unsigned进行记录行选择
							p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
							LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
							LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
						#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
							ClientConnectInfo=ClientConnectInfo;
						#else
							//客户机接收缓冲长度
							ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
							i=ClientConnectInfo->LEN_Rx;
							i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
							if(i<2048)
							{
								i=2048;
							}
							if(i>(TxAPDUOffset+1))
							{//不为0
								i-=(TxAPDUOffset+1);
								if(LENmax_TxSPACE>i)
								{
									LENmax_TxSPACE=i;
								}
							}
						#endif
//68 42 00 C3 05 01 00 00 00 00 00 10 75 85 85 03 0C 
//60 34 02 00 01 
//00 60 35 02 00 01 01 02 08 11 01 16 02 1C 07 E0 0B 0B 00 02 00 1C 07 E0 0B 0B 00 02 02 12 00 02 12 00 02 12 00 02 12 00 02 00 00 3D 50 16 
							p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
							p8tx[0]=133;//读取响应         	[133]	GET-Response
							p8tx[1]=3;//[3] GeResponseRecord
							p8tx[2]=UARTCtrl->PIID;//PIID-ACD
							p8tx[3]=p8rx[0];//OAD
							p8tx[4]=p8rx[1];
							p8tx[5]=p8rx[2];
							p8tx[6]=p8rx[3];
							p8tx[7]=1;//有数据
							p8tx[8]=0;//OAD类型
							p8tx[9]=0x60;
							p8tx[10]=0x35;
							p8tx[11]=0x02;
							p8tx[12]=0x00;
							p8tx[13]=1;//记录数据
							p8tx[14]=1;//M条
							
							n=p8rx[10];//任务ID
							if(n>=RMTASKmax)
							{
								UARTCtrl->DAR=255;
								break;
							}
							RM_Monitor(n,0,0);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数,NUM_Rx=接收条数
							n=ADDR_6034+(n*LENper_6034);
							LEN_TxAPDU=Get_DL698DataLen((u8*)n,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
							LEN_TxAPDU>>=8;
							if((15+LEN_TxAPDU)>LENmax_TxSPACE)
							{
								UARTCtrl->DAR=255;
								break;
							}
							MR((u32)p8tx+15,n,LEN_TxAPDU);
							LEN_TxAPDU+=15;
							return LEN_TxAPDU;
						default:
							UARTCtrl->DAR=255;
							break;
					}
					break;
				}
			break;
//---6034采集任务监控 END----
				
		case 0x60020200://所有搜表结果
			UARTCtrl->DAR=255;//错误代码
			return 0;
		case 0x60020500://跨台区搜表结果
			UARTCtrl->DAR=255;//错误代码
			return 0;
			
		case 0x601A0200://透明方案结果集
		case 0x601C0200://上报方案集
			UARTCtrl->DAR=255;//错误代码
			return 0;
		
//---60120300任务配置表 START----
		case 0x60120300://任务配置表属性3抄表记录
			//计算RSD长度
			LEN_RSD=DataLen_RSD_NoMS(p8rx+4,0,0);//内联的MS长度不计
			LEN_RSD>>=8;
			if(LEN_RSD)
			{
				LEN_RSD--;//无数据类型
			}
			//计算MS长度
			if((p8rx[4]==0)||(p8rx[4]==9))//0=不选择无MS,9=上第n次记录无MS
			{
				LEN_MS=0;
			}
			else
			{
				LEN_MS=DataLen_MS(p8rx+4+LEN_RSD,0,0);
				LEN_MS>>=8;
				if(LEN_MS)
				{
					LEN_MS--;//无数据类型
				}
			}
			LEN_RxOAD+=4;
			LEN_RxOAD+=LEN_RSD;
			LEN_RxOAD+=LEN_MS;
			LENmax_TxSPACE=ms->ms_malloc_sizemax();
			pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
			if(pDATA==0)
			{
				UARTCtrl->DAR=255;
				break;
			}
			if(p8rx[4]==0)
			{//不选择无MS
				NUMmax_LIST=0;
				p16=(u16*)(ADDR_AddrList+2);
				p8SOUR=(u8*)ADDR_6000_SDRAM;
				for(i=0;i<NUMmax_6000;i++)
				{
					if(p8SOUR[0]==DataType_structure)
					{
						p16s=(u16*)(p8SOUR+10);//取采集档案配置序号的通信地址
						p16[0]=p16s[0];
						p16[1]=p16s[1];
						p16[2]=p16s[2];
						p16+=3;
						NUMmax_LIST+=1;
					}
					p8SOUR+=LENper_6000;
				}
				p16=(u16*)(ADDR_AddrList);
				p16[0]=NUMmax_LIST;
			}
			else
			{
				if(p8rx[4]==9)//9=上第n次记录无MS
				{
					p16=(u16*)(ADDR_AddrList);
					p16[0]=1;//无MS,为了下程序for()能进入计算用值=1
				}
				else
				{
					MStoAddrList(p8rx+4+LEN_RSD);//电能表集合到地址列表;入口:pMS(无数据类型);出口:地址列表在ADDR_AddrList
				}
			}
			p16=(u16*)(ADDR_AddrList);
			NUMmax_LIST=p16[0];
			LEN_TxAPDU=0;
			//注:RCSD前没数据类型，RCSDtoFileID()或RCSDtoID()是有数据类型
			FileID=RCSDtoFileID(p8rx+LEN_RxOAD-1);//记录列选择RCSD相对应的FileID,入口:pRCSD指向类型字节;返回:0表示没找到无效FileID,当有2个上相同方案时TaskNO=0xff
			if(FileID==0)
			{//0表示没找到无效FileID
				pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
				UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
				UARTCtrl->LEN_TxData=LEN_TxAPDU;//需发送数据总长度(包括每段前的2字节段长度)
				UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
				break;
			}
			for(MeterCount=0;MeterCount<NUMmax_LIST;MeterCount++)
			{
				if(LENmax_TxSPACE>2)
				{
					switch(p8rx[4])
					{
						case 0:
				//不选择       [0]  NULL
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//数据块长度(包括本身2字节)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;//错误代码
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							break;
						case 1:
				//Selector1∷=SEQUENCE
				//{
				//  对象属性描述符  OAD，
				//  数值            Data
				//}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//数据块长度(包括本身2字节)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
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
							n=(p8rx[5]<<24)|(p8rx[6]<<16)|(p8rx[7]<<8)|p8rx[8];
							switch(n)
							{
								case 0x20210200://数据冻结时间
									if((p8rx[4+5]==DataType_date_time_s)&&(p8rx[4+5+8]==DataType_date_time_s))
									{
										if((p8rx[4+5+8+8]==DataType_TI)||(p8rx[4+5+8+8]==0))
										{
											x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
											NUM_TIDOT=0;//数据招读时最大按TI间隔的数据点数
											while((x!=1)&&(NUM_TIDOT<NUMmax_TIDOT))
											{
												NUM_TIDOT++;
												if(LENmax_TxSPACE<2)
												{
													break;
												}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
												x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
												if(x)
												{
													x+=2;//数据块长度(包括本身2字节)
													if(x>LENmax_TxSPACE)
													{
														UARTCtrl->DAR=255;//错误代码
														break;
													}
													pDATA[LEN_TxAPDU+0]=x;
													pDATA[LEN_TxAPDU+1]=x>>8;
													LEN_TxAPDU+=x;
													LENmax_TxSPACE-=x;
												}
												//数据间隔
												if(p8rx[4+5+8+8]==0)
												{//TI=空
													break;
												}
												else
												{
													x=pTItoSECOND(p8rx+4+5+8+8+1);//TI转为秒值,入口:pTI无数据类型,出口:hex秒
													if(x==0)
													{//无间隔
														break;
													}
												}
												pYYMDHMS_AddTI_hex(p8rx+4+5+1,p8rx+4+5+8+8+1);//年年月日时分秒加时间间隔TI,入口:pYYMDHMS,pTI无数据类型,返回加间隔TI后的年年月日时分秒
												//pYYMDHMS_AddS_hex(p8rx+4+5+1,x);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
												x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
											}
										}
									}
									break;
								default:
									break;
							}
							break;
						case 3:
				//Selector3∷=SEQUENCE OF Selector2
							n=(p8rx[6]<<24)|(p8rx[7]<<16)|(p8rx[8]<<8)|p8rx[9];
							switch(n)
							{
								case 0x20210200://数据冻结时间
									NUMmax=p8rx[5];
									if(NUMmax>=2)
									{
										n=Compare_DL698DataString(p8rx+4+2,p8rx+4+6+8+8+4,4+8+8+4);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
										if(n==0)
										{
											if(NUMmax)
											{
												NUMmax--;//2个时间相同
											}
										}
									}
									for(n=0;n<NUMmax;n++)
									{
										if((p8rx[4+6]==DataType_date_time_s)&&(p8rx[4+6+8]==DataType_date_time_s))
										{
											if((p8rx[4+6+8+8]==DataType_TI)||(p8rx[4+6+8+8]==0))
											{
												x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
												NUM_TIDOT=0;//数据招读时最大按TI间隔的数据点数
												while((x!=1)&&(NUM_TIDOT<NUMmax_TIDOT))
												{
													NUM_TIDOT++;
													if(LENmax_TxSPACE<2)
													{
														break;
													}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
													x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
													if(x)
													{
														x+=2;//数据块长度(包括本身2字节)
														if(x>LENmax_TxSPACE)
														{
															UARTCtrl->DAR=255;//错误代码
															break;
														}
														pDATA[LEN_TxAPDU+0]=x;
														pDATA[LEN_TxAPDU+1]=x>>8;
														LEN_TxAPDU+=x;
														LENmax_TxSPACE-=x;
													}
													//数据间隔
													if(p8rx[4+6+8+8]==0)
													{//无间隔
														break;
													}
													else
													{
														x=pTItoSECOND(p8rx+4+6+8+8+1);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
														if(x==0)
														{//无间隔
															break;
														}
													}
													pYYMDHMS_AddTI_hex(p8rx+4+6+1,p8rx+4+6+8+8+1);//年年月日时分秒加时间间隔TI,入口:pYYMDHMS,pTI无数据类型,返回加间隔TI后的年年月日时分秒
													//pYYMDHMS_AddS_hex(p8rx+4+6+1,x);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
													x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
												}
											}
										}
										if(n)
										{
											MW((u32)p8rx,(u32)p8rx+(4+6+8+8+4)-6,6);
										}
										p8rx+=4+8+8+4;
										if(LEN_RSD>=(4+8+8+4))
										{
											LEN_RSD-=(4+8+8+4);
										}
									}
									p8rx-=NUMmax*(4+8+8+4);
									LEN_RSD+=NUMmax*(4+8+8+4);
									break;
								default:
									break;
							}
							break;
						case 4:
				//Selector4∷=SEQUENCE
				//{
				//  采集启动时间  date_time_s，
				//  电能表集合    MS
				//}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//数据块长度(包括本身2字节)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;//错误代码
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							break;
						case 5:
				//Selector5∷=SEQUENCE
				//{
				//  采集存储时间  date_time_s，
				//  电能表集合    MS
				//}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							if(x)
							{
								x+=2;//数据块长度(包括本身2字节)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;//错误代码
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							break;
						case 6:
				//Selector6∷=SEQUENCE
				//{
				//  采集启动时间起始值  date_time_s，
				//  采集启动时间结束值  date_time_s，
				//  时间间隔            TI，
				//  电能表集合          MS
				//}
						case 7:
				//Selector7∷=SEQUENCE
				//{
				//  采集存储时间起始值  date_time_s，
				//  采集存储时间结束值  date_time_s，
				//  时间间隔            TI，
				//  电能表集合          MS
				//}
						case 8:
				//Selector8∷=SEQUENCE
				//{
				//  采集成功时间起始值  date_time_s，
				//  采集成功时间结束值  date_time_s，
				//  时间间隔            TI，
				//  电能表集合          MS
				//}
//05 03 0D 
//60 12 03 00 
//07  Selector7
//07 E0 0B 0B 00 00 00  采集存储时间起始值  date_time_s
//07 E0 0B 0B 17 3B 3B  采集存储时间结束值  date_time_s
//00 FF FF  时间间隔  TI
//01 04 00 20 2A 02 00 00 00 10 02 00 00 20 00 02 00 00 20 01 02 00 
//00 
							d64=MRR((u32)p8rx+4+1,7);
							x=Compare_DL698DataString(p8rx+4+1,p8rx+4+1+7,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							NUM_TIDOT=0;//数据招读时最大按TI间隔的数据点数
							while((x!=1)&&(NUM_TIDOT<NUMmax_TIDOT))
							{
								NUM_TIDOT++;
								if(LENmax_TxSPACE<2)
								{
									break;
								}
//u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
								x=GET_RecordFileData(FileID,0xffffffff,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
								if(x)
								{
									x+=2;//数据块长度(包括本身2字节)
									if(x>LENmax_TxSPACE)
									{
										UARTCtrl->DAR=255;//错误代码
										break;
									}
									pDATA[LEN_TxAPDU+0]=x;
									pDATA[LEN_TxAPDU+1]=x>>8;
									LEN_TxAPDU+=x;
									LENmax_TxSPACE-=x;
								}
//								if(p8rx[4]==8)
//								{//采集成功时间
//									break;
//								}
								x=pTItoSECOND(p8rx+4+1+7+7);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
								if(x==0)
								{//无间隔
									break;
								}
								pYYMDHMS_AddTI_hex(p8rx+4+1,p8rx+4+1+7+7);//年年月日时分秒加时间间隔TI,入口:pYYMDHMS,pTI无数据类型,返回加间隔TI后的年年月日时分秒
								//pYYMDHMS_AddS_hex(p8rx+4+1,x);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
								x=Compare_DL698DataString(p8rx+4+1,p8rx+4+1+7,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							}
							MWR(d64,(u32)p8rx+4+1,7);
							break;
						case 9:
				//Selector9∷=SEQUENCE
				//{
				//  上第n次记录  unsigned
				//}
							UARTCtrl->DAR=255;//错误代码
							break;
						case 10:
				//Selector10∷=SEQUENCE
				//{
				//上n条记录  unsigned，
				//电能表集合  MS
				//}
							NUMmax=p8rx[4+1];//上n条记录
							i=0;//开始文件计数号
							while(1)
							{
								if((NUMmax==0)||(i>=127))
								{
									break;
								}
//u32 Get_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							#if(USER/100)==17//江苏系
								
								
								if(FileID==MeterAutoReportFileID_3011)
								{
										Comm_Ram->msFILESearchCount=0;
										filename=Get_RecordFileName(FileID,0xffffffff,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFFFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
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
											//ms=Get_ms();
											//p8d=(u8*)ms->ms_realloc(p8d,i);
											//p8d=(u8*)ms->ms_malloc(i);//动态分配我的内存堆
											p8d=(u8*)ADDR_DATABUFF;
											if(p8d)
											{
												//ms->ms_free(p8d);//释放我的内存堆分配
												ms->msfile_read(filename,NUM_RMmax*sizeof(RecordFileHead_TypeDef),p8d,i);
												n=file->NUM_DATA;
												if((n*sizeof(RecordFileHead_TypeDef))>LEN_128KDATABUFF)
												{
													n=0;
												}
												ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,n*sizeof(RecordFileHead_TypeDef));
												RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
												for(i=0;i<n;i++)
												{
													x=Get_RecordFileData_RCSD(p8rx+4+LEN_RSD+LEN_MS,(u8*)&RecordFileHead->Addr0,p8d,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2,0);//记录表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pTSA,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
													if(x)
													{
														//f=1;//有找到数据标志
														NUMmax--;//上n条-1
														x+=2;//数据块长度(包括本身2字节)
														if(x>LENmax_TxSPACE)
														{
															UARTCtrl->DAR=255;//错误代码
															break;
														}
														pDATA[LEN_TxAPDU+0]=x;
														pDATA[LEN_TxAPDU+1]=x>>8;
														LEN_TxAPDU+=x;
														LENmax_TxSPACE-=x;
													}
													p8d+=RecordFileHead->LEN_DATA;
													RecordFileHead++;
												}
												MeterCount=NUMmax_LIST+1;
												break;
//												
												}
											}
										
								}
								else
								x=GET_RecordFileData(FileID,i,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							#else	
								x=GET_RecordFileData(FileID,i,(u16*)((ADDR_AddrList+2)+(6*MeterCount)),p8rx+4,p8rx+4+LEN_RSD+LEN_MS,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
							#endif	
								if(x)
								{
									//f=1;//有找到数据标志
									NUMmax--;//上n条-1
									x+=2;//数据块长度(包括本身2字节)
									if(x>LENmax_TxSPACE)
									{
										UARTCtrl->DAR=255;//错误代码
										break;
									}
									pDATA[LEN_TxAPDU+0]=x;
									pDATA[LEN_TxAPDU+1]=x>>8;
									LEN_TxAPDU+=x;
									LENmax_TxSPACE-=x;
								}
								i++;//文件计数号
							}
							break;
						default:
							UARTCtrl->DAR=255;//错误代码
							break;
					}
				}
			}
			pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
			UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
			UARTCtrl->LEN_TxData=LEN_TxAPDU;//需发送数据总长度(包括每段前的2字节段长度)
			UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
			break;
//---60120300任务配置表 END----
			
//---50终端冻结 START---
		case 0x50000200://瞬时冻结
		case 0x50010200://秒冻结
		case 0x50020200://分钟冻结
		case 0x50030200://小时冻结
		case 0x50040200://日冻结
		case 0x50050200://结算日冻结
		case 0x50060200://月冻结
		case 0x50070200://年冻结
		case 0x50080200://时区表切换冻结
		case 0x50090200://日时段表切换冻结
		case 0x500A0200://费率电价切换冻结
		case 0x500B0200://阶梯切换冻结
		case 0x50110200://阶梯结算冻结
//05 03 03 
//50 04 02 00 01 20 21 02 00 1c 07 E0 01 14 00 00 00 02 00 20 21 02 00 00 00 10 02 00 00
			//计算RSD长度
			LEN_RSD=DataLen_RSD_NoMS(p8rx+4,0,0);//内联的MS长度不计
			LEN_RSD>>=8;
			if(LEN_RSD)
			{
				LEN_RSD--;//无数据类型
			}
			LEN_RxOAD+=4;
			LEN_RxOAD+=LEN_RSD;
			LENmax_TxSPACE=ms->ms_malloc_sizemax();
			if(LENmax_TxSPACE<2)
			{
					UARTCtrl->DAR=255;
					break;
			}
			pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
			if(pDATA==0)
			{
					UARTCtrl->DAR=255;
					break;
			}
			LEN_TxAPDU=0;
			switch(p8rx[4])
			{
				case 0:
		//不选择       [0]  NULL
					UARTCtrl->DAR=255;//错误代码
					break;
				case 1:
		//Selector1∷=SEQUENCE
		//{
		//  对象属性描述符  OAD，
		//  数值            Data
		//}
					x=Get_CongealFileData(OAD>>16,0xffffffff,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到冻结文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
					if(x)
					{
						x+=2;//数据块长度(包括本身2字节)
						if(x>LENmax_TxSPACE)
						{
							UARTCtrl->DAR=255;
							break;
						}
						pDATA[LEN_TxAPDU+0]=x;
						pDATA[LEN_TxAPDU+1]=x>>8;
						LEN_TxAPDU+=x;
						LENmax_TxSPACE-=x;
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
					n=(p8rx[5]<<24)|(p8rx[6]<<16)|(p8rx[7]<<8)|p8rx[8];
					switch(n)
					{
						case 0x20210200://数据冻结时间
							if((p8rx[4+5]==DataType_date_time_s)&&(p8rx[4+5+8]==DataType_date_time_s))
							{
								if((p8rx[4+5+8+8]==DataType_TI)||(p8rx[4+5+8+8]==0))
								{
									x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
									while(x!=1)
									{
										if(LENmax_TxSPACE<2)
										{
											break;
										}
										x=Get_CongealFileData(OAD>>16,0xffffffff,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到冻结文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
										if(x)
										{
											x+=2;//数据块长度(包括本身2字节)
											if(x>LENmax_TxSPACE)
											{
												UARTCtrl->DAR=255;//错误代码
												break;
											}
											pDATA[LEN_TxAPDU+0]=x;
											pDATA[LEN_TxAPDU+1]=x>>8;
											LEN_TxAPDU+=x;
											LENmax_TxSPACE-=x;
										}
										//数据间隔
										if(p8rx[4+5+8+8]==0)
										{
											break;;//x=15*60;//15分
										}
										else
										{
											x=pTItoSECOND(p8rx+4+5+8+8+1);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
											if(x==0)
											{
												break;//x=24*60*60;
											}
										}
										pYYMDHMS_AddTI_hex(p8rx+4+5+1,p8rx+4+5+8+8+1);//年年月日时分秒加时间间隔TI,入口:pYYMDHMS,pTI无数据类型,返回加间隔TI后的年年月日时分秒
										//pYYMDHMS_AddS_hex(p8rx+4+5+1,x);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
										x=Compare_DL698DataString(p8rx+4+5+1,p8rx+4+5+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
									}
								}
							}
							break;
						default:
							break;
					}
					break;
				case 3:
		//Selector3∷=SEQUENCE OF Selector2
					n=(p8rx[6]<<24)|(p8rx[7]<<16)|(p8rx[8]<<8)|p8rx[9];
					switch(n)
					{
						case 0x20210200://数据冻结时间
							NUMmax=p8rx[5];
							if(NUMmax>=2)
							{
								n=Compare_DL698DataString(p8rx+4+2,p8rx+4+6+8+8+4,4+8+8+4);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
								if(n==0)
								{
									if(NUMmax)
									{
										NUMmax--;//2个时间相同
									}
								}
							}
							for(n=0;n<NUMmax;n++)
							{
								if((p8rx[4+6]==DataType_date_time_s)&&(p8rx[4+6+8]==DataType_date_time_s))
								{
									if((p8rx[4+6+8+8]==DataType_TI)||(p8rx[4+6+8+8]==0))
									{
										x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
										while(x!=1)
										{
											if(LENmax_TxSPACE<2)
											{
												break;
											}
											x=Get_CongealFileData(OAD>>16,0xffffffff,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到冻结文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
											if(x)
											{
												x+=2;//数据块长度(包括本身2字节)
												if(x>LENmax_TxSPACE)
												{
													UARTCtrl->DAR=255;//错误代码
													break;
												}
												pDATA[LEN_TxAPDU+0]=x;
												pDATA[LEN_TxAPDU+1]=x>>8;
												LEN_TxAPDU+=x;
												LENmax_TxSPACE-=x;
											}
											//数据间隔
											if(p8rx[4+6+8+8]==0)
											{
												break;//x=15*60;//15分
											}
											else
											{
												x=pTItoSECOND(p8rx+4+6+8+8+1);//TI转为秒值,入口:pTI无数据类型;出口:hex秒
												if(x==0)
												{
													break;//x=24*60*60;
												}
											}
											pYYMDHMS_AddTI_hex(p8rx+4+6+1,p8rx+4+6+8+8+1);//年年月日时分秒加时间间隔TI,入口:pYYMDHMS,pTI无数据类型,返回加间隔TI后的年年月日时分秒
											//pYYMDHMS_AddS_hex(p8rx+4+6+1,x);//年年月日时分秒加S秒,返回加S秒后的年年月日时分秒
											x=Compare_DL698DataString(p8rx+4+6+1,p8rx+4+6+8+1,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
										}
									}
								}
								if(n)
								{
									MW((u32)p8rx,(u32)p8rx+(4+6+8+8+4)-6,6);
								}
								p8rx+=4+8+8+4;
								if(LEN_RSD>=(4+8+8+4))
								{
									LEN_RSD-=(4+8+8+4);
								}
							}
							p8rx-=NUMmax*(4+8+8+4);
							LEN_RSD+=NUMmax*(4+8+8+4);
							break;
						default:
							break;
					}
					break;
				case 4:
		//Selector4∷=SEQUENCE
		//{
		//  采集启动时间  date_time_s，
		//  电能表集合    MS
		//}
				case 5:
		//Selector5∷=SEQUENCE
		//{
		//  采集存储时间  date_time_s，
		//  电能表集合    MS
		//}
				case 6:
		//Selector6∷=SEQUENCE
		//{
		//  采集启动时间起始值  date_time_s，
		//  采集启动时间结束值  date_time_s，
		//  时间间隔            TI，
		//  电能表集合          MS
		//}
				case 7:
		//Selector7∷=SEQUENCE
		//{
		//  采集存储时间起始值  date_time_s，
		//  采集存储时间结束值  date_time_s，
		//  时间间隔            TI，
		//  电能表集合          MS
		//}
				case 8:
		//Selector8∷=SEQUENCE
		//{
		//  采集成功时间起始值  date_time_s，
		//  采集成功时间结束值  date_time_s，
		//  时间间隔            TI，
		//  电能表集合          MS
		//}
					break;
				case 9:
		//Selector9∷=SEQUENCE
		//{
		//  上第n次记录  unsigned
		//}
					NUMmax=p8rx[4+1];//上第n次
					if(NUMmax)
					{
						NUMmax--;
					}
					if(p8rx[4+2]==0)
					{//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”
						//增加列选择OAD
						if(LEN_RxOAD<1024)
						{//保证缓冲长度
							MR(ADDR_DATABUFF,(u32)p8rx,LEN_RxOAD);
							p8rx=(u8*)ADDR_DATABUFF;
							n=MRR(ADDR_5004+1,1);//日冻结关联对象属性表
							p8rx[LEN_RxOAD]=n;//个数
							for(x=0;x<n;x++)
							{
								p8rx[LEN_RxOAD+1+(5*x)]=0;//OAD
								MR((u32)p8rx+LEN_RxOAD+1+(5*x)+1,ADDR_5004+2+(x*13)+6,4);
							}
						}
					}
					x=Get_CongealFileData(OAD>>16,NUMmax,p8rx+4,p8rx+4+LEN_RSD,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2);//得到冻结文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
					if(x)
					{
						x+=2;//数据块长度(包括本身2字节)
						if(x>LENmax_TxSPACE)
						{
							UARTCtrl->DAR=255;
							break;
						}
						pDATA[LEN_TxAPDU+0]=x;
						pDATA[LEN_TxAPDU+1]=x>>8;
						LEN_TxAPDU+=x;
						LENmax_TxSPACE-=x;
					}
			#if (USER/100)==0//电科院测试
					else
					{
						//01 00 90 01 09 00
						i=p8rx[4+LEN_RSD]<<8;
						i|=p8rx[4+LEN_RSD+1];
						if(i==0x0100)
						{
							i=p8rx[4+LEN_RSD+2]<<24;
							i|=p8rx[4+LEN_RSD+3]<<16;
							i|=p8rx[4+LEN_RSD+4]<<8;
							i|=p8rx[4+LEN_RSD+5];
							if(i==0x90010900)
							{
								x=1+2;//数据块长度(包括本身2字节)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								pDATA[LEN_TxAPDU+2]=0;//数据空
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
						}
					}
			#endif
					break;
				case 10:
		//Selector10∷=SEQUENCE
		//{
		//上n条记录  unsigned，
		//电能表集合  MS
		//}
				default:
					break;
			}
			pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
			UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
			UARTCtrl->LEN_TxData=LEN_TxAPDU;//需发送数据总长度(包括每段前的2字节段长度)
			UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
			break;
//---50终端冻结 END---
			
		default:
			switch(OAD>>28)
			{
//---事件记录 START---
				case 3://事件记录
					switch(p8rx[4])//记录选择描述符RSD
					{
						case 0:
				//不选择       [0]  NULL
							
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 1:
				//Selector1∷=SEQUENCE
				//{
				//  对象属性描述符  OAD，
				//  数值            Data
				//}
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 2:
				//Selector2∷=SEQUENCE
				//{
				//  对象属性描述符  OAD，
				//  起始值          Data，
				//  结束值          Data，
				//  数据间隔        Data
				//}
//05 03 0d 
//30 0f 02 00 
//02
//20 1e 02 00 
//1c 07 e1 05 1f 0d 2f 1c 
//1c 07 e1 05 1f 0d 30 3b 
//54 00 00 00 
//00
//00
//电科院电压相序测试时用到，暂与读最近一次相同，实际没用
//返回
//85 03 0D 
//30 0F 02 00 
//04 
//00 20 22 02 00 
//00 20 1E 02 00 
//00 20 20 02 00 
//00 20 24 02 00 
//01 00 
//00 00 
							LEN_RSD=25;
							LEN_RxOAD+=4+LEN_RSD;
							OAD&=0xffffff00;
							OAD|=1;//上第n次
							GET_EventRecord(PORTn,OAD,(u8*)ADDR_128KDATABUFF,LEN_128KDATABUFF,1);//得到事件记录;入口:ALL=0只事件无OAD列表,ALL=1有OAD列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
							if(UARTCtrl->DAR)
							{
								//无事件，电科院测试时需返回有数据
								UARTCtrl->DAR=0;
								p8tx=(u8*)ADDR_1kDATABUFF;
								p8tx[0]=4;
								MWR(0x0002222000,ADDR_1kDATABUFF+1,5);
								MWR(0x00021e2000,ADDR_1kDATABUFF+1+5,5);
								MWR(0x0002202000,ADDR_1kDATABUFF+1+5+5,5);
								MWR(0x0002242000,ADDR_1kDATABUFF+1+5+5+5,5);
								pDATA=(u8*)ms->ms_malloc(1);//仅使指针不为空，其他无用
								if(pDATA==0)
								{
									UARTCtrl->DAR=255;
									break;
								}
								UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
								UARTCtrl->LEN_TxData=0;//需发送数据总长度(包括每段前的2字节段长度)
								UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
								break;
							}
							LENmax_TxSPACE=ms->ms_malloc_sizemax();
							if(LENmax_TxSPACE<2)
							{
								UARTCtrl->DAR=255;
								break;
							}
							pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
							if(pDATA==0)
							{
								UARTCtrl->DAR=255;
								break;
							}
							LEN_TxAPDU=0;
							x=GET_EventRecord_RCSD(OAD,p8rx+4+LEN_RSD,(u8*)ADDR_128KDATABUFF,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2,ALLDATANULLSEL);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
							if(x)
							{
								x+=2;//数据块长度(包括本身2字节)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
							UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
							UARTCtrl->LEN_TxData=LEN_TxAPDU;//需发送数据总长度(包括每段前的2字节段长度)
							UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
							break;
						case 3:
				//Selector3∷=SEQUENCE OF Selector2
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 4:
				//Selector4∷=SEQUENCE
				//{
				//  采集启动时间  date_time_s，
				//  电能表集合    MS
				//}
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 5:
				//Selector5∷=SEQUENCE
				//{
				//  采集存储时间  date_time_s，
				//  电能表集合    MS
				//}
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 6:
				//Selector6∷=SEQUENCE
				//{
				//  采集启动时间起始值  date_time_s，
				//  采集启动时间结束值  date_time_s，
				//  时间间隔            TI，
				//  电能表集合          MS
				//}
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 7:
				//Selector7∷=SEQUENCE
				//{
				//  采集存储时间起始值  date_time_s，
				//  采集存储时间结束值  date_time_s，
				//  时间间隔            TI，
				//  电能表集合          MS
				//}
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 8:
				//Selector8∷=SEQUENCE
				//{
				//  采集成功时间起始值  date_time_s，
				//  采集成功时间结束值  date_time_s，
				//  时间间隔            TI，
				//  电能表集合          MS
				//}
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						case 9:
				//Selector9∷=SEQUENCE
				//{
				//  上第n次记录  unsigned
				//}
							LEN_RSD=2;
							LEN_RxOAD+=4+LEN_RSD;
							n=p8rx[5];//上第n次
							OAD&=0xffffff00;
							OAD|=n;
							GET_EventRecord(PORTn,OAD,(u8*)ADDR_128KDATABUFF,LEN_128KDATABUFF,1);//得到事件记录;入口:ALL=0只事件无OAD列表,ALL=1有OAD列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
							if(UARTCtrl->DAR)
							{
								break;
							}
							LENmax_TxSPACE=ms->ms_malloc_sizemax();
							if(LENmax_TxSPACE<2)
							{
								UARTCtrl->DAR=255;
								break;
							}
							pDATA=(u8*)ms->ms_malloc(LENmax_TxSPACE);
							if(pDATA==0)
							{
								UARTCtrl->DAR=255;
								break;
							}
							LEN_TxAPDU=0;
							x=GET_EventRecord_RCSD(OAD,p8rx+4+LEN_RSD,(u8*)ADDR_128KDATABUFF,pDATA+LEN_TxAPDU+2,LENmax_TxSPACE-2,ALLDATANULLSEL);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
							if(x)
							{
								x+=2;//数据块长度(包括本身2字节)
								if(x>LENmax_TxSPACE)
								{
									UARTCtrl->DAR=255;
									break;
								}
								pDATA[LEN_TxAPDU+0]=x;
								pDATA[LEN_TxAPDU+1]=x>>8;
								LEN_TxAPDU+=x;
								LENmax_TxSPACE-=x;
							}
							pDATA=(u8*)ms->ms_realloc(pDATA,LEN_TxAPDU);
							UARTCtrl->pTxData=pDATA;//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
							UARTCtrl->LEN_TxData=LEN_TxAPDU;//需发送数据总长度(包括每段前的2字节段长度)
							UARTCtrl->LEN_ReadData=0;//已读取的发送数据总长度(包括每段前的2字节段长度)
							break;
						case 10:
				//Selector10∷=SEQUENCE
				//{
				//上n条记录  unsigned，
				//电能表集合  MS
				//}
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
						default:
							UARTCtrl->DAR=6;//对象不存在           （6）
							break;
					}
					break;
//---事件记录 END---
					
//---未知  START---
				default:
//RET_ErrDAR:
					if(UARTCtrl->pTxData)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
					{
						ms->ms_free(UARTCtrl->pTxData);
						UARTCtrl->pTxData=0;
					}
					//计算RSD长度
					LEN_RSD=DataLen_RSD_NoMS(p8rx+4,0,0);//内联的MS长度不计
					LEN_RSD>>=8;
					if(LEN_RSD)
					{
						LEN_RSD--;//无数据类型
					}
					LEN_RxOAD=4+LEN_RSD;
					LEN_TxAPDU=0;
					//break;

					p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
					LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
					LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
					p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
					p8tx[0]=133;//读取响应         	[133]	GET-Response
					p8tx[1]=3;//[3] GeResponseRecord
					p8tx[2]=UARTCtrl->PIID;//PIID-ACD
					p8tx[3]=p8rx[0];//OAD
					p8tx[4]=p8rx[1];
					p8tx[5]=p8rx[2];
					p8tx[6]=p8rx[3];
					LEN_TxAPDU=7;
					LENmax_TxSPACE-=LEN_TxAPDU;
					x=LEN_RxOAD;
					n=p8rx[LEN_RxOAD];//SEQUENCE OF CSD个数
					if(n==0)
					{
						//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”。
						p8rx=(u8*)ADDR_1kDATABUFF;
						n=p8rx[0];
						p8rx-=x;
					}
					LEN_RxOAD++;
					while(n--)
					{
						if(p8rx[LEN_RxOAD]==0)
						{
							LEN_RxOAD+=5;
						}
						else
						{
							i=p8rx[LEN_RxOAD+5];
							i*=4;
							i+=6;
							LEN_RxOAD+=i;
						}
					}
					n=LEN_RxOAD-x;//CSD长度
					LEN_RxOAD<<=16;
					if((n+2)>LENmax_TxSPACE)
					{
						UARTCtrl->DAR=8;//越界（8）
						return LEN_RxOAD;
					}
					MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+x,n);//copy CSD
					LEN_TxAPDU+=n;
					LENmax_TxSPACE-=n;
					p8tx[LEN_TxAPDU]=0;//0=错误
					p8tx[LEN_TxAPDU+1]=4;//对象未定义           （4）
					LEN_TxAPDU+=2;
					return LEN_RxOAD|LEN_TxAPDU;
//---未知  END---
			}
			break;
	}
	
//GetResponseRecord∷=SEQUENCE
//{
//  服务序号-优先级-ACD         PIID-ACD，
//  一个记录型对象属性及其结果  A-ResultRecord
//}
//A-ResultRecord∷=CHOICE
//{
//  错误信息       [0] DAR，
//  记录数据       [1] A-RecordData
//}
//A-RecordData∷=SEQUENCE
//{
//  记录型对象属性描述符    	OAD，
//  一行记录N列属性描述符 	RCSD，
//  M条记录                	SEQUENCE OF A-RecordRow
//}
//A-RecordRow∷=SEQUENCE
//{
//  第1列数据   Data，
//  第2列数据   Data，
//  …
//  第N列数据   Data
//}
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	p8tx[0]=133;//读取响应         	[133]	GET-Response
	p8tx[1]=3;//[3] GeResponseRecord
	p8tx[2]=UARTCtrl->PIID;//PIID-ACD
	p8tx[3]=p8rx[0];//OAD
	p8tx[4]=p8rx[1];
	p8tx[5]=p8rx[2];
	p8tx[6]=p8rx[3];
	LEN_TxAPDU=7;
	LENmax_TxSPACE-=LEN_TxAPDU;
	
	x=LEN_RxOAD;
	n=p8rx[LEN_RxOAD];//SEQUENCE OF CSD个数
	if(n==0)
	{
		//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”。
		p8rx=(u8*)ADDR_1kDATABUFF;
		n=p8rx[0];
		p8rx-=x;
	}
	LEN_RxOAD++;
	while(n--)
	{
		if(p8rx[LEN_RxOAD]==0)
		{
			LEN_RxOAD+=5;
		}
		else
		{
			i=p8rx[LEN_RxOAD+5];
			i*=4;
			i+=6;
			LEN_RxOAD+=i;
		}
	}
	n=LEN_RxOAD-x;//CSD长度
	LEN_RxOAD<<=16;
	if((n+2)>LENmax_TxSPACE)
	{
		if(UARTCtrl->pTxData)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
		{
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
		}
		UARTCtrl->DAR=8;//越界（8）
		return LEN_RxOAD;
	}
	MR((u32)p8tx+LEN_TxAPDU,(u32)p8rx+x,n);//copy CSD
	LEN_TxAPDU+=n;
	LENmax_TxSPACE-=n;
	if(UARTCtrl->pTxData==0)//需发送数据缓冲指针(动态分配),分段组织,每段前有2字节段长度
	{
		//UARTCtrl->DAR=6;//对象不存在           （6）
		//return LEN_RxOAD|LEN_TxAPDU;
		UARTCtrl->LEN_TxData=0;//需发送数据总长度(包括每段前的2字节段长度)
	}
	if(UARTCtrl->DAR)
	{//错误
		ms->ms_free(UARTCtrl->pTxData);
		UARTCtrl->pTxData=0;
		//return LEN_RxOAD|LEN_TxAPDU;
		UARTCtrl->DAR=0;
		UARTCtrl->LEN_TxData=0;//需发送数据总长度(包括每段前的2字节段长度)
	}
	
	p8tx[LEN_TxAPDU]=1;//01 —— 记录数据
	M_Offset=LEN_TxAPDU+1;//M条记录偏移
	LEN_TxAPDU+=2;
	LENmax_TxSPACE-=2;
	LEN_TxData=UARTCtrl->LEN_TxData;//需发送数据总长度(包括每段前的2字节段长度)
	LEN_ReadData=UARTCtrl->LEN_ReadData;
	pDATA=UARTCtrl->pTxData;
	n=0;
	while(1)
	{
		if(LEN_ReadData>=LEN_TxData)
		{
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		if(n>=127)
		{
			break;
		}
		x=pDATA[LEN_ReadData]+(pDATA[LEN_ReadData+1]<<8);
		if(x<2)
		{
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		x-=2;
		if(x>LENmax_TxSPACE)
		{
			break;
		}
		MR((u32)p8tx+LEN_TxAPDU,(u32)pDATA+LEN_ReadData+2,x);
		LEN_TxAPDU+=x;
		LENmax_TxSPACE-=x;
		LEN_ReadData+=x+2;
		n++;//M条记录
	}
	UARTCtrl->LEN_ReadData=LEN_ReadData;
	p8tx[M_Offset]=n;//M条记录
	if(UARTCtrl->pTxData==0)
	{//无下帧
		UARTCtrl->NEXT=0;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	}
	else
	{//有下帧
		UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
		UARTCtrl->NEXTBlock=1;
	}
	return LEN_RxOAD|LEN_TxAPDU;
}


u32 GET_RecordOAD_Next(u32 PORTn)//读一个记录型对象属性分帧响应的下一帧;返回：LEN_TxAPDU
{
	u32 i;
	u32 x;
	u32 n;
	UARTCtrl_TypeDef *UARTCtrl;
	u8* p8;
	u8* p8rx;
	u8 *p8tx;
//	u16* p16;
	u32 LEN_SA;
	u32 LEN_TxAPDU;
	u32 LENmax_TxSPACE;
	u32 M_Offset;
	u32 LEN_TxData;
	u32 LEN_ReadData;
	ms_Type *ms;
	ClientConnectInfo_TypeDef* ClientConnectInfo;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->pTxData==0)
	{
		return 0;
	}
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//上帧TX:68 D4 07 C3 05 01 00 00 00 00 00 10 A9 57 85 05 0D  00 00 01  02 01  60 12 03 00  03 00 60 42 02 00 00 20 2A 02 00 01 50 02 02 00 03 20 21 02 00 00 10 02 00 00 20 02 00 01 18 1C 07 E0 0B 0B 00 00 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 00 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 1C 07 E0 0B 0B 00 0F 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 0F 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 1C 07 E0 0B 0B 00 1E 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 1E 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 1C 07 E0 0B 0B 00 2D 00 55 07 05 00 00 00 00 00 01 01 03 1C 07 E0 0B 0B 00 2D 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 ...
	//copy Tx到Rx
	LEN_SA=p8tx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	x=5+LEN_SA+3+8+4;
	n=p8tx[x];//SEQUENCE OF CSD个数
	x++;
	while(n--)
	{
		if(p8tx[x]==0)
		{//OAD
			x+=5;
		}
		else
		{//ROAD
			i=p8tx[x+5];
			i*=4;
			i+=6;
			x+=i;
		}
	}
	i=Get_LEN_UARTnRx(PORTn);
	if(x>i)
	{
		UARTCtrl->pTxData=0;
		return 0;
	}
	MR((u32)p8rx,(u32)p8tx,x);
	//copy OAD和CSD
	LEN_TxAPDU=x-(5+LEN_SA+3+8);
	MR((u32)p8tx+TxAPDUOffset+3,(u32)p8rx+5+LEN_SA+3+8,LEN_TxAPDU);
	
	p8tx+=TxAPDUOffset;//TxAPDU存放开始地址
	p8tx[0]=133;//读取响应         	[133]	GET-Response
	p8tx[1]=5;//[3] GeResponseRecord,,[5]  GetResponseNext
	p8tx[2]=UARTCtrl->PIID;//PIID-ACD
	//p8tx[3]=p8rx[0];//OAD
	//p8tx[4]=p8rx[1];
	//p8tx[5]=p8rx[2];
	//p8tx[6]=p8rx[3];
	LEN_TxAPDU+=3;
	LENmax_TxSPACE-=LEN_TxAPDU;
	
	p8tx[LEN_TxAPDU]=1;//01 —— 记录数据
	M_Offset=LEN_TxAPDU+1;//M条记录偏移
	LEN_TxAPDU+=2;
	LENmax_TxSPACE-=2;
	LEN_TxData=UARTCtrl->LEN_TxData;
	LEN_ReadData=UARTCtrl->LEN_ReadData;
	p8=UARTCtrl->pTxData;
	n=0;
	while(1)
	{
		if(LEN_ReadData>=LEN_TxData)
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		if(n>=127)
		{
			break;
		}
		x=p8[LEN_ReadData]+(p8[LEN_ReadData+1]<<8);
		if(x<2)
		{
			ms=Get_ms();
			ms->ms_free(UARTCtrl->pTxData);
			UARTCtrl->pTxData=0;
			break;
		}
		x-=2;
		if(x>LENmax_TxSPACE)
		{
			break;
		}
		MR((u32)p8tx+LEN_TxAPDU,(u32)p8+LEN_ReadData+2,x);
		LEN_TxAPDU+=x;
		LENmax_TxSPACE-=x;
		LEN_ReadData+=x+2;
		n++;//M条记录
	}
	UARTCtrl->LEN_ReadData=LEN_ReadData;
	p8tx[M_Offset]=n;//M条记录
	if(UARTCtrl->pTxData==0)
	{//无下帧
		UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	}
	else
	{//有下帧
		UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	}
//	//copy rx到rx+Rxoffset
//	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
//	i=Get_LEN_UARTnRx(PORTn);
//	x=p8rx[1]+(p8rx[2]<<8);
//	x+=3;//整半字
//	x>>=1;
//	x<<=1;
//	if((x+Rxoffset)>i)
//	{
//		x=i-Rxoffset;
//	}
//	x/=2;
//	p16=(u16*)p8rx;
//	while(x)
//	{
//		x--;
//		p16[(Rxoffset/2)+x]=p16[x];
//	}
	return LEN_TxAPDU;
}























