
//�ն���2���ݼ���
#include "Project.h"
#include "Terminal_AFN0D_CongealData.h"
#include "../Device/MEMRW.h"
#include "../Device/DMW.h"
#include "../Calculate/Calculate.h"
#include "Terminal_AFN0D_CongealData_Fn.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_AFN0D_SourceData.h"

#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "KeyUser.h"




extern const Terminal_Class2DataLib_TypeDef	Terminal_Class2DataLib[];
u32 Get_Class2Data_MaxFn(void);



void Init_Class2Data_UpPower(void)//��2���ݳ�ʼ��(�ϵ�)
{
//������ԭʼ���ݳ�ʼ��
	
	//���߶����õ�����ԭʼ���ݳ�ʼ��
	MC(0xee,ADDR_AFNOD_SOUR_Start,(ADDR_AFNOD_SOUR_End)-(ADDR_AFNOD_SOUR_Start));
}

void Init_Class2Data_Command(void)//��2���ݳ�ʼ��(����,ȫ�����)
{
	u32 * p32;
	
//��ԭ����
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//�嶳���Զ�IOW
	p32[3]=0;
//��,������,��,���߶���������Ч
	MC(0x0,ADDR_TCongealCount,LEN_TCongealCount);
	MC(0x0,ADDR_PnCongealCount,LEN_PnCongealCount*MaxRS485AddCarrierPn);
	Init_Class2Data_UpPower();//��2���ݳ�ʼ��(�ϵ�)
	
#if (USER/100)==5//�Ϻ�
	MC(0xff,ADDR_AFN0CF242_Next,6*AFN0CF242MaxCount);//���յ�¼��վ��Ϣ
	MC(0xff,ADDR_AFN0CF243,24);//�����ź�ǿ��
	MC(0,ADDR_CURVECONGEAL_ExtCount,7);//��0��չ���߼���
#endif

#if (USER/100)==11//����
	//��ѹͳ�����ݵ���RAM�е�����,������Ѹ�ָ��RAM
	MC(0,ADDR_AFN0DF27_SOUR,(66+27)*AFN0DF27MaxPn);
	MC(0,ADDR_AFN0DF35_SOUR,(66+27)*AFN0DF35MaxPn);
#endif
}

void Class2Congeal_AutoRW(void)//��2�����Զ�IO��д����0,��ɺ�������
{
	u32 i;
	u32 Count;
	u8 * p8;
	u32 * p32;

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);
	if(p32[3]!=0x0)
	{
		//i=Get_LEN_FLASHSectorWrite(p32[2]);//�õ�FLASH��ǰ��ַ������д�ֽڳ���
		i=(128*1024)-(p32[2]&0x1ffff);//��ǰ����д�ֽ�
		if(i>p32[3])
		{
			i=p32[3];
		}
		p8=(u8 *)(ADDR_Class2Congeal_AutoRW);
		switch(p8[0])
		{
			case 0://IOCLR
				MC(p8[1],p32[2],i);
				break;
			case 1://IOR
				MR(p32[1],p32[2],i);
				break;
			case 2://IOW
				MW(p32[1],p32[2],i);
				break;
			default:
				p32[3]=0;
				return;
		}
		p32[1]+=i;
		p32[2]+=i;
		p32[3]-=i;
		if(p32[3]==0x0)
		{//����=0,��ɺ���
			//����+1,�������Զ�д�����־
			switch(p8[2])//���ݱ��0=���߶���,1=�ն���,2=�����ն���,3=�¶���,4=�ն������,5=������2��,6=������3��
			{
				case 0://0=���߶���
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],5);//������Ч(д����ʱ��)
					if(Terminal_Ram->CurveCongealTask==0)//���߶�������0-24(�������1-24��)
					{
						MW(ADDR_Class2Congeal_AutoRW+20,ADDR_TCongealCount+9,5);//д�ϴζ���ʱ��
					}
					Count=MRR(ADDR_TCongealCount+7,2);
					if(p32[4]==(ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*(Count&0x7fff))))
					{//��ǰ�����ַ�Ǽ��������,����+1
						PnCurveCongealCount();//����������߶������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
						i=Count&0x8000;
						Count&=0x7fff;
						Count+=1;
						if(Count>=MaxCurveCongeal)
						{
							Count=0;
							i=0x8000;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+7,2);
					}
					Terminal_Ram->CurveCongealTask++;//���߶�������0-24(��ʱ,��1-24ʱ)
					break;
				case 1://1=�ն���
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],3);//������Ч(д����ʱ��)
					Count=MRR(ADDR_TCongealCount+14,1);
					if(p32[4]==(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*(Count&0x7f))))
					{//��ǰ�����ַ�Ǽ��������,����+1
						PnDateCongealCount();//��������ն������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxDateCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+14,1);
					}
					MW(ADDR_TYMDHMS+3,ADDR_TCongealCount+16,3);//д�ϴζ���ʱ��
					Terminal_Ram->CongealFlags|=4;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
					
					MC(0x0,ADDR_AFN0DF27_PAR,18*AFN0DF27MaxPn);//��ƽ����ѹ�ۼӼ���(�ն������0)
					break;
				case 2://2=�����ն���
					//�����պ�ʱ�ֶ���
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],3);//������Ч(д����ʱ��)
				/*
					//��������δ24�㶳��
					i=YMD_Sub1D(MRR(ADDR_TYMDHMS+3,3));//�����ռ�1��,���ؼ�1���������
					MWR(i,p32[4],3);//������Ч(д����ʱ��)
				*/
					Count=MRR(ADDR_TCongealCount+21,1);
					if(p32[4]==(ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*(Count&0x7f))))
					{//��ǰ�����ַ�Ǽ��������,����+1
						PnRMDateCongealCount();//������ĳ����ն������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxRMDateCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+21,1);
					}
					MW(ADDR_TYMDHMS+1,ADDR_TCongealCount+23,5);//д�ϴζ���ʱ��
					Terminal_Ram->CongealFlags|=8;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
					break;
				case 3://3=�¶���
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],2);//������Ч(д����ʱ��)
					Count=MRR(ADDR_TCongealCount+28,1);
					if(p32[4]==(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*(Count&0x7f))))
					{//��ǰ�����ַ�Ǽ��������,����+1
						PnMonthCongealCount();//��������¶������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxMonthCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+28,1);
					}
					MW(ADDR_TYMDHMS+4,ADDR_TCongealCount+30,2);//д�ϴζ���ʱ��
					Terminal_Ram->CongealFlags|=0x10;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
					
					MC(0x0,ADDR_AFN0DF35_PAR,18*AFN0DF27MaxPn);//��ƽ����ѹ�ۼӼ���(�¶������0)
					break;
				case 4://4=�ն������(���ճ������ݲ���Ϊ�����ն���)
				case 5://5=������2��
				case 6://6=������3��
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],3);//������Ч(д����ʱ��)
					Count=MRR(ADDR_TCongealCount+14,1);
					if(p32[4]==(ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*(Count&0x7f))))
					{//��ǰ�����ַ�Ǽ��������,����+1
						PnDateCongealCount();//��������ն������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxDateCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+14,1);
					}
					break;
				case 7://7=�¶������(���³������ݲ���Ϊ�����¶���)
					DMW(ADDR_Class2Congeal_AutoRW+20,p32[4],2);//������Ч(д����ʱ��)
					Count=MRR(ADDR_TCongealCount+28,1);
					if(p32[4]==(ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*(Count&0x7f))))
					{//��ǰ�����ַ�Ǽ��������,����+1
						PnMonthCongealCount();//��������¶������(��0���ܱ����ñ��ʱ�Ķ������ֵ);�ڶ����д��ǰ�������ǰִ����������
						i=Count&0x80;
						Count&=0x7f;
						Count+=1;
						if(Count>=MaxMonthCongeal)
						{
							Count=0;
							i=0x80;
						}
						Count|=i;
						MWR(Count,ADDR_TCongealCount+28,1);
					}
					break;
			}
		}
	}
}


void PointkWhInc_KeyUser(u32 SourAddr,u32 CurrAddr,u32 MapPn,u32 Pn)//�ص㻧��������������й�����������,���ֵ��DATABUFF
{
	u64 d64a;
	u64 d64b;
	u8 * p8;
	//��ÿ�ζ��Ὺʼʱֵ
	MR(ADDR_DATABUFF,SourAddr+(5*MapPn),5);
	//����ǰֵ
	MR(ADDR_DATABUFF+5,CurrAddr+6+((6+(5*(1+TMaxTariff)))*Pn),5);
	//�浱ǰֵΪ��ʼʱֵ
	MW(ADDR_DATABUFF+5,SourAddr+(5*MapPn),5);

	p8=(u8 *)(ADDR_DATABUFF);
	if(p8[0]==0xee)
	{
		return;
	}
	if(p8[5]==0xee)
	{
		MC(0xee,ADDR_DATABUFF,5);
		return;
	}
	d64a=MRR(ADDR_DATABUFF,5);//�Ƕ����ֽ�װ�뵽��,������ݵ�ַ,װ���ֽ���
	d64b=MRR(ADDR_DATABUFF+5,5);//�Ƕ����ֽ�װ�뵽��,������ݵ�ַ,װ���ֽ���
	d64a=bcd16_hex(d64a);
	d64b=bcd16_hex(d64b);
	if(d64b<d64a)
	{//���ܲ�����ֵ��0
		d64b+=10000000000;
	}
	d64b-=d64a;
	d64b=hex_bcd16(d64b);
	MWR(d64b,ADDR_DATABUFF,4);//�ִ浽�Ƕ����ֽ�,������ݵ�ַ,�����ֽ���
}

void PointkvarhInc_KeyUser(u32 SourAddr,u32 CurrAddr,u32 MapPn,u32 Pn)//�ص㻧���������޹�����������,���ֵ��DATABUFF
{
	u32 i;
	u32 * p32;
	//��ÿ�ζ��Ὺʼʱֵ
	MR(ADDR_DATABUFF,SourAddr+(4*MapPn),4);
	//����ǰֵ
	MR(ADDR_DATABUFF+4,CurrAddr+6+((6+(4*(1+TMaxTariff)))*Pn),4);
	//�浱ǰֵΪ��ʼʱֵ
	MW(ADDR_DATABUFF+4,SourAddr+(4*MapPn),4);

	p32=(u32 *)(ADDR_DATABUFF);
	i=0xeeeeeeee;
	if(p32[0]==i)
	{
		return;
	}
	if(p32[1]==i)
	{
		p32[0]=i;
		return;
	}
	p32[0]=bcd_hex(p32[0]);
	p32[1]=bcd_hex(p32[1]);
	if(p32[1]<p32[0])
	{//���ܲ�����ֵ��0
		p32[1]+=100000000;
	}
	p32[1]-=p32[0];
	p32[1]=hex_bcd(p32[1]);
	p32[0]=p32[1]<<8;//2λС����4λС��
}

void AFN0C_Full_CurveCongeal_Buff(u32 Addr_CongealBuff)//AFN0Cʵʱ�����������߶��Ỻ��
{
	u32 Fn;
	u32 Pn;
	u32 Class2Data_MaxFn;
	u32 i;
	u32 MapPn;
	u32 PORT;
	u16 *pn16;
//	u32 *p32;


	PnVSPORTProtocolList(ADDR_PnVSSetList);//�������Pn��ӦAFN04F10���ö˿ںź�ͨ��Э���б�,�������ҷ������Ĭ�϶˿ں�0ͨ��Э��Ϊ30(DL645-2007)
	pn16=(u16 *)(ADDR_PnVSSetList);
	KeyUserPnList();//���г����ص㻧���߶���Pn�б�

	
	MC(0xFF,Addr_CongealBuff,5);//����ʱ����Ϊ0XFF
	Class2Data_MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<Class2Data_MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==3)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//p0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//ԭʼ��ַ=0,�޴�����
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
						{
							switch(Fn)
							{
								case 75://�ܼ����й�����������
									TotalAddEnergyInc_Computer(ADDR_AFN0DF75_SOUR,ADDR_AFN0CF19,Pn);//���������ܼӵ���������,���ֵ��DATABUFF
									break;
								case 76://�ܼ����޹�����������
									TotalAddEnergyInc_Computer(ADDR_AFN0DF76_SOUR,ADDR_AFN0CF20,Pn);//���������ܼӵ���������,���ֵ��DATABUFF
									break;
								case 97://�����������й��ܵ���������
								#if AFN0DF97MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkWhInc_KeyUser(ADDR_AFN0DF97_SOUR,ADDR_AFN0CF129,MapPn,Pn);//�ص㻧��������������й�����������,���ֵ��DATABUFF
									}
									break;
								#endif
								case 98://�����������޹��ܵ���������
								#if AFN0DF98MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkvarhInc_KeyUser(ADDR_AFN0DF98_SOUR,ADDR_AFN0CF130,MapPn,Pn);//�ص㻧���������޹�����������,���ֵ��DATABUFF
									}
									break;
								#endif
								case 99://�����㷴���й��ܵ���������
								#if AFN0DF99MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkWhInc_KeyUser(ADDR_AFN0DF99_SOUR,ADDR_AFN0CF131,MapPn,Pn);//�ص㻧��������������й�����������,���ֵ��DATABUFF
									}
									break;
								#endif
								case 100://�����㷴���޹��ܵ���������
								#if AFN0DF100MaxPn==0
									continue;
								#else
									MapPn=KeyUserMapPn(Pn+1);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
									if(MapPn!=0)
									{
										MapPn--;
										PointkvarhInc_KeyUser(ADDR_AFN0DF100_SOUR,ADDR_AFN0CF132,MapPn,Pn);//�ص㻧���������޹�����������,���ֵ��DATABUFF
									}
									break;
								#endif
							}
							if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
							{//ԭʼ��ַ=0,�޴�����
								MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								if(Terminal_Class2DataLib[Fn].SourAddr==ADDR_DATABUFF)
								{//ԭʼ���ݵ�ַ=ADDR_DATABUFF
									if(Terminal_Class2DataLib[Fn].PnType!=pnType)
									{//�ǲ�������
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
									}
									else
									{//��������
										MapPn=KeyUserMapPn(Pn+1);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
										if(MapPn!=0)
										{
											MapPn--;
											MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										}
									}
								}
								else
								{//ԭʼ���ݵ�ַ!=ADDR_DATABUFF
									if(Terminal_Class2DataLib[Fn].PnType!=pnType)
									{//�ǲ�������
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
									}
									else
									{//��������
										MapPn=KeyUserMapPn(Pn+1);//���г����ص㻧��ӳ��Pn(1-MaxKeyUser);���:Pn=1-MaxRS485AddCarrierPn
										if(MapPn!=0)
										{
											MapPn--;
											i=Check_AFN04F11_Pn(Pn+1);//����ն��������ò���ָ���Ĳ������;����0��,1��
											if(i!=0)
											{//�������
												MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
											}
											else
											{//���������
												PORT=pn16[Pn+1]&0x1f;//ͨ�Ŷ˿ں�
												i=pn16[Pn+1]>>8;//ͨ��Э��
											#if (((Project/100)==2)&&(USER==600))//�Ǻ��ϼ�����(RS485���ܱ���15��ʵʱ����,������ǰ��װ���ܱ����߶�����Ϊ60��)
												if(PORT!=31)//���ڲ���������(2)��RS485��
											#else
												if((i<30)||(PORT==1))//���ڲ���������(2)��DL/T645-1997(1)
											#endif	
												{
													MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
												}
												else
												{//��DL/T645-2007(30)���нӿ�����խ����ѹ�ز�(31)
													if(PORT!=31)
													{//485��
										#if RMM_RS485_IntegralCongeal==0//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����			
														//��������������,������ʵʱ���ݶ�
														MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														continue;
										#endif
										#if RMM_RS485_IntegralCongeal==1//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����			
														//������������
														switch(Fn)
														{
															default:
																MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
																break;
															case 101://�����������й��ܵ���ʾֵ
															case 103://�����㷴���й��ܵ���ʾֵ
														#if (USER/100)==11//����ϵ
															//case 89://������A���ѹ����
														#endif
																//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
																break;
														}
														continue;
										#endif
										#if RMM_RS485_IntegralCongeal==2//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����			
														//������������
														if(((Fn>=81)&&(Fn<=95))||((Fn>=101)&&(Fn<=108))||((Fn>=145)&&(Fn<=148)))
														{
															//��,A,B,C�й�����;��,A,B,C�޹�����;A,B,C���ѹ;A,B,C�����
															//���������޹��ܵ���ʾֵ;A,B,C��������
															//4�������޹�����ʾֵ
															//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
														}
														else
														{
															MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														}
														continue;
										#endif
													}
													if(PORT==31)
													{//���ز���
										#if RMM_Carrier_IntegralCongeal==0//�ز�ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
														//��������������,������ʵʱ���ݶ�
														MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														continue;
										#endif
										#if RMM_Carrier_IntegralCongeal==1//�ز�ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
														//������������
														switch(Fn)
														{
															default:
																MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
																break;
															case 101://�����������й��ܵ���ʾֵ
															case 103://�����㷴���й��ܵ���ʾֵ
														#if (USER/100)==11//����ϵ
															///case 89://������A���ѹ����
														#endif
																//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
																break;
														}
														continue;
										#endif
										#if RMM_Carrier_IntegralCongeal==2//�ز�ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
														//������������
														if(((Fn>=81)&&(Fn<=95))||((Fn>=101)&&(Fn<=108))||((Fn>=145)&&(Fn<=148)))
														{
															//��,A,B,C�й�����;��,A,B,C�޹�����;A,B,C���ѹ;A,B,C�����
															//���������޹��ܵ���ʾֵ;A,B,C��������
															//4�������޹�����ʾֵ
															//MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].PnLen);
														}
														else
														{
															MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*MapPn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
														}
														continue;
										#endif
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


void Terminal_CurveCongeal(void)//���߶���
{
	u32 i;
	u8 * p8;
	u32 * p32;
	u64 YMDHM;
	u64 YMDHMsour;
	u32 AddPoint;
//#if(RMM_RS485_IntegralCongeal!=0)//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
	u32 SatrtCongealNo;
	u32 CongealCount;
//#endif

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
	if(p32[3]!=0x0)
	{//ԭ��2���ݶ���AUTOIOWû���
		return;
	}
	if(Terminal_Ram->CongealFlags&2)//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	{
//	#if(RMM_RS485_IntegralCongeal==0)//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
//		AddPoint=AddPoint;
//		return;
//	#endif
	
#if (RMM_RS485_IntegralCongeal==1)//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
	#if IC_SDRAM
	  //��128KDATABUFF
	  #if ((LEN_CURVECONGEAL*2)>(128*1024))
		//����ǰʱ�䵽����0���x���������߶����
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
		AddPoint+=24+1;//����24��,0����1��
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		i*=60;
		YMDHM=MRR(ADDR_TYMDHMS+2,4);
		YMDHM<<=8;
		YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
		CreateCurveCongealNo(YMDHM);//�������߶����;����:0=û����(ԭ�Ѵ���),1=����
		Terminal_Ram->CurveCongealCheckCount++;//���߶��������
		return;
	  #else
		//����ǰʱ�䵽����0���x���������߶����
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
 #if (USER/100)==11//����ϵ
		AddPoint*=4;
		AddPoint+=96+1;//����96��+��ǰ1������
 #else
		AddPoint+=24+1;//����24��,0����1��
 #endif
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//�ص㻧���߶���Pn�б�
		p32=(u32*)(ADDR_128KDATABUFF);
		for(i=0;i<(LEN_128KDATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//��ʼ�����
		CongealCount=0;//�����������
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//���߶��������<�貹����
			i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
			Terminal_Ram->CurveCongealCheckCount++;//���߶��������
 #if (USER/100)==11//����ϵ
			i*=15;
 #else
			i*=60;
 #endif
			YMDHM=MRR(ADDR_TYMDHMS+2,4);
			YMDHM<<=8;
			YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
			i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
			if(i!=0xffffffff)
			{//ԭ����ͬʱ��
				continue;
			}
			MWR(YMDHM,ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//������Ч(����ʱ��)
			MR(ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//�����������
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_128KDATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//�������򶳽�洢��0
				break;
			}
		}
		if(CongealCount!=0)//�����������
		{
			MW(ADDR_128KDATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//��ʼ�����
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//ԭ����־
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#else
	 	//��128KDATABUFF(ר��)
	  #if ((LEN_CURVECONGEAL*2)>LEN_DATABUFF)
		//����ǰʱ�䵽����0���x���������߶����
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
		AddPoint+=24+1;//����24��,0����1��
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		i*=60;
		YMDHM=MRR(ADDR_TYMDHMS+2,4);
		YMDHM<<=8;
		YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
		CreateCurveCongealNo(YMDHM);//�������߶����;����:0=û����(ԭ�Ѵ���),1=����
		Terminal_Ram->CurveCongealCheckCount++;//���߶��������
		return;
	  #else
		//����ǰʱ�䵽����0���x���������߶����
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
		AddPoint+=24+1;//����24��,0����1��
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//�ص㻧���߶���Pn�б�
		p32=(u32*)(ADDR_DATABUFF);
		for(i=0;i<(LEN_DATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//��ʼ�����
		CongealCount=0;//�����������
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//���߶��������<�貹����
			i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
			Terminal_Ram->CurveCongealCheckCount++;//���߶��������
			i*=60;
			YMDHM=MRR(ADDR_TYMDHMS+2,4);
			YMDHM<<=8;
			YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
			i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
			if(i!=0xffffffff)
			{//ԭ����ͬʱ��
				continue;
			}
			MWR(YMDHM,ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//������Ч(����ʱ��)
			MR(ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//�����������
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_DATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//�������򶳽�洢��0
				break;
			}
		}
		if(CongealCount!=0)//�����������
		{
			MW(ADDR_DATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//��ʼ�����
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//ԭ����־
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#endif
#endif//#if(RMM_RS485_IntegralCongeal==1)//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
	
#if ((RMM_RS485_IntegralCongeal==0)||(RMM_RS485_IntegralCongeal==2))//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
	#if IC_SDRAM
	  //��128KDATABUFF
	  #if ((LEN_CURVECONGEAL*2)>LEN_128KDATABUFF)
		//����ǰʱ�䵽����0���x�����߶����
		i=Comm_Ram->TYMDHMS[1];//��ǰRTC��
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//ÿСʱ4��
		AddPoint+=i;
		AddPoint+=96+1;//����96��,0����1��
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		i*=15;
		YMDHM=CurveCongeal_YMDHM();//��ǰ��׼���߶���ʱ��YMDHM
		YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
		CreateCurveCongealNo(YMDHM);//�������߶����;����:0=û����(ԭ�Ѵ���),1=����
		Terminal_Ram->CurveCongealCheckCount++;//���߶��������
		return;
	  #else
		//����ǰʱ�䵽����0���x�����߶����
		i=Comm_Ram->TYMDHMS[1];//��ǰRTC��
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//ÿСʱ4��
		AddPoint+=i;
		AddPoint+=96+1;//����96��,0����1��
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//�ص㻧���߶���Pn�б�
		p32=(u32*)(ADDR_128KDATABUFF);
		for(i=0;i<(LEN_128KDATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//��ʼ�����
		CongealCount=0;//�����������
		
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//���߶��������<�貹����
			i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
			Terminal_Ram->CurveCongealCheckCount++;//���߶��������
			i*=15;
			YMDHM=CurveCongeal_YMDHM();//��ǰ��׼���߶���ʱ��YMDHM
			YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
			i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
			if(i!=0xffffffff)
			{//ԭ����ͬʱ��
				continue;
			}
			MWR(YMDHM,ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//������Ч(����ʱ��)
			MR(ADDR_128KDATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//�����������
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_128KDATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//�������򶳽�洢��0
				break;
			}
		}
		if(CongealCount!=0)//�����������
		{
			MW(ADDR_128KDATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//��ʼ�����
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//ԭ����־
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#else
	 	//��128KDATABUFF(ר��)
	  #if ((LEN_CURVECONGEAL*2)>LEN_DATABUFF)
		//����ǰʱ�䵽����0���x�����߶����
		i=Comm_Ram->TYMDHMS[1];//��ǰRTC��
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//ÿСʱ4��
		AddPoint+=i;
		AddPoint+=1;//0����1��
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		i*=15;
		YMDHM=CurveCongeal_YMDHM();//��ǰ��׼���߶���ʱ��YMDHM
		YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
		CreateCurveCongealNo(YMDHM);//�������߶����;����:0=û����(ԭ�Ѵ���),1=����
		Terminal_Ram->CurveCongealCheckCount++;//���߶��������
		return;
	  #else
		//����ǰʱ�䵽����0���x�����߶����
		i=Comm_Ram->TYMDHMS[1];//��ǰRTC��
		i=bcd_hex(i);
		i/=15;
		AddPoint=Comm_Ram->TYMDHMS[2];//��ǰRTCʱ
		AddPoint=bcd_hex(AddPoint);
		AddPoint*=4;//ÿСʱ4��
		AddPoint+=i;
		AddPoint+=96+1;//����96��,0����1��
		i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
		if(i>=AddPoint)
		{
			return;
		}
		KeyUserPnList();//�ص㻧���߶���Pn�б�
		p32=(u32*)(ADDR_DATABUFF);
		for(i=0;i<(LEN_DATABUFF/4);i++)
		{
			p32[i]=0xffffffff;
		}
		SatrtCongealNo=MRR(ADDR_TCongealCount+7,2);
		if((SatrtCongealNo&0x7fff)>=MaxCurveCongeal)
		{
			MWR(SatrtCongealNo&0x8000,ADDR_TCongealCount+7,2);
		}
		SatrtCongealNo&=0x7fff;//��ʼ�����
		CongealCount=0;//�����������
		while(Terminal_Ram->CurveCongealCheckCount<AddPoint)
		{//���߶��������<�貹����
			i=Terminal_Ram->CurveCongealCheckCount;//���߶��������
			Terminal_Ram->CurveCongealCheckCount++;//���߶��������
			i*=15;
			YMDHM=CurveCongeal_YMDHM();//��ǰ��׼���߶���ʱ��YMDHM
			YMDHM=YMDHM_SubM(YMDHM,i);//������ʱ�ּ�m��,���ؼ�m�ֺ��������ʱ��
			i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
			if(i!=0xffffffff)
			{//ԭ����ͬʱ��
				continue;
			}
			MWR(YMDHM,ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount),5);//������Ч(����ʱ��)
			MR(ADDR_DATABUFF+(LEN_CURVECONGEAL*CongealCount)+ADDR_KeyUserPnListCongeal,ADDR_KeyUserPnList,2*MaxKeyUser);
			CongealCount++;//�����������
			if(((LEN_CURVECONGEAL*(CongealCount+1))>LEN_DATABUFF)||((SatrtCongealNo+CongealCount)>=MaxCurveCongeal))
			{//�������򶳽�洢��0
				break;
			}
		}
		if(CongealCount!=0)//�����������
		{
			MW(ADDR_DATABUFF,ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*SatrtCongealNo),LEN_CURVECONGEAL*CongealCount);
			SatrtCongealNo+=CongealCount;//��ʼ�����
			if(SatrtCongealNo>=MaxCurveCongeal)
			{
				SatrtCongealNo=0x8000;
			}
			i=MRR(ADDR_TCongealCount+7,2);
			i&=0x8000;//ԭ����־
			SatrtCongealNo|=i;
			MWR(SatrtCongealNo,ADDR_TCongealCount+7,2);
		}
		return;
	  #endif
	#endif//#if IC_SDRAM
#endif//#if(RMM_RS485_IntegralCongeal==2)//RS485ͨ�����������1-24��������߶���;0=��,1=��1-24����,2=����
	}
	Terminal_Ram->CongealFlags|=0x2;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	if(Terminal_Ram->CurveCongealTask==0)//���߶�������0-24(�������1-24��)
	{
		YMDHMsour=MRR(ADDR_TCongealCount+9,5);
		YMDHM=CurveCongeal_YMDHM();//��ǰ��׼���߶���ʱ��YMDHM
		if(YMDHM==YMDHMsour)
		{//�Ѷ���
			return;
		}
	}
	Terminal_Ram->CurveCongealCheckCount=0;//���߶��������
	//����
	if(Terminal_Ram->CurveCongealTask>24)//���߶�������0-24(�������1-24��)
	{
		Terminal_Ram->CurveCongealTask=0;//���߶�������0-24(�������1-24��)
		Terminal_Ram->CongealFlags|=2;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
		return;
	}
	Terminal_Ram->CongealFlags&=0xfd;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	if(Terminal_Ram->CurveCongealTask==0)//���߶�������0-24(�������1-24��)
	{//��ǰ�㶳��
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_CURVECONGEAL);
		MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_CURVECONGEALFLAG,1);//�����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
		AFN0C_Full_CurveCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start);//AFN0Cʵʱ�����������߶��Ỻ��
		i=GetCurveCongealNo(YMDHM);//�õ����߶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if(i==0xffffffff)
		{//ԭ����û��ͬʱ��
			MR(ADDR_DATABUFF,ADDR_TCongealCount+7,2);
			p8=(u8 *)(ADDR_DATABUFF);
			i=(p8[0]+(p8[1]<<8))&0x7fff;
			if(i>=MaxCurveCongeal)
			{
				i=(p8[0]+(p8[1]<<8))&0x8000;
				MWR(i,ADDR_TCongealCount+7,2);
				i=0;
			}
		}
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		p32[0]=2;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_CURVECONGEAL_START+(LEN_CURVECONGEAL*i);
		p32[3]=LEN_CURVECONGEAL;
		p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
		MWR(YMDHM,ADDR_Class2Congeal_AutoRW+20,5);//������Ч(����ʱ��)
		//��������ݴ���(������ԭʼ�����ڼ���ʱ�Ѵ���)

		return;
	}

	Terminal_Ram->CurveCongealTask=100;//���߶�������0-24(��ʱ,��1-24ʱ)
}


void AFN0C_Full_DateCongeal_Buff(u32 Addr_CongealBuff)//AFN0Cʵʱ���������ն��Ỻ��
{
	u32 Fn;
	u32 Pn;
	u32 Class2Data_MaxFn;
/*
	u16 *pn16;
	PnVSSetnoList(ADDR_PnVSSetList);//�������Pn��ӦAFN04F10��������б�
	pn16=(u16 *)(ADDR_PnVSSetList);
*/
	MC(0xFF,Addr_CongealBuff,3);//����ʱ����Ϊ0XFF
	Class2Data_MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<Class2Data_MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==0)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//p0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//ԭʼ��ַ=0,�޴�����
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
						{//ԭʼ��ַ=0,�޴�����
							MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							switch(Fn)
							{//��������ͳ������
								case 121://F121 A��г��Խ����ͳ������
								case 122://F122 B��г��Խ����ͳ������
								case 123://F123 C��г��Խ����ͳ������
									MC(0xee,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
									for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
									{
										MC(19,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),1);
									}
									break;
							}
						}
						else
						{
							for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
							{
								switch(Fn)
								{
								#if (USER/100)==11//����
									case 27://�������յ�ѹͳ������
										//���ֵ�ѹͳ�����ݳ����ɲ��������ö�ȡ�ĵ������
										if(Pn<AFN0DF27MaxPn_ReadMeter)
										{
											if(Check_ACSamplePn(Pn+1))//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
											{
												MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_AFN0DF27_SOUR+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
												break;
											}
										}
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								#endif
									case 153://F153 �ն�����������й�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5,5);//A�������й�����ʾֵ
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18,5);//B�������й�����ʾֵ
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18,5);//C�������й�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 154://F154 �ն�����������޹�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+10,4);//A�������޹�����ʾֵ
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+10,4);//B�������޹�����ʾֵ
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+10,4);//C�������޹�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 155://F155 �ն�����෴���й�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+5,5);//A�෴���й�����ʾֵ
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18+5,5);//B�෴���й�����ʾֵ
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18+5,5);//C�෴���й�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 156://F156 �ն�����෴���޹�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+14,4);//A�෴���޹�����ʾֵ
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+14,4);//B�෴���޹�����ʾֵ
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+14,4);//C�෴���޹�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									default:
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								}
							}
						}
					}
				}
			}
		}
	}	
}

void Terminal_DateCongeal(void)//�ն���
{
	u32 i;
	u32 Fn;
	u8 * p8;
	u32 * p32;
	u32 x;
	#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	u32 y;
	u32 CongealFlag;//�����־:0=û,1=ԭ����
	#endif

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
	if(p32[3]!=0x0)
	{//ԭ��2���ݶ���AUTOIOWû���
		return;
	}
	if(Terminal_Ram->CongealFlags&4)
	{
	#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		return;
	#endif
	#if RMM_DateCongeal_0xEE==1//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		//���ն����
		i=Terminal_Ram->DateCongealCheckCount;//�ն����������
		if(i>=3)
		{
			return;
		}
		x=MRR(ADDR_TYMDHMS+3,3);//��ǰ������
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		for(;i!=0;i--)
		{
			x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		}
		CreateDateCongealNo(x);//�����ն����;����:����:ԭ�Ѵ��ڻ��´����Ķ����
		Terminal_Ram->DateCongealCheckCount++;//�ն����������
		return;
	#endif
	}
	Terminal_Ram->CongealFlags|=0x4;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	i=MRR(ADDR_TCongealCount+16,3);
	if(MRR(ADDR_TYMDHMS+3,3)==i)
	{//�Ѷ���
		return;
	}
	Terminal_Ram->DateCongealCheckCount=0;//�ն����������
	//����
	Terminal_Ram->CongealFlags&=0xfb;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	if((Terminal_Ram->CongealFlags&0x3)!=0x3)
	{//Сʱ��������߶���û���
		return;
	}
	
	#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	//�ն������(���ճ������ݲ���Ϊ�����ն���)
	i=MRR(ADDR_TYMDHMS+3,3);
	x=MRR(ADDR_TCongealCount+32,3);//�ն�����µ�������(���ճ������ݲ���Ϊ�����ն���)
	if(i!=x)
	{
		MWR(i,ADDR_TCongealCount+32,3);//�ն�����µ�������(���ճ������ݲ���Ϊ�����ն���)
		i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
		i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
		i=GetDateCongealNo(i);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if((i!=0xffffffff)&&(i<MaxDateCongeal))
		{//�������ն���
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//��ԭ�����ն�������
		}
		else
		{
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//�嶳�Ỻ��
			i=MRR(ADDR_TCongealCount+14,1);
			i&=0x7f;
			if(i>=MaxDateCongeal)
			{
				i=MRR(ADDR_TCongealCount+14,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+14,1);
				i=0;
			}
		}
		x=MRR(ADDR_TYMDHMS+3,3);//��Ч����ʱ��
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,0,0,x,0);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee

		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,3);//����ʱ������Ч
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		p32[0]=0x40002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
		p32[3]=LEN_DATECONGEAL;
		p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
		i=MRR(ADDR_TYMDHMS+3,3);
		i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
		i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
		MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//������Ч(����ʱ��)
		return;
	}
	//������2�ն���
	x=MRR(ADDR_TYMDHMS+3,3);
	i=MRR(ADDR_TCongealCount+35,3);//�ն�����µ�������
	if(i!=x)
	{
		MWR(x,ADDR_TCongealCount+35,3);//�ն�����µ�������
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		y=x;//������ʱ��
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		i=GetDateCongealNo(x);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if((i!=0xffffffff)&&(i<MaxDateCongeal))
		{//����2�ն���,�Ҷ������Ч
			CongealFlag=1;//�����־:0=û,1=ԭ����
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//��ԭ��������
		}
		else
		{//û����Ż���Ч
			CongealFlag=0;//�����־:0=û,1=ԭ����
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//�嶳�Ỻ��
			i=MRR(ADDR_TCongealCount+14,1);
			i&=0x7f;
			if(i>=MaxDateCongeal)
			{
				i=MRR(ADDR_TCongealCount+14,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+14,1);
				i=0;
			}
		}
		x=LastSource_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,y);//��2-3�ճ���ԭʼ�������붳�Ỻ��;���:LastYMD=�����ʱ��;����:0=û��������,1=��
		if((CongealFlag==0)||(x!=0))//�����־:0=û,1=ԭ����
		{
			MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,3);//����ʱ������Ч
			p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
			p32[0]=0x50002;
			p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
			p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
			p32[3]=LEN_DATECONGEAL;
			p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
			i=MRR(ADDR_TYMDHMS+3,3);
			i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//������Ч(����ʱ��)
			return;
		}
	}
	//������3�ն���
	x=MRR(ADDR_TYMDHMS+3,3);
	i=MRR(ADDR_TCongealCount+38,3);//�ն�����µ�������
	if(i!=x)
	{
		MWR(x,ADDR_TCongealCount+38,3);//�ն�����µ�������
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		y=x;//������ʱ��
		x=YMD_Sub1D(x);//�����ռ�1��,���ؼ�1���������
		i=GetDateCongealNo(x);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if((i!=0xffffffff)&&(i<MaxDateCongeal))
		{//����3�ն���,�Ҷ������Ч
			CongealFlag=1;//�����־:0=û,1=ԭ����
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i),LEN_DATECONGEAL);//��ԭ��������
		}
		else
		{//û����Ż���Ч
			CongealFlag=0;//�����־:0=û,1=ԭ����
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_DATECONGEAL);//�嶳�Ỻ��
			i=MRR(ADDR_TCongealCount+14,1);
			i&=0x7f;
			if(i>=MaxDateCongeal)
			{
				i=MRR(ADDR_TCongealCount+14,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+14,1);
				i=0;
			}
		}
		x=LastSource_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,y);//��2-3�ճ���ԭʼ�������붳�Ỻ��;���:LastYMD=�����ʱ��;����:0=û��������,1=��
		if((CongealFlag==0)||(x!=0))//�����־:0=û,1=ԭ����
		{
			MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,3);//����ʱ������Ч
			p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
			p32[0]=0x60002;
			p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
			p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
			p32[3]=LEN_DATECONGEAL;
			p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
			i=MRR(ADDR_TYMDHMS+3,3);
			i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
			MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//������Ч(����ʱ��)
			return;
		}
	}
	#endif
	
	//���ն���
	MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_DATECONGEALFLAG,1);//�����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
	AFN0C_Full_DateCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start);//AFN0Cʵʱ���������ն��Ỻ��
	x=MRR(ADDR_TYMDHMS+3,3);//��Ч����ʱ��
	#if RMM_DateCongeal_0xEE==0//2�������ն���û��������ն�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,0,0,0,0);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
	#else
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,0,0,x,1);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
	#endif

	i=MRR(ADDR_TYMDHMS+3,3);
	i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
	i=GetDateCongealNo(i);//�õ��ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{//ԭ����û��ͬʱ��
		MR(ADDR_DATABUFF,ADDR_TCongealCount+14,1);
		p8=(u8 *)(ADDR_DATABUFF);
		i=p8[0]&0x7f;
		if(i>=MaxDateCongeal)
		{
			i=p8[0]&0x80;
			MWR(i,ADDR_TCongealCount+14,1);
			i=0;
		}
	}
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
	p32[0]=0x10002;
	p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
	p32[2]=ADDR_DATECONGEAL_START+(LEN_DATECONGEAL*i);
	p32[3]=LEN_DATECONGEAL;
	p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
	i=MRR(ADDR_TYMDHMS+3,3);
	i=YMD_Sub1D(i);//�����ռ�1��,���ؼ�1���������
	MWR(i,ADDR_Class2Congeal_AutoRW+20,3);//������Ч(����ʱ��)

	//��������ݴ���
	for(Fn=0;Fn<Get_Class2Data_MaxFn();Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==0)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
			{
				switch(Terminal_Class2DataLib[Fn].SourInit)//�����ԭʼ���ݴ���:0=����,1=��Ϊ0,2=��Ϊ0xee,3=�ó�ֵ
				{
					case 0://0=����
						break;
					case 1://1=��Ϊ0
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//ԭʼ���ݵ�ַ��Ч
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
							#if AFN0DF5MaxPn==0
								switch(Fn)
								{
									case 5://�����й����������ܡ�����1��M��
									case 6://�����޹����������ܡ�����1��M��
									case 7://�����й����������ܡ�����1��M��
									case 8://�����޹����������ܡ�����1��M��
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*AFN0CF41MaxPn);
										break;
									default:
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
										break;
								}
							#else
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							#endif
							}
						}
						break;
					case 2://2=��Ϊ0xee
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//ԭʼ���ݵ�ַ��Ч
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
					case 3://3=�ó�ֵ
						break;
					case 4://4=��Ϊ0xff
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//ԭʼ���ݵ�ַ��Ч
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
				}
			}
		}
	}
	//ͬʱ��0Сʱ���������������ԭʼ����
	MC(0x0,ADDR_AFN0CF83_SOUR,4*AFN0CF83MaxPn);
	MC(0x0,ADDR_AFN0CF84_SOUR,4*AFN0CF84MaxPn);
	//ͬʱ��0���߶��������������ԭʼ����
	MC(0x0,ADDR_AFN0DF75_SOUR,4*AFN0DF75MaxPn);
	MC(0x0,ADDR_AFN0DF76_SOUR,4*AFN0DF76MaxPn);
	
#if IC_SDRAM
	//�ϴ�RAM���浽FLASH��ʵʱ��ʱ(�ϵ���Ϊ0,���¶������Ϊ0xffʹ�������б���)
	MC(0xff,ADDR_HOURFLASH_RAM_START,1);
	//��0�ն���A ���ѹг����ѹ�����ʡ�г������95%����ֵȫ��ԭʼ����
	MC(0,ADDR_AFN0DF124_SOUR,LEN_AFN0DF124_SOUR*AFN0DF124MaxPn);
	//��0�ն���B ���ѹг����ѹ�����ʡ�г������95%����ֵȫ��ԭʼ����
	MC(0,ADDR_AFN0DF125_SOUR,LEN_AFN0DF125_SOUR*AFN0DF125MaxPn);
	//��0�ն���C ���ѹг����ѹ�����ʡ�г������95%����ֵȫ��ԭʼ����
	MC(0,ADDR_AFN0DF126_SOUR,LEN_AFN0DF126_SOUR*AFN0DF126MaxPn);
	//��0�ն��������ѹ��������ƽ���95%����ֵȫ��ԭʼ����
	MC(0,ADDR_AFN0DF211_SOUR,LEN_AFN0DF211_SOUR*AFN0DF211MaxPn);
#endif

#if (USER/100)==5//�û���ʶ:�Ϻ�ϵ
	MC(0xff,ADDR_AFN0CF242_Next,6*AFN0CF242MaxCount);//���յ�¼��վ��Ϣ
	MC(0xff,ADDR_AFN0CF243,24);//�����ź�ǿ��
#endif
#if (USER/100)==6//�û���ʶ:����ϵ
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		//��������AFN0CF25,F26
		if(Comm_Ram->TYMDHMS[3]!=0x01)
		{//ÿ��1�����¶������
			MC(0xEE,ADDR_AFN0CF25,67*AFN0CF25MaxPn);
			MC(0xEE,ADDR_AFN0CF26,61*AFN0CF26MaxPn);
		}
	#endif
#endif
#if (USER/100)==11//����
	//��0���籣���е��յ�ѹͳ��,������Ѹ�ָ��RAM
	MC(0,ADDR_AFN0DF27_SOUR,(66+27)*AFN0DF27MaxPn);
#endif
}

void AFN0C_Full_RMDateCongeal_Buff(u32 Addr_CongealBuff,u32 ADDR_CongealFlags)//AFN0Cʵʱ�������볭���ն��Ỻ��;���:������ֶ˿ںŶ����־�Ѽ���
{
	u32 Fn;
	u32 Pn;
	u32 MaxFn;
	u32 i;
	u8 *p8f;
	
	p8f=(u8*)ADDR_CongealFlags;//������ֶ˿��足���־
	MC(0xff,Addr_CongealBuff,3);//����ʱ����Ϊ0xff
	MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==1)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//P0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//ԭʼ��ַ=0,�޴�����
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//Pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
						{//ԭʼ��ַ=0,�޴�����
							MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
						}
						else
						{
							for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
							{
								if(Terminal_Class2DataLib[Fn].PnType==pnType)//���������
								{
									i=p8f[Pn/8];
									i&=1<<(Pn%8);
									if(i==0)
									{//�ֶ˿ڱ������㶳���־=0
										continue;
									}
								}
							#if RMDateCongealSour==0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
								MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
							#else
								MC(0xff,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
							#endif
							}
						}
					}
				}
			}
		}
	}
}

void RMDateCongealFlags(u32 PORTf,u32 ADDR_CongealFlags)//�����ն����������ŷֶ˿ڶ����־;���:�˿ڱ�־B1-B31�ֱ��ʾ�˿�1-�˿�31,��Ż���;����:B0-Bn�ֱ��ʾ�������1-(n+1),λ0=������,λ1=����
{
	u32 i;
	u32 Meter;
	u32 Pn;
	u8 *p8d;
	u32 Addr;

	p8d=(u8*)(ADDR_CongealFlags);
	MC(0xff,ADDR_CongealFlags,(AFN04F10MaxPn+7)/8);//��ʼΪȫ���㶳��
	#if (LEN_AFN04F10_Pn*AFN04F10MaxPn)>LEN_128KDATABUFF
		#error
	#endif
	MR(ADDR_128KDATABUFF,ADDR_AFN04F10,LEN_AFN04F10_Pn*AFN04F10MaxPn);
	Addr=ADDR_128KDATABUFF+2;
	//Addr=ADDR_AFN04F10+2;
	for(Meter=0;Meter<AFN04F10MaxPn;Meter++)
	{
		Pn=MRR(Addr,2);
		if((Pn<=AFN04F10MaxPn)&&(Pn!=0))
		{//Pn��Ч
			Pn--;
			i=MRR(Addr+2,1);
			i&=0x1f;
			i=1<<i;
			i&=PORTf;
			if(i==0)
			{//���˿ڲ�����
				i=1<<(Pn%8);
				i=~i;
				p8d[Pn/8]&=i;
			}
		}
		Addr+=LEN_AFN04F10_Pn;
	}
}

void Terminal_RMDateCongeal(void)//�����ն���
{
	u32 i;
	u32 x;
	u32 hm;
	u32 d;
	u32 PORT;
	u8 * p8;
	u32 * p32;
	
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
	if(p32[3]!=0x0)
	{//ԭ��2���ݶ���AUTOIOWû���
		return;
	}
	if(Terminal_Ram->CongealFlags&8)
	{
		return;
	}
	Terminal_Ram->CongealFlags|=0x8;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���

//�������պ�ʱ�ֶ���
	PORT=0;//����˿ڱ�־B1-B31�ֱ��ʾ�˿�1-�˿�31
	p8=(u8 *)(ADDR_TYMDHMS);
	d=p8[3];
	d=bcd_hex(d);
	d=1<<d;
	hm=p8[1]+(p8[2]<<8);//��ǰʱ��
	//ͨ�Ŷ˿�1 ���������ӿ�ͨ��
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*0),4);
	x<<=1;
	if(d&x)
	{//������
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*0),2);
		if(hm==x)
		{//��������ʱ��
			PORT|=1<<1;
		}
	}
	//ͨ�Ŷ˿�2 RS485-1
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*1),4);
	x<<=1;
	if(d&x)
	{//������
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*1),2);
		if(hm==x)
		{//��������ʱ��
			PORT|=1<<2;
		}
	}
	//ͨ�Ŷ˿�3 RS485-2
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*2),4);
	x<<=1;
	if(d&x)
	{//������
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*2),2);
		if(hm==x)
		{//��������ʱ��
			PORT|=1<<3;
		}
	}
	//ͨ�Ŷ˿�4 RS485-3
#if (Project/100)==5//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*3),4);
	x<<=1;
	if(d&x)
	{//������
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*3),2);
		if(hm==x)
		{//��������ʱ��
			PORT|=1<<4;
		}
	}
#endif
	//ͨ�Ŷ˿�31 �ز�����
#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
	x=MRR(ADDR_AFN04F33+3+((14+(4*24))*30),4);
	x<<=1;
	if(d&x)
	{//������
		x=MRR(ADDR_AFN04F33+7+((14+(4*24))*30),2);
		if(hm==x)
		{//��������ʱ��
			PORT|=0x80000000;
		}
	}
#endif
	if(PORT==0x0)
	{//û�˿�Ҫ�����ն���
		return;
	}

	if(MRR(ADDR_TYMDHMS+1,5)==MRR(ADDR_TCongealCount+23,5))
	{//�Ѷ���
		return;
	}
	//����
//Congeal:
	Terminal_Ram->CongealFlags&=0xf7;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	i=GetRMDateCongealNo(MRR(ADDR_TYMDHMS+3,3));//�õ������ն�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{//ԭ����û��ͬʱ��
		MR(ADDR_DATABUFF,ADDR_TCongealCount+21,1);
		p8=(u8 *)(ADDR_DATABUFF);
		i=p8[0]&0x7f;
		if(i>=MaxRMDateCongeal)
		{
			i=p8[0]&0x80;
			MWR(i,ADDR_TCongealCount+21,1);
			i=0;
		}
	#if RMDateCongealSour==0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
		//�建��Ϊ0xee
		MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_RMDATECONGEAL);
	#else
		//�建��Ϊ0xff
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,LEN_RMDATECONGEAL);
	#endif
	}
	else
	{//ԭ��������ͬʱ��
		//��ԭ���ᵽ����
		MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i),LEN_RMDATECONGEAL);
	}
	
	RMDateCongealFlags(PORT,ADDR_DATABUFF);//�����ն����������ŷֶ˿ڶ����־;���:�˿ڱ�־B1-B31�ֱ��ʾ�˿�1-�˿�31,��Ż���;����:B0-Bn�ֱ��ʾ�������1-(n+1),λ0=������,λ1=����
#if RMDateCongealSour==0//�����ն���ԭʼ����:0=��ǰʵʱֵ,1=�Ͻ����ն���ֵ
	MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_RMDATECONGEALFLAG,1);//�����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
#else
	MC(0xff,ADDR_AFN0D_Congeal_Buff_Start+ADDR_RMDATECONGEALFLAG,1);//�����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
#endif
	AFN0C_Full_RMDateCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start,ADDR_DATABUFF);//AFN0Cʵʱ�������볭���ն��Ỻ��;���:������ֶ˿ںŶ����־�Ѽ���
//	//x=MRR(ADDR_TYMDHMS+3,3);//��Ч����ʱ��
//	//Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,1,PORT,0,0);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee

	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
	p32[0]=0x20002;
	p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
	p32[2]=ADDR_RMDATECONGEAL_START+(LEN_RMDATECONGEAL*i);
	p32[3]=LEN_RMDATECONGEAL;
	p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
	MW(ADDR_TYMDHMS+3,ADDR_Class2Congeal_AutoRW+20,3);//������Ч(����ʱ��)
	//��������ݴ���

}

void AFN0C_Full_MonthCongeal_Buff(u32 Addr_CongealBuff)//AFN0Cʵʱ���������¶��Ỻ��
{
	u32 Fn;
	u32 Pn;
	u32 Class2Data_MaxFn;
/*
	u16 *pn16;
	PnVSSetnoList(ADDR_PnVSSetList);//�������Pn��ӦAFN04F10��������б�
	pn16=(u16 *)(ADDR_PnVSSetList);
*/
	MC(0xff,Addr_CongealBuff,2);//����ʱ����Ϊ0xff
	Class2Data_MaxFn=Get_Class2Data_MaxFn();
	for(Fn=0;Fn<Class2Data_MaxFn;Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==2)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
			{
				if(Terminal_Class2DataLib[Fn].PnType==0x0)
				{//p0
					if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
					{//ԭʼ��ַ=0,�޴�����
						MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
					else
					{
						MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
					}
				}
				else
				{//pn
					if(Terminal_Class2DataLib[Fn].PnMax!=0x0)
					{
						if(Terminal_Class2DataLib[Fn].SourAddr==0x0)
						{//ԭʼ��ַ=0,�޴�����
							MC(0x0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
						}
						else
						{
							for(Pn=0;Pn<Terminal_Class2DataLib[Fn].PnMax;Pn++)
							{
								switch(Fn)
								{
								#if (USER/100)==11//����
									case 35://�������µ�ѹͳ������
										//���ֵ�ѹͳ�����ݳ����ɲ��������ö�ȡ�ĵ������
										if(Pn<AFN0DF35MaxPn)
										{
											if(Check_ACSamplePn(Pn+1))//��Ч���ɲ�����ż��;����0=��Ч,1=��Ч
											{
												MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_AFN0DF35_SOUR+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
												break;
											}
										}
//										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										MC(0xFF,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								#endif
									case 157://F157 �¶�����������й�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5,5);//A�������й�����ʾֵ
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18,5);//B�������й�����ʾֵ
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18,5);//C�������й�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 158://F158 �¶�����������޹�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+10,4);//A�������޹�����ʾֵ
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+10,4);//B�������޹�����ʾֵ
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+10,4);//C�������޹�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 159://F159 �¶�����෴���й�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+5,5);//A�෴���й�����ʾֵ
										MR(ADDR_DATABUFF+10,ADDR_AFN0CF31+(59*Pn)+5+18+5,5);//B�෴���й�����ʾֵ
										MR(ADDR_DATABUFF+15,ADDR_AFN0CF31+(59*Pn)+5+18+18+5,5);//C�෴���й�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 160://F160 �¶�����෴���޹�����ʾֵ
										MR(ADDR_DATABUFF,ADDR_AFN0CF31+(59*Pn),5);//�ն˳���ʱ��
										MR(ADDR_DATABUFF+5,ADDR_AFN0CF31+(59*Pn)+5+14,4);//A�෴���޹�����ʾֵ
										MR(ADDR_DATABUFF+9,ADDR_AFN0CF31+(59*Pn)+5+18+14,4);//B�෴���޹�����ʾֵ
										MR(ADDR_DATABUFF+13,ADDR_AFN0CF31+(59*Pn)+5+18+18+14,4);//C�෴���޹�����ʾֵ
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),ADDR_DATABUFF,Terminal_Class2DataLib[Fn].PnLen);
										break;
									case 216://F216 ��������ܱ������Ϣ,��ԭʼ����AFN0CF168����5�ֽڹ��ϵ���
										MC(0,Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].SourPnLen);
										break;
									default:
										MR(Addr_CongealBuff+Terminal_Class2DataLib[Fn].OfficeAddr+(Terminal_Class2DataLib[Fn].PnLen*Pn),Terminal_Class2DataLib[Fn].SourAddr+(Terminal_Class2DataLib[Fn].SourPnLen*Pn),Terminal_Class2DataLib[Fn].PnLen);
										break;
								}
							}
						}
					}	
				}
			}
		}
	}
}

void Terminal_MonthCongeal(void)//�¶���
{
	u32 i;
	u32 Fn;
	u8 * p8;
	u32 * p32;
	
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
	if(p32[3]!=0x0)
	{//ԭ��2���ݶ���AUTOIOWû���
		return;
	}
	if(Terminal_Ram->CongealFlags&0x10)
	{
	#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		return;
	#endif
	#if RMM_MonthCongeal_0xEE==1//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
		//���¶����
		i=Terminal_Ram->MonthCongealCheckCount;//�¶����������
		if(i>=2)
		{
			return;
		}
		Fn=MRR(ADDR_TYMDHMS+4,2);//��ǰ������
		Fn=YM_Sub1M(Fn);//���¼�1��,���ؼ�1�������
		for(;i!=0;i--)
		{
			Fn=YM_Sub1M(Fn);//���¼�1��,���ؼ�1�������
		}
		CreateMonthCongealNo(Fn);//�����¶����;����:����:ԭ�Ѵ��ڻ��´����Ķ����
		Terminal_Ram->MonthCongealCheckCount++;//�¶����������
		return;
	#endif
	}
	Terminal_Ram->CongealFlags|=0x10;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	i=MRR(ADDR_TCongealCount+30,2);
	if(MRR(ADDR_TYMDHMS+4,2)==i)
	{//�Ѷ���
		return;
	}
	Terminal_Ram->MonthCongealCheckCount=0;//�¶����������
	Terminal_Ram->CongealFlags&=~0x10;//B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
	if((Terminal_Ram->CongealFlags&0x7)!=0x7)
	{//Сʱ����,���߶���,�ն���û���
		return;
	}
	
	#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	//�¶������(���³������ݲ���Ϊ�����¶���)
	i=MRR(ADDR_TYMDHMS+4,2);
	Fn=MRR(ADDR_TCongealCount+19,2);//���³������ݲ���Ϊ�����¶��������
	if(i!=Fn)
	{
		MWR(i,ADDR_TCongealCount+19,2);//���³������ݲ���Ϊ�����¶��������
		i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
		i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
		i=GetMonthCongealNo(i);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
		if((i!=0xffffffff)&&(i<MaxMonthCongeal))
		{//�������¶���
			MR(ADDR_AFN0D_Congeal_Buff_Start,ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i),LEN_MONTHCONGEAL);//��ԭ�����¶�������
		}
		else
		{
			MC(0xee,ADDR_AFN0D_Congeal_Buff_Start,LEN_MONTHCONGEAL);//�嶳�Ỻ��
			i=MRR(ADDR_TCongealCount+28,1);
			i&=0x7f;
			if(i>=MaxMonthCongeal)
			{
				i=MRR(ADDR_TCongealCount+28,1);
				i&=0x80;
				MWR(i,ADDR_TCongealCount+28,1);
				i=0;
			}
		}
		Fn=MRR(ADDR_TYMDHMS+4,2);//��Ч����ʱ��
		Fn=YM_Sub1M(Fn);//���¼�1��,���ؼ�1�������
		Fn<<=8;
		Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,2,0,Fn,0);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
		
		MC(0xff,ADDR_AFN0D_Congeal_Buff_Start,2);//����ʱ������Ч
		p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
		p32[0]=0x70002;
		p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
		p32[2]=ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i);
		p32[3]=LEN_MONTHCONGEAL;
		p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
		i=MRR(ADDR_TYMDHMS+4,2);
		i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
		i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
		MWR(i,ADDR_Class2Congeal_AutoRW+20,2);//������Ч(����ʱ��)
		return;
	}
	#endif

	//���¶���
	MC(0xee,ADDR_AFN0D_Congeal_Buff_Start+ADDR_MONTHCONGEALFLAG,1);//�����־:0xEE��������,0xFF������(ֻ�е��ܱ������������ݻᲹ,��������Ϊ��Ч,���ڷ�BCD������Ҳ��ʶΪ��Ч)
	AFN0C_Full_MonthCongeal_Buff(ADDR_AFN0D_Congeal_Buff_Start);//AFN0Cʵʱ���������¶��Ỻ��
	i=MRR(ADDR_TYMDHMS+4,2);//��Ч����ʱ��
	i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
	i<<=8;
	#if RMM_MonthCongeal_0xEE==0//2�������¶���û��������¶�������ǰ��0xff;0=��(��ʵʱ���ݶ���),1=��
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,2,0,0,0);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
	#else
	Source_Full_CongealBuff(ADDR_AFN0D_Congeal_Buff_Start,2,0,i,1);//����ԭʼ�������붳�Ỻ��;��������:0=�ն���,1=�����ն���,2=�¶���,3=����;�˿�B0=��,B1-B31�ֱ��ʾ�˿�1-31;YMD:��Ч����ʱ��������(ֵ0��ʾ���Ƚ�);Full0xee!=0��ʾDL645-2007����ԭʼ����ʱ��0xee
	#endif


	i=MRR(ADDR_TYMDHMS+4,2);
	i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
	i=GetMonthCongealNo(i);//�õ��¶�����ͬʱ��Ķ����,����=0xffffffff��ʾû�ҵ�
	if(i==0xffffffff)
	{//ԭ����û��ͬʱ��
		MR(ADDR_DATABUFF,ADDR_TCongealCount+28,1);
		p8=(u8 *)(ADDR_DATABUFF);
		i=p8[0]&0x7f;
		if(i>=MaxMonthCongeal)
		{
			i=p8[0]&0x80;
			MWR(i,ADDR_TCongealCount+28,1);
			i=0;
		}
	}
	p32=(u32 *)(ADDR_Class2Congeal_AutoRW);//��2���ݶ����Զ�дEFLASH���Ƶ�ַ(�ֶ���)
	p32[0]=0x30002;
	p32[1]=ADDR_AFN0D_Congeal_Buff_Start;
	p32[2]=ADDR_MONTHCONGEAL_START+(LEN_MONTHCONGEAL*i);
	p32[3]=LEN_MONTHCONGEAL;
	p32[4]=p32[2];//���Ὺʼ��ַ(ʱ���ַ),ͬ��ʼֵR1
	i=MRR(ADDR_TYMDHMS+4,2);
	i=YM_Sub1M(i);//���¼�1��,���ؼ�1�������
	MWR(i,ADDR_Class2Congeal_AutoRW+20,2);//������Ч(����ʱ��)

	//��������ݴ���
	for(Fn=0;Fn<Get_Class2Data_MaxFn();Fn++)
	{
		if(Terminal_Class2DataLib[Fn].OfficeAddr!=0x0)
		{
			if(Terminal_Class2DataLib[Fn].CongealType==2)//��������:0=�ն���,1=�����ն���,2=�¶���,3=����
			{
				switch(Terminal_Class2DataLib[Fn].SourInit)//�����ԭʼ���ݴ���:0=����,1=��Ϊ0,2=��Ϊ0xee,3=�ó�ֵ
				{
					case 0://0=����
						break;
					case 1://1=��Ϊ0
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//ԭʼ���ݵ�ַ��Ч
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
							#if AFN0DF21MaxPn==0
								switch(Fn)
								{
									case 21://�����й����������ܡ�����1��M��
									case 22://�����޹����������ܡ�����1��M��
									case 23://�����й����������ܡ�����1��M��
									case 24://�����޹����������ܡ�����1��M��
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*AFN0CF45MaxPn);
										break;
									default:
										MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
										break;
								}
							#else
								MC(0x0,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							#endif
							}
						}
						break;
					case 2://2=��Ϊ0xee
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//ԭʼ���ݵ�ַ��Ч
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xee,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
					case 3://3=�ó�ֵ
						break;
					case 4://2=��Ϊ0xff
						if(Terminal_Class2DataLib[Fn].SourAddr!=0x0)
						{//ԭʼ���ݵ�ַ��Ч
							if(Terminal_Class2DataLib[Fn].PnType==0x0)
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen);
							}
							else
							{
								MC(0xff,Terminal_Class2DataLib[Fn].SourAddr,Terminal_Class2DataLib[Fn].PnLen*Terminal_Class2DataLib[Fn].PnMax);
							}
						}
						break;
				}
			}
		}
	}
	
#if IC_SDRAM
	//�ϴ�RAM���浽FLASH��ʵʱ��ʱ(�ϵ���Ϊ0,���¶������Ϊ0xffʹ�������б���)
	MC(0xff,ADDR_HOURFLASH_RAM_START,1);
	//��0�¶��������ѹ��������ƽ���95%����ֵȫ��ԭʼ����
	MC(0,ADDR_AFN0DF212_SOUR,LEN_AFN0DF212_SOUR*AFN0DF212MaxPn);
#endif

#if (USER/100)==6//�û���ʶ:����ϵ
	#if (Project/100)==2//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��,700=
		//��������AFN0CF25,F26
		MC(0xEE,ADDR_AFN0CF25,67*AFN0CF25MaxPn);
		MC(0xEE,ADDR_AFN0CF26,61*AFN0CF26MaxPn);
	#endif
#endif
#if (USER/100)==11//����
	//��0���籣���е��µ�ѹͳ��,�����AFN0DF35MaxPn��ΪAFN0DF35MaxPn_ReadMeter��ʵ�ʵ��籣���ram�ռ䲻��,����0
	MC(0,ADDR_AFN0DF35_SOUR,(66+27)*AFN0DF35MaxPn);
#endif
}









void Terminal_Class2Data_Computer(void)//�ն���2���ݼ���
{
	u32 i;
	u32 x;
	u32 y;
	u16 *p16;
	
	
	if(DataComp(((u32)Terminal_Ram->LastCongealTime),ADDR_TYMDHMS+1,5))//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
	{//������ʱ�ָ���
		if((Terminal_Ram->CongealFlags&0x1f)==0)//��ʵʱ�ӷָ���ʱ��0;���ڶ����־(0���ڶ���,1�������);B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
		{
			MR((u32)Terminal_Ram->LastCongealTime,ADDR_TYMDHMS+1,5);//�ϴζ���ʱ��ʵʱ�ӷּĴ�
		}
		else
		{
			if((Terminal_Ram->CongealFlags&0x1f)==0x1f)//��ʵʱ�ӷָ���ʱ��0;���ڶ����־(0���ڶ���,1�������);B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
			{
				Terminal_Ram->CongealFlags=0;//��ʵʱ�ӷָ���ʱ��0;���ڶ����־(0���ڶ���,1�������);B0=Сʱ����,B1=���߶���,B2=�ն���,B3=�����ն���,B4=�¶���
				MR((u32)Terminal_Ram->LastCongealTime,ADDR_TYMDHMS+1,5);//�ϴζ���ʱ��ʵʱ�ӷּĴ�
			}
		}
	}
	switch(Terminal_Ram->I2SUBTask2)
	{
		case 0:
			x=MRR(ADDR_TCongealCount+7,2);
			p16=(u16*)(ADDR_CurveCongealList);
			y=p16[0];
			if(x!=y)
			{
				if((x&0x7fff)>=MaxCurveCongeal)
				{
					x=0;
					MWR(x,ADDR_TCongealCount+7,2);
				}
				if((y&0x7fff)>=MaxCurveCongeal)
				{
					y=0;
				}
				if((x&0x8000)==0)
				{
					if(y&0x8000)
					{
						y=0;//x��0
					}
				}
				i=100;
				while(i--)
				{
					if(x==y)
					{
						break;
					}
					MR(ADDR_CurveCongealList+2+((y&0x7fff)*6),ADDR_CURVECONGEAL_START+((y&0x7fff)*LEN_CURVECONGEAL),5);
					y++;
					if((y&0x7fff)>=MaxCurveCongeal)
					{
						y=0x8000;
					}
				}
				p16[0]=y;
				return;
			}
			Terminal_CurveCongeal();//���߶���
			break;
		case 1:
			Terminal_DateCongeal();//�ն���
			break;
		case 2:
			Terminal_RMDateCongeal();//�����ն���
			break;
		case 3:
			Terminal_MonthCongeal();//�¶���
			break;
		default:
			Terminal_Ram->I2SUBTask2=0;
			return;
	}
	Terminal_Ram->I2SUBTask2+=1;
}










