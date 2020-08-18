

#ifndef Terminal_Init_H
#ifdef __cplusplus
 extern "C" {
#endif


void Init_Terminal(void);//启动初始化
void Init_ALLDATA(void);//终端全部数据初始化(清0)

void Init_Terminal_SoftwareUpdate(void);//软件更新初始化

void Init_EnergyInc_Souser_Pn(u32 Pn);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
void Init_Energy_Pn(u32 Pn);//测量点日月电能量清为0(指定的Pn数据)
void Terminal_Init_Command(void);//终端初始化命令


#ifdef __cplusplus
 }
#endif
#endif
