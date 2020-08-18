
//÷’∂À¿‡2 ˝æ›Fn
#include "Project.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "KeyUser.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"




extern Terminal_Class2DataLib_TypeDef Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);



u32 Class2Data_FnOnly(u32 ClassNo,u32 Fn)//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
{
	u32 i;
	u8 * p8;
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F39+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
	i=MRR((u32)p8,1);
	if(i>=((Fn/8)+1))
	{//–≈œ¢¿‡◊È ˝n(0°´31)
		p8+=1;
		p8+=(Fn/8);
		i=MRR((u32)p8,1);
		i>>=(Fn%8);
		i&=0x1;
		return(i);
	}
	return(0);
}

u32 Class2Data_Fn(u32 ClassNo,u32 Fn)//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
{
	u32 i;
	u8 * p8;
	
	if(Fn==0x0)
	{
		return(0);
	}
	Fn-=1;
	p8=(u8 *)(ADDR_AFN04F39+((ClassNo>>4)*32*16)+((ClassNo&0x0f)*32));
	i=MRR((u32)p8,1);
	if(i>=((Fn/8)+1))
	{//–≈œ¢¿‡◊È ˝n(0°´31)
		p8+=1;
		p8+=(Fn/8);
		i=MRR((u32)p8,1);
		i>>=(Fn%8);
		i&=0x1;
	}
	else
	{
		i=0;
	}
	if(i!=0x0)
	{
		return(1);
	}
	else
	{
		Fn++;
		switch(Fn)
		{
//F1 »’∂≥Ω·’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 161://F161 »’∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 162://F162 »’∂≥Ω·’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 165://F165 »’∂≥Ω·“ªœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 168://F168 »’∂≥Ω·ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
				i=Class2Data_FnOnly(ClassNo,1);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
//F2 »’∂≥Ω·∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 163://F163 »’∂≥Ω·∑¥œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 164://F164 »’∂≥Ω·∑¥œÚŒﬁπ¶£®◊È∫œŒﬁπ¶2£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 166://F166 »’∂≥Ω·∂˛œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 167://F167 »’∂≥Ω·»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
				i=Class2Data_FnOnly(ClassNo,2);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
//F3 »’∂≥Ω·’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 185://»’∂≥Ω·’˝œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 186://»’∂≥Ω·’˝œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
				i=Class2Data_FnOnly(ClassNo,3);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
//F4 »’∂≥Ω·∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 187://»’∂≥Ω·∑¥œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 188://»’∂≥Ω·∑¥œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
				i=Class2Data_FnOnly(ClassNo,4);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
//F17 ‘¬∂≥Ω·’˝œÚ”–/Œﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 177://‘¬∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 178://‘¬∂≥Ω·’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 181://‘¬∂≥Ω·“ªœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 184://‘¬∂≥Ω·ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
				i=Class2Data_FnOnly(ClassNo,17);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
//F18 ‘¬∂≥Ω·∑¥œÚ”–/Œﬁπ¶£®◊È∫œŒﬁπ¶2£©µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 179://‘¬∂≥Ω·∑¥œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 180://‘¬∂≥Ω·∑¥œÚŒﬁπ¶£®◊È∫œŒﬁπ¶2£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 182://‘¬∂≥Ω·∂˛œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			case 183://‘¬∂≥Ω·»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
				i=Class2Data_FnOnly(ClassNo,18);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
//F19 ‘¬∂≥Ω·’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 193://‘¬∂≥Ω·’˝œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 194://‘¬∂≥Ω·’˝œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
				i=Class2Data_FnOnly(ClassNo,19);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
//F20 ‘¬∂≥Ω·∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 195://‘¬∂≥Ω·∑¥œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			case 196://‘¬∂≥Ω·∑¥œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
				i=Class2Data_FnOnly(ClassNo,20);//2¿‡ ˝æ›Fn≈‰÷√,»Îø⁄ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Fn;∑µªÿ0=≤ª÷ß≥÷,1=÷ß≥÷
				return(i);
		}
	}

	return(0);
}


u32 GetClass2DataOfficeAddr(u32 Fn,u32 Pn)//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
{
	u32 Addr;
	
	if(Fn>=Get_Class2Data_MaxFn())
	{
		return(0);
	}
	if(Pn>Terminal_Class2DataLib[Fn].PnMax)
	{
		if(Fn==246)
		{//AFN0DF246µ±«∞µÙµÁº«¬º ˝æ›”≥…‰µƒPn
			if(Pn>MaxRS485AddCarrierPn)
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	if((Terminal_Class2DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//±æ∑Ω∞∏Œﬁ¥ÀFnπ¶ƒ‹
		return(0);
	}
	if(Terminal_Class2DataLib[Fn].PnType==0x0)
	{
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	}
	return(Addr);
}

u32 GetClass2DataF246OfficeAddr(u32 CongealNo,u32 Pn)//»°¿‡2 ˝æ›F246¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;
	u32 Addr;
	
	Addr=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+ADDR_AFN0DF246Pn;
	for(i=0;i<F246MaxPn;i++)
	{
		x=MRR(Addr+(2*i),2);
		if(x==Pn)
		{
			Addr=Terminal_Class2DataLib[246].OfficeAddr+(Terminal_Class2DataLib[246].PnLen*i);
			return(Addr);
		}
	}
	return 0;
}

u32 GetClass2DataOfficeAddr_MapPn(u32 Fn,u32 Pn)//»°«˙œﬂ”≥…‰Pnµƒ¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
{
	u32 Addr;
	
	if(Fn>=Get_Class2Data_MaxFn())
	{
		return(0);
	}
//	if(Pn>Terminal_Class2DataLib[Fn].PnMax)
//	{
//		return(0);
//	}
	if((Terminal_Class2DataLib[Fn].project&(1<<(Project/100)))==0x0)
	{//±æ∑Ω∞∏Œﬁ¥ÀFnπ¶ƒ‹
		return(0);
	}
	if(Terminal_Class2DataLib[Fn].PnType==0x0)
	{
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr;
	}
	else
	{
		if(Pn==0x0)
		{
			return(0);
		}
		Addr=Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*(Pn-1));
	}
	return(Addr);
}


u32 GetClass2DataCongealType(u32 Fn)//»°¿‡2 ˝æ›∂≥Ω·¿‡–Õ
{
	return(Terminal_Class2DataLib[Fn].CongealType);
}

u32 GetClass2DataLen(u32 Fn,u32 Pn,u32 DataAddr)//»°¿‡2 ˝æ›≥§∂»
{
	u32 Len;
	u32 *p32;
	u32 Tariff;
	
	p32=(u32*)(ADDR_Terminal_ClassDataLen);//1¿‡ ˝æ›≥§∂»ºƒ¥Ê∆˜ªÚ2¿‡ ˝æ›∑—¬ ºƒ¥Ê∆˜(‘≠2¿‡ ˝æ›÷±Ω”∏ƒ–¥FLASH,ºØ÷–∆˜µƒ29GL–¥–Ë0.5√Î‘Ÿ»Ù»´≤ø≤‚¡øµ„Ã´≥§Ã´≥§ ±º‰,π ‘ˆº”∑—¬ ºƒ¥Ê∆˜)
	p32[0]=0;
#if DefaultTariff==0//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
		Tariff=Get_SetTariff(Pn);//»°AFN04F10÷–≈‰÷√µƒ∑—¬  ˝
	#else
		Tariff=TMaxTariff;
	#endif
#endif
	switch(Fn)
	{
		case 1://»’∂≥Ω·’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 2://»’∂≥Ω·∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 9://≥≠±Ì»’∂≥Ω·’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 10://≥≠±Ì»’∂≥Ω·∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 17://‘¬∂≥Ω·’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 18://‘¬∂≥Ω·∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
#if AFN0DF1MaxPn==0
		//“‘œ¬Fn‘⁄ºØ÷–≥≠±ÌŒ™Ω⁄ °FLASHΩ´MaxPn…ËŒ™0,µ´ø…”…∆‰À˚∑÷œÓ∫œ≥…,∫œ≥…∫Û ˝æ›∑≈ADDR_DATABUFF
			Len=MRR(ADDR_DATABUFF+5,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#endif
			Len=6+(17*(Len+1));
#else
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			Len=MRR(DataAddr+5,1);
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(17*(Len+1));
#endif
			break;
		case 3://»’∂≥Ω·’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 4://»’∂≥Ω·∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 11://≥≠±Ì»’∂≥Ω·’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 12://≥≠±Ì»’∂≥Ω·∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 19://‘¬∂≥Ω·’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
		case 20://‘¬∂≥Ω·∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©	
#if AFN0DF1MaxPn==0
		//“‘œ¬Fn‘⁄ºØ÷–≥≠±ÌŒ™Ω⁄ °FLASHΩ´MaxPn…ËŒ™0,µ´ø…”…∆‰À˚∑÷œÓ∫œ≥…,∫œ≥…∫Û ˝æ›∑≈ADDR_DATABUFF
			Len=MRR(ADDR_DATABUFF+5,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				MC(Len,ADDR_DATABUFF+5,1);
			}
		#endif
			Len=6+(14*(Len+1));
#else
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(14*(Len+1));
#endif
			break;
		case 5://»’∂≥Ω·’˝œÚ”–π¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 6://»’∂≥Ω·’˝œÚŒﬁπ¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 7://»’∂≥Ω·∑¥œÚ”–π¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 8://»’∂≥Ω·∑¥œÚŒﬁπ¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 21://‘¬∂≥Ω·’˝œÚ”–π¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 22://‘¬∂≥Ω·’˝œÚŒﬁπ¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 23://‘¬∂≥Ω·∑¥œÚ”–π¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 24://‘¬∂≥Ω·∑¥œÚŒﬁπ¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
	#if AFN0DF5MaxPn==0
		//º∆À„µƒµÁƒ‹¡ø∑—¬ “™πÃ∂®,∑Ò‘Ú»°Õ®–≈≥§∂»≤ª»∑∂®
		#if DefaultTariff==0//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			Tariff=TMaxTariff;
		#endif
			Len=Tariff;//πÃ∂®µƒ∑—¬  ˝
			p32[0]=0x10000+Len;
			Len=1+(4*(Len+1));
	#else
			Len=MRR(DataAddr,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10000+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10000+Len;
			}
		#endif
			Len=1+(4*(Len+1));
	#endif
			break;
		case 58://»’∂≥Ω·◊‹º”◊È»’¿€º∆”–π¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 59://»’∂≥Ω·◊‹º”◊È»’¿€º∆Œﬁπ¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 61://‘¬∂≥Ω·◊‹º”◊È»’¿€º∆”–π¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
		case 62://‘¬∂≥Ω·◊‹º”◊È»’¿€º∆Œﬁπ¶µÁƒ‹¡ø£®◊‹°¢∑—¬ 1°´M£©
			Len=MRR(DataAddr,1);
			if(Len>TMaxTariff)
			{
				Len=0;//TMaxTariff;
				p32[0]=0x10000+Len;
			}
			Len=1+(4*(Len+1));
			break;
		case 121://»’∂≥Ω·Aœ‡–≥≤®‘Ωœﬁ»’Õ≥º∆ ˝æ›
		case 122://»’∂≥Ω·Bœ‡–≥≤®‘Ωœﬁ»’Õ≥º∆ ˝æ›
		case 123://»’∂≥Ω·Cœ‡–≥≤®‘Ωœﬁ»’Õ≥º∆ ˝æ›
			Len=MRR(DataAddr,1);
			if(Len>19)
			{
				p32[0]=0x10000+19;
			}
			Len=Terminal_Class2DataLib[Fn].PnLen;
			break;
		case 161://»’∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 163://»’∂≥Ω·∑¥œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 169://≥≠±Ì»’∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 171://≥≠±Ì»’∂≥Ω·∑¥œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 177://‘¬∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 179://‘¬∂≥Ω·∑¥œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 201://‘¬∂≥Ω·µ⁄1 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 202://‘¬∂≥Ω·µ⁄2 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 203://‘¬∂≥Ω·µ⁄3 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 204://‘¬∂≥Ω·µ⁄4 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 205://‘¬∂≥Ω·µ⁄5 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 206://‘¬∂≥Ω·µ⁄6 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 207://‘¬∂≥Ω·µ⁄7 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 208://‘¬∂≥Ω·µ⁄8 ±«¯∂≥Ω·’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(5*(Len+1));
			break;
		case 162://»’∂≥Ω·’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 164://»’∂≥Ω·∑¥œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 165://»’∂≥Ω·1œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 166://»’∂≥Ω·2œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 167://»’∂≥Ω·3œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 168://»’∂≥Ω·4œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 170://≥≠±Ì»’∂≥Ω·’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 172://≥≠±Ì»’∂≥Ω·∑¥œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 173://≥≠±Ì»’∂≥Ω·1œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 174://≥≠±Ì»’∂≥Ω·2œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 175://≥≠±Ì»’∂≥Ω·3œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 176://≥≠±Ì»’∂≥Ω·4œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 178://‘¬∂≥Ω·’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 180://‘¬∂≥Ω·∑¥œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 181://‘¬∂≥Ω·1œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 182://‘¬∂≥Ω·2œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 183://‘¬∂≥Ω·3œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		case 184://‘¬∂≥Ω·4œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(4*(Len+1));
			break;
		case 185://»’∂≥Ω·’˝œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 186://»’∂≥Ω·’˝œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 187://»’∂≥Ω·∑¥œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 188://»’∂≥Ω·∑¥œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 189://≥≠±Ì»’∂≥Ω·’˝œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 190://≥≠±Ì»’∂≥Ω·’˝œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 191://≥≠±Ì»’∂≥Ω·∑¥œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 192://≥≠±Ì»’∂≥Ω·∑¥œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 193://‘¬∂≥Ω·’˝œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 194://‘¬∂≥Ω·’˝œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 195://‘¬∂≥Ω·∑¥œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		case 196://‘¬∂≥Ω·∑¥œÚŒﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(7*(Len+1));
			break;
		case 216://‘¬∂≥Ω·µÁƒ‹±ÌΩ·À„–≈œ¢
			Len=MRR(DataAddr+5,1);
		#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
			if(Len!=Tariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#else
			if(Len>TMaxTariff)
			{
				Len=Tariff;
				p32[0]=0x10500+Len;
			}
		#endif
			Len=6+(10*(Len+1))+5;
			break;
			
		default:
			Len=Terminal_Class2DataLib[Fn].PnLen;
			break;
	}
	return(Len);
}

u32 GetDateCongealNo(u32 YMD)//µ√µΩ»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;
	
	x=MRR(ADDR_TCongealCount+14,1);
	if((x&0x80)!=0x0)
	{
		x=MaxDateCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMD==MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),3))
		{
			return i;//’“µΩ
		}
	}
	return 0xffffffff;
}
u32 GetDateCongealAddr(u32 YMD,u32 Fn,u32 Pn)//µ√µΩ»’∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;
	i=GetDateCongealNo(YMD);//µ√µΩ»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(i==0xffffffff)
	{
		return 0;
	}
	if(Fn==246)
	{//»’∂≥Ω·µÙµÁº«¬º ˝æ›(≤‚¡øµ„”≥…‰)
		x=GetClass2DataF246OfficeAddr(i,Pn);//»°¿‡2 ˝æ›F246¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	}
	else
	{
		x=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	}
	if(x==0)
	{
		return 0;
	}
	return (ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+x);
}

u32 GetRMDateCongealNo(u32 YMD)//µ√µΩ≥≠±Ì»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;

	x=MRR(ADDR_TCongealCount+21,1);
	if((x&0x80)!=0x0)
	{
		x=MaxRMDateCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMD==MRR(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3))
		{
			return i;//’“µΩ
		}
	}
	return 0xffffffff;
}
u32 GetRMDateCongealAddr(u32 YMD,u32 Fn,u32 Pn)//µ√µΩ≥≠±Ì»’∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;
	i=GetRMDateCongealNo(YMD);//µ√µΩ≥≠±Ì»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(i==0xffffffff)
	{
		return 0;
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(x==0)
	{
		return 0;
	}
	return (ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+x);
}

u32 GetMonthCongealNo(u32 YM)//µ√µΩ‘¬∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;

	x=MRR(ADDR_TCongealCount+28,1);
	if((x&0x80)!=0x0)
	{
		x=MaxMonthCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YM==MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),2))
		{
			return i;//’“µΩ
		}
	}
	return 0xffffffff;
}
u32 GetMonthCongealAddr(u32 YM,u32 Fn,u32 Pn)//µ√µΩ‘¬∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;
	i=GetMonthCongealNo(YM);//µ√µΩ‘¬∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(i==0xffffffff)
	{
		return 0;
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(x==0)
	{
		return 0;
	}
	return (ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i)+x);
}
u32 GetCurveCongealNo(u64 YMDHM)//µ√µΩ«˙œﬂ∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;
	
	x=MRR(ADDR_TCongealCount+7,2);
	if(x&0x8000)
	{
		x=MaxCurveCongeal;
	}
	for(i=0;i<x;i++)
	{
		if(YMDHM==MRR(ADDR_CurveCongealList+2+(6*i),5))
		{
			return i;//’“µΩ
		}
	}
	return 0xffffffff;
}

u32 GetCurveCongealAddr(u64 YMDHM,u32 Fn,u32 Pn)//µ√µΩ«˙œﬂ∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	CongealNo=GetCurveCongealNo(YMDHM);//µ√µΩ«˙œﬂ∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(CongealNo==0xffffffff)
	{//√ª’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
		return 0;
	}
	MapPn=KeyUserMapPn(Pn);//ºØ÷–≥≠±Ì÷ÿµ„ªßµƒ”≥…‰Pn(1-MaxKeyUser);»Îø⁄:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return 0;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//∂≥Ω·µƒ≤‚¡øµ„¡–±Ì÷–√ª’“µΩœ‡Õ¨µƒ≤‚¡øµ„∫≈
		//≤È∂≥Ω·¡–±Ì «∑Ò”–ø’ø…”√
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//”–ø’ø…”√
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				break;
			}
		}
		if(MapPn>=MaxKeyUser)
		{//√ªø…”√
			return 0;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//»°«˙œﬂ”≥…‰Pnµƒ¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr==0)
	{
		return 0;
	}
	return (ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr);
}

u32 GetACSampleCurveCongealAddr(u64 YMDHM,u32 Fn)//µ√µΩΩª≤…≤‚¡øµ„«˙œﬂ∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
{
#if MaxACSampleCurveCongeal==0//◊‹∂≥Ω·µ„ ˝
	return 0;
#else
	u32 i;
	u8 *p8;
	u64 d64;
	
	MC(0,ADDR_DATABUFF,7);
	p8=(u8*)(ADDR_DATABUFF);
	p8[3]=1;//00ƒÍ01‘¬01»’00 ±00∑÷00√Î
	p8[4]=1;
	MWR(YMDHM,ADDR_DATABUFF+7,5);
	i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);// ±÷”≤Ó÷µº∆À„,»Îø⁄R0= ±÷”1(√Î∑÷ ±»’‘¬ƒÍ)µÿ÷∑;R1= ±÷”2(√Î∑÷ ±»’‘¬ƒÍ)µÿ÷∑;≥ˆø⁄R0=HEX≤π¬Î ±÷”øÏªÚ¬˝µƒ≤Ó÷µ(√Î), ±÷”1 ±÷”2∑«∑®R0=0, ±÷”2>= ±÷”1≤Ó÷µŒ™’˝, ±÷”2< ±÷”1≤Ó÷µŒ™∏∫
	i/=60;
	i%=MaxACSampleCurveCongeal;//∆´“∆µ„ ˝
	i*=LEN_ACSampleCurve;//∆´“∆µÿ÷∑
	i+=ADDR_ACSampleCurve;//µÿ÷∑
	d64=MRR(i,5);
	if(d64==YMDHM)
	{// ±±Íœ‡Õ¨
		switch(Fn)
		{
			case 89://Aœ‡µÁ—π«˙œﬂ
				return i+5;
			case 90://Bœ‡µÁ—π«˙œﬂ
				return i+7;
			case 91://Cœ‡µÁ—π«˙œﬂ
				return i+9;
		}
	}
	return 0;
#endif
}


u32 CheckCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn)//ºÏ≤È«˙œﬂ∂≥Ω·;∑µªÿ:0= ˝æ›√ª∂≥Ω·,1= ˝æ›“—∂≥Ω·,2=√ª’“µΩ”––ß∂≥Ω·
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	CongealNo=GetCurveCongealNo(YMDHM);//µ√µΩ«˙œﬂ∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	CongealNo=PnCurveCongealNo(CongealNo,Pn);//≤‚¡øµ„∂≥Ω·∫≈”––ß,∑µªÿ=0xffffffff±Ì æŒﬁ–ß(√ª’“µΩ)
	if(CongealNo==0xffffffff)
	{//√ª’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
		return 2;
	}
	MapPn=KeyUserMapPn(Pn);//ºØ÷–≥≠±Ì÷ÿµ„ªßµƒ”≥…‰Pn(1-MaxKeyUser);»Îø⁄:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return 2;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//∂≥Ω·µƒ≤‚¡øµ„¡–±Ì÷–√ª’“µΩœ‡Õ¨µƒ≤‚¡øµ„∫≈
		//≤È∂≥Ω·¡–±Ì «∑Ò”–ø’ø…”√
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//”–ø’ø…”√
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				return 0;//∑µªÿ:0= ˝æ›√ª∂≥Ω·
			}
		}
		if(MapPn>=MaxKeyUser)
		{//√ªø…”√
			return 2;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//»°«˙œﬂ”≥…‰Pnµƒ¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//œ‡Õ¨ ±±Í“—≥≠
		return 1;
	}
	return 0;
}

void AddCurveCongeal(u64 YMDHM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
{
	u32 CongealNo;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;

	CongealNo=GetCurveCongealNo(YMDHM);//µ√µΩ«˙œﬂ∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(CongealNo==0xffffffff)
	{//√ª’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
		return;
	}
	MapPn=KeyUserMapPn(Pn);//ºØ÷–≥≠±Ì÷ÿµ„ªßµƒ”≥…‰Pn(1-MaxKeyUser);»Îø⁄:Pn=1-MaxRS485AddCarrierPn
	if(MapPn==0)
	{
		return;
	}
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	p16=(u16 *)(ADDR_PnVSSetList);
	for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
	{
		if(Pn==p16[MapPn])
		{
			break;
		}
	}
	if(MapPn>=MaxKeyUser)
	{//∂≥Ω·µƒ≤‚¡øµ„¡–±Ì÷–√ª’“µΩœ‡Õ¨µƒ≤‚¡øµ„∫≈
		//≤È∂≥Ω·¡–±Ì «∑Ò”–ø’ø…”√
		for(MapPn=0;MapPn<MaxKeyUser;MapPn++)
		{
			if(p16[MapPn]==0xffff)
			{//”–ø’ø…”√
				DMWR(Pn,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal+(2*MapPn),2);
				break;
			}
		}
		if(MapPn>=MaxKeyUser)
		{//√ªø…”√
			return;
		}
	}
	MapPn+=1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//»°«˙œﬂ”≥…‰Pnµƒ¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,Byte);
}

u64 CurveCongeal_YMDHM(void)//µ±«∞±Í◊º«˙œﬂ∂≥Ω· ±º‰YMDHM
{
	u32 i;
	u64 YMDHM;
	i=Comm_Ram->TYMDHMS[1];
	i=bcd_hex(i);
	i/=CurveCongealTime;
	i*=CurveCongealTime;
	i=hex_bcd(i);
	YMDHM=MRR(ADDR_TYMDHMS+2,4);
	YMDHM<<=8;
	YMDHM|=i;//µ±«∞±Í◊º«˙œﬂ∂≥Ω·µ„µƒYMDHM
	return YMDHM;
}

u32 CheckDateCongeal(u32 YMD,u32 Fn,u32 Pn)//ºÏ≤È»’∂≥Ω·;∑µªÿ:0= ˝æ›√ª∂≥Ω·,1= ˝æ›“—∂≥Ω·,2=√ª’“µΩ”––ß∂≥Ω·
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetDateCongealNo(YMD);//µ√µΩ»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	CongealNo=PnDateCongealNo(CongealNo,Pn);//≤‚¡øµ„∂≥Ω·∫≈”––ß,∑µªÿ=0xffffffff±Ì æŒﬁ–ß(√ª’“µΩ)
	if(CongealNo==0xffffffff)
	{//√ª’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
		return 2;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//œ‡Õ¨ ±±Í“—≥≠
		return 1;
	}
	return 0;
}

void AddDateCongeal(u32 YMD,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//≤π»’∂≥Ω·µ„ ˝æ›
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetDateCongealNo(YMD);//µ√µΩ»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(CongealNo==0xffffffff)
	{//√ª’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*CongealNo)+OfficeAddr,Byte);
}

u32 CheckMonthCongeal(u32 YM,u32 Fn,u32 Pn)//ºÏ≤È‘¬∂≥Ω·;∑µªÿ:0= ˝æ›√ª∂≥Ω·,1= ˝æ›“—∂≥Ω·,2=√ª’“µΩ”––ß∂≥Ω·
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetMonthCongealNo(YM);//µ√µΩ‘¬∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	CongealNo=PnMonthCongealNo(CongealNo,Pn);//≤‚¡øµ„∂≥Ω·∫≈”––ß,∑µªÿ=0xffffffff±Ì æŒﬁ–ß(√ª’“µΩ)
	if(CongealNo==0xffffffff)
	{//√ª’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
		return 2;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr==0)
	{
		return 2;
	}
	Pn=MRR(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*CongealNo)+OfficeAddr,1);
	if(Pn!=0xff)
	{//œ‡Õ¨ ±±Í“—≥≠
		return 1;
	}
	return 0;
}

void AddMonthCongeal(u32 YM,u32 Fn,u32 Pn,u32 ADDR_Data,u32 Byte)//≤π‘¬∂≥Ω·µ„ ˝æ›
{
	u32 CongealNo;
	u32 OfficeAddr;

	CongealNo=GetMonthCongealNo(YM);//µ√µΩ‘¬∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(CongealNo==0xffffffff)
	{//√ª’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
		return;
	}
	OfficeAddr=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr==0)
	{
		return;
	}
	DMW(ADDR_Data,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*CongealNo)+OfficeAddr,Byte);
}









//2¿‡ ˝æ›.»’∂≥Ω·
//F1≤‚¡øµ„’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//F2≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//2¿‡ ˝æ›.≥≠±Ì»’∂≥Ω·
//F9≤‚¡øµ„’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//F10≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//2¿‡ ˝æ›.‘¬∂≥Ω·
//F17≤‚¡øµ„’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//F18≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
void Class2Group_F1_F2_F9_F10_F17_F18(u32 CongealAddr,u32 Pn,u32 Fn1,u32 Fn2,u32 Fn3,u32 Fn4)//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 RMTime;
	u8 * p8;
	u32 Tariff;
#if DefaultTariff==0//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
		Tariff=Get_SetTariff(Pn);//»°AFN04F10÷–≈‰÷√µƒ∑—¬  ˝
	#else
		Tariff=TMaxTariff;
	#endif
#endif

	RMTime=0;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
	p8=(u8*)ADDR_DATABUFF;
	//’˝œÚ”–π¶
	i=GetClass2DataOfficeAddr(Fn1,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(i==0)
	{//√ª’“µΩ
		MC(0xee,ADDR_DATABUFF,6+5);
		x=Tariff;//∑—¬  ˝
		p8[5]=x;//∑—¬  ˝
	}
	else
	{//’“µΩ
		x=MRR(CongealAddr+i+5,1);//∑—¬  ˝
	#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
		if(x!=Tariff)
		{
			x=Tariff;//∑—¬  ˝
		}
	#else
		if(x>TMaxTariff)
		{
			x=Tariff;//∑—¬  ˝
		}
	#endif
		MR(ADDR_DATABUFF,CongealAddr+i,6+((x+1)*5));
		Data_0xFFto0xEE(ADDR_DATABUFF,6+((x+1)*5));//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		p8[5]=x;//∑—¬  ˝
		if(p8[0]!=0xee)
		{
			RMTime=1;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
		}
	}
	//’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©
	i=GetClass2DataOfficeAddr(Fn2,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(i==0)
	{//√ª’“µΩ
		MC(0xee,ADDR_DATABUFF+6+((x+1)*5),(x+1)*4);
	}
	else
	{//’“µΩ
		MR(ADDR_DATABUFF+6+((x+1)*5),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*5),(x+1)*4);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		if(RMTime==0)//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
		{
			if(p8[6+((x+1)*5)]!=0xee)
			{
				RMTime=1;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
	//“ªœÛœﬁŒﬁπ¶
	i=GetClass2DataOfficeAddr(Fn3,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(i==0)
	{//√ª’“µΩ
		MC(0xee,ADDR_DATABUFF+6+((x+1)*9),(x+1)*4);
	}
	else
	{//’“µΩ
		MR(ADDR_DATABUFF+6+((x+1)*9),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*9),(x+1)*4);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		if(RMTime==0)//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
		{
			if(p8[6+((x+1)*9)]!=0xee)
			{
				RMTime=1;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
	//ÀƒœÛœﬁŒﬁπ¶
	i=GetClass2DataOfficeAddr(Fn4,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(i==0)
	{//√ª’“µΩ
		MC(0xee,ADDR_DATABUFF+6+((x+1)*13),(x+1)*4);
	}
	else
	{//’“µΩ
		MR(ADDR_DATABUFF+6+((x+1)*13),CongealAddr+i+6,(x+1)*4);
		Data_0xFFto0xEE(ADDR_DATABUFF+6+((x+1)*13),(x+1)*4);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		if(RMTime==0)//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
		{
			if(p8[6+((x+1)*13)]!=0xee)
			{
				RMTime=1;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
				MR(ADDR_DATABUFF,CongealAddr+i,5);
			}
		}
	}
}

//2¿‡ ˝æ›.»’∂≥Ω·
//F3≤‚¡øµ„’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//F4≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//2¿‡ ˝æ›.≥≠±Ì»’∂≥Ω·
//F11≤‚¡øµ„’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//F12≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//2¿‡ ˝æ›.‘¬∂≥Ω·
//F19≤‚¡øµ„’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
//F20≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
void Class2Group_F3_F4_F11_F12_F19_F20(u32 CongealAddr,u32 Pn,u32 Fn1,u32 Fn2)//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
{
	u32 i;
	u32 x;
	u32 y;
	u32 RMTime;
	u8 * p8;
	u32 Tariff;
#if DefaultTariff==0//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
	Tariff=0;
#else
	#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
		Tariff=Get_SetTariff(Pn);//»°AFN04F10÷–≈‰÷√µƒ∑—¬  ˝
	#else
		Tariff=TMaxTariff;
	#endif
#endif

	RMTime=0;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
	p8=(u8*)ADDR_DATABUFF;
	//”–π¶
	y=GetClass2DataOfficeAddr(Fn1,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(y==0)
	{//√ª’“µΩ
		MC(0xee,ADDR_DATABUFF,6+7);
		x=Tariff;//∑—¬  ˝
		p8[5]=x;//∑—¬  ˝
	}
	else
	{//’“µΩ
		x=MRR(CongealAddr+y+5,1);//∑—¬  ˝
	#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
		if(x!=Tariff)
		{
			x=Tariff;//∑—¬  ˝
		}
	#else
		if(x>TMaxTariff)
		{
			x=Tariff;//∑—¬  ˝
		}
	#endif
		MR(ADDR_DATABUFF,CongealAddr+y,6);
		Data_0xFFto0xEE(ADDR_DATABUFF,6);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(3*i),CongealAddr+y+6+(7*i),3);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(3*i),3);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		}
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(3*(x+1)+(4*i)),CongealAddr+y+6+(7*i)+3,4);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(3*(x+1)+(4*i)),4);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		}
		p8[5]=x;//∑—¬  ˝
		if(p8[0]!=0xee)
		{
			RMTime=1;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
		}
	}
	//Œﬁπ¶
	y=GetClass2DataOfficeAddr(Fn2,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(y==0)
	{//√ª’“µΩ
		MC(0xee,ADDR_DATABUFF+6+((x+1)*7),(x+1)*7);
	}
	else
	{//’“µΩ
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(7*(x+1))+(3*i),CongealAddr+y+6+(7*i),3);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(7*(x+1))+(3*i),3);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		}
		for(i=0;i<(x+1);i++)
		{
			MR(ADDR_DATABUFF+6+(10*(x+1)+(4*i)),CongealAddr+y+6+(7*i)+3,4);
			Data_0xFFto0xEE(ADDR_DATABUFF+6+(10*(x+1)+(4*i)),4);//±»Ωœ ˝æ›:»´0xFF‘ÚÃÊªªŒ™»´0xEE
		}
		if(RMTime==0)//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
		{
			i=MRR(CongealAddr+y,1);
			if((i!=0xee)&&(i!=0xff))
			{
				RMTime=1;//≥≠±Ì ±º‰ÃÓ»Î±Í÷æ,0=√ª,1=ÃÓ»Î
				MR(ADDR_DATABUFF,CongealAddr+y,5);
			}
		}
	}
}

void Class2_Group(u32 CongealAddr,u32 Fn,u32 Pn)//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
{
	switch(Fn)
	{
	//2¿‡ ˝æ›.»’∂≥Ω·
		case 1://F1≤‚¡øµ„’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,161,162,165,168);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 2://F2≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,163,164,166,167);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 3://F3≤‚¡øµ„’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,185,186);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 4://F4≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,187,188);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
	//2¿‡ ˝æ›.≥≠±Ì»’∂≥Ω·
		case 9://F9≤‚¡øµ„’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,169,170,173,176);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 10://F10≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,171,172,174,175);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 11://F11≤‚¡øµ„’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,189,190);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 12://F12≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,191,192);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
	//2¿‡ ˝æ›.‘¬∂≥Ω·
		case 17://F17≤‚¡øµ„’˝œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,177,178,181,184);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 18://F18≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F1_F2_F9_F10_F17_F18(CongealAddr,Pn,179,180,182,183);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 19://F19≤‚¡øµ„’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,193,194);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
		case 20://F20≤‚¡øµ„∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
			Class2Group_F3_F4_F11_F12_F19_F20(CongealAddr,Pn,195,196);//2¿‡ ˝æ›∫œ≥…;≥ˆø⁄: ˝æ›∑≈ADDR_DATABUFF
			break;
	}
}

void UART_AFN0DF97F98F99F100_Computer(u32 ADDR_FullData,u64 YMDHM,u32 m,u32 Fn,u32 Pn,u32 CongealNo)//2¿‡«˙œﬂ ˝æ›µÁƒ‹¡øº∆À„;»Îø⁄£∫FnŒ™”√”⁄º∆À„µƒ æ÷µ«˙œﬂFn
{
	u32 x;
	u32 y;
	u32 OfficeAddr;
	u32 MapPn;
	u16 *p16;
	
	MC(0xee,ADDR_FullData,4);//«Â ˝æ›
	p16=(u16 *)(ADDR_PnVSSetList);
	MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
	for(x=0;x<MaxKeyUser;x++)
	{
		if(Pn==p16[x])
		{
			break;
		}
	}
	if(x>=MaxKeyUser)
	{//∂≥Ω·µƒ≤‚¡øµ„¡–±Ì÷–√ª’“µΩœ‡Õ¨µƒ≤‚¡øµ„∫≈
		return;
	}
	MapPn=x+1;
	OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//»°«˙œﬂ”≥…‰Pnµƒ¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(OfficeAddr!=0)
	{
		x=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,4);
		if((x!=0xeeeeeeee)&&(x!=0xffffffff))
		{
			YMDHM=YMDHM_SubM(YMDHM,m);//ƒÍ‘¬»’ ±∑÷ºım∑÷,∑µªÿºım∑÷∫ÛµƒƒÍ‘¬»’ ±∑÷
			CongealNo=GetCurveCongealNo(YMDHM);//µ√µΩ«˙œﬂ∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
			CongealNo=PnCurveCongealNo(CongealNo,Pn);//≤‚¡øµ„∂≥Ω·∫≈”––ß,∑µªÿ=0xffffffff±Ì æŒﬁ–ß(√ª’“µΩ)
			if(CongealNo!=0xffffffff)
			{//’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
				MR(ADDR_PnVSSetList,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+ADDR_KeyUserPnListCongeal,2*MaxKeyUser);
				for(y=0;y<MaxKeyUser;y++)
				{
					if(Pn==p16[y])
					{
						break;
					}
				}
				if(y>=MaxKeyUser)
				{//∂≥Ω·µƒ≤‚¡øµ„¡–±Ì÷–√ª’“µΩœ‡Õ¨µƒ≤‚¡øµ„∫≈
					return;
				}
				MapPn=y+1;
				OfficeAddr=GetClass2DataOfficeAddr_MapPn(Fn,MapPn);//»°«˙œﬂ”≥…‰Pnµƒ¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“Ω
				if(OfficeAddr!=0)
				{
					y=MRR(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*CongealNo)+OfficeAddr,4);
					if((y!=0xeeeeeeee)&&(y!=0xffffffff))
					{
						x=bcd_hex(x);
						y=bcd_hex(y);
						if(x<y)
						{//ø…ƒ‹÷µªÿ0
							x+=100000000;
						}
						x-=y;
						x=hex_bcd(x);
						x<<=8;//µÁƒ‹¡øŒ™4Œª–° ˝
						MWR(x,ADDR_FullData,4);
					}
				}
			}
		}
	}
}


#if ((RMM_DateCongeal_0xEE!=0)||(RMM_MonthCongeal_0xEE!=0))//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
void UART_EntreyIncComputer(u32 Pn,u32 ADDR_FullData,u32 Addr1,u32 Addr2,u32 Len,u32 Tariff)//2¿‡»’∂≥Ω· ˝æ›µÁƒ‹¡øº∆À„
{
	u32 x;
	u32 y;
	u64 a64;
	u64 b64;
	
	for(x=0;x<=Tariff;x++)
	{
		//»°µ±«∞÷µ
		b64=MRR(Addr1+6+(Len*x),Len);
		//»°◊ÚÃÏ÷µ
		a64=MRR(Addr2+6+(Len*x),Len);
		y=a64&0xff;
		if((y==0xee)||(y==0xff))
		{//‘≠ º÷µ=ø’
			continue;
		}
		y=b64&0xff;
		if((y==0xee)||(y==0xff))
		{//µ±«∞÷µ=ø’
			continue;
		}
		//‘ˆ¡øº∆À„
		a64=bcd16_hex(a64);
		b64=bcd16_hex(b64);
		if(b64>=a64)
		{//µ±«∞÷µ>=‘≠ º÷µ
			b64-=a64;
		}
		else
		{
			if(Len==5)
			{
				if(a64>9000000000)
				{//µ±«∞÷µªÿ0
					b64+=10000000000;
					b64-=a64;
				}
				else
				{//¥ÌŒÛ
					b64=0;
				}
			}
			else
			{
				if(a64>90000000)
				{//µ±«∞÷µªÿ0
					b64+=100000000;
					b64-=a64;
				}
				else
				{//¥ÌŒÛ
					b64=0;
				}
			}
		}
		b64=hex_bcd16(b64);
		y=b64;
		if(Len==4)
		{
			y<<=8;//–° ˝µ„±‰4Œª
		}
		MWR(y,ADDR_FullData+1+(4*x),4);
	}
}
void UART_AFN0DF5F6F7F8_Computer(u32 ADDR_FullData,u32 YMD,u32 Fn,u32 Pn,u32 Len)//2¿‡»’∂≥Ω· ˝æ›µÁƒ‹¡øº∆À„;»Îø⁄£∫FnŒ™”√”⁄º∆À„µƒ æ÷µFn
{
	u32 Addr1;
	u32 Addr2;
	u32 Tariff;//∑—¬  ˝πÃ∂®Œ™…Ë÷√∑—¬ 

//º∆À„µƒµÁƒ‹¡ø∑—¬ “™πÃ∂®,∑Ò‘Ú»°Õ®–≈≥§∂»≤ª»∑∂®
#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
	Tariff=Get_SetTariff(Pn);//»°AFN04F10÷–≈‰÷√µƒ∑—¬  ˝
#else
	Tariff=TMaxTariff;
#endif
	MC(0xee,ADDR_FullData,1+(4*(Tariff+1)));//«Â ˝æ›
	Addr1=GetDateCongealAddr(YMD,Fn,Pn);//µ√µΩ»’∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
	if(Addr1==0)
	{
		return;
	}
	YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
	Addr2=GetDateCongealAddr(YMD,Fn,Pn);//µ√µΩ»’∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
	if(Addr2==0)
	{
		return;
	}
	UART_EntreyIncComputer(Pn,ADDR_FullData,Addr1,Addr2,Len,Tariff);//2¿‡»’∂≥Ω· ˝æ›µÁƒ‹¡øº∆À„
}
void UART_AFN0DF21F22F23F24_Computer(u32 ADDR_FullData,u32 YM,u32 Fn,u32 Pn,u32 Len)//2¿‡‘¬∂≥Ω· ˝æ›µÁƒ‹¡øº∆À„;»Îø⁄£∫FnŒ™”√”⁄º∆À„µƒ æ÷µFn
{
	u32 Addr1;
	u32 Addr2;
	u32 Tariff;//∑—¬  ˝πÃ∂®Œ™…Ë÷√∑—¬ 

//º∆À„µƒµÁƒ‹¡ø∑—¬ “™πÃ∂®,∑Ò‘Ú»°Õ®–≈≥§∂»≤ª»∑∂®
#if DefaultTariff==1//Œﬁ ˝æ› ±µƒƒ¨»œ∑—¬  ˝:0=∑—¬  ˝Œ™0,1=”√AFN04F10µ»…Ë÷√µƒ∑—¬  ˝,2=◊Ó¥Û∑—¬ TMaxTariff
	Tariff=Get_SetTariff(Pn);//»°AFN04F10÷–≈‰÷√µƒ∑—¬  ˝
#else
	Tariff=TMaxTariff;
#endif
	MC(0xee,ADDR_FullData,1+(4*(Tariff+1)));//«Â ˝æ›
	Addr1=GetMonthCongealAddr(YM,Fn,Pn);//µ√µΩ‘¬∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
	if(Addr1==0)
	{
		return;
	}
	YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
	Addr2=GetMonthCongealAddr(YM,Fn,Pn);//µ√µΩ‘¬∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
	if(Addr2==0)
	{
		return;
	}
	UART_EntreyIncComputer(Pn,ADDR_FullData,Addr1,Addr2,Len,Tariff);//2¿‡»’∂≥Ω· ˝æ›µÁƒ‹¡øº∆À„
}
#endif


void SaveClass2Data(u32 PORTn,u32 Pn,u32 FnDataAddr,u8 *p8RMLib)//¥Ê÷¸¿‡2≤‚¡øµ„≥≠±Ì ˝æ›
{
	u32 i;
	u32 x;
	u32 y;
	u32 Addr;
	u8 *p8FnData;
	u32 Fn;
	u32 YM;
	u32 YMD;
	u64 YMDHM;
	#if (RMM_DateCongeal_0xEE==0)//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
	u32 f;
	u32 *p32;
	#endif
	#if (USER/100)==2//”√ªß±Í ∂:…Ω∂´∞Ê
	u64 LastDayEnergy;
	u64 TodayEnergy;
	#endif
	
	x=x;
	y=y;
	if((u32)p8RMLib==0)
	{
		return;
	}
	if((Pn==0)||(Pn>MaxRS485AddCarrierPn))
	{
		return;
	}
	p8FnData=(u8*)(FnDataAddr);
	i=p8RMLib[0];
	if((i==0xe3)||(i==0xee)||(i==0xef))
	{
		if(p8FnData[0]!=1)
		{//∂≥Ω·»’∆⁄¥ÌŒÛªÚ «DL645-1997±Ì
			return;
		}
	}
	else
	{
		if(p8FnData[6]==0xee)
		{// ˝æ›Œﬁ–ß
			return;
		}
	}

#if (RMM_CompCongealTime==0)//∂¡µÁƒ‹±Ì»’∂≥Ω· ˝æ›±»Ωœ∂≥Ω· ±º‰:0=≤ª±»Ωœ,1=“™±»Ωœ
	//≤ª±»Ωœ
	YMD=MRR(ADDR_TYMDHMS+3,3);//»°µ±«∞ ±º‰ƒÍ‘¬»’
	YM=YMD>>8;
	YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
	switch(i)
	{
		case 0xe3://…œ1»’µÁ±Ì»’∂≥Ω·
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);// ˝æ› ±±Í
			YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
			Addr=GetClass2SourceDataAddr(161,Pn)-3;//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
			break;
		case 0xee://…œ2»’µÁ±Ì»’∂≥Ω·
			YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);// ˝æ› ±±Í
			YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
			Addr=ADDR_LastDate_Source_Start+((Pn-1)*LEN_LastDate_Source_PerPn);
			break;
		case 0xef://…œ3»’µÁ±Ì»’∂≥Ω·
			YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
			YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
			MWR(YMD,((u32)p8FnData)+6+Office_LastDate_Date,3);// ˝æ› ±±Í
			YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
			Addr=ADDR_LastDate_Source_Start+(LEN_LastDate_Source_PerPn*MaxRS485AddCarrierPn)+((Pn-1)*LEN_LastDate_Source_PerPn);
			break;
		default:
			Addr=0;
			break;
	}
#else//
	//“™±»Ωœ
	YMD=MRR(((u32)p8FnData)+6+Office_LastDate_Date,3);// ˝æ› ±±Í
	YM=YMD>>8;
	YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
	YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
	switch(i)
	{
		case 0xe3://…œ1»’µÁ±Ì»’∂≥Ω·
		case 0xee://…œ2»’µÁ±Ì»’∂≥Ω·
		case 0xef://…œ3»’µÁ±Ì»’∂≥Ω·
			Addr=1;
			break;
		default:
			Addr=0;
			break;
	}
#endif

	if(Addr!=0)
	{// «0xe3,0xee,0xef…œ1-3»’µÁ±Ì»’∂≥Ω·
	#if (USER/100)==2//”√ªß±Í ∂:…Ω∂´∞Ê
		if(p8RMLib[0]==0xE3)
		{//ºØ÷–≥≠±Ì»’∂≥Ω·’˝∑¥”–Œﬁπ¶,4∏ˆœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f=GetClass2SourceDataAddr(161,Pn);//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
			//≈–∂œ÷‹∆⁄Œ™»’∂≥Ω·µÁ¡øµƒµÁƒ‹±Ì∑…◊ﬂ,≥¨≤Ó
			i=MRR(f+2,3);//»°‘≠≥≠±ÌƒÍ‘¬»’
			if(i==YMD)
			{// «œ‡¡Ÿ2»’≥≠±Ì
				if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
				{//µ±«∞≥≠±Ì’˝œÚ”–π¶µÁƒ‹ æ÷µ”––ß
					TodayEnergy=MRR(FnDataAddr+6+Office_AFN0DF161_LastDate_Source+6,5);
					LastDayEnergy=MRR(f+6,5);
					if((LastDayEnergy&0xff)!=0xff)//ºØ÷–≥≠≥≠±Ì2¿‡ ˝æ›»’‘¬∂≥Ω·‘≠ º ˝æ›(≥ı º÷µŒ™≤¡≥˝÷µ0xFF)
					{
						DayAveragePower(Pn,0,0);//»’∆Ωæ˘’˝”–π¶π¶¬ º∆À„;»Îø⁄Pn¥”1ø™ º,Power=”–π¶π¶¬ (BCD),Count=±æ¥Œº∆À„¥Œ ˝
						ERC29_Event_Day(Pn,LastDayEnergy,TodayEnergy);//≈–∂œ÷‹∆⁄Œ™»’∂≥Ω·µÁ¡øµƒµÁƒ‹±Ì∑…◊ﬂ,≥¨≤Ó;»Îø⁄Pn=≤‚¡øµ„∫≈(1ø™ º),LastDayEnergy=…œ «∂≥Ω·µÁ¡ø,ToDayEnergy=ΩÒ»’∂≥Ω·µÁ¡ø
					}
				}
			}
		#else
			if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
			{//µ±«∞≥≠±Ì’˝œÚ”–π¶µÁƒ‹ æ÷µ”––ß
				i=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
				i=GetDateCongealNo(i);//µ√µΩ»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
				if(i!=0xffffffff)
				{//’“µΩœ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈
					f=GetClass2DataOfficeAddr(161,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
					if(f!=0)
					{
						TodayEnergy=MRR(FnDataAddr+6+Office_AFN0DF161_LastDate_Source+6,5);
						LastDayEnergy=MRR(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i)+f,5);
						DayAveragePower(Pn,0,0);//»’∆Ωæ˘’˝”–π¶π¶¬ º∆À„;»Îø⁄Pn¥”1ø™ º,Power=”–π¶π¶¬ (BCD),Count=±æ¥Œº∆À„¥Œ ˝
						ERC29_Event_Day(Pn,LastDayEnergy,TodayEnergy);//≈–∂œ÷‹∆⁄Œ™»’∂≥Ω·µÁ¡øµƒµÁƒ‹±Ì∑…◊ﬂ,≥¨≤Ó;»Îø⁄Pn=≤‚¡øµ„∫≈(1ø™ º),LastDayEnergy=…œ «∂≥Ω·µÁ¡ø,ToDayEnergy=ΩÒ»’∂≥Ω·µÁ¡ø
					}
				}
			}
		#endif
		}
	#endif
	
	#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
		MR(ADDR_DATABUFF,Addr,3+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//∂¡‘≠‘≠ º ˝æ›
		MR(ADDR_DATABUFF,ADDR_TYMDHMS+3,3);//÷’∂À≥≠±ÌƒÍ‘¬»’
		f=0;//∏¸–¬±Í÷æ
		i=ADDR_DATABUFF+3;
	#endif
		//F161 ≤‚¡øµ„’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF161_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,161,Pn,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
		i+=6+(5*(1+TMaxTariff));
		//F162 ≤‚¡øµ„’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF162_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,162,Pn,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F163 ≤‚¡øµ„∑¥œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF163_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,163,Pn,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
		i+=6+(5*(1+TMaxTariff));
		//F164 ≤‚¡øµ„∑¥œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF164_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,164,Pn,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F165 ≤‚¡øµ„1œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF165_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,165,Pn,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F166 ≤‚¡øµ„2œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF166_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,166,Pn,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F167 ≤‚¡øµ„3œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF167_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,167,Pn,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
		i+=6+(4*(1+TMaxTariff));
		//F168 ≤‚¡øµ„4œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
		{
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			f|=DataComp(i+2,FnDataAddr+6+Office_AFN0DF168_LastDate_Source+2,3);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
			MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));
		#else
			AddDateCongeal(YMD,168,Pn,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		#endif
		}
	#if (((USER/100)==5)&&(Project==300))//…œ∫£◊®±‰
		//F185 ’˝œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF185_LastDate_Source]!=0xee)
		{
			AddDateCongeal(YMD,185,Pn,FnDataAddr+6+Office_AFN0DF185_LastDate_Source,6+(7*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		}
		//F187 ∑¥œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
		if(p8FnData[6+Office_AFN0DF187_LastDate_Source]!=0xee)
		{
			AddDateCongeal(YMD,187,Pn,FnDataAddr+6+Office_AFN0DF187_LastDate_Source,6+(7*(1+TMaxTariff)));//≤π»’∂≥Ω·µ„ ˝æ›
		}
	#endif
		
	#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
		if(f!=0)
		{//∏¸–¬
			MW(ADDR_DATABUFF,Addr,3+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//–¥‘≠ º ˝æ›
		}
	#endif
		
	#if RMM_MonthCongealUserDate1==1//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›;0=∑Ò,1= «
		//≤π≥≠µΩµƒ‘¬≥ı1»’ ˝æ›◊™Œ™‘¬∂≥Ω·‘≠ º ˝æ›
		if(p8FnData[6]==1)
		{//1»’ ˝æ›
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			Addr=GetClass2SourceDataAddr(177,Pn)-2;//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
			MR(ADDR_DATABUFF,Addr,2+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//∂¡‘≠‘≠ º ˝æ›
			MR(ADDR_DATABUFF,ADDR_TYMDHMS+4,2);//÷’∂À≥≠±ÌƒÍ‘¬
			f=0;//∏¸–¬±Í÷æ
			i=ADDR_DATABUFF+2;
		#endif
			//F177 ≤‚¡øµ„’˝œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF161_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF161_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,177,Pn,FnDataAddr+6+Office_AFN0DF161_LastDate_Source,6+(5*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
			i+=6+(5*(1+TMaxTariff));
			//F178 ≤‚¡øµ„’˝œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF162_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF162_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,178,Pn,FnDataAddr+6+Office_AFN0DF162_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F179 ≤‚¡øµ„∑¥œÚ”–π¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF163_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF163_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,179,Pn,FnDataAddr+6+Office_AFN0DF163_LastDate_Source,6+(5*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
			i+=6+(5*(1+TMaxTariff));
			//F180 ≤‚¡øµ„∑¥œÚŒﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF164_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF164_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,180,Pn,FnDataAddr+6+Office_AFN0DF164_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F181 ≤‚¡øµ„1œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF165_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF165_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,181,Pn,FnDataAddr+6+Office_AFN0DF165_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F182 ≤‚¡øµ„2œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF166_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF166_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,182,Pn,FnDataAddr+6+Office_AFN0DF166_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F183 ≤‚¡øµ„3œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF167_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF167_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,183,Pn,FnDataAddr+6+Office_AFN0DF167_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
			i+=6+(4*(1+TMaxTariff));
			//F184 ≤‚¡øµ„4œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF168_LastDate_Source]!=0xee)
			{
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
				f|=DataComp(i+3,FnDataAddr+6+Office_AFN0DF168_LastDate_Source+3,2);// ˝æ›±»Ωœ,∑µªÿ0=œ‡µ»;µ±Byte>8 ±∑µªÿ1=≤ªœ‡µ»;µ±Byte<=8 ±1=Data1>Data2,2=Data1<Data2
				MR(i,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));
			#else
				AddMonthCongeal(YM,184,Pn,FnDataAddr+6+Office_AFN0DF168_LastDate_Source,6+(4*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
			}
		#if (((USER/100)==5)&&(Project==300))//…œ∫£◊®±‰
			//!!!‘¬∂≥Ω·◊Ó¥Û–Ë¡ø≤ªƒ‹”√√ø‘¬1»’µƒ◊Ó¥Û–Ë¡ø,“Úπ˝‘¬«Â0 (…œ∫£“™«Û∞¥¥À◊ˆ∑®)
			//F193 ’˝œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF185_LastDate_Source]!=0xee)
			{
				AddMonthCongeal(YM,193,Pn,FnDataAddr+6+Office_AFN0DF185_LastDate_Source,6+(7*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			}
			//F195 ∑¥œÚ”–π¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£©
			if(p8FnData[6+Office_AFN0DF187_LastDate_Source]!=0xee)
			{
				AddMonthCongeal(YM,195,Pn,FnDataAddr+6+Office_AFN0DF187_LastDate_Source,6+(7*(1+TMaxTariff)));//≤π‘¬∂≥Ω·µ„ ˝æ›
			}
		#endif
			
		#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			if(f!=0)
			{
				MW(ADDR_DATABUFF,Addr,2+(6*8)+((5+4+5+4+4+4+4+4)*(1+TMaxTariff)));//–¥‘≠ º ˝æ›
			}
		#endif
		}
	#endif
		return;
	}
	
	YMD=MRR(ADDR_TYMDHMS+3,3);//»°µ±«∞ ±º‰ƒÍ‘¬»’
	YM=YMD>>8;
	YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
	YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
	if((p8RMLib[4]&0x08)!=0)// ˝æ›∑÷¿‡:B5=‘¬∂≥Ω·,B4=≥≠±Ì»’∂≥Ω·,B3=»’∂≥Ω·,B2=«˙œﬂ∂≥Ω·,B1=–° ±∂≥Ω·,B0= µ ±
	{//2¿‡ ˝æ›B3=»’∂≥Ω·
		MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//ÃÓ»Î≥≠±Ì ±º‰
	#if RMM_DateCongeal_0xEE==0//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
		Fn=p8RMLib[7];//»°»’∂≥Ω·Fn
		i=GetClass2SourceDataAddr(Fn,Pn);//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
		if(i!=0)
		{
			x=MRR(i+2,3);//»°‘≠≥≠±ÌƒÍ‘¬»’
			y=MRR(ADDR_TYMDHMS+3,3);//»°µ±«∞ ±º‰ƒÍ‘¬»’
			if(x!=y)
			{//ƒÍ‘¬»’≤ªÕ¨
				p32=(u32 *)(ADDR_Terminal_ClassDataLen);
				MW(FnDataAddr+1,i,p32[0]);
			}
		#if RMM_MonthCongealUserDate1==1//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›;0=∑Ò,1= «
			if((Fn>=185)&&(Fn<=188))
			{
				Fn+=8;
				i=GetClass2SourceDataAddr(Fn,Pn);//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
				if(i!=0)
				{
					x=MRR(i+3,2);//»°‘≠≥≠±ÌƒÍ‘¬
					y=MRR(ADDR_TYMDHMS+4,2);//»°µ±«∞ ±º‰ƒÍ‘¬
					if(x!=y)
					{//ƒÍ‘¬≤ªÕ¨
						p32=(u32 *)(ADDR_Terminal_ClassDataLen);
						MW(FnDataAddr+1,i,p32[0]);
					}
				}
			}
		#endif
		}
	#else
		//RMM_DateCongeal_0xEE=1//2¿‡ ˝æ›»’∂≥Ω·√ª≥≠µΩµÁ±Ì»’∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
		Fn=p8RMLib[7];//»°»’∂≥Ω·Fn
#if (USER/100)==5//…œ∫£
	#if (RMM_CompCongealTime==1)//∂¡µÁƒ‹±Ì»’∂≥Ω· ˝æ›±»Ωœ∂≥Ω· ±º‰:0=≤ª±»Ωœ,1=“™±»Ωœ
		if((Fn==185)||(Fn==187))
		{
			if(MRR(FnDataAddr,1)==1)
			{// ˝æ›”––ß±Í÷æ
				AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π»’∂≥Ω·µ„ ˝æ›
			}
		}
		else
		{
			AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π»’∂≥Ω·µ„ ˝æ›
		}
	#else
		AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π»’∂≥Ω·µ„ ˝æ›
	#endif
#else
		AddDateCongeal(YMD,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π»’∂≥Ω·µ„ ˝æ›
#endif
	#if (((USER/100)==5)&&((Project/100)==3))//…œ∫£◊®±‰
		//!!!‘¬∂≥Ω·◊Ó¥Û–Ë¡ø≤ªƒ‹”√√ø‘¬1»’µƒ◊Ó¥Û–Ë¡ø,“Úπ˝‘¬«Â0 (…œ∫£“™«Û∞¥¥À◊ˆ∑®)
		#if RMM_MonthCongealUserDate1==1//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›;0=∑Ò,1= «
			if(MRR(ADDR_TYMDHMS+3,1)==1)//»°µ±«∞ ±º‰»’
			{
				if((Fn>=185)&&(Fn<=188))
				{//Fn=185-188»’∂≥Ω·’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø
					Fn+=8;//Fn=193-196…œ‘¬∂≥Ω·’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
				}			
			}
		#endif
	#else
		//!!!‘¬∂≥Ω·◊Ó¥Û–Ë¡ø≤ªƒ‹”√√ø‘¬1»’µƒ◊Ó¥Û–Ë¡ø,“Úπ˝‘¬«Â0
//		#if RMM_MonthCongealUserDate1==1//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›;0=∑Ò,1= «
//			if((Fn>=185)&&(Fn<=188))
//			{//Fn=185-188»’∂≥Ω·’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø
//				Fn+=8;//Fn=193-196…œ‘¬∂≥Ω·’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø
//				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
//			}
//		#endif
	#endif
	#endif
	}

#if RMDateCongealSour!=0//≥≠±Ì»’∂≥Ω·‘≠ º ˝æ›:0=µ±«∞ µ ±÷µ,1=…œΩ·À„»’∂≥Ω·÷µ
	if((p8RMLib[4]&0x10)!=0)// ˝æ›∑÷¿‡:B5=‘¬∂≥Ω·,B4=≥≠±Ì»’∂≥Ω·,B3=»’∂≥Ω·,B2=«˙œﬂ∂≥Ω·,B1=–° ±∂≥Ω·,B0= µ ±
	{//2¿‡ ˝æ›B4=≥≠±Ì»’∂≥Ω·
		Fn=p8RMLib[8];//»°≥≠±Ì»’∂≥Ω·Fn
		Addr=GetLastRMDateCongealAddr(PORTn,Fn,Pn);//∞¥∂Àø⁄∫≈µ√µΩ◊Ó∫Û¥Œ≥≠±Ì»’∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
		if(Addr!=0)
		{
			MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//ÃÓ»Î≥≠±Ì ±º‰
			DMW(FnDataAddr+1,Addr,Terminal_Class2DataLib[Fn].PnLen);
		}
	}
#endif

	if((p8RMLib[4]&0x20)!=0)// ˝æ›∑÷¿‡:B5=‘¬∂≥Ω·,B4=≥≠±Ì»’∂≥Ω·,B3=»’∂≥Ω·,B2=«˙œﬂ∂≥Ω·,B1=–° ±∂≥Ω·,B0= µ ±
	{//2¿‡ ˝æ›B5=‘¬∂≥Ω·
#if (((USER/100)==5)&&((Project/100)==2))//…œ∫£ºØ÷–∆˜
		//…œ∫£ºØ÷–∆˜“™«Û…œ∫£±Ì¥Œ‘¬1»’≤…ºØ±Ìƒ⁄…œ3‘¬’˝œÚ”–π¶µÁ¡ø◊˜Œ™‘¬∂≥Ω·’˝œÚ”–π¶µÁ¡ø
		i=Check_AFN04F10_Pn(Pn);//ºÏ≤È÷’∂ÀµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√≈‰÷√≤Œ ˝÷∏∂®µƒ≤‚¡øµ„∫≈;∑µªÿ0Œﬁ,!=0Œ™œ‡Õ¨≤‚¡øµ„∫≈µƒ≈‰÷√–Ú∫≈(1ø™ º)
		if(MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+5,1)==21)
		{//…œ∫£πÊ‘º
			Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
			i=p8RMLib[0];
			if(i==137)
			{//…œ‘¬ ˝æ›
				//…œ1‘¬
				if(MRR(FnDataAddr+7,1)!=0xee)
				{
					MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//ÃÓ»Î≥≠±Ì ±º‰
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
				}
				//…œ2‘¬
				if(MRR(FnDataAddr+7+Terminal_Class2DataLib[Fn].PnLen,1)!=0xee)
				{
					MW(ADDR_TYMDHMS+1,FnDataAddr+1+Terminal_Class2DataLib[Fn].PnLen,5);//ÃÓ»Î≥≠±Ì ±º‰
					YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
					AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1+Terminal_Class2DataLib[Fn].PnLen,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
				}
			}
		}
		else
		{
			MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//ÃÓ»Î≥≠±Ì ±º‰
		#if RMM_MonthCongealUserDate1==0//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›;0=∑Ò,1= «
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//≤π≥≠…œ2‘¬ ˝æ›,‘⁄∑«ÃÓ0xffƒ£ ΩŒﬁ…œ2‘¬
				return;
			}
			i=GetClass2SourceDataAddr(Fn,Pn);//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				x=MRR(i+3,2);//»°‘≠≥≠±ÌƒÍ‘¬
				y=MRR(ADDR_TYMDHMS+4,2);//»°µ±«∞ ±º‰ƒÍ‘¬
				if(x!=y)
				{//ƒÍ‘¬≤ªÕ¨
					p32=(u32 *)(ADDR_Terminal_ClassDataLen);
					MW(FnDataAddr+1,i,p32[0]);
				}
			}
			#else
			Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//≤π≥≠…œ2‘¬ ˝æ›
				YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
			}
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
			#endif
		#else
			//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›
			#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
			Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//≤π≥≠…œ2‘¬ ˝æ›,√ª”√1»’ ˝æ›¥˙,‘⁄∑«ÃÓ0xffƒ£ ΩŒﬁ…œ2‘¬
				return;
			}
			if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
			{//’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø√ª”√1»’ ˝æ›¥˙,µÁƒ‹±Ì≤Œ ˝–ﬁ∏ƒ¥Œ ˝º∞ ±º‰µ»√ª”√1»’ ˝æ›¥˙
				i=GetClass2SourceDataAddr(Fn,Pn);//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
				if(i!=0)
				{
					x=MRR(i+3,2);//»°‘≠≥≠±ÌƒÍ‘¬
					y=MRR(ADDR_TYMDHMS+4,2);//»°µ±«∞ ±º‰ƒÍ‘¬
					if(x!=y)
					{//ƒÍ‘¬≤ªÕ¨
						p32=(u32 *)(ADDR_Terminal_ClassDataLen);
						MW(FnDataAddr+1,i,p32[0]);
					}
				}
			}
			#else
			Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
			i=p8RMLib[0];
			if((i>=0xd1)&&(i<=0xdc))
			{//≤π≥≠…œ2‘¬ ˝æ›,√ª”√1»’ ˝æ›¥˙
				YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
				return;
			}
			if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
			{//’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø√ª”√1»’ ˝æ›¥˙,µÁƒ‹±Ì≤Œ ˝–ﬁ∏ƒ¥Œ ˝º∞ ±º‰µ»√ª”√1»’ ˝æ›¥˙
				AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
			}
			#endif
		#endif
		}
#else//#if (((USER/100)==5)&&((Project/100)==2))//…œ∫£ºØ÷–∆˜
		MW(ADDR_TYMDHMS+1,FnDataAddr+1,5);//ÃÓ»Î≥≠±Ì ±º‰
	#if RMM_MonthCongealUserDate1==0//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›;0=∑Ò,1= «
		#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
		Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//≤π≥≠…œ2‘¬ ˝æ›,‘⁄∑«ÃÓ0xffƒ£ ΩŒﬁ…œ2‘¬
			return;
		}
		i=GetClass2SourceDataAddr(Fn,Pn);//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
		if(i!=0)
		{
			x=MRR(i+3,2);//»°‘≠≥≠±ÌƒÍ‘¬
			y=MRR(ADDR_TYMDHMS+4,2);//»°µ±«∞ ±º‰ƒÍ‘¬
			if(x!=y)
			{//ƒÍ‘¬≤ªÕ¨
				p32=(u32 *)(ADDR_Terminal_ClassDataLen);
				MW(FnDataAddr+1,i,p32[0]);
			}
		}
		#else
		Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//≤π≥≠…œ2‘¬ ˝æ›
			YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
		}
		AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
		#endif
	#else
		//2¿‡ ˝æ›µ±‘¬∂≥Ω· π”√µÁ±Ì√ø‘¬1»’∂≥Ω·µƒ ˝æ›
		#if RMM_MonthCongeal_0xEE==0//2¿‡ ˝æ›‘¬∂≥Ω·√ª≥≠µΩµÁ±Ì‘¬∂≥Ω· ˝æ›«∞ÃÓ0xff;0=∑Ò(”√ µ ± ˝æ›∂≥Ω·),1= «
		Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//≤π≥≠…œ2‘¬ ˝æ›,√ª”√1»’ ˝æ›¥˙,‘⁄∑«ÃÓ0xffƒ£ ΩŒﬁ…œ2‘¬
			return;
		}
		if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
		{//’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø√ª”√1»’ ˝æ›¥˙,µÁƒ‹±Ì≤Œ ˝–ﬁ∏ƒ¥Œ ˝º∞ ±º‰µ»√ª”√1»’ ˝æ›¥˙
			i=GetClass2SourceDataAddr(Fn,Pn);//»°¿‡2‘≠ º ˝æ›¥Ê÷¸µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ,”––ß ˝æ›≥§∂»∑≈ADDR_Terminal_ClassDataLen
			if(i!=0)
			{
				x=MRR(i+3,2);//»°‘≠≥≠±ÌƒÍ‘¬
				y=MRR(ADDR_TYMDHMS+4,2);//»°µ±«∞ ±º‰ƒÍ‘¬
				if(x!=y)
				{//ƒÍ‘¬≤ªÕ¨
					p32=(u32 *)(ADDR_Terminal_ClassDataLen);
					MW(FnDataAddr+1,i,p32[0]);
				}
			}
		}
		#else
		Fn=p8RMLib[9];//»°‘¬∂≥Ω·Fn
		i=p8RMLib[0];
		if((i>=0xd1)&&(i<=0xdc))
		{//≤π≥≠…œ2‘¬ ˝æ›,√ª”√1»’ ˝æ›¥˙
			YM=YM_Sub1M(YM);//ƒÍ‘¬ºı1‘¬,∑µªÿºı1∫ÛµƒƒÍ‘¬
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
			return;
		}
	#if (((USER/100)==5)&&((Project/100)==3))//…œ∫£◊®±‰
		if((Fn>=213)&&(Fn<=216))
	#else
		if(((Fn>=193)&&(Fn<=196))||((Fn>=213)&&(Fn<=216)))
	#endif
		{//’˝∑¥”–Œﬁπ¶◊Ó¥Û–Ë¡ø√ª”√1»’ ˝æ›¥˙,µÁƒ‹±Ì≤Œ ˝–ﬁ∏ƒ¥Œ ˝º∞ ±º‰µ»√ª”√1»’ ˝æ›¥˙
			AddMonthCongeal(YM,Fn,Pn,FnDataAddr+1,Terminal_Class2DataLib[Fn].PnLen);//≤π‘¬∂≥Ω·µ„ ˝æ›
		}
		#endif
	#endif
#endif
	}
	
	if((p8RMLib[4]&0x04)!=0)// ˝æ›∑÷¿‡:B5=‘¬∂≥Ω·,B4=≥≠±Ì»’∂≥Ω·,B3=»’∂≥Ω·,B2=«˙œﬂ∂≥Ω·,B1=–° ±∂≥Ω·,B0= µ ±
	{//2¿‡ ˝æ›B2=«˙œﬂ∂≥Ω·
		if(((p8RMLib[0]>=0xe4)&&(p8RMLib[0]<=0xea))||(p8RMLib[0]==1))
		{//∂¡µÁ±Ì1-6¿‡∏∫∫…«˙œﬂ
			Addr=6+1+4;
			//∏∫∫…º«¬º∆ º¬Î£∫A0H£¨A0H(ªÚ E0H£¨E0H£∫±æ ˝æ›øÈ≤ª’˝»∑),2 ◊÷Ω⁄
			//∏∫∫…º«¬º◊÷Ω⁄ ˝£∫1 ◊÷Ω⁄( Æ¡˘Ω¯÷∆)
			//∏∫∫…º«¬º¥Ê¥¢ ±º‰£∫ƒÍ°¢‘¬°¢»’°¢ ±°¢∑÷,  5 ◊÷Ω⁄
			if(p8FnData[6]>=(Addr-7+8))
			{
				if((p8FnData[Addr]!=0xa0)||(p8FnData[Addr+1]!=0xa0))
				{
					return;
				}
				YMDHM=MRR(FnDataAddr+Addr+3,5);//∏∫∫…º«¬º¥Ê¥¢ ±º‰
				Addr+=8;
				//µÁ—π°¢µÁ¡˜°¢∆µ¬ £∫17 ◊÷Ω⁄£ª 
				//øÈ∑÷∏Ù¬Î£∫AAH£¨1 ◊÷Ω⁄£ª
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+17))
					{
						return;
					}
					else
					{
						//A°¢B°¢C œ‡µÁ—π£®√øœ‡ 2 ◊÷Ω⁄£¨π≤ 6 ◊÷Ω⁄£¨µ•Œª£∫0.1V£©
						AddCurveCongeal(YMDHM,89,Pn,FnDataAddr+Addr,2);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,90,Pn,FnDataAddr+Addr+2,2);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,91,Pn,FnDataAddr+Addr+4,2);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						//A°¢B°¢C œ‡µÁ¡˜£®√øœ‡ 3 ◊÷Ω⁄£¨π≤ 9 ◊÷Ω⁄£¨µ•Œª£∫0.001A£©
						AddCurveCongeal(YMDHM,92,Pn,FnDataAddr+Addr+6,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,93,Pn,FnDataAddr+Addr+9,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,94,Pn,FnDataAddr+Addr+12,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						Addr+=17+1;
					}
				}
				//”–°¢Œﬁπ¶π¶¬ £∫24 ◊÷Ω⁄
				//øÈ∑÷∏Ù¬Î£∫AAH£¨1 ◊÷Ω⁄
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+24))
					{
						return;
					}
					else
					{
						//◊‹º∞ A°¢B°¢C œ‡”–π¶π¶¬ £®√ø∏ˆ 3 ◊÷Ω⁄£¨π≤ 12 ◊÷Ω⁄£¨µ•Œª£∫0.0001kW£©
						AddCurveCongeal(YMDHM,81,Pn,FnDataAddr+Addr,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,82,Pn,FnDataAddr+Addr+3,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,83,Pn,FnDataAddr+Addr+6,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,84,Pn,FnDataAddr+Addr+9,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						//◊‹º∞ A°¢B°¢C œ‡Œﬁπ¶π¶¬ £®√ø∏ˆ 3 ◊÷Ω⁄£¨π≤ 12 ◊÷Ω⁄£¨µ•Œª£∫0.0001kvar£©
						AddCurveCongeal(YMDHM,85,Pn,FnDataAddr+Addr+12,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,86,Pn,FnDataAddr+Addr+15,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,87,Pn,FnDataAddr+Addr+18,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,88,Pn,FnDataAddr+Addr+21,3);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						Addr+=24+1;
					}
				}
				//π¶¬ “Ú ˝£∫8 ◊÷Ω⁄
				//øÈ∑÷∏Ù¬Î£∫AAH£¨1 ◊÷Ω⁄
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+8))
					{
						return;
					}
					else
					{
						//◊‹º∞ A°¢B°¢C œ‡π¶¬ “Ú ˝£®√ø∏ˆ 2 ◊÷Ω⁄£¨π≤ 8 ◊÷Ω⁄£¨µ•Œª£∫0.001£©
						AddCurveCongeal(YMDHM,105,Pn,FnDataAddr+Addr,2);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,106,Pn,FnDataAddr+Addr+2,2);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,107,Pn,FnDataAddr+Addr+4,2);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,108,Pn,FnDataAddr+Addr+6,2);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						Addr+=8+1;
					}
				}
				//”–°¢Œﬁπ¶◊‹µÁƒ‹£∫16 ◊÷Ω⁄
				//øÈ∑÷∏Ù¬Î£∫AAH£¨1 ◊÷Ω⁄
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+16))
					{
						return;
					}
					else
					{
						//’˝œÚ”–π¶◊‹µÁƒ‹,∑¥œÚ”–π¶◊‹µÁƒ‹,◊È∫œŒﬁπ¶ 1 ◊‹µÁƒ‹,◊È∫œŒﬁπ¶ 2 ◊‹µÁƒ‹
					#if ((USER/100)==5)&&(Project==200)//…œ∫£ºØ÷–∆˜¥Û¿‡∫≈=5 ±;’˝œÚ”–π¶=’˝œÚ”–π¶+∑¥œÚ”–π¶
						i=Check_AFN04F10_Pn(Pn);//ºÏ≤È÷’∂ÀµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√≈‰÷√≤Œ ˝÷∏∂®µƒ≤‚¡øµ„∫≈;∑µªÿ0Œﬁ,!=0Œ™œ‡Õ¨≤‚¡øµ„∫≈µƒ≈‰÷√–Ú∫≈(1ø™ º)
						if(i!=0)
						{
							i--;
							i=MRR(ADDR_AFN04F10+(LEN_AFN04F10_Pn*i)+26,1);//»°¥Û–°¿‡∫≈
							if((i!=0x02)&&(i!=0x62)&&(i!=0x71)&&(i!=0x72))
							{//¥Û¿‡∫≈=5;’˝œÚ”–π¶=’˝œÚ”–π¶+∑¥œÚ”–π¶
								x=MRR(FnDataAddr+Addr,4);
								y=MRR(FnDataAddr+Addr+4,4);
								if(((x&0xff)<=0x99)&&((y&0xff)<=0x99))
								{//’˝∑¥”–π¶∂º≥≠µΩ
									x=bcd_hex(x);
									y=bcd_hex(y);
									x+=y;
									x=hex_bcd(x);
									MWR(x,FnDataAddr+Addr,4);
								}
								else
								{
									MC(0xee,FnDataAddr+Addr,4);
								}
							}
						}
					#endif
						AddCurveCongeal(YMDHM,101,Pn,FnDataAddr+Addr,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,103,Pn,FnDataAddr+Addr+4,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,102,Pn,FnDataAddr+Addr+8,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,104,Pn,FnDataAddr+Addr+12,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						Addr+=16+1;
					}
				}
				//ÀƒœÛœﬁŒﬁπ¶◊‹µÁƒ‹£∫16 ◊÷Ω⁄
				//øÈ∑÷∏Ù¬Î£∫AAH£¨1 ◊÷Ω⁄
				if(p8FnData[Addr]==0xaa)
				{
					Addr++;
				}
				else
				{
					if(p8FnData[6]<(Addr-7+16))
					{
						return;
					}
					else
					{
						//µ⁄“ª,µ⁄∂˛,µ⁄»˝,µ⁄ÀƒœÛœﬁŒﬁπ¶◊‹µÁƒ‹£®4 ◊÷Ω⁄£¨µ•Œª£∫0.01kvarh£©
						AddCurveCongeal(YMDHM,145,Pn,FnDataAddr+Addr,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,147,Pn,FnDataAddr+Addr+4,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,148,Pn,FnDataAddr+Addr+8,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						AddCurveCongeal(YMDHM,146,Pn,FnDataAddr+Addr+12,4);//≤π«˙œﬂ∂≥Ω·µ„ ˝æ›
						Addr+=16+1;
					}
				}
				//µ±«∞–Ë¡ø£∫6 ◊÷Ω⁄
				//øÈ∑÷∏Ù¬Î£∫AAH£¨1 ◊÷Ω⁄
			}
			return;
		}
	}
}


u32 CreateCurveCongealNo(u64 YMDHM)//¥¥Ω®«˙œﬂ∂≥Ω·∫≈;∑µªÿ:‘≠“—¥Ê‘⁄ªÚ–¬¥¥Ω®µƒ∂≥Ω·∫≈
{
	u32 i;
	u32 *p32;

	i=GetCurveCongealNo(YMDHM);//µ√µΩ«˙œﬂ∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(i==0xffffffff)
	{//‘≠∂≥Ω·√ªœ‡Õ¨ ±±Í
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_CURVECONGEAL);
		KeyUserPnList();//÷ÿµ„ªß«˙œﬂ∂≥Ω·Pn¡–±Ì
		MR(ADDR_AFN0D_Congeal_Buff_Start+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
		i=MRR(ADDR_TCongealCount+7,2);
		if((i&0x7fff)>=MaxCurveCongeal)
		{
			i=0x8000;
			MWR(i,ADDR_TCongealCount+7,2);
		}
		i&=0x7fff;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//¿‡2 ˝æ›∂≥Ω·◊‘∂Ø–¥EFLASHøÿ÷∆µÿ÷∑(◊÷∂‘∆Î)
		p32[0]=2;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i);
		p32[3]=LEN_CURVECONGEAL;
		p32[4]=p32[2];//∂≥Ω·ø™ ºµÿ÷∑( ±±Íµÿ÷∑),Õ¨≥ı º÷µR1
		MWR(YMDHM,ADDR_Class2Congeal_AutoRW+20,5);//∂≥Ω·”––ß(∂≥Ω· ±±Í)
	}
	return i;
}

u32 CreateDateCongealNo(u32 YMD)//¥¥Ω®»’∂≥Ω·∫≈;∑µªÿ:‘≠“—¥Ê‘⁄ªÚ–¬¥¥Ω®µƒ∂≥Ω·∫≈
{
	u32 i;
	u32 *p32;
	
	i=GetDateCongealNo(YMD);//µ√µΩ»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(i==0xffffffff)
	{//‘≠∂≥Ω·√ªœ‡Õ¨ ±±Í
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//«Â∂≥Ω·ª∫≥Â
		i=MRR(ADDR_TCongealCount+14,1);
		if((i&0x7F)>=MaxDateCongeal)
		{
			i=0x80;
			MWR(i,ADDR_TCongealCount+14,1);
		}
		i&=0x7f;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//¿‡2 ˝æ›∂≥Ω·◊‘∂Ø–¥EFLASHøÿ÷∆µÿ÷∑(◊÷∂‘∆Î)
		p32[0]=0x10002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
		p32[3]=LEN_DATECONGEAL;
		p32[4]=p32[2];//∂≥Ω·ø™ ºµÿ÷∑( ±±Íµÿ÷∑),Õ¨≥ı º÷µR1
		MWR(YMD,ADDR_Class2Congeal_AutoRW+20,3);//∂≥Ω·”––ß(∂≥Ω· ±±Í)
	}
	return i;
}

u32 CreateMonthCongealNo(u32 YM)//¥¥Ω®‘¬∂≥Ω·∫≈;∑µªÿ:‘≠“—¥Ê‘⁄ªÚ–¬¥¥Ω®µƒ∂≥Ω·∫≈
{
	u32 i;
	u32 *p32;
	
	i=GetMonthCongealNo(YM);//µ√µΩ‘¬∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(i==0xffffffff)
	{//‘≠∂≥Ω·√ªœ‡Õ¨ ±±Í
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_MONTHCONGEAL);//«Â∂≥Ω·ª∫≥Â
		i=MRR(ADDR_TCongealCount+28,1);
		if((i&0x7f)>=MaxMonthCongeal)
		{
			i=0x80;
			MWR(i,ADDR_TCongealCount+28,1);
		}
		i&=0x7f;
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//¿‡2 ˝æ›∂≥Ω·◊‘∂Ø–¥EFLASHøÿ÷∆µÿ÷∑(◊÷∂‘∆Î)
		p32[0]=0x30002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i);
		p32[3]=LEN_MONTHCONGEAL;
		p32[4]=p32[2];//∂≥Ω·ø™ ºµÿ÷∑( ±±Íµÿ÷∑),Õ¨≥ı º÷µR1
		MWR(YM,ADDR_Class2Congeal_AutoRW+20,2);//∂≥Ω·”––ß(∂≥Ω· ±±Í)
	}
	return i;
}


#if RMDateCongealSour==1//≥≠±Ì»’∂≥Ω·‘≠ º ˝æ›:0=µ±«∞ µ ±÷µ,1=…œΩ·À„»’∂≥Ω·÷µ
u32 LastRMDateCongealYMD(u32 PORTn)//∞¥∂Àø⁄∫≈º∆À„◊Ó∫Û¥Œ≥≠±Ì»’∂≥Ω·µƒ ±±ÍƒÍ‘¬»’;»Îø⁄:Õ®–≈ø⁄∫≈;∑µªÿ:0±Ì æ√ª
{
	u32 i;
	u32 x;
	u32 YMD;
	u32 HM;

	switch(PORTn)
	{
		case ACSAMPLEPORT:
			PORTn=1-1;
			break;
		case RS485_1PORT://RS485_1
			PORTn=2-1;
			break;
		case RS485_2PORT://RS485_2
			PORTn=3-1;
			break;
		case RS485_4PORT://RS485_4(‘ÿ≤®)
			PORTn=31-1;
			break;
	}
	HM=MRR(ADDR_AFN04F33+7+((14+(4*24))*PORTn),2);
	PORTn=MRR(ADDR_AFN04F33+3+((14+(4*24))*PORTn),4);
	PORTn<<=1;
	YMD=MRR(ADDR_TYMDHMS+3,3);
	i=MRR(ADDR_TYMDHMS+1,2);
	x=YMD&0xff;
	x=bcd_hex(x);
	x=1<<x;
	if((PORTn&x)&&(i>=HM))
	{
		return YMD;
	}
	else
	{
		for(i=0;i<30;i++)
		{
			YMD=YMD_Sub1D(YMD);//ƒÍ‘¬»’ºı1»’,∑µªÿºı1∫ÛµƒƒÍ‘¬»’
			x=YMD&0xff;
			x=bcd_hex(x);
			x=1<<x;
			if(PORTn&x)
			{
				return YMD;
			}
		}
	}
	return 0;
}
u32 GetLastRMDateCongealAddr(u32 PORTn,u32 Fn,u32 Pn)//∞¥∂Àø⁄∫≈µ√µΩ◊Ó∫Û¥Œ≥≠±Ì»’∂≥Ω·¥Ê¥¢µÿ÷∑,∑µªÿ=0±Ì æ√ª’“µΩ
{
	u32 i;
	u32 x;
	u32 YMD;
	u32 *p32;
	
	YMD=LastRMDateCongealYMD(PORTn);//∞¥∂Àø⁄∫≈º∆À„◊Ó∫Û¥Œ≥≠±Ì»’∂≥Ω·µƒ ±±ÍƒÍ‘¬»’;»Îø⁄:Õ®–≈ø⁄∫≈;∑µªÿ:0±Ì æ√ª
	if(YMD==0)
	{
		return 0;
	}
	i=GetRMDateCongealNo(YMD);//µ√µΩ≥≠±Ì»’∂≥Ω·œ‡Õ¨ ±±Íµƒ∂≥Ω·∫≈,∑µªÿ=0xffffffff±Ì æ√ª’“µΩ
	if(i==0xffffffff)
	{
		//¥¥Ω®∂≥Ω·∫≈
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//¿‡2 ˝æ›∂≥Ω·◊‘∂Ø–¥EFLASHøÿ÷∆µÿ÷∑(◊÷∂‘∆Î)
		if(p32[3]!=0x0)
		{//‘≠¿‡2 ˝æ›∂≥Ω·AUTOIOW√ªÕÍ≥…
			return 0;
		}
		i=MRR(ADDR_TCongealCount+21,1);
		if((i&0x7F)>=MaxRMDateCongeal)
		{
			i=0x80;
		}
		x=i;
		i&=0x7f;
		MC(0xff,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),LEN_RMDATECONGEAL);
		DMWR(YMD,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),3);//∂≥Ω·”––ß(–¥∂≥Ω· ±±Í)
		//∂≥Ω·∫≈º∆ ˝+1
		YMD=x&0x80;
		x&=0x7f;
		x+=1;
		if(x>=MaxRMDateCongeal)
		{
			x=0x80;
		}
		x|=YMD;
		MWR(x,ADDR_TCongealCount+21,1);
	}
	x=GetClass2DataOfficeAddr(Fn,Pn);//»°¿‡2 ˝æ›¥Ê÷¸∆´“∆µÿ÷∑,∑µªÿ0±Ì æ√ª’“µΩ
	if(x==0)
	{
		return 0;
	}
	return (ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i)+x);
}
#endif




