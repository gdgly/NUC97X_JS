
//内部交采1分钟平均电压计算
#include "Project.h"
#include "Average_Voltage.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "../Display/DisplayLoop_ACSample.h"



void Average_Voltage(void)//内部交采1分钟平均电压计算和曲线冻结
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	
	if(Terminal_Ram->RMDelay_S_Timer!=0)//上电后延时抄表秒定时器(同时用于上电事件可记录延时)
	{
		return;
	}
	p8=(u8*)(ADDR_Average_Count);//内部交采1分钟平均电压值计算计数
	if(p8[0]!=Comm_Ram->TYMDHMS[0])
	{//秒更新
		p8[0]=Comm_Ram->TYMDHMS[0];
		GetACSample(0x0201ff00);//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
		for(i=0;i<3;i++)
		{
			x=MRR(Get_ADDR_UARTnTx(ACSAMPLEPORT)+14+(2*i),2);
			x=bcd_hex(x);
			y=MRR(ADDR_Average_Count+3+(3*i),3);
			x+=y;
			MWR(x,ADDR_Average_Count+3+(3*i),3);
		}
		p8[2]++;//平均值计算加次数计数+1
		if(p8[1]!=Comm_Ram->TYMDHMS[1])
		{//分更新
			p8[1]=Comm_Ram->TYMDHMS[1];
			i=p8[2];//平均值计算加次数计数
			if(i!=0)
			{//平均加次数不为0
				for(i=0;i<3;i++)
				{
					x=MRR(ADDR_Average_Count+3+(3*i),3);
					x/=p8[2];
					x=hex_bcd(x);
					MWR(x,ADDR_Average_Voltage+(2*i),2);
				}
			#if MeterPW==1//初始接线方式;0=三相四线,1=三相三线
				MC(0xee,ADDR_Average_Voltage+2,2);//三相三线B相为0xee值
			#endif
			}
			MC(0,ADDR_Average_Count+2,10);//清加计数
	#if MaxACSampleCurveCongeal!=0//最大交采1分钟冻结曲线点数(可设为0表示不用)
			//1分钟曲线冻结
			MC(0,ADDR_DATABUFF,7);
			p8=(u8*)(ADDR_DATABUFF);
			p8[3]=1;//00年01月01日00时00分00秒
			p8[4]=1;
			MR(ADDR_DATABUFF+7,ADDR_TYMDHMS+1,5);//年月日时分0秒
			i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
			i/=60;
			i%=MaxACSampleCurveCongeal;//偏移点数
			i*=LEN_ACSampleCurve;//偏移地址
			i+=ADDR_ACSampleCurve;//地址
			x=(i+11)&0xfff;
			if((x!=0)&&(x<=11))
			{//本扇区刚开始写入
				//EFLASH_Sector_Erase(i+11);//扇区擦除,入口R0=要擦除的扇区在本扇区中的任意地址(每扇区2KWORD)
			}
			MR(ADDR_DATABUFF,i,11);
			for(x=0;x<11;x++)
			{
				if(p8[x]!=0xff)
				{
					break;
				}
			}
			if(x!=11)
			{//没擦除
				x=DataComp(ADDR_DATABUFF,ADDR_TYMDHMS+1,5);//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
				if(x==0)
				{//相同时标重复写
					MR(ADDR_DATABUFF+5,ADDR_Average_Voltage,6);
					MW(ADDR_DATABUFF,i,11);
					return;
				}
				//EFLASH_Sector_Erase(i);//扇区擦除,入口R0=要擦除的扇区在本扇区中的任意地址(每扇区2KWORD)
			}
			MR(ADDR_DATABUFF,ADDR_TYMDHMS+1,5);
			MR(ADDR_DATABUFF+5,ADDR_Average_Voltage,6);
			DMW(ADDR_DATABUFF,i,11);
	#endif
		}
	}
}








