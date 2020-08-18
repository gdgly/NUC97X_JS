

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../DL698/DL698_Uart.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"


#include "../DL698/DL698_JSON.h"



u32 RM_TxDATA(u32 PORTn,u8 *p8CSD,u8 *p8tx)//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
{
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	{
		case 1://DL/T645-1997
			return RM_TxDATA_DL1997(PORTn,p8CSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
		case 2://DL/T645-2007
			return RM_TxDATA_DL2007(PORTn,p8CSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
		case 3://DL/T698.45
			return RM_TxDATA_DL698(PORTn,p8CSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
		case 4://CJ/T 188-2004
			return 0;//RM_TxDATA_CJ188(PORTn,p8CSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
		case ProtocolNO_ABB://ABB
			return RM_TxDATA_ABB(PORTn,p8CSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
		case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
			return RM_TxDATA_Landis(PORTn,p8CSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���
		default://δ֪ (0)
			return 0;//����:��֡���ܳ����ֽ���

	}
}

u32 RM_TxDATA_Manage(u32 PORTn)//����������֡����;����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
{
	u32 i;
	u32 x;
	u32 y;
	u8* p8tx;
	u8* p8rx;
	u16* p16;
	u8* pCSD;
	UARTCtrl_TypeDef * UARTCtrl;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	u16* p16timer;
#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
	u32 n;
	u32 NUM_SomeTaskID;
#endif
	u32 SetNo;
	
	SetNo=SetNo;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);
//	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	UARTCtrl->Task=0;//����1��
	UARTCtrl->TxByte=0;
	
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;//�����
	SetNo=UARTCtrl->MeterCount[i];

	switch(UARTCtrl->CONNECT)//DL698����:0=��,1=��������,2=��100=����,101=������,102=�ر�����
	{
		default://698��Լ���Ӻ͹رչ���
			UARTCtrl->CONNECT=0;
			return 4;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
		case 1://���ӹ���1
		case 2://���ӹ���2
			switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
			{
				default:
				case 1://DL/T645-1997
				case 2://DL/T645-2007
					UARTCtrl->CONNECT=100;
					return 4;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case 3://DL/T698.45
					UARTCtrl->CONNECT=100;
					return 4;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case 4://CJ/T 188-2004
					UARTCtrl->CONNECT=100;
					return 4;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case ProtocolNO_ABB://ABB
					x=RM_TxDATA(PORTn,pCSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
					if(x)
					{
						UARTCtrl->TxByte=x;//�����ֽ���
						return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
					}
					return 0;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
					x=RM_TxDATA(PORTn,pCSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
					if(x)
					{
						UARTCtrl->TxByte=x;//�����ֽ���
						return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
					}
					return 0;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
			}
		case 0://0=��
			i=UARTCtrl->TaskID;
			if(i==0)
			{
				if(PORTn!=ACSAMPLEPORT)
				{//���ǽ���
					RM_NUM_Display(PORTn);//����������ʾ
					if(PORTn==RS485_4PORT)
					{
						WarningOccur(PORTn,(u8 *)"�ز� ��ǰ�������");//�澯����,��ڸ澯����͸澯�ַ���
					}
				}
		#if DEBUG==1//0=����,1=����
				else
				{//�ǽ���
					RM_NUM_Display(PORTn);//����������ʾ
				}
		#endif
				return 4;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
			}
			i>>=8;
			i&=0xff;
			if(i>=(RMTASKmax+ExtRMTASKmax))
			{
				return 4;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
			}
			if(PORTn==RS485_4PORT)
			{//�ز�
				RM_NUM_Display(PORTn);//����������ʾ
			}
			for(x=0;x<100;x++)
			{
			#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
				if(UARTCtrl->RMMeterCountAuto==1)//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
				{
					if(SetNo>=NUM_RMmax)
					{//��
						return 3;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
					}
					NUM_SomeTaskID=UARTCtrl->NUM_SomeTaskID;//��ǰͬ��ɼ������id�ŵ��б����
					n=0;
					y=0;
					for(n=0;n<NUM_SomeTaskID;n++)
					{
						y=UARTCtrl->TaskID;
						y&=0xffff0000;
						y|=UARTCtrl->SomeTaskid[n];//RMTASKmax*2 byte ��ǰͬ��ɼ������b15-b8�����,b7-b0�������
						UARTCtrl->TaskID=y;
						y>>=8;
						y&=0xff;//�����
						if(y>=(RMTASKmax+ExtRMTASKmax))
						{
							continue;
						}
						UARTCtrl->MeterCount[y]=SetNo;//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
						y=RM_Meter(PORTn);//��ǰ���ܱ�����Ƿ�Ҫ��;���:UARTCtrl->MeterCount[],UARTCtrl->TaskID;����:0=����,1=Ҫ��,2=���,ͬʱ��дͨ�ŵ�ַ,������,�����Լ����,
						if(y)
						{
							break;
						}
					}
				}
				else
				{
					y=RM_Meter(PORTn);//��ǰ���ܱ�����Ƿ�Ҫ��;���:UARTCtrl->MeterCount[],UARTCtrl->TaskID;����:0=����,1=Ҫ��,2=���,ͬʱ��дͨ�ŵ�ַ,������,�����Լ����,
				}
			#else//#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
				y=RM_Meter(PORTn);//��ǰ���ܱ�����Ƿ�Ҫ��;���:UARTCtrl->MeterCount[],UARTCtrl->TaskID;����:0=����,1=Ҫ��,2=���,ͬʱ��дͨ�ŵ�ַ,������,�����Լ����,
			#endif
				if(y==1)
				{//Ҫ��
					i=UARTCtrl->TaskID;//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
					if(((i>>20)&0xf)==4)
					{//4=RetryMetering ����
						if(((i>>24)&0xf)==1)
						{//��ͨ�ɼ�����
							i=UARTCtrl->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
							if(((i!=2)&&(i!=3))||(PORTn==ACSAMPLEPORT))//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
							{//��������645-2007��DL698.45��ı�;Ҳ����������
								RM_OK_FAIL(PORTn,1);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�,2=�ɹ�������Ϊ��
								y=0;//0=����
							}
						}
					}
				}
				if(y==1)
				{//Ҫ��
					break;
				}
			#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
				if(PORTn==RS485_4PORT)
				{//�ز���
					if(UARTCtrl->RMMeterCountAuto!=0)//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
					{//·�����������ڳ���ģʽ
						if(y==2)
						{
							return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
						}
						return 0;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
					}
				}
			#endif
				y=0;
				if(UARTCtrl->RMMeterCountAuto!=0)//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
				{
					break;
				}
				i=RM_MeterCount(PORTn);//���ܱ����+1;����:0=����,1=�����
				if(i)
				{
					return 0;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				}
			}
			if(y==0)
			{
				return 0;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
			}
			
			UARTCtrl->CONNECT=1;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������
			UARTCtrl->ReRMCount=0;//�����ط�����
		#if(USER/100)==17//����ϵ
			if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
			//if(((UARTCtrl->TaskID>>20)&0xf)==5)
				UARTCtrl->RMprotocol=3;
		#endif
			switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
			{
				case 1://DL/T645-1997
				case 2://DL/T645-2007
				case 3://DL/T698.45
					break;
				case 4://CJ/T 188-2004
					break;
				case ProtocolNO_ABB://ABB
					x=RM_TxDATA(PORTn,pCSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
					if(x)
					{
						UARTCtrl->TxByte=x;//�����ֽ���
					}
					return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
					x=RM_TxDATA(PORTn,pCSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
					if(x)
					{
						UARTCtrl->TxByte=x;//�����ֽ���
					}
					return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
			}
		case 100://100=����
			UARTCtrl->CONNECT=101;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������
			UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
			UARTCtrl->RMCSDCount=0;//����CSD����
			UARTCtrl->RMCSDSubCount=0;//����CSD�Ӽ���
			UARTCtrl->RMROADCount=0;//����CSD��ROAD��OAD����(��DL698����ʱ)
			UARTCtrl->RMROADCountOK=0;//�������ݻ���ROADʱ�����������OAD���ݸ�������
			UARTCtrl->ReRMCount=0;//�����ط�����
			UARTCtrl->RM_OKFAIL=0;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
			UARTCtrl->RM_DI_TI_Count=0;//��ʱ�����ɼ�ʱ�ļ������
			UARTCtrl->RM_Rx_date_40020200[0]=0;//������յ�����������MAC��֤�ĵ��ܱ���;0th=״̬(0=��,1=���ڳ�,2=���),1th=��ų���(<=6),2th-7th���ֵ
			RM_RxDATA_START(PORTn);//����������ݴ���-��ʼ
			break;
		case 101://101=������
			i=i;
			break;
		case 102://102=�ر�����
			switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
			{
				default:
				case 1://DL/T645-1997
				case 2://DL/T645-2007
					//�µ��ܱ�
					RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
					return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case 3://DL/T698.45
					//�µ��ܱ�
					RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
					return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case 4://CJ/T 188-2004
					//�µ��ܱ�
					RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
					return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case ProtocolNO_ABB://ABB
					x=RM_TxDATA(PORTn,pCSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
					if(x)
					{
						UARTCtrl->TxByte=x;//�����ֽ���
					}
					return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
				case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
					x=RM_TxDATA(PORTn,pCSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
					if(x)
					{
						UARTCtrl->TxByte=x;//�����ֽ���
					}
					return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
			}
	}
	
#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
	if(UARTCtrl->RMMeterCountAuto==1)//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
	{
		NUM_SomeTaskID=UARTCtrl->NUM_SomeTaskID;//��ǰͬ��ɼ������id�ŵ��б����
		n=0;
		y=0;
		for(n=0;n<NUM_SomeTaskID;n++)
		{
			y=UARTCtrl->TaskID;
			y&=0xffff0000;
			y|=UARTCtrl->SomeTaskid[n];//RMTASKmax*2 byte ��ǰͬ��ɼ������b15-b8�����,b7-b0�������
			UARTCtrl->TaskID=y;
			y>>=8;
			y&=0xff;//�����
			if(y>=(RMTASKmax+ExtRMTASKmax))
			{
				continue;
			}
			UARTCtrl->MeterCount[y]=SetNo;//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
			y=RM_Meter(PORTn);//��ǰ���ܱ�����Ƿ�Ҫ��;���:UARTCtrl->MeterCount[],UARTCtrl->TaskID;����:0=����,1=Ҫ��,2=���,ͬʱ��дͨ�ŵ�ַ,������,�����Լ����,
			if(y)
			{
				break;
			}
		}
		if(y==0)
		{//û�ҵ�
			return 3;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
		}
	}
#endif
	while(1)
	{
		pCSD=RM_CSD(PORTn);//ȡ�������ݱ�ʶCSD;����:0=��Ч,1=CSD��URATCtrl,����ֵ=͸������֡����ָ��
		if(pCSD!=0)
		{//��Ч
			if((u32)pCSD==1)
			{//��ͨ�ɼ�����,�¼��ɼ�����
				pCSD=(u8*)&UARTCtrl->RMCSD;//1=CSD��URATCtrl
				switch((UARTCtrl->TaskID>>20)&0xf)//�ɼ�����
				{
					default:
						break;
					case 0://0=�ɼ���ǰ����
					case 1://1=�ɼ��ϵ�N��
					case 2://2=������ʱ��ɼ�
					case 3://3=��ʱ�����ɼ�
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
						switch(i)
						{//
							case 0x5002://���Ӷ���
								//����������,������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����
								DateSaveTI(PORTn,UARTCtrl->date_time_s);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
								FileID=Get_FileID(PORTn);//UART�������FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0����
								//FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע
								Comm_Ram->msFILESearchCount=0;
								filename=Get_RecordFileName(FileID,0xffffffff,UARTCtrl->date_time_s);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
								if(filename!=0xffffffff)
								{//�ҵ�
									if(filename<FILENAMEmax_FLASH)
									{
										file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
									}
									else
									{
										file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
									}
									y=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
									if(y>NUM_RMmax)
									{
										y=NUM_RMmax;
									}
									ms=Get_ms();
									ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
									RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
									Addr0=UARTCtrl->RMAddr[0];
									Addr1=UARTCtrl->RMAddr[1];
									Addr2=UARTCtrl->RMAddr[2];
									for(i=0;i<y;i++)
									{
										if(RecordFileHead->Addr0==Addr0)
										{
											if(RecordFileHead->Addr1==Addr1)
											{
												if(RecordFileHead->Addr2==Addr2)
												{
													pCSD=0;//�ѳ�
													x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
													x&=0xf0;
													x|=1;
													UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
													break;
												}
											}
										}
										RecordFileHead++;
									}
								}
								break;
							default:
								break;
						}
						break;
					case 4://4=RetryMetering ����(��ͬ,2=������ʱ��ɼ�)
						i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
						switch(i)
						{//
							case 0x5002://���Ӷ���
							case 0x5004://�ն���
								DateSaveTI(PORTn,UARTCtrl->date_time_s);//��ͨ�ɼ������Ĳɼ��洢ʱ��;����:pOUT=date_time_s
								FileID=Get_FileID(PORTn);//UART�������FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0����
								//FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע
								Comm_Ram->msFILESearchCount=0;
								filename=Get_RecordFileName(FileID,0xffffffff,UARTCtrl->date_time_s);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFFFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
								if(filename!=0xffffffff)
								{//�ҵ�
									if(filename<FILENAMEmax_FLASH)
									{
										file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
									}
									else
									{
										file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
									}
									y=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
									if(y>NUM_RMmax)
									{
										y=NUM_RMmax;
									}
									ms=Get_ms();
									ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
									RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
									Addr0=UARTCtrl->RMAddr[0];
									Addr1=UARTCtrl->RMAddr[1];
									Addr2=UARTCtrl->RMAddr[2];
									for(i=0;i<y;i++)
									{
										if(RecordFileHead->Addr0==Addr0)
										{
											if(RecordFileHead->Addr1==Addr1)
											{
												if(RecordFileHead->Addr2==Addr2)
												{
													pCSD=0;//�ѳ�
													x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
													x&=0xf0;
													x|=1;
													UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
													break;
												}
											}
										}
										RecordFileHead++;
									}
								}
								break;
							default:
								pCSD=0;//�ѳ�
								break;
						}
						break;
				}
			//֡����
				if(pCSD!=0)
				{
				//����
					if(PORTn==ACSAMPLEPORT)
					{//����
						p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
						x=RM_TxDATA(PORTn,pCSD,p8rx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ���						
						if(x)
						{
							p8rx[1]=0xaa;
							p8rx[2]=0xaa;
							p8rx[3]=0xaa;
							p8rx[4]=0xaa;
							p8rx[5]=0xaa;
							p8rx[6]=0xaa;
							UARTCtrl->TxByte=x;//�����ֽ���
							#ifdef DEBUG_JSON
							for(i=0;i<x;i++)
								myprintf("%02x ",p8rx[i],0,0);
							myprintf("\n",0,0,0);
							#endif
							return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
						}
					}
				//RS485
					if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
					{//RS485
						switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
						{
							case 1://DL/T645-1997
							case 2://DL/T645-2007
							case 3://DL/T698.45
								x=RM_TxDATA(PORTn,pCSD,p8tx+4);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
								if(x)
								{
									p8tx[0]=0xfe;
									p8tx[1]=0xfe;
									p8tx[2]=0xfe;
									p8tx[3]=0xfe;
									x+=4;
									UARTCtrl->TxByte=x;//�����ֽ���
									#ifdef DEBUG_JSON
									for(i=0;i<x;i++)
										myprintf("%02x ",p8tx[i],0,0);
									myprintf("\n",0,0,0);
									#endif
									return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
								}
								break;
							default:
								x=RM_TxDATA(PORTn,pCSD,p8tx);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
								if(x)
								{
									UARTCtrl->TxByte=x;//�����ֽ���
									return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
								}
								break;
						}
					}
				//�ز�
					if(PORTn==RS485_4PORT)
					{//�ز�
						switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
						{
							case 1://DL/T645-1997
							case 2://DL/T645-2007
							case 3://DL/T698.45
								if(Comm_Ram->RouterType==10)//·��������:0=����,1=��̩,2=����,3=����,4=����,5=��˹��,6=����΢,7=Ѷ��,8=Ǯ��,9=����,10=�¿���
								{//�¿���ǿ�Ƽ�������������,ͬʱ����0xfe
									x=RM_TxDATA(PORTn,pCSD,p8tx+128);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
									if(x)
									{
										UARTCtrl->TxByte=x;//�����ֽ���
										return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
									}
									break;
								}
								x=RM_TxDATA(PORTn,pCSD,p8tx+128+4);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
								//myprintf("[%s:%d]:�ز���֡����ֽ���%d\n",(u32)&__func__,(u32)__LINE__,x);
								if(x)
								{//�����ֽ���
									p8tx[128]=0xfe;
									p8tx[129]=0xfe;
									p8tx[130]=0xfe;
									p8tx[131]=0xfe;
									x+=4;
									UARTCtrl->TxByte=x;//�����ֽ���
									//for(x=0;x<UARTCtrl->TxByte;x++)
										//myprintf("%02x ",p8tx[x],0,0);
									//myprintf("\n",0,0,0);
									return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
								}
								break;
							default:
								x=RM_TxDATA(PORTn,pCSD,p8tx+128);//������֡;���:p8CSD=�跢�͵�CSDָ��,p8tx=���ͻ���ָ��;����:��֡���ܳ����ֽ��� 
								if(x)
								{
									UARTCtrl->TxByte=x;//�����ֽ���
									return 2;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
								}
								break;
						}
					}
				}
			}
			else
			{//͸������
				
				
				
			}
		}
		i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
		i++;
		UARTCtrl->RMCSDSubCount=i;
		x=UARTCtrl->RMCSDSubCountMax;
		if(i>=x)
		{
			//1��������CSD������
			i=RM_One_CSD(PORTn);//��鳭���Ƿ����1��������CSD������;����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127
			if(i)
			{
				switch(i)
				{
					case 1:
						i=CopyString((u8 *)"����1���Ȳ���",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
					case 2:
						i=CopyString((u8 *)"����2���Ȳ���",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
					case 3:
						i=CopyString((u8 *)"�������йس���ֵ��",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
					case 4:
						i=CopyString((u8 *)"�޶�Ӧ�ӿ����",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
					case 5:
						i=CopyString((u8 *)"����������127",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
					case 6:
						i=CopyString((u8 *)"����������������",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
						break;
				}
				p8=(u8*)ADDR_DATABUFF;
				p8[i]=0;
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=1000/10;//��֡����������ʱ,����ʾ������
			}
			
			UARTCtrl->RMCSDSubCount=0;//����CSD�Ӽ���
			i=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����
			i++;
			UARTCtrl->RMROADCount=i;
			x=UARTCtrl->RMROADCountMax;
			if(i>=x)
			{
				UARTCtrl->RMROADCount=0;//����CSD��ROAD��OAD����
				UARTCtrl->RMROADCountOK=0;//�������ݻ���ROADʱ�����������OAD���ݸ�������
				i=UARTCtrl->RMCSDCount;//����CSD����
				i++;
				UARTCtrl->RMCSDCount=i;
				x=UARTCtrl->RMCSDCountMax;
				if(i>=x)
				{//���������
					RecordFileHead=(RecordFileHead_TypeDef*)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
					p16=(u16 *)Get_ADDR_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ����ַ
					p16+=(LEN_RM_DI_CSD)/2;//����д��buff����CSD�ĳ���
					i=p16[1];
					#if(USER/100)==17
					i=(UARTCtrl->TaskID>>20)&0xf;
					if(i==4||i==3)//����//���ɼ�ʱ��ɼ��ķ�������
					{
						i=1;
					}else
					{
						i=p16[1];
					}
					#endif
					if(i)
					{//����������
						//д��¼
						RM_RxDATA_END(PORTn);//����������ݴ���-����
						RM_RecordFile(PORTn,(u8*)RecordFileHead);//�����¼
						//�峭��ɹ��ж�����
						p16[1]=0;
						x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
						x&=0xf0;
						x|=1;
						UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
					}
					else
					{//����������
						//�峭��ɹ��ж�����
						p16[1]=0;
						x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
						if((x&0xf)==0)//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
						{
							x|=2;
							UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
						}
					}
					switch((UARTCtrl->TaskID>>20)&0xf)//�ɼ�����
					{
						default:
						case 0://0=�ɼ���ǰ����
						case 1://1=�ɼ��ϵ�N��
						case 2://2=������ʱ��ɼ�
						case 3://3=��ʱ�����ɼ�
							i=(UARTCtrl->RMCSD[2]<<8)|(UARTCtrl->RMCSD[3]<<0);//�ɼ����ݵ�OI
							switch(i)
							{
								case 0x5002://���Ӷ���(����������)
									i=UARTCtrl->TaskID;
									i>>=20;
									i&=0xf;
									switch(i)//��ͨ�ɼ������Ĳɼ�����
									{
										default:
										case 0://�ɼ���ǰ����,
										case 1://�ɼ��ϵ�N��,
										case 2://������ʱ��ɼ�,
											break;
										case 3://��ʱ�����ɼ�(����)
											UARTCtrl->RM_DI_TI_Count++;//��ʱ�����ɼ�ʱ�ļ������
											//������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����
											i=UARTCtrl->TaskID;
											i>>=8;
											i&=0xff;//�����
											//����ִ�м��ʱ��
											x=ADDR_6012_2_SDRAM+(i*LENper_6012_2);
											i=pTItoSECOND((u8*)x+5);//TIתΪ��ֵ,����:hex��
											if(PORTn==RS485_4PORT)
											{//���ز���
											#if (CurveTaskTImax)
												if(i<CurveTaskTImax)//(24*60*60)//���߳�������ִ��Ƶ�����ֵ(��),��ֵ�������ɼ������ɼ����ʱ�����ǰ���������ߵ���
												{
													i=CurveTaskTImax;
												}
											#endif
											}
											//�ɼ������ɼ����ʱ��
											x=pTItoSECOND(UARTCtrl->RM_DI_TI);//TIתΪ��ֵ,���:pTI����������;����:hex��
											//#if (USER/100)==17
											//if(UARTCtrl->RM_DI_TI_Count>0)
											//{
												//x*=(UARTCtrl->RM_DI_TI_Count-1);
											//}
											//#else
											//x*=UARTCtrl->RM_DI_TI_Count;
											//#endif
											x*=UARTCtrl->RM_DI_TI_Count;
											if(x<i)
											{
												UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
												UARTCtrl->RMCSDCount=0;//����CSD����
												UARTCtrl->RMCSDSubCount=0;//����CSD�Ӽ���
												UARTCtrl->RMROADCount=0;//����CSD��ROAD��OAD����(��DL698����ʱ)
												UARTCtrl->RMROADCountOK=0;//�������ݻ���ROADʱ�����������OAD���ݸ�������
												UARTCtrl->ReRMCount=0;//�����ط�����
												x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
												if((x>>4)!=2)
												{
													if((x&0xf)==2)
													{
														x=0x20;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
													}
												}
												x&=0xf0;
												UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
												RM_RxDATA_START(PORTn);//����������ݴ���-��ʼ
												continue;
											}
											//���ɼ�����ĵ�������
											x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
											x>>=4;
											if(x!=2)
											{
												x=1;
											}
											UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
											break;
										case 4://RetryMetering ����
											break;
									}
									break;
								//========����̨��Ķ�==========
								case 0x5006://�¶���
								case 0x5004://�ն���
									UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
									UARTCtrl->RMCSDCount=0;//����CSD����
									UARTCtrl->RMCSDSubCount=0;//����CSD�Ӽ���
									UARTCtrl->RMROADCount=0;//����CSD��ROAD��OAD����(��DL698����ʱ)
									UARTCtrl->RMROADCountOK=0;//�������ݻ���ROADʱ�����������OAD���ݸ�������
									UARTCtrl->ReRMCount=0;//�����ط�����
									break;
								default:
									break;
							}
							break;
						case 4://4=RetryMetering ����(��ͬ,2=������ʱ��ɼ�)
							switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
							{
								default:
									break;
								case 2://DL/T645-2007
								case 3://DL/T698.45
									UARTCtrl->RM_DI_TI_Count++;//��ʱ�����ɼ�ʱ�ļ������
									//#if(USER/100)==17
									//if((UARTCtrl->RM_DI_TI_Count-1)<UARTCtrl->RM_DI_N)
									//#else
									//if(UARTCtrl->RM_DI_TI_Count<UARTCtrl->RM_DI_N)
									//#endif
									if(UARTCtrl->RM_DI_TI_Count<UARTCtrl->RM_DI_N)
									{
										UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
										UARTCtrl->RMCSDCount=0;//����CSD����
										UARTCtrl->RMCSDSubCount=0;//����CSD�Ӽ���
										UARTCtrl->RMROADCount=0;//����CSD��ROAD��OAD����(��DL698����ʱ)
										UARTCtrl->RMROADCountOK=0;//�������ݻ���ROADʱ�����������OAD���ݸ�������
										UARTCtrl->ReRMCount=0;//�����ط�����
										x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
										if((x>>4)!=2)
										{
											if((x&0xf)==2)
											{
												x=0x20;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
											}
										}
										x&=0xf0;
										UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
										RM_RxDATA_START(PORTn);//����������ݴ���-��ʼ
										continue;
									}
									//���ɼ��ĵ�������
									x=UARTCtrl->RM_OKFAIL;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
									x>>=4;
									if(x!=2)
									{
										x=1;
									}
									UARTCtrl->RM_OKFAIL=x;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
									break;
							}
							break;
					}
					
					RM_OK_FAIL(PORTn,UARTCtrl->RM_OKFAIL);//����ɹ�ʧ�ܵ�ַ��¼:���:OK=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
					
					//���ر�����֡
					switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
					{
						default:
						case 1://DL/T645-1997
						case 2://DL/T645-2007
							//�µ��ܱ�
							RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
							return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
						case 3://DL/T698.45
							//�µ��ܱ�
							RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
							return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
						case 4://CJ/T 188-2004
							//�µ��ܱ�
							RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
							return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
						case ProtocolNO_ABB://ABB
							//�µ��ܱ�
							RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
							return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
						case ProtocolNO_Landis://Landis
							//�µ��ܱ�
							RM_MeterCount(PORTn);//���ܱ����+1;����:0=û����,1=����
							return 1;//����:0=���賭,1=�ѳ�,2=���ڳ�,3=ʧ��,4=������ͣ
					}
				
				}
			}
		}
	}
}











