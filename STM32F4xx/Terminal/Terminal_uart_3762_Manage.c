
#include "Project.h"
#include "Terminal_Uart_3762_Manage.h"
#include "Terminal_Uart_3762_RxTx.h"
#include "Terminal_ReadMeter_3762.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"

#include "Terminal_ReadMeter_3762_Std.h"
#include "AutoSearchMeter_3762.h"
#include "BatchTask_3762.h"
#include "KeyUser.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_ReadMeter_Event.h"
#include "RouterBroadcastTime_Collector.h"



#if (Project/100)==2//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=

void Terminal_AFN0CF11_3762(void)//ÖÕ¶ËÀà1Êı¾İF11,ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u8 *p8;

#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
	i=MRR(ADDR_TYMDHMS+1,2);
	if(i<RMM_StartHM)
	{
		return;
	}
#endif
	KeyUserPnList();//¼¯ÖĞ³­±íÖØµã»§ÇúÏß¶³½áPnÁĞ±í
	n=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı(°ë×Ö¶ÔÆë)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	x=0;//³É¹¦Êı
	y=0;//Ê§°ÜÊı
	w=0;//ÖØµã»§³É¹¦Êı
	for(i=0;i<n;i++)
	{
	#if DateRMTask==0//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
		z=Terminal_Note->List[i].RMCompleteS[MaxRMTask-1];//¼ÇÑ­³­ÈÎÎñºÅ
	#else
		z=Terminal_Note->List[i].RMCompleteS[1];//Ö»¼ÇÈÎÎñºÅ1
	#endif
		if(z==2)
		{
			x++;//³É¹¦Êı
			if(KeyUserPnSet(Terminal_Note->List[i].Pn))//ÖØµã»§ÓĞĞ§²âÁ¿µãºÅ¼ì²é;·µ»Ø0=ÎŞĞ§,1=ÓĞĞ§;Ê¹ÓÃÖØµã»§ÇúÏß¶³½áPnÁĞ±íADDR_KeyUserPnList
			{
				w++;//ÖØµã»§³­±í³É¹¦Êı
			}
		}
		else
		{
			if(z!=0)
			{
				y++;//Ê§°ÜÊı
			}
		}
	}

	CopyString((u8 *)"ÔØ²¨____³É____°Ü____",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
	//×ÜÊı
	p8=(u8*)(ADDR_DATABUFF+4+3);
	p8[0]='0';
	i=hex_bcd(n);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	//³É¹¦Êı
	p8=(u8*)(ADDR_DATABUFF+10+3);
	p8[0]='0';
	i=hex_bcd(x);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}	
	//Ê§°ÜÊı
	p8=(u8*)(ADDR_DATABUFF+16+3);
	p8[0]='0';
	p8[1]=0;
	i=hex_bcd(y);
	while(i)
	{
		p8[0]=(i&0xf)+0x30;
		p8--;
		i>>=4;
	}
	WarningOccur(100,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
	
	p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
	p8[0]=3;//±¾ÏîÊı¾İ¿é¸öÊın	BIN	1(1-31) 
	p8[1]=2;//¶Ë¿ÚºÅ2
	p8[19+1]=3;//¶Ë¿ÚºÅ3
	p8[38+1]=31;//ÖÕ¶ËÍ¨ĞÅ¶Ë¿ÚºÅ	BIN	1
	p8+=(2*19);
	MWR(n,((u32)p8)+2,2);//Òª³­µç±í×ÜÊı	BIN	2
	if((x==n)&&(n!=0))
	{
		p8[4]|=2;//D0=1ÕıÔÚ³­±í,D1=1Ê±¶ÎÄÚÍê³É
		if(p8[14+3]==0xee)
		{//½áÊøÊ±¼äÎ´Ìî
			MR(((u32)p8)+14,ADDR_AFN0CF2,6);
		}
	}
	else
	{
		p8[4]&=0xfd;//D0=1ÕıÔÚ³­±í,D1=0Ê±¶ÎÄÚÎ´Íê³É
		MC(0xee,((u32)p8)+14,6);//³­±í½áÊøÊ±¼ä	¼û¸½Â¼A.1	6
	}
	MWR(x,((u32)p8)+5,2);//³­±í³É¹¦¿éÊı	BIN	2
	MWR(w,((u32)p8)+7,1);//³­ÖØµã±í³É¹¦¿éÊı	BIN	1

	//ÔİÍ£Â·ÓÉ±êÖ¾:
	//b0=³­±í¼ä¸ô¶¨Ê±,b1=³­ÖØµã»§,b2=Êı¾İ×ª·¢,b3=UART¶ÁÂ·ÓÉĞÅÏ¢,b4=¹ã²¥Ğ£Ê±,b5=¹ã²¥¶³½á,b6=Í¨¶Ïµç¿ØÖÆ,b7=ÊµÊ±³­¶Á
	//b8=µµ°¸Í¬²½,b9=ÅúÁ¿¶ÔÊ±,b10=ÃüÁîÔİÍ£³­±í,b11=²»ÔÊĞí×Ô¶¯³­±í,b12=·Ç³­±íÊ±¶Î,b13=×Ô¶¯ËÑ±í,b14=µÈ´ıÊ±ÖÓ¹ı0,b15=ÕıÔÚ¶³½á
	//b16=Â·ÓÉÈí¼ş¸üĞÂ,b17=,b18=,b19=,b20=,b21=,b22=,b23=
	if(((Terminal_Router->StopFlags&0xFFFFFF7D)==0)&&(Terminal_Router->RouterInitTask>=200)&&(Terminal_Router->NoteNum!=0))
	{
		p8[4]|=1;//D0=1,³­±í
		//³­±í¿ªÊ¼Ê±¼ä	¼û¸½Â¼A.1	6
		if(p8[8+3]==0xee)
		{//ÎŞ¿ªÊ¼Ê±¼ä
			MR(((u32)p8)+8,ADDR_AFN0CF2,6);
		}
	}
	else
	{
		p8[4]&=0xfe;//D0=0,Î´³­±í
	}
	//³­±í½áÊøÊ±¼ä	¼û¸½Â¼A.1	6
	
/*	
	//ÏÔÊ¾ÎŞÏßĞÅµÀ
	if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
	{
		CopyString((u8 *)"ĞÅµÀ×é------·¢Éä¹¦ÂÊ",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
		p8=(u8*)(ADDR_DATABUFF);
		p8[20]=0;
		i=Terminal_Router->RadioChannel;
		i=hex_bcd(i);
		p8[6]=((i>>4)&0xf)+0x30;
		p8[7]=(i&0xf)+0x30;
		i=Terminal_Router->RadioPower;//ÎŞÏßÖ÷½Úµã·¢Éä¹¦ÂÊ:00£º×î¸ß·¢Éä¹¦ÂÊ£¬01£º´Î¸ß·¢Éä¹¦ÂÊ£¬02£º´ÎµÍ·¢Éä¹¦ÂÊ£¬03£º×îµÍ·¢Éä¹¦ÂÊ£¬04¡«254£º±£Áô£¬255£º±£³Ö²»±ä
		switch(i)
		{
			case 0://×î¸ß·¢Éä¹¦ÂÊ
				CopyString((u8 *)"×î¸ß",(u8*)ADDR_DATABUFF+8);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
				break;
			case 1://´Î¸ß·¢Éä¹¦ÂÊ
				CopyString((u8 *)"´Î¸ß",(u8*)ADDR_DATABUFF+8);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
				break;
			case 2://´ÎµÍ·¢Éä¹¦ÂÊ
				CopyString((u8 *)"´ÎµÍ",(u8*)ADDR_DATABUFF+8);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
				break;
			case 3://×îµÍ·¢Éä¹¦ÂÊ
				CopyString((u8 *)"×îµÍ",(u8*)ADDR_DATABUFF+8);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
				break;
		}
		WarningOccur(101,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
	}
*/
}


u32 NoRMCompletes_3762(u32 RMTask)//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
{
	u32 i;
	u32 n;
	u32 x;
	u32 y;
	u32 z;
	
	n=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı(°ë×Ö¶ÔÆë)
	if(RMTask!=0)
	{//²»ÊÇÆô³­
		if(n==0)
		{//ÒÑÖª×Ü´Ó½ÚµãÊı=0
			return 1;//·µ»Øb0-b15Î´³­Êı!=0ÒÔ±¸ÅäÖÃ±íºó¼´¿ªÊ¼ÈÕ³­
		}
	}
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	x=0;//Î´³­Êı
	y=0;//Ê§°ÜÊı
	for(i=0;i<n;i++)
	{
		z=Terminal_Note->List[i].RMCompleteS[RMTask];//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
		if(z==0)
		{
			x++;//Î´³­Êı
		}
		else
		{
			if(z==1)
			{
				y++;//Ê§°ÜÊı
			}
		}
	}
	return (x+(y<<16));
}

void ClrRMTaskComplete_3762(u32 RMTask,u32 All)//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
{
	u32 i;
	u32 n;
	
	n=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı(°ë×Ö¶ÔÆë)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	for(i=0;i<n;i++)
	{
		if(All==0)
		{//Ö»ÇåÊ§°Ü
			if(Terminal_Note->List[i].RMCompleteS[RMTask]==1)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
			{
				Terminal_Note->List[i].RMCompleteS[RMTask]=0;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
			}
		}
		else
		{//ÇåÈ«²¿
			Terminal_Note->List[i].RMCompleteS[RMTask]=0;
		}
	}
}

u32 NewTask_3762(u32 PORTn)//3762¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
{
	u32 i;
	u32 n;
  UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	u32 x;
	u32 y;
	u16 *p16timer;
	
	if(Terminal_Router->RouterInitTask<200)
	{
		return 0;
	}
	if(Terminal_Router->RouterCtrl!=0)//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
	{
		return 0;
	}
	if(Terminal_Router->StopFlags&0xfffffffc)//ÔİÍ£Â·ÓÉ±êÖ¾:b0=³­±í¼ä¸ô¶¨Ê±,b1=³­ÖØµã»§
	{//Â·ÓÉÓĞ·Ç³­±íÀàÔİÍ£
		return 0;
	}
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if(UARTCtrl->Lock==0x55)//0x55=Í¨ĞÅ¹¦ÄÜÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
	{
		return 0;
	}
	if(((UARTCtrl->RMMeterEvent&1)!=0)||(UARTCtrl->SubsequentFrames!=0))
	{//ÕıÔÚ³­¶ÁµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş»ò¶ÁºóĞøÖ¡
		return 0;
	}
	
	n=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı(°ë×Ö¶ÔÆë)
	if(n>MaxRS485AddCarrierPn)
	{
		n=MaxRS485AddCarrierPn;
	}
	if(UARTCtrl->RMTaskN&0x10)//ÓĞÃ¿Ğ¡Ê±ÇúÏß³­¶ÁÈÎÎñ
	{//ÓĞĞÂÃ¿Ğ¡Ê±ÇúÏß³­¶ÁÈÎÎñ,²ÉÓÃµã³­
		if((UARTCtrl->RMTaskC!=0)&&(UARTCtrl->RMTaskC!=4)&&(UARTCtrl->RMCount==0))
		{//Ô­ÈÎÎñ²»ÊÇÆô³­»ò4=Ã¿Ğ¡Ê±ÇúÏßÈÎÎñ
			UARTCtrl->RMMeterEvent=0;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
			UARTCtrl->RMTaskN&=0xef;//È¥ĞÂÈÎÎñ
			UARTCtrl->RMTaskCOfKeyUser=UARTCtrl->RMTaskC;
			UARTCtrl->RMTaskC=4;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].RMCompleteS[4]=0;
			}
			UARTCtrl->MeterNoBackupOfKeyUser=UARTCtrl->MeterNo;//485³­±íÔ­Ñ­³­ÈÎÎñµÄÅäÖÃĞòºÅ¼ÆÊı(°ë×Ö¶ÔÆë)
			UARTCtrl->FnCountBackupOfKeyUser=UARTCtrl->FnCount;//485³­±íÔ­Ñ­³­ÈÎÎñµÄFn¼ÆÊı
			UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
			UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
			UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
			UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].FnCountSOfKeyUser=Terminal_Note->List[i].FnCount;//²åÈëÃ¿Ğ¡Ê±³­¶ÁÇúÏßÈÎÎñÇ°Ô­Ñ­³­ÈÎÎñµÄ³­±íFn¼ÆÊı
				Terminal_Note->List[i].FnCount=0;//³­±íFn¼ÆÊı
				Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
				Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
				Terminal_Note->List[i].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
			}
			UARTCtrl->RMFlag=1;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
			return 1;
		}
	}
#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
	if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
	{//Ö»ÓĞÑ­³­Ê±»ò³­¶ÁµçÄÜ±íÊÂ¼şÊ±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
		if(UARTCtrl->DateReRMCount!=0)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
		{
			NewRMEvene(PORTn);//ĞÂ»ò»Ö¸´¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ;·µ»Ø0=Ã»ĞÂ½øÈë,1=ĞÂ½øÈë
		}
	}
#endif
	if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
	{//Ñ­³­Ê±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
		if(UARTCtrl->RMTaskN&2)//ĞÂÃ¿ÈÕÈÎÎñ»ò²¹³­ÈÎÎñ
		{
		ReTask:
			UARTCtrl->RMTaskN&=0xfd;//È¥ĞÂÃ¿ÈÕÈÎÎñ
			UARTCtrl->RMTaskC=1;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			if(UARTCtrl->DateReRMCount!=0xff)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
			{
				UARTCtrl->DateReRMCount++;//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
			}
			for(i=0;i<n;i++)
			{
				if(Terminal_Note->List[i].RMCompleteS[1]==1)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				{//Ö»Çå²»³É¹¦µÄ
					Terminal_Note->List[i].RMCompleteS[1]=0;
				}
			}
		NewTask:
			UARTCtrl->RMMeterEvent=0;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
			UARTCtrl->MeterNoS=UARTCtrl->MeterNo;//485³­±íÔ­Ñ­³­ÈÎÎñµÄÅäÖÃĞòºÅ¼ÆÊı(°ë×Ö¶ÔÆë)
			UARTCtrl->FnCountS=UARTCtrl->FnCount;//485³­±íÔ­Ñ­³­ÈÎÎñµÄFn¼ÆÊı
			UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
			UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
			UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
			UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
			for(i=0;i<n;i++)
			{
				Terminal_Note->List[i].FnCountS=Terminal_Note->List[i].FnCount;//²åÈëÈÎÎñÇ°Ô­Ñ­³­ÈÎÎñµÄ³­±íFn¼ÆÊı
				Terminal_Note->List[i].FnCount=0;//³­±íFn¼ÆÊı
				Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
				Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
				Terminal_Note->List[i].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
			}
			UARTCtrl->RMFlag=1;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
			//Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
			return 1;
		}
		if(UARTCtrl->RMTaskN&4)//ĞÂÃ¿ÖÜÈÎÎñ
		{//²ÉÓÃµã³­
			UARTCtrl->RMTaskN&=0xfb;//È¥ĞÂÃ¿ÖÜÈÎÎñ
			UARTCtrl->RMTaskC=2;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			ClrRMTaskComplete_3762(2,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
			goto NewTask;
		}
		if(UARTCtrl->RMTaskN&8)//ĞÂ¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ
		{//²ÉÓÃµã³­
			UARTCtrl->RMTaskN&=0xf7;//È¥ĞÂ¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ
			UARTCtrl->RMTaskC=3;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			goto NewTask;
		}
	}
	else
	{//»Ö¸´
		y=NoRMCompletes_3762(UARTCtrl->RMTaskC);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
		if((y&0xffff)==0)//Î´³­Êı=0
		{//µ±Ç°ÈÎÎñÍê³É
			//»Ö¸´
			UARTCtrl->RMMeterEvent=0;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
			if(UARTCtrl->RMTaskC!=0)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			{//²»ÊÇÉÏµçÆô³­
				//»Ö¸´
				if(UARTCtrl->RMTaskC!=4)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=Ã¿Ğ¡Ê±ÇúÏßÈÎÎñ,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
				{//²»ÊÇÃ¿Ğ¡Ê±³­ÖØµã»§ÇúÏß»Ö¸´
					i=UARTCtrl->MeterNoS;//485³­±íÔ­Ñ­³­ÈÎÎñµÄÅäÖÃĞòºÅ¼ÆÊı(°ë×Ö¶ÔÆë)
					if(i!=0)
					{
						i--;
					}
					UARTCtrl->MeterNo=i;
					UARTCtrl->FnCount=UARTCtrl->FnCountS;//485³­±íÔ­Ñ­³­ÈÎÎñµÄFn¼ÆÊı
					UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
					UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
					UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
					for(i=0;i<n;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountS;//²åÈëÈÎÎñÇ°Ô­Ñ­³­ÈÎÎñµÄ³­±íFn¼ÆÊı
						Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
						Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
						Terminal_Note->List[i].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					}
				#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
					DelRMEvene(PORTn);//É¾³ı¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ
					if(UARTCtrl->RMTaskC==(MaxRMTask-2))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
					{//»Ö¸´ºó»¹ÊÇ³­¶ÁÉÏµÍ1¼¶µÄµçÄÜ±íÊÂ¼ş
						return 1;
					}
					if(UARTCtrl->DateReRMCount!=0)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
					{//ÖÁÉÙÒÑ³­1ÂÖ
						y=NoRMCompletes_3762(1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
						x=y;
						x&=0xffff;
						y>>=16;//Ê§°ÜÊı
						y+=x;//¼ÓÎ´³­Êı
						x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı(°ë×Ö¶ÔÆë)
						if(x!=0)
						{
							y*=100;
							y/=x;//Ê§°ÜÂÊ
						}
					#if (USER/100)==5//ÉÏº£
						if(y<=2)//³­±í³É¹¦ÂÊ>=98%
						{
							y=0;//Ê§°ÜÂÊ=0
						}
					#endif
						x=Comm_Ram->TYMDHMS[2];//ÊµÊ±ÖÓµÄÊ±
					#if (USER/100)==11//¼ªÁÖÏµ
						if(x>=0x09)
						{
							if(y<=2)//³­±í³É¹¦ÂÊ>=98%
							{
								x=0x12;//Ê±ÖÓµÈÍ¬12µã
								y=0;//Ê§°ÜÂÊ=0
							}
						}
						else
						{
							y=1;//Ê§°ÜÂÊ=1
						}
					#endif
						if((y==0)||(x>=0x12))
						{//³­±í³É¹¦ÂÊ100%»òÊ±ÖÓ>=12µã
							UARTCtrl->RMTaskC=(MaxRMTask-1);//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
						#if((USER/100)==5)//ÉÏº£
							RMEveneTimer_ShH(PORTn,y);//ÉÏº£¼¯ÖĞÆ÷·Ö¼¶¶ÁµçÄÜ±íÊÂ¼ş¶¨Ê±
						#endif
							NewRMEvene(PORTn);//ĞÂ»ò»Ö¸´¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ;·µ»Ø0=Ã»ĞÂ½øÈë,1=ĞÂ½øÈë
							if(UARTCtrl->RMTaskC==(MaxRMTask-2))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
							{//ÓÖĞÂ½øÈëµçÄÜ±íÊÂ¼ş
								return 1;
							}
						}
					}
				#endif
					if(UARTCtrl->RMTaskN&4)//ĞÂÃ¿ÖÜÈÎÎñ
					{//²ÉÓÃµã³­
						UARTCtrl->RMTaskN&=0xfb;//È¥ĞÂÃ¿ÖÜÈÎÎñ
						UARTCtrl->RMTaskC=2;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
						ClrRMTaskComplete_3762(2,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
						goto NewTask;//ÔÚ´Ë½øÈë,²»½øÈëÑ­³­ÃâÖØÆôÂ·ÓÉ¶øÓ°ÏìÂ·ÓÉ½¨Á¢
					}
					if(UARTCtrl->RMTaskN&8)//ĞÂ¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ
					{//²ÉÓÃµã³­
						UARTCtrl->RMTaskN&=0xf7;//È¥ĞÂ¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ
						UARTCtrl->RMTaskC=3;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
						goto NewTask;//ÔÚ´Ë½øÈë,²»½øÈëÑ­³­ÃâÖØÆôÂ·ÓÉ¶øÓ°ÏìÂ·ÓÉ½¨Á¢
					}
					y=NoRMCompletes_3762(1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
					y>>=16;//Ê§°ÜÊı
				#if((USER/100)!=5)//ÉÏº£ÓÃ»§
					if(y!=0)
					{//ÓĞÊ§°Ü,Á¢¼´²¹³­,²»½øÈëÑ­³­ÃâÖØÆôÂ·ÓÉ¶øÓ°ÏìÂ·ÓÉ½¨Á¢
					#if (USER/100)!=0//Î¢¹¦ÂÊÎŞÏßµç¿ÆÔºËÍÑù²»ÖØĞÂ×éÍø,ÒÔÃâÓ°Ïì³­±í³É¹¦ÂÊ²âÊÔ
						//²»ÊÇÎ¢¹¦ÂÊÎŞÏßµç¿ÆÔºËÍÑù
						if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
						{//Î¢¹¦ÂÊÎŞÏß
							if((Terminal_Router->RUNState&1)!=0)//µ±Ç°Â·ÓÉÔËĞĞ×´Ì¬(´ÓÄ£¿éÖĞ¶ÁÈ¡Öµ):b2=ÉÏ±¨ÊÂ¼ş±êÖ¾,b1=¹¤×÷±êÖ¾,b0=Â·ÓÉÍê³É±êÖ¾
							{//µ±Ç°²»ÊÇÕıÔÚ×éÍø
								if((Comm_Ram->TYMDHMS[2]<=6)||(Comm_Ram->TYMDHMS[2]>=0x20))
								{//ÔÚÊ±ÖÓ0-6µã»ò20µã-24µãÆô¶¯ÖØĞÂ×éÍø
									Terminal_Router->RouterCtrl=6;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´ÑÓÊ±,4=¶Á×´Ì¬,5=»Ö¸´,6=Æô¶¯×éÍø;Íê³Éºó×Ô¶¯»Øµ½0
									return 0;
								}
							}
						}
					#endif
						UARTCtrl->MeterNo++;
						goto ReTask;
					}
				#else
					//ÉÏº£ĞèÒª½øÈëÑ­³­,ÆäËûµØÇøÈôÈÕ¶³½áÓĞÊ§°Ü£¬Á¢¼´²¹³­,²»½øÈëÑ­³­ÃâÖØÆôÂ·ÓÉ¶øÓ°ÏìÂ·ÓÉ½¨Á¢
					if(UARTCtrl->RMTaskC==(MaxRMTask-1))
					{//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
						x=NoRMCompletes_3762(MaxRMTask-1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
						if(x==0)
						{//È«²¿³­³É¹¦
							ClrRMTaskComplete_3762(MaxRMTask-1,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
						}
						else
						{
							x&=0xffff;
							if(x==0)
							{//È«²¿ÒÑ³­µ«ÓĞÊ§°Ü
								ClrRMTaskComplete_3762(MaxRMTask-1,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
								goto SupplementRM;
							}
						}
					}
					else
					{
					SupplementRM:	
						if(y!=0)
						{//ÓĞÊ§°Ü,Á¢¼´²¹³­,²»½øÈëÑ­³­ÃâÖØÆôÂ·ÓÉ¶øÓ°ÏìÂ·ÓÉ½¨Á¢
						#if (USER/100)!=0//Î¢¹¦ÂÊÎŞÏßµç¿ÆÔºËÍÑù²»ÖØĞÂ×éÍø,ÒÔÃâÓ°Ïì³­±í³É¹¦ÂÊ²âÊÔ
							//²»ÊÇÎ¢¹¦ÂÊÎŞÏßµç¿ÆÔºËÍÑù
							if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËĞĞÄ£Ê½_Í¨ĞÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ĞÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎŞÏßÍ¨ĞÅ"£¬ÆäËüÈ¡Öµ±£Áô
							{//Î¢¹¦ÂÊÎŞÏß
								if((Terminal_Router->RUNState&1)!=0)//µ±Ç°Â·ÓÉÔËĞĞ×´Ì¬(´ÓÄ£¿éÖĞ¶ÁÈ¡Öµ):b2=ÉÏ±¨ÊÂ¼ş±êÖ¾,b1=¹¤×÷±êÖ¾,b0=Â·ÓÉÍê³É±êÖ¾
								{//µ±Ç°²»ÊÇÕıÔÚ×éÍø
									if((Comm_Ram->TYMDHMS[2]<=6)||(Comm_Ram->TYMDHMS[2]>=0x20))
									{//ÔÚÊ±ÖÓ0-6µã»ò20µã-24µãÆô¶¯ÖØĞÂ×éÍø
										Terminal_Router->RouterCtrl=6;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´ÑÓÊ±,4=¶Á×´Ì¬,5=»Ö¸´,6=Æô¶¯×éÍø;Íê³Éºó×Ô¶¯»Øµ½0
										return 0;
									}
								}
							}
						#endif
							UARTCtrl->MeterNo++;
							goto ReTask;
						}
					}
				#endif
				}
				else
				{//ÊÇÃ¿Ğ¡Ê±³­ÖØµã»§ÇúÏß»Ö¸´
					i=UARTCtrl->MeterNoBackupOfKeyUser;//485³­±íÔ­Ñ­³­ÈÎÎñµÄÅäÖÃĞòºÅ¼ÆÊı(°ë×Ö¶ÔÆë)
					if(i!=0)
					{
						i--;
					}
					UARTCtrl->MeterNo=i;
					UARTCtrl->FnCount=UARTCtrl->FnCountBackupOfKeyUser;//485³­±íÔ­Ñ­³­ÈÎÎñµÄFn¼ÆÊı
					UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
					UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
					UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
					for(i=0;i<n;i++)
					{
						Terminal_Note->List[i].FnCount=Terminal_Note->List[i].FnCountSOfKeyUser;//²åÈëÃ¿Ğ¡Ê±³­¶ÁÇúÏßÈÎÎñÇ°Ô­Ñ­³­ÈÎÎñµÄ³­±íFn¼ÆÊı
						Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
						Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
						Terminal_Note->List[i].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
					}
					UARTCtrl->RMTaskC=UARTCtrl->RMTaskCOfKeyUser;//½øÈëÖØµã»§³­¶ÁÈÎÎñÇ°Ô­ÈÎÎñ
					UARTCtrl->RMFlag=1;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
					return 0;
				}
			}
			else
			{//ÊÇÉÏµçÆô³­Íê³É
				if((Terminal_Ram->PowerTask==1)||(Terminal_Ram->PowerTask==10))//10=µÈ´ı³­±íÍê³É±È¶ÔµçÄÜ±í
				{//ÊÂ¼şÈÎÎñ:µÈ´ı³­±íÍê³É±È¶ÔµçÄÜ±í
					//i=NoRMCompletes_3762(UARTCtrl->RMTaskC);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
					//if(i!=0)
					//{//Ã»È«³­³É¹¦
						p16timer=(u16*)((u32)&Terminal_Ram->RS4854RM_S_Timer);//20 RS485-4³­±íÃë¶¨Ê±Æ÷(ÔØ²¨)
						if(p16timer[8]!=0)//Í£µçÊÂ¼ş³­¶ÁÊ±¼äÏŞÖµÃë¶¨Ê±Æ÷
						{
							ClrRMTaskComplete_3762(0,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
							return 0;
						}
					//}
				}
				//ÏÂ´Î³­Í£µç¼ÇÂ¼¼ä¸ôÊ±¼ä
				i=MRR(ADDR_AFN04F97+1,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô	BIN	1	µ¥Î»ÎªĞ¡Ê±
				p16timer=(u16*)(&Terminal_Ram->RS4854RMF246_M_Timer);//25 RS485-1Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô·Ö¶¨Ê±Æ÷
				p16timer[0]=i*60;//·Ö
				x=MRR(ADDR_TYMDHMS+2,4);
				x=YMDH_AddH(x,i);//ÄêÔÂÈÕÊ±¼ÓhÊ±,·µ»Ø¼ÓhÊ±ºóµÄÄêÔÂÈÕÊ±
				d64a=x;
				d64a<<=16;//
				MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//¶¨Ê±³­Í£µç¼ÇÂ¼¼ä¸ôÖÜÆÚÏÂ´Î³­±íÊ±¼äÃë·ÖÊ±ÈÕÔÂÄê
			#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
				goto ReTask;//½øÈëÈÕÈÎÎñ,Ãâ½øÈë1´ÎÑ­³­ºóÔÙ½øÈë
			#endif
			}
			Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
			UARTCtrl->RMTaskC=(MaxRMTask-1);//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
		}
	}
	return 0;
}


void Terminal_uart_3762_Manage(void)//376.2Â·ÓÉÄ£¿é¹ÜÀí
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u64 d64a;
	u8 *p8;
	u16 * p16;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 PORTn;
#if Extern_BatchTask_3762!=0//Íâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=
	u32 *p32;
#endif
	
	PORTn=RS485_4PORT;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);


#if PowerOnRMTask==0//µçÔ´¿ª³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
	if(UARTCtrl->RMTaskC==0)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
	{
		UARTCtrl->RMTaskC=(MaxRMTask-1);
	}
#endif
	
//·Ö¸üĞÂ
	if(UARTCtrl->RTCM!=Comm_Ram->TYMDHMS[1])
	{
		UARTCtrl->RTCM=Comm_Ram->TYMDHMS[1];
	#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
		RMEveneTimer(PORTn);//·Ö¼¶¶ÁµçÄÜ±íÊÂ¼ş¶¨Ê±
	#if((USER/100)==5)//ÉÏº£
		RMEveneTimer_ShH(PORTn,y);//ÉÏº£¼¯ÖĞÆ÷·Ö¼¶¶ÁµçÄÜ±íÊÂ¼ş¶¨Ê±
	#endif
	#endif
		//¹ã²¥Ğ£Ê±
		i=MRR(ADDR_AFN05F30+1,1);//ÖÕ¶Ë¶ÔµçÄÜ±í¶ÔÊ±¹¦ÄÜµÄ¿ªÆôÓë¹Ø±Õ
		if(i==1)
		{//0=¹Ø±Õ£¬1=¿ªÆô£¬ÆäËüÖµ¹Ø±Õ
			i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+1,1);//ÖÕ¶Ë³­±íÔËĞĞ²ÎÊıÉèÖÃ,¶Ë¿Ú31
			if((i&0x08)!=0x0)
			{//ÒªÇóÖÕ¶Ë¶¨Ê±¶Ôµç±í¹ã²¥Ğ£Ê±
				x=MRR(ADDR_TYMDHMS+1,3);//È¡µ±Ç°ÊµÊ±ÖÓÈÕÊ±·Ö
				y=MRR((ADDR_AFN04F33+((14+(4*24))*30))+10,3);//¶Ôµç±í¹ã²¥Ğ£Ê±¶¨Ê±Ê±¼äÈÕÊ±·Ö
				if((y>>16)==0x0)
				{//µ±ÈÕ=0Ê±,±íÊ¾Ã¿ÈÕĞ£Ê±
					x&=0xffff;
				}
				if(x==y)
				{
					if(UARTCtrl->BroadcastTime<3)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
					{
						UARTCtrl->BroadcastTime=3;//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
					}
				}
			}
		}
	#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­						
		x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
		if(x==1)
		{
			x=Check_HMS(MRR(ADDR_AFN04F298_Ext+2,2));//Ê±·ÖÃëºÏ·¨¼ì²é,·µ»Ø0ºÏ·¨,1·Ç·¨
			x<<=8;
			y=Check_HMS(MRR(ADDR_AFN04F298_Ext+4,2));//Ê±·ÖÃëºÏ·¨¼ì²é,·µ»Ø0ºÏ·¨,1·Ç·¨
			y<<=8;
			if((x==0)&&(y==0))
			{//Ê±ÖÓºÏ·¨
				if(DataComp(ADDR_AFN04F298_Ext+2,ADDR_TYMDHMS+1,2)!=1)//Êı¾İ±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
				{
					if(DataComp(ADDR_AFN04F298_Ext+4,ADDR_TYMDHMS+1,2)!=2)//Êı¾İ±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
					{
						if(UARTCtrl->BroadcastTime<3)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
						{
							x=MRR(ADDR_AFN04F298_Ext,1);
							if((x==1)||(x==2))
							{//Ö¸¶¨±íµØÖ·»ò²É¼¯Æ÷µØÖ·µÄ¹ã²¥¶ÔÊ±ÃüÁî
								if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)<3)//²É¼¯Æ÷À©Õ¹Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
								{
									MWR(3,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);//²É¼¯Æ÷À©Õ¹Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
								}
							}
							else
							{//È«ÖÕ¶ËµÄ¹ã²¥¶ÔÊ±ÃüÁî
								UARTCtrl->BroadcastTime=3;//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
							}
							MWR(2,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
						}
					}
				}
			}
		}
	#endif	
	#if((USER/100)==5)//ÉÏº£
		i=MRR(ADDR_AFN05F201,1);
		if(i&(1<<3))
		{
			if(UARTCtrl->BroadcastTime<3)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
			{
				UARTCtrl->BroadcastTime=3;//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
			}
		}
	#endif											    
	}
//Ê±¸üĞÂ
	if(UARTCtrl->RTCH!=Comm_Ram->TYMDHMS[2])
	{
		UARTCtrl->RTCH=Comm_Ram->TYMDHMS[2];
		i=Comm_Ram->TYMDHMS[2];
		switch(i)//²É¼¯Æµ¶ÈÎªÃ¿Ìì2¡¢6¡¢10¡¢14¡¢18¡¢22µã
		{
			case 0x02:
			case 0x06:
			case 0x10:
			case 0x14:
			case 0x18:
			case 0x22:
			#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
				{//µ±Ç°ÊÇÑ­³­
					i=NoRMCompletes_3762(1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
					if(i!=0)
					{//Ğè²¹³­
						UARTCtrl->RMTaskN|=2;//ĞÂÃ¿ÈÕÈÎÎñ
					}
				}
			#endif
	#if (((USER/100)==4)||((USER/100)==6)||((USER/100)==8)||((USER/100)==11))//ÓÃ»§±êÊ¶:ÊÇÉÂÎ÷,ºşÄÏ,Í¨ÓÃÊĞ³¡,¼ªÁÖ
			//Ìæ´ú ÖÜÆÚ³­¶ÁÔØ²¨¿Ú07±íµÄÇúÏß
		#if WeekRMTask==1//Ã¿ÖÜ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ(Ìæ´úÃ¿ÈÕµÄÊ±ÈÎÎñ:ÈıÏà±íÓÃ»§°üÀ¨¾ÓÃñÓÃ»§ºÍ¹¤ÉÌÒµÓÃ»§µ±Ç°µçÑ¹¡¢µçÁ÷¡¢¶ÏÏàÀÛ¼ÆÊ±¼äÊı¾İµÄ²É¼¯)
			#if (((USER/100)==6))//ÓÃ»§±êÊ¶:ÊÇºşÄÏÏµ
				if(UARTCtrl->DateReRMCount!=0)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
				{//ÖÁÉÙÒÑ³­1ÂÖ
					y=NoRMCompletes_3762(1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
					x=y;
					x&=0xffff;
					y>>=16;//Ê§°ÜÊı
					y+=x;//¼ÓÎ´³­Êı
					x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞÒÑÖª×Ü´Ó½ÚµãÊı(°ë×Ö¶ÔÆë)
					if(x!=0)
					{
						y*=100;
						y/=x;//Ê§°ÜÂÊ
					}
				#if (USER/100)==6//ºşÄÏ
					if(y<=2)
					{
						y=0;//ºşÄÏ³­±í³É¹¦ÂÊ>=98%
					}
				#endif
					x=Comm_Ram->TYMDHMS[2];//ÊµÊ±ÖÓµÄÊ±
					if((y==0)||(x>=0x14))
					{//³­±í³É¹¦ÂÊ100%»òÊ±ÖÓ>=14µã
						UARTCtrl->RMTaskN|=4;//ĞÂÃ¿ÖÜÈÎÎñ
					}
				}
			#else//ÓÃ»§±êÊ¶:ÊÇÉÂÎ÷,Í¨ÓÃÊĞ³¡,¼ªÁÖ
				UARTCtrl->RMTaskN|=4;//ĞÂÃ¿ÖÜÈÎÎñ
			#endif

		#endif
	#endif
				break;
		}
	#if HourTask==1//Ã¿Ğ¡Ê±ÇúÏß³­¶Á
		KeyUserPnList();//ÖØµã»§ÇúÏß¶³½áPnÁĞ±í
		//ÅĞÊÇ·ñÓĞÖØµã»§Îªµ¥Ïà±í
		p16=(u16*)(ADDR_KeyUserPnList);
		for(i=0;i<MaxKeyUser;i++)
		{
			x=p16[i];//²âÁ¿µãºÅ
			if(x!=0)
			{//ÊÇÖØµã»§
				x=Check_AFN04F10_Pn(x);//¼ì²éÖÕ¶ËµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÅäÖÃ²ÎÊıÖ¸¶¨µÄ²âÁ¿µãºÅ;·µ»Ø0ÎŞ,!=0ÎªÏàÍ¬²âÁ¿µãºÅµÄÅäÖÃĞòºÅ(1¿ªÊ¼)
				if(x!=0)
				{
			#if (USER/100)==8//Í¨ÓÃ°æ±¾£¨ÁÉÄş²»ÅĞ´óĞ¡ÀàºÅ£©
					break;
			#else
					x--;
					x=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*x)+26,1);//È¡´óĞ¡ÀàºÅ
				#if (USER/100)==5//ÉÏº£Ïµ
					if((x!=0x02)&&(x!=0x52)&&(x!=0x62)&&(x!=0x72))//´óĞ¡ÀàºÅ=51»ò71,ÆäËû´óĞ¡ÀàºÅ°´51´¦Àí
				#else
					#if (USER/100)==11//¼ªÁÖ
					if((x==0x30)||(x==0x31)||(x==0x40)||(x==0x41)||(x==0x44)||(x==0x45))
					#else
					if((x==0x50)||(x==0x51)||(x==0x40)||(x==0x41))//ºşÄÏµÈÆäËû
					#endif
				#endif
					{
						break;
					}
			#endif
				}
			}
		}
		if(i!=MaxKeyUser)
		{
			UARTCtrl->RMTaskN|=0x10;//ĞÂÃ¿Ğ¡Ê±ÇúÏßÈÎÎñ
		}
	#endif
	}
//ÈÕ¸üĞÂ
	if((UARTCtrl->Task==0)&&(UARTCtrl->Lock!=0x55))//0=¿ÕÏĞ
	{
		x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//Ô­ÔÂÈÕ
		y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//µ±Ç°ÔÂÈÕ
		if(x!=y)
		{
		#if ((USER/100)==5)
			UARTCtrl->RMEventTaskN|=0x20;//ĞÂÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼şÈÎÎñb0-b2·Ö±ğÎªÊÂ¼ş·Ö¼¶1-3;b5=ÈÕ¸üĞÂ,b6=ÔÂ¸üĞÂ(ÉÏº£Ã¿ÈÕÔÂ³­ÊÂ¼şÓÃ)
			if(UARTCtrl->RTCD[1]!=Comm_Ram->TYMDHMS[4])
			{
				//ÔÂ¸üĞÂ
				UARTCtrl->RMEventTaskN|=0x40;//ĞÂÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼şÈÎÎñb0-b2·Ö±ğÎªÊÂ¼ş·Ö¼¶1-3;b5=ÈÕ¸üĞÂ,b6=ÔÂ¸üĞÂ(ÉÏº£Ã¿ÈÕÔÂ³­ÊÂ¼şÓÃ)
			}
		#endif
			UARTCtrl->RTCD[0]=y;
			UARTCtrl->RTCD[1]=y>>8;
			UARTCtrl->RMComplete&=0x7f;//485³­±íD7=1ÉÏµçºóÖÁĞ¡ÒÑÍê³É1¸öÑ­»·³­±í,D6-D4±£Áô,D3-D0Íê³ÉÑ­»·³­±í¼ÆÊı(ÔÚ±ê×¢Î´³­±íÊ±Çå0,²»Òç³ö»Ø0)
		#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
			StoreRMEvene(PORTn);//Ôİ´æÕıÔÚ¶ÁµçÄÜ±íÊÂ¼şµÄÈÎÎñ
		#endif
	#if (USER/100)!=0
			//µç¿ÆÔº²âÊÔ¹ıÈÕ²»ÖØ³õÊ¼»¯,ÃâÍ¨ĞÅ´ÎÊı²âÊÔÊ±·µ»Ø·ñÈÏ,Í¬Ê±Ñ­³­±êÖ¾²»Çå0¶ø¼ÌĞøÔ­À´Ñ­³­
			Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	#endif
			//Ã¿ÈÕÈÎÎñ
			UARTCtrl->DateReRMCount=0;//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
	#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
			UARTCtrl->RMTaskN|=2;//ĞÂÃ¿ÈÕÈÎÎñ
			ClrRMTaskComplete_3762(1,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
			WarningCancel(100);//¸æ¾¯È¡Ïû,Èë¿Ú¸æ¾¯´úÂë
	#endif
			//Çå¼¯ÖĞ³­±íĞÅÏ¢
			p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
			p8+=(19*2);
			MC(0,((u32)p8)+4,4);
			MC(0xee,((u32)p8)+8,12);
			//×Ô¶¯ËÑ±í
	#if (USER/100)!=6//ÓÃ»§:²»ÊÇºşÄÏ;ºşÄÏÁíÓĞ×ÔĞĞ¶¨Ê±
			UARTCtrl->LockFlags&=0xfb;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
	#endif
	#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­
			//ÈÕ¸üĞÂÊ±ÇåÁã
			MC(0,ADDR_AFN0CF250_Ext1,10*MaxRS485AddCarrierPn+4);
			MC(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,3);
	#endif
		}
	}
//ÖÜ¸üĞÂ
#if (USER/100)==3//ÓÃ»§±êÊ¶:¸£½¨Ïµ
	#if WeekRMTask==1//Ã¿ÖÜ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
	if(UARTCtrl->Task==0)//0=¿ÕÏĞ
	{
		if(Comm_Ram->TYMDHMS[6]==1)
		{//ÊÇĞÇÆÚÒ»
			if(Comm_Ram->TYMDHMS[2]>=0x15)
			{//15µã¿ªÊ¼
				x=UARTCtrl->RTCW[0]+(UARTCtrl->RTCW[1]<<8);//Ô­ÄêÔÂÈÕ
				y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//µ±Ç°ÄêÔÂÈÕ
				if(x!=y)
				{
					UARTCtrl->RTCW[0]=y;
					UARTCtrl->RTCW[1]=y>>8;
					UARTCtrl->RMTaskN|=4;//ĞÂÃ¿ÖÜÈÎÎñ
				}
			}
		}
	}
	#endif
#endif

//Ãë¸üĞÂ
	if(UARTCtrl->RTCS!=Comm_Ram->TYMDHMS[0])
	{//ÓĞĞÂÈÎÎñÊ±¿ÕÏĞÊ±Ò²½øÈë1´Î,ÒòNewTask_3762ĞÂÈÎÎñÔÚ¿ÕÏĞÊ±ÓĞĞ§
		UARTCtrl->RTCS=Comm_Ram->TYMDHMS[0];
		//³­±íÊ±¶Î
		if(UARTCtrl->RMTaskC==0)
		{//ÊÇÆô³­,×ÜÊÇÒª³­±í
			Terminal_Router->RMTD=1;//0=·Ç³­±íÊ±¶Î,1=³­±íÊ±¶Î
		}
		else
		{
			x=MRR((ADDR_AFN04F33+((14+(4*24))*30))+13,1);//¶Ë¿Ú31,ÔÊĞí³­±íÊ±¶ÎÊım
			if(x>24)
			{
				x=24;
			}
//		#if (USER/100)==0//xxx
//			//µç¿ÆÔº²âÊÔ×ÜÊÇ³­±í
//			w=w;
//			Terminal_Router->RMTD=1;//0=·Ç³­±íÊ±¶Î,1=³­±íÊ±¶Î
//		#else
			y=MRR(ADDR_TYMDHMS+1,2);//È¡µ±Ç°ÊµÊ±ÖÓÊ±·Ö
			Terminal_Router->RMTD=0;//0=·Ç³­±íÊ±¶Î,1=³­±íÊ±¶Î
			for(i=0;i<x;i++)
			{
				z=MRR((ADDR_AFN04F33+((14+(4*24))*30))+14+(i*4),2);//È¡ÔÊĞí³­±íÊ±¶Î¿ªÊ¼Ê±¼äÊ±·Ö
				if(y>=z)
				{
					w=MRR((ADDR_AFN04F33+((14+(4*24))*30))+14+(i*4)+2,2);//È¡ÔÊĞí³­±íÊ±¶Î½áÊøÊ±¼äÊ±·Ö
					if(w==0x0)
					{
						w=0x2400;//½áÊøÊ±¼äÊ±·Ö00:00µãÈÏÎª24:00
					}
					if(y<w)
					{//³­±íÊ±¶ÎÄÚ
						Terminal_Router->RMTD=1;//0=·Ç³­±íÊ±¶Î,1=³­±íÊ±¶Î
						break;
					}
				}
			}
//		#endif	
		}
		if((Terminal_Router->RMTD!=0)&&(Terminal_Router->RouterInitTask>=200))//0=·Ç³­±íÊ±¶Î,1=³­±íÊ±¶Î
		{//³­±íÊ±¶ÎÄÚ
			Terminal_AFN0CF11_3762();//ÖÕ¶ËÀà1Êı¾İF11,ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢

			//¶¨Ê±²É¼¯Í£µç¼ÇÂ¼ÈÎÎñ
		#if TimerRMTask==1//¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
			if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			{//Ñ­³­ÈÎÎñ
				i=MRR(ADDR_AFN04F97,1);//Í£µçÊı¾İ²É¼¯ÅäÖÃ²ÎÊı
				i&=1;
				if(i!=0)
				{//Òª²É¼¯
					i=MRR(ADDR_AFN04F97+1,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô	BIN	1	µ¥Î»ÎªĞ¡Ê±
					if(i!=0)
					{//³­¶ÁÊ±¼ä¼ä¸ô!=0
						p16=(u16*)(&Terminal_Ram->RS4854RMF246_M_Timer);//25 RS485-1Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô·Ö¶¨Ê±Æ÷
						if(p16[0]==0)
						{
							UARTCtrl->RMTaskN|=8;//ĞÂ¶¨Ê±³­Í£µç¼ÇÂ¼ÈÎÎñ
						}
						i=MRR(ADDR_TYMDHMS+2,4);//µ±Ç°YMDH
						x=MRR(((u32)&UARTCtrl->NextTimerRMTime)+2,4);
						if(i>=x)
						{
							UARTCtrl->RMTaskN|=8;//ĞÂ¶¨Ê±³­Í£µç¼ÇÂ¼ÈÎÎñ
						}
						if(UARTCtrl->RMTaskN&8)
						{
							ClrRMTaskComplete_3762(3,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
							i=MRR(ADDR_AFN04F97+1,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô	BIN	1	µ¥Î»ÎªĞ¡Ê±
							p16[0]=i*60;//·Ö
							x=MRR(ADDR_TYMDHMS+2,4);
							x=YMDH_AddH(x,i);//ÄêÔÂÈÕÊ±¼ÓhÊ±,·µ»Ø¼ÓhÊ±ºóµÄÄêÔÂÈÕÊ±
							d64a=x;
							d64a<<=16;//
							MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//¶¨Ê±³­Í£µç¼ÇÂ¼¼ä¸ôÖÜÆÚÏÂ´Î³­±íÊ±¼äÃë·ÖÊ±ÈÕÔÂê
	
							i=MRR(ADDR_AFN04F97+2,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼äÏŞÖµ	BIN	1	µ¥Î»Îª·ÖÖÓ
							i*=60;
							p16=(u16*)(&Terminal_Ram->RS4854RMF246_S_Timer);//25 RS485-4Í£µçÊÂ¼ş³­¶ÁÊ±¼äÏŞÖµÃë¶¨Ê±Æ÷
							p16[0]=i;
						}
					}
				}
			}
		#endif
		
//			i=NewTask_3762(PORTn);//3762¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
//			if(i!=0)
//			{//´Ë·µÎŞ±ğÒâÒå,½öÊ¹Ö÷³ÌĞòÃ¿Ñ­»·Ê±¼äĞ¡
//				return;
//			}
			//³­±í¼ä¸ôÔİÍ£
			if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			{//µ±Ç°ÊÇÑ­³­
				p16=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
//				z=DataComp(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime,6);//Êı¾İ±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
//				if(z!=2)
//				{
//					z=0;//ÏàµÈ»òTYMDHMS>NextRMTime
//				}
//				if((p16[0]==0)||(z==0))
//				{
//					z=0;//³­±í¶¨Ê±ÖÜÆÚµ½
//				}
//				else
//				{
//					z=1;//Ã»³­±í¶¨Ê±ÖÜÆÚµ½
//				}
				z=TimeDifference(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime);//Ê±ÖÓ²îÖµ¼ÆËã,Èë¿ÚR0=Ê±ÖÓ1(Ãë·ÖÊ±ÈÕÔÂÄê)µØÖ·;R1=Ê±ÖÓ2(Ãë·ÖÊ±ÈÕÔÂÄê)µØÖ·;³ö¿ÚR0=HEX²¹ÂëÊ±ÖÓ¿ì»òÂıµÄ²îÖµ(Ãë),Ê±ÖÓ1Ê±ÖÓ2·Ç·¨R0=0,Ê±ÖÓ2>=Ê±ÖÓ1²îÖµÎªÕı,Ê±ÖÓ2<Ê±ÖÓ1²îÖµÎª¸º
				if(z>>31)
				{//TYMDHMS>NextRMTime
					z=0;//ÏàµÈ»òTYMDHMS>NextRMTime
				}
				else
				{
					i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);//³­±í¼ä¸ôĞ¡Ê±1-60
					if(i>60)
					{
						i=60;
					}
					i*=3600;
					if(z>i)
					{
						z=0;//Ê±ÖÓµ¹ÍË
					}
				}
				if(p16[0]==0)
				{
					z=0;//³­±í¶¨Ê±ÖÜÆÚµ½
				}
				if(UARTCtrl->RMFlag!=0)//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
				{
					x=NoRMCompletes_3762(MaxRMTask-1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
					if(x==0)
					{//È«²¿³­³É¹¦	
						if(((Terminal_Router->StopFlags&0xfffffffe)==0)&&(Terminal_Router->RouterCtrl==0))//ÔİÍ£Â·ÓÉ±êÖ¾:b0=³­±í¼ä¸ô¶¨Ê±
						{
						#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
							i=NoRMCompletes_3762(1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
							if(i!=0)
							{//Ğè²¹³­,²»ÔİÍ£
								UARTCtrl->RMTaskN|=2;//ĞÂÃ¿ÈÕÈÎÎñ
								NewTask_3762(PORTn);//3762¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
								Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
								return;
							}
						#endif
							UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
							if((Terminal_Router->StopFlags&1)==0)//ÔİÍ£Â·ÓÉ±êÖ¾:b0=³­±í¼ä¸ô¶¨Ê±
							{
								Terminal_Router->StopFlags|=(1<<0);//ÔİÍ£Â·ÓÉ±êÖ¾:b0=³­±í¼ä¸ô¶¨Ê±
								Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
							}
						}
					}
					else
					{//
						x&=0xffff;//Î´³­Êı
						if(x==0)
						{//È«²¿ÒÑ³­µ«ÓĞÊ§°Ü
							if(((Terminal_Router->StopFlags&0xfffffffe)==0)&&(Terminal_Router->RouterCtrl==0))//ÔİÍ£Â·ÓÉ±êÖ¾:b0=³­±í¼ä¸ô¶¨Ê±
							{
							#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
								i=NoRMCompletes_3762(1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
								if(i!=0)
								{//Ğè²¹³­,²»ÔİÍ£
									UARTCtrl->RMTaskN|=2;//ĞÂÃ¿ÈÕÈÎÎñ
									NewTask_3762(PORTn);//3762¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
									Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
									return;
								}
							#endif
							}	
							if(z==0)
							{//³­±í¶¨Ê±ÖÜÆÚµ½
								UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
							}
						}
					#if (USER/100)==0
						//µç¿ÆÔº»¥»»ĞÔ²âÊÔÃ¿µ½³­±íÖÜÆÚ¼´ĞèÖØÆô
						if(z==0)
						{//³­±í¶¨Ê±ÖÜÆÚµ½
							UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
						}
					#endif
					}
				}
				if(UARTCtrl->RMFlag==0x0)//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
				{//0=³­±í¼ä¸ôÔİÍ£
					if(z==0)
					{//³­±í¶¨Ê±ÖÜÆÚµ½
						if((Terminal_Router->StopFlags&0xfffffffe)==0)//ÔİÍ£Â·ÓÉ±êÖ¾:b0=³­±í¼ä¸ô¶¨Ê±
						{
							Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
							UARTCtrl->RMFlag=1;//1=³­±í½øĞĞÖĞ
							i=MRR(ADDR_AFN04F33+((14+(4*24))*30)+9,1);//³­±í¼ä¸ôĞ¡Ê±1-60
							if(i==0)
							{
								i=1;//×îĞ¡1
							}
							if(i>60)
							{
								i=60;
							}
							y=MRR(ADDR_TYMDHMS+2,4);
							d64a=YMDH_AddH(y,i);//ÄêÔÂÈÕÊ±¼ÓhÊ±,·µ»Ø¼ÓhÊ±ºóµÄÄêÔÂÈÕÊ±
							d64a<<=16;
							MWR(d64a,(u32)&UARTCtrl->NextRMTime,6);
							i*=3600;
							i+=2;//Ãë¼Ó2,ÃâÔÚ59·Ö59Ãë¶¨Ê±µ½,Í¬²½ºóÓÖ¹é0,Õâ»áÉÙ1Ê±
							if(i>0xffff)
							{
								i=0xffff;
							}
							p16[0]=i;//³­±í¼ä¸ôÊ±¼äs
							//ÇåÁĞ±í³­±í±êÖ¾
							ClrRMTaskComplete_3762(MaxRMTask-1,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
							UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
							UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
							UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
							UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
							UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
							y=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
							if(y>MaxRS485AddCarrierPn)
							{
								y=MaxRS485AddCarrierPn;
							}
							for(i=0;i<y;i++)
							{
								Terminal_Note->List[i].FnCount=0;//³­±íFn¼ÆÊı
								Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
								Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
								Terminal_Note->List[i].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
							}
						}
					}
				}
			
			}
		}
	#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­						
		x=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
		switch(x)
		{
			case 0:
				i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
				if(i&0x01)
				{
					x=MRR(ADDR_AFN0CF249_Ext+9,1);
					if(x!=0)
					{//ÖÕ¶Ë¾«È·¶ÔÊ±³É¹¦
						if(MRR(ADDR_AFN04F298_Ext,1)!=0)
						{//µç±í¶ÔÊ±¿ª¹Ø¿ªÆô
							if(MRR(ADDR_AFN04F298_Ext+6,1))
							{//ÔÊĞí¶ÔÊ±´ÎÊı²»Îª0
								if(UARTCtrl->RMFlag==0)//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
								{
									x=NoRMCompletes_3762(MaxRMTask-1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
									if(x==0)
									{//È«²¿³­³É¹¦
										MWR(1,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
									}	
								}
							}
						}
					}
				}
				break;
			case 2:
				if(UARTCtrl->BroadcastTime<3)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
				{
					if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)<3)//²É¼¯Æ÷À©Õ¹Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
					{
						MWR(3,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						MWR(i&0xfe,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
	
						Terminal_Router->RouterCtrl=1;//Â·ÓÉÆ÷¿ØÖÆÃüÁî0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
						UARTCtrl->RMFlag=1;//1=³­±í½øĞĞÖĞ
						//ÇåÁĞ±í³­±í±êÖ¾
						ClrRMTaskComplete_3762(MaxRMTask-1,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
						UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
						UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
						UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
						UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
						UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
						y=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
						if(y>MaxRS485AddCarrierPn)
						{
							y=MaxRS485AddCarrierPn;
						}
						for(i=0;i<y;i++)
						{
							Terminal_Note->List[i].FnCount=0;//³­±íFn¼ÆÊı
							Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
							Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
							Terminal_Note->List[i].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
						}
					}
				}
				break;
			case 3:
				if(UARTCtrl->RMFlag==0)//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
				{
					x=NoRMCompletes_3762(MaxRMTask-1);//ÔØ²¨Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
					if(x==0)
					{//È«²¿³­³É¹¦
						MWR(4,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn,1);
					}	
				}
				break;
			case 4://ÈÕ¸üĞÂÊ±ÇåÁã
				break;
		}
	#endif	
	}
	if((UARTCtrl->Task==0)||(UARTCtrl->Task==2))//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
	{//¼ä¸ôÔİÍ£Ê±,µÈ´ı½ÓÊÕÄ£¿éµÄÖ÷¶¯ÉÏ±¨»á½øÈë³¬Ê±×´Ì¬
		if(Terminal_Router->RMTD!=0)//0=·Ç³­±íÊ±¶Î,1=³­±íÊ±¶Î
		{//³­±íÊ±¶ÎÄÚ
			NewTask_3762(PORTn);//3762¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
		}
	}


//Â·ÓÉÄ£¿é
	if(Pin_Read(PIN_PLC_ID)!=0)
	{//Ä£¿é²¦³ö
		//Comm_Ram->SelfTestFlags&=~(1<<16);//B16=ÔØ²¨Ä£¿é,(ÖÃÎ»±íÊ¾OK)
		Terminal_Router->NoteNum=0;//ÒÑÖª×Ü´Ó½ÚµãÊı
		Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	}
	Uart_3762_RxTx(PORTn);//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
	Terminal_RouterCtrl(PORTn);//Â·ÓÉÆ÷¿ØÖÆ;ÖØÆô,ÔİÍ£,»Ö¸´,¼ì²â
	if(Terminal_Router->RouterCtrl!=0)//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔİÍ£,3=»Ö¸´,4=¼ì²â;Íê³Éºó×Ô¶¯»Øµ½0
	{
		return;
	}
//ÊµÊ±³­¶Á
	if(Terminal_Router->RouterRealTimeTask!=0)//ÔØ²¨Fn>=129µÄÊµÊ±³­¶ÁÈÎÎñ;0=¿ÕÏĞ,1=ÔİÍ£Â·ÓÉ,2=·¢ËÍ,3=½ÓÊÕ
	{
		//RealTimeReadMeter_3762_Fn_Std(PORTn);//½ö³­1¸öFn>=129Êı¾İÌîĞ´µ½FnDataBuff;RouterRealTimeTask=0=¿ÕÏĞ(±¾Fn³­¶ÁÍê³É)
		return;
	}
	
//ÅúÁ¿¶ÔÊ±ÈÎÎñ
#if NewERC14==0//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	if(Terminal_Router->RouterInitTask>=200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
#else
	if((Terminal_Router->RouterInitTask>=200)&&(Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
#endif
	{
	#if Extern_BatchTask_3762==0//Íâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=
		BatchTask_3762_SETRTC(PORTn);//Êı¾İ×ª·¢ÅúÁ¿ÈÎÎñ-3762-Ê±ÖÓ¶ÔÊ±
		if(UARTCtrl->BatchTask>=2)//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
		{
			return;
		}
	#else
		if(UARTCtrl->BatchTaskLock!=0x55)//0x55=×ª·¢ÅúÁ¿ÈÎÎñÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
		{
			i=MRR(ADDR_AFN05F30+1,1);//ÖÕ¶Ë¶ÔµçÄÜ±í¶ÔÊ±¹¦ÄÜµÄ¿ªÆôÓë¹Ø±Õ
			if(i!=1)
			{//0=¹Ø±Õ£¬1=¿ªÆô£¬ÆäËüÖµ¹Ø±Õ
				if(UARTCtrl->BatchTask>=2)//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
				{
					Uart_DL645_RxTx(PORTn);//UART_DL645½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
					if(UARTCtrl->Task<=2)//UART_DL645½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
					{
						UARTCtrl->Task=0;//UART_DL645½ÓÊÕ·¢ËÍ;0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
						UARTCtrl->BatchTask=0;//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
					}
				}
				else
				{
					UARTCtrl->BatchTask=0;//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
				}
			}
			else
			{//¿ªÆô
				if(UARTCtrl->BatchTask==0)//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
				{//0=¿Õ
					x=MRR(ADDR_AFN10F12+LEN_AFN10F12PerTask+5+LEN_AFN10F12TaskData,2);//È¡ÈÎÎñÀàĞÍ1(¶ÔÊ±ÈÎÎñ)ÅäÖÃµç±íÊıÁ¿n
					if(x==0)
					{//ÅäÖÃµç±íÊıÁ¿n=0
						UARTCtrl->BatchMeterCount=0;//×ª·¢ÅúÁ¿ÈÎÎñµç±í¼ÆÊı
					}
					else
					{
						if(x>MaxRS485AddCarrierPn)
						{//ÅäÖÃµç±íÊıÁ¿>MaxRS485AddCarrierPn
							x=MaxRS485AddCarrierPn;
						}
						if(UARTCtrl->BatchMeterCount<x)
						{//×ª·¢ÅúÁ¿ÈÎÎñ¼ÆÊı<ÅäÖÃµç±íÊıÁ¿
							UARTCtrl->BatchTask=1;//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
						}
					}
				}
				else
				{//1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
				#if IC_SST39VF6401B==0//0=Ã»,1=ÓĞ
					BatchTask_3762_SETRTC(PORTn);//Êı¾İ×ª·¢ÅúÁ¿ÈÎÎñ-3762-Ê±ÖÓ¶ÔÊ±
				#else
					if((Comm_Ram->SoftModule&(1<<4))==0)//Èí¼şÄ£¿é´íÎó±êÖ¾(ÖÃÎ»±íÊ¾´íÎó):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
					{
		    		p32=(u32 *)(ADDR_E1_ENTRY);
		    		(*(void(*)())p32[104])(PORTn);//Êı¾İ×ª·¢ÅúÁ¿ÈÎÎñ-3762-Ê±ÖÓ¶ÔÊ±
					}
				#endif
				}
			}
		}
		if(UARTCtrl->BatchTask>=2)//×ª·¢ÅúÁ¿ÈÎÎñ:0=¿Õ,1=µÈ´ı¿Ú¿ÕÏĞÆô¶¯,2...¹ı³Ì
		{
			return;
		}
	#endif
	}
	
//×Ô¶¯ËÑ±í
#if NewERC14==0//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	if(Terminal_Router->RouterInitTask>=200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
#else
	if((Terminal_Router->RouterInitTask>=200)&&(Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
#endif
	{
		i=MRR(ADDR_AFN05F149+2+3,1);//µçÄÜ±íÍ¨ĞÅ²ÎÊı×Ô¶¯Î¬»¤¹¦ÄÜ¿ªÆôÓë¹Ø±Õ
		switch(i)
		{
			default:
				if((UARTCtrl->LockFlags&2)!=0)//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
				{//ÕıÔÚ×Ô¶¯ËÑ±í
					if(UARTCtrl->AutoSearchTask<100)//×Ô¶¯ËÑ±íÈÎÎñ:0=¿Õ,1=¿ªÊ¼,2...¹ı³Ì
					{
						UARTCtrl->AutoSearchReCount=0;//×Ô¶¯ËÑ±íÈÎÎñÖØ·¢¼ÆÊı
						UARTCtrl->AutoSearchTask=100;//ÖÕÖ¹Ö÷¶¯×¢²á
					}
					AutoSearchMeter_3762(PORTn);//×Ô¶¯ËÑ±í3762¶Ë¿ÚµÄµçÄÜ±í
					return;
				}
				break;
			case 0x55://ÊÕµ½´ËÃüÁî
				if((UARTCtrl->LockFlags&2)!=0)//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
				{//ÕıÔÚ×Ô¶¯ËÑ±í
					if(UARTCtrl->AutoSearchTask<100)//×Ô¶¯ËÑ±íÈÎÎñ:0=¿Õ,1=¿ªÊ¼,2...¹ı³Ì
					{
						UARTCtrl->AutoSearchReCount=0;//×Ô¶¯ËÑ±íÈÎÎñÖØ·¢¼ÆÊı
						UARTCtrl->AutoSearchTask=100;//ÖÕÖ¹Ö÷¶¯×¢²á
					}
					AutoSearchMeter_3762(PORTn);//×Ô¶¯ËÑ±í3762¶Ë¿ÚµÄµçÄÜ±í
					return;
				}
				if((UARTCtrl->Lock==0x55)||(UARTCtrl->Task==4))//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
				{
					break;
				}
				//ÇåÁĞ±í³­±íÖĞ³­±íÊı¾İ±êÊ¶¼ÆÊı
				x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
				for(i=0;i<x;i++)
				{
					Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
					Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
					Terminal_Note->List[i].RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
				}
				MWR(0x5a,ADDR_AFN05F149+2+3,1);//Á¢¼´Ö´ĞĞ
				UARTCtrl->AutoSearchTask=1;//×Ô¶¯ËÑ±íÈÎÎñ:0=¿Õ,1=¿ªÊ¼,2...¹ı³Ì
				UARTCtrl->LockFlags|=6;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
				UARTCtrl->Lock=0x55;//0x55=Í¨ĞÅ¹¦ÄÜÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
		#if (USER/100)==6//ÓÃ»§:ºşÄÏ
				i=MRR(ADDR_AFN05F103+1,2);
				i++;//¼Ó1·Ö
				Terminal_Ram->AutoSearchMeterTimer=i;//2 ×Ô¶¯ËÑ±íÔÊĞíÊ±¼ä·Ö¶¨Ê±Æ÷
		#endif
				return;
			case 0x5a://Á¢¼´Ö´ĞĞ
		#if (USER/100)==6//ÓÃ»§:ºşÄÏ
				if(Terminal_Ram->AutoSearchMeterTimer==0)//2 ×Ô¶¯ËÑ±íÔÊĞíÊ±¼ä·Ö¶¨Ê±Æ÷
				{
					i=MRR(ADDR_AFN05F103,1);
					if(i==0x55)
					{//ÊÇF103Æô¶¯,²»Ã¿ÈÕÖ´ĞĞ
						MWR(0,ADDR_AFN05F149+2+3,1);//µçÄÜ±íÍ¨ĞÅ²ÎÊı×Ô¶¯Î¬»¤¹¦ÄÜ¿ªÆôÓë¹Ø±Õ
					}
					if(UARTCtrl->AutoSearchTask<100)//×Ô¶¯ËÑ±íÈÎÎñ:0=¿Õ,1=¿ªÊ¼,2...¹ı³Ì
					{
						UARTCtrl->AutoSearchReCount=0;//×Ô¶¯ËÑ±íÈÎÎñÖØ·¢¼ÆÊı
						UARTCtrl->AutoSearchTask=100;//ÖÕÖ¹Ö÷¶¯×¢²á
					}
					AutoSearchMeter_3762(PORTn);//×Ô¶¯ËÑ±í3762¶Ë¿ÚµÄµçÄÜ±í
					return;
				}
		#endif
				UARTCtrl->LockFlags|=6;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
				UARTCtrl->Lock=0x55;//0x55=Í¨ĞÅ¹¦ÄÜÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
				AutoSearchMeter_3762(PORTn);//×Ô¶¯ËÑ±í3762¶Ë¿ÚµÄµçÄÜ±í
				return;
			case 0xaa://Ã¿ÈÕÖ´ĞĞ
				if((UARTCtrl->LockFlags&2)==0)//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
				{//Ã»Æô¶¯×Ô¶¯ËÑ±í,Ã¿ÈÕÔÚ³­±í½áÊøºóÆô¶¯
					if((UARTCtrl->LockFlags&4)==0)//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
					{//±¾ÈÕÒÑ×Ô¶¯ËÑ±í==0
						if((UARTCtrl->Lock!=0x55)&&(UARTCtrl->Task==0))
						{
							if((Comm_Ram->TYMDHMS[2]>=0x20)&&(Comm_Ram->TYMDHMS[2]<0x22))
							{//Ã¿ÈÕÔÚ20:00-22:00Æô¶¯,µ½23:00½áÊøÖÁĞ¡ÓĞ60·Ö
							#if (USER/100)!=7//·ÇÕã½­ÓÃ»§
							//Õã½­¼¯ÖĞÆ÷ÏÖ³¡Ö÷Õ¾²»»áÏÂ·¢µµ°¸,Òª¼¯ÖĞÆ÷×Ô¶¯ËÑ±í
								if(UARTCtrl->RMComplete&0x80)//485³­±íD7=1ÉÏµçºóÖÁĞ¡ÒÑÍê³É1¸öÑ­»·³­±í,D6-D4±£Áô,D3-D0Íê³ÉÑ­»·³­±í¼ÆÊı(ÔÚ±ê×¢Î´³­±íÊ±Çå0,²»Òç³ö»Ø0)
							#endif
								{
									//ÇåÁĞ±í³­±íÖĞ³­±íÊı¾İ±êÊ¶¼ÆÊı
									x=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
									for(i=0;i<x;i++)
									{
										Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
										Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
									}
									UARTCtrl->AutoSearchTask=1;//×Ô¶¯ËÑ±íÈÎÎñ:0=¿Õ,1=¿ªÊ¼,2...¹ı³Ì
									UARTCtrl->LockFlags|=6;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
									UARTCtrl->Lock=0x55;//0x55=Í¨ĞÅ¹¦ÄÜÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
									return;
								}
							}
						}
					}
				}
				else
				{//×Ô¶¯ËÑ±í
					AutoSearchMeter_3762(PORTn);//×Ô¶¯ËÑ±í3762¶Ë¿ÚµÄµçÄÜ±í
					if((Comm_Ram->TYMDHMS[2]<0x20)||(Comm_Ram->TYMDHMS[2]>=0x23))
					{//²»ÊÇÃ¿ÈÕÔÚ20:00-23:00
						UARTCtrl->Lock=0;//0x55=Í¨ĞÅ¹¦ÄÜÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
						Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
					}
					return;
				}
				break;
		}
	}

//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÖØĞÂ³­±í
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if((MRR(ADDR_AFN05F51+30,1)==0x55)&&(UARTCtrl->RMTaskC!=0))//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÖØĞÂ³­±í
	{
		if((UARTCtrl->Task!=1)&&(UARTCtrl->Task!=4))//0=¿ÕÏĞ
		{
			MC(0,ADDR_AFN05F49+30,1);//ÇåÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÔİÍ£³­±í
			MC(0x5a,ADDR_AFN05F51+30,1);//Æô¶¯ÖØĞÂ³­±íºó0x5A
		#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
			ClrRMEvene(PORTn);//Çå¶ÁµçÄÜ±íÊÂ¼ş
		#endif
			p16=(u16 *)&Terminal_Ram->RS4854RM_S_Timer;
			p16[0]=0;//³­±í¼ä¸ô¶¨Ê±
			Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			for(i=1;i<MaxRMTask;i++)
			{//³­±íÈÎÎñ´Ó1µ½MaxRMTask-1
				ClrRMTaskComplete_3762(i,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
			}
			UARTCtrl->RMTaskN=0x02;//ĞÂÈÎÎñb0-b7·Ö±ğ±íÊ¾ÈÎÎñ0-7ÓĞĞÂÈÎÎñ
			UARTCtrl->RMTaskC=(MaxRMTask-1);//Ñ­³­Ê±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
			UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
			UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
			UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
			UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
			y=Terminal_Router->NoteNum;//µ±Ç°Â·ÓÉÆ÷ÖĞ×Ü´Ó½ÚµãÊı
			if(y>MaxRS485AddCarrierPn)
			{
				y=MaxRS485AddCarrierPn;
			}
			for(i=0;i<y;i++)
			{
				Terminal_Note->List[i].FnCount=0;//³­±íFn¼ÆÊı
				Terminal_Note->List[i].RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
				Terminal_Note->List[i].ReRMCount=0;//ÖØ·¢¼ÆÊı
				Terminal_Note->List[i].RMErr=0;//µ±Ç°³­±íÊ§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü
			}	
			//¼¯ÖĞ³­±íĞÅÏ¢¿ªÊ¼½áÊøÊ±¼ä
			p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
			p8+=(19*2);
			MC(0xee,((u32)p8)+8,12);//¿ªÊ¼Ê±¼ä,½áÊøÊ±¼ä
			Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			return;
		}
	}
	
//³õÊ¼»¯Ö¸¶¨Í¨ĞÅ¶Ë¿ÚÏÂµÄÈ«²¿ÖĞ¼ÌÂ·ÓÉĞÅÏ¢
	if(UARTCtrl->Task==0)//0=¿ÕÏĞ
	{
		if(MRR(ADDR_AFN05F52+30,1)==0x55)
		{
			MC(0,ADDR_AFN05F52+30,1);
			MWR(3,ADDR_AFN01Fx+5,1);//³õÊ¼»¯ÃüÁî:0=·ñ,1=Ó²¼ş³õÊ¼»¯,2=Êı¾İÇø³õÊ¼»¯,3=²ÎÊı¼°È«ÌåÊı¾İÇø³õÊ¼»¯(¼´»Ö¸´ÖÁ³ö³§ÅäÖÃ),4=²ÎÊı(³ıÓëÏµÍ³Ö÷Õ¾Í¨ĞÅÓĞ¹ØµÄ)¼°È«ÌåÊı¾İÇø³õÊ¼»¯,5=²ÎÊı¼°È«ÌåÊı¾İÇø³õÊ¼»¯(¼´»Ö¸´ÖÁ³ö³§ÅäÖÃ)ºóÓ²¼ş¸´Î»
			Terminal_Router->RouterInitTask=0;//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
		}
	}



//
	if(Terminal_Router->RouterInitTask>=200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	{
		if(UARTCtrl->Lock==0x55)//0x55=Í¨ĞÅ¹¦ÄÜÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
		{
			return;
		}
	}
//Ö´ĞĞ¹ã²¥Ğ£Ê±
#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	{
#endif
		RouterBroadcastTime(PORTn);//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
		if(UARTCtrl->BroadcastTime>=4)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
		{
			if(Terminal_Router->RouterInitTask<200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			{
				UARTCtrl->BroadcastTime=0;//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
			}
			return;
		}
	#if ((USER/100)==7)||((USER/100)==8)//Õã½­¡¢Í¨ÓÃ°æ
		RouterBroadcastTime_Collector(PORTn);//²É¼¯Æ÷À©Õ¹Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
		if(MRR(ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1)>=4)//²É¼¯Æ÷À©Õ¹Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
		{
			if(Terminal_Router->RouterInitTask<200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
			{
				MWR(0,ADDR_AFN0CF250_Ext+(1+10*31)*MaxRS485AddCarrierPn,1);//²É¼¯Æ÷À©Õ¹Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
			}
			return;
		}
	#endif

#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	}
#endif

//Ö´ĞĞ³­±í
	Terminal_ReadMeter_3762(PORTn);//ÖÕ¶Ë³­±íQ/GDW376.2
	
}

#endif



