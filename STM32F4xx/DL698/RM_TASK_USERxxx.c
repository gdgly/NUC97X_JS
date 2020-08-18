
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../DL698/RM_RxDATA.h"
#include "../DL698/RM_Record.h"
#include "../DL698/RM_Event_DL2007.h"

#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Calculate/Calculate.h"
#include "../Display/Warning.h"
#include "../DL698/CONGEAL_Record.h"
#include "../DL698/EVENT_Record.h"

#include "../DL698/DL698_JSON.h"

extern const DL698_LIB_List_TypeDef SETINIT_601E_List[];

u32 RM_Meter(u32 PORTn)//��ǰ���ܱ�����Ƿ�Ҫ��;���:UARTCtrl->MeterCount[],UARTCtrl->TaskID;����:0=����,1=Ҫ��,2=���,ͬʱ��дͨ�ŵ�ַ,������,�����Լ����
{
	u32 i;
	u32 x;
	u32 id;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* pMS;
	u8 *p8s;
	u8* p8d;
	RMokfailLIST_TypeDef *rmOKFAILlist;
	UARTCtrl_TypeDef *UARTCtrl;

	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	id=UARTCtrl->TaskID;
	if(id==0)
	{
		return 0;
	}
	i=(id>>8)&0xff;//�����
	if(i>=(RMTASKmax+ExtRMTASKmax))
	{
		return 0;
	}
	switch(i)//�����
	{
		case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
			id>>=8;
			id&=0xff;
			i=UARTCtrl->MeterCount[id];
			i=PowerEvent->setno[i];
			p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
			p8s+=5;
			break;
	
		default:
			i=id&0xff;//�������
			x=id>>24;
			x&=0x0f;
			switch(x)//��������
			{
				case 1://��ͨ�ɼ�����
					i*=LENper_6014;
					i+=ADDR_6014_SDRAM;
					p8s=(u8*)i;
					if(p8s[0]!=DataType_structure)
					{
						return 0;
					}
					pMS=Get_Element(p8s,5,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(pMS==0)
					{
						return 0;
					}
					id>>=8;
					id&=0xff;
					pMS++;//ָ���������ͺ���ֽ�
					i=Compare_SETNO_MS((u8*)ADDR_6000_SDRAM,UARTCtrl->MeterCount[id],pMS);//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
					if(i==0)
					{
						return 0;
					}
					p8s=(u8*)ADDR_6000_SDRAM+(UARTCtrl->MeterCount[id]*LENper_6000);
					p8s+=5;
					break;
				case 2://�¼��ɼ�����
					i*=LENper_6016;
					i+=ADDR_6016_SDRAM;
					p8s=(u8*)i;
					if(p8s[0]!=DataType_structure)
					{
						return 0;
					}
					pMS=Get_Element(p8s,3,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(pMS==0)
					{
						return 0;
					}
					id>>=8;
					id&=0xff;
					pMS++;//ָ���������ͺ���ֽ�
					i=Compare_SETNO_MS((u8*)ADDR_6000_SDRAM,UARTCtrl->MeterCount[id],pMS);//�Ƚ���������Ƿ����MS����,���:pADDR_6000����RAM,pMS����RAM��ָ���������ͺ���ֽ�;����:0=������,1=����
					if(i==0)
					{
						return 0;
					}
					p8s=(u8*)ADDR_6000_SDRAM+(UARTCtrl->MeterCount[id]*LENper_6000);
					p8s+=5;
					break;
				case 3://͸������
		#if LENper_6018>LEN_128KDATABUFF
			#error
		#endif
					i*=LENper_6018;
					i+=ADDR_6018;
					MR(ADDR_128KDATABUFF,i,LENper_6018);
					p8s=(u8*)ADDR_128KDATABUFF;
					if(p8s[0]!=DataType_structure)
					{
						return 0;
					}
					id>>=8;
					id&=0xff;
					i=Get_DL698ByteTypeLenValue(p8s+5);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
					if(UARTCtrl->MeterCount[id]>=i)
					{
						UARTCtrl->MeterCount[id]=NUM_RMmax;
						return 0;
					}
					p8s=Get_Element(p8s+4,UARTCtrl->MeterCount[id]+1,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8s==0)
					{
						return 0;//����:0=����,1=Ҫ��,2=���
					}
					//͸��������ַ
					p8s+=1;
					//��ͨ�ŵ�ַ���������
					p8d=(u8*)ADDR_6000_SDRAM;
					for(i=0;i<NUMmax_6000;i++)
					{
						if(p8d[0]==DataType_structure)
						{
							//x=Compare_octet_string(p8s,p8d+8);//�Ƚ��ֽڴ�,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							x=Compare_TSA_string(p8s,p8d+8);//�Ƚ�TSA��,p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
							if(x==0)
							{
								break;
							}
							
						}
						p8d+=LENper_6000;
					}
					if(i>=NUMmax_6000)
					{
						return 0;
					}
					p8s=p8d+2;
		//			UARTCtrl->MeterSetNo=i;//�������
					break;
				case 4://�ϱ�����
					
				case 5://�ű�����
					break;
				default:
					break;
			}
			break;
	}
	//�˿�
	p8d=Get_Element(p8s,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
	if(p8d==0)
	{
		return 0;
	}
	x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//�˿�OAD
	x=OADtoPORTn(x);
	if(x!=PORTn)
	{
		return 0;
	}
	//ͨ�ŵ�ַ
	p8s+=3;
	i=p8s[1];
	i++;//ͨ�ŵ�ַ�ֽ���
	if(i>=6)
	{
		i-=6;//ֻȡ����ֽ�
		Addr0=p8s[i+2]|(p8s[i+3]<<8);
		Addr1=p8s[i+4]|(p8s[i+5]<<8);
		Addr2=p8s[i+6]|(p8s[i+7]<<8);
	}
	else
	{
		switch(i)//ͨ�ŵ�ַ�ֽ���
		{
			default:
				Addr0=0;
				Addr1=0;
				Addr2=0;
				break;
			case 1:
				Addr0=0;
				Addr1=0;
				Addr2=p8s[2];
				Addr2<<=8;
				break;
			case 2:
				Addr0=0;
				Addr1=0;
				Addr2=p8s[2]|(p8s[3]<<8);
				break;
			case 3:
				Addr0=0;
				Addr1=p8s[2];
				Addr1<<=8;
				Addr2=p8s[3]|(p8s[4]<<8);
				break;
			case 4:
				Addr0=0;
				Addr1=p8s[2]|(p8s[3]<<8);
				Addr2=p8s[4]|(p8s[5]<<8);
				break;
			case 5:
				Addr0=p8s[2];
				Addr0<<=8;
				Addr1=p8s[3]|(p8s[4]<<8);
				Addr2=p8s[5]|(p8s[6]<<8);
				break;
		}
	}
	if(UARTCtrl->RMMeterCountAuto==0)//������ܱ��Զ���1����,0=�Զ�,1=���Զ�(·��ģ������)
	{
		UARTCtrl->RMAddr[0]=Addr0;
		UARTCtrl->RMAddr[1]=Addr1;
		UARTCtrl->RMAddr[2]=Addr2;
	}
	x=UARTCtrl->NUMrmOK[id];//�����Ѳɼ������
	if(x>=NUM_RMmax)
	{
		x=NUM_RMmax;
	}
	rmOKFAILlist=UARTCtrl->rmOKFAILlist[id];
	while(x--)
	{
		if(Addr0==rmOKFAILlist->Addr0)
		{
			if(Addr1==rmOKFAILlist->Addr1)
			{
				if(Addr2==rmOKFAILlist->Addr2)
				{
					return 2;//����:0=����,1=Ҫ��,2=���
				}
			}
		}
		rmOKFAILlist++;
	}
	x=p8s[0];
	x++;
	//������
	p8s+=x;
	p8s++;
	i=p8s[0];
	switch(i)
	{
		case 0://300
		case 1://600
		case 2://1200
		case 3://2400
		case 4://4800
		case 5://7200
		case 6://9600
		case 7://19200
		case 8://38400
		case 9://57600
		case 10://115200
			if(PORTn!=RS485_4PORT)
			{//�����ز���
				UARTCtrl->BpsCtrl=(0xb+(i<<5));
			}
			break;
		case 255://����Ӧ
		default:
			if(PORTn!=RS485_4PORT)
			{//�����ز���
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485��Ĭ�ϵ����ʿ�����0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			break;
	}
	//��Լ����
	if(PORTn==ACSAMPLEPORT)
	{//�ڲ�����
	#if iMeterProtocol==2//�ڲ����ɳ����Լ:2=DL/T645-2007��3=DL/T698.45
		UARTCtrl->RMprotocol=2;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	#else
		UARTCtrl->RMprotocol=3;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	#endif
		return 1;//����:0=����,1=Ҫ��,2=���
	}
	p8s+=2;
	i=p8s[0];
	UARTCtrl->RMprotocol=i;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
	return 1;//����:0=����,1=Ҫ��,2=���
}

u8* RM_CSD(u32 PORTn)//ȡ�������ݱ�ʶCSD;����:0=��Ч,1=CSD��URATCtrl,����ֵ=͸������֡����ָ��
{
	u32 i;
	u32 n;
	u32 x;
	u32 id;
	u32 Len;
	u32 LEN_CSD;
	u32 OAD;
	u32 ROAD;
	u8* p8s;
	u8* p8d;
	u32* p32;
//	u16 *p16;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	id=UARTCtrl->TaskID;
	//myprintf("[%s:%d]:UARTCtrl->TaskID=%08x\n",(u32)&__func__,(u32)__LINE__,UARTCtrl->TaskID);
	if(id==0)
	{
		return 0;
	}
	i=(id>>8)&0xff;//�����
	switch(i)
	{
		case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
			UARTCtrl->RMCSDCountMax=1;//����CSD������
			if(UARTCtrl->RMCSDSubCount==0)//����CSD�Ӽ���
			{
				UARTCtrl->RMCSD[0]=DataType_CSD;
			#if (USER/100)==15
				UARTCtrl->RMCSD[1]=1;//ROAD
				UARTCtrl->RMCSD[2]=0x30;
				UARTCtrl->RMCSD[3]=0x11;
				UARTCtrl->RMCSD[4]=0x02;
				UARTCtrl->RMCSD[5]=0x00;
				UARTCtrl->RMCSD[6]=5;//RCSD��SEQUENCE OF����
				//UARTCtrl->RMCSD[7]=0;//OAD
				UARTCtrl->RMCSD[7]=0x20;
				UARTCtrl->RMCSD[8]=0x22;
				UARTCtrl->RMCSD[9]=0x02;
				UARTCtrl->RMCSD[10]=0x00;
				//UARTCtrl->RMCSD[12]=0;//OAD
				UARTCtrl->RMCSD[11]=0x20;
				UARTCtrl->RMCSD[12]=0x1E;
				UARTCtrl->RMCSD[13]=0x02;
				UARTCtrl->RMCSD[14]=0x00;
				//UARTCtrl->RMCSD[17]=0;//OAD
				UARTCtrl->RMCSD[15]=0x20;
				UARTCtrl->RMCSD[16]=0x20;
				UARTCtrl->RMCSD[17]=0x02;
				UARTCtrl->RMCSD[18]=0x00;
				//UARTCtrl->RMCSD[22]=0;//OAD
				UARTCtrl->RMCSD[19]=0x20;
				UARTCtrl->RMCSD[20]=0x24;
				UARTCtrl->RMCSD[21]=0x02;
				UARTCtrl->RMCSD[22]=0x00;
				//UARTCtrl->RMCSD[27]=0;//OAD
				UARTCtrl->RMCSD[23]=0x33;
				UARTCtrl->RMCSD[24]=0x00;
				UARTCtrl->RMCSD[25]=0x02;
				UARTCtrl->RMCSD[26]=0x00;
			#else
				UARTCtrl->RMCSD[1]=0;//OAD
				UARTCtrl->RMCSD[2]=0x30;
				UARTCtrl->RMCSD[3]=0x11;
				UARTCtrl->RMCSD[4]=0x02;
				UARTCtrl->RMCSD[5]=0x01;
				UARTCtrl->RMCSD[6]=0;//RCSD��SEQUENCE OF����
			#endif
				
//				UARTCtrl->RMCSD[0]=DataType_CSD;
//				UARTCtrl->RMCSD[1]=1;//ROAD
//				UARTCtrl->RMCSD[2]=0x30;
//				UARTCtrl->RMCSD[3]=0x11;
//				UARTCtrl->RMCSD[4]=0x02;
//				UARTCtrl->RMCSD[5]=0x00
//				UARTCtrl->RMCSD[6]=0;//RCSD��SEQUENCE OF����
				switch(UARTCtrl->RMprotocol)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
				{
					case 1://DL/T645-1997
						return 0;
					case 2://DL/T645-2007
						UARTCtrl->RM_DI=0x03110001;//(��1��)���緢��ʱ�̣�����ʱ��
						break;
					case 3://DL/T698.45��3��
						break;
					default://ABB,Landis
						return 0;
				}
				return (u8*)1;
			}
			return 0;
	}
	
	i=id&0xff;//�������
	switch((id>>24)&0xf)//��������
	{
		case 1://��ͨ�ɼ�����
			i*=LENper_6014;
			i+=ADDR_6014_SDRAM;
		#if (USER/100)==17//����ϵ
			if(Terminal_Router->MinuteCollect==1&&PORTn!=ACSAMPLEPORT)
			{
				p8s=Get_Element((u8*)i,3,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8s)
				{
					if((p8s[0]!=0)&&(p8s[1]==0xf0))//���ɼ���������ʽ�ɼ�
					{
						UARTCtrl->RMCSDCountMax=0;
						return (u8*)1;
					}
					else
						return 0;
				}
			}
			else
			{
				p8s=Get_Element((u8*)i,3,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
				if(p8s)
				{
					if((p8s[0]!=0)&&(p8s[1]==0xf0))//���ɼ���������ʽ�ɼ�
						return 0;
				}
			}
//				p8s=Get_Element((u8*)i,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
//				if(p8s==0)
//				{
//					return 0;//0=��Ч,1=CSD��URATCtrl,����ֵ=͸������֡����ָ��
//				}
//				else
//				{
//					if(p8s[3]==1)//��ʷ���ݣ�ֻҪ�õ㳭����
//					{
//						p16=(u16*)(p8s+4);
//						if(p16[0]!=0x0550)
//						 return 0;
//					}
//				}
//			}
//			else
//			{
//				p8s=Get_Element((u8*)i,3,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
//				if(p8s)
//				{
//					if((p8s[0]!=0)&&(p8s[1]==0xf0))//���ɼ���������ʽ�ɼ�
//						return 0;
//				}
//			}
		#endif
			p8s=Get_Element((u8*)i,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;//0=��Ч,1=CSD��URATCtrl,����ֵ=͸������֡����ָ��
			}
			i=UARTCtrl->RMCSDCount;//����CSD����
			x=Get_DL698ByteTypeLenValue(p8s+1);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
			UARTCtrl->RMCSDCountMax=x;//����CSD������
			if(i>=x)
			{//����CSD����>=������
				return 0;//0=��Ч,1=CSD��URATCtrl,����ֵ=͸������֡����ָ��
			}
			p8s=Get_Element(p8s,i+1,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			LEN_CSD=Get_DL698DataLen(p8s,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
			LEN_CSD>>=8;
			if((LEN_CSD>sizeof(UARTCtrl->RMCSD))||(LEN_CSD<6))
			{
				return 0;
			}
			if(p8s[1]==0)//������������OAD����ROAD
			{//OAD
				UARTCtrl->RMOADROAD|=1;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND
			}
			else
			{//ROAD
				UARTCtrl->RMOADROAD|=2;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND
			}
			x=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
			MR((u32)&UARTCtrl->RMCSD,(u32)p8s,LEN_CSD);
			n=(p8s[2]<<8)|p8s[3];
			switch(n)
			{
				case 0x5002://���Ӷ���,����������,��Ჹ����96�����������Чʱ��û��ͬ�ļ���ʹ�ѳ���Ϊ0,��RM_TxDATA.c�л�ÿһ�����Ƿ��ѳ�
				case 0x5004://�ն���
				case 0x5006://�¶���
//					if(x!=n)
//					{//OI�仯
//						NUMrm(PORTn);//������;����:�����賭������������Ƿ������,����¼�ļ�������ɵĵ�ַ��
//						//��װ,NUMrm(PORTn)���޸�Ϊ0xff
//						MR((u32)&UARTCtrl->RMCSD,(u32)p8s,LEN_CSD);
//					}
					//��������ͨ�ſ��ȶ���󳭱�
					if(PORTn==ACSAMPLEPORT)//��������ͨ�ſں�(����)
					{
						CONGEAL_Record();//����
					}
					break;
			}
			
			x=UARTCtrl->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
			switch(x)
			{
				default://δ֪
					return 0;
				case 1://DL/T645-1997
					//���߳���
					if(UARTCtrl->RMCSD[1]==1)
					{//ROAD
						OAD=UARTCtrl->RMCSD[2+0]<<24;
						OAD|=UARTCtrl->RMCSD[2+1]<<16;
						OAD|=UARTCtrl->RMCSD[2+2]<<8;
						OAD|=UARTCtrl->RMCSD[2+3];
						if(OAD==0x50020200)
						{//���Ӷ���
							i=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
							x=UARTCtrl->RMCSD[6];
							UARTCtrl->RMCSDSubCountMax=1;//����CSD���ӳ������(��DL698�����Լ���趨)
							UARTCtrl->RMROADCountMax=x;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
							if(i>=x)
							{//����CSD�Ӽ���>=������
								return 0;//����
							}
							OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
							switch(OAD)
							{
								case 0x20210200://1 ����ʱ��
									OAD=0xC011;//DI���ܱ�����ʱ��Hhmmss	
									break;
								case 0x00100201://2 �����й��ܵ���
									OAD=0x9010;//DL645-1997��ʶ �������ݸ�ʽ��XXXXXX.XX
									break;
								case 0x00200201://3 �����й��ܵ���  
									OAD=0x9020;//DL645-1997��ʶ �������ݸ�ʽ��XXXXXX.XX
									break;
								case 0x00300200://4 ����޹�1����
									OAD=0x911F;//DL645-1997��ʶ �������ݸ�ʽ��XXXXXX.XX
									break;
								case 0x00400200://5 ����޹�2����
									OAD=0x912F;//DL645-1997��ʶ �������ݸ�ʽ��XXXXXX.XX
									break;
								case 0x00500201://6 һ�����޹��ܵ���
									OAD=0x9130;//DL645-1997��ʶ �������ݸ�ʽ��XXXXXX.XX
									break;
								case 0x00600201://7 �������޹��ܵ��� �������ݸ�ʽ��XXXXXX.XX
									OAD=0x9150;//DL645-1997��ʶ
									break;
								case 0x00700201://8 �������޹��ܵ��� �������ݸ�ʽ��XXXXXX.XX
									OAD=0x9160;//DL645-1997��ʶ
									break;
								case 0x00800201://9 �������޹��ܵ��� �������ݸ�ʽ��XXXXXX.XX
									OAD=0x9140;//DL645-1997��ʶ
									break;
								case 0x20170200://10 �й�����
									return 0;
								case 0x20180200://11 �޹�����
									return 0;
								case 0x20000200://12 ��ѹ
									UARTCtrl->RMCSDSubCountMax=3;//����CSD���ӳ������(��DL698�����Լ���趨)
									switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
									{
										case 0://A���ѹ
											OAD=0xB611;//DL645-1997��ʶ �������ݸ�ʽ��XXX
											break;
										case 1://B���ѹ
											OAD=0xB612;//DL645-1997��ʶ �������ݸ�ʽ��XXX
											break;
										case 2://C���ѹ
											OAD=0xB613;//DL645-1997��ʶ �������ݸ�ʽ��XXX
											break;										
									}
									break;
								case 0x20000201://12 A���ѹ
									OAD=0xB611;//DL645-1997��ʶ �������ݸ�ʽ��XXX
									break;
								case 0x20010200://13 ����
									UARTCtrl->RMCSDSubCountMax=3;//����CSD���ӳ������(��DL698�����Լ���趨)
									switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
									{
										case 0://A�����
											OAD=0xB621;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
											break;
										case 1://B�����
											OAD=0xB622;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
											break;
										case 2://C�����
											OAD=0xB623;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
											break;										
									}
									break;
								case 0x20010201://13 A�����
									OAD=0xB621;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
									break;
								case 0x20010400://14 ���ߵ���
									return 0;
								case 0x20040200://15 �й�����
									UARTCtrl->RMCSDSubCountMax=4;//����CSD���ӳ������(��DL698�����Լ���趨)
									switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
									{
										case 0://���й�����
											OAD=0xB630;//DL645-1997��ʶ �������ݸ�ʽ��XX.XXXX
											break;
										case 1://A���й�����
											OAD=0xB631;//DL645-1997��ʶ �������ݸ�ʽ��XX.XXXX
											break;
										case 2://B���й�����
											OAD=0xB632;//DL645-1997��ʶ �������ݸ�ʽ��XX.XXXX
											break;
										case 3://C���й�����
											OAD=0xB633;//DL645-1997��ʶ �������ݸ�ʽ��XX.XXXX
											break;										
									}
									break;									
								case 0x20040202://15 A���й�����
									OAD=0xB631;//DL645-1997��ʶ �������ݸ�ʽ��XX.XXXX
									break;
								case 0x20050200://16 �޹�����
									UARTCtrl->RMCSDSubCountMax=4;//����CSD���ӳ������(��DL698�����Լ���趨)
									switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
									{
										case 0://���޹�����
											OAD=0xB640;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
											break;
										case 1://A���޹�����
											OAD=0xB641;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
											break;
										case 2://B���޹�����
											OAD=0xB642;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
											break;
										case 3://C���޹�����
											OAD=0xB643;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
											break;										
									}
									break;									
								case 0x20050202://16 A���޹�����
									OAD=0xB641;//DL645-1997��ʶ �������ݸ�ʽ��XX.XX
									break;
								case 0x200A0200://17 ��������
									UARTCtrl->RMCSDSubCountMax=4;//����CSD���ӳ������(��DL698�����Լ���趨)
									switch(UARTCtrl->RMCSDSubCount)//����CSD�Ӽ���(��DL698����ʱ)
									{
										case 0://�ܹ�������
											OAD=0xB650;//DL645-1997��ʶ �������ݸ�ʽ��X.XXX
											break;
										case 1://A�๦������
											OAD=0xB651;//DL645-1997��ʶ �������ݸ�ʽ��X.XXX
											break;
										case 2://B�๦������
											OAD=0xB652;//DL645-1997��ʶ �������ݸ�ʽ��X.XXX
											break;
										case 3://C�๦������
											OAD=0xB653;//DL645-1997��ʶ �������ݸ�ʽ��X.XXX
											break;										
									}
									break;																		
								case 0x200A0202://17 A�๦������
									OAD=0xB651;//DL645-1997��ʶ �������ݸ�ʽ��X.XXX
									break;
								case 0x11100201://18 ���������������й�������� 
									return 0;
								default:
									return 0;
							}
							i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
							if(i>=1)
							{//DL645-1997�����߳�ʵʱ���ݣ�ֻ����ǰ1����
								return 0;//����
							}
							i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
							if(i>=UARTCtrl->RMCSDSubCountMax)
							{
								return 0;
							}
							UARTCtrl->RM_DI=OAD;
							return (u8*)1;
						}
					}
				
					//����ȫƥ��
		#if PARAMETER_601E==0//���ò���601E�ɼ������ʹ��:0=�ڲ�Ĭ��,1=������
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//�õ��ɼ�������ʼ���ÿ��б����
					for(i=0;i<n;i++)
					{
						p8d=(u8*)p32[0];
						if(p8d[0]==DataType_structure)
						{
							for(x=0;x<LEN_CSD;x++)
							{
								if(UARTCtrl->RMCSD[x]!=p8d[2+x])
								{
									break;
								}
							}
							if(x>=LEN_CSD)
							{
								break;
							}
						}
						p32+=2;
					}
					if(i>=n)
					{
						i=NUMmax_601E;
					}
		#else
			#if LENmax_601E>LEN_128KDATABUFF
					#error
			#endif
					MR(ADDR_128KDATABUFF,ADDR_601E,LENmax_601E);
					p8d=(u8*)ADDR_128KDATABUFF;
					for(i=0;i<NUMmax_601E;i++)
					{
						if(p8d[0]==DataType_structure)
						{
							for(x=0;x<LEN_CSD;x++)
							{
								if(UARTCtrl->RMCSD[x]!=p8d[2+x])
								{
									break;
								}
							}
							if(x>=LEN_CSD)
							{
								break;
							}
						}
						p8d+=LENper_601E;
					}
		#endif
					if(i<NUMmax_601E)
					{//�ɼ���������ҵ�ȫƥ��
						Len=Get_DL698DataLen(p8d+2,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
						Len>>=8;
						p8d+=2+Len+4;
						//2007 ����DI
						x=p8d[1];//������
						x*=6;
						x+=2;
						p8d+=x;
						//2007 ����DI
						x=p8d[1];//������
						x*=6;
						x+=2;
						p8d+=x;
						p8d+=2;
						if(UARTCtrl->RM_DI_CHOICE!=0)//����DIѡ��:0=��DI,1=����DI
						{//����DI
							x=p8d[1];//������
							x*=4;
							x+=2;
							p8d+=x;
						}
						i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
						x=p8d[1];
						UARTCtrl->RMCSDSubCountMax=x;//����CSD���ӳ������(��DL698�����Լ���趨)
						UARTCtrl->RMROADCountMax=0;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
						if(i>=x)
						{//����CSD�Ӽ���>=������
							return 0;
						}
						p8d+=4+(i*4);
						i=(p8d[0]<<8)|p8d[1];
						UARTCtrl->RM_DI=i;
						return (u8*)1;
					}
				//���ҵ���OADƥ��
					if(UARTCtrl->RMCSD[1]==0)//��ǰ���ڳ���CSD(����������)
					{//����ROAD��
						return 0;
					}
					i=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
					x=UARTCtrl->RMCSD[6];//ROAD����
					UARTCtrl->RMROADCountMax=x;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
					if(i>=x)
					{//����ROAD����>=������
						return 0;//����
					}
					OAD=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|UARTCtrl->RMCSD[5];
					i*=4;
					i+=7;
					ROAD=(UARTCtrl->RMCSD[i]<<24)|(UARTCtrl->RMCSD[i+1]<<16)|(UARTCtrl->RMCSD[i+2]<<8)|UARTCtrl->RMCSD[i+3];
		#if PARAMETER_601E==0//���ò���601E�ɼ������ʹ��:0=�ڲ�Ĭ��,1=������
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//�õ��ɼ�������ʼ���ÿ��б����
					for(i=0;i<n;i++)
					{
						p8d=(u8*)p32[0];
						if(p8d[0]==DataType_structure)
						{
							if(p8d[3]==1)
							{//��ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//�ǵ���ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//�ҵ�
											break;
										}
									}
								}
							}
						}
						p32+=2;
					}
					if(i>=n)
					{
						i=NUMmax_601E;
					}
		#else
			#if LENmax_601E>LEN_128KDATABUFF
					#error
			#endif
					MR(ADDR_128KDATABUFF,ADDR_601E,LENmax_601E);
					p8d=(u8*)ADDR_128KDATABUFF;
					for(i=0;i<NUMmax_601E;i++)
					{
						if(p8d[0]==DataType_structure)
						{
							if(p8d[3]==1)
							{//��ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//�ǵ���ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//�ҵ�
											break;
										}
									}
								}
							}
						}
						p8d+=LENper_601E;
					}
		#endif
					if(i>=NUMmax_601E)
					{
						return 0;
					}
					Len=Get_DL698DataLen(p8d+2,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
					Len>>=8;
					p8d+=2+Len+4;
					//2007 ����DI
					x=p8d[1];//������
					x*=6;
					x+=2;
					p8d+=x;
					//2007 ����DI
					x=p8d[1];//������
					x*=6;
					x+=2;
					p8d+=x;
					p8d+=2;
					if(UARTCtrl->RM_DI_CHOICE!=0)//����DIѡ��:0=��DI,1=����DI
					{//����DI
						x=p8d[1];//������
						x*=4;
						x+=2;
						p8d+=x;
					}
					i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
					x=p8d[1];
					UARTCtrl->RMCSDSubCountMax=x;//����CSD���ӳ������(��DL698�����Լ���趨)
					if(i>=x)
					{//����CSD�Ӽ���>=������
						return 0;
					}
					p8d+=4+(i*4);
					i=(p8d[0]<<8)|p8d[1];
					UARTCtrl->RM_DI=i;
					return (u8*)1;
				case 2://DL/T645-2007
				case ProtocolNO_Landis://Landis(IEC1107 ģʽC)
//����2��=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	������ѡ��������  CSD��
//	��������  structrue
//	{
//		AcqCmd_2007  structure
//		{
//			����DI  array octet-string(SIZE(4))��
//			����DI  array octet-string(SIZE(4))
//		}��
//		AcqCmd_1997  structure
//		{
//			����DI  array octet-string(SIZE(2))��
//			����DI  array octet-string(SIZE(2))
//		}��
//		AcqCmd_Trans  structure
//		{
//			Frame  octet-string
//		}
//	}
//}
				//���߳���
					if(UARTCtrl->RMCSD[1]==1)
					{//ROAD
						OAD=UARTCtrl->RMCSD[2+0]<<24;
						OAD|=UARTCtrl->RMCSD[2+1]<<16;
						OAD|=UARTCtrl->RMCSD[2+2]<<8;
						OAD|=UARTCtrl->RMCSD[2+3];
					#if(USER/100)==17//����ϵ
						if((OAD==0x50020200)||(OAD==0x50050200))
					#else
						if(OAD==0x50020200)
					#endif
						{//���Ӷ���
							i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
							if(i>=1)
							{
								return 0;
							}
							id=UARTCtrl->TaskID;
							id=(id>>8)&0xff;//�����
							i=UARTCtrl->MeterCount[id];//ȡ������
							p8d=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							p8d=Get_Element(p8d,2,8,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							if(p8d==0)
							{
								return 0;
							}

							i=p8d[1];//ȡ���߷�ʽ
#if (((USER/100)==9)||((USER/100)==17)) //����Ǻ����г����ֿ��˱�û�з��Ӷ������ݣ��ó�ʵʱ����ת
              if(UARTCtrl->RMprotocol==2)
              {//�����645-07Э��
                x=UARTCtrl->MeterCount[id];//ȡ������
                p8d=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
                p8d=Get_Element(p8d,2,4,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
                x=(p8d[1]<<24)|(p8d[2]<<16)|(p8d[3]<<8)|(p8d[4]);
                if(x==0xF2010201)
                {//RS485ͨ���ó�ʵʱ�����������������
                  i=1;
                }
              }
#endif
							if(UARTCtrl->RMprotocol==ProtocolNO_Landis)//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
							{//Landis��
								i=1;//Landis���ɵ������ʵʱ���ݴ�
							}
							if(i==1)//δ֪��0�������ࣨ1�����������ߣ�2�����������ߣ�3��
							{//����
								i=UARTCtrl->RM_DI_TI_Count;//��ʱ�����ɼ�ʱ�ļ������
								if(i>=1)
								{//��������߳�ʵʱ���ݣ�ֻ����ǰ1����
									return 0;//����
								}
								i=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
								x=UARTCtrl->RMCSD[6];
								UARTCtrl->RMCSDSubCountMax=1;//����CSD���ӳ������(��DL698�����Լ���趨)
								UARTCtrl->RMROADCountMax=x;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
								if(i>=x)
								{//����CSD�Ӽ���>=������
									return 0;//����
								}
								OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
								switch(OAD)
								{
									case 0x20210200://1 ����ʱ��
										OAD=0x04000102;//DI���ܱ�����ʱ��Hhmmss
										break;
									case 0x00100201://2 �����й��ܵ���
										OAD=0x00010000;//DL645-2007��ʶ �������ݸ�ʽ��XXXXXX.XX
										break;
									case 0x00100200://2 ??????
										OAD=0x0001ff00;//DL645-2007?? ??????:XXXXXX.XX
										break;
									case 0x00200201://3 ???????  
										OAD=0x00020000;//DL645-2007?? ??????:XXXXXX.XX
										break;
									case 0x00200200://3 ??????
										OAD=0x0002ff00;//DL645-2007?? ??????:XXXXXX.XX
										break;
									case 0x00300200://4 ����޹�1����
										OAD=0x0003ff00;//DL645-2007��ʶ �������ݸ�ʽ��XXXXXX.XX(�ܼ��ַ���)
										break;
									case 0x00400200://5 ����޹�2����
										OAD=0x0004ff00;//DL645-2007��ʶ �������ݸ�ʽ��XXXXXX.XX(�ܼ��ַ���)
										break;
									case 0x00500201://6 һ�����޹��ܵ���
										OAD=0x00050000;//DL645-2007��ʶ �������ݸ�ʽ��XXXXXX.XX
										break;
									case 0x00500200://6 ???????
										OAD=0x0005ff00;//DL645-2007?? ??????:XXXXXX.XX
										break;
									case 0x00600201://7 ???????? ??????:XXXXXX.XX
										OAD=0x00060000;//DL645-2007??
										break;
									case 0x00600200://7 ??????? ??????:XXXXXX.XX
										OAD=0x0006ff00;//DL645-2007??
										break;
									case 0x00700201://8 ???????? ??????:XXXXXX.XX
										OAD=0x00070000;//DL645-2007??
										break;
									case 0x00700200://8 ??????? ??????:XXXXXX.XX
										OAD=0x0007ff00;//DL645-2007??
										break;
									case 0x00800201://9 ???????? ??????:XXXXXX.XX
										OAD=0x00080000;//DL645-2007??
										break;
									case 0x00800200://9 ??????? ??????:XXXXXX.XX
										OAD=0x0008ff00;//DL645-2007??
										break;
									case 0x20170200://10 �й�����
						        OAD=0x02800004;//DL645-2007��ʶ �������ݸ�ʽ��XX.XXXX
										break;
									case 0x20180200://11 �޹�����
										OAD=0x02800005;//DL645-2007��ʶ �������ݸ�ʽ��XX.XXXX
										break;
									case 0x20000200://12 ��ѹ
										OAD=0x0201ff00;//DL645-2007��ʶ �������ݸ�ʽ��A��XXX.X B��XXX.X C��XXX.X
										break;										
									case 0x20000201://12 A���ѹ
										OAD=0x02010100;//DL645-2007��ʶ �������ݸ�ʽ��A��XXX.X 
										break;
									case 0x20000202://12 B???
										OAD=0x02010200;//DL645-2007?? ??????:A?XXX.X 
										break;
									case 0x20000203://12 C???
										OAD=0x02010300;//DL645-2007?? ??????:A?XXX.X 
										break;
									case 0x20010200://13 ??
										OAD=0x0202ff00;//DL645-2007?? ??????:A?XXX.XXX B?XXX.XXX C?XXX.XXX
										break;										
									case 0x20010201://13 A???
										OAD=0x02020100;//DL645-2007?? ??????:A?XXX.XXX 
										break;
									case 0x20010202://13 B???
										OAD=0x02020200;//DL645-2007?? ??????:A?XXX.XXX 
										break;
									case 0x20010203://13 B???
										OAD=0x02020300;//DL645-2007?? ??????:A?XXX.XXX 
										break;
									case 0x20010400://14 ���ߵ���
										OAD=0x02800001;//DL645-2007��ʶ �������ݸ�ʽ��XXX.XXX
										break;
									case 0x20040200://15 �й�����
									case 0x20040202://15 A���й�����
										OAD=0x0203ff00;//DL645-2007��ʶ �������ݸ�ʽ����XX.XXXX A��XX.XXXX B��XX.XXXX C��XX.XXXX
										break;
									case 0x20050200://16 �޹�����
									case 0x20050202://16 A���޹�����
										OAD=0x0204ff00;//DL645-2007��ʶ �������ݸ�ʽ����XX.XXXX A��XX.XXXX B��XX.XXXX C��XX.XXXX
										break;
									case 0x200A0200://17 ��������
									case 0x200A0202://17 A�๦������
										OAD=0x0206ff00;//DL645-2007��ʶ �������ݸ�ʽ����X.XXX A��X.XXX B��X.XXX C��X.XXX
										break;
									case 0x11100201://18 ���������������й�������� 
										return 0;
									case 0x10100200://??????
										OAD=0x0101ff00;
										break;
									case 0x10300200://??????
										OAD=0x0103ff00;
										break;
									case 0x10200200:
										OAD=0x0102ff00;
										break;
									case 0x10400200:
										OAD=0x0104ff00;
										break;
									default:
										return 0;
								}
								UARTCtrl->RM_DI=OAD;
								return (u8*)1;
							}
							else
							{//����
								i=0;
								OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
								switch(OAD)
								{
									case 0x20000200://12 ��ѹ
									case 0x20000201://12 A���ѹ
									case 0x20010200://13 ����
									case 0x20010201://13 A�����
									case 0x20010400://14 ���ߵ���
										OAD=0x06010001;//DL645��ʶ 1�ฺ������(��ѹ��������Ƶ��)
										break;
									case 0x20040200://15 �й�����
									case 0x20040202://15 A���й�����
									case 0x20050200://16 �޹�����
									case 0x20050202://16 A���޹�����
										OAD=0x06020001;//DL645��ʶ 2�ฺ������(���޹�����)
										break;
									case 0x200A0200://17 ��������
									case 0x200A0202://17 A�๦������
										OAD=0x06030001;//DL645��ʶ 3�ฺ������(��������)
										break;
									case 0x20210200://1 ����ʱ��
									case 0x00100201://2 �����й��ܵ���
									case 0x00200201://3 �����й��ܵ���  
									case 0x00300200://4 ����޹�1����
									case 0x00400200://5 ����޹�2����
										OAD=0x06040001;//DL645��ʶ 4�ฺ������(�����й�,�M���޹�1��2�ܵ���)
										break;
									case 0x00500201://6 һ�����޹��ܵ���
									case 0x00600201://7 �������޹��ܵ��� �������ݸ�ʽ��XXXXXX.XX
									case 0x00700201://8 �������޹��ܵ��� �������ݸ�ʽ��XXXXXX.XX
									case 0x00800201://9 �������޹��ܵ��� �������ݸ�ʽ��XXXXXX.XX
										OAD=0x06050001;//DL645��ʶ 5�ฺ������(1-4�����޹��ܵ���)
										break;
									case 0x20170200://10 �й�����
									case 0x20180200://11 �޹�����
										OAD=0x06060001;//DL645��ʶ 6�ฺ������(��ǰ�й�����,��ǰ�޹�����)
										break;
									case 0x11100201://18 ���������������й�������� 
										return 0;
									default:
										return 0;
								}
								UARTCtrl->RM_DI=OAD;//DL645��ʶ ��������
								UARTCtrl->RMCSDSubCountMax=1;//����CSD���ӳ������(��DL698�����Լ���趨)
								UARTCtrl->RMROADCountMax=0;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
								return (u8*)1;
							}
						}
					}

				//����ȫƥ��
		#if PARAMETER_601E==0//���ò���601E�ɼ������ʹ��:0=�ڲ�Ĭ��,1=������
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//�õ��ɼ�������ʼ���ÿ��б����
					for(i=0;i<n;i++)
					{
						p8d=(u8*)p32[0];
						if(p8d[0]==DataType_structure)
						{
							for(x=0;x<LEN_CSD;x++)
							{
								if(UARTCtrl->RMCSD[x]!=p8d[2+x])
								{
									break;
								}
							}
							if(x>=LEN_CSD)
							{
								break;
							}
						}
						p32+=2;
					}
					if(i>=n)
					{
						i=NUMmax_601E;
					}
		#else
			#if LENmax_601E>LEN_128KDATABUFF
					#error
			#endif
					MR(ADDR_128KDATABUFF,ADDR_601E,LENmax_601E);
					p8d=(u8*)ADDR_128KDATABUFF;
					for(i=0;i<NUMmax_601E;i++)
					{
						if(p8d[0]==DataType_structure)
						{
							for(x=0;x<LEN_CSD;x++)
							{
								if(UARTCtrl->RMCSD[x]!=p8d[2+x])
								{
									break;
								}
							}
							if(x>=LEN_CSD)
							{
								break;
							}
						}
						p8d+=LENper_601E;
					}
		#endif
					if(i<NUMmax_601E)
					{//�ɼ���������ҵ�ȫƥ��
						Len=Get_DL698DataLen(p8d+2,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
						Len>>=8;
						p8d+=2+Len+4;
						if(UARTCtrl->RM_DI_CHOICE!=0)//����DIѡ��:0=��DI,1=����DI
						{//����DI
							x=p8d[1];//������
							x*=6;
							x+=2;
							p8d+=x;
						}
						i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
						x=p8d[1];
						UARTCtrl->RMCSDSubCountMax=x;//����CSD���ӳ������(��DL698�����Լ���趨)
						UARTCtrl->RMROADCountMax=0;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
						if(i>=x)
						{//����CSD�Ӽ���>=������
							return 0;//����
						}
						p8d+=4+(i*6);
						i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
						UARTCtrl->RM_DI=i;
						return (u8*)1;
					}
				//���ҵ���OADƥ��
					if(UARTCtrl->RMCSD[1]==0)//��ǰ���ڳ���CSD(����������)
					{//����ROAD��
						return 0;
					}
					i=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
					x=UARTCtrl->RMCSD[6];//ROAD����
					UARTCtrl->RMROADCountMax=x;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
					if(i>=x)
					{//����ROAD����>=������
						return 0;//����
					}
					OAD=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|UARTCtrl->RMCSD[5];
					i*=4;
					i+=7;
					ROAD=(UARTCtrl->RMCSD[i]<<24)|(UARTCtrl->RMCSD[i+1]<<16)|(UARTCtrl->RMCSD[i+2]<<8)|UARTCtrl->RMCSD[i+3];
		#if PARAMETER_601E==0//���ò���601E�ɼ������ʹ��:0=�ڲ�Ĭ��,1=������
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//�õ��ɼ�������ʼ���ÿ��б����
					for(i=0;i<n;i++)
					{
						p8d=(u8*)p32[0];
						if(p8d[0]==DataType_structure)
						{
							if(p8d[3]==1)
							{//��ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//�ǵ���ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//�ҵ�
											break;
										}
									}
								}
							}
						}
						p32+=2;
					}
					if(i>=n)
					{
						i=NUMmax_601E;
					}
		#else
			#if LENmax_601E>LEN_128KDATABUFF
					#error
			#endif
					MR(ADDR_128KDATABUFF,ADDR_601E,LENmax_601E);
					p8d=(u8*)ADDR_128KDATABUFF;
					for(i=0;i<NUMmax_601E;i++)
					{
						if(p8d[0]==DataType_structure)
						{
							if(p8d[3]==1)
							{//��ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//�ǵ���ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//�ҵ�
											break;
										}
									}
								}
							}
						}
						p8d+=LENper_601E;
					}
		#endif
					if(i>=NUMmax_601E)
					{
						return 0;
					}
					Len=Get_DL698DataLen(p8d+2,0,0,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:b31-b8����,b7-b0Ԫ�ؼ���
					Len>>=8;
					p8d+=2+Len+4;
					if(UARTCtrl->RM_DI_CHOICE!=0)//����DIѡ��:0=��DI,1=����DI
					{//����DI
						x=p8d[1];//������
						x*=6;
						x+=2;
						p8d+=x;
					}
					i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
					x=p8d[1];
					UARTCtrl->RMCSDSubCountMax=x;//����CSD���ӳ������(��DL698�����Լ���趨)
					if(i>=x)
					{//����CSD�Ӽ���>=������
						return 0;//����
					}
					p8d+=4+(i*6);
					i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
					UARTCtrl->RM_DI=i;
					return (u8*)1;
					
				case 3://DL/T698.45
				case ProtocolNO_ABB://ABB
/*
				//�������߳���
					if(UARTCtrl->RMCSD[1]==1)
					{//ROAD
						OAD=UARTCtrl->RMCSD[2+0]<<24;
						OAD|=UARTCtrl->RMCSD[2+1]<<16;
						OAD|=UARTCtrl->RMCSD[2+2]<<8;
						OAD|=UARTCtrl->RMCSD[2+3];
						if(OAD==0x50020200)
						{//���Ӷ���
							i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
							if(i>=1)
							{
								return 0;
							}
							id=UARTCtrl->TaskID;
							id=(id>>8)&0xff;//�����
							i=UARTCtrl->MeterCount[id];//ȡ������
							p8d=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							p8d=Get_Element(p8d,2,8,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
							if(p8d==0)
							{
								return 0;
							}
							i=p8d[1];//ȡ���߷�ʽ
							if(i==1)//δ֪��0�������ࣨ1�����������ߣ�2�����������ߣ�3��
							{//����,��ʵʱ����
								i=UARTCtrl->RMROADCount;//����CSD��ROAD��OAD����(��DL698����ʱ)
								x=UARTCtrl->RMCSD[6];
								UARTCtrl->RMCSDSubCountMax=1;//����CSD���ӳ������(��DL698�����Լ���趨)
								UARTCtrl->RMROADCountMax=x;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
								if(i>=x)
								{//����CSD�Ӽ���>=������
									return 0;//����
								}
								OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
								switch(OAD)
								{
									case 0x20210200://1 ����ʱ��
										return 0;
									case 0x00100201://2 �����й��ܵ���
										break;
									case 0x00200201://3 �����й��ܵ���  
										break;
									case 0x00300200://4 ����޹�1����
										break;
									case 0x00400200://5 ����޹�2����
										break;
									case 0x00500201://6 һ�����޹��ܵ���
										break;
									case 0x00600201://7 �������޹��ܵ���
										break;
									case 0x00700201://8 �������޹��ܵ���
										break;
									case 0x00800201://9 �������޹��ܵ���
										break;
									case 0x20170200://10 �й�����
										break;
									case 0x20180200://11 �޹�����
										break;
									case 0x20000200://12 ��ѹ
										break;										
									case 0x20000201://13 A���ѹ
										break;
									case 0x20010200://14 ����
										break;										
									case 0x20010201://15 A�����
										break;
									case 0x20010400://16 ���ߵ���
										break;
									case 0x20040200://17 �й�����
									case 0x20040202://18 A���й�����
										break;
									case 0x20050200://19 �޹�����
									case 0x20050202://20 A���޹�����
										break;
									case 0x200A0200://21 ��������
									case 0x200A0202://22 A�๦������
										break;
									case 0x11100201://23 ���������������й�������� 
										return 0;
									default:
										return 0;
								}
								UARTCtrl->RM_DI=OAD;
								return (u8*)1;
							}
							else
							{//���࣬�����߶�������
								
							}
						}
					}
*/
				//
					x=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
					if(x)
					{//�Ӽ���!=0
						return 0;
					}
					UARTCtrl->RMROADCountMax=0;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
					UARTCtrl->RMCSDSubCountMax=0;//����CSD���ӳ������(��DL698�����Լ���趨)
					return (u8*)1;
				case 4://CJ/T 188-2004
					return 0;
			}
		case 2://�¼��ɼ�����
			
		#if (USER/100)==17//����ϵ
			if(Terminal_Router->MinuteCollect==1)
				return 0;
		#endif
			i*=LENper_6016;
			i+=ADDR_6016_SDRAM;
			p8s=Get_Element((u8 *)i,2,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			i=p8s[3];//�ɼ�����
			switch(i)
			{
				case 0://0	array ROAD	���ڲɼ��¼�����
					p8s+=4;
					break;
				case 1://1	NULL	����֪ͨ�ɼ������¼�����
					return 0;
				case 2://2	array ROAD	����֪ͨ�ɼ�ָ���¼�����
					p8s+=4;
					break;
				case 3://3  NULL ����֪ͨ�洢���ɵ��¼�����
					return 0;
				default:
					return 0;
			}
			i=UARTCtrl->RMCSDCount;//����CSD����
			x=Get_DL698ByteTypeLenValue(p8s+1);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
			UARTCtrl->RMCSDCountMax=x;//����CSD������
			if(i>=x)
			{//����CSD����>=������
				return 0;
			}
			p8s=Get_Element(p8s,i+1,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			LEN_CSD=Get_DL698DataLen_S(p8s,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if(((LEN_CSD+1)>sizeof(UARTCtrl->RMCSD))||(LEN_CSD<6))
			{
				return 0;
			}
			//MR((u32)&UARTCtrl->RMCSD,(u32)p8s,LEN_CSD);
			//ROADתΪ��ͨ������RCSD��ʽ��
			if(LEN_CSD)
			{
				LEN_CSD--;
			}
			UARTCtrl->RMCSD[0]=DataType_CSD;
			UARTCtrl->RMCSD[1]=1;
			MR(((u32)&UARTCtrl->RMCSD)+2,(u32)p8s+1,LEN_CSD);
		#if CompareMeterEventCount==1//ȫ�¼��ɼ������Ƿ�ȽϷ�������:0=���Ƚ�(ȫ����������д���ļ�),1=�Ƚ�(ֻ�д����仯���¼�д���ļ�)
			RM_MeterEventAutoInc0x20220200(PORTn);//���ܱ��¼�������ѡ�����޷�������(�¼���¼���)���Զ�����
		#endif
			x=UARTCtrl->RMprotocol;//�����Լ����=δ֪ (0)��DL/T645-1997��1����DL/T645-2007��2����DL/T698.45��3����CJ/T 188-2004��4��
			switch(x)
			{
				case 1://DL/T645-1997
					return 0;
				case 2://DL/T645-2007
					OAD=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|UARTCtrl->RMCSD[5];
					i=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
					UARTCtrl->RMCSDSubCountMax=2;//����CSD���ӳ������(��DL698�����Լ���趨)
					if(i>=2)
					{//����CSD�Ӽ���>=2
						return 0;
					}
					OAD=Get_Event645_DI(OAD,i);//��ȡ��ǰ�ĳ�645��������OAD=�¼�OAD��index=��ǰOAD��Ӧ�ĵڼ������ݱ�ʾ����0��ʼ������0=û��Ҫ���������ݱ�ʶ
					if(OAD==0)
					{
						return 0;
					}
					UARTCtrl->RM_DI=OAD;
					UARTCtrl->RM_DI_N=0;
					return (u8*)1;
				case 3://DL/T698.45
					x=UARTCtrl->RMCSDSubCount;//����CSD�Ӽ���
					if(x)
					{//�Ӽ���!=0
						return 0;
					}
					UARTCtrl->RMROADCountMax=0;//����CSD��ROAD����OAD������(��DL698�����Լ���趨)
					UARTCtrl->RMCSDSubCountMax=0;//����CSD���ӳ������(��DL698�����Լ���趨)
					return (u8*)1;
				case 4://CJ/T 188-2004
					return 0;
				default://ABB,Landis��
					return 0;
			}
		case 3://͸������
//����2��͸����������=structure//ɾ��ʱstructure�����ݵ�Ԫ��Ϊ0(NULL)
//{
//	�������     unsigned��
//	�������ݼ�   array �������ݣ�
//	�������ݡ�=structure
//	{
//		ͨ�ŵ�ַ        	TSA��
//		��ʼǰ�ű�id   	long-unsigned��
//		��ɺ�ű�id   	long-unsigned��
//		�������Ʊ�־  	structure��
//		{
//			�ϱ�͸������������ȴ���������  bool��
//			�ȴ��������ĳ�ʱʱ�䣨�룩      long-unsigned��
//			����ȶԱ�ʶ    enum{���ȶԣ�0�����ȣ�1�����ȶ��ϱ���2��}��
//			����ȶԲ���    structure
//			{
//				�����ֽ�      unsigned��
//				��ȡ��ʼ      long-unsigned��
//				��ȡ����      long-unsigned
//			}
//		}
//		�������ļ� 	 	array ��������
//		�������ġ�=structure
//		{
//			�������  unsigned��
//			��������  octet-string
//		}
//	}
//	�洢���     long-unsigned
//}
			i*=LENper_6018;
			i+=ADDR_6018;
			MR(ADDR_DATABUFF,i,7);
			p8s=(u8*)ADDR_DATABUFF;
			if(p8s[0]!=DataType_structure)
			{
				return 0;
			}
			id>>=8;
			id&=0xff;
			x=Get_DL698ByteTypeLenValue(p8s+5);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
			if(UARTCtrl->MeterCount[id]>=x)
			{
				return 0;
			}
			p8s=Get_Element((u8*)(i+4),UARTCtrl->MeterCount[id]+1,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			//�������ļ�
			p8s=Get_Element(p8s,5,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			i=UARTCtrl->RMCSDCount;
			x=Get_DL698ByteTypeLenValue(p8s+1);//�õ�DL698�ֽ����ͳ��ȵ�Ԫ��ֵ
			UARTCtrl->RMCSDCountMax=x;//����CSD������
			if(i>=x)
			{
				return 0;
			}
			p8s=Get_Element(p8s,UARTCtrl->RMCSDCount+1,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			return p8s+4;
		case 4://�ϱ�����
		case 5://�ű�����
		default:
			return 0;
	}
}















