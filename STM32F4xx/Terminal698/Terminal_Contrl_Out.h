

#ifndef Terminal_Contrl_Out_H
#define Terminal_Contrl_Out_H


void Init_Contrl_Pin(void);//初始化终端输出控制和LED引脚
u32 Terminal_Contrl_Round(void);//终端当前设定的控制轮次(功控合电控);返回B0-B7位标志
void Terminal_Contrl_Out(void);//终端控制-输出.继电器和LED



#endif

