

#ifndef MEMRW_H
#define MEMRW_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
void MR(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//存储器读
void MW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//存储器写
void MC(u32 Data,u32 ADDR_Dest,u32 Byte);//存储器清0
void ME(u32 ADDR_Dest,u32 Byte);//FLASH块擦除
u64 MRR(u32 ADDR_Sour,u32 Byte);//存储器读到寄存器(<=8字节)
void MWR(u64 Data,u32 ADDR_Dest,u32 Byte);//寄存器写到存储器(<=8字节)

void LCDDataOut(void);//点阵数据输出到LCD屏
	
#ifdef IC_NUC9xx

#ifndef RTC97x
typedef struct
{
	u32 INIT;
	u32 RWEN;
	u32 FREQADJ;
	u32 TIME;
	u32 CAL;
	u32 TIMEFMT;
	u32 WEEKDAY;
	u32 TALM;
	u32 CALM;
	u32 LEAPYEAR;
	u32 INTEN;
	u32 INTSTS;
	u32 TICK;
	u32 PWRCTL;
	u32 PWRCNT;
	u32 res;
  u32 BKP0R;
  u32 BKP1R;
  u32 BKP2R;
  u32 BKP3R;
  u32 BKP4R;
  u32 BKP5R;
  u32 BKP6R;
  u32 BKP7R;
  u32 BKP8R;
  u32 BKP9R;
  u32 BKP10R;
  u32 BKP11R;
  u32 BKP12R;
  u32 BKP13R;
  u32 BKP14R;
  u32 BKP19R;//改为19因只有16个原STM32F4xx用到了19
} RTC_TypeDef;
#define RTC97x    ((RTC_TypeDef *) RTC_BA)
u32 MRR_RTC(u32* pRTC);//读RTC97x数据
void MWR_RTC(u32 Data,u32* pRTC);//写RTC97x数据

#endif
#endif

#ifdef __cplusplus
 }
#endif 
#endif

