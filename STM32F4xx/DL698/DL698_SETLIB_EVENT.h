

#ifndef DL698_EventLIB_H
#define DL698_EventLIB_H
#ifdef __cplusplus
 extern "C" {
#endif





DL698_EVENTLIB_TypeDef *Get_DL698_EVENT_pLib(u32 OAD);//�õ�EVENT_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
void DL698_SETINIT_EVENT(u32 Command,u8* pOAD);//���ò�����ʼ��:���Command:3=������ʼ��(���ָ�����������),4/5=����ϵͳ��վͨ����Ĳ�����ʼ��,,6=�����ݲ�����ʼ��



#ifdef __cplusplus
 }
#endif
#endif


