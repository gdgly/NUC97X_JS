
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/DL698_Uart.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"
#include "../Device/IC_TESAM.h"
#include "../DL698/RM_Record.h"

#include "../MS/MS.h"
//void my_printf(char *pStr,u32 param1,u32 param2,u32 param3)
//{
//	ms_Type *ms;
//	ms=Get_ms();
//	ms->link_printf((char *)pStr,param1,param2,param3);
//}

 #include "../DL698/DL698_JSON.h"
u32 RM_RxDATA_DL698_CHECK(u32 PORTn)//DL698帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
{
	u32 i;
	u32 x;
	u32 n;
	u8* p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
#if RM_SECURITY_CompMAC==1//0=否,1=是,安全模式抄表时进行MAC校验码比较
	u8* p8;
	u32 DL698ByteTypeLenByte;
#endif
	u64 d64a;
	u64 d64b;
	u32 y;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	if((p8rx[4]&0x0f)!=5)
	{
		return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
/*
#if ((Project/100)==2)//集中器
#if SEL_RouterRunMode3ADDR==1//微功率无线发送帧目标地址:0=电能表地址，1=采集器地址
	if((PORTn==RS485_4PORT)&&(Terminal_Router->RouterRunMode_1==3))//路由运行模式_通信方式：1表示"窄带电力线载波通信"，2表示"宽带电力线载波通信"，3表示"微功率无线通信"，其它取值保留
	{//微功率无线,目标地址用采集器地址
		i=UARTCtrl->TaskID;
		i>>=8;
		i&=0xff;
		switch(i)
		{
			case RMTASKnoPOWER://停上电事件固定任务号
				i=UARTCtrl->MeterCount[i];
				i=PowerEvent->setno[i];
				break;
			default:
				i=UARTCtrl->MeterCount[i];
				break;
		}
		p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
		p8=Get_Element(p8,3,0,0);//计算元素地址
		if(p8)
		{//采集器可取
			p8+=5;
			x=p8[4]|(p8[5]<<8);
			i=(p8rx[5]<<8)|p8rx[6];
			if(i!=x)
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			x=p8[2]|(p8[3]<<8);
			i=(p8rx[7]<<8)|p8rx[8];
			if(i!=x)
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			x=p8[0]|(p8[1]<<8);
			i=(p8rx[9]<<8)|p8rx[10];
			if(i!=x)
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
		}
		else
		{
			return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
	}
	else
#endif
#endif
*/
	#if (USER/100)==17//江苏系
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;
	if(i==0x22)
	{
		if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
		//if(((UARTCtrl->TaskID>>20)&0xf)==5)
		{
			switch(i)
			{
				case RMTASKnoPOWER://停上电事件固定任务号
					i=UARTCtrl->MeterCount[i];
					i=PowerEvent->setno[i];
					break;
				default:
					i=UARTCtrl->MeterCount[i];
					break;
			}
			d64a=MRR((u32)p8rx+5,6);
			#if 0
			x=Terminal_Router->NoteNum1;//已知总从节点数
			for(y=0;y<x;y++)
			{
				d64b=MRR((u32)&NoteList1->List1[y].CollectorAddr,6);
				if(d64a==d64b)
				{
					break;//原已存在
				}
			}
			if(y==x)
			{
				x=Terminal_Router->NoteNum1;//已知总从节点数
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList1->List1[y].Addr,6);
					if(d64a==d64b)
					{
						break;//原已存在
					}
				}
				if(x==y)
				{
					i=(p8rx[5]<<8)|p8rx[6];
					if(i!=UARTCtrl->RMAddr[2])
					{
						myprintf("[%s:%d]:地址错\n",(u32)&__func__,(u32)__LINE__,0);
						return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
					}
					i=(p8rx[7]<<8)|p8rx[8];
					if(i!=UARTCtrl->RMAddr[1])
					{
						myprintf("[%s:%d]:地址错\n",(u32)&__func__,(u32)__LINE__,0);
						return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
					}
					i=(p8rx[9]<<8)|p8rx[10];
					if(i!=UARTCtrl->RMAddr[0])
					{
						myprintf("[%s:%d]:地址错\n",(u32)&__func__,(u32)__LINE__,0);
						return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
					}
				}	
			}
			#else
			x=Terminal_Router->NoteNum;//已知总从节点数
			for(y=0;y<x;y++)
			{
				d64b=MRR((u32)&NoteList->List[y].CollectorAddr,6);
				if(d64a==d64b)
				{
					break;//原已存在
				}
			}
			if(y==x)
			{
				x=Terminal_Router->NoteNum;//已知总从节点数
				for(y=0;y<x;y++)
				{
					d64b=MRR((u32)&NoteList->List[y].Addr,6);
					if(d64a==d64b)
					{
						break;//原已存在
					}
				}
				if(x==y)
				{
					i=(p8rx[5]<<8)|p8rx[6];
					if(i!=UARTCtrl->RMAddr[2])
					{
						return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
					}
					i=(p8rx[7]<<8)|p8rx[8];
					if(i!=UARTCtrl->RMAddr[1])
					{
						return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
					}
					i=(p8rx[9]<<8)|p8rx[10];
					if(i!=UARTCtrl->RMAddr[0])
					{
						return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
					}
				}	
			}
			#endif
		}
		else
		{
			i=(p8rx[5]<<8)|p8rx[6];
			if(i!=UARTCtrl->RMAddr[2])
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			i=(p8rx[7]<<8)|p8rx[8];
			if(i!=UARTCtrl->RMAddr[1])
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			i=(p8rx[9]<<8)|p8rx[10];
			if(i!=UARTCtrl->RMAddr[0])
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
		}
	}
	#else
	{
		i=(p8rx[5]<<8)|p8rx[6];
		if(i!=UARTCtrl->RMAddr[2])
		{
			return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
		i=(p8rx[7]<<8)|p8rx[8];
		if(i!=UARTCtrl->RMAddr[1])
		{
			return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
		i=(p8rx[9]<<8)|p8rx[10];
		if(i!=UARTCtrl->RMAddr[0])
		{
			return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
	}
	#endif
#if (USER/100)==17//江苏系
	if((p8rx[3]&0x0f)!=0x03)
	{
		return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
#else
	//控制码
	if(p8rx[3]!=0xc3)
	{
		return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
#endif
	//长度
	i=p8rx[1]+(p8rx[2]<<8);
	if(i<=(13+8+3))
	{
		return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
	
	//安全模式读取
	if(p8rx[14]==0x90)
	{
		//明文+MAC方式验证抄读数据
//68 72 00 C3 05 01 00 00 00 00 00 00 3B 06 
//90 安全响应   		[144]	SECURITY-Response
//00 明文应用数据单元   [0]  octet-string
//59 长度89
//85 03 05 50 04 02 00 03 00 20
//21 02 00 00 00 10 02 00 00 00 
//20 02 00 01 01 1C 07 E2 09 07 
//00 00 00 01 05 06 00 00 03 E8 
//06 00 00 01 90 06 00 00 01 2C 
//06 00 00 00 C8 06 00 00 00 64 
//01 05 06 00 00 03 E8 06 00 00 
//01 90 06 00 00 01 2C 06 00 00 
//00 C8 06 00 00 00 64 00 00 
//01 数据验证信息  CHOICE   OPTIONAL=1
//00 数据MAC    [0]  MAC
//04 24 80 09 E0
//AA AC 16
		
 #if RM_SECURITY_CompMAC==1//0=否,1=是,安全模式抄表时进行MAC校验码比较
	#if RM_SECURITY_MeterNo==0//0=否,1=是;DL698安全模式抄表时先抄读电表表号 或 ESAM序列号
		#error
	#endif
	//注!!!:电能表需先做密钥更新关联电能表表号后才能比对通过
	if(PORTn!=RS485_4PORT)
	{//注!!!:载波口测698事件主动上报互换性时不能判MAC
		//先用电能表表号比较
		MR(ADDR_UART7Tx+7,((u32)&UARTCtrl->RM_Rx_date_40020200)+2,8);
		//16byte Rand
		MR(ADDR_UART7Tx+7+8,(u32)&UARTCtrl->Rand,16);
		//Data2(明文APDU)
		DL698ByteTypeLenByte=Get_DL698ByteTypeLenByte(p8rx+16);//得到DL698字节类型用来表示长度单元的字节数
		x=Get_DL698ByteTypeLenValue(p8rx+16);//得到DL698字节类型长度单元的值
		if((x+2+6+16+4)>(LEN_UART7Tx-7))
		{
			return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
		MR(ADDR_UART7Tx+7+8+16,(u32)p8rx+16+DL698ByteTypeLenByte,x);
		//4byte MAC
		n=Get_DL698ByteTypeLenByte(p8rx+16+DL698ByteTypeLenByte+x+2);//得到DL698字节类型用来表示长度单元的字节数
		MR(ADDR_UART7Tx+7+8+16+x,(u32)p8rx+16+DL698ByteTypeLenByte+x+2+n,4);
		x+=8+16+4;
		//发送： 800E4887+LC+电表表号+Rand+Data2+MAC
		//返回： 9000+0000
		i=TESAM_CMD_TxRx(0x800E4887,x,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
		if(i)
		{
			//再用8byte 通信地址(台体测试软件用通信地址计算)
			p8=(u8*)ADDR_UART7Tx+7;
			p8[0]=0;
			p8[1]=0;
			MR(ADDR_UART7Tx+7+2,(u32)&UARTCtrl->RMAddr,6);
			i=TESAM_CMD_TxRx(0x800E4887,x,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
			if(i)
			{
				return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
		}
	}
 #endif
		//
		x=p8rx[16];
		if(x<128)
		{
			MR((u32)p8rx+14,(u32)p8rx+17,x);
		}
		else
		{
			x&=0x3f;//表示长度的字节数
			if(x==1)
			{//1byte长度
				x=p8rx[17];
				MR((u32)p8rx+14,(u32)p8rx+18,x);
			}
			else
			{//2byte长度
				x=(p8rx[17]<<8)|p8rx[18];
				if(x>(LEN_UART2Rx-14))
				{
					return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
				}
				MR((u32)p8rx+14,(u32)p8rx+19,x);
			}
		}
		x+=14+2;//(长度域L  2Byte(除起始字符和结束字符之外的帧字节数))
		x--;
		p8rx[1]=x;
		p8rx[2]=x>>8;
		Uart_698_HCSFCS(p8rx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	}
#if (USER/100)==17//江苏系
	if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
	//if(((UARTCtrl->TaskID>>20)&0xf)==5)
	{
		return 0;
	}
#endif

	//OAD比较
	i=p8rx[15];
	switch(i)
	{
		default:
			return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	#if (USER/100)==0//电科院
		//互换性测试,自动上报终端抄读698新增事件的返回帧有BUG(85 01变为85 10)
		case 0x10:
	#endif
		case 0x01://GetRequestNormal
/*
			//注:单相表曲线用实时数据
			i=UARTCtrl->RMCSD[2+0]<<24;
			i|=UARTCtrl->RMCSD[2+1]<<16;
			i|=UARTCtrl->RMCSD[2+2]<<8;
			i|=UARTCtrl->RMCSD[2+3];
			if(i==0x50020200)
			{//分钟冻结
				i=UARTCtrl->TaskID;
				i=(i>>8)&0xff;//任务号
				i=UARTCtrl->MeterCount[i];//取档案号
				p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
				p8=Get_Element(p8,2,8,0);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8)
				{
					i=p8[1];//取接线方式
					if(i==1)//未知（0），单相（1），三相三线（2），三相四线（3）
					{//单相表曲线,抄实时数据
						i=p8rx[17+0]<<24;
						i|=p8rx[17+1]<<16;
						i|=p8rx[17+2]<<8;
						i|=p8rx[17+3]<<0;
						if(i!=UARTCtrl->RM_DI)
						{
							return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
						}
						break;
					}
				}
			}
*/
			for(i=0;i<4;i++)
			{
				if(p8rx[17+i]!=UARTCtrl->RMCSD[2+i])
				{
					break;
				}
			}
			if(i<4)
			{
				return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			if(p8rx[21]!=1)
			{//0=错误，1=记录数据
				return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			break;
	#if (USER/100)==17//江苏系
		case 2://GetRequestNormalList
		case 6://ReportNotificationSimplifyRecord
			return 0;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	#endif
//		case 2://GetRequestNormalList
//			return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		case 0x03://GeResponseRecord
			for(i=0;i<4;i++)
			{
				if(p8rx[17+i]!=UARTCtrl->RMCSD[2+i])
				{
					return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
				}
			}
			i=p8rx[21];//RCSD，SEQUENCE OF个数
			i*=5;
			if(p8rx[22+i]!=1)
			{//01 —— 记录数据
				return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			if(p8rx[22+i+1]==0)
			{//记录数据个数=0
				return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			i=UARTCtrl->TaskID;
			switch((i>>24)&0xf)//方案类型
			{
				case 2://事件采集方案
					return 0;
				default://普通采集方案
					//比较冻结时标(在最后)
		//68 68 00 C3 05 01 00 00 00 00 00 00 F6 60 85 03 1D    17byte
		//50 02 02 00 03 00 20 21 02 00 00 00 10 02 00 00 00 20 02 00 01 01 1C 07 E1 04 19 00 0F 00 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 01 05 06 00 09 00 B0 06 00 01 D4 C0 06 00 02 22 E0 06 00 01 FB D0 06 00 03 0D 40 00 00 4D 92 16
					i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
					switch(i)
					{
						case 0x5002://分钟冻结(采曲线数据)
						//case 0x5004://日冻结
						//case 0x5006://月冻结
							switch((UARTCtrl->TaskID>>20)&0xf)//采集类型
							{
								default:
								case 0://0=采集当前数据
								case 1://1=采集上第N次
									//不比较
									return 0;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
								case 2://2=按冻结时标采集
								case 3://3=按时标间隔采集
									break;
								case 4://4=RetryMetering 补抄
									break;
							}
							n=UARTCtrl->RMCSD[6];//RCSD，SEQUENCE OF个数
							for(i=0;i<n;i++)
							{
								x=(UARTCtrl->RMCSD[7+(i*4)]<<8)|UARTCtrl->RMCSD[7+(i*4)+1];
								if(x==0x2021)
								{//有数据冻结时间列选择发送
									n=p8rx[21];//RCSD，SEQUENCE OF个数
									for(i=0;i<n;i++)
									{
										x=p8rx[22+(i*5)+1]<<8;
										x|=p8rx[22+(i*5)+2];
										if(x==0x2021)
										{//找到接收冻结时间列选择
											p8rx+=22+(5*n)+2;
											while(i--)
											{
												p8rx+=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
											}
									#if CompareTime_5002==0//抄读分钟冻结(采曲线数据)是否比较冻结时间:0=不比较,1=比较
											p8rx++;
											for(i=0;i<7;i++)//秒不比较
											{
												p8rx[i]=UARTCtrl->date_time_s[i];
											}
											return 0;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
									#else
											if(p8rx[0]!=DataType_date_time_s)
											{//数据为空
												return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
											}
											p8rx++;
											for(i=0;i<6;i++)//秒不比较
											{
												if(p8rx[i]!=UARTCtrl->date_time_s[i])
												{
													break;
												}
											}
											if(i>=6)
											{//冻结时间相同
												return 0;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
											}
											return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
									#endif
										}
									}
									//没找到接收冻结时间列选择
									return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
								}
							}
							break;
					}
					break;
			}
			break;
//		case 4://GetRequestRecordList
//			return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
//		case 5://GetRequestNext
//			
//			break;
	}
	return 0;
}


//UART抄表数据缓冲定义
//#define LEN_RM_DI_CSD     512
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)

//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)

//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的数据长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的数据长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)(ROAD时不写入总关联的OAD个数数组!!!)

u32 RM_RxDATA_DL698(u32 PORTn)//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
{
	//u32 iii;
	u32 iRes;
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 OAD;
	u32 ROAD;
	u32 NUM_RCSD;
	u32 OADrx;
	u32 NUM_RCSDrx;
	u8* p8;
//	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
//	u8* p8Array;
	u32 NUM_RecordRow;
	u32 NUM_AddrData;
	u8* p8RMADDR;
	u32 NUM_ADDR;
	u8* p8Data;
	u16* p16Data;
	u8* p8rx;
	u32 m;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);

	
#if(USER/100)==17//江苏系
	i=p8rx[14];
	if(i==0x88)//采集器有数据上报
	{
		UARTCtrl->res23=0;//一开始赋值为0
		p8Data=(u8 *)ADDR_DATABUFF;
		i=p8rx[15];
		//UARTCtrl->PIID=p8rx[16];
		UARTCtrl->PIID=(p8rx[16]&0x3f);
		x=0;
		switch(i)
		{
			case 6://ReportNotificationSimplifyRecord上报方式
				if(p8rx[21]==7)
				{
					p8RMADDR=(u8*)ADDR_4KWRITE_BUFF;
					if(p8rx[39]==3)//MS:一组用户地址
					{
						NUM_ADDR=p8rx[40];
						for(i=0;i<NUM_ADDR;i++)
						{
							MR((u32)p8RMADDR+i*6,(u32)p8rx+43+i*8,6);//
						}
					}                      
					
					MR((u32)p8Data+8,ADDR_DL698YMDHMS,7);//采集成功时标
					MR((u32)p8Data+15,ADDR_DL698YMDHMS,7);//采集成功时标
					MR((u32)p8Data+22,(u32)p8rx+22,7);//采集存储时标
					
					NUM_RCSD=1;
					NUM_RCSDrx=p8rx[45+NUM_ADDR*8];
					
					p8Data[29]=1;
					p8Data[30]=1;
					p8Data[31]=0x5B;
					p8Data[32]=1;
					MR((u32)p8Data+33,(u32)p8rx+41+NUM_ADDR*8,5+NUM_RCSDrx*4);
					MR((u32)UARTCtrl->RMCSD,(u32)p8Data+31,7+NUM_RCSDrx*4);					
					if(p8rx[41+5+NUM_RCSDrx*4+NUM_ADDR*8]==1)//响应数据choice的[1]A-SimplifyRecord
					{
						if(p8rx[38]==1)
						{
							UARTCtrl->res23=3;//1分钟曲线数据
						}
						else
						{
							UARTCtrl->res23=8;//其他分钟曲线数据
						}
					}
					else if(p8rx[41+5+NUM_RCSDrx*4+NUM_ADDR*8]==0xc8)//DAR=200表示无后续帧
					{
						if(p8rx[38]==1)
						{
							UARTCtrl->res23=4;//1分钟曲线数据
						}
						else
						{
							UARTCtrl->res23=9;//其他分钟曲线数据
						}
					}
					
					LEN_Data0=0;//表示接收到的数据的长度					
					NUM_AddrData=p8rx[47+NUM_RCSDrx*4+NUM_RCSDrx+NUM_ADDR*8+LEN_Data0];
					NUM_AddrData=(NUM_AddrData/NUM_ADDR);
					for(i=0;i<NUM_ADDR;i++)
					{
						MR((u32)p8Data,(u32)p8RMADDR+i*6,6);
						//LEN_Data1=30+NUM_RCSDrx*4+NUM_ADDR*8;//表示存储数据的长度				
						z=NUM_RCSDrx;
						for(j=0;j<NUM_AddrData;j++)
						{
							LEN_Data1=30+NUM_RCSDrx*4+8;//表示存储数据的长度
							for(x=0;x<NUM_RCSDrx;x++)//ROAD中OAD的数量
							{
								OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
								OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
								OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
								OAD|=UARTCtrl->RMCSD[10+(4*x)];
								if(OAD==0x20210200)
								{
									z=x;
									break;
								}
							}
							MR((u32)p8Data+22,(u32)p8rx+22,7);//采集存储时标
							y=j*p8rx[38]*60;
							if(x==NUM_RCSDrx)
							{
								pYYMDHMS_AddS_hex(p8Data+22,y);//采集存储时标
							}
							for(x=0;x<NUM_RCSDrx;x++)
							{
								y=p8rx[42+5+NUM_RCSDrx*4+x+NUM_ADDR*8];
								p8Data[LEN_Data1]=y;
								y=Get_DL698DataLen_S(p8rx+47+NUM_RCSDrx*4+x+NUM_ADDR*8,0);
								m=Deal_DataNull_JS(p8Data[LEN_Data1],p8rx+42+5+NUM_RCSDrx*4+NUM_ADDR*8+NUM_RCSDrx+LEN_Data0+1);
								if(m==1)
								{
									p8Data[LEN_Data1]=0;
									LEN_Data0+=(y-1);
									LEN_Data1+=1;
								}
								else
								{
									MR((u32)p8Data+LEN_Data1+1,(u32)p8rx+42+5+NUM_RCSDrx*4+NUM_ADDR*8+NUM_RCSDrx+LEN_Data0+1,y-1);
									if(z!=NUM_RCSDrx)//冻结时间存储成采集存储时标
									{
										MR((u32)p8Data+22,(u32)p8rx+42+5+NUM_ADDR*8+NUM_RCSDrx*4+NUM_RCSDrx+LEN_Data0+1,y-1);
										z=NUM_RCSDrx;
									}
									LEN_Data0+=(y-1);
									LEN_Data1+=y;
								}
							}
							p8Data[6]=(LEN_Data1-8)&0xff;
							p8Data[7]=(LEN_Data1-8)>>8;
							RM_RecordFile(PORTn,p8Data);
							iRes=UARTCtrl->res23;
							switch(iRes)
							{
								case 3:
								case 4:
									UARTCtrl->res23=12;
									RM_RecordFile(PORTn,p8Data);
									UARTCtrl->res23=iRes;
									break;
								case 8:
								case 9:
									UARTCtrl->res23=13;
									RM_RecordFile(PORTn,p8Data);
									UARTCtrl->res23=iRes;
									break;
								default:
									return 0;	
							}
						
						}
					}
					return 0;
				}
				break;
			case 2://上报若干个记录型对象属性
				//UARTCtrl->res23=2;
				if(p8rx[22]!=1)
					return 1;
				MR((u32)p8Data,(u32)p8rx+5,6);
				Data_Inverse((u32)p8Data,6);
				if((p8rx[24]==0x50)&&(p8rx[25]==0x04))
				{
					UARTCtrl->res23=1;//有后续帧，日冻结
					i=p8rx[28];
					if(p8rx[i*4+28+1]==0)
					{
						UARTCtrl->res23=6;//无后续帧，日冻结
					}
				}
				else if((p8rx[24]==0x50)&&(p8rx[25]==0x06))
				{
					UARTCtrl->res23=2;//有后续帧，月冻结
					i=p8rx[28];
					if(p8rx[i*4+28+1]==0)
					{
						UARTCtrl->res23=7;//无后续帧，月冻结
					}
				}
				else
					return 1;	
				
				NUM_RCSD=p8rx[23];
				p8Data[29]=1;
				p8Data[30]=1;
				p8Data[31]=0x5B;
				p8Data[32]=1;
				NUM_RCSDrx=p8rx[28];
				MR((u32)p8Data+33,(u32)p8rx+24,5+NUM_RCSDrx*4);
				MR((u32)p8Data+8,ADDR_DL698YMDHMS,7);//采集启动时标
				MR((u32)p8Data+15,ADDR_DL698YMDHMS,7);//采集成功时标
				MR((u32)p8Data+22,ADDR_DL698YMDHMS,7);//采集成功时标

				UARTCtrl->RMCSD[1]=p8rx[23];
				if(UARTCtrl->RMCSD[1]==1)
				{//ROAD
					MR((u32)UARTCtrl->RMCSD,(u32)p8Data+31,7+NUM_RCSDrx*4);
				}	
				NUM_RecordRow=p8rx[NUM_RCSDrx*4+28+2];
				p8=p8rx+24+(4*NUM_RCSDrx)+7;
				LEN_Data1=0;
				//p8+=2;
				while(NUM_RecordRow--)//记录条数
				{
					y=p8[1];
					LEN_Data1=38+NUM_RCSDrx*4;//表示存储数据的长度
					LEN_Data0=2;
					for(x=0;x<y;x++)//ROAD中OAD的数量
					{
						OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
						OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
						OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
						OAD|=UARTCtrl->RMCSD[10+(4*x)];
						if(OAD==0x20210200)
						{
							i=Get_DL698DataLen_S(p8+LEN_Data0,0);
							MR((u32)p8Data+22,(u32)p8+1+LEN_Data0,7);
							MR((u32)p8Data+LEN_Data1,(u32)p8+LEN_Data0,8);
							pYYMDHMS_SubS_hex(p8Data+22,60);
							pYYMDHMS_SubS_hex(p8Data+LEN_Data1+1,60);
							LEN_Data0+=i;
							LEN_Data1+=i;
							continue;
						}
						if(OAD==0x10100200)
						{
							if((UARTCtrl->res23==1)||(UARTCtrl->res23==6))
								UARTCtrl->res23=10;
							else if((UARTCtrl->res23==2)||(UARTCtrl->res23==7))
								UARTCtrl->res23=11;
						}
						i=Get_DL698DataLen_S(p8+LEN_Data0,0);
						MR((u32)p8Data+LEN_Data1,(u32)p8+LEN_Data0,i);
						LEN_Data1+=i;
						LEN_Data0+=i;
					}
					p8Data[6]=(LEN_Data1-8)&0xff;
					p8Data[7]=(LEN_Data1-8)>>8;
					RM_RecordFile(PORTn,p8Data);
					p8+=LEN_Data0;
				}			
				return 0;
		}
	}
	else if(i==0x87)
	{
		if(p8rx[21]==0x06)//采集器无数据响应
		{
			UARTCtrl->res23=5;
			return 0;
		}
		if(p8rx[21]==0)
		{
			UARTCtrl->res23=5;
			return 0;
		}
	}
#endif
	
	
//	p8Array=(u8*)p8Data;
//	LEN_Data0=p16Data[0];
//	if(LEN_RxDataBuff<LEN_Data0)
//	{//错
//		return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
//	}
//	LEN_RxDataBuff-=LEN_Data0;
	
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲长度
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8Data+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//错
		return 3;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data+=LEN_RM_DI_CSD+4+LEN_Data1;
	
	//无数据(空数据)
	if(UARTCtrl->RM_Rx_date_NULL!=0)//抄表接收返回:0=有数据,!=0无数据(空数据)
	{
		if(UARTCtrl->RMCSD[1]==0)
		{//OAD
			LEN_Rx=1;
		}
		else
		{//ROAD
			LEN_Rx=UARTCtrl->RMCSD[6];
		}	
		if(LEN_RxDataBuff<LEN_Rx)
		{//缓冲长度不够
			return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		for(i=0;i<LEN_Rx;i++)
		{
			p8Data[i]=0;
		}
		p16Data[0]=LEN_Rx;
		return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	
	LEN_Rx=p8rx[1]+(p8rx[2]<<8);
	#if(USER/100)==15//安徽
	if(LEN_Rx<(13+8+3))
	{//长度<x
		return 0;
	}
	if(LEN_Rx==(13+8+3))
	{//长度==x 空数据
		if(UARTCtrl->RMCSD[1]==0)
		{//OAD
			LEN_Rx=1;
		}
		else
		{//ROAD
			LEN_Rx=UARTCtrl->RMCSD[6];
		}	
		if(LEN_RxDataBuff<LEN_Rx)
		{//缓冲长度不够
			return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		for(i=0;i<LEN_Rx;i++)
		{
			p8Data[i]=0;
		}
		p16Data[0]=LEN_Rx;
		return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数

	}
	#else
	if(LEN_Rx<=(13+8+3))
	{//长度<x
		return 0;
	}
	#endif
	
	i=p8rx[15];
	switch(i)
	{
		default:
			if(UARTCtrl->RMCSD[1]==0)
			{//OAD
				LEN_Rx=1;
			}
			else
			{//ROAD
				LEN_Rx=UARTCtrl->RMCSD[6];
			}	
			if(LEN_RxDataBuff<LEN_Rx)
			{//缓冲长度不够
				return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			for(i=0;i<LEN_Rx;i++)
			{
				p8Data[i]=0;
			}
			p16Data[0]=LEN_Rx;
			return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		case 1://GetRequestNormal
			LEN_Rx=Get_DL698DataLen_S(p8rx+22,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if(LEN_RxDataBuff<LEN_Rx)
			{//缓冲长度不够
				return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			p8rx+=22;
			if(UARTCtrl->RM_Rx_date_40020200[0]==1)//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
			{
//68 23 00 C3 05 01 00 00 00 00 00 00 EA EF 85 01 01 40 02 02 00 01 09 08 00 00 00 00 00 00 00 00 00 00 66 5F 16
				MC(0,((u32)&UARTCtrl->RM_Rx_date_40020200)+2,8);
				i=p8rx[1];//表号长度
				if(i>8)
				{
					i=8;
				}
				UARTCtrl->RM_Rx_date_40020200[1]=i;
				x=8-i;
				MR(((u32)&UARTCtrl->RM_Rx_date_40020200)+2+x,(u32)p8rx+2,i);
				UARTCtrl->RM_Rx_date_40020200[0]=2;//抄表接收到的数据用于MAC验证的电能表表号;0th=状态(0=空,1=正在抄,2=完成),1th=表号长度(<=6),2th-7th表号值
				UARTCtrl->RMCSDSubCount--;//抄表CSD子计数
				return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			for(i=0;i<LEN_Rx;i++)
			{
				p8Data[i]=p8rx[i];
			}
			p16Data[0]=LEN_Rx;
			
			//检查跟随上报
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{
				//若干个对象属性及其数据        [1]  SEQUENCE OF A-ResultNormal，
				//若干个记录型对象属性及其数据  [2]  SEQUENCE OF A-ResultRecord
				p8rx+=LEN_Rx;
				if(p8rx[0]!=0)
				{//有跟随上报
				#if EnMeterAutoReportEvent==1//允许电能表主动上报事件:0=不允许,1=允许
					if(UARTCtrl->AutoReportTask==0)//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
					{
						UARTCtrl->AutoReportTask=1;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
						UARTCtrl->AutoReportRMprotocol=3;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
						MW((u32)&UARTCtrl->RMAddr,(u32)&UARTCtrl->AutoReportRMAddr,6);
					}
				#endif
				}
			}
			break;
//		#if (USER/100)==17//江苏系
//			case 2://GetResponseNormalList-----江苏抄表日数据采用GetResponseNormalList方式应答
//				ROAD=UARTCtrl->RMCSD[1];
//				if(ROAD==1)
//				{//ROAD
//					ROAD=UARTCtrl->RMCSD[2+0]<<24;
//					ROAD|=UARTCtrl->RMCSD[2+1]<<16;
//					ROAD|=UARTCtrl->RMCSD[2+2]<<8;
//					ROAD|=UARTCtrl->RMCSD[2+3];
//				}
//				NUM_RCSD=UARTCtrl->RMCSD[6];
//				NUM_RCSDrx=p8rx[17];//RCSD，SEQUENCE OF个数
//				p16Data[0]=0;
//				for(x=0;x<NUM_RCSD;x++)
//				{
//					OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
//					OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
//					OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
//					OAD|=UARTCtrl->RMCSD[10+(4*x)];
//					LEN_Data0=0;
//					for(y=0;y<NUM_RCSDrx;y++)
//					{
//							OADrx=p8rx[18+LEN_Data0]<<24;
//							OADrx|=p8rx[18+LEN_Data0+1]<<16;
//							OADrx|=p8rx[18+LEN_Data0+2]<<8;
//							OADrx|=p8rx[18+LEN_Data0+3];
//							if(OAD==OADrx)
//							{
//								break;
//							}
//							i=p8rx[18+LEN_Data0+4];
//							if(i==0)
//								LEN_Data0+=2;
//							else
//							{
//								i=Get_DL698DataLen_S(p8rx+18+LEN_Data0+5,0);
//								LEN_Data0+=i;
//							}
//								
//					}
//					if(y>=NUM_RCSDrx)
//					{//没找到列选择OAD
//						if(LEN_RxDataBuff<1)
//						{//缓冲长度不够
//							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
//						}
//						p8Data[0]=0;
//						p8Data+=1;
//						LEN_RxDataBuff-=1;
//						p16Data[0]+=1;
//					}
//					else
//					{
//						i=p8rx[18+LEN_Data0+4];
//						if(i==0)
//							LEN_Data0+=2;
//						else
//						{
//							i=Get_DL698DataLen_S(p8rx+18+LEN_Data0+5,0);
//							MW((u32)p8rx+18+LEN_Data0+5,(u32)p8Data,i);
//							p8Data+=i;
//							LEN_RxDataBuff-=i;
//							p16Data[0]+=i;
//						}
//					}
//				}
//				//检查跟随上报
//				if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
//				{
//					p8=p8rx+18+LEN_Data0;
//					while(NUM_RCSDrx--)
//					{
//						i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
//						p8+=i;
//					}
//					if(p8[0]!=0)
//					{//有跟随上报
//					#if EnMeterAutoReportEvent==1//允许电能表主动上报事件:0=不允许,1=允许
//						if(UARTCtrl->AutoReportTask==0)//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
//						{
//							UARTCtrl->AutoReportTask=1;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
//							UARTCtrl->AutoReportRMprotocol=3;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
//							MW((u32)&UARTCtrl->RMAddr,(u32)&UARTCtrl->AutoReportRMAddr,6);
//						}
//					#endif
//					}
//				}
//				break;
//		#endif
		case 2://GetRequestNormalList

			break;
		case 3://GeResponseRecord
		#if CompareMeterEventCount==1//全事件采集抄表是否比较发生次数:0=不比较(全部抄表数据写入文件),1=比较(只有次数变化的事件写入文件)
			i=UARTCtrl->TaskID;
			if(((i>>24)&0xf)==2)
			{//事件采集方案
				i=Get_RxDATA_DL698_MeterEventCount(PORTn);//从698读事件记录接收帧中取出电能表事件发生次数(事件记录序号);返回:发生次数,没找到时返回0xffffffff
				if(i==0xffffffff)
				{
					return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				OAD=UARTCtrl->RMCSD[2+0]<<24;
				OAD|=UARTCtrl->RMCSD[2+1]<<16;
				OAD|=UARTCtrl->RMCSD[2+2]<<8;
				OAD|=UARTCtrl->RMCSD[2+3];
				x=UARTCtrl->TaskID;
				x>>=8;
				x&=0xff;
				x=UARTCtrl->MeterCount[x];//2*RMTASKmax byte各任务已抄电能表计数,主动上报时用于文件中已上报电能表计数
				i=CompSourMeterEventCount(x,OAD,i);//比较原电能表事件次数;入口:SetNo=档案配置号(0开始),Count=当前次数;返回:0=初始值,1=次数相同,2=次数不同,同时修改原计数,使用ADDR_DATABUFF
				if(i!=2)
				{
					return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
			}
		#endif
			ROAD=UARTCtrl->RMCSD[1];
			if(ROAD==1)
			{//ROAD
				ROAD=UARTCtrl->RMCSD[2+0]<<24;
				ROAD|=UARTCtrl->RMCSD[2+1]<<16;
				ROAD|=UARTCtrl->RMCSD[2+2]<<8;
				ROAD|=UARTCtrl->RMCSD[2+3];
			}
			NUM_RCSD=UARTCtrl->RMCSD[6];
			NUM_RCSDrx=p8rx[21];//RCSD，SEQUENCE OF个数
			if(NUM_RCSD==0)
			{//当无一个OAD时，RCSD=0，即SEQUENCE OF的数据项个数为0，表示“不选择（即全选）”
				//为方便计算将接收的CSD复制作为发送选择的CSD
				x=NUM_RCSDrx*4;
				if((x+7)<sizeof(UARTCtrl->RMCSD))
				{
					UARTCtrl->RMCSD[6]=NUM_RCSDrx;
					for(y=0;y<NUM_RCSDrx;y++)
					{
						UARTCtrl->RMCSD[7+(4*y)]=p8rx[22+(y*5)+1];
					}
				}
			}
			p16Data[0]=0;
			for(x=0;x<NUM_RCSD;x++)
			{
				OAD=UARTCtrl->RMCSD[7+(4*x)]<<24;
				OAD|=UARTCtrl->RMCSD[8+(4*x)]<<16;
				OAD|=UARTCtrl->RMCSD[9+(4*x)]<<8;
				OAD|=UARTCtrl->RMCSD[10+(4*x)];
				for(y=0;y<NUM_RCSDrx;y++)
				{
					if(p8rx[22+(y*5)+0]==0)
					{
						OADrx=p8rx[22+(y*5)+1]<<24;
						OADrx|=p8rx[22+(y*5)+2]<<16;
						OADrx|=p8rx[22+(y*5)+3]<<8;
						OADrx|=p8rx[22+(y*5)+4];
						if(OAD==OADrx)
						{
							break;
						}
					}
				}
				if(y>=NUM_RCSDrx)
				{//没找到列选择OAD
					if(LEN_RxDataBuff<1)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data[0]=0;
					p8Data+=1;
					LEN_RxDataBuff-=1;
					p16Data[0]+=1;
				}
				else
				{//找到列选择OAD
					p8=p8rx+22+(5*NUM_RCSDrx)+2;
					while(y--)
					{
						i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
						p8+=i;
					}
					y=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
//				#if CompareTime_5004==1//抄读日冻结是否比较冻结时间:0=不比较,1=比较
//						if(ROAD==0x50040200)
//						{//日冻结
//							if(OAD==0x20210200)
//							{//数据冻结时间
//								for(i=0;i<4;i++)
//								{
//									if(p8[1+i]!=Comm_Ram->DL698YMDHMS[i])
//									{
//										break;
//									}
//								}
//								if(i<4)
//								{//年月日不同
//									p16Data[0]=0;
//									return 0;
//								}
//							}
//						}
//				#endif

				//#if (USER==901)//河南版
					if(ROAD==0x50020200)
					{//分钟冻结(曲线)
						switch(OAD)
						{
							//方案里是”00100201“和”00200201“，主站用”00100200“和”00200200“召读，费率电量可以是空，只把总电量返回就行了
							case 0x00100201://正向有功电能总
							case 0x00200201://反向有功电能总
							case 0x00300201://组合无功1电能总
							case 0x00400201://组合无功2电能总
							case 0x00500201://第1象限无功电能总
							case 0x00600201://第2象限无功电能总
							case 0x00700201://第3象限无功电能总
							case 0x00800201://第4象限无功电能总
								UARTCtrl->RMCSD[10+(4*x)]=0;
								if(LEN_RxDataBuff<2)
								{//缓冲长度不够
									return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Data[0]=DataType_array;
								p8Data[1]=5;
								p8Data+=2;
								LEN_RxDataBuff-=2;
								p16Data[0]+=2;
								
								if(LEN_RxDataBuff<(y+4))
								{//缓冲长度不够
									return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								if(y<=1)
								{//无数据
									if(LEN_RxDataBuff<5)
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data[0]=0;
									y=1;
								}
								else
								{
									for(i=0;i<y;i++)
									{
										p8Data[i]=p8[i];
									}
								}
								p8Data[y]=0;//费率1
								p8Data[y+1]=0;//费率2
								p8Data[y+2]=0;//费率3
								p8Data[y+3]=0;//费率4
								p8Data+=y+4;
								LEN_RxDataBuff-=y+4;
								p16Data[0]+=y+4;
								continue;
							case 0x20000201://A相电压
							case 0x20010201://A相电流
								UARTCtrl->RMCSD[10+(4*x)]=0;
								if(LEN_RxDataBuff<2)
								{//缓冲长度不够
									return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Data[0]=DataType_array;
								p8Data[1]=3;
								p8Data+=2;
								LEN_RxDataBuff-=2;
								p16Data[0]+=2;
								
								if(LEN_RxDataBuff<(y+2))
								{//缓冲长度不够
									return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								if(y<=1)
								{//无数据
									if(LEN_RxDataBuff<3)
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data[0]=0;
									y=1;
								}
								else
								{
									for(i=0;i<y;i++)
									{
										p8Data[i]=p8[i];
									}
								}
								p8Data[y]=0;//B相空
								p8Data[y+1]=0;//C相空
								p8Data+=y+2;
								LEN_RxDataBuff-=y+2;
								p16Data[0]+=y+2;
								continue;
							case 0x20040202://A相有功功率
							case 0x20050202://A相无功功率
							case 0x200A0202://A相功率因数
								UARTCtrl->RMCSD[10+(4*x)]=0;
								if(LEN_RxDataBuff<2)
								{//缓冲长度不够
									return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Data[0]=DataType_array;
								p8Data[1]=1;
								p8Data+=2;
								LEN_RxDataBuff-=2;
								p16Data[0]+=2;
								
								if(LEN_RxDataBuff<y)
								{//缓冲长度不够
									return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								if(y<=1)
								{//无数据
									if(LEN_RxDataBuff<1)
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data[0]=0;
									y=1;
								}
								else
								{
									for(i=0;i<y;i++)
									{
										p8Data[i]=p8[i];
									}
								}
								p8Data+=y;
								LEN_RxDataBuff-=y;
								p16Data[0]+=y;
								continue;
						}
					}
				//#endif
					if(y<=1)
					{//无数据
						if(LEN_RxDataBuff<1)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_RxDataBuff-=1;
						p16Data[0]+=1;
						continue;
					}
					if(LEN_RxDataBuff<y)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					for(i=0;i<y;i++)
					{
						p8Data[i]=p8[i];
					}
					p8Data+=y;
					LEN_RxDataBuff-=y;
					p16Data[0]+=y;
				}
			}
			
			//检查跟随上报
			if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
			{
				p8=p8rx+22+(5*NUM_RCSDrx)+2;
				while(NUM_RCSDrx--)
				{
					i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					p8+=i;
				}
				if(p8[0]!=0)
				{//有跟随上报
				#if EnMeterAutoReportEvent==1//允许电能表主动上报事件:0=不允许,1=允许
					if(UARTCtrl->AutoReportTask==0)//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
					{
						UARTCtrl->AutoReportTask=1;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
						UARTCtrl->AutoReportRMprotocol=3;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
						MW((u32)&UARTCtrl->RMAddr,(u32)&UARTCtrl->AutoReportRMAddr,6);
					}
				#endif
				}
			}
			break;
//		case 4://GetRequestRecordList
//			
//			break;
//		case 5://GetRequestNext
//			
//			break;
	}
	return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
}























