
//UART����
#ifndef STM32F4xx_UART_H
#define STM32F4xx_UART_H
#ifdef __cplusplus
 extern "C" {
#endif

u32 Get_ADDR_UARTn(u32 PORTn);//�õ�UART�����׵�ַ
u32 Get_LEN_UARTnIntRx(u32 PORTn);//�õ�UART�жϽ��ջ��峤��
u32 Get_LEN_UARTnCtrl(u32 PORTn);//�õ�UART�������ݳ���
u32 Get_LEN_UARTnRx(u32 PORTn);//�õ�UART���ջ��峤��
u32 Get_LEN_UARTnTx(u32 PORTn);//�õ�UART���ͻ��峤��
u32 Get_LEN_UARTnTxMax(u32 PORTn);//�õ�UART������󻺳峤��
u32 Get_LEN_UARTnFnDataBuff(u32 PORTn);//�õ�UART�������ݻ��峤��
u32 Get_ADDR_UARTnTimer(u32 PORTn);//�õ�UART��ʱ����ַ
u32 Get_ADDR_UARTnCtrl(u32 PORTn);//�õ�UART��������ַ
u32 Get_ADDR_UARTnRx(u32 PORTn);//�õ�UART���ջ����ַ
u32 Get_ADDR_UARTnTx(u32 PORTn);//�õ�UART���ͻ����ַ
u32 Get_ADDR_UARTnFnDataBuff(u32 PORTn);//�õ�UART�������ݻ����ַ
	#if MainProtocol==698
u32 Get_ADDR_UARTnTimer_S(u32 PORTn);//�õ�UART�붨ʱ����ַ
	#endif
	 
u32 Get_UARTBps(u32 PORTn);//�õ�UARTͨ������



void Init_UARTn(u32 PORTn);//��ʼ��UART��,���ͨ�ſ�����������
void Start_UARTn_Tx(u32 PORTn);//����UART�ڷ���
u32 Wait_UARTn_Tx(u32 PORTn);//�ȴ�UART�ڷ���,����0=�������,1=����û���

u32 FeatureWord_CtrlWord(u32 word);//DL645-07������תΪͨ�����ʿ����ÿ�����
void PrintString_UARTn(u32 PORTn,u8 * pStr);//UART��ӡ�ַ���



#ifdef __cplusplus
 }
#endif 
#endif
