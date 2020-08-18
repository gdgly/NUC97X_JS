

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
	u32 TestResult;//总测试结果0=合格,1=不合格,0xff=无结论
	
	TestResult=0;//总测试结果0=合格,1=不合格,0xff=无结论
	DisplayStringAutoUp(0,9,(u8*)"\x0""24AA02E48");//显示字符串自动向上平移;出口:修改下一显示行并寄存在ADDR_AutoRowCol
	Read_24AA02E48(ADDR_DATABUFF+1,0xFA,6);//24AAxxx读
	if(Comm_Ram->ErrorIC&(1<<10))//硬件错误标志(置位)
	{
		DisplayString(AutoDisplayRowCol->Row-1,16,0,(u8*)"\x0""FAIL");//显示字符串
		TestResult=1;//总测试结果0=合格,1=不合格,0xff=无结论
	}
	else
	{
		DisplayString(AutoDisplayRowCol->Row-1,18,0,(u8*)"\x0""OK");//显示字符串
	}
	
	//存储参数
	MWR(TestResult,ADDR_DATABUFF,1);
	MW(ADDR_DATABUFF,ADDR_BASE+(OFFSET_24AA02E48_Characteristics),7);
	
	return TestResult;
}




