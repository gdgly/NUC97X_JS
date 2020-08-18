

#include "Project.h"
#include "Menu.h"
#include "../Display/Display.h"
#include "string.h"
#include "../Display/Draw.h"
#include "../Device/MEMRW.h"







#if (((Project/100)==3&&(USER/100)==5) || ((USER/100)==13) || ((USER/100)==11))//�Ϻ�ר�䡢������������

//-------------------------------------------------------------------------------------------------
// Function name:       NdigitSelect_SH
// Descriptions:        Nλ������ѡ��
// input parameters:    Nλ�����༭���к�Col����ѡ��Χ��С��Min����ѡ��Χ�����Max
// output parameters:   ��
// Returned value:      ��
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
			{//�Ҽ�
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
			{//���
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
			{//ȷ��
				Stack->MenuStack[MenuCount->Count].EditCol=Col;
				DisplayString(Ln,16,0,"����");
				Comm_Ram->ButtonVal&=~0x1C;//��ʹ�ú���0
			}
			else
			{
				Comm_Ram->ButtonVal&=~0x0C;//��ʹ�ú���0
				break;
			}
		}
		else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
		{
			Select_SH(Ln,Col+N-1,Col);
			UpDownSelect_SH(Ln);
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
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
				Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
			}
			else
			{
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				MenuCount->v2=p8_bcd(p8,N);
				MenuCount->v1=0;
				Comm_Ram->ButtonVal&=~0x0F;//��ʹ�ú���0
				break;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       Select_SH
// Descriptions:        ʮ����������ѡ��
// input parameters:    �ַ����ұ߽磬�ַ�����߽�
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120213
// Date Modified��      20120301
//-------------------------------------------------------------------------------------------------
void Select_SH(u32 Ln,u32 MaxCol,u32 MinCol)
{
	u16 *p16;
	u32 x;
	u32 i;

	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//���Ƽ�����
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
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//ֻ�����֡�>��ѡ�У���������ȫ������
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
	{//���Ƽ�����
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
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//ֻ�����֡�>��ѡ�У���������ȫ������
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
// Descriptions:        ʮ����������ѡ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void UpDownSelect_SH(u32 Ln)//ʮ����������ѡ��
{
	u16 *p16;
	u32 x;

	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//���Ƽ�����
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
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//��ʾ�ַ���
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//���Ƽ�����
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
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//��ʾ�ַ���
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
}
//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenuString_SH
// Descriptions:        �Ϻ�ר������ʾ�˵��ַ���
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�Զ�����ʱ��������,�ַ���ָ��(0Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B23��������,B24-B31����
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString_SH(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//��ʾ�˵��ַ���
{
  u32 i;
  u32 x;
  u32 y;
  u32 Row;//�ַ�����
  u16 * pStrBuff;
  u32 MenuMaxLn;//20140305�˵���ʾ��Һ����MenuMaxLn������÷
  
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
//�Ϻ�Ҫ�󣺲˵���ʾ�ӵ�2����9�У�Һ������������Ϊ��ʾͼ���״̬��
	MenuMaxLn=9;
#else
	MenuMaxLn=10;
#endif

  i=0;
  Row=1;//�˵�����
  if(Col>=20)
  {
    Col=AutoIndent;
    Ln++;
  }
  if(Ln>=MenuMaxLn)
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
      if(Ln>=MenuMaxLn)
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
        if(Ln>=MenuMaxLn)
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
        if(Ln>=MenuMaxLn)
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
        if(Ln>=MenuMaxLn)
//	    if(Ln>=9)//2012��02��08�ղ˵��ַ�����ʾ��Һ����9������÷
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
// Descriptions:        ��ʾ�˵�
// input parameters:    �˵���������,�ַ���ָ��(\n������,\r����0,0����)
// output parameters:   ������ʾ��������
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void DisplayMenu_SH(u32 Ln,u32 MoveRow,u8 * pStr)//��ʾ�˵�
{
	u32 i;
	u32 TotalRow;//�˵��ַ��������в˵��м���(�Ʊ�����)
	u8 * p8s;//��ʼ��ָ��
	
	//��ȫ��
	DisplayClr();//��ȫ��
	Draw_Clr();
	//����˵�������������ʼ��ָ��
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
	
	//װ��˵�
	//������
	DispalyStringOnlyOneRow(Ln,0,pStr);//��ʾ�ַ�������1����
	DispalyAlign(Ln,0,2);//����ʾ����(1�����,2����,3�Ҷ���) 
	//��ʾ�˵���
	if(i!=0)
	{
		i=DisplayMenuString(Ln+1,0,2,p8s);//��ʾ�˵��ַ���
		Stack->MenuStack[MenuCount->Count].MenuRow=((i>>16)&0xff)+1;
	}
	Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
}

#endif

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenu
// Descriptions:        ��ʾ�˵�
// input parameters:    �˵���������,�ַ���ָ��(\n������,\r����0,0����)
// output parameters:   ������ʾ��������
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void DisplayMenu(u32 MoveRow,u8 * pStr)//��ʾ�˵�
{
	u32 i;
	u32 TotalRow;//�˵��ַ��������в˵��м���(�Ʊ�����)
	u8 * p8s;//��ʼ��ָ��
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  
	
	//��ȫ��
	DisplayClr();//��ȫ��
	Draw_Clr();
	//����˵�������������ʼ��ָ��
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
	
	//װ��˵�
	//������
	DispalyStringOnlyOneRow(Ln,0,pStr);//��ʾ�ַ�������1����
	DispalyAlign(Ln,0,2);//����ʾ����(1�����,2����,3�Ҷ���) 
	//��ʾ�˵���
	if(i!=0)
	{
#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
		i=DisplayMenuString_SH(Ln+1,0,2,p8s);//��ʾ�˵��ַ���
#else
		i=DisplayMenuString(Ln+1,0,2,p8s);//��ʾ�˵��ַ���
#endif
		Stack->MenuStack[MenuCount->Count].MenuRow=((i>>16)&0xff)+1;
	}
	Comm_Ram->DisplayScanTimer=0;//2 ��ʾɨ��10MS��ʱ��
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuIn
// Descriptions:        �����¼��˵�
// input parameters:    �¼��˵�������ַ
// output parameters:   ��ʼ���¼��˵��������������
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuCreate(u32 FunctionAddr)//�����¼��˵�
{
	if(MenuCount->Count>=MaxMenuCount)
	{
		return;
	}
	MenuCount->Count++; 
	Stack->MenuStack[MenuCount->Count].PC=FunctionAddr;//�˵��������е�ַ(�ر�ע��:��ֵ�������Ķ�)
	Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
	Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
	Stack->MenuStack[MenuCount->Count].EditRow=1;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
	Stack->MenuStack[MenuCount->Count].EditCol=0;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuDel
// Descriptions:        �����ϼ��˵�
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuDel(void)//�����ϼ��˵�
{
	if(MenuCount->Count!=0)
	{
		MenuCount->Count--;
	}
	Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
	Comm_Ram->DisplayScanTimer=10;//2 ��ʾɨ��10MS��ʱ��
}

//-------------------------------------------------------------------------------------------------
// Function name:       MenuNegative
// Descriptions:        ���Բ˵���
// input parameters:    �к�0-9
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuNegative(u32 Row)//���Բ˵���
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
// Descriptions:        �˵�ѡ��
// input parameters:    ��
// output parameters:   �޸�Stack->MenuStack[MenuCount->Count].EditRow
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void MenuSelect(void)//�˵�ѡ��
{
	u32 i;
	u32 Row;
	
	
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//���Ƽ�����
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
			MenuNegative(Row);//���Բ˵���
		}
		else
		{
			if((Stack->MenuStack[MenuCount->Count].MoveRow+Stack->MenuStack[MenuCount->Count].MenuRow)<Stack->MenuStack[MenuCount->Count].TotalRow)
			{
				Stack->MenuStack[MenuCount->Count].MoveRow++;
				Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
			}
		}
	}
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//���Ƽ�����
		Row=Stack->MenuStack[MenuCount->Count].EditRow;
		if(Row==1)
		{
			i=Stack->MenuStack[MenuCount->Count].MoveRow;//�˵���������(����ʱ��Ϊ0,����ʱ����)
			if(i!=0x0)
			{
				i--;
				Stack->MenuStack[MenuCount->Count].MoveRow=i;//�˵���������(����ʱ��Ϊ0,����ʱ����)
				Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
			}
		}
		else
		{
			Row--;
			Stack->MenuStack[MenuCount->Count].EditRow=Row;
			MenuNegative(Row);//���Բ˵���
		}
	}   
}

//-------------------------------------------------------------------------------------------------
// Function name:       UpArrow
// Descriptions:        ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void UpArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif						  

	DisplayString(Ln,18,0,"��");
}

//-------------------------------------------------------------------------------------------------
// Function name:       DownArrow
// Descriptions:        ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void DownArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=8;//�˵��ڵ�8�н���
#else
	Ln=9;//�˵��ڵ�9�н���
#endif						  

	DisplayString(Ln,18,0,"��");
}

//-------------------------------------------------------------------------------------------------
// Function name:       LeftArrow
// Descriptions:        ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void LeftArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	DisplayString(Ln,0,0,"��");
}

//-------------------------------------------------------------------------------------------------
// Function name:       RightArrow
// Descriptions:        ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void RightArrow(void)
{
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=8;//�˵��ڵ�8�н���
#else
	Ln=9;//�˵��ڵ�9�п�ʼ
#endif

	DisplayString(Ln,0,0,"��");
}

//-------------------------------------------------------------------------------------------------
// Function name:       Arrowhead
// Descriptions:        ������һ��ʱ����Ļ���½���ʾ���µļ�ͷ������һ��ʱ����Ļ���Ͻ���ʾ���ϵļ�ͷ
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120211
// Date Modified��		20120221
//-------------------------------------------------------------------------------------------------
void Arrowhead(void)//������һ��ʱ����Ļ���½���ʾ���µļ�ͷ������һ��ʱ����Ļ���Ͻ���ʾ���ϵļ�ͷ
{
//20120211 ������һ��ʱ����Ļ���½���ʾ���µļ�ͷ ����÷
	if(Stack->MenuStack[MenuCount->Count].TotalRow-(Stack->MenuStack[MenuCount->Count].MenuRow+Stack->MenuStack[MenuCount->Count].MoveRow)>0)
	{
		DownArrow();
	}			 
//20120211 ������һ��ʱ����Ļ���Ͻ���ʾ���ϵļ�ͷ ����÷	  
	if(Stack->MenuStack[MenuCount->Count].MoveRow!=0)
	{
		UpArrow();
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeRowCol
// Descriptions:        ����ָ�����к���
// input parameters:    �к�0-9���к�0-19
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120213
//-------------------------------------------------------------------------------------------------
void NegativeRowCol(u32 Row,u32 Col)//���ַ������йأ��˵���
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
			*p16|=0x8000;//����ָ���С��� ����÷
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnCol
// Descriptions:        ����ָ�����к���
// input parameters:    �к�0-9���к�0-19
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
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
	*p16|=0x8000;//����ָ���С��� ����÷
}

//-------------------------------------------------------------------------------------------------
// Function name:       Select
// Descriptions:        ʮ����������ѡ��
// input parameters:    �ַ����ұ߽磬�ַ�����߽�
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120213
// Date Modified��      20120301
//-------------------------------------------------------------------------------------------------
void Select(u32 MaxCol,u32 MinCol)
{
	u16 *p16;
	u32 x;
	u32 i;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//���Ƽ�����
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
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//ֻ�����֡�>��ѡ�У���������ȫ������
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
	{//���Ƽ�����
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
			if((x<0x30 && x!=0x3e) || (x>0x39 && x!=0x3e))//ֻ�����֡�>��ѡ�У���������ȫ������
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
// Descriptions:        ʮ������������ѡ��
// input parameters:    �ַ����ұ߽磬�ַ�����߽�
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120427
//-------------------------------------------------------------------------------------------------
void SelectH(u32 MaxCol,u32 MinCol)
{
	u16 *p16;
	u32 x;
	u32 i;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//���Ƽ�����
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
			if((x<0x30 && x!=0x3e) || (x>0x39 && x<0x41 && x!=0x3e) || (x>0x46 && x!=0x3e))//ֻ��ʮ���������֡�>��ѡ�У���������ȫ������
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
	{//���Ƽ�����
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
			if((x<0x30 && x!=0x3e) || (x>0x39 && x<0x41 && x!=0x3e) || (x>0x46 && x!=0x3e))//ֻ��ʮ���������֡�>��ѡ�У���������ȫ������
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
// Descriptions:        ����
// input parameters:    ��һ��������ַ
// output parameters:   ��ʼ���¼��˵��������������
// Returned value:      ��
// Author:              ����÷
// Date:                20120220
//-------------------------------------------------------------------------------------------------
void CreateScreen(u32 FunctionAddr)//����
{
	Stack->MenuStack[MenuCount->Count].PC=FunctionAddr;//�˵��������е�ַ(�ر�ע��:��ֵ�������Ķ�)
	Stack->MenuStack[MenuCount->Count].Task=0;//�˵��������(������˳�ʱ����Ϊ0)
	Stack->MenuStack[MenuCount->Count].MoveRow=0;//�˵���������(����ʱ��Ϊ0,����ʱ����)
	Stack->MenuStack[MenuCount->Count].EditRow=1;//�˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
	Stack->MenuStack[MenuCount->Count].EditCol=0;//�˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
}


//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnMaxColMinCol
// Descriptions:        ���Գ���MinCol~MaxCol�������
// input parameters:    �к�0-9���к�
// output parameters:   ��
// Returned value:      ��
// Author:              ����÷
// Date:                20120310
//-------------------------------------------------------------------------------------------------
void NegativeLnMaxColMinCol(u32 Row,u32 MaxCol,u32 MinCol)//���ַ������й�
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
				*p16|=0x8000;//����ָ���С��� ����÷    
			}
			for(i=MaxCol+1;i<20;i++)
			{
				p16=(u16 *)(ADDR_STRINGBUFF+(Ln*84)+4+i*2);
				*p16|=0x8000;//����ָ���С��� ����÷    
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnMaxColMinColN
// Descriptions:        ���Գ���MinCol~MaxCol�������
// input parameters:    �к�0-9���к�
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void NegativeLnMaxColMinColN(u32 Row,u32 MaxCol,u32 MinCol)//���ַ������޹�
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
		*p16|=0x8000;//����ָ���С��� ����÷    
	}
	for(i=MaxCol+1;i<20;i++)
	{
		p16=(u16 *)(ADDR_STRINGBUFF+(Row*84)+4+i*2);
		*p16|=0x8000;//����ָ���С��� ����÷    
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       BoxSelect
// Descriptions:        �༭��ѡ��
// input parameters:    ��
// output parameters:   �޸�Stack->MenuStack[MenuCount->Count].EditRow
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void BoxSelect(void)//�༭��ѡ��
{
	u32 i;
	u32 Row;
	
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//���Ƽ�����
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
				Stack->MenuStack[MenuCount->Count].Task=2;//���ڲ˵�8_9
			}
		}
	}
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//���Ƽ�����
		Row=Stack->MenuStack[MenuCount->Count].EditRow;
		if(Row==1)
		{
			i=Stack->MenuStack[MenuCount->Count].MoveRow;//�˵���������(����ʱ��Ϊ0,����ʱ����)
			if(i!=0x0)
			{
				i--;
				Stack->MenuStack[MenuCount->Count].MoveRow=i;//�˵���������(����ʱ��Ϊ0,����ʱ����)
				Stack->MenuStack[MenuCount->Count].Task=2;//���ڲ˵�8_9
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
// Descriptions:        ����ѡ��
// input parameters:    ��������С�к�MinLn������������к�MaxLn�������̵����һ������к�LastLnMaxCol����������С�к�MinCol������������к�MaxCol���༭���к�EditLn���༭����ʼ�к�EditCol
// output parameters:   
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void KeyboardSelect(u32 MinLn,u32 MaxLn,u32 LastLnMaxCol,u32 MinCol,u32 MaxCol,u32 EditLn,u32 EditCol)//����ѡ��
{
	u16 *p16;
	u32 x;

	if((Comm_Ram->ButtonVal&0x08)!=0)
	{//���Ƽ�����
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
	{//���Ƽ�����
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
	{//���¼�����
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
	{//���ϼ�����
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
	{//ȷ��
		p16=(u16 *)(ADDR_STRINGBUFF+(Stack->MenuStack[MenuCount->Count].EditRow)*84+4+(Stack->MenuStack[MenuCount->Count].EditCol)*2);
		*p16&=0x7FFF;
		memcpy(&x,p16,2);
		x&=0x00ff;
		DisplayString(EditLn,EditCol+Stack->MenuStack[MenuCount->Count].MoveRow,0, (u8 *)&x);//��ʾ�ַ���
		Stack->MenuStack[MenuCount->Count].Task = 2;
	}
	p16=(u16 *)(ADDR_STRINGBUFF+((EditLn)*84)+4+(EditCol+Stack->MenuStack[MenuCount->Count].MoveRow)*2);
	*p16|=0x8000;
	if((Comm_Ram->ButtonVal&0x20)!=0)
	{//����
		Stack->MenuStack[MenuCount->Count].MoveRow =0;
		MenuDel();//�����ϼ��˵�
	}
	Comm_Ram->ButtonVal&=~0x3F;//��ʹ�ú���0
}

//-------------------------------------------------------------------------------------------------
// Function name:      bcd_p8       
// Descriptions:       ��ʮ������x�ŵ���ʾ������*p8��        
// input parameters:   ��ʾ������*p8,ʮ������x,��ʾ��λ��z    
// output parameters:  ��   
// Returned value:     ��      
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
// Descriptions:       ��ʮ������x�ŵ���ʾ������*p16��        
// input parameters:   ��ʾ������*p16,ʮ������x,��ʾ��λ��z    
// output parameters:  ��   
// Returned value:     ��      
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
// Descriptions:       ����ʾ������*p8�е���ת��Ϊʮ������x               
// input parameters:   ��ʾ������*p8��ת�����λ��z       
// output parameters:  ��   
// Returned value:     ת�����ʮ������x      
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
// Descriptions:       ����ʾ������*p8�е�����1              
// input parameters:   ��ʾ������*p8����ʾ����λ��z       
// output parameters:  ��   
// Returned value:     ��      
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
// Descriptions:       ����ʾ������*p8�е�����1              
// input parameters:   ��ʾ������*p8����ʾ����λ��z       
// output parameters:  ��   
// Returned value:     ��      
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
// Function name:      MeterPnCompare       
// Descriptions:       �Ƚ����õĲ��������F10��������Ƿ��ظ�������1�ظ���0���ظ�              
// input parameters:   ���õ�װ�����N1���������Pn������������ʱ��N1=MaxRS485AddCarrierPn+1��������װ����Ŷ�Ҫ�Ƚϡ���       
// output parameters:  ��   
// Returned value:     1�ظ���0���ظ�      
//-------------------------------------------------------------------------------------------------

u32 MeterPnCompare(u32 N1,u32 Pn)
{
	u32 i;

	for(i=0;i<MaxRS485AddCarrierPn;i++)
	{
		if(i+1!=N1 && Pn!=0 && Pn == MRR(ADDR_AFN04F10+27*i+2,2))//װ�������������
		{
			break;
		}	
	}
	if(i!=MaxRS485AddCarrierPn)
	{//�������ظ�
		return 1;
	}
	else
	{//�����㲻�ظ�
		return 0;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:      PulsePnCompare       
// Descriptions:       �Ƚ����õĲ��������F11��������Ƿ��ظ�������1�ظ���0���ظ�              
// input parameters:   ���õ�����˿�N1���������Pn��������װ��ʱ��N1=MaxImpInPort+1������������˿ڶ�Ҫ�Ƚϡ���       
// output parameters:  ��   
// Returned value:     1�ظ���0���ظ�      
//-------------------------------------------------------------------------------------------------

u32 PulsePnCompare(u32 N1,u32 Pn)
{
#if MaxImpInPort
	u32 i;

	for(i=0;i<MaxImpInPort;i++)
	{
		if(i+1!=N1 && Pn!=0 && Pn == MRR(ADDR_AFN04F11+5*i+1,1))//����˿������������
		{
			break;
		}
	}
	if(i!=MaxImpInPort)
	{//�������ظ�
		return 1;
	}
	else
	{//�����㲻�ظ�
		return 0;
	}
#else
	return 0;
#endif
}

//-------------------------------------------------------------------------------------------------
// Function name:      Event       
// Descriptions:       ��ѯ���������Ҫ/��ͨ�¼�����                     
// input parameters:   m=1��Ҫ�¼���m=0һ���¼����¼���¼����ERC�����n��      
// output parameters:  ��   
// Returned value:     �������n����Ҫ/��ͨ�¼�ERC��λ��          
//-------------------------------------------------------------------------------------------------

u32 Event(u32 m,u32 ERC,u32 n)
{
	u8 *a;
	u32 i;
	u32 j=0;
	u32 EC;
	u32 ADDR1;
	u32 ADDR2;

	MC(0,ADDR_DATABUFF,256);
	a=(u8 *)ADDR_DATABUFF;//������1~256��ERC��Ҫ/��ͨ�¼�
	if(m)
	{
		ADDR1=ADDR_AFN0CF7_FM;
		ADDR2=ADDR_AFN0EF1;
	}
	else
	{
		ADDR1=ADDR_AFN0CF7_FM+1;
		ADDR2=ADDR_AFN0EF2;
	}
	EC=MRR(ADDR1,1);//��Ҫ/��ͨ�¼�����
	if(EC)
	{
		for(i=EC;i>0;i--)
		{
			if(ERC == MRR(ADDR2+(LEN_TERMINAL_EVENT*(i-1)),1))
			{//��Ҫ/��ͨ�¼�ERC����
				a[j]=i;
				j++;
			}		
		}
		for(i=256;i>EC;i--)
		{
			if(ERC == MRR(ADDR2+(LEN_TERMINAL_EVENT*(i-1)),1))
			{//��Ҫ/��ͨ�¼�ERC����
				a[j]=i;
				j++;
			}		
		}
	}
	else
	{
		for(i=256;i>0;i--)
		{
			if(ERC == MRR(ADDR2+(LEN_TERMINAL_EVENT*(i-1)),1))
			{//��Ҫ/��ͨ�¼�ERC����
				a[j]=i;
				j++;
			}		
		}
	}
	return a[n-1];
}
//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelect
// Descriptions:        ʮ����������ѡ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void UpDownSelect(void)
{
	u16 *p16;
	u32 x;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//���Ƽ�����
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
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//��ʾ�ַ���
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//���Ƽ�����
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
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//��ʾ�ַ���
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelectH
// Descriptions:        ʮ������������ѡ��
// input parameters:    ��
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void UpDownSelectH(void)
{
	u16 *p16;
	u32 x;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	if((Comm_Ram->ButtonVal&0x02)!=0)
	{//���Ƽ�����
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
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//��ʾ�ַ���
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
	if((Comm_Ram->ButtonVal&0x01)!=0)
	{//���Ƽ�����
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
		DisplayString(Ln,Stack->MenuStack[MenuCount->Count].EditCol,0, (u8 *)&x);//��ʾ�ַ���
		NegativeLnCol(Ln,Stack->MenuStack[MenuCount->Count].EditCol);	   
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       NdigitSelect
// Descriptions:        Nλ������ѡ��
// input parameters:    Nλ�����༭���к�Col����ѡ��Χ��С��Min����ѡ��Χ�����Max
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------
void NdigitSelect(u32 N,u32 Col,u32 Min,u32 Max)
{
	u8 *p8;
	u32 x;
	u32 i;
	u32 Ln;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Ln=1;//�˵��ӵ�1�п�ʼ
#else
	Ln=0;//�˵��ӵ�0�п�ʼ
#endif

	Ln+=Stack->MenuStack[MenuCount->Count].EditRow;
	for(i=0;i<2;i++)
	{
		if(Stack->MenuStack[MenuCount->Count].EditCol==0)
		{
			if((Comm_Ram->ButtonVal&0x08)!=0)
			{//�Ҽ�
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
			{//���
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
			{//ȷ��
				Stack->MenuStack[MenuCount->Count].EditCol=Col;
				DisplayString(Ln,16,0,"����");
				Comm_Ram->ButtonVal&=~0x1C;//��ʹ�ú���0
			}
			else
			{
				Comm_Ram->ButtonVal&=~0x0C;//��ʹ�ú���0
				break;
			}
		}
		else//if(Stack->MenuStack[MenuCount->Count].EditCol!=0)
		{
			Select(Col+N-1,Col);
			UpDownSelect();
			if((Comm_Ram->ButtonVal&0x10)!=0)
			{//ȷ��
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
				Comm_Ram->ButtonVal&=~0x1F;//��ʹ�ú���0
			}
			else
			{
				p8=(u8 *)(ADDR_STRINGBUFF+(Ln)*84+4+Col*2);
				MenuCount->v2=p8_bcd(p8,N);
				MenuCount->v1=0;
				Comm_Ram->ButtonVal&=~0x0F;//��ʹ�ú���0
				break;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       ArrowLR
// Descriptions:        ��Row�С�Ln����ʾ���Ҽ�ͷ��ͬʱ��������е����Ҽ�ͷ
// input parameters:    Row��Ln
// output parameters:   ��
// Returned value:      ��
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
	p16[0]=32;//'���Ҽ�ͷ'
	p16[1]=33;
}

//-------------------------------------------------------------------------------------------------
// Function name:       PowerP16
// Descriptions:        ��376.1Э����A2��ʽ�Ĺ���ת���������ʾ������p16�С�����ʾ����ָ���ڱ���������ָ�������ʡ��ڱ����������жϣ�
// input parameters:    ����y����ʾ������p16
// output parameters:   ��
// Returned value:      ���ַ���ʾ����ָ��
//-------------------------------------------------------------------------------------------------
u16 * PowerP16(u32 y,u16 *p16)
{
	u32 z;
	u32 m;

	z=y&0xfff;
	switch(y>>13)
	{
		case 0://10��4�η�
			m=z<<(4*4);
			p16 = DisplayData_OffZero(m,7,0,p16);
			break;
		case 1://10��3�η�
			m=z<<(3*4);
			p16 = DisplayData_OffZero(m,6,0,p16);
			break;
		case 2://10��2�η�
			m=z<<(2*4);
			p16 = DisplayData_OffZero(m,5,0,p16);
			break;
		case 3://10��1�η�
			m=z<<(1*4);
			p16 = DisplayData_OffZero(m,4,0,p16);
			break;
		case 4://10��0�η�
			m=z;
			p16 = DisplayData_OffZero(m,3,0,p16);
			break;
		case 5://10��-1�η�
			m=z;
			p16 = DisplayData_OffZero(m,3,1,p16);
			break;
		case 6://10��-2�η�
			m=z;
			p16 = DisplayData(m,3,2,p16);
			break;
		case 7://10��-3�η�
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
// Descriptions:        ����Ļ��ʾ��ָ����(4�������ֲ���)
// input parameters:    �к�(�к�0-9(160*160),0-14(320*240))
// output parameters:   ��
// Returned value:      ��
//-------------------------------------------------------------------------------------------------

void DisplayClrRow_4(u32 Row)//����Ļ��ʾ��ָ����(4�������ֲ���)
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
// Descriptions:        ��ʾ�ַ�����\n\r����һ������֣������س����д�����
// input parameters:    ��Ļ��(0-9),��Ļ��(0-19),�Զ�����ʱ��������,�ַ���ָ��(0Ϊ������)
// output parameters:   ��
// Returned value:      B0-B7��ǰ��Ļ��,B8-B15��ǰ��Ļ��,B16-B23��ǰ�ַ���λ��,B24-B31����
//-------------------------------------------------------------------------------------------------
u32 DisplayString1(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)
{
	u32 i;
	u32 x;
	u32 y;
	u16 * pStrBuff;
	u32 Row;

#if ((Project/100)==3 && (USER/100)==5)//�Ϻ�ר��
	Row=8;//�˵��ڵ�8�п�ʼ
#else
	Row=9;//�˵��ڵ�9�п�ʼ
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
		{//����
			i++;
			return (Ln+(Col<<8)+(i<<16));
		}
		if(pStr[i]=='\n')
		{//����
			i++;
			Col++;
			if(Col>=20)
			{//����
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
		{//�س�
			i++;
			Col++;
			if(Col>=20)
			{//����
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
			{//����
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
			//ASCII�ֿ�
			pStrBuff[0]=x+0x2000;
			pStrBuff++;
			Col++;
			if(Col>=20)
			{//����
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
// Descriptions:        ���ݱ�ʶת��ΪFn
// input parameters:    DT1��DT2��DT1�ĵ�iλ
// output parameters:   ��
// Returned value:      Fn,m=1�д�Fn,m=0�޴�Fn
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
// Descriptions:        ���ݱ�ʶת��Ϊpn
// input parameters:    DA1��DA2��DA1�ĵ�iλ
// output parameters:   ��
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