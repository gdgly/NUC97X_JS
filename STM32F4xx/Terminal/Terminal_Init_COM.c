
#include "Project.h"
#include "../Terminal/Terminal_Init.h"
#include "../Device/MEMRW.h"






void Init_EnergyInc_Souser_Pn(u32 Pn)//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
{
	if((Pn==0)||(Pn>MaxRS485Pn))
	{
		return;
	}
	Pn--;
//正向有功电能增量
	MC(0xee,ADDR_PKWH_INC+((1+(9*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
//正向无功电能增量
	MC(0xee,ADDR_PKVARH_INC+((1+(8*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
//反向有功电能增量
	MC(0xee,ADDR_NKWH_INC+((1+(9*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(5*(TMaxTariff+1)));
//反向无功电能增量
	MC(0xee,ADDR_NKVARH_INC+((1+(8*(TMaxTariff+1)))*Pn)+1+(4*(TMaxTariff+1)),(4*(TMaxTariff+1)));
}

void Init_Energy_Pn(u32 Pn)//测量点日月电能量清为0(指定的Pn数据)
{
	if((Pn==0)||(Pn>MaxRS485Pn))
	{
		return;
	}
	Pn--;
//当日正向有功电能量(总、费率l～M)(测量点号)
	MC(0,ADDR_AFN0CF41+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//当日正向无功电能量(总、费率1～M)(测量点号)
	MC(0,ADDR_AFN0CF42+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//当日反向有功电能量(总、费率l～M)(测量点号)
	MC(0,ADDR_AFN0CF43+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//当日反向无功电能量(总、费率l～M)(测量点号)
	MC(0,ADDR_AFN0CF44+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//当月正向有功电能量(总、费率l～M)(测量点号)
	MC(0,ADDR_AFN0CF45+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//当月正向无功电能量(总、费率1～M)(测量点号)
	MC(0,ADDR_AFN0CF46+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//当月反向有功电能量(总、费率1～M)(测量点号)
	MC(0,ADDR_AFN0CF47+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
//当月反向无功电能量(总、费率l～M)(测量点号)
	MC(0,ADDR_AFN0CF48+((1+(4*(TMaxTariff+1)))*Pn),1+(4*(TMaxTariff+1)));
}





