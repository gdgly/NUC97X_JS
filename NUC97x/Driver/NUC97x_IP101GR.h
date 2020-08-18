

#ifndef NUC97x_IP101GR_H
#define NUC97x_IP101GR_H


#ifndef PHYAddr_IP101
#define PHYAddr_IP101     1//eth0
#endif
#ifndef PHYAddr_LAN8720
#define PHYAddr_LAN8720     1//eth1
#endif


u32 Read_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg);//��ETH0_PHY�Ĵ���
void Write_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//дETH0_PHY�Ĵ���
u32 Read_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg);//��ETH1_PHY�Ĵ���
void Write_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//дETH1_PHY�Ĵ���
u32 Read_ETH_PHYReg(u32 PHYAddr,u32 PHYReg);//��ETH_PHY�Ĵ���
void Write_ETH_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//дETH_PHY�Ĵ���

void Init_IP101(void);//eth0
void Init_LAN8720(void);//eth1


#endif









