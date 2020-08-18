

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../MS/SetInit.h"
#include "../Device/MEMRW.h"


typedef __packed struct
{
	u32 DataAddr;//���ݿ�ʼ��ַ
	u32 DataLen;//���ݳ���
	u16 LibLen;//�¸����ݳ���
	u8 InitDI;//��ʼ����ʶ:0=��0���¸�����,1=���¸����ݳ�ʼ��,2=���¸����ݳ�ʼ����Ϊͨ�Ų���;(���¸����ݳ���<��ʼ�ܳ���ʱ�����¸������ظ���ʼ��)
}SetLib_TypeDef;

void SetInit(u32 ADDR_LIB,u32 ADDR_LIBEnd,u32 Communicate)//������ʼ��,���ADDR_LIB������ʼ�����ַ,ADDR_LIBEnd�������ַ,Communicateͨ�Ų���0=����ʼ��,1=��ʼ��
{
	u32 i;
	u32 DataAddr;
	u32 DataLen;
	u32 LibLen;
	SetLib_TypeDef *pSetLib;
	
	pSetLib=(SetLib_TypeDef*)(ADDR_DATABUFF);
	MR(ADDR_DATABUFF,ADDR_LIB,4);
	if(pSetLib->DataAddr!=0x5555AAAA)
	{
		return;
	}
	ADDR_LIB+=4;
	while(1)
	{
		if(ADDR_LIB>=ADDR_LIBEnd)
		{
			return;
		}
		MR(ADDR_DATABUFF,ADDR_LIB,11);
		ADDR_LIB+=11;
		DataAddr=pSetLib->DataAddr;
		if(DataAddr==0)
		{
			return;
		}
		DataLen=pSetLib->DataLen;
		LibLen=pSetLib->LibLen;
		if(Communicate==0)
		{//ͨ�Ų�������ʼ��
			if(pSetLib->InitDI==2)
			{//��ǰ��ͨ�Ų���
				ADDR_LIB+=LibLen;
				continue;
			}
		}

		if(pSetLib->InitDI==0)
		{
			MC(0,DataAddr,DataLen);
		}
		else
		{
			MR(ADDR_DATABUFF,ADDR_LIB,LibLen);
			if(DataLen<=LibLen)
			{
				MW(ADDR_DATABUFF,DataAddr,DataLen);
			}
			else
			{
				while(DataLen!=0)
				{
					i=LibLen;
					while(i<DataLen)
					{
						if((i+LibLen)>LEN_DATABUFF)
						{
							break;
						}
						MW(ADDR_DATABUFF,ADDR_DATABUFF+i,LibLen);
						i+=LibLen;
					}
					if(i>DataLen)
					{
						i=DataLen;
					}
					MW(ADDR_DATABUFF,DataAddr,i);
					DataAddr+=i;
					DataLen-=i;
				}
			}
			ADDR_LIB+=LibLen;
		}
	}
}






