

#ifndef CONGEAL_H
#define CONGEAL_H
#ifdef __cplusplus
 extern "C" {
#endif


u32 Get_CongealFileData_RCSD(u8 *pRCSD,u8 *pRecord,u8* pTime,u32 NUM_DATA,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL);//��������ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pRecordָ�����ݿ�,pTimeָ�򶳽�ʱ��(����������),NUM_DATA���������б���,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
u32 Get_CongealFileData(u32 FileID,u32 FileCount,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax);//�õ������ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF

void CONGEAL_Record(void);//����
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
