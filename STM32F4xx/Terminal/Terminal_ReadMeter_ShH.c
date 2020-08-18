
//÷’∂À≥≠±ÌRS485ø⁄_…œ∫£πÊ‘º
#include "Project.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0E_Event.h"

#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "Terminal_ReadMeter_SaveFnData.h"
#include "Terminal_ReadMeter_ShH_FnData.h"
#include "Terminal_ReadMeter_DL645_2007_FnData.h"

#include "Terminal_ReadMeter_CountAndFlag.h"
#include "KeyUser.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_Fn.h"

#include "Terminal_ReadMeter_3762.h"
#include "Terminal_ReadMeter_Note.h"


#if (USER/100)!=5//≤ª «…œ∫£
void Terminal_ReadMeter_ShH(u32 PORTn)//÷’∂À≥≠±ÌRS485ø⁄_…œ∫£πÊ‘º
{
}
void Terminal_ReadMeter_ShH_Fn(u32 PORTn,u8 *p8RMLib)//Ωˆ≥≠1∏ˆFn ˝æ›ÃÓ–¥µΩFnDataBuff;Task=0=ø’œ–(±æFn≥≠∂¡ÕÍ≥…)
{
}
#else
extern const u32 ShH_RMLib[];
u32 Get_RM_ShH_MaxFn(void);
extern const KeyUserReadMeterLib_TypeDef  KeyUserReadMeterLib[];


u32 ShH_Rx(u32 PORTn)//…œ∫£πÊ‘º÷°Ω” ’;∑µªÿ0√ªÕÍ≥…,1ÕÍ≥…,2≥¨ ±
{
/*
	…œ∫£πÊ‘º÷°∏Ò Ω£∫
	«∞µº◊÷∑˚	FE  //0
	÷°∆ º∑˚	68H //1+0
	µÿ÷∑”Ú	    A0  //1+1
				A1  //1+2
				A2  //1+3
				A3  //1+4	   ÷∆‘Ï≥ß¥˙¬Î£¨1◊÷Ω⁄ASCII¬Î£¨»ÁP£¨‘Ú∏√≥≠±Ì÷°¿ÔŒ™50H£¨÷˜’æœ¬∑¢∏¯÷’∂ÀŒ™80H(BCD¬Î)
				A4  //1+5
				A5  //1+6
	÷°∆ º∑˚	68H //1+7
	√¸¡Ó¬Î		C   //1+8
	 ˝æ›”Ú≥§∂»	L   //1+9	    Æ¡˘Ω¯÷∆
	 ˝æ›”Ú		DATA//1+10	    ˝æ›”Ú+33H¥¶¿Ì
	◊›œÚ–£—È¬Î	CS	//1+10+L
	÷°Ω· ¯∑˚	16H	//1+11+L
*/
	u32 i;
	u32 Pn;
	u32 p0;
	u32 p1;
	u32 RxByte;
	
	u8 * p8fifo;
	u16 * p16fifo;
	u8 * p8rxbuff;
	u16 * p16timer;
  UARTCtrl_TypeDef * UARTCtrl;
	
	u32 LEN_UARTnIntRx=Get_LEN_UARTnIntRx(PORTn);	
	u32 LEN_UARTnRx=Get_LEN_UARTnRx(PORTn);
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8fifo=(u8 *)(Get_ADDR_UARTn(PORTn)+6);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	
	
	p0=p16fifo[0];//÷–∂œΩ” ’÷∏’Î
	p1=p16fifo[1];//µ±«∞“—∂¡÷∏’Î
	if((p0>=(LEN_UARTnIntRx-6))||(p1>=(LEN_UARTnIntRx-6)))
	{
		p16fifo[0]=0;//÷–∂œΩ” ’÷∏’Î
		p16fifo[1]=0;//µ±«∞“—∂¡÷∏’Î
		p16fifo[2]=0;//πÊ‘ººÏ≤È÷∏’Î
		return 0;
	}
	RxByte=UARTCtrl->RxByte;
	if(p0!=p1)
	{//”– ˝æ›Ω” ’÷ÿ÷√≥¨ ±∂® ±
		p16timer[0]=500/10;//◊÷Ω⁄º‰≥¨ ±∂® ±
	}
	while(p0!=p1)
	{
		if(RxByte>=LEN_UARTnRx)
		{
			RxByte=0;
		}
		i=p8fifo[p1];
		p1++;
		if(p1>=(LEN_UARTnIntRx-6))
		{
			p1=0;
		}
		p8rxbuff[RxByte]=i;
		RxByte++;
		if(RxByte==1)
		{
			if(i!=0x68)
			{
				RxByte=0;
				continue;
			}
		}
		else
		{
			if(RxByte==8)
			{
				if(i!=0x68)
				{
					RxByte=0;
					continue;
				}
			}
			else
			{
				if(RxByte>=10)
				{
					if(RxByte>=(p8rxbuff[9]+12))
					{
						if(i==0x16)
						{//Ω” ’ÕÍ≥…
							//∫Õ–£—Èº∆À„
							Pn=0;
							for(i=0;i<(p8rxbuff[9]+10);i++)
							{
								Pn+=p8rxbuff[i];
							}
							Pn&=0xff;
							if(Pn==p8rxbuff[i])
							{//∫Õ–£—È’˝»∑
								p16fifo[1]=p1;//µ±«∞“—∂¡÷∏’Î
								p16fifo[2]=p1;//ºÏ≤È÷∏’Î==∂¡÷∏’Î,√‚ºÏ ° ±
								UARTCtrl->RxByte=RxByte;
								return 1;
							}
							else
							{//∫Õ–£—È¥ÌŒÛ
								RxByte=0;
								continue;
							}
						}
						else
						{
							RxByte=0;
							continue;
						}
					}
				}
			}
		}
	}
	p16fifo[1]=p1;
	UARTCtrl->RxByte=RxByte;
	if(p16timer[0]==0x0)
	{//≥¨ ±
		return 2;//2=Ω” ’≥¨ ±
	}
	return 0;
}
u32 ShH_RxData(u32 PORTn)//…œ∫£πÊ‘º÷°Ω” ’ ˝æ›,±»Ωœ∑¢ ’µÿ÷∑°¢√¸¡Ó¬Î°¢ºı0x33;∑µªÿ0’˝»∑,1µÿ÷∑¥Ì,2√¸¡Ó¬Î¥Ì
{
	u32 i;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	//Õ®–≈µÿ÷∑±»Ωœ
	//A5≤ª”√±»Ωœ
	for(i=0;i<5;i++)
	{
		if(p8rxbuff[1+i]!=p8txbuff[1+1+i])
		{
			break;
		}
	}
	if(i!=5)
	{
		return 1;//∑µªÿ0’˝»∑,1µÿ÷∑¥Ì,2√¸¡Ó¬Î¥Ì
	}
	//√¸¡Ó¬Î±»Ωœ
	i=(p8rxbuff[8]|0x80);
	if(i!=p8txbuff[1+8])
	{
		return 2;//∑µªÿ0’˝»∑,1µÿ÷∑¥Ì,2√¸¡Ó¬Î¥Ì
	}
	//ºı0x33
	for(i=0;i<p8rxbuff[9];i++)
	{
		p8rxbuff[10+i]-=0x33;
	}
	return 0;//∑µªÿ0’˝»∑,1µÿ÷∑¥Ì,2√¸¡Ó¬Î¥Ì
}

void Terminal_ReadMeter_ShH(u32 PORTn)//÷’∂À≥≠±ÌRS485ø⁄_…œ∫£πÊ‘º
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
		return;//0x55=Õ®–≈π¶ƒ‹’˝‘⁄±ªµ˜”√Ω˚÷π∆‰À˚≥Ã–Ú‘Ÿ¥Œµ˜”√
	}
	switch(UARTCtrl->Task)//0=ø’œ–,1=ÕÍ≥…1÷°∑¢ ’,2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ),3=∆Ù∂Ø∑¢ÀÕ,4=’˝‘⁄∑¢ÀÕ,5=’˝‘⁄Ω” ’
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=ø’œ–
			if(UARTCtrl->BatchTask>=1)//◊™∑¢≈˙¡ø»ŒŒÒ:0=ø’,1=µ»¥˝ø⁄ø’œ–∆Ù∂Ø,2...π˝≥Ã
			{
				return;
			}
/*
#if RMM_RS485_PassageZero==1//RS485Õ®µ¿≥≠±Ìπ˝0µ„;0=≤ª‘›Õ£,1=‘›Õ£
			i=MRR(ADDR_TYMDHMS+1,2);
			if((i>=0x2355)||(i<RMM_StartHM))
			{//RS485ø⁄‘⁄√ø»’0µ„∏ΩΩ¸‘›Õ£≥≠±Ì
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
				CopyString((u8 *)"485 Õ£≥≠µ»¥˝ ±÷”π˝0",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
				MWR(PORT485,ADDR_DATABUFF+3,1);
				WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//∏ÊæØ∑¢…˙,»Îø⁄∏ÊæØ¥˙¬Î∫Õ∏ÊæØ◊÷∑˚¥Æ
				return;
			}
#endif
*/
			if(p16timer[0]==0x0)
			{//œ¬÷°∆Ù∂Ø∑¢ÀÕ—” ±
				UARTCtrl->Task=3;//∆Ù∂Ø∑¢ÀÕ
			}
			break;
		case 1://1=ÕÍ≥…1÷°∑¢ ’
			UARTCtrl->Task=0;//ø’œ–1¥Œ
			break;
		case 2://2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ)
			UARTCtrl->Task=0;//ø’œ–1¥Œ
			if(UARTCtrl->ReRMCount<2)
			{
				UARTCtrl->ReRMCount+=1;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//≥≠±Ì√¸¡Óº∆ ˝+1
				UARTCtrl->ReRMCount=0;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝
				Fn=MRR(ShH_RMLib[UARTCtrl->FnCount],1);
				if(Fn!=0xe1)
				{
					UARTCtrl->RMCountErr++;//Fnƒ⁄”–≥≠∂¡ ß∞‹ªÚ≤ª≥≠µƒ ˝æ›±Í ∂º∆ ˝
				}
				switch(Fn)
				{
					default:
						UARTCtrl->RMError|=1;//±æµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√–Ú∫≈Õ®–≈ ß∞‹±Í÷æ,B0= ß∞‹,B1=F129 ß∞‹
						break;
					case 129://F129 µ±«∞’˝œÚ”–π¶µÁƒ‹ æ÷µ
						UARTCtrl->RMError|=2;//±æµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√–Ú∫≈Õ®–≈ ß∞‹±Í÷æ,B0= ß∞‹,B1=F129 ß∞‹
						break;
//					case 0xE1://¿©’π1 ERC8µÁƒ‹±Ì≤Œ ˝±‰∏¸ ¬º˛”√ ˝æ›
//						i=UARTCtrl->RMCount;
//						if(i<14)//≥≠±Ì√¸¡Óº∆ ˝
//						{//µ⁄1»’ ±∂Œ±Ì1-14∂Œ
//							UARTCtrl->RMCount=14;
//						}
//						break;
				}
			}
			break;
		case 3://3=∆Ù∂Ø∑¢ÀÕ
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if((Pn==0x0)||(Pn>MaxRS485AddCarrierPn))
			{//±æ–Ú∫≈µƒµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√…æ≥˝
				goto END_Fn;//≤ª≥≠
			}
			if(AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]>=2)//≥≠±Ì»ŒŒÒÕÍ≥…±Í÷æ:0=√ªÕÍ≥…,1=ÕÍ≥…µ´÷¡…Ÿ”–1∏ˆ ˝æ›±Í ∂≥≠∂¡ ß∞‹,2=≥…π¶ÕÍ≥…,3=≤ª–Ë≥≠
			{
				goto END_Fn;//“—≥…π¶≥≠±Ì
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
#if (((Project/100)==2)||((Project/100)==5))//∑Ω∞∏0=÷«ƒ‹±Ì,100=Õ¯¬Á±Ì,200=ºØ÷–∆˜,300=◊®±‰÷’∂À,400=π´±‰÷’∂À,500=≤…ºØ∆˜,600=Õ®–≈ƒ£øÈ,700=
			i=MRR(ADDR_AFN04F33+((14+(4*24))*PORT485)+1,1);//÷’∂À≥≠±Ì‘À––≤Œ ˝…Ë÷√,∂Àø⁄31
			if((i&0x02)!=0x0)
			{//D1÷√"1"“™«Û÷’∂À÷ª≥≠÷ÿµ„±Ì£¨÷√"0"“™«Û÷’∂À≥≠À˘”–±Ì
				y=MRR(ADDR_AFN04F35,1);//Ã®«¯ºØ÷–≥≠±Ì÷ÿµ„ªß…Ë÷√
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
				{//≤ª «÷ÿµ„ªß
					goto END_Fn;//≤ª≥≠
				}
			}
			i=MRR(ADDR_AFN04F30+(Pn-1),1);//Ã®«¯ºØ÷–≥≠±ÌÕ£≥≠/Õ∂≥≠…Ë÷√(≤‚¡øµ„∫≈)
			if(i!=0)
			{//Õ£≥≠
				goto END_Fn;//≤ª≥≠
			}
#endif
			if(UARTCtrl->FnCount>=Get_RM_ShH_MaxFn())
			{//Fnº∆ ˝>=◊Ó¥ÛFn∏ˆ ˝,±æ±Ì≥≠±ÌÕÍ≥…
				if(UARTCtrl->RMError==0)
				{//≥≠±Ì≥…π¶
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=2;//ºƒ¥Ê≥≠±Ì≥…π¶ ß∞‹±Í÷æ:0=ø’,1=÷¡…Ÿ”–1∏ˆ ˝æ›±Í ∂≥≠∂¡ ß∞‹,2=≥…π¶
					ReadMeterNote_OK(PORTn,Pn,0);//ºØ÷–≥≠±ÌµÁ±Ì≥≠∂¡–≈œ¢.≥…π¶
					DateRMFlags(Pn,0);//»’≥≠±Ì±Í÷æ;»Îø⁄:f=0≥…π¶,=1 ß∞‹
				}
				else
				{//≥≠±Ì ß∞‹
					AllRMList->Pn[Pn].RMCompleteS[UARTCtrl->RMTaskC]=1;//ºƒ¥Ê≥≠±Ì≥…π¶ ß∞‹±Í÷æ:0=ø’,1=÷¡…Ÿ”–1∏ˆ ˝æ›±Í ∂≥≠∂¡ ß∞‹,2=≥…π¶
					ReadMeterNote_Fail(PORTn,Pn);//ºØ÷–≥≠±ÌµÁ±Ì≥≠∂¡–≈œ¢. ß∞‹
					DateRMFlags(Pn,1);//»’≥≠±Ì±Í÷æ;»Îø⁄:f=0≥…π¶,=1 ß∞‹
				}
END_Fn:
				UARTCtrl->Task=0;//0=ø’œ–
				UARTCtrl->FnEnd=0;//485≥≠±ÌFnº∆ ˝Ω· ¯±Í÷æ,0=Ω· ¯,1=√ªΩ· ¯
				UARTCtrl->FnCount=0;//Fnº∆ ˝=0
				UARTCtrl->RMCount=0;//≥≠±Ì√¸¡Óº∆ ˝=0
				UARTCtrl->ReRMCount=0;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝
				UARTCtrl->RMCountErr=0;//Fnƒ⁄”–≥≠∂¡ ß∞‹ªÚ≤ª≥≠µƒ ˝æ›±Í ∂º∆ ˝
				UARTCtrl->RMError=0;//±æµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√–Ú∫≈Õ®–≈ ß∞‹±Í÷æ,B0= ß∞‹,B1=F129 ß∞‹
				break;
			}
			if(ShH_RMLib[UARTCtrl->FnCount]==0x0)
			{//±æFnŒﬁ≥≠±Ì
NEXT_Fn:
				UARTCtrl->Task=1;//ÕÍ≥…1÷°∑¢ ’
				UARTCtrl->FnCount+=1;//Fnº∆ ˝+1
				UARTCtrl->RMCount=0;//≥≠±Ì√¸¡Óº∆ ˝=0
				UARTCtrl->ReRMCount=0;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝
				UARTCtrl->RMCountErr=0;//Fnƒ⁄”–≥≠∂¡ ß∞‹ªÚ≤ª≥≠µƒ ˝æ›±Í ∂º∆ ˝
				UARTCtrl->RMError&=0xfe;//±æµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√–Ú∫≈Õ®–≈ ß∞‹±Í÷æ,B0= ß∞‹,B1=F129 ß∞‹
				break;	
			}
			p8RMLib=(u8 *)ShH_RMLib[UARTCtrl->FnCount];
			if((p8RMLib[3]&0x2)==0x0)
			{//±æFn≤ª «RS485≥≠±Ì
				goto NEXT_Fn;
			}
			Fn=p8RMLib[0];
			
			ClassNo=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+26,1);//»°¥Û–°¿‡∫≈			
			y=ReadMeter_Fn_YesNo(PORTn,ClassNo,Pn,p8RMLib,UARTCtrl->RMCount,1);//∞¥¥Û–°¿‡∫≈≥≠±ÌµƒFn «∑Ò“™≥≠;∑µªÿ0=≤ª≥≠,1=“™≥≠;»Îø⁄:ClassNoµƒD7-D4¥Û¿‡∫≈D3-D0–°¿‡∫≈,Pn,p8RMLib≥≠±Ìø‚÷∏’Î,RMCount≥≠±Ìº∆ ˝
			if(y==0x0)
			{//±æ√¸¡Ó≤ª≥≠
				if(p8RMLib[1]==0x0)
				{//±æFn≥≠±Ì√¸¡Ó ˝=0
					goto NEXT_Fn;
				}
				if(UARTCtrl->RMCount==0)//≥≠±Ì ˝æ›±Í ∂º∆ ˝
				{//≥≠±Ì ˝æ›±Í ∂º∆ ˝=0,«ÂFn ˝æ›ª∫≥Â
					UARTCtrl->RMCountErr=0;//Fnƒ⁄”–≥≠∂¡ ß∞‹ªÚ≤ª≥≠µƒ ˝æ›±Í ∂º∆ ˝
					MC(0xee,(u32)p8FnData,Get_LEN_UARTnFnDataBuff(PORTn));
				}
				UARTCtrl->RMCountErr++;//Fnƒ⁄”–≥≠∂¡ ß∞‹ªÚ≤ª≥≠µƒ ˝æ›±Í ∂º∆ ˝,◊˜Œ™ ß∞‹√‚»´√ª≥≠ ±ø’ ˝æ›ÃÓ»Î
				UARTCtrl->RMCount++;//≥≠±Ì ˝æ›±Í ∂º∆ ˝
				UARTCtrl->ReRMCount=0;//÷ÿ∑¢º∆ ˝
				if(UARTCtrl->RMCount>=p8RMLib[1])
				{//≥≠±Ì√¸¡Óº∆ ˝>=Fn–Ë∑¢√¸¡Ó ˝;±æFn≥≠±ÌÕÍ≥…
				#if(RMM_RS485_FailData_0xEE==0)//RS485≥≠±Ì ß∞‹ ˝æ›ÃÓ0xee;0=∑Ò,1= «
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//≤ª «»´≤ø±Í ∂≤ª≥≠
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
			{//±æFn≥≠±Ì√¸¡Ó ˝=0
			#if AFN0CF33MaxPn==0
				goto NEXT_Fn;
			#else
				if((p8RMLib[4]&0x01)!=0)// ˝æ›∑÷¿‡:B5=‘¬∂≥Ω·,B4=≥≠±Ì»’∂≥Ω·,B3=»’∂≥Ω·,B2=«˙œﬂ∂≥Ω·,B1=–° ±∂≥Ω·,B0= µ ±
				{//1¿‡ ˝æ›, µ ±
					switch(Fn)
					{
					//F33 µ±«∞’˝œÚ”–/Œﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
					//F34 µ±«∞∑¥œÚ”–/Œﬁπ¶£®◊È∫œŒﬁπ¶2£©µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
					//F35 µ±‘¬’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
					//F36 µ±‘¬∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
						case 33:
						case 34:
						case 35:
						case 36:
					//F37 …œ‘¬£®…œ“ªΩ·À„»’£©’˝œÚ”–/Œﬁπ¶£®◊È∫œŒﬁπ¶1£©µÁƒ‹ æ÷µ°¢“ª/ÀƒœÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
					//F38 …œ‘¬£®…œ“ªΩ·À„»’£©∑¥œÚ”–/Œﬁπ¶£®◊È∫œŒﬁπ¶2£©µÁƒ‹ æ÷µ°¢∂˛/»˝œÛœﬁŒﬁπ¶µÁƒ‹ æ÷µ£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
					//F39 …œ‘¬£®…œ“ªΩ·À„»’£©’˝œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M°‹12£©
					//F40 …œ‘¬£®…œ“ªΩ·À„»’£©∑¥œÚ”–/Œﬁπ¶◊Ó¥Û–Ë¡øº∞∑¢…˙ ±º‰£®◊‹°¢∑—¬ 1°´M£¨1°‹M‹12©
						case 37:
						case 38:
						case 39:
						case 40:
							break;
						default:
							goto NEXT_Fn;
					}
					ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›
					goto NEXT_Fn;
				}
			#endif
			}

			if(UARTCtrl->RMCount==0x0)
			{//≥≠±Ì√¸¡Óº∆ ˝=0,«ÂFn ˝æ›ª∫≥Â
				UARTCtrl->RMCountErr=0;//Fnƒ⁄”–≥≠∂¡ ß∞‹ªÚ≤ª≥≠µƒ ˝æ›±Í ∂º∆ ˝
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//≥≠±Ì√¸¡Óº∆ ˝>=Fn–Ë∑¢√¸¡Ó ˝;±æFn≥≠±ÌÕÍ≥…
			OK_Fn:
				if(Fn==129)//F129 µ±«∞’˝œÚ”–π¶µÁƒ‹ æ÷µ
				{
					if((UARTCtrl->RMError&0x2)==0)//±æµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√–Ú∫≈Õ®–≈ ß∞‹±Í÷æ,B0= ß∞‹,B1=F129 ß∞‹
					{
				#if(RMM_RS485_FailData_0xEE==1)//RS485≥≠±Ì ß∞‹ ˝æ›ÃÓ0xee;0=∑Ò,1= «
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›;»Ù÷ª¥Ê¥¢ µ ± ˝æ›u8 *p8RMLib=0
				#else
						if(p8RMLib[1]>UARTCtrl->RMCountErr)
						{//≤ª «»´≤ø±Í ∂∂º√ª≥≠
							ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›;»Ù÷ª¥Ê¥¢ µ ± ˝æ›u8 *p8RMLib=0
						}
				#endif
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,0);//µÁƒ‹±Ì≥≠±Ì≥…π¶ ß∞‹º∆ ˝∫Õ±Í÷æ,»Îø⁄UARTª∫≥Âµÿ÷∑,µÁƒ‹±Ì–Ú∫≈,±Í÷æf=0≥…π¶,f=1 ß∞‹
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,0);//÷’∂À485≥≠±Ì ß∞‹ ¬º˛,»Îø⁄≤‚¡øµ„∫≈,f=1≤˙…˙=0ª÷∏¥
						#endif
						MC(0,ADDR_ERC21_Flag,1);//«ÂERC21 485≥≠±Ìπ ’œ ¬º˛º«¬º±Í÷æ
					}
					else
					{
						RM_CountAndFlags(PORTn,UARTCtrl->MeterNo,1);//µÁƒ‹±Ì≥≠±Ì≥…π¶ ß∞‹º∆ ˝∫Õ±Í÷æ,»Îø⁄UARTª∫≥Âµÿ÷∑,µÁƒ‹±Ì–Ú∫≈,±Í÷æf=0≥…π¶,f=1 ß∞‹
						#if ERC31==1
						ERC31_Event(UARTCtrl->MeterNo,Pn,1);//÷’∂À485≥≠±Ì ß∞‹ ¬º˛,»Îø⁄≤‚¡øµ„∫≈,f=1≤˙…˙=0ª÷∏¥
						#endif
						#if ERC21==1
						ERC21_Event();//ERC21 485≥≠±Ìπ ’œ ¬º˛
						#endif
				#if(RMM_RS485_FailData_0xEE==1)//RS485≥≠±Ì ß∞‹ ˝æ›ÃÓ0xee;0=∑Ò,1= «
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›
				#else
						ClrERC29SOUR(Pn);//ºØ÷–≥≠±Ì ß∞‹«ÂERC29µÁƒ‹±Ì∑…◊ﬂ ¬º˛‘≠ º ˝æ›
				#endif
					}
					goto NEXT_Fn;
				}
		#if(RMM_RS485_FailData_0xEE==1)//RS485≥≠±Ì ß∞‹ ˝æ›ÃÓ0xee;0=∑Ò,1= «
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›;»Ù÷ª¥Ê¥¢ µ ± ˝æ›u8 *p8RMLib=0
		#else
				if(UARTCtrl->RMError==0)//±æµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√–Ú∫≈Õ®–≈ ß∞‹±Í÷æ,B0= ß∞‹,B1=F129 ß∞‹
				{
					if(p8RMLib[1]>UARTCtrl->RMCountErr)
					{//≤ª «»´≤ø±Í ∂∂º√ª≥≠
						ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›;»Ù÷ª¥Ê¥¢ µ ± ˝æ›u8 *p8RMLib=0
					}
				}
		#endif
				goto NEXT_Fn;
			}
			if(UARTCtrl->RMError!=0x0)
			{//±æµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√–Ú∫≈Õ®–≈ ß∞‹±Í÷æ,B0= ß∞‹,B1=F129 ß∞‹
		#if(RMM_RS485_FailData_0xEE==1)//RS485≥≠±Ì ß∞‹ ˝æ›ÃÓ0xee;0=∑Ò,1= «
				ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›;»Ù÷ª¥Ê¥¢ µ ± ˝æ›u8 *p8RMLib=0
		#endif
				if(p8RMLib[1]>UARTCtrl->RMCountErr)
				{//≤ª «»´≤ø±Í ∂∂º√ª≥≠
					ReadMeter_SaveFnData(PORTn,Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›;»Ù÷ª¥Ê¥¢ µ ± ˝æ›u8 *p8RMLib=0
				}
//				goto NEXT_Fn;//…œ∫£πÊ‘ºµ•œ‡±Ì√ª”–0x2A◊ÓΩ¸“ª¥Œ–Ë¡ø«Â¡„∑¢…˙ ±øÃ£¨≤ª∑µªÿ»Œ∫Œ ˝æ›÷°£¨≤ªÃ¯µΩœ¬“ª∏ˆFn±‹√‚F27√ª”– ˝æ›°£
			}
		
			Terminal_ReadMeter_ShH_TxData(PORTn,p8RMLib);//÷’∂À…œ∫£πÊ‘º≥≠±Ì∑¢ÀÕ ˝æ›
			
			//Õ®–≈ÀŸ¬ 
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//ƒ¨»œ1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//∆Ù∂Ø
			Init_UARTn(PORTn);//≥ı ºªØUARTø⁄,»Îø⁄Õ®–≈øÿ÷∆◊÷“—ÃÓ»Î
			Start_UARTn_Tx(PORTn);//∆Ù∂ØUARTø⁄∑¢ÀÕ
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//∑Ω∞∏0=÷«ƒ‹±Ì,100=Õ¯¬Á±Ì,200=ºØ÷–∆˜,300=◊®±‰÷’∂À,400=π´±‰÷’∂À,500=≤…ºØ∆˜,600=Õ®–≈ƒ£øÈ
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485≥≠±Ì∑¢ÀÕLED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485≥≠±Ì∑¢ÀÕLED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
			}
#endif
			//œ‘ æ≥≠±Ìµÿ÷∑
			switch(UARTCtrl->RMTaskC)
			{
				case 0://…œµÁ
					CopyString((u8 *)"485 ∆Ù≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
					break;
				case 1://»’≥≠
					if(UARTCtrl->DateReRMCount<=1)//√ø»’»ŒŒÒ(∞¸¿®≤π≥≠»ŒŒÒ)µƒ÷¥––◊‹¥Œ ˝;0=ø’(…œµÁªÚ»’∏¸–¬),1-255=◊‹¥Œ ˝º∆ ˝
					{
						CopyString((u8 *)"485 »’≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
					}
					else
					{
						CopyString((u8 *)"485 ≤π≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
					}
					break;
				case 2://÷‹≥≠
				#if (USER/100)==6//”√ªß±Í ∂:∫˛ƒœœµ
					CopyString((u8 *)"485  ±≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
				#else
					CopyString((u8 *)"485 ÷‹≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
				#endif
					break;
				case 3://∂®≥≠
					CopyString((u8 *)"485 ∂®≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
					break;
				case 5:// ¬º˛
					CopyString((u8 *)"485  ¬º˛",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
					break;
				default://—≠≥≠
					CopyString((u8 *)"485 —≠≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
					break;
			}
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			MWR(PORT485,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8rxbuff[7-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//∏ÊæØ∑¢…˙,»Îø⁄∏ÊæØ¥˙¬Î∫Õ∏ÊæØ◊÷∑˚¥Æ
			break;
		case 4://4=’˝‘⁄∑¢ÀÕ
			if(Wait_UARTn_Tx(PORTn)==0)//µ»¥˝UARTø⁄∑¢ÀÕ,∑µªÿ0=∑¢ÀÕÕÍ≥…,1=∑¢ÀÕ√ªÕÍ≥…
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645∂¡±Ì≥¨ ±∂® ±÷µms
				UARTCtrl->RxByte=0;//RXº∆ ˝(“—Ω” ’◊÷Ω⁄ ˝)=0
			}
			break;
		case 5://5=’˝‘⁄Ω” ’
			i=ShH_Rx(PORTn);//…œ∫£πÊ‘º÷°Ω” ’;∑µªÿ0√ªÕÍ≥…,1ÕÍ≥…,2≥¨ ±
			switch(i)
			{
				case 0://0√ªÕÍ≥…
					return;
				case 1://1ÕÍ≥…
					break;
				default://2≥¨ ±
					UARTCtrl->Task=2;//2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ)
					return;
			}	
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//∑Ω∞∏0=÷«ƒ‹±Ì,100=Õ¯¬Á±Ì,200=ºØ÷–∆˜,300=◊®±‰÷’∂À,400=π´±‰÷’∂À,500=≤…ºØ∆˜,600=Õ®–≈ƒ£øÈ
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485≥≠±ÌΩ” ’LED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485≥≠±ÌΩ” ’LED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
			}
#endif
			p16timer[0]=30/10;//œ¬÷°∆Ù∂Ø∑¢ÀÕ—” ±20MS∫Û
			
			//±»Ωœ∑¢ ’µÿ÷∑°¢√¸¡Ó¬Î°¢ºı0x33
			i=ShH_RxData(PORTn);//…œ∫£πÊ‘º÷°Ω” ’ ˝æ›,±»Ωœ∑¢ ’µÿ÷∑°¢√¸¡Ó¬Î°¢ºı0x33;∑µªÿ0’˝»∑,1µÿ÷∑¥Ì,2√¸¡Ó¬Î¥Ì
			if(i!=0)
			{
				UARTCtrl->Task=2;//2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ)
				break;
			}
			
			UARTCtrl->Task=1;//ÕÍ≥…1÷°∑¢ ’
			p8RMLib=(u8 *)ShH_RMLib[UARTCtrl->FnCount];
			ReadMeter_ShH_FnData(PORTn,p8RMLib);//…œ∫£πÊ‘º≥≠±ÌΩ” ’ ˝æ›ÃÓ»ÎFn ˝æ›ª∫≥Â
			UARTCtrl->RMCount+=1;//≥≠±Ì√¸¡Óº∆ ˝+1
			UARTCtrl->ReRMCount=0;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝=0
			break;
	}
}


void Terminal_ReadMeter_ShH_Fn(u32 PORTn,u8 *p8RMLib)//Ωˆ≥≠1∏ˆFn ˝æ›ÃÓ–¥µΩFnDataBuff;Task=0=ø’œ–(±æFn≥≠∂¡ÕÍ≥…)
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
		
	switch(UARTCtrl->Task)//0=ø’œ–,1=ÕÍ≥…1÷°∑¢ ’,2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ),3=∆Ù∂Ø∑¢ÀÕ,4=’˝‘⁄∑¢ÀÕ,5=’˝‘⁄Ω” ’
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=ø’œ–
			break;
		case 1://1=ÕÍ≥…1÷°∑¢ ’
			UARTCtrl->Task=3;
			break;
		case 2://2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ)
			if(UARTCtrl->ReRMCount<1)
			{
				UARTCtrl->ReRMCount+=1;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝+1
			}
			else
			{
				UARTCtrl->RMCount+=1;//≥≠±Ì√¸¡Óº∆ ˝+1
				UARTCtrl->ReRMCount=0;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝
			}
			UARTCtrl->Task=3;
			break;
		case 3://3=∆Ù∂Ø∑¢ÀÕ
			Pn=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+2,2);
			if(Pn==0x0)
			{//±æ–Ú∫≈µƒµÁƒ‹±Ì/Ωª¡˜≤…—˘◊∞÷√…æ≥˝
			END_Fn:
				UARTCtrl->Task=0;//ø’œ–
				UARTCtrl->RMCount=0;//≥≠±Ì√¸¡Óº∆ ˝=0
				UARTCtrl->ReRMCount=0;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝
				break;
			}
			if((p8RMLib[3]&0x2)==0x0)
			{//±æFn≤ª «RS485≥≠±Ì
				goto END_Fn;
			}
//			Fn=p8RMLib[0];
		
			if(p8RMLib[1]==0x0)
			{//±æFn≥≠±Ì√¸¡Ó ˝=0
				goto END_Fn;
			}
			if(UARTCtrl->RMCount==0x0)
			{//≥≠±Ì√¸¡Óº∆ ˝=0,«ÂFn ˝æ›ª∫≥Â
				MC(0xee,Get_ADDR_UARTnFnDataBuff(PORTn),Get_LEN_UARTnFnDataBuff(PORTn));
			}
			if(UARTCtrl->RMCount>=p8RMLib[1])
			{//≥≠±Ì√¸¡Óº∆ ˝>=Fn–Ë∑¢√¸¡Ó ˝;±æFn≥≠±ÌÕÍ≥…
//				ReadMeter_SaveFnData(Fn,Pn,(u32)p8FnData,p8RMLib);//¥Ê÷¸≥≠±ÌFn ˝æ›
				goto END_Fn;
			}
			
			Terminal_ReadMeter_ShH_TxData(PORTn,p8RMLib);//÷’∂À…œ∫£πÊ‘º≥≠±Ì∑¢ÀÕ ˝æ›
			
			//Õ®–≈ÀŸ¬ 
			i=MRR(ADDR_AFN04F10+((UARTCtrl->MeterNo-1)*LEN_AFN04F10_Pn)+4,1);
			if((i&0xe0)==0x0)
			{//ƒ¨»œ1200
				UARTCtrl->BpsCtrl=(2<<5)+0x0b;
			}
			else
			{
				UARTCtrl->BpsCtrl=(i&0xe0)+0x0b;
			}

			//∆Ù∂Ø
			Init_UARTn(PORTn);//≥ı ºªØUARTø⁄,»Îø⁄Õ®–≈øÿ÷∆◊÷“—ÃÓ»Î
			Start_UARTn_Tx(PORTn);//∆Ù∂ØUARTø⁄∑¢ÀÕ
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//∑Ω∞∏0=÷«ƒ‹±Ì,100=Õ¯¬Á±Ì,200=ºØ÷–∆˜,300=◊®±‰÷’∂À,400=π´±‰÷’∂À,500=≤…ºØ∆˜,600=Õ®–≈ƒ£øÈ
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485≥≠±Ì∑¢ÀÕLED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485≥≠±Ì∑¢ÀÕLED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
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
			//œ‘ æ≥≠±Ìµÿ÷∑
			p8rxbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			CopyString((u8 *)"485  µ≥≠",(u8*)ADDR_DATABUFF);//øΩ±¥◊÷∑˚¥Æ;∑µªÿøΩ±¥◊÷∑˚¥Æ◊÷Ω⁄ ˝(≤ªº∆Ω· ¯0)
			MWR(Pn,ADDR_DATABUFF+3,1);
			for(i=0;i<6;i++)
			{
				x=p8rxbuff[7-i];
				MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
				MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
			}
			MWR(0,ADDR_DATABUFF+8+12,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//∏ÊæØ∑¢…˙,»Îø⁄∏ÊæØ¥˙¬Î∫Õ∏ÊæØ◊÷∑˚¥Æ
			break;
		case 4://4=’˝‘⁄∑¢ÀÕ
			if(Wait_UARTn_Tx(PORTn)==0)//µ»¥˝UARTø⁄∑¢ÀÕ,∑µªÿ0=∑¢ÀÕÕÍ≥…,1=∑¢ÀÕ√ªÕÍ≥…
			{
				UARTCtrl->Task=5;
				p16timer[0]=ReadDL645OverTimer/10;//DL645∂¡±Ì≥¨ ±∂® ±÷µms
				UARTCtrl->RxByte=0;//RXº∆ ˝(“—Ω” ’◊÷Ω⁄ ˝)=0
			}
			break;
		case 5://5=’˝‘⁄Ω” ’
			i=ShH_Rx(PORTn);//…œ∫£πÊ‘º÷°Ω” ’;∑µªÿ0√ªÕÍ≥…,1ÕÍ≥…,2≥¨ ±
			switch(i)
			{
				case 0://0√ªÕÍ≥…
					return;
				case 1://1ÕÍ≥…
					break;
				default://2≥¨ ±
					UARTCtrl->Task=2;//2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ)
					return;
			}	
			
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//∑Ω∞∏0=÷«ƒ‹±Ì,100=Õ¯¬Á±Ì,200=ºØ÷–∆˜,300=◊®±‰÷’∂À,400=π´±‰÷’∂À,500=≤…ºØ∆˜,600=Õ®–≈ƒ£øÈ
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485≥≠±ÌΩ” ’LED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485≥≠±ÌΩ” ’LED÷∏ æMS∂® ±∆˜,√ø10MSºı1∂® ±∆˜
			}
#endif
			p16timer[0]=30/10;//œ¬÷°∆Ù∂Ø∑¢ÀÕ—” ±20MS∫Û
			
			//±»Ωœ∑¢ ’µÿ÷∑°¢√¸¡Ó¬Î°¢ºı0x33
			i=ShH_RxData(PORTn);//…œ∫£πÊ‘º÷°Ω” ’ ˝æ›,±»Ωœ∑¢ ’µÿ÷∑°¢√¸¡Ó¬Î°¢ºı0x33;∑µªÿ0’˝»∑,1µÿ÷∑¥Ì,2√¸¡Ó¬Î¥Ì
			if(i!=0)
			{
				UARTCtrl->Task=2;//2=Ω” ’≥¨ ±(ªÚ¥ÌŒÛ)
				break;
			}
			UARTCtrl->Task=1;//ÕÍ≥…1÷°∑¢ ’
			
			ReadMeter_ShH_FnData(PORTn,p8RMLib);//…œ∫£πÊ‘º≥≠±ÌΩ” ’ ˝æ›ÃÓ»ÎFn ˝æ›ª∫≥Â
			UARTCtrl->RMCount+=1;//≥≠±Ì√¸¡Óº∆ ˝+1
			UARTCtrl->ReRMCount=0;//≥≠±Ì√¸¡Ó÷ÿ∑¢º∆ ˝=0
			break;
	}
}


#endif





















