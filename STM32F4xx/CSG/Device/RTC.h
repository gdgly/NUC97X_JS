
//ERTC
#ifndef ERTC_H
#define ERTC_H
#ifdef __cplusplus
 extern "C" {
#endif



void DS3231_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231��
void RX8025T_Read(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025T��
void DS3231_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//DS3231д
void RX8025T_Write(u32 ADDR_RAM,u32 ADDR_Drive,u32 Byte);//RX8025Tд



void Init_RTC(void);//���ⲿʵʱ�ӳ�ʼ��

//--------------------------------------------------------------------------------------------------------
// Function name:       ReadRTC
// Descriptions:        ���ⲿӲʵʱ�ӵ�ָ�������ַ
// input parameters:    �����ַ
// output parameters:   ��������ʱ������������ַ��
// Returned value:      ��
//--------------------------------------------------------------------------------------------------------
void ReadRTC(u32 ADDR_BUFF);//��ʵʱ�ӵ�ָ�������ַ


//--------------------------------------------------------------------------------------------------------
// Function name:       WriteRTC
// Descriptions:        д�ⲿӲʵʱ��
// input parameters:    ��������ʱ�������ڻ����ַ��
// output parameters:   ��
// Returned value:      ��
//--------------------------------------------------------------------------------------------------------
void WriteRTC(u32 ADDR_BUFF);//дʵʱ��

#ifdef __cplusplus
 }
#endif
#endif
