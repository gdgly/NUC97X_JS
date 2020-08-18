
//�Զ��ѱ�RS485
#include "Project.h"
#include "AutoSearchMeter_RS485.h"
#include "AutoSearchMeter.h"
#include "Terminal_ReadMeter_DL645.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_DMA.h"
#include "../Device/MEMRW.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"


void AutoSearchMeter_RS485_DL645(u32 PORTn)//�Զ��ѱ�RS485�˿ڵ�DL645
{
	u32 i;
	u32 x;
	u32 RS485PORT;
	u8 *p8;
	u8 * p8txbuff;
	u8 * p8rxbuff;
  UARTCtrl_TypeDef * UARTCtrl;
	u16 * p16fifo;
	u16 *p16timer;
	u32 ProtocolBps;

	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	p16fifo=(u16 *)Get_ADDR_UARTn(PORTn);
	p16timer=(u16 *)Get_ADDR_UARTnTimer(PORTn);

	if(UARTCtrl->Lock==0x55)
	{
		return;//0x55=ͨ�Ź������ڱ����ý�ֹ���������ٴε���
	}
	switch(PORTn)
	{
		case RS485_1PORT://RS485-1
			RS485PORT=0;
			break;
		case RS485_2PORT://RS485-2
			RS485PORT=1;
			break;
		case RS485_3PORT://RS485-3
			RS485PORT=2;
			break;
		default:
			return;
	}
	ProtocolBps=MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT)+6,1);
	switch(UARTCtrl->Task)//0=����,1=���1֡����,2=���ճ�ʱ(�����),3=��������,4=���ڷ���,5=���ڽ���
	{
		default:
			UARTCtrl->Task=0;
			break;
		case 0://0=����
			if(UARTCtrl->BatchTask>=1)//ת����������:0=��,1=�ȴ��ڿ�������,2...����
			{
				return;
			}
			if(p16timer[0]==0x0)
			{//��֡����������ʱ
				UARTCtrl->Task=3;//��������
			}
			break;
		case 3://3=��������
			if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
			{//DL645-2007
				Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x00010000,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			}
			else
			{//DL645-1997
				Read_DL645_1997_TxData(((u32)p8txbuff)+4,0x9010,MRR(ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6));//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
			}
		TX:
			if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
			{//DL645-2007
				UARTCtrl->TxByte=4+16;//TX����(�跢���ֽ���)
				UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//Ĭ��2400
			}
			else
			{//DL645-1997
				UARTCtrl->TxByte=4+14;//TX����(�跢���ֽ���)
				UARTCtrl->BpsCtrl=((ProtocolBps&0xf)<<5)+0x0b;//Ĭ��1200
			}
			MC(0xfe,(u32)p8txbuff,4);
			//p16fifo[0]=0;
			//p16fifo[1]=0;
			//p16fifo[2]=0;
			UARTCtrl->RxByte=0;//RX����(�ѽ����ֽ���)=0
			Init_UARTn(PORTn);//��ʼ��UART��,���ͨ�ſ�����������
			Start_UARTn_Tx(PORTn);//����UART�ڷ���
			UARTCtrl->Task=4;
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
			if(PORTn==RS485_1PORT)
			{
				Terminal_Ram->RS485_1_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
			if(PORTn==RS485_2PORT)
			{
				Terminal_Ram->RS485_2_TxLED_MS_Timer=50/10;//11 RS485������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
			}
#endif
			CopyString((u8 *)"485x�����Զ��ѱ�",(u8*)ADDR_DATABUFF);//�����ַ���;���ؿ����ַ����ֽ���(���ƽ���0)
			MWR(RS485PORT+1,ADDR_DATABUFF+3,1);
			MWR(0,ADDR_DATABUFF+16,1);
			WarningOccur(PORTn,(u8 *)ADDR_DATABUFF);//�澯����,��ڸ澯����͸澯�ַ���
			break;
		case 4://4=���ڷ���
			if(Wait_UARTn_Tx(PORTn)==0)//�ȴ�UART�ڷ���,����0=�������,1=����û���
			{
				UARTCtrl->Task=5;
				p16timer[0]=1000/10;//1000ms���ն�ʱ
			}
			break;
		case 5://5=���ڽ���
			DMA_UARTn_RX_Pointer(PORTn);//����DMA_UARTn����ָ��ֵ(p0)
			if(p16fifo[0]!=p16fifo[2])
			{//���ֽ��յ�
				p16fifo[2]=p16fifo[0];
				p16timer[0]=500/10;//500ms��ʱ
#if (((Project/100)==2)||((Project/100)==3)||((Project/100)==4))//����0=���ܱ�,100=�����,200=������,300=ר���ն�,400=�����ն�,500=����������,600=ͨ��ģ��
				if(PORTn==RS485_1PORT)
				{
					Terminal_Ram->RS485_1_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				}
				if(PORTn==RS485_2PORT)
				{
					Terminal_Ram->RS485_2_RxLED_MS_Timer=50/10;//11 RS485�������LEDָʾMS��ʱ��,ÿ10MS��1��ʱ��
				}
#endif
				break;
			}
			if(p16timer[0]!=0)
			{//�ȴ�ȫ���������			
				break;
			}
			i=DL645_Rx(PORTn);//DL645֡����;����0û���,1���,2��ʱ
			//p16timer[0]=50/10;//��֡����������ʱ20MS��
			switch(i)
			{
				case 0://0û���
					return;
				case 1://1���
					UARTCtrl->Task=0;//����1��
					if((p8rxbuff[8]&0x80)==0x0)
					{//���ǴӶ�֡
						return;//�ط�
					}
					if(p16fifo[0]!=p16fifo[1])
					{
						goto Over;//�ж����ֽڽ�����Ϊ��ʱ
					}
					for(i=5;i<11;i++)
					{
						if(p8txbuff[i]==0xaa)
						{
							break;
						}
					}
					if(i<11)
					{//���͵�ַ��֤
						if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
						{//DL645-2007
							Read_DL645_2007_TxData(((u32)p8txbuff)+4,0x00010000,MRR(((u32)p8rxbuff)+1,6));//DL645-2007������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
						}
						else
						{//DL645-1997
							Read_DL645_1997_TxData(((u32)p8txbuff)+4,0x9010,MRR(((u32)p8rxbuff)+1,6));//DL645-1997������֡,���:Ҫ����Ļ����ַ,���ݱ�ʶ,ͨ�ŵ�ַ
						}
						goto TX;
					}
					if(DataComp(((u32)p8rxbuff)+1,((u32)p8txbuff)+5,6)==0)//���ݱȽ�,����0=���;��Byte>8ʱ����1=�����;��Byte<=8ʱ1=Data1>Data2,2=Data1<Data2
					{
						if(((ProtocolBps>>4)==0)||((ProtocolBps>>4)==2))//B4-B7:0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
						{//DL645-2007
							//AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),30,0xeeeeeeeeeeee,ProtocolBps&0x0f,0);//�Զ�����AFN04F10�ĵ��ܱ�����
							AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),30,0x0,ProtocolBps&0x0f,0);//�Զ�����AFN04F10�ĵ��ܱ�����
						}
						else
						{//DL645-1997
							//AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),1,0xeeeeeeeeeeee,ProtocolBps&0x0f,0);//�Զ�����AFN04F10�ĵ��ܱ�����
							AutoSetAFN04F10(PORTn,MRR(((u32)p8txbuff)+5,6),1,0x0,ProtocolBps&0x0f,0);//�Զ�����AFN04F10�ĵ��ܱ�����
						}
						//���ѱ��ַ
						if((ProtocolBps>>4)<2)//B4-B7:0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
						{
							MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							p8=(u8*)(ADDR_DATABUFF);
							for(i=0;i<6;i++)
							{
								if(p8[i]==0xaa)
								{
									break;
								}
							}
							if(i!=0)
							{
								i--;
							}
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										if((ProtocolBps>>4)==0)
										{//ԭDL645-2007����
											MWR(0x12aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
										}
										else
										{//ԭDL645-1997����
											MWR(0x23aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
										}
										return;
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
							MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
							break;
						}
					}
				default://2��ʱ
				Over://�ж����ֽڽ�����Ϊ��ʱ
					UARTCtrl->Task=0;//����1��
					MR(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					p8=(u8*)(ADDR_DATABUFF);
					for(i=0;i<6;i++)
					{
						if(p8[i]==0xaa)
						{
							break;
						}
					}
					if(i!=0)
					{
						i--;
					}
					if(p16fifo[0]==0)
					{//û�յ��κ��ֽ�
						while(1)
						{
							x=p8[i];
							if(x==0x99)
							{
								if(i==0)
								{//��ַ��0
									switch(ProtocolBps>>4)
									{
										case 0://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
											MWR(0x12aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 1://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
											MWR(0x23aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 2://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
											MWR(0x32aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											return;
										case 3://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
										default:
											MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
											if(MRR(ADDR_AFN05F149,1)==0x55)
											{//��F149����
												if(MRR(ADDR_AFN05F149+2+RS485PORT,1)==0x5a)//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
												{//����ִ��
													MWR(0xaa,ADDR_AFN05F149+2+RS485PORT,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
												}
											}
											MC(0,Get_ADDR_UARTn(PORTn),6);//��fifoָ��
											UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=�˵�����,B1=�Զ��ѱ�,B2-B7����
											return;
									}
								}
								else
								{
									p8[i]=0xaa;
									i--;
								}
							}
							else
							{
								break;
							}
						}
						x=bcd_hex(x);
						x++;
						x=hex_bcd(x);
						p8[i]=x;
					}
					else
					{//���յ��ֽ�
						if(i<5)
						{
							i++;
							p8[i]=0;
						}
						else
						{
							while(1)
							{
								x=p8[i];
								if(x==0x99)
								{
									if(i!=0)
									{
										p8[i]=0xaa;
										i--;
										continue;
									}
									else
									{
										switch(ProtocolBps>>4)
										{
											case 0://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
												MWR(0x12aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 1://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
												MWR(0x23aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 2://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
												MWR(0x32aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												return;
											case 3://ԭ0=DL645-2007����,1=DL645-1997����,2=DL645-2007׼ȷ,3=DL645-1997׼ȷ
											default:
												MWR(0x03aaaaaaaaaa00,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),7);
												if(MRR(ADDR_AFN05F149,1)==0x55)
												{//��F149����
													if(MRR(ADDR_AFN05F149+2+RS485PORT,1)==0x5a)//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
													{//����ִ��
														MWR(0xaa,ADDR_AFN05F149+2+RS485PORT,1);//���ܱ�ͨ�Ų����Զ�ά�����ܿ�����ر�
													}
												}
												MC(0,Get_ADDR_UARTn(PORTn),6);//��fifoָ��
												UARTCtrl->LockFlags&=0xfd;//ͨ�Ź��ܱ���ֹ��־:B0=�˵�����,B1=�Զ��ѱ�,B2-B7����
												return;
										}
									}
								}
								break;
							}
							x=bcd_hex(x);
							x++;
							x=hex_bcd(x);
							p8[i]=x;
						}
					}
					MW(ADDR_DATABUFF,ADDR_AutoSearchMeter_Start+(LEN_PerPortAutoSearchMeter*RS485PORT),6);
					return;
			}
//			break;
	}
}

