
//¶¦ĞÅ¼¼ÊõÖ§³Ö:Ğì½£Ó¢	13625322351
//             ÀîÓÂ 18661440042

//¶«ÈíÂ·ÓÉÎÊÌâÁªÏµÈË£ºÎâ³£Óñ£¬ÊÖ»ú13589207445,µç»°0532-83676919,ÓÊÏäwucy@eastsoft.com.cn

//ÈğË¹¿µ15989482340 ºé°®Ï¼



#include "Project.h"
#include "Terminal_ReadMeter_3762_Std.h"
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



void Uart_3762_AFN14F1_Tx(u32 PORTn,u32 RMFlag)//Â·ÓÉÆ÷ÇëÇó³­±í·µ»ØÖ¡;Èë¿Ú:RMFlag³­¶Á±êÖ¾=0³­¶ÁÊ§°Ü,=1³­¶Á³É¹¦,2=³­¶Á
{
	u32 x;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(Terminal_Router->RouterRunMode_1==2)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
	{
		p8txbuff[3]=0x03;//¿ØÖÆÂë;¼¯ÖĞÊ½Â·ÓÉÔØ²¨Í¨ĞÅ
	}
	else
	{
		if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
		{
			p8txbuff[3]=0x0a;//¿ØÖÆÂë;Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ
		}
		else
		{
			p8txbuff[3]=0x01;//¿ØÖÆÂë;¼¯ÖĞÊ½Â·ÓÉÔØ²¨Í¨ĞÅ
		}
	}
	p8txbuff[4]=4;//Í¨ĞÅÄ£¿é±êÊ¶=1
	MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
	MR(((u32)p8txbuff)+16,((u32)p8rxbuff)+14,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
	p8txbuff[22]=0x14;//AFN=0x14
	p8txbuff[23]=0x01;//Fn=1
	p8txbuff[24]=0x00;//Fn=1
	p8txbuff[25]=RMFlag;//³­¶Á±êÖ¾=0³­¶ÁÊ§°Ü,=1³­¶Á³É¹¦,2=³­¶Á
	p8txbuff[26]=0;//Í¨ĞÅÑÓÊ±Ïà¹ØĞÔ±êÖ¾,0=ÎŞ¹Ø,1=ÓĞ¹Ø
	p8txbuff[27]=0;//DL645Ö¡³¤¶È=0
	p8txbuff[28]=0;//¸½Êô½ÚµãÊıÁ¿N=0
	x=29+2;//Ğè×Ü·¢ËÍ×Ö½ÚÊı
	p8txbuff[1]=x;
	p8txbuff[2]=x>>8;
	UARTCtrl->TxByte=x;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
	/*
	switch(Comm_Ram->RouterType)//Â·ÓÉÆ÷ÀàĞÍ:0=±£Áô,1=ÕıÌ©,2=¶¦ĞÅ,3=¶«Èí,4=Ïş³¿,5=
	{
		default://ÆäËû
		case 2://¶¦ĞÅ
			break;
		case 3://¶«Èí
			p8txbuff[4]=0x0;//Í¨ĞÅÄ£¿é±êÊ¶=0
			MR(((u32)p8txbuff)+10,((u32)p8txbuff)+22,7);
			x=17+2;//Ğè×Ü·¢ËÍ×Ö½ÚÊı
			p8txbuff[1]=x;
			p8txbuff[2]=x>>8;
			UARTCtrl->TxByte=x;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
			break;
	}
	*/
	Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
	UARTCtrl->Task=3;//µÈ´ı·¢ËÍ			
	UARTCtrl->TxToRxTimerOver=0;//1000/10//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµ=0,Ê¹ÄÜºÜ¿ì³¬Ê±¶ø×ª¿ÕÏĞÒÔ±ãÊı¾İ×ª·¢
}


void GeneralReadMeter_3762_Std(u32 PORTn)//±ê×¼ÔØ²¨Ä£¿é,³£¹æ³­±í
{
	u32 i;
	u32 x;
	u32 y;
#if RMM_DateCongeal_0xEE==0//2ÀàÊı¾İÈÕ¶³½áÃ»³­µ½µç±íÈÕ¶³½áÊı¾İÇ°Ìî0xff;0=·ñ(ÓÃÊµÊ±Êı¾İ¶³½á),1=ÊÇ
	u32 z;
#endif
	u32 ClassNo;
	u32 ListNo;
	u32 Fn;
	u32 Pn;
	u32 MaxFnCount;
	u32 Protocol;
	u32 AddrAFN04F10;
	u64 d64a;
	u64 d64b;
	u32 Info;
	
	u8 * p8RMLib;
	u32 * p32;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
	u32 AutoReportSearch;//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
#endif
#endif

#if ((USER/100)==5)||((USER/100)==8)||((USER/100)==7)//ÉÏº£¡¢Í¨ÓÃ¡¢Õã½­
	u8 *p8;
#endif

	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	
	switch(UARTCtrl->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
	{
		case 0://0=¿ÕÏĞ
			UARTCtrl->RxByte=0;//RX¼ÆÊı(ÒÑ½ÓÊÕ×Ö½ÚÊı)=0
			UARTCtrl->Task=5;//ÕıÔÚ½ÓÊÕ
			break;
		case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
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
			if((i<RMM_EndHM)&&(i>=x))
			{//²»ÊÇÔÚÃ¿ÈÕ0µã¸½½üÔİÍ£³­±í
				if((Terminal_Router->StopFlags&(1<<14))!=0)//ÔİÍ£Â·ÓÉ±êÖ¾:b14=µÈ´ıÊ±ÖÓ¹ı0
				{
					Terminal_Router->StopFlags&=~(1<<14);//ÔİÍ£Â·ÓÉ±êÖ¾:b14=µÈ´ıÊ±ÖÓ¹ı0
					Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
				}
			}
		#else
			if((i<RMM_EndHM)&&(i>=RMM_StartHM))
			{//²»ÊÇÔÚÃ¿ÈÕ0µã¸½½üÔİÍ£³­±í
				if((Terminal_Router->StopFlags&(1<<14))!=0)//ÔİÍ£Â·ÓÉ±êÖ¾:b14=µÈ´ıÊ±ÖÓ¹ı0
				{
					Terminal_Router->StopFlags&=~(1<<14);//ÔİÍ£Â·ÓÉ±êÖ¾:b14=µÈ´ıÊ±ÖÓ¹ı0
					Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
				}
			}
		#endif
			if(Terminal_Ram->RouterCheck_S_Timer==0)//24 ÔØ²¨Â·ÓÉÆ÷ËÀ»úÃë¶¨Ê±Æ÷
			{
				Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			}
			break;
		case 1://1=Íê³É1Ö¡·¢ÊÕ
			Terminal_Ram->RouterCheck_S_Timer=RouterCheckTimerVal;//(60*10)//ÔØ²¨Â·ÓÉÆ÷ËÀ»úÃë¶¨Ê±Öµ
			i=p8rxbuff[4];//¶ÁÍ¨ĞÅÄ£¿é±êÊ¶
			i&=0x4;
			if(i==0x0)
			{//¼¯ÖĞÆ÷Í¨ĞÅÄ£¿é
				i=Get_Uart_3762_RxData_AFN_Fn(PORTn);//UART_3762½ÓÊÕÊı¾İµÃµ½AFN,Fn;B15-B8=AFN,B7-B0=Fn
				switch(i)
				{
					case 0x1401://AFN=0x14,Fn=1;Â·ÓÉÆ÷·¢ËÍÇëÇó³­±í
						if(UARTCtrl->RMTaskC!=0)//µ±Ç°³­±íÈÎÎñºÅ:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=¶¨Ê±ÈÎÎñ,(MaxRMTask-1)=Ñ­³­ÈÎÎñ(Ö»ÓĞÑ­³­ÈÎÎñ²Å¿É²åÈë¸ßÓÅÏÈ¼¶³­±í)
						{//²»ÊÇÆô³­
							i=MRR(ADDR_TYMDHMS+1,2);
							if((i>=RMM_EndHM)||(i<RMM_StartHM))
							{//ÔÚÃ¿ÈÕ0µã¸½½üÔİÍ£³­±í
								UARTCtrl->Task=0;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
								WarningOccur(PORTn,(u8 *)"ÔØ²¨Í£³­µÈ´ıÊ±ÖÓ¹ı0");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
								if(Terminal_Router->StopFlags==0)//ÔİÍ£Â·ÓÉ±êÖ¾:b14=µÈ´ıÊ±ÖÓ¹ı0
								{
									Terminal_Router->StopFlags|=(1<<14);//ÔİÍ£Â·ÓÉ±êÖ¾:b14=µÈ´ıÊ±ÖÓ¹ı0
									Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
								}
								return;
							}
						}
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//¸´ÖÆ10×Ö½Ú½ÓÊÕÍ·µ½·¢ËÍÍ·
						d64a=MRR(((u32)p8rxbuff)+14,6);//È¡´Ó½ÚµãµØÖ·
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
						{//Ã»ÕÒµ½ÄÚ´æÖĞ´Ó½ÚµãÁĞ±í,Ö±½Ó·µ»Ø³­¶Á³É¹¦
							Uart_3762_AFN14F1_Tx(PORTn,1);//Â·ÓÉÆ÷ÇëÇó³­±í·µ»ØÖ¡;Èë¿Ú:RMFlag³­¶Á±êÖ¾=0³­¶ÁÊ§°Ü,=1³­¶Á³É¹¦,2=³­¶Á
							return;
						}
						if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]>=2)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
						{//±¾ÈÎÎñ¼º³É¹¦³­±í,Ö±½Ó·µ»Ø³­¶Á³É¹¦
							Uart_3762_AFN14F1_Tx(PORTn,1);//Â·ÓÉÆ÷ÇëÇó³­±í·µ»ØÖ¡;Èë¿Ú:RMFlag³­¶Á±êÖ¾=0³­¶ÁÊ§°Ü,=1³­¶Á³É¹¦,2=³­¶Á
							if(Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]==2)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
							{
								CopyString((u8 *)"ÔØ²¨³É¹¦",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
							}
							else
							{
								CopyString((u8 *)"ÔØ²¨²»³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
							}
						//dispskip:
							d64a<<=16;
							for(i=0;i<6;i++)
							{
								x=d64a>>56;
								d64a<<=8;
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
							return;
						}
						p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
						x=Terminal_Note->List[ListNo].MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
						if((x==0)||(x>AFN04F10MaxPn))
						{//ĞòºÅ´í,²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
					#if (USER/100)!=0
						RMOK:
					#endif
							Terminal_Note->List[ListNo].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
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
								goto RMOK;//²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
							}
						}
					#endif
						x--;
						AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
						d64b=MRR(AddrAFN04F10+6,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
						if(d64a!=d64b)
						{//ÅäÖÃ±ä»¯
						RouterSyn:
							if(Terminal_Ram->RouterSynDelay_S_Timer==0)//AFN04F10ÉèÖÃ±ä»¯±êÖ¾0=Ã»,1=±ä»¯;ÓÃÓÚµµ°¸Í¬²½
							{//É¾³ıÊ±²»ÄÜÂíÉÏÍ¬²½,Òòµç¿ÆÔº»¥»»ĞÔ²âÊÔÈí¼şÔ¼ÓĞ30ÃëÉèÖÃÊ±¼ä²»ÏìÓ¦
								Terminal_Router->AFN04F10=0;//AFN04F10ÉèÖÃ±ä»¯±êÖ¾0=Ã»,1=±ä»¯;ÓÃÓÚµµ°¸Í¬²½
								Terminal_Router->RouterSynTask=1;//µµ°¸Í¬²½ÈÎÎñ;0=¿ÕÏĞ,1=µÈ´ıÆô¶¯,2-nÍ¬²½¹ı³ÌÍê³Éºó»Ø0
								UARTCtrl->Task=0;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
								return;
							}
							Terminal_Note->List[ListNo].RMErr=1;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
							goto END_Fn;
						}
						Pn=MRR(AddrAFN04F10+2,2);//È¡ÅäÖÃµÄ²âÁ¿µãºÅ
						if(Pn==0)
						{//ÅäÖÃÉ¾³ı
							goto RouterSyn;//Â·Í¬²½
						}
					#if (USER/100)!=0//µç¿ÆÔº²âÊÔÉèÖÃF33µÈBUG
						i=MRR(ADDR_AFN04F30+(Pn-1),1);//Ì¨Çø¼¯ÖĞ³­±íÍ£³­/Í¶³­ÉèÖÃ(²âÁ¿µãºÅ)
						if(i!=0)
						{//Í£³­
							goto RMOK;//²»³­,Ö±½Ó·µ»Ø³­¶Á³É¹¦
						}
					#endif
						Protocol=MRR(AddrAFN04F10+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
						switch(Protocol)
						{
							case 1://DL/T645-1997
								MaxFnCount=Get_RM_DL645_1997_MaxFn();
								break;
						#if (USER/100)==5//ÉÏº£ÓÃ»§
							case 21://ÉÏº£¹æÔ¼
								MaxFnCount=Get_RM_ShH_MaxFn();
								break;
						#endif
							default://DL/T645-2007
								MaxFnCount=Get_RM_DL645_2007_MaxFn();
								break;
						}
						if(Terminal_Note->List[ListNo].FnCount>=MaxFnCount)
						{//Fn¼ÆÊı>=×î´óFn¸öÊı,±¾±í³­±í½áÊø
			END_Fn:
							if(Terminal_Note->List[ListNo].RMErr==0)//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
							{//³É¹¦
								Uart_3762_AFN14F1_Tx(PORTn,1);//Â·ÓÉÆ÷ÇëÇó³­±í·µ»ØÖ¡;Èë¿Ú:RMFlag³­¶Á±êÖ¾=0³­¶ÁÊ§°Ü,=1³­¶Á³É¹¦,2=³­¶Á
								Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=2;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
							}
							else
							{//ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
								Uart_3762_AFN14F1_Tx(PORTn,0);//Â·ÓÉÆ÷ÇëÇó³­±í·µ»ØÖ¡;Èë¿Ú:RMFlag³­¶Á±êÖ¾=0³­¶ÁÊ§°Ü,=1³­¶Á³É¹¦,2=³­¶Á
								Terminal_Note->List[ListNo].RMCompleteS[UARTCtrl->RMTaskC]=1;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
								
								CopyString((u8 *)"ÔØ²¨Ê§°Ü",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
								d64a<<=16;
								for(i=0;i<6;i++)
								{
									x=d64a>>56;
									d64a<<=8;
									MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
									MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
								}
								MWR(0,ADDR_DATABUFF+8+12,1);
								WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
							}
							Terminal_Note->List[ListNo].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
							Terminal_Note->List[ListNo].FnCount=0;//Fn¼ÆÊı
							Terminal_Note->List[ListNo].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
							Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢´ÎÊı
							Terminal_Note->List[ListNo].RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
							UARTCtrl->RMCount=0;//½«´ËÖµÒ²ÖÃÎª0,½øÈëÖØµã»§³­¶ÁÇúÏßÅĞ¶ÏÓÃ
							return;
						}
						
						ClassNo=MRR(AddrAFN04F10+26,1);//È¡´óĞ¡ÀàºÅ
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
							#if (USER/100)==5//ÉÏº£ÓÃ»§
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
								if(UARTCtrl->RMTaskC!=0)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
								{//²»ÊÇÆô³­
									if(AutoReportSearch==0)//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
									{
										AutoReportSearch=1;//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
										x=Check_Meter_Event_Save();//¼ì²éµçÄÜ±íÊÂ¼ş´æÖü¿Õ¼ä;·µ»Ø0=´æ´¢M(Ã»ÉÏ±¨),1-255ÔÊĞí»¹¿É´æ´¢µÄÌõÊı
										if(x>=10)
										{//¿Õ¼äÖÁÉÙ¿É´æ10Ìõ
											x=Check_AutoReportWord(PORTn);//¼ì²éµçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;·µ»Ø:0=ÎŞ,!=0 B31-B16ÎªÅäÖÃĞòºÅ(1¿ªÊ¼),B15-B0ÎªÒª¶ÁÈ¡µÄÖ÷¶¯ÉÏ±¨×´Ì¬×ÖÎ»ºÅ(0-95)
											x>>=16;
											if(x!=0)
											{//ÓĞÖ÷¶¯ÉÏ±¨ĞèÒª¶Á
												Terminal_Note->List[ListNo].FnCountBackupOfAutoReport=Terminal_Note->List[ListNo].FnCount;//485³­±íFn¼ÆÊı(Ô­³£¹æ³­±íµÄFn¼ÆÊı¼Ä´æ)
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
											#if (USER/100)==5//ÉÏº£ÓÃ»§
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
													MC(0,ADDR_AutoReportWord+((UARTCtrl->MeterNo-1)*LEN_AutoReportWord),LEN_AutoReportWord);
													UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfAutoReport;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
													return;
												}
												
												Terminal_Note->List[ListNo].FnCount=2;
												AutoReportSearch=0;//Ö÷¶¯ÉÏ±¨ËÑË÷±êÖ¾
												UARTCtrl->RMMeterEvent|=1;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
												UARTCtrl->FnEnd=1;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
												
												Terminal_Router->StopFlags|=(1<<17);//ÔİÍ£Â·ÓÉ;b17=³­Ö÷¶¯ÉÏ±¨ÊÂ¼ş
												Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
												UARTCtrl->RxByte=0;//RX¼ÆÊı(ÒÑ½ÓÊÕ×Ö½ÚÊı)=0
												UARTCtrl->Task=0;//¿ÕÏĞ
												return;
											}
										}
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
							UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//¸´ÖÆ´ËÖµ,½øÈëÖØµã»§³­¶ÁÇúÏßÅĞ¶ÏÓÃ
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
							if(Fn==26)
							{
							#if (USER/100)==5//ÉÏº£
								if((Protocol!=1) && (Protocol!=21))//(1=1997,21=ÉÏº£¹æÔ¼,ÆäËû=2007)
							#else
								if(Protocol!=1)//(1=1997,ÆäËû=2007)
							#endif
								{//2007
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
										i=MRR(AddrAFN04F10+18,1);//È¡µçÄÜ±í·ÑÂÊ
										if(i==1)
										{
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
								ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.Ê§°Ü
								if(p8RMLib[0]==129)
								{
									ClrERC29SOUR(Pn);//¼¯ÖĞ³­±íÊ§°ÜÇåERC29µçÄÜ±í·É×ßÊÂ¼şÔ­Ê¼Êı¾İ
								}
								//ÈÕ³­±í±êÖ¾
								if(UARTCtrl->RMTaskC==1)//µ±Ç°³­±íÈÎÎñºÅ:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=¶¨Ê±ÈÎÎñ,(MaxRMTask-1)=Ñ­³­ÈÎÎñ(Ö»ÓĞÑ­³­ÈÎÎñ²Å¿É²åÈë¸ßÓÅÏÈ¼¶³­±í)
								{//
									if(UARTCtrl->DateReRMCount>=3)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
									{
										DateRMFlags(Pn,1);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
									}
								}
								Terminal_Note->List[ListNo].RMErr=1;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
								Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
								Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı
								Terminal_Note->List[ListNo].ReRMCount=0;//ÖØ·¢¼ÆÊı
								Terminal_Note->List[ListNo].RMErr_0x901F=0;//ÔØ²¨DL645-1997ÈÕ¶³½á0xE3ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í·µ»Ø±êÊ¶´íÎó
							#if (USER/100)==5//ÉÏº£
								if(Protocol!=21)//(1=1997,21=ÉÏº£¹æÔ¼,ÆäËû=2007)
								{
									if((p8RMLib[0]==0xe3)||(p8RMLib[0]==0xee)||(p8RMLib[0]==0xef))
									{//³­ÉÏ1-3ÈÕÈÕ¶³½áÊı¾İ
										if(Terminal_Note->List[ListNo].RMCount==1)//³­±íÊı¾İ±êÊ¶¼ÆÊı
										{//ÊÇµÚ1¸ö³­ÈÕ¶³½áÊ±¼äYYMMDDhhmm´íÎóºó¼Ì³­µ½Ò²ÎŞÓÃ
											goto END_Fn;//½áÊø,¿ìËÙ·µ»ØÊ§°Ü,ÒÔ±ãÑ§³­¿ìËÙÇĞ»»Â·ÓÉ
										}
									}
								}
							#else
								if((p8RMLib[0]==0xe3)||(p8RMLib[0]==0xee)||(p8RMLib[0]==0xef))
								{//³­ÉÏ1-3ÈÕÈÕ¶³½áÊı¾İ
									if(Terminal_Note->List[ListNo].RMCount==1)//³­±íÊı¾İ±êÊ¶¼ÆÊı
									{//ÊÇµÚ1¸ö³­ÈÕ¶³½áÊ±¼äYYMMDDhhmm´íÎóºó¼Ì³­µ½Ò²ÎŞÓÃ
										goto END_Fn;//½áÊø,¿ìËÙ·µ»ØÊ§°Ü,ÒÔ±ãÑ§³­¿ìËÙÇĞ»»Â·ÓÉ
									}
								}
							#endif
								if(UARTCtrl->RMTaskC==2)
								{//ÖÜ³­»òÌæ´úµÄÊ±³­,Îª¼Ó¿ìÈÎÎñÍË³ö²»³­ºó¼ÌÊı¾İÏî
									goto END_Fn;
								}
								continue;
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
						
							switch(Protocol)
							{
								case 1://DL/T645-1997
									//È¡DIÍ¬Ê±¼Ó0x33
									i=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//È¡DI
								#if (USER/100)==4//ÉÂÎ÷ÓÃ»§
									if(p8RMLib[0]==0xE3)
									{
										if(i==0x901f)
										{
											if(Terminal_Note->List[ListNo].RMErr_0x901F==2)//ÔØ²¨DL645-1997ÈÕ¶³½á0xE3ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í·µ»Ø±êÊ¶´íÎó
											{
												i=0x9010;
												Terminal_Note->List[ListNo].RMErr_0x901F=0;//ÔØ²¨DL645-1997ÈÕ¶³½á0xE3ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í·µ»Ø±êÊ¶´íÎó
											}
											else
											{
												Terminal_Note->List[ListNo].RMErr_0x901F=1;//ÔØ²¨DL645-1997ÈÕ¶³½á0xE3ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í·µ»Ø±êÊ¶´íÎó
											}
										}
										else
										{
											Terminal_Note->List[ListNo].RMErr_0x901F=0;//ÔØ²¨DL645-1997ÈÕ¶³½á0xE3ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í·µ»Ø±êÊ¶´íÎó
										}
									}
								#endif
									p8txbuff[28+10+0]=i+0x33;
									p8txbuff[28+10+1]=(i>>8)+0x33;
									//È¡Í¨ĞÅµØÖ·
									MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
									p8txbuff[28+0]=0x68;
									p8txbuff[28+7]=0x68;
									p8txbuff[28+8]=0x01;
									p8txbuff[28+9]=0x2;
									p8txbuff[28+13]=0x16;
									//CS¼ÆËã
									p8txbuff[28+12]=0;
									for(i=0;i<12;i++)
									{
										p8txbuff[28+12]+=p8txbuff[28+i];
									}
									//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
									i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];//·µ»Ø³¤¶È
									if(i==0)
									{//·µ»Ø³¤¶È=0
										i=10+(3*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
										i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
									}
									p8txbuff[6]=i+14;
									i=14;//DL645Ö¡³¤¶È
									break;
							#if (USER/100)==5//ÉÏº£ÓÃ»§
								case 21://ÉÏº£¹æÔ¼
									p8txbuff[28+0]=0x68;
									//È¡Í¨ĞÅµØÖ·
									MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
									//¶¦ĞÅÒªÇóÉÏº£¹æÔ¼±íÍ¨ĞÅµØÖ·A5¹Ì¶¨·¢0xff;
									p8txbuff[28+6]=0xff;
									p8txbuff[28+7]=0x68;
									//È¡ÃüÁîÂë
									i=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//È¡DI
									i|=0x80;
									p8txbuff[28+8+0]=i;
									//³¤¶È
									p8txbuff[28+9]=0;
									//CS¼ÆËã
									p8txbuff[28+10]=0;
									for(i=0;i<10;i++)
									{
										p8txbuff[28+10]+=p8txbuff[28+i];
									}
									p8txbuff[28+11]=0x16;
									//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
									i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];//·µ»Ø³¤¶È
									p8txbuff[6]=i+14;
									i=12;//ÉÏº£¹æÔ¼Ö¡³¤¶È
									break;
							#endif
								default://DL/T645-2007
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
												continue;
										}
									#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
										x=MRR(AddrAFN04F10+18,1);//È¡µçÄÜ±í·ÑÂÊ
										if((x==1)&&(p8RMLib[0]==0xE3))
										{//±ä»»ÈÕ¶³½áÊı¾İ±êÊ¶
											switch(Terminal_Note->List[ListNo].RMCount)
											{
												default://ÈÕ¶³½áÊ±¼äYYMMDDhhmm
													Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
													Terminal_Note->List[ListNo].RMCount++;//³­±íÃüÁî¼ÆÊı
													Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
													continue;
												case 1://ÉÏ1´ÎÈÕÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
													i=0x00010000;//µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
													break;
											}
										}
									#endif
										//DIÍ¬Ê±¼Ó0x33
										p8txbuff[28+10+0]=i+0x33;
										p8txbuff[28+10+1]=(i>>8)+0x33;
										p8txbuff[28+10+2]=(i>>16)+0x33;
										p8txbuff[28+10+3]=(i>>24)+0x33;
										//È¡Í¨ĞÅµØÖ·
										MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
										p8txbuff[28+0]=0x68;
										p8txbuff[28+7]=0x68;
										p8txbuff[28+8]=0x11;
										p8txbuff[28+9]=0x4;
										p8txbuff[28+15]=0x16;
										//CS¼ÆËã
										p8txbuff[28+14]=0;
										for(i=0;i<14;i++)
										{
											p8txbuff[28+14]+=p8txbuff[28+i];
										}
										//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
										i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];//·µ»Ø³¤¶È
										if(i==0)
										{//·µ»Ø³¤¶È=0
											i=10+(5*p8RMLib[1])+Terminal_Note->List[ListNo].RMCount;
											i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
										}
										p8txbuff[6]=i+16;
										i=16;//DL645Ö¡³¤¶È
									}
									else
									{//¶Áµç±íÇúÏß
										//²¹µ±Ç°µ½ÉÏÈÕ0µãÇúÏßµã
										d64a=MRR(ADDR_TYMDHMS+1,5);//È¡µ±Ç°Ê±¼äÄêÔÂÈÕÊ±·Ö
									#if RMM_Carrier_CurveTime==15//ÔØ²¨Í¨µÀ¶Áµç±íÇúÏßµÄÕû¶³½á¼ä¸ôÊ±¼ä·Ö;15·Ö»ò60·Ö
										d64a=YMDHM_Int15(d64a);//ÄêÔÂÈÕÊ±·Ö»¯ÎªÕû15·Ö,Èç00Ê±18·Ö»¯Îª00Ê±15·Ö
										i=Terminal_Note->List[ListNo].RMCount*15;//Ã¿15·Ö¼ä¸ô
									#else
										d64a&=0xffffffff00;
										i=Terminal_Note->List[ListNo].RMCount*60;//Ã¿60·Ö¼ä¸ô
									#endif
										d64a=YMDHM_SubM(d64a,i);//ÄêÔÂÈÕÊ±·Ö¼õm·Ö,·µ»Ø¼õm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
										i=p8RMLib[0];
										i-=0xe4;
										i<<=16;
										i+=0x06000001;
										MWR(i,((u32)p8txbuff)+28+10,4);
										MWR(1,((u32)p8txbuff)+28+10+4,1);//¶Á1¸öµã
										MWR(d64a,((u32)p8txbuff)+28+10+4+1,5);
										//¼Ó0x33
										for(i=0;i<10;i++)
										{
											p8txbuff[28+10+i]+=0x33;
										}
										//È¡Í¨ĞÅµØÖ·
										MR(((u32)p8txbuff)+28+1,AddrAFN04F10+6,6);
										p8txbuff[28+0]=0x68;
										p8txbuff[28+7]=0x68;
										p8txbuff[28+8]=0x11;
										p8txbuff[28+9]=10;
										p8txbuff[28+21]=0x16;
										//CS¼ÆËã
										p8txbuff[28+20]=0;
										for(i=0;i<20;i++)
										{
											p8txbuff[28+20]+=p8txbuff[28+i];
										}
										//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
										i=p8RMLib[10+4];//·µ»Ø³¤¶È
										if(i==0)
										{//·µ»Ø³¤¶È=0
											i=10+(5*1);
											i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
										}
										p8txbuff[6]=i+16;
										i=22;//DL645Ö¡³¤¶È
									}
									break;
							}
							if(Terminal_Router->RouterRunMode_1==2)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
							{
								p8txbuff[3]=0x03;//¿ØÖÆÂë;¼¯ÖĞÊ½Â·ÓÉÔØ²¨Í¨ĞÅ
							}
							else
							{
								if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
								{
									p8txbuff[3]=0x0a;//¿ØÖÆÂë;Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ
								}
								else
								{
									p8txbuff[3]=0x01;//¿ØÖÆÂë;¼¯ÖĞÊ½Â·ÓÉÔØ²¨Í¨ĞÅ
								}
							}
							p8txbuff[4]=4;//Í¨ĞÅÄ£¿é±êÊ¶=1
							MR(((u32)p8txbuff)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
							MR(((u32)p8txbuff)+16,(u32)&Terminal_Note->List[ListNo].Addr,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
//						#if ((USER/100)==5)//ÉÏº£ÓÃ»§
//							if(Protocol==21)
//							{//ÉÏº£¹æÔ¼
//								p8txbuff[16+4]=bcd_hex(p8txbuff[16+4]);//8Î»Ñ¹ËõBCDÊı×ªÎªHEX
//							}
//						#endif
							p8txbuff[22]=0x14;//AFN=0x14
							p8txbuff[23]=0x01;//Fn=1
							p8txbuff[24]=0x00;//Fn=1
							p8txbuff[25]=2;//³­¶Á±êÖ¾=2
							p8txbuff[26]=0;//Í¨ĞÅÑÓÊ±Ïà¹ØĞÔ±êÖ¾,0=ÎŞ¹Ø,1=ÓĞ¹Ø
							p8txbuff[27]=i;//DL645Ö¡³¤¶È
							p8txbuff[28+i]=0;//¸½Êô½ÚµãÊıÁ¿N=0
							i+=29+2;//Ğè×Ü·¢ËÍ×Ö½ÚÊı
							p8txbuff[1]=i;
							p8txbuff[2]=i>>8;
							UARTCtrl->TxByte=i;//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)

							Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´ÈëºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
							UARTCtrl->Task=3;//µÈ´ı·¢ËÍ
							UARTCtrl->TxToRxTimerOver=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
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

								default://Ñ­³­
									CopyString((u8 *)"ÔØ²¨Ñ­³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
									break;
							}
							for(i=0;i<6;i++)
							{
								x=p8txbuff[16+5-i];
								MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
								MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
							}
							MWR(0,ADDR_DATABUFF+8+12,1);
							WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
							return;
						}
						//break;//²»Òª,ÒòÉÏÊÇwhile(1)
					case 0x0602://AFN=0x06,Fn=2;Â·ÓÉÆ÷·¢ËÍÉÏ±¨³­±íÊı¾İ
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//¸´ÖÆ10×Ö½Ú½ÓÊÕÍ·µ½·¢ËÍÍ·
						Info=MRR(((u32)p8rxbuff)+4,4);//È¡6×Ö½Ú376.2·µ»ØµÄĞÅÏ¢Óò
						i=p8rxbuff[18];//¶ÁDL645Ö¡³¤¶È
						UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+16,2);//ÏßÂ·ÉÏ´«ÊäÑÓÊ±Ê±¼äÃë
						MR(((u32)p8rxbuff),((u32)p8rxbuff)+19,i);//È¡645Ö¡
						goto RMESC;//³­±í·µ»Ø
					case 0x1402://AFN=0x14,Fn=2;Â·ÓÉÇëÇó¼¯ÖĞÆ÷Ê±ÖÓ
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//¸´ÖÆ10×Ö½Ú½ÓÊÕÍ·µ½·¢ËÍÍ·
						MW(ADDR_TYMDHMS,((u32)p8txbuff)+128,6);
						Uart_3762_TxData(PORTn,0x14,2,6);//376.2·¢ËÍÊı¾İÌîÈë·¢ËÍÇø;Èë¿ÚÊ±ÈôÓĞĞÅÏ¢ÓòÄ¿µÄµØÖ·Êı¾İµ¥ÔªµÈÔòÒÑÌîÈëÇÒÊı¾İµ¥ÔªÔİ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
						break;
					//case 0x1404://AFN=0x14,Fn=3;ÇëÇóÒÀÍ¨ĞÅÑÓÊ±ĞŞÕıÍ¨ĞÅÊı¾İ
						//break;
					default:
						UARTCtrl->RxByte=0;//½ÓÊÕ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
						UARTCtrl->Task=5;//ÕıÔÚ½ÓÊÕ
						break;
				}
			}
			else
			{//µçÄÜ±íÍ¨ĞÅÄ£¿é
				i=MRR(((u32)p8rxbuff)+22,3);
				switch(i)
				{
					case 0x000206://AFN=0x06,Fn=2;Â·ÓÉÆ÷·¢ËÍÉÏ±¨³­±íÊı¾İ
						MR((u32)p8txbuff,(u32)p8rxbuff,10);//¸´ÖÆ10×Ö½Ú½ÓÊÕÍ·µ½·¢ËÍÍ·
						Info=MRR(((u32)p8rxbuff)+4,4);//È¡6×Ö½Ú376.2·µ»ØµÄĞÅÏ¢Óò
						i=p8rxbuff[30];//¶ÁDL645Ö¡³¤¶È
						UARTCtrl->TransferDelay=MRR(((u32)p8rxbuff)+28,2);//ÏßÂ·ÉÏ´«ÊäÑÓÊ±Ê±¼äÃë
						MR(((u32)p8rxbuff),((u32)p8rxbuff)+31,i);//È¡645Ö¡
				RMESC://³­±í·µ»Ø
						for(y=0;y<i;y++)
						{//È¥·µ»Ø645Ö¡µÄÇ°µ¼0xfe
							if(p8rxbuff[y]!=0xfe)
							{
								if(y!=0)
								{
									MR((u32)p8rxbuff,((u32)p8rxbuff)+y,i-y);
								}
								break;
							}
						}
						d64a=MRR(((u32)p8rxbuff)+1,6);//Ö¡·µ»Ø±íµØÖ·
						x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
						p32=(u32 *)((u32)&Terminal_Note->List[0].Addr);
						for(ListNo=0;ListNo<x;ListNo++)
						{
							d64b=p32[1]&0xffff;
							d64b<<=32;
							d64b|=p32[0];
						#if (USER/100)==5//ÉÏº£
							i=Terminal_Note->List[ListNo].MeterNo;
							Protocol=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
							if(Protocol==21)
							{//ÉÏº£¹æÔ¼
								//Í¨ĞÅµØÖ·A5²»ÓÃ±È½Ï
								d64a&=0xffffffffff;
								d64b&=0xffffffffff;
							}
						#endif
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
						p8FnData=(u8 *)(u32)&Terminal_Note->List[ListNo].FnDataBuff;
						//MWR(Info,(u32)&Terminal_Note->List[ListNo].Info,4);//4×Ö½Ú×îºó´ÎÔØ²¨³­±í·µ»ØµÄĞÅÏ¢Óò
						
						x=Terminal_Note->List[ListNo].MeterNo;//È¡¶ÔÓ¦µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(´Ó1¿ªÊ¼)
						if((x==0)||(x>AFN04F10MaxPn))
						{//ĞòºÅ´í
							goto RXESC;//ÉÏ±¨·µ»Ø
						}
						x--;
						AddrAFN04F10=ADDR_AFN04F10+(x*LEN_AFN04F10_Pn);
						Pn=MRR(AddrAFN04F10+2,2);//È¡ÅäÖÃµÄ²âÁ¿µãºÅ
						if(Pn==0)
						{//ÅäÖÃÉ¾³ı
							goto RXESC;//ÉÏ±¨·µ»Ø	
						}
						d64b=MRR(AddrAFN04F10+6,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
//					#if ((USER/100)==5)//ÉÏº£ÓÃ»§
//						Protocol=MRR(AddrAFN04F10+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
//						if(Protocol==21)
//						{//ÉÏº£¹æÔ¼
//							i=(d64b>>24)&0xff;
//							d64a=hex_bcd(i);//HEXÊı×ªÎª8Î»Ñ¹ËõBCDÊı
//							d64b&=0xffff00ffffff;
//							d64b+=(d64a<<24);
//						}
//						d64a=MRR(((u32)p8rxbuff)+1,6);//Ö¡·µ»Ø±íµØÖ·
//					#endif
					#if (USER/100)==5//ÉÏº£
						Protocol=MRR(ADDR_AFN04F10+(x*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
						if(Protocol==21)
						{//ÉÏº£¹æÔ¼
							//Í¨ĞÅµØÖ·A5²»ÓÃ±È½Ï
							d64a&=0xffffffffff;
							d64b&=0xffffffffff;
						}
					#endif
						if(d64a!=d64b)
						{//ÅäÖÃ±ä»¯
							goto RXESC;//ÉÏ±¨·µ»Ø
						}
					#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
					#if EnMeterAutoAutoReport==1//ÔÊĞíµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş:0=²»ÔÊĞí,1=ÔÊĞí
						if(Comm_Ram->RouterType==2)//Â·ÓÉÆ÷ÀàĞÍ:0=±£Áô,1=ÕıÌ©,2=¶¦ĞÅ,3=¶«Èí,4=Ïş³¿,5=ÈğË¹¿µ,6=Á¦ºÏÎ¢,7=
						{//¶¦ĞÅÄ£¿é
							if(p8txbuff[8]==0x01)
							{//¼Ğ´øÉÏ±¨Ä£Ê½
								MR(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
								MWR(1,ADDR_DATABUFF+1,1);//B1±£Áô,ÏÖÓÃÓÚ±íÊ¾ÓĞÖ÷¶¯ÉÏ±¨
								MWR(1,ADDR_DATABUFF+96,1);//Êı¾İ»ò
								MW(ADDR_DATABUFF,ADDR_AutoReportWord+(x*LEN_AutoReportWord),96+1);
							}
						}
					#endif
					#endif
						Protocol=MRR(AddrAFN04F10+5,1);//È¡Í¨ĞÅĞ­ÒéÀàĞÍ:1=DL/T645-1997,30=DL/T645-2007
						switch(Protocol)
						{
							case 1://DL/T645-1997
								i=DL645_1997_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						#if (USER/100)==5//ÉÏº£ÓÃ»§
							case 21://ÉÏº£¹æÔ¼
								i=ShH_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						#endif
							default://DL/T645-2007
								i=DL645_2007_RMLib[Terminal_Note->List[ListNo].FnCount];
								break;
						}
						p8RMLib=(u8 *)i;//³­±í¿â
						
						if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
						{//ÆğÊ¼·û0x68´í
							goto RXESC;//ÉÏ±¨·µ»Ø
						}
						x=0;//ºÍĞ£Ñé¼ÆËã
						for(i=0;i<(p8rxbuff[9]+10);i++)
						{
							x+=p8rxbuff[i];
						}
						x&=0xff;
						if((x!=p8rxbuff[i])||(p8rxbuff[i+1]!=0x16))
						{//ºÍĞ£Ñé´íÎó
							goto RXESC;//ÉÏ±¨·µ»Ø
						}
						for(i=0;i<p8rxbuff[9];i++)
						{//Êı¾İÓò¼õ0x33
							p8rxbuff[10+i]-=0x33;
						}
					#if (USER/100)==5//ÉÏº£ÓÃ»§
						if(Protocol!=21)//²»ÊÇÉÏº£¹æÔ¼
						{
							if((p8rxbuff[8]&0x40)!=0x0)
							{//´ÓÕ¾Òì³£»Ø´ğ,ÎŞ´ËÊı¾İÏî,Õı³£Ìø¹ı
								Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
								Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
								Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
								goto RXESC;//ÉÏ±¨·µ»Ø
							}
						}
					#else
						i=Terminal_Note->List[ListNo].RMErr_0x901F;//ÔØ²¨DL645-1997ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í
						Terminal_Note->List[ListNo].RMErr_0x901F=0;//ÔØ²¨DL645-1997ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í
						if((p8rxbuff[8]&0x40)!=0x0)
						{//´ÓÕ¾Òì³£»Ø´ğ,ÎŞ´ËÊı¾İÏî,Õı³£Ìø¹ı
//20150310
						#if (USER/100)==4//ÓÃ»§:ÉÂÎ÷
							if(Protocol==1)
							{//DL/T645-1997
								if(p8RMLib[0]==0xE3)
								{
									if(Terminal_Note->List[ListNo].RMCount==1)//³­±íÊı¾İ±êÊ¶¼ÆÊı=1,ÃüÁî=0x901f
									{
										if(i==1)//ÔØ²¨DL645-1997ÈÕ¶³½á0xE3ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í·µ»Ø±êÊ¶´íÎó
										{
											Terminal_Note->List[ListNo].RMErr_0x901F=2;//ÔØ²¨DL645-1997ÈÕ¶³½á0xE3ÎŞ0x901FÃüÁîÊ±ÓÃ0x9010³­±íµÄ±ê¼Ç;0=¿Õ,1=·¢0x901f,2=0x901F³­±í·µ»Ø±êÊ¶´íÎó
											if(	Terminal_Note->List[ListNo].ReRMCount!=0)//³­±íÖØ·¢¼ÆÊı
											{
												Terminal_Note->List[ListNo].ReRMCount--;//³­±íÖØ·¢¼ÆÊı
											}
											goto RXESC;//ÉÏ±¨·µ»Ø
										}
									}
								}
							}
						#endif
							Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
							Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
							Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
							goto RXESC;//ÉÏ±¨·µ»Ø
						}
					#endif
						switch(Protocol)
						{
							case 1://DL/T645-1997
								x=MRR((u32)&p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)],2);//È¡DI
								y=MRR(((u32)p8rxbuff)+10,2);
//20150310
							#if (USER/100)==4//ÓÃ»§:ÉÂÎ÷
								if(p8RMLib[0]==0xE3)
								{
									if(y==0x9010)
									{
										y=0x901f;
									}
								}
							#endif
								if(x!=y)
								{//DI´íÎó
									goto RXESC;//ÉÏ±¨·µ»Ø
								}
								if(p8rxbuff[9]==2)
								{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø2×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
									Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
									Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
									Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
									goto RXESC;//ÉÏ±¨·µ»Ø
								}
								i=p8RMLib[10+(3*Terminal_Note->List[ListNo].RMCount)+2];
								if((i==0x0)||(p8rxbuff[9]==(i+2))||(p8rxbuff[9]==(i+2+1)))
								{//·µ»Ø³¤¶ÈÏàÍ¬
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//¶ÔÓ¦µçÄÜ±íÅäÖÃĞòºÅ(°ë×Ö¶ÔÆë)(´Ó1¿ªÊ¼)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
									ReadMeter_DL645_1997_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_1997³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
									ReadMeterNote_OK(PORTn,Pn,Info);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
									DateRMFlags(Pn,0);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
								#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­
									if((p8RMLib[0]==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))//³­±íÃüÁî¼ÆÊı
									{
										goto Time;
									}
								#endif
//									switch(p8RMLib[0])
//									{
//										case 129://F129 µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ
//										case 162://F162 µçÄÜ±íÈÕÀúÊ±ÖÓ
//										case 0xe3://À©Õ¹3 ¼¯ÖĞ³­±íÈÕ¶³½áÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
//									}
									
								}
								break;
						#if (USER/100)==5//ÉÏº£ÓÃ»§
							case 21://ÉÏº£¹æÔ¼
								x=MRR((u32)&p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)],1);//È¡DI
								y=MRR(((u32)p8rxbuff)+8,1);
								if(x!=y)
								{//DI´íÎó
									goto RXESC;//ÉÏ±¨·µ»Ø
								}
								i=p8RMLib[10+(2*Terminal_Note->List[ListNo].RMCount)+1];
								if((p8rxbuff[9]==i))
								{//·µ»Ø³¤¶ÈÏàÍ¬
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//¶ÔÓ¦µçÄÜ±íÅäÖÃĞòºÅ(°ë×Ö¶ÔÆë)(´Ó1¿ªÊ¼)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
									ReadMeter_ShH_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//ÉÏº£¹æÔ¼³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
									ReadMeterNote_OK(PORTn,Pn,Info);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
									DateRMFlags(Pn,0);//ÈÕ³­±í±êÖ¾;Èë¿Ú:f=0³É¹¦,=1Ê§°Ü
								}
								break;
						#endif
							default://DL/T645-2007
								if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
								{//²»ÊÇ¶Áµç±íÇúÏß
									x=MRR((u32)&p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)],4);//È¡DI
								#if (USER/100)==4//ÓÃ»§±êÊ¶:ÉÂÎ÷
									i=MRR(AddrAFN04F10+18,1);//È¡µçÄÜ±í·ÑÂÊ
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
								}
								else
								{//ÊÇ¶Áµç±íÇúÏß
									x=MRR((u32)&p8RMLib[10],4);//È¡DI
								}
								y=MRR(((u32)p8rxbuff)+10,4);
								if(x!=y)
								{//DI´íÎó
									goto RXESC;//ÉÏ±¨·µ»Ø
								}
								if(p8rxbuff[9]==4)
								{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø4×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
									Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
									Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶¼ÆÊı+1
									Terminal_Note->List[ListNo].RMCountErr++;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
									goto RXESC;//ÉÏ±¨·µ»Ø
								}
								if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
								{//²»ÊÇ¶Áµç±íÇúÏß
									i=p8RMLib[10+(5*Terminal_Note->List[ListNo].RMCount)+4];
								}
								else
								{//ÊÇ¶Áµç±íÇúÏß
									i=p8RMLib[10+4];
								}
								if((i==0x0)||(p8rxbuff[9]==(i+4))||(p8rxbuff[9]==(i+4+1)))
								{//·µ»Ø³¤¶ÈÏàÍ¬
									UARTCtrl->MeterNo=Terminal_Note->List[ListNo].MeterNo;//¶ÔÓ¦µçÄÜ±íÅäÖÃĞòºÅ(°ë×Ö¶ÔÆë)(´Ó1¿ªÊ¼)
									UARTCtrl->FnCount=Terminal_Note->List[ListNo].FnCount;//³­±íFn¼ÆÊı
									UARTCtrl->RMCount=Terminal_Note->List[ListNo].RMCount;//³­±íÊı¾İ±êÊ¶¼ÆÊı
									ReadMeter_DL645_2007_MulFnData(PORTn,(u32)p8FnData,p8RMLib);//DL645_2007³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
									Terminal_Note->List[ListNo].RMCount=UARTCtrl->RMCount;//·µ»ØµÄ³­±íÊı¾İ±êÊ¶¼ÆÊı
	
									Pn=Terminal_Note->List[ListNo].Pn;
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
							#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­
									if((p8RMLib[0]==162)&&(Pn!=0)&&(UARTCtrl->RMCount==1))//³­±íÃüÁî¼ÆÊı
									{
									Time:
								 		MR(ADDR_DATABUFF,((u32)p8FnData)+1+6,6);
										MR(ADDR_DATABUFF+6,((u32)p8FnData)+1,6);
										p8=(u8 *)(ADDR_DATABUFF);
										if(p8[0]!=0xee)
										{
											p8[4]&=0x1f;//È¥¼ÓÔÚÔÂ×Ö½ÚÉÏµÄĞÇÆÚ
											p8[10]&=0x1f;//È¥¼ÓÔÚÔÂ×Ö½ÚÉÏµÄĞÇÆÚ
											i=Check_YMDHMS(MRR(ADDR_DATABUFF,6));//ÄêÔÂÈÕÊ±·ÖÃëºÏ·¨¼ì²é,·µ»Ø0ºÏ·¨,1·Ç·¨
											if(i==0)
											{//µçÄÜ±íÊ±ÖÓºÏ·¨
												i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
												if((i&0x01)==0)
												{
													MWR(i|0x01,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
												}
												i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//Ê±ÖÓ²îÖµ¼ÆËã,Èë¿ÚR0=Ê±ÖÓ1(Ãë·ÖÊ±ÈÕÔÂÄê)µØÖ·;R1=Ê±ÖÓ2(Ãë·ÖÊ±ÈÕÔÂÄê)µØÖ·;³ö¿ÚR0=HEX²¹ÂëÊ±ÖÓ¿ì»òÂıµÄ²îÖµ(Ãë),Ê±ÖÓ1Ê±ÖÓ2·Ç·¨R0=0,Ê±ÖÓ2>=Ê±ÖÓ1²îÖµÎªÕı,Ê±ÖÓ2<Ê±ÖÓ1²îÖµÎª¸º
												x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
												if(x<3)
												{
													MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
													MWR(i,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//µçÄÜ±í¶ÔÊ±Ç°µÄÎó²î
													MWR(0,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2,2);//µçÄÜ±í¶ÔÊ±ºóµÄÎó²î
													MWR(0xff,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//µçÄÜ±í¶ÔÊ±ºóµÄ½á¹û
												}
												if(x==3)
												{
													if(MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1)==0xff)//µçÄÜ±í¶ÔÊ±ºóµÄ½á¹û
													{
														x=MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//µçÄÜ±í¶ÔÊ±Ç°µÄÎó²î
														if((x>>15)!=0x0)
														{//¸º
															x=~x;
															x++;
															MWR(x,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5,2);//µçÄÜ±í¶ÔÊ±Ç°µÄÎó²î
														}
														if(x>MRR(ADDR_AFN04F298_Ext+7,1))
														{
															MR(ADDR_AFN0CF250_Ext1+10*(Pn-1),ADDR_TYMDHMS+1,5);
															if((i>>31)!=0x0)
															{//¸º
																i=~i;
																i++;
															}
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
															ERC51_Event(Pn+(1<<15),ADDR_DATABUFF);//Ğ£Ê±¼ÇÂ¼ÊÂ¼ş;Èë¿Ú:Pn²âÁ¿µãºÅ(´Ó0¿ªÊ¼)
														#if (USER/100)==8//ÓÃ»§±êÊ¶£ºÍ¨ÓÃ
															p8=(u8 *)ADDR_TYMDHMS;
															MR((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1),3);
															x=DataComp((u32)ADDR_DATABUFF,(u32)ADDR_TYMDHMS+3,3);//Êı¾İ±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
															if(x==0)
															{
																x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);
																if(x==0xffffff)
																{
																	DMWR(i,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*(p8[3]-1)+3+4+(Pn-1)*3,3);//¼Ä´æÆ÷Ö±½ÓĞ´µ½´æ´¢Æ÷(<=8×Ö½Ú)(ÎŞ²Á³ı)
																}
															}
															//Ê±¼ä³¬²î 5min ²âÁ¿µãÊıÁ¿
															if(i>300)
															{
																x=MRR(ADDR_AFN0DF321_Ext_Source,2);
																x++;
																MWR(x,ADDR_AFN0DF321_Ext_Source,2);
															}
															//³¬¹ıÊ±¼äãĞÖµÎ´³¬²î5min²âÁ¿µãÊıÁ¿
															if((i>MRR(ADDR_AFN04F298_Ext+7,1))&&(i<300))
															{
																x=MRR(ADDR_AFN0DF321_Ext_Source+2,2);
																x++;
																MWR(x,ADDR_AFN0DF321_Ext_Source+2,2);
															}
														#endif
														}
														else
														{
															MWR(2,ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1);//µçÄÜ±í¶ÔÊ±ºóµÄ½á¹û
														}
														x=MRR(ADDR_AFN0CF250_Ext+(1+10*31)*(Pn-1),1);//µçÄÜ±í¶ÔÊ±´ÎÊı
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
									}
							#endif
//									switch(p8RMLib[0])
//									{
//										case 129://F129 µ±Ç°ÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ
//										case 162://F162 µçÄÜ±íÈÕÀúÊ±ÖÓ
//										case 0xe3://À©Õ¹3 ¼¯ÖĞ³­±íÈÕ¶³½áÕıÏòÓĞ¹¦µçÄÜÊ¾Öµ£¨×Ü¡¢·ÑÂÊ1¡«M£©
//									}
									
								}
								break;
						}
						Terminal_Note->List[ListNo].ReRMCount=0;//³­±íÖØ·¢¼ÆÊı=0
						Terminal_Note->List[ListNo].RMCount++;//³­±íÊı¾İ±êÊ¶+1
						
						//·¢È·ÈÏ»Ø´ğÉÏ±¨³­±íÊı¾İ
					RXESC:
						MWR(0xffffffff,((u32)p8txbuff)+128,6);
						Uart_3762_TxData(PORTn,0,1,6);//376.2·¢ËÍÊı¾İÌîÈë·¢ËÍÇø,Èë¿ÚÊ±ÈôÓĞĞÅÏ¢ÓòÄ¿µÄµØÖ·Êı¾İµ¥ÔªµÈÔòÒÑÌîÈë,Êı¾İµ¥ÔªÔİ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
						if(Terminal_Router->RouterRunMode_1==2)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
						{
							p8txbuff[3]=0x03;//¿ØÖÆÂë;¼¯ÖĞÊ½Â·ÓÉÔØ²¨Í¨ĞÅ
						}
						else
						{
							if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
							{
								p8txbuff[3]=0x0a;//¿ØÖÆÂë;Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ
							}
							else
							{
								p8txbuff[3]=0x01;//¿ØÖÆÂë;¼¯ÖĞÊ½Â·ÓÉÔØ²¨Í¨ĞÅ
							}
						}
						Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
						break;
					default:
						UARTCtrl->RxByte=0;//½ÓÊÕ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
						UARTCtrl->Task=5;//ÕıÔÚ½ÓÊÕ
						break;
				}
			}
			break;
	}
}



	#if(USER/100)==0
#define OnlyFnMaxReRMCount     2//1//³­±íÖØ·¢´ÎÊı(½öÖØ·¢´ÎÊı,²»Ëã³õÊ¼1´ÎµÄ·¢ËÍ);µçÁ¦ÏßÔØ²¨=1,Î¢¹¦ÂÊÎŞÏß=2
	#else
#define OnlyFnMaxReRMCount     0//³­±íÖØ·¢´ÎÊı(½öÖØ·¢´ÎÊı,²»Ëã³õÊ¼1´ÎµÄ·¢ËÍ)
	#endif
void RealTimeReadMeter_3762_Fn_Std(u32 PORTn,u8 *p8RMLib)//½ö³­1¸öFnÊı¾İÌîĞ´µ½FnDataBuff;RouterRealTimeTask=0=¿ÕÏĞ(±¾Fn³­¶ÁÍê³É)
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
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterRealTimeTask)//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ;0=¿ÕÏĞ,1=ÔİÍ£Â·ÓÉ,2=·¢ËÍ,3=½ÓÊÕ
	{
		default:
			Terminal_Router->RouterRealTimeTask=0;//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ;0=¿ÕÏĞ,1=ÔİÍ£Â·ÓÉ,2=·¢ËÍ,3=½ÓÊÕ
			break;
		case 0://¿ÕÏĞ
			break;
		case 1://ÔİÍ£Â·ÓÉ
			if(UARTCtrl->Task<=2)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				UARTCtrl->Task=0;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				Terminal_Router->StopFlags|=(1<<7);//ÔİÍ£Â·ÓÉ
				Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				Terminal_Router->RouterRealTimeTask++;//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ
			}
			break;
		case 2://AFN13 F1 Â·ÓÉÊı¾İ×ª·¢:¼à¿ØÔØ²¨´Ó½Úµã
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//±¾ĞòºÅµÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÉ¾³ı
			END_Fn:
				UARTCtrl->Task=0;//
				UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊı=0
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
				Terminal_Router->StopFlags&=~(1<<7);//»Ö¸´Â·ÓÉ
				Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				Terminal_Router->RouterRealTimeTask=0;//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ;0=¿ÕÏĞ,1=ÔİÍ£Â·ÓÉ,2=·¢ËÍ,3=½ÓÊÕ
				break;
			}
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			if((p8RMLib[3]&0x04)==0)//³­±íÖ§³Ö:B2=ÔØ²¨³­±í,B1=RS485³­±í,B0=ÄÚ²¿½»²É³­±í
			{//±¾Fn²»ÊÇÔØ²¨³­±í
				goto END_Fn;
			}
			if(p8RMLib[1]==0x0)
			{//±¾Fn³­±íÃüÁîÊı=0
				goto END_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//³­±íÃüÁî¼ÆÊı=0,ÇåFnÊı¾İ»º³å
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(p8RMLib[0]==165)
			{//F165 µçÄÜ±í¿ª¹Ø²Ù×÷´ÎÊı¼°Ê±¼ä
				//ÃüÁî¼ÆÊı0-5,ÆäÖĞ×îºó2Ïî4-5½öÓÃÓÚÊÂ¼ş¼ÇÂ¼Êµ³­Ê±²»³­
//				dcdu 0x03300000//±à³Ì×Ü´ÎÊı
//				dcb 3//·µ»Ø³¤¶È
//				dcdu 0x03300001//×î½üÒ»´Î±à³Ì·¢ÉúÊ±¿Ì
//				dcb 50//·µ»Ø³¤¶È
//				dcdu 0x03300E00//µçÄÜ±íÎ²¸Ç´ò¿ª´ÎÊı
//				dcb 3//·µ»Ø³¤¶È
//				dcdu 0x03300E01//×î½üÒ»´ÎÎ²¸Ç´ò¿ªÊ±¼ä
//				dcb 60//·µ»Ø³¤¶È
//				dcdu 0x03300D00//µçÄÜ±í¿ª±í¸Ç´ò¿ª´ÎÊı(½öÓÃÓÚÊÂ¼ş¼ÇÂ¼)
//				dcb 3//·µ»Ø³¤¶È
//				dcdu 0x03300D01//×î½üÒ»´Î¿ª±í¸Ç¼ÇÂ¼(½öÓÃÓÚÊÂ¼ş¼ÇÂ¼)
//				dcb 60//·µ»Ø¤È
				if(UARTCtrl->RMCount>=4)
				{
					UARTCtrl->RMCount=6;
				}
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//³­±íÃüÁî¼ÆÊı>=FnĞè·¢ÃüÁîÊı;±¾Fn³­±íÍê³É
				if(p8RMLib[0]==28)
				{//F28 µç±íÔËĞĞ×´Ì¬×Ö¼°Æä±äÎ»±êÖ¾
					ReadMeter_SaveFnData(PORTn,p8RMLib[0],Pn,Get_ADDR_UARTnFnDataBuff(PORTn),p8RMLib);//´æÖü³­±íFnÊı¾İ;ÈôÖ»´æ´¢ÊµÊ±Êı¾İu8 *p8RMLib=0
				}
				goto END_Fn;
			}
			
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
			switch(Protocol)
			{
				case 1://DL/T645-1997
					i=MRR((u32)&p8RMLib[10+(3*UARTCtrl->RMCount)],2);//È¡DI
					Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
					//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
					i=p8RMLib[10+(3*UARTCtrl->RMCount)+2];//·µ»Ø³¤¶È
					if(i==0)
					{//·µ»Ø³¤¶È=0
						i=10+(3*p8RMLib[1])+UARTCtrl->RMCount;
						i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
					}
					i+=14;
					Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
					if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
					{//Î¢¹¦ÂÊÎŞÏß,µµ°¸µØÖ·ÓÃ²É¼¯Æ÷µØÖ·
						d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
						if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
						{
							MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
							Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
						}
					}
					break;
			#if (USER/100)==5//ÉÏº£ÓÃ»§
				case 21://ÉÏº£¹æÔ¼
					i=MRR((u32)&p8RMLib[10+(2*UARTCtrl->RMCount)],1);//È¡DI
					Read_ShH_TxData(ADDR_DATABUFF,i,d64a);//ÉÏº£¹æÔ¼¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
					//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
					i=p8RMLib[10+(2*UARTCtrl->RMCount)+1];//·µ»Ø³¤¶È
					i+=14;
					//¶¦ĞÅÒªÇóÌí¼ÓÉÏº£Ğ­Òéµç±íÊ±£¬ÒªÇó¹æÔ¼ÀàĞÍ±ØĞëÎª¡°00¡±
					Uart_3762_AFN13F1_Tx(PORTn,0,ADDR_DATABUFF,12,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
					MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
					Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
					if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
					{//Î¢¹¦ÂÊÎŞÏß,µµ°¸µØÖ·ÓÃ²É¼¯Æ÷µØÖ·
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
			#if (USER/100)==6//ºşÄÏ
					if(p8RMLib[0]==27)//F27 µçÄÜ±íÈÕÀúÊ±ÖÓ¡¢±à³Ì´ÎÊı¼°Æä×î½üÒ»´Î²Ù×÷Ê±¼äµÈ
					{
						switch(UARTCtrl->RMCount)
						{
							case 0://µçÄÜ±íÈÕÀúÊ±ÖÓHhmmss  r
							case 1://µçÄÜ±íÈÕÀúÊ±ÖÓYYMMDDWW r
							case 2://µç³Ø¹¤×÷Ê±¼ä r
							case 3://±à³Ì×Ü´ÎÊı  r
							case 4://×î½üÒ»´Î±à³Ì·¢ÉúÊ±¿Ì  r
							//case 5://µç±íÇåÁã×Ü´ÎÊı
							//case 6://×î½üÒ»´Îµç±íÇåÁã·¢ÉúÊ±¿Ì
							case 7://ĞèÁ¿ÇåÁã×Ü´ÎÊı  r
							//case 8://×î½üÒ»´ÎĞèÁ¿ÇåÁã·¢ÉúÊ±¿Ì
							//case 9://ÊÂ¼şÇåÁã×Ü´ÎÊı
							//case 10://×î½üÒ»´ÎÊÂ¼şÇåÁã·¢ÉúÊ±¿Ì
							//case 11://Ğ£Ê±×Ü´ÎÊı
							//case 12://×î½üÒ»´ÎĞ£Ê±·¢ÉúÊ±¿Ì
								break;
							default:
								UARTCtrl->RMCount++;
								UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
								return;
						}
					}
			#endif
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//²»ÊÇ¶Áµç±íÇúÏß
						i=MRR((u32)&p8RMLib[10+(5*UARTCtrl->RMCount)],4);//È¡DI
						Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
						//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
						i=p8RMLib[10+(5*UARTCtrl->RMCount)+4];//·µ»Ø³¤¶È
						if(i==0)
						{//·µ»Ø³¤¶È=0
							i=10+(5*p8RMLib[1])+UARTCtrl->RMCount;
							i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
						}
						i+=16;
						Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
						if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
						{//Î¢¹¦ÂÊÎŞÏß,µµ°¸µØÖ·ÓÃ²É¼¯Æ÷µØÖ·
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
								Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
							}
						}
					}
					else
					{//¶Áµç±íÇúÏß
						i=MRR(((u32)p8RMLib)+10,4);
						d64b=MRR((u32)&UARTCtrl->RMCommandYMDHM[0],5);//ÇúÏßµÄÊ±±êÄêÔÂÈÕÊ±·Ö
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
						{//Î¢¹¦ÂÊÎŞÏß,µµ°¸µØÖ·ÓÃ²É¼¯Æ÷µØÖ·
							d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
							if((d64a!=0xeeeeeeeeeeee)&&(d64a!=0))
							{
								MWR(d64a,((u32)p8txbuff)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
								Uart_3762_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã²¢Ğ´Èë(Í¬Ê±Ğ´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕıÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
							}
						}
					}
					break;
			}
			UARTCtrl->TxByte=31+p8txbuff[28];//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
			i=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;
			Terminal_Router->RouterRealTimeTask++;//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ
			
			//ÏÔÊ¾³­±íµØÖ·
			CopyString((u8 *)"ÔØ²¨Êµ³­",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[30+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
			break;
		case 3://½ÓÊÕ
			if(UARTCtrl->Task>=3)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				break;
			}
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(UARTCtrl->Task!=1)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{//³¬Ê±
			#if OnlyFnMaxReRMCount!=0
				if(UARTCtrl->ReRMCount<OnlyFnMaxReRMCount)//485³­±íÃüÁîÖØ·¢¼ÆÊı
				{
					UARTCtrl->ReRMCount++;//485³­±íÃüÁîÖØ·¢¼ÆÊı
				}
				else
				{
					UARTCtrl->RMCount=200;//³­±íÃüÁî¼ÆÊı,µã³­Ê§°Ü²»³­ºóĞøÃüÁî
					UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
					ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.Ê§°Ü
				}
			#else
				UARTCtrl->RMCount=200;//³­±íÃüÁî¼ÆÊı,µã³­Ê§°Ü²»³­ºóĞøÃüÁî
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
				ReadMeterNote_Fail(PORTn,Pn);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.Ê§°Ü
			#endif
				UARTCtrl->Task=0;//¿ÕÏĞ1´Î
				Terminal_Router->RouterRealTimeTask--;//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ
				break;
			}
			d64a=MRR(((u32)p8rxbuff)+4,6);
			ReadMeterNote_OK(PORTn,Pn,d64a);//¼¯ÖĞ³­±íµç±í³­¶ÁĞÅÏ¢.³É¹¦
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//·ñÈÏ»Ø´ğ
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			Protocol=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
//		#if (USER/100)==5//ÉÏº£
//			if(Protocol==21)
//			{//ÉÏº£¹æÔ¼
//				i=(d64a>>24)&0xff;
//				d64b=bcd_hex(i);//8Î»Ñ¹ËõBCDÊı×ªÎªHEX
//				d64a&=0xffff00ffffff;
//				d64a+=(d64b<<24);
//			}
//		#endif
			y=p8rxbuff[28];//·µ»ØÊı¾İÓò³¤¶È
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				y=0;//½ÓÊÕÊı¾İ³¤¶È
			}
			if(y==0)
			{//½ÓÊÕÊı¾İ³¤¶È=0
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			i=MRR(((u32)p8rxbuff)+25,2);//ÏßÂ·ÉÏ´«ÊäÑÓÊ±Ê±¼äÃë
			UARTCtrl->TransferDelay=i;
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
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//·µ»ØµÄµØÖ·
		#if (USER/100)==5//ÉÏº£
			if(Protocol==21)
			{//ÉÏº£¹æÔ¼
				//Í¨ĞÅµØÖ·A5²»ÓÃ±È½Ï
				d64a&=0xffffffffff;
				d64b&=0xffffffffff;
			}
		#endif
			if(d64a!=d64b)
			{//±íµØÖ·²»ÏàÍ¬
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			x=0;//ºÍĞ£Ñé¼ÆËã
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//ºÍĞ£Ñé´í
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
		#if (USER/100)==5//ÉÏº£
			if(Protocol==21)
			{//ÉÏº£¹æÔ¼
				//DI±È½Ï
				i=MRR(((u32)p8txbuff)+29+8,1);//·¢³öµÄÊı¾İ±êÊ¶
				x=MRR(((u32)p8rxbuff)+8,1);//·µ»ØµÄÊı¾İ±êÊ¶
				x|=0x80;
			}
			else
			{
				if((p8rxbuff[8]&0x40)!=0x0)
				{//¿ØÖÆÂë´ÓÕ¾»Ø´ğÒì³£
					goto NEXT_RMCount;//ÏÂ³­±íÃüÁî¼ÆÊı
				}
				//DI±È½Ï
				i=MRR(((u32)p8txbuff)+29+10,4);//·¢³öµÄÊı¾İ±êÊ¶
				x=MRR(((u32)p8rxbuff)+10,4);//·µ»ØµÄÊı¾İ±êÊ¶
				if(Protocol==1)
				{//1997
					i&=0xffff;
					x&=0xffff;
				}
			}
		#else
			if((p8rxbuff[8]&0x40)!=0x0)
			{//¿ØÖÆÂë´ÓÕ¾»Ø´ğÒì³£
				goto NEXT_RMCount;//ÏÂ³­±íÃüÁî¼ÆÊı
			}
			//DI±È½Ï
			i=MRR(((u32)p8txbuff)+29+10,4);//·¢³öµÄÊı¾İ±êÊ¶
			x=MRR(((u32)p8rxbuff)+10,4);//·µ»ØµÄÊı¾İ±êÊ¶
			if(Protocol==1)
			{//1997
				i&=0xffff;
				x&=0xffff;
			}
		#endif
			if(i!=x)
			{//DI²»ÏàÍ¬
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//Êı¾İÓò¼õ0x33
				p8rxbuff[10+i]-=0x33;
			}
			switch(Protocol)
			{
				case 1://1997
					p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
					if(p8RMLib[0]!=0x0)
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+2))
						{
							if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
							{//¿éÃüÁîºó¿ÉÄÜÓĞ0xaa
								if(p8rxbuff[9]==2)
								{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø2×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
									goto NEXT_RMCount;//ÏÂ³­±íÃüÁî¼ÆÊı
								}
								UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
								break;
							}
						}
					}
					p8RMLib-=10+((3*UARTCtrl->RMCount)+2);
					ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
					break;
			#if(USER/100)==5//ÉÏº£
				case 21:
					i=p8RMLib[10+(2*UARTCtrl->RMCount)+1];
					if(p8rxbuff[9]==i)
					{//·µ»Ø³¤¶ÈÏàÍ¬
						ReadMeter_ShH_FnData(PORTn,p8RMLib);//ÉÏº£¹æÔ¼³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
					}
					break;
			#endif
				default://2007
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//²»ÊÇ¶Áµç±íÇúÏß
						p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
					}
					else
					{//ÊÇ¶Áµç±íÇúÏß
						p8RMLib+=10+4;
					}
					if(p8RMLib[0]!=0x0)
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+4))
						{
							if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
							{//¿éÃüÁîºó¿ÉÄÜÓĞ0xaa
								if(p8rxbuff[9]==4)
								{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø4×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
									goto NEXT_RMCount;//ÏÂ³­±íÃüÁî¼ÆÊı
								}
								UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
								break;
							}
						}
					}
					if((p8RMLib[0]<0xe4)||(p8RMLib[0]>0xea))
					{//²»ÊÇ¶Áµç±íÇúÏß
						p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
					}
					else
					{
						p8RMLib-=10+4;
					}
					ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å				
					break;
			}
	NEXT_RMCount://ÏÂ³­±íÃüÁî¼ÆÊı
			UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊı+1
			UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			Terminal_Router->RouterRealTimeTask--;//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ
			break;
	}
}

/*
void KeyUserReadMeter_3762_Std(u32 PORTn)//ÔØ²¨Ä£¿é,ÖØµã»§³­±í
{
	u32 i;
	u32 x;
	u32 y;
	u32 Protocol;
	u32 Fn;
	u32 Pn;
	u64 d64a;
	u64 d64b;

	u32 ClassNo;
	u8 * p8RMLib;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u8 * p8FnData;
	UARTCtrl_TypeDef * UARTCtrl;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8FnData=(u8 *)Get_ADDR_UARTnFnDataBuff(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	
	switch(Terminal_Router->RouterKeyUserTask)//ÖØµã»§³­¶ÁÈÎÎñ
	{
		case 0://¼ì²éÊÇ·ñÓĞÖØµã»§ÈôÓĞÔòÔİÍ£Â·ÓÉ
			if(UARTCtrl->Task<=2)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{	
				y=MRR(ADDR_AFN04F35,1);//Ì¨Çø¼¯ÖĞ³­±íÖØµã»§ÉèÖÃ
				if(y>MaxKeyUser)
				{
					y=MaxKeyUser;
				}
				while(1)
				{
					if(UARTCtrl->KeyUserRMCount>=y)//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
					{//È«²¿ÖØµã»§³­Íê
						Terminal_Router->RouterKeyUserTask=0;//ÖØµã»§³­¶ÁÈÎÎñ
						UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfKeyUser;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
						UARTCtrl->FnEnd=UARTCtrl->FnEndBackupOfKeyUser;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø(Ô­³£¹æ³­±íµÄFn¼ÆÊı½áÊø±êÖ¾¼Ä´æ)
						UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485³­±íFn¼ÆÊı(Ô­³£¹æ³­±íµÄFn¼ÆÊı¼Ä´æ)
						UARTCtrl->RMError=UARTCtrl->RMErrorBackupOfKeyUser;//485³­±í±¾µçÄÜ±íÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü(Ô­³£¹æ³­±íµÄÊ§°Ü±êÖ¾¼Ä´æ)
						UARTCtrl->KeyUserRMCount=0;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
						UARTCtrl->RMKeyUser=0;//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
						return;
					}
					x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//ÖØµã»§µÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ
					if((x==0)||(x>AFN04F10MaxPn))
					{//×°ÖÃĞòºÅ´í
				NEXT_KeyUser_Start:
						UARTCtrl->FnCount=0;//Fn¼ÆÊı
						UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
						UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
						UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
						continue;
					}
					UARTCtrl->MeterNo=x;
					x--;
					x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
					Pn=MRR(x,2);
					if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
					{//²âÁ¿µãºÅ´í
						goto NEXT_KeyUser_Start;//ÏÂÒ»¸öÖØµã»§
					}
					i=MRR(x+2,1);
					i&=0x1f;
					if(i!=31)
					{//¶Ë¿Ú²»ÊÇÔØ²¨
						goto NEXT_KeyUser_Start;//ÏÂÒ»¸öÖØµã»§
					}
					Protocol=MRR(x+3,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
					if(Protocol!=1)
					{//²»ÊÇDL/T645-1997,2007³­Õûµã¶³½á
						goto NEXT_KeyUser_Start;//ÏÂÒ»¸öÖØµã»§
					}
					break;
				}
				UARTCtrl->Task=0;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				Terminal_Router->StopFlags|=(1<<1);//ÔİÍ£Â·ÓÉ:b1=³­ÖØµã»§
				Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				Terminal_Router->RouterKeyUserTask++;//ÖØµã»§³­¶ÁÈÎÎñ
			}
			break;
		case 1://AFN13 F1 Â·ÓÉÊı¾İ×ª·¢:¼à¿ØÔØ²¨´Ó½Úµã
			i=MRR(ADDR_AFN04F35,1);//Ì¨Çø¼¯ÖĞ³­±íÖØµã»§ÉèÖÃ
			if(i>MaxKeyUser)
			{
				i=MaxKeyUser;
			}
			if(UARTCtrl->KeyUserRMCount>=i)//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
			{//È«²¿ÖØµã»§³­Íê
				Terminal_Router->RouterKeyUserTask=0;//ÖØµã»§³­¶ÁÈÎÎñ
//				UARTCtrl->KeyUserRMOKCountLast=UARTCtrl->KeyUserRMOKCount;//ÉÏ´ÎÖØµã»§³­¶Á³É¹¦¼ÆÊı
//				UARTCtrl->KeyUserRMOKCount=0;//µ±Ç°ÖØµã»§³­¶Á³É¹¦¼ÆÊı
				UARTCtrl->MeterNo=UARTCtrl->MeterNoBackupOfKeyUser;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
				UARTCtrl->FnEnd=UARTCtrl->FnEndBackupOfKeyUser;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø(Ô­³£¹æ³­±íµÄFn¼ÆÊı½áÊø±êÖ¾¼Ä´æ)
				UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485³­±íFn¼ÆÊı(Ô­³£¹æ³­±íµÄFn¼ÆÊı¼Ä´æ)
				UARTCtrl->RMError=UARTCtrl->RMErrorBackupOfKeyUser;//485³­±í±¾µçÄÜ±íÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü(Ô­³£¹æ³­±íµÄÊ§°Ü±êÖ¾¼Ä´æ)
				UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
				UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
				UARTCtrl->KeyUserRMCount=0;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				UARTCtrl->RMKeyUser=0;//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
				
				Terminal_Router->StopFlags&=~(1<<1);//»Ö¸´Â·ÓÉ
				Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
				return;
			}
			x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//ÖØµã»§µÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ
			if((x==0)||(x>AFN04F10MaxPn))
			{//×°ÖÃĞòºÅ´í
			NEXT_KeyUser:
				UARTCtrl->FnCount=0;//Fn¼ÆÊı
				UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
				UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
				UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				break;
			}
			UARTCtrl->MeterNo=x;
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MRR(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//²âÁ¿µãºÅ´í
				goto NEXT_KeyUser;//ÏÂÒ»¸öÖØµã»§
			}
			i=MRR(x+2,1);
			i&=0x1f;
			if(i!=31)
			{//¶Ë¿Ú²»ÊÇÔØ²¨
				goto NEXT_KeyUser;//ÏÂÒ»¸öÖØµã»§
			}
			Protocol=MRR(x+3,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[UARTCtrl->FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[UARTCtrl->FnCount];
			}
			if(i==0)
			{//±¾FnÊÇ¿Õ
			NEXT_Fn:
				UARTCtrl->FnCount+=1;//Fn¼ÆÊı+1
				UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊı=0
				UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
				UARTCtrl->RMError&=0xfe;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				if(Protocol==1)
				{//DL/T645-1997
					i=Get_RM_DL645_1997_MaxFn();
				}
				else
				{//DL/T645-2007
					i=Get_RM_DL645_2007_MaxFn();
				}
				if(UARTCtrl->FnCount>=i)
				{//Fn¼ÆÊı>=×î´óFn¸öÊı,±¾±í³­±í½áÊø
			//END_Fn:
					UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
					if(UARTCtrl->RMError==0)//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					{
//						UARTCtrl->KeyUserRMOKCount++;//µ±Ç°ÖØµã»§³­¶Á³É¹¦¼ÆÊı
					}
					UARTCtrl->RMError=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
				}
				break;
			}
			p8RMLib=(u8 *)i;//³­±í¿â
			if((p8RMLib[3]&0x04)==0)//³­±íÖ§³Ö:B2=ÔØ²¨³­±í,B1=RS485³­±í,B0=ÄÚ²¿½»²É³­±í
			{//±¾Fn²»ÊÇÔØ²¨³­±í
				goto NEXT_Fn;
			}
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//È¡´óĞ¡ÀàºÅ
			Fn=p8RMLib[0];
			y=0;
			for(i=0;i<Get_KeyUserReadMeterLib_MaxFn();i++)
			{
				if(KeyUserReadMeterLib[i].AFN0CFn==Fn)//Òª³­¶Á1ÀàÊµÊ±Êı¾İFn,Îª0±íÊ¾ÎŞĞ§²»³­
				{
					x=KeyUserReadMeterLib[i].AFN0DFn;//ÖØµã»§ÇúÏßFn
					y|=Class2Data_Fn(ClassNo,x);//2ÀàÊı¾İFnÅäÖÃ,Èë¿ÚClassNoµÄD7-D4´óÀàºÅD3-D0Ğ¡ÀàºÅ,Fn;·µ»Ø0=²»Ö§³Ö,1=Ö§³Ö
				}
			}
			if(y==0x0)
			{//±¾Fn²»³­
				goto NEXT_Fn;
			}
			i=GetClass1DataAddr(Fn,Pn);//È¡Àà1Êı¾İ´æÖüµØÖ·,·µ»Ø0±íÊ¾Ã»ÕÒµ½,ÓĞĞ§Êı¾İ³¤¶È·ÅADDR_Terminal_ClassDataLen
			if(i==0)
			{//Ã»ÕÒµ½
				goto NEXT_Fn;
			}
			i=MRR(i+1,4);//È¡Ô­³­±íÊ±¼äÄêÔÂÈÕÊ±
			x=MRR(ADDR_TYMDHMS+2,4);//È¡µ±Ç°³­±íÊ±¼äÄêÔÂÈÕÊ±
			if(i==x)
			{//ÒÑ³­
				goto NEXT_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//³­±íÃüÁî¼ÆÊı=0,ÇåFnÊı¾İ»º³å
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//³­±íÃüÁî¼ÆÊı>=FnĞè·¢ÃüÁîÊı;±¾Fn³­±íÍê³É
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,0);//´æÖü³­±íFnÊı¾İ;ÈôÖ»´æ´¢ÊµÊ±Êı¾İu8 *p8RMLib=0	
				goto NEXT_Fn;	
			}
			if(UARTCtrl->RMError!=0x0)
			{//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,0);//´æÖü³­±íFnÊı¾İ;ÈôÖ»´æ´¢ÊµÊ±Êı¾İu8 *p8RMLib=0
				goto NEXT_Fn;
			}
			
			d64a=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+6,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
			if(Protocol==1)
			{//dl645-1997
				i=MRR((u32)&p8RMLib[10+(3*UARTCtrl->RMCount)],2);//È¡DI
				Read_DL645_1997_TxData(ADDR_DATABUFF,i,d64a);//DL645-1997¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
				//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
				i=p8RMLib[10+(3*UARTCtrl->RMCount)+2];//·µ»Ø³¤¶È
				if(i==0)
				{//·µ»Ø³¤¶È=0
					i=10+(3*p8RMLib[1])+UARTCtrl->RMCount;
					i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
				}
				i+=14;
				Uart_3762_AFN13F1_Tx(PORTn,1,ADDR_DATABUFF,14,i,0);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı,Í¨ĞÅÑÓÊ±Ïà¹Ø±êÖ¾
			}
			else
			{//dl645-2007
				goto NEXT_KeyUser;//ÏÂÒ»¸öÖØµã»§,dl645-2007³­Õûµã¶³½á

//				i=MRR((u32)&p8RMLib[10+(5*UARTCtrl->RMCount)],4);//È¡DI
//				Read_DL645_2007_TxData(ADDR_DATABUFF,i,d64a);//DL645-2007¶ÁÊı¾İÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êı¾İ±êÊ¶,Í¨ĞÅµØÖ·
//				//Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı
//				i=p8RMLib[10+(5*UARTCtrl->RMCount)+4];//·µ»Ø³¤¶È
//				if(i==0)
//				{//·µ»Ø³¤¶È=0
//					i=10+(5*p8RMLib[1])+UARTCtrl->RMCount;
//					i=p8RMLib[i];//È¡Ô¤¼Æ·µ»Ø×Ö½ÚÊı
//				}
//				i+=16;
//				Uart_3762_AFN13F1_Tx(PORTn,2,ADDR_DATABUFF,16,i);//376.2¼à¿Ø´Ó½ÚµãÊı¾İÌîÈë·¢ËÍÇø;Èë¿Ú:¶Ë¿Ú,Í¨ĞÅĞ­Òé(1=1997,2=2007),±ê×¼645Ö¡µØÖ·,645Ö¡³¤¶È,Ô¤¼ÆÓ¦´ğ×Ö½ÚÊı

			}		
			UARTCtrl->TxByte=31+p8txbuff[28];//·¢ËÍ×Ö½ÚÊı(°ë×Ö¶ÔÆë)
			i=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
			UARTCtrl->TxToRxTimerOver=i;
			UARTCtrl->Task=3;
			Terminal_Router->RouterKeyUserTask++;//ÖØµã»§³­¶ÁÈÎÎñ
			//ÏÔÊ¾³­±íµØÖ·
			CopyString((u8 *)"ÔØ²¨ÖØµã",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
			for(i=0;i<6;i++)
			{
				x=p8txbuff[16+5-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
			break;
		case 2://½ÓÊÕ
			if(UARTCtrl->Task>=3)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{
				break;
			}
			if(UARTCtrl->Task!=1)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
			{//³¬Ê±
				if(UARTCtrl->ReRMCount<1)//485³­±íÃüÁîÖØ·¢¼ÆÊı
				{
					UARTCtrl->ReRMCount++;//485³­±íÃüÁîÖØ·¢¼ÆÊı
				}
				else
				{
					UARTCtrl->RMError|=0x1;//±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
				}
				Terminal_Router->RouterKeyUserTask--;//ÖØµã»§³­¶ÁÈÎÎñ
				break;
			}
			i=p8rxbuff[1]+(p8rxbuff[2]<<8);
			if(i<=16)
			{//·ñÈÏ»Ø´ğ
		FnErr:
				UARTCtrl->Task=0;//¿ÕÏĞ1´Î
				UARTCtrl->KeyUserRMCount++;//ÖØµã»§³­¶ÁµçÄÜ±í¼ÆÊı
				Terminal_Router->RouterKeyUserTask--;//ÖØµã»§³­¶ÁÈÎÎñ
				UARTCtrl->FnCount=0;//Fn¼ÆÊı
				UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
				UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
				break;
			}
			x=MRR(ADDR_AFN04F35+1+(2*UARTCtrl->KeyUserRMCount),2);//ÖØµã»§µÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ
			if((x==0)||(x>AFN04F10MaxPn))
			{//×°ÖÃĞòºÅ´í
				goto FnErr;
			}
			UARTCtrl->MeterNo=x;
			x--;
			x=ADDR_AFN04F10+2+(LEN_AFN04F10_Pn*x);
			Pn=MRR(x,2);
			if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
			{//²âÁ¿µãºÅ´í
				goto FnErr;
			}
			Protocol=MRR(x+3,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			if(Protocol==1)
			{//DL/T645-1997
				i=DL645_1997_RMLib[UARTCtrl->FnCount];
			}
			else
			{//DL/T645-2007
				i=DL645_2007_RMLib[UARTCtrl->FnCount];
			}
			if(i==0)
			{//±¾FnÊÇ¿Õ
				goto FnErr;
			}
			p8RMLib=(u8 *)i;//³­±í¿â
			d64a=MRR(x+4,6);//È¡ÅäÖÃµÄÍ¨ĞÅµØÖ·
			y=p8rxbuff[28];//·µ»ØÊı¾İÓò³¤¶È
			i=MRR(((u32)p8rxbuff)+22,3);
			if(i!=0x113)
			{//AFN=13H,Fn=1
				y=0;//½ÓÊÕÊı¾İ³¤¶È
			}
			if(y==0)
			{//½ÓÊÕÊı¾İ³¤¶È=0
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			MR((u32)p8rxbuff,((u32)p8rxbuff)+28+1,y);

			if((p8rxbuff[0]!=0x68)||(p8rxbuff[7]!=0x68))
			{//ÆğÊ¼·û0x68´í
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			d64b=MRR(((u32)p8rxbuff)+1,6);//·µ»ØµÄµØÖ·
			if(d64a!=d64b)
			{//±íµØÖ·²»ÏàÍ¬
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			x=0;//ºÍĞ£Ñé¼ÆËã
			for(i=0;i<(p8rxbuff[9]+10);i++)
			{
				x+=p8rxbuff[i];
			}
			x&=0xff;
			if(x!=p8rxbuff[i])
			{//ºÍĞ£Ñé´í
				UARTCtrl->Task=2;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				break;
			}
			if((p8rxbuff[8]&0x40)!=0x0)
			{//¿ØÖÆÂë´ÓÕ¾»Ø´ğÒì³£
				goto NEXT_RMCount;//ÏÂ³­±íÃüÁî¼ÆÊı
			}
			for(i=0;i<p8rxbuff[9];i++)
			{//Êı¾İÓò¼õ0x33
				p8rxbuff[10+i]-=0x33;
			}
			if(Protocol==1)
			{//1997
				p8RMLib+=10+((3*UARTCtrl->RMCount)+2);
				if(p8RMLib[0]!=0x0)
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+2))
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+2+1))
						{//¿éÃüÁîºó¿ÉÄÜÓĞ0xaa
							if(p8rxbuff[9]==2)
							{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø2×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
								goto NEXT_RMCount;//ÏÂ³­±íÃüÁî¼ÆÊı
							}
							UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
							break;
						}
					}
				}
				p8RMLib-=10+((3*UARTCtrl->RMCount)+2);
				ReadMeter_DL645_1997_FnData(PORTn,p8RMLib);//DL645_1997³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
			}
			else
			{//2007
				p8RMLib+=10+((5*UARTCtrl->RMCount)+4);
				if(p8RMLib[0]!=0x0)
				{
					if(p8rxbuff[9]!=(p8RMLib[0]+4))
					{
						if(p8rxbuff[9]!=(p8RMLib[0]+4+1))
						{//¿éÃüÁîºó¿ÉÄÜÓĞ0xaa
							if(p8rxbuff[9]==4)
							{//µç¿ÆÔº²âÊÔÈí¼şµ±ÊÂ¼ş¼ÇÂ¼ÎŞÊı¾İÊ±,Ö»·µ»Ø4×Ö½ÚÊı¾İ±êÊ¶,Í¬´ÓÕ¾Òì³£»Ø´ğ
								goto NEXT_RMCount;//ÏÂ³­±íÃüÁî¼ÆÊı
							}
							UARTCtrl->Task=2;//2=½ÓÊÕ³¬Ê±(»ò´íÎó)
							break;
						}
					}
				}
				p8RMLib-=10+((5*UARTCtrl->RMCount)+4);
				ReadMeter_DL645_2007_FnData(PORTn,p8RMLib);//DL645_2007³­±í½ÓÊÕÊı¾İÌîÈëFnÊı¾İ»º³å
			}
	NEXT_RMCount://ÏÂ³­±íÃüÁî¼ÆÊı
			UARTCtrl->RMCount+=1;//³­±íÃüÁî¼ÆÊı+1
			UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			UARTCtrl->Task=0;//¿ÕÏĞ1´Î
			Terminal_Router->RouterKeyUserTask--;//ÖØµã»§³­¶ÁÈÎÎñ
			break;
	}
}
*/



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


