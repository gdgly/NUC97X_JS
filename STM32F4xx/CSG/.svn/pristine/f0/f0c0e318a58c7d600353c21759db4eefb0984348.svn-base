

//显示
#ifndef Display_H
#define Display_H
#ifdef __cplusplus
 extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayClr
// Descriptions:        清全屏显示
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayClr(void);//清屏


//-------------------------------------------------------------------------------------------------
// Function name:       DisplayClrRow
// Descriptions:        清屏幕显示的指定行
// input parameters:    开始行号,结束行号(行号0-9(160*160),0-14(320*240))
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayClrRow(u32 StartRow,u32 EndRow);//清屏幕显示的指定行



void DisplayStringAutoUp(u32 StartRow,u32 EndRow,u8 * pStr);//在指定窗口内自动向上平移后显示字符串;出口:修改下一显示行并寄存在ADDR_AutoRowCol

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString
// Descriptions:        显示字符串
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//显示字符串

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString
// Descriptions:        显示字符串
// input parameters:    屏幕行(0-15),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//						注意使用时最好不要显示顶图标(0行)与底图标(9行)
//-------------------------------------------------------------------------------------------------
u32 DisplayStringOutoffScreenLine(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//显示字符串

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyStringOnlyOneRow
// Descriptions:        显示字符串仅在1行内
// input parameters:    屏幕行(0-9),屏幕列(0-19),字符串指针(\n,\r,0均为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B31填入半角字符数
//-------------------------------------------------------------------------------------------------
u32 DispalyStringOnlyOneRow(u32 Row,u32 Col,u8 * pStr);//显示字符串仅在1行内



u16 * DispalyStringContinue(u16 *pStrBuff,u8 * pStr);//显示字符串继续


//---------------------------------------------------------------------------------------------------------
// Function name:       DispalyAlign
// Descriptions:        显示对齐(1左对齐,2居中,3右对齐)
// input parameters:    Row需对齐的行,AlignCol表示对齐到列号(160*160屏的列号0-19,仅用于左对齐,右对齐),Align对齐方式:1=左对齐,2=居中,3=右对齐
// output parameters:   显示对齐(对齐后的溢出部份丢弃)
// Returned value:      无
//---------------------------------------------------------------------------------------------------------
void DispalyAlign(u32 Row,u32 AlignCol,u32 Align);//行显示对齐(1左对齐,2居中,3右对齐)


//---------------------------------------------------------------------------------------------------------
// Function name:       DisplayData
// Descriptions:        数显示
// input parameters:    Num要显BCD数,BitNum显示总位数,DpNum小数位数,p16字符显示缓冲指针
// output parameters:   ASCII字库显示
// Returned value:      下字符显示缓冲指针
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData(u64 Num,u32 BitNum,u32 DpNum,u16 * p16);//数据显示,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF


//---------------------------------------------------------------------------------------------------------
// Function name:       DisplayData_OffZero
// Descriptions:        数显示
// input parameters:    Num要显BCD数,BitNum显示总位数,DpNum小数位数,p16字符显示缓冲指针
// output parameters:   ASCII字库显示,不显无效高位的0
// Returned value:      下字符显示缓冲指针
//---------------------------------------------------------------------------------------------------------
u16 * DisplayData_OffZero(u64 Num,u32 BitNum,u32 DpNum,u16 * p16);//仅显示有效位数据,入口指定要显BCD位数,小点数插入位等,使用ADDR_MEMRWREGBUFF


//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenuString
// Descriptions:        显示菜单字符串
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//显示菜单字符串
u32 DisplayMenuString_CSG(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//显示菜单字符串

void DisplayScrollUp(u32 Row);//显示上滚除第0行外,入口上滚行数




u16 * DisplayData1(u64 Num1,u32 BitNum,u32 DpNum,u8 Num2,u16 * p16)	;//Num2里放置". : -等符号"
void DisplayRam(u32 Ln,u32 RamAddr);//显示中RAM数据(调试用)


void DisplayRATE(u32 Ln,u32 Rate);//显示进度;入口:Ln=显示行,Rate=进度(hex,0-100%)

void Startdisplay(void);			//起动显示
#if ((Project/100)==2)
void NWJZQDisplayDefaultInterface(void);//南网集中器默认界面显示
#endif


#ifdef __cplusplus
 }
#endif 
#endif
