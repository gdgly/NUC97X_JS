

#ifndef RM_Record_H
#define RM_Record_H
#ifdef __cplusplus
 extern "C" {
#endif



u32 Get_FileID(u32 PORTn);//UART�������UART�������FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0����
u32 IDtoFileID(u32 id);//���:�����id,����:�ļ��洢FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����;��Чid����=0
u32 IDtoTaskID(u32 id);//���:�����id;���ڣ�TaskID
//u32 RCSDtoNextProjectID(u8* pRCSD,u32 ProjectID);//��¼��ѡ��RCSD���Ӧ���¸�������,���:ProjectID=��ʼ�����ķ�����;����:������,>=NUMmax_6014��ʾ��Ч������
//u32 ProjectIDtoTaskID(u32 ProjectID);//��ͨ�ɼ�������תΪ�����;����:�ɼ�����������,>=RMTASKmax��ʾ��Ч�����
u32 RCSDtoID(u8* pRCSD);//��¼��ѡ��RCSD���Ӧ�������,����:�����ڲɼ�����������,>=RMTASKmax��ʾ��Ч�����
u32 RCSDtoFileID(u8* pRCSD);//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff;RCSD��ֻ��ͨ�ŵ�ַ�ȷǳ���������ʱ����0xffffffff
u32 Get_RecordFileName(u32 FileID,u32 FileCount,u8* pTime);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
u32 Get_RecordFileName_JS(u32 FileID,u32 FileCount,u8* pTime);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
 	 
u32 Get_RecordFileData_RCSD(u8 *pRCSD,u8 *pAddr,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax);//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
u32 Get_LastRecordFileData(u32 ID,u16* pAddr);//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
u32 Get_LastRecordFileDataHasID(u32 ID,u16* pAddr);//
	 
u32 AutoDeleteFile(u32 filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
u32 AutoDelete_ErrID_RecordFile(void);//�Զ�ɾ�������Ѳ����ڵļ�¼�ļ�;����:0=ûɾ��,1=��ɾ��
	 
void RM_RecordFile(u32 PORTn,u8 *pbuf);//�������ݴ���(����,�¼�)
void RM_RecordFile_FailAddr(u32 PORTn);//����ʧ�ܵ�ַ��¼
	 
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
