
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/Driver/NUC97x_ATT7022.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SPI.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_IO.h"
#include "../../STM32F4xx/Device/MEMRW.h"
#include "../../STM32F4xx/STM32F4xx/STM32F4xx_SoftDelay.h"
#include "../NUC97x/NUC97x_TMR.h"



#if IC_ATT7022==1//0=没,1=有

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

__align(4) const u16 MeterCalInitLib[]=//校表参数初始化库
{
//电表校表参数------------------------------------------------------------------------------------
//#define ADDR_MeterCalibration_IEEPROM_Start     (ADDR_IEEPROM_Start)
	0,0,//4BYTE CRC32
	0x12345678&0xffff,0x12345678>>16,//4BYTE 电表校表参数有效标志固定为0x12345678
	0,//2BYTE RTCCAL
	25,//2BYTE 温度CAL(ATT7022在温度校定时的当前温度值xx度(hex,总是正温度值)
	0,//2BYTE reserved
	0,//2BYTE reserved
	0,//2BYTE reserved
	0,//2BYTE reserved
	0,//2BYTE reserved
	0,//2BYTE reserved
//#define ADDR_EMU_01     ADDR_MeterCalibration_IEEPROM_Start+8+16
//模式配置寄存器（地址：0x01）(复位:0x89aa)
	//Bit00 =1表示开启零线电流I0通道adc；=0关闭。
	//Bit01 =1表示开启Ia通道adc；=0关闭。
	//Bit02 =1表示开启Ua通道adc；=0关闭。
	//Bit03 =1表示开启Ib通道adc；=0关闭。
	//Bit04 =1表示开启Ub通道adc；=0关闭。
	//Bit05 =1表示开启Ic通道adc；=0关闭。
	//Bit06 =1表示开启Uc通道adc；=0关闭。
	//Bit07 =1表示开启adc的chop功能；=0关闭。推荐关闭，配置为0。
	//Bit09 Bit08 SampleR1/0：用于选择femu时钟
		//00=1.8432MHz 01=921.6kHz 1x=460.8kHz
	//Bit11 Bit10 CIB_ADC1/0：用于选择iref偏置电流
		//11=10uA 10=7.5uA 01=5uA 00=5uA 在降低芯片功耗与得到更好的ADC性能矛盾，折中推荐0x10选择7.5uA
	//Bit12 有效值和功率的更新速率选择，=1表示慢速(1.76Hz)；=0快速(14.4Hz)。
		//正常运用时，为得到稳定的有效值与功率值，推荐慢速方式；
		//在全失压模式下，为快速得到电流有效值，推荐选用快速。
		//Femu=1.8432MHz时，更新速率慢速为3.52Hz，快速为28.8Hz
	//Bit13 选择有效值的稳定时间，=1表示慢速，跳动小；=0快速，跳动大。
		//正常运用时，为得到稳定的有效值，推荐慢速方式；
		//在全失压模式下，为快速得到电流有效值，推荐选用快速。
	//Bit14 三相三线时Ub有效值数据源选择，=1表示内部(ua-uc)；=0表示ub通道。
	//Bit15 =1表示开启ref的chop功能；=0关闭。为得到更稳定的Vref，推荐打开。
	0xB97E,//(0x8000+(1<<13)+(1<<12)+(3<<10)+(1<<8)+0x7e),
//#define ADDR_EMU_02     ADDR_EMU_01+2
//ADC增益配置寄存器（地址：0x02）(复位:0x0000)
	//Bit01 Bit00 表示零线电流I0通道ADC增益放大，00/01/10/11分别表示为1/2/8/16倍增益
	//Bit03 Bit02 表示A相电流通道ADC增益放大，00/01/10/11分别表示为1/2/8/16倍增益
	//Bit05 Bit04 表示B相电流通道ADC增益放大，00/01/10/11分别表示为1/2/8/16倍增益
	//Bit07 Bit06 表示C相电流通道ADC增益放大，00/01/10/11分别表示为1/2/8/16倍增益
	//Bit09 Bit08 表示三相电压通道ADC增益放大，00/01/10/11分别表示为1/2/8/8倍增益
	0,
//#define ADDR_EMU_03     ADDR_EMU_02+2
//EMU单元配置（地址：0x03）(复位:0x0804)
	//Bit02 Bit01 Bi00 SPL[2:0]：波形采样频率选择，当fosc=5.5296M，femu=921.6kHz时，选择频率如下：
		//1XX 011 010 001 000
		//14.4K 7.2K 3.6K 1.8K 0.9K
		//当femu=1.8432MHz /460.8kHz时，选择的波形采样频率与上表成正向比例变化。
	//Bit03 =1， 视在功率/能量寄存器采用RMS方式计量；
		//=0，视在功率/能量寄存器采用PQS方式计量。
	//Bit05 Bit04 ZXD：选择电压过零中断方式
		//00 01 1X
		//正向过0中断/负向过0中断/ 双向过0中断
	//Bit06 =1，三相四线制使用代数和累加方式，三相三线下使用绝对值和累加方式；
		//=0，三相四线制使用绝对值和累加方式，三相三线下使用代数和累加方式。
	//Bit07 =1，能量寄存器读后清0；
		//=0能量寄存器读后不清0。
	//Bit09 Bit08 WaveSel[1:0]：波形缓冲数据源选择，
		//=00，选择ADC采样数据来源于未经高通的原始数据；
		//=01，选择ADC采样数据来源于经高通且增益校正后的数据；
		//=1x，选择ADC采样数据来源于经基波滤波器后的数据。
	//Bit10 =1，开启基波/谐波计量功能；
		//=0，关闭基波/谐波计量功能。
		//需通过寄存器(校表参数0x70 bit5)EnHarmonic进行基波测量和谐波测量的切换 该bit为ATT7022E专有，ATT7026E/28E无效。
	//Bit11 =1，选择功率作为潜动起动判断依据；
		//=0，选择电流有效值作为潜动起动判断依据。推荐使用功率作为潜动起动判断依据。
	//Bit12 =1，开启有功能量CF1通路能量计量功能；=0，关闭CF1计量功能。
	//Bit13 =1，开启无功能量CF2通路能量计量功能；=0，关闭CF2计量功能。
	//Bit14 =1，开启视在能量CF3通路能量计量功能；=0，关闭CF3计量功能。
	//Bit15 =1，开启基波有功能量CF4通路能量计量功能；=0，关闭CF4计量功能。
	0xF884,//0x3c84,
//#define ADDR_EMU_04     ADDR_EMU_03+2
//在功率因数cos(φ)=1时进行功率增益校正，其中有功功率增益校正寄存器与无功功率增益校正寄存器写入同一个校正值，
//视在功率增益校正寄存器在Smode=0选择PQS方式计量时，可以不校正，但在Smode=1选择RMS方式计量时，需要校正，校正值与有功/无功功率增益值相同。
	0x0B21,//04H w_PgainA 2 0x0000 A相有功功率增益
//#define ADDR_EMU_05     ADDR_EMU_04+2
	0x0B21,//05H w_PgainB 2 0x0000 B相有功功率增益
//#define ADDR_EMU_06     ADDR_EMU_05+2
	0x0B21,//06H w_PgainC 2 0x0000 C相有功功率增益
//#define ADDR_EMU_07     ADDR_EMU_06+2
	0x0B21,//07H w_QgainA 2 0x0000 A相无功功率增益
//#define ADDR_EMU_08     ADDR_EMU_07+2
	0x0B21,//08H w_QgainB 2 0x0000 B相无功功率增益
//#define ADDR_EMU_09     ADDR_EMU_08+2
	0x0B21,//09H w_QgainC 2 0x0000 C相无功功率增益
//#define ADDR_EMU_0A     ADDR_EMU_09+2
	0x0B21,//0AH w_SgainA 2 0x0000 A相视在功率增益
//#define ADDR_EMU_0B     ADDR_EMU_0A+2
	0x0B21,//0BH w_SgainB 2 0x0000 B相视在功率增益
//#define ADDR_EMU_0C     ADDR_EMU_0B+2
	0x0B21,//0CH w_SgainC 2 0x0000 C相视在功率增益
//#define ADDR_EMU_0D     ADDR_EMU_0C+2
//在功率因数cos(φ)=1时，功率增益已经校正好之后，再进行相位补偿，相位校正在cos(φ)=0.5L时进行校正。
	0xFFDA,//0DH w_PhSregApq0 2 0x0000 A相相位校正0
//#define ADDR_EMU_0E     ADDR_EMU_0D+2
	0xFFDA,//0EH w_PhSregBpq0 2 0x0000 B相相位校正0
//#define ADDR_EMU_0F     ADDR_EMU_0E+2
	0xFFDA,//0FH w_PhSregCpq0 2 0x0000 C相相位校正0
//#define ADDR_EMU_10     ADDR_EMU_0F+2
	0xFFDA,//10H w_PhSregApq1 2 0x0000 A相相位校正1
//#define ADDR_EMU_11     ADDR_EMU_10+2
	0xFFDA,//11H w_PhSregBpq1 2 0x0000 B相相位校正1
//#define ADDR_EMU_12     ADDR_EMU_11+2
	0xFFDA,//12H w_PhSregCpq1 2 0x0000 C相相位校正1
//#define ADDR_EMU_13     ADDR_EMU_12+2
//在功率增益校正及相位校正后，进行功率offset校正，输入小信号x%Ib（5%或者2%）点的电表误差为Err%
	0,//13H w_PoffsetA 2 0x0000 A相有功功率offset校正
//#define ADDR_EMU_14     ADDR_EMU_13+2
	0,//14H w_PoffsetB 2 0x0000 B相有功功率offset校正
//#define ADDR_EMU_15     ADDR_EMU_14+2
	0,//15H w_PoffsetC 2 0x0000 C相有功功率offset校正
//#define ADDR_EMU_16     ADDR_EMU_15+2
//基波无功相位校正
//默认值对应于femu=921.6K时的情况，不需要再校正；femu为其他频率、或测量的工频频率不为
//50Hz时需要按照下面的公式进行校正：只用于无功选择为基波无功时使用，无功选择全波无功时不需校正。
//在30度时进行校正
	0,//16H w_QPhscal 2 0x0000 基波无功相位校正
//#define ADDR_EMU_17     ADDR_EMU_16+2
	0xCEEA,//17H w_UgainA 2 0x0000 A相电压增益
//#define ADDR_EMU_18     ADDR_EMU_17+2
	0xCEEA,//18H w_UgainB 2 0x0000 B相电压增益
//#define ADDR_EMU_19     ADDR_EMU_18+2
	0xCEEA,//19H w_UgainC 2 0x0000 C相电压增益
//#define ADDR_EMU_1A     ADDR_EMU_19+2
	0x07BD,//1AH w_IgainA 2 0x0000 A相电流增益
//#define ADDR_EMU_1B     ADDR_EMU_1A+2
	0x07BD,//1BH w_IgainB 2 0x0000 B相电流增益
//#define ADDR_EMU_1C     ADDR_EMU_1B+2
	0x07BD,//1CH w_IgainC 2 0x0000 C相电流增益
//#define ADDR_EMU_1D     ADDR_EMU_1C+2
//起动电流阈值设置
	//计算公式：Istartup=INT[0.8*Io*2^13]
	//其中Io=Ib*N*比例设置点(额定电流对应取样信号为25mV，则N=30/Ib；额定电流对应取样信号为
	//50mV，则N=60/Ib；) 例如，启动电流设置为0.4%，Ib=1.5A取样信号50mV，则Io=1.5*40*0.4%。
	//N——与电流有效值计算公式中的系数N相同。
	//配置起动阈值时，需要同时写起动电流阈值(校表参数0x1D)与起动功率阈值(校表参数0x36)
	Istartup,
//#define ADDR_EMU_1E     ADDR_EMU_1D+2
//参数HFconst决定校表用的高频脉冲输出CF的频率，HFconst不能写入大于0x000D00，或小于0x000002的参数值。
//输入最大正弦信号(满量程)有效值是0.5V，建议将电压通道Un 对应到ADC 的输入选在有效值0.22V左右，而电流通道Ib 时的ADC输入选在有效值0.05V 左右
	HFConst,//HFConst＝INT[2.592*10^10*G*G*Vu*Vi/(EC*Un*Ib)]
//#define ADDR_EMU_1F     ADDR_EMU_1E+2
//失压阈值是根据校正后的电压有效值进行设定的。
	//具体公式为：失压阈值FailVoltage=Un*2^5*D
	//Un：表示校正后的电压有效值
	//D： 表示失压电压百分比
	MeterUBhex*12,//(MeterUBhex*0x20*0.6)
//#define ADDR_EMU_20     ADDR_EMU_1F+2
	0,//20H w_GainADC7 2 0x0000 第七路ADC输入信号增益
//#define ADDR_EMU_21     ADDR_EMU_20+2
	0,//21H w_QoffsetA 2 0x0000 A相无功功率offset校正
//#define ADDR_EMU_22     ADDR_EMU_21+2
	0,//22H w_QoffsetB 2 0x0000 B相无功功率offset校正
//#define ADDR_EMU_23     ADDR_EMU_22+2
	0,//23H w_QoffsetC 2 0x0000 C相无功功率offset校正
//#define ADDR_EMU_24     ADDR_EMU_23+2
	0,//24H w_UaRmsoffse 2 0x0000 A相电压有效值offset校正
//#define ADDR_EMU_25     ADDR_EMU_24+2
	0,//25H w_UbRmsoffse 2 0x0000 B相电压有效值offset校正
//#define ADDR_EMU_26     ADDR_EMU_25+2
	0,//26H w_UcRmsoffse 2 0x0000 C相电压有效值offset校正
//#define ADDR_EMU_27     ADDR_EMU_26+2
	0,//27H w_IaRmsoffse 2 0x0000 A相电流有效值offset校正
//#define ADDR_EMU_28     ADDR_EMU_27+2
	0,//28H w_IbRmsoffse 2 0x0000 B相电流有效值offset校正
//#define ADDR_EMU_29     ADDR_EMU_28+2
	0,//29H w_IcRmsoffse 2 0x0000 C相电流有效值offset校正
//#define ADDR_EMU_2A     ADDR_EMU_29+2
//注意：ADC实时采样数据为19位，且高位补符号位，而ADC offset寄存器为16位，即ADC offset与ADC采样数据19位中的高16位对齐
	0,//2AH w_UoffsetA 2 0x0000 A相电压通道ADC offset校正
//#define ADDR_EMU_2B     ADDR_EMU_2A+2
	0,//2BH w_UoffsetB 2 0x0000 B相电压通道ADC offset校正
//#define ADDR_EMU_2C     ADDR_EMU_2B+2
	0,//2CH w_UoffsetC 2 0x0000 C相电压通道ADC offset校正
//#define ADDR_EMU_2D     ADDR_EMU_2C+2
	0,//2DH w_IoffsetA 2 0x0000 A相电流通道ADC offset校正
//#define ADDR_EMU_2E     ADDR_EMU_2D+2
	0,//2EH w_IoffsetB 2 0x0000 B相电流通道ADC offset校正
//#define ADDR_EMU_2F     ADDR_EMU_2E+2
	0,//2FH w_IoffsetC 2 0x0000 C相电流通道ADC offset校正
//#define ADDR_EMU_30     ADDR_EMU_2F+2
	1,//30H w_EMUIE 2 0x0001 中断使能
//#define ADDR_EMU_31     ADDR_EMU_30+2
//模拟模块使能寄存器(复位0x4527)
	//Bit00 =1，开启第七通道数字高通滤波器；=0关闭。
	//Bit01 =1，开启电流通道数字高通滤波器；=0关闭。
	//Bit02 =1，开启电压通道数字高通滤波器；=0关闭。
	//Bit03 =1，选择使能电流通道的罗氏线圈功能；=0关闭。
	//Bit04 =1，选择开启TPS功能；=0关闭。
	//Bit05 =1，选择开启BOR功能；=0关闭。
	//Bit06 =1，中断选择高电平有效；=0低电平有效。
	//Bit07 温度传感器类型选择信号，=0，选择PN传感器(推荐)；=1，选用电阻传感器。
	//Bit11 =1, 选择SPI低速模式(SCLK频率0~1.4MHz)； =0 选择SPI 高速模式(SCLK频率0~10MHz)
	//Bit15~12,10~8 模拟性能配置，推荐配置为0011，100
	0x3437,//31H w_ModuleCFG
//#define ADDR_EMU_32     ADDR_EMU_31+2
	0,//32H w_AllGain 2 0x0000 全通道增益，用于Vref的温度校正
//#define ADDR_EMU_33     ADDR_EMU_32+2
	//HFDouble寄存器内容脉冲常数放大倍率
	//0x33CC 脉冲常数放大2倍
	//0x33CD 脉冲常数放大4倍
	//0x33CE 脉冲常数放大8倍
	//0x33CF 脉冲常数放大16倍
	//其他值 脉冲常数不变
	0,//33H w_HFDouble 2 0x0000 脉冲常数加倍选择
//#define ADDR_EMU_34     ADDR_EMU_33+2
//对基波增益进行补偿，在基波增益寄存器LineGain(0x34)=0x2C59(复位值)情况下进行
	0x2c59,//34H w_LineGain 2 0x2C59 基波增益校正
//#define ADDR_EMU_35     ADDR_EMU_34+2
//IO状态配置寄存器(地址：0x35)(复位0x080f)
	0x000F,//0x080F,//35H w_PinCtrl 2 0x000F 数字pin上下拉电阻选择控制
//#define ADDR_EMU_36     ADDR_EMU_35+2
//起动功率寄存器 (地址：0x36)
//已知：额定电压Ub，基本电流Ib，启动电流点k‰
//计算公式： Pstartup=INT[0.6*Ub*Ib*HFconst*EC* k‰*2^23/(2.592*10^10)]
//HFconst计算公式：
//HFConst＝INT[2.592*10^10*G*G*Vu*Vi/(EC*Un*Ib)]
//注：上式中G=1.163
	Pstartup,
//#define ADDR_EMU_37     ADDR_EMU_36+2
	0x7fff,//37 w_Iregion 2 0x7FFF 相位补偿区域设置寄存器
//#define ADDR_EMU_38     ADDR_EMU_37+2
	//SAG数据长度设置寄存器(0x38)
	0x1000,
//#define ADDR_EMU_39     ADDR_EMU_38+2
//SAG检测阈值设置寄存器 0x4500
	0x4500,
//#define ADDR_EMU_60     ADDR_EMU_39+2
//60 w_Iregion1 2 0x0000 相位补偿区域设置寄存器1
	0x0000,
//#define ADDR_EMU_61     ADDR_EMU_60+2
//61 w_PhSregApq2 2 0x0000 A相相位校正2
	0xFFDA,
//#define ADDR_EMU_62     ADDR_EMU_61+2
//62 w_PhSregBpq2 2 0x0000 B相相位校正2
	0xFFDA,
//#define ADDR_EMU_63     ADDR_EMU_62+2
//63 w_PhSregCpq2 2 0x0000 C相相位校正2
	0xFFDA,
//#define ADDR_EMU_64     ADDR_EMU_63+2
//64 w_PoffsetAL 2 0x0000 A相有功功率offset校正低字节
	0x0000,
//#define ADDR_EMU_65     ADDR_EMU_64+2
//65 w_PoffsetBL 2 0x0000 B相有功功率offset校正低字节
	0x0000,
//#define ADDR_EMU_66     ADDR_EMU_65+2
//66 w_PoffsetCL 2 0x0000 C相有功功率offset校正低字节
	0x0000,
//#define ADDR_EMU_67     ADDR_EMU_66+2
//67 w_QoffsetAL 2 0x0000 A相无功功率offset校正低字节
	0x0000,
//#define ADDR_EMU_68     ADDR_EMU_67+2
//68 w_QoffsetBL 2 0x0000 B相无功功率offset校正低字节
	0x0000,
//#define ADDR_EMU_69     ADDR_EMU_68+2
//69 w_QoffsetCL 2 0x0000 C相无功功率offset校正低字节
	0x0000,
//#define ADDR_EMU_6A     ADDR_EMU_69+2
//6A w_ItRmsoffset 2 0x0000 电流矢量和offset校正寄存器
	0x0000,
//#define ADDR_EMU_6B     ADDR_EMU_6A+2
//6B w_TPSoffset 2 0x0000 TPS初值校正寄存器
	0x0000,
//#define ADDR_EMU_6C     ADDR_EMU_6B+2
//6C w_TPSgain 2 0x0000 TPS斜率校正寄存器
	0x0000,
//#define ADDR_EMU_6D     ADDR_EMU_6C+2
//考虑Vref和外围电阻（选用20ppm正温度系数电阻）因素时推荐系数：0x6D=0xFF11；0x6E=0x2B53；0x6F=0xD483
//6D w_TCcoffA 2 0xFEFF Vrefgain的二次系数
	0xFF11,
//#define ADDR_EMU_6E     ADDR_EMU_6D+2
//6E w_TCcoffB 2 0xEF7A Vrefgain的一次系数
	0x2B53,
//#define ADDR_EMU_6F     ADDR_EMU_6E+2
//6F w_TCcoffC 2 0x047C Vrefgain的常数项
	0xD483,
//#define ADDR_EMU_70     ADDR_EMU_6F+2
//70 w_EMCfg 2 0x0000 新增算法控制寄存器
	//Bit00 ModSel：在外部引脚SEL=1时，通过寄存器ModSel控制位进行工作模式判别。ModSel =0为三相四线制；ModSel =1 为三相三线制
	//Bit01 VrefAotu_en：基准自动补偿使能VrefAotu_en=1，使用自动补偿，此时内部计算的allgain = Vrefgain，来自内部计算值，同时写入寄存器VrefGain
					//VrefAotu_en=0 ， 关闭自动补偿， 功能同原ATT7022E, 即内部计算的allgain=Allgain（0x32，来源于校表寄存器）
	//Bit02 ISUMSel：电流矢量算法选择位，公式详见4.2.2章节（page36）ISUMSel=0，采用算法1; ISUMSel=1，采用算法2.
	//Bit03 Ymodsel：角度算法选择位,Ymodsel = 0（old算法一） Ymodsel = 1（new算法二）
				//计量参数地址 Ymodsel = 0（old算法一） Ymodsel = 1（new算法二）
				//0x26	YUaUb	YUa
				//0x27	YUaUc	YUb
				//0x28	YUbUc	YUc
				//0x18	Pga	YIa
				//0x19	Pgb	YIb
				//0x1A	Pgc	YIc
	//Bit04 QEnergySel：无功能量选择寄存器,QEnergySel=0，选择全波无功； QEnergySel=1，选择基波无功；
	//Bit05 EnHarmonic:基波测量和谐波测量切换控制位EnHarmonic=1，选择谐波测量；EnHarmonic=0，选择基波测量。
	//Bit8~7 ChlSel[1..0]夹角选择新算法时选择以哪路信号作为参考。
	//Bit09 Fcntmod：电能及快速脉冲累加方式选择位Fcntmod=0，为正Fcnt+1，为负则Fcnt-1（老方式）Fcntmod=1，全部采用Fcnt+1方式（新方式）
	0x000A,
//#define ADDR_EMU_71     ADDR_EMU_70+2
//71 w_OILVL 2 0x0000 过流阈值设置寄存器	
	0x0000,
	
//#define ADDR_MeterCalibration_IEEPROM_End     (ADDR_EMU_71+2)
};


typedef __packed struct
{
  u16 Addr;//命令和寄存器地址
	u16 RW;//读写:0=读,0x80=写
  u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
}EMU_Adj_List_TypeDef;

__align(4) const EMU_Adj_List_TypeDef   EMU_Adj_List[]=
{
//01H w_ModeCfg 2 0x89AA 模式相关控制
	{
  	0x01,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_01,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//02H w_PGACtrl 2 0x0000 ADC增益配置
	{
  	0x02,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_02,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//03H w_EMUCfg 2 0x0804 EMU单元配置
	{
  	0x03,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_03,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//04H w_PgainA 2 0x0000 A相有功功率增益
	{
  	0x04,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_04,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//05H w_PgainB 2 0x0000 B相有功功率增益
	{
  	0x05,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_05,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//06H w_PgainC 2 0x0000 C相有功功率增益
	{
  	0x06,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_06,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//07H w_QgainA 2 0x0000 A相无功功率增益
	{
  	0x07,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_07,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//08H w_QgainB 2 0x0000 B相无功功率增益
	{
  	0x08,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_08,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//09H w_QgainC 2 0x0000 C相无功功率增益
	{
  	0x09,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_09,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//0AH w_SgainA 2 0x0000 A相视在功率增益
	{
  	0x0A,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_0A,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//0BH w_SgainB 2 0x0000 B相视在功率增益
	{
  	0x0B,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_0B,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//0CH w_SgainC 2 0x0000 C相视在功率增益
	{
  	0x0C,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_0C,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//0DH w_PhSregApq0 2 0x0000 A相相位校正0
	{
  	0x0D,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_0D,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//0EH w_PhSregBpq0 2 0x0000 B相相位校正0
	{
  	0x0E,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_0E,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//0FH w_PhSregCpq0 2 0x0000 C相相位校正0
	{
  	0x0F,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_0F,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//10H w_PhSregApq1 2 0x0000 A相相位校正1
	{
  	0x10,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_10,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//11H w_PhSregBpq1 2 0x0000 B相相位校正1
	{
  	0x11,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_11,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//12H w_PhSregCpq1 2 0x0000 C相相位校正1
	{
  	0x12,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_12,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//13H w_PoffsetA 2 0x0000 A相有功功率offset校正
	{
  	0x13,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_13,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//14H w_PoffsetB 2 0x0000 B相有功功率offset校正
	{
  	0x14,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_14,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//15H w_PoffsetC 2 0x0000 C相有功功率offset校正
	{
  	0x15,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_15,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//16H w_QPhscal 2 0x0000 无功相位校正
	{
  	0x16,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_16,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//17H w_UgainA 2 0x0000 A相电压增益
	{
  	0x17,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_17,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//18H w_UgainB 2 0x0000 B相电压增益
	{
  	0x18,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_18,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//19H w_UgainC 2 0x0000 C相电压增益
	{
  	0x19,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_19,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//1AH w_IgainA 2 0x0000 A相电流增益
	{
  	0x1A,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_1A,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//1BH w_IgainB 2 0x0000 B相电流增益
	{
  	0x1B,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_1B,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//1CH w_IgainC 2 0x0000 C相电流增益
	{
  	0x1C,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_1C,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//1DH w_Istarup 2 0x0160 起动电流阈值设置
	{
  	0x1D,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_1D,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//1EH w_Hfconst 2 0x0500 高频脉冲输出设置;HFconst不能写入大于0x000D00或小于0x000002的参数值
	{
  	0x1E,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_1E,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//1FH w_FailVoltage 2 0x0600 失压阈值设置（三相四线模式）
										//0x1200 失压阈值设置（三相三线模式）
	{
  	0x1F,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_1F,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//20H w_GainADC7 2 0x0000 第七路ADC输入信号增益
	{
  	0x20,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_20,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//21H w_QoffsetA 2 0x0000 A相无功功率offset校正
	{
  	0x21,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_21,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//22H w_QoffsetB 2 0x0000 B相无功功率offset校正
	{
  	0x22,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_22,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//23H w_QoffsetC 2 0x0000 C相无功功率offset校正
	{
  	0x23,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_23,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//24H w_UaRmsoffse 2 0x0000 A相电压有效值offset校正
	{
  	0x24,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_24,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//25H w_UbRmsoffse 2 0x0000 B相电压有效值offset校正
	{
  	0x25,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_25,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//26H w_UcRmsoffse 2 0x0000 C相电压有效值offset校正
	{
  	0x26,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_26,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//27H w_IaRmsoffse 2 0x0000 A相电流有效值offset校正
	{
  	0x27,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_27,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//28H w_IbRmsoffse 2 0x0000 B相电流有效值offset校正
	{
  	0x28,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_28,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//29H w_IcRmsoffse 2 0x0000 C相电流有效值offset校正
	{
  	0x29,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_29,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//2AH w_UoffsetA 2 0x0000 A相电压通道ADC offset校正
	{
  	0x2A,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_2A,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//2BH w_UoffsetB 2 0x0000 B相电压通道ADC offset校正
	{
  	0x2B,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_2B,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//2CH w_UoffsetC 2 0x0000 C相电压通道ADC offset校正
	{
  	0x2C,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_2C,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//2DH w_IoffsetA 2 0x0000 A相电流通道ADC offset校正
	{
  	0x2D,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_2D,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//2EH w_IoffsetB 2 0x0000 B相电流通道ADC offset校正
	{
  	0x2E,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_2E,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//2FH w_IoffsetC 2 0x0000 C相电流通道ADC offset校正
	{
  	0x2F,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_2F,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
/*
//30H w_EMUIE 2 0x0001 中断使能
	{
  	0x30,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_30,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
*/
//31H w_ModuleCFG 2 0x4527 电路模块配置寄存器
	{
  	0x31,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_31,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//32H w_AllGain 2 0x0000 全通道增益，用于Vref的温度校正
	{
  	0x32,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_32,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//33H w_HFDouble 2 0x0000 脉冲常数加倍选择
	{
  	0x33,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_33,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//34H w_LineGain 2 0x2C59 基波增益校正
	{
  	0x34,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_34,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//35H w_PinCtrl 2 0x000F 数字pin上下拉电阻选择控制
	{
  	0x35,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_35,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//#define ADDR_EMU_36     ADDR_EMU_35+2
	{
  	0x36,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_36,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
//#define ADDR_EMU_37     ADDR_EMU_36+2
	{
  	0x37,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_37,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x38,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_38,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x39,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_39,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	
	{
  	0x60,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_60,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x61,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_61,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x62,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_62,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x63,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_63,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x64,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_64,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x65,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_65,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x66,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_66,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x67,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_67,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x68,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_68,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x69,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_69,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x6A,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_6A,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},	
	{
  	0x6B,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_6B,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x6C,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_6C,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x6D,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_6D,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x6E,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_6E,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x6F,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_6F,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x70,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_70,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
	{
  	0x71,//u8 Addr;//命令和寄存器地址
		0x80,//u8 RW;//读写:0=读,0x80=写
  	ADDR_EMU_71,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
/*
//读校表数据校验和
	{
  	0x3E,//u8 Addr;//命令和寄存器地址
		0x0,//u8 RW;//读写:0=读,0x80=写
  	0x0000,//u32 Data;//寄存器参数,若高16位=0则直接用低16位数据;若高16位!=0则为数据地址从存储器读取16位数据
	},
*/
};


void Write_ATT7022(u32 Data)//写ATT7022
{
//	u32 i;
	u32* p32cs;
	u32 *p32ctrl;
	u32 *p32TxData;
	
	Init_SPI2();//SPI2初始化(ATM90E36或 ATT7022)
	p32cs=(u32*)(REG_SPI1_SSR);
	p32cs[0]=1;//cs=0
	p32TxData=(u32*)(REG_SPI1_TX0);
	p32TxData[0]=Data>>24;
	p32ctrl=(u32*)(REG_SPI1_CNTRL);
	p32ctrl[0]=
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		//[2]Tx_NEG
	(1<<1)|//[1]Rx_NEG
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	SoftDelayUS(2);//软件us延时
	p32TxData[0]=Data;
	p32ctrl[0]=
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(24<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		//[2]Tx_NEG
	(1<<1)|//[1]Rx_NEG
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	p32cs[0]=0;//cs=1
}

u32 Read_ATT7022(u32 Addr)//读ATT7022
{
//	u32 i;
	u32* p32cs;
	u32 *p32ctrl;
	u32 *p32TxData;
	u32 *p32RxData;
	
	Init_SPI2();//SPI2初始化(ATM90E36或 ATT7022)
	p32cs=(u32*)(REG_SPI1_SSR);
	p32cs[0]=1;//cs=0
	p32TxData=(u32*)(REG_SPI1_TX0);
	p32TxData[0]=Addr;
	p32ctrl=(u32*)(REG_SPI1_CNTRL);
	p32ctrl[0]=
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(8<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		//[2]Tx_NEG
	(1<<1)|//[1]Rx_NEG
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	SoftDelayUS(2);//软件us延时
	p32RxData=(u32*)(REG_SPI1_RX0);
	p32TxData[0]=0;
	p32ctrl[0]=
	(0<<8)|//[9:8]Tx_NUM:0=1,1=2,2=3,3=4
	(24<<3)|//[7:3]Tx_BIT_LEN:0=32,1-31=1-31
		//[2]Tx_NEG
	(1<<1)|//[1]Rx_NEG
	1;//[0]GO_BUSY
	while(p32ctrl[0]&1);
	Addr=p32RxData[0]&0xffffff;
	p32cs[0]=0;//cs=1
	return Addr;
}

u32 Security_Read_ATT7022(u32 Addr)//安全读ATT7022计量数据,判数据可靠性;返回:数据,=0xffffffff表示读错误
{
	u32 Data1;
	u32 Data2;
	u32 ReNum;
	
	Data1=Read_ATT7022(Addr);//读ATT7022
	ReNum=200;//EMC干忧时防误读，注:200次约需50ms
	while(ReNum)
	{
		Data2=Read_ATT7022(0x2E);//读最后次通信命令
		Data2>>=16;
		if(Addr==Data2)
		{
			break;
		}
		ReNum--;
		if(ReNum==0)
		{
			return 0xffffffff;
		}
		if(Pin_Read(PIN_WKUP)==0)
		{//掉电
			return 0xffffffff;
		}
	}
	ReNum=200;//EMC干忧时防误读，注:200次约需50ms
	while(ReNum--)
	{
		Data2=Read_ATT7022(0x2D);//读最后次通信数据
		if(Data1==Data2)
		{
			return Data1;
		}
		Data1=Data2;
		if(Pin_Read(PIN_WKUP)==0)
		{//掉电
			return 0xffffffff;
		}
	}
	return 0xffffffff;
}









void EMURW(void)//初始化EMU
{
	u32 i;
	u32 x;
	u32 Addr;
	u32 Data;
	u32 ParaCount;
	u32 ReNum;
	
	i=Meter_Ram->EMUInit;
	switch(i)
	{//计量芯片初始化标志:0=关电源(放电),1=开电源,2=开电源延时,3=,4=初始化,5=完成
		case 0://关电源(放电)
			UnInit_SPI2();//缷载SPI2初始化(ATM90E36或 ATT7022),SPI为输入下拉
			OffEMU();//关EMU(复位)
			OffA5V0();//关A5V0电源
			Comm_Ram->SPI2Timer=1000/10;//12 SSP1 每10MS减1定时器
			Meter_Ram->EMUInit++;
			return;
		case 1://开电源
			if(Comm_Ram->SPI2Timer==0)
			{
				OnA5V0();//打开A5V0电源
				Comm_Ram->SPI2Timer=1000/10;//12 SSP1 每10MS减1定时器
				Meter_Ram->EMUInit++;
			}
			return;
		case 2://开电源延时
			if(Comm_Ram->SPI2Timer==0)
			{
				OnEMU();//打开EMU(复位解除)
				Comm_Ram->SPI2Timer=100/10;//12 SSP1 每10MS减1定时器
				Meter_Ram->EMUInit++;
			}
			return;
		case 3:
			if(Comm_Ram->SPI2Timer==0)
			{
				Meter_Ram->EMUParaCount=0;//计量芯片参数初始化计数
				Meter_Ram->SSP1Task1Count=0;//SSP1任务1多个数据连续读计数
				Meter_Ram->SSP1Task0=0;//SSP1任务0:0=空,0x1=等待读,0x2=读开始,0x3=读比较,0x4=读完成,0x5=读错误;0x81=等待写,0x82=写开始,0x83=写比较,0x84=写完成,0x85=写错误
				Meter_Ram->SSP1Task1=0;//SSP1任务1:0=空,0x1=等待读,0x2=读开始,0x3=读比较,0x4=读完成,0x5=读错误;0x81=等待写,0x82=写开始,0x83=写比较,0x84=写完成,0x85=写错误
				Meter_Ram->TxCount=0;//
				Meter_Ram->EMUInit++;
			}
			return;
		case 4://参数初始化
		//实测SCK=500KHz时初始化时间=15.5ms,1Mhz时=7.1ms.5ms,5Mhz时=7.2ms
			if(Comm_Ram->Factory==0x55)//工厂状态
			{
				i=MRR(ADDR_MeterCalibration_IEEPROM_START+4,4);
				if(i!=0x12345678)
				{
					MW((u32)MeterCalInitLib,ADDR_MeterCalibration_IEEPROM_START,ADDR_MeterCalibration_IEEPROM_End-ADDR_MeterCalibration_IEEPROM_START);
				}
			}
//		Init_NUC97x_TMR2();//初始化基本定时器TMR2为从0开始的1MHz加计数(用于测量)
/*
			if(Meter_Ram->PW==0)//接线方式;0=三相四线,1=三相三线;(三相三线低电平,三相四线高电平)
			{
				Pin_Configure(PIN_EMURST,(1<<4)|(1<<11));//输出=1(ATT7022中用于3P4W和3P3W选择)
			}
			else
			{
				Pin_Configure(PIN_EMURST,(1<<4)|(0<<11));//输出=0(ATT7022中用于3P4W和3P3W选择)
			}
*/
			ParaCount=((sizeof(EMU_Adj_List)/sizeof(EMU_Adj_List_TypeDef))/1)+1;//分1次完成参数写
			i=Meter_Ram->EMUParaCount;//计量芯片参数初始化计数
			Write_ATT7022(0xC900005A);//校表数据写允许
			Write_ATT7022(0xC600005A);//校表数据读允许
			while(ParaCount--)
			{
				if(i<(sizeof(EMU_Adj_List)/sizeof(EMU_Adj_List_TypeDef)))
				{
					Addr=EMU_Adj_List[i].Addr;
					Data=MRR(EMU_Adj_List[i].Data,2);
					switch(Addr)
					{
						case 0x03:
							Data|=0x80;//Bit07=1能量寄存器读后清0（在此加入原因是原有部分生产已校验，仅修改参数要校验初始化生效）
							break;
						case 0x35://IO状态配置寄存器,只能写部份位
							Data&=0x000F;//未初始化时会写错误！
							break;
						case 0x64://A相有功功率offset校正低字节
						case 0x65://B相有功功率offset校正低字节
						case 0x66://C相有功功率offset校正低字节
						case 0x67://A相无功功率offset校正低字节
						case 0x68://B相无功功率offset校正低字节
						case 0x69://C相无功功率offset校正低字节
						case 0x6A://电流矢量和offset校正寄存器
							Data&=0xff;
							break;
						case 0x70://新增算法控制寄存器
							//Bit00 ModSel：在外部引脚SEL=1时，通过寄存器ModSel控制位进行工作模式判别。ModSel =0为三相四线制；ModSel =1 为三相三线制
							if(Meter_Ram->PW)//接线方式;0=三相四线,1=三相三线;(三相三线低电平,三相四线高电平)
							{
								Data|=1;//三相三线
							}
							Data&=0x3ff;
							break;
					}
					ReNum=2;
					while(ReNum)
					{
						ReNum--;
						Write_ATT7022(((Addr|0x80)<<24)|Data);
						x=Read_ATT7022(Addr);
						if((x&0xffff)==Data)
						{
							i++;//计量芯片参数初始化计数
							break;
						}
					}
					if(ReNum==0)
					{
						Meter_Ram->EMUParaCount=i;//计量芯片参数初始化计数
					#if (USER/100)!=0//不是电科院
						Comm_Ram->ErrorIC|=(1<<5);//硬件错误标志(置位):B5=EMU 计量芯片
						Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
					#endif
						Meter_Ram->EMUInit=0;//计量芯片初始化标志:0=关电源(放电)
						return;
					}
					continue;
				}
				Write_ATT7022(0xC9000000);//校表数据写关闭
				Write_ATT7022(0xC6000000);//读计量数据使能(读通讯数据备份寄存器有效)
				x=Read_ATT7022(0);
				if((x!=0x7122A0)&&(x!=0x7126A0))
				{
					x=Read_ATT7022(0);
					if((x!=0x7122A0)&&(x!=0x7126A0))
					{
					#if (USER/100)!=0//不是电科院
						Comm_Ram->ErrorIC|=(1<<5);//硬件错误标志(置位):B5=EMU 计量芯片
						Comm_Ram->DisplayAllErrorTimer=10;//上电全显或系统错误显示秒定时器
					#endif
						Meter_Ram->EMUInit=0;//计量芯片初始化标志:0=关电源(放电)
						return;
					}
				}
				x=Read_ATT7022(0x3e);
				Meter_Ram->SSP1Data0=x;//校表数据校验和
				x=Read_ATT7022(0x5e);
				Meter_Ram->SSP1Data1=x;//校表数据校验和
				Meter_Ram->EMUParaCount=i;//计量芯片参数初始化计数
				Comm_Ram->ErrorIC&=~(1<<5);//硬件错误标志(置位):B5=EMU 计量芯片
				Comm_Ram->SPI2Timer=2000/10;//下次参数重写在2秒后(在Meter07_Measure()中检查此值)
				Meter_Ram->EMUInit++;
//		x=Get_NUC97x_TMR2_Count();//取TMR2为从0开始的24位1MHz加计数(用于测量)的计数值,每计数值表示1us
//		if(x)
//		{
//			x=x;
//		}
				return;
			}
			Meter_Ram->EMUParaCount=i;//计量芯片参数初始化计数
			return;
		case 5://完成
//				Meter_Ram->EMUParaCount=0;//计量芯片参数初始化计数
//				Meter_Ram->EMUInit--;
//				return;
			break;
		default:
			Meter_Ram->EMUInit=0;
			return;
	}
	
}

#endif








