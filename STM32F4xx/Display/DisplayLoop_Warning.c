
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


u32 DisplayWarningString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr)//��ʾ�澯�ַ���
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
			if(x<=3)
			{
				//�Զ����
				pStrBuff[0]=x+34;
			}
			else
			{
      	//ASCII�ֿ�
      	pStrBuff[0]=x+0x2000;
			}
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

u32 DisplayLoop_Warning(u32 Row)//�澯��Ϣ��ʾ,��������
{
	u32 i;
	u16 *p16;
//	UARTCtrl_TypeDef * UARTCtrl;

	if(TerminalDisplayLoop->Count==0)
	{
		//��ʾ
		DisplayString(Row,0,0,(u8 *)"��ʾ");//��ʾ�ַ���
		DispalyAlign(Row,0,2);//����ʾ����(1�����,2����,3�Ҷ���)
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
				//DisplayString(Row,0,0,(u8 *)(((u32)p16)+4));//��ʾ�ַ���
				DisplayWarningString(Row,0,0,(u8 *)(((u32)p16)+4));//��ʾ�澯�ַ���
				Row++;
			}
			p16+=84/2;
		}

//������ʾ
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_4PORT);
//	i=UARTCtrl->DateReRMCount;
//	DisplayData_OffZero(Comm_Ram->TEMP,4,1,(u16 *)(ADDR_STRINGBUFF+(8*84)+4));//����ʾ��Чλ����,���ָ��Ҫ��BCDλ��,С��������λ��,ʹ��ADDR_MEMRWREGBUFF
//	DisplayRam(9,ADDR_RUNSPEEDTEST+2);//��ʾ��RAM����(������) 
//		Row++;
	}
	return Row;
}



		
		




