/*************************************************
版权:正泰仪表智能终端部
文件:Menu_6_AbnormalAlarm.c
作者:zyl
版本:1.00
日期:20170717
描述:南网负控菜单6异常告警处理文件
*************************************************/



//菜单6 异常告警
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
// Descriptions:        异常告警
// input parameters:    无
// output parameters:   无
// Returned value:      无
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
	
	if(AlarmData[0]==0x01)                        //告警状态
	{
		DisplayString(5,9,0,"发生");
	}
	else //(AlarmData[0]==0x00)
	{
		DisplayString(5,9,0,"恢复");
	}

	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+15*2);   //告警发生时间	 秒							
	DisplayData(AlarmData[4],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+12*2);   //分
	DisplayData(AlarmData[5],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+9*2);    //时
	DisplayData(AlarmData[6],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+6*2);    //日
	DisplayData(AlarmData[7],2,0,(u16*)p8);									
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+3*2);	   //月
	DisplayData(AlarmData[8],2,0,(u16*)p8);										
	p8=(u8*)(ADDR_STRINGBUFF+7*84+4+0*2);    //年
	DisplayData(AlarmData[9],2,0,(u16*)p8);
	
	switch(AlarmHead->DataID)
	{
		case 0xE2000001:
			DisplayString(3,5,0,"计量装置门开闭");		 //告警名称	
			DisplayString(4,9,0,"E20000001");
			break;
		case 0xE2000002:
			DisplayString(3,5,0,"电流反极性");
			DisplayString(4,9,0,"E2000002");
			break;
		case 0xE2000003:
			DisplayString(3,5,0,"电压逆向序");
			DisplayString(4,9,0,"E2000003");
			break;
		case 0xE2000004:
			DisplayString(3,5,0,"电流不平衡");
			DisplayString(4,9,0,"E2000004");
			break;
		case 0xE2000005:
			DisplayString(3,5,0,"电压不平衡");
			DisplayString(4,9,0,"E2000005");
			break;
		case 0xE2000006:
			DisplayString(3,5,0,"零序电流偏大");
			DisplayString(4,9,0,"E2000006");
			break;
		case 0xE2000007:
			DisplayString(3,5,0,"A相CT二次短路");
			DisplayString(4,9,0,"E2000007");
			break;
		case 0xE2000008:
			DisplayString(3,5,0,"B相CT二次短路");
			DisplayString(4,9,0,"E2000008");
			break;
		case 0xE2000009:
			DisplayString(3,5,0,"C相CT二次短路");
			DisplayString(4,9,0,"E2000009");
			break;
		case 0xE200000A:
			DisplayString(3,5,0,"A相CT二次开路");
			DisplayString(4,9,0,"E200000A");
			break;
		case 0xE200000B:
			DisplayString(3,5,0,"B相CT二次开路");
			DisplayString(4,9,0,"E200000B");
			break;
		case 0xE200000C:
			DisplayString(3,5,0,"C相CT二次开路");
			DisplayString(4,9,0,"E200000C");
			break;
		case 0xE200000D:
			DisplayString(3,5,0,"A相潮流反向");
			DisplayString(4,9,0,"E200000D");
			break;
		case 0xE200000E:
			DisplayString(3,5,0,"B相潮流反向");
			DisplayString(4,9,0,"E200000E");
			break;							
		case 0xE200000F:
			DisplayString(3,5,0,"C相潮流反向");
			DisplayString(4,9,0,"E200000F");
			break;
		case 0xE2000010:
			DisplayString(3,5,0,"A相电流过流");
			DisplayString(4,9,0,"E2000010");
			break;
		case 0xE2000011:
			DisplayString(3,5,0,"B相电流过流");
			DisplayString(4,9,0,"E2000011");
			break;
		case 0xE2000012:
			DisplayString(3,5,0,"C相电流过流");
			DisplayString(4,9,0,"E2000012");
			break;
		case 0xE2000013:
			DisplayString(3,5,0,"A相电流失流");
			DisplayString(4,9,0,"E2000013");
			break;
		case 0xE2000014:
			DisplayString(3,5,0,"B相电流失流");
			DisplayString(4,9,0,"E2000014");
			break;
		case 0xE2000015:
			DisplayString(3,5,0,"C相电流失流");
			DisplayString(4,9,0,"E2000015");
			break;
		case 0xE2000016:
			DisplayString(3,5,0,"A相电压失压");
			DisplayString(4,9,0,"E2000016");
			break;
		case 0xE2000017:
			DisplayString(3,5,0,"B相电压失压");
			DisplayString(4,9,0,"E2000017");
			break;							
		case 0xE2000018:
			DisplayString(3,5,0,"C相电压失压");
			DisplayString(4,9,0,"E2000018");
			break;
		case 0xE2000019:
			DisplayString(3,5,0,"全失压");
			DisplayString(4,9,0,"E2000019");
			break;
		case 0xE200001A:
			DisplayString(3,5,0,"A相电压过压");
			DisplayString(4,9,0,"E200001A");
			break;
		case 0xE200001B:
			DisplayString(3,5,0,"B相电压过压");
			DisplayString(4,9,0,"E200001B");
			break;
		case 0xE200001C:
			DisplayString(3,5,0,"C相电压过压");
			DisplayString(4,9,0,"E200001C");
			break;
		case 0xE200001D:
			DisplayString(3,5,0,"A相电压断相");
			DisplayString(4,9,0,"E200001D");
			break;
		case 0xE200001E:
			DisplayString(3,5,0,"B相电压断相");
			DisplayString(4,9,0,"E200001E");
			break;
		case 0xE200001F:
			DisplayString(3,5,0,"C相电压断相");
			DisplayString(4,9,0,"E200001F");
			break;
		case 0xE2000020:
			DisplayString(3,5,0,"A相电压畸变");
			DisplayString(4,9,0,"E2000020");
			break;
		case 0xE2000021:
			DisplayString(3,5,0,"B相电压畸变");
			DisplayString(4,9,0,"E2000021");
			break;
		case 0xE2000022:
			DisplayString(3,5,0,"C相电压畸变");
			DisplayString(4,9,0,"E2000022");
			break;
		case 0xE2000023:
			DisplayString(3,5,0,"A相电流畸变");
			DisplayString(4,9,0,"E2000023");
			break;
		case 0xE2000024:
			DisplayString(3,5,0,"B相电流畸变");
			DisplayString(4,9,0,"E2000024");
			break;
		case 0xE2000025:
			DisplayString(3,5,0,"C相电流畸变");
			DisplayString(4,9,0,"E2000025");
			break;
		case 0xE2000026:
			DisplayString(3,5,0,"无功过补偿");
			DisplayString(4,9,0,"E2000026");
			break;
		case 0xE2000027:
			DisplayString(3,5,0,"无功欠补偿");
			DisplayString(4,9,0,"E2000027");
			break;
		case 0xE2000028:
			DisplayString(3,5,0,"功率超定值");
			DisplayString(4,9,0,"E2000028");
			break;
		case 0xE2000029:
			DisplayString(3,5,0,"负荷过载");
			DisplayString(4,9,0,"E2000029");
			break;
		case 0xE200002A:
			DisplayString(3,5,0,"超合同容量用电");
			DisplayString(4,9,0,"E200002A");
			break;
		case 0xE200002B:
			DisplayString(3,5,0,"剩余电费不足");
			DisplayString(4,9,0,"E200002B");
			break;
		case 0xE200002C:
			DisplayString(3,5,0,"示度下降");
			DisplayString(4,9,0,"E200002C");
			break;
		case 0xE200002D:
			DisplayString(3,5,0,"电能表飞走");
			DisplayString(4,9,0,"E200002D");
			break;
		case 0xE200002E:
			DisplayString(3,5,0,"电能表停走");
			DisplayString(4,9,0,"E200002E");
			break;
		case 0xE200002F:
			DisplayString(3,5,0,"电能表通讯失败");
			DisplayString(4,9,0,"E200002F");
			break;
		case 0xE2000030:
			DisplayString(3,5,0,"差动告警");
			DisplayString(4,9,0,"E2000030");
			break;
		case 0xE2000031:
			DisplayString(3,5,0,"最大需量复零");
			DisplayString(4,9,0,"E2000031");
			break;
		case 0xE2000032:
			DisplayString(3,5,0,"时钟电池电压低");
			DisplayString(4,9,0,"E2000032");
			break;
		case 0xE2000033://终端掉电								
			DisplayString(3,5,0,"终端掉电");           //告警名称								
			DisplayString(4,9,0,"E2000033");           //告警代码													
			break;
		case 0xE2000034://终端上电					
			DisplayString(3,5,0,"终端上电");           //告警名称
			DisplayString(4,9,0,"E2000034");           //告警代码
			break;
		case 0xE2000035:
			DisplayString(3,5,0,"编程时间更改");
			DisplayString(4,9,0,"E2000035");
			break;
		case 0xE2000036:
			DisplayString(3,5,0,"时段或费率更改");
			DisplayString(4,9,0,"E2000036");
			break;
		case 0xE2000037:
			DisplayString(3,5,0,"脉冲常数更改");
			DisplayString(4,9,0,"E2000037");
			break;
		case 0xE2000038:
			DisplayString(3,5,0,"互感器倍率更改");
			DisplayString(4,9,0,"E2000038");
			break;
		case 0xE2000039:
			DisplayString(3,5,0,"遥信变位");
			DisplayString(4,9,0,"E2000039");
			break;
		case 0xE200003A:
			DisplayString(3,5,0,"月通信流量越限");
			DisplayString(4,9,0,"E200003A");
			break;
		case 0xE200003B:
			DisplayString(3,5,0,"继电器变位");
			DisplayString(4,9,0,"E200003B");
			break;
		case 0xE200003C:
			DisplayString(3,5,0,"电表拉合闸失败");
			DisplayString(4,9,0,"E200003C");
			break;
		case 0xE200003D:
			DisplayString(3,5,0,"抄表失败");
			DisplayString(4,9,0,"E200003D");
			break;
		case 0xE200003E:
			DisplayString(3,5,0,"电能表时钟异常");
			DisplayString(4,9,0,"E200003E");
			break;
		case 0xE200003F:
			DisplayString(3,5,0,"电能表校时失败");
			DisplayString(4,9,0,"E200003F");
			break;
		case 0xE2000040:
			DisplayString(3,5,0,"三相失压总次数");
			DisplayString(4,9,0,"E2000040");
			break;
		case 0xE2000041:
			DisplayString(3,5,0,"三相失流总次数");
			DisplayString(4,9,0,"E2000041");
			break;
		case 0xE2000042:
			DisplayString(3,5,0,"三相潮流反向数");
			DisplayString(4,9,0,"E2000042");
			break;
		case 0xE2000043:
			DisplayString(3,5,0,"电表编程总次数");
			DisplayString(4,9,0,"E2000043");
			break;
		case 0xE2000044:
			DisplayString(3,5,0,"A相电压偏差越限");
			DisplayString(4,9,0,"E2000044");
			break;
		case 0xE2000045:
			DisplayString(3,5,0,"B相电压偏差越限");
			DisplayString(4,9,0,"E2000045");
			break;
		case 0xE2000046:
			DisplayString(3,5,0,"C相电压偏差越限");
			DisplayString(4,9,0,"E2000046");
			break;
		case 0xE2000047:
			DisplayString(3,5,0,"频率偏差越限");
			DisplayString(4,9,0,"E2000047");
			break;
		case 0xE2000048:
			DisplayString(3,5,0,"A相闪变越限");
			DisplayString(4,9,0,"E2000048");
			break;
		case 0xE2000049:
			DisplayString(3,5,0,"B相闪变越限");
			DisplayString(4,9,0,"E2000049");
			break;
		case 0xE200004A:
			DisplayString(3,5,0,"C相闪变越限");
			DisplayString(4,9,0,"E200004A");
			break;
		case 0xE200004B:
			DisplayString(3,5,0,"电压不平衡越限");
			DisplayString(4,9,0,"E200004B");
			break;
		case 0xE200004C:
			DisplayString(3,5,0,"发现未知电表");
			DisplayString(4,9,0,"E200004C");
			break;
		case 0xE200004D:
			DisplayString(3,5,0,"表端钮盒开启");
			DisplayString(4,9,0,"E200004D");
			break;
		case 0xE200004E:
			DisplayString(3,5,0,"表盖开启告警");
			DisplayString(4,9,0,"E200004E");
			break;							
	}
}


void Menu_6(void)//异常告警
{
	u8 *p8;
	u8 ReadData[SIZE_512];
	u32 AlarmNum = 0,DataLen = 0, OutFileName = 0;	
	StrBuff_TypeDef* DispBuf = StrBuff;	
	AlarmEventFileHeadStruct AlarmHead;

	
	switch(Stack->MenuStack[MenuCount->Count].Task)//菜单任务计数
	{			
		case 0://任务0:初始化	
			DisplayClr();//清全屏
			MenuCount->v1=0;
			MenuCount->v2=0;		
			AlarmNum = NAND_GetAlarmRecordNum();
			if(AlarmNum > ALARM_FILE_MAXDEEP) AlarmNum = ALARM_FILE_MAXDEEP;			
		  if(AlarmNum == 0)
			{
				DisplayString(4,1,0,"终端当前无告警数据");				
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
			DisplayClr();//清全屏	
			DisplayString(1,0,0,"第   条/总   条告警 "); 
			DisplayString(2,0,0,"地址:            \n\r"); 
			DisplayString(3,0,0,"名称:           \n\r");
			DisplayString(4,0,0,"告警代码:       \n\r");
			DisplayString(5,0,0,"告警状态:       \n\r");
			DisplayString(6,0,0,"告警发生时间:   \n\r"); 		 
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
		case 2://任务1:选择
				if(Stack->MenuStack[MenuCount->Count].EditRow == 1)
				{
					if((Comm_Ram->ButtonVal&0x08)!=0)
					{// 右键
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
					{// 左键
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
			{//返回
				MenuDel();//返回上级菜单
			}
			Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
			break;
		default:
			break;
	}
}

