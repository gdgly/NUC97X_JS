
//ʵʱ����1������
#include "Project.h"
#include "Terminal_ReadMeter_AFN0C_RealTime.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"

#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_DL645_1997_Lib.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "Terminal_ReadMeter_DL645_1997.h"
#include "Terminal_ReadMeter_3762_Std.h"

#include "Terminal_ReadMeter_WSH41_Lib.h"
#include "Terminal_ReadMeter_WSH41.h"
#include "Terminal_ReadMeter_ShH_Lib.h"
#include "Terminal_ReadMeter_ShH.h"

#if ((Project/100)==3)&&((USER/100)==12)//�Ĵ�ר��
	#include "Terminal_ReadMeter_HX_6E_Lib.h"	 
	#include "Terminal_ReadMeter_HX_6E.h"		 
	extern const u32 HX_6E_RMLib[];
#endif
#include "Terminal_ReadMeter_DL698_Lib.h"
#include "Terminal_ReadMeter_DL698.h"



extern const u32 DL645_2007_RMLib[];
extern const u32 DL645_1997_RMLib[];
extern const u32 WSH41_RMLib[];
extern const u32 ShH_RMLib[];
extern const u32 DL698_RMLib[];
u32 Wait_ReadMeter_RealTime(u32 PORTn,u32 PORTRS485,u32 Fn,u32 Protocol)//�ȴ�1��Fn����ʵʱ�������
{
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
	u16 *p16;
#endif

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
	p32=(u32 *)(ADDR_MS_ENTRY);
	
	switch(Protocol)
	{
		case 1://DL/T645-1997(1)
			UARTCtrl->FnCount=Get_RM_DL645_1997_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	#if ((Project/100)==3)&&((USER/100)==12)//�Ĵ�ר��
		case 4:
			UARTCtrl->FnCount=Get_RM_HX_6E_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;			
	#endif
	#if (USER/100)==10//�ӱ�ר��
		case 7://��ʤ��Լ(�ӱ�)
			UARTCtrl->FnCount=Get_RM_WSH41_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	#endif
	#if (USER/100)==5//�Ϻ�ר��
		case 21://�Ϻ���Լ
			UARTCtrl->FnCount=Get_RM_ShH_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
		case 3://DL698��Լ
			UARTCtrl->FnCount=Get_RM_DL698_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	#endif
		default://��������ΪDL/T645-2007
			UARTCtrl->FnCount=Get_RM_DL645_2007_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	}
	if(UARTCtrl->FnCount==0xff)
	{
		return 0;//û�õ�Fnֵ����Ӧ���������
	}
	switch(PORTRS485)
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
						Terminal_ReadMeter_DL645_1997_Fn(PORTRS485,(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
					#else
						if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
						{
		    			p32=(u32 *)(ADDR_E1_ENTRY);
		    			(*(void(*)())p32[102])(PORTRS485,(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount]);//102 ����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
							p32=(u32 *)(ADDR_I0_ENTRY);
						}
						else
						{
							return 0;
						}
					#endif
						break;
				#if ((Project/100)==3)&&((USER/100)==12)//�Ĵ�ר��
					case 4:	
						Terminal_ReadMeter_HX_6E_Fn(PORTRS485,(u8 *)HX_6E_RMLib[UARTCtrl->FnCount]);
						break;
				#endif
				#if (USER/100)==10//�ӱ�ר��
					case 7://��ʤ��Լ(�ӱ�)
						Terminal_ReadMeter_WSH41_Fn(PORTRS485,(u8 *)WSH41_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
						break;
				#endif
				#if (USER/100)==5//�Ϻ�ר��
					case 21://�Ϻ���Լ
					#if Extern_Terminal_ReadMeter_ShH_Fn==0//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
						Terminal_ReadMeter_ShH_Fn(PORTRS485,(u8 *)ShH_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
					#else
						if((Comm_Ram->SoftModule&(1<<4))==0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
						{
		    			p32=(u32 *)(ADDR_E1_ENTRY);
		    			(*(void(*)())p32[113])(PORTRS485,(u8 *)ShH_RMLib[UARTCtrl->FnCount]);//113 ����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
							p32=(u32 *)(ADDR_I0_ENTRY);
						}
						else
						{
							return 0;
						}
					#endif
						break;
					case 3://698��Լ
						Terminal_ReadMeter_DL698_Fn(PORTRS485,(u8 *)DL698_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
						break;
				#endif
					default://��������ΪDL/T645-2007
						Terminal_ReadMeter_DL645_2007_Fn(PORTRS485,(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
						break;
				}
				
				(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
			#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
				if(Terminal_Ram->PowerTask<=10)//ͣ���¼�����
				{
					return 0;
				}
			#endif
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
					Terminal_Router->RouterRealTimeTask=0;//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
					return 0;
				}
				if(Terminal_Router->RouterCtrl==0)//·��������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
				{
					switch(Protocol)
					{
						case 1://DL/T645-1997(1)
							RealTimeReadMeter_3762_Fn_Std(PORTRS485,(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;RouterRealTimeTask=0=����(��Fn�������)
							break;
					#if (USER/100)==5//�Ϻ�ר��
						case 21://�Ϻ���Լ
							RealTimeReadMeter_3762_Fn_Std(PORTRS485,(u8 *)ShH_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;RouterRealTimeTask=0=����(��Fn�������)
							break;
					#endif
						default://DL/T645-2007(30),���нӿ�����խ����ѹ�ز�(31)
							RealTimeReadMeter_3762_Fn_Std(PORTRS485,(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount]);//����1��Fn������д��FnDataBuff;RouterRealTimeTask=0=����(��Fn�������)
							break;
					}
					if(UARTCtrl->Task==2)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
					{
					#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
						p16=(u16 *)Get_ADDR_UARTn(PORTn);
						DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
						if(p16[0]!=p16[1])
						{//�ѽ��յ���֡
							if(Terminal_Router->StopFlags&(1<<7))//b7=ʵʱ����
							{
								Terminal_Router->StopFlags&=~(1<<7);//�ָ�·��
								Terminal_Router->RouterCtrl=3;//·������������0=��,1=����,2=��ͣ,3=�ָ�,4=���;��ɺ��Զ��ص�0
							}
							Terminal_Router->RouterRealTimeTask=0;//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
							return 0;//0=û��
						}
					#endif
					}
				}
				(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
			#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
				if(Terminal_Ram->PowerTask<=10)//ͣ���¼�����
				{
					Terminal_Router->RouterRealTimeTask=0;//�ز�Fn>=129��ʵʱ��������;0=����,1=��ͣ·��,2=����,3=����
					return 0;
				}
			#endif
			}
	#endif		
			break;
	}
	return 1;
}

u32 ReadMeter_RealTime(u32 PORTn,u32 Fn,u32 Pn)//ʵʱ��ȡ1������;����0=û��,!=0������FnDataBuff��ַ
{
	u32 i;
	u32 MeterNo;//���ܱ��������
	u32 PORTRS485;
	u32 Protocol;
  UARTCtrl_TypeDef * UARTCtrl;
	u32 *p32;
#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
	u16 *p16;
#endif

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
			PORTRS485=RS485_1PORT;
			break;
		case 3://485-2
			PORTRS485=RS485_2PORT;
			break;
		case 4://485-3
			PORTRS485=RS485_3PORT;
			break;
		case 31://�ز�
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
			if(Terminal_Router->RouterInitTask>=200)//·������������0-99������ʼ��,100-199�ֳ��ҳ�ʼ��,>=200·�ɳ���
			{
				PORTRS485=RS485_4PORT;
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
	Protocol=MRR(ADDR_AFN04F10+((MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//ͨ��Э������ BIN 1 
	switch(Protocol)
	{
		case 1://DL/T645-1997(1)
		#if Extern_Terminal_ReadMeter_DL645_1997_Fn==1//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
			if((Comm_Ram->SoftModule&(1<<4))!=0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				return 0;//0=û��
			}
		#endif
			i=Get_RM_DL645_1997_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	#if ((Project/100)==3)&&((USER/100)==12)//�Ĵ�ר��
		case 4:
			i=Get_RM_HX_6E_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	#endif
	#if (USER/100)==10//�ӱ�ר��
		case 7://��ʤ��Լ(�ӱ�)
			i=Get_RM_WSH41_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	#endif
	#if (USER/100)==5//�Ϻ�ר��
		case 21://�Ϻ���Լ
		#if Extern_Terminal_ReadMeter_ShH_Fn==1//�ⲿ����:0=I2,1=E1,2=E2,3=;����1��Fn������д��FnDataBuff;Task=0=����(��Fn�������)
			if((Comm_Ram->SoftModule&(1<<4))!=0)//���ģ������־(��λ��ʾ����):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				return 0;//0=û��
			}
		#endif
			i=Get_RM_ShH_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
		case 3://698��Լ
			i=Get_RM_DL698_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	#endif
		default://��������ΪDL/T645-2007
			i=Get_RM_DL645_2007_FnCount(Fn);//�õ�Fnֵ����Ӧ���������;����0xffffffff��ʾû�ҵ�
			break;
	}

	if(i==0xffffffff)
	{//û�õ�Fnֵ����Ӧ���������
		return 0;//0=û��
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
	p32=(u32 *)(ADDR_MS_ENTRY);
#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
	p16=(u16 *)Get_ADDR_UARTn(PORTn);
	DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
	if(p16[0]!=p16[1])
	{//�ѽ��յ���֡
		return 0;//0=û��
	}
#endif
	while((UARTCtrl->Lock==0x55)||((UARTCtrl->Task!=0)&&(UARTCtrl->Task!=2)))//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{//�����ͣʱ,�ȴ�����ģ��������ϱ�����볬ʱ״̬
		(*(void(*)())p32[115])();//����I0_Main_Return;63 ����������
		if(PORTRS485==RS485_4PORT)
		{
			if(UARTCtrl->Lock==0x55)
			{
				return 0;//0=û��
			}
		}
	#if MulRealRead==1//��֡�·�ʵʱ����������ת��:0=ÿ֡������Ӧ,1=����ǰ��ֻ֡�������֡��������֡�����������ȵ�ȷ��֡
		DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
		if(p16[0]!=p16[1])
		{//�ѽ��յ���֡
			return 0;//0=û��
		}
	#endif
	#if NewERC14==1//ERC14ͣ���¼���¼����0=ԭ��׼,1=����Ӫ����2014.1.15֪ͨ
		if(Terminal_Ram->PowerTask<=10)//ͣ���¼�����
		{
			return 0;
		}
	#endif
	}
	//
	UARTCtrl->Lock=0x55;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	UARTCtrl->MeterNoBackup=UARTCtrl->MeterNo;//485������ܱ�/��������װ�����(���ֶ���)(���г�����볭��ʵʱ����ʱ����)
	UARTCtrl->MeterNo=MeterNo;
	UARTCtrl->FnCountBackup=UARTCtrl->FnCount;//485����Fn����(���볭��ʵʱ����ʱ����)
	UARTCtrl->RMCountBackup=UARTCtrl->RMCount;//485�����������(���볭��ʵʱ����ʱ����)
	
	i=Wait_ReadMeter_RealTime(PORTn,PORTRS485,Fn,Protocol);//�ȴ�1��Fn����ʵʱ�������
	
	UARTCtrl->MeterNo=UARTCtrl->MeterNoBackup;//485������ܱ�/��������װ�����(���ֶ���)(���볭��ʵʱ����ʱ����)
	UARTCtrl->FnCount=UARTCtrl->FnCountBackup;//485����Fn����(���볭��ʵʱ����ʱ����)
	UARTCtrl->RMCount=UARTCtrl->RMCountBackup;//485�����������(���볭��ʵʱ����ʱ����)
	UARTCtrl->SubsequentFrames=0;//0=�޶�����֡,1-255=DL645����֡��֡��
	if((UARTCtrl->FnCount<2)||(UARTCtrl->FnCount>3))
	{//ԭ���ǳ����ܱ��¼�
		UARTCtrl->RMCount=0;//485�����������
	}
	UARTCtrl->ReRMCount=0;//485���������ط�����
	UARTCtrl->Lock=0;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	UARTCtrl->Task=0;
	if(i==0)
	{
		return 0;
	}
	return (Get_ADDR_UARTnFnDataBuff(PORTRS485)+1);//!=0��
}




