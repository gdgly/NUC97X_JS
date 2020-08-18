
// ��ͷ�ļ�ֻ��������ERAMʱ��ʹ�õ�����
#ifndef IRAM_TerminalDisplay_H
#define IRAM_TerminalDisplay_H

#define ADDR_SystemStack     ADDR_Terminal_IRAM_Start		// ���ò˵����ģ��ǰ��ջָ��(�ֶ���),��Ϊ0ֵ��ʾ���ڵ��ò˵�
// 4byte
#define ADDR_MenuStack     (ADDR_SystemStack+4)
// �˵���ջ
typedef __packed struct
{
  u8 Count;		//�˵���ջ����
  u8 Res1;
  u8 Res2;
  u8 Res3;
  u32 v1;		// �˵��ñ���1
  u32 v2;		// �˵��ñ���2
  u32 v3;		// �˵��ñ���3
}MenuCount_TypeDef;
#define MenuCount     ((MenuCount_TypeDef *)(ADDR_MenuStack))
#define MaxMenuCount     10		// ���˵�����
typedef __packed struct
{
  u32 PC;						// �˵��������е�ַ(�ر�ע��:��ֵ�������Ķ�)
  u8 Task;						// �˵��������(������˳�ʱ����Ϊ0)
  u8 MoveRow;					// �˵���������(����ʱ��Ϊ0,����ʱ����)
  u8 EditRow;					// �˵���ǰ���ڱ༭�к�(0-9)(����ʱ��Ϊ1,����ʱ����)
  u8 EditCol;					// �˵���ǰ���ڱ༭�к�(0-19)(����ʱ��Ϊ0,����ʱ����)
  u8 TotalRow;					// �˵�������
  u8 MenuRow;					// �˵���ǰװ������
  u8 res2;
  u8 res3;
}MenuSave_TypeDef;
typedef __packed struct
{
  MenuSave_TypeDef     MenuStack[MaxMenuCount+1];
}MenuStack_TypeDef;
#define Stack     ((MenuStack_TypeDef *)(ADDR_MenuStack+16))
#define ADDR_MenuStack_End     (ADDR_MenuStack+16+(12*(MaxMenuCount+1)))

//������ʾ�ϳ�ģʽ
#define ADDR_DOTMIXPMODE     (ADDR_MenuStack_End)
typedef __packed struct
{
	u8 Task;					// ��ǰ��ʾ����:0=�ϵ���Ļ,1=USB������ʾ,2=������ʾ,3=�˵���ʾ,4=ѭ����ʾ
	u8 Mix;						// ���󻺳�A��B�ϳɷ�ʽ:0=A,1=B,2=A|B,3=A^B,4=A&B
	u8 FlashMode;				// ����ģʽ:0=��,1=����,2=����,3=����,4=����,5=...
	u8 FlashDelay;				// �����仯��ʱ(��λ10ms),0���,255����
	u8 FlashTask;				// ��������:0=��,1=��ʼ,2=����
	u8 FlashCount;				// �������ż���
	u8 res1;
	u8 res2;
}DOTMIXPMODE_TypeDef;
#define DOTMIXPMODE     ((DOTMIXPMODE_TypeDef *)(ADDR_DOTMIXPMODE))
//ѭ����ʾ����
#define ADDR_TerminalDisplayLoop     (ADDR_DOTMIXPMODE+8)
typedef __packed struct
{
	u8 Task;					// ��ǰѭ����ʾ����
	u8 SubTask;					// ��ǰѭ����ʾ������(�ݲ���)
	u8 Count;					// ��ǰѭ����ʾ��������������
	u8 ButtonVal;				// ����״̬�Ĵ������Զ���Ѱ����
}TerminalDisplayLoop_TypeDef;
#define TerminalDisplayLoop     ((TerminalDisplayLoop_TypeDef *)(ADDR_TerminalDisplayLoop))

// �������ݻ���
#define ADDR_DOTDATA_START     (ADDR_TerminalDisplayLoop+4)
#define LEN_DOTDATABUFF     9600		//�������ݻ��峤��
#define ADDR_DOTDATABUFF     ADDR_DOTDATA_START
// 320*240/8=9600�ֽڵ������ݻ���1
#define ADDR_DOTDATABUFF2     (ADDR_DOTDATABUFF+LEN_DOTDATABUFF)
// 320*240/8=9600�ֽڵ������ݻ���2
#define ADDR_STRINGBUFF     (ADDR_DOTDATABUFF2+LEN_DOTDATABUFF)
// �ַ�������1;ÿ��84�ֽ�(4�ֽڿ���,80�ֽ�����)
typedef __packed struct
{
  u8 DotLn;				// �����м�����(0-255)
  u8 Row;				// �ַ�����(0-9)(1���ַ��������Ͽɷֶ�����ʾ)
  u8 Font;				// ���������0-4
  u8 Align;				// ���뷽ʽ:0=�޶���,1=�����,2=����,3=�Ҷ���
  u16 Col[40];			// ��ʾ����
						// ��ʾ����ÿ2�ֽڴ���8*16����,ÿ����ʾ����80�ֽ�40������
						// B15=0����,B15=1����
						// B14=0 B13=0,���Խ�С�ֿ�ȡ����,����ſռ�0-0x2000=8192
						// B14=0 B13=1,B12-B0=0-255��ASCII8*16��׼�ֿ�ȡ����,����ſռ�=256
						// B14=0 B13=1,B12-B0=256-511��ASCII8*12��׼ȡ����(�ݲ���),����ſռ�=256
						// B14=0 B13=1,B12-B0=512-767��ASCII8*8��׼ȡ����(�ݲ���),����ſռ�=256
						// B14=1 B13-B0,��ʾ��GB2312��׼�ֿ�(�޸ĺ�)ȡ����,����ſռ�0-0x4000
}StrLnBuff_TypeDef;
typedef __packed struct
{
  StrLnBuff_TypeDef Row[16];
}StrBuff_TypeDef;
#define StrBuff     ((StrBuff_TypeDef *)(ADDR_STRINGBUFF))
//�ַ�������2;ÿ��84�ֽ�(4�ֽڿ���,80�ֽ�����)
#define ADDR_STRINGBUFF2     (ADDR_STRINGBUFF+(84*16))
#define StrBuff2     ((StrBuff_TypeDef *)(ADDR_STRINGBUFF2))

#define ADDR_WarningStringBuff     (ADDR_STRINGBUFF2+(84*16))
#define MaxWarningString     8	//���澯�ַ�������
// 8���澯�ַ���������,ÿ��84�ֽ�(2�ֽڸ澯����,2�����ֽ�,80�ַ�)
// 2BYTE �澯����,0��ʾ�޸澯
// 2BYTE ����
// 80BYTE �澯�ַ�������
	
#define ADDR_MenuICO     (ADDR_WarningStringBuff+(84*MaxWarningString))
// 1byte �˵�����ʾ���к͵���ͼ�� 0=��,1=��
#define ADDR_AutoDisplayRowCol     ADDR_MenuICO+1 //�Զ�˳����ʾʱ�ĵ�ǰ��ʾ�к��к�;��void DisplayClr(void)//��ȫ��ʱͬʱ��0
// 1byte Row��
// 1byte Col��
typedef __packed struct
{
  u8 Row;
  u8 Col;
}AutoDisplayRowCol_TypeDef;
#define AutoDisplayRowCol     ((AutoDisplayRowCol_TypeDef *)(ADDR_AutoDisplayRowCol))

#define ADDR_GUI_TTF_DispCode     ADDR_AutoDisplayRowCol+2
typedef __packed struct
{
	u8 Code;		//GUI_TTF.c��ԭ��ʾ�Ĵ���>=0zA1�Ĵ��ʾ��һ��GB2312��,���¸�GB2312һ����ʾ
	u8 res;
}GUI_TTF_DispCode_TypeDef;
#define GUI_TTF_DispCode     ((GUI_TTF_DispCode_TypeDef *)(ADDR_GUI_TTF_DispCode))


#define ADDR_DOTDATA_END     (ADDR_GUI_TTF_DispCode+2)


		
#define ADDR_TerminalDisplay_IRAM_End     ((ADDR_DOTDATA_END+3)&0xfffffffc)		// 4�ֽڶ���


#endif

