

//¶«ÈíÔØ²¨Ä£¿é
//¼¼ÊõÖ§³Ö£ºÁÖ 13864805390

#include "AllHead.h"
#include "Terminal_ReadMeter_3762_EASTSOFT.h"
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

void Terminal_ReadMeter_3762_EASTSOFT(u32 PORTn)//¶«ÈíÔØ²¨Ä£¿é
{
	u32 i;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16timer;
	u32 * p32;
	
//	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	switch(Terminal_Router->RouterInitTask)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	{
		case 100://ÉèÖÃÂ·ÓÉÔËĞĞÄ£Ê½
			switch(Terminal_Router->RouterInitSubTask)
			{
				case 0:
					Terminal_Router->RouterInitReCount=0;//ÃüÁîÖØ·¢¼ÆÊı
					p8txbuff[128]=1;//1=±ê×¼Ä£Ê½ºÍ5=×ÔÊÊÓ¦Ä£Ê½¶«ÈíËµÄ¿Ç°ÏàÍ¬
					Uart_3762_TxData(PORTn,0x01,7,1);//376.2·¢ËÍÊı¾İÌîÈë·¢ËÍÇø;Èë¿ÚÊ±ÈôÓĞĞÅÏ¢ÓòÄ¿µÄµØÖ·Êı¾İµ¥ÔªµÈÔòÒÑÌîÈëÇÒÊı¾İµ¥ÔªÔİ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
					p8txbuff[3]=0x47;//¿ØÖÆÂë,Í¨ĞÅ·½Ê½=7
					Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
					Terminal_Router->RouterInitSubTask=1;
					break;
				case 1:
					switch(UARTCtrl->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
					{
						case 1://1=Íê³É1Ö¡·¢ÊÕ
							if(Get_Uart_3762_RxData_AFN_Fn(PORTn)==((0<<8)+1))//UART_3762½ÓÊÕÊı¾İµÃµ½AFN,Fn;B15-B8=AFN,B7-B0=Fn
							{
								UARTCtrl->Task=0;//0=¿ÕÏĞ,ÆäËû×ª·¢µÈ²åÈë²Ù×÷
								Terminal_Router->RouterInitReCount=0;//ÃüÁîÖØ·¢¼ÆÊı
								Terminal_Router->RouterInitSubTask=0;//×ÓÈÎÎñ
								Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
							}
							else
							{
								if(Terminal_Router->RouterInitReCount<2)
								{
									Terminal_Router->RouterInitReCount++;
									Uart_3762_TxData(PORTn,0x01,7,1);//376.2·¢ËÍÊı¾İÌîÈë·¢ËÍÇø;Èë¿ÚÊ±ÈôÓĞĞÅÏ¢ÓòÄ¿µÄµØÖ·Êı¾İµ¥ÔªµÈÔòÒÑÌîÈëÇÒÊı¾İµ¥ÔªÔİ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
									p8txbuff[3]=0x47;//¿ØÖÆÂë,Í¨ĞÅ·½Ê½=7
									Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
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
		case 101://"AFN=01H(À©Õ¹376.2Ğ­Òé)-F7ÉèÖÃÂ·ÓÉÔËĞĞÄ£Ê½¡±Ê±¼äÎª10Ãë
			p16timer[0]=12000/10;
			Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
		case 102:
			if(p16timer[0]==0)
			{
				Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			}
			break;
		case 103://ÖØÆôÂ·ÓÉ
			if(Terminal_Router->NoteNum!=0)//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı
			{
				Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				Terminal_Router->RouterState=1;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
			}
			Terminal_Router->RouterInitTask++;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
		case 104:
			//Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//ÏÂ´ÎÔØ²¨Â·ÓÉÆ÷¼ì²éÃë¶¨Ê±Öµ
			Terminal_Router->RouterInitTask=200;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			break;
		
		
		case 200://>=200Â·ÓÉ³­±í
			p32=(u32*)(ADDR_E4_ENTRY);
			if((p32[64]==ADDR_E4_ENTRY)||(Terminal_Router->UpDateTask!=0))//Èí¼ş¸üĞÂÈÎÎñ
			{
				UpData_EASTSOFT(PORTn);//Â·ÓÉÈí¼ş¸üĞÂ
				break;
			}

			if(UARTCtrl->Task==0)//0=¿ÕÏĞ
			{
				if(Terminal_Router->AFN04F10!=0)//AFN04F10ÉèÖÃ±ä»¯±êÖ¾0=Ã»,1=±ä»¯
				{
//					for(i=0;i<Terminal_Router->NoteNum;i++)
//					{
//						if(Terminal_Note->List[i].RMFlag!=2)//³­±í±êÖ¾;0=ÉÏµç»òÈÕ¸üĞÂ,1=ÃüÁîÇ¿ÖÆÖØ³­,2=Íê³É
//						{
//							break;
//						}
//					}
//					if(i==Terminal_Router->NoteNum)
//					{//Ô­ÒÑÖª½ÚµãÈ«²¿ÒÑ³­
//						Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
//						break;
//					}
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
			}
			if(Terminal_Router->NoteNum==0)
			{//½Úµã±í¿Õ
				DisplayRouterVer(PORTn);//ÏÔÊ¾Â·ÓÉ³§¼ÒºÍ°æ±¾ºÅ
				break;
			}
			if(Terminal_Router->RouterState==0)//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
			{
				Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				Terminal_Router->RouterState=1;//Â·ÓÉÆ÷µ±Ç°×´Ì¬0=ÔİÍ£,1=Æô¶¯
				break;
			}
			
			if(UARTCtrl->RMKeyUser==0)//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
			{
				GeneralReadMeter_3762_ES(PORTn);//ÔØ²¨Ä£¿é,³£¹æ³­±í
			}
			else
			{
				KeyUserReadMeter_3762_ES(PORTn);//ÔØ²¨Ä£¿é,ÖØµã»§³­±í
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

void GeneralReadMeter_3762_ES(u32 PORTn)//ÔØ²¨Ä£¿é,³£¹æ³­±í
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
	u64 d64c;
	
	u8 * p8RMLib;
	u32 * p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	
	switch(UARTCtrl->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
	{
		case 0://0=¿ÕÏĞ
			UARTCtrl->RxByte=0;//RX¼ÆÊı(ÒÑ½ÓÊÕ×Ö½ÚÊı)=0
			UARTCtrl->Task=5;//ÕıÔÚ½ÓÊÕ
			break;
		case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			break;
		case 1://1=Íê³É1Ö¡·¢ÊÕ
			i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762½ÓÊÕÊı¾İµÃµ½AFN,Fn;B15-B8=AFN,B7-B0=Fn
			switch(i)
			{
				case 0x1401://AFN=0x14,Fn=1;Â·ÓÉÆ÷·¢ËÍÇëÇó³­±í
					i=MEMReadReg(ADDR_YMDHMS+1,2);
					if((i>0x2350)||(i<0x0010))
					{//ÔÚÃ¿ÈÕ0µã¸½½üÔİÍ£³­±í
						UARTCtrl->Task=0;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
						break;
					}
					MEMRead((u32)p8txbuff,(u32)p8rxbuff,10);//¸´ÖÆ10×Ö½Ú½ÓÊÕÍ·µ½·¢ËÍÍ·
					d64a=MEMReadReg(((u32)p8rxbuff)+14,6);//È¡´Ó½ÚµãµØÖ·
					x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
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
					{//Ã»ÕÒµ½ÄÚ´æÖĞ´Ó½ÚµãÁĞ±í,Ö±½Ó·µ»Ø³­¶Á³É¹¦
				RMOK:
						Terminal_Note->List[ListNo].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
						goto RMEND;
					}
					x=Terminal_Note->List[ListNo].MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
					if((x==0)||(x>AFN04F10MaxPn))
					{//ĞòºÅ´í
						goto RMOK;//²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
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
					d64b=MEMReadReg(AddrAFN04F10+6,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
					if(d64a!=d64b)
					{//ÅäÖÃ±ä»¯
						goto RMOK;//²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
					}
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
							p8txbuff[3]=0x1;//¿ØÖÆÂëÎª0x1
							p8txbuff[4]=0x0;//Í¨ĞÅÄ£¿é±êÊ¶=0
							p8txbuff[10]=0x14;//AFN=0x14
							p8txbuff[11]=0x01;//Fn=1
							p8txbuff[12]=0x00;//Fn=1
							if(Terminal_Note->List[ListNo].RMErr==0)//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
							{
								p8txbuff[13]=1;//³­¶Á±êÖ¾=1³­¶Á³É¹¦
								Terminal_Note->List[ListNo].OldRMErr=2;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
							}
							else
							{
								p8txbuff[13]=0;//³­¶Á±êÖ¾=0³­¶ÁÊ§°Ü
								if(Terminal_Note->List[ListNo].OldRMErr<2)//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
								{
									Terminal_Note->List[ListNo].OldRMErr=1;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
								}
							}
							Terminal_Note->List[ListNo].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
							
							p8txbuff[14]=0;//DL645Ö¡³¤¶È=0
							p8txbuff[15]=0;//¸½Êô½ÚµãÊıÁ¿N=0
							x=16+2;//Ğè×Ü·¢ËÍ×Ö½ÚÊı
							p8txbuff[1]=x;
							p8txbuff[2]=x>>8;
							UARTCtrl->TxByte=x;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
							Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
							UARTCtrl->Task=3;//µÈ´ı·¢ËÍ			
							UARTCtrl->RouterTimeOut=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
							DisplayRouterVer(PORTn);//ÏÔÊ¾Â·ÓÉ³§¼ÒºÍ°æ±¾ºÅ
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
						{///³­±íÊı¾İ±êÊ¶¼ÆÊı=0,ÇåFnÊı¾İ»º³å
							MEMClr(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
						}
						if(Terminal_Note->List[ListNo].DICount>=p8RMLib[1])
						{//³­±íÊı¾İ±êÊ¶¼ÆÊı>=FnĞè·¢ÃüÁîÊı;±¾Fn³­±íÍê³É
							ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//´æÖü³­±íFnÊı¾İ
							goto NEXT_Fn;
						}
						
						if(Protocol==1)
						{//dl645-1997³­±í
							//È¡DIÍ¬Ê±¼Ó0x33
							p8txbuff[15+10+0]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)]+0x33;
							p8txbuff[15+10+1]=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+1]+0x33;
							//È¡Í¨ĞÅµØÖ·
							MEMRead(((u32)p8txbuff)+15+1,AddrAFN04F10+6,6);
							p8txbuff[15+0]=0x68;
							p8txbuff[15+7]=0x68;
							p8txbuff[15+8]=0x01;
							p8txbuff[15+9]=0x2;
							p8txbuff[15+13]=0x16;
							//CS¼ÆËã
							p8txbuff[15+12]=0;
							for(i=0;i<12;i++)
							{
								p8txbuff[15+12]+=p8txbuff[15+i];
							}
							//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
							i=p8RMLib[10+(3*Terminal_Note->List[ListNo].DICount)+2];//·µ»Ø³¤¶È
							if(i==0)
							{//·µ»Ø³¤¶È=0
								p8txbuff[6]=0;//Ä¬ÈÏÊ±¼ä
							}
							else
							{
								p8txbuff[6]=i+14;
							}
						}
						else
						{//dl645-2007³­±í
							i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)];
							i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]<<8;
							i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]<<16;
							i|=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]<<24;
							switch(i)
							{//ÒÔÏÂÃüÁîÒòÖ¡Ì«³¤²»³­
								case 0x03300101://F27 ×î½üÒ»´Îµç±íÇåÁã·¢ÉúÊ±¿Ì
								case 0x03300201://F27 ×î½üÒ»´ÎĞèÁ¿ÇåÁã·¢ÉúÊ±¿Ì
								case 0x03300501://F166 ×î½üÒ»´ÎµçÄÜ±íÊ±¶Î²ÎÊıĞŞ¸ÄÊ±¼ä
									Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢¼ÆÊı=0
									Terminal_Note->List[ListNo].DICount++;//³­±íÃüÁî¼ÆÊı
									return;
							}
							//È¡DIÍ¬Ê±¼Ó0x33
							p8txbuff[15+10+0]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)]+0x33;
							p8txbuff[15+10+1]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+1]+0x33;
							p8txbuff[15+10+2]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+2]+0x33;
							p8txbuff[15+10+3]=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+3]+0x33;
							//È¡Í¨ĞÅµØÖ·
							MEMRead(((u32)p8txbuff)+15+1,AddrAFN04F10+6,6);
							p8txbuff[15+0]=0x68;
							p8txbuff[15+7]=0x68;
							p8txbuff[15+8]=0x11;
							p8txbuff[15+9]=0x4;
							p8txbuff[15+15]=0x16;
							//CS¼ÆËã
							p8txbuff[15+14]=0;
							for(i=0;i<14;i++)
							{
								p8txbuff[15+14]+=p8txbuff[15+i];
							}
							//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
							i=p8RMLib[10+(5*Terminal_Note->List[ListNo].DICount)+4];//·µ»Ø³¤¶È
							if(i==0)
							{//·µ»Ø³¤¶È=0
								p8txbuff[6]=0;//Ä¬ÈÏÊ±¼ä
							}
							else
							{
								p8txbuff[6]=i+16;
							}
						}
						p8txbuff[3]=1;//¿ØÖÆÂëÎª0x1
						p8txbuff[4]=0;//Í¨ĞÅÄ£¿é±êÊ¶=0
						p8txbuff[10]=0x14;//AFN=0x14
						p8txbuff[11]=0x01;//Fn=1
						p8txbuff[12]=0x00;//Fn=1
						p8txbuff[13]=2;//³­¶Á±êÖ¾=2
						if(Protocol==1)
						{//dl645-1997³­±í
							i=14;//DL645Ö¡³¤¶È
						}
						else
						{//dl645-2007³­±í
							i=16;//DL645Ö¡³¤¶È
						}
						p8txbuff[14]=i;//DL645Ö¡³¤¶È
						p8txbuff[15+i]=0;//¸½Êô½ÚµãÊıÁ¿N=0
						i+=16+2;//Ğè×Ü·¢ËÍ×Ö½ÚÊı
						p8txbuff[1]=i;
						p8txbuff[2]=i>>8;
						UARTCtrl->TxByte=i;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
						Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
						UARTCtrl->Task=3;//µÈ´ı·¢ËÍ
						UARTCtrl->RouterTimeOut=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
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
				case 0x0602://AFN=0x06,Fn=2;Â·ÓÉÆ÷·¢ËÍÉÏ±¨³­±íÊı¾İ
					d64c=MEMReadReg(((u32)p8rxbuff)+4,6);//È¡6×Ö½Ú376.2·µ»ØµÄĞÅÏ¢Óò
					i=p8rxbuff[16];//¶ÁDL645Ö¡³¤¶È
					MEMRead(((u32)p8rxbuff),((u32)p8rxbuff)+17,i);//È¡645Ö¡
					d64a=MEMReadReg(((u32)p8rxbuff)+1,6);//Ö¡·µ»Ø±íµØÖ·
					x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
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
						goto RXESC;//ÉÏ±¨·µ»Ø				
					}
					MEMWriteReg(d64c,(u32)&Terminal_Note->List[ListNo].Info,6);//6×Ö½Ú×îºó´ÎÔØ²¨³­±í·µ»ØµÄĞÅÏ¢Óò
					
					x=Terminal_Note->List[ListNo].MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
					if((x==0)||(x>AFN04F10MaxPn))
					{//ĞòºÅ´í
						goto RXESC;//ÉÏ±¨·µ»Ø	
					}
					x--;
					AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
					Pn=MEMReadReg(AddrAFN04F10+2,2);//È¡ÅäÖÃµÄ²âÁ¿µãºÅ
					if(Pn==0)
					{//ÅäÖÃÉ¾³ı
						goto RXESC;//ÉÏ±¨·µ»Ø	
					}
					d64b=MEMReadReg(AddrAFN04F10+6,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
					if(d64a!=d64b)
					{//ÅäÖÃ±ä»¯
						goto RXESC;//ÉÏ±¨·µ»Ø	
					}
					
					Protocol=MEMReadReg(AddrAFN04F10+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
					if(Protocol==1)
					{//DL/T645-1997
						i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
					}
					else
					{//DL/T645-2007
						i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
					}
					p8RMLib=(u8 *)i;//³­±í¿â
					
					if((p8rxbuff[8]&0x40)!=0x0)
					{//´ÓÕ¾Òì³£»Ø´ğ,ÎŞ´ËÊı¾İÏî,Õı³£Ìø¹ı
						Terminal_Note->List[ListNo].ReCount=0;//³­±íÖØ·¢¼ÆÊı=0
						Terminal_Note->List[ListNo].DICount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
						goto RXESC;//ÉÏ±¨·µ»Ø
					}
					x=0;//ºÍĞ£Ñé¼ÆËã
					for(i=0;i<(p8rxbuff[9]+10);i++)
					{
						x+=p8rxbuff[i];
					}
					x&=0xff;
					if(x!=p8rxbuff[i])
					{//ºÍĞ£Ñé´íÎó
						goto RXESC;//ÉÏ±¨·µ»Ø
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
							goto RXESC;//ÉÏ±¨·µ»Ø
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
							goto RXESC;//ÉÏ±¨·µ»Ø
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
					
					//·¢È·ÈÏ»Ø´ğÉÏ±¨³­±íÊı¾İ
				RXESC:
					MEMWriteReg(0xffff,((u32)p8txbuff)+128,4);
					Uart_3762_TxData(PORTn,0x00,1,4);//376.2·¢ËÍÊı¾İÌîÈë·¢ËÍÇø,Èë¿ÚÊ±ÈôÓĞĞÅÏ¢ÓòÄ¿µÄµØÖ·Êı¾İµ¥ÔªµÈÔòÒÑÌîÈë,Êı¾İµ¥ÔªÔİ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
					p8txbuff[3]=1;//¿ØÖÆÂëÎª0x1
					Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
					break;
				default:
					UARTCtrl->RxByte=0;//½ÓÊÕ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
					UARTCtrl->Task=5;//ÕıÔÚ½ÓÊÕ
					break;
			}
			break;
	}
}

void KeyUserReadMeter_3762_ES(u32 PORTn)//ÔØ²¨Ä£¿é,ÖØµã»§³­±í
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
				GeneralReadMeter_3762_ES(PORTn);//ÔØ²¨Ä£¿é,³£¹æ³­±í
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


void UpData_EASTSOFT(u32 PORTn)//¶«ÈíÂ·ÓÉÈí¼ş¸üĞÂ
{
	u32 i;
	u32 x;
	u32 y;
	u8 *p8;
	u32 *p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->UpDateTask)//Èí¼ş¸üĞÂÈÎÎñ
	{
		case 0://½øÈë
			p32=(u32*)(ADDR_E4_ENTRY);
			i=p32[68];//°æ±¾ĞÅÏ¢
			p32=(u32*)i;
			if(p32[5]!=Terminal_Router->RouterType)//Â·ÓÉÆ÷ÀàĞÍ:0=±£Áô,1=ÕıÌ©,2=¶¦ĞÅ,3=¶«Èí,4=Ïş³¿,5=
			{//Â·ÓÉÆ÷³§¼Ò´í
				MEMClr(0,ADDR_E4_ENTRY,4096);
				break;
			}
			if(UARTCtrl->Task<=2)
			{//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				Terminal_Router->UpDateCount=0;//Èí¼ş¸üĞÂÖ¡¼ÆÊı
				Terminal_Router->UpDateReCount=0;//Èí¼ş¸üĞÂÖØÊÔ´ÎÊı
				Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
				WarningOccur(PORTn,(u8 *)"ÕıÔÚ¸üĞÂÔØ²¨Â·ÓÉÈí¼ş");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
			}
			break;
		case 1://ÔİÍ£Â·ÓÉÆ÷
			Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
			Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
			break;
		case 2://Æô¶¯Â·ÓÉÉı¼¶
			p8txbuff[0]=0x68;
			p8txbuff[1]=21;
			p8txbuff[2]=0;
			p8txbuff[3]=0x47;//¿ØÖÆÂëÎª0x47
			p8txbuff[4]=0;//Í¨ĞÅÄ£¿é±êÊ¶=0
			p8txbuff[5]=0;
			p8txbuff[6]=0;
			p8txbuff[7]=0;
			p8txbuff[8]=0;
			p8txbuff[9]=0;
			p8txbuff[10]=0x1;//AFN=01H
			p8txbuff[11]=1;//Fn=1
			p8txbuff[12]=0;
			//¿ØÖÆ×Ö	BIN	1
			p8txbuff[13]=1;//1=Æô¶¯Éı¼¶
			//Éı¼¶³ÌĞò°æ±¾ºÅ	BCD	2
			p32=(u32*)(ADDR_E4_ENTRY);
			i=p32[62];
			p8=(u8*)i;
			p8txbuff[14]=p8[16];
			p8txbuff[15]=p8[17];
			//²ğ·ÖÊı¾İ¿éµÄ¸öÊı	BIN	2
			p32=(u32*)(ADDR_E4_ENTRY);
			x=p32[63]-p32[62];
			x-=64;//Ğè·¢ËÍ×Ü×Ö½ÚÊı
			x+=199;
			i=x/200;
			p8txbuff[16]=i;
			p8txbuff[17]=i>>8;
			//Ã¿¸öÊı¾İ¿éµÄ´óĞ¡L	BIN	1
			p8txbuff[18]=200;
			Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
			UARTCtrl->Task=3;//µÈ´ı·¢ËÍ
			UARTCtrl->TxByte=21;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
			UARTCtrl->RouterTimeOut=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
			Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
			break;
		case 3://Æô¶¯½ÓÊÕ
			if(UARTCtrl->Task>=3)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				case 1://1=Íê³É1Ö¡·¢ÊÕ
					if(p8rxbuff[13+3]==0)
					{//Éı¼¶±êÊ¶£º00H¿ÉÒÔÉı¼¶£¬FFHÎŞĞèÉı¼¶£¬01HÏÂĞĞ±¨ÎÄÊı¾İµ¥ÔªÄÚÈİ´íÎó
						Terminal_Router->UpDateReCount=0;//Èí¼ş¸üĞÂÖØÊÔ´ÎÊı
						Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
						break;
					}
				case 0://0=¿ÕÏĞ
				case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
					if(Terminal_Router->UpDateReCount<3)//Èí¼ş¸üĞÂÖØÊÔ´ÎÊı
					{
						Terminal_Router->UpDateReCount++;//Èí¼ş¸üĞÂÖØÊÔ´ÎÊı
						Terminal_Router->UpDateTask--;//Èí¼ş¸üĞÂÈÎÎñ
					}
					else
					{//Æô¶¯Ê§°Ü
						MEMClr(0,ADDR_E4_ENTRY,4096);
						Terminal_Router->UpDateTask=0;//Èí¼ş¸üĞÂÈÎÎñ
						Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³õÊ¼»¯ÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
					}
					break;
			}
			break;
		case 4://´«Êı¾İ
			p8txbuff[0]=0x68;
			p8txbuff[1]=222;
			p8txbuff[2]=0;
			p8txbuff[3]=0x47;//¿ØÖÆÂëÎª0x47
			p8txbuff[4]=0;//Í¨ĞÅÄ£¿é±êÊ¶=0
			p8txbuff[5]=0;
			p8txbuff[6]=0;
			p8txbuff[7]=0;
			p8txbuff[8]=0;
			p8txbuff[9]=0;
			p8txbuff[10]=0x1;//AFN=01H
			p8txbuff[11]=1;//Fn=1
			p8txbuff[12]=0;
			//¿ØÖÆ×Ö	BIN	1
			p8txbuff[13]=2;//02HÉı¼¶ÄÚÈİ´«Êä
			//Êı¾İ¿éĞòºÅ	BIN	2
			i=Terminal_Router->UpDateCount;//Èí¼ş¸üĞÂÖ¡¼ÆÊı
			i++;
			p8txbuff[14]=i;
			p8txbuff[15]=i>>8;
			//Éı¼¶³ÌĞò°æ±¾ºÅ	BCD	2
			p32=(u32*)(ADDR_E4_ENTRY);
			i=p32[62];
			p8=(u8*)i;
			p8txbuff[16]=p8[16];
			p8txbuff[17]=p8[17];
			//Êı¾İ¿é´óĞ¡L	BIN	1
			p8txbuff[18]=200;
			//Êı¾İ¿éÄÚÈİ	BIN	L
			p32=(u32*)(ADDR_E4_ENTRY);
			x=p32[63]-p32[62];
			x-=64;//Ğè·¢ËÍ×Ü×Ö½ÚÊı
			y=Terminal_Router->UpDateCount;//Èí¼ş¸üĞÂÖ¡¼ÆÊı
			y*=200;//ÒÑ·¢ËÍ×Ö½ÚÊı
			if(x<=y)
			{//·¢Íê
				Terminal_Router->UpDateReCount=0;//Èí¼ş¸üĞÂÖØÊÔ´ÎÊı
				Terminal_Router->UpDateTask+=2;//Èí¼ş¸üĞÂÈÎÎñ
				p16timer[0]=5000/10;//ÑÓÊ±5Ãëºó·¢²éÑ¯
				break;
			}
			x-=y;
			if(x<200)
			{
				MEMClr(0,((u32)p8txbuff)+19,200);//²»¹»200×Ö½ÚÓÃ0²¹
			}
			else
			{
				x=200;
			}
			MEMRead(((u32)p8txbuff)+19,p32[62]+64+y,x);
			Terminal_Router->UpDateCount++;//Èí¼ş¸üĞÂÖ¡¼ÆÊı
			//±êÊ¶·û	BIN	1
			p8txbuff[219]=0;
			Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
			UARTCtrl->Task=3;//µÈ´ı·¢ËÍ
			UARTCtrl->TxByte=222;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
			UARTCtrl->RouterTimeOut=20/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
			Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
			break;
		case 5://´«Êı¾İ½ÓÊÕ
			if(UARTCtrl->Task>=3)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				break;
			}
			Terminal_Router->UpDateTask--;//Èí¼ş¸üĞÂÈÎÎñ
			break;
		case 6://·¢²éÑ¯Î´Íê³ÉÊı¾İ¿éĞÅÏ¢
			if(p16timer[0]!=0)//ÑÓÊ±5ÃëºóÖØ·¢
			{
				break;
			}
			p8txbuff[0]=0x68;
			p8txbuff[1]=18;
			p8txbuff[2]=0;
			p8txbuff[3]=0x47;//¿ØÖÆÂëÎª0x47
			p8txbuff[4]=0;//Í¨ĞÅÄ£¿é±êÊ¶=0
			p8txbuff[5]=0;
			p8txbuff[6]=0;
			p8txbuff[7]=0;
			p8txbuff[8]=0;
			p8txbuff[9]=0;
			p8txbuff[10]=0x1;//AFN=01H
			p8txbuff[11]=1;//Fn=1
			p8txbuff[12]=0;
			//¿ØÖÆ×Ö	BIN	1
			p8txbuff[13]=3;//03H²éÑ¯´«ÊäÎ´Íê³ÉÊı¾İ¿éĞÅÏ¢
			//Éı¼¶³ÌĞò°æ±¾ºÅ	BCD	2
			p32=(u32*)(ADDR_E4_ENTRY);
			i=p32[62];
			p8=(u8*)i;
			p8txbuff[14]=p8[16];
			p8txbuff[15]=p8[17];
			Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
			UARTCtrl->Task=3;//µÈ´ı·¢ËÍ
			UARTCtrl->TxByte=18;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
			UARTCtrl->RouterTimeOut=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
			Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
			break;
		case 7://²éÑ¯½ÓÊÕ
			if(UARTCtrl->Task>=3)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				break;
			}
			switch(UARTCtrl->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				case 1://1=Íê³É1Ö¡·¢ÊÕ
					if(p8rxbuff[13+1]==1)
					{//Éı¼¶×´Ì¬£º01HÉı¼¶³É¹¦
						Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
						WarningOccur(PORTn,(u8 *)"ÔØ²¨Â·ÓÉÈí¼ş¸üĞÂOK");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
						break;
					}
				case 0://0=¿ÕÏĞ
				case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
					if(Terminal_Router->UpDateReCount<5)//Èí¼ş¸üĞÂÖØÊÔ´ÎÊı
					{
						Terminal_Router->UpDateReCount++;//Èí¼ş¸üĞÂÖØÊÔ´ÎÊı
						Terminal_Router->UpDateTask--;//Èí¼ş¸üĞÂÈÎÎñ
						p16timer[0]=5000/10;//ÑÓÊ±5ÃëºóÖØ·¢
					}
					else
					{//²éÑ¯Ê§°Ü
						Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
						WarningOccur(PORTn,(u8 *)"ÔØ²¨Â·ÓÉÈí¼ş¸üĞÂÊ§°Ü");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
					}
					break;
			}
			break;
		case 8://Íê³É
			p16timer[0]=10000/10;
			Terminal_Router->UpDateTask++;//Èí¼ş¸üĞÂÈÎÎñ
			break;
		case 9://ÑÓÊ±10ºó¸´Î»
			if(p16timer[0]==0)
			{
				MEMClr(0,ADDR_E4_ENTRY,4096);
				Terminal_Router->UpDateTask=0;//Èí¼ş¸üĞÂÈÎÎñ
				Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³õÊ¼»¯ÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			}
			break;
	}
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


