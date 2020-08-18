
//南网终端Uart管理
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Uart/UartAuto_645.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../QCSG_Head/QCSG_Resolve.h"
#include "../QCSG_Data/QCSG_DefaultData.h"

#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../LIB/Ethernet.h"


#include "../QCSG_Uart/TerminalQCSG_Uart_Manage.h"
#include "../QCSG_Head/CH_AMR_TaskManage.h"
#include "../QCSG_Head/CH_PLC_AMR_Task.h" 
#include "../QCSG_Head/CH_RS485_AMR_Task.h" 
#include "../GPRS/GPRS_QCSG.h"
#include "../GPRS/ATNo.h"
#include "../GPRS/ATString.h"
#include "Terminal_Task.h"
#include "../TEST/LOG.h"

//2018.3.26 修改，修正485为抄表口时无法刷新.FUNCTION=1的问题
void Refresh485PortFunction(u32 PORTn, RS485PortParaStruct* PortPara)
{
	u8 Temp[4];
	u32 BpsCtrl;
	UARTCtrl_TypeDef * UARTCtrl;

	BpsCtrl = ConvertToBpsCtrl(PortPara->BaudRate, PortPara->Parity, PortPara->DataBit, PortPara->StopBit);
	switch((u8)PORTn)
	{
		case RS485_1PORT:
			UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
			MR((u32)Temp, ADDR_UART_PORT_SET + 4, 2);
			if(PortPara->Function == UART_FUNC_AMR)
			{
				if(Temp[0] == UART_FUNC_MAINTAIN)
				{
					Temp[0] = UART_FUNC_UNKNOWN;
					Temp[1] = (BpsCtrl >> 5) & 0x0F;
					MW((u32)Temp, ADDR_UART_PORT_SET + 4, 2);
				}	
				UARTCtrl->Function = 1;
			}
			else //if(PortPara->Function == UART_FUNC_UPLINK)
			{
				if(Temp[0] != UART_FUNC_MAINTAIN)
				{
					Temp[0] = UART_FUNC_MAINTAIN;
					Temp[1] = (BpsCtrl >> 5) & 0x0F;
					MW((u32)Temp, ADDR_UART_PORT_SET + 4, 2);
				}
				UARTCtrl->Function = UART_FUNC_MAINTAIN;
				if(UARTCtrl->BpsCtrl != BpsCtrl)
				{
					UARTCtrl->BpsCtrl = BpsCtrl;
					Init_UARTn(RS485_1PORT);
				}
			}
			break;
		case RS485_2PORT:
			UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
			MR((u32)Temp, ADDR_UART_PORT_SET + 8, 2);
			if(PortPara->Function == UART_FUNC_AMR)
			{
				if(Temp[0] == UART_FUNC_MAINTAIN)
				{
					Temp[0] = UART_FUNC_UNKNOWN;
					Temp[1] = (BpsCtrl >> 5) & 0x0F;
					MW((u32)Temp, ADDR_UART_PORT_SET + 8, 2);
				}	
				UARTCtrl->Function = 1;
			}
			else //if(PortPara->Function == UART_FUNC_UPLINK)
			{
				if(Temp[0] != UART_FUNC_MAINTAIN)
				{
					Temp[0] = UART_FUNC_MAINTAIN;
					Temp[1] = (BpsCtrl >> 5) & 0x0F;
					MW((u32)Temp, ADDR_UART_PORT_SET + 8, 2);
				}
				UARTCtrl->Function = UART_FUNC_MAINTAIN;
				if(UARTCtrl->BpsCtrl != BpsCtrl)
				{
					UARTCtrl->BpsCtrl = BpsCtrl;
					Init_UARTn(RS485_2PORT);
				}
			}
			break;
		case RS485_3PORT:
			UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
			MR((u32)Temp, ADDR_UART_PORT_SET + 12, 2);
			if(PortPara->Function == UART_FUNC_AMR)
			{
				if(Temp[0] == UART_FUNC_MAINTAIN)
				{
					Temp[0] = UART_FUNC_UNKNOWN;
					Temp[1] = (BpsCtrl >> 5) & 0x0F;
					MW((u32)Temp, ADDR_UART_PORT_SET + 12, 2);
				}	
				UARTCtrl->Function = 1;
			}
			else //if(PortPara->Function == UART_FUNC_UPLINK)
			{
				if(Temp[0] != UART_FUNC_MAINTAIN)
				{
					Temp[0] = UART_FUNC_MAINTAIN;
					Temp[1] = (BpsCtrl >> 5) & 0x0F;
					MW((u32)Temp, ADDR_UART_PORT_SET + 12, 2);
				}
				UARTCtrl->Function = UART_FUNC_MAINTAIN;
				if(UARTCtrl->BpsCtrl != BpsCtrl)
				{
					UARTCtrl->BpsCtrl = BpsCtrl;
					Init_UARTn(RS485_3PORT);
				}
			}
			break;
		default:
			break;
	}
}
//初始化
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=RS485-3
//UART5=载波
//UART6=红外
//UART7=
//UART8=
//UART9=
//UART10=以太网通信口号
//UART11=
//UART12=交流采样通信口号(虚拟),使用DATABUFF
//速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
void Terminal_UartInit_Manage(void)//Terminal_Uart初始化管理
{
  	UARTCtrl_TypeDef * UARTCtrl;
	RS485PortParaStruct PortPara;
	  
//RS232
	if(Get_LEN_UARTnCtrl(RS232PORT) != 0)//得到UART控制数据长度
	{
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS232PORT);
		UARTCtrl->BpsCtrl = RS232DefaultBPS;
		UARTCtrl->Function = UART_FUNC_MAINTAIN;
	  	Init_UARTn(RS232PORT);//初始化UART口,入口通信控制字已填入
	}
/*
//GPRS
	if(Get_LEN_UARTnCtrl(GPRSPORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(GPRSPORT);
		UARTCtrl->BpsCtrl = GPRSDefaultBPS;
		UARTCtrl->Function = UART_FUNC_MAINTAIN;
	  	Init_UARTn(GPRSPORT);//初始化UART口,入口通信控制字已填入
	}
*/

//RS485-1
	if(Get_LEN_UARTnCtrl(RS485_1PORT) != 0)//得到UART控制数据长度
	{
		MC(0, (u32)&PortPara, sizeof(RS485PortParaStruct));
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
		
		MR((u32)&PortPara, ADDR_RS485PortParaTbl, sizeof(RS485PortParaStruct));	
		Refresh485PortFunction(RS485_1PORT, &PortPara);
		
		if((PortPara.Function == UART_FUNC_ATTACH) || 
		   (PortPara.Function == UART_FUNC_SLAVE) || 
		   (PortPara.Function == UART_FUNC_UPLINK))
		{
			UARTCtrl->BpsCtrl = ConvertToBpsCtrl(PortPara.BaudRate, PortPara.Parity, PortPara.DataBit, PortPara.StopBit);
			UARTCtrl->Function = UART_FUNC_MAINTAIN;
		}
		else
		{
			UARTCtrl->BpsCtrl = RS485DefaultBPS;
			UARTCtrl->Function = 1;
		}		
	  	Init_UARTn(RS485_1PORT);
		Pin_OutH(PIN_UART5_OE);
	}
//RS485-2
	if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)//得到UART控制数据长度
	{
		MC(0, (u32)&PortPara, sizeof(RS485PortParaStruct));
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
		
		MR((u32)&PortPara, ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct), sizeof(RS485PortParaStruct));
		Refresh485PortFunction(RS485_2PORT, &PortPara);

		if((PortPara.Function == UART_FUNC_ATTACH) || 
		   (PortPara.Function == UART_FUNC_SLAVE) || 
		   (PortPara.Function == UART_FUNC_UPLINK))
		{
			UARTCtrl->BpsCtrl = ConvertToBpsCtrl(PortPara.BaudRate, PortPara.Parity, PortPara.DataBit, PortPara.StopBit);
			UARTCtrl->Function = UART_FUNC_MAINTAIN;	
		}
		else
		{
			UARTCtrl->BpsCtrl = RS485DefaultBPS;
			UARTCtrl->Function = 1;
		}
	  	Init_UARTn(RS485_2PORT);
		Pin_OutH(PIN_UART7_OE);
	}

	#if ((Project/100)==2)||((Project/100)==3)	||((Project/100)==9)	
		//RS485-3
		if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)//得到UART控制数据长度
		{
			MC(0, (u32)&PortPara, sizeof(RS485PortParaStruct));
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
			
			MR((u32)&PortPara, ADDR_RS485PortParaTbl + (2 * sizeof(RS485PortParaStruct)), sizeof(RS485PortParaStruct));
			Refresh485PortFunction(RS485_3PORT, &PortPara);

			if((PortPara.Function == UART_FUNC_ATTACH) || 
			   (PortPara.Function == UART_FUNC_SLAVE) || 
			   (PortPara.Function == UART_FUNC_UPLINK))
			{
				UARTCtrl->BpsCtrl = ConvertToBpsCtrl(PortPara.BaudRate, PortPara.Parity, PortPara.DataBit, PortPara.StopBit);
				UARTCtrl->Function = UART_FUNC_MAINTAIN;
				
			}
			else
			{
				UARTCtrl->BpsCtrl = RS485DefaultBPS;
				UARTCtrl->Function = 1;
			}
		  	Init_UARTn(RS485_3PORT);
			Pin_OutH(PIN_USART3_OE);
		}
	#endif	
	#if ((Project/100)==3)	//负控终端
		if(Get_LEN_UARTnCtrl(PLC_PORT)!=0)	// PLC 端口即是遥信端口
		{
			UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PLC_PORT);
			UARTCtrl->BpsCtrl = PLCDefaultBPS;
			UARTCtrl->Function = 1;
		  	Init_UARTn(PLC_PORT);	
		}
	#endif

//红外
	if(Get_LEN_UARTnCtrl(IRPORT)!=0)//得到UART控制数据长度
	{
		UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(IRPORT);
		UARTCtrl->BpsCtrl = IRDefaultBPS;
	  	Init_UARTn(IRPORT);
	}

}


//接收发送
//UART0=RS232
//UART1=GPRS
//UART2=RS485-1
//UART3=RS485-2
//UART4=RS485-3
//UART5=载波
//UART6=红外
//UART7=
//UART8=
//UART9=
//UART10=以太网通信口号
//UART11=
//UART12=交流采样通信口号(虚拟),使用DATABUFF
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
void Terminal_Uart_Manage(void)	//Terminal_Uart管理
{
	//u32 i;
  	//UARTCtrl_TypeDef * UARTCtrl;
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	//u32 Addr1, Addr2, Addr3, Addr4, Addr5, Addr6;
	//i = sizeof("String");
	//Addr1 = ADDR_QCSG_ERAM_END;
	//Addr2 = ADDR_QCSG_ERAM_START;
	//Addr3 = ADDR_UARTnCtrl_End;
	//Addr4 = ADDR_UARTnCtrl_Start;
	//Addr5 = ADDR_Terminal_ERAM_Start;
	//Addr6 = ADDR_UARTnCtrl_End;
	if(pComm_Ram->POWER == 3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		UartPortManage(RS232PORT, (UartTaskCtrlStruct*)ADDR_RS232_Task_Ctrl);
		UartTaskProcess(RS232PORT, (UartTaskCtrlStruct*)ADDR_RS232_Task_Ctrl);

		UartPortManage(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
		UartTaskProcess(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);

		UartPortManage(RS485_1PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH1_Task_Ctrl);
		UartTaskProcess(RS485_1PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH1_Task_Ctrl);

		UartPortManage(RS485_2PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH2_Task_Ctrl);
		UartTaskProcess(RS485_2PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH2_Task_Ctrl);

		UartPortManage(RS485_3PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH3_Task_Ctrl);
		UartTaskProcess(RS485_3PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH3_Task_Ctrl);
		
		UartPortManage(PLC_PORT, (UartTaskCtrlStruct*)ADDR_PLC_YX_UART_Task_Ctrl);
		UartTaskProcess(PLC_PORT, (UartTaskCtrlStruct*)ADDR_PLC_YX_UART_Task_Ctrl);

	}

	EthernetModuleManage(ENETPORT);

	GPRSModuleManage(GPRSPORT);
}
#endif

#if ((Project/100)==5)
void Terminal_Uart_Manage(void)	//Terminal_Uart管理
{
	//u32 i;
  	//UARTCtrl_TypeDef * UARTCtrl;
	Comm_Ram_TypeDef* pComm_Ram = Comm_Ram;
	//u32 Addr1, Addr2, Addr3, Addr4, Addr5, Addr6;
	//i = sizeof("String");
	//Addr1 = ADDR_QCSG_ERAM_END;
	//Addr2 = ADDR_QCSG_ERAM_START;
	//Addr3 = ADDR_UARTnCtrl_End;
	//Addr4 = ADDR_UARTnCtrl_Start;
	//Addr5 = ADDR_Terminal_ERAM_Start;
	//Addr6 = ADDR_UARTnCtrl_End;
	if(pComm_Ram->POWER == 3)//电源:0=电池供电,1=电网供电转电池供电重启动,2=电网供电转电池供电,3=电网供电
	{
		UartPortManage(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);
		UartTaskProcess(IRPORT, (UartTaskCtrlStruct*)ADDR_IR_Task_Ctrl);

		UartPortManage(RS485_1PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH1_Task_Ctrl);
		UartTaskProcess(RS485_1PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH1_Task_Ctrl);

		UartPortManage(RS485_2PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH2_Task_Ctrl);
		UartTaskProcess(RS485_2PORT, (UartTaskCtrlStruct*)ADDR_RS485_CH2_Task_Ctrl);

//		UartPortManage(PLC_PORT, (UartTaskCtrlStruct*)ADDR_PLC_YX_UART_Task_Ctrl);
//		UartTaskProcess(PLC_PORT, (UartTaskCtrlStruct*)ADDR_PLC_YX_UART_Task_Ctrl);
	}

	GPRSModuleManage(GPRSPORT);
}
#endif


void GPRSModuleManage(u32 PORTn)
{
	GPRS_QCSG_RxTx(GPRSPORT);	
	UartTaskProcess(GPRSPORT, (UartTaskCtrlStruct*)ADDR_GPRS_UART_Task_Ctrl);
	GPRSLinkMaintain(GPRSPORT);
}

void GPRSLinkMaintain(u32 PORTn)
{
	u32 Index = 0, FrameLen = 0;
	UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* UartTask;
	CommunicationParaStruct CommPara;
	QCSGFrameStruct SrcFrame;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	if((u8)PORTn != GPRSPORT) return;
	
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->LinkTask < 100) return;

	UartTask = GetUartTaskCtrl(PORTn);
	
	if((UARTCtrl->LinkTask == 100) && (pTerminalRam->GPRSHeartBeat_S_Timer == 0))
	{	
		MC(0, (u32)&SrcFrame, sizeof(QCSGFrameStruct));
		MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		SrcFrame.A1 = CommPara.TerminalLocationCode;
		SrcFrame.A2 = CommPara.TerminalAddr;
		SrcFrame.A3 = 0x0000;
		SrcFrame.AFN = 0x02;
		SrcFrame.SEQ = 0x70 | (UARTCtrl->ReTxSEQ & 0x0F);
		SrcFrame.FC = 0x00C9;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x00;
		
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x10;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0xE0;

		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x01;
		SrcFrame.DataLen = Index;
		FrameLen = QCSG_BuildFrameByStruct(&SrcFrame, (u8*)Get_ADDR_UARTnTx(PORTn));
		UartTask->AckLen = FrameLen;
		pTerminalRam->CopyTxByte = FrameLen;
		UartTask->Flag |= UART_FLAG_TX_ACK;	
		pTerminalRam->GPRSHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
		pTerminalRam->GPRSHeartBeat_S_Timer *= 60;
		if(pTerminalRam->GPRSHeartBeat_S_Timer == 0) pTerminalRam->GPRSHeartBeat_S_Timer = 15*60;
		UARTCtrl->HeartberatCount = 0;
		UARTCtrl->ReTxSEQ++;
		return;
	}

		
				
	if((UARTCtrl->LinkTask == 101) && (pTerminalRam->GPRSHeartBeat_S_Timer == 0))
	{
	
#if (TEST_ON == 0)
		if((UARTCtrl->Task &  UART_TASK_TX) != 0) return;	//err20180404-22 GPRS丢帧错误
		if((UartTask->Flag & UART_FLAG_TX_TASK_FRAME) != 0) return;
#else 
		
		//if((UARTCtrl->Task &  UART_TASK_TX) != 0) return;	//err20180404-22 GPRS丢帧错误
		//if((UartTask->Flag & UART_FLAG_TX_TASK_FRAME) != 0) return;
		if(((UARTCtrl->Task &  UART_TASK_TX) != 0)||((UARTCtrl->Task &  UART_TASK_TXING) != 0)//如果有数据通信，重置心跳时间
		   ||((UARTCtrl->Task &  UART_TASK_TX_DONE) != 0)||((UartTask->Flag & UART_FLAG_TX_TASK_FRAME) != 0))
		{
			pTerminalRam->GPRSHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
			pTerminalRam->GPRSHeartBeat_S_Timer *= 60;
			if(pTerminalRam->GPRSHeartBeat_S_Timer == 0) pTerminalRam->GPRSHeartBeat_S_Timer = 15*60;
			return;
	   	}
#endif	
		
		
		if(UARTCtrl->HeartberatCount > 2)
		{
			UARTCtrl->ATCount = AT_MYPOWEROFF;//关机
			UARTCtrl->ATTask = 1;//AT任务:0=空闲,1=发送,2=正在发送,3=接收
			pTerminalRam->GPRSHeartBeat_S_Timer = 60;
		}
		else
		{
			MC(0, (u32)&SrcFrame, sizeof(QCSGFrameStruct));
			MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
			MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			SrcFrame.A1 = CommPara.TerminalLocationCode;
			SrcFrame.A2 = CommPara.TerminalAddr;
			SrcFrame.A3 = 0x0000;
			SrcFrame.AFN = 0x02;
			SrcFrame.SEQ = 0x70 | (UARTCtrl->ReTxSEQ & 0x0F);
			SrcFrame.FC = 0x00C9;
			SrcFrame.Data[Index++] = 0x00;
			SrcFrame.Data[Index++] = 0x00;
			
			SrcFrame.Data[Index++] = 0x01;
			SrcFrame.Data[Index++] = 0x10;
			SrcFrame.Data[Index++] = 0x00;
			SrcFrame.Data[Index++] = 0xE0;

			SrcFrame.DataLen = Index;
			FrameLen = QCSG_BuildFrameByStruct(&SrcFrame, (u8*)Get_ADDR_UARTnTx(PORTn));
			pTerminalRam->CopyTxByte = FrameLen;
			UartTask->AckLen = FrameLen;
			UartTask->Flag |= UART_FLAG_TX_ACK;	
			pTerminalRam->GPRSHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
			pTerminalRam->GPRSHeartBeat_S_Timer *= 60;
			if(pTerminalRam->GPRSHeartBeat_S_Timer == 0) pTerminalRam->GPRSHeartBeat_S_Timer = 15*60;
			UARTCtrl->HeartberatCount++;
			UARTCtrl->ReTxSEQ++;
		}
		return;
	}
	if((UARTCtrl->LinkTask == 102) && (pTerminalRam->GPRSHeartBeat_S_Timer == 0))
	{
		MC(0, (u32)&SrcFrame, sizeof(QCSGFrameStruct));
		MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		SrcFrame.A1 = CommPara.TerminalLocationCode;
		SrcFrame.A2 = CommPara.TerminalAddr;
		SrcFrame.A3 = 0x0000;
		SrcFrame.AFN = 0x02;
		SrcFrame.SEQ = 0x70 | (UARTCtrl->ReTxSEQ & 0x0F);
		SrcFrame.FC = 0x00C9;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x00;
		
		SrcFrame.Data[Index++] = 0x02;
		SrcFrame.Data[Index++] = 0x10;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0xE0;

		SrcFrame.DataLen = Index;
		FrameLen = QCSG_BuildFrameByStruct(&SrcFrame, (u8*)Get_ADDR_UARTnTx(PORTn));
		pTerminalRam->CopyTxByte = FrameLen;
		UartTask->AckLen = FrameLen;
		UartTask->Flag |= UART_FLAG_TX_ACK;	
		pTerminalRam->GPRSHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
		pTerminalRam->GPRSHeartBeat_S_Timer *= 60;
		if(pTerminalRam->GPRSHeartBeat_S_Timer == 0) pTerminalRam->GPRSHeartBeat_S_Timer = 15*60;
		UARTCtrl->ReTxSEQ++;
	}
}

void EthernetModuleManage(u32 PORTn)
{
	#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
		Ethernet_RxTx();
	#endif
	UartPortManage(ENETPORT, (UartTaskCtrlStruct*)ADDR_ETHE_UART_Task_Ctrl);
	UartTaskProcess(ENETPORT, (UartTaskCtrlStruct*)ADDR_ETHE_UART_Task_Ctrl);
	EthernetLinkMaintain(ENETPORT);
}

void EthernetLinkMaintain(u32 PORTn)
{
	u32 Index = 0, FrameLen = 0;
	UARTCtrl_TypeDef* UARTCtrl;
	UartTaskCtrlStruct* UartTask;
	CommunicationParaStruct CommPara;
	QCSGFrameStruct SrcFrame;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;

	if((u8)PORTn != ENETPORT) return;
	
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->LinkTask < 100) return;

	UartTask = GetUartTaskCtrl(PORTn);

	//登录帧
	if((UARTCtrl->LinkTask == 100) && (pTerminalRam->ENETHeartBeat_S_Timer == 0))
	{	
		MC(0, (u32)&SrcFrame, sizeof(QCSGFrameStruct));
		MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		SrcFrame.A1 = CommPara.TerminalLocationCode;
		SrcFrame.A2 = CommPara.TerminalAddr;
		SrcFrame.A3 = 0x0000;
		SrcFrame.AFN = 0x02;
		SrcFrame.SEQ = 0x70 | (UARTCtrl->ReTxSEQ & 0x0F);
		SrcFrame.FC = 0x00C9;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x00;
		
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x10;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0xE0;

		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x01;
		SrcFrame.DataLen = Index;
		FrameLen = QCSG_BuildFrameByStruct(&SrcFrame, (u8*)Get_ADDR_UARTnTx(PORTn));
		UartTask->AckLen = FrameLen;
		UartTask->Flag |= UART_FLAG_TX_ACK;	
		//pTerminalRam->ENETHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
		//pTerminalRam->ENETHeartBeat_S_Timer *= 60;
		//if(pTerminalRam->ENETHeartBeat_S_Timer == 0) pTerminalRam->ENETHeartBeat_S_Timer = 15*60;
		pTerminalRam->ENETHeartBeat_S_Timer = 60;
		UARTCtrl->HeartberatCount = 0;
		UARTCtrl->ReTxSEQ++;
		return;
	}
//心跳帧
	if((UARTCtrl->LinkTask == 101) && (pTerminalRam->ENETHeartBeat_S_Timer == 0))
	{
		if((UartTask->Flag & UART_FLAG_TX_ACK) != 0) return;
		if((UartTask->Flag & UART_FLAG_TX_TASK_FRAME) != 0) return;
		
		if(UARTCtrl->HeartberatCount > 2)
		{
			UARTCtrl->LinkTask = 0;
		}
		else
		{
			MC(0, (u32)&SrcFrame, sizeof(QCSGFrameStruct));
			MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
			MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
			SrcFrame.A1 = CommPara.TerminalLocationCode;
			SrcFrame.A2 = CommPara.TerminalAddr;
			SrcFrame.A3 = 0x0000;
			SrcFrame.AFN = 0x02;
			SrcFrame.SEQ = 0x70 | (UARTCtrl->ReTxSEQ & 0x0F);
			SrcFrame.FC = 0x00C9;
			SrcFrame.Data[Index++] = 0x00;
			SrcFrame.Data[Index++] = 0x00;
			
			SrcFrame.Data[Index++] = 0x01;
			SrcFrame.Data[Index++] = 0x10;
			SrcFrame.Data[Index++] = 0x00;
			SrcFrame.Data[Index++] = 0xE0;

			SrcFrame.DataLen = Index;
			FrameLen = QCSG_BuildFrameByStruct(&SrcFrame, (u8*)Get_ADDR_UARTnTx(PORTn));
			UartTask->AckLen = FrameLen;
			UartTask->Flag |= UART_FLAG_TX_ACK;	
			pTerminalRam->ENETHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
			pTerminalRam->ENETHeartBeat_S_Timer *= 60;
			if(pTerminalRam->ENETHeartBeat_S_Timer == 0) pTerminalRam->ENETHeartBeat_S_Timer = 15*60;
			UARTCtrl->HeartberatCount++;
			UARTCtrl->ReTxSEQ++;
		}	
		return;
	}
//退出登录帧
	if((UARTCtrl->LinkTask == 102) && (pTerminalRam->ENETHeartBeat_S_Timer == 0))
	{
		MC(0, (u32)&SrcFrame, sizeof(QCSGFrameStruct));
		MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		SrcFrame.A1 = CommPara.TerminalLocationCode;
		SrcFrame.A2 = CommPara.TerminalAddr;
		SrcFrame.A3 = 0x0000;
		SrcFrame.AFN = 0x02;
		SrcFrame.SEQ = 0x70 | (UARTCtrl->ReTxSEQ & 0x0F);
		SrcFrame.FC = 0x00C9;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0x00;
		
		SrcFrame.Data[Index++] = 0x02;
		SrcFrame.Data[Index++] = 0x10;
		SrcFrame.Data[Index++] = 0x00;
		SrcFrame.Data[Index++] = 0xE0;

		SrcFrame.DataLen = Index;
		FrameLen = QCSG_BuildFrameByStruct(&SrcFrame, (u8*)Get_ADDR_UARTnTx(PORTn));
		UartTask->AckLen = FrameLen;
		UartTask->Flag |= UART_FLAG_TX_ACK;	
		pTerminalRam->ENETHeartBeat_S_Timer = bcd_hex(CommPara.HeartBeatTimeGap);
		pTerminalRam->ENETHeartBeat_S_Timer *= 60;
		if(pTerminalRam->ENETHeartBeat_S_Timer == 0) pTerminalRam->ENETHeartBeat_S_Timer = 15*60;
		UARTCtrl->ReTxSEQ++;
	}

}


void QCSG_Local_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	u8* pData;
	u32 FrameLen = 0;		//, PortBufLen = 0;
	UARTCtrl_TypeDef* UARTCtrl;	
	UartRxChCtrlStruct* UartRxChCtrl;
	
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	pData = (u8*)Get_ADDR_UARTnRx(PORTn);

	FrameLen = UARTCtrl->RxByte;
	if((u16)FrameLen != UARTCtrl->RxByte)
	{
		FrameLen = UARTCtrl->RxByte;
	}

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);

	if(UartRxChCtrl == NULL) return;
	if(UartRxChCtrl->Empty != BUFFER_EMPTY) return;

	//PortBufLen = GetUartPortRxBufLen(PORTn);
	//if(FrameLen > PortBufLen) FrameLen = PortBufLen;

	UartRxChCtrl->SrcPort = PORTn;
	UartRxChCtrl->Empty = BUFFER_FULL;
	UartRxChCtrl->FrameType = UARTCtrl->FrameType;
	UartRxChCtrl->DataType = DATA_TYPE_FRAME;
	UartRxChCtrl->Flag |= UART_FRAME_INDICATION;
	UartRxChCtrl->FrameLen = FrameLen;
	UartRxChCtrl->FrameBuf = pData;
	//UartRxChCtrl->FrameBuf = (u8*)GetUartPortRxBuf(PORTn, DATA_TYPE_FRAME);
	//MR((u32)UartRxChCtrl->FrameBuf, (u32)pData, FrameLen);	

	//UartTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
	//MC(0, Get_ADDR_UARTnRx(PORTn), UARTCtrl->RxByte);
	//UARTCtrl->RxByte = 0;
	//UARTCtrl->Task = UART_TASK_RX;
	//UARTCtrl->FrameType = FRAME_TYPE_NONE;
	//UARTCtrl->Lock = UART_UNLOCK;
}


void DLT_645_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	u8* pData;
	u32 FrameLen = 0;		// PortBufLen = 0;
	UARTCtrl_TypeDef* UARTCtrl;	
	UartRxChCtrlStruct* UartRxChCtrl;
	
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	pData = (u8*)Get_ADDR_UARTnRx(PORTn);

	FrameLen = UARTCtrl->RxByte;
	if((u16)FrameLen != UARTCtrl->RxByte)
	{
		FrameLen = UARTCtrl->RxByte;
	}
//  以下三行用于回显调试	
//	MR(Get_ADDR_UARTnTx(PORTn), Get_ADDR_UARTnRx(PORTn), UARTCtrl->RxByte);
//	UARTCtrl->TxByte = UARTCtrl->RxByte;
//	UartTask->Flag |= UART_FLAG_TX_ACK;

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);

	if(UartRxChCtrl == NULL) return;
	if(UartRxChCtrl->Empty != BUFFER_EMPTY) return;

	//PortBufLen = GetUartPortRxBufLen(PORTn);
	//if(FrameLen > PortBufLen) FrameLen = PortBufLen;

	UartRxChCtrl->SrcPort = PORTn;
	UartRxChCtrl->Empty = BUFFER_FULL;
	UartRxChCtrl->FrameType = UARTCtrl->FrameType;
	UartRxChCtrl->DataType = DATA_TYPE_FRAME;
	UartRxChCtrl->Flag |= UART_FRAME_INDICATION;
	UartRxChCtrl->FrameLen = FrameLen;
	UartRxChCtrl->FrameBuf = pData;
	//UartRxChCtrl->FrameBuf = (u8*)GetUartPortRxBuf(PORTn, DATA_TYPE_FRAME);
	//MR((u32)UartRxChCtrl->FrameBuf, (u32)pData, FrameLen);

	//UartTask->Flag |= UART_FLAG_RX_BUF_READ_DONE;
	//MC(0, Get_ADDR_UARTnRx(PORTn), UARTCtrl->RxByte);
	//UARTCtrl->RxByte = 0;
	//UARTCtrl->Task = UART_TASK_RX;
	//UARTCtrl->FrameType = FRAME_TYPE_NONE;
	//UARTCtrl->Lock = UART_UNLOCK;

}

void Mixed_645_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	u8* pData;
	u32 FrameLen = 0;		
	UARTCtrl_TypeDef* UARTCtrl;	
	UartRxChCtrlStruct* UartRxChCtrl;
	
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	pData = (u8*)Get_ADDR_UARTnRx(PORTn);

	FrameLen = UARTCtrl->RxByte;
	if((u16)FrameLen != UARTCtrl->RxByte)
	{
		FrameLen = UARTCtrl->RxByte;
	}

	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);

	if(UartRxChCtrl == NULL) return;
	if(UartRxChCtrl->Empty != BUFFER_EMPTY) return;

	UartRxChCtrl->SrcPort = PORTn;
	UartRxChCtrl->Empty = BUFFER_FULL;
	UartRxChCtrl->FrameType = UARTCtrl->FrameType;
	UartRxChCtrl->DataType = DATA_TYPE_FRAME;
	UartRxChCtrl->Flag |= UART_FRAME_INDICATION;
	UartRxChCtrl->FrameLen = FrameLen;
	UartRxChCtrl->FrameBuf = pData;
	UARTCtrl->FrameType = FRAME_TYPE_NONE;
	
	
}

void EDMI_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	
	u8* pData;
	u32 FrameLen = 0;		// PortBufLen = 0;
	UARTCtrl_TypeDef* UARTCtrl;	
	UartRxChCtrlStruct* UartRxChCtrl;
	
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	pData = (u8*)Get_ADDR_UARTnRx(PORTn);

	FrameLen = UARTCtrl->RxByte;
	if((u16)FrameLen != UARTCtrl->RxByte)
	{
		FrameLen = UARTCtrl->RxByte;
	}
	UartRxChCtrl = (UartRxChCtrlStruct*)GetUartPortRxChCtrl(PORTn);

	if(UartRxChCtrl == NULL) return;
	if(UartRxChCtrl->Empty != BUFFER_EMPTY) return;

	UartRxChCtrl->SrcPort = PORTn;
	UartRxChCtrl->Empty = BUFFER_FULL;
	UartRxChCtrl->FrameType = UARTCtrl->FrameType;
	UartRxChCtrl->DataType = DATA_TYPE_FRAME;
	UartRxChCtrl->Flag |= UART_FRAME_INDICATION;
	UartRxChCtrl->FrameLen = FrameLen;
	UartRxChCtrl->FrameBuf = pData;

}



void QCSG_UnKnown_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	u8* pData;
	u32 FrameLen = 0;
	UARTCtrl_TypeDef* UARTCtrl;	
	UARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(PORTn);
	pData = (u8*)Get_ADDR_UARTnRx(PORTn);

	FrameLen = UARTCtrl->RxByte;
	if((u16)FrameLen != UARTCtrl->RxByte)
	{
		FrameLen = UARTCtrl->RxByte;
	}
	if((u16)FrameLen > SIZE_2K) FrameLen = SIZE_2K;	

	MC(0, (u32)pData, UARTCtrl->RxByte);
	UARTCtrl->RxByte = 0;
	UARTCtrl->Task = UART_TASK_RX;
	UARTCtrl->FrameType = FRAME_TYPE_NONE;
	UARTCtrl->Lock = UART_UNLOCK;
}

/*
void UartTaskProcess(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	u16* p16timer;
	u32 Addr = 0;
	UARTCtrl_TypeDef* UARTCtrl;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);	
	if((UartTask->Flag & UART_FLAG_RX_DONE) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_RX_DONE;
		if(UartTask->State == UART_STATE_WAIT_ACK) 
		{
			UartTask->AckTiemOutSetByTask = 0;
			UartTask->WaitAckTimeOut = 0;
			UartTask->State = UART_STATE_IDLE;
		}
		switch(UARTCtrl->FrameType)
		{
			case FRAME_TYPE_QCSG_UP:				
				QCSG_Up_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_QCSG_LOCAL:
				QCSG_Local_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_DLT_645:
				DLT_645_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_UNKNOWN:
				UartTask->Flag &= ~UART_FLAG_RX_DONE;
				QCSG_UnKnown_Protocol_Req_Process(PORTn, UartTask);
				break;
			default:
				break;			
		}
	}

	if((UartTask->Flag & UART_FLAG_RX_BUF_READ_DONE) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_RX_BUF_READ_DONE;
		MC(0, Get_ADDR_UARTnRx(PORTn), UARTCtrl->RxByte);
		UARTCtrl->Round = 0;
		UARTCtrl->RxByte = 0;
		UARTCtrl->Task = UART_TASK_RX;
		UARTCtrl->FrameType = FRAME_TYPE_NONE;
		UARTCtrl->Lock = UART_UNLOCK;
	}

	if((UartTask->Flag & UART_FLAG_TX_DONE) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_TX_DONE;
		if(UartTask->NeedAck == 0)
		{
			UartTask->FrameType = FRAME_TYPE_NONE;	
			UartTask->SrcTaskID = TASK_ID_NONE;
			UartTask->State = UART_STATE_IDLE;
		}
		else
		{
			UartTask->State = UART_STATE_WAIT_ACK;
			if(UartTask->AckTiemOutSetByTask == 0)
			{
				UartTask->WaitAckTimeOut = UART_DEFAULT_ACK_TIME;
			}
			else
			{
				UartTask->WaitAckTimeOut = UartTask->AckTiemOutSetByTask;
			}		
		}
		UartTask->FrameLen = 0;
		UartTask->RetryCnt = 0;
	}

	if((UartTask->Flag & UART_FLAG_WAIT_ACK_TIME_OUT) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_WAIT_ACK_TIME_OUT;
		UartTask->FrameType = FRAME_TYPE_NONE;	
		UartTask->SrcTaskID = TASK_ID_NONE;
		UartTask->State = UART_STATE_IDLE;
		UartTask->AckTiemOutSetByTask = 0;
	}

	if((UartTask->Flag & UART_FLAG_TX_TIME_OUT) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_TX_TIME_OUT;
		if(UartTask->RetryCnt > 0)
		{
			UartTask->RetryCnt--;
			UARTCtrl->Task = UART_TASK_TX;
			p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);
			if(PORTn != GPRSPORT)
			{
				UARTCtrl->TxByte = UartTask->FrameLen;
				p16timer[0] = UART_TX_TIME_OUT;
				Start_UARTn_Tx(PORTn);
			}
			else
			{
				pTerminalRam->CopyTxByte = UartTask->FrameLen;
				p16timer[0] = GPRS_UART_TX_TIME_OUT;
			}
		}
		else
		{
			UartTask->State = UART_STATE_IDLE;
			UartTask->SrcTaskID = TASK_ID_NONE;
			UartTask->FrameLen = 0;
			UartTask->RetryCnt = 0;
		}
	}

	if((UartTask->Flag & UART_FLAG_TX_TASK_FRAME) != 0)
	{
		p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);
		if((PORTn != GPRSPORT) && (p16timer[0] != 0)) return;
	
		UartTask->Flag &= ~UART_FLAG_TX_TASK_FRAME;
		if((UartTask->FrameBuf == NULL) || (UartTask->FrameLen == 0)) return;
		if((PORTn != GPRSPORT) && (UartTask->BpsCtrl != 0) && (UartTask->BpsCtrl != UARTCtrl->BpsCtrl))
		{
			switch(UartTask->BpsCtrl)
			{
				case BPS_300:
				case BPS_600:
				case BPS_1200:
				case BPS_2400:
				case BPS_4800:
				case BPS_9600:
				case BPS_19200:
				case BPS_38400:
				case BPS_57600:
				case BPS_115200:
				case BPS_230400:
					UARTCtrl->BpsCtrl = UartTask->BpsCtrl;
					Init_UARTn(PORTn);
					break;
				default:
					UartTask->BpsCtrl = UARTCtrl->BpsCtrl;
					break;		
			}		
		}	
		UartTask->State = UART_STATE_SEND_TASK_FRAME;	
		Addr = Get_ADDR_UARTnTx(PORTn);
		MR(Addr, (u32)UartTask->FrameBuf, UartTask->FrameLen);	
		UARTCtrl->Task = UART_TASK_TX;
		if(PORTn != GPRSPORT)
		{
			UARTCtrl->TxByte = UartTask->FrameLen;
			UARTCtrl->TxCount = 0;
			p16timer[0] = UART_TX_TIME_OUT;
			if(UartTask->AckTiemOutSetByTask != 0)
			{
				p16timer[0] = UartTask->AckTiemOutSetByTask * 100;
			}
			Start_UARTn_Tx(PORTn);
		}
		else
		{
			pTerminalRam->CopyTxByte = UartTask->FrameLen;
			p16timer[0] = GPRS_UART_TX_TIME_OUT;
			if(UartTask->AckTiemOutSetByTask != 0)
			{
				p16timer[0] += UartTask->AckTiemOutSetByTask * 100;
			}
		}	
		return;
	}

	if(((UartTask->Flag & UART_FLAG_TX_ACK) != 0) && (UartTask->State == UART_STATE_IDLE))
	{// 响应此标志位任务前已设置好对应串口发送的数据与长度
		UartTask->Flag &= ~UART_FLAG_TX_ACK;
		UartTask->NeedAck = 0;
		UartTask->RetryCnt = 0;
		UartTask->State = UART_STATE_SEND_LOCAL_RESPONSE;
		UARTCtrl->Task = UART_TASK_TX;
		p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);
		if(PORTn != GPRSPORT)
		{
			UARTCtrl->TxByte = UartTask->AckLen;
			UARTCtrl->TxCount = 0;		
			p16timer[0] = UART_TX_TIME_OUT;
			Start_UARTn_Tx(PORTn);
		}
		else
		{
			pTerminalRam->CopyTxByte = UartTask->AckLen;
			p16timer[0] = GPRS_UART_TX_TIME_OUT;
		}
	}	
}
*/

void UartTaskProcess(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	u16* p16timer;
	u32 Addr = 0;
	UARTCtrl_TypeDef* UARTCtrl;
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	
	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);	
	if((UartTask->Flag & UART_FLAG_RX_DONE) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_RX_DONE;
		switch(UARTCtrl->FrameType)
		{
			case FRAME_TYPE_QCSG_UP:				
				QCSG_Up_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_QCSG_LOCAL:
				QCSG_Local_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_DLT_645:
				DLT_645_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_UNKNOWN:
				QCSG_UnKnown_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_MIXED_645:
				Mixed_645_Protocol_Req_Process(PORTn, UartTask);
				break;
			case FRAME_TYPE_EDMI:
				EDMI_Protocol_Req_Process(PORTn, UartTask);
				break;
			default:
				break;			
		}
	}

	if((UartTask->Flag & UART_FLAG_RX_BUF_READ_DONE) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_RX_BUF_READ_DONE;
		MC(0, Get_ADDR_UARTnRx(PORTn), UARTCtrl->RxByte);
		UARTCtrl->Round = 0;
		UARTCtrl->RxByte = 0;
		UARTCtrl->Task = UART_TASK_RX;
		UARTCtrl->FrameType = FRAME_TYPE_NONE;
		UARTCtrl->Lock = UART_UNLOCK;
	}

	if((UartTask->Flag & UART_FLAG_TX_DONE) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_TX_DONE;
		UartTask->FrameType = FRAME_TYPE_NONE;	
		UartTask->SrcTaskID = TASK_ID_NONE;
		UartTask->State = UART_STATE_IDLE;
		UartTask->FrameLen = 0;
		UartTask->AckLen = 0;
		UartTask->Flag = 0;
		UartTask->RetryCnt = 0;
	}

	if((UartTask->Flag & UART_FLAG_TX_TIME_OUT) != 0)
	{
		UartTask->Flag &= ~UART_FLAG_TX_TIME_OUT;
		UartTask->State = UART_STATE_IDLE;
		UartTask->SrcTaskID = TASK_ID_NONE;
		UartTask->FrameType = FRAME_TYPE_NONE;
		UartTask->FrameLen = 0;
		UartTask->AckLen = 0;
		UartTask->Flag = 0;
		UartTask->RetryCnt = 0;
	}

	if(((UartTask->Flag & UART_FLAG_TX_ACK) != 0) && (UartTask->State == UART_STATE_IDLE))
	{// 响应此标志位任务前已设置好对应串口发送的数据与长度
		UartTask->Flag &= ~UART_FLAG_TX_ACK;
		UartTask->NeedAck = 0;
		UartTask->RetryCnt = 0;
		UartTask->State = UART_STATE_SEND_LOCAL_RESPONSE;
		UARTCtrl->Task = UART_TASK_TX;
		p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);
		if(PORTn != GPRSPORT)
		{
			UARTCtrl->TxByte = UartTask->AckLen;
			UARTCtrl->TxCount = 0;		
			p16timer[0] = UART_TX_TIME_OUT;
			Start_UARTn_Tx(PORTn);
		}
		else
		{
#if ((Project/100)==5)
			SetPortTxLed(GPRSPORT);
#endif
			pTerminalRam->CopyTxByte = UartTask->AckLen;
			p16timer[0] = GPRS_UART_TX_TIME_OUT;
		}
		return;
	}	

	if((UartTask->Flag & UART_FLAG_TX_TASK_FRAME) != 0)
	{
		p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);
		if((PORTn != GPRSPORT) && (p16timer[0] != 0)) return;
	
		UartTask->Flag &= ~UART_FLAG_TX_TASK_FRAME;
		if((UartTask->FrameBuf == NULL) || (UartTask->FrameLen == 0)) return;
		if((PORTn != GPRSPORT) && (UartTask->BpsCtrl != 0) && (UartTask->BpsCtrl != UARTCtrl->BpsCtrl))
		{	
			UARTCtrl->BpsCtrl = UartTask->BpsCtrl;
			Init_UARTn(PORTn);
		}	
		UartTask->State = UART_STATE_SEND_TASK_FRAME;	
		Addr = Get_ADDR_UARTnTx(PORTn);
		MR(Addr, (u32)UartTask->FrameBuf, UartTask->FrameLen);	
		UARTCtrl->Task = UART_TASK_TX;
		if(PORTn != GPRSPORT)
		{
			UARTCtrl->TxByte = UartTask->FrameLen;
			UARTCtrl->TxCount = 0;
			p16timer[0] = UART_TX_TIME_OUT;
			if(UartTask->AckTiemOutSetByTask != 0)
			{
				p16timer[0] = UartTask->AckTiemOutSetByTask * 100;
			}
			Start_UARTn_Tx(PORTn);
		}
		else
		{
#if ((Project/100)==5)
			SetPortTxLed(GPRSPORT);
#endif
			pTerminalRam->CopyTxByte = UartTask->FrameLen;
			p16timer[0] = GPRS_UART_TX_TIME_OUT;
			if(UartTask->AckTiemOutSetByTask != 0)
			{
				p16timer[0] += UartTask->AckTiemOutSetByTask * 100;
			}
		}	
		return;
	}

	//if((PORTn == GPRSPORT) && (UartTask->State != UART_STATE_IDLE))
	if(UartTask->State != UART_STATE_IDLE)
	{
		p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);
		if(p16timer[0] == 0) 
		{
			UartTask->State = UART_STATE_IDLE;
		}
	}
}

void UartPortManage(u32 PORTn, UartTaskCtrlStruct* UartTask)
{
	u16* p16timer;
  	UARTCtrl_TypeDef* UARTCtrl;

  	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);

	switch(UARTCtrl->Task)
	{
		case UART_TASK_RX:
			UartReveiveFrame(PORTn);
			break;
		case UART_TASK_RX_DONE:
			if(UARTCtrl->Protocol == 0)
			{
				if(PORTn == IRPORT)
				{
					UARTCtrl->Round++;
					if((UARTCtrl->Round > 250) && (UARTCtrl->Lock != UART_LOCK))
					{
						UARTCtrl->Round = 0;
						UartTask->Flag |= UART_FLAG_RX_DONE;
						UartTask->FrameLen = UARTCtrl->RxByte;
						UartTask->FrameType = UARTCtrl->FrameType;
						UartTask->FrameBuf = (u8*)Get_ADDR_UARTnRx(PORTn);
						UARTCtrl->Lock = UART_LOCK;
						p16timer[0] = 300;
					}
				}
				else if(PORTn == ENETPORT)
				{
					UARTCtrl->Round++;
					if((UARTCtrl->Round > 100) && (UARTCtrl->Lock != UART_LOCK))
					{
						UARTCtrl->Round = 0;
						UartTask->Flag |= UART_FLAG_RX_DONE;
						UartTask->FrameLen = UARTCtrl->RxByte;
						UartTask->FrameType = UARTCtrl->FrameType;
						UartTask->FrameBuf = (u8*)Get_ADDR_UARTnRx(PORTn);
						UARTCtrl->Lock = UART_LOCK;
						p16timer[0] = 300;
					}
				}
				else
				{
					if(UARTCtrl->Lock != UART_LOCK)
					{
						UartTask->Flag |= UART_FLAG_RX_DONE;
						UartTask->FrameLen = UARTCtrl->RxByte;
						UartTask->FrameType = UARTCtrl->FrameType;
						UartTask->FrameBuf = (u8*)Get_ADDR_UARTnRx(PORTn);
						UARTCtrl->Lock = UART_LOCK;
						p16timer[0] = 300;
					}
				}	

				if((UARTCtrl->Lock == UART_LOCK) && (p16timer[0] == 0))
				{
					MC(0, Get_ADDR_UARTnRx(PORTn), UARTCtrl->RxByte);
					UARTCtrl->Round = 0;
					UARTCtrl->RxByte = 0;
					UARTCtrl->Task = UART_TASK_RX;
					UARTCtrl->FrameType = FRAME_TYPE_NONE;
					UARTCtrl->Lock = UART_UNLOCK;
				}
			}			
			break;
		case UART_TASK_RX_TIMEOUT:
			if(UARTCtrl->Protocol == 0)
			{
				MC(0, Get_ADDR_UARTnRx(PORTn), UARTCtrl->RxByte);
				UARTCtrl->RxByte = 0;
				UARTCtrl->Task = UART_TASK_RX;
				UARTCtrl->FrameType = FRAME_TYPE_NONE;
				UARTCtrl->Lock = UART_UNLOCK;
			}						
			break;
		case UART_TASK_TX:
			if(UARTCtrl->Protocol == 0)
			{
				SetPortTxLed(PORTn);
				if(p16timer[0] == 0) 
				{
					UARTCtrl->TxByte = 0;
					UARTCtrl->Task = UART_TASK_RX;
					UARTCtrl->FrameType = FRAME_TYPE_NONE;
					UARTCtrl->Lock = UART_UNLOCK;
					UARTCtrl->BpsCtrl = GetUartPortBpsCtrl(PORTn);
					UartTask->Flag |= UART_FLAG_TX_TIME_OUT;
					Init_UARTn(PORTn);
				}				
				if(Wait_UARTn_Tx(PORTn) == 0)
				{
					if(PORTn == PLC_PORT)
					{
						#if LOG_1376_2==1//0=没,1=有
							LOG(13762,1,PLC_PORT,(u8*)Get_ADDR_UARTnTx(PORTn), UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
						#endif
					}
					MC(0, Get_ADDR_UARTnTx(PORTn), UARTCtrl->TxByte);
					p16timer[0] = 0;
					UARTCtrl->TxByte = 0;
					UARTCtrl->Task = UART_TASK_RX;
					UARTCtrl->FrameType = FRAME_TYPE_NONE;
					UARTCtrl->Lock = UART_UNLOCK;
					UartTask->Flag |= UART_FLAG_TX_DONE;
				}
				
			}			
			break;
		case UART_TASK_TX_DONE:			//以太网发送数据完成后，会将 Task 设置为 7，表示发送完成
			if(UARTCtrl->Protocol == 0)
			{
				MC(0, Get_ADDR_UARTnTx(PORTn), UARTCtrl->TxByte);
				p16timer[0] = 0;
				UARTCtrl->RxByte = 0;
				UARTCtrl->TxByte = 0;
				UARTCtrl->Task = UART_TASK_RX;
				UARTCtrl->FrameType = FRAME_TYPE_NONE;
				UARTCtrl->Lock = UART_UNLOCK;
				UartTask->Flag |= UART_FLAG_TX_DONE;
			}
			
			break;
		default:
			if(UARTCtrl->Protocol == 0)
			{
				UARTCtrl->Lock = UART_UNLOCK;
				UARTCtrl->TxByte = 0;
				UARTCtrl->RxByte = 0;
				UARTCtrl->Task = UART_TASK_RX;
				UARTCtrl->FrameType = FRAME_TYPE_NONE;
			}			
			break;
	}
}


u32 UartReveiveFrame(u32 PORTn)
{	
	u8* p8fifo;
	u16* p16fifo;
	u8* p8rx;
	u16* p16timer;
  	UARTCtrl_TypeDef* UARTCtrl;
	u8 Temp645Addr[6], TempMeterAddr[6];
	u32 LEN_UARTnRx, LEN_UARTnIntRx, pWrite, pRead, RxByte;
	u32 i = 0, Sum = 0;
	
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;

//	u8 p8rx_EDMI[100];	//接收到的帧
//	u8 p8rx_EDMI_Real[100];//除去0X10并减去0x40 处理后的原始帧
//	u32 RxByte_EDMI,RxByte_EDMI_Real;
	u16 crc16;
	
  	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_UARTnIntRx = Get_LEN_UARTnIntRx(PORTn);
	p16timer = (u16*)Get_ADDR_UARTnTimer(PORTn);	
	p16fifo = (u16*)Get_ADDR_UARTn(PORTn);
	DMA_UARTn_RX_Pointer(PORTn);					// 计算DMA_UARTn接收指针值(p0)
	pWrite = p16fifo[0];							// 中断接收指针
	pRead  = p16fifo[1];							// 当前已读指针
	if((pWrite >= (LEN_UARTnIntRx - 10)) || ( pRead >= (LEN_UARTnIntRx - 10)))
	{
		pWrite = p16fifo[0];							// 中断接收指针
		pRead  = p16fifo[1];
		if((pWrite >= (LEN_UARTnIntRx - 10)) || ( pRead >= (LEN_UARTnIntRx - 10)))
		{
			p16fifo[0] = 0;		// 中断接收指针
			p16fifo[1] = 0;		// 当前已读指针
			p16fifo[2] = 0;		// 规约检查指针
			p16fifo[3] = 0;
			p16fifo[4] = 0;
			return 0;			// 返回: 0没完成, 1完成, 2超时
		}		
	}
	
	if((UARTCtrl->Protocol == 1) && (pWrite == pRead))
	{
		UARTCtrl->Protocol = 0;
	}
	if(pWrite != pRead)
	{//有数据接收,重置接收超时定时
		if(PORTn==RS485_1PORT)
		{
			pTerminalRam->RS485_1_RxLED_MS_Timer = 20/10;		// 11 RS485抄表接收LED指示MS定时器,每10MS减1定时器
			UARTCtrl->FrameType = FRAME_TYPE_MIXED_645;
		}
		if(PORTn==RS485_2PORT)
		{
			#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)	||((Project/100)==9)	
				pTerminalRam->RS485_2_RxLED_MS_Timer = 20/10;		// 13 RS485抄表接收LED指示MS定时器,每10MS减1定时器
			#endif
			#if (Project/100)==5    //ybzyl20180601对于II型集中器，2路485的发送灯的毫秒定时器也使用1路485发送灯的毫秒定时器
				pTerminalRam->RS485_1_RxLED_MS_Timer = 20/10;		// 13 RS485抄表接收LED指示MS定时器,每10MS减1定时器
			#endif
			UARTCtrl->FrameType = FRAME_TYPE_MIXED_645;
		}
		if(PORTn==RS485_3PORT)
		{
			pTerminalRam->RS485_3_RxLED_MS_Timer = 20/10;		// 15 RS485抄表接收LED指示MS定时器,每10MS减1定时器
			UARTCtrl->FrameType = FRAME_TYPE_MIXED_645;
		}

		if(PORTn==ENETPORT)
		{
			pTerminalRam->ENETLED_MS_Timer = 20/10;		
		}
		if(PORTn==GPRSPORT)
		{
			pTerminalRam->GPRS_RxLED_MS_Timer = 100/10;		
		}
		p8fifo = (u8*)(Get_ADDR_UARTn(PORTn)+10);
	  	p8rx = (u8*)Get_ADDR_UARTnRx(PORTn);
		LEN_UARTnRx = Get_LEN_UARTnRx(PORTn);
		if(PORTn == ENETPORT)
		{
			p16timer[0] = RCV_ENET_FRAME_TIME_OUT;				// 以太网模块超时定时 10S
		}
		else if(PORTn == GPRSPORT)
		{
			p16timer[0] = RCV_GPRS_FRAME_TIME_OUT;				// GPRS无线通信模块超时定时 10S
		}
		else if(PORTn == SMSPORT)
		{
			p16timer[0] = RCV_SMS_FRAME_TIME_OUT;				// 短信通道超时定时 60S
		}
		else
		{//如果是波特率1200的红相表 1s的超时时间可能不够。需要2s或3s
			p16timer[0] = RCV_UART_FRAME_TIME_OUT;				// 其他通道 1000ms
			
		}
	}

	RxByte = UARTCtrl->RxByte;
	while(pWrite != pRead)
	{
		if(RxByte >= LEN_UARTnRx) RxByte = 0;

		i = p8fifo[pRead++];
		if(pRead >= (LEN_UARTnIntRx-10)) pRead = 0;
		p8rx[RxByte++] = i;

		if((p8rx[0] != DLT_645_HEAD) && (p8rx[0] != EDMI_HEAD) )
		{
			if(RxByte == 1) 
			{
				UARTCtrl->Jump_Flag = 0;//开始清零标识
				if(i == DLT_645_HEAD)//帧头判断
				{
					;//DLT645_EDMI_Flag = 1;//645
				}
				else if(i == EDMI_HEAD)//帧头判断
				{
					;//DLT645_EDMI_Flag = 2;//EDMI
				}
				else
				{
					RxByte = 0;
					//UARTCtrl->RxByte = 0;
					UARTCtrl->Jump_Flag = 0;//开始清零标识
					p16fifo[1] = pRead;		//当前已读指针
					continue;
				}
			}
			else RxByte = 0;
		}
		else if(p8rx[0] == DLT_645_HEAD)
		{
			if(RxByte < MIN_645_FRAME_LEN) continue;

			if((p8rx[7] == DLT_645_HEAD) && (RxByte == (12 + p8rx[9])) && (i == DLT_645_TAIL))
			{// DLT645帧格式检查
				Sum = SumCaculation(p8rx, 10 + p8rx[9]);
				if((u8)Sum != p8rx[10 + p8rx[9]]) 
				{
					p16fifo[1] = pRead;		//当前已读指针
					p16fifo[2] = pRead;		//检查指针==读指针,免检省时
					p16fifo[3] = pRead;		//检查指针==读指针,免检省时
					p16fifo[4] = pRead;		//检查指针==读指针,免检省时
					UARTCtrl->RxByte = 0;
					//DLT645_EDMI_Flag = 0;//无类型
					return 0;	
				}
				if(UARTCtrl->Function == UART_FUNC_MAINTAIN)		// UARTCtrl->Function == 0,表示维护口
				{
					#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)		
						MR((u32)TempMeterAddr, (u32)ADDR_04000401, 6);
					#endif
					Temp645Addr[0] = p8rx[1]; Temp645Addr[1] = p8rx[2];
					Temp645Addr[2] = p8rx[3]; Temp645Addr[3] = p8rx[4];
					Temp645Addr[4] = p8rx[5]; Temp645Addr[5] = p8rx[6];
					
					if((TempMeterAddr[0] == Temp645Addr[0]) && (TempMeterAddr[1] == Temp645Addr[1]) && 
			   		   (TempMeterAddr[2] == Temp645Addr[2]) && (TempMeterAddr[3] == Temp645Addr[3]) && 
			   		   (TempMeterAddr[4] == Temp645Addr[4]) && (TempMeterAddr[5] == Temp645Addr[5]))
					{//  与本地交采地址相等
						UARTCtrl->Protocol = 1;
						return 0;
					}

					if((Temp645Addr[0] == 0xAA) && (Temp645Addr[1] == 0xAA) && 
					   (Temp645Addr[2] == 0xAA) && (Temp645Addr[3] == 0xAA) && 
					   (Temp645Addr[4] == 0xAA) && (Temp645Addr[5] == 0xAA))
					{
						UARTCtrl->Protocol = 1;
						return 0;
					}

					if((Temp645Addr[0] == 0x99) && (Temp645Addr[1] == 0x99) && 
					   (Temp645Addr[2] == 0x99) && (Temp645Addr[3] == 0x99) && 
					   (Temp645Addr[4] == 0x99) && (Temp645Addr[5] == 0x99))
					{
						UARTCtrl->Protocol = 1;
						return 0;
					}
				}
				
				p16fifo[1] = pRead;		//当前已读指针
				p16fifo[2] = pRead;		//检查指针==读指针,免检省时
				p16fifo[3] = pRead;		//检查指针==读指针,免检省时
				p16fifo[4] = pRead;		//检查指针==读指针,免检省时
				UARTCtrl->RxByte = RxByte;
				UARTCtrl->Protocol = 0;
				UARTCtrl->FrameType = FRAME_TYPE_DLT_645;
				UARTCtrl->Task = UART_TASK_RX_DONE;
				return 1;				//返回0没完成,1完成,2超时
		
			}
			if((p8rx[1] == p8rx[3]) && (p8rx[2] == p8rx[4]) && (p8rx[5] == QCSG_FRAME_HEAD) && 
			   (RxByte == (8 + p8rx[1] + (p8rx[2] << 8))) && (i == QCSG_FRAME_TAIL))
			{// 南网上行帧格式检查
				Sum = SumCaculation(&p8rx[6], p8rx[1] + (p8rx[2] << 8));
				if((u8)Sum == p8rx[6 + p8rx[1] + (p8rx[2] << 8)])
				{
					p16fifo[1] = pRead;		//当前已读指针
					p16fifo[2] = pRead;		//检查指针==读指针,免检省时
					p16fifo[3] = pRead;		//检查指针==读指针,免检省时
					p16fifo[4] = pRead;		//检查指针==读指针,免检省时
					
					UARTCtrl->RxByte = RxByte;
					UARTCtrl->Protocol = 0;
					UARTCtrl->FrameType = FRAME_TYPE_QCSG_UP;
					UARTCtrl->Task = UART_TASK_RX_DONE;
					return 1;				//返回0没完成,1完成,2超时
				}
			}
			if((RxByte == (p8rx[1] + (p8rx[2] << 8))) && (i == DLT_645_TAIL))
			{// 南网下行帧格式检查
				Sum = SumCaculation(&p8rx[3], (p8rx[1] + (p8rx[2] << 8)) - 5);
				if((u8)Sum == p8rx[p8rx[1] + (p8rx[2] << 8) - 2])
				{
					p16fifo[1] = pRead;		//当前已读指针
					p16fifo[2] = pRead;		//检查指针==读指针,免检省时
					p16fifo[3] = pRead;		//检查指针==读指针,免检省时
					p16fifo[4] = pRead;		//检查指针==读指针,免检省时
					UARTCtrl->RxByte = RxByte;
					UARTCtrl->Protocol = 0;
					UARTCtrl->FrameType = FRAME_TYPE_QCSG_LOCAL;
					UARTCtrl->Task = UART_TASK_RX_DONE;
					return 1;				//返回0没完成,1完成,2超时
				}
			}
		}
		else if(p8rx[0] == EDMI_HEAD)
		{
			if(UARTCtrl->Jump_Flag == 1)//上一个字符是0x10 跳过判断
			{
				UARTCtrl->Jump_Flag = 0;
				if((i == 0x42)||(i == 0x43)||(i == 0x50)||(i == 0x51)||(i == 0x53))
				{
					p8rx[RxByte-2] = i - 0x40;
					RxByte--;
				}
				else 
				{
					UARTCtrl->Jump_Flag = 0;
					p16fifo[1] = pRead;		//当前已读指针
					p16fifo[2] = pRead;		//检查指针==读指针,免检省时
					p16fifo[3] = pRead;		//检查指针==读指针,免检省时
					p16fifo[4] = pRead;		//检查指针==读指针,免检省时
					UARTCtrl->RxByte = 0;
					return 0;
				}
			}
			else 
			{
				if(i == 0x10)	//判断是0x10则不存储该字节
				{
					UARTCtrl->Jump_Flag = 1;
					continue;
				}
				else 
				{
					if(RxByte < MIN_EDMI_FRAME_LEN) 
						continue;
					else 
					{	
						if(p8rx[1] != EDMI_FRAME_FLAG)
						{
							UARTCtrl->Jump_Flag = 0;
							p16fifo[1] = pRead;		//当前已读指针
							p16fifo[2] = pRead;		//检查指针==读指针,免检省时
							p16fifo[3] = pRead;		//检查指针==读指针,免检省时
							p16fifo[4] = pRead;		//检查指针==读指针,免检省时
							UARTCtrl->RxByte = 0;
							return 0;
						}
						if(i == EDMI_TAIL)//未经过0x10遇见0x03就是帧结尾
						{
							crc16 = cal_CRC16(p8rx,RxByte-3);
							if((p8rx[0] == EDMI_HEAD) && (p8rx[1] == EDMI_FRAME_FLAG) && (crc16 == ((p8rx[RxByte-3]<<8) + p8rx[RxByte-2])))
							{
								if(UARTCtrl->Function != UART_FUNC_MAINTAIN)		// UARTCtrl->Function == 0,表示维护口
								{
									p16fifo[1] = pRead;		//当前已读指针
									p16fifo[2] = pRead;		//检查指针==读指针,免检省时
									p16fifo[3] = pRead;		//检查指针==读指针,免检省时
									p16fifo[4] = pRead;		//检查指针==读指针,免检省时
									UARTCtrl->RxByte = RxByte;
									UARTCtrl->Protocol = 0;
									UARTCtrl->FrameType = FRAME_TYPE_EDMI;
									UARTCtrl->Task = UART_TASK_RX_DONE;
									return 1;				//返回0没完成,1完成,2超时
								}
								else
								{
									UARTCtrl->Protocol = 1;
									return 0;
								}
							}
							else //错误帧 直接返回
							{
								UARTCtrl->Jump_Flag = 0;
								p16fifo[1] = pRead;		//当前已读指针
								p16fifo[2] = pRead;		//检查指针==读指针,免检省时
								p16fifo[3] = pRead;		//检查指针==读指针,免检省时
								p16fifo[4] = pRead;		//检查指针==读指针,免检省时
								UARTCtrl->RxByte = 0;
								return 0;
							}
						}
					}
				}
			}
		}
		
	}
	p16fifo[1] = pRead;		//当前已读指针
	UARTCtrl->RxByte = RxByte;
	#if((Project/100 == 2) ||(Project/100 == 5))
		if((UARTCtrl->Protocol == 0) && (UARTCtrl->FrameType == FRAME_TYPE_MIXED_645) && (p16timer[0] == 0))
		{	//搜表收到的非法帧
			p16fifo[1] = pRead;
			UARTCtrl->RxByte = RxByte;
			UARTCtrl->Protocol = 0;
			UARTCtrl->Task = UART_TASK_RX_DONE;
			return 2;
		}
	#endif
	if((UARTCtrl->Protocol == 0) && (UARTCtrl->RxByte > 0) && (p16timer[0] == 0))
	{							//超时
		p16fifo[1] = pRead;
		UARTCtrl->RxByte = RxByte;
		UARTCtrl->Protocol = 0;
		UARTCtrl->Task = UART_TASK_RX_TIMEOUT;
		UARTCtrl->FrameType = FRAME_TYPE_UNKNOWN;
		return 2;				//返回0没完成,1完成,2超时
	}
	return 0;					//返回0没完成,1完成,2超时
}


void SetPortTxLed(u32 PORTn)
{
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	switch(PORTn)
	{
		case RS232PORT:
			break;		
		case GPRSPORT:
			pTerminalRam->GPRS_TxLED_MS_Timer = 20;
			break;		
		case RS485_1PORT:
			pTerminalRam->RS485_1_TxLED_MS_Timer = 3;
			break;
		case RS485_2PORT:
			#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==4)||((Project/100)==9)							
					pTerminalRam->RS485_2_TxLED_MS_Timer = 3;			
			#endif
			#if (Project/100)==5   //ybzyl20180601对于II型集中器，2路485的发送灯的毫秒定时器也使用1路485发送灯的毫秒定时器
					pTerminalRam->RS485_1_TxLED_MS_Timer = 3;
			#endif
			break;
		case RS485_3PORT:
			break;		
		case PLC_PORT:
			break;
		case IRPORT:
			break;
		case ENETPORT:
			pTerminalRam->ENETLED_MS_Timer = 3;
			break;
		default:
			break;
	}
}


u16 GetUartPortBpsCtrl(u32 PORTn)
{
	u32 i = 0;	
	RS485PortParaStruct PortPara;
	
	if(PORTn == RS232PORT) return RS232DefaultBPS;

	if(PORTn == IRPORT) return IRDefaultBPS;

	if(PORTn == PLC_PORT) return PLCDefaultBPS;

	if(PORTn == GPRSPORT) return GPRSDefaultBPS;

	if(PORTn == RS485_1PORT)
	{
		i = 0;
	}
	else if(PORTn == RS485_2PORT)
	{
		i = 1;
	}
	else if(PORTn == RS485_3PORT)
	{
		i = 2;
	}
	else 
	{
		return RS485DefaultBPS;
	}

	MC(0, (u32)&PortPara, sizeof(RS485PortParaStruct));
	MR((u32)&PortPara, ADDR_RS485PortParaTbl + (i * sizeof(RS485PortParaStruct)), sizeof(RS485PortParaStruct));
	if((PortPara.Function == UART_FUNC_ATTACH) || (PortPara.Function == UART_FUNC_SLAVE) || 
	   (PortPara.Function == UART_FUNC_UPLINK))
	{
		return ConvertToBpsCtrl(PortPara.BaudRate, PortPara.Parity, PortPara.DataBit, PortPara.StopBit);
	}
	
	return RS485DefaultBPS;
}

/****************************************************************************
功     能: 根据输入的串口控制参数得到串口初始化控制字
入口参 数: BaudRateCode: 根据波特率数组，按照下标获取，注意输入参数是南网标准baudrate/300
		   需要先根据南网的取值获得实际波特率，然后转换成王总格式对应的控制字。
		   0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,
		   10=115200,11=230400
		   ParityCode: 0：无校验；1：偶校验；2：奇校验 
		   DataBitCode: 数据位，5,6,7,8
		   StopBitCode: 停止位，0：1位；1：1.5位； 2：2位
返     回: 转换出的王总串口初始化控制字
		   D8-D5位表示波特率,O～11依次表示0=300,1=600,2=1200,3=2400,4=4800,
		   5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400
		   D4位表示停止位位数0/1分别表示1/2个停止位
		   D3位表示有无校验位0/1分别表示无/有
		   D2位表示偶／奇校验位0/1分别表示偶／奇校验
		   D1-D0 数据位数0～3分别表示5-8位
编     写: zhaoyun
编写时 间; 2017-7-10
****************************************************************************/
/*u16 ConvertToBpsCtrl(u8 BaudRateCode, u8 ParityCode, u8 DataBitCode, u8 StopBitCode)
{
	u16 i = 0;
	u16 BspCtrl = 0;
	u32 BaudRate = 0; 
	BaudRate = BaudRateCode;
	BaudRate *= 300;
	for(i = 0; i < MAX_BAUD_NUM; i++)
	{
		if(BaudRate == UartBaudRate[i]) break;
	}
	if(i >= MAX_BAUD_NUM)
	{// 未找到存在的波特率，则默认为2400
		i = 3;			
	}
	BspCtrl |= (i << 5);

	if(StopBitCode == 2)
	{
		BspCtrl |= 0x20;
	}

	if(ParityCode == 0)
	{// 无校验
	
	}
	else if(ParityCode == 2)
	{// 奇校验 
		BspCtrl |= 0xC0;
	}
	else
	{// 偶校验
		BspCtrl |= 0x80;
	}

	if(DataBitCode == 5)
	{
		BspCtrl |= 0x00;
	}
	else if(DataBitCode == 6)
	{
		BspCtrl |= 0x01;
	}
	else if(DataBitCode == 7)
	{
		BspCtrl |= 0x02;
	}
	else
	{
		BspCtrl |= 0x03;
	}
	return BspCtrl;
}
*/













