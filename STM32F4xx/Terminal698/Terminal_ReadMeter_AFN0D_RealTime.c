
//ʵʱ����2������
#include "Project.h"
#include "Terminal_ReadMeter_AFN0D_RealTime.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_DL645_1997_Lib.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "Terminal_ReadMeter_DL645_1997.h"
#include "Terminal_ReadMeter_3762_Std.h"
#include "Terminal_AFN0D_CongealData_Fn.h"




void Wait_ReadMeter_AFN0D_RealTime(u32 PORTn,u32 Protocol)//�ȴ�1��Fn����ʵʱ�������
{
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32 *)(ADDR_MS_ENTRY);
	
	switch(PORTn)
	{
		case RS485_1PORT:
		case RS485_2PORT:
		case RS485_3PORT:
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			UARTCtrl->Task=3;//3=��������
			while(UARTCtrl->Task!=0)
			{
				switch(Protocol)
				{
					case 1://DL/T645-1997(1)
					#if Extern_Terminal_ReadMeter_DL645_1997_Fn==0//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
						Terminal_ReadMeter_DL645_1997_Fn(PORTn,(u8 *)&UARTCtrl->RMLib);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
					#else
						if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
						{
		    			p32=(u32 *)(ADDR_E1_ENTRY);
		    			(*(void(*)())p32[102])(PORTn,(u8 *)&UARTCtrl->RMLib);//102 ����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
							p32=(u32 *)(ADDR_I0_ENTRY);
						}
						else
						{
							return;
						}
					#endif
						break;
					case 30://DL/T645-2007(30)
						Terminal_ReadMeter_DL645_2007_Fn(PORTn,(u8 *)&UARTCtrl->RMLib);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
						break;
					default:
						return;
				}
				(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
			}
			break;
		case RS485_4PORT://�ز�
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			UARTCtrl->RMCount=0;//485�����������
			UARTCtrl->ReRMCount=0;//485���������ط�����
			Terminal_Router->RouterRealTimeTask=1;//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
			while(Terminal_Router->RouterRealTimeTask!=0)
			{
				if(Terminal_Router->RouterInitTask<200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
				{//·����������,������
					break;
				}
				if(Terminal_Router->RouterCtrl==0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				{
					RealTimeReadMeter_3762_Fn_Std(PORTn,(u8 *)&UARTCtrl->RMLib);//����1��Fn������д��FnDataBuff;RouterRealTimeTask=0=����(��Fn�������)
				}
				(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
			}
	#endif		
			break;
	}
}

u32 ReadMeter_AFN0D_RealTime(u32 Fn,u32 Pn,u64 YMDHM)//ʵʱ��ȡ2������;����0=û��,!=0������д��FLASH������FnDataBuff��ַ
{
	u32 i;
	u32 di;
	u32 MeterNo;//���ܱ��������
	u32 PORTn;
	u32 Protocol;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
	u32 ReadFlag;
	u8 *p8FnData;
	u32 YMD;
	u32 s;
	
	if(Pn==0)
	{
		return 0;//0=û��
	}
	MeterNo=Check_AFN04F10_Pn(Pn);//����ն˵��ܱ�/��������װ�����ò���ָ���Ĳ������;����0��,!=0Ϊ��ͬ������ŵ��������(1��ʼ)
	if(MeterNo==0)
	{
		return 0;//0=û��
	}
	i=MRR(ADDR_AFN04F10+((MeterNo-1)*LEN_AFN04F10_Pn)+4,1);//ͨ�����ʼ��˿ں�
	i&=0x1f;
	switch(i)
	{
		case 2://485-1
			PORTn=RS485_1PORT;
			break;
		case 3://485-2
			PORTn=RS485_2PORT;
			break;
		case 4://485-3
			PORTn=RS485_3PORT;
			break;
		case 31://�ز�
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			{
				PORTn=RS485_4PORT;
			}
			else
			{
				return 0;
			}
			break;
	#else
			return 0;
	#endif	
		default:
			return 0;//0=û��
	}
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p32=(u32 *)(ADDR_MS_ENTRY);
	while((UARTCtrl->Lock==0x55)||(UARTCtrl->Task!=0))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
		if(PORTn==RS485_4PORT)
		{
			if(UARTCtrl->Lock==0x55)
			{
				return 0;//0=û��
			}
		}
	}
	//
	UARTCtrl->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	UARTCtrl->MeterNoBackup=UARTCtrl->MeterNo;//485������ܱ�/��������װ�����(���ֶ���)(���г�����볭��ʵʱ����ʱ����)
	UARTCtrl->MeterNo=MeterNo;
	UARTCtrl->FnCountBackup=UARTCtrl->FnCount;//485����Fn����(���г�����볭��ʵʱ����ʱ����)
	
	Protocol=MRR(ADDR_AFN04F10+((MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������ BIN 1 
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	ReadFlag=0;//��ȷ����־
	switch(Protocol)
	{
		case 1://DL/T645-1997(1)
		#if Extern_Terminal_ReadMeter_DL645_1997_Fn==1//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
			if((Comm_Ram->SoftModule&(1<<4))!=0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				break;
			}
		#endif
			break;
		case 30://DL/T645-2007(30)
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		case 31://���нӿ�����խ����ѹ�ز�(31)
	#endif
		//�ն���
			if((Fn==161)||(Fn==163))
			{//F161�ն��� �����й�����ʾֵ(��,����1��M);F163�ն��� �����й�����ʾֵ(��,����1��M)
				//������ͬ�ն���ʱ����ϼ��ζ���(1-n)
				YMDHM>>=16;
				YMD=YMDHM;
				YMD=YMD_Add1D(YMD);//���������������ն˳�����������+1
				MC(0,ADDR_DATABUFF,12);
				MWR(YMD,ADDR_DATABUFF+3,3);
				MR(ADDR_DATABUFF+9,ADDR_TYMDHMS+3,3);
				di=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//ʱ�Ӳ�ֵ����,���R0=ʱ��1(���ʱ������)��ַ;R1=ʱ��2(���ʱ������)��ַ;����R0=HEX����ʱ�ӿ�����Ĳ�ֵ(��),ʱ��1ʱ��2�Ƿ�R0=0,ʱ��2>=ʱ��1��ֵΪ��,ʱ��2<ʱ��1��ֵΪ��
				if(di>>31)
				{//����
					break;
				}
				di/=(24*60*60);//���������
				if(di>=62)
				{
					break;//�����ඳ����62��
				}
				//di=���ۼ����(��n��)�ն���ʱ�����ݱ�ʶ
				s=2;//0=��,1=��,2=��
				while(1)
				{
					UARTCtrl->RMLib[0]=0;//ʵʱFn����մ�����ͬ��Fn
					UARTCtrl->RMLib[1]=1;//ͨ�ű�ʶ����
					UARTCtrl->RMLib[2]=0;//����ͷ,0=�ն˳���ʱ��,1=�ն˳���ʱ��+������
					UARTCtrl->RMLib[3]=0x06;//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
					UARTCtrl->RMLib[4]=1;//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
					MWR(di+0x05060001,(u32)&UARTCtrl->RMLib[10],4);//���ݱ�ʶ
					UARTCtrl->RMLib[14]=5;//���س���
					Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//�ȴ�1��Fn����ʵʱ�������
					i=MRR(((u32)p8FnData)+6+2,3);
					if(i==YMD)
					{
						break;
					}
					if(i==0xeeeeee)
					{//������
						if(s==2)
						{//���ǵ�һ��
							s=0;
							di=0;
							continue;
						}
						else
						{
							break;
						}
					}
					if(i>YMD)
					{//�������ϴζ���ʱ��>��Ҫ����ʱ��
						if(s==1)
						{
							break;//ԭΪ�����
						}
						s=0;
						di++;
						if(di>=62)
						{
							break;//�����ඳ����62��
						}
					}
					else
					{//�������ϴζ���ʱ��<��Ҫ����ʱ��
						if(s==0)
						{
							break;//ԭΪ�����
						}
						s=1;
						if(di==0)
						{
							break;//�����ඳ����62��
						}
						di--;
					}
				}
				if(i!=YMD)
				{
					break;//û�ҵ���ͬ����ʱ��
				}
				UARTCtrl->RMLib[0]=131;//ʵʱFn����մ�����ͬ��Fn;��131����129
				UARTCtrl->RMLib[1]=1;//ͨ�ű�ʶ����
				UARTCtrl->RMLib[2]=1;//����ͷ,0=�ն˳���ʱ��,1=�ն˳���ʱ��+������
				UARTCtrl->RMLib[3]=0x06;//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
				UARTCtrl->RMLib[4]=1;//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				if(Fn==161)
				{//F161�ն��� �����й�����ʾֵ(��,����1��M);F163�ն��� �����й�����ʾֵ(��,����1��M)
					MWR(di+0x05060101,(u32)&UARTCtrl->RMLib[10],4);//���ݱ�ʶ
				}
				if(Fn==163)
				{//F161�ն��� �����й�����ʾֵ(��,����1��M);F163�ն��� �����й�����ʾֵ(��,����1��M)
					MWR(di+0x05060201,(u32)&UARTCtrl->RMLib[10],4);//���ݱ�ʶ
				}
				UARTCtrl->RMLib[14]=0;//���س���
				UARTCtrl->RMLib[15]=20;//����ȷ���س���=0ʱ��Ԥ�Ʒ����ֽ���
				Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//�ȴ�1��Fn����ʵʱ�������
				if(p8FnData[6]!=0xee)
				{//��ȷ��
					ReadFlag=1;//��ȷ����־
					MR(((u32)p8FnData)+1,ADDR_TYMDHMS+1,5);//����ʱ��
					if(Fn==161)
					{//F161�ն��� �����й�����ʾֵ(��,����1��M);F163�ն��� �����й�����ʾֵ(��,����1��M)
						AddDateCongeal(YMDHM,161,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//���ն��������
					}
					if(Fn==163)
					{//F161�ն��� �����й�����ʾֵ(��,����1��M);F163�ն��� �����й�����ʾֵ(��,����1��M)
						AddDateCongeal(YMDHM,163,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//���ն��������
					}
				}
				break;
			}
		//�¶���
			if((Fn==177)||(Fn==179))
			{//F161�ն��� �����й�����ʾֵ(��,����1��M);F163�ն��� �����й�����ʾֵ(��,����1��M)
				//������ͬ�¶���ʱ����ϼ��ζ���(1-n)
				YMDHM>>=24;//�ն˳��������
				s=YMDHM;
				i=MRR(ADDR_TYMDHMS+4,2);//��ǰ����
				for(di=0;di<12;di++)
				{
					if(s==i)
					{
						break;
					}
					i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
				}
				if(di>=MaxMonthCongeal)
				{
					break;//�����ඳ����12��
				}
				UARTCtrl->RMLib[0]=131;//ʵʱFn����մ�����ͬ��Fn;��131����129
				UARTCtrl->RMLib[1]=1;//ͨ�ű�ʶ����
				UARTCtrl->RMLib[2]=1;//����ͷ,0=�ն˳���ʱ��,1=�ն˳���ʱ��+������
				UARTCtrl->RMLib[3]=0x06;//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
				UARTCtrl->RMLib[4]=1;//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				if(Fn==177)
				{//F177�¶��� �����й�����ʾֵ(��,����1��M);F179�¶��� �����й�����ʾֵ(��,����1��M)
					MWR(di+0x0001FF00,(u32)&UARTCtrl->RMLib[10],4);//���ݱ�ʶ
				}
				if(Fn==179)
				{//F177�¶��� �����й�����ʾֵ(��,����1��M);F179�¶��� �����й�����ʾֵ(��,����1��M)
					MWR(di+0x0002FF00,(u32)&UARTCtrl->RMLib[10],4);//���ݱ�ʶ
				}
				UARTCtrl->RMLib[14]=0;//���س���
				UARTCtrl->RMLib[15]=20;//����ȷ���س���=0ʱ��Ԥ�Ʒ����ֽ���
				Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//�ȴ�1��Fn����ʵʱ�������
				if(p8FnData[6]!=0xee)
				{//��ȷ��
					ReadFlag=1;//��ȷ����־
					MR(((u32)p8FnData)+1,ADDR_TYMDHMS+1,5);//����ʱ��
					if(Fn==177)
					{//F177�¶��� �����й�����ʾֵ(��,����1��M);F179�¶��� �����й�����ʾֵ(��,����1��M)
						AddMonthCongeal(YMDHM,177,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//���¶��������
					}
					if(Fn==179)
					{//F177�¶��� �����й�����ʾֵ(��,����1��M);F179�¶��� �����й�����ʾֵ(��,����1��M)
						AddMonthCongeal(YMDHM,179,Pn,((u32)p8FnData)+1,6+(5*(1+MaxTariff)));//���¶��������
					}
				}
				break;
			}
		//���߶���
			if((Fn==101)||(Fn==103))
			{//F101���߶��� �����й��ܵ���ʾֵ;F103���߶��� �����й��ܵ���ʾֵ
				UARTCtrl->RMLib[0]=0xe8;//ʵʱFn����մ�����ͬ��Fn
				UARTCtrl->RMLib[1]=1;//ͨ�ű�ʶ����
				UARTCtrl->RMLib[2]=0;//����ͷ,0=�ն˳���ʱ��,1=�ն˳���ʱ��+������
				UARTCtrl->RMLib[3]=0x06;//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
				UARTCtrl->RMLib[4]=0x04;//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
				MWR(0x06040001,(u32)&UARTCtrl->RMLib[10],4);//���ݱ�ʶ
				UARTCtrl->RMLib[14]=0;//���س���
				UARTCtrl->RMLib[15]=32;//����ȷ���س���=0ʱ��Ԥ�Ʒ����ֽ���
				MWR(YMDHM,(u32)&UARTCtrl->RMCommandYMDHM[0],5);//���ߵ�ʱ��������ʱ��
				Wait_ReadMeter_AFN0D_RealTime(PORTn,Protocol);//�ȴ�1��Fn����ʵʱ�������
				if(p8FnData[6]!=0xee)
				{//��ȷ��
					ReadFlag=1;//��ȷ����־
					SaveClass2Data(Pn,(u32)p8FnData,(u8*)&UARTCtrl->RMLib);//������2�����㳭������
				}
				break;
			}
			break;
	}

	UARTCtrl->MeterNo=UARTCtrl->MeterNoBackup;//485������ܱ�/��������װ�����(���ֶ���)(���г�����볭��ʵʱ����ʱ����)
	UARTCtrl->FnCount=UARTCtrl->FnCountBackup;//485����Fn����(���г�����볭��ʵʱ����ʱ����)
	UARTCtrl->RMCount=0;//485�����������
	UARTCtrl->ReRMCount=0;//485���������ط�����
	UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	UARTCtrl->Task=0;
	
	i=Get_ADDR_UARTnFnDataBuff(PORTn)+1;
	MR(i,ADDR_TYMDHMS+1,5);//���볭��ʱ��
	if(ReadFlag==0)//��ȷ����־
	{
		return 0;
	}
	return i;//!=0��
}



