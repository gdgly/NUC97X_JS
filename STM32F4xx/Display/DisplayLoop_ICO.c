
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayLoop_ICO.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
//#include "../I2_terminal/Terminal_SwitchIn.h"
//#include "../I2_terminal/Terminal_Contrl_Out.h"
#include "../Display/Display.h"
#include "../Calculate/Calculate.h"
#include "../Device/IC_TESAM.h"




u32 DisplayLoop_TopICO(u32 Row)//��ͼ���ʱ����ʾ
{
#if MainProtocol==376
	u32 i;
	u8 *p8;
  u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;
//#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
//	u32 x;
//#endif

  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(0*2));
//�����źź�����
	p16[0]=0;
	p16[1]=0;
	p16[2]=0;
	p16[3]=0;
	p16[4]=0;
	i=Terminal_Ram->Signal;
	if(i!=0)
	{
		p16[0]=2;//'����'
		switch(i)
		{
			case 1:
				p16[1]=3;//'�ź�ǿ��1'
				break;
			case 2:
				p16[1]=4;//'�ź�ǿ��2'
				break;
			case 3:
				p16[1]=4;//'�ź�ǿ��2'
				p16[2]=5;//'�ź�ǿ��3'
				break;
			case 4:
				p16[1]=4;//'�ź�ǿ��2'
				p16[2]=6;//'�ź�ǿ��4'
				break;
		}
	}
//GPRS����
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
	#if (USER/100)==5//�Ϻ�
		switch(Comm_Ram->CSPN)
		{
			case 0://0=��
				break;
			case 1://1=�й��ƶ�ͨ��-----china mobile  CMCC
				p16[3]=7;//'G'
				p16[4]=8;
				break;
			default://2=�й���ͨͨѶ-----china unicom  CUCC
							//3=�й�����  ------CHINA TELECOM  CTCC
							//255=δ֪
				p16[3]=9;//'C'
				p16[4]=10;
				break;
		}
	#else
		if(Terminal_Ram->MODULSEL==1)//����ģ��ѡ��0=,1=��ΪCDMA MC323,2=GPRS��׼3763,3=ģ���û����
		{
			p16[3]=9;//'C'
			p16[4]=10;
		}
		else
		{
//			if((Comm_Ram->RemoteWirelessModuleType&0x01)!=0)//Զ������ģ������λ��־:b0=GSM����֧��GPRS,b1=WCDMA����,b2=TD-SCDMA����,b3=CDMA 2000,b4=CDMA EVDO,b5=LTE,b6=PSTN,b7=��ʾ��չһ���ֽ�
//			{
//				p16[3]=7;//'G'
//				p16[4]=8;
//			}
//			else
//			{
//				p16[3]=9;//'C'
//				p16[4]=10;
//			}
			#if (USER/100)==0//���Ժ����
				p16[3]=69;//'�п�4G'
				p16[4]=70;
		//		p16[3]=7;//'�п�G'
		//		p16[4]=8;
			#else
				i=Comm_Ram->RemoteWirelessModuleSYSINFO;
				switch(i)//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
				{
					case 4:
						p16[3]=69;//'�п�4G'
						p16[4]=70;
						break;
					case 3:
						p16[3]=67;//'�п�3G'
						p16[4]=68;
						break;
					default:
						p16[3]=65;//'�п�2G'
						p16[4]=66;
						break;
				}
			#endif
		}
	#endif
	}
	
//��̫������
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
	#if (USER/100)==5//�Ϻ�
		p16[3]=30;//30 'E'
		p16[4]=31;
	#else
		p16[3]=11;//11 'L'
		p16[4]=12;
	#endif
	}
	

	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(5*2));
#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=ͨ��ģ��
//����״̬
	p8=(u8 *)(ADDR_Terminal_Protect);
	if(p8[0]!=0)
	{
		p16[0]=13;//����
		p16[1]=14;
	}
#endif
	
//�ļ�����(�뱣�����ͬһλ��������ʾ)
/*
	i=MRR(ADDR_AFN0FF1,1);//ȡ�����ļ���ʶ
	if((i!=0)&&(i<16))
	{
		i=MRR(ADDR_AFN0FF1+2,1);//ȡ�ļ�ָ��
		if(i!=1)
		{//����FTP ��ʽ��װ
			p16[0]=15;//'����'
			p16[1]=16;
		}
	}
*/

//�쳣�澯ָʾ����ʾ�ն˻���������쳣��������ն˷����쳣ʱ���ñ�־����1Hz��Ƶ����˸��ʾ
//#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
//	p8=(u8 *)(ADDR_TYMDHMS);
//	if(p8[0]&0x1)
//	{
//		i=Terminal_Contrl_Round();//�ն˵�ǰ�趨�Ŀ����ִ�(���غϵ��);����B0-B7λ��־
//		p8=(u8 *)(ADDR_ContrlModeData+2);//�ӿ���ģ������Ķ��߱�־�ֽ�0:b0=��·1����,b1=��·1����,b2=��·2����,b3��·2����;(1����,0δ����)
//		x=0;
//		if((p8[0]&0x3)==0x3)
//		{
//			x=1;
//		}
//		if(((p8[0]>>2)&0x3)==0x3)
//		{
//			x|=2;
//		}
//		i&=x;
//		if(i!=0x0)
//		{
//			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(7*2));
//			p16[0]=17;// '�澯'
//			p16[1]=18;
//		}
//	}
//#endif

//���ƽ��
//#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��
////�ն˼��ƽ��
//	p8=(u8 *)(ADDR_Terminal_Period);
//#else
	p8=(u8 *)(ADDR_METER07_CTRL_SRAM);
//#endif
#if (USER/100)==5//���Ϻ�
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"ƽ");//��ʾ�ַ���
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
	}
#else//�����Ϻ�
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"ƽ");//��ʾ�ַ���
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
	}
#endif	
	
//����(����ƽ�ȷ���ͬһλ��������ʾ)
	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		p16=(u16 *)(ADDR_Class2Congeal_AutoRW+12);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		i=p16[0]|p16[1];//ԭ��2���ݶ���AUTOIOWû���
		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(10*2));
		if(((Terminal_Ram->CongealFlags&0x1f)!=0x1f)||(i!=0))//��ʵʱ�ӷָ���ʱ��0;���ڶ����־(0���ڶ���,1�������);B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
		{
			p16[0]=28;// '����ͼ'
			p16[1]=29;
		}
	}
	
//��ǰʱ��
	p8=(u8 *)(ADDR_RTCBUFF);
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
	p16[0]=((p8[2]>>4)&0xf)+0x30+0x2000;
	p16[1]=((p8[2]>>0)&0xf)+0x30+0x2000;
	p16[2]=':'+0x2000;
	p16[3]=((p8[1]>>4)&0xf)+0x30+0x2000;
	p16[4]=((p8[1]>>0)&0xf)+0x30+0x2000;
	p16[5]=':'+0x2000;
	p16[6]=((p8[0]>>4)&0xf)+0x30+0x2000;
	p16[7]=((p8[0]>>0)&0xf)+0x30+0x2000;
//
	Row++;
	return Row;
#endif
	
#if MainProtocol==698
	u32 i;
	u8 *p8;
  u16 *p16;
  UARTCtrl_TypeDef * UARTCtrl;

  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(0*2));
//�����źź�����
	p16[0]=0;
	p16[1]=0;
	p16[2]=0;
	p16[3]=0;
	p16[4]=0;
	i=Terminal_Ram->Signal;
	if(i!=0)
	{
		p16[0]=2;//'����'
		switch(i)
		{
			case 1:
				p16[1]=3;//'�ź�ǿ��1'
				break;
			case 2:
				p16[1]=4;//'�ź�ǿ��2'
				break;
			case 3:
				p16[1]=4;//'�ź�ǿ��2'
				p16[2]=5;//'�ź�ǿ��3'
				break;
			case 4:
				p16[1]=4;//'�ź�ǿ��2'
				p16[2]=6;//'�ź�ǿ��4'
				break;
		}
	}
//GPRS����
	if((UART1Ctrl->LinkTask==101)||(UART1Ctrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
	#if (USER/100)==0//���Ժ����
		p16[3]=69;//'�п�4G'
		p16[4]=70;
//		p16[3]=7;//'�п�G'
//		p16[4]=8;
	#else
		switch(Comm_Ram->RemoteWirelessModuleSYSINFO)//Զ������ģ�鵱ǰע�������:0=No service,1=,2=2G(GPRS,CDMA),3=3G(WCDMA,TD-SCDMA,CDMA EVD0),4=4G(FDD-LTE,TDD-LTE)
		{
			case 4:
				p16[3]=69;//'�п�4G'
				p16[4]=70;
				break;
			case 3:
				p16[3]=67;//'�п�3G'
				p16[4]=68;
				break;
			default:
				p16[3]=65;//'�п�2G'
				p16[4]=66;
				break;
		}
	#endif
	}
	
//��̫������
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ENETPORT);
	if((UARTCtrl->LinkTask==101)||(UARTCtrl->LinkTask==102))//��������0-99��ʼ����,100=��������,101=��¼,102=���˳���¼,103=�˳���¼
	{
	#if (USER/100)==5//�Ϻ�
		p16[5]=30;//30 'E'
		p16[6]=31;
	#else
		p16[5]=11;//11 'L'
		p16[6]=12;
	#endif
	}
	
#if ((Project/100)==3)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=ͨ��ģ��
//����״̬
	p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
	if(p8[1]!=0)
	{
		p16[7]=13;//����
		p16[8]=14;
	}
#endif
#if ((Project/100)==4)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=ͨ��ģ��
//����״̬
	if(Comm_Ram->MODE3in1==0)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
	{
		p8=(u8 *)(ADDR_8001_2);//����-����2������״̬��ֻ����
		if(p8[1]!=0)
		{
			p16[7]=13;//����
			p16[8]=14;
		}
	}
#endif
	
//���ƽ��
	p8=(u8 *)(ADDR_METER07_CTRL_SRAM);
#if (USER/100)==5//���Ϻ�
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"ƽ");//��ʾ�ַ���
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
	}
#else//�����Ϻ�
	switch(p8[0])
	{
		case 1:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 2:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
		case 3:
			DisplayString(Row,10,0,(u8 *)"ƽ");//��ʾ�ַ���
			break;
		case 4:
			DisplayString(Row,10,0,(u8 *)"��");//��ʾ�ַ���
			break;
	}
#endif	
//�쳣�澯ָʾ,���ȼ��ƽ����ʾ
//��ʾ�ն˻���������쳣��������ն˷����쳣ʱ���ñ�־����1Hz��Ƶ����˸��ʾ
//#if (Project/100)==3//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
//	p8=(u8 *)(ADDR_TYMDHMS);
//	if(p8[0]&0x1)
//	{
//		i=Terminal_Contrl_Round();//�ն˵�ǰ�趨�Ŀ����ִ�(���غϵ��);����B0-B7λ��־
//		p8=(u8 *)(ADDR_ContrlModeData+2);//�ӿ���ģ������Ķ��߱�־�ֽ�0:b0=��·1����,b1=��·1����,b2=��·2����,b3��·2����;(1����,0δ����)
//		x=0;
//		if((p8[0]&0x3)==0x3)
//		{
//			x=1;
//		}
//		if(((p8[0]>>2)&0x3)==0x3)
//		{
//			x|=2;
//		}
//		i&=x;
//		if(i!=0x0)
//		{
//			p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(10*2));
//			p16[0]=17;// '�澯'
//			p16[1]=18;
//		}
//	}
//#endif
	
//��ǰʱ��
	p8=(u8 *)(ADDR_RTCBUFF);
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(12*2));
	p16[0]=((p8[2]>>4)&0xf)+0x30+0x2000;
	p16[1]=((p8[2]>>0)&0xf)+0x30+0x2000;
	p16[2]=':'+0x2000;
	p16[3]=((p8[1]>>4)&0xf)+0x30+0x2000;
	p16[4]=((p8[1]>>0)&0xf)+0x30+0x2000;
	p16[5]=':'+0x2000;
	p16[6]=((p8[0]>>4)&0xf)+0x30+0x2000;
	p16[7]=((p8[0]>>0)&0xf)+0x30+0x2000;
//
	Row++;
	return Row;
#endif
}

u32 DisplayLoop_BottomICO(u32 Row)//��ͼ����ʾ
{
	u32 i;
	u32 x;
  u16 *p16;
#if (USER/100)!=5//�Ϻ�
  UARTCtrl_TypeDef * UARTCtrl;
#endif
#if (USER/100)==15//����
	u8 *p8;
	u8 *p8time;
#endif

#if (USER/100)==5//�Ϻ�
//UA UB UC
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4);
	if(Meter_Ram->IPFlags&0x80)
	{//��ѹ������
		DisplayString(Row,4,0,(u8 *)"Uc Ub Ua");//��ʾ�ַ���
		if(Meter_Ram->LPFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"����");//��ʾ�ַ���
			if(Meter_Ram->LPFlags&0x1)
			{//A
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x4)
			{//C
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
		}
		if(Meter_Ram->LVFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"ʧѹ");//��ʾ�ַ���
			if(Meter_Ram->LVFlags&0x1)
			{//A
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x4)
			{//C
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
		}
	}
	else
	{
		DisplayString(Row,4,0,(u8 *)"Ua Ub Uc");//��ʾ�ַ���
		if(Meter_Ram->LPFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"����");//��ʾ�ַ���
			if(Meter_Ram->LPFlags&0x1)
			{//A
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LPFlags&0x4)
			{//C
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
		}
		if(Meter_Ram->LVFlags&0x7)
		{
			DisplayString(Row,0,0,(u8 *)"ʧѹ");//��ʾ�ַ���
			if(Meter_Ram->LVFlags&0x1)
			{//A
				p16[4]|=0X8000;
				p16[5]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x2)
			{//B
				p16[7]|=0X8000;
				p16[8]|=0X8000;
			}
			if(Meter_Ram->LVFlags&0x4)
			{//C
				p16[10]|=0X8000;
				p16[11]|=0X8000;
			}
		}
	}
#else//#if (USER/100)==5//�Ϻ�
//�ն˳���״̬
	i=DOTMIXPMODE->Ln9Code;//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=�򿪴���,2=���ͨ��ģ��,3=���SIM��,4=����ע��,5=��ȡ�ź�,6=��ʼ����,7=��¼��վ,8=��¼��վ�ɹ�
	switch(i)
	{
	#if LCDLn9Code!=0//��Ļ�ײ�(Ln=9)��ʾ���ݴ���:0=�ն˳���״̬,1=GPRS����״̬
		case 1:
			DisplayString(Row,0,0,(u8 *)"�򿪴���");//��ʾ�ַ���
			break;
		case 2:
			DisplayString(Row,0,0,(u8 *)"���ͨ��ģ��");//��ʾ�ַ���
			break;
		case 3:
			DisplayString(Row,0,0,(u8 *)"���SIM��");//��ʾ�ַ���
			break;
		case 4:
			DisplayString(Row,0,0,(u8 *)"����ע��");//��ʾ�ַ���
			break;
		case 5:
			DisplayString(Row,0,0,(u8 *)"��ȡ�ź�");//��ʾ�ַ���
			break;
		case 6:
			DisplayString(Row,0,0,(u8 *)"��ʼ����");//��ʾ�ַ���
			break;
		case 7:
			DisplayString(Row,0,0,(u8 *)"��¼��վ");//��ʾ�ַ���
			break;
		case 8:
			DisplayString(Row,0,0,(u8 *)"��¼��վ�ɹ�");//��ʾ�ַ���
			break;
	#endif
		
		default:
			if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
			{
		#if MainProtocol==376
				//376
				i=0;
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
				if((UARTCtrl->RMFlag!=0)&&(UARTCtrl->FnEnd!=0))//0=��������ͣ,1=���������;485����Fn����������־,0=����,1=û����
				{
					i=1;
				}
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				if((UARTCtrl->RMFlag!=0)&&(UARTCtrl->FnEnd!=0))//0=��������ͣ,1=���������;485����Fn����������־,0=����,1=û����
				{
					i=1;
				}
				if(i!=0)
				{
			#if RMM_RS485_PassageZero==1//RS485ͨ�������0��;0=����ͣ,1=��ͣ
					x=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);
					if((x>=0x2355)||(x<0x0005))
					{//RS485����ÿ��0�㸽����ͣ����
						i=0;
					}
			#endif
				}
			#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
				if(Terminal_Router->RouterRunMode_4==1)//·������ģʽ_���ڳ���ģʽ�� 01��ʾͨ��ģ���֧�ּ��������������ڳ���ģʽ��10��ʾͨ��ģ���֧��·�����������ڳ���ģʽ��11��ʾͨ��ģ������ֳ���ģʽ��֧�֣�00����
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					if((UARTCtrl->RMFlag!=0)&&(UARTCtrl->FnEnd!=0))//0=��������ͣ,1=���������;485����Fn����������־,0=����,1=û����
					{
						i=1;
					}
				}
				else
				{
					//��ͣ·�ɱ�־:
					//b0=��������ʱ,b1=���ص㻧,b2=����ת��,b3=UART��·����Ϣ,b4=�㲥Уʱ,b5=�㲥����,b6=ͨ�ϵ����,b7=ʵʱ����
					//b8=����ͬ��,b9=������ʱ,b10=������ͣ����,b11=�������Զ�����,b12=�ǳ���ʱ��,b13=�Զ��ѱ�,b14=�ȴ�ʱ�ӹ�0,b15=���ڶ���
					//b16=·���������,b17=,b18=,b19=,b20=,b21=,b22=,b23=
					if(((Terminal_Router->StopFlags&0xFFFFFF7D)==0)&&(Terminal_Router->RouterInitTask>=200)&&(Terminal_Router->NoteNum!=0))
					{
						i=1;
					}
				}
			#endif
		#else
			//698
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
				i=UARTCtrl->TaskID;
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
				i|=UARTCtrl->TaskID;
			#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
				UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
				i|=UARTCtrl->TaskID;
			#endif
			#if (Project/100)==4//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=����������,600=ͨ��ģ��,700=
				if(Comm_Ram->MODE3in1==1)//����һ�ն˵�ǰ����ģʽ:0=ר��,1=������
				{
					UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
					i|=UARTCtrl->TaskID;
				}
			#endif
				i>>=8;
		#endif
				if(i!=0)
				{
					DisplayString(Row,0,0,(u8 *)"�ն����ڳ���...");//��ʾ�ַ���
				}
			}
			break;
	}
#endif//#if (USER/100)==5//�Ϻ�
	
//���ü�
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(14*2));
#if KEY_SET==1//���ð���0=û,1=��
	#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��
	if(MRR(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
	#else
	if(MRR(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
	#endif
#endif
//��Կ

	
#if (USER/100)==15//����
	p8=(u8*)ADDR_UART7Rx;
	MR(ADDR_DATABUFF,ADDR_MYMDHMS,6);
	p8time=(u8*)ADDR_DATABUFF;
	if((p8time[0]%2)==0)
	{
		TESAM_CMD_TxRx(0x803600FF,0,0);
		if(p8[17]==0)//��Կ
		{
			p16[0]=103;
			p16[1]=104;
		}
	}	
#endif
//����ģʽ(���������ü�)
	if(Comm_Ram->Factory==0x55)//����״̬
	{
		p16[0]=25;// '����ģʽ'
		p16[1]=26;
		

	}
//ʱ�ӵ��Ƿѹ
	if(Comm_Ram->URTC<0x200)
	{//С��2.00V
		if(Comm_Ram->RTCBuff[0]&1)
		{
	  	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(16*2));
			p16[0]=21;//'���Ƿѹ'
			p16[1]=22;
		}
	}
//�ɳ����
	x=Comm_Ram->UBACKUP;
	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		x=bcd_hex(x);
		if(x>=30)
		{
			x-=30;//�����������ڳ��-0.30V
		}
		else
		{
			x=0;
		}
		x=hex_bcd(x);
	}
	i=38;//38'���0��ͼ'
	if(x>=RechargeableBatteryGrid1)
	{
  	i=40;//40'���1��ͼ'
	}
	if(x>=RechargeableBatteryGrid2)
	{
  	i=42;//42'���2��ͼ'
	}
	if(x>=RechargeableBatteryGrid3)
	{
  	i=44;//44'���3��ͼ'
	}
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(18*2));
	p16[0]=i;
	p16[1]=i+1;

	Row++;
	return Row;
}
/*
u32 DisplayLoop_BottomICO(u32 Row)//��ͼ����ʾ
{
	u32 i;
	u32 x;
  u16 *p16;
	
//״̬�������λ��δ��λ
	x=MRR(ADDR_AFN0CF9+1,1);//D0��D7��λ��Ӧ1��8·״̬���ı�λCD����"O"����ǰ��ң�Ŵ��ͺ���״̬�仯����"1"����ǰ��ң�Ŵ��ͺ�������һ��״̬�仯
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(4*2));
	//for(i=0;i<Get_SwitchIn_Route();i++)
	for(i=0;i<8;i++)
	{
		if((x&(1<<i))==0)
		{
			p16[0]='-'+0x2000;
		}
		else
		{
			p16[0]=27;//���¼�ͷ
		}
		p16++;
	}
//ʱ�ӵ��Ƿѹ
	if(Comm_Ram->URTC<0x200)
	{//С��2.00V
		if(Comm_Ram->YMDHMS[0]&1)
		{
	  	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(14*2));
			p16[0]=21;//'���Ƿѹ'
			p16[1]=22;
		}
	}
//�ɳ����
	x=Comm_Ram->UBACKUP;
	if(Comm_Ram->POWER==3)//��Դ:0=��ع���,1=��������ת��ع���������,2=��������ת��ع���,3=��������
	{
		x=bcd_hex(x);
		if(x>=30)
		{
			x-=30;//�����������ڳ��-0.30V
		}
		else
		{
			x=0;
		}
		x=hex_bcd(x);
	}
	i=38;//38'���0��ͼ'
	if(x>=RechargeableBatteryGrid1)
	{
  	i=40;//40'���1��ͼ'
	}
	if(x>=RechargeableBatteryGrid2)
	{
  	i=42;//42'���2��ͼ'
	}
	if(x>=RechargeableBatteryGrid3)
	{
  	i=44;//44'���3��ͼ'
	}
	p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(16*2));
	p16[0]=i;
	p16[1]=i+1;
//���ü�
  p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(18*2));
#if ((Project/100)<5)//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=����һ�ն�,500=�ɼ���,600=ͨ��ģ��
	if(MRR(ADDR_MeterFMTimer,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
#else
	if(MRR(ADDR_TerminalFMTimer+12,2))
	{
		p16[0]=23;// '��̰���'
		p16[1]=24;
	}
#endif
//����ģʽ(���������ü�)
	if(Comm_Ram->Factory==0x55)//����״̬
	{
		p16[0]=25;// '����ģʽ'
		p16[1]=26;
	}
	Row++;
	return Row;
}
*/



