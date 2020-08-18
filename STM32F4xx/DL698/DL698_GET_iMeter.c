
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_iMeter.h"
#include "../DL698/DL698_GET_iMeterLIB.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../Calculate/Calculate.h"




u32 GET_iMeter_OAD(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 s;
	u32 DataAddr;
	u32 Len;
	DL698_iMeterLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	pLib=Get_DL698_iMeter_pLib(OAD>>16);//得到iMeter_pLib指针;入口:OI;返回:Lib指针(=0表示无相应的OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//对象不存在           （6）
		return 0;
	}
	Len=0;
	DataAddr=ADDR_128KDATABUFF;
	p8=(u8*)ADDR_128KDATABUFF;
	if(OAD&0xff00)
	{//单个属性
		if((OAD&0xff00)==0x0100)
		{//读逻辑名
			if(LENmax_TxSPACE<4)
			{
				GET_Request_Frame(PORTn);//分帧标志
				return 0;
			}
			p8tx[0]=DataType_octet_string;//long-unsigned
			p8tx[1]=2;
			p8tx[2]=OAD>>24;
			p8tx[3]=OAD>>16;
			return 4;
		}
		switch(pLib->IC)
		{
			case 1://电能量类
				switch((OAD>>8)&0xff)
				{//属性
					case 2://总及费率电能量数组	（dyn.）	array
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(i*pLib->LENper),4);
							if(pLib->DataType==DataType_double_long_unsigned)
							{
								x=bcd_hex(x);
							}
							else
							{
								s=x&0x80000000;
								x&=0x7fffffff;
								x=bcd_hex(x);
								if(s)
								{
									x=~x;
									x++;
								}
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>24;
							p8[2]=x>>16;
							p8[3]=x>>8;
							p8[4]=x;
							Len+=5;
							p8+=5;
						}
						break;
					case 3://换算及单位	（static）	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					case 4://高精度总及费率电能量数组	（dyn.）	array
					case 5://高精度换算及单位	（static）	Scaler_Unit
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
				break;
			case 2://最大需量类
				switch((OAD>>8)&0xff)
				{//属性
					case 2://总及费率最大需量数组	（dyn.）	array
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(i*pLib->LENper),3);
							if(pLib->DataType==DataType_double_long_unsigned)
							{
								x=bcd_hex(x);
							}
							else
							{
								s=x&0x800000;
								x&=0x7fffff;
								x=bcd_hex(x);
								if(s)
								{
									x=~x;
									x++;
								}
							}
							p8[0]=DataType_structure;
							p8[1]=2;
							p8[2]=pLib->DataType;
							p8[3]=x>>24;
							p8[4]=x>>16;
							p8[5]=x>>8;
							p8[6]=x;
							Len+=7;
							p8+=7;
							//发生时间   date_time_s
							p8[0]=DataType_date_time_s;
							x=MRR(pLib->Addr+(i*pLib->LENper)+7,1);//年
							x=bcd_hex(x);
							x+=2000;
							p8[1]=x>>8;
							p8[2]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+6,1);//月
							x=bcd_hex(x);
							p8[3]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+5,1);//日
							x=bcd_hex(x);
							p8[4]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+4,1);//时
							x=bcd_hex(x);
							p8[5]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+3,1);//分
							x=bcd_hex(x);
							p8[6]=x;
						#if(USER/100)==17
							p8[7]=0x00;//秒
						#else
							p8[7]=0xFF;//秒
						#endif
							Len+=8;
							p8+=8;
						}
						break;
					case 3://换算及单位	（static）	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
				break;
			case 3://分相变量类
				switch((OAD>>8)&0xff)
				{//属性
					case 2://分相数值组	（dyn.）	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						switch((OAD>>16)&0xffff)
						{
							case 0x2000://电压
							case 0x2002://电压相角
							case 0x2003://电压电流相角
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),2);
									if(pLib->DataType==DataType_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{ 
										s=x&0x8000;
										x&=0x7fff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>8;
									p8[2]=x;
									Len+=3;
									p8+=3;
								}
								break;
							case 0x2001://电流
								//数值组按A相、B相、C相，当接线方式为单相时，A、B、C三相改为A相（某一相）
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),3);
									if(pLib->DataType==DataType_double_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{
										s=x&0x800000;
										x&=0x7fffff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>24;
									p8[2]=x>>16;
									p8[3]=x>>8;
									p8[4]=x;
									Len+=5;
									p8+=5;
								}
								break;
							default:
								UARTCtrl->DAR=6;//对象不存在           （6）
								return 0;
						}
						break;
					case 3://换算及单位	（static）	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					case 4://属性4零线电流∷=double-long, 单位：A换算：-3
						if(OAD==0x20010400)
						{//属性4零线电流∷=double-long, 单位：A换算：-3
							x=MRR(ADDR_IN+1,3);
							s=x&0x800000;
							x&=0x7fffff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=DataType_double_long;
							p8[1]=x>>24;
							p8[2]=x>>16;
							p8[3]=x>>8;
							p8[4]=x;
							Len=5;
						}
						else
						{
							UARTCtrl->DAR=6;//对象不存在           （6）
							return 0;
						}
						break;
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
				break;
			case 4://功率类
				switch((OAD>>8)&0xff)
				{//属性
					case 2://总及分相数值组	（dyn.）	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						switch((OAD>>16)&0xffff)
						{
							case 0x2004://有功功率
							case 0x2005://无功功率
							case 0x2006://视在功率
							case 0x2007://一分钟平均有功功率
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),3);
									if(pLib->DataType==DataType_double_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{ 
										s=x&0x800000;
										x&=0x7fffff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>24;
									p8[2]=x>>16;
									p8[3]=x>>8;
									p8[4]=x;
									Len+=5;
									p8+=5;
								}
								break;
							case 0x200A://功率因数
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),2);
									if(pLib->DataType==DataType_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{
										s=x&0x8000;
										x&=0x7fff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>8;
									p8[2]=x;
									Len+=3;
									p8+=3;
								}
								break;
							default:
								UARTCtrl->DAR=6;//对象不存在           （6）
								return 0;
						}
						break;
					case 3://换算及单位	（static）	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
				break;
			case 5://谐波变量类
				switch((OAD>>8)&0xff)
				{//属性
					case 2://A相n次数值组	（dyn.）	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(i*pLib->LENper),2);
							s=x&0x8000;
							x&=0x7fff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>8;
							p8[2]=x;
							Len+=3;
							p8+=3;
						}
						break;
					case 3://B相n次数值组	（dyn.）	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(2*22)+(i*pLib->LENper),2);
							s=x&0x8000;
							x&=0x7fff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>8;
							p8[2]=x;
							Len+=3;
							p8+=3;
						}
						break;
					case 4://C相n次数值组	（dyn.）	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(2*22)+(2*22)+(i*pLib->LENper),2);
							s=x&0x8000;
							x&=0x7fff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>8;
							p8[2]=x;
							Len+=3;
							p8+=3;
						}
						break;
					case 5://谐波次数n	（static）	unsigned  
							p8[0]=DataType_unsigned;
							p8[1]=0x15;
							Len=2;
						break;
					case 6://换算及单位	（static）	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
				break;
			case 6://数据变量类
				switch((OAD>>8)&0xff)
				{//属性
					case 2://数值	（dyn.）	instance-specific  
//						p8[0]=DataType_array;
//						p8[1]=pLib->array;
//						p8+=2;
//						Len=2;
						Len=0;
						switch((OAD>>16)&0xffff)
						{
							case 0x200F://电网频率
								x=MRR(pLib->Addr+(i*pLib->LENper),2);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>8;
								p8[2]=x;
								Len+=3;
								p8+=3;
								break;
							case 0x2011://时钟电池电压
								x=MRR(pLib->Addr+(i*pLib->LENper),2);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>8;
								p8[2]=x;
								Len+=3;
								p8+=3;
								break;
							case 0x2014://电表运行状态字1-7
								p8[0]=DataType_array;
								p8[1]=pLib->array;
								p8+=2;
								Len=2;
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),2);
									x=Bit_Inverse(x,16);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
									p8[0]=pLib->DataType;
									p8[1]=16;
									p8[2]=x>>8;
									p8[3]=x;
									Len+=4;
									p8+=4;
								}
								break;
							case 0x2017://当前有功需量
							case 0x2018://当前无功需量
							case 0x2019://当前视在需量
								x=MRR(pLib->Addr+(i*pLib->LENper),3);
								s=x&0x800000;
								x&=0x7fffff;
								x=bcd_hex(x);
								if(s)
								{
									x=~x;
									x++;
								}
								p8[0]=pLib->DataType;
								p8[1]=x>>24;
								p8[2]=x>>16;
								p8[3]=x>>8;
								p8[4]=x;
								Len+=5;
								p8+=5;
								break;
							case 0x201A://当前电价
							case 0x201B://当前费率电价
							case 0x201C://当前阶梯电价
								x=MRR(pLib->Addr+(i*pLib->LENper),4);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>24;
								p8[2]=x>>16;
								p8[3]=x>>8;
								p8[4]=x;
								Len+=5;
								p8+=5;
								break;
							case 0x2026://电压不平衡率
							case 0x2027://电流不平衡率
								x=MRR(pLib->Addr+(i*pLib->LENper),2);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>8;
								p8[2]=x;
								Len+=3;
								p8+=3;
								break;
							default:
								UARTCtrl->DAR=6;//对象不存在           （6）
								return 0;
						}
						break;
					case 3://换算及单位	（static）	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//对象不存在           （6）
						return 0;
				}
				break;
			default:
				UARTCtrl->DAR=6;//对象不存在           （6）
				return 0;
		}
/*
			if(Len>pLib->LENmax)
			{
				if(pLib->LENmax!=0)
				{//数据是存储在FLASH
					UARTCtrl->DAR=9;//数据块不可用         （9）
					return 0;
				}
			}
*/
		i=OAD&0xff;
		if(i)
		{//单个元素
			p8=Get_Element((u8*)DataAddr,i,0,0);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
			if(p8==0)
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
				return 0;
			}
			Len=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			DataAddr=(u32)p8;
		}
		if(Len>LENmax_TxSPACE)
		{
			GET_Request_Frame(PORTn);//分帧标志
			return 0;
		}
		MR((u32)p8tx,DataAddr,Len);
		return Len;
	}
	else
	{//0表示整个对象属性，即对象的所有属性
		UARTCtrl->DAR=255;
		return 0;
	}
}





