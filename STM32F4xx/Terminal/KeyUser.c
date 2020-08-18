
//�ص㻧
#include "Project.h"
#include "KeyUser.h"
#include "../Calculate/Calculate.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_CongealData_Fn.h"




u32 KeyUserPnSet(u32 Pn)//�ص㻧��Ч������ż��;����0=��Ч,1=��Ч;ʹ���ص㻧���߶���Pn�б�ADDR_KeyUserPnList
{
#if (MaxKeyUser!=MaxRS485AddCarrierPn)
	u32 i;
	u16 *p16;
	
	p16=(u16 *)(ADDR_KeyUserPnList);
	for(i=0;i<MaxKeyUser;i++)
	{
		if(p16[i]==Pn)
		{
			return 1;
		}
	}
	return 0;
#else
	if((Pn==0)||(Pn>MaxKeyUser))
	{
		return 0;
	}
	else
	{
		return 1;
	}
#endif
}


const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[]=
{
/*
//#define  ADDR_AFN0DF73     5//ADDR_CURVECONGEAL_START+5//�ܼ����й���������
	{
  	73,//�ص㻧2����������Fn
  	0,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF74      ADDR_AFN0DF73+(2*AFN0DF73MaxPn)//�ܼ����޹���������
	{
  	74,//�ص㻧2����������Fn
  	0,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF75      ADDR_AFN0DF74+(2*AFN0DF74MaxPn)//�ܼ����й�����������
	{
  	75,//�ص㻧2����������Fn
  	0,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF76      ADDR_AFN0DF75+(4*AFN0DF75MaxPn)//�ܼ����޹�����������
	{
  	76,//�ص㻧2����������Fn
  	0,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF81      ADDR_AFN0DF76+(4*AFN0DF76MaxPn)//�������й���������
	{
  	81,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF82      ADDR_AFN0DF81+(3*AFN0DF81MaxPn)//������A���й���������
	{
  	82,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF83      ADDR_AFN0DF82+(3*AFN0DF82MaxPn)//������B���й���������
	{
  	83,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF84      ADDR_AFN0DF83+(3*AFN0DF83MaxPn)//������C���й���������
	{
  	84,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF85      ADDR_AFN0DF84+(3*AFN0DF84MaxPn)//�������޹���������
	{
  	85,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF86      ADDR_AFN0DF85+(3*AFN0DF85MaxPn)//������A�޹���������
	{
  	86,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF87      ADDR_AFN0DF86+(3*AFN0DF86MaxPn)//������B�޹���������
	{
  	87,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF88      ADDR_AFN0DF87+(3*AFN0DF87MaxPn)//������C�޹���������
	{
  	88,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF89      ADDR_AFN0DF88+(3*AFN0DF88MaxPn)//������A���ѹ����
	{
  	89,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF90      ADDR_AFN0DF89+(2*AFN0DF89MaxPn)//������B���ѹ����
	{
  	90,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF91      ADDR_AFN0DF90+(2*AFN0DF90MaxPn)//������C���ѹ����
	{
  	91,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF92      ADDR_AFN0DF91+(2*AFN0DF91MaxPn)//������A���������
	{
  	92,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF93      ADDR_AFN0DF92+(3*AFN0DF92MaxPn)//������B���������
	{
  	93,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF94      ADDR_AFN0DF93+(3*AFN0DF93MaxPn)//������C���������
	{
  	94,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF95      ADDR_AFN0DF94+(3*AFN0DF94MaxPn)//�����������������
	{
  	95,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF97      ADDR_AFN0DF95+(3*AFN0DF95MaxPn)//�����������й��ܵ�����
	{
  	97,//�ص㻧2����������Fn
  	129,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF98      ADDR_AFN0DF97+(4*AFN0DF97MaxPn)//�����������޹��ܵ�����
	{
  	98,//�ص㻧2����������Fn
  	130,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF99      ADDR_AFN0DF98+(4*AFN0DF98MaxPn)//�����㷴���й��ܵ�����
	{
  	99,//�ص㻧2����������Fn
  	131,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF100      ADDR_AFN0DF99+(4*AFN0DF99MaxPn)//�����㷴���޹��ܵ�����
	{
  	100,//�ص㻧2����������Fn
  	132,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
*/
//#define  ADDR_AFN0DF101      ADDR_AFN0DF100+(4*AFN0DF100MaxPn)//�����������й��ܵ���ʾֵ
	{
  	101,//�ص㻧2����������Fn
  	129,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF102      ADDR_AFN0DF101+(4*AFN0DF101MaxPn)//�����������޹��ܵ���ʾֵ
	{
  	102,//�ص㻧2����������Fn
  	130,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF103      ADDR_AFN0DF102+(4*AFN0DF102MaxPn)//�����㷴���й��ܵ���ʾֵ
	{
  	103,//�ص㻧2����������Fn
  	131,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF104      ADDR_AFN0DF103+(4*AFN0DF103MaxPn)//�����㷴���޹��ܵ���ʾֵ
	{
  	104,//�ص㻧2����������Fn
  	132,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
/*
//#define  ADDR_AFN0DF105      ADDR_AFN0DF104+(4*AFN0DF104MaxPn)//�������ܹ�������
	{
  	105,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF106      ADDR_AFN0DF105+(2*AFN0DF105MaxPn)//������A�๦������
	{
  	106,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF107      ADDR_AFN0DF106+(2*AFN0DF106MaxPn)//������B�๦������
	{
  	107,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF108      ADDR_AFN0DF107+(2*AFN0DF107MaxPn)//������C�๦������
	{
  	108,//�ص㻧2����������Fn
  	25,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF109      ADDR_AFN0DF108+(2*AFN0DF108MaxPn)//�������ѹ��λ������
	{
  	109,//�ص㻧2����������Fn
  	0,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF110      ADDR_AFN0DF109+(6*AFN0DF109MaxPn)//�����������λ������
	{
  	110,//�ص㻧2����������Fn
  	0,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF138      ADDR_AFN0DF110+(6*AFN0DF110MaxPn)//ֱ��ģ������������
	{
  	138,//�ص㻧2����������Fn
  	0,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF145      ADDR_AFN0DF138+(2*AFN0DF138MaxPn)//������1�����޹��ܵ���ʾֵ����
	{
  	145,//�ص㻧2����������Fn
  	133,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF146      ADDR_AFN0DF145+(4*AFN0DF145MaxPn)//������4�����޹��ܵ���ʾֵ����
	{
  	146,//�ص㻧2����������Fn
  	136,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF147      ADDR_AFN0DF146+(4*AFN0DF146MaxPn)//������2�����޹��ܵ���ʾֵ����
	{
  	147,//�ص㻧2����������Fn
  	134,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
//#define  ADDR_AFN0DF148      ADDR_AFN0DF147+(4*AFN0DF147MaxPn)//������3�����޹��ܵ���ʾֵ����
	{
  	148,//�ص㻧2����������Fn
  	135,//Ҫ����1��ʵʱ����Fn,Ϊ0��ʾ��Ч����
	},
*/
//#define  ADDR_AFN0DF217      ADDR_AFN0DF148+(4*AFN0DF148MaxPn)//̨�����г����ز����ڵ����������

//#define  ADDR_AFN0DF218      ADDR_AFN0DF217+1//̨�����г����ز����ڵ�ɫ��������

};

u32 Get_KeyUserReadMeterLib_MaxFn(void)
{
  return(sizeof(KeyUserReadMeterLib)/sizeof(KeyUserReadMeterLib_TypeDef));
}



void KeyUserPnList(void)//�ص㻧���߶���Pn�б�
{
#if (MaxKeyUser!=MaxRS485AddCarrierPn)
//������
	u32 i;
	u32 SourPn;
	u32 Pn;
	u32 MeterNo;
	u32 KeyUser;
	u32 x;
	u16 *p16;
	#if IC_SST39VF6401B==1//0=û,1=��
	u8 *p8;
	#endif
#if (USER/100)==11//����ϵ
	u32 y;
#endif	
	if(Terminal_Ram->AFN04F10F35Update!=0)//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
	{
		return;
	}
	Terminal_Ram->AFN04F10F35Update=0x55;//0=AFN04F10,F35���ò������±�־,!=0û����;���ڼ��г����ص㻧���߶���Pn�б�
	
	p16=(u16*)(ADDR_KeyUserPnList);
	KeyUser=MRR(ADDR_AFN04F35,1);
	if(KeyUser>MaxKeyUser)
	{
		KeyUser=MaxKeyUser;
	}
	for(i=0;i<KeyUser;i++)
	{
		SourPn=p16[i];//ԭPn
		Pn=0xffff;
		MeterNo=MRR(ADDR_AFN04F35+1+(2*i),2);//�ص㻧�ĵ��ܱ�/��������װ�����
		if((MeterNo!=0)&&(MeterNo<=AFN04F10MaxPn))
		{
			MeterNo--;
			Pn=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*MeterNo)+2,2);//�����������
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{
				Pn=0xffff;
			}
		}
		p16[i]=Pn;
		if(SourPn!=Pn)
		{//Pnӳ���仯
			//�����߶��������������ʼֵ
			MC(0xee,ADDR_AFN0DF97_SOUR+(5*i),5);//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
			MC(0xee,ADDR_AFN0DF98_SOUR+(4*i),4);//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
			MC(0xee,ADDR_AFN0DF99_SOUR+(5*i),5);//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
			MC(0xee,ADDR_AFN0DF100_SOUR+(4*i),4);//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
			//
		}
	}
	//�����ص㻧���ڲ�����,RS485��AFN04F10������˳����Ϊ�ص㻧�������߶���,ֱ����Ч�ص㻧���ﵽMaxKeyUserΪֹ
	if(i<MaxKeyUser)
	{
		#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
			#error
		#endif
		MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
		for(MeterNo=0;MeterNo<AFN04F10MaxPn;MeterNo++)
		{
			Pn=MRR(ADDR_128KDATABUFF+(LEN_AFN04F10_Pn*MeterNo)+2,2);//�������
			if((Pn!=0)&&(Pn<=MaxRS485AddCarrierPn))
			{
				SourPn=MRR(ADDR_128KDATABUFF+(LEN_AFN04F10_Pn*MeterNo)+4,2);
				x=SourPn>>8;//ͨ��Э������://1��DL/T 645-1997.��2����������װ��ͨ��Э�飻30��DL/T 645-2007��31��"���нӿ�����խ����ѹ�ز�ͨ��ģ��"�ӿ�Э��
				SourPn&=0x1f;//�˿ں�
		#if (USER/100)==11//����ϵ
				y=MRR(ADDR_128KDATABUFF+(LEN_AFN04F10_Pn*MeterNo)+26,1);;//��С���
				if(((SourPn!=0)&&(SourPn<=3))||(x==2)||((SourPn==31)&&(x==30)&&(y==0x43)))
				{//�˿ں�1-3,���ڲ����ɻ�RS485��,���ز�07���С���Ϊ(4,3)
		#else
				if(((SourPn!=0)&&(SourPn<=3))||(x==2))
				{//�˿ں�1-3,���ڲ����ɻ�RS485��
		#endif
					for(x=0;x<i;x++)
					{
						SourPn=p16[x];
						if(SourPn==Pn)
						{//��ͬ������������ص㻧
							break;
						}
					}
					if(x>=i)
					{//ԭ�ص㻧û���������
						SourPn=p16[i];
						p16[i]=Pn;
						if(SourPn!=Pn)
						{//Pnӳ���仯
							//�����߶��������������ʼֵ
							MC(0xee,ADDR_AFN0DF97_SOUR+(5*i),5);//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
							MC(0xee,ADDR_AFN0DF98_SOUR+(4*i),4);//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
							MC(0xee,ADDR_AFN0DF99_SOUR+(5*i),5);//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
							MC(0xee,ADDR_AFN0DF100_SOUR+(4*i),4);//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
							//
						}
						i++;
						if(i>=MaxKeyUser)
						{
							break;
						}
					}
				}
			}
		}
	}
	//�����Ҳ��Ϊ�ص㻧(�������������)
	
	
	//�����ò��������Ϊ��0xff
	for(;i<MaxKeyUser;i++)
	{
		SourPn=p16[i];//ԭPn
		p16[i]=0xffff;
		if(SourPn!=0xffff)
		{//Pnӳ���仯
			//�����߶��������������ʼֵ
			MC(0xee,ADDR_AFN0DF97_SOUR+(5*i),5);//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
			MC(0xee,ADDR_AFN0DF98_SOUR+(4*i),4);//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
			MC(0xee,ADDR_AFN0DF99_SOUR+(5*i),5);//�����й��ܵ���ԭʼ,��ʼ��ֵ0xee
			MC(0xee,ADDR_AFN0DF100_SOUR+(4*i),4);//�����޹��ܵ���ԭʼ,��ʼ��ֵ0xee
			//
		}
	}
#else
//ר�䣬����
	u32 i;
	u16 *p16;
	p16=(u16 *)(ADDR_KeyUserPnList);
	for(i=0;i<MaxKeyUser;i++)
	{
		p16[i]=i+1;
	}
#endif
}

u32 KeyUserMapPn(u32 Pn)//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
{
#if (MaxKeyUser!=MaxRS485AddCarrierPn)
	u32 i;
	u16 *p16;

	p16=(u16 *)(ADDR_KeyUserPnList);
	for(i=0;i<MaxKeyUser;i++)
	{
		if(p16[i]==Pn)
		{
			return (i+1);
		}
	}
	return 0;
#else
	return Pn;
#endif
}













