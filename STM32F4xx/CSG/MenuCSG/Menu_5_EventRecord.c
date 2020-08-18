/*************************************************
��Ȩ:��̩�Ǳ������ն˲�
�ļ�:Menu_5_EventRecord.c
����:zyl
�汾:1.00
����:20170717
����:�������ز˵�5�¼���¼�����ļ�
*************************************************/


//�˵�5 �¼���¼
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu5.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"


const u8 pMenu_5[]=//�����ն��¼���¼
{
	"\n\r"
	"�����¼�\n\r"
	"����¼�\n\r"
	"ʧѹ�¼�\n\r"
	"ʧ���¼�\n\r"
	"�����¼�\n\r"
	"����/�ϵ��¼�\n\r"
	"�����¼�\n\r"
	"Уʱ�¼�\n\r"
	"����ť�п����¼�\n\r"
	"���ǿ����¼�\n\r"
	"���ſ����¼�\n\r"
	"��Ϣ��֤�����¼�\n\r"
};

const u8 pMenu_5_3[]=//
{
	"\n\r"	
	"A��ʧѹ�¼�\n\r"
	"B��ʧѹ�¼�\n\r"
	"C��ʧѹ�¼�\n\r"
};


const u8 pMenu_5_4[]=//
{
	"\n\r"	
	"A��ʧ���¼�\n\r"
	"B��ʧ���¼�\n\r"
	"C��ʧ���¼�\n\r"
};


const u8 pMenu_5_5[]=//
{
	"\n\r"	
	"A������¼�\n\r"
	"B������¼�\n\r"
	"C������¼�\n\r"
};


//-------------------------------------------------------------------------------------------------
// Function name:       DispEventDataInfo(AlarmEventFileHeadStruct* AlarmHead, u8* AlarmData, u32 DataLen)
// Descriptions:        ��ʾ�¼���¼��Ϣ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void DispEventDataInfo(AlarmEventFileHeadStruct* EventHead, u8* EventData, u32 DataLen)
{	
	u8 *p8,*p;
	u64 x = 0;
	StrBuff_TypeDef* DispBuf = StrBuff;	
	
	p8 = (u8*)&DispBuf->Row[2].Col[5];                              //��ַ
	MR((u32)&x, (u32)EventHead->MeterAddr, METER_ADDR_SIZE);
	DisplayData(x,12,0,(u16*)p8);		

	
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+15*2);                          //����ʱ��	 ��							
	DisplayData(EventData[0],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+12*2);                          //��
	DisplayData(EventData[1],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+9*2);                           //ʱ
	DisplayData(EventData[2],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+6*2);                           //��
	DisplayData(EventData[3],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+3*2);	                          //��
	DisplayData(EventData[4],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+6*84+4+0*2);                           //��
	DisplayData(EventData[5],2,0,(u16*)p8);
	
	switch(EventHead->DataID)
	{
		case 0xE2010003:
			DisplayString(3,5,0,"���������¼�");		
			DisplayString(4,9,0,"0xE2010003");
			break;
		case 0xE2010001:
			DisplayString(3,5,0,"����¼�");
			DisplayString(4,9,0,"E2010001");
			break;
		case 0xE2010004:
			DisplayString(3,5,0,"A��ʧѹ�¼�");
			DisplayString(4,9,0,"E2010004");
		
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010005:
			DisplayString(3,5,0,"B��ʧѹ�¼�");
			DisplayString(4,9,0,"E2010005");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010006:
			DisplayString(3,5,0,"C��ʧѹ�¼�");
			DisplayString(4,9,0,"E2010006");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010007:
			DisplayString(3,5,0,"A��ʧ���¼�");
			DisplayString(4,9,0,"E2010007");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010008:
			DisplayString(3,5,0,"B��ʧ���¼�");
			DisplayString(4,9,0,"E2010008");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010009:
			DisplayString(3,5,0,"C��ʧ���¼�");
			DisplayString(4,9,0,"E2010009");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000B:
			DisplayString(3,5,0,"A������¼�");
			DisplayString(4,9,0,"E201000B");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000C:
			DisplayString(3,5,0,"B������¼�");
			DisplayString(4,9,0,"E201000C");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000D:
			DisplayString(3,5,0,"C������¼�");
			DisplayString(4,9,0,"E201000D");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000A:
			DisplayString(3,5,0,"����/�ϵ��¼�");
			DisplayString(4,9,0,"E201000A");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //�ϵ�ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE201000F:
			DisplayString(3,5,0,"���������¼�");
			DisplayString(4,9,0,"E201000F");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010010:
			DisplayString(3,5,0,"Уʱ�¼�");
			DisplayString(4,9,0,"E2010010");
			break;							
		case 0xE2010011:
			DisplayString(3,5,0,"����ť�п����¼�");
			DisplayString(4,9,0,"E2010011");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010012:
			DisplayString(3,5,0,"���ǿ����¼�");
			DisplayString(4,9,0,"E2010012");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010015:
			DisplayString(3,5,0,"�������俪���¼�");
			DisplayString(4,9,0,"E2010015");
			
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+15*2);                           //����ʱ��	 ��							
			DisplayData(EventData[8],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+12*2);                           //��
			DisplayData(EventData[9],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+9*2);                            //ʱ
			DisplayData(EventData[10],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+6*2);                            //��
			DisplayData(EventData[11],2,0,(u16*)p);									
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+3*2);	                          //��
			DisplayData(EventData[12],2,0,(u16*)p);										
			p=(u8*)(ADDR_STRINGBUFF+8*84+4+0*2);                            //��
			DisplayData(EventData[13],2,0,(u16*)p);
			break;
		case 0xE2010013:
			DisplayString(3,5,0,"��Ϣ��֤�����¼�");
			DisplayString(4,9,0,"E2010013");
			break;
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:       EventRecordDealFunction(u32 DataID)
// Descriptions:        �¼���¼��������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void EventRecordDealFunction(u32 DataID)
{	
	u8 *p8;
	u8 ReadData[SIZE_512];
	u32 EventNum = 0, DataLen = 0;
	AlarmEventFileHeadStruct EventHead;	
	StrBuff_TypeDef* DispBuf = StrBuff;	
	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{	
		case 0://����0:��ʼ��
			DisplayClr();//��ȫ��	
			MenuCount->v1=0;
			MenuCount->v2=0;		
			switch(DataID)
			{
				case 0xE2010003://���������¼�
					EventNum = NAND_GetEventRecordNum(0xE2010003);					
					break;
				case 0xE2010001://����¼�
					EventNum = NAND_GetEventRecordNum(0xE2010001);
					break;
				case 0xE2010004://A��ʧѹ�¼�
					EventNum = NAND_GetEventRecordNum(0xE2010004);
					break;
				case 0xE2010005://B��ʧѹ�¼�
					EventNum = NAND_GetEventRecordNum(0xE2010005);
					break;
				case 0xE2010006://C��ʧѹ�¼�
					EventNum = NAND_GetEventRecordNum(0xE2010006);
					break;
				case 0xE2010007://A��ʧ���¼�
					EventNum = NAND_GetEventRecordNum(0xE2010007);
					break;
				case 0xE2010008://B��ʧ���¼�
					EventNum = NAND_GetEventRecordNum(0xE2010008);
					break;
				case 0xE2010009://C��ʧ���¼�
					EventNum = NAND_GetEventRecordNum(0xE2010009);
					break;
				case 0xE201000B://A������¼�
					EventNum = NAND_GetEventRecordNum(0xE201000B);
					break;
				case 0xE201000C://B������¼�
					EventNum = NAND_GetEventRecordNum(0xE201000C);
					break;
				case 0xE201000D://C������¼�
					EventNum = NAND_GetEventRecordNum(0xE201000D);
					break;
				case 0xE201000A://�ն˵�/�ϵ��¼�
					EventNum = NAND_GetEventRecordNum(0xE201000A);
					break;
				case 0xE201000F://���������¼�
					EventNum = NAND_GetEventRecordNum(0xE201000F);
					break;
				case 0xE2010010://Уʱ�¼�
					EventNum = NAND_GetEventRecordNum(0xE2010010);
					break;							
				case 0xE2010011://����ť�п����¼�
					EventNum = NAND_GetEventRecordNum(0xE2010011);
					break;
				case 0xE2010012://���ǿ����¼�
					EventNum = NAND_GetEventRecordNum(0xE2010012);
					break;
				case 0xE2010015://�������俪���¼�
					EventNum = NAND_GetEventRecordNum(0xE2010015);
					break;
				case 0xE2010013://��Ϣ��֤�����¼�
					EventNum = NAND_GetEventRecordNum(0xE2010013);
					break;
			}		
			
			if(EventNum > 10) EventNum = 10;		
		  if(EventNum == 0)
			{
				switch(DataID)
				{
					case 0xE2010003://���������¼�
						DisplayString(4,1,0,"�����������¼���¼");				
						break;
					case 0xE2010001://����¼�
						DisplayString(4,3,0,"�ޱ���¼���¼");	
						break;
					case 0xE2010004://A��ʧѹ�¼�
						DisplayString(4,1,0,"��A��ʧѹ�¼���¼");	
						break;
					case 0xE2010005://B��ʧѹ�¼�
						DisplayString(4,1,0,"��B��ʧѹ�¼���¼");	
						break;
					case 0xE2010006://C��ʧѹ�¼�
						DisplayString(4,1,0,"��C��ʧѹ�¼���¼");	
						break;
					case 0xE2010007://A��ʧ���¼�
						DisplayString(4,1,0,"��A��ʧ���¼���¼");	
						break;
					case 0xE2010008://B��ʧ���¼�
						DisplayString(4,1,0,"��B��ʧ���¼���¼");	
						break;
					case 0xE2010009://C��ʧ���¼�
						DisplayString(4,1,0,"��C��ʧ���¼���¼");	
						break;
					case 0xE201000B://A������¼�
						DisplayString(4,1,0,"��A������¼���¼");	
						break;
					case 0xE201000C://B������¼�
						DisplayString(4,1,0,"��B������¼���¼");	
						break;
					case 0xE201000D://C������¼�
						DisplayString(4,1,0,"��C������¼���¼");	
						break;
					case 0xE201000A://�ն˵�/�ϵ��¼�
						DisplayString(4,0,0,"���ն˵���/�ϵ��¼�");	
						break;
					case 0xE201000F://���������¼�
						DisplayString(4,1,0,"�޵��������¼���¼");	
						break;
					case 0xE2010010://Уʱ�¼�
						DisplayString(4,3,0,"��Уʱ�¼���¼");	
						break;							
					case 0xE2010011://����ť�п����¼�
						DisplayString(4,1,0,"�ޱ���ť�п����¼�");	
						break;
					case 0xE2010012://���ǿ����¼�
						DisplayString(4,1,0,"�ޱ��ǿ����¼���¼");	
						break;
					case 0xE2010015://�������俪���¼�
						DisplayString(4,1,0,"�޼������俪���¼�");	
						break;
					case 0xE2010013://��Ϣ��֤�����¼�
						DisplayString(4,1,0,"����Ϣ��֤�����¼�");	
						break;
				}
				Stack->MenuStack[MenuCount->Count].Task = 2;
			}
			else
			{			
				MenuCount->v2 = EventNum;
				MenuCount->v1 = EventNum;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			break;
		case 1:
			DisplayClr();//��ȫ��	
			DisplayString(1,0,0,"��   ��/��   ���¼� "); 
			DisplayString(2,0,0,"��ַ:            \n\r"); 
			DisplayString(3,0,0,"����:            \n\r");
			DisplayString(4,0,0,"�¼�����:        \n\r");
			DisplayString(6,0,0,"  -  -     :  :  ");		
			switch(DataID)
			{
				case 0xE2010003://���������¼�
					DisplayString(5,0,0,"����ʱ��:        \n\r"); 		 								
					break;
				case 0xE2010001://����¼�
					DisplayString(5,0,0,"���ʱ��:        \n\r"); 	
					break;
				case 0xE2010004://A��ʧѹ�¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010005://B��ʧѹ�¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE2010006://C��ʧѹ�¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010007://A��ʧ���¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010008://B��ʧ���¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE2010009://C��ʧ���¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000B://A������¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000C://B������¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000D://C������¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE201000A://�ն˵�/�ϵ��¼�
					DisplayString(5,0,0,"����ʱ��:        \n\r"); 
					DisplayString(7,0,0,"�ϵ�ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE201000F://���������¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");						
					break;
				case 0xE2010010://Уʱ�¼�
					DisplayString(5,0,0,"Уʱ����ʱ��:        \n\r"); 		
					break;							
				case 0xE2010011://����ť�п����¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");					
					break;
				case 0xE2010012://���ǿ����¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");				
					break;
				case 0xE2010015://�������俪���¼�
					DisplayString(5,0,0,"��ʼʱ��:        \n\r"); 
					DisplayString(7,0,0,"����ʱ��:        \n\r");
					DisplayString(8,0,0,"  -  -     :  :  ");				
					break;
				case 0xE2010013://��Ϣ��֤�����¼�
					DisplayString(5,0,0,"����ʱ��:        \n\r"); 
					break;
			}

			EventNum = MenuCount->v1;
			p8 = (u8*)&DispBuf->Row[1].Col[2];
			bcd_p8(p8, EventNum, 3);

			if(MenuCount->v2 > 10)
			{
				MenuCount->v2 = 10;
			}
			EventNum = MenuCount->v2;
			p8 = (u8*)&DispBuf->Row[1].Col[10];
			bcd_p8(p8, EventNum, 3);
			
			switch(DataID)
			{
				case 0xE2010003://���������¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010003, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);
					break;
				case 0xE2010001://����¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010001, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);
					break;
				case 0xE2010004://A��ʧѹ�¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010004, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010005://B��ʧѹ�¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010005, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010006://C��ʧѹ�¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010006, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010007://A��ʧ���¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010007, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);				
					break;
				case 0xE2010008://B��ʧ���¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010008, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010009://C��ʧ���¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010009, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);	
					break;
				case 0xE201000B://A������¼�
					NAND_ReadEventRecordByLatelyNum(0xE201000B, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);		
					break;
				case 0xE201000C://B������¼�
					NAND_ReadEventRecordByLatelyNum(0xE201000C, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);		
					break;
				case 0xE201000D://C������¼�
					NAND_ReadEventRecordByLatelyNum(0xE201000D, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE201000A://�ն˵���/�ϵ��¼�
					NAND_ReadEventRecordByLatelyNum(0xE201000A, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);		
					break;
				case 0xE201000F://���������¼�
					NAND_ReadEventRecordByLatelyNum(0xE201000F, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);					
					break;
				case 0xE2010010://Уʱ�¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010010, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen); 		
					break;							
				case 0xE2010011://����ť�п����¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010011, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
				case 0xE2010012://���ǿ����¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010012, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);	
					break;
				case 0xE2010015://�������俪���¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010015, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);	
					break;
				case 0xE2010013://��Ϣ��֤�����¼�
					NAND_ReadEventRecordByLatelyNum(0xE2010013, EventNum+1-MenuCount->v1, &EventHead, ReadData, &DataLen);			
					break;
			}			
			DispEventDataInfo(&EventHead, ReadData, DataLen);	 									
			
			Stack->MenuStack[MenuCount->Count].Task = 2;
			Stack->MenuStack[MenuCount->Count].EditRow = 1;
			NegativeLnMaxColMinCol(Stack->MenuStack[MenuCount->Count].EditRow,4,2);
			break;
		case 2://����1:ѡ��
				if(Stack->MenuStack[MenuCount->Count].EditRow == 1)
				{
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{// �Ҽ�
						Comm_Ram->ButtonVal &= ~0x08;
						EventNum = MenuCount->v1;			
						if(EventNum < MenuCount->v2)
						{
							MenuCount->v1++;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
						if((EventNum != 0)&&(EventNum == MenuCount->v2))
						{
							MenuCount->v1 = 1;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
					}

					if((Comm_Ram->ButtonVal&0x04)!=0)
					{// ���
						Comm_Ram->ButtonVal &= ~0x04;
						EventNum = MenuCount->v1;			
						if(EventNum > 1)
						{
							MenuCount->v1--;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
						if(EventNum == 1)
						{
							MenuCount->v1 = MenuCount->v2;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}		
					}				
				}		
//			}
			if((Comm_Ram->ButtonVal&0x20)!=0)
			{//����
				MenuDel();//�����ϼ��˵�
			}
			Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
			break;
		default:
			break;
	}
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_1
// Descriptions:        �����¼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_1(void)//�����¼�
{
	EventRecordDealFunction(0xE2010003);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_2
// Descriptions:        ����¼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_2(void)//����¼�
{
	EventRecordDealFunction(0xE2010001);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3
// Descriptions:        ʧѹ�¼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_3(void)//ʧѹ�¼�
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_3);//��ʾ�˵�
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
				break;
			case 1://����1:ѡ��
				MenuSelect();//�˵�ѡ��
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_3_1);//�����¼��˵�  1.A��ʧѹ�¼�
							break;
						case 2:
							MenuCreate((u32)Menu_5_3_2);//�����¼��˵�  2.B��ʧѹ�¼�
							break;
						case 3:
							MenuCreate((u32)Menu_5_3_3);//�����¼��˵�  3.C��ʧѹ�¼�
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
					MenuDel();//�����ϼ��˵�
				}
				Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
				break;
			default:
				break;
		}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3_1
// Descriptions:        A��ʧѹ�¼�(E2010004)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_3_1(void)//A��ʧѹ�¼�
{
	EventRecordDealFunction(0xE2010004);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3_2
// Descriptions:        B��ʧѹ�¼�(E2010005)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_3_2(void)//B��ʧѹ�¼�
{
	EventRecordDealFunction(0xE2010005);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_3_3
// Descriptions:        C��ʧѹ�¼�(E2010006)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_3_3(void)//C��ʧѹ�¼�
{
	EventRecordDealFunction(0xE2010006);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4
// Descriptions:        ʧ���¼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_4(void)//ʧ���¼�
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_4);//��ʾ�˵�
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
				break;
			case 1://����1:ѡ��
				MenuSelect();//�˵�ѡ��
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_4_1);//�����¼��˵�  1.A��ʧ���¼�
							break;
						case 2:
							MenuCreate((u32)Menu_5_4_2);//�����¼��˵�  2.B��ʧ���¼�
							break;
						case 3:
							MenuCreate((u32)Menu_5_4_3);//�����¼��˵�  3.C��ʧ���¼�
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
					MenuDel();//�����ϼ��˵�
				}
				Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
				break;
			default:
				break;
		}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4_1
// Descriptions:        A��ʧ���¼�(E2010007)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_4_1(void)//A��ʧ���¼�
{
	EventRecordDealFunction(0xE2010007);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4_2
// Descriptions:        B��ʧ���¼�(E2010008)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_4_2(void)//B��ʧ���¼�
{
	EventRecordDealFunction(0xE2010008);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_4_3
// Descriptions:        C��ʧ���¼�(E2010009)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_4_3(void)//C��ʧ���¼�
{
	EventRecordDealFunction(0xE2010009);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}
//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5
// Descriptions:        �����¼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_5(void)//�����¼�
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5_5);//��ʾ�˵�
				Arrowhead();
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
				break;
			case 1://����1:ѡ��
				MenuSelect();//�˵�ѡ��
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_5_1);//�����¼��˵�  1.A������¼�
							break;
						case 2:
							MenuCreate((u32)Menu_5_5_2);//�����¼��˵�  2.B������¼�
							break;
						case 3:
							MenuCreate((u32)Menu_5_5_3);//�����¼��˵�  3.C������¼�
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
					MenuDel();//�����ϼ��˵�
				}
				Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
				break;
			default:
				break;
		}
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5_1
// Descriptions:        A������¼�(E201000B)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_5_1(void)//A������¼�
{
	EventRecordDealFunction(0xE201000B);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5_2
// Descriptions:        B������¼�(E201000C)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_5_2(void)//B������¼�
{
	EventRecordDealFunction(0xE201000C);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_5_3
// Descriptions:        C������¼�(E201000D)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_5_3(void)//C������¼�
{
	EventRecordDealFunction(0xE201000D);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_6
// Descriptions:        ����/�ϵ��¼�(E201000A)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_6(void)//����/�ϵ��¼�
{
	EventRecordDealFunction(0xE201000A);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_7(E201000F)
// Descriptions:        �����¼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_7(void)//�����¼�
{
	EventRecordDealFunction(0xE201000F);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_8(E2010010)
// Descriptions:        Уʱ�¼�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_8(void)//Уʱ�¼�
{
	EventRecordDealFunction(0xE2010010);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_9
// Descriptions:        ����ť�п����¼�(E2010011)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_9(void)//����ť�п����¼�
{
	EventRecordDealFunction(0xE2010011);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_10
// Descriptions:        ���ǿ����¼�(E2010012)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_10(void)//���ǿ����¼�
{
	EventRecordDealFunction(0xE2010012);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_11
// Descriptions:        ���ſ����¼�(E2010015)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_11(void)//���ſ����¼�
{
	EventRecordDealFunction(0xE2010015);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5_12
// Descriptions:        ��Ϣ��֤�����¼�(E2010013)
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5_12(void)//��Ϣ��֤�����¼�
{
	EventRecordDealFunction(0xE2010013);
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_5
// Descriptions:        �¼���¼
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_5(void)//�¼���¼
{
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayClr();//��ȫ��
				DisplayMenu_CSG(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_5);//��ʾ�˵�
				MenuNegative(Stack->MenuStack[MenuCount->Count].EditRow);//���Բ˵���
				break;
			case 1://����1:ѡ��
				MenuSelect();//�˵�ѡ��
				if((Comm_Ram->ButtonVal&0x10)!=0)
				{//ȷ��
					switch(Stack->MenuStack[MenuCount->Count].EditRow+Stack->MenuStack[MenuCount->Count].MoveRow)
					{
						case 1:
							MenuCreate((u32)Menu_5_1);//�����¼��˵�  1.�����¼�
							break;
						case 2:
							MenuCreate((u32)Menu_5_2);//�����¼��˵�  2.����¼�
							break;
						case 3:
							MenuCreate((u32)Menu_5_3);//�����¼��˵�  3.ʧѹ�¼�
							break;
						case 4:
							MenuCreate((u32)Menu_5_4);//�����¼��˵�  4.ʧ���¼�
							break;
						case 5:
							MenuCreate((u32)Menu_5_5);//�����¼��˵�  5.�����¼�
							break;
						case 6:
							MenuCreate((u32)Menu_5_6);//�����¼��˵�  6.����/�ϵ��¼�
							break;
						case 7:						
							MenuCreate((u32)Menu_5_7);//�����¼��˵�  7.�����¼�
							break;
						case 8:
							MenuCreate((u32)Menu_5_8);//�����¼��˵�  8.Уʱ�¼�
							break;
						case 9:
							MenuCreate((u32)Menu_5_9);//�����¼��˵�  9.����ť�п����¼�
							break;
						case 10:
							MenuCreate((u32)Menu_5_10);//�����¼��˵�  10.���ǿ����¼�
							break;
						case 11:
							MenuCreate((u32)Menu_5_11);//�����¼��˵�  11.���ſ����¼�
							break;
						case 12:
							MenuCreate((u32)Menu_5_12);//�����¼��˵�  12.��Ϣ��֤�����¼�
							break;
						default:
							break;
					}        
				}
				if((Comm_Ram->ButtonVal&0x20)!=0)
				{//����
					MenuDel();//�����ϼ��˵�
				}
				Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
				break;
			default:
				break;
		}
}
