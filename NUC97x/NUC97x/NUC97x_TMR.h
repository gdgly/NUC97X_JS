
#ifndef NUC97x_TMR_H
#define NUC97x_TMR_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
typedef struct
{
	u32 CSR;
	u32 CMPR;
	u32 DR;
}TMRx_TypeDef;

void Init_NUC97x_TMR2(void);//初始化基本定时器TMR2为从0开始的1MHz加计数(用于测量)
u32 Get_NUC97x_TMR2_Count(void);//取TMR2为从0开始的24位1MHz加计数(用于测量)的计数值,每计数值表示1us
void Init_NUC97x_TMR3(void);//初始化基本定时器TMR3为1ms重复中断
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
