
#ifndef TerminalQCSG_Uart_Manage_H
#define TerminalQCSG_Uart_Manage_H

#define RCV_UART_FRAME_TIME_OUT				(30)
#define RCV_GPRS_FRAME_TIME_OUT				(500)
#define RCV_ENET_FRAME_TIME_OUT				(1000)
#define RCV_SMS_FRAME_TIME_OUT				(6000)

#define UART_TX_TIME_OUT					(1000)
#define GPRS_UART_TX_TIME_OUT				(6000)



void Refresh485PortFunction(u32 PORTn, RS485PortParaStruct* PortPara);


void Terminal_UartInit_Manage(void);//Terminal_Uart初始化管理
void Terminal_Uart_Manage(void);//Terminal_Uart管理


void QCSG_Local_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask);

void DLT_645_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask);

void YaoXin_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask);

void Mixed_645_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask);


void QCSG_UnKnown_Protocol_Req_Process(u32 PORTn, UartTaskCtrlStruct* UartTask);



void GPRSModuleManage(u32 PORTn);

void GPRSLinkMaintain(u32 PORTn);

void EthernetModuleManage(u32 PORTn);

void EthernetLinkMaintain(u32 PORTn);

void UartTaskProcess(u32 PORTn, UartTaskCtrlStruct* UartTask);

void UartPortManage(u32 PORTn, UartTaskCtrlStruct* UartTask);

u32 UartReveiveFrame(u32 PORTn);

void SetPortTxLed(u32 PORTn);

u16 GetUartPortBpsCtrl(u32 PORTn);

//u16 ConvertToBpsCtrl(u8 BaudRateCode, u8 ParityCode, u8 DataBitCode, u8 StopBitCode);





#endif
