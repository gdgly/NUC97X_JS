
// 本头文件只定义在无ERAM时不使用的数据
#ifndef IRAM_TerminalDisplay_H
#define IRAM_TerminalDisplay_H

#define ADDR_SystemStack     ADDR_Terminal_IRAM_Start		// 调用菜单软件模块前堆栈指针(字对齐),不为0值表示正在调用菜单
// 4byte
#define ADDR_MenuStack     (ADDR_SystemStack+4)
// 菜单堆栈
typedef __packed struct
{
  u8 Count;		//菜单堆栈计数
  u8 Res1;
  u8 Res2;
  u8 Res3;
  u32 v1;		// 菜单用变量1
  u32 v2;		// 菜单用变量2
  u32 v3;		// 菜单用变量3
}MenuCount_TypeDef;
#define MenuCount     ((MenuCount_TypeDef *)(ADDR_MenuStack))
#define MaxMenuCount     10		// 最大菜单级数
typedef __packed struct
{
  u32 PC;						// 菜单任务运行地址(特别注意:此值不可随便改动)
  u8 Task;						// 菜单任务计数(进入和退出时均清为0)
  u8 MoveRow;					// 菜单上移条数(进入时总为0,返回时不变)
  u8 EditRow;					// 菜单当前正在编辑行号(0-9)(进入时总为1,返回时不变)
  u8 EditCol;					// 菜单当前正在编辑列号(0-19)(进入时总为0,返回时不变)
  u8 TotalRow;					// 菜单总条数
  u8 MenuRow;					// 菜单当前装入条数
  u8 res2;
  u8 res3;
}MenuSave_TypeDef;
typedef __packed struct
{
  MenuSave_TypeDef     MenuStack[MaxMenuCount+1];
}MenuStack_TypeDef;
#define Stack     ((MenuStack_TypeDef *)(ADDR_MenuStack+16))
#define ADDR_MenuStack_End     (ADDR_MenuStack+16+(12*(MaxMenuCount+1)))

//点阵显示合成模式
#define ADDR_DOTMIXPMODE     (ADDR_MenuStack_End)
typedef __packed struct
{
	u8 Task;					// 当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
	u8 Mix;						// 点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
	u8 FlashMode;				// 动画模式:0=无,1=下移,2=上移,3=左移,4=右移,5=...
	u8 FlashDelay;				// 动画变化延时(单位10ms),0最快,255最慢
	u8 FlashTask;				// 动画任务:0=无,1=开始,2=播放
	u8 FlashCount;				// 动画播放计数
	u8 res1;
	u8 res2;
}DOTMIXPMODE_TypeDef;
#define DOTMIXPMODE     ((DOTMIXPMODE_TypeDef *)(ADDR_DOTMIXPMODE))
//循环显示控制
#define ADDR_TerminalDisplayLoop     (ADDR_DOTMIXPMODE+8)
typedef __packed struct
{
	u8 Task;					// 当前循环显示任务
	u8 SubTask;					// 当前循环显示子任务(暂不用)
	u8 Count;					// 当前循环显示任务已显屏计数
	u8 ButtonVal;				// 按键状态寄存用于自动找寻方向
}TerminalDisplayLoop_TypeDef;
#define TerminalDisplayLoop     ((TerminalDisplayLoop_TypeDef *)(ADDR_TerminalDisplayLoop))

// 点阵数据缓冲
#define ADDR_DOTDATA_START     (ADDR_TerminalDisplayLoop+4)
#define LEN_DOTDATABUFF     9600		//点阵数据缓冲长度
#define ADDR_DOTDATABUFF     ADDR_DOTDATA_START
// 320*240/8=9600字节点阵数据缓冲1
#define ADDR_DOTDATABUFF2     (ADDR_DOTDATABUFF+LEN_DOTDATABUFF)
// 320*240/8=9600字节点阵数据缓冲2
#define ADDR_STRINGBUFF     (ADDR_DOTDATABUFF2+LEN_DOTDATABUFF)
// 字符缓冲区1;每行84字节(4字节控制,80字节数据)
typedef __packed struct
{
  u8 DotLn;				// 与上行间距点数(0-255)
  u8 Row;				// 字符串号(0-9)(1个字符串在屏上可分多行显示)
  u8 Font;				// 本行字体号0-4
  u8 Align;				// 对齐方式:0=无对齐,1=左对齐,2=居中,3=右对齐
  u16 Col[40];			// 显示编码
						// 显示编码每2字节代表8*16半字,每行显示缓冲80字节40个半字
						// B15=0正显,B15=1反显
						// B14=0 B13=0,从自建小字库取点阵,最大编号空间0-0x2000=8192
						// B14=0 B13=1,B12-B0=0-255从ASCII8*16标准字库取点阵,最大编号空间=256
						// B14=0 B13=1,B12-B0=256-511从ASCII8*12标准取点阵(暂不用),最大编号空间=256
						// B14=0 B13=1,B12-B0=512-767从ASCII8*8标准取点阵(暂不用),最大编号空间=256
						// B14=1 B13-B0,表示从GB2312标准字库(修改后)取点阵,最大编号空间0-0x4000
}StrLnBuff_TypeDef;
typedef __packed struct
{
  StrLnBuff_TypeDef Row[16];
}StrBuff_TypeDef;
#define StrBuff     ((StrBuff_TypeDef *)(ADDR_STRINGBUFF))
//字符缓冲区2;每行84字节(4字节控制,80字节数据)
#define ADDR_STRINGBUFF2     (ADDR_STRINGBUFF+(84*16))
#define StrBuff2     ((StrBuff_TypeDef *)(ADDR_STRINGBUFF2))

#define ADDR_WarningStringBuff     (ADDR_STRINGBUFF2+(84*16))
#define MaxWarningString     8	//最大告警字符串条数
// 8条告警字符串缓冲区,每条84字节(2字节告警代码,2保留字节,80字符)
// 2BYTE 告警代码,0表示无告警
// 2BYTE 保留
// 80BYTE 告警字符缓冲区
	
#define ADDR_MenuICO     (ADDR_WarningStringBuff+(84*MaxWarningString))
// 1byte 菜单中显示顶行和底行图标 0=否,1=是
#define ADDR_AutoDisplayRowCol     ADDR_MenuICO+1 //自动顺序显示时的当前显示行号列号;在void DisplayClr(void)//清全屏时同时清0
// 1byte Row号
// 1byte Col号
typedef __packed struct
{
  u8 Row;
  u8 Col;
}AutoDisplayRowCol_TypeDef;
#define AutoDisplayRowCol     ((AutoDisplayRowCol_TypeDef *)(ADDR_AutoDisplayRowCol))

#define ADDR_GUI_TTF_DispCode     ADDR_AutoDisplayRowCol+2
typedef __packed struct
{
	u8 Code;		//GUI_TTF.c用原显示的代码>=0zA1寄存表示第一个GB2312码,等下个GB2312一起显示
	u8 res;
}GUI_TTF_DispCode_TypeDef;
#define GUI_TTF_DispCode     ((GUI_TTF_DispCode_TypeDef *)(ADDR_GUI_TTF_DispCode))


#define ADDR_DOTDATA_END     (ADDR_GUI_TTF_DispCode+2)


		
#define ADDR_TerminalDisplay_IRAM_End     ((ADDR_DOTDATA_END+3)&0xfffffffc)		// 4字节对齐


#endif

