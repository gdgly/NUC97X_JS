

#ifndef Terminal_Uart_AFN05_Command_H
#define Terminal_Uart_AFN05_Command_H

typedef __packed struct
{
  vu32 Addr;//���ݵ�ַ
	vu8 project;//֧�ַ���:B7=,B6=ͨ��ģ��,B5=����������,B4=�����ն�,B3=ר���ն�,B2=������,B1=�����,B0=���ܱ�
  vu8 PnType;//p0Type=�ն���Ϣ��,pnType=�������,GnType=�ܼ����,CnType=�������������,DnType=ֱ��ģ������,RnType=�����ִκ�,TnType=�����
  vu16 Len;//���ݳ���
  vu8 PnMax;//���Pn
  vu8 Command;//������0x0��0x55
}TerminalCommandUartLib_TypeDef;


u32 Get_AFN05CommandLib_MaxFn(void);

#endif


