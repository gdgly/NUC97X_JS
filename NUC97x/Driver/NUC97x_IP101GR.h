

#ifndef NUC97x_IP101GR_H
#define NUC97x_IP101GR_H


#ifndef PHYAddr_IP101
#define PHYAddr_IP101     1//eth0
#endif
#ifndef PHYAddr_LAN8720
#define PHYAddr_LAN8720     1//eth1
#endif


u32 Read_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg);//¶ÁETH0_PHY¼Ä´æÆ÷
void Write_ETH0_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//Ð´ETH0_PHY¼Ä´æÆ÷
u32 Read_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg);//¶ÁETH1_PHY¼Ä´æÆ÷
void Write_ETH1_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//Ð´ETH1_PHY¼Ä´æÆ÷
u32 Read_ETH_PHYReg(u32 PHYAddr,u32 PHYReg);//¶ÁETH_PHY¼Ä´æÆ÷
void Write_ETH_PHYReg(u32 PHYAddr,u32 PHYReg,u32 Data);//Ð´ETH_PHY¼Ä´æÆ÷

void Init_IP101(void);//eth0
void Init_LAN8720(void);//eth1


#endif









