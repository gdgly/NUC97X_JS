
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_ACTION_8000.h"
#include "../DL698/DL698_ACTION_6000.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/Contrl_Manage.h"
#include "../DL698/EVENT_Record.h"




//遥控
u32 ACTION_8000(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 y;
	u32 n;
	u32 f;
	u32 OAD;
	u8* p8;
	u8* p8d;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	
	LenRx=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
	if(p8rx[4+LenRx]==0)
	{//无时间标签
		UARTCtrl->DAR=32;//时间标签无效          (32)
		return (LenRx<<16)+LenTx;
	}
	i=Uart_TimeTag(p8rx+4+LenRx+1);//时间标签有效性判断;入口:p8rx指向时间标签;返回:0=有效,1=无效
	if(i)
	{
		UARTCtrl->DAR=32;//时间标签无效          (32)
		return (LenRx<<16)+LenTx;
	}
	
	i=p8rx[2];
	switch(i)
	{
		case 127://方法127：触发告警（参数）参数∷=NULL
			MC(0x55,ADDR_8000_127,1);
			return (LenRx<<16)+LenTx;
		case 128://方法128：解除报警（参数）参数∷=NULL
			MC(0,ADDR_8000_127,1);
			return (LenRx<<16)+LenTx;
		case 129://方法129：跳闸（参数）参数∷=array structure
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(i>LENmax_8000_129)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+0;
			}
			//读原跳闸参数
			MR(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			p8=p8rx;
			n=p8[1];//当前命令数组数
			if(n>NUMmax_CR)
			{
				n=NUMmax_CR;
			}
			p8+=2;
			while(n--)
			{
				OAD=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//继电器OAD
				//限电时间
				i=OAD&0xff;//继电器号
				if((i!=0)&&(i<=NUMmax_CR))
				{
					i--;
					x=(p8[10]<<8)|p8[11];//限电时间    long-unsigned（单位：分钟，换算：0；值为0表示永久限电）
					if(x)
					{//有限电时间
						MWR(x,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+22+(i*2),2);
					}
				}
				p8d=(u8*)ADDR_DATABUFF;
				x=p8d[1];//数组数
				if(x>NUMmax_CR)
				{
					x=NUMmax_CR;
					p8d[1]=x;//数组数
				}
				p8d+=2;
				for(i=0;i<x;i++)
				{
					f=(p8d[3]<<24)|(p8d[4]<<16)|(p8d[5]<<8)|p8d[6];//继电器OAD
					if(f==OAD)
					{//原已经有
						MW((u32)p8,(u32)p8d,LENper_8000_129);
						break;
					}
					p8d+=LENper_8000_129;
				}
				if(i>=x)
				{//没找到
					x++;//数组数+1
					if(x<=NUMmax_CR)
					{
						MW((u32)p8,(u32)p8d,LENper_8000_129);
						p8d=(u8*)ADDR_DATABUFF;
						p8d[0]=DataType_array;
						p8d[1]=x;//数组数
					}
				}
				i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
				p8+=i;
			}
			MW(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);

			//修改合闸参数
			f=0;//合闸参数变化标志
			MR(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);
			n=p8rx[1];//数组数
			if(n>NUMmax_CR)
			{
				n=NUMmax_CR;
			}
			p8rx+=2;
			while(n--)
			{
				OAD=(p8rx[3]<<24)|(p8rx[4]<<16)|(p8rx[5]<<8)|p8rx[6];//继电器OAD
				p8=(u8*)ADDR_DATABUFF;
				x=p8[1];//数组数
				if(x>NUMmax_CR)
				{
					x=NUMmax_CR;
					p8[1]=x;//数组数
				}
				p8+=2;
				for(y=0;y<x;y++)
				{
					i=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//继电器OAD
					if(i==OAD)
					{
						//删除合闸参数
						i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
						MR((u32)p8,((u32)p8)+i,LENmax_8000_130);
						x--;
						p8=(u8*)ADDR_DATABUFF;
						p8[1]=x;//数组数
						f=1;//参数变化标志
//						p8[3]=0;
//						p8[4]=0;
//						p8[5]=0;
//						p8[6]=0;
						break;
					}
					i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
					p8+=i;
				}
				i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				p8rx+=i;
			}
			if(f)
			{//参数变化标志
				MW(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);
			}

		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Remote_Control();//终端控制-遥控跳闸
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Remote_Control();//终端控制-遥控跳闸
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 130://方法130：合闸（参数）参数∷=array structure
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(i>LENmax_8000_130)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+LenTx;
			}
			//读原合闸参数
			MR(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);
			p8=p8rx;
			n=p8[1];//当前命令数组数
			p8+=2;
			while(n--)
			{
				OAD=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//继电器OAD
				p8d=(u8*)ADDR_DATABUFF;
				x=p8d[1];//数组数
				p8d+=2;
				for(i=0;i<x;i++)
				{
					f=(p8d[3]<<24)|(p8d[4]<<16)|(p8d[5]<<8)|p8d[6];//继电器OAD
					if(f==OAD)
					{
						
						break;
					}
					f=Get_DL698DataLen_S(p8d,0);//得到DL698数据类型长度(包括:类型字节和数据)
					p8d+=f;
				}
				if(i>=x)
				{//没找到
					x++;//数组数+1
					if(x<=NUMmax_CR)
					{
						i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
						MW((u32)p8,(u32)p8d,i);
						p8d=(u8*)ADDR_DATABUFF;
						p8d[0]=DataType_array;
						p8d[1]=x;//数组数
					}
				}
				i=Get_DL698DataLen_S(p8,0);//得到DL698数据类型长度(包括:类型字节和数据)
				p8+=i;
			}
			MW(ADDR_DATABUFF,ADDR_8000_130,LENmax_8000_130);

			//修改跳闸参数
			f=0;//跳闸参数变化标志
			MR(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			n=p8rx[1];//数组数
			if(n>NUMmax_CR)
			{
				n=NUMmax_CR;
			}
			p8rx+=2;
			while(n--)
			{
				OAD=(p8rx[3]<<24)|(p8rx[4]<<16)|(p8rx[5]<<8)|p8rx[6];//继电器OAD
				p8=(u8*)ADDR_DATABUFF;
				x=p8[1];//数组数
				if(x>NUMmax_CR)
				{
					x=NUMmax_CR;
					p8[1]=x;//数组数
				}
				p8+=2;
				for(y=0;y<x;y++)
				{
					i=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//继电器OAD
					if(i==OAD)
					{
						//删除跳闸参数
						f=1;//参数变化标志
						MR((u32)p8,((u32)p8)+LENper_8000_129,LENmax_8000_129);
						x--;
						p8=(u8*)ADDR_DATABUFF;
						p8[1]=x;//数组数
						break;
					}
					p8+=LENper_8000_129;
				}
				i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				p8rx+=i;
			}
			if(f)
			{//参数变化标志
				MW(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			}
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Remote_Control();//终端控制-遥控跳闸
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Remote_Control();//终端控制-遥控跳闸
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 131://方法131：电表明文合闸（参数） 参数∷=array structure
	#if (Project/100)<=2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(i>LENmax_8000_130)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+0;
			}
			MW((u32)p8rx,ADDR_8000_130,i);
			//修改跳闸参数
			f=0;//跳闸参数变化标志
			MR(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			n=p8rx[1];//数组数
			p8rx+=2;
			while(n--)
			{
				OAD=(p8rx[3]<<24)|(p8rx[4]<<16)|(p8rx[5]<<8)|p8rx[6];//继电器OAD
				p8=(u8*)ADDR_DATABUFF;
				x=p8[1];//数组数
				p8+=2;
				while(x--)
				{
					i=(p8[3]<<24)|(p8[4]<<16)|(p8[5]<<8)|p8[6];//继电器OAD
					if(i==OAD)
					{
						f=1;//参数变化标志
						p8[3]=0;
						p8[4]=0;
						p8[5]=0;
						p8[6]=0;
						break;
					}
					p8+=LENper_8000_129;
				}
				i=Get_DL698DataLen_S(p8rx,0);//得到DL698数据类型长度(包括:类型字节和数据)
				p8rx+=i;
			}
			if(f)
			{//参数变化标志
				MW(ADDR_DATABUFF,ADDR_8000_129,LENmax_8000_129);
			}
	#else
			UARTCtrl->DAR=255;
	#endif
			return (LenRx<<16)+LenTx;
			
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//保电
u32 ACTION_8001(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	
	LenRx=Get_DL698DataLen_S(p8rx+4,0);//得到DL698数据类型长度(包括:类型字节和数据)
	if(p8rx[4+LenRx]==0)
	{//无时间标签
		UARTCtrl->DAR=32;//时间标签无效          (32)
		return (LenRx<<16)+LenTx;
	}
	i=Uart_TimeTag(p8rx+4+LenRx+1);//时间标签有效性判断;入口:p8rx指向时间标签;返回:0=有效,1=无效
	if(i)
	{
		UARTCtrl->DAR=32;//时间标签无效          (32)
		return (LenRx<<16)+LenTx;
	}
	
	i=p8rx[2];
	switch(i)
	{
		case 127://方法127：投入保电（参数）参数∷=NULL
			MC(0x55,ADDR_8001_127,1);
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl_Protect();//终端控制-保电
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl_Protect();//终端控制-保电
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 128://方法128：解除保电（参数）参数∷=NULL
			MC(0,ADDR_8001_127,1);
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl_Protect();//终端控制-保电
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl_Protect();//终端控制-保电
			}
		#endif
			return (LenRx<<16)+LenTx;
		case 129://方法129：解除自动保电（参数）参数∷=NULL
			MWR(DataType_long_unsigned,ADDR_8001_3,3);//保电-允许与主站最大无通信时长（分钟）
			MWR(DataType_long_unsigned,ADDR_8001_4,3);//保电-上电自动保电时长（分钟）
			MWR(DataType_array,ADDR_8001_5,2);//保电-自动保电时段
		#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			Terminal_Contrl_Protect();//终端控制-保电
		#endif
		#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==0)//三合一终端当前运行模式:0=专变,1=集中器
			{
				Terminal_Contrl_Protect();//终端控制-保电
			}
		#endif
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//催费告警
u32 ACTION_8002(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 127://方法127：催费告警投入（参数）
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			if(i>LENmax_8002_127)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+0;
			}
			MW((u32)p8rx,ADDR_8002_127,i);
			Terminal_Contrl_Out();//终端控制-输出.继电器和LED
			return (LenRx<<16)+LenTx;
		case 128://方法128：取消催费告警（参数）参数∷=NULL
			MC(0,ADDR_8002_127,1);
			LenRx=1;
			Terminal_Contrl_Out();//终端控制-输出.继电器和LED
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//时段功控
u32 ACTION_8103(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://添加控制单元
		case 5://更新控制单元
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx!=LENper_8103_2)
			{
				UARTCtrl->DAR=7;//类型不匹配           （7）
				return (LenRx<<16)+LenTx;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_2;
			}
			if(i<Array)
			{//找到
				MW((u32)p8rx,(u32)p8,LENper_8103_2);
				MW(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			}
			else
			{//没找到
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8103_2*Array),LENper_8103_2);
					MW(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
				}
				else
				{
					UARTCtrl->DAR=8;//越界                 （8）
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://删除控单元
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_2;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8103_2,LENmax_8103_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8103_2,LENmax_8103_2);
			}
			else
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
			}
	//控制投入状态
			MR(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_3;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8103_3,LENmax_8103_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			}
	//时段功控方案切换
			MR(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_127,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_127;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8103_127,LENmax_8103_127);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			}
			return (LenRx<<16)+LenTx;
		case 6://控制投入
		case 7://控制解除
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_3;
			}
			if(i>Array)
			{//没找到
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8103_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{未投入（0），投入（1）}
			}
			else
			{
				p8[6]=0;//{未投入（0），投入（1）}
			}
			MW(ADDR_128KDATABUFF,ADDR_8103_3,LENmax_8103_3);
			return (LenRx<<16)+LenTx;
		case 127://方法127：时段功控方案切换（总加组对象，控制方案）
//时段功控方案切换∷=structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//时段功控投入标识    bit-string(SIZE(8))，
//时段功控定值方案号  unsigned
//}
//时段功控投入标识：D0～D7按顺序对位表示第1～第8时段，置“1”：有效，置“0”：无效。
//时段功控定值方案号：数值范围：0～2依次表示第1～第3套方案，其他值无效。
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			if(i>LENper_8103_127)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
			MR(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8103_127,2);
			}
			p8+=2;
			for(Method=1;Method<=Array;Method++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8103_127;
			}
			if(Method>Array)
			{//没找到
				Array=Method;
				if(Method>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			Array--;
			MW((u32)p8rx,ADDR_128KDATABUFF+2+(Array*LENper_8103_127),LENper_8103_127);
			MW(ADDR_128KDATABUFF,ADDR_8103_127,LENmax_8103_127);
			return (LenRx<<16)+LenTx;

		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//厂休控
u32 ACTION_8104(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://添加控制单元
		case 5://更新控制单元
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_2;
			}
			if(i<Array)
			{//找到
				MW((u32)p8rx,(u32)p8,LENper_8104_2);
				MW(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			}
			else
			{//没找到
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8104_2*Array),LENper_8104_2);
					MW(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
				}
				else
				{
					UARTCtrl->DAR=8;//越界                 （8）
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://删除控单元
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8104_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_2;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8104_2,LENmax_8104_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8104_2,LENmax_8104_2);
			}
			else
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
			}
	//控制投入状态
			MR(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8104_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_3;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8104_3,LENmax_8104_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://控制投入
		case 7://控制解除
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8104_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8104_3;
			}
			if(i>Array)
			{//没找到
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8104_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{未投入（0），投入（1）}
			}
			else
			{
				p8[6]=0;//{未投入（0），投入（1）}
			}
			MW(ADDR_128KDATABUFF,ADDR_8104_3,LENmax_8104_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//营业报停控
u32 ACTION_8105(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://添加控制单元
		case 5://更新控制单元
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_2;
			}
			if(i<Array)
			{//找到
				MW((u32)p8rx,(u32)p8,LENper_8105_2);
				MW(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			}
			else
			{//没找到
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8105_2*Array),LENper_8105_2);
					MW(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
				}
				else
				{
					UARTCtrl->DAR=8;//越界                 （8）
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://删除控单元
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8105_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_2;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8105_2,LENmax_8105_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8105_2,LENmax_8105_2);
			}
			else
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
			}
	//控制投入状态
			MR(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8105_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_3;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8105_3,LENmax_8105_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://控制投入
		case 7://控制解除
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8105_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8105_3;
			}
			if(i>Array)
			{//没找到
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8105_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{未投入（0），投入（1）}
			}
			else
			{
				p8[6]=0;//{未投入（0），投入（1）}
			}
			MW(ADDR_128KDATABUFF,ADDR_8105_3,LENmax_8105_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//当前功率下浮控
u32 ACTION_8106(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Pn;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	i=p8rx[2];
	switch(i)
	{
		case 7://7	控制解除（总加组对象）总加组对象∷=OI
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];//总加组对象
			if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+0;
			}
	//控制投入状态
			MR(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8106_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8106_3;
			}
			if(i<Array)
			{//找到
				p8[6]=0;//未投入（0），投入（1）
				MW(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			}
			return (LenRx<<16)+LenTx;
			
		case 127://方法127 投入（总加组对象，控制方案）
//structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//当前功率下浮控定值滑差时间    unsigned（单位：分钟），
//当前功率下浮控定值浮动系数    integer（单位：%），
//控后总加有功功率冻结延时时间  unsigned（单位：分钟），
//当前功率下浮控的控制时间      unsigned（单位：0.5小时），
//当前功率下浮控第1轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第2轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第3轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第4轮告警时间  unsigned（单位：分钟）
//}
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			if(LenRx>LENper_8106_127)
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];//总加组对象
			if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
			{
				UARTCtrl->DAR=8;//越界                 （8）
				return (LenRx<<16)+0;
			}
			Pn=OI&0xff;
			Pn--;
			i=p8rx[14];//当前功率下浮控的控制时间0.5h
			i*=30;//当前功率下浮控的控制时间0.5h
			MWR(i,ADDR_TerminalFMTimer+6+(TFMSTIMER_NUM*2)+6+(Pn*2),2);
			
			MR(ADDR_128KDATABUFF,ADDR_8106_127,LENmax_8106_127);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8106_127,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8106_127;
			}
			if(i>=Array)
			{//没找到
				if(i<NUMmax_TAG)
				{
					i++;
				}
				else
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			Array=i;
			if(Array==0)
			{
				Array=1;
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			Array--;
			MW((u32)p8rx,ADDR_128KDATABUFF+2+(Array*LENper_8106_127),LENper_8106_127);
			MW(ADDR_128KDATABUFF,ADDR_8106_127,LENmax_8106_127);
	//控制投入状态
			MR(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8106_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8106_3;
			}
			if(i<Array)
			{//找到
				p8[6]=1;//未投入（0），投入（1）
			}
			else
			{//没找到
				if(i<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=i+1;
					p8+=2;
					p8+=LENper_8106_3*i;
					p8[0]=DataType_structure;
					p8[1]=2;
					p8[2]=DataType_OI;
					p8[3]=OI>>8;
					p8[4]=OI;
					p8[5]=DataType_enum;
					p8[6]=1;
				}
				else
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			MW(ADDR_128KDATABUFF,ADDR_8106_3,LENmax_8106_3);
			return (LenRx<<16)+LenTx;

		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//购电控
u32 ACTION_8107(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Pn;
	u32 Array;
	u8 *p8;
	u64 d64a;
	u64 d64b;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://添加控制单元
		case 5://更新控制单元
//购电控配置单元
//属性2∷=structure
//{
//	总加组对象      OI，
//购电单号        double-long-unsigned，
//追加/刷新标识   enum{追加（0），刷新（1）}，
//购电类型        enum{电量（0），电费（1）}，
//购电量（费）值  long64（单位：kWh/元， 换算：-4），
//	报警门限值      long64（单位：kWh/元，换算：-4），
//	跳闸门限值      long64（单位：kWh/元，换算：-4）
//	购电控模式      enum{本地模式（0），远程模式（1）}
//}
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用（9）
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
			if((OI<0x2301)||(OI>(0x2300+NUMmax_TAG)))
			{
				UARTCtrl->DAR=255;
				return (LenRx<<16)+LenTx;
			}
			Pn=OI&0xff;
			Pn--;
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_2;
			}
			if(i<Array)
			{//找到
			#if (USER/100)!=0//不是电科院测试
				//比较购电单号
				i=MRR((u32)p8rx+6,4);
				Method=MRR((u32)p8+6,4);
				if(i==Method)
				{//购电单号相同
					UARTCtrl->DAR=255;
					return (LenRx<<16)+LenTx;
				}
			#endif
				MW((u32)p8rx,(u32)p8,LENper_8107_2);
				MW(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			}
			else
			{//没找到
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8107_2*Array),LENper_8107_2);
					MW(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
				}
				else
				{
					UARTCtrl->DAR=8;//越界（8）
					return (LenRx<<16)+LenTx;
				}
			}
			if(p8rx[11])
			{//刷新标识enum{追加（0），刷新（1）}
				MW((u32)p8rx+15,ADDR_230x_11+(Pn*LENper_230x_11)+1,8);
			}
			else
			{//追加
				d64a=MRR((u32)p8rx+15,8);
				d64b=MRR(ADDR_230x_11+(Pn*LENper_230x_11)+1,8);
				d64a=R_Inverse(d64a,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
				d64b=R_Inverse(d64b,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
				d64a+=d64b;
				d64a=R_Inverse(d64a,8);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
				MWR(d64a,ADDR_230x_11+(Pn*LENper_230x_11)+1,8);
			}
			//购电事件
			p8=(u8*)ADDR_128KDATABUFF+(64*1024);//产生事件用临时数据
			p8[0]=DataType_OI;//产生源
			p8[1]=OI>>8;
			p8[2]=OI;
			Event_OAD(0x32020200,p8,2,2);//标准事件记录;入口:OAD,pOccurSour产生源=0表示空,occur=0恢复=1产生=2无,OccurkWh=0恢复=1产生=2无;返回:0=没记录,1=记录;使用ADDR_DATABUFF和ADDR_128KDATABUFF
			return (LenRx<<16)+LenTx;
		case 4://删除控单元
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8107_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_2;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8107_2,LENmax_8107_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8107_2,LENmax_8107_2);
			}
			else
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
			}
	//控制投入状态
			MR(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8107_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_3;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8107_3,LENmax_8107_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://控制投入
		case 7://控制解除
			LenRx=0;
			p8rx+=4;
			LEN_RxAPDU-=4;
			i=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			i>>=8;
			if(LEN_RxAPDU<i)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			LenRx+=i;
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8107_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8107_3;
			}
			if(i>Array)
			{//没找到
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8107_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{未投入（0），投入（1）}
			}
			else
			{
				p8[6]=0;//{未投入（0），投入（1）}
			}
			MW(ADDR_128KDATABUFF,ADDR_8107_3,LENmax_8107_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}

//月电控
u32 ACTION_8108(u32 PORTn,u8 *p8rx,u32 LEN_RxAPDU,u8 *p8tx,u32 LENmax_TxSPACE)//方法;返回:b31-b16接收区的数据长度(不包括4字节OMD),b15-b0填入发送区的需返回数据长度(不包括4字节OMD),数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Method;
	u32 LenRx;
	u32 LenTx;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 OI;
	u32 Array;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LenTx=0;
	Method=p8rx[2];
	switch(Method)
	{
		case 3://添加控制单元
		case 5://更新控制单元
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用（9）
				return 0;
			}
			OI=(p8rx[3]<<8)+p8rx[4];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				p8[0]=DataType_array;
				p8[1]=0;
				Array=0;
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_2;
			}
			if(i<Array)
			{//找到
				MW((u32)p8rx,(u32)p8,LENper_8108_2);
				MW(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			}
			else
			{//没找到
				if(Array<NUMmax_TAG)
				{
					p8=(u8*)ADDR_128KDATABUFF;
					p8[0]=DataType_array;
					p8[1]=Array+1;
					MW((u32)p8rx,ADDR_128KDATABUFF+2+(LENper_8108_2*Array),LENper_8108_2);
					MW(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
				}
				else
				{
					UARTCtrl->DAR=8;//越界（8）
				}
			}
			return (LenRx<<16)+LenTx;
		case 4://删除控单元
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用（9）
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];
	//控制方案集
			MR(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8108_2,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_2;
			}
			if((i<Array)&&(Array!=0))
			{//找到
				MR((u32)p8,(u32)p8+LENper_8108_2,LENmax_8108_2);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8108_2,LENmax_8108_2);
			}
			else
			{
				UARTCtrl->DAR=6;//对象不存在           （6）
			}
	//控制投入状态
			MR(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8108_3,2);
			}
			p8+=2;
			for(i=0;i<Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_3;
			}
			if(i<Array)
			{//找到
				MR((u32)p8,(u32)p8+LENper_8108_3,LENmax_8108_3);
				p8=(u8*)ADDR_128KDATABUFF;
				Array--;
				p8[1]=Array;
				MW(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			}
			return (LenRx<<16)+LenTx;
		case 6://控制投入
		case 7://控制解除
			p8rx+=4;
			LEN_RxAPDU-=4;
			LenRx=Get_DL698DataLen(p8rx,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LenRx>>=8;
			if(LEN_RxAPDU<LenRx)
			{
				UARTCtrl->DAR=9;//数据块不可用         （9）
				return 0;
			}
			OI=(p8rx[1]<<8)+p8rx[2];
			MR(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			//找相同的OI
			p8=(u8*)ADDR_128KDATABUFF;
			Array=p8[1];
			if(Array>NUMmax_TAG)
			{
				Array=0;
				MC(0,ADDR_8108_3,2);
			}
			p8+=2;
			for(i=1;i<=Array;i++)
			{
				if(OI==((p8[3]<<8)+p8[4]))
				{
					break;
				}
				p8+=LENper_8108_3;
			}
			if(i>Array)
			{//没找到
				Array=i;
				if(i>NUMmax_TAG)
				{
					UARTCtrl->DAR=8;//越界                 （8）
					return (LenRx<<16)+0;
				}
			}
			p8=(u8*)ADDR_128KDATABUFF;
			p8[0]=DataType_array;
			p8[1]=Array;
			i--;
			p8+=2+(i*LENper_8108_3);
			p8[0]=DataType_structure;
			p8[1]=2;
			p8[2]=DataType_OI;
			p8[3]=OI>>8;
			p8[4]=OI;
			p8[5]=DataType_enum;
			if(Method==6)
			{
				p8[6]=1;//{未投入（0），投入（1）}
			}
			else
			{
				p8[6]=0;//{未投入（0），投入（1）}
			}
			MW(ADDR_128KDATABUFF,ADDR_8108_3,LENmax_8108_3);
			return (LenRx<<16)+LenTx;
		default:
			UARTCtrl->DAR=6;//对象不存在           （6）
			return 0;
	}
}





