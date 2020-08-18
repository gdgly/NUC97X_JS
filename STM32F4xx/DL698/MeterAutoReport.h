

#ifndef MeterAutoReport_H
#define MeterAutoReport_H
#ifdef __cplusplus
 extern "C" {
#endif

typedef __packed struct
{
	u8 BIT;//��ӦDL645-2007�����ϱ�״̬���е�λB0-B95
	u32 OAD;//��Ӧ�¼���OAD
	u32 DI_NUM;//��ӦDL645-2007�¼����������ĳ�����ʶ,0=��
	u32 DI_TIME;//��ӦDL645-2007�¼��ۼ�ʱ��ĳ�����ʶ,0=��
	u32 DI_LASTEVENT1;//��ӦDL645-2007���1���¼��ĳ�����ʶ1,0=��
	u32 DI_LASTEVENT2;//��ӦDL645-2007���1���¼��ĳ�����ʶ2,0=��
}MeterAutoReportLIB_typedef;

u32 MeterAutoReportStateWord(u32 PORTn,u8* p8rx);//�����ϱ�״̬��;���:�Զ��ϱ�����֡;����:0=û����,1=����
void MeterAutoReport_RS485(u32 PORTn);//���ܱ������ϱ�����-rs485:0=��,1=�������ϱ��ȴ���ȡ,2...������
void MeterAutoReport_3762(u32 PORTn);//���ܱ������ϱ�����-3762:0=��,1=�������ϱ��ȴ���ȡ,2...������



#ifdef __cplusplus
 }
#endif
#endif
