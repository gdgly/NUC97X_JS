

#ifndef DL698_DataDef_H
#define DL698_DataDef_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
//数据类型定义
#define DataType_NULL	0//	空	
#define DataType_array	1//	SEQUENCE OF Data（见6.3.3.1　）数组的元素在对象属性或方法的描述中定义	
#define DataType_structure	2//	SEQUENCE OF Data（见6.3.3.1　）结构的元素在对象属性或方法的描述中定义	
#define DataType_bool	3//	布尔值（BOOLEAN）	1或0
#define DataType_bit_string	4//	比特位串（BIT STRING）	
#define DataType_double_long	5//	32比特位整数（Integer32）	-231…231-1
#define DataType_double_long_unsigned	6//	32比特位正整数（double-long-unsigned）	0…232-1
//保留	7-8		
#define DataType_octet_string	9//	8比特位位组（字节）串（OCTET STRING）	
#define DataType_visible_string	10//	ASCII字符串（VisibleString）	
//保留	11		
#define DataType_UTF8_string	12//	UTF-8编码的字符串	
//保留	13-14		
#define DataType_integer	15//	8比特位整数（integer）	-128…127
#define DataType_long	16//	16比特位整数（long）	-32768…32767
#define DataType_unsigned	17//	8比特位正整数（Unsigned8）	0…255
#define DataType_long_unsigned	18//	16比特位正整数（Unsigned16）	0…65535
//保留	19		
#define DataType_long64	20//	64比特位整数（Integer64）	-263…263-1
#define DataType_long64_unsigned	21//	64比特位正整数（Unsigned64）	0…264-1
#define DataType_enum	22//	枚举的元素在对象属性或方法的描述中定义	0…255
#define DataType_float32	23//	octet-string（SIZE（4））
#define DataType_float64	24//	octet-string（SIZE（8））
#define DataType_date_time	25//	octet-string（SIZE（10））
#define DataType_date	26//	octet-string（SIZE（5））
#define DataType_time	27//	octet-string（SIZE（3））
#define DataType_date_time_s	28//octet-string（SIZE（7））

//保留	29-79	
#define DataType_OI	80//
#define DataType_OAD	81//
#define DataType_ROAD	82//
#define DataType_OMD	83//
#define DataType_TI	84//
#define DataType_TSA	85//
#define DataType_MAC	86//
#define DataType_RN	87//
#define DataType_Region	88//
#define DataType_Scaler_Unit	89//
#define DataType_RSD	90//
#define DataType_CSD	91//
#define DataType_MS	92//
#define DataType_SID	93//
#define DataType_SID_MAC	94//
#define DataType_COMDCB	95//
#define DataType_RCSD	96//
//保留	97-255	
	 
#define DataType_Data	255//自定义(数据类型非固定的)


//单位定义
#define Unit_a  1//	a	时间	年	
#define Unit_mo  2//	mo	时间	月	
#define Unit_wk  3//	wk	时间	周	7*24*60*60s
#define Unit_d  4//	d	时间	日	24*60*60s
#define Unit_h  5//	h	时间	小时	60*60s
#define Unit_min  6//	min	时间	分	60s
#define Unit_s  7//	s	时间（t）	秒	s
#define Unit_0  8//	°	（相）角	度	rad*180/π
#define Unit_0c  9//	℃	温度（T）	摄氏度	K-273.15
#define Unit_rmb  10//	货币	（当地）货币		
#define Unit_m  11//	m	长度（l）	米	m
#define Unit_m_s  12//	m/s	速度（v）	米/秒	m/s
#define Unit_m3  13//	m3	体积（V）rV，仪表常数或脉冲值（容积）	立方米	m3
#define Unit_m3_  14//	m3	修正的体积	立方米	m3
#define Unit_m3_h  15//	m3/h	流量	立方米每小时	m3/(60*60s)
#define Unit_m3_h_  16//	m3/h	修正的流量	立方米每小时	m3/(60*60s)
#define Unit_m3_d  17//	m3/d	流量	立方米每24小时	m3/(24*60*60s)
#define Unit_m3_d_  18//	m3/d	修正的流量	立方米每24小时	m3/(24*60*60s)
#define Unit_l  19//	l	容积	升	10-3m3
#define Unit_kg  20//	kg	质量（m）	千克	kg
#define Unit_N  21//	N	力（F）	牛顿	N
#define Unit_Nm  22//	Nm	能量	牛顿米	J=Nm=Ws
#define Unit_P  23//	P	压力（p）	帕斯卡	N/m2
#define Unit_bar  24//	bar	压力（p）	巴	10-5N/m2
#define Unit_J  25//	J	能量	焦耳	J=Nm=Ws
#define Unit_J_h  26//	J/h	热功	焦每小时	J/(60*60s)
#define Unit_W  27//	W	有功功率（P）	瓦	W=J/s
#define Unit_kW  28//	kW	有功功率（P）	千瓦	kW=J/(s*1000)
#define Unit_VA  29//	VA	视在功率（S）	伏安	
#define Unit_kVA  30//	kVA	视在功率（S）	千伏安	
#define Unit_var  31//	var	无功功率（Q）	乏	
#define Unit_kvar  32//	kvar	无功功率（Q）	千乏	
#define Unit_kWh  33//	kWh	有功能量 rw，有功电能表常数或脉冲值	千瓦-时	kW*(60*60s)
#define Unit_kVAh 34//	kVAh	视在能量rS，视在电能表常数或脉冲值	千伏-安-小时	kVA*(60*60s)
#define Unit_kvarh  35//	kvarh	无功能量rB，无功电能表常数或脉冲	千乏-时	kvar*(60*60s)
#define Unit_A  36//	A	电流（I）	安培	A
#define Unit_C  37//	C	电量（Q）	库仑	C=As
#define Unit_V  38//	V	电压（U）	伏特	V
#define Unit_V_m  39//	V/m	电场强度（E）	伏每米	V/m
#define Unit_F  40//	F	电容（C）	法拉	C/V=As/V
#define Unit_R  41//	Ω	电阻（R）	欧姆	Ω=V/A
#define Unit_Rm  42//	Ωm2/m	电阻系数（ρ）		Ωm
#define Unit_Wb  43//	Wb	磁通量（Φ）	韦伯	Wb=Vs
#define Unit_T  44//	T	磁通密度（T）	泰斯拉	Wb/m2
#define Unit_A_m  45//	A/m	磁场强度（H）	安培每米	A/m
#define Unit_H  46//	H	电感（L）	亨利	H=Wb/A
#define Unit_Hz  47//	Hz	频率	赫兹	1/s
#define Unit_1_Wh  48//	1/(Wh)	有功能量表常数或脉冲		imp/kWh
#define Unit_1_varh  49//	1/(varh)	无功能量表常数或脉冲		
#define Unit_1_VAh  50//	1/(VAh)	视在能量表常数或脉冲		
#define Unit__bfb  51//	%	百分比	百分之	
#define Unit_byte  52//	byte	字节	字节	
#define Unit_dBm  53//	dBm	分贝毫瓦		
#define Unit_rmb_kWh  54//	元/kWh	电价		
#define Unit_Ah  55//	Ah	安时	安时	
#define Unit_ms  56//	ms	毫秒	毫秒	
//57…253	保留
//254	其他单位
//255	无单位、缺单位、计数





typedef __packed struct
{	 
	u8 TypeNum;//类型列表数
	u8 TypeList[256];//类型列表
}Attribute_TypeDef;//属性定义

typedef __packed struct
{
#ifdef IC_STM32F4xx
	u16 OI;//OI
  u32 Addr;//原始数据地址
	u8 LENper;//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	u8 array;//数组数
	u8 DataType;//数据类型
	s8 Scaler;//换算
  u8 Unit;//单位
	u8 IC;//接口类
#endif
#ifdef IC_NUC9xx
	u32 OI;//u16 OI;//OI
  u32 Addr;//原始数据地址
	u16 LENper;//u8 LENper;//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	u16 array;//u8 array;//数组数
	u8 DataType;//数据类型
	s8 Scaler;//换算
  u8 Unit;//单位
	u8 IC;//接口类
#endif
}DL698_iMeterLIB_TypeDef;

typedef __packed struct
{
#ifdef IC_STM32F4xx
	u32 OAD;//对象标识
  u32 Addr;//数据地址
	u16 LENper;//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	u32 LENmax;//FLASH空间中预留的最大长度
	u8 rw;//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	u32 *pDataType;//数据类型列表指针
	u32 *pInitData;//初始化原始数据指针
	u16 LEN_Sour;//初始化原始数据长度
#endif
#ifdef IC_NUC9xx
	u32 OAD;//对象标识
  u32 Addr;//数据地址
	u32 LENper;//u16 LENper;//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	u32 LENmax;//FLASH空间中预留的最大长度
	u32 rw;//u8 rw;//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	u32 *pDataType;//数据类型列表指针
	u32 *pInitData;//初始化原始数据指针
	u32 LEN_Sour;//u16 LEN_Sour;//初始化原始数据长度
#endif
}DL698_SETLIB_TypeDef;

typedef __packed struct
{
#ifdef IC_STM32F4xx
	u16 OI;//对象标识
	u32 Addr;//配置参数地址
	u16 LENper;//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	u32 LENmax;//配置参数FLASH空间中预留的最大长度
	u8 rw;//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	u32 *pDataType;//配置参数数据类型列表指针
	u32 *pInitData;//配置参数初始化原始数据指针
	u16 LEN_Sour;//配置参数初始化原始数据长度
	
	u8 IC;//接口类
  u32 Addr_EventOAD;//有效标识,上报标识,关联对象OAD地址
	u32 *pInitEventOAD;//有效标识,上报标识,关联对象OAD初始化原始数据指针
	u16 LEN_EventOAD;//有效标识,上报标识,关联对象OAD初始化原始数据长度
	
	u32 Addr_Currentvalue;//当前值记录表地址,0=无
#endif
#ifdef IC_NUC9xx
	u32 OI;//u16 OI;//对象标识
	u32 Addr;//配置参数地址
	u32 LENper;//u16 LENper;//当数组的每组有固定间隔时为每组的固定间隔长度,否则为0
	u32 LENmax;//配置参数FLASH空间中预留的最大长度
	u32 rw;//u8 rw;//读写:b0=读,b1=写,b2=通信参数;0=无读写,1=只读,2=只写,3=读写
	u32 *pDataType;//配置参数数据类型列表指针
	u32 *pInitData;//配置参数初始化原始数据指针
	u32 LEN_Sour;//u16 LEN_Sour;//配置参数初始化原始数据长度
	
	u32 IC;//u8 IC;//接口类
  u32 Addr_EventOAD;//有效标识,上报标识,关联对象OAD地址
	u32 *pInitEventOAD;//有效标识,上报标识,关联对象OAD初始化原始数据指针
	u32 LEN_EventOAD;//u16 LEN_EventOAD;//有效标识,上报标识,关联对象OAD初始化原始数据长度
	
	u32 Addr_Currentvalue;//当前值记录表地址,0=无
#endif
}DL698_EVENTLIB_TypeDef;

typedef __packed struct
{
	u32 OAD;//对象标识
  u32 Addr;//数据地址
}EVENT_TEMPDATALIB_TypeDef;//生成事件用临时数据指针库

typedef __packed struct
{
	u32 *pLib;//库地址
}DL698_LIB_List_TypeDef;




void Get_date_time_s(u8* p8d);//得到date_time_s共7字节
u64 R_Inverse(u64 Data,u32 Byte);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
u32 Bit_Inverse(u32 Data,u32 bit);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
u32 Get_DL698ByteTypeLenByte(u8 *p8);//得到DL698字节类型用来表示长度单元的字节数
u32 Get_DL698ByteTypeLenValue(u8 *p8);//得到DL698字节类型长度单元的值
u32 Get_DL698BinTypeLenByte(u8 *p8);//得到DL698位类型用来表示长度单元的字节数
u32 Get_DL698BinTypeLenValue(u8 *p8);//得到DL698位类型长度长度单元的值转为字节数
u64 Get_TSA_Value(u8 *p8);//得到TSA的不大于6字节通信地址,数据顺序不变,入口:p8=TSA类型后的字节，若无效值则=0
u32 TItoSECOND(u32 TI);//TI转为秒值,入口:单位在低字节,出口:hex秒
u32 pTItoSECOND(u8 *pTI);//TI转为秒值,入口:pTI无数据类型,出口:hex秒
void pYYMDHMS_AddTI_hex(u8* pYYMDHMS,u8* pTI);//年年月日时分秒加时间间隔TI,入口:pYYMDHMS,pTI无数据类型,返回加间隔TI后的年年月日时分秒

u32 DataLen_RSD_NoMS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//内联的MS长度不计
u32 DataLen_MS(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//
u32 DataLen_RCSD(u8 *p8,Attribute_TypeDef *pDataType,u32 TypeCount);//
u32 Get_DL698DataLen(u8* p8,Attribute_TypeDef *pAttribute,u32 TypeCount,u32 LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b31-b8长度,b7-b0元素计数
u32 Get_DL698DataLen_S(u8* p8,u32 LENmax);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
u64 Get_DL698DataAddr(u8 *p8,Attribute_TypeDef *pAttribute,u32 TypeCount,u32 ElementCount,u32 Element,u32 LENmax);//得到DL698数据地址,入口:LENmax仅取NAND数据时先读到ADDR_128K_DATABUFF用,返回:b63-b32 p8,b31-b8总元素计数,b7-b0元素计数
//计算元素地址
//入口:pData=要计算的数据开始地址
//LENmax_Data=要计算的数据串可能的最大长度用于当数据在nand flash时先读入sdram的数据字节数(为快速计算,计算数据总是在ram中进行),最大长度限制128k;当数据本在ram此值无关;当数据在nand flash但LENmax_Data=0时会自动读取128k数据影响运算速度
//Element：要计算的数据地址的主元素计数,每1个数据类型为1个元素,结构和数组不论内有多少个元素计为1个元素,元素从0开始编号,要计算的元素从1开始因第0号元素为开始地址不需计算
//SubElement：若主元素为结构或数组,要计算主元素结构或数组内的元素地址需指定子元素计数,子元素也从0开始编号,要计算的子元素从1开始因第0号子元素为主元素地址不需计算,无子元素时此值必需=0
//注意:当主元素为数组时,子元素计数与第几组数组相关,子元素计数=(第几组*组内总元素个数)+当前组元素计数;
//出口:地址指向元素的类型字节,0=错误
u8 *Get_Element(u8 *pData,u32 Element,u32 SubElement,u32 LENmax_Data);//计算元素地址(虚拟地址指针),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误
u32 Get_ElementADDR(u32 ADDR_Data,u32 Element,u32 SubElement,u32 LENmax_Data);//计算元素地址(实际地址),使用ADDR_128KDATABUFF;出口:地址指向元素的类型字节,0=错误

u32 Compare_TSA_MS(u8 *pADDR_6000,u8 *pTSA,u8 *pMS);//比较TSA是否符合MS集合,入口:pADDR_6000已在RAM,pTSA,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
u32 Compare_SETNO_MS(u8 *pADDR_6000,u32 SETNO,u8 *pMS);//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
void MStoAddrList(u8*pMS);//电能表集合到地址列表;入口:pMS(无数据类型);出口:地址列表在ADDR_AddrList

u32 PORTntoOAD(u32 PORTn);//通信口号转为OAD值
u32 OADtoPORTn(u32 OAD);//OAD值转为通信口号
u32 Compare_DL698DataString(u8 *p1,u8 *p2,u32 Byte);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
u32 Compare_octet_string(u8 *p1,u8 *p2);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
u32 Compare_TSA_string(u8 *p1,u8 *p2);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
u32 OI_IC(u32 OI);//得到接口类号;入口:对象标识OI
u32 AddrToMeterSetNo(u16* pAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
u32 AddrTowToMeterSetNo(u16* pAddr);//搜索通信地址采集器地址在采集档案配置表中的序号;返回:从0开始的配置序号,>=NUM_RMmax表示无效
u32 AddrToMeterSetNoJS(u16* pAddr);//搜索通信地址在采集档案配置表中的序号;返回:从0开?
u16 *MeterSetAddrNo(u16* pCollectorAddr);

u32 Deal_DataNull_JS(u8 pDataType,u8* p8);



#ifdef __cplusplus
 }
#endif
#endif


