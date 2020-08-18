
#include "Project.h"
#include "Terminal_AFN0D_SourceData.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "../Device/MEMRW.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Calculate/Calculate.h"
#include "KeyUser.h"




void Init_Class2SourceData(u32 Pn)//��2ԭʼ���ݳ�ʼ��(ָ��Pn����)
{
	if(Pn==0)
	{
		return;
	}
	//��1��ԭʼ
	MC(0xff,ADDR_AFN0D_Source_Start+(LEN_AFN0D_Source*(Pn-1)),LEN_AFN0D_Source);
	//��2��ԭʼ
	MC(0xff,ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1)),LEN_LastDate_Source_PerPn);
	//��3��ԭʼ
	MC(0xff,ADDR_LastDate_Source_Start+LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn+(LEN_LastDate_Source_PerPn*(Pn-1)),LEN_LastDate_Source_PerPn);

}


