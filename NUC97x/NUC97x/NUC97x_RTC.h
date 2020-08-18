
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
  u32 BKP19R;//��Ϊ19��ֻ��16��ԭSTM32F4xx�õ���19
}RTC_TypeDef;
#define RTC97x    ((RTC_TypeDef *) RTC_BA)
u32 MRR_RTC(u32* pRTC);//��RTC97x����
void MWR_RTC(u32 Data,u32* pRTC);//дRTC97x����
#endif 

void Init_NUC97x_RTC(void);//�ڲ�RTC��ʼ��
void ON_POWER(void);//���ܵ�Դ:��ЧSYS_nWAKEUP��Ϊ�͵�ƽʱ�ĵ�Դ�����Զ�����
void OFF_POWER(void);//���ܵ�Դ:��SYS_nWAKEUP=highʱ���ܹ�,��3������SYS_nWAKEUP=low�򲻹ػ��˳�
#endif
	 
	 
void SyncERTC(void);//ͬ���ⲿRTC(���ⲿд�ڲ�)
void SyncIRTC(void);//ͬ���ڲ�RTC(���ڲ�д�ⲿ)
	 
#ifdef __cplusplus
 }
#endif 
#endif

