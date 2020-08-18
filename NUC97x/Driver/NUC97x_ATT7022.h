
#ifndef NUC97x_ATT7022_H
#define NUC97x_ATT7022_H


#define G      1.163

//计算公式： HFConst＝INT[2.592*10^10*G*G*Vu*Vi/(EC*Un*Ib)]
#define HFConst     ((25920000000*G*G*ADC_Vu*ADC_Vi)/(MeterActiveConst*((MeterUBhex*MeterIBhex)/1000)))
	#if MeterClass==0x02//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
//计算公式：Istartup=INT[0.8*Io*2^13]
//其中Io=Ib*N*比例设置点(额定电流对应取样信号为25mV，则N=30/Ib；额定电流对应取样信号为50mV，则N=60/Ib；)
//例如，启动电流设置为0.4%，Ib=1.5A取样信号50mV，则Io=1.5*40*0.4%。
//N——与电流有效值计算公式中的系数N相同
#define Istartup      (ADC_Vi*0.001*(1<<13))//起动电流
	#else
#define Istartup      (ADC_Vi*0.001*(1<<13))//起动电流
	#endif
	
	#if MeterClass==0x02//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
//计算公式： Pstartup=INT[0.6*Ub*Ib*HFconst*EC* k‰*2^23/(2.592*10^10)]
//Pstartup=INT[0.6*Ub*Ib**EC([2.592*10^10*G*G*Vu*Vi/(EC*Un*Ib))* k‰*2^23/(2.592*10^10)]
//Pstartup=INT[0.6*G*G*Vu*Vi* k‰*2^23]
#define Pstartup     (0.6*G*G*ADC_Vu*ADC_Vi*0.001*(1<<23))//起动功率
	#else
#define Pstartup     (0.6*G*G*ADC_Vu*ADC_Vi*0.001*(1<<23))//起动功率
	#endif


void Write_ATT7022(u32 Data);//写ATT7022
u32 Read_ATT7022(u32 Addr);//读ATT7022
u32 Security_Read_ATT7022(u32 Addr);//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
u32 Get_MeterSpec(void);//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V


#endif












