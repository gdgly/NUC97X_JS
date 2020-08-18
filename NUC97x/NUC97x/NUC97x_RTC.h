
#ifndef NUC97x_RTC_H
#define NUC97x_RTC_H
#ifdef __cplusplus
 extern "C" {
#endif

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
}RTC_TypeDef;
#define RTC97x    ((RTC_TypeDef *) RTC_BA)
u32 MRR_RTC(u32* pRTC);//读RTC97x数据
void MWR_RTC(u32 Data,u32* pRTC);//写RTC97x数据
#endif 

void Init_NUC97x_RTC(void);//内部RTC初始化
void ON_POWER(void);//开总电源:无效SYS_nWAKEUP脚为低电平时的电源开关自动控制
void OFF_POWER(void);//关总电源:当SYS_nWAKEUP=high时才能关,若3抄后检测SYS_nWAKEUP=low则不关机退出
#endif
	 
	 
void SyncERTC(void);//同步外部RTC(读外部写内部)
void SyncIRTC(void);//同步内部RTC(读内部写外部)
	 
#ifdef __cplusplus
 }
#endif 
#endif

