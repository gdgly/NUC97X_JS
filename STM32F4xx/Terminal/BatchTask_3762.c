
//����ת����������-�ز�3762
#include "Project.h"
#include "BatchTask_3762.h"
#include "Terminal_Uart_3761.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Calculate/Calculate.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"


#if (Project/100)!=2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=�ɼ���,600=ͨ��ģ��
void BatchTask_3762_SETRTC(u32 PORTn)//����ת����������-3762-ʱ�Ӷ�ʱ
{
}
#else
void BatchTask_3762_SETRTC(u32 PORTn)//����ת����������-3762-ʱ�Ӷ�ʱ
{
	u32 i;
	u32 x;
	u64 d64a;
	u64 d64b;
	u64 Random;
	u64 Key;
	
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

#if Extern_BatchTask_3762==0//�ⲿ����:0=I2,1=E1,2=E2,3=
	if(UARTCtrl->BatchTaskLock==0x55)//0x55=ת�������������ڱ����ý�ֹ���������ٴε���
	{
		return;
	}
	i=MRR(ADDR_AFN05F30+1,1);//�ն˶Ե��ܱ��ʱ���ܵĿ�����ر�
	if(i!=1)
	{//0=�رգ�1=����������ֵ�ر�
		if(UARTCtrl->BatchTask>=2)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
		{
			Uart_DL645_RxTx(PORTn);//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			if(UARTCtrl->Task<=2)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
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
	
	switch(UARTCtrl->BatchTask)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
	{
		case 0://��
		
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
		
			break;
		case 1://�ȴ��ڿ�������
			x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//ȡ��������1(��ʱ����)���õ������n
			if(UARTCtrl->BatchMeterCount>=x)
			{//ת�������������>=���õ������				
				UARTCtrl->BatchTask=0;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
				Terminal_Router->StopFlags&=~(1<<9);//�ָ�·��
				Terminal_Router->RouterCtrl=3;//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=��״̬;��ɺ��Զ��ص�0
				
				return;
			}
			i=MRR(ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);
			if(i==0)
			{//0=û���,1=���,����ֵ=ִ�д���
				d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
				i=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(i)
				{//����ͬ��ַ������
					if(UARTCtrl->Lock!=0x55)//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
					{
						if(UARTCtrl->Task==0)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
						{
							Terminal_Router->StopFlags|=(1<<9);//��ͣ·��
							Terminal_Router->RouterCtrl=2;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
							UART7Ctrl->BatchTaskLock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
							UARTCtrl->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
							UARTCtrl->ReRMCount=0;//485���������ط�����
							UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
						}
					}
					return;
				}
			}
			UARTCtrl->BatchMeterCount++;//ת���������������
			break;
	NEXT://����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
			UARTCtrl->BatchTask=1;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			UART7Ctrl->BatchTaskLock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			UARTCtrl->Task=0;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->BatchMeterCount++;//ת���������������
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
			Read_DL645_2007_TxData(ADDR_DATABUFF,0x04000402,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,6,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{//΢��������,������ַ�òɼ�����ַ
				x=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			}
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			
			UARTCtrl->TxByte=(31+16);//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			//��ʾ��ʱ��ַ
			CopyString((u8 *)"�ز���ʱ",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[30+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 5://����Ž���
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
			if(i!=0)
			{//����
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=(31+16);//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
					UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					return;
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
			MR(ADDR_DATABUFF,((u32)p8txbuff)+4,10+32+2);
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,10+32+2,16+12,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{//΢��������,������ַ�òɼ�����ַ
				x=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			}
			
			UARTCtrl->TxByte=31+10+32+2;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 7://	�����֤����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
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
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
			if(i!=0)
			{//����
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=31+10+32+2;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
					UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					return;
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
		case 8://Ԥ���ӳ�ʱ��
			//645����
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),8);//ȡ��ͨ�ŵ�ַ
			MWR(d64a,((u32)p8txbuff)+1,6);
			p8txbuff[8]=0x14;//������
			p8txbuff[9]=32;//�����򳤶�
			//0C 01 00 04�����ݱ�ʶ�� 98(PA) 000000(P0P1P2) 78563412(�����ߴ��뵹��) +8B 29 73 C3 09 2F 39 9B 29 18 83 4F 04 74 AD 5D������ʱ�������ĵ�� + 03031C8A��MAC���
			MWR(0x0400010C,((u32)p8txbuff)+10,4);//���ݱ�ʶ
			MWR(0x00000098,((u32)p8txbuff)+10+4,4);//��Կ�ȼ�
			MWR(0x12345678,((u32)p8txbuff)+10+8,4);//�����ߴ���
			MR(((u32)p8txbuff)+10+12,ADDR_UART7Rx+5+4,16);//����ʱ��������
			MR(((u32)p8txbuff)+10+28,ADDR_UART7Rx+5+4+16,4);//MAC
			DL645_68H_Add33H_CS_16H(((u32)p8txbuff));//DL465֡��ʼ��0x68,�������0x33,CS,��β0x16
			MR(ADDR_DATABUFF,((u32)p8txbuff),10+32+2);
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,10+32+2,12,1);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{//΢��������,������ַ�òɼ�����ַ
				x=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			}
			
			UARTCtrl->TxByte=31+10+32+2;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 9://Ԥ���ӳ�ʱ�����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=MRR(((u32)p8rxbuff)+10,3);
			if((i!=0x414)||(UARTCtrl->Task!=1))
			{//AFN=14H,Fn=3
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=31+10+32+2;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
					UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					return;
				}
				else
				{
					MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//��ʱ��ɱ�־:0=û���,1=���,����ֵ=ִ�д���
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+19,2);//Ԥ���ӳ�ʱ����
			MR((u32)p8txbuff,(u32)p8rxbuff,10);//����10�ֽڽ���ͷ������ͷ
			
			p8txbuff[3]=1;//������Ϊ0x1
			p8txbuff[4]=0;//ͨ��ģ���ʶ=0
			p8txbuff[10]=0x14;//AFN=0x14
			p8txbuff[11]=0x04;//Fn=3
			p8txbuff[12]=0x00;//Fn=3
			p8txbuff[13]=0;//���ݳ���=0,����������
			i=14+2;//���ܷ����ֽ���
			p8txbuff[1]=i;
			p8txbuff[2]=i>>8;
			UARTCtrl->TxByte=i;//�����ֽ���(���ֶ���)
			Uart_3762_CS((u32)p8txbuff);//��У����㲢д��ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���

			UARTCtrl->TxToRxTimerOver=6000/10;//���ͺ���ճ�ʱ��ʱֵms/10(���ֶ���)
			UARTCtrl->Task=3;//�ȴ�����
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 10:
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			//��ʱ����
			p8txbuff[0]=1;//��������
			MWR(0x050032000507,((u32)p8txbuff)+1,6);
			Data_Inverse(((u32)p8txbuff)+1,6);//���ݵ���(�ߵ��ֽڵ���),���Le>=2
			//�ն˵�ǰʱ��YMDWHMS+Ԥ���ӳ�ʱ����
			d64a=MRR(ADDR_TYMDHMS,6);
			d64a=YMDHMS_AddS(d64a,UARTCtrl->TransferDelay);//������ʱ�����S��,���ؼ�S����������ʱ����
			MWR(d64a,((u32)p8txbuff)+7,3);
			MWR(d64a>>24,((u32)p8txbuff)+7+4,3);
			//���ڼ���
			YMDCalculateW(((u32)p8txbuff)+7);//ָ����ַ�����е���������ʱ���������
			
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
			MR(ADDR_DATABUFF,((u32)p8txbuff)+4,10+32+2);
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,10+32+2,12,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{//΢��������,������ַ�òɼ�����ַ
				x=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			}

			UARTCtrl->TxByte=31+10+32+2;//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			break;
		case 11://��ʱ����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
			if(i!=0)
			{//����
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=31+10+32+2;//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
					UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					return;
				}
				else
				{
					MWR(2,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//��ʱ��ɱ�־:0=û���,1=���,����ֵ=ִ�д���
					goto NEXT;//����1���Ա���վ��������,Ȼ��ȴ���ֻ���ʱ
				}
			}
			//��ȷ
			MWR(1,ADDR_AFN10F13+MaxRS485AddCarrierPn+UARTCtrl->BatchMeterCount,1);//��ʱ��ɱ�־:0=û���,1=���,����ֵ=ִ�д���
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			//��ʱ�ȴ��������Уʱ��¼
			Terminal_Ram->Trip4_MS_Timer=1000/10;//6 �ն���բ4 10MS��ʱ��;1200ms,>=1000��,<1000��
			break;
		case 12://��ʱ�ȴ��������Уʱ��¼
			if(Terminal_Ram->Trip4_MS_Timer==0)//6 �ն���բ4 10MS��ʱ��;1200ms,>=1000��,<1000��
			{
				UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			}
			break;
		case 13://����1��Уʱ��¼
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			Read_DL645_2007_TxData(ADDR_DATABUFF,0x03300401,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,23,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{//΢��������,������ַ�òɼ�����ַ
				x=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			}
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			
			UARTCtrl->TxByte=(31+16);//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			break;
		case 14://����1��Уʱ��¼����
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
			if(i!=0)
			{//����
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=(31+16);//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
					UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					return;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14+4,12);//��ʱǰ����ʱ��Ϊ��
				}
			}
			//1������F166 ��ʱǰʱ���ʱ��ʱ��
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			i=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
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
		case 15://�����Уʱ�ܴ���
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			Read_DL645_2007_TxData(ADDR_DATABUFF,0x03300400,d64a);//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,23,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
			{//΢��������,������ַ�òɼ�����ַ
				x=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
				if(x!=0)
				{
					d64a=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//�ɼ�����ַ(Ŀ�ĵ�ַ)
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8txbuff)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8txbuff);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			}
			UARTCtrl->BatchTask++;//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			
			UARTCtrl->TxByte=(31+16);//�����ֽ���(���ֶ���)
			UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			break;
		case 16://�����Уʱ�ܴ�������
			if(UARTCtrl->Task>=3)//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
			{
				break;
			}
			i=Uart_3762_AFN13F1_Rx(PORTn);//376.2��شӽڵ��������֡ȡ����׼645֡;����0��ȷ,1��ַ��,2�������,3,4֡3762��,5��ʱ,255������
			if(i!=0)
			{//����
				if(UARTCtrl->ReRMCount<1)//�ط�����
				{
					UARTCtrl->ReRMCount++;//�ط�����+1
					UARTCtrl->TxByte=(31+16);//�����ֽ���(���ֶ���)
					UARTCtrl->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
					UARTCtrl->Task=3;//UART_3762���շ���;0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					return;
				}
				else
				{
					MC(0xee,((u32)p8rxbuff)+14,3);//���Уʱ�ܴ���Ϊ��
				}
			}
			//1������F166 ���Уʱ�ܴ���
			d64a=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData+2+(40*UARTCtrl->BatchMeterCount),6);//ȡ��ͨ�ŵ�ַ
			i=Check_AFN04F10_Addr(31,d64a);//����ն˵��ܱ�/��������װ�����õ�ͨ�ŵ�ַ;����0����ͬ,!=0��ͬ��ַ���������(1��ʼ)
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

#endif










