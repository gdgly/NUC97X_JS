
#ifndef RM_3762_RouterActive_H
#define RM_3762_RouterActive_H
#ifdef __cplusplus
 extern "C" {
#endif

void Clr_RMListRMCount(u32 ListNo);//�峭������������;���:ListNo=�б�Ŵ�0��ʼ 
void Load_RMCount_NoCompAddr(u32 PORTn,u32 ListNo);//�õ���������������(���Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
void Load_RMCount_CompAddr(u32 PORTn,u32 ListNo);//�õ���������������(�Ƚϵ�ַ);���:ListNo=�б�Ŵ�0��ʼ
void Save_RMCount(u32 PORTn,u32 ListNo);//���泭������������;���:ListNo=�б�Ŵ�0��ʼ
	 
void ClrALL_RMListRMCount(void);//��ȫ����������������(�ز���);���:SetNo=�����Ŵ�0��ʼ
void RM_3762_RouterActive(u32 PORTn);//�ز�����(·��ģ�����������ڳ���ģʽ)


#ifdef __cplusplus
}
#endif
#endif

