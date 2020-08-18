
//ELCD
#ifndef LCDModule_H
#define LCDModule_H


void Init_LCDModule(void);//点阵LCD显示外部总线控制EMC初始化
void Startdisplay(void);//起动显示
void LCDDataOut(void);//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)



#endif
