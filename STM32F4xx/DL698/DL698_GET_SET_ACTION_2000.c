

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_SET_ACTION_2000.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/Contrl_Manage.h"
#include "../DL698/CALC_TAG.h"



const s8 ADDR_230x_18[]=//总加组换算及单位
{
//换算及单位∷=structure
 DataType_structure,10,
//{
// 属性3单位换算∷=单位：W，换算：-1
 DataType_Scaler_Unit,-1,Unit_W,
// 属性4单位换算∷=单位：var，换算：-1
 DataType_Scaler_Unit,-1,31, 
// 属性5单位换算∷=单位：W，换算：-1
 DataType_Scaler_Unit,-1,27,
// 属性6单位换算∷=单位：var，换算：-1
 DataType_Scaler_Unit,-1,31, 
// 属性7单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,33, 
// 属性8单位换算∷=单位：kvarh，换算：-4
 DataType_Scaler_Unit,-4,35, 
// 属性9单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,33, 
// 属性10单位换算∷=单位：kvarh，换算：-4
 DataType_Scaler_Unit,-4,35, 
// 属性11单位换算∷=单位：kWh/元，换算：-4
 DataType_Scaler_Unit,-4,54, 
// 属性12单位换算∷=单位：W，换算：-1 
 DataType_Scaler_Unit,-1,27,
//}
};
const s8 ADDR_240x_19[]=//脉冲计量换算及单位
{
//换算及单位∷=structure
 DataType_structure,14,
//{
//属性5单位换算∷=单位：W，换算：-1
 DataType_Scaler_Unit,-1,Unit_W,
//属性6单位换算∷=单位：var，换算：-1
 DataType_Scaler_Unit,-1,Unit_var,
//属性7单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//属性8单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//属性9单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//属性10单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//属性11单位换算∷=单位：kvarh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//属性12单位换算∷=单位：kvarh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//属性13单位换算∷=单位：kvarh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//属性14单位换算∷=单位：kvarh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//属性15单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//属性16单位换算∷=单位：kWh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kWh,
//属性17单位换算∷=单位：kvarh，换算：-4
 DataType_Scaler_Unit,-4,Unit_kvarh,
//属性18单位换算∷=单位：kvarh，换算：-4 
 DataType_Scaler_Unit,-4,Unit_kvarh,
//}
};

extern const u8 Init_EnergyData_double_long_unsigned[];//初始化数据double_long_unsigned电能
extern const u8 Init_double_long[];//初始化数据double_long
u32 GET_23xx_24xx(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//读总加组和脉冲计量;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Len;
	u32 LENmax;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	switch(OAD>>24)
	{
		case 0x23://总加组
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_TAG))
			{
				UARTCtrl->DAR=6;//对象不存在
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1．逻辑名	（static）
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
				case 2://2．总加配置表	（static）
					p8=(u8*)ADDR_230x_2+(n*LENper_230x_2);
					LENmax=LENper_230x_2;
					break;
				case 3://3．总加有功功率	（dyn.）
					p8=(u8*)ADDR_230x_3+(n*LENper_230x_3);
					LENmax=LENper_230x_3;
					break;
				case 4://4．总加无功功率	（dyn.）
					p8=(u8*)ADDR_230x_4+(n*LENper_230x_4);
					LENmax=LENper_230x_4;
					break;
				case 5://5．总加滑差时间内平均有功功率	（dyn.）
					p8=(u8*)ADDR_230x_5+(n*LENper_230x_5);
					LENmax=LENper_230x_5;
					break;
				case 6://6．总加滑差时间内平均无功功率	（dyn.）
					p8=(u8*)ADDR_230x_6+(n*LENper_230x_6);
					LENmax=LENper_230x_6;
					break;
				case 7://7．总加日有功电量	（dyn.）
					p8=(u8*)ADDR_230x_7+(n*LENper_230x_7);
					LENmax=LENper_230x_7;
					break;
				case 8://8．总加日无功电量	（dyn.）
					p8=(u8*)ADDR_230x_8+(n*LENper_230x_8);
					LENmax=LENper_230x_8;
					break;
				case 9://9．总加月有功电量	（dyn.）
					p8=(u8*)ADDR_230x_9+(n*LENper_230x_9);
					LENmax=LENper_230x_9;
					break;
				case 10://10．总加月无功电量	（dyn.）
					p8=(u8*)ADDR_230x_10+(n*LENper_230x_10);
					LENmax=LENper_230x_10;
					break;
				case 11://11．总加剩余电量（费）	（dyn.）
					p8=(u8*)ADDR_230x_11+(n*LENper_230x_11);
					LENmax=LENper_230x_11;
					break;
				case 12://12．当前功率下浮控控后总加有功功率冻结值（dyn.）
					p8=(u8*)ADDR_230x_12+(n*LENper_230x_12);
					LENmax=LENper_230x_12;
					break;
				case 13://13．总加组滑差时间周期	（static）
					p8=(u8*)ADDR_230x_13+(n*LENper_230x_13);
					LENmax=LENper_230x_13;
					break;
				case 14://14．总加组功控轮次配置	（static）
					p8=(u8*)ADDR_230x_14+(n*LENper_230x_14);
					LENmax=LENper_230x_14;
					break;
				case 15://15．总加组电控轮次配置	（static）
					p8=(u8*)ADDR_230x_15+(n*LENper_230x_15);
					LENmax=LENper_230x_15;
					break;
				case 16://16．总加组控制设置状态	（dyn.）
				#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					Contrl_SetState(n);//总加组控制设置状态;入口:总加组号0-7
				#endif
				#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
					Contrl_SetState(n);//总加组控制设置状态;入口:总加组号0-7
				#endif
					p8=(u8*)ADDR_230x_16+(n*LENper_230x_16);
					LENmax=LENper_230x_16;
					break;
				case 17://17．总加组当前控制状态	（dyn.）
					p8=(u8*)ADDR_230x_17+(n*LENper_230x_17);
					LENmax=LENper_230x_17;
					break;
				case 18://18. 换算及单位	（static）
					p8=(u8*)ADDR_230x_18;
					LENmax=sizeof(ADDR_230x_18);
					break;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
			i=OAD&0xff;
			if(i==0)
			{//全部元素
				Len=Get_DL698DataLen_S(p8,LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//分帧标志
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			else
			{//指定元素
				p8=Get_Element(p8,i,0,LENmax);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
				if(p8==0)
				{
					UARTCtrl->DAR=6;//对象不存在
					return 0;
				}
				Len=Get_DL698DataLen_S(p8,LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//分帧标志
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			return Len;
			
		case 0x24://脉冲计量
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_IMPIN))
			{
				UARTCtrl->DAR=6;//对象不存在
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1．逻辑名	（static）
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
				case 2://2．通信地址	（static）
					p8=(u8*)ADDR_240x_2+(n*LENper_240x_2);
					LENmax=LENmax_240x_2;
					break;
				case 3://3．互感器倍率	（static）
					p8=(u8*)ADDR_240x_3+(n*LENper_240x_3);
					LENmax=LENper_240x_3;
					break;
				case 4://4．脉冲配置	（static）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4);
					LENmax=LENper_240x_4;
					break;
				case 5://5．有功功率	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if((i==0)||(i==2))
					{
						p8=(u8*)ADDR_240x_5+(n*LENper_240x_5);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_double_long;
						LENmax=0;
					}
					break;
				case 6://6．无功功率	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if((i==1)||(i==3))
					{
						p8=(u8*)ADDR_240x_5+(n*LENper_240x_5);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_double_long;
						LENmax=0;
					}
					break;
				case 7://7．当日正向有功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==0)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 8://8．当月正向有功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==0)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 9://9．当日反向有功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==2)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 10://10．当月反向有功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==2)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 11://11．当日正向无功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==1)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 12://12．当月正向无功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==1)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 13://13．当日反向无功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==3)
					{
						p8=(u8*)ADDR_impEnergy_Day+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 14://14．当月反向无功电量	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==3)
					{
						p8=(u8*)ADDR_impEnergy_Month+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 15://15．正向有功电能示值	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==0)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 16://16．正向无功电能示值	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==1)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 17://17．反向有功电能示值	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==2)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 18://18．反向无功电能示值	（dyn.）
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4)+10;
					i=MRR((u32)p8,1);//正向有功（0),正向无功（1）， 反向有功（2），反向无功（3）
					if(i==3)
					{
						p8=(u8*)ADDR_impEnergy+(n*LENper_impEnergy);
						LENmax=0;
					}
					else
					{
						p8=(u8*)Init_EnergyData_double_long_unsigned;
						LENmax=0;
					}
					break;
				case 19://19．换算及单位	（static）
					p8=(u8*)ADDR_240x_19;
					LENmax=sizeof(ADDR_240x_19);
					break;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
			i=OAD&0xff;
			if(i==0)
			{//全部元素
				Len=Get_DL698DataLen_S(p8,LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//分帧标志
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			else
			{//指定元素
				p8=Get_Element(p8,i,0,LENmax);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
				if(p8==0)
				{
					UARTCtrl->DAR=6;//对象不存在
					return 0;
				}
				Len=Get_DL698DataLen_S(p8,LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
				if(LENmax_TxSPACE<Len)
				{
					GET_Request_Frame(PORTn);//分帧标志
					return 0;
				}
				MR((u32)p8tx,(u32)p8,Len);
			}
			return Len;
		default:
			UARTCtrl->DAR=6;//对象不存在
			return 0;
	}
}

u32 SET_23xx_24xx(u32 PORTn,u8* p8rx,u32 LEN_RxAPDU)//设置总加组和脉冲计量;返回:本对象的数据长度(不包括4字节OAD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 Len;
	u32 OAD;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	OAD=p8rx[0]<<24;
	OAD|=p8rx[1]<<16;
	OAD|=p8rx[2]<<8;
	OAD|=p8rx[3];
	p8rx+=4;
	Len=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
	switch(OAD>>24)
	{
		case 0x23://总加组
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_TAG))
			{
				UARTCtrl->DAR=6;//对象不存在
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 2://2．总加配置表	（static）
					if(Len>LENper_230x_2)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_2+(n*LENper_230x_2);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 13://13．总加组滑差时间周期	（static）
					if(Len>LENper_230x_13)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_13+(n*LENper_230x_13);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 14://14．总加组功控轮次配置	（static）
					if(Len>LENper_230x_14)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_14+(n*LENper_230x_14);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 15://15．总加组电控轮次配置	（static）
					if(Len>LENper_230x_15)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_230x_15+(n*LENper_230x_15);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
		case 0x24://脉冲计量
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_IMPIN))
			{
				UARTCtrl->DAR=6;//对象不存在
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 2://2．通信地址	（static）
					if(Len>LENper_240x_2)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_240x_2+(n*LENper_240x_2);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 3://3．互感器倍率	（static）
					if(Len>LENper_240x_3)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_240x_3+(n*LENper_240x_3);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				case 4://4．脉冲配置	（static）
					if(Len>LENper_240x_4)
					{
						UARTCtrl->DAR=255;
						return Len;
					}
					p8=(u8*)ADDR_240x_4+(n*LENper_240x_4);
					MW((u32)p8rx,(u32)p8,Len);
					return Len;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
		default:
			UARTCtrl->DAR=6;//对象不存在
			return Len;
	}
}

u32 ACTION_23xx_24xx(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法总加组和脉冲计量;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 x;
	u32 LenRx;
	u32 LenTx;
	u32 OAD;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	OAD=p8rx[0]<<24;
	OAD|=p8rx[1]<<16;
	OAD|=p8rx[2]<<8;
	OAD|=p8rx[3];
	p8rx+=4;
	switch(OAD>>24)
	{
		case 0x23://总加组
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_TAG))
			{
				UARTCtrl->DAR=6;//对象不存在
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1	清空总加配置表(参数) 参数∷=NULL
					Init_Data230x();//初始化总加组数据
					MWR(DataType_array,ADDR_230x_2+(n*LENper_230x_2),2);//清空时数组数=0
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 2://2	执行（参数） 参数∷=Data
					LenRx=2;
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 3://3	添加一个总加配置单元（参数）参数∷=总加配置单元
			#if LENper_230x_2>(LEN_DATABUFF/2)
				#error
			#endif
//总加配置表∷=array 总加组配置单元//清空时数组数=0
//总加配置单元∷=structure
//{
//参与总加的分路通信地址  TSA，
//总加标志	            enum{正向（0），反向（1）}，
//运算符标志	            enum{加（0），减（1）}
//}
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					i=p8rx[3]<<8;
					i|=p8rx[4];
					if(i!=0x0705)
					{
						UARTCtrl->DAR=255;
						LenTx=0;
						return (LenRx<<16)+LenTx;
					}
					MR(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					p8=(u8*)ADDR_DATABUFF;
					p8[0]=DataType_array;
					n=p8[1];//数组数
					p8+=2;
					for(i=0;i<n;i++)
					{
						LenTx=Compare_DL698DataString(p8rx+5,p8+5,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(LenTx==0)
						{//相同
							break;
						}
						p8+=15;
					}
					MW((u32)p8rx,(u32)p8,15);
					p8=(u8*)ADDR_DATABUFF;
					if(i>=n)
					{//增加
						n++;
						p8[1]=n;
					}
					LenTx=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
					if(LenTx>LENper_230x_2)
					{
						UARTCtrl->DAR=255;
						LenTx=0;
						return (LenRx<<16)+LenTx;
					}
					n=(OAD>>16)&0xff;
					n--;
					MW(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 4://4	批量添加总加配置单元（参数）参数∷=array总加配置单元
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					x=p8rx[1];//批量添加个数
					p8rx+=2;
					MR(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					while(x--)
					{
						i=p8rx[3]<<8;
						i|=p8rx[4];
						if(i!=0x0705)
						{
							UARTCtrl->DAR=255;
							LenTx=0;
							return (LenRx<<16)+LenTx;
						}
						
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_array;
						n=p8[1];//数组数
						p8+=2;
						for(i=0;i<n;i++)
						{
							LenTx=Compare_DL698DataString(p8rx+5,p8+5,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							if(LenTx==0)
							{//相同
								break;
							}
							p8+=15;
						}
						MW((u32)p8rx,(u32)p8,15);
						p8=(u8*)ADDR_DATABUFF;
						if(i>=n)
						{//增加
							n++;
							p8[1]=n;
						}
						p8=(u8*)ADDR_DATABUFF;
						LenTx=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
						if(LenTx>LENper_230x_2)
						{
							UARTCtrl->DAR=255;
							LenTx=0;
							return (LenRx<<16)+LenTx;
						}
						p8rx+=15;
					}
					n=(OAD>>16)&0xff;
					n--;
					MW(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 5://5	删除一个总加配置单元（参数）参数∷=参与总加的分路通信地址TSA
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					MR(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
					p8=(u8*)ADDR_DATABUFF;
					n=p8[1];//数组数
					p8+=2;
					for(i=0;i<n;i++)
					{
						LenTx=Compare_DL698DataString(p8rx+3,p8+5,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
						if(LenTx==0)
						{//找到
							MR((u32)p8,(u32)p8+15,(n-(i+1))*15);
							p8=(u8*)ADDR_DATABUFF;
							n--;
							p8[1]=n;
							n=(OAD>>16)&0xff;
							n--;
							MW(ADDR_DATABUFF,ADDR_230x_2+(n*LENper_230x_2),LENper_230x_2);
							LenTx=0;
							return (LenRx<<16)+LenTx;
						}
						p8+=15;
					}
					//没找到
					UARTCtrl->DAR=6;//对象不存在
					LenTx=0;
					return (LenRx<<16)+LenTx;
				default:
					UARTCtrl->DAR=255;
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					LenTx=0;
					return (LenRx<<16)+LenTx;
			}
		case 0x24://脉冲计量
			n=(OAD>>16)&0xff;
			if((n==0)||(n>NUMmax_IMPIN))
			{
				UARTCtrl->DAR=6;//对象不存在
				return 0;
			}
			n--;
			i=(OAD>>8)&0xff;
			switch(i)
			{
				case 1://1	复位（参数）参数∷=bit-string
				case 2://2	执行（参数）参数∷=Data
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 3://3	添加脉冲输入单元（脉冲单元）
				#if LENper_240x_4>(LEN_DATABUFF/2)
					#error
				#endif
//脉冲单元∷=structure
//{
//脉冲输入端口号	OAD，//0xF20A0201
//脉冲属性		enum
//{
//   正向有功（0），
//	 正向无功（1），
//	 反向有功（2），
//	 反向无功（3）
//}，
//脉冲常数k     long-unsigned
//}
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					p8=(u8*)ADDR_DATABUFF;
					p8[0]=DataType_array;
					p8[1]=1;//数组数
					MR((u32)p8+2,(u32)p8rx,12);
					MW(ADDR_DATABUFF,ADDR_240x_4+(n*LENper_240x_4),LENper_240x_4);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				case 4://4	删除脉冲输入单元（脉冲输入端口号）
					LenRx=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
					MWR(DataType_array,ADDR_240x_4+(n*LENper_240x_4),2);
					LenTx=0;
					return (LenRx<<16)+LenTx;
				default:
					LenRx=0;
					LenTx=0;
					return (LenRx<<16)+LenTx;
			}
		default:
			UARTCtrl->DAR=6;//对象不存在
			LenRx=0;
			LenTx=0;
			return (LenRx<<16)+LenTx;
	}
}















