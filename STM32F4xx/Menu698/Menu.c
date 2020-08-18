

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "Menu.h"
#include "../Display/Display.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../DL698/DL698_DataDef.h"
#include "../MS/MS.h"
#include "../STM32F4xx/STM32F4xx_UART.h"



#if (USER/100)==17 //江苏
void myprintf(char *pStr,u32 param1,u32 param2,u32 param3)
{
#ifdef DEBUG_MENU
	ms_Type *ms;
	ms=Get_ms();
	
	ms->link_printf((char *)pStr,param1,param2,param3);
#endif
}

void GetACSample(u32 DI)//得到内部交采数据,入口通信数据标识,返回645帧格式在通信口缓冲的发送区
{
	u32 i;
	u32 Task;
	u32 *p32;
  UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rxbuff;
	u8 * p8txbuff;
	
  UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(ACSAMPLEPORT);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(ACSAMPLEPORT);
	Task=UARTCtrl->Task;//原任务寄存
	UARTCtrl->Task=1;//1=接收完成
	UARTCtrl->Lock=0;//0x55=通信功能正在被调用禁止其他程序再次调用
	MWR(DI,((u32)p8rxbuff)+10,4);
	//加0x33
	for(i=0;i<4;i++)
	{
		p8rxbuff[10+i]+=0x33;
	}
	//通信地址
	MC(0xaa,((u32)p8rxbuff)+1,6);
	p8rxbuff[8]=0x11;//读命令
	p8rxbuff[9]=0x4;
	/*
	myprintf("[%s:%d]:交采电压发送\n",(u32)__func__,(u32)__LINE__,0);
	for(i=0;i<50;i++)
	{	
		myprintf("%02x ",p8rxbuff[i],0,0);
	}
	myprintf("\n",0,0,0);
	*/
#if ((Project/100)<5)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  if(FileDownload->File&0x4)//文件标志(置位表示OK)::B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=,B7=Router,...B31=
  {
		p32=(u32 *)(ADDR_Meter_ENTRY);
    	(*(void(*)())p32[130])((u32)ACSAMPLEPORT);//调用void Meter07_DL645_RxTx(u32 PORTn)//Meter07_DL645接收发送
		//减33H
		for(i=0;i<p8txbuff[9];i++)
		{
			p8txbuff[10+i]-=0x33;
		}
		/*
		myprintf("[%s:%d]:交采电压接受\n",(u32)__func__,(u32)__LINE__,0);
		for(i=0;i<50;i++)
		{	
			myprintf("%02x ",p8txbuff[i],0,0);
		}
		myprintf("\n",0,0,0);
		*/
	}
	else
	{
		MC(0,Get_ADDR_UARTnTx(ACSAMPLEPORT),Get_LEN_UARTnTx(ACSAMPLEPORT));
	}
#else//#if ((Project/100)<5)
	MC(0,Get_ADDR_UARTnTx(ACSAMPLEPORT),Get_LEN_UARTnTx(ACSAMPLEPORT));
#endif//#if ((Project/100)<5)
	UARTCtrl->Task=Task;//原任务恢复
}

void NextScreenSelect(u32 MaxScrnNum)//按键翻屏操作选择,MaxScrnNum最大有多少个屏幕,Stack->MenuStack[MenuCount->Count].MoveRow从0开始算
{
	u32 i;
	if(MaxScrnNum<1)//屏幕至少要有一个
	{
		return ;
	}
	MaxScrnNum--;//第几个屏幕序号从0开始计数
	if(Comm_Ram->ButtonVal&0x01)//下移
	{
		i=Stack->MenuStack[MenuCount->Count].MoveRow;
		if(i==MaxScrnNum)//这边的数字大小取决于下面SWITCH(CASE)的大小
		{
			i=0;
		}else
		{
			i++;//记录下换哪个屏幕
		}
		Stack->MenuStack[MenuCount->Count].MoveRow=i;
		Comm_Ram->ButtonVal&=~0x03;//清楚键位
	}
	
	if(Comm_Ram->ButtonVal&0x02)//上移
	{
		i=Stack->MenuStack[MenuCount->Count].MoveRow;
		if(i==0)//第一个屏幕
		{
			i=MaxScrnNum;
		}else
		{
			i--;
		}
		Stack->MenuStack[MenuCount->Count].MoveRow=i;
		Comm_Ram->ButtonVal&=~0x03;//清楚键位
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       LCD_CmpString
// Descriptions:        比较转换之后的字符
// input parameters:    pDest源内存中已经转换过数据,pSrc原始国标码,Len比较的长度(按照字符个数计算)
// output parameters:  	无
// Returned value:      成功0 失败1
//-------------------------------------------------------------------------------------------------
u32 LCD_CmpString(u16 *pDest,u8 *pSrc,u32 Len)//内存中的数据与国标码比较 成功0 失败1 pDest是液晶BUf里面的数据
{
	u16 x;
	u16 y;
	//是不是ASCII码
	while(Len>0)
	{
		Len--;
		if(*pSrc>=0xA1)
		{
			x=*pSrc;
			pSrc++;
			if(x>=0xB0)
			{
				x-=6;
			}
			x-=0xA1;
			x*=94;
			y=*pSrc;
			y-=0xA1;
			x+=y;
			x<<=1;
			x+=0x4000;
			pSrc++;
			if(*pDest!=x)
			{
				return 1;
			}
			pDest+=2;
		}else
		{
			x=*pSrc;
			x+=0x2000;
			pSrc++;
			if(*pDest!=x)
			{
				return 1;
			}
			pDest++;
		}
	}
	return 0;
}

#endif


#if ((Project/100)==3 && (USER/100)==5)//上海专变

//-------------------------------------------------------------------------------------------------
// Function name:       NdigitSelect_SH
// Descriptions:        N位数左右选择
// input parameters:    N位数，编辑框列号Col，可选范围最小数Min，可选范围最大数Max
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void NdigitSelect_SH(u32 Ln,u32 N,u32 Col,u32 Min,u32 Max)
{
	u8 *p8;
	u32 x;
	u32 i;

	for(i=0;i<2;i++)
	{
		if(Stack->MenuStack[MenuCount->Count].EditCol==0)
		{
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右键
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				x=p8_bcd(p8,N);
				if(x<Max)
				{
					p8_add1(p8,N);
				}
				else
				{
					bcd_p8(p8,Min,N);
				}
				MenuCount->v2=p8_bcd(p8,N);
				Stack->MenuStack[MenuCount->Count].Task=0;
				MenuCount->v1=0;
			}
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左键
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				x=p8_bcd(p8,N);
				if(x>Min)
				{
					p8_sub1(p8,N);
				}
				else
				{
					bcd_p8(p8,Max,N);
				}
				MenuCount->v2=p8_bcd(p8,N);
				Stack->MenuStack[MenuCount->Count].Task=0;
				MenuCount->v1=0;
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				Stack->MenuStack[MenuCount->Count].EditCol=Col;
				DisplayString(Ln,16,0,"输入");
				Comm_Ram->ButtonVal&=~0x1C;//键使用后清0
			}
			else
			{
				Comm_Ram->ButtonVal&=~0x0C;//键使用后清0
				break;
			}
		}
		else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
		{
			Select_SH(Ln,Col+N-1,Col);
			UpDownSelect_SH(Ln);
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				x=p8_bcd(p8,N);
				if(x>Max || x<Min)
				{
					bcd_p8(p8,Min,N);
				}
				MenuCount->v2=p8_bcd(p8,N);
				Stack->MenuStack[MenuCount->Count].Task=0;
				MenuCount->v1=0;
				Stack->MenuStack[MenuCount->Count].EditCol=0;
				Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
			}
			else
			{
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				MenuCount->v2=p8_bcd(p8,N);
				MenuCount->v1=0;
				Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
				break;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Select_SH
// Descriptions:        十进制数左右选择
// input parameters:    字符串右边界，字符串左边界
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120213
// Date Modified：      20120301
//-------------------------------------------------------------------------------------------------
void Select_SH(u32 Ln,u32 MaxCol,u32 MinCol)
{
	u16 *p16;
	u32 x;
	u32 i;

	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//右移键按下
		Stack->MenuStack[MenuCount->Count].EditCol++;
		for(i=0;i<2;i++)
		{
			if(Stack->MenuStack[MenuCount->Count].EditCol==MaxCol+1)
			{
				Stack->MenuStack[MenuCount->Count].EditCol=MinCol;
			}    
			p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
			memcpy(&x,p16,2);
			x&=0x00ff;
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//只有数字、>能选中，其他符号全部跳过
			{
				Stack->MenuStack[MenuCount->Count].EditCol ++;
			}	
			else
			{
				break;
			}	
		}
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)
	{//左移键按下
		Stack->MenuStack[MenuCount->Count].EditCol--;
		for(i=0;i<2;i++)
		{
			if(Stack->MenuStack[MenuCount->Count].EditCol==MinCol-1)
			{
				Stack->MenuStack[MenuCount->Count].EditCol=MaxCol;
			}	
			p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
			memcpy(&x,p16,2);
			x&=0x00ff;
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//只有数字、>能选中，其他符号全部跳过
			{
				Stack->MenuStack[MenuCount->Count].EditCol --;
			}
			else
			{
				break;
			}	
		}
	}
	NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);
}
//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelect_SH
// Descriptions:        十进制数上下选择
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void UpDownSelect_SH(u32 Ln)//十进制数上下选择
{
	u16 *p16;
	u32 x;

	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//上移键按下
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		memcpy(&x,p16,2);
		x&=0x00ff;
		if(x>=0x30 && x<0x39)
		{
			x++;
		}
		else
		{
			if(x==0x39)
			{
				x=0x30;
			}
		}
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//显示字符串
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//下移键按下
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		memcpy(&x,p16,2);
		x&=0x00ff;
		if(x>0x30 && x<=0x39)
		{
			x--;
		}
		else
		{
			if(x==0x30)
			{
				x=0x39;
			}
		}
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//显示字符串
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenuString_SH
// Descriptions:        上海专变用显示菜单字符串
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString_SH(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示菜单字符串
{
  u32 i;
  u32 x;
  u32 y;
  u32 Row;//字符串号
  u16 * pStrBuff;
  u32 MenuMaxLn;//20140305菜单显示到液晶第MenuMaxLn行宋玉梅
  
#if ((Project/100)==3 && (USER/100)==5)//上海专变
//上海要求：菜单显示从第2到第9行，液晶最上下两行为显示图标等状态栏
	MenuMaxLn=9;
#else
	MenuMaxLn=10;
#endif

  i=0;
  Row=1;//菜单条号
  if(Col>=20)
  {
    Col=AutoIndent;
    Ln++;
  }
  if(Ln>=MenuMaxLn)
//  if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
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
	  if( Col != AutoIndent )//2012年02月09日避免出现问题：刚好20个字符但液晶上多显示一个空行宋玉梅
      Ln++;
      if(Ln>=MenuMaxLn)
//	  if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
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
//    if(x>0xA1)
    if(x>=0xA1)	 //20120221宋玉梅
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
      StrBuff->Row[Ln].Row=Row;//字符串号
      Col++;
	  if(Col>=20)
	  {//换行
          pStrBuff[0]=0;
	  	  Col=AutoIndent;
          Ln++;
        if(Ln>=MenuMaxLn)
//	  	  if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
          {
            i--;
            return (Ln+(Col<<8)+(Row<<16));
          }
          pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
          pStrBuff[0]=x;
	  }
	  x+=1;
	  pStrBuff[1]=x;
      StrBuff->Row[Ln].Row=Row;//字符串号
      pStrBuff+=2;
      Col++;
	  if(Col>=20)
	  {//换行
	 	  Col=AutoIndent;
          Ln++;
        if(Ln>=MenuMaxLn)
//	      if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
          {
            return (Ln+(Col<<8)+(Row<<16));
          }
          pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
	  }
    }
    else
    {
      //ASCII字库
      pStrBuff[0]=x+0x2000;
      StrBuff->Row[Ln].Row=Row;//字符串号
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//换行
				Col=AutoIndent;
        Ln++;
        if(Ln>=MenuMaxLn)
//	    if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
        {
          return (Ln+(Col<<8)+(Row<<16));
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
	  	}
    }
  }
}

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenu_SH
// Descriptions:        显示菜单
// input parameters:    菜单上移行数,字符串指针(\n换下行,\r回列0,0结束)
// output parameters:   填入显示缓冲区等
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayMenu_SH(u32 Ln,u32 MoveRow,u8 * pStr)//显示菜单
{
	u32 i;
	u32 TotalRow;//菜单字符串中总有菜单行计数(计标题行)
	u8 * p8s;//开始行指针
	
	//清全屏
	DisplayClr();//清全屏
	Draw_Clr();
	//计算菜单总有行数及开始行指针
	i=0;
	TotalRow=0;
	p8s=pStr;
	while(1)
	{
		if(pStr[i]==0)
		{
			break;
		}
		if(pStr[i]=='\n')
		{
			TotalRow++;
			i++;
			if(TotalRow==(MoveRow+1))
			{
				p8s+=i;
			}
			continue;
		}
		i++;
	}
	Stack->MenuStack[MenuCount->Count].TotalRow=TotalRow;
	
	//装入菜单
	//主标题
	DispalyStringOnlyOneRow(Ln,0,pStr);//显示字符串仅在1行内
	DispalyAlign(Ln,0,2);//行显示对齐(1左对齐,2居中,3右对齐) 
	//显示菜单行
	if(i!=0)
	{
		i=DisplayMenuString(Ln+1,0,2,p8s);//显示菜单字符串
		Stack->MenuStack[MenuCount->Count].MenuRow=((i>>16)&0xff)+1;
	}
	Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
}

#endif

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenu
// Descriptions:        显示菜单
// input parameters:    菜单上移行数,字符串指针(\n换下行,\r回列0,0结束)
// output parameters:   填入显示缓冲区等
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayMenu(u32 MoveRow,u8 * pStr)//显示菜单
{
	u32 i;
	u32 TotalRow;//菜单字符串中总有菜单行计数(计标题行)
	u8 * p8s;//开始行指针
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif ((Project/100)==2 && (USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  
	
	//清全屏
	DisplayClr();//清全屏
	Draw_Clr();
	//计算菜单总有行数及开始行指针
	i=0;
	TotalRow=0;
	p8s=pStr;
	while(1)
	{
		if(pStr[i]==0)
		{
			break;
		}
		if(pStr[i]=='\n')
		{
			TotalRow++;
			i++;
			if(TotalRow==(MoveRow+1))
			{
				p8s+=i;
			}
			continue;
		}
		i++;
	}
	Stack->MenuStack[MenuCount->Count].TotalRow=TotalRow;
	
	//装入菜单
	//主标题
	DispalyStringOnlyOneRow(Ln,0,pStr);//显示字符串仅在1行内
	DispalyAlign(Ln,0,2);//行显示对齐(1左对齐,2居中,3右对齐) 
	//显示菜单行
	if(i!=0)
	{
#if ((Project/100)==3 && (USER/100)==5)//上海专变
		i=DisplayMenuString_SH(Ln+1,0,2,p8s);//显示菜单字符串
#else
		i=DisplayMenuString(Ln+1,0,2,p8s);//显示菜单字符串
#endif
		Stack->MenuStack[MenuCount->Count].MenuRow=((i>>16)&0xff)+1;
	}
	Comm_Ram->DisplayScanTimer=0;//2 显示扫描10MS定时器
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuIn
// Descriptions:        进入下级菜单
// input parameters:    下级菜单函数地址
// output parameters:   初始化下级菜单任务计数等数据
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuCreate(u32 FunctionAddr)//创建下级菜单
{
	if(MenuCount->Count>=MaxMenuCount)
	{
		return;
	}
	MenuCount->Count++; 
	Stack->MenuStack[MenuCount->Count].PC=FunctionAddr;//菜单任务运行地址(特别注意:此值不可随便改动)
	Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
	Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditRow=1;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuDel
// Descriptions:        返回上级菜单
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuDel(void)//返回上级菜单
{
	if(MenuCount->Count!=0)
	{
		MenuCount->Count--;
	}
	Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
	Comm_Ram->DisplayScanTimer=10;//2 显示扫描10MS定时器
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuNegative
// Descriptions:        反显菜单行
// input parameters:    行号0-9
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuNegative(u32 Row)//反显菜单行
{
	u32 i;
	u32 Ln;
	u16 * p16;
	
	for(Ln=0;Ln<10;Ln++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4);
		for(i=0;i<20;i++)
		{
			p16[i]&=0x7fff;
		}
	}
	for(Ln=0;Ln<10;Ln++)
	{
		if(StrBuff->Row[Ln].Row==Row)
		{
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4);
			for(i=0;i<20;i++)
			{
				p16[i]|=0x8000;
			}														
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuSelect
// Descriptions:        菜单选择
// input parameters:    无
// output parameters:   修改Stack->MenuStack[MenuCount->Count].EditRow
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuSelect(void)//菜单选择
{
	u32 i;
	u32 Row;
	
	
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//下移键按下
		Row=Stack->MenuStack[MenuCount->Count].EditRow+1;
		for(i=0;i<10;i++)
		{
			if(StrBuff->Row[i].Row==Row)
			{
				break;
			}
		}
		
		if(i!=10)
		{
			Stack->MenuStack[MenuCount->Count].EditRow=Row;
			MenuNegative(Row);//反显菜单行
		}
		else
		{
			if((Stack->MenuStack[MenuCount->Count].MoveRow+Stack->MenuStack[MenuCount->Count].MenuRow)<Stack->MenuStack[MenuCount->Count].TotalRow)
			{
				Stack->MenuStack[MenuCount->Count].MoveRow++;
				Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
			}
		}
	}
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//上移键按下
		Row=Stack->MenuStack[MenuCount->Count].EditRow;
		if(Row==1)
		{
			i=Stack->MenuStack[MenuCount->Count].MoveRow;//菜单上移条数(进入时总为0,返回时不变)
			if(i!=0x0)
			{
				i--;
				Stack->MenuStack[MenuCount->Count].MoveRow=i;//菜单上移条数(进入时总为0,返回时不变)
				Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
			}
		}
		else
		{
			Row--;
			Stack->MenuStack[MenuCount->Count].EditRow=Row;
			MenuNegative(Row);//反显菜单行
		}
	}   
}

//-------------------------------------------------------------------------------------------------
// Function name:       UpArrow
// Descriptions:        ↑
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void UpArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2 && (USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif						  

	DisplayString(Ln,18,0,"↑");
}

//-------------------------------------------------------------------------------------------------
// Function name:       DownArrow
// Descriptions:        ↓
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void DownArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=8;//菜单在第8行结束
#elif((Project/100)==2 && (USER/100)==17)//江苏集中器
	Ln=8;//菜单从第1行开始
#else
	Ln=9;//菜单在第9行结束
#endif						  

	DisplayString(Ln,18,0,"↓");
}

//-------------------------------------------------------------------------------------------------
// Function name:       LeftArrow
// Descriptions:        ←
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void LeftArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	DisplayString(Ln,0,0,"←");
}

//-------------------------------------------------------------------------------------------------
// Function name:       RightArrow
// Descriptions:        →
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void RightArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=8;//菜单在第8行结束
#else
	Ln=9;//菜单在第9行开始
#endif

	DisplayString(Ln,0,0,"→");
}

//-------------------------------------------------------------------------------------------------
// Function name:       Arrowhead
// Descriptions:        当有下一屏时，屏幕右下角显示向下的箭头当有上一屏时，屏幕右上角显示向上的箭头
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120211
// Date Modified：		20120221
//-------------------------------------------------------------------------------------------------
void Arrowhead(void)//当有下一屏时，屏幕右下角显示向下的箭头当有上一屏时，屏幕右上角显示向上的箭头
{
//20120211 当有下一屏时，屏幕右下角显示向下的箭头 宋玉梅
	if(Stack->MenuStack[MenuCount->Count].TotalRow-(Stack->MenuStack[MenuCount->Count].MenuRow+Stack->MenuStack[MenuCount->Count].MoveRow)>0)
	{
		DownArrow();
	}			 
//20120211 当有上一屏时，屏幕右上角显示向上的箭头 宋玉梅	  
	if(Stack->MenuStack[MenuCount->Count].MoveRow!=0)
	{
		UpArrow();
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeRowCol
// Descriptions:        反显指定的行和列
// input parameters:    行号0-9，列号0-19
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120213
//-------------------------------------------------------------------------------------------------
void NegativeRowCol(u32 Row,u32 Col)//与字符串号有关，菜单用
{
	u32 i;
	u32 Ln;
	u16 * p16;
	
	for(Ln=0;Ln<10;Ln++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4);
		for(i=0;i<20;i++)
		{
			p16[i]&=0x7fff;
		}
	}
	for(Ln=0;Ln<10;Ln++)
	{
		if(StrBuff->Row[Ln].Row==Row)
		{
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+Col*2);
			*p16|=0x8000;//反显指定行、列 宋玉梅
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnCol
// Descriptions:        反显指定的行和列
// input parameters:    行号0-9，列号0-19
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120217
//-------------------------------------------------------------------------------------------------
void NegativeLnCol(u32 Ln,u32 Col)
{
	u16 * p16;
	u32 i;
	u32 Row;
	
	for(Row=0;Row<10;Row++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4);
		for(i=0;i<20;i++)
		{
			p16[i]&=0x7fff;
		}
	}
	p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+Col*2);
	*p16|=0x8000;//反显指定行、列 宋玉梅
}

//-------------------------------------------------------------------------------------------------
// Function name:       Select
// Descriptions:        十进制数左右选择
// input parameters:    字符串右边界，字符串左边界
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120213
// Date Modified：      20120301
//-------------------------------------------------------------------------------------------------
void Select(u32 MaxCol,u32 MinCol)
{
	u16 *p16;
	u32 x;
	u32 i;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif ((Project/100)==2 && (USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//右移键按下
		Stack->MenuStack[MenuCount->Count].EditCol++;
		for(i=0;i<2;i++)
		{
			if(Stack->MenuStack[MenuCount->Count].EditCol==MaxCol+1)
			{
				Stack->MenuStack[MenuCount->Count].EditCol=MinCol;
			}    
			p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
			memcpy(&x,p16,2);
			x&=0x00ff;
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//只有数字、>能选中，其他符号全部跳过
			{
				Stack->MenuStack[MenuCount->Count].EditCol ++;
			}	
			else
			{
				break;
			}	
		}
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)
	{//左移键按下
		Stack->MenuStack[MenuCount->Count].EditCol--;
		for(i=0;i<2;i++)
		{
			if(Stack->MenuStack[MenuCount->Count].EditCol==MinCol-1)
			{
				Stack->MenuStack[MenuCount->Count].EditCol=MaxCol;
			}	
			p16=(u16 *)(ADDR_STRINGBUFF+Ln*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
			memcpy(&x,p16,2);
			x&=0x00ff;
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//只有数字、>能选中，其他符号全部跳过
			{
				Stack->MenuStack[MenuCount->Count].EditCol --;
			}
			else
			{
				break;
			}	
		}
	}
	NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);
}
//-------------------------------------------------------------------------------------------------
// Function name:       SelectH
// Descriptions:        十六进制数左右选择
// input parameters:    字符串右边界，字符串左边界
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120427
//-------------------------------------------------------------------------------------------------
void SelectH(u32 MaxCol,u32 MinCol)
{
	u16 *p16;
	u32 x;
	u32 i;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第一行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//右移键按下
		Stack->MenuStack[MenuCount->Count].EditCol++;
		for(i=0;i<2;i++)
		{
			if(Stack->MenuStack[MenuCount->Count].EditCol==MaxCol+1)
			{
				Stack->MenuStack[MenuCount->Count].EditCol=MinCol;
			}    
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
			memcpy(&x,p16,2);
			x&=0x00ff;
			if((x<0x30 && x!=0x3e) || (x>0x39 && x<0x41 && x!=0x3e) || (x>0x46 && x!=0x3e))//只有十六进制数字、>能选中，其他符号全部跳过
			{
				Stack->MenuStack[MenuCount->Count].EditCol ++;
			}	
			else
			{
				break;
			}	
		}
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)
	{//左移键按下
		Stack->MenuStack[MenuCount->Count].EditCol--;
		for(i=0;i<2;i++)
		{
			if(Stack->MenuStack[MenuCount->Count].EditCol==MinCol-1)
			{
				Stack->MenuStack[MenuCount->Count].EditCol=MaxCol;
			}	
			p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
			memcpy(&x,p16,2);
			x&=0x00ff;
			if((x<0x30 && x!=0x3e) || (x>0x39 && x<0x41 && x!=0x3e) || (x>0x46 && x!=0x3e))//只有十六进制数字、>能选中，其他符号全部跳过
			{
				Stack->MenuStack[MenuCount->Count].EditCol --;
			}
			else
			{
				break;
			}	
		}
	}
	NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);
}
//-------------------------------------------------------------------------------------------------
// Function name:       CreateScreen
// Descriptions:        翻屏
// input parameters:    下一屏函数地址
// output parameters:   初始化下级菜单任务计数等数据
// Returned value:      无
// Author:              宋玉梅
// Date:                20120220
//-------------------------------------------------------------------------------------------------
void CreateScreen(u32 FunctionAddr)//翻屏
{
	Stack->MenuStack[MenuCount->Count].PC=FunctionAddr;//菜单任务运行地址(特别注意:此值不可随便改动)
	Stack->MenuStack[MenuCount->Count].Task=0;//菜单任务计数(进入和退出时均清为0)
	Stack->MenuStack[MenuCount->Count].MoveRow=0;//菜单上移条数(进入时总为0,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditRow=1;//菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
	Stack->MenuStack[MenuCount->Count].EditCol=0;//菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
}


//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnMaxColMinCol
// Descriptions:        反显除了MinCol~MaxCol以外的列
// input parameters:    行号0-9，列号
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120310
//-------------------------------------------------------------------------------------------------
void NegativeLnMaxColMinCol(u32 Row,u32 MaxCol,u32 MinCol)//与字符串号有关
{
	u16 * p16;
	u32 i;
	u32 Ln;
	
	for(Ln=0;Ln<10;Ln++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4);
		for(i=0;i<20;i++)
		{
			p16[i]&=0x7fff;
		}
	}
	for(Ln=0;Ln<10;Ln++)
	{
		if(StrBuff->Row[Ln].Row==Row)
		{
			for(i=0;i<MinCol;i++)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+i*2);
				*p16|=0x8000;//反显指定行、列 宋玉梅    
			}
			for(i=MaxCol+1;i<20;i++)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+i*2);
				*p16|=0x8000;//反显指定行、列 宋玉梅    
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnMaxColMinColN
// Descriptions:        反显除了MinCol~MaxCol以外的列
// input parameters:    行号0-9，列号
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void NegativeLnMaxColMinColN(u32 Row,u32 MaxCol,u32 MinCol)//与字符串号无关
{
	u16 * p16;
	u32 i;
	u32 Ln;
	
	for(Ln=0;Ln<10;Ln++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4);
		for(i=0;i<20;i++)
		{
			p16[i]&=0x7fff;
		}
	}
	for(i=0;i<MinCol;i++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+i*2);
		*p16|=0x8000;//反显指定行、列 宋玉梅    
	}
	for(i=MaxCol+1;i<20;i++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+i*2);
		*p16|=0x8000;//反显指定行、列 宋玉梅    
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       BoxSelect
// Descriptions:        编辑框选择
// input parameters:    无
// output parameters:   修改Stack->MenuStack[MenuCount->Count].EditRow
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void BoxSelect(void)//编辑框选择
{
	u32 i;
	u32 Row;
	
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//下移键按下
		Row=Stack->MenuStack[MenuCount->Count].EditRow+1;
		for(i=0;i<10;i++)
		{
			if(StrBuff->Row[i].Row==Row)
			{
				break;
			}
		}
		
		if(i!=10)
		{
			Stack->MenuStack[MenuCount->Count].EditRow=Row;
		}
		else
		{
			if((Stack->MenuStack[MenuCount->Count].MoveRow+Stack->MenuStack[MenuCount->Count].MenuRow)<Stack->MenuStack[MenuCount->Count].TotalRow)
			{
				Stack->MenuStack[MenuCount->Count].MoveRow++;
				Stack->MenuStack[MenuCount->Count].Task=2;//用于菜单8_9
			}
		}
	}
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//上移键按下
		Row=Stack->MenuStack[MenuCount->Count].EditRow;
		if(Row==1)
		{
			i=Stack->MenuStack[MenuCount->Count].MoveRow;//菜单上移条数(进入时总为0,返回时不变)
			if(i!=0x0)
			{
				i--;
				Stack->MenuStack[MenuCount->Count].MoveRow=i;//菜单上移条数(进入时总为0,返回时不变)
				Stack->MenuStack[MenuCount->Count].Task=2;//用于菜单8_9
			}
		}
		else
		{
			Row--;
			Stack->MenuStack[MenuCount->Count].EditRow=Row;
		}
	}   
}

//-------------------------------------------------------------------------------------------------
// Function name:       KeyboardSelect
// Descriptions:        键盘选择
// input parameters:    软键盘最小行号MinLn，软键盘最大行号MaxLn，软键盘的最后一行最大列号LastLnMaxCol，软键盘最小列号MinCol，软键盘最大列号MaxCol，编辑框行号EditLn，编辑框起始列号EditCol
// output parameters:   
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void KeyboardSelect(u32 MinLn,u32 MaxLn,u32 LastLnMaxCol,u32 MinCol,u32 MaxCol,u32 EditLn,u32 EditCol)//键盘选择
{
	u16 *p16;
	u32 x;

	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//右移键按下
		if(((Stack->MenuStack[MenuCount->Count].EditCol >= MaxCol)&&(Stack->MenuStack[MenuCount->Count].EditRow !=MaxLn))||((Stack->MenuStack[MenuCount->Count].EditCol >= LastLnMaxCol)&&(Stack->MenuStack[MenuCount->Count].EditRow ==MaxLn)))
		{
			Stack->MenuStack[MenuCount->Count].EditRow ++;
			if(Stack->MenuStack[MenuCount->Count].EditRow == MaxLn+1)
			{
				Stack->MenuStack[MenuCount->Count].EditRow = MinLn;
			}
			Stack->MenuStack[MenuCount->Count].EditCol = MinCol;
		}
		else
		{
			Stack->MenuStack[MenuCount->Count].EditCol ++;	
		}
	}
	if((Comm_Ram->ButtonVal&0x04)!=0)
	{//左移键按下
		if(Stack->MenuStack[MenuCount->Count].EditCol <= MinCol)
		{
			if(Stack->MenuStack[MenuCount->Count].EditRow <= MinLn)
			{
				Stack->MenuStack[MenuCount->Count].EditRow = MaxLn;
				Stack->MenuStack[MenuCount->Count].EditCol = LastLnMaxCol;
			}
			else
			{
				Stack->MenuStack[MenuCount->Count].EditRow --;
				Stack->MenuStack[MenuCount->Count].EditCol = MaxCol;
			}
		}
		else
		{
			Stack->MenuStack[MenuCount->Count].EditCol --;	
		}
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//向下键按下
		if(Stack->MenuStack[MenuCount->Count].EditCol <= LastLnMaxCol)
		{
			Stack->MenuStack[MenuCount->Count].EditRow ++;
			if((Stack->MenuStack[MenuCount->Count].EditRow)==MaxLn+1)
			{
				Stack->MenuStack[MenuCount->Count].EditRow = MinLn;
			}	
		}
		else
		{
			Stack->MenuStack[MenuCount->Count].EditRow ++;
			if(Stack->MenuStack[MenuCount->Count].EditRow == MaxLn)
			{
				Stack->MenuStack[MenuCount->Count].EditRow = MinLn;
			}
		}	    
	}
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//向上键按下
		if(Stack->MenuStack[MenuCount->Count].EditCol <= LastLnMaxCol)
		{
			Stack->MenuStack[MenuCount->Count].EditRow --; 
			if((Stack->MenuStack[MenuCount->Count].EditRow)==MinLn-1)
			{
				Stack->MenuStack[MenuCount->Count].EditRow = MaxLn;
			}	
		}
		else
		{
			Stack->MenuStack[MenuCount->Count].EditRow --; 
			if((Stack->MenuStack[MenuCount->Count].EditRow)==MinLn-1)
			{
				Stack->MenuStack[MenuCount->Count].EditRow = MaxLn-1;
			}	
		}	    
	}
	NegativeLnCol(Stack->MenuStack[MenuCount->Count].EditRow,Stack->MenuStack[MenuCount->Count].EditCol);
	if((Comm_Ram->ButtonVal&0x10)!=0)
	{//确认
		p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		*p16&=0x7FFF;
		memcpy(&x,p16,2);
		x&=0x00ff;
		DisplayString(EditLn,EditCol+Stack->MenuStack[MenuCount->Count].MoveRow,0, (u8 *)&x);//显示字符串
		Stack->MenuStack[MenuCount->Count].Task = 2;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+((EditLn)*84)+4+(EditCol+Stack->MenuStack[MenuCount->Count].MoveRow)*2);
	*p16|=0x8000;
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//返回
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//返回上级菜单
	}
	Comm_Ram->ButtonVal&=~0x3F;//键使用后清0
}

//-------------------------------------------------------------------------------------------------
// Function name:      bcd_p8       
// Descriptions:       将十进制数x放到显示缓冲区*p8中        
// input parameters:   显示缓冲区*p8,十进制数x,显示的位数z    
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void bcd_p8(u8 *p8,u64 x,u32 z)
{
	u32 i;
	u64 y=10;

	for(i=0;i<z;i++)
	{
		p8[(z-1-i)*2]=x%y/(y/10)+0x30;
		y*=10;
		p8[i*2+1]=0x20;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:      bcd_p16       
// Descriptions:       将十进制数x放到显示缓冲区*p16中        
// input parameters:   显示缓冲区*p16,十进制数x,显示的位数z    
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void bcd_p16(u16 *p16,u64 x,u32 z)
{
	u32 i;
	u64 y=10;

	for(i=0;i<z;i++)
	{
		p16[z-1-i]=x%y/(y/10)+0x2030;
		y*=10;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:      p8_bcd       
// Descriptions:       将显示缓冲区*p8中的数转化为十进制数x               
// input parameters:   显示缓冲区*p8，转化后的位数z       
// output parameters:  无   
// Returned value:     转化后的十进制数x      
//-------------------------------------------------------------------------------------------------
u64 p8_bcd(u8 *p8,u32 z)
{
	u32 i;
	u64 x=0;
	u64 y=1;

	for(i=0;i<z;i++)
	{
		x+=(p8[(z-1-i)*2]-0x30)*y;
		y*=10;
	}
	return x;
}

//-------------------------------------------------------------------------------------------------
// Function name:      p8_add1       
// Descriptions:       将显示缓冲区*p8中的数加1              
// input parameters:   显示缓冲区*p8，显示数的位数z       
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void p8_add1(u8 *p8,u32 z)
{
	u32 i;

	for(i=z;i>0;i--)
	{
		if(p8[(i-1)*2]>=0x30 && p8[(i-1)*2]<0x39)
		{
			p8[(i-1)*2]++;
			break;
		}
		else if(p8[(i-1)*2]==0x39)
		{
			p8[(i-1)*2]=0x30;
		}
	}
}
	
//-------------------------------------------------------------------------------------------------
// Function name:      p8_sub1       
// Descriptions:       将显示缓冲区*p8中的数减1              
// input parameters:   显示缓冲区*p8，显示数的位数z       
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void p8_sub1(u8 *p8,u32 z)
{
	u32 i;

	for(i=z;i>0;i--)
	{
		if(p8[(i-1)*2]>0x30 && p8[(i-1)*2]<=0x39)
		{
			p8[(i-1)*2]--;
			break;
		}
		else if(p8[(i-1)*2]==0x30)
		{
			p8[(i-1)*2]=0x39;
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelect
// Descriptions:        十进制数上下选择
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void UpDownSelect(void)
{
	u16 *p16;
	u32 x;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif ((Project/100)==2 && (USER/100)==17)//江苏集中器
	Ln=1;//菜单从第1行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//上移键按下
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		memcpy(&x,p16,2);
		x&=0x00ff;
		if(x>=0x30 && x<0x39)
		{
			x++;
		}
		else
		{
			if(x==0x39)
			{
				x=0x30;
			}
		}
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//显示字符串
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//下移键按下
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		memcpy(&x,p16,2);
		x&=0x00ff;
		if(x>0x30 && x<=0x39)
		{
			x--;
		}
		else
		{
			if(x==0x30)
			{
				x=0x39;
			}
		}
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//显示字符串
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelectH
// Descriptions:        十六进制数上下选择
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void UpDownSelectH(void)
{
	u16 *p16;
	u32 x;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2&&(USER/100)==17)//江苏集中器
	Ln=1;//菜单从第一行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//上移键按下
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		memcpy(&x,p16,2);
		x&=0x00ff;
		if((x>=0x30 && x<0x39) || (x>=0x41 && x<0x46))
		{
			x++;
		}
		else
		{
			if(x==0x39)
			{
				x=0x41;
			}
			else
			{
				if(x==0x46)
				{
					x=0x30;
				}
			}
		}
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//显示字符串
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//下移键按下
		p16=(u16 *)(ADDR_STRINGBUFF+(Ln)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		memcpy(&x,p16,2);
		x&=0x00ff;
		if((x>0x30 && x<=0x39) || (x>0x41 && x<=0x46))
		{
			x--;
		}
		else
		{
			if(x==0x41)
			{
				x=0x39;
			}
			else
			{
				if(x==0x30)
				{
					x=0x46;
				}
			}
		}
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//显示字符串
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NdigitSelect
// Descriptions:        N位数左右选择
// input parameters:    N位数，编辑框列号Col，可选范围最小数Min，可选范围最大数Max
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void NdigitSelect(u32 N,u32 Col,u32 Min,u32 Max)
{
	u8 *p8;
	u32 x;
	u32 i;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Ln=1;//菜单从第1行开始
#elif((Project/100)==2 && (USER/100)==17)//江苏集中器
	Ln=1;//菜单从第2行开始
#else
	Ln=0;//菜单从第0行开始
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	for(i=0;i<2;i++)
	{
		if(Stack->MenuStack[MenuCount->Count].EditCol==0)
		{
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//右键
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				x=p8_bcd(p8,N);
				if(x<Max)
				{
					p8_add1(p8,N);
				}
				else
				{
					bcd_p8(p8,Min,N);
				}
				MenuCount->v2=p8_bcd(p8,N);
				Stack->MenuStack[MenuCount->Count].Task=0;
				MenuCount->v1=0;
			}
			if((Comm_Ram->ButtonVal&0x04)!=0)
			{//左键
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				x=p8_bcd(p8,N);
				if(x>Min)
				{
					p8_sub1(p8,N);
				}
				else
				{
					bcd_p8(p8,Max,N);
				}
				MenuCount->v2=p8_bcd(p8,N);
				Stack->MenuStack[MenuCount->Count].Task=0;
				MenuCount->v1=0;
			}
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				Stack->MenuStack[MenuCount->Count].EditCol=Col;
				DisplayString(Ln,16,0,"输入");
				Comm_Ram->ButtonVal&=~0x1C;//键使用后清0
			}
			else
			{
				Comm_Ram->ButtonVal&=~0x0C;//键使用后清0
				break;
			}
		}
		else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
		{
			Select(Col+N-1,Col);
			UpDownSelect();
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//确认
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				x=p8_bcd(p8,N);
				if(x>Max || x<Min)
				{
					bcd_p8(p8,Min,N);
				}
				MenuCount->v2=p8_bcd(p8,N);
				Stack->MenuStack[MenuCount->Count].Task=0;
				MenuCount->v1=0;
				Stack->MenuStack[MenuCount->Count].EditCol=0;
				Comm_Ram->ButtonVal&=~0x1F;//键使用后清0
			}
			else
			{
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				MenuCount->v2=p8_bcd(p8,N);
				MenuCount->v1=0;
				Comm_Ram->ButtonVal&=~0x0F;//键使用后清0
				break;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       ArrowLR
// Descriptions:        在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
// input parameters:    Row，Ln
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void ArrowLR(u32 Row,u32 Ln)
{
	u16 *p16;
	u32 i;
	u32 k;

	for(k=0;k<10;k++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+k*84+4+Ln*2);
		if((p16[0]&0x7fff)==32 && (p16[1]&0x7fff)==33)
		{
			for(i=0;i<=1;i++)
			{
				p16[i]=(p16[i]&0x8000)+0x2020;
			}
		}
	}
	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4+Ln*2);
	p16[0]=32;//'左右箭头'
	p16[1]=33;
}

//-------------------------------------------------------------------------------------------------
// Function name:       PowerP16
// Descriptions:        将376.1协议中A2格式的功率转换后放入显示缓冲区p16中。（显示缓冲指针在本函数外面指定，功率±在本函数外面判断）
// input parameters:    功率y，显示缓冲区p16
// output parameters:   无
// Returned value:      下字符显示缓冲指针
//-------------------------------------------------------------------------------------------------
u16 * PowerP16(u32 y,u16 *p16)
{
	u32 z;
	u32 m;

	z=y&0xfff;
	switch(y>>13)
	{
		case 0://10的4次方
			m=z<<(4*4);
			p16 = DisplayData_OffZero(m,7,0,p16);
			break;
		case 1://10的3次方
			m=z<<(3*4);
			p16 = DisplayData_OffZero(m,6,0,p16);
			break;
		case 2://10的2次方
			m=z<<(2*4);
			p16 = DisplayData_OffZero(m,5,0,p16);
			break;
		case 3://10的1次方
			m=z<<(1*4);
			p16 = DisplayData_OffZero(m,4,0,p16);
			break;
		case 4://10的0次方
			m=z;
			p16 = DisplayData_OffZero(m,3,0,p16);
			break;
		case 5://10的-1次方
			m=z;
			p16 = DisplayData_OffZero(m,3,1,p16);
			break;
		case 6://10的-2次方
			m=z;
			p16 = DisplayData(m,3,2,p16);
			break;
		case 7://10的-3次方
			m=z;
			p16 = DisplayData(m,4,3,p16);
			break;
		default:
			break;
	}
	return p16;
}

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayClrRow_4
// Descriptions:        清屏幕显示的指定行(4个控制字不清)
// input parameters:    行号(行号0-9(160*160),0-14(320*240))
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------

void DisplayClrRow_4(u32 Row)//清屏幕显示的指定行(4个控制字不清)
{
	u16 *p16;
	u32 i;

	p16=(u16 *)(ADDR_STRINGBUFF+Row*84+4);
	for(i=0;i<20;i++)
	{
		p16[i]=0x0;
	}
}



//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString1
// Descriptions:        显示字符串（\n\r各空一个半角字，不按回车换行处理）
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23当前字符串位置,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayString1(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)
{
	u32 i;
	u32 x;
	u32 y;
	u16 * pStrBuff;
	u32 Row;

#if ((Project/100)==3 && (USER/100)==5)//上海专变
	Row=8;//菜单在第8行开始
#else
	Row=9;//菜单在第9行开始
#endif
	
	if(Ln>(Row-1))
	{
		return (Ln+(Col<<8)+(0<<16));
	}
	
	i=0;
	if(Col>=20)
	{
		Col=AutoIndent;
		Ln++;
	}
	if(Ln>=Row)
	{
		return (Ln+(Col<<8)+(i<<16));
	}
	pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
	while(1)
	{
		if(pStr[i]==0)
		{//结束
			i++;
			return (Ln+(Col<<8)+(i<<16));
		}
		if(pStr[i]=='\n')
		{//换行
			i++;
			Col++;
			if(Col>=20)
			{//换行
				Col=AutoIndent;
				Ln++;
				if(Ln>=Row)
				{
					return (Ln+(Col<<8)+(i<<16));
				}
			}
			pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
			continue;
		}
		if(pStr[i]=='\r')
		{//回车
			i++;
			Col++;
			if(Col>=20)
			{//换行
				Col=AutoIndent;
				Ln++;
				if(Ln>=Row)
				{
					return (Ln+(Col<<8)+(i<<16));
				}
			}
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
			Col++;
			if(Col>=20)
			{//换行
				pStrBuff[0]=0;
				Col=AutoIndent;
				Ln++;
				if(Ln>=Row)
				{
					i=i-2;
					return (Ln+(Col<<8)+(i<<16));
				}
				pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
				pStrBuff[0]=x;
				Col ++;//2012-6-13
			}
			x+=1;
			pStrBuff[1]=x;
			pStrBuff+=2;
			Col++;
			if(Col>=20)
			{//换行
				Col=AutoIndent;
				Ln++;
				if(Ln>=Row)
				{
					return (Ln+(Col<<8)+(i<<16));
				}
				pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
			}
		}
		else
		{
			//ASCII字库
			pStrBuff[0]=x+0x2000;
			pStrBuff++;
			Col++;
			if(Col>=20)
			{//换行
				Col=AutoIndent;
				Ln++;
				if(Ln>=Row)
				{
					return (Ln+(Col<<8)+(i<<16));
				}
				pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
			}
		}
	}
}


//-------------------------------------------------------------------------------------------------
// Function name:       DT1DT2_Fn
// Descriptions:        数据标识转化为Fn
// input parameters:    DT1，DT2，DT1的第i位
// output parameters:   无
// Returned value:      Fn,m=1有此Fn,m=0无此Fn
//-------------------------------------------------------------------------------------------------
u32 DT1DT2_Fn(u32 DT1,u32 DT2,u32 i)
{
	u32 m;
	u32 Fn;

	if((DT1>>i)&0x01)
	{
		m=1;
		Fn=DT2*8+i+1;
	}
	else
	{
		m=0;
		Fn=0;
	}
	return (m<<8)+Fn;
}

//-------------------------------------------------------------------------------------------------
// Function name:       DA1DA2_pn
// Descriptions:        数据标识转化为pn
// input parameters:    DA1，DA2，DA1的第i位
// output parameters:   无
// Returned value:      pn
//-------------------------------------------------------------------------------------------------
u32 DA1DA2_pn(u32 DA1,u32 DA2,u32 i)
{
	u32 pn;

	if(DA2==0)
	{
		pn=0;
	}
	else
	{
		if((DA1>>i)&0x01)
		{
			pn=(DA2-1)*8+i+1;
		}
		else
		{
			pn=0;
		}
	}
	return pn;
}

//把功率值d64,输出到液晶显示缓存区p16，显示的单位是kw
//unit表示d64原来的单位w(unit=0)或kw(unit=1)，bit表示d64的换算位数
//u32 type显示的单位 0=kW 1=kvar 2=kWh 3=kvarh 4=元
//负数是用补码表示的!!!
void disp_long64_kw(u64 d64,u32 unit,u32 bit,u16 *p16,u32 type)//
{
	u32 DpNum=0;
	if((unit!=1)&&(unit!=0))
		return;
	if(bit>4)
		return;
	if((d64>>63)!=0x0)
	{//负
		d64=(~d64)+1;
		p16[0]='-'+0x2000;
		p16++;
	}
	if(unit==0)
		DpNum=3;
	else
		DpNum=0;
	DpNum+=bit;
	
	d64=hex_bcd16(d64);
	p16=DisplayData_OffZero(d64,12,DpNum,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF
	if(type==0)
		DispalyStringContinue(p16,(u8 *)"kW");//显示字符串继续
	else if(type==1)
		DispalyStringContinue(p16,(u8 *)"kvar");//显示字符串继续
	else if(type==2)
		DispalyStringContinue(p16,(u8 *)"kWh");//显示字符串继续
	else if(type==3)
		DispalyStringContinue(p16,(u8 *)"kvarh");//显示字符串继续
	else if(type==4)
		DispalyStringContinue(p16,(u8 *)"元");//显示字符串继续
}


//数据倒序
u64 REG_Inverse(u64 Data,u32 Byte)//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
{
	u64 NewData;
	if(Byte<2)
	{
		return Data;
	}
	NewData=0;
	while(Byte--)
	{
		NewData<<=8;
		NewData|=Data&0xff;
		Data>>=8;
	}
	return NewData;
}

void Disp_DataType_date_time_s(u32 Ln,u32 Addr_date_time_s)//显示DataType_date_time_s到Ln行
{
	u16 *p16;
	u64 y;
	y=MRR(Addr_date_time_s,1);
	if(y!=28)//DataType_date_time_s
	{
		return;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+4+Ln*84+4);
	y=MRR(Addr_date_time_s+1,2);
	y=REG_Inverse(y,2);
	y=hex_bcd(y);
	p16=DisplayData(y,4,0,p16);
	p16[0]=0x2000+'-';
	p16+=1;
	y=MRR(Addr_date_time_s+3,1);
	y=hex_bcd(y);
	p16=DisplayData(y,2,0,p16);
	p16[0]=0x2000+'-';
	p16+=1;
	y=MRR(Addr_date_time_s+4,1);
	y=hex_bcd(y);
	p16=DisplayData(y,2,0,p16);
	p16[0]=0x2000+' ';
	p16+=1;
	y=MRR(Addr_date_time_s+5,1);
	y=hex_bcd(y);
	p16=DisplayData(y,2,0,p16);
	p16[0]=0x2000+':';
	p16+=1;
	y=MRR(Addr_date_time_s+6,1);
	y=hex_bcd(y);
	DisplayData(y,2,0,p16);
}
