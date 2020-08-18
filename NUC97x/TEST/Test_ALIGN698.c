
#if MainProtocol==376
#include "../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../NUC97x/TEST/Test_ALIGN.h"



#if MainProtocol==698

const u32 UART_ALIGN32_List[]=
{
	(u32)&UART0Ctrl->RxTxByte,
	(u32)&UART0Ctrl->FTPByte,
	(u32)&UART0Ctrl->FTPFileLen,
	(u32)&UART0Ctrl->TCPSocket,
	(u32)&UART0Ctrl->UDPSocket,
	(u32)&UART0Ctrl->pRxBuff,
	(u32)&UART0Ctrl->NEXTOADNum,
	(u32)&UART0Ctrl->NEXTOADSubNum,
	(u32)&UART0Ctrl->pTxData,
	(u32)&UART0Ctrl->LEN_TxData,
	(u32)&UART0Ctrl->LEN_ReadData,
	(u32)&UART0Ctrl->TaskID,
	(u32)&UART0Ctrl->RM_DI,
	ADDR_UART1,
	ADDR_UART2,
	ADDR_UART3,
	ADDR_UART4,
	ADDR_UART5,
	ADDR_UART6,
	ADDR_UART7,
	ADDR_UART8,
	ADDR_UART9,
	ADDR_UART10,
	ADDR_UART11,
	ADDR_UART12,
	ADDR_UART13,
	ADDR_UART14,
	ADDR_UART15,
	ADDR_UART16,
};
	
const u32 UART_ALIGN16_List[]=
{
	(u32)&UART0Ctrl->RxByte,
	(u32)&UART0Ctrl->TxByte,
	(u32)&UART0Ctrl->TxCount,
	(u32)&UART0Ctrl->BpsCtrl,
	(u32)&UART0Ctrl->PORT,
	(u32)&UART0Ctrl->TxAPDUSEQ,
	(u32)&UART0Ctrl->TxAPDUByte,
	(u32)&UART0Ctrl->AutoSearchTime,
	(u32)&UART0Ctrl->TransferDelay,
	(u32)&UART0Ctrl->SMSRxSEQ,
	(u32)&UART0Ctrl->SMSTxSEQ,
	(u32)&UART0Ctrl->RM_DI_N,
	(u32)&UART0Ctrl->RM_DI_TI_Count,
	(u32)&UART0Ctrl->RM_SubsequentFrames,
	(u32)&UART0Ctrl->TxToRxTimerOver,
	(u32)&UART0Ctrl->ProxyTxRxByte,
	(u32)&UART0Ctrl->ProxyOverTime,
	(u32)&UART0Ctrl->REPORT_OVER_SECOND,
	(u32)&UART0Ctrl->REPORT_filename,
	(u32)&UART0Ctrl->REPORT_NUM_Data,
	(u32)&UART0Ctrl->AutoReportCopyNUM,
	(u32)&UART0Ctrl->SourMeterCount_id,
	(u32)&UART0Ctrl->NUM_SetMeter,
	(u32)&UART0Ctrl->MeterCount,
	(u32)&UART0Ctrl->NUMrmTOTAL,
	(u32)&UART0Ctrl->NUMrmOK,
	(u32)&UART0Ctrl->NUMrmFAIL,
	(u32)&UART0Ctrl->NUMrmBAD,
	(u32)&UART0Ctrl->RMAddr,	
	(u32)&UART0Ctrl->rmOKFAILlist,
	(u32)&UART0Ctrl->SomeTaskid,
	(u32)&UART0Ctrl->LastSomeTaskid,
};

const u32 Router_ALIGN32_List[]=
{
	(u32)&Terminal_Router->StopFlags,
	ADDR_213x_REAL,
	ADDR_Event_Sour,
};
const u32 Router_ALIGN16_List[]=
{
	(u32)&Terminal_Router->RMListCount,
	(u32)&Terminal_Router->ModeNoteNum,
	(u32)&Terminal_Router->NoteNum,
	(u32)&Terminal_Router->TempCount,
	(u32)&Terminal_Router->UpDateCount,
	(u32)&Terminal_Router->ReadNoteNum,
	(u32)&Terminal_Router->RS485Time,
	(u32)&Terminal_Router->KeepTime,
	(u32)&Terminal_Router->MaxBroadcastOverTime,
	(u32)&Terminal_Router->MaxTxByte,
	(u32)&Terminal_Router->MaxFileByte,
	(u32)&Terminal_Router->MaxUpDateTime,
	(u32)&Terminal_Router->MaxNoteNum,
	ADDR_PowerEvent,
};

const u32 RMLIST_ALIGN32_List[]=
{
	(u32)&RMLIST->List->RM_DI,
};
const u32 RMLIST_ALIGN16_List[]=
{
	(u32)&RMLIST->List->RMAddr,
	(u32)&RMLIST->List->RM_DI_TI_Count,
	(u32)&RMLIST->List->RM_SubsequentFrames,
	#ifdef (USER/100)==17
	(u32)&RMLIST->List->collectoraddr,
	(u32)&RMLIST->List->res24,
	#endif
};

const u32 Terminal_Ram_ALIGN32_List[]=
{
	(u32)&Terminal_Ram->res13,//菜单中已使用了(黄从海)
	
};
const u32 Terminal_Ram_ALIGN16_List[]=
{
	(u32)&Terminal_Ram->I2SUBTask4_1,
	(u32)&Terminal_Ram->CopyTxByte,
};

const u32 JSON_ALIGN32_List[]=
{
	(u32)&JSON->g_Filesize,
	
};
	
const u32 JSON_ALIGN16_List[]=
{
	(u32)&JSON->CurFrzMetCount,
	(u32)&JSON->CurRealMetCount,
	(u32)&JSON->g_Block_Number,
	(u32)&JSON->g_Need_report_Block,
	(u32)&JSON->res2,
};

const u32 Comm_Ram_ALIGN32_List[]=
{
	(u32)&Comm_Ram->IRQ0,
	(u32)&Comm_Ram->ErrorIC,
	
};
const u32 Comm_Ram_ALIGN16_List[]=
{
	(u32)&Comm_Ram->msFILESearchCount,
	(u32)&Comm_Ram->TESTKEY,
	(u32)&Comm_Ram->URTC,
	(u32)&Comm_Ram->UART0Timer,
	(u32)&Comm_Ram->UART6DATA,
	(u32)&Comm_Ram->LOOPCOUNT,
	
};

const u32 Meter_Ram_ALIGN32_List[]=
{
	(u32)&Meter_Ram->SSP1Data0,
	(u32)&Meter_Ram->kWParAdd,
	(u32)&Meter_Ram->ActiveParAdd,
	(u32)&Meter_Ram->ReactiveParAdd,
	(u32)&Meter_Ram->CS,
};
const u32 Meter_Ram_ALIGN16_List[]=
{
	(u32)&Meter_Ram->SSP1Addr0,
	(u32)&Meter_Ram->DisplayScanTimer,
	(u32)&Meter_Ram->RunFlag1,
	(u32)&Meter_Ram->ActiveParCount,
	(u32)&Meter_Ram->ReactiveParCount,
};







u32 Check_ALIGN32(u32* pList,u32 NUM)//检查4字节对齐:返回:0=正确,1=错误
{
	u32 i;
	u32 x;
	
	for(i=0;i<NUM;i++)
	{
		x=pList[i];
		if(x&3)
		{
			return 1;
		}
	}
	return 0;
}
u32 Check_ALIGN16(u32* pList,u32 NUM)//检查2字节对齐:返回:0=正确,1=错误
{
	u32 i;
	u32 x;
	
	for(i=0;i<NUM;i++)
	{
		x=pList[i];
		if(x&1)
		{
			return 1;
		}
	}
	return 0;
}

u32 Test_ALIGN(void)//数据对齐测试;返回:0=正确,1=错误
{
	u32 i;
	
	
//ERAM698.h
	//ms文件管理
	i=sizeof(msFILE_TypeDef);
	i%=4;
	if(i) return 1;
	//ECC BUFF
	i=ADDR_ECC_BUFF;
	if(i&3) return 1;
	//UART
	i=Check_ALIGN32((u32*)&UART_ALIGN32_List,sizeof(UART_ALIGN32_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	i=Check_ALIGN16((u32*)&UART_ALIGN16_List,sizeof(UART_ALIGN16_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	//Router
	i=Check_ALIGN32((u32*)&Router_ALIGN32_List,sizeof(Router_ALIGN32_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	i=Check_ALIGN16((u32*)&Router_ALIGN16_List,sizeof(Router_ALIGN16_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	//RMLIST
	i=Check_ALIGN32((u32*)&RMLIST_ALIGN32_List,sizeof(RMLIST_ALIGN32_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	i=Check_ALIGN16((u32*)&RMLIST_ALIGN16_List,sizeof(RMLIST_ALIGN16_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	//Terminal_Ram
	i=Check_ALIGN32((u32*)&Terminal_Ram_ALIGN32_List,sizeof(Terminal_Ram_ALIGN32_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	i=Check_ALIGN16((u32*)&Terminal_Ram_ALIGN16_List,sizeof(Terminal_Ram_ALIGN16_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	//JSON
	i=Check_ALIGN32((u32*)&JSON_ALIGN32_List,sizeof(JSON_ALIGN32_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	i=Check_ALIGN16((u32*)&JSON_ALIGN16_List,sizeof(JSON_ALIGN16_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
//IRAM_Terminal698.h

//IRAM_TerminalDisplay698.h

//IRAM698.h
	//Comm_Ram
	i=Check_ALIGN32((u32*)&Comm_Ram_ALIGN32_List,sizeof(Comm_Ram_ALIGN32_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	i=Check_ALIGN16((u32*)&Comm_Ram_ALIGN16_List,sizeof(Comm_Ram_ALIGN16_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
//IRAM_Meter698.h
	//Meter_Ram
	i=Check_ALIGN32((u32*)&Meter_Ram_ALIGN32_List,sizeof(Meter_Ram_ALIGN32_List)/4);//返回:0=正确,1=错误
	if(i) return 1;
	i=Check_ALIGN16((u32*)&Meter_Ram_ALIGN16_List,sizeof(Meter_Ram_ALIGN16_List)/4);//返回:0=正确,1=错误
	if(i) return 1;


	return 0;
}

#endif



#if MainProtocol==376
u32 Test_ALIGN(void)//数据对齐测试;返回:0=正确,1=错误
{
	return 0;
}
#endif


#if MainProtocol==CSG
u32 Test_ALIGN(void)//数据对齐测试;返回:0=正确,1=错误
{
	return 0;
}
#endif




