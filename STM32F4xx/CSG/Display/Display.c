//显示
#include "../Hcsg/ProjectCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../Display/Display.h"
#include "../Display/LCDModule.h"
#include "../Calculate/Calculate.h"


void DisplayClrRow(u32 StartRow,u32 EndRow)//清屏幕显示的指定行
{
	u32 i;
	u32 * p32;
	while(StartRow<=EndRow)
	{
		p32=(u32 *)(ADDR_STRINGBUFF+(StartRow*84));
		for(i=0;i<21;i++)
		{
			p32[i]=0x0;
		}
		StartRow++;
	}
}

void DisplayClr(void)//清全屏
{
  u32 i;
  u32 * p32;
	p32=(u32 *)(ADDR_STRINGBUFF);
	for(i=0;i<(10*21);i++)
	{
		p32[i]=0x0;
	}
	AutoDisplayRowCol->Row=0;
	AutoDisplayRowCol->Col=0;
}

u8* LanguageString(u8 *pStr)//按显示语言选择显示字符串
{
	u8 *p8;
	
	if(*pStr<=3)
	{//开始字节是非字符
		if(*pStr<=1)
		{//只有一种语言
			pStr++;
			return pStr;
		}
		pStr++;
	}
	p8=(u8*)ADDR_RamLanguage;
	if(p8[0]==0)
	{//中文
		return pStr;
	}
	//英文
	while(1)
	{
		if(*pStr==0)
		{
			pStr++;
			return pStr;
		}
		pStr++;
	}
}

void DisplayUpRow(u32 StartRow,u32 EndRow,u32 Row)//显示屏幕指定的窗口向上平移指定的行数;出口:平移后窗口底部空出的行清显示
{
	u32 i;
	u32 x;
	u32 *p32s;
	u32 *p32d;
	
	if(Row==0)
	{
		return;
	}
	//窗口内行数
	if(EndRow<StartRow)
	{
		return;
	}
	i=EndRow-StartRow;
	i++;
	if(Row>=i)
	{//需平移行数>=窗口内行数
		DisplayClrRow(StartRow,EndRow);//清屏幕显示的指定行
		return;
	}
	p32d=(u32 *)(ADDR_STRINGBUFF+(StartRow*84));
	p32s=(u32 *)(ADDR_STRINGBUFF+((StartRow+Row)*84));
	x=i-Row;
	x*=21;
	for(i=0;i<x;i++)
	{
		p32d[i]=p32s[i];
	}
	DisplayClrRow((EndRow+1)-Row,EndRow);//清屏幕显示的指定行
}

void DisplayStringAutoUp(u32 StartRow,u32 EndRow,u8 * pStr)//在指定窗口内自动向上平移后显示字符串;出口:修改下一显示行并寄存在ADDR_AutoRowCol
{
  u32 i;
  u32 x;
  u32 y;
  u16 * pStrBuff;
	u32 Ln;
	u32 Col;
	
	pStr=LanguageString(pStr);//按显示语言选择显示字符串
	Ln=AutoDisplayRowCol->Row;
	Col=0;
	
	if(Ln>9)
	{
		DisplayUpRow(StartRow,EndRow,1);//屏幕显示的指定行向上平移指定的行数;出口:平移后底部空出的行清行
		Ln=9;
	}
	DisplayClrRow(Ln,9);//清屏幕显示的指定行
  i=0;
  pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
  while(1)
  {
    if(pStr[i]==0)
    {//结束
			if(Col!=0)
			{
				Ln++;
			}
			AutoDisplayRowCol->Row=Ln;
			
			LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
			Comm_Ram->DisplayScanTimer=10000/10;//10s
      return;
    }
    if(pStr[i]=='\n')
    {//换行
      i++;
      Ln++;
      if(Ln>9)
      {
				DisplayUpRow(StartRow,EndRow,1);//屏幕显示的指定行向上平移指定的行数;出口:平移后底部空出的行清行
				Ln=9;
				Col=0;
      }
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
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
				Col=0;
        Ln++;
        if(Ln>9)
        {
          i--;
					DisplayUpRow(StartRow,EndRow,1);//屏幕显示的指定行向上平移指定的行数;出口:平移后底部空出的行清行
					Ln=9;
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
				Col=0;
        Ln++;
        if(Ln>9)
        {
					if(pStr[i]!=0)
					{
						DisplayUpRow(StartRow,EndRow,1);//屏幕显示的指定行向上平移指定的行数;出口:平移后底部空出的行清行
						Ln=9;
					}
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
			}
    }
    else
    {
      //ASCII字库
      pStrBuff[0]=x+0x2000;
      //StrBuff->Row[Ln].Row=Row;//字符串号
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//换行
				Col=0;
        Ln++;
        if(Ln>9)
        {
					if(pStr[i]!=0)
					{
						DisplayUpRow(StartRow,EndRow,1);//屏幕显示的指定行向上平移指定的行数;出口:平移后底部空出的行清行
						Ln=9;
					}
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
	  	}
    }
  }
}

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString
// Descriptions:        显示字符串
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示字符串
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
	pStr=LanguageString(pStr);//按显示语言选择显示字符串
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
      //ASCII字库
      pStrBuff[0]=x+0x2000;
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


//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString
// Descriptions:        显示字符串
// input parameters:    屏幕行(0-15),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//						注意使用时最好不要显示顶图标(0行)与底图标(9行)
//-------------------------------------------------------------------------------------------------
u32 DisplayStringOutoffScreenLine(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示字符串
{
	u32 i;
	u32 x;
	u32 y;
	u32 Row;//字符串号
	u16 * pStrBuff;
  
	if(Ln >= 16) return (Ln+(Col<<8)+(0<<16));

	pStr=LanguageString(pStr);//按显示语言选择显示字符串
	i=0;
	Row=1;//菜单条号
	if(Col>=20)
	{
		Col=AutoIndent;
		Ln++;
	}
	if(Ln>=16) return (Ln+(Col<<8)+(Row<<16));
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
			if(Ln>=16) return (Ln+(Col<<8)+(Row<<16));
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
				if(Ln>=16)
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
				if(Ln>=16) return (Ln+(Col<<8)+(Row<<16));
				pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
			}
		}
		else
		{
		  	//ASCII字库
		  	pStrBuff[0]=x+0x2000;
		  	//StrBuff->Row[Ln].Row=Row;//字符串号
		  	pStrBuff++;
		  	Col++;
		    if(Col>=20)
		  	{//换行
				Col=AutoIndent;
		    	Ln++;
		   		if(Ln>=16) return (Ln+(Col<<8)+(Row<<16));
		    	pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
		  	}
		}
	}
}


//---------------------------------------------------------------------------------------------------------
// Function name:       DispalyStringOnlyOneRow
// Descriptions:        显示字符串仅在1行内
// input parameters:    屏幕行(0-9),屏幕列(0-19),字符串指针(\n,\r,0均为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B31填入半角字符数
//---------------------------------------------------------------------------------------------------------
u32 DispalyStringOnlyOneRow(u32 Row,u32 Col,u8 * pStr)//显示字符串仅在1行内
{
  u32 i;
  u32 x;
  u32 y;
  u16 * pStrBuff;
  
	if(Row>9)
	{
		return (Row+(Col<<8)+(0<<16));
	}
	pStr=LanguageString(pStr);//按显示语言选择显示字符串
  i=0;
  if(Col>=20)
  {
    return (Row+(Col<<8)+(i<<16));
  }
  if(Row>=10)
  {
    return (Row+(Col<<8)+(i<<16));
  }
  pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+(Col*2));
  while(1)
  {
    if((pStr[i]==0)||(pStr[i]=='\n')||(pStr[i]=='\r'))
    {
      i++;
      return (Row+(Col<<8)+(i<<16));
    }
    x=pStr[i];
    i++;
    if(x>0xA1)
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
				return (Row+(Col<<8)+(i<<16));
			}
			x+=1;
			pStrBuff[1]=x;
      pStrBuff+=2;
      Col++;
			if(Col>=20)
			{//换行
				return (Row+(Col<<8)+(i<<16));
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
	  		return (Row+(Col<<8)+(i<<16));
	  	}
    }
  }
}


u16 * DispalyStringContinue(u16 *pStrBuff,u8 * pStr)//显示字符串继续
{
  u32 i;
  u32 x;
  u32 y;
  u32 Col;
	
	pStr=LanguageString(pStr);//按显示语言选择显示字符串
	Col=((u32)pStrBuff)-ADDR_STRINGBUFF;
	Col%=84;
	Col-=4;
	Col/=2;
  i=0;
  while(1)
  {
    if((pStr[i]==0)||(pStr[i]=='\n')||(pStr[i]=='\r'))
    {
      i++;
      return (pStrBuff);
    }
    x=pStr[i];
    i++;
    if(x>0xA1)
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
				return (pStrBuff);
			}
			x+=1;
			pStrBuff[1]=x;
      pStrBuff+=2;
      Col++;
			if(Col>=20)
			{//换行
				return (pStrBuff);
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
	  		return (pStrBuff);
	  	}
    }
  }
}



//---------------------------------------------------------------------------------------------------------
// Function name:       DispalyAlign
// Descriptions:        显示对齐(左对齐,居中,右对齐)
// input parameters:    Row需对齐的行,AlignCol表示对齐到列号(160*160屏的列号0-19,仅用于左对齐,右对齐),Align对齐方式:1=左对齐,2=居中,3=右对齐
// output parameters:   显示对齐(对齐后的溢出部份丢弃)
// Returned value:      无
//---------------------------------------------------------------------------------------------------------
void DispalyAlign(u32 Row,u32 AlignCol,u32 Align)//行显示对齐(1左对齐,2居中,3右对齐)
{
	u32 i;
	u32 x;
	u32 y;
	u32 Col;
	u16 * p16;

#if LCDPixels==3//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
	Col=160/8;
#endif
#if LCDPixels==2//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
	Col=320/8;
#endif
#if LCDPixels==1//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
	Col=192/8;
#endif
#if LCDPixels==0//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
	Col=0;
#endif
	Row=(ADDR_STRINGBUFF+(84*Row)+4);
//计算左开始空字符列数
	x=0;
	p16=(u16 *)(Row);
	for(i=0;i<Col;i++)
	{
		if(p16[i]&0x7fff)
		{
			break;
		}
		x+=1;
	}
//计算右开始空字符列数
	y=0;
	for(i=Col;i!=0x0;i--)
	{
		if(p16[i-1]&0x7fff)
		{
			break;
		}
		y+=1;
	}
	switch(Align)
	{
		case 1://左对齐
			if(x!=0x0)
			{
				if((x+y)<Col)
				{
					for(i=0;i<(Col-(x+y));i++)
					{
						p16[i]=p16[i+x];
					}
					for(i=0;i<x;i++)
					{
						p16[Col-(x+y)+i]=0;
					}
				}
			}
			if((AlignCol!=0)&&(AlignCol<Col))
			{
				for(i=0;i<(Col-AlignCol);i++)
				{
					p16[Col-1-i]=p16[Col-1-AlignCol-i];
				}
				for(i=0;i<AlignCol;i++)
				{
					p16[i]=0;
				}
			}
			break;
		case 2://居中
			if(x!=y)
			{
				if((x+y)<Col)
				{
					for(i=0;i<(Col-(x+y));i++)
					{
						p16[Col-1-i]=p16[Col-1-y-i];
					}
					for(i=0;i<y;i++)
					{
						p16[x+i]=0;
					}
				}
				x+=y;
				y=x/2;
				if(y!=0x0)
				{
					for(i=0;i<(Col-x);i++)
					{
						p16[y+i]=p16[x+i];
					}
					for(i=0;i<(x-y);i++)
					{
						p16[Col-1-i]=0;
					}
				}
			}
			break;
		case 3://右对齐
			if(y!=0x0)
			{
				if((x+y)<Col)
				{
					for(i=0;i<(Col-(x+y));i++)
					{
						p16[Col-1-i]=p16[Col-1-y-i];
					}
					for(i=0;i<y;i++)
					{
						p16[x+i]=0;
					}
				}
			}
			if((AlignCol+1)<Col)
			{
				for(i=0;i<(AlignCol+1);i++)
				{
					p16[i]=p16[Col-(AlignCol+1)+i];
				}
				for(i=0;i<(Col-(AlignCol+1));i++)
				{
					p16[(AlignCol+1)+i]=0;
				}
			}
			break;
	}
}

//---------------------------------------------------------------------------------------------------------
// Function name:       DisplayData
// Descriptions:        数显示
// input parameters:    Num要显BCD数,BitNum显示总位数,DpNum小数位数,p16字符显示缓冲指针
// output parameters:   ASCII字库显示
// Returned value:      下字符显示缓冲指针
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData(u64 Num,u32 BitNum,u32 DpNum,u16 * p16)//数据显示,入口指定要显BCD位数,小点数插入位等,使用ADDR_MRWBUFF
{
	u32 i;
	u8 * p8d;
  
	if(BitNum>LEN_MRWBUFF)
	{
		return p16;
	}
	p8d=(u8 *)(ADDR_MRWBUFF);
	for(i=0;i<BitNum;i++)
	{
		p8d[0]=(Num&0x0f);
		Num>>=4;
		p8d+=1;
	}
	p8d=(u8 *)(ADDR_MRWBUFF+BitNum-1);
	for(i=BitNum;i!=0x0;i--)
	{
		if(i==DpNum)
		{
			if(i==BitNum)
			{
				p16[0]='0'+0x2000;
				p16[1]='.'+0x2000;
				p16+=2;
			}
			else
			{
				p16[0]='.'+0x2000;
				p16+=1;
			}
		}
		if(p8d[0]>=10)
		{
			p16[0]=p8d[0]+(0x2041-10);
		}
		else
		{
			p16[0]=p8d[0]+0x2030;
		}
		p16+=1;
		p8d-=1;
	}
	return(p16);
}

//---------------------------------------------------------------------------------------------------------
// Function name:       DisplayData_OffZero
// Descriptions:        数显示
// input parameters:    Num要显BCD数,BitNum显示总位数,DpNum小数位数,p16字符显示缓冲指针
// output parameters:   ASCII字库显示,不显无效高位的0
// Returned value:      下字符显示缓冲指针
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData_OffZero(u64 Num,u32 BitNum,u32 DpNum,u16 * p16)//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MRWBUFF
{
	u32 i;
	u64 d64;
	u8 * p8d;
	d64=Num;
	p8d=(u8 *)(ADDR_MRWBUFF);
	if(BitNum>LEN_MRWBUFF)
	{
		return p16;
	}
	for(i=0;i<BitNum;i++)
	{
		p8d[0]=(Num&0x0f);
		Num>>=4;
		p8d+=1;
	}
	p8d=(u8 *)(ADDR_MRWBUFF+BitNum-1);
	while(BitNum>(DpNum+1))
	{
		if(p8d[0]!=0x0)
		{
			break;
		}
		p8d--;
		BitNum--;
	}
	p16=DisplayData(d64,BitNum,DpNum,p16);//ASCII显示数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MRWBUFF
	return(p16);
}


//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenuString
// Descriptions:        显示菜单字符串
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示菜单字符串
{
  u32 i;
  u32 x;
  u32 y;
  u32 Row;//字符串号
  u16 * pStrBuff;
  
	pStr=LanguageString(pStr);//按显示语言选择显示字符串
  i=0;
  Row=1;//菜单条号
  if(Col>=20)
  {
    Col=AutoIndent;
    Ln++;
  }
  if(Ln>=10)
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
      if(Ln>=10)
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
        if(Ln>=10)
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
        if(Ln>=10)
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
        if(Ln>=10)
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
// Function name:       DisplayMenuString
// Descriptions:        显示菜单字符串
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString_CSG(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//显示菜单字符串
{
	u32 i, x, y, Row;	//字符串号
	u16 * pStrBuff;

	pStr=LanguageString(pStr);//按显示语言选择显示字符串
	i=0;
	Row=1;//菜单条号
	if(Col>=20)
	{
		Col=AutoIndent;
		Ln++;
	}
	if(Ln >= 9)	//2017年11月09日菜单字符串显示到液晶第9行赵云
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
			if(Ln >= 9)//2017年11月09日菜单字符串显示到液晶第9行赵云
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
				if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
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
			  	if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
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
			    if(Ln>=9)//2012年02月08日菜单字符串显示到液晶第9行宋玉梅
			    {
			      return (Ln+(Col<<8)+(Row<<16));
			    }
			    pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
		  	}
		}
	}
}


void DisplayScrollUp(u32 Row)//显示上滚除第0行外,入口上滚行数
{
	u32 i;
	u8 *p8s;
	u8 *p8d;
	
	p8s=(u8*)(ADDR_STRINGBUFF+84+(Row*84));
	p8d=(u8*)(ADDR_STRINGBUFF+84);
	for(i=0;i<84*8;i++)
	{
		p8d[i]=p8s[i];
	}
	DisplayClrRow(10-Row,9);//清屏幕显示的指定行
}


u16 * DisplayData1(u64 Num1,u32 BitNum,u32 DpNum,u8 Num2,u16 * p16)	//Num2里放置". : -等符号"
{
	u32 i;
	u8 * p8d;
  
  if(BitNum>16)
  {
    BitNum=16;
  }
	p8d=(u8 *)(ADDR_MRWBUFF);
	for(i=0;i<BitNum;i++)
	{
		p8d[0]=(Num1&0x0f);
		Num1>>=4;
		p8d+=1;
	}
	p8d=(u8 *)(ADDR_MRWBUFF+BitNum-1);
	for(i=BitNum;i!=0x0;i--)
	{
		if(i==DpNum)
		{
			if(i==BitNum)
			{
				p16[0]='0'+0x2000;
				p16[1]=Num2+0x2000;
				p16+=2;
			}
			else
			{
				p16[0]=Num2+0x2000;
				p16+=1;
			}
		}
		if(p8d[0]>=10)
		{
			p16[0]=p8d[0]+(0x2041-10);
		}
		else
		{
			p16[0]=p8d[0]+0x2030;
		}
		p16+=1;
		p8d-=1;
	}
	return(p16);
}


void DisplayRam(u32 Ln,u32 RamAddr)//显示中RAM数据(调试用)
{
	u32 i;
	u8 * p8;
	u16 * p16;
	
	i=15;
	p8=(u8 *)(RamAddr);
	p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84));
	p16=DisplayData(RamAddr>>8,2,0,p16);//ASCII显示数据,入口指定要显BCD位数,小点数插入位等,使用IAPBUFF
	p16=DisplayData(RamAddr,2,0,p16);//ASCII显示数据,入口指定要显BCD位数,小点数插入位等,使用IAPBUFF
	p16[0]=0;
	p16+=1;
	while(i>=2)
	{
		p16=DisplayData(p8[0],2,0,p16);//ASCII显示数据,入口指定要显BCD位数,小点数插入位等,使用
		p16[0]=0;
		p16+=1;
		if(i>2)
		{
			i-=3;
		}
		else
		{
			i=0;
		}
		p8+=1;
	}
}

void DisplayRATE(u32 Ln,u32 Rate)//显示进度;入口:Ln=显示行,Rate=进度(hex,0-100%)
{
	u32 i;
	u32 x;
	u16 * p16;

	p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+4);//当前行的2列开始	
	if(Rate>100)
	{
		Rate=100;
	}
	if(Rate<=2)
	{
		p16[0]=46+Rate;
		p16++;
		for(i=0;i<12;i++)
		{
			p16[0]=49;//进度中框0
			p16++;
		}
		p16[0]=58;//'进度右框0'
	}
	else
	{
		if(Rate<=98)
		{
			p16[0]=48;//'进度左框2'
			p16++;
			i=(Rate-2)/8;
			x=12-i;
			for(;i!=0;i--)
			{
				p16[0]=57;//'进度中框8'
				p16++;
			}
			if(x!=0)
			{
				i=(Rate-2)%8;
				p16[0]=49+i;
				p16++;
				x--;
				for(;x!=0;x--)
				{
					p16[0]=49;//进度中框0
					p16++;
				}
			}
			p16[0]=58;//'进度右框0'
		}
		else
		{
			p16[0]=48;//'进度左框2'
			p16++;
			for(i=0;i<12;i++)
			{
				p16[0]=57;//'进度中框8'
				p16++;
			}
			i=Rate-98;
			p16[0]=58+i;//'进度右框0+i'
		}
	}
	//
	p16++;
	for(i=0;i<4;i++)
	{
		p16[i]=0;
	}
	Rate=hex_bcd(Rate);
	p16=DisplayData_OffZero(Rate,4,0,p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MRWBUFF
	p16[0]='%'+0x2000;
	
	LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
	Comm_Ram->DisplayScanTimer=10000/10;//10s
}



void Startdisplay(void)//起动显示
{
//	u16 *p16;
/*
	u32 *p32;
	p32=(u32*)(ADDR_AUTO_ERROR_RESTART);//自动检测到如SDRAM等错误看门狗复位标志
	if(p32[0]==0x12345678)
	{
		Pin_SCU(PIN_LCD_RST,0+(0<<3)+(1<<4)+(0<<5)+(1<<6)+(1<<17)+(1<<18));//解除复位LCD
		Comm_Ram->DisplayAllErrorTimer=0;//10;//上电全显或系统错误显示秒定时器
		return;
	}
*/
  DisplayString(0,2,0,(u8 *)"用电信息采集系统");//显示字符串
  StrBuff->Row[0].Font=1;
	
#if (Project/100)==0//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"智能表");//显示字符串
#endif
#if (Project/100)==1//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"无线智能表");//显示字符串
#endif
#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"南网Ⅰ型集中器");//显示字符串
#endif
#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	#if DLT500_REPORT == 1
		DisplayString(2,0,0,(u8 *)"配变监测计量终端");//显示字符串
	#else
		DisplayString(2,0,0,(u8 *)"负荷管理终端");//显示字符串
	#endif
#endif
#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"配变监测计量终端");//显示字符串
#endif
#if (Project/100)==5//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"南网Ⅱ型集中器");//显示字符串
#endif
#if (Project/100)==6//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
  DisplayString(2,0,0,(u8 *)"通信模块");//显示字符串
#endif
#if (Project/100)==9//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=采集器,600=通信模块
	  DisplayString(2,0,0,(u8 *)"配变监测计量终端");//显示字符串
#endif

	DispalyAlign(2,0,2);//行显示对齐(1左对齐,2居中,3右对齐)

//	DisplayString(3,0,0,(u8 *)"Q/GDW137x-2013");//显示字符串
//	DispalyAlign(3,0,2);//行显示对齐(1左对齐,2居中,3右对齐)


//	p16=(u16 *)(ADDR_STRINGBUFF+(4*84)+4);      //程序版本号
//	p16[0]='V'+0x2000;
//	p16[1]=((DefaultVersionPara.SoftWareVersion>>8)&0xf)+0x2030;
//	p16[2]='.'+0x2000;
//	p16[3]=((DefaultVersionPara.SoftWareVersion>>4)&0xf)+0x2030;
//	p16[4]=(DefaultVersionPara.SoftWareVersion&0xf)+0x2030;
//	DispalyAlign(4,0,2);//行显示对齐(1左对齐,2居中,3右对齐)


#if USER==3000//用户标识
  DisplayString(4,0,0,(u8 *)"标准版");//显示字符串
#endif

#if USER==3100//用户标识
  DisplayString(4,0,0,(u8 *)"广东版");//显示字符串
#endif

#if USER==3200//用户标识
  DisplayString(4,0,0,(u8 *)"广西版");//显示字符串
#endif

#if USER==3300//用户标识
  DisplayString(4,0,0,(u8 *)"贵州版");//显示字符串
#endif

#if USER==3400//用户标识
  DisplayString(4,0,0,(u8 *)"云南版");//显示字符串
#endif

#if USER==3500//用户标识
  DisplayString(4,0,0,(u8 *)"海南版");//显示字符串
#endif

	DispalyAlign(4,0,2);//行显示对齐(1左对齐,2居中,3右对齐)	
	
	
#if (USER/100)!=0//用户标识
  DisplayString(6,6,0,(u8 *)"正泰仪表");//显示字符串
#endif
  StrBuff->Row[6].Font=1;
	
	LCDDataOut();//点阵数据输出到LCD屏(约需7MS,cclk=96MHz)
}





















