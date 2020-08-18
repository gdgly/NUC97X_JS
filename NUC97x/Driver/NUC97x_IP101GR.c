
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../NUC97x/Driver/NUC97x_IP101GR.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"


//u32 ETH_PHY_CR(void)//选择时钟频率
//{
//	return 0;
//}

u32 Read_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg)//读ETH0_PHY寄存器
{
	u32 i;
	
	while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC正忙
	}
	outpw(REG_EMAC0_MIIDA, (PHYAddr << 8) | PHYReg | 0xA0000);
	i=1000;
	while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
	{
		i--;
		if(i==0)
		{
			return 0;
		}
		SoftDelayUS(100);//软件us延时(TMR4)
	}
	return inpw(REG_EMAC0_MIID);
}
void Write_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data)//写ETH0_PHY寄存器
{
	u32 i;
	
	while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC正忙
	}
	outpw(REG_EMAC0_MIID, Data);
	outpw(REG_EMAC0_MIIDA, (PHYAddr << 8) | PHYReg | 0xB0000);
	i=1000;
	while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
	{
		i--;
		if(i==0)
		{
			return;
		}
		SoftDelayUS(100);//软件us延时(TMR4)
	}
}

u32 Read_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg)//读ETH1_PHY寄存器
{
	u32 i;
	
	while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC正忙
	}
	outpw(REG_EMAC1_MIIDA, (PHYAddr << 8) | PHYReg | 0xA0000);
	i=1000;
	while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
	{
		i--;
		if(i==0)
		{
			return 0;
		}
		SoftDelayUS(100);//软件us延时(TMR4)
	}
	return inpw(REG_EMAC1_MIID);
}
void Write_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data)//写ETH1_PHY寄存器
{
	u32 i;
	
	while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC正忙
	}
	outpw(REG_EMAC1_MIID, Data);
	outpw(REG_EMAC1_MIIDA, (PHYAddr << 8) | PHYReg | 0xB0000);
	i=1000;
	while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
	{
		i--;
		if(i==0)
		{
			return;
		}
		SoftDelayUS(100);//软件us延时(TMR4)
	}
}

u32 Read_ETH_PHYReg(u32 PHYAddr,u32 PHYReg)//读ETH_PHY寄存器
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	return Read_ETH0_PHYReg(PHYAddr,PHYReg);//读ETH0_PHY寄存器
#else
	return Read_ETH1_PHYReg(PHYAddr,PHYReg);//读ETH1_PHY寄存器
#endif
}

void Write_ETH_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data)//写ETH_PHY寄存器
{
#if RJ45SELECT!=1//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
	Write_ETH0_PHYReg(PHYAddr,PHYReg,Data);//写ETH1_PHY寄存器
#else
	Write_ETH1_PHYReg(PHYAddr,PHYReg,Data);//写ETH1_PHY寄存器
#endif
}



void Init_IP101(void)//eth0
{
	u32 i;
	
	Write_ETH0_PHYReg(PHYAddr_IP101,0x04,0x0001|0x0020|0x0040|0x0080|0x0100);//bit14 loopback mode
	i=Read_ETH0_PHYReg(PHYAddr_IP101,0);
	Write_ETH0_PHYReg(PHYAddr_IP101,0,i|0x0200);
}

void Init_LAN8720(void)//eth1
{
	u32 i;
	
	Write_ETH1_PHYReg(PHYAddr_LAN8720,0x04,0x0001|0x0020|0x0040|0x0080|0x0100);//bit14 loopback mode
	i=Read_ETH1_PHYReg(PHYAddr_LAN8720,0);
	Write_ETH1_PHYReg(PHYAddr_LAN8720,0,i|0x0200);
}




