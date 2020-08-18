
//ÖÕ¶Ë³­±íRS485¿Ú_DL645_1997
#include "Project.h"
#include "Terminal_ReadMeter_DL645_1997.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
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



extern const u32 DL645_1997_RMLib[];
u32 Get_RM_DL645_1997_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];

void Terminal_ReadMeter_DL645_1997(u32 PORTn)//ÖÕ¶Ë³­±íRS485¿Ú_DL645-1997
{
	u32 i;
	u32 x;
	u32 Fn;
	u32 Pn;
	u8 * p8RMLib;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u16 * p16timer;
	u8 * p8FnData;
	u32 PORT485;
	u32 ClassNo;
#if Extern_ReadMeter_Fn_YesNo!=0//Íâ²¿³ÌÐò:0=I2,1=E1,°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý,Protocol(1=1997,ÆäËû=2007)
	u32 *p32;
#endif
#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­
	u8 *p8;
#endif

	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
	u32 LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	u32 y;


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
				Fn=MRR(DL645_1997_RMLib[UARTCtrl->FnCount],1);
				if(Fn!=0xe1)
				{
					UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý
				}
				switch(Fn)
				{
					default:
						UARTCtrl->RMError|=1;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
						break;
					case 129://F129 µ±Ç°ÕýÏòÓÐ¹¦µçÄÜÊ¾Öµ
						UARTCtrl->RMError|=2;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÐòºÅÍ¨ÐÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
						break;
					case 0xE1://À©Õ¹1 ERC8µçÄÜ±í²ÎÊý±ä¸üÊÂ¼þÓÃÊý¾Ý
						i=UARTCtrl->RMCount;
						if(i<14)//³­±íÃüÁî¼ÆÊý
						{//µÚ1ÈÕÊ±¶Î±í1-14¶Î
							UARTCtrl->RMCount=14;
						}
						break;
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
				default:
					return;
			}
#if (((Project/100)==2)||((Project/100)==5))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=²É¼¯Æ÷,600=Í¨ÐÅÄ£¿é,700=
			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//ÖÕ¶Ë³­±íÔËÐÐ²ÎÊýÉèÖÃ,¶Ë¿Ú31
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
			if(UARTCtrl->FnCount>=Get_RM_DL645_1997_MaxFn())
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
			if(DL645_1997_RMLib[UARTCtrl->FnCount]==0x0)
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
			p8RMLib=(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//±¾Fn²»ÊÇRS485³­±í
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];
			
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//È¡´óÐ¡ÀàºÅ			
		#if Extern_ReadMeter_Fn_YesNo==0//Íâ²¿³ÌÐò:0=I2,1=E1,°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý,Protocol(1=1997,ÆäËû=2007)	
			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,1);//°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý
		#else
			y=0;
			if((Comm_Ram->SoftModule&(1<<4))==0)//Èí¼þÄ£¿é´íÎó±êÖ¾(ÖÃÎ»±íÊ¾´íÎó):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
			{
				p32=(u32 *)(ADDR_E1_ENTRY);
				y=(*(u32(*)())p32[73])((u32)PORTn,(u32)ClassNo,(u32)Pn,(u8*)p8RMLib,(u32)UARTCtrl->RMCount,(u32)1);//°´´óÐ¡ÀàºÅ³­±íµÄFnÊÇ·ñÒª³­;·µ»Ø0=²»³­,1=Òª³­;Èë¿Ú:ClassNoµÄD7-D4´óÀàºÅD3-D0Ð¡ÀàºÅ,Pn,p8RMLib³­±í¿âÖ¸Õë,RMCount³­±í¼ÆÊý
			}
		#endif
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
				goto NEXT_Fn;
			}
		
			Terminal_ReadMeter_DL645_1997_TxData(PORTn,p8RMLib);//ÖÕ¶ËDL645-1997³­±í·¢ËÍÊý¾Ý
			
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
			
			//±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33
			i=DL645_1997_RxData(PORTn);//DL645_2007Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í
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
			if(p8rxbuff[9]==2)
			{//µç¿ÆÔº²âÊÔÈí¼þµ±ÊÂ¼þ¼ÇÂ¼ÎÞÊý¾ÝÊ±,Ö»·µ»Ø2×Ö½ÚÊý¾Ý±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ð
				UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
				UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
				UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîÈë
				break;
			}
			p8RMLib=(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount];
			p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
			if(p8RMLib[0]!=0x0)
			{
				if(p8rxbuff[9]!=(p8RMLib[0]+2))
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
					{//¿éÃüÁîºó¿ÉÄÜÓÐ0xaa
						UARTCtrl->Task=0;//¿ÕÏÐ1´Î
						UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
						UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý
						UARTCtrl->RMCountErr++;//FnÄÚÓÐ³­¶ÁÊ§°Ü»ò²»³­µÄÊý¾Ý±êÊ¶¼ÆÊý,×÷ÎªÊ§°ÜÃâÈ«Ã»³­Ê±¿ÕÊý¾ÝÌîë
						break;
					}
				}
			}

			UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
			p8RMLib=(u8 *)DL645_1997_RMLib[UARTCtrl->FnCount];
			ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997³­±í½ÓÊÕÊý¾ÝÌîÈëFnÊý¾Ý»º³å
	#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­
			Fn=p8RMLib[0];
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
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


void Terminal_ReadMeter_DL645_1997_Fn(u32 PORTn,u8 *p8RMLib)//½ö³­1¸öFnÊý¾ÝÌîÐ´µ½FnDataBuff;Task=0=¿ÕÏÐ(±¾Fn³­¶ÁÍê³É)
{
	u32 i;
	u32 x;
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
			
			Terminal_ReadMeter_DL645_1997_TxData(PORTn,p8RMLib);//ÖÕ¶ËDL645-1997³­±í·¢ËÍÊý¾Ý
			
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
			
			//±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33
			i=DL645_1997_RxData(PORTn);//DL645_2007Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í
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
			p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
			if(p8RMLib[0]!=0x0)
			{
				if(p8rxbuff[9]!=(p8RMLib[0]+2))
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
					{//¿éÃüÁîºó¿ÉÄÜÓÐ0xaa
						if(p8rxbuff[9]==2)
						{//µç¿ÆÔº²âÊÔÈí¼þµ±ÊÂ¼þ¼ÇÂ¼ÎÞÊý¾ÝÊ±,Ö»·µ»Ø2×Ö½ÚÊý¾Ý±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ð
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
			p8RMLib-=10+((3*UARTCtrl->RMCount)+2);
			UARTCtrl->Task=1;//Íê³É1Ö¡·¢ÊÕ
			
			ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997³­±í½ÓÊÕÊý¾ÝÌîÈëFnÊý¾Ý»º³å
			UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊý+1
			UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊý=0
			break;
	}
}






















