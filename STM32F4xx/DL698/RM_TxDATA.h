

#ifndef RM_TxDATA_H
#define RM_TxDATA_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 RM_TxDATA_DL1997(u32 PORTn,u8 *p8CSD,u8 *p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
u32 RM_TxDATA_DL2007_DI(u32 PORTn,u32 DI);//������֡DI����n�θ���
u32 RM_TxDATA_DL2007(u32 PORTn,u8 *p8CSD,u8 *p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
u32 RM_SECURITY(u32 OI);//����ȫģʽ��,���:OI,����:(ͬ��ȫ����)b7=0�ް�ȫ=1��ȫ����,b6=0����=1����,b5=,b4=,b3-b0=0������֤��[0]SID_MAC =1�����[1]RN =2�����+����MAC[2]RN_MAC =3��ȫ��ʶ[3]SID
u32 RM_TxDATA_DL698(u32 PORTn,u8 *p8CSD,u8 *p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
u32 RM_TxDATA_ABB(u32 PORTn,u8 *p8CSD,u8 *p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
u32 RM_TxDATA_Landis(u32 PORTn,u8 *p8CSD,u8 *p8tx);//Landis(IEC1107 ģʽC)������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
	 
u32 RM_TxDATA(u32 PORTn,u8 *p8CSD,u8 *p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
u32 RM_TxDATA_Manage(u32 PORTn);//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ

u32 BroadcastTime_TxDATA_DL645(u8 *p8tx);//DL645�㲥Уʱ����֡;����:��֡���ܳ����ֽ���
u32 BroadcastTime_TxDATA_DL698(u8 *p8tx);//DL698�㲥Уʱ����֡;����:��֡���ܳ����ֽ���

void DL645_68H_Add33H_CS_16H(u8* p8tx);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16 
	 

#ifdef __cplusplus
 }
#endif
#endif
