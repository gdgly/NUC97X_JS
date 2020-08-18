

#ifndef FLASH_class8_H
#define FLASH_class8_H
#ifdef __cplusplus
 extern "C" {
#endif

// ��������-----------------------------------------------------------------------------------------------
#define ADDR_Com_EFLASH_Start     	ADDR_QCSG_SET_START
#define ADDR_RTCOffice     			ADDR_Com_EFLASH_Start	//�ն�ʱ������Ǳ�ʱ�Ӳ�ֵ(�������ʱ��)
// 4 BYTE HEX����,��
#define ADDR_Language     			ADDR_RTCOffice+4		//��ʾ����ͬ��RAM�е�Comm_Ram->Language
// 1 BYTE 0=����,1=Ӣ��,2=
// 3 BYTE ����
#define ADDR_FileDown     			ADDR_Language+4			//�Զ�����������
//��������1
// 1 BYTE �ŵ�0=RS232,1=GPRS��RJ45
// 1 BYTE ����
// 1 BYTE �ļ��������	BIN	1
// 1 BYTE �ļ����(0-255)	BIN	1
// 16 BYTE �ļ�����	ASCII	16
// 2 BYTE �ļ��ܶ���n(n>=1)	BIN	2
// 2 BYTE �ļ����ݵ�ǰ�����ţ�i=1~n��	BIN	2
// 2 BYTE �ļ�ÿ���ֽڳ���(L)	BIN	2
// 4 BYTE ���������ֽ���
// 2 BYTE ����
// ��������2
// ....
// ��������8
// ...
#define ADDR_LEN_AFN0FF1    		ADDR_FileDown+(32*8)//376.1 F1�����ļ����ܳ��ȼ����Ĵ�
// 4byte
#define ADDR_AFN0FF1     			ADDR_LEN_AFN0FF1+4//376.1 F1����
// 1BYTE �ļ���ʶ:00H�����װ�ļ�;01H-06H�����ļ�
// 1BYTE �ļ����ԣ���ʼ���м�֡��00H������֡��01H��
// 1BYTE �ļ�ָ�00H�����ķ�ʽ��װ��01H��FTP ��ʽ��װ��02H���������ַ������
// 2BYTE �ܶ���n
// 4BYTE ��i �α�ʶ��ƫ�ƣ�i=0��n��
// 2BYTE ��i �����ݳ���Lf
// xBYTE �ļ�����
#define LEN_AFN0FF1     			256
#define ADDR_AFN0CF14     			ADDR_AFN0FF1+LEN_AFN0FF1//�ļ���װδ�յ����ݶ�
// 4BYTE ��� BIN 4
// 128BYTE ���ڸ����ݶ�δ�յ���־ BS8 128
#define ADDR_FTPFileFlags     		ADDR_AFN0CF14+4+128//FTP�ļ����ر�־
// 4BYTE ��
// 1BYTE �����ļ�����;0-x,=MaxFileNum��FTP,=(MaxFileNum+1)����֡,=(MaxFileNum+2)���������ʾ,=(MaxFileNum+3)����������ʾ
// 1BYTE MS�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 1BYTE OS�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 1BYTE Meter�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 1BYTE Terminal�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 1BYTE Menu�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 1BYTE Font�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 1BYTE Module�ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 1BYTE �ļ� :0=û����,1=���ڶ�ר��,2=ר�����,3=���ڶ�ͨ��,4=ͨ�����,5=�ļ�����,6=�ļ�û�ҵ�
// 8BYTE 
#define ADDR_AFN0FF1_Rx     		ADDR_FTPFileFlags+5+16		//FTP����֡��ǰ256�ֽڱ���
// 256BYTE
#define ADDR_AFN0FF1_Tx     		ADDR_AFN0FF1_Rx+256			//FTP����֡��ǰ18�ֽڱ���
// 18BYTE
#define ADDR_UART_PORT_SET     		ADDR_AFN0FF1_Tx+18		//ͨ�ſ�����
// UART0 (RS232)
// 1BYTE ���� 0=�ն˼��ӿ�,!=0�ն�ά����
// 1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE ����
// 1BYTE ����
// UART2(RS485-1)
// 1BYTE ���� 0=�ն�ά����,!=0�ն˳�����
// 1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE ����
// 1BYTE ����
// UART3(RS485-2)
// 1BYTE���� 0=�ն�ά����,!=0�ն˳�����
// 1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE ����
// 1BYTE ����
// UART4(RS485-3)
// 1BYTE ���� 0=�ն�ά����,!=0�ն˳�����
// 1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE ����
// 1BYTE ����
// UART5(����)
// 1BYTE ���� 0=�ն�ά����,!=0�ն˳�����
// 1BYTE ��ʼ���� 0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=23400
// 1BYTE ����
// 1BYTE ����
#define LEN_UART_PORT_SET     	4

// �ն�����ʱ����

#define ADDR_E1800001			(ADDR_UART_PORT_SET+(LEN_UART_PORT_SET*5))
#define LEN_E1800001			(2)
#define ADDR_E1800002			(ADDR_E1800001 + LEN_E1800001)
#define LEN_E1800002			(1)
#define ADDR_E1800003			(ADDR_E1800002 + LEN_E1800002)
#define LEN_E1800003			(1)
#define ADDR_E1800004			(ADDR_E1800003 + LEN_E1800003)
#define LEN_E1800004			(2)
#define ADDR_E1800005			(ADDR_E1800004 + LEN_E1800004)
#define LEN_E1800005			(2)
#define ADDR_E1800006			(ADDR_E1800005 + LEN_E1800005)
#define LEN_E1800006			(2)
#define ADDR_E1800007			(ADDR_E1800006 + LEN_E1800006)
#define LEN_E1800007			(2)
#define ADDR_E1800008			(ADDR_E1800007 + LEN_E1800007)
#define LEN_E1800008			(2)
#define ADDR_E1800009			(ADDR_E1800008 + LEN_E1800008)
#define LEN_E1800009			(16)

#define ADDR_E1800010			(ADDR_E1800009 + LEN_E1800009)
#define LEN_E1800010			(3)
#define ADDR_E1800011			(ADDR_E1800010 + LEN_E1800010)
#define LEN_E1800011			(2)
#define ADDR_E1800012			(ADDR_E1800011 + LEN_E1800011)
#define LEN_E1800012			(2)
#define ADDR_E1800013			(ADDR_E1800012 + LEN_E1800012)
#define LEN_E1800013			(3)
#define ADDR_E1800014			(ADDR_E1800013 + LEN_E1800013)
#define LEN_E1800014			(3)
#define ADDR_E1800015			(ADDR_E1800014 + LEN_E1800014)
#define LEN_E1800015			(1)
#define ADDR_E1800016			(ADDR_E1800015 + LEN_E1800015)
#define LEN_E1800016			(2)
#define ADDR_E1800017			(ADDR_E1800016 + LEN_E1800016)
#define LEN_E1800017			(2)
#define ADDR_E1800018			(ADDR_E1800017 + LEN_E1800017)
#define LEN_E1800018			(9)
#define ADDR_E1800020			(ADDR_E1800018 + LEN_E1800018)
#define LEN_E1800020			(40)

#define ADDR_GUIPASSWORD		(ADDR_E1800020 + LEN_E1800020)
#define LEN_GUIPASSWORD			(4)
#define ADDR_TerminalResetCnt	(ADDR_GUIPASSWORD + LEN_GUIPASSWORD)
#define LEN_TerminalResetCnt	(4)

#define ADDR_Com_EFLASH_BASEEnd     (ADDR_Com_EFLASH_Start + 128*1024)


#define LEN_Com_EFLASH     			(128*1024)	//�޶��ܳ����ֽ���(1������,128K)
#define ADDR_Com_EFLASH_End     	ADDR_Com_EFLASH_BASEEnd

#if (ADDR_Com_EFLASH_End-ADDR_Com_EFLASH_Start)>LEN_Com_EFLASH
#error ADDR_Com_EFLASH_End Over
#endif
//�������ݽ���--------------------------------------------------------------------------------------------


// ������-------------------------------------------------------------------------------------------------
#define ADDR_TERMINAL_PARA_START	(ADDR_QCSG_SET_START + LEN_Com_EFLASH)
#define LEN_TERMINAL_PARA_EFLASH	(384*1024)


#define ADDR_CommunicationPara		ADDR_TERMINAL_PARA_START
#define ADDR_AlarmEventMaskPara		(ADDR_CommunicationPara + 160)
#define ADDR_SettlementDatePara		(ADDR_AlarmEventMaskPara + 100)//err20190109 256�ֽ�Ϊ��ŵ���洢�澯�ı�־
#define ADDR_IDAuthenticationPara	(ADDR_SettlementDatePara + 8)
#define ADDR_ThresholdPara			(ADDR_IDAuthenticationPara + 68)

#define ADDR_AMRChTimeTbl			(ADDR_ThresholdPara + 24)
#define AMR_CH_NUM					(5)
#define ADDR_AMRChSetTbl			(ADDR_AMRChTimeTbl + AMR_CH_NUM*12)

#define ADDR_RS485PortParaTbl		(ADDR_AMRChSetTbl + AMR_CH_NUM*652)	//3260
#define ADDR_AttachmentPara			(ADDR_RS485PortParaTbl + 3*8)
#define ADDR_LoadManagementPara		(ADDR_AttachmentPara + 40)
#define ADDR_MeterPointCtrlPara		(ADDR_LoadManagementPara + 536)



#define ADDR_AnalogPara				(ADDR_MeterPointCtrlPara + 68)
#define ADDR_PulsePara				(ADDR_AnalogPara + 52)
#define ADDR_CaculationPara			(ADDR_PulsePara + 52)
#define ADDR_DiffPara				(ADDR_CaculationPara + 552)

#define ADDR_NormalTaskPara			(ADDR_DiffPara + 296)
#define NormalTaskPara_Num			(32)
#define ADDR_RelayTaskPara			(ADDR_NormalTaskPara + 672*NormalTaskPara_Num)	//21504
#define RelayTaskPara_Num			(256)	//(RelayTaskPara Len: 296*256) //75776
#define ADDR_SelfAdaptionPara		(ADDR_RelayTaskPara + 296*RelayTaskPara_Num)	//����Ӧͨ�Ų��� 0:������Ӧ 1:����Ӧ
//SelfAdaptionPara_Len = 4
#define ADDR_AlarmSavePara			(ADDR_SelfAdaptionPara + 4)//err20190216 �澯��־����洢����
//AlarmSavePara_Len = 260

#define ADDR_CommunicationPara_E0000111 (ADDR_AlarmSavePara+260)
#define ADDR_CommunicationPara_E0000112 (ADDR_CommunicationPara_E0000111+420)
#define ADDR_CommunicationPara_E0000113 (ADDR_CommunicationPara_E0000112+420)
//ADDR_CommunicationPara_E0000113_Len = 420



//�ϼ�:102612+4+258�ֽ�  		Լ100M
// 
#define ADDR_TERMINAL_PARA_END     ADDR_AlarmSavePara+258
#if ((ADDR_TERMINAL_PARA_END)-(ADDR_TERMINAL_PARA_START))>LEN_TERMINAL_PARA_EFLASH
	#error
#endif

// ����������-------------------------------------------------------------------------------------------------


// �����㵵���洢��---512K------------------------------------------------------------------------------------------
#define ADDR_MP_START				(ADDR_TERMINAL_PARA_START + LEN_TERMINAL_PARA_EFLASH)
#define LEN_MP_RECORD_SIZE			(512*1024)
#define MP_SIZE						(64)
#define POWER_QUALITY_SIZE			(104)
#define MP_RECORD_SIZE				(MP_SIZE + POWER_QUALITY_SIZE)

#define ADDR_MP_END     ADDR_MP_START+MP_RECORD_SIZE
#if ((ADDR_MP_END)-(ADDR_MP_START))>LEN_MP_RECORD_SIZE
	#error
#endif



#define ADDR_QCSG_SET_END     ADDR_MP_START+LEN_MP_RECORD_SIZE

#if ((ADDR_QCSG_SET_END)-(ADDR_QCSG_SET_START)) > LEN_FLASH_QCSG_SET
#error LEN_FLASH_QCSG_SET
#endif




#ifdef __cplusplus
 }
#endif
#endif


