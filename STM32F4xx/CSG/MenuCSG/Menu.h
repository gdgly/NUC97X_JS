

#ifndef DisplayMenu_H
#define DisplayMenu_H

#define INVALID_DATA 0xEEEEEEEE
#define PWD			0x101010
#define PWD_1			0x123456

#include "../QCSG_Head/Chint_Type.h"


#define DISP_BUF_MAX_LINE		(16)
#define DISP_SCROLL_UP			(0)
#define DISP_SCROLL_DOWN		(1)

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenu
// Descriptions:        显示菜单
// input parameters:    菜单上移行数,字符串指针(\n换下行,\r回列0,0结束)
// output parameters:   填入显示缓冲区等
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayMenu(u32 MoveLn,u8 * pStr);//显示菜单
void DisplayMenu_CSG(u32 MoveLn,u8 * pStr);//显示菜单

void DisplayMenu_Scroll(u32 Direction, u32 OutLine);

//-------------------------------------------------------------------------------------------------
// Function name:       MenuCreate
// Descriptions:        创建下级菜单
// input parameters:    下级菜单函数地址
// output parameters:   初始化下级菜单任务计数等数据
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuCreate(u32 FunctionAddr);//进入下级菜单

//-------------------------------------------------------------------------------------------------
// Function name:       MenuDel
// Descriptions:        返回上级菜单
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuDel(void);//返回上级菜单

//-------------------------------------------------------------------------------------------------
// Function name:       MenuNegative
// Descriptions:        反显菜单行
// input parameters:    行号0-9
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuNegative(u32 Ln);//反显菜单行

//-------------------------------------------------------------------------------------------------
// Function name:       MenuSelect
// Descriptions:        菜单选择
// input parameters:    无
// output parameters:   修改Stack->MenuStack[MenuCount->Count].EditRow
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void MenuSelect(void);//菜单选择

//-------------------------------------------------------------------------------------------------
// Function name:       UpArrow
// Descriptions:        ↑
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void UpArrow(void);

//-------------------------------------------------------------------------------------------------
// Function name:       DownArrow
// Descriptions:        ↓
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void DownArrow(void);

//-------------------------------------------------------------------------------------------------
// Function name:       LeftArrow
// Descriptions:        ←
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void LeftArrow(void);

//-------------------------------------------------------------------------------------------------
// Function name:       RightArrow
// Descriptions:        →
// input parameters:    无
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120221
//-------------------------------------------------------------------------------------------------
void RightArrow(void);

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
void Arrowhead(void);//当有下一屏时，屏幕右下角显示向下的箭头当有上一屏时，屏幕右上角显示向上的箭头

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeRowCol
// Descriptions:        反显指定的行和列
// input parameters:    行号0-9，列号0-19
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120213
//-------------------------------------------------------------------------------------------------
void NegativeRowCol(u32 Row,u32 Col);//与字符串号有关，菜单用

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnCol
// Descriptions:        反显指定的行和列
// input parameters:    行号0-9，列号0-19
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120217
//-------------------------------------------------------------------------------------------------
void NegativeLnCol(u32 Ln,u32 Col);

//-------------------------------------------------------------------------------------------------
// Function name:       Select
// Descriptions:        左右选择
// input parameters:    字符串右边界，字符串左边界
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120213
// Date Modified：		20120301
//-------------------------------------------------------------------------------------------------
void Select(u32 MaxCol,u32 MinCol);

//-------------------------------------------------------------------------------------------------
// Function name:       SelectH
// Descriptions:        十六进制数左右选择
// input parameters:    字符串右边界，字符串左边界
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120427
//-------------------------------------------------------------------------------------------------
void SelectH(u32 MaxCol,u32 MinCol);

//-------------------------------------------------------------------------------------------------
// Function name:       CreateScreen
// Descriptions:        翻屏
// input parameters:    下一屏函数地址
// output parameters:   初始化下级菜单任务计数等数据
// Returned value:      无
// Author:              宋玉梅
// Date:                20120220
//-------------------------------------------------------------------------------------------------
void CreateScreen(u32 FunctionAddr);//翻屏

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnMaxColMinCol
// Descriptions:        反显除了MinCol~MaxCol以外的列
// input parameters:    行号0-9，列号
// output parameters:   无
// Returned value:      无
// Author:              宋玉梅
// Date:                20120310
//-------------------------------------------------------------------------------------------------
void NegativeLnMaxColMinCol(u32 Row,u32 MaxCol,u32 MinCol);//与字符串号有关

//-------------------------------------------------------------------------------------------------
// Function name:       NegativeLnMaxColMinColN
// Descriptions:        反显除了MinCol~MaxCol以外的列
// input parameters:    行号0-9，列号
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void NegativeLnMaxColMinColN(u32 Row,u32 MaxCol,u32 MinCol);//与字符串号无关

//-------------------------------------------------------------------------------------------------
// Function name:       BoxSelect
// Descriptions:        编辑框选择
// input parameters:    无
// output parameters:   修改Stack->MenuStack[MenuCount->Count].EditRow
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void BoxSelect(void);//编辑框选择
void BoxSelectOutofScreenLine(void);//编辑框选择

//-------------------------------------------------------------------------------------------------
// Function name:       KeyboardSelect
// Descriptions:        键盘选择
// input parameters:    软键盘最小行号MinLn，软键盘最大行号MaxLn，软键盘的最后一行最大列号LastLnMaxCol，软键盘最小列号MinCol，软键盘最大列号MaxCol，编辑框行号EditLn，编辑框起始列号EditCol
// output parameters:   
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void KeyboardSelect(u32 MinLn,u32 MaxLn,u32 LastLnMaxCol,u32 MinCol,u32 MaxCol,u32 EditLn,u32 EditCol);//键盘选择

//-------------------------------------------------------------------------------------------------
// Function name:      bcd_p8       
// Descriptions:       将十进制数x放到显示缓冲区*p8中        
// input parameters:   显示缓冲区*p8,十进制数x,显示的位数z    
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void bcd_p8(u8 *p8,u64 x,u32 z);

//-------------------------------------------------------------------------------------------------
// Function name:      bcd_p16       
// Descriptions:       将十进制数x放到显示缓冲区*p16中        
// input parameters:   显示缓冲区*p16,十进制数x,显示的位数z    
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void bcd_p16(u16 *p16,u64 x,u32 z);

//-------------------------------------------------------------------------------------------------
// Function name:      p8_bcd       
// Descriptions:       将显示缓冲区*p8中的数转化为十进制数x               
// input parameters:   显示缓冲区*p8，转化后的位数z       
// output parameters:  无   
// Returned value:     转化后的十进制数x      
//-------------------------------------------------------------------------------------------------
u64 p8_bcd(u8 *p8,u32 z);

//-------------------------------------------------------------------------------------------------
// Function name:      p8_add1       
// Descriptions:       将显示缓冲区*p8中的数加1              
// input parameters:   显示缓冲区*p8，显示数的位数z       
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void p8_add1(u8 *p8,u32 z);
	
//-------------------------------------------------------------------------------------------------
// Function name:      p8_sub1       
// Descriptions:       将显示缓冲区*p8中的数减1              
// input parameters:   显示缓冲区*p8，显示数的位数z       
// output parameters:  无   
// Returned value:     无      
//-------------------------------------------------------------------------------------------------
void p8_sub1(u8 *p8,u32 z);

//-------------------------------------------------------------------------------------------------
// Function name:      MeterPnCompare       
// Descriptions:       比较配置的测量点号与F10测量点号是否重复，返回1重复，0不重复              
// input parameters:   配置的装置序号N1，测量点号Pn（当配置脉冲时，N1=MaxRS485AddCarrierPn+1，即所有装置序号都要比较。）       
// output parameters:  无   
// Returned value:     1重复，0不重复      
//-------------------------------------------------------------------------------------------------
u32 MeterPnCompare(u32 N1,u32 Pn);

//-------------------------------------------------------------------------------------------------
// Function name:      PulsePnCompare       
// Descriptions:       比较配置的测量点号与F11测量点号是否重复，返回1重复，0不重复              
// input parameters:   配置的脉冲端口N1，测量点号Pn（当配置装置时，N1=MaxImpInPort+1，即所有脉冲端口都要比较。）       
// output parameters:  无   
// Returned value:     1重复，0不重复      
//-------------------------------------------------------------------------------------------------
u32 PulsePnCompare(u32 N1,u32 Pn);

//-------------------------------------------------------------------------------------------------
// Function name:      Event       
// Descriptions:       查询最近几次重要/普通事件内容                     
// input parameters:   m=1重要事件，m=0一般事件；事件记录代码ERC；最近n次      
// output parameters:  无   
// Returned value:     返回最近n次重要/普通事件ERC的位置          
//-------------------------------------------------------------------------------------------------
u32 Event(u32 m,u32 ERC,u32 n);

//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelect
// Descriptions:        十进制数上下选择
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void UpDownSelect(void);

//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelectH
// Descriptions:        十六进制数上下选择
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void UpDownSelectH(void);

//-------------------------------------------------------------------------------------------------
// Function name:       NdigitSelect
// Descriptions:        N位数左右选择
// input parameters:    N位数，编辑框列号Col，可选范围最小数Min，可选范围最大数Max
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void NdigitSelect(u32 N,u32 Col,u32 Min,u32 Max);

//-------------------------------------------------------------------------------------------------
// Function name:       ArrowLR
// Descriptions:        在Row行、Ln列显示左右箭头，同时清掉其他行的左右箭头
// input parameters:    Row，Ln
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void ArrowLR(u32 Row,u32 Ln);

//-------------------------------------------------------------------------------------------------
// Function name:       PowerP16
// Descriptions:        将376.1协议中A2格式的功率转换后放入显示缓冲区p16中。（显示缓冲指针在本函数外面指定，功率±在本函数外面判断）
// input parameters:    功率y，显示缓冲区p16
// output parameters:   无
// Returned value:      下字符显示缓冲指针
//-------------------------------------------------------------------------------------------------
u16 * PowerP16(u32 y,u16 *p16);

//-------------------------------------------------------------------------------------------------
// Function name:       DisplayClrRow_4
// Descriptions:        清屏幕显示的指定行(4个控制字不清)
// input parameters:    行号(行号0-9(160*160),0-14(320*240))
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayClrRow_4(u32 Row);//清屏幕显示的指定行(4个控制字不清)

//-------------------------------------------------------------------------------------------------
// Function name:       DispalyString1
// Descriptions:        显示字符串（\n\r各空一个半角字，不按回车换行处理）
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23当前字符串位置,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayString1(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);




//-------------------------------------------------------------------------------------------------
// Function name:       DT1DT2_Fn
// Descriptions:        数据标识转化为Fn
// input parameters:    DT1，DT2，DT1的第i位
// output parameters:   无
// Returned value:      Fn,m=1有此Fn,m=0无此Fn
//-------------------------------------------------------------------------------------------------
u32 DT1DT2_Fn(u32 DT1,u32 DT2,u32 i);

//-------------------------------------------------------------------------------------------------
// Function name:       DA1DA2_pn
// Descriptions:        数据标识转化为pn
// input parameters:    DA1，DA2，DA1的第i位
// output parameters:   无
// Returned value:      pn
//-------------------------------------------------------------------------------------------------
u32 DA1DA2_pn(u32 DA1,u32 DA2,u32 i);

//-------------------------------------------------------------------------------------------------
// Function name:       PassWordCreateEdit
// Descriptions:        创建密码编辑框
// input parameters:    下一屏函数地址
// output parameters:   初始化下级菜单任务计数等数据
// Returned value:      无
// Author:              宋玉梅
//-------------------------------------------------------------------------------------------------
void PassWordCreateEdit(void);//创建密码编辑框





//-------------------------------------------------------------------------------------------------
// Function name:       NdigitSelect_SH
// Descriptions:        N位数左右选择
// input parameters:    N位数，编辑框列号Col，可选范围最小数Min，可选范围最大数Max
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void NdigitSelect_SH(u32 Ln,u32 N,u32 Col,u32 Min,u32 Max);
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
void Select_SH(u32 Ln,u32 MaxCol,u32 MinCol);
//-------------------------------------------------------------------------------------------------
// Function name:       UpDownSelect_SH
// Descriptions:        十进制数上下选择
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void UpDownSelect_SH(u32 Ln);//十进制数上下选择
//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenuString_SH
// Descriptions:        上海专变用显示菜单字符串
// input parameters:    屏幕行(0-9),屏幕列(0-19),自动换行时缩进列数,字符串指针(0为串结束)
// output parameters:   无
// Returned value:      B0-B7当前屏幕行,B8-B15当前屏幕列,B16-B23填入行数,B24-B31保留
//-------------------------------------------------------------------------------------------------
u32 DisplayMenuString_SH(u32 Ln,u32 Col,u32 AutoIndent,u8 * pStr);//显示菜单字符串
//-------------------------------------------------------------------------------------------------
// Function name:       DisplayMenu_SH
// Descriptions:        显示菜单
// input parameters:    菜单上移行数,字符串指针(\n换下行,\r回列0,0结束)
// output parameters:   填入显示缓冲区等
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void DisplayMenu_SH(u32 Ln,u32 MoveRow,u8 * pStr);//显示菜单
void NdigitSelect_CSG(u32 N,u32 Col,u32 Min,u32 Max);


#endif






