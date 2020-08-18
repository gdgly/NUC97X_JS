
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayLoop_Warning.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


u32 DisplayWarningString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示告警字符串
{
  u32 i;
  u32 x;
  u32 y;
  u32 Row;//字符串号
  u16 * pStrBuff;
  
	if(Ln>9)
	{
		return (Ln+(Col<<8)+(0<<16));
	}
	
  i=0;
  Row=1;//菜单条号
  if(Col>=20)
  {
    Col=AutoIndent;
    Ln++;
  }
  if(Ln>=10)
  {
    return (Ln+(Col<<8)+(Row<<16));
  }
  pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
  while(1)
  {
    if(pStr[i]==0)
    {//结束
      i++;
      return (Ln+(Col<<8)+(Row<<16));
    }
    if(pStr[i]=='\n')
    {//换行
      i++;
      Ln++;
      if(Ln>=10)
      {
        return (Ln+(Col<<8)+(Row<<16));
      }
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      Row++;//字符串号
      continue;
    }
    if(pStr[i]=='\r')
    {//回车
      i++;
      Col=0;
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      continue;
    }
    x=pStr[i];
    i++;
    if(x>=0xA1)
    {//码>=0xA1A1国标库
      y=pStr[i];
      i++;
      y-=0xA1;
			if(x>=0xB0)
			{
				x-=0x06;
			}
			x-=0xA1;
			x*=94;
			x+=y;
			x<<=1;
			x+=0x4000;
      pStrBuff[0]=x;
      //StrBuff->Row[Ln].Row=Row;//字符串号
      Col++;
			if(Col>=20)
			{//换行
        pStrBuff[0]=0;
				Col=AutoIndent;
        Ln++;
        if(Ln>=10)
        {
          i--;
          return (Ln+(Col<<8)+(Row<<16));
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
        pStrBuff[0]=x;
			}
			x+=1;
			pStrBuff[1]=x;
      //StrBuff->Row[Ln].Row=Row;//字符串号
      pStrBuff+=2;
      Col++;
			if(Col>=20)
			{//换行
				Col=AutoIndent;
        Ln++;
        if(Ln>=10)
        {
          return (Ln+(Col<<8)+(Row<<16));
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
			}
    }
    else
    {
			if(x<=3)
			{
				//自定义库
				pStrBuff[0]=x+34;
			}
			else
			{
      	//ASCII字库
      	pStrBuff[0]=x+0x2000;
			}
      //StrBuff->Row[Ln].Row=Row;//字符串号
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//换行
				Col=AutoIndent;
        Ln++;
        if(Ln>=10)
        {
          return (Ln+(Col<<8)+(Row<<16));
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
	  	}
    }
  }
}

u32 DisplayLoop_Warning(u32 Row)//告警信息显示,返回下行
{
	u32 i;
	u16 *p16;
//	UARTCtrl_TypeDef * UARTCtrl;

	if(TerminalDisplayLoop->Count==0)
	{
		//提示
		DisplayString(Row,0,0,(u8 *)"提示");//显示字符串
		DispalyAlign(Row,0,2);//行显示对齐(1左对齐,2居中,3右对齐)
		Row++;
		//
		p16=(u16 *)(ADDR_WarningStringBuff);
		for(i=0;i<MaxWarningString;i++)
		{
			if(Row>8)
			{
				break;
			}
			if(p16[0])
			{
				//DisplayString(Row,0,0,(u8 *)(((u32)p16)+4));//显示字符串
				DisplayWarningString(Row,0,0,(u8 *)(((u32)p16)+4));//显示告警字符串
				Row++;
			}
			p16+=84/2;
		}

//调试显示
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//	i=UARTCtrl->DateReRMCount;
//	DisplayData_OffZero(Comm_Ram->TEMP,4,1,(u16 *)(ADDR_STRINGBUFF+(8*84)+4));//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
//	DisplayRam(9,ADDR_RUNSPEEDTEST+2);//显示中RAM数据(调试用) 
//		Row++;
	}
	return Row;
}



		
		




