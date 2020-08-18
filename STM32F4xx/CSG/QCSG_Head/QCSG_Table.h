/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_DLT645_Table.h
作  者:mxn
版  本:1.00
日  期:20170501
描  述:南网数据标识与645数据标识对应表
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_DLT645_TABLE_H
#define __QCSG_DLT645_TABLE_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_PublicDefine.h"
/* Exported define -----------------------------------------------------------*/
#define MAX_AFN040A_IDENT_NUM										384
#define MAX_AFN0C_0D_IDENT_NUM										512
#define MAX_FROZEN_RD97_IDENT_NUM									8
#define MAX_FROZEN_RD07_IDENT_NUM									11
#define MAX_FROZEN_EDMI_IDENT_NUM									11//增加登陆
#define MAX_FIXED_RD97_IDENT_NUM									22
#define MAX_FIXED_RD07_IDENT_NUM									20//18//17 //err20181107 增加校时事件记录//err20181108 增加事件 电能表掉上电事件(只抄读不产生) 
#define MAX_ALARM_IDENT_NUM											32
#define MAX_FIXED_EDMI_IDENT_NUM									78
#define MAX_FIXED_EDMI_NUM 			15
//#define MAX_EDMI_INFO_NUM    			8


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
extern QCSG_RESOLVE_S const gc_stQCSGResolve_AFN04[MAX_AFN040A_IDENT_NUM];
extern QCSG_RESOLVE_S const gc_stQCSGResolve_AFN0A[MAX_AFN040A_IDENT_NUM];
extern QCSG_DLT645_CON_S const gc_stQCSGAFN0C_0D_DLT645_Table[MAX_AFN0C_0D_IDENT_NUM];
extern QCSG_EDMI_CON_S const gc_stQCSGAFN0C_0D_EDMI_Table[MAX_AFN0C_0D_IDENT_NUM];
extern QCSG_ALARM_CON_S const gc_stQCSGAlarm_DLT645_Table[MAX_ALARM_IDENT_NUM];
extern u32 const gc_Jc_Rs485_FrozenReadCode_97[MAX_FROZEN_RD97_IDENT_NUM];
extern u32 const gc_Jc_Rs485_FrozenReadCode_07[MAX_FROZEN_RD07_IDENT_NUM];
extern QCSG_EDMI_FROZEN_CON_S const gc_Jc_Rs485_FrozenReadCode_EDMI[MAX_FROZEN_EDMI_IDENT_NUM];
extern u32 const gc_Jc_Rs485_FixedReadCode_97[MAX_FIXED_RD97_IDENT_NUM];
extern u32 const gc_Jc_Rs485_FixedReadCode_07[MAX_FIXED_RD07_IDENT_NUM];
extern QCSG_STATISTICSDATA_TABLE_S const gc_StatisticsIdent_Table[];
extern stNwEdmi const EDMI_cmd[MAX_FIXED_EDMI_NUM];
extern u8 const EDMI_Load_Fram[4];
extern u8 const EDMI_UnLoad_Fram[4];
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __QCSG_DLT645_TABLE_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
