

#ifndef Menu8_H
#define Menu8_H


u32 ConvertRs485PortParaToBitValue(u32 Func, u32 BaudRate, u32 Parity, u32 DataBit, u32 StopBit);

void ConvertBitValueToRs485PortPara(u8* Func, u8* BaudRate, u8* Parity, u8* DataBit, u8* StopBit, u32 BitValue);

#define RS485_FUNCTION_BIT_MASK			(0x00000007)
#define RS485_BAUD_RATE_BIT_MASK		(0x00000078)
#define RS485_PARITY_BIT_MASK			(0x00000180)
#define RS485_DATA_BIT_MASK				(0x00001E00)
#define RS485_STOP_BIT_MASK				(0x00006000)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_8
// Descriptions:        终端管理与维护
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_8(void);//菜单8



void Menu_8_1_1(void);
void Menu_8_1_2(void);
void Menu_8_1_3(void);
void Menu_8_3_1(void);
void Menu_8_3_1_1(void);
void Menu_8_3_2(void);
void Menu_8_3_2_1(void);
void Menu_8_3_2_2(void);
void Menu_8_3_2_3(void);
void Menu_8_3_2_4(void);
void Menu_8_8_1(void);
void Menu_8_8_2(void);
void Menu_8_8_3(void);
void Menu_8_10_1(void);
void Menu_8_10_2(void);

#endif

