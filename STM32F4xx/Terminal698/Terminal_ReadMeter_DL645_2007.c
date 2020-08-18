
//ÖÕ¶Ë³­±íRS485¿Ú_DL645_2007
#include "Project.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "Terminal_ReadMeter_DL645_2007_Lib.h"
#include "Terminal_ReadMeter_Event.h"

	#if (USER/100)==8//ÓÃ»§±êÊ¶£ºÍ¨ÓÃ
#include "../Driver/DMW.h"
	#endif

extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];


void Terminal_ReadMeter_DL645_2007(u32 PORTn)//ÖÕ¶Ë³­±íRS485¿Ú_DL645_2007
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2ÀàÊý¾ÝÈÕ¶³½áÃ»³­µ½µç±íÈÕ¶³½áÊý¾ÝÇ°Ìî0xff;0=·ñ(ÓÃÊµÊ±Êý¾Ý¶³½á),1=ÊÇ
	u32 z;
#endif
	u32 Fn;
	u32 Pn;
	u8 * p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
	u32 ClassNo;
	u64 YMDHM;
#if Extern_ReadMeter_Fn_YesNo!=0//Íâ²¿³ÌÐò:0=I2,1=E1,°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý,Protocol(1=1997,ÆäËû=2007)
	u32 *p32;
#endif
#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­
	u8 *p8;
#endif

//	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	
	
	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
	}
	switch(UARTCtrl->Task)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=¿ÕÏÐ
			if(UARTCtrl->BatchTask>=1)//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ý¿Ú¿ÕÏÐÆô¶¯,2...¹ý³Ì
			{
				return;
			}
/*
#if RMM_RS485_PassageZero==1//RS485Í¨µÀ³­±í¹ý0µã;0=²»ÔÝÍ£,1=ÔÝÍ£
			i=MRR(ADDR_TYMDHMS+1,2);
			if((i>=0x2355)||(i<RMM_StartHM))
			{//RS485¿ÚÔÚÃ¿ÈÕ0µã¸½½üÔÝÍ£³­±í
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						PORT485=1;//RS485PORT=1;
						break;
					case RS485_2PORT://RS485-2
						PORT485=2;//RS485PORT=2;
						break;
					case RS485_3PORT://RS485-3
						PORT485=3;//RS485PORT=3;
						break;
					default:
						return;
				}
				MC(0,ADDR_DATABUFF,21);
				CopyString((u8 *)"485 Í£³­µÈ´ýÊ±ÖÓ¹ý0",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
				MWR(PORT485,ADDR_DATABUFF+3,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
				return;
			}
#endif
*/
			if(p16timer[0]==0x0)
			{//ÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±
				UARTCtrl->Task=3;//Æô¶¯·¢ËÍ
			}
			break;
		case 1://1=Íê³É1Ö¡·¢ÊÕ
			UARTCtrl->Task=0;//¿ÕÏÐ1´Î
			break;
		case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
			UARTCtrl->Task=0;//¿ÕÏÐ1´Î
			if(UARTCtrl->ReRMCount<2)
			{
				UARTCtrl->ReRMCount+=1;//³­±íÃüÁîÖØ·¢¼ÆÊý+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				Fn=MRR(DL645_2007_RMLib[UARTCtrl->FnCount],1);
				if(Fn==3)
				{
					UARTCtrl->SearchDICount=0;//ÖÜÆÚÊÂ¼þ³­¶ÁAFN04F106ÉèÖÃËÑË÷¼ÆÊý
					UARTCtrl->RMDICount=0;//ÖÜÆÚÊÂ¼þ³­¶ÁÒÑ³­DI¼ÆÊý
					UARTCtrl->RMDI=0xffffffff;//ÖÜÆÚÊÂ¼þ³­¶ÁµÄÊý¾Ý±êÊ¶,0xffffffff±íÊ¾ÎÞÐ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊý¾Ý±êÊ¶²»ÊÇÓÃ¿âÖÐµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼þÀàµÄ³­¶ÁÊý¾Ý±êÊ¶
				}
				if(Fn!=0xE1)
				{
					UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
				}
				switch(Fn)
				{
					default:
						UARTCtrl->RMError|=1;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
						break;
					case 129://F129 µ±Ç°ÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ
					case 28://µç±íÔËÐÐ×´Ì¬×Ö¼°Æä±äÎ»±êÖ¾
					case 0xE3://À©Õ¹3 ¼¯ÖÐ³­±íÈÕ¶³½áÕý·´ÓÐÎÞ¹¦,4¸öÏóÏÞÎÞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
						UARTCtrl->RMError|=2;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
						break;
					case 0xE1://À©Õ¹1 ERC8µçÄÜ±í²ÎÊý±ä¸üÊÂ¼þÓÃÊý¾Ý
						i=UARTCtrl->RMCount;
						if(i<2)//³­±íÃüÁî¼ÆÊý
						{//µÚ1Ì×µÚ1-2ÈÕÊ±¶Î±í
							UARTCtrl->RMCount=2;
							break;
						}
						if(i<4)//³­±íÃüÁî¼ÆÊý
						{//µÚ2Ì×µÚ1-2ÈÕÊ±¶Î±í
							UARTCtrl->RMCount=4;
						}
						return;
				#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
					case 2://¶ÁµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ
						//ÇåÊÂ¼þ±êÖ¾,ÃâËÀÑ­»·
						MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),96+1);
						return;
				#endif
				}
			}
			break;
		case 3://3=Æô¶¯·¢ËÍ
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//±¾ÐòºÅµÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÉ¾³ý
				goto END_Fn;//²»³­
			}
			if(AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]>=2)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ðè³­
			{
				goto END_Fn;//ÒÑ³É¹¦³­±í
			}
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					PORT485=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					PORT485=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					PORT485=3;//RS485PORT=3;
					break;
				default://ÄÚ²¿½»Á÷²ÉÑù»òÔØ²¨¿Ú
					return;
			}
#if (((Project/100)==2)||((Project/100)==5))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é,700=
			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//ÖÕ¶Ë³­±íÔËÐÐ²ÎÊýÉèÖÃ
			if((i&0x02)!=0x0)
			{//D1ÖÃ"1"ÒªÇóÖÕ¶ËÖ»³­ÖØµã±í£¬ÖÃ"0"ÒªÇóÖÕ¶Ë³­ËùÓÐ±í
				y=MRR(ADDR_AFN04F35,1);//Ì¨Çø¼¯ÖÐ³­±íÖØµã»§ÉèÖÃ
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MRR(ADDR_AFN04F35+1+(2*i),2)==UARTCtrl->MeterNo)
					{
						break;
					}
				}
				if(i==y)
				{//²»ÊÇÖØµã»§
					goto END_Fn;//²»³­
				}
			}
			i=MRR(ADDR_AFN04F30+(Pn-1),1);//Ì¨Çø¼¯ÖÐ³­±íÍ£³­/Í¶³­ÉèÖÃ(²âÁ¿µãºÅ)
			if(i!=0)
			{//Í£³­
				goto END_Fn;//²»³­
			}
#endif
			if(UARTCtrl->FnCount>=Get_RM_DL645_2007_MaxFn())
			{//Fn¼ÆÊý>=×î´óFn¸öÊý,±¾±í³­±íÍê³É
				if(UARTCtrl->RMError==0)
				{//³­±í³É¹¦
				#if EventProject==0//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
				#else
					if(UARTCtrl->RMMeterEvent==0)//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
					{
						AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
					}
				#endif
					ReadMeterNote_OK(PORTn,Pn,0);//¼¯ÖÐ³­±íµç±í³­¶ÁÐÅÏ¢.³É¹¦
					DateRMFlags(Pn,0);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
				}
				else
				{//³­±íÊ§°Ü
				#if EventProject==0//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
				#else
					if(UARTCtrl->RMMeterEvent==0)//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
					{
						AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
					}
				#endif
					ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖÐ³­±íµç±í³­¶ÁÐÅÏ¢.Ê§°Ü
					DateRMFlags(Pn,1);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
				}
	END_Fn:
				UARTCtrl->Task=0;//0=¿ÕÏÐ
				UARTCtrl->FnCount=0;//Fn¼ÆÊý=0
				UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊý½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
				UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊý=0
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				UARTCtrl->SearchDICount=0;//ÖÜÆÚÊÂ¼þ³­¶ÁAFN04F106ÉèÖÃËÑË÷¼ÆÊý
				UARTCtrl->RMDICount=0;//ÖÜÆÚÊÂ¼þ³­¶ÁÒÑ³­DI¼ÆÊý
				UARTCtrl->RMDI=0xffffffff;//ÖÜÆÚÊÂ¼þ³­¶ÁµÄÊý¾Ý±êÊ¶,0xffffffff±íÊ¾ÎÞÐ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊý¾Ý±êÊ¶²»ÊÇÓÃ¿âÖÐµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼þÀàµÄ³­¶ÁÊý¾Ý±êÊ¶
				UARTCtrl->RMCountErr=0;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
				UARTCtrl->RMError=0;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				UARTCtrl->RMMeterEvent=0;//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
				break;
			}
			if(DL645_2007_RMLib[UARTCtrl->FnCount]==0x0)
			{//±¾FnÎÞ³­±í
	NEXT_Fn:
				UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
				UARTCtrl->FnCount+=1;//Fn¼ÆÊý+1
				UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊý=0
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				UARTCtrl->RMCountErr=0;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
				UARTCtrl->RMError&=0xfe;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				
	#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
	#if EnMeterAutoAutoReport==1//ÔÊÐíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ:0=²»ÔÊÐí,1=ÔÊÐí
				if((UARTCtrl->RMMeterEvent&1)==0)//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
				{//0=·ñ
					UARTCtrl->SearchDICount=0;//ÖÜÆÚÊÂ¼þ³­¶ÁAFN04F106ÉèÖÃËÑË÷¼ÆÊý
					UARTCtrl->RMDICount=0;//ÖÜÆÚÊÂ¼þ³­¶ÁÒÑ³­DI¼ÆÊý
					UARTCtrl->RMDI=0xffffffff;//ÖÜÆÚÊÂ¼þ³­¶ÁµÄÊý¾Ý±êÊ¶,0xffffffff±íÊ¾ÎÞÐ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊý¾Ý±êÊ¶²»ÊÇÓÃ¿âÖÐµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼þÀàµÄ³­¶ÁÊý¾Ý±êÊ¶
					if(UARTCtrl->RMTaskC!=0)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
					{//²»ÊÇÆô³­
						i=Check_Meter_Event_Save();//¼ì²éµçÄÜ±íÊÂ¼þ´æÖü¿Õ¼ä;·µ»Ø0=´æ´¢M(Ã»ÉÏ±¨),1-255ÔÊÐí»¹¿É´æ´¢µÄÌõÊý
						if(i>=30)
						{//¿Õ¼ä¿É´æ>=30Ìõ,Ã¿¿Ú10Ìõ
							i=Check_AutoReportWord(PORTn);//¼ì²éµçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;·µ»Ø:0=ÎÞ,!=0 B31-B16ÎªÅäÖÃÐòºÅ(1¿ªÊ¼),B15-B0ÎªÒª¶ÁÈ¡µÄÖ÷¶¯ÉÏ±¨×´Ì¬×ÖÎ»ºÅ(0-95)
							i>>=16;
							if(i!=0)
							{//ÓÐÖ÷¶¯ÉÏ±¨ÐèÒª¶Á
								UARTCtrl->RMMeterEvent|=1;//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
								UARTCtrl->MeterNoBackupOfAutoReport=UARTCtrl->MeterNo;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃÐòºÅ¼Ä´æ)
								UARTCtrl->FnCountBackupOfAutoReport=UARTCtrl->FnCount;//485³­±íFn¼ÆÊý(Ô­³£¹æ³­±íµÄFn¼ÆÊý¼Ä´æ)
								UARTCtrl->MeterNo=i;
								UARTCtrl->FnCount=2;
							}
						}
						if((UARTCtrl->RMMeterEvent&1)==0)//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
						{
							p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
							if(p8RMLib[0]==2)//F2
							{
								UARTCtrl->FnCount++;//Fn¼ÆÊý+1;·ÇÖ÷¶¯ÊÂ¼þÌø¹ý²»³­
							}
						}
					}
				}
				else
				{//ÊÇ
					UARTCtrl->RMMeterEvent&=0xfe;//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
					UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃÐòºÅ¼Ä´æ)
					UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfAutoReport;//485³­±íFn¼ÆÊý(Ô­³£¹æ³­±íµÄFn¼ÆÊý¼Ä´æ)
				}
	#endif
	#endif
				break;
			}
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//±¾Fn²»ÊÇRS485³­±í
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];

			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//È¡´óÐ¡ÀàºÅ
		#if Extern_ReadMeter_Fn_YesNo==0//Íâ²¿³ÌÐò:0=I2,1=E1,°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý,Protocol(1=1997,ÆäËû=2007)	
			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,30);//°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý
		#else
			y=0;
			if((Comm_Ram->SoftModule&(1<<4))==0)//Èí¼þÄ£¿é´íÎó±êÖ¾(ÖÃÎ»±íÊ¾´íÎó):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				p32=(u32 *)(ADDR_E1_ENTRY);
				y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)UARTCtrl->RMCount,(u32)30);//°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý
			}
		#endif
	#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
	#if EnMeterAutoAutoReport==1//ÔÊÐíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ:0=²»ÔÊÐí,1=ÔÊÐí
			if(UARTCtrl->RMMeterEvent!=0)//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
			{
				y=1;//³­¶ÁÖ÷¶¯ÉÏ±¨×ÜÊÇÔÊÐí
			}
	#endif
	#endif
			if(Fn==26)
			{
			#if RMM_AFN0CF26==0//AFN0CF26¶ÏÏàÊý¾Ý³­±í;0=³­µç±í¶ÏÏà¼ÇÂ¼Êý¾Ý,1=³­µç±íÊ§Ñ¹¼ÇÂ¼Êý¾Ý
				if(UARTCtrl->RMCount==12)//³­±íÊý¾Ý±êÊ¶¼ÆÊý
			#endif
			#if RMM_AFN0CF26==1//AFN0CF26¶ÏÏàÊý¾Ý³­±í;0=³­µç±í¶ÏÏà¼ÇÂ¼Êý¾Ý,1=³­µç±íÊ§Ñ¹¼ÇÂ¼Êý¾Ý
				if(UARTCtrl->RMCount==16)//³­±íÊý¾Ý±êÊ¶¼ÆÊý
			#endif
				{//³­±íÃüÁî¼ÆÊý16ÒÔÉÏÊÇ¼æÈÝÃüÁî,Ö»ÓÐ²»³É¹¦Ê±²Å³­
					for(i=0;i<(1+61);i++)
					{
						if(p8FnData[i]!=0xee)
						{
							break;
						}
					}
					if(i!=(1+61))
					{//ÓÐ³­³É¹¦
						y=0;//²»³­
						UARTCtrl->RMCount=100;//³­±íÊý¾Ý±êÊ¶¼ÆÊý
					}
				}
			}
		#if RMM_DateCongeal_0xEE==0//2ÀàÊý¾ÝÈÕ¶³½áÃ»³­µ½µç±íÈÕ¶³½áÊý¾ÝÇ°Ìî0xff;0=·ñ(ÓÃÊµÊ±Êý¾Ý¶³½á),1=ÊÇ
			if(y!=0)
			{
			//À©Õ¹14 ²¹³­ÉÏ2ÈÕÈÕ¶³½áÊý¾Ý
			//À©Õ¹15 ²¹³­ÉÏ3ÈÕÈÕ¶³½áÊý¾Ý
				if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
				{
					y=0;//Ã»Íê³É1¸öÑ­»·³­±íÊ±²»³­
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
						z=UARTCtrl->RMCount;//³­±íÃüÁî¼ÆÊý
						if((z>0)&&(z<9))
						{
							z+=160;//³­±íÃüÁî¼ÆÊý1-8¶ÔÓ¦Fn=161-168
							z=GetClass2DataOfficeAddr(z,Pn);//È¡Àà2Êý¾Ý´æÖüÆ«ÒÆµØÖ·,·µ»Ø0±íÊ¾Ã»ÕÒµ½
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
						i=UARTCtrl->RMCount;//³­±íÊý¾Ý±êÊ¶¼ÆÊý
						y=1;//³­±íÊý¾Ý±êÊ¶¼ÆÊý=0ºÍ9×ÜÊÇÒª³­
						if(i>9)
						{
							break;
						}
						if((i>0)&&(i<9))
						{
							i+=160;//Òª³­µÄFn=161-168
							y=Class2Data_Fn(ClassNo,i);//2ÀàÊý¾ÝFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö		
						}
						if(y==0)
						{
							UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîÈë
							UARTCtrl->RMCount++;//³­±íÊý¾Ý±êÊ¶¼ÆÊý
						}
						else
						{
							break;
						}
					}
					if(UARTCtrl->RMCount>=p8RMLib[1])
					{//³­±íÊý¾Ý±êÊ¶¼ÆÊý>=FnÐè·¢ÃüÁîÊý;±¾Fn³­±íÍê³É
						if(p8RMLib[1]>UARTCtrl->RMCountErr)
						{//²»ÊÇÈ«²¿±êÊ¶È«²¿³­¶ÁÊ§°Ü
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý
						}
						goto NEXT_Fn;
					}
				}
			}
		#endif
			if(y!=0)
			{
				if((p8RMLib[0]==0xE3)||(p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
				{//³­ÉÏ1-3ÈÕÈÕ¶³½áÊý¾Ý
				#if ERC39==1
					if(p8RMLib[0]==0xEF)
					{
						if(UARTCtrl->RMCount<=1)
						{
							ERC39_Event(Pn);//²¹³­Ê§°ÜÊÂ¼þ¼ÇÂ¼
						}
					}
				#endif
				#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
					if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
					{
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//µçÄÜ·ÑÂÊ¸öÊý
						if(i==1)
						{//³­ÉÏ2-3ÈÕÈÕ¶³½áÊý¾Ý
							y=0;//ÉÂÎ÷Å©Íø±íÎÞÈÕ¶³½á
						}
					}
				#endif
					if(UARTCtrl->RMCount==9)
					{//ÔÙ´ÎÈ·ÈÏÈÕ¶³½áÊ±¼äYYMMDDhhmm
						//Ê¡ÂÔ²»³­,Òò³­±íÔÚ¹ý0µãºóÑÓÊ±½øÐÐ,ÔÚ´ËÅÐ¶Ï²»ÐÞ¸Ä³­±í¿â¼ò»¯Éè¼Æ
						y=0;
					}
				}
			}
			if(y==0x0)
			{//±¾ÃüÁî²»³­
				if(p8RMLib[1]==0x0)
				{//±¾Fn³­±íÃüÁîÊý=0
					goto NEXT_Fn;
				}
				if(UARTCtrl->RMCount==0)//³­±íÊý¾Ý±êÊ¶¼ÆÊý
				{//³­±íÊý¾Ý±êÊ¶¼ÆÊý=0,ÇåFnÊý¾Ý»º³å
					UARTCtrl->RMCountErr=0;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîÈë
				UARTCtrl->RMCount++;//³­±íÊý¾Ý±êÊ¶¼ÆÊý
				UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
				if(UARTCtrl->RMCount>=p8RMLib[1])
				{//³­±íÃüÁî¼ÆÊý>=FnÐè·¢ÃüÁîÊý;±¾Fn³­±íÍê³É
				#if(RMM_RS485_FailData_0xEE==0)//RS485³­±íÊ§°ÜÊý¾ÝÌî0xee;0=·ñ,1=ÊÇ
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//²»ÊÇÈ«²¿±êÊ¶²»³­
						goto OK_Fn;
					}
					goto NEXT_Fn;
				#else
					goto OK_Fn;
				#endif
				}
				return;
			}

			if(p8RMLib[1]==0x0)
			{//±¾Fn³­±íÃüÁîÊý=0
			#if AFN0CF33MaxPn==0
				goto NEXT_Fn;
			#else
				if((p8RMLib[4]&0x01)!=0)//Êý¾Ý·ÖÀà:B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ð¡Ê±¶³½á,B0=ÊµÊ±
				{//1ÀàÊý¾Ý,ÊµÊ±
					switch(Fn)
					{
					//F33 µ±Ç°ÕýÏòÓÐ/ÎÞ¹¦£¨×éºÏÎÞ¹¦1£©µçÄÜÊ¾Öµ¡¢Ò»/ËÄÏóÏÞÎÞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
					//F34 µ±Ç°·´ÏòÓÐ/ÎÞ¹¦£¨×éºÏÎÞ¹¦2£©µçÄÜÊ¾Öµ¡¢¶þ/ÈýÏóÏÞÎÞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
					//F35 µ±ÔÂÕýÏòÓÐ/ÎÞ¹¦×î´óÐèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
					//F36 µ±ÔÂ·´ÏòÓÐ/ÎÞ¹¦×î´óÐèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
						case 33:
						case 34:
						case 35:
						case 36:
					//F37 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©ÕýÏòÓÐ/ÎÞ¹¦£¨×éºÏÎÞ¹¦1£©µçÄÜÊ¾Öµ¡¢Ò»/ËÄÏóÏÞÎÞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
					//F38 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©·´ÏòÓÐ/ÎÞ¹¦£¨×éºÏÎÞ¹¦2£©µçÄÜÊ¾Öµ¡¢¶þ/ÈýÏóÏÞÎÞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
					//F39 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©ÕýÏòÓÐ/ÎÞ¹¦×î´óÐèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜM¡Ü12£©
					//F40 ÉÏÔÂ£¨ÉÏÒ»½áËãÈÕ£©·´ÏòÓÐ/ÎÞ¹¦×î´óÐèÁ¿¼°·¢ÉúÊ±¼ä£¨×Ü¡¢·ÑÂÊ1¡«M£¬1¡ÜMÜ12©
						case 37:
						case 38:
						case 39:
						case 40:
							break;
						default:
							goto NEXT_Fn;
					}
					ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý
					goto NEXT_Fn;
				}
			#endif
			}

			if(UARTCtrl->RMCount==0x0)
			{//³­±íÃüÁî¼ÆÊý=0,ÇåFnÊý¾Ý»º³å
				UARTCtrl->RMCountErr=0;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//³­±íÃüÁî¼ÆÊý>=FnÐè·¢ÃüÁîÊý;±¾Fn³­±íÍê³É
			OK_Fn:
				if(Fn==129)//F129 µ±Ç°ÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ
				{
					if((UARTCtrl->RMError&0x2)==0)//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
					{
				#if(RMM_RS485_FailData_0xEE==1)//RS485³­±íÊ§°ÜÊý¾ÝÌî0xee;0=·ñ,1=ÊÇ
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý;ÈôÖ»´æ´¢ÊµÊ±Êý¾Ýu8 *p8RMLib=0
				#else
						if(p8RMLib[1]>UARTCtrl->RMCountErr)
						{//²»ÊÇÈ«²¿±êÊ¶¶¼Ã»³­
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý;ÈôÖ»´æ´¢ÊµÊ±Êý¾Ýu8 *p8RMLib=0
						}
				#endif
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,0);//µçÄÜ±í³­±í³É¹¦Ê§°Ü¼ÆÊýºÍ±êÖ¾,Èë¿ÚUART»º³åµØÖ·,µçÄÜ±íÐòºÅ,±êÖ¾f=0³É¹¦,f=1Ê§°Ü
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,0);//ÖÕ¶Ë485³­±íÊ§°ÜÊÂ¼þ,Èë¿Ú²âÁ¿µãºÅ,f=1²úÉú=0»Ö¸´
						#endif
						MC(0,ADDR_ERC21_Flag,1);//ÇåERC21 485³­±í¹ÊÕÏÊÂ¼þ¼ÇÂ¼±êÖ¾
					}
					else
					{
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,1);//µçÄÜ±í³­±í³É¹¦Ê§°Ü¼ÆÊýºÍ±êÖ¾,Èë¿ÚUART»º³åµØÖ·,µçÄÜ±íÐòºÅ,±êÖ¾f=0³É¹¦,f=1Ê§°Ü
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,1);//ÖÕ¶Ë485³­±íÊ§°ÜÊÂ¼þ,Èë¿Ú²âÁ¿µãºÅ,f=1²úÉú=0»Ö¸´
						#endif
						#if ERC21==1
						ERC21_Event();//ERC21 485³­±í¹ÊÕÏÊÂ¼þ
						#endif
				#if(RMM_RS485_FailData_0xEE==1)//RS485³­±íÊ§°ÜÊý¾ÝÌî0xee;0=·ñ,1=ÊÇ
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý
				#else
						ClrERC29SOUR(Pn);//¼¯ÖÐ³­±íÊ§°ÜÇåERC29µçÄÜ±í·É×ßÊÂ¼þÔ­Ê¼Êý¾Ý
				#endif
					}
					goto NEXT_Fn;
				}
		#if(RMM_RS485_FailData_0xEE==1)//RS485³­±íÊ§°ÜÊý¾ÝÌî0xee;0=·ñ,1=ÊÇ
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý;ÈôÖ»´æ´¢ÊµÊ±Êý¾Ýu8 *p8RMLib=0
		#else
				if(UARTCtrl->RMError==0)//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				{
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//²»ÊÇÈ«²¿±êÊ¶¶¼Ã»³­
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý;ÈôÖ»´æ´¢ÊµÊ±Êý¾Ýu8 *p8RMLib=0
					}
				}
		#endif
				goto NEXT_Fn;
			}
			if(UARTCtrl->RMError!=0x0)
			{//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
		#if(RMM_RS485_FailData_0xEE==1)//RS485³­±íÊ§°ÜÊý¾ÝÌî0xee;0=·ñ,1=ÊÇ
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý;ÈôÖ»´æ´¢ÊµÊ±Êý¾Ýu8 *p8RMLib=0
		#endif
				//²»ÊÇ×ªµ½NEXT_FnÃâÏÂ¸öFn²»³­±í·´¶øÖÃ³­±íÎª³É¹¦±êÖ¾
			#if EventProject==0//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
				AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
			#else
				if(UARTCtrl->RMMeterEvent==0)//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
				{
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
				}
			#endif
				ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖÐ³­±íµç±í³­¶ÁÐÅÏ¢.Ê§°Ü
				DateRMFlags(Pn,1);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
				goto END_Fn;//²»ÊÇ×ªµ½NEXT_FnÃâÏÂ¸öFn²»³­±í·´¶øÖÃ³­±íÎª³É¹¦±êÖ¾
			}

			//À©Õ¹13 ÉÏ1-24Õûµãµç±í¶³½áµÄÕý·´ÓÐ¹¦µçÄÜÊ¾Öµ
			if(p8RMLib[0]==0xED)
			{
				//Fn101ÕýÓÐ¹¦µçÄÜÊ¾ÖµÇúÏßÅäÖÃ
				y=Class2Data_Fn(ClassNo,101);//2ÀàÊý¾ÝFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
				//Fn103·´ÓÐ¹¦µçÄÜÊ¾ÖµÇúÏßÅäÖÃ
				y|=Class2Data_Fn(ClassNo,103);//2ÀàÊý¾ÝFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
				if(y==0)
				{
					goto NEXT_Fn;
				}
				if(UARTCtrl->RMCount==0)
				{//½öµ±ÃüÁî¼ÆÊý=0Ê±ÁÐ±í,¼õÉÙÔËÐÐÊ±¼ä
					KeyUserPnList();//¼¯ÖÐ³­±íÖØµã»§ÇúÏß¶³½áPnÁÐ±í
				}
				i=MRR(ADDR_TYMDHMS+2,4);//È¡µ±Ç°ÄêÔÂÈÕÊ±
				YMDHM=YMDH_SubH(i,UARTCtrl->RMCount);//ÄêÔÂÈÕÊ±¼õhÊ±,·µ»Ø¼õhÊ±ºóµÄÄêÔÂÈÕÊ±
				YMDHM<<=8;//·Ö=0
				i=CheckCurveCongeal(YMDHM,101,Pn);//¼ì²éÇúÏß¶³½á;·µ»Ø:0=Êý¾ÝÃ»¶³½á,1=Êý¾ÝÒÑ¶³½á,2=Ã»ÕÒµ½ÓÐÐ§¶³½á
				if(i!=0)
				{//ÒÑ¶³½á»òÃ»ÕÒµ½¶³½áºÅ
					UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
					UARTCtrl->RMCount++;//³­±íÊý¾Ý±êÊ¶¼ÆÊý+1
					UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
					return;
				}
			}
	#if EventProject==0//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
		//EventProject==0
			Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//ÖÕ¶ËDL645-2007³­±í·¢ËÍÊý¾Ý
		#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//µçÄÜ·ÑÂÊ¸öÊý
			if((i==1)&&(p8RMLib[0]==0xE3))
			{//±ä»»ÈÕ¶³½áÊý¾Ý±êÊ¶
				switch(UARTCtrl->RMCount)
				{
					default://ÈÕ¶³½áÊ±¼äYYMMDDhhmm
						UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
						UARTCtrl->RMCount++;//³­±íÊý¾Ý±êÊ¶¼ÆÊý+1
						UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
						return;
					case 1://ÉÏ1´ÎÈÕÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
						i=0x00010000;//µ±Ç°ÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
						break;
				}
				MWR(i,((u32)p8txbuff)+4+10,4);
				//DI¼Ó0x33
				for(i=0;i<4;i++)
				{
					p8txbuff[4+10+i]+=0x33;
				}
				//cs
				p8txbuff[4+14]=0;
				for(i=0;i<14;i++)
				{
					p8txbuff[4+14]+=p8txbuff[4+i];
				}
			}
		#endif
			Fn=p8RMLib[0];
			if((Fn>=0xe4)&&(Fn<=0xe9))
			{
			#if (((Project/100)==2)&&((USER/100)==5))//ÉÏº£Ïµ¼¯ÖÐÆ÷
				//²¹µ±Ç°µ½µ±ÈÕ0µãÇúÏßµã
				YMDHM=MRR(ADDR_TYMDHMS+1,5);//È¡µ±Ç°Ê±¼äÄêÔÂÈÕÊ±·Ö
				YMDHM&=0xffffffff00;
				i=UARTCtrl->RMCount*60;//Ã¿60·Ö¼ä¸ô
				YMDHM=YMDHM_SubM(YMDHM,i);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
				i=Fn;
				i-=0xe4;
				i<<=16;
				i+=0x06000001;
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//ÖÕ¶ËDL645-2007³­±í·¢ËÍ¸ººÉÇúÏßÊý¾Ý
			#else
				//²¹µ±Ç°µ½ÉÏÈÕ0µãÇúÏßµã
				YMDHM=MRR(ADDR_TYMDHMS+1,5);//È¡µ±Ç°Ê±¼äÄêÔÂÈÕÊ±·Ö
				YMDHM=YMDHM_Int15(YMDHM);//ÄêÔÂÈÕÊ±·Ö»¯ÎªÕû15·Ö,Èç00Ê±18·Ö»¯Îª00Ê±15·Ö
				i=UARTCtrl->RMCount*15;//Ã¿15·Ö¼ä¸ô
				YMDHM=YMDHM_SubM(YMDHM,i);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
				i=Fn;
				i-=0xe4;
				i<<=16;
				i+=0x06000001;
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//ÖÕ¶ËDL645-2007³­±í·¢ËÍ¸ººÉÇúÏßÊý¾Ý
			#endif
			}
	#else//#if EventProject==0//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
		//EventProject==1
			if(UARTCtrl->SubsequentFrames==0)//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
			{//·ÇºóÐøÖ¡
				Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//ÖÕ¶ËDL645-2007³­±í·¢ËÍÊý¾Ý
			#if EnMeterAutoAutoReport==1//ÔÊÐíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ:0=²»ÔÊÐí,1=ÔÊÐí
				if(p8RMLib[0]==2)//F2 µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ
				{
					switch(UARTCtrl->RMCount)
					{
						case 0:
						case 1://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö(½öµ±¶¦ÐÅÄ£¿éÊ±)
							UARTCtrl->RMCount++;
							return;
						case (2+(96*2))://¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							MWR(YMDHM,((u32)p8txbuff)+4+1,6);
							p8txbuff[4+8]=0x14;
							p8txbuff[4+9]=24;
							MWR(0x04001503,((u32)p8txbuff)+4+10,4);
							MC(0,((u32)p8txbuff)+4+14,8);//ÃÜÂë,²Ù×÷Õß´úÂë
							MR(((u32)p8txbuff)+4+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×ÖµÄÎ»±êÖ¾
							for(i=0;i<12;i++)
							{//È¡·´±êÖ¾Î»
								x=p8txbuff[4+22+i];
								x=~x;
								p8txbuff[4+22+i]=x;
							}
							DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465Ö¡ÆðÊ¼·û0x68,Êý¾ÝÓò¼Ó0x33,CS,½áÎ²0x16
							UARTCtrl->TxByte=4+36;//·¢ËÍ×Ö½ÚÊý
							break;
						case (2+(96*2)+1)://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö
							break;
						default://2-97¶Á´ÎÊýºÍÊÂ¼þ
							//È¡DI	
							x=(u32)p8RMLib;
							x+=10+(5*UARTCtrl->RMCount);
							x=MRR(x,4);
							if(x==0)
							{//±£Áô
								MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-2)/2),1);
								UARTCtrl->RMCount++;
								if(UARTCtrl->RMCount&1)
								{
									UARTCtrl->RMCount++;
								}
								UARTCtrl->ReRMCount=0;
								return;
							}
							i=MRR(ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+((UARTCtrl->RMCount-2)/2),1);
							if(i==0)
							{//²»Ðè¶Á
								UARTCtrl->RMCount++;
								if(UARTCtrl->RMCount&1)
								{
									UARTCtrl->RMCount++;
								}
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(UARTCtrl->RMCount&1)
							{//¶Á1-10´ÎÊÂ¼þ
								if(i>10)
								{
									i=10;
								}
								i--;
								x+=i;
								YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
								i=Get_ADDR_UARTnTx(PORTn);
								Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
							}
							MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//µÚ1Ö¡±êÖ¾
							break;
					}
				}
			#endif//#if EnMeterAutoAutoReport==1//ÔÊÐíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ:0=²»ÔÊÐí,1=ÔÊÐí
				if(p8RMLib[0]==3)//F3 ÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼þ
				{
				#if ((USER/100)==5)//ÉÏº£ÓÃ»§
					switch(UARTCtrl->RMCount)
					{
					#if EnMeterAutoAutoReport==0//ÔÊÐíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ:0=²»ÔÊÐí,1=ÔÊÐí
						case ((60)*3)://¶ÁÖ÷¶¯ÉÏ±¨×´Ì¬×Ö
							break;
						case ((60)*3+1)://¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							MWR(YMDHM,((u32)p8txbuff)+4+1,6);
							p8txbuff[4+8]=0x14;
							p8txbuff[4+9]=24;
							MWR(0x04001503,((u32)p8txbuff)+4+10,4);
							MC(0,((u32)p8txbuff)+4+14,8);//ÃÜÂë,²Ù×÷Õß´úÂë
							MR(((u32)p8txbuff)+4+22,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord)+97,12);//¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×ÖµÄÎ»±êÖ¾

							for(i=0;i<12;i++)
							{
								p8txbuff[4+22+i]=0xff;
							}
							//È¡·´±êÖ¾Î»bit5¡¢bit7
							x=p8txbuff[4+22];
							x=(~(x&0xa0));
							p8txbuff[4+22]=x;
							DL645_68H_Add33H_CS_16H(((u32)p8txbuff)+4);//DL465Ö¡ÆðÊ¼·û0x68,Êý¾ÝÓò¼Ó0x33,CS,½áÎ²0x16
							UARTCtrl->TxByte=4+36;//·¢ËÍ×Ö½ÚÊý
							break;
						case ((60)*3+3)://¶Áµç±íÔËÐÐ×´Ì¬×Ö
							break;
					#endif
						default:
							i=UARTCtrl->RMCount;
							if((i%3)!=0)
							{//¶Á1-10´ÎÊÂ¼þ
								i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/3))+3,1);//ÐÂÔö´ÎÊý(³õÊ¼»¯Öµ0xff)
								if(i==0xff)
								{
									i=0;
								}
								if(i==0)
								{//²»Ðè¶Á
									UARTCtrl->RMCount++;
									UARTCtrl->ReRMCount=0;
									return;
								}
								if(i>10)
								{
									i=10;
								}
								i--;
								//È¡DI	
								x=(u32)p8RMLib;
								x+=10+(5*UARTCtrl->RMCount);
								x=MRR(x,4);
								x+=i;
								YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
								i=Get_ADDR_UARTnTx(PORTn);
								Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn)+((UARTCtrl->RMCount%3)-1)*2,2);//µÚ1Ö¡±êÖ¾
							}
							break;
					}
				#else//#if ((USER/100)==5)//ÉÏº£ÓÃ»§
				//!!!2015-7-29ÖÜÆÚÊÂ¼þ³­¶ÁÐÞ¸Ä
					if(UARTCtrl->RMDI==0xffffffff)//³­±íÊý¾Ý±êÊ¶(ÓÃÓÚÖÜÆÚÊÂ¼þ³­¶Á),0xffffffff±íÊ¾ÎÞÐ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊý¾Ý±êÊ¶²»ÊÇÓÃ¿âÖÐµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼þÀàµÄ³­¶ÁÊý¾Ý±êÊ¶
					{//UARTCtrl->RMDIÎÞÐ§
						i=UARTCtrl->RMCount;
						if((i&1)!=0)
						{//¶Á1-10´ÎÊÂ¼þ
							i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//ÐÂÔö´ÎÊý(³õÊ¼»¯Öµ0xff)
							if(i==0xff)
							{
								i=0;
							}
							if(i==0)
							{//²»Ðè¶Á
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;
								return;
							}
							if(i>10)
							{
								i=10;
							}
							i--;
							//È¡DI	
							x=(u32)p8RMLib;
							x+=10+(5*UARTCtrl->RMCount);
							x=MRR(x,4);
							x+=i;
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
							MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//µÚ1Ö¡±êÖ¾
						}
					}
					else
					{//UARTCtrl->RMDIÓÐÐ§
						i=UARTCtrl->RMCount;
						if((i&1)==0)
						{//¶Á´ÎÊý(·ÇÊÂ¼þÀàÊý¾Ý)
							x=UARTCtrl->RMDI;//³­±íÊý¾Ý±êÊ¶(ÓÃÓÚÖÜÆÚÊÂ¼þ³­¶Á),0xffffffff±íÊ¾ÎÞÐ§,ÆäËûÖµ±íÊ¾Òª³­µÄÊý¾Ý±êÊ¶²»ÊÇÓÃ¿âÖÐµÄ¿é³­±êÊ¶ºÍ·ÇÊÂ¼þÀàµÄ³­¶ÁÊý¾Ý±êÊ¶
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
							if(UARTCtrl->RMDICount==0)//ÖÜÆÚÊÂ¼þ³­¶ÁÒÑ³­DI¼ÆÊý
							{
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//µÚ1Ö¡±êÖ¾
							}
						}
						else
						{//¶Á1-10´ÎÊÂ¼þ,Ö»Ö¸¶¨µÄÊý¾Ý±êÊ¶
							i=MRR(ADDR_MeterEventCount+((Pn-1)*LEN_MeterEventCount)+(4*(i/2))+3,1);//ÐÂÔö´ÎÊý(³õÊ¼»¯Öµ0xff)
							if(i==0xff)
							{
								i=0;
							}
							if(i==0)
							{//²»Ðè¶Á
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;
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
							YMDHM=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
							i=Get_ADDR_UARTnTx(PORTn);
							Read_DL645_2007_TxData(i+4,x,YMDHM);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
							if(UARTCtrl->RMDICount==0)//ÖÜÆÚÊÂ¼þ³­¶ÁÒÑ³­DI¼ÆÊý
							{
								MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),4);//µÚ1Ö¡±êÖ¾
							}
						}
					}
				#endif
				}
			#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
				i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+18,1);//µçÄÜ·ÑÂÊ¸öÊý
				if((i==1)&&(p8RMLib[0]==0xE3))
				{//±ä»»ÈÕ¶³½áÊý¾Ý±êÊ¶
					switch(UARTCtrl->RMCount)
					{
						default://ÈÕ¶³½áÊ±¼äYYMMDDhhmm
							UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
							UARTCtrl->RMCount++;//³­±íÊý¾Ý±êÊ¶¼ÆÊý+1
							UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
							return;
						case 1://ÉÏ1´ÎÈÕÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
							i=0x00010000;//µ±Ç°ÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
							break;
					}
					MWR(i,((u32)p8txbuff)+4+10,4);
					//¼Ó0x33
					for(i=0;i<4;i++)
					{
						p8txbuff[4+10+i]+=0x33;
					}
					//cs
					p8txbuff[4+14]=0;
					for(i=0;i<14;i++)
					{
						p8txbuff[4+14]+=p8txbuff[4+i];
					}
				}
			#endif
				Fn=p8RMLib[0];
				if((Fn>=0xe4)&&(Fn<=0xe9))
				{
				#if (((Project/100)==2)&&((USER/100)==5))//ÉÏº£Ïµ¼¯ÖÐÆ÷
					//²¹µ±Ç°µ½µ±ÈÕ0µãÇúÏßµã
					YMDHM=MRR(ADDR_TYMDHMS+1,5);//È¡µ±Ç°Ê±¼äÄêÔÂÈÕÊ±·Ö
					YMDHM&=0xffffffff00;
					i=UARTCtrl->RMCount*60;//Ã¿60·Ö¼ä¸ô
					YMDHM=YMDHM_SubM(YMDHM,i);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
					i=Fn;
					i-=0xe4;
					i<<=16;
					i+=0x06000001;
					PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//ÖÕ¶ËDL645-2007³­±í·¢ËÍ¸ººÉÇúÏßÊý¾Ý
				#else
					//²¹µ±Ç°µ½ÉÏÈÕ0µãÇúÏßµã
					YMDHM=MRR(ADDR_TYMDHMS+1,5);//È¡µ±Ç°Ê±¼äÄêÔÂÈÕÊ±·Ö
					YMDHM=YMDHM_Int15(YMDHM);//ÄêÔÂÈÕÊ±·Ö»¯ÎªÕû15·Ö,Èç00Ê±18·Ö»¯Îª00Ê±15·Ö
					i=UARTCtrl->RMCount*15;//Ã¿15·Ö¼ä¸ô
					YMDHM=YMDHM_SubM(YMDHM,i);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
					i=Fn;
					i-=0xe4;
					i<<=16;
					i+=0x06000001;
					PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//ÖÕ¶ËDL645-2007³­±í·¢ËÍ¸ººÉÇúÏßÊý¾Ý
				#endif
				}
			}
			else
			{//ÊÇºóÐøÖ¡
				p8txbuff[4+8]=0x12;
				p8txbuff[4+9]=5;
				p8txbuff[4+14]=UARTCtrl->SubsequentFrames+0x33;
				p8txbuff[4+15]=0;//cs
				for(i=0;i<15;i++)
				{
					p8txbuff[4+15]+=p8txbuff[4+i];
				}
				p8txbuff[4+16]=0x16;
				UARTCtrl->TxByte=4+17;//·¢ËÍ×Ö½ÚÊý
			}
	#endif//#if EventProject==0//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
	
			//Í¨ÐÅËÙÂÊ
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ä¬ÈÏ
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485¿ÚÄ¬ÈÏµÄËÙÂÊ¿ØÖÆÂë0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			//Æô¶¯
			Init_UARTn(PORTn);//³õÊ¼»¯UART¿Ú,Èë¿ÚÍ¨ÐÅ¿ØÖÆ×ÖÒÑÌîÈë
			Start_UARTn_Tx(PORTn);//Æô¶¯UART¿Ú·¢ËÍ
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
#endif
			//ÏÔÊ¾³­±íµØÖ·
			switch(UARTCtrl->RMTaskC)
			{
				case 0://ÉÏµç
					CopyString((u8 *)"485 Æô³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					break;
				case 1://ÈÕ³­
					if(UARTCtrl->DateReRMCount<=1)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ÐÐ×Ü´ÎÊý;0=¿Õ(ÉÏµç»òÈÕ¸üÐÂ),1-255=×Ü´ÎÊý¼ÆÊý
					{
						CopyString((u8 *)"485 ÈÕ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					}
					else
					{
						CopyString((u8 *)"485 ²¹³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					}
					break;
				case 2://ÖÜ³­
					CopyString((u8 *)"485 Ê±³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					//CopyString((u8 *)"485 ÖÜ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					break;
				case 3://¶¨³­
					CopyString((u8 *)"485 ¶¨³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					break;
				case (MaxRMTask-2)://ÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼þ
					switch(UARTCtrl->RMEventTaskC)//µ±Ç°³­¶ÁµçÄÜ±íÊÂ¼þÈÎÎñ:0=¿Õ,1-3=1-3¼¶
					{
						case 1:
							CopyString((u8 *)"485  1¼¶",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
							break;
						case 2:
							CopyString((u8 *)"485  2¼¶",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
							break;
						case 3:
							CopyString((u8 *)"485  3¼¶",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
							break;
					}
					break;
				default://Ñ­³­
					CopyString((u8 *)"485 Ñ­³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					break;
			}
		#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
			if(UARTCtrl->RMMeterEvent&1)//ÕýÔÚ³­¶ÁµçÄÜ±íÊÂ¼þ±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼þ,b1=
			{
				CopyString((u8 *)"485 ÉÏ±¨",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
			}
		#endif
			MWR(PORT485,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8txbuff[10-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
			break;
		case 4://4=ÕýÔÚ·¢ËÍ
			if(Wait_UARTn_Tx(PORTn)==0)//µÈ´ýUART¿Ú·¢ËÍ,·µ»Ø0=·¢ËÍÍê³É,1=·¢ËÍÃ»Íê³É
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645¶Á±í³¬Ê±¶¨Ê±Öµms
				UARTCtrl->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
			}
			break;
		case 5://5=ÕýÔÚ½ÓÊÕ
			i=DL645_Rx(PORTn);//DL645Ö¡½ÓÊÕ;·µ»Ø0Ã»Íê³É,1Íê³É,2³¬Ê±
			switch(i)
			{
				case 0://0Ã»Íê³É
					return;
				case 1://1Íê³É
					break;
				default://2³¬Ê±
					UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
					return;
			}	
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
#endif
			p16timer[0]=30/10;//ÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±20MSºó
			
	#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			if(UARTCtrl->SubsequentFrames!=0)//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
			{//ÊÇºóÐøÖ¡
				if((p8RMLib[0]<2)||(p8RMLib[0]>3))
				{//²»ÊÇ¶ÁÖ÷¶¯ÉÏ±¨ÊÂ¼þºÍÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼þ
					//¼õ0x33
					for(i=0;i<p8rxbuff[9];i++)
					{
						p8rxbuff[10+i]-=0x33;
					}
					i=MRR(((u32)p8rxbuff)+10,4);//·µ»ØµÄÊý¾Ý±êÊ¶
					if(i==0x04001501)
					{//ÊÇÊÕµ½Ö÷¶¯ÉÏ±¨Ö¡
						if((UARTCtrl->RMCount+1)>=p8RMLib[1])//³­±íÃüÁî¼ÆÊý
						{//Ö»¼Ç1´Î,·ñÔòµ±Ö÷¶¯ÉÏ±¨ÎÞÊÂ¼þ¼ÆÊýÊ±Ã¿FnÖÐÓÐ¶à¸ö³­±íÃüÁîÊ±»á¼Ç¶à´Î.
							WriteAutoReportWord(PORTn,((u32)p8rxbuff));//Ð´µçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;Èë¿Ú:¶Ë¿ÚºÅ(1-31),Í¨ÐÅµØÖ·,×Ô¶¯ÉÏ±¨×´Ì¬×ÖµØÖ·
						}
						UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
						UARTCtrl->Task=0;//¿ÕÏÐ1´Î
						UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
						return;
					}
					if(p8rxbuff[8]&0x20)
					{//ÓÐºóÐøÖ¡
						UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
					}
					else
					{//ÎÞºóÐøÖ¡
						UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
					}
					UARTCtrl->Task=0;//¿ÕÏÐ1´Î
					UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
					return;
				}
			}
			if(p8rxbuff[8]&0x20)
			{//ÓÐºóÐøÖ¡
			#if EnMeterAutoAutoReport==1//ÔÊÐíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ:0=²»ÔÊÐí,1=ÔÊÐí
				UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
			#else
				if((p8RMLib[0]==2)||(p8RMLib[0]==3))
				{//ÔØ²¨Ö»ÓÐÖ÷¶¯³­ÊÂ¼þ»òÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼þÊ±²ÅÔÊÐíºóÐøÖ¡
					UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
				}
			#endif
			}
			else
			{//ÎÞºóÐøÖ¡
				UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
			}
	#endif
			//±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33
			i=DL645_2007_RxData(PORTn);//DL645_2007Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í

	#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
			if(i==3)
			{//3Êý¾Ý±êÊ¶´í
				//´íÎóÊ±Ã»¼õ0x33
				for(x=0;x<p8rxbuff[9];x++)
				{
					p8rxbuff[10+x]-=0x33;
				}
				x=MRR(((u32)p8rxbuff)+10,4);
				if((p8RMLib[0]==2)&&((UARTCtrl->RMCount==(2+(96*2)))||(x==0x04001501)))
				{//F2 µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þµÄ¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö»Ø´ð»òºóÐøÖ¡
					i=0;//ÕýÈ·
				}
				if((p8RMLib[0]==3)&&(x==0x04001501))
				{//F3 ÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼þµÄÖ÷¶¯ÉÏ±¨×´Ì¬×ÖºóÐøÖ¡
					i=0;//ÕýÈ·
				}
			#if ((USER/100)==5)//ÉÏº£ÓÃ»§
			#if EnMeterAutoAutoReport==0//ÔÊÐíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ:0=²»ÔÊÐí,1=ÔÊÐí
				if((p8RMLib[0]==3)&&(UARTCtrl->RMCount==((60)*3)+1))
				{//F3 ÖÜÆÚ¶ÁµçÄÜ±íÊÂ¼þµÄ¸´Î»Ö÷¶¯ÉÏ±¨×´Ì¬×Ö»Ø´ð
					i=0;//ÕýÈ·
				}
			#endif
			#endif
			}
	#endif
			if(i!=0)
			{
				if(i==2)
				{//´ÓÕ¾Òì³£»Ø´ð
					UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
					UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
					UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
					UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîÈë
					break;
				}
				//µØÖ·»òÊý¾Ý±êÊ¶´í,µç¿ÆÔº²âÊÔÈí¼þÄ£Äâ±í»áÍ£Ö¹µ«½ÓÊÕÊý¾ÝÕý³£,»Ö¸´Ä£Äâ±íºó»á½«ÊÕµ½µÄÖ¡Í¬Ê±·¢³ö
				//¼ÌÐø½ÓÊÕ
				p16timer[0]=ReadDL645OverTimer/10;//DL645¶Á±í³¬Ê±¶¨Ê±Öµms
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=ÕýÔÚ½ÓÊÕ
				break;
			}
			//·µ»Ø³¤¶È
			if(p8rxbuff[9]==4)
			{//µç¿ÆÔº²âÊÔÈí¼þµ±ÊÂ¼þ¼ÇÂ¼ÎÞÊý¾ÝÊ±,Ö»·µ»Ø4×Ö½ÚÊý¾Ý±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ð
				UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
				UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîÈë
				break;
			}
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			Fn=p8RMLib[0];
			if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
			{//²»ÊÇ¶Áµç±íÇúÏß
				p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
			}
			else
			{//ÊÇ¶Áµç±íÇúÏß
				p8RMLib+=10+4;
			}
	#if EventProject==0//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
			if(p8RMLib[0]!=0x0)
	#else
			if((p8RMLib[0]!=0x0)&&(p8rxbuff[8]!=0x92)&&(p8rxbuff[8]!=0xB2))
			//Ó¦·µ»Ø³¤¶È²»Îª0 Í¬Ê± ²»ÊÇºóÐøÖ¡
	#endif
			{
			//!!!2015-7-29ÖÜÆÚÊÂ¼þ³­¶ÁÐÞ¸Ä
				if((Fn==3)&&(UARTCtrl->RMDI!=0xffffffff))
				{
					goto SKIP;//ÖÜÆÚÊÂ¼þ³­¶Á·Ç¿âÖÐÃüÁîÊ±ÎÞ·µ»Ø³¤¶È¿É±È½Ï
				}
				if(p8rxbuff[9]!=(p8RMLib[0]+4))
				{
				#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
					if(p8RMLib[0]==2)
					{//F2 µçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼þ
						if(p8rxbuff[9]>=4)
						{
							goto SKIP;//²»±È½Ï³¤¶È
						}
					}
				#endif
					if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
					{//¿éÃüÁîºó¿ÉÄÜÓÐ0xaa
						UARTCtrl->Task=0;//¿ÕÏÐ1´Î
						UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
						UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
						UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîë
						break;
					}
				}
			}
SKIP:
			UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
			p8RMLib=(u8 *)DL645_2007_RMLib[UARTCtrl->FnCount];
			ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007³­±í½ÓÊÕÊý¾ÝÌîÈëFnÊý¾Ý»º³å
			
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			Fn=p8RMLib[0];
			//¶Áµç±í1-6Àà¸ººÉÇúÏß
			if((Fn>=0xe4)&&(Fn<=0xea))
			{
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý;ÈôÖ»´æ´¢ÊµÊ±Êý¾Ýu8 *p8RMLib=0
				UARTCtrl->RMCountErr=255;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý=×î´óÖµÃâ×î´Î´æ´¢Êý¾Ý
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			//À©Õ¹13 ÉÏ1-24Õûµãµç±í¶³½áµÄÕý·´ÓÐ¹¦µçÄÜÊ¾Öµ
			if(Fn==0xED)
			{
				YMDHM=MRR(((u32)p8FnData)+6+1,4);//È¡µç±í·µ»ØµÄ¶³½áÊ±±êÄêÔÂÈÕÊ±
				YMDHM<<=8;
				AddCurveCongeal(YMDHM,101,Pn,((u32)p8FnData)+6+1+4,4);//²¹ÇúÏß¶³½áµãÊý¾Ý
				AddCurveCongeal(YMDHM,103,Pn,((u32)p8FnData)+6+1+4+4,4);//²¹ÇúÏß¶³½áµãÊý¾Ý
			#if (USER/100)==11//¼ªÁÖÏµ
				//¼ªÁÖÌ¨Ìå²âÊÔÓÐÕû°ëµã¶³½áÊý¾Ý
				YMDHM=YMDHM_SubM(YMDHM,30);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
				AddCurveCongeal(YMDHM,101,Pn,((u32)p8FnData)+6+1+4,4);//²¹ÇúÏß¶³½áµãÊý¾Ý
			#endif
			}
#if (USER/100)==11//¼ªÁÖÏµ
			if(Fn==0xFB)
			{
				YMDHM=MRR(((u32)p8FnData)+6+1,4);//È¡µç±íµÄ¶³½áÊ±±êÄêÔÂÈÕÊ±
				YMDHM<<=8;
				AddCurveCongeal(YMDHM,89,Pn,((u32)p8FnData)+6+1+4,2);//²¹ÇúÏß¶³½áµãÊý¾Ý
				AddCurveCongeal(YMDHM+0x15,89,Pn,((u32)p8FnData)+6+1+4+2,2);//²¹ÇúÏß¶³½áµãÊý¾Ý
				AddCurveCongeal(YMDHM+0x30,89,Pn,((u32)p8FnData)+6+1+4+2+2,2);//²¹ÇúÏß¶³½áµãÊý¾Ý
				AddCurveCongeal(YMDHM+0x45,89,Pn,((u32)p8FnData)+6+1+4+2+2+2,2);//²¹ÇúÏß¶³½áµãÊý¾Ý
			}
			if(Fn==0xFA)
			{
				if(UARTCtrl->RMCount==2)
				{
					i=MRR(ADDR_TYMDHMS+4,2);//È¡µ±Ç°Ê±¼äÄêÔÂ
					i=YM_Sub1M(i);//ÄêÔÂ¼õ1ÔÂ,·µ»Ø¼õ1ºóµÄÄêÔÂ
					AddMonthCongeal(i,35,Pn,((u32)p8FnData)+6+1,93);//²¹ÔÂ¶³½áµãÊý¾Ý
				}
			}
#endif
	#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­
			if((Fn==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))
			{
		 		MR(ADDR_DATABUFF,((u32)p8FnData)+1+6,6);
				MR(ADDR_DATABUFF+6,ADDR_TYMDHMS,6);
				p8=(u8 *)(ADDR_DATABUFF);
				if(p8[0]!=0xee)
				{
					p8[4]&=0x1f;//È¥¼ÓÔÚÔÂ×Ö½ÚÉÏµÄÐÇÆÚ
					p8[10]&=0x1f;//È¥¼ÓÔÚÔÂ×Ö½ÚÉÏµÄÐÇÆÚ
					i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//ÄêÔÂÈÕÊ±·ÖÃëºÏ·¨¼ì²é,·µ»Ø0ºÏ·¨,1·Ç·¨
					if(i==0)
					{//µçÄÜ±íÊ±ÖÓºÏ·¨
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								if((i&0x02)==0)
								{
									i|=0x02;
								}
								break;
							case RS485_2PORT://RS485-2
								if((i&0x04)==0)
								{
									i|=0x04;
								}
								break;
						}
						MWR(i,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//Ê±ÖÓ²îÖµ¼ÆËã,Èë¿ÚR0=Ê±ÖÓ1(Ãë·ÖÊ±ÈÕÔÂÄê)µØÖ·;R1=Ê±ÖÓ2(Ãë·ÖÊ±ÈÕÔÂÄê)µØÖ·;³ö¿ÚR0=HEX²¹ÂëÊ±ÖÓ¿ì»òÂýµÄ²îÖµ(Ãë),Ê±ÖÓ1Ê±ÖÓ2·Ç·¨R0=0,Ê±ÖÓ2>=Ê±ÖÓ1²îÖµÎªÕý,Ê±ÖÓ2<Ê±ÖÓ1²îÖµÎª¸º
						if((i>>31)!=0x0)
						{//¸º
							i=~i;
							i++;
						}
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								x=0;
								break;
							case RS485_2PORT://RS485-2
								x=1;
								break;
						}
						x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
						if(x<4)
						{
							MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
							MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//µçÄÜ±í¶ÔÊ±Ç°µÄÎó²î
							MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//µçÄÜ±í¶ÔÊ±ºóµÄÎó²î
							MWR(2,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//µçÄÜ±í¶ÔÊ±ºóµÄ½á¹û
						}
						if(x==4)
						{
							x=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//µçÄÜ±í¶ÔÊ±Ç°µÄÎó²î
							if(x>MRR(ADDR_AFN04F298_Ext+7,1))
							{
								MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
								MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//µçÄÜ±í¶ÔÊ±ºóµÄÎó²î
								if(i<=MRR(ADDR_AFN04F298_Ext+7,1))
								{//³É¹¦
									MWR(1,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//µçÄÜ±í¶ÔÊ±ºóµÄ½á¹û
								}
								else
								{//Ê§°Ü
									MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//µçÄÜ±í¶ÔÊ±ºóµÄ½á¹û
								}
								//ERC51
								p8=(u8 *)ADDR_DATABUFF;
								//µçÄÜ±í¶ÔÊ±Ç°µÄÎó²î
								MWR(x,ADDR_DATABUFF+9,2);
								//µçÄÜ±í¶ÔÊ±ºóµÄÎó²î
								MWR(i,ADDR_DATABUFF+11,2);
								p8[13]=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);
								ERC51_Event(Pn+(1<<15),ADDR_DATABUFF);//Ð£Ê±¼ÇÂ¼ÊÂ¼þ;Èë¿Ú:Pn²âÁ¿µãºÅ(´Ó0¿ªÊ¼)
							#if (USER/100)==8//ÓÃ»§±êÊ¶£ºÍ¨ÓÃ
								p8=(u8 *)ADDR_TYMDHMS;
								MR((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1),3);
								x=DataComp((u32)ADDR_DATABUFF,(u32)ADDR_TYMDHMS+3,3);//Êý¾Ý±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
								if(x==0)
								{
									x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);
									if(x==0xffffff)
									{
										DMWR(i,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);//¼Ä´æÆ÷Ö±½ÓÐ´µ½´æ´¢Æ÷(<=8×Ö½Ú)(ÎÞ²Á³ý)
									}
								}
								//Ê±¼ä³¬²î 5min ²âÁ¿µãÊýÁ¿
								if(i>300)
								{
									x=MRR(ADDR_AFN0DF321_Ext_Source,2);
									x++;
									MWR(x,ADDR_AFN0DF321_Ext_Source,2);
								}
								//³¬¹ýÊ±¼äãÐÖµÎ´³¬²î5min²âÁ¿µãÊýÁ¿
								if((i>MRR(ADDR_AFN04F298_Ext+7,1))&&(i<300))
								{
									x=MRR(ADDR_AFN0DF321_Ext_Source+2,2);
									x++;
									MWR(x,ADDR_AFN0DF321_Ext_Source+2,2);
								}
							#endif
							}
							x=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),1);//µçÄÜ±í¶ÔÊ±´ÎÊý
							if(x<31)
							{
								x++;
							}
							MWR(x,ADDR_DATABUFF,1);
							MR(ADDR_DATABUFF+1,ADDR_AFN0CF250_Ext1+10*(Pn-1),10);
							if(x>=31)
							{
								x=30;
							}
							MR(ADDR_DATABUFF+11,ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1)+1,10*x);
							MR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),ADDR_DATABUFF,1+10*x+10);
						}
					}
				}
			}
	#endif
			UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
			UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý=0
			break;
	}
}


void Terminal_ReadMeter_DL645_2007_Fn(u32 PORTn,u8 *p8RMLib)//½ö³­1¸öFn>=129Êý¾ÝÌîÐ´µ½FnDataBuff;Task=0=¿ÕÏÐ(±¾Fn³­¶ÁÍê³É)
{
	u32 i;
	u32 x;
	u32 Pn;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	u64 YMDHM;
	
//	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);

  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(UARTCtrl->Task)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=¿ÕÏÐ
			break;
		case 1://1=Íê³É1Ö¡·¢ÊÕ
			UARTCtrl->Task=3;
			break;
		case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
			if(UARTCtrl->ReRMCount<1)
			{
				UARTCtrl->ReRMCount+=1;//³­±íÃüÁîÖØ·¢¼ÆÊý+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
			}
			UARTCtrl->Task=3;
			break;
		case 3://3=Æô¶¯·¢ËÍ
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//±¾ÐòºÅµÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÉ¾³ý
			END_Fn:
				UARTCtrl->Task=0;//¿ÕÏÐ
				UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊý=0
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				break;
			}
			if((p8RMLib[3]&0x2)==0x0)
			{//±¾Fn²»ÊÇRS485³­±í
				goto END_Fn;
			}
//			Fn=p8RMLib[0];

			if(p8RMLib[1]==0x0)
			{//±¾Fn³­±íÃüÁîÊý=0
				goto END_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//³­±íÃüÁî¼ÆÊý=0,ÇåFnÊý¾Ý»º³å
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//³­±íÃüÁî¼ÆÊý>=FnÐè·¢ÃüÁîÊý;±¾Fn³­±íÍê³É
				if(p8RMLib[0]==28)
				{//F28 µç±íÔËÐÐ×´Ì¬×Ö¼°Æä±äÎ»±êÖ¾
					ReadMeter_SaveFnData(PORTn,p8RMLib[0],Pn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//´æÖü³­±íFnÊý¾Ý;ÈôÖ»´æ´¢ÊµÊ±Êý¾Ýu8 *p8RMLib=0
				}
				goto END_Fn;
			}
			
			//¶ÁÊý¾ÝÖ¡
			if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
			{//²»ÊÇ¶Áµç±íÇúÏß
				Terminal_ReadMeter_DL645_2007_TxData(PORTn,p8RMLib);//ÖÕ¶ËDL645-2007³­±í·¢ËÍÊý¾Ý
			}
			else
			{//¶Áµç±íÇúÏß
				i=MRR(((u32)p8RMLib)+10,4);
				YMDHM=MRR((u32)&UARTCtrl->RMCommandYMDHM[0],5);//ÇúÏßµÄÊ±±êÄêÔÂÈÕÊ±·Ö
			#if (((Project/100)==2)&&((USER/100)==5))//ÉÏº£Ïµ¼¯ÖÐÆ÷
				YMDHM&=0xffffffff00;
			#endif
				PORTn_ReadMeter_DL645_2007_TxCurveData(PORTn,i,YMDHM);//ÖÕ¶ËDL645-2007³­±í·¢ËÍ¸ººÉÇúÏßÊý¾Ý
			}
			
			//Í¨ÐÅËÙÂÊ
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ä¬ÈÏ2400
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485¿ÚÄ¬ÈÏµÄËÙÂÊ¿ØÖÆÂë0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}
			//Æô¶¯
			Init_UARTn(PORTn);//³õÊ¼»¯UART¿Ú,Èë¿ÚÍ¨ÐÅ¿ØÖÆ×ÖÒÑÌîÈë
			Start_UARTn_Tx(PORTn);//Æô¶¯UART¿Ú·¢ËÍ
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
#endif
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					Pn=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					Pn=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					Pn=3;//RS485PORT=3;
					break;
				default:
					Pn=0;//RS485PORT=0;
					break;
			}
			//ÏÔÊ¾³­±íµØÖ·
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			CopyString((u8 *)"485 Êµ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
			MWR(Pn,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8rxbuff[10-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
			break;
		case 4://4=ÕýÔÚ·¢ËÍ
			if(Wait_UARTn_Tx(PORTn)==0)//µÈ´ýUART¿Ú·¢ËÍ,·µ»Ø0=·¢ËÍÍê³É,1=·¢ËÍÃ»Íê³É
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645¶Á±í³¬Ê±¶¨Ê±Öµms
				UARTCtrl->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
			}
			break;
		case 5://5=ÕýÔÚ½ÓÊÕ
			i=DL645_Rx(PORTn);//DL645Ö¡½ÓÊÕ;·µ»Ø0Ã»Íê³É,1Íê³É,2³¬Ê±
			switch(i)
			{
				case 0://0Ã»Íê³É
					return;
				case 1://1Íê³É
					break;
				default://2³¬Ê±
					UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
					return;
			}	
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
			}
#endif
			p16timer[0]=30/10;//ÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±20MSºó

			//±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33
			i=DL645_2007_RxData(PORTn);//DL645_2007Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í
			if(i!=0)
			{
				if(i==2)
				{//´ÓÕ¾Òì³£»Ø´ð
					UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
					UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
					UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
					break;
				}
				//µØÖ·»òÊý¾Ý±êÊ¶´í,µç¿ÆÔº²âÊÔÈí¼þÄ£Äâ±í»áÍ£Ö¹µ«½ÓÊÕÊý¾ÝÕý³£,»Ö¸´Ä£Äâ±íºó»á½«ÊÕµ½µÄÖ¡Í¬Ê±·¢³ö
				//¼ÌÐø½ÓÊÕ
				p16timer[0]=ReadDL645OverTimer/10;//DL645¶Á±í³¬Ê±¶¨Ê±Öµms
				UARTCtrl->RxByte=0;
				UARTCtrl->Task=5;//5=ÕýÔÚ½ÓÊÕ
				break;
			}
	
			//·µ»Ø³¤¶È
			p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
			if(p8RMLib[0]!=0x0)
			{
				if(p8rxbuff[9]!=(p8RMLib[0]+4))
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
					{//¿éÃüÁîºó¿ÉÄÜÓÐ0xaa
						if(p8rxbuff[9]==4)
						{//µç¿ÆÔº²âÊÔÈí¼þµ±ÊÂ¼þ¼ÇÂ¼ÎÞÊý¾ÝÊ±,Ö»·µ»Ø4×Ö½ÚÊý¾Ý±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ð
							UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
							UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
							UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
						}
						else
						{
							UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
						}
						break;
					}
				}
			}
			p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
			UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
			
			ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007³­±í½ÓÊÕÊý¾ÝÌîÈëFnÊý¾Ý»º³å
			UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
			UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý=0
			break;
	}
}











