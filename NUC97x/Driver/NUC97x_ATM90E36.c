
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




#if IC_90E36==1//0=没,1=有

#if MeterSpec==20//自动适应电能表规格
u32 Get_MeterSpec(void)//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
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
		Meter_Ram->PW=0;//初始接线方式;0=三相四线,1=三相三线
		return 0;//0=220V,1=100V,2=57.7V
	}
	if(Umax>0x0750)//75.0V
	{
		Meter_Ram->PW=1;//初始接线方式;0=三相四线,1=三相三线
		return 1;//0=220V,1=100V,2=57.7V
	}
	Meter_Ram->PW=0;//初始接线方式;0=三相四线,1=三相三线
	return 2;//0=220V,1=100V,2=57.7V
}
#endif

#if MeterIMaxhex<=2000//最大电流xx.xx(A)
__asm void MeterCalInitLib(void)//校表参数初始化库(IGAIN=0)
{
//电表校表参数------------------------------------------------------------------------------------
//#define ADDR_MeterCalibration_IEEPROM_Start     (ADDR_IEEPROM_Start)
	dcdu 0//4BYTE CRC32
	dcdu 0x12345678//4BYTE 电表校表参数有效标志固定为0x12345678
	dcwu 0//2BYTE RTCCAL
	dcwu 0//2BYTE 温度CAL
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
#else//#if MeterIMaxhex<=2000//最大电流xx.xx(A)
__asm void MeterCalInitLib(void)//校表参数初始化库(IGAIN=0)
{
//电表校表参数------------------------------------------------------------------------------------
//#define ADDR_MeterCalibration_IEEPROM_Start     (ADDR_IEEPROM_Start)
	dcdu 0//4BYTE CRC32
	dcdu 0x12345678//4BYTE 电表校表参数有效标志固定为0x12345678
	dcwu 0//2BYTE RTCCAL
	dcwu 0//2BYTE 温度CAL
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
#endif//#if MeterIMaxhex<=2000//最大电流xx.xx(A)




typedef __packed struct
{
  u16 Addr;//ADI寄存器地址
	u8 RW;//读写:1=读,4=写CS,0x81=写参数
	u8 Phase;//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
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
  	0x30,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x8765,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //High Word of PL_Constant
	{
  	0x31,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
	#if MeterIMaxhex<=2000//最大电流xx.xx(A)
  	(450000000000/MeterActiveConst)>>16,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#else
  	(450000000000/(MeterActiveConst*2))>>16,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#endif
	},
  //Low Word of PL_Constant
	{
  	0x32,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
	#if MeterIMaxhex<=2000//最大电流xx.xx(A)
  	(450000000000/MeterActiveConst)&0xffff,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#else
  	(450000000000/(MeterActiveConst*2))&0xffff,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#endif
	},
  //Metering method configuration
	{
  	0x33,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
	#if MeterSpec==20//自动适应电能表规格
		0x287+(0<<8)+(MeterFreq<<12),//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#else
  	0x287+(MeterPW<<8)+(MeterFreq<<12),//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#endif
	},
  //PGA Gain Configuration
	{
  	0x34,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Active Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 Watt (DSP bit 27 aligned)
	{
  	0x35,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
	#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
  	(MeterPStartupHex*2)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#else
  	(MeterPStartupHex)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#endif
	},
  //Reactive Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 var
	{
  	0x36,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
	#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
  	(MeterQStartupHex*2)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#else
  	(MeterQStartupHex)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#endif
	},
  //Apparent Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 VA
	{
  	0x37,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
	#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
  	(MeterPStartupHex*2)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#else
  	(MeterPStartupHex)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	#endif
	},
  //Startup power threshold (for |P|+|Q| of a phase) for any phase participating Active Energy Accumulation.Common for phase A/B/C.16 bit unsigned integer,Unit: 0.00032 Watt/var (DSP bit 23 aligned)
	{
  	0x38,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	MeterPStartupHex/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Startup power threshold (for |P|+|Q| of a phase) for any phase participating ReActive Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
	{
  	0x39,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	MeterQStartupHex/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Startup power threshold (for |P|+|Q| of a phase) for any phase participating Apparent Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
	{
  	0x3a,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	MeterPStartupHex/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS0-R
	{
  	0x3b,//ADI寄存器地址
		1,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS0-W
	{
  	0x3b,//ADI寄存器地址
		4,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	
//6.4.3 ENERGY CALIBRATION REGISTERS
  //Calibration Start Command
	{
  	0x40,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x8765,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Active Power Offset
	{
  	0x41,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_41,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Reactive Power Offset
	{
  	0x42,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_42,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Active Power Offset
	{
  	0x43,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_43,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Reactive Power Offset
	{
  	0x44,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_44,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Active Power Offset
	{
  	0x45,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_45,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Reactive Power Offset
	{
  	0x46,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_46,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Active/Reactive Energy calibration gain
	{
  	0x47,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_47,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A calibration phase angle
	{
  	0x48,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_48,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Active/Reactive Energy calibration gain
	{
  	0x49,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_49,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B calibration phase angle
	{
  	0x4A,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_4A,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Active/Reactive Energy calibration gain
	{
  	0x4B,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_4B,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C calibration phase angle
	{
  	0x4C,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_4C,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS-R
	{
  	0x4D,//ADI寄存器地址
		1,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS-W
	{
  	0x4D,//ADI寄存器地址
		4,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},

//6.4.4 FUNDAMENTAL/HARMONIC ENERGY CALIBRATION REGISTERS
  //Harmonic Calibration Startup Command
	{
  	0x50,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x8765,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Fundamental Active Power Offset
	{
  	0x51,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_51,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Fundamental Active Power Offset
	{
  	0x52,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_52,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Fundamental Active Power Offset
	{
  	0x53,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_53,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Fundamental Active Power Gain
	{
  	0x54,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_54,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Fundamental Active Power Gain
	{
  	0x55,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_55,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Fundamental Active Power Gain
	{
  	0x56,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_56,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS-R
	{
  	0x57,//ADI寄存器地址
		1,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS-W
	{
  	0x57,//ADI寄存器地址
		4,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},

//6.4.5 MEASUREMENT CALIBRATION
  //Measurement Calibration Startup Command
	{
  	0x60,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x8765,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Voltage RMS Gain
	{
  	0x61,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_61,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Current RMS Gain
	{
  	0x62,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_62,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Voltage RMS Offset
	{
  	0x63,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_63,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase A Current RMS Offset
	{
  	0x64,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		1,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_64,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Voltage RMS Gain
	{
  	0x65,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_65,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Current RMS Gain
	{
  	0x66,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_66,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Voltage RMS Offset
	{
  	0x67,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_67,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase B Current RMS Offset
	{
  	0x68,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		2,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_68,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Voltage RMS Gain
	{
  	0x69,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_69,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Current RMS Gain
	{
  	0x6A,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_6A,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Voltage RMS Offset
	{
  	0x6B,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_6B,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Phase C Current RMS Offset
	{
  	0x6C,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		3,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_6C,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Sampled N line Current RMS Gain
	{
  	0x6D,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		4,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_6D,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
  //Sampled N line Current RMS Offset
	{
  	0x6E,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		4,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	ADDR_EMU_6E,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS-R
	{
  	0x6F,//ADI寄存器地址
		1,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//CS-W
	{
  	0x6F,//ADI寄存器地址
		4,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},

//7.1片上温度传感器的设置
	{
  	0x02FD,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0xAA55,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x0216,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x5122,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x0219,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x012B,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x02FD,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x0000,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//7.2 基于A/D 采样通道的温度补偿
	//向2FCH 地址写入AA55H
	{
  	0x02FC,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0xAA55,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//向270H 地址写入需要补偿的温度系数(温度补偿系数，bit7 为符号位，单位ppm/ ℃)
	{
  	0x0270,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//向27BH 地址写入温度补偿的基准点温度
	{
  	0x027B,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x8000+25,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	//向2FCH 地址写入0000H
	{
  	0x02FC,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x0000,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	
//8 谐波分析
	//开启DFT计算
	{
  	0x01D1,//ADI寄存器地址
		0x81,//读写:1=读,4=写CS,0x81=写参数
		0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
  	0x0001,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},

};



void Write_90E36(u32 Data)//写90E36
{
//	u32 i;
	u32* p32cs;
	u32 *p32ctrl;
	u32 *p32TxData;
	
	Init_SPI2();//SPI2初始化(ATM90E36或 ATT7022)
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

u32 Read_90E36(u32 Addr)//读90E36
{
//	u32 i;
	u32* p32cs;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	Init_SPI2();//SPI2初始化(ATM90E36或 ATT7022)
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


//90E36 BUG 读写数据相隔5ms以上再读0x0f寄存器检查，则0x0f寄存器读出的值为0或0xffff

//重读比较
void EMURWTask0(void)//读写EMU任务0
{
  u32 i;

    switch(Meter_Ram->SSP1Task0)
    {    
      case 0x1://等待读
        Meter_Ram->SSP1Task0=0x2;
        //break;
      case 0x2://读开始
				Meter_Ram->SSP1Data0=Read_90E36(Meter_Ram->SSP1Addr0|0x8000);//读90E36
        Meter_Ram->SSP1Task0=0x3;
        //break;
      case 0x3://读检查
				i=Read_90E36(0x800F);//读90E36
        if(Meter_Ram->SSP1Data0!=i)
        {
					Meter_Ram->SSP1Data0=0xffff;
					Meter_Ram->SSP1Task0=0x5;//读错误
					Meter_Ram->SSP1ErrorCount++;//SSP1通信错误计数+1
					if(Meter_Ram->SSP1ErrorCount>16)
					{//连续16次错误作为读完成，使能读下个数据
						Meter_Ram->SSP1ErrorCount=0;//SSP1通信错误计数
						Meter_Ram->SSP1Task0=0x4;//读完成
					}
        }
        else
        {
					Meter_Ram->SSP1ErrorCount=0;//SSP1通信错误计数
          Meter_Ram->SSP1Task0=0x4;//读完成
        }
        break;
      case 0x4://读完成
      case 0x5://读错误
        break;
        
      case 0x81://等待写
        Meter_Ram->SSP1Task0=0x82;
        //break;
      case 0x82://写开始
				i=(Meter_Ram->SSP1Addr0<<16)|Meter_Ram->SSP1Data0;
				Write_90E36(i);//写90E36
        Meter_Ram->SSP1Task0=0x83;
        //break;
      case 0x83://写检查
				i=Read_90E36(0x800F);//读90E36
        if(Meter_Ram->SSP1Data0!=i)
        {
          Meter_Ram->EMUInit=0;//计量芯片初始化标志:0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
          Comm_Ram->ErrorIC|=(1<<5);//硬件错误标志(置位):B5=EMU 计量芯片
					Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
          Meter_Ram->SSP1Task0=0x85;//写错误
        }
        else
        {
          Meter_Ram->SSP1Task0=0x84;//写完成
        }
        break;
      case 0x84://写完成
      case 0x85://写错误
        break;
      default:
        Meter_Ram->SSP1Task0=0;
        break;
    }  
}
void EMURWTask1(void)//读写EMU任务1
{
  u32 i;

		switch(Meter_Ram->SSP1Task1)
    {    
      case 0x1://等待读
        Meter_Ram->SSP1Task1=0x2;
        //break;
      case 0x2://读开始
				Meter_Ram->SSP1Data1=Read_90E36(Meter_Ram->SSP1Addr1|0x8000);//读90E36
        Meter_Ram->SSP1Task1=0x3;
        //break;
      case 0x3://读检查
				i=Read_90E36(0x800F);//读90E36
        if(Meter_Ram->SSP1Data1!=i)
        {
					Meter_Ram->SSP1Data1=0xffff;
					Meter_Ram->SSP1Task1=0x5;//读错误
					Meter_Ram->SSP1ErrorCount++;//SSP1通信错误计数+1
					if(Meter_Ram->SSP1ErrorCount>16)
					{//连续16次错误作为读完成，使能读下个数据
						Meter_Ram->SSP1ErrorCount=0;//SSP1通信错误计数
						Meter_Ram->SSP1Task1=0x4;//读完成
					}
        }
        else
        {
					Meter_Ram->SSP1ErrorCount=0;//SSP1通信错误计数
          Meter_Ram->SSP1Task1=0x4;//读完成
        }
        break;
      case 0x4://读完成
      case 0x5://读错误
        break;
        
      case 0x81://等待写
        Meter_Ram->SSP1Task1=0x82;
        //break;
      case 0x82://写开始
				Write_90E36((Meter_Ram->SSP1Addr1<<16)|Meter_Ram->SSP1Data1);//写90E36
        Meter_Ram->SSP1Task1=0x83;
        //break;
      case 0x83://写检查
				i=Read_90E36(0x800F);//读90E36
        if(Meter_Ram->SSP1Data1!=i)
        {
          Meter_Ram->SSP1Task1=0x85;//写错误
        }
        else
        {
          Meter_Ram->SSP1Task1=0x84;//写完成
        }
        break;
      case 0x84://写完成
      case 0x85://写错误
        break;
      default:
        Meter_Ram->SSP1Task1=0;
        break;
    }  
}



void EMURW(void)//读写EMU(自动任务0或任务1)
{
  u32 i;
	
	switch(Meter_Ram->EMUInit)
	{//计量芯片初始化标志:0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
		case 0://关电源(放电)
			OffEMU();//关EMU(复位)
			OffA5V0();//关A5V0电源
			Comm_Ram->SPI2Timer=1000/10;//12 SSP1 每10MS减1定时器
			Meter_Ram->EMUInit=1;
			return;
		case 1://开电源
			if(Comm_Ram->SPI2Timer==0)
			{
				OnA5V0();//打开A5V0电源
				Comm_Ram->SPI2Timer=1000/10;//12 SSP1 每10MS减1定时器
				Meter_Ram->EMUInit=2;
			}
			return;
		case 2://复位
			if(Comm_Ram->SPI2Timer==0)
			{
				OnEMU();//打开EMU(复位解除)
				Comm_Ram->SPI2Timer=100/10;//12 SSP1 每10MS减1定时器
				Meter_Ram->EMUInit=3;
			}
			return;
		case 3://复位解除
			if(Comm_Ram->SPI2Timer==0)
			{
//				Init_SPI2();//SSP1初始化(EMU)
				Meter_Ram->EMUParaCount=0;//计量芯片参数初始化计数
				Meter_Ram->SSP1Task1Count=0;//SSP1任务1多个数据连续读计数
				Meter_Ram->SSP1Task0=0;//SSP1任务0:0=空,0x1=等待读,0x2=读开始,0x3=读比较,0x4=读完成,0x5=读错误;0x81=等待写,0x82=写开始,0x83=写比较,0x84=写完成,0x85=写错误
				Meter_Ram->SSP1Task1=0;//SSP1任务1:0=空,0x1=等待读,0x2=读开始,0x3=读比较,0x4=读完成,0x5=读错误;0x81=等待写,0x82=写开始,0x83=写比较,0x84=写完成,0x85=写错误
				Meter_Ram->EMUInit=4;
				Meter_Ram->TxCount=2;//
			}
			return;
		case 4://初始化
		case 5://完成
			break;
		default:
			Meter_Ram->EMUInit=0;
			return;
	}
	//if((SPI2->SR&1)==0)
	if(Meter_Ram->TxCount!=2)
  {//0：接收缓冲区为空
    if(Comm_Ram->SPI2Timer!=0)
    {//定时没到
			return;
		}
		//SSP1死机
    //Init_SPI2();//SSP1初始化(EMU)
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
		EMURWTask0();//读写EMU任务0
		switch(Meter_Ram->SSP1Task0)
		{
			case 0:
			case 0x4://读完成
			case 0x5://读错误
			case 0x84://写完成
			case 0x85://写错误
				i=Meter_Ram->EMUParaCount;
				if(i>=(sizeof(EMU_Adj_List)/sizeof(EMU_Adj_List_TypeDef)))
				{
				#if MeterSpec==20//自动适应电能表规格	
					//重复写参数
					Meter_Ram->EMUParaCount=0;
					Meter_Ram->EMUInit=5;//0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
					return;
				#else
					//不重复写参数
					Meter_Ram->SSP1Task0=0;
					Meter_Ram->EMUInit=5;//0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
					Meter_Ram->SSP1Sched=1;//切换到任务1
					return;
				#endif
				}
				Meter_Ram->SSP1Addr0=EMU_Adj_List[i].Addr;
				switch(EMU_Adj_List[i].RW)//读写:1=读,4=写CS,0x81=写参数
				{
					case 0x81://写参数
					#if MeterSpec==20//自动适应电能表规格	
						switch(EMU_Adj_List[i].Addr)
						{
					////High Word of PL_Constant
					//{
					//	0x31,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//#if MeterIMaxhex<=2000//最大电流xx.xx(A)
					//	(450000000000/MeterActiveConst)>>16,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#else
					//	(450000000000/(MeterActiveConst*2))>>16,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#endif
					//},
							case 0x31:
								switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
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
					//	0x32,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//#if MeterIMaxhex<=2000//最大电流xx.xx(A)
					//	(450000000000/MeterActiveConst)&0xffff,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#else
					//	(450000000000/(MeterActiveConst*2))&0xffff,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#endif
					//},
							case 0x32:
								switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
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
					//	0x33,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//	0x287+(MeterPW<<8)+(MeterFreq<<12),//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
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
					//	0x35,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
					//	(MeterPStartupHex*2)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#else
					//	(MeterPStartupHex)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#endif
					//},
					////Reactive Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 var
					//{
					//	0x36,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
					//	(MeterQStartupHex*2)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#else
					//	(MeterQStartupHex)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#endif
					//},
					////Apparent Startup Power Threshold.16 bit unsigned integer, Unit: 0.00032 VA
					//{
					//	0x37,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//#if MeterPW==0//初始接线方式;0=三相四线,1=三相三线
					//	(MeterPStartupHex*2)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#else
					//	(MeterPStartupHex)/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//#endif
					//},
					////Startup power threshold (for |P|+|Q| of a phase) for any phase participating Active Energy Accumulation.Common for phase A/B/C.16 bit unsigned integer,Unit: 0.00032 Watt/var (DSP bit 23 aligned)
					//{
					//	0x38,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//	MeterPStartupHex/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//},
					////Startup power threshold (for |P|+|Q| of a phase) for any phase participating ReActive Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
					//{
					//	0x39,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//	MeterQStartupHex/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//},
					////Startup power threshold (for |P|+|Q| of a phase) for any phase participating Apparent Energy Accumulation.Common for phase A/B/C.16bit unsigned integer,Unit: 0.00032 Watt/var
					//{
					//	0x3a,//ADI寄存器地址
					//	0x81,//读写:1=读,4=写CS,0x81=写参数
					//	0,//参数归属相:0=合相,1=A相,2=B相,3=C相,4=零线
					//	MeterPStartupHex/0.32,//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
					//},
							case 0x35:
							case 0x36:
							case 0x37:
							case 0x38:
							case 0x39:
							case 0x3a:
								switch(Get_MeterSpec())//自动适应电能表规格;返回:0=220V,1=100V,2=57.7V
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
						Meter_Ram->SSP1Task0=0x81;//SSP1任务0:0=空,0x1=等待读,0x2=读开始,0x4=读完成;0x81=等待写,0x82=写开始,0x84=写完成
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
						Meter_Ram->SSP1Task0=0x81;//SSP1任务0:0=空,0x1=等待读,0x2=读开始,0x4=读完成;0x81=等待写,0x82=写开始,0x84=写完成
					#endif
						break;
					case 4://写CS
						Meter_Ram->SSP1Task0=0x81;//SSP1任务0:0=空,0x1=等待读,0x2=读开始,0x4=读完成;0x81=等待写,0x82=写开始,0x84=写完成
						break;
					case 1://读
						Meter_Ram->SSP1Task0=1;//SSP1任务0:0=空,0x1=等待读,0x2=读开始,0x4=读完成;0x81=等待写,0x82=写开始,0x84=写完成
						break;
				}
				i++;
				Meter_Ram->EMUParaCount=i;
				if(Meter_Ram->EMUInit>=5)//计量芯片初始化标志:0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
				{
					Meter_Ram->SSP1Sched=1;//切换到任务1
				}
				break;
		}
	}
	else
	{
		EMURWTask1();//读写EMU任务1
		switch(Meter_Ram->SSP1Task1)
		{
			case 0:
			case 0x4://读完成
			case 0x5://读错误
			case 0x84://写完成
			case 0x85://写错误
				Meter_Ram->SSP1Sched=0;//切换到任务0
				break;
		}
	}
}



u32 EMUReadWait(u32 Addr)//读EMU(直至本次读操作完成,仅用于校验时读)
{
  u32 i;

  while(1)
  {
		while(Meter_Ram->EMUInit<5)
		{//计量芯片初始化标志:0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
			EMURW();//读写EMU(自动任务0或任务1)
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
			EMURW();//读写EMU(自动任务0或任务1)
		}
  }
	i=Read_90E36(Addr|0x8000);//读90E36
  return i;
}

void EMUWriteWait(u32 Addr,u32 Data)//写EMU(直至本次写操作完成,仅用于校验时写)
{
  u32 i;

  while(1)
  {
		while(Meter_Ram->EMUInit<5)
		{//计量芯片初始化标志:0=关电源(放电),1=开电源,2=复位,3=复位解除,4=初始化,5=完成
			EMURW();//读写EMU(自动任务0或任务1)
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
			EMURW();//读写EMU(自动任务0或任务1)
		}
  }
	Write_90E36((Addr<<16)|Data);//写90E36
}


void EMURead(u32 Addr1,u32 Addr2)//读EMU多个数据,SSP1Task1Count=n表示读n个完成
{
	u32 i;
	
	if(Meter_Ram->SSP1Task1Count==0)
	{
		if(Meter_Ram->SSP1Task1!=4)
		{
			Meter_Ram->SSP1Addr1=Addr1;
			Meter_Ram->SSP1Task1=1;//0x1=等待读
		}
		else
		{//读完成
			Meter_Ram->SSP1Data2=Meter_Ram->SSP1Data1;//暂存SSP1Data1
			Meter_Ram->SSP1Addr1=Addr2;
			Meter_Ram->SSP1Task1=1;//0x1=等待读
			Meter_Ram->SSP1Task1Count=1;
		}
	}
	else
	{
		if(Meter_Ram->SSP1Task1!=4)
		{
			Meter_Ram->SSP1Addr1=Addr2;
			Meter_Ram->SSP1Task1=1;//0x1=等待读
		}
		else
		{//读完成
			i=Meter_Ram->SSP1Data1;
			Meter_Ram->SSP1Data1=Meter_Ram->SSP1Data2;
			Meter_Ram->SSP1Data2=i;
			Meter_Ram->SSP1Task1Count=2;
		}
	}
}

#endif//#if IC_EMU==1//0=没,1=有

















