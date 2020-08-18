

#include "../Hcsg/ProjectCSG.h"
//#include "../Display/Draw.h"




//-------------------------------------------------------------------------------------------------
// Function name:       Draw_Clr
// Descriptions:        清绘画显示
// input parameters:    无
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
void Draw_Clr(void)//清绘画显示
{
  u32 i;
  u32 * p32;
	p32=(u32 *)(ADDR_DOTDATABUFF+LEN_DOTDATABUFF);
	for(i=0;i<(LEN_DOTDATABUFF/4);i++)
	{
		p32[i]=0x0;
	}
}

//-------------------------------------------------------------------------------------------------
// Function name:       Draw_Dot
// Descriptions:        绘点
// input parameters:    座标x,y,座标原点LCD左上角
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
__asm void Draw_Dot(u32 x,u32 y)
{
	PUSH {LR}
  PUSH {R5-R7}
  MOV R5,#40;//320
  MOV R7,#LCDPixels//LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
  CMP R7,#2
  BEQ DRAW_DOT_1
  MOV R5,#20;//160
DRAW_DOT_1
  MUL R6,R5,R1;//Y*40或Y*20
	MOV R5,R6
  MOV R6,R0
  LSR R6,#3;//X/8
  ADD R6,R5
	LDR R5,=ADDR_DOTDATABUFF+LEN_DOTDATABUFF;//缓冲区地址
  ADD R6,R5
  MOV R5,R0
  MOV R7,#0x07
  AND R7,R5
  MOV R5,#0x80
  LSR R5,R7
  LDRB R7,[R6]
  ORR R7,R5
  STRB R7,[R6]
  POP {R5-R7}
	POP {PC}
	ALIGN
}

//-------------------------------------------------------------------------------------------------
// Function name:       Draw_Line
// Descriptions:        绘直线
// input parameters:    开始座标X1,Y1,结束座标X2,Y2,座标原点LCD左上角
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
__asm void Draw_Line(u32 x1,u32 y1,u32 x2,u32 y2)
{
	PUSH {R2-R12,LR}
  CMP R0,R2
  BCS LINE_1;//R0>R2
  MOV R5,R2
  SUB R5,R0;//△X=R2-R0
  B LINE_2
LINE_1
  MOV R5,R0
  SUB R5,R2;//△X=R0-R2
LINE_2
  CMP R1,R3
  BCS LINE_3;//R1>R3
  MOV R6,R3
  SUB R6,R1;//△Y=R3-R1
  B LINE_4
LINE_3
  MOV R6,R1
  SUB R6,R3;//△Y=R1-R3
LINE_4
  CMP R5,R6
  BCC LINE_Y;//△X<△Y  △X=K△Y绘图(K总是<±1)
LINE_X
  CMP R0,R2;//△X>△Y  △Y=K△X绘图(K总是<±1)
  BCC LINE_X_1;//开始X座标<结束X座标
  MOV R7,R0;//座标交换
  MOV R0,R2
  MOV R2,R7
  MOV R7,R1
  MOV R1,R3
  MOV R3,R7
LINE_X_1
  BL Draw_Dot;//绘开始点
  ;//R5=△X,R6=△Y,R8=△Xn,△Yn=(△Y*△Xn)/△X
  MOV R7,#0
  MOV R8,R7;//R8=△Xn=0
  PUSH {R0}
  PUSH {R1}
LINE_X_2
  CMP R0,R2
  BNE LINE_X_NEXT;//X没到结束点
	B LINE_END
LINE_X_NEXT
  MOV R7,R8;//△Xn+1
  ADD R7,#1
  MOV R8,R7
  MOV R0,R6
  MUL R1,R0,R7;//R0=(△Y*△Xn)
	MOV R0,R1
  MOV R1,#0;//△Yn=0
LINE_X_3
  CMP R0,R5
  BCC LINE_X_4;//R0=(△Y*△Xn)<R5=△X
  SUB R0,R5;//R0=(△Y*△Xn)-R5=△X
  ADD R1,#1;//△Yn+1
  B LINE_X_3
LINE_X_4
  MOV R7,R1;//R7=△Yn
  POP {R1}
  POP {R0} 
  PUSH {R0}
  PUSH {R1}
  ADD R0,R8;//X=Xs+△Xn
  CMP R1,R3
  BCC LINE_X_5;//△Ys<△Ye  Y=Ys+△Yn
  SUB R1,R7;//△Ys>△Ye  Y=Ys-△Yn
  BL Draw_Dot;//绘点
  B LINE_X_2
LINE_X_5
  ADD R1,R7
  BL Draw_Dot;//绘点
  B LINE_X_2

LINE_Y;//△X<△Y  △X=K△Y绘图
  CMP R1,R3;//△X<△Y  △X=K△Y绘图(K总是<±1)
  BCC LINE_Y_1;//开始Y座标<结束Y座标
  MOV R7,R0;//座标交换
  MOV R0,R2
  MOV R2,R7
  MOV R7,R1
  MOV R1,R3
  MOV R3,R7
LINE_Y_1
  BL Draw_Dot;//绘开始点
  ;//R5=△X,R6=△Y,R8=△Yn,△Xn=(△X*△Yn)/△Y
  MOV R7,#0
  MOV R8,R7;//R8=△Yn=0
  PUSH {R0}
  PUSH {R1}
LINE_Y_2
  CMP R1,R3
  BNE LINE_Y_NEXT;//Y没到结束点
  B LINE_END
LINE_Y_NEXT
  MOV R7,R8;//△Yn+1
  ADD R7,#1
  MOV R8,R7
  MOV R0,R5
  MUL R1,R0,R7;//R0=(△X*△Yn)
	MOV R0,R1
  MOV R1,#0;//△Xn=0
LINE_Y_3
  CMP R0,R6
  BCC LINE_Y_4;//R0=(△X*△Yn)<R6=△Y
  SUB R0,R6;//R0=(△X*△Yn)-R6=△Y
  ADD R1,#1;//△Xn+1
  B LINE_Y_3
LINE_Y_4
  MOV R7,R1;//R7=△Xn
  POP {R1}
  POP {R0} 
  PUSH {R0}
  PUSH {R1}
  ADD R1,R8;//Y=Ys+△Yn
  CMP R0,R2
  BCC LINE_Y_5;//△Xs<△Xe  X=Xs+△Xn
  SUB R0,R7;//△Xs>△Xe  X=Xs-△Xn
  BL Draw_Dot;//绘点
  B LINE_Y_2
LINE_Y_5
  ADD R0,R7
  BL Draw_Dot;//绘点
  B LINE_Y_2
LINE_END
  POP {R1}
  POP {R0}
  POP {R2-R12,PC}
	ALIGN
}


//-------------------------------------------------------------------------------------------------
// Function name:       Draw_Round
// Descriptions:        绘园
// input parameters:    圆心座标x,y,半径r,座标原点LCD左上角
// output parameters:   无
// Returned value:      无
//-------------------------------------------------------------------------------------------------
__asm void Draw_Round(u32 x,u32 y,u32 r)
{
;//贝森海姆算法,45度为1个区,共分8区绘
	PUSH {R2-R12,LR}
  MOV R10,R0;//R10=圆心X
  MOV R11,R1;//R11=圆心Y
  MOV R12,R2;//R12=半径
  PUSH {R4}
  MOV R7,R2
  LDR R2,=1000
  MUL R6,R7,R2
	MOV R7,R6
  LDR R6,=1414
#ifdef IC_NUC9xx
	MOV R5,#0
div_2
	CMP R7,R6
	BCC div_1
	SUB R7,R6
	ADD R5,#1
	B div_2
div_1
#else
	UDIV R5,R7,R6
#endif
  MOV R9,R5;//R9=半径/根号2
  POP {R4}

  MOV R6,#0;//R6=X'
  MOV R7,R12;//R7=Y'
DRAW_ROUND_1
  MOV R0,R10
  ADD R0,R6
  MOV R1,R11
  SUB R1,R7
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R6,R9
  BEQ DRAW_ROUND_2
  BL DRAW_ROUND_COM
  B DRAW_ROUND_1
DRAW_ROUND_2

  MOV R6,#0;//R6=Y'
  MOV R7,R12;//R7=X'
DRAW_ROUND_3
  MOV R0,R10
  ADD R0,R7
  MOV R1,R11
  SUB R1,R6
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R7,R9
  BEQ DRAW_ROUND_4
  BL DRAW_ROUND_COM
  B DRAW_ROUND_3
DRAW_ROUND_4
  MOV R6,#0;//R6=Y'
  MOV R7,R12;//R7=X'
DRAW_ROUND_5
  MOV R0,R10
  ADD R0,R7
  MOV R1,R11
  ADD R1,R6
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R7,R9
  BEQ DRAW_ROUND_6
  BL DRAW_ROUND_COM
  B DRAW_ROUND_5
DRAW_ROUND_6
  MOV R6,#0;//R6=X'
  MOV R7,R12;//R7=Y'
DRAW_ROUND_7
  MOV R0,R10
  ADD R0,R6
  MOV R1,R11
  ADD R1,R7
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R6,R9
  BEQ DRAW_ROUND_8
  BL DRAW_ROUND_COM
  B DRAW_ROUND_7
DRAW_ROUND_8
  MOV R6,#0;//R6=X'
  MOV R7,R12;//R7=Y'
DRAW_ROUND_9
  MOV R0,R10
  SUB R0,R6
  MOV R1,R11
  ADD R1,R7
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R6,R9
  BEQ DRAW_ROUND_10
  BL DRAW_ROUND_COM
  B DRAW_ROUND_9
DRAW_ROUND_10
  MOV R6,#0;//R6=Y'
  MOV R7,R12;//R7=X'
DRAW_ROUND_11
  MOV R0,R10
  SUB R0,R7
  MOV R1,R11
  ADD R1,R6
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R7,R9
  BEQ DRAW_ROUND_12
  BL DRAW_ROUND_COM
  B DRAW_ROUND_11
DRAW_ROUND_12
  MOV R6,#0;//R6=Y'
  MOV R7,R12;//R7=X'
DRAW_ROUND_13
  MOV R0,R10
  SUB R0,R7
  MOV R1,R11
  SUB R1,R6
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R7,R9
  BEQ DRAW_ROUND_14
  BL DRAW_ROUND_COM
  B DRAW_ROUND_13
DRAW_ROUND_14
  MOV R6,#0;//R6=X'
  MOV R7,R12;//R7=Y'
DRAW_ROUND_15
  MOV R0,R10
  SUB R0,R6
  MOV R1,R11
  SUB R1,R7
  BL Draw_Dot;//绘点,入口R0,R1点座标X,Y,座标原点相对LCD左上角,R4绘图层,使用寄存器保护
  CMP R6,R9
  BEQ DRAW_ROUND_16
  BL DRAW_ROUND_COM
  B DRAW_ROUND_15
DRAW_ROUND_16
  POP {R2-R12,PC}

DRAW_ROUND_COM
	PUSH {LR}
  ADD R6,#1
  MOV R0,R6
  MUL R2,R0,R6;//R0=X+1平方
	MOV R0,R2
  MOV R1,R7
  MUL R2,R1,R7;//R1=Y平方
	MOV R1,R2
  MOV R2,R7
  SUB R2,#1
  MOV R3,R2
  MUL R5,R2,R3;//R2=Y-1平方
	MOV R2,R5
  MOV R3,R12
  MOV R5,R3
	PUSH {R0}
  MUL R0,R3,R5;//R3=R平方
	MOV R3,R0
	POP {R0}
  ADD R1,R0;//R1=X+1平方+Y平方
  ADD R2,R0;//R2=X+1平方+Y-1平方
  SUB R1,R3;//R1=X+1平方+Y平方-R平方
  SUB R2,R3;//R2=X+1平方+Y-1平方-R平方
  MOV R0,R1
  LSL R0,#1;//B31符号位
  BCC DRAW_ROUND_COM_2
  MVN R1,R1
  ADD R1,#1
DRAW_ROUND_COM_2
  MOV R0,R2
  LSL R0,#1;//B31符号位
  BCC DRAW_ROUND_COM_3
  MVN R2,R2
  ADD R2,#1
DRAW_ROUND_COM_3
  CMP R1,R2
  BCC DRAW_ROUND_COM_1;//R1=X+1平方+Y平方-R平方<R2=X+1平方+Y-1平方-R平方
  SUB R7,#1
DRAW_ROUND_COM_1
  POP {PC}
}















