
//Ïş³¿ÔØ²¨Ä£¿é
//¼¼ÊõÖ§³Ö:010-68459012-8053 ËÎ¹¤
//010-68466652(´«Õæ)¸ßÀöÏ¼
//68459012-8011(µç»°)¸ßÀöÏ¼


#include "AllHead.h"
#include "Terminal_ReadMeter_3762_CX.h"
#include "Terminal_ReadMeter_3762.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../LPC177x8x/lpc177x8x_UART.h"
#include "../LPC177x8x/lpc177x8x_IO.h"
#include "../Calculate/Calculate.h"
#include "../Driver/MEMRW.h"
#include "../Display/Warning.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_DL645_1997_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"



#if (Project/100)==2//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=²É¼¯Æ÷,600=Í¨ĞÅÄ£¿é

#define RouterCheckTimerVal     (60*10)//ÏÂ´ÎÔØ²¨Â·ÓÉÆ÷¼ì²éÃë¶¨Ê±Öµ

void Terminal_ReadMeter_3762_CX(u32 PORTn)//Ïş³¿ÔØ²¨Ä£¿é
{
	u32 i;
	u8 * p8rxbuff;
//	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
//	u16 * p16timer;
//	u32 * p32;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
//	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
//	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(Terminal_Router->RouterInitTask)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	{
		case 100://ÉèÖÃÂ·ÓÉÔËĞĞÄ£Ê½
			Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
		case 101://¿ªÆôÂ·ÓÉ×éÍø
			if(Terminal_Router->NoteNum!=0)//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı
			{
				//×î´óÑ§Ï°Ê±¼ä¼ÆËã
				i=Terminal_Router->NoteNum;
				i*=30;//Æ½¾ùÃ¿±íÑ§Ï°Ê±¼ä30Ãë
				if(i>0xffff)
				{
					i=0xffff;
				}
				Terminal_Ram->RouterCheck_S_Timer=i;
				Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				Terminal_Router->RouterState=1;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
			}
			Terminal_Router->RouterInitSubTask=0;//×ÓÈÎÎñ
			Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
		case 102://²éÑ¯Â·ÓÉ×éÍøÍê³É
			if(Terminal_Router->NoteNum==0)//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı
			{
				Terminal_Router->RouterInitSubTask=0;//×ÓÈÎÎñ
				Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
				break;
			}
			
			if(MEMReadReg(ADDR_AFN05F49+30,1)==0x55)//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÔİÍ£³­±í
			{
				WarningOccur(PORTn,(u8 *)"ÔØ²¨ÃüÁîÔİÍ£³­±í");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
				if(Terminal_Router->RouterState==1)//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				{
					Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
					Terminal_Router->RouterState=0;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				}
				break;
			}
			i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//ÖÕ¶Ë³­±íÔËĞĞ²ÎÊıÉèÖÃ,¶Ë¿Ú31
			if((i&0x01)!=0x0)
			{//D0ÖÃ"1"²»ÔÊĞí×Ô¶¯³­±í£¬ÖÃ"0" ÒªÇóÖÕ¶Ë¸ù¾İ³­±íÊ±¶Î×Ô¶¯³­±í
				WarningOccur(PORTn,(u8 *)"ÔØ²¨²»ÔÊĞí×Ô¶¯³­±í");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
				if(Terminal_Router->RouterState==1)//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				{
					Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
					Terminal_Router->RouterState=0;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				}
				break;
			}
			if(UARTCtrl->RMFlag==0)//1=³­±í½øĞĞÖĞ,0=³­±í¼ä¸ôÔİÍ£
			{
				WarningOccur(PORTn,(u8 *)"ÔØ²¨·Ç³­±íÊ±¶Î");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
				if(Terminal_Router->RouterState==1)//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				{
					Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
					Terminal_Router->RouterState=0;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				}
				break;
			}
			if(Terminal_Router->RouterState==0)//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
			{
				Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				Terminal_Router->RouterState=1;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				break;
			}
		
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					Uart_3762_TxData(PORTn,0x10,4,0);//376.2·¢ËÍÊı¾İÌîÈë·¢ËÍÇø;Èë¿ÚÊ±ÈôÓĞĞÅÏ¢ÓòÄ¿µÄµØÖ·Êı¾İµ¥ÔªµÈÔòÒÑÌîÈëÇÒÊı¾İµ¥ÔªÔİ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
					Terminal_Router->RouterInitSubTask=1;
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ,6=×¼±¸·¢ËÍ
					{
						case 1://1=Íê³É1Ö¡·¢ÊÕ
							UARTCtrl->Task=0;//0=¿ÕÏĞ,ÆäËû×ª·¢µÈ²åÈë²Ù×÷
							i=p8rxbuff[13];
							if(i&1)
							{//B0Â·ÓÉÍê³É±êÖ¾:1ÎªÂ·ÓÉÑ§Ï°Íê³É,0ÎªÎ´Íê³É		
								Terminal_Router->RouterInitSubTask=0;//×ÓÈÎÎñ
								Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
							}
							else
							{
								if(Terminal_Ram->RouterCheck_S_Timer==0)
								{//×î´óÑ§Ï°Ê±¼äµ½
									Terminal_Router->RouterInitSubTask=0;//×ÓÈÎÎñ
									Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
									break;
								}
								if(Terminal_Router->AFN04F10==0)//AFN04F10ÉèÖÃ±ä»¯±êÖ¾0=Ã»,1=±ä»¯
								{
									Terminal_Router->RouterInitSubTask=0;//×ÓÈÎÎñ
								}
								else
								{
									Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
								}
							}
							break;
						case 0://0=¿ÕÏĞ
						case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
							Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
							break;
					}
					break;
				default:
					Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
					break;
			}
			break;
		case 103://Í£Ö¹Â·ÓÉ×éÍø,½øĞĞ³­±í
			Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
			Terminal_Router->RouterState=0;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
			Terminal_Router->RouterInitSubTask=0;//×ÓÈÎÎñ
			Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
		case 104:
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//ÏÂ´ÎÔØ²¨Â·ÓÉÆ÷¼ì²éÃë¶¨Ê±Öµ
			UARTCtrl->Task=0;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ,6=×¼±¸·¢ËÍ
			Terminal_Router->RouterInitTask=200;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
		
		
		case 200://>=200Â·ÓÉ³­±í
//			p32=(u32*)(ADDR_E4_ENTRY);
//			if((p32[64]==ADDR_E4_ENTRY)||(Terminal_Router->UpDateTask!=0))//Èí¼ş¸üĞÂÈÎÎñ
//			{
//				UpData_CX(PORTn);//Â·ÓÉÈí¼ş¸üĞÂ
//				break;
//			}

			if(UARTCtrl->Task==0)//0=¿ÕÏĞ
			{
				if(Terminal_Router->AFN04F10!=0)//AFN04F10ÉèÖÃ±ä»¯±êÖ¾0=Ã»,1=±ä»¯
				{
					Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
					break;
				}
				
				if(Terminal_Ram->RouterCheck_S_Timer==0)//24 ÔØ²¨Â·ÓÉÆ÷¼ì²éÃë¶¨Ê±Æ÷
				{
					Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//ÏÂ´ÎÔØ²¨Â·ÓÉÆ÷¼ì²éÃë¶¨Ê±Æ÷
					Terminal_Router->RouterCtrl=4;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
					break;
				}
				if(MEMReadReg(ADDR_AFN05F49+30,1)==0x55)//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÔİÍ£³­±í
				{
					WarningOccur(PORTn,(u8 *)"ÔØ²¨ÃüÁîÔİÍ£³­±í");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
					break;
				}
				i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//ÖÕ¶Ë³­±íÔËĞĞ²ÎÊıÉèÖÃ,¶Ë¿Ú31
				if((i&0x01)!=0x0)
				{//D0ÖÃ"1"²»ÔÊĞí×Ô¶¯³­±í£¬ÖÃ"0" ÒªÇóÖÕ¶Ë¸ù¾İ³­±íÊ±¶Î×Ô¶¯³­±í
					WarningOccur(PORTn,(u8 *)"ÔØ²¨²»ÔÊĞí×Ô¶¯³­±í");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
					break;
				}
				if(UARTCtrl->RMFlag==0)//1=³­±í½øĞĞÖĞ,0=³­±í¼ä¸ôÔİÍ£
				{
					WarningOccur(PORTn,(u8 *)"ÔØ²¨·Ç³­±íÊ±¶Î");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
					break;
				}
			}
			if(Terminal_Router->NoteNum==0)
			{//½Úµã±í¿Õ
				DisplayRouterVer(PORTn);//ÏÔÊ¾Â·ÓÉ³§¼ÒºÍ°æ±¾ºÅ
				break;
			}
			
			if(UARTCtrl->RMKeyUser==0)//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
			{
				GeneralReadMeter_3762_CX(PORTn);//ÔØ²¨Ä£¿é,³£¹æ³­±í
			}
			else
			{
				KeyUserReadMeter_3762_CX(PORTn);//ÔØ²¨Ä£¿é,ÖØµã»§³­±í
			}
			break;
		default:
			Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
	}
}


extern const u32 DL645_2007_RMLib[];
u32 Get_RM_DL645_2007_MaxFn(void);
extern const u32 DL645_1997_RMLib[];
u32 Get_RM_DL645_1997_MaxFn(void);

void GeneralReadMeter_3762_CX(u32 PORTn)//ÔØ²¨Ä£¿é,³£¹æ³­±í
{
	u32 i;
	u32 x;
	u32 y;
	u32 ClassNo;
	u32 ListNo;
	u32 Fn;
	u32 Pn;
	u32 Protocol;
	u32 AddrAFN04F10;
	u64 d64a;
	u64 d64b;
	
	u8 * p8RMLib;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	switch(UARTCtrl->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ,6=×¼±¸·¢ËÍ
	{
		case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			break;
		case 0://0=¿ÕÏĞ
			i=MEMReadReg(ADDR_YMDHMS+1,2);
			if((i>0x2350)||(i<0x0010))
			{//ÔÚÃ¿ÈÕ0µã¸½½üÔİÍ£³­±í
				UARTCtrl->Task=0;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}

			ListNo=UARTCtrl->GRMCount;//³­¶ÁµçÄÜ±í¼ÆÊı(Ïş³ÌºÍÕıÌ©ÔØ²¨)
			if(ListNo>=Terminal_Router->NoteNum)
			{//³­±í¼ÆÊı>=µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
				ListNo=0;
				UARTCtrl->GRMCount=0;
				break;
			}
			x=Terminal_Note->List[ListNo].MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
			if((x==0)||(x>AFN04F10MaxPn))
			{//ĞòºÅ´í
		RMOK:
				Terminal_Note->List[ListNo].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
				goto RMEND;
			}
			i=MEMReadReg(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//ÖÕ¶Ë³­±íÔËĞĞ²ÎÊıÉèÖÃ,¶Ë¿Ú31
			if((i&0x02)!=0x0)
			{//D1ÖÃ"1"ÒªÇóÖÕ¶ËÖ»³­ÖØµã±í£¬ÖÃ"0"ÒªÇóÖÕ¶Ë³­ËùÓĞ±í
				y=MEMReadReg(ADDR_AFN04F35,1);//Ì¨Çø¼¯ÖĞ³­±íÖØµã»§ÉèÖÃ
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				for(i=0;i<y;i++)
				{
					if(MEMReadReg(ADDR_AFN04F35+1+(2*i),2)==x)
					{
						break;
					}
				}
				if(i==y)
				{//²»ÊÇÖØµã»§
					goto RMOK;//²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
				}
			}
			x--;
			
			AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
			d64a=MEMReadReg(AddrAFN04F10+6,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
			Pn=MEMReadReg(AddrAFN04F10+2,2);//È¡ÅäÖÃµÄ²âÁ¿µãºÅ
			if(Pn==0)
			{//ÅäÖÃÉ¾³ı
				goto RMOK;//²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
			}
			i=MEMReadReg(ADDR_AFN04F30+(Pn-1),1);//Ì¨Çø¼¯ÖĞ³­±íÍ£³­/Í¶³­ÉèÖÃ(²âÁ¿µãºÅ)
			if(i!=0)
			{//Í£³­
				goto RMOK;//²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
			}
			if(Terminal_Note->List[ListNo].ReCount>=1)
			{//³­±íÖØ·¢´ÎÊı>=1
				Terminal_Note->List[ListNo].RMErr=1;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
				ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.Ê§°Ü
				goto RMEND;//½áÊø³­±í
			}
			Terminal_Note->List[ListNo].ReCount++;//³­±íÖØ·¢¼ÆÊı+1
			Protocol=MEMReadReg(AddrAFN04F10+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
			ClassNo=MEMReadReg(AddrAFN04F10+26,1);//È¡´óĞ¡ÀàºÅ
			
			while(1)
			{
				if(Protocol==1)
				{//DL/T645-1997
					i=Get_RM_DL645_1997_MaxFn();
				}
				else
				{//DL/T645-2007
					i=Get_RM_DL645_2007_MaxFn();
				}
				if(Terminal_Note->List[ListNo].FnCount>=i)
				{//Fn¼ÆÊı>=×î´óFn¸öÊı,±¾ÂÖ³­±í½áÊø
			RMEND:
					DisplayRouterVer(PORTn);//ÏÔÊ¾Â·ÓÉ³§¼ÒºÍ°æ±¾ºÅ
					Terminal_Note->List[ListNo].RMFlag=2;//³­±í±êÖ¾;0=ÉÏµç»òÈÕ¸üĞÂ,1=ÃüÁîÇ¿ÖÆÖØ³­,2=Íê³ÉÒ»ÂÖ³­±í
			 		Terminal_Note->List[ListNo].FnCount=0;//Fn¼ÆÊı
					Terminal_Note->List[ListNo].DICount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢´ÎÊı
					if(Terminal_Note->List[ListNo].RMErr==0)//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					{
						Terminal_Note->List[ListNo].OldRMErr=2;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
					}
					else
					{
						if(Terminal_Note->List[ListNo].OldRMErr<2)//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
						{
							Terminal_Note->List[ListNo].OldRMErr=1;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
						}
					}
					Terminal_Note->List[ListNo].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					UARTCtrl->GRMCount++;//³­¶ÁµçÄÜ±í¼ÆÊı(Ïş³ÌºÍÕıÌ©ÔØ²¨)
					return;
				}

				if(Protocol==1)
				{//DL/T645-1997
					i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
				}
				else
				{//DL/T645-2007
					i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
				}
				if(i==0)
				{//±¾FnÊÇ¿Õ
					goto NEXT_Fn;
				}
				p8RMLib=(u8 *)i;//³­±í¿â
				if((p8RMLib[3]&0x04)==0)//³­±íÖ§³Ö:B2=ÔØ²¨³­±í,B1=RS485³­±í,B0=ÄÚ²¿½»²É³­±í
				{
					goto NEXT_Fn;
				}
				
				
				y=0;
				if((p8RMLib[4]&0x01)!=0)//Êı¾İ·ÖÀà:B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
				{//1ÀàÊı¾İ,ÊµÊ±
					Fn=p8RMLib[0];//È¡ÊµÊ±Fn
					y=Class1Data_Fn(ClassNo,Fn);//1ÀàÊı¾İFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
					//i=GetClass1DataAddr(Fn,Pn);//È¡Àà1Êı¾İ´æÖüµØÖ·,·µ»Ø0±íÊ¾Ã»ÕÒµ½,ÓĞĞ§Êı¾İ³¤¶È·ÅADDR_Terminal_ClassDataLen
					//if(i!=0)
					//{
					//}
				}
				x=0;
				if((p8RMLib[4]&0x08)!=0)//Êı¾İ·ÖÀà:B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
				{//2ÀàÊı¾İB3=ÈÕ¶³½á
					Fn=p8RMLib[7];//È¡ÈÕ¶³½áFn
					i=GetClass2SourceDataAddr(Fn,Pn);//È¡Àà2Ô­Ê¼Êı¾İ´æÖüµØÖ·,·µ»Ø0±íÊ¾Ã»ÕÒµ½,ÓĞĞ§Êı¾İ³¤¶È·ÅADDR_Terminal_ClassDataLen
					if(i!=0)
					{
						MEMRead(ADDR_DATABUFF,i,5);//È¡Ô­³­±íÊ±¼äÄêÔÂÈÕÊ±·Ö
						x|=DataComp(ADDR_DATABUFF+2,ADDR_YMDHMS+3,3);//Êı¾İ±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
					}
				}
				if((p8RMLib[4]&0x20)!=0)//Êı¾İ·ÖÀà:B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
				{//2ÀàÊı¾İB5=ÔÂ¶³½á
					Fn=p8RMLib[9];//È¡ÔÂ¶³½áFn
					i=GetClass2SourceDataAddr(Fn,Pn);//È¡Àà2Ô­Ê¼Êı¾İ´æÖüµØÖ·,·µ»Ø0±íÊ¾Ã»ÕÒµ½,ÓĞĞ§Êı¾İ³¤¶È·ÅADDR_Terminal_ClassDataLen
					if(i!=0)
					{
						MEMRead(ADDR_DATABUFF,i,5);//È¡Ô­³­±íÊ±¼äÄêÔÂÈÕÊ±·Ö
						x|=DataComp(ADDR_DATABUFF+3,ADDR_YMDHMS+4,2);//Êı¾İ±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
					}
				}
				if(x==0)
				{//³­±íÊ±±êÏàÍ¬
					if(Terminal_Note->List[ListNo].RMFlag==1)//³­±í±êÖ¾;0=ÉÏµç»òÈÕ¸üĞÂ,1=ÃüÁîÇ¿ÖÆÖØ³­,2=Íê³É
					{
						y|=Class2Data_Fn(ClassNo,Fn);//2ÀàÊı¾İFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
					}
				}
				else
				{
					y|=Class2Data_Fn(ClassNo,Fn);//2ÀàÊı¾İFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
				}
				if(y==0x0)
				{//±¾Fn²»³­
		NEXT_Fn:
					Terminal_Note->List[ListNo].FnCount++;//Fn¼ÆÊı+1
					Terminal_Note->List[ListNo].DICount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					Terminal_Note->List[ListNo].ReCount=0;//ÖØ·¢¼ÆÊı
					continue;
				}
				if(p8RMLib[1]==0x0)
				{//±¾Fn³­±íÃüÁîÊı=0
					if((p8RMLib[4]&0x01)!=0)//Êı¾İ·ÖÀà:B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
					{//1ÀàÊı¾İ,ÊµÊ±
						Fn=p8RMLib[0];//È¡ÊµÊ±Fn
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
						p8FnData[0]=0x53;//Ç¿ÖÆ³­±í³É¹¦
						ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊı¾İ
						goto NEXT_Fn;
					}
				}
				if(Terminal_Note->List[ListNo].DICount==0)//³­±íÊı¾İ±êÊ¶¼ÆÊı
				{//³­±íÊı¾İ±êÊ¶¼ÆÊı=0,ÇåFnÊı¾İ»º³å
					MEMClr(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				if(Terminal_Note->List[ListNo].DICount>=p8RMLib[1])
				{//³­±íÊı¾İ±êÊ¶¼ÆÊı>=FnĞè·¢ÃüÁîÊı;±¾Fn³­±íÍê³É
					ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊı¾İ
					goto NEXT_Fn;
				}
				
				
				p8txbuff[0]=0x68;
				p8txbuff[3]=0x41;//¿ØÖÆÂëÎª0x41
				p8txbuff[4]=4;//Í¨ĞÅÄ£¿é±êÊ¶=1
				p8txbuff[5]=0;
				p8txbuff[6]=0;
				p8txbuff[7]=0;
				p8txbuff[8]=0;
				p8txbuff[9]=0;
				MEMClr(0,((u32)p8txbuff)+10,6);
				MEMRead(((u32)p8txbuff)+10,ADDR_TERMINALADDR,2);;//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)(Í¬ÖÕ¶ËµØÖ·)
				MEMWriteReg(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
				p8txbuff[22]=0x13;//AFN=13H
				p8txbuff[23]=1;//Fn=1
				p8txbuff[24]=0;
				if(Protocol==1)
				{//dl645-1997
					p8txbuff[25]=1;//2;//×ª·¢Êı¾İ±êÊ¶ÀàĞÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
					p8txbuff[26]=0;//¸½Êô½ÚµãÊı=0
					p8txbuff[27]=14;//DL645±¨ÎÄ³¤¶È
					p8txbuff[28]=0x68;
					MEMWriteReg(d64a,((u32)p8txbuff)+29,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
					p8txbuff[35]=0x68;
					p8txbuff[36]=0x01;
					p8txbuff[37]=2;
					p8txbuff[38]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)]+0x33;//È¡DIÍ¬Ê±¼Ó0x33
					p8txbuff[39]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+1]+0x33;
					p8txbuff[40]=0;
					p8txbuff[41]=0x16;
					for(i=0;i<12;i++)
					{
						p8txbuff[40]+=p8txbuff[28+i];
					}
				}
				else
				{//dl645-2007
					i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)];
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]<<8;
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]<<16;
					i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]<<24;
					switch(i)
					{//ÒÔÏÂÃüÁîÒòÖ¡Ì«³¤²»³­
						case 0x03300101://F27 ×î½üÒ»´Îµç±íÇåÁã·¢ÉúÊ±¿Ì
						case 0x03300201://F27 ×î½üÒ»´ÎĞèÁ¿ÇåÁã·¢ÉúÊ±¿Ì
						case 0x03300501://F166 ×î½üÒ»´ÎµçÄÜ±íÊ±¶Î²ÎÊıĞŞ¸ÄÊ±¼ä
//						case 0x03300001://(½öÕıÌ©)F27 ×î½üÒ»´Î±à³Ì·¢ÉúÊ±¿Ì
//						case 0x1E0002FF://(½öÕıÌ©)F161 ÉÏ1-10´ÎºÏÕ¢²Ù×÷Õß´úÂë
//						case 0x1E0001FF://(½öÕıÌ©)F161 ÉÏ1-10´ÎºÏÕ¢·¢ÉúÊ±¿Ì
//						case 0x1D0002FF://(½öÕıÌ©)F161 ÉÏ1-10´ÎÌøÕ¢²Ù×÷Õß´úÂë
//						case 0x1D0001FF://(½öÕıÌ©)F161 ÉÏ1-10´ÎÌøÕ¢·¢ÉúÊ±¿Ì
							Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢¼ÆÊı=0
							Terminal_Note->List[ListNo].DICount++;//³­±íÃüÁî¼ÆÊı
							return;
					}
					p8txbuff[25]=2;//2;//×ª·¢Êı¾İ±êÊ¶ÀàĞÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
					p8txbuff[26]=0;//¸½Êô½ÚµãÊı=0
					p8txbuff[27]=16;//DL645±¨ÎÄ³¤¶È
					p8txbuff[28]=0x68;
					MEMWriteReg(d64a,((u32)p8txbuff)+29,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
					p8txbuff[35]=0x68;
					p8txbuff[36]=0x11;
					p8txbuff[37]=4;
					p8txbuff[38]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)]+0x33;//È¡DIÍ¬Ê±¼Ó0x33
					p8txbuff[39]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]+0x33;
					p8txbuff[40]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]+0x33;
					p8txbuff[41]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]+0x33;
					p8txbuff[42]=0;
					p8txbuff[43]=0x16;
					for(i=0;i<14;i++)
					{
						p8txbuff[42]+=p8txbuff[28+i];
					}
				}
				i=28+p8txbuff[27]+2;//Ğè×Ü·¢ËÍ×Ö½ÚÊı
				p8txbuff[1]=i;
				p8txbuff[2]=i>>8;
				Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
				UARTCtrl->TxByte=i;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
				UARTCtrl->Task=3;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				UARTCtrl->RouterTimeOut=60000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)

				//ÏÔÊ¾³­±íµØÖ·
				CopyString((u8 *)"ÔØ²¨ÔÚ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
				for(i=0;i<6;i++)
				{
					x=p8txbuff[16+5-i];
					MEMWriteReg((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
					MEMWriteReg((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
				}
				MEMWriteReg(0,ADDR_DATABUFF+8+12,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
				return;
			}
			//break;
		case 1://1=Íê³É1Ö¡·¢ÊÕ
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			ListNo=UARTCtrl->GRMCount;//³­¶ÁµçÄÜ±í¼ÆÊı(Ïş³ÌºÍÕıÌ©ÔØ²¨)
			if(ListNo>=Terminal_Router->NoteNum)
			{//³­±í¼ÆÊı>=µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
				ListNo=0;
				UARTCtrl->GRMCount=0;
			}
			x=Terminal_Note->List[ListNo].MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
			if((x==0)||(x>AFN04F10MaxPn))
			{//ĞòºÅ´í
		rxerr:
				break;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if(Pn==0)
			{//²âÁ¿µãºÅ´í
				goto rxerr;
			}
			i=MEMReadReg(x+3,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//Í¨ĞÅĞ­Òé²»Öª
				goto rxerr;
			}
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
			}
			p8RMLib=(u8 *)i;//³­±í¿â
			
			d64a=MEMReadReg(x+4,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//·ñÈÏ»Ø´ğ
				goto rxerr;
//				Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢¼ÆÊı=0
//				Terminal_Note->List[ListNo].DICount++;//³­±íÊı¾İ±êÊ¶+1
//				break;
			}
			y=p8rxbuff[26];//·µ»ØÊı¾İÓò³¤¶È
			MEMRead((u32)p8rxbuff,((u32)p8rxbuff)+26+1,y);
			
			d64b=MEMReadReg(((u32)p8rxbuff)+1,6);//·µ»ØµÄµØÖ·
			if(d64a!=d64b)
			{//±íµØÖ·²»ÏàÍ¬
				goto rxerr;
			}
			x=0;//ºÍĞ£Ñé¼ÆËã
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//ºÍĞ£Ñé´í
				goto rxerr;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//´ÓÕ¾Òì³£»Ø´ğ,ÎŞ´ËÊı¾İÏî,Õı³£Ìø¹ı
				Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢¼ÆÊı=0
				Terminal_Note->List[ListNo].DICount++;//³­±íÊı¾İ±êÊ¶+1
				break;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//Êı¾İÓò¼õ0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//DL/T645-1997
				x=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)];
				x|=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+1]<<8;
				y=MEMReadReg(((u32)p8rxbuff)+10,2);
				if(x!=y)
				{//DI´íÎó
					goto rxerr;
				}
				i=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+2];
				if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
				{//·µ»Ø³¤¶ÈÏàÍ¬
					UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//¶ÔÓ¦µçÄÜ±íÅäÖÃĞòºÅ(°ë×Ö¶ÔÆë)(´Ó1¿ªÊ¼)
					UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
					UARTCtrl->RMCount=Terminal_Note->List[ListNo].DICount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					ReadMeter_DL645_1997_FnData(PORTn);//DL645_1997³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
					Terminal_Note->List[ListNo].DICount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
					ReadMeterNote_OK(PORTn,Pn);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
				}
				Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢¼ÆÊı=0
				Terminal_Note->List[ListNo].DICount++;//³­±íÊı¾İ±êÊ¶+1
			}
			else
			{//DL/T645-2007
				x=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)];
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]<<8;
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]<<16;
				x|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]<<24;
				y=MEMReadReg(((u32)p8rxbuff)+10,4);
				if(x!=y)
				{//DI´íÎó
					goto rxerr;
				}
				i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+4];
				if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
				{//·µ»Ø³¤¶ÈÏàÍ¬
					UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//¶ÔÓ¦µçÄÜ±íÅäÖÃĞòºÅ(°ë×Ö¶ÔÆë)(´Ó1¿ªÊ¼)
					UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
					UARTCtrl->RMCount=Terminal_Note->List[ListNo].DICount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					ReadMeter_DL645_2007_FnData(PORTn);//DL645_2007³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
					Terminal_Note->List[ListNo].DICount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
					ReadMeterNote_OK(PORTn,Pn);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
				}
				Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢¼ÆÊı=0
				Terminal_Note->List[ListNo].DICount++;//³­±íÊı¾İ±êÊ¶+1
			}
			
			break;
	}
}

void KeyUserReadMeter_3762_CX(u32 PORTn)//ÔØ²¨Ä£¿é,ÖØµã»§³­±í
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Pn;
	u64 d64a;
	u64 d64b;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	//p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	p8FnData=(u8 *)ADDR_DATABUFF;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterKeyUserTask)//ÖØµã»§³­¶ÁÈÎÎñ
	{
		case 0://ÔİÍ£Â·ÓÉ
			if(UARTCtrl->Task==0)
			{
				if(Terminal_Router->RouterStop==0)//ÔİÍ£Â·ÓÉ±êÊ¶:0=ÎŞ,1=³­ÖØµã»§,2=Êı¾İ×ª·¢,3=UART¶ÁÂ·ÓÉĞÅÏ¢,4=¹ã²¥Ğ£Ê±,5=¹ã²¥¶³½á,6=Í¨¶Ïµç¿ØÖÆ,7=...
				{
					Terminal_Router->RouterStop=1;//ÔİÍ£Â·ÓÉ±êÊ¶:0=ÎŞ,1=³­ÖØµã»§,2=Êı¾İ×ª·¢,3=UART¶ÁÂ·ÓÉĞÅÏ¢,4=¹ã²¥Ğ£Ê±,5=¹ã²¥¶³½á,6=Í¨¶Ïµç¿ØÖÆ,7=...
					Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				}
				Terminal_Router->RouterKeyUserTask++;//ÖØµã»§³­¶ÁÈÎÎñ
			}
			else
			{
				GeneralReadMeter_3762_CX(PORTn);//ÔØ²¨Ä£¿é,³£¹æ³­±í
			}
			break;
		case 1://AFN13 F1 Â·ÓÉÊı¾İ×ª·¢:¼à¿ØÔØ²¨´Ó½Úµã
			i=MEMReadReg(ADDR_AFN04F35,1);//Ì¨Çø¼¯ÖĞ³­±íÖØµã»§ÉèÖÃ
			if(i>MaxKeyUser)
			{
				i=MaxKeyUser;
			}
			if(UARTCtrl->KeyUserRMCount>=i)//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
			{//È«²¿ÖØµã»§³­Íê
				UARTCtrl->KeyUserRMCount=0;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				Terminal_Router->RouterKeyUserTask=0;//ÖØµã»§³­¶ÁÈÎÎñ
				UARTCtrl->KeyUserRMOKCountLast=UARTCtrl->KeyUserRMOKCount;//ÉÏ´ÎÖØµã»§³­¶Á³É¹¦¼ÆÊı
				UARTCtrl->KeyUserRMOKCount=0;//µ±Ç°ÖØµã»§³­¶Á³É¹¦¼ÆÊı
				if(Terminal_Router->RouterStop==1)//ÔİÍ£Â·ÓÉ±êÊ¶:0=ÎŞ,1=³­ÖØµã»§,2=Êı¾İ×ª·¢,3=UART¶ÁÂ·ÓÉĞÅÏ¢,4=¹ã²¥Ğ£Ê±,5=¹ã²¥¶³½á,6=Í¨¶Ïµç¿ØÖÆ,7=...
				{
					Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				}
				UARTCtrl->RMKeyUser=0;//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
				//Ğ¡Ê±¸üĞÂÔÚÖØµã»§³­ÍêºóÈôÓĞAFN04F10ÉèÖÃ±ä»¯Ôò³õÊ¼»¯Â·ÓÉÆ÷
				if(Terminal_Router->AFN04F10!=0)//AFN04F10ÉèÖÃ±ä»¯±êÖ¾0=Ã»,1=±ä»¯
				{
					Terminal_Router->AFN04F10=0;//AFN04F10ÉèÖÃ±ä»¯±êÖ¾0=Ã»,1=±ä»¯
					Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
				}
				return;
			}
			x=MEMReadReg(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//ÖØµã»§µÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ
			if((x==0)||(x>AFN04F10MaxPn))
			{//×°ÖÃĞòºÅ´í
				UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				break;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//²âÁ¿µãºÅ´í
				UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				break;
			}
			i=MEMReadReg(x+2,1);
			i&=0x1f;
			if(i!=31)
			{//¶Ë¿Ú²»ÊÇÔØ²¨
				UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				break;
			}
			i=MEMReadReg(x+3,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//Í¨ĞÅĞ­Òé²»Öª
				UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				break;
			}

			p8txbuff[0]=0x68;
			p8txbuff[3]=0x41;//¿ØÖÆÂëÎª0x41
			p8txbuff[4]=4;//Í¨ĞÅÄ£¿é±êÊ¶=1
			p8txbuff[5]=0;
			p8txbuff[6]=0;
			p8txbuff[7]=0;
			p8txbuff[8]=0;
			p8txbuff[9]=0;
			MEMClr(0,((u32)p8txbuff)+10,6);
			MEMRead(((u32)p8txbuff)+10,ADDR_TERMINALADDR,2);;//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)(Í¬ÖÕ¶ËµØÖ·)
			MEMRead(((u32)p8txbuff)+16,x+4,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
			p8txbuff[22]=0x13;//AFN=13H
			p8txbuff[23]=1;//Fn=1
			p8txbuff[24]=0;
			if(Protocol==1)
			{//dl645-1997
				p8txbuff[25]=1;//2;//×ª·¢Êı¾İ±êÊ¶ÀàĞÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
				p8txbuff[26]=0;//¸½Êô½ÚµãÊı=0
				p8txbuff[27]=14;//DL645±¨ÎÄ³¤¶È
				p8txbuff[28]=0x68;
				MEMRead(((u32)p8txbuff)+29,x+4,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
				p8txbuff[35]=0x68;
				p8txbuff[36]=0x01;
				p8txbuff[37]=2;
				p8txbuff[38]=0x0f+0x33;//DI=0x901f
				p8txbuff[39]=0x90+0x33;
				p8txbuff[40]=0;
				p8txbuff[41]=0x16;
				for(i=0;i<12;i++)
				{
					p8txbuff[40]+=p8txbuff[28+i];
				}
			}
			else
			{//dl645-2007
				p8txbuff[25]=2;//2;//×ª·¢Êı¾İ±êÊ¶ÀàĞÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
				p8txbuff[26]=0;//¸½Êô½ÚµãÊı=0
				p8txbuff[27]=16;//DL645±¨ÎÄ³¤¶È
				p8txbuff[28]=0x68;
				MEMRead(((u32)p8txbuff)+29,x+4,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
				p8txbuff[35]=0x68;
				p8txbuff[36]=0x11;
				p8txbuff[37]=4;
				p8txbuff[38]=0x00+0x33;//DI=0x0001FF00
				p8txbuff[39]=0x32;//0xff+0x33;
				p8txbuff[40]=0x01+0x33;
				p8txbuff[41]=0x00+0x33;
				p8txbuff[42]=0;
				p8txbuff[43]=0x16;
				for(i=0;i<14;i++)
				{
					p8txbuff[42]+=p8txbuff[28+i];
				}
			}
			i=28+p8txbuff[27]+2;//Ğè×Ü·¢ËÍ×Ö½ÚÊı
			p8txbuff[1]=i;
			p8txbuff[2]=i>>8;
			Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
			UARTCtrl->TxByte=i;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
			UARTCtrl->Task=3;
			UARTCtrl->RouterTimeOut=60000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
			//ÏÔÊ¾³­±íµØÖ·
			CopyString((u8 *)"³­ÖØµã»§",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[16+5-i];
				MEMWriteReg((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MEMWriteReg((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MEMWriteReg(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
			Terminal_Router->RouterKeyUserTask++;//ÖØµã»§³­¶ÁÈÎÎñ
			break;
		case 2://½ÓÊÕ
			if(UARTCtrl->Task>=3)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				break;
			}
			x=MEMReadReg(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//ÖØµã»§µÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ
			if((x==0)||(x>AFN04F10MaxPn))
			{//×°ÖÃĞòºÅ´í
				goto rxerr;
			}
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MEMReadReg(x,2);
			if((Pn==0)||(Pn>AFN0CF129KeyUserMaxPn))
			{//²âÁ¿µãºÅ´í
				goto rxerr;
			}
			i=MEMReadReg(x+3,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			Protocol=0;
			if(i==MEMReadReg(ADDR_Protocol_No+1,1))
			{//DL/T645-1997(1)
				Protocol=1;
			}
			if(i==MEMReadReg(ADDR_Protocol_No+2,1))
			{//DL/T645-2007(30)
				Protocol=2;
			}
			if(Protocol==0)
			{//Í¨ĞÅĞ­Òé²»Öª
				goto rxerr;
			}
			d64a=MEMReadReg(x+4,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
			MEMClr(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
			MEMRead(((u32)p8FnData)+1,ADDR_YMDHMS+1,5);//³­±íÊ±¼ä
			
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//·ñÈÏ»Ø´ğ
		rxerr:
				UARTCtrl->Task=0;//¿ÕÏĞ1´Î
				UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				Terminal_Router->RouterKeyUserTask--;//ÖØµã»§³­¶ÁÈÎÎñ
				break;
			}
			y=p8rxbuff[26];//·µ»ØÊı¾İÓò³¤¶È
			if(UARTCtrl->Task!=1)
			{
				y=0;//½ÓÊÕÊı¾İ³¤¶È
			}
			else
			{
				i=MEMReadReg(((u32)p8rxbuff)+22,3);
				if(i!=0x113)
				{//AFN=13H,Fn=1
					y=0;//½ÓÊÕÊı¾İ³¤¶È
				}
			}
			if(y==0)
			{//½ÓÊÕÊı¾İ³¤¶È=0
				goto rxerr;
			}
			MEMRead((u32)p8rxbuff,((u32)p8rxbuff)+26+1,y);
			
			d64b=MEMReadReg(((u32)p8rxbuff)+1,6);//·µ»ØµÄµØÖ·
			if(d64a!=d64b)
			{//±íµØÖ·²»ÏàÍ¬
				goto rxerr;
			}
			x=0;//ºÍĞ£Ñé¼ÆËã
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//ºÍĞ£Ñé´í
				goto rxerr;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//¿ØÖÆÂë´ÓÕ¾»Ø´ğÒì³£
				goto rxerr;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//Êı¾İÓò¼õ0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//1997
				if(p8rxbuff[9]<=2)
				{//ÎŞÊı¾İ
					goto rxerr;
				}
				i=MEMReadReg(((u32)p8rxbuff)+10,2);
				if(i!=0x901f)
				{
					goto rxerr;
				}
			}
			else
			{//2007
				if(p8rxbuff[9]<=4)
				{//ÎŞÊı¾İ
					goto rxerr;
				}
				i=MEMReadReg(((u32)p8rxbuff)+10,4);
				if(i!=0x0001FF00)
				{
					goto rxerr;
				}
			}
			for(i=0;i<p8rxbuff[9];i++)
			{
				if(p8rxbuff[10+i]==0xff)
				{
					p8rxbuff[10+i]=0;//µ¥Ïà±í¿éÎŞĞ§Êı¾İÌî0xff
				}
			}
			if(Protocol==1)
			{//1997
				i=p8rxbuff[9];
				if(i>=6)
				{
					i-=6;
					i/=4;
					if(i>MaxTariff)
					{
						i=MaxTariff;
					}
					p8FnData[6]=i;
					i+=1;
					p8FnData+=7;
					p8rxbuff+=12;
					for(;i!=0x0;i--)
					{
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						p8FnData+=5;
						p8rxbuff+=4;
					}
				}
			}
			else
			{//2007
				i=p8rxbuff[9];
				if(i>=8)
				{
					i-=8;
					i/=4;
					if(i>MaxTariff)
					{
						i=MaxTariff;
					}
					p8FnData[6]=i;
					i+=1;
					p8FnData+=7;
					p8rxbuff+=14;
					for(;i!=0x0;i--)
					{
						p8FnData[0]=0;
						p8FnData[1]=p8rxbuff[0];
						p8FnData[2]=p8rxbuff[1];
						p8FnData[3]=p8rxbuff[2];
						p8FnData[4]=p8rxbuff[3];
						p8FnData+=5;
						p8rxbuff+=4;
					}
				}
			}
			//SaveClass1Data(129,Pn,((u32)p8FnData)+1);//´æÖüÀà1Êı¾İ
			MEMWrite(((u32)p8FnData)+1,ADDR_AFN0CF129_KeyUser+((6+(5*(1+MaxTariff)))*(Pn-1)),6+(5*(1+MaxTariff)));
			
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			UARTCtrl->KeyUserRMOKCount++;//µ±Ç°ÖØµã»§³­¶Á³É¹¦¼ÆÊı
			UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
			Terminal_Router->RouterKeyUserTask--;//ÖØµã»§³­¶ÁÈÎÎñ
			break;
		default:
			Terminal_Router->RouterKeyUserTask=0;//ÖØµã»§³­¶Á³õÊ¼»¯ÈÎÎñ
			break;
	}	
}


void UpData_CX(u32 PORTn)//Ïş³¿Â·ÓÉÈí¼ş¸üĞÂ
{

}


#endif

//0x68
//×Ü³¤¶ÈL
//×Ü³¤¶ÈL
//¿ØÖÆÂë
//ĞÅÏ¢ÓòR-1
//ĞÅÏ¢ÓòR-2
//ĞÅÏ¢ÓòR-3
//ĞÅÏ¢ÓòR-4
//ĞÅÏ¢ÓòR-5
//ĞÅÏ¢ÓòR-6
//µØÖ·ÓòA1,A2,A3
//Ó¦ÓÃ¹¦ÄÜÂëAFN
//Êı¾İµ¥Ôª±êÊ¶DT1
//Êı¾İµ¥Ôª±êÊ¶DT2
//Êı¾İµ¥Ôª
//CS
//0x16






