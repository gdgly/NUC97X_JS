

#ifndef Terminal_ReadMeter_DL645_2007_FnData_H
#define Terminal_ReadMeter_DL645_2007_FnData_H


void Terminal_ReadMeter_DL645_2007_TxData(u32 PORTn,u8 *p8RMLib);//�ն�DL645-2007����������
void PORTn_ReadMeter_DL645_2007_TxCurveData(u32 PORTn,u32 DI,u64 YMDHM);//DL645-2007��������������֡
void ReadMeter_DL645_2007_FnData(u32 PORTn,u8 *p8RMLib);//DL645_2007���������������Fn���ݻ���
void ReadMeter_DL645_2007_MulFnData(u32 PORTn,u32 Addr_FnDataBuff,u8 *p8RMLib);//DL645_2007���������������Fn���ݻ���


#endif
