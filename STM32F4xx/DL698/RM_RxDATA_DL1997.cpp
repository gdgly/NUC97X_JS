
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_RxDATA.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"



u32 RM_RxDATA_DL1997_CHECK(u32 PORTn)//DL645_1997֡��������,�ȽϷ��յ�ַ�������롢���ݱ�ʶ����0x33;����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
{
	u32 i;
	u32 x;
	u8 * p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
	u8* p8;
	
	p8=p8;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
	//������
	if((p8rx[8]&0x40)!=0x0)
	{//��վ�쳣�ش�
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
	if(x!=UARTCtrl->RM_DI)
	{
		return 3;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
	}
	return 0;//����0��ȷ,1��ַ��,2�������(������),3���ݱ�ʶ��,4�����ϱ�(�Ѵ���)
}

__align(4) const u32 signed_DL1997_List[]=//DL/T-1997���ݴ����������ŵ����ݱ�ʶ�б�
{
//����
	0xB62F,//����
	0xB63F,//˲ʱ�й�����
	0xB64F,//˲ʱ�޹�����
	0xB65F,//��������
};

u32 signed_DL1997_DI(u32 PORTn)//DL/T-1997���ݴ����������ŵ����ݱ�ʶ;���:UARTCtrl->RM_DI;����:0=û���ţ�1=�з���
{
	u32 i;
	u32 x;
	u32 RMDI1;
	u32 RMDI0;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->RM_DI;
	RMDI0=i&0xff;
	RMDI1=(i>>8);
	for(i=0;i<(sizeof(signed_DL1997_List)/4);i++)
	{
		x=signed_DL1997_List[i];
		x>>=8;
		if(RMDI1==x)
		{
				x=signed_DL1997_List[i];
				x&=0xff;
				if(x==RMDI0)
				{
					return 1;
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
//LEN_RM_DI_CSD+4 �ɼ�������(����������)

u32 RxDATA_Process_DL1997(u32 PORTn,u32 Byte_DATAper,u32 DPbit,u32 DataType)
{	
	u32 i;
	u32 j;
	u32 n;
	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u32 Byte;
	u8* p8CSD;
	u8* p8Array;
	u8* p8Data;
	u8* p8rx;
	u16* p16Data;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_Rx=p8rx[9];
	if(LEN_Rx<=2)
	{//����<2
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ��峤��
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//��
		return 3;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data=(u8*)(p8CSD+LEN_RM_DI_CSD+4+LEN_Data1);
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
	
	if(LEN_Data0==0)//����CSD�Ӽ���(��DL698����ʱ)
	{
		if(UARTCtrl->RMCSDSubCount!=0)
		{
			UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
			p16Data[0]=0;
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		if(LEN_RxDataBuff<2)
		{//���峤�Ȳ���
			return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		p8Data+=LEN_Data0;
		p8Array=(u8*)p8Data;
		p8Array[0]=DataType_array;
		p8Array[1]=0;
		LEN_Data0+=2;
		LEN_RxDataBuff-=2;
		p8Data+=LEN_Data0;
	}
	else
	{
		if(p8Array[1]!=UARTCtrl->RMCSDSubCount)
		{
			UARTCtrl->RMCSDSubCount=UARTCtrl->RMCSDSubCountMax;
			p16Data[0]=0;
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		p8Data+=LEN_Data0;
	}
	
	p8rx+=12;
	LEN_Rx-=2;
	n=LEN_Rx/Byte_DATAper;//�������=���ݳ��ȳ���ÿ�����ݵ��ֽ���
	for(i=0;i<n;i++)
	{
		Data=0;
		for(Byte=0;Byte<Byte_DATAper;Byte++)
		{
			Data|=p8rx[0]<<(Byte*8);
			p8rx++;
		}
		switch(Byte_DATAper)
		{
			case 2:
				if(Data==0xffff)
				{
					Data=0;
				}
				break;
			case 3:
				if(Data==0xffffff)
				{
					Data=0;
				}
				break;
			default:
				break;
		}		
		switch(DataType)
		{
			case DataType_long_unsigned:
				Data<<=DPbit;//645С��λ��698��һ��							
				Data=bcd_hex(Data);
				if(LEN_RxDataBuff<3)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Data[0]=DataType_long_unsigned;
				p8Data[1]=Data>>8;
				p8Data[2]=Data;
				p8Data+=3;
				LEN_Data0+=3;
				LEN_RxDataBuff-=3;
				break;
			case DataType_long:
				Data<<=DPbit;//645С��λ��698��һ��							
				j=Data&(1<<((Byte_DATAper*8)-1));
				if(j)
				{//��
					Data&=~(1<<((Byte_DATAper*8)-1));//ȥ����λ
					Data=bcd_hex(Data);
					Data=~Data;
					Data++;
				}
				else
				{
					Data=bcd_hex(Data);
				}
				if(LEN_RxDataBuff<3)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Data[0]=DataType_long;
				p8Data[1]=Data>>8;
				p8Data[2]=Data;
				p8Data+=3;
				LEN_Data0+=3;
				LEN_RxDataBuff-=3;
				break;
			case DataType_double_long:
				Data<<=DPbit;//645С��λ��698��һ��							
				j=Data&(1<<((Byte_DATAper*8)-1));
				if(j)
				{//��
					Data&=~(1<<((Byte_DATAper*8)-1));//ȥ����λ
					Data=bcd_hex(Data);
					Data=~Data;
					Data++;
				}
				else
				{
					Data=bcd_hex(Data);
				}
				if(LEN_RxDataBuff<5)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Data[0]=DataType_double_long;
				p8Data[1]=Data>>24;
				p8Data[2]=Data>>16;
				p8Data[3]=Data>>8;
				p8Data[4]=Data;
				p8Data+=5;
				LEN_Data0+=5;
				LEN_RxDataBuff-=5;
				break;
			case DataType_double_long_unsigned:
				Data<<=DPbit;//645С��λ��698��һ��							
				Data=bcd_hex(Data);
				if(LEN_RxDataBuff<5)
				{//���峤�Ȳ���
					return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
				}
				p8Data[0]=DataType_double_long;
				p8Data[1]=Data>>24;
				p8Data[2]=Data>>16;
				p8Data[3]=Data>>8;
				p8Data[4]=Data;
				p8Data+=5;
				LEN_Data0+=5;
				LEN_RxDataBuff-=5;
				break;
			default:
				break;
		}
		if(p8Array[1]>=127)
		{
			return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
		p8Array[1]++;//����������
	}
	p16Data[0]=LEN_Data0;
	return 0;
}

u32 RM_RxDATA_DL1997(u32 PORTn)//����������ݴ���:����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
{
	u32 i;
	u32 OI;
	u32 Data;
	u32 LEN_Rx;
	u32 LEN_RxDataBuff;
	u32 LEN_Data0;
	u32 LEN_Data1;
	u32 signedDI;
	u8* p8CSD;
	u8* p8Array;
	u8* p8Data;
	u16* p16Data;
	u8* p8rx;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 n;
	
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	LEN_Rx=p8rx[9];
	if(LEN_Rx<=2)
	{//����<2
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LEN_RxDataBuff=Get_LEN_UARTnFnDataBuff(PORTn);//�õ�UART�������ݻ��峤��
	LEN_RxDataBuff-=(LEN_RM_DI_CSD+4);
	p8CSD=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p16Data=(u16*)(p8CSD+LEN_RM_DI_CSD);
	LEN_Data1=p16Data[1];
	if(LEN_RxDataBuff<LEN_Data1)
	{//��
		return 3;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
	}
	LEN_RxDataBuff-=LEN_Data1;
	p8Data=(u8*)(p8CSD+LEN_RM_DI_CSD+4+LEN_Data1);
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
	
//---���߿�ʼ---
	//��ʵʱ���ݶ���Ϊ����
	i=UARTCtrl->RMCSD[1];
	if(i)
	{//ROAD
		OI=UARTCtrl->RMCSD[2+0]<<24;
		OI|=UARTCtrl->RMCSD[2+1]<<16;
		OI|=UARTCtrl->RMCSD[2+2]<<8;
		OI|=UARTCtrl->RMCSD[2+3];
		if(OI==0x50020200)
		{//���Ӷ���
//			p8Data+=LEN_Data0;
//			p8Array=(u8*)p8Data;
			p8rx+=12;
			n=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
			OI=(UARTCtrl->RMCSD[7+(4*n)+0]<<24)+(UARTCtrl->RMCSD[7+(4*n)+1]<<16)+(UARTCtrl->RMCSD[7+(4*n)+2]<<8)+(UARTCtrl->RMCSD[7+(4*n)+3]<<0);
			switch(OI)
			{
				case 0x20210200://����ʱ��
					if(LEN_RxDataBuff<8)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//�õ�date_time_s��7�ֽ�
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					p16Data[0]=LEN_Data0;
					break;
				case 0x00100201://�����й��ܵ���
				case 0x00200201://�����й��ܵ���
				case 0x00500201://һ�����޹��ܵ���
				case 0x00600201://�������޹��ܵ���
				case 0x00700201://�������޹��ܵ���
				case 0x00800201://�������޹��ܵ���
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					if(LEN_RxDataBuff<(2+5+4))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=5;//���ϰ����ٶ������ݸ�����5���ַ�����ֵ���
					p8Data+=2;
					LEN_Data0+=2;
					LEN_RxDataBuff-=2;
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
					p8Data[5]=0;//���ϰ����ٶ����ַ�����ֵ���
					p8Data[6]=0;
					p8Data[7]=0;
					p8Data[8]=0;
					p8Data+=9;
					LEN_Data0+=9;
					LEN_RxDataBuff-=9;
					p16Data[0]=LEN_Data0;
					break;
				case 0x00300200://����޹�1����
				case 0x00400200://����޹�2����
					RxDATA_Process_DL1997(PORTn,4,0,DataType_double_long_unsigned);
					break;
				case 0x20170200://�й�����
				case 0x20180200://�޹�����
				case 0x11100200://���������������й�������� 
					break;
				case 0x20000200://��ѹ
				case 0x20000201://A��ѹ
					RxDATA_Process_DL1997(PORTn,2,4,DataType_long_unsigned);
					break;
				case 0x20010200://����
				case 0x20010201://A����
					RxDATA_Process_DL1997(PORTn,2,4,DataType_double_long);
					break;
				case 0x20010400://���ߵ���
					//���ߵ���������
					break;
				case 0x20040200://�й�����
				case 0x20040202://A���й�����
					RxDATA_Process_DL1997(PORTn,3,0,DataType_double_long);
					break;
				case 0x20050200://�޹�����
				case 0x20050202://A���޹�����
					RxDATA_Process_DL1997(PORTn,2,0,DataType_double_long);
					break;
				case 0x200A0200://��������
				case 0x200A0202://A�๦������
					RxDATA_Process_DL1997(PORTn,2,0,DataType_long);
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
					p16Data[0]=LEN_Data0;
					break;
			}
			return 0;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
		}
	}
//---���߽���---	
	
	signedDI=signed_DL1997_DI(PORTn);//DL/T-1997���ݴ����������ŵ����ݱ�ʶ;���:UARTCtrl->RM_DI;����:0=û���ţ�1=�з���
	i=UARTCtrl->RMCSD[1];//��ǰ���ڳ���CSD(����������)
	if(i==0)
	{//OAD
		OI=UARTCtrl->RMCSD[2]<<8;//��ǰ���ڳ���CSD
		OI|=UARTCtrl->RMCSD[3];
	}
	else
	{//ROAD
		i=UARTCtrl->RMROADCount;//����CSD��ROAD�ڹ�����OAD����(��DL698����ʱ)
		i*=4;
		OI=UARTCtrl->RMCSD[7+i]<<8;//��ǰ���ڳ���CSD
		OI|=UARTCtrl->RMCSD[7+i+1];
	}
	i=OI_IC(OI);//�õ��ӿ����;���:�����ʶOI
	LEN_Rx-=2;
	p8rx+=12;
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
					LEN_Rx/=4;//������;DL1997�������ݸ�ʽ:XXXXXX.XX
					if(LEN_RxDataBuff<(5*LEN_Rx))
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Data+=LEN_Data0;
					for(i=0;i<LEN_Rx;i++)
					{
						//97��Լ�İ��ձ�ֻ���ܡ��塢ƽ����
						if(LEN_Rx==4)
						{
							if(i==1)
							{//��Ϊ��
								LEN_Rx=5;
								p8Data[0]=0;//��
								p8Data+=1;
								LEN_Data0+=1;
								p8Array[1]++;//����������
								continue;
							}
						}
						
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
					switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
					{
						case 0://�������
							LEN_Rx/=3;//������;DL1997�������ݸ�ʽ:XX.XXXX
							if(LEN_RxDataBuff<(13*LEN_Rx))
							{//���峤�Ȳ���
								return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
							}
							p8Data+=LEN_Data0;
							for(i=0;i<LEN_Rx;i++)
							{
								//�������ֵ
								Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
								if(Data==0xffffff)
								{//��
									p8rx+=3;
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
								//����ʱ��   date_time_s
								p8Data[5]=DataType_date_time_s;
								p8Data[6]=0;//��
								p8Data[7]=0;
								p8Data[8]=0;//��
								p8Data[9]=0;//��
								p8Data[10]=0;//ʱ
								p8Data[11]=0;//��
								p8Data[12]=0;//��
								p8rx+=3;
								p8Data+=5+8;
								LEN_Data0+=2+5+8;
								if(p8Array[1]>=127)
								{
									return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
								}
								p8Array[1]++;//����������
							}
							p16Data[0]=LEN_Data0;
							break;
						case 1://����ʱ��
							if(LEN_Data0<(2+15))
							{
								break;//����д��buff����û����һ��CSD�����ݳ���
							}
							if(LEN_Data0<(2+(p8Array[1]*15)))
							{
								break;//����д��buff����û����һ��CSD�����ݳ���
							}
							if((LEN_Rx/4)>p8Array[1])
							{
								break;//����д��buff����û����һ��CSD�����ݳ���
							}
							p8Data+=LEN_Data0;
							p8Data-=(p8Array[1]*15);
							for(i=0;i<LEN_Rx;i++)
							{
								p8Data+=2+5;
								p8Data[0]=DataType_date_time_s;
								Data=0;//��
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
								p8rx+=4;
								p8Data+=8;
							}
							break;
					}
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
							switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
							{
								case 0://A���ѹ
								case 1://B���ѹ
								case 2://C���ѹ
									LEN_Rx/=2;//������;DL1997�������ݸ�ʽ:XXX
									if(LEN_RxDataBuff<(3*LEN_Rx))
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									if(Data==0xffff)
									{
										break;
									}
									Data<<=4;
									Data=bcd_hex(Data);
									p8Data[0]=DataType_long_unsigned;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									LEN_Data0+=3;
									if(p8Array[1]>=127)
									{
										return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Array[1]++;//����������
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x2001://����
							switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
							{
								case 0://A�����
								case 1://B�����
								case 2://C�����
									LEN_Rx/=2;//������;DL1997�������ݸ�ʽ:XX.XX
									if(LEN_RxDataBuff<(5*LEN_Rx))
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									Data<<=4;
									if(Data==0xffff)
									{
										break;
									}
									if(Data>>19)
									{//��
										Data<<=13;
										Data>>=13;
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
									LEN_Data0+=5;
									if(p8Array[1]>=127)
									{
										return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Array[1]++;//����������
									p16Data[0]=LEN_Data0;
									break;							
							}
							break;
						case 0x2002://��ѹ���
						case 0x2003://��ѹ�������
						case 0x200B://��ѹ����ʧ���
						case 0x200C://��������ʧ���
						default:
							return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					break;
				case 4://���ߵ���
					return 4;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
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
							switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
							{
								case 0://���й�����
								case 1://A���й�����
								case 2://B���й�����
								case 3://C���й�����
									LEN_Rx/=3;//������;DL1997�������ݸ�ʽ:XX.XXXX
									if(LEN_RxDataBuff<(5*LEN_Rx))
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8)|(p8rx[2]<<16);
									if(Data==0xffffff)
									{
										break;
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
									//Data*=1000;//��λ��W�����㣺-1
									p8Data[0]=DataType_double_long;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									LEN_Data0+=5;
									if(p8Array[1]>=127)
									{
										return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Array[1]++;//����������
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x2005://�޹�����
							switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
							{
								case 0://���޹�����
								case 1://A���޹�����
								case 2://B���޹�����
								case 3://C���޹�����
									LEN_Rx/=2;//������;DL1997�������ݸ�ʽ:XX.XX
									if(LEN_RxDataBuff<(5*LEN_Rx))
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									if(Data==0xffff)
									{
										break;
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
									//Data*=1000;//��λ��W�����㣺-1
									p8Data[0]=DataType_double_long;
									p8Data[1]=Data>>24;
									p8Data[2]=Data>>16;
									p8Data[3]=Data>>8;
									p8Data[4]=Data>>0;
									LEN_Data0+=5;
									if(p8Array[1]>=127)
									{
										return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Array[1]++;//����������
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x200A://��������
							switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
							{
								case 0://�ܹ�������
								case 1://A�๦������
								case 2://B�๦������
								case 3://C�๦������
									LEN_Rx/=2;//������;DL1997�������ݸ�ʽ:X.XXX
									if(LEN_RxDataBuff<(3*LEN_Rx))
									{//���峤�Ȳ���
										return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Data+=LEN_Data0;
									Data=p8rx[0]|(p8rx[1]<<8);
									if(Data==0xffff)
									{
										break;
									}
									if(Data>>15)
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
									p8Data[0]=DataType_long;
									p8Data[1]=Data>>8;
									p8Data[2]=Data;
									LEN_Data0+=3;
									if(p8Array[1]>=127)
									{
										return 5;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
									}
									p8Array[1]++;//����������
									p16Data[0]=LEN_Data0;
									break;
							}
							break;
						case 0x2006://���ڹ���
						case 0x2007://һ����ƽ���й�����
						case 0x2008://һ����ƽ���޹�����
						case 0x2009://һ����ƽ�����ڹ���
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
				case 0x2014://�������״̬��1
					if(LEN_RxDataBuff<5)
					{//���峤�Ȳ���
						return 2;//����:�������0=��ȷ,1=����1���Ȳ���,2=����2���Ȳ���,3=�������йس���ֵ��,4=�޶�Ӧ�ӿ����,5=����������127,6=����������������
					}
					p8Array[0]=DataType_array;
					p8Array[1]=1;
					p8Data+=2;
					p8Data[0]=DataType_bit_string;
					p8Data[1]=8;
					Data=p8rx[0];
					Data=Bit_Inverse(Data,8);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
					p8Data[2]=Data;
					LEN_Data0=5;
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
					p8Data+=LEN_Data0;
					p8Array=(u8*)p8Data;
					p8Data[0]=DataType_date_time_s;
					Get_date_time_s(p8Data+1);//�õ�date_time_s��7�ֽ�
					p8Data+=8;
					LEN_Data0+=8;
					LEN_RxDataBuff-=8;
					p16Data[0]=LEN_Data0;
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






