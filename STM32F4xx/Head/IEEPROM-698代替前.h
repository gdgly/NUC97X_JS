
//EEPROM
#ifndef IEEPROM_H
#define IEEPROM_H




//90E36У�����------------------------------------------------------------------------------------
#define ADDR_MeterCalibration_IEEPROM_Start     0x08008000//16k
//4BYTE CRC32
//4BYTE У�������Ч��־�̶�Ϊ0x12345678
//2BYTE RTCCAL
//2BYTE �¶�CAL
#define ADDR_ADC4MA     ADDR_MeterCalibration_IEEPROM_Start+12
//2BYTE ֱ��ģ����������4MA����ʱADCת����׼ֵ
#define ADDR_ADC20MA     ADDR_MeterCalibration_IEEPROM_Start+14
//2BYTE ֱ��ģ����������20MA����ʱADCת����׼ֵ
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
#define ADDR_EMU_41     ADDR_MeterCalibration_IEEPROM_Start+8+16
//2BYTE Phase A Active Power Offset
#define ADDR_EMU_42     ADDR_EMU_41+2
//2BYTE Phase A Reactive Power Offset
#define ADDR_EMU_43     ADDR_EMU_42+2
//2BYTE Phase B Active Power Offset
#define ADDR_EMU_44     ADDR_EMU_43+2
//2BYTE Phase B Reactive Power Offset
#define ADDR_EMU_45     ADDR_EMU_44+2
//2BYTE Phase C Active Power Offset
#define ADDR_EMU_46     ADDR_EMU_45+2
//2BYTE Phase C Reactive Power Offset
#define ADDR_EMU_47     ADDR_EMU_46+2
//2BYTE Phase A Active/Reactive Energy calibration gain
#define ADDR_EMU_48     ADDR_EMU_47+2
//2BYTE Phase A calibration phase angle
#define ADDR_EMU_49     ADDR_EMU_48+2
//2BYTE Phase B Active/Reactive Energy calibration gain
#define ADDR_EMU_4A     ADDR_EMU_49+2
//2BYTE Phase B calibration phase angle
#define ADDR_EMU_4B     ADDR_EMU_4A+2
//2BYTE Phase C Active/Reactive Energy calibration gain
#define ADDR_EMU_4C     ADDR_EMU_4B+2
//2BYTE Phase C calibration phase angle
#define ADDR_EMU_51     ADDR_EMU_4C+2
//2BYTE Phase A Fundamental Active Power Offset
#define ADDR_EMU_52     ADDR_EMU_51+2
//2BYTE Phase B Fundamental Active Power Offset
#define ADDR_EMU_53     ADDR_EMU_52+2
//2BYTE Phase C Fundamental Active Power Offset
#define ADDR_EMU_54     ADDR_EMU_53+2
//2BYTE Phase A Fundamental Active Power Gain
#define ADDR_EMU_55     ADDR_EMU_54+2
//2BYTE Phase B Fundamental Active Power Gain
#define ADDR_EMU_56     ADDR_EMU_55+2
//2BYTE Phase C Fundamental Active Power Gain
#define ADDR_EMU_61     ADDR_EMU_56+2
//2BYTE Phase A Voltage RMS Gain
#define ADDR_EMU_62     ADDR_EMU_61+2
//2BYTE Phase A Current RMS Gain
#define ADDR_EMU_63     ADDR_EMU_62+2
//2BYTE Phase A Voltage RMS Offset
#define ADDR_EMU_64     ADDR_EMU_63+2
//2BYTE Phase A Current RMS Offset
#define ADDR_EMU_65     ADDR_EMU_64+2
//2BYTE Phase B Voltage RMS Gain
#define ADDR_EMU_66     ADDR_EMU_65+2
//2BYTE Phase B Current RMS Gain
#define ADDR_EMU_67     ADDR_EMU_66+2
//2BYTE Phase B Voltage RMS Offset
#define ADDR_EMU_68     ADDR_EMU_67+2
//2BYTE Phase B Current RMS Offset
#define ADDR_EMU_69     ADDR_EMU_68+2
//2BYTE Phase C Voltage RMS Gain
#define ADDR_EMU_6A     ADDR_EMU_69+2
//2BYTE Phase C Current RMS Gain
#define ADDR_EMU_6B     ADDR_EMU_6A+2
//2BYTE Phase C Voltage RMS Offset
#define ADDR_EMU_6C     ADDR_EMU_6B+2
//2BYTE Phase C Current RMS Offset
#define ADDR_EMU_6D     ADDR_EMU_6C+2
//2BYTE Sampled N line Current RMS Gain
#define ADDR_EMU_6E     ADDR_EMU_6D+2
//2BYTE Sampled N line Current RMS Offset



#define ADDR_MeterCalibration_IEEPROM_End     (ADDR_EMU_6E+2)
#define LEN_MeterCalibration_IEEPROM     (128*2)//У������ܳ���=128*2�ֽ�
#if ((ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_Start)>LEN_MeterCalibration_IEEPROM)
#error ADDR_MeterCalibration_IEEPROM_End Over
#endif
//У���������--------------------------------------------------------------------------------


//���ܱ����춨��¼����---------------------------------------------------------
#define ADDR_MeterError_IEEPROM_Start     (ADDR_MeterCalibration_IEEPROM_Start+LEN_MeterCalibration_IEEPROM)
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����1.0 Imax
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����0.5L Imax
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����1.0 Ib
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����0.5L Ib
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����1.0 5%Ib
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����0.5L 5%Ib
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����1.0 1%Ib
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ����0.5L 1%Ib
//A��...
//B��...
//C��...

//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� A���ѹ���
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� B���ѹ���
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� C���ѹ���
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� A��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� B��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� C��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ���й��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� A���й��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� B���й��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� C���й��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� ���޹��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� A���޹��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� B���޹��������
//2BYTE_BCD 3λС�� ���λΪ1��ʾ�� C���޹��������
	//����64+28BYTE

#define LEN_MeterError_IEEPROM     (128*1)//���ܱ����춨��¼�����ܳ���=128*1�ֽ�
#define ADDR_MeterError_IEEPROM_End          (ADDR_MeterError_IEEPROM_Start+LEN_MeterError_IEEPROM)
//���ܱ����춨��¼���ݽ���----------------------------------------------------------------------


#if ((ADDR_MeterError_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_Start)>0x4000)
#error ADDR_MeterError_IEEPROM Over
#endif



//NANDFLASH REPLACE----------------------------------------------------------------------------------------------
#define ADDR_NANDFLASH_BLOCK_REPLACE     0x0800C000//16k   NANDFLASH REPLACE
//����1�ļ�ϵͳʹ�õĿ�:0xFC=�ѳ�ʼ����־,0xFD=δ��,0xFE=����,(0x00,0xFF����Ƭ�ӳ�����ʼֵ����);�ļ�ϵͳ�Ŀ�ֻ��3����ʾֵ0xFC,0xFD��0xFE,����ֵ��ͬ0xFF
//1BYTE �����0
//1BYTE ...
//1BYTE �����NAND_FILE_BLOCK_COUNT-1
//����2�滻�õĿ�:0xFD=�������滻,0xFE=����,(0x00,0xFF����Ƭ�ӳ�����ʼֵ����);�滻�õĿ�ֻ��2����ʾֵ0xFD��0xFE,����ֵ��ͬ0xFF
//1BYTE �����NAND_FILE_BLOCK_COUNT
//1BYTE ...
//1BYTE �����NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT-1
//����3ͨ�����ݿ�:1-NAND_REPLACE_BLOCK_COUNT��ʾ���滻���滻���ͬʱ��ʾ�����,1-250(0xFA)=���滻���滻�����滻���,0xFB=����,0xFC=����,0xFD=δ��(���滻),0xFE=����(���滻),(0x00,0xFF����Ƭ�ӳ�����ʼֵ����)
//1BYTE �����NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT
//...
//1BYTE �����NAND_BLOCK_COUNT-1

#define ADDR_NANDFLASH_BLOCK_REPLACE_End     (ADDR_NANDFLASH_BLOCK_REPLACE+8192)
#if NAND_BLOCK_COUNT>8192
#error NAND_BLOCK_COUNT>8192
#endif
//NANDFLASH REPLACE����--------------------------------------------------------------------------------------------------



//�������ܲ��Լ�¼-----------------------------------------------------------------------------------
#define ADDR_Characteristics_NO     ADDR_NANDFLASH_BLOCK_REPLACE_End
//1BYTE ���¶����ֵĲ��Ժ�:0=����(-5<T<45),1=����(T<-5),2=���£�T>55)
#define ADDR_Characteristics_BASE1     (ADDR_Characteristics_NO+1)//���¶����ֵĲ��Ժ�:0=����
#define OFFSET_Test_Temperature     0
//2BYTE BCD 1λС��,����ʱ�����¶�
#define OFFSET_NAND_ERROR_BLOCKS     OFFSET_Test_Temperature+2
//2BYTE nand�������,0xffff��ʾû����
#define OFFSET_NAND_LIFE     OFFSET_NAND_ERROR_BLOCKS+2
//4BYTE nand����(�Ѳ�д����)
#define OFFSET_BATTERY_LIFE     OFFSET_NAND_LIFE+4
//2BYTE ���ʱ���
//2BYTE �ŵ��ʱ���
//1BYTE ��翪ʼʱ��ѹx.xV BCD
//1BYTE ������ʱ��ѹx.xV BCD
//1BYTE �ŵ翪ʼʱ��ѹx.xV BCD
//1BYTE �ŵ����ʱ��ѹx.xV BCD

#define OFFSET_POWER_Characteristics     (OFFSET_BATTERY_LIFE+8)
//2BYTE BCD 2λС��,VDD5V0
//2BYTE BCD 2λС��,ADD5V0
//2BYTE BCD 2λС��,IO3V3
//2BYTE BCD 2λС��,RTC_BATTERY3V0
//2BYTE BCD 2λС��,CHARGE_BATTERY4V8
//2BYTE BCD 2λС��,GPRS4V0
//2BYTE BCD 2λС��,ETH3V3
//2BYTE BCD 2λС��,CHARGE12V
#define OFFSET_POWER_DOWN_Characteristics     OFFSET_POWER_Characteristics+16
//1BYTE 4K(BKPSRAM)���籣��,0=OK,1=ERR,0xff=û����,0x55=���в���
//1BYTE ��Դ�������ݻָ�,0=OK,1=ERR,0xff=û����,0x55=���в���
//1BYTE ��Դ����ʱ���ݱ���ʱ��ms
//2BYTE ��Դ����ά�ֹ���ʱ��ms
#define OFFSET_STM32F4xx_Characteristics     OFFSET_POWER_DOWN_Characteristics+5//STM32F4xx
//1BYTE ISRAM 0=OK,1=ERR,0xff=û����
//1BYTE ECC 0=OK,1=ERR
//1BYTE CRC 0=OK,1=ERR
//1BYTE �ⲿ����(LSE)32768Hz:0=OK,1=ERR,0xff=û����
//4BYTE �ⲿ����(HSE)16MHz����Ƶ��Hz
//4BYTE �����ڲ� RC (LSI RC)Ƶ��Hz
#define OFFSET_SDRAM_Characteristics     OFFSET_STM32F4xx_Characteristics+12//SDRAM��������
//1BYTE SDRAM_CAS_LATENCY
//1BYTE SdramTiming.LoadToActiveDelay;// = 16;//1-16//����ģʽ�Ĵ��������� (Load Mode Register to Active)
//1BYTE SdramTiming.ExitSelfRefreshDelay;// = 1;//1-16//�˳���ˢ���ӳ� (Exit Self-refresh delay)
//1BYTE SdramTiming.SelfRefreshTime;// = 1;//1-16//��ˢ��ʱ�� (Self refresh time)
//1BYTE SdramTiming.RowCycleDelay;// = 4;//1-16//��ѭ���ӳ� (Row cycle delay)
//1BYTE SdramTiming.WriteRecoveryTime;// = 2;//1-16//�ָ��ӳ� (Recovery delay)
//1BYTE SdramTiming.RPDelay;// = 1;//1-16//��Ԥ����ӳ� (Row precharge delay)
//1BYTE SdramTiming.RCDDelay;// = 1;//1-16//�е����ӳ� (Row to column delay)
#define OFFSET_NAND_Characteristics     OFFSET_SDRAM_Characteristics+8//NAND��������
//1BYTE hnand.Init.TCLRSetupTime = 0;//;CPUֵ(0-15);CLE �� RE ���ӳ�
//1BYTE hnand.Init.TARSetupTime = 0;//;CPUֵ(0-15);ALE �� RE ���ӳ� 
//1BYTE ComSpaceTiming.SetupTime = 8-1;//1-255;CPUֵ(0-255)ͨ�ô洢������ʱ��
//1BYTE ComSpaceTiming.WaitSetupTime = 4-1;//2-255;CPUֵ(1-255)ͨ�ô洢���ȴ�ʱ��
//1BYTE ComSpaceTiming.HoldSetupTime = 2;//1-254;CPUֵ(1-255)ͨ�ô洢������ʱ��
//1BYTE ComSpaceTiming.HiZSetupTime = 1-1;//1-255;CPUֵ(0-255)ͨ�ô洢���������߸���̬ʱ��
//2BYTE Block Erase Time(ms)
//2BYTE Program Time(us)
//2BYTE Page Read Time(us)
#define OFFSET_90E36_Characteristics     OFFSET_NAND_Characteristics+12
//4BYTE ���ͨ������Hz
//1BYTE ��Ч��λʱ��us
#define OFFSET_RTC_Characteristics     OFFSET_90E36_Characteristics+5
//1BYTE ����ʱ��99-12-31 6 23:59:59�����Ƚ�:0=��ȷ,1=����
//1BYTE 1����ʱ��ֵ����λ00-01-01 0 00:00:00 ;0=��ȷ,1=����
//2BYTE I2C���߸�λ���ʱ��
//1BYTE 1Hz���(0=OK,1=ERROR)
//1BYTE 32kHz���(0=OK,1=ERROR)
#define OFFSET_LAN8720A_Characteristics     OFFSET_RTC_Characteristics+6
//1BYTE Serial Management Interface (SMI):0=OK,1=ERR,0xff=û����
//1BYTE Auto-Negotiation:0=OK,1=ERR,0xff=û����
//1BYTE REF_CLK:0=OK,1=ERR,0xff=û����
//1BYTE tRESET��Ч��λʱ��(us)
#define OFFSET_UART_Characteristics     OFFSET_LAN8720A_Characteristics+4
//1BYTE RS485-1,RS485-2ͨ������O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400
//1BYTE �����,ͨ������O��11���α�ʾ...
//1BYTE RS232,ͨ������O��11���α�ʾ...
//1BYTE ETH  0=OK,1=ERR,0xff=û����
//1BYTE GPRSģ��,0=OK,1=ERR,0xff=û����
//1BYTE �����ز�ģ��A��,0=OK,1=ERR,0xff=û����
//1BYTE �����ز�ģ��B��,0=OK,1=ERR,0xff=û����
//1BYTE �����ز�ģ��C��,0=OK,1=ERR,0xff=û����
#define OFFSET_ESAM_Characteristics     OFFSET_UART_Characteristics+8
//4BYTE TESAM���ͨ������Hz
//8BYTE TESAM���к�
//4BYTE MESAM���ͨ������Hz
//8BYTE MESAM���к�
#define OFFSET_24AA02E48_Characteristics     OFFSET_ESAM_Characteristics+24
//1BYTE 0=OK,1=ERR,0xff=û����
//6BYTE MAC��ַ
#define OFFSET_KEY_Characteristics     OFFSET_24AA02E48_Characteristics+7
//1BYTE ����0=��ȷ,����ֵûͨ��
#define OFFSET_TestResult_Characteristics     OFFSET_KEY_Characteristics+1
//1BYTE �ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
#define OFFSET_SWIN_Characteristics     OFFSET_TestResult_Characteristics+1
//1BYTE ����������1 0=OK,1=ERR,0xff=û����
//1BYTE ����������2 0=OK,1=ERR,0xff=û����
//1BYTE ����������3 0=OK,1=ERR,0xff=û����
//1BYTE ����������4 0=OK,1=ERR,0xff=û����
//1BYTE ����������5 0=OK,1=ERR,0xff=û����
//1BYTE ����������6 0=OK,1=ERR,0xff=û����
//1BYTE ����������7 0=OK,1=ERR,0xff=û����
//1BYTE ����������8 0=OK,1=ERR,0xff=û����
#define LEN_BASE_Characteristics     (OFFSET_SWIN_Characteristics+8)

#define ADDR_Characteristics_BASE2    ADDR_Characteristics_BASE1+LEN_BASE_Characteristics//���¶����ֵĲ��Ժ�:1=����(T<-5)

#define ADDR_Characteristics_BASE3    ADDR_Characteristics_BASE2+LEN_BASE_Characteristics//���¶����ֵĲ��Ժ�:2=���£�T>45)
//�������ܲ��Լ�¼����-----------------------------------------------------------------------------------




#define ADDR_ICERROR_IEEPROM_End     (ADDR_Characteristics_End)
//Ӳ�����Լ�¼����--------------------------------------------------------------------------------



#define ADDR_IEEPROM_End     (ADDR_ICERROR_IEEPROM_End)


#if ((ADDR_IEEPROM_End-ADDR_NANDFLASH_BLOCK_REPLACE)>0x4000)
#error ADDR_ICERROR_IEEPROM Over
#endif








#endif//#ifndef IEEPROM_H





