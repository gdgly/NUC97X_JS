
//类2数据冻结缓冲
#ifndef AFN0D_Congeal_Buff_H
#define AFN0D_Congeal_Buff_H



//类2原始数据
#define ADDR_AFN0DF75_SOUR     ADDR_AFNOD_SOUR_Start//曲线冻结总加有功总电量原始
//总加有功总电量原始,初始化值0xee,在日冻结清零AFN0CF19时同时清0
#define ADDR_AFN0DF76_SOUR     ADDR_AFN0DF75_SOUR+(4*AFN0DF75MaxPn)//曲线冻结总加无功总电量原始
//总加无功总电量原始,初始化值0xee,在日冻结清零AFN0CF20时同时清0
#define ADDR_AFN0DF97_SOUR     ADDR_AFN0DF76_SOUR+(4*AFN0DF76MaxPn)//曲线冻结正向有功总电能量原始
//正向有功总电量原始,初始化值0xee
#define ADDR_AFN0DF98_SOUR     ADDR_AFN0DF97_SOUR+(5*AFN0DF97MaxPn)//曲线冻结正向无功总电能量原始
//正向无功总电量原始,初始化值0xee
#define ADDR_AFN0DF99_SOUR     ADDR_AFN0DF98_SOUR+(4*AFN0DF98MaxPn)//曲线冻结反向有功总电量原始
//反向有功总电量原始,初始化值0xee
#define ADDR_AFN0DF100_SOUR     ADDR_AFN0DF99_SOUR+(5*AFN0DF99MaxPn)//曲线冻结反向无功总电量原始
//反向无功总电量原始,初始化值0xee
#define ADDR_AFNOD_SOUR_End     (ADDR_AFN0DF100_SOUR+(4*AFN0DF100MaxPn))//






#define ADDR_AFN0D_Congeal_Buff_Start     ADDR_AFNOD_SOUR_End//类2数据冻结缓冲
//选类2冻结数据中最长的
#if ((LEN_DATECONGEAL>=LEN_RMDATECONGEAL)&&(LEN_DATECONGEAL>=LEN_MONTHCONGEAL)&&(LEN_DATECONGEAL>=LEN_CURVECONGEAL))
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_DATECONGEAL)
#else
 #if ((LEN_RMDATECONGEAL>=LEN_DATECONGEAL)&&(LEN_RMDATECONGEAL>=LEN_MONTHCONGEAL)&&(LEN_RMDATECONGEAL>=LEN_CURVECONGEAL))
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_RMDATECONGEAL)
 #else
  #if ((LEN_MONTHCONGEAL>=LEN_DATECONGEAL)&&(LEN_MONTHCONGEAL>=LEN_RMDATECONGEAL)&&(LEN_MONTHCONGEAL>=LEN_CURVECONGEAL))
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_MONTHCONGEAL)
  #else
#define ADDR_AFN0D_Congeal_Buff_End     (ADDR_AFN0D_Congeal_Buff_Start+LEN_CURVECONGEAL)
  #endif
 #endif
#endif

#define LEN_AFN0D_Congeal_Buff     ((ADDR_AFN0D_Congeal_Buff_End)-(ADDR_AFN0D_Congeal_Buff_Start))

#endif

















