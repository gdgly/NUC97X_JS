/*************************************************
��  Ȩ:��̩�Ǳ������ն˲�
��  ��:QCSG_PublicFunc.c
��  ��:mxn
��  ��:1.00
��  ��:20170601
��  ��:����Э�鹫�ú����ļ�
*************************************************/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "../Hcsg/ProjectCSG.h"
#include "../Hcsg/ERAM_For_Mxn.h"
#include "../Hcsg/IRAM_TerminalSaveCSG.h"
#include "../QCSG_Head/QCSG_UpLinkProtocol.h"
#include "../QCSG_Head/QCSG_LocalLinkProtocol.h"
#include "../QCSG_Head/QCSG_DLT645_Protocol.h"

#include "../QCSG_Head/Chint_Type.h"
#include "../QCSG_Head/QCSG_PublicDefine.h"
#include "../QCSG_Head/QCSG_PublicFunc.h"
#include "../QCSG_Head/QCSG_Table.h"
#include "../Device/MEMRW.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../QCSG_Data/QCSG_DefaultData.h"
#include "../QCSG_Data/QCSG_NandDataInterface.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"
#include "../QCSG_Head/QCSG_Hal.h"

#include "../Display/Display.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**--------------------------------------------------------------------
*��    ��:����ֵ
*�������:fx
*�������:��
*��    ��:����ֵ
--------------------------------------------------------------------**/
 float QCSG_ABS(float fx) 
 {
     if(fx < 0)
	 {
		fx = fx * (-1);		 
	 }
     return fx;
 }
/**--------------------------------------------------------------------
*��    ��:SIN����
*�������:fx
*�������:��
*��    ��:SINֵ
--------------------------------------------------------------------**/
float QCSG_SIN(float fx) 
{
	float fB = 1.2732395447; 
	float fC = -0.4052847346;
	float fP = 0.2310792853;
	float fPI = 3.1415926536;
	float fy = 0.0;
	if(fx > fPI) 
	{
		fx = fx - 2 * fPI; 
	}
	fy = fB * fx + fC * fx * QCSG_ABS(fx);
	fy = fP * (fy * QCSG_ABS(fy) - fy) + fy;
	return fy;
}
/**--------------------------------------------------------------------
*��    ��:COS����
*�������:fx
*�������:��
*��    ��:COSֵ
--------------------------------------------------------------------**/
float QCSG_COS(float fx) 
{
	float fQ = 1.5707963268;
	float fPI = 3.1415926536;
	fx = fx + fQ; 
	if(fx > fPI) 
	{
		fx = fx - 2 * fPI; 
	}
	return QCSG_SIN(fx);
}
 /**--------------------------------------------------------------------
*��    ��:��������
*�������:fa
*�������:��
*��    ��:����ֵ
--------------------------------------------------------------------**/
float QCSG_SQRT(float fa)
{
	double fx,fy;
	fx = 0.0;
	fy = fa / 2;
	while(fx != fy)
	{
		fx = fy;
		fy = (fx + fa / fx) / 2;
	}
	return fx;
}
/**--------------------------------------------------------------------
*��    ��:��ȡ��ǰͨ������״̬
*�������:u8ChNo
*�������:��
*��    ��:�Ƿ�����
--------------------------------------------------------------------**/
u8 QCSG_GetCHConnectState(u8 u8ChNo)
{
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:ʱ�����
*�������:TimerMinuend��TimerSubtrahend
*�������:
       Result[0] //��������,
       Result[1] //����Сʱ��(��ȥ��֮��,
       Result[2] //���ķ���������ȥСʱ֮��,
       Result[3] //������������ȥ����֮��
*��    ��:
       >0 TimerMinuend>TimerSubtrahend;
       =0 TimerMinuend=TimerSubtrahend;
       <0 TimerMinuend<TimerSubtrahend;
--------------------------------------------------------------------**/
s8 QCSG_TimerSub(TIME_PARAM_S* pstTimerMinuend,TIME_PARAM_S* pstTimerSubtrahend,u32* pu32Result)
{
    u32 const MonthDay[]={0, 31, 59, 90, 120, 151,181, 212, 243, 273, 304, 334};

    s8 compPos = 1;
    s8 compZero = 0;
    s8 compNeg = -1;
    s8 s8compResult = compZero;
    TIME_PARAM_S* pstTimerHi = NULL;
    TIME_PARAM_S* pstTimerLo = NULL;
    u32 au32subYear[2], au32submonth[2], au32subday[2], u32hivalue = 0, u32lovalue = 0;
    u32 u32tempday = 0, u32tempYear = 0;
    int i = 0;

    au32subYear[0] = (pstTimerMinuend->u8Year >> 4 ) * 10 + (pstTimerMinuend->u8Year & 0x0f);
    au32subYear[1] = (pstTimerSubtrahend->u8Year >> 4) * 10 + (pstTimerSubtrahend->u8Year & 0x0f);
    au32submonth[0] = ((pstTimerMinuend->u8Month&0x1f) >> 4) * 10 + (pstTimerMinuend->u8Month & 0x0f) - 1;
    au32submonth[1] = ((pstTimerSubtrahend->u8Month&0x1f) >> 4) * 10 + (pstTimerSubtrahend->u8Month & 0x0f) - 1;
    au32subday[0] = (pstTimerMinuend->u8Day >> 4) * 10 + (pstTimerMinuend->u8Day & 0x0f) - 1;
    au32subday[1] = (pstTimerSubtrahend->u8Day >> 4) * 10 + (pstTimerSubtrahend->u8Day & 0x0f) - 1;
    for (i = 0; i < 2; ++i)
    {
        u32tempYear = au32subYear[i] / 4;
        u32tempday = au32subYear[i] * 365 + u32tempYear + 1;
        if (((au32subYear[i] % 4) == 0) && (au32submonth[i] <= 1))
        {
            u32tempday = u32tempday - 1;
        }
        au32subday[i] = au32subday[i] + u32tempday;
        u32tempday = MonthDay[au32submonth[i]];
        au32subday[i] = au32subday[i] + u32tempday;
    }
    if (au32subday[0] > au32subday[1])
    {
        pu32Result[0] = au32subday[0] - au32subday[1];
        pstTimerHi = pstTimerMinuend;
        pstTimerLo = pstTimerSubtrahend;
        s8compResult = compPos;
    }
    else if (au32subday[0] < au32subday[1])
    {
        pu32Result[0] = au32subday[1] - au32subday[0];
        pstTimerLo = pstTimerMinuend;
        pstTimerHi = pstTimerSubtrahend;
        s8compResult = compNeg;
    }
    else
    {
        pu32Result[0] = 0;
    }
    if (compZero == s8compResult)
    {
        if (pstTimerMinuend->u8Hour > pstTimerSubtrahend->u8Hour)
        {
            pstTimerHi = pstTimerMinuend;
            pstTimerLo = pstTimerSubtrahend;
            s8compResult = compPos;
        }
        else if (pstTimerMinuend->u8Hour < pstTimerSubtrahend->u8Hour)
        {
            pstTimerLo = pstTimerMinuend;
            pstTimerHi = pstTimerSubtrahend;
            s8compResult = compNeg;
        }
        else
        {
            pu32Result[1] = 0;
        }
        if (compZero != s8compResult)
        {
            u32hivalue = (pstTimerHi->u8Hour >> 4) * 10 + (pstTimerHi->u8Hour & 0x0f);
            u32lovalue = (pstTimerLo->u8Hour >> 4) * 10 + (pstTimerLo->u8Hour & 0x0f);
            pu32Result[1] = u32hivalue - u32lovalue;
        }
    }
    else
    {
        if (pstTimerHi->u8Hour > pstTimerLo->u8Hour)
        {
            u32hivalue = (pstTimerHi->u8Hour >> 4) * 10 + (pstTimerHi->u8Hour & 0x0f);
            u32lovalue = (pstTimerLo->u8Hour >> 4) * 10 + (pstTimerLo->u8Hour & 0x0f);
            pu32Result[1] = u32hivalue - u32lovalue;
        }
        else if (pstTimerHi->u8Hour < pstTimerLo->u8Hour)
        {
            u32hivalue = (pstTimerHi->u8Hour >> 4) * 10 + (pstTimerHi->u8Hour & 0x0f);
            u32lovalue = (pstTimerLo->u8Hour >> 4) * 10 + (pstTimerLo->u8Hour & 0x0f);
            pu32Result[1] = u32hivalue + 24 - u32lovalue;
            pu32Result[0] = pu32Result[0] - 1;
        }
        else
        {
            pu32Result[1] = 0;
        }
    }
    if (compZero == s8compResult)
    {
        if (pstTimerMinuend->u8Minute > pstTimerSubtrahend->u8Minute)
        {
            pstTimerHi = pstTimerMinuend;
            pstTimerLo = pstTimerSubtrahend;
            s8compResult = compPos;
        }
        else if (pstTimerMinuend->u8Minute < pstTimerSubtrahend->u8Minute)
        {
            pstTimerLo = pstTimerMinuend;
            pstTimerHi = pstTimerSubtrahend;
            s8compResult = compNeg;
        }
        else
        {
            pu32Result[2] = 0;
        }
        if (compZero != s8compResult)
        {
            u32hivalue = (pstTimerHi->u8Minute >> 4) * 10 + (pstTimerHi->u8Minute & 0x0f);
            u32lovalue = (pstTimerLo->u8Minute >> 4) * 10 + (pstTimerLo->u8Minute & 0x0f);
            pu32Result[2] = u32hivalue - u32lovalue;
        }
    }
    else
    {
        if (pstTimerHi->u8Minute > pstTimerLo->u8Minute)
        {
            u32hivalue = (pstTimerHi->u8Minute >> 4) * 10 + (pstTimerHi->u8Minute & 0x0f);
            u32lovalue = (pstTimerLo->u8Minute >> 4) * 10 + (pstTimerLo->u8Minute & 0x0f);
            pu32Result[2] = u32hivalue - u32lovalue;
        }
        else if (pstTimerHi->u8Minute < pstTimerLo->u8Minute)
        {
            u32hivalue = (pstTimerHi->u8Minute >> 4) * 10 + (pstTimerHi->u8Minute & 0x0f);
            u32lovalue = (pstTimerLo->u8Minute >> 4) * 10 + (pstTimerLo->u8Minute & 0x0f);
            pu32Result[2] = u32hivalue + 60 - u32lovalue;
            if (pu32Result[1] >= 1)
            {
                pu32Result[1] = pu32Result[1] - 1;
            }
            else
            {
                pu32Result[1] = pu32Result[1] + 24 - 1;
                pu32Result[0] = pu32Result[0] - 1;
            }
        }
        else
        {
            pu32Result[2] = 0;
        }
    }
    if (compZero == s8compResult)
    {
        if (pstTimerMinuend->u8Second > pstTimerSubtrahend->u8Second)
        {
            pstTimerHi = pstTimerMinuend;
            pstTimerLo = pstTimerSubtrahend;
            s8compResult = compPos;
        }
        else if (pstTimerMinuend->u8Second < pstTimerSubtrahend->u8Second)
        {
            pstTimerLo = pstTimerMinuend;
            pstTimerHi = pstTimerSubtrahend;
            s8compResult = compNeg;
        }
        else
        {
            pu32Result[3] = 0;
        }
        if (compZero != s8compResult)
        {
            u32hivalue = (pstTimerHi->u8Second >> 4) * 10 + (pstTimerHi->u8Second & 0x0f);
            u32lovalue = (pstTimerLo->u8Second >> 4) * 10 + (pstTimerLo->u8Second & 0x0f);
            pu32Result[3] = u32hivalue - u32lovalue;
        }
    }
    else
    {
        if (pstTimerHi->u8Second > pstTimerLo->u8Second)
        {
            u32hivalue = (pstTimerHi->u8Second >> 4) * 10 + (pstTimerHi->u8Second & 0x0f);
            u32lovalue = (pstTimerLo->u8Second >> 4) * 10 + (pstTimerLo->u8Second & 0x0f);
            pu32Result[3] = u32hivalue - u32lovalue;
        }
        else if (pstTimerHi->u8Second < pstTimerLo->u8Second)
        {
            u32hivalue = (pstTimerHi->u8Second >> 4) * 10 + (pstTimerHi->u8Second & 0x0f);
            u32lovalue = (pstTimerLo->u8Second >> 4) * 10 + (pstTimerLo->u8Second & 0x0f);
            pu32Result[3] = u32hivalue + 60 - u32lovalue;
            if (pu32Result[2] >= 1)
            {
                pu32Result[2] = pu32Result[2] - 1;
            }
            else
            {
                pu32Result[2] = pu32Result[2] + 60 - 1;
                if (pu32Result[1] >= 1)
                {
                    pu32Result[1] = pu32Result[1] - 1;
                }
                else
                {
                    pu32Result[1] = pu32Result[1] + 24 - 1;
                    pu32Result[0] = pu32Result[0] - 1;
                }
            }
        }
        else
        {
            pu32Result[3] = 0;
        }
    }
    return  s8compResult;
}
/**--------------------------------------------------------------------
*��    ��:ʱ��ӷ���TIME_PARAM_S *TimerTotal = TIME_PARAM_S *TimerAddend+u32 Sec
*�������:TimerAddend,Sec
*�������:TimerTotal
*��    ��:�����
--------------------------------------------------------------------**/
u8 QCSG_TimerAdd(TIME_PARAM_S *TimerAddend,u32 Sec,TIME_PARAM_S *TimerTotal)	
{
	u32 AddSecond = Sec % 60;
	u32 AddMinute = Sec / 60;
	u32 AddHour = 0, AddDay = 0;
	u32 const MonthDay[]={0, 31, 28, 31, 30, 31, 30,31, 31, 30, 31, 30, 31};
	u32 CurYear = 0, CurMonth = 0, CurDay = 0, CurHour = 0, CurMinute = 0, CurSecond = 0;
	u32 CurMonthDay = 0;
	u8 AddResult = 0;
	
	TimerTotal->u8Month=TimerTotal->u8Month&0x1f;
	TimerAddend->u8Month=TimerAddend->u8Month&0x1f;
	TimerTotal->u8Year = TimerAddend->u8Year;
	TimerTotal->u8Month = TimerAddend->u8Month;
	TimerTotal->u8Day = TimerAddend->u8Day;
	TimerTotal->u8Hour = TimerAddend->u8Hour;
	TimerTotal->u8Minute = TimerAddend->u8Minute;
	TimerTotal->u8Second = TimerAddend->u8Second;
	CurYear = (TimerTotal->u8Year >> 4) * 10 + (TimerTotal->u8Year & 0x0f);
	CurMonth = (TimerTotal->u8Month >> 4) * 10 + (TimerTotal->u8Month & 0x0f);
	CurDay = (TimerTotal->u8Day >> 4) * 10 + (TimerTotal->u8Day & 0x0f);
	CurHour = (TimerTotal->u8Hour >> 4) * 10 + (TimerTotal->u8Hour & 0x0f);
	CurMinute = (TimerTotal->u8Minute >> 4) * 10 + (TimerTotal->u8Minute & 0x0f);
	CurSecond = (TimerTotal->u8Second >> 4) * 10 + (TimerTotal->u8Second & 0x0f);
	if ((CurSecond + AddSecond) < 60)
	{
		CurSecond = CurSecond + AddSecond;
	}
	else
	{
		CurSecond = CurSecond + AddSecond - 60;
		AddMinute = AddMinute + 1;
	}
	AddHour = AddMinute / 60;
	AddMinute = AddMinute % 60;
	if ((CurMinute + AddMinute) < 60)
	{
		CurMinute = CurMinute + AddMinute;
	}
	else
	{
		CurMinute = CurMinute + AddMinute - 60;
		AddHour = AddHour + 1;
	}
	AddDay = AddHour / 24;
	AddHour = AddHour % 24;
	if ((CurHour + AddHour) < 24)
	{
		CurHour = CurHour + AddHour;
	}
	else
	{
		CurHour = CurHour + AddHour - 24;
		AddDay = AddDay + 1;
	}
	while (AddDay > 0)
	{
		CurMonthDay = MonthDay[CurMonth];
		if ((2 == CurMonth) && (0 == CurYear % 4))
		{
			CurMonthDay = CurMonthDay + 1;
		}
		if ((CurDay + AddDay) <= CurMonthDay)
		{
			CurDay = CurDay + AddDay;
			AddDay = 0;
		}
		else
		{
			AddDay = AddDay + CurDay - CurMonthDay - 1;
			CurDay = 1;
			if ((CurMonth + 1) <= 12)
			{
				CurMonth = CurMonth + 1;
			}
			else
			{
				CurMonth = 1;
				CurYear = CurYear + 1;
				if (CurYear > 99)
				{
					AddResult = 1;
					AddDay = 0;
				}
			}
		}
	}
	TimerTotal->u8Year = (CurYear / 10) * 16 + (CurYear % 10);
	TimerTotal->u8Month = (CurMonth / 10) * 16 + (CurMonth % 10);
	TimerTotal->u8Day = (CurDay / 10) * 16 + (CurDay % 10);
	TimerTotal->u8Hour = (CurHour / 10) * 16 + (CurHour % 10);
	TimerTotal->u8Minute = (CurMinute / 10) * 16 + (CurMinute % 10);
	TimerTotal->u8Second = (CurSecond / 10) * 16 + (CurSecond % 10);
	return	AddResult;
}
/**--------------------------------------------------------------------
*��    ��:BCD�ж�
*�������:u8Bcd
*�������:��
*��    ��:������
--------------------------------------------------------------------**/
u8 QCSG_BCDCheck(u8* pu8Data,u32 u16DataLen)
{
	u32 i = 0;
	for(i = 0;i < u16DataLen;i++)
	{
		if(((pu8Data[i] & 0x0F)> 0x09) || ((pu8Data[i] & 0xF0)> 0x90))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:BCD2HEX
*�������:u8Bcd
*�������:��
*��    ��:������
--------------------------------------------------------------------**/
u8 QCSG_BCD2HEX(u8 u8BCD)
{
    u8 u8Hex;
    u8Hex = (u8BCD>>4)*10 + (u8BCD&0x0F);
    return u8Hex;
}
/**--------------------------------------------------------------------
*��    ��:HEX2BCD
*�������:u8HEX
*�������:��
*��    ��:������
--------------------------------------------------------------------**/
u8 QCSG_HEX2BCD(u8 u8HEX)
{
    u8 u8Bcd;
    u8Bcd = (u8HEX/10)*16 + (u8HEX%10);
    return u8Bcd;
}
/**--------------------------------------------------------------------
*��    ��:BCD��1
*�������:u8Bcd
*�������:��
*��    ��:������
--------------------------------------------------------------------**/
u8 QCSG_IncBCD(u8 u8Bcd)
{
    u8Bcd = u8Bcd+1;
    if((u8Bcd & 0x0f) >= 0x0a)
    {
        u8Bcd = u8Bcd + 0x10;
        u8Bcd = u8Bcd & 0xf0;
    }
    return u8Bcd;
}
/**--------------------------------------------------------------------
*��    ��:BCD��1��������������0
*�������:u8Bcd
*�������:��
*��    ��:������
--------------------------------------------------------------------**/
u8 QCSG_DecBCD(u8 u8Bcd)
{
    if(u8Bcd==0)
    {
        return 0;
    }
    if((u8Bcd&0x0f)==0)
    {
        u8Bcd=u8Bcd-0x10;
        u8Bcd=u8Bcd|0x09;
    }
    else
    {
        u8Bcd=u8Bcd-1;
    }
    return u8Bcd;
}
/**--------------------------------------------------------------------
*��    ��:����һ�����������
*�������:pstTime
*�������:��
*��    ��:������
--------------------------------------------------------------------**/
u8 QCSG_CalMaxDateInMonth(TIME_PARAM_S* pstTime)
{
    u8 u8Month,u8Year,u8MaxDate;
    u8 au8MonthDay[13]={0x00, 0x31, 0x28, 0x31, 0x30, 0x31, 0x30,0x31, 0x31, 0x30, 0x31,0x30, 0x31};

    u8Month = pstTime->u8Month & 0x1f;
    u8Month = (((u8Month>>4) & 0x0f) * 10) + (u8Month & 0x0f);
    u8Year = pstTime->u8Year;
    u8Year = (((u8Year>>4) & 0x0f) * 10) + (u8Year & 0x0f);
    if((u8Month > 12)||(u8Month == 0))
    {
        return 0;
    }
    u8MaxDate = au8MonthDay[u8Month];
    if((u8Month == 2) && (u8Year%4 == 0))
    {
        u8MaxDate = u8MaxDate + 1;
    }
    return u8MaxDate;

/* 	u8 MaxDayNum = 0;
	u8 YearInDec = (pstTime->Year & 0x0F) + (pstTime->Year >> 4) * 10;
	switch(pstTime->u8Month & 0x1F)
	{
		case 0x01:
		case 0x03:
		case 0x05:
		case 0x07:
		case 0x08:
		case 0x10:
		case 0x12:
			MaxDayNum = 31;
			break;
		case 0x02:
			if(LeapYearCheck(YearInDec + 2000) != 0)			//�����ж�
			{
				MaxDayNum = 29;
			}
			else
			{
				MaxDayNum = 28;
			}
			break;
		case 0x04:
		case 0x06:
		case 0x09:
		case 0x11:
			MaxDayNum = 30;
			break;
		default:
			MaxDayNum = 31;
			break;
		
	}
	return MaxDayNum;
*/
}

u8 QCSG_IncOneSecond(TIME_PARAM_S* pstTime)
{
	pstTime->u8Second = QCSG_IncBCD(pstTime->u8Second);
	if(pstTime->u8Second >= 0x60)
	{
		pstTime->u8Second = 0x00;
		QCSG_IncOneMinute(pstTime);
	}
    return TRUE;
}

u8 QCSG_DecOneSecond(TIME_PARAM_S* pstTime)
{
	if(pstTime->u8Second == 0x00)
	{
		pstTime->u8Second = 0x59;
		QCSG_DecOneMinute(pstTime);
	}
	else
	{
		pstTime->u8Second = QCSG_DecBCD(pstTime->u8Second);
	}
    return TRUE;
}

/**--------------------------------------------------------------------
*��    ��:ʱ���һСʱ
*�������:pstTime
*�������:pstTime
*��    ��:�Ƿ�ӳɹ�
--------------------------------------------------------------------**/
u8 QCSG_IncOneMinute(TIME_PARAM_S* pstTime)
{
	pstTime->u8Minute = QCSG_IncBCD(pstTime->u8Minute);
	if(pstTime->u8Minute >= 0x60)
	{
		pstTime->u8Minute = 0x00;
		QCSG_IncOneHour(pstTime);
	}
    return TRUE;
}

u8 QCSG_DecOneMinute(TIME_PARAM_S* pstTime)
{
	if(pstTime->u8Minute == 0x00)
	{
		pstTime->u8Minute = 0x59;
		QCSG_DecOneHour(pstTime);
	}
	else
	{
		pstTime->u8Minute = QCSG_DecBCD(pstTime->u8Minute);
	}
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:ʱ���һСʱ
*�������:pstTime
*�������:pstTime
*��    ��:�Ƿ�ӳɹ�
--------------------------------------------------------------------**/
u8 QCSG_IncOneHour(TIME_PARAM_S* pstTime)
{
	pstTime->u8Hour = QCSG_IncBCD(pstTime->u8Hour);
	if(pstTime->u8Hour >= 0x24)
	{
		pstTime->u8Hour = 0x00;
		QCSG_IncOneDay(pstTime);
	}
    return TRUE;
}
u8 QCSG_DecOneHour(TIME_PARAM_S* pstTime)
{
	if(pstTime->u8Hour == 0x00)
	{
		pstTime->u8Hour = 0x23;
		QCSG_DecOneDay(pstTime);
	}
	else
	{
		pstTime->u8Hour = QCSG_DecBCD(pstTime->u8Hour);
	}
    return TRUE;
}

/**--------------------------------------------------------------------
*��    ��:ʱ���һ��
*�������:pstTime
*�������:pstTime
*��    ��:�Ƿ�ӳɹ�
--------------------------------------------------------------------**/
u8 QCSG_IncOneDay(TIME_PARAM_S* pstTime)
{
    u8 u8MaxData,u8Month;

    u8MaxData = QCSG_CalMaxDateInMonth(pstTime);
    if(u8MaxData == 0)
    {
        return FALSE;
    }
    pstTime->u8Day = QCSG_IncBCD(pstTime->u8Day);
    if(pstTime->u8Day > u8MaxData)
    {
        pstTime->u8Day = 1;
        u8Month = pstTime->u8Month;
        u8Month = QCSG_IncBCD(u8Month);
        pstTime->u8Month = u8Month;
        if(u8Month > 0x12)
        {
            pstTime->u8Month = 1;
            pstTime->u8Year = QCSG_IncBCD(pstTime->u8Year);
        }
    }
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:ʱ���һ��
*�������:pstTime
*�������:pstTime
*��    ��:�Ƿ���ɹ�
--------------------------------------------------------------------**/
u8 QCSG_DecOneDay(TIME_PARAM_S* pstTime)
{
    u8 u8MaxData;

    if(pstTime->u8Day == 1)
    {
        if(pstTime->u8Month == 1)
        {
            pstTime->u8Month = 0x12;
            pstTime->u8Year = QCSG_DecBCD(pstTime->u8Year);
        }
        else
        {
            pstTime->u8Month = QCSG_DecBCD(pstTime->u8Month);
        }
        u8MaxData = QCSG_CalMaxDateInMonth(pstTime);
        if(u8MaxData == 0)
        {
            return FALSE;
        }
        pstTime->u8Day = u8MaxData;
    }
    else
    {
        pstTime->u8Day = QCSG_DecBCD(pstTime->u8Day);
    }
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:ʱ���һ��
*�������:pstTime
*�������:pstTime
*��    ��:�Ƿ�ӳɹ�
--------------------------------------------------------------------**/
u8 QCSG_IncOneMonth(TIME_PARAM_S* pstTime)
{
    u8 u8Month;

    u8Month = pstTime->u8Month;
    u8Month = QCSG_IncBCD(u8Month);
    pstTime->u8Month = u8Month;
    if(u8Month > 0x12)
    {
        pstTime->u8Month = 1;
        pstTime->u8Year = QCSG_IncBCD(pstTime->u8Year);
    }
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:ʱ���һ��
*�������:pstTime
*�������:pstTime
*��    ��:�Ƿ���ɹ�
--------------------------------------------------------------------**/
u8 QCSG_DecOneMonth(TIME_PARAM_S* pstTime)
{
    if(pstTime->u8Month == 1)
    {
        pstTime->u8Month = 0x12;
        pstTime->u8Year = QCSG_DecBCD(pstTime->u8Year);
    }
    else
    {
        pstTime->u8Month = QCSG_DecBCD(pstTime->u8Month);
    }
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:�����У��
*�������:pu8Buf�����뻺�壬u16Len�����峤��
*�������:��
*��    ��:������
--------------------------------------------------------------------**/
u8 QCSG_CalCs(u8* pu8Buf,u16 u16Len)
{
    u16 i = 0;
    u8  u8Result = 0;

    for(i = 0;i < u16Len;i++)
        u8Result = u8Result+pu8Buf[i];
    return u8Result;
}
/**--------------------------------------------------------------------
*��    ��:��645Э��֡
*�������:pu8MeterID����ţ�u8CtrlCode�������룬u16InLen:���뻺�峤�ȣ�pu8InBuf:���뻺��
*�������:pu16OutLen�����֡���ȣ�pu8OutBuf�����뻺��
*��    ��:��֡�ɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_MakeDLT645Frame(u8 u8FEFlag,u8* pu8MeterID,u8 u8CtrlCode,u8* pu8InBuf,u8 u8InLen,u8 u8OutBufSize,u8* pu8OutLen,u8* pu8OutBuf)
{
	u8 i = 0,u8StartPos = 0;
    if((u8InLen + 16) > u8OutBufSize)
    {
        return FALSE;
    }
	for(i = 0;i < u8InLen;i++)
	{
		pu8InBuf[i] += 0x33;
	}
	if(u8FEFlag == TRUE)
	{
		MemSet(&pu8OutBuf[0],0xFE,4);
		u8StartPos = 4;
	}
    pu8OutBuf[u8StartPos] = DLT_645_HEAD;
    MemCopy(&pu8OutBuf[u8StartPos + 1],pu8MeterID,6);
    pu8OutBuf[u8StartPos + 7] = DLT_645_HEAD;
    pu8OutBuf[u8StartPos + 8] = u8CtrlCode;
    pu8OutBuf[u8StartPos + 9] = u8InLen;
    MemCopy(&pu8OutBuf[u8StartPos + 10],pu8InBuf,u8InLen);
    pu8OutBuf[u8StartPos + 10 + u8InLen] = (u8)SumCaculation(&pu8OutBuf[u8StartPos],u8InLen + 10);
    pu8OutBuf[u8StartPos + 11 + u8InLen] = DLT_645_TAIL;
	
	if(u8FEFlag == TRUE)
	{
		(*pu8OutLen) =  u8InLen + 16;
	}
	else
	{
		(*pu8OutLen) =  u8InLen + 12;
	}
    
    return TRUE;
}


/**--------------------------------------------------------------------
*��    ��:��EDMIЭ��֡
*�������:pu8MeterID����ţ�u8CtrlCode�������룬u16InLen:���뻺�峤�ȣ�pu8InBuf:���뻺��
*�������:pu16OutLen�����֡���ȣ�pu8OutBuf�����뻺��
*��    ��:��֡�ɹ�orʧ��
--------------------------------------------------------------------**/

u8 QCSG_MakeEDMIFrame_Test(u8 u8FEFlag,u8* pu8MeterID,u8 u8CtrlCode,u8* pu8InBuf,u8 u8InLen,u8 u8OutBufSize,u8* pu8OutLen,u8* pu8OutBuf)
{//������
	//EDMI��½֡
	u8 Login_Code[30] = {0x02 ,0x45 ,0x0C ,0x4F ,0xD3 ,0xA5 ,0x01 ,0x2B ,
						 0x16 ,0x68 ,0xFF ,0xFF ,0x4C ,0x45 ,0x44 ,0x4D ,
						 0x49 ,0x2C ,0x49 ,0x4D ,0x44 ,0x45 ,0x49 ,0x4D ,
						 0x44 ,0x45 ,0x00 ,0xEF ,0x4D ,0x03}; 

	u8 Read_0169_Code[25] = {0x02 ,0x45 ,0x0C ,0x4F ,0xD3 ,0xA5 ,0x01 ,0x2B ,
							 0x16 ,0x68 ,0xFF ,0xFF ,0x4D ,0x00 ,0x00 ,0xFF ,
							 0xF0 ,0x00 ,0x00 ,0x01 ,0x69 ,0x44 ,0x87 ,0x33 ,
							 0x03};

	u8 Unlogin_Code[16] = {0x02 ,0x45 ,0x0C ,0x4F ,0xD3 ,0xA5 ,0x01 ,0x2B ,
						   0x16 ,0x68 ,0xFF ,0xFF ,0x58 ,0x92 ,0xA7 ,0x03};
		
	if(u8FEFlag == 0)
	{
		MemCopy(&pu8OutBuf[0],Login_Code,30);
		(*pu8OutLen) =  30;
	}
	else if(u8FEFlag == 1)
	{
		MemCopy(&pu8OutBuf[0],Read_0169_Code,25);
		(*pu8OutLen) =  25;
	}
	else if(u8FEFlag == 2)
	{
		MemCopy(&pu8OutBuf[0],Unlogin_Code,16);
		(*pu8OutLen) =  16;
	}
	else 
		return FALSE;


	return TRUE;
}




u16 cal_CRC16(u8* buffer,int len)
{
	u16 CRCConst= 0x1021;
	u16 crc = 0;
	for (int i = 0; i < len; i++)
	{
		for(int j=7;j>=0;j--)
		{
			if ((crc & 0x8000) != 0)
				crc = (crc << 1) ^ CRCConst; 
                	else
                    	crc = crc << 1;
		        if((buffer[i] &(1<<j))!=0)
			        crc= crc^CRCConst;
	        }
	}
	return crc;

}

u8 TrantoEdmi(u8* inbuf,u8 inlen,u8*outbuf){

	u8 i,j=1;	//j��Ҫ������i����,��datasize++����˼·
	outbuf[0]=0x02;
	for(i=1;i<(inlen-1);i++){	//��β�����
		if(inbuf[i]==0x02||inbuf[i]==0x03||inbuf[i]==0x10||inbuf[i]==0x11||inbuf[i]==0x13){

			outbuf[j++]=0x10;
			outbuf[j++]=inbuf[i]+0x40;

		}
		else{
			outbuf[j++]=inbuf[i];
		}
	
	}
	outbuf[j++]=0x03;

	return j;

}
//�����������ݱ�ʶ�ҵ���Ӧedmi���ݱ�ʶ�б�
//����edmi���������
u8 get_edmi_cmd(u32 nwcmd,u32* edmicmd){
	u8 i,num=0;

	if((nwcmd | 0x0000FFFF) == 0x0506FFFF)//��������
	{
		for(i = 0;i < MAX_FROZEN_EDMI_IDENT_NUM;i++)
		{
			if(nwcmd==gc_Jc_Rs485_FrozenReadCode_EDMI[i].u32Fn )
			{//�ҵ��˶�Ӧ ������������
				while(gc_Jc_Rs485_FrozenReadCode_EDMI[i].stEDMIFROZENResolve[num].u32Ident!=0xffff){
					edmicmd[num]=gc_Jc_Rs485_FrozenReadCode_EDMI[i].stEDMIFROZENResolve[num].u32Ident;
					num++;
				}
				break;
			}
		}
	}
	else	//��������
	{
		for(i=0;i<MAX_FIXED_EDMI_NUM;i++){
			if(nwcmd==EDMI_cmd[i].NwCmd ){//�ҵ��˶�Ӧ ������������

				while(EDMI_cmd[i].Edmi_info[num].EdmiCmd!=0xffff)
				{
					edmicmd[num]=EDMI_cmd[i].Edmi_info[num].EdmiCmd;
					num++;
				}
				break;
			}
		}
	}

	return num;
}

//�����������ݱ�ʶ�ҵ���Ӧ������ת��ģʽ
//��������ת��ģʽ
u32 get_edmi_method(u32 nwcmd){
	u8 i = 0;

	if((nwcmd | 0x0000FFFF) == 0x0506FFFF)//��������
	{
		for(i = 0;i < MAX_FROZEN_EDMI_IDENT_NUM;i++)
		{
			if(nwcmd==gc_Jc_Rs485_FrozenReadCode_EDMI[i].u32Fn )
			{//�ҵ��˶�Ӧ ������������
				return gc_Jc_Rs485_FrozenReadCode_EDMI[i].u32FixMethod;
			}
		}
	}
	else 
	{
		for(i = 0;i < MAX_FIXED_EDMI_NUM;i++)
		{
			if(nwcmd==EDMI_cmd[i].NwCmd )
			{//�ҵ��˶�Ӧ ������������
				return EDMI_cmd[i].u32FixMethod;
			}
		}
	}
	//û�ҵ� ֱ�ӿ���
	return COMMON_COV_METHOD;
}


u8 QCSG_MakeEDMIFrame(u8* pu8MeterID,u8* ident,u8* psendlen,u8* psendbuf){
	u8 tmp[60]={0};
	u32 addr,nwcmd;
	u16 check;
	u32 edmicmd[8];	//8���ú�MAX_EDMI_INFO_NUM����
	
	int len;
	u8 i,edmi_num,index=0;
	
	
	tmp[0]=0x02;
	tmp[1]=0x45;

	addr=QCSG_BCD2HEX(pu8MeterID[0])+QCSG_BCD2HEX(pu8MeterID[1])*100+
		QCSG_BCD2HEX(pu8MeterID[2])*10000+QCSG_BCD2HEX(pu8MeterID[3])*1000000+
		QCSG_BCD2HEX(pu8MeterID[4])*100000000;
	tmp[2]=(addr>>24)&0xff;
	tmp[3]=(addr>>16)&0xff;
	tmp[4]=(addr>>8)&0xff;
	tmp[5]=addr&0xff;

	tmp[6]=0x01;
	tmp[7]=0x2b;
	tmp[8]=0x16;
	tmp[9]=0x68;

	tmp[10]=0xff;
	tmp[11]=0xff;	
	
	tmp[12]='M';	//��0x4d
	
	tmp[15]=0xff;
	tmp[16]=0xf0;

	nwcmd=ident[0]+ident[1]*0x100+ident[2]*0x10000+ident[3]*0x1000000;

	if((nwcmd != 0x0000fffc) && (nwcmd != 0x0000fffd))
		edmi_num = get_edmi_cmd(nwcmd,edmicmd);
	else 
		edmi_num = 1;
	
	if(edmi_num==0) 
		return FALSE;//û����Ӧ��edmi������
	

	switch(nwcmd){

		case 0x0000fffd://��¼
			tmp[12]=0x4c;	//��'L'
			tmp[13]='E';
			tmp[14]='D';
			tmp[15]='M';
			tmp[16]='I';

			tmp[17]=',';
			
			tmp[18]='I';
			tmp[19]='M';
			tmp[20]='D';
			tmp[21]='E';

			tmp[22]='I';
			tmp[23]='M';
			tmp[24]='D';
			tmp[25]='E';

			len=27;//����tmp[26]=00����ֹ�ֽ�		
			break;

		case 0x0001ff00:
		case 0x0002ff00:
		case 0x0003ff00:
		case 0x0004ff00:
		case 0x0005ff00:
		case 0x0006ff00:
		case 0x0007ff00:
		case 0x0008ff00:

		case 0x0201ff00:	//����
		case 0x0202ff00://��ѹ
		case 0x0203ff00://�й�����
		case 0x0204ff00://�޹�����
		case 0x0207ff00://���
			index=19;
			for(i=0;i<edmi_num;i++){
				tmp[index++]=(edmicmd[i]>>8)&0xff;
				tmp[index++]=edmicmd[i]&0xff;
				index+=2;
			}
			index-=2;	//���һ���������������2��Ҫ��ȥ
			//tmp[index++]='D';	//0x44�����Ӹ��ֽ�Ҳ����ͨ����ʱͳһ����
			len=index;
			break;


		case 0x0000fffc://�˳���¼
			tmp[12]=0x58;	//��'X'
			len=13;
			break;
		default://�ն���
			index=19;
			for(i=0;i<edmi_num;i++){
				tmp[index++]=(edmicmd[i]>>8)&0xff;
				tmp[index++]=edmicmd[i]&0xff;
				index+=2;
			}
			index-=2;	//���һ���������������2��Ҫ��ȥ
			//tmp[index++]='D';	//0x44�����Ӹ��ֽ�Ҳ����ͨ����ʱͳһ����
			len=index;
			break;
	}

	check=cal_CRC16(tmp,len);

	tmp[len++]=check>>8;
	tmp[len++]=check&0xff;
	tmp[len++]=0x03;


	*psendlen=TrantoEdmi(tmp, (u8)len,psendbuf);
	

	return TRUE;
	
}





u8 QCSG_MakeEDMIFrame_RT(u8* pu8MeterID,u32 NwIdent,u8* psendlen,u8* psendbuf)
{
	u8 tmp[100]={0};
	u32 addr;
	u16 check;
	u8 i,j;
	int len;
	
	
	tmp[0]=0x02;
	tmp[1]=0x45;

	addr=QCSG_BCD2HEX(pu8MeterID[0])+QCSG_BCD2HEX(pu8MeterID[1])*100+
		QCSG_BCD2HEX(pu8MeterID[2])*10000+QCSG_BCD2HEX(pu8MeterID[3])*1000000+
		QCSG_BCD2HEX(pu8MeterID[4])*100000000;
	tmp[2]=(addr>>24)&0xff;
	tmp[3]=(addr>>16)&0xff;
	tmp[4]=(addr>>8)&0xff;
	tmp[5]=addr&0xff;

	tmp[6]=0x01;
	tmp[7]=0x2b;
	tmp[8]=0x16;
	tmp[9]=0x68;

	tmp[10]=0xff;
	tmp[11]=0xff;	
	
	tmp[12]=0x4d;	//��'M'
	
	tmp[15]=0xff;
	tmp[16]=0xf0;


	switch(NwIdent)
	{
		case 0x0001FF00:
		case 0x00010000:
		case 0x00020000:
		case 0x00030000:
		case 0x00040000:
			for(i=0;i<MAX_AFN0C_0D_IDENT_NUM;i++)
			{
				if(gc_stQCSGAFN0C_0D_EDMI_Table[i].u32Fn == NwIdent)
				{
					for(j=0;j<12;j++)
					{
						if(gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident != 0xFFFFFFFF)
						{
							tmp[17+4*j] = 0;
							tmp[18+4*j] = 0;
							tmp[19+4*j] = (gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident >> 8) & 0xFF;
							tmp[20+4*j] = gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident & 0xFF;
						}
						else 
							break;
					}
					break;
					
				}
			}
			len = 20+ 4*(j-1) +1;
			break;
		default: 
			for(i=0;i<MAX_AFN0C_0D_IDENT_NUM;i++)
			{
				if(gc_stQCSGAFN0C_0D_EDMI_Table[i].u32Fn == NwIdent)
				{
					for(j=0;j<12;j++)
					{
						if(gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident != 0xFFFFFFFF)
						{
							tmp[17+4*j] = 0;
							tmp[18+4*j] = 0;
							tmp[19+4*j] = (gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident >> 8) & 0xFF;
							tmp[20+4*j] = gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident & 0xFF;
						}
						else 
							break;
					}
					break;
					
				}
			}
			len = 20+ 4*(j-1) +1;
			break;
			
			
	}
	check=cal_CRC16(tmp,len);

	tmp[len++]=check>>8;
	tmp[len++]=check&0xff;
	tmp[len++]=0x03;


	*psendlen=TrantoEdmi(tmp, (u8)len,psendbuf);
	

	return TRUE;
	
}


/**--------------------------------------------------------------------
*��    ��:645֡��ʽ�ж�
*�������:pu8Data�����뻺�壬pu8DataLen�����峤��
*�������:pu8Data�����뻺�壬pu8DataLen�����峤��
*��    ��:��ʽ�Ƿ���ȷ
--------------------------------------------------------------------**/
u8 QCSG_Dlt645FrameJudge(u8* pu8Data,u8* pu8DataLen)
{
	//���ݲ��Ѿ���֤645֡��׼ȷ�ԣ������Ѿ�ȥ��0xFE��20170814
    u8 i = 0;
    u8 Len = 0;
    u8 Pos = 0xFF;
    if(((*pu8DataLen) == 0)||((*pu8DataLen) < 12)||((*pu8DataLen) > 216))
    {
        return FALSE;
    }
    for(i = 0; i < (*pu8DataLen); i++)
    {
        if(pu8Data[i] == 0x68)
        {
            Pos = i;
            break;
        }
    }
    if(Pos >= (*pu8DataLen))
    {
        return FALSE;
    }
    Len = pu8Data[Pos + 9];
    if(Len > 200)
    {
        return FALSE;
    }
    if(pu8Data[Pos + Len + 2 + 10 - 1] != 0x16)
    {
        return FALSE;
    }
    if(pu8Data[Pos + Len + 2 + 10 - 2] != (u8)SumCaculation(&pu8Data[Pos],Len + 10))
    {
        return FALSE;
    }
    MemCopy(&pu8Data[0],&pu8Data[Pos],Len + 10 + 2);
    (*pu8DataLen) = Len + 10 + 2;
    return TRUE;
}

/**--------------------------------------------------------------------
*��    ��:EDMI֡��ʽ�ж�
*�������:pu8Data�����뻺�壬pu8DataLen�����峤��
*�������:pu8Data�����뻺�壬pu8DataLen�����峤��
*��    ��:��ʽ�Ƿ���ȷ
--------------------------------------------------------------------**/
u8 QCSG_EDMIFrameJudge(u8* pu8Data,u8* pu8DataLen)
{
	//�ڴ�֮ǰ֡�Ѿ�����Ϊԭʼ����֡ ����Ҫ����0x10���жϴ���
	u8 DataLen = (*pu8DataLen);
	u8 Pos = 0;
	u8 i=0;
	if((DataLen == 0)||(DataLen < MIN_EDMI_FRAME_LEN)||(DataLen > 216))
    {
        return FALSE;
    }

	for(i = 0; i < (*pu8DataLen); i++)
	{
		if(pu8Data[i] == EDMI_HEAD)
		{
			Pos = i;
			break;
		}
	}
	if(Pos >= (*pu8DataLen))
	{
		return FALSE;
	}
	DataLen -=	Pos;
	MemCopy(&pu8Data[0],&pu8Data[Pos],DataLen);
	
	if(pu8Data[0] != EDMI_HEAD)
	{
		return FALSE;
	}
	
	if(pu8Data[DataLen - 1] != EDMI_TAIL)
	{
		return FALSE;
	}
	if(((pu8Data[DataLen-3]<<8) + pu8Data[DataLen-2]) != (u16)EDMI_Crc_Cala(pu8Data,DataLen-3))
	{
		return FALSE;
	}

	if(pu8Data[12] == 0x18)//������
	{
		return FALSE;
	}
	
	(*pu8DataLen) = DataLen;
	return TRUE;
}

/**--------------------------------------------------------------------
*��    ��:��������¼֡
*�������:pu8Addr:�ն˵�ַ,u8Pseq:֡���к�
*�������:pu8OutLen�����֡���ȣ�pu8OutBuf���������
*��    ��:�ɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_MakeLogInFrame(u8* pu8Addr,u8* pu8Pseq,u16 u16OutBufSize,u16* pu16OutLen,u8* pu8OutBuf)
{
    u8 u8Index = 0;
    u32 u32Ident = 0xE0001000;

    if(u16OutBufSize < 26)
    {
        return FALSE;
    }
    (*pu8Pseq) ++;
    if((*pu8Pseq) > 0x0F)
    {
        (*pu8Pseq) = 0;
    }
    pu8OutBuf[u8Index] = QCSG_FRAME_HEAD;
    u8Index++;
    pu8OutBuf[u8Index] = 1 + 7 + 1 + 1 + 2 + 4 + 2;//C+A+AFN+SEQ+DA+IDENT+DATA(2)
    u8Index++;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    pu8OutBuf[u8Index] = 1 + 7 + 1 + 1 + 2 + 4 + 2;//C+A+AFN+SEQ+DA+IDENT+DATA(2)
    u8Index++;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    pu8OutBuf[u8Index] = QCSG_FRAME_HEAD;
    u8Index++;
    //C
    pu8OutBuf[u8Index] = C_LINK_ACTREPORT;
    u8Index++;
    //A
    MemCopy(&pu8OutBuf[u8Index],pu8Addr,6);
    u8Index+=6;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    //AFN
    pu8OutBuf[u8Index] = 0x02;
    u8Index++;
    //SEQ
    pu8OutBuf[u8Index] = 0x10 | (*pu8Pseq);
    u8Index++;
    //DA
    MemSet(&pu8OutBuf[u8Index],0,2);
    u8Index+=2;
    //IDENT
    MemCopy(&pu8OutBuf[u8Index],(u8*)&u32Ident,4);
    u32Ident+=4;
    //��Լ�汾��
    pu8OutBuf[u8Index] = 0x00;
    u8Index++;
    pu8OutBuf[u8Index] = 0x01;
    u8Index++;
    //CS
    pu8OutBuf[u8Index] = (u8)SumCaculation(&pu8OutBuf[6],u8Index - 6);
    u8Index++;
    pu8OutBuf[u8Index] = QCSG_FRAME_TAIL;
    u8Index++;
    (*pu16OutLen) = u8Index;
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:��������֡
*�������:pu8Addr:�ն˵�ַ,u8Pseq:֡���к�
*�������:pu8OutLen�����֡���ȣ�pu8OutBuf���������
*��    ��:�ɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_MakeHeartBeatFrame(u8* pu8Addr,u8* pu8Pseq,u16 u16OutBufSize,u16* pu16OutLen,u8* pu8OutBuf)
{
    u8 u8Index = 0;
    u32 u32Ident = 0xE0001001;

    if(u16OutBufSize < 26)
    {
        return FALSE;
    }
    (*pu8Pseq) ++;
    if((*pu8Pseq) > 0x0F)
    {
        (*pu8Pseq) = 0;
    }
    pu8OutBuf[u8Index] = QCSG_FRAME_HEAD;
    u8Index++;
    pu8OutBuf[u8Index] = 1 + 7 + 1 + 1 + 2 + 4 + 2;//C+A+AFN+SEQ+DA+IDENT+DATA(2)
    u8Index++;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    pu8OutBuf[u8Index] = 1 + 7 + 1 + 1 + 2 + 4 + 2;//C+A+AFN+SEQ+DA+IDENT+DATA(2)
    u8Index++;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    pu8OutBuf[u8Index] = QCSG_FRAME_HEAD;
    u8Index++;
    //C
    pu8OutBuf[u8Index] = C_LINK_ACTREPORT;
    u8Index++;
    //A
    MemCopy(&pu8OutBuf[u8Index],pu8Addr,6);
    u8Index+=6;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    //AFN
    pu8OutBuf[u8Index] = 0x02;
    u8Index++;
    //SEQ
    pu8OutBuf[u8Index] = 0x10 | (*pu8Pseq);
    u8Index++;
    //DA
    MemSet(&pu8OutBuf[u8Index],0,2);
    u8Index+=2;
    //IDENT
    MemCopy(&pu8OutBuf[u8Index],(u8*)&u32Ident,4);
    u32Ident+=4;
    //CS
    pu8OutBuf[u8Index] = (u8)SumCaculation(&pu8OutBuf[6],u8Index - 6);
    u8Index++;
    pu8OutBuf[u8Index] = QCSG_FRAME_TAIL;
    u8Index++;
    (*pu16OutLen) = u8Index;
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:�������˳���¼֡
*�������:pu8Addr:�ն˵�ַ,u8Pseq:֡���к�
*�������:pu8OutLen�����֡���ȣ�pu8OutBuf���������
*��    ��:�ɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_MakeLogOutFrame(u8* pu8Addr,u8* pu8Pseq,u16 u16OutBufSize,u16* pu16OutLen,u8* pu8OutBuf)
{
    u8 u8Index = 0;
    u32 u32Ident = 0xE0001002;

    if(u16OutBufSize < 26)
    {
        return FALSE;
    }
    (*pu8Pseq) ++;
    if((*pu8Pseq) > 0x0F)
    {
        (*pu8Pseq) = 0;
    }
    pu8OutBuf[u8Index] = QCSG_FRAME_HEAD;
    u8Index++;
    pu8OutBuf[u8Index] = 1 + 7 + 1 + 1 + 2 + 4 + 2;//C+A+AFN+SEQ+DA+IDENT+DATA(2)
    u8Index++;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    pu8OutBuf[u8Index] = 1 + 7 + 1 + 1 + 2 + 4 + 2;//C+A+AFN+SEQ+DA+IDENT+DATA(2)
    u8Index++;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    pu8OutBuf[u8Index] = QCSG_FRAME_HEAD;
    u8Index++;
    //C
    pu8OutBuf[u8Index] = C_LINK_ACTREPORT;
    u8Index++;
    //A
    MemCopy(&pu8OutBuf[u8Index],pu8Addr,6);
    u8Index+=6;
    pu8OutBuf[u8Index] = 0;
    u8Index++;
    //AFN
    pu8OutBuf[u8Index] = 0x02;
    u8Index++;
    //SEQ
    pu8OutBuf[u8Index] = 0x10 | (*pu8Pseq);
    u8Index++;
    //DA
    MemSet(&pu8OutBuf[u8Index],0,2);
    u8Index+=2;
    //IDENT
    MemCopy(&pu8OutBuf[u8Index],(u8*)&u32Ident,4);
    u32Ident+=4;
    //CS
    pu8OutBuf[u8Index] = (u8)SumCaculation(&pu8OutBuf[6],u8Index - 6);
    u8Index++;
    pu8OutBuf[u8Index] = QCSG_FRAME_TAIL;
    u8Index++;
    (*pu16OutLen) = u8Index;
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:֪ͨͨ����������
*�������:u8ChNo��ͨ����
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_NoticeReconnect(u8 u8ChNo)
{

}
/**--------------------------------------------------------------------
*��    ��:֪ͨ��Ҫ�������,����Э�����ò���ʱ����
*�������:u8ChNo��ͨ����
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_NoticeDIChange(QCSG_DICHANGE_E eDi,u8 u8Flag)
{
	u8* pu8Value = (u8*)QCSG_DICHARGE_FLAG_LIST;
	pu8Value[eDi] = u8Flag;
}
/**--------------------------------------------------------------------
*��    ��:��ȡ���������Ϣ�����Ƽ�������ʱ����
*�������:u8ChNo��ͨ����
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
u8 QCSG_GetDIChange(QCSG_DICHANGE_E eDi)
{
	u8* pu8Value = (u8*)QCSG_DICHARGE_FLAG_LIST;
	return pu8Value[eDi];
}
/**--------------------------------------------------------------------
*��    ��:������Ӧ֡��֯������
*�������:u8ChNo:����ͨ����u8FrameFlag��ͬFIR��FIN���壬pstQCSG_Info��Э�黺��
*�������:��
*��    ��:���ͳɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_ResLinkFrameSend(u8 u8ChNo,u8 u8FrameFlag,QCSG_RESOLVE_INFO_S* pstQCSG_Info)
{
    u16 u16Index = 0;

    if(pstQCSG_Info->u16TxLen == 0x00)
    {
        return FALSE;
    }
    pstQCSG_Info->u16TxLField = 1 + 7 + 1 + 1 + pstQCSG_Info->u16TxLen;//C+A+AFN+SEQ+DATA
    pstQCSG_Info->u8TxCField = (0x80 | 0x08);
    pstQCSG_Info->u8TxAFNField = pstQCSG_Info->u8RxAFNField;
    pstQCSG_Info->u8SendBuf[u16Index] = QCSG_FRAME_HEAD;
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = (u8)(pstQCSG_Info->u16TxLField);
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = (u8)(pstQCSG_Info->u16TxLField >> 8);
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = (u8)(pstQCSG_Info->u16TxLField);
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = (u8)(pstQCSG_Info->u16TxLField >> 8);
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = QCSG_FRAME_HEAD;
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = pstQCSG_Info->u8TxCField;
    u16Index++;
    MemCopy(&pstQCSG_Info->u8SendBuf[u16Index],pstQCSG_Info->u8RxAddrField,7);
    u16Index+=7;
    pstQCSG_Info->u8SendBuf[u16Index] = pstQCSG_Info->u8TxAFNField;
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = (pstQCSG_Info->u8RxSEQField & 0x0F) | u8FrameFlag;
    u16Index++;
    MemCopy(&pstQCSG_Info->u8SendBuf[u16Index],pstQCSG_Info->u8TxBuf,pstQCSG_Info->u16TxLen);
    u16Index+=pstQCSG_Info->u16TxLen;
    pstQCSG_Info->u8SendBuf[u16Index] = (u8)SumCaculation(&pstQCSG_Info->u8SendBuf[6],u16Index - 6);
    u16Index++;
    pstQCSG_Info->u8SendBuf[u16Index] = QCSG_FRAME_TAIL;
    u16Index++;
    pstQCSG_Info->u16SendLen = u16Index;
    /*
     * TODO:���ݷ��ͺ���������
     */
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:�����ϱ�֡��֯������
*�������:u8ChNo:����ͨ����u8FrameFlag��ͬFIR��FIN���壬pstReport_Info��Э�黺��
*�������:��
*��    ��:���ͳɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_ReqLinkFrameSend(u8 u8ChNo,u8 u8FrameFlag,QCSG_ACTIVEREPORT_S* pstReport_Info)
{
    u16 u16Index = 0;

    if(pstReport_Info->stReport_Info.u16TxLen == 0)
    {
        return FALSE;
    }
    pstReport_Info->u8ReportPseq ++;
    if(pstReport_Info->u8ReportPseq > 0x0F)
    {
        pstReport_Info->u8ReportPseq = 0;
    }
    pstReport_Info->stReport_Info.u16TxLField = 1 + 7 + 1 + 1 + pstReport_Info->stReport_Info.u16TxLen;//C+A+AFN+SEQ+DATA
    pstReport_Info->stReport_Info.u8TxCField = (0x80 | 0x40 | 0x0B);
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = QCSG_FRAME_HEAD;
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = (u8)(pstReport_Info->stReport_Info.u16TxLField);
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = (u8)(pstReport_Info->stReport_Info.u16TxLField >> 8);
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = (u8)(pstReport_Info->stReport_Info.u16TxLField);
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = (u8)(pstReport_Info->stReport_Info.u16TxLField >> 8);
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = QCSG_FRAME_HEAD;
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = pstReport_Info->stReport_Info.u8TxCField;
    u16Index++;
    MemCopy(&pstReport_Info->stReport_Info.u8SendBuf[u16Index],pstReport_Info->stReport_Info.u8RxAddrField,7);
    u16Index+=7;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = pstReport_Info->stReport_Info.u8TxAFNField;
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = (pstReport_Info->u8ReportPseq & 0x0F) | u8FrameFlag;
    u16Index++;
    MemCopy(&pstReport_Info->stReport_Info.u8SendBuf[u16Index],pstReport_Info->stReport_Info.u8TxBuf,pstReport_Info->stReport_Info.u16TxLen);
    u16Index+=pstReport_Info->stReport_Info.u16TxLen;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = (u8)SumCaculation(&pstReport_Info->stReport_Info.u8SendBuf[6],u16Index - 6);
    u16Index++;
    pstReport_Info->stReport_Info.u8SendBuf[u16Index] = QCSG_FRAME_TAIL;
    u16Index++;
    pstReport_Info->stReport_Info.u16SendLen = u16Index;
    /*
     * TODO:���ݷ��ͺ���������
     */
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:��ȡ����Э��֡
*�������:u16InLen:���뻺�峤�ȣ�pu8InBuf:���뻺��
*�������:pu16OutLen�����֡���ȣ�pu8OutBuf���������
*��    ��:��ȡ�ɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_TakeProtocolFrame(u16 u16InLen,u8* pu8InBuf,u16* pu16OutLen,u8* pu8OutBuf)
{
    u16 i = 0;
    u16 u16Len1 = 0,u16Len2 = 0,u16FrameLen = 0;
    u8 u8crc = 0;

    while(1)
    {
        if((pu8InBuf[i] == 0x68)&&(pu8InBuf[i + 5] == 0x68))
        {
            u16Len1 = pu8InBuf[i + 1] + pu8InBuf[i + 2] * 0x100;
            u16Len2 = pu8InBuf[i + 3] + pu8InBuf[i + 4] * 0x100;
            if(u16Len1 != u16Len2)
            {
                i++;
                if(i >= u16InLen)
                {
                    break;
                }
                continue;
            }
            if((i + u16Len1 + 2) >= u16InLen)
            {
                i++;
                if(i >= u16InLen)
                {
                    break;
                }
                continue;
            }
            if(pu8InBuf[i + 5 + u16Len1 + 2] != 0x16)
            {
                i++;
                if(i >= u16InLen)
                {
                    break;
                }
                continue;
            }
            u8crc = (u8)SumCaculation(&pu8InBuf[i + 6],u16Len1);
            if(u8crc != pu8InBuf[i + 5 + u16Len1 + 1])
            {
                i++;
                if(i >= u16InLen)
                {
                    break;
                }
                continue;
            }
        }
        else
        {
            i++;
            if(i >= u16InLen)
            {
                break;
            }
        }
    }
    if(i >= u16InLen)
    {
        return FALSE;
    }
    u16FrameLen = u16Len1 + 8;
    MemCopy(pu8OutBuf,&pu8InBuf[i],u16FrameLen);
    *pu16OutLen = u16FrameLen;
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:��ȡ����Э��֡�йؼ�Ԫ��
*�������:u16InLen:���볤�ȣ�pu8InBuf�����뻺��
*�������:pstQCSG_Info
*��    ��:��ȡ�ɹ�orʧ��
--------------------------------------------------------------------**/
u8 QCSG_TakeProtocolInfo(u16 u16InLen,u8* pu8InBuf,QCSG_RESOLVE_INFO_S* pstQCSG_Info)
{
    u16 u16DataLen = 0;
    u8 u8Afn = 0;

    u16DataLen = pu8InBuf[1] + pu8InBuf[2] * 0x100;
    if(u16DataLen > MAX_QCSG_FRAME_LEN)
    {
        return FALSE;
    }
    u8Afn = pu8InBuf[14];
    if(u8Afn > 0x15)
    {
        return FALSE;
    }
    pstQCSG_Info->u16RxLField = u16DataLen;
    pstQCSG_Info->u8RxCField = pu8InBuf[6];
    pstQCSG_Info->u8RxAFNField = pu8InBuf[14];
    pstQCSG_Info->u8RxSEQField = pu8InBuf[15];
    MemCopy(pstQCSG_Info->u8RxAddrField,&pu8InBuf[7],7);
    return TRUE;
}
/**--------------------------------------------------------------------
 *��    ��:�ж�Э���ַ�Ƿ�Ϸ�
 *�������:pu8Addr����ַ
 *�������:��
 *��    ��:�Ƿ�Ϸ�
 --------------------------------------------------------------------**/
u8 QCSG_Chek_Addr(u8* pu8Addr)
{
    /*
     * TODO:�����䣬��������ַ����δȷ��
     */
    return TRUE;
}
/**--------------------------------------------------------------------
 *��    ��:�ж�ʱ���ǩ
 *�������:pu8Data������
 *�������:pstQ13761_Info:�������
 *��    ��:�Ƿ�Ϸ�
 --------------------------------------------------------------------**/
u8 QCSG_Chek_Tpv(u8* pu8Data,QCSG_RESOLVE_INFO_S* pstQCSG_Info)
{
    /*
     * TODO:ϵͳʱ���ȡ����û��ȷ������ɴ˺���
     */
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:��ȡPn
*�������:pu8Buf:����
*�������:��
*��    ��:Pn
--------------------------------------------------------------------**/
u16 QCSG_TakePn(u8* pu8Buf)
{
    //u8 DA2 = 0,DA1 = 0;
    u16 u16Pn = 0;
/*
    DA2 = pu8Buf[1];
    DA1 = pu8Buf[0];
    if((DA2 == 0)&&(DA1 != 0))
    {
        //��Ч������
        return 0xfffe;
    }
    if((DA2 == 0xff)&&(DA1 == 0xff))
    {
        //ȫ��������
        return 0xffff;
    }
    u16Pn = (u16)DA2;
    u16Pn = u16Pn << 8;
    u16Pn = u16Pn + DA1;
    if(u16Pn > PN_MAX)
    {
        //��Ч������
        return 0xfffe;
    }
*/
    u16Pn += pu8Buf[1];
    u16Pn <<= 8;
    u16Pn += pu8Buf[0];
    return u16Pn;
}
/**--------------------------------------------------------------------
*��    ��:��ȡFn
*�������:pucBuf:����
*�������:��
*��    ��:Fn
--------------------------------------------------------------------**/
u32 QCSG_TakeFn(u8* pu8Buf)
{
    u32 u32Fn = 0;

    //u32Fn = pu8Buf[0] + pu8Buf[1] * 0x100 + pu8Buf[2] * 0x10000 + pu8Buf[3] * 0x1000000;
    u32Fn += pu8Buf[3];
	u32Fn <<= 8;
	u32Fn += pu8Buf[2];
	u32Fn <<= 8;
	u32Fn += pu8Buf[1];
	u32Fn <<= 8;
	u32Fn += pu8Buf[0];
    return u32Fn;
}
/**--------------------------------------------------------------------
*��    ��:�ж�AFN04��Fn�Ƿ�֧��
*�������:u32Fn������Fn
*�������:pu16Index:�������±�
*��    ��:�������
--------------------------------------------------------------------**/
u8 QCSG_Check_AFN04_0AFn(u32 u32Fn,u16* pu16Index)
{
    u16 i = 0,u16FnNum = 0;

    u16FnNum = MAX_AFN040A_IDENT_NUM;
    if((u32Fn & 0xFFFFFF00) == 0xE0000300)
    {
        //�����������������⴦��
        u32Fn = 0xE00003FF;
    }
    if((u32Fn & 0xFFFFFF00) == 0xE0000400)
    {
        //�����������������⴦��
        u32Fn = 0xE00004FF;
    }
    for(i = 0;i < u16FnNum;i++)
    {
        if(u32Fn == gc_stQCSGResolve_AFN04[i].u32Fn)
        {
            break;
        }
    }
    if(i >= u16FnNum)
    {
        return FALSE;
    }
    *pu16Index = i;
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:�жϸ澯�Ƿ�����
*�������:u32AlarmIdent:�澯��ʶ
*�������:��
*��    ��:TRUE�����ã�FALSE��δ����
--------------------------------------------------------------------**/
u8 QCSG_CheckAlarmConfig(u32 DI)
{
	AlarmEventMaskParaStruct AlarmMaskPara;
	u32 MaskPos = 0, MaskBit = 0;
	
	if((DI == 0x00)||(DI > 0xE200FFFF)) return FALSE;
	MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));

	MaskPos = (DI & 0x000000FF) - 1;
	MaskBit = 1 << (MaskPos & 0x00000007);
	MaskPos >>= 3;	
	
	if((AlarmMaskPara.AlarmJudgeMask[MaskPos] & MaskBit) == 0) return FALSE;

	return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:�жϸ澯�ϱ��Ƿ�����
*�������:u32AlarmIdent:�澯��ʶ
*�������:��
*��    ��:TRUE�����ã�FALSE��δ����
--------------------------------------------------------------------**/
u8 QCSG_CheckAlarmReport(u32 DI)
{
	AlarmEventMaskParaStruct AlarmMaskPara;
	u32 MaskPos = 0, MaskBit = 0;
	
	if((DI == 0x00)||(DI > 0xE200FFFF)) return FALSE;
	MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
	
	MaskPos = (DI & 0x000000FF) - 1;
	MaskBit = 1 << (MaskPos & 0x00000007);
	MaskPos >>= 3;	
	if((AlarmMaskPara.AlarmReportMask[MaskPos] & MaskBit) == 0) return FALSE;

	return TRUE;
}

u8 QCSG_CheckEventMask(u32 DI)
{
	AlarmEventMaskParaStruct AlarmMaskPara;
	u32 MaskPos = 0, MaskBit = 0;
	
	if((DI == 0x00)||(DI > 0xE20100FF)) return FALSE;
	MR((u32)&AlarmMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
	
	MaskPos = (DI & 0x000000FF) - 1;
	MaskBit = 1 << (MaskPos & 0x00000007);
	MaskPos >>= 3;	
	if((AlarmMaskPara.EventRecordMask[MaskPos] & MaskBit) == 0) return FALSE;

	return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:���ݶ˿ںŶ�ȡ�м�����
*�������:u8Port:�˿ںţ�u8Dimnumber�����������С
*�������:pu8GetCount����ȡ����������pu8TaskList�������б�,0,1,2,3...
*��    ��:��ȡ���
--------------------------------------------------------------------**/
u8 QCSG_GetRelayTaskByPort(u8 u8Port,u8 u8Dimnumber,u8* pu8GetCount,u8* pu32TaskList)
{
    u32 i=0;
    u32 FlashAddr;
    u32 ReadLen = sizeof(RelayTaskParaStruct);
    RelayTaskParaStruct RelayTaskParam;
    *pu8GetCount=0;
    for(i=0;i<0xff;i++)
    {
        FlashAddr = ADDR_RelayTaskPara + (i * sizeof(RelayTaskParaStruct));
        MR((u32)&RelayTaskParam, FlashAddr, ReadLen);
        if((RelayTaskParam.Flag==1)&&(RelayTaskParam.ComPortId==u8Port))
        {
            pu32TaskList[*pu8GetCount]=(u8)i + 1;
            *pu8GetCount +=1;
            if(*pu8GetCount >u8Dimnumber)
            {
                break;
            }
        }
    }
    return TRUE;
}
/**--------------------------------------------------------------------
*��    ��:��ȡ�˿��µ�Pn�б�
*�������:u8Port:�˿ں�
*�������:pu16PnList,pu8PnNum
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_GetPnListByPort(u8 u8Port,u8 u8MaxListNum,u16* pu16PnList,u16* pu16PnNum)
{
    MeasurePointStruct  MeasurePoint;
    u32 i=0;
    u32 offset=0,flashLen=0;
    *pu16PnNum=0;
    for(i = 0; i<MAX_PN_NUM;i++)
    {
        offset = ADDR_MP_START + i * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
        flashLen= sizeof(MeasurePointStruct) ;
        MR((u32)&MeasurePoint, offset, flashLen);
        if(MeasurePoint.State==0)
        {
            continue;
        }
		#if ((Project/100)==9)//err20181212 �������ƣ�ֻ�������Ҫ����Ϊ�ܱ�
			if(MeasurePoint.TotalType == 0)//err20181017 ��� �ܱ���ø��ش�������澯
			{
				continue;
			}
		#endif
        if(MeasurePoint.PortID==u8Port)
        {
            pu16PnList[*pu16PnNum] = (u16)SwitchNumToPn(i);
            *pu16PnNum +=1;
        }
        if( *pu16PnNum >u8MaxListNum)
        {
            break;
        }
    }
}
/**--------------------------------------------------------------------
*��    ��:����Pn�б�
*�������:u8Port:�˿ں�
*�������:pu16PnList,pu8PnNum
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_UpdatePnList(u8 u8Port,u16 u8MaxListNum,u16* pu16PnList,u16* pu16PnNum)
{
	u8 i = 0,j = 0;
	u16 u16PnNum = 0;
	u16 u16PnList[MAX_PN_NUM];
	
	QCSG_GetPnListByPort(u8Port,u8MaxListNum,u16PnList,&u16PnNum);
	for(i = 0;i < u16PnNum;i++)
	{
		for(j = 0;j < (*pu16PnNum);j++)
		{
			if(u16PnList[i] == pu16PnList[j])
			{
				break;
			}
		}
		if(j >= (*pu16PnNum))
		{
			pu16PnList[*pu16PnNum] = u16PnList[i];
			(*pu16PnNum)++;
		}
	}
}
/**--------------------------------------------------------------------
*��    ��:�����м������б�
*�������:u8Port:�˿ںţ�pstRelayTask_List
*�������:pstRelayTask_List
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_UpdateRelayTaskList(u8 u8Port,QCSG_RDRELAYTASK_INFO_S* pstRelayTask_List)
{
    u8 i = 0,j = 0;
    u8 u8NewTaskId = 0,u8OldTaskId = 0;

    //����ִ�е������б�����ǰ�ı�����
    if(QCSG_GetRelayTaskByPort(u8Port,254,&pstRelayTask_List->u8NewRelayTaskNum,pstRelayTask_List->u8NewRelayTaskList) != TRUE)
    {
        return;
    }
    for(i = 0;i < pstRelayTask_List->u8NewRelayTaskNum;i++)
    {
        u8NewTaskId = pstRelayTask_List->u8NewRelayTaskList[i];
        for(j = 0;j < pstRelayTask_List->u8RelayTaskNum;i++)
        {
            u8OldTaskId = pstRelayTask_List->u8RelayTaskList[j];
            if(u8NewTaskId == u8OldTaskId)
            {
                break;
            }
        }
        if(j >= pstRelayTask_List->u8RelayTaskNum)
        {
            pstRelayTask_List->u8RelayTaskList[pstRelayTask_List->u8RelayTaskNum] = u8NewTaskId;
            pstRelayTask_List->u8RelayTaskNum++;
        }
    }
}
/**--------------------------------------------------------------------
*��    ��:������ͨ����
*�������:pstCommonTask_List
*�������:pstCommonTask_List
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_UpdateCommonTaskList(QCSG_RDCOMMONTASK_INFO_S* pstCommonTask_List)
{
	NormalTaskParaStruct stNormalTaskPara;
	u8 i = 0,j = 0;
	u8 u8NewTaskId = 0,u8OldTaskId = 0;
	u32 u32Ident = 0xE0000301,u32ReadLen = 0;
	pstCommonTask_List->u8NewCommonTaskNum = 0;
	for(i = 0;i < MAX_NORMAL_TASK_NUM;i++)
	{
		u32Ident += i;
		if(NAND_ReadPara(PARA_Task,0,u32Ident,(u8*)&stNormalTaskPara,&u32ReadLen) != NO_ERROR)		
		{
			continue;
		}
		if(stNormalTaskPara.Flag != 1)
		{
			continue;
		}
		if(stNormalTaskPara.DANum == 0)
		{
			continue;
		}
		if(stNormalTaskPara.DataIDNum == 0)
		{
			continue;
		}
		if(stNormalTaskPara.DataIDNum > AMR_MAX_DATA_ID)
		{
			continue;
		}
		if(stNormalTaskPara.SamplePeriod == 0)
		{			
			continue;
		}
		if(TimeValidCheck(&stNormalTaskPara.ReportTime) != NO_ERROR)
		{
			continue;
		}	
		if(TimeValidCheck(&stNormalTaskPara.SampleTime) != NO_ERROR)
		{
			continue;
		}
		pstCommonTask_List->u8NewCommonTaskList[pstCommonTask_List->u8NewCommonTaskNum] = i + 1;
		pstCommonTask_List->u8NewCommonTaskNum++;		
	}
	for(i = 0;i < pstCommonTask_List->u8NewCommonTaskNum;i++)
	{
		u8NewTaskId = pstCommonTask_List->u8NewCommonTaskList[i];
		for(j = 0;j < pstCommonTask_List->u8CommonTaskNum;j++)
		{
			u8OldTaskId = pstCommonTask_List->u8CommonTaskList[j];
			if(u8NewTaskId == u8OldTaskId)
			{
				break;
			}
		}
		if(j >= pstCommonTask_List->u8CommonTaskNum)
		{
            pstCommonTask_List->u8CommonTaskList[pstCommonTask_List->u8CommonTaskNum] = u8NewTaskId;
            pstCommonTask_List->u8CommonTaskNum++;			
		}
	}

}
/**--------------------------------------------------------------------
*��    ��:�ж�����ʱʱ���Ƿ�
*�������:pstCurrTime��pstRefTime��u8PeriodValue,u8PeriodUnit
*�������:��
*��    ��:ʱ���Ƿ�
--------------------------------------------------------------------**/
u8 QCSG_Judge_TaskTiming(TIME_PARAM_S* pstLastTime,TIME_PARAM_S* pstCurrTime,TIME_PARAM_S* pstRefTime,u8 u8PeriodValue,u8 u8PeriodUnit)
{
    TIME_PARAM_S stLastTime;
    TIME_PARAM_S stCurrTime;
    TIME_PARAM_S stRefTime;
    u32 au32Result[4];
    u32 u32PeriodMinute = 0,u32IntervalMinute = 0,u32PeriodSec = 0,u32IntervalSec = 0;
    u8 i= 0;

    MemCopy(&stLastTime,pstLastTime,sizeof(TIME_PARAM_S));
    MemCopy(&stCurrTime,pstCurrTime,sizeof(TIME_PARAM_S));
    MemCopy(&stRefTime,pstRefTime,sizeof(TIME_PARAM_S));
    stCurrTime.u8Second = 0;
    stRefTime.u8Second = 0;
    if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result) < 0)
    {
        //��ǰʱ��С�ڲο�ʱ�䣬����ʱ��δ��
        return FALSE;
    }
    if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result) == 0)
    {
        //���˲ο�ʱ��
        return TRUE;
    }
    if(u8PeriodUnit == 3)
    {
        //���ڵ�λΪ��
        while(1)
        {
            for(i=0;i<u8PeriodValue;i++)
            {
                QCSG_IncOneMonth(&stRefTime);
            }
            if((stCurrTime.u8Year == stRefTime.u8Year)&&(stRefTime.u8Month == stCurrTime.u8Month))
            {
                break;
            }
            if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result) < 0)
            {
                //ʱ��ӹ���˵��δ���ϱ�ʱ��
                return FALSE;
            }
        }
        if((stCurrTime.u8Day == stRefTime.u8Day)&&(stCurrTime.u8Hour == stRefTime.u8Hour))
        {
            if((stCurrTime.u8Month != stLastTime.u8Month))
            {
                //��һ��ִ�е��²�һ����˵����������
                return TRUE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    if(u8PeriodUnit == 2)
    {
        //ʱ�䵥λΪ��
        while(1)
        {
            for(i=0;i<u8PeriodValue;i++)
            {
                QCSG_IncOneDay(&stRefTime);
            }
            if((stCurrTime.u8Year == stRefTime.u8Year)&&(stRefTime.u8Month == stCurrTime.u8Month)&&(stCurrTime.u8Day == stRefTime.u8Day))
            {
                break;
            }
            if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result) < 0)
            {
                //ʱ��ӹ���˵��δ���ϱ�ʱ��
                return FALSE;
            }
        }
        if(stCurrTime.u8Hour == stRefTime.u8Hour)
        {
            if((stCurrTime.u8Day != stLastTime.u8Day))
            {
                //��һ��ִ�е��ղ�һ����˵����������
                return TRUE;
            }
        }
        else
        {
            return FALSE;
        }
    }
    if(u8PeriodUnit == 1)
    {
        //ʱ�䵥λΪСʱ
        if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result)>0)
        {
            u32PeriodMinute = u8PeriodValue * 60;
            u32IntervalMinute = au32Result[0] * 24 * 60 + au32Result[1] * 60 + au32Result[2];
            if((u32IntervalMinute % u32PeriodMinute) == 0)
            {
                //���õ�����
                return TRUE;
            }
            else if((u32IntervalMinute % u32PeriodMinute) <= 10)
            {
                //��ֹ������ڣ���λΪСʱ�����񣬵����ں��10min�ڣ�����Ϊ����Ч����ִ��ʱ��
                if((stCurrTime.u8Hour != stLastTime.u8Hour))
                {
                    //��һ��ִ�е�Сʱ��һ����˵����������
                    return TRUE;
                }
            }
            else
            {
                return FALSE;
            }
        }
    }
    if(u8PeriodUnit == 0)
    {
        //ʱ�䵥λΪ��
        if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result)>0)
        {
            u32PeriodSec = u8PeriodValue * 60;
            u32IntervalSec = au32Result[0] * 24 * 60 * 60 + au32Result[1] * 60 * 60 + au32Result[2] * 60;
            if((u32IntervalSec % u32PeriodSec) == 0)
            {
                return TRUE;
            }
            else if((u32IntervalSec % u32PeriodSec) <= ((u8PeriodValue - 2) * 60))
            {
                //��ֹ������ڣ���λΪ�ֵ����񣬵����ں��u8PeriodValue-1min�ڣ�����Ϊ����Ч����ִ��ʱ��
                if(QCSG_TimerSub(&stCurrTime,&stLastTime,au32Result)>0)
                {
                    if((au32Result[0] != 0) || (au32Result[1] != 0) ||(au32Result[2] >= u8PeriodValue))
                    {
                        //�ϴ�ִ��ʱ���뵱ǰʱ��ȣ�����һ��ִ�����ڣ�˵����������
                        return TRUE;
                    }
                }
            }
            else
            {
                return FALSE;
            }
        }
    }
    return FALSE;
}
/**--------------------------------------------------------------------
*��    ��:�ж����ݱ�ʶ�Ƿ��Ƿǳ�������
*�������:u32Ident
*�������:��
*��    ��:�Ƿ��Ƿǳ�������
--------------------------------------------------------------------**/
u8 QCSG_Check_RDIdent(u32 u32Ident)
{
	if((u32Ident == 0x03100000)||(u32Ident == 0x03100100)||(u32Ident == 0x03100200)||(u32Ident == 0x03100300))
	{
		return TRUE;
	}
	//if((u32Ident & 0xFF000000) == 0xE1000000)
	if((u32Ident & 0xFF000000) == 0xE1000000&&u32Ident!=0xE1000008)	//ʣ���Ӧ��ͳ���ദ��
	{
		return TRUE;
	}
	return FALSE;
}
/**--------------------------------------------------------------------
*��    ��:����AFN0C��AFN0D���ݱ�ʶת��Ϊ������645���ݱ�ʶ
*�������:u32NwIdent��u8ProtocolType
*�������:pu8IdentNum��pu32IdentList
*��    ��:�Ƿ���ȡ�ɹ�
--------------------------------------------------------------------**/
u8 QCSG_GetDlt645Ident_By_AFN0C0DTable(u32 u32NwIdent,u8 u8ProtocolType,u8* pu8IdentNum,u32* pu32IdentList,u8* pu8ConvertMethodList,u8* pu8LenList)
{
	u8 j = 0;
	u16 i = 0;
	(*pu8IdentNum) = 0;
	
	if(u8ProtocolType == METER_DLT645_97)
	{
		for(i = 0;i < MAX_AFN0C_0D_IDENT_NUM;i++)
		{
			if(u32NwIdent == gc_stQCSGAFN0C_0D_DLT645_Table[i].u32Fn)
			{
				for(j = 0;j < 12;j++)
				{
					if(gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u32Ident != 0xFFFFFFFF)
					{
						MemCopy(&pu32IdentList[(*pu8IdentNum)],&gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u32Ident,4);
						pu8ConvertMethodList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u8TakeMethod;
						pu8LenList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u8BlDataLen;
						(*pu8IdentNum)++;
					}
					else
					{
						break;
					}
				}
				if(j == 0)
				{
					pu8ConvertMethodList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u8TakeMethod;	
				}
				break;
			}
		}
	}
	if(u8ProtocolType == METER_DLT645_07)
	{
		for(i = 0;i < MAX_AFN0C_0D_IDENT_NUM;i++)
		{
			if(u32NwIdent == gc_stQCSGAFN0C_0D_DLT645_Table[i].u32Fn)
			{
				for(j = 0;j < 12;j++)
				{
					if(gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u32Ident != 0xFFFFFFFF)
					{
						MemCopy(&pu32IdentList[(*pu8IdentNum)],&gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u32Ident,4);
						pu8ConvertMethodList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u8TakeMethod;
						pu8LenList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u8BlDataLen;
						(*pu8IdentNum)++;
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
	}
	if((*pu8IdentNum) > 0)
	{
		return TRUE;
	}
	return FALSE;
}

/**--------------------------------------------------------------------
*��    ��:����AFN0C��AFN0D���ݱ�ʶת��Ϊ������645���ݱ�ʶ
*�������:u32NwIdent��u8ProtocolType
*�������:pu8IdentNum��pu32IdentList
*��    ��:�Ƿ���ȡ�ɹ�
--------------------------------------------------------------------**/
u8 QCSG_GetDlt645_EDMI_Ident_By_AFN0C0DTable(u32 u32NwIdent,u8 u8ProtocolType,u8* pu8IdentNum,u32* pu32IdentList,u8* pu8ConvertMethodList,u8* pu8LenList)
{
	u8 j = 0;
	u16 i = 0;
	(*pu8IdentNum) = 0;
	
	if(u8ProtocolType == METER_DLT645_97)
	{
		for(i = 0;i < MAX_AFN0C_0D_IDENT_NUM;i++)
		{
			if(u32NwIdent == gc_stQCSGAFN0C_0D_DLT645_Table[i].u32Fn)
			{
				for(j = 0;j < 12;j++)
				{
					if(gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u32Ident != 0xFFFFFFFF)
					{
						MemCopy(&pu32IdentList[(*pu8IdentNum)],&gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u32Ident,4);
						pu8ConvertMethodList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u8TakeMethod;
						pu8LenList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u8BlDataLen;
						(*pu8IdentNum)++;
					}
					else
					{
						break;
					}
				}
				if(j == 0)
				{
					pu8ConvertMethodList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64597Resolve[j].u8TakeMethod;	
				}
				break;
			}
		}
	}
	if(u8ProtocolType == METER_DLT645_07)
	{
		for(i = 0;i < MAX_AFN0C_0D_IDENT_NUM;i++)
		{
			if(u32NwIdent == gc_stQCSGAFN0C_0D_DLT645_Table[i].u32Fn)
			{
				for(j = 0;j < 12;j++)
				{
					if(gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u32Ident != 0xFFFFFFFF)
					{
						MemCopy(&pu32IdentList[(*pu8IdentNum)],&gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u32Ident,4);
						pu8ConvertMethodList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u8TakeMethod;
						pu8LenList[(*pu8IdentNum)] = gc_stQCSGAFN0C_0D_DLT645_Table[i].stDLT64507Resolve[j].u8BlDataLen;
						(*pu8IdentNum)++;
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
	}
	if(u8ProtocolType == METER_EDMI)
	{
		if(u32NwIdent == 0x00000000)//���⴦�����ݱ�ʶ ��������������δ�����ȫ0�ռ䵼���жϴ��ڸñ�ʶ
			return FALSE;
		for(i = 0;i < MAX_AFN0C_0D_IDENT_NUM;i++)
		{
			if(u32NwIdent == gc_stQCSGAFN0C_0D_EDMI_Table[i].u32Fn)
			{
				pu8ConvertMethodList[0] = gc_stQCSGAFN0C_0D_EDMI_Table[i].u8FixMethod;
				for(j = 0;j < 12;j++)
				{
					if(gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident != 0xFFFFFFFF)
					{
						MemCopy(&pu32IdentList[(*pu8IdentNum)],&gc_stQCSGAFN0C_0D_EDMI_Table[i].stEDMIResolve[j].u32Ident,4);					
						//pu8LenList[(*pu8IdentNum)] = 8;//stEDMIResolve[j].u8BlDataLen;
						(*pu8IdentNum)++;
					}
					else
					{
						break;
					}
				}
				break;
			}
		}
	}
	if((*pu8IdentNum) > 0)
	{
		pu8LenList[0] = gc_stQCSGAFN0C_0D_EDMI_Table[i].u16Dlen;
		return TRUE;
	}
	return FALSE;
}



/**--------------------------------------------------------------------
*��    ��:����645���ݱ�ʶ��ȡ���ݳ��ȣ���Ҫ���ڳ���ش���ʱ��Ч���ݵ����,��E1��ͷ��ͳ�����ݣ��ڳ��������ֱ�Ӵ����
*�������:u32Ident
*�������:pu8Len
*��    ��:�Ƿ���ȡ�ɹ�
--------------------------------------------------------------------**/
u8 QCSG_GetDlt645DataLen_By_Ident(u8 u8AFn,u32 u32Ident,u8* pu8Len)
{
	(*pu8Len) = 0;
	if(u8AFn == 0x0D)
	{
		if(((u32Ident & 0xFFFF0000) == 0x05060000) && ((u32Ident & 0x000000FF) != 0x000000FF))
		{
			//�ն��ᵥ������
			(*pu8Len) = 4;			
		}
		if(((u32Ident & 0xFFFF0000) == 0x05060000) && ((u32Ident & 0x000000FF) == 0x000000FF))
		{
			//�ն���������
			(*pu8Len) = 20;			
		}	
		if(((u32Ident & 0xFFFFFF00) == 0x05060900) && ((u32Ident & 0x000000FF) != 0x000000FF))
		{
			//�ն��������й����������������
			(*pu8Len) = 8;			
		}
		if(((u32Ident & 0xFFFFFF00) == 0x05060900) && ((u32Ident & 0x000000FF) == 0x000000FF))
		{
			//�ն��������й��������
			(*pu8Len) = 40;	
		}
		if(((u32Ident & 0xFFFFFF00) == 0x05060A00) && ((u32Ident & 0x000000FF) != 0x000000FF))
		{
			//�ն��ᷴ���й����������������
			(*pu8Len) = 8;			
		}
		if(((u32Ident & 0xFFFFFF00) == 0x05060A00) && ((u32Ident & 0x000000FF) == 0x000000FF))
		{
			//�ն��ᷴ���й��������
			(*pu8Len) = 40;	
		}				
		if(((u32Ident & 0xFF0000FF) == 0x00000001) && ((u32Ident & 0x0000FF00) != 0x0000FF00))
		{
			//�¶��ᵥ������
			(*pu8Len) = 4;			
		}
		if(((u32Ident & 0xFF0000FF) == 0x00000001) && ((u32Ident & 0x0000FF00) == 0x0000FF00))
		{
			//�¶�����������
			(*pu8Len) = 20;			
		}
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//�������������ݣ����ݳ���Ϊ4
			(*pu8Len) = 4;
		}
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//������������ݣ����ݳ���Ϊ20
			(*pu8Len) = 20;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//��ѹ�������ݣ����ݳ���Ϊ2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//��ѹ������ݣ����ݳ���Ϊ18
			(*pu8Len) = 2 * 9;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//�����������ݣ����ݳ���Ϊ3
			(*pu8Len) = 3;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//����������ݣ����ݳ���Ϊ18
			(*pu8Len) = 3 * 6;
		}	
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//���ʵ������ݣ����ݳ���Ϊ3
			(*pu8Len) = 3;
		}
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//���ʴ�����ݣ����ݳ���Ϊ12
			(*pu8Len) = 3 * 4;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//���������������ݣ����ݳ���Ϊ3
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//��������������ݣ����ݳ���Ϊ8
			(*pu8Len) = 2 * 4;
		}			
	}
	if(u8AFn == 0x0C)
	{
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//�������������ݣ����ݳ���Ϊ4
			(*pu8Len) = 4;
		}
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//������������ݣ����ݳ���Ϊ20
			(*pu8Len) = 20;
		}
		if(((u32Ident & 0xFF000000) == 0x01000000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//�����������ݣ����ݳ���Ϊ8
			(*pu8Len) = 8;
		}
		if(((u32Ident & 0xFF000000) == 0x01000000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//����������ݣ����ݳ���Ϊ40
			(*pu8Len) = 40;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//��ѹ�������ݣ����ݳ���Ϊ2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//��ѹ������ݣ����ݳ���Ϊ18
			(*pu8Len) = 2 * 9;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//�����������ݣ����ݳ���Ϊ3
			(*pu8Len) = 3;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//����������ݣ����ݳ���Ϊ18
			(*pu8Len) = 3 * 6;
		}	
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//���ʵ������ݣ����ݳ���Ϊ3
			(*pu8Len) = 3;
		}
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//���ʴ�����ݣ����ݳ���Ϊ12
			(*pu8Len) = 3 * 4;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//���������������ݣ����ݳ���Ϊ3
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//��������������ݣ����ݳ���Ϊ8
			(*pu8Len) = 2 * 4;
		}	
		if(((u32Ident & 0xFFFF0000) == 0x02070000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//��ǵ������ݣ����ݳ���Ϊ2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02070000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//��Ǵ�����ݣ����ݳ���Ϊ12
			(*pu8Len) = 2 * 6;
		}		
		if((((u32Ident & 0xFFFF0000) == 0x02080000) || ((u32Ident & 0xFFFF0000) == 0x02090000)) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//����ʧ�浥�����ݣ����ݳ���Ϊ2
			(*pu8Len) = 2;
		}
		if((((u32Ident & 0xFFFF0000) == 0x02080000) || ((u32Ident & 0xFFFF0000) == 0x02090000)) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//����ʧ�������ݣ����ݳ���Ϊ6
			(*pu8Len) = 2 * 3;
		}
		if((((u32Ident & 0xFFFF0000) == 0x020A0000) || ((u32Ident & 0xFFFF0000) == 0x020B0000)) && (u32Ident & 0x000000FF) != 0x000000FF)
		{
			//г���������ݣ����ݳ���Ϊ2
			(*pu8Len) = 2;
		}
		if((((u32Ident & 0xFFFF0000) == 0x020A0000) || ((u32Ident & 0xFFFF0000) == 0x020B0000)) && (u32Ident & 0x000000FF) == 0x000000FF)
		{
			//г��������ݣ����ݳ���Ϊ32
			(*pu8Len) = 2 * 21;
		}
		if(((u32Ident & 0xFFFFFF00) == 0x04000500) && (u32Ident & 0x000000FF) != 0x000000FF)
		{
			//�������״̬�֣����ݳ���Ϊ2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFFFF00) == 0x04000500) && (u32Ident & 0x000000FF) == 0x000000FF)
		{
			//�������״̬�֣����ݳ���Ϊ14
			(*pu8Len) = 2 * 7;
		}	
		switch(u32Ident)
		{
			//�޹��ɵ�������
			case 0x02800001:
			case 0x02800003:
			case 0x02800004:	
			case 0x02800005:	
			case 0x02800006:	
			case 0x02800107:
			case 0x02800108:
			case 0x02800109:
			case 0x0280010B:
			case 0x0280010C:
			case 0x0280010D:
			case 0x03300000:
			case 0x03300100:
			case 0x03300200:	
				(*pu8Len) = 3;
				break;	
			case 0x02800002:
			case 0x02800007:
			case 0x02800008:	
			case 0x02800009:	
			case 0x02800101:
			case 0x02800102:
			case 0x02800103:
			case 0x02800104:
			case 0x02800105:
			case 0x02800106:
			case 0x0280010A:
			case 0x0280010E:
				(*pu8Len) = 2;
				break;	
			case 0x0280000A:
			case 0x04000101:
			case 0x04000102:
				(*pu8Len) = 4;
				break;
			case 0x028001FF:
				(*pu8Len) = 32;
				break;
			case 0x03010000:
			case 0x030B0000:
			case 0x030E0000:				
				(*pu8Len) = 18;
				break;
			case 0x03100000:
			case 0x03100100:
			case 0x03100200:
			case 0x03100300:				
				(*pu8Len) = 27;
				break;					

			case 0xe1000008:
				(*pu8Len) = 4;
				break;
		}
	}
	if((*pu8Len) != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}		
}
/**--------------------------------------------------------------------
*��    ��:Э��˿ڵ�Ӳ���˿ڵ�ת������
*�������:u8ChNo
*�������:��
*��    ��:�Ƿ�����
--------------------------------------------------------------------**/
u8 QCSG_PortToHwPort(u8 u8ProtocolPort,u8* pu8HwPort)
{
	u8 u8Res = TRUE;
	switch(u8ProtocolPort)
	{
		case 0:
			(*pu8HwPort) = RS485_1PORT;
			break;
		case 1:
			(*pu8HwPort) = RS485_2PORT;
			break;
		case 2:
			(*pu8HwPort) = RS485_3PORT;
			break;
		default:
			u8Res = FALSE;
			break;
	}
	return u8Res;
}
/**--------------------------------------------------------------------
*��    ��:д�ն�״̬��,д��λ�ð�������Э��λ������
*�������:u8YXState,u8YKState
*�������:��
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_WriteTerminal_State(u8 u8YXState,u8 u8YKState)
{
	u8* pState = (u8*)QCSG_TERMINAL_STATE;
	pState[0] = 0x79;
	pState[1] = 0x00;
	//�����Žӵ�
	if((u8YXState & 0x10) == 0)
	{
		pState[0] &= 0x78;
	}
	//��·YX
	if((u8YXState & 0x01) != 0)
	{
		pState[0] &= 0x77;
	}
	if((u8YXState & 0x02) != 0)
	{
		pState[0] &= 0x6F;
	}
	if((u8YXState & 0x04) != 0)
	{
		pState[0] &= 0x5F;
	}
	if((u8YXState & 0x08) != 0)
	{
		pState[0] &= 0x3F;
	}
	//��·YK
	u8YKState = (~u8YKState);
	u8YKState &= 0x0F;
	u8YKState <<= 3;
	pState[1] |= u8YKState;
}
/**--------------------------------------------------------------------
*��    ��:��ȡ�ն�״̬��
*�������:��
*�������:pu8YXState,pu8YKState
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_ReadTerminal_State(u8* pu8YXState,u8* pu8YKState)
{
	u8* pState = (u8*)QCSG_TERMINAL_STATE;
	(*pu8YXState) = pState[0];
	(*pu8YKState) = pState[1];
}
u32 GetRealTimeAMRReqAddr(u32 PORTn)
{
	u32 RT_AMRAddr = 0;
	switch((u8)PORTn)
	{
		case RS232PORT:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;		
		case GPRSPORT:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;		
		case RS485_1PORT:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;
		case RS485_2PORT:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;
		case RS485_3PORT:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;		
		case PLC_PORT:
			RT_AMRAddr = 0;
			break;
		case IRPORT:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;
		case ENETPORT:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;
		case GUI_PORT:
			RT_AMRAddr = 0;
			break;
		default:
			RT_AMRAddr = QCSG_RT_RDMETER_REQ;
			break;
	}

	return RT_AMRAddr;
}
/**--------------------------------------------------------------------
*��    ��:������ա������ڴ��е�ͳ������
*�������:��
*�������:pu8YXState,pu8YKState
*��    ��:��
--------------------------------------------------------------------**/
void QCSG_ClearStatisticsData(void)
{
	MemSet((u8*)QCSG_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_A_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_B_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_C_VOLTAGE_PASSRATE_DAY,0,1024);
	MemSet((u8*)QCSG_CURRENT_EXTREME_DAY,0,1024);
	MemSet((u8*)QCSG_N_CURRENT_EXTREME_DAY,0,512);
	MemSet((u8*)QCSG_ACTIVE_POWER_DAY,0,512);
	MemSet((u8*)QCSG_REACTIVE_POWER_DAY,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_DAY,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_DAY,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_A_DAY,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_B_DAY,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_C_DAY,0,1024);
	MemSet((u8*)QCSG_POWER_DOWN_NUM_DAY,0,4);
	MemSet((u8*)QCSG_POWER_DOWN_TIME_DAY,0,4);
	
	MemSet((u8*)QCSG_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_A_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_B_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_C_VOLTAGE_PASSRATE_MONTH,0,1024);
	MemSet((u8*)QCSG_CURRENT_EXTREME_MONTH,0,1024);
	MemSet((u8*)QCSG_N_CURRENT_EXTREME_MONTH,0,512);
	MemSet((u8*)QCSG_ACTIVE_POWER_MONTH,0,512);
	MemSet((u8*)QCSG_REACTIVE_POWER_MONTH,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_MONTH,0,512);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_MONTH,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_A_MONTH,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_B_MONTH,0,1024);
	MemSet((u8*)QCSG_POWER_FACTOR_ZONE_C_MONTH,0,1024);
	MemSet((u8*)QCSG_POWER_DOWN_NUM_MONTH,0,4);
	MemSet((u8*)QCSG_POWER_DOWN_TIME_MONTH,0,4);

	//����ն��ᡢ�¶����¼ȫ�ֱ���
	MemSet((u8*)QCSG_RECORD_FROZEN_PN_LIST,0,256);
}

/*******************************************************************************************************
���º���Ϊ��������
******************************************************************************************?************/

AMR_ENUM GetAMREnumByDI(u32 DI)
{
	AMR_ENUM AmrEnum = AMR_UNKNOWN;
	
	
	if(DI <= 0x01460001)
	{
		if((DI & 0x00000001) != 0)

		{
			AmrEnum = AMR_MonthFreeze;
		}
		else
		{
			AmrEnum = AMR_CurveFreeze;
		}	
	}
	else if((DI >= 0x02010100) && (DI <= 0x028001FF))
	{
		AmrEnum = AMR_CurveFreeze;	
	}
	else if((DI >= 0x05060000) && (DI <= 0x05060CFF))
	{
		AmrEnum = AMR_DailyFreeze;
	}
	else if((DI >= 0xE1008010) && (DI <= 0xE1009C20))
	{
		AmrEnum = AMR_DailyStatisticFreeze;
	}
	else if((DI >= 0xE100C010) && (DI <= 0xE100DBFF))
	{
		AmrEnum = AMR_MonthStatisticFreeze;
	}
	else if((DI >= 0xE0000301) && (DI <= 0xE00003FE))
	{
		AmrEnum = AMR_NormalTask;
	}
	else if((DI >= 0xE0000401) && (DI <= 0xE00004FE))
	{
		AmrEnum = AMR_RelayTask;
	}
#if ((Project/100)==5) //err20181122 II�ͼ����� ������������ �ź�ǿ�� 
	else if(DI == 0xE1800015)
	{
		AmrEnum = AMR_CurveFreeze;	
	}
#endif
	else
	{
		AmrEnum = AMR_UNKNOWN;
	}
	
	return AmrEnum;
}


ResultEnum CheckDataDensity(AMR_ENUM AmrEnum, u32 DataDensity, u32 SampleUnit, u32 SamplePeriod)
{
	ResultEnum Result = NO_ERROR;
	u32 DataDensityTime = 0, SamplePeriodTime = 0;
	if(DataDensity == 0) return Result;

	switch((u8)DataDensity)
	{
		case QCSG_DATA_DENSITY_1_MINUTE:
			DataDensityTime = 1;
			break;
		case QCSG_DATA_DENSITY_5_MINUTE:
			DataDensityTime = 5;
			break;
		case QCSG_DATA_DENSITY_15_MINUTE:
			DataDensityTime = 15;
			break;
		case QCSG_DATA_DENSITY_30_MINUTE:
			DataDensityTime = 30;
			break;
		case QCSG_DATA_DENSITY_1_HOUR:
			DataDensityTime = 60;
			break;
		case QCSG_DATA_DENSITY_1_DAY:
			DataDensityTime = 24*60;
			break;
		case QCSG_DATA_DENSITY_1_MONTH:
			DataDensityTime = 31*24*60;
			break;
		default:
			DataDensityTime = 0;
			break;
	}

	switch((u8)SampleUnit)
	{
		case QCSG_SAMPLE_UNIT_MINUTE:
			SamplePeriodTime = SamplePeriod;
			break;
		case QCSG_SAMPLE_UNIT_HOUR:
			SamplePeriodTime = SamplePeriod * 60;
			break;
		case QCSG_SAMPLE_UNIT_DAY:
			SamplePeriodTime = SamplePeriod * 24 * 60;
			break;
		case QCSG_SAMPLE_UNIT_MONTH:
			SamplePeriodTime = SamplePeriod * 31 * 24 * 60;
			break;
		default:
			SamplePeriodTime = 0xFFFFFFFF;
			break;
	}
	
	switch(AmrEnum)
	{
		case AMR_DailyFreeze:		
		case AMR_DailyStatisticFreeze:
			if(DataDensity  != QCSG_DATA_DENSITY_1_DAY)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			break;
		case AMR_MonthFreeze:
		case AMR_MonthStatisticFreeze:
			if(DataDensity != QCSG_DATA_DENSITY_1_MONTH)
			{
				Result = ERROR_WRONG_ACCESS;
			}
			break;
		case AMR_CurveFreeze:
			if((DataDensity > QCSG_DATA_DENSITY_1_HOUR) || (DataDensity < QCSG_DATA_DENSITY_5_MINUTE))
			{
				Result = ERROR_WRONG_ACCESS;
			}
			break;
		case AMR_NormalTask:
		case AMR_RelayTask:
			if(DataDensityTime < SamplePeriodTime) 
			{
				Result = ERROR_WRONG_ACCESS;
			}
			break;
		case AMR_UNKNOWN:
		default:
			Result = ERROR_WRONG_ACCESS;
			break;
	}
	return Result;
}

u8 GetDefaultDataDensityByAMREnum(AMR_ENUM AmrEnum)
{
	u8 DataDensity = 0;
	switch(AmrEnum)
	{
		case AMR_DailyFreeze:		
		case AMR_DailyStatisticFreeze:
			DataDensity = QCSG_DATA_DENSITY_1_DAY;
			break;
		case AMR_MonthFreeze:
		case AMR_MonthStatisticFreeze:
			DataDensity = QCSG_DATA_DENSITY_1_MONTH;
			break;
		case AMR_CurveFreeze:
			#if ((Project/100)==2)
				DataDensity = QCSG_DATA_DENSITY_1_HOUR;
			#endif
			#if ((Project/100)==3)
				DataDensity = QCSG_DATA_DENSITY_15_MINUTE;
			#endif
			#if ((Project/100)==5)
				DataDensity = QCSG_DATA_DENSITY_1_HOUR;
			#endif
			#if ((Project/100)==9)
				DataDensity = QCSG_DATA_DENSITY_1_HOUR;
			#endif
			break;
		case AMR_NormalTask:		
		case AMR_RelayTask:
			DataDensity = QCSG_DATA_DENSITY_1_DAY;
			break;
		case AMR_UNKNOWN:
		default:
			DataDensity = QCSG_DATA_DENSITY_1_DAY;
			break;
	}
	return DataDensity;
}

/****************************************************************************
��     ��: �������������ʱ���ܶ�ȡֵ��������Ӧ��ʱ��
��ڲ� ��: Time,��Ҫ���ӵ�ʱ�䣬
		   DataDensity�������ܶȣ��������ܶȲ��Ϸ�Ĭ��Ϊ6����������
��     ��: 
��     д: zhaoyun
��дʱ ��; 2017-7-10
****************************************************************************/
ResultEnum AddTimeByDataDensity(RealTimeYYStruct* Time, u32 DataDensity)
{
	ResultEnum Result = NO_ERROR;
	RealTimeStruct RealTime;
	u32 i = 0;
	if((DataDensity == QCSG_DATA_DENSITY_DEFAULT) || (DataDensity > QCSG_DATA_DENSITY_1_MONTH))
	{
		DataDensity = QCSG_DATA_DENSITY_1_DAY;
	}
	YYTimeToRealTime(Time, &RealTime);
	switch ((u8)DataDensity)
	{
		case QCSG_DATA_DENSITY_1_MINUTE:
			QCSG_IncOneMinute(&RealTime);
			break;
		case QCSG_DATA_DENSITY_5_MINUTE:
			for(i = 0; i < 5; i++)
			{
				QCSG_IncOneMinute(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_15_MINUTE:
			for(i = 0; i < 15; i++)
			{
				QCSG_IncOneMinute(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_30_MINUTE:
			for(i = 0; i < 30; i++)
			{
				QCSG_IncOneMinute(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_1_HOUR:
			QCSG_IncOneHour(&RealTime);
			break;
		case QCSG_DATA_DENSITY_1_DAY:
			QCSG_IncOneDay(&RealTime);
			break;
		case QCSG_DATA_DENSITY_1_MONTH:
			QCSG_IncOneMonth(&RealTime);
			break;
		default:
			Result = ERROR_InvalidData;
			break;
		
	}
	RealTimeToYYTime(&RealTime, Time);
	return Result;
}


ResultEnum DecTimeByDataDensity(RealTimeYYStruct* Time, u32 DataDensity)
{
	ResultEnum Result = NO_ERROR;
	RealTimeStruct RealTime;
	u32 i = 0;
	if((DataDensity == QCSG_DATA_DENSITY_DEFAULT) || (DataDensity > QCSG_DATA_DENSITY_1_MONTH))
	{
		DataDensity = QCSG_DATA_DENSITY_1_DAY;
	}
	YYTimeToRealTime(Time, &RealTime);
	switch ((u8)DataDensity)
	{
		case QCSG_DATA_DENSITY_1_MINUTE:
			QCSG_DecOneMinute(&RealTime);
			break;
		case QCSG_DATA_DENSITY_5_MINUTE:
			for(i = 0; i < 5; i++)
			{
				QCSG_DecOneMinute(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_15_MINUTE:
			for(i = 0; i < 15; i++)
			{
				QCSG_DecOneMinute(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_30_MINUTE:
			for(i = 0; i < 30; i++)
			{
				QCSG_DecOneMinute(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_1_HOUR:
			QCSG_DecOneHour(&RealTime);
			break;
		case QCSG_DATA_DENSITY_1_DAY:
			QCSG_DecOneDay(&RealTime);
			break;
		case QCSG_DATA_DENSITY_1_MONTH:
			QCSG_DecOneMonth(&RealTime);
			break;
		default:
			Result = ERROR_InvalidData;
			break;
		
	}
	RealTimeToYYTime(&RealTime, Time);
	return Result;
}

ResultEnum ConvertTimeToDataDensityTime(RealTimeYYStruct* Time, u32 DataDensity)
{
	ResultEnum Result = NO_ERROR;
	RealTimeStruct RealTime;
	if((DataDensity == QCSG_DATA_DENSITY_DEFAULT) || (DataDensity > QCSG_DATA_DENSITY_1_MONTH))
	{
		DataDensity = QCSG_DATA_DENSITY_1_DAY;
	}
	YYTimeToRealTime(Time, &RealTime);
	switch ((u8)DataDensity)
	{
		case QCSG_DATA_DENSITY_1_MINUTE:
			break;
		case QCSG_DATA_DENSITY_5_MINUTE:
			if(RealTime.u8Minute == 0x00)
			{
				RealTime.u8Minute = 0x00;
			}
			else if(RealTime.u8Minute <= 0x05)
			{
				RealTime.u8Minute = 0x05;
			}
			else if(RealTime.u8Minute <= 0x10)
			{
				RealTime.u8Minute = 0x10;
			}
			else if(RealTime.u8Minute <= 0x15)
			{
				RealTime.u8Minute = 0x15;
			}
			else if(RealTime.u8Minute <= 0x20)
			{
				RealTime.u8Minute = 0x20;
			}
			else if(RealTime.u8Minute <= 0x25)
			{
				RealTime.u8Minute = 0x25;
			}
			else if(RealTime.u8Minute <= 0x30)
			{
				RealTime.u8Minute = 0x30;
			}
			else if(RealTime.u8Minute <= 0x35)
			{
				RealTime.u8Minute = 0x35;
			}
			else if(RealTime.u8Minute <= 0x40)
			{
				RealTime.u8Minute = 0x40;
			}
			else if(RealTime.u8Minute <= 0x45)
			{
				RealTime.u8Minute = 0x45;
			}
			else if(RealTime.u8Minute <= 0x50)
			{
				RealTime.u8Minute = 0x50;
			}
			else if(RealTime.u8Minute <= 0x55)
			{
				RealTime.u8Minute = 0x55;
			}
			else
			{
				RealTime.u8Minute = 0x00;
				QCSG_IncOneHour(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_15_MINUTE:
			if(RealTime.u8Minute == 0x00)
			{
				RealTime.u8Minute = 0x00;
			}
			else if(RealTime.u8Minute <= 0x15)
			{
				RealTime.u8Minute = 0x15;
			}
			else if(RealTime.u8Minute <= 0x30)
			{
				RealTime.u8Minute = 0x30;
			}
			else if(RealTime.u8Minute <= 0x45)
			{
				RealTime.u8Minute = 0x45;
			}
			else
			{
				RealTime.u8Minute = 0x00;
				QCSG_IncOneHour(&RealTime);
			}
			break;
		case QCSG_DATA_DENSITY_30_MINUTE:
			if(RealTime.u8Minute == 0x00)
			{
				RealTime.u8Minute = 0x00;
			}
			else if(RealTime.u8Minute <= 0x30)
			{
				RealTime.u8Minute = 0x30;
			}
			else
			{
				RealTime.u8Minute = 0x00;
				QCSG_IncOneHour(&RealTime);
			}	
			break;
		case QCSG_DATA_DENSITY_1_HOUR:
			if(RealTime.u8Minute != 0x00)
			{
				QCSG_IncOneHour(&RealTime);
				RealTime.u8Minute = 0x00;
			}
			break;
		case QCSG_DATA_DENSITY_1_DAY:		
			if((RealTime.u8Hour != 0x00) || (RealTime.u8Minute != 0x00))
			{
				QCSG_IncOneDay(&RealTime);
				RealTime.u8Hour = 0x00;
				RealTime.u8Minute = 0x00;
			}	
			break;
		case QCSG_DATA_DENSITY_1_MONTH:	
			if((RealTime.u8Day != 0x01) || (RealTime.u8Hour != 0x00) || (RealTime.u8Minute != 0x00))
			{
				QCSG_IncOneMonth(&RealTime);
				RealTime.u8Day = 0x01;
				RealTime.u8Hour = 0x00;
				RealTime.u8Minute = 0x00;
			}	
			break;
		default:
			Result = ERROR_InvalidData;
			break;
		
	}
	RealTimeToYYTime(&RealTime, Time);
	return Result;
}
/****************************************************************************
��     ��: �������������ʱ��Ƚ�����ʱ��Ĵ�С
��ڲ� ��: AmrEnum����ͬ�ĳ������ͣ��Ƚϵ�ʱ�䷶Χ��һ��
		   Time1,Time2: �Ƚ�Time1��Time2С����������
��     ��: 
��     д: zhaoyun
��дʱ ��; 2017-7-10
****************************************************************************/
ResultEnum TimeCompare(AMR_ENUM AmrEnum, RealTimeYYStruct* Time1, RealTimeYYStruct* Time2)
{
	ResultEnum Result = ERROR_TIME_IS_EQUAL;
	u32 i = 0, CompareNum = 0;
	u8* SrcTime = (u8*)Time1;
	u8* DstTime = (u8*)Time2;
	switch(AmrEnum)
    {
        case AMR_DailyFreeze:
        case AMR_DailyStatisticFreeze:
			CompareNum = 4;
			break;
        case AMR_CurveFreeze:
			CompareNum = 6;
            break;
        case AMR_MonthFreeze:
        case AMR_MonthStatisticFreeze:
			CompareNum = 3;
             break;
        case AMR_NormalTask:
        case AMR_RelayTask:
		case AMR_ALARM:
		case AMR_EVENT:
			CompareNum = 6;
		break;
	default:
		CompareNum = 6;
		break;
    }
	for(i = 0; i < CompareNum; i++)
    {
        if(SrcTime[i]>DstTime[i])
        {
            Result = ERROR_TIME_IS_BIGGER;
            break;
        }
        else if(SrcTime[i]<DstTime[i])
        {
            Result = ERROR_TIME_IS_SMALLER;
            break;
        }
        else
        {
            Result = ERROR_TIME_IS_EQUAL;
        }
    }	 
	return Result;
}

void RealTimeToYYTime(RealTimeStruct *srcTime,RealTimeYYStruct *dstTime)
{
    dstTime->YearH = 0x20;
    dstTime->YearL = srcTime->u8Year;
    dstTime->Month = srcTime->u8Month;
    dstTime->Day = srcTime->u8Day;
    dstTime->Hour = srcTime->u8Hour;
    dstTime->Minute = srcTime->u8Minute;
}
void YYTimeToRealTime(RealTimeYYStruct *srcTime,RealTimeStruct *dstTime)
{
    dstTime->u8Year = srcTime->YearL;
    dstTime->u8Month = srcTime->Month;
    dstTime->u8Day = srcTime->Day;
    dstTime->u8Hour = srcTime->Hour;
    dstTime->u8Minute = srcTime->Minute;
    dstTime->u8Second = 0;
	dstTime->u8Week = 0;
}

u16 GetMaxPnNum(void)
{
	return MAX_PN_NUM;
}


u16 SplitPnToList(u16 Pn, u16* List, u8 PnCheck)
{
	u16 i = 0, Num = 0, PnIncEn = 0, TempPn = 0, MaxPn = 0;

	if(List == NULL) return Num;
	if(((Pn & 0xFF00) == 0) && ((Pn & 0x00FF) != 0)) return Num;
	if(Pn == 0)
	{
		List[Num] = Pn;
		Num++;
	}
	else
	{
		if(Pn == QCSG_ALL_PN)
		{
			Pn = 0x01FF;
			PnIncEn = 1;
		}
		// �ȴ���ɻ�ȡ��ǰ��������ţ����� MAX_PN_NUM ����
		MaxPn = SwitchNumToPn(MAX_PN_NUM);
		
		for(i = 0; i < 8; i++)
		{
			if((Pn & (1 << i)) != 0) 
			{
				TempPn = Pn & 0xFF00;
				TempPn |= (1 << i);
				if((PnCheck != PN_CHECK_DISABLE) && ((TempPn > MaxPn) || (CheckPnValid(TempPn) != NO_ERROR)))
				{
					continue;
				}
				List[Num] = TempPn;
				Num++;
			}
			if((i >= 8) && (PnIncEn != 0))
			{
				i = 0;
				Pn += 0x0100;
				if((Pn & 0xFF00) == 0xFF00) break;
			}
		}
	}
	
	return Num;
}

u16 SetAlarmEventDIDataByDIInfo(u8* DstData, u8* SrcData, const DIDataInfoStruct* DiInfo)
{
	u16 i = 0, Len = 0, DescNum = 0;

	for(DescNum = 0; DescNum < DiInfo->DescNum; DescNum++)
	{
		//MR((u32)&DstData[Len], (u32)&SrcData[DiInfo->Descriptor[DescNum].OffSet], DiInfo->Descriptor[DescNum].Len);
		//Len += DiInfo->Descriptor[DescNum].Len;
		for(i = 0; i < DiInfo->Descriptor[DescNum].Len; i++)
		{
			DstData[Len++] = SrcData[DiInfo->Descriptor[DescNum].OffSet + i];
		}
	}	
	return Len;
}

/****************************************************************************
��     ��: ��źϷ����ж�
��ڲ� ��: MeterAddr=���
���ڲ� ��: ��
��     ��: �ϣ����򲻺Ϸ�
����ע ��: 
��Ҫ�� ��:
��     д: zhaoyun
��дʱ ��; 2015-11-10
****************************************************************************/
ResultEnum MeterAddrCheck(u8* MeterAddr)
{
	u32 i = 0;
	for(i = 0; i < METER_ADDR_SIZE; i++)
	{
		if(((MeterAddr[i] & 0xF0) > 0x90) || ((MeterAddr[i] & 0x0F) > 0x09))
		{
			return ERROR_WRONG_ADDR;
		}
	}
	return NO_ERROR;
}

//Pn = 0101 -> PnNum = 1
u32 SwitchPnToNum(u32 Pn)
{
	u32 PnNum = 0, i = 0;
	if(Pn == 0) return PnNum;

	PnNum = ((Pn >> 8) - 1) * 8;
	for(i = 0; i < 8; i++)
	{
		if((Pn & (1 << i)) != 0) break;
	}
	PnNum += (i + 1);
	return PnNum;
}


u32 SwitchNumToPn(u32 PnNum)
{
	u32 Pn = 0;
	if(PnNum == 0) return Pn;
	if(PnNum <= MAX_PN_NUM)
	{
		Pn = ((PnNum- 1) >> 3) + 1;
		Pn <<= 8;
		Pn += 1 << ((PnNum - 1) % 8); 
	}
	return Pn;
}

ResultEnum QCSG_GetLastUnUsedPnByAddr(u32 *Pn, u8* Addr)
{
	MeasurePointStruct MeasurePoint;
	u32 i = 0;
	u32 TmpPn = 0;
	
	for(i = 0; i < MAX_PN_NUM; i++)
	{
		NAND_ReadMeasurePointByPnNum(i, &MeasurePoint);
		if(MemCompare(MeasurePoint.Addr, Addr, METER_ADDR_SIZE) == 0) break;
		
		if(MeasurePoint.State == 0x01)
		{
			TmpPn = i; //�ҵ����һ����Ч������
		}
	}

	if(i < MAX_PN_NUM) return ERROR_InvalidData;

	TmpPn++;
	(*Pn) =  SwitchNumToPn(TmpPn);

	return NO_ERROR;
}

ResultEnum QCSG_CheckPnAddrIsDuplicate(u32 Pn, u8* Addr)
{
	MeasurePointStruct MeasurePoint;
	u32 i = 0, PnNum = 0;

	PnNum = SwitchPnToNum(Pn);
	for(i = 0; i < MAX_PN_NUM; i++)
	{
		if(i == PnNum) continue;
		NAND_ReadMeasurePointByPnNum(i, &MeasurePoint);
		if(MemCompare(MeasurePoint.Addr, Addr, METER_ADDR_SIZE) == 0) break;
	}

	if(i < MAX_PN_NUM) return ERROR_InvalidData;

	return NO_ERROR;
}

ResultEnum CheckPnValid(u16 Pn)
{	
	MeasurePointStruct MeasurePoint;
	ResultEnum Result = NO_ERROR;
	u32 DataLen = 0;
	u16 TempPn = 0;

	if(Pn == 0) return NO_ERROR;
	
	TempPn = SwitchNumToPn(MAX_PN_NUM);
	if(Pn > TempPn) return ERROR_VerifyFail;
		
	MC(0, (u32)&MeasurePoint, sizeof(MeasurePointStruct));
	NAND_ReadMeasurePoint(Pn, 0xE0800000, (u8*)&MeasurePoint, &DataLen);

	//if(MeasurePoint.State == 0) return ERROR_VerifyFail;

	Result = MeterAddrCheck(MeasurePoint.Addr);

	return Result;
}
/*
ResultEnum CheckPnValid(u16 Pn)
{	
	//MeasurePointStruct MeasurePoint;
	NodeListStruct* NodeList = (NodeListStruct*)ADDR_NodeList;
	ResultEnum Result = NO_ERROR;
	u16 TempPn = 0;

	if(Pn == 0) return NO_ERROR;
	
	TempPn = SwitchNumToPn(MAX_PN_NUM);
	if(Pn > TempPn) return ERROR_VerifyFail;
	
	TempPn = SwitchPnToNum(Pn);
	if(NodeList[TempPn].State == 0x01)
	{
		Result = MeterAddrCheck(NodeList[TempPn].Addr);
	}
	eles
	{
		Result = ERROR_VerifyFail;
	}

	return Result;
}
*/
u8 CaculateWeek(RealTimeStruct* Time)
{
	u8 Week = 0;
	u16 w = 0, d = 0;
	u16 YearInDec = 2000 + (Time->u8Year & 0x0F) + (Time->u8Year >> 4) * 10;	
	u8 MonthInDec = (Time->u8Month & 0x0F) + (Time->u8Month >> 4) * 10;
	u8 DayInDec = (Time->u8Day & 0x0F) + (Time->u8Day >> 4) * 10;
		
	switch(MonthInDec)
	{
		case 1:
			d = DayInDec;
			break;
		case 2:
			d = 31 + DayInDec;
			break;
		case 3:
			d = 31 + 28 + DayInDec;		
			break;
		case 4:
			d = 31 + 28 + 31 + DayInDec;
			break;
		case 5:
			d = 31 + 28 + 31 + 30 + DayInDec;
			break;
		case 6:
			d = 31 + 28 + 31 + 30 + 31 + DayInDec;
			break;
		case 7:
			d = 31 + 28 + 31 + 30 + 31 + 30 + DayInDec;
			break;
		case 8:
			d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + DayInDec;
			break;
		case 9:
			d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + DayInDec;
			break;
		case 10:
			d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + DayInDec;
			break;
		case 11:
			d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + DayInDec;
			break;
		case 12:
			d = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + DayInDec;
			break;
		default:
			break;
	}
	if((LeapYearCheck(YearInDec) != 0) && (MonthInDec > 0x02)) d++;
	w = (YearInDec -1) + ((YearInDec -1) / 4)	-((YearInDec -1) / 100) + ((YearInDec -1) / 400) + d;
	Week = w % 7;
	return Week;
}

u8 LeapYearCheck(u16 sYear)
{
	return(((sYear % 4 == 0) && (sYear % 100 != 0)) || (sYear % 400 == 0));
}

ResultEnum TimeValidCheck(RealTimeStruct* Calendar)
{
	// ���ºϷ����ж�
	if(((Calendar->u8Year & 0x0F) > 0x09) || ((Calendar->u8Year & 0xF0) > 0x90))	//(0x00 - 0x99)
	{
		return ERROR_InvalidData;
	}

	if((Calendar->u8Month == 0x00) || ((Calendar->u8Month & 0x0F) > 0x09) || ((Calendar->u8Month & 0xF0) > 0x10) || (Calendar->u8Month > 0x12))	//(0x01 - 0x12)
	{
		return ERROR_InvalidData;
	}

	// �պϷ����ж�
	if((Calendar->u8Day == 0x00) || ((Calendar->u8Day & 0x0F) > 0x09) || ((Calendar->u8Day & 0xF0) > 0x30))		//(0x01 - 0x31)
	{
		return ERROR_InvalidData;
	}

	if((Calendar->u8Month == 0x04) || (Calendar->u8Month == 0x06) || (Calendar->u8Month == 0x09) || (Calendar->u8Month == 0x11))
	{
		if(Calendar->u8Day > 0x30) return ERROR_InvalidData;		
	}
	else if(Calendar->u8Month == 2)
	{
		if(LeapYearCheck(2000 + (Calendar->u8Year & 0x0F) + ((Calendar->u8Year >> 4) * 10)) != 0)			//�����ж�
		{
			if(Calendar->u8Day > 0x29) return ERROR_InvalidData;
		}
		else
		{
			if(Calendar->u8Day > 0x28) return ERROR_InvalidData;
		}
	}
	else
	{
		if(Calendar->u8Day > 0x31) return ERROR_InvalidData;	
	}

	// Сʱ�����ӡ���Ϸ����ж�

	if(((Calendar->u8Hour & 0x0F) > 0x09) || ((Calendar->u8Hour & 0xF0) > 0x20) || (Calendar->u8Hour > 0x23))	//(0x00 - 0x23)
	{
		return ERROR_InvalidData;
	}

	if(((Calendar->u8Minute & 0x0F) > 0x09) || ((Calendar->u8Minute & 0xF0) > 0x50) || (Calendar->u8Minute > 0x59))	//(0x00 - 0x59)
	{
		return ERROR_InvalidData;
	}

	if(((Calendar->u8Second & 0x0F) > 0x09) || ((Calendar->u8Second & 0xF0) > 0x50) || (Calendar->u8Second > 0x59))	//(0x00 - 0x59)
	{
		return ERROR_InvalidData;
	}
	
	// ���ںϷ����ж�
	if(Calendar->u8Week > 0x06) return ERROR_InvalidData;

	return NO_ERROR;
}



u16 QCSG_BuildFrameByStruct(QCSGFrameStruct* Frame, u8* Buf)
{
	u16 Index = 0, CSStartPos = 0;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	
	Buf[Index++] = QCSG_FRAME_HEAD;
	Buf[Index++] = (u8)(Frame->DataLen + 10);
	Buf[Index++] = (u8)((Frame->DataLen + 10) >> 8);
	Buf[Index++] = (u8)(Frame->DataLen + 10);
	Buf[Index++] = (u8)((Frame->DataLen + 10) >> 8);
	Buf[Index++] = QCSG_FRAME_HEAD;
	CSStartPos = Index;
	if(pTSysCtrl->LatelyAlarmNum != 0)
	{
		Buf[Index++] = Frame->FC | QCSG_FC_ACD_BIT;
	}
	else
	{
		Buf[Index++] = Frame->FC;
	}
	//Buf[Index++] = Frame->FC;
	Buf[Index++] = Frame->A1;
	Buf[Index++] = Frame->A1 >> 8;
	Buf[Index++] = Frame->A1 >> 16;
	Buf[Index++] = Frame->A2;
	Buf[Index++] = Frame->A2 >> 8;
	Buf[Index++] = Frame->A2 >> 16;
	Buf[Index++] = Frame->A3;
	Buf[Index++] = Frame->AFN;
	Buf[Index++] = Frame->SEQ;
	MemCopy(&Buf[Index], Frame->Data, Frame->DataLen);
	Index += Frame->DataLen;
	Buf[Index++] = (u8)SumCaculation(&Buf[CSStartPos], Frame->DataLen + 10);
	Buf[Index++] = QCSG_FRAME_TAIL;
	return Index;
}

/****************************************************************************
��     ��: �����У��
��ڲ� ��: *Data,Len
���ڲ� ��: ��
��     ��: Result
����ע ��:
��Ҫ�� ��:
��     д: zhaoyun
��дʱ ��; 2014-4-10
****************************************************************************/
u32 SumCaculation(u8 *Data, u32 Len)
{
	u32	Result = 0;
	u32	i = 0;
	for(i = 0; i < Len; i ++)
	{
		Result += Data[i];
	}
	return Result;
}



/****************************************************************************
��     ��: EDMI�����֡У��
��ڲ� ��: *buffer,Len
���ڲ� ��: crc
��     ��: crc
����ע ��:
��Ҫ�� ��:
��     д: ybzyl
��дʱ ��; 2019-05-27
****************************************************************************/
u16  EDMI_Crc_Cala(u8* buffer,u32 len)
{
	u16 CRCConst= 0x1021;
	u16 crc = 0;
	s32 i = 0,j = 0;
	
	for (i = 0; i < len; i++)
	{
	    for(j = 7; j >= 0; j--)
	    {
	        if ((crc & 0x8000) != 0)
	            crc = (crc << 1) ^ CRCConst; 
	        else
	            crc = crc << 1;
	        if((buffer[i] &(1<<j))!=0)
		        crc= crc^CRCConst;
	    }
	}
	return crc;
}


//err20190531 IEEE754 ������ת���㷨
//32bit����������
/****************************************************************************
��     ��: EDMI���������ת���㷨32bit 
��ڲ� ��: u32 Float_Value, u32* Output_Value
���ڲ� ��: u32* Output_Value

Output_Value[0] С����ǰ��ֵ
Output_Value[1]	С�������ֵ

��     ��: u8sign_bit(��ֵ���� 0�� 1��)
****************************************************************************/
u8 IEEE754_Float_Arithmetic(u32 Float_Value,u32* Output_Value)
{
	//u32 u32Value = 0x4260fbed;
	u8 u8Sigh_bit;//���� 0�� 1��
	u8 u8Exponent;//ָ��������λ��
	u32 u32Value_Temp;
	u32 u32Value1;//���С����ǰ����
	u32 u32Value2;//���С����󲿷�
	
	u8Sigh_bit = (u8)(Float_Value >> 31);
	
	Float_Value &= 0x7FFFFFFF;//ȥ������λ
	
	u8Exponent = (u8)(Float_Value >> 23);
	u32Value_Temp = Float_Value & (0x7FFFFF);

	if(u8Exponent>= 127)
	{
		u8Exponent = u8Exponent - 127;	//����λ��
		u32Value1 = (u32Value_Temp >> (23 - u8Exponent)) + (1 << u8Exponent);
		u32Value2 = u32Value_Temp & (0x7FFFFF >> u8Exponent);
		u32Value2 = (u32Value2*100)>>(23-u8Exponent);//����С����2λ
	}
	else 
	{
		u8Exponent = 127- u8Exponent ;	//����λ��
		u32Value1 = (u32Value_Temp >> (23 + u8Exponent)) + (1 >> u8Exponent);
		u32Value2 = u32Value_Temp | (1 << 23);
		u32Value2 = (u32Value2*100)>>(23+u8Exponent);//����С����2λ
	}

	//u32Value1 = (u32Value_Temp >> (23 - u8Exponent)) + (1 << u8Exponent);
	//u32Value2 = u32Value_Temp & (0x7FFFFF >> u8Exponent);
	//u32Value2 = (u32Value2*100)>>(23-u8Exponent);//����С����2λ
	
	Output_Value[0] = u32Value1;
	Output_Value[1] = u32Value2;

	return u8Sigh_bit;
}

/****************************************************************************
��     ��: EDMI���������ת���㷨64bit 
��ڲ� ��: u64 Float_Value, u64* Output_Value
���ڲ� ��: u64* Output_Value

Output_Value[0] С����ǰ��ֵ
Output_Value[1]	С�������ֵ

��     ��: u8sign_bit(��ֵ���� 0�� 1��)
****************************************************************************/
u8 IEEE754_Double_Arithmetic(u64 Float_Value,u64* Output_Value)
{
	//u32 u32Value = 0x4260fbed;
	u8 u8Sigh_bit;//���� 0�� 1��
	u32 u32Exponent;//ָ��������λ��
	u64 u64Value_Temp;
	u64 u64Value1;//���С����ǰ����
	u64 u64Value2;//���С����󲿷�
	
	u8Sigh_bit = (u8)(Float_Value >> 63);
	
	Float_Value &= 0x7FFFFFFFFFFFFFFF;//ȥ������λ
	
	u32Exponent = (u32)(Float_Value >> 52);
	u64Value_Temp = Float_Value & (0xFFFFFFFFFFFFF);

	if(u32Exponent>= 1023)
	{
		u32Exponent = u32Exponent - 1023;	//����λ��
		u64Value1 = (u64Value_Temp >> (52 - u32Exponent)) + (1 << u32Exponent);
		u64Value2 = u64Value_Temp & (0xFFFFFFFFFFFFF >> u32Exponent);
		u64Value2 = (u64Value2*100)>>(52-u32Exponent);//����С����2λ
	}
	else 
	{
		u32Exponent = 1023 - u32Exponent;	//����λ����
		u64Value1 = (u64Value_Temp >> (52 + u32Exponent)) + (1 >> u32Exponent);
		u64Value2 = u64Value_Temp | ((u64)1 << 52);//& (0xFFFFFFFFFFFFF << u32Exponent);
		u64Value2 = (u64Value2*100)>>(52+u32Exponent);//����С����2λ
	}

	//u64Value1 = (u64Value_Temp >> (52 - u32Exponent)) + (1 << u32Exponent);
	//u64Value2 = u64Value_Temp & (0xFFFFFFFFFFFFF >> u32Exponent);
	
	Output_Value[0] = u64Value1;
	Output_Value[1] = u64Value2;

	return u8Sigh_bit;
}


//err20190531 ��EDMI�����֡��ʽ����Ԥ����
//ɨ������0x10 ����һ������ȥ0x40��ֵ���롣

/****************************************************************************
��     ��: EDMI�����֡������Ŵ��� 
��ڲ� ��: u8* pu8InData, u8* pu8DataLen
���ڲ� ��: u8* pu8InData, u8* pu8DataLen

pu8InData:���յ���EDMI֡
pu8DataLenn:���յ���֡����
//����ִ���� pu8InData�ͱ��ת���������,pu8DataLenΪת���󳤶�
��     ��: �ɹ���ʧ��
****************************************************************************/
u8 QCSG_GetEDMIRealFrame(u8* pu8InData,u8* pu8DataLen)
{
	u8 i=0;
	u8 k=0;
	u8 Temp_num = 0;//�������
	u8 Len = (*pu8DataLen);//���ĳ���
	for(i=0;i<Len;i++)
	{
		if(pu8InData[i] == 0x10)
		{
			Temp_num ++;
			pu8InData[i] = pu8InData[i+1] - 0x40;
			for(k=i+1;k<Len-Temp_num;k++)
			{
				pu8InData[k] = pu8InData[k+1];
			}
			pu8InData[k] = 0x00;
		}
	}
	 (*pu8DataLen) -= Temp_num;
	return NO_ERROR;

}




void MemCopy(void *dest, const void *source, u16 bytes)
{
	u32 ADDR_Dest = (u32)dest;

	if(ADDR_Dest>=(ADDR_EFLASH_Start))
	{
		if(ADDR_Dest<(ADDR_Download_ENTRY))
		{
			while(1)
			{
				DisplayStringAutoUp(0,9,(u8*)"\x0""MemCopy_ERROR");
				WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
			}

		}
	}
	
  	memcpy(dest, source, bytes);
}

s8 MemCompare(const void *source0, const void *source1, u16 bytes)
{
  	return memcmp(source0, source1, bytes);
}

void MemSet(void *dest, u8 val, u16 bytes)
{
	u32 ADDR_Dest = (u32)dest;

	if(ADDR_Dest>=(ADDR_EFLASH_Start))
	{
		if(ADDR_Dest<(ADDR_Download_ENTRY))
		{
			while(1)
			{
				DisplayStringAutoUp(0,9,(u8*)"\x0""MemSet_ERROR");
				WWDT_Enable_Feed(WDTTimerOutVal);//�����Ź���ι��;���TimerOutMS=(0xfff*256)/32=32768ms
			}

		}
	}
  	memset(dest, val, bytes);
}

ResultEnum DLT645FrameCheck(u8* FrameBuf, u32 FrameLen)
{
	u32 DataLen = 0, i = 0;
	if(FrameLen < MIN_645_FRAME_LEN) return ERROR_WRONG_LENGTH;

	for(i = 0; i < FrameLen; i++)
	{
		if((FrameBuf[i] != DLT_645_HEAD) || (FrameBuf[i + 7] != DLT_645_HEAD)) continue;
		
		DataLen = FrameBuf[i + 9];
		if(DataLen > MAX_645_DATA_LEN) continue;
		if(FrameBuf[i + 11 + DataLen] != DLT_645_TAIL) continue;
				
		if((FrameBuf[i + 10 + DataLen] != (u8)SumCaculation(&FrameBuf[i], DataLen + 10))) continue;
		
		return NO_ERROR;
	}
	
	return ERROR_WRONG_PROTOCOL;
}


ResultEnum EDMI_FrameCheck(u8* FrameBuf, u32 FrameLen)
{
	u32 i = 0;
	if(FrameLen < MIN_EDMI_FRAME_LEN) return ERROR_WRONG_LENGTH;

	for(i = 0; i < FrameLen; i++)
	{
		if((FrameBuf[i] != EDMI_HEAD) || (FrameBuf[i + 1] != EDMI_FRAME_FLAG)) continue;
		
		//DataLen = FrameBuf[i + 9];
		//if(DataLen > MAX_645_DATA_LEN) continue;
		if(FrameBuf[i + FrameLen - 1] != EDMI_TAIL) continue;
				
		return NO_ERROR;
	}
	
	return ERROR_WRONG_PROTOCOL;
}


const u16 fcstab[256] =
{                                                     //CRC����
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

//G(X) = 1 + X5 + X12 + X16, CCIT-16
//#define CRC_POLY 			(0xF0B8)
#define CRC_POLY 			(0x1021)
#define GOOD_CRC			(0x0F47)

u16 CCIT16_CRC(u8 *ptr, u32 len)    
{ 
	u16 crc = 0xFFFF;  
	u8 i;  
	while(len--) 
	{ 
		crc ^= *ptr++;  
		for(i = 0; i < 8; i++)  
		{ 
			if(crc & 0x0001) 
			{ 
				crc >>= 1; 
				crc ^= CRC_POLY;   
			} 
			else   
			{ 
				crc >>= 1; 
			}  
		} 
	}   
	crc ^= 0xFFFF; 
	return crc; 
} 
//CRC16_XMODEM ��ʼֵ0x0000 ������ֵ0x0000 ���뷴תFalse �����תFalse
u16 CRC16_XMODEM(u8 *ptr, u32 len)  
{  
	u16 crc = 0x0000;  
    u8 i;
	
	while (len--)     
	{    
		crc ^= (*(ptr++) << 8);  
		for(i = 0;i < 8;i++)  
		{  
			if(crc & 0x8000)  
				crc = (crc << 1) ^ CRC_POLY;  
			else  
				crc = crc << 1;  
		}  
	}
	return crc;  
}  

// ��ʼֵ 0x0000 ������ֵ 0x0000 ʱ Ϊ CCITT ��ʱ���뷴תTrue  �����תTrue
// ��ʼֵ 0xFFFF ������ֵ 0xFFFF ʱ Ϊ X25   ��ʱ���뷴תTrue  �����תTrue
/*
u16 CCIT16_CRC(u8* ptr, u32 len)
{
	u16 crc = 0x0000; //0xffff;
	while(len--)
	{
		crc = (crc>>8) ^ fcstab[(crc ^ *(ptr++) ) & 0xFF];
	}
	crc ^= 0x0000; //0xffff;
	return crc;
}
*/


/*
//8λ�ַ� ��λ��ת
void InvertUint8(u8 *dstBuf,u8 *srcBuf)  
{  
    int i;  
    u8 tmp[4];  
    tmp[0] = 0;  
    for(i=0;i< 8;i++)  
    {  
      if(srcBuf[0]& (1 << i))  
        tmp[0]|=1<<(7-i);  
    }  
    dstBuf[0] = tmp[0];  
      
}
//16λ�ַ� ��λ��ת
void InvertUint16(u16 *dstBuf,u16 *srcBuf)  
{  
    int i;  
    u16 tmp[4];  
    tmp[0] = 0;  
    for(i=0;i< 16;i++)  
    {  
      if(srcBuf[0]& (1 << i))  
        tmp[0]|=1<<(15 - i);  
    }  
    dstBuf[0] = tmp[0];  
}  
*/



ResultEnum BCD_Check(u8* BCD, u32 Len)
{
	u32 i = 0;
	for(i = 0; i < Len; i++)
	{
		if(((BCD[i] & 0x0F) > 0x09) || ((BCD[i] & 0xF0) > 0x90))
		{
			return ERROR_VerifyFail;
		}
	}
	return NO_ERROR;
}

/****************************************************************************
��     ��: ��������Ĵ��ڿ��Ʋ����õ����ڳ�ʼ��������
��ڲ� ��: BaudRateCode: ���ݲ��������飬�����±��ȡ��ע�����������������׼baudrate/300
		   ��Ҫ�ȸ���������ȡֵ���ʵ�ʲ����ʣ�Ȼ��ת�������ܸ�ʽ��Ӧ�Ŀ����֡�
		   0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,
		   10=115200,11=230400
		   ParityCode: 0����У�飻1��żУ�飻2����У�� 
		   DataBitCode: ����λ��5,6,7,8
		   StopBitCode: ֹͣλ��0��1λ��1��1.5λ�� 2��2λ
��     ��: ת���������ܴ��ڳ�ʼ��������
		   D8-D5λ��ʾ������,O��11���α�ʾ0=300,1=600,2=1200,3=2400,4=4800,
		   5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400
		   D4λ��ʾֹͣλλ��0/1�ֱ��ʾ1/2��ֹͣλ
		   D3λ��ʾ����У��λ0/1�ֱ��ʾ��/��
		   D2λ��ʾż����У��λ0/1�ֱ��ʾż����У��
		   D1-D0 ����λ��0��3�ֱ��ʾ5-8λ
��     д: zhaoyun
��дʱ ��; 2017-7-10
****************************************************************************/
u16 ConvertToBpsCtrl(u8 BaudRateCode, u8 ParityCode, u8 DataBitCode, u8 StopBitCode)
{
	u16 i = 0;
	u16 BspCtrl = 0;
	u32 BaudRate = 0; 
	BaudRate = BaudRateCode;
	BaudRate *= 300;
	for(i = 0; i < MAX_BAUD_NUM; i++)
	{
		if(BaudRate == UartBaudRate[i]) break;
	}
	if(i >= MAX_BAUD_NUM)
	{// δ�ҵ����ڵĲ����ʣ���Ĭ��Ϊ2400
		i = 3;			
	}
	BspCtrl |= (i << 5);

	if(StopBitCode == 2)
	{
		BspCtrl |= 0x10;
	}

	if(ParityCode == 0)
	{// ��У��
	
	}
	else if(ParityCode == 2)
	{// ��У�� 
		BspCtrl |= 0x0C;
	}
	else
	{// żУ��
		BspCtrl |= 0x08;
	}

	if(DataBitCode == 5)
	{
		BspCtrl |= 0x00;
	}
	else if(DataBitCode == 6)
	{
		BspCtrl |= 0x01;
	}
	else if(DataBitCode == 7)
	{
		BspCtrl |= 0x02;
	}
	else
	{
		BspCtrl |= 0x03;
	}
	return BspCtrl;
}

void DataResetAction(u32 ResetType, u32 PORTn)
{
	Comm_Ram_TypeDef* pCommRam = Comm_Ram;
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	CommunicationParaStruct CommPara;
	//u32 i = 0, MaxFileName = 0;
	ms_Type *ms;
	u32 i = 0;
	AlarmSaveParaStruct AlarmSavePara;//err20190216 �޸ĵ���洢��־�Ĵ洢λ��

	if((u8)ResetType == 0x00)//���п�д�����ָ���������
	{
		MC(0, (u32)&CommPara, sizeof(CommunicationParaStruct));
		MR((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
		ME(ADDR_TERMINAL_PARA_START, 128*1024);
		ME(ADDR_MP_START, 256*1024);
		Init_ALLPara();
		MW((u32)&CommPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));

		ms = Get_ms();
		/*MaxFileName = FILENAMEmax_FLASH;
		for(i = 0; i < MaxFileName; i++)
		{
			ms->msfile_Delete(i);
			checkFile(i);
			pCommRam->msFILEchange = 0;
		}*/
		ms->msfile_format();
		MC(0, ADDR_E1800001, 256);
		QCSG_ClearStatisticsData();
		pTSysCtrl->DayPowerDownCnt = 0;
		pTSysCtrl->DayPowerDownTime = 0;
		pTSysCtrl->MonthPowerDownCnt = 0;
		pTSysCtrl->MonthPowerDownTime = 0;
		pTSysCtrl->LastAlarmDI = 0;
		pTSysCtrl->LatelyAlarmNum = 0;	
		pTSysCtrl->GPRSRxTxBytes = 0;//err20180404-20 ͨ���������
		MC(0, ADDR_RS485_CH1_AMR_TaskCtrl, LEN_RS485_CH1_AMR_TaskCtrl * 4);

		
	}
	if((u8)ResetType == 0x01)//��������λ
	{
		ms = Get_ms();
		/*MaxFileName = FILENAMEmax_FLASH;
		for(i = 0; i < MaxFileName; i++)
		{
			ms->msfile_Delete(i);
			checkFile(i);
			pCommRam->msFILEchange = 0;
		}*/
		ms->msfile_format();
		MC(0, ADDR_E1800001, 256);
		QCSG_ClearStatisticsData();
		pTSysCtrl->DayPowerDownCnt = 0;
		pTSysCtrl->DayPowerDownTime = 0;
		pTSysCtrl->MonthPowerDownCnt = 0;
		pTSysCtrl->MonthPowerDownTime = 0;
		pTSysCtrl->LastAlarmDI = 0;
		pTSysCtrl->LatelyAlarmNum = 0;
		pTSysCtrl->GPRSRxTxBytes = 0;//err20180404-20 ͨ���������
		MC(0, ADDR_RS485_CH1_AMR_TaskCtrl, LEN_RS485_CH1_AMR_TaskCtrl * 4);

		//���ݳ�ʼ��ͬʱ��ʼ������洢�ĸ澯��־		
		MR((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		
		AlarmSavePara.AlarmDayReportFlag = 0;//err20180710 ÿ���ϱ�һ�εĸ澯��ʶ�������洢
		for(i = 0; i < SIZE_128; i++)//err20190109 ���ʱ�ӵ�ص�ѹ�͡����ܱ�ʱ���쳣�澯��־
		{
			AlarmSavePara.ClkBatLowVolAlarmFLagList[i] = 0x00;
			AlarmSavePara.TimeFaultAlarmFLagList[i] = 0x00;
		}
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
	}
	//if((u8)ResetType == 0x02)//Ӳ����λ
	{
		pCommRam->RESET = 0x55;					//0x55=������(����ʹ�ÿ��Ź���λ)
   		pCommRam->ACTION_RESET_PORTn = PORTn;	//������λ��ͨ�Ŷ˿ں� 
	}

}


#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)	

void GetACSampleAddr(u8* Addr)
{
	MR((u32)Addr, (u32)ADDR_04000401, METER_ADDR_SIZE);
}

void SetACSampleAddr(u8* Addr)
{
	MW((u32)Addr, (u32)ADDR_04000401, METER_ADDR_SIZE);
}

#endif

u16 FindNwDIFrom_AFN0C_0D_Tbl(u32 NwDI)
{
	u16 i = 0;
	for(i = 0; i < MAX_AFN0C_0D_IDENT_NUM; i++)
	{
		if(gc_stQCSGAFN0C_0D_DLT645_Table[i].u32Fn == NwDI) break;
	}
	return i;
}

//��RAM��ͨ�����ַ��ȡ������� 
u16 FindMeterInNodeList(u8* MeterAddr, NodeListStruct* NodeList, u16 NodeListNum)
{
	u16 i = 0;
	for(i = 0; i < NodeListNum; i++)
	{
		if((MeterAddr[0] == NodeList[i].Addr[0]) && (MeterAddr[1] == NodeList[i].Addr[1]) &&
		   (MeterAddr[2] == NodeList[i].Addr[2]) && (MeterAddr[3] == NodeList[i].Addr[3]) &&
		   (MeterAddr[4] == NodeList[i].Addr[4]) && (MeterAddr[5] == NodeList[i].Addr[5]))
		{
			break;
		}
	}
	return i;
}

//��FLASH��ͨ�����ַ��ȡ������� 
u16 FindMeterInMP(u8* MeterAddr, u16 NodeListNum)
{
	u16 i = 0;	
	MeasurePointStruct  MeasurePoint;
    u32 offset=0,flashLen=0;
	for(i = 0; i < NodeListNum; i++)
	{
		offset = ADDR_MP_START + i * (sizeof(MeasurePointStruct) + sizeof(PowerQualityParaStruct));
        flashLen= sizeof(MeasurePointStruct) ;
        MR((u32)&MeasurePoint, offset, flashLen);
		if((MeterAddr[0] == MeasurePoint.Addr[0]) && (MeterAddr[1] == MeasurePoint.Addr[1]) &&
		   (MeterAddr[2] == MeasurePoint.Addr[2]) && (MeterAddr[3] == MeasurePoint.Addr[3]) &&
		   (MeterAddr[4] == MeasurePoint.Addr[4]) && (MeterAddr[5] == MeasurePoint.Addr[5]) &&
		   (MeasurePoint.State == 0x01))//��� ��ַƥ����Ϊ��Ч��Ϊ�ҵ�
		{
			break;
		}
	}
	return i;
}


u16 FindPortNextMeterInNodeList(u32 PortID, NodeListStruct* NodeList, u16 StartIndex, u16 NodeListNum)
{
	u16 i = 0;
	for(i = StartIndex; i < NodeListNum; i++)
	{
		if((NodeList[i].Effective == 0x01) && (NodeList[i].PortID == (u8)PortID)) break;
	}
	return i;
}


void TerminalReportAlarmInc(void)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	if(pTSysCtrl->LatelyAlarmNum < REPORT_ALARM_MAX_NUM) pTSysCtrl->LatelyAlarmNum++;
}

void TerminalReportAlarmDec(void)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	if(pTSysCtrl->LatelyAlarmNum > 0) pTSysCtrl->LatelyAlarmNum--;
}

void TerminalSetLastAlarmDI(u32 AlarmDI)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	pTSysCtrl->LastAlarmDI = AlarmDI;
}

u32 TerminalGetLastAlarmDI(void)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	return pTSysCtrl->LastAlarmDI;
}

u32 Get_GPRSState(u32 PORTn)	// GPRSPORT
{
	UARTCtrl_TypeDef* UARTCtrl;
  	UARTCtrl = (UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	return UARTCtrl->State;
}

void Get_GPRSLocalIPAddr(u8* IPAddr)
{
	MR((u32)IPAddr, ADDR_MobilePhoneNumber+40, 16);
}

void Get_GPRSLocalPort(u8* LocalPort)
{
	MR((u32)LocalPort, ADDR_MobilePhoneNumber+40+16, 6);
}

u32 Get_GPRSSignaldBm(void)		 
{
	Terminal_Ram_TypeDef* pTerminalRam = Terminal_Ram;
	u32 SignaldBm = 0;
	SignaldBm = pTerminalRam->SignaldBm;
	if(SignaldBm >= 0x99) SignaldBm = 0;
	return SignaldBm;
}

u32 Get_GPRSRxTxBytes(void)
{
	UARTCtrl_TypeDef* pUARTCtrl = NULL;

	pUARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(GPRSPORT);

	return pUARTCtrl->RxTxByte;;
}

u32 Get_GPRSStoredRxTxBytes(void)
{
	TerminalRunTimeCtrlStruct* pTSysCtrl = (TerminalRunTimeCtrlStruct*)ADDR_Terminal_RunTime;
	return pTSysCtrl->GPRSRxTxBytes;;
}

void Clear_GPRSRxTxBytes(void)
{
	UARTCtrl_TypeDef* pUARTCtrl = NULL;

	pUARTCtrl = (UARTCtrl_TypeDef*)Get_ADDR_UARTnCtrl(GPRSPORT);

	pUARTCtrl->RxTxByte = 0;
}

u32 Get_GPRSCurMonthRxTxBytes(void)
{
	u32 CurMonthRxTxBytes = 0, CurRxTxBytes = 0;
	CurRxTxBytes = Get_GPRSStoredRxTxBytes();
	MR((u32)&CurMonthRxTxBytes, ADDR_E1800013, 3);
	CurMonthRxTxBytes = bcd_hex(CurMonthRxTxBytes);
	CurMonthRxTxBytes += CurRxTxBytes >> 10;
	CurMonthRxTxBytes = hex_bcd(CurMonthRxTxBytes);

	return CurMonthRxTxBytes;
}

u32 GetTerminalRouterInfo(u8* Data)
{
	Terminal_Router_TypeDef* pTerminalRouter = Terminal_Router;
	u32 Index = 0;
	
	Data[Index++] = pTerminalRouter->RouterMFCode[0];
	Data[Index++] = pTerminalRouter->RouterMFCode[1];

	Data[Index++] = pTerminalRouter->RouterICCode[0];
	Data[Index++] = pTerminalRouter->RouterICCode[1];

	Data[Index++] = pTerminalRouter->RouterVerDate[0];
	Data[Index++] = pTerminalRouter->RouterVerDate[1];
	Data[Index++] = pTerminalRouter->RouterVerDate[2];
	
	Data[Index++] = pTerminalRouter->RouterVer[0];
	Data[Index++] = pTerminalRouter->RouterVer[1];
	return Index;
}

void Get_MeterAMRSuccessInfo(u8* AmrInfo)
{	 
	u32 DayOkMeters = 0, MonthOkMeters = 0;
	MeterFileInfoStruct* MeterFileInfo = (MeterFileInfoStruct*)ADDR_MeterFileInfo;
/*	AMRCtrlStruct* AMRCtrl = NULL;
	
	if(MeterFileInfo->RS485_CH1_Meters != 0)
	{
		AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_1PORT);
		if(AMRCtrl != NULL)
		{
			DayOkMeters += AMRCtrl->DayFreezOkMeters;
			MonthOkMeters += AMRCtrl->MonthFreezOkMeters;
		}
	}

	if(MeterFileInfo->RS485_CH2_Meters != 0)
	{
		AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_2PORT);
		if(AMRCtrl != NULL)
		{
			DayOkMeters += AMRCtrl->DayFreezOkMeters;
			MonthOkMeters += AMRCtrl->MonthFreezOkMeters;
		}
	}

	if(MeterFileInfo->RS485_CH3_Meters != 0)
	{
		AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(RS485_3PORT);
		if(AMRCtrl != NULL)
		{
			DayOkMeters += AMRCtrl->DayFreezOkMeters;
			MonthOkMeters += AMRCtrl->MonthFreezOkMeters;
		}
	}

	if(MeterFileInfo->PLCMeters != 0)
	{
		AMRCtrl = (AMRCtrlStruct*)Get_AMR_TaskCtrlAddr(PLC_PORT);
		if(AMRCtrl != NULL)
		{
			DayOkMeters += AMRCtrl->DayFreezOkMeters;
			MonthOkMeters += AMRCtrl->MonthFreezOkMeters;
		}
	}
*/	
	DayOkMeters = MeterFileInfo->TotalMeters - MeterFileInfo->AMRDayFreezFailMeters;
	AmrInfo[0] = DayOkMeters;
	AmrInfo[1] = DayOkMeters >> 8;

	AmrInfo[2] = MeterFileInfo->AMRDayFreezFailMeters;
	AmrInfo[3] = MeterFileInfo->AMRDayFreezFailMeters >> 8;

	MonthOkMeters = MeterFileInfo->TotalMeters - MeterFileInfo->AMRMonthFreezFailMeters;
	AmrInfo[4] = MonthOkMeters;
	AmrInfo[5] = MonthOkMeters >> 8;

	AmrInfo[6] = MeterFileInfo->AMRMonthFreezFailMeters;
	AmrInfo[7] = MeterFileInfo->AMRMonthFreezFailMeters >> 8;
}



/****************************************************************************
 ��     ��: ��645֡
 ��ڲ� ��: DLT645FrameStruct* SrcFrame 
 ���ڲ� ��: u8* FrameBuf
 ��     ��: 645֡����
 ����ע ��:
 ��Ҫ�� ��:
 ��     д: zhaoyun
 ��дʱ ��; 2015-11-10
 ****************************************************************************/
u8 Build645Frame(DLT645FrameStruct* SrcFrame, u8* FrameBuf, u8 Need_FE)
{
	 u8 index = 0, i = 0, StartPos = 0;
	 if(Need_FE != NO_FE_IN_645)
	 {
		 FrameBuf[index++] = 0xFE;
		 FrameBuf[index++] = 0xFE;
		 FrameBuf[index++] = 0xFE;
		 FrameBuf[index++] = 0xFE;
		 StartPos = 4;
	 }
	 FrameBuf[index++] = DLT_645_HEAD;
	 for(i = 0; i < METER_ADDR_SIZE; i++)
	 {
		 FrameBuf[index++] = SrcFrame->Addr[i];
	 }
	 FrameBuf[index++] = DLT_645_HEAD;
	 FrameBuf[index++] = SrcFrame->CtrlCode;
	 FrameBuf[index++] = SrcFrame->DataLen;
	 for(i = 0; i < SrcFrame->DataLen; i++)
	 {
		 FrameBuf[index++] = SrcFrame->Data[i] + DLT_645_FIXED_DATA_SUM;
	 }
	 FrameBuf[index++] = SumCaculation(&FrameBuf[StartPos], (SrcFrame->DataLen + DLT_645_FIXED_LEN));
	 FrameBuf[index++] = DLT_645_TAIL;
	 return index;
}



void Init_ALLPara(void)
{	
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
	RealTimeStruct RealTime;
#endif
	MeasurePointStruct Mp;
	MW((u32)&DefaultCommuniactionPara, ADDR_CommunicationPara, sizeof(CommunicationParaStruct));
	MW((u32)&DefaultAlarmEventMaskPara, ADDR_AlarmEventMaskPara, sizeof(AlarmEventMaskParaStruct));
	MW((u32)&DefaultSettleDatePara, ADDR_SettlementDatePara, sizeof(SettleDateParaStruct));	
	MW((u32)&DeaultThresholdPara, ADDR_ThresholdPara, sizeof(ThresholdParaStruct));
	MW((u32)&DefaultRS485Port1Para, ADDR_RS485PortParaTbl, sizeof(RS485PortParaStruct));
	MW((u32)&DefaultRS485Port2Para, ADDR_RS485PortParaTbl + sizeof(RS485PortParaStruct), sizeof(RS485PortParaStruct));
#if ((Project/100)==2)||((Project/100)==3)	||((Project/100)==9)
	MW((u32)&DefaultRS485Port3Para, ADDR_RS485PortParaTbl + 2*sizeof(RS485PortParaStruct), sizeof(RS485PortParaStruct));
#endif
	MW((u32)&DefaultAttachPara, ADDR_AttachmentPara, sizeof(AttachmentParaStruct));
	MW((u32)&DefaultLoadManagementPara, ADDR_LoadManagementPara, sizeof(LoadManagementParaStruct));
	MW((u32)&DefaultSelfAdaptionPara, ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));//err20190218 ��������Ӧ������ʼ��

#if ((Project/100 == 3) && (USER == 3400))//err20190528 �������ϸ���GPRS����Ӧ����
	MW((u32)&Default_E0000111_CommunicationParaStruct, ADDR_CommunicationPara_E0000111, sizeof(E000011x_CommunicationParaStruct));//err20190218 ��������Ӧ������ʼ��
	MW((u32)&Default_E0000112_CommunicationParaStruct, ADDR_CommunicationPara_E0000112, sizeof(E000011x_CommunicationParaStruct));//err20190218 ��������Ӧ������ʼ��
	MW((u32)&Default_E0000113_CommunicationParaStruct, ADDR_CommunicationPara_E0000113, sizeof(E000011x_CommunicationParaStruct));//err20190218 ��������Ӧ������ʼ��

#endif


	MR((u32)&Mp, ADDR_MP_START, sizeof(MeasurePointStruct));

	//err20190218 ��ʼ��ͬʱ��ʼ������洢�ĸ澯��־
	MC(0, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));


	
#if ((Project/100)==2)||((Project/100)==3)||((Project/100)==9)
	GetACSampleAddr(Mp.Addr);
	if(MeterAddrCheck(Mp.Addr) != NO_ERROR)
	{
		HAL_GetSystemTime(&RealTime);
		MR((u32)Mp.Addr, (u32)&RealTime, METER_ADDR_SIZE);
		SetACSampleAddr(Mp.Addr);
	}
	Mp.State = 0x01;
	Mp.MeasureType = 0x05;
	Mp.ProtocolType = DLT_645_PROTOCOL_07;
	MW((u32)&Mp, ADDR_MP_START, sizeof(MeasurePointStruct));
#endif	
}


//err20181112 ���Ӹ澯 CT���ο�·��· ADC3�ɼ�����
#define VREF    250//ADC�����ѹx.xxV


#ifdef IC_STM32F4xx
u32 Get_ADC3(u32 Channel,u32 mul,u32 div,u32 ADDR_RAM)//ADC;���:Channel=ADC3ͨ����,mul=����,div=����,ADDR_RAM=�����ŵ�ַ; ����:����ֵ(0xffffffff��ʾת��ʧ��)
{
	u32 i;
	u32 t;
	u32 ReNum;
	u16 *p16;

//����ʱ��
	RCC->APB2ENR|=1<<10;
//��λ
	RCC->APB2RSTR=1<<8;
	RCC->APB2RSTR=0;
//��Ƶ
	ADC->CCR=0|//λ 31:24 ���������뱣�ָ�λֵ��
		 (0<<23)|//λ 23 TSVREFE���¶ȴ������� VREFINT ʹ�� (Temperature sensor and VREFINT enable)ͨ���������λ�� 1 �������ʹ��/��ֹ�¶ȴ������� VREFINT ͨ����
			//0����ֹ�¶ȴ������� VREFINT ͨ��
			//1��ʹ���¶ȴ������� VREFINT ͨ��
			//ע�⣺ ���� STM32F42x �� STM32F43x �������� TSVREFE λ�� 1 ʱ �����ֹ VBATE������λͬʱ�� 1 ʱ�������� VBAT ת����
		 (1<<22)|//λ 22 VBATE��VBAT ʹ�� (VBAT enable)ͨ���������λ�� 1 �������ʹ��/��ֹ VBAT ͨ����
			//0����ֹ VBAT ͨ��
			//1��ʹ�� VBAT ͨ��
		//λ 21:18 ���������뱣�ָ�λֵ��
		 (0<<17)|//λ 17:16 ADCPRE��ADC Ԥ��Ƶ�� (ADC prescaler)������� 1 �����㣬��ѡ�� ADC ��ʱ��Ƶ�ʡ���ʱ��Ϊ���� ADC �����á�ע��:
			//00��PCLK2 2 ��Ƶ
			//01��PCLK2 4 ��Ƶ
			//10��PCLK2 6 ��Ƶ
			//11��PCLK2 8 ��Ƶ
		//λ 15:14 DMA��ֱ�Ӵ洢������ģʽ�����ڶ�� ADC ģʽ��(Direct memory access mode for multi ADC mode)��λ������� 1 �����㡣�й���ϸ��Ϣ����μ� DMA ������һ�ڡ�
			//00����ֹ DMA ģʽ
			//01��ʹ�� DMA ģʽ 1������ 2/3 ���� - 1��2��3 ���ν��У�
			//10��ʹ�� DMA ģʽ 2���ɶ� 2/3 ���� - 2 �� 1��1 �� 3��3 �� 2 ���ν��У�
			//11��ʹ�� DMA ģʽ 3���ɶ� 2/3 �ֽ� - 2 �� 1��1 �� 3��3 �� 2 ���ν��У�
		//λ 13 DDS��DMA ��ֹѡ�񣨶��ڶ�� ADC ģʽ��(DMA disable selection (for multi-ADC mode))��λ������� 1 �����㡣
			//0�����һ�δ���󲻷����µ� DMA ������ DMA �������н������ã�DMA λ��ͨ��Ӳ�����㣬�������������µ� DMA ����ǰ��ͨ��������㲢����Ϊ��Ҫ��ģʽ��
			//1��ֻҪ���ݷ���ת���� DMA = 01��10 �� 11����ᷢ�� DMA ����
		//λ 12 ���������뱣�ָ�λֵ��
		//λ 11:8 DELAY��2 �������׶�֮����ӳ� (Delay between 2 sampling phases)������� 1 �����㡣��Щλ��˫�ػ����ؽ���ģʽ��ʹ�á�
			//0000��5 * TADCCLK
			//0001��6 * TADCCLK
			//0010��7 * TADCCLK
			//...
			//1111��20 * TADCCLK
		//λ 7:5 ���������뱣�ָ�λֵ��
		0;//λ 4:0 MULTI[4:0]������ ADC ģʽѡ�� (Multi ADC mode selection)ͨ�����д����Щλ��ѡ�����ģʽ��
			//�� ���� ADC ��������
			//00000������ģʽ
			//�� 00001 �� 01001��˫��ģʽ��ADC1 �� ADC2 һ������ADC3 ����
			//00001������ͬʱ + ע��ͬʱ���ģʽ
			//00010������ͬʱ + ���津�����ģʽ
			//00011��Reserved?
			//00101����ע��ͬʱģʽ
			//00110��������ͬʱģʽ
			//������ģʽ
			//01001�������津��ģʽ
			//�� 10001 �� 11001������ģʽ��ADC1��ADC2 �� ADC3 һ����
			//10001������ͬʱ + ע��ͬʱ���ģʽ
			//10010������ͬʱ + ���津�����ģʽ
			//10011��Reserved?
			//10101����ע��ͬʱģʽ
			//10110��������ͬʱģʽ
			//������ģʽ
			//11001�������津��ģʽ
			//����������Ͼ��豣���Ҳ�������
			//ע�⣺ �ڶ���ģʽ�£�����ͨ�����û�������ֹ����������ͬ����ʧ�������ڸ�������ǰ���ö��� ADC ģʽ��
	ADC3->SQR3=Channel;
			//λ 31:30 ���������뱣�ָ�λֵ��
			//λ 29:25 SQ6[4:0]�����������еĵ�����ת�� (6th conversion in regular sequence)ͨ�����д����Щλ������ͨ����� (0..18) ����Ϊ�����еĵ�����ת����
			//λ 24:20 SQ5[4:0]�����������еĵ����ת�� (5th conversion in regular sequence)
			//λ 19:15 SQ4[4:0]�����������еĵ��Ĵ�ת�� (4th conversion in regular sequence)
			//λ 14:10 SQ3[4:0]�����������еĵ�����ת�� (3rd conversion in regular sequence)
			//λ 9:5 SQ2[4:0]�����������еĵڶ���ת�� (2nd conversion in regular sequence)
			//λ 4:0 SQ1[4:0]�����������еĵ�һ��ת�� (1st conversion in regular sequence)
	ADC3->SQR2=0;
			//λ 31:30 ���������뱣�ָ�λֵ��
			//λ 29:26 SQ12[4:0]�����������еĵ�ʮ����ת�� (12th conversion in regular sequence)ͨ�����д����Щλ������ͨ����� (0..18) ����Ϊ�����еĵ�ʮ����ת����
			//λ 24:20 SQ11[4:0]�����������еĵ�ʮһ��ת�� (11th conversion in regular sequence)
			//λ 19:15 SQ10[4:0]�����������еĵ�ʮ��ת�� (10th conversion in regular sequence)
			//λ 14:10 SQ9[4:0]�����������еĵھŴ�ת�� (9th conversion in regular sequence)
			//λ 9:5 SQ8[4:0]�����������еĵڰ˴�ת�� (8th conversion in regular sequence)
			//λ 4:0 SQ7[4:0]�����������еĵ��ߴ�ת�� (7th conversion in regular sequence)
	ADC3->SQR1=0;
			//λ 31:24 ���������뱣�ָ�λֵ��
			//λ 23:20 L[3:0]������ͨ�����г��� (Regular channel sequence length)ͨ�����д����Щλ�ɶ������ͨ��ת�������е�ת��������
				//0000��1 ��ת��
				//0001��2 ��ת��
				//...?
				//1111��16 ��ת��
			//λ 19:15 SQ16[4:0]�����������еĵ�ʮ����ת�� (16th conversion in regular sequence)ͨ�����д����Щλ������ͨ����� (0..18) ����Ϊת�������еĵ�ʮ����ת����
			//λ 14:10 SQ15[4:0]�����������еĵ�ʮ���ת�� (15th conversion in regular sequence)
			//λ 9:5 SQ14[4:0]�����������еĵ�ʮ�Ĵ�ת�� (14th conversion in regular sequence)
			//λ 4:0 SQ13[4:0]�����������еĵ�ʮ����ת�� (13th conversion in regular sequence)
		
	ADC3->CR1=0;
			//λ 31:27 ���������뱣�ָ�λֵ��
			//λ 26 OVRIE������ж�ʹ�� (Overrun interrupt enable)ͨ���������λ�� 1 �������ʹ��/��ֹ����жϡ�
				//0����ֹ����ж�
				//1��ʹ������жϡ�OVR λ�� 1 ʱ�����жϡ�
			//λ 25:24 RES[1:0]���ֱ��� (Resolution)ͨ�����д����Щλ��ѡ��ת���ķֱ��ʡ�
				//00��12 λ��15 ADCCLK ���ڣ�
				//01��10 λ��13 ADCCLK ���ڣ�
				//10��8 λ��11 ADCCLK ���ڣ�
				//11��6 λ��9 ADCCLK ���ڣ�
			//λ 23 AWDEN������ͨ���ϵ�ģ�⿴�Ź�ʹ�� (Analog watchdog enable on regular channels)��λ������� 1 �����㡣
				//0���ڹ���ͨ���Ͻ�ֹģ�⿴�Ź�
				//1���ڹ���ͨ����ʹ��ģ�⿴�Ź�
			//λ 22 JAWDEN��ע��ͨ���ϵ�ģ�⿴�Ź�ʹ�� (Analog watchdog enable on injected channels)��λ������� 1 �����㡣
				//0����ע��ͨ���Ͻ�ֹģ�⿴�Ź�
				//1����ע��ͨ����ʹ��ģ�⿴�Ź�
			//λ 21:16 ���������뱣�ָ�λֵ��
			//λ 15:13 DISCNUM[2:0]������������ģʽͨ������ (Discontinuous mode channel count)�����д����Щλ�����ڶ����ڽ��յ��ⲿ�������ڲ���������ģʽ��ת���Ĺ���ͨ������
				//000��1 ��ͨ��
				//001��2 ��ͨ��
				//...?
				//111��8 ��ͨ��
			//λ 12 JDISCEN��ע��ͨ���Ĳ���������ģʽ (Discontinuous mode on injected channels)ͨ���������λ�� 1 �������ʹ��/��ֹע��ͨ���Ĳ���������ģʽ��
				//0����ֹע��ͨ���Ĳ���������ģʽ
				//1��ʹ��ע��ͨ���Ĳ���������ģʽ
			//λ 11 DISCEN������ͨ���Ĳ���������ģʽ (Discontinuous mode on regular channels)ͨ���������λ�� 1 �������ʹ��/��ֹ����ͨ���Ĳ���������ģʽ��
				//0����ֹ����ͨ���Ĳ���������ģʽ
				//1��ʹ�ܹ���ͨ���Ĳ���������ģʽ
			//λ 10 JAUTO��ע�����Զ�ת�� (Automatic injected group conversion)ͨ���������λ�� 1 ��������ڹ�����ת����ֱ�ʹ��/��ֹע�����Զ�ת����
				//0����ֹע�����Զ�ת��
				//1��ʹ��ע�����Զ�ת��
			//λ 9 AWDSGL����ɨ��ģʽ��ʹ�ܵ�һͨ���ϵĿ��Ź� (Enable the watchdog on a single channelin scan mode)ͨ���������λ�� 1 ������ɷֱ�ʹ��/��ֹͨ�� AWDCH[4:0] λȷ����ͨ���ϵ�ģ�⿴�Ź���
				//0��������ͨ����ʹ��ģ�⿴�Ź�
				//1���ڵ�һͨ����ʹ��ģ�⿴�Ź�
			//λ 8 SCAN��ɨ��ģʽ (Scan mode)ͨ���������λ�� 1 �������ʹ��/��ֹɨ��ģʽ����ɨ��ģʽ�£�ת��ͨ�� ADC_SQRx ��ADC_JSQRx �Ĵ���ѡ������롣
				//0����ֹɨ��ģʽ
				//1��ʹ��ɨ��ģʽ
				//ע�⣺ EOCIE λ�� 1 ʱ������ EOC �жϣ�
				//�� ��� EOCS λ���㣬��ÿ������������ת������ʱ
				//�� ��� EOCS λ�� 1����ÿ������ͨ��ת������ʱ
				//ע�⣺ JEOCIE λ�� 1 ʱ��JEOC �жϽ������һ��ͨ��ת������ʱ���ɡ�
			//λ 7 JEOCIE��ע��ͨ�����ж�ʹ�� (Interrupt enable for injected channels)ͨ���������λ�� 1 �������ʹ��/��ֹע��ͨ����ת�������жϡ�
				//0����ֹ JEOC �ж�
				//1��ʹ�� JEOC �жϡ�JEOC λ�� 1 ʱ�����жϡ�
			//λ 6 AWDIE��ģ�⿴�Ź��ж�ʹ�� (Analog watchdog interrupt enable)ͨ���������λ�� 1 �������ʹ��/��ֹģ�⿴�Ź��жϡ�
				//0����ֹģ�⿴�Ź��ж�
				//1��ʹ��ģ�⿴�Ź��ж�
			//λ 5 EOCIE��EOC �ж�ʹ�� (Interrupt enable for EOC)ͨ���������λ�� 1 �������ʹ��/��ֹת�������жϡ�
				//0����ֹ EOC �ж�
				//1��ʹ�� EOC �ж�EOC λ�� 1 ʱ�����жϡ�
			//λ 4:0 AWDCH[4:0]��ģ�⿴�Ź�ͨ��ѡ��λ (Analog watchdog channel select bits)��Щλ��������� 1 �����㡣��������ѡ����ģ�⿴�Ź���ص�����ͨ����
				//ע�⣺ 
				//00000��ADC ģ������ͨ�� 0
				//00001��ADC ģ������ͨ�� 1
				//...?
				//01111��ADC ģ������ͨ�� 15
				//10000��ADC ģ������ͨ�� 16
				//10001��ADC ģ������ͨ�� 17
				//10010��ADC ģ������ͨ�� 18
				//��������ֵ
	ADC3->CR2=0|
			//λ 31 ���������뱣�ָ�λֵ��
			//λ 30 SWSTART����ʼת������ͨ�� (Start conversion of regular channels)ͨ���������λ�� 1 �ɿ�ʼת������Ӳ������ת����ʼ�󽫸�λ���㡣
				//0����λ״̬
				//1����ʼת������ͨ��
				//ע�⣺ ��λֻ���� ADON = 1 ʱ�� 1�����򲻻�����ת����
			//λ 29:28 EXTEN������ͨ�����ⲿ����ʹ�� (External trigger enable for regular channels)ͨ���������Щλ�� 1 �������ѡ���ⲿ�������Ժ�ʹ�ܹ�����Ĵ�����
				//00����ֹ�������
				//01���������ϵĴ������
				//10���½����ϵĴ������
				//11�������غ��½����ϵĴ������
			//λ 27:24 EXTSEL[3:0]��Ϊ������ѡ���ⲿ�¼� (External event select for regular group)��Щλ��ѡ�����ڴ���������ת�����ⲿ�¼���
				//0000����ʱ�� 1 CC1 �¼�
				//0001����ʱ�� 1 CC2 �¼�
				//0010����ʱ�� 1 CC3 �¼�
				//0011����ʱ�� 2 CC2 �¼�
				//0100����ʱ�� 2 CC3 �¼�
				//0101����ʱ�� 2 CC4 �¼�
				//0110����ʱ�� 2 TRGO �¼�
				//0111����ʱ�� 3 CC1 �¼�
				//1000����ʱ�� 3 TRGO �¼�
				//1001����ʱ�� 4 CC4 �¼�
				//1010����ʱ�� 5 CC1 �¼�
				//1011����ʱ�� 5 CC2 �¼�
				//1100����ʱ�� 5 CC3 �¼�
				//1101����ʱ�� 8 CC1 �¼�
				//1110����ʱ�� 8 TRGO �¼�
				//1111��EXTI �� 11
			//λ 23 ���������뱣�ָ�λֵ��
			//λ 22 JSWSTART����ʼת��ע��ͨ�� (Start conversion of injected channels)ת����ʼ���������λ�� 1����Ӳ������λ���㡣
				//0����λ״̬
				//1����ʼת��ע��ͨ��
				//ע�⣺ ��λֻ���� ADON = 1 ʱ�� 1�����򲻻�����ת����
			//λ 21:20 JEXTEN��ע��ͨ�����ⲿ����ʹ�� (External trigger enable for injected channels)ͨ���������Щλ�� 1 �������ѡ���ⲿ�������Ժ�ʹ��ע����Ĵ�����
				//00����ֹ�������
				//01���������ϵĴ������
				//10���½����ϵĴ������
				//11�������غ��½����ϵĴ������
			//λ 19:16 JEXTSEL[3:0]��Ϊע����ѡ���ⲿ�¼� (External event select for injected group)��Щλ��ѡ�����ڴ���ע����ת�����ⲿ�¼���
				//0000����ʱ�� 1 CC4 �¼�
				//0001����ʱ�� 1 TRGO �¼�
				//0010����ʱ�� 2 CC1 �¼�
				//0011����ʱ�� 2 TRGO �¼�
				//0100����ʱ�� 3 CC2 �¼�
				//0101����ʱ�� 3 CC4 �¼�
				//0110����ʱ�� 4 CC1 �¼�
				//0111����ʱ�� 4 CC2 �¼�
				//1000����ʱ�� 4 CC3 �¼�
				//1001����ʱ�� 4 TRGO �¼�
				//1010����ʱ�� 5 CC4 �¼�
				//1011����ʱ�� 5 TRGO �¼�
				//1100����ʱ�� 8 CC2 �¼�
				//1101����ʱ�� 8 CC3 �¼�
				//1110����ʱ�� 8 CC4 �¼�
				//1111��EXTI �� 15
			//λ 15:12 ���������뱣�ָ�λֵ��
			(0<<11)|//λ 11 ALIGN�����ݶ��� (Data alignment)��λ������� 1 �����㡣��μ�ͼ 38 ��ͼ 39��
				//0���Ҷ���
				//1�������
			(0<<10)|//λ 10 EOCS������ת��ѡ�� (End of conversion selection)��λ������� 1 �����㡣
				//0����ÿ������ת�����н���ʱ�� EOC λ�� 1����������� DMA=1 ʱʹ�ܡ�
				//1����ÿ������ת������ʱ�� EOC λ�� 1��ʹ�������⡣
			//λ 9 DDS��DMA ��ֹѡ�񣨶��ڵ�һ ADC ģʽ�� (DMA disable selection (for single ADC mode))��λ������� 1 �����㡣
				//0�����һ�δ���󲻷����µ� DMA ������ DMA �������н������ã�
				//1��ֻҪ��������ת���� DMA = 1����ᷢ�� DAM ����
			//λ 8 DMA��ֱ�Ӵ洢������ģʽ�����ڵ�һ ADC ģʽ��(Direct memory access mode (for single ADC mode))��λ������� 1 �����㡣�й���ϸ��Ϣ����μ� DMA ������һ�¡�
				//0����ֹ DMA ģʽ
				//1��ʹ�� DMA ģʽ
			//λ 7:2 ���������뱣�ָ�λֵ��
			//λ 1 CONT������ת�� (Continuous conversion)��λ������� 1 �����㡣��λ�� 1 ʱ��ת�����������У�ֱ����λ���㡣
				//0������ת��ģʽ
				//1������ת��ģʽ
			1;//λ 0 ADON��A/D ת�������� / �ر� (A/D Converter ON / OFF)��λ������� 1 �����㡣ע�⣺ 
				//0����ֹ ADC ת����ת������ģʽ
				//1��ʹ�� ADC
#define SMPx     7
	ADC3->SMPR2=SMPx+(SMPx<<3)+(SMPx<<6)+(SMPx<<9)+(SMPx<<12)+(SMPx<15)+(SMPx<<18)+(SMPx<<21)+(SMPx<<24)+(SMPx<<27);//SMP0-SMP9
			//λ 31:30 ���������뱣�ָ�λֵ��
			//λ 29:0 SMPx[2:0]��ͨ�� X ����ʱ��ѡ�� (Channel x sampling time selection)ͨ�����д����Щλ�ɷֱ�Ϊ����ͨ��ѡ�����ʱ�䡣�ڲ��������ڼ䣬ͨ��ѡ��λ���뱣�ֲ��䡣
				//000��3 ������
				//001��15 ������
				//010��28 ������
				//011��56 ������
				//100��84 ������
				//101��112 ������
				//110��144 ������
				//111��480 ������
	ADC3->SMPR1=SMPx+(SMPx<<3)+(SMPx<<6)+(SMPx<<9)+(SMPx<<12)+(SMPx<15)+(SMPx<<18)+(SMPx<<21)+(SMPx<<24);//SMP10-SMP18
			//λ 31:27 ���������뱣�ָ�λֵ��
			//λ 26:0 SMPx[2:0]��ͨ�� X ����ʱ��ѡ�� (Channel x sampling time selection)ͨ�����д����Щλ�ɷֱ�Ϊ����ͨ��ѡ�����ʱ�䡣�ڲ��������ڼ䣬ͨ��ѡ��λ���뱣�ֲ��䡣
				//000��3 ������
				//001��15 ������
				//010��28 ������
				//011��56 ������
				//100��84 ������
				//101��112 ������
				//110��144 ������
				//111��480 ������
				
	i=0;
	ReNum=4;
	while(ReNum--)
	{
//		PWR->CR|=(1<<13);//ADCDC1=1
//		FLASH->ACR&=~(1<<8);//�ر�Ԥȡ
		ADC3->CR2|=(1<<30);//��ʼת������ͨ��
		t=0xA410;//t=RCC_GetHCLKFreq()/4000;//1ms������ʱ
		while(1)//λ 1 EOC ������ͨ��ת��������Ӳ������λ�� 1��ͨ�������ͨ����ȡ ADC_DR �Ĵ�������λ����
		{
			if(ADC3->SR&0x2)
			{
				break;
			}
			if(Comm_Ram->IntFlags&4)//B2=�ж���ʹ��ADC��ͻ��־
			{
				Comm_Ram->IntFlags=0;
				return 0xffffffff;//�жϵ���ADCת�����ѹر���ADC
			}
			t--;
			if(t==0)
			{//ADC����
				return 0xffffffff;
			}
		}
//		FLASH->ACR|=(1<<8);//ʹ��Ԥȡ
//		PWR->CR&=~(1<<13);//ADCDC1=0
		i+=ADC3->DR;
	}
//�ر�ADC
	ADC3->CR2=0;//ADON λ������ֹͣת����ʹ ADC �������ģʽ���ڴ�ģʽ�£�ADC �������ĵ� ��ֻ�м� ��A��	
	RCC->APB2ENR&=~(1<<10);
//����
	i*=mul;
	i*=VREF;
	i/=(0xfff*4);
	i/=div;
	i=hex_bcd(i);
	
	if(ADDR_RAM!=0)
	{
		p16=(u16 *)(ADDR_RAM);
		p16[0]=i;
	}
	return i;
}
#endif



/************************ (C) COPYRIGHT M***********************END OF FILE****/
