

#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../Hcsg/ProjectCSG.h"
#endif
#include "../Device/24AA02E48.h"
#include "../Display/Display.h"
#include "../Device/MEMRW.h"



u32 Test_24AA02E48(u32 ADDR_BASE)
{
	u32 TestResult;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	
	TestResult=0;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	DisplayStringAutoUp(0,9,(u8*)"\x0""24AA02E48");//��ʾ�ַ����Զ�����ƽ��;����:�޸���һ��ʾ�в��Ĵ���ADDR_AutoRowCol
	Read_24AA02E48(ADDR_DATABUFF+1,0xFA,6);//24AAxxx��
	if(Comm_Ram->ErrorIC&(1<<10))//Ӳ�������־(��λ)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//��ʾ�ַ���
		TestResult=1;//�ܲ��Խ��0=�ϸ�,1=���ϸ�,0xff=�޽���
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//��ʾ�ַ���
	}
	
	//�洢����
	MWR(TestResult,ADDR_DATABUFF,1);
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_24AA02E48_Characteristics),7);
	
	return TestResult;
}




