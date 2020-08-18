
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_6000.h"
#include "../DL698/RM_TASK.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"





//����2�����ñ�ֻ������=array �ɼ��������õ�Ԫ
//����2��Acquisition document definition����=structure
//{
//�������  long-unsigned��
//������Ϣ  Basic_object��
//��չ��Ϣ  Extended_object��
//������Ϣ  Annex_object
//}
//Basic_object��=structure
//{
//ͨ�ŵ�ַ  TSA��
//������    enum
//{
//300bps��0����  600bps��1����    1200bps��2����
//2400bps��3���� 4800bps��4����   7200bps��5����
//9600bps��6���� 19200bps��7����  38400bps��8����
//57600bps��9����115200bps��10��������Ӧ��255��
//}��
//��Լ����  enum
//{
//  δ֪ (0)��
//DL/T645-1997��1����
//DL/T645-2007��2����
//DL/T698.45��3����
//CJ/T 188-2004��4��
//}��
//�˿�      OAD��
//ͨ������  octet-string��
//���ʸ���  unsigned��
//�û�����  unsigned��
//���߷�ʽ  enum
//{
//   δ֪��0����
//   ���ࣨ1����
//   �������ߣ�2����
//   �������ߣ�3��
//}��
//���ѹ    long-unsigned(����-1����λV),
//�����    long-unsigned(����-1����λA)
//}
//Extended_object��=structure
//{
//�ɼ�����ַ  TSA��
//�ʲ���      octet-string��
//PT          long-unsigned��
//CT          long-unsigned
//}
//Annex_object��=array structure
//{
//������������  OAD��
//����ֵ        Data
//}



extern const u8 DL698_Attribute_6000_2[];
extern const u8 DL698_Attribute_6002_2[];
extern const u8 DL698_Attribute_6002_5[];
extern const u8 DL698_Attribute_6012_2[];
extern const u8 DL698_Attribute_6014[];



u32 GET_NUMper(DL698_SETLIB_TypeDef *pLib)//���㵱ǰԪ�ظ���
{
	u32 i;
	u32 x;
	u32 n;
	u32 NUMmax;
	u8 *p8;
	
	x=0;
	n=0;
	NUMmax=pLib->LENmax/pLib->LENper;
	while(1)
	{
		if(n>=NUMmax)
		{
			break;
		}
		MR(ADDR_128KDATABUFF,pLib->Addr+(n*pLib->LENper),LEN_128KDATABUFF);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/pLib->LENper);i++)
		{
			if(n>=NUMmax)
			{
				break;
			}
			if(p8[0]==DataType_structure)
			{//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
				x++;
			}
			n++;
			p8+=pLib->LENper;
		}
	}
	return x;
}


//�ɼ��������ñ��...
u32 GET_Request_6000(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 n;
	u32 Array;
	u32 Len;
	u32 NUMmax;
#if ((USER/100)==17)//����		
	u32 j,k,l,Nummax;
	u32 Phase;
	u8* p8s1;
#endif		
	u8 *p8;
	UARTCtrl_TypeDef * UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	NUMmax=pLib->LENmax/pLib->LENper;
	n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
	switch(OAD>>16)
	{
		case 0x6032://�ɼ�״̬

			break;
		case 0x6034://�ɼ�������
			for(i=0;i<RMTASKmax;i++)
			{
				RM_Monitor(i,0,0);//�ɼ�������;���:ID=�����,NUM_Tx=��������,NUM_Rx=��������
			}
			break;
	}
	p8tx[0]=DataType_array;
	Len=2;
	Array=0;//������������
	
	while(1)
	{
		if(n>=NUMmax)
		{
			break;
		}
		MR(ADDR_128KDATABUFF,pLib->Addr+(n*pLib->LENper),(LEN_128KDATABUFF/pLib->LENper)*pLib->LENper);
		p8=(u8*)ADDR_128KDATABUFF;
		for(i=0;i<(LEN_128KDATABUFF/pLib->LENper);i++)
		{
			if(n>=NUMmax)
			{
				break;
			}
			if(p8[0])
			{//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
				//x=Get_DL698DataLen(p8,(Attribute_TypeDef *)pLib->pDataType,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				x=Get_DL698DataLen(p8,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
				if(x&0xff)
				{
					x>>=8;
					if((Array>=127)||(x>LENmax_TxSPACE))
					{//���ͻ��岻��,��֡
						if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						{
							UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						}
						else
						{
							UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						}
						UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
						p8tx[1]=Array;//������������
						return Len;
					}
					MR((u32)p8tx+Len,(u32)p8,x);
					if((OAD>>16) == 0x6032)
					{
#if ((USER/100)==17) // ����
#if 0
						// ��������ʱ����λ��Ϣ���ܻ�û������ɣ���Ҫ�ȴ��ɼ����ڵ���
						//һ���ɼ�״̬��=structure 
								p8d[0]=DataType_structure;
								p8d[1]=8;
						//{ 
						//1.ͨ�ŵ�ַ              TSA�� 
								p8d[2]=DataType_TSA;
								p8d[3]=7;
								p8d[4]=5;
								MR((u32)p8d+5,ADDR_6000_SDRAM+(SetNo*LENper_6000)+10,6);
								offset=11;
						//2.�м̼���              unsigned�� 
								p8d[offset]=DataType_unsigned;
								p8d[offset+1]=Class;
								offset+=2;
						//3.�м̵�ַ              TSA�� 
								p8d[offset]=0;
								offset+=1;
						//4.�˿�                  OAD�� 
								p8d[offset]=DataType_OAD;
								i=PORTntoOAD(PORTn);//ͨ�ſں�תΪOADֵ
								p8d[offset+1]=i>>24;
								p8d[offset+2]=(i>>16)&0xff;
								p8d[offset+3]=(i>>8)&0xff;
								p8d[offset+4]=i;
								offset+=5;
						//5.���һ�βɼ��ɹ�ʱ��  date_time_s�� 
								p8d[offset]=DataType_date_time_s;
								Get_date_time_s(p8d+offset+1);//�õ�date_time_s��7�ֽ�
								offset+=8;
						//6.�ɼ�ʧ�ܴ���          unsigned�� 
								p8d[offset]=DataType_unsigned;
								p8d[offset+1]=0;
								offset+=2;
						//7.��λ enum{δ֪��0����A �ࣨ1����B �ࣨ2����C �ࣨ3��}��
								p8d[offset+1]=Phase;
								offset+=2;
						//8.�����쳣 enum{������0����LN ���ף�1����������2��} 
								p8d[offset]=DataType_enum;
								p8d[offset+1]=0;
#endif
						Data_Inverse((u32)p8tx+Len+5,6);// ��ַ����
						p8s1=(u8*)ADDR_AFN10F31;
						Nummax=p8s1[0];
						Nummax|=p8s1[1]<<8;
						for(j=0;j<NUM_RMmax;j++)
						{
							// ƥ����ַ
							if(Compare_DL698DataString(p8s1 + 2 + 8 * j ,p8tx+Len+5, 6) == 0)
							{
								break;
							}
						}
					
						if(j == NUM_RMmax)
						{// δ�ɹ��ҽڵ��ַ����Ҫ���ɼ������г���
							for(i=0;i<NUM_RMmax;i++)
							{
								if(Compare_DL698DataString(&NoteList->List[i].Addr[0], p8tx+Len+5, 6)==0)
								{
									break;
								}
							}
							if(i<NUM_RMmax)
							{// ���²ɼ�����ַ
								if(IsAllAByte(&NoteList->List[i].CollectorAddr[0], 0, 6) < 1)
								{// ��Ʊ�����ʶ�𵽶�Ӧ�Ĳɼ���
									for(k=0;k<NUM_RMmax;k++)
									{
										for(l=0;l<Nummax;l++)
										{// �ȴ�AFN10F31�ҵ����Ӧ�Ĳɼ���
											if(Compare_DL698DataString(&NoteList->List[k].Addr[0], p8s1 + 2 + 8 * l, 6)==0)
											{	// �ҵ�AFN10F31��Ӧ�ɼ���	
												if(Compare_DL698DataString(&NoteList->List[k].CollectorAddr[0], &NoteList->List[i].CollectorAddr[0], 6)==0)
												{// AFN10F31�ͳ�����ɼ�����ַƥ����
													j=l;
													break;
												}
											}
										}
										if(l<Nummax)
										{
											break;
										}
									}
								}
							}
						}

						// �޸�ʵ�ʱ����λ
						Phase = 0;
						if(j < NUM_RMmax)	// AFN10F31��λ����ַ����ƥ��ɹ�
						{
							if(p8s1[ 2+ 8*j + 6]&0x01)
							{
								Phase = 1;
							}
							if(p8s1[ 2+ 8*j + 6]&0x02)
							{
								Phase = 2;
							}
							if(p8s1[ 2+ 8*j + 6]&0x04)
							{
								Phase = 3;
							}
						}
						p8tx[Len+30]=Phase;
						Data_Inverse((u32)p8tx+Len+5,6);// ��ַ�����λ
#endif	
					}
					Len+=x;
					LENmax_TxSPACE-=x;
					Array++;//������������
				}
			}
			n++;
			p8+=pLib->LENper;
		}
	}
	if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	{
		UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
	}
	UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
	p8tx[1]=Array;//������������
	return Len;
}


//�ѱ���
u32 GET_Request_6002(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE)//��һ����������;����:����������ݳ���,���ݷ��ʽ��UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 n;
	u32 Array;
	u32 Len;
	UARTCtrl_TypeDef * UARTCtrl;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 FileID;//FileID=3ȫ���ѱ����ļ���=4��̨���ѱ����ļ�
	u32 TN;//�����ѱ�����¼��
	u32 offset;
	SearchMeterFileHead_TypeDef* SearchMeterFileHead;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	if((OAD==0x60020500)||(OAD==0x60020700))//��̨���ѱ������¼��
	{
		FileID=4;
	}
	else
	{
		FileID=3;
	}
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			//b7-b0�ļ�����
			if(file->FILEID!=FileID)
			{
				file++;
				continue;
			}
			//�ҵ�
			break;
		}
		file++;
	}
	if(filename>=recordFILENAMEmax_FLASH)
	{//û�ҵ�
		TN=0;//�����ѱ�����¼��
	}
	else
	{
		TN=file->NUM_DATA;//�����ѱ�����¼��
	}
	switch(OAD)
	{
		case 0x60020400://���Ԫ�ظ���
			if(LENmax_TxSPACE<3)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_long_unsigned;
			p8tx[1]=NUM_RMmax>>8;
			p8tx[2]=NUM_RMmax&0xff;
			return 3;
		case 0x60020300://��ǰԪ�ظ���
		case 0x60020600://�����ѱ�����¼��
		case 0x60020700://��̨���ѱ�����¼��
			if(LENmax_TxSPACE<3)
			{
				GET_Request_Frame(PORTn);//��֡��־
				return 0;
			}
			p8tx[0]=DataType_long_unsigned;
			p8tx[1]=TN>>8;
			p8tx[2]=TN;
			return 3;
		case 0x60020200://�����ѱ���
		case 0x60020500://��̨���ѱ���
			p8tx[0]=DataType_array;
			Len=2;
			Array=0;//������������
			n=UARTCtrl->NEXTOADSubNum;//DL698��֡����OAD���Ӽ���
			if(filename<recordFILENAMEmax_FLASH)
			{//�ҵ��ļ�
				ms=Get_ms();
				ms->msfile_read(filename,0,(u8*)ADDR_AddrList,TN*sizeof(SearchMeterFileHead_TypeDef));
				SearchMeterFileHead=(SearchMeterFileHead_TypeDef*)(ADDR_AddrList);
				offset=0;
				for(i=0;i<n;i++)
				{
					offset+=SearchMeterFileHead->LEN_DATA;
					SearchMeterFileHead++;
				}
				while(1)
				{
					if(n>=TN)
					{
						break;
					}
					x=SearchMeterFileHead->LEN_DATA;
					if((Array>=127)||(x>LENmax_TxSPACE))
					{//���ͻ��岻��,��֡
						if(UARTCtrl->NEXT==0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						{
							UARTCtrl->NEXT=1;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						}
						else
						{
							UARTCtrl->NEXT=2;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
						}
						UARTCtrl->NEXTOADSubNum=n;//DL698��֡����OAD���Ӽ���
						p8tx[1]=Array;//������������
						return Len;
					}
					ms->msfile_read(filename,(NUM_RMmax*sizeof(SearchMeterFileHead_TypeDef))+offset,p8tx+Len,x);
					offset+=x;
					Len+=x;
					LENmax_TxSPACE-=x;
					Array++;//������������
					SearchMeterFileHead++;
					n++;
				}
			}
			if(UARTCtrl->NEXT!=0)//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
			{
				UARTCtrl->NEXT=3;//DL698��֡�����־:0=����֡,1=��֡��1֡,2=��֡�м�֡,3=��֡���֡;B7λ=1��ʾ���ڷ�������APDU�ٷ�֡����
			}
			p8tx[1]=Array;//������������
			return Len;
		default:
			UARTCtrl->DAR=6;
			return 0;
	}
}














