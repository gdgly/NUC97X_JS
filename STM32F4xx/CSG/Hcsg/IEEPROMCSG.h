
//EEPROM
#ifndef IEEPROM_H
#define IEEPROM_H




//---У�����------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
#define ADDR_MeterCalibration_IEEPROM_START     0x08008000//16k
#endif

#ifdef IC_NUC9xx
#define ADDR_SPIFLASH_START     0x10000000
//0x10000000-0x102FFFFF 3MB���������
//0x10300000-0x103BFFFF 768K MS,OS,Router����ʱ���ݻ����ش����ݴ�
//0x103C0000-0x103FFFFF 256K IEEPROM(��DDR2ӳ��)
#define ADDR_UPDATEBACK     0x10300000//MS,OS,Router����ʱ���ݻ����ش����ݴ�
#define LEN_UPDATEBACK     (768*1024)
#define ADDR_IEEPROM_START     (ADDR_SPIFLASH_START+0x003C0000)
#define ADDR_MeterCalibration_IEEPROM_START     ADDR_IEEPROM_START//256k
#endif

#if IC_90E36==1
//4BYTE CRC32
//4BYTE У�������Ч��־�̶�Ϊ0x12345678
//2BYTE RTCCAL
//2BYTE �¶�CAL
#define ADDR_ADC4MA     ADDR_MeterCalibration_IEEPROM_START+12
//2BYTE ֱ��ģ����������4MA����ʱADCת����׼ֵ
#define ADDR_ADC20MA     ADDR_MeterCalibration_IEEPROM_START+14
//2BYTE ֱ��ģ����������20MA����ʱADCת����׼ֵ
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
#define ADDR_EMU_41     ADDR_MeterCalibration_IEEPROM_START+8+16
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
#define LEN_MeterCalibration_IEEPROM     (2*128)//У������ܳ���=128*2�ֽ�
#if ((ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START)>LEN_MeterCalibration_IEEPROM)
#error ADDR_MeterCalibration_IEEPROM_End Over
#endif
#endif//#if IC_90E36==1


#if IC_ATT7022==1
//4BYTE CRC32
//4BYTE У�������Ч��־�̶�Ϊ0x12345678
//2BYTE RTCCAL
//2BYTE �¶�CAL(ATT7022���¶�У��ʱ�ĵ�ǰ�¶�ֵxx��(hex,�������¶�ֵ)
#define ADDR_ADC4MA     ADDR_MeterCalibration_IEEPROM_START+12
//2BYTE ֱ��ģ����������4MA����ʱADCת����׼ֵ
#define ADDR_ADC20MA     ADDR_MeterCalibration_IEEPROM_START+14
//2BYTE ֱ��ģ����������20MA����ʱADCת����׼ֵ
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
#define ADDR_EMU_01     ADDR_MeterCalibration_IEEPROM_START+8+16
//ģʽ���üĴ�������ַ��0x01��(��λ:0x89aa)
#define ADDR_EMU_02     ADDR_EMU_01+2
//ADC�������üĴ�������ַ��0x02��(��λ:0x0000)
#define ADDR_EMU_03     ADDR_EMU_02+2
//EMU��Ԫ���ã���ַ��0x03��(��λ:0x0804)
#define ADDR_EMU_04     ADDR_EMU_03+2
//�ڹ�������cos(��)=1ʱ���й�������У��
#define ADDR_EMU_05     ADDR_EMU_04+2
//05H w_PgainB 2 0x0000 B���й���������
#define ADDR_EMU_06     ADDR_EMU_05+2
//06H w_PgainC 2 0x0000 C���й���������
#define ADDR_EMU_07     ADDR_EMU_06+2
//07H w_QgainA 2 0x0000 A���޹���������
#define ADDR_EMU_08     ADDR_EMU_07+2
//08H w_QgainB 2 0x0000 B���޹���������
#define ADDR_EMU_09     ADDR_EMU_08+2
//09H w_QgainC 2 0x0000 C���޹���������
#define ADDR_EMU_0A     ADDR_EMU_09+2
//0AH w_SgainA 2 0x0000 A�����ڹ�������
#define ADDR_EMU_0B     ADDR_EMU_0A+2
//0BH w_SgainB 2 0x0000 B�����ڹ�������
#define ADDR_EMU_0C     ADDR_EMU_0B+2
//0CH w_SgainC 2 0x0000 C�����ڹ�������
#define ADDR_EMU_0D     ADDR_EMU_0C+2
//�ڹ�������cos(��)=1ʱ�����������Ѿ�У����֮���ٽ�����λ��������λУ����cos(��)=0.5Lʱ����У����
//0DH w_PhSregApq0 2 0x0000 A����λУ��0
#define ADDR_EMU_0E     ADDR_EMU_0D+2
//0EH w_PhSregBpq0 2 0x0000 B����λУ��0
#define ADDR_EMU_0F     ADDR_EMU_0E+2
//0FH w_PhSregCpq0 2 0x0000 C����λУ��0
#define ADDR_EMU_10     ADDR_EMU_0F+2
//10H w_PhSregApq1 2 0x0000 A����λУ��1
#define ADDR_EMU_11     ADDR_EMU_10+2
//11H w_PhSregBpq1 2 0x0000 B����λУ��1
#define ADDR_EMU_12     ADDR_EMU_11+2
//12H w_PhSregCpq1 2 0x0000 C����λУ��1
#define ADDR_EMU_13     ADDR_EMU_12+2
//�ڹ�������У������λУ���󣬽��й���offsetУ��������С�ź�x%Ib��5%����2%����ĵ�����ΪErr%
//13H w_PoffsetA 2 0x0000 A���й�����offsetУ��
#define ADDR_EMU_14     ADDR_EMU_13+2
//14H w_PoffsetB 2 0x0000 B���й�����offsetУ��
#define ADDR_EMU_15     ADDR_EMU_14+2
//15H w_PoffsetC 2 0x0000 C���й�����offsetУ��
#define ADDR_EMU_16     ADDR_EMU_15+2
//�����޹���λУ��
//16H w_QPhscal 2 0x0000 �����޹���λУ��
#define ADDR_EMU_17     ADDR_EMU_16+2
//17H w_UgainA 2 0x0000 A���ѹ����
#define ADDR_EMU_18     ADDR_EMU_17+2
//18H w_UgainB 2 0x0000 B���ѹ����
#define ADDR_EMU_19     ADDR_EMU_18+2
//19H w_UgainC 2 0x0000 C���ѹ����
#define ADDR_EMU_1A     ADDR_EMU_19+2
//1AH w_IgainA 2 0x0000 A���������
#define ADDR_EMU_1B     ADDR_EMU_1A+2
//1BH w_IgainB 2 0x0000 B���������
#define ADDR_EMU_1C     ADDR_EMU_1B+2
//1CH w_IgainC 2 0x0000 C���������
#define ADDR_EMU_1D     ADDR_EMU_1C+2
//�𶯵�����ֵ����
#define ADDR_EMU_1E     ADDR_EMU_1D+2
//����HFconst����У���õĸ�Ƶ�������CF��Ƶ�ʣ�HFconst����д�����0x000D00����С��0x000002�Ĳ���ֵ��
//������������ź�(������)��Чֵ��0.5V�����齫��ѹͨ��Un ��Ӧ��ADC ������ѡ����Чֵ0.22V���ң�������ͨ��Ib ʱ��ADC����ѡ����Чֵ0.05V ����
#define ADDR_EMU_1F     ADDR_EMU_1E+2
//ʧѹ��ֵ�Ǹ���У����ĵ�ѹ��Чֵ�����趨�ġ�
#define ADDR_EMU_20     ADDR_EMU_1F+2
//20H w_GainADC7 2 0x0000 ����·ADC�����ź�����
#define ADDR_EMU_21     ADDR_EMU_20+2
//21H w_QoffsetA 2 0x0000 A���޹�����offsetУ��
#define ADDR_EMU_22     ADDR_EMU_21+2
//22H w_QoffsetB 2 0x0000 B���޹�����offsetУ��
#define ADDR_EMU_23     ADDR_EMU_22+2
//23H w_QoffsetC 2 0x0000 C���޹�����offsetУ��
#define ADDR_EMU_24     ADDR_EMU_23+2
//24H w_UaRmsoffse 2 0x0000 A���ѹ��ЧֵoffsetУ��
#define ADDR_EMU_25     ADDR_EMU_24+2
//25H w_UbRmsoffse 2 0x0000 B���ѹ��ЧֵoffsetУ��
#define ADDR_EMU_26     ADDR_EMU_25+2
//26H w_UcRmsoffse 2 0x0000 C���ѹ��ЧֵoffsetУ��
#define ADDR_EMU_27     ADDR_EMU_26+2
//27H w_IaRmsoffse 2 0x0000 A�������ЧֵoffsetУ��
#define ADDR_EMU_28     ADDR_EMU_27+2
//28H w_IbRmsoffse 2 0x0000 B�������ЧֵoffsetУ��
#define ADDR_EMU_29     ADDR_EMU_28+2
//29H w_IcRmsoffse 2 0x0000 C�������ЧֵoffsetУ��
#define ADDR_EMU_2A     ADDR_EMU_29+2
//ע�⣺ADCʵʱ��������Ϊ19λ���Ҹ�λ������λ����ADC offset�Ĵ���Ϊ16λ����ADC offset��ADC��������19λ�еĸ�16λ����
//2AH w_UoffsetA 2 0x0000 A���ѹͨ��ADC offsetУ��
#define ADDR_EMU_2B     ADDR_EMU_2A+2
//2BH w_UoffsetB 2 0x0000 B���ѹͨ��ADC offsetУ��
#define ADDR_EMU_2C     ADDR_EMU_2B+2
//2CH w_UoffsetC 2 0x0000 C���ѹͨ��ADC offsetУ��
#define ADDR_EMU_2D     ADDR_EMU_2C+2
//2DH w_IoffsetA 2 0x0000 A�����ͨ��ADC offsetУ��
#define ADDR_EMU_2E     ADDR_EMU_2D+2
//2EH w_IoffsetB 2 0x0000 B�����ͨ��ADC offsetУ��
#define ADDR_EMU_2F     ADDR_EMU_2E+2
//2FH w_IoffsetC 2 0x0000 C�����ͨ��ADC offsetУ��
#define ADDR_EMU_30     ADDR_EMU_2F+2
//30H w_EMUIE 2 0x0001 �ж�ʹ��
#define ADDR_EMU_31     ADDR_EMU_30+2
//ģ��ģ��ʹ�ܼĴ���(��λ0x4527)
#define ADDR_EMU_32     ADDR_EMU_31+2
//32H w_AllGain 2 0x0000 ȫͨ�����棬����Vref���¶�У��
#define ADDR_EMU_33     ADDR_EMU_32+2
//33H w_HFDouble 2 0x0000 ���峣���ӱ�ѡ��
#define ADDR_EMU_34     ADDR_EMU_33+2
//�Ի���������в������ڻ�������Ĵ���LineGain(0x34)=0x2C59(��λֵ)����½���
//34H w_LineGain 2 0x2C59 ��������У��
#define ADDR_EMU_35     ADDR_EMU_34+2
//IO״̬���üĴ���(��ַ��0x35)(��λ0x080f)
//35H w_PinCtrl 2 0x000F ����pin����������ѡ�����
#define ADDR_EMU_36     ADDR_EMU_35+2
//�𶯹��ʼĴ��� (��ַ��0x36)
#define ADDR_EMU_37     ADDR_EMU_36+2
//37 w_Iregion 2 0x7FFF ��λ�����������üĴ���
#define ADDR_EMU_38     ADDR_EMU_37+2
//SAG���ݳ������üĴ��� 0x1000
#define ADDR_EMU_39     ADDR_EMU_38+2
//SAG�����ֵ���üĴ��� 0x4500

#define ADDR_EMU_60     ADDR_EMU_39+2
//60 w_Iregion1 2 0x0000 ��λ�����������üĴ���1
#define ADDR_EMU_61     ADDR_EMU_60+2
//61 w_PhSregApq2 2 0x0000 A����λУ��2
#define ADDR_EMU_62     ADDR_EMU_61+2
//62 w_PhSregBpq2 2 0x0000 B����λУ��2
#define ADDR_EMU_63     ADDR_EMU_62+2
//63 w_PhSregCpq2 2 0x0000 C����λУ��2
#define ADDR_EMU_64     ADDR_EMU_63+2
//64 w_PoffsetAL 2 0x0000 A���й�����offsetУ�����ֽ�
#define ADDR_EMU_65     ADDR_EMU_64+2
//65 w_PoffsetBL 2 0x0000 B���й�����offsetУ�����ֽ�
#define ADDR_EMU_66     ADDR_EMU_65+2
//66 w_PoffsetCL 2 0x0000 C���й�����offsetУ�����ֽ�
#define ADDR_EMU_67     ADDR_EMU_66+2
//67 w_QoffsetAL 2 0x0000 A���޹�����offsetУ�����ֽ�
#define ADDR_EMU_68     ADDR_EMU_67+2
//68 w_QoffsetBL 2 0x0000 B���޹�����offsetУ�����ֽ�
#define ADDR_EMU_69     ADDR_EMU_68+2
//69 w_QoffsetCL 2 0x0000 C���޹�����offsetУ�����ֽ�
#define ADDR_EMU_6A     ADDR_EMU_69+2
//6A w_ItRmsoffset 2 0x0000 ����ʸ����offsetУ���Ĵ���
#define ADDR_EMU_6B     ADDR_EMU_6A+2
//6B w_TPSoffset 2 0x0000 TPS��ֵУ���Ĵ���
#define ADDR_EMU_6C     ADDR_EMU_6B+2
//6C w_TPSgain 2 0x0000 TPSб��У���Ĵ���
#define ADDR_EMU_6D     ADDR_EMU_6C+2
//6D w_TCcoffA 2 0xFEFF Vrefgain�Ķ���ϵ��
#define ADDR_EMU_6E     ADDR_EMU_6D+2
//6E w_TCcoffB 2 0xEF7A Vrefgain��һ��ϵ��
#define ADDR_EMU_6F     ADDR_EMU_6E+2
//6F w_TCcoffC 2 0x047C Vrefgain�ĳ�����
#define ADDR_EMU_70     ADDR_EMU_6F+2
//70 w_EMCfg 2 0x0000 �����㷨���ƼĴ���
#define ADDR_EMU_71     ADDR_EMU_70+2
//71 w_OILVL 2 0x0000 ������ֵ���üĴ���


#define ADDR_MeterCalibration_IEEPROM_End     (ADDR_EMU_71+2)
#define LEN_MeterCalibration_IEEPROM     (2*128)//У������ܳ���=128*2�ֽ�
#if ((ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START)>LEN_MeterCalibration_IEEPROM)
#error ADDR_MeterCalibration_IEEPROM_End Over
#endif
#endif//#if IC_ATT7022==1

//У���������--------------------------------------------------------------------------------


//���ܱ����춨��¼����---------------------------------------------------------
#define ADDR_MeterError_IEEPROM_Start     (ADDR_MeterCalibration_IEEPROM_START+LEN_MeterCalibration_IEEPROM)
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


#if ((ADDR_MeterError_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START)>0x4000)
#error ADDR_MeterError_IEEPROM Over
#endif



//NANDFLASH���滻----------------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
#define ADDR_NANDFLASH_BLOCK_REPLACE     0x0800C000//16k   NANDFLASH���滻
#endif
#ifdef IC_NUC9xx
#define ADDR_NANDFLASH_BLOCK_REPLACE     (ADDR_IEEPROM_START+0x00004000)//16k   NANDFLASH���滻
#endif
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
//NANDFLASH���滻����--------------------------------------------------------------------------------------------------



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




#define ADDR_ICERROR_IEEPROM_End     (ADDR_Characteristics_BASE3+LEN_BASE_Characteristics)
//Ӳ�����Լ�¼����--------------------------------------------------------------------------------



#define ADDR_IEEPROM_End     (ADDR_ICERROR_IEEPROM_End)


#if ((ADDR_IEEPROM_End-ADDR_NANDFLASH_BLOCK_REPLACE)>0x4000)
#error ADDR_ICERROR_IEEPROM Over
#endif








#endif//#ifndef IEEPROM_H





