
//EEPROM
#ifndef IEEPROM_H
#define IEEPROM_H




//90E36校表参数------------------------------------------------------------------------------------
#define ADDR_MeterCalibration_IEEPROM_Start     0x08008000//16k
//4BYTE CRC32
//4BYTE 校表参数有效标志固定为0x12345678
//2BYTE RTCCAL
//2BYTE 温度CAL
#define ADDR_ADC4MA     ADDR_MeterCalibration_IEEPROM_Start+12
//2BYTE 直流模拟量电流环4MA输入时ADC转换标准值
#define ADDR_ADC20MA     ADDR_MeterCalibration_IEEPROM_Start+14
//2BYTE 直流模拟量电流环20MA输入时ADC转换标准值
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
#define LEN_MeterCalibration_IEEPROM     (128*2)//校表参数总长度=128*2字节
#if ((ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_Start)>LEN_MeterCalibration_IEEPROM)
#error ADDR_MeterCalibration_IEEPROM_End Over
#endif
//校表参数结束--------------------------------------------------------------------------------


//电能表误差检定记录数据---------------------------------------------------------
#define ADDR_MeterError_IEEPROM_Start     (ADDR_MeterCalibration_IEEPROM_Start+LEN_MeterCalibration_IEEPROM)
//2BYTE_BCD 3位小数 最高位为1表示负 合相1.0 Imax
//2BYTE_BCD 3位小数 最高位为1表示负 合相0.5L Imax
//2BYTE_BCD 3位小数 最高位为1表示负 合相1.0 Ib
//2BYTE_BCD 3位小数 最高位为1表示负 合相0.5L Ib
//2BYTE_BCD 3位小数 最高位为1表示负 合相1.0 5%Ib
//2BYTE_BCD 3位小数 最高位为1表示负 合相0.5L 5%Ib
//2BYTE_BCD 3位小数 最高位为1表示负 合相1.0 1%Ib
//2BYTE_BCD 3位小数 最高位为1表示负 合相0.5L 1%Ib
//A相...
//B相...
//C相...

//2BYTE_BCD 3位小数 最高位为1表示负 A相电压误差
//2BYTE_BCD 3位小数 最高位为1表示负 B相电压误差
//2BYTE_BCD 3位小数 最高位为1表示负 C相电压误差
//2BYTE_BCD 3位小数 最高位为1表示负 A相电流误差
//2BYTE_BCD 3位小数 最高位为1表示负 B相电流误差
//2BYTE_BCD 3位小数 最高位为1表示负 C相电流误差
//2BYTE_BCD 3位小数 最高位为1表示负 总有功功率误差
//2BYTE_BCD 3位小数 最高位为1表示负 A相有功功率误差
//2BYTE_BCD 3位小数 最高位为1表示负 B相有功功率误差
//2BYTE_BCD 3位小数 最高位为1表示负 C相有功功率误差
//2BYTE_BCD 3位小数 最高位为1表示负 总无功功率误差
//2BYTE_BCD 3位小数 最高位为1表示负 A相无功功率误差
//2BYTE_BCD 3位小数 最高位为1表示负 B相无功功率误差
//2BYTE_BCD 3位小数 最高位为1表示负 C相无功功率误差
	//以上64+28BYTE

#define LEN_MeterError_IEEPROM     (128*1)//电能表误差检定记录数据总长度=128*1字节
#define ADDR_MeterError_IEEPROM_End          (ADDR_MeterError_IEEPROM_Start+LEN_MeterError_IEEPROM)
//电能表误差检定记录数据结束----------------------------------------------------------------------


#if ((ADDR_MeterError_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_Start)>0x4000)
#error ADDR_MeterError_IEEPROM Over
#endif



//NANDFLASH REPLACE----------------------------------------------------------------------------------------------
#define ADDR_NANDFLASH_BLOCK_REPLACE     0x0800C000//16k   NANDFLASH REPLACE
//区域1文件系统使用的块:0xFC=已初始化标志,0xFD=未损坏,0xFE=块损坏,(0x00,0xFF可能片子出厂初始值禁用);文件系统的块只有3个表示值0xFC,0xFD和0xFE,其他值等同0xFF
//1BYTE 块计数0
//1BYTE ...
//1BYTE 块计数NAND_FILE_BLOCK_COUNT-1
//区域2替换用的块:0xFD=已用于替换,0xFE=块损坏,(0x00,0xFF可能片子出厂初始值禁用);替换用的块只有2个表示值0xFD和0xFE,其他值等同0xFF
//1BYTE 块计数NAND_FILE_BLOCK_COUNT
//1BYTE ...
//1BYTE 块计数NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT-1
//区域3通用数据块:1-NAND_REPLACE_BLOCK_COUNT表示被替换的替换块号同时表示块错误,1-250(0xFA)=有替换的替换区的替换块号,0xFB=保留,0xFC=保留,0xFD=未损坏(无替换),0xFE=块损坏(无替换),(0x00,0xFF可能片子出厂初始值禁用)
//1BYTE 块计数NAND_FILE_BLOCK_COUNT+NAND_REPLACE_BLOCK_COUNT
//...
//1BYTE 块计数NAND_BLOCK_COUNT-1

#define ADDR_NANDFLASH_BLOCK_REPLACE_End     (ADDR_NANDFLASH_BLOCK_REPLACE+8192)
#if NAND_BLOCK_COUNT>8192
#error NAND_BLOCK_COUNT>8192
#endif
//NANDFLASH REPLACE结束--------------------------------------------------------------------------------------------------



//电气性能测试记录-----------------------------------------------------------------------------------
#define ADDR_Characteristics_NO     ADDR_NANDFLASH_BLOCK_REPLACE_End
//1BYTE 按温度区分的测试号:0=常温(-5<T<45),1=低温(T<-5),2=高温（T>55)
#define ADDR_Characteristics_BASE1     (ADDR_Characteristics_NO+1)//按温度区分的测试号:0=常温
#define OFFSET_Test_Temperature     0
//2BYTE BCD 1位小数,测试时表内温度
#define OFFSET_NAND_ERROR_BLOCKS     OFFSET_Test_Temperature+2
//2BYTE nand错误块数,0xffff表示没测试
#define OFFSET_NAND_LIFE     OFFSET_NAND_ERROR_BLOCKS+2
//4BYTE nand寿命(已擦写次数)
#define OFFSET_BATTERY_LIFE     OFFSET_NAND_LIFE+4
//2BYTE 充电时间分
//2BYTE 放电电时间分
//1BYTE 充电开始时电压x.xV BCD
//1BYTE 充电结束时电压x.xV BCD
//1BYTE 放电开始时电压x.xV BCD
//1BYTE 放电结束时电压x.xV BCD

#define OFFSET_POWER_Characteristics     (OFFSET_BATTERY_LIFE+8)
//2BYTE BCD 2位小数,VDD5V0
//2BYTE BCD 2位小数,ADD5V0
//2BYTE BCD 2位小数,IO3V3
//2BYTE BCD 2位小数,RTC_BATTERY3V0
//2BYTE BCD 2位小数,CHARGE_BATTERY4V8
//2BYTE BCD 2位小数,GPRS4V0
//2BYTE BCD 2位小数,ETH3V3
//2BYTE BCD 2位小数,CHARGE12V
#define OFFSET_POWER_DOWN_Characteristics     OFFSET_POWER_Characteristics+16
//1BYTE 4K(BKPSRAM)掉电保持,0=OK,1=ERR,0xff=没测试,0x55=进行测试
//1BYTE 电源掉电数据恢复,0=OK,1=ERR,0xff=没测试,0x55=进行测试
//1BYTE 电源掉电时数据保存时间ms
//2BYTE 电源掉电维持工作时间ms
#define OFFSET_STM32F4xx_Characteristics     OFFSET_POWER_DOWN_Characteristics+5//STM32F4xx
//1BYTE ISRAM 0=OK,1=ERR,0xff=没测试
//1BYTE ECC 0=OK,1=ERR
//1BYTE CRC 0=OK,1=ERR
//1BYTE 外部低速(LSE)32768Hz:0=OK,1=ERR,0xff=没测试
//4BYTE 外部高速(HSE)16MHz晶体频率Hz
//4BYTE 低速内部 RC (LSI RC)频率Hz
#define OFFSET_SDRAM_Characteristics     OFFSET_STM32F4xx_Characteristics+12//SDRAM电气性能
//1BYTE SDRAM_CAS_LATENCY
//1BYTE SdramTiming.LoadToActiveDelay;// = 16;//1-16//加载模式寄存器到激活 (Load Mode Register to Active)
//1BYTE SdramTiming.ExitSelfRefreshDelay;// = 1;//1-16//退出自刷新延迟 (Exit Self-refresh delay)
//1BYTE SdramTiming.SelfRefreshTime;// = 1;//1-16//自刷新时间 (Self refresh time)
//1BYTE SdramTiming.RowCycleDelay;// = 4;//1-16//行循环延迟 (Row cycle delay)
//1BYTE SdramTiming.WriteRecoveryTime;// = 2;//1-16//恢复延迟 (Recovery delay)
//1BYTE SdramTiming.RPDelay;// = 1;//1-16//行预充电延迟 (Row precharge delay)
//1BYTE SdramTiming.RCDDelay;// = 1;//1-16//行到列延迟 (Row to column delay)
#define OFFSET_NAND_Characteristics     OFFSET_SDRAM_Characteristics+8//NAND电气性能
//1BYTE hnand.Init.TCLRSetupTime = 0;//;CPU值(0-15);CLE 到 RE 的延迟
//1BYTE hnand.Init.TARSetupTime = 0;//;CPU值(0-15);ALE 到 RE 的延迟 
//1BYTE ComSpaceTiming.SetupTime = 8-1;//1-255;CPU值(0-255)通用存储器建立时间
//1BYTE ComSpaceTiming.WaitSetupTime = 4-1;//2-255;CPU值(1-255)通用存储器等待时间
//1BYTE ComSpaceTiming.HoldSetupTime = 2;//1-254;CPU值(1-255)通用存储器保持时间
//1BYTE ComSpaceTiming.HiZSetupTime = 1-1;//1-255;CPU值(0-255)通用存储器数据总线高阻态时间
//2BYTE Block Erase Time(ms)
//2BYTE Program Time(us)
//2BYTE Page Read Time(us)
#define OFFSET_90E36_Characteristics     OFFSET_NAND_Characteristics+12
//4BYTE 最大通信速率Hz
//1BYTE 有效复位时间us
#define OFFSET_RTC_Characteristics     OFFSET_90E36_Characteristics+5
//1BYTE 设置时钟99-12-31 6 23:59:59读出比较:0=正确,1=错误
//1BYTE 1秒后读时钟值检查进位00-01-01 0 00:00:00 ;0=正确,1=错误
//2BYTE I2C总线复位间隔时间
//1BYTE 1Hz输出(0=OK,1=ERROR)
//1BYTE 32kHz输出(0=OK,1=ERROR)
#define OFFSET_LAN8720A_Characteristics     OFFSET_RTC_Characteristics+6
//1BYTE Serial Management Interface (SMI):0=OK,1=ERR,0xff=没测试
//1BYTE Auto-Negotiation:0=OK,1=ERR,0xff=没测试
//1BYTE REF_CLK:0=OK,1=ERR,0xff=没测试
//1BYTE tRESET有效复位时间(us)
#define OFFSET_UART_Characteristics     OFFSET_LAN8720A_Characteristics+4
//1BYTE RS485-1,RS485-2通信速率O～11依次表示0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400
//1BYTE 红外口,通信速率O～11依次表示...
//1BYTE RS232,通信速率O～11依次表示...
//1BYTE ETH  0=OK,1=ERR,0xff=没测试
//1BYTE GPRS模块,0=OK,1=ERR,0xff=没测试
//1BYTE 本地载波模块A相,0=OK,1=ERR,0xff=没测试
//1BYTE 本地载波模块B相,0=OK,1=ERR,0xff=没测试
//1BYTE 本地载波模块C相,0=OK,1=ERR,0xff=没测试
#define OFFSET_ESAM_Characteristics     OFFSET_UART_Characteristics+8
//4BYTE TESAM最大通信速率Hz
//8BYTE TESAM序列号
//4BYTE MESAM最大通信速率Hz
//8BYTE MESAM序列号
#define OFFSET_24AA02E48_Characteristics     OFFSET_ESAM_Characteristics+24
//1BYTE 0=OK,1=ERR,0xff=没测试
//6BYTE MAC地址
#define OFFSET_KEY_Characteristics     OFFSET_24AA02E48_Characteristics+7
//1BYTE 按键0=正确,其他值没通过
#define OFFSET_TestResult_Characteristics     OFFSET_KEY_Characteristics+1
//1BYTE 总测试结果0=合格,1=不合格,0xff=无结论
#define OFFSET_SWIN_Characteristics     OFFSET_TestResult_Characteristics+1
//1BYTE 开关量输入1 0=OK,1=ERR,0xff=没测试
//1BYTE 开关量输入2 0=OK,1=ERR,0xff=没测试
//1BYTE 开关量输入3 0=OK,1=ERR,0xff=没测试
//1BYTE 开关量输入4 0=OK,1=ERR,0xff=没测试
//1BYTE 开关量输入5 0=OK,1=ERR,0xff=没测试
//1BYTE 开关量输入6 0=OK,1=ERR,0xff=没测试
//1BYTE 开关量输入7 0=OK,1=ERR,0xff=没测试
//1BYTE 开关量输入8 0=OK,1=ERR,0xff=没测试
#define LEN_BASE_Characteristics     (OFFSET_SWIN_Characteristics+8)

#define ADDR_Characteristics_BASE2    ADDR_Characteristics_BASE1+LEN_BASE_Characteristics//按温度区分的测试号:1=低温(T<-5)

#define ADDR_Characteristics_BASE3    ADDR_Characteristics_BASE2+LEN_BASE_Characteristics//按温度区分的测试号:2=高温（T>45)
//电气性能测试记录结束-----------------------------------------------------------------------------------




#define ADDR_ICERROR_IEEPROM_End     (ADDR_Characteristics_End)
//硬件测试记录结束--------------------------------------------------------------------------------



#define ADDR_IEEPROM_End     (ADDR_ICERROR_IEEPROM_End)


#if ((ADDR_IEEPROM_End-ADDR_NANDFLASH_BLOCK_REPLACE)>0x4000)
#error ADDR_ICERROR_IEEPROM Over
#endif








#endif//#ifndef IEEPROM_H





