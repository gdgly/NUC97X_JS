
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/MeterAutoReport.h"
#include "../DL698/RM_Event_DL2007.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"


#include "../DL698/DL698_JSON.h"

u32 RM_RxDATA_DL2007_CHECK(u32 PORTn)//DL645_2007帧接收数据,比较发收地址、控制码、数据标识、减0x33;返回:0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
{
	u32 i;
	u32 x;
	u8* p8rx;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8=p8;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(PORTn==ACSAMPLEPORT)
	{
		p8rx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	}else
	{
		p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
		if(PORTn!=ACSAMPLEPORT)
		{//不是交采
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
	}
	
	//控制码
	if((p8rx[8]&0x40)!=0x0)
	{//从站异常回答
		UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
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
	x|=p8rx[10+2]<<16;
	x|=p8rx[10+3]<<24;
	if(x==0x04001501)
	{//主动上报状态字
		UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
		#if EnMeterAutoReportEvent==1//允许电能表主动上报事件:0=不允许,1=允许
			MeterAutoReportStateWord(PORTn,p8rx);//主动上报状态字;入口:自动上报接收帧;返回:0=没接受,1=接受
		#endif
		//数据处理完成,返回:4主动上报(已处理)
		return 4;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
	i=UARTCtrl->RM_DI;//DL/T645-2007/1997当前抄表数据标识
	switch(i)
	{
	#if (USER/100)==15//安徽
		case 0x060000FF://全部6类负荷曲线(安徽)
	#endif
		case 0x06000001://全部6类负荷曲线
		case 0x06010001://1类负荷曲线(电压、电流、频率)
		case 0x06020001://2类负荷曲线(有无功功率)
		case 0x06030001://3类负荷曲线(功率因数)
		case 0x06040001://4类负荷曲线(正反有功,組合无功1和2总电能)
		case 0x06050001://5类负荷曲线(1-4象限无功总电能)
		case 0x06060001://6类负荷曲线(当前有功需量,当前无功需量)
			//采曲线数据,按任务执行间隔时间和采集方案采集间隔时间计算
			break;
		default:
			i=RM_TxDATA_DL2007_DI(PORTn,i);//抄表发送帧DI按上n次更改
			break;
	}
	if(x!=i)
	{
		return 3;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
	}
	return 0;//返回0正确,1地址错,2控制码错(无数据),3数据标识错,4主动上报(已处理)
}


__align(4) const u32 signed_DL2007_List[]=//DL/T-2007数据带有正负符号的数据标识列表
{
//1电能量类
	0x0000FFFF,//组合有功总电能
	0x0003FFFF,//组合无功1总电能
	0x0004FFFF,//组合无功2总电能
	0x001700FF,//A相组合无功1电能
	0x001800FF,//A相组合无功2电能
	0x002B00FF,//B相组合无功1电能
	0x002C00FF,//B相组合无功2电能
	0x003F00FF,//C相组合无功1电能
	0x004000FF,//C相组合无功2电能
	0x008000FF,//关联总电能
	0x009400FF,//A相关联总电能
	0x00A800FF,//B相关联总电能
	0x00BC00FF,//C相关联总电能

//2最大需量及发生时间类
	0x0103FFFF,//组合无功1总最大需量及发生时间
	0x0104FFFF,//组合无功1总最大需量及发生时间
	0x011700FF,//A相组合无功1最大需量及发生时间
	0x011800FF,//A相组合无功2最大需量及发生时间
	0x012B00FF,//B相组合无功1最大需量及发生时间
	0x012C00FF,//B相组合无功2最大需量及发生时间
	0x013F00FF,//C相组合无功1最大需量及发生时间
	0x014000FF,//C相组合无功2最大需量及发生时间

//3变量
	0x0202FF00,//电流
	0x02800001,//零线电流
	0x0203FF00,//瞬时有功功率
	0x0204FF00,//瞬时无功功率
	0x0205FF00,//瞬时视在功率
	0x0206FF00,//功率因数
	0x02800004,//当前有功需量
	0x02800005,//当前无功需量
	0x02800006,//当前视在需量
	0x02800007,//表内温度

//4事件记录

//5参变量数据

//6冻结数据
	0x050003FF,//定时冻结组合无功1电能数据
	0x050004FF,//定时冻结组合无功2电能数据
	0x050103FF,//瞬时冻结组合无功1电能数据
	0x050104FF,//瞬时冻结组合无功2电能数据
	0x050203FF,//两套时区表切换组合无功1电能数据
	0x050204FF,//两套时区表切换组合无功2电能数据
	0x050303FF,//两套日时段表切换组合无功1电能数据
	0x050304FF,//两套日时段表切换组合无功2电能数据
	0x05061001,//日冻结有功无功功率(总/A/B/C有功功率,总/A/B/C无功功率)

//7负荷记录数据
};

u32 signed_DL2007_DI(u32 PORTn)//DL/T-2007数据带有正负符号的数据标识;入口:UARTCtrl->RM_DI;返回:0=没符号，1=有符号
{
	u32 i;
	u32 x;
	u32 RMDI32;
	u32 RMDI1;
	u32 RMDI0;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->RM_DI;
	RMDI0=i&0xff;
	RMDI1=(i>>8)&0xff;
	RMDI32=(i>>16);
	for(i=0;i<(sizeof(signed_DL2007_List)/4);i++)
	{
		x=signed_DL2007_List[i];
		x>>=16;
		if(RMDI32==x)
		{
			x=signed_DL2007_List[i];
			x>>=8;
			x&=0xff;
			if((x==0xff)||(x==RMDI1))
			{
				x=signed_DL2007_List[i];
				x&=0xff;
				if((x==0xff)||(x==RMDI0))
				{
					return 1;
				}
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
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)(ROAD时不写入总关联的OAD个数数组!!!)

u32 RM_RxDATA_DL2007(u32 PORTn)//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
{
	u32 i;
	u32 n;
	u32 OI;
	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u32 signedDI;
	u8* p8Array;
	u8* p8Data;
	u16* p16Data;
	u8* p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 offset;
	#if(USER/100)==15//安徽
	u32 s;
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
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
//		if(UARTCtrl->RMCSD[1]==0)
//		{//OAD
//			LEN_Rx=1;
//		}
//		else
//		{//ROAD
//			LEN_Rx=UARTCtrl->RMCSD[6];
//		}	
//		if(LEN_RxDataBuff<LEN_Rx)
//		{//缓冲长度不够
//			return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
//		}
//		for(i=0;i<LEN_Rx;i++)
//		{
//			p8Data[i]=0;
//		}
//		p16Data[0]=LEN_Rx;
		UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
		UARTCtrl->RMROADCount=UARTCtrl->RMROADCountMax;
		return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	if(PORTn==ACSAMPLEPORT)
	{
		p8rx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	}else
	{
		p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	}
	
	LEN_Rx=p8rx[9];
	# if(USER/100)==15//安徽
	if(LEN_Rx<4)
	{//长度<4
		return 0;
	}
	if(LEN_Rx==4)
	{//长度==4，表示空数据
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
	#else
	if(LEN_Rx<=4)
	{//长度<4
		return 0;
	}
	#endif
	
//---后续帧---
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{//后续帧标志
		if(UARTCtrl->RM_SubsequentFrames==0)//0=无读后续帧,1-n=读续帧的帧号
		{//非后续帧
			if(p8rx[8]&0x20)
			{//有后续帧
			#if EnMeterAutoReportEvent==1//允许电能表主动上报事件:0=不允许,1=允许
				UARTCtrl->RM_SubsequentFrames++;//0=无读后续帧,1-n=读续帧的帧号
			#endif
			}
		}
		else
		{
			UARTCtrl->RM_SubsequentFrames=0;//0=无读后续帧,1-n=读续帧的帧号
			//后续帧数据处理
			
			return 0;
		}
	}
//---后续帧结束---
	
//---曲线开始---
	OI=p8rx[10+0]|(p8rx[10+1]<<8)|(p8rx[10+2]<<16)|(p8rx[10+3]<<24);
	switch(OI)
	{
		case 0x06000001://全部6类负荷曲线
	#if (USER/100)==15//安徽
		case 0x060000FF://全部6类负荷曲线(安徽)
	#endif
//负荷记录起始码：A0H，A0H（或 E0H，E0H：本数据块不正确），2 字节； 
//负荷记录字节数：1 字节（十六进制）； 
//负荷记录存储时间：年、月、日、时、分,  5 字节；
		
//电压、电流、频率：17 字节； 
//块分隔码：AAH，1 字节； 
//有、无功功率：24 字节； 
//块分隔码：AAH，1 字节； 
//功率因数：8 字节； 
//块分隔码：AAH，1 字节； 
//有、无功总电能：16 字节； 
//块分隔码：AAH，1 字节； 
//四象限无功总电能：16 字节；
//块分隔码：AAH，1 字节；
//当前需量：6 字节； 
//块分隔码：AAH，1 字节；
//负荷记录累加校验码：1 字节（从第一个 A0H 开始到最后一个数据块结束码）；
//负荷记录结束码：E5H,1 字节。
			break;
		case 0x06010001://1类负荷曲线(电压、电流、频率)
			break;
		case 0x06020001://2类负荷曲线(有无功功率)
			if(p8rx[14+8]!=0xAA)
			{//前面无分隔符AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,25);
				p8rx[9]+=1;
				p8rx[14+8]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+1,25);
			}
			break;
		case 0x06030001://3类负荷曲线(功率因数)
			if(p8rx[14+8]!=0xAA)
			{//前面无分隔符AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,9);
				p8rx[9]+=2;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+2,9);
			}
			break;
		case 0x06040001://4类负荷曲线(正反有功,組合无功1和2总电能)
			if(p8rx[14+8]!=0xAA)
			{//前面无分隔符AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,17);
				p8rx[9]+=3;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				p8rx[14+10]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+3,17);
			}
			break;
		case 0x06050001://5类负荷曲线(1-4象限无功总电能)
			if(p8rx[14+8]!=0xAA)
			{//前面无分隔符AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,17);
				p8rx[9]+=4;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				p8rx[14+10]=0xAA;
				p8rx[14+11]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+4,17);
			}
			break;
		case 0x06060001://6类负荷曲线(当前有功需量,当前无功需量)
			if(p8rx[14+8]!=0xAA)
			{//前面无分隔符AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,7);
				p8rx[9]+=5;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				p8rx[14+10]=0xAA;
				p8rx[14+11]=0xAA;
				p8rx[14+12]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+5,7);
			}
			break;
		default:
			OI=0;
			break;
	}
	if(OI)
	{//是曲线
		if(UARTCtrl->RMCSD[1]==0)
		{//非ROAD错
			return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		LEN_Rx=p8rx[9];
		if(LEN_Rx<(4+8))
		{
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		p8Data+=LEN_Data0;
		for(n=0;n<UARTCtrl->RMCSD[6];n++)
		{
			offset=4+8;
			p8Array=(u8*)p8Data;
			OI=(UARTCtrl->RMCSD[7+(4*n)]<<24)|(UARTCtrl->RMCSD[7+(4*n)+1]<<16)|(UARTCtrl->RMCSD[7+(4*n)+2]<<8)|(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{//方案里是”00100201“和”00200201“，主站用”00100200“和”00200200“召读，费率电量可以是空，只把总电量返回就行了
				case 0x00100201://正向有功电能总
				case 0x00200201://反向有功电能总
				case 0x00300201://组合无功1电能总
				case 0x00400201://组合无功2电能总
				case 0x00500201://第1象限无功电能总
				case 0x00600201://第2象限无功电能总
				case 0x00700201://第3象限无功电能总
				case 0x00800201://第4象限无功电能总
				case 0x20000201://A相电压
				case 0x20010201://A相电流
				case 0x20040202://A相有功功率
				case 0x20050202://A相无功功率
				case 0x200A0202://A相功率因数
					UARTCtrl->RMCSD[7+(4*n)+3]=0;
					break;
			}
			if(OI==0x20010400)
			{//零线电流，645表曲线冻中无零线电流
				if(LEN_RxDataBuff<1)
				{//缓冲长度不够
					return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				p8Data[0]=0;
				p8Data+=1;
				LEN_Data0+=1;
				LEN_RxDataBuff-=1;
				continue;
			}
			OI>>=16;
			switch(OI)
			{
				case 0x2021://冻结时标
					if(LEN_RxDataBuff<8)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data[0]=DataType_date_time_s;
					i=p8rx[14+7];//年
					i=bcd_hex(i);
					i+=2000;
					p8Data[1]=i>>8;
					p8Data[2]=i;
					i=p8rx[14+6];//月
					i=bcd_hex(i);
					p8Data[3]=i;
					i=p8rx[14+5];//日
					i=bcd_hex(i);
					p8Data[4]=i;
					i=p8rx[14+4];//时
					i=bcd_hex(i);
					p8Data[5]=i;
					i=p8rx[14+3];//分
					i=bcd_hex(i);
					p8Data[6]=i;
					p8Data[7]=0;//秒
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					break;
				case 0x2000://电压
				case 0x2001://电流
				case 0x200f://电网频率
			//电压、电流、频率：17 字节； 
			//块分隔码：AAH，1 字节；
					if((LEN_Rx<(offset+17))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x2000://电压
							//DL2007数据格式:XXX.X
							if(LEN_RxDataBuff<(2+(3*3)))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							
							p8Array[0]=DataType_array;
							p8Array[1]=0;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							for(i=0;i<3;i++)
							{
								Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8);
								if(Data==0xffff)
								{
									offset+=2;
									continue;
								}
								Data=bcd_hex(Data);
								p8Data[0]=DataType_long_unsigned;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								offset+=2;
								p8Data+=3;
								LEN_Data0+=3;
								LEN_RxDataBuff-=3;
								p8Array[1]++;//增加数组数
							}
							break;
						case 0x2001://电流
							offset+=6;
							//DL2007数据格式:XXX.XXX
							if(LEN_RxDataBuff<(2+(3*5)))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Array[0]=DataType_array;
							p8Array[1]=0;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							for(i=0;i<3;i++)
							{
								Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16);
								if(Data==0xffffff)
								{
									offset+=3;
									continue;
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
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								offset+=3;
								p8Data+=5;
								LEN_Data0+=5;
								LEN_RxDataBuff-=5;
								p8Array[1]++;//增加数组数
							}
							break;
						case 0x200f://电网频率(数据类型：long-unsigned，单位：Hz，换算：-2)
							offset+=6+9;
							//DL2007数据格式:XXX.XXX
							if(LEN_RxDataBuff<3)
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8);
							if(Data==0xffff)
							{
								p8Data[0]=0;
								p8Data+=1;
								LEN_Data0+=1;
								LEN_RxDataBuff-=1;
								break;
							}
							Data=bcd_hex(Data);
							p8Data[0]=DataType_long_unsigned;
							p8Data[1]=Data>>8;
							p8Data[2]=Data;
							offset+=2;
							p8Data+=3;
							LEN_Data0+=3;
							LEN_RxDataBuff-=3;
							break;
					}
					break;
				case 0x2004://有功功率
				case 0x2005://无功功率
			//有、无功功率：24 字节
			//块分隔码：AAH，1 字节
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if((LEN_Rx<(offset+24))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x2005://无功功率
							offset+=12;
						case 0x2004://有功功率
							if(LEN_RxDataBuff<(2+(4*5)))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Array[0]=DataType_array;
							p8Array[1]=0;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							for(i=0;i<4;i++)
							{
								Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16);
								if(Data==0xffffff)
								{
									offset+=3;
									continue;
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
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								offset+=3;
								p8Data+=5;
								LEN_Data0+=5;
								LEN_RxDataBuff-=5;
								p8Array[1]++;//增加数组数
							}
							break;
					}
					break;
				case 0x200a://功率因数
			//功率因数：8 字节
			//块分隔码：AAH，1 字节
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if((LEN_Rx<(offset+8))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					if(LEN_RxDataBuff<(2+(4*3)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=0;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<4;i++)
					{
						Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8);
						if(Data==0xffff)
						{
							offset+=2;
							continue;
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
						p8Data[0]=DataType_long;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						offset+=2;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8Array[1]++;//增加数组数
					}
					break;
				case 0x0010://正向有功电能
				case 0x0020://反向有功电能
				case 0x0030://组合无功1电能
				case 0x0040://组合无功2电能
			//有、无功总电能：16 字节
			//块分隔码：AAH，1 字节
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=8+1;
					}
					if((LEN_Rx<(offset+16))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x0020://反向有功电能
							offset+=4;
						case 0x0010://正向有功电能
							if(LEN_RxDataBuff<(2+5+4))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Array[0]=DataType_array;
							p8Array[1]=5;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16)|(p8rx[10+offset+3]<<24);
							Data=bcd_hex(Data);
							p8Data[0]=DataType_double_long_unsigned;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data[5]=0;//费率1
							p8Data[6]=0;//费率2
							p8Data[7]=0;//费率3
							p8Data[8]=0;//费率4
							p8Data+=5+4;
							LEN_Data0+=5+4;
							LEN_RxDataBuff-=5+4;
							break;
						case 0x0040://组合无功2电能
							offset+=4;
						case 0x0030://组合无功1电能
							offset+=8;
							if(LEN_RxDataBuff<(2+5+4))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Array[0]=DataType_array;
							p8Array[1]=5;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16)|(p8rx[10+offset+3]<<24);
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
							p8Data[0]=DataType_double_long;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data[5]=0;//费率1
							p8Data[6]=0;//费率2
							p8Data[7]=0;//费率3
							p8Data[8]=0;//费率4
							p8Data+=5+4;
							LEN_Data0+=5+4;
							LEN_RxDataBuff-=5+4;
							break;
					}
					break;
				case 0x0050://第1象限无功电能
				case 0x0060://第2象限无功电能
				case 0x0070://第3象限无功电能
				case 0x0080://第4象限无功电能
			//四象限无功总电能：16 字节
			//块分隔码：AAH，1 字节
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=8+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=16+1;
					}
					if((LEN_Rx<(offset+16))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x0080://第4象限无功电能
							offset+=4;
						case 0x0070://第3象限无功电能
							offset+=4;
						case 0x0060://第2象限无功电能
							offset+=4;
						case 0x0050://第1象限无功电能
							if(LEN_RxDataBuff<(2+5+4))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Array[0]=DataType_array;
							p8Array[1]=5;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16)|(p8rx[10+offset+3]<<24);
							Data=bcd_hex(Data);
							p8Data[0]=DataType_double_long_unsigned;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data[5]=0;//费率1
							p8Data[6]=0;//费率2
							p8Data[7]=0;//费率3
							p8Data[8]=0;//费率4
							p8Data+=5+4;
							LEN_Data0+=5+4;
							LEN_RxDataBuff-=5+4;
							break;
					}
					break;
				case 0x2017://当前有功需量
				case 0x2018://当前无功需量
			//当前需量：6 字节
			//块分隔码：AAH，1 字节
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=8+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=16+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=16+1;
					}
					if((LEN_Rx<(offset+6))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x2018://当前无功需量
							offset+=3;
						case 0x2017://当前有功需量
							if(LEN_RxDataBuff<5)
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16);
							if(Data==0xffffff)
							{
								p8Data[0]=0;
								p8Data+=1;
								LEN_Data0+=1;
								LEN_RxDataBuff-=1;
								break;
							}
							Data=bcd_hex(Data);
							p8Data[0]=DataType_double_long_unsigned;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data+=5;
							LEN_Data0+=5;
							LEN_RxDataBuff-=5;
							break;
					}
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
					break;
			}
		}
		p16Data[0]=LEN_Data0;
		return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	
//用实时数据冻结为曲线
	i=UARTCtrl->RMCSD[1];
	if(i)
	{//ROAD
		OI=UARTCtrl->RMCSD[2+0]<<24;
		OI|=UARTCtrl->RMCSD[2+1]<<16;
		OI|=UARTCtrl->RMCSD[2+2]<<8;
		OI|=UARTCtrl->RMCSD[2+3];
	#if(USER/100)==17//江苏系
		if((OI==0x50020200)||(OI==0x50050200))
	#else
		if(OI==0x50020200)
	#endif
		{//分钟冻结
			p8Data+=LEN_Data0;
			p8Array=(u8*)p8Data;
			LEN_Rx-=4;
			p8rx+=14;
			n=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
			OI=(UARTCtrl->RMCSD[7+(4*n)+0]<<24)+(UARTCtrl->RMCSD[7+(4*n)+1]<<16)+(UARTCtrl->RMCSD[7+(4*n)+2]<<8)+(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{//方案里是”00100201“和”00200201“，主站用”00100200“和”00200200“召读，费率电量可以是空，只把总电量返回就行了
				case 0x00100201://正向有功电能总
				case 0x00200201://反向有功电能总
				case 0x00300201://组合无功1电能总
				case 0x00400201://组合无功2电能总
				case 0x00500201://第1象限无功电能总
				case 0x00600201://第2象限无功电能总
				case 0x00700201://第3象限无功电能总
				case 0x00800201://第4象限无功电能总
				case 0x20000201://A相电压
				case 0x20010201://A相电流
				case 0x20040202://A相有功功率
				case 0x20050202://A相无功功率
				case 0x200A0202://A相功率因数
					UARTCtrl->RMCSD[7+(4*n)+3]=0;
					break;
			}
			switch(OI)
			{
				case 0x20210200://冻结时标
					if(LEN_RxDataBuff<8)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//得到date_time_s共7字节
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					break;
					
			#if(USER/100)==17//???
				case 0x10100200:
				case 0x10300200:
					signedDI=signed_DL2007_DI(PORTn);
					LEN_Rx/=8;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//??????
						return 2;//??:????0=??,1=??1????,2=??2????,3=?????????,4=???????,5=?????127,6=?????????
					}
					p8Array[0]=DataType_array;
					p8Array[1]=0;
					LEN_Data0=2;
					LEN_RxDataBuff-=2;
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						//?
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{//?
							p8rx+=8;
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
							{//?
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
						p8rx+=3;
						p8Data+=5;
						//????
						p8Data[0]=DataType_date_time_s;
						Data=p8rx[4];//?
						Data=bcd_hex(Data);
						Data+=2000;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						Data=p8rx[3];//?
						Data=bcd_hex(Data);
						p8Data[3]=Data;
						Data=p8rx[2];//?
						Data=bcd_hex(Data);
						p8Data[4]=Data;
						Data=p8rx[1];//?
						Data=bcd_hex(Data);
						p8Data[5]=Data;
						Data=p8rx[0];//?
						Data=bcd_hex(Data);
						p8Data[6]=Data;
						p8Data[7]=0;//?
						p8rx+=5;
						p8Data+=8;
						
						LEN_Data0+=2+5+8;;
						if(p8Array[1]>=127)
						{
							return 5;//??:????0=??,1=??1????,2=??2????,3=?????????,4=???????,5=?????127,6=?????????
						}
						p8Array[1]++;//?????
					}
					p16Data[0]=LEN_Data0;
					break;
			case 0x00100200://??????
			case 0x00200200://??????
			case 0x00500200://???????
			case 0x00600200://???????
			case 0x00700200://???????
			case 0x00800200://???????
		#endif
				case 0x00100201://正向有功总电能
				case 0x00200201://反向有功总电能
				case 0x00500201://一象限无功总电能
				case 0x00600201://二象限无功总电能
				case 0x00700201://三象限无功总电能
				case 0x00800201://四象限无功总电能
					LEN_Rx/=4;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
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
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=4;
					}
					if(LEN_Rx==1)
					{
						if(LEN_RxDataBuff<4)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Array[1]=5;//数组数
						p8Data[0]=0;//费率1
						p8Data[1]=0;//费率2
						p8Data[2]=0;//费率3
						p8Data[3]=0;//费率4
						p8Data+=4;
						LEN_Data0+=4;
						LEN_RxDataBuff-=4;
					}
					break;
				case 0x00300200://组合无功1电能
				case 0x00400200://组合无功2电能
					LEN_Rx/=4;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16)|(p8rx[3]<<24);
						signedDI=1;
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
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=4;
					}
					if(LEN_Rx==1)
					{
						if(LEN_RxDataBuff<4)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Array[1]=5;//数组数
						p8Data[0]=0;//费率1
						p8Data[1]=0;//费率2
						p8Data[2]=0;//费率3
						p8Data[3]=0;//费率4
						p8Data+=4;
						LEN_Data0+=4;
						LEN_RxDataBuff-=4;
					}
					break;
				case 0x20170200://当前有功需量
				case 0x20180200://当前无功需量
				//case 0x11100200://冻结周期内正向有功最大需量 
					if(LEN_RxDataBuff<5)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
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
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data+=5;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
					break;
				case 0x20000200://电压
					LEN_Rx/=2;
					if(LEN_RxDataBuff<(2+(3*LEN_Rx)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8);
						if(Data==0xffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_long_unsigned;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=2;
					}
					break;
				case 0x20010200://电流
					LEN_Rx/=3;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
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
						p8Data[0]=DataType_double_long;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
					}
					break;
				case 0x20010400://零线电流
					//零线电流无数组
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
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
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data+=5;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
					break;
				case 0x20040200://有功功率
				case 0x20050200://无功功率
					LEN_Rx/=3;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
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
						p8Data[0]=DataType_double_long;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
					}
					break;
				case 0x200A0200://功率因数
					LEN_Rx/=2;
					if(LEN_RxDataBuff<(2+(3*LEN_Rx)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8);
						if(Data==0xffff)
						{
							Data=0;
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
						p8Data[0]=DataType_long;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=2;
					}
					break;
				case 0x20000201://A电压
					if(LEN_RxDataBuff<(2+3+2))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8);
					if(Data==0xffff)
					{
						Data=0;
					}
					Data=bcd_hex(Data);
					p8Data[0]=DataType_long_unsigned;
					p8Data[1]=Data>>8;
					p8Data[2]=Data;
					p8Data[3]=0;//B电压
					p8Data[4]=0;//C电压
					p8Data+=3+2;
					LEN_Data0+=3+2;
					LEN_RxDataBuff-=3+2;
					break;
				case 0x20010201://A电流
					if(LEN_RxDataBuff<(2+5+2))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
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
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data[5]=0;//B电流空
					p8Data[6]=0;//C电流空
					p8Data+=5+2;
					LEN_Data0+=5+2;
					LEN_RxDataBuff-=5+2;
					break;
				case 0x20040202://A相有功功率
				case 0x20050202://A相无功功率
					if(LEN_RxDataBuff<(2+5))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=1;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
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
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data+=5;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
					break;
				case 0x200A0202://A相功率因数
					if(LEN_RxDataBuff<(2+3))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=1;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8);
					if(Data==0xffff)
					{
						Data=0;
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
					p8Data[0]=DataType_long;
					p8Data[1]=Data>>8;
					p8Data[2]=Data;
					p8rx+=2;
					p8Data+=3;
					LEN_Data0+=3;
					LEN_RxDataBuff-=3;
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
					break;
			}
			p16Data[0]=LEN_Data0;
			return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
	}
//---曲线结束---
	
//---事件类开始---
	i=UARTCtrl->TaskID;
	//停上电事件固定任务号
	if(((i>>8)&0xff)==RMTASKnoPOWER)
	{
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
		if(LEN_RxDataBuff<23)
		{//缓冲长度不够
			return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
		}
		p8Data[0]=DataType_structure;
		p8Data[1]=3;
		//事件记录序号  double-long-unsigned
		p8Data[2]=DataType_double_long_unsigned;
		p8Data[3]=0;
		p8Data[4]=0;
		p8Data[5]=0;
		p8Data[6]=0;
		//事件发生时间  date_time_s
		//事件结束时间  date_time_s
		n=2;
		while(n--)
		{
			p8Data[7]=DataType_date_time_s;
			i=p8rx[14+5];//年
			i=bcd_hex(i);
			i+=2000;
			p8Data[8]=i>>8;
			p8Data[9]=i;
			i=p8rx[14+4];//月
			i=bcd_hex(i);
			p8Data[10]=i;
			i=p8rx[14+3];//日
			i=bcd_hex(i);
			p8Data[11]=i;
			i=p8rx[14+2];//时
			i=bcd_hex(i);
			p8Data[12]=i;
			i=p8rx[14+1];//分
			i=bcd_hex(i);
			p8Data[13]=i;
			i=p8rx[14+0];//秒
			i=bcd_hex(i);
			p8Data[14]=i;
			p8rx+=6;
			p8Data+=8;
		}
		LEN_Data0+=23;
		LEN_RxDataBuff-=23;
		p16Data[0]=LEN_Data0;
		return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
	//全事件采集
	if(((i>>24)&0xf)==2)
	{//事件采集方案
	#if CompareMeterEventCount==1//全事件采集抄表是否比较发生次数:0=不比较(全部抄表数据写入文件),1=比较(只有次数变化的事件写入文件)
		if(UARTCtrl->RMCSDSubCount==0)//抄表CSD子计数(非DL698抄表时)
		{
			i=p8rx[14+0];//取事件次数
			i|=p8rx[14+1]<<8;
			i|=p8rx[14+2]<<16;
			i=bcd_hex(i);
//			i=Get_RxDATA_DL2007_MeterEventCount(PORTn);//从645读事件记录接收帧中取出电能表事件发生次数(事件记录序号);返回:hex发生次数,没找到时返回0xffffffff
//			if(i==0xffffffff)
//			{
//				return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
//			}
			OI=UARTCtrl->RMCSD[2+0]<<24;
			OI|=UARTCtrl->RMCSD[2+1]<<16;
			OI|=UARTCtrl->RMCSD[2+2]<<8;
			OI|=UARTCtrl->RMCSD[2+3];
			n=UARTCtrl->TaskID;
			n>>=8;
			n&=0xff;
			n=UARTCtrl->MeterCount[n];//2*RMTASKmax byte各任务已抄电能表计数,主动上报时用于文件中已上报电能表计数
			if(i==0)
			{
				i=CompSourMeterEventCount(n,OI,i);
				UARTCtrl->RMCSDSubCount=1;//抄表CSD子计数(非DL698抄表时)
				return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			i=CompSourMeterEventCount(n,OI,i);//比较原电能表事件次数;入口:SetNo=档案配置号(0开始),Count=当前次数;返回:0=初始值,1=次数相同,2=次数不同,同时修改原计数,使用ADDR_DATABUFF
			if(i!=2)
			{
				UARTCtrl->RMCSDSubCount=1;//抄表CSD子计数(非DL698抄表时)
				return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
		}
	#endif
		i=Change_Frame_645_to_698(PORTn);//把645的事件数据帧转成698格式
		return i;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
	}
//---事件类结束---
	
	signedDI=signed_DL2007_DI(PORTn);//DL/T-2007数据带有正负符号的数据标识;入口:UARTCtrl->RM_DI;返回:0=没符号，1=有符号
	i=UARTCtrl->RMCSD[1];//当前正在抄的CSD(有数据类型)
	if(i==0)
	{//OAD
		OI=UARTCtrl->RMCSD[2]<<8;//当前正在抄的CSD
		OI|=UARTCtrl->RMCSD[3];
	#if(USER/100)==15//安徽
		s=UARTCtrl->RMCSD[4]<<8;
		s|=UARTCtrl->RMCSD[5];
	#endif
	}
	else
	{//ROAD
		i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
		i*=4;
		OI=UARTCtrl->RMCSD[7+i]<<8;//当前正在抄的CSD
		OI|=UARTCtrl->RMCSD[7+i+1];
	#if(USER/100)==15//安徽
		s=UARTCtrl->RMCSD[7+i+2]<<8;
		s|=UARTCtrl->RMCSD[7+i+3];
	#endif
	}
	i=OI_IC(OI);//得到接口类号;入口:对象标识OI
	
	LEN_Rx-=4;
	p8rx+=14;
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
					LEN_Rx/=4;//数组数;DL2007返回数据格式:XXXXXX.XX
					if(LEN_RxDataBuff<(5*LEN_Rx))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
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
					LEN_Rx/=8;//数组数;DL2007返回数据格式:XX.XXXX YYMMDDhhmm
					if(LEN_RxDataBuff<(13*LEN_Rx))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						//值
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{//空
							p8rx+=8;
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
						p8rx+=3;
						p8Data+=5;
						//发生时间
						p8Data[0]=DataType_date_time_s;
						Data=p8rx[4];//年
						Data=bcd_hex(Data);
						Data+=2000;
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
						p8rx+=5;
						p8Data+=8;
						
						LEN_Data0+=2+5+8;;
						if(p8Array[1]>=127)
						{
							return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Array[1]++;//增加数组数
					}
					p16Data[0]=LEN_Data0;
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
							LEN_Rx/=2;//数组数;DL2007返回数据格式:XXX.X
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
								}
								Data=bcd_hex(Data);
								p8Data[0]=DataType_long_unsigned;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Array[1]++;//增加数组数
							}
							p16Data[0]=LEN_Data0;
							break;
						case 0x2001://电流
							LEN_Rx/=3;//数组数;DL2007返回数据格式:XXX.XXX
							if(LEN_RxDataBuff<(5*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{
									p8rx+=3;
									continue;
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
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								p8rx+=3;
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
						case 0x2002://电压相角
						case 0x2003://电压电流相角
							LEN_Rx/=2;//数组数;DL2007返回数据格式:XXX.X
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
								}
								Data=bcd_hex(Data);
								p8Data[0]=DataType_long_unsigned;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Array[1]++;//增加数组数
							}
							p16Data[0]=LEN_Data0;
							break;
						case 0x200B://电压波形失真度
						case 0x200C://电流波形失真度
							LEN_Rx/=2;//数组数;DL2007返回数据格式:XX.XX
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
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
								p8Data[0]=DataType_long;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Array[1]++;//增加数组数
							}
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
				case 4://零线电流
					switch(OI)
					{
						case 0x2001://电流
							LEN_Rx=1;//数组数;DL2007返回数据格式:XXX.XXX
							if(LEN_RxDataBuff<(5*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							LEN_Data0=0;//零线电流无数组
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{
									p8rx+=3;
									continue;
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
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								p8rx+=3;
								p8Data+=5;
								LEN_Data0+=5;
								//零线电流无数组
//								if(p8Array[1]>=127)
//								{
//									return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
//								}
//								p8Array[1]++;//增加数组数
							}
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 4;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					break;
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
						case 0x2005://无功功率
							if(UARTCtrl->RMCSD[1]==1)
							{//读记录
								if(((UARTCtrl->RMCSD[2]<<8)+UARTCtrl->RMCSD[3])==0x5004)
								{//日冻结:上1次日冻结变量数据（字节数3*8）(总/A/B/C有功功率,总/A/B/C无功功率)
									if(LEN_Rx!=24)
									{
										return 0;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
									}
									LEN_Rx-=12;
									if(OI==0x2005)//无功功率
									{
										p8rx+=12;//跳过有功功率
									}
								}
							}
						case 0x2006://视在功率
						case 0x2007://一分钟平均有功功率
						case 0x2008://一分钟平均无功功率
						case 0x2009://一分钟平均视在功率
							LEN_Rx/=3;//数组数;DL2007返回数据格式:XX.XXXX
							if(LEN_RxDataBuff<(5*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{
									p8rx+=3;
									continue;
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
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								p8rx+=3;
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
						case 0x200A://功率因数
							LEN_Rx/=2;//数组数;DL2007返回数据格式:XX.XX
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//缓冲长度不够
								return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
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
								p8Data[0]=DataType_long;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
								}
								p8Array[1]++;//增加数组数
							}
							p16Data[0]=LEN_Data0;
							break;
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
			#if(USER/100)==15//安徽
				case 0x2131://月A相电压合格率
				case 0x2132://月B相电压合格率
				case 0x2133://月C相电压合格率
					if(((s>>8)&0xff)==2&&(s&0xff)!=0)//月电压合格率或者日电压合格率
					{
						if(LEN_RxDataBuff<15)
						{//缓冲长度不够
							return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
						}
						p8Data+=LEN_Data0;
						p8Data[0]=DataType_structure;
						p8Data[1]=5;
						p8Data+=2;
						LEN_Data0+=2;
						LEN_RxDataBuff-=2;
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_long_unsigned;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_long_unsigned;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
						
						p16Data[0]=LEN_Data0;
					}
					break;
				#endif
				case 0x2014://电表运行状态字1-7
					LEN_Rx/=2;//数组数;DL2007返回数据格式:2byte状态字1...2byte状态字7
					if(LEN_RxDataBuff<(2+(4*LEN_Rx)))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Array[0]=DataType_array;
					p8Array[1]=0;
					LEN_Data0=2;
					LEN_RxDataBuff-=2;
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						p8Data[0]=DataType_bit_string;
						p8Data[1]=16;
						Data=p8rx[0];
						Data=Bit_Inverse(Data,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
						p8Data[2]=Data;
						Data=p8rx[1];
						Data=Bit_Inverse(Data,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
						p8Data[3]=Data;
						
						p8Data+=4;
						p8Array[1]++;
						LEN_Data0+=4;
						LEN_RxDataBuff-=4;
						p8rx+=2;
					}
					p16Data[0]=LEN_Data0;
					break;
				case 0x202D://（当前）透支金额  数据类型：double-long-unsigned，单位：元，换算：-2
					if(LEN_RxDataBuff<5)
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					Data=p8rx[6]|(p8rx[7]<<8)|(p8rx[8]<<16)|(p8rx[9]<<24);
					Data=bcd_hex(Data);
					p8Data[0]=DataType_double_long_unsigned;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
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
					if(UARTCtrl->RM_DI==0x00FE0001)//DL/T645-2007/1997当前抄表数据标识
					{//是上一结算日的冻结时标，只有年月日时4字节
						for(i=0;i<4;i++)
						{
							Data=p8rx[i];
							Data=bcd_hex(Data);
							p8rx[i]=Data;
						}
						p8Data+=LEN_Data0;
						p8Data[0]=DataType_date_time_s;
						Data=p8rx[3];//年
						Data+=2000;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data[3]=p8rx[2];//月
						p8Data[4]=p8rx[1];//日
						p8Data[5]=p8rx[0];//时
						p8Data[6]=0;//分
						p8Data[7]=0;//秒
						LEN_Data0+=8;
						p16Data[0]=LEN_Data0;
						break;
					}
					//时标，年月日时分5字节
					for(i=0;i<5;i++)
					{
						Data=p8rx[i];
						Data=bcd_hex(Data);
						p8rx[i]=Data;
					}
					p8Data+=LEN_Data0;
					p8Data[0]=DataType_date_time_s;
					Data=p8rx[4];//年
					Data+=2000;
					p8Data[1]=Data>>8;
					p8Data[2]=Data;
					p8Data[3]=p8rx[3];//月
					p8Data[4]=p8rx[2];//日
					p8Data[5]=p8rx[1];//时
					p8Data[6]=p8rx[0];//分
					p8Data[7]=0;//秒
					LEN_Data0+=8;
					p16Data[0]=LEN_Data0;
//				#if CompareTime_5004==1//抄读日冻结是否比较冻结时间:0=不比较,1=比较
//					i=UARTCtrl->RMCSD[1];//当前正在抄的CSD(有数据类型)
//					if(i==1)
//					{//ROAD
//						OI=UARTCtrl->RMCSD[2]<<24;//当前正在抄的OAD
//						OI|=UARTCtrl->RMCSD[2+1]<<16;
//						OI|=UARTCtrl->RMCSD[2+2]<<8;
//						OI|=UARTCtrl->RMCSD[2+3]<<0;
//						if(OI==0x50040200)
//						{//日冻结
//							i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内关联的OAD计数(非DL698抄表时)
//							i*=4;
//							OI=UARTCtrl->RMCSD[7+i]<<24;//当前正在抄的CSD
//							OI|=UARTCtrl->RMCSD[7+i+1]<<16;
//							OI|=UARTCtrl->RMCSD[7+i+2]<<8;
//							OI|=UARTCtrl->RMCSD[7+i+3]<<0;
//							if(OI==0x20210200)
//							{//数据冻结时间
//								for(i=0;i<4;i++)
//								{
//									if(p8Data[1+i]!=Comm_Ram->DL698YMDHMS[i])
//									{
//										break;
//									}
//								}
//								if(i<4)
//								{//年月日不同
//									LEN_Data0=0;
//									p16Data[0]=LEN_Data0;
//								}
//							}
//						}
//					}
//				#endif
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
				case 0x202C://（当前）钱包文件 剩余金额	double-long-unsigned（单位：元，换算：-2）， 购电次数	double-long-unsigned
					if(LEN_RxDataBuff<(2+5+5))
					{//缓冲长度不够
						return 2;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					}
					p8Data+=LEN_Data0;
					p8Data[0]=DataType_structure;
					p8Data[1]=2;
					//剩余金额	double-long-unsigned（单位：元，换算：-2）
					Data=p8rx[2]|(p8rx[3]<<8)|(p8rx[4]<<16)|(p8rx[5]<<24);
					Data=bcd_hex(Data);
					p8Data[2]=DataType_double_long_unsigned;
					p8Data[3]=Data>>24;
					p8Data[4]=Data>>16;
					p8Data[5]=Data>>8;
					p8Data[6]=Data;
					//购电次数	double-long-unsigned
					Data=p8rx[0]|(p8rx[1]<<8);
					Data=bcd_hex(Data);
					p8Data[7]=DataType_double_long_unsigned;
					p8Data[8]=Data>>24;
					p8Data[9]=Data>>16;
					p8Data[10]=Data>>8;
					p8Data[11]=Data;
					LEN_Data0+=12;
					LEN_RxDataBuff-=12;
					p16Data[0]=LEN_Data0;
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

















