/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN04.h
作  者:mxn
版  本:1.00
日  期:20170601
描  述:南网AFN04处理文件
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_AFN04_H
#define __QCSG_AFN04_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_PublicDefine.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern u8 QCSG_AFN04_Resolve(u8 u8ChNo,QCSG_REVFRAME_S* pstRevFrame,QCSG_RESOLVE_INFO_S* pstQCSG_Info);

ResultEnum QCSG_AFN04_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame);

ResultEnum QCSG_AFN04_E0000100_E000012F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000100_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000101_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000102_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000103_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000104_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000105_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000106_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000107_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000108_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000109_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E000010A_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E000010B_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E000010F_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000111_Write(u8* Data, E000011x_CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000112_Write(u8* Data, E000011x_CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000113_Write(u8* Data, E000011x_CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000120_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000121_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000122_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000123_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000124_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000125_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000126_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000127_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E000012F_Write(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000130_E0000174(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000131_Write(u8* Data, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame);

ResultEnum QCSG_AFN04_E0000140_Write(u8* Data, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000180_E000018F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000180_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000181_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000182_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000183_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000184_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000185_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000186_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000187_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000188_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000189_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E000018A_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E000018B_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E000018C_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E000018F_Write(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN04_E0000200_E0000220(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000221_E0000250(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000300_E00003FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000400_E00004FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000500_E00005FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000500_Write(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000501_Write(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000502_Write(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00005FE_Write(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00005FF_Write(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000600_E00006FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000600_Write(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000601_Write(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000602_Write(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00006FE_Write(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00006FF_Write(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000700_E00007FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000700_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000701_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000702_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000703_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000704_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000705_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000706_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000707_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000708_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00007FE_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00007FF_Write(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000800_E00008FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000800_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000801_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000802_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000803_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000804_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000805_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000806_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000807_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000808_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00008FE_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E00008FF_Write(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN04_E0000A00_E0000B02(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000C00_E0000E11(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0000D00_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D01_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D02_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D03_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D04_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D05_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D06_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D07_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D08_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D09_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D0F_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D10_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D11_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D12_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D13_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D14_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D15_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D16_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D17_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D18_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D19_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D1F_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D20_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D21_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D22_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D23_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D24_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D25_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D26_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D27_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D28_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D29_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D2F_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D30_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D31_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D32_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D33_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D34_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D35_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D36_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D37_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D38_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D39_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0000D3F_Write(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN04_E0001100_E0002000(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0001100_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame);

ResultEnum QCSG_AFN04_E0001101_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame);

ResultEnum QCSG_AFN04_E0001102_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame);

ResultEnum QCSG_AFN04_E0001103_Write(u8* Data, MeterPointCtrlParaStruct* Para, QCSGFrameProcessStruct* QCSGFp, QCSGFrameStruct* SrcFrame);

ResultEnum QCSG_AFN04_E0001104_Write(u8* Data, MeterPointCtrlParaStruct* Para);

ResultEnum QCSG_AFN04_E0800000_E080002F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0003000_E0003001(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

//定时自动搜表参数
ResultEnum QCSG_AFN04_E0003000_Write(u8* Data, MeasurePointStruct* Para);

//手动启动搜表
ResultEnum QCSG_AFN04_E0003001_Write(u8* Data, MeasurePointStruct* Para);


ResultEnum QCSG_AFN04_E0800000_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800001_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800002_Write(QCSGFrameProcessStruct* QCSGFp, u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800003_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800004_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800005_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800006_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800007_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800008_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800009_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E080000A_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E080000B_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E080000C_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E080000D_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E080000F_Write(QCSGFrameProcessStruct* QCSGFp, u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800010_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800011_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800012_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800013_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800014_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E080001F_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800021_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800022_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E080002F_Write(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN04_E0800100_E080020E(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN04_E0800100_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800101_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800102_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800103_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800104_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800105_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800106_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800107_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800108_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800109_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080010A_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080010B_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080010C_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080010F_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800200_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800201_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800202_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800203_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800205_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800206_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800207_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800208_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E0800209_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080020A_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080020B_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080020C_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080020D_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN04_E080020E_Write(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_Check_AFN04_DataID(u32 DataID, u16* Index);

ResultEnum CheckANF04Pn(u32 Pn);

#endif /* __QCSG_AFN04_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/

