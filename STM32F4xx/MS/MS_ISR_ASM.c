
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../STM32F4xx/STM32F4xx_IO.h"
#include "../Display/Display.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"


__asm void BUZZLib(void)
{
BUZZ_MODE_LIB_LIST;//蜂鸣模式库表
;//不循环1-10
	DCD BUZZ_MODE_LIB1;//蜂鸣模式库1,按键声
	DCD BUZZ_MODE_LIB2;//蜂鸣模式库2,参数设置声
	DCD BUZZ_MODE_LIB3;//蜂鸣模式库3,控制时段变化声
	DCD 0;//蜂鸣模式库4
	DCD 0;//蜂鸣模式库5
	DCD 0;//蜂鸣模式库6
	DCD 0;//蜂鸣模式库7
	DCD 0;//蜂鸣模式库8
	DCD 0;//蜂鸣模式库9
	DCD 0;//蜂鸣模式库10
;//循环11-N
	DCD BUZZ_MODE_LIB11;//蜂鸣模式库11,崔费告警声
	DCD BUZZ_MODE_LIB12;//蜂鸣模式库12,允许合闸声
	DCD BUZZ_MODE_LIB13;//蜂鸣模式库13,功控告警声
	DCD BUZZ_MODE_LIB14;//蜂鸣模式库14,电控告警声
	DCD BUZZ_MODE_LIB15;//蜂鸣模式库15,遥控跳闸告警声

;//不循环
BUZZ_MODE_LIB1;//蜂鸣模式库1,按键声
	DCB 1;//1BYTE 蜂鸣号1
	DCB 1;//1BYTE 蜂鸣级数(1-8)
	DCB 0;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 50/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 0;//2BYTE 2 关时间MS(级数计数=1)
	DCW 0;//2BYTE 3 开时间MS(级数计数=2)
	DCW 0;//2BYTE 4 关时间MS(级数计数=3)
	DCW 0;//2BYTE 5 开时间MS(级数计数=4)
	DCW 0;//2BYTE 6 关时间MS(级数计数=5)
	DCW 0;//2BYTE 7 开时间MS(级数计数=6)
	DCW 0;//2BYTE 8 关时间MS(级数计数=7)
BUZZ_MODE_LIB2;//蜂鸣模式库2,参数设置声
	DCB 2;//1BYTE 蜂鸣号2
	DCB 3;//1BYTE 蜂鸣级数(1-8)
	DCB 0;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 50/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 300/10;//2BYTE 2 关时间MS(级数计数=1)
	DCW 50/10;//2BYTE 3 开时间MS(级数计数=2)
	DCW 0;//2BYTE 4 关时间MS(级数计数=3)
	DCW 0;//2BYTE 5 开时间MS(级数计数=4)
	DCW 0;//2BYTE 6 关时间MS(级数计数=5)
	DCW 0;//2BYTE 7 开时间MS(级数计数=6)
	DCW 0;//2BYTE 8 关时间MS(级数计数=7)
BUZZ_MODE_LIB3;//蜂鸣模式库3,控制时段变化声
	DCB 3;//1BYTE 蜂鸣号3
	DCB 5;//1BYTE 蜂鸣级数(1-8)
	DCB 0;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 50/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 500/10;//2BYTE 2 关时间MS(级数计数=1)
	DCW 50/10;//2BYTE 3 开时间MS(级数计数=2)
	DCW 500/10;//2BYTE 4 关时间MS(级数计数=3)
	DCW 50/10;//2BYTE 5 开时间MS(级数计数=4)
	DCW 0;//2BYTE 6 关时间MS(级数计数=5)
	DCW 0;//2BYTE 7 开时间MS(级数计数=6)
	DCW 0;//2BYTE 8 关时间MS(级数计数=7)



;//循环
BUZZ_MODE_LIB11;//蜂鸣模式库11,崔费告警声
	DCB 11;//1BYTE 蜂鸣号11
	DCB 2;//1BYTE 蜂鸣级数(1-8)
	DCB 1;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 5000/10;//2BYTE 2 关时间MS(级数计数=1)
	DCW 0;//2BYTE 3 开时间MS(级数计数=2)
	DCW 0;//2BYTE 4 关时间MS(级数计数=3)
	DCW 0;//2BYTE 5 开时间MS(级数计数=4)
	DCW 0;//2BYTE 6 关时间MS(级数计数=5)
	DCW 0;//2BYTE 7 开时间MS(级数计数=6)
	DCW 0;//2BYTE 8 关时间MS(级数计数=7)
BUZZ_MODE_LIB12;//蜂鸣模式库12,允许合闸声
	DCB 12;//1BYTE 蜂鸣号12
	DCB 2;//1BYTE 蜂鸣级数(1-8)
	DCB 1;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 2000/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 5000/10;//2BYTE 2 关时间MS(级数计数=1)
	DCW 0;//2BYTE 3 开时间MS(级数计数=2)
	DCW 0;//2BYTE 4 关时间MS(级数计数=3)
	DCW 0;//2BYTE 5 开时间MS(级数计数=4)
	DCW 0;//2BYTE 6 关时间MS(级数计数=5)
	DCW 0;//2BYTE 7 开时间MS(级数计数=6)
	DCW 0;//2BYTE 8 关时间MS(级数计数=7)
BUZZ_MODE_LIB13;//蜂鸣模式库13,功控告警声
	DCB 13;//1BYTE 蜂鸣号13
	DCB 6;//1BYTE 蜂鸣级数(1-8)
	DCB 1;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 500/10;//2BYTE 2 关时间MS(级数计数=1)
	DCW 100/10;//2BYTE 3 开时间MS(级数计数=2)
	DCW 500/10;//2BYTE 4 关时间MS(级数计数=3)
	DCW 100/10;//2BYTE 5 开时间MS(级数计数=4)
	DCW 5000/10;//2BYTE 6 关时间MS(级数计数=5)
	DCW 0;//2BYTE 7 开时间MS(级数计数=6)
	DCW 0;//2BYTE 8 关时间MS(级数计数=7)
BUZZ_MODE_LIB14;//蜂鸣模式库14,电控告警声
	DCB 14;//1BYTE 蜂鸣号14
	DCB 8;//1BYTE 蜂鸣级数(1-8)
	DCB 1;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 500/10;//2BYTE 2 关时间MS(级数计数=1)
	DCW 100/10;//2BYTE 3 开时间MS(级数计数=2)
	DCW 500/10;//2BYTE 4 关时间MS(级数计数=3)
	DCW 100/10;//2BYTE 5 开时间MS(级数计数=4)
	DCW 500/10;//2BYTE 6 关时间MS(级数计数=5)
	DCW 100/10;//2BYTE 7 开时间MS(级数计数=6)
	DCW 5000/10;//2BYTE 8 关时间MS(级数计数=7)
BUZZ_MODE_LIB15;//蜂鸣模式库15,遥控跳闸告警声
	DCB 15;//1BYTE 蜂鸣号15
	DCB 8;//1BYTE 蜂鸣级数(1-8)
	DCB 1;//1BYTE 循环控制0=不循环=1循环
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 开时间MS(级数计数=0)
	DCW 200/10;//2BYTE 2 关时间MS(级数计数=1)
	DCW 100/10;//2BYTE 3 开时间MS(级数计数=2)
	DCW 200/10;//2BYTE 4 关时间MS(级数计数=3)
	DCW 100/10;//2BYTE 5 开时间MS(级数计数=4)
	DCW 200/10;//2BYTE 6 关时间MS(级数计数=5)
	DCW 100/10;//2BYTE 7 开时间MS(级数计数=6)
	DCW 1000/10;//2BYTE 8 关时间MS(级数计数=7)
}


__asm u32 BUZZTimer(u32 ADDR_BUZZMSTimer)//返回0=关,1=开,2=不变
{
  PUSH{R1-R12,LR}
;//蜂鸣器
	//LDR R0,=&Comm_Ram->BUZZMSTimer;//R0=蜂鸣器MS定时器
	LDR R1,=ADDR_BUZZCTRL;//R1=BUZZ_CTRL_ADDR
	LDRB R2,[R1]
	CMP R2,#0
	BNE I2_ISR_BUZZ_1;//运行插入
;//运行当前
	LDRB R3,[R1,#1]
	CMP R3,#0
	BEQ I2_ISR_BUZZ_0_1;//当前蜂鸣号0=不蜂鸣,判是否有等待插入
	LDRB R5,[R1,#3];//运行级数计数
	ANDS R8,R5,#1
	BEQ I2_ISR_BUZZ_0_1;//偶数级关,判是否有等待插入
I2_ISR_BUZZ_0_3
	LDRH R2,[R0]
	CMP R2,#0
	BNE I2_ISR_BUZZ_END2;//定时不为0
	CMP R3,#0
	BNE I2_ISR_BUZZ_0_4;//当前蜂鸣号0=不蜂鸣,!=0蜂鸣号
	B I2_ISR_BUZZ_END0
I2_ISR_BUZZ_END2
  MOV R0,#2
  POP{R1-R12,PC}
I2_ISR_BUZZ_0_4
	SUB R3,R3,#1;//蜂鸣号
	MOV R3,R3,LSL #2;//每地址4字节
	LDR R4,=BUZZ_MODE_LIB_LIST;//蜂鸣模式库表
	LDR R4,[R4,R3];//R4=蜂鸣模式库首址
	LDRB R6,[R4,#1];//蜂鸣级数
	CMP R5,R6
	BCC I2_ISR_BUZZ_0_5;//没到级数
	LDRB R7,[R4,#2];//循环控制0=不循环=1循环
	CMP R7,#0
	BNE I2_ISR_BUZZ_0_6;//循环
	MOV R3,#0
	STRB R3,[R1,#1];//当前蜂鸣号=0
	B I2_ISR_BUZZ_END2
I2_ISR_BUZZ_0_6;//循环
	MOV R5,#0;//运行级数计数=0
	STRB R5,[R1,#3]
	B I2_ISR_BUZZ_END2
I2_ISR_BUZZ_0_5
	ADD R5,R5,#1
	STRB R5,[R1,#3];//运行级数计数+1
  MOV R7,#1;//返回0=关,1=开
	ANDS R8,R5,#1
	BNE I2_ISR_BUZZ_0_7;//奇数级开
  MOV R7,#0;//返回0=关,1=开
I2_ISR_BUZZ_0_7
	;//本级定时
	ADD R4,R4,#2
	MOV R5,R5,LSL #1;//每级2字节
	LDRH R6,[R4,R5]
	STRH R6,[R0]
  MOV R0,R7
	B I2_ISR_BUZZ_ENDN
;//BUZZ_CTRL_ADDR     ADDR_RUN_SEEP+8//蜂鸣器控制缓冲,半字对齐
;//1BYTE 0=运行当前,1=运行插入
;//1BYTE 当前蜂鸣号0=不蜂鸣,!=0蜂鸣号
;//1BYTE 当前运行插入号
;//1BYTE 运行级数计数
;//1byte 等待插入0蜂鸣号0=不蜂鸣,!=0蜂鸣号
;//1byte 等待插入1蜂鸣号0=不蜂鸣,!=0蜂鸣号
;//1byte 等待插入2蜂鸣号0=不蜂鸣,!=0蜂鸣号
;//1byte 等待插入3蜂鸣号0=不蜂鸣,!=0蜂鸣号
I2_ISR_BUZZ_0_1
	MOV R7,#0
	LDRB R2,[R1,#4];//等待插入0
	CMP R2,#0
	BNE I2_ISR_BUZZ_0_2
	MOV R7,#1
	LDRB R2,[R1,#5];//等待插入1
	CMP R2,#0
	BNE I2_ISR_BUZZ_0_2
	MOV R7,#2
	LDRB R2,[R1,#6];//等待插入2
	CMP R2,#0
	BNE I2_ISR_BUZZ_0_2
	MOV R7,#3
	LDRB R2,[R1,#7];//等待插入3
	CMP R2,#0
	BEQ I2_ISR_BUZZ_0_3
I2_ISR_BUZZ_0_2
	STRB R7,[R1,#2];//当前运行插入号
	MOV R7,#0
	STRB R7,[R1,#3];//运行级数计数=0
	MOV R7,#1
	STRB R7,[R1,#0];//1=运行插入
	MOV R5,#2000/10
	STRH R5,[R0];//插入关输出定时2000MS
	B I2_ISR_BUZZ_END0
;//运行插入
I2_ISR_BUZZ_1
	LDRH R2,[R0]
	CMP R2,#0
	BNE I2_ISR_BUZZ_END2;//定时不为0
	LDRB R2,[R1,#2];//R2=当前运行插入号0-3
	ADD R2,R2,#4;//R2=当前运行插入号4-7
	LDRB R3,[R1,R2];//R3=当前运行插入蜂鸣号
	CMP R3,#0
	BNE I2_ISR_BUZZ_1_1
	;//蜂鸣号=0
	STRB R3,[R1];//0=运行当前
	STRB R3,[R1,#3];//运行级数计数=0
	B I2_ISR_BUZZ_END2
I2_ISR_BUZZ_1_1
	SUB R3,R3,#1;//蜂鸣号
	MOV R3,R3,LSL #2;//每地址4字节
	LDR R4,=BUZZ_MODE_LIB_LIST;//蜂鸣模式库表
	LDR R4,[R4,R3];//R4=蜂鸣模式库首址
	LDRB R5,[R1,#3];//运行级数计数
	LDRB R6,[R4,#1];//蜂鸣级数
	CMP R5,R6
	BCC I2_ISR_BUZZ_1_2;//没到级数
	;//到级数,插入总是不循环的,插入结束总是关蜂鸣器,插入结束关蜂鸣器延时
	MOV R3,#0
	STRB R3,[R1];//0=运行当前
	STRB R3,[R1,#3];//运行级数计数=0
	STRB R3,[R1,R2];//当前运行插入蜂鸣号=0
	MOV R3,#2000/10
	STRH R3,[R0];//插入结束关蜂鸣器延时2000MS
	B I2_ISR_BUZZ_END0
I2_ISR_BUZZ_1_2;//没到级数
	ADD R5,R5,#1
	STRB R5,[R1,#3];//运行级数计数+1
  MOV R7,#1;//返回0=关,1=开
	ANDS R8,R5,#1
	BNE I2_ISR_BUZZ_1_3;//奇数级开
  MOV R7,#0;//返回0=关,1=开
I2_ISR_BUZZ_1_3
	;//本级定时
	ADD R4,R4,#2
	MOV R5,R5,LSL #1;//每级2字节
	LDRH R6,[R4,R5]
	STRH R6,[R0]
  MOV R0,R7
I2_ISR_BUZZ_ENDN
  POP{R1-R12,PC}
I2_ISR_BUZZ_END0
  MOV R0,#0
  POP{R1-R12,PC}
I2_ISR_BUZZ_END1
  MOV R0,#1
  POP{R1-R12,PC}

}


void BAD_Display(void)
{
	Comm_Ram->Init_OS|=1<<5;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=HardFault,B6=,B7=TEST
#if DEBUG==1//0=发行,1=调试
	DisplayStringAutoUp(0,9,(u8*)"\x0""非法指令");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
#endif
}

/*
//返回到非法处
__asm void HardFault_Handler(void)
{
	PRESERVE8
	extern ReSTART_OS;
	extern BAD_Display;
	extern WWDT_RESET;//立即使能看门狗复位
	
	LDR R0,=0xE000ED28;//fault寄存器
	LDR R1,[R0]
	STR R1,[R0];//写位1值清0
	LDR R1,[R0]
	LDR R0,=0xE000ED2C;//硬fault 状态寄存器（HFSR）
	LDR R1,[R0]
	STR R1,[R0];//写位1值清0
	
	MOV R4,LR
	BL BAD_Display
	MOV LR,R4
;//	BL WWDT_RESET;//立即使能看门狗复位

	TST LR, #0x4 ;// 测试EXC_RETURN的B2
	ITE EQ ;// 如果为0,
	MRSEQ R0, MSP ;// 则使用的是主堆栈，故把MSP的值取出
	MRSNE R0, PSP ;// 否则, 使用的是进程堆栈，故把PSP的值取出

	LDR R1,=0x01000000;//T=1(Thumb)
	STR R1,[R0,#28];//设置栈中xPSR的值,强制为Thumb状态

//	LDR R2,=0xE000ED0C;//应用程序中断及复位控制寄存器(AIRCR) 0xE000_ED0C
////31:16 VECTKEY RW ‐ 访问钥匙：任何对该寄存器的写操作，都必须同时把0x05FA 写入此段，否则写操作被忽略。若读取此半字，则0xFA05
////15 ENDIANESS R ‐ 指示端设置。1＝大端(BE8)，0＝小端。此值是在复位时确定的，不能更改。
////10:8 PRIGROUP R/W 0 优先级分组
////2 SYSRESETREQ W ‐ 请求芯片控制逻辑产生一次复位
////1 VECTCLRACTIVE W ‐ 清零所有异常的活动状态信息。通常只在调试时用，或者在OS 从错误中恢复时用
////0 VECTRESET W ‐ 复位CM3 处理器内核（调试逻辑除外），但是此复位不影响芯片上在内核以外的电路
//	LDR R3,=0x05FA0001;//清零所有异常的活动状态信息。通常只在调试时用，或者在OS 从错误中恢复时用
//	STR R3,[R2]
	
//EXC_RETURN位段详解
//[31:4] EXC_RETURN的标识：必须全为1
//3 0=返回后进入Handler模式,1=返回后进入线程模式
//2 0=从主堆栈中做出栈操作，返回后使用MSP，1=从进程堆栈中做出栈操作，返回后使用PSP
//1 保留，必须为0
//0 0=返回ARM状态。1=返回Thumb状态。在CM3中必须为1
	ORR LR,LR,#9;//返回后进入线程模式
	BX LR;
	ALIGN
}
*/

//返回去重启OS
__asm void HardFault_Handler(void)
{
	PRESERVE8
	extern ReSTART_OS;
	extern BAD_Display;
	extern WWDT_RESET;//立即使能看门狗复位
	
	LDR R0,=0xE000ED28;//fault寄存器
	LDR R1,[R0]
	STR R1,[R0];//写位1值清0
	LDR R1,[R0]
//	LDR R0,=0xE000ED2C;//硬fault 状态寄存器（HFSR）
//	LDR R1,[R0]
//	STR R1,[R0];//写位1值清0
	
	LDR R0,=ADDR_SYSTEM+32;//OS需重启原因标志:B0=软件更新,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=其他
	LDRB R0,[R0]
	CMP R0,#0
	BNE ReSTARTOS;//重启动OS操作失败
	LDR R0,=ADDR_SYSTEM+31;//(汇编取地址)当前运行的程序段(用于HardFault中断时重启OS识别):0=空,1=FS,2=USB0,3=USB1,4=ETH,5=...
	LDRB R0,[R0]
	CMP R0,#2;
	BEQ ReSTARTOS;//是USB0操作失败
	
;//	B .;//其他原因操作失败
	
ReSTARTOS
#ifdef IC_STM32F4xx
	MOV R4,LR
	BL BAD_Display;//OS需重启原因标志
	MOV LR,R4
;//	BL WWDT_RESET;//立即使能看门狗复位

	TST LR, #0x4 ;// 测试EXC_RETURN的B2
	ITE EQ ;// 如果为0,
	MRSEQ R0, MSP ;// 则使用的是主堆栈，故把MSP的值取出
	MRSNE R0, PSP ;// 否则, 使用的是进程堆栈，故把PSP的值取出
	LDR R1,=RET;//中断返回后要转向的地址
	STR R1,[R0,#24] ;// 设置栈中PC的值
	LDR R1,=0x01000000;//T=1(Thumb)
	STR R1,[R0,#28];//设置栈中xPSR的值,强制为Thumb状态
	
//	LDR R2,=0xE000ED0C;//应用程序中断及复位控制寄存器(AIRCR) 0xE000_ED0C
////31:16 VECTKEY RW ‐ 访问钥匙：任何对该寄存器的写操作，都必须同时把0x05FA 写入此段，否则写操作被忽略。若读取此半字，则0xFA05
////15 ENDIANESS R ‐ 指示端设置。1＝大端(BE8)，0＝小端。此值是在复位时确定的，不能更改。
////10:8 PRIGROUP R/W 0 优先级分组
////2 SYSRESETREQ W ‐ 请求芯片控制逻辑产生一次复位
////1 VECTCLRACTIVE W ‐ 清零所有异常的活动状态信息。通常只在调试时用，或者在OS 从错误中恢复时用
////0 VECTRESET W ‐ 复位CM3 处理器内核（调试逻辑除外），但是此复位不影响芯片上在内核以外的电路
//	LDR R3,=0x05FA0001;//清零所有异常的活动状态信息。通常只在调试时用，或者在OS 从错误中恢复时用
//	STR R3,[R2]
	
//EXC_RETURN位段详解
//[31:4] EXC_RETURN的标识：必须全为1
//3 0=返回后进入Handler模式,1=返回后进入线程模式
//2 0=从主堆栈中做出栈操作，返回后使用MSP，1=从进程堆栈中做出栈操作，返回后使用PSP
//1 保留，必须为0
//0 0=返回ARM状态。1=返回Thumb状态。在CM3中必须为1
	
	ORR LR,LR,#9;//返回后进入线程模式
	BX LR;
#endif
RET
	LDR R0,=ReSTART_OS;//重启OS
	BX R0;
	ALIGN
}







