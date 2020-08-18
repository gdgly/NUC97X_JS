
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_TxDATA.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_Uart.h"

#include "../DL698/DL698_JSON.h"


u32 RM_Rx(u32 PORTn)//抄表接收帧;入口:PORTn;返回:0=正在接收,1=完成1帧接收,2=接收超时
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
	if(i>=2)
	{
		i=UARTCtrl->AutoReportRMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	}
	else
	{
		i=UARTCtrl->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	}
	switch(i)
	{
		default://未知 (0)
			return RM_Rx_Unknown(PORTn);//未知规约接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
		case 1://DL/T645-1997
		case 2://DL/T645-2007
			return RM_Rx_DL645(PORTn);//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
		case 3://DL/T698.45
			return Uart_698_Rx(PORTn);//UART_698接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
		case 4://CJ/T 188-2004
			return RM_Rx_Unknown(PORTn);//未知规约接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
		
		case ProtocolNO_ABB://ABB
			return RM_Rx_ABB(PORTn);//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
		case ProtocolNO_Landis://Landis(IEC1107 模式C)
			return RM_Rx_Landis(PORTn);//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时

	}
}

u32 RM_RxDATA_CHECK(u32 PORTn)//帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回:0=正确,1=地址错,2=控制码错,3=数据标识错
{
	u32 i;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->AutoReportTask;//电能表主动上报任务:0=空,1=有主动上报等待读取,2...抄读中
	if(i>=2)
	{
		i=UARTCtrl->AutoReportRMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	}
	else
	{
		i=UARTCtrl->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	}
	switch(i)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	{
		default://未知 (0)
			return 0;
		case 1://DL/T645-1997
			return RM_RxDATA_DL1997_CHECK(PORTn);//DL645_1997帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
		case 2://DL/T645-2007
			return RM_RxDATA_DL2007_CHECK(PORTn);//DL645_2007帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
		case 3://DL/T698.45
			return RM_RxDATA_DL698_CHECK(PORTn);//DL698帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
		case 4://CJ/T 188-2004
			return 0;
		
		case ProtocolNO_ABB://ABB
			return RM_RxDATA_ABB_CHECK(PORTn);//ABB帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错
		case ProtocolNO_Landis://Landis(IEC1107 模式C)
			return RM_RxDATA_Landis_CHECK(PORTn);//Landis帧接收数据检查,比较发收地址、控制码、数据标识、减0x33;返回0正确,1地址错,2控制码错,3数据标识错

	}
}





//UART抄表数据缓冲定义
//#define LEN_RM_DI_CSD     1024
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)

//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型,全部数据为单独顺序排列无总数组)

//抄表结束后合并在数据缓冲1
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)

//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29 采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型,全部数据为单独顺序排列无总数组)

void RM_RxDATA_START(u32 PORTn)//抄表接收数据处理-开始
{
	u32 id;
	u8* p8;
	u16* p16;
  UARTCtrl_TypeDef * UARTCtrl;
	RecordFileHead_TypeDef* RecordFileHead;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	RecordFileHead=(RecordFileHead_TypeDef*)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
	
	MR((u32)&RecordFileHead->Addr0,(u32)&UARTCtrl->RMAddr,6);//采集通信地址
	RecordFileHead->LEN_DATA=23;//长度=时标+已采集的CSD(不包括本身2字节)
	
	id=UARTCtrl->TaskID;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
	id>>=8;
	id&=0xff;//任务号
	MR(((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef),(u32)&UARTCtrl->TaskTime[id].StdTime,7);//采集启动时标(任务开始标准时间)  date_time_s(无数据类型)
//	DateSaveTI(PORTn,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14);//采集存储时标
	p8=(u8*)RecordFileHead;
	p8[sizeof(RecordFileHead_TypeDef)+21]=DataType_array;//采集的CSD  array(有数据类型)
	p8[sizeof(RecordFileHead_TypeDef)+22]=0;
	
	p16=(u16*)RecordFileHead;
	p16[LEN_RM_DI_CSD/2]=0;//长度=已填写入buff但还没完整一个CSD的长度
	p16[(LEN_RM_DI_CSD/2)+1]=0;//长度=已填写入buff完整CSD的长度(不包括本身2字节)
	//p16[(LEN_RM_DI_CSD/2)+2]=DataType_array;//采集的数据(有数据类型)
}

#if (USER/100)==15//安徽
void RM_RxDATA_END(u32 PORTn)//抄表接收数据处理-结束
{
	u32 len1;
	u32 len2;
	u16* p16;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 i;
	u8* pDATA;
	u32 x;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p16=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
	RecordFileHead=(RecordFileHead_TypeDef*)p16;
	MR((u32)p16+sizeof(RecordFileHead_TypeDef)+7,ADDR_DL698YMDHMS,7);//采集成功时标  date_time_s(无数据类型)
	DateSaveTI(PORTn,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14);//采集存储时标
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;//任务号
	if((i==2)&&(UARTCtrl->RMprotocol==2))
	{
		MR(ADDR_4KWRITE_BUFF,((u32)p16)+LEN_RM_DI_CSD+4,8);//冻结时间
		pDATA=(u8*)ADDR_4KWRITE_BUFF;
		x=Compare_DL698DataString(pDATA+1,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14,7);//电表的数据冻结时间和采集成功时间的日期不符，不保存电表的数据
		if(x!=0)
		{
			len2=15;
			MC(0,((u32)p16)+LEN_RM_DI_CSD+4,15);
		}
		else
		{
			len2=p16[(LEN_RM_DI_CSD/2)+1];
		}
		len1=RecordFileHead->LEN_DATA;
		RecordFileHead->LEN_DATA=len1+len2;
		MR(((u32)p16)+sizeof(RecordFileHead_TypeDef)+len1,((u32)p16)+LEN_RM_DI_CSD+4,len2);
	}
	else
	{
		//合并数据
		len1=RecordFileHead->LEN_DATA;
		len2=p16[(LEN_RM_DI_CSD/2)+1];
		RecordFileHead->LEN_DATA=len1+len2;
		MR(((u32)p16)+sizeof(RecordFileHead_TypeDef)+len1,((u32)p16)+LEN_RM_DI_CSD+4,len2);
	}
}

#else
void RM_RxDATA_END(u32 PORTn)//抄表接收数据处理-结束
{
	u32 len1;
	u32 len2;
	u16* p16;
	RecordFileHead_TypeDef* RecordFileHead;
	
	p16=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);//得到UART抄表数据缓冲地址
	RecordFileHead=(RecordFileHead_TypeDef*)p16;
	MR((u32)p16+sizeof(RecordFileHead_TypeDef)+7,ADDR_DL698YMDHMS,7);//采集成功时标  date_time_s(无数据类型)
	DateSaveTI(PORTn,(u8*)((u32)RecordFileHead)+sizeof(RecordFileHead_TypeDef)+14);//采集存储时标
	//合并数据
	len1=RecordFileHead->LEN_DATA;
	len2=p16[(LEN_RM_DI_CSD/2)+1];
	RecordFileHead->LEN_DATA=len1+len2;
	MR(((u32)p16)+sizeof(RecordFileHead_TypeDef)+len1,((u32)p16)+LEN_RM_DI_CSD+4,len2);
}

#endif


u32 RM_One_CSD(u32 PORTn)//检查抄表是否完成1个完整的CSD的数据;返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
{
	u32 i;
	u32 n;
	u32 OAD;
	u32 RMOADCount;
	u32 OADCountOK;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u8 *p8CSD;
	u16 *p16Data;
	UARTCtrl_TypeDef * UARTCtrl;
	u8* p8CSDlast;
	
	n=n;
	OAD=OAD;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数(非DL698抄表时)
	if(i>=UARTCtrl->RMCSDSubCountMax)//抄表CSD有子抄表个数(非DL698抄表规约库设定)
	{//1个OAD抄完
		//采集的CSD  array
		if(UARTCtrl->RMROADCountMax<=1)
		{//要抄的CSD为OAD或ROAD不分多个OAD抄
	#if LEN_RecordFileHead<8
		#error
	#endif
			//一个完整OAD的数据长度
			p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
			p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
			LEN_Data0=p16Data[0];
			LEN_Data1=p16Data[1];
			if(LEN_Data0==0)
			{//无数据
				return 0;
			}
			LEN_Data1+=LEN_Data0;
			p16Data[1]=LEN_Data1;
			p16Data[0]=0;
			//
			p16Data=(u16*)(p8CSD+6);
			LEN_Data1=p16Data[0];
			LEN_Data0=Get_DL698DataLen_S(UARTCtrl->RMCSD,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if((LEN_Data1+LEN_Data0)>(LEN_RM_DI_CSD-sizeof(RecordFileHead_TypeDef)))
			{
				return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
//		#if (USER==901)//河南版
			//方案里是”00100201“和”00200201“，主站用”00100200“和”00200200“召读，费率电量可以是空，只把总电量返回就行了
			i=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|(UARTCtrl->RMCSD[5]<<0);
			if(i==0x50020200)
			{//分钟冻结(曲线)
				i=UARTCtrl->RMCSD[6];
				for(n=0;n<i;n++)
				{
					OAD=(UARTCtrl->RMCSD[7+(4*n)]<<24)|(UARTCtrl->RMCSD[7+(4*n)+1]<<16)|(UARTCtrl->RMCSD[7+(4*n)+2]<<8)|(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
					switch(OAD)
					{
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
				}
			}
//		#endif
			p8CSDlast=p8CSD;
			p8CSDlast+=sizeof(RecordFileHead_TypeDef)+21;//指向采集的CSD  array CSD(有数据类型)
			i=p8CSDlast[1];//取数组数
			if((i==0)||(UARTCtrl->RMCSD[1]==0))
			{//数组数=0或本次写入不是ROAD
				MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,(u32)&UARTCtrl->RMCSD,LEN_Data0);
				LEN_Data1+=LEN_Data0;
				p16Data[0]=LEN_Data1;
				p8CSD+=sizeof(RecordFileHead_TypeDef)+21;
				i=p8CSD[1];//array CSD个数
				i++;
				p8CSD[1]=i;
			}
			else
			{//数组数>=1
				p8CSDlast[1]=i-1;//取数组数-1,计算上最后次抄读数据的OAD或ROAD地址，用于比较是否与本次相同
				n=Get_DL698DataLen_S(p8CSDlast,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				p8CSDlast[1]=i;//恢复数组数
				p8CSDlast+=n;
				for(n=0;n<6;n++)
				{
					if(p8CSDlast[n]!=UARTCtrl->RMCSD[n])
					{
						break;
					}
				}
				if(n<6)
				{//与上次不相同
					MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,(u32)&UARTCtrl->RMCSD,LEN_Data0);
					LEN_Data1+=LEN_Data0;
					p16Data[0]=LEN_Data1;
					p8CSD+=sizeof(RecordFileHead_TypeDef)+21;
					i=p8CSD[1];//array CSD个数
					i++;
					p8CSD[1]=i;
				}
				else
				{//与上次相同
					if(LEN_Data0>=7)
					{
						LEN_Data0-=7;
					}
					MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,((u32)&UARTCtrl->RMCSD)+7,LEN_Data0);
					LEN_Data1+=LEN_Data0;
					p16Data[0]=LEN_Data1;
					i=UARTCtrl->RMCSD[6];//本次ROAD中的OAD个数
					n=p8CSDlast[6];//原ROAD中的OAD个数
					i+=n;
					if(i>127)
					{
						i=127;
					}
					p8CSDlast[6]=i;
				}
			}
		}
		else
		{//ROAD分多个OAD抄
			//一个完整OAD的数据长度
			p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
			p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
			LEN_Data0=p16Data[0];
			LEN_Data1=p16Data[1];
			if(LEN_Data0==0)
			{//无数据
				return 0;
			}
			LEN_Data1+=LEN_Data0;
			p16Data[1]=LEN_Data1;
			p16Data[0]=0;
			//
			RMOADCount=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
			OADCountOK=UARTCtrl->RMROADCountOK;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
			if(RMOADCount>=UARTCtrl->RMROADCountMax)
			{
				return 6;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			if(OADCountOK>RMOADCount)
			{
				return 6;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
			p16Data=(u16*)(p8CSD+6);
			LEN_Data1=p16Data[0];
			
			p8CSDlast=p8CSD;
			p8CSDlast+=sizeof(RecordFileHead_TypeDef)+21;//指向采集的CSD  array CSD(有数据类型)
			i=p8CSDlast[1];//取数组数
			if((i==0)||(UARTCtrl->RMCSD[1]==0))
			{//数组数=0或本次写入不是ROAD
				p8CSDlast=0;//不组合
			}
			else
			{//数组数>=1
				p8CSDlast[1]=i-1;//取数组数-1,计算上最后次抄读数据的OAD或ROAD地址，用于比较是否与本次相同
				n=Get_DL698DataLen_S(p8CSDlast,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				p8CSDlast[1]=i;//恢复数组数
				p8CSDlast+=n;
				for(n=0;n<6;n++)
				{
					if(p8CSDlast[n]!=UARTCtrl->RMCSD[n])
					{
						break;
					}
				}
				if(n<6)
				{//与上次不相同
					p8CSDlast=0;//不组合
				}
				else
				{//与上次相同
					
				}
			}
			
			if((OADCountOK==0)&&(p8CSDlast==0))
			{//计数=0,填写ROAD的前7字节
				if((LEN_Data1+7)>(LEN_RM_DI_CSD-sizeof(RecordFileHead_TypeDef)))
				{
					return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				}
				MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,(u32)&UARTCtrl->RMCSD,7);
				LEN_Data1+=7;
			}
			if((LEN_Data1+4)>(LEN_RM_DI_CSD-sizeof(RecordFileHead_TypeDef)))
			{
				return 1;//返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			}
//		#if (USER==901)//河南版
			//方案里是”00100201“和”00200201“，主站用”00100200“和”00200200“召读，费率电量可以是空，只把总电量返回就行了
			i=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|(UARTCtrl->RMCSD[5]<<0);
			if(i==0x50020200)
			{//分钟冻结(曲线)
				i=UARTCtrl->RMCSD[6];
				for(n=0;n<i;n++)
				{
					OAD=(UARTCtrl->RMCSD[7+(4*n)]<<24)|(UARTCtrl->RMCSD[7+(4*n)+1]<<16)|(UARTCtrl->RMCSD[7+(4*n)+2]<<8)|(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
					switch(OAD)
					{
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
				}
			}
//		#endif
			MR((u32)p8CSD+sizeof(RecordFileHead_TypeDef)+LEN_Data1,((u32)&UARTCtrl->RMCSD)+7+(4*RMOADCount),4);
			LEN_Data1+=4;
			p16Data[0]=LEN_Data1;
			OADCountOK++;
			UARTCtrl->RMROADCountOK=OADCountOK;//抄表数据缓冲ROAD时已填入关联的OAD数据个数计数
			if(p8CSDlast)
			{//组合
				i=p8CSDlast[6];//ROAD中的OAD个数+1
				if(i<127)
				{
					i++;
				}
				p8CSDlast[6]=i;
			}
			else
			{//不组合
				p8CSD+=sizeof(RecordFileHead_TypeDef)+LEN_Data1;
				p8CSD-=(4*OADCountOK);
				p8CSD-=1;
				p8CSD[0]=OADCountOK;//SEQUENCE OF OAD计数
			}
			if((OADCountOK==1)&&(p8CSDlast==0))
			{//array CSD个数
				p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
				p8CSD+=sizeof(RecordFileHead_TypeDef)+21;
				i=p8CSD[1];//array CSD个数
				i++;
				p8CSD[1]=i;
			}

		}
	}
	return 0;
}





/*
u32 Uart_645_CS(u8 *p8)//DL645帧CS值计算,比较,并填写CS值,同时在结尾加0x16;返回0=正确，1=错误
{
	u32 i;
	u32 x;
	u32 cs;

	x=p8[9];//长度
	//cs
	cs=0;
	x+=10;
	for(i=0;i<x;i++)
	{
		cs+=p8[i];
	}
	cs&=0xff;
	if(cs==p8[x])
	{
		i=0;//0=正确，1=错误
	}
	else
	{
		i=1;//0=正确，1=错误
	}
	p8[x]=cs;
	//结尾0x16
	p8[x+1]=0x16;
	return i;
}
*/

u32 RM_Rx_Unknown(u32 PORTn)//未知规约接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
{
	u32 i;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
	p0=p16fifo[0];//中断接收指针
	p1=p16fifo[1];//当前已读指针
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		p16fifo[2]=0;//规约检查指针
		p16fifo[3]=0;
		p16fifo[4]=0;
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//有数据接收重置超时定时
		p16timer[0]=500/10;//字节间超时定时
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-10))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		if(RxByte)
		{
			return 1;//1=接收OK
		}
		else
		{
			return 2;//2=接收超时
		}
	}
	return 0;
}

u32 RM_Rx_DL645(u32 PORTn)//UART_645接收;返回:0=正在接收,1=完成1帧接收,2=接收超时
{
	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+10);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	DMA_UARTn_RX_Pointer(PORTn);//计算DMA_UARTn接收指针值(p0)
	p0=p16fifo[0];//中断接收指针
	p1=p16fifo[1];//当前已读指针
	if((p0>=(LEN_UARTnIntRx-10))||(p1>=(LEN_UARTnIntRx-10)))
	{
		p16fifo[0]=0;//中断接收指针
		p16fifo[1]=0;//当前已读指针
		p16fifo[2]=0;//规约检查指针
		p16fifo[3]=0;
		p16fifo[4]=0;
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//有数据接收重置超时定时
		p16timer[0]=500/10;//字节间超时定时
		if(PORTn==RS485_1PORT)
		{
			Terminal_Ram->RS485_1_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
		if(PORTn==RS485_2PORT)
		{
			Terminal_Ram->RS485_2_RxLED_MS_Timer=20/10;//11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
		}
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-10))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(i!=0x68)
			{
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte==8)
			{
				if(i!=0x68)
				{
					RxByte=0;
					continue;
				}
			}
			else
			{
				if(RxByte>=12)
				{
					if(RxByte>=(p8rxbuff[9]+12))
					{
						if(i==0x16)
						{//接收完成
							//和校验计算
							Pn=0;
							for(i=0;i<(p8rxbuff[9]+10);i++)
							{
								Pn+=p8rxbuff[i];
							}
							Pn&=0xff;
							if(Pn==p8rxbuff[i])
							{//和校验正确
								p16fifo[1]=p1;//当前已读指针
								p16fifo[2]=p1;//检查指针==读指针,免检省时
								UARTCtrl->RxByte=RxByte;
								return 1;
							}
							else
							{//和校验错误
								RxByte=0;
								continue;
							}
						}
						else
						{
							RxByte=0;
							continue;
						}
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//超时
		return 2;//2=接收超时
	}
	return 0;
}



void RM_RxDATA(u32 PORTn)//抄表接收数据处理
{
	u32 Err;
  UARTCtrl_TypeDef * UARTCtrl;
	u16* p16timer;
	u8* p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	Err=UARTCtrl->CONNECT;
	switch(Err)//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
	{
		case 1://连接过程1
		case 2://连接过程2
		case 102://关闭连接
			switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
			{
				default://未知 (0)
				case 1://DL/T645-1997
				case 2://DL/T645-2007
				case 3://DL/T698.45
				case 4://CJ/T 188-2004
					UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
					break;;
				case ProtocolNO_ABB://ABB
					RM_RxDATA_ABB(PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					break;;
				case ProtocolNO_Landis://Landis(IEC1107 模式C)
					RM_RxDATA_Landis(PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
					break;;
			}
			return;
		
		case 101://101=抄表中
			break;
		default:
			UARTCtrl->CONNECT=0;//DL698连接:0=空,1=启动连接，2=，100=连接,101=抄表中,102=关闭连接
			return;
	}
	switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	{
		default://未知 (0)
			return;
		case 1://DL/T645-1997
			Err=RM_RxDATA_DL1997(PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			break;
		case 2://DL/T645-2007
			Err=RM_RxDATA_DL2007(PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			#ifdef DEBUG_JSON
			myprintf("[%s:%d]:Err=%d\n",(u32)&__func__,(u32)__LINE__,Err);
			#endif
			break;
		case 3://DL/T698.45
			Err=RM_RxDATA_DL698(PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
			break;
		case 4://CJ/T 188-2004
			return;
		case ProtocolNO_ABB://ABB
			while(1)
			{
				Err=RM_RxDATA_ABB(PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				if(Err)
				{
					break;
				}
				UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
				Err=RM_RxDATA_ABB(PORTn);//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				if(Err!=2)
				{
					UARTCtrl->CONNECT = 102; 
					return;
				}
			}	
			break;
		case ProtocolNO_Landis://Landis(IEC1107 模式C)
			while(1)
			{
				Err=RM_RxDATA_Landis(PORTn);//抄表接收数据处理:返回:错误代码0=正确,1=缓冲1长度不够,2=缓冲2长度不够,3=缓冲中有关长度值错,4=无对应接口类号,5=数组数大于127,6=抄表计数大于最大数
				if(Err)
				{
					break;
				}
				UARTCtrl->RMCSDSubCount++;//抄表CSD子计数
				Err=RM_TxDATA_Manage(PORTn);//抄表发送数据帧管理;返回:0=不需抄,1=已抄,2=正在抄,3=失败,4=任务暂停
				if(Err!=2)
				{
					return;
				}
			}	
			break;
	}
	if(Err)
	{
		switch(Err)
		{
			case 1:
				Err=CopyString((u8 *)"缓冲1长度不够",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 2:
				Err=CopyString((u8 *)"缓冲2长度不够",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 3:
				Err=CopyString((u8 *)"缓冲中有关长度值错",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 4:
				Err=CopyString((u8 *)"无对应接口类号",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 5:
				Err=CopyString((u8 *)"数组数大于127",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
			case 6:
				Err=CopyString((u8 *)"抄表计数大于最大数",(u8*)ADDR_DATABUFF);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
				break;
		}
		p8=(u8*)ADDR_DATABUFF;
		p8[Err]=0;
		WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
		p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
		p16timer[0]=1000/10;//下帧启动发送延时,以显示错误码
	}
}






