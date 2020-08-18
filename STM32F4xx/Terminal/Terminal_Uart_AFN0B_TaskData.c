
//ÇëÇóÈÎÎñÊı¾İ
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"


extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
//#define Class2Data_MaxFn	218//Àà2Êı¾İ×î´óFn
u32 Get_Class2Data_MaxFn(void);








void Uart_AFN0B(u32 PORTn)//ÇëÇóÈÎÎñÊı¾İ
{
	u32 i;
	u32 x;
	u32 y;
	u32 t;
	u32 Fn;
	u32 Pn;
	u32 DIlen;
	u32 DIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 SourFn;
	u32 ALLPn;//È«²¿ÓĞĞ§Á¿µã±êÖ¾£¬0=·ñ,1=ÊÇ
  u32 LEN_UARTnTx;
	
	u8 * p8rxbuff;
	u8 * p8txbuff;
	UARTCtrl_TypeDef * UARTCtrl;
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//µÃµ½UART¿ØÖÆÇøµØÖ·
	LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
	
	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//ÏìÓ¦¹¦ÄÜÂë 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//TpÊ±¼ä±êÇ©ÓĞĞ§ÅĞ¶¨
	if((p8rxbuff[13]&0x80)!=0x0)
	{//ÓĞTp
		if(DIlen<(12+6))
		{
err:
			//È«²¿·ñÈÏ
			p8txbuff[6]=(p8txbuff[6]&0xf0)+9;//ÏìÓ¦¹¦ÄÜÂë 9ÎŞÊı¾İ
			Uart_376_ACK(PORTn,2);//È«²¿È·ÈÏ£¯·ñÈÏÖ¡;ACK=1È·ÈÏ=2·ñÈÏ;³ö¿ÚÌîĞ´Êı¾İ³¤¶È,¿ØÖÆÂë,AFN,Ö¡ĞòÁĞÓò,Êı¾İµ¥Ôª±êÊ¶
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//¸½¼ÓĞÅÏ¢
			Uart_376_CS((u32)p8txbuff);//ºÍĞ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ĞŞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//Ê±¼ä±êÇ©ÓĞĞ§ÅĞ¶¨,Èë¿ÚÊ±¼ä±êÇ©Ê×Ö·,³ö¿Ú0ÓĞĞ§,1ÎŞĞ§
		if(i!=0x0)
		{
			return;//ÉáÆú¸Ã±¨ÎÄ
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//ÓÃ»§ÇøÊı¾İ±êÊ¶³¤¶È
	if((p8rxbuff[13]&0x80)!=0x0)
	{//ÓĞTp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	TXaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXlen=0;
	CopyDI(PORTn,DIaddr);//COPYÊı¾İ±êÊ¶µ½¿ØÖÆ»º³å×¼±¸·Ö½â
	i=MRR((u32)(&UARTCtrl->SourDI),2);
	if(i!=0x00ff)
	{//±¾Êı¾İ±êÊ¶ÎªÈ«²¿ÓĞĞ§²âÁ¿µã
		ALLPn=0;//È«²¿ÓĞĞ§Á¿µã±êÖ¾£¬0=·ñ,1=ÊÇ
	}
	else
	{
		ALLPn=1;//È«²¿ÓĞĞ§Á¿µã±êÖ¾£¬0=·ñ,1=ÊÇ
	}
	SourFn=0;
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//Êı¾İ±êÊ¶·Ö½âÎªFnPn
		if(i==0x0)//Êı¾İ±êÊ¶·Ö½âÎªFnPn
		{
			switch(SourFn)
			{
				case 1:
					DIlen-=4;
					DIaddr+=4;
					break;
				case 2:
					if(ALLPn==0x0)
					{//±¾Êı¾İ±êÊ¶ÎªÈ«²¿ÓĞĞ§²âÁ¿µã,·¢ËÍÊ±Êı¾İ±êÊ¶²»½øĞĞ×éºÏ
						DIlen-=4;
						DIaddr+=4;
					}
					else
					{
						DIlen-=9;
						DIaddr+=9;
					}
					break;
				default:
					goto err;
			}
			CopyDI(PORTn,DIaddr);//COPYÊı¾İ±êÊ¶µ½¿ØÖÆ»º³å×¼±¸·Öâ

			i=MRR((u32)(&UARTCtrl->SourDI),2);
			if(i!=0x00ff)
			{//±¾Êı¾İ±êÊ¶ÎªÈ«²¿ÓĞĞ§²âÁ¿µã
				ALLPn=0;//È«²¿ÓĞĞ§Á¿µã±êÖ¾£¬0=·ñ,1=ÊÇ
			}
			else
			{
				ALLPn=1;//È«²¿ÓĞĞ§Á¿µã±êÖ¾£¬0=·ñ,1=ÊÇ
			}
			continue;
		}
		Fn=(i>>16);
		Pn=i&0xffff;
		SourFn=Fn;
		if(Pn==0x0)
		{
			goto err;
		}
		if(Pn>MaxTimerTask)
		{//È«²¿PnÍê
			if(ALLPn!=0x0)
			{//±¾Êı¾İ±êÊ¶ÎªÈ«²¿ÓĞĞ§²âÁ¿µã,·¢ËÍÊ±Êı¾İ±êÊ¶²»½øĞĞ×éºÏ
				MC(0,(u32)(&UARTCtrl->DestDI),4);
				continue;
			}
			goto err;
		}
		Pn--;
		switch(Fn)
		{
			case 1://ÇëÇó¶¨Ê±ÉÏ±¨1ÀàÊı¾İÈÎÎñ
				i=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Pn)+8,1);//
				i*=4;
				if((TXlen+32+i)>LEN_UARTnTx)
				{
					goto err;
				}
				MR(TXaddr,ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Pn)+9,i);
				TXaddr+=i;
				TXlen+=i;
				break;
			case 2://ÇëÇó¶¨Ê±ÉÏ±¨2ÀàÊı¾İÈÎÎñ
				if(DIlen<9)
				{
					goto err;
				}
				x=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+8,1);//
				for(i=0;i<x;i++)
				{
					y=BitLoca(MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i)+2,1));
					y+=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i)+3,1)*8;
					if((y>=Get_Class2Data_MaxFn())||(y==0x0))
					{//FnÎŞĞ§Ê±²»×°Èë,ÒòÊ±±ê³¤¶È²»¿ÉÖª
						if(ALLPn==0)//È«²¿ÓĞĞ§Á¿µã±êÖ¾£¬0=·ñ,1=ÊÇ
						{
							DIlen-=5;
							DIaddr+=5;
						}
						continue;
					}
					if((TXlen+32+4)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+9+(4*i),4);//È¡DI
					TXaddr+=4;
					TXlen+=4;
					switch(Terminal_Class2DataLib[y].CongealType)//¶³½áÀàĞÍ:0=ÈÕ¶³½á,1=³­±íÈÕ¶³½á,2=ÔÂ¶³½á,3=ÇúÏß
					{
						case 0://ÈÕ¶³½á
							if((TXlen+32+3)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+2,TXaddr,3);
							TXaddr+=3;
							TXlen+=3;
							break;
						case 1://³­±íÈÕ¶³½á
							if((TXlen+32+3)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+2,TXaddr,3);
							TXaddr+=3;
							TXlen+=3;
							break;
						case 2://ÔÂ¶³½á(ÉÏ±¨ÉÏ1ÔÂ)
							if((TXlen+32+2)>LEN_UARTnTx)
							{
								goto err;
							}
							MW(DIaddr+4+3,TXaddr,2);
							TXaddr+=2;
							TXlen+=2;
							break;
						case 3://ÇúÏß(ÉÏ±¨ÉÏ1ÈÕÕûÈÕµãÊı)
/*
#define CurveCongealTime     15//ÇúÏß¶³½á¼ä¸ôÊ±¼ä(ÃÜ¶È)0=²»¶³½á;15=15,30,45,0;30=30,0;60=0;5=5,10,15...0;1=1,2,3...0
ÇúÏß³éÈ¡Êı¾İ±¶ÂÊR£ºÈ¡Öµ·¶Î§1¡«96
Èç±»³éÈ¡µÄÊı¾İµÄ¶³½áÃÜ¶Èm=2£¬¼´Ã¿30·ÖÖÓ¶³½áÒ»¸öÖµ£¬ÄÇÃ´µ±R=2Ê±£¬±íÊ¾°´60·ÖÖÓ³éÈ¡£¬R=1Ê±£¬±íÊ¾ÈÔ°´30·ÖÖÓ³éÈ¡
Êı¾İ¶³½áÃÜ¶Èm			
0	²»¶³½á	ÎŞ
1	15	15·Ö¡¢30·Ö¡¢45·Ö¡¢0·Ö
2	30	30·Ö¡¢0·Ö£»
3	60	0·Ö
254	5	5·Ö¡¢10·Ö¡¢15·Ö......¡¢0·Ö
255	1	1·Ö¡¢2·Ö¡¢3·Ö¡¢......0·Ö
Êı¾İµãÊın
*/
							if((TXlen+32+7)>LEN_UARTnTx)
							{
								goto err;
							}
							MR(TXaddr,DIaddr+4,5);
							//ÇúÏß¶³½á¼ä¸ôÊ±¼ä*³éÈ¡Êı¾İ±¶ÂÊ
							t=CurveCongealTime*MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Pn)+7,1);
							switch(t)
							{
								case 15:
									y=1;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
									break;
								case 30:
									y=2;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
									break;
								case 60:
									y=3;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
									break;
								case 5:
									y=254;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
									break;
								case 1:
									y=255;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
									break;
								default:
									t=CurveCongealTime;
									switch(t)
									{
										case 15:
											y=1;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
											break;
										case 30:
											y=2;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
											break;
										case 60:
											y=3;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
											break;
										case 5:
											y=254;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
											break;
										case 1:
											y=255;//Òª¶ÁÈ¡ÇúÏßµÄÊı¾İÃÜ¶Èm
											break;
									}
									break;
							}
							MC(y,TXaddr+5,1);//Êı¾İ¶³½áÃÜ¶Èm
							t=(24*60)/t;//Õû1ÌìµãÊı
							MC(t,TXaddr+6,1);//Êı¾İ¶³½áÃÜ¶Èm
							TXaddr+=7;
							TXlen+=7;
							break;
					}
				}
				if(ALLPn==0)//È«²¿ÓĞĞ§Á¿µã±êÖ¾£¬0=·ñ,1=ÊÇ
				{
					DIlen-=5;
					DIaddr+=5;
				}
				break;
			default:
				goto err;
		}
	}
	if(TXlen==0)
	{
		goto err;
	}
	if((p8rxbuff[13]&0x80)!=0x0)
	{//ÓĞTp
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		i=((u32)p8rxbuff)+6+DIlen-6;
		MR(TXaddr,i,6);
		TXaddr+=6;
		TXlen+=6;
	}
	TXlen+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	TXlen+=8;
	i=Get_LEN_UARTnRx(PORTn);
	if(TXlen>i)
	{
		goto err;
	}
	MR((u32)p8rxbuff,(u32)p8txbuff,TXlen+8);
	if(Fn==1)
	{
		UARTCtrl->LockFlags|=0x40;//Í¨ĞÅ¹¦ÄÜ±»½ûÖ¹±êÖ¾:B0=±£Áô,B1=ÕıÔÚ×Ô¶¯ËÑ±í,B2=±¾ÈÕÒÑ×Ô¶¯ËÑ±í,B3=±£Áô,B4=Ö÷¶¯1Àà¶¨Ê±ÈÎÎñ,B5=Ö÷¶¯2Àà¶¨Ê±ÈÎÎñ,B6=ÕÙ¶Á1Àà¶¨Ê±ÈÎÎñ,B7
		Uart_AFN0C(PORTn);//Àà1Êı¾İ
	}
	else
	{
		Uart_AFN0D(PORTn);//Àà2Êı¾İ
	}
}







