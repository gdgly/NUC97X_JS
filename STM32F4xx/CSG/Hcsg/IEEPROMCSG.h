
//EEPROM
#ifndef IEEPROM_H
#define IEEPROM_H




//---校表参数------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
#define ADDR_MeterCalibration_IEEPROM_START     0x08008000//16k
#endif

#ifdef IC_NUC9xx
#define ADDR_SPIFLASH_START     0x10000000
//0x10000000-0x102FFFFF 3MB程序存贮区
//0x10300000-0x103BFFFF 768K MS,OS,Router更新时备份或下载传输暂存
//0x103C0000-0x103FFFFF 256K IEEPROM(有DDR2映射)
#define ADDR_UPDATEBACK     0x10300000//MS,OS,Router更新时备份或下载传输暂存
#define LEN_UPDATEBACK     (768*1024)
#define ADDR_IEEPROM_START     (ADDR_SPIFLASH_START+0x003C0000)
#define ADDR_MeterCalibration_IEEPROM_START     ADDR_IEEPROM_START//256k
#endif

#if IC_90E36==1
//4BYTE CRC32
//4BYTE 校表参数有效标志固定为0x12345678
//2BYTE RTCCAL
//2BYTE 温度CAL
#define ADDR_ADC4MA     ADDR_MeterCalibration_IEEPROM_START+12
//2BYTE 直流模拟量电流环4MA输入时ADC转换标准值
#define ADDR_ADC20MA     ADDR_MeterCalibration_IEEPROM_START+14
//2BYTE 直流模拟量电流环20MA输入时ADC转换标准值
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
#define LEN_MeterCalibration_IEEPROM     (2*128)//校表参数总长度=128*2字节
#if ((ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START)>LEN_MeterCalibration_IEEPROM)
#error ADDR_MeterCalibration_IEEPROM_End Over
#endif
#endif//#if IC_90E36==1


#if IC_ATT7022==1
//4BYTE CRC32
//4BYTE 校表参数有效标志固定为0x12345678
//2BYTE RTCCAL
//2BYTE 温度CAL(ATT7022在温度校定时的当前温度值xx度(hex,总是正温度值)
#define ADDR_ADC4MA     ADDR_MeterCalibration_IEEPROM_START+12
//2BYTE 直流模拟量电流环4MA输入时ADC转换标准值
#define ADDR_ADC20MA     ADDR_MeterCalibration_IEEPROM_START+14
//2BYTE 直流模拟量电流环20MA输入时ADC转换标准值
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
//2BYTE reserved
#define ADDR_EMU_01     ADDR_MeterCalibration_IEEPROM_START+8+16
//模式配置寄存器（地址：0x01）(复位:0x89aa)
#define ADDR_EMU_02     ADDR_EMU_01+2
//ADC增益配置寄存器（地址：0x02）(复位:0x0000)
#define ADDR_EMU_03     ADDR_EMU_02+2
//EMU单元配置（地址：0x03）(复位:0x0804)
#define ADDR_EMU_04     ADDR_EMU_03+2
//在功率因数cos(φ)=1时进行功率增益校正
#define ADDR_EMU_05     ADDR_EMU_04+2
//05H w_PgainB 2 0x0000 B相有功功率增益
#define ADDR_EMU_06     ADDR_EMU_05+2
//06H w_PgainC 2 0x0000 C相有功功率增益
#define ADDR_EMU_07     ADDR_EMU_06+2
//07H w_QgainA 2 0x0000 A相无功功率增益
#define ADDR_EMU_08     ADDR_EMU_07+2
//08H w_QgainB 2 0x0000 B相无功功率增益
#define ADDR_EMU_09     ADDR_EMU_08+2
//09H w_QgainC 2 0x0000 C相无功功率增益
#define ADDR_EMU_0A     ADDR_EMU_09+2
//0AH w_SgainA 2 0x0000 A相视在功率增益
#define ADDR_EMU_0B     ADDR_EMU_0A+2
//0BH w_SgainB 2 0x0000 B相视在功率增益
#define ADDR_EMU_0C     ADDR_EMU_0B+2
//0CH w_SgainC 2 0x0000 C相视在功率增益
#define ADDR_EMU_0D     ADDR_EMU_0C+2
//在功率因数cos(φ)=1时，功率增益已经校正好之后，再进行相位补偿，相位校正在cos(φ)=0.5L时进行校正。
//0DH w_PhSregApq0 2 0x0000 A相相位校正0
#define ADDR_EMU_0E     ADDR_EMU_0D+2
//0EH w_PhSregBpq0 2 0x0000 B相相位校正0
#define ADDR_EMU_0F     ADDR_EMU_0E+2
//0FH w_PhSregCpq0 2 0x0000 C相相位校正0
#define ADDR_EMU_10     ADDR_EMU_0F+2
//10H w_PhSregApq1 2 0x0000 A相相位校正1
#define ADDR_EMU_11     ADDR_EMU_10+2
//11H w_PhSregBpq1 2 0x0000 B相相位校正1
#define ADDR_EMU_12     ADDR_EMU_11+2
//12H w_PhSregCpq1 2 0x0000 C相相位校正1
#define ADDR_EMU_13     ADDR_EMU_12+2
//在功率增益校正及相位校正后，进行功率offset校正，输入小信号x%Ib（5%或者2%）点的电表误差为Err%
//13H w_PoffsetA 2 0x0000 A相有功功率offset校正
#define ADDR_EMU_14     ADDR_EMU_13+2
//14H w_PoffsetB 2 0x0000 B相有功功率offset校正
#define ADDR_EMU_15     ADDR_EMU_14+2
//15H w_PoffsetC 2 0x0000 C相有功功率offset校正
#define ADDR_EMU_16     ADDR_EMU_15+2
//基波无功相位校正
//16H w_QPhscal 2 0x0000 基波无功相位校正
#define ADDR_EMU_17     ADDR_EMU_16+2
//17H w_UgainA 2 0x0000 A相电压增益
#define ADDR_EMU_18     ADDR_EMU_17+2
//18H w_UgainB 2 0x0000 B相电压增益
#define ADDR_EMU_19     ADDR_EMU_18+2
//19H w_UgainC 2 0x0000 C相电压增益
#define ADDR_EMU_1A     ADDR_EMU_19+2
//1AH w_IgainA 2 0x0000 A相电流增益
#define ADDR_EMU_1B     ADDR_EMU_1A+2
//1BH w_IgainB 2 0x0000 B相电流增益
#define ADDR_EMU_1C     ADDR_EMU_1B+2
//1CH w_IgainC 2 0x0000 C相电流增益
#define ADDR_EMU_1D     ADDR_EMU_1C+2
//起动电流阈值设置
#define ADDR_EMU_1E     ADDR_EMU_1D+2
//参数HFconst决定校表用的高频脉冲输出CF的频率，HFconst不能写入大于0x000D00，或小于0x000002的参数值。
//输入最大正弦信号(满量程)有效值是0.5V，建议将电压通道Un 对应到ADC 的输入选在有效值0.22V左右，而电流通道Ib 时的ADC输入选在有效值0.05V 左右
#define ADDR_EMU_1F     ADDR_EMU_1E+2
//失压阈值是根据校正后的电压有效值进行设定的。
#define ADDR_EMU_20     ADDR_EMU_1F+2
//20H w_GainADC7 2 0x0000 第七路ADC输入信号增益
#define ADDR_EMU_21     ADDR_EMU_20+2
//21H w_QoffsetA 2 0x0000 A相无功功率offset校正
#define ADDR_EMU_22     ADDR_EMU_21+2
//22H w_QoffsetB 2 0x0000 B相无功功率offset校正
#define ADDR_EMU_23     ADDR_EMU_22+2
//23H w_QoffsetC 2 0x0000 C相无功功率offset校正
#define ADDR_EMU_24     ADDR_EMU_23+2
//24H w_UaRmsoffse 2 0x0000 A相电压有效值offset校正
#define ADDR_EMU_25     ADDR_EMU_24+2
//25H w_UbRmsoffse 2 0x0000 B相电压有效值offset校正
#define ADDR_EMU_26     ADDR_EMU_25+2
//26H w_UcRmsoffse 2 0x0000 C相电压有效值offset校正
#define ADDR_EMU_27     ADDR_EMU_26+2
//27H w_IaRmsoffse 2 0x0000 A相电流有效值offset校正
#define ADDR_EMU_28     ADDR_EMU_27+2
//28H w_IbRmsoffse 2 0x0000 B相电流有效值offset校正
#define ADDR_EMU_29     ADDR_EMU_28+2
//29H w_IcRmsoffse 2 0x0000 C相电流有效值offset校正
#define ADDR_EMU_2A     ADDR_EMU_29+2
//注意：ADC实时采样数据为19位，且高位补符号位，而ADC offset寄存器为16位，即ADC offset与ADC采样数据19位中的高16位对齐
//2AH w_UoffsetA 2 0x0000 A相电压通道ADC offset校正
#define ADDR_EMU_2B     ADDR_EMU_2A+2
//2BH w_UoffsetB 2 0x0000 B相电压通道ADC offset校正
#define ADDR_EMU_2C     ADDR_EMU_2B+2
//2CH w_UoffsetC 2 0x0000 C相电压通道ADC offset校正
#define ADDR_EMU_2D     ADDR_EMU_2C+2
//2DH w_IoffsetA 2 0x0000 A相电流通道ADC offset校正
#define ADDR_EMU_2E     ADDR_EMU_2D+2
//2EH w_IoffsetB 2 0x0000 B相电流通道ADC offset校正
#define ADDR_EMU_2F     ADDR_EMU_2E+2
//2FH w_IoffsetC 2 0x0000 C相电流通道ADC offset校正
#define ADDR_EMU_30     ADDR_EMU_2F+2
//30H w_EMUIE 2 0x0001 中断使能
#define ADDR_EMU_31     ADDR_EMU_30+2
//模拟模块使能寄存器(复位0x4527)
#define ADDR_EMU_32     ADDR_EMU_31+2
//32H w_AllGain 2 0x0000 全通道增益，用于Vref的温度校正
#define ADDR_EMU_33     ADDR_EMU_32+2
//33H w_HFDouble 2 0x0000 脉冲常数加倍选择
#define ADDR_EMU_34     ADDR_EMU_33+2
//对基波增益进行补偿，在基波增益寄存器LineGain(0x34)=0x2C59(复位值)情况下进行
//34H w_LineGain 2 0x2C59 基波增益校正
#define ADDR_EMU_35     ADDR_EMU_34+2
//IO状态配置寄存器(地址：0x35)(复位0x080f)
//35H w_PinCtrl 2 0x000F 数字pin上下拉电阻选择控制
#define ADDR_EMU_36     ADDR_EMU_35+2
//起动功率寄存器 (地址：0x36)
#define ADDR_EMU_37     ADDR_EMU_36+2
//37 w_Iregion 2 0x7FFF 相位补偿区域设置寄存器
#define ADDR_EMU_38     ADDR_EMU_37+2
//SAG数据长度设置寄存器 0x1000
#define ADDR_EMU_39     ADDR_EMU_38+2
//SAG检测阈值设置寄存器 0x4500

#define ADDR_EMU_60     ADDR_EMU_39+2
//60 w_Iregion1 2 0x0000 相位补偿区域设置寄存器1
#define ADDR_EMU_61     ADDR_EMU_60+2
//61 w_PhSregApq2 2 0x0000 A相相位校正2
#define ADDR_EMU_62     ADDR_EMU_61+2
//62 w_PhSregBpq2 2 0x0000 B相相位校正2
#define ADDR_EMU_63     ADDR_EMU_62+2
//63 w_PhSregCpq2 2 0x0000 C相相位校正2
#define ADDR_EMU_64     ADDR_EMU_63+2
//64 w_PoffsetAL 2 0x0000 A相有功功率offset校正低字节
#define ADDR_EMU_65     ADDR_EMU_64+2
//65 w_PoffsetBL 2 0x0000 B相有功功率offset校正低字节
#define ADDR_EMU_66     ADDR_EMU_65+2
//66 w_PoffsetCL 2 0x0000 C相有功功率offset校正低字节
#define ADDR_EMU_67     ADDR_EMU_66+2
//67 w_QoffsetAL 2 0x0000 A相无功功率offset校正低字节
#define ADDR_EMU_68     ADDR_EMU_67+2
//68 w_QoffsetBL 2 0x0000 B相无功功率offset校正低字节
#define ADDR_EMU_69     ADDR_EMU_68+2
//69 w_QoffsetCL 2 0x0000 C相无功功率offset校正低字节
#define ADDR_EMU_6A     ADDR_EMU_69+2
//6A w_ItRmsoffset 2 0x0000 电流矢量和offset校正寄存器
#define ADDR_EMU_6B     ADDR_EMU_6A+2
//6B w_TPSoffset 2 0x0000 TPS初值校正寄存器
#define ADDR_EMU_6C     ADDR_EMU_6B+2
//6C w_TPSgain 2 0x0000 TPS斜率校正寄存器
#define ADDR_EMU_6D     ADDR_EMU_6C+2
//6D w_TCcoffA 2 0xFEFF Vrefgain的二次系数
#define ADDR_EMU_6E     ADDR_EMU_6D+2
//6E w_TCcoffB 2 0xEF7A Vrefgain的一次系数
#define ADDR_EMU_6F     ADDR_EMU_6E+2
//6F w_TCcoffC 2 0x047C Vrefgain的常数项
#define ADDR_EMU_70     ADDR_EMU_6F+2
//70 w_EMCfg 2 0x0000 新增算法控制寄存器
#define ADDR_EMU_71     ADDR_EMU_70+2
//71 w_OILVL 2 0x0000 过流阈值设置寄存器


#define ADDR_MeterCalibration_IEEPROM_End     (ADDR_EMU_71+2)
#define LEN_MeterCalibration_IEEPROM     (2*128)//校表参数总长度=128*2字节
#if ((ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START)>LEN_MeterCalibration_IEEPROM)
#error ADDR_MeterCalibration_IEEPROM_End Over
#endif
#endif//#if IC_ATT7022==1

//校表参数结束--------------------------------------------------------------------------------


//电能表误差检定记录数据---------------------------------------------------------
#define ADDR_MeterError_IEEPROM_Start     (ADDR_MeterCalibration_IEEPROM_START+LEN_MeterCalibration_IEEPROM)
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


#if ((ADDR_MeterError_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START)>0x4000)
#error ADDR_MeterError_IEEPROM Over
#endif



//NANDFLASH块替换----------------------------------------------------------------------------------------------
#ifdef IC_STM32F4xx
#define ADDR_NANDFLASH_BLOCK_REPLACE     0x0800C000//16k   NANDFLASH块替换
#endif
#ifdef IC_NUC9xx
#define ADDR_NANDFLASH_BLOCK_REPLACE     (ADDR_IEEPROM_START+0x00004000)//16k   NANDFLASH块替换
#endif
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
//NANDFLASH块替换结束--------------------------------------------------------------------------------------------------



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




#define ADDR_ICERROR_IEEPROM_End     (ADDR_Characteristics_BASE3+LEN_BASE_Characteristics)
//硬件测试记录结束--------------------------------------------------------------------------------



#define ADDR_IEEPROM_End     (ADDR_ICERROR_IEEPROM_End)


#if ((ADDR_IEEPROM_End-ADDR_NANDFLASH_BLOCK_REPLACE)>0x4000)
#error ADDR_ICERROR_IEEPROM Over
#endif








#endif//#ifndef IEEPROM_H





