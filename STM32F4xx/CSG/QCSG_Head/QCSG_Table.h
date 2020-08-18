/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_DLT645_Table.h
��  ��:mxn
��  ��:1.00
��  ��:20170501
��  ��:�������ݱ�ʶ��645���ݱ�ʶ��Ӧ��
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
#define MAX_FROZEN_EDMI_IDENT_NUM									11//���ӵ�½
#define MAX_FIXED_RD97_IDENT_NUM									22
#define MAX_FIXED_RD07_IDENT_NUM									20//18//17 //err20181107 ����Уʱ�¼���¼//err20181108 �����¼� ���ܱ����ϵ��¼�(ֻ����������) 
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