
#ifndef NUC97x_AIC_H
#define NUC97x_AIC_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
unsigned long off_irq(void);//关IRQ中断,返回:关闭前允许状态
void destore_irq(unsigned long IRQ);//恢复IRQ中断,入口:关闭前允许状态
	 
	 
	 
#ifdef __cplusplus
 }
#endif 
#endif
