
//重点户
#include "Project.h"
#include "KeyUser.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_CongealData_Fn.h"




u32 KeyUserPnSet(u32 Pn)//重点户有效测量点号检查;返回0=无效,1=有效;使用重点户曲线冻结Pn列表ADDR_KeyUserPnList
{
#if (MaxKeyUser!=MaxRS485AddCarrierPn)
	u32 i;
	u16 *p16;
	
	p16=(u16 *)(ADDR_KeyUserPnList);
	for(i=0;i<MaxKeyUser;i++)
	{
		if(p16[i]==Pn)
		{
			return 1;
		}
	}
	return 0;
#else
	if((Pn==0)||(Pn>MaxKeyUser))
	{
		return 0;
	}
	else
	{
		return 1;
	}
#endif
}


const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[]=
{
/*
//#define  ADDR_AFN0DF73     5//ADDR_CURVECONGEAL_START+5//总加组有功功率曲线
	{
  	73,//重点户2类数据曲线Fn
  	0,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF74      ADDR_AFN0DF73+(2*AFN0DF73MaxPn)//总加组无功功率曲线
	{
  	74,//重点户2类数据曲线Fn
  	0,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF75      ADDR_AFN0DF74+(2*AFN0DF74MaxPn)//总加组有功电能量曲线
	{
  	75,//重点户2类数据曲线Fn
  	0,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF76      ADDR_AFN0DF75+(4*AFN0DF75MaxPn)//总加组无功电能量曲线
	{
  	76,//重点户2类数据曲线Fn
  	0,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF81      ADDR_AFN0DF76+(4*AFN0DF76MaxPn)//测量点有功功率曲线
	{
  	81,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF82      ADDR_AFN0DF81+(3*AFN0DF81MaxPn)//测量点A相有功功率曲线
	{
  	82,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF83      ADDR_AFN0DF82+(3*AFN0DF82MaxPn)//测量点B相有功功率曲线
	{
  	83,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF84      ADDR_AFN0DF83+(3*AFN0DF83MaxPn)//测量点C相有功功率曲线
	{
  	84,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF85      ADDR_AFN0DF84+(3*AFN0DF84MaxPn)//测量点无功功率曲线
	{
  	85,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF86      ADDR_AFN0DF85+(3*AFN0DF85MaxPn)//测量点A无功功率曲线
	{
  	86,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF87      ADDR_AFN0DF86+(3*AFN0DF86MaxPn)//测量点B无功功率曲线
	{
  	87,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF88      ADDR_AFN0DF87+(3*AFN0DF87MaxPn)//测量点C无功功率曲线
	{
  	88,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF89      ADDR_AFN0DF88+(3*AFN0DF88MaxPn)//测量点A相电压曲线
	{
  	89,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF90      ADDR_AFN0DF89+(2*AFN0DF89MaxPn)//测量点B相电压曲线
	{
  	90,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF91      ADDR_AFN0DF90+(2*AFN0DF90MaxPn)//测量点C相电压曲线
	{
  	91,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF92      ADDR_AFN0DF91+(2*AFN0DF91MaxPn)//测量点A相电流曲线
	{
  	92,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF93      ADDR_AFN0DF92+(3*AFN0DF92MaxPn)//测量点B相电流曲线
	{
  	93,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF94      ADDR_AFN0DF93+(3*AFN0DF93MaxPn)//测量点C相电流曲线
	{
  	94,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF95      ADDR_AFN0DF94+(3*AFN0DF94MaxPn)//测量点零序电流曲线
	{
  	95,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF97      ADDR_AFN0DF95+(3*AFN0DF95MaxPn)//测量点正向有功总电能量
	{
  	97,//重点户2类数据曲线Fn
  	129,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF98      ADDR_AFN0DF97+(4*AFN0DF97MaxPn)//测量点正向无功总电能量
	{
  	98,//重点户2类数据曲线Fn
  	130,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF99      ADDR_AFN0DF98+(4*AFN0DF98MaxPn)//测量点反向有功总电能量
	{
  	99,//重点户2类数据曲线Fn
  	131,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF100      ADDR_AFN0DF99+(4*AFN0DF99MaxPn)//测量点反向无功总电能量
	{
  	100,//重点户2类数据曲线Fn
  	132,//要抄读1类实时数据Fn,为0表示无效不抄
	},
*/
//#define  ADDR_AFN0DF101      ADDR_AFN0DF100+(4*AFN0DF100MaxPn)//测量点正向有功总电能示值
	{
  	101,//重点户2类数据曲线Fn
  	129,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF102      ADDR_AFN0DF101+(4*AFN0DF101MaxPn)//测量点正向无功总电能示值
	{
  	102,//重点户2类数据曲线Fn
  	130,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF103      ADDR_AFN0DF102+(4*AFN0DF102MaxPn)//测量点反向有功总电能示值
	{
  	103,//重点户2类数据曲线Fn
  	131,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF104      ADDR_AFN0DF103+(4*AFN0DF103MaxPn)//测量点反向无功总电能示值
	{
  	104,//重点户2类数据曲线Fn
  	132,//要抄读1类实时数据Fn,为0表示无效不抄
	},
/*
//#define  ADDR_AFN0DF105      ADDR_AFN0DF104+(4*AFN0DF104MaxPn)//测量点总功率因数
	{
  	105,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF106      ADDR_AFN0DF105+(2*AFN0DF105MaxPn)//测量点A相功率因数
	{
  	106,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF107      ADDR_AFN0DF106+(2*AFN0DF106MaxPn)//测量点B相功率因数
	{
  	107,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF108      ADDR_AFN0DF107+(2*AFN0DF107MaxPn)//测量点C相功率因数
	{
  	108,//重点户2类数据曲线Fn
  	25,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF109      ADDR_AFN0DF108+(2*AFN0DF108MaxPn)//测量点电压相位角曲线
	{
  	109,//重点户2类数据曲线Fn
  	0,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF110      ADDR_AFN0DF109+(6*AFN0DF109MaxPn)//测量点电流相位角曲线
	{
  	110,//重点户2类数据曲线Fn
  	0,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF138      ADDR_AFN0DF110+(6*AFN0DF110MaxPn)//直流模拟量数据曲线
	{
  	138,//重点户2类数据曲线Fn
  	0,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF145      ADDR_AFN0DF138+(2*AFN0DF138MaxPn)//测量点1象限无功总电能示值曲线
	{
  	145,//重点户2类数据曲线Fn
  	133,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF146      ADDR_AFN0DF145+(4*AFN0DF145MaxPn)//测量点4象限无功总电能示值曲线
	{
  	146,//重点户2类数据曲线Fn
  	136,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF147      ADDR_AFN0DF146+(4*AFN0DF146MaxPn)//测量点2象限无功总电能示值曲线
	{
  	147,//重点户2类数据曲线Fn
  	134,//要抄读1类实时数据Fn,为0表示无效不抄
	},
//#define  ADDR_AFN0DF148      ADDR_AFN0DF147+(4*AFN0DF147MaxPn)//测量点3象限无功总电能示值曲线
	{
  	148,//重点户2类数据曲线Fn
  	135,//要抄读1类实时数据Fn,为0表示无效不抄
	},
*/
//#define  ADDR_AFN0DF217      ADDR_AFN0DF148+(4*AFN0DF148MaxPn)//台区集中抄表载波主节点白噪声曲线

//#define  ADDR_AFN0DF218      ADDR_AFN0DF217+1//台区集中抄表载波主节点色噪声曲线

};

u32 Get_KeyUserReadMeterLib_MaxFn(void)
{
  return(sizeof(KeyUserReadMeterLib)/sizeof(KeyUserReadMeterLib_TypeDef));
}



void KeyUserPnList(void)//重点户曲线冻结Pn列表
{
#if (MaxKeyUser!=MaxRS485AddCarrierPn)
//集中器
	u32 i;
	u32 SourPn;
	u32 Pn;
	u32 MeterNo;
	u32 KeyUser;
	u32 x;
	u16 *p16;
	#if IC_SST39VF6401B==1//0=没,1=有
	u8 *p8;
	#endif
#if (USER/100)==11//吉林系
	u32 y;
#endif	
	if(Terminal_Ram->AFN04F10F35Update!=0)//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
	{
		return;
	}
	Terminal_Ram->AFN04F10F35Update=0x55;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
	
	p16=(u16*)(ADDR_KeyUserPnList);
	KeyUser=MRR(ADDR_AFN04F35,1);
	if(KeyUser>MaxKeyUser)
	{
		KeyUser=MaxKeyUser;
	}
	for(i=0;i<KeyUser;i++)
	{
		SourPn=p16[i];//原Pn
		Pn=0xffff;
		MeterNo=MRR(ADDR_AFN04F35+1+(2*i),2);//重点户的电能表/交流采样装置序号
		if((MeterNo!=0)&&(MeterNo<=AFN04F10MaxPn))
		{
			MeterNo--;
			Pn=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*MeterNo)+2,2);//所属测量点号
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				Pn=0xffff;
			}
		}
		p16[i]=Pn;
		if(SourPn!=Pn)
		{//Pn映射表变化
			//清曲线冻结区间电能量起始值
			MC(0xee,ADDR_AFN0DF97_SOUR+(5*i),5);//正向有功总电量原始,初始化值0xee
			MC(0xee,ADDR_AFN0DF98_SOUR+(4*i),4);//正向无功总电量原始,初始化值0xee
			MC(0xee,ADDR_AFN0DF99_SOUR+(5*i),5);//反向有功总电量原始,初始化值0xee
			MC(0xee,ADDR_AFN0DF100_SOUR+(4*i),4);//反向无功总电量原始,初始化值0xee
			//
		}
	}
	//将非重点户的内部交采,RS485表按AFN04F10中配置顺序作为重点户进行曲线冻结,直到等效重点户数达到MaxKeyUser为止
	if(i<MaxKeyUser)
	{
		#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
			#error
		#endif
		MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
		for(MeterNo=0;MeterNo<AFN04F10MaxPn;MeterNo++)
		{
			Pn=MRR(ADDR_128KDATABUFF+(LEN_AFN04F10_Pn*MeterNo)+2,2);//测量点号
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				SourPn=MRR(ADDR_128KDATABUFF+(LEN_AFN04F10_Pn*MeterNo)+4,2);
				x=SourPn>>8;//通信协议类型://1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议
				SourPn&=0x1f;//端口号
		#if (USER/100)==11//吉林系
				y=MRR(ADDR_128KDATABUFF+(LEN_AFN04F10_Pn*MeterNo)+26,1);;//大小类号
				if(((SourPn!=0)&&(SourPn<=3))||(x==2)||((SourPn==31)&&(x==30)&&(y==0x43)))
				{//端口号1-3,是内部交采或RS485表,是载波07表大小类号为(4,3)
		#else
				if(((SourPn!=0)&&(SourPn<=3))||(x==2))
				{//端口号1-3,是内部交采或RS485表
		#endif
					for(x=0;x<i;x++)
					{
						SourPn=p16[x];
						if(SourPn==Pn)
						{//相同测量点号已是重点户
							break;
						}
					}
					if(x>=i)
					{//原重点户没本测量点号
						SourPn=p16[i];
						p16[i]=Pn;
						if(SourPn!=Pn)
						{//Pn映射表变化
							//清曲线冻结区间电能量起始值
							MC(0xee,ADDR_AFN0DF97_SOUR+(5*i),5);//正向有功总电量原始,初始化值0xee
							MC(0xee,ADDR_AFN0DF98_SOUR+(4*i),4);//正向无功总电量原始,初始化值0xee
							MC(0xee,ADDR_AFN0DF99_SOUR+(5*i),5);//反向有功总电量原始,初始化值0xee
							MC(0xee,ADDR_AFN0DF100_SOUR+(4*i),4);//反向无功总电量原始,初始化值0xee
							//
						}
						i++;
						if(i>=MaxKeyUser)
						{
							break;
						}
					}
				}
			}
		}
	}
	//脉冲口也作为重点户(集中器无脉冲口)
	
	
	//将无用测量点号清为空0xff
	for(;i<MaxKeyUser;i++)
	{
		SourPn=p16[i];//原Pn
		p16[i]=0xffff;
		if(SourPn!=0xffff)
		{//Pn映射表变化
			//清曲线冻结区间电能量起始值
			MC(0xee,ADDR_AFN0DF97_SOUR+(5*i),5);//正向有功总电量原始,初始化值0xee
			MC(0xee,ADDR_AFN0DF98_SOUR+(4*i),4);//正向无功总电量原始,初始化值0xee
			MC(0xee,ADDR_AFN0DF99_SOUR+(5*i),5);//反向有功总电量原始,初始化值0xee
			MC(0xee,ADDR_AFN0DF100_SOUR+(4*i),4);//反向无功总电量原始,初始化值0xee
			//
		}
	}
#else
//专变，公变
	u32 i;
	u16 *p16;
	p16=(u16 *)(ADDR_KeyUserPnList);
	for(i=0;i<MaxKeyUser;i++)
	{
		p16[i]=i+1;
	}
#endif
}

u32 KeyUserMapPn(u32 Pn)//集中抄表重点户的映射Pn(1-MaxKeyUser);入口:Pn=1-MaxRS485AddCarrierPn
{
#if (MaxKeyUser!=MaxRS485AddCarrierPn)
	u32 i;
	u16 *p16;

	p16=(u16 *)(ADDR_KeyUserPnList);
	for(i=0;i<MaxKeyUser;i++)
	{
		if(p16[i]==Pn)
		{
			return (i+1);
		}
	}
	return 0;
#else
	return Pn;
#endif
}













