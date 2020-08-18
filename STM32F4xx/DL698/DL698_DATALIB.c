

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_DATALIB.h"
#include "../Device/MEMRW.h"





#include "../DL698/DL698_DATALIB_2000.c"








__align(4) const DL698_LIB_List_TypeDef  DL698_DATALIB_List[]=
{

//�ն�����
	{(u32*)DL698_GETLIB_2131,},
	{(u32*)DL698_GETLIB_2132,},
	{(u32*)DL698_GETLIB_2133,},
	{(u32*)DL698_GETLIB_2140,},
	{(u32*)DL698_GETLIB_2141,},
	
	{(u32*)DL698_GETLIB_2200,},
	{(u32*)DL698_GETLIB_2203,},
	{(u32*)DL698_GETLIB_2204,},
	
	
	
};



DL698_SETLIB_TypeDef *Get_DL698_DATA_pLib(u32 OAD)//�õ�DATA_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
{
	u32 i;
	DL698_SETLIB_TypeDef *pLib;

	for(i=0;i<sizeof(DL698_DATALIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
	{
		pLib=(DL698_SETLIB_TypeDef *)DL698_DATALIB_List[i].pLib;
		if(OAD==pLib->OAD)
		{
			return pLib;
		}
	}
	
	return 0;
}


void DL698_DATALIB_INIT(void)//ȫ�����ݿ����б�����ݳ�ʼ��(��0)
{
	u32 i;
	DL698_SETLIB_TypeDef *pLib;
	
	for(i=0;i<sizeof(DL698_DATALIB_List)/sizeof(DL698_LIB_List_TypeDef);i++)
	{
		pLib=(DL698_SETLIB_TypeDef *)DL698_DATALIB_List[i].pLib;
		if(pLib->pInitData)
		{
			if(pLib->LEN_Sour>pLib->LENmax)
			{//ԭʼ���ݳ���>FLASHԤ������󳤶�
				continue;
			}
			MW((u32)pLib->pInitData,(u32)pLib->Addr,pLib->LEN_Sour);
		}
	}
}














