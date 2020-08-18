

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/BroadcastTime.h"
#include "../DL698/RM_RS485.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../Device/MEMRW.h"
#include "../DL698/RM_TASK.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_Uart.h"




void BroadcastTime_RS485(u32 PORTn)//�㲥Уʱ-rs485;��ɺ���UARTCtrl->BroadcastTimeTask=0
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8tx;
	u16 * p16timer;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->BroadcastTimeTask)//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
	{
		default:
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 1:
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 2:
			RM_ADDR_Display(PORTn);//�����ַ��ʾ
			p16timer[0]=1000/10;//��ʱ1������
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 3://DL698 9600����
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(6<<5)+0x0b;
			i=BroadcastTime_TxDATA_DL698(p8tx);//DL698�㲥Уʱ����֡;����:��֡���ܳ����ֽ���
			UARTCtrl->TxByte=i;//�����ֽ���
			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;//���ڷ���
			RS485_TxLED(PORTn);//RS485 LED����ָʾ��
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 4:
			RS485_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				p16timer[0]=1000/10;//��ʱ1������
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			}
			break;
		case 5://DL645 1200����
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			i=BroadcastTime_TxDATA_DL645(p8tx);//DL645�㲥Уʱ����֡;����:��֡���ܳ����ֽ���
			UARTCtrl->TxByte=i;//�����ֽ���
			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;//���ڷ���
			RS485_TxLED(PORTn);//RS485 LED����ָʾ��
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 6:
			RS485_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				p16timer[0]=1000/10;//��ʱ1������
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			}
			break;
		case 7://DL645 2400����
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->BpsCtrl=(3<<5)+0x0b;
			i=BroadcastTime_TxDATA_DL645(p8tx);//DL645�㲥Уʱ����֡;����:��֡���ܳ����ֽ���
			UARTCtrl->TxByte=i;//�����ֽ���
			//����
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;//���ڷ���
			RS485_TxLED(PORTn);//RS485 LED����ָʾ��
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 8:
			RS485_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<3)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				p16timer[0]=1000/10;//��ʱ1������
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			}
			break;
		case 9:
			if(p16timer[0]!=0)
			{
				break;
			}
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
	}
}






#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=

void BroadcastTime_3762(u32 PORTn)//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
{
	u32 i;
	u32 x;
	u64 d64;
	u8 *p8rx;
	u8 *p8tx;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef *UARTCtrlProxy;
	u8* p8Data;	
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->BroadcastTimeTask)//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
	{
		default:
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 1:
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ�,2-127Уʱ-2����;0x81=Уʱ-3�ȴ�,0x82-0xFFУʱ-3����
			break;
		case 2://��֡������ʱ
//			p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
//			if(p16timer[0]!=0)//��֡����������ʱ
//			{
//				return;
//			}
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
			break;
		case 3://��ͣ·��
			Terminal_Router->StopFlags|=(1<<4);//��ͣ·��
			Terminal_Router->RouterCtrl=2;//·��������0=��,1=����,2=��ͣ,3=�ָ�;��ɺ��Զ��ص�0
		
			if(UARTCtrl->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
			{
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			else
			{//��͸��ת�������·��Ĺ㲥Уʱ
				UARTCtrlProxy=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(UARTCtrl->ProxyPORTn);
				if(UARTCtrlProxy->RMprotocol==3)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
				{//DL698
					UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				else
				{//
					UARTCtrl->BroadcastTimeTask=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
			}
			break;
		case 4://�ȴ�1��
//			MC(0,ADDR_DATABUFF+18,4);
//			CopyString((u8 *)"�ز�׼��698�㲥Уʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
//			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			p16timer[0]=0;//60000/10;
			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 5://ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
			if(p16timer[0]==0)
			{
				MC(0,ADDR_DATABUFF+18,4);
				CopyString((u8 *)"�ز�����698�㲥Уʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				p8tx+=128;
				p8tx[0]=0;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
				//p8tx[1]=32;//���ĳ���
				//MC(0xfe,((u32)p8tx)+2,4);
				//p8tx+=6;
				p8tx[1]=32-4;//���ĳ���
				
				p8tx+=6-4;
			//��ʼ�ַ���68H��  1Byte
				p8tx[0]=0x68;
			//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
			//������C  1Byte
				p8tx[3]=0x43;
			//��ַ��A
				p8tx[4]=0xC0;
				p8tx[5]=0xAA;
			//�ͻ�����ַCA  1Byte
				p8tx[6]=0;
			//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
			//APDU
				p8tx[9]=0x07;
				p8tx[10]=1;
				p8tx[11]=0;//PIID
				p8tx[12]=0x40;//OAD
				p8tx[13]=0x00;
				p8tx[14]=0x7F;
				p8tx[15]=0;
				p8tx[16]=DataType_date_time_s;
				if(UARTCtrl->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
				{
					MR((u32)p8tx+17,ADDR_DL698YMDHMS,7);
				}
				else
				{
					p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//�õ�UART�������ݻ����ַ
					MR((u32)p8tx+17,(u32)p8Data+17,7);
				}
				i=24;
			//ʱ���ǩ
				p8tx[i]=0;
			//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
			//�����ַ���16H��
				i+=4;
				
				i-=2;
				p8tx[1]=i;
				p8tx[2]=i>>8;
				Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
				//i+=2+6;
				i+=2+6-4;
				Uart_3762_TxData(PORTn,0x03,9,i);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
		case 6://���㲥Уʱ
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=MRR(((u32)p8rx)+10,3);
			if((i!=0x010103)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
				{
					Terminal_Router->ReRouterCtrl++;
					UARTCtrl->BroadcastTimeTask--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				else
				{
					UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				break;
			}
			x=MRR(((u32)p8rx)+13,2);//�㲥ͨ���ӳ�ʱ����
			//��DL698�㲥Уʱ
			p8tx+=128;
			p8tx[0]=0;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
			//p8tx[1]=32;//���ĳ���
			//MC(0xfe,((u32)p8tx)+2,4);
			//p8tx+=6;
			
			p8tx[1]=32-4;//���ĳ���
			p8tx+=6-4;
		//��ʼ�ַ���68H��  1Byte
			p8tx[0]=0x68;
		//������L  2Byte(����ʼ�ַ��ͽ����ַ�֮���֡�ֽ���)
		//������C  1Byte
			p8tx[3]=0x43;
		//��ַ��A
			p8tx[4]=0xC0;
			p8tx[5]=0xAA;
		//�ͻ�����ַCA  1Byte
			p8tx[6]=0;
		//֡ͷУ��HCS  2Byte(֡ͷУ��HCSΪ2�ֽڣ��Ƕ�֡ͷ���ֳ���ʼ�ַ���HCS����֮��������ֽڵ�У��)
		//APDU
			p8tx[9]=0x07;
			p8tx[10]=1;
			p8tx[11]=0;//PIID
			p8tx[12]=0x40;//OAD
			p8tx[13]=0x00;
			p8tx[14]=0x7F;
			p8tx[15]=0;
			p8tx[16]=DataType_date_time_s;
			if(UARTCtrl->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
			{
				MR((u32)p8tx+17,ADDR_DL698YMDHMS,7);
			}
			else
			{
				p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//�õ�UART�������ݻ����ַ
				MR((u32)p8tx+17,(u32)p8Data+17,7);
			}
			pYYMDHMS_AddS_hex(p8tx+17,x);//��������ʱ�����S��,���ؼ�S������������ʱ����
			
			i=24;
		//ʱ���ǩ
			p8tx[i]=0;
		//֡У��FCS  2Byte(�Ƕ���֡����ʼ�ַ��������ַ���FCS����֮��������ֽڵ�У��)
		//�����ַ���16H��
			i+=4;
			
			i-=2;
			p8tx[1]=i;
			p8tx[2]=i>>8;
			Uart_698_HCSFCS(p8tx);//��֡У�����(HCS��FCS),ͬʱ�ӽ���0x16;����0=��ȷ��1=����
			//i+=2+6;
			i+=2+6-4;
			Uart_3762_TxData(PORTn,0x05,3,i);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			//Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
			{
				Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//���㲥��ʱʱ����(���ֶ���)
			}
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 7://����ȷ��֡
			if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
			{//0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ���
				switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					case 1://1=���1֡����
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
							UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
						}
						break;
					case 0://0=����
					case 2://2=���ճ�ʱ(�����)
						if(Terminal_Ram->RouterCheck_S_Timer==0)//���㲥��ʱʱ����(���ֶ���)
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
							UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
						}
						break;
				}
			}
			else
			{//1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
				switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					case 1://1=���1֡����
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							//ȡ�ȴ�ʱ��
							i=p8rx[17];
							i+=p8rx[18]<<8;
							i*=1000/10;
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16timer[0]=i;
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							break;
						}
						else
						{
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								UARTCtrl->BroadcastTimeTask--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
							else
							{
								UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
						}
						break;
					case 0://0=����
					case 2://2=���ճ�ʱ(�����)
						UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						break;
				}
			}
			break;
		case 8://�ȴ��㲥��ʱ
			if(p16timer[0]==0)
			{
				if(UARTCtrl->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
				{
					UARTCtrl->BroadcastTimeTask=10;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				else
				{//͸��ת�������·��Ĺ㲥Уʱ
					UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
			}
			break;
//		case 9:
//			break;
		case 10:
//			MC(0,ADDR_DATABUFF+18,4);
//			CopyString((u8 *)"�ز�׼��645�㲥Уʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
//			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			p16timer[0]=0;//5000/10;
			Terminal_Router->ReRouterCtrl=0;//·���������ط�����
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			break;
		case 11://ͨ����ʱ��ع㲥ͨ��ʱ����ѯ
			if(p16timer[0]==0)
			{
				MC(0,ADDR_DATABUFF+18,4);
				CopyString((u8 *)"�ز�����645�㲥Уʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				p8tx+=128;
				p8tx[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
				//p8tx[1]=22;//���ĳ���
				//MC(0xfe,((u32)p8tx)+2,4);
				p8tx[1]=22-4;//���ĳ���
				p8tx-=4;
				p8tx[6]=0x68;
				MC(0x99,((u32)p8tx)+7,6);
				p8tx[13]=0x68;
				p8tx[14]=0x08;
				p8tx[15]=0x06;

				if(UARTCtrl->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
				{
					MR((u32)p8tx+16,ADDR_MYMDHMS,6);
				}
				else
				{//��͸��ת�������·��Ĺ㲥Уʱ
					p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//�õ�UART�������ݻ����ַ
					MR((u32)p8tx+16,(u32)p8Data+10,6);
					for(i=0;i<6;i++)
					{
						p8tx[16+i]-=0x33;
					}
				}
				
				for(i=0;i<6;i++)
				{
					p8tx[16+i]+=0x33;
				}
				x=0;
				for(i=0;i<16;i++)
				{
					x+=p8tx[6+i];
				}
				p8tx[22]=x;
				p8tx[23]=0x16;
				//Uart_3762_TxData(PORTn,0x03,9,24);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
				Uart_3762_TxData(PORTn,0x03,9,20);//376.2�����������뷢����;���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ���������������ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
				UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
		case 12://���㲥Уʱ
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=MRR(((u32)p8rx)+10,3);
			if((i!=0x010103)||(UARTCtrl->Task!=1))
			{
				if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
				{
					Terminal_Router->ReRouterCtrl++;
					UARTCtrl->BroadcastTimeTask--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				else
				{
					UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
				}
				break;
			}
			x=MRR(((u32)p8rx)+13,2);//�㲥ͨ���ӳ�ʱ����
			//��DL645�㲥Уʱ
			p8tx+=128;
			p8tx[0]=2;//������,0=͸�����䣬1=645-1997��2=645-2007��3=��λʶ��
			//p8tx[1]=22;//���ĳ���
			//MC(0xfe,((u32)p8tx)+2,4);
			p8tx[1]=22-4;//���ĳ���
			p8tx-=4;
			p8tx[6]=0x68;
			MC(0x99,((u32)p8tx)+7,6);
			p8tx[13]=0x68;
			p8tx[14]=0x08;
			p8tx[15]=0x06;
			
			if(UARTCtrl->BroadcastTimeProxyTask==0)//����͸��ת���㲥Уʱ����:0=��,1=�д���
			{
				MR((u32)p8tx+16,ADDR_MYMDHMS,6);
			}
			else
			{//��͸��ת�������·��Ĺ㲥Уʱ
				p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(UARTCtrl->ProxyPORTn);//�õ�UART�������ݻ����ַ
				MR((u32)p8tx+16,(u32)p8Data+10,6);
				for(i=0;i<6;i++)
				{
					p8tx[16+i]-=0x33;
				}
			}
			d64=MRR(((u32)p8tx)+16,6);
			d64=YMDHMS_AddS(d64,x);//������ʱ�����S��,���ؼ�S����������ʱ����
			MWR(d64,((u32)p8tx)+16,6);
			
			for(i=0;i<6;i++)
			{
				p8tx[16+i]+=0x33;
			}
			x=0;
			for(i=0;i<16;i++)
			{
				x+=p8tx[6+i];
			}
			p8tx[22]=x;
			p8tx[23]=0x16;
			//Uart_3762_TxData(PORTn,0x05,3,24);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			Uart_3762_TxData(PORTn,0x05,3,20);//376.2�����������뷢����,���ʱ������Ϣ��Ŀ�ĵ�ַ���ݵ�Ԫ����������,���ݵ�Ԫ�ݹ̶����뷢�ͻ�����+128��ַ
			if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
			{
				Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//���㲥��ʱʱ����(���ֶ���)
			}
			UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0	
			break;
		case 13://����ȷ��֡
			if(Terminal_Router->RouterRunMode_7==0)//·������ģʽ_�㲥����ȷ�Ϸ�ʽ��0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ��ģ�1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
			{//0��ʾ�㲥�����ڱ���ͨ��ģ��ִ�й㲥ͨ�Ź�����Ϻ󷵻�ȷ�ϱ���
				switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					case 1://1=���1֡����
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
							UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
						}
						break;
					case 0://0=����
					case 2://2=���ճ�ʱ(�����)
						if(Terminal_Ram->RouterCheck_S_Timer==0)//���㲥��ʱʱ����(���ֶ���)
						{
							p16timer[0]=0;
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						}
						else
						{
							UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
							UARTCtrl->Task=5;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
							p16timer[0]=1000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
						}
						break;
				}
			}
			else
			{//1��ʾ�㲥�����ڱ����ŵ�ִ�й㲥ͨ��֮ǰ�ͷ���ȷ�ϱ��ģ���Ҫ�������ȴ���ʱ����ȷ�ϱ��ĵ�"�ȴ�ִ��ʱ��"��Ϣ������
				switch(UARTCtrl->Task)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				{
					case 1://1=���1֡����
						if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==0x0001)//UART_3762�������ݵõ�AFN,Fn;B15-B8=AFN,B7-B0=Fn
						{
							//ȡ�ȴ�ʱ��
							i=p8rx[17];
							i+=p8rx[18]<<8;
							i*=1000/10;
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16timer[0]=i;
							UARTCtrl->BroadcastTimeTask++;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							break;
						}
						else
						{
							if(Terminal_Router->ReRouterCtrl<2)//·���������ط�����
							{
								Terminal_Router->ReRouterCtrl++;
								UARTCtrl->BroadcastTimeTask--;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
							else
							{
								UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
							}
						}
						break;
					case 0://0=����
					case 2://2=���ճ�ʱ(�����)
						UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
						break;
				}
			}
			break;
		case 14://�ȴ��㲥��ʱ
			if(p16timer[0]==0)
			{
				UARTCtrl->BroadcastTimeTask=100;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			}
			break;
			
		case 100:
			Terminal_Router->StopFlags&=~(1<<4);//�ָ�·��
			//Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬,5=��ָ�;��ɺ��Զ��ص�0
			UARTCtrl->TaskID=0;
			UARTCtrl->BroadcastTimeTask=0;//�㲥Уʱ0=��,1=,2=,3=����,4=����...,��ɺ��Զ��ص�0
			UARTCtrl->BroadcastTimeProxyTask=0;//����͸��ת���㲥Уʱ����:0=��,1=�д���
			break;
	}
}


#endif





















