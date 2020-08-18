

#ifndef Terminal_Paramter_Check_H
#define Terminal_Paramter_Check_H

void PnVSAFN04F10SetNoList(void);//测量点号Pn对应AFN04F10配置序号列表
u32 Check_AFN04F10_Addr(u32 port,u64 CommAddr);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0相同地址的配置序号(1开始)
u32 Check_AFN04F10_Pn(u32 Pn);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
u32 Check_AFN04F11_Pn(u32 Pn);//检查终端脉冲配置参数指定的测量点号;返回0无,1有
u32 Check_AFN04F13_Pn(u32 Pn);//检查终端电压/电流模拟量配置参数指定的测量点号;返回0无,1有
void AFN04F150_Computer(void);//测量点有效标志计算
u32 PnSet(u32 Pn);//有效测量点号检查;返回0=无效,1=有效
u32 Check_ACSamplePn(u32 Pn);//有效交采测量点号检查;返回0=无效,1=有效
u32 ACSampleVoltageCurvePn(u32 Fn,u32 Pn);//有效交采电压曲线测量点号检查;返回0=无效,1=有效
u32 GroupTotalSet(u32 Pn);//有效总加组号检查;返回0=无效,1=有效
void Terminal_AFN0CF3_Computer(u32 Fn);//终端类1数据F3,终端参数状态

void PnVSProtocolList(u32 ADDR_BUFF);//测量点号Pn对应AFN04F10配置通信协议列表,无配置且非脉冲口默认通信协议为30(DL645-2007)
void PnVSPORTProtocolList(u32 ADDR_BUFF);//测量点号Pn对应AFN04F10配置端口号和通信协议列表,无配置且非脉冲口默认端口号0通信协议为30(DL645-2007)
void PnVSSetnoList(u32 ADDR_BUFF);//测量点号Pn对应AFN04F10配置序号列表
u32 Get_SetTariff(u32 Pn);//取AFN04F10中配置的费率数

u32 F246MapPn(u32 Pn);//F246掉电记录数据映射Pn;返回0表示无效
void F246PnList(void);//要抄读的F246掉电记录数据Pn列表


u32 Get_CurveSetFnListNo(u32 Fn);//得到曲线冻结密度参数Fn列表号;返回:列表号0-n,0xffffffff表示无

#endif
