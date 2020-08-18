
#include "AllHead.h"
#include "Terminal_ReadMeter_Fn.h"
#include "../Calculate/Calculate.h"
#include "../Driver/MEMRW.h"
#include "Terminal_AFN0C_RealTimeData_Fn.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0D_SourceData_Fn.h"
#include "../LPC177x8x/lpc177x8x_UART.h"
#include "KeyUser.h"



#define RMM_TimerTask     0//0=����,1=�Զ�������ʱ�������õ��������
#define RMM_Event     0//0=����,1=�Զ������¼���¼���õ��������





#if RMM_TimerTask==1//0=����,1=�Զ�������ʱ�������õ��������
u32 CompFnPn(u32 DI,u32 Fn,u32 Pn)//�ֽ�DIΪFn,Pn�ұȽ�;����0=��ͬ,1=����ͬ;ʹ��ADDR_DATABUFF
{
	u32 FnPn;
	u8 *p8;
	u32 *p32;
	p8=(u8*)(ADDR_DATABUFF);
	p32=(u32*)(ADDR_DATABUFF);
	
	p32[0]=DI;
	//Pn
	DI&=0xffff;
	if(DI!=0x00ff)
	{//����ȫ����Ч������
		if(p8[1]==0x0)
		{
			return 0;//Pn=0;
		}
		else
		{
			if(p8[0]==0x0)
			{
				return 0;//Pn=0;
			}
			else
			{
				while(1)
				{
					FnPn=BitLoca(p8[0]);//�����1����λΪ1��λ��(0-32)
					DI=p8[0];
					DI<<=1;
					DI&=(~(1<<FnPn));
					DI>>=1;
					p8[0]=DI;
					FnPn+=((p8[1]-1)*8);
					if(FnPn==Pn)
					{
						break;
					}
					if(DI==0)
					{//�ֽ���
						return 0;
					}
				}
			}
		}
	}
	//Fn
	if(p8[2]==0x0)
	{
		return 0;//Fn=0;
	}
	else
	{
		while(1)
		{
			FnPn=BitLoca(p8[2]);//�����1����λΪ1��λ��(0-32)
			DI=p8[2];
			DI<<=1;
			DI&=(~(1<<FnPn));
			DI>>=1;
			p8[2]=DI;
			FnPn+=(p8[3]*8);
			if(FnPn==Fn)
			{
				return 1;
			}
			if(DI==0)
			{//�ֽ���
				return 0;
			}
		}
	}
}
u32 ReadMeter_Fn_Class1TimerTask(u32 Fn,u32 Pn)//FnPn��1�����ݶ�ʱ����,����:0=û,1=��;ʹ�� ADDR_DATABUFF
{
	u32 i;
	u32 Task;
	u32 MaxDINum;
	u32 DI;
	
	for(Task=0;Task<AFN04F65MaxPn;Task++)
	{
		i=MRR(ADDR_AFN04F67+Task,1);//��ʱ����1����������������ֹͣ����(�����)
		if(i!=0x55)
		{//��"55H"����������"AAH"ֹͣ��������ֵ��Ч
			continue;
		}
		MaxDINum=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Task)+8,1);
		if(MaxDINum>MaxTimerTaskDI)
		{
			MaxDINum=MaxTimerTaskDI;
		}
		for(i=0;i<MaxDINum;i++)
		{
			DI=MRR(ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*Task)+9+(4*i),4);//ȡDI
			DI=CompFnPn(DI,Fn,Pn);//�ֽ�DIΪFn,Pn�ұȽ�;����0=��ͬ,1=����ͬ;ʹ��ADDR_DATABUFF
			if(DI!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}
u32 ReadMeter_Fn_Class2TimerTask(u32 Fn,u32 Pn)//FnPn��2�����ݶ�ʱ����,����:0=û,1=��
{
	u32 i;
	u32 Task;
	u32 MaxDINum;
	u32 DI;
	
	for(Task=0;Task<AFN04F66MaxPn;Task++)
	{
		i=MRR(ADDR_AFN04F68+Task,1);//��ʱ����2����������������ֹͣ����(�����)
		if(i!=0x55)
		{//��"55H"����������"AAH"ֹͣ��������ֵ��Ч
			continue;
		}
		MaxDINum=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Task)+8,1);
		if(MaxDINum>MaxTimerTaskDI)
		{
			MaxDINum=MaxTimerTaskDI;
		}
		for(i=0;i<MaxDINum;i++)
		{
			DI=MRR(ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*Task)+9+(4*i),4);//ȡDI
			DI=CompFnPn(DI,Fn,Pn);//�ֽ�DIΪFn,Pn�ұȽ�;����0=��ͬ,1=����ͬ;ʹ��ADDR_DATABUFF
			if(DI!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}
#endif

u32 ReadMeter_Fn_Com(u32 Fn,u32 Pn,u32 ClassNo,u32 RMFLAGS)
{
	u32 i;
	u32 y;
	
	y=0;
	i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
	if(i!=0)
	{
		MR(ADDR_DATABUFF,i+2,3);//ȡԭ����ʱ��������
		i=DataComp(ADDR_DATABUFF,ADDR_TYMDHMS+3,3);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
		if((i!=0)||(RMFLAGS==1))//�����־:0=�ϵ���ո���,1=����ǿ���س�,2=����������;0xfe=�ѳ�����û�ɹ�,0xff=�ɹ���ɳ���
		{
			y|=Class2Data_Fn(ClassNo,Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
		#if RMM_TimerTask==1//0=����,1=�Զ�������ʱ�������õ��������
			y|=ReadMeter_Fn_Class2TimerTask(Fn,Pn);//FnPn��2�����ݶ�ʱ����,����:0=û,1=��;ʹ�� ADDR_DATABUFF
		#endif
		}
	}
	return y;
}

u32 ReadMeter_Fn_YesNo(u32 PORTn,u32 RMFLAGS,u32 ClassNo,u32 Pn,u8 * p8RMLib,u32 RMCount,u32 Protocol)//����С��ų����Fn�Ƿ�Ҫ��;����0=����,1=Ҫ��;���:RMFLAGS������־,ClassNo��D7-D4�����D3-D0С���,Pn,p8RMLib�����ָ��,RMCount�������,Protocol(1=1997,����=2007)
//�����־:0=�ϵ���ո���,1=����ǿ���س�,2=����������;0xfe=�ѳ�����û�ɹ�,0xff=�ɹ���ɳ���
{
	u32 i;
	u32 y;
	u32 Fn;
	u64 d64a;
	u64 d64b;

#if (USER/100)==2//�û���ʶ:ɽ����
	//�ϵ�ʱ�ȳ����й�����ʾֵ
	UARTCtrl_TypeDef * UARTCtrl;
	if(PORTn==RS485_4PORT)
	{//���ز���
		UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
		if((UARTCtrl->RMComplete&0x0f)==0)//485����D7=1�ϵ����С�����1��ѭ������,D6-D4����,D3-D0���ѭ���������(�ڱ�עδ����ʱ��0,�������0)
		{
			if((p8RMLib[0]==33)||(p8RMLib[0]==129))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
#endif	

	if(ClassNo==0)
	{
		switch(PORTn)
		{
			case RS485_1PORT://RS485_1
			case RS485_2PORT://RS485_2
			case RS485_3PORT://RS485_3
				if(p8RMLib[3]&0x2)//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
				{
					return 1;//����0=����,1=Ҫ��
				}
				else
				{
					return 0;//����0=����,1=Ҫ��
				}
			case RS485_4PORT://RS485_4(�ز�)
				if(p8RMLib[3]&0x4)//����֧��:B2=�ز�����,B1=RS485����,B0=�ڲ����ɳ���
				{
					return 1;//����0=����,1=Ҫ��
				}
				else
				{
					return 0;//����0=����,1=Ҫ��
				}
		}
	}

	y=0;
	//1������
	if((p8RMLib[4]&0x01)!=0)//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//1������,ʵʱ
		Fn=p8RMLib[0];//ȡʵʱFn
		y=Class1Data_Fn(ClassNo,Fn);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��		
	#if RMM_TimerTask==1//0=����,1=�Զ�������ʱ�������õ��������
		y|=ReadMeter_Fn_Class1TimerTask(Fn,Pn);//FnPn��1�����ݶ�ʱ����,����:0=û,1=��;ʹ�� ADDR_DATABUFF
	#endif
		//1��ʵʱ������2�������������
		if(y==0)
		{
			i=KeyUserPnSet(Pn);//�ص㻧��Ч������ż��;����0=��Ч,1=��Ч;ʹ���ص㻧���߶���Pn�б�ADDR_KeyUserPnList
			if(i!=0)
			{//���ص㻧
				//F25 ��ǰ���༰����/�޹����ʡ����������������ѹ��������������������ڹ���
				if(Fn==25)
				{
					if(Protocol==1)
					{//DL/T645-1997
						i=MRR((u32)&p8RMLib[10+(3*RMCount)],2);//ȡDI
						switch(i)
						{
						//F81-84��������,A,B,C���й���������
							case 0xb630://���й�����
								y|=Class2Data_Fn(ClassNo,81);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb631://A�й�����
								y|=Class2Data_Fn(ClassNo,82);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb632://B�й�����
								y|=Class2Data_Fn(ClassNo,83);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb633://C�й�����
								y|=Class2Data_Fn(ClassNo,84);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
						//F85-88��������,A,B,C���޹���������
							case 0xb640://���޹�����
								y|=Class2Data_Fn(ClassNo,85);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb641://A�޹�����
								y|=Class2Data_Fn(ClassNo,86);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb642://B�޹�����
								y|=Class2Data_Fn(ClassNo,87);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb643://C�޹�����
								y|=Class2Data_Fn(ClassNo,88);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							//F105-108��������,A,B,C�๦����������
							case 0xb650://�ܹ�������
								y|=Class2Data_Fn(ClassNo,105);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb651://A��������
								y|=Class2Data_Fn(ClassNo,106);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb652://B��������
								y|=Class2Data_Fn(ClassNo,107);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb653://C��������
								y|=Class2Data_Fn(ClassNo,108);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							//F89-91������A,B,C���ѹ����	
							case 0xb611://A���ѹ
								y|=Class2Data_Fn(ClassNo,89);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb612://B���ѹ
								y|=Class2Data_Fn(ClassNo,90);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xb613://C���ѹ
								y|=Class2Data_Fn(ClassNo,91);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							//F92-94������A,B,C���������
							case 0xB621://A�����
								y|=Class2Data_Fn(ClassNo,92);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xB622://B�����
								y|=Class2Data_Fn(ClassNo,93);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0xB623://C�����
								y|=Class2Data_Fn(ClassNo,94);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
						}
					}
					else
					{//DL/T645-2007
						i=MRR((u32)&p8RMLib[10+(5*RMCount)],4);//ȡDI
						switch(i)
						{
							case 0x0203ff00://��,A,B,C�й�����
								//F81-84��������,A,B,C���й���������
								y|=Class2Data_Fn(ClassNo,81);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,82);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,83);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,84);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0x0204ff00://��,A,B,C�޹�����
								//F85-88��������,A,B,C���޹���������
								y|=Class2Data_Fn(ClassNo,85);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,86);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,87);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,88);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0x0206ff00://��,A,B,C��������
								//F105-108��������,A,B,C�๦����������
								y|=Class2Data_Fn(ClassNo,105);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,106);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,107);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,108);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0x0201ff00://A,B,C���ѹ
								//F89-91������A,B,C���ѹ����
								y|=Class2Data_Fn(ClassNo,89);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,90);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,91);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0x0202ff00://A,B,C�����
								//F92-94������A,B,C���������
								y|=Class2Data_Fn(ClassNo,92);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,93);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								y|=Class2Data_Fn(ClassNo,94);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0x02800001://�������
								//F95�����������������
								y|=Class2Data_Fn(ClassNo,95);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
								break;
							case 0x0205ff00://��,A,B,C���ڹ���
								break;
						}
					}
				}
				//F129 ��ǰ�����й�����ʾֵ(�ѳ�Сʱ����)
				//F130 ��ǰ�����޹�������޹�1������ʾֵ
				if(Fn==130)
				{
					//F102�����������޹��ܵ���ʾֵ����
					y|=Class2Data_Fn(ClassNo,102);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
				//F131 ��ǰ�����й�����ʾֵ(�ѳ�Сʱ����)
				//F132 ��ǰ�����޹�������޹�2������ʾֵ
				if(Fn==132)
				{
					//F104�����㷴���޹��ܵ���ʾֵ����
					y|=Class2Data_Fn(ClassNo,104);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
				//F133 ��ǰһ�����޹�����ʾֵ
				if(Fn==133)
				{
					//F145������1�����޹��ܵ���ʾֵ����
					y|=Class2Data_Fn(ClassNo,145);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
				//F134 ��ǰ�������޹�����ʾֵ
				if(Fn==134)
				{
					//F147������2�����޹��ܵ���ʾֵ����
					y|=Class2Data_Fn(ClassNo,147);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
				//F135 ��ǰ�������޹�����ʾֵ
				if(Fn==135)
				{
					//F148������3�����޹��ܵ���ʾֵ����
					y|=Class2Data_Fn(ClassNo,148);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
				//F136 ��ǰ�������޹�����ʾֵ
				if(Fn==136)
				{
					//F146������4�����޹��ܵ���ʾֵ����
					y|=Class2Data_Fn(ClassNo,146);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
				}
			}
		}
	}
	//2������
	if((p8RMLib[4]&0x10)!=0)//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������,�����ն���
		Fn=p8RMLib[8];//�����ն���Fn
		y|=Class2Data_Fn(ClassNo,Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��		
	#if RMM_TimerTask==1//0=����,1=�Զ�������ʱ�������õ��������
		y|=ReadMeter_Fn_Class2TimerTask(Fn,Pn);//FnPn��2�����ݶ�ʱ����,����:0=û,1=��;ʹ�� ADDR_DATABUFF
	#endif
	}
	if((p8RMLib[4]&0x04)!=0)//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������,����
		//��ǰ������Ϊ���ߵ�ֻ��  ��չ13 ��1-24��������������й�����ʾֵ
		y=1;//���ڳ�������F101,F103�Ƿ�����
	}
	if((p8RMLib[4]&0x08)!=0)//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������B3=�ն���
		Fn=p8RMLib[7];//ȡ�ն���Fn
		if(p8RMLib[0]==0xE3)
		{//���г����ն����������޹�,4�������޹�����ʾֵ���ܡ�����1��M��
			Fn=GetClass2SourceDataAddr(161,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
			if(Fn!=0)
			{
//				i=MRR(Fn-3,3);//ȡԭ����������
//				Fn=MRR(ADDR_TYMDHMS+3,3);//ȡ��ǰʱ��������
//				if(i!=Fn)
//				{//�����ղ�ͬ
					if((RMCount>0)&&(RMCount<9))
					{
						Fn=160+RMCount;//�����������1-8��ӦFn=161-168
						y|=ReadMeter_Fn_Com(Fn,Pn,ClassNo,RMFLAGS);
					}
					else
					{//�����������=0��9,��1���ն���ʱ��YYMMDDhhmm
						y|=ReadMeter_Fn_Com(161,Pn,ClassNo,RMFLAGS);
						y|=ReadMeter_Fn_Com(162,Pn,ClassNo,RMFLAGS);
						y|=ReadMeter_Fn_Com(163,Pn,ClassNo,RMFLAGS);
						y|=ReadMeter_Fn_Com(164,Pn,ClassNo,RMFLAGS);
						y|=ReadMeter_Fn_Com(165,Pn,ClassNo,RMFLAGS);
						y|=ReadMeter_Fn_Com(166,Pn,ClassNo,RMFLAGS);
						y|=ReadMeter_Fn_Com(167,Pn,ClassNo,RMFLAGS);
						y|=ReadMeter_Fn_Com(168,Pn,ClassNo,RMFLAGS);
					}
//				}
			}
		}
		else
		{
			y|=ReadMeter_Fn_Com(Fn,Pn,ClassNo,RMFLAGS);
		}
	}
	if((p8RMLib[4]&0x20)!=0)//���ݷ���:B6=������2-3�ն���,B5=�¶���,B4=�����ն���,B3=�ն���,B2=���߶���,B1=Сʱ����,B0=ʵʱ
	{//2������B5=�¶���
		Fn=p8RMLib[9];//ȡ�¶���Fn
		i=GetClass2SourceDataAddr(Fn,Pn);//ȡ��2ԭʼ���ݴ�����ַ,����0��ʾû�ҵ�,��Ч���ݳ��ȷ�ADDR_Terminal_ClassDataLen
		if(i!=0)
		{
			MR(ADDR_DATABUFF,i+3,2);//ȡԭ����ʱ������
			i=DataComp(ADDR_DATABUFF,ADDR_TYMDHMS+4,2);//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
			if((i!=0)||(RMFLAGS==1))//�����־:0=�ϵ���ո���,1=����ǿ���س�,2=����������,0xff=��ɱ���־����
			{
				y|=Class2Data_Fn(ClassNo,Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			#if RMM_TimerTask==1//0=����,1=�Զ�������ʱ�������õ��������
				y|=ReadMeter_Fn_Class2TimerTask(Fn,Pn);//FnPn��2�����ݶ�ʱ����,����:0=û,1=��;ʹ�� ADDR_DATABUFF
			#endif
			}
		}
	}
	if((p8RMLib[0]==0xEE)||(p8RMLib[0]==0xEF))
	{//������2-3�ն���(�ն����������޹�,4�������޹�����ʾֵ���ܡ�����1��M��)
		y=0;
		if((RMCount>0)&&(RMCount<9))
		{
			Fn=160+RMCount;//�����������1-8��ӦFn=161-168
			y|=Class2Data_Fn(ClassNo,Fn);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
		}
		else
		{//�����������=0��9,��1���ն���ʱ��YYMMDDhhmm
			y|=Class2Data_Fn(ClassNo,161);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			y|=Class2Data_Fn(ClassNo,162);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			y|=Class2Data_Fn(ClassNo,163);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			y|=Class2Data_Fn(ClassNo,164);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			y|=Class2Data_Fn(ClassNo,165);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			y|=Class2Data_Fn(ClassNo,166);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			y|=Class2Data_Fn(ClassNo,167);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			y|=Class2Data_Fn(ClassNo,168);//2������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
		}	
	}
	
	Fn=p8RMLib[0];
	d64a=MRR(ADDR_AFN04F9,8);//�¼���¼��Ч��־λ BS64 8
	//ERC8���ܱ��������¼�������
	if(Fn==0xE1)
	{
		d64b=1;
		d64b<<=(8-1);
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//ERC13��������Ϣ�¼�������
	if(Fn==0xE2)
	{
		d64b=1;
		d64b<<=(13-1);
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//ERC40�����ܱ�㶨�ų������ܴ���
	if(Fn==0xF0)
	{
		d64b=1;
		d64b<<=(40-1);//ERC40�ų��쳣�¼���¼
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
#if (USER/100)==2//�û���ʶ:ɽ����
	//��չ ERC9������·�쳣�¼�������
	if(Fn==0xF2)
	{
		d64b=1;
		d64b<<=(9-1);//ERC9������·�쳣�¼�
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//��չ ERC10��ѹ��·�쳣�¼�������
	if(Fn==0xF3)
	{
		d64b=1;
		d64b<<=(10-1);//ERC10��ѹ��·�쳣�¼�
		d64b&=d64a;
		if(d64b!=0)
		{
			y=1;
		}
	}
	//��չ ERC29���ܱ�����¼�,ERC28���ܱ����¼�������
	if(Fn==0xF4)
	{
		d64b=3;
		d64b<<=(28-1);//ERC28,ERC29
		d64b&=d64a;
		if(d64b!=0)
		{
			i=Class1Data_Fn(ClassNo,25);//1������Fn����,���ClassNo��D7-D4�����D3-D0С���,Fn;����0=��֧��,1=֧��
			if(i==0)
			{//û��F25(�й�����)
				y=1;
			}
		}
	}
#endif
	
	
#if RMM_Event==1//0=����,1=�Զ������¼���¼���õ��������
	//ERC8���ܱ��������¼�
	//ERC11�����쳣
	//ERC12���ܱ�ʱ�䳬��
	//ERC13��������Ϣ�¼�
	//ERC27���ܱ�ʾ���½�
	//ERC33���ܱ�����״̬�ֱ�λ�¼�
	//ERC37���ܱ�����¼�
	//ERC38���ܱ��˸��¼�
	//ERC40ֱ�Ӷ����ܱ�ų��쳣�¼���¼
	//ERC41���ܱ����ն˶�ʱ�¼���¼
	switch(Fn)
	{
		case 27://F27 ���ܱ�����ʱ�ӡ���̴����������һ�β���ʱ��
			goto RMERC1;
		case 28://F28 �������״̬�ּ����λ��־
			switch(PORTn)
			{
				case RS485_1PORT://RS485-1
					i=1;//RS485PORT=1;
					break;
				case RS485_2PORT://RS485-2
					i=2;//RS485PORT=2;
					break;
				case RS485_3PORT://RS485-3
					i=3;//RS485PORT=3;
					break;
				default://RS485-4(�ز�)
					i=30;//RS485PORT=4
					break;
			}
			i=MRR(ADDR_AFN04F33+((14+(4*24))*i)+1,1);//�ն˳������в�������,�˿�31
			if(i&0x20)
			{//2)	D5��"1"Ҫ���ն˳���"���״̬��"����"0"��Ҫ��
				y=1;
			}
			d64b=1;
			d64b<<=(11-1);//ERC11�����쳣
			d64b&=d64a;
			if(d64b!=0)
			{
				y=1;
			}
			d64b=1;
			d64b<<=(33-1);//ERC33���ܱ�����״̬�ֱ�λ�¼�
			d64b&=d64a;
			if(d64b!=0)
			{
				y=1;
			}
			goto RMERC1;
		case 165://F165 ���ܱ��ز���������ʱ��
			d64b=3;
			d64b<<=(37-1);//ERC37���ܱ�����¼�,ERC38���ܱ��˸��¼�
			goto RMERC2;
		case 129://F129 ��ǰ�����й�����ʾֵ���ܡ�����1��M��
			d64b=1;
			d64b<<=(27-1);//ERC27���ܱ�ʾ���½�
			goto RMERC2;
		case 162://F162 ���ܱ�����ʱ��
			d64b=1;
			d64b<<=(12-1);//ERC12���ܱ�ʱ�䳬��
			goto RMERC2;
		case 166://F166 ���ܱ�����޸Ĵ�����ʱ��
			d64b=1;
			d64b<<=(41-1);//ERC41���ܱ����ն˶�ʱ�¼���¼
			goto RMERC2;
		case 0xe1://��չ1 ERC8���ܱ��������¼�������
			d64b=1;
			d64b<<=(8-1);//ERC8���ܱ��������¼�
			goto RMERC2;
		case 0xe2://��չ2 ERC13��������Ϣ�¼�������,ͬ��F27,F28
		RMERC1:
			d64b=1;
			d64b<<=(13-1);//ERC13��������Ϣ�¼�
		RMERC2:
			d64b&=d64a;
			if(d64b!=0)
			{
				y=1;
			}
			break;
	}
#endif
	
	return y;
}



