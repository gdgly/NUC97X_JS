
#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../Device/MEMRW.h"
#include "../DL698/CALC_TAG.h"
#include "../Calculate/Calculate.h"
#include "../MS/MS.h"
#include "../DL698/RM_Record.h"
#include "../DL698/DL698_REPORT.h"


//extern u32 RCSDtoID(u8* pRCSD);//记录列选择RCSD相对应的任务号,返回:数据在采集任务的任务号,>=RMTASKmax表示无效任务号


#define INVALID_DATA 0xffffffffffffffff


const u8 Init_long64[]=
{
	DataType_long64,
	0,0,0,0,0,0,0,0,
};
const u8 Init_EnergyData_long64[]=
{
	DataType_array,
	TMaxTariff+1,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
	DataType_long64,
	0,0,0,0,0,0,0,0,
#if TMaxTariff>4
	#error
#endif
};


void Init_Data230x_D(void)//初始化总加组日数据
{
	u32 n;
	u8* p8;
	
//总加日有功电量	
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_7+(n*LENper_230x_7);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
//总加日无功电量
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_8+(n*LENper_230x_8);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
}

void Init_Data230x_M(void)//初始化总加组月数据
{
	u32 n;
	u8* p8;
	
//总加月有功电量
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_9+(n*LENper_230x_9);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
//总加月无功电量
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_10+(n*LENper_230x_10);
		MW((u32)Init_EnergyData_long64,(u32)p8,sizeof(Init_EnergyData_long64));
	}
}







void Init_Data230x(void)//初始化总加组数据
{
	u32 n;
	u8* p8;
	
//总加有功功率
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_3+(n*LENper_230x_3);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//总加无功功率
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_4+(n*LENper_230x_4);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//总加滑差时间内平均有功功率
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_5+(n*LENper_230x_5);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//总加滑差时间内平均无功功率
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_6+(n*LENper_230x_6);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
	
//总加电量计算用原始电能示值
	MC(0,ADDR_230x_Sour,LENmax_230x_Sour);
//总加组日数据
	Init_Data230x_D();//初始化总加组日数据
//总加组月数据
	Init_Data230x_M();//初始化总加组月数据
	
//总加剩余电量（费）
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_11+(n*LENper_230x_11);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//当前功率下浮控控后总加有功功率冻结值
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_12+(n*LENper_230x_12);
		MW((u32)Init_long64,(u32)p8,sizeof(Init_long64));
	}
//总加组滑差时间周期
//总加组功控轮次配置
//总加组电控轮次配置
	
//总加组控制设置状态
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_16+(n*LENper_230x_16);
//总加组控制设置状态∷=structure
		p8[0]=DataType_structure;
		p8[1]=6;
//{
//时段控定值方案号	   unsigned，
		p8[2]=DataType_unsigned;
		p8[3]=0;
//功控时段有效标志位  bit-string(SIZE(8))，
		p8[4]=DataType_bit_string;
		p8[5]=8;
		p8[6]=0;
//功控状态            PCState，
		p8[7]=DataType_bit_string;
		p8[8]=8;
		p8[9]=0;
//电控状态            ECState，
		p8[10]=DataType_bit_string;
		p8[11]=8;
		p8[12]=0;
//功控轮次状态        TrunState，
		p8[13]=DataType_bit_string;
		p8[14]=8;
		p8[15]=0;
//电控轮次状态        TrunState
		p8[16]=DataType_bit_string;
		p8[17]=8;
		p8[18]=0;
//}
	}
//总加组控制设置状态
	for(n=0;n<NUMmax_TAG;n++)
	{
		p8=(u8*)ADDR_230x_17+(n*LENper_230x_17);
//当前控制状态∷=structure
		p8[0]=DataType_structure;
		p8[1]=7;
//{
//	当前功控定值             long64（单位：W 换算：-1），   
		p8[2]=DataType_long64;
		p8[3]=0;
		p8[4]=0;
		p8[5]=0;
		p8[6]=0;
		p8[7]=0;
		p8[8]=0;
		p8[9]=0;
		p8[10]=0;
//	当前功率下浮控浮动系数   integer（单位：%）
		p8[11]=DataType_integer;
		p8[12]=0;
//	功控跳闸输出状态         OutputState，
		p8[13]=DataType_bit_string;
		p8[14]=8;
		p8[15]=0;
//	月电控跳闸输出状态       OutputState，
		p8[16]=DataType_bit_string;
		p8[17]=8;
		p8[18]=0;
//	购电控跳闸输出状态       OutputState，
		p8[19]=DataType_bit_string;
		p8[20]=8;
		p8[21]=0;
//	功控越限告警状态         PCAlarmState，
		p8[22]=DataType_bit_string;
		p8[23]=8;
		p8[24]=0;
//	电控越限告警状态         ECAlarmState
		p8[25]=DataType_bit_string;
		p8[26]=8;
		p8[27]=0;
//}
	}
}





//pAddr该TSA不存在ADDR_6000里和总加组配置里，返回0
//如果是存在的，但是CT,PT是0的，默认成1
u32 Get_CT_PT(u16 * pAddr)//获取TSA对应的CT,PT值，CT放高2字节，PT放低2字节，值已经高低位交换过了，可以直接使用。
{
	u32 i;
	u32 j;
	u32 CTPT=0;
	u8 *p8;
	
	i=AddrToMeterSetNo(pAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
	if(i<NUM_RMmax)//在配置档案里面
	{
		p8=Get_Element((u8 *)(ADDR_6000_SDRAM+LENper_6000*i), 2,4,LENper_6000);
		if(p8==0)
		{
			return CTPT;
		}
		j=MRR((u32)(p8+1),4);
		j=R_Inverse(j,4);
		if((j&0xffffff00)==0xf20a0200)//在6000里端口是秒冲，也不算在6000里面
		{
			i=NUM_RMmax;
		}
	}

	if(i<NUM_RMmax)//在配置档案里面
	{
		j=ADDR_6000_SDRAM+LENper_6000*i;
		j=Get_ElementADDR(ADDR_6000_SDRAM+LENper_6000*i,3,3,LENper_6000);//CT = 0X12 XX XX 
		if(j!=0)
		{
			j=MRR(j+1,2);
			j=R_Inverse(j,2);
		}
		if(j==0)//如果参数设置成0，或是没找到参数，则默认成1
		{
			j=1;
		}
		CTPT|=(j<<16);
		j=Get_ElementADDR(ADDR_6000_SDRAM+LENper_6000*i,3,4,LENper_6000);//PT = 0X12 XX XX 
		if(j!=0)
		{
			j=MRR(j+1,2);
			j=R_Inverse(j,2);
		}
		if(j==0)//如果参数设置成0，或是没找到参数，则默认成1
		{
			j=1;
		}
		CTPT|=j;
		return CTPT;
	}
	//到这说该TSA不存在ADDR_6000里面，可能是脉冲
#if NUMmax_IMPIN!=0
	#if LENmax_240x_2>LEN_DATABUFF
		#error LENmax_240x_2 Over
	#endif		
	MR(ADDR_DATABUFF,ADDR_240x_2,LENmax_240x_2);
	for(i=0;i<NUMmax_IMPIN;i++)
	{
		if(Is_TSA_Same((u8*)pAddr,(u8 *)(ADDR_DATABUFF+i*LENper_240x_2+2))==0)
		{
			j=Get_ElementADDR(ADDR_240x_3+i*LENper_240x_3,2,0,LENper_240x_3);
			if(j!=0)
			{
				j=MRR(j+1,2);
				j=R_Inverse(j,2);
			}
			if(j==0)//如果参数设置成0，或是没找到参数，则默认成1
			{
				j=1;
			}
			CTPT|=(j<<16);
			j=Get_ElementADDR(ADDR_240x_3+i*LENper_240x_3,1,0,LENper_240x_3);
			if(j!=0)
			{
				j=MRR(j+1,2);
				j=R_Inverse(j,2);
			}
			if(j==0)//如果参数设置成0，或是没找到参数，则默认成1
			{
				j=1;
			}
			CTPT|=j;
			return CTPT;	
		}
	}
	return CTPT;
#else
	return 0;
#endif
}

u32 Is_TSA_Same(u8 *pAddr1,u8 *pAddr2)//判断地址是否相同,返回 0=相同 1=不相同
{
	u32 index;
	
	if((pAddr1==0)||(pAddr2==0))//形参错误
	{
		return 1;
	}
	for(index=0;index<6;index++)
	{
		if(pAddr1[index]!=pAddr2[index])
		{
			return 1;
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

//pbuf是从Get_ADDR_UARTnFnDataBuff(PORTn)得到的
//从数据pbuf里找出当前数据oad对应数据的地址，oad只能是当前数据，当前正向有功电能示值、功率,返回 地址包括数据类型
u32 Is_FnDataBuff_Have_TAGData(u32 oad,u8* FnDataBuff)//判断FnDataBuff是否存在总加组的数据，返回 0=不存在oad的数据，!=0，oad对应的数据地址
{
	u32 pos=0;//记录FnDataBuff里数据开始的地址
	u32 index=0;
	u32 num=0xffff;//形参中需要查询oad在记录当中的序号，=0xffff 表示记录里没有改oad的数据
	u32 sum=0;//记录oad个数，是road就计算关联oad的个数
	u32 i;
	if(FnDataBuff==0)//形参错误
	{
		return 0;
	}

	pos=31;//定位到  采集的CSD  array CSD(有数据类型)，ARRAY N CSD-1 CSD-2 CSD-N
	for(index=0;index<FnDataBuff[30];index++)
	{
		pos+=1;
		if(FnDataBuff[pos]==1)//road
		{
			pos+=1;
			pos+=4;
			i=FnDataBuff[pos];
			pos+=i*4;
		}
		if(FnDataBuff[pos]==0)//oad
		{
			pos+=1;
			i=MRR((u32)FnDataBuff+pos,4);
			i=R_Inverse(i,4);
			pos+=4;
			if(i==oad)//找到形参的oad了，记录下该oad在记录里的序号，该序号从0开始
			{
				num=sum;
			}
			sum+=1;
		}
	}
	i=(u32)FnDataBuff+pos;//纯粹调试用，无意义。
	if(num==0xffff)//该帧数据记录里没该oad，直接返回0
	{
		return 0;
	}
	while(num)//从数据开始，跳过前面的数据，就指向形参oad的数据起始地址了。
	{
		i=Get_DL698DataLen_S(FnDataBuff+pos,0);
		pos+=i;
		num-=1;
	}
	return (u32)FnDataBuff+pos;
}

//data_sour的数据类型:
//有功
//电能量∷=double-long-unsigned；单位：kWh，换算：-2  06 NN NN NN NN 
//高精度电能量∷=long64-unsigned；单位：kWh，换算：-4  0X15 NN NN NN NN NN NN NN NN 

//无功
//电能量∷=double-long；单位：kvarh，换算：-2
//高精度电能量∷=long64；单位：kvarh，换算：-4

//data_sour的值乘以 CT PT之后存入data_dest，
//u32 TAG_NO是总加组序号，从0开始；u32 add_mise是运算符标志0=加 1=减
//u32 type 0=正有 1=正无 2=反有 3=反无
//u32 group_type=0;//总加标志 0=正向 1=反向
//原始值按 0.1W 的精度来存储,数据类型是 long64-unsigned  0x15 
//u32 pn_type 配置测量点类型 0=6000  1=脉冲

void Do_Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 data_dest,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type)//pAddr1是TSA，data_sour是01 05 06 XXXX 06 XXXX,data_dest 0X14 XXXXXXXX，
{
	u32 CTPT;
	u32 index;
	u64 value_sour;//存放data_sour的值，即新抄到的数据
	u64 value_dest;//存放之前data_dest里的值，即之前存放的数据
	u64 value_add=0;//增量
	u8 *psour=(u8 *)oad_data_addr;
	u8 *pdest=(u8 *)data_dest;
	u32 data_type=0xff;//0=double-long-unsigned  1=long64-unsigned
	
	CTPT=Get_CT_PT(pAddr);
	if(CTPT==0)
	{
		CTPT=0x00010001;
	}
	if((psour[0]!=DataType_array)||(psour[1]==0))
	{
		return;
	}
	if(psour[2]==DataType_double_long_unsigned)
	{
		data_type=0;
	}
	else if(psour[2]==DataType_long64_unsigned)
	{
		data_type=1;
	}
	else if(psour[2]==DataType_double_long)
	{
		data_type=2;
	}
	else if(psour[2]==DataType_long64)
	{
		data_type=3;
	}

	if(data_type==0xff)
		return;
	for(index=0;index<psour[1];index++)
	{
		if((data_type==0)||(data_type==2))//DataType_double_long_unsigned
		{
			value_sour=MRR(oad_data_addr+2+index*5+1,4);
			value_sour=R_Inverse(value_sour,4);
			if(pn_type==0)
				value_sour*=100;// KW -2 转成 0.1W
			value_sour*=(CTPT&0xffff);
			value_sour*=((CTPT>>16)&0xffff);
		}
		else//DataType_long64_unsigned
		{
			value_sour=MRR(oad_data_addr+2+index*9+1,8);
			value_sour=R_Inverse(value_sour,8);
			value_sour*=(CTPT&0xffff);
			value_sour*=((CTPT>>16)&0xffff);
		}
		if(pdest[index*9]==DataType_long64_unsigned)//说明之前data_dest里已经存过数据了
		{
			value_dest=MRR(data_dest+index*9+1,8);
			value_dest=R_Inverse(value_dest,8);
			if(value_dest>value_sour)//新的值比老的值小, 普通情况不存在，除非是电表已经记到最大了，又回到0
			{
				if(value_dest>9000000000)
				{//当前值回0
					value_add=value_sour;
					value_add+=10000000000;
					value_add-=value_dest;
				}
				else//错误，不应该
				{
					value_add=0;
				}
			}
			else//新的值比老的值大
			{
				value_add=value_sour-value_dest;
			}
			//value_add=1;//debug用，每次增量是1
			if(value_add!=0)//增量不等于0
			{
				if((group_type==0)&&(type==0))//总加标志 0=正向 1=反向
				{
					Update_ADDR_230x_x(ADDR_230x_7,LENper_230x_7,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_9,LENper_230x_9,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
				else if((group_type==0)&&(type==1))//总加标志 0=正向 1=反向
				{
					Update_ADDR_230x_x(ADDR_230x_8,LENper_230x_8,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_10,LENper_230x_10,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
				else if((group_type==1)&&(type==2))//总加标志 0=正向 1=反向
				{
					Update_ADDR_230x_x(ADDR_230x_7,LENper_230x_7,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_9,LENper_230x_9,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
				else if((group_type==1)&&(type==3))//总加标志 0=正向 1=反向
				{
					Update_ADDR_230x_x(ADDR_230x_8,LENper_230x_8,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_x(ADDR_230x_10,LENper_230x_10,value_add,TAG_NO,add_mise,index);
					Update_ADDR_230x_11(value_add,TAG_NO,index);
				}
			}
		}
		else//第一次进来，不算增量，当底度
		{
			pdest[index*9]=DataType_long64_unsigned;
		}
		//更新成新值
		MWR(value_sour,data_dest+index*9+1,8);
		Data_Inverse(data_dest+index*9+1,8);
	}
}
//u32 TAG_NO是总加组序号，从0开始；u32 add_mise是运算符标志0=加 1=减
//u32 type 0=正有 1=正无 2=反有 3=反无
//u32 group_type 总加标志 0=正向 1=反向
//u32 pn_type 配置测量点类型 0=6000  1=脉冲
void Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type)//把数据更新到ADDR_230x_Sour，之前没有该TSA的就新加
{
	u8 *p_data;
	u32 index;
	p_data=(u8 *)(ADDR_230x_Sour+LENper_230x_Sour*TAG_NO);
	if(p_data[0]>NUMmax_230x)
	{
		p_data[0]=NUMmax_230x;
	}
	for(index=0;index<p_data[0];index++)
	{
		if(Is_TSA_Same((u8 *)pAddr,(u8 *)(ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+index*LENsig_230x_sour))==0)
		{
			Do_Update_ADDR_230x_Sour(pAddr,oad_data_addr,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+index*LENsig_230x_sour+6+type*9*(TMaxTariff+1),TAG_NO,add_mise,type,group_type,pn_type);
			break;
		}
	}
	if(index>=p_data[0])//到这就说明之前没存过，要新加，加到p_data[0]位置，需判断是否是上限了
	{
		if(p_data[0]<(NUMmax_230x-1))
		{
			MW((u32)pAddr,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+p_data[0]*LENsig_230x_sour,6);
			MC(0,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+p_data[0]*LENsig_230x_sour+6,LENsig_230x_sour-6);//因为是新加，清下原来的数据。
			Do_Update_ADDR_230x_Sour(pAddr,oad_data_addr,ADDR_230x_Sour+LENper_230x_Sour*TAG_NO+1+p_data[0]*LENsig_230x_sour+6+type*9*(TMaxTariff+1),TAG_NO,add_mise,type,group_type,pn_type);
			p_data[0]+=1;//个数加1
		}
	}
}
//u32 TAG_NO是总加组序号，从0开始；u32 add_mise是运算符标志0=加 1=减;u32 fee 费率数 从0开始,0=总，1=尖...
//value的精度是0.1W,  0.1var
//u32 addr_data 更新数据的地址，u32 LENper
void Update_ADDR_230x_x(u32 addr_data,u32 LENper,u64 value,u32 TAG_NO,u32 add_mise,u32 fee)//更新总加电量
{
	u8 *p_data;
	u32 index;
	u64 value_sour;
	p_data=(u8 *)(addr_data+LENper*TAG_NO);
	if((p_data[0]==0)||(p_data[1]==0))
	{
		MC(0,addr_data+LENper*TAG_NO,LENper);
		p_data[0]=DataType_array;
	}
	p_data[2+fee*9]=DataType_long64;
	value_sour=MRR(addr_data+LENper*TAG_NO+2+fee*9+1,8);
	value_sour=R_Inverse(value_sour,8);
	if(value_sour>>63)//原来是负数了
	{
		value_sour=(~value_sour)+1;//转成正数
		if(add_mise==0)//加
		{
			if(value_sour>value)//原值加上增量还是负的
			{
				value_sour-=value;
				value_sour=(~value_sour)+1;
			}
			else//原值加上增量后变成正直了
			{
				value_sour=value-value_sour;
			}
		}
		else//负值减去一个值等于负值
		{
			value_sour+=value;
			value_sour=(~value_sour)+1;
		}
	}
	else//原来是正数
	{
		if(add_mise==0)//加
		{
			value_sour+=value;
		}
		else//减
		{
			if(value_sour>=value)
			{
				value_sour-=value;
			}
			else
			{
				value_sour=value-value_sour;
				value_sour=(~value_sour)+1;
			}
		}
	}

	MWR(value_sour,addr_data+LENper*TAG_NO+2+fee*9+1,8);
	Data_Inverse(addr_data+LENper*TAG_NO+2+fee*9+1,8);
	value_sour=0;
	for(index=0;index<(TMaxTariff+1);index++)
	{
		if(p_data[2+index*9]==DataType_long64)
			value_sour+=1;
	}
	p_data[1]=value_sour;
	return;
}

void Update_ADDR_230x_11(u64 value,u32 TAG_NO,u32 fee)//总加剩余电量（费）
{
	u8 *p_data;
	u64 value_sour;
	u32 i;//0=购电量,1=购电费
	u32 j;

	i=MRR(ADDR_Terminal_kWhCtrl2_State+TAG_NO*Terminal_Ctrl_Pn_Len,1);
	if(i==0)//该总加组的购电控没有投入，不计算了。
	{
		return;
	}
	i=MRR(ADDR_Terminal_kWhCtrl2_State+TAG_NO*Terminal_Ctrl_Pn_Len+7,1);
	if(i==1)//是剩余电费，需要找出当前时间是费率数和对应的费率
	{
		if(fee==0)//有可能是电费，要按费率时段把电算成钱，所以不管总。
	  {
			return;
		}	
		i=MRR(ADDR_METER07_CTRL_SRAM,1);//1BYTE 电能表当前执行费率(HEX 费率1,2,3,4 分别对应尖峰,峰,平,谷等费率)
		j=Get_ElementADDR(ADDR_4018,i,0,LENmax_4018);//定位费率电价
		if(j!=0)
		{
			j=MRR(j+1,4);//单位：元/kWh，换算：-4,double-long-unsigned
			j=R_Inverse(j,4);
			value*=j;//电量增量乘以费率就等于电费
		}
		//如果不存在，就相当于费率是1
	}
	else//是剩于电量
	{
		if(fee!=0)//就计算总，台体数据只有总
		{
			return;
		}
	}
	p_data=(u8 *)(ADDR_230x_11+LENper_230x_11*TAG_NO);
	if(p_data[0]==0)
	{
		MC(0,ADDR_230x_11+LENper_230x_11*TAG_NO,LENper_230x_11);
		p_data[0]=DataType_long64;
	}
	value_sour=MRR(ADDR_230x_11+LENper_230x_11*TAG_NO+1,8);
	value_sour=R_Inverse(value_sour,8);
	if(value_sour>>63)//原来是负数了
	{
		value_sour=(~value_sour)+1;//转成正数
		value_sour+=value;
		value_sour=(~value_sour)+1;//转成负数
	}
	else//原来是正数
	{
		if(value_sour>=value)
		{
			value_sour-=value;
		}
		else
		{
			value_sour=value-value_sour;
			value_sour=(~value_sour)+1;//转成负数
		}
	}
	MWR(value_sour,ADDR_230x_11+LENper_230x_11*TAG_NO+1,8);
	Data_Inverse(ADDR_230x_11+LENper_230x_11*TAG_NO+1,8);
}

void Update_Group_Total_Data_6000(u8 *FnDataBuff)//更新总加组的数据,配置表是6000里的
{
	u32 TAG_NO;//总加组序号，从0开始
	u32 index;
	u8 *p_data;
	u8 *p8;//指向Get_Element的执行结果，=0表示没找到
	u32 oad_data_addr=0;//oad对应数据在pbuf里的地址，=0表示没找到
	u32 add_mise=0;//运算符标志 0=加 1=减
	u32 group_type=0;//总加标志 0=正向 1=反向
	ms_Type *ms;
	
	ms=Get_ms();
	p_data=ms->ms_malloc((size_t)LENmax_230x_2);//976字节
	if((u32)p_data==0)//动态分配失败
	{
		return;
	}
 	MR((u32)p_data,ADDR_230x_2,LENmax_230x_2);
	
	for(TAG_NO=0;TAG_NO<NUMmax_TAG;TAG_NO++)
	{
		if(p_data[LENper_230x_2*TAG_NO]==DataType_array)//该总加组设置了参数
		{
			for(index=0;index<p_data[LENper_230x_2*TAG_NO+1];index++)//从总加组配置参数里找TSA
			{
				p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,1,LENper_230x_2);
				if(p8!=0)
				{
					if(Is_TSA_Same((u8 *)(p8+3),(u8 *)FnDataBuff)==0)//该总加组里配置了该TSA
					{
						p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,2,LENper_230x_2);//获取总加标志
						if(p8!=0)
						{
							group_type=p8[1];//enum{正向（0），反向（1）}，
						}
						p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,3,LENper_230x_2);//获取运算符标志
						if(p8!=0)
						{
							add_mise=p8[1];//enum{加（0），减（1）}
						}	
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00100200,FnDataBuff);//判断FnDataBuff是否存在总加组的数据，返回 0=不存在oad的数据，!=0，oad对应的数据地址
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,0,group_type,0);
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00200200,FnDataBuff);//判断FnDataBuff是否存在总加组的数据，返回 0=不存在oad的数据，!=0，oad对应的数据地址
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,2,group_type,0);
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00300200,FnDataBuff);//判断FnDataBuff是否存在总加组的数据，返回 0=不存在oad的数据，!=0，oad对应的数据地址
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,1,group_type,0);
						oad_data_addr=Is_FnDataBuff_Have_TAGData(0x00400200,FnDataBuff);//判断FnDataBuff是否存在总加组的数据，返回 0=不存在oad的数据，!=0，oad对应的数据地址
						if(oad_data_addr!=0)
							Update_ADDR_230x_Sour((u16 *)FnDataBuff,oad_data_addr,TAG_NO,add_mise,3,group_type,0);
					}
				}
			}
		}
	}
	ms->ms_free(p_data);
}
void Update_Group_Total_Data_240x(void)//更新总加组的数据,配置表是脉冲量
{
	u32 TAG_NO;//总加组序号，从0开始
	u32 index;
	u8 *p_data;
	u8 *p_240x;
	u8 *p8;//指向Get_Element的执行结果，=0表示没找到
	u32 add_mise=0;//运算符标志 0=加 1=减
	u32 group_type=0;//总加标志 0=正向 1=反向
	u32 imp_type=0;//脉冲属性  正向有功（0），正向无功（1） 反向有功（2），反向无功（3）
	u32 k;
	
	ms_Type *ms;
	ms=Get_ms();
	p_data=ms->ms_malloc((size_t)LENmax_230x_2);//976字节
	if((u32)p_data==0)//动态分配失败
	{
		return;
	}
	MR((u32)p_data,ADDR_230x_2,LENmax_230x_2);
	
	p_240x=ms->ms_malloc((size_t)((ADDR_DL698_SET_END)-(ADDR_240x_2)));//976字节
	if((u32)p_240x==0)//动态分配失败
	{
		ms->ms_free(p_data);
		return;
	}
	MR((u32)p_240x,ADDR_240x_2,((ADDR_DL698_SET_END)-(ADDR_240x_2)));

	
	for(TAG_NO=0;TAG_NO<NUMmax_TAG;TAG_NO++)
	{
		if(p_data[LENper_230x_2*TAG_NO]==DataType_array)//该总加组设置了参数
		{
			for(index=0;index<p_data[LENper_230x_2*TAG_NO+1];index++)
			{
				p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,1,LENper_230x_2);//找出总加组里的TSA
				if(p8!=0)
				{
				#if NUMmax_IMPIN!=0
					for(k=0;k<NUMmax_IMPIN;k++)
					{
						if(Is_TSA_Same((u8 *)(p8+3),(u8 *)(p_240x+k*LENper_240x_2+2))==0)//总加组里的TSA和脉冲的地址一样
						{
							p8=Get_Element(p_240x+LENmax_240x_2+LENmax_240x_3+k*LENper_240x_4,1,2,LENper_240x_4);//获取总加标志
							if(p8!=0)
							{
								imp_type=p8[1];//脉冲属性  正向有功（0），正向无功（1） 反向有功（2），反向无功（3）
							}
							p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,2,LENper_230x_2);//获取总加标志
							if(p8!=0)
							{
								group_type=p8[1];//enum{正向（0），反向（1）}，
							}
							p8=Get_Element(p_data+LENper_230x_2*TAG_NO,index+1,3,LENper_230x_2);//获取运算符标志
							if(p8!=0)
							{
								add_mise=p8[1];//enum{加（0），减（1）}
							}
							Update_ADDR_230x_Sour((u16 *)(p_240x+k*LENper_240x_2+2),ADDR_impEnergy+k*LENper_impEnergy,TAG_NO,add_mise,imp_type,group_type,1);
						}
					}
				#else
					add_mise=add_mise;
					group_type=group_type;
					imp_type=imp_type;
					k=k;
				#endif
				}
			}
		}
	}
	ms->ms_free(p_data);
	ms->ms_free(p_240x);
	Calc_TAG_Power();
}

void Calc_TAG_Power(void)//计算全部总加组的功率
{
	u32 index;
	for(index=0;index<NUMmax_TAG;index++)
	{
		Calc_PerTAG_Power(index+1,0);
		Calc_PerTAG_Power(index+1,1);
	}
}
void Calc_PerTAG_Power(u32 TAG_NO,u32 Power_type)//计算单一总加的功率，TAG_NO=总加组编号 1--8，Power_type=功率类型，0=有功功率 1=无功功率
{
	u8 *p_data;
	u8 *p8;
	u32 i;
	u32 j;
	u32 index;
	u64 a64=0;
	u64 b64;
	u32 CTPT;
	ms_Type *ms;
	ms=Get_ms();
	p_data=ms->ms_malloc((size_t)((LENmax_230x_2)+(LENmax_240x_2)+(LENmax_240x_4)));//976字节
	if((u32)p_data==0)//动态分配失败
	{
		return ;
	}
 	MR((u32)p_data,ADDR_230x_2,LENmax_230x_2);
	MR((u32)p_data+LENmax_230x_2,ADDR_240x_2,LENmax_240x_2);
	MR((u32)p_data+LENmax_230x_2+LENmax_240x_2,ADDR_240x_4,LENmax_240x_4);
	i=MRR((u32)p_data+(TAG_NO-1)*LENper_230x_2,2);
	if(((i&0xff)!=DataType_array)||((i>>8)==0))//获取的总加组不存在有效参数
	{
		goto err;
	}
	for(index=0;index<p_data[(TAG_NO-1)*LENper_230x_2+1];index++)
	{
		p8=Get_Element(p_data+(TAG_NO-1)*LENper_230x_2,index+1,1,LENper_230x_2);//指向TSA
		if(p8!=0)
		{
			CTPT=Get_CT_PT((u16 *)(p8+3));
			if(CTPT==0)
			{
				CTPT=0x00010001;
			}
			i=AddrToMeterSetNo((u16 *)(p8+3));
			if(i<NUM_RMmax)//在配置档案里面
			{
				p8=Get_Element((u8 *)(ADDR_6000_SDRAM+LENper_6000*i), 2,4,LENper_6000);
				if(p8==0)
				{
					goto err;
				}
				j=MRR((u32)(p8+1),4);
				j=R_Inverse(j,4);
				if((j&0xffffff00)==0xf20a0200)//在6000里端口是秒冲，也不算在6000里面
				{
					i=NUM_RMmax;
				}
			}
			p8=Get_Element(p_data+(TAG_NO-1)*LENper_230x_2,index+1,1,LENper_230x_2);//指向TSA
			if(i<NUM_RMmax)//在配置档案里面
			{
				b64=Get_6000_Power(i,Power_type);
				if(b64==INVALID_DATA)//没有功率数据
				{
					goto err;
				}
				b64*=(CTPT&0xffff);
				b64*=((CTPT>>16)&0xffff);
				a64+=b64;
			}
			else//只可能是脉冲测量点了。
			{
			#if NUMmax_IMPIN!=0
				for(j=0;j<NUMmax_IMPIN;j++)
				{
					if(Is_TSA_Same((u8 *)(p8+3),(u8 *)((u32)p_data+LENmax_230x_2+j*LENper_240x_2+2))==0)
					{
						break;
					}
				}
				if(j>=NUMmax_IMPIN)//不在6000里，也不是脉冲测量点，这总加组参数不对，直接返回错误
				{
					goto err;
				}
				p8=Get_Element((u8 *)((u32)p_data+LENmax_230x_2+LENmax_240x_2+j*LENper_240x_4),1,2,LENper_240x_4);
				if(p8==0)
				{
					goto err;
				}
				i=p8[1];//脉冲属性		enum
				if((Power_type==0)&&((i==1)||(i==3)))//脉冲属性不匹配的
				{
					b64=0;
				}
				else if((Power_type==1)&&((i==0)||(i==2)))//脉冲属性不匹配的
				{
					b64=0;
				}
				else
				{
					i=MRR(ADDR_240x_5+j*LENper_240x_5+1,4);
					i=R_Inverse(i,4);
					b64=i;
					b64*=(CTPT&0xffff);
					b64*=((CTPT>>16)&0xffff);
				}
				a64+=b64;
			#else
				goto err;
			#endif
			}
		}
	}
	if(Power_type==0)
	{
		MWR(DataType_long64,ADDR_230x_3+(TAG_NO-1)*LENper_230x_3,1);
		MWR(a64,ADDR_230x_3+(TAG_NO-1)*LENper_230x_3+1,8);
		Data_Inverse(ADDR_230x_3+(TAG_NO-1)*LENper_230x_3+1,8);
	}
	else
	{
		MWR(DataType_long64,ADDR_230x_4+(TAG_NO-1)*LENper_230x_4,1);
		MWR(a64,ADDR_230x_4+(TAG_NO-1)*LENper_230x_4+1,8);
		Data_Inverse(ADDR_230x_4+(TAG_NO-1)*LENper_230x_4+1,8);
	}
err:
	ms->ms_free(p_data);
}

u64 Get_6000_Power(u32 pn,u32 Power_type)//获取测量点的功率 Power_type=功率类型，0=有功功率 1=无功功率
{
	u64 res=INVALID_DATA;
	u8 pRCSD[]={DataType_RCSD,0x01,0x00,0x00,0x00,0x00,0x00};
	u32 task_id;//包含oad的任务号
	u32 data_addr;//最近一条记录oad数据的存放地址
	u32 data_len;
	u64 tsa;
	u8 *p_data;
	u8 *pp;
	u32 oad;
	
	if(Power_type==0)
		oad=0x20040200;
	else
		oad=0x20050200;
	tsa=Get_TSA_BY_PN_noInverse(pn);
	if(tsa==INVALID_DATA)//获取测量点地址失败
	{
		return INVALID_DATA;
	}
	//第一步 根据oad 读出全部由的数据。
	pRCSD[3]=(oad>>24)&0xff;
	pRCSD[4]=(oad>>16)&0xff;
	pRCSD[5]=(oad>>8)&0xff;
	task_id=RCSDtoID(pRCSD);//记录列选择RCSD相对应的任务号,返回:数据在采集任务的任务号,>=RMTASKmax表示无效任务号
	if(task_id>=RMTASKmax)//要查询的OAD不在任何一个任务里面，肯定是没有数据的。
	{
		return INVALID_DATA;
	}
	#if (USER/100)==15//安徽
		data_addr=Get_LastRecordFileDataHasID(task_id,(u16* )&tsa);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
	#else
		data_addr=Get_LastRecordFileData(task_id,(u16* )&tsa);//得到最近次记录文件数据(先找文件计数0再找文件计数1):入口:ID=任务号,pTSA=无类型的电表地址;返回:读入到ADDR_128KDATABUFF的数据长度，0=没找到
	#endif
	if(data_addr==0)//在数据记录里面没有该TSA的数据
	{
		return INVALID_DATA;
	}
	data_len=Get_RecordFileData_RCSD(pRCSD+1,(u8 * )&tsa,(u8 * )ADDR_128KDATABUFF,(u8 *)ADDR_DATABUFF,1024,0);//记录表的列选择;入口:pRCSD指向接收的RCSD(无数据类型),pAddr,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
	if(data_len==0)
	{
		return INVALID_DATA;
	}
	//到这，数据都已经读到ADDR_DATABUFF
	//
	p_data=(u8 *)ADDR_DATABUFF;
	if(p_data[0]==DataType_array)//因为前面列选时是选全部的，出来的应该是数组的。
	{
		pp=Get_Element(p_data,1,0,LEN_DATABUFF);
		if(pp!=0)
		{
			switch(pp[0])
			{
				case DataType_long_unsigned:
				case DataType_long:
					res=(pp[1]<<8)+pp[2];
					break;
				case DataType_double_long:
				case DataType_double_long_unsigned:
					res=(pp[1]<<24)+(pp[2]<<16)+(pp[3]<<8)+pp[4];
//					res/=1000;
					break;
				default:
					break;
			}
		}
		else
		{
			res=INVALID_DATA;
		}
	}
	else
	{
		res=INVALID_DATA;
	}
	return res;
}

u64 Get_TSA_BY_PN_noInverse(u32 Pn)//根据配置序号，得到TSA。不包含数据类型。不调换顺序
{
	u32 Addr;
	u64 Meter;
	u8 *p8;
	u32 Len;
	
	Addr=ADDR_6000+LENper_6000*Pn;
	p8=Get_Element((u8 *)Addr,2,1,LENper_6000);//计算元素地址,使用ADDR_128K_DATABUFF
	if(p8==0)
	{
		return INVALID_DATA;
	}
	Len=p8[1];
	if(Len==0)
	{
		return 0;
	}
	MR(ADDR_DATABUFF,(u32)p8+3,Len);
	if(Len>8)
		Meter=MRR(ADDR_DATABUFF,8);
	else
		Meter=MRR(ADDR_DATABUFF,Len);
	return Meter;
}

