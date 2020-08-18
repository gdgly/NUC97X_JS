
#ifndef RM_3762_RouterActive_H
#define RM_3762_RouterActive_H
#ifdef __cplusplus
 extern "C" {
#endif

void Clr_RMListRMCount(u32 ListNo);//清抄表计数相关数据;入口:ListNo=列表号从0开始 
void Load_RMCount_NoCompAddr(u32 PORTn,u32 ListNo);//得到抄表计数相关数据(不比较地址);入口:ListNo=列表号从0开始
void Load_RMCount_CompAddr(u32 PORTn,u32 ListNo);//得到抄表计数相关数据(比较地址);入口:ListNo=列表号从0开始
void Save_RMCount(u32 PORTn,u32 ListNo);//保存抄表计数相关数据;入口:ListNo=列表号从0开始
	 
void ClrALL_RMListRMCount(void);//清全部抄表计数相关数据(载波口);入口:SetNo=档案号从0开始
void RM_3762_RouterActive(u32 PORTn);//载波抄表(路由模块主导的周期抄表模式)


#ifdef __cplusplus
}
#endif
#endif

