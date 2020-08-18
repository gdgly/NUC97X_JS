#ifndef CALC_TAG_H
#define CALC_TAG_H
#ifdef __cplusplus
 extern "C" {
#endif


void Init_Data230x_D(void);//初始化总加组日数据
void Init_Data230x_M(void);//初始化总加组月数据
void Init_Data230x(void);//初始化总加组数据
u32 Get_CT_PT(u16 * pAddr);//获取TSA对应的CT,PT值，CT放高2字节，PT放低2字节，值已经高低位交换过了，可以直接使用。
u32 Is_TSA_Same(u8 *pAddr1,u8 *pAddr2);//判断地址是否相同,返回 0=相同 1=不相同
u32 Is_FnDataBuff_Have_TAGData(u32 oad,u8* pbuf);//返回 0=不存在oad的数据，!=0，oad对应的数据地址
void Do_Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 data_dest,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type);//pAddr1是TSA，data_sour是01 05 06 XXXX 06 XXXX,data_dest 0X14 XXXXXXXX
void Update_ADDR_230x_Sour(u16 * pAddr,u32 oad_data_addr,u32 TAG_NO,u32 add_mise,u32 type,u32 group_type,u32 pn_type);//把数据更新到ADDR_230x_Sour，之前没有就新加
void Update_ADDR_230x_x(u32 addr_data,u32 LENper,u64 value,u32 TAG_NO,u32 add_mise,u32 fee);//更新总加电量
void Update_ADDR_230x_11(u64 value,u32 TAG_NO,u32 fee);//更新总加剩余电量（费）
void Update_Group_Total_Data_6000(u8 *FnDataBuff);//更新总加组的数据,配置表是6000里的
void Update_Group_Total_Data_240x(void);//更新总加组的数据,配置表是脉冲量

void Calc_TAG_Power(void);//计算全部总加组的功率
void Calc_PerTAG_Power(u32 TAG_NO,u32 Power_type);//计算单一总加的功率，TAG_NO=总加组编号 1--8，Power_type=功率类型，0=有功功率 1=无功功率
u64 Get_6000_Power(u32 pn,u32 Power_type);//获取测量点的功率
u64 Get_TSA_BY_PN_noInverse(u32 Pn);//根据配置序号，得到TSA。不包含数据类型。不调换顺序
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif

