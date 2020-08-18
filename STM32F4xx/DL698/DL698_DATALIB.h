


#ifndef DL698_DATALIB_H
#define DL698_DATALIB_H
#ifdef __cplusplus
 extern "C" {
#endif


DL698_SETLIB_TypeDef *Get_DL698_DATA_pLib(u32 OAD);//得到DATA_pLib指针;入口:OAD;返回:Lib指针(=0表示无相应的OI)
void DL698_DATALIB_INIT(void);//全部数据库中列表的数据初始化(清0)







#ifdef __cplusplus
 }
#endif
#endif












