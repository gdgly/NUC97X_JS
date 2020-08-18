
//UART ��������
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"






void Uart_AFN01(u32 PORTn)//��λ
{
	u32 i;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u32 * p32;
  UARTCtrl_TypeDef * UARTCtrl;
	
  UARTCtrl=(UARTCtrl_TypeDef *)(Get_ADDR_UARTnCtrl(PORTn));
	p8rxbuff=(u8 *)(Get_ADDR_UARTnRx(PORTn));
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	
	p8txbuff[6]=(p8txbuff[6]&0xf0);//��Ӧ������ 0
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<(12+16))
	{
		goto err;
	}
//Tpʱ���ǩ��Ч�ж�
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		if(DIlen<(12+16+6))
		{
			goto err;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//ʱ���ǩ��Ч�ж�,���ʱ���ǩ��ַ,����0��Ч,1��Ч
		if(i!=0x0)
		{
			return;//�����ñ���
		}
	}
//PW��֤
	i=Uart_376_PW(PORTn);//��Ϣ��֤���ֶ�PW;���PW��ַ,����0��Ч,1��Ч
	if(i==4)
	{
		return;
	}
	if(i!=0x0)
	{
err:
		//ȫ������
		Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		return;
	}

	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//�û������ݱ�ʶ�Ͳ������ݳ���
	if((p8rxbuff[13]&0x80)!=0x0)
	{//��Tp
		DIlen-=(8+16+6);
	}
	else
	{
		DIlen-=(8+16);
	}
	if(DIlen<4)
	{
		goto err;
	}
//����(����վ������10������1�����ݣ�����Ӧ�ò�����ȷ�ϣ�CON=1������·����)
	DIaddr=((u32)p8rxbuff)+14;
//	RXaddr=ADDR_UART_BUFF+4+UART_FIFO_LEN+UART_CTRL_LEN+14+4;
	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	if(DIlen<4)
	{
		goto err;
	}
	else
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		Fn=(i>>16);
		Pn=i&0xffff;
		if(Fn==0x0)
		{
			goto err;
		}
		if(Fn>5)
		{
			goto err;
		}
		if(Pn!=0x0)
		{
			goto err;
		}
		p32=(u32 *)(ADDR_DATABUFF);
		p32[0]=(u32)UARTCtrl;
		p32[1]=Fn;
		MW(ADDR_DATABUFF,ADDR_AFN01Fx,5);
		//ȫ��ȷ��
		Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		
		BUZZ_Generate(2);//��������,��ڷ�����
		Terminal_Ram->ResetCommand_S_Timer=3;//��ʱ5S
	}
}

