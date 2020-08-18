
#include "Project.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_SourceData.h"



void Init_Class2Data(u32 Pn)//��2���ݳ�ʼ��(ָ���Ĳ���������,Pn=1~n)
{
#if (USER/100)==3//�û���ʶ:������
	u8 *p8;
#endif

	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
#if (USER/100)==3//�û���ʶ:������
	Init_Class2SourceData(Pn);//��2ԭʼ���ݳ�ʼ��(ָ��Pn����)
	
	Pn--;
	p8=(u8*)(ADDR_DATABUFF);
	MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),1);
//1BYTE B0Сʱ����0=��Ч1=��Ч
	p8[0]&=0xfe;
	MR(ADDR_DATABUFF+1,ADDR_TCongealCount+7,23);
//2BYTE B0-B14���߶������,b15=1��ʾ���߶��������(��ʼ��ֵ0)
//1BYTE B0-B6�ն������,B7=1��ʾ�ն��������(��ʼ��ֵ0)
	p8[3]=p8[1+7];
//1BYTE B0-B6�ն������,B7=1��ʾ�����ն��������(��ʼ��ֵ0)
	p8[4]=p8[1+7+7];
//1BYTE B0-B6�����¶������,B7=1��ʾ�¶��������(��ʼ��ֵ0)
	p8[5]=p8[1+7+7+7];
	MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),6);
#else
	Init_Class2SourceData(Pn);//��2ԭʼ���ݳ�ʼ��(ָ��Pn����)
#endif
	
}

void PnHourCongealCount(void)//�������Сʱ�������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
{
	u32 i;
	u32 x;
	u32 Pn;
	u8 *p8;
	
	Pn=0;
	while(Pn<MaxRS485AddCarrierPn)
	{
		i=LEN_DATABUFF/LEN_PnCongealCount;
		if(i>(MaxRS485AddCarrierPn-Pn))
		{
			i=MaxRS485AddCarrierPn-Pn;
		}
		MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		p8=(u8*)(ADDR_DATABUFF);
		for(x=0;x<i;x++)
		{
			p8[0]|=0x01;//B0Сʱ����0=��Ч1=��Ч,B1-B7����
			p8+=LEN_PnCongealCount;
		}
		MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		Pn+=i;
	}
}

void PnCurveCongealCount(void)//����������߶������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u32 Count;
	u8 *p8;
	
	Count=MRR(ADDR_TCongealCount+7,2);//��ǰ���߶������
	Count&=0x7fff;
	Count+=1;
	if(Count>=MaxCurveCongeal)
	{
		Count=0;
	}
	Pn=0;
	while(Pn<MaxRS485AddCarrierPn)
	{
		i=LEN_DATABUFF/LEN_PnCongealCount;
		if(i>(MaxRS485AddCarrierPn-Pn))
		{
			i=MaxRS485AddCarrierPn-Pn;
		}
		MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		p8=(u8*)(ADDR_DATABUFF+1);
		for(x=0;x<i;x++)
		{
			y=p8[0]+(p8[1]<<8);//���ʱ�Ķ������ֵ
			y&=0x7fff;
			if(y==Count)
			{//���ֻ�
				p8[0]=0;//��0,��ʾȫ���������Ч
				p8[1]=0;//��0,��ʾȫ���������Ч
			}
			p8+=LEN_PnCongealCount;
		}
		MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		Pn+=i;
	}
}

void PnCongealCount(u32 Count,u32 MaxCount,u32 Office)//��0���ܱ����ñ��ʱ�Ķ������ֵ
{
	u32 i;
	u32 x;
	u32 y;
	u32 Pn;
	u8 *p8;

	Count&=0x7f;
	Count+=1;
	if(Count>=MaxCount)
	{
		Count=0;
	}
	Pn=0;
	while(Pn<MaxRS485AddCarrierPn)
	{
		i=LEN_DATABUFF/LEN_PnCongealCount;
		if(i>(MaxRS485AddCarrierPn-Pn))
		{
			i=MaxRS485AddCarrierPn-Pn;
		}
		MR(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		p8=(u8*)(ADDR_DATABUFF+Office);
		for(x=0;x<i;x++)
		{
			y=p8[0];//���ʱ�Ķ������ֵ
			y&=0x7f;
			if(y==Count)
			{//���ֻ�
				p8[0]=0;//��0,��ʾȫ���������Ч
			}
			p8+=LEN_PnCongealCount;
		}
		MW(ADDR_DATABUFF,ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),i*LEN_PnCongealCount);
		Pn+=i;
	}

}
void PnDateCongealCount(void)//��������ն������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
{
	u32 Count;
	Count=MRR(ADDR_TCongealCount+14,1);
	PnCongealCount(Count,MaxDateCongeal,3);//��0���ܱ����ñ��ʱ�Ķ������ֵ
}

void PnRMDateCongealCount(void)//������ĳ����ն������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
{
	u32 Count;
	Count=MRR(ADDR_TCongealCount+21,1);
	PnCongealCount(Count,MaxRMDateCongeal,4);//��0���ܱ����ñ��ʱ�Ķ������ֵ
}

void PnMonthCongealCount(void)//��������¶������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
{
	u32 Count;
	Count=MRR(ADDR_TCongealCount+28,1);
	PnCongealCount(Count,MaxMonthCongeal,5);//��0���ܱ����ñ��ʱ�Ķ������ֵ
}


u32 PnHourCongeal(u32 Pn)//������Сʱ������Ч,����=0��Ч,1=��Ч
{
	u32 i;
	if(Pn==0)
	{
		return 1;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0;
	}
	Pn--;
	i=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount),1);
	i&=1;
	return i;
}


u32 PnCongealNo(u32 CongealNo,u32 PnCount,u32 Count)//���������߶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
{
	if(PnCount==Count)
	{//��ʼ�����¶���
		return 0xffffffff;
	}
	if(Count>PnCount)
	{//���¶���,��û��0
		if((CongealNo>=PnCount)&&(CongealNo<Count))
		{//�����>=��ʼ��,ͬʱ<��ǰ��
			return CongealNo;
		}
	}
	else
	{//���¶���,���ѻ�0
		if(CongealNo>=PnCount)
		{//�����>=��ʼ��
			return CongealNo;
		}
		if(CongealNo<Count)
		{//�����<��ǰ��
			return CongealNo;
		}
	}
	return 0xffffffff;
}

u32 PnCurveCongealNo(u32 CongealNo,u32 Pn)//�����㶳�����Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+1,2);
	if(PnCount==0)
	{//�޳�ʼ����ֵ
		return CongealNo;
	}	
	PnCount&=0x7fff;
	Count=MRR(ADDR_TCongealCount+7,2);
	Count&=0x7fff;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//���������߶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
	return CongealNo;
}

u32 PnDateCongealNo(u32 CongealNo,u32 Pn)//�������ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+3,1);
	if(PnCount==0)
	{//�޳�ʼ����ֵ
		return CongealNo;
	}	
	PnCount&=0x7f;
	Count=MRR(ADDR_TCongealCount+14,1);
	Count&=0x7f;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//���������߶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
	return CongealNo;
}

u32 PnRMDateCongealNo(u32 CongealNo,u32 Pn)//�����㳭���ն������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+4,1);
	if(PnCount==0)
	{//�޳�ʼ����ֵ
		return CongealNo;
	}	
	PnCount&=0x7f;
	Count=MRR(ADDR_TCongealCount+21,1);
	Count&=0x7f;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//���������߶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
	return CongealNo;
}

u32 PnMonthCongealNo(u32 CongealNo,u32 Pn)//�������¶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
{
	u32 PnCount;
	u32 Count;

	if(CongealNo==0xffffffff)
	{
		return CongealNo;
	}	
	if(Pn==0)
	{
		return CongealNo;
	}
	if(Pn>MaxRS485AddCarrierPn)
	{
		return 0xffffffff;
	}
	Pn--;
	PnCount=MRR(ADDR_PnCongealCount+(Pn*LEN_PnCongealCount)+5,1);
	if(PnCount==0)
	{//�޳�ʼ����ֵ
		return CongealNo;
	}	
	PnCount&=0x7f;
	Count=MRR(ADDR_TCongealCount+28,1);
	Count&=0x7f;
	CongealNo=PnCongealNo(CongealNo,PnCount,Count);//���������߶������Ч,����=0xffffffff��ʾ��Ч(û�ҵ�)
	return CongealNo;
}
















