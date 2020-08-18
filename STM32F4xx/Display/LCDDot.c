

//字符转为点阵
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
//#include "LCDdot.h"



__asm void LoadFontLib_Addr(void)
{
Load_GB2312FontLib_Addr;//取国标汉字库地址=R1,不用其他R,库址址保证ARM有效地址内
	PUSH {LR}
  PUSH {R0}
//	LDR R0,=ADDR_FileDownload;//文件标志(置位表示OK):B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,B5=Font,B6=...B31=
//	LDR R0,[R0]
//	MOV R1,#0x20
//	ANDS R0,R0,R1
//	BNE Load_GB2312FontLib_Addr0
//  LDR R1,=0xC0000000;//无字库装入错
//  POP {R0}
//  POP {PC}
//Load_GB2312FontLib_Addr0
	LDR R0,=ADDR_Font_ENTRY+(4*117)
	LDR R1,[R0]
  CMP R1,R0
  BCS Load_GB2312FontLib_Addr3
  LDR R1,=0xC0000000;//小于入口地址错
  POP {R0}
  POP {PC}
Load_GB2312FontLib_Addr3
	PUSH {R1}
	LDR R1,=(256*1024)-(4*130)
	ADD R0,R0,R1
	POP {R1}
  CMP R1,R0
  BCC Load_GB2312FontLib_Addr4
  LDR R1,=0xC0000000;//大于结束地址错
  POP {R0}
  POP {PC}
Load_GB2312FontLib_Addr4
  POP {R0}
	POP {PC}

	extern ADDR_ASCIILib;
Load_ASCIIFontLib_Addr;//取ASCII字库地址=R1,不用其他R,库址址保证ARM有效地址内
	PUSH {LR}
	LDR R1,=ADDR_ASCIILib-1;
	POP {PC}
	
	extern ADDR_SelfFontLib;//自建小字库首地址
Load_SelfFontLib_Addr;//取自建字库地址=R1,不用其他R,库址址保证ARM有效地址内
	PUSH {LR}
	LDR R1,=ADDR_SelfFontLib-1;
	POP {PC}
//Load_SelfFontLib_Addr;//取自建字库地址=R1,不用其他R,库址址保证ARM有效地址内
//	PUSH {LR}
//  PUSH {R0}
//	LDR R0,=ADDR_FontLib_ENTRY+(4*79)
//	LDR R1,[R0]
//  CMP R1,R0
//  BCS Load_SelfFontLib_Addr1
//  MOV R1,#0;//小于入口地址错
//  POP {R0}
//  POP {PC}
//Load_SelfFontLib_Addr1
//	LDR R0,=ADDR_FontLib_ENTRY+(256*1024)
//  CMP R1,R0
//  BCC Load_SelfFontLib_Addr2
//  MOV R1,#0;//大于结束地址错
//  POP {R0}
//  POP {PC}
//Load_SelfFontLib_Addr2
//  POP {R0}
//	POP {PC}
	
	ALIGN
}




__asm void LCDDotData(u32 TextBuff,u32 DotBuff)//LCD点阵数据转换,入口:R0=字符串缓冲地址,R1=点阵缓冲地址
{
	PUSH {R2-R12,LR}
	MOV R10,R0;//R10=文本缓冲地址
	MOV R8,R1;//R8=点阵缓冲地址
  LDR R0,=0x10000000
  BL Load_GB2312FontLib_Addr;//取国标汉字库地址=R1,不用其他R,库址址保证ARM有效地址内
  STR R1,[R0]
  BL Load_ASCIIFontLib_Addr;//取ASCII字库地址=R1,不用其他R,库址址保证ARM有效地址内
  STR R1,[R0,#4]
	BL Load_SelfFontLib_Addr;//取自建字库地址=R1,不用其他R,库址址保证ARM有效地址内
  STR R1,[R0,#8]
	MOV R7,#0;//R7=当前输出行总,R10=LCDL0C,

THUMB_LCDOUT_LOOP
  if LCDPixels=3;//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  MOV R6,#160;//当前屏幕可视行数
  else
  MOV R6,#240;//当前屏幕可视行数
  endif
	LDRB R0,[R10]
	CMP R0,#0
	BEQ LCDOUT_1;//上行间距点数=0
	ADD R5,R0,R7
	CMP R5,R6
	BCC LCDOUT_3
	SUB R0,R6,R7
	BL LCDOUT_LNCLR;//LCD输出当前点阵行清除;入口R7=当前行(0-239),R0=清除连续行数
	POP {R2-R12,PC}
LCDOUT_3
	ADD R7,R0;//修改当前输出行
	BL LCDOUT_LNCLR;//LCD输出当前点阵行清除;入口R7=当前行(0-239),R0=清除连续行数
LCDOUT_1
  if LCDPixels=3;//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  MOV R1,#160;//当前屏幕可视行数
  else
  MOV R1,#240;//当前屏幕可视行数
  endif
	LDRB R5,[R10,#2];//R10=LCDL0C
	CMP R5,#3;//14
	BCC LCDOUT_100
	MOV R5,#0;//大于=3号作0号
LCDOUT_100
  CMP R5,#0
  BEQ LCDOUT_FONT0;//0号字(X,Y)
  CMP R5,#1
  BEQ LCDOUT_FONT1;//1号字(X,2Y)
  CMP R5,#2
  BEQ LCDOUT_FONT2;//2号字(2X,2Y)
  CMP R5,#3
  BEQ LCDOUT_FONT3;//3号字(2X,3Y)
  CMP R5,#4
  BEQ LCDOUT_FONT4;//4号字(2X,4Y)
//;  CMP R5,#5
//;  BEQ LCDOUT_FONT5;//5号字(3X,3Y)
//;  CMP R5,#6
//;  BEQ LCDOUT_FONT6;//6号字(3X,4Y)
//;  CMP R5,#7
//;  BEQ LCDOUT_FONT7;//7号字(3X,5Y)
//;  CMP R5,#8
//;  BEQ LCDOUT_FONT8;//8号字(3X,6Y)
//;  CMP R5,#9
//;  BEQ LCDOUT_FONT9;//9号字(4X,4Y)
//;  CMP R5,#10
//;  BEQ LCDOUT_FONT10;//10号字(4X,5Y)
//;  CMP R5,#11
//;  BEQ LCDOUT_FONT11;//11号字(4X,6Y)
//;  CMP R5,#12
//;  BEQ LCDOUT_FONT12;//12号字(4X,7Y)
//;  CMP R5,#13
//;  BEQ LCDOUT_FONT13;//13号字(4X,8Y)


LCDOUT_FONT0;//0号字(X,Y)
  MOV R2,#16;//16行
  BL LCDOUT_FONT_COM;//入口R1=当前屏幕可视行数,R2=当前行字体占有行数-1,出口:清屏未没用行,R6=0
LCDOUT_FONT0_2
  BL LCDOUT_X
  ADD R6,#1
  CMP R6,#16
  BNE LCDOUT_FONT0_2
  ADD R7,#16;//修改当前输出行
THUMB_LCDOUT_LOOP0
  ADD R10,#84
  B THUMB_LCDOUT_LOOP


LCDOUT_FONT1;//1号字(X,2Y)
  MOV R2,#32;//32行
  BL LCDOUT_FONT_COM;//入口R1=当前屏幕可视行数,R2=当前行字体占有行数-1,出口:清屏未没用行,R6=0
LCDOUT_FONT1_2
  BL LCDOUT_X
  BL LCDOUT_X
  ADD R6,#1
  CMP R6,#16
  BNE LCDOUT_FONT1_2
  ADD R7,#32;//修改当前输出行
  B THUMB_LCDOUT_LOOP0

LCDOUT_FONT2;//2号字(2X,2Y)
  MOV R2,#32;//32行
  MOV R3,#2
LCDOUT_FONT2_0
  BL LCDOUT_FONT_COM;//入口R1=当前屏幕可视行数,R2=当前行字体占有行数-1,出口:清屏未没用行,R6=0
  PUSH {R2}
  PUSH {R3}
LCDOUT_FONT2_2
  POP {R3}
  PUSH {R3}
  CMP R3,#2
  BEQ LCDOUT_FONT2_2Y
  CMP R3,#3
  BEQ LCDOUT_FONT2_3Y
  BL LCDOUT_2X
LCDOUT_FONT2_3Y
  BL LCDOUT_2X
LCDOUT_FONT2_2Y
  BL LCDOUT_2X
  BL LCDOUT_2X
  ADD R6,#1
  CMP R6,#16
  BNE LCDOUT_FONT2_2
  POP {R3}
  POP {R2}
  ADD R7,R2;//修改当前输出行
  B THUMB_LCDOUT_LOOP0

LCDOUT_FONT3;//3号字(2X,3Y)
  MOV R2,#48;//48行
  MOV R3,#3
  B LCDOUT_FONT2_0

LCDOUT_FONT4;//4号字(2X,4Y)
  MOV R2,#64;//64行
  MOV R3,#4
  B LCDOUT_FONT2_0

LCDOUT_FONT5;//5号字(3X,3Y)
  MOV R2,#48;//64行
  MOV R3,#3
LCDOUT_FONT5_0
  BL LCDOUT_FONT_COM;//入口R1=当前屏幕可视行数,R2=当前行字体占有行数-1,出口:清屏未没用行,R6=0
  PUSH {R2}
  PUSH {R3}
LCDOUT_FONT5_2
  POP {R3}
  PUSH {R3}
  CMP R3,#3
  BEQ LCDOUT_FONT5_3Y
  CMP R3,#4
  BEQ LCDOUT_FONT5_4Y
  CMP R3,#5
  BEQ LCDOUT_FONT5_5Y
  BL LCDOUT_3X
LCDOUT_FONT5_5Y
  BL LCDOUT_3X
LCDOUT_FONT5_4Y
  BL LCDOUT_3X
LCDOUT_FONT5_3Y
  BL LCDOUT_3X
  BL LCDOUT_3X
  BL LCDOUT_3X
  ADD R6,#1
  CMPS R6,#16
  BNE LCDOUT_FONT5_2
  POP {R3}
  POP {R2}
  ADD R7,R2;//修改当前输出行
  B THUMB_LCDOUT_LOOP0

LCDOUT_FONT6;//6号字(3X,4Y)
  MOV R2,#64;//64行
  MOV R3,#4
  B LCDOUT_FONT5_0
LCDOUT_FONT7;//7号字(3X,5Y)
  MOV R2,#80;//64行
  MOV R3,#5
  B LCDOUT_FONT5_0
LCDOUT_FONT8;//8号字(3X,6Y)
  MOV R2,#96;//64行
  MOV R3,#6
  B LCDOUT_FONT5_0

LCDOUT_FONT9;//9号字(4X,4Y)
  MOV R2,#64;//64行
  MOV R3,#4
LCDOUT_FONT9_0
  BL LCDOUT_FONT_COM;//入口R1=当前屏幕可视行数,R2=当前行字体占有行数-1,出口:清屏未没用行,R6=0
  PUSH {R2}
  PUSH {R3}
LCDOUT_FONT9_2
  POP {R3}
  PUSH {R3}
  CMP R3,#4
  BEQ LCDOUT_FONT9_4Y
  CMP R3,#5
  BEQ LCDOUT_FONT9_5Y
  CMP R3,#6
  BEQ LCDOUT_FONT9_6Y
  CMP R3,#7
  BEQ LCDOUT_FONT9_7Y
  BL LCDOUT_4X
LCDOUT_FONT9_7Y
  BL LCDOUT_4X
LCDOUT_FONT9_6Y
  BL LCDOUT_4X
LCDOUT_FONT9_5Y
  BL LCDOUT_4X
LCDOUT_FONT9_4Y
  BL LCDOUT_4X
  BL LCDOUT_4X
  BL LCDOUT_4X
  BL LCDOUT_4X
  ADD R6,#1
  CMP R6,#16
  BNE LCDOUT_FONT9_2
  POP {R3}
  POP {R2}
  ADD R7,R2;//修改当前输出行
  B THUMB_LCDOUT_LOOP0

LCDOUT_FONT10;//10号字(4X,5Y)
  MOV R2,#80;//64行
  MOV R3,#5
  B LCDOUT_FONT9_0
LCDOUT_FONT11;//11号字(4X,6Y)
  MOV R2,#96;//64行
  MOV R3,#6
  B LCDOUT_FONT9_0
LCDOUT_FONT12;//12号字(4X,7Y)
  MOV R2,#112;//64行
  MOV R3,#7
  B LCDOUT_FONT9_0
LCDOUT_FONT13;//13号字(4X,8Y)
  MOV R2,#128;//64行
  MOV R3,#8
  B LCDOUT_FONT9_0

	ALIGN 4
	LTORG;//数据缓冲池声明

LCDOUT_LNCLR;//LCD输出当前点阵行清除;入口R7=当前行(0-239),R0=清除连续行数
  PUSH {LR}
  CMP R0,#0
  BNE LCDOUT_LNCLR_2
  POP {PC}
LCDOUT_LNCLR_2
  if LCDPixels=3;//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  MOV R2,#20;//
  else
  MOV R2,#40;//
  endif
  MUL R2,R0;//清除字节数
  MOV R1,R8
  MOV R3,#0
LCDOUT_LNCLR_1
  STRB R3,[R1]
  ADD R1,#1
  MOV R8,R1
  SUBS R2,#1
  BNE LCDOUT_LNCLR_1
  POP {PC}

LCDOUT_X
  PUSH {LR}
  MOV R0,R10;//R10=LCDL0C,
  ADD R0,#4
  if LCDPixels=3;//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  MOV R5,#20;//当前输出列,每8点为一列
  else
  MOV R5,#40;//当前输出列,每8点为一列
  endif
LCDOUT_X_1
  LDRH R2,[R0]
  PUSH {R2}
  PUSH {R0}
  LDR R0,=0x10000000
  LDR R9,[R0]
  LDR R4,=0X3FFF
  LDR R1,=0X4000
  ANDS R1,R2
  BNE LCDOUT_X_FONTLIB
  LDR R9,[R0,#4]
  LDR R4,=0X1FFF
  LDR R1,=0X2000
  ANDS R1,R2
  BNE LCDOUT_X_FONTLIB
  LDR R9,[R0,#8]
LCDOUT_X_FONTLIB
  POP {R0}
  ANDS R2,R4
  ADD R0,#2
  MOV R1,#16
  MUL R2,R1
  ADD R2,R6
  ADD R2,R9
  LDRB R2,[R2]
  POP {R1}
  LDR R4,=0X8000
  ANDS R4,R1
  BEQ LCDOUT_X_2
  MVN R2,R2
LCDOUT_X_2
  STRB R2,[R8];//R8=SDA开始地址
  ADD R8,#1
  SUBS R5,#1
  BNE LCDOUT_X_1
  POP {PC}



LCDOUT_2X
  PUSH {LR}
  ADD R0,R10,#4;//R7=当前输出行总,R10=LCDL0C
  if LCDPixels=3;//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  MOV R1,#10;//当前输出列,每8点为一列
  else
  MOV R1,#20;//当前输出列,每8点为一列
  endif
  MOV R2,#16
  LDR R12,=0X8000
LCDOUT_2X_1
  LDRH R3,[R0]
  ADD R0,R0,#2
  PUSH {R1}
  PUSH {R2}
  LDR R1,=0x10000000
  LDR R9,[R1,#0]
  LDR R4,=0X3FFF
  LDR R2,=0X4000
  ANDS R2,R2,R3
  BNE LCDOUT_2X_FONTLIB
  LDR R9,[R1,#4]
  LDR R4,=0X1FFF
  LDR R2,=0X2000
  ANDS R2,R2,R3
  BNE LCDOUT_2X_FONTLIB
  LDR R9,[R1,#8]
LCDOUT_2X_FONTLIB
  ADD R11,R6,R9
  POP {R2}
  POP {R1}
  PUSH {R3}
  AND R3,R3,R4
  MUL R4,R3,R2
  ADD R4,R4,R11
  LDRB R4,[R4]
  POP {R3}
  ANDS R5,R3,R12
  BEQ LCDOUT_2X_2
  MVN R4,R4
LCDOUT_2X_2
  MOV R3,#8
LCDOUT_2X_3
  MOVS R4,R4,RRX
  MOV R5,R5,RRX
  MOV R5,R5,RRX
  SUBS R3,R3,#1
  BNE LCDOUT_2X_3
  MOV R3,R5,LSR #24
  MOV R4,R1
  MOV R1,R8
  STRB R3,[R1]
  ADD R1,R1,#1
  MOV R5,R5,LSR #16
  MOV R3,R5
  STRB R3,[R1]
  ADD R8,R8,#2
  MOV R1,R4
  SUBS R1,R1,#1
  BNE LCDOUT_2X_1
  POP {PC}


LCDOUT_3X
  PUSH {LR}
  ADD R0,R10,#4;//R7=当前输出行总,R10=LCDL0C
  if LCDPixels=3;//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  MOV R1,#6;//当前输出列,每8点为一列
  else
  MOV R1,#13;//当前输出列,每8点为一列
  endif
  MOV R2,#16
  LDR R12,=0X8000
LCDOUT_3X_1
  LDRH R3,[R0]
  ADD R0,R0,#2
  PUSH {R1}
  PUSH {R2}
  LDR R1,=0x10000000
  LDR R1,[R1,#0]
  LDR R4,=0X3FFF
  LDR R2,=0X4000
  ANDS R2,R2,R3
  BNE LCDOUT_3X_FONTLIB
  LDR R1,=0x10000000
  LDR R1,[R1,#4]
  LDR R4,=0X1FFF
  LDR R2,=0X2000
  ANDS R2,R2,R3
  BNE LCDOUT_3X_FONTLIB
  LDR R1,=0x10000000
  LDR R1,[R1,#8]
LCDOUT_3X_FONTLIB
  ADD R11,R6,R1
  POP {R2}
  POP {R1}
  PUSH {R3}
  AND R3,R3,R4
  MUL R4,R3,R2
  ADD R4,R4,R11
  LDRB R4,[R4]
  POP {R3}
  ANDS R5,R3,R12
  BEQ LCDOUT_3X_2
  MVN R4,R4
LCDOUT_3X_2
  MOV R3,#8
LCDOUT_3X_3
  MOVS R4,R4,RRX
  MOV R5,R5,RRX
  MOV R5,R5,RRX
  MOV R5,R5,RRX
  SUBS R3,R3,#1
  BNE LCDOUT_3X_3
  MOV R3,R5,LSR #24
  MOV R4,R1
  MOV R1,R8
  STRB R3,[R1]
  ADD R1,R1,#1
  MOV R3,R5,LSR #16
  STRB R3,[R1]
  ADD R1,R1,#1
  MOV R3,R5,LSR #8
  STRB R3,[R1]
  ADD R8,R8,#3
  MOV R1,R4
  SUBS R1,R1,#1
  BNE LCDOUT_3X_1
  MOV R3,#0;//13*3+1=40
  MOV R4,R1
  MOV R1,R8
  STRB R3,[R1]
  ADD R8,R8,#1
  MOV R1,R4
  POP {PC}


LCDOUT_4X
  PUSH {LR}
  ADD R0,R10,#4;//R7=当前输出行总,R10=LCDL0C
  if LCDPixels=3;//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  MOV R1,#5;//当前输出列,每8点为一列
  else
  MOV R1,#10;//当前输出列,每8点为一列
  endif
  MOV R2,#16
  LDR R12,=0X8000
LCDOUT_4X_1
  LDRH R3,[R0]
  ADD R0,R0,#2
  PUSH {R1}
  PUSH {R2}
  LDR R1,=0x10000000
  LDR R1,[R1,#0]
  LDR R4,=0X3FFF
  LDR R2,=0X4000
  ANDS R2,R2,R3
  BNE LCDOUT_4X_FONTLIB
  LDR R1,=0x10000000
  LDR R1,[R1,#4]
  LDR R4,=0X1FFF
  LDR R2,=0X2000
  ANDS R2,R2,R3
  BNE LCDOUT_4X_FONTLIB
  LDR R1,=0x10000000
  LDR R1,[R1,#8]
LCDOUT_4X_FONTLIB
  ADD R11,R6,R1
  POP {R2}
  POP {R1}
  PUSH {R3}
  AND R3,R3,R4
  MUL R4,R3,R2
  ADD R4,R4,R11
  LDRB R4,[R4]
  POP {R3}
  ANDS R5,R3,R12
  BEQ LCDOUT_4X_2
  MVN R4,R4
LCDOUT_4X_2
  MOV R3,#8
LCDOUT_4X_3
  MOVS R4,R4,RRX
  MOV R5,R5,RRX
  MOV R5,R5,RRX
  MOV R5,R5,RRX
  MOV R5,R5,RRX
  SUBS R3,R3,#1
  BNE LCDOUT_4X_3
  MOV R3,R5,LSR #24
  MOV R4,R1
  MOV R1,R8
  STRB R3,[R1]
  ADD R1,R1,#1
  MOV R3,R5,LSR #16
  STRB R3,[R1]
  ADD R1,R1,#1
  MOV R3,R5,LSR #8
  STRB R3,[R1]
  ADD R1,R1,#1
  MOV R3,R5
  STRB R3,[R1]
  ADD R8,R8,#4
  MOV R1,R4
  SUBS R1,R1,#1
  BNE LCDOUT_4X_1
  POP {PC}

LCDOUT_FONT_COM;//入口R1=当前屏幕可视行数,R2=当前行字体占有行数,出口:清屏未没用行,R6=0
	PUSH {LR}
  PUSH {R2}
  SUB R2,#1
  ADD R2,R7
  CMP R2,R1
  BCC LCDOUT_FONT0_1;//加上本显示数据行数<当前屏幕可视行数
  SUB R0,R1,R7;//清除行数=当前屏幕可视行数-当前输出行
  BL LCDOUT_LNCLR;//LCD输出当前点阵行清除;入口R7=当前行(0-239),R0=清除连续行数
  POP {R2}
 	POP {R0}
  POP {R2-R12,PC}
LCDOUT_FONT0_1
  MOV R6,#0
  POP {R2}
	POP {PC}
	
	ALIGN	
}
















