
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
BUZZ_MODE_LIB_LIST;//����ģʽ���
;//��ѭ��1-10
	DCD BUZZ_MODE_LIB1;//����ģʽ��1,������
	DCD BUZZ_MODE_LIB2;//����ģʽ��2,����������
	DCD BUZZ_MODE_LIB3;//����ģʽ��3,����ʱ�α仯��
	DCD 0;//����ģʽ��4
	DCD 0;//����ģʽ��5
	DCD 0;//����ģʽ��6
	DCD 0;//����ģʽ��7
	DCD 0;//����ģʽ��8
	DCD 0;//����ģʽ��9
	DCD 0;//����ģʽ��10
;//ѭ��11-N
	DCD BUZZ_MODE_LIB11;//����ģʽ��11,�޷Ѹ澯��
	DCD BUZZ_MODE_LIB12;//����ģʽ��12,�����բ��
	DCD BUZZ_MODE_LIB13;//����ģʽ��13,���ظ澯��
	DCD BUZZ_MODE_LIB14;//����ģʽ��14,��ظ澯��
	DCD BUZZ_MODE_LIB15;//����ģʽ��15,ң����բ�澯��

;//��ѭ��
BUZZ_MODE_LIB1;//����ģʽ��1,������
	DCB 1;//1BYTE ������1
	DCB 1;//1BYTE ��������(1-8)
	DCB 0;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 50/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 0;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 0;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 0;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 0;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 0;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 0;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 0;//2BYTE 8 ��ʱ��MS(��������=7)
BUZZ_MODE_LIB2;//����ģʽ��2,����������
	DCB 2;//1BYTE ������2
	DCB 3;//1BYTE ��������(1-8)
	DCB 0;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 50/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 300/10;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 50/10;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 0;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 0;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 0;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 0;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 0;//2BYTE 8 ��ʱ��MS(��������=7)
BUZZ_MODE_LIB3;//����ģʽ��3,����ʱ�α仯��
	DCB 3;//1BYTE ������3
	DCB 5;//1BYTE ��������(1-8)
	DCB 0;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 50/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 500/10;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 50/10;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 500/10;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 50/10;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 0;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 0;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 0;//2BYTE 8 ��ʱ��MS(��������=7)



;//ѭ��
BUZZ_MODE_LIB11;//����ģʽ��11,�޷Ѹ澯��
	DCB 11;//1BYTE ������11
	DCB 2;//1BYTE ��������(1-8)
	DCB 1;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 5000/10;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 0;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 0;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 0;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 0;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 0;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 0;//2BYTE 8 ��ʱ��MS(��������=7)
BUZZ_MODE_LIB12;//����ģʽ��12,�����բ��
	DCB 12;//1BYTE ������12
	DCB 2;//1BYTE ��������(1-8)
	DCB 1;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 2000/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 5000/10;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 0;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 0;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 0;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 0;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 0;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 0;//2BYTE 8 ��ʱ��MS(��������=7)
BUZZ_MODE_LIB13;//����ģʽ��13,���ظ澯��
	DCB 13;//1BYTE ������13
	DCB 6;//1BYTE ��������(1-8)
	DCB 1;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 500/10;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 100/10;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 500/10;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 100/10;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 5000/10;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 0;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 0;//2BYTE 8 ��ʱ��MS(��������=7)
BUZZ_MODE_LIB14;//����ģʽ��14,��ظ澯��
	DCB 14;//1BYTE ������14
	DCB 8;//1BYTE ��������(1-8)
	DCB 1;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 500/10;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 100/10;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 500/10;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 100/10;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 500/10;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 100/10;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 5000/10;//2BYTE 8 ��ʱ��MS(��������=7)
BUZZ_MODE_LIB15;//����ģʽ��15,ң����բ�澯��
	DCB 15;//1BYTE ������15
	DCB 8;//1BYTE ��������(1-8)
	DCB 1;//1BYTE ѭ������0=��ѭ��=1ѭ��
	DCB 0;//1BYTE 
	DCW 100/10;//2BYTE 1 ��ʱ��MS(��������=0)
	DCW 200/10;//2BYTE 2 ��ʱ��MS(��������=1)
	DCW 100/10;//2BYTE 3 ��ʱ��MS(��������=2)
	DCW 200/10;//2BYTE 4 ��ʱ��MS(��������=3)
	DCW 100/10;//2BYTE 5 ��ʱ��MS(��������=4)
	DCW 200/10;//2BYTE 6 ��ʱ��MS(��������=5)
	DCW 100/10;//2BYTE 7 ��ʱ��MS(��������=6)
	DCW 1000/10;//2BYTE 8 ��ʱ��MS(��������=7)
}


__asm u32 BUZZTimer(u32 ADDR_BUZZMSTimer)//����0=��,1=��,2=����
{
  PUSH{R1-R12,LR}
;//������
	//LDR R0,=&Comm_Ram->BUZZMSTimer;//R0=������MS��ʱ��
	LDR R1,=ADDR_BUZZCTRL;//R1=BUZZ_CTRL_ADDR
	LDRB R2,[R1]
	CMP R2,#0
	BNE I2_ISR_BUZZ_1;//���в���
;//���е�ǰ
	LDRB R3,[R1,#1]
	CMP R3,#0
	BEQ I2_ISR_BUZZ_0_1;//��ǰ������0=������,���Ƿ��еȴ�����
	LDRB R5,[R1,#3];//���м�������
	ANDS R8,R5,#1
	BEQ I2_ISR_BUZZ_0_1;//ż������,���Ƿ��еȴ�����
I2_ISR_BUZZ_0_3
	LDRH R2,[R0]
	CMP R2,#0
	BNE I2_ISR_BUZZ_END2;//��ʱ��Ϊ0
	CMP R3,#0
	BNE I2_ISR_BUZZ_0_4;//��ǰ������0=������,!=0������
	B I2_ISR_BUZZ_END0
I2_ISR_BUZZ_END2
  MOV R0,#2
  POP{R1-R12,PC}
I2_ISR_BUZZ_0_4
	SUB R3,R3,#1;//������
	MOV R3,R3,LSL #2;//ÿ��ַ4�ֽ�
	LDR R4,=BUZZ_MODE_LIB_LIST;//����ģʽ���
	LDR R4,[R4,R3];//R4=����ģʽ����ַ
	LDRB R6,[R4,#1];//��������
	CMP R5,R6
	BCC I2_ISR_BUZZ_0_5;//û������
	LDRB R7,[R4,#2];//ѭ������0=��ѭ��=1ѭ��
	CMP R7,#0
	BNE I2_ISR_BUZZ_0_6;//ѭ��
	MOV R3,#0
	STRB R3,[R1,#1];//��ǰ������=0
	B I2_ISR_BUZZ_END2
I2_ISR_BUZZ_0_6;//ѭ��
	MOV R5,#0;//���м�������=0
	STRB R5,[R1,#3]
	B I2_ISR_BUZZ_END2
I2_ISR_BUZZ_0_5
	ADD R5,R5,#1
	STRB R5,[R1,#3];//���м�������+1
  MOV R7,#1;//����0=��,1=��
	ANDS R8,R5,#1
	BNE I2_ISR_BUZZ_0_7;//��������
  MOV R7,#0;//����0=��,1=��
I2_ISR_BUZZ_0_7
	;//������ʱ
	ADD R4,R4,#2
	MOV R5,R5,LSL #1;//ÿ��2�ֽ�
	LDRH R6,[R4,R5]
	STRH R6,[R0]
  MOV R0,R7
	B I2_ISR_BUZZ_ENDN
;//BUZZ_CTRL_ADDR     ADDR_RUN_SEEP+8//���������ƻ���,���ֶ���
;//1BYTE 0=���е�ǰ,1=���в���
;//1BYTE ��ǰ������0=������,!=0������
;//1BYTE ��ǰ���в����
;//1BYTE ���м�������
;//1byte �ȴ�����0������0=������,!=0������
;//1byte �ȴ�����1������0=������,!=0������
;//1byte �ȴ�����2������0=������,!=0������
;//1byte �ȴ�����3������0=������,!=0������
I2_ISR_BUZZ_0_1
	MOV R7,#0
	LDRB R2,[R1,#4];//�ȴ�����0
	CMP R2,#0
	BNE I2_ISR_BUZZ_0_2
	MOV R7,#1
	LDRB R2,[R1,#5];//�ȴ�����1
	CMP R2,#0
	BNE I2_ISR_BUZZ_0_2
	MOV R7,#2
	LDRB R2,[R1,#6];//�ȴ�����2
	CMP R2,#0
	BNE I2_ISR_BUZZ_0_2
	MOV R7,#3
	LDRB R2,[R1,#7];//�ȴ�����3
	CMP R2,#0
	BEQ I2_ISR_BUZZ_0_3
I2_ISR_BUZZ_0_2
	STRB R7,[R1,#2];//��ǰ���в����
	MOV R7,#0
	STRB R7,[R1,#3];//���м�������=0
	MOV R7,#1
	STRB R7,[R1,#0];//1=���в���
	MOV R5,#2000/10
	STRH R5,[R0];//����������ʱ2000MS
	B I2_ISR_BUZZ_END0
;//���в���
I2_ISR_BUZZ_1
	LDRH R2,[R0]
	CMP R2,#0
	BNE I2_ISR_BUZZ_END2;//��ʱ��Ϊ0
	LDRB R2,[R1,#2];//R2=��ǰ���в����0-3
	ADD R2,R2,#4;//R2=��ǰ���в����4-7
	LDRB R3,[R1,R2];//R3=��ǰ���в��������
	CMP R3,#0
	BNE I2_ISR_BUZZ_1_1
	;//������=0
	STRB R3,[R1];//0=���е�ǰ
	STRB R3,[R1,#3];//���м�������=0
	B I2_ISR_BUZZ_END2
I2_ISR_BUZZ_1_1
	SUB R3,R3,#1;//������
	MOV R3,R3,LSL #2;//ÿ��ַ4�ֽ�
	LDR R4,=BUZZ_MODE_LIB_LIST;//����ģʽ���
	LDR R4,[R4,R3];//R4=����ģʽ����ַ
	LDRB R5,[R1,#3];//���м�������
	LDRB R6,[R4,#1];//��������
	CMP R5,R6
	BCC I2_ISR_BUZZ_1_2;//û������
	;//������,�������ǲ�ѭ����,����������ǹط�����,��������ط�������ʱ
	MOV R3,#0
	STRB R3,[R1];//0=���е�ǰ
	STRB R3,[R1,#3];//���м�������=0
	STRB R3,[R1,R2];//��ǰ���в��������=0
	MOV R3,#2000/10
	STRH R3,[R0];//��������ط�������ʱ2000MS
	B I2_ISR_BUZZ_END0
I2_ISR_BUZZ_1_2;//û������
	ADD R5,R5,#1
	STRB R5,[R1,#3];//���м�������+1
  MOV R7,#1;//����0=��,1=��
	ANDS R8,R5,#1
	BNE I2_ISR_BUZZ_1_3;//��������
  MOV R7,#0;//����0=��,1=��
I2_ISR_BUZZ_1_3
	;//������ʱ
	ADD R4,R4,#2
	MOV R5,R5,LSL #1;//ÿ��2�ֽ�
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
	Comm_Ram->Init_OS|=1<<5;//OS������ԭ���־:B0=�������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=HardFault,B6=,B7=TEST
#if DEBUG==1//0=����,1=����
	DisplayStringAutoUp(0,9,(u8*)"\x0""�Ƿ�ָ��");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
#endif
}

/*
//���ص��Ƿ���
__asm void HardFault_Handler(void)
{
	PRESERVE8
	extern ReSTART_OS;
	extern BAD_Display;
	extern WWDT_RESET;//����ʹ�ܿ��Ź���λ
	
	LDR R0,=0xE000ED28;//fault�Ĵ���
	LDR R1,[R0]
	STR R1,[R0];//дλ1ֵ��0
	LDR R1,[R0]
	LDR R0,=0xE000ED2C;//Ӳfault ״̬�Ĵ�����HFSR��
	LDR R1,[R0]
	STR R1,[R0];//дλ1ֵ��0
	
	MOV R4,LR
	BL BAD_Display
	MOV LR,R4
;//	BL WWDT_RESET;//����ʹ�ܿ��Ź���λ

	TST LR, #0x4 ;// ����EXC_RETURN��B2
	ITE EQ ;// ���Ϊ0,
	MRSEQ R0, MSP ;// ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ��
	MRSNE R0, PSP ;// ����, ʹ�õ��ǽ��̶�ջ���ʰ�PSP��ֵȡ��

	LDR R1,=0x01000000;//T=1(Thumb)
	STR R1,[R0,#28];//����ջ��xPSR��ֵ,ǿ��ΪThumb״̬

//	LDR R2,=0xE000ED0C;//Ӧ�ó����жϼ���λ���ƼĴ���(AIRCR) 0xE000_ED0C
////31:16 VECTKEY RW �\ ����Կ�ף��κζԸüĴ�����д������������ͬʱ��0x05FA д��˶Σ�����д���������ԡ�����ȡ�˰��֣���0xFA05
////15 ENDIANESS R �\ ָʾ�����á�1�����(BE8)��0��С�ˡ���ֵ���ڸ�λʱȷ���ģ����ܸ��ġ�
////10:8 PRIGROUP R/W 0 ���ȼ�����
////2 SYSRESETREQ W �\ ����оƬ�����߼�����һ�θ�λ
////1 VECTCLRACTIVE W �\ ���������쳣�Ļ״̬��Ϣ��ͨ��ֻ�ڵ���ʱ�ã�������OS �Ӵ����лָ�ʱ��
////0 VECTRESET W �\ ��λCM3 �������ںˣ������߼����⣩�����Ǵ˸�λ��Ӱ��оƬ�����ں�����ĵ�·
//	LDR R3,=0x05FA0001;//���������쳣�Ļ״̬��Ϣ��ͨ��ֻ�ڵ���ʱ�ã�������OS �Ӵ����лָ�ʱ��
//	STR R3,[R2]
	
//EXC_RETURNλ�����
//[31:4] EXC_RETURN�ı�ʶ������ȫΪ1
//3 0=���غ����Handlerģʽ,1=���غ�����߳�ģʽ
//2 0=������ջ������ջ���������غ�ʹ��MSP��1=�ӽ��̶�ջ������ջ���������غ�ʹ��PSP
//1 ����������Ϊ0
//0 0=����ARM״̬��1=����Thumb״̬����CM3�б���Ϊ1
	ORR LR,LR,#9;//���غ�����߳�ģʽ
	BX LR;
	ALIGN
}
*/

//����ȥ����OS
__asm void HardFault_Handler(void)
{
	PRESERVE8
	extern ReSTART_OS;
	extern BAD_Display;
	extern WWDT_RESET;//����ʹ�ܿ��Ź���λ
	
	LDR R0,=0xE000ED28;//fault�Ĵ���
	LDR R1,[R0]
	STR R1,[R0];//дλ1ֵ��0
	LDR R1,[R0]
//	LDR R0,=0xE000ED2C;//Ӳfault ״̬�Ĵ�����HFSR��
//	LDR R1,[R0]
//	STR R1,[R0];//дλ1ֵ��0
	
	LDR R0,=ADDR_SYSTEM+32;//OS������ԭ���־:B0=�������,B1=FS,B2=USB0,B3=USB1,B4=ETH,B5=,B6=,B7=����
	LDRB R0,[R0]
	CMP R0,#0
	BNE ReSTARTOS;//������OS����ʧ��
	LDR R0,=ADDR_SYSTEM+31;//(���ȡ��ַ)��ǰ���еĳ����(����HardFault�ж�ʱ����OSʶ��):0=��,1=FS,2=USB0,3=USB1,4=ETH,5=...
	LDRB R0,[R0]
	CMP R0,#2;
	BEQ ReSTARTOS;//��USB0����ʧ��
	
;//	B .;//����ԭ�����ʧ��
	
ReSTARTOS
#ifdef IC_STM32F4xx
	MOV R4,LR
	BL BAD_Display;//OS������ԭ���־
	MOV LR,R4
;//	BL WWDT_RESET;//����ʹ�ܿ��Ź���λ

	TST LR, #0x4 ;// ����EXC_RETURN��B2
	ITE EQ ;// ���Ϊ0,
	MRSEQ R0, MSP ;// ��ʹ�õ�������ջ���ʰ�MSP��ֵȡ��
	MRSNE R0, PSP ;// ����, ʹ�õ��ǽ��̶�ջ���ʰ�PSP��ֵȡ��
	LDR R1,=RET;//�жϷ��غ�Ҫת��ĵ�ַ
	STR R1,[R0,#24] ;// ����ջ��PC��ֵ
	LDR R1,=0x01000000;//T=1(Thumb)
	STR R1,[R0,#28];//����ջ��xPSR��ֵ,ǿ��ΪThumb״̬
	
//	LDR R2,=0xE000ED0C;//Ӧ�ó����жϼ���λ���ƼĴ���(AIRCR) 0xE000_ED0C
////31:16 VECTKEY RW �\ ����Կ�ף��κζԸüĴ�����д������������ͬʱ��0x05FA д��˶Σ�����д���������ԡ�����ȡ�˰��֣���0xFA05
////15 ENDIANESS R �\ ָʾ�����á�1�����(BE8)��0��С�ˡ���ֵ���ڸ�λʱȷ���ģ����ܸ��ġ�
////10:8 PRIGROUP R/W 0 ���ȼ�����
////2 SYSRESETREQ W �\ ����оƬ�����߼�����һ�θ�λ
////1 VECTCLRACTIVE W �\ ���������쳣�Ļ״̬��Ϣ��ͨ��ֻ�ڵ���ʱ�ã�������OS �Ӵ����лָ�ʱ��
////0 VECTRESET W �\ ��λCM3 �������ںˣ������߼����⣩�����Ǵ˸�λ��Ӱ��оƬ�����ں�����ĵ�·
//	LDR R3,=0x05FA0001;//���������쳣�Ļ״̬��Ϣ��ͨ��ֻ�ڵ���ʱ�ã�������OS �Ӵ����лָ�ʱ��
//	STR R3,[R2]
	
//EXC_RETURNλ�����
//[31:4] EXC_RETURN�ı�ʶ������ȫΪ1
//3 0=���غ����Handlerģʽ,1=���غ�����߳�ģʽ
//2 0=������ջ������ջ���������غ�ʹ��MSP��1=�ӽ��̶�ջ������ջ���������غ�ʹ��PSP
//1 ����������Ϊ0
//0 0=����ARM״̬��1=����Thumb״̬����CM3�б���Ϊ1
	
	ORR LR,LR,#9;//���غ�����߳�ģʽ
	BX LR;
#endif
RET
	LDR R0,=ReSTART_OS;//����OS
	BX R0;
	ALIGN
}







