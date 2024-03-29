

#ifndef Menu7_H
#define Menu7_H

#include "../QCSG_Head/Chint_Type.h"

u32 ConvertPnParaToBitValue(MeasurePointStruct* Pn);

void ConvertBitValueToPnPara(MeasurePointStruct* Pn, u32 BitValue);

#define PN_EFFECTIVE_BIT_MASK			(0x00000001)
#define PN_BAUD_RATE_BIT_MASK			(0x0000001E)
#define PN_PORT_ID_BIT_MASK				(0x000000E0)
#define PN_PROTOCOL_BIT_MASK			(0x00000F00)
#define PN_METER_TYPE_BIT_MASK			(0x00007000)
#define PN_TOTAL_TYPE_BIT_MASK			(0x00018000)
#define PN_USER_TYPE_BIT_MASK			(0x00020000)
#define PN_ON_OFF_BIT_MASK				(0x00040000)
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_7
// Descriptions:        参数设置与查询
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Menu_7(void);//菜单7


void Menu_7_1_1(void);
void Menu_7_1_1_1(void);
void Menu_7_1_1_2(void);
void Menu_7_1_2(void);
void Menu_7_1_3(void);
void Menu_7_1_4(void);
void Menu_7_1_5(void);
void Menu_7_1_6(void);
void Menu_7_1_7(void);
void Menu_7_1_7_1(void);
void Menu_7_1_7_1_1(void);
void Menu_7_1_7_1_1_1(void);
void Menu_7_1_7_1_1_2(void);
void Menu_7_1_7_1_1_3(void);
void Menu_7_1_7_1_2(void);
void Menu_7_1_7_1_2_1(void);
void Menu_7_1_7_1_2_2(void);
void Menu_7_1_7_1_2_3(void);
void Menu_7_1_7_2(void);
void Menu_7_2_1(void);
void Menu_7_2_2(void);
void Menu_7_2_3(void);
void Menu_7_2_3_1(void);
void Menu_7_2_4(void);
void  DisplayMPInformation(u32 PnNum);
void  MeasurePointClr(u32 PnNum1,u32 PnNum2);
void Menu_7_3_1(void);
void Menu_7_3_2(void);
void Menu_7_3_3(void);
void Menu_7_3_4(void);
#if Project == 200 //南网集中器
void Menu_7_4(void);
void Menu_7_4_1(void);
void Menu_7_4_2(void);
void Menu_7_4_3(void);
void Menu_7_4_4(void);
void Menu_7_4_1_1(void);
void Menu_7_4_1_2(void);
void Menu_7_4_1_3(void);
void Menu_7_4_1_4(void);
void Menu_7_4_1_1_1(void);
void Menu_7_4_1_1_2(void);
void Menu_7_4_1_2_1(void);
void Menu_7_4_1_2_2(void);
void Menu_7_4_1_3_1(void);
void Menu_7_4_1_3_2(void);
void Menu_7_4_1_4_1(void);
void Menu_7_4_1_4_2(void);
void Menu_7_4_2_1(void);
void Menu_7_4_2_2(void);
void Menu_7_4_3_1(void);
void Menu_7_4_3_2(void);
#endif
void Menu_7(void);

#endif

