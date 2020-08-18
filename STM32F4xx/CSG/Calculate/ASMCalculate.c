
//ASMCalculate
#include "../Hcsg/ProjectCSG.h"
//#include "../Calculate/ASMCalculate.h"







__asm u32 sqrt4_2(u32 r0)//开平方
{
;//开平方(R6)开方=R5,使用R7-R2
;//计算方法
;//(R7←R6)-(R5←0X40000000)比较  0X40000000为固定值!!!
;//步骤1
;//若(R7←R6)>=(R5←0X40000000)
;//(R7←R6)-(R5←0X40000000)相减后低位补0左移2次;(R5)低位补1左移1次
;//步骤2
;//若(R7←R6)<(R5←0X40000000)
;//(R7←R6)低位补0左移2次;(R5)低位补0左移1次
;//循环次数=4*被开方字节数(完成整数位开方)
	PUSH {R2-R7,LR}
	MOV R6,R0

  MOV R7,#0
  MOV R5,#0
  MOV R4,#0X40000000
  MOV R3,#4*4
KPF4_2_1
  SUBS R6,R6,R4
  SBCS R7,R7,R5
  BCS KPF4_2_2;//(R7←R6)>=(R5←0X40000000)
  ADDS R6,R6,R4
  ADCS  R7,R7,R5
  MOV R7,R7,LSL #2;//(R7←R6)低位补0左移2次
  MOV R2,R6
  MOV R2,R2,LSR #30
  ADD R7,R7,R2
  MOV R6,R6,LSL #2
  MOV R5,R5,LSL #1;//(R5)低位补0左移1次
  SUBS R3,R3,#1
  BNE KPF4_2_1
	MOV R0,R5
  POP {R2-R7,PC}
KPF4_2_2;//(R7←R6)>=(R5←0X40000000)
  MOV R7,R7,LSL #2;//(R7←R6)低位补0左移2次
  MOV R2,R6
  MOV R2,R2,LSR #30
  ADD R7,R7,R2
  MOV R6,R6,LSL #2
  MOV R5,R5,LSL #1;//(R5)低位补1左移1次
  MOV R2,#1
  ADD R5,R5,R2
  SUBS R3,R3,#1
  BNE KPF4_2_1
	MOV R0,R5
  POP {R2-R7,PC}
}













