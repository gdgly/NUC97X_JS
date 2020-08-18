

#ifndef Terminal_AFN0D_SourceData_Fn_H
#define Terminal_AFN0D_SourceData_Fn_H



typedef __packed struct
{
	vu8 Fn;//类2数据Fn
  vu32 SourAddr;//原始数据测量点1地址,当为0时表示无效
	vu16 Len;//原始数据每Pn长度
}Terminal_Class2SourceDataLib_TypeDef;


u32 GetClass2SourceDataAddr(u32 Fn,u32 Pn);//取类2原始数据存贮地址,返回0表示没找到,有效数据长度放ADDR_Terminal_ClassDataLen
void SaveClass2SourceData(u32 Fn,u32 Pn,u32 ADDR_Buff);//存贮类2载波抄表原始数据
void SaveAddDateSourceData(u32 Fn,u32 Pn,u32 ADDR_Buff);//存贮类2集中抄表补抄上2-3日电能示值原始数据;入口:Fn=0xee上2日,=0xef上3日
void SaveAddDataToMonth(u32 Fn,u32 Pn,u32 FnDataAddr,u32 OfficeAddr);//补抄到的月初1日数据转为月冻结


#endif


