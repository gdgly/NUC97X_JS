
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif

#include "../../STM32F4xx/Device/ATM90E36.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"




#if IC_90E36==1//0=û,1=��

#if MeterSpec==20//�Զ���Ӧ���ܱ���
u32 Get_MeterSpec(void)//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
{
	u32 i;
	u32 Umax;
	
	Umax=MRR(ADDR_UA+2,2);
	i=MRR(ADDR_UB+2,2);
	if(i>Umax)
	{
		Umax=i;
	}
	i=MRR(ADDR_UC+2,2);
	if(i>Umax)
	{
		Umax=i;
	}
	if(Umax>0x1300)//130.0V
	{
		Meter_Ram->PW=0;//��ʼ���߷�ʽ;0=��������,1=��������
		return 0;//0=220V,1=100V,2=57.7V
	}
	if(Umax>0x0750)//75.0V
	{
		Meter_Ram->PW=1;//��ʼ���߷�ʽ;0=��������,1=��������
		return 1;//0=220V,1=100V,2=57.7V
	}
	Meter_Ram->PW=0;//��ʼ���߷�ʽ;0=��������,1=��������
	return 2;//0=220V,1=100V,2=57.7V
}
#endif

#if MeterIMaxhex<=2000//������xx.xx(A)
__asm void MeterCalInitLib(void)//У�������ʼ����(IGAIN=0)
{
//���У�����------------------------------------------------------------------------------------
//#define ADDR_MeterCalibration_IEEPROM_Start     (ADDR_IEEPROM_Start)
	dcdu 0//4BYTE CRC32
	dcdu 0x12345678//4BYTE ���У�������Ч��־�̶�Ϊ0x12345678
	dcwu 0//2BYTE RTCCAL
	dcwu 0//2BYTE �¶�CAL
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
//#define ADDR_EMU_41     ADDR_MeterCalibration_IEEPROM_Start+8+16
	dcwu 0//2BYTE Phase A Active Power Offset
//#define ADDR_EMU_42     ADDR_EMU_41+2
	dcwu 0//2BYTE Phase A Reactive Power Offset
//#define ADDR_EMU_43     ADDR_EMU_42+2
	dcwu 0//2BYTE Phase B Active Power Offset
//#define ADDR_EMU_44     ADDR_EMU_43+2
	dcwu 0//2BYTE Phase B Reactive Power Offset
//#define ADDR_EMU_45     ADDR_EMU_44+2
	dcwu 0//2BYTE Phase C Active Power Offset
//#define ADDR_EMU_46     ADDR_EMU_45+2
	dcwu 0//2BYTE Phase C Reactive Power Offset
//#define ADDR_EMU_47     ADDR_EMU_46+2
	dcwu 0//2BYTE Phase A Active/Reactive Energy calibration gain
//#define ADDR_EMU_48     ADDR_EMU_47+2
	dcwu 0//2BYTE Phase A calibration phase angle
//#define ADDR_EMU_49     ADDR_EMU_48+2
	dcwu 0//2BYTE Phase B Active/Reactive Energy calibration gain
//#define ADDR_EMU_4A     ADDR_EMU_49+2
	dcwu 0//2BYTE Phase B calibration phase angle
//#define ADDR_EMU_4B     ADDR_EMU_4A+2
	dcwu 0//2BYTE Phase C Active/Reactive Energy calibration gain
//#define ADDR_EMU_4C     ADDR_EMU_4B+2
	dcwu 0//2BYTE Phase C calibration phase angle
//#define ADDR_EMU_51     ADDR_EMU_4C+2
	dcwu 0//2BYTE Phase A Fundamental Active Power Offset
//#define ADDR_EMU_52     ADDR_EMU_51+2
	dcwu 0//2BYTE Phase B Fundamental Active Power Offset
//#define ADDR_EMU_53     ADDR_EMU_52+2
	dcwu 0//2BYTE Phase C Fundamental Active Power Offset
//#define ADDR_EMU_54     ADDR_EMU_53+2
	dcwu 0//2BYTE Phase A Fundamental Active Power Gain
//#define ADDR_EMU_55     ADDR_EMU_54+2
	dcwu 0//2BYTE Phase B Fundamental Active Power Gain
//#define ADDR_EMU_56     ADDR_EMU_55+2
	dcwu 0//2BYTE Phase C Fundamental Active Power Gain
//#define ADDR_EMU_61     ADDR_EMU_56+2
	dcwu 0xCE40//2BYTE Phase A Voltage RMS Gain
//#define ADDR_EMU_62     ADDR_EMU_61+2
	dcwu 0x7530//2BYTE Phase A Current RMS Gain
//#define ADDR_EMU_63     ADDR_EMU_62+2
	dcwu 0//2BYTE Phase A Voltage RMS Offset
//#define ADDR_EMU_64     ADDR_EMU_63+2
	dcwu 0//2BYTE Phase A Current RMS Offset
//#define ADDR_EMU_65     ADDR_EMU_64+2
	dcwu 0xCE40//2BYTE Phase B Voltage RMS Gain
//#define ADDR_EMU_66     ADDR_EMU_65+2
	dcwu 0x7530//2BYTE Phase B Current RMS Gain
//#define ADDR_EMU_67     ADDR_EMU_66+2
	dcwu 0//2BYTE Phase B Voltage RMS Offset
//#define ADDR_EMU_68     ADDR_EMU_67+2
	dcwu 0//2BYTE Phase B Current RMS Offset
//#define ADDR_EMU_69     ADDR_EMU_68+2
	dcwu 0xCE40//2BYTE Phase C Voltage RMS Gain
//#define ADDR_EMU_6A     ADDR_EMU_69+2
	dcwu 0x7530//2BYTE Phase C Current RMS Gain
//#define ADDR_EMU_6B     ADDR_EMU_6A+2
	dcwu 0//2BYTE Phase C Voltage RMS Offset
//#define ADDR_EMU_6C     ADDR_EMU_6B+2
	dcwu 0//2BYTE Phase C Current RMS Offset
//#define ADDR_EMU_6D     ADDR_EMU_6C+2
	dcwu 0x7530//2BYTE Sampled N line Current RMS Gain
//#define ADDR_EMU_6E     ADDR_EMU_6D+2
	dcwu 0//2BYTE Sampled N line Current RMS Offset
//#define ADDR_MeterCalibration_IEEPROM_End     (ADDR_EMU_6E+2)
}
#else//#if MeterIMaxhex<=2000//������xx.xx(A)
__asm void MeterCalInitLib(void)//У�������ʼ����(IGAIN=0)
{
//���У�����------------------------------------------------------------------------------------
//#define ADDR_MeterCalibration_IEEPROM_Start     (ADDR_IEEPROM_Start)
	dcdu 0//4BYTE CRC32
	dcdu 0x12345678//4BYTE ���У�������Ч��־�̶�Ϊ0x12345678
	dcwu 0//2BYTE RTCCAL
	dcwu 0//2BYTE �¶�CAL
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
	dcwu 0//2BYTE reserved
//#define ADDR_EMU_41     ADDR_MeterCalibration_IEEPROM_Start+8+16
	dcwu 0//2BYTE Phase A Active Power Offset
//#define ADDR_EMU_42     ADDR_EMU_41+2
	dcwu 0//2BYTE Phase A Reactive Power Offset
//#define ADDR_EMU_43     ADDR_EMU_42+2
	dcwu 0//2BYTE Phase B Active Power Offset
//#define ADDR_EMU_44     ADDR_EMU_43+2
	dcwu 0//2BYTE Phase B Reactive Power Offset
//#define ADDR_EMU_45     ADDR_EMU_44+2
	dcwu 0//2BYTE Phase C Active Power Offset
//#define ADDR_EMU_46     ADDR_EMU_45+2
	dcwu 0//2BYTE Phase C Reactive Power Offset
//#define ADDR_EMU_47     ADDR_EMU_46+2
	dcwu 0//2BYTE Phase A Active/Reactive Energy calibration gain
//#define ADDR_EMU_48     ADDR_EMU_47+2
	dcwu 0//2BYTE Phase A calibration phase angle
//#define ADDR_EMU_49     ADDR_EMU_48+2
	dcwu 0//2BYTE Phase B Active/Reactive Energy calibration gain
//#define ADDR_EMU_4A     ADDR_EMU_49+2
	dcwu 0//2BYTE Phase B calibration phase angle
//#define ADDR_EMU_4B     ADDR_EMU_4A+2
	dcwu 0//2BYTE Phase C Active/Reactive Energy calibration gain
//#define ADDR_EMU_4C     ADDR_EMU_4B+2
	dcwu 0//2BYTE Phase C calibration phase angle
//#define ADDR_EMU_51     ADDR_EMU_4C+2
	dcwu 0//2BYTE Phase A Fundamental Active Power Offset
//#define ADDR_EMU_52     ADDR_EMU_51+2
	dcwu 0//2BYTE Phase B Fundamental Active Power Offset
//#define ADDR_EMU_53     ADDR_EMU_52+2
	dcwu 0//2BYTE Phase C Fundamental Active Power Offset
//#define ADDR_EMU_54     ADDR_EMU_53+2
	dcwu 0//2BYTE Phase A Fundamental Active Power Gain
//#define ADDR_EMU_55     ADDR_EMU_54+2
	dcwu 0//2BYTE Phase B Fundamental Active Power Gain
//#define ADDR_EMU_56     ADDR_EMU_55+2
	dcwu 0//2BYTE Phase C Fundamental Active Power Gain
//#define ADDR_EMU_61     ADDR_EMU_56+2
	dcwu 0xB60F//2BYTE Phase A Voltage RMS Gain
//#define ADDR_EMU_62     ADDR_EMU_61+2
	dcwu 0x5AE0//2BYTE Phase A Current RMS Gain
//#define ADDR_EMU_63     ADDR_EMU_62+2
	dcwu 0//2BYTE Phase A Voltage RMS Offset
//#define ADDR_EMU_64     ADDR_EMU_63+2
	dcwu 0//2BYTE Phase A Current RMS Offset
//#define ADDR_EMU_65     ADDR_EMU_64+2
	dcwu 0xB60F//2BYTE Phase B Voltage RMS Gain
//#define ADDR_EMU_66     ADDR_EMU_65+2
	dcwu 0x5AE0//2BYTE Phase B Current RMS Gain
//#define ADDR_EMU_67     ADDR_EMU_66+2
	dcwu 0//2BYTE Phase B Voltage RMS Offset
//#define ADDR_EMU_68     ADDR_EMU_67+2
	dcwu 0//2BYTE Phase B Current RMS Offset
//#define ADDR_EMU_69     ADDR_EMU_68+2
	dcwu 0xB60F//2BYTE Phase C Voltage RMS Gain
//#define ADDR_EMU_6A     ADDR_EMU_69+2
	dcwu 0x5AE0//2BYTE Phase C Current RMS Gain
//#define ADDR_EMU_6B     ADDR_EMU_6A+2
	dcwu 0//2BYTE Phase C Voltage RMS Offset
//#define ADDR_EMU_6C     ADDR_EMU_6B+2
	dcwu 0//2BYTE Phase C Current RMS Offset
//#define ADDR_EMU_6D     ADDR_EMU_6C+2
	dcwu 0x5AE0//2BYTE Sampled N line Current RMS Gain
//#define ADDR_EMU_6E     ADDR_EMU_6D+2
	dcwu 0//2BYTE Sampled N line Current RMS Offset
//#define ADDR_MeterCalibration_IEEPROM_End     (ADDR_EMU_6E+2)
}
#endif//#if MeterIMaxhex<=2000//������xx.xx(A)




typedef __packed struct
{
  u16 Addr;//ADI�Ĵ�����ַ
	u8 RW;//��д:1=��,4=дCS,0x81=д����
	u8 Phase;//����������:0=����,1=A��,2=B��,3=C��,4=����
  u32 Data;//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
}EMU_Adj_List_TypeDef;
__align(4) const EMU_Adj_List_TypeDef   EMU_Adj_List[]=
{
//6.2 SPECIAL REGISTERS
//6.2.1 SOFT RESET REGISTER
//6.2.2 IRQ AND WARNOUT SIGNAL GENERATION
  //System Status 0
  //System Status 1
  //Function Enable 0
  //Function Enable 1
//6.2.3 SPECIAL CONFIGURATION REGISTERS
  //Zero-Crossing Configuration
  //Voltage Sag Threshold
  //Voltage Phase Losing Threshold
  //Neutral Current (Calculated) Warning Threshold
  //Neutral Current (Sampled) Warning Threshold
  //Voltage THD Warning Threshold
  //Current THD Warning Threshold
  //DMA Mode Interface Control
//6.2.4 LAST SPI DATA REGISTER
//6.3 LOW-POWER MODES REGISTERS
//6.3.1 DETECTION MODE REGISTERS
  //Current Detect Control
  //{0x10,0x07},
  //Phase A Current Threshold in Detection Mode
  //{0x11,0x00},
  //Phase B Current Threshold in Detection Mode
  //{0x12,0x00},
  //Phase C Current Threshold in Detection Mode
  //{0x13,0x00},
//6.3.2 PARTIAL MEASUREMENT MODE REGISTERS
  //Ioffset for phase A in Partial Measurement mode
  //{0x14,0x00},
  //Ioffset for phase B in Partial Measurement mode
  //{0x15,0x00},
  //Ioffset for phase C in Partial Measurement mode
  //{0x16,0x00},
  //PGAgain Configuration in Partial Measurement mode
  //{0x17,0x00},
  //Irms for phase A in Partial Measurement mode
  //Irms for phase B in Partial Measurement mode
  //Irms for phase C in Partial Measurement mode
  //Measure Configuration in Partial Measurement mode
  //{0x1B,0x00},
  //Number of 8K Samples to be Averaged
  //LSB bits of PMRrms[A/B/C]
//6.4 CONFIGURATION AND CALIBRATION REGISTERS
//6.4.2 CONFIGURATION REGISTERS
  //Configure Start Command
	{
  	0x30,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x8765,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //High Word of PL_Constant
	{
  	0x31,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
	#if MeterIMaxhex<=2000//������xx.xx(A)
  	(450000000000/MeterActiveConst)>>16,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#else
  	(450000000000/(MeterActiveConst*2))>>16,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#endif
	},
  //Low Word of PL_Constant
	{
  	0x32,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
	#if MeterIMaxhex<=2000//������xx.xx(A)
  	(450000000000/MeterActiveConst)&0xffff,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#else
  	(450000000000/(MeterActiveConst*2))&0xffff,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#endif
	},
  //Metering method configuration
	{
  	0x33,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
	#if MeterSpec==20//�Զ���Ӧ���ܱ���
		0x287+(0<<8)+(MeterFreq<<12),//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#else
  	0x287+(MeterPW<<8)+(MeterFreq<<12),//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#endif
	},
  //PGA Gain Configuration
	{
  	0x34,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Active Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 Watt (DSP bit 27 aligned)
	{
  	0x35,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
	#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
  	(MeterPStartupHex*2)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#else
  	(MeterPStartupHex)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#endif
	},
  //Reactive Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 var
	{
  	0x36,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
	#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
  	(MeterQStartupHex*2)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#else
  	(MeterQStartupHex)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#endif
	},
  //Apparent Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 VA
	{
  	0x37,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
	#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
  	(MeterPStartupHex*2)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#else
  	(MeterPStartupHex)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	#endif
	},
  //Startup power threshold (for |P|+|Q| of a phase) for any phase participating Active Energy Accumulation.Common for phase A/B/C.16 bit unsigned integer,Unit: 0.00032 Watt/var (DSP bit 23 aligned)
	{
  	0x38,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	MeterPStartupHex/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Startup power threshold (for |P|+|Q| of a phase) for any phase participating ReActive Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
	{
  	0x39,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	MeterQStartupHex/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Startup power threshold (for |P|+|Q| of a phase) for any phase participating Apparent Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
	{
  	0x3a,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	MeterPStartupHex/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS0-R
	{
  	0x3b,//ADI�Ĵ�����ַ
		1,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS0-W
	{
  	0x3b,//ADI�Ĵ�����ַ
		4,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	
//6.4.3 ENERGY CALIBRATION REGISTERS
  //Calibration Start Command
	{
  	0x40,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x8765,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Active Power Offset
	{
  	0x41,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_41,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Reactive Power Offset
	{
  	0x42,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_42,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Active Power Offset
	{
  	0x43,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_43,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Reactive Power Offset
	{
  	0x44,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_44,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Active Power Offset
	{
  	0x45,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_45,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Reactive Power Offset
	{
  	0x46,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_46,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Active/Reactive Energy calibration gain
	{
  	0x47,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_47,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A calibration phase angle
	{
  	0x48,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_48,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Active/Reactive Energy calibration gain
	{
  	0x49,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_49,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B calibration phase angle
	{
  	0x4A,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_4A,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Active/Reactive Energy calibration gain
	{
  	0x4B,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_4B,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C calibration phase angle
	{
  	0x4C,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_4C,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS-R
	{
  	0x4D,//ADI�Ĵ�����ַ
		1,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS-W
	{
  	0x4D,//ADI�Ĵ�����ַ
		4,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},

//6.4.4 FUNDAMENTAL/HARMONIC ENERGY CALIBRATION REGISTERS
  //Harmonic Calibration Startup Command
	{
  	0x50,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x8765,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Fundamental Active Power Offset
	{
  	0x51,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_51,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Fundamental Active Power Offset
	{
  	0x52,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_52,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Fundamental Active Power Offset
	{
  	0x53,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_53,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Fundamental Active Power Gain
	{
  	0x54,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_54,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Fundamental Active Power Gain
	{
  	0x55,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_55,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Fundamental Active Power Gain
	{
  	0x56,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_56,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS-R
	{
  	0x57,//ADI�Ĵ�����ַ
		1,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS-W
	{
  	0x57,//ADI�Ĵ�����ַ
		4,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},

//6.4.5 MEASUREMENT CALIBRATION
  //Measurement Calibration Startup Command
	{
  	0x60,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x8765,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Voltage RMS Gain
	{
  	0x61,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_61,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Current RMS Gain
	{
  	0x62,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_62,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Voltage RMS Offset
	{
  	0x63,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_63,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase A Current RMS Offset
	{
  	0x64,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		1,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_64,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Voltage RMS Gain
	{
  	0x65,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_65,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Current RMS Gain
	{
  	0x66,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_66,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Voltage RMS Offset
	{
  	0x67,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_67,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase B Current RMS Offset
	{
  	0x68,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		2,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_68,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Voltage RMS Gain
	{
  	0x69,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_69,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Current RMS Gain
	{
  	0x6A,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_6A,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Voltage RMS Offset
	{
  	0x6B,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_6B,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Phase C Current RMS Offset
	{
  	0x6C,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		3,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_6C,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Sampled N line Current RMS Gain
	{
  	0x6D,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		4,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_6D,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
  //Sampled N line Current RMS Offset
	{
  	0x6E,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		4,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	ADDR_EMU_6E,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS-R
	{
  	0x6F,//ADI�Ĵ�����ַ
		1,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//CS-W
	{
  	0x6F,//ADI�Ĵ�����ַ
		4,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},

//7.1Ƭ���¶ȴ�����������
	{
  	0x02FD,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0xAA55,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	{
  	0x0216,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x5122,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	{
  	0x0219,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x012B,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	{
  	0x02FD,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x0000,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
//7.2 ����A/D ����ͨ�����¶Ȳ���
	//��2FCH ��ַд��AA55H
	{
  	0x02FC,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0xAA55,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//��270H ��ַд����Ҫ�������¶�ϵ��(�¶Ȳ���ϵ����bit7 Ϊ����λ����λppm/ ��)
	{
  	0x0270,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//��27BH ��ַд���¶Ȳ����Ļ�׼���¶�
	{
  	0x027B,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x8000+25,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	//��2FCH ��ַд��0000H
	{
  	0x02FC,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x0000,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},
	
//8 г������
	//����DFT����
	{
  	0x01D1,//ADI�Ĵ�����ַ
		0x81,//��д:1=��,4=дCS,0x81=д����
		0,//����������:0=����,1=A��,2=B��,3=C��,4=����
  	0x0001,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
	},

};



void Write_90E36(u32 Data)//д90E36
{
//	u32 i;
	u32* p32cs;
	u32 *p32ctrl;
	u32 *p32TxData;
	
	Init_SPI2();//SPI2��ʼ��(ATM90E36�� ATT7022)
	SoftDelayUS(1);
	p32cs=(u32*)(REG_SPI1_SSR);
	p32cs[0]=1;//cs=0
	p32TxData=(u32*)(REG_SPI1_TX0);
	p32TxData[0]=Data;
	p32ctrl=(u32*)(REG_SPI1_CNTRL);
	p32ctrl[0]=
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	(0<<2)|	//[2]Tx_NEG
	(0<<1)|//[1]Rx_NEG
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	p32cs[0]=0;//cs=1
}

u32 Read_90E36(u32 Addr)//��90E36
{
//	u32 i;
	u32* p32cs;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	Init_SPI2();//SPI2��ʼ��(ATM90E36�� ATT7022)
	SoftDelayUS(1);
	p32RxData=(u32*)(REG_SPI1_RX0);
	p32cs=(u32*)(REG_SPI1_SSR);
	p32cs[0]=1;//cs=0
	p32TxData=(u32*)(REG_SPI1_TX0);
	p32TxData[0]=Addr<<16;
	p32ctrl=(u32*)(REG_SPI1_CNTRL);
	p32ctrl[0]=
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(0<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
	(0<<2)|	//[2]Tx_NEG
	(0<<1)|//[1]Rx_NEG
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	Addr=p32RxData[0]&0xffff;
	p32cs[0]=0;//cs=1
	return Addr;
}


//90E36 BUG ��д�������5ms�����ٶ�0x0f�Ĵ�����飬��0x0f�Ĵ���������ֵΪ0��0xffff

//�ض��Ƚ�
void EMURWTask0(void)//��дEMU����0
{
  u32 i;

    switch(Meter_Ram->SSP1Task0)
    {    
      case 0x1://�ȴ���
        Meter_Ram->SSP1Task0=0x2;
        //break;
      case 0x2://����ʼ
				Meter_Ram->SSP1Data0=Read_90E36(Meter_Ram->SSP1Addr0|0x8000);//��90E36
        Meter_Ram->SSP1Task0=0x3;
        //break;
      case 0x3://�����
				i=Read_90E36(0x800F);//��90E36
        if(Meter_Ram->SSP1Data0!=i)
        {
					Meter_Ram->SSP1Data0=0xffff;
					Meter_Ram->SSP1Task0=0x5;//������
					Meter_Ram->SSP1ErrorCount++;//SSP1ͨ�Ŵ������+1
					if(Meter_Ram->SSP1ErrorCount>16)
					{//����16�δ�����Ϊ����ɣ�ʹ�ܶ��¸�����
						Meter_Ram->SSP1ErrorCount=0;//SSP1ͨ�Ŵ������
						Meter_Ram->SSP1Task0=0x4;//�����
					}
        }
        else
        {
					Meter_Ram->SSP1ErrorCount=0;//SSP1ͨ�Ŵ������
          Meter_Ram->SSP1Task0=0x4;//�����
        }
        break;
      case 0x4://�����
      case 0x5://������
        break;
        
      case 0x81://�ȴ�д
        Meter_Ram->SSP1Task0=0x82;
        //break;
      case 0x82://д��ʼ
				i=(Meter_Ram->SSP1Addr0<<16)|Meter_Ram->SSP1Data0;
				Write_90E36(i);//д90E36
        Meter_Ram->SSP1Task0=0x83;
        //break;
      case 0x83://д���
				i=Read_90E36(0x800F);//��90E36
        if(Meter_Ram->SSP1Data0!=i)
        {
          Meter_Ram->EMUInit=0;//����оƬ��ʼ����־:0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
          Comm_Ram->ErrorIC|=(1<<5);//Ӳ�������־(��λ):B5=EMU ����оƬ
					Comm_Ram->DisplayAllErrorTimer=10;//�ϵ�ȫ�Ի�ϵͳ������ʾ�붨ʱ��
          Meter_Ram->SSP1Task0=0x85;//д����
        }
        else
        {
          Meter_Ram->SSP1Task0=0x84;//д���
        }
        break;
      case 0x84://д���
      case 0x85://д����
        break;
      default:
        Meter_Ram->SSP1Task0=0;
        break;
    }  
}
void EMURWTask1(void)//��дEMU����1
{
  u32 i;

		switch(Meter_Ram->SSP1Task1)
    {    
      case 0x1://�ȴ���
        Meter_Ram->SSP1Task1=0x2;
        //break;
      case 0x2://����ʼ
				Meter_Ram->SSP1Data1=Read_90E36(Meter_Ram->SSP1Addr1|0x8000);//��90E36
        Meter_Ram->SSP1Task1=0x3;
        //break;
      case 0x3://�����
				i=Read_90E36(0x800F);//��90E36
        if(Meter_Ram->SSP1Data1!=i)
        {
					Meter_Ram->SSP1Data1=0xffff;
					Meter_Ram->SSP1Task1=0x5;//������
					Meter_Ram->SSP1ErrorCount++;//SSP1ͨ�Ŵ������+1
					if(Meter_Ram->SSP1ErrorCount>16)
					{//����16�δ�����Ϊ����ɣ�ʹ�ܶ��¸�����
						Meter_Ram->SSP1ErrorCount=0;//SSP1ͨ�Ŵ������
						Meter_Ram->SSP1Task1=0x4;//�����
					}
        }
        else
        {
					Meter_Ram->SSP1ErrorCount=0;//SSP1ͨ�Ŵ������
          Meter_Ram->SSP1Task1=0x4;//�����
        }
        break;
      case 0x4://�����
      case 0x5://������
        break;
        
      case 0x81://�ȴ�д
        Meter_Ram->SSP1Task1=0x82;
        //break;
      case 0x82://д��ʼ
				Write_90E36((Meter_Ram->SSP1Addr1<<16)|Meter_Ram->SSP1Data1);//д90E36
        Meter_Ram->SSP1Task1=0x83;
        //break;
      case 0x83://д���
				i=Read_90E36(0x800F);//��90E36
        if(Meter_Ram->SSP1Data1!=i)
        {
          Meter_Ram->SSP1Task1=0x85;//д����
        }
        else
        {
          Meter_Ram->SSP1Task1=0x84;//д���
        }
        break;
      case 0x84://д���
      case 0x85://д����
        break;
      default:
        Meter_Ram->SSP1Task1=0;
        break;
    }  
}



void EMURW(void)//��дEMU(�Զ�����0������1)
{
  u32 i;
	
	switch(Meter_Ram->EMUInit)
	{//����оƬ��ʼ����־:0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
		case 0://�ص�Դ(�ŵ�)
			OffEMU();//��EMU(��λ)
			OffA5V0();//��A5V0��Դ
			Comm_Ram->SPI2Timer=1000/10;//12 SSP1 ÿ10MS��1��ʱ��
			Meter_Ram->EMUInit=1;
			return;
		case 1://����Դ
			if(Comm_Ram->SPI2Timer==0)
			{
				OnA5V0();//��A5V0��Դ
				Comm_Ram->SPI2Timer=1000/10;//12 SSP1 ÿ10MS��1��ʱ��
				Meter_Ram->EMUInit=2;
			}
			return;
		case 2://��λ
			if(Comm_Ram->SPI2Timer==0)
			{
				OnEMU();//��EMU(��λ���)
				Comm_Ram->SPI2Timer=100/10;//12 SSP1 ÿ10MS��1��ʱ��
				Meter_Ram->EMUInit=3;
			}
			return;
		case 3://��λ���
			if(Comm_Ram->SPI2Timer==0)
			{
//				Init_SPI2();//SSP1��ʼ��(EMU)
				Meter_Ram->EMUParaCount=0;//����оƬ������ʼ������
				Meter_Ram->SSP1Task1Count=0;//SSP1����1�����������������
				Meter_Ram->SSP1Task0=0;//SSP1����0:0=��,0x1=�ȴ���,0x2=����ʼ,0x3=���Ƚ�,0x4=�����,0x5=������;0x81=�ȴ�д,0x82=д��ʼ,0x83=д�Ƚ�,0x84=д���,0x85=д����
				Meter_Ram->SSP1Task1=0;//SSP1����1:0=��,0x1=�ȴ���,0x2=����ʼ,0x3=���Ƚ�,0x4=�����,0x5=������;0x81=�ȴ�д,0x82=д��ʼ,0x83=д�Ƚ�,0x84=д���,0x85=д����
				Meter_Ram->EMUInit=4;
				Meter_Ram->TxCount=2;//
			}
			return;
		case 4://��ʼ��
		case 5://���
			break;
		default:
			Meter_Ram->EMUInit=0;
			return;
	}
	//if((SPI2->SR&1)==0)
	if(Meter_Ram->TxCount!=2)
  {//0�����ջ�����Ϊ��
    if(Comm_Ram->SPI2Timer!=0)
    {//��ʱû��
			return;
		}
		//SSP1����
    //Init_SPI2();//SSP1��ʼ��(EMU)
		i=Meter_Ram->SSP1Task0;
		i&=0x80;
		i|=1;
		Meter_Ram->SSP1Task0=i;
		i=Meter_Ram->SSP1Task1;
		i&=0x80;
		i|=1;
		Meter_Ram->SSP1Task1=i;
  }

	if(Meter_Ram->SSP1Sched==0)
	{
		EMURWTask0();//��дEMU����0
		switch(Meter_Ram->SSP1Task0)
		{
			case 0:
			case 0x4://�����
			case 0x5://������
			case 0x84://д���
			case 0x85://д����
				i=Meter_Ram->EMUParaCount;
				if(i>=(sizeof(EMU_Adj_List)/sizeof(EMU_Adj_List_TypeDef)))
				{
				#if MeterSpec==20//�Զ���Ӧ���ܱ���	
					//�ظ�д����
					Meter_Ram->EMUParaCount=0;
					Meter_Ram->EMUInit=5;//0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
					return;
				#else
					//���ظ�д����
					Meter_Ram->SSP1Task0=0;
					Meter_Ram->EMUInit=5;//0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
					Meter_Ram->SSP1Sched=1;//�л�������1
					return;
				#endif
				}
				Meter_Ram->SSP1Addr0=EMU_Adj_List[i].Addr;
				switch(EMU_Adj_List[i].RW)//��д:1=��,4=дCS,0x81=д����
				{
					case 0x81://д����
					#if MeterSpec==20//�Զ���Ӧ���ܱ���	
						switch(EMU_Adj_List[i].Addr)
						{
					////High Word of PL_Constant
					//{
					//	0x31,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//#if MeterIMaxhex<=2000//������xx.xx(A)
					//	(450000000000/MeterActiveConst)>>16,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#else
					//	(450000000000/(MeterActiveConst*2))>>16,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#endif
					//},
							case 0x31:
								switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
								{
									case 0:
										Meter_Ram->SSP1Data0=(450000000000/(MeterActiveConst220V*2))>>16;
										break;
									case 1:
										Meter_Ram->SSP1Data0=(450000000000/(MeterActiveConst100V*2))>>16;
										break;
									default:
										Meter_Ram->SSP1Data0=(450000000000/(MeterActiveConst57V*2))>>16;
										break;
								}
								break;
					////Low Word of PL_Constant
					//{
					//	0x32,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//#if MeterIMaxhex<=2000//������xx.xx(A)
					//	(450000000000/MeterActiveConst)&0xffff,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#else
					//	(450000000000/(MeterActiveConst*2))&0xffff,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#endif
					//},
							case 0x32:
								switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
								{
									case 0:
										Meter_Ram->SSP1Data0=(450000000000/(MeterActiveConst220V*2))&0xffff;
										break;
									case 1:
										Meter_Ram->SSP1Data0=(450000000000/(MeterActiveConst100V*2))&0xffff;
										break;
									default:
										Meter_Ram->SSP1Data0=(450000000000/(MeterActiveConst57V*2))&0xffff;
										break;
								}
								break;
					////Metering method configuration
					//{
					//	0x33,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//	0x287+(MeterPW<<8)+(MeterFreq<<12),//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//},
							case 0x33:
								if(Get_MeterSpec()==1)
								{
									Meter_Ram->SSP1Data0=0x287+(1<<8)+(MeterFreq<<12);
								}else
								{
									Meter_Ram->SSP1Data0=0x287+(0<<8)+(MeterFreq<<12);
								}
								break;
					////Active Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 Watt (DSP bit 27 aligned)
					//{
					//	0x35,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
					//	(MeterPStartupHex*2)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#else
					//	(MeterPStartupHex)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#endif
					//},
					////Reactive Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 var
					//{
					//	0x36,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
					//	(MeterQStartupHex*2)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#else
					//	(MeterQStartupHex)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#endif
					//},
					////Apparent Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 VA
					//{
					//	0x37,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//#if MeterPW==0//��ʼ���߷�ʽ;0=��������,1=��������
					//	(MeterPStartupHex*2)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#else
					//	(MeterPStartupHex)/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//#endif
					//},
					////Startup power threshold (for |P|+|Q| of a phase) for any phase participating Active Energy Accumulation.Common for phase A/B/C.16 bit unsigned integer,Unit: 0.00032 Watt/var (DSP bit 23 aligned)
					//{
					//	0x38,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//	MeterPStartupHex/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//},
					////Startup power threshold (for |P|+|Q| of a phase) for any phase participating ReActive Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
					//{
					//	0x39,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//	MeterQStartupHex/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//},
					////Startup power threshold (for |P|+|Q| of a phase) for any phase participating Apparent Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
					//{
					//	0x3a,//ADI�Ĵ�����ַ
					//	0x81,//��д:1=��,4=дCS,0x81=д����
					//	0,//����������:0=����,1=A��,2=B��,3=C��,4=����
					//	MeterPStartupHex/0.32,//�Ĵ�������,����16λ=0��ֱ���õ�16λ����;����16λ!=0��Ϊ���ݵ�ַ�Ӵ洢����ȡ16λ����
					//},
							case 0x35:
							case 0x36:
							case 0x37:
							case 0x38:
							case 0x39:
							case 0x3a:
								switch(Get_MeterSpec())//�Զ���Ӧ���ܱ���;����:0=220V,1=100V,2=57.7V
								{
									case 0:
										Meter_Ram->SSP1Data0=(2200*100*0.00066)/0.32;
										break;
									case 1:
										Meter_Ram->SSP1Data0=(1000*150*0.00066)/0.32;
										break;
									default:
										Meter_Ram->SSP1Data0=(577*150*0.00066)/0.32;
										break;
								}
								break;
							default:
								if((EMU_Adj_List[i].Data>>16)==0x0)
								{
									Meter_Ram->SSP1Data0=EMU_Adj_List[i].Data;
								}
								else
								{
									Meter_Ram->SSP1Data0=0;
									MR((u32)&Meter_Ram->SSP1Data0,EMU_Adj_List[i].Data,2);
								}
								break;
						}
						Meter_Ram->SSP1Task0=0x81;//SSP1����0:0=��,0x1=�ȴ���,0x2=����ʼ,0x4=�����;0x81=�ȴ�д,0x82=д��ʼ,0x84=д���
					#else
						if((EMU_Adj_List[i].Data>>16)==0x0)
						{
							Meter_Ram->SSP1Data0=EMU_Adj_List[i].Data;
						}
						else
						{
							Meter_Ram->SSP1Data0=0;
							MR((u32)&Meter_Ram->SSP1Data0,EMU_Adj_List[i].Data,2);
						}
						Meter_Ram->SSP1Task0=0x81;//SSP1����0:0=��,0x1=�ȴ���,0x2=����ʼ,0x4=�����;0x81=�ȴ�д,0x82=д��ʼ,0x84=д���
					#endif
						break;
					case 4://дCS
						Meter_Ram->SSP1Task0=0x81;//SSP1����0:0=��,0x1=�ȴ���,0x2=����ʼ,0x4=�����;0x81=�ȴ�д,0x82=д��ʼ,0x84=д���
						break;
					case 1://��
						Meter_Ram->SSP1Task0=1;//SSP1����0:0=��,0x1=�ȴ���,0x2=����ʼ,0x4=�����;0x81=�ȴ�д,0x82=д��ʼ,0x84=д���
						break;
				}
				i++;
				Meter_Ram->EMUParaCount=i;
				if(Meter_Ram->EMUInit>=5)//����оƬ��ʼ����־:0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
				{
					Meter_Ram->SSP1Sched=1;//�л�������1
				}
				break;
		}
	}
	else
	{
		EMURWTask1();//��дEMU����1
		switch(Meter_Ram->SSP1Task1)
		{
			case 0:
			case 0x4://�����
			case 0x5://������
			case 0x84://д���
			case 0x85://д����
				Meter_Ram->SSP1Sched=0;//�л�������0
				break;
		}
	}
}



u32 EMUReadWait(u32 Addr)//��EMU(ֱ�����ζ��������,������У��ʱ��)
{
  u32 i;

  while(1)
  {
		while(Meter_Ram->EMUInit<5)
		{//����оƬ��ʼ����־:0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
			EMURW();//��дEMU(�Զ�����0������1)
		}
    if(Meter_Ram->SSP1Sched==0)
    {
      i=Meter_Ram->SSP1Task0;
    }
    else
    {
      i=Meter_Ram->SSP1Task1;
    }
		if((i==0)||(i==4)||(i==5)||(i==0x84)||(i==0x85))
		{
			break;
		}
		else
		{
			EMURW();//��дEMU(�Զ�����0������1)
		}
  }
	i=Read_90E36(Addr|0x8000);//��90E36
  return i;
}

void EMUWriteWait(u32 Addr,u32 Data)//дEMU(ֱ������д�������,������У��ʱд)
{
  u32 i;

  while(1)
  {
		while(Meter_Ram->EMUInit<5)
		{//����оƬ��ʼ����־:0=�ص�Դ(�ŵ�),1=����Դ,2=��λ,3=��λ���,4=��ʼ��,5=���
			EMURW();//��дEMU(�Զ�����0������1)
		}
    if(Meter_Ram->SSP1Sched==0)
    {
      i=Meter_Ram->SSP1Task0;
    }
    else
    {
      i=Meter_Ram->SSP1Task1;
    }
		if((i==0)||(i==4)||(i==5)||(i==0x84)||(i==0x85))
		{
			break;
		}
		else
		{
			EMURW();//��дEMU(�Զ�����0������1)
		}
  }
	Write_90E36((Addr<<16)|Data);//д90E36
}


void EMURead(u32 Addr1,u32 Addr2)//��EMU�������,SSP1Task1Count=n��ʾ��n�����
{
	u32 i;
	
	if(Meter_Ram->SSP1Task1Count==0)
	{
		if(Meter_Ram->SSP1Task1!=4)
		{
			Meter_Ram->SSP1Addr1=Addr1;
			Meter_Ram->SSP1Task1=1;//0x1=�ȴ���
		}
		else
		{//�����
			Meter_Ram->SSP1Data2=Meter_Ram->SSP1Data1;//�ݴ�SSP1Data1
			Meter_Ram->SSP1Addr1=Addr2;
			Meter_Ram->SSP1Task1=1;//0x1=�ȴ���
			Meter_Ram->SSP1Task1Count=1;
		}
	}
	else
	{
		if(Meter_Ram->SSP1Task1!=4)
		{
			Meter_Ram->SSP1Addr1=Addr2;
			Meter_Ram->SSP1Task1=1;//0x1=�ȴ���
		}
		else
		{//�����
			i=Meter_Ram->SSP1Data1;
			Meter_Ram->SSP1Data1=Meter_Ram->SSP1Data2;
			Meter_Ram->SSP1Data2=i;
			Meter_Ram->SSP1Task1Count=2;
		}
	}
}

#endif//#if IC_EMU==1//0=û,1=��

















