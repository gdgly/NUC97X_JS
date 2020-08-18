
//Êý¾Ý×ª·¢´¦Àí¹ý³Ì
#include "Project.h"
#include "Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"

#include "Terminal_Uart_3762_RxTx.h"
#include "../Display/Warning.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../Device/IC_BUZZ.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0E_Event.h"


	#if(USER/100)==0
#define MaxReRMCount     2//1//³­±íÖØ·¢´ÎÊý(½öÖØ·¢´ÎÊý,²»Ëã³õÊ¼1´ÎµÄ·¢ËÍ);µçÁ¦ÏßÔØ²¨=1,Î¢¹¦ÂÊÎÞÏß=2
	#else
#define MaxReRMCount     0//³­±íÖØ·¢´ÎÊý(½öÖØ·¢´ÎÊý,²»Ëã³õÊ¼1´ÎµÄ·¢ËÍ)
	#endif


void Forwarding(u32 PORTn)//Êý¾Ý×ª·¢´¦Àí¹ý³Ì
{
	u32 i;
	u32 Fn;
	u32 DIaddr;
	u32 n;
	u32 DI;
	u64 CommAddr;
	u32 PORTRS485;
	u32 x;
	u32 y;
	u32 z;
	u32 Protocol;
	
	u8 * p8;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl_TypeDef * UARTCtrlRS485;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	u16 * p16timer;
	u16 * p16;
	u32 * p32;
#if ((Project/100)==2)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
	u64 d64;
//	#if (USER/100)==0//µç¿ÆÔº²âÊÔ
//	u8 *p8d;
//	#endif
#endif
	

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);//µÃµ½UART¿ØÖÆÇøµØÖ·
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	
#if (USER/100)==0//µç¿ÆÔº²âÊÔ
//ÔØ²¨Í¨µÀ×ÜÊÇÎÞÆô³­

#else
 #if NewERC14==1//ERC14Í£µçÊÂ¼þ¼ÇÂ¼·½·¨0=Ô­±ê×¼,1=¹úÍøÓªÏú²¿2014.1.15Í¨Öª
//	if(Terminal_Ram->PowerTask<=10)//Í£µçÊÂ¼þÈÎÎñ
//	{
//		if(Comm_Ram->SelfTestCommand!=0x55555555)//×Ô²âÊÔÃüÁî0x55555555²âÊÔ1(Ó²¼þ),0xAAAAAAAA²âÊÔ2(FLASH),0x5555AAAA²âÊÔ3(FLASHÊÙÃü²âÊÔ)
//		{
//			goto err;
//		}
//	}
 #endif
#endif//µç¿ÆÔº²âÊÔ

	CopyDI(PORTn,DIaddr);//COPYÊý¾Ý±êÊ¶µ½¿ØÖÆ»º³å×¼±¸·Ö½â
	i=DItoFnPn(PORTn);//Êý¾Ý±êÊ¶·Ö½âÎªFnPn
	Fn=(i>>16);
//if(p8rxbuff[18]==2)
//{
//	p8rxbuff[18]=31;//½öÄ£ÄâÌ¨Ìå×ßÔØ²¨¿ÚESAMÄÚ²¿²âÊÔÓÃ
//}
	switch(p8rxbuff[18])
	{
		case 1://ÄÚ²¿½»Á÷²ÉÑù½Ó¿Ú
			PORTRS485=ACSAMPLEPORT;
		case 2://RS485-1
			PORTRS485=RS485_1PORT;
			break;
		case 3://RS485-2
			PORTRS485=RS485_2PORT;
			break;
		case 4://RS485-3
			PORTRS485=RS485_3PORT;
			break;
		case 31://¶Ë¿ÚºÅ=31,ÔØ²¨
			PORTRS485=RS485_4PORT;
			break;
		default:
			UARTCtrl->Task=0;
			return;
	}
	switch(Fn)
	{
		case 1://F1Í¸Ã÷×ª·¢
			switch(UARTCtrl->Task)
			{
				case 8://8=µÈ´ýÍ¨¹ýÆäËü¿ÚÊý¾Ý×ª·¢
					switch(p8rxbuff[18])
					{
						case 1://ÄÚ²¿½»Á÷²ÉÑù½Ó¿Ú
							p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//×ª·¢×Ö½ÚÊý
							p8=(u8 *)((u32)p8rxbuff+18+6);
							//×÷ÎªÕýÈ·Ö¡¿ÉÄÜÇ°µ¼0xfeÈ¥³ý,´Ó0x68¿ªÊ¼
							while(i!=0x0)
							{
								if(p8[0]==0x68)
								{
									break;
								}
								p8++;
								i--;
							}
							if(i!=0x0)
							{
								if(i>Get_LEN_UARTnRx(ACSAMPLEPORT))
								{//×ª·¢³¤¶ÈÌ«³¤´í
									goto err;
								}
								MW((u32)p8,Get_ADDR_UARTnRx(ACSAMPLEPORT),i);
								MC(0xaa,Get_ADDR_UARTnRx(ACSAMPLEPORT)+1,6);//Í¨ÐÅµØÖ·ÓÃ0xaa
								i=UARTCtrlRS485->Task;//Ô­ÈÎÎñ¼Ä´æ
								UARTCtrlRS485->Task=1;//1=½ÓÊÕÍê³É
							  if(FileDownload->File&0x04)//ÎÄ¼þ±êÖ¾(ÖÃÎ»±íÊ¾OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
							  {
									p32=(u32 *)(ADDR_Meter_ENTRY);
							    (*(void(*)())p32[130])((u32)ACSAMPLEPORT);//µ÷ÓÃvoid Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645½ÓÊÕ·¢ËÍ
								}
								UARTCtrlRS485->Task=i;//Ô­ÈÎÎñ»Ö¸´
							}
							i=UARTCtrlRS485->TxByte;//·¢ËÍ×Ö½ÚÊý
//							if(i==0x0)
//							{//Ã»ÊÕµ½Êý¾Ý
//								goto err;
//							}
							MW(Get_ADDR_UARTnTx(ACSAMPLEPORT),((u32)p8txbuff)+21,i);
							p8txbuff[19]=i;
							p8txbuff[20]=i>>8;
							i+=7;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,2×Ö½Ú³¤¶È
							i+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
							p8txbuff[1]=i;
							p8txbuff[2]=i>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
							Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
							break;
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							if(p16timer[0]==0)//µÈ´ýÊý¾Ý×ª·¢¿Ú¿ÕÏÐ¶¨Ê±5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
								if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
								{//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
									UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±
								}
							}
							break;
#if ((Project/100)==2)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
						case 31://¶Ë¿ÚºÅ=31,ÔØ²¨
							//¶Ë¿ÚºÅ=31,×ª·¢Ö»ÄÜ¶ÔDL645-1997»ò2007½øÐÐ
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ¼ÆÊý,ÔÚÃ¿Ö¡½ÓÊÕOKÊ±³õÊ¼»¯Îª0)
							{
								case 0://µÈ´ý¿Ú¿Õ±ÕÊ±½øÈë
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//¶àÖ¡ÏÂ·¢ÊµÊ±³­¶Á»òÊý¾Ý×ª·¢:0=Ã¿Ö¡³­¶ÁÏìÓ¦,1=·ÅÆúÇ°ÃæÖ¡Ö»³­¶Á×îºóÖ¡²»¼ì²é×îºóÖ¡¿ÉÄÜÊÇÐÄÌøµÈµÄÈ·ÈÏÖ¡
									DMA_UARTn_RX_Pointer(PORTn);//¼ÆËãDMA_UARTn½ÓÊÕÖ¸ÕëÖµ(p0)
									if(p16[0]!=p16[1])
									{//ÒÑ½ÓÊÕµ½ÏÂÖ¡
										goto err;
									}
								#endif
									if(p16timer[0]==0)//µÈ´ýÊý¾Ý×ª·¢¿Ú¿ÕÏÐ¶¨Ê±120S
									{
										goto err;
									}
									if(Terminal_Router->RouterInitTask<200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
									{
										break;
									}
									if(Terminal_Router->UpDateTask!=0)//Èí¼þ¸üÐÂÈÎÎñ
									{
										p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)	
										goto err;
									}
									if(UARTCtrlRS485->Lock==0x55)
									{
										goto err;//¿ÚÏÂÔÚ±»Õ¼ÓÃ
									}
									else
									{//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
										{//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
											p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
											UARTCtrlRS485->Lock=0x55;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
											UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
										}
									}
									break;
								case 1://ÔÝÍ£Â·ÓÉ
									if(Terminal_Router->RouterCtrl!=0)//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//ÔÝÍ£Â·ÓÉ±êÖ¾
									Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									break;
								case 2://ÔÝÍ£Â·ÓÉ½ÓÊÕ
									if(Terminal_Router->RouterCtrl==0)//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									{
										UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
										UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									}
									break;
								case 3://Â·ÓÉ×ª·¢Êý¾Ý
									x=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//×ª·¢×Ö½ÚÊý
									if((x<14)||(x>(128-2)))
									{//DL645-1997×îÐ¡14×Ö½Ú,DL645-2007×îÐ¡16×Ö½Ú,×î´ó×ª·¢(128-2)×Ö½Ú
										Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
										Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
										UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										goto err;
									}
									y=0;//²éÇ°µ¼0xfe¸öÊý
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									if((p8rxbuff[18+6+y]!=0x68)||(p8rxbuff[18+6+y+7]!=0x68))
									{//²»ÊÇDL645±ê×¼Ö¡
										Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
										Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
										UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										goto err;
									}
									for(i=0;i<6;i++)
									{
										if(p8rxbuff[18+6+y+1+i]!=0x99)
										{
											break;
										}
									}
									if(i==6)
									{//ÊÇ¹ã²¥ÃüÁî
										UARTCtrl->SubTask=5;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
										break;
									}
								#if(USER/100)==7//Õã½­ÓÃ»§
									if(p8rxbuff[18+6+y+8]==0x08)
									{//ÊÇÖ¸¶¨±íµØÖ·Ð£Ê±ÃüÁî
										UARTCtrl->SubTask=10;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
										break;
									}
								#endif
//								#if(USER/100)==0//¹úÍøËÍÑù
//									if(p8rxbuff[18+6+y+8]==0x14)
//									{//Ð´ÃüÁî
//										i=MRR(((u32)p8rxbuff)+18+6+y+8+2,4);
//										if((i==0x3733343F)||(i==0x37333435))
//										{
//											UARTCtrl->SubTask=10;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
//											break;
//										}
//									}
//								#endif
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1==2)//Â·ÓÉÔËÐÐÄ£Ê½_Í¨ÐÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ"£¬ÆäËüÈ¡Öµ±£Áô
									{
										p8[3]=0x43;//¿ØÖÆÂë;¼¯ÖÐÊ½Â·ÓÉÔØ²¨Í¨ÐÅ
									}
									else
									{
										if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËÐÐÄ£Ê½_Í¨ÐÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ"£¬ÆäËüÈ¡Öµ±£Áô
										{
											p8[3]=0x4a;//¿ØÖÆÂë;Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ
										}
										else
										{
											p8[3]=0x41;//¿ØÖÆÂë;¼¯ÖÐÊ½Â·ÓÉÔØ²¨Í¨ÐÅ
										}
									}
									p8[4]=4;//Í¨ÐÅÄ£¿é±êÊ¶=1
									p8[5]=0;
									p8[6]=0;//Ô¤¼ÆÓ¦´ð×Ö½ÚÊý
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//Æô¶¯Ö¡ÐòÁÐÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ýÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
									p8[9]=UARTCtrlRS485->TxSEQ;//Æô¶¯Ö¡ÐòÁÐÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ýÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
									MR(((u32)p8)+16,((u32)p8rxbuff)+18+6+y+1,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[18+6+y+8];//È¡¿ØÖÆÂë
									if((i&0x10)==0x10)
									{
										if(i==0x10)
										{//1997µÄ×î´óÐèÁ¿ÇåÁã
											p8[25]=1;//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
										}
										else
										{
											p8[25]=2;//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
										}
									}
									else
									{
										p8[25]=1;//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ=0Í¸Ã÷´«Êä,1=DL645-1997,2=DL645-2007
									}
									p8[26]=0;//Í¨ÐÅÑÓÊ±Ïà¹Ø±êÖ¾
									p8[27]=0;//¸½Êô½ÚµãÊý=0
									
									p8[28]=x-y;//DL645±¨ÎÄ³¤¶È(Ç°µ¼0xfe²»¼ÓÈë)
									MR(((u32)p8)+29,((u32)p8rxbuff)+18+6+y,x-y);//×ª·¢ÄÚÈÝ(Ç°µ¼0xfe²»¼ÓÈë)
									
									i=29+p8[28]+2;//Ðè×Ü·¢ËÍ×Ö½ÚÊý
									p8[1]=i;
									p8[2]=i>>8;
									if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËÐÐÄ£Ê½_Í¨ÐÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ"£¬ÆäËüÈ¡Öµ±£Áô
									{//Î¢¹¦ÂÊÎÞÏß,µµ°¸µØÖ·ÓÃ²É¼¯Æ÷µØÖ·
										CommAddr=MRR(((u32)p8)+16,6);
										x=Check_AFN04F10_Addr(31,CommAddr);//¼ì²éÖÕ¶ËµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÅäÖÃµÄÍ¨ÐÅµØÖ·;·µ»Ø0ÎÞÏàÍ¬,!=0ÏàÍ¬µØÖ·µÄÅäÖÃÐòºÅ(1¿ªÊ¼)
										if(x!=0)
										{
											d64=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
											if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
											{
												MWR(d64,((u32)p8)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
											}
										}
									}
									Uart_3762_CS((u32)p8);//ºÍÐ£Ñé¼ÆËã²¢Ð´Èë(Í¬Ê±Ð´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕýÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
									UARTCtrlRS485->TxByte=i;//·¢ËÍ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									CopyString((u8 *)"ÔØ²¨F1  ",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(18+6+y+1+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û?
									break;
								case 4://×ª·¢½ÓÊÕ
									if(UARTCtrlRS485->Task>=3)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=p8[1]+(p8[2]<<8);
									if((UARTCtrlRS485->Task!=1)||(i<=16))
									{//³¬Ê±»ò·ñÈÏ»Ø´ð
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									}
									else
									{
										y=p8[28];//·µ»Ø645Ö¡×Ü³¤¶È
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											UARTCtrlRS485->Task=5;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
											UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(i==0x0200)
											{//AFN=00,Fn=2Ö±½Ó·ñÈÏÖ¡(Î¢¹¦ÂÊÎÞÏßÓÐ´ËÇé¿ö)
												p16[0]=6000/10;//6Ãë³¬Ê±Ê±¼ä
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
											}
											return;
										}
										//µØÖ·±È½Ï
										//²éµ±Ç°½ÓÊÕÇ°µ¼0xfe¸öÊý
										for(i=0;i<14;i++)
										{
											if(p8[29+i]!=0xfe)
											{
												break;
											}
										}
										i=DataComp(((u32)p8)+29+1+i,Get_ADDR_UARTnTx(PORTRS485)+29+1,6);//Êý¾Ý±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
										if(i!=0)
										{//²»ÏàÍ¬
											UARTCtrlRS485->Task=5;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
											UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(y==0)
											{//Ã»ÊÕµ½Êý¾Ý
												p16[0]=6000/10;//6Ãë³¬Ê±Ê±¼ä
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
											}
											return;
										}
									}
									if(y==0x0)
									{//Ã»ÊÕµ½Êý¾Ý,ÔØ²¨Â·ÓÉÓÐÊý¾Ý·µ»Ø¼´Îª±ê×¼645Ö¡²»Ðè¼ì²é·µ»ØÖ¡ºÏ·¨ÐÔ
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//ÖØ·¢¼ÆÊý
										{
										#if MulRealRead==1//¶àÖ¡ÏÂ·¢ÊµÊ±³­¶Á»òÊý¾Ý×ª·¢:0=Ã¿Ö¡³­¶ÁÏìÓ¦,1=·ÅÆúÇ°ÃæÖ¡Ö»³­¶Á×îºóÖ¡²»¼ì²é×îºóÖ¡¿ÉÄÜÊÇÐÄÌøµÈµÄÈ·ÈÏÖ¡
											p16=(u16 *)Get_ADDR_UARTn(PORTn);
											DMA_UARTn_RX_Pointer(PORTn);//¼ÆËãDMA_UARTn½ÓÊÕÖ¸ÕëÖµ(p0)
											if(p16[0]==p16[1])
											{//Ã»½ÓÊÕµ½ÏÂÖ¡
												UARTCtrl->ReRMCount++;//ÖØ·¢¼ÆÊý
												UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
												return;
											}
										#else
											UARTCtrl->ReRMCount++;//ÖØ·¢¼ÆÊý
											UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
											return;
										#endif
										}
									#endif
									}
						BroadcastESC:
									Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
									Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									UARTCtrlRS485->Task=0;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									MW(((u32)p8)+28+1,((u32)p8txbuff)+21,y);
									p8txbuff[19]=y;
									p8txbuff[20]=y>>8;
									y+=7;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,2×Ö½Ú³¤¶È
									y+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
									p8txbuff[1]=y;
									p8txbuff[2]=y>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
									Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
									break;
								case 5://ÔØ²¨¹ã²¥×ª·¢
									y=0;//²éÇ°µ¼0xfe¸öÊý
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									if(p8rxbuff[18+6+y+8]==0x08)
									{//ÊÇ¹ã²¥Ð£Ê±
										UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
									i=p8rxbuff[18+6+y+8];//È¡¿ØÖÆÂë
									if((i&0x10)==0x10)
									{
										if(i==0x10)
										{//1997µÄ×î´óÐèÁ¿ÇåÁã
											p8[0]=1;//¿ØÖÆ×Ö,0=Í¸Ã÷´«Êä£¬1=645-1997£¬2=645-2007£¬3=ÏàÎ»Ê¶±ð
										}
										else
										{
											p8[0]=2;//¿ØÖÆ×Ö,0=Í¸Ã÷´«Êä£¬1=645-1997£¬2=645-2007£¬3=ÏàÎ»Ê¶±ð
										}
									}
									else
									{
										p8[0]=1;//¿ØÖÆ×Ö,0=Í¸Ã÷´«Êä£¬1=645-1997£¬2=645-2007£¬3=ÏàÎ»Ê¶±ð
									}
									i=p8rxbuff[18+4];
									p8[1]=i;//±¨ÎÄ³¤¶È
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,i);
									Uart_3762_TxData(PORTRS485,0x05,3,2+i);//376.2·¢ËÍÊý¾ÝÌîÈë·¢ËÍÇø,Èë¿ÚÊ±ÈôÓÐÐÅÏ¢ÓòÄ¿µÄµØÖ·Êý¾Ýµ¥ÔªµÈÔòÒÑÌîÈë,Êý¾Ýµ¥ÔªÔÝ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
									UARTCtrl->SubTask=8;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									if(Terminal_Router->RouterRunMode_7==0)//Â·ÓÉÔËÐÐÄ£Ê½_¹ã²¥ÃüÁîÈ·ÈÏ·½Ê½£º0±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÍ¨ÐÅÄ£¿éÖ´ÐÐ¹ã²¥Í¨ÐÅ¹ý³ÌÍê±Ïºó·µ»ØÈ·ÈÏ±¨ÎÄ£¬1±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÐÅµÀÖ´ÐÐ¹ã²¥Í¨ÐÅÖ®Ç°¾Í·µ»ØÈ·ÈÏ±¨ÎÄ£¬ÐèÒª¼¯ÖÐÆ÷µÈ´ýµÄÊ±¼äÔÚÈ·ÈÏ±¨ÎÄµÄ"µÈ´ýÖ´ÐÐÊ±¼ä"ÐÅÏ¢ÖÐÌåÏÖ
									{
										Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//×î´ó¹ã²¥³¬Ê±Ê±¼äÃë(°ë×Ö¶ÔÆë)
									}
									WarningOccur(PORTRS485,(u8 *)"ÔØ²¨×ª·¢¹ã²¥Êý¾Ý");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
									break;
								case 6://¹ã²¥Ð£Ê±.Í¨ÐÅÑÓÊ±Ïà¹Ø¹ã²¥Í¨ÐÅÊ±³¤²éÑ¯
									x=p8rxbuff[18+4];//×ª·¢×Ö½ÚÊý
									if(x>(128-2))
									{
										x=(128-2);
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
								#if (USER/100)<=1
									p8[0]=2;//¿ØÖÆ×Ö,0=Í¸Ã÷´«Êä£¬1=645-1997£¬2=645-2007£¬3=ÏàÎ»Ê¶±ð
								#else
									p8[0]=0;//¿ØÖÆ×Ö,0=Í¸Ã÷´«Êä£¬1=645-1997£¬2=645-2007£¬3=ÏàÎ»Ê¶±ð
								#endif
									p8[1]=x;//±¨ÎÄ³¤¶È
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,x);
									Uart_3762_TxData(PORTRS485,0x03,9,2+x);//376.2·¢ËÍÊý¾ÝÌîÈë·¢ËÍÇø;Èë¿ÚÊ±ÈôÓÐÐÅÏ¢ÓòÄ¿µÄµØÖ·Êý¾Ýµ¥ÔªµÈÔòÒÑÌîÈëÇÒÊý¾Ýµ¥ÔªÔÝ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)	
									WarningOccur(PORTRS485,(u8 *)"ÔØ²¨×ª·¢¹ã²¥Ð£Ê±");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Öû®
									break;
								case 7://¹ã²¥Ð£Ê±.½ÓÊÕÊ±³¤²éÑ¯,·¢¹ã²¥Ð£Ê±
									if(UARTCtrlRS485->Task>=3)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=MRR(((u32)p8)+10,3);
									if((i!=0x010103)||(UARTCtrlRS485->Task!=1))
									{
										Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
										Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
										UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										goto err;
									}
									i=MRR(((u32)p8)+13,2);//¹ã²¥Í¨ÐÅÑÓ³ÙÊ±¼äÃë
									y=0;//²éÇ°µ¼0xfe¸öÊý
									for(x=0;x<14;x++)
									{
										if(p8rxbuff[18+6+x]!=0xfe)
										{
											break;
										}
										y++;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
								#if (USER/100)<=1
									p8[0]=2;//¿ØÖÆ×Ö,0=Í¸Ã÷´«Êä£¬1=645-1997£¬2=645-2007£¬3=ÏàÎ»Ê¶±ð
								#else
									p8[0]=0;//¿ØÖÆ×Ö,0=Í¸Ã÷´«Êä£¬1=645-1997£¬2=645-2007£¬3=ÏàÎ»Ê¶±ð
								#endif
									p8[1]=y+18;//±¨ÎÄ³¤¶È
									MR(((u32)p8)+2,((u32)p8rxbuff)+18+6,y+18);
									for(x=0;x<6;x++)//Ô­¹ã²¥Ê±¼ä¼õ0x33
									{
										p8[2+y+10+x]-=0x33;
									}
									d64=MRR(((u32)p8)+2+y+10,6);
									d64=YMDHMS_AddS(d64,i);//ÄêÔÂÈÕÊ±·ÖÃë¼ÓSÃë,·µ»Ø¼ÓSÃëºóµÄÄêÔÂÈÕÊ±·ÖÃë
									MWR(d64,((u32)p8)+2+y+10,6);
									for(i=0;i<6;i++)
									{
										p8[2+y+10+i]+=0x33;
									}
									x=0;
									for(i=0;i<16;i++)
									{
										x+=p8[2+y+i];
									}
									p8[y+18]=x;
									p8[y+19]=0x16;
									Uart_3762_TxData(PORTRS485,0x05,3,y+20);//376.2·¢ËÍÊý¾ÝÌîÈë·¢ËÍÇø,Èë¿ÚÊ±ÈôÓÐÐÅÏ¢ÓòÄ¿µÄµØÖ·Êý¾Ýµ¥ÔªµÈÔòÒÑÌîÈë,Êý¾Ýµ¥ÔªÔÝ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									if(Terminal_Router->RouterRunMode_7==0)//Â·ÓÉÔËÐÐÄ£Ê½_¹ã²¥ÃüÁîÈ·ÈÏ·½Ê½£º0±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÍ¨ÐÅÄ£¿éÖ´ÐÐ¹ã²¥Í¨ÐÅ¹ý³ÌÍê±Ïºó·µ»ØÈ·ÈÏ±¨ÎÄ£¬1±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÐÅµÀÖ´ÐÐ¹ã²¥Í¨ÐÅÖ®Ç°¾Í·µ»ØÈ·ÈÏ±¨ÎÄ£¬ÐèÒª¼¯ÖÐÆ÷µÈ´ýµÄÊ±¼äÔÚÈ·ÈÏ±¨ÎÄµÄ"µÈ´ýÖ´ÐÐÊ±¼ä"ÐÅÏ¢ÖÐÌåÏÖ
									{
										Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxBroadcastOverTime;//×î´ó¹ã²¥³¬Ê±Ê±¼äÃë(°ë×Ö¶ÔÆë)
									}
									break;
								case 8://ÔØ²¨¹ã²¥×ª·¢.½ÓÊÕÂ·ÓÉÈ·ÈÏÖ¡
									if(Terminal_Router->RouterRunMode_7==0)//Â·ÓÉÔËÐÐÄ£Ê½_¹ã²¥ÃüÁîÈ·ÈÏ·½Ê½£º0±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÍ¨ÐÅÄ£¿éÖ´ÐÐ¹ã²¥Í¨ÐÅ¹ý³ÌÍê±Ïºó·µ»ØÈ·ÈÏ±¨ÎÄ£¬1±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÐÅµÀÖ´ÐÐ¹ã²¥Í¨ÐÅÖ®Ç°¾Í·µ»ØÈ·ÈÏ±¨ÎÄ£¬ÐèÒª¼¯ÖÐÆ÷µÈ´ýµÄÊ±¼äÔÚÈ·ÈÏ±¨ÎÄµÄ"µÈ´ýÖ´ÐÐÊ±¼ä"ÐÅÏ¢ÖÐÌåÏÖ
									{//0±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÍ¨ÐÅÄ£¿éÖ´ÐÐ¹ã²¥Í¨ÐÅ¹ý³ÌÍê±Ïºó·µ»ØÈ·ÈÏ±¨ÎÄ
										switch(UARTCtrlRS485->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
										{
											case 1://1=Íê³É1Ö¡·¢ÊÕ
												if(Get_Uart_3762_RxData_AFN_Fn(PORTRS485)==0x0001)//UART_3762½ÓÊÕÊý¾ÝµÃµ½AFN,Fn;B15-B8=AFN,B7-B0=Fn
												{
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
													goto BroadcastESC;
												}
												else
												{
													UARTCtrlRS485->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
													UARTCtrlRS485->Task=5;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
												}
												break;
											case 0://0=¿ÕÏÐ
											case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
												if(Terminal_Ram->RouterCheck_S_Timer==0)//×î´ó¹ã²¥³¬Ê±Ê±¼äÃë(°ë×Ö¶ÔÆë)
												{
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
													goto BroadcastESC;
												}
												else
												{
													UARTCtrlRS485->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
													UARTCtrlRS485->Task=5;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
												}
												break;
										}
									}
									else
									{//1±íÊ¾¹ã²¥ÃüÁîÔÚ±¾µØÐÅµÀÖ´ÐÐ¹ã²¥Í¨ÐÅÖ®Ç°¾Í·µ»ØÈ·ÈÏ±¨ÎÄ£¬ÐèÒª¼¯ÖÐÆ÷µÈ´ýµÄÊ±¼äÔÚÈ·ÈÏ±¨ÎÄµÄ"µÈ´ýÖ´ÐÐÊ±¼ä"ÐÅÏ¢ÖÐÌåÏÖ
										switch(UARTCtrlRS485->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
										{
											case 1://1=Íê³É1Ö¡·¢ÊÕ
												if(Get_Uart_3762_RxData_AFN_Fn(PORTRS485)==0x0001)//UART_3762½ÓÊÕÊý¾ÝµÃµ½AFN,Fn;B15-B8=AFN,B7-B0=Fn
												{
													//È¡µÈ´ýÊ±¼ä
													p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
													i=p8[17];
													i+=p8[18]<<8;
													i*=1000/10;
													if(i>0xffff)
													{
														i=0xffff;
													}
													p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
													p16timer[0]=i;
													UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
													break;
												}
											case 0://0=¿ÕÏÐ
											case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
												goto BroadcastESC;
										}
									}
									break;
								case 9://µÈ´ý¹ã²¥ÑÓÊ±
									p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
									if(p16timer[0]==0)
									{
										p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										goto BroadcastESC;
									}
									break;
						#if (USER/100)==7//Õã½­ÓÃ»§
								case 10://Ö¸¶¨±íµØÖ·Ð£Ê±.Í¨ÐÅÑÓÊ±Ïà¹Ø¹ã²¥Í¨ÐÅÊ±³¤²éÑ¯
									x=p8rxbuff[18+4];//×ª·¢×Ö½ÚÊý
									if(x>(128-2))
									{
										x=(128-2);
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[4]=0x04;
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
									y=0;//²éÇ°µ¼0xfe¸öÊý
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									for(i=0;i<6;i++)
									{
										p8[16+i]=p8rxbuff[18+6+y+1+i];
									}

									p8+=128;
									p8[0]=0;//Í¨ÐÅÐ­ÒéÀàÐÍ£º00HÎªÍ¸Ã÷´«Êä£»01HÎª DL/T 645-1997£»02HÎª DL/T 645-2007£»03H¡«FFH±£Áô
									p8[1]=1;//										
									p8[2]=0;//										
									p8[3]=x;//±¨ÎÄ³¤¶È
									MR(((u32)p8)+4,((u32)p8rxbuff)+18+6,x);
									Uart_3762_TxData(PORTRS485,0x13,1,4+x);//376.2·¢ËÍÊý¾ÝÌîÈë·¢ËÍÇø;Èë¿ÚÊ±ÈôÓÐÐÅÏ¢ÓòÄ¿µÄµØÖ·Êý¾Ýµ¥ÔªµÈÔòÒÑÌîÈëÇÒÊý¾Ýµ¥ÔªÔÝ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)	
									WarningOccur(PORTRS485,(u8 *)"ÔØ²¨×ª·¢Ð£Ê±");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Öû®
									break;
								case 11://Ö¸¶¨±íµØÖ·Ð£Ê±.½ÓÊÕÊ±³¤²éÑ¯,·¢¹ã²¥Ð£Ê±
									if(UARTCtrlRS485->Task>=3)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									{
										break;
									}
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=MRR(((u32)p8)+10,3);
									if((i!=0x000414)||(UARTCtrlRS485->Task!=1))
									{
										Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
										Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
										UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										goto err;
									}
									i=MRR(((u32)p8)+19,2);//¹ã²¥Í¨ÐÅÑÓ³ÙÊ±¼äÃë
									y=0;//²éÇ°µ¼0xfe¸öÊý
									for(x=0;x<14;x++)
									{
										if(p8rxbuff[18+6+x]!=0xfe)
										{
											break;
										}
										y++;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8+=128;
									p8[0]=y+18;//±¨ÎÄ³¤¶È
									MR(((u32)p8)+1,((u32)p8rxbuff)+18+6,y+18);
									for(x=0;x<6;x++)//Ô­¹ã²¥Ê±¼ä¼õ0x33
									{
										p8[1+y+10+x]-=0x33;
									}
									d64=MRR(((u32)p8)+1+y+10,6);
									d64=YMDHMS_AddS(d64,i);//ÄêÔÂÈÕÊ±·ÖÃë¼ÓSÃë,·µ»Ø¼ÓSÃëºóµÄÄêÔÂÈÕÊ±·ÖÃë
									MWR(d64,((u32)p8)+1+y+10,6);
									for(i=0;i<6;i++)
									{
										p8[1+y+10+i]+=0x33;
									}
									x=0;
									for(i=0;i<16;i++)
									{
										x+=p8[1+y+i];
									}
									p8[y+17]=x;
									p8[y+18]=0x16;
									Uart_3762_TxData(PORTRS485,0x14,3,y+19);//376.2·¢ËÍÊý¾ÝÌîÈë·¢ËÍÇø,Èë¿ÚÊ±ÈôÓÐÐÅÏ¢ÓòÄ¿µÄµØÖ·Êý¾Ýµ¥ÔªµÈÔòÒÑÌîÈë,Êý¾Ýµ¥ÔªÔÝ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
									break;
								case 12://ÔØ²¨¹ã²¥×ª·¢.½ÓÊÕÂ·ÓÉÈ·ÈÏÖ¡
									switch(UARTCtrlRS485->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									{
										case 1://1=Íê³É1Ö¡·¢ÊÕ
											p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
											i=MRR(((u32)p8)+22,3);
											if(i==0x000113)
											{
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=p8[28];//·µ»Ø645Ö¡×Ü³¤¶È
												goto BroadcastESC;
											}
											else
											{
												UARTCtrlRS485->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
												UARTCtrlRS485->Task=5;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
												p16timer[0]=RouterTxtoRxOverTimer;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
											}
											break;
										case 0://0=¿ÕÏÐ
										case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
											if(Terminal_Ram->RouterCheck_S_Timer==0)//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
											{
												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
												y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
												goto BroadcastESC;
											}
											else
											{
												UARTCtrlRS485->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
												UARTCtrlRS485->Task=5;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
												p16timer[0]=RouterTxtoRxOverTimer;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
											}
											break;
									}
									break;
						#endif
//						#if(USER/100)==0//¹úÍøËÍÑù
//								case 10://Ö¸¶¨±íµØÖ·Ð£Ê±.Í¨ÐÅÑÓÊ±Ïà¹Ø¹ã²¥Í¨ÐÅÊ±³¤²éÑ¯
//									x=p8rxbuff[18+4];//×ª·¢×Ö½ÚÊý
//									if(x>(128-2))
//									{
//										x=(128-2);
//									}
//									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
//									p8[4]=0x04;
//									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
//									y=0;//²éÇ°µ¼0xfe¸öÊý
//									for(i=0;i<14;i++)
//									{
//										if(p8rxbuff[18+6+i]!=0xfe)
//										{
//											break;
//										}
//										y++;
//									}
//									for(i=0;i<6;i++)
//									{
//										p8[16+i]=p8rxbuff[18+6+y+1+i];
//									}
//
//									p8+=128;
//									p8[0]=0;//Í¨ÐÅÐ­ÒéÀàÐÍ£º00HÎªÍ¸Ã÷´«Êä£»01HÎª DL/T 645-1997£»02HÎª DL/T 645-2007£»03H¡«FFH±£Áô
//									p8[1]=1;//										
//									p8[2]=0;//										
//									p8[3]=x;//±¨ÎÄ³¤¶È
//									MR(((u32)p8)+4,((u32)p8rxbuff)+18+6,x);
//									Uart_3762_TxData(PORTRS485,0x13,1,4+x);//376.2·¢ËÍÊý¾ÝÌîÈë·¢ËÍÇø;Èë¿ÚÊ±ÈôÓÐÐÅÏ¢ÓòÄ¿µÄµØÖ·Êý¾Ýµ¥ÔªµÈÔòÒÑÌîÈëÇÒÊý¾Ýµ¥ÔªÔÝ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
//									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)	
//									WarningOccur(PORTRS485,(u8 *)"ÔØ²¨×ª·¢F1");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Öû®
//									break;
//								case 11://Ö¸¶¨±íµØÖ·Ð£Ê±.½ÓÊÕÊ±³¤²éÑ¯,·¢¹ã²¥Ð£Ê±
//									if(UARTCtrlRS485->Task>=3)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
//									{
//										break;
//									}
//									p8d=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//									i=MRR(((u32)p8d)+10,3);
//									if((i!=0x000414)||(UARTCtrlRS485->Task!=1))
//									{
//										Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
//										Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
//										UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
//										UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
//										goto err;
//									}
//									i=MRR(((u32)p8d)+19,2);//¹ã²¥Í¨ÐÅÑÓ³ÙÊ±¼äÃë
//									y=0;//²éÇ°µ¼0xfe¸öÊý
//									for(x=0;x<14;x++)
//									{
//										if(p8rxbuff[18+6+x]!=0xfe)
//										{
//											break;
//										}
//										y++;
//									}
//									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
//									p8+=128;
//									p8[0]=MRR(((u32)p8d)+21,1);//±¨ÎÄ³¤¶È
//									MR(((u32)p8)+1,((u32)p8rxbuff)+18+6,p8[0]);
//
//									x=MRR(((u32)p8d)+18+6+y+8+2,4);
//									if(x==0x3733343F)
//									{
//										for(x=0;x<7;x++)//Ô­Ê±¼ä¼õ0x33
//										{
//											p8[1+y+22+x]-=0x33;
//										}
//										d64=MRR(((u32)p8)+1+y+22,3);
//										d64+=(MRR(((u32)p8)+1+y+22+4,3)<<24);
//										d64=YMDHMS_AddS(d64,i);//ÄêÔÂÈÕÊ±·ÖÃë¼ÓSÃë,·µ»Ø¼ÓSÃëºóµÄÄêÔÂÈÕÊ±·ÖÃë
//										MWR(d64,((u32)p8)+1+y+22,3);
//										MWR((d64>>24),((u32)p8)+1+y+22+4,3);
//										for(i=0;i<7;i++)
//										{
//											p8[1+y+22+i]+=0x33;
//										}
//									}
//									else
//									{//if(x==0x37333435)
//										for(x=0;x<3;x++)//Ô­Ê±¼ä¼õ0x33
//										{
//											p8[1+y+22+x]-=0x33;
//										}
//										d64=MRR(((u32)p8)+1+y+22,3);
//										d64+=0x010101000000;
//										d64=YMDHMS_AddS(d64,i);//ÄêÔÂÈÕÊ±·ÖÃë¼ÓSÃë,·µ»Ø¼ÓSÃëºóµÄÄêÔÂÈÕÊ±·ÖÃë
//										MWR(d64,((u32)p8)+1+y+22,3);
//										for(i=0;i<3;i++)
//										{
//											p8[1+y+22+i]+=0x33;
//										}
//									}
//									x=0;
//									for(i=0;i<(p8[0]-y-2);i++)
//									{
//										x+=p8[1+y+i];
//									}
//									p8[p8[0]-1]=x;//cs
//									p8[p8[0]]=0x16;
//									
//									Uart_3762_TxData(PORTRS485,0x14,3,p8[0]+1);//376.2·¢ËÍÊý¾ÝÌîÈë·¢ËÍÇø,Èë¿ÚÊ±ÈôÓÐÐÅÏ¢ÓòÄ¿µÄµØÖ·Êý¾Ýµ¥ÔªµÈÔòÒÑÌîÈë,Êý¾Ýµ¥ÔªÔÝ¹Ì¶¨ÌîÈë·¢ËÍ»º³åÇø+128µØÖ·
//									UARTCtrl->SubTask++;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
//									Terminal_Ram->RouterCheck_S_Timer=Terminal_Router->MaxOverTime;//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
//									break;
//								case 12://Ö¸¶¨±íµØÖ·Ð£Ê±.½ÓÊÕÂ·ÓÉÈ·ÈÏÖ¡
//									switch(UARTCtrlRS485->Task)//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
//									{
//										case 1://1=Íê³É1Ö¡·¢ÊÕ
//											p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//											i=MRR(((u32)p8)+22,3);
//											if(i==0x000113)
//											{
//												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//												y=p8[28];//·µ»Ø645Ö¡×Ü³¤¶È
//												goto BroadcastESC;
//											}
//											else
//											{
//												UARTCtrlRS485->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
//												UARTCtrlRS485->Task=5;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
//												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
//												p16timer[0]=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
//											}
//											break;
//										case 0://0=¿ÕÏÐ
//										case 2://2=½ÓÊÕ³¬Ê±(»ò´íÎó)
//											if(Terminal_Ram->RouterCheck_S_Timer==0)//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
//											{
//												p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
//												y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
//												goto BroadcastESC;
//											}
//											else
//											{
//												UARTCtrlRS485->RxByte=0;//RX¼ÆÊý(ÒÑ½ÓÊÕ×Ö½ÚÊý)=0
//												UARTCtrlRS485->Task=5;//UART_3762½ÓÊÕ·¢ËÍ;0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
//												p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
//												p16timer[0]=1000/10;//·¢ËÍºó½ÓÊÕ³¬Ê±¶¨Ê±Öµms/10(°ë×Ö¶ÔÆë)
//											}
//											break;
//									}
//									break;
//						#endif
						
								default:
									UARTCtrl->Task=0;
									Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
									Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±(Ô­×÷Îª³­±í¿ÚÊ±ÖÃÎªµÄÖµ£©
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2.RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//µÃµ½UART¶¨Ê±Æ÷µØÖ·
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								i=p8rxbuff[18+4]+(p8rxbuff[18+5]<<8);//×ª·¢×Ö½ÚÊý
								MW(((u32)p8rxbuff)+18+6,Get_ADDR_UARTnTx(PORTRS485),i);
								UARTCtrlRS485->BpsCtrl=p8rxbuff[19];
								Init_UARTn(PORTRS485);//³õÊ¼»¯UART¿Ú,Èë¿ÚÍ¨ÐÅ¿ØÖÆ×ÖÒÑÌîÈë
								UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý
								UARTCtrlRS485->TxByte=i;//·¢ËÍ×Ö½ÚÊý
								//p16=(u16 *)Get_ADDR_UARTn(PORTRS485);//µÃµ½UART»º³åÊ×µØÖ·
								//p16[0]=0;//FIFOÖ¸Õë
								//p16[1]=0;//FIFOÒÑ¶ÁÖ¸Õë
								Start_UARTn_Tx(PORTRS485);//Æô¶¯UART¿Ú·¢ËÍ
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
					#endif
								UARTCtrl->Task=10;//10=ÕýÔÚÍ¨¹ýÆäËü¿Ú·¢ËÍÊý¾Ý
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=ÕýÔÚÍ¨¹ýÆäËü¿Ú·¢ËÍÊý¾Ý
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//µÈ´ýUART¿Ú·¢ËÍ,·µ»Ø0=·¢ËÍÍê³É,1=·¢ËÍÃ»Íê³É
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								i=p8rxbuff[20];//Í¸Ã÷×ª·¢½ÓÊÕµÈ´ý±¨ÎÄ³¬Ê±Ê±¼ä
								if((i&0x80)==0x0)
								{//10ms
									i*=10;
								}
								else
								{//s
									i&=0x7f;
									i*=1000;
									if(i>0xffff)
									{
										i=0xffff;
									}
								}
								if(i<1000)
								{
									i=1000;//×îÐ¡1000ms
								}
								if(i>ReadDL645OverTimer)
								{
									i=ReadDL645OverTimer;//×î´óReadDL645OverTimer
								}
								i/=10;
								p16[0]=i;
								UARTCtrl->Task=11;//11=ÕýÔÚÍ¨¹ýÆäËü¿Ú½ÓÊÕÊý¾Ý
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=ÕýÔÚÍ¨¹ýÆäËü¿Ú½ÓÊÕÊý¾Ý
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
							p16=(u16 *)Get_ADDR_UARTn(PORTRS485);
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							DMA_UARTn_RX_Pointer(PORTRS485);//¼ÆËãDMA_UARTn½ÓÊÕÖ¸ÕëÖµ(p0)
							if(p16[0]!=p16[1])
							{//ÓÐÊÕµ½×Ö½Ú
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
					#endif
								if(p16[0]>p16[1])
								{
									i=p16[0]-p16[1];
									p16[1]=p16[0];
									UARTCtrlRS485->RxByte+=i;///½ÓÊÕ×Ö½ÚÊý¼Ä´æ
									i=p8rxbuff[21];//Í¸Ã÷×ª·¢½ÓÊÕµÈ´ý×Ö½Ú³¬Ê±Ê±¼ä NIB 10MS
									if(i<(50/10))
									{
										i=50/10;//×îÐ¡50ms
									}
									if(i>(1000/10))
									{
										i=1000/10;//×î´ó1000ms
									}
									p16timer[0]=i;
								}
								else
								{
									UARTCtrlRS485->RxByte=0;//Çå½ÓÊÕ×Ö½ÚÊý¼Ä´æ
									UARTCtrlRS485->Lock=0;//½â³ýµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									UARTCtrlRS485->Task=0;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									goto err;
								}
							}
							else
							{
								if(p16timer[0]==0)
								{//½ÓÊÕÊ±¼äµ½
									y=0;//²é×ª·¢Ö¡Ç°µ¼0xfe¸öÊý
									for(i=0;i<14;i++)
									{
										if(p8rxbuff[18+6+i]!=0xfe)
										{
											break;
										}
										y++;
									}
									i=UARTCtrlRS485->RxByte;//ÊÕµ½Êý¾Ý³¤¶È
									if(i==0x0)
									{//Ã»ÊÕµ½Êý¾Ý
			F1_RS485_ReTx:
										for(i=0;i<6;i++)
										{//ÅÐÊÇ·ñ¹ã²¥µØÖ·
											if(p8rxbuff[18+6+y+1+i]!=0x99)
											{
												break;
											}
										}
										if(i!=6)
										{//²»ÊÇ¹ã²¥µØÖ·,ÖØ·¢
											if(UARTCtrl->ReRMCount<2)//ÖØ·¢¼ÆÊý
											{
												UARTCtrl->ReRMCount++;//ÖØ·¢¼ÆÊý
												p16timer[0]=2000/10;//¹Ì¶¨ÑÓÊ±2ÃëºóÖØ·¢,½ö·ÀÄ£Äâ±í·µ»ØÏìÓ¦ÑÓÊ±
												UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±
												return;
											}
										}
									}
									else
									{//ÓÐÊý¾ÝÊÕµ½
										//¼ì²éÊý¾ÝÖ¡
										x=0;//²é×ª·¢½ÓÊÕÖ¡Ç°µ¼0xfe¸öÊý
										p8=(u8*)(Get_ADDR_UARTn(PORTRS485)+10);
										for(i=0;i<14;i++)
										{
											if(p8[i]!=0xfe)
											{
												break;
											}
											x++;
										}
										//Ö¡Í·0x68±È½Ï
										if((p8[x]!=0x68)||(p8[x+7]!=0x68))
										{
											goto F1_RS485_ReTx;
										}
										//µØÖ·±È½Ï
										for(i=0;i<6;i++)
										{
											if(p8rxbuff[18+6+y+1+i]!=0xaa)
											{
												if(p8rxbuff[18+6+y+1+i]!=p8[x+1+i])
												{
													break;
												}
											}
										}
										if(i!=6)
										{
											goto F1_RS485_ReTx;
										}
									}
									
									i=UARTCtrlRS485->RxByte;//ÊÕµ½Êý¾Ý³¤¶È
									UARTCtrlRS485->Lock=0;//½â³ýµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									UARTCtrlRS485->Task=0;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									UARTCtrlRS485->RxByte=0;//Çå½ÓÊÕ×Ö½ÚÊý¼Ä´æ
									MW(Get_ADDR_UARTn(PORTRS485)+10,((u32)p8txbuff)+21,i);
									p8txbuff[19]=i;
									p8txbuff[20]=i>>8;
									i+=7;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,2×Ö½Ú³¤¶È
									i+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
									p8txbuff[1]=i;
									p8txbuff[2]=i>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
									Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
								}
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
err:
					//È«²¿·ñÈÏ
					Uart_376_ACK(PORTn,2);//È«²¿È·ÈÏ£¯·ñÈÏÖ¡;ACK=1È·ÈÏ=2·ñÈÏ;³ö¿ÚÌîÐ´Êý¾Ý³¤¶È,¿ØÖÆÂë,AFN,Ö¡ÐòÁÐÓò,Êý¾Ýµ¥Ôª±êÊ¶
					i=p8rxbuff[1]+(p8rxbuff[2]<<8);
					Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
					Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
					break;
				default:
					UARTCtrl->Task=0;
					break;
			}
			break;
/*
		case 9://F9×ª·¢Ö÷Õ¾Ö±½Ó¶Ôµç±íµÄ³­¶ÁÊý¾ÝÃüÁî
			switch(UARTCtrl->Task)
			{
				case 8://8=µÈ´ýÍ¨¹ýÆäËü¿ÚÊý¾Ý×ª·¢
					switch(p8rxbuff[18])
					{
						case 1://ÄÚ²¿½»Á÷²ÉÑù½Ó¿Ú
							p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							n=p8rxbuff[19];//×ª·¢ÖÐ¼Ì¼¶Êýn
							if(n==0xff)
							{
								n=0;//ÎÞÖ¸¶¨ÖÐ¼ÌÂ·ÓÉ
							}
							n*=6;//ÖÐ¼Ì¼¶ÊýºÍµØÖ·×Ö½Ú³¤¶È
							n++;
							Protocol=p8rxbuff[19+n+6];//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶ÀàÐÍ
							Protocol&=3;
							if(Protocol==0)
							{//0£º·ûºÏDL/T645-1997£¬ 1£º·ûºÏDL/T645-2007
								goto err;
							}
							CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//×ª·¢Ä¿±êµØÖ·
							DI=MRR(((u32)p8rxbuff)+19+n+6+1,4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
							Read_DL645_2007_TxData(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
							i=UARTCtrlRS485->Task;//Ô­ÈÎÎñ¼Ä´æ
							UARTCtrlRS485->Task=1;//1=½ÓÊÕÍê³É
						  if((Comm_Ram->SoftModule&(1<<1))==0)
						  {
								p32=(u32 *)(ADDR_I1_ENTRY);
						    (*(void(*)())p32[70])((u32)ACSAMPLEPORT);//µ÷ÓÃvoid Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645½ÓÊÕ·¢ËÍ
							}
							UARTCtrlRS485->Task=i;//Ô­ÈÎÎñ»Ö¸´
							p8=(u8*)Get_ADDR_UARTnTx(ACSAMPLEPORT);
							if((p8[8]&0x40)!=0)
							{
								x=4;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
								y=4;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
								MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
							}
							else
							{
								x=5;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
								y=p8[9];//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
								for(i=0;i<y;i++)
								{//Êý¾ÝÓò¼õ0x33
									p8[10+i]-=0x33;
								}
							}
							MWR(CommAddr,((u32)p8txbuff)+19,6);//×ª·¢Ä¿±êµØÖ·
							p8txbuff[25]=x;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
							p8txbuff[26]=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+2
							MR(((u32)p8txbuff)+27,((u32)p8)+10,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
							y+=13;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾,1×Ö½ÚÊý¾ÝÄÚÈÝ×Ö½ÚÊý
							y+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
							p8txbuff[1]=y;
							p8txbuff[2]=y>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
							Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
							break;
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							if(p16timer[0]==0)//µÈ´ýÊý¾Ý×ª·¢¿Ú¿ÕÏÐ¶¨Ê±5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
								if(UARTCtrlRS485->Task==0)
								{//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
									UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±
								}
							}
							break;
#if ((Project/100)==2)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
						case 31://¶Ë¿ÚºÅ=31,ÔØ²¨
							//¶Ë¿ÚºÅ=31,×ª·¢Ö»ÄÜ¶ÔDL645-1997»ò2007½øÐÐ
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ¼ÆÊý,ÔÚÃ¿Ö¡½ÓÊÕOKÊ±³õÊ¼»¯Îª0)
							{
								case 0://µÈ´ý¿Ú¿Õ±ÕÊ±½øÈë
									n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
									if(p16[0]!=p16[1])
									{//ÒÑ½ÓÊÕµ½ÏÂÖ¡
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										goto F9err;
									}
									if(p16timer[0]==0)//µÈ´ýÊý¾Ý×ª·¢¿Ú¿ÕÏÐ¶¨Ê±120S
									{
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										goto F9err;
									}
									if(Terminal_Router->RouterInitTask<200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
									{
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
										goto F9err;
									}
									if(Terminal_Router->UpDateTask!=0)//Èí¼þ¸üÐÂÈÎÎñ
									{
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
										goto F9err;
									}
									if(UARTCtrlRS485->Lock!=0x55)
									{//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										//if(UARTCtrlRS485->Task==0)
										if(UARTCtrlRS485->Task!=4)
										{//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
											p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
											UARTCtrlRS485->Lock=0x55;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
											UARTCtrl->SubTask=1;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
										}
									}
									break;
								case 1://ÔÝÍ£Â·ÓÉ
									if(Terminal_Router->RouterCtrl!=0)//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//ÔÝÍ£Â·ÓÉ
									Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrl->SubTask=2;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									break;
								case 2://ÔÝÍ£Â·ÓÉ½ÓÊÕ
									if(Terminal_Router->RouterCtrl==0)//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									{
										UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
										UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									}
									break;
								case 3://Â·ÓÉ×ª·¢Êý¾Ý
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1!=3)//Â·ÓÉÔËÐÐÄ£Ê½_Í¨ÐÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ"£¬ÆäËüÈ¡Öµ±£Áô
									{
										p8[3]=0x41;//¿ØÖÆÂë;¼¯ÖÐÊ½Â·ÓÉÔØ²¨Í¨ÐÅ
									}
									else
									{
										p8[3]=0x4a;//¿ØÖÆÂë;Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ
									}
									p8[4]=4;//Í¨ÐÅÄ£¿é±êÊ¶=1
									p8[5]=0;
									p8[6]=0;//Ô¤¼ÆÓ¦´ð×Ö½ÚÊý
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//Æô¶¯Ö¡ÐòÁÐÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ýÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
									p8[9]=UARTCtrlRS485->TxSEQ;//Æô¶¯Ö¡ÐòÁÐÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ýÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
									n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									MR(((u32)p8)+16,((u32)p8rxbuff)+18+2+n,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
									CommAddr=MRR(((u32)p8)+16,6);//Í¨ÐÅµØÖ·
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[18+2+n+6];//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ
									i++;
									p8[25]=i;
									DI=MRR(((u32)p8rxbuff)+18+2+n+6+1,4);//È¡Êý¾Ý±êÊ¶
									p8[26]=0;//Í¨ÐÅÏà¹Ø±êÖ¾
									p8[27]=0;//¸½Êô½ÚµãÊý=0
									if(i==0x1)
									{//DL/T645-1997
										p8[28]=14;//DL645±¨ÎÄ³¤¶È
										Read_DL645_1997_TxData(((u32)p8)+29,DI,CommAddr);//DL645-1997¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
									}
									else
									{
										p8[28]=16;//DL645±¨ÎÄ³¤¶È
										Read_DL645_2007_TxData(((u32)p8)+29,DI,CommAddr);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
									}
									i=31+p8[28];//Ðè×Ü·¢ËÍ×Ö½ÚÊý
									p8[1]=i;
									p8[2]=i>>8;
									Uart_3762_CS((u32)p8);//ºÍÐ£Ñé¼ÆËã²¢Ð´Èë(Í¬Ê±Ð´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕýÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
									UARTCtrlRS485->TxByte=i;//·¢ËÍ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
									UARTCtrl->SubTask=4;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									//WarningOccur(PORTRS485,(u8 *)"ÔØ²¨×ª·¢Ö±½Ó³­¶ÁF9");//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û´®
									CopyString((u8 *)"ÔØ²¨F9  ",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(18+2+n+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û
									break;
								case 4://×ª·¢½ÓÊÕ
									if(UARTCtrlRS485->Task>=3)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									{
										break;
									}
							
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									if(UARTCtrlRS485->Task!=1)
									{
										x=1;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									}
									else
									{
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											x=4;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
											y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										}
										else
										{
											x=5;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
											y=p8[28];//·µ»ØÊý¾ÝÓò³¤¶È
											if(y==0)
											{//½ÓÊÕÊý¾Ý³¤¶È=0
												x=4;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
												y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
											}
											else
											{
												for(i=0;i<y;i++)
												{//È¥Ç°µ¼0xfe
													if(p8[29+i]!=0xfe)
													{
														if(i!=0)
														{
															MR(((u32)p8)+29,((u32)p8)+29+i,y-i);
														}
														break;
													}
												}
												y=p8[29+9];//·µ»ØÊý¾ÝÓò³¤¶È
												p8+=29;
												//¿ØÖÆÂë
												if((p8[8]&0x40)!=0x0)
												{//´ÓÕ¾Òì³£»Ø´ð
													x=4;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
													y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
												}
												else
												{
													//CSÐ£Ñé
													Fn=0;
													for(i=0;i<(y+10);i++)
													{
														Fn+=p8[i];
													}
													Fn&=0xff;
													if(Fn!=p8[i])
													{
														x=2;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
														y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
													}
													//Êý¾ÝÓò¼õ0x33
													p8=(u8 *)(Get_ADDR_UARTnRx(PORTRS485)+29+10);
													for(i=0;i<y;i++)
													{
														p8[i]-=0x33;
													}
												}
											}
										}
									}
									Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
									Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
									if(n==0xff)
									{
										n=0;
									}
									if(n>32)
									{
										n=32;
									}
									n*=6;
									//Ä¿µÄµØÖ·±È½Ï
									if(x==5)
									{//5=×ª·¢½ÓÊÕÊý¾Ý
					 					if(DataComp(((u32)p8rxbuff)+18+2+n,Get_ADDR_UARTnRx(PORTRS485)+29+1,6)!=0x0)//Êý¾Ý±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
										{//²»Í¬
											x=2;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
											y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										}
									}
									if(y==0x0)
									{//Ã»ÊÕµ½Êý¾Ý,ÔØ²¨Â·ÓÉÓÐÊý¾Ý·µ»Ø¼´Îª±ê×¼645Ö¡²»Ðè¼ì²é·µ»ØÖ¡ºÏ·¨ÐÔ
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//ÖØ·¢¼ÆÊý
										{
											p16=(u16 *)Get_ADDR_UARTn(PORTn);
											if(p16[0]==p16[1])
											{//Ã»½ÓÊÕµ½ÏÂÖ¡
												UARTCtrl->ReRMCount++;//ÖØ·¢¼ÆÊý
												UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
												return;
											}
										}
									#endif
									}
							F9err:
									UARTCtrlRS485->Task=0;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									MR(((u32)p8txbuff)+18+1,((u32)p8rxbuff)+18+2+n,6);//×ª·¢Ä¿±êµØÖ·
									p8txbuff[18+7]=x;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
									if(y==0x0)
									{//ÎÞ·µ»Ø
										i=p8rxbuff[18+2+n+6];//È¡×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ
										if(i==0x0)
										{//1997
											y=2;
										}
										else
										{//2007
											y=4;
										}
										p8txbuff[18+8]=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+2
										MR(((u32)p8txbuff)+18+9,((u32)p8rxbuff)+18+2+n+6+1,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
									}
									else
									{//ÓÐ·µ»Ø
										p8txbuff[18+8]=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+2
										MR(((u32)p8txbuff)+18+9,Get_ADDR_UARTnRx(PORTRS485)+29+10,y);
									}
									i=9+y+12;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ+4×Ö½Ú±êÊ¶=12×Ö½Ú
									p8txbuff[1]=i;
									p8txbuff[2]=i>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
									Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
									break;
								default:
									Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
									Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrl->Task=0;
									UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±(Ô­×÷Îª³­±í¿ÚÊ±ÖÃÎªµÄÖµ£©
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//µÃµ½UART¶¨Ê±Æ÷µØÖ·
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								n=p8rxbuff[19];//×ª·¢ÖÐ¼Ì¼¶Êýn
								if(n==0xff)
								{
									n=0;//ÎÞÖ¸¶¨ÖÐ¼ÌÂ·ÓÉ
								}
								n*=6;//ÖÐ¼Ì¼¶ÊýºÍµØÖ·×Ö½Ú³¤¶È
								n++;
								CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//×ª·¢Ä¿±êµØÖ·
								DI=MRR(((u32)p8rxbuff)+19+n+6+1,4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
								i=p8rxbuff[19+n+6];//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶ÀàÐÍ
								i&=3;
								MC(0xfe,Get_ADDR_UARTnTx(PORTRS485),4);
								if(i==0)
								{//0£º·ûºÏDL/T645-1997
									UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//Ä¬ÈÏ1200
									Read_DL645_1997_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
									UARTCtrlRS485->TxByte=4+14;//·¢ËÍ×Ö½ÚÊý
								}
								else
								{//1£º·ûºÏDL/T645-2007
									UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//Ä¬ÈÏ2400
									Read_DL645_2007_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
									UARTCtrlRS485->TxByte=4+16;//·¢ËÍ×Ö½ÚÊý
								}
								i=Check_AFN04F10_Addr(p8rxbuff[18],CommAddr);//¼ì²éÖÕ¶ËµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÅäÖÃµÄÍ¨ÐÅµØÖ·;·µ»Ø0ÎÞÏàÍ¬,!=0ÏàÍ¬µØÖ·µÄÅäÖÃÐòºÅ(1¿ªÊ¼)
								if(i!=0)
								{//ÕÒµ½
									//Í¨ÐÅËÙÂÊ
									i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
									if((i&0xe0)!=0x0)
									{//²»ÊÇÄ¬ÈÏ
										UARTCtrlRS485->BpsCtrl=(i&0xe0)+0x0b;
									}
								}
								Init_UARTn(PORTRS485);//³õÊ¼»¯UART¿Ú,Èë¿ÚÍ¨ÐÅ¿ØÖÆ×ÖÒÑÌîÈë
								Start_UARTn_Tx(PORTRS485);//Æô¶¯UART¿Ú·¢ËÍ
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
					#endif
								UARTCtrl->Task=10;//10=ÕýÔÚÍ¨¹ýÆäËü¿Ú·¢ËÍÊý¾Ý
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=ÕýÔÚÍ¨¹ýÆäËü¿Ú·¢ËÍÊý¾Ý
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//µÈ´ýUART¿Ú·¢ËÍ,·µ»Ø0=·¢ËÍÍê³É,1=·¢ËÍÃ»Íê³É
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								p16[0]=ReadDL645OverTimer/10;//×ª·¢½ÓÊÕµÈ´ý±¨ÎÄ³¬Ê±Ê±¼ä
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý
								UARTCtrl->Task=11;//11=ÕýÔÚÍ¨¹ýÆäËü¿Ú½ÓÊÕÊý¾Ý
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=ÕýÔÚÍ¨¹ýÆäËü¿Ú½ÓÊÕÊý¾Ý
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							i=DL645_Rx(PORTRS485);//DL645Ö¡½ÓÊÕ;·µ»Ø0Ã»Íê³É,1Íê³É,2³¬Ê±
							if(i==0)
							{
								break;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							n=p8rxbuff[19];//×ª·¢ÖÐ¼Ì¼¶Êýn
							if(n==0xff)
							{
								n=0;//ÎÞÖ¸¶¨ÖÐ¼ÌÂ·ÓÉ
							}
							n*=6;//ÖÐ¼Ì¼¶ÊýºÍµØÖ·×Ö½Ú³¤¶È
							n++;
							Protocol=p8rxbuff[19+n+6];//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶ÀàÐÍ
							Protocol&=3;//0=·ûºÏDL/T645-1997,1=·ûºÏDL/T645-2007
							if(i==2)
							{//2=³¬Ê±
								if(UARTCtrl->ReRMCount<2)//ÖØ·¢¼ÆÊý
								{
									UARTCtrl->ReRMCount++;//ÖØ·¢¼ÆÊý
									UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±
									break;
								}
							}
							if(i==1)
							{//1Íê³É
								if(Protocol==0)
								{//0£º·ûºÏDL/T645-1997
									x=DL645_1997_RxData(PORTRS485);//DL645_1997Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í
								}
								else
								{//1£º·ûºÏDL/T645-2007
									x=DL645_2007_RxData(PORTRS485);//DL645_2007Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í
								}
								if(x!=0)
								{
									if(x!=2)
									{
										//µØÖ·»òÊý¾Ý±êÊ¶´í,µç¿ÆÔº²âÊÔÈí¼þÄ£Äâ±í»áÍ£Ö¹µ«½ÓÊÕÊý¾ÝÕý³£,»Ö¸´Ä£Äâ±íºó»á½«ÊÕµ½µÄÖ¡Í¬Ê±·¢³ö
										//¼ÌÐø½ÓÊÕ
										p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
										p16[0]=ReadDL645OverTimer/10;//DL645¶Á±í³¬Ê±¶¨Ê±Öµms
										UARTCtrlRS485->RxByte=0;//Çå½ÓÊÕ×Ö½ÚÊý¼Ä´æ
										break;
									}
								}
							}
							
							UARTCtrlRS485->Lock=0;//½â³ýµ÷ÓÃ
							UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
							UARTCtrlRS485->RxByte=0;//Çå½ÓÊÕ×Ö½ÚÊý¼Ä´æ
							UARTCtrlRS485->Task=0;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
							p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
							if(i!=1)
							{
								x=1;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
								if(Protocol==0x0)
								{//1997
									y=2;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
								}
								else
								{//2007
									y=4;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
								}
								MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
							}
							else
							{
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
					#endif
								if((p8[8]&0x40)!=0)
								{
									x=4;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
									if(Protocol==0x0)
									{//1997
										y=2;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									}
									else
									{//2007
										y=4;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									}
									MR(((u32)p8)+10,((u32)p8rxbuff)+19+n+6+1,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
								}
								else
								{
									x=5;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
									y=p8[9];//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
								}
							}
							CommAddr=MRR(((u32)p8rxbuff)+19+n,6);//×ª·¢Ä¿±êµØÖ·
							MWR(CommAddr,((u32)p8txbuff)+19,6);//×ª·¢Ä¿±êµØÖ·
							p8txbuff[25]=x;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
							p8txbuff[26]=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+2
							MR(((u32)p8txbuff)+27,((u32)p8)+10,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
							y+=13;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾,1×Ö½ÚÊý¾ÝÄÚÈÝ×Ö½ÚÊý
							y+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
							p8txbuff[1]=y;
							p8txbuff[2]=y>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
							Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾?
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
			}
			break;
*/
	
		case 9://F9×ª·¢Ö÷Õ¾Ö±½Ó¶Ôµç±íµÄ³­¶ÁÊý¾ÝÃüÁî
		case 10://F10×ª·¢Ö÷Õ¾Ö±½Ó¶Ôµç±íµÄÅúÁ¿³­¶ÁÊý¾ÝÃüÁî
			switch(UARTCtrl->Task)
			{
				case 8://8=µÈ´ýÍ¨¹ýÆäËü¿ÚÊý¾Ý×ª·¢
					switch(p8rxbuff[18])
					{
						case 1://ÄÚ²¿½»Á÷²ÉÑù½Ó¿Ú
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
							n=p8rxbuff[19];//×ª·¢ÖÐ¼Ì¼¶Êýn
							if(n==0xff)
							{
								n=0;//ÎÞÖ¸¶¨ÖÐ¼ÌÂ·ÓÉ
							}
							n*=6;//ÖÐ¼Ì¼¶ÊýºÍµØÖ·×Ö½Ú³¤¶È
							CommAddr=MRR(((u32)p8rxbuff)+20+n,6);//×ª·¢Ä¿±êµØÖ·
							Protocol=p8rxbuff[20+n+6];//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶ÀàÐÍ
							Protocol&=3;
							if(Protocol==0)
							{//0:·ûºÏDL/T645-1997,1:·ûºÏDL/T645-2007
								goto err;
							}
							Protocol=Get_LEN_UARTnTx(PORTn);
							
							UARTCtrl->F10ReadDICount=0;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
							UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
							p8txbuff[26]=0;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
							while(UARTCtrl->F10ReadDICount<p8rxbuff[20+n+7])
							{
								i=UARTCtrl->F10ReadDICount;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
								DI=MRR(((u32)p8rxbuff)+20+n+8+(i*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
								if(UARTCtrl->SubsequentFrames==0)
								{
									Read_DL645_2007_TxData(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
								}
								else
								{
									Read_DL645_2007_TxData_SubsequentFrames(Get_ADDR_UARTnRx(ACSAMPLEPORT),DI,0xaaaaaaaaaaaa,UARTCtrl->SubsequentFrames);//DL645-2007¶ÁºóÐøÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·,ºóÐøÖ¡ºÅ
								}
								x=UARTCtrlRS485->Task;//Ô­ÈÎÎñ¼Ä´æ
								UARTCtrlRS485->Task=1;//1=½ÓÊÕÍê³É
							  if(FileDownload->File&0x04)//ÎÄ¼þ±êÖ¾(ÖÃÎ»±íÊ¾OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
							  {
									p32=(u32 *)(ADDR_Meter_ENTRY);
							    (*(void(*)())p32[130])((u32)ACSAMPLEPORT);//µ÷ÓÃvoid Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645½ÓÊÕ·¢ËÍ
								}
								UARTCtrlRS485->Task=x;//Ô­ÈÎÎñ»Ö¸´
								p8=(u8*)Get_ADDR_UARTnTx(ACSAMPLEPORT);
								if((p8[8]&0x40)!=0)
								{
									if(UARTCtrl->SubsequentFrames==0)
									{
										y=4;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(i*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
									}
									else
									{
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									}
								}
								else
								{
									y=p8[9];//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									for(x=0;x<y;x++)
									{//Êý¾ÝÓò¼õ0x33
										p8[10+x]-=0x33;
									}
									z=MRR(((u32)p8)+10,4);//·µ»ØµÄÊý¾Ý±êÊ¶
									if(z!=DI)
									{
										y=0;
									}
								}
								x=0;//ÒÑÓÐÊý¾Ý³¤¶È
								for(z=0;z<p8txbuff[26];z++)
								{
									x+=p8txbuff[27+x];
									x++;
								}
								if(UARTCtrl->SubsequentFrames==0)
								{//µÚ1Ö¡
									if((27+(x+1+y)+8+2)>Protocol)
									{
										UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										i++;
										if(i>=256)
										{
											break;
										}
										UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									}
									else
									{
										MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
										p8txbuff[27+x]=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+4
										if((p8[8]&0x20)&&(y!=0))
										{//ÓÐºóÐøÖ¡
											UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										}
										else
										{//ÎÞºóÐøÖ¡
											UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
											p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
											i++;
											if(i>=256)
											{
												break;
											}
											UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										}
									}
								}
								else
								{//ºóÐøÖ¡
									if(y>5)
									{//4×Ö½Ú±êÊ¶,1×Ö½Úseq
										y-=5;
									}
									else
									{
										y=0;
									}
									z=p8txbuff[27+x];//±¾±êÊ¶ÒÑ¶ÁÈë³¤¶È
									z=255-z;//»¹¿ÉÌî³¤¶È
									if(y>z)
									{
										y=z;
									}
									if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
									{
										UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										i++;
										if(i>=256)
										{
											break;
										}
										UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									}
									else
									{
										MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
										p8txbuff[27+x]+=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+4
										if((p8[8]&0x20)&&(y!=0))
										{//ÓÐºóÐøÖ¡
											UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										}
										else
										{//ÎÞºóÐøÖ¡
											UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
											p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
											i++;
											if(i>=256)
											{
												break;
											}
											UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										}
									}
								}
							}
							MWR(CommAddr,((u32)p8txbuff)+19,6);//×ª·¢Ä¿±êµØÖ·
							x=0;//ÒÑÓÐÊý¾Ý³¤¶È
							for(z=0;z<p8txbuff[26];z++)
							{
								x+=p8txbuff[27+x];
								x++;
							}
							if(x>(p8txbuff[26]*5))//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
							{
								p8txbuff[25]=5;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
							}
							else
							{
								p8txbuff[25]=2;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
							}
							if(Fn==9)
							{
								MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
								//x+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
								//x+=12;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾
								x+=20;
							}
							else
							{
								//x+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
								//x+=13;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾,1×Ö½ÚÊý¾Ý±êÊ¶¸öÊý
								x+=21;
							}
							p8txbuff[1]=x;
							p8txbuff[2]=x>>8;
							i=p8rxbuff[1]+(p8rxbuff[2]<<8);
							Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
							Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾Î»
							break;
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							if(p16timer[0]==0)//µÈ´ýÊý¾Ý×ª·¢¿Ú¿ÕÏÐ¶¨Ê±5S
							{
								goto err;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							if(UARTCtrlRS485->Lock!=0x55)
							{//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
								if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
								{//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									p16timer[0]=50/10;//»Ö¸´20msºó·¢ËÍ(¿¼ÂÇÌ¨Ìå²âÊÔÄ£Äâ±íËÙ¶ÈÏÖÓÃ50MS)
									UARTCtrlRS485->Lock=0x55;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
									UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±
									
									UARTCtrl->F10ReadDICount=0;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									p8txbuff[26]=0;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
								}
							}
							break;
#if ((Project/100)==2)//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
						case 31://¶Ë¿ÚºÅ=31,ÔØ²¨
							//¶Ë¿ÚºÅ=31,×ª·¢Ö»ÄÜ¶ÔDL645-1997»ò2007½øÐÐ
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							switch(UARTCtrl->SubTask)//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ¼ÆÊý,ÔÚÃ¿Ö¡½ÓÊÕOKÊ±³õÊ¼»¯Îª0)
							{
								case 0://µÈ´ý¿Ú¿Õ±ÕÊ±½øÈë
									p8txbuff[26]=0;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									
									n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//¶àÖ¡ÏÂ·¢ÊµÊ±³­¶Á»òÊý¾Ý×ª·¢:0=Ã¿Ö¡³­¶ÁÏìÓ¦,1=·ÅÆúÇ°ÃæÖ¡Ö»³­¶Á×îºóÖ¡²»¼ì²é×îºóÖ¡¿ÉÄÜÊÇÐÄÌøµÈµÄÈ·ÈÏÖ¡
									DMA_UARTn_RX_Pointer(PORTn);//¼ÆËãDMA_UARTn½ÓÊÕÖ¸ÕëÖµ(p0)
									if(p16[0]!=p16[1])
									{//ÒÑ½ÓÊÕµ½ÏÂÖ¡
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										goto F10err;
									}
								#endif
									if(p16timer[0]==0)//µÈ´ýÊý¾Ý×ª·¢¿Ú¿ÕÏÐ¶¨Ê±120S
									{
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										goto F10err;
									}
									if(Terminal_Router->RouterInitTask<200)//Â·ÓÉÆ÷³­¶ÁÈÎÎñ0-99¹«¹²³õÊ¼»¯,100-199·Ö³§¼Ò³õÊ¼»¯,>=200Â·ÓÉ³­±í
									{
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										goto F10err;
									}
									if(Terminal_Router->UpDateTask!=0)//Èí¼þ¸üÐÂÈÎÎñ
									{
										x=0;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										goto F10err;
									}
									if(UARTCtrlRS485->Lock!=0x55)
									{//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										if((UARTCtrlRS485->Task==0)||(UARTCtrlRS485->Task==2))
										{//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
											UARTCtrlRS485->Lock=0x55;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
											UARTCtrl->SubTask=1;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
										}
									}
									break;
								case 1://ÔÝÍ£Â·ÓÉ
									if(Terminal_Router->RouterCtrl!=0)//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									{
										break;
									}
									Terminal_Router->StopFlags|=(1<<2);//ÔÝÍ£Â·ÓÉ
									Terminal_Router->RouterCtrl=2;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrl->SubTask=2;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									break;
								case 2://ÔÝÍ£Â·ÓÉ½ÓÊÕ
									if(Terminal_Router->RouterCtrl==0)//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									{
										UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
										UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
										
										UARTCtrl->F10ReadDICount=0;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									}
									break;
								case 3://Â·ÓÉ×ª·¢Êý¾Ý
									n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p16=(u16 *)Get_ADDR_UARTn(PORTn);
								#if MulRealRead==1//¶àÖ¡ÏÂ·¢ÊµÊ±³­¶Á»òÊý¾Ý×ª·¢:0=Ã¿Ö¡³­¶ÁÏìÓ¦,1=·ÅÆúÇ°ÃæÖ¡Ö»³­¶Á×îºóÖ¡²»¼ì²é×îºóÖ¡¿ÉÄÜÊÇÐÄÌøµÈµÄÈ·ÈÏÖ¡
									DMA_UARTn_RX_Pointer(PORTn);//¼ÆËãDMA_UARTn½ÓÊÕÖ¸ÕëÖµ(p0)
									if((UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])||(p16[0]!=p16[1]))
									{//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý>=Ðè¶Á¸öÊý»òÓÐÊÕµ½ÏÂÖ¡
								#else
									if(UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])
									{//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý>=Ðè¶Á¸öÊý
								#endif
									F10End:
										Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
										Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
										
										x=0;//ÒÑÓÐÊý¾Ý³¤¶È
										for(z=0;z<p8txbuff[26];z++)
										{
											x+=p8txbuff[27+x];
											x++;
										}
										if(x>(p8txbuff[26]*5))//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										{
											x=5;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										}
										else
										{
											x=2;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										}
									F10err:
										UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
										UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										p8txbuff[25]=x;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
										
										n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
										if(n==0xff)
										{
											n=0;
										}
										n*=6;
										MR(((u32)p8txbuff)+19,((u32)p8rxbuff)+20+n,6);//×ª·¢Ä¿±êµØÖ·
										x=0;//ÒÑÓÐÊý¾Ý³¤¶È
										for(z=0;z<p8txbuff[26];z++)
										{
											x+=p8txbuff[27+x];
											x++;
										}
										if(Fn==9)
										{
											MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
											//x+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
											//x+=12;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾
											x+=20;
										}
										else
										{
											//x+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
											//x+=13;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾,1×Ö½ÚÊý¾Ý±êÊ¶¸öÊý
											x+=21;
										}
										p8txbuff[1]=x;
										p8txbuff[2]=x>>8;
										i=p8rxbuff[1]+(p8rxbuff[2]<<8);
										Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
										Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾»
										return;
									}
									p8=(u8 *)Get_ADDR_UARTnTx(PORTRS485);
									p8[0]=0x68;
									if(Terminal_Router->RouterRunMode_1==2)//Â·ÓÉÔËÐÐÄ£Ê½_Í¨ÐÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ"£¬ÆäËüÈ¡Öµ±£Áô
									{
										p8[3]=0x43;//¿ØÖÆÂë;¼¯ÖÐÊ½Â·ÓÉÔØ²¨Í¨ÐÅ
									}
									else
									{
										if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËÐÐÄ£Ê½_Í¨ÐÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ"£¬ÆäËüÈ¡Öµ±£Áô
										{
											p8[3]=0x4a;//¿ØÖÆÂë;Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ
										}
										else
										{
											p8[3]=0x41;//¿ØÖÆÂë;¼¯ÖÐÊ½Â·ÓÉÔØ²¨Í¨ÐÅ
										}
									}
									p8[4]=4;//Í¨ÐÅÄ£¿é±êÊ¶=1
									p8[5]=0;
									p8[6]=0;//Ô¤¼ÆÓ¦´ð×Ö½ÚÊý
									p8[7]=0;
									p8[8]=0;
									UARTCtrlRS485->TxSEQ++;//Æô¶¯Ö¡ÐòÁÐÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ýÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
									p8[9]=UARTCtrlRS485->TxSEQ;//Æô¶¯Ö¡ÐòÁÐÓòSEQ(Ã¿´Î¼Ó1ºó·¢ËÍ(·Ç·¢ËÍºó¼Ó1),µÈ´ýÏìÓ¦Ê±²»±ä,ÒÔ±ã±È½Ï)
									MR(((u32)p8)+10,(u32)&Terminal_Router->RouterAddr,6);//Ö÷½ÚµãµØÖ·(Ô´µØÖ·)
									MR(((u32)p8)+16,((u32)p8rxbuff)+20+n,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
									CommAddr=MRR(((u32)p8)+16,6);//Í¨ÐÅµØÖ·
									p8[22]=0x13;//AFN=13H
									p8[23]=1;//Fn=1
									p8[24]=0;
									i=p8rxbuff[20+n+6];//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ
									i++;
									p8[25]=i;
									DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
									p8[26]=0;//Í¨ÐÅÏà¹Ø±êÖ¾
									p8[27]=0;//¸½Êô½ÚµãÊý=0
									if(UARTCtrl->SubsequentFrames==0)
									{
										if(i==0x1)
										{//DL/T645-1997
											p8[28]=14;//DL645±¨ÎÄ³¤¶È
											Read_DL645_1997_TxData(((u32)p8)+29,DI,CommAddr);//DL645-1997¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
										}
										else
										{
											p8[28]=16;//DL645±¨ÎÄ³¤¶È
											Read_DL645_2007_TxData(((u32)p8)+29,DI,CommAddr);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
										}
									}
									else
									{
										if(i==0x1)
										{//DL/T645-1997
											p8[28]=14;//DL645±¨ÎÄ³¤¶È
											Read_DL645_1997_TxData_SubsequentFrames(((u32)p8)+29,DI,CommAddr);//DL645-1997¶ÁºóÐøÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
										}
										else
										{
											p8[28]=17;//DL645±¨ÎÄ³¤¶È
											Read_DL645_2007_TxData_SubsequentFrames(((u32)p8)+29,DI,CommAddr,UARTCtrl->SubsequentFrames);//DL645-2007¶ÁºóÐøÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·,ºóÐøÖ¡ºÅ
										}
									}
									i=31+p8[28];//Ðè×Ü·¢ËÍ×Ö½ÚÊý
									p8[1]=i;
									p8[2]=i>>8;
									if(Terminal_Router->RouterRunMode_1==3)//Â·ÓÉÔËÐÐÄ£Ê½_Í¨ÐÅ·½Ê½£º1±íÊ¾"Õ­´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬2±íÊ¾"¿í´øµçÁ¦ÏßÔØ²¨Í¨ÐÅ"£¬3±íÊ¾"Î¢¹¦ÂÊÎÞÏßÍ¨ÐÅ"£¬ÆäËüÈ¡Öµ±£Áô
									{//Î¢¹¦ÂÊÎÞÏß,µµ°¸µØÖ·ÓÃ²É¼¯Æ÷µØÖ·
										x=Check_AFN04F10_Addr(31,CommAddr);//¼ì²éÖÕ¶ËµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÅäÖÃµÄÍ¨ÐÅµØÖ·;·µ»Ø0ÎÞÏàÍ¬,!=0ÏàÍ¬µØÖ·µÄÅäÖÃÐòºÅ(1¿ªÊ¼)
										if(x!=0)
										{
											d64=MRR(ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn)+20,6);//²É¼¯Æ÷µØÖ·(Ä¿µÄµØÖ·)
											if((d64!=0xeeeeeeeeeeee)&&(d64!=0))
											{
												MWR(d64,((u32)p8)+16,6);//´Ó½ÚµãµØÖ·(Ä¿µÄµØÖ·)
											}
										}
									}
									Uart_3762_CS((u32)p8);//ºÍÐ£Ñé¼ÆËã²¢Ð´Èë(Í¬Ê±Ð´Èë½áÊø0x16)ºÍ±È½Ï,·µ»Ø0=ÕýÈ·1=´íÎó,Èë¿Ú½ÓÊÕ»ò·¢ËÍ»º³å
									UARTCtrlRS485->TxByte=i;//·¢ËÍ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
									UARTCtrlRS485->Task=3;
									UARTCtrlRS485->TxToRxTimerOver=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
									UARTCtrl->SubTask=4;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									if(Fn==9)
									{
										CopyString((u8 *)"ÔØ²¨F9  ",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
									}
									else
									{
										CopyString((u8 *)"ÔØ²¨F10 ",(u8*)ADDR_DATABUFF);//¿½±´×Ö·û´®;·µ»Ø¿½±´×Ö·û´®×Ö½ÚÊý(²»¼Æ½áÊø0)
									}
									for(i=0;i<6;i++)
									{
										x=p8rxbuff[(20+n+5)-i];
										MWR((x>>4)+0x30,ADDR_DATABUFF+8+(i*2),1);
										MWR((x&0xf)+0x30,ADDR_DATABUFF+8+(i*2)+1,1);
									}
									MWR(0,ADDR_DATABUFF+8+12,1);
									WarningOccur(PORTRS485,(u8 *)ADDR_DATABUFF);//¸æ¾¯·¢Éú,Èë¿Ú¸æ¾¯´úÂëºÍ¸æ¾¯×Ö·û
									break;
								case 4://×ª·¢½ÓÊÕ
									if(UARTCtrlRS485->Task>=3)//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
									{
										break;
									}
									n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									p8=(u8 *)Get_ADDR_UARTnRx(PORTRS485);
									i=p8[1]+(p8[2]<<8);
									if((UARTCtrlRS485->Task!=1)||(i<=16))
									{//³¬Ê±»ò·ñÈÏ»Ø´ð
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									}
									else
									{
										i=MRR(((u32)p8)+22,3);
										if(i!=0x113)
										{//AFN=13H,Fn=1
											UARTCtrlRS485->Task=5;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
											UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
											if(i==0x0200)
											{//AFN=00,Fn=2Ö±½Ó·ñÈÏÖ¡(Î¢¹¦ÂÊÎÞÏßÓÐ´ËÇé¿ö)
												p16[0]=6000/10;//6Ãë³¬Ê±Ê±¼ä
											}
											else
											{
												p16[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
											}
											return;
										}
										else
										{
											y=p8[28];//·µ»Ø645Ö¡×Ü³¤¶È
											if(y!=0)
											{
												for(i=0;i<y;i++)
												{//È¥Ç°µ¼0xfe
													if(p8[29+i]!=0xfe)
													{
														if(i!=0)
														{
															MR(((u32)p8)+29,((u32)p8)+29+i,y-i);
														}
														break;
													}
												}
												//¿ØÖÆÂë
												if((p8[29+8]&0x40)!=0x0)
												{//´ÓÕ¾Òì³£»Ø´ð
													y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
												}
												else
												{
													y=p8[29+9];//·µ»Ø645Ö¡Êý¾ÝÓò³¤¶È
													//CSÐ£Ñé
													z=0;
													for(i=0;i<(y+10);i++)
													{
														z+=p8[29+i];
													}
													z&=0xff;
													if(z!=p8[29+i])
													{
														y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
													}
													else
													{
													#if (USER/100)==11//¼ªÁÖ°æ
														//¼ªÁÖÊý¾ÝÓò²»¼õ0x33
														//Êý¾Ý±êÊ¶¼õ0x33
														for(i=0;i<4;i++)
														{
															p8[29+10+i]-=0x33;
														}
													#else
														//Êý¾ÝÓò¼õ0x33
														for(i=0;i<y;i++)
														{
															p8[29+10+i]-=0x33;
														}
													#endif
														//Ä¿µÄµØÖ·±È½Ï
									 					if(DataComp(((u32)p8rxbuff)+20+n,Get_ADDR_UARTnRx(PORTRS485)+29+1,6)!=0x0)//Êý¾Ý±È½Ï,·µ»Ø0=ÏàµÈ;µ±Byte>8Ê±·µ»Ø1=²»ÏàµÈ;µ±Byte<=8Ê±1=Data1>Data2,2=Data1<Data2
														{//²»Í¬
															UARTCtrlRS485->Task=5;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
															UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
															p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
															p16[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
															return;
														}
														//DI±È½Ï
														DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
														z=MRR(((u32)p8)+29+10,4);//·µ»ØµÄÊý¾Ý±êÊ¶
														i=p8rxbuff[20+n+6];//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ
														if(i==0)
														{//1997
															DI&=0xffff;
															z&=0xffff;
														}
														if(DI!=z)
														{
															if(z==0x04001501)
															{//ÊÇÊÕµ½Ö÷¶¯ÉÏ±¨Ö¡
//															#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
//																d64=MRR(((u32)p8rxbuff)+20+n,6);
//																Write_AutoReportWord(31,d64,((u32)p8)+29+14);//Ð´µçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;Èë¿Ú:¶Ë¿ÚºÅ(1-31),Í¨ÐÅµØÖ·,×Ô¶¯ÉÏ±¨×´Ì¬×ÖµØÖ·
//															#endif
																UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
																p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
																i=UARTCtrl->F10ReadDICount;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
																i++;
																if(i>=256)
																{
																	goto F10End;
																}
																UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
																UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
																UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
																return;
															}
															UARTCtrlRS485->Task=5;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ
															UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý(°ë×Ö¶ÔÆë)
															p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
															p16[0]=Terminal_Router->MaxOverTime*(1000/10);//´Ó½Úµã¼à¿Ø×î´ó³¬Ê±Ê±¼äÃë
															return;
														}
														else
														{
															y=p8[29+9];//·µ»ØÊý¾ÝÓò³¤¶È
															if(i==0)
															{//1997
																//2×Ö½Ú±êÊ¶±äÎª4×Ö½Ú
																if(y<2)
																{
																	y=0;
																}
																else
																{
																	y-=2;
																	MR(ADDR_DATABUFF,((u32)p8)+29+12,y);
																	MR(((u32)p8)+29+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý
																	MR(((u32)p8)+29+14,ADDR_DATABUFF,y);
																	y+=4;
																}
															}
														}
													}
												}
											}
										}
									}
									
									p8=(u8 *)(Get_ADDR_UARTnRx(PORTRS485)+29);
									if(y==0x0)
									{//Ã»ÊÕµ½Êý¾Ý
									#if MaxReRMCount!=0
										if(UARTCtrl->ReRMCount<MaxReRMCount)//ÖØ·¢¼ÆÊý
										{
											UARTCtrl->ReRMCount++;//ÖØ·¢¼ÆÊý
											UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
											return;
										}
									#endif
										y=4;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										p8[8]=0;//È¥ºóÐøÖ¡±êÖ¾
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
									}
									Protocol=Get_LEN_UARTnTx(PORTn);
									x=0;//ÒÑÓÐÊý¾Ý³¤¶È
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									i=UARTCtrl->F10ReadDICount;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									if(UARTCtrl->SubsequentFrames==0)
									{//µÚ1Ö¡
										if((27+(x+1+y)+8+2)>Protocol)
										{
											UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
											i++;
											if(i>=256)
											{
												goto F10End;
											}
											UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										}
										else
										{
											MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
											p8txbuff[27+x]=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+4
											if((p8[8]&0x20)&&(y!=0))
											{//ÓÐºóÐøÖ¡
												UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
											}
											else
											{//ÎÞºóÐøÖ¡
												UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
												p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
												i++;
												if(i>=256)
												{
													goto F10End;
												}
												UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
											}
										}
									}
									else
									{//ºóÐøÖ¡
										i=p8rxbuff[20+n+6];//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ
										if(i==0)
										{//1997
											if(y>4)
											{//4×Ö½Ú±êÊ¶
												y-=4;
											}
											else
											{
												y=0;
											}
										}
										else
										{//2007
											if(y>5)
											{//4×Ö½Ú±êÊ¶,1×Ö½Úseq
												y-=5;
											}
											else
											{
												y=0;
											}
										}
										z=p8txbuff[27+x];//±¾±êÊ¶ÒÑ¶ÁÈë³¤¶È
										z=255-z;//»¹¿ÉÌî³¤¶È
										if(y>z)
										{
											y=z;
										}
										if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
										{
											UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
											i++;
											if(i>=256)
											{
												goto F10End;
											}
											UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										}
										else
										{
											MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
											p8txbuff[27+x]+=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+4
											if((p8[8]&0x20)&&(y!=0))
											{//ÓÐºóÐøÖ¡
												UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
											}
											else
											{//ÎÞºóÐøÖ¡
												UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
												p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
												i++;
												if(i>=256)
												{
													goto F10End;
												}
												UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
											}
										}
									}
									UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
									UARTCtrl->SubTask=3;//×ÓÈÎÎñ(ÁÙÊ±²Ù×÷Â·ÓÉÄ£¿éÈÎÎñ)
									break;
								default:
									Terminal_Router->StopFlags&=~(1<<2);//»Ö¸´Â·ÓÉ
									Terminal_Router->RouterCtrl=3;//Â·ÓÉÆ÷¿ØÖÆ0=¿Õ,1=ÖØÆô,2=ÔÝÍ£,3=»Ö¸´,4=¶Á×´Ì¬;Íê³Éºó×Ô¶¯»Øµ½0
									UARTCtrl->Task=0;
									UARTCtrlRS485->Lock=0;//0x55=Í¨ÐÅ¹¦ÄÜÕýÔÚ±»µ÷ÓÃ½ûÖ¹ÆäËû³ÌÐòÔÙ´Îµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									break;
							}
							break;
#endif
					}
					break;
				case 9://9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±(Ô­×÷Îª³­±í¿ÚÊ±ÖÃÎªµÄÖµ£©
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//µÃµ½UART¶¨Ê±Æ÷µØÖ·
							if(p16[0]==0x0)
							{
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								n=p8rxbuff[19];//×ª·¢ÖÐ¼Ì¼¶Êýn
								if(n==0xff)
								{
									n=0;//ÎÞÖ¸¶¨ÖÐ¼ÌÂ·ÓÉ
								}
								n*=6;//ÖÐ¼Ì¼¶ÊýºÍµØÖ·×Ö½Ú³¤¶È
								
								p16=(u16 *)Get_ADDR_UARTn(PORTn);
							#if MulRealRead==1//¶àÖ¡ÏÂ·¢ÊµÊ±³­¶Á»òÊý¾Ý×ª·¢:0=Ã¿Ö¡³­¶ÁÏìÓ¦,1=·ÅÆúÇ°ÃæÖ¡Ö»³­¶Á×îºóÖ¡²»¼ì²é×îºóÖ¡¿ÉÄÜÊÇÐÄÌøµÈµÄÈ·ÈÏÖ¡
								DMA_UARTn_RX_Pointer(PORTn);//¼ÆËãDMA_UARTn½ÓÊÕÖ¸ÕëÖµ(p0)
								if((UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])||(p16[0]!=p16[1]))
								{//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý>=Ðè¶Á¸öÊý»òÓÐÊÕµ½ÏÂÖ¡
							#else
								if(UARTCtrl->F10ReadDICount>=p8rxbuff[20+n+7])
								{//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý>=Ðè¶Á¸öÊý
							#endif
								F10End_2:
									x=0;//ÒÑÓÐÊý¾Ý³¤¶È
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									if(x>(p8txbuff[26]*5))//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									{
										x=5;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
									}
									else
									{
										x=2;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
									}
								//F10err_2:
									p8txbuff[25]=x;//×ª·¢½á¹û±êÖ¾:0=²»ÄÜÖ´ÐÐ×ª·¢,1=×ª·¢½ÓÊÕ³¬Ê±,2=×ª·¢½ÓÊÕ´íÎó,3=×ª·¢½ÓÊÕÈ·ÈÏ,4=×ª·¢½ÓÊÕ·ñÈÏ,5=×ª·¢½ÓÊÕÊý¾Ý,ÆäËûÖµÎÞÐ§
									UARTCtrlRS485->Lock=0;//½â³ýµ÷ÓÃ
									UARTCtrlRS485->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									UARTCtrlRS485->RxByte=0;//Çå½ÓÊÕ×Ö½ÚÊý¼Ä´æ
									UARTCtrlRS485->Task=0;//0=¿ÕÏÐ,1=Íê³É1Ö¡·¢ÊÕ,2=½ÓÊÕ³¬Ê±(»ò´íÎó),3=Æô¶¯·¢ËÍ,4=ÕýÔÚ·¢ËÍ,5=ÕýÔÚ½ÓÊÕ


									n=p8rxbuff[19];//È¡×ª·¢ÖÐ¼Ì¼¶Êý
									if(n==0xff)
									{
										n=0;
									}
									n*=6;
									MR(((u32)p8txbuff)+19,((u32)p8rxbuff)+20+n,6);//×ª·¢Ä¿±êµØÖ·
									x=0;//ÒÑÓÐÊý¾Ý³¤¶È
									for(z=0;z<p8txbuff[26];z++)
									{
										x+=p8txbuff[27+x];
										x++;
									}
									if(Fn==9)
									{
										MR(((u32)p8txbuff)+26,((u32)p8txbuff)+27,x);
										//x+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
										//x+=12;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾
										x+=20;
									}
									else
									{
										//x+=8;//1×Ö½Ú¿ØÖÆÓò+5×Ö½ÚµØÖ·Óò+1×Ö½ÚAFN+1×Ö½ÚSEQ=8×Ö½Ú
										//x+=13;//4×Ö½Ú±êÊ¶,1×Ö½Ú¶Ë¿ÚºÅ,6×Ö½Ú×ª·¢Ä¿±êµØÖ·,1×Ö½Ú×ª·¢½á¹û±êÖ¾,1×Ö½ÚÊý¾Ý±êÊ¶¸öÊý
										x+=21;
									}
									p8txbuff[1]=x;
									p8txbuff[2]=x>>8;
									i=p8rxbuff[1]+(p8rxbuff[2]<<8);
									Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+i-6);//¸½¼ÓÐÅÏ¢
									Uart_376_CS((u32)p8txbuff);//ºÍÐ£Ñé¼ÆËã,Í¬Ê±¼Ó½áÊø0x16;ÐÞ¸Ä³¤¶È×Ö½ÚµÄ±êÖ¾»
									return;
								}
								CommAddr=MRR(((u32)p8rxbuff)+20+n,6);//×ª·¢Ä¿±êµØÖ·
								DI=MRR(((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
								i=p8rxbuff[20+n+6];//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶ÀàÐÍ
								i&=3;
								MC(0xfe,Get_ADDR_UARTnTx(PORTRS485),4);
								if(UARTCtrl->SubsequentFrames==0)
								{//µÚ1Ö¡
									if(i==0)
									{//0£º·ûºÏDL/T645-1997
										UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//Ä¬ÈÏ1200
										Read_DL645_1997_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
										UARTCtrlRS485->TxByte=4+14;//·¢ËÍ×Ö½ÚÊý
									}
									else
									{//1£º·ûºÏDL/T645-2007
										UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//Ä¬ÈÏ2400
										Read_DL645_2007_TxData(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
										UARTCtrlRS485->TxByte=4+16;//·¢ËÍ×Ö½ÚÊý
									}
								}
								else
								{//ºóÐøÖ¡
									if(i==0)
									{//0£º·ûºÏDL/T645-1997
										UARTCtrlRS485->BpsCtrl=(2<<5)+0x0b;//Ä¬ÈÏ1200
										Read_DL645_1997_TxData_SubsequentFrames(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
										UARTCtrlRS485->TxByte=4+14;//·¢ËÍ×Ö½ÚÊý
									}
									else
									{//1£º·ûºÏDL/T645-2007
										UARTCtrlRS485->BpsCtrl=(3<<5)+0x0b;//Ä¬ÈÏ2400
										Read_DL645_2007_TxData_SubsequentFrames(Get_ADDR_UARTnTx(PORTRS485)+4,DI,CommAddr,UARTCtrl->SubsequentFrames);//DL645-2007¶ÁÊý¾ÝÖ¡,Èë¿Ú:ÒªÌîÈëµÄ»º³åµØÖ·,Êý¾Ý±êÊ¶,Í¨ÐÅµØÖ·
										UARTCtrlRS485->TxByte=4+17;//·¢ËÍ×Ö½ÚÊý
									}
								}
								i=Check_AFN04F10_Addr(p8rxbuff[18],CommAddr);//¼ì²éÖÕ¶ËµçÄÜ±í/½»Á÷²ÉÑù×°ÖÃÅäÖÃµÄÍ¨ÐÅµØÖ·;·µ»Ø0ÎÞÏàÍ¬,!=0ÏàÍ¬µØÖ·µÄÅäÖÃÐòºÅ(1¿ªÊ¼)
								if(i!=0)
								{//ÕÒµ½
									//Í¨ÐÅËÙÂÊ
									i=MRR(ADDR_AFN04F10+((i-1)*LEN_AFN04F10_Pn)+4,1);
									if((i&0xe0)!=0x0)
									{//²»ÊÇÄ¬ÈÏ
										UARTCtrlRS485->BpsCtrl=(i&0xe0)+0x0b;
									}
								}
								Init_UARTn(PORTRS485);//³õÊ¼»¯UART¿Ú,Èë¿ÚÍ¨ÐÅ¿ØÖÆ×ÖÒÑÌîÈë
								Start_UARTn_Tx(PORTRS485);//Æô¶¯UART¿Ú·¢ËÍ
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485³­±í·¢ËÍLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
					#endif
								UARTCtrl->Task=10;//10=ÕýÔÚÍ¨¹ýÆäËü¿Ú·¢ËÍÊý¾Ý
							}
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 10://10=ÕýÔÚÍ¨¹ýÆäËü¿Ú·¢ËÍÊý¾Ý
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							if(Wait_UARTn_Tx(PORTRS485)==0)//µÈ´ýUART¿Ú·¢ËÍ,·µ»Ø0=·¢ËÍÍê³É,1=·¢ËÍÃ»Íê³É
							{
								p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
								p16[0]=ReadDL645OverTimer/10;//×ª·¢½ÓÊÕµÈ´ý±¨ÎÄ³¬Ê±Ê±¼ä
								UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
								UARTCtrlRS485->RxByte=0;//½ÓÊÕ×Ö½ÚÊý
								UARTCtrl->Task=11;//11=ÕýÔÚÍ¨¹ýÆäËü¿Ú½ÓÊÕÊý¾Ý
							}	
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
				case 11://11=ÕýÔÚÍ¨¹ýÆäËü¿Ú½ÓÊÕÊý¾Ý
					switch(p8rxbuff[18])
					{
						case 2://¶Ë¿ÚºÅ=2,RS485-1
						case 3://¶Ë¿ÚºÅ=3,RS485-2
						case 4://¶Ë¿ÚºÅ=4,RS485-3
							i=DL645_Rx(PORTRS485);//DL645Ö¡½ÓÊÕ;·µ»Ø0Ã»Íê³É,1Íê³É,2³¬Ê±
							if(i==0)
							{
								break;
							}
							UARTCtrlRS485=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTRS485);
							n=p8rxbuff[19];//×ª·¢ÖÐ¼Ì¼¶Êýn
							if(n==0xff)
							{
								n=0;//ÎÞÖ¸¶¨ÖÐ¼ÌÂ·ÓÉ
							}
							n*=6;//ÖÐ¼Ì¼¶ÊýºÍµØÖ·×Ö½Ú³¤¶È
							
							Protocol=p8rxbuff[20+n+6];//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶ÀàÐÍ
							Protocol&=3;//0=·ûºÏDL/T645-1997,1=·ûºÏDL/T645-2007
							if(i==2)
							{//2=³¬Ê±
								if(UARTCtrl->ReRMCount<2)//ÖØ·¢¼ÆÊý
								{
									UARTCtrl->ReRMCount++;//ÖØ·¢¼ÆÊý
									UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±
									break;
								}
							}
							if(i==1)
							{//1Íê³É
								if(Protocol==0)
								{//0£º·ûºÏDL/T645-1997
									x=DL645_1997_RxData(PORTRS485);//DL645_1997Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í
								}
								else
								{//1£º·ûºÏDL/T645-2007
									x=DL645_2007_RxData(PORTRS485);//DL645_2007Ö¡½ÓÊÕÊý¾Ý,±È½Ï·¢ÊÕµØÖ·¡¢¿ØÖÆÂë¡¢Êý¾Ý±êÊ¶¡¢¼õ0x33;·µ»Ø0ÕýÈ·,1µØÖ·´í,2¿ØÖÆÂë´í,3Êý¾Ý±êÊ¶´í
								}
								if(x!=0)
								{
									if(x==3)
									{//3=Êý¾Ý±êÊ¶´í
										p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
										//¼õ0x33
										for(z=0;z<p8[9];z++)
										{
											p8[10+z]-=0x33;
										}
										z=MRR(((u32)p8)+10,4);//·µ»ØµÄÊý¾Ý±êÊ¶
										if(z==0x04001501)//if(z==0x37334834)
										{//ÊÇÊÕµ½Ö÷¶¯ÉÏ±¨Ö¡
//										#if EventProject==1//ÊÂ¼þ¼ÇÂ¼²É¼¯·½°¸:0=Ô­ÄÜ×ßÌ¨Ìå±ê×¼,1=°´ÐÂÖÇÄÜµçÄÜ±í¼°²É¼¯ÖÕ¶ËÊÂ¼þ¼ÇÂ¼²É¼¯¹æÔò
//											d64=MRR(((u32)p8)+1,6);
//											Write_AutoReportWord(p8rxbuff[18],d64,((u32)p8)+14);//Ð´µçÄÜ±íÖ÷¶¯ÉÏ±¨×´Ì¬×Ö;Èë¿Ú:¶Ë¿ÚºÅ(1-31),Í¨ÐÅµØÖ·,×Ô¶¯ÉÏ±¨×´Ì¬×ÖµØÖ·
//										#endif
											UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
											p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
											i=UARTCtrl->F10ReadDICount;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
											i++;
											if(i>=256)
											{
												goto F10End_2;
											}
											UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
											UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
											p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//µÃµ½UART¶¨Ê±Æ÷µØÖ·
											p16[0]=0;
											UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±(Ô­×÷Îª³­±í¿ÚÊ±ÖÃÎªµÄÖµ£©
											return;
										}
									}
									if(x!=2)
									{
										//µØÖ·»òÊý¾Ý±êÊ¶´í,µç¿ÆÔº²âÊÔÈí¼þÄ£Äâ±í»áÍ£Ö¹µ«½ÓÊÕÊý¾ÝÕý³£,»Ö¸´Ä£Äâ±íºó»á½«ÊÕµ½µÄÖ¡Í¬Ê±·¢³ö
										//¼ÌÐø½ÓÊÕ
										p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);
										p16[0]=ReadDL645OverTimer/10;//DL645¶Á±í³¬Ê±¶¨Ê±Öµms
										UARTCtrlRS485->RxByte=0;//Çå½ÓÊÕ×Ö½ÚÊý¼Ä´æ
										break;
									}
								}
							}
							p8=(u8*)Get_ADDR_UARTnRx(PORTRS485);
							if(i!=1)
							{
								y=4;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
								p8[8]=0;//È¥ºóÐøÖ¡±êÖ¾
								MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý
							}
							else
							{
					#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//·½°¸0=ÖÇÄÜ±í,100=ÍøÂç±í,200=¼¯ÖÐÆ÷,300=×¨±äÖÕ¶Ë,400=¹«±äÖÕ¶Ë,500=¼¯ÖÐÆ÷¢òÐÍ,600=Í¨ÐÅÄ£¿é
								if(PORTRS485==RS485_1PORT)
								{
									Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
								if(PORTRS485==RS485_2PORT)
								{
									Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485³­±í½ÓÊÕLEDÖ¸Ê¾MS¶¨Ê±Æ÷,Ã¿10MS¼õ1¶¨Ê±Æ÷
								}
					#endif
								if((p8[8]&0x40)!=0)
								{
									if(UARTCtrl->SubsequentFrames==0)
									{
										y=4;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
										MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶
									}
									else
									{
										y=0;//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									}
								}
								else
								{
									y=p8[9];//×ª·¢½ÓÊÕÊý¾Ý³¤¶È
									if(Protocol==0)
									{//1997
										//2×Ö½Ú±êÊ¶±äÎª4×Ö½Ú
										if(y<2)
										{
											y=0;
										}
										else
										{
											y-=2;
											MR(ADDR_DATABUFF,((u32)p8)+12,y);
											MR(((u32)p8)+10,((u32)p8rxbuff)+20+n+8+(UARTCtrl->F10ReadDICount*4),4);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý
											MR(((u32)p8)+14,ADDR_DATABUFF,y);
											y+=4;
										}
									}
								}
							}
							Protocol=Get_LEN_UARTnTx(PORTn);
							x=0;//ÒÑÓÐÊý¾Ý³¤¶È
							for(z=0;z<p8txbuff[26];z++)
							{
								x+=p8txbuff[27+x];
								x++;
							}
							i=UARTCtrl->F10ReadDICount;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
							if(UARTCtrl->SubsequentFrames==0)
							{//µÚ1Ö¡
								if((27+(x+1+y)+8+2)>Protocol)
								{
									UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									i++;
									if(i>=256)
									{
										goto F10End_2;
									}
									UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
								}
								else
								{
									MR(((u32)p8txbuff)+28+x,((u32)p8)+10,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
									p8txbuff[27+x]=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+4
									if((p8[8]&0x20)&&(y!=0))
									{//ÓÐºóÐøÖ¡
										UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									}
									else
									{//ÎÞºóÐøÖ¡
										UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										i++;
										if(i>=256)
										{
											goto F10End_2;
										}
										UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									}
								}
							}
							else
							{//ºóÐøÖ¡
								i=p8rxbuff[20+n+6];//×ª·¢Êý¾Ý±êÊ¶ÀàÐÍ
								if(i==0)
								{//1997
									if(y>4)
									{//4×Ö½Ú±êÊ¶
										y-=4;
									}
									else
									{
										y=0;
									}
								}
								else
								{//2007
									if(y>5)
									{//4×Ö½Ú±êÊ¶,1×Ö½Úseq
										y-=5;
									}
									else
									{
										y=0;
									}
								}
								z=p8txbuff[27+x];//±¾±êÊ¶ÒÑ¶ÁÈë³¤¶È
								z=255-z;//»¹¿ÉÌî³¤¶È
								if(y>z)
								{
									y=z;
								}
								if((27+(x+p8txbuff[27+x]+y)+8+2)>Protocol)
								{
									UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									i++;
									if(i>=256)
									{
										goto F10End_2;
									}
									UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
								}
								else
								{
									MR(((u32)p8txbuff)+28+x+p8txbuff[27+x],((u32)p8)+14,y);//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ
									p8txbuff[27+x]+=y;//×ª·¢Ö±½Ó³­¶ÁµÄÊý¾ÝÄÚÈÝ×Ö½ÚÊýk+4
									if((p8[8]&0x20)&&(y!=0))
									{//ÓÐºóÐøÖ¡
										UARTCtrl->SubsequentFrames++;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
									}
									else
									{//ÎÞºóÐøÖ¡
										UARTCtrl->SubsequentFrames=0;//0=ÎÞ¶ÁºóÐøÖ¡,1-255=DL645¶ÁÐøÖ¡µÄÖ¡ºÅ
										p8txbuff[26]++;//·¢ËÍÇø×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
										i++;
										if(i>=256)
										{
											goto F10End_2;
										}
										UARTCtrl->F10ReadDICount=i;//F10ÒÑ×ª·¢Ö±½Ó³­¶ÁµÄÊý¾Ý±êÊ¶¸öÊý
									}
								}
							}
							UARTCtrl->ReRMCount=0;//ÖØ·¢¼ÆÊý
							p16=(u16 *)Get_ADDR_UARTnTimer(PORTRS485);//µÃµ½UART¶¨Ê±Æ÷µØÖ·
							p16[0]=0;
							UARTCtrl->Task=9;//9=µÈ´ýÏÂÖ¡Æô¶¯·¢ËÍÑÓÊ±(Ô­×÷Îª³­±í¿ÚÊ±ÖÃÎªµÄÖµ£©
							break;
						default:
							UARTCtrl->Task=0;
							break;
					}
					break;
			}
			break;
		default:
			UARTCtrl->Task=0;
			break;
	}
}












