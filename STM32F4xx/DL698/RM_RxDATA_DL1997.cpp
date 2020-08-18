
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RxDATA.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"



u32 RM_RxDATA_DL1997_CHECK(u32 PORTn)//DL645_1997帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
{
	u32 i;
	u32 x;
	u8 * p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
	u8* p8;
	
	p8=p8;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
			i=(p8rx[1]<<8)|p8rx[2];
			if(i!=x)
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			x=p8[2]|(p8[3]<<8);
			i=(p8rx[3]<<8)|p8rx[4];
			if(i!=x)
			{
				return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
			}
			x=p8[0]|(p8[1]<<8);
			i=(p8rx[5]<<8)|p8rx[6];
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
	{
		i=(p8rx[1]<<8)|p8rx[2];
		if(i!=UARTCtrl->RMAddr[2])
		{
			return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
		i=(p8rx[3]<<8)|p8rx[4];
		if(i!=UARTCtrl->RMAddr[1])
		{
			return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
		i=(p8rx[5]<<8)|p8rx[6];
		if(i!=UARTCtrl->RMAddr[0])
		{
			return 1;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
		}
	}
	//控制码
	if((p8rx[8]&0x40)!=0x0)
	{//从站异常回答
		return 2;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
	//减0x33
	for(i=0;i<p8rx[9];i++)
	{
		p8rx[10+i]-=0x33;
	}
	//DI比较
	x=p8rx[10];
	x|=p8rx[10+1]<<8;
	if(x!=UARTCtrl->RM_DI)
	{
		return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
	return 0;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
}

__align(4) const u32 signed_DL1997_List[]=//DL/T-1997数据带有正负符号的数据标识列表
{
//变量
	0xB62F,//电流
	0xB63F,//瞬时有功功率
	0xB64F,//瞬时无功功率
	0xB65F,//功率因数
};

u32 signed_DL1997_DI(u32 PORTn)//DL/T-1997数据带有正负符号的数据标识;入口:UARTCtrl->RM_DI;返回:0=没符号，1=有符号
{
	u32 i;
	u32 x;
	u32 RMDI1;
	u32 RMDI0;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->RM_DI;
	RMDI0=i&0xff;
	RMDI1=(i>>8);
	for(i=0;i<(sizeof(signed_DL1997_List)/4);i++)
	{
		x=signed_DL1997_List[i];
		x>>=8;
		if(RMDI1==x)
		{
				x=signed_DL1997_List[i];
				x&=0xff;
				if(x==RMDI0)
				{
					return 1;
				}
		}
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
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)

u32 RxDATA_Process_DL1997(u32 PORTn,u32 Byte_DATAper,u32 DPbit,u32 DataType)
{	
	u32 i;
	u32 j;
	u32 n;
	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u32 Byte;
	u8* p8CSD;
	u8* p8Array;
	u8* p8Data;
	u8* p8rx;
	u16* p16Data;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_Rx=p8rx[9];
	if(LEN_Rx<=2)
	{//长度<2
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲长度
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//错
		return 3;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data=(u8*)(p8CSD+LEN_RM_DI_CSD+4+LEN_Data1);
	p8Array=(u8*)p8Data;
	if(UARTCtrl->RMCSDSubCount==0)//抄表CSD子计数(非DL698抄表时)
	{
		p16Data[0]=0;//已填写入buff但还没完整一个CSD的数据长度(不包括本身2字节)
	}
	LEN_Data0=p16Data[0];
	if(LEN_RxDataBuff<LEN_Data0)
	{//错
		return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	LEN_RxDataBuff-=LEN_Data0;
	
	if(LEN_Data0==0)//抄表CSD子计数(非DL698抄表时)
	{
		if(UARTCtrl->RMCSDSubCount!=0)
		{
			UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
			p16Data[0]=0;
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		if(LEN_RxDataBuff<2)
		{//缓冲长度不够
			return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		p8Data+=LEN_Data0;
		p8Array=(u8*)p8Data;
		p8Array[0]=DataType_array;
		p8Array[1]=0;
		LEN_Data0+=2;
		LEN_RxDataBuff-=2;
		p8Data+=LEN_Data0;
	}
	else
	{
		if(p8Array[1]!=UARTCtrl->RMCSDSubCount)
		{
			UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
			p16Data[0]=0;
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		p8Data+=LEN_Data0;
	}
	
	p8rx+=12;
	LEN_Rx-=2;
	n=LEN_Rx/Byte_DATAper;//数组个数=数据长度除以每个数据的字节数
	for(i=0;i<n;i++)
	{
		Data=0;
		for(Byte=0;Byte<Byte_DATAper;Byte++)
		{
			Data|=p8rx[0]<<(Byte*8);
			p8rx++;
		}
		switch(Byte_DATAper)
		{
			case 2:
				if(Data==0xffff)
				{
					Data=0;
				}
				break;
			case 3:
				if(Data==0xffffff)
				{
					Data=0;
				}
				break;
			default:
				break;
		}		
		switch(DataType)
		{
			case DataType_long_unsigned:
				Data<<=DPbit;//645小数位跟698不一致							
				Data=bcd_hex(Data);
				if(LEN_RxDataBuff<3)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Data[0]=DataType_long_unsigned;
				p8Data[1]=Data>>8;
				p8Data[2]=Data;
				p8Data+=3;
				LEN_Data0+=3;
				LEN_RxDataBuff-=3;
				break;
			case DataType_long:
				Data<<=DPbit;//645小数位跟698不一致							
				j=Data&(1<<((Byte_DATAper*8)-1));
				if(j)
				{//负
					Data&=~(1<<((Byte_DATAper*8)-1));//去符号位
					Data=bcd_hex(Data);
					Data=~Data;
					Data++;
				}
				else
				{
					Data=bcd_hex(Data);
				}
				if(LEN_RxDataBuff<3)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Data[0]=DataType_long;
				p8Data[1]=Data>>8;
				p8Data[2]=Data;
				p8Data+=3;
				LEN_Data0+=3;
				LEN_RxDataBuff-=3;
				break;
			case DataType_double_long:
				Data<<=DPbit;//645小数位跟698不一致							
				j=Data&(1<<((Byte_DATAper*8)-1));
				if(j)
				{//负
					Data&=~(1<<((Byte_DATAper*8)-1));//去符号位
					Data=bcd_hex(Data);
					Data=~Data;
					Data++;
				}
				else
				{
					Data=bcd_hex(Data);
				}
				if(LEN_RxDataBuff<5)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Data[0]=DataType_double_long;
				p8Data[1]=Data>>24;
				p8Data[2]=Data>>16;
				p8Data[3]=Data>>8;
				p8Data[4]=Data;
				p8Data+=5;
				LEN_Data0+=5;
				LEN_RxDataBuff-=5;
				break;
			case DataType_double_long_unsigned:
				Data<<=DPbit;//645小数位跟698不一致							
				Data=bcd_hex(Data);
				if(LEN_RxDataBuff<5)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Data[0]=DataType_double_long;
				p8Data[1]=Data>>24;
				p8Data[2]=Data>>16;
				p8Data[3]=Data>>8;
				p8Data[4]=Data;
				p8Data+=5;
				LEN_Data0+=5;
				LEN_RxDataBuff-=5;
				break;
			default:
				break;
		}
		if(p8Array[1]>=127)
		{
			return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		p8Array[1]++;//增加数组数
	}
	p16Data[0]=LEN_Data0;
	return 0;
}

u32 RM_RxDATA_DL1997(u32 PORTn)//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
{
	u32 i;
	u32 OI;
	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u32 signedDI;
	u8* p8CSD;
	u8* p8Array;
	u8* p8Data;
	u16* p16Data;
	u8* p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 n;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_Rx=p8rx[9];
	if(LEN_Rx<=2)
	{//长度<2
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲长度
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//错
		return 3;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data=(u8*)(p8CSD+LEN_RM_DI_CSD+4+LEN_Data1);
	p8Array=(u8*)p8Data;
	if(UARTCtrl->RMCSDSubCount==0)//抄表CSD子计数(非DL698抄表时)
	{
		p16Data[0]=0;//已填写入buff但还没完整一个CSD的数据长度(不包括本身2字节)
	}
	LEN_Data0=p16Data[0];
	if(LEN_RxDataBuff<LEN_Data0)
	{//错
		return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	LEN_RxDataBuff-=LEN_Data0;
	
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
		UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
		UARTCtrl->RMROADCount=UARTCtrl->RMROADCountMax;
		return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	
//---曲线开始---
	//用实时数据冻结为曲线
	i=UARTCtrl->RMCSD[1];
	if(i)
	{//ROAD
		OI=UARTCtrl->RMCSD[2+0]<<24;
		OI|=UARTCtrl->RMCSD[2+1]<<16;
		OI|=UARTCtrl->RMCSD[2+2]<<8;
		OI|=UARTCtrl->RMCSD[2+3];
		if(OI==0x50020200)
		{//分钟冻结
//			p8Data+=LEN_Data0;
//			p8Array=(u8*)p8Data;
			p8rx+=12;
			n=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
			OI=(UARTCtrl->RMCSD[7+(4*n)+0]<<24)+(UARTCtrl->RMCSD[7+(4*n)+1]<<16)+(UARTCtrl->RMCSD[7+(4*n)+2]<<8)+(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{
				case 0x20210200://冻结时标
					if(LEN_RxDataBuff<8)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//得到date_time_s共7字节
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					p16Data[0]=LEN_Data0;
					break;
				case 0x00100201://正向有功总电能
				case 0x00200201://反向有功总电能
				case 0x00500201://一象限无功总电能
				case 0x00600201://二象限无功总电能
				case 0x00700201://三象限无功总电能
				case 0x00800201://四象限无功总电能
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					if(LEN_RxDataBuff<(2+5+4))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=5;//河南按块召读，数据个数填5，分费率数值填空
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16)|(p8rx[3]<<24);
					signedDI=0;
					if(signedDI==0)
					{
						p8Data[0]=DataType_double_long_unsigned;
						Data=bcd_hex(Data);
					}
					else
					{
						p8Data[0]=DataType_double_long;
						if(Data>>31)
						{//负
							Data<<=1;
							Data>>=1;
							Data=bcd_hex(Data);
							Data=~Data;
							Data+=1;
						}
						else
						{
							Data=bcd_hex(Data);
						}
					}
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data[5]=0;//河南按块召读，分费率数值填空
					p8Data[6]=0;
					p8Data[7]=0;
					p8Data[8]=0;
					p8Data+=9;
					LEN_Data0+=9;
					LEN_RxDataBuff-=9;
					p16Data[0]=LEN_Data0;
					break;
				case 0x00300200://组合无功1电能
				case 0x00400200://组合无功2电能
					RxDATA_Process_DL1997(PORTn,4,0,DataType_double_long_unsigned);
					break;
				case 0x20170200://有功需量
				case 0x20180200://无功需量
				case 0x11100200://冻结周期内正向有功最大需量 
					break;
				case 0x20000200://电压
				case 0x20000201://A电压
					RxDATA_Process_DL1997(PORTn,2,4,DataType_long_unsigned);
					break;
				case 0x20010200://电流
				case 0x20010201://A电流
					RxDATA_Process_DL1997(PORTn,2,4,DataType_double_long);
					break;
				case 0x20010400://零线电流
					//零线电流无数组
					break;
				case 0x20040200://有功功率
				case 0x20040202://A相有功功率
					RxDATA_Process_DL1997(PORTn,3,0,DataType_double_long);
					break;
				case 0x20050200://无功功率
				case 0x20050202://A相无功功率
					RxDATA_Process_DL1997(PORTn,2,0,DataType_double_long);
					break;
				case 0x200A0200://功率因数
				case 0x200A0202://A相功率因数
					RxDATA_Process_DL1997(PORTn,2,0,DataType_long);
					break;
				default:
					if(LEN_RxDataBuff<1)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data[0]=0;
					p8Data+=1;
					LEN_Data0+=1;
					LEN_RxDataBuff-=1;
					p16Data[0]=LEN_Data0;
					break;
			}
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
	}
//---曲线结束---	
	
	signedDI=signed_DL1997_DI(PORTn);//DL/T-1997数据带有正负符号的数据标识;入口:UARTCtrl->RM_DI;返回:0=没符号，1=有符号
	i=UARTCtrl->RMCSD[1];//当前正在抄的CSD(有数据类型)
	if(i==0)
	{//OAD
		OI=UARTCtrl->RMCSD[2]<<8;//当前正在抄的CSD
		OI|=UARTCtrl->RMCSD[3];
	}
	else
	{//ROAD
		i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
		i*=4;
		OI=UARTCtrl->RMCSD[7+i]<<8;//当前正在抄的CSD
		OI|=UARTCtrl->RMCSD[7+i+1];
	}
	i=OI_IC(OI);//得到接口类号;入口:对象标识OI
	LEN_Rx-=2;
	p8rx+=12;
	switch(i)
	{
		case 1://电能量接口类,本接口类对象提供存储电能量类信息
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://总及费率电能量数组∷=array 电能量
				//电能量：
				//CHOICE
				//{
				//double-long-unsigned   [6]，
				//double-long            [5]
				//}
					LEN_Rx/=4;//数组数;DL1997返回数据格式:XXXXXX.XX
					if(LEN_RxDataBuff<(5*LEN_Rx))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						//97规约的安徽表，只有总、峰、平、谷
						if(LEN_Rx==4)
						{
							if(i==1)
							{//尖为空
								LEN_Rx=5;
								p8Data[0]=0;//空
								p8Data+=1;
								LEN_Data0+=1;
								p8Array[1]++;//增加数组数
								continue;
							}
						}
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16)|(p8rx[3]<<24);
						if(signedDI==0)
						{
							p8Data[0]=DataType_double_long_unsigned;
							Data=bcd_hex(Data);
						}
						else
						{
							p8Data[0]=DataType_double_long;
							if(Data>>31)
							{//负
								Data<<=1;
								Data>>=1;
								Data=bcd_hex(Data);
								Data=~Data;
								Data+=1;
							}
							else
							{
								Data=bcd_hex(Data);
							}
						}
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8rx+=4;
						p8Data+=5;
						LEN_Data0+=5;
						if(p8Array[1]>=127)
						{
							return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Array[1]++;//增加数组数
					}
					p16Data[0]=LEN_Data0;
					break;
//				case 4://高精度总及费率电能量数组∷=array 高精度电能量
//				//高精度电能量：
//				//CHOICE
//				//{
//				//long64-unsigned        [21]，
//				//long64                 [20]
//				//}

//					break;
				default:
					return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
		case 2://最大需量接口类,本接口类对象提供存储最大需量类信息
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://总及费率最大需量数组∷=array 最大需量及发生时间
				//最大需量及发生时间∷=structure
				//{
				//  最大需量值 CHOICE，
				//  发生时间   date_time_s
				//}
				//最大需量值 ：
				//CHOICE
				//{
				//double-long            [5]，
				//double-long-unsigned   [6]
				//}
					switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
					{
						case 0://最大需量
							LEN_Rx/=3;//数组数;DL1997返回数据格式:XX.XXXX
							if(LEN_RxDataBuff<(13*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								//最大需量值
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{//空
									p8rx+=3;
									continue;
								}
								p8Data[0]=DataType_structure;
								p8Data[1]=2;
								p8Data+=2;
								if(signedDI==0)
								{
									p8Data[0]=DataType_double_long_unsigned;
									Data=bcd_hex(Data);
								}
								else
								{
									p8Data[0]=DataType_double_long;
									if(Data>>23)
									{//负
										Data<<=9;
										Data>>=9;
										Data=bcd_hex(Data);
										Data=~Data;
										Data+=1;
									}
									else
									{
										Data=bcd_hex(Data);
									}
								}
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								//发生时间   date_time_s
								p8Data[5]=DataType_date_time_s;
								p8Data[6]=0;//年
								p8Data[7]=0;
								p8Data[8]=0;//月
								p8Data[9]=0;//日
								p8Data[10]=0;//时
								p8Data[11]=0;//分
								p8Data[12]=0;//秒
								p8rx+=3;
								p8Data+=5+8;
								LEN_Data0+=2+5+8;
								if(p8Array[1]>=127)
								{
									return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Array[1]++;//增加数组数
							}
							p16Data[0]=LEN_Data0;
							break;
						case 1://发生时间
							if(LEN_Data0<(2+15))
							{
								break;//已填写入buff但还没完整一个CSD的数据长度
							}
							if(LEN_Data0<(2+(p8Array[1]*15)))
							{
								break;//已填写入buff但还没完整一个CSD的数据长度
							}
							if((LEN_Rx/4)>p8Array[1])
							{
								break;//已填写入buff但还没完整一个CSD的数据长度
							}
							p8Data+=LEN_Data0;
							p8Data-=(p8Array[1]*15);
							for(i=0;i<LEN_Rx;i++)
							{
								p8Data+=2+5;
								p8Data[0]=DataType_date_time_s;
								Data=0;//年
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								Data=p8rx[3];//月
								Data=bcd_hex(Data);
								p8Data[3]=Data;
								Data=p8rx[2];//日
								Data=bcd_hex(Data);
								p8Data[4]=Data;
								Data=p8rx[1];//时
								Data=bcd_hex(Data);
								p8Data[5]=Data;
								Data=p8rx[0];//分
								Data=bcd_hex(Data);
								p8Data[6]=Data;
								p8Data[7]=0;//秒
								p8rx+=4;
								p8Data+=8;
							}
							break;
					}
					break;
				default:
					return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
		case 3://分相变量接口类,本接口类对象提供存储电压、电流、相角等分相变量类信息
		//分相数值组∷=array 分相数值
		//数值组按A相、B相、C相、N线顺序排列，其中：仅电流有N线，另外，当接线方式为单相时，A、B、C三相改为A相（某一相）。
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://分相数值组
					switch(OI)
					{
						case 0x2000://电压
							switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
							{
								case 0://A相电压
								case 1://B相电压
								case 2://C相电压
									LEN_Rx/=2;//数组数;DL1997返回数据格式:XXX
									if(LEN_RxDataBuff<(3*LEN_Rx))
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									if(Data==0xffff)
									{
										break;
									}
									Data<<=4;
									Data=bcd_hex(Data);
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									LEN_Data0+=3;
									if(p8Array[1]>=127)
									{
										return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Array[1]++;//增加数组数
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x2001://电流
							switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
							{
								case 0://A相电流
								case 1://B相电流
								case 2://C相电流
									LEN_Rx/=2;//数组数;DL1997返回数据格式:XX.XX
									if(LEN_RxDataBuff<(5*LEN_Rx))
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									Data<<=4;
									if(Data==0xffff)
									{
										break;
									}
									if(Data>>19)
									{//负
										Data<<=13;
										Data>>=13;
										Data=bcd_hex(Data);
										Data=~Data;
										Data+=1;
									}
									else
									{
										Data=bcd_hex(Data);
									}
									p8Data[0]=DataType_double_long;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									LEN_Data0+=5;
									if(p8Array[1]>=127)
									{
										return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Array[1]++;//增加数组数
									p16Data[0]=LEN_Data0;
									break;							
							}
							break;
						case 0x2002://电压相角
						case 0x2003://电压电流相角
						case 0x200B://电压波形失真度
						case 0x200C://电流波形失真度
						default:
							return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
				case 4://零线电流
					return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				default:
					return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
		case 4://功率接口类,本接口类对象提供存储功率、功率因数等信息
			//总及分相数值组∷=array 总或分相数值
			//数值组按总、A相、B相、C相顺序排列，当接线方式为单相时，为总、A相（某一相）
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//属性
			{
				case 2://总及分相数值组
					switch(OI)
					{
						case 0x2004://有功功率
							switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
							{
								case 0://总有功功率
								case 1://A相有功功率
								case 2://B相有功功率
								case 3://C相有功功率
									LEN_Rx/=3;//数组数;DL1997返回数据格式:XX.XXXX
									if(LEN_RxDataBuff<(5*LEN_Rx))
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
									if(Data==0xffffff)
									{
										break;
									}
									if(Data>>23)
									{//负
										Data<<=9;
										Data>>=9;
										Data=bcd_hex(Data);
										Data=~Data;
										Data+=1;
									}
									else
									{
										Data=bcd_hex(Data);
									}
									//Data*=1000;//单位：W，换算：-1
									p8Data[0]=DataType_double_long;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									LEN_Data0+=5;
									if(p8Array[1]>=127)
									{
										return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Array[1]++;//增加数组数
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x2005://无功功率
							switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
							{
								case 0://总无功功率
								case 1://A相无功功率
								case 2://B相无功功率
								case 3://C相无功功率
									LEN_Rx/=2;//数组数;DL1997返回数据格式:XX.XX
									if(LEN_RxDataBuff<(5*LEN_Rx))
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									if(Data==0xffff)
									{
										break;
									}
									if(Data>>15)
									{//负
										Data<<=17;
										Data>>=17;
										Data=bcd_hex(Data);
										Data=~Data;
										Data+=1;
									}
									else
									{
										Data=bcd_hex(Data);
									}
									//Data*=1000;//单位：W，换算：-1
									p8Data[0]=DataType_double_long;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									LEN_Data0+=5;
									if(p8Array[1]>=127)
									{
										return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Array[1]++;//增加数组数
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x200A://功率因数
							switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
							{
								case 0://总功率因数
								case 1://A相功率因数
								case 2://B相功率因数
								case 3://C相功率因数
									LEN_Rx/=2;//数组数;DL1997返回数据格式:X.XXX
									if(LEN_RxDataBuff<(3*LEN_Rx))
									{//缓冲长度不够
										return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									if(Data==0xffff)
									{
										break;
									}
									if(Data>>15)
									{//负
										Data<<=1;
										Data>>=1;
										Data=bcd_hex(Data);
										Data=~Data;
										Data+=1;
									}
									else
									{
										Data=bcd_hex(Data);
									}
									p8Data[0]=DataType_long;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									LEN_Data0+=3;
									if(p8Array[1]>=127)
									{
										return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									p8Array[1]++;//增加数组数
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x2006://视在功率
						case 0x2007://一分钟平均有功功率
						case 0x2008://一分钟平均无功功率
						case 0x2009://一分钟平均视在功率
						default:
							return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
				default:
					return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
//		case 5://谐波变量接口类,本接口类对象提供存储谐波变量类信息
//			break;
		case 6://数据变量接口类,本接口类对象提供存储过程值或与过程值单元相关的状态值信息
			switch(OI)
			{
				case 0x2014://电表运行状态字1
					if(LEN_RxDataBuff<5)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=1;
					p8Data+=2;
					p8Data[0]=DataType_bit_string;
					p8Data[1]=8;
					Data=p8rx[0];
					Data=Bit_Inverse(Data,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
					p8Data[2]=Data;
					LEN_Data0=5;
					p16Data[0]=LEN_Data0;
					break;
				default:
					return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			break;
//		case 7://事件对象接口类,本接口类对象提供配置、存储事件记录类信息
//			break;
		case 8://参数变量接口类,本接口类对象提供存储终端的各种参数类信息
			switch(OI)
			{
				case 0x2021://数据冻结时间
					if(LEN_RxDataBuff<8)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//得到date_time_s共7字节
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					p16Data[0]=LEN_Data0;
					break;
				case 0x4000://日期时间
					switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
					{
						case 0://电能表日历时钟Hhmmss
							if(LEN_RxDataBuff<8)
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							for(i=0;i<3;i++)
							{
								Data=p8rx[i];
								Data=bcd_hex(Data);
								p8rx[i]=Data;
							}
							p8Data+=LEN_Data0;
							p8Data[0]=DataType_date_time_s;
							p8Data[1]=0;//年
							p8Data[2]=0;
							p8Data[3]=0;//月
							p8Data[4]=0;//日
							p8Data[5]=p8rx[2];//时
							p8Data[6]=p8rx[1];//分
							p8Data[7]=p8rx[0];//秒
							LEN_Data0+=8;
							p16Data[0]=LEN_Data0;
							break;
						case 1://电能表日历时钟YYMMDDWW
							if(LEN_Data0<8)
							{
								break;//已填写入buff但还没完整一个CSD的数据长度
							}
							p8Data+=LEN_Data0;
							p8Data-=8;
							Data=p8rx[3];//年
							Data=bcd_hex(Data);
							Data+=2000;
							p8Data[1]=Data>>8;
							p8Data[2]=Data;
							Data=p8rx[2];//月
							Data=bcd_hex(Data);
							p8Data[3]=Data;//月
							Data=p8rx[1];//日
							Data=bcd_hex(Data);
							p8Data[4]=Data;//日
							break;
					}
					break;
			}
			break;
//		case 9://冻结数据接口类,本接口类对象提供配置、存储冻结数据及其相关信息
//			break;
//		case 10://采集监控接口类
//			break;
//		case 11://集合接口类,本接口类对象提供配置、存储终端采集数据及其相关信息
//			break;
//		case 12://脉冲计量接口类
//			break;
//		case 13://负荷控制对象接口类,本接口类对象提供负荷管理有关的控制功能
//			break;
//		case 14://区间统计接口类,本接口类对象提供依据配置参数判定、统计与越限相关的信息
//			break;
//		case 15://累加平均接口类,本接口类对象提供对相同物理属性的数值进行累加、平均的运算功能
//			break;
//		case 16://极值工具接口类,本接口类对象提供采集或生成最大、最小值及其发生时间
//			break;
//		case 17://显示接口类,本接口类对象提供与终端显示或对外打印相关的信息
//			break;
//		case 18://文件传输接口类,本接口类对象提供终端实现上传和下载文件的功能
//			break;
//		case 19://设备管理接口类,
//			break;
//		case 20://应用连接接口类
//			break;
//		case 21://ESAM接口类
//			break;
//		case 22://输入输出设备接口类
//			break;
//		case 23://总加组接口类
//			break;
//		case 24://分项事件对象接口类,本接口类对象提供配置、存储分项事件记录类信息
//			break;
//		case 25://无线公网通信接口类
//			break;
//		case 26://以太网通信接口类
//			break;
		default:
			//测试看未处理的OI值
			i=OI_IC(OI);//得到接口类号;入口:对象标识OI
			if(i)
			{
				i=i;
			}
			return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	
	return 0;
}






