

#ifndef Terminal_Uart_AFN04_Paramter_H
#define Terminal_Uart_AFN04_Paramter_H

typedef __packed struct
{
  vu32 Addr;//���ݵ�ַ
	vu8 project;//֧�ַ���:B7=,B6=ͨ��ģ��,B5=�ɼ���,B4=�����ն�,B3=ר���ն�,B2=������,B1=�����,B0=���ܱ�
  vu8 PnType;//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
  vu16 Len;//���ݳ���
  vu16 PnMax;//���Pn
}TerminalParamterLib_TypeDef;

u32 Get_AFN04ParamterLib_MaxFn(void);

#endif
