

#include "Project.h"
#include "Terminal_ReadMeter_3762_NoRouter.h"
#include "Terminal_ReadMeter_3762.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "KeyUser.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_ReadMeter_Fn.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"

#include "Terminal_ReadMeter_ShH_Lib.h"
#include "Terminal_ReadMeter_ShH_FnData.h"
#include "Terminal_ReadMeter_ShH.h"



#if (Project/100)==2//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=
	#if (USER/100)==5//ÉÏº£
extern const u32 ShH_RMLib[];
u32 Get_RM_ShH_MaxFn(void);
	#endif

extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const u32 DL645_1997_RMLib[];
u32 Get_RM_DL645_1997_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];

#define MaxReRMCount     1//³­±íÖØ·¢´ÎÊı(½öÖØ·¢´ÎÊı,²»Ëã³õÊ¼1´ÎµÄ·¢ËÍ)


void Terminal_ReadMeter_3762_NoRouter(u32 PORTn)//ÔØ²¨³­±í(¼¯ÖĞÆ÷Ö÷µ¼µÄÖÜÆÚ³­±íÄ£Ê½)
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2ÀàÊı¾İÈÕ¶³½áÃ»³­µ½µç±íÈÕ¶³½áÊı¾İÇ°Ìî0xff;0=·ñ(ÓÃÊµÊ±Êı¾İ¶³½á),1=ÊÇ
	u32 z;
#endif
	u32 Protocol;
	u32 Fn;
	u32 Pn;
	u32 MaxFnCount;
	u32 ClassNo;
	u32 ListNo;
	u64 d64a;
	u64 d64b;
	u32 Info;
	u8 * p8RMLib;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	u8 *p8FnData;
	u32 *p32;
	u16 *p16timer;
#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
	u32 AutoReportSearch;//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
#endif
#endif

#if ((USER/100)==5)//ÉÏº£
	u8 *p8;
#endif

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	
	if(UARTCtrl->RMFlag==0x0)
	{//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
		if((Terminal_Router->StopFlags&(1<<17))==0)//ÔİÍ£Â·ÓÉ;b17=³­Ö÷¶¯ÉÏ±¨ÊÂ¼ş;Ò²×öÎª¼ä¸ôÔİÍ£Ê±½øÈëÖ÷¶¯ÉÏ±¨ÊÂ¼ş³­¶Á±êÖ¾
		{//ÔØ²¨µÈ´ıÉÏµç10ÃëÄÚÈôÓĞÖ÷¶¯ÉÏ±¨,ÓĞUARTCtrl->RMFlag=0³­±í¼ä¸ôÔİÍ£Çé¿ö,³­±í½«ÎŞ·¨Ö´ĞĞ¶øËÀÑ­»·!!!
			//ÔÊĞíÖ÷¶¯ÉÏ±¨½ÓÊÕ
			switch(UARTCtrl->Task)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				case 0://0=¿ÕÏĞ
					UARTCtrl->Task=5;//½ÓÊÕ
					UARTCtrl->RxByte=0;//RX¼ÆÊı(ÒÑ½ÓÊÕ×Ö½ÚÊı)=0
					p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
					p16timer[0]=100/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
					break;
				case 1://1=Íê³É1Ö¡·¢ÊÕ
				case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
					UARTCtrl->Task=0;
					break;
			}
			return;
		}
	}
	if(UARTCtrl->FnEnd==0)//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
	{
		y=AFN04F10MaxPn;
		if(y>100)
		{
			y=100;//±¾´Î×î´ó²éÕÒ100Ö»±í
		}
		for(x=0;x<y;x++)
		{
			UARTCtrl->MeterNo++;
			if(UARTCtrl->MeterNo>AFN04F10MaxPn)
			{
				UARTCtrl->MeterNo=0;//Íê³É1¸öÑ­»·³­±í,µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ=0
			#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
				DelRMEvene(PORTn);//É¾³ı¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ
			#endif
				/*
				for(i=0;i<Terminal_Router->NoteNum;i++)
				{
					if(Terminal_Note->List[i].RMFLAGS!=0xff)//³­±í±êÖ¾:0=ÉÏµç»òÈÕ¸üĞÂ,1=ÃüÁîÇ¿ÖÆÖØ³­,2=³­±í¼ä¸ôÖÜÆÚ;0xfe=ÒÑ³­¹ıµ«Ã»³É¹¦,0xff=³É¹¦Íê³É³­±í
					{//ÓĞ±íÃ»³­¶Á³É¹¦,²»ÔİÍ£
						return;
					}
				}
				i=UARTCtrl->RMComplete;
				if((i&0x0f)<0x0f)
				{
					i+=1;//Íê³ÉÑ­»·³­±í´ÎÊı+1
				}
				i|=0x80;
				UARTCtrl->RMComplete=i;
				UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
				*/
				return;
			}
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);//¶Ë¿ÚºÅ
			i&=0x1f;
			if(i==31)
			{
				Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//ËùÊô²âÁ¿µãºÅ BIN 2 
				if(Pn!=0x0)
				{//ËùÊô²âÁ¿µãºÅ!=0x0
					if(Pn<=MaxRS485AddCarrierPn)
					{
						//Æô¶¯·¢ËÍ
						UARTCtrl->FnEnd=1;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
						return;
					}
				}
			}
		}
		return;
	}
	if((Terminal_Ram->CongealFlags&0x1f)!=0x1f)//B0=Ğ¡Ê±¶³½á,B1=ÇúÏß¶³½á,B2=ÈÕ¶³½á,B3=³­±íÈÕ¶³½á,B4=ÔÂ¶³½á
	{//³­±íÔÚ¶³½áºó½øĞĞ
		return;
	}
	
//³­±í	
	switch(UARTCtrl->Task)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
	{
		case 0://0=¿ÕÏĞ
			if(UARTCtrl->RMTaskC!=0)//µ±Ç°³­±íÈÎÎñºÅ:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=¶¨Ê±ÈÎÎñ,(MaxRMTask-1)=Ñ­³­ÈÎÎñ(Ö»ÓĞÑ­³­ÈÎÎñ²Å¿É²åÈë¸ßÓÅÏÈ¼¶³­±í)
			{//²»ÊÇÆô³­
				i=MRR(ADDR_TYMDHMS+1,2);
			#if ((USER/100)==5)//ÉÏº£
				p8=(u8*)(ADDR_AFN04F242);//³­±íÑÓÊ±·ÖÖÓÊı
				x=p8[0];
				if(x>=60)//³­±íÑÓÊ±·ÖÖÓÊı£º1-60·ÖÖÓ
				{
					x=0x100;//1:00
				}
				else
				{
					x=hex_bcd(x);
				}
				if((i>=RMM_EndHM)||(i<x))
				{//ÔÚÃ¿ÈÕ0µã¸½½üÔİÍ£³­±í
					WarningOccur(PORTn,(u8 *)"ÔØ²¨Í£³­µÈ´ıÊ±ÖÓ¹ı0");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
					return;
				}
			#else
				if((i>=RMM_EndHM)||(i<RMM_StartHM))
				{//ÔÚÃ¿ÈÕ0µã¸½½üÔİÍ£³­±í
					WarningOccur(PORTn,(u8 *)"ÔØ²¨Í£³­µÈ´ıÊ±ÖÓ¹ı0");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
					return;
				}
			#endif
			}
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//È¡´Ó½ÚµãµØÖ·
			x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
			for(ListNo=0;ListNo<x;ListNo++)
			{
				d64b=p32[1]&0xffff;
				d64b<<=32;
				d64b|=p32[0];
				if(d64a==d64b)
				{
					break;
				}
				p32+=(LEN_NoteList/4);
			}
			if(ListNo==x)
			{//Ã»ÕÒµ½ÄÚ´æÖĞ´Ó½ÚµãÁĞ±í
				UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
				return;
			}
		#if EventProject==0//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
			if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]>=2)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
			{//±¾ÈÎÎñ¼º³É¹¦³­±í,Ö±½Ó·µ»Ø³­¶Á³É¹¦
				UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
				return;
			}
		#else
			if((Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]>=2)&&(UARTCtrl->RMMeterEvent==0))//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
			{//±¾ÈÎÎñ¼º³É¹¦³­±í,Ö±½Ó·µ»Ø³­¶Á³É¹¦
				if((Terminal_Router->StopFlags&(1<<17))==0)//ÔİÍ£Â·ÓÉ;b17=³­Ö÷¶¯ÉÏ±¨ÊÂ¼ş;Ò²×öÎª¼ä¸ôÔİÍ£Ê±½øÈëÖ÷¶¯ÉÏ±¨ÊÂ¼ş³­¶Á±êÖ¾
				{
					UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
					return;
				}
			}
		#endif
			p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//±¾ĞòºÅµÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÉ¾³ı
				goto END_Fn;//²»³­
			}
			x=UARTCtrl->MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
			if((x==0)||(x>AFN04F10MaxPn))
			{//ĞòºÅ´í,²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
				goto END_Fn;
			}
		#if (USER/100)!=0//µç¿ÆÔº²âÊÔÉèÖÃF33Ö»³­ÖØµã±íBUG
			i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//ÖÕ¶Ë³­±íÔËĞĞ²ÎÊıÉèÖÃ,¶Ë¿Ú31
			if((i&0x02)!=0x0)
			{//D1ÖÃ"1"ÒªÇóÖÕ¶ËÖ»³­ÖØµã±í£¬ÖÃ"0"ÒªÇóÖÕ¶Ë³­ËùÓĞ±í
				y=MRR(ADDR_AFN04F35,1);//Ì¨Çø¼¯ÖĞ³­±íÖØµã»§ÉèÖÃ
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MRR(ADDR_AFN04F35+1+(2*i),2)==x)
					{
						break;
					}
				}
				if(i==y)
				{//²»ÊÇÖØµã»§
					goto END_Fn;//²»³­
				}
			}
			i=MRR(ADDR_AFN04F30+(Pn-1),1);//Ì¨Çø¼¯ÖĞ³­±íÍ£³­/Í¶³­ÉèÖÃ(²âÁ¿µãºÅ)
			if(i!=0)
			{//Í£³­
				goto END_Fn;//²»³­
			}
		#endif
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
			switch(Protocol)
			{
				case 1://DL/T645-1997
					MaxFnCount=Get_RM_DL645_1997_MaxFn();
					break;
			#if ((USER/100)==5)//ÉÏº£ÓÃ»§
				case 21://ÉÏº£¹æÔ¼
					MaxFnCount=Get_RM_ShH_MaxFn();
					break;
			#endif
				default://DL/T645-2007
					MaxFnCount=Get_RM_DL645_2007_MaxFn();
					break;
			}
			if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
			{//Fn¼ÆÊı>=×î´óFn¸öÊı,±¾±í³­±íÍê³É
	END_Fn:	
			#if EventProject==0//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
				if(Terminal_Note->List[ListNo].RMErr==0)//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
				{//³­±í³É¹¦
					Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=2;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				}
				else
				{//ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=1;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				}
			#else
				if(UARTCtrl->RMMeterEvent==0)//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
				{
					if(Terminal_Note->List[ListNo].RMErr==0)//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					{//³­±í³É¹¦
						Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=2;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
					}
					else
					{//ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
						Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=1;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
					}
				}
			#endif
				Terminal_Note->List[ListNo].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
				Terminal_Note->List[ListNo].FnCount=0;//Fn¼ÆÊı=0
				Terminal_Note->List[ListNo].RMCount=0;//³­±íÃüÁî¼ÆÊı=0
				Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
				UARTCtrl->SearchDICount=0;//ÖÜÆÚÊÂ¼ş³­¶ÁAFN04F106ÉèÖÃËÑË÷¼ÆÊı
				UARTCtrl->RMDICount=0;//ÖÜÆÚÊÂ¼ş³­¶ÁÒÑ³­DI¼ÆÊı
				UARTCtrl->RMDI=0xffffffff;//ÖÜÆÚÊÂ¼ş³­¶ÁµÄÊı¾İ±êÊ¶,0xffffffff±íÊ¾ÎŞĞ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊı¾İ±êÊ¶²»ÊÇÓÃ¿âÖĞµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼şÀàµÄ³­¶ÁÊı¾İ±êÊ¶
				
				UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
				Terminal_Note->List[ListNo].RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
				UARTCtrl->RMMeterEvent=0;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
				return;
			}
			
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//È¡´óĞ¡ÀàºÅ
		#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
		#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
			AutoReportSearch=0;//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
		#endif
		#endif
			while(1)
			{
				if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
				{//Fn¼ÆÊı>=×î´óFn¸öÊı,±¾±í³­±í½áÊø
					goto END_Fn;
				}
				switch(Protocol)
				{
					case 1://DL/T645-1997
						i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
						break;
				#if ((USER/100)==5)//ÉÏº£ÓÃ»§
					case 21://ÉÏº£¹æÔ¼
						i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
						break;
				#endif
					default://DL/T645-2007
						i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
						break;
				}

				if(i==0)
				{//±¾FnÊÇ¿Õ
		NEXT_Fn:
					Terminal_Note->List[ListNo].FnCount++;//Fn¼ÆÊı+1
					Terminal_Note->List[ListNo].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					Terminal_Note->List[ListNo].ReRMCount=0;//ÖØ·¢¼ÆÊı
					Terminal_Note->List[ListNo].RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı

		#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
		#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
					if((UARTCtrl->RMMeterEvent&1)==0)//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
					{//0=·ñ
						UARTCtrl->SearchDICount=0;//ÖÜÆÚÊÂ¼ş³­¶ÁAFN04F106ÉèÖÃËÑË÷¼ÆÊı
						UARTCtrl->RMDICount=0;//ÖÜÆÚÊÂ¼ş³­¶ÁÒÑ³­DI¼ÆÊı
						UARTCtrl->RMDI=0xffffffff;//ÖÜÆÚÊÂ¼ş³­¶ÁµÄÊı¾İ±êÊ¶,0xffffffff±íÊ¾ÎŞĞ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊı¾İ±êÊ¶²»ÊÇÓÃ¿âÖĞµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼şÀàµÄ³­¶ÁÊı¾İ±êÊ¶
						if(UARTCtrl->RMTaskC!=0)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
						{//²»ÊÇÆô³­
							if(AutoReportSearch==0)//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
							{
								AutoReportSearch=1;//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
								x=Check_Meter_Event_Save();//¼ì²éµçÄÜ±íÊÂ¼ş´æÖü¿Õ¼ä;·µ»Ø0=´æ´¢M(Ã»ÉÏ±¨),1-255ÔÊĞí»¹¿É´æ´¢µÄÌõÊı
								if(x>=30)
								{//¿Õ¼ä¿É´æ>=30Ìõ,Ã¿¿Ú10Ìõ

										x=Check_AutoReportWord(PORTn);//¼ì²éµçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;·µ»Ø:0=ÎŞ,!=0 B31-B16ÎªÅäÖÃĞòºÅ(1¿ªÊ¼),B15-B0ÎªÒª¶ÁÈ¡µÄÖ÷¶¯ÉÏ±¨×´Ì¬×ÖÎ»ºÅ(0-95)
										x>>=16;
										if(x!=0)
										{//ÓĞÖ÷¶¯ÉÏ±¨ĞèÒª¶Á
											UARTCtrl->MeterNoBackupOfAutoReport=UARTCtrl->MeterNo;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
											UARTCtrl->MeterNo=x;
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//È¡´Ó½ÚµãµØÖ·
											x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
											if(x>MaxRS485AddCarrierPn)
											{
												x=MaxRS485AddCarrierPn;
											}
											p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
											for(ListNo=0;ListNo<x;ListNo++)
											{
												d64b=p32[1]&0xffff;
												d64b<<=32;
												d64b|=p32[0];
												if(d64a==d64b)
												{
													break;
												}
												p32+=(LEN_NoteList/4);
											}
											Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
											Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
									#if ((USER/100)==5)//ÉÏº£ÓÃ»§
											if((ListNo==x)||(Protocol==1)||(Pn==0)||(Protocol==21))
									#else
										#if(USER/100)==7//Õã½­ÓÃ»§
											if((ListNo==x)||(Pn==0))
										#else
											if((ListNo==x)||(Protocol==1)||(Pn==0))
										#endif
									#endif
											{//Ã»ÕÒµ½ÄÚ´æÖĞ´Ó½ÚµãÁĞ±í»òDL645-1997»ò²âÁ¿µãÉ¾³ı
												//ÇåÊÂ¼ş±êÖ¾,ÃâËÀÑ­»·
												MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
												UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
												return;
											}
											Terminal_Note->List[ListNo].FnCountBackupOfAutoReport=Terminal_Note->List[ListNo].FnCount;//485³­±íFn¼ÆÊı(Ô­³£¹æ³­±íµÄFn¼ÆÊı¼Ä´æ)
											Terminal_Note->List[ListNo].FnCount=2;
											
											AutoReportSearch=0;//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
											UARTCtrl->RMMeterEvent|=1;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
											return;
										}

								}
								Terminal_Router->StopFlags&=~(1<<17);//ÔİÍ£Â·ÓÉ;b17=³­Ö÷¶¯ÉÏ±¨ÊÂ¼ş
							}
							if(Protocol==30)//30=DL/T645-2007
							{//µ±Ç°Ö»ÓĞ2007¿â²ÅÓĞF2(µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş)
								p8RMLib=(u8 *)DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
								if(p8RMLib[0]==2)//F2
								{
									Terminal_Note->List[ListNo].FnCount++;//Fn¼ÆÊı+1;·ÇÖ÷¶¯ÊÂ¼şÌø¹ı²»³­
								}
							}
						#if(USER/100)==7//Õã½­ÓÃ»§
							if(Protocol==1)//1=DL/T645-1997
							{//Õã½­1997¿âÒ²ÓĞF2(µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş)
								MaxFnCount=Get_RM_DL645_1997_MaxFn();
								if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
								{//Fn¼ÆÊı>=×î´óFn¸öÊı,±¾±í³­±í½áÊø
									goto END_Fn;
								}
								p8RMLib=(u8 *)DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
								if(p8RMLib[0]==2)//F2
								{
									Terminal_Note->List[ListNo].FnCount++;//Fn¼ÆÊı+1;·ÇÖ÷¶¯ÊÂ¼şÌø¹ı²»³­
								}
							}
						#endif
						}
					}
					else
					{//ÊÇ
						UARTCtrl->RMMeterEvent&=0xfe;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
						UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
						Terminal_Note->List[ListNo].FnCount=Terminal_Note->List[ListNo].FnCountBackupOfAutoReport;//485³­±íFn¼ÆÊı(Ô­³£¹æ³­±íµÄFn¼ÆÊı¼Ä´æ)
						if((Terminal_Router->StopFlags&(1<<17))!=0)//ÔİÍ£Â·ÓÉ;b17=³­Ö÷¶¯ÉÏ±¨ÊÂ¼ş;Ò²×öÎª¼ä¸ôÔİÍ£Ê±½øÈëÖ÷¶¯ÉÏ±¨ÊÂ¼ş³­¶Á±êÖ¾
						{//
							x=Check_Meter_Event_Save();//¼ì²éµçÄÜ±íÊÂ¼ş´æÖü¿Õ¼ä;·µ»Ø0=´æ´¢M(Ã»ÉÏ±¨),1-255ÔÊĞí»¹¿É´æ´¢µÄÌõÊı
							if(x>=30)
							{//¿Õ¼ä¿É´æ>=30Ìõ,Ã¿¿Ú10Ìõ
								while(1)
								{
									x=Check_AutoReportWord(PORTn);//¼ì²éµçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;·µ»Ø:0=ÎŞ,!=0 B31-B16ÎªÅäÖÃĞòºÅ(1¿ªÊ¼),B15-B0ÎªÒª¶ÁÈ¡µÄÖ÷¶¯ÉÏ±¨×´Ì¬×ÖÎ»ºÅ(0-95)
									x>>=16;
									if(x==0)
									{
										break;
									}
									else
									{//ÓĞÖ÷¶¯ÉÏ±¨ĞèÒª¶Á
										UARTCtrl->MeterNoBackupOfAutoReport=UARTCtrl->MeterNo;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
										UARTCtrl->MeterNo=x;
										d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//È¡´Ó½ÚµãµØÖ·
										x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
										if(x>MaxRS485AddCarrierPn)
										{
											x=MaxRS485AddCarrierPn;
										}
										p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
										for(ListNo=0;ListNo<x;ListNo++)
										{
											d64b=p32[1]&0xffff;
											d64b<<=32;
											d64b|=p32[0];
											if(d64a==d64b)
											{
												break;
											}
											p32+=(LEN_NoteList/4);
										}
										Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
										Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
								#if ((USER/100)==5)//ÉÏº£ÓÃ»§
										if((ListNo==x)||(Protocol==1)||(Pn==0)||(Protocol==21))
								#else
										#if(USER/100)==7//Õã½­ÓÃ»§
											if((ListNo==x)||(Pn==0))
										#else
											if((ListNo==x)||(Protocol==1)||(Pn==0))
										#endif
								#endif
										{//Ã»ÕÒµ½ÄÚ´æÖĞ´Ó½ÚµãÁĞ±í»òDL645-1997»ò²âÁ¿µãÉ¾³ı
											//ÇåÊÂ¼ş±êÖ¾,ÃâËÀÑ­»·
											MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
											UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
											continue;
										}
										Terminal_Note->List[ListNo].FnCountBackupOfAutoReport=Terminal_Note->List[ListNo].FnCount;//485³­±íFn¼ÆÊı(Ô­³£¹æ³­±íµÄFn¼ÆÊı¼Ä´æ)
										Terminal_Note->List[ListNo].FnCount=2;
										
										UARTCtrl->RMMeterEvent|=1;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
										return;
									}
								}
							}
						}
						Terminal_Router->StopFlags&=~(1<<17);//ÔİÍ£Â·ÓÉ;b17=³­Ö÷¶¯ÉÏ±¨ÊÂ¼ş;Ò²×öÎª¼ä¸ôÔİÍ£Ê±½øÈëÊÂ¼ş³­¶Á±êÖ¾
						return;
					}
		#endif
		#endif
					continue;
				}
				p8RMLib=(u8 *)i;//³­±í¿â
				Fn=p8RMLib[0];//È¡ÊµÊ±Fn
				if((p8RMLib[3]&0x04)==0)//³­±íÖ§³Ö:B2=ÔØ²¨³­±í,B1=RS485³­±í,B0=ÄÚ²¿½»²É³­±í
				{
					goto NEXT_Fn;
				}
				if(p8RMLib[1]!=0)
				{//±¾Fn³­±íÃüÁîÊı!=0
					if(Terminal_Note->List[ListNo].RMCount>=p8RMLib[1])
					{//³­±íÊı¾İ±êÊ¶¼ÆÊı>=FnĞè·¢ÃüÁîÊı;±¾Fn³­±íÍê³É
						if(p8RMLib[1]>Terminal_Note->List[ListNo].RMCountErr)
						{//²»ÊÇÈ«²¿±êÊ¶È«²¿³­¶ÁÊ§°Ü
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊı¾İ
						}
						goto NEXT_Fn;
					}
				}
				
			#if Extern_ReadMeter_Fn_YesNo==0//Íâ²¿³ÌĞò:0=I2,1=E1,°´´óĞ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊı,Protocol(1=1997,ÆäËû=2007)	
				y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,Terminal_Note->List[ListNo].RMCount,Protocol);//°´´óĞ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë
			#else
				y=0;
				if((Comm_Ram->SoftModule&(1<<4))==0)//Èí¼şÄ£¿é´íÎó±êÖ¾(ÖÃÎ»±íÊ¾´íÎó):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
				{
					p32=(u32 *)(ADDR_E1_ENTRY);
					y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)Terminal_Note->List[ListNo].RMCount,(u32)Protocol);//°´´óĞ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊı
				}
			#endif
		#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
		#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
				if(UARTCtrl->RMMeterEvent!=0)//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
				{
					y=1;//³­¶ÁÖ÷¶¯ÉÏ±¨×ÜÊÇÔÊĞí
				}
		#endif
		#endif
				if(Fn==26)
				{
					if(Protocol!=1)//(1=1997,ÆäËû=2007)
					{
					#if RMM_AFN0CF26==0//AFN0CF26¶ÏÏàÊı¾İ³­±í;0=³­µç±í¶ÏÏà¼ÇÂ¼Êı¾İ,1=³­µç±íÊ§Ñ¹¼ÇÂ¼Êı¾İ
						if(Terminal_Note->List[ListNo].RMCount==12)//³­±íÊı¾İ±êÊ¶¼ÆÊı
					#endif
					#if RMM_AFN0CF26==1//AFN0CF26¶ÏÏàÊı¾İ³­±í;0=³­µç±í¶ÏÏà¼ÇÂ¼Êı¾İ,1=³­µç±íÊ§Ñ¹¼ÇÂ¼Êı¾İ
						if(Terminal_Note->List[ListNo].RMCount==16)//³­±íÊı¾İ±êÊ¶¼ÆÊı
					#endif
						{//³­±íÃüÁî¼ÆÊı16ÒÔÉÏÊÇ¼æÈİÃüÁî,Ö»ÓĞ²»³É¹¦Ê±²Å³­
							for(i=0;i<(1+61);i++)
							{
								if(p8FnData[i]!=0xee)
								{
									break;
								}
							}
							if(i!=(1+61))
							{//ÓĞ³­³É¹¦
								y=0;//²»³­
								Terminal_Note->List[ListNo].RMCount=100;//³­±íÊı¾İ±êÊ¶¼ÆÊı
							}
						}
					}
				}
			#if RMM_DateCongeal_0xEE==0//2ÀàÊı¾İÈÕ¶³½áÃ»³­µ½µç±íÈÕ¶³½áÊı¾İÇ°Ìî0xff;0=·ñ(ÓÃÊµÊ±Êı¾İ¶³½á),1=ÊÇ
				if(y!=0)
				{
				//À©Õ¹14 ²¹³­ÉÏ2ÈÕÈÕ¶³½áÊı¾İ
				//À©Õ¹15 ²¹³­ÉÏ3ÈÕÈÕ¶³½áÊı¾İ
					if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
					{
						y=0;//
							x=MRR(ADDR_TYMDHMS+3,3);//È¡µ±Ç°ÄêÔÂÈÕ
							x=YMD_Sub1D(x);//ÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÔÂÈÕ
							if(p8RMLib[0]==0xEE)
							{
								y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*(Pn-1));
							}
							else
							{
								x=YMD_Sub1D(x);//ÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÔÂÈÕ
								y=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+(LEN_LastDate_Source_PerPn*(Pn-1));
							}
							y=MRR(y,3);//È¡ÉÏ?ÈÕ²¹³­Ê±±ê
							if(x==y)
							{//¼º³­
								goto NEXT_Fn;
							}
							x=YMD_Sub1D(x);//ÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÔÂÈÕ
							i=GetDateCongealNo(x);//µÃµ½ÈÕ¶³½áÏàÍ¬Ê±±êµÄ¶³½áºÅ,·µ»Ø=0xffffffff±íÊ¾Ã»ÕÒµ½
							if(i!=0xffffffff)
							{//ÕÒµ½
								z=Terminal_Note->List[ListNo].RMCount;//³­±íÃüÁî¼ÆÊı
								if((z>0)&&(z<9))
								{
									z+=160;//³­±íÃüÁî¼ÆÊı1-8¶ÔÓ¦Fn=161-168
									z=GetClass2DataOfficeAddr(z,Pn);//È¡Àà2Êı¾İ´æÖüÆ«ÒÆµØÖ·,·µ»Ø0±íÊ¾Ã»ÕÒµ½
									if(z!=0)
									{//ÕÒµ½
										y=MRR(ADDR_TYMDHMS+3,3);//È¡µ±Ç°ÄêÔÂÈÕ
										z=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+z+2,3);//È¡Ô­³­±íÊ±¼äÄêÔÂÈÕ
										if((z!=0xeeeeee)&&(z!=0xffffff))
										{
											if((z>x)&&(z<=y))
											{//Ô­³­±íÊ±¼ä>ÉÏ2ÈÕÊ±±ê,Í¬Ê±<=µ±Ç°Ê±¼ä
												goto NEXT_Fn;//¼º³­
											}
										}
									}
								}
							}
							while(1)
							{
								i=Terminal_Note->List[ListNo].RMCount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
								y=1;//³­±íÊı¾İ±êÊ¶¼ÆÊı=0ºÍ9×ÜÊÇÒª³­
								if(i>9)
								{
									break;
								}
								if((i>0)&&(i<9))
								{
									i+=160;//Òª³­µÄFn=161-168
									y=Class2Data_Fn(ClassNo,i);//2ÀàÊı¾İFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö		
								}
								if(y==0)
								{
									Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊı¾İÌîÈë
									Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı
								}
								else
								{
									break;
								}
							}
							if(Terminal_Note->List[ListNo].RMCount>=p8RMLib[1])
							{//³­±íÊı¾İ±êÊ¶¼ÆÊı>=FnĞè·¢ÃüÁîÊı;±¾Fn³­±íÍê³É
								if(p8RMLib[1]>Terminal_Note->List[ListNo].RMCountErr)
								{//²»ÊÇÈ«²¿±êÊ¶È«²¿³­¶ÁÊ§°Ü
									ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊı¾İ
								}
								goto NEXT_Fn;
							}
					}
				}
			#endif
				if(y!=0)
				{
					if((p8RMLib[0]==0xE3)||(p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
					{//³­ÉÏ1-3ÈÕÈÕ¶³½áÊı¾İ
					#if ERC39==1
						if(p8RMLib[0]==0xEF)
						{
							if(Terminal_Note->List[ListNo].RMCount<=1)
							{
								ERC39_Event(Pn);//²¹³­Ê§°ÜÊÂ¼ş¼ÇÂ¼
							}
						}
					#endif
					#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
						if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
						{
							i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//µçÄÜ·ÑÂÊ¸öÊı
							if(i==1)
							{//³­ÉÏ2-3ÈÕÈÕ¶³½áÊı¾İ
								y=0;//ÉÂÎ÷Å©Íø±íÎŞÈÕ¶³½á
							}
						}
					#endif
						if(Terminal_Note->List[ListNo].RMCount==9)
						{//ÔÙ´ÎÈ·ÈÏÈÕ¶³½áÊ±¼äYYMMDDhhmm
							//Ê¡ÂÔ²»³­,Òò³­±íÔÚ¹ı0µãºóÑÓÊ±½øĞĞ,ÔÚ´ËÅĞ¶Ï²»ĞŞ¸Ä³­±í¿â¼õÉÙ´íÎó
							y=0;
						}
					}
				}
				if(y==0x0)
				{//±¾ÃüÁî²»³­
					if(p8RMLib[1]==0x0)
					{//±¾Fn³­±íÃüÁîÊı=0
						goto NEXT_Fn;
					}
					if(Terminal_Note->List[ListNo].RMCount==0)//³­±íÊı¾İ±êÊ¶¼ÆÊı
					{//³­±íÊı¾İ±êÊ¶¼ÆÊı=0,ÇåFnÊı¾İ»º³å
						Terminal_Note->List[ListNo].RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
						MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
					}
					Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊı¾İÌîÈë
					Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					Terminal_Note->List[ListNo].ReRMCount=0;//ÖØ·¢¼ÆÊı
					continue;
				}
				if(p8RMLib[1]==0x0)
				{//±¾Fn³­±íÃüÁîÊı=0
				#if AFN0CF33MaxPn==0
					goto NEXT_Fn;
				#else
					if((p8RMLib[4]&0x01)!=0)//Êı¾İ·ÖÀà:B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
					{//1ÀàÊı¾İ,ÊµÊ±
						switch(Fn)
						{
						//F33 µ±Ç°ÕıÏòÓĞ/ÎŞ¹¦£¨×éºÏÎŞ¹¦1£©µçÄÜÊ¾Öµ¡¢Ò»/ËÄÏóÏŞÎŞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
						//F34 µ±Ç°·´ÏòÓĞ/ÎŞ¹¦£¨×éºÏÎŞ¹¦2£©µçÄÜÊ¾Öµ¡¢¶ş/ÈıÏóÏŞÎŞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
						//F35 µ±ÔÂÕıÏòÓĞ/ÎŞ¹¦×î´óĞèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
						//F36 µ±ÔÂ·´ÏòÓĞ/ÎŞ¹¦×î´óĞèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
							case 33:
							case 34:
							case 35:
							case 36:
						//F37 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©ÕıÏòÓĞ/ÎŞ¹¦£¨×éºÏÎŞ¹¦1£©µçÄÜÊ¾Öµ¡¢Ò»/ËÄÏóÏŞÎŞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
						//F38 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©·´ÏòÓĞ/ÎŞ¹¦£¨×éºÏÎŞ¹¦2£©µçÄÜÊ¾Öµ¡¢¶ş/ÈıÏóÏŞÎŞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
						//F39 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©ÕıÏòÓĞ/ÎŞ¹¦×î´óĞèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
						//F40 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©·´ÏòÓĞ/ÎŞ¹¦×î´óĞèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜMÜ12©
							case 37:
							case 38:
							case 39:
							case 40:
								break;
							default:
								goto NEXT_Fn;
						}
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊı¾İ
						goto NEXT_Fn;
					}
				#endif
				}

				if(Terminal_Note->List[ListNo].RMCount==0)//³­±íÊı¾İ±êÊ¶¼ÆÊı
				{///³­±íÊı¾İ±êÊ¶¼ÆÊı=0,ÇåFnÊı¾İ»º³å
					Terminal_Note->List[ListNo].RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				
				if(Terminal_Note->List[ListNo].ReRMCount>MaxReRMCount)
				{//³­±íÖØ·¢´ÎÊı>MaxReRMCount
			#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
					if(p8RMLib[0]==2)//³­¶ÁÖ÷¶¯ÉÏ±¨ÊÂ¼ş
					{
						//ÇåÊÂ¼ş±êÖ¾,ÃâËÀÑ­»·
						MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
						goto NEXT_Fn;
					}
			#endif
					ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.Ê§°Ü
					if(p8RMLib[0]==129)
					{
						ClrERC29SOUR(Pn);//¼¯ÖĞ³­±íÊ§°ÜÇåERC29µçÄÜ±í·É×ßÊÂ¼şÔ­Ê¼Êı¾İ
					}
					//ÈÕ³­±í±êÖ¾
					if(UARTCtrl->RMTaskC==1)//µ±Ç°³­±íÈÎÎñºÅ:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=,(MaxRMTask-1)=Ñ­³­ÈÎÎñ(Ö»ÓĞÑ­³­ÈÎÎñ²Å¿É²åÈë¸ßÓÅÏÈ¼¶³­±í)
					{
						if(UARTCtrl->DateReRMCount>=3)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
						{
							DateRMFlags(Pn,1);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
						}
					}
					Terminal_Note->List[ListNo].RMErr=1;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
					Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					Terminal_Note->List[ListNo].ReRMCount=0;//ÖØ·¢¼ÆÊı
					
					//ÏÈ³­ÆäËû±í
					goto END_Fn;
				}
				Terminal_Note->List[ListNo].ReRMCount++;//³­±íÖØ·¢¼ÆÊı+1
				
				if(p8RMLib[0]==0xED)
				{//À©Õ¹13 ÉÏ1-24Õûµãµç±í¶³½áµÄÕı·´ÓĞ¹¦µçÄÜÊ¾Öµ
					//Fn101ÕıÓĞ¹¦µçÄÜÊ¾ÖµÇúÏßÅäÖÃ
					y=Class2Data_Fn(ClassNo,101);//2ÀàÊı¾İFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
					//Fn103·´ÓĞ¹¦µçÄÜÊ¾ÖµÇúÏßÅäÖÃ
					y|=Class2Data_Fn(ClassNo,103);//2ÀàÊı¾İFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
					if(y==0)
					{
						goto NEXT_Fn;
					}
					if(Terminal_Note->List[ListNo].RMCount==0)
					{//½öµ±ÃüÁî¼ÆÊı=0Ê±ÁĞ±í,¼õÉÙÔËĞĞÊ±¼ä
						KeyUserPnList();//¼¯ÖĞ³­±íÖØµã»§ÇúÏß¶³½áPnÁĞ±í
					}
					i=MRR(ADDR_TYMDHMS+2,4);//È¡µ±Ç°ÄêÔÂÈÕÊ±
					d64a=YMDH_SubH(i,Terminal_Note->List[ListNo].RMCount);//ÄêÔÂÈÕÊ±¼õhÊ±,·µ»Ø¼õhÊ±ºóµÄÄêÔÂÈÕÊ±
					d64a<<=8;//·Ö=0
					i=CheckCurveCongeal(d64a,101,Pn);//¼ì²éÇúÏß¶³½á;·µ»Ø:0=Êı¾İÃ»¶³½á,1=Êı¾İÒÑ¶³½á,2=Ã»ÕÒµ½ÓĞĞ§¶³½á
					if(i!=0)
					{//ÒÑ¶³½á»òÃ»ÕÒµ½¶³½áºÅ
						Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
						Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı
						Terminal_Note->List[ListNo].ReRMCount=0;//ÖØ·¢¼ÆÊı
						continue;
					}					
				}
		
				d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
				switch(Protocol)
				{
					case 1://DL/T645-1997
						i=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//È¡DI
						Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
						//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
						i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];//·µ»Ø³¤¶È
						if(i==0)
						{//·µ»Ø³¤¶È=0
							i=10+(3*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
							i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
						}
						i+=14;
						Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
						if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
						{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
								Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
							}
						}
				#if(USER/100)==7//Õã½­ÓÃ»§
					#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
						if(p8RMLib[0]==2)//F2 µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş
						{
							//È¡DI	
							x=(u32)p8RMLib;
							x+=10+(3*Terminal_Note->List[ListNo].RMCount);
							x=MRR(x,2);
							if(x==0)
							{//±£Áô
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+(Terminal_Note->List[ListNo].RMCount/2),1);
								Terminal_Note->List[ListNo].RMCount++;
								if(Terminal_Note->List[ListNo].RMCount&1)
								{
									Terminal_Note->List[ListNo].RMCount++;
								}
								Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı
								return;
							}
							i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+(Terminal_Note->List[ListNo].RMCount/2),1);
							if(i==0)
							{//²»Ğè¶Á
								Terminal_Note->List[ListNo].RMCount++;
								if(Terminal_Note->List[ListNo].RMCount&1)
								{
									Terminal_Note->List[ListNo].RMCount++;
								}
								Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı
								return;
							}
							MC(0xee,(u32)p8FnData,4);//µÚ1Ö¡±êÖ¾
						}
					#endif
				#endif
						break;
				#if (USER/100)==5//ÉÏº£ÓÃ»§
					case 21://ÉÏº£¹æÔ¼
						i=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//È¡DI
						Read_ShH_TxData(ADDR_DATABUFF,i,d64a);//ÉÏº£¹æÔ¼¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
						//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
						i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];//·µ»Ø³¤¶È
						i+=14;
						Uart_3762_AFN13F1_Tx(PORTn,0,ADDR_DATABUFF,14,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
						if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
						{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
								Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
							}
						}
						break;
				#endif
					default://DL/T645-2007
					#if EventProject==0//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
						//EventProject==0
						if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
						{//²»ÊÇ¶Áµç±íÇúÏß
							i=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//È¡DI
							switch(i)
							{//ÒÔÏÂÃüÁîÒòÖ¡Ì«³¤²»³­
								case 0x03300101://F27 ×î½üÒ»´Îµç±íÇåÁã·¢ÉúÊ±¿Ì
								case 0x03300201://F27 ×î½üÒ»´ÎĞèÁ¿ÇåÁã·¢ÉúÊ±¿Ì
								case 0x03300501://F166 ×î½üÒ»´ÎµçÄÜ±íÊ±¶Î²ÎÊıĞŞ¸ÄÊ±¼ä
									Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
									Terminal_Note->List[ListNo].RMCount++;//³­±íÃüÁî¼ÆÊı
									Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
									return;
							}
						#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
							x=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//µçÄÜ·ÑÂÊ¸öÊı
							if((x==1)&&(p8RMLib[0]==0xE3))
							{//±ä»»ÈÕ¶³½áÊı¾İ±êÊ¶
								switch(Terminal_Note->List[ListNo].RMCount)
								{
									default://ÈÕ¶³½áÊ±¼äYYMMDDhhmm
										Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
										Terminal_Note->List[ListNo].RMCount++;//³­±íÃüÁî¼ÆÊı
										Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
										return;
									case 1://ÉÏ1´ÎÈÕÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
										i=0x00010000;//µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
										break;
								}
							}
						#endif
							Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
							//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
							i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];//·µ»Ø³¤¶È
							if(i==0)
							{//·µ»Ø³¤¶È=0
								i=10+(5*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
								i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
							}
							i+=16;
							Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
							if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
							{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
								d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
								if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
								{
									MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
									Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
								}
							}
						}
						else
						{
							//²¹µ±Ç°µ½ÉÏÈÕ0µãÇúÏßµã
							d64b=MRR(ADDR_TYMDHMS+1,5);//È¡µ±Ç°Ê±¼äÄêÔÂÈÕÊ±·Ö
						#if RMM_Carrier_CurveTime==15//ÔØ²¨Í¨µÀ¶Áµç±íÇúÏßµÄÕû¶³½á¼ä¸ôÊ±¼ä·Ö;15·Ö»ò60·Ö
							d64b=YMDHM_Int15(d64b);//ÄêÔÂÈÕÊ±·Ö»¯ÎªÕû15·Ö,Èç00Ê±18·Ö»¯Îª00Ê±15·Ö
							i=Terminal_Note->List[ListNo].RMCount*15;//Ã¿15·Ö¼ä¸ô
						#else
							d64b&=0xffffffff00;
							i=Terminal_Note->List[ListNo].RMCount*60;//Ã¿60·Ö¼ä¸ô
						#endif
							d64b=YMDHM_SubM(d64b,i);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
							i=p8RMLib[0];
							i-=0xe4;
							i<<=16;
							i+=0x06000001;
							ReadMeter_DL645_2007_TxCurveData(ADDR_DATABUFF,i,d64a,d64b);//DL645-2007¶Á¸ººÉÇúÏßÊı¾İÖ¡
							//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
							i=p8RMLib[10+4];//·µ»Ø³¤¶È
							if(i==0)
							{//·µ»Ø³¤¶È=0
								i=10+(5*1);
								i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
							}
							i+=16;
							Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,22,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
							if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
							{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
								d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
								if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
								{
									MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
									Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
								}
							}
						}
					#else//#if EventProject==0//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
						//EventProject==1
						if(UARTCtrl->SubsequentFrames==0)//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
						{//·ÇºóĞøÖ¡
							if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
							{//²»ÊÇ¶Áµç±íÇúÏß
								i=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//È¡DI
//								switch(i)
//								{//ÒÔÏÂÃüÁîÒòÖ¡Ì«³¤²»³­
//									case 0x03300101://F27 ×î½üÒ»´Îµç±íÇåÁã·¢ÉúÊ±¿Ì
//									case 0x03300201://F27 ×î½üÒ»´ÎĞèÁ¿ÇåÁã·¢ÉúÊ±¿Ì
//									case 0x03300501://F166 ×î½üÒ»´ÎµçÄÜ±íÊ±¶Î²ÎÊıĞŞ¸ÄÊ±¼ä
//										Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
//										Terminal_Note->List[ListNo].RMCount++;//³­±íÃüÁî¼ÆÊı
//										Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
//										return;
//								}
							#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
								x=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//µçÄÜ·ÑÂÊ¸öÊı
								if((x==1)&&(p8RMLib[0]==0xE3))
								{//±ä»»ÈÕ¶³½áÊı¾İ±êÊ¶
									switch(Terminal_Note->List[ListNo].RMCount)
									{
										default://ÈÕ¶³½áÊ±¼äYYMMDDhhmm
											Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
											Terminal_Note->List[ListNo].RMCount++;//³­±íÃüÁî¼ÆÊı
											Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
											return;
										case 1://ÉÏ1´ÎÈÕÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
											i=0x00010000;//µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
											break;
									}
								}
							#endif
								Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
								//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
								i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];//·µ»Ø³¤¶È
								if(i==0)
								{//·µ»Ø³¤¶È=0
									i=10+(5*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
									i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
								}
								i+=16;
								Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
								if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
								{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
									d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
									if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
									{
										MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
										Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
									}
								}
							#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
								if(p8RMLib[0]==2)//F2 µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş
								{
									i=Terminal_Note->List[ListNo].RMCount;
									switch(i)
									{
										case 0:
											Terminal_Note->List[ListNo].RMCount++;
											Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı
											return;
										case (2+(96*2))://¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											p8rxbuff=(u8*)(ADDR_DATABUFF);
											MWR(d64a,ADDR_DATABUFF+1,6);
											p8rxbuff[8]=0x14;
											p8rxbuff[9]=24;
											MWR(0x04001503,ADDR_DATABUFF+10,4);
											MC(0,ADDR_DATABUFF+14,8);//ÃÜÂë,²Ù×÷Õß´úÂë
											MR(((u32)p8rxbuff)+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×ÖµÄÎ»±êÖ¾
											for(i=0;i<12;i++)
											{//È¡·´±êÖ¾Î»
												x=p8rxbuff[22+i];
												x=~x;
												p8rxbuff[22+i]=x;
											}
											DL645_68H_Add33H_CS_16H(ADDR_DATABUFF);//DL465Ö¡ÆğÊ¼·û0x68,Êı¾İÓò¼Ó0x33,CS,½áÎ²0x16
											UARTCtrl->TxSEQ--;//Æô¶¯Ö¡ĞòÁĞÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ıÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,36,12,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
											if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
											{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
													Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
												}
											}
											break;
										case 1://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö(½öµ±¶¦ĞÅÄ£¿éÊ±)
											if(Comm_Ram->RouterType!=2)//Â·ÓÉÆ÷ÀàĞÍ:0=±£Áô,1=ÕıÌ©,2=¶¦ĞÅ,3=¶«Èí,4=Ïş³¿,5=ÈğË¹¿µ,6=Á¦ºÏÎ¢,7=
											{//²»ÊÇ¶¦ĞÅÄ£¿é
												Terminal_Note->List[ListNo].RMCount++;
												Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı
												return;
											}
										case (2+(96*2)+1)://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											x=0x04001501;
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
											UARTCtrl->TxSEQ--;//Æô¶¯Ö¡ĞòÁĞÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ıÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
											if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
											{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
													Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
												}
											}
											break;
										default://2-97¶ÁÊÂ¼ş
											//È¡DI	
											x=(u32)p8RMLib;
											x+=10+(5*Terminal_Note->List[ListNo].RMCount);
											x=MRR(x,4);
											if(x==0)
											{//±£Áô
												MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
												Terminal_Note->List[ListNo].RMCount++;
												if(Terminal_Note->List[ListNo].RMCount&1)
												{
													Terminal_Note->List[ListNo].RMCount++;
												}
												Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı
												return;
											}
											i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
											if(i==0)
											{//²»Ğè¶Á
												Terminal_Note->List[ListNo].RMCount++;
												if(Terminal_Note->List[ListNo].RMCount&1)
												{
													Terminal_Note->List[ListNo].RMCount++;
												}
												Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı
												return;
											}
											if(Terminal_Note->List[ListNo].RMCount&1)
											{//¶Á1-10´ÎÊÂ¼ş
												if(i>10)
												{
													i=10;
												}
												i--;
												x+=i;
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
												Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
												UARTCtrl->TxSEQ--;//Æô¶¯Ö¡ĞòÁĞÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ıÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
												Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
												if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
												{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
													d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
													if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
													{
														MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
														Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
													}
												}
											}
											MC(0xee,(u32)p8FnData,4);//µÚ1Ö¡±êÖ¾
											break;
									}
								}
							#endif
								if(p8RMLib[0]==3)//F3 ÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼ş
								{
								#if ((USER/100)==5)//ÉÏº£ÓÃ»§
									switch(Terminal_Note->List[ListNo].RMCount)
									{
									#if EnMeterAutoAutoReport==0//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
										case ((60)*3)://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö
											break;
										case ((60)*3+1)://¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											p8rxbuff=(u8*)(ADDR_DATABUFF);
											MWR(d64a,ADDR_DATABUFF+1,6);
											p8rxbuff[8]=0x14;
											p8rxbuff[9]=24;
											MWR(0x04001503,ADDR_DATABUFF+10,4);
											MC(0,ADDR_DATABUFF+14,8);//ÃÜÂë,²Ù×÷Õß´úÂë
											MR(((u32)p8rxbuff)+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×ÖµÄÎ»±êÖ¾
											
											for(i=0;i<12;i++)
											{
												p8txbuff[22+i]=0xff;
											}
											//È¡·´±êÖ¾Î»bit5¡¢bit7
											x=p8txbuff[22];
											x=(~(x&0xa0));
											p8txbuff[22]=x;
											DL645_68H_Add33H_CS_16H(ADDR_DATABUFF);//DL465Ö¡ÆğÊ¼·û0x68,Êı¾İÓò¼Ó0x33,CS,½áÎ²0x16
											UARTCtrl->TxSEQ--;//Æô¶¯Ö¡ĞòÁĞÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ıÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,36,12,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
											if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
											{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
													Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
												}
											}
											break;
										case ((60)*3+3)://¶Áµç±íÔËĞĞ×´Ì¬×Ö
											break;
									#endif
										default:
											i=Terminal_Note->List[ListNo].RMCount;
											if((i%3)!=0)
											{//¶Á1-10´ÎÊÂ¼ş
												i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/3))+3,1);//ĞÂÔö´ÎÊı(³õÊ¼»¯Öµ0xff)
												if(i==0xff)
												{
													i=0;
												}
												if(i==0)
												{//²»Ğè¶Á
													Terminal_Note->List[ListNo].RMCount++;
													Terminal_Note->List[ListNo].ReRMCount=0;
													return;
												}
												if(i>10)
												{
													i=10;
												}
												i--;
												//È¡DI	
												x=(u32)p8RMLib;
												x+=10+(5*Terminal_Note->List[ListNo].RMCount);
												x=MRR(x,4);
												x+=i;
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
												Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
												Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
												if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
												{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
													d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
													if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
													{
														MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
														Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
													}
												}
												MC(0xee,(u32)p8FnData+((Terminal_Note->List[ListNo].RMCount%3)-1)*2,2);//µÚ1Ö¡±êÖ¾
											}
											break;
									}
								#else//#if ((USER/100)==5)//ÉÏº£ÓÃ»§
								//²»ÊÇÉÏº£µÄÖÜÆÚÊÂ¼ş³­¶Á
									if(UARTCtrl->RMDI==0xffffffff)//³­±íÊı¾İ±êÊ¶(ÓÃÓÚÖÜÆÚÊÂ¼ş³­¶Á),0xffffffff±íÊ¾ÎŞĞ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊı¾İ±êÊ¶²»ÊÇÓÃ¿âÖĞµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼şÀàµÄ³­¶ÁÊı¾İ±êÊ¶
									{//UARTCtrl->RMDIÎŞĞ§
										i=Terminal_Note->List[ListNo].RMCount;
										if((i&1)!=0)
										{//¶Á1-10´ÎÊÂ¼ş
											i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//ĞÂÔö´ÎÊı(³õÊ¼»¯Öµ0xff)
											if(i==0xff)
											{
												i=0;
											}
											if(i==0)
											{//²»Ğè¶Á
												Terminal_Note->List[ListNo].RMCount++;
												Terminal_Note->List[ListNo].ReRMCount=0;
												return;
											}
											if(i>10)
											{
												i=10;
											}
											i--;
											//È¡DI	
											x=(u32)p8RMLib;
											x+=10+(5*Terminal_Note->List[ListNo].RMCount);
											x=MRR(x,4);
											x+=i;
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
											if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
											{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
													Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
												}
											}
											MC(0xee,(u32)p8FnData,4);//µÚ1Ö¡±êÖ¾
										}
									}
									else
									{//UARTCtrl->RMDIÓĞĞ§
										i=Terminal_Note->List[ListNo].RMCount;
										if((i&1)==0)
										{//¶Á´ÎÊı(·ÇÊÂ¼şÀàÊı¾İ)
											x=UARTCtrl->RMDI;//³­±íÊı¾İ±êÊ¶(ÓÃÓÚÖÜÆÚÊÂ¼ş³­¶Á),0xffffffff±íÊ¾ÎŞĞ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊı¾İ±êÊ¶²»ÊÇÓÃ¿âÖĞµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼şÀàµÄ³­¶ÁÊı¾İ±êÊ¶
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
											if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
											{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
													Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
												}
											}
											if(UARTCtrl->RMDICount==0)//ÖÜÆÚÊÂ¼ş³­¶ÁÒÑ³­DI¼ÆÊı
											{
												MC(0xee,(u32)p8FnData,4);//µÚ1Ö¡±êÖ¾
											}
										}
										else
										{//¶Á1-10´ÎÊÂ¼ş
											i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//ĞÂÔö´ÎÊı(³õÊ¼»¯Öµ0xff)
											if(i==0xff)
											{
												i=0;
											}
											if(i==0)
											{//²»Ğè¶Á
												Terminal_Note->List[ListNo].RMCount++;
												Terminal_Note->List[ListNo].ReRMCount=0;
												return;
											}
											if(i>10)
											{
												i=10;
											}
											i--;
											//È¡DI	
											x=UARTCtrl->RMDI;
											x&=0xffffff00;
											x|=0x01;
											x+=i;
											d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
											Read_DL645_2007_TxData(ADDR_DATABUFF,x,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
											Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,0,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
											if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
											{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
												d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
												if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
												{
													MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
													Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
												}
											}
											if(UARTCtrl->RMDICount==0)//ÖÜÆÚÊÂ¼ş³­¶ÁÒÑ³­DI¼ÆÊı
											{
												MC(0xee,(u32)p8FnData,4);//µÚ1Ö¡±êÖ¾
											}
										}
									}
								#endif//#if ((USER/100)==5)//ÉÏº£ÓÃ»§
								}
							}
							else
							{
								//²¹µ±Ç°µ½ÉÏÈÕ0µãÇúÏßµã
								d64b=MRR(ADDR_TYMDHMS+1,5);//È¡µ±Ç°Ê±¼äÄêÔÂÈÕÊ±·Ö
							#if RMM_Carrier_CurveTime==15//ÔØ²¨Í¨µÀ¶Áµç±íÇúÏßµÄÕû¶³½á¼ä¸ôÊ±¼ä·Ö;15·Ö»ò60·Ö
								d64b=YMDHM_Int15(d64b);//ÄêÔÂÈÕÊ±·Ö»¯ÎªÕû15·Ö,Èç00Ê±18·Ö»¯Îª00Ê±15·Ö
								i=Terminal_Note->List[ListNo].RMCount*15;//Ã¿15·Ö¼ä¸ô
							#else
								d64b&=0xffffffff00;
								i=Terminal_Note->List[ListNo].RMCount*60;//Ã¿60·Ö¼ä¸ô
							#endif
								d64b=YMDHM_SubM(d64b,i);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
								i=p8RMLib[0];
								i-=0xe4;
								i<<=16;
								i+=0x06000001;
								ReadMeter_DL645_2007_TxCurveData(ADDR_DATABUFF,i,d64a,d64b);//DL645-2007¶Á¸ººÉÇúÏßÊı¾İÖ¡
								//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
								i=p8RMLib[10+4];//·µ»Ø³¤¶È
								if(i==0)
								{//·µ»Ø³¤¶È=0
									i=10+(5*1);
									i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
								}
								i+=16;
								Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,22,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
								if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
								{//Î¢¹¦ÂÊÎŞÏß,Ä¿±êµØÖ·ÓÃ²É¼¯Æ÷µØÖ·
									d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
									if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
									{
										MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
										Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
									}
								}
							}
						}
						else
						{//ÊÇºóĞøÖ¡
							//p8txbuff[0]=0x68;
							p8txbuff[1]=(29+17+2);//Ğè×Ü·¢ËÍ×Ö½ÚÊı
							p8txbuff[2]=(29+17+2)>>8;
							//p8txbuff[3]=0x41;//¿ØÖÆÂëÎª0x41
							//p8txbuff[4]=4;//Í¨ĞÅÄ£¿é±êÊ¶=1
							//p8txbuff[5]=0;
							p8txbuff[6]=0;//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
							//p8txbuff[7]=0;
							//p8txbuff[8]=0;
							UARTCtrl->TxSEQ++;//Æô¶¯Ö¡ĞòÁĞÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ıÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
							p8txbuff[9]=UARTCtrl->TxSEQ;//Æô¶¯Ö¡ĞòÁĞÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ıÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
							//MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
							//MR(((u32)p8txbuff)+16,ADDR_Data+1,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
							//p8txbuff[22]=0x13;//AFN=13H
							//p8txbuff[23]=1;//Fn=1
							//p8txbuff[24]=0;
							//p8txbuff[25]=2;//×ª·¢Êı¾İ±êÊ¶ÀàĞÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
							//p8txbuff[26]=0;//Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
							//p8txbuff[27]=0;//¸½Êô½ÚµãÊı=0
							p8txbuff[28]=17;//DL645±¨ÎÄ³¤¶È
							p8txbuff[29+8]=0x12;
							p8txbuff[29+9]=5;
							p8txbuff[29+14]=UARTCtrl->SubsequentFrames+0x33;
							p8txbuff[29+15]=0;//cs
							for(i=0;i<15;i++)
							{
								p8txbuff[29+15]+=p8txbuff[29+i];
							}
							p8txbuff[29+16]=0x16;
							Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
						}
					#endif//#if EventProject==0//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
						break;
				}
				UARTCtrl->TxByte=31+p8txbuff[28];//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
				i=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
				UARTCtrl->TxToRxTimerOver=i;
				UARTCtrl->Task=3;//Æô¶¯·¢ËÍ
				
				//ÏÔÊ¾³­±íµØÖ·
				switch(UARTCtrl->RMTaskC)
				{
					case 0://ÉÏµç
						CopyString((u8 *)"ÔØ²¨Æô³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						break;
					case 1://ÈÕ³­
						if(UARTCtrl->DateReRMCount<=1)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
						{
							CopyString((u8 *)"ÔØ²¨ÈÕ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						}
						else
						{
							CopyString((u8 *)"ÔØ²¨²¹³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						}
						break;
					case 2://ÖÜ³­
						CopyString((u8 *)"ÔØ²¨Ê±³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						//CopyString((u8 *)"ÔØ²¨ÖÜ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						break;
					case 3://¶¨³­
						CopyString((u8 *)"ÔØ²¨¶¨³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						break;
					case 4://Ã¿Ğ¡Ê±ÖØµã»§ÇúÏß³­¶Á(½öÓÃÓÚÎŞÇúÏß¶³½áµÄµ¥Ïà±í)
						CopyString((u8 *)"ÔØ²¨ÖØµã",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						break;
					case (MaxRMTask-2)://ÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼ş
						switch(UARTCtrl->RMEventTaskC)//µ±Ç°³­¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ:0=¿Õ,1-3=1-3¼¶
						{
							case 1:
								CopyString((u8 *)"ÔØ²¨ 1¼¶",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
								break;
							case 2:
								CopyString((u8 *)"ÔØ²¨ 2¼¶",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
								break;
							case 3:
								CopyString((u8 *)"ÔØ²¨ 3¼¶",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
								break;
						}
						break;
					default://Ñ­³­
						CopyString((u8 *)"ÔØ²¨Ñ­³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
						break;
				}
			#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
				if(UARTCtrl->RMMeterEvent&1)//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
				{
					CopyString((u8 *)"ÔØ²¨ÉÏ±¨",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
				}
			#endif
				for(i=0;i<6;i++)
				{
					x=p8txbuff[30+5-i];
					MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
					MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
				}
				MWR(0,ADDR_DATABUFF+8+12,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Öû®
				return;
			}
			//break;//²»Òª,ÒòÉÏÊÇwhile(1)
			
		case 1://1=Íê³É1Ö¡·¢ÊÕ
			Terminal_Ram->RouterCheck_S_Timer=(10*60);//ÔØ²¨Â·ÓÉÆ÷ËÀ»úÃë¶¨Ê±Öµ
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//·ñÈÏ»Ø´ğ
				return;
			}
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//È¡´Ó½ÚµãµØÖ·
			x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
			if(x>MaxRS485AddCarrierPn)
			{
				x=MaxRS485AddCarrierPn;
			}
			p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
			for(ListNo=0;ListNo<x;ListNo++)
			{
				d64b=p32[1]&0xffff;
				d64b<<=32;
				d64b|=p32[0];
				if(d64a==d64b)
				{
					break;
				}
				p32+=(LEN_NoteList/4);
			}
			if(ListNo==x)
			{//Ã»ÕÒµ½ÄÚ´æÖĞ´Ó½ÚµãÁĞ±í
				UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
				return;
			}
			p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			switch(Protocol)
			{
				case 1://DL/T645-1997
					i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
					MaxFnCount=Get_RM_DL645_1997_MaxFn();
					break;
			#if ((USER/100)==5)//ÉÏº£ÓÃ»§
				case 21://ÉÏº£¹æÔ¼
					i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
					break;
			#endif
				default://DL/T645-2007
					i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
					break;
			}
			if(i==0x0)
			{//±¾FnÎŞ³­±í
				Terminal_Note->List[ListNo].FnCount=0;//Fn¼ÆÊı=0
				Terminal_Note->List[ListNo].RMCount=0;//³­±íÃüÁî¼ÆÊı=0
				Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
				UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø				
				break;
			}
			p8RMLib=(u8 *)i;//³­±í¿â
			
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				//¼ÌĞø½ÓÊÕ
				p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
				p16timer[0]=1000/10;
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=ÕıÔÚ½ÓÊÕ
				return;
			}
			y=p8rxbuff[28];//·µ»ØÊı¾İÓò³¤¶È
			if(y==0)
			{//½ÓÊÕÊı¾İ³¤¶È=0
				return;
			}
			i=MRR(((u32)p8rxbuff)+25,2);//ÏßÂ·ÉÏ´«ÊäÑÓÊ±Ê±¼äÃë
			UARTCtrl->TransferDelay=i;
			
			Info=MRR(((u32)p8rxbuff)+4,4);
			Fn=p8rxbuff[8];//¶¦ĞÅÄ£¿éÓÃÔİ´æÖ÷¶¯ÉÏ±¨±êÖ¾
			MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,y);
			for(i=0;i<y;i++)
			{//È¥·µ»Ø645Ö¡µÄÇ°µ¼0xfe
				if(p8rxbuff[i]!=0xfe)
				{
					if(i!=0)
					{
						MR((u32)p8rxbuff,((u32)p8rxbuff)+i,y-i);
					}
					break;
				}
			}
			
			if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
			{//ÆğÊ¼·û0x68´í
				return;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//Ö¡·µ»Ø±íµØÖ·
			if(d64a!=d64b)
			{//µØÖ·´í
				return;
			}
			x=0;//ºÍĞ£Ñé¼ÆËã
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if((x!=p8rxbuff[i])||(p8rxbuff[i+1]!=0x16))
			{//ºÍĞ£Ñé´íÎóºÍ½áÊø·û´í
				return;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//Êı¾İÓò¼õ0x33
				p8rxbuff[10+i]-=0x33;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//´ÓÕ¾Òì³£»Ø´ğ,ÎŞ´ËÊı¾İÏî,Õı³£Ìø¹ı
//20150310
			#if (USER/100)==4//ÉÂÎ÷ÓÃ»§
				Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1
				if(Protocol==1)
				{////DL/T645-1997
					if(p8RMLib[0]==0xE3)
					{
						if((p8txbuff[39]==0x52) && (p8txbuff[40]==0xC3))
						{
							p8txbuff[39]=0x43;
							p8txbuff[41]-=0x0f;
							p8txbuff[43]-=0x0f;
							p8txbuff[43]-=0x0f;
							//¼ÌĞø½ÓÊÕ
							UARTCtrl->TxByte=31+p8txbuff[28];//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
							i=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
							UARTCtrl->TxToRxTimerOver=i;
							UARTCtrl->Task=3;//Æô¶¯·¢ËÍ
							return;
						}
					}
				}
			#endif
//xxx
				if(p8RMLib[0]==2)
				{//³­Ö÷¶¯ÉÏ±¨,³­¶ÁÊÂ¼ş·µ»Ø·Ç·¨Ê±,¼ÆÊı¼õ
					i=Terminal_Note->List[ListNo].RMCount;
					switch(i)//³­±íÃüÁî¼ÆÊı
					{
						case 0:
						case (2+(96*2))://¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö
						case 1://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö(½öµ±¶¦ĞÅÄ£¿éÊ±)
						case (2+(96*2)+1)://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö
							break;
						default://¶Á´ÎÊıºÍ¸÷ÊÂ¼ş
							if((i&1)!=0)//¶Á1-10´ÎÊÂ¼ş
							{
								//Ğè¶Á¼ÆÊı-1
								i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
								if(i>10)
								{
									i=10;
								}
								if(i!=0)
								{
									i--;
								}
								MWR(i,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((Terminal_Note->List[ListNo].RMCount-2)/2),1);
							}
					}
				}
				Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
				Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
				Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊı¾İÌîÈë
				return;
			}
			Pn=Terminal_Note->List[ListNo].Pn;
			
	#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
		#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
			if(Comm_Ram->RouterType==2)//Â·ÓÉÆ÷ÀàĞÍ:0=±£Áô,1=ÕıÌ©,2=¶¦ĞÅ,3=¶«Èí,4=Ïş³¿,5=ÈğË¹¿µ,6=Á¦ºÏÎ¢,7=
			{//¶¦ĞÅÄ£¿é
				if(Fn==0x01)
				{//¼Ğ´øÉÏ±¨Ä£Ê½
					x=Terminal_Note->List[ListNo].MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
					if((x!=0)&&(x<=AFN04F10MaxPn))
					{//ĞòºÅÓĞĞ§
						x--;
						MR(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
						MWR(1,ADDR_DATABUFF+1,1);//B1±£Áô,ÏÖÓÃÓÚ±íÊ¾ÓĞÖ÷¶¯ÉÏ±¨
						MWR(1,ADDR_DATABUFF+96,1);//Êı¾İ»ò
						MW(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
					}
				}
			}
		#endif
			if(UARTCtrl->SubsequentFrames!=0)//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
			{//ÊÇºóĞøÖ¡
				if((p8RMLib[0]<2)||(p8RMLib[0]>3))
				{//²»ÊÇ¶ÁÖ÷¶¯ÉÏ±¨ÊÂ¼şºÍÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼ş
					i=MRR(((u32)p8rxbuff)+10,4);//·µ»ØµÄÊı¾İ±êÊ¶
					if(i==0x04001501)
					{//ÊÇÊÕµ½Ö÷¶¯ÉÏ±¨Ö¡
						if((Terminal_Note->List[ListNo].RMCount+1)>=p8RMLib[1])//³­±íÃüÁî¼ÆÊı
						{//Ö»¼Ç1´Î,·ñÔòµ±Ö÷¶¯ÉÏ±¨ÎŞÊÂ¼ş¼ÆÊıÊ±Ã¿FnÖĞÓĞ¶à¸ö³­±íÃüÁîÊ±»á¼Ç¶à´Î.
							WriteAutoReportWord(PORTn,((u32)p8rxbuff));//Ğ´µçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;Èë¿Ú:¶Ë¿ÚºÅ(1-31),Í¨ĞÅµØÖ·,×Ô¶¯ÉÏ±¨×´Ì¬×ÖµØÖ·
						}
						UARTCtrl->SubsequentFrames=0;//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
						Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
						return;
					}
					if(p8rxbuff[8]&0x20)
					{//ÓĞºóĞøÖ¡
						UARTCtrl->SubsequentFrames++;//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
					}
					else
					{//ÎŞºóĞøÖ¡
						UARTCtrl->SubsequentFrames=0;//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
					}
					Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
					return;
				}
			}
			if(p8rxbuff[8]&0x20)
			{//ÓĞºóĞøÖ¡
			/*
			#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
				UARTCtrl->SubsequentFrames++;//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
			#else
				if(p8RMLib[0]==3)
				{//Ö»ÓĞÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼şÊ±²ÅÔÊĞíºóĞøÖ¡
					UARTCtrl->SubsequentFrames++;//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
				}
			#endif
			*/
				if((p8RMLib[0]==2)||(p8RMLib[0]==3))
				{//ÔØ²¨Ö»ÓĞÖ÷¶¯³­ÊÂ¼ş»òÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼şÊ±²ÅÔÊĞíºóĞøÖ¡
					UARTCtrl->SubsequentFrames++;//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
				}
			}
			else
			{//ÎŞºóĞøÖ¡
				UARTCtrl->SubsequentFrames=0;//0=ÎŞ¶ÁºóĞøÖ¡,1-255=DL645¶ÁĞøÖ¡µÄÖ¡ºÅ
			}
	#endif
			switch(Protocol)
			{
				case 1://DL/T645-1997
					x=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//È¡DI
					y=MRR(((u32)p8rxbuff)+10,2);
//20150310
				#if (USER/100)==4//ÉÂÎ÷ÓÃ»§
					if(p8RMLib[0]==0xE3)
					{
						if(y==0x9010)
						{
							y=0x901f;
						}
					}
				#endif
//20150310
					if(x!=y)
					{//DI´íÎó
						//¼ÌĞø½ÓÊÕ
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
						UARTCtrl->RxByte=0;
						UARTCtrl->Task=5;//5=ÕıÔÚ½ÓÊÕ
						return;
					}
					if(p8rxbuff[9]==2)
					{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø2×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
						Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
						Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
						Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊı¾İÌîÈë
						return;//ÉÏ±¨·µ»Ø
					}
					i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];
					if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
					{//·µ»Ø³¤¶ÈÏàÍ¬
						UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
						UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
						ReadMeter_DL645_1997_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_1997³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
						Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
						ReadMeterNote_OK(PORTn,Pn,Info);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
						DateRMFlags(Pn,0);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
//						switch(p8RMLib[0])
//						{
//							case 129://F129 µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ
//							case 162://F162 µçÄÜ±íÈÕÀúÊ±ÖÓ
//							case 0xe3://À©Õ¹3 ¼¯ÖĞ³­±íÈÕ¶³½áÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
//						}
					}
					break;
			#if ((USER/100)==5)//ÉÏº£ÓÃ»§
				case 21://ÉÏº£¹æÔ¼
					x=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//È¡DI
					y=MRR(((u32)p8rxbuff)+8,1);
					if(x!=y)
					{//DI´íÎó
						//¼ÌĞø½ÓÊÕ
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
						UARTCtrl->RxByte=0;
						UARTCtrl->Task=5;//5=ÕıÔÚ½ÓÊÕ
						return;
					}
					i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];
					if(p8rxbuff[9]==i)
					{//·µ»Ø³¤¶ÈÏàÍ¬
						UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
						UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
						ReadMeter_ShH_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_1997³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
						Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
						ReadMeterNote_OK(PORTn,Pn,Info);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
						DateRMFlags(Pn,0);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
//						switch(p8RMLib[0])
//						{
//							case 129://F129 µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ
//							case 162://F162 µçÄÜ±íÈÕÀúÊ±ÖÓ
//							case 0xe3://À©Õ¹3 ¼¯ÖĞ³­±íÈÕ¶³½áÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
//						}
					}
					break;
			#endif
				default://DL/T645-2007
					y=MRR(((u32)p8rxbuff)+10,4);
					i=y;
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//²»ÊÇ¶Áµç±íÇúÏß
						x=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//È¡DI
						//¶ÁµçÄÜ±íµÄÉÏ1-10´ÎÊÂ¼ş,Êı¾İ±êÊ¶ÓëÉÏx´ÎÓĞ¹Ø
						if((p8RMLib[0]==2)&&((Terminal_Note->List[ListNo].RMCount&1)!=0))
						{
							if((Terminal_Note->List[ListNo].RMCount>=2)&&(Terminal_Note->List[ListNo].RMCount<=(2+(2*96))))
							{
								x&=0xffffff00;
								y&=0xffffff00;
							}
						}
						if((p8RMLib[0]==3)&&(UARTCtrl->RMDI!=0xffffffff))
						{//ÖÜÆÚÊÂ¼ş³­¶Á·Ç¿âÖĞÃüÁîÊ±
							x=UARTCtrl->RMDI;//DI
						}
						if((p8RMLib[0]==3)&&((Terminal_Note->List[ListNo].RMCount&1)!=0))
						{
							x&=0xffffff00;
							y&=0xffffff00;
						}
//20150113
					#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//µçÄÜ·ÑÂÊ¸öÊı
						if((i==1)&&(p8RMLib[0]==0xE3))
						{//±ä»»ÈÕ¶³½áÊı¾İ±êÊ¶
							switch(Terminal_Note->List[ListNo].RMCount)
							{
								default://ÈÕ¶³½áÊ±¼äYYMMDDhhmm
									break;
								case 1://ÉÏ1´ÎÈÕÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
									x=0x00010000;//µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
									break;
							}
						}
					#endif
//20150113
					}
					else
					{//ÊÇ¶Áµç±íÇúÏß
						x=MRR((u32)&p8RMLib[10],4);//È¡DI
					}
					if(x!=y)
					{//DI´íÎó
					#if EventProject==0//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
						//¼ÌĞø½ÓÊÕ
						p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
						p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
						UARTCtrl->RxByte=0;
						UARTCtrl->Task=5;//5=ÕıÔÚ½ÓÊÕ
						return;
					#else
						//EventProject==1
						if((p8RMLib[0]==2)&&((Terminal_Note->List[ListNo].RMCount==(2+(96*2)))||(i==0x04001501)))
						{//F2 µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼şµÄ¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö»Ø´ğ»òºóĞøÖ¡
						}
						else
						{
							if((p8RMLib[0]==3)&&(i==0x04001501))
							{//F3 ÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼ş
							}
							else
							{
								//¼ÌĞø½ÓÊÕ
								p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
								p16timer[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
								UARTCtrl->RxByte=0;
								UARTCtrl->Task=5;//5=ÕıÔÚ½ÓÊÕ
								return;
							}
						}
					#endif
					}
					if(p8rxbuff[9]==4)
					{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø4×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
						Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
						Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
						Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊı¾İÌîÈë
						return;//ÉÏ±¨·µ»Ø
					}
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//²»ÊÇ¶Áµç±íÇúÏß
						i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];
					}
					else
					{//ÊÇ¶Áµç±íÇúÏß
						i=p8RMLib[10+4];
					}
				#if EventProject==0//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
					if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
				#else
					if((p8RMLib[0]==3)&&(UARTCtrl->RMDI!=0xffffffff))
					{
						goto SKIP;//ÖÜÆÚÊÂ¼ş³­¶Á·Ç¿âÖĞÃüÁîÊ±ÎŞ·µ»Ø³¤¶È¿É±È½Ï
					}
					if((i==0x0)||(p8rxbuff[8]==0x92)||(p8rxbuff[8]==0xB2)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
				#endif
					{//·µ»Ø³¤¶ÈÏàÍ¬
			SKIP:
						UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
						UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
						ReadMeter_DL645_2007_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_2007³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
						Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
						
						Fn=p8RMLib[0];
						//¶Áµç±í1-6Àà¸ººÉÇúÏß
						if((Fn>=0xe4)&&(Fn<=0xea))
						{
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊı¾İ;ÈôÖ»´æ´¢ÊµÊ±Êı¾İu8 *p8RMLib=0
							Terminal_Note->List[ListNo].RMCountErr=255;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı=×î´óÖµÃâ×î´Î´æ´¢Êı¾İ
							MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
						}
						//75 À©Õ¹13 ÉÏ1-24Õûµãµç±í¶³½áµÄÕı·´ÓĞ¹¦µçÄÜÊ¾Öµ
						if(Fn==0xED)
						{
							d64a=MRR(((u32)p8FnData)+6+1,4);//È¡µç±í·µ»ØµÄ¶³½áÊ±±êÄêÔÂÈÕÊ±
							d64a<<=8;
							AddCurveCongeal(d64a,101,Pn,((u32)p8FnData)+6+1+4,4);//²¹ÇúÏß¶³½áµãÊı¾İ
							AddCurveCongeal(d64a,103,Pn,((u32)p8FnData)+6+1+4+4,4);//²¹ÇúÏß¶³½áµãÊı¾İ
						#if (USER/100)==11//¼ªÁÖÏµ
							//¼ªÁÖÌ¨Ìå²âÊÔÓĞÕû°ëµã¶³½áÊı¾İ
							d64a=YMDHM_SubM(d64a,30);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
							AddCurveCongeal(d64a,101,Pn,((u32)p8FnData)+6+1+4,4);//²¹ÇúÏß¶³½áµãÊı¾İ
						#endif
						}
						#if (USER/100)==11//¼ªÁÖÏµ
						if(Fn==0xFB)
						{
							d64a=MRR(((u32)p8FnData)+6+1,4);//È¡µç±íµÄ¶³½áÊ±±êÄêÔÂÈÕÊ±
							d64a<<=8;
							AddCurveCongeal(d64a,89,Pn,((u32)p8FnData)+6+1+4,2);//²¹ÇúÏß¶³½áµãÊı¾İ
							AddCurveCongeal(d64a+0x15,89,Pn,((u32)p8FnData)+6+1+4+2,2);//²¹ÇúÏß¶³½áµãÊı¾İ
							AddCurveCongeal(d64a+0x30,89,Pn,((u32)p8FnData)+6+1+4+2+2,2);//²¹ÇúÏß¶³½áµãÊı¾İ
							AddCurveCongeal(d64a+0x45,89,Pn,((u32)p8FnData)+6+1+4+2+2+2,2);//²¹ÇúÏß¶³½áµãÊı¾İ
						}
						if(Fn==0xFA)
						{
							if(UARTCtrl->RMCount==2)
							{
								i=MRR(ADDR_TYMDHMS+4,2);//È¡µ±Ç°Ê±¼äÄêÔÂ
								i=YM_Sub1M(i);//ÄêÔÂ¼õ1ÔÂ,·µ»Ø¼õ1ºóµÄÄêÔÂ
								AddMonthCongeal(i,35,Pn,((u32)p8FnData)+6+1,93);//²¹ÔÂ¶³½áµãÊı¾İ
							}
						}
						#endif
						ReadMeterNote_OK(PORTn,Pn,Info);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
						DateRMFlags(Pn,0);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
//						switch(p8RMLib[0])
//						{
//							case 129://F129 µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ
//							case 162://F162 µçÄÜ±íÈÕÀúÊ±ÖÓ
//							case 0xe3://À©Õ¹3 ¼¯ÖĞ³­±íÈÕ¶³½áÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
//						}
					}
					else
					{
					#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
						if(p8RMLib[0]==2)
						{//F2 µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş
							if(p8rxbuff[9]>=4)
							{
								goto SKIP;//²»±È½Ï³¤¶È
							}
						}
					#endif
					}
					break;
			}
			Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
			Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶+1
			break;
		case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
//			if(Terminal_Ram->RouterCheck_S_Timer==0)//ÔØ²¨Â·ÓÉÆ÷ËÀ»úÃë¶¨Ê±Æ÷
//			{
//				Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
//				break;
//			}
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			break;
	}
	
}

#endif

