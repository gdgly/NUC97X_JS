

#ifndef Terminal_Contrl_Manage_H
#define Terminal_Contrl_Manage_H
#ifdef __cplusplus
 extern "C" {
#endif



void Terminal_Contrl_Manage(void);//终端控制管理(分任务运行)
void Terminal_Contrl_Protect(void);//终端控制-保电
void Terminal_Remote_Control(void);//终端控制-遥控跳闸
void Terminal_Contrl1(void);//终端控制-功控.当前功率下浮控
void Terminal_Contrl2(void);//终端控制-功控.营业报停功控
void Terminal_Contrl3(void);//终端控制-功控.厂休功控
void Terminal_Contrl4(void);//终端控制-功控.时段功控
void Terminal_Contrl_MonthEnergy(void);//终端控制-月电控
void Terminal_Contrl_BuyEnergy(void);//终端控制-购电控
void Terminal_Contrl_Out(void);//终端控制-输出.继电器和LED
void Contrl_State(void);//总加组当前控制状态
void Contrl_SetState(u32 n);//总加组控制设置状态;入口:总加组号0-7
	 
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif


