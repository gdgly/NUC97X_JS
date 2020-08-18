/*************************************************
��Ȩ:��̩�Ǳ������ն˲�
�ļ�:Menu_6_AbnormalAlarm.c
����:zyl
�汾:1.00
����:20170717
����:�������ز˵�6�쳣�澯�����ļ�
*************************************************/



//�˵�6 �쳣�澯
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../Display/Display.h"
#include "Menu.h"
#include "Menu6.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "../Device/RTC.h"



//-------------------------------------------------------------------------------------------------
// Function name:       Menu_6
// Descriptions:        �쳣�澯
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void DispAlarmDataInfo(AlarmEventFileHeadStruct* AlarmHead, u8* AlarmData, u32 DataLen)
{	
	u8* p8;
	//u32 Pn = 0;
	u64 x = 0;
	StrBuff_TypeDef* DispBuf = StrBuff;	
	
	//Pn = FindMeterInNodeList(AlarmHead->MeterAddr, (NodeListStruct*)ADDR_NodeList, MAX_PN_NUM);
	p8 = (u8*)&DispBuf->Row[2].Col[5];
	//bcd_p8(p8, Pn, 4);

	MR((u32)&x, (u32)AlarmHead->MeterAddr, METER_ADDR_SIZE);
//	x = bcd16_hex(x);
//	bcd_p8(p8, x, 12);
		DisplayData(x,12,0,(u16*)p8);		
	
	if(AlarmData[0]==0x01)                        //�澯״̬
	{
		DisplayString(5,9,0,"����");
	}
	else //(AlarmData[0]==0x00)
	{
		DisplayString(5,9,0,"�ָ�");
	}

	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+15*2);   //�澯����ʱ��	 ��							
	DisplayData(AlarmData[4],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+12*2);   //��
	DisplayData(AlarmData[5],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+9*2);    //ʱ
	DisplayData(AlarmData[6],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+6*2);    //��
	DisplayData(AlarmData[7],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+3*2);	   //��
	DisplayData(AlarmData[8],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+0*2);    //��
	DisplayData(AlarmData[9],2,0,(u16*)p8);
	
	switch(AlarmHead->DataID)
	{
		case 0xE2000001:
			DisplayString(3,5,0,"����װ���ſ���");		 //�澯����	
			DisplayString(4,9,0,"E20000001");
			break;
		case 0xE2000002:
			DisplayString(3,5,0,"����������");
			DisplayString(4,9,0,"E2000002");
			break;
		case 0xE2000003:
			DisplayString(3,5,0,"��ѹ������");
			DisplayString(4,9,0,"E2000003");
			break;
		case 0xE2000004:
			DisplayString(3,5,0,"������ƽ��");
			DisplayString(4,9,0,"E2000004");
			break;
		case 0xE2000005:
			DisplayString(3,5,0,"��ѹ��ƽ��");
			DisplayString(4,9,0,"E2000005");
			break;
		case 0xE2000006:
			DisplayString(3,5,0,"�������ƫ��");
			DisplayString(4,9,0,"E2000006");
			break;
		case 0xE2000007:
			DisplayString(3,5,0,"A��CT���ζ�·");
			DisplayString(4,9,0,"E2000007");
			break;
		case 0xE2000008:
			DisplayString(3,5,0,"B��CT���ζ�·");
			DisplayString(4,9,0,"E2000008");
			break;
		case 0xE2000009:
			DisplayString(3,5,0,"C��CT���ζ�·");
			DisplayString(4,9,0,"E2000009");
			break;
		case 0xE200000A:
			DisplayString(3,5,0,"A��CT���ο�·");
			DisplayString(4,9,0,"E200000A");
			break;
		case 0xE200000B:
			DisplayString(3,5,0,"B��CT���ο�·");
			DisplayString(4,9,0,"E200000B");
			break;
		case 0xE200000C:
			DisplayString(3,5,0,"C��CT���ο�·");
			DisplayString(4,9,0,"E200000C");
			break;
		case 0xE200000D:
			DisplayString(3,5,0,"A�ೱ������");
			DisplayString(4,9,0,"E200000D");
			break;
		case 0xE200000E:
			DisplayString(3,5,0,"B�ೱ������");
			DisplayString(4,9,0,"E200000E");
			break;							
		case 0xE200000F:
			DisplayString(3,5,0,"C�ೱ������");
			DisplayString(4,9,0,"E200000F");
			break;
		case 0xE2000010:
			DisplayString(3,5,0,"A���������");
			DisplayString(4,9,0,"E2000010");
			break;
		case 0xE2000011:
			DisplayString(3,5,0,"B���������");
			DisplayString(4,9,0,"E2000011");
			break;
		case 0xE2000012:
			DisplayString(3,5,0,"C���������");
			DisplayString(4,9,0,"E2000012");
			break;
		case 0xE2000013:
			DisplayString(3,5,0,"A�����ʧ��");
			DisplayString(4,9,0,"E2000013");
			break;
		case 0xE2000014:
			DisplayString(3,5,0,"B�����ʧ��");
			DisplayString(4,9,0,"E2000014");
			break;
		case 0xE2000015:
			DisplayString(3,5,0,"C�����ʧ��");
			DisplayString(4,9,0,"E2000015");
			break;
		case 0xE2000016:
			DisplayString(3,5,0,"A���ѹʧѹ");
			DisplayString(4,9,0,"E2000016");
			break;
		case 0xE2000017:
			DisplayString(3,5,0,"B���ѹʧѹ");
			DisplayString(4,9,0,"E2000017");
			break;							
		case 0xE2000018:
			DisplayString(3,5,0,"C���ѹʧѹ");
			DisplayString(4,9,0,"E2000018");
			break;
		case 0xE2000019:
			DisplayString(3,5,0,"ȫʧѹ");
			DisplayString(4,9,0,"E2000019");
			break;
		case 0xE200001A:
			DisplayString(3,5,0,"A���ѹ��ѹ");
			DisplayString(4,9,0,"E200001A");
			break;
		case 0xE200001B:
			DisplayString(3,5,0,"B���ѹ��ѹ");
			DisplayString(4,9,0,"E200001B");
			break;
		case 0xE200001C:
			DisplayString(3,5,0,"C���ѹ��ѹ");
			DisplayString(4,9,0,"E200001C");
			break;
		case 0xE200001D:
			DisplayString(3,5,0,"A���ѹ����");
			DisplayString(4,9,0,"E200001D");
			break;
		case 0xE200001E:
			DisplayString(3,5,0,"B���ѹ����");
			DisplayString(4,9,0,"E200001E");
			break;
		case 0xE200001F:
			DisplayString(3,5,0,"C���ѹ����");
			DisplayString(4,9,0,"E200001F");
			break;
		case 0xE2000020:
			DisplayString(3,5,0,"A���ѹ����");
			DisplayString(4,9,0,"E2000020");
			break;
		case 0xE2000021:
			DisplayString(3,5,0,"B���ѹ����");
			DisplayString(4,9,0,"E2000021");
			break;
		case 0xE2000022:
			DisplayString(3,5,0,"C���ѹ����");
			DisplayString(4,9,0,"E2000022");
			break;
		case 0xE2000023:
			DisplayString(3,5,0,"A���������");
			DisplayString(4,9,0,"E2000023");
			break;
		case 0xE2000024:
			DisplayString(3,5,0,"B���������");
			DisplayString(4,9,0,"E2000024");
			break;
		case 0xE2000025:
			DisplayString(3,5,0,"C���������");
			DisplayString(4,9,0,"E2000025");
			break;
		case 0xE2000026:
			DisplayString(3,5,0,"�޹�������");
			DisplayString(4,9,0,"E2000026");
			break;
		case 0xE2000027:
			DisplayString(3,5,0,"�޹�Ƿ����");
			DisplayString(4,9,0,"E2000027");
			break;
		case 0xE2000028:
			DisplayString(3,5,0,"���ʳ���ֵ");
			DisplayString(4,9,0,"E2000028");
			break;
		case 0xE2000029:
			DisplayString(3,5,0,"���ɹ���");
			DisplayString(4,9,0,"E2000029");
			break;
		case 0xE200002A:
			DisplayString(3,5,0,"����ͬ�����õ�");
			DisplayString(4,9,0,"E200002A");
			break;
		case 0xE200002B:
			DisplayString(3,5,0,"ʣ���Ѳ���");
			DisplayString(4,9,0,"E200002B");
			break;
		case 0xE200002C:
			DisplayString(3,5,0,"ʾ���½�");
			DisplayString(4,9,0,"E200002C");
			break;
		case 0xE200002D:
			DisplayString(3,5,0,"���ܱ����");
			DisplayString(4,9,0,"E200002D");
			break;
		case 0xE200002E:
			DisplayString(3,5,0,"���ܱ�ͣ��");
			DisplayString(4,9,0,"E200002E");
			break;
		case 0xE200002F:
			DisplayString(3,5,0,"���ܱ�ͨѶʧ��");
			DisplayString(4,9,0,"E200002F");
			break;
		case 0xE2000030:
			DisplayString(3,5,0,"��澯");
			DisplayString(4,9,0,"E2000030");
			break;
		case 0xE2000031:
			DisplayString(3,5,0,"�����������");
			DisplayString(4,9,0,"E2000031");
			break;
		case 0xE2000032:
			DisplayString(3,5,0,"ʱ�ӵ�ص�ѹ��");
			DisplayString(4,9,0,"E2000032");
			break;
		case 0xE2000033://�ն˵���								
			DisplayString(3,5,0,"�ն˵���");           //�澯����								
			DisplayString(4,9,0,"E2000033");           //�澯����													
			break;
		case 0xE2000034://�ն��ϵ�					
			DisplayString(3,5,0,"�ն��ϵ�");           //�澯����
			DisplayString(4,9,0,"E2000034");           //�澯����
			break;
		case 0xE2000035:
			DisplayString(3,5,0,"���ʱ�����");
			DisplayString(4,9,0,"E2000035");
			break;
		case 0xE2000036:
			DisplayString(3,5,0,"ʱ�λ���ʸ���");
			DisplayString(4,9,0,"E2000036");
			break;
		case 0xE2000037:
			DisplayString(3,5,0,"���峣������");
			DisplayString(4,9,0,"E2000037");
			break;
		case 0xE2000038:
			DisplayString(3,5,0,"���������ʸ���");
			DisplayString(4,9,0,"E2000038");
			break;
		case 0xE2000039:
			DisplayString(3,5,0,"ң�ű�λ");
			DisplayString(4,9,0,"E2000039");
			break;
		case 0xE200003A:
			DisplayString(3,5,0,"��ͨ������Խ��");
			DisplayString(4,9,0,"E200003A");
			break;
		case 0xE200003B:
			DisplayString(3,5,0,"�̵�����λ");
			DisplayString(4,9,0,"E200003B");
			break;
		case 0xE200003C:
			DisplayString(3,5,0,"�������բʧ��");
			DisplayString(4,9,0,"E200003C");
			break;
		case 0xE200003D:
			DisplayString(3,5,0,"����ʧ��");
			DisplayString(4,9,0,"E200003D");
			break;
		case 0xE200003E:
			DisplayString(3,5,0,"���ܱ�ʱ���쳣");
			DisplayString(4,9,0,"E200003E");
			break;
		case 0xE200003F:
			DisplayString(3,5,0,"���ܱ�Уʱʧ��");
			DisplayString(4,9,0,"E200003F");
			break;
		case 0xE2000040:
			DisplayString(3,5,0,"����ʧѹ�ܴ���");
			DisplayString(4,9,0,"E2000040");
			break;
		case 0xE2000041:
			DisplayString(3,5,0,"����ʧ���ܴ���");
			DisplayString(4,9,0,"E2000041");
			break;
		case 0xE2000042:
			DisplayString(3,5,0,"���ೱ��������");
			DisplayString(4,9,0,"E2000042");
			break;
		case 0xE2000043:
			DisplayString(3,5,0,"������ܴ���");
			DisplayString(4,9,0,"E2000043");
			break;
		case 0xE2000044:
			DisplayString(3,5,0,"A���ѹƫ��Խ��");
			DisplayString(4,9,0,"E2000044");
			break;
		case 0xE2000045:
			DisplayString(3,5,0,"B���ѹƫ��Խ��");
			DisplayString(4,9,0,"E2000045");
			break;
		case 0xE2000046:
			DisplayString(3,5,0,"C���ѹƫ��Խ��");
			DisplayString(4,9,0,"E2000046");
			break;
		case 0xE2000047:
			DisplayString(3,5,0,"Ƶ��ƫ��Խ��");
			DisplayString(4,9,0,"E2000047");
			break;
		case 0xE2000048:
			DisplayString(3,5,0,"A������Խ��");
			DisplayString(4,9,0,"E2000048");
			break;
		case 0xE2000049:
			DisplayString(3,5,0,"B������Խ��");
			DisplayString(4,9,0,"E2000049");
			break;
		case 0xE200004A:
			DisplayString(3,5,0,"C������Խ��");
			DisplayString(4,9,0,"E200004A");
			break;
		case 0xE200004B:
			DisplayString(3,5,0,"��ѹ��ƽ��Խ��");
			DisplayString(4,9,0,"E200004B");
			break;
		case 0xE200004C:
			DisplayString(3,5,0,"����δ֪���");
			DisplayString(4,9,0,"E200004C");
			break;
		case 0xE200004D:
			DisplayString(3,5,0,"���ť�п���");
			DisplayString(4,9,0,"E200004D");
			break;
		case 0xE200004E:
			DisplayString(3,5,0,"��ǿ����澯");
			DisplayString(4,9,0,"E200004E");
			break;							
	}
}


void Menu_6(void)//�쳣�澯
{
	u8 *p8;
	u8 ReadData[SIZE_512];
	u32 AlarmNum = 0,DataLen = 0, OutFileName = 0;	
	StrBuff_TypeDef* DispBuf = StrBuff;	
	AlarmEventFileHeadStruct AlarmHead;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//�˵��������
	{			
		case 0://����0:��ʼ��	
			DisplayClr();//��ȫ��
			MenuCount->v1=0;
			MenuCount->v2=0;		
			AlarmNum = NAND_GetAlarmRecordNum();
			if(AlarmNum > ALARM_FILE_MAXDEEP) AlarmNum = ALARM_FILE_MAXDEEP;			
		  if(AlarmNum == 0)
			{
				DisplayString(4,1,0,"�ն˵�ǰ�޸澯����");				
				Stack->MenuStack[MenuCount->Count].Task = 2;
			}
			else
			{			
				MenuCount->v2 = AlarmNum;
				MenuCount->v1 = AlarmNum;
				Stack->MenuStack[MenuCount->Count].Task = 1;
			}
			break;
		case 1:
			DisplayClr();//��ȫ��	
			DisplayString(1,0,0,"��   ��/��   ���澯 "); 
			DisplayString(2,0,0,"��ַ:            \n\r"); 
			DisplayString(3,0,0,"����:           \n\r");
			DisplayString(4,0,0,"�澯����:       \n\r");
			DisplayString(5,0,0,"�澯״̬:       \n\r");
			DisplayString(6,0,0,"�澯����ʱ��:   \n\r"); 		 
			DisplayString(7,0,0,"  -  -     :  :  ");

			AlarmNum = MenuCount->v1;
			p8 = (u8*)&DispBuf->Row[1].Col[2];
			bcd_p8(p8, AlarmNum, 3);

			AlarmNum = MenuCount->v2;
			p8 = (u8*)&DispBuf->Row[1].Col[10];
			bcd_p8(p8, AlarmNum, 3);
			
			NAND_ReadAlarmRecordByLatelyNum((AlarmNum+1)-MenuCount->v1, &AlarmHead, ReadData, &DataLen, &OutFileName);			
			DispAlarmDataInfo(&AlarmHead, ReadData, DataLen);
			
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
						AlarmNum = MenuCount->v1;			
						if(AlarmNum < MenuCount->v2)
						{
							MenuCount->v1++;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
						if((AlarmNum != 0)&&(AlarmNum == MenuCount->v2))
						{
							MenuCount->v1 = 1;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
					}

					if((Comm_Ram->ButtonVal&0x04)!=0)
					{// ���
						Comm_Ram->ButtonVal &= ~0x04;
						AlarmNum = MenuCount->v1;			
						if(AlarmNum > 1)
						{
							MenuCount->v1--;
							Stack->MenuStack[MenuCount->Count].Task = 1;
						}
						if(AlarmNum == 1)
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

