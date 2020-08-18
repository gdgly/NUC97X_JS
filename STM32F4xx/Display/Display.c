
//��ʾ
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Display/Display.h"
#include "../Display/LCDModule.h"
#include "../Calculate/Calculate.h"


void DisplayClrRow(u32 StartRow,u32 EndRow)//����Ļ��ʾ��ָ����
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

void DisplayClr(void)//��ȫ��
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

u8* LanguageString(u8 *pStr)//����ʾ����ѡ����ʾ�ַ���
{
	u8 *p8;
	
	if(*pStr<=3)
	{//��ʼ�ֽ��Ƿ��ַ�
		if(*pStr<=1)
		{//ֻ��һ������
			pStr++;
			return pStr;
		}
		pStr++;
	}
	p8=(u8*)ADDR_RamLanguage;
	if(p8[0]==0)
	{//����
		return pStr;
	}
	//Ӣ��
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

void DisplayUpRow(u32 StartRow,u32 EndRow,u32 Row)//��ʾ��Ļָ���Ĵ�������ƽ��ָ��������;����:ƽ�ƺ󴰿ڵײ��ճ���������ʾ
{
	u32 i;
	u32 x;
	u32 *p32s;
	u32 *p32d;
	
	if(Row==0)
	{
		return;
	}
	//����������
	if(EndRow<StartRow)
	{
		return;
	}
	i=EndRow-StartRow;
	i++;
	if(Row>=i)
	{//��ƽ������>=����������
		DisplayClrRow(StartRow,EndRow);//����Ļ��ʾ��ָ����
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
	DisplayClrRow((EndRow+1)-Row,EndRow);//����Ļ��ʾ��ָ����
}

void DisplayStringAutoUp(u32 StartRow,u32 EndRow,u8 * pStr)//��ָ���������Զ�����ƽ�ƺ���ʾ�ַ���;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
{
  u32 i;
  u32 x;
  u32 y;
  u16 * pStrBuff;
	u32 Ln;
	u32 Col;
	
	pStr=LanguageString(pStr);//����ʾ����ѡ����ʾ�ַ���
	Ln=AutoDisplayRowCol->Row;
	Col=0;
	
	if(Ln>9)
	{
		DisplayUpRow(StartRow,EndRow,1);//��Ļ��ʾ��ָ��������ƽ��ָ��������;����:ƽ�ƺ�ײ��ճ���������
		Ln=9;
	}
	DisplayClrRow(Ln,9);//����Ļ��ʾ��ָ����
  i=0;
  pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
  while(1)
  {
    if(pStr[i]==0)
    {//����
			if(Col!=0)
			{
				Ln++;
			}
			AutoDisplayRowCol->Row=Ln;
			
			LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
			Comm_Ram->DisplayScanTimer=10000/10;//10s
      return;
    }
    if(pStr[i]=='\n')
    {//����
      i++;
      Ln++;
      if(Ln>9)
      {
				DisplayUpRow(StartRow,EndRow,1);//��Ļ��ʾ��ָ��������ƽ��ָ��������;����:ƽ�ƺ�ײ��ճ���������
				Ln=9;
				Col=0;
      }
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      continue;
    }
    if(pStr[i]=='\r')
    {//�س�
      i++;
      Col=0;
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      continue;
    }
    x=pStr[i];
    i++;
//����GB2312_1980ɾ���հ�����ת��Ϊ����LCD���ֿ�(HZK16_LCD)
//1-9��(A1-A9)������,682��
//10-81��(B0-F7)������,6763��(����(6763+682)*2=14890������ַ�)
//ÿ������Ϊ94*2������ַ�
    if(((x>=0xA1)&&(x<=0xA9))||((x>=0xB0)&&(x<=0xF7)))
    {//��>=0xA1A1�����
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
      //StrBuff->Row[Ln].Row=Row;//�ַ�����
      Col++;
			if(Col>=20)
			{//����
        pStrBuff[0]=0;
				Col=0;
        Ln++;
        if(Ln>9)
        {
          i--;
					DisplayUpRow(StartRow,EndRow,1);//��Ļ��ʾ��ָ��������ƽ��ָ��������;����:ƽ�ƺ�ײ��ճ���������
					Ln=9;
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
        pStrBuff[0]=x;
			}
			x+=1;
			pStrBuff[1]=x;
      //StrBuff->Row[Ln].Row=Row;//�ַ�����
      pStrBuff+=2;
      Col++;
			if(Col>=20)
			{//����
				Col=0;
        Ln++;
        if(Ln>9)
        {
					if(pStr[i]!=0)
					{
						DisplayUpRow(StartRow,EndRow,1);//��Ļ��ʾ��ָ��������ƽ��ָ��������;����:ƽ�ƺ�ײ��ճ���������
						Ln=9;
					}
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
			}
    }
    else
    {
      //ASCII�ֿ�
      pStrBuff[0]=x+0x2000;
      //StrBuff->Row[Ln].Row=Row;//�ַ�����
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//����
				Col=0;
        Ln++;
        if(Ln>9)
        {
					if(pStr[i]!=0)
					{
						DisplayUpRow(StartRow,EndRow,1);//��Ļ��ʾ��ָ��������ƽ��ָ��������;����:ƽ�ƺ�ײ��ճ���������
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
// Descriptions:        ��ʾ�ַ���
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�Զ�����ʱ��������,�ַ���ָ��(0Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B23��������,B24-B31����
//-------------------------------------------------------------------------------------------------
u32 DisplayString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//��ʾ�ַ���
{
  u32 i;
  u32 x;
  u32 y;
  u32 Row;//�ַ�����
  u16 * pStrBuff;
  
	if(Ln>9)
	{
		return (Ln+(Col<<8)+(0<<16));
	}
	pStr=LanguageString(pStr);//����ʾ����ѡ����ʾ�ַ���
  i=0;
  Row=1;//�˵�����
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
    {//����
      i++;
      return (Ln+(Col<<8)+(Row<<16));
    }
    if(pStr[i]=='\n')
    {//����
      i++;
      Ln++;
      if(Ln>=10)
      {
        return (Ln+(Col<<8)+(Row<<16));
      }
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      Row++;//�ַ�����
      continue;
    }
    if(pStr[i]=='\r')
    {//�س�
      i++;
      Col=0;
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      continue;
    }
    x=pStr[i];
    i++;
//����GB2312_1980ɾ���հ�����ת��Ϊ����LCD���ֿ�(HZK16_LCD)
//1-9��(A1-A9)������,682��
//10-81��(B0-F7)������,6763��(����(6763+682)*2=14890������ַ�)
//ÿ������Ϊ94*2������ַ�
    if(((x>=0xA1)&&(x<=0xA9))||((x>=0xB0)&&(x<=0xF7)))
    {//��>=0xA1A1�����
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
      //StrBuff->Row[Ln].Row=Row;//�ַ�����
      Col++;
			if(Col>=20)
			{//����
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
      //StrBuff->Row[Ln].Row=Row;//�ַ�����
      pStrBuff+=2;
      Col++;
			if(Col>=20)
			{//����
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
      //ASCII�ֿ�
      pStrBuff[0]=x+0x2000;
      //StrBuff->Row[Ln].Row=Row;//�ַ�����
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//����
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

//---------------------------------------------------------------------------------------------------------
// Function name:       DispalyStringOnlyOneRow
// Descriptions:        ��ʾ�ַ�������1����
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�ַ���ָ��(\n,\r,0��Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B31�������ַ���
//---------------------------------------------------------------------------------------------------------
u32 DispalyStringOnlyOneRow(u32 Row,u32 Col,u8 * pStr)//��ʾ�ַ�������1����
{
  u32 i;
  u32 x;
  u32 y;
  u16 * pStrBuff;
  
	if(Row>9)
	{
		return (Row+(Col<<8)+(0<<16));
	}
	pStr=LanguageString(pStr);//����ʾ����ѡ����ʾ�ַ���
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
    {//��>=0xA1A1�����
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
			{//����
				return (Row+(Col<<8)+(i<<16));
			}
			x+=1;
			pStrBuff[1]=x;
      pStrBuff+=2;
      Col++;
			if(Col>=20)
			{//����
				return (Row+(Col<<8)+(i<<16));
			}
    }
    else
    {
      //ASCII�ֿ�
      pStrBuff[0]=x+0x2000;
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//����
	  		return (Row+(Col<<8)+(i<<16));
	  	}
    }
  }
}


u16 * DispalyStringContinue(u16 *pStrBuff,u8 * pStr)//��ʾ�ַ�������
{
  u32 i;
  u32 x;
  u32 y;
  u32 Col;
	
	pStr=LanguageString(pStr);//����ʾ����ѡ����ʾ�ַ���
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
    {//��>=0xA1A1�����
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
			{//����
				return (pStrBuff);
			}
			x+=1;
			pStrBuff[1]=x;
      pStrBuff+=2;
      Col++;
			if(Col>=20)
			{//����
				return (pStrBuff);
			}
    }
    else
    {
      //ASCII�ֿ�
      pStrBuff[0]=x+0x2000;
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//����
	  		return (pStrBuff);
	  	}
    }
  }
}



//---------------------------------------------------------------------------------------------------------
// Function name:       DispalyAlign
// Descriptions:        ��ʾ����(�����,����,�Ҷ���)
// input parameters:    Row��������,AlignCol��ʾ���뵽�к�(160*160�����к�0-19,�����������,�Ҷ���),Align���뷽ʽ:1=�����,2=����,3=�Ҷ���
// output parameters:   ��ʾ����(������������ݶ���)
// Returned value:      ��
//---------------------------------------------------------------------------------------------------------
void DispalyAlign(u32 Row,u32 AlignCol,u32 Align)//����ʾ����(1�����,2����,3�Ҷ���)
{
	u32 i;
	u32 x;
	u32 y;
	u32 Col;
	u16 * p16;

#if LCDPixels==3//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE
	Col=160/8;
#endif
#if LCDPixels==2//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE
	Col=320/8;
#endif
#if LCDPixels==1//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE
	Col=192/8;
#endif
#if LCDPixels==0//����LCD���� 3=160160,2=320240,1=19264,0=SEGMODE
	Col=0;
#endif
	Row=(ADDR_STRINGBUFF+(84*Row)+4);
//������ʼ���ַ�����
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
//�����ҿ�ʼ���ַ�����
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
		case 1://�����
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
		case 2://����
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
		case 3://�Ҷ���
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
// Descriptions:        ����ʾ
// input parameters:    NumҪ��BCD��,BitNum��ʾ��λ��,DpNumС��λ��,p16�ַ���ʾ����ָ��
// output parameters:   ASCII�ֿ���ʾ
// Returned value:      ���ַ���ʾ����ָ��
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData(u64 Num,u32 BitNum,u32 DpNum,u16 * p16)//������ʾ,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MRWBUFF
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
// Descriptions:        ����ʾ
// input parameters:    NumҪ��BCD��,BitNum��ʾ��λ��,DpNumС��λ��,p16�ַ���ʾ����ָ��
// output parameters:   ASCII�ֿ���ʾ,������Ч��λ��0
// Returned value:      ���ַ���ʾ����ָ��
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData_OffZero(u64 Num,u32 BitNum,u32 DpNum,u16 * p16)//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MRWBUFF
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
	p16=DisplayData(d64,BitNum,DpNum,p16);//ASCII��ʾ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MRWBUFF
	return(p16);
}


//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenuString
// Descriptions:        ��ʾ�˵��ַ���
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�Զ�����ʱ��������,�ַ���ָ��(0Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B23��������,B24-B31����
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//��ʾ�˵��ַ���
{
  u32 i;
  u32 x;
  u32 y;
  u32 Row;//�ַ�����
  u16 * pStrBuff;
  
	pStr=LanguageString(pStr);//����ʾ����ѡ����ʾ�ַ���
  i=0;
  Row=1;//�˵�����
  if(Col>=20)
  {
    Col=AutoIndent;
    Ln++;
  }
  if(Ln>=10)
//  if(Ln>=9)//2012��02��08�ղ˵��ַ�����ʾ��Һ����9������÷
  {
    return (Ln+(Col<<8)+(Row<<16));
  }
  pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
  while(1)
  {
    if(pStr[i]==0)
    {//����
      i++;
      return (Ln+(Col<<8)+(Row<<16));
    }
    if(pStr[i]=='\n')
    {//����
      i++;
	  if( Col != AutoIndent )//2012��02��09�ձ���������⣺�պ�20���ַ���Һ���϶���ʾһ����������÷
      Ln++;
      if(Ln>=10)
//	  if(Ln>=9)//2012��02��08�ղ˵��ַ�����ʾ��Һ����9������÷
      {
        return (Ln+(Col<<8)+(Row<<16));
      }
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      Row++;//�ַ�����
      continue;
    }
    if(pStr[i]=='\r')
    {//�س�
      i++;
      Col=0;
      pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
      continue;
    }
    x=pStr[i];
    i++;
//    if(x>0xA1)
    if(x>=0xA1)	 //20120221����÷
    {//��>=0xA1A1�����
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
      StrBuff->Row[Ln].Row=Row;//�ַ�����
      Col++;
	  if(Col>=20)
	  {//����
          pStrBuff[0]=0;
	  	  Col=AutoIndent;
          Ln++;
        if(Ln>=10)
//	  	  if(Ln>=9)//2012��02��08�ղ˵��ַ�����ʾ��Һ����9������÷
          {
            i--;
            return (Ln+(Col<<8)+(Row<<16));
          }
          pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
          pStrBuff[0]=x;
	  }
	  x+=1;
	  pStrBuff[1]=x;
      StrBuff->Row[Ln].Row=Row;//�ַ�����
      pStrBuff+=2;
      Col++;
	  if(Col>=20)
	  {//����
	 	  Col=AutoIndent;
          Ln++;
        if(Ln>=10)
//	      if(Ln>=9)//2012��02��08�ղ˵��ַ�����ʾ��Һ����9������÷
          {
            return (Ln+(Col<<8)+(Row<<16));
          }
          pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
	  }
    }
    else
    {
      //ASCII�ֿ�
      pStrBuff[0]=x+0x2000;
      StrBuff->Row[Ln].Row=Row;//�ַ�����
      pStrBuff++;
      Col++;
	    if(Col>=20)
	  	{//����
				Col=AutoIndent;
        Ln++;
        if(Ln>=10)
//	    if(Ln>=9)//2012��02��08�ղ˵��ַ�����ʾ��Һ����9������÷
        {
          return (Ln+(Col<<8)+(Row<<16));
        }
        pStrBuff=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+(Col*2));
	  	}
    }
  }
}

void DisplayScrollUp(u32 Row)//��ʾ�Ϲ�����0����,����Ϲ�����
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
	DisplayClrRow(10-Row,9);//����Ļ��ʾ��ָ����
}

u16 * DisplayData1(u64 Num1,u32 BitNum,u32 DpNum,u8 Num2,u16 * p16)	//Num2�����". : -�ȷ���"
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


void DisplayRam(u32 Ln,u32 RamAddr)//��ʾ��RAM����(������)
{
	u32 i;
	u8 * p8;
	u16 * p16;
	
	i=15;
	p8=(u8 *)(RamAddr);
	p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84));
	p16=DisplayData(RamAddr>>8,2,0,p16);//ASCII��ʾ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��IAPBUFF
	p16=DisplayData(RamAddr,2,0,p16);//ASCII��ʾ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��IAPBUFF
	p16[0]=0;
	p16+=1;
	while(i>=2)
	{
		p16=DisplayData(p8[0],2,0,p16);//ASCII��ʾ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��
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

void DisplayRATE(u32 Ln,u32 Rate)//��ʾ����;���:Ln=��ʾ��,Rate=����(hex,0-100%)
{
	u32 i;
	u32 x;
	u16 * p16;

	p16=(u16 *)(ADDR_STRINGBUFF+4+(Ln*84)+4);//��ǰ�е�2�п�ʼ	
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
			p16[0]=49;//�����п�0
			p16++;
		}
		p16[0]=58;//'�����ҿ�0'
	}
	else
	{
		if(Rate<=98)
		{
			p16[0]=48;//'�������2'
			p16++;
			i=(Rate-2)/8;
			x=12-i;
			for(;i!=0;i--)
			{
				p16[0]=57;//'�����п�8'
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
					p16[0]=49;//�����п�0
					p16++;
				}
			}
			p16[0]=58;//'�����ҿ�0'
		}
		else
		{
			p16[0]=48;//'�������2'
			p16++;
			for(i=0;i<12;i++)
			{
				p16[0]=57;//'�����п�8'
				p16++;
			}
			i=Rate-98;
			p16[0]=58+i;//'�����ҿ�0+i'
		}
	}
	//
	p16++;
	for(i=0;i<4;i++)
	{
		p16[i]=0;
	}
	Rate=hex_bcd(Rate);
	p16=DisplayData_OffZero(Rate,4,0,p16);//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MRWBUFF
	p16[0]='%'+0x2000;
	
	LCDDataOut();//�������������LCD��(Լ��7MS,cclk=96MHz)
	Comm_Ram->DisplayScanTimer=10000/10;//10s
}

























