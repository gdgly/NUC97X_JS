

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"
#include "../DL698/SearchMeter_Record.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"
#include "../Terminal698/Terminal_PowerDown.h"
#include "../DL698/RM_3762_RouterActive.h"

#include "../DL698/DL698_JSON.h"


//Comm_Ram->DL698YMDHMS[10];//��(�ȸ��ֽں���ֽ�),��,��,ʱ,��,��,�ٷ���,��
//#define ADDR_6012_2     ADDR_6002_127+LENmax_6002_127//�������ñ�(��ʼ��ֵ0x00)
//0 ����2���������õ�Ԫ����=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
// {
//2 	����ID    unsigned��
//4 	ִ��Ƶ��   TI��
//8 	��������   enum
//  	{
//  		1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//		}��
//10	�������    unsigned��
//12	��ʼʱ��    date_time_s��
//20	����ʱ��    date_time_s��
//28	��ʱ        TI��
//32	ִ�����ȼ�  enum{����չ���(0),��Ҫ��1������Ҫ��2������Ҫ��3�������ܣ�4��}��
//34	״̬        enum{������1����ͣ�ã�2��}��
//36	����ʼǰ�ű�id   long-unsigned��
//39	������ɺ�ű�id   long-unsigned��
//42	��������ʱ��       structure��
//		{
//44  	����  enum
//			{
//       ǰ�պ�    ��0����
//       ǰ�����    ��1����
//       ǰ�պ��    ��2����
//       ǰ����    ��3��
//			}��
//46		ʱ�α�  array ʱ��
//48		ʱ�Ρ�=structure
//			{
//50			��ʼСʱ  unsigned��
//				��ʼ����  unsigned��
//				����Сʱ  unsigned��
//				��������  unsigned
//			}
//		}
//}
//����������Ϊ�ű�ʱ���������Ϊ�ű�id

//#define LENper_6012_2     128
//#define NUMmax_6012_2     16
//#define LENmax_6012_2     (LENper_6012_2*NUMmax_6012_2)




//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//b31-b28���ȼ�:0=����չ���,1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//b27-b24��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//b23-b20��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�,4=����
//b19-b16��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��
//b15-b8�����
//b7-b0�������


void CLR_RMCount(u32 PORTn)//��0������֡������ֵ
{
	UARTCtrl_TypeDef *UARTCtrl;
	u32 i;
	u16* p16timer;
	
	i=i;
	p16timer=p16timer;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->CONNECT=0;//DL698����:0=��,1=�������ӣ�2=��100=����,101=������
	UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
	UARTCtrl->RMCSDCount=0;//����CSD����
	UARTCtrl->RMCSDSubCount=0;//����CSD�Ӽ���
	UARTCtrl->RMROADCount=0;//����CSD��ROAD��OAD����(��DL698����ʱ)
	UARTCtrl->RMROADCountOK=0;//�������ݻ���ROADʱ�����������OAD���ݸ�������
	UARTCtrl->ReRMCount=0;//�����ط�����
	UARTCtrl->RM_OKFAIL=0;//����ɹ�ʧ�ܱ�־;=0ʧ�ܣ�=1�ɹ�,2=�ɹ���������
	UARTCtrl->RM_DI_TI_Count=0;//��ʱ�����ɼ�ʱ�ļ������
	UARTCtrl->RMOADROAD=0;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND
	
	UARTCtrl->RMAddr[0]=0;
	UARTCtrl->RMAddr[1]=0;
	UARTCtrl->RMAddr[2]=0;
#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	if(PORTn==RS485_4PORT)
	{
		ClrALL_RMListRMCount();//��ȫ����������������(�ز���);���:SetNo=�����Ŵ�0��ʼ
	#if (USER/100)==0//���Ժ����
		i=UARTCtrl->TaskID;
		i>>=8;
		i&=0xff;
		UARTCtrl->MeterCount[i]=0;//������ܱ����
		Terminal_Router->NUM_Enable_Concurrency=5;//����ز���ǰ��������֡��
		p16timer=(u16 *)(ADDR_Terminal_MS_Timer+4);//������ʱ�����ն��յ�·��ģ������֡�󵽷�����ʱ��ʱ��
		p16timer[0]=5000/10;//��֡����������ʱ,5���
	#else
		Terminal_Router->NUM_Enable_Concurrency=5;//����ز���ǰ��������֡��
	#endif
	}
#endif
}

u32 Check_OtherTask(u32 PORTn)//����Ƿ����������񣻷���:0=û,!=0��
{
	u32 i;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//��������
	i=UARTCtrl->ProxyTask;//��������:0=��,1=�д���,2...=���ڴ������,��ɽ������0
	i|=UARTCtrl->BroadcastTimeTask;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
	i|=UARTCtrl->AutoSearchTask;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
	i|=UARTCtrl->AutoReportTask;//���ܱ������ϱ�����:0=��,1=�������ϱ��ȴ���ȡ,2...������
	return i;
}

void TASK(u32 PORTn,u32 RM)//����;���:RM=0�ϱ�����,RM=1�ɼ�����
{
	u32 i;
	u32 x;
	u32 s;
	u32 id;
	u32 TaskID;
	u32 SourTaskID;
	u32 FileID;
	u32 ti;
	u32 HM1;
	u32 HM2;
	u32 NUM_TaskID;//��ǰ������
	u8 *p8s;
	u8 *p8d;
	u16 *p16;
	u32* p32;
	UARTCtrl_TypeDef *UARTCtrl;
	u64 d64;
	u32 n;
	RMokfailLIST_TypeDef *rmOKFAILlist;
#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
	u32 NUM_SomeTaskID;
	u32 TaskChange;
	u32 TH1TaskID;//ͬ���еĵ�һ������
#endif
		
	
#if (8+(NUMmax_6012_2*4))>LEN_DATABUFF
	#error
#endif
//ADDR_DATABUFF����
//7byte YYMDHMS ��Ƶ�ʱ�׼��ʼʱ���
//1byte
//7byte YYMDHMS ��Ƶ�ʱ�׼��ʼʱ���+��ʱ
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(RM)
	{//��������
		if(PORTn!=ACSAMPLEPORT)
		{//���ǽ���
			//ͣ���¼�����
			//if(PowerEvent->task<=10)//10=�����ܱ�ͣ���¼
			if(PowerEvent->task==10)//10=�����ܱ�ͣ���¼
			{
				if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run!=2)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
				{//ͣ���¼�����û���
					TaskID=(RMTASKnoPOWER<<8)+1;
					UARTCtrl->TaskID=TaskID;
					UARTCtrl->NUM_SomeTaskID=1;//ͬ��ɼ������id�ŵ��б����
					UARTCtrl->SomeTaskid[0]=TaskID;//RMTASKmax byte ͬ��ɼ������b15-b8�����,b7-b0�������
					if(UARTCtrl->TaskTime[RMTASKnoPOWER].Run==0)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
					{
//						if(PowerEvent->task<10)//10=�����ܱ��¼
//						{
//							UARTCtrl->TaskID=0;//�ϵ�,�ȴ���Դ�ﵽ�ָ���ѹֵ
//							return;
//						}
						PowerEvent->TimeErr=0;//�г����ĵ��ܱ�ͣ���¼�ͣ����ϵ�ʱ�䲻����
						UARTCtrl->TaskTime[RMTASKnoPOWER].Run=1;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
						UARTCtrl->NUMrmFAIL[RMTASKnoPOWER]=0;//��0����ʧ����
						CLR_RMCount(PORTn);//��0������֡������ֵ
						UARTCtrl->MeterCount[RMTASKnoPOWER]=0;//�������ѳ����ܱ����
						UARTCtrl->RMLOOP[RMTASKnoPOWER]=0;//RMTASKmax byte ȫ������һ��ı�������
						RM_Meter_Event3106();//ͣ/�ϵ��¼��賭�������������(��0��ʼ)
						NUMrm(PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
					}
					else
					{
						if(UARTCtrl->RMLOOP[RMTASKnoPOWER]>=1)//RMTASKmax byte ȫ������һ��ı�������
						{//����
							if(PowerEvent->TimeErr)//�г����ĵ��ܱ�ͣ�ϵ��¼�δ��������
							{//��
								UARTCtrl->TaskTime[RMTASKnoPOWER].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
								return;
							}
							UARTCtrl->TaskTime[RMTASKnoPOWER].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
							UARTCtrl->TaskID=0;
							return;
						}
					}
					return;
				}
			}
			//��������
			i=Check_OtherTask(PORTn);//����Ƿ����������񣻷���:0=û,!=0��
			if(i)
			{//����������
				return;
			}
			//�㲥Уʱ����ʼʱ��
			if(UARTCtrl->LastBroadcastRTCD2!=Comm_Ram->DL698YMDHMS[3])
			{//�ձ仯
				MR(ADDR_DATABUFF,ADDR_4204_2,LENmax_4204_2);
				p8s=(u8*)ADDR_DATABUFF;
				if(p8s[7]==1)
				{//����
					i=(Comm_Ram->DL698YMDHMS[4]<<16)|(Comm_Ram->DL698YMDHMS[5]<<8)|Comm_Ram->DL698YMDHMS[6];
					x=(p8s[3]<<16)|(p8s[4]<<8)|p8s[5];
					if((x&0xff)==0xff)
					{
						i&=0xffff00;
						x&=0xffff00;
					}
					if((x&0xff00)==0xff00)
					{
						i&=0xff00ff;
						x&=0xff00ff;
					}
					if((x&0xff0000)==0xff0000)
					{
						i&=0x00ffff;
						x&=0x00ffff;
					}
					if(i>=x)
					{//������ʱ��
						UARTCtrl->LastBroadcastRTCD2=Comm_Ram->DL698YMDHMS[3];//�ϴ�ִ�й㲥Уʱ-2��ʵʱ����
						UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+2;
						UARTCtrl->BroadcastTimeTask=1;//�㲥Уʱ����:0=��,1=Уʱ-2�ȴ���ʼ,2-127Уʱ-2����;0x81=Уʱ-3�ȴ���ʼ,0x82-0xFFУʱ-3����
						return;
					}
				}
			}
			//ÿ�������ѱ�����ʼʱ��
			//#define ADDR_6002_8     ADDR_6000+LENmax_6000//�ѱ����
			//����8��=structure
			//{
			//	�Ƿ�����ÿ�������ѱ�   bool��
			//	�Զ����²ɼ�����       bool��
			//	�Ƿ�����ѱ�����¼�   bool��
			//	����ѱ���ѡ��       enum
			//	{
			//		�����             ��0����
			//		ÿ�������ѱ�ǰ��� ��1����
			//		ÿ���ѱ�ǰ���     ��2��
			//	}
			//}
			//#define LENmax_6002_8     10
			//#define ADDR_6002_9     ADDR_6002_8+LENmax_6002_8//ÿ�������ѱ��������
			//����9��ÿ�������ѱ�������ã���=array ��ʱ�ѱ����
			//��ʱ�ѱ������=structure
			//{
			//   ��ʼʱ��         time��
			//   �ѱ�ʱ����min��  long-unsigned
			//}
			//#define LENmax_6002_9     (4*16)
			if(UARTCtrl->LastAutoSearchRTCD!=Comm_Ram->DL698YMDHMS[3])//�ϴ��Զ��ѱ�ʵʱ����
			{//�ձ仯
			#if (LENmax_6002_8+LENmax_6002_9)>LEN_DATABUFF
				#error
			#endif
				MR(ADDR_DATABUFF,ADDR_6002_8,LENmax_6002_8+LENmax_6002_9);
				p8s=(u8*)ADDR_DATABUFF;
				i=p8s[3];
			#if EnRS485AutoSearchMeter==0//0=��,1=��;����RS485���Զ��ѱ�
				if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
				{
					i=0;
				}
			#endif
				if(i==1)
				{//����ÿ�������ѱ�
					p8s=(u8*)ADDR_DATABUFF+LENmax_6002_8;
					s=p8s[1];//������
					if(s>1)
					{
						s=1;//��ֻ֧��һ��ʱ��
					}
					p8s+=2;
					while(s--)
					{
						i=(Comm_Ram->DL698YMDHMS[4]<<16)|(Comm_Ram->DL698YMDHMS[5]<<8)|Comm_Ram->DL698YMDHMS[6];
						x=(p8s[3]<<16)|(p8s[4]<<8)|p8s[5];
						if((x&0xff)==0xff)
						{
							i&=0xffff00;
							x&=0xffff00;
						}
						if((x&0xff00)==0xff00)
						{
							i&=0xff00ff;
							x&=0xff00ff;
						}
						if((x&0xff0000)==0xff0000)
						{
							i&=0x00ffff;
							x&=0x00ffff;
						}
						if(i>=x)
						{//������ʱ��
							UARTCtrl->LastAutoSearchRTCD=Comm_Ram->DL698YMDHMS[3];//�ϴ��Զ��ѱ�ʵʱ����
							i=(p8s[7]<<8)|p8s[8];
							UARTCtrl->AutoSearchTime=i;//�Զ��ѱ����ʱ��֣�0��ʾ����ʱ��ֱ���ѱ����
							if(i==0)
							{
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i=0;
										break;
									case RS485_2PORT://RS485-2
										i=1;
										break;
									default:
										i=255;
										break;
								}
								if(i<=2)
								{
									MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*i),7);
								}
								i=65535;
							}
							Terminal_Ram->AutoSearch_M_Timer=i;//2 �Զ��ѱ�����ʱ��ֶ�ʱ��
							UARTCtrl->TaskID=(RMTASKnoPOWER<<8)+4;//������4=ÿ�������ѱ�
							UARTCtrl->AutoSearchTask=1;//�Զ��ѱ�����:0=��,1=�ȴ�,2...����
							p8s=(u8*)ADDR_DATABUFF;
							//if(p8s[9]==2)
							if(p8s[9])
							{//ÿ�������ѱ�ǰ��ջ�ÿ���ѱ�ǰ���
								DEL_SearchMeter_Record(3);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
								DEL_SearchMeter_Record(4);//����ѱ���;��ڣ�FileID=3ȫ���ѱ�����=4��̨���ѱ���
							}
							return;
						}
						p8s+=9;
					}
				}
			}
			//
			
		}
		p32=(u32*)(ADDR_RMTaskID+4);//��ǰ��ִ�еĳ��������б�
	}
	else
	{
		p32=(u32*)(ADDR_REPORTTaskID+4);//��ǰ��ִ�е��ϱ������б�
	}
	
	NUM_TaskID=0;//��ǰ������
	for(id=0;id<NUMmax_6012_2;id++)
	{
		switch(id)
		{
			case RMTASKnoPOWER://ͣ�ϵ��¼��̶������

				continue;
		}
		p8s=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
		if(p8s[0]!=DataType_structure)
		{//ɾ��
			p16=(u16*)&UARTCtrl->TaskTime[id];
			for(i=0;i<(LEN_UARTnTask/2);i++)
			{
				p16[0]=0;
			}
			continue;
		}
		if(p8s[35]!=1)
		{//״̬        enum{������1����ͣ�ã�2��}
			p16=(u16*)&UARTCtrl->TaskTime[id];
			for(i=0;i<(LEN_UARTnTask/2);i++)
			{
				p16[0]=0;
			}
			continue;
		}
		d64=YYMDHMS_Sub_hex64(p8s+13,(u8*)&Comm_Ram->DL698YMDHMS);//hex64λʱ�Ӳ�ֵ����;���:pRTC1(��������ʱ����);pRTC2(��������ʱ����);����:pRTC2-pRTC1=HEX����ʱ�ӿ�����Ĳ�ֵ��,pRTC1��pRTC2�Ƿ�=0,pRTC2>=pRTC1��ֵΪ��,pRTC2<pRTC1��ֵΪ��
		if(d64>>32)
		{//ʱ��δ�����õĿ�ʼʱ��
			continue;
		}
		s=d64;
		MR(ADDR_DATABUFF,(u32)p8s+13,7);//ȡ���õĿ�ʼʱ��
		ti=(p8s[6]<<8)+p8s[7];//ȡƵ��ֵ
		if(ti==0)
		{
			ti=1;//��Сֵ=1
		}
		switch(p8s[5])
		{
			case 0://��      ��0����
				break;
			case 1://��      ��1����
				ti*=60;
				break;
			case 2://ʱ      ��2����
				ti*=60*60;
				break;
			case 3://��      ��3����
				ti*=60*60*24;
				break;
			case 4://��      ��4����
				x=(p8s[13]<<8)+p8s[14];//��ʼʱ�����
				i=(Comm_Ram->DL698YMDHMS[0]<<8)+Comm_Ram->DL698YMDHMS[1];//��ǰʱ�����
				i-=x;
				i*=12;
				i+=Comm_Ram->DL698YMDHMS[2];//��ǰʱ�����
				i-=p8s[15];//��ʼʱ�����
				i/=ti;//�»���Ƶ��ֵ
				i*=ti;
				pYYM_AddM_hex((u8*)ADDR_DATABUFF,i);//�����¼�m��,���ؼ�m�º��������
				break;
			case 5://��      ��5��
				x=(p8s[13]<<8)+p8s[14];//��ʼʱ�����
				i=(Comm_Ram->DL698YMDHMS[0]<<8)+Comm_Ram->DL698YMDHMS[1];//��ǰʱ�����
				i-=x;
				i/=ti;//�껯��Ƶ��ֵ
				i*=ti;
				i+=x;
				p8d=(u8*)ADDR_DATABUFF;
				p8d[0]=i>>8;
				p8d[1]=i;
			default:
				continue;
		}
		if(p8s[5]<=3)
		{
			if(PORTn==RS485_4PORT)
			{//���ز���
			#if (CurveTaskTImin)
				if(ti<CurveTaskTImin)//(60*60)//���߳�������ִ��Ƶ����Сֵ(��)
				{
					ti=CurveTaskTImin;//���߳�������ִ��Ƶ����Сֵ(��)
				}
			#endif
			}
			s/=ti;//�뻯��Ƶ��ֵ
			s*=ti;
			pYYMDHMS_AddS_hex((u8*)ADDR_DATABUFF,s);//��������ʱ�����S��,���ؼ�S������������ʱ����
		}
		//��ǰʱ�� �Ƚ� ��Ƶ�ʱ�׼��ʼʱ���
		i=Compare_DL698DataString((u8*)&Comm_Ram->DL698YMDHMS,(u8*)ADDR_DATABUFF,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
		if(i<=1)
		{//��ǰʱ�� >= ��Ƶ�ʱ�׼��ʼʱ���
			//��ʱ�α�
			s=(Comm_Ram->DL698YMDHMS[4]<<8)+Comm_Ram->DL698YMDHMS[5];//��ǰʱ��ʱ��
//			#if (USER%100)!=0//����̨�����
//				//��ǰʱ��ʱ��>=23��50�ֻ�<0��10�֣�������
//				if((s>=((23<<8)+50))||(s<10))
//				{
//					continue;
//				}
//			#endif
			x=p8s[47];//ʱ�α���
			if(x)
			{//ʱ�α�����=0����ʱ�α���Ϊ��Ч
				p8d=p8s+50;
				for(i=0;i<x;i++)
				{
					HM1=(p8d[1]<<8)+p8d[3];//ʱ�ο�ʼʱ��
					HM2=(p8d[5]<<8)+p8d[7];//ʱ�ν���ʱ��
					if(p8s[45]==0)
					{//ǰ�պ�    ��0��
						if((s>=HM1)&&(s<HM2))
						{
							break;
						}
					}
					if(p8s[45]==1)
					{//ǰ�����    ��1��
						if((s>HM1)&&(s<=HM2))
						{
							break;
						}
					}
					if(p8s[45]==2)
					{//ǰ�պ��    ��2��
						if((s>=HM1)&&(s<=HM2))
						{
							break;
						}
					}
					if(p8s[45]==3)
					{//ǰ����    ��3��
						if((s>HM1)&&(s<HM2))
						{
							break;
						}
					}	
					p8d+=10;
				}
				if(i>=x)
				{//ȫ��ʱ����Ч
					continue;
				}
			}
	//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	//���ȼ�:0=����չ���,1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
	//��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
	//��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�,4=RetryMetering ����
	//��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��,7=���������ĩ23��59��
			TaskID=p8s[33];//���ȼ�
			if(TaskID==0)
			{//���Ժ�����Բ���ʱ���ȼ���Ϊ0
				TaskID=1;//���ȼ�,0=�̶����ڳ����ϵ��¼�
			}
			if(TaskID>15)
			{
				TaskID=15;
			}
			TaskID<<=4;
			x=p8s[9];//��������(1-5)
			if(RM)
			{//��������
				if(x>=4)
				{
					continue;
				}
			}
			else
			{//�ϱ�����
				if(x!=4)
				{
					continue;
				}
			}
			x&=0xf;
			TaskID|=x;
			switch(x)//��������
			{
				case 1://��ͨ�ɼ����� ��1��
					//��ͨ����ʱ�ɼ�����
					TaskID<<=4;
					x=p8s[11];//�������0-127
					p8d=(u8*)ADDR_6014_SDRAM+(x*LENper_6014);
				#if (USER/100)==17//����ϵ
					if((p8d[10]&0xff)==0xf0)//���ɼ�����ʽ������ʽ�ɼ�
					{
						TaskID|=0x05;
						if(PORTn==ACSAMPLEPORT)
							continue;
					}
					else
						TaskID|=p8d[10]&0xf;
				#else
					TaskID|=p8d[10]&0xf;
				#endif
					//TaskID|=p8d[10]&0xf;
					//��ͨ����ʱ�洢ʱ��
					TaskID<<=4;
					p8d=Get_Element(p8d,6,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8d)
					{
						TaskID|=p8d[1]&0xf;
					}
					break;
				case 2://�¼��ɼ����� ��2��
					if(PORTn==ACSAMPLEPORT)
					{//���ɿڲ����¼�
						continue;
					}
					//�¼��ɼ�����ʱ�ɼ�����
					TaskID<<=4;
					x=p8s[11];//�������0-127
					p8d=(u8*)ADDR_6016_SDRAM+(x*LENper_6016);
					x=p8d[7]&0xf;
					switch(x)
					{
						case 0://���ڲɼ��¼�����
							break;
						case 1://����֪ͨ�ɼ������¼�����
							continue;
						case 2://����֪ͨ�ɼ�ָ���¼�����
						#if (USER%100)==0//���Ժ����
							continue;
						#else
							break;
						#endif
						case 3://ֱ�Ӵ洢֪ͨ�¼�����
						default:
							continue;
					}
					TaskID|=x;
					//�¼��ɼ�����ʱ�洢ʱ��=0
					TaskID<<=4;
					break;
				//csae 3://͸������     ��3��
				//case 4://�ϱ�����     ��4��
				//case 5://�ű�����     ��5��
				default:
					//�ɼ�����=0
					//�洢ʱ��=0
					TaskID<<=8;
					break;
			}
			TaskID<<=8;
			TaskID|=id;//�����
			TaskID<<=8;
			x=p8s[11];//�������0-127
			TaskID|=x;//�������
		#if CurveTaskTImin!=0//���߳�������ִ��Ƶ����Сֵ(��)
			if(PORTn==RS485_4PORT)
			{//���ز���
//				if(((TaskID>>16)&0xfff)==0x136)
//				{//���߳�������
//					TaskID&=0x0fffffff;//���ȼ�:0=����չ���
//				}
				p8d=(u8*)(u8*)ADDR_6014_SDRAM+(x*LENper_6014);
				p8d=Get_Element(p8d,4,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8d)
				{
					i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
					if(i==0x5B015002)//���Ӷ���(����)
					{
						TaskID&=0x0fffffff;//���ȼ�:0=����չ���
					}
				}
			}
		#endif
			
			i=Compare_DL698DataString((u8*)&UARTCtrl->TaskTime[id].StdTime,(u8*)ADDR_DATABUFF,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i==0)
			{//��׼ʱ�����ͬ
				if(UARTCtrl->TaskTime[id].Run==2)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
				{
					continue;
				}
			}
			else
			{//����ͬ
				//ִ��״̬��Ϊ0,ʹ��ؼ�����0��·������,����ԭ״̬���
				UARTCtrl->TaskTime[id].Run=0;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
				MW(ADDR_DATABUFF,(u32)&UARTCtrl->TaskTime[id].StdTime,7);
				MW(ADDR_DATABUFF,(u32)&UARTCtrl->TaskTime[id].StartTime,7);
				for(i=0;i<7;i++)
				{
					UARTCtrl->TaskTime[id].EndTime[i]=0xff;//����ִ�н���ʱ��YYMDHMS
				}
				if(RM)
				{
					UARTCtrl->RMLOOP[id]=0;//ȫ������һ��ı�������,����ID������
					UARTCtrl->NUMrmFAIL[id]=0;//��0����ʧ����
					SourTaskID=UARTCtrl->TaskID;
					UARTCtrl->TaskID=TaskID;
					NUMrm(PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
					UARTCtrl->TaskID=SourTaskID;
				}
			}
			//��ǰʱ�� �Ƚ� ��Ƶ�ʱ�׼��ʼʱ���+��ʱ
			ti=(p8s[30]<<8)+p8s[31];//ȡ��ʱ
			switch(p8s[29])
			{
				case 0://��      ��0����
					break;
				case 1://��      ��1����
					ti*=60;
					break;
				case 2://ʱ      ��2����
					ti*=60*60;
					break;
				default:
					ti=0;
					break;
			}
			MW((u32)&UARTCtrl->TaskTime[id].StdTime,ADDR_DATABUFF+8,7);
			pYYMDHMS_AddS_hex((u8*)ADDR_DATABUFF+8,ti);//��������ʱ�����S��,���ؼ�S������������ʱ����
			i=Compare_DL698DataString((u8*)&Comm_Ram->DL698YMDHMS,(u8*)ADDR_DATABUFF+8,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i<=1)
			{//��ǰʱ�� >= ��Ƶ�ʱ�׼��ʼʱ���+��ʱ
				p32[NUM_TaskID]=TaskID;
				NUM_TaskID++;//������id��
			}
		}
	}
	
	p32--;
	p32[0]=NUM_TaskID;//��ЧTask����
	if(NUM_TaskID==0)
	{//������
		if(RM)
		{//����
			UARTCtrl->NUM_SomeTaskID=0;//ͬ��ɼ������id�ŵ��б����
			UARTCtrl->SomeTaskid[0]=0;
		}
		UARTCtrl->TaskID=0;//ֵ0��ʾ������
		return;
	}
	p32++;
	
//���������
	if(RM)
	{
		for(n=0;n<NUM_TaskID;n++)
		{
			TaskID=p32[n];
			id=TaskID>>8;
			id&=0xff;
			if(id<NUMmax_6012_2)
			{
				if(UARTCtrl->TaskTime[id].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
				{
					i=UARTCtrl->NUMrmOK[id];//�Ѳɼ���ɵ��ܱ�����
					x=UARTCtrl->NUMrmBAD[id];//�Ѳɼ���ɵĵ��ܱ����������ñ����ԭ�����賭������
					s=UARTCtrl->NUMrmTOTAL[id];//������ɼ����ܱ�����
					SourTaskID=UARTCtrl->TaskID;
					UARTCtrl->TaskID=TaskID;
					FileID=Get_FileID(PORTn);
					UARTCtrl->TaskID=SourTaskID;
					if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
					{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
						//UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
					}
					else
					{
						if(i>=x)
						{
							i-=x;
						}
					}
					if(i>=s)
					{//OK��>=�賭��
						UARTCtrl->TaskTime[id].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
						for(i=0;i<7;i++)
						{
							UARTCtrl->TaskTime[id].EndTime[i]=Comm_Ram->DL698YMDHMS[i];//����ִ�н���ʱ��YYMDHMS
						}
						for(i=n;i<(NUM_TaskID-1);i++)
						{
							HM2=p32[i+1];
							p32[i]=HM2;
						}
						NUM_TaskID--;//����id��
					}
					
				}
			}
		}
	}
	
#if (USER%100)!=0//���ǵ��Ժ����
//ʱ����,����>=12ʱ�����ѳ���1�����Ϊ4=���ն���ִ��δ��ɵ�����,������ʱ��㲹��
	if(RM)
	{
		i=Comm_Ram->DL698YMDHMS[4];
		if(UARTCtrl->RTCh!=i)//ʵʱ�Ӹ��±�־ԭʼʱ
		{
			UARTCtrl->RTCh=i;//ʵʱ�Ӹ��±�־ԭʼʱ
			switch(i)
			{
				case 15:
				case 18:
				case 21:
					for(i=0;i<NUM_TaskID;i++)
					{
						HM1=p32[i];
						id=HM1>>8;
						id&=0xff;
						if(id<NUMmax_6012_2)
						{
							if(UARTCtrl->TaskTime[id].Run==4)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
							{
								UARTCtrl->TaskTime[id].Run=3;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
							}
						}
					}
					break;
			}
		}
	}
#endif
	
//����,�����ȸߵ�����ǰ
	while(NUM_TaskID>=2)
	{//������id��>=2
		//�����ȼ�����
		while(1)
		{
			x=0;//����û�仯��־
			for(i=0;i<(NUM_TaskID-1);i++)
			{
				HM1=p32[i];
				HM2=p32[i+1];
				s=HM1;
				ti=HM2;
				if(RM)
				{
					id=HM1>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
						{
							s|=0xf0000000;//4=���ն���ִ��δ���,5=ִ��δ���,�����ȼ�Ϊ���
						}
					}
					id=HM2>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
						{
							ti|=0xf0000000;//4=���ն���ִ��δ���,5=ִ��δ���,�����ȼ�Ϊ���
						}
					}
				}
				if((ti&0xfff00000)<(s&0xfff00000))
				{
					p32[i]=HM2;
					p32[i+1]=HM1;
					x=1;//����仯��־
				}
			}
			if(x==0)
			{
				break;
			}
		}
		//��ͬ���ȼ���������С������
		if(RM)
		{
			while(1)
			{
				x=0;//����û�仯��־
				for(i=0;i<(NUM_TaskID-1);i++)
				{
					HM1=p32[i];
					s=HM1>>28;
					HM2=p32[i+1];
					ti=HM2>>28;
					id=HM1>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
						{
							s|=0xf;//4=���ն���ִ��δ���,5=ִ��δ���,�����ȼ�Ϊ���
						}
					}
					id=HM2>>8;
					id&=0xff;
					if(id<NUMmax_6012_2)
					{
						if(UARTCtrl->TaskTime[id].Run>=4)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
						{
							ti|=0xf;//4=���ն���ִ��δ���,5=ִ��δ���,�����ȼ�Ϊ���
						}
					}
					if(s==ti)
					{//��ͬ���ȼ�
						id=HM1>>8;
						id&=0xff;
						s=UARTCtrl->RMLOOP[id];
						id=HM2>>8;
						id&=0xff;
						ti=UARTCtrl->RMLOOP[id];
						if(ti<s)
						{
							p32[i]=HM2;
							p32[i+1]=HM1;
							x=1;//����仯��־
						}
					}
				}
				if(x==0)
				{
					break;
				}
			}
		}
		//�����������ͣ
		if(RM)
		{
			TaskID=p32[0];
			id=TaskID>>8;
			id&=0xff;
			if(id<NUMmax_6012_2)
			{
				if(UARTCtrl->TaskTime[id].Run==1)//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
				{
					i=UARTCtrl->NUMrmOK[id];//�Ѳɼ���ɵ��ܱ�����
					x=UARTCtrl->NUMrmBAD[id];//�Ѳɼ���ɵĵ��ܱ����������ñ����ԭ�����賭������
					s=UARTCtrl->NUMrmTOTAL[id];//������ɼ����ܱ�����
					n=UARTCtrl->NUMrmFAIL[id];//�Ѳɼ�ʧ�ܵ��ܱ�����

					SourTaskID=UARTCtrl->TaskID;
					UARTCtrl->TaskID=TaskID;
					FileID=Get_FileID(PORTn);
					UARTCtrl->TaskID=SourTaskID;
					if((((FileID>>24)&0xf)==3)||(((TaskID>>20)&0xf)==4))
					{//ԭ�Ѳɼ������=0;�����ߺͲ���(�ն��������)�賭�����,�ڷ��͹���ʱʶ������Ƿ��ѳ�
						//UARTCtrl->NUMrmOK[id]=0;//ԭ�Ѳɼ������
					}
					else
					{
						if(i>=x)
						{
							i-=x;
						}
					}
					HM1=0;//��������������Ϊ�ռ�Ϊʧ����(̨����Բɼ���ؼ�Ϊ����ʧ��)
					rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
					rmOKFAILlist--;
					for(HM2=0;HM2<n;HM2++)
					{
						if(rmOKFAILlist->SpaceData==2)
						{//0=ʧ��,1=,2=��������������Ϊ�ձ�־ʧ��(̨����Բɼ���ؼ�Ϊ����ʧ��)
							HM1++;
						}
						rmOKFAILlist--;
					}
					if((i+HM1)>=s)
					{//(OK��+������������Ϊ�ռ�Ϊʧ����)>=�賭��
						UARTCtrl->TaskTime[id].Run=2;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
						for(i=0;i<7;i++)
						{
							UARTCtrl->TaskTime[id].EndTime[i]=Comm_Ram->DL698YMDHMS[i];//����ִ�н���ʱ��YYMDHMS
						}
						for(i=0;i<(NUM_TaskID-1);i++)
						{
							HM2=p32[i+1];
							p32[i]=HM2;
						}
						NUM_TaskID--;
						continue;
					}
//			#if (USER%100)!=0//���ǵ��Ժ
					i+=n;
					if(i>=s)
					{//(ʧ��+OK��)>=�賭��;�ѳ�һ��
						if(PORTn==RS485_4PORT)
						{//�ز��ڣ�485�ڲ�����ط��������������ص�������ʱ̨����Բɼ���ز����㳭��ɹ�����
							//����ط�����>=3
							HM1=UARTCtrl->NUMrmFAIL[id];//2*RMTASKmax byte�������Ѳɼ�ʧ�ܵ��ܱ�����
							if(HM1>NUM_RMmax)
							{
								HM1=0;
							}
							rmOKFAILlist=UARTCtrl->rmOKFAILlist[id+1];
							rmOKFAILlist--;
							for(i=0;i<HM1;i++)
							{
								if(rmOKFAILlist->ReRMCount<2)//���ʹ���<2
								//if(rmOKFAILlist->ReRMCount<5)//���ʹ���<5
								{//���ʹ���<2
									break;
								}
								rmOKFAILlist--;
							}
							if(i<HM1)
							{//���ط�����<3,û��ɳ�һ��
								break;
							}
						}
						//
						UARTCtrl->NUMrmFAIL[id]=0;//2*RMTASKmax byte�������Ѳɼ�ʧ�ܵ��ܱ�����
						i=UARTCtrl->RMLOOP[id];
						if(i!=0xff)
						{//RMTASKmax byte ȫ������һ��ı�������
							i++;
							UARTCtrl->RMLOOP[id]=i;
						}
						if(i==0xff)
						{//��ͬ���ȼ�,�ѳ�������Ϊ0xFFʱ,�ѳ�������0
							HM1=p32[0];
							HM1>>=28;
							for(i=1;i<NUM_TaskID;i++)
							{
								HM2=p32[i];
								if(HM1==(HM2>>28))
								{
									id=HM2>>8;
									id&=0xff;
									if(id<NUMmax_6012_2)
									{
										if(UARTCtrl->RMLOOP[id]!=0xff)
										{
											break;
										}
									}
								}
							}
							if(i>=NUM_TaskID)
							{//��Ϊ0xff
								for(i=0;i<NUM_TaskID;i++)
								{
									HM2=p32[i];
									if(HM1==(HM2>>28))
									{
										id=HM2>>8;
										id&=0xff;
										if(id<NUMmax_6012_2)
										{
											UARTCtrl->RMLOOP[id]=0;
										}
									}
								}
							}
						}
						HM1=p32[0];
						id=HM1>>8;
						id&=0xff;
						if(id<NUMmax_6012_2)
						{
							if(PORTn==RS485_4PORT)
							{//�ز���
							#if RMovertimeACKfail==1//�ز�·���������󳭱����������ʧ��ʱ:0=����������֡,1=���س���ʧ��(����)
								Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
							#endif
								HM1>>=28;//ִ�����ȼ�  enum{����չ��ߣ�0������Ҫ��1������Ҫ��2������Ҫ��3�������ܣ�4��}
							#if (USER/100)==9//����
								#define priority
								if(HM1==1)//��Ҫ(�ն���)
							#endif
								
							#if (USER/100)==15//����
								#define priority
								if(HM1==2)//(�ն���)
							#endif
								
							#ifndef priority
								if(HM1==1)//��Ҫ(�ն���)
							#endif
								{
								#if (USER%100)!=0//���ǵ��Ժ����
									i=Comm_Ram->DL698YMDHMS[4];//ʱ
									if(i>=14)
								#endif
									{//ʱ>=14��
										UARTCtrl->TaskTime[id].Run=4;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
										for(i=0;i<(NUM_TaskID-1);i++)
										{
											HM2=p32[i+1];
											p32[i]=HM2;
										}
										NUM_TaskID--;//����id��
										continue;
									}
								}
								else
								{//����Ҫ����(�ն���)
									UARTCtrl->TaskTime[id].Run=5;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
									for(i=0;i<(NUM_TaskID-1);i++)
									{
										HM2=p32[i+1];
										p32[i]=HM2;
									}
									NUM_TaskID--;//����id��
									continue;
								}
							}
							else
							{//485��
								UARTCtrl->TaskTime[id].Run=5;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
								for(i=0;i<(NUM_TaskID-1);i++)
								{
									HM2=p32[i+1];
									p32[i]=HM2;
								}
								NUM_TaskID--;//����id��
								continue;
							}
						}
						break;
					}
					else
					{//û��ɳ�һ��

					}
//			#endif
				}
			}
		}
		break;
	}

	if(NUM_TaskID==0)
	{
		p32[0]=0;
	}
	
//�ɼ��ϵ�N��,�ɼ�ʱ����
	id=p32[0];
	if(RM)
	{//��������
		i=id;
		i>>=24;
		i&=0xf;
		if(i==1)
		{//��ͨ�ɼ�����
			UARTCtrl->RM_DI_N=1;//�ɼ��ϵ�N��
			UARTCtrl->RM_DI_TI_Count=0;//��ʱ�����ɼ�ʱ�ļ������
			i=id&0xff;
			p8d=(u8*)ADDR_6014_SDRAM+(i*LENper_6014)+12;
			i=(id>>20)&0xf;
			switch(i)//�ɼ�����
			{
				default:
				case 0://0=�ɼ���ǰ����
					break;
				case 1://1=�ɼ��ϵ�N��
					
					UARTCtrl->RM_DI_N=p8d[0];//�ɼ��ϵ�N��
					break;
				case 2://2=������ʱ��ɼ�
					break;
				case 3://3=��ʱ�����ɼ�
					UARTCtrl->RM_DI_TI[0]=p8d[0];//��ʱ�����ɼ�
					UARTCtrl->RM_DI_TI[1]=p8d[1];//��ʱ�����ɼ�
					UARTCtrl->RM_DI_TI[2]=p8d[2];//��ʱ�����ɼ�
					break;
				case 4://4=RetryMetering ����
//�ɼ���ʽ���ݸ�ʽ���£�
//�ɼ�����	 �ɼ�����	 ��ʾ
//0	       NULL	     �ɼ���ǰ����
//1	       unsigned	 �ɼ��ϵ�N��
//2				 NULL	     ������ʱ��ɼ�
//3	       TI	       ��ʱ�����ɼ�
//4        RetryMetering ����
//RetryMetering::=structure
//{
//	����ʱ���� TI
//	��������(��N��) long-unsigned
//}
					p8d+=2;
					UARTCtrl->RM_DI_TI[0]=p8d[0];//����ʱ���� TI
					UARTCtrl->RM_DI_TI[1]=p8d[1];//����ʱ���� TI
					UARTCtrl->RM_DI_TI[2]=p8d[2];//����ʱ���� TI
					n=(p8d[4]<<8)|p8d[5];//��������(��N��) long-unsigned
					UARTCtrl->RM_DI_N=n;//�ɼ��ϵ�N��
				//��������:������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����ɼ��ϵ�N��
					i=id&0xff;//������
					p8d=(u8*)(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
					p8d=Get_Element(p8d,4,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8d)
					{
						i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
						if(i==0x5B015002)//���Ӷ���(����)
						{
							i=id;
							i>>=8;
							i&=0xff;//�����
							//����ִ�м��ʱ��
							x=ADDR_6012_2_SDRAM+(i*LENper_6012_2);
							i=pTItoSECOND((u8*)x+5);//TIתΪ��ֵ,����:hex��
							//�ɼ������ɼ����ʱ��
							x=pTItoSECOND(UARTCtrl->RM_DI_TI);//TIתΪ��ֵ,���:pTI����������;����:hex��
							i/=x;
							n*=i;
							n++;//��Ʋ����ж�8�����22:00-23:59,������ʱ�Ǵ�00:00�ϼ�8���㼴22:15-00:00�������1��
							UARTCtrl->RM_DI_TI_Count=1;//��ʱ�����ɼ�ʱ�ļ������
							UARTCtrl->RM_DI_N=n;//�ɼ��ϵ�N��
						}
					}
					break;
			}
		}
	}
	
//ͬ�೭�������б�
#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
	if(RM)
	{
		NUM_SomeTaskID=0;//���ܰ�ͬ�೭
		i=id;
		i>>=24;
		i&=0xf;//��������
		switch(i)
		{
			case 1://��ͨ�ɼ�����
				NUM_SomeTaskID=1;//�ɰ�ͬ�೭
				break;
			case 2://�¼��ɼ�����
				break;
			default:
				break;
		}
		if(NUM_SomeTaskID==0)
		{//���ܰ�ͬ�೭
			NUM_SomeTaskID=1;
			UARTCtrl->NUM_SomeTaskID=1;//ͬ��ɼ������id�ŵ��б����
			UARTCtrl->SomeTaskid[0]=id;//RMTASKmax byte ͬ��ɼ������b15-b8�����,b7-b0�������
		}
		else
		{//�ɰ�ͬ�೭
			NUM_SomeTaskID=0;//ͬ�����
			HM1=p32[0];
			HM1>>=16;//ȥb15-b8�����,b7-b0�������
			for(i=0;i<NUM_TaskID;i++)
			{
				HM2=p32[i];
				HM2>>=16;//ȥb15-b8�����,b7-b0�������
				if(HM1==HM2)
				{
					HM2=p32[i];
					UARTCtrl->SomeTaskid[NUM_SomeTaskID]=HM2;//RMTASKmax byte ͬ��ɼ������b15-b8�����,b7-b0�������
					NUM_SomeTaskID++;
				}
			}
			UARTCtrl->NUM_SomeTaskID=NUM_SomeTaskID;//ͬ��ɼ������id�ŵ��б����
		}
	}
#endif
	
//����ִ��״̬
#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
	if(RM)
	{//��������
		TaskChange=0;//����仯��־
		TH1TaskID=id;//ͬ���еĵ�һ������
		for(n=0;n<NUM_SomeTaskID;n++)
		{
			x=UARTCtrl->SomeTaskid[n];
			UARTCtrl->TaskID=(id&0xffff0000)|x;//RMTASKmax byte ��ǰͬ��ɼ������id���б�
			s=UARTCtrl->NUM_LastSomeTaskID;//�������ͬ��ɼ������id�ŵ��б����
			for(i=0;i<s;i++)
			{
				if(x==UARTCtrl->LastSomeTaskid[i])
				{//�ϴ�����ͬ�����
					break;
				}
			}
			if(i<s)
			{//�������ͬ
				SourTaskID=UARTCtrl->TaskID;//ԭ����TaskID
			}
			else
			{
				SourTaskID=0;
			}
			i=(x>>8)&0xff;
			if(i>=NUMmax_6012_2)
			{
				return;
			}
			if((UARTCtrl->TaskTime[i].Run==1)||(UARTCtrl->TaskTime[i].Run==3)||(UARTCtrl->TaskTime[i].Run==4)||(UARTCtrl->TaskTime[i].Run==5))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
			{//ԭִ����
				UARTCtrl->TaskTime[i].Run=1;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
				s=0;//���ñ仯��־
				if(SourTaskID!=UARTCtrl->TaskID)//���Ƚ����ȼ�
				{//�����л�
					s=1;//���ñ仯��־
					TaskChange=1;//����仯��־
				#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
					}
				#endif
				}
				x=Terminal_Ram->SETTask;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
				if(x&(1<<PORTn))
				{
					s=1;//���ñ仯��־
				#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
					}
				#endif
				}
				if(s)
				{//���ñ仯
					//UARTCtrl->NUMrmFAIL[i]=0;//��0����ʧ����
					CLR_RMCount(PORTn);//��0������֡������ֵ
					NUMrm(PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
				}
			}
			else
			{//����ִ��
				UARTCtrl->RMLOOP[i]=0;//ȫ������һ��ı�������,����ID������
				UARTCtrl->NUMrmFAIL[i]=0;//��0����ʧ����
				NUMrm(PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
				HM2=UARTCtrl->NUMrmOK[i];
				HM1=UARTCtrl->NUMrmBAD[i];
				if(HM2>=HM1)
				{
					HM2-=HM1;
				}
				HM1=UARTCtrl->NUMrmTOTAL[i];
				if(HM2>=HM1)
				{//OK��>=�賭��
					//���賭��(�賭��=0);��ͣ�ڵ�ǰ�����������������TaskID=0�󷵻�,�����вɼ������س�ʼ��
					UARTCtrl->TaskTime[i].Run=2;//ע:��RM_Monitor_Init(i)����Ϊ=1;����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
					TH1TaskID=0;//ͬ���еĵ�һ������
				}
				else
				{
					TaskChange=1;//����仯��־
					UARTCtrl->TaskTime[i].Run=1;//ע:��RM_Monitor_Init(i)����Ϊ=1;����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
					CLR_RMCount(PORTn);//��0������֡������ֵ
				#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(PORTn==RS485_4PORT)
					{
						if((id>>28)!=0)
						{//����,���ȼ�=0;�ϵ�����,ÿСʱ���ߵ��õ㳭
							Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
						}
					}
				#endif
					MW((u32)&Comm_Ram->DL698YMDHMS[0],(u32)&UARTCtrl->TaskTime[i].StartTime,7);//����ִ�п�ʼʱ��
				//#if (USER/100)==0//���Ժ����
				#if ((USER/100)==0||(USER/100)==15)
					RM_Monitor_Init(i);//�ɼ������س�ʼ��;���:ID=�����
				#else
					RM_Monitor_Init_PORTn(PORTn,i);//�ɼ������س�ʼ��(���˿ں�);���:PORTn=�˿ں�,ID=�����
				#endif
				}
			}
		}
		x=Terminal_Ram->SETTask;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
		x&=~(1<<PORTn);
		Terminal_Ram->SETTask=x;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
		
	//����ִ��������Run==1��ͣ��־Run=3
		for(x=0;x<RMTASKmax;x++)
		{
			for(n=0;n<NUM_SomeTaskID;n++)
			{
				s=UARTCtrl->SomeTaskid[n];//RMTASKmax*2 byte ��ǰͬ��ɼ������b15-b8�����,b7-b0�������
				s>>=8;
				if(x==s)
				{
					break;
				}
			}
			if(n<NUM_SomeTaskID)
			{//�������ͬ
				continue;
			}
			if(UARTCtrl->TaskTime[x].Run==1)
			{
				UARTCtrl->TaskTime[x].Run=3;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
			}
		}
		
	//��Ϊ�����������
		if(TaskChange)//����仯��־
		{
			for(n=0;n<NUM_SomeTaskID;n++)
			{
				UARTCtrl->LastSomeTaskid[n]=UARTCtrl->SomeTaskid[n];
			}
			UARTCtrl->NUM_LastSomeTaskID=NUM_SomeTaskID;
		}
	//����ͬ���еĵ�һ������
		UARTCtrl->TaskID=TH1TaskID;//ͬ���еĵ�һ������
		
	}
	else
	{//�ϱ�����
#endif
		//����Ϊԭͬ���������ǰ���򣬱�����Ϊ�ϱ������Ա��ο���
		//UARTCtrl->LastTaskID=id;//�������Ч�ɼ�����ID,������ʾ��
		i=(id>>8)&0xff;
		if(i>=NUMmax_6012_2)
		{
			return;
		}
		if((UARTCtrl->TaskTime[i].Run==1)||(UARTCtrl->TaskTime[i].Run==3)||(UARTCtrl->TaskTime[i].Run==4)||(UARTCtrl->TaskTime[i].Run==5))//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
		{//ԭִ����
			UARTCtrl->TaskTime[i].Run=1;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
			s=0;//���ñ仯��־
			if((id<<4)==(UARTCtrl->TaskID<<4))//���Ƚ����ȼ�
			{//����û��
				x=Terminal_Ram->SETTask;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
				if(x&(1<<PORTn))
				{
					s=1;//���ñ仯��־
				}
			}
			else
			{//�����л�
				UARTCtrl->TaskID=id;
				s=1;//���ñ仯��־
			}
			if(s)
			{//���ñ仯
				if(RM)
				{//��������
					//UARTCtrl->NUMrmFAIL[i]=0;//��0����ʧ����
					CLR_RMCount(PORTn);//��0������֡������ֵ
					NUMrm(PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
				#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
					}
				#endif
				}
				else
				{//�ϱ�����
					UARTCtrl->NUMrmTOTAL[i]=NUM_RMmax;//������ɼ�����
					UARTCtrl->NUMrmOK[i]=0;//�������Ѳɼ���ɵ��ܱ���
					UARTCtrl->NUMrmFAIL[i]=0;//�������Ѳɼ�ʧ�ܵ��ܱ���
					UARTCtrl->NUMrmBAD[i]=0;//�������Ѳɼ���ɵĵ��ܱ����������ñ����ԭ�����賭������
				}
				if(RM)
				{
					x=Terminal_Ram->SETTask;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
					x&=~(1<<PORTn);
					Terminal_Ram->SETTask=x;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
				}
			}
		}
		else
		{//����ִ��
			UARTCtrl->RMLOOP[i]=0;//ȫ������һ��ı�������,����ID������
			UARTCtrl->TaskID=id;
			if(RM)
			{//��������
				UARTCtrl->NUMrmFAIL[i]=0;//��0����ʧ����
				//CLR_RMCount(PORTn);//��0������֡������ֵ
				NUMrm(PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
				HM2=UARTCtrl->NUMrmOK[i];
				HM1=UARTCtrl->NUMrmBAD[i];
				if(HM2>=HM1)
				{
					HM2-=HM1;
				}
				HM1=UARTCtrl->NUMrmTOTAL[i];
				if(HM2>=HM1)
				{//OK��>=�賭��
					//���賭��;����ܲ�������������TaskID=0�󷵻�,�����вɼ������س�ʼ��
					UARTCtrl->TaskTime[i].Run=2;//ע:��RM_Monitor_Init(i)����Ϊ=1;����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
					UARTCtrl->TaskID=0;
				}
				else
				{
					UARTCtrl->TaskTime[i].Run=1;//ע:��RM_Monitor_Init(i)����Ϊ=1;����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
					CLR_RMCount(PORTn);//��0������֡������ֵ
				#if ((Project/100)==2)||((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
					if(PORTn==RS485_4PORT)
					{
						Terminal_Router->RouterCtrl=1;//·������������0=��,1=����,2=��ͣ,3=�ָ���ʱ,4=��״̬,5=�ָ�,6=��������;��ɺ��Զ��ص�0
					}
				#endif
					MW((u32)&Comm_Ram->DL698YMDHMS[0],(u32)&UARTCtrl->TaskTime[i].StartTime,7);//����ִ�п�ʼʱ��
				//#if (USER/100)==0//���Ժ����
				#if ((USER/100)==0||(USER/100)==15)
					RM_Monitor_Init(i);//�ɼ������س�ʼ��;���:ID=�����
				#else
					RM_Monitor_Init_PORTn(PORTn,i);//�ɼ������س�ʼ��(���˿ں�);���:PORTn=�˿ں�,ID=�����
				#endif
				}
			}
			else
			{//�ϱ�����
				MW((u32)&Comm_Ram->DL698YMDHMS[0],(u32)&UARTCtrl->TaskTime[i].StartTime,7);//����ִ�п�ʼʱ��
	//			UARTCtrl->NUMrmTOTAL[i]=0;//������ɼ�����;�����ļ���������;
	//			UARTCtrl->NUMrmOK[i]=0;//�������Ѳɼ���ɵ��ܱ���;�����ϱ������Ѿ��ϱ�����ѡ���������
	//			UARTCtrl->NUMrmFAIL[i]=0;//�������Ѳɼ�ʧ�ܵ��ܱ���;����
	//			UARTCtrl->NUMrmBAD[i]=0;//�������Ѳɼ���ɵĵ��ܱ����������ñ����ԭ�����賭������;����
				UARTCtrl->TaskTime[i].Run=1;//ע:��RM_Monitor_Init(i)����Ϊ=1;����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
			}
			if(RM)
			{
				x=Terminal_Ram->SETTask;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
				x&=~(1<<PORTn);
				Terminal_Ram->SETTask=x;//�������ñ�ɼ����������ñ�־:0=û����,0xff=����
			}
		}

	//����ִ��������Run==1��ͣ��־Run=3
		if(RM)
		{
			for(x=0;x<RMTASKmax;x++)
			{
				if(x==i)
				{
					continue;
				}
				if(UARTCtrl->TaskTime[x].Run==1)
				{
					UARTCtrl->TaskTime[x].Run=3;//����ִ��״̬:0=δִ��,1=ִ����,2=��ִ��,3=ִ������ͣ,4=���ն���ִ��δ���,5=ִ��δ���
				}
			}
		}
#if RMSOMETASK==1//0=��,1=��;ͬ�೭�������Ϊͬ�����񳭱�,������·��
	}
#endif
}



