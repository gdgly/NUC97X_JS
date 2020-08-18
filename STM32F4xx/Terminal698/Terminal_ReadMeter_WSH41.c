
//ÖÕ¶Ë³­±íRS485¿Ú_WSH41
#include "Project.h"
#include "Terminal_ReadMeter_WSH41.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_WSH.h"
#include "Terminal_ReadMeter_WSH41_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"


#if (USER/100)==10//ºÓ±±
extern const u32 WSH41_RMLib[];
u32 Get_RM_WSH41_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];

void Terminal_ReadMeter_WSH41(u32 PORTn)//ÖÕ¶Ë³­±íRS485¿Ú_ÍþÊ¤¹æÔ¼4.1°æ±¾
{
	u32 i;
	u32 Fn;
	u32 Pn;
	u8 * p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
//	u32 ClassNo;
	u64 d64a;

	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
//	u32 y;


  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
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
				Fn=MRR(WSH41_RMLib[UARTCtrl->FnCount],1);
//				if(Fn!=0xe1)
//				{
					UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
//				}
				switch(Fn)
				{
					default:
						UARTCtrl->RMError|=1;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
						break;
					case 129://F129 µ±Ç°ÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ
						UARTCtrl->RMError|=2;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
						break;
//					case 0xE1://À©Õ¹1 ERC8µçÄÜ±í²ÎÊý±ä¸üÊÂ¼þÓÃÊý¾Ý
//						i=UARTCtrl->RMCount;
//						if(i<14)//³­±íÃüÁî¼ÆÊý
//						{//µÚ1ÈÕÊ±¶Î±í1-14¶Î
//							UARTCtrl->RMCount=14;
//						}
//						break;
				}
			}
			break;
		case 3://3=Æô¶¯·¢ËÍ
			//È¡Í¨ÐÅµØÖ·
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);
			if(d64a>0x255)
			{//ÍþÊ¤¹æÔ¼µÄÍ¨ÐÅµØÖ·½öÖ§³Ö0~255
				goto END_Fn;//²»³­
			}
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
				default:
					return;
			}
//#if (((Project/100)==2)||((Project/100)==5))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=²É¼¯Æ÷,600=Í¨ÐÅÄ£¿é,700=
//			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//ÖÕ¶Ë³­±íÔËÐÐ²ÎÊýÉèÖÃ,¶Ë¿Ú31
//			if((i&0x02)!=0x0)
//			{//D1ÖÃ"1"ÒªÇóÖÕ¶ËÖ»³­ÖØµã±í£¬ÖÃ"0"ÒªÇóÖÕ¶Ë³­ËùÓÐ±í
//				y=MRR(ADDR_AFN04F35,1);//Ì¨Çø¼¯ÖÐ³­±íÖØµã»§ÉèÖÃ
//				if(y>MaxKeyUser)
//				{
//					y=MaxKeyUser;
//				}
//				for(i=0;i<y;i++)
//				{
//					if(MRR(ADDR_AFN04F35+1+(2*i),2)==UARTCtrl->MeterNo)
//					{
//						break;
//					}
//				}
//				if(i==y)
//				{//²»ÊÇÖØµã»§
//					goto END_Fn;//²»³­
//				}
//			}
//			i=MRR(ADDR_AFN04F30+(Pn-1),1);//Ì¨Çø¼¯ÖÐ³­±íÍ£³­/Í¶³­ÉèÖÃ(²âÁ¿µãºÅ)
//			if(i!=0)
//			{//Í£³­
//				goto END_Fn;//²»³­
//			}
//#endif
			if(UARTCtrl->FnCount>=Get_RM_WSH41_MaxFn())
			{//Fn¼ÆÊý>=×î´óFn¸öÊý,±¾±í³­±íÍê³É
				if(UARTCtrl->RMError==0)
				{//³­±í³É¹¦
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
					ReadMeterNote_OK(PORTn,Pn,0);//¼¯ÖÐ³­±íµç±í³­¶ÁÐÅÏ¢.³É¹¦
					DateRMFlags(Pn,0);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
				}
				else
				{//³­±íÊ§°Ü
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓÐ1¸öÊý¾Ý±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
					ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖÐ³­±íµç±í³­¶ÁÐÅÏ¢.Ê§°Ü
					DateRMFlags(Pn,1);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
				}
END_Fn:
				UARTCtrl->Task=0;//0=¿ÕÏÐ
				UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊý½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
				UARTCtrl->FnCount=0;//Fn¼ÆÊý=0
				UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊý=0
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				UARTCtrl->RMCountErr=0;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
				UARTCtrl->RMError=0;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				break;
			}
			if(WSH41_RMLib[UARTCtrl->FnCount]==0x0)
			{//±¾FnÎÞ³­±í
NEXT_Fn:
				UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
				UARTCtrl->FnCount+=1;//Fn¼ÆÊý+1
				UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊý=0
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				UARTCtrl->RMCountErr=0;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
				UARTCtrl->RMError&=0xfe;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				break;	
			}
			p8RMLib=(u8 *)WSH41_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//±¾Fn²»ÊÇRS485³­±í
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];
			
//			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//È¡´óÐ¡ÀàºÅ			
//			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,1);//°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý
//			if(y==0x0)
//			{//±¾ÃüÁî²»³­
//				if(p8RMLib[1]==0x0)
//				{//±¾Fn³­±íÃüÁîÊý=0
//					goto NEXT_Fn;
//				}
//				if(UARTCtrl->RMCount==0)//³­±íÊý¾Ý±êÊ¶¼ÆÊý
//				{//³­±íÊý¾Ý±êÊ¶¼ÆÊý=0,ÇåFnÊý¾Ý»º³å
//					UARTCtrl->RMCountErr=0;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
//					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
//				}
//				UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîÈë
//				UARTCtrl->RMCount++;//³­±íÊý¾Ý±êÊ¶¼ÆÊý
//				UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
//				if(UARTCtrl->RMCount>=p8RMLib[1])
//				{//³­±íÃüÁî¼ÆÊý>=FnÐè·¢ÃüÁîÊý;±¾Fn³­±íÍê³É
//				#if(RMM_RS485_FailData_0xEE==0)//RS485³­±íÊ§°ÜÊý¾ÝÌî0xee;0=·ñ,1=ÊÇ
//					if(p8RMLib[1]>UARTCtrl->RMCountErr)
//					{//²»ÊÇÈ«²¿±êÊ¶²»³­
//						goto OK_Fn;
//					}
//					goto NEXT_Fn;
//				#else
//					goto OK_Fn;
//				#endif
//				}
//				return;
//			}
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
//			OK_Fn:
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
				goto NEXT_Fn;
			}
		
			Terminal_ReadMeter_WSH41_TxData(PORTn,p8RMLib);//ÖÕ¶ËÍþÊ¤¹æÔ¼4.1°æ±¾³­±í·¢ËÍÊý¾Ý
			
			//Í¨ÐÅËÙÂÊ
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ä¬ÈÏ1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//Æô¶¯
			Init_UARTn(PORTn);//³õÊ¼»¯UART¿Ú,Èë¿ÚÍ¨ÐÅ¿ØÖÆ×ÖÒÑÌîÈë
			Start_UARTn_Tx(PORTn);//Æô¶¯UART¿Ú·¢ËÍ
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=²É¼¯Æ÷,600=Í¨ÐÅÄ£¿é
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
				#if (USER/100)==6//ÓÃ»§±êÊ¶:ºþÄÏÏµ
					CopyString((u8 *)"485 Ê±³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
				#else
					CopyString((u8 *)"485 ÖÜ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
				#endif
					break;
				case 3://¶¨³­
					CopyString((u8 *)"485 ¶¨³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					break;
				case 5://ÊÂ¼þ
					CopyString((u8 *)"485 ÊÂ¼þ",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					break;
				default://Ñ­³­
					CopyString((u8 *)"485 Ñ­³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
					break;
			}
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MWR(PORT485,ADDR_DATABUFF+3,1);
			for(i=0;i<9;i++)
			{
				MWR(0x30,ADDR_DATABUFF+8+i,1);
			}
			i=hex_bcd(p8rxbuff[0]);
			MWR(((i>>8)&0xf)+0x30,ADDR_DATABUFF+8+9,1);
			MWR(((i>>4)&0xf)+0x30,ADDR_DATABUFF+8+10,1);
			MWR((i&0xf)+0x30,ADDR_DATABUFF+8+11,1);
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
			i=WSH41_Rx(PORTn);//ÍþÊ¤½ÓÊÕ;·µ»Ø0Ã»Íê³É,1Íê³É,2³¬Ê±
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
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=²É¼¯Æ÷,600=Í¨ÐÅÄ£¿é
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
			
			i=WSH41_RxDATA(PORTn);//ÍþÊ¤²éÑ¯Ó¦´ðÃüÁî½ÓÊÕÊý¾Ý;·µ»Ø0Ó¦´ðÕýÈ·£¬1±íÖ·´í£¬2ÃüÁî±êÖ¾×Ö´í£¬3±êÖ¾µØÖ·´í£¬4Êý¾Ý³¤¶È´í
			if(i!=0)
			{//´ÓÕ¾Òì³£»Ø´ð
				UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
				break;
			}
			UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
			
			p8RMLib=(u8 *)WSH41_RMLib[UARTCtrl->FnCount];
			ReadMeter_WSH41_FnData(PORTn,p8RMLib);//WSH41³­±í½ÓÊÕÊý¾ÝÌîÈëFnÊý¾Ý»º³å
			UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
			UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý=0
			break;
	}
}


void Terminal_ReadMeter_WSH41_Fn(u32 PORTn,u8 *p8RMLib)//½ö³­1¸öFnÊý¾ÝÌîÐ´µ½FnDataBuff;Task=0=¿ÕÏÐ(±¾Fn³­¶ÁÍê³É)
{
	u32 i;
	u32 Pn;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
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
//				ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊý¾Ý
				goto END_Fn;
			}
			
			Terminal_ReadMeter_WSH41_TxData(PORTn,p8RMLib);//ÖÕ¶ËÍþÊ¤¹æÔ¼4.1°æ±¾³­±í·¢ËÍÊý¾Ý
			
			//Í¨ÐÅËÙÂÊ
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//Ä¬ÈÏ1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//Æô¶¯
			Init_UARTn(PORTn);//³õÊ¼»¯UART¿Ú,Èë¿ÚÍ¨ÐÅ¿ØÖÆ×ÖÒÑÌîÈë
			Start_UARTn_Tx(PORTn);//Æô¶¯UART¿Ú·¢ËÍ
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=²É¼¯Æ÷,600=Í¨ÐÅÄ£¿é
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
			for(i=0;i<9;i++)
			{
				MWR(0x30,ADDR_DATABUFF+8+i,1);
			}
			i=hex_bcd(p8rxbuff[0]);
			MWR(((i>>8)&0xf)+0x30,ADDR_DATABUFF+8+9,1);
			MWR(((i>>4)&0xf)+0x30,ADDR_DATABUFF+8+10,1);
			MWR((i&0xf)+0x30,ADDR_DATABUFF+8+11,1);
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
			i=WSH41_Rx(PORTn);//ÍþÊ¤½ÓÊÕ;·µ»Ø0Ã»Íê³É,1Íê³É,2³¬Ê±
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
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=²É¼¯Æ÷,600=Í¨ÐÅÄ£¿é
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
			
			i=WSH41_RxDATA(PORTn);//ÍþÊ¤²éÑ¯Ó¦´ðÃüÁî½ÓÊÕÊý¾Ý;·µ»Ø0Ó¦´ðÕýÈ·£¬1±íÖ·´í£¬2ÃüÁî±êÖ¾×Ö´í£¬3±êÖ¾µØÖ·´í£¬4Êý¾Ý³¤¶È´í
			if(i!=0)
			{//´ÓÕ¾Òì³£»Ø´ð
				UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
				break;
			}
			UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
			
			ReadMeter_WSH41_FnData(PORTn,p8RMLib);//WSH41³­±í½ÓÊÕÊý¾ÝÌîÈëFnÊý¾Ý»º³å
			UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
			UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý=0
			break;
	}
}

#endif




















