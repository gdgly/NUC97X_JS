

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_3762_iActive.h"
#include "../DL698/Uart_3762_RxTx.h"
#include "../DL698/RM_RS485.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Display/Warning.h"
#include "../Calculate/Calculate.h"


#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
void RM_3762_iActive(u32 PORTn)//��������������
{
	u32 i;
	u32 y;
	u32 Info;
	u32 f;
  UARTCtrl_TypeDef * UARTCtrl;
	u8* p8rx;
	u8* p8tx;
	u16* p16timer;
	u32 SetNo;
#if SEL_RouterRunMode3ADDR==1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
	u8* pCSD;
	u64 d64a;
#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���,6=������(ͬRS485��3=��������)
	{
		case 0://0=����
			UARTCtrl->Task=6;//3;//��������
			break;
		case 2://2=���ճ�ʱ(�����)
			UARTCtrl->Task=0;//����1��
			break;
		case 3://3=��������
			break;
		case 6://6=������(ͬRS485��3=��������)
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			i=p16timer[0];
			if(i!=0)
			{
				if(i>(30/10))//��֡����������ʱ
				{
					p16timer[0]=30/10;
				}
				break;
			}
			p16timer[0]=30/10;//��֡����������ʱ
			UARTCtrl->RMMeterCountAuto=0;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
			i=UARTCtrl->TaskID;
			if(i==0)
			{
				p16timer[0]=1000/10;//�´μ���TASK()��1���,�����Ӱ��˵��ٶ�
				UARTCtrl->Task=5;//���ڽ���
				RM_NUM_Display(PORTn);//����������ʾ
				WarningOccur(PORTn,(u8 *)"�ز� ��ǰ�������");//�澯����,��ڸ澯����͸澯�ַ���
				if(UARTCtrl->RTCS_TASK!=Comm_Ram->DL698YMDHMS[6])//ʵʱ�Ӹ��±�־ԭʼ��(Task����)
				{
					UARTCtrl->RTCS_TASK=Comm_Ram->DL698YMDHMS[6];//ʵʱ�Ӹ��±�־ԭʼ��(Task����)
					TASK(PORTn,1);//�ɼ�����;TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
				}
				return;
			}
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			if(UARTCtrl->ReRMCount>=2)//�����ط�����
			{
				RM_OK_FAIL(PORTn,0);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�
				UARTCtrl-> ReRMCount=0;//�����ط�����
				UARTCtrl->RM_OKFAIL=0;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
				//�µ��ܱ�
				RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
				TASK(PORTn,1);//�ɼ�����;TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
				return;
			}
			if(UARTCtrl->RM_SubsequentFrames==0)//0=�޶�����֡,1-n=����֡��֡��
			{//�Ǻ���֡
				UARTCtrl->RMMeterCountAuto=0;//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
				i=RM_TxDATA_Manage(PORTn);//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				//ע:RM_TxDATA_Manage(PORTn)����UARTCtrl-> ReRMCount=0
				UARTCtrl-> ReRMCount++;//�����ط�����
				if(UARTCtrl->TxByte==0)
				{
					i=UARTCtrl->RM_DI_TI_Count;//����
					TASK(PORTn,1);//�ɼ�����;TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
					if(UARTCtrl->CONNECT!=0)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
					{
						UARTCtrl->RM_DI_TI_Count=i;//�ָ�
					}
					return;
				}
				if(UARTCtrl->RMprotocol==3)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
				{
					i=100;//Ԥ��Ӧ���ֽ���
				}
				else
				{
					i=0;//Ԥ��Ӧ���ֽ���
				}
			#if (USER/100)==0//���Ժ������
				Uart_3762_AFN13F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
			#else
				if(Comm_Ram->RouterType==2)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��
				{//������698����͸��
					if(UARTCtrl->RMprotocol==3)
					{
						Uart_3762_AFN13F1_Tx(PORTn,0,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
					}
					else
					{
						Uart_3762_AFN13F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
					}
				}
				else
				{
					Uart_3762_AFN13F1_Tx(PORTn,UARTCtrl->RMprotocol,(u32)p8tx+128,UARTCtrl->TxByte,i,0);//376.2��شӽڵ��������뷢����;���:�˿�,ͨ��Э��(1=1997,2=2007),��׼645֡��ַ,645֡����,Ԥ��Ӧ���ֽ���,ͨ����ʱ��ر�־
				}
			#endif
			#if SEL_RouterRunMode3ADDR==1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
				if(Terminal_Router->RouterRunMode_1==3)//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
				{//΢��������,Ŀ���ַ�òɼ�����ַ
					i=UARTCtrl->TaskID;
					i>>=8;
					i&=0xff;
					switch(i)
					{
						case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
							i=UARTCtrl->MeterCount[i];
							i=PowerEvent->setno[i];
							break;
						default:
							i=UARTCtrl->MeterCount[i];
							break;
					}
					if(i>=NUM_RMmax)
					{
						return;
					}
					d64a=0;
					pCSD=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
					pCSD=Get_Element(pCSD,3,0,0);//����Ԫ�ص�ַ
					if(pCSD)
					{//�ɼ�����ȡ
						d64a=MRR((u32)pCSD+5,6);//ȡ���òɼ�����ַ(Ŀ�ĵ�ַ)
						d64a=R_Inverse(d64a,6);
					}
					if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
					{
						MWR(d64a,((u32)p8tx)+16,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
						Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
					}
				}
			#endif
			}
			else
			{//�Ǻ���֡
				UARTCtrl-> ReRMCount++;//�����ط�����
				//p8tx[0]=0x68;
				p8tx[1]=(29+17+2);//���ܷ����ֽ���
				p8tx[2]=(29+17+2)>>8;
				//p8tx[3]=0x41;//������Ϊ0x41
				//p8tx[4]=4;//ͨ��ģ���ʶ=1
				//p8tx[5]=0;
				p8tx[6]=0;//Ԥ��Ӧ���ֽ���
				//p8tx[7]=0;
				//p8tx[8]=0;
				UARTCtrl->TxSEQ++;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
				p8tx[9]=UARTCtrl->TxSEQ;//����֡������SEQ(ÿ�μ�1����(�Ƿ��ͺ��1),�ȴ���Ӧʱ����,�Ա�Ƚ�)
				//MR(((u32)p8tx)+10,(u32)&Terminal_Router->RouterAddr,6);//���ڵ��ַ(Դ��ַ)
				//MR(((u32)p8tx)+16,ADDR_Data+1,6);//�ӽڵ��ַ(Ŀ�ĵ�ַ)
				//p8tx[22]=0x13;//AFN=13H
				//p8tx[23]=1;//Fn=1
				//p8tx[24]=0;
				//p8tx[25]=2;//ת�����ݱ�ʶ����=0͸������,1=DL645-1997,2=DL645-2007
				//p8tx[26]=0;//ͨ����ʱ��ر�־
				//p8tx[27]=0;//�����ڵ���=0
				p8tx[28]=17;//DL645���ĳ���
				p8tx[29+8]=0x12;
				p8tx[29+9]=5;
				p8tx[29+14]=UARTCtrl->RM_SubsequentFrames+0x33;
				p8tx[29+15]=0;//cs
				for(i=0;i<15;i++)
				{
					p8tx[29+15]+=p8tx[29+i];
				}
				p8tx[29+16]=0x16;
				Uart_3762_CS((u32)p8tx);//��У����㲢д��(ͬʱд�����0x16)�ͱȽ�,����0=��ȷ1=����,��ڽ��ջ��ͻ���
				UARTCtrl->TxByte=31+p8tx[28];//�����ֽ���(���ֶ���)
			}
			if(UARTCtrl->TxByte==0)
			{
				return;
			}
			UARTCtrl->TxByte=31+p8tx[28];//�����ֽ���(���ֶ���)
			i=Terminal_Router->MaxOverTime*(1000/10);//�ӽڵ������ʱʱ����
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;//��������
			RM_ADDR_Display(PORTn);//�����ַ��ʾ
			i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
			i>>=8;
			i&=0xff;
			RM_Monitor_TxNUM(i,1);//�ɼ�������;���:ID=�����,NUM_Tx=��������
			break;
		case 1://1=�������
			i=UARTCtrl->TaskID;//b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
			i>>=8;
			i&=0xff;
			RM_Monitor_RxNUM(i,1);//�ɼ�������;���:ID=�����,NUM_Rx=��������
			SetNo=UARTCtrl->MeterCount[i];//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
			p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
			p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
			p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			p16timer[0]=30/10;//��֡����������ʱ20MS��
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//�ز�·���������붨ʱֵ
		
			UARTCtrl->Task=0;//����1��
			i=p8rx[1]+(p8rx[2]<<8);
			if(i<=16)
			{//���ϻش�
				return;
			}
			i=p8rx[22]+(p8rx[23]<<8)+(p8rx[24]<<16);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
			#if (USER/100)==0//���Ժ����
				p16timer[0]=1000/10;
			#else
				p16timer[0]=30000/10;
			#endif
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=���ڽ���
				return;
			}
			y=p8rx[28];//���������򳤶�
			if(y==0)
			{//�������ݳ���=0
				return;
			}
			i=p8rx[25]+(p8rx[26]<<8);//��·�ϴ�����ʱʱ����
			UARTCtrl->TransferDelay=i;
			
			Info=p8rx[4]+(p8rx[5]<<8)+(p8rx[6]<<16)+(p8rx[7]<<24);
			f=p8rx[8];//����ģ�����ݴ������ϱ���־
			f=f;
			MR((u32)p8rx,((u32)p8rx)+28+1,y);
			for(i=0;i<y;i++)
			{//ȥ����֡��ǰ��0xfe
				if(p8rx[i]!=0xfe)
				{
					if(i!=0)
					{
						MR((u32)p8rx,((u32)p8rx)+i,y-i);
					}
					break;
				}
			}
			i=RM_RxDATA_CHECK(PORTn);//֡�������ݼ��,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			if((i==1)||(i==3))
			{//1��ַ��,3���ݱ�ʶ��
			#if (USER/100)==0//���Ժ����
				UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
			#else
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=30000/10;
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=���ڽ���
			#endif
				return;
			}
			if(i==4)
			{//4�����ϱ�(�Ѵ���)
			#if (USER/100)==0//���Ժ����
				
			#else
				//��������
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=30000/10;
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=���ڽ���
			#endif
				return;
			}
			UARTCtrl->RM_Rx_date_NULL=i;//������շ���:0=������,!=0������(������)
			if(i==2)
			{//2�������(������)
//				UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
//				UARTCtrl->ReRMCount=0;//�����ط�����
//				return;
			#if (USER/100)==15//����
				//����:����������Ҫ֧��06000001��061000FF�ȿ鳭��
				if(UARTCtrl->RM_DI==0x06000001)
				{
					UARTCtrl->Task=2;//2=���ճ�ʱ(�����)
					return;
				}
			#endif
				//���ߵȶ������ݵ�ָ��ʱ��������ݶ���ʱд������ݱ��ⲹ��ʱ���³���
				i=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
				Info=(UARTCtrl->TaskID>>20)&0xf;//�ɼ�����
				if(!((i==0x5002)||(Info==4)))
				{//���ǳ����Ӷ���(����������)�Ͳ���(2=�¼��ɼ�����ʱ����������ݶ��ϱ�)
					UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
					UARTCtrl->ReRMCount=0;//�����ط�����
					return;
				}
			}
			RM_RxDATA(PORTn);//����������ݴ���
			RM_State_RMUpDate(PORTn,1,SetNo,(Info>>4)&0xf,(Info>>16)&0xf);//�ɼ�״̬,ÿ�γ�������:���:OK=0ʧ��=1�ɹ�,OK=1ʱClass=�м̼���,Phase=��λ
			
			UARTCtrl->ReRMCount=0;//�����ط�����
			UARTCtrl->RMCSDSubCount++;//����CSD�Ӽ���
	}
}

#endif


