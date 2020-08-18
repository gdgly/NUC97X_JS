
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_6000.h"
#include "../DL698/RM_TASK.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"





//属性2（配置表，只读）∷=array 采集档案配置单元
//属性2（Acquisition document definition）∷=structure
//{
//配置序号  long-unsigned，
//基本信息  Basic_object，
//扩展信息  Extended_object，
//附属信息  Annex_object
//}
//Basic_object∷=structure
//{
//通信地址  TSA，
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
//端口      OAD，
//通信密码  octet-string，
//费率个数  unsigned，
//用户类型  unsigned，
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}，
//额定电压    long-unsigned(换算-1，单位V),
//额定电流    long-unsigned(换算-1，单位A)
//}
//Extended_object∷=structure
//{
//采集器地址  TSA，
//资产号      octet-string，
//PT          long-unsigned，
//CT          long-unsigned
//}
//Annex_object∷=array structure
//{
//对象属性描述  OAD，
//属性值        Data
//}



extern const u8 DL698_Attribute_6000_2[];
extern const u8 DL698_Attribute_6002_2[];
extern const u8 DL698_Attribute_6002_5[];
extern const u8 DL698_Attribute_6012_2[];
extern const u8 DL698_Attribute_6014[];



u32 GET_NUMper(DL698_SETLIB_TypeDef *pLib)//计算当前元素个数
{
	u32 i;
	u32 x;
	u32 n;
	u32 NUMmax;
	u8 *p8;
	
	x=0;
	n=0;
	NUMmax=pLib->LENmax/pLib->LENper;
	while(1)
	{
		if(n>=NUMmax)
		{
			break;
		}
		MR(ADDR_128KDATABUFF,pLib->Addr+(n*pLib->LENper),LEN_128KDATABUFF);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/pLib->LENper);i++)
		{
			if(n>=NUMmax)
			{
				break;
			}
			if(p8[0]==DataType_structure)
			{//删除时structure的数据单元清为0(NULL)
				x++;
			}
			n++;
			p8+=pLib->LENper;
		}
	}
	return x;
}


//采集档案配置表等...
u32 GET_Request_6000(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 n;
	u32 Array;
	u32 Len;
	u32 NUMmax;
#if ((USER/100)==17)//江苏		
	u32 j,k,l,Nummax;
	u32 Phase;
	u8* p8s1;
#endif		
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	NUMmax=pLib->LENmax/pLib->LENper;
	n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
	switch(OAD>>16)
	{
		case 0x6032://采集状态

			break;
		case 0x6034://采集任务监控
			for(i=0;i<RMTASKmax;i++)
			{
				RM_Monitor(i,0,0);//采集任务监控;入口:ID=任务号,NUM_Tx=发送条数,NUM_Rx=接收条数
			}
			break;
	}
	p8tx[0]=DataType_array;
	Len=2;
	Array=0;//填入配置组数
	
	while(1)
	{
		if(n>=NUMmax)
		{
			break;
		}
		MR(ADDR_128KDATABUFF,pLib->Addr+(n*pLib->LENper),(LEN_128KDATABUFF/pLib->LENper)*pLib->LENper);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/pLib->LENper);i++)
		{
			if(n>=NUMmax)
			{
				break;
			}
			if(p8[0])
			{//删除时structure的数据单元清为0(NULL)
				//x=Get_DL698DataLen(p8,(Attribute_TypeDef *)pLib->pDataType,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				x=Get_DL698DataLen(p8,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
				if(x&0xff)
				{
					x>>=8;
					if((Array>=127)||(x>LENmax_TxSPACE))
					{//发送缓冲不够,分帧
						if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						{
							UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						}
						else
						{
							UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						}
						UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
						p8tx[1]=Array;//填入配置组数
						return Len;
					}
					MR((u32)p8tx+Len,(u32)p8,x);
					if((OAD>>16) == 0x6032)
					{
#if ((USER/100)==17) // 江苏
#if 0
						// 抄读数据时，相位信息可能还没抄读完成，需要等待采集周期到来
						//一个采集状态∷=structure 
								p8d[0]=DataType_structure;
								p8d[1]=8;
						//{ 
						//1.通信地址              TSA， 
								p8d[2]=DataType_TSA;
								p8d[3]=7;
								p8d[4]=5;
								MR((u32)p8d+5,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
								offset=11;
						//2.中继级别              unsigned， 
								p8d[offset]=DataType_unsigned;
								p8d[offset+1]=Class;
								offset+=2;
						//3.中继地址              TSA， 
								p8d[offset]=0;
								offset+=1;
						//4.端口                  OAD， 
								p8d[offset]=DataType_OAD;
								i=PORTntoOAD(PORTn);//通信口号转为OAD值
								p8d[offset+1]=i>>24;
								p8d[offset+2]=(i>>16)&0xff;
								p8d[offset+3]=(i>>8)&0xff;
								p8d[offset+4]=i;
								offset+=5;
						//5.最后一次采集成功时间  date_time_s， 
								p8d[offset]=DataType_date_time_s;
								Get_date_time_s(p8d+offset+1);//得到date_time_s共7字节
								offset+=8;
						//6.采集失败次数          unsigned， 
								p8d[offset]=DataType_unsigned;
								p8d[offset+1]=0;
								offset+=2;
						//7.相位 enum{未知（0），A 相（1），B 相（2），C 相（3）}，
								p8d[offset+1]=Phase;
								offset+=2;
						//8.相序异常 enum{正常（0），LN 互易（1），逆相序（2）} 
								p8d[offset]=DataType_enum;
								p8d[offset+1]=0;
#endif
						Data_Inverse((u32)p8tx+Len+5,6);// 地址倒序
						p8s1=(u8*)ADDR_AFN10F31;
						Nummax=p8s1[0];
						Nummax|=p8s1[1]<<8;
						for(j=0;j<NUM_RMmax;j++)
						{
							// 匹配表地址
							if(Compare_DL698DataString(p8s1 + 2 + 8 * j ,p8tx+Len+5, 6) == 0)
							{
								break;
							}
						}
					
						if(j == NUM_RMmax)
						{// 未成功找节点地址，需要按采集器进行抄读
							for(i=0;i<NUM_RMmax;i++)
							{
								if(Compare_DL698DataString(&NoteList->List[i].Addr[0], p8tx+Len+5, 6)==0)
								{
									break;
								}
							}
							if(i<NUM_RMmax)
							{// 记下采集器地址
								if(IsAllAByte(&NoteList->List[i].CollectorAddr[0], 0, 6) < 1)
								{// 表计必须先识别到对应的采集器
									for(k=0;k<NUM_RMmax;k++)
									{
										for(l=0;l<Nummax;l++)
										{// 先从AFN10F31找到表对应的采集器
											if(Compare_DL698DataString(&NoteList->List[k].Addr[0], p8s1 + 2 + 8 * l, 6)==0)
											{	// 找到AFN10F31对应采集器	
												if(Compare_DL698DataString(&NoteList->List[k].CollectorAddr[0], &NoteList->List[i].CollectorAddr[0], 6)==0)
												{// AFN10F31和抄读表采集器地址匹配上
													j=l;
													break;
												}
											}
										}
										if(l<Nummax)
										{
											break;
										}
									}
								}
							}
						}

						// 修改实际表记相位
						Phase = 0;
						if(j < NUM_RMmax)	// AFN10F31相位里表地址数据匹配成功
						{
							if(p8s1[ 2+ 8*j + 6]&0x01)
							{
								Phase = 1;
							}
							if(p8s1[ 2+ 8*j + 6]&0x02)
							{
								Phase = 2;
							}
							if(p8s1[ 2+ 8*j + 6]&0x04)
							{
								Phase = 3;
							}
						}
						p8tx[Len+30]=Phase;
						Data_Inverse((u32)p8tx+Len+5,6);// 地址倒序归位
#endif	
					}
					Len+=x;
					LENmax_TxSPACE-=x;
					Array++;//填入配置组数
				}
			}
			n++;
			p8+=pLib->LENper;
		}
	}
	if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	{
		UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	}
	UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
	p8tx[1]=Array;//填入配置组数
	return Len;
}


//搜表结果
u32 GET_Request_6002(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 n;
	u32 Array;
	u32 Len;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 FileID;//FileID=3全部搜表结果文件，=4跨台区搜表结果文件
	u32 TN;//所有搜表结果记录数
	u32 offset;
	SearchMeterFileHead_TypeDef* SearchMeterFileHead;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if((OAD==0x60020500)||(OAD==0x60020700))//跨台区搜表结果或记录数
	{
		FileID=4;
	}
	else
	{
		FileID=3;
	}
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
			break;
		}
		file++;
	}
	if(filename>=recordFILENAMEmax_FLASH)
	{//没找到
		TN=0;//所有搜表结果记录数
	}
	else
	{
		TN=file->NUM_DATA;//所有搜表结果记录数
	}
	switch(OAD)
	{
		case 0x60020400://最大元素个数
			if(LENmax_TxSPACE<3)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_long_unsigned;
			p8tx[1]=NUM_RMmax>>8;
			p8tx[2]=NUM_RMmax&0xff;
			return 3;
		case 0x60020300://当前元素个数
		case 0x60020600://所有搜表结果记录数
		case 0x60020700://跨台区搜表结果记录数
			if(LENmax_TxSPACE<3)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_long_unsigned;
			p8tx[1]=TN>>8;
			p8tx[2]=TN;
			return 3;
		case 0x60020200://所有搜表结果
		case 0x60020500://跨台区搜表结果
			p8tx[0]=DataType_array;
			Len=2;
			Array=0;//填入配置组数
			n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
			if(filename<recordFILENAMEmax_FLASH)
			{//找到文件
				ms=Get_ms();
				ms->msfile_read(filename,0,(u8*)ADDR_AddrList,TN*sizeof(SearchMeterFileHead_TypeDef));
				SearchMeterFileHead=(SearchMeterFileHead_TypeDef*)(ADDR_AddrList);
				offset=0;
				for(i=0;i<n;i++)
				{
					offset+=SearchMeterFileHead->LEN_DATA;
					SearchMeterFileHead++;
				}
				while(1)
				{
					if(n>=TN)
					{
						break;
					}
					x=SearchMeterFileHead->LEN_DATA;
					if((Array>=127)||(x>LENmax_TxSPACE))
					{//发送缓冲不够,分帧
						if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						{
							UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						}
						else
						{
							UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
						}
						UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
						p8tx[1]=Array;//填入配置组数
						return Len;
					}
					ms->msfile_read(filename,(NUM_RMmax*sizeof(SearchMeterFileHead_TypeDef))+offset,p8tx+Len,x);
					offset+=x;
					Len+=x;
					LENmax_TxSPACE-=x;
					Array++;//填入配置组数
					SearchMeterFileHead++;
					n++;
				}
			}
			if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
			{
				UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
			}
			p8tx[1]=Array;//填入配置组数
			return Len;
		default:
			UARTCtrl->DAR=6;
			return 0;
	}
}














