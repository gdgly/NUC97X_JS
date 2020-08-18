
#include "Project.h"
#include "../Terminal/Terminal_Init.h"
#include "../Device/MEMRW.h"






void Init_EnergyInc_Souser_Pn(u32 Pn)//�������������������ԭʼ������Ϊ0xee(ָ����Pn����)
{
	if((Pn==0)||(Pn>MaxRS485Pn))
	{
		return;
	}
	Pn--;
//�����й���������
	MC(0xee,ADDR_PKWH_INC+((1+(9*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
//�����޹���������
	MC(0xee,ADDR_PKVARH_INC+((1+(8*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
//�����й���������
	MC(0xee,ADDR_NKWH_INC+((1+(9*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
//�����޹���������
	MC(0xee,ADDR_NKVARH_INC+((1+(8*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
}

void Init_Energy_Pn(u32 Pn)//���������µ�������Ϊ0(ָ����Pn����)
{
	if((Pn==0)||(Pn>MaxRS485Pn))
	{
		return;
	}
	Pn--;
//���������й�������(�ܡ�����l��M)(�������)
	MC(0,ADDR_AFN0CF41+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//���������޹�������(�ܡ�����1��M)(�������)
	MC(0,ADDR_AFN0CF42+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//���շ����й�������(�ܡ�����l��M)(�������)
	MC(0,ADDR_AFN0CF43+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//���շ����޹�������(�ܡ�����l��M)(�������)
	MC(0,ADDR_AFN0CF44+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//���������й�������(�ܡ�����l��M)(�������)
	MC(0,ADDR_AFN0CF45+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//���������޹�������(�ܡ�����1��M)(�������)
	MC(0,ADDR_AFN0CF46+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//���·����й�������(�ܡ�����1��M)(�������)
	MC(0,ADDR_AFN0CF47+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//���·����޹�������(�ܡ�����l��M)(�������)
	MC(0,ADDR_AFN0CF48+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
}





