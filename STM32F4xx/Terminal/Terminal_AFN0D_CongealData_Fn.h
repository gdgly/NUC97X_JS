

#ifndef Terminal_AFN0D_CongealData_Fn_H
#define Terminal_AFN0D_CongealData_Fn_H



typedef __packed struct
{
  vu32 OfficeAddr;//����ƫ�Ƶ�ַ
  vu32 SourAddr;//����ʱԭʼ���ݵ�ַ
	vu8 project;//֧�ַ���:B7=,B6=ͨ��ģ��,B5=�ɼ���,B4=�����ն�,B3=ר���ն�,B2=������,B1=�����,B0=���ܱ�
  vu8 PnType;//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
  vu16 PnLen;//ÿPn���ݳ���
  vu16 SourPnLen;//ԭʼ����ÿPn���ݳ���
  vu16 PnMax;//���Pn
  vu8 CongealType;//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
  vu8 SourInit;//�����ԭʼ���ݴ���:0=����,1=��Ϊ0,2=��Ϊ0xee,3=�ó�ֵ
}Terminal_Class2DataLib_TypeDef;


u32 Class2Data_FnOnly(u32 ClassNo,u32 Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
u32 Class2Data_Fn(u32 ClassNo,u32 Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
u32 GetClass2DataOfficeAddr(u32 Fn,u32 Pn);//ȡ��2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
u32 GetClass2DataF246OfficeAddr(u32 CongealNo,u32 Pn);//ȡ��2����F246����ƫ�Ƶ�ַ,����0��ʾû�ҵ�
u32 GetClass2DataOfficeAddr_MapPn(u32 Fn,u32 Pn);//ȡ����ӳ��Pn����2���ݴ���ƫ�Ƶ�ַ,����0��ʾû�ҵ�
u32 GetClass2DataCongealType(u32 Fn);//ȡ��2���ݶ�������
u32 GetClass2DataLen(u32 Fn,u32 Pn,u32 DataAddr);//ȡ��2���ݳ���
u32 GetDateCongealNo(u32 YMD);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
u32 GetDateCongealAddr(u32 YMD,u32 Fn,u32 Pn);//�õ��ն���洢��ַ,����=0��ʾû�ҵ�
u32 GetRMDateCongealNo(u32 YMD);//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
u32 GetRMDateCongealAddr(u32 YMD,u32 Fn,u32 Pn);//�õ������ն���洢��ַ,����=0��ʾû�ҵ�
u32 GetMonthCongealNo(u32 YM);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
u32 GetMonthCongealAddr(u32 YM,u32 Fn,u32 Pn);//�õ��¶���洢��ַ,����=0��ʾû�ҵ�
u32 GetCurveCongealNo(u64 YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
u32 GetCurveCongealAddr(u64 YMDHM,u32 Fn,u32 Pn);//�õ����߶���洢��ַ,����=0��ʾû�ҵ�
u32 GetACSampleCurveCongealAddr(u64 YMDHM,u32 Fn);//�õ����ɲ��������߶���洢��ַ,����=0��ʾû�ҵ�

u32 CheckCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn);//������߶���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
void AddCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte);//�����߶��������
u64 CurveCongeal_YMDHM(void);//��ǰ��׼���߶���ʱ��YMDHM
u32 CheckDateCongeal(u32 YMD,u32 Fn,u32 Pn);//����ն���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
void AddDateCongeal(u32 YMD,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte);//���ն��������
u32 CheckMonthCongeal(u32 YM,u32 Fn,u32 Pn);//����¶���;����:0=����û����,1=�����Ѷ���,2=û�ҵ���Ч����
void AddMonthCongeal(u32 YM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte);//���¶��������


void Class2_Group(u32 CongealAddr,u32 Fn,u32 Pn);//2�����ݺϳ�;����:���ݷ�ADDR_DATABUFF
void UART_AFN0DF97F98F99F100_Computer(u32 ADDR_FullData,u64 YMDHM,u32 m,u32 Fn,u32 Pn,u32 CongealNo);//2���������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵ����Fn
void UART_AFN0DF5F6F7F8_Computer(u32 ADDR_FullData,u32 YMD,u32 Fn,u32 Pn,u32 Len);//2���ն������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn
void UART_AFN0DF21F22F23F24_Computer(u32 ADDR_FullData,u32 YM,u32 Fn,u32 Pn,u32 Len);//2���¶������ݵ���������;��ڣ�FnΪ���ڼ����ʾֵFn


void SaveClass2Data(u32 PORTn,u32 Pn,u32 FnDataAddr,u8 *p8RMLib);//������2�����㳭������

u32 CreateCurveCongealNo(u64 YMDHM);//�������߶����;����:ԭ�Ѵ��ڻ��´����Ķ����
u32 CreateDateCongealNo(u32 YMD);//�����ն����;����:ԭ�Ѵ��ڻ��´����Ķ����
u32 CreateMonthCongealNo(u32 YM);//�����¶����;����:ԭ�Ѵ��ڻ��´����Ķ����

u32 LastRMDateCongealYMD(u32 PORTn);//���˿ںż������γ����ն����ʱ��������;���:ͨ�ſں�;����:0��ʾû
u32 GetLastRMDateCongealAddr(u32 PORTn,u32 Fn,u32 Pn);//���˿ںŵõ����γ����ն���洢��ַ,����=0��ʾû�ҵ�


#endif


