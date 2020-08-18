
//�����֤����ԿЭ��
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"





u32 Uart_376_PW(u32 PORTn)//��Ϣ��֤���ֶ�PW;����0��Ч,1��Ч,4�ѷ�����֡
{
	u32 i;
	u32 mac;
	u32 DataLen;
	u32 p1;
	u32 p2;
	u64 PWL;
	u64 PWH;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	
	if(UART7Ctrl->Lock==0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	{
		return 1;
	}
	if(Comm_Ram->POWER<3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
  {
		return 1;
	}
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	
	i=MRR(ADDR_AFN04F5,1);//�ն�����ͨ����Ϣ��֤����
	switch(i)//��Ϣ��֤������
	{
		default://��ʾ����֤
			return 0;
		case 255://255��ʾר��Ӳ����֤����
//MAC������̵��������ݰ���:AFN,SEQ,���ݵ�Ԫ��ʶ,���ݵ�Ԫ,���������е�ʱ���ǩ(ע��376�������ʱ���ǩ���������)
			//����PW��ַȡMAC�Ĵ�
			DataLen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if((p8rxbuff[13]&0x80)==0x0)
			{//��Tp
				i=((u32)p8rxbuff)+6+DataLen-16;
			}
			else
			{//��Tp
				i=((u32)p8rxbuff)+6+DataLen-(16+6);
			}
			mac=MRR(i,4);//�����������ݵ��Ĵ���,Byte<=8
			PWL=MRR(i,8);//�����������ݵ��Ĵ���,Byte<=8
			PWH=MRR(i+8,8);//�����������ݵ��Ĵ���,Byte<=8

//			//����Tp,��6�ֽ�Tp�Ƶ�PW��������Դ���ݱ���MAC����ʱ����
//			if((p8rxbuff[13]&0x80)!=0x0)
//			{//��Tp
//				MEMRead(i,i+16,6);
//			}

			//����MAC������������ݳ���
			if((p8rxbuff[13]&0x80)==0x0)
			{//��Tp
				DataLen-=6+16;//��AFN��ʼ,��16�ֽ�PW���μ�
			}
			else
			{//��Tp
				DataLen-=6+16+6;//��AFN��ʼ,��16�ֽ�PW��ʱ���ǩ���μ�(ע��376�������ʱ���ǩ���������)
			}
			//P1��P2����ָʾ����ԿKID
			if((p8rxbuff[11]&0x1)==0x0)
			{//����ַ
				switch(p8rxbuff[12])
				{
					case 0x01:
						p2=0x20;
						break;
					case 0x04:
						p2=0x21;
						break;
					case 0x05:
						p2=0x22;
						break;
					case 0x10:
						p2=0x23;
						break;
				#if (USER/100)==0//���Ժ����
					case 0x0F:
						p2=0x24;
						break;
				#endif
					default:
						return 0;
				}
				if(MRR(ADDR_TerminalRealTimer+2,2)==0)//��ԿʧЧ��ʱ24Сʱ
				{//�ط���֡F4,ǩ��У�����
					p8txbuff[12]=0;//AFN=0
					p8txbuff[14]=0;
					p8txbuff[15]=0;
					p8txbuff[16]=8;//F4
					p8txbuff[17]=0;
					p8txbuff[18]=1;//��������:1=ǩ��У���,2=����У���,3=�Գ�MAC��֤ʧ��
					MC(0xff,((u32)p8txbuff)+19,16);
					p8txbuff[1]=12+17;//���ݳ���
					p8txbuff[2]=0;
					p8txbuff[6]=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
					i=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ(Tp)
					Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ			
					return 4;
				}
				i=TESAM_CMD_TxRx(0x80FA0300+p2,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto MACErr;
				}
				i=TESAM_CMD_TxRx(0x80FA0200,DataLen,((u32)p8rxbuff)+12);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto MACErr;
				}
				Data_Inverse(ADDR_UART7Rx+5,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				i=MRR(ADDR_UART7Rx+5,4);//�����������ݵ��Ĵ���,Byte<=8
				if(i==mac)
				{
					return 0;
				}
			}
			else
			{//���ַ
				//8022+KID1+KID2+LC+(00+TotalTN+130202224622+Data)+ 20130202224622+ 05
				i=p8rxbuff[9]+(p8rxbuff[10]<<8);//���ַ
				if(i==0xffff)
				{//�㲥��ַ
					switch(p8rxbuff[12])
					{
						case 0x01:
							p1=0x10;//KID1
							p2=0x1a;//KID2
							break;
						case 0x04:
							p1=0x11;//KID1
							p2=0x1b;//KID2
							break;
						case 0x05:
							p1=0x12;//KID1
							p2=0x1c;//KID2
							break;
						case 0x10:
							p1=0x13;//KID1
							p2=0x1d;//KID2
							break;
					#if (USER/100)==0//���Ժ����
						case 0x0F:
							p1=0x15;//KID1
							p2=0x1F;//KID2
							break;
					#endif
						default:
							return 0;
					}
					if(MRR(ADDR_TerminalRealTimer+2,2)==0)//��ԿʧЧ��ʱ24Сʱ
					{//�ط���֡F4,ǩ��У�����
						p8txbuff[12]=0;//AFN=0
						p8txbuff[14]=0;
						p8txbuff[15]=0;
						p8txbuff[16]=8;//F4
						p8txbuff[17]=0;
						p8txbuff[18]=1;//��������:1=ǩ��У���,2=����У���,3=�Գ�MAC��֤ʧ��
						MC(0xff,((u32)p8txbuff)+19,16);
						p8txbuff[1]=12+17;//���ݳ���
						p8txbuff[2]=0;
						p8txbuff[6]=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
						i=p8rxbuff[1]+(p8rxbuff[2]<<8);
						Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ(Tp)
						Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ			
						return 4;
					}
				}
				else
				{//���ַ
					switch(p8rxbuff[12])
					{
						case 0x01:
							p1=0x0c;//KID1
							p2=0x16;//KID2
							break;
						case 0x04:
							p1=0x0d;//KID1
							p2=0x17;//KID2
							break;
						case 0x05:
							p1=0x0e;//KID1
							p2=0x18;//KID2
							break;
						case 0x10:
							p1=0x0f;//KID1
							p2=0x19;//KID2
							break;
					#if (USER/100)==0//���Ժ����
						case 0x0F:
							p1=0x14;//KID1
							p2=0x1E;//KID2
							break;
					#endif
						default:
							return 0;
					}
					if(MRR(ADDR_TerminalRealTimer+2,2)==0)//��ԿʧЧ��ʱ24Сʱ
					{//�ط���֡F4,ǩ��У�����
						p8txbuff[12]=0;//AFN=0
						p8txbuff[14]=0;
						p8txbuff[15]=0;
						p8txbuff[16]=8;//F4
						p8txbuff[17]=0;
						p8txbuff[18]=1;//��������:1=ǩ��У���,2=����У���,3=�Գ�MAC��֤ʧ��
						MC(0xff,((u32)p8txbuff)+19,16);
						p8txbuff[1]=12+17;//���ݳ���
						p8txbuff[2]=0;
						p8txbuff[6]=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
						i=p8rxbuff[1]+(p8rxbuff[2]<<8);
						Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ(Tp)
						Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ			
						return 4;
					}
				}
				//MAC����
				DataLen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				if((p8rxbuff[13]&0x80)==0x0)
				{//��Tp
					i=((u32)p8rxbuff)+6+DataLen-16;
				}
				else
				{//��Tp
					i=((u32)p8rxbuff)+6+DataLen-(16+6);
				}
				Data_Inverse(i,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//����MAC������������ݳ���
				if((p8rxbuff[13]&0x80)==0x0)
				{//��Tp
					DataLen-=6+16;//��AFN��ʼ,��16�ֽ�PW���μ�
				}
				else
				{//��Tp
					DataLen-=6+16+6;//��AFN��ʼ,��16�ֽ�PW��ʱ���ǩ���μ�(ע��376�������ʱ���ǩ���������)
				}
				
				p8rxbuff[3]=0;//�㲥�������ͣ��̶�Ϊ0������+MAC
				p8rxbuff[4]=p8rxbuff[10];//���ַ���ֽ�
				p8rxbuff[5]=p8rxbuff[9];//���ַ���ֽ�
				p8rxbuff[6]=0x13;//��վʱ�䣻6�ֽڣ�Ĭ��130202224622
				p8rxbuff[7]=0x02;
				p8rxbuff[8]=0x02;
				p8rxbuff[9]=0x22;
				p8rxbuff[10]=0x46;
				p8rxbuff[11]=0x22;
				//12 1 Ӧ�ò㹦����AFN
				//13 1 ֡������SEQ
				//14 4 ���ݵ�Ԫ��ʶ
				//18 n ����
				//	 4 MAC
				p8rxbuff[12+DataLen+4+0]=0x20;
				p8rxbuff[12+DataLen+4+1]=0x13;
				p8rxbuff[12+DataLen+4+2]=0x02;
				p8rxbuff[12+DataLen+4+3]=0x02;
				p8rxbuff[12+DataLen+4+4]=0x22;
				p8rxbuff[12+DataLen+4+5]=0x46;
				p8rxbuff[12+DataLen+4+6]=0x22;
				p8rxbuff[12+DataLen+4+7]=0x05;
				
				i=TESAM_CMD_TxRx(0x80220000+(p1<<8)+p2,9+DataLen+4+8,((u32)p8rxbuff)+3);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto MACErr;
				}
				return 0;
			}
		MACErr://����
			//ERC20�¼���¼
			i=p8rxbuff[11]>>1;//����վ��ַ
			ERC20_Event(PWL,PWH,i);//��Ϣ��֤�����¼,���:PWH_PWL��Ϣ��֤��,MSA=����վ��ַ
//			//ȡ�����
//			TESAM_CMD_TxRx(0x801A0800,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
//			MR(ADDR_TESAM_User+8,ADDR_UART7Rx+5,4);
			//����֡
			p8txbuff[12]=0;//AFN=0
			p8txbuff[14]=0;
			p8txbuff[15]=0;
			p8txbuff[16]=8;//F4
			p8txbuff[17]=0;
			p8txbuff[18]=3;//��������:1=ǩ��У���,2=����У���,3=�Գ�MAC��֤ʧ��
				
			MC(0xff,((u32)p8txbuff)+19,8);//�������8�ֽڵĵ�ǰ�ն������Ϊ8�ֽ�0xFF
			MR(((u32)p8txbuff)+19+8,ADDR_TESAM_User,8);//��8�ֽ�ΪоƬ���к�
			Data_Inverse(((u32)p8txbuff)+19,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			Data_Inverse(((u32)p8txbuff)+19+8,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			p8txbuff[1]=12+17;//���ݳ���
			p8txbuff[2]=0;
			p8txbuff[6]=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+i-6);//������Ϣ(Tp)
			Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
			return 4;
	}
}

/*
֡��ʽ
0 1 0x68
1 2 L(�û��򳤶�,����ʱȥ��D1D0��ʾ��Э��ż�����2λ��
3 2 L(�û��򳤶ȣ�
5 1 0x68
6 1 ������C
7 2 ����������A1
9 2 �ն˵�ַA2(�ն˵�ַA2ѡַ��ΧΪ1��65535��A2=0000HΪ��Ч��ַ��A2=FFFFH��A3��D0λΪ"1"ʱ��ʾϵͳ�㲥��ַ)
11 1 ��վ��ַ�����ַ��־A3(A3��D0λΪ�ն����ַ��־��D0=0��ʾ�ն˵�ַA2Ϊ����ַ��D0=1��ʾ�ն˵�ַA2Ϊ���ַ��A3��D1��D7���0��127����վ��ַMSA)
12 1 Ӧ�ò㹦����AFN
13 1 ֡������SEQ
14 4 ���ݵ�Ԫ��ʶ
18 n ���ݵ�Ԫ
...
PW 16
Tp 6
CS 1
0x16 1
*/


void Uart_AFN06(u32 PORTn)//�����֤����ԿЭ��
{
	u32 i;
	u32 x;
	u32 DIlen;
	u32 DIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 Fn;
	u32 p1;
//	u32 ErrType;//��������:1=ǩ��У���,2=����У���,3=�Գ�MAC��֤ʧ��
  
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 *p8esamrxbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	
	if(UART7Ctrl->Lock==0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	{
		return;
	}
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8esamrxbuff=(u8 *)(ADDR_UART7Rx);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//��Ӧ������ 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<(12+16))
	{
err:
		//ȫ������
		Uart_376_ACK(PORTn,2);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ(Tp)
		Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
		return;
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
	DIaddr=((u32)p8rxbuff)+14;
	TXaddr=((u32)p8txbuff)+14+4;
	TXlen=0;

	CopyDI(PORTn,DIaddr);//COPY���ݱ�ʶ�����ƻ���׼���ֽ�
	if(DIlen<4)
	{
		goto err;
	}
	else
	{
		i=DItoFnPn(PORTn);//���ݱ�ʶ�ֽ�ΪFnPn
		Fn=(i>>16);
		switch(Fn)
		{
			case 14://F14	�Գ���Կ����
			case 15://F15	�ն�֤�����(����)
			case 16://F16	CA֤�����
			//case 17://F17	�ڲ���֤
			//case 18://F18	�ⲿ��֤
			//case 19://F19	״̬�л�
			case 20://F20	�����߼�����
			case 21://F21	ת������Ȩ
				if(MRR(ADDR_TerminalRealTimer+2,2)==0)//��ԿʧЧ��ʱ24Сʱ
				{//�ط���֡F4,ǩ��У�����
				err_F4:
					p8txbuff[12]=0;//AFN=0
					p8txbuff[14]=0;
					p8txbuff[15]=0;
					p8txbuff[16]=8;//F4
					p8txbuff[17]=0;
					p8txbuff[18]=1;//��������:1=ǩ��У���,2=����У���,3=�Գ�MAC��֤ʧ��
					MC(0xff,((u32)p8txbuff)+19,16);
					p8txbuff[1]=12+17;//���ݳ���
					p8txbuff[2]=0;
					p8txbuff[6]=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
					DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ(Tp)
					Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
					return;
				}
				break;
		}
		switch(Fn)
		{
			case 11://F11	��ȡ�ն���Ϣ
				//��ȡоƬ״̬��Ϣ1�ֽ�
				i=TESAM_CMD_TxRx(0x800E0005,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					UART7Ctrl->Task=0;//�ص�Դ������
					goto err;
				}
				p1=MRR(ADDR_UART7Rx+5,1);
				//��ȡоƬ���к�8�ֽ�
				i=TESAM_CMD_TxRx(0x800E0002,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(ADDR_TESAM_User,ADDR_UART7Rx+5,8);
				MR(TXaddr,ADDR_UART7Rx+5,8);
				Data_Inverse(TXaddr,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//��ȡ֤�����кţ�16�ֽ�
				i=TESAM_CMD_TxRx(0x80320002+(p1<<8),0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(TXaddr+8,ADDR_UART7Rx+5,16);
				Data_Inverse(TXaddr+8,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//��ȡ���߼�����4�ֽ�
				i=TESAM_CMD_TxRx(0x800E0003,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(TXaddr+8+16,ADDR_UART7Rx+5,4);
				Data_Inverse(TXaddr+8+16,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				MWR(p1,TXaddr+8+16+4,1);//оƬ״̬��Ϣ1�ֽ�
				//��ȡ��Կ�汾��8�ֽ�
				TESAM_CMD_TxRx(0x800E0006,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(TXaddr+8+16+4+1,ADDR_UART7Rx+5,8);
				Data_Inverse(TXaddr+8+16+4+1,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				TXlen=8+16+4+1+8;
			ACKDATA:
				TXlen+=12;//1�ֽڿ�����+5�ֽڵ�ַ��+1�ֽ�AFN+1�ֽ�SEQ+4�ֽڱ�ʶ=12�ֽ�
				p8txbuff[1]=TXlen;
				p8txbuff[2]=TXlen>>8;
				DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
				Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ(Tp)
				Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
				return;
			case 12://F12	�Ự��ʼ��/�ָ�
				if(p8rxbuff[18+3]==1)
				{//�Ự�ָ�
					if(MRR(ADDR_TerminalRealTimer+2,2)==0)//��ԿʧЧ��ʱ24Сʱ
					{//�ط���֡F4,ǩ��У�����
						goto err_F4;
					}
				}
				i=MRR(((u32)p8rxbuff)+18,2);//����1����
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				//����1����=Ln+86
				if(i<86)
				{
					goto err;
				}
				p1=i-86;//��վ֤�鳤��
				Data_Inverse(((u32)p8rxbuff)+18+4,p1);//��վ֤��
				Data_Inverse(((u32)p8rxbuff)+18+4+p1,16);//EKS1
				Data_Inverse(((u32)p8rxbuff)+18+4+p1+16,4);//MAC1
				Data_Inverse(((u32)p8rxbuff)+18+4+p1+16+4,64);//ǩ������S1
				MR(((u32)p8rxbuff)+11,((u32)p8rxbuff)+18+2,i);
				//����1+YYYYMMDDhhmmss
				p8rxbuff[11+i]=0x20;
				MR(((u32)p8rxbuff)+11+i+1,ADDR_TYMDHMS,6);
				Data_Inverse(((u32)p8rxbuff)+11+i+1,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				i=TESAM_CMD_TxRx(0x84100000,i+7,((u32)p8rxbuff)+11);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				i=(p8esamrxbuff[3]<<8)+p8esamrxbuff[4];//�������ݳ���
				if((18+2+i+8+2)>Get_LEN_UARTnTx(PORTn))
				{
					goto err;
				}
				if(p8esamrxbuff[6]==0)//�ỰID:0=��ʼ��,1=�ָ�
				{//�Ự��ʼ��
					//Version(1�ֽ�)+Session ID(1�ֽڣ�00)+CRL���к�(16�ֽ�)+��վ֤��C1���к�(16�ֽ�)+�ն�֤��C2(Var)+ Eks2(R2) (16�ֽ�)+����ǩ��S2(64�ֽ�)
					if(i<144)
					{
						goto err;
					}
					p1=i-114;//���ر���2����=Ln+114
					MWR(i,((u32)p8txbuff)+18,2);
					MR(((u32)p8txbuff)+18+2,((u32)p8esamrxbuff)+5,i);
					//1�ֽڰ汾��
					//1�ֽڻỰID
					Data_Inverse(((u32)p8txbuff)+18+4,16);//CRL���к�
					Data_Inverse(((u32)p8txbuff)+18+4+16,16);//��վ���к�
					Data_Inverse(((u32)p8txbuff)+18+4+16+16,p1);//�ն�֤��
					Data_Inverse(((u32)p8txbuff)+18+4+16+16+p1,16);//EKS2
					Data_Inverse(((u32)p8txbuff)+18+4+16+16+p1+16,64);//ǩ������S2
					TXlen=2+i;
				}
				else
				{//�Ự�ָ�
					//Version(1�ֽ�)+Session ID(1�ֽ�)+ Eks2(R2) (16�ֽ�)+R3(16�ֽ�)+MAC2(4�ֽ�)
					MWR(38,((u32)p8txbuff)+18,2);
					MR(((u32)p8txbuff)+18+2,((u32)p8esamrxbuff)+5,i);
					//1�ֽڰ汾��
					//1�ֽڻỰID
					Data_Inverse(((u32)p8txbuff)+18+4,16);//Eks2(R2)
					Data_Inverse(((u32)p8txbuff)+18+4+16,16);//R3
					Data_Inverse(((u32)p8txbuff)+18+4+16+16,4);//MAC2
					TXlen=2+38;
					MWR(24*60,ADDR_TerminalRealTimer+2,2);//��ԿʧЧ��ʱ24Сʱ
				}
				goto ACKDATA;//�ϴ�����2
			case 13://F13	�ỰЭ��
				i=MRR(((u32)p8rxbuff)+18,2);//����3����
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				//����3����=Ln+181
				if(i<181)
				{
					goto err;
				}
				p1=i-181;//��վ֤����֤�볤��
				Data_Inverse(((u32)p8rxbuff)+18+2,113);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+2+113,p1);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+2+113+p1,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+2+113+p1+4,64);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				i=TESAM_CMD_TxRx(0x84120000,i,((u32)p8rxbuff)+18+2);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				i=(p8esamrxbuff[3]<<8)+p8esamrxbuff[4];//�������ݳ���
				if(i!=(16+4))
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,i);
				Data_Inverse(((u32)p8txbuff)+18,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				Data_Inverse(((u32)p8txbuff)+18+16,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				TXlen=16+4;
				MWR(24*60,ADDR_TerminalRealTimer+2,2);//��ԿʧЧ��ʱ24Сʱ
				goto ACKDATA;
			case 14://F14	�Գ���Կ����
				x=MRR(((u32)p8rxbuff)+18+1,1);//������Կ��������
				i=32*x;//ÿ����Կ32�ֽ�
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				while(x!=0)
				{
					Data_Inverse(((u32)p8rxbuff)+18+2+(32*(x-1)),32);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
					x--;
				}
				i=TESAM_CMD_TxRx(0x84240100,i+1,((u32)p8rxbuff)+18+1);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 15://F15	�ն�֤�����(����)
				//��ȡоƬ״̬��Ϣ1�ֽ�
				i=TESAM_CMD_TxRx(0x800E0005,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				p1=MRR(ADDR_UART7Rx+5,1);
				//��ȡ�ն�֤�����к�16�ֽ�
				i=TESAM_CMD_TxRx(0x80320002+(p1<<8),0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//�ն������ǶԳ���Կ�ԣ����ع�Կ����Ϊ64�ֽ�
				i=TESAM_CMD_TxRx(0x802C0001,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18+16,((u32)p8esamrxbuff)+5,64);
				Data_Inverse(((u32)p8txbuff)+18+16,64);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				TXlen=16+64;
				goto ACKDATA;
			case 16://F16	CA֤�����
				//֤����±��İ�����֤����������е�P1��P2�����Ⱥ�֤����������
				i=MRR(((u32)p8rxbuff)+18,2);//���ĳ���
				if((18+2+i+16+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				Data_Inverse(((u32)p8rxbuff)+18+2,i);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				p1=p8rxbuff[18+2];//p1
				p1<<=8;
				p1+=p8rxbuff[18+2+1];//p2
				i=p8rxbuff[18+4];//Lc
				i<<=8;
				i+=p8rxbuff[18+4+1];//Lc���ֽ�
				i=TESAM_CMD_TxRx(0x84300000+p1,i,((u32)p8rxbuff)+18+2+4);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 17://F17	�ڲ���֤(����������ڱ���ͨ�ſ�,Զ��ͨ�ſ�Ӧ��֧�ָ�����)
				//������վ�µ������R4
				Data_Inverse(((u32)p8rxbuff)+18,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				i=TESAM_CMD_TxRx(0x84260000,16,((u32)p8rxbuff)+18);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//ȡ16�ֽ������R5
				i=TESAM_CMD_TxRx(0x801A1000,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18+16,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18+16,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				TXlen=16+16;
				goto ACKDATA;
			case 18://F18	�ⲿ��֤(����������ڱ���ͨ�ſ�,Զ��ͨ�ſ�Ӧ��֧�ָ�����)
				//����R5
				Data_Inverse(((u32)p8rxbuff)+18,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				i=TESAM_CMD_TxRx(0x84280000,16,((u32)p8rxbuff)+18);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				//ȡ16�ֽ������R6,����״̬�л�
				i=TESAM_CMD_TxRx(0x801A1000,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				MR(((u32)p8txbuff)+18,((u32)p8esamrxbuff)+5,16);
				Data_Inverse(((u32)p8txbuff)+18,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				TXlen=16;
				goto ACKDATA;
			case 19://F19	״̬�л�
				Data_Inverse(((u32)p8rxbuff)+18+1,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				Data_Inverse(((u32)p8rxbuff)+18+1+16,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				p1=p8rxbuff[18];//P1����վ�·��л�״̬00�Ӳ���֤���л�����ʽ֤��,01����ʽ֤���л�������֤��
				i=TESAM_CMD_TxRx(0x842A0000+(p1<<8),20,((u32)p8rxbuff)+18+1);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 20://F20	�����߼�����
				Data_Inverse(((u32)p8rxbuff)+18,20);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				i=TESAM_CMD_TxRx(0x84200000,20,((u32)p8rxbuff)+18);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				goto ACKOK;
			case 21://F21	ת������Ȩ
				Data_Inverse(((u32)p8rxbuff)+18,32);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				i=TESAM_CMD_TxRx(0x80160000,32,((u32)p8rxbuff)+18);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
				if(i)
				{
					goto err;
				}
				//��ԭ����
				for(i=0;i<MaxAFN10F12TaskType;i++)
				{
					MC(0,ADDR_AFN10F12+(i*LEN_AFN10F12PerTask),2+1+2+LEN_AFN10F12TaskData+2);//������
				}
				//��������ɱ�־
				MC(0,ADDR_AFN10F13,MaxRS485AddCarrierPn*MaxAFN10F12TaskType);
				//������������,��ִ�б����������·��������������������ֵ��������ʱ�侹����ͻ
			//RS485-1
				if(Get_LEN_UARTnCtrl(RS485_1PORT)!=0)//�õ�UART�������ݳ���
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
					UARTCtrl->BatchMeterCount=0;//ת���������������
				}
			//RS485-2
				if(Get_LEN_UARTnCtrl(RS485_2PORT)!=0)//�õ�UART�������ݳ���
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
					UARTCtrl->BatchMeterCount=0;//ת���������������
				}
			//RS485-3
				if(Get_LEN_UARTnCtrl(RS485_3PORT)!=0)//�õ�UART�������ݳ���
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
					UARTCtrl->BatchMeterCount=0;//ת���������������
				}
			//RS485-4(�ز�)
				if(Get_LEN_UARTnCtrl(RS485_4PORT)!=0)//�õ�UART�������ݳ���
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					UARTCtrl->BatchMeterCount=0;//ת���������������
				}
				goto ACKOK;
			default:
				goto err;
		
		}
	}
/*
err_F4://Ӳ����ȫ��֤����
	p8txbuff[12]=0;//AFN=0
	p8txbuff[14]=0;//Pn=0
	p8txbuff[15]=0;
	p8txbuff[16]=8;//F4
	p8txbuff[17]=0;
	p8txbuff[18]=ErrType;//��������:1=ǩ��У���,2=����У���,3=�Գ�MAC��֤ʧ��
	MEMClr(0xff,((u32)p8txbuff)+19,16);
	p8txbuff[1]=12+17;//���ݳ���
	p8txbuff[2]=0;
	p8txbuff[6]=0x80;//������DIR=1����,PRM=0��ʾ��֡�������ԴӶ�վ,Ҫ�����λACD��0,������=0
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ(Tp)
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
	return;
*/
ACKOK:
	//ȫ��ȷ��
	Uart_376_ACK(PORTn,1);//ȫ��ȷ�ϣ�����֡;ACK=1ȷ��=2����;������д���ݳ���,������,AFN,֡������,���ݵ�Ԫ��ʶ
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUXTp(PORTn,((u32)p8rxbuff)+6+DIlen-6);//������Ϣ(Tp)
	Uart_376_CS((u32)p8txbuff);//��У�����,ͬʱ�ӽ���0x16;�޸ĳ����ֽڵı�־λ
}















