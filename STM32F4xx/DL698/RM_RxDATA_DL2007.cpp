
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_TxDATA.h"
#include "../DL698/MeterAutoReport.h"
#include "../DL698/RM_Event_DL2007.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../DL698/EVENT_Record.h"


#include "../DL698/DL698_JSON.h"

u32 RM_RxDATA_DL2007_CHECK(u32 PORTn)//DL645_2007֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����:0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
{
	u32 i;
	u32 x;
	u8* p8rx;
	u8* p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8=p8;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(PORTn==ACSAMPLEPORT)
	{
		p8rx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	}else
	{
		p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	}
	
/*
#if ((Project/100)==2)//������
#if SEL_RouterRunMode3ADDR==1//΢�������߷���֡Ŀ���ַ:0=���ܱ��ַ��1=�ɼ�����ַ
	if((PORTn==RS485_4PORT)&&(Terminal_Router->RouterRunMode_1==3))//·������ģʽ_ͨ�ŷ�ʽ��1��ʾ"խ���������ز�ͨ��"��2��ʾ"����������ز�ͨ��"��3��ʾ"΢��������ͨ��"������ȡֵ����
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
		p8=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
		p8=Get_Element(p8,3,0,0);//����Ԫ�ص�ַ
		if(p8)
		{//�ɼ�����ȡ
			p8+=5;
			x=p8[4]|(p8[5]<<8);
			i=(p8rx[1]<<8)|p8rx[2];
			if(i!=x)
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			x=p8[2]|(p8[3]<<8);
			i=(p8rx[3]<<8)|p8rx[4];
			if(i!=x)
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			x=p8[0]|(p8[1]<<8);
			i=(p8rx[5]<<8)|p8rx[6];
			if(i!=x)
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
		}
		else
		{
			return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
		}
	}
	else
#endif
#endif
*/
	{
		if(PORTn!=ACSAMPLEPORT)
		{//���ǽ���
			i=(p8rx[1]<<8)|p8rx[2];
			if(i!=UARTCtrl->RMAddr[2])
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			i=(p8rx[3]<<8)|p8rx[4];
			if(i!=UARTCtrl->RMAddr[1])
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
			i=(p8rx[5]<<8)|p8rx[6];
			if(i!=UARTCtrl->RMAddr[0])
			{
				return 1;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
			}
		}
	}
	
	//������
	if((p8rx[8]&0x40)!=0x0)
	{//��վ�쳣�ش�
		UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
		return 2;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
	//��0x33
	for(i=0;i<p8rx[9];i++)
	{
		p8rx[10+i]-=0x33;
	}
	//DI�Ƚ�
	x=p8rx[10];
	x|=p8rx[10+1]<<8;
	x|=p8rx[10+2]<<16;
	x|=p8rx[10+3]<<24;
	if(x==0x04001501)
	{//�����ϱ�״̬��
		UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
		#if EnMeterAutoReportEvent==1//������ܱ������ϱ��¼�:0=������,1=����
			MeterAutoReportStateWord(PORTn,p8rx);//�����ϱ�״̬��;���:�Զ��ϱ�����֡;����:0=û����,1=����
		#endif
		//���ݴ������,����:4�����ϱ�(�Ѵ���)
		return 4;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
	i=UARTCtrl->RM_DI;//DL/T645-2007/1997��ǰ�������ݱ�ʶ
	switch(i)
	{
	#if (USER/100)==15//����
		case 0x060000FF://ȫ��6�ฺ������(����)
	#endif
		case 0x06000001://ȫ��6�ฺ������
		case 0x06010001://1�ฺ������(��ѹ��������Ƶ��)
		case 0x06020001://2�ฺ������(���޹�����)
		case 0x06030001://3�ฺ������(��������)
		case 0x06040001://4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
		case 0x06050001://5�ฺ������(1-4�����޹��ܵ���)
		case 0x06060001://6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
			//����������,������ִ�м��ʱ��Ͳɼ������ɼ����ʱ�����
			break;
		default:
			i=RM_TxDATA_DL2007_DI(PORTn,i);//������֡DI����n�θ���
			break;
	}
	if(x!=i)
	{
		return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
	return 0;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
}


__align(4) const u32 signed_DL2007_List[]=//DL/T-2007���ݴ����������ŵ����ݱ�ʶ�б�
{
//1��������
	0x0000FFFF,//����й��ܵ���
	0x0003FFFF,//����޹�1�ܵ���
	0x0004FFFF,//����޹�2�ܵ���
	0x001700FF,//A������޹�1����
	0x001800FF,//A������޹�2����
	0x002B00FF,//B������޹�1����
	0x002C00FF,//B������޹�2����
	0x003F00FF,//C������޹�1����
	0x004000FF,//C������޹�2����
	0x008000FF,//�����ܵ���
	0x009400FF,//A������ܵ���
	0x00A800FF,//B������ܵ���
	0x00BC00FF,//C������ܵ���

//2�������������ʱ����
	0x0103FFFF,//����޹�1���������������ʱ��
	0x0104FFFF,//����޹�1���������������ʱ��
	0x011700FF,//A������޹�1�������������ʱ��
	0x011800FF,//A������޹�2�������������ʱ��
	0x012B00FF,//B������޹�1�������������ʱ��
	0x012C00FF,//B������޹�2�������������ʱ��
	0x013F00FF,//C������޹�1�������������ʱ��
	0x014000FF,//C������޹�2�������������ʱ��

//3����
	0x0202FF00,//����
	0x02800001,//���ߵ���
	0x0203FF00,//˲ʱ�й�����
	0x0204FF00,//˲ʱ�޹�����
	0x0205FF00,//˲ʱ���ڹ���
	0x0206FF00,//��������
	0x02800004,//��ǰ�й�����
	0x02800005,//��ǰ�޹�����
	0x02800006,//��ǰ��������
	0x02800007,//�����¶�

//4�¼���¼

//5�α�������

//6��������
	0x050003FF,//��ʱ��������޹�1��������
	0x050004FF,//��ʱ��������޹�2��������
	0x050103FF,//˲ʱ��������޹�1��������
	0x050104FF,//˲ʱ��������޹�2��������
	0x050203FF,//����ʱ�����л�����޹�1��������
	0x050204FF,//����ʱ�����л�����޹�2��������
	0x050303FF,//������ʱ�α��л�����޹�1��������
	0x050304FF,//������ʱ�α��л�����޹�2��������
	0x05061001,//�ն����й��޹�����(��/A/B/C�й�����,��/A/B/C�޹�����)

//7���ɼ�¼����
};

u32 signed_DL2007_DI(u32 PORTn)//DL/T-2007���ݴ����������ŵ����ݱ�ʶ;���:UARTCtrl->RM_DI;����:0=û���ţ�1=�з���
{
	u32 i;
	u32 x;
	u32 RMDI32;
	u32 RMDI1;
	u32 RMDI0;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->RM_DI;
	RMDI0=i&0xff;
	RMDI1=(i>>8)&0xff;
	RMDI32=(i>>16);
	for(i=0;i<(sizeof(signed_DL2007_List)/4);i++)
	{
		x=signed_DL2007_List[i];
		x>>=16;
		if(RMDI32==x)
		{
			x=signed_DL2007_List[i];
			x>>=8;
			x&=0xff;
			if((x==0xff)||(x==RMDI1))
			{
				x=signed_DL2007_List[i];
				x&=0xff;
				if((x==0xff)||(x==RMDI0))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}


//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     512
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�����ݳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������)(ROADʱ��д���ܹ�����OAD��������!!!)

u32 RM_RxDATA_DL2007(u32 PORTn)//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
{
	u32 i;
	u32 n;
	u32 OI;
	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u32 signedDI;
	u8* p8Array;
	u8* p8Data;
	u16* p16Data;
	u8* p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 offset;
	#if(USER/100)==15//����
	u32 s;
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ��峤��
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8Data=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8Data+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//��
		return 3;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data+=LEN_RM_DI_CSD+4+LEN_Data1;
	p8Array=(u8*)p8Data;
	if(UARTCtrl->RMCSDSubCount==0)//����CSD�Ӽ���(��DL698����ʱ)
	{
		p16Data[0]=0;//����д��buff����û����һ��CSD�����ݳ���(����������2�ֽ�)
	}
	LEN_Data0=p16Data[0];
	if(LEN_RxDataBuff<LEN_Data0)
	{//��
		return 1;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	LEN_RxDataBuff-=LEN_Data0;

	//������(������)
	if(UARTCtrl->RM_Rx_date_NULL!=0)//������շ���:0=������,!=0������(������)
	{
//		if(UARTCtrl->RMCSD[1]==0)
//		{//OAD
//			LEN_Rx=1;
//		}
//		else
//		{//ROAD
//			LEN_Rx=UARTCtrl->RMCSD[6];
//		}	
//		if(LEN_RxDataBuff<LEN_Rx)
//		{//���峤�Ȳ���
//			return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
//		}
//		for(i=0;i<LEN_Rx;i++)
//		{
//			p8Data[i]=0;
//		}
//		p16Data[0]=LEN_Rx;
		UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
		UARTCtrl->RMROADCount=UARTCtrl->RMROADCountMax;
		return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	if(PORTn==ACSAMPLEPORT)
	{
		p8rx=(u8 *)Get_ADDR_UARTnTx(PORTn);
	}else
	{
		p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	}
	
	LEN_Rx=p8rx[9];
	# if(USER/100)==15//����
	if(LEN_Rx<4)
	{//����<4
		return 0;
	}
	if(LEN_Rx==4)
	{//����==4����ʾ������
		if(UARTCtrl->RMCSD[1]==0)
		{//OAD
			LEN_Rx=1;
		}
		else
		{//ROAD
			LEN_Rx=UARTCtrl->RMCSD[6];
		}	
		if(LEN_RxDataBuff<LEN_Rx)
		{//���峤�Ȳ���
			return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		for(i=0;i<LEN_Rx;i++)
		{
			p8Data[i]=0;
		}
		p16Data[0]=LEN_Rx;
		UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
		UARTCtrl->RMROADCount=UARTCtrl->RMROADCountMax;
		return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	#else
	if(LEN_Rx<=4)
	{//����<4
		return 0;
	}
	#endif
	
//---����֡---
	if((PORTn==RS485_1PORT)||(PORTn==RS485_2PORT))
	{//����֡��־
		if(UARTCtrl->RM_SubsequentFrames==0)//0=�޶�����֡,1-n=����֡��֡��
		{//�Ǻ���֡
			if(p8rx[8]&0x20)
			{//�к���֡
			#if EnMeterAutoReportEvent==1//������ܱ������ϱ��¼�:0=������,1=����
				UARTCtrl->RM_SubsequentFrames++;//0=�޶�����֡,1-n=����֡��֡��
			#endif
			}
		}
		else
		{
			UARTCtrl->RM_SubsequentFrames=0;//0=�޶�����֡,1-n=����֡��֡��
			//����֡���ݴ���
			
			return 0;
		}
	}
//---����֡����---
	
//---���߿�ʼ---
	OI=p8rx[10+0]|(p8rx[10+1]<<8)|(p8rx[10+2]<<16)|(p8rx[10+3]<<24);
	switch(OI)
	{
		case 0x06000001://ȫ��6�ฺ������
	#if (USER/100)==15//����
		case 0x060000FF://ȫ��6�ฺ������(����)
	#endif
//���ɼ�¼��ʼ�룺A0H��A0H���� E0H��E0H�������ݿ鲻��ȷ����2 �ֽڣ� 
//���ɼ�¼�ֽ�����1 �ֽڣ�ʮ�����ƣ��� 
//���ɼ�¼�洢ʱ�䣺�ꡢ�¡��ա�ʱ����,  5 �ֽڣ�
		
//��ѹ��������Ƶ�ʣ�17 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//�С��޹����ʣ�24 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//����������8 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//�С��޹��ܵ��ܣ�16 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ� 
//�������޹��ܵ��ܣ�16 �ֽڣ�
//��ָ��룺AAH��1 �ֽڣ�
//��ǰ������6 �ֽڣ� 
//��ָ��룺AAH��1 �ֽڣ�
//���ɼ�¼�ۼ�У���룺1 �ֽڣ��ӵ�һ�� A0H ��ʼ�����һ�����ݿ�����룩��
//���ɼ�¼�����룺E5H,1 �ֽڡ�
			break;
		case 0x06010001://1�ฺ������(��ѹ��������Ƶ��)
			break;
		case 0x06020001://2�ฺ������(���޹�����)
			if(p8rx[14+8]!=0xAA)
			{//ǰ���޷ָ���AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,25);
				p8rx[9]+=1;
				p8rx[14+8]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+1,25);
			}
			break;
		case 0x06030001://3�ฺ������(��������)
			if(p8rx[14+8]!=0xAA)
			{//ǰ���޷ָ���AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,9);
				p8rx[9]+=2;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+2,9);
			}
			break;
		case 0x06040001://4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
			if(p8rx[14+8]!=0xAA)
			{//ǰ���޷ָ���AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,17);
				p8rx[9]+=3;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				p8rx[14+10]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+3,17);
			}
			break;
		case 0x06050001://5�ฺ������(1-4�����޹��ܵ���)
			if(p8rx[14+8]!=0xAA)
			{//ǰ���޷ָ���AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,17);
				p8rx[9]+=4;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				p8rx[14+10]=0xAA;
				p8rx[14+11]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+4,17);
			}
			break;
		case 0x06060001://6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
			if(p8rx[14+8]!=0xAA)
			{//ǰ���޷ָ���AA
				MR(ADDR_DATABUFF,(u32)p8rx+14+8,7);
				p8rx[9]+=5;
				p8rx[14+8]=0xAA;
				p8rx[14+9]=0xAA;
				p8rx[14+10]=0xAA;
				p8rx[14+11]=0xAA;
				p8rx[14+12]=0xAA;
				MW(ADDR_DATABUFF,(u32)p8rx+14+8+5,7);
			}
			break;
		default:
			OI=0;
			break;
	}
	if(OI)
	{//������
		if(UARTCtrl->RMCSD[1]==0)
		{//��ROAD��
			return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		LEN_Rx=p8rx[9];
		if(LEN_Rx<(4+8))
		{
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		p8Data+=LEN_Data0;
		for(n=0;n<UARTCtrl->RMCSD[6];n++)
		{
			offset=4+8;
			p8Array=(u8*)p8Data;
			OI=(UARTCtrl->RMCSD[7+(4*n)]<<24)|(UARTCtrl->RMCSD[7+(4*n)+1]<<16)|(UARTCtrl->RMCSD[7+(4*n)+2]<<8)|(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{//�������ǡ�00100201���͡�00200201������վ�á�00100200���͡�00200200���ٶ������ʵ��������ǿգ�ֻ���ܵ������ؾ�����
				case 0x00100201://�����й�������
				case 0x00200201://�����й�������
				case 0x00300201://����޹�1������
				case 0x00400201://����޹�2������
				case 0x00500201://��1�����޹�������
				case 0x00600201://��2�����޹�������
				case 0x00700201://��3�����޹�������
				case 0x00800201://��4�����޹�������
				case 0x20000201://A���ѹ
				case 0x20010201://A�����
				case 0x20040202://A���й�����
				case 0x20050202://A���޹�����
				case 0x200A0202://A�๦������
					UARTCtrl->RMCSD[7+(4*n)+3]=0;
					break;
			}
			if(OI==0x20010400)
			{//���ߵ�����645�����߶��������ߵ���
				if(LEN_RxDataBuff<1)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Data[0]=0;
				p8Data+=1;
				LEN_Data0+=1;
				LEN_RxDataBuff-=1;
				continue;
			}
			OI>>=16;
			switch(OI)
			{
				case 0x2021://����ʱ��
					if(LEN_RxDataBuff<8)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data[0]=DataType_date_time_s;
					i=p8rx[14+7];//��
					i=bcd_hex(i);
					i+=2000;
					p8Data[1]=i>>8;
					p8Data[2]=i;
					i=p8rx[14+6];//��
					i=bcd_hex(i);
					p8Data[3]=i;
					i=p8rx[14+5];//��
					i=bcd_hex(i);
					p8Data[4]=i;
					i=p8rx[14+4];//ʱ
					i=bcd_hex(i);
					p8Data[5]=i;
					i=p8rx[14+3];//��
					i=bcd_hex(i);
					p8Data[6]=i;
					p8Data[7]=0;//��
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					break;
				case 0x2000://��ѹ
				case 0x2001://����
				case 0x200f://����Ƶ��
			//��ѹ��������Ƶ�ʣ�17 �ֽڣ� 
			//��ָ��룺AAH��1 �ֽڣ�
					if((LEN_Rx<(offset+17))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x2000://��ѹ
							//DL2007���ݸ�ʽ:XXX.X
							if(LEN_RxDataBuff<(2+(3*3)))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							
							p8Array[0]=DataType_array;
							p8Array[1]=0;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							for(i=0;i<3;i++)
							{
								Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8);
								if(Data==0xffff)
								{
									offset+=2;
									continue;
								}
								Data=bcd_hex(Data);
								p8Data[0]=DataType_long_unsigned;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								offset+=2;
								p8Data+=3;
								LEN_Data0+=3;
								LEN_RxDataBuff-=3;
								p8Array[1]++;//����������
							}
							break;
						case 0x2001://����
							offset+=6;
							//DL2007���ݸ�ʽ:XXX.XXX
							if(LEN_RxDataBuff<(2+(3*5)))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Array[0]=DataType_array;
							p8Array[1]=0;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							for(i=0;i<3;i++)
							{
								Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16);
								if(Data==0xffffff)
								{
									offset+=3;
									continue;
								}
								if(Data>>23)
								{//��
									Data<<=9;
									Data>>=9;
									Data=bcd_hex(Data);
									Data=~Data;
									Data+=1;
								}
								else
								{
									Data=bcd_hex(Data);
								}
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								offset+=3;
								p8Data+=5;
								LEN_Data0+=5;
								LEN_RxDataBuff-=5;
								p8Array[1]++;//����������
							}
							break;
						case 0x200f://����Ƶ��(�������ͣ�long-unsigned����λ��Hz�����㣺-2)
							offset+=6+9;
							//DL2007���ݸ�ʽ:XXX.XXX
							if(LEN_RxDataBuff<3)
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8);
							if(Data==0xffff)
							{
								p8Data[0]=0;
								p8Data+=1;
								LEN_Data0+=1;
								LEN_RxDataBuff-=1;
								break;
							}
							Data=bcd_hex(Data);
							p8Data[0]=DataType_long_unsigned;
							p8Data[1]=Data>>8;
							p8Data[2]=Data;
							offset+=2;
							p8Data+=3;
							LEN_Data0+=3;
							LEN_RxDataBuff-=3;
							break;
					}
					break;
				case 0x2004://�й�����
				case 0x2005://�޹�����
			//�С��޹����ʣ�24 �ֽ�
			//��ָ��룺AAH��1 �ֽ�
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if((LEN_Rx<(offset+24))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x2005://�޹�����
							offset+=12;
						case 0x2004://�й�����
							if(LEN_RxDataBuff<(2+(4*5)))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Array[0]=DataType_array;
							p8Array[1]=0;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							for(i=0;i<4;i++)
							{
								Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16);
								if(Data==0xffffff)
								{
									offset+=3;
									continue;
								}
								if(Data>>23)
								{//��
									Data<<=9;
									Data>>=9;
									Data=bcd_hex(Data);
									Data=~Data;
									Data+=1;
								}
								else
								{
									Data=bcd_hex(Data);
								}
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								offset+=3;
								p8Data+=5;
								LEN_Data0+=5;
								LEN_RxDataBuff-=5;
								p8Array[1]++;//����������
							}
							break;
					}
					break;
				case 0x200a://��������
			//����������8 �ֽ�
			//��ָ��룺AAH��1 �ֽ�
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if((LEN_Rx<(offset+8))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					if(LEN_RxDataBuff<(2+(4*3)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=0;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<4;i++)
					{
						Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8);
						if(Data==0xffff)
						{
							offset+=2;
							continue;
						}
						if(Data>>15)
						{//��
							Data<<=17;
							Data>>=17;
							Data=bcd_hex(Data);
							Data=~Data;
							Data+=1;
						}
						else
						{
							Data=bcd_hex(Data);
						}
						p8Data[0]=DataType_long;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						offset+=2;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8Array[1]++;//����������
					}
					break;
				case 0x0010://�����й�����
				case 0x0020://�����й�����
				case 0x0030://����޹�1����
				case 0x0040://����޹�2����
			//�С��޹��ܵ��ܣ�16 �ֽ�
			//��ָ��룺AAH��1 �ֽ�
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=8+1;
					}
					if((LEN_Rx<(offset+16))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x0020://�����й�����
							offset+=4;
						case 0x0010://�����й�����
							if(LEN_RxDataBuff<(2+5+4))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Array[0]=DataType_array;
							p8Array[1]=5;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16)|(p8rx[10+offset+3]<<24);
							Data=bcd_hex(Data);
							p8Data[0]=DataType_double_long_unsigned;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data[5]=0;//����1
							p8Data[6]=0;//����2
							p8Data[7]=0;//����3
							p8Data[8]=0;//����4
							p8Data+=5+4;
							LEN_Data0+=5+4;
							LEN_RxDataBuff-=5+4;
							break;
						case 0x0040://����޹�2����
							offset+=4;
						case 0x0030://����޹�1����
							offset+=8;
							if(LEN_RxDataBuff<(2+5+4))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Array[0]=DataType_array;
							p8Array[1]=5;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16)|(p8rx[10+offset+3]<<24);
							if(Data>>31)
							{//��
								Data<<=1;
								Data>>=1;
								Data=bcd_hex(Data);
								Data=~Data;
								Data+=1;
							}
							else
							{
								Data=bcd_hex(Data);
							}
							p8Data[0]=DataType_double_long;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data[5]=0;//����1
							p8Data[6]=0;//����2
							p8Data[7]=0;//����3
							p8Data[8]=0;//����4
							p8Data+=5+4;
							LEN_Data0+=5+4;
							LEN_RxDataBuff-=5+4;
							break;
					}
					break;
				case 0x0050://��1�����޹�����
				case 0x0060://��2�����޹�����
				case 0x0070://��3�����޹�����
				case 0x0080://��4�����޹�����
			//�������޹��ܵ��ܣ�16 �ֽ�
			//��ָ��룺AAH��1 �ֽ�
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=8+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=16+1;
					}
					if((LEN_Rx<(offset+16))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x0080://��4�����޹�����
							offset+=4;
						case 0x0070://��3�����޹�����
							offset+=4;
						case 0x0060://��2�����޹�����
							offset+=4;
						case 0x0050://��1�����޹�����
							if(LEN_RxDataBuff<(2+5+4))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Array[0]=DataType_array;
							p8Array[1]=5;
							p8Data+=2;
							LEN_Data0+=2;
							LEN_RxDataBuff-=2;
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16)|(p8rx[10+offset+3]<<24);
							Data=bcd_hex(Data);
							p8Data[0]=DataType_double_long_unsigned;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data[5]=0;//����1
							p8Data[6]=0;//����2
							p8Data[7]=0;//����3
							p8Data[8]=0;//����4
							p8Data+=5+4;
							LEN_Data0+=5+4;
							LEN_RxDataBuff-=5+4;
							break;
					}
					break;
				case 0x2017://��ǰ�й�����
				case 0x2018://��ǰ�޹�����
			//��ǰ������6 �ֽ�
			//��ָ��룺AAH��1 �ֽ�
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=17+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=24+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=8+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=16+1;
					}
					if(p8rx[10+offset]==0xaa)
					{
						offset+=1;
					}
					else
					{
						offset+=16+1;
					}
					if((LEN_Rx<(offset+6))||(p8rx[10+offset]==0xaa))
					{
						if(LEN_RxDataBuff<1)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data[0]=0;
						p8Data+=1;
						LEN_Data0+=1;
						LEN_RxDataBuff-=1;
						break;
					}
					switch(OI)
					{
						case 0x2018://��ǰ�޹�����
							offset+=3;
						case 0x2017://��ǰ�й�����
							if(LEN_RxDataBuff<5)
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							Data=p8rx[10+offset]|(p8rx[10+offset+1]<<8)|(p8rx[10+offset+2]<<16);
							if(Data==0xffffff)
							{
								p8Data[0]=0;
								p8Data+=1;
								LEN_Data0+=1;
								LEN_RxDataBuff-=1;
								break;
							}
							Data=bcd_hex(Data);
							p8Data[0]=DataType_double_long_unsigned;
							p8Data[1]=Data>>24;
							p8Data[2]=Data>>16;
							p8Data[3]=Data>>8;
							p8Data[4]=Data>>0;
							p8Data+=5;
							LEN_Data0+=5;
							LEN_RxDataBuff-=5;
							break;
					}
					break;
				default:
					if(LEN_RxDataBuff<1)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data[0]=0;
					p8Data+=1;
					LEN_Data0+=1;
					LEN_RxDataBuff-=1;
					break;
			}
		}
		p16Data[0]=LEN_Data0;
		return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	
//��ʵʱ���ݶ���Ϊ����
	i=UARTCtrl->RMCSD[1];
	if(i)
	{//ROAD
		OI=UARTCtrl->RMCSD[2+0]<<24;
		OI|=UARTCtrl->RMCSD[2+1]<<16;
		OI|=UARTCtrl->RMCSD[2+2]<<8;
		OI|=UARTCtrl->RMCSD[2+3];
	#if(USER/100)==17//����ϵ
		if((OI==0x50020200)||(OI==0x50050200))
	#else
		if(OI==0x50020200)
	#endif
		{//���Ӷ���
			p8Data+=LEN_Data0;
			p8Array=(u8*)p8Data;
			LEN_Rx-=4;
			p8rx+=14;
			n=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
			OI=(UARTCtrl->RMCSD[7+(4*n)+0]<<24)+(UARTCtrl->RMCSD[7+(4*n)+1]<<16)+(UARTCtrl->RMCSD[7+(4*n)+2]<<8)+(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{//�������ǡ�00100201���͡�00200201������վ�á�00100200���͡�00200200���ٶ������ʵ��������ǿգ�ֻ���ܵ������ؾ�����
				case 0x00100201://�����й�������
				case 0x00200201://�����й�������
				case 0x00300201://����޹�1������
				case 0x00400201://����޹�2������
				case 0x00500201://��1�����޹�������
				case 0x00600201://��2�����޹�������
				case 0x00700201://��3�����޹�������
				case 0x00800201://��4�����޹�������
				case 0x20000201://A���ѹ
				case 0x20010201://A�����
				case 0x20040202://A���й�����
				case 0x20050202://A���޹�����
				case 0x200A0202://A�๦������
					UARTCtrl->RMCSD[7+(4*n)+3]=0;
					break;
			}
			switch(OI)
			{
				case 0x20210200://����ʱ��
					if(LEN_RxDataBuff<8)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//�õ�date_time_s��7�ֽ�
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					break;
					
			#if(USER/100)==17//???
				case 0x10100200:
				case 0x10300200:
					signedDI=signed_DL2007_DI(PORTn);
					LEN_Rx/=8;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//??????
						return 2;//??:????0=??,1=??1????,2=??2????,3=?????????,4=???????,5=?????127,6=?????????
					}
					p8Array[0]=DataType_array;
					p8Array[1]=0;
					LEN_Data0=2;
					LEN_RxDataBuff-=2;
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						//?
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{//?
							p8rx+=8;
							continue;
						}
						p8Data[0]=DataType_structure;
						p8Data[1]=2;
						p8Data+=2;
						if(signedDI==0)
						{
							p8Data[0]=DataType_double_long_unsigned;
							Data=bcd_hex(Data);
						}
						else
						{
							p8Data[0]=DataType_double_long;
							if(Data>>23)
							{//?
								Data<<=9;
								Data>>=9;
								Data=bcd_hex(Data);
								Data=~Data;
								Data+=1;
							}
							else
							{
								Data=bcd_hex(Data);
							}
						}
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8rx+=3;
						p8Data+=5;
						//????
						p8Data[0]=DataType_date_time_s;
						Data=p8rx[4];//?
						Data=bcd_hex(Data);
						Data+=2000;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						Data=p8rx[3];//?
						Data=bcd_hex(Data);
						p8Data[3]=Data;
						Data=p8rx[2];//?
						Data=bcd_hex(Data);
						p8Data[4]=Data;
						Data=p8rx[1];//?
						Data=bcd_hex(Data);
						p8Data[5]=Data;
						Data=p8rx[0];//?
						Data=bcd_hex(Data);
						p8Data[6]=Data;
						p8Data[7]=0;//?
						p8rx+=5;
						p8Data+=8;
						
						LEN_Data0+=2+5+8;;
						if(p8Array[1]>=127)
						{
							return 5;//??:????0=??,1=??1????,2=??2????,3=?????????,4=???????,5=?????127,6=?????????
						}
						p8Array[1]++;//?????
					}
					p16Data[0]=LEN_Data0;
					break;
			case 0x00100200://??????
			case 0x00200200://??????
			case 0x00500200://???????
			case 0x00600200://???????
			case 0x00700200://???????
			case 0x00800200://???????
		#endif
				case 0x00100201://�����й��ܵ���
				case 0x00200201://�����й��ܵ���
				case 0x00500201://һ�����޹��ܵ���
				case 0x00600201://�������޹��ܵ���
				case 0x00700201://�������޹��ܵ���
				case 0x00800201://�������޹��ܵ���
					LEN_Rx/=4;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16)|(p8rx[3]<<24);
						signedDI=0;
						if(signedDI==0)
						{
							p8Data[0]=DataType_double_long_unsigned;
							Data=bcd_hex(Data);
						}
						else
						{
							p8Data[0]=DataType_double_long;
							if(Data>>31)
							{//��
								Data<<=1;
								Data>>=1;
								Data=bcd_hex(Data);
								Data=~Data;
								Data+=1;
							}
							else
							{
								Data=bcd_hex(Data);
							}
						}
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=4;
					}
					if(LEN_Rx==1)
					{
						if(LEN_RxDataBuff<4)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Array[1]=5;//������
						p8Data[0]=0;//����1
						p8Data[1]=0;//����2
						p8Data[2]=0;//����3
						p8Data[3]=0;//����4
						p8Data+=4;
						LEN_Data0+=4;
						LEN_RxDataBuff-=4;
					}
					break;
				case 0x00300200://����޹�1����
				case 0x00400200://����޹�2����
					LEN_Rx/=4;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16)|(p8rx[3]<<24);
						signedDI=1;
						if(signedDI==0)
						{
							p8Data[0]=DataType_double_long_unsigned;
							Data=bcd_hex(Data);
						}
						else
						{
							p8Data[0]=DataType_double_long;
							if(Data>>31)
							{//��
								Data<<=1;
								Data>>=1;
								Data=bcd_hex(Data);
								Data=~Data;
								Data+=1;
							}
							else
							{
								Data=bcd_hex(Data);
							}
						}
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=4;
					}
					if(LEN_Rx==1)
					{
						if(LEN_RxDataBuff<4)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Array[1]=5;//������
						p8Data[0]=0;//����1
						p8Data[1]=0;//����2
						p8Data[2]=0;//����3
						p8Data[3]=0;//����4
						p8Data+=4;
						LEN_Data0+=4;
						LEN_RxDataBuff-=4;
					}
					break;
				case 0x20170200://��ǰ�й�����
				case 0x20180200://��ǰ�޹�����
				//case 0x11100200://���������������й�������� 
					if(LEN_RxDataBuff<5)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
					}
					if(Data>>23)
					{//��
						Data<<=9;
						Data>>=9;
						Data=bcd_hex(Data);
						Data=~Data;
						Data+=1;
					}
					else
					{
						Data=bcd_hex(Data);
					}
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data+=5;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
					break;
				case 0x20000200://��ѹ
					LEN_Rx/=2;
					if(LEN_RxDataBuff<(2+(3*LEN_Rx)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8);
						if(Data==0xffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_long_unsigned;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=2;
					}
					break;
				case 0x20010200://����
					LEN_Rx/=3;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						if(Data>>23)
						{//��
							Data<<=9;
							Data>>=9;
							Data=bcd_hex(Data);
							Data=~Data;
							Data+=1;
						}
						else
						{
							Data=bcd_hex(Data);
						}
						p8Data[0]=DataType_double_long;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
					}
					break;
				case 0x20010400://���ߵ���
					//���ߵ���������
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
					}
					if(Data>>23)
					{//��
						Data<<=9;
						Data>>=9;
						Data=bcd_hex(Data);
						Data=~Data;
						Data+=1;
					}
					else
					{
						Data=bcd_hex(Data);
					}
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data+=5;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
					break;
				case 0x20040200://�й�����
				case 0x20050200://�޹�����
					LEN_Rx/=3;
					if(LEN_RxDataBuff<(2+(5*LEN_Rx)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						if(Data>>23)
						{//��
							Data<<=9;
							Data>>=9;
							Data=bcd_hex(Data);
							Data=~Data;
							Data+=1;
						}
						else
						{
							Data=bcd_hex(Data);
						}
						p8Data[0]=DataType_double_long;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
					}
					break;
				case 0x200A0200://��������
					LEN_Rx/=2;
					if(LEN_RxDataBuff<(2+(3*LEN_Rx)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=LEN_Rx;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8);
						if(Data==0xffff)
						{
							Data=0;
						}
						if(Data>>15)
						{//��
							Data<<=17;
							Data>>=17;
							Data=bcd_hex(Data);
							Data=~Data;
							Data+=1;
						}
						else
						{
							Data=bcd_hex(Data);
						}
						p8Data[0]=DataType_long;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=2;
					}
					break;
				case 0x20000201://A��ѹ
					if(LEN_RxDataBuff<(2+3+2))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8);
					if(Data==0xffff)
					{
						Data=0;
					}
					Data=bcd_hex(Data);
					p8Data[0]=DataType_long_unsigned;
					p8Data[1]=Data>>8;
					p8Data[2]=Data;
					p8Data[3]=0;//B��ѹ
					p8Data[4]=0;//C��ѹ
					p8Data+=3+2;
					LEN_Data0+=3+2;
					LEN_RxDataBuff-=3+2;
					break;
				case 0x20010201://A����
					if(LEN_RxDataBuff<(2+5+2))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=3;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
					}
					if(Data>>23)
					{//��
						Data<<=9;
						Data>>=9;
						Data=bcd_hex(Data);
						Data=~Data;
						Data+=1;
					}
					else
					{
						Data=bcd_hex(Data);
					}
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data[5]=0;//B������
					p8Data[6]=0;//C������
					p8Data+=5+2;
					LEN_Data0+=5+2;
					LEN_RxDataBuff-=5+2;
					break;
				case 0x20040202://A���й�����
				case 0x20050202://A���޹�����
					if(LEN_RxDataBuff<(2+5))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=1;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
					if(Data==0xffffff)
					{
						Data=0;
					}
					if(Data>>23)
					{//��
						Data<<=9;
						Data>>=9;
						Data=bcd_hex(Data);
						Data=~Data;
						Data+=1;
					}
					else
					{
						Data=bcd_hex(Data);
					}
					p8Data[0]=DataType_double_long;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data>>0;
					p8Data+=5;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
					break;
				case 0x200A0202://A�๦������
					if(LEN_RxDataBuff<(2+3))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=1;
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
					Data=p8rx[0]|(p8rx[1]<<8);
					if(Data==0xffff)
					{
						Data=0;
					}
					if(Data>>15)
					{//��
						Data<<=17;
						Data>>=17;
						Data=bcd_hex(Data);
						Data=~Data;
						Data+=1;
					}
					else
					{
						Data=bcd_hex(Data);
					}
					p8Data[0]=DataType_long;
					p8Data[1]=Data>>8;
					p8Data[2]=Data;
					p8rx+=2;
					p8Data+=3;
					LEN_Data0+=3;
					LEN_RxDataBuff-=3;
					break;
				default:
					if(LEN_RxDataBuff<1)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data[0]=0;
					p8Data+=1;
					LEN_Data0+=1;
					LEN_RxDataBuff-=1;
					break;
			}
			p16Data[0]=LEN_Data0;
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
	}
//---���߽���---
	
//---�¼��࿪ʼ---
	i=UARTCtrl->TaskID;
	//ͣ�ϵ��¼��̶������
	if(((i>>8)&0xff)==RMTASKnoPOWER)
	{
		//��׼�¼���¼��Ԫ��=structure
		//{
		//  �¼���¼���  double-long-unsigned��
		//  �¼�����ʱ��  date_time_s��
		//  �¼�����ʱ��  date_time_s��
				
		//  �¼�����Դ    instance-specific��
		//  �¼��ϱ�״̬  array ͨ���ϱ�״̬��
		//  ��1�������������Ե�����  Data��
		//  ��
		//  ��n�������������Ե�����  Data
		//}
		if(LEN_RxDataBuff<23)
		{//���峤�Ȳ���
			return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		p8Data[0]=DataType_structure;
		p8Data[1]=3;
		//�¼���¼���  double-long-unsigned
		p8Data[2]=DataType_double_long_unsigned;
		p8Data[3]=0;
		p8Data[4]=0;
		p8Data[5]=0;
		p8Data[6]=0;
		//�¼�����ʱ��  date_time_s
		//�¼�����ʱ��  date_time_s
		n=2;
		while(n--)
		{
			p8Data[7]=DataType_date_time_s;
			i=p8rx[14+5];//��
			i=bcd_hex(i);
			i+=2000;
			p8Data[8]=i>>8;
			p8Data[9]=i;
			i=p8rx[14+4];//��
			i=bcd_hex(i);
			p8Data[10]=i;
			i=p8rx[14+3];//��
			i=bcd_hex(i);
			p8Data[11]=i;
			i=p8rx[14+2];//ʱ
			i=bcd_hex(i);
			p8Data[12]=i;
			i=p8rx[14+1];//��
			i=bcd_hex(i);
			p8Data[13]=i;
			i=p8rx[14+0];//��
			i=bcd_hex(i);
			p8Data[14]=i;
			p8rx+=6;
			p8Data+=8;
		}
		LEN_Data0+=23;
		LEN_RxDataBuff-=23;
		p16Data[0]=LEN_Data0;
		return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	//ȫ�¼��ɼ�
	if(((i>>24)&0xf)==2)
	{//�¼��ɼ�����
	#if CompareMeterEventCount==1//ȫ�¼��ɼ������Ƿ�ȽϷ�������:0=���Ƚ�(ȫ����������д���ļ�),1=�Ƚ�(ֻ�д����仯���¼�д���ļ�)
		if(UARTCtrl->RMCSDSubCount==0)//����CSD�Ӽ���(��DL698����ʱ)
		{
			i=p8rx[14+0];//ȡ�¼�����
			i|=p8rx[14+1]<<8;
			i|=p8rx[14+2]<<16;
			i=bcd_hex(i);
//			i=Get_RxDATA_DL2007_MeterEventCount(PORTn);//��645���¼���¼����֡��ȡ�����ܱ��¼���������(�¼���¼���);����:hex��������,û�ҵ�ʱ����0xffffffff
//			if(i==0xffffffff)
//			{
//				return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
//			}
			OI=UARTCtrl->RMCSD[2+0]<<24;
			OI|=UARTCtrl->RMCSD[2+1]<<16;
			OI|=UARTCtrl->RMCSD[2+2]<<8;
			OI|=UARTCtrl->RMCSD[2+3];
			n=UARTCtrl->TaskID;
			n>>=8;
			n&=0xff;
			n=UARTCtrl->MeterCount[n];//2*RMTASKmax byte�������ѳ����ܱ����,�����ϱ�ʱ�����ļ������ϱ����ܱ����
			if(i==0)
			{
				i=CompSourMeterEventCount(n,OI,i);
				UARTCtrl->RMCSDSubCount=1;//����CSD�Ӽ���(��DL698����ʱ)
				return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			i=CompSourMeterEventCount(n,OI,i);//�Ƚ�ԭ���ܱ��¼�����;���:SetNo=�������ú�(0��ʼ),Count=��ǰ����;����:0=��ʼֵ,1=������ͬ,2=������ͬ,ͬʱ�޸�ԭ����,ʹ��ADDR_DATABUFF
			if(i!=2)
			{
				UARTCtrl->RMCSDSubCount=1;//����CSD�Ӽ���(��DL698����ʱ)
				return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
		}
	#endif
		i=Change_Frame_645_to_698(PORTn);//��645���¼�����֡ת��698��ʽ
		return i;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
//---�¼������---
	
	signedDI=signed_DL2007_DI(PORTn);//DL/T-2007���ݴ����������ŵ����ݱ�ʶ;���:UARTCtrl->RM_DI;����:0=û���ţ�1=�з���
	i=UARTCtrl->RMCSD[1];//��ǰ���ڳ���CSD(����������)
	if(i==0)
	{//OAD
		OI=UARTCtrl->RMCSD[2]<<8;//��ǰ���ڳ���CSD
		OI|=UARTCtrl->RMCSD[3];
	#if(USER/100)==15//����
		s=UARTCtrl->RMCSD[4]<<8;
		s|=UARTCtrl->RMCSD[5];
	#endif
	}
	else
	{//ROAD
		i=UARTCtrl->RMROADCount;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
		i*=4;
		OI=UARTCtrl->RMCSD[7+i]<<8;//��ǰ���ڳ���CSD
		OI|=UARTCtrl->RMCSD[7+i+1];
	#if(USER/100)==15//����
		s=UARTCtrl->RMCSD[7+i+2]<<8;
		s|=UARTCtrl->RMCSD[7+i+3];
	#endif
	}
	i=OI_IC(OI);//�õ��ӿ����;���:�����ʶOI
	
	LEN_Rx-=4;
	p8rx+=14;
	switch(i)
	{
		case 1://�������ӿ���,���ӿ�������ṩ�洢����������Ϣ
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://�ܼ����ʵ����������=array ������
				//��������
				//CHOICE
				//{
				//double-long-unsigned   [6]��
				//double-long            [5]
				//}
					LEN_Rx/=4;//������;DL2007�������ݸ�ʽ:XXXXXX.XX
					if(LEN_RxDataBuff<(5*LEN_Rx))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16)|(p8rx[3]<<24);
						if(signedDI==0)
						{
							p8Data[0]=DataType_double_long_unsigned;
							Data=bcd_hex(Data);
						}
						else
						{
							p8Data[0]=DataType_double_long;
							if(Data>>31)
							{//��
								Data<<=1;
								Data>>=1;
								Data=bcd_hex(Data);
								Data=~Data;
								Data+=1;
							}
							else
							{
								Data=bcd_hex(Data);
							}
						}
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8rx+=4;
						p8Data+=5;
						LEN_Data0+=5;
						if(p8Array[1]>=127)
						{
							return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Array[1]++;//����������
					}
					p16Data[0]=LEN_Data0;
					break;
//				case 4://�߾����ܼ����ʵ����������=array �߾��ȵ�����
//				//�߾��ȵ�������
//				//CHOICE
//				//{
//				//long64-unsigned        [21]��
//				//long64                 [20]
//				//}

//					break;
				default:
					return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
		case 2://��������ӿ���,���ӿ�������ṩ�洢�����������Ϣ
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://�ܼ�����������������=array �������������ʱ��
				//�������������ʱ���=structure
				//{
				//  �������ֵ CHOICE��
				//  ����ʱ��   date_time_s
				//}
				//�������ֵ ��
				//CHOICE
				//{
				//double-long            [5]��
				//double-long-unsigned   [6]
				//}
					LEN_Rx/=8;//������;DL2007�������ݸ�ʽ:XX.XXXX YYMMDDhhmm
					if(LEN_RxDataBuff<(13*LEN_Rx))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						//ֵ
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{//��
							p8rx+=8;
							continue;
						}
						p8Data[0]=DataType_structure;
						p8Data[1]=2;
						p8Data+=2;
						if(signedDI==0)
						{
							p8Data[0]=DataType_double_long_unsigned;
							Data=bcd_hex(Data);
						}
						else
						{
							p8Data[0]=DataType_double_long;
							if(Data>>23)
							{//��
								Data<<=9;
								Data>>=9;
								Data=bcd_hex(Data);
								Data=~Data;
								Data+=1;
							}
							else
							{
								Data=bcd_hex(Data);
							}
						}
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8rx+=3;
						p8Data+=5;
						//����ʱ��
						p8Data[0]=DataType_date_time_s;
						Data=p8rx[4];//��
						Data=bcd_hex(Data);
						Data+=2000;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						Data=p8rx[3];//��
						Data=bcd_hex(Data);
						p8Data[3]=Data;
						Data=p8rx[2];//��
						Data=bcd_hex(Data);
						p8Data[4]=Data;
						Data=p8rx[1];//ʱ
						Data=bcd_hex(Data);
						p8Data[5]=Data;
						Data=p8rx[0];//��
						Data=bcd_hex(Data);
						p8Data[6]=Data;
						p8Data[7]=0;//��
						p8rx+=5;
						p8Data+=8;
						
						LEN_Data0+=2+5+8;;
						if(p8Array[1]>=127)
						{
							return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Array[1]++;//����������
					}
					p16Data[0]=LEN_Data0;
					break;
				default:
					return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
		case 3://��������ӿ���,���ӿ�������ṩ�洢��ѹ����������ǵȷ����������Ϣ
		//������ֵ���=array ������ֵ
		//��ֵ�鰴A�ࡢB�ࡢC�ࡢN��˳�����У����У���������N�ߣ����⣬�����߷�ʽΪ����ʱ��A��B��C�����ΪA�ࣨĳһ�ࣩ��
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://������ֵ��
					switch(OI)
					{
						case 0x2000://��ѹ
							LEN_Rx/=2;//������;DL2007�������ݸ�ʽ:XXX.X
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
								}
								Data=bcd_hex(Data);
								p8Data[0]=DataType_long_unsigned;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						case 0x2001://����
							LEN_Rx/=3;//������;DL2007�������ݸ�ʽ:XXX.XXX
							if(LEN_RxDataBuff<(5*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{
									p8rx+=3;
									continue;
								}
								if(Data>>23)
								{//��
									Data<<=9;
									Data>>=9;
									Data=bcd_hex(Data);
									Data=~Data;
									Data+=1;
								}
								else
								{
									Data=bcd_hex(Data);
								}
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								p8rx+=3;
								p8Data+=5;
								LEN_Data0+=5;
								if(p8Array[1]>=127)
								{
									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						case 0x2002://��ѹ���
						case 0x2003://��ѹ�������
							LEN_Rx/=2;//������;DL2007�������ݸ�ʽ:XXX.X
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
								}
								Data=bcd_hex(Data);
								p8Data[0]=DataType_long_unsigned;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						case 0x200B://��ѹ����ʧ���
						case 0x200C://��������ʧ���
							LEN_Rx/=2;//������;DL2007�������ݸ�ʽ:XX.XX
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
								}
								if(Data>>15)
								{//��
									Data<<=17;
									Data>>=17;
									Data=bcd_hex(Data);
									Data=~Data;
									Data+=1;
								}
								else
								{
									Data=bcd_hex(Data);
								}
								p8Data[0]=DataType_long;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
				case 4://���ߵ���
					switch(OI)
					{
						case 0x2001://����
							LEN_Rx=1;//������;DL2007�������ݸ�ʽ:XXX.XXX
							if(LEN_RxDataBuff<(5*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							LEN_Data0=0;//���ߵ���������
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{
									p8rx+=3;
									continue;
								}
								if(Data>>23)
								{//��
									Data<<=9;
									Data>>=9;
									Data=bcd_hex(Data);
									Data=~Data;
									Data+=1;
								}
								else
								{
									Data=bcd_hex(Data);
								}
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								p8rx+=3;
								p8Data+=5;
								LEN_Data0+=5;
								//���ߵ���������
//								if(p8Array[1]>=127)
//								{
//									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
//								}
//								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
				default:
					return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
		case 4://���ʽӿ���,���ӿ�������ṩ�洢���ʡ�������������Ϣ
			//�ܼ�������ֵ���=array �ܻ������ֵ
			//��ֵ�鰴�ܡ�A�ࡢB�ࡢC��˳�����У������߷�ʽΪ����ʱ��Ϊ�ܡ�A�ࣨĳһ�ࣩ
			if(LEN_Data0==0)
			{
				if(LEN_RxDataBuff<2)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Array[0]=DataType_array;
				p8Array[1]=0;
				LEN_Data0=2;
				LEN_RxDataBuff-=2;
			}
			switch(UARTCtrl->RMCSD[4])//����
			{
				case 2://�ܼ�������ֵ��
					switch(OI)
					{
						case 0x2004://�й�����
						case 0x2005://�޹�����
							if(UARTCtrl->RMCSD[1]==1)
							{//����¼
								if(((UARTCtrl->RMCSD[2]<<8)+UARTCtrl->RMCSD[3])==0x5004)
								{//�ն���:��1���ն���������ݣ��ֽ���3*8��(��/A/B/C�й�����,��/A/B/C�޹�����)
									if(LEN_Rx!=24)
									{
										return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									LEN_Rx-=12;
									if(OI==0x2005)//�޹�����
									{
										p8rx+=12;//�����й�����
									}
								}
							}
						case 0x2006://���ڹ���
						case 0x2007://һ����ƽ���й�����
						case 0x2008://һ����ƽ���޹�����
						case 0x2009://һ����ƽ�����ڹ���
							LEN_Rx/=3;//������;DL2007�������ݸ�ʽ:XX.XXXX
							if(LEN_RxDataBuff<(5*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{
									p8rx+=3;
									continue;
								}
								if(Data>>23)
								{//��
									Data<<=9;
									Data>>=9;
									Data=bcd_hex(Data);
									Data=~Data;
									Data+=1;
								}
								else
								{
									Data=bcd_hex(Data);
								}
								p8Data[0]=DataType_double_long;
								p8Data[1]=Data>>24;
								p8Data[2]=Data>>16;
								p8Data[3]=Data>>8;
								p8Data[4]=Data>>0;
								p8rx+=3;
								p8Data+=5;
								LEN_Data0+=5;
								if(p8Array[1]>=127)
								{
									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						case 0x200A://��������
							LEN_Rx/=2;//������;DL2007�������ݸ�ʽ:XX.XX
							if(LEN_RxDataBuff<(3*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								Data=p8rx[0]|(p8rx[1]<<8);
								if(Data==0xffff)
								{
									p8rx+=2;
									continue;
								}
								if(Data>>15)
								{//��
									Data<<=17;
									Data>>=17;
									Data=bcd_hex(Data);
									Data=~Data;
									Data+=1;
								}
								else
								{
									Data=bcd_hex(Data);
								}
								p8Data[0]=DataType_long;
								p8Data[1]=Data>>8;
								p8Data[2]=Data;
								p8rx+=2;
								p8Data+=3;
								LEN_Data0+=3;
								if(p8Array[1]>=127)
								{
									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						default:
							return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
				default:
					return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
//		case 5://г�������ӿ���,���ӿ�������ṩ�洢г����������Ϣ
//			break;
		case 6://���ݱ����ӿ���,���ӿ�������ṩ�洢����ֵ�������ֵ��Ԫ��ص�״ֵ̬��Ϣ
			switch(OI)
			{
			#if(USER/100)==15//����
				case 0x2131://��A���ѹ�ϸ���
				case 0x2132://��B���ѹ�ϸ���
				case 0x2133://��C���ѹ�ϸ���
					if(((s>>8)&0xff)==2&&(s&0xff)!=0)//�µ�ѹ�ϸ��ʻ����յ�ѹ�ϸ���
					{
						if(LEN_RxDataBuff<15)
						{//���峤�Ȳ���
							return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
						}
						p8Data+=LEN_Data0;
						p8Data[0]=DataType_structure;
						p8Data[1]=5;
						p8Data+=2;
						LEN_Data0+=2;
						LEN_RxDataBuff-=2;
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_long_unsigned;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_long_unsigned;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data+=3;
						LEN_Data0+=3;
						LEN_RxDataBuff-=3;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
						
						Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
						if(Data==0xffffff)
						{
							Data=0;
						}
						Data=bcd_hex(Data);
						p8Data[0]=DataType_double_long_unsigned;
						p8Data[1]=Data>>24;
						p8Data[2]=Data>>16;
						p8Data[3]=Data>>8;
						p8Data[4]=Data>>0;
						p8Data+=5;
						LEN_Data0+=5;
						LEN_RxDataBuff-=5;
						p8rx+=3;
						
						p16Data[0]=LEN_Data0;
					}
					break;
				#endif
				case 0x2014://�������״̬��1-7
					LEN_Rx/=2;//������;DL2007�������ݸ�ʽ:2byte״̬��1...2byte״̬��7
					if(LEN_RxDataBuff<(2+(4*LEN_Rx)))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=0;
					LEN_Data0=2;
					LEN_RxDataBuff-=2;
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						p8Data[0]=DataType_bit_string;
						p8Data[1]=16;
						Data=p8rx[0];
						Data=Bit_Inverse(Data,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
						p8Data[2]=Data;
						Data=p8rx[1];
						Data=Bit_Inverse(Data,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
						p8Data[3]=Data;
						
						p8Data+=4;
						p8Array[1]++;
						LEN_Data0+=4;
						LEN_RxDataBuff-=4;
						p8rx+=2;
					}
					p16Data[0]=LEN_Data0;
					break;
				case 0x202D://����ǰ��͸֧���  �������ͣ�double-long-unsigned����λ��Ԫ�����㣺-2
					if(LEN_RxDataBuff<5)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					Data=p8rx[6]|(p8rx[7]<<8)|(p8rx[8]<<16)|(p8rx[9]<<24);
					Data=bcd_hex(Data);
					p8Data[0]=DataType_double_long_unsigned;
					p8Data[1]=Data>>24;
					p8Data[2]=Data>>16;
					p8Data[3]=Data>>8;
					p8Data[4]=Data;
					LEN_Data0+=5;
					LEN_RxDataBuff-=5;
					p16Data[0]=LEN_Data0;
					break;
				default:
					return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
			}
			break;
//		case 7://�¼�����ӿ���,���ӿ�������ṩ���á��洢�¼���¼����Ϣ
//			break;
		case 8://���������ӿ���,���ӿ�������ṩ�洢�ն˵ĸ��ֲ�������Ϣ
			switch(OI)
			{
				case 0x2021://���ݶ���ʱ��
					if(LEN_RxDataBuff<8)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					if(UARTCtrl->RM_DI==0x00FE0001)//DL/T645-2007/1997��ǰ�������ݱ�ʶ
					{//����һ�����յĶ���ʱ�ֻ꣬��������ʱ4�ֽ�
						for(i=0;i<4;i++)
						{
							Data=p8rx[i];
							Data=bcd_hex(Data);
							p8rx[i]=Data;
						}
						p8Data+=LEN_Data0;
						p8Data[0]=DataType_date_time_s;
						Data=p8rx[3];//��
						Data+=2000;
						p8Data[1]=Data>>8;
						p8Data[2]=Data;
						p8Data[3]=p8rx[2];//��
						p8Data[4]=p8rx[1];//��
						p8Data[5]=p8rx[0];//ʱ
						p8Data[6]=0;//��
						p8Data[7]=0;//��
						LEN_Data0+=8;
						p16Data[0]=LEN_Data0;
						break;
					}
					//ʱ�꣬������ʱ��5�ֽ�
					for(i=0;i<5;i++)
					{
						Data=p8rx[i];
						Data=bcd_hex(Data);
						p8rx[i]=Data;
					}
					p8Data+=LEN_Data0;
					p8Data[0]=DataType_date_time_s;
					Data=p8rx[4];//��
					Data+=2000;
					p8Data[1]=Data>>8;
					p8Data[2]=Data;
					p8Data[3]=p8rx[3];//��
					p8Data[4]=p8rx[2];//��
					p8Data[5]=p8rx[1];//ʱ
					p8Data[6]=p8rx[0];//��
					p8Data[7]=0;//��
					LEN_Data0+=8;
					p16Data[0]=LEN_Data0;
//				#if CompareTime_5004==1//�����ն����Ƿ�Ƚ϶���ʱ��:0=���Ƚ�,1=�Ƚ�
//					i=UARTCtrl->RMCSD[1];//��ǰ���ڳ���CSD(����������)
//					if(i==1)
//					{//ROAD
//						OI=UARTCtrl->RMCSD[2]<<24;//��ǰ���ڳ���OAD
//						OI|=UARTCtrl->RMCSD[2+1]<<16;
//						OI|=UARTCtrl->RMCSD[2+2]<<8;
//						OI|=UARTCtrl->RMCSD[2+3]<<0;
//						if(OI==0x50040200)
//						{//�ն���
//							i=UARTCtrl->RMROADCount;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
//							i*=4;
//							OI=UARTCtrl->RMCSD[7+i]<<24;//��ǰ���ڳ���CSD
//							OI|=UARTCtrl->RMCSD[7+i+1]<<16;
//							OI|=UARTCtrl->RMCSD[7+i+2]<<8;
//							OI|=UARTCtrl->RMCSD[7+i+3]<<0;
//							if(OI==0x20210200)
//							{//���ݶ���ʱ��
//								for(i=0;i<4;i++)
//								{
//									if(p8Data[1+i]!=Comm_Ram->DL698YMDHMS[i])
//									{
//										break;
//									}
//								}
//								if(i<4)
//								{//�����ղ�ͬ
//									LEN_Data0=0;
//									p16Data[0]=LEN_Data0;
//								}
//							}
//						}
//					}
//				#endif
					break;
				case 0x4000://����ʱ��
					switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
					{
						case 0://���ܱ�����ʱ��Hhmmss
							if(LEN_RxDataBuff<8)
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							for(i=0;i<3;i++)
							{
								Data=p8rx[i];
								Data=bcd_hex(Data);
								p8rx[i]=Data;
							}
							p8Data+=LEN_Data0;
							p8Data[0]=DataType_date_time_s;
							p8Data[1]=0;//��
							p8Data[2]=0;
							p8Data[3]=0;//��
							p8Data[4]=0;//��
							p8Data[5]=p8rx[2];//ʱ
							p8Data[6]=p8rx[1];//��
							p8Data[7]=p8rx[0];//��
							LEN_Data0+=8;
							p16Data[0]=LEN_Data0;
							break;
						case 1://���ܱ�����ʱ��YYMMDDWW
							if(LEN_Data0<8)
							{
								break;//����д��buff����û����һ��CSD�����ݳ���
							}
							p8Data+=LEN_Data0;
							p8Data-=8;
							Data=p8rx[3];//��
							Data=bcd_hex(Data);
							Data+=2000;
							p8Data[1]=Data>>8;
							p8Data[2]=Data;
							Data=p8rx[2];//��
							Data=bcd_hex(Data);
							p8Data[3]=Data;//��
							Data=p8rx[1];//��
							Data=bcd_hex(Data);
							p8Data[4]=Data;//��
							break;
					}
					break;
				case 0x202C://����ǰ��Ǯ���ļ� ʣ����	double-long-unsigned����λ��Ԫ�����㣺-2���� �������	double-long-unsigned
					if(LEN_RxDataBuff<(2+5+5))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					p8Data[0]=DataType_structure;
					p8Data[1]=2;
					//ʣ����	double-long-unsigned����λ��Ԫ�����㣺-2��
					Data=p8rx[2]|(p8rx[3]<<8)|(p8rx[4]<<16)|(p8rx[5]<<24);
					Data=bcd_hex(Data);
					p8Data[2]=DataType_double_long_unsigned;
					p8Data[3]=Data>>24;
					p8Data[4]=Data>>16;
					p8Data[5]=Data>>8;
					p8Data[6]=Data;
					//�������	double-long-unsigned
					Data=p8rx[0]|(p8rx[1]<<8);
					Data=bcd_hex(Data);
					p8Data[7]=DataType_double_long_unsigned;
					p8Data[8]=Data>>24;
					p8Data[9]=Data>>16;
					p8Data[10]=Data>>8;
					p8Data[11]=Data;
					LEN_Data0+=12;
					LEN_RxDataBuff-=12;
					p16Data[0]=LEN_Data0;
					break;
			}
			break;
//		case 9://�������ݽӿ���,���ӿ�������ṩ���á��洢�������ݼ��������Ϣ
//			break;
//		case 10://�ɼ���ؽӿ���
//			break;
//		case 11://���Ͻӿ���,���ӿ�������ṩ���á��洢�ն˲ɼ����ݼ��������Ϣ
//			break;
//		case 12://��������ӿ���
//			break;
//		case 13://���ɿ��ƶ���ӿ���,���ӿ�������ṩ���ɹ����йصĿ��ƹ���
//			break;
//		case 14://����ͳ�ƽӿ���,���ӿ�������ṩ�������ò����ж���ͳ����Խ����ص���Ϣ
//			break;
//		case 15://�ۼ�ƽ���ӿ���,���ӿ�������ṩ����ͬ�������Ե���ֵ�����ۼӡ�ƽ�������㹦��
//			break;
//		case 16://��ֵ���߽ӿ���,���ӿ�������ṩ�ɼ������������Сֵ���䷢��ʱ��
//			break;
//		case 17://��ʾ�ӿ���,���ӿ�������ṩ���ն���ʾ������ӡ��ص���Ϣ
//			break;
//		case 18://�ļ�����ӿ���,���ӿ�������ṩ�ն�ʵ���ϴ��������ļ��Ĺ���
//			break;
//		case 19://�豸����ӿ���,
//			break;
//		case 20://Ӧ�����ӽӿ���
//			break;
//		case 21://ESAM�ӿ���
//			break;
//		case 22://��������豸�ӿ���
//			break;
//		case 23://�ܼ���ӿ���
//			break;
//		case 24://�����¼�����ӿ���,���ӿ�������ṩ���á��洢�����¼���¼����Ϣ
//			break;
//		case 25://���߹���ͨ�Žӿ���
//			break;
//		case 26://��̫��ͨ�Žӿ���
//			break;
		default:
			//���Կ�δ�����OIֵ
			i=OI_IC(OI);//�õ��ӿ����;���:�����ʶOI
			if(i)
			{
				i=i;
			}
			return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	return 0;
}

















