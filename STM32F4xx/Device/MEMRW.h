

#ifndef MEMRW_H
#define MEMRW_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
void MR(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//�洢����
void MW(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//�洢��д
void MC(u32 Data,u32 ADDR_Dest,u32 Byte);//�洢����0
void ME(u32 ADDR_Dest,u32 Byte);//FLASH�����
u64 MRR(u32 ADDR_Sour,u32 Byte);//�洢�������Ĵ���(<=8�ֽ�)
void MWR(u64 Data,u32 ADDR_Dest,u32 Byte);//�Ĵ���д���洢��(<=8�ֽ�)

void LCDDataOut(void);//�������������LCD��
	
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
} RTC_TypeDef;
#define RTC97x    ((RTC_TypeDef *) RTC_BA)
u32 MRR_RTC(u32* pRTC);//��RTC97x����
void MWR_RTC(u32 Data,u32* pRTC);//дRTC97x����

#endif
#endif

#ifdef __cplusplus
 }
#endif 
#endif

