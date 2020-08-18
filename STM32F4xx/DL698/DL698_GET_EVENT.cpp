
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698_GET_EVENT.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/EVENT_Record.h"
#include "../Device/MEMRW.h"
#include "../DL698/DL698_SETLIB_EVENT.h"
#include "../MS/msFILE.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Calculate/Calculate.h"





u32 GET_EVENT_OAD(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 n;
	u32 DataAddr;
	u32 Len;
	u32 Array;
	DL698_EVENTLIB_TypeDef *pLib;
	UARTCtrl_TypeDef *UARTCtrl;
	u8 *p8;
	u32 filename;
	msFILE_TypeDef *file;
	
	
	Len=0;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//���ݷ��ʽ��;�ɹ�
	pLib=Get_DL698_EVENT_pLib(OAD);//�õ�EVENT_pLibָ��;���:OAD;����:Libָ��(=0��ʾ����Ӧ��OI)
	if(pLib==0)
	{
		UARTCtrl->DAR=6;//���󲻴���           ��6��
		return 0;
	}
	if(OAD&0xff00)
	{//��������
		if((OAD&0xff00)==0x0100)
		{//���߼���
			if(LENmax_TxSPACE<4)
			{//��֡
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_octet_string;//long-unsigned
			p8tx[1]=2;
			p8tx[2]=OAD>>24;
			p8tx[3]=OAD>>16;
			return 4;
		}
		if((OAD&0xff)==0)
		{//ȫ��Ԫ��
			if(pLib->IC==7)
			{
				switch((OAD>>8)&0xff)
				{
					case 2://2���¼���¼��	��dyn.��
#if(USER/100)==17//����ϵ
						n=0;
						p8tx[0]=DataType_array;
						p8tx[1]=0;
						Len=2;
					 	LENmax_TxSPACE-=2;
						Array=0;
					 	i=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
					 	while(1)
						{
							if(i>=NUMmax_Event)
							{
							break;
							}
							if(LENmax_TxSPACE<400)
							{
								if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								{
									UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
								}
								UARTCtrl->NEXTOADSubNum=i;//DL698��֡����OAD���Ӽ���
								p8tx[1]=Array;//������������
								UARTCtrl->DAR=0;
								return Len;
							}
							OAD&=0xffffff00;
							OAD|=(i+1);
							n=GET_EventRecord(PORTn,OAD,p8tx+Len,LENmax_TxSPACE,0);//�õ��¼���¼;���:ALL=0ֻ�¼���OAD�б�,ALL=1��OAD�б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
							if(n!=0)
							{
								Len+=n;
								LENmax_TxSPACE-=n;
								Array++;//������������
								i++;
							}
							else
							{
								p8tx[1]=Array;
								UARTCtrl->DAR=0;
								return Len;
							}
						}
#else
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
#endif				
					case 3://3�������������Ա�	��static��
						DataAddr=pLib->Addr_EventOAD+4;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							Len>>=8;
						}
						break;
					case 4://4����ǰ��¼��	��dyn.��
						Len=0;
						OAD&=0xffff0000;
						OAD|=0x0202;
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
						for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
						{
							if(file->BLOCK_ENTRY)
							{
								i=file->FILEID;
								//�ļ�����
								if((i&0xff)!=2)
								{
									file++;
									continue;
								}
								//OI
								if(i!=OAD)
								{
									file++;
									continue;
								}
								Len+=file->NUM_DATA;
							}
							file++;
						}
						i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
						if(Len>i)
						{
							Len=i;
						}
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						p8[1]=Len>>8;
						p8[2]=Len;
						Len=3;
						break;
					case 5://5������¼��	��static��
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
						p8[1]=i>>8;
						p8[2]=i;
						Len=3;
						break;
					case 6://6�����ò���	��static��
						DataAddr=pLib->Addr;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return 0;
							}
							Len>>=8;
						}
						break;
					case 7://7����ǰֵ��¼��	��dyn.��
				//��ǰֵ��¼���=array ��ǰֵ
				//��ǰֵ��=structure
				//{
				//�¼�����Դ  instance-specific��
				//�ۼ�ʱ�估��������  structure
				//{
				//�¼��������� double-long-unsigned��
				//�¼��ۼ�ʱ�� double-long-unsigned
				//}
				//}
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
					case 8://8���ϱ���ʶ	��static��
						DataAddr=pLib->Addr_EventOAD+2;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 9://9����Ч��ʶ	��static��
						DataAddr=pLib->Addr_EventOAD;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 10://ʱ��״̬��¼��
					//ʱ��״̬��¼���=array ��ǰֵ 
					//��ǰֵ��=structure 
					//{ 
					//�¼�����Դ instance-specific�� 
					//�ۼ�ʱ�估�������� structure 
					//{
					//���һ�η���ʱ�� datetime_s��
					//���һ�ν���ʱ�� datetime_s 
					//} 
					//} 
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_array;
						p8[1]=1;
						p8[2]=DataType_structure;
						p8[3]=2;
						p8[4]=0;//�¼�����Դ
						p8[5]=DataType_structure;
						p8[6]=2;
						Len=7;
					//���һ�η���ʱ�� datetime_s��
						i=OAD&0xffff0000;
						i|=0x0200;//����2�¼���¼��
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:1=�ҵ��¼�ȡʱ�䵽pOut,0=û�ҵ�û���κ�����
					//���һ�ν���ʱ�� datetime_s 
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:1=�ҵ��¼�ȡʱ�䵽pOut,0=û�ҵ�û���κ�����
						break;
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
			}
			else
			{
				if(pLib->IC==24)
				{
					switch((OAD>>8)&0xff)
					{
						case 2://2�������������Ա�	��static��
							DataAddr=pLib->Addr_EventOAD+4;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
								Len>>=8;
							}
							break;
						case 3://3����ǰ��¼��	��dyn.��
							Len=0;
							OAD|=0x2;
							file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
							for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
							{
								if(file->BLOCK_ENTRY)
								{
									i=file->FILEID;
									//�ļ�����
									if((i&0xff)!=2)
									{
										file++;
										continue;
									}
									//OI
									if(i!=OAD)
									{
										file++;
										continue;
									}
									Len+=file->NUM_DATA;
								}
								file++;
							}
							i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
							if(Len>i)
							{
								Len=i;
							}
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							p8[1]=Len>>8;
							p8[2]=Len;
							Len=3;
							break;
						case 4://4������¼��	��static��
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
							p8[1]=i>>8;
							p8[2]=i;
							Len=3;
							break;
						case 5://5�����ò���	��static��
							DataAddr=pLib->Addr;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
									return 0;
								}
								Len>>=8;
							}
							break;
						case 6://6���¼���¼��1	��dyn.��
						case 7://7���¼���¼��2	��dyn.��
						case 8://8���¼���¼��3	��dyn.��
						case 9://9���¼���¼��4	��dyn.��
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							return 0;
						case 10://10����ǰֵ��¼��	��dyn.��
							DataAddr=pLib->Addr_Currentvalue;
							if(DataAddr==0)
							{
								UARTCtrl->DAR=6;//���󲻴���           ��6��
								return 0;
							}
							Len=Get_DL698DataLen_S((u8*)DataAddr,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
							break;
						case 11://11���ϱ���ʶ	��static��
							DataAddr=pLib->Addr_EventOAD+2;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 12://12����Ч��ʶ	��static��
							DataAddr=pLib->Addr_EventOAD;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 14://ʱ��״̬��¼��
						//ʱ��״̬��¼���=array structure 
						//{ 
						//���һ�η���ʱ�� datetime_s�� 
						//���һ�ν���ʱ�� datetime_s 
						//} 
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_array;
							p8[1]=4;
							Len=2;
							for(n=0;n<4;n++)
							{
								p8[Len+0]=DataType_structure;
								p8[Len+1]=2;
								Len+=2;
							//���һ�η���ʱ�� datetime_s��
								i=OAD&0xffff0000;
								i|=(n<<8)+0x0600;//6���¼���¼��1
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
							//���һ�ν���ʱ�� datetime_s 
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
							}
							break;
							
						default:
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							return 0;
					}
				}
			}
		}
		else
		{//����Ԫ��
			if(pLib->IC==7)
			{
				switch((OAD>>8)&0xff)
				{
					case 2://2���¼���¼��	��dyn.��
						Len=GET_EventRecord(PORTn,OAD,p8tx,LENmax_TxSPACE,0);//�õ��¼���¼;���:ALL=0ֻ�¼���OAD�б�,ALL=1��OAD�б�;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
						return Len;
					case 3://3�������������Ա�	��static��
						DataAddr=pLib->Addr_EventOAD+4;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							Len>>=8;
						}
						break;
					case 4://4����ǰ��¼��	��dyn.��
						Len=0;
						OAD&=0xffff0000;
						OAD|=0x0202;
						file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
						for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
						{
							if(file->BLOCK_ENTRY)
							{
								i=file->FILEID;
								//�ļ�����
								if((i&0xff)!=2)
								{
									file++;
									continue;
								}
								//OI
								if(i!=OAD)
								{
									file++;
									continue;
								}
								Len+=file->NUM_DATA;
							}
							file++;
						}
						i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
						if(Len>i)
						{
							Len=i;
						}
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						p8[1]=Len>>8;
						p8[2]=Len;
						Len=3;
						break;
					case 5://5������¼��	��static��
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_long_unsigned;
						i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
						p8[1]=i>>8;
						p8[2]=i;
						Len=3;
						break;
					case 6://6�����ò���	��static��
						DataAddr=pLib->Addr;
						if(DataAddr)
						{
							Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
							if((Len&0xff)==0)
							{
								UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
								return 0;
							}
							Len>>=8;
						}
						break;
					case 7://7����ǰֵ��¼��	��dyn.��
				//��ǰֵ��¼���=array ��ǰֵ
				//��ǰֵ��=structure
				//{
				//�¼�����Դ  instance-specific��
				//�ۼ�ʱ�估��������  structure
				//{
				//�¼��������� double-long-unsigned��
				//�¼��ۼ�ʱ�� double-long-unsigned
				//}
				//}
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
					case 8://8���ϱ���ʶ	��static��
						DataAddr=pLib->Addr_EventOAD+2;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 9://9����Ч��ʶ	��static��
						DataAddr=pLib->Addr_EventOAD;
						if(DataAddr)
						{
							Len=2;
						}
						break;
					case 10://ʱ��״̬��¼��
					//ʱ��״̬��¼���=array ��ǰֵ 
					//��ǰֵ��=structure 
					//{ 
					//�¼�����Դ instance-specific�� 
					//�ۼ�ʱ�估�������� structure 
					//{
					//���һ�η���ʱ�� datetime_s��
					//���һ�ν���ʱ�� datetime_s 
					//} 
					//} 
						DataAddr=ADDR_DATABUFF;
						p8=(u8*)ADDR_DATABUFF;
						p8[0]=DataType_array;
						p8[1]=1;
						p8[2]=DataType_structure;
						p8[3]=2;
						p8[4]=0;//�¼�����Դ
						p8[5]=DataType_structure;
						p8[6]=2;
						Len=7;
					//���һ�η���ʱ�� datetime_s��
						i=OAD&0xffff0000;
						i|=0x0200;//����2�¼���¼��
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:1=�ҵ��¼�ȡʱ�䵽pOut,0=û�ҵ�û���κ�����
					//���һ�ν���ʱ�� datetime_s 
						Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:1=�ҵ��¼�ȡʱ�䵽pOut,0=û�ҵ�û���κ�����
						break;
					default:
						UARTCtrl->DAR=6;//���󲻴���           ��6��
						return 0;
				}
			}
			else
			{
				if(pLib->IC==24)
				{
					switch((OAD>>8)&0xff)
					{
						case 2://2�������������Ա�	��static��
							DataAddr=pLib->Addr_EventOAD+4;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,0,0,LENmax_EventOAD);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
								Len>>=8;
							}
							break;
						case 3://3����ǰ��¼��	��dyn.��
							Len=0;
							OAD|=0x2;
							file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
							for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
							{
								if(file->BLOCK_ENTRY)
								{
									i=file->FILEID;
									//�ļ�����
									if((i&0xff)!=2)
									{
										file++;
										continue;
									}
									//OI
									if(i!=OAD)
									{
										file++;
										continue;
									}
									Len+=file->NUM_DATA;
								}
								file++;
							}
							i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
							if(Len>i)
							{
								Len=i;
							}
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							p8[1]=Len>>8;
							p8[2]=Len;
							Len=3;
							break;
						case 4://4������¼��	��static��
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_long_unsigned;
							i=DEPTH_EventFile(OAD>>16);//�¼��ļ��洢���;����:�洢���
							p8[1]=i>>8;
							p8[2]=i;
							Len=3;
							break;
						case 5://5�����ò���	��static��
							DataAddr=pLib->Addr;
							if(DataAddr)
							{
								Len=Get_DL698DataLen((u8*)DataAddr,(Attribute_TypeDef *)pLib->pDataType,0,pLib->LENmax);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128K_DATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
								if((Len&0xff)==0)
								{
									UARTCtrl->DAR=7;//���Ͳ�ƥ��           ��7��
									return 0;
								}
								Len>>=8;
							}
							break;
						case 6://6���¼���¼��1	��dyn.��
						case 7://7���¼���¼��2	��dyn.��
						case 8://8���¼���¼��3	��dyn.��
						case 9://9���¼���¼��4	��dyn.��
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							return 0;
						case 10://10����ǰֵ��¼��	��dyn.��
							DataAddr=pLib->Addr_Currentvalue;
							if(DataAddr==0)
							{
								UARTCtrl->DAR=6;//���󲻴���           ��6��
								return 0;
							}
							Len=Get_DL698DataLen_S((u8*)DataAddr,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
							break;
						case 11://11���ϱ���ʶ	��static��
							DataAddr=pLib->Addr_EventOAD+2;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 12://12����Ч��ʶ	��static��
							DataAddr=pLib->Addr_EventOAD;
							if(DataAddr)
							{
								Len=2;
							}
							break;
						case 14://ʱ��״̬��¼��
						//ʱ��״̬��¼���=array structure 
						//{ 
						//���һ�η���ʱ�� datetime_s�� 
						//���һ�ν���ʱ�� datetime_s 
						//} 
							DataAddr=ADDR_DATABUFF;
							p8=(u8*)ADDR_DATABUFF;
							p8[0]=DataType_array;
							p8[1]=4;
							Len=2;
							for(n=0;n<4;n++)
							{
								p8[Len+0]=DataType_structure;
								p8[Len+1]=2;
								Len+=2;
							//���һ�η���ʱ�� datetime_s��
								i=OAD&0xffff0000;
								i|=(n<<8)+0x0600;//6���¼���¼��1
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,1);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
							//���һ�ν���ʱ�� datetime_s 
								Len+=GET_LastEventTime_OccurRecovery(i,0,p8+Len,0);//�õ�������¼�����ʱ��;���:pTSA(������)����Ч����Ƚϵ�ַ��������;OccurRecovery=0�ָ�,=1����;����:8=�ҵ��¼�ȡʱ��8byte��pOut,1=û�ҵ���1byte��(0)��pOut
							}
							break;
							
						default:
							UARTCtrl->DAR=6;//���󲻴���           ��6��
							return 0;
					}
				}
			}
//�¼�����ʹ��Լ��
//ADDR_DATABUFF�¼��༭���
//ADDR_128KDATABUFF  32KBYTE ������DL698_DataDef
//ADDR_128KDATABUFF+32K 32KBYTE �¼�����,�����������Ա��
//ADDR_128KDATABUFF+64K 32KBYTE �����¼�����ʱ����
//ADDR_128KDATABUFF+96K 32KBYTE �¼��и������ݵ���ӦOAD˳���array OAD
			p8=Get_Element((u8 *)DataAddr,OAD&0xff,0,32*1024);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
			if(p8==0)
			{
				UARTCtrl->DAR=6;//���󲻴���
				return 0;
			}
			Len=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			DataAddr=(u32)p8;
		}
		if(Len>LENmax_TxSPACE)
		{//��֡
			GET_Request_Frame(PORTn);//��֡��־
			return 0;
		}
		MR((u32)p8tx,DataAddr,Len);
		return Len;
	}
	else
	{//0��ʾ�����������ԣ����������������
		UARTCtrl->DAR=6;//���󲻴���           ��6��
		return 0;
	}
}
	
	
	
	
	
	
	
	
	




