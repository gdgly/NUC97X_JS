

#ifndef LAN8720A_H
#define LAN8720A_H

#define PHYAddr_LAN8720A_0     0

u32 Read_ETH_PHYReg(u32 PHYAddr,u32 PHYReg);//��ETH_PHY�Ĵ���
void Write_ETH_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//дETH_PHY�Ĵ���

void Init_LAN8720A(void);

#endif


