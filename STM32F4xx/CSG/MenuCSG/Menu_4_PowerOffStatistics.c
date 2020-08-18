/*************************************************
��Ȩ:��̩�Ǳ������ն˲�
�ļ�:Menu_4_PowerOffStatistics.c
����:zyl
�汾:1.00
����:20170717
����:�������ز˵�4ͣ��ͳ�ƴ����ļ�
*************************************************/


//�˵�4 ͣ��ͳ��
#include "../Hcsg/ProjectCSG.h"
#include "../Hcsg/IRAM_TerminalSaveCSG.h"
#include "../Display/Display.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "Menu.h"
#include "Menu4.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"
#include "../QCSG_Head/QCSG_Hal.h"


const u8 pMenu_4[]=//�����ն�ͣ��ͳ������
{
	"\n\r"
	"��ǰͣ��ͳ������\n\r"
	"��ʷ��ͣ��ͳ������\n\r"
	"��ʷ��ͣ��ͳ������\n\r"
};

const u8 pMenu_4_1[][20]=
{
	"ͣ�����:      (��)",
	"ͣ��ʱ��:      (��)",
};

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_1
// Descriptions:        ���ա�����ͣ��ͳ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_1(void)//��ǰͣ��ͳ������
{
	u16 *p16;
	u32 i;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																															"\n\r"
																															"  ��ǰͣ��ͳ������ "
																															);//��ʾ�˵�	
	DisplayClrRow(Ln+3,Ln+7);//����Ļ��ʾ��ָ����		
	//����ͣ��ͳ������
	DisplayString(Ln+2,0,0,"����:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	//ͣ�����
	MR((u32)&Databuff,QCSG_POWER_DOWN_NUM_DAY,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+3,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	//ͣ��ʱ��
	MR((u32)&Databuff,QCSG_POWER_DOWN_TIME_DAY,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+4,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	
	//����ͣ��ͳ������
	DisplayString(Ln+5,0,0,"����:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+6+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	
	//ͣ�����
	MR((u32)&Databuff,QCSG_POWER_DOWN_NUM_MONTH,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+6,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+6)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	//ͣ��ʱ��
	MR((u32)&Databuff,QCSG_POWER_DOWN_TIME_MONTH,3);
	if(Databuff==0xFFFFFF)
	{
		DisplayString(Ln+7,9,0,"------");	
	}
	else
	{
		p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+7)*84+9*2);
		bcd_p8((u8*)p16,Databuff,6);
	}
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	


//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_2
// Descriptions:        ��ʷ��ͣ��ͳ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_2(void)//��ʷ��ͣ��ͳ������
{
	u16 *p16;
	u32 i,Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;
	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																															"\n\r"
																															" ��ʷ��ͣ��ͳ������ "
																															);//��ʾ�˵�
	DisplayClrRow(Ln+2,Ln+4);//����Ļ��ʾ��ָ����		
	//��ʷ��ͣ��ͳ������
	DisplayString(Ln+2,0,0,"��һ��:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneDay(&RealTime);
//		if(Ret==TRUE)
//		{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008030, &DataTime, PnData, &DataLen);
			//ͣ�����
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
			//ͣ��ʱ��
			NAND_ReadMeterAMRData(AMR_DailyStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE1008031, &DataTime, PnData, &DataLen);
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
		//}
	}			
		
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	

//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4_3
// Descriptions:        ��ʷ��ͣ��ͳ������
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4_3(void)//��ʷ��ͣ��ͳ������
{
	u16 *p16;
	u32 i,Ret;
	u32 DataLen = 0;
	u32 Ln = 0, Databuff = 0;   //�˵��ӵ�0�п�ʼ
	u8* PnData = (u8*)(ADDR_PnDataBuf);
	AMRFileHeadStruct AMRFileHead;
	RealTimeYYStruct DataTime;
	RealTimeStruct RealTime;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	QCSG_LCD_RAMDATA_S* Pn_Lcd_Data = (QCSG_LCD_RAMDATA_S*)ADDR_LCD_MeterAMRDataBuf;	
	
	DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)
																															"\n\r"
																															" ��ʷ��ͣ��ͳ������ "
																														);//��ʾ�˵�	
	DisplayClrRow(Ln+2,Ln+4);//����Ļ��ʾ��ָ����		
	//��ʷ��ͣ��ͳ������
	DisplayString(Ln+2,0,0,"��һ��:");
	for(i=0;i<=1;i++)
	{
		DisplayString(Ln+3+i,0,0,(u8 *)pMenu_4_1[i]);
	}
	
	Pn_Lcd_Data->u16Pn = 0x0000;
	Ret=HAL_GetSystemTime(&RealTime);
	if(Ret==TRUE)
	{
//		Ret=QCSG_DecOneMonth(&RealTime);
//		if(Ret==TRUE)
//		{
			RealTimeToYYTime(&RealTime, &DataTime);
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C030, &DataTime, PnData, &DataLen);
			//ͣ�����
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+3,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+3)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
			//ͣ��ʱ��
			NAND_ReadMeterAMRData(AMR_MonthStatisticFreeze, NodeList[Pn_Lcd_Data->u16Pn].Addr, &AMRFileHead, 0xE100C031, &DataTime, PnData, &DataLen);
			MR((u32)&Databuff,(u32)&PnData[0],3);
			if(Databuff==0xFFFFFF)
			{
				DisplayString(Ln+4,9,0,"------");	
			}
			else
			{
				p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln+4)*84+9*2);
				bcd_p8((u8*)p16,Databuff,6);
			}
		//}
	}			
	
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x2C;//��ʹ�ú���0
}	




//-------------------------------------------------------------------------------------------------
// Function name:       Menu_4
// Descriptions:        ͣ��ͳ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void Menu_4(void)
{
		switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
		{
			case 0://����0:��ʼ��
				Stack->MenuStack[MenuCount->Count].Task=1;
				DisplayMenu(Stack->MenuStack[MenuCount->Count].MoveRow,(u8 *)pMenu_4);//��ʾ�˵�
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
							MenuCreate((u32)Menu_4_1);
							break;
						case 2:
							MenuCreate((u32)Menu_4_2);
							break;
						case 3:
							MenuCreate((u32)Menu_4_3);
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
