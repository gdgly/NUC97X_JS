/*************************************************
版  权:正泰仪表智能终端部
文  件:QCSG_PublicFunc.c
作  者:mxn
版  本:1.00
日  期:20170601
描  述:南网协议公用函数文件
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
*功    能:绝对值
*输入参数:fx
*输出参数:无
*返    回:绝对值
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
*功    能:SIN函数
*输入参数:fx
*输出参数:无
*返    回:SIN值
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
*功    能:COS函数
*输入参数:fx
*输出参数:无
*返    回:COS值
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
*功    能:开方函数
*输入参数:fa
*输出参数:无
*返    回:开方值
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
*功    能:获取当前通道连接状态
*输入参数:u8ChNo
*输出参数:无
*返    回:是否连接
--------------------------------------------------------------------**/
u8 QCSG_GetCHConnectState(u8 u8ChNo)
{
    return TRUE;
}
/**--------------------------------------------------------------------
*功    能:时间减法
*输入参数:TimerMinuend，TimerSubtrahend
*输出参数:
       Result[0] //相差的天数,
       Result[1] //相差的小时数(除去天之后）,
       Result[2] //相差的分钟数（除去小时之后）,
       Result[3] //相差的秒数（除去分钟之后）
*返    回:
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
*功    能:时间加法：TIME_PARAM_S *TimerTotal = TIME_PARAM_S *TimerAddend+u32 Sec
*输入参数:TimerAddend,Sec
*输出参数:TimerTotal
*返    回:年溢出
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
*功    能:BCD判断
*输入参数:u8Bcd
*输出参数:无
*返    回:计算结果
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
*功    能:BCD2HEX
*输入参数:u8Bcd
*输出参数:无
*返    回:计算结果
--------------------------------------------------------------------**/
u8 QCSG_BCD2HEX(u8 u8BCD)
{
    u8 u8Hex;
    u8Hex = (u8BCD>>4)*10 + (u8BCD&0x0F);
    return u8Hex;
}
/**--------------------------------------------------------------------
*功    能:HEX2BCD
*输入参数:u8HEX
*输出参数:无
*返    回:计算结果
--------------------------------------------------------------------**/
u8 QCSG_HEX2BCD(u8 u8HEX)
{
    u8 u8Bcd;
    u8Bcd = (u8HEX/10)*16 + (u8HEX%10);
    return u8Bcd;
}
/**--------------------------------------------------------------------
*功    能:BCD加1
*输入参数:u8Bcd
*输出参数:无
*返    回:计算结果
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
*功    能:BCD减1，不够减，返回0
*输入参数:u8Bcd
*输出参数:无
*返    回:计算结果
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
*功    能:计算一个月最大天数
*输入参数:pstTime
*输出参数:无
*返    回:计算结果
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
			if(LeapYearCheck(YearInDec + 2000) != 0)			//闰年判断
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
*功    能:时间加一小时
*输入参数:pstTime
*输出参数:pstTime
*返    回:是否加成功
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
*功    能:时间加一小时
*输入参数:pstTime
*输出参数:pstTime
*返    回:是否加成功
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
*功    能:时间加一天
*输入参数:pstTime
*输出参数:pstTime
*返    回:是否加成功
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
*功    能:时间减一天
*输入参数:pstTime
*输出参数:pstTime
*返    回:是否减成功
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
*功    能:时间加一月
*输入参数:pstTime
*输出参数:pstTime
*返    回:是否加成功
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
*功    能:时间减一月
*输入参数:pstTime
*输出参数:pstTime
*返    回:是否减成功
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
*功    能:计算和校验
*输入参数:pu8Buf：输入缓冲，u16Len：缓冲长度
*输出参数:无
*返    回:计算结果
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
*功    能:组645协议帧
*输入参数:pu8MeterID：表号，u8CtrlCode：控制码，u16InLen:输入缓冲长度，pu8InBuf:输入缓冲
*输出参数:pu16OutLen：输出帧长度，pu8OutBuf：输入缓冲
*返    回:组帧成功or失败
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
*功    能:组EDMI协议帧
*输入参数:pu8MeterID：表号，u8CtrlCode：控制码，u16InLen:输入缓冲长度，pu8InBuf:输入缓冲
*输出参数:pu16OutLen：输出帧长度，pu8OutBuf：输入缓冲
*返    回:组帧成功or失败
--------------------------------------------------------------------**/

u8 QCSG_MakeEDMIFrame_Test(u8 u8FEFlag,u8* pu8MeterID,u8 u8CtrlCode,u8* pu8InBuf,u8 u8InLen,u8 u8OutBufSize,u8* pu8OutLen,u8* pu8OutBuf)
{//待补充
	//EDMI登陆帧
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

	u8 i,j=1;	//j不要依赖于i即可,与datasize++类似思路
	outbuf[0]=0x02;
	for(i=1;i<(inlen-1);i++){	//首尾不检查
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
//根据南网数据标识找到对应edmi数据标识列表
//返回edmi数据项个数
u8 get_edmi_cmd(u32 nwcmd,u32* edmicmd){
	u8 i,num=0;

	if((nwcmd | 0x0000FFFF) == 0x0506FFFF)//冻结数据
	{
		for(i = 0;i < MAX_FROZEN_EDMI_IDENT_NUM;i++)
		{
			if(nwcmd==gc_Jc_Rs485_FrozenReadCode_EDMI[i].u32Fn )
			{//找到了对应 的南网数据项
				while(gc_Jc_Rs485_FrozenReadCode_EDMI[i].stEDMIFROZENResolve[num].u32Ident!=0xffff){
					edmicmd[num]=gc_Jc_Rs485_FrozenReadCode_EDMI[i].stEDMIFROZENResolve[num].u32Ident;
					num++;
				}
				break;
			}
		}
	}
	else	//曲线数据
	{
		for(i=0;i<MAX_FIXED_EDMI_NUM;i++){
			if(nwcmd==EDMI_cmd[i].NwCmd ){//找到了对应 的南网数据项

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

//根据南网数据标识找到对应的数据转换模式
//返回数据转换模式
u32 get_edmi_method(u32 nwcmd){
	u8 i = 0;

	if((nwcmd | 0x0000FFFF) == 0x0506FFFF)//冻结数据
	{
		for(i = 0;i < MAX_FROZEN_EDMI_IDENT_NUM;i++)
		{
			if(nwcmd==gc_Jc_Rs485_FrozenReadCode_EDMI[i].u32Fn )
			{//找到了对应 的南网数据项
				return gc_Jc_Rs485_FrozenReadCode_EDMI[i].u32FixMethod;
			}
		}
	}
	else 
	{
		for(i = 0;i < MAX_FIXED_EDMI_NUM;i++)
		{
			if(nwcmd==EDMI_cmd[i].NwCmd )
			{//找到了对应 的南网数据项
				return EDMI_cmd[i].u32FixMethod;
			}
		}
	}
	//没找到 直接拷贝
	return COMMON_COV_METHOD;
}


u8 QCSG_MakeEDMIFrame(u8* pu8MeterID,u8* ident,u8* psendlen,u8* psendbuf){
	u8 tmp[60]={0};
	u32 addr,nwcmd;
	u16 check;
	u32 edmicmd[8];	//8需用宏MAX_EDMI_INFO_NUM代替
	
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
	
	tmp[12]='M';	//即0x4d
	
	tmp[15]=0xff;
	tmp[16]=0xf0;

	nwcmd=ident[0]+ident[1]*0x100+ident[2]*0x10000+ident[3]*0x1000000;

	if((nwcmd != 0x0000fffc) && (nwcmd != 0x0000fffd))
		edmi_num = get_edmi_cmd(nwcmd,edmicmd);
	else 
		edmi_num = 1;
	
	if(edmi_num==0) 
		return FALSE;//没有相应的edmi数据项
	

	switch(nwcmd){

		case 0x0000fffd://登录
			tmp[12]=0x4c;	//即'L'
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

			len=27;//包含tmp[26]=00，终止字节		
			break;

		case 0x0001ff00:
		case 0x0002ff00:
		case 0x0003ff00:
		case 0x0004ff00:
		case 0x0005ff00:
		case 0x0006ff00:
		case 0x0007ff00:
		case 0x0008ff00:

		case 0x0201ff00:	//电流
		case 0x0202ff00://电压
		case 0x0203ff00://有功功率
		case 0x0204ff00://无功功率
		case 0x0207ff00://相角
			index=19;
			for(i=0;i<edmi_num;i++){
				tmp[index++]=(edmicmd[i]>>8)&0xff;
				tmp[index++]=edmicmd[i]&0xff;
				index+=2;
			}
			index-=2;	//最后一个数据项后面多加了2，要减去
			//tmp[index++]='D';	//0x44，不加该字节也抄的通，暂时统一不加
			len=index;
			break;


		case 0x0000fffc://退出登录
			tmp[12]=0x58;	//即'X'
			len=13;
			break;
		default://日冻结
			index=19;
			for(i=0;i<edmi_num;i++){
				tmp[index++]=(edmicmd[i]>>8)&0xff;
				tmp[index++]=edmicmd[i]&0xff;
				index+=2;
			}
			index-=2;	//最后一个数据项后面多加了2，要减去
			//tmp[index++]='D';	//0x44，不加该字节也抄的通，暂时统一不加
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
	
	tmp[12]=0x4d;	//即'M'
	
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
*功    能:645帧格式判断
*输入参数:pu8Data：输入缓冲，pu8DataLen：缓冲长度
*输出参数:pu8Data：输入缓冲，pu8DataLen：缓冲长度
*返    回:格式是否正确
--------------------------------------------------------------------**/
u8 QCSG_Dlt645FrameJudge(u8* pu8Data,u8* pu8DataLen)
{
	//数据层已经保证645帧的准确性，并且已经去掉0xFE，20170814
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
*功    能:EDMI帧格式判断
*输入参数:pu8Data：输入缓冲，pu8DataLen：缓冲长度
*输出参数:pu8Data：输入缓冲，pu8DataLen：缓冲长度
*返    回:格式是否正确
--------------------------------------------------------------------**/
u8 QCSG_EDMIFrameJudge(u8* pu8Data,u8* pu8DataLen)
{
	//在此之前帧已经处理为原始数据帧 不需要进行0x10的判断处理
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

	if(pu8Data[12] == 0x18)//错误码
	{
		return FALSE;
	}
	
	(*pu8DataLen) = DataLen;
	return TRUE;
}

/**--------------------------------------------------------------------
*功    能:组南网登录帧
*输入参数:pu8Addr:终端地址,u8Pseq:帧序列号
*输出参数:pu8OutLen：输出帧长度，pu8OutBuf：输出缓冲
*返    回:成功or失败
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
    //规约版本号
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
*功    能:组南心跳帧
*输入参数:pu8Addr:终端地址,u8Pseq:帧序列号
*输出参数:pu8OutLen：输出帧长度，pu8OutBuf：输出缓冲
*返    回:成功or失败
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
*功    能:组南网退出登录帧
*输入参数:pu8Addr:终端地址,u8Pseq:帧序列号
*输出参数:pu8OutLen：输出帧长度，pu8OutBuf：输出缓冲
*返    回:成功or失败
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
*功    能:通知通道重新连接
*输入参数:u8ChNo：通道号
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
void QCSG_NoticeReconnect(u8 u8ChNo)
{

}
/**--------------------------------------------------------------------
*功    能:通知重要参数变更,上行协议设置参数时调用
*输入参数:u8ChNo：通道号
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
void QCSG_NoticeDIChange(QCSG_DICHANGE_E eDi,u8 u8Flag)
{
	u8* pu8Value = (u8*)QCSG_DICHARGE_FLAG_LIST;
	pu8Value[eDi] = u8Flag;
}
/**--------------------------------------------------------------------
*功    能:获取参数变更信息，控制及抄表适时调用
*输入参数:u8ChNo：通道号
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
u8 QCSG_GetDIChange(QCSG_DICHANGE_E eDi)
{
	u8* pu8Value = (u8*)QCSG_DICHARGE_FLAG_LIST;
	return pu8Value[eDi];
}
/**--------------------------------------------------------------------
*功    能:南网响应帧组织及发送
*输入参数:u8ChNo:发送通道，u8FrameFlag：同FIR，FIN定义，pstQCSG_Info：协议缓冲
*输出参数:无
*返    回:发送成功or失败
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
     * TODO:数据发送函数待补充
     */
    return TRUE;
}
/**--------------------------------------------------------------------
*功    能:南网上报帧组织及发送
*输入参数:u8ChNo:发送通道，u8FrameFlag：同FIR，FIN定义，pstReport_Info：协议缓冲
*输出参数:无
*返    回:发送成功or失败
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
     * TODO:数据发送函数待补充
     */
    return TRUE;
}
/**--------------------------------------------------------------------
*功    能:提取南网协议帧
*输入参数:u16InLen:输入缓冲长度，pu8InBuf:输入缓冲
*输出参数:pu16OutLen：输出帧长度，pu8OutBuf：输出缓冲
*返    回:提取成功or失败
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
*功    能:提取南网协议帧中关键元素
*输入参数:u16InLen:输入长度，pu8InBuf：输入缓冲
*输出参数:pstQCSG_Info
*返    回:提取成功or失败
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
 *功    能:判断协议地址是否合法
 *输入参数:pu8Addr：地址
 *输出参数:无
 *返    回:是否合法
 --------------------------------------------------------------------**/
u8 QCSG_Chek_Addr(u8* pu8Addr)
{
    /*
     * TODO:待补充，读本机地址函数未确定
     */
    return TRUE;
}
/**--------------------------------------------------------------------
 *功    能:判断时间标签
 *输入参数:pu8Data：数据
 *输出参数:pstQ13761_Info:输出缓冲
 *返    回:是否合法
 --------------------------------------------------------------------**/
u8 QCSG_Chek_Tpv(u8* pu8Data,QCSG_RESOLVE_INFO_S* pstQCSG_Info)
{
    /*
     * TODO:系统时间获取函数没明确，待完成此函数
     */
    return TRUE;
}
/**--------------------------------------------------------------------
*功    能:提取Pn
*输入参数:pu8Buf:缓冲
*输出参数:无
*返    回:Pn
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
        //无效测量点
        return 0xfffe;
    }
    if((DA2 == 0xff)&&(DA1 == 0xff))
    {
        //全部测量点
        return 0xffff;
    }
    u16Pn = (u16)DA2;
    u16Pn = u16Pn << 8;
    u16Pn = u16Pn + DA1;
    if(u16Pn > PN_MAX)
    {
        //无效测量点
        return 0xfffe;
    }
*/
    u16Pn += pu8Buf[1];
    u16Pn <<= 8;
    u16Pn += pu8Buf[0];
    return u16Pn;
}
/**--------------------------------------------------------------------
*功    能:提取Fn
*输入参数:pucBuf:缓冲
*输出参数:无
*返    回:Fn
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
*功    能:判断AFN04，Fn是否支持
*输入参数:u32Fn：输入Fn
*输出参数:pu16Index:输出表格下标
*返    回:解析结果
--------------------------------------------------------------------**/
u8 QCSG_Check_AFN04_0AFn(u32 u32Fn,u16* pu16Index)
{
    u16 i = 0,u16FnNum = 0;

    u16FnNum = MAX_AFN040A_IDENT_NUM;
    if((u32Fn & 0xFFFFFF00) == 0xE0000300)
    {
        //任务参数表格做了特殊处理
        u32Fn = 0xE00003FF;
    }
    if((u32Fn & 0xFFFFFF00) == 0xE0000400)
    {
        //任务参数表格做了特殊处理
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
*功    能:判断告警是否配置
*输入参数:u32AlarmIdent:告警标识
*输出参数:无
*返    回:TRUE：配置，FALSE：未配置
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
*功    能:判断告警上报是否配置
*输入参数:u32AlarmIdent:告警标识
*输出参数:无
*返    回:TRUE：配置，FALSE：未配置
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
*功    能:根据端口号读取中继任务
*输入参数:u8Port:端口号，u8Dimnumber：输入数组大小
*输出参数:pu8GetCount：获取到的数量，pu8TaskList：任务列表,0,1,2,3...
*返    回:读取结果
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
*功    能:获取端口下的Pn列表
*输入参数:u8Port:端口号
*输出参数:pu16PnList,pu8PnNum
*返    回:无
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
		#if ((Project/100)==9)//err20181212 增加限制，只有配变需要设置为总表
			if(MeasurePoint.TotalType == 0)//err20181017 配变 总表采用负控代码产生告警
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
*功    能:更新Pn列表
*输入参数:u8Port:端口号
*输出参数:pu16PnList,pu8PnNum
*返    回:无
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
*功    能:更新中继任务列表
*输入参数:u8Port:端口号，pstRelayTask_List
*输出参数:pstRelayTask_List
*返    回:无
--------------------------------------------------------------------**/
void QCSG_UpdateRelayTaskList(u8 u8Port,QCSG_RDRELAYTASK_INFO_S* pstRelayTask_List)
{
    u8 i = 0,j = 0;
    u8 u8NewTaskId = 0,u8OldTaskId = 0;

    //更新执行的任务列表，往当前的表后面加
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
*功    能:更新普通任务
*输入参数:pstCommonTask_List
*输出参数:pstCommonTask_List
*返    回:无
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
*功    能:判断任务定时时间是否到
*输入参数:pstCurrTime，pstRefTime，u8PeriodValue,u8PeriodUnit
*输出参数:无
*返    回:时间是否到
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
        //当前时间小于参考时间，返回时间未到
        return FALSE;
    }
    if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result) == 0)
    {
        //到了参考时间
        return TRUE;
    }
    if(u8PeriodUnit == 3)
    {
        //周期单位为月
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
                //时间加过，说明未到上报时间
                return FALSE;
            }
        }
        if((stCurrTime.u8Day == stRefTime.u8Day)&&(stCurrTime.u8Hour == stRefTime.u8Hour))
        {
            if((stCurrTime.u8Month != stLastTime.u8Month))
            {
                //上一次执行的月不一样，说明是新周期
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
        //时间单位为日
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
                //时间加过，说明未到上报时间
                return FALSE;
            }
        }
        if(stCurrTime.u8Hour == stRefTime.u8Hour)
        {
            if((stCurrTime.u8Day != stLastTime.u8Day))
            {
                //上一次执行的日不一样，说明是新周期
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
        //时间单位为小时
        if(QCSG_TimerSub(&stCurrTime,&stRefTime,au32Result)>0)
        {
            u32PeriodMinute = u8PeriodValue * 60;
            u32IntervalMinute = au32Result[0] * 24 * 60 + au32Result[1] * 60 + au32Result[2];
            if((u32IntervalMinute % u32PeriodMinute) == 0)
            {
                //正好到周期
                return TRUE;
            }
            else if((u32IntervalMinute % u32PeriodMinute) <= 10)
            {
                //防止错过周期，单位为小时的任务，到周期后的10min内，都认为是有效任务执行时间
                if((stCurrTime.u8Hour != stLastTime.u8Hour))
                {
                    //上一次执行的小时不一样，说明是新周期
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
        //时间单位为分
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
                //防止错过周期，单位为分的任务，到周期后的u8PeriodValue-1min内，都认为是有效任务执行时间
                if(QCSG_TimerSub(&stCurrTime,&stLastTime,au32Result)>0)
                {
                    if((au32Result[0] != 0) || (au32Result[1] != 0) ||(au32Result[2] >= u8PeriodValue))
                    {
                        //上次执行时间与当前时间比，大于一个执行周期，说明是新周期
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
*功    能:判断数据标识是否是非抄表数据
*输入参数:u32Ident
*输出参数:无
*返    回:是否是非抄表数据
--------------------------------------------------------------------**/
u8 QCSG_Check_RDIdent(u32 u32Ident)
{
	if((u32Ident == 0x03100000)||(u32Ident == 0x03100100)||(u32Ident == 0x03100200)||(u32Ident == 0x03100300))
	{
		return TRUE;
	}
	//if((u32Ident & 0xFF000000) == 0xE1000000)
	if((u32Ident & 0xFF000000) == 0xE1000000&&u32Ident!=0xE1000008)	//剩余金额不应按统计类处理
	{
		return TRUE;
	}
	return FALSE;
}
/**--------------------------------------------------------------------
*功    能:南网AFN0C、AFN0D数据标识转换为抄读的645数据标识
*输入参数:u32NwIdent，u8ProtocolType
*输出参数:pu8IdentNum，pu32IdentList
*返    回:是否提取成功
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
*功    能:南网AFN0C、AFN0D数据标识转换为抄读的645数据标识
*输入参数:u32NwIdent，u8ProtocolType
*输出参数:pu8IdentNum，pu32IdentList
*返    回:是否提取成功
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
		if(u32NwIdent == 0x00000000)//特殊处理数据标识 否则遍历至数组后未定义的全0空间导致判断存在该标识
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
*功    能:根据645数据标识获取数据长度，主要用于抄表回错误时无效数据的填充,以E1开头的统计数据，在抄表解析中直接处理掉
*输入参数:u32Ident
*输出参数:pu8Len
*返    回:是否提取成功
--------------------------------------------------------------------**/
u8 QCSG_GetDlt645DataLen_By_Ident(u8 u8AFn,u32 u32Ident,u8* pu8Len)
{
	(*pu8Len) = 0;
	if(u8AFn == 0x0D)
	{
		if(((u32Ident & 0xFFFF0000) == 0x05060000) && ((u32Ident & 0x000000FF) != 0x000000FF))
		{
			//日冻结单条数据
			(*pu8Len) = 4;			
		}
		if(((u32Ident & 0xFFFF0000) == 0x05060000) && ((u32Ident & 0x000000FF) == 0x000000FF))
		{
			//日冻结打包数据
			(*pu8Len) = 20;			
		}	
		if(((u32Ident & 0xFFFFFF00) == 0x05060900) && ((u32Ident & 0x000000FF) != 0x000000FF))
		{
			//日冻结正向有功最大需量单条数据
			(*pu8Len) = 8;			
		}
		if(((u32Ident & 0xFFFFFF00) == 0x05060900) && ((u32Ident & 0x000000FF) == 0x000000FF))
		{
			//日冻结正向有功最大需量
			(*pu8Len) = 40;	
		}
		if(((u32Ident & 0xFFFFFF00) == 0x05060A00) && ((u32Ident & 0x000000FF) != 0x000000FF))
		{
			//日冻结反向有功最大需量单条数据
			(*pu8Len) = 8;			
		}
		if(((u32Ident & 0xFFFFFF00) == 0x05060A00) && ((u32Ident & 0x000000FF) == 0x000000FF))
		{
			//日冻结反向有功最大需量
			(*pu8Len) = 40;	
		}				
		if(((u32Ident & 0xFF0000FF) == 0x00000001) && ((u32Ident & 0x0000FF00) != 0x0000FF00))
		{
			//月冻结单条数据
			(*pu8Len) = 4;			
		}
		if(((u32Ident & 0xFF0000FF) == 0x00000001) && ((u32Ident & 0x0000FF00) == 0x0000FF00))
		{
			//月冻结打包条数据
			(*pu8Len) = 20;			
		}
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//电能量单条数据，数据长度为4
			(*pu8Len) = 4;
		}
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//电能量打包数据，数据长度为20
			(*pu8Len) = 20;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//电压单条数据，数据长度为2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//电压打包数据，数据长度为18
			(*pu8Len) = 2 * 9;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//电流单条数据，数据长度为3
			(*pu8Len) = 3;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//电流打包数据，数据长度为18
			(*pu8Len) = 3 * 6;
		}	
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//功率单条数据，数据长度为3
			(*pu8Len) = 3;
		}
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//功率打包数据，数据长度为12
			(*pu8Len) = 3 * 4;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//功率因数单条数据，数据长度为3
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//功率因数打包数据，数据长度为8
			(*pu8Len) = 2 * 4;
		}			
	}
	if(u8AFn == 0x0C)
	{
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//电能量单条数据，数据长度为4
			(*pu8Len) = 4;
		}
		if(((u32Ident & 0xFF0000FF) == 0) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//电能量打包数据，数据长度为20
			(*pu8Len) = 20;
		}
		if(((u32Ident & 0xFF000000) == 0x01000000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//需量单条数据，数据长度为8
			(*pu8Len) = 8;
		}
		if(((u32Ident & 0xFF000000) == 0x01000000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//需量打包数据，数据长度为40
			(*pu8Len) = 40;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//电压单条数据，数据长度为2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02010000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//电压打包数据，数据长度为18
			(*pu8Len) = 2 * 9;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//电流单条数据，数据长度为3
			(*pu8Len) = 3;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02020000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//电流打包数据，数据长度为18
			(*pu8Len) = 3 * 6;
		}	
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//功率单条数据，数据长度为3
			(*pu8Len) = 3;
		}
		if((((u32Ident & 0xFFFF0000) == 0x02030000) || ((u32Ident & 0xFFFF0000) == 0x02040000) || ((u32Ident & 0xFFFF0000) == 0x02050000)) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//功率打包数据，数据长度为12
			(*pu8Len) = 3 * 4;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//功率因数单条数据，数据长度为3
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02060000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//功率因数打包数据，数据长度为8
			(*pu8Len) = 2 * 4;
		}	
		if(((u32Ident & 0xFFFF0000) == 0x02070000) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//相角单条数据，数据长度为2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFF0000) == 0x02070000) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//相角打包数据，数据长度为12
			(*pu8Len) = 2 * 6;
		}		
		if((((u32Ident & 0xFFFF0000) == 0x02080000) || ((u32Ident & 0xFFFF0000) == 0x02090000)) && (u32Ident & 0x0000FF00) != 0x0000FF00)
		{
			//波形失真单条数据，数据长度为2
			(*pu8Len) = 2;
		}
		if((((u32Ident & 0xFFFF0000) == 0x02080000) || ((u32Ident & 0xFFFF0000) == 0x02090000)) && (u32Ident & 0x0000FF00) == 0x0000FF00)
		{
			//波形失真打包数据，数据长度为6
			(*pu8Len) = 2 * 3;
		}
		if((((u32Ident & 0xFFFF0000) == 0x020A0000) || ((u32Ident & 0xFFFF0000) == 0x020B0000)) && (u32Ident & 0x000000FF) != 0x000000FF)
		{
			//谐波单条数据，数据长度为2
			(*pu8Len) = 2;
		}
		if((((u32Ident & 0xFFFF0000) == 0x020A0000) || ((u32Ident & 0xFFFF0000) == 0x020B0000)) && (u32Ident & 0x000000FF) == 0x000000FF)
		{
			//谐波打包数据，数据长度为32
			(*pu8Len) = 2 * 21;
		}
		if(((u32Ident & 0xFFFFFF00) == 0x04000500) && (u32Ident & 0x000000FF) != 0x000000FF)
		{
			//电表运行状态字，数据长度为2
			(*pu8Len) = 2;
		}
		if(((u32Ident & 0xFFFFFF00) == 0x04000500) && (u32Ident & 0x000000FF) == 0x000000FF)
		{
			//电表运行状态字，数据长度为14
			(*pu8Len) = 2 * 7;
		}	
		switch(u32Ident)
		{
			//无规律的数据项
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
*功    能:协议端口到硬件端口的转换函数
*输入参数:u8ChNo
*输出参数:无
*返    回:是否连接
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
*功    能:写终端状态字,写的位置按照上行协议位置来的
*输入参数:u8YXState,u8YKState
*输出参数:无
*返    回:无
--------------------------------------------------------------------**/
void QCSG_WriteTerminal_State(u8 u8YXState,u8 u8YKState)
{
	u8* pState = (u8*)QCSG_TERMINAL_STATE;
	pState[0] = 0x79;
	pState[1] = 0x00;
	//本体门接点
	if((u8YXState & 0x10) == 0)
	{
		pState[0] &= 0x78;
	}
	//四路YX
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
	//四路YK
	u8YKState = (~u8YKState);
	u8YKState &= 0x0F;
	u8YKState <<= 3;
	pState[1] |= u8YKState;
}
/**--------------------------------------------------------------------
*功    能:获取终端状态字
*输入参数:无
*输出参数:pu8YXState,pu8YKState
*返    回:无
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
*功    能:清除当日、当月内存中的统计数据
*输入参数:无
*输出参数:pu8YXState,pu8YKState
*返    回:无
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

	//清除日冻结、月冻结记录全局变量
	MemSet((u8*)QCSG_RECORD_FROZEN_PN_LIST,0,256);
}

/*******************************************************************************************************
以下函数为赵云增加
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
#if ((Project/100)==5) //err20181122 II型集中器 增加曲线数据 信号强度 
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
功     能: 根据输入的数据时间密度取值，增加相应的时间
入口参 数: Time,需要增加的时间，
		   DataDensity，数据密度，当数据密度不合法默认为6，按天增加
返     回: 
编     写: zhaoyun
编写时 间; 2017-7-10
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
功     能: 根据输入的数据时间比较两个时间的大小
入口参 数: AmrEnum，不同的抄表类型，比较的时间范围不一致
		   Time1,Time2: 比较Time1比Time2小、大或者相等
返     回: 
编     写: zhaoyun
编写时 间; 2017-7-10
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
		// 等待完成获取当前最大测量点号，暂用 MAX_PN_NUM 代替
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
功     能: 表号合法性判断
入口参 数: MeterAddr=表号
出口参 数: 无
返     回: 合：法或不合法
调用注 意: 
主要算 法:
编     写: zhaoyun
编写时 间; 2015-11-10
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
			TmpPn = i; //找到最后一个有效测量点
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
	// 年月合法性判断
	if(((Calendar->u8Year & 0x0F) > 0x09) || ((Calendar->u8Year & 0xF0) > 0x90))	//(0x00 - 0x99)
	{
		return ERROR_InvalidData;
	}

	if((Calendar->u8Month == 0x00) || ((Calendar->u8Month & 0x0F) > 0x09) || ((Calendar->u8Month & 0xF0) > 0x10) || (Calendar->u8Month > 0x12))	//(0x01 - 0x12)
	{
		return ERROR_InvalidData;
	}

	// 日合法性判断
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
		if(LeapYearCheck(2000 + (Calendar->u8Year & 0x0F) + ((Calendar->u8Year >> 4) * 10)) != 0)			//闰年判断
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

	// 小时、分钟、秒合法性判断

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
	
	// 星期合法性判断
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
功     能: 计算和校验
入口参 数: *Data,Len
出口参 数: 无
返     回: Result
调用注 意:
主要算 法:
编     写: zhaoyun
编写时 间; 2014-4-10
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
功     能: EDMI红相表帧校验
入口参 数: *buffer,Len
出口参 数: crc
返     回: crc
调用注 意:
主要算 法:
编     写: ybzyl
编写时 间; 2019-05-27
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


//err20190531 IEEE754 浮点数转换算法
//32bit单浮点运算
/****************************************************************************
功     能: EDMI红相表浮点数转换算法32bit 
入口参 数: u32 Float_Value, u32* Output_Value
出口参 数: u32* Output_Value

Output_Value[0] 小数点前数值
Output_Value[1]	小数点后数值

返     回: u8sign_bit(数值符号 0正 1负)
****************************************************************************/
u8 IEEE754_Float_Arithmetic(u32 Float_Value,u32* Output_Value)
{
	//u32 u32Value = 0x4260fbed;
	u8 u8Sigh_bit;//符号 0正 1负
	u8 u8Exponent;//指数，右移位数
	u32 u32Value_Temp;
	u32 u32Value1;//结果小数点前部分
	u32 u32Value2;//结果小数点后部分
	
	u8Sigh_bit = (u8)(Float_Value >> 31);
	
	Float_Value &= 0x7FFFFFFF;//去除符号位
	
	u8Exponent = (u8)(Float_Value >> 23);
	u32Value_Temp = Float_Value & (0x7FFFFF);

	if(u8Exponent>= 127)
	{
		u8Exponent = u8Exponent - 127;	//右移位数
		u32Value1 = (u32Value_Temp >> (23 - u8Exponent)) + (1 << u8Exponent);
		u32Value2 = u32Value_Temp & (0x7FFFFF >> u8Exponent);
		u32Value2 = (u32Value2*100)>>(23-u8Exponent);//保留小数点2位
	}
	else 
	{
		u8Exponent = 127- u8Exponent ;	//右移位数
		u32Value1 = (u32Value_Temp >> (23 + u8Exponent)) + (1 >> u8Exponent);
		u32Value2 = u32Value_Temp | (1 << 23);
		u32Value2 = (u32Value2*100)>>(23+u8Exponent);//保留小数点2位
	}

	//u32Value1 = (u32Value_Temp >> (23 - u8Exponent)) + (1 << u8Exponent);
	//u32Value2 = u32Value_Temp & (0x7FFFFF >> u8Exponent);
	//u32Value2 = (u32Value2*100)>>(23-u8Exponent);//保留小数点2位
	
	Output_Value[0] = u32Value1;
	Output_Value[1] = u32Value2;

	return u8Sigh_bit;
}

/****************************************************************************
功     能: EDMI红相表浮点数转换算法64bit 
入口参 数: u64 Float_Value, u64* Output_Value
出口参 数: u64* Output_Value

Output_Value[0] 小数点前数值
Output_Value[1]	小数点后数值

返     回: u8sign_bit(数值符号 0正 1负)
****************************************************************************/
u8 IEEE754_Double_Arithmetic(u64 Float_Value,u64* Output_Value)
{
	//u32 u32Value = 0x4260fbed;
	u8 u8Sigh_bit;//符号 0正 1负
	u32 u32Exponent;//指数，右移位数
	u64 u64Value_Temp;
	u64 u64Value1;//结果小数点前部分
	u64 u64Value2;//结果小数点后部分
	
	u8Sigh_bit = (u8)(Float_Value >> 63);
	
	Float_Value &= 0x7FFFFFFFFFFFFFFF;//去除符号位
	
	u32Exponent = (u32)(Float_Value >> 52);
	u64Value_Temp = Float_Value & (0xFFFFFFFFFFFFF);

	if(u32Exponent>= 1023)
	{
		u32Exponent = u32Exponent - 1023;	//右移位数
		u64Value1 = (u64Value_Temp >> (52 - u32Exponent)) + (1 << u32Exponent);
		u64Value2 = u64Value_Temp & (0xFFFFFFFFFFFFF >> u32Exponent);
		u64Value2 = (u64Value2*100)>>(52-u32Exponent);//保留小数点2位
	}
	else 
	{
		u32Exponent = 1023 - u32Exponent;	//右移位负数
		u64Value1 = (u64Value_Temp >> (52 + u32Exponent)) + (1 >> u32Exponent);
		u64Value2 = u64Value_Temp | ((u64)1 << 52);//& (0xFFFFFFFFFFFFF << u32Exponent);
		u64Value2 = (u64Value2*100)>>(52+u32Exponent);//保留小数点2位
	}

	//u64Value1 = (u64Value_Temp >> (52 - u32Exponent)) + (1 << u32Exponent);
	//u64Value2 = u64Value_Temp & (0xFFFFFFFFFFFFF >> u32Exponent);
	
	Output_Value[0] = u64Value1;
	Output_Value[1] = u64Value2;

	return u8Sigh_bit;
}


//err20190531 对EDMI红相表帧格式进行预处理
//扫描碰到0x10 将后一个数减去0x40的值填入。

/****************************************************************************
功     能: EDMI红相表帧特殊符号处理 
入口参 数: u8* pu8InData, u8* pu8DataLen
出口参 数: u8* pu8InData, u8* pu8DataLen

pu8InData:接收到的EDMI帧
pu8DataLenn:接收到的帧长度
//函数执行完 pu8InData就变成转换完的数据,pu8DataLen为转换后长度
返     回: 成功或失败
****************************************************************************/
u8 QCSG_GetEDMIRealFrame(u8* pu8InData,u8* pu8DataLen)
{
	u8 i=0;
	u8 k=0;
	u8 Temp_num = 0;//运算次数
	u8 Len = (*pu8DataLen);//最后的长度
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
				WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
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
				WWDT_Enable_Feed(WDTTimerOutVal);//允许看门狗和喂狗;最大TimerOutMS=(0xfff*256)/32=32768ms
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
{                                                     //CRC交验
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
//CRC16_XMODEM 初始值0x0000 结果异或值0x0000 输入反转False 输出反转False
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

// 初始值 0x0000 结果异或值 0x0000 时 为 CCITT 此时输入反转True  输出反转True
// 初始值 0xFFFF 结果异或值 0xFFFF 时 为 X25   此时输入反转True  输出反转True
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
//8位字符 按位反转
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
//16位字符 按位反转
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
功     能: 根据输入的串口控制参数得到串口初始化控制字
入口参 数: BaudRateCode: 根据波特率数组，按照下标获取，注意输入参数是南网标准baudrate/300
		   需要先根据南网的取值获得实际波特率，然后转换成王总格式对应的控制字。
		   0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,
		   10=115200,11=230400
		   ParityCode: 0：无校验；1：偶校验；2：奇校验 
		   DataBitCode: 数据位，5,6,7,8
		   StopBitCode: 停止位，0：1位；1：1.5位； 2：2位
返     回: 转换出的王总串口初始化控制字
		   D8-D5位表示波特率,O～11依次表示0=300,1=600,2=1200,3=2400,4=4800,
		   5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400
		   D4位表示停止位位数0/1分别表示1/2个停止位
		   D3位表示有无校验位0/1分别表示无/有
		   D2位表示偶／奇校验位0/1分别表示偶／奇校验
		   D1-D0 数据位数0～3分别表示5-8位
编     写: zhaoyun
编写时 间; 2017-7-10
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
	{// 未找到存在的波特率，则默认为2400
		i = 3;			
	}
	BspCtrl |= (i << 5);

	if(StopBitCode == 2)
	{
		BspCtrl |= 0x10;
	}

	if(ParityCode == 0)
	{// 无校验
	
	}
	else if(ParityCode == 2)
	{// 奇校验 
		BspCtrl |= 0x0C;
	}
	else
	{// 偶校验
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
	AlarmSaveParaStruct AlarmSavePara;//err20190216 修改掉电存储标志的存储位置

	if((u8)ResetType == 0x00)//所有可写参数恢复出厂设置
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
		pTSysCtrl->GPRSRxTxBytes = 0;//err20180404-20 通信流量清空
		MC(0, ADDR_RS485_CH1_AMR_TaskCtrl, LEN_RS485_CH1_AMR_TaskCtrl * 4);

		
	}
	if((u8)ResetType == 0x01)//数据区复位
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
		pTSysCtrl->GPRSRxTxBytes = 0;//err20180404-20 通信流量清空
		MC(0, ADDR_RS485_CH1_AMR_TaskCtrl, LEN_RS485_CH1_AMR_TaskCtrl * 4);

		//数据初始化同时初始化掉电存储的告警标志		
		MR((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
		
		AlarmSavePara.AlarmDayReportFlag = 0;//err20180710 每日上报一次的告警标识，需掉电存储
		for(i = 0; i < SIZE_128; i++)//err20190109 清空时钟电池电压低、电能表时钟异常告警标志
		{
			AlarmSavePara.ClkBatLowVolAlarmFLagList[i] = 0x00;
			AlarmSavePara.TimeFaultAlarmFLagList[i] = 0x00;
		}
		MW((u32)&AlarmSavePara, ADDR_AlarmSavePara, sizeof(AlarmSaveParaStruct));
	}
	//if((u8)ResetType == 0x02)//硬件复位
	{
		pCommRam->RESET = 0x55;					//0x55=重启动(立即使用看门狗复位)
   		pCommRam->ACTION_RESET_PORTn = PORTn;	//操作复位的通信端口号 
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

//从RAM中通过表地址读取测量点号 
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

//从FLASH中通过表地址读取测量点号 
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
		   (MeasurePoint.State == 0x01))//配变 地址匹配且为有效则为找到
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
 功     能: 组645帧
 入口参 数: DLT645FrameStruct* SrcFrame 
 出口参 数: u8* FrameBuf
 返     回: 645帧长度
 调用注 意:
 主要算 法:
 编     写: zhaoyun
 编写时 间; 2015-11-10
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
	MW((u32)&DefaultSelfAdaptionPara, ADDR_SelfAdaptionPara, sizeof(SelfAdaptionParaStruct));//err20190218 增加自适应参数初始化

#if ((Project/100 == 3) && (USER == 3400))//err20190528 增加云南负控GPRS自适应参数
	MW((u32)&Default_E0000111_CommunicationParaStruct, ADDR_CommunicationPara_E0000111, sizeof(E000011x_CommunicationParaStruct));//err20190218 增加自适应参数初始化
	MW((u32)&Default_E0000112_CommunicationParaStruct, ADDR_CommunicationPara_E0000112, sizeof(E000011x_CommunicationParaStruct));//err20190218 增加自适应参数初始化
	MW((u32)&Default_E0000113_CommunicationParaStruct, ADDR_CommunicationPara_E0000113, sizeof(E000011x_CommunicationParaStruct));//err20190218 增加自适应参数初始化

#endif


	MR((u32)&Mp, ADDR_MP_START, sizeof(MeasurePointStruct));

	//err20190218 初始化同时初始化掉电存储的告警标志
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


//err20181112 增加告警 CT二次开路短路 ADC3采集函数
#define VREF    250//ADC参与电压x.xxV


#ifdef IC_STM32F4xx
u32 Get_ADC3(u32 Channel,u32 mul,u32 div,u32 ADDR_RAM)//ADC;入口:Channel=ADC3通道号,mul=乘数,div=除数,ADDR_RAM=结果存放地址; 返回:测量值(0xffffffff表示转换失败)
{
	u32 i;
	u32 t;
	u32 ReNum;
	u16 *p16;

//允许时钟
	RCC->APB2ENR|=1<<10;
//复位
	RCC->APB2RSTR=1<<8;
	RCC->APB2RSTR=0;
//分频
	ADC->CCR=0|//位 31:24 保留，必须保持复位值。
		 (0<<23)|//位 23 TSVREFE：温度传感器和 VREFINT 使能 (Temperature sensor and VREFINT enable)通过软件将该位置 1 和清零可使能/禁止温度传感器和 VREFINT 通道。
			//0：禁止温度传感器和 VREFINT 通道
			//1：使能温度传感器和 VREFINT 通道
			//注意： 对于 STM32F42x 和 STM32F43x 器件，当 TSVREFE 位置 1 时 必须禁止 VBATE。两个位同时置 1 时，仅进行 VBAT 转换。
		 (1<<22)|//位 22 VBATE：VBAT 使能 (VBAT enable)通过软件将该位置 1 和清零可使能/禁止 VBAT 通道。
			//0：禁止 VBAT 通道
			//1：使能 VBAT 通道
		//位 21:18 保留，必须保持复位值。
		 (0<<17)|//位 17:16 ADCPRE：ADC 预分频器 (ADC prescaler)由软件置 1 和清零，以选择 ADC 的时钟频率。该时钟为所有 ADC 所共用。注意:
			//00：PCLK2 2 分频
			//01：PCLK2 4 分频
			//10：PCLK2 6 分频
			//11：PCLK2 8 分频
		//位 15:14 DMA：直接存储器访问模式（对于多个 ADC 模式）(Direct memory access mode for multi ADC mode)此位由软件置 1 和清零。有关详细信息，请参见 DMA 控制器一节。
			//00：禁止 DMA 模式
			//01：使能 DMA 模式 1（依次 2/3 半字 - 1、2、3 依次进行）
			//10：使能 DMA 模式 2（成对 2/3 半字 - 2 和 1、1 和 3、3 和 2 依次进行）
			//11：使能 DMA 模式 3（成对 2/3 字节 - 2 和 1、1 和 3、3 和 2 依次进行）
		//位 13 DDS：DMA 禁止选择（对于多个 ADC 模式）(DMA disable selection (for multi-ADC mode))此位由软件置 1 和清零。
			//0：最后一次传输后不发出新的 DMA 请求（在 DMA 控制器中进行配置）DMA 位不通过硬件清零，但必须在生成新的 DMA 请求前，通过软件清零并设置为需要的模式。
			//1：只要数据发生转换且 DMA = 01、10 或 11，便会发出 DMA 请求。
		//位 12 保留，必须保持复位值。
		//位 11:8 DELAY：2 个采样阶段之间的延迟 (Delay between 2 sampling phases)由软件置 1 和清零。这些位在双重或三重交错模式下使用。
			//0000：5 * TADCCLK
			//0001：6 * TADCCLK
			//0010：7 * TADCCLK
			//...
			//1111：20 * TADCCLK
		//位 7:5 保留，必须保持复位值。
		0;//位 4:0 MULTI[4:0]：多重 ADC 模式选择 (Multi ADC mode selection)通过软件写入这些位可选择操作模式。
			//— 所有 ADC 均独立：
			//00000：独立模式
			//— 00001 到 01001：双重模式，ADC1 和 ADC2 一起工作，ADC3 独立
			//00001：规则同时 + 注入同时组合模式
			//00010：规则同时 + 交替触发组合模式
			//00011：Reserved?
			//00101：仅注入同时模式
			//00110：仅规则同时模式
			//仅交错模式
			//01001：仅交替触发模式
			//— 10001 到 11001：三重模式：ADC1、ADC2 和 ADC3 一起工作
			//10001：规则同时 + 注入同时组合模式
			//10010：规则同时 + 交替触发组合模式
			//10011：Reserved?
			//10101：仅注入同时模式
			//10110：仅规则同时模式
			//仅交错模式
			//11001：仅交替触发模式
			//其它所有组合均需保留且不允许编程
			//注意： 在多重模式下，更改通道配置会生成中止，进而导致同步丢失。建议在更改配置前禁用多重 ADC 模式。
	ADC3->SQR3=Channel;
			//位 31:30 保留，必须保持复位值。
			//位 29:25 SQ6[4:0]：规则序列中的第六次转换 (6th conversion in regular sequence)通过软件写入这些位，并将通道编号 (0..18) 分配为序列中的第六次转换。
			//位 24:20 SQ5[4:0]：规则序列中的第五次转换 (5th conversion in regular sequence)
			//位 19:15 SQ4[4:0]：规则序列中的第四次转换 (4th conversion in regular sequence)
			//位 14:10 SQ3[4:0]：规则序列中的第三次转换 (3rd conversion in regular sequence)
			//位 9:5 SQ2[4:0]：规则序列中的第二次转换 (2nd conversion in regular sequence)
			//位 4:0 SQ1[4:0]：规则序列中的第一次转换 (1st conversion in regular sequence)
	ADC3->SQR2=0;
			//位 31:30 保留，必须保持复位值。
			//位 29:26 SQ12[4:0]：规则序列中的第十二次转换 (12th conversion in regular sequence)通过软件写入这些位，并将通道编号 (0..18) 分配为序列中的第十二次转换。
			//位 24:20 SQ11[4:0]：规则序列中的第十一次转换 (11th conversion in regular sequence)
			//位 19:15 SQ10[4:0]：规则序列中的第十次转换 (10th conversion in regular sequence)
			//位 14:10 SQ9[4:0]：规则序列中的第九次转换 (9th conversion in regular sequence)
			//位 9:5 SQ8[4:0]：规则序列中的第八次转换 (8th conversion in regular sequence)
			//位 4:0 SQ7[4:0]：规则序列中的第七次转换 (7th conversion in regular sequence)
	ADC3->SQR1=0;
			//位 31:24 保留，必须保持复位值。
			//位 23:20 L[3:0]：规则通道序列长度 (Regular channel sequence length)通过软件写入这些位可定义规则通道转换序列中的转换总数。
				//0000：1 次转换
				//0001：2 次转换
				//...?
				//1111：16 次转换
			//位 19:15 SQ16[4:0]：规则序列中的第十六次转换 (16th conversion in regular sequence)通过软件写入这些位，并将通道编号 (0..18) 分配为转换序列中的第十六次转换。
			//位 14:10 SQ15[4:0]：规则序列中的第十五次转换 (15th conversion in regular sequence)
			//位 9:5 SQ14[4:0]：规则序列中的第十四次转换 (14th conversion in regular sequence)
			//位 4:0 SQ13[4:0]：规则序列中的第十三次转换 (13th conversion in regular sequence)
		
	ADC3->CR1=0;
			//位 31:27 保留，必须保持复位值。
			//位 26 OVRIE：溢出中断使能 (Overrun interrupt enable)通过软件将该位置 1 和清零可使能/禁止溢出中断。
				//0：禁止溢出中断
				//1：使能溢出中断。OVR 位置 1 时产生中断。
			//位 25:24 RES[1:0]：分辨率 (Resolution)通过软件写入这些位可选择转换的分辨率。
				//00：12 位（15 ADCCLK 周期）
				//01：10 位（13 ADCCLK 周期）
				//10：8 位（11 ADCCLK 周期）
				//11：6 位（9 ADCCLK 周期）
			//位 23 AWDEN：规则通道上的模拟看门狗使能 (Analog watchdog enable on regular channels)此位由软件置 1 和清零。
				//0：在规则通道上禁止模拟看门狗
				//1：在规则通道上使能模拟看门狗
			//位 22 JAWDEN：注入通道上的模拟看门狗使能 (Analog watchdog enable on injected channels)此位由软件置 1 和清零。
				//0：在注入通道上禁止模拟看门狗
				//1：在注入通道上使能模拟看门狗
			//位 21:16 保留，必须保持复位值。
			//位 15:13 DISCNUM[2:0]：不连续采样模式通道计数 (Discontinuous mode channel count)软件将写入这些位，用于定义在接收到外部触发后于不连续采样模式下转换的规则通道数。
				//000：1 个通道
				//001：2 个通道
				//...?
				//111：8 个通道
			//位 12 JDISCEN：注入通道的不连续采样模式 (Discontinuous mode on injected channels)通过软件将该位置 1 和清零可使能/禁止注入通道的不连续采样模式。
				//0：禁止注入通道的不连续采样模式
				//1：使能注入通道的不连续采样模式
			//位 11 DISCEN：规则通道的不连续采样模式 (Discontinuous mode on regular channels)通过软件将该位置 1 和清零可使能/禁止规则通道的不连续采样模式。
				//0：禁止规则通道的不连续采样模式
				//1：使能规则通道的不连续采样模式
			//位 10 JAUTO：注入组自动转换 (Automatic injected group conversion)通过软件将该位置 1 和清零可在规则组转换后分别使能/禁止注入组自动转换。
				//0：禁止注入组自动转换
				//1：使能注入组自动转换
			//位 9 AWDSGL：在扫描模式下使能单一通道上的看门狗 (Enable the watchdog on a single channelin scan mode)通过软件将该位置 1 和清零可分别使能/禁止通过 AWDCH[4:0] 位确定的通道上的模拟看门狗。
				//0：在所有通道上使能模拟看门狗
				//1：在单一通道上使能模拟看门狗
			//位 8 SCAN：扫描模式 (Scan mode)通过软件将该位置 1 和清零可使能/禁止扫描模式。在扫描模式下，转换通过 ADC_SQRx 或ADC_JSQRx 寄存器选择的输入。
				//0：禁止扫描模式
				//1：使能扫描模式
				//注意： EOCIE 位置 1 时将生成 EOC 中断：
				//— 如果 EOCS 位清零，在每个规则组序列转换结束时
				//— 如果 EOCS 位置 1，在每个规则通道转换结束时
				//注意： JEOCIE 位置 1 时，JEOC 中断仅在最后一个通道转换结束时生成。
			//位 7 JEOCIE：注入通道的中断使能 (Interrupt enable for injected channels)通过软件将该位置 1 和清零可使能/禁止注入通道的转换结束中断。
				//0：禁止 JEOC 中断
				//1：使能 JEOC 中断。JEOC 位置 1 时产生中断。
			//位 6 AWDIE：模拟看门狗中断使能 (Analog watchdog interrupt enable)通过软件将该位置 1 和清零可使能/禁止模拟看门狗中断。
				//0：禁止模拟看门狗中断
				//1：使能模拟看门狗中断
			//位 5 EOCIE：EOC 中断使能 (Interrupt enable for EOC)通过软件将该位置 1 和清零可使能/禁止转换结束中断。
				//0：禁止 EOC 中断
				//1：使能 EOC 中断EOC 位置 1 时产生中断。
			//位 4:0 AWDCH[4:0]：模拟看门狗通道选择位 (Analog watchdog channel select bits)这些位将由软件置 1 和清零。它们用于选择由模拟看门狗监控的输入通道。
				//注意： 
				//00000：ADC 模拟输入通道 0
				//00001：ADC 模拟输入通道 1
				//...?
				//01111：ADC 模拟输入通道 15
				//10000：ADC 模拟输入通道 16
				//10001：ADC 模拟输入通道 17
				//10010：ADC 模拟输入通道 18
				//保留其它值
	ADC3->CR2=0|
			//位 31 保留，必须保持复位值。
			//位 30 SWSTART：开始转换规则通道 (Start conversion of regular channels)通过软件将该位置 1 可开始转换，而硬件会在转换开始后将该位清零。
				//0：复位状态
				//1：开始转换规则通道
				//注意： 该位只能在 ADON = 1 时置 1，否则不会启动转换。
			//位 29:28 EXTEN：规则通道的外部触发使能 (External trigger enable for regular channels)通过软件将这些位置 1 和清零可选择外部触发极性和使能规则组的触发。
				//00：禁止触发检测
				//01：上升沿上的触发检测
				//10：下降沿上的触发检测
				//11：上升沿和下降沿上的触发检测
			//位 27:24 EXTSEL[3:0]：为规则组选择外部事件 (External event select for regular group)这些位可选择用于触发规则组转换的外部事件。
				//0000：定时器 1 CC1 事件
				//0001：定时器 1 CC2 事件
				//0010：定时器 1 CC3 事件
				//0011：定时器 2 CC2 事件
				//0100：定时器 2 CC3 事件
				//0101：定时器 2 CC4 事件
				//0110：定时器 2 TRGO 事件
				//0111：定时器 3 CC1 事件
				//1000：定时器 3 TRGO 事件
				//1001：定时器 4 CC4 事件
				//1010：定时器 5 CC1 事件
				//1011：定时器 5 CC2 事件
				//1100：定时器 5 CC3 事件
				//1101：定时器 8 CC1 事件
				//1110：定时器 8 TRGO 事件
				//1111：EXTI 线 11
			//位 23 保留，必须保持复位值。
			//位 22 JSWSTART：开始转换注入通道 (Start conversion of injected channels)转换开始后，软件将该位置 1，而硬件将该位清零。
				//0：复位状态
				//1：开始转换注入通道
				//注意： 该位只能在 ADON = 1 时置 1，否则不会启动转换。
			//位 21:20 JEXTEN：注入通道的外部触发使能 (External trigger enable for injected channels)通过软件将这些位置 1 和清零可选择外部触发极性和使能注入组的触发。
				//00：禁止触发检测
				//01：上升沿上的触发检测
				//10：下降沿上的触发检测
				//11：上升沿和下降沿上的触发检测
			//位 19:16 JEXTSEL[3:0]：为注入组选择外部事件 (External event select for injected group)这些位可选择用于触发注入组转换的外部事件。
				//0000：定时器 1 CC4 事件
				//0001：定时器 1 TRGO 事件
				//0010：定时器 2 CC1 事件
				//0011：定时器 2 TRGO 事件
				//0100：定时器 3 CC2 事件
				//0101：定时器 3 CC4 事件
				//0110：定时器 4 CC1 事件
				//0111：定时器 4 CC2 事件
				//1000：定时器 4 CC3 事件
				//1001：定时器 4 TRGO 事件
				//1010：定时器 5 CC4 事件
				//1011：定时器 5 TRGO 事件
				//1100：定时器 8 CC2 事件
				//1101：定时器 8 CC3 事件
				//1110：定时器 8 CC4 事件
				//1111：EXTI 线 15
			//位 15:12 保留，必须保持复位值。
			(0<<11)|//位 11 ALIGN：数据对齐 (Data alignment)此位由软件置 1 和清零。请参见图 38 和图 39。
				//0：右对齐
				//1：左对齐
			(0<<10)|//位 10 EOCS：结束转换选择 (End of conversion selection)此位由软件置 1 和清零。
				//0：在每个规则转换序列结束时将 EOC 位置 1。溢出检测仅在 DMA=1 时使能。
				//1：在每个规则转换结束时将 EOC 位置 1。使能溢出检测。
			//位 9 DDS：DMA 禁止选择（对于单一 ADC 模式） (DMA disable selection (for single ADC mode))此位由软件置 1 和清零。
				//0：最后一次传输后不发出新的 DMA 请求（在 DMA 控制器中进行配置）
				//1：只要发生数据转换且 DMA = 1，便会发出 DAM 请求
			//位 8 DMA：直接存储器访问模式（对于单一 ADC 模式）(Direct memory access mode (for single ADC mode))此位由软件置 1 和清零。有关详细信息，请参见 DMA 控制器一章。
				//0：禁止 DMA 模式
				//1：使能 DMA 模式
			//位 7:2 保留，必须保持复位值。
			//位 1 CONT：连续转换 (Continuous conversion)此位由软件置 1 和清零。该位置 1 时，转换将持续进行，直到该位清零。
				//0：单次转换模式
				//1：连续转换模式
			1;//位 0 ADON：A/D 转换器开启 / 关闭 (A/D Converter ON / OFF)此位由软件置 1 和清零。注意： 
				//0：禁止 ADC 转换并转至掉电模式
				//1：使能 ADC
#define SMPx     7
	ADC3->SMPR2=SMPx+(SMPx<<3)+(SMPx<<6)+(SMPx<<9)+(SMPx<<12)+(SMPx<15)+(SMPx<<18)+(SMPx<<21)+(SMPx<<24)+(SMPx<<27);//SMP0-SMP9
			//位 31:30 保留，必须保持复位值。
			//位 29:0 SMPx[2:0]：通道 X 采样时间选择 (Channel x sampling time selection)通过软件写入这些位可分别为各个通道选择采样时间。在采样周期期间，通道选择位必须保持不变。
				//000：3 个周期
				//001：15 个周期
				//010：28 个周期
				//011：56 个周期
				//100：84 个周期
				//101：112 个周期
				//110：144 个周期
				//111：480 个周期
	ADC3->SMPR1=SMPx+(SMPx<<3)+(SMPx<<6)+(SMPx<<9)+(SMPx<<12)+(SMPx<15)+(SMPx<<18)+(SMPx<<21)+(SMPx<<24);//SMP10-SMP18
			//位 31:27 保留，必须保持复位值。
			//位 26:0 SMPx[2:0]：通道 X 采样时间选择 (Channel x sampling time selection)通过软件写入这些位可分别为各个通道选择采样时间。在采样周期期间，通道选择位必须保持不变。
				//000：3 个周期
				//001：15 个周期
				//010：28 个周期
				//011：56 个周期
				//100：84 个周期
				//101：112 个周期
				//110：144 个周期
				//111：480 个周期
				
	i=0;
	ReNum=4;
	while(ReNum--)
	{
//		PWR->CR|=(1<<13);//ADCDC1=1
//		FLASH->ACR&=~(1<<8);//关闭预取
		ADC3->CR2|=(1<<30);//开始转换规则通道
		t=0xA410;//t=RCC_GetHCLKFreq()/4000;//1ms至少延时
		while(1)//位 1 EOC 规则组通道转换结束后，硬件将该位置 1。通过软件或通过读取 ADC_DR 寄存器将该位清零
		{
			if(ADC3->SR&0x2)
			{
				break;
			}
			if(Comm_Ram->IntFlags&4)//B2=中断中使用ADC冲突标志
			{
				Comm_Ram->IntFlags=0;
				return 0xffffffff;//中断调用ADC转换后已关闭了ADC
			}
			t--;
			if(t==0)
			{//ADC死机
				return 0xffffffff;
			}
		}
//		FLASH->ACR|=(1<<8);//使能预取
//		PWR->CR&=~(1<<13);//ADCDC1=0
		i+=ADC3->DR;
	}
//关闭ADC
	ADC3->CR2=0;//ADON 位清零来停止转换并使 ADC 进入掉电模式。在此模式下，ADC 几乎不耗电 （只有几 μA）	
	RCC->APB2ENR&=~(1<<10);
//计算
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
