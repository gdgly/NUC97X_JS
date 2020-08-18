
//�ն�
#ifndef IRAM_Terminal_H
#define IRAM_Terminal_H

#include "IRAM_TerminalDisplay698.h"




#define ADDR_IRAMTerminal_Start     (ADDR_TerminalDisplay_IRAM_End)

//��������
#define ADDR_Terminal_impIn_ESRAM_START     ADDR_IRAMTerminal_Start
#define ADDR_Terminal_impIn_Ctrl     ADDR_Terminal_impIn_ESRAM_START//�ն������������(�ֶ���)
//1BYTE ����1����10ms����
//...
//1BYTE ����8����10ms����
#define ADDR_Terminal_impIn_Count     ADDR_Terminal_impIn_Ctrl+NUMmax_IMPIN
//2BYTE ����1�������(���ֶ���)
//...
//2BYTE ����NUMmax_IMPIN�������(���ֶ���)
#define ADDR_Terminal_InDemandimp     ADDR_Terminal_impIn_Count+(2*NUMmax_IMPIN)//�ն���������,�������廬��(�ֶ���)
//4*15*2BYTE ��������1(���ֶ���)
//...
//4*15*2BYTE ��������NUMmax_IMPIN(���ֶ���)
#define LEN_Terminal_InDemandimp     (4*15*2)//ÿ�����ÿ15�뻬��
#define ADDR_Terminal_impIn_ESRAM_END     ADDR_Terminal_InDemandimp+(LEN_Terminal_InDemandimp*NUMmax_IMPIN)

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
		
		
		
		
#define ADDR_IRAMTerminal_End     (ADDR_I2Task+1)
#if ((ADDR_IRAMTerminal_End)-(ADDR_IRAMTerminal_Start))>(1*1024)
	#error
#endif


#define ADDR_Terminal_IRAM_End     ((ADDR_IRAMTerminal_End+3)&0xfffffffc)//4�ֽڶ���


#endif






























