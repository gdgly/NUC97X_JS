

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/CALC_SwitchIn.h"
#include "../DL698/EVENT_Record.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Device/MEMRW.h"




__align(4) const u16 PIN_SwitchIn_List[]=//终端开关量输入引脚列表
{
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,4=三合一,500=集中器Ⅱ型,600=通信模块,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	//PIN_DOOR_IN,//门接点
	#endif
	
	#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,4=三合一,500=集中器Ⅱ型,600=通信模块,700=
	PIN_SWIN1,
	PIN_SWIN2,
//	PIN_SWIN3,
//	PIN_SWIN4,
	#if (USER/100)==15//安徽
		#if NUMmax_SWITCHIN!=3
			#error
		#endif
	PIN_DOOR_IN,//门接点
	#endif
	#endif
	
	#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,4=三合一,500=集中器Ⅱ型,600=通信模块,700=
	PIN_SWIN1,
	PIN_SWIN2,
	PIN_SWIN3,
	PIN_SWIN4,
	//PIN_DOOR_IN,//门接点
	#endif
};

u32 Get_SwitchIn_Route(void)//得到开关量输入路数
{
	u32 i;
	
 #if (Project/100)!=4//方案0=智能表,100=网络表,200=集中器,300=专变终端,4=三合一,500=集中器Ⅱ型,600=通信模块,700=
	i=sizeof(PIN_SwitchIn_List)/2;
	if(i>NUMmax_SWITCHIN)//最大开关量输入端口数(脉冲输入端口数+门接点)
	{
		i=NUMmax_SWITCHIN;//最大开关量输入端口数(脉冲输入端口数+门接点)
	}
	return i;
 #else
	#if NUMmax_SWITCHIN!=4
		#error
	#endif
	if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
	{
		i=2;
		return i;
	}
	else
	{
		i=4;
		return i;
	}
 #endif
}

void Init_SwitchIn(void)//初始化终端开关量输入
{
	u32 i;
	u32 x;
	u32 y;
	u32 set1;
	u32 set2;
	u32 MaxPort;
	u8 *p8;
	
	MaxPort=Get_SwitchIn_Route();//得到开关量输入路数
//读引脚
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//读引脚,入口引脚使用名;出口0或1
	}
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}

	x=~x;//置"O"："分"状态；置"l"："合"状态
	x&=y;

//接入和分合状态
	set1=MRR(ADDR_F203_4+4,4);
	set2=set1>>24;//开关量属性标志
	set1&=0xff;//开关量接入标志
	for(i=0;i<MaxPort;i++)
	{
		if((set1&(0x80>>i))==0x0)
		{//未接入=置"O"："分"状态
			x&=~(1<<i);
		}
		else
		{//接入
			if((set2&(0x80>>i))==0x0)
			{//置"0"：常闭触点
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
	//存状态
	p8=(u8*)ADDR_F203_2;
	p8[0]=DataType_array;
	p8[1]=MaxPort;
	p8+=2;
	for(i=0;i<MaxPort;i++)
	{
		p8[0]=DataType_structure;
		p8[1]=2;
		p8[2]=DataType_unsigned;
		p8[3]=x&0x1;
		p8[4]=DataType_unsigned;
		p8[5]=0;
		p8+=6;
		x>>=1;
	}
}

void CALC_SwitchIn(void)//开关量输入
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 f;
	u32 set1;
	u32 set2;
	u32 MaxPort;
	u8* p8s;
	u8* p8d;
	
	MaxPort=Get_SwitchIn_Route();//得到开关量输入路数
//读引脚
	x=0;
	for(i=0;i<MaxPort;i++)
	{
		x|=(Pin_Read(PIN_SwitchIn_List[i])<<i);//读引脚,入口引脚使用名;出口0或1
	}
	y=0;
	for(i=0;i<MaxPort;i++)
	{
		y|=(1<<i);
	}
	x=~x;//置"O"："分"状态；置"l"："合"状态
	x&=y;
//输入滤波
	f=0;//滤波OK标志
	p8s=(u8*)ADDR_SwitchIn;//开关量输入
	for(i=0;i<MaxPort;i++)
	{
		if((x&(1<<i))!=p8s[2*i])
		{
			p8s[2*i]=x&(1<<i);//开关量输入原状态
			p8s[(2*i)+1]&=0xf0;//开关量输入:b0-b3滤波次数,b4-b7接入有效后稳定次数
		}
		else
		{
			if((p8s[(2*i)+1]&0x0f)<3)//约1秒
			{
				p8s[(2*i)+1]++;//开关量输入:b0-b3滤波次数,b4-b7接入有效后稳定次数
			}
			else
			{
				f|=(1<<i);//滤波OK标志
			}
		}
	}
	if(f!=y)
	{
		return;//没稳定
	}
	
//接入和分合状态
	set1=MRR(ADDR_F203_4+4,4);
	set2=set1>>24;//开关量属性标志
	set1&=0xff;//开关量接入标志
	for(i=0;i<MaxPort;i++)
	{
		if((set1&(0x80>>i))==0x0)
		{//未接入=置"O"："分"状态
			x&=~(1<<i);
			p8s[(2*i)+1]=0;//开关量输入:b0-b3滤波次数,b4-b7接入有效后稳定次数
		}
		else
		{//接入
			if((set2&(0x80>>i))==0x0)
			{//置"0"：常闭触点
				y=x;
				x&=~(1<<i);
				y=~y;
				y&=(1<<i);
				x|=y;
			}
		}
	}
	//存状态
	p8d=(u8*)ADDR_F203_2;
	p8d[0]=DataType_array;
	p8d[1]=MaxPort;
	p8d+=2;
	f=0;
	for(i=0;i<MaxPort;i++)
	{
		p8d[0]=DataType_structure;
		p8d[2]=DataType_unsigned;
		p8d[4]=DataType_unsigned;
		p8d[1]=2;
		y=p8d[3];
		set2=(x>>i)&0x1;
		p8d[3]=set2;
		z=p8s[(2*i)+1];//开关量输入:b0-b3滤波次数,b4-b7接入有效后稳定次数
		if(z<0x20)
		{
			z+=0x10;
			p8s[(2*i)+1]=z;
		}
		z>>=4;
		if(z>=2)
		{//接入有效后至少2次采集后判断变位
			if(y^set2)
			{//变位
				p8d[5]=1;
				f|=1<<i;
			}
		}
		p8d+=6;
	}
	
	if(f!=0x0)
	{//有变位
		Event_OAD(0x31040200,0,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
	}
}

void CLR_SwitchIn_CD(void)//清开关量输入变位CD
{
	u32 i;
	u32 MaxPort;
	u8* p8;
	
	MaxPort=Get_SwitchIn_Route();//得到开关量输入路数
	p8=(u8*)ADDR_F203_2+2;
	for(i=0;i<MaxPort;i++)
	{
		p8[5]=0;
		p8+=6;
	}
}


//#define ADDR_F203_4     ADDR_F101_3+LENmax_F101_3
//属性4∷=structure
//{
//开关量接入标志bit-string(SIZE（8）)
//（
//bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”：接入，置“0”：未接入。
//），
//开关量属性标志bit-string(SIZE（8）)
//（
//  bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”常开触点。置“0”：常闭触点。
//）
//}

//#define ADDR_F203_2     ADDR_F100_13+LENmax_F100_13
//属性2（设备对象列表，只读）∷=array 开关量单元
//开关量单元∷=structure
//{
	//状态ST  unsigned，
	//变位CD  unsigned
//}
//状态ST——0：“分”状态；1：“合”状态。
//变位CD——0：自前次遥信传送后无状态变化；1：自前次遥信传送后至少有一次状态变化。





















