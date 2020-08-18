
//ÖÕ¶Ë³­±í
#include "Project.h"
#include "Terminal_ReadMeter_RS485.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_ReadMeter_ACSample.h"
#include "Terminal_ReadMeter_DL645.h"
#include "Terminal_ReadMeter_DL645_2007.h"
#include "Terminal_ReadMeter_DL645_1997.h"

#include "AutoSearchMeter_RS485.h"
#include "BatchTask_RS485.h"
#include "../Display/Warning.h"
#include "KeyUser.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_ReadMeter_Event.h"

#include "Terminal_ReadMeter_WSH41.h"
#include "Terminal_ReadMeter_ShH.h"



void ReadMeter_Protocol(u32 PORTn)//³­±í¹æÔ¼
{
	u32 i;
  UARTCtrl_TypeDef * UARTCtrl;
#if (Extern_Terminal_ReadMeter_DL645_1997!=0)||(Extern_Terminal_ReadMeter_ShH!=0)//Íâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=;ÖÕ¶Ë³­±íDL645-1997»òÍâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=;ÖÕ¶Ë³­±í ÉÏº£±í¹æÔ¼
	u32 *p32;
	p32=p32;
#endif

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ
	if(i==2)
	{//½»Á÷²ÉÑù×°ÖÃ(2)
		if(PORTn==ACSAMPLEPORT)
		{//µ±Ç°¿ÚÊÇÄÚ²¿²ÉÑù¿Ú
			Terminal_ReadMeter_ACSample(PORTn);//ÖÕ¶Ë³­±í,½»Á÷²ÉÑù×°ÖÃ
		}
		else
		{
			UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
		}
		return;
	}
	if(i==1)
	{//DL/T645-1997(1)
	#if Extern_Terminal_ReadMeter_DL645_1997==0//Íâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=;ÖÕ¶Ë³­±íDL645-1997
		Terminal_ReadMeter_DL645_1997(PORTn);//ÖÕ¶Ë³­±íDL645-1997
	#else
		if((Comm_Ram->SoftModule&(1<<4))==0)//Èí¼şÄ£¿é´íÎó±êÖ¾(ÖÃÎ»±íÊ¾´íÎó):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[101])(PORTn);//ÖÕ¶Ë³­±íDL645-1997
		}
	#endif	
		return;
	}
	if(i==30)
	{//DL/T645-2007(30)
		Terminal_ReadMeter_DL645_2007(PORTn);//ÖÕ¶Ë³­±íDL645-2007
		return;
	}

#if (USER/100)==10//ºÓ±±×¨ÓÃ
	if(i==7)
	{//ÍşÊ¤¹æÔ¼(ºÓ±±)
		Terminal_ReadMeter_WSH41(PORTn);//ÖÕ¶Ë³­±íRS485¿Ú_ÍşÊ¤¹æÔ¼4.1°æ±¾
		return;
	}
#endif

#if (USER/100)==5//ÉÏº£×¨ÓÃ
	if(i==21)
	{//ÉÏº£¹æÔ¼
	#if Extern_Terminal_ReadMeter_ShH==0//Íâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=;ÖÕ¶Ë³­±í ÉÏº£±í¹æÔ¼
		Terminal_ReadMeter_ShH(PORTn);//ÖÕ¶Ë³­±íRS485¿Ú_ÉÏº£¹æÔ¼
	#else
		if((Comm_Ram->SoftModule&(1<<4))==0)//Èí¼şÄ£¿é´íÎó±êÖ¾(ÖÃÎ»±íÊ¾´íÎó):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
		{
			p32=(u32 *)(ADDR_E1_ENTRY);
			(*(void(*)())p32[112])(PORTn);//ÖÕ¶Ë³­±í_ÉÏº£¹æÔ¼
		}
	#endif
		return;
	}
#endif

	//ÎŞ¶ÔÓ¦¹æÔ¼
	UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
}


void Terminal_AFN0CF11_RS485(u32 PORTn,u32 RMTask)//ÖÕ¶ËÀà1Êı¾İF11,ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢;Èë¿ÚÒª¼ÆËãµÄ³­±íÈÎÎñºÅ
{
	u32 i;
	u32 x;
	u32 Pn;
	u32 RS485PORT;
	u8 *p8;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 MeterNum;
#if IC_SST39VF6401B==0//0=Ã»,1=ÓĞ
	u32 Addr;
#else
	u8 *p8d;
#endif
#if ((Project/100)==2)||((Project/100)==5)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=
	u32 y;
	u32 KeyUser;
	u32 n;
	#if IC_SST39VF6401B==1//0=Ã»,1=ÓĞ
	u8 *p8s;
	#endif
#endif


	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
	i=MRR(ADDR_TYMDHMS+1,2);
	#if (((Project/100)==2) && ((USER/100)==5))//ÉÏº£¼¯ÖĞÆ÷
	p8=(u8*)(ADDR_AFN04F242);//³­±íÑÓÊ±·ÖÖÓÊı
	n=p8[0];
	if(n>=60)//³­±íÑÓÊ±·ÖÖÓÊı£º1-60·ÖÖÓ
	{
		n=0x100;//1:00
	}
	else
	{
		n=hex_bcd(n);
	}
	if(i<n)
	{
		return;
	}
	#else
	if(i<RMM_StartHM)
	{
		return;
	}
	#endif
#endif
#if (USER/100)==5//ÉÏº£
	i=MRR(ADDR_TYMDHMS+1,2);
#if ((Project/100)==2)//ÉÏº£¼¯ÖĞÆ÷
	p8=(u8*)(ADDR_AFN04F242);//³­±íÑÓÊ±·ÖÖÓÊı
	n=p8[0];
	if(n>=60)//³­±íÑÓÊ±·ÖÖÓÊı£º1-60·ÖÖÓ
	{
		n=0x100;//1:00
	}
	else
	{
		n=hex_bcd(n);
	}
	if(i<n)
	{
		return;
	}
#endif
	if(i<RMM_StartHM)
	{
		return;
	}
#endif
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=1;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=2;
			break;
		case RS485_4PORT://RS485-4
			RS485PORT=3;
			break;
		default:
			return;
	}
	p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
#if ((Project/100)==2)||((Project/100)==5)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=
	KeyUser=MRR(ADDR_AFN04F35,1);
	if(KeyUser>MaxKeyUser)
	{
		KeyUser=MaxKeyUser;
	}
	p8[0]=3;//±¾ÏîÊı¾İ¿é¸öÊın	BIN	1(1-31)
#else
	p8[0]=2;//±¾ÏîÊı¾İ¿é¸öÊın	BIN	1(1-31)
#endif
	p8+=(19*(RS485PORT-1));
	if(PORTn!=RS485_4PORT)
	{//485¿Ú
		RS485PORT++;
		p8[1]=RS485PORT;//ÖÕ¶ËÍ¨ĞÅ¶Ë¿ÚºÅ	BIN	1
	}
	else
	{//ÔØ²¨¿Ú
		p8[1]=31;//ÖÕ¶ËÍ¨ĞÅ¶Ë¿ÚºÅ	BIN	1
		RS485PORT=31;
	}
	MeterNum=0;//Òª³­µç±í×ÜÊı=0
	x=0;//³­±í³É¹¦Êı
#if ((Project/100)==2)||((Project/100)==5)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=
	y=0;//³­ÖØµã»§³É¹¦Êı
#endif
#if IC_SST39VF6401B==0//0=Ã»,1=ÓĞ
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//Í¨ĞÅËÙÂÊ¼°¶Ë¿ÚºÅ
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//Í¨ĞÅ¶Ë¿ÚºÅÏàÍ¬
			Pn=MRR(Addr,2);//ËùÊô²âÁ¿µãºÅ BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//ËùÊô²âÁ¿µãºÅ!=0x0
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]!=3)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				{
					MeterNum++;//Òª³­µç±í×ÜÊı
				}
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==2)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				{
					x++;//³­±í³É¹¦Êı
				#if ((Project/100)==2)||((Project/100)==5)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=
					for(n=0;n<KeyUser;n++)
					{
						if((i+1)==MRR(ADDR_AFN04F35+1+(2*n),2))//ÖØµã»§µÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ
						{
							y++;//³­ÖØµã»§³É¹¦Êı
							break;
						}
					}
				#endif
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
#else
	p8d=(u8*)(ADDR_AFN04F10+2);
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=p8d[2];//Í¨ĞÅËÙÂÊ¼°¶Ë¿ÚºÅ
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//Í¨ĞÅ¶Ë¿ÚºÅÏàÍ¬
			Pn=p8d[0]+(p8d[1]<<8);//ËùÊô²âÁ¿µãºÅ BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//ËùÊô²âÁ¿µãºÅ!=0x0
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]!=3)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				{
					MeterNum++;//Òª³­µç±í×ÜÊı
				}
				if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==2)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				{
					x++;//³­±í³É¹¦Êı
				#if ((Project/100)==2)||((Project/100)==5)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=
					p8s=(u8*)(ADDR_AFN04F35+1);
					for(n=0;n<KeyUser;n++)
					{
						if((i+1)==(p8s[0]+(p8s[1]<<8)))//ÖØµã»§µÄµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ
						{
							y++;//³­ÖØµã»§³É¹¦Êı
							break;
						}
						p8s+=2;
					}
				#endif
				}
			}
		}
		p8d+=LEN_AFN04F10_Pn;
	}
#endif
	MWR(MeterNum,((u32)p8)+2,2);//Òª³­µç±í×ÜÊı	BIN	2
	if(MeterNum==x)
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
	if(UARTCtrl->RMFlag!=0)//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
	{//×¢:³­±íÔİÍ£Ê±Çå0ÁËÍê³É±êÖ¾
		MWR(x,((u32)p8)+5,2);//³­±í³É¹¦¿éÊı	BIN	2
	#if ((Project/100)==2)||((Project/100)==5)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖĞÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖĞÆ÷¢òĞÍ,600=Í¨ĞÅÄ£¿é,700=
		MWR(y,((u32)p8)+7,1);//³­ÖØµã±í³É¹¦¿éÊı	BIN	1
	#else
		MWR(0,((u32)p8)+7,1);//³­ÖØµã±í³É¹¦¿éÊı	BIN	1
	#endif
	}
	//³­±í¿ªÊ¼Ê±¼ä	¼û¸½Â¼A.1	6
	if(p8[8+3]==0xee)
	{//ÎŞ¿ªÊ¼Ê±¼ä
		MR(((u32)p8)+8,ADDR_AFN0CF2,6);
	}
	//³­±í½áÊøÊ±¼ä	¼û¸½Â¼A.1	6
}

u32 NoRMCompletes_RS485(u32 PORTn,u32 RMTask)//RS485Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 Pn;
	u32 RS485PORT;
#if IC_SST39VF6401B==0//0=Ã»,1=ÓĞ
	u32 Addr;
#else
	u8 *p8;
#endif

	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=2;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=3;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=4;
			break;
		default:
			return 0;
	}
	x=0;//Î´³­Êı
	y=0;//Ê§°ÜÊı
#if IC_SST39VF6401B==0//0=Ã»,1=ÓĞ
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//Í¨ĞÅËÙÂÊ¼°¶Ë¿ÚºÅ
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//Í¨ĞÅ¶Ë¿ÚºÅÏàÍ¬
			Pn=MRR(Addr,2);//ËùÊô²âÁ¿µãºÅ BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//ËùÊô²âÁ¿µãºÅ!=0x0
				z=AllRMList->Pn[Pn].RMCompleteS[RMTask];
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
		}
		Addr+=LEN_AFN04F10_Pn;
	}
#else
	p8=(u8*)(ADDR_AFN04F10+2);
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=p8[2];//Í¨ĞÅËÙÂÊ¼°¶Ë¿ÚºÅ
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//Í¨ĞÅ¶Ë¿ÚºÅÏàÍ¬
			Pn=p8[0]+(p8[1]<<8);//ËùÊô²âÁ¿µãºÅ BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//ËùÊô²âÁ¿µãºÅ!=0x0
				z=AllRMList->Pn[Pn].RMCompleteS[RMTask];
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
		}
		p8+=LEN_AFN04F10_Pn;
	}
#endif
	return (x+(y<<16));
}
/*
void ClrRMTaskCompletePn_RS485(u32 Pn)//ÇåÖ¸¶¨PnµÄ³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:Pn
{
	u32 i;
	for(i=0;i<MaxRMTask;i++)
	{
		AllRMList->Pn[Pn].RMCompleteS[i]=0;//¼Ä´æ³­±í³É¹¦Ê§°Ü±êÖ¾:0=¿Õ,1=ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦
	}
}
*/
void ClrRMTaskCompletes_RS485(u32 PORTn,u32 RMTask,u32 All)//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
{
	u32 i;
	u32 Pn;
	u32 RS485PORT;
#if IC_SST39VF6401B==0//0=Ã»,1=ÓĞ
	u32 Addr;
#else
	u8 *p8;
#endif

	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=2;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=3;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=4;
			break;
		default:
			return;
	}
#if IC_SST39VF6401B==0//0=Ã»,1=ÓĞ
	Addr=ADDR_AFN04F10+2;
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=MRR(Addr+2,1);//Í¨ĞÅËÙÂÊ¼°¶Ë¿ÚºÅ
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//Í¨ĞÅ¶Ë¿ÚºÅÏàÍ¬
			Pn=MRR(Addr,2);//ËùÊô²âÁ¿µãºÅ BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//ËùÊô²âÁ¿µãºÅ!=0x0
				if(All==0)
				{//Ö»ÇåÊ§°Ü
					if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==1)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
					{
						AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
					}
				}
				else
				{//ÇåÈ«²¿
					AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				}
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
#else
	p8=(u8*)(ADDR_AFN04F10+2);
	for(i=0;i<AFN04F10MaxPn;i++)
	{
		Pn=p8[2];//Í¨ĞÅËÙÂÊ¼°¶Ë¿ÚºÅ
		Pn&=0x1f;
		if(Pn==RS485PORT)
		{//Í¨ĞÅ¶Ë¿ÚºÅÏàÍ¬
			Pn=p8[0]+(p8[1]<<8);//ËùÊô²âÁ¿µãºÅ BIN 2 
			if((Pn!=0x0)&&(Pn<=MaxRS485AddCarrierPn))
			{//ËùÊô²âÁ¿µãºÅ!=0x0
				if(All==0)
				{//Ö»ÇåÊ§°Ü
					if(AllRMList->Pn[Pn].RMCompleteS[RMTask]==1)//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
					{
						AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
					}
				}
				else
				{//ÇåÈ«²¿
					AllRMList->Pn[Pn].RMCompleteS[RMTask]=0;//³­±íÈÎÎñÍê³É±êÖ¾:0=Ã»Íê³É,1=Íê³Éµ«ÖÁÉÙÓĞ1¸öÊı¾İ±êÊ¶³­¶ÁÊ§°Ü,2=³É¹¦Íê³É,3=²»Ğè³­
				}
			}
		}
		p8+=LEN_AFN04F10_Pn;
	}
#endif
}

u32 NewTask_RS485(u32 PORTn)//RS485¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
{
  UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);

	if(UARTCtrl->Lock==0x55)//0x55=Í¨ĞÅ¹¦ÄÜÕıÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌĞòÔÙ´Îµ÷ÓÃ
	{
		return 0;
	}
	if(((UARTCtrl->RMMeterEvent&1)!=0)||(UARTCtrl->SubsequentFrames!=0))
	{//ÕıÔÚ³­¶ÁµçÄÜ±íÖ÷¶¯ÉÏ±¨ÊÂ¼ş»ò¶ÁºóĞøÖ¡
		return 0;
	}
#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
	#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
	if(UARTCtrl->DateReRMCount!=0)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
	{
		if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
		{//Ö»ÓĞÑ­³­Ê±»ò³­¶ÁµçÄÜ±íÊÂ¼şÊ±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
			NewRMEvene(PORTn);//ĞÂ»ò»Ö¸´¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ;·µ»Ø0=Ã»ĞÂ½øÈë,1=ĞÂ½øÈë
		}
	}
	#else
	if(UARTCtrl->RMTaskC>=(MaxRMTask-2))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
	{//Ö»ÓĞÑ­³­Ê±»ò³­¶ÁµçÄÜ±íÊÂ¼şÊ±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
		NewRMEvene(PORTn);//ĞÂ»ò»Ö¸´¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ;·µ»Ø0=Ã»ĞÂ½øÈë,1=ĞÂ½øÈë
	}
	#endif
#endif
	if(UARTCtrl->RMTaskC>=(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
	{//Ö»ÓĞÑ­³­Ê±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
		if(UARTCtrl->RMTaskN&2)//ĞÂÃ¿ÈÕÈÎÎñ
		{
			UARTCtrl->RMTaskN&=0xfd;//È¥ĞÂÃ¿ÈÕÈÎÎñ
			UARTCtrl->RMTaskC=1;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			ClrRMTaskCompletes_RS485(PORTn,1,0);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
			if(UARTCtrl->DateReRMCount!=0xff)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
			{
				UARTCtrl->DateReRMCount++;//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
			}
		NewTask:
			UARTCtrl->RMMeterEvent=0;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
			UARTCtrl->FnCountS=UARTCtrl->FnCount;//485³­±íÔ­Ñ­³­ÈÎÎñµÄFn¼ÆÊı
			UARTCtrl->MeterNoS=UARTCtrl->MeterNo;//485³­±íÔ­Ñ­³­ÈÎÎñµÄÅäÖÃĞòºÅ¼ÆÊı(°ë×Ö¶ÔÆë)
			UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
			UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
			UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
			UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
			UARTCtrl->RMFlag=1;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
			UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
			return 1;
		}
		if(UARTCtrl->RMTaskN&4)//ĞÂÃ¿ÖÜÈÎÎñ
		{
			UARTCtrl->RMTaskN&=0xfb;//È¥ĞÂÃ¿ÖÜÈÎÎñ
			UARTCtrl->RMTaskC=2;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			goto NewTask;
		}
		if(UARTCtrl->RMTaskN&8)//ĞÂ¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ
		{
			UARTCtrl->RMTaskN&=0xf7;//È¥ĞÂ¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ
			UARTCtrl->RMTaskC=3;//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
			goto NewTask;
		}
	}
	return 0;
}


void ReadMeter_RS485(u32 PORTn)//RS485³­±í
{
	u32 i;
	u32 x;
	u32 y;
	u32 RS485PORT;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	u64 d64a;
	u8 *p8;
	u16 *p16;
#if Extern_BatchTask_RS485!=0//Íâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=
#if IC_SST39VF6401B==1//0=Ã»,1=ÓĞ
	u32 * p32;
#endif
#endif

	if((Comm_Ram->SelfTestCommand==0x55555555)||(Comm_Ram->SelfTestCommand==0xAAAAAAAA))
	{//¹¤³§ÄÚ²¿×Ô²âÊÔÃüÁî
		return;
	}
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	switch(PORTn)
	{
		case ACSAMPLEPORT://ÄÚ²¿½»Á÷²ÉÑù
			RS485PORT=0;
			p16timer=(u16 *)&Terminal_Ram->ACSampleRM_S_Timer;
			break;
		case RS485_1PORT://RS485-1
			RS485PORT=1;
			p16timer=(u16 *)&Terminal_Ram->RS4851RM_S_Timer;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=2;
			p16timer=(u16 *)&Terminal_Ram->RS4852RM_S_Timer;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=3;
			p16timer=(u16 *)&Terminal_Ram->RS4853RM_S_Timer;
			break;
		default:
			return;
	}
	
	if(RS485PORT!=0)
	{//²»ÊÇ½»²É
		if(Terminal_Ram->RMDelay_S_Timer!=0x0)//10 ÉÏµçºóÑÓÊ±³­±íÃë¶¨Ê±Æ÷
		{
			CopyString((u8 *)"485 µÈ´ıÉÏµç",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
			MWR(RS485PORT,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
			return;
		}
	}

#if PowerOnRMTask==0//µçÔ´¿ª³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
	if(UARTCtrl->RMTaskC==0)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
	{
		UARTCtrl->RMTaskC=(MaxRMTask-1);
	}
#endif

//×ª·¢ÅúÁ¿ÈÎÎñ
#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	{
#endif
		#if Extern_BatchTask_RS485==0//Íâ²¿³ÌĞò:0=I2,1=E1,2=E2,3=
		BatchTask_RS485_SETRTC(PORTn);//Êı¾İ×ª·¢ÅúÁ¿ÈÎÎñ-RS485-Ê±ÖÓ¶ÔÊ±
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
					BatchTask_RS485_SETRTC(PORTn);//Êı¾İ×ª·¢ÅúÁ¿ÈÎÎñ-RS485-Ê±ÖÓ¶ÔÊ±
				#else
					if((Comm_Ram->SoftModule&(1<<4))==0)//Èí¼şÄ£¿é´íÎó±êÖ¾(ÖÃÎ»±íÊ¾´íÎó):B0= ,B1=I1,B2=I2,B3=E5,B4=E1,B5=E2,B6=E3,B7=E4
					{
		    		p32=(u32 *)(ADDR_E1_ENTRY);
		    		(*(void(*)())p32[103])(PORTn);//Êı¾İ×ª·¢ÅúÁ¿ÈÎÎñ-RS485-Ê±ÖÓ¶ÔÊ±
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
#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	}
#endif
	if(PORTn==ACSAMPLEPORT)
	{//ÊÇÄÚ²¿½»Á÷²ÉÑù
		//ÈÕ¸üĞÂ
		if(UARTCtrl->Task==0)//0=¿ÕÏĞ
		{
			x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//Ô­ÔÂÈÕ
			y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//µ±Ç°ÔÂÈÕ
			if(x!=y)
			{
				UARTCtrl->RTCD[0]=y;
				UARTCtrl->RTCD[1]=y>>8;
				UARTCtrl->RMComplete&=0x7f;//485³­±íD7=1ÉÏµçºóÖÁĞ¡ÒÑÍê³É1¸öÑ­»·³­±í,D6-D4±£Áô,D3-D0Íê³ÉÑ­»·³­±í¼ÆÊı(ÔÚ±ê×¢Î´³­±íÊ±Çå0,²»Òç³ö»Ø0)
				UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
				UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
				UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			}
		}
	}
	else
	{//²»ÊÇÄÚ²¿½»Á÷²ÉÑù
//·Ö¸üĞÂ
		if(UARTCtrl->RTCM!=Comm_Ram->TYMDHMS[1])
		{
			UARTCtrl->RTCM=Comm_Ram->TYMDHMS[1];
		#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
			RMEveneTimer(PORTn);//·Ö¼¶¶ÁµçÄÜ±íÊÂ¼ş¶¨Ê±
		#endif
			//¹ã²¥Ğ£Ê±
			i=MRR(ADDR_AFN05F30+1,1);//ÖÕ¶Ë¶ÔµçÄÜ±í¶ÔÊ±¹¦ÄÜµÄ¿ªÆôÓë¹Ø±Õ
			if(i==1)
			{//0=¹Ø±Õ£¬1=¿ªÆô£¬ÆäËüÖµ¹Ø±Õ
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+1,1);//ÖÕ¶Ë³­±íÔËĞĞ²ÎÊıÉèÖÃ,¶Ë¿Ú31
				if((i&0x08)!=0x0)
				{//ÒªÇóÖÕ¶Ë¶¨Ê±¶Ôµç±í¹ã²¥Ğ£Ê±
					x=MRR(ADDR_TYMDHMS+1,3);//È¡µ±Ç°ÊµÊ±ÖÓÈÕÊ±·Ö
					y=MRR((ADDR_AFN04F33+((14+(4*24))*RS485PORT))+10,3);//¶Ôµç±í¹ã²¥Ğ£Ê±¶¨Ê±Ê±¼äÈÕÊ±·Ö
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
			if(x==2)
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
								UARTCtrl->BroadcastTime=3;//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										x=0;
										break;
									case RS485_2PORT://RS485-2
										x=1;
										break;
								}
								MWR(3,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+x,1);
							}
						}
					}
				}
			}
		#endif	
		#if((USER/100)==5)//ÉÏº£
			i=MRR(ADDR_AFN05F201,1);
			if(i&(1<<(RS485PORT-1)))
			{
				if(UARTCtrl->BroadcastTime<3)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
				{
					UARTCtrl->BroadcastTime=3;//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
				}
			}
		#endif
		}
//Ê±¸üĞÂ
		if(UARTCtrl->Task==0)//0=¿ÕÏĞ
		{
			if(UARTCtrl->RTCH!=Comm_Ram->TYMDHMS[2])
			{
				UARTCtrl->RTCH=Comm_Ram->TYMDHMS[2];
			#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
				{//µ±Ç°ÊÇÑ­³­
					i=NoRMCompletes_RS485(PORTn,1);//RS485Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
					if(i!=0)
					{//Ğè²¹³­
						UARTCtrl->RMTaskN|=2;//ĞÂÃ¿ÈÕÈÎÎñ
					}
				}
			#endif
				/*
				if(UARTCtrl->RMKeyUser==0)//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
				{
					UARTCtrl->MeterNoBackupOfKeyUser=UARTCtrl->MeterNo;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)(Ô­³£¹æ³­±íµÄ×°ÖÃĞòºÅ¼Ä´æ)
					UARTCtrl->FnEndBackupOfKeyUser=UARTCtrl->FnEnd;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø(Ô­³£¹æ³­±íµÄFn¼ÆÊı½áÊø±êÖ¾¼Ä´æ)
					UARTCtrl->FnCountBackupOfKeyUser=UARTCtrl->FnCount;//485³­±íFn¼ÆÊı(Ô­³£¹æ³­±íµÄFn¼ÆÊı¼Ä´æ)
					UARTCtrl->RMErrorBackupOfKeyUser=UARTCtrl->RMError;//485³­±í±¾µçÄÜ±íÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü(Ô­³£¹æ³­±íµÄÊ§°Ü±êÖ¾¼Ä´æ)
					UARTCtrl->KeyUserRMCount=0;//ÖØµã»§³­¶Á±íÊı¼ÆÊı
					UARTCtrl->FnEnd=0;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
					UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
					UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
					UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
				}
				UARTCtrl->RMKeyUser=1;//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
				*/
			}
		}
//ÈÕ¸üĞÂ
		if(UARTCtrl->Task==0)//0=¿ÕÏĞ
		{
			x=UARTCtrl->RTCD[0]+(UARTCtrl->RTCD[1]<<8);//Ô­ÔÂÈÕ
			y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//µ±Ç°ÔÂÈÕ
			if(x!=y)
			{
				UARTCtrl->RMEventTaskN|=0x20;//ĞÂÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼şÈÎÎñb0-b2·Ö±ğÎªÊÂ¼ş·Ö¼¶1-3;b5=ÈÕ¸üĞÂ,b6=ÔÂ¸üĞÂ(ÉÏº£Ã¿ÈÕÔÂ³­ÊÂ¼şÓÃ)
				if(UARTCtrl->RTCD[1]!=Comm_Ram->TYMDHMS[4])
				{
					//ÔÂ¸üĞÂ
					UARTCtrl->RMEventTaskN|=0x40;//ĞÂÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼şÈÎÎñb0-b2·Ö±ğÎªÊÂ¼ş·Ö¼¶1-3;b5=ÈÕ¸üĞÂ,b6=ÔÂ¸üĞÂ(ÉÏº£Ã¿ÈÕÔÂ³­ÊÂ¼şÓÃ)
				}
				UARTCtrl->RTCD[0]=y;
				UARTCtrl->RTCD[1]=y>>8;
				UARTCtrl->RMComplete&=0x7f;//485³­±íD7=1ÉÏµçºóÖÁĞ¡ÒÑÍê³É1¸öÑ­»·³­±í,D6-D4±£Áô,D3-D0Íê³ÉÑ­»·³­±í¼ÆÊı(ÔÚ±ê×¢Î´³­±íÊ±Çå0,²»Òç³ö»Ø0)
			#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
				StoreRMEvene(PORTn);//Ôİ´æÕıÔÚ¶ÁµçÄÜ±íÊÂ¼şµÄÈÎÎñ
			#endif
				//Ã¿ÈÕÈÎÎñ
				UARTCtrl->DateReRMCount=0;//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
		#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
				UARTCtrl->RMTaskN|=0x2;//ĞÂÃ¿ÈÕÈÎÎñ
				ClrRMTaskCompletes_RS485(PORTn,1,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
		#endif
				//Çå¼¯ÖĞ³­±íĞÅÏ¢
				p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
				p8+=(19*(RS485PORT-1));
				MC(0,((u32)p8)+4,4);
				MC(0xee,((u32)p8)+8,12);
				//×Ô¶¯ËÑ±í
		#if (USER/100)!=6//ÓÃ»§:²»ÊÇºşÄÏ;ºşÄÏÁíÓĞ×ÔĞĞ¶¨Ê±
				UARTCtrl->LockFlags&=0xfb;//Çå±¾ÒÑ×Ô¶¯ËÑ±í
				i=MRR(ADDR_AFN05F149+2+(RS485PORT-1),1);//µçÄÜ±íÍ¨ĞÅ²ÎÊı×Ô¶¯Î¬»¤¹¦ÄÜ¿ªÆôÓë¹Ø±Õ
				if(i==0xaa)
				{//Ã¿ÈÕÖ´ĞĞ
					MC(0,Get_ADDR_UARTn(PORTn),6);//ÇëfifoÖ¸Õë
					UARTCtrl->LockFlags&=0xfd;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
				}
		#endif
		#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±êÊ¶£ºÍ¨ÓÃ¡¢Õã½­
				switch(PORTn)
				{
					case RS485_1PORT://RS485-1
						i=0;
						break;
					case RS485_2PORT://RS485-2
						i=1;
						break;
				}
				MWR(0,ADDR_AFN04F299_Ext+28,1);
				if(MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1)>=5)
				{//ÈÕ¸üĞÂÊ±ÇåÁã
					MWR(0,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
				}
		#endif
		#if (USER/100)==8//ÓÃ»§±êÊ¶£ºÍ¨ÓÃ
				y=Comm_Ram->TYMDHMS[3];//µ±Ç°ÈÕ
				y--;
				MC(0xff,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3+4+MaxRS485AddCarrierPn*3);
				p8=(u8 *)ADDR_DATABUFF;
				p8[0]=Comm_Ram->TYMDHMS[3];//µ±Ç°ÈÕ
				p8[1]=Comm_Ram->TYMDHMS[4];//µ±Ç°ÔÂ
				p8[2]=Comm_Ram->TYMDHMS[5];//µ±Ç°Äê
				DMW((u32)ADDR_DATABUFF,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3);//Ö±½Ó´æ´¢Æ÷Ğ´(ÎŞ²Á³ı)

				y=p8[0]+(p8[1]<<8)+(p8[2]<<16);//µ±Ç°ÈÕÔÂÄê
				y=YMD_Sub1D(y);//ÄêÔÂÈÕ¼õ1ÈÕ,·µ»Ø¼õ1ºóµÄÄêÔÂÈÕ
				y&=0xff;
				y--;
				x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y,3);
				x=YMD_Add1D(x);//ÄêÔÂÈÕ¼Ó1ÈÕ,·µ»Ø¼Ó1ºóµÄÄêÔÂÈÕ
				if(x==(p8[0]+(p8[1]<<8)+(p8[2]<<16)))
				{
					x=MRR(ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y+3,4);
					if(x==0xffffffff)
					{
						DMW((u32)ADDR_AFN0DF321_Ext_Source,ADDR_AFN0DExt_Start+(3+4+MaxRS485AddCarrierPn*3)*y+3,4);//Ö±½Ó´æ´¢Æ÷Ğ´(ÎŞ²Á³ı)
					}
				}

				MC(0,ADDR_AFN0DF321_Ext_Source,4);
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
					x=UARTCtrl->RTCW[0]+(UARTCtrl->RTCW[1]<<8);//Ô­ÔÂÈÕ
					y=Comm_Ram->TYMDHMS[3]+(Comm_Ram->TYMDHMS[4]<<8);//µ±Ç°ÔÂÈÕ
					if(x!=y)
					{
						UARTCtrl->RTCW[0]=y;
						UARTCtrl->RTCW[1]=y>>8;
						UARTCtrl->RMTaskN|=4;//ĞÂÃ¿ÖÜÈÎÎñ
						ClrRMTaskCompletes_RS485(PORTn,2,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
					}
				}
			}
		}
	#endif
#endif

//Ãë¸üĞÂ
		if(UARTCtrl->Task==0)
		{
			if(UARTCtrl->RTCS!=Comm_Ram->TYMDHMS[0])
			{
				UARTCtrl->RTCS=Comm_Ram->TYMDHMS[0];
				if(MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÔİÍ£³­±í
				{
					UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
					p16timer[0]=0;//³­±í¼ä¸ôÊ±¼ä
				}
			#if DateRMTask==1//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
				Terminal_AFN0CF11_RS485(PORTn,1);//ÖÕ¶ËÀà1Êı¾İF11,ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢;Èë¿ÚÒª¼ÆËãµÄ³­±íÈÎÎñºÅ
			#else
				Terminal_AFN0CF11_RS485(PORTn,MaxRMTask-1);//ÖÕ¶ËÀà1Êı¾İF11,ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢;Èë¿ÚÒª¼ÆËãµÄ³­±íÈÎÎñºÅ
			#endif
			//¶¨Ê±²É¼¯Í£µç¼ÇÂ¼ÈÎÎñ
			#if TimerRMTask==1//¶¨Ê±³­¶ÁÍ£µç¼ÇÂ¼ÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
				{//Ñ­³­ÈÎÎñ
					i=MRR(ADDR_AFN04F97,1);//Í£µçÊı¾İ²É¼¯ÅäÖÃ²ÎÊı
					i&=1;
					if(i!=0)
					{//Òª²É¼¯
						i=MRR(ADDR_AFN04F97+1,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô	BIN	1	µ¥Î»ÎªĞ¡Ê±
						if(i!=0)
						{//³­¶ÁÊ±¼ä¼ä¸ô!=0
							p16=(u16*)(&Terminal_Ram->RS4851RMF246_M_Timer);//25 RS485-1Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô·Ö¶¨Ê±Æ÷
							if(p16[RS485PORT-1]==0)
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
								ClrRMTaskCompletes_RS485(PORTn,3,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
								i=MRR(ADDR_AFN04F97+1,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô	BIN	1	µ¥Î»ÎªĞ¡Ê±
								p16[RS485PORT-1]=i*60;//·Ö
								x=MRR(ADDR_TYMDHMS+2,4);
								x=YMDH_AddH(x,i);//ÄêÔÂÈÕÊ±¼ÓhÊ±,·µ»Ø¼ÓhÊ±ºóµÄÄêÔÂÈÕÊ±
								d64a=x;
								d64a<<=16;//
								MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//¶¨Ê±³­Í£µç¼ÇÂ¼¼ä¸ôÖÜÆÚÏÂ´Î³­±íÊ±¼äÃë·ÖÊ±ÈÕÔÂê
	
								i=MRR(ADDR_AFN04F97+2,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼äÏŞÖµ	BIN	1	µ¥Î»Îª·ÖÖÓ
								i*=60;
								p16=(u16*)(&Terminal_Ram->RS4851RMF246_S_Timer);//25 RS485-1Í£µçÊÂ¼ş³­¶ÁÊ±¼äÏŞÖµÃë¶¨Ê±Æ÷
								p16[RS485PORT-1]=i;
							}
						}
					}
				}
			#endif
				//²åÈë¸ßÓÅÏÈ¼¶³­±í
				NewTask_RS485(PORTn);//RS485¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
			#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­						
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
				switch(x)
				{
					case 0:
						i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
						switch(PORTn)
						{
							case RS485_1PORT://RS485-1
								i&=0x02;
								break;
							case RS485_2PORT://RS485-2
								i&=0x04;
								break;
						}
						if(i)
						{
							if(MRR(ADDR_AFN0CF249_Ext+9,1)!=0)
							{//ÖÕ¶Ë¾«È·¶ÔÊ±³É¹¦
								if(MRR(ADDR_AFN04F298_Ext,1)!=0)
								{//µç±í¶ÔÊ±¿ª¹Ø¿ªÆô
									if(MRR(ADDR_AFN04F298_Ext+6,1))
									{//ÔÊĞí¶ÔÊ±´ÎÊı²»Îª0
										//ÖØĞÂ³­±í
										if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
										{
											switch(PORTn)
											{
												case RS485_1PORT://RS485-1
													i=0;
													break;
												case RS485_2PORT://RS485-2
													i=1;
													break;
											}
											MWR(1,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
											goto Re;
										}
									}
								}
							}
						}
						break;
					case 3:
						if(UARTCtrl->BroadcastTime<3)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
						{
							//ÖØĞÂ³­±í
							if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
							{
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i=0;
										break;
									case RS485_2PORT://RS485-2
										i=1;
										break;
								}
								MWR(4,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
								i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+3,1);
								switch(PORTn)
								{
									case RS485_1PORT://RS485-1
										i&=0xfd;
										break;
									case RS485_2PORT://RS485-2
										i&=0xfb;
										break;
								}
						Re:
							#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
								ClrRMEvene(PORTn);//Çå¶ÁµçÄÜ±íÊÂ¼ş
							#endif
								p16timer[0]=0;//³­±í¼ä¸ô¶¨Ê±
								for(i=1;i<MaxRMTask;i++)
								{//³­±íÈÎÎñ´Ó1µ½MaxRMTask-1
									ClrRMTaskCompletes_RS485(PORTn,i,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
								}
								UARTCtrl->RMMeterEvent=0;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
								UARTCtrl->RMTaskN=0x02;//ĞÂÈÎÎñb0-b7·Ö±ğ±íÊ¾ÈÎÎñ0-7ÓĞĞÂÈÎÎñ
								UARTCtrl->RMTaskC=(MaxRMTask-1);//Ñ­³­Ê±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
								UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
								UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
								UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
								UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
								UARTCtrl->RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
								UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
								//¼¯ÖĞ³­±íĞÅÏ¢
								p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
								p8+=(19*(RS485PORT-1));
								MC(0xee,((u32)p8)+8,12);//¿ªÊ¼Ê±¼ä,½áÊøÊ±¼ä
							}
						}
						break;
				}
			#endif	
			}
		}


//×Ô¶¯ËÑ±í
	#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
		if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
		{
	#endif
			i=MRR(ADDR_AFN05F149+2+(RS485PORT-1),1);//µçÄÜ±íÍ¨ĞÅ²ÎÊı×Ô¶¯Î¬»¤¹¦ÄÜ¿ªÆôÓë¹Ø±Õ
			switch(i)
			{
				default:
					if(UARTCtrl->Task==0)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
					{
						UARTCtrl->LockFlags&=0xfd;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
					}
					break;
				case 0x55://ÊÕµ½´ËÃüÁî
					if(UARTCtrl->Task==0)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
					{
						UARTCtrl->RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
						UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
						MWR(0x5a,ADDR_AFN05F149+2+(RS485PORT-1),1);//Á¢¼´Ö´ĞĞ
					#if (USER/100)!=5//²»ÊÇÉÏº£
						MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+((RS485PORT-1)*LEN_PerPortAutoSearchMeter),7);//´Ó0µØÖ·¿ªÊ¼ËÑ±í,DL645-2007ËÙÂÊ3=2400
					#else
						//ÉÏº£Ä¬ÈÏµÄËÙÂÊ4800
						MWR(0x04aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+((RS485PORT-1)*LEN_PerPortAutoSearchMeter),7);//´Ó0µØÖ·¿ªÊ¼ËÑ±í,DL645-2007ËÙÂÊ3=2400
					#endif
						UARTCtrl->LockFlags|=6;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
			#if (USER/100)==6//ÓÃ»§:ºşÄÏ
						i=MRR(ADDR_AFN05F103+1,2);
						i++;//¼Ó1·Ö
						Terminal_Ram->AutoSearchMeterTimer=i;//2 ×Ô¶¯ËÑ±íÔÊĞíÊ±¼ä·Ö¶¨Ê±Æ÷
			#endif
						return;
					}
					else
					{//µÈ´ı¿ÕÏĞ
						if((UARTCtrl->LockFlags&2)!=0)//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
						{
							AutoSearchMeter_RS485_DL645(PORTn);//×Ô¶¯ËÑ±íRS485¶Ë¿ÚµÄDL645
							return;
						}
					}
					break;
				case 0x5a://Á¢¼´Ö´ĞĞ
		#if (USER/100)==6//ÓÃ»§:ºşÄÏ
					if(Terminal_Ram->AutoSearchMeterTimer==0)//2 ×Ô¶¯ËÑ±íÔÊĞíÊ±¼ä·Ö¶¨Ê±Æ÷
					{
						if(UARTCtrl->Task==0)//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
						{
							i=MRR(ADDR_AFN05F103,1);
							if(i==0x55)
							{//ÊÇF103Æô¶¯,²»Ã¿ÈÕÖ´ĞĞ
								MWR(0,ADDR_AFN05F149+2+(RS485PORT-1),1);//µçÄÜ±íÍ¨ĞÅ²ÎÊı×Ô¶¯Î¬»¤¹¦ÄÜ¿ªÆôÓë¹Ø±Õ
							}
							UARTCtrl->RMCount=0;//³­±íÃüÁî¼ÆÊı=0
							UARTCtrl->ReRMCount=0;//³­±íÃüÁîÖØ·¢¼ÆÊı
							UARTCtrl->RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
							MC(0,Get_ADDR_UARTn(PORTn),6);//ÇëfifoÖ¸Õë
							UARTCtrl->LockFlags&=0xfd;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=²Ëµ¥²Ù×÷,B1=×Ô¶¯ËÑ±í,B2-B7±£Áô
							return;
						}
					}
		#endif
					UARTCtrl->LockFlags|=6;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
					AutoSearchMeter_RS485_DL645(PORTn);//×Ô¶¯ËÑ±íRS485¶Ë¿ÚµÄDL645
					return;
				case 0xaa://Ã¿ÈÕÖ´ĞĞ
					if((UARTCtrl->LockFlags&2)==0)//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
					{//Ã»Æô¶¯×Ô¶¯ËÑ±í,Ã¿ÈÕÔÚ³­±í½áÊøºóÆô¶¯
						if((UARTCtrl->LockFlags&4)==0)//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
						{//±¾ÈÕÒÑ×Ô¶¯ËÑ±í==0
							if((Comm_Ram->TYMDHMS[2]>=0x20)&&(Comm_Ram->TYMDHMS[2]<0x23))
							{//Ã¿ÈÕÔÚ20:00-23:00Æô¶¯
								if(UARTCtrl->RMComplete&0x80)//485³­±íD7=1ÉÏµçºóÖÁĞ¡ÒÑÍê³É1¸öÑ­»·³­±í,D6-D4±£Áô,D3-D0Íê³ÉÑ­»·³­±í¼ÆÊı(ÔÚ±ê×¢Î´³­±íÊ±Çå0,²»Òç³ö»Ø0)
								{
									if(UARTCtrl->Lock!=0x55)
									{
										UARTCtrl->Task=0;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
										UARTCtrl->RMCount=0;//³­±íÊı¾İ±êÊ¶¼ÆÊı
										UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
										UARTCtrl->LockFlags|=6;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
										return;
									}
								}
							}
						}
					}
					else
					{//×Ô¶¯ËÑ±í
						//if(UARTCtrl->RMKeyUser==0)//³­¶ÁÖØµã»§±êÖ¾,0=³£¹æ³­¶Á,1=ÖØµã»§³­¶Á
						//{//²»ÊÇÖØµã»§³­¶Á
							AutoSearchMeter_RS485_DL645(PORTn);//×Ô¶¯ËÑ±íRS485¶Ë¿ÚµÄDL645
							if((Comm_Ram->TYMDHMS[2]<0x20)||(Comm_Ram->TYMDHMS[2]>=0x23))
							{//²»ÊÇÃ¿ÈÕÔÚ20:00-23:00
								if(UARTCtrl->Lock!=0x55)
								{
									UARTCtrl->Task=0;//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
									UARTCtrl->LockFlags&=0xfd;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯¶¨Ê±ÈÎÎñ1ÀàÊı¾İ,B5=Ö÷¶¯¶¨Ê±ÈÎÎñ2ÀàÊı¾İ,B6=,B7=±£Áô
								}
							}
							return;
						//}
					}
					break;
			}
	#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
		}
	#endif
	
	}

//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÖØĞÂ³­±í
	if((UARTCtrl->Task==0)&&(UARTCtrl->RMTaskC!=0))//0=¿ÕÏĞ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕıÔÚ·¢ËÍ,5=ÕıÔÚ½ÓÊÕ
	{
		if(MRR(ADDR_AFN05F51+RS485PORT,1)==0x55)//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÖØĞÂ³­±í
		{
			MC(0,ADDR_AFN05F49+RS485PORT,1);//ÇåÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÔİÍ£³­±í
			MC(0x5a,ADDR_AFN05F51+RS485PORT,1);//Æô¶¯ÖØĞÂ³­±íºó0x5A
		#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
			ClrRMEvene(PORTn);//Çå¶ÁµçÄÜ±íÊÂ¼ş
		#endif
			p16timer[0]=0;//³­±í¼ä¸ô¶¨Ê±
			for(i=1;i<MaxRMTask;i++)
			{//³­±íÈÎÎñ´Ó1µ½MaxRMTask-1
				ClrRMTaskCompletes_RS485(PORTn,i,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
			}
			UARTCtrl->RMMeterEvent=0;//ÕıÔÚ³­¶ÁµçÄÜ±íÊÂ¼ş±êÖ¾;b0=Ö÷¶¯ÉÏ±¨ÊÂ¼ş,b1=
			UARTCtrl->RMTaskN=0x02;//ĞÂÈÎÎñb0-b7·Ö±ğ±íÊ¾ÈÎÎñ0-7ÓĞĞÂÈÎÎñ
			UARTCtrl->RMTaskC=(MaxRMTask-1);//Ñ­³­Ê±²ÅÄÜ²åÈë¸ßÓÅÏÈ¼¶
			UARTCtrl->MeterNo=0;//485³­±íµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ(°ë×Ö¶ÔÆë)
			UARTCtrl->FnCount=0;//485³­±íFn¼ÆÊı
			UARTCtrl->RMCount=0;//485³­±íÃüÁî¼ÆÊı
			UARTCtrl->ReRMCount=0;//485³­±íÃüÁîÖØ·¢¼ÆÊı
			UARTCtrl->RMCountErr=0;//FnÄÚÓĞ³­¶ÁÊ§°Ü»ò²»³­µÄÊı¾İ±êÊ¶¼ÆÊı
			UARTCtrl->RMError=0;//485³­±í±¾µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅÍ¨ĞÅÊ§°Ü±êÖ¾,B0=Ê§°Ü,B1=F129Ê§°Ü
			//¼¯ÖĞ³­±íĞÅÏ¢
			p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
			p8+=(19*(RS485PORT-1));
			MC(0xee,((u32)p8)+8,12);//¿ªÊ¼Ê±¼ä,½áÊøÊ±¼ä
		}
	}
	
//Ö´ĞĞ¹ã²¥Ğ£Ê±
#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	if((Terminal_Ram->PowerTask>=99)&&(UARTCtrl->RMTaskC!=0))//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
	{
#endif
		if(UARTCtrl->BroadcastTime>=3)//¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
		{
			Terminal_BroadcastTime_DL645(PORTn);//DL645¹ã²¥Ğ£Ê±0=¿Õ,1=,2=,3=ÃüÁî,4=¹ı³Ì...,Íê³Éºó×Ô¶¯»Øµ½0
			return;
		}
#if NewERC14==1//ERC14Í£µçÊÂ¼ş¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
	}
#endif

//³­±í
	if(UARTCtrl->RMFlag!=0x0)
	{//1=³­±í½øĞĞÖĞ
		if(PORTn!=ACSAMPLEPORT)
		{
			p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
			p8+=(19*(RS485PORT-1));
			p8[4]|=0x1;//D0=1,³­±í
		}
	}
	else
	{//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
		if(PORTn!=ACSAMPLEPORT)
		{
			p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
			p8+=(19*(RS485PORT-1));
			p8[4]&=0xfe;//D0=0,Î´³­±í
			if(MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÔİÍ£³­±í
			{
				CopyString((u8 *)"485 ÃüÁîÔİÍ£³­±í",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
			}
			else
			{
				CopyString((u8 *)"485 ³­±í¼ä¸ôÔİÍ£",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊı(²»¼Æ½áÊø0)
			}
			MWR(RS485PORT,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
		}
		UARTCtrl->RMTaskFlags=0;//µ±Ç°³­¶ÁÈÎÎñÊı¾İ·ÖÀà(Í¬³­±í¿â):B6=²¹³­ÉÏ2-3ÈÕ¶³½á,B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
	#if (USER/100)==5//ÉÏº£
		if(RS485PORT!=0)
		{//²»ÊÇÄÚ²¿½»Á÷²ÉÑùÍ¨ĞÅ¿Ú
			if(p16timer[4]==0)
			{//ÇúÏßÑÓÊ±³­±í¶¨Ê±µ½
				p16timer[4]=0xffff;//ÏÂ¼ä¸ô¶¨Ê±
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);
				i=hex_bcd(i);
				x=Comm_Ram->TYMDHMS[1]+(Comm_Ram->TYMDHMS[2]<<8);//Ê±·Ö
			#if ((Project/100)==2)
				if(x<i)
				{//¹ı0µãºóµÄµÚ1¸ö³­±íÖÜÆÚÄÚ
					UARTCtrl->RMTaskFlags|=0xfe;//µ±Ç°³­¶ÁÈÎÎñÊı¾İ·ÖÀà(Í¬³­±í¿â):B6=²¹³­ÉÏ2-3ÈÕ¶³½á,B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
					UARTCtrl->RMFlag=1;//1=³­±í½øĞĞÖĞ
				}
			#else
				if(x<i)
				{//¹ı0µãºóµÄµÚ1¸ö³­±íÖÜÆÚÄÚ
					UARTCtrl->RMTaskFlags|=0xfe;//µ±Ç°³­¶ÁÈÎÎñÊı¾İ·ÖÀà(Í¬³­±í¿â):B6=²¹³­ÉÏ2-3ÈÕ¶³½á,B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
				}
				else
				{
					UARTCtrl->RMTaskFlags|=0x04;//µ±Ç°³­¶ÁÈÎÎñÊı¾İ·ÖÀà(Í¬³­±í¿â):B6=²¹³­ÉÏ2-3ÈÕ¶³½á,B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
				}
				UARTCtrl->RMFlag=1;//1=³­±í½øĞĞÖĞ
			#endif
			}
		}
	#endif
		i=DataComp(ADDR_TYMDHMS,(u32)&UARTCtrl->NextRMTime,6);//Êı¾İ±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
		if((i!=2)||(p16timer[0]==0))
		{//ÏàµÈ»òTYMDHMS>NextRMTime
			//³­±í±êÖ¾
			UARTCtrl->RMTaskFlags|=0xfb;//µ±Ç°³­¶ÁÈÎÎñÊı¾İ·ÖÀà(Í¬³­±í¿â):B6=²¹³­ÉÏ2-3ÈÕ¶³½á,B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
		#if (USER/100)==5//ÉÏº£
			if(RS485PORT!=0)
			{//²»ÊÇÄÚ²¿½»Á÷²ÉÑùÍ¨ĞÅ¿Ú
				//ÉèÑÓÊ±³­±í¶¨Ê±
				i=MRR(ADDR_AFN04F242,1);//ÖÕ¶Ë¶ÔµçÄÜ±í³­±í»úÖÆ²ÎÊı(·Ö)
				x=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);//·Ö
				if(i>=x)
				{//ÑÓÊ±³­±í¶¨Ê±·Ö>=³­±íÖÜÆÚ
					i=x;
					if(i!=0)
					{//ÖÁĞ¡Ğ¡1·Ö
						i--;
					}
				}
				i*=60;
				i+=2;//¶à2ÃëÃâ¹ı0Ê±Í¬Ê±
				p16timer[4]=i;//ÑÓÊ±³­±íÃë¶¨Ê±Æ÷
			}
		#endif
		}
		if(UARTCtrl->RMTaskFlags==0)
		{//¶¨Ê±Ã»µ½
		/*
			//¼¯ÖĞ³­±íĞÅÏ¢¿ªÊ¼½áÊøÊ±¼ä
			p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
			p8+=(19*(RS485PORT-1));
			if(p8[14]==0xee)
			{//ÎŞ½áÊøÊ±¼ä
				MR(((u32)p8)+14,ADDR_AFN0CF2,6);
			}
		*/
			return;
		}
		if((MRR(ADDR_AFN05F49+RS485PORT,1)==0x55)&&(UARTCtrl->RMTaskC!=0))//ÃüÁîÖ¸¶¨Í¨ĞÅ¶Ë¿ÚÔİÍ£³­±í
		{
			return;
		}
		UARTCtrl->RMFlag=1;//1=³­±í½øĞĞÖĞ
		if(RS485PORT!=0)
		{//²»ÊÇÄÚ²¿½»Á÷²ÉÑùÍ¨ĞÅ¿Ú
			if(UARTCtrl->RMTaskFlags&0x1)
			{//ÊÇ³£¹æ³­¶¨Ê±µ½
				i=MRR(ADDR_AFN04F33+((14+(4*24))*RS485PORT)+9,1);//·Ö
				if(i==0)
				{
					i=1;//×îĞ¡1·Ö
				}
				d64a=MRR(ADDR_TYMDHMS+1,5);
				d64a=YMDHM_AddM(d64a,i);//ÄêÔÂÈÕÊ±·Ö¼Óm·Ö,·µ»Ø¼Óm·ÖºóµÄÄêÔÂÈÕÊ±·Ö
				if((60%i)==0)
				{//¼ä¸ôÊ±¼äµÄÕûÊı±¶ÊÇ60·Ö
					//Í¬²½µ½Õû¼ä¸ôÊ±¼ä·ÖÖµ
					x=d64a;
					x&=0xff;
					x=bcd_hex(x);
					x/=i;
					x*=i;
					x=hex_bcd(x);
					d64a>>=8;
					d64a<<=8;
					d64a|=x;
				}
				d64a<<=8;//ÃëÖµ=0
				MWR(d64a,(u32)&UARTCtrl->NextRMTime,6);
				
				i*=60;
				i+=2;//Ãë¼Ó2,ÃâÔÚ59Ãë¶¨Ê±µ½,Í¬²½ºóÓÖ¹é0,Õâ»áÉÙ1·Ö
				p16timer[0]=i;//ÖÕ¶Ë³­±í¼ä¸ô
			/*
				//¼¯ÖĞ³­±íĞÅÏ¢¿ªÊ¼½áÊøÊ±¼ä
				Terminal_AFN0CF2_Computer();//ÖÕ¶ËÀà1Êı¾İF2,ÖÕ¶ËÈÕÀúÊ±ÖÓ
				p8=(u8 *)(ADDR_AFN0CF11);//ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢
				p8+=(19*(RS485PORT-1));
				MR(((u32)p8)+8,ADDR_AFN0CF2,6);//¿ªÊ¼Ê±¼ä
				MC(0xee,((u32)p8)+14,6);//½áÊøÊ±¼ä
			*/
			}
		}
		else
		{//ÊÇÄÚ²¿½»Á÷²ÉÑùÍ¨ĞÅ¿Ú
			p16timer[0]=1;//³­±í¼ä¸ôÊ±¼ä¹Ì¶¨Îª1s
		}
	}

	if(UARTCtrl->FnEnd==0)//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
	{
		if(AFN04F10MaxPn>100)
		{
			y=100;//±¾´Î×î´ó²éÕÒ100Ö»±í
		}
		else
		{
			y=AFN04F10MaxPn;
		}
		for(x=0;x<y;x++)
		{
			UARTCtrl->MeterNo++;
			if(UARTCtrl->MeterNo>AFN04F10MaxPn)
			{
				//È«²¿±í1ÂÖ³­Íê
			#if ((USER/100)==8)||((USER/100)==7)//ÓÃ»§±íÊ¾£ºÍ¨ÓÃ¡¢Õã½­						
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
				{//Ñ­³­ÈÎÎñ½áÊø
					switch(PORTn)
					{
						case RS485_1PORT://RS485-1
							i=0;
							break;
						case RS485_2PORT://RS485-2
							i=1;
							break;
					}
					i=MRR(ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
					switch(i)
					{
						case 1:
							switch(PORTn)
							{
								case RS485_1PORT://RS485-1
									i=0;
									break;
								case RS485_2PORT://RS485-2
									i=1;
									break;
							}
							MWR(2,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
							break;
						case 4:
							switch(PORTn)
							{
								case RS485_1PORT://RS485-1
									i=0;
									break;
								case RS485_2PORT://RS485-2
									i=1;
									break;
							}
							MWR(5,ADDR_AFN0CF250_Ext1+10*MaxRS485AddCarrierPn+1+i,1);
							break;
					}
				}
			#endif	

			#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
			#if (USER/100)==5//ÉÏº£;µçÄÜ±íÊÂ¼ş²É¼¯·½°¸Áí¶¨Òå
				if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
				{//ÊÇÑ­³­
					if(UARTCtrl->RMTaskFlags&1)//µ±Ç°³­¶ÁÈÎÎñÊı¾İ·ÖÀà(Í¬³­±í¿â):B6=²¹³­ÉÏ2-3ÈÕ¶³½á,B5=ÔÂ¶³½á,B4=³­±íÈÕ¶³½á,B3=ÈÕ¶³½á,B2=ÇúÏß¶³½á,B1=Ğ¡Ê±¶³½á,B0=ÊµÊ±
					{//ÊÇ³­ÊµÊ±
						//Ã¿¸ö³­±íÖÜÆÚÊı¾İÏî²É¼¯Íê³Éºó,³­1¼¶ÊÂ¼ş
						UARTCtrl->RMEventTaskN|=1;//ĞÂÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼şÈÎÎñb0-b2·Ö±ğÎªÊÂ¼ş·Ö¼¶1-3
						//Ã¿ÌìµÚ1ÂÖ³­Íê,³­2¼¶ÊÂ¼ş
						if((UARTCtrl->RMEventTaskN&0x20)!=0)
						{
							UARTCtrl->RMEventTaskN&=~0x20;
							UARTCtrl->RMEventTaskN|=2;//ĞÂÖÜÆÚ³­¶ÁµçÄÜ±íÊÂ¼şÈÎÎñb0-b2·Ö±ğÎªÊÂ¼ş·Ö¼¶1-3,;b6=ÔÂ¸üĞÂ(ÉÏº£Ã¿ÔÂ³­ÊÂ¼şÓÃ)
						}
						//Ã¿ÔÂµÚ1ÂÖ³­Íê,³­3¼¶ÊÂ¼ş
						if((UARTCtrl->RMEventTaskN&0x40)!=0)
						{//ÔÂ¸üĞÂ
							UARTCtrl->RMEventTaskN&=~0x40;
							UARTCtrl->RMEventTaskN|=4;
						}
					}
				}
			#endif
			#endif
				UARTCtrl->MeterNo=0;//Íê³É1¸öÑ­»·³­±í,µçÄÜ±í/½»Á÷²ÉÑù×°ÖÃĞòºÅ=0
				if(RS485PORT==0)
				{//ÊÇÄÚ²¿½»Á÷²ÉÑùÍ¨ĞÅ¿Ú
					i=UARTCtrl->RMComplete;
					if((i&0x0f)<0x0f)
					{
						i+=1;//Íê³ÉÑ­»·³­±í´ÎÊı+1
					}
					i|=0x80;
					UARTCtrl->RMComplete=i;
					UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
				}
				else
				{//ÊÇÍâ²¿485¿Ú
					if(UARTCtrl->RMTaskC==(MaxRMTask-1))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
					{//Ñ­³­ÈÎÎñ½áÊø
						i=UARTCtrl->RMComplete;
						if((i&0x0f)<0x0f)
						{
							i+=1;//Íê³ÉÑ­»·³­±í´ÎÊı+1
						}
						i|=0x80;
						UARTCtrl->RMComplete=i;
					#if DateRMTask==0//Ã¿ÈÕ³­±íÈÎÎñ;0=Ã»ÓĞ,1=ÓĞ
						//ÔİÍ£Ç°ÌîÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢,Òò»áÇå0Íê³É±êÖ¾
						Terminal_AFN0CF11_RS485(PORTn,MaxRMTask-1);//ÖÕ¶ËÀà1Êı¾İF11,ÖÕ¶Ë¼¯ÖĞ³­±í×´Ì¬ĞÅÏ¢;Èë¿ÚÒª¼ÆËãµÄ³­±íÈÎÎñºÅ
					#endif
						ClrRMTaskCompletes_RS485(PORTn,MaxRMTask-1,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
						UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
					}
					else
					{//ÊÇ²åÈë¸ßÓÅÏÈ¼¶³­±í½áÊø
						if(UARTCtrl->RMTaskC!=0)//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
						{//²»ÊÇÉÏµç
							//»Ö¸´Ñ­³­
							UARTCtrl->FnCount=UARTCtrl->FnCountS;//485³­±íÔ­Ñ­³­ÈÎÎñµÄFn¼ÆÊı
							i=UARTCtrl->MeterNoS;//485³­±íÔ­Ñ­³­ÈÎÎñµÄÅäÖÃĞòºÅ¼ÆÊı(°ë×Ö¶ÔÆë)
							if(i!=0)
							{
								i--;
							}
							UARTCtrl->MeterNo=i;
							//Á¢¼´²¹³­Ã¿ÈÕÈÎÎñ
							if(UARTCtrl->DateReRMCount<2)//Ã¿ÈÕÈÎÎñ(°üÀ¨²¹³­ÈÎÎñ)µÄÖ´ĞĞ×Ü´ÎÊı;0=¿Õ(ÉÏµç»òÈÕ¸üĞÂ),1-255=×Ü´ÎÊı¼ÆÊı
							{
								i=NoRMCompletes_RS485(PORTn,1);//RS485Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
								if(i!=0)
								{//Ğè²¹³­
									UARTCtrl->MeterNo++;
									UARTCtrl->RMTaskN|=2;//ĞÂÃ¿ÈÕÈÎÎñ
								}
							}
						#if EventProject==1//ÊÂ¼ş¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ĞÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼ş¼ÇÂ¼²É¼¯¹æÔò
							if(UARTCtrl->RMTaskC==(MaxRMTask-2))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
							{
								DelRMEvene(PORTn);//É¾³ı¶ÁµçÄÜ±íÊÂ¼şÈÎÎñ
								if(UARTCtrl->RMTaskC==(MaxRMTask-2))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,4=³­ÇúÏß,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
								{//»Ö¸´ºó»¹ÊÇ³­¶ÁÉÏµÍ1¼¶µÄµçÄÜ±íÊÂ¼ş
									return;
								}
							#if (USER/100)==5//ÉÏº£;µçÄÜ±íÊÂ¼ş²É¼¯·½°¸Áí¶¨Òå
							//ÉÏº£ÔÚÑ­³­½áÊøºó³­ÊÂ¼ş,ÊÂ¼ş½áÊøºóĞèÔİÍ£
								UARTCtrl->RMFlag=0;//0=³­±í¼ä¸ôÔİÍ£,1=³­±í½øĞĞÖĞ
							#endif
							}
						#endif
						}
						if((UARTCtrl->RMTaskC==0)||(UARTCtrl->RMTaskC==3))//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
						{//ÊÇ³­Í£µç¼ÇÂ¼ÈÎÎñ½áÊø
							if((Terminal_Ram->PowerTask==1)||(Terminal_Ram->PowerTask==10))//10=µÈ´ı³­±íÍê³É±È¶ÔµçÄÜ±í
							{//ÊÂ¼şÈÎÎñ:µÈ´ı³­±íÍê³É±È¶ÔµçÄÜ±í
								//i=NoRMCompletes_RS485(PORTn,UARTCtrl->RMTaskC);//RS485Î´³­ÊıºÍÊ§°ÜÊı;Èë¿Ú:RMTask=ÈÎÎñºÅ;·µ»Ø:b0-b15Î´³­Êı,b16-b31Ê§°ÜÊı
								//if(i!=0)
								//{//Ã»È«³­³É¹¦
									if(UARTCtrl->RMTaskC==0)
									{
										if(p16timer[8]!=0)//Í£µçÊÂ¼ş³­¶ÁÊ±¼äÏŞÖµÃë¶¨Ê±Æ÷
										{
											ClrRMTaskCompletes_RS485(PORTn,0,1);//Çå¶Ë¿Ú³­±íÈÎÎñÍê³É±êÖ¾;Èë¿Ú:PORTn=¶Ë¿ÚºÅ,RMTask=ÈÎÎñºÅ,All=0Ö»ÇåÊ§°Ü,=1ÇåÈ«²¿
											return;
										}
									}
								//}
							}
							//ÏÂ´Î³­Í£µç¼ÇÂ¼¼ä¸ôÊ±¼ä
							i=MRR(ADDR_AFN04F97+1,1);//Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô	BIN	1	µ¥Î»ÎªĞ¡Ê±
							p16=(u16*)(&Terminal_Ram->RS4851RMF246_M_Timer);//25 RS485-1Í£µçÊÂ¼ş³­¶ÁÊ±¼ä¼ä¸ô·Ö¶¨Ê±Æ÷
							p16[RS485PORT-1]=i*60;//·Ö
							x=MRR(ADDR_TYMDHMS+2,4);
							x=YMDH_AddH(x,i);//ÄêÔÂÈÕÊ±¼ÓhÊ±,·µ»Ø¼ÓhÊ±ºóµÄÄêÔÂÈÕÊ±
							d64a=x;
							d64a<<=16;//
							MWR(d64a,(u32)&UARTCtrl->NextTimerRMTime,6);//¶¨Ê±³­Í£µç¼ÇÂ¼¼ä¸ôÖÜÆÚÏÂ´Î³­±íÊ±¼äÃë·ÖÊ±ÈÕÔÂÄê
						}
						UARTCtrl->RMTaskC=(MaxRMTask-1);//µ±Ç°³­±íÈÎÎñºÅ0-3:0=ÉÏµçÈÎÎñ,1=Ã¿ÈÕÈÎÎñ,2=Ã¿ÖÜÈÎÎñ,3=³­Í£µç¼ÇÂ¼,(MaxRMTask-1)=Ñ­³­ÈÎÎñ
					}
					NewTask_RS485(PORTn);//RS485¿ÚÖ´ĞĞĞÂÈÎÎñ;·µ»Ø£º0=Ã»Ö´ĞĞĞÂÈÎÎñ£¬1=Ö´ĞĞĞÂÈÎÎñ
				}
				return;
			}
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+5,1);//Í¨ĞÅĞ­ÒéÀàĞÍ BIN 1 
			//if(i==MRR(ADDR_Protocol_No,1))
			if(i==2)
			{//½»Á÷²ÉÑù×°ÖÃÍ¨ĞÅĞ­Òé
				if(PORTn==ACSAMPLEPORT)
				{//µ±Ç°¿ÚÊÇÄÚ²¿²ÉÑù¿Ú
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//ËùÊô²âÁ¿µãºÅ BIN 2 
					if((i!=0x0)&&(i<=MaxRS485AddCarrierPn))
					{//ËùÊô²âÁ¿µãºÅ!=0x0
						//Æô¶¯·¢ËÍ
						UARTCtrl->FnEnd=1;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
						return;
					}
				}
			}
			else
			{
				if(PORTn!=ACSAMPLEPORT)
				{//µ±Ç°¿Ú²»ÊÇÄÚ²¿²ÉÑù¿Ú
					i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);//Í¨ĞÅËÙÂÊ¼°¶Ë¿ÚºÅ
					i&=0x1f;
					if(i==(RS485PORT+1))
					{//Í¨ĞÅ¶Ë¿ÚºÅÏàÍ¬
						i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);//ËùÊô²âÁ¿µãºÅ BIN 2 
						if((i!=0x0)&&(i<=MaxRS485AddCarrierPn))
						{//ËùÊô²âÁ¿µãºÅ!=0x0
							//Æô¶¯·¢ËÍ
							UARTCtrl->FnEnd=1;//485³­±íFn¼ÆÊı½áÊø±êÖ¾,0=½áÊø,1=Ã»½áÊø
							return;
						}
					}
				}
			}
		}
		return;
	}
	if((Terminal_Ram->CongealFlags&0x1f)==0x1f)//B0=Ğ¡Ê±¶³½á,B1=ÇúÏß¶³½á,B2=ÈÕ¶³½á,B3=³­±íÈÕ¶³½á,B4=ÔÂ¶³½á
	{//³­±íÔÚ¶³½áºó½øĞĞ
		ReadMeter_Protocol(PORTn);//³­±í¹æÔ¼
	}
}



















