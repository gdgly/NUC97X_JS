

//����ת����������-RS485
#include "Project.h"
#include "BatchTask_RS485.h"
#include "../terminal/Terminal_Uart_3761.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "../terminal/Terminal_ReadMeter_DL645.h"
#include "../terminal/Terminal_AFN0E_Event.h"
#include "../terminal/Terminal_AFN0C_RealTimeData_Fn.h"
#include "../Display/Warning.h"


void BatchTask_RS485_SETRTC(u32 PORTn)//����ת����������-RS485-ʱ�Ӷ�ʱ
{
	u32 i;
	u32 x;
	u32 PORT485;
	u64 d64a;
	u64 d64b;
	u64 Random;
	u64 Key;
	
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
#if Extern_BatchTask_RS485==0//�ⲿ����:0=I2,1=E1,2=E2,3=
	if(UARTCtrl->BatchTaskLock==0x55)//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
	{
		return;
	}
	i=MRR(ADDR_AFN05F30+1,1);//�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
	if(i!=1)
	{//0=�رգ�1=����������ֵ�ر�
		if(UARTCtrl->BatchTask>=2)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
		{
			Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<=2)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->Task=0;//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			}
		}
		else
		{
			UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
		}
		return;
	}
#endif

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			PORT485=2;
			break;
		case RS485_2PORT://RS485-2
			PORT485=3;
			break;
		case RS485_3PORT://RS485-3
			PORT485=4;
			break;
		default:
			PORT485=32;
			return;
	}
	switch(UARTCtrl->BatchTask)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
	{
		case 0://��
#if Extern_BatchTask_RS485==0//�ⲿ����:0=I2,1=E1,2=E2,3=
			x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//ȡ��������1(��ʱ����)���õ������n
			if(x==0)
			{//���õ������n=0
				UARTCtrl->BatchMeterCount=0;//ת���������������
				return;
			}
			if(x>MaxRS485AddCarrierPn)
			{//���õ������>MaxRS485AddCarrierPn
				x=MaxRS485AddCarrierPn;
			}
			if(UARTCtrl->BatchMeterCount<x)
			{//ת�������������<���õ������
				UARTCtrl->BatchTask=1;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			}
#endif
			break;
		case 1://�ȴ��ڿ�������
			x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//ȡ��������1(��ʱ����)���õ������n
			if(UARTCtrl->BatchMeterCount>=x)
			{//ת�������������>=���õ������
				UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
				return;
			}
			i=MRR(ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);
			if(i==0)
			{//0=û���,1=���,����ֵ=ִ�д���
				d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
				i=Check_AFN04F10_Addr(PORT485,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(i)
				{//����ͬ��ַ������
					if(UARTCtrl->Lock!=0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
					{
						if(UARTCtrl->Task==0)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						{
							UART7Ctrl->BatchTaskLock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
							UARTCtrl->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
							UARTCtrl->ReRMCount=0;//485���������ط�����
							p16timer[0]=50/10;//��֡����������ʱ50MS��
							UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
						}
					}
					return;
				}
			}
			UARTCtrl->BatchMeterCount++;//ת���������������
			break;
	NEXT://����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
			UARTCtrl->BatchMeterCount++;//ת���������������
			UARTCtrl->BatchTask=1;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			UART7Ctrl->BatchTaskLock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			UARTCtrl->Task=0;//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			return;
		case 2://�Ȳ���̨��ģ�����ʱ
			if(UARTCtrl->BatchMeterCount==0)
			{
				p16timer[0]=6000/10;//��1ֻ��ʼ��ʱ10���,����Ȳ���̨��ģ���
			}
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 3://�Ȳ���̨��ģ�����ʱ
			if(UARTCtrl->BatchMeterCount==0)
			{
				if(p16timer[0]!=0)//��1ֻ��ʼ��ʱ10���,����Ȳ���̨��ģ���
				{
					break;
				}
			}
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 4://���Ͷ����
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			MC(0xfe,((u32)p8txbuff),4);
			Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x04000402,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			UARTCtrl->TxByte=4+16;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
		Tx:
			//ͨ������
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			i=Check_AFN04F10_Addr(PORT485,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
			if(i==0)
			{
				goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
			}
			i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ĭ��2400
				UARTCtrl->BpsCtrl=(3<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			UARTCtrl->Task=3;//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���

			//��ʾ��ʱ��ַ
			CopyString((u8 *)"485x��ʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			MWR((PORT485-1),ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8txbuff[5+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 5://����Ž���
			if(UARTCtrl->Task>=3)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{//��ʱ
				goto ReTx3;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if(i!=0)
			{//����
				if((i==1)||(i==3))
				{//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
					//��������
					p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=���ڽ���
					break;
				}
			ReTx3:
				if(UARTCtrl->ReRMCount<2)//�ط�����
				{//���Ժ����̨��ʱ����ģ���,�����ط������Եȴ�ģ���
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=4+16;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					goto Tx;
				}
				else
				{
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			//��ȷ,������
			MR((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+14,6);
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 6://�����֤����
			//ȡ�ն�TESAM�����
			UARTCtrl->BatchTaskLock=0x55;//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
			i=TESAM_CMD_TxRx(0x801A1000,0,0);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			UARTCtrl->BatchTaskLock=0;//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
			if(i)
			{
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					return;
				}
				else
				{
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			d64a=MRR(ADDR_UART7Rx+5,8);
			d64b=MRR(ADDR_UART7Rx+5+8,8);
			Random=d64a^d64b;
			//ȡ��Կ����
			x=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData,2);//ȡ��������0(�����֤����)���õ������n
			//���ҵ�����֤
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//ȡ��ͨ�ŵ�ַ
			for(i=0;i<x;i++)
			{
				d64b=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i),8);//ȡ��ͨ�ŵ�ַ
				if(d64a==d64b)
				{
					break;
				}
			}
			if(i>=x)
			{//������֤û�ҵ�,����ͨ����֤
				for(i=0;i<x;i++)
				{
					d64b=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i),8);//ȡ��ͨ�ŵ�ַ
					if(d64b==0xaaaaaaaaaaaaaaaa)
					{
						break;
					}
				}
				if(i>=x)
				{//ͨ����֤Ҳû�ҵ�
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			MR((u32)p8rxbuff,ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i)+8,32);//ȡ�����Կ
			Data_Inverse((u32)p8rxbuff,32);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			UARTCtrl->BatchTaskLock=0x55;//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
			if(d64b!=0xaaaaaaaaaaaaaaaa)
			{//����ַ
				x=32+8+8;//����TESAM���ݳ���
				//8BYTE���
				//MWR(d64a,((u32)p8rxbuff)+32,8);//��ͨ�ŵ�ַ
				MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+32,6);//���
				MWR(0,((u32)p8rxbuff)+32+6,2);
				Data_Inverse(((u32)p8rxbuff)+32,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//8BYTE�����
				MWR(Random,((u32)p8rxbuff)+32+8,8);
				i=TESAM_CMD_TxRx(0x841C0001,x,(u32)p8rxbuff);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			}
			else
			{//ͨ�õ�ַ
				x=32+2+6+2+6+8;//����TESAM���ݳ���
				//2BYTE��������
				MR(((u32)p8rxbuff)+32,ADDR_AFN10F12+5,2);
				Data_Inverse(((u32)p8rxbuff)+32,2);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//6BYTE���
				//MWR(d64a,((u32)p8rxbuff)+32+2,6);
				MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+32+2,6);//���
				Data_Inverse(((u32)p8rxbuff)+32+2,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//2BYTE 0000
				MWR(0,((u32)p8rxbuff)+32+8,2);
				//6BYTE���
				//MWR(d64a,((u32)p8rxbuff)+32+10,6);
				MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8rxbuff)+32+10,6);//���
				Data_Inverse(((u32)p8rxbuff)+32+10,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
				//8BYTE�����
				MWR(Random,((u32)p8rxbuff)+32+16,8);
				i=TESAM_CMD_TxRx(0x841C0002,x,(u32)p8rxbuff);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			}
			UARTCtrl->BatchTaskLock=0;//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
			if(i)
			{
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					return;
				}
				else
				{
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			Key=MRR(ADDR_UART7Rx+5,8);
			d64b=MRR(ADDR_UART7Rx+5+8,8);
			Key^=d64b;//����
			//645����
			MC(0xfe,(u32)p8txbuff,4);
			MWR(d64a,((u32)p8txbuff)+4+1,6);
			p8txbuff[4+8]=0x03;//������
			p8txbuff[4+9]=4+4+8+8+8;//�����򳤶�
			//FF000007 78563412 77FDC349F9A7F0FD d29847bb16a776d2 0100785634120000
			MWR(0x070000FF,((u32)p8txbuff)+4+10,4);//���ݱ�ʶ
			MWR(0x12345678,((u32)p8txbuff)+4+10+4,4);//�����ߴ���
			MWR(Key,((u32)p8txbuff)+4+10+8,8);//����
			Data_Inverse(((u32)p8txbuff)+4+10+8,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			MWR(Random,((u32)p8txbuff)+4+10+16,8);//�����
			Data_Inverse(((u32)p8txbuff)+4+10+16,8);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			//MWR(d64a,((u32)p8txbuff)+4+10+24,8);//�õ�ַ���ķ�ɢ����
			MW((u32)&UARTCtrl->BatchMeterNo,((u32)p8txbuff)+4+10+24,6);//��ɢ����(���)
			MWR(0,((u32)p8txbuff)+4+10+30,2);
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
			UARTCtrl->TxByte=4+10+32+2;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=10000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			goto Tx;
		case 7://	�����֤����
			if(UARTCtrl->Task>=3)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			//�ҵ�ǰ��ʱ�����������������֤���������,����ִ�н����¼
			x=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData,2);//ȡ��������0(�����֤����)���õ������n
			if(x>MaxRS485AddCarrierPn)
			{//���õ������>MaxRS485AddCarrierPn
				x=MaxRS485AddCarrierPn;
			}
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//ȡ��ͨ�ŵ�ַ
			for(i=0;i<x;i++)
			{
				d64b=MRR(ADDR_AFN10F12+5+LEN_AFN10F12TaskData+2+(40*i),8);//ȡ��ͨ�ŵ�ַ
				if(d64a==d64b)
				{
					break;
				}
			}
			if(i>=x)
			{//û�ҵ�
				x=0xffffffff;
			}
			else
			{
				x=i;
			}
			//
			if(UARTCtrl->Task!=1)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{//��ʱ
				goto ReTx7;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if(i!=0)
			{//����
				if((i==1)||(i==3))
				{//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
					//��������
					p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=���ڽ���
					break;
				}
			ReTx7:
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=4+10+32+2;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=10000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					goto Tx;
				}
				else
				{
					if(x!=0xffffffff)
					{//�ҵ���ǰ��ʱ�����������������֤���������
						MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+x,1);//��ʱ��ɱ�־:0=û���,1=���,����ֵ=ִ�д���
					}
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			if(x!=0xffffffff)
			{//�ҵ���ǰ��ʱ�����������������֤���������
				MWR(1,ADDR_AFN10F13+x,1);//�����֤��ɱ�־
			}
			MR((u32)&UARTCtrl->Random,((u32)p8rxbuff)+14,4);//��������֤ʱ���ص������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 8://��ʱ����
			p8txbuff[0]=1;//��������
			MWR(0x050032000507,((u32)p8txbuff)+1,6);
			Data_Inverse(((u32)p8txbuff)+1,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			MR(((u32)p8txbuff)+7,ADDR_TYMDHMS,3);//�ն˵�ǰʱ��YMDWHMS
			MR(((u32)p8txbuff)+7+3,ADDR_TYMDHMS+6,1);//�ն˵�ǰʱ��YMDWHMS
			MR(((u32)p8txbuff)+7+4,ADDR_TYMDHMS+3,3);//�ն˵�ǰʱ��YMDWHMS
			Data_Inverse(((u32)p8txbuff)+7,7);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			MR(((u32)p8txbuff)+14,ADDR_AFN10F12+LEN_AFN10F12PerTask+5,40);//��������
			Data_Inverse(((u32)p8txbuff)+14,40);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			MR(((u32)p8txbuff)+54,ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount)+8,32);//��ʱ�����е����Կ����
			Data_Inverse(((u32)p8txbuff)+54,32);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			MR(((u32)p8txbuff)+86,(u32)&UARTCtrl->Random,4);//��������֤ʱ���ص������
			Data_Inverse(((u32)p8txbuff)+86,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			
			UARTCtrl->BatchTaskLock=0x55;//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
			i=TESAM_CMD_TxRx(0x84180404,0x5A,(u32)p8txbuff);//����ESAM����;����:0=��ȷ�����ҷ���9000,1=����
			UARTCtrl->BatchTaskLock=0;//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
			if(i)
			{
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					return;
				}
				else
				{
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			//645����
			MC(0xfe,(u32)p8txbuff,4);
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//ȡ��ͨ�ŵ�ַ
			MWR(d64a,((u32)p8txbuff)+4+1,6);
			p8txbuff[4+8]=0x14;//������
			p8txbuff[4+9]=32;//�����򳤶�
			//0C 01 00 04�����ݱ�ʶ�� 98(PA) 000000(P0P1P2) 78563412(�����ߴ��뵹��) +8B 29 73 C3 09 2F 39 9B 29 18 83 4F 04 74 AD 5D������ʱ�������ĵ�� + 03031C8A��MAC���
			MWR(0x0400010C,((u32)p8txbuff)+4+10,4);//���ݱ�ʶ
			MWR(0x00000098,((u32)p8txbuff)+4+10+4,4);//��Կ�ȼ�
			MWR(0x12345678,((u32)p8txbuff)+4+10+8,4);//�����ߴ���
			MR(((u32)p8txbuff)+4+10+12,ADDR_UART7Rx+5+4,16);//����ʱ��������
			Data_Inverse(((u32)p8txbuff)+4+10+12,16);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			MR(((u32)p8txbuff)+4+10+28,ADDR_UART7Rx+5+4+16,4);//MAC
			Data_Inverse(((u32)p8txbuff)+4+10+28,4);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
			UARTCtrl->TxByte=4+10+32+2;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=10000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			goto Tx;
		case 9://��ʱ����
			if(UARTCtrl->Task>=3)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{//��ʱ
				goto ReTx9;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if((i!=0)&&(i!=3))
			{//����
			ReTx9:
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=4+10+32+2;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=10000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					goto Tx;
				}
				else
				{
					MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//��ʱ��ɱ�־:0=û���,1=���,����ֵ=ִ�д���
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			MWR(1,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//��ʱ��ɱ�־:0=û���,1=���,����ֵ=ִ�д���
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			p16timer[0]=1000/10;//��ʱ�ȴ��������Уʱ��¼
			break;
		case 10://��ʱ�ȴ��������Уʱ��¼
			if(p16timer[0]==0)
			{
				UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			}
			break;
		case 11://����1��Уʱ��¼
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			MC(0xfe,((u32)p8txbuff),4);
			Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x03300401,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			UARTCtrl->TxByte=4+16;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			goto Tx;
		case 12://����1��Уʱ��¼����
			if(UARTCtrl->Task>=3)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{//��ʱ
				goto ReTx12;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if(i!=0)
			{//����
				if((i==1)||(i==3))
				{//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
					//��������
					p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=���ڽ���
					break;
				}
			ReTx12:
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=4+16;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					goto Tx;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14+4,12);//��ʱǰ����ʱ��Ϊ��
				}
			}
			//1������F166 ��ʱǰʱ���ʱ��ʱ��
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			i=Check_AFN04F10_Addr(PORT485,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
			if(i)
			{//����ͬ��ַ������
				x=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+2,2);//�������
				i=GetClass1DataAddr(166,x);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
				if(i)
				{
					MR(i+8,((u32)p8rxbuff)+14+4,12);//��ʱǰʱ��,��ʱ��ʱ��
				}
			}
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 13://�����Уʱ�ܴ���
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			MC(0xfe,((u32)p8txbuff),4);
			Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x03300400,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			UARTCtrl->TxByte=4+16;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			goto Tx;
		case 14://�����Уʱ�ܴ�������
			if(UARTCtrl->Task>=3)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				Uart_DL645_RxTx(PORTn);//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
				break;
			}
			if(UARTCtrl->Task!=1)//UART_DL645���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{//��ʱ
				goto ReTx14;
			}
			i=DL645_2007_RxData(PORTn);//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������,3���ݱ�ʶ��
			if(i!=0)
			{//����
				if((i==1)||(i==3))
				{//��ַ�����ݱ�ʶ��,���Ժ�������ģ����ֹͣ��������������,�ָ�ģ����Ὣ�յ���֡ͬʱ����
					//��������
					p16timer[0]=ReadDL645OverTimer/10;//DL645����ʱ��ʱֵms
					UARTCtrl->RxByte=0;
					UARTCtrl->Task=5;//5=���ڽ���
					break;
				}
			ReTx14:
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=4+16;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=ReadDL645OverTimer/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
					goto Tx;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14,3);//���Уʱ�ܴ���Ϊ��
				}
			}
			//1������F166 ���Уʱ�ܴ���
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			i=Check_AFN04F10_Addr(PORT485,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
			if(i)
			{//����ͬ��ַ������
				x=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+2,2);//�������
				i=GetClass1DataAddr(166,x);//ȡ��1���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
				if(i)
				{
					MR(i+5,((u32)p8rxbuff)+14,3);//���Уʱ�ܴ���
					MR(i,ADDR_TYMDHMS+1,5);//�޸��ն˳���ʱ��
					//ERC41��ʱ�¼�
					MWR(x,ADDR_DATABUFF+2,2);
					MR(ADDR_DATABUFF+4,i+8,12);//��ʱǰʱ��,��ʱ��ʱ��
					ERC41_Event(ADDR_DATABUFF);//��ʱ�¼���¼;���:ADDR_BUFF�ĵ�2�ֽڿ�ʼ�ֱ�Ϊ2BYTE�������,6BYTE��ʱǰʱ��,6BYTE��ʱ��ʱ��
				}
			}		
			goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
		default:
			goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
	}
}











