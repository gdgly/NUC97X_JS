

#ifndef Terminal_AFN0C_RealTimeData_Fn_H
#define Terminal_AFN0C_RealTimeData_Fn_H




typedef __packed struct
{
  vu32 Addr;//���ݵ�ַ
	vu8 project;//֧�ַ���:B7=,B6=ͨ��ģ��,B5=�ɼ���,B4=�����ն�,B3=ר���ն�,B2=������,B1=�����,B0=���ܱ�
  vu8 PnType;//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
  vu16 PnLen;//ÿPn���ݳ���
  vu16 PnMax;//���Pn
  vu8 Init;//�ϵ��ʼ��0=��ʼ��,1=����ʼ��
  vu8 InitValue;//��ʼֵ
}Terminal_Class1DataLib_TypeDef;

typedef __packed struct
{
  vu32 SourAddr;//ԭʼ���ݵ�ַ
  vu32 DestAddr;//Ŀ�����ݵ�ַ
	vu8 project;//֧�ַ���:B7=,B6=ͨ��ģ��,B5=�ɼ���,B4=�����ն�,B3=ר���ն�,B2=������,B1=�����,B0=���ܱ�
  vu8 PnType;//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
  vu16 SourPnLen;//ԭʼÿPn���ݳ���
  vu16 DestPnLen;//Ŀ��ÿPn���ݳ���
  vu16 PnMax;//���Pn
}Terminal_HourCongealLib_TypeDef;

u32 Get_Class1Data_MaxFn(void);
u32 Get_InitClass1Data_MaxFn(void);
u32 Get_HourCongeal_MaxFn(void);

u32 Class1Data_FnOnly(u32 ClassNo,u32 Fn);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
u32 Class1Data_Fn(u32 ClassNo,u32 Fn);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
u32 GetClass1DataAddr(u32 Fn,u32 Pn);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
u32 GetClass1Data_PnMax(u32 Fn);//ȡ��1���ݸ�Fn�����Pn
void SaveClass1Data(u32 Fn,u32 Pn,u32 SourAddr);//������1����

void Class1_F33_F34_F37_F38(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4);//����������ϳ�F33_F34_F37_F38
void Class1_F35_F36_F39_F40(u32 Pn,u32 FnDataAddr,u32 Fn1,u32 Fn2);//����������ϳ�F35_F36_F39_F40
#endif
