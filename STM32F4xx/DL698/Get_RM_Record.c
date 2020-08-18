
//�õ������¼
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_Record.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"


#include "../DL698/DL698_JSON.h"


u32 SomeTaskID(u32 id)//���Ƿ�����ͬ����OAD��ROAD������;���:����id��;����:0=û��ͬ,1=����ͬ
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 m;
	u32 NUM_CSD;
	u32 OAD;
	u32 ROAD;
	u32 NUM_OAD;
	u8* pCSD;
	u8* p8;
	
	p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
	if(p8[0]!=DataType_structure)
	{
		return 0;//����:0=û��ͬ,1=����ͬ
	}
	id=p8[11];//�������0-127
	p8=(u8*)ADDR_6014_SDRAM+(id*LENper_6014);
	p8=Get_Element(p8,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
	if(p8==0)
	{
		return 0;//����:0=û��ͬ,1=����ͬ
	}
	pCSD=p8;
	
	NUM_CSD=pCSD[1];//RCSD��=SEQUENCE OF CSD
	pCSD+=2;
	while(NUM_CSD--)
	{
		if(pCSD[1]==0)
		{//OAD
			OAD=(pCSD[2]<<24)|(pCSD[3]<<16)|(pCSD[4]<<8)|pCSD[5];
			switch(OAD)
			{
				case 0x40010200://ͨ�ŵ�ַ
				case 0x202A0200://Ŀ���������ַ
				case 0x60400200://�ɼ�����ʱ��
				case 0x60410200://�ɼ��ɹ�ʱ��
				case 0x60420200://�ɼ��洢ʱ��
					pCSD+=6;
					continue;
			}
			for(i=0;i<NUMmax_6014;i++)
			{
				if(i==id)
				{
					continue;
				}
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//��ͨ�ɼ�����
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//��ѡ��  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==0)
							{
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if((y>>8)==(OAD>>8))
								{//�ҵ���ͨ�ɼ�������
									return 1;//����:0=û��ͬ,1=����ͬ
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
					}
				}
			}
			pCSD+=6;
		}
		else
		{//ROAD
			ROAD=(pCSD[2]<<24)|(pCSD[3]<<16)|(pCSD[4]<<8)|pCSD[5];
			for(i=0;i<NUMmax_6014;i++)
			{
				if(i==id)
				{
					continue;
				}
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//��ͨ�ɼ�����
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//��ѡ��  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==1)
							{//ROAD
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if(y==ROAD)
								{
									NUM_OAD=pCSD[6];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pCSD[7+(j*4)]<<24)|(pCSD[7+(j*4)+1]<<16)|(pCSD[7+(j*4)+2]<<8)|pCSD[7+(j*4)+3];
										m=p8[6];
										for(z=0;z<m;z++)
										{
											y=(p8[7+(z*4)]<<24)|(p8[7+(z*4)+1]<<16)|(p8[7+(z*4)+2]<<8)|p8[7+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												return 1;//����:0=û��ͬ,1=����ͬ
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
					}
				}
			}
			pCSD+=Get_DL698DataLen_S(pCSD,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
		}			
	}
	return 0;//����:0=û��ͬ,1=����ͬ
}

u32 FileID_ChangeTaskNO(u32 FileID)//�ļ�ID���궨����������,�����޹صĹ̶�Ϊ0xFE
{
	u32 ProjectType;//��������
	u32 CollectionType;//�ɼ�����
	u32 TaskNo;//�����
	
	TaskNo=(FileID>>12)&0xff;//�����
	ProjectType=SomeTaskID(TaskNo);//���Ƿ�����ͬ����OAD��ROAD������;���:����id��;����:0=û��ͬ,1=����ͬ
	if(ProjectType)
	{
		ProjectType=FileID>>28;//��������
		CollectionType=(FileID>>24)&0xf;//�ɼ�����
		switch(ProjectType)//��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
		{
			case 1://1=��ͨ�ɼ�����
				switch(CollectionType)//�ɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�,4=����
				{
					default:
					case 0://0=�ɼ���ǰ����
					#if FileID_10x==0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=10x(��ͨ�ɼ�����,�ɼ���ǰ����)�ļ���ʶ��������ID��
						TaskNo=0xFE;//�����
					#endif
						break;
					//1=�ɼ��ϵ�N��
					case 2://2=������ʱ��ɼ�(�ն�������)
						//�ն���
					#if FileID_12xD==0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=12x(��ͨ�ɼ�����,������ʱ��ɼ��ն���)�ļ���ʶ��������ID��
						TaskNo=0xFE;//�����
					#endif
						break;
					case 3://3=��ʱ�����ɼ�(��������)
					#if FileID_136==0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=136(��ͨ�ɼ�����,��ʱ�����ɼ�����)�ļ���ʶ��������ID��
						TaskNo=0xFE;//�����
					#endif
						break;
					//4=����
					case 14://��չ(�¶�������)
						//�¶���
					#if FileID_12xM==0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=12x(��ͨ�ɼ�����,������ʱ��ɼ��¶���)�ļ���ʶ��������ID��
						TaskNo=0xFE;//�����
					#endif
						break;
				}
				break;
			case 2://2=�¼��ɼ�����
			#if FileID_2xx==0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=2xx(�¼��ɼ�����)�ļ���ʶ��������ID��
				TaskNo=0xFE;//�����
			#endif
				break;
			//3=͸������,4=�ϱ�����,5=�ű�����
		}
		FileID&=~(0xff<<12);
		FileID|=TaskNo<<12;
	}	
	return FileID;
}

u32 Get_FileID(u32 PORTn)//UART�������FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
{
	u32 i;
	u32 FileID;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	FileID=UARTCtrl->TaskID;//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	if(((FileID>>24)&0xf)==1)
	{//��ͨ�ɼ�����
		i=FileID&0xff;//������
		p8=(u8*)(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
		p8=Get_Element(p8,4,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
		if(p8)
		{
			i=(p8[0]<<24)|(p8[1]<<16)|(p8[2]<<8)|p8[3];
			switch(i)
			{
				case 0x5B015002://���Ӷ���(����)
					FileID&=~(0xf<<20);
					FileID|=3<<20;//3=��ʱ�����ɼ�(��������)
					break;
				case 0x5B015004://�ն���
					FileID&=~(0xf<<20);
					FileID|=2<<20;//2=������ʱ��ɼ�(�ն�������)
					break;
				case 0x5B015006://�¶���
					FileID&=~(0xf<<20);
					FileID|=14<<20;//14=��չ(�¶�������)
					break;
			#if (USER/100)==17
				case 0x5B015005://�����ն���
					FileID&=~(0xf<<20);
					FileID|=15<<20;//15=��չ(�¶�������)
					break;
			#endif
			}
		}
	}
	
//#if(USER/100)==17//����ϵ
//	switch(UARTCtrl->res23)
//	{
//		case 3:
//		case 4:
//			FileID&=~(0xf<<20);
//			FileID|=3<<20;//3=��ʱ�����ɼ�(��������)
//			FileID>>=8;
//			FileID<<=12;
//		  FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff22);
//			FileID<<=12;
//			break;
//		case 1:
//		case 6:
//			FileID&=~(0xf<<20);
//			FileID|=2<<20;//2=������ʱ��ɼ�(�ն�������)
//			FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff16);
//			FileID<<=12;
//			break;
//		case 2:
//		case 7:
//			FileID&=~(0xf<<20);
//			FileID|=14<<20;//14=��չ(�¶�������)
//			FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff18);
//			FileID<<=12;
//			break;
//		case 8:
//		case 9:
//			FileID&=~(0xf<<20);
//			FileID|=15<<20;//15=��չ(�������Ӷ���)
//			FileID=((FileID>>12)|0xff);
//			FileID=(FileID&0xffffff19);
//			FileID<<=12;
//			break;
//		default:
//			break;	
//	}
//#else
	FileID>>=8;
	FileID<<=12;
	FileID=FileID_ChangeTaskNO(FileID);//�ļ�ID���궨����������,�����޹صĹ̶�Ϊ0xFE
//#endif
	FileID|=0xf<<20;//�洢ʱ�����Ͳ���
	return FileID;
}

u32 IDtoFileID(u32 id)//���:�����id,����:�ļ��洢FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����;��Чid����=0
{
	u32 i;
	u32 FileID;
	u8* p8;
	
	p8=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
	if(p8[0]!=DataType_structure)
	{
		return 0;//��Чid
	}
	i=p8[9];//��������(1-5)
	FileID=i<<28;
	switch(i)
	{
		case 1://1=��ͨ�ɼ�����,
			i=p8[11];//�������0-127
			p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);
			if(p8[0]!=DataType_structure)
			{
				return 0;//��Чid
			}
			//�ɼ�����
			FileID|=(p8[10]&0xf)<<24;
			p8=Get_Element(p8,4,1,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8==0)
			{
				return 0;//��Чid
			}
			i=(p8[0]<<24)|(p8[1]<<16)|(p8[2]<<8)|p8[3];
			switch(i)
			{
				case 0x5B015002://���Ӷ���(����)
					FileID&=~(0xf<<24);
					FileID|=3<<24;//3=��ʱ�����ɼ�(��������)
					break;
				case 0x5B015004://�ն���
					FileID&=~(0xf<<24);
					FileID|=2<<24;//2=������ʱ��ɼ�(�ն�������)
					break;
				case 0x5B015006://�¶���
					FileID&=~(0xf<<24);
					FileID|=14<<24;//14=��չ(�¶�������)
					break;
			#if (USER/100)==17
				case 0x5B015005://�����ն���
					FileID&=~(0xf<<24);
					FileID|=15<<24;//15=��չ(�����ն���)
					break;
			#endif
			}
			//�洢ʱ������
			FileID|=0xf<<20;//�洢ʱ�����Ͳ���
			//
			FileID|=id<<12;//�����
			FileID|=0xf<<8;//�˿ں�
			//FileID|=0;//�ļ�����
			break;
		case 2://2=�¼��ɼ�����,
			i=p8[11];//�������0-127
			p8=(u8*)ADDR_6016_SDRAM+(i*LENper_6016);
			if(p8[0]!=DataType_structure)
			{
				return 0;//��Чid
			}
			FileID|=0xf<<24;//�ɼ�����
			FileID|=0xf<<20;//�洢ʱ��
			FileID|=id<<12;//�����
			FileID|=0xf<<8;//�˿ں�
			//FileID|=0;//�ļ�����
			break;
		case 3://3=͸������,
		case 4://4=�ϱ�����,
		case 5://5=�ű�����
		default:
			FileID|=0xf<<24;//�ɼ�����
			FileID|=0xf<<20;//�洢ʱ��
			FileID|=id<<12;//�����
			FileID|=0xf<<8;//�˿ں�
			//FileID|=0;//�ļ�����
			break;
	}
	FileID=FileID_ChangeTaskNO(FileID);//�ļ�ID���궨����������,�����޹صĹ̶�Ϊ0xFE
	return FileID;
}

u32 IDtoTaskID(u32 id)//���:�����id;���ڣ�TaskID
{
	u32 x;
	u32 TaskID;//��ǰ�ɼ�����ID;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
	u8* p8s;
	u8* p8d;
	
	if(id>=(RMTASKmax+ExtRMTASKmax))
	{
		return 0;
	}
	p8s=(u8*)ADDR_6012_2_SDRAM+(id*LENper_6012_2);
	TaskID=p8s[33];//���ȼ�
	if(TaskID>15)
	{
		TaskID=15;
	}
	TaskID<<=4;
	x=p8s[9];//��������(1-5)
	if(x>=4)
	{
		return 0;
	}
	x&=0xf;
	TaskID|=x;
	switch(x)//��������
	{
		case 1://��ͨ�ɼ����� ��1��
			//��ͨ����ʱ�ɼ�����
			TaskID<<=4;
			x=p8s[11];//�������0-127
			p8d=(u8*)ADDR_6014_SDRAM+(x*LENper_6014);
			TaskID|=p8d[10]&0xf;
			//��ͨ����ʱ�洢ʱ��
			TaskID<<=4;
			p8d=Get_Element(p8d,6,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8d)
			{
				TaskID|=p8d[1]&0xf;
			}
			break;
		case 2://�¼��ɼ����� ��2��
			//�¼��ɼ�����ʱ�ɼ�����
			TaskID<<=4;
			x=p8s[11];//�������0-127
			p8d=(u8*)ADDR_6016_SDRAM+(x*LENper_6016);
			x=p8d[7]&0xf;
			TaskID|=x;
			//�¼��ɼ�����ʱ�洢ʱ��=0
			TaskID<<=4;
			break;
		//csae 3://͸������     ��3��
		//case 4://�ϱ�����     ��4��
		//case 5://�ű�����     ��5��
		default:
			//�ɼ�����=0
			//�洢ʱ��=0
			TaskID<<=8;
			break;
	}
	TaskID<<=8;
	TaskID|=id;//�����
	TaskID<<=8;
	x=p8s[11];//�������0-127
	TaskID|=x;//�������
		
	return TaskID;
}


u32 RCSDtoID(u8* pRCSD)//��¼��ѡ��RCSD���Ӧ�������,����:�����ڲɼ�����������,>=RMTASKmax��ʾ��Ч�����
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 m;
	u32 offset;
	u32 NUM_CSD;
	u32 OAD;
	u32 ROAD;
	u32 NUM_OAD;
	u8* p8;
	
//X RecordData��=structure
//{
//X  ����������������      OAD��
//	��¼�Ͷ�������������  RCSD��
//	��ѡ��                RSD
//}
//X 02 03
//X 51 60 12 03 00 ����������������      OAD
//60 01 RCSD��=SEQUENCE OF CSD
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 
//5a 00 ��ѡ��                RSD
	NUM_CSD=pRCSD[1];//RCSD��=SEQUENCE OF CSD
	pRCSD+=2;
//����ͨ�����б�
	offset=0;
	while(NUM_CSD--)
	{
		if(pRCSD[offset]==0)
		{//OAD
			OAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			switch(OAD)
			{
				case 0x40010200://ͨ�ŵ�ַ
				case 0x202A0200://Ŀ���������ַ
				case 0x60400200://�ɼ�����ʱ��
				case 0x60410200://�ɼ��ɹ�ʱ��
				case 0x60420200://�ɼ��洢ʱ��
					offset+=5;
					continue;
			}
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//��ͨ�ɼ�����
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//��¼��ѡ��  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==0)
							{
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if((y>>8)==(OAD>>8))
								{//�ҵ���ͨ�ɼ�������
									
									//��ͨ�����б�תΪ�����
									for(j=0;j<NUMmax_6012_2;j++)
									{
										p8=(u8*)ADDR_6012_2_SDRAM+(j*LENper_6012_2);//�������ñ�
										if(p8[0]==DataType_structure)
										{
											if(p8[9]==1)
											{//��ͨ�ɼ�����
												if(p8[11]==i)
												{//�������
													return j;//�������ú�
												}
											}
										}
									}
									return RMTASKmax;
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
					}
				}
			}
			offset+=5;
		}
		else
		{//ROAD
			ROAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//��ͨ�ɼ�����
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//��¼��ѡ��  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==1)
							{//ROAD
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if(y==ROAD)
								{
									NUM_OAD=pRCSD[offset+5];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pRCSD[offset+6+(j*4)]<<24)|(pRCSD[offset+6+(j*4)+1]<<16)|(pRCSD[offset+6+(j*4)+2]<<8)|pRCSD[offset+6+(j*4)+3];
										m=p8[6];
										for(z=0;z<m;z++)
										{
											y=(p8[7+(z*4)]<<24)|(p8[7+(z*4)+1]<<16)|(p8[7+(z*4)+2]<<8)|p8[7+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												
												//��ͨ�����б�תΪ�����
												for(j=0;j<NUMmax_6012_2;j++)
												{
													p8=(u8*)ADDR_6012_2_SDRAM+(j*LENper_6012_2);//�������ñ�
													if(p8[0]==DataType_structure)
													{
														if(p8[9]==1)
														{//��ͨ�ɼ�����
															if(p8[11]==i)
															{//�������
																return j;//�������ú�
															}
														}
													}
												}
												return RMTASKmax;
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
					}
				}
			}
			i=pRCSD[offset+5];
			i*=4;
			i+=6;
			offset+=6;
		}			
	}
	return RMTASKmax;
}

u32 RCSDtoFileID(u8* pRCSD)//��¼��ѡ��RCSD���Ӧ��FileID,���:pRCSDָ�������ֽ�;����:0��ʾû�ҵ���ЧFileID,����2������ͬ����ʱTaskNO=0xff;RCSD��ֻ��ͨ�ŵ�ַ�ȷǳ���������ʱ����0xffffffff
{
	u32 i;
	u32 j;
	u32 x;
	u32 y;
	u32 z;
	u32 n;
	u32 m;
	u32 k;
	u32 offset;
	u32 NUM_CSD;
	u32 OAD;
	u32 ROAD;
	u32 NUM_OAD;
	u8* p8;
	u32 ProjectID;//�ɼ�������
	u32 NUM_ProjectID;//�����е���ͬ�ɼ���������
	u32 Data_Com;//��ͨ�ŵ�ַ�ȹ�������
	ProjectID=0;//�ɼ�������
	NUM_ProjectID=0;//�����е���ͬ�ɼ���������
	Data_Com=0;//RCSD����ͨ�ŵ�ַ�ȷǳ���������
//X RecordData��=structure
//{
//X  ����������������      OAD��
//	��¼�Ͷ�������������  RCSD��
//	��ѡ��                RSD
//}
//X 02 03
//X 51 60 12 03 00 ����������������      OAD
//60 01 RCSD��=SEQUENCE OF CSD
//01 50 04 02 00 02 00 10 02 00 00 20 02 00 
//5a 00 ��ѡ��                RSD
	NUM_CSD=pRCSD[1];//RCSD��=SEQUENCE OF CSD
	pRCSD+=2;
//����ͨ�����б�
	offset=0;
	k=0;
	while(NUM_CSD--)
	{
		if(pRCSD[offset]==0)
		{//OAD
			OAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			switch(OAD)
			{
				case 0x40010200://ͨ�ŵ�ַ
				case 0x202A0200://Ŀ���������ַ
				case 0x60400200://�ɼ�����ʱ��
				case 0x60410200://�ɼ��ɹ�ʱ��
				case 0x60420200://�ɼ��洢ʱ��
					Data_Com=0xffffffff;//RCSD����ͨ�ŵ�ַ�ȷǳ���������
					offset+=5;
					continue;
			}
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//��ͨ�ɼ�����
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//��¼��ѡ��  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==0)
							{
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if((y>>8)==(OAD>>8))
								{//�ҵ���ͨ�ɼ�������
									NUM_ProjectID++;//�����е���ͬ�ɼ���������
									if(NUM_ProjectID>=2)//�����е���ͬ�ɼ���������
									{
										break;
									}
									ProjectID=i;//�ɼ�������
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
					}
				}
			}
			offset+=5;
		}
		else
		{//ROAD
			ROAD=(pRCSD[offset+1]<<24)|(pRCSD[offset+2]<<16)|(pRCSD[offset+3]<<8)|pRCSD[offset+4];
			if(Terminal_Router->MinuteCollect==1)//֧�ַ��Ӽ��ɼ�
			//if(1)//֧�ַ��Ӽ��ɼ�
			{
				if(ROAD==0x50040200)
				{
					return 0x13f16000;
				}
				if(ROAD==0x50060200)
				{
					return 0x13f18000;
				}
			}
			k=0;
			for(i=0;i<NUMmax_6014;i++)
			{
				p8=(u8*)ADDR_6014_SDRAM+(i*LENper_6014);//��ͨ�ɼ�����
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//��¼��ѡ��  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[1]==1)
							{//ROAD
								y=(p8[2]<<24)|(p8[3]<<16)|(p8[4]<<8)|p8[5];
								if(y==ROAD)
								{
									NUM_OAD=pRCSD[offset+5];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pRCSD[offset+6+(j*4)]<<24)|(pRCSD[offset+6+(j*4)+1]<<16)|(pRCSD[offset+6+(j*4)+2]<<8)|pRCSD[offset+6+(j*4)+3];
										m=p8[6];
										for(z=0;z<m;z++)
										{
											y=(p8[7+(z*4)]<<24)|(p8[7+(z*4)+1]<<16)|(p8[7+(z*4)+2]<<8)|p8[7+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												NUM_ProjectID++;//�����е���ͬ�ɼ���������
												if(NUM_ProjectID>=2)//�����е���ͬ�ɼ���������
												{
													break;
												}
												ProjectID=i;//�ɼ�������
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
					}
				}
			}
		#if(USER/100)==17//����ϵ
			if(NUM_ProjectID==0)
			{
				for(i=0;i<NUMmax_6016;i++)
				{
				p8=(u8*)ADDR_6016_SDRAM+(i*LENper_6016);//�¼��ɼ�����
				if(p8[0]==DataType_structure)
				{
					p8=Get_Element(p8,2,2,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
					if(p8)
					{//��¼��ѡ��  array CSD
						n=p8[1];
						p8+=2;
						for(x=0;x<n;x++)
						{
							if(p8[0]==0x52)
							{//ROAD
								y=(p8[1]<<24)|(p8[2]<<16)|(p8[3]<<8)|p8[4];
								if(y==ROAD)
								{
									NUM_OAD=pRCSD[offset+5];
									for(j=0;j<NUM_OAD;j++)
									{
										OAD=(pRCSD[offset+6+(j*4)]<<24)|(pRCSD[offset+6+(j*4)+1]<<16)|(pRCSD[offset+6+(j*4)+2]<<8)|pRCSD[offset+6+(j*4)+3];
										m=p8[5];
										for(z=0;z<m;z++)
										{
											y=(p8[6+(z*4)]<<24)|(p8[6+(z*4)+1]<<16)|(p8[6+(z*4)+2]<<8)|p8[6+(z*4)+3];
											if((y>>8)==(OAD>>8))
											{
												NUM_ProjectID++;//�����е���ͬ�ɼ���������
												if(NUM_ProjectID>=2)//�����е���ͬ�ɼ���������
												{
													break;
												}
												ProjectID=i;//�ɼ�������
												k=1;
											}
										}
									}
								}
							}
							p8+=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						}
					}
				}
			}
			}
			#endif
			
//		#if(USER/100)==17//����ϵ
//			if(ROAD==0x30110200)
//				return 0x23000000;
//		#endif
			i=pRCSD[offset+5];
			i*=4;
			i+=6;
			offset+=i;
			//offset+=6;
		}			
	}
	if(NUM_ProjectID==0)//�����е���ͬ�ɼ���������
	{
		if(Data_Com)//RCSD����ͨ�ŵ�ַ�ȷǳ���������
		{
			return Data_Com;
		}
		return 0;//����:0��ʾû�ҵ���ЧFileID
	}
	//��ͨ�����б�תΪ�����
	for(j=0;j<NUMmax_6012_2;j++)
	{
		p8=(u8*)ADDR_6012_2_SDRAM+(j*LENper_6012_2);//�������ñ�
		if(p8[0]==DataType_structure)
		{
			if((k==0)&&(p8[9]==1))
			{//��ͨ�ɼ�����
				if(p8[11]==ProjectID)
				{//�������
					i=IDtoFileID(j);//���:�����id,����:�ļ��洢FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����;��Чid����=0
					if(NUM_ProjectID>=2)//�����е���ͬ�ɼ���������
					{
						i|=0xff<<12;
					}
					return i;//FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
				}
			}
			else if((k==1)&&(p8[9]==2))
			{
				if(p8[11]==ProjectID)
				{//�������
					i=IDtoFileID(j);//���:�����id,����:�ļ��洢FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����;��Чid����=0
					if(NUM_ProjectID>=2)//�����е���ͬ�ɼ���������
					{
						i|=0xff<<12;
					}
					return i;//FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
				}
			}
		}
	}
	return 0;//����:0��ʾû�ҵ���ЧFileID
}


u32 Get_RecordFileName(u32 FileID,u32 FileCount,u8* pTime)//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
{
	u32 i;
	msFILE_TypeDef *file;
	u32 ProjectType;//��������
	u32 CollectionType;//�ɼ�����
	u32 StorageType;//�洢ʱ������
	u32 TaskNo;//�����
	u32 SearchCount;
	
	ProjectType=FileID>>28;//��������
	CollectionType=(FileID>>24)&0xf;//�ɼ�����
	StorageType=(FileID>>20)&0xf;//�洢ʱ������
	TaskNo=(FileID>>12)&0xff;//�����

	SearchCount=Comm_Ram->msFILESearchCount;
	if(SearchCount<recordFILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(SearchCount*sizeof(msFILE_TypeDef)));
	}
	else
	{
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((SearchCount-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	for(;SearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);SearchCount++)
	{
		if(SearchCount==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			//b7-b0�ļ�����
			if((FileID&0xff)!=0)
			{
				file++;
				continue;
			}
			//b31-b28��������
			if(ProjectType!=0xf)//��������
			{
				if(ProjectType!=(FileID>>28))
				{
					file++;
					continue;
				}
			}
			//b27-b24�ɼ�����
			if(CollectionType!=0xf)//�ɼ�����
			{
				if(CollectionType!=((FileID>>24)&0xf))
				{
					file++;
					continue;
				}
			}
			//b23-b20�洢ʱ������
			if(StorageType!=0xf)//�洢ʱ������
			{
				if(StorageType!=((FileID>>20)&0xf))
				{
					file++;
					continue;
				}
			}
			//b19-b12�����
			if(TaskNo!=0xff)//�����
			{
				if(TaskNo!=((FileID>>12)&0xff))
				{
					file++;
					continue;
				}
			}
			//�ļ�����
			if(FileCount!=0xffffffff)//�ļ�����
			{
				if(FileCount!=file->FILECOUNT)
				{
					file++;
					continue;
				}
			}
			//�洢ʱ��
			if(pTime)
			{
				for(i=0;i<6;i++)//�벻�Ƚ�
				{
					if(pTime[i]!=file->save_date_time_s[i])
					{
						break;
					}
				}
				if(i<6)
				{
					file++;
					continue;
				}
			}
			Comm_Ram->msFILESearchCount=SearchCount+1;//�ļ���������
			if(SearchCount>=recordFILENAMEmax_FLASH)
			{
				SearchCount-=recordFILENAMEmax_FLASH;
				SearchCount+=FILENAMEmax_FLASH;
			}
			return SearchCount;
		}
		//file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//�ļ���������
	return 0xffffffff;
}

u32 Get_RecordFileName_JS(u32 FileID,u32 FileCount,u8* pTime)//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
{
	u32 i;
	msFILE_TypeDef *file;
	u32 ProjectType;//��������
	u32 CollectionType;//�ɼ�����
	u32 StorageType;//�洢ʱ������
	u32 TaskNo;//�����
	u32 SearchCount;
	
	ProjectType=FileID>>28;//��������
	CollectionType=(FileID>>24)&0xf;//�ɼ�����
	StorageType=(FileID>>20)&0xf;//�洢ʱ������
	TaskNo=(FileID>>12)&0xff;//�����

	SearchCount=Comm_Ram->msFILESearchCount;
	if(SearchCount<recordFILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(SearchCount*sizeof(msFILE_TypeDef)));
	}
	else
	{
		return 0xffffffff;
		//file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((SearchCount-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	
	//for(;SearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);SearchCount++)
	for(;SearchCount<(recordFILENAMEmax_FLASH);SearchCount++)
	{
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			//b7-b0�ļ�����
			if((FileID&0xff)!=0)
			{
				file++;
				continue;
			}
			//b31-b28��������
			if(ProjectType!=0xf)//��������
			{
				if(ProjectType!=(FileID>>28))
				{
					file++;
					continue;
				}
			}
			//b27-b24�ɼ�����
			if(CollectionType!=0xf)//�ɼ�����
			{
				if(CollectionType!=((FileID>>24)&0xf))
				{
					file++;
					continue;
				}
			}
			//b23-b20�洢ʱ������
			if(StorageType!=0xf)//�洢ʱ������
			{
				if(StorageType!=((FileID>>20)&0xf))
				{
					file++;
					continue;
				}
			}
			//b19-b12�����
			if(TaskNo!=0xff)//�����
			{
				if(TaskNo!=((FileID>>12)&0xff))
				{
					file++;
					continue;
				}
			}
			//�ļ�����
			if(FileCount!=0xffffffff)//�ļ�����
			{
				if(FileCount!=file->FILECOUNT)
				{
					file++;
					continue;
				}
			}
			//�洢ʱ��
			if(pTime)
			{
				for(i=0;i<6;i++)//�벻�Ƚ�
				{
					if(pTime[i]!=file->save_date_time_s[i])
					{
						break;
					}
				}
				if(i<6)
				{
					file++;
					continue;
				}
			}
			Comm_Ram->msFILESearchCount=SearchCount+1;//�ļ���������
			//if(SearchCount>=recordFILENAMEmax_FLASH)
			//{
				//SearchCount-=recordFILENAMEmax_FLASH;
				//SearchCount+=FILENAMEmax_FLASH;
			//}
			return SearchCount;
		}
		file++;
	}
	Comm_Ram->msFILESearchCount=SearchCount;//�ļ���������
	return 0xffffffff;
}

u32 Get_LastRecordFileDataHasID(u32 ID,u16* pAddr)//
{
	u32 i;
	u32 n;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//�����¼�ļ�ͷ�ļ�
	u32 FileCount;
	u32 offset;
	
	FileID=IDtoFileID(ID);//���:�����id,����:�ļ��洢FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����;��Чid����=0
	FileID&=0xfffff0ff;
	ms=(ms_Type *)Get_ms();
	//�����ļ�����0�����ļ�����1
	for(FileCount=0;FileCount<2;FileCount++)
	{
		Comm_Ram->msFILESearchCount=0;
		while(1)
		{
			filename=Get_RecordFileName(FileID,FileCount,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename==0xffffffff)
			{//û�ҵ�
				break;
			}
			if(filename<recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>LEN_128KDATABUFF)
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
			for(i=0;i<n;i++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							break;
						}
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			if(i<n)
			{//�ҵ�
				i=RecordFileHead->LEN_DATA;
				if(i>LEN_128KDATABUFF)
				{
					i=LEN_128KDATABUFF;
				}
				ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF,i);
				return i;//����:���뵽ADDR_128KDATABUFF���ݳ��ȣ�0=û�ҵ�
			}
		}
	}
	return 0;//����:0=û�ҵ�,1=�ҵ������뵽ADDR_128KDATABUFF
}
	
u32 Get_LastRecordFileData(u32 ID,u16* pAddr)//�õ�����μ�¼�ļ�����(�����ļ�����0�����ļ�����1):���:ID=�����,pTSA=�����͵ĵ���ַ;����:���뵽ADDR_128KDATABUFF�����ݳ��ȣ�0=û�ҵ�
{
	u32 i;
	u32 n;
	u32 FileID;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//�����¼�ļ�ͷ�ļ�
	u32 FileCount;
	u32 offset;
	
	FileID=IDtoFileID(ID);//���:�����id,����:�ļ��洢FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����;��Чid����=0
	FileID|=0xff<<12;//������޹�
	ms=(ms_Type *)Get_ms();
	//�����ļ�����0�����ļ�����1
	for(FileCount=0;FileCount<2;FileCount++)
	{
		Comm_Ram->msFILESearchCount=0;
		while(1)
		{
			filename=Get_RecordFileName(FileID,FileCount,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
			if(filename==0xffffffff)
			{//û�ҵ�
				break;
			}
			if(filename<recordFILENAMEmax_FLASH)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
			}
			else
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
			}
			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>LEN_128KDATABUFF)
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF);
			for(i=0;i<n;i++)
			{
				if(RecordFileHead->Addr0==pAddr[0])
				{
					if(RecordFileHead->Addr1==pAddr[1])
					{
						if(RecordFileHead->Addr2==pAddr[2])
						{
							break;
						}
					}
				}
				offset+=RecordFileHead->LEN_DATA;
				RecordFileHead++;
			}
			if(i<n)
			{//�ҵ�
				i=RecordFileHead->LEN_DATA;
				if(i>LEN_128KDATABUFF)
				{
					i=LEN_128KDATABUFF;
				}
				ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF,i);
				return i;//����:���뵽ADDR_128KDATABUFF���ݳ��ȣ�0=û�ҵ�
			}
		}
	}
	return 0;//����:0=û�ҵ�,1=�ҵ������뵽ADDR_128KDATABUFF
}

/*
u32 Get_RecordFileData_RCSD(u8 *pRCSD,u8 *pAddr,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
{
	u32 i;
	u32 x;
	u32 Offset;
	u32 Len;
	u32 Nrcsd;
	u32 OADn;
	u32 Nroad;
	u32 Mcsd;
	u32 OADm;
	u32 Mroad;
	u32 OADcount;
	u8 *pData;
	u32 fDATA;//����ѡ��OAD���ݱ�־
	
	fDATA=0;//����ѡ��OAD���ݱ�־
	//�����¼���ݿ�ʼָ��
	Offset=21;
	Mcsd=pRecord[Offset+1];
	Offset+=2;
	for(i=0;i<Mcsd;i++)
	{
		if(pRecord[Offset+1]==0)
		{//OAD
			
			Offset+=6;
		}
		else
		{//ROAD
			x=pRecord[Offset+6];//����OAD����
			
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=(u8*)pRecord+Offset;
	//
	Nrcsd=pRCSD[0];
	if(Nrcsd==0)
	{
		//RCSD��=SEQUENCE OF CSD ����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ����
		Nrcsd=Mcsd;
		Offset-=21+2;
		pRCSD=(u8*)ADDR_1kDATABUFF;
		pRCSD[0]=Nrcsd;//RCSD��SEQUENCE OF����
		MR((u32)pRCSD+1,(u32)pRecord+21+2,Offset);//���ջ�����ǰ,5*m���=5*256����Ҳֻ�ܳ������ջ����Ƥ�뷢���������ᳬ��������
		//ȥ���ݴ���ʱ������ǰ����
		pRCSD++;
		Len=0;
		Offset=0;
		for(i=0;i<Mcsd;i++)
		{
			if(pRCSD[Offset+1]==0)
			{//OAD
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,5);
				Len+=5;
				Offset+=6;
			}
			else
			{//ROAD
				x=pRecord[Offset+6];//����OAD����
				x*=4;
				x+=7;
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,x-1);
				Len+=x-1;
				Offset+=x;
			}
		}
	}
	Len=0;
	pRCSD++;
	while(Nrcsd--)
	{
//���ܱ����ݿ���,���Ȱ�ʵ��д����
//0 7byte �ɼ�����ʱ��  date_time_s(����������)
//7 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//14 7byte �ɼ��洢ʱ��  date_time_s(����������)
//21  �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������)(ROADʱ��д���ܹ�����OAD��������!!!)
		OADn=(pRCSD[1]<<24)+(pRCSD[2]<<16)+(pRCSD[3]<<8)+pRCSD[4];
		switch(OADn)
		{
			case 0x40010200://ͨ�ŵ�ַ
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_octet_string;
				pout[Len+1]=6;
				MR((u32)pout+Len+2,(u32)pAddr,6);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x202A0200://Ŀ���������ַ
				if(LEN_DATAmax<9)
				{
					return 0;
				}
				pout[Len]=DataType_TSA;
				pout[Len+1]=7;
				pout[Len+2]=5;
				MR((u32)pout+Len+3,(u32)pAddr,6);
				Len+=9;
				LEN_DATAmax-=9;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x60400200://�ɼ�����ʱ��
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord,7);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x60410200://�ɼ��ɹ�ʱ��
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+7,7);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x60420200://�ɼ��洢ʱ��
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+14,7);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
		}
		//����ͬOAD�ڼ�¼�е�OAD����
		Offset=21;
		Mcsd=pRecord[Offset+1];
		Offset+=2;
		OADcount=0;//��ͬOAD�ڼ�¼�е�OAD����
		for(i=0;i<Mcsd;i++)
		{
			OADm=(pRecord[Offset+2]<<24)+(pRecord[Offset+3]<<16)+(pRecord[Offset+4]<<8)+pRecord[Offset+5];
			if(OADn==OADm)
			{
				break;
			}
			if(pRecord[Offset+1]==0)
			{//OAD
				OADcount++;
				Offset+=6;
			}
			else
			{//ROAD
				x=pRecord[Offset+6];//����OAD����
				OADcount+=x;
				x*=4;
				x+=7;
				Offset+=x;
			}
		}
		if(i>=Mcsd)
		{//û�ҵ����������CSD,��0=NULL����
			if(pRCSD[0]==0)
			{//OAD
				pRCSD+=5;
				if(LEN_DATAmax<1)
				{
					return 0;
				}
				pout[Len]=0;//NULL
				Len+=1;
				LEN_DATAmax-=1;
			}
			else
			{//ROAD
				Nroad=pRCSD[5];//SEQUENCE OF����
				pRCSD+=6+(Nroad*4);
				if(LEN_DATAmax<(2+Nroad))
				{
					return 0;
				}
				pout[Len]=DataType_array;
				pout[Len+1]=Nroad;
				for(i=0;i<Nroad;i++)
				{
					pout[Len+2+i]=0;//NULL
				}
				Len+=2+Nroad;
				LEN_DATAmax-=2+Nroad;
			}
			continue;
		}
		if(pRCSD[0]==0)
		{//OAD
			pRCSD+=5;
			x=OADcount;
			OADn=0;//����ƫ��
			while(x--)
			{
				OADm=Get_DL698DataLen_S(pData+OADn,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
				OADn+=OADm;
			}
			x=Get_DL698DataLen_S(pData+OADn,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
			if(LEN_DATAmax<x)
			{
				return 0;
			}
			MR((u32)pout+Len,(u32)pData+OADn,x);
			Len+=x;
			LEN_DATAmax-=x;
			fDATA=1;//����ѡ��OAD���ݱ�־
		}
		else
		{//ROAD
			Nroad=pRCSD[5];//SEQUENCE OF����
			pRCSD+=6;
			if(pRecord[Offset+1]==0)
			{//��¼���ݷ�ROAD
				return 0;
			}
			if(LEN_DATAmax<2)
			{
				return 0;
			}
			pout[Len]=DataType_array;
			pout[Len+1]=Nroad;
			Len+=2;
			LEN_DATAmax-=2;
			Mroad=pRecord[Offset+6];//SEQUENCE OF����
			Offset+=7;
			for(i=0;i<Nroad;i++)
			{
				OADn=(pRCSD[0]<<24)+(pRCSD[1]<<16)+(pRCSD[2]<<8)+pRCSD[3];
				for(x=0;x<Mroad;x++)
				{
					OADm=(pRecord[Offset+(4*x)+0]<<24)+(pRecord[Offset+(4*x)+1]<<16)+(pRecord[Offset+(4*x)+2]<<8)+pRecord[Offset+(4*x)+3];
					if(OADn==OADm)
					{
						x+=OADcount;
						OADn=0;//����ƫ��
						while(x--)
						{
							OADm=Get_DL698DataLen_S(pData+OADn,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
							OADn+=OADm;
						}
						x=Get_DL698DataLen_S(pData+OADn,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
						if(LEN_DATAmax<x)
						{
							return 0;
						}
						MR((u32)pout+Len,(u32)pData+OADn,x);
						Len+=x;
						LEN_DATAmax-=x;
						x=0;
						fDATA=1;//����ѡ��OAD���ݱ�־
						break;
					}
				}
				if(x>=Mroad)
				{//û�ҵ�,��0=NULL����
					if(LEN_DATAmax<1)
					{
						return 0;
					}
					pout[Len]=0;//NULL
					Len+=1;
					LEN_DATAmax-=1;
				}
				pRCSD+=4;
			}
		}
	}
	if(fDATA)//����ѡ��OAD���ݱ�־
	{
		return Len;
	}
	if(DATANULLSEL)//ȫ�����ݿ�ʱ��0=�޷���,1=�ÿ�����0
	{
		return Len;
	}
	return 0;
}
*/

u32 Get_RecordFileData_RCSD(u8 *pRCSD,u8 *pAddr,u8 *pRecord,u8 *pout,u32 LEN_DATAmax,u32 DATANULLSEL)//��¼�����ѡ��;���:pRCSDָ����յ�RCSD(����������),pAddr,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
{
	u32 i;
	u32 x;
	u32 Offset;
	u32 Len;
	u32 Nrcsd;
	u32 Nroad;
	u32 OADrcsd;
	u32 OADroad;
	u32 Mcsd;
	u32 Mroad;
	u32 OAD;
	u32 OADcount;
	u32 OADoffset;
	u8 *pData;
	u32 fDATA;//����ѡ��OAD���ݱ�־
	u8* p8;
	
	fDATA=0;//����ѡ��OAD���ݱ�־
	//�����¼���ݿ�ʼָ��
	Offset=21;
	Mcsd=pRecord[Offset+1];
	Offset+=2;
	for(i=0;i<Mcsd;i++)
	{
		if(pRecord[Offset+1]==0)
		{//OAD
			Offset+=6;
		}
		else
		{//ROAD
			x=pRecord[Offset+6];//����OAD����
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=(u8*)pRecord+Offset;
	//
	Nrcsd=pRCSD[0];//RCSD��=SEQUENCE OF CSD ����һ��OADʱ��RCSD=0����SEQUENCE OF�����������Ϊ0����ʾ����ѡ�񣨼�ȫѡ����
	if(Nrcsd==0)
	{
		Nrcsd=Mcsd;
		Offset-=21+2;
		pRCSD=(u8*)ADDR_1kDATABUFF;
		pRCSD[0]=Nrcsd;//RCSD��SEQUENCE OF����
		if(Offset>(LEN_1kDATABUFF-1))
		{
			Offset=(LEN_1kDATABUFF-1);
		}
		MR((u32)pRCSD+1,(u32)pRecord+21+2,Offset);//���ջ�����ǰ,5*m���=5*256����Ҳֻ�ܳ������ջ����Ƥ�뷢���������ᳬ��������
		//ȥ���ݴ���ʱ������ǰ����
		pRCSD++;
		Len=0;
		Offset=0;
		for(i=0;i<Mcsd;i++)
		{
			if(pRCSD[Offset+1]==0)
			{//OAD
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,5);
				Len+=5;
				Offset+=6;
			}
			else
			{//ROAD
				x=pRecord[Offset+6];//����OAD����
				x*=4;
				x+=7;
				MR((u32)pRCSD+Len,(u32)pRCSD+Offset+1,x-1);
				Len+=x-1;
				Offset+=x;
			}
		}
	}
	Len=0;
	pRCSD++;
//���ܱ����ݿ���,���Ȱ�ʵ��д����
//0 7byte �ɼ�����ʱ��  date_time_s(����������)
//7 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//14 7byte �ɼ��洢ʱ��  date_time_s(����������)
//21  �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������)(ROADʱ��д���ܹ�����OAD��������!!!)
	while(Nrcsd--)
	{
		OADrcsd=(pRCSD[1]<<24)+(pRCSD[2]<<16)+(pRCSD[3]<<8)+pRCSD[4];
		switch(OADrcsd)
		{
			case 0x40010200://ͨ�ŵ�ַ
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_octet_string;
				pout[Len+1]=6;
				MR((u32)pout+Len+2,(u32)pAddr,6);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x202A0200://Ŀ���������ַ
				if(LEN_DATAmax<9)
				{
					return 0;
				}
				pout[Len]=DataType_TSA;
				pout[Len+1]=7;
				pout[Len+2]=5;
				MR((u32)pout+Len+3,(u32)pAddr,6);
				Len+=9;
				LEN_DATAmax-=9;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x60400200://�ɼ�����ʱ��
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord,7);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x60410200://�ɼ��ɹ�ʱ��
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+7,7);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
			case 0x60420200://�ɼ��洢ʱ��
				if(LEN_DATAmax<8)
				{
					return 0;
				}
				pout[Len]=DataType_date_time_s;
				MR((u32)pout+Len+1,(u32)pRecord+14,7);
				Len+=8;
				LEN_DATAmax-=8;
				if(pRCSD[0]==0)
				{
					pRCSD+=5;
				}
				else
				{
					return 0;//CSD����
				}
				continue;
		}
		//�ڼ�¼������ͬOAD����
		if(pRCSD[0]==0)
		{//OAD
			Offset=21;
			Mcsd=pRecord[Offset+1];
			Offset+=2;
			OADcount=0;//��ͬOAD�ڼ�¼�е�OAD����
			for(i=0;i<Mcsd;i++)
			{
				OAD=(pRecord[Offset+2]<<24)+(pRecord[Offset+3]<<16)+(pRecord[Offset+4]<<8)+pRecord[Offset+5];
				if((OADrcsd>>8)==(OAD>>8))
				{
					pRCSD+=5;
					x=OADcount;
					OADoffset=0;//����ƫ��
					while(x--)
					{
						OADoffset+=Get_DL698DataLen_S(pData+OADoffset,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					}
					p8=pData+OADoffset;
					if(((OAD&0xff)==0)&&((OADrcsd&0xff)!=0))
					{//����ʱȫ��Ԫ��,ȡ���е�Ԫ��
						p8=Get_Element(p8,OADrcsd&0xff,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
						if(p8==0)
						{//û�ҵ�,��0=NULL����
							if(LEN_DATAmax<1)
							{
								return 0;
							}
							pout[Len]=0;//NULL
							Len+=1;
							LEN_DATAmax-=1;
							break;
						}
					}
					else
					{
						if(OADrcsd!=OAD)
						{
							continue;
						}
					}
					x=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
					if(LEN_DATAmax<x)
					{
						return 0;
					}
					MR((u32)pout+Len,(u32)p8,x);
					Len+=x;
					LEN_DATAmax-=x;
					fDATA=1;//����ѡ��OAD���ݱ�־
					break;
				}
				if(pRecord[Offset+1]==0)
				{//OAD
					OADcount++;
					Offset+=6;
				}
				else
				{//ROAD
					x=pRecord[Offset+6];//����OAD����
					OADcount+=x;
					x*=4;
					x+=7;
					Offset+=x;
				}
			}
			if(i>=Mcsd)
			{//û�ҵ����������CSD,��0=NULL����
				pRCSD+=5;
				if(LEN_DATAmax<1)
				{
					return 0;
				}
				pout[Len]=0;//NULL
				Len+=1;
				LEN_DATAmax-=1;
			}
		}
		else
		{//ROAD
		
			Nroad=pRCSD[5];//SEQUENCE OF����
			pRCSD+=6;
			if(LEN_DATAmax<2)
			{
				return 0;
			}
			pout[Len]=DataType_array;
			pout[Len+1]=Nroad;
			Len+=2;
			LEN_DATAmax-=2;
			while(Nroad--)
			{
				OADroad=(pRCSD[0]<<24)+(pRCSD[1]<<16)+(pRCSD[2]<<8)+pRCSD[3];
				pRCSD+=4;
				Offset=21;
				Mcsd=pRecord[Offset+1];
				Offset+=2;
				OADcount=0;//��ͬOAD�ڼ�¼�е�OAD����
				for(i=0;i<Mcsd;i++)
				{
					if(pRecord[Offset+1]==0)
					{//OAD
						OADcount++;
						Offset+=6;
						continue;
					}
					else
					{//ROAD
						OAD=(pRecord[Offset+2]<<24)+(pRecord[Offset+3]<<16)+(pRecord[Offset+4]<<8)+pRecord[Offset+5];
						if(OADrcsd!=OAD)
						{
							x=pRecord[Offset+6];//����OAD����
							OADcount+=x;
							x*=4;
							x+=7;
							Offset+=x;
							continue;
						}
						Mroad=pRecord[Offset+6];//����OAD����
						Offset+=7;
						for(x=0;x<Mroad;x++)
						{
							OAD=(pRecord[Offset+0]<<24)+(pRecord[Offset+1]<<16)+(pRecord[Offset+2]<<8)+pRecord[Offset+3];
							if((OADroad>>8)==(OAD>>8))
							{
								if(((OAD&0xff)==0)&&((OADroad&0xff)!=0))
								{//����ʱȫ��Ԫ��,ȡ���е�Ԫ��
									
								}
								else
								{
									if(OADroad!=OAD)
									{
										OADcount++;
										Offset+=4;
										continue;
									}
								}
								break;
							}
							OADcount++;
							Offset+=4;
						}
						if(x<Mroad)
						{//�ҵ�
							x=OADcount;
							OADoffset=0;//����ƫ��
							while(x--)
							{
								OADoffset+=Get_DL698DataLen_S(pData+OADoffset,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
							}
							p8=pData+OADoffset;
							if(((OAD&0xff)==0)&&((OADroad&0xff)!=0))
							{//����ʱȫ��Ԫ��,ȡ���е�Ԫ��
								p8=Get_Element(p8,OADroad&0xff,0,0);//����Ԫ�ص�ַ(�����ַָ��),ʹ��ADDR_128KDATABUFF;����:��ַָ��Ԫ�ص������ֽ�,0=����
								if(p8==0)
								{//û�ҵ�,��0=NULL����
									if(LEN_DATAmax<1)
									{
										return 0;
									}
									pout[Len]=0;//NULL
									Len+=1;
									LEN_DATAmax-=1;
									break;
								}
							}
							x=Get_DL698DataLen_S(p8,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
							if(LEN_DATAmax<x)
							{
								return 0;
							}
							MR((u32)pout+Len,(u32)p8,x);
							Len+=x;
							LEN_DATAmax-=x;
							fDATA=1;//����ѡ��OAD���ݱ�־
							break;
						}
					}
					
				}
				if(i>=Mcsd)
				{//û�ҵ����������CSD,��0=NULL����
					if(LEN_DATAmax<1)
					{
						return 0;
					}
					pout[Len]=0;//NULL
					Len+=1;
					LEN_DATAmax-=1;
				}
			}
		}
	}
	if(fDATA)//����ѡ��OAD���ݱ�־
	{
		return Len;
	}
	if(DATANULLSEL)//ȫ�����ݿ�ʱ��0=�޷���,1=�ÿ�����0
	{
		return Len;
	}
	return 0;
}





