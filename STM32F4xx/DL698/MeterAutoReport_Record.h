
#ifndef MeterAutoReport_Record_H
#define MeterAutoReport_Record_H
#ifdef __cplusplus
 extern "C" {
#endif


void MeterAutoReport_Record(u32 PORTn,u8* pDATA,u32 FileID);//���ܱ������ϱ���¼�ļ�;���:pDATA=����ָ��(�ֱ�ΪRecordFileHead+ʱ��+֡�����ͳ���+����)
void MeterAutoReport_3011(u8* p8rx,u32 Occur);//�ز����ϱ��ӽڵ��¼�(���ܱ�ͣ���¼�);���:pAddr=��·��ģ����յ��Ľ��ջ����е�6�ֽڵ�ַ(����ɼ�����ַ)

	 

#ifdef __cplusplus
 }
#endif
#endif
