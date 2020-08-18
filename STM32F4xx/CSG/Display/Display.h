

//��ʾ
#ifndef Display_H
#define Display_H
#ifdef __cplusplus
 extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayClr
// Descriptions:        ��ȫ����ʾ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void DisplayClr(void);//����


//-------------------------------------------------------------------------------------------------
// Function name:       DisplayClrRow
// Descriptions:        ����Ļ��ʾ��ָ����
// input parameters:    ��ʼ�к�,�����к�(�к�0-9(160*160),0-14(320*240))
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void DisplayClrRow(u32 StartRow,u32 EndRow);//����Ļ��ʾ��ָ����



void DisplayStringAutoUp(u32 StartRow,u32 EndRow,u8 * pStr);//��ָ���������Զ�����ƽ�ƺ���ʾ�ַ���;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString
// Descriptions:        ��ʾ�ַ���
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�Զ�����ʱ��������,�ַ���ָ��(0Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B23��������,B24-B31����
//-------------------------------------------------------------------------------------------------
u32 DisplayString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//��ʾ�ַ���

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString
// Descriptions:        ��ʾ�ַ���
// input parameters:    ��Ļ��(0-15),��Ļ��(0-19),�Զ�����ʱ��������,�ַ���ָ��(0Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B23��������,B24-B31����
//						ע��ʹ��ʱ��ò�Ҫ��ʾ��ͼ��(0��)���ͼ��(9��)
//-------------------------------------------------------------------------------------------------
u32 DisplayStringOutoffScreenLine(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//��ʾ�ַ���

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyStringOnlyOneRow
// Descriptions:        ��ʾ�ַ�������1����
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�ַ���ָ��(\n,\r,0��Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B31�������ַ���
//-------------------------------------------------------------------------------------------------
u32 DispalyStringOnlyOneRow(u32 Row,u32 Col,u8 * pStr);//��ʾ�ַ�������1����



u16 * DispalyStringContinue(u16 *pStrBuff,u8 * pStr);//��ʾ�ַ�������


//---------------------------------------------------------------------------------------------------------
// Function name:       DispalyAlign
// Descriptions:        ��ʾ����(1�����,2����,3�Ҷ���)
// input parameters:    Row��������,AlignCol��ʾ���뵽�к�(160*160�����к�0-19,�����������,�Ҷ���),Align���뷽ʽ:1=�����,2=����,3=�Ҷ���
// output parameters:   ��ʾ����(������������ݶ���)
// Returned value:      ��
//---------------------------------------------------------------------------------------------------------
void DispalyAlign(u32 Row,u32 AlignCol,u32 Align);//����ʾ����(1�����,2����,3�Ҷ���)


//---------------------------------------------------------------------------------------------------------
// Function name:       DisplayData
// Descriptions:        ����ʾ
// input parameters:    NumҪ��BCD��,BitNum��ʾ��λ��,DpNumС��λ��,p16�ַ���ʾ����ָ��
// output parameters:   ASCII�ֿ���ʾ
// Returned value:      ���ַ���ʾ����ָ��
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData(u64 Num,u32 BitNum,u32 DpNum,u16 * p16);//������ʾ,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF


//---------------------------------------------------------------------------------------------------------
// Function name:       DisplayData_OffZero
// Descriptions:        ����ʾ
// input parameters:    NumҪ��BCD��,BitNum��ʾ��λ��,DpNumС��λ��,p16�ַ���ʾ����ָ��
// output parameters:   ASCII�ֿ���ʾ,������Ч��λ��0
// Returned value:      ���ַ���ʾ����ָ��
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData_OffZero(u64 Num,u32 BitNum,u32 DpNum,u16 * p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF


//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenuString
// Descriptions:        ��ʾ�˵��ַ���
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�Զ�����ʱ��������,�ַ���ָ��(0Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B23��������,B24-B31����
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//��ʾ�˵��ַ���
u32 DisplayMenuString_CSG(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//��ʾ�˵��ַ���

void DisplayScrollUp(u32 Row);//��ʾ�Ϲ�����0����,����Ϲ�����




u16 * DisplayData1(u64 Num1,u32 BitNum,u32 DpNum,u8 Num2,u16 * p16)	;//Num2�����". : -�ȷ���"
void DisplayRam(u32 Ln,u32 RamAddr);//��ʾ��RAM����(������)


void DisplayRATE(u32 Ln,u32 Rate);//��ʾ����;���:Ln=��ʾ��,Rate=����(hex,0-100%)

void Startdisplay(void);			//����ʾ
#if ((Project/100)==2)
void NWJZQDisplayDefaultInterface(void);//����������Ĭ�Ͻ�����ʾ
#endif


#ifdef __cplusplus
 }
#endif 
#endif
