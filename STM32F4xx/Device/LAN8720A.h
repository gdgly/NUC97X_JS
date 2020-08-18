

#ifndef LAN8720A_H
#define LAN8720A_H

#define PHYAddr_LAN8720A_0     0

u32 Read_ETH_PHYReg(u32 PHYAddr,u32 PHYReg);//¶ÁETH_PHY¼Ä´æÆ÷
void Write_ETH_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//Ð´ETH_PHY¼Ä´æÆ÷

void Init_LAN8720A(void);

#endif


