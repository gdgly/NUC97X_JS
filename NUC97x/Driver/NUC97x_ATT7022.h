
#ifndef NUC97x_ATT7022_H
#define NUC97x_ATT7022_H


#define G      1.163

//���㹫ʽ�� HFConst��INT[2.592*10^10*G*G*Vu*Vi/(EC*Un*Ib)]
#define HFConst     ((25920000000*G*G*ADC_Vu*ADC_Vi)/(MeterActiveConst*((MeterUBhex*MeterIBhex)/1000)))
	#if MeterClass==0x02//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
//���㹫ʽ��Istartup=INT[0.8*Io*2^13]
//����Io=Ib*N*�������õ�(�������Ӧȡ���ź�Ϊ25mV����N=30/Ib���������Ӧȡ���ź�Ϊ50mV����N=60/Ib��)
//���磬������������Ϊ0.4%��Ib=1.5Aȡ���ź�50mV����Io=1.5*40*0.4%��
//N�����������Чֵ���㹫ʽ�е�ϵ��N��ͬ
#define Istartup      (ADC_Vi*0.001*(1<<13))//�𶯵���
	#else
#define Istartup      (ADC_Vi*0.001*(1<<13))//�𶯵���
	#endif
	
	#if MeterClass==0x02//0x02=0.2s��,0x05=0.5s��,0x10=1.0��,0x20=2.0��
//���㹫ʽ�� Pstartup=INT[0.6*Ub*Ib*HFconst*EC* k��*2^23/(2.592*10^10)]
//Pstartup=INT[0.6*Ub*Ib**EC([2.592*10^10*G*G*Vu*Vi/(EC*Un*Ib))* k��*2^23/(2.592*10^10)]
//Pstartup=INT[0.6*G*G*Vu*Vi* k��*2^23]
#define Pstartup     (0.6*G*G*ADC_Vu*ADC_Vi*0.001*(1<<23))//�𶯹���
	#else
#define Pstartup     (0.6*G*G*ADC_Vu*ADC_Vi*0.001*(1<<23))//�𶯹���
	#endif


void Write_ATT7022(u32 Data);//дATT7022
u32 Read_ATT7022(u32 Addr);//��ATT7022
u32 Security_Read_ATT7022(u32 Addr);//��ȫ��ATT7022��������,�����ݿɿ���;����:����,=0xffffffff��ʾ������
u32 Get_MeterSpec(void);//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V


#endif












