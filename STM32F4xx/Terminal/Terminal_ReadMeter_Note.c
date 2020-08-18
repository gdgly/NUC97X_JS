
#include "Project.h"
#include "Terminal_ReadMeter_Note.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "AutoSearchMeter.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"





void ReadMeterNote_OK(u32 PORTn,u32 Pn,u32 Info)//集中抄表电表抄读信息.成功
{
	u32 i;
	u8 *p8;

	if((Pn==0)||(Pn>AFN0CF170MaxPn))
	{
		return;
	}
	Pn--;
	p8=(u8 *)(ADDR_AFN0CF170+(18*Pn));
//所属终端通信端口号	BIN	1
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			p8[0]=2;
			break;
		case RS485_1PORT:
			p8[0]=2;
			break;
		case RS485_2PORT:
			p8[0]=3;
			break;
		case RS485_3PORT:
			p8[0]=4;
			break;
		case RS485_4PORT:
			p8[0]=31;
			break;
	}
	if(PORTn==RS485_4PORT)
	{
//中继路由级数	BIN	1
		i=Info;
		i&=0xff;
		p8[1]=i>>4;
//载波抄读通信相位	BS8	1
		i=Info>>16;
		i=i&0xf;
		if((i!=0)&&(i<=3))
		{
			i--;
			i=1<<i;
			p8[2]=i;
		}
		else
		{
			p8[2]=0;
		}
//载波信号品质	BS8	1
		p8[3]=Info>>24;
	}
	else
	{
//中继路由级数	BIN	1
		p8[1]=0;
//载波抄读通信相位	BS8	1
		p8[2]=0;
//载波信号品质	BS8	1
		p8[3]=0;
	}
//最近一次抄表成功/失败标志	BIN	1
	p8[4]=1;
//最近一次抄表成功时间	见附录A.1	6
	MW(ADDR_TYMDHMS,((u32)p8)+5,6);
//最近一次抄表失败时间	见附录A.1	6
//最近连续失败累计次数	BIN	1	
	p8[17]=0;
}

void ReadMeterNote_Fail(u32 PORTn,u32 Pn)//集中抄表电表抄读信息.失败
{
	u8 *p8;

	if((Pn==0)||(Pn>AFN0CF170MaxPn))
	{
		return;
	}
	Pn--;
	p8=(u8 *)(ADDR_AFN0CF170+(18*Pn));
//所属终端通信端口号	BIN	1
	switch(PORTn)
	{
		case ACSAMPLEPORT:
			p8[0]=2;
			break;
		case RS485_1PORT:
			p8[0]=2;
			break;
		case RS485_2PORT:
			p8[0]=3;
			break;
		case RS485_3PORT:
			p8[0]=4;
			break;
		case RS485_4PORT:
			p8[0]=31;
			break;
	}
	if(PORTn==RS485_4PORT)
	{
//中继路由级数	BIN	1
//载波抄读通信相位	BS8	1
//载波信号品质	BS8	1
	}
	else
	{
//中继路由级数	BIN	1
		p8[1]=0;
//载波抄读通信相位	BS8	1
		p8[2]=0;
//载波信号品质	BS8	1
		p8[3]=0;
	}
//最近一次抄表成功/失败标志	BIN	1
	p8[4]=0;
//最近一次抄表成功时间	见附录A.1	6
//最近一次抄表失败时间	见附录A.1	6
	MW(ADDR_TYMDHMS,((u32)p8)+11,6);
//最近连续失败累计次数	BIN	1	
	if(p8[17]!=0xff)
	{
		p8[17]++;
	}
}

void DateRMFlags(u32 Pn,u32 f)//日抄表标志;入口:f=0成功,=1失败
{
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
#if (USER/100)==7//用户标识:浙江版才允许自动删除
	u32 i;
	u32 x;
	u32 y;
	
	i=MRR(ADDR_AFN05F149+1,1);//自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
	if(i!=0x1)
	{	
		return;
	}
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	Pn--;
	//1-31日抄表成功失败标志,用于自动删除
	//i=MRR(ADDR_AFN05F149+1,1);//自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
	//if(i==0x1)
	//{	
		i=Comm_Ram->TYMDHMS[3];//秒,分,时,日,月,年
		i=bcd_hex(i);
		if((i!=0)&&(i<=31))
		{
			i=(1<<i);
			x=MRR(ADDR_RMDateFlags+(4*Pn),4);//测量点日抄表成功失败标志;B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
			if(f==0)
			{//成功
				i=(~i);
				x&=i;
				MWR(x,ADDR_RMDateFlags+(4*Pn),4);//测量点日抄表成功失败标志;B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
			}
			else
			{//失败
				x|=i;
				MWR(x,ADDR_RMDateFlags+(4*Pn),4);//测量点日抄表成功失败标志;B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
				//自动删除AFN04F10的电能表配置
				i=~x;
				i=BitNum(i);//计算置位为1的个数(0-32)
				if(i<=2)
				{//连续30天抄表失败
					i=Check_AFN04F10_Pn(Pn+1);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
					if(i!=0)
					{
						i--;
						MC(0,ADDR_AFN04F10+(LEN_AFN04F10_Pn*i)+2,2);//所属测量点号=0
						//测量点有效标志
					#if AFN04F150Dn==1//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
						Pn++;
						if(Pn>=2048)
						{
							return;
						}
					#endif
						x=Pn/8;
						i=Pn%8;
						y=MRR(ADDR_AFN04F150+x,1);
						y&=(~(1<<i));
						MWR(y,ADDR_AFN04F150+x,1);
						//测量点参数变更标志
						Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
						y=MRR(ADDR_AFN04F150+256+x,1);
						y|=(1<<i);
						MWR(y,ADDR_AFN04F150+256+x,1);
						//ERC3参数变更事件
						AutoUpdataAFN04F10ERC3();//自动AFN04F10的电能表配置的ERC3事件
						//
						PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
						AFN04F150_Computer();//测量点有效标志计算
					}
				}
			}
		}
	//}
#endif
#endif
}




