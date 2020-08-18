
//�ն�
#ifndef IRAM_Terminal_H
#define IRAM_Terminal_H

#include "IRAM_TerminalDisplay.h"

#define ADDR_Terminal_ClassDataIRAM_Satrt     (ADDR_TerminalDisplay_IRAM_End)
#define ADDR_Terminal_ClassDataLen     ADDR_Terminal_ClassDataIRAM_Satrt//ERAM���ֶ���
//1�����ݳ��ȼĴ�����2�����ݷ��ʼĴ���(ԭ2������ֱ�Ӹ�дFLASH,��������29GLд��0.5������ȫ��������̫��̫��ʱ��,�����ӷ��ʼĴ���)
//1byte ��ǰȡ�õ�2�����ݵķ�������г������
//1byte ��ǰȡ�õ�2�����ݵķ����ֽ�ƫ��
//1byte 1=��ǰȡ��2�����ݵķ�������г������������,0=��������
//1byte
#define ADDR_HourCongeal_AutoRW     ADDR_Terminal_ClassDataLen+4//Сʱ�����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
//1BYTE ��д��������0=IOCLR,1=IOR,2=IOW
//1BYTE IOCLR��ֵ
//1BYTE ���ݱ��0=
//1BYTE ���ݼ���
//4BYTE R0��ַ(sram)
//4BYTE R1��ַ(flash������)
//4BYTE R2����,ֵ!=0��ʾ����δ���
//4BYTE ���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
#define ADDR_Class2Congeal_AutoRW     ADDR_HourCongeal_AutoRW+20//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
//1BYTE ��д��������0=IOCLR,1=IOR,2=IOW
//1BYTE IOCLR��ֵ
//1BYTE ���ݱ��0=���߶���,1=�ն���,2=�����ն���,3=�¶���,4=�ն������,5=������2��,6=������3��
//1BYTE ���ݼ���
//4BYTE R0��ַ(sram)
//4BYTE R1��ַ(flash������)
//4BYTE R2����,ֵ!=0��ʾ����δ���
//4BYTE ���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
//5BYTE ����ʱ��������ʱ��
//3BYTE 

//��������
#define ADDR_Terminal_impIn_ESRAM_START     (((ADDR_Class2Congeal_AutoRW+28)+3)&0xfffffffc)
#define ADDR_Terminal_impIn_Ctrl     ADDR_Terminal_impIn_ESRAM_START//�ն������������(�ֶ���)
//1BYTE ����1����10ms����
//...
//1BYTE ����8����10ms����
#define ADDR_Terminal_impIn_Count     ADDR_Terminal_impIn_Ctrl+8
//2BYTE ����1�������(���ֶ���)
//...
//2BYTE ����MaxImpInPort�������(���ֶ���)
#define ADDR_Terminal_InDemandimp     ADDR_Terminal_impIn_Count+(2*MaxImpInPort)//�ն���������,�������廬��(�ֶ���)
//4*15*2BYTE ��������1(���ֶ���)
//...
//4*15*2BYTE ��������MaxImpInPort(���ֶ���)
#define LEN_Terminal_InDemandimp     (4*15*2)//ÿ�����ÿ15�뻬��
#define ADDR_Terminal_impIn_ESRAM_END     ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*MaxImpInPort)
#define ADDR_Average_Count     ADDR_Terminal_impIn_ESRAM_END//�ڲ�����1����ƽ����ѹֵ�������
//1BYTE �ϴμ���ʵʱ����
//1BYTE �ϴμ���ʵʱ�ӷ�
//1BYTE ƽ��ֵ����Ӵ�������
//3BYTE HEX UA��
//3BYTE HEX UB��
//3BYTE HEX UC��
#define ADDR_Average_Voltage     ADDR_Average_Count+12//�ڲ�����A,B,C��1����ƽ����ѹ(�ϵ��ʼΪ0xee)
//2BYTE_BCD 1λС��,A��,(�ϵ��ʼΪ0xee)
//2BYTE_BCD 1λС��,B��,(�ϵ��ʼΪ0xee)
//2BYTE_BCD 1λС��,C��,(�ϵ��ʼΪ0xee)
#define ADDR_I2Task     ADDR_Average_Voltage+6//I2���������(ԭ��SDRAM,��SDRAM�������޷�����������SDRAM�������Ŀ��ܹʸķ���IRAM)
//1byte ����
		
		
#define ADDR_Terminal_ClassDataIRAM_End     (ADDR_I2Task+1)


#if ((ADDR_Terminal_ClassDataIRAM_End-ADDR_Terminal_ClassDataIRAM_Satrt)>(1*1024))
	#error ADDR_Terminal_ClassDataIRAM>1024
#endif


#define ADDR_Terminal_IRAM_End     ((ADDR_Terminal_ClassDataIRAM_End+3)&0xfffffffc)//4�ֽڶ���


#endif






























