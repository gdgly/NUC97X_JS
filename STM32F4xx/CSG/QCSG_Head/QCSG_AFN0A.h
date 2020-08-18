/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_AFN0A.h
作  者:mxn
版  本:1.00
日  期:20170601
描  述:南网AFN0A处理文件
*************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QCSG_AFN0A_H
#define __QCSG_AFN0A_H
/* Includes ------------------------------------------------------------------*/
//#include "Chint_Type.h"
//#include "QCSG_PublicDefine.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern u8 QCSG_AFN0A_Resolve(u8 u8ChNo,QCSG_REVFRAME_S* pstRevFrame,QCSG_RESOLVE_INFO_S* pstQCSG_Info);


ResultEnum QCSG_AFN0A_Resolve_ZY(u32 PORTn, QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame);

ResultEnum QCSG_AFN0A_E0000100_E000012F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000100_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000101_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000102_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000103_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000104_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000105_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000106_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000107_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000108_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000109_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E000010A_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E000010B_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E000010F_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000111_Read(u8* Data, E000011x_CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000112_Read(u8* Data, E000011x_CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000113_Read(u8* Data, E000011x_CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000120_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000121_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000122_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000123_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000124_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000125_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000126_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000127_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E000012F_Read(u8* Data, CommunicationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000130_E0000174(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000131_Read(u8* Data);

ResultEnum QCSG_AFN0A_E0000140_Read(u8* Data);

ResultEnum QCSG_AFN0A_E0000180_E000018F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000180_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000181_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000182_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000183_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000184_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000185_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000186_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000187_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000188_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000189_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E000018A_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E000018B_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E000018C_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E000018F_Read(u8* Data, ThresholdParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000200_E0000220(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000221_E0000250(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000300_E00003FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000400_E00004FE(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000500_E00005FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000500_Read(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000501_Read(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000502_Read(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00005FE_Read(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00005FF_Read(u8* Data, AnalogCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000600_E00006FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000600_Read(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000601_Read(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000602_Read(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00006FE_Read(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00006FF_Read(u8* Data, PulseCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000700_E00007FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000700_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000701_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000702_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000703_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000704_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000705_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000706_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000707_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000708_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00007FE_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00007FF_Read(u8* Data, CaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000800_E00008FF(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000800_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000801_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000802_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000803_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000804_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000805_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000806_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000807_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000808_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00008FE_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E00008FF_Read(u8* Data, DiffCaculationParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000A00_E0000B02(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000C00_E0000E11(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0000D00_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D01_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D02_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D03_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D04_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D05_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D06_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D07_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D08_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D09_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D0F_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D10_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D11_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D12_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D13_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D14_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D15_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D16_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D17_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D18_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D19_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D1F_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D20_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D21_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D22_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D23_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D24_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D25_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D26_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D27_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D28_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D29_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D2F_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D30_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D31_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D32_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D33_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D34_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D35_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D36_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D37_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D38_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D39_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0000D3F_Read(u8* Data, LoadManagementParaStruct* Para);

ResultEnum QCSG_AFN0A_E0001100_E0002000(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0800000_E080002F(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0800000_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800001_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800002_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800003_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800004_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800005_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800006_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800007_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800008_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800009_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E080000A_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E080000B_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E080000C_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E080000D_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E080000F_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800010_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800011_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800012_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800013_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800014_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E080001F_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800021_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800022_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E080002F_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800030_E0800031(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0800030_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800031_Read(u8* Data, MeasurePointStruct* Para);

ResultEnum QCSG_AFN0A_E0800100_E080020E(QCSGFrameStruct* SrcFrame, QCSGFrameStruct* DstFrame, QCSGFrameProcessStruct* QCSGFp);

ResultEnum QCSG_AFN0A_E0800100_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800101_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800102_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800103_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800104_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800105_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800106_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800107_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800108_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800109_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080010A_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080010B_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080010C_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080010F_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800200_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800201_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800202_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800203_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800205_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800206_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800207_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800208_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E0800209_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080020A_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080020B_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080020C_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080020D_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_AFN0A_E080020E_Read(u8* Data, PowerQualityParaStruct* Para);

ResultEnum QCSG_Check_AFN0A_DataID(u32 DataID, u16* Index);

ResultEnum CheckANF0APn(u32 Pn);

#endif /* __QCSG_AFN0A_H */
/************************ (C) COPYRIGHT M***********************END OF FILE****/
