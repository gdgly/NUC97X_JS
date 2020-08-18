
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


//u32 ETH_PHY_CR(void)//ѡ��ʱ��Ƶ��
//{
//	return 0;
//}

u32 Read_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg)//��ETH0_PHY�Ĵ���
{
	u32 i;
	
	while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC��æ
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
		SoftDelayUS(100);//���us��ʱ(TMR4)
	}
	return inpw(REG_EMAC0_MIID);
}
void Write_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data)//дETH0_PHY�Ĵ���
{
	u32 i;
	
	while (inpw(REG_EMAC0_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC��æ
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
		SoftDelayUS(100);//���us��ʱ(TMR4)
	}
}

u32 Read_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg)//��ETH1_PHY�Ĵ���
{
	u32 i;
	
	while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC��æ
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
		SoftDelayUS(100);//���us��ʱ(TMR4)
	}
	return inpw(REG_EMAC1_MIID);
}
void Write_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data)//дETH1_PHY�Ĵ���
{
	u32 i;
	
	while (inpw(REG_EMAC1_MIIDA) & 0x20000)// wait busy flag clear
	{//MDC��æ
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
		SoftDelayUS(100);//���us��ʱ(TMR4)
	}
}

u32 Read_ETH_PHYReg(u32 PHYAddr,u32 PHYReg)//��ETH_PHY�Ĵ���
{
#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
	return Read_ETH0_PHYReg(PHYAddr,PHYReg);//��ETH0_PHY�Ĵ���
#else
	return Read_ETH1_PHYReg(PHYAddr,PHYReg);//��ETH1_PHY�Ĵ���
#endif
}

void Write_ETH_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data)//дETH_PHY�Ĵ���
{
#if RJ45SELECT!=1//��̫��RJ45ͨ��ѡ��:0=eth0(IP101),1=eth1(LAN8720),2=eth0��eth1����������,eth0������̫��RJ45ͨ��
	Write_ETH0_PHYReg(PHYAddr,PHYReg,Data);//дETH1_PHY�Ĵ���
#else
	Write_ETH1_PHYReg(PHYAddr,PHYReg,Data);//дETH1_PHY�Ĵ���
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




