


#ifndef EVENT_CALC_iMeter_H
#define EVENT_CALC_iMeter_H
#ifdef __cplusplus
 extern "C" {
#endif


	 
void EVENT_CALC_300F_3010(void);//电压电流逆相序事件
	 
void EventCurrentvalue_NUM(u8* pADDR,u32 Phase);//事件当前值记录表发生次数+1，入口:pADDR=记录表地址,Phase=相(b0=A相,b1=B相,b2=C相)
void EventCurrentvalue_TIME(u8* pADDR,u32 Phase,u32 s);//事件当前值记录表累计时间，入口:pADDR=记录表地址,Phase=相(b0=A相,b1=B相,b2=C相),s=时间秒
void EVENT_CALC_3003(void);//断相标志和事件(分项事件)


	 
 
	 
	 
	 
	 
#ifdef __cplusplus
 }
#endif
#endif
