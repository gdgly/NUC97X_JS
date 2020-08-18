

#ifndef RM_TASK_H
#define RM_TASK_H
#ifdef __cplusplus
 extern "C" {
#endif



u32 RM_MeterCount(u32 PORTn);//���ܱ����+1;����:0=����,1=�����
u32 RM_Addr(u32 PORTn,u16* pAddr);//ָ��ָ���ͨ���Ƿ��ѳ�;����:0=û��,1=�ѳ�
u32 RM_Meter(u32 PORTn);//��ǰ���ܱ�����Ƿ�Ҫ��;���:UARTCtrl->MeterCount[],UARTCtrl->TaskID;����:0=����,1=Ҫ��,2=���,ͬʱ��дͨ�ŵ�ַ,������,�����Լ����
u8 *RM_CSD(u32 PORTn);//ȡ�������ݱ�ʶCSD;����:0=��Ч,1=CSD��URATCtrl,����ֵ=͸������֡����ָ��
	 
//p16=(u16*)(ADDR_6000_SDRAM+(MeterSetNo*LENper_6000)+8);//ȡ�ɼ�����������ŵ�ͨ�ŵ�ַ
u16* MeterSetNoToCollectorAddr(u32 MeterSetNo);//ȡ�ɼ�����������ŵĲɼ�����ַ;���:��0��ʼ�����;����:�ɼ�����ַָ��,0=��Ч
void NUMrm(u32 PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
void RM_OK_FAIL(u32 PORTn,u32 OK);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�,2=�ɹ�������Ϊ��
u32 DateSaveTI(u32 PORTn,u8* pOUT);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:ʱ������1=����ʼʱ��,2=��Ե���0��0��,3=...,6=;pOUT=date_time_s
void RM_UpDate(void);//�����������;����:�ж�,�ض��ɼ��������ñ��flash��sdram,��λ��PORTn���������ؼ������賭�����ɹ�����


void RM_ADDR_Display(u32 PORTn);//�����ַ��ʾ
void RM_NUM_Display(u32 PORTn);//����������ʾ
void RM_Monitor_Init(u32 ID);//�ɼ������س�ʼ��;���:ID=�����
void RM_Monitor_Init_PORTn(u32 PORTn,u32 ID);//�ɼ������س�ʼ��(���˿ں�);���:PORTn=�˿ں�,ID=�����
void RM_Monitor(u32 ID,u32 NUM_Tx,u32 NUM_Rx);//�ɼ�������;���:ID=�����,NUM_Tx=��������,NUM_Rx=��������
void RM_Monitor_TxNUM(u32 ID,u32 NUM_Tx);//�ɼ�������;���:ID=�����,NUM_Tx=��������
void RM_Monitor_RxNUM(u32 ID,u32 NUM_Rx);//�ɼ�������;���:ID=�����,NUM_Rx=��������
	 
void RM_State_SetUpDate(void);//�ɼ�״̬,�������ʱ����(��Ӧ�������)
void RM_State_RMUpDate(u32 PORTn,u32 OK,u32 SetNo,u32 Class,u32 Phase);//�ɼ�״̬,ÿ�γ�������:���:OK=0ʧ��=1�ɹ�,OK=1ʱClass=�м̼���,Phase=��λ
//void RM_State_UpDate(u32 PORTn,u32 SetNo,u32 Phase,u32 Err);//�ɼ�״̬����:���:SetNo=���ú�,Phase=��λ,Err=��·�쳣
//void RM_State_ReadRouterAllUpDate(u32 PORTn);//�ɼ�״̬,��ȫ��·����Ϣ����
	 
	 
void PROXY_ADDR_Display(u32 PORTn);//�����ַ��ʾ
	 
	
u32 Get_SETINIT_601E_List_NUM(void);//�õ��ɼ�������ʼ���ÿ��б����	



#ifdef __cplusplus
}
#endif
#endif



