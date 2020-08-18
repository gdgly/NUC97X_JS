

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_MAC.h"
#include "../Device/LAN8720A.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Device/24AA02E48.h"
#include "../OS/OS.h"



u32 Test_ETH(u32 ADDR_BASE)//ETH测试
{
	u32 i;
	u8 *p8;
	u32 *p32;
	u32 ReNUM;
	
	WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
	DisplayStringAutoUp(0,9,(u8*)"\x0""Ethernet");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol

	ReNUM=1;
	while(ReNUM--)
	{
		Init_STM32F4xx_MAC();
		Init_LAN8720A();
	//
		SoftDelayMS(3000);//经实测需延时1500ms以上后才能正常收发,否则失败
		ETH_MAC_Config();//MAC配置,在LINK后配置,读取PYH状态后按相应值配置
		ETH_DMA_Config(0x2001c000,0x2001c000+32);//DMA配置;入口:DMATDLA=发送描述符列表地址,DMARDLA=接收描述符列表地址
		ETH_DMARxDescList(0x2001c000+32,0x2001c000+64,0,2048,1,1);//ETH接收描述符表并启动接收
		//清接收
		p8=(u8*)0x2001c000;
		for(i=0;i<2048;i++)
		{
			p8[i]=0;
		}
		//写发送
	//IEEE 802.3-2002 标准规定，MAC 块使用 MAC 子层和可选 MAC 控制子层 (10/100 Mbit/s)
	//为使用 CSMA/CD MAC 的数据通信系统指定了两个帧格式:基本 MAC 帧格式,标记 MAC 帧格式（扩展了基本 MAC 帧格式）

	//7BYTE 报头0x55 0x55 0x55 0x55 0x55 0x55 0x55(硬件自动生成加入)
	//1BYTE SDF 0xD5(硬件自动生成加入)
	//6BYTE 目标地址(需相同设置的MAC地址)
		Read_24AA02E48(0x2001c000+4096,0xFA,6);//24AAxxx读
	//6BYTE 源地址(用于测试时可任意地址)
		MC(0xff,0x2001c000+4096+6,6);
		//Read_24AA02E48(0x2001c000+4096+6,0xFA,6);//24AAxxx读
	//4BYTE QTag前辍(带标记的MAC帧)
		//2BYTE 长度/类型=802.1QTagType
		//2BYTE 标记控制信息
	//2BYTE MAC客户端长度/类型:<=1500表示 802.3 帧的后续数据字段中所包含的 MAC 客户端数据字节的数量（长度解析）;>0x6000则该字段表示与以太网帧相关的 MAC 客户端协议的种类（类型解析）,=0x8100 802.1Q变量协议类型
		p8=(u8*)(0x2001c000+4096+12);
		p8[0]=1024>>8;//高字节
		p8[1]=1024&0xff;//低字节
		
	//46-1500BYTE 数据和 PAD 字段
		p8=(u8*)0x2001c000+4096+14;
		for(i=0;i<1024;i++)
		{
			p8[i]=i;
		}
	//4BYTE CRC (CRC 计算基于下列字段：源地址、目标地址、QTag 前缀、长度/类型、LLC 数据和 PAD（即，除报头、SFD 字段以外的所有字段）
		
		ETH_DMATxDescList(0x2001c000,0x2001c000+4096,0x2001c000+4096,1024+14,1,1);//ETH发送描述符表并启动发送
		__enable_irq();//允许中断
		Comm_Ram->GPMSTimer=1000/10;
		while(1)
		{
			p32=(u32*)(0x2001c000+32);
			if((p32[0]&0x80000000)==0)
			{
				i=p32[0]>>16;//位 29:16 FL：帧长度 (Frame length)
				i&=0x3fff;
				if(i==(1024+14))
				{
					p8=(u8*)0x2001c000+64+14;
					for(i=0;i<1024;i++)
					{
						if(p8[i]!=(i&0xff))
						{
							break;
						}
					}
					if(i>=1024)
					{
						MWR(0,ADDR_BASE+(OFFSET_UART_Characteristics+3),1);
						DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
						return 0;
					}
				}
			}
			if(Comm_Ram->GPMSTimer==0)
			{
				break;
			}
		}
	}
	MWR(1,ADDR_BASE+(OFFSET_UART_Characteristics+3),1);
	DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
	return 1;
}










