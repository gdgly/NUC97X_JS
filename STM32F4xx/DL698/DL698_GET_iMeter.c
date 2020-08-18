
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_iMeter.h"
#include "../DL698/DL698_GET_iMeterLIB.h"

#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_Uart.h"
#include "../Calculate/Calculate.h"




u32 GET_iMeter_OAD(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 s;
	u32 DataAddr;
	u32 Len;
	DL698_iMeterLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	u8 *p8;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	pLib=Get_DL698_iMeter_pLib(OAD>>16);//�õ�iMeter_pLibָ��;���:OI;����:Libָ��(=0��ʾ����Ӧ��OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//���󲻴���           ��6��
		return 0;
	}
	Len=0;
	DataAddr=ADDR_128KDATABUFF;
	p8=(u8*)ADDR_128KDATABUFF;
	if(OAD&0xff00)
	{//��������
		if((OAD&0xff00)==0x0100)
		{//���߼���
			if(LENmax_TxSPACE<4)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_octet_string;//long-unsigned
			p8tx[1]=2;
			p8tx[2]=OAD>>24;
			p8tx[3]=OAD>>16;
			return 4;
		}
		switch(pLib->IC)
		{
			case 1://��������
				switch((OAD>>8)&0xff)
				{//����
					case 2://�ܼ����ʵ���������	��dyn.��	array
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(i*pLib->LENper),4);
							if(pLib->DataType==DataType_double_long_unsigned)
							{
								x=bcd_hex(x);
							}
							else
							{
								s=x&0x80000000;
								x&=0x7fffffff;
								x=bcd_hex(x);
								if(s)
								{
									x=~x;
									x++;
								}
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>24;
							p8[2]=x>>16;
							p8[3]=x>>8;
							p8[4]=x;
							Len+=5;
							p8+=5;
						}
						break;
					case 3://���㼰��λ	��static��	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					case 4://�߾����ܼ����ʵ���������	��dyn.��	array
					case 5://�߾��Ȼ��㼰��λ	��static��	Scaler_Unit
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
				break;
			case 2://���������
				switch((OAD>>8)&0xff)
				{//����
					case 2://�ܼ����������������	��dyn.��	array
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(i*pLib->LENper),3);
							if(pLib->DataType==DataType_double_long_unsigned)
							{
								x=bcd_hex(x);
							}
							else
							{
								s=x&0x800000;
								x&=0x7fffff;
								x=bcd_hex(x);
								if(s)
								{
									x=~x;
									x++;
								}
							}
							p8[0]=DataType_structure;
							p8[1]=2;
							p8[2]=pLib->DataType;
							p8[3]=x>>24;
							p8[4]=x>>16;
							p8[5]=x>>8;
							p8[6]=x;
							Len+=7;
							p8+=7;
							//����ʱ��   date_time_s
							p8[0]=DataType_date_time_s;
							x=MRR(pLib->Addr+(i*pLib->LENper)+7,1);//��
							x=bcd_hex(x);
							x+=2000;
							p8[1]=x>>8;
							p8[2]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+6,1);//��
							x=bcd_hex(x);
							p8[3]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+5,1);//��
							x=bcd_hex(x);
							p8[4]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+4,1);//ʱ
							x=bcd_hex(x);
							p8[5]=x;
							x=MRR(pLib->Addr+(i*pLib->LENper)+3,1);//��
							x=bcd_hex(x);
							p8[6]=x;
						#if(USER/100)==17
							p8[7]=0x00;//��
						#else
							p8[7]=0xFF;//��
						#endif
							Len+=8;
							p8+=8;
						}
						break;
					case 3://���㼰��λ	��static��	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
				break;
			case 3://���������
				switch((OAD>>8)&0xff)
				{//����
					case 2://������ֵ��	��dyn.��	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						switch((OAD>>16)&0xffff)
						{
							case 0x2000://��ѹ
							case 0x2002://��ѹ���
							case 0x2003://��ѹ�������
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),2);
									if(pLib->DataType==DataType_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{ 
										s=x&0x8000;
										x&=0x7fff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>8;
									p8[2]=x;
									Len+=3;
									p8+=3;
								}
								break;
							case 0x2001://����
								//��ֵ�鰴A�ࡢB�ࡢC�࣬�����߷�ʽΪ����ʱ��A��B��C�����ΪA�ࣨĳһ�ࣩ
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),3);
									if(pLib->DataType==DataType_double_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{
										s=x&0x800000;
										x&=0x7fffff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>24;
									p8[2]=x>>16;
									p8[3]=x>>8;
									p8[4]=x;
									Len+=5;
									p8+=5;
								}
								break;
							default:
								UARTCtrl->DAR=6;//���󲻴���           ��6��
								return 0;
						}
						break;
					case 3://���㼰��λ	��static��	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					case 4://����4���ߵ�����=double-long, ��λ��A���㣺-3
						if(OAD==0x20010400)
						{//����4���ߵ�����=double-long, ��λ��A���㣺-3
							x=MRR(ADDR_IN+1,3);
							s=x&0x800000;
							x&=0x7fffff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=DataType_double_long;
							p8[1]=x>>24;
							p8[2]=x>>16;
							p8[3]=x>>8;
							p8[4]=x;
							Len=5;
						}
						else
						{
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							return 0;
						}
						break;
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
				break;
			case 4://������
				switch((OAD>>8)&0xff)
				{//����
					case 2://�ܼ�������ֵ��	��dyn.��	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						switch((OAD>>16)&0xffff)
						{
							case 0x2004://�й�����
							case 0x2005://�޹�����
							case 0x2006://���ڹ���
							case 0x2007://һ����ƽ���й�����
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),3);
									if(pLib->DataType==DataType_double_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{ 
										s=x&0x800000;
										x&=0x7fffff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>24;
									p8[2]=x>>16;
									p8[3]=x>>8;
									p8[4]=x;
									Len+=5;
									p8+=5;
								}
								break;
							case 0x200A://��������
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),2);
									if(pLib->DataType==DataType_long_unsigned)
									{
										x=bcd_hex(x);
									}
									else
									{
										s=x&0x8000;
										x&=0x7fff;
										x=bcd_hex(x);
										if(s)
										{
											x=~x;
											x++;
										}
									}
									p8[0]=pLib->DataType;
									p8[1]=x>>8;
									p8[2]=x;
									Len+=3;
									p8+=3;
								}
								break;
							default:
								UARTCtrl->DAR=6;//���󲻴���           ��6��
								return 0;
						}
						break;
					case 3://���㼰��λ	��static��	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
				break;
			case 5://г��������
				switch((OAD>>8)&0xff)
				{//����
					case 2://A��n����ֵ��	��dyn.��	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(i*pLib->LENper),2);
							s=x&0x8000;
							x&=0x7fff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>8;
							p8[2]=x;
							Len+=3;
							p8+=3;
						}
						break;
					case 3://B��n����ֵ��	��dyn.��	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(2*22)+(i*pLib->LENper),2);
							s=x&0x8000;
							x&=0x7fff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>8;
							p8[2]=x;
							Len+=3;
							p8+=3;
						}
						break;
					case 4://C��n����ֵ��	��dyn.��	array  
						p8[0]=DataType_array;
						p8[1]=pLib->array;
						p8+=2;
						Len=2;
						for(i=0;i<pLib->array;i++)
						{
							x=MRR(pLib->Addr+(2*22)+(2*22)+(i*pLib->LENper),2);
							s=x&0x8000;
							x&=0x7fff;
							x=bcd_hex(x);
							if(s)
							{
								x=~x;
								x++;
							}
							p8[0]=pLib->DataType;
							p8[1]=x>>8;
							p8[2]=x;
							Len+=3;
							p8+=3;
						}
						break;
					case 5://г������n	��static��	unsigned  
							p8[0]=DataType_unsigned;
							p8[1]=0x15;
							Len=2;
						break;
					case 6://���㼰��λ	��static��	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
				break;
			case 6://���ݱ�����
				switch((OAD>>8)&0xff)
				{//����
					case 2://��ֵ	��dyn.��	instance-specific  
//						p8[0]=DataType_array;
//						p8[1]=pLib->array;
//						p8+=2;
//						Len=2;
						Len=0;
						switch((OAD>>16)&0xffff)
						{
							case 0x200F://����Ƶ��
								x=MRR(pLib->Addr+(i*pLib->LENper),2);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>8;
								p8[2]=x;
								Len+=3;
								p8+=3;
								break;
							case 0x2011://ʱ�ӵ�ص�ѹ
								x=MRR(pLib->Addr+(i*pLib->LENper),2);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>8;
								p8[2]=x;
								Len+=3;
								p8+=3;
								break;
							case 0x2014://�������״̬��1-7
								p8[0]=DataType_array;
								p8[1]=pLib->array;
								p8+=2;
								Len=2;
								for(i=0;i<pLib->array;i++)
								{
									x=MRR(pLib->Addr+(i*pLib->LENper),2);
									x=Bit_Inverse(x,16);//�Ĵ�������λ����(�ߵ�λ����),���:Data=�Ĵ�������,bit=�赹���λ����;����:λ����������
									p8[0]=pLib->DataType;
									p8[1]=16;
									p8[2]=x>>8;
									p8[3]=x;
									Len+=4;
									p8+=4;
								}
								break;
							case 0x2017://��ǰ�й�����
							case 0x2018://��ǰ�޹�����
							case 0x2019://��ǰ��������
								x=MRR(pLib->Addr+(i*pLib->LENper),3);
								s=x&0x800000;
								x&=0x7fffff;
								x=bcd_hex(x);
								if(s)
								{
									x=~x;
									x++;
								}
								p8[0]=pLib->DataType;
								p8[1]=x>>24;
								p8[2]=x>>16;
								p8[3]=x>>8;
								p8[4]=x;
								Len+=5;
								p8+=5;
								break;
							case 0x201A://��ǰ���
							case 0x201B://��ǰ���ʵ��
							case 0x201C://��ǰ���ݵ��
								x=MRR(pLib->Addr+(i*pLib->LENper),4);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>24;
								p8[2]=x>>16;
								p8[3]=x>>8;
								p8[4]=x;
								Len+=5;
								p8+=5;
								break;
							case 0x2026://��ѹ��ƽ����
							case 0x2027://������ƽ����
								x=MRR(pLib->Addr+(i*pLib->LENper),2);
								x=bcd_hex(x);
								p8[0]=pLib->DataType;
								p8[1]=x>>8;
								p8[2]=x;
								Len+=3;
								p8+=3;
								break;
							default:
								UARTCtrl->DAR=6;//���󲻴���           ��6��
								return 0;
						}
						break;
					case 3://���㼰��λ	��static��	Scaler_Unit
						p8[0]=DataType_Scaler_Unit;
						p8[1]=pLib->Scaler;
						p8[2]=pLib->Unit;
						Len=3;
						break;
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
				break;
			default:
				UARTCtrl->DAR=6;//���󲻴���           ��6��
				return 0;
		}
/*
			if(Len>pLib->LENmax)
			{
				if(pLib->LENmax!=0)
				{//�����Ǵ洢��FLASH
					UARTCtrl->DAR=9;//���ݿ鲻����         ��9��
					return 0;
				}
			}
*/
		i=OAD&0xff;
		if(i)
		{//����Ԫ��
			p8=Get_Element((u8*)DataAddr,i,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8==0)
			{
				UARTCtrl->DAR=6;//���󲻴���           ��6��
				return 0;
			}
			Len=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			DataAddr=(u32)p8;
		}
		if(Len>LENmax_TxSPACE)
		{
			GET_Request_Frame(PORTn);//��֡��־
			return 0;
		}
		MR((u32)p8tx,DataAddr,Len);
		return Len;
	}
	else
	{//0��ʾ�����������ԣ����������������
		UARTCtrl->DAR=255;
		return 0;
	}
}





