

#ifndef DL698_SETLIB_H
#define DL698_SETLIB_H
#ifdef __cplusplus
 extern "C" {
#endif

	


DL698_SETLIB_TypeDef *Get_DL698_SET_pLib(u32 OAD);//�õ�SET_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
void DL698_SETINIT(u32 Command,u8* pOAD);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��
void Init_6000_2_1_Addr(void);//�������1ͨ�ŵ�ַΪADDR_4001��ַ(�̶���Ϊ����ʱ��)






#ifdef __cplusplus
 }
#endif
#endif
