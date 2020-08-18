

#ifndef EVENT_Record_H
#define EVENT_Record_H
#ifdef __cplusplus
 extern "C" {
#endif

void RM_MeterEventAutoInc0x20220200(u32 PORTn);//���ܱ��¼�������ѡ�����޷�������(�¼���¼���)���Զ�����
u32 Get_RxDATA_DL698_MeterEventCount(u32 PORTn);//��698���¼���¼����֡��ȡ�����ܱ��¼���������(�¼���¼���);����:��������,û�ҵ�ʱ����0xffffffff
u32 CompSourMeterEventCount(u32 SetNo,u32 OAD,u32 Count);//�Ƚ�ԭ���ܱ��¼�����;���:SetNo=�������ú�(0��ʼ),Count=��ǰ����;����:0=��ʼֵ,1=������ͬ,2=������ͬ,ͬʱ�޸�ԭ����,ʹ��ADDR_DATABUFF

u32 GET_EventFileName(u32 OAD,u32 FileCount);//�����¼���¼�ļ���:���:OAD,FileCountֵΪ0xffffffff��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
u32 DEPTH_EventFile(u32 OI);//�¼��ļ��洢���;����:�洢���
u32 GET_EventRecord_RCSD(u32 OAD,u8 *pRCSD,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
u32 GET_EventRecord(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE,u32 ALL);//�õ��¼���¼;���:ALL=0ֻ�¼���OAD�б�,ALL=1��OAD�б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR 
u32 GET_LastEventOccurRecovery(u32 OAD,u8* pTSA);//�õ�������¼������ָ���־;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:0=�ָ�,1=����;ʹ��ADDR_DATABUFF
u32 GET_LastEventTime_OccurRecovery(u32 OAD,u8* pTSA,u8* pOut,u32 OccurRecovery);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
u32 GET_LastEventTime_Occur(u32 OAD,u8* pTSA,u8* pOut);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
u32 GET_LastEventTime_Recovery(u32 OAD,u8* pTSA,u8* pOut);//�õ�������¼��ָ�ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut


EventSour_typedef* GET_EventSour(u16* pRMAddr);//�õ��¼�ԭֵ����ָ�룬���:�����ַ, ����:����ָ��,0=����
void CLR_EventRecord(u32 OI);//����ӦOI���¼���¼
	
	
	
void Event_OAD(u32 OAD,u8* pOccurSour,u32 Occur,u32 OccurkWh);//��׼�¼���¼;���:OAD,pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��,OccurkWh=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF

void Event_30300200(u8* pDATA, u8* pOccurSour,u32 FramLen);//ģ��ID����¼���¼;���:pDATA=ģ��ID�������;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	 
void Event_31050200(u8* pOccurSour,u32 Occur,u8*p1);//���ܱ�ʱ�ӳ����¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
void Event_31060200(u8* pOccurSour,u32 Occur);//�ն�ͣ/�ϵ��¼���¼;���:pOccurSour����Դ=0��ʾ��,occur=0�ָ�=1����=2��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF	 

void Event_31110200(u8* pDATA);//����δ֪���ܱ��¼���¼;���:pDATA=1���ѱ���;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
void Event_31120200(u8* pDATA);//��̨�����ܱ��¼���¼;���:pDATA=һ����̨�����;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
	 
	 
void Event_31150200(u8* pOAD,u32 Occur);//ң���¼���¼(�غ�2����),���:pOAD=�¼�����ԴOAD���̵�����Ԫ��,occur=0�ָ�=1����=2��	 
 
	 
void Event_31180200(u8* pOAD);//�ն˱���¼���¼;���:pOAD=���õ�OAD����ָ��;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF	 
	 
void Event_311C0200(u32 PORTn,u8* pCSD,u8* pDATA1,u8* pDATA2);//���ܱ����ݱ����ؼ�¼,���:pCSD=��ص�CSD,pDATA1=���ǰ����,pDATA2=���������	 
 
void Event_32000200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst,u8* pAfter);//������բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
void Event_32010200(u32 pOccurSour,u32 OI,u32 bit,u8* pFixedValue,u8* pFirst);//�����բ��¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF
void Event_32030200(u32 pOccurSour,u32 OI,u8* pFixedValue);//��ظ澯�¼���¼;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF

void Event_310A0200(u8* pOccurSour,u32 Occur,u8* pTSA);

void Event_311F0200(u8* pDATA,u8* pOccurSour,u32 FramLen);//�ܾ��ӽڵ�������¼:���:pDATA=1���ܾ��ӽڵ�������¼��Ԫ;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF

#if (USER/100)==9//����
void Event_34110200(u8* pDATA);//�����ߴ����¼���¼;���:pDATA=1��������;ʹ��ADDR_DATABUFF��ADDR_128KDATABUFF	 
#endif	 
void Event_31010200(void);//�ն˰汾����¼�	 
	 
#ifdef __cplusplus
 }
#endif
#endif
