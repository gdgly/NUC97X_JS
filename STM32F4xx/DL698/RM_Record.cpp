
//��ͨ�ɼ�������¼�ļ�
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/RM_Record.h"
#include "../DL698/RM_TASK.h"
#include "../MS/msFILE.h"
#include "../MS/MS.h"
#include "../Device/MEMRW.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "../Calculate/Calculate.h"

#include "../Terminal698/Terminal_PowerDown.h"
#include "../DL698/EVENT_Record.h"
#include "../DL698/EVENT_CALC.h"
#include "../DL698/DL698_REPORT.h"
#include "../DL698/CALC_TAG.h"

#include "../DL698/DL698_JSON.h"


//��������:1=��ͨ�ɼ�����,3=͸������  �ļ��洢��ʽ
//���ܱ��ַ�����ݿ鳤���б���,�б������ȹ̶� 8*NUM_RMmax byte
//6byte ��1�����ܱ��ַ
//2byte ��1�����ܱ����ݿ鳤��
//...
//6byte ��NUM_RMmax�����ܱ��ַ
//2byte ��NUM_RMmax�����ܱ����ݿ鳤��

//���ܱ����ݿ���,���Ȱ�ʵ��д����
//0 7byte �ɼ�����ʱ��  date_time_s(����������)
//7 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//14 7byte �ɼ��洢ʱ��  date_time_s(����������)
//21  �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������)


//��������:2=�¼��ɼ�����  �ļ��洢��ʽ

void Find_And_CreateNextData(u8 *pout,u32 Len,u32 FileID,u32 YYMD,u16 *pAddr)
{
	u8 *p8;
	u8 *pda;
	u8 OADNUM1;
	u8 OADNUM2;
	u32 i;
	u32 n;
	u32 offset;
	u32 filename;
	u32 datalen;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//ӭҭ݇¼τݾͷτݾ
	ms=(ms_Type *)Get_ms();
	if(0x13f16000==FileID)
	{
		FileID=0x13f11000;
	}else if(0x13f18000==FileID)
	{
		FileID=0x13f13000;
	}
	filename=Get_RecordFileName_JS(FileID,0xFFFFFFFF,0);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
	if(filename==0xffffffff)
	{
		return;
	}
	if(filename<FILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
	}
	else
	{
		//file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		return ;
	}
	if(file->BLOCK_ENTRY==0)
	{
		return;
	}
	//ʱݤքɸѡ
	//YYMD=YYMD_Sub1D_hex(YYMD);//ŪŪՂɕݵ1ɕ,׵ܘݵ1۳քŪŪՂɕ
	i=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
	if(YYMD!=i)
	{
		return;
	}
	//ɧڻ֒սԐ˽ߝ,Јёԭ4ք˽ߝ߽Ѵս­΢һٶBUFFoĦ
	if(Len>4096)
	{
		return;
	}
	//����һ��ʼ�����ݵ���ʱ�ռ�����
	MW((u32)pout,(u32)ADDR_DATABUFF,Len);
	n=file->NUM_DATA;
	if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
	{
		n=0;
	}
	ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
	offset=0;
	RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
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
	if(i>=n)
	{//û�ҵ�
		MW((u32)ADDR_DATABUFF,(u32)pout,Len);
		return;
	}
	i=RecordFileHead->LEN_DATA;
	if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
	{
		return;
	}
	ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);
	p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	
	datalen=i;
	//ЂĦߪʼۏҢ˽ߝЮ
	pda=(u8 *)ADDR_DATABUFF;//Ԑ}ٶŚɝ,ۏҢԉһٶ698ք֡
	if((datalen+Len)>4096)
	{
		MW((u32)pda,(u32)p8,Len);
		return;
	}
	//ётք˽ߝЮۏ՚Oք˽ߝЮ۳Ħ
	
	MW((u32)p8,(u32)pda+Len,datalen);//�������ݳ�����������
	
	p8=(u8 *)pda+Len;//p8ָв۳Ħһٶ˽ߝЮ
	//Јё21ٶؖޚքʱҪ߽Ѵս
	offset=0;
	//Јё21ٶʱݤڽËԶȥ
	MW((u32)pda,(u32)pout,21);
	pda+=21;//û
	p8+=21;
	offset=21;
	Len-=21;
	datalen-=21;
	//6ٶؖޚѼ(ROAD
	MW((u32)pda,(u32)pout+offset,8);
	offset+=8;
	pda+=8;
	p8+=8;
	Len-=8;
	datalen-=8;
	//˽ߝЮքٶ˽
	i=pda[0];
	OADNUM1=i;
	n=p8[0];
	OADNUM2=n;
	if((i+n)>0)//˽ߝЮٶ˽
	{
		pout[offset++]=i+n-1;//
	}
	Len--;
	datalen--;
	pda++;
	//ǰĦһٶ˽ߝЮք˽ߝ߽Ѵսpout
	MW((u32)pda,(u32)pout+offset,i*4);
	pda+=(i*4);
	offset+=(i*4);
	Len-=(i*4);
	p8++;
	if(n>0)
	{
		p8+=4;//ڽËִ˽ߝֳޡʱݤ֢ٶ
		MW((u32)p8,(u32)pout+offset,(n-1)*4);
		p8+=((n-1))*4;
		offset+=((n-1)*4);
		datalen-=(n*4);
	}

	//����������ĳ���
	datalen=0;
	for(i=0;i<OADNUM1;i++)
	{
		n=Get_DL698DataLen_S(pda+datalen,0);//�����һ�����ݵĳ���
		datalen+=n;
	}
	MW((u32)pda,(u32)pout+offset,datalen);
	offset+=datalen;

	p8+=8;//ڽËֳִޡʱݤ˽ߝЮ
	datalen-=8;
	//����������ĳ���
	datalen=0;
	for(i=0;i<OADNUM2;i++)
	{
		n=Get_DL698DataLen_S(p8+datalen,0);//�����һ�����ݵĳ���
		datalen+=n;
	}
	MW((u32)p8,(u32)pout+offset,datalen);
	
	offset+=datalen;
	//for(i=0;i<offset;i++)
		//myprintf("%02x ", pout[i], 0, 0);
	//myprintf("\n",0,0,0);
}

u32 Get_RecordFileData_Fixed(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//�õ���¼�ļ�����;���:pRSD��pRCSDָ����յ�RSD��RCSD(����������)��=0��ʾ����ע;����:���������ݳ���,0=������;ʹ��ADDR_128KDATABUFF
{
	u32 i;
	u32 n;
	u32 offset;
	u32 filename;
	u32 YYMD;
	u32 YYMD1;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//�����¼�ļ�ͷ�ļ�
	u8* p8;
	YYMD=0;
	YYMD1=0;
	ms=(ms_Type *)Get_ms();
#if(USER/100)==17//����ϵ
	if(Terminal_Router->MinuteCollect==1)
	//if(1)
		filename=Get_RecordFileName_JS(FileID,FileCount,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	else
		filename=Get_RecordFileName(FileID,FileCount,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	#else
		filename=Get_RecordFileName(FileID,FileCount,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0,��Ӧ��ֵΪ0xF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;SearchCount=��ʼ�������ļ���0xFFFF��ʾ���ϴν����ſ�ʼ����;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
	#endif	
	if(filename==0xffffffff)
	{
		return 0;
	}
	if(filename<FILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
	}
	else
	{
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	if(file->BLOCK_ENTRY==0)
	{
		return 0;
	}
	switch(pRSD[0])
	{
		case 0:
//��ѡ��       [0]  NULL
			break;
		case 1:
//Selector1��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ֵ            Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://���ݶ���ʱ��
					pRSD+=4;
					if(pRSD[0]!=DataType_date_time_s)
					{//��ֵData=��
						return 0;
					}
					pRSD+=1;
					//��ֵData=date_time_s
					for(i=0;i<6;i++)
					{//�벻�Ƚ�
						if(pRSD[i]!=file->save_date_time_s[i])
						{
							return 0;
						}
					}
					break;
				//case 0x40000200://����ʱ��
				default:
					return 0;
			}
			break;
		case 2:
//Selector2��=SEQUENCE
//{
//  ��������������  OAD��
//  ��ʼֵ          Data��
//  ����ֵ          Data��
//  ���ݼ��        Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://���ݶ���ʱ��
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=0)
					{//����ͬ
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=2)
					{//ʱ��>=����ֵ
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 3:
//Selector3��=SEQUENCE OF Selector2
			pRSD+=2;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://���ݶ���ʱ��
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=0)
					{//����ͬ
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=2)
					{//ʱ��>=����ֵ
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 4:
//Selector4��=SEQUENCE
//{
//  �ɼ�����ʱ��  date_time_s��
//  ���ܱ���    MS
//}
			if((file->FILEID&0xff)!=0)
			{//�ļ�����!=0
				return 0;
			}
			//ȡ7byte �ɼ�����ʱ��  date_time_s(����������)
//			ms->msfile_read(filename,NUM_RMmax*sizeof(RecordFileHead_TypeDef),(u8*)ADDR_DATABUFF,7);
//			p8=(u8*)ADDR_DATABUFF;
//			pRSD++;
//			for(i=0;i<6;i++)//�벻�Ƚ�
//			{
//				if(pRSD[i]!=p8[i])
//				{
//					return 0;
//				}
//			}
			pRSD++;
			for(i=0;i<6;i++)//�벻�Ƚ�
			{
				if(pRSD[i]!=file->start_date_time_s[i])
				{
					return 0;
				}
			}
			break;
		case 5:
//Selector5��=SEQUENCE
//{
//  �ɼ��洢ʱ��  date_time_s��
//  ���ܱ���    MS
//}
#if (USER/100==17)	
			if(((FileID>>24)!=0x1E)&&(Terminal_Router->MinuteCollect==1))//�¶�������
			{
				pRSD++;
				i=(pRSD[0]<<16)|(pRSD[1]<<24)|(pRSD[2]<<8)|(pRSD[3]);
				i=YYMD_Sub1D_hex(i);//�������ռ�1��,���ؼ�1�����������
				n=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
				if(i!=n)
				{
					return 0;
				}
			}
			else
			{
				pRSD++;
				for(i=0;i<6;i++)//�벻�Ƚ�
				{
					if(pRSD[i]!=file->save_date_time_s[i])
					{
						return 0;
					}
				}
			}
#else
			pRSD++;
			for(i=0;i<6;i++)//�벻�Ƚ�
			{
				if(pRSD[i]!=file->save_date_time_s[i])
				{
					return 0;
				}
			}
#endif
			break;
		case 6:
//Selector6��=SEQUENCE
//{
//  �ɼ�����ʱ����ʼֵ  date_time_s��
//  �ɼ�����ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			if((file->FILEID&0xff)!=0)
			{//�ļ�����!=0
				return 0;
			}
			//ȡ7byte �ɼ�����ʱ��  date_time_s(����������)
//			ms->msfile_read(filename,NUM_RMmax*sizeof(RecordFileHead_TypeDef),(u8*)ADDR_DATABUFF,7);
//			p8=(u8*)ADDR_DATABUFF;
//			pRSD++;
//			i=Compare_DL698DataString(p8,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//			if(i!=0)
//			{//����ͬ
//				return 0;
//			}
//			pRSD+=7;
//			i=Compare_DL698DataString(p8,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//			if(i!=2)
//			{//ʱ��>=����ֵ
//				return 0;
//			}
			pRSD++;
			i=Compare_DL698DataString(file->start_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i!=0)
			{//����ͬ
				return 0;
			}
			pRSD+=7;
			i=Compare_DL698DataString(file->start_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i!=2)
			{//ʱ��>=����ֵ
				return 0;
			}
			break;
		case 7:
//Selector7��=SEQUENCE
//{
//  �ɼ��洢ʱ����ʼֵ  date_time_s��
//  �ɼ��洢ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
//		#if (USER%100)!=0//���ǹ�̨�����
			//if(((FileID>>20)==0x136)||((FileID>>20)==0x101))
			if(((FileID>>24)==0x13)||((FileID>>24)==0x10))
			{//���߻�ʵʱ����
				pRSD++;
				
			#if(USER/100)==17//
			if(Terminal_Router->MinuteCollect==1)
			//if(1)
			{
				YYMD=(pRSD[0]<<16)|(pRSD[1]<<24)|(pRSD[2]<<8)|(pRSD[3]);
				YYMD1=YYMD;
				//i=YYMD_Sub1D_hex(YYMD);//�������ռ�1��,���ؼ�1�����������
				n=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
				if(n<YYMD)
				{				
					return 0;
				}
				if(n==YYMD)
				{
					YYMD=(pRSD[4]<<8)|(pRSD[5]);
					n=(file->save_date_time_s[4]<<8)|(file->save_date_time_s[5]);
					if(n<YYMD)
					{
						return 0;
					}
				}
				//�����ٲ��ն����Ҫ��
				pRSD+=7;
				YYMD=(pRSD[0]<<16)|(pRSD[1]<<24)|(pRSD[2]<<8)|(pRSD[3]);
				n=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
				if(n>YYMD)
				{
					return 0;
				}
				if(n==YYMD)
				{
					YYMD=(pRSD[4]<<8)|(pRSD[5]);
					n=(file->save_date_time_s[4]<<8)|(file->save_date_time_s[5]);
					if(n>=YYMD)
					{
						return 0;
					}
				}
			}else
			{
				i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//?????(??????),p1?p2??RAM?,??:0=??,1=???p1>p2,2=???p1<p2
				if(i!=0)
				{
					pYYMDHMS_AddS_hex(pRSD,60);
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//?????(??????),p1?p2??RAM?,??:0=??,1=???p1>p2,2=???p1<p2
					pYYMDHMS_SubS_hex(pRSD,60);
					if(i!=0)
					{//
						return 0;
					}
				}
//				i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//?????(??????),p1?p2??RAM?,??:0=??,1=???p1>p2,2=???p1<p2
//				if(i!=0)
//				{
//					pYYMDHMS_AddS_hex(pRSD,3600);
//					i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//?????(??????),p1?p2??RAM?,??:0=??,1=???p1>p2,2=???p1<p2
//					pYYMDHMS_SubS_hex(pRSD,3600);
//					if(i!=0)
//					{//
//						return 0;
//					}
//				}
				
			#else
				i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//?????(??????),p1?p2??RAM?,??:0=??,1=???p1>p2,2=???p1<p2
			#endif
				if(i!=0)
				{//����ͬ
					return 0;
				}
				pRSD+=7;
				i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(i!=2)
				{//ʱ��>=����ֵ
					return 0;
				}
			}
			}
			else
			{//�ջ��¶���
				//if((FileID>>20)==0x123)
				if((FileID>>24)==0x12)
				{//�ն���
					pRSD++;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,4);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=0)
					{//�����ղ�ͬ
						return 0;
					}
				}
			#if(USER/100)==17//����ϵ
				else if((FileID>>24)==0x1F)//�����ն���
				{
					pRSD++;
					i=(pRSD[0]<<16)|(pRSD[1]<<24)|(pRSD[2]<<8)|(pRSD[3]);
					i=YYMD_Add1D_hex(i);//�������ռ�1��,���ؼ�1�����������
					n=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
					if(i!=n)
					{
						return 0;
					}
				}
			#endif
				else
				{//�¶���
					pRSD++;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,3);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
					if(i!=0)
					{//���²�ͬ
						return 0;
					}
				}
			}
//		#else
//			pRSD++;
//			i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//			if(i!=0)
//			{//����ͬ
//				return 0;
//			}
//			pRSD+=7;
//			i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//			if(i!=2)
//			{//ʱ��>=����ֵ
//				return 0;
//			}
//		#endif
			break;
		case 8:
//Selector8��=SEQUENCE
//{
//  �ɼ��ɹ�ʱ����ʼֵ  date_time_s��
//  �ɼ��ɹ�ʱ�����ֵ  date_time_s��
//  ʱ����            TI��
//  ���ܱ���          MS
//}
			if((file->FILEID&0xff)!=0)
			{//�ļ�����!=0
				return 0;
			}
			//ȡ7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//			ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+7,(u8*)ADDR_DATABUFF,7);
//			p8=(u8*)ADDR_DATABUFF;
//			pRSD++;
//			i=(pRSD[7+7+1]<<8)+pRSD[7+7+2];
//			if(i==0)
//			{//����:TI=0;����һ���Բ���ʱ�����ٷ���1������
//				i=Compare_DL698DataString(p8,pRSD,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//				if(i==2)
//				{//�ɼ��ɹ�ʱ��<��ʼֵ
//					return 0;
//				}
//				i=Compare_DL698DataString(p8,pRSD+7,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//				if(i!=2)
//				{//�ɼ��ɹ�ʱ��>=����ֵ
//					return 0;
//				}
//				break;
//			}
//			switch(pRSD[7+7])
//			{
//				case 0://  ��      ��0����
//					n=7;
//					break;
//				case 1://��      ��1����
//					n=6;
//					break;
//				case 2://ʱ      ��2����
//					n=5;
//					break;
//				case 3://��      ��3����
//					n=4;
//					break;
//				case 4://��      ��4����
//					n=3;
//					break;
//				case 5://��      ��5��
//					n=2;
//					break;
//				default:
//					return 0;
//			}
//			i=Compare_DL698DataString(p8,pRSD,n);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//			if(i!=0)
//			{//����ͬ
//				return 0;
//			}
//			i=Compare_DL698DataString(p8,pRSD+7,n);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
//			if(i!=2)
//			{//�ɼ��ɹ�ʱ��>=����ֵ
//				return 0;
//			}
//			break;

			n=file->NUM_DATA;
			if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
			{
				n=0;
			}
			ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
			offset=0;
			RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
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
			if(i>=n)
			{//û�ҵ�
				return 0;
			}
			i=RecordFileHead->LEN_DATA;
			if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
			{
				return 0;
			}
			ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);
			p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+7;//7byte �ɼ��ɹ�ʱ��
		
			pRSD++;
			i=(pRSD[7+7+1]<<8)+pRSD[7+7+2];
			if(i==0)
			{//����:TI=0;����һ���Բ���ʱ�����ٷ���1������
				i=Compare_DL698DataString(p8,pRSD,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(i==2)
				{//�ɼ��ɹ�ʱ��<��ʼֵ
					return 0;
				}
				i=Compare_DL698DataString(p8,pRSD+7,7);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
				if(i!=2)
				{//�ɼ��ɹ�ʱ��>=����ֵ
					return 0;
				}
				i=Get_RecordFileData_RCSD(pRCSD,(u8*)&RecordFileHead->Addr0,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pTSA,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
				return i;
			}
			switch(pRSD[7+7])
			{
				case 0://  ��      ��0����
					n=7;
					break;
				case 1://��      ��1����
					n=6;
					break;
				case 2://ʱ      ��2����
					n=5;
					break;
				case 3://��      ��3����
					n=4;
					break;
				case 4://��      ��4����
					n=3;
					break;
				case 5://��      ��5��
					n=2;
					break;
				default:
					return 0;
			}
			i=Compare_DL698DataString(p8,pRSD,n);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i!=0)
			{//����ͬ
				return 0;
			}
			i=Compare_DL698DataString(p8,pRSD+7,n);//���ݴ��Ƚ�(��λ�ڵ��ֽ�),p1��p2����RAM��,����:0=��ͬ,1=����ͬp1>p2,2=����ͬp1<p2
			if(i!=2)
			{//�ɼ��ɹ�ʱ��>=����ֵ
				return 0;
			}
			i=Get_RecordFileData_RCSD(pRCSD,(u8*)&RecordFileHead->Addr0,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pTSA,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
			return i;
			
		case 9:
//Selector9��=SEQUENCE
//{
//  �ϵ�n�μ�¼  unsigned
//}
			return 0;
		case 10:
//Selector10��=SEQUENCE
//{
//��n����¼  unsigned��
//���ܱ���  MS
//}
			break;//��������RCSDtoID��ֻҪ�ҵ��ļ�����Ч
		default:
			return 0;
	}
		
	n=file->NUM_DATA;
	if((n*sizeof(RecordFileHead_TypeDef))>(LEN_128KDATABUFF-2))
	{
		n=0;
	}
	ms->msfile_read(filename,0,(u8*)ADDR_128KDATABUFF+2,n*sizeof(RecordFileHead_TypeDef));
	offset=0;
	RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_128KDATABUFF+2);
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
	if(i>=n)
	{//û�ҵ�
		return 0;
	}
	i=RecordFileHead->LEN_DATA;
	if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
	{
		return 0;
	}
	ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);
	if(YYMD1&&Terminal_Router->MinuteCollect==1)
	//if(YYMD1)//���ջ��õ��������
	{
		p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
		#ifdef DEBUG_JSON
		for(i=0;i<600;i++)
			myprintf("%02x ", p8[i], 0, 0);
		myprintf("\n",0,0,0);
		#endif
		Find_And_CreateNextData((u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i,FileID,YYMD1,pAddr);	
		i=Get_RecordFileData_RCSD(pRCSD,(u8*)pAddr,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pTSA,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
		return i;
	}
	i=Get_RecordFileData_RCSD(pRCSD,(u8*)&RecordFileHead->Addr0,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//��¼�����ѡ��;���:pRCSDָ����ѡ���RCSD(����������),pTSA,pRecordָ��ÿֻ���¼��ַ�����ݿ鿪ʼ,p8outָ�����ݷ��ͻ���;����;��¼ѡ�����ݵĳ���,0=û��¼�����
	return i;
}

u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//փս݇¼τݾ˽ߝ;ɫ�?pRSDbpRCSDָвޓ˕քRSDbRCSD(Ϟ˽ߝ`э)ì=0ҭʾһژע;׵ܘ:ׁԶք˽ߝӤ׈,0=Ϟ˽ߝ;ʹԃADDR_128KDATABUFF
{
	u32 LEN;

	WWDT_Enable_Feed(WDTTimerOutVal);//Պѭߴąٷۍιٷ
	Comm_Ram->msFILESearchCount=0;
	if(Terminal_Router->MinuteCollect==1)
	//if(1)
	{
		while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
		{
			LEN=Get_RecordFileData_Fixed(FileID,FileCount,pAddr,pRSD,pRCSD,pOUT,LEN_OUTmax);//փս݇¼τݾ˽ߝ
			if(LEN)
			{
				return LEN;
			}
		}
	}else
	{
		while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
		{
			LEN=Get_RecordFileData_Fixed(FileID,FileCount,pAddr,pRSD,pRCSD,pOUT,LEN_OUTmax);//փս݇¼τݾ˽ߝ
			if(LEN)
			{
				return LEN;
			}
		}
	}	
	
	return 0;
}



u32 DEPTH_RecordFile(u32 PORTn)//��¼�ļ��洢���;����:�洢���
{
	u32 no;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	no=UARTCtrl->TaskID&0xff;//������
	switch((UARTCtrl->TaskID>>24)&0xf)//��������
	{
		case 1://��ͨ�ɼ����� ��1��
			if(no>=NUMmax_6014)
			{
				no=0;
			}
			p8=(u8*)ADDR_6014_SDRAM+(no*LENper_6014);
			p8=Get_Element(p8,2,0,LENper_6014);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8)
			{
				return (p8[1]<<8)+p8[2];
			}
			return 0;
		case 2://�¼��ɼ����� ��2��
			if(no>=NUMmax_6016)
			{
				no=0;
			}
			p8=(u8*)ADDR_6016_SDRAM+(no*LENper_6016);
			p8=Get_Element(p8,5,0,LENper_6016);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8)
			{
				return (p8[1]<<8)+p8[2];
			}
			return 0;
		case 3://͸������     ��3��
			if(no>=NUMmax_6018)
			{
				no=0;
			}
			p8=(u8*)ADDR_6018+(no*LENper_6018);
			p8=Get_Element(p8,4,0,LENper_6018);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8)
			{
				return (p8[1]<<8)+p8[2];
			}
			return 0;
		case 4://�ϱ�����     ��4��
		case 5://�ű�����     ��5��
			return 0;
		default:
			return 0;
	}
}

void CountInc_RecordFile(u32 PORTn)//�����ļ�����=0ʱ,ȫ����¼�ļ��ļ�����+1,ɾ���ﵽ��¼��ȵ��ļ�
{
	u32 i;
	u32 FileID;
	u32 set_depth;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
	FileID=Get_FileID(PORTn);//UART�������FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
	ms=(ms_Type *)Get_ms();
	set_depth=DEPTH_RecordFile(PORTn);//��¼�ļ��洢���;���:TaskID=UARTCtrl->TaskID;����:�洢���
	if(set_depth<=1)
	{
		set_depth=2;//��Сֵ
	}
	if(set_depth>0xfffe)
	{
		set_depth=0xfffe;//���ֵ
	}
	set_depth--;
	Comm_Ram->msFILESearchCount=0;
	while(1)
	{
		filename=Get_RecordFileName(FileID,0,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
		if(filename==0xffffffff)
		{//û�ҵ�
			return;
		}
		if(filename<recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		if(file->FILECOUNT==0)
		{
			break;
		}
	}
	//�м���=0���ļ�
	Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
	Comm_Ram->msFILESearchCount=0;
	while(1)
	{
		filename=Get_RecordFileName(FileID,0xffffffff,0);//�����ļ���:���:FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����,��Ӧ��ֵΪ0xFF��ʾ����ע,FileCount�ļ�����=0xFFFFFFFF��ʾ����ע,pTime���ݴ洢ʱ��=0��ʾ����ע;����:0--(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)��ʾ�ļ���RAM,0xffffffff��ʾû�ҵ�
		if(filename==0xffffffff)
		{//û�ҵ�
			return;
		}
		if(filename<recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-recordFILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		if(file->FILECOUNT>=set_depth)
		{
			i=filename;
			if(i>=recordFILENAMEmax_FLASH)
			{
				i-=recordFILENAMEmax_FLASH;
				i+=FILENAMEmax_FLASH;
			}
			ms->msfile_Delete(i);
		}
		else
		{
			file->FILECOUNT++;
		}
	}
}

u32 AutoDelete_ErrID_RecordFile(void)//�Զ�ɾ�������Ѳ����ڵļ�¼�ļ�;����:0=ûɾ��,1=��ɾ��
{
	u32 i;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 FileID;
	u32 Delete;
	u8* p8;
	u32 id;
	
	Delete=0;
	ms=Get_ms();
	
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM);filename++)
	{
		if(filename==recordFILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		}
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			switch(FileID&0xff)//�ļ�����
			{
				case 1://1.���ݶ���
				case 2://2.�¼���¼
				case 3://3.�����ѱ���
				case 4://4.��̨���ѱ���
				case 5://5.DL645���ܱ������ϱ�
				default:
					file++;
					continue;
				case 0://��ͨ�ɼ�������¼,ȫ�¼��ɼ�������¼
					i=FileID>>28;
					if(i==1)
					{//1=��ͨ�ɼ�����
						FileID>>=12;
						for(id=0;id<NUMmax_6012_2;id++)
						{
							i=IDtoFileID(id);//���:�����id,����:�ļ��洢FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����;��Чid����=0
							i>>=12;
							if(i==FileID)
							{
								break;
							}
						}
						if(id>=NUMmax_6012_2)
						{//û�������
							i=filename;
							if(i>=recordFILENAMEmax_FLASH)
							{
								i-=recordFILENAMEmax_FLASH;
								i+=FILENAMEmax_FLASH;
							}
							ms->msfile_Delete(i);
							Delete=1;
						}
					}
					else
					{//ȫ�¼��ɼ�������¼,��������
					#if FileID_2xx==0//0=��,1=��;��������_�ɼ�����_�洢ʱ������=2xx(�¼��ɼ�����)�ļ���ʶ��������ID��
						#error
					#endif
//#define MeterAutoReportTaskID     0x02300000;//��������ϱ�Լ������ID:b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//FILEID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
						if(FileID==MeterAutoReportFileID_DL645)
						{
							break;
						}
						if(FileID==MeterAutoReportFileID_DL698)
						{
							break;
						}
						if(FileID==MeterAutoReportFileID_3011)
						{
							break;
						}
						FileID>>=12;
						FileID&=0xff;//�����
						p8=(u8*)ADDR_6012_2_SDRAM+(FileID*LENper_6012_2);
						if(p8[0]!=DataType_structure)
						{
							i=filename;
							if(i>=recordFILENAMEmax_FLASH)
							{
								i-=recordFILENAMEmax_FLASH;
								i+=FILENAMEmax_FLASH;
							}
							ms->msfile_Delete(i);
							Delete=1;
						}
					}
					break;
			}
		}
		file++;
	}
	return Delete;
}

//=================
//��������,1����,15�����������ݾͱ���һ��,�ն��ᱣ��3��,�¶���2����

//һ����1683��ʱ��� 1����1440,15����96,�ն���3,�¶���2
//1440+96+3+2=1541	//һ��һ����Ҫ��ô����㱣����Ӷ��� //��ΪҪ���ڶ����� �ټ�70���� 1541+70=1611
//1683-1611 =72    //һ��ʣ����ô�����
//=================
u32 AutoDeleteFile_SeletTime(u32 FileID,u8 *ptime)
{
	u32 i;
	u32 ret;

	u32 YYMD1;
	u32 YYMD2;

	ret=0;
	i=Check_pYYMDHMS_hex(JSON->Lastrealtime);
	if(i)
	{
		return ret;
	}
	//����FileID��ɾ����ͬ���ļ�
	switch(FileID)
	{
		case 0x13f12000://ʵʱ���ļ������֮���ɾ��
		case 0x13f17000:
			break;
		case 0x13f16000://�ն���
		case 0x13f11000:
			YYMD1=0;
			YYMD1=(JSON->Lastrealtime[0]<<8)|(JSON->Lastrealtime[1]<<16)|(JSON->Lastrealtime[2]);
			YYMD2=0;
			YYMD2=(ptime[0]<<8)|(ptime[1]<<16)|(ptime[2]);
			if(YYMD1>YYMD2)//ǰ���ʱ��Ⱥ����ʱ����˵���Ѿ�����һ������,����ļ�Ҫɾ��
			{
				ret=1;
			}
		break;
		case 0x13f18000://�¶���
		case 0x13f13000:
			//YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<8)|JSON->Lastrealtime[2];
			//YYMD1=YYM_SubM_hex(YYMD1,12);//�������ռ�1��,���ؼ�1�����������
			//YYMD2=(ptime[0]<<16)|(ptime[1]<<8)|ptime[2];
			YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<8);
			//YYMD1=YYM_SubM_hex(YYMD1,12);//�������ռ�1��,���ؼ�1�����������
			YYMD2=(ptime[0]<<16)|(ptime[1]<<8);
			if(YYMD1>YYMD2)//��ݴ����ļ���ݾ�ɾ��
			{
				ret=1;
			}
		break;
		case 0x13f19000://15���Ӷ���
			YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|JSON->Lastrealtime[3];
			YYMD2=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|ptime[3];
			
			if((JSON->Lastrealtime[4]*60+JSON->Lastrealtime[5])<=(60+10))//�������1��10��֮���Ǿ�ɾ��,��Ȼ��ɾ����Ϊ��û�д��
			{
				YYMD1=YYMD_Sub1D_hex(YYMD1);//�������ռ�1��,���ؼ�1�����������
				if(YYMD1>YYMD2)
				{
					ret=1;
				}
			}else
			{
				if(YYMD1!=YYMD2)
				{
					ret=1;
				}
			}
		break;
		case 0x13f22000://1���Ӷ���
			YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|JSON->Lastrealtime[3];
			YYMD2=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|ptime[3];
			
			if((JSON->Lastrealtime[4]*60+JSON->Lastrealtime[5])<=(60+10))//�������1��10��֮���Ǿ�ɾ��,��Ȼ��ɾ����Ϊ��û�д��
			{
				YYMD1=YYMD_Sub1D_hex(YYMD1);//�������ռ�1��,���ؼ�1�����������
				if(YYMD1>YYMD2)
				{
					ret=1;
				}
			}else
			{
				if(YYMD1!=YYMD2)
				{
					ret=1;
				}
			}
			break;
		case 3://�ѱ�����ɾ��
		case 4:
			break;
		default:
			ret=1;//������ʲô�����Ӱ˵Ķ�ɾ��
		break;
	}
	return ret;
}

u32 AutoDeleteFile_SeletFileID(u32 FileID)
{
	u32 ret;
	ret=0;
	//����FileID��ɾ����ͬ���ļ�
	switch(FileID)
	{
		case 0x13f12000:
		case 0x13f17000:
		case 0x13f16000://�ն���
		case 0x13f11000:
		case 0x13f18000://�¶���
		case 0x13f13000:
		case 0x13f19000://15���Ӷ���
		case 0x13f22000://1���Ӷ���
		case 3://�ѱ�����ɾ��
		case 4:
			break;
		default:
			ret=1;//������ʲô�����Ӱ˵Ķ�ɾ��
		break;
	}
	return ret;
}

u32 AutoDeleteFile_RealData(u32 FileID)
{
	u32 ret;
	ret=0;
	//����FileID��ɾ����ͬ���ļ�
	switch(FileID)
	{
		case 0x13f12000:
		case 0x13f17000:
			ret=1;//������ʲô�����Ӱ˵Ķ�ɾ��
		break;
	}
	return ret;
}

u32 AutoDelet_JSData(u32 FileID)
{
	u32 ret;
	ret=0;
	switch(FileID)
	{
		case 0x13f16000://�ն���
		case 0x13f11000:
		case 0x13f18000://�¶���
		case 0x13f13000:
		case 0x13f19000://15���Ӷ���
		case 0x13f22000://1���Ӷ���
		case 0x13f12000:
		case 0x13f17000:
			ret=1;
			break;
	}
	return ret;
}

u32 AutoDeleteFile(u32 filename)//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
{
	u32 i;
	u32 FileCount;
	u32 FileID;
	msFILE_TypeDef *file;
	ms_Type *ms;
	ms=Get_ms();
	FileCount=0;	
	if(filename<FILENAMEmax_FLASH)
	{//FLASH
	if(Terminal_Router->MinuteCollect==1)//�ز��Ǳ߾������ֵ
	{
		//��ɾ�����ǽ�����Ҫ������
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);	
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				//i=file->FILECOUNT;
				FileID=file->FILEID;
				i=AutoDeleteFile_SeletFileID(FileID);
				if(i)
				{
					ms->msfile_Delete(filename);
					FileCount=1;
					break;
				}
			}
			file++;
		}
		if(FileCount)
		{
			return 1;
		}
		//��ɾ������ʱ����������
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);	
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				//i=file->FILECOUNT;
				FileID=file->FILEID;
				i=AutoDeleteFile_SeletTime(FileID,file->save_date_time_s);
				if(i)
				{
					ms->msfile_Delete(filename);
					FileCount=1;
					break;
				}
			}
			file++;
		}
		if(FileCount)
		{
			return 1;
		}
		//��ɾ��ʵʱ����
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);	
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				//i=file->FILECOUNT;
				FileID=file->FILEID;
				i=AutoDeleteFile_RealData(FileID);
				if(i)
				{
					ms->msfile_Delete(filename);
					FileCount=1;
					break;
				}
			}
			file++;
		}
		if(FileCount)
		{
			return 1;
		}
	
		return 0;//����:0=ûɾ��,1=��ɾ��
	}else
	{
		//�鵱ǰ�����ļ�����
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				i=file->FILECOUNT;
				FileID=file->FILEID;
				#if(USER/100)==17//����
				if(AutoDelet_JSData(FileID))//Flash���滹��û��ԭ�����յ�����
				{
					ms->msfile_Delete(filename);
					return 1;//��ɾ��
				}
				#endif
				//if((FileID>>20)==0x136)
				if((FileID>>24)==0x13)
				{//�����ļ�
				#if (USER/100)==9//����ÿ5����1����
					if(i<=(3*96*3))//����3��
					{
						i=0;
					}
				#else
					if(i<=(3*96))//����3��
					{
						i=0;
					}
				#endif
				}
				if(i>FileCount)
				{
					FileCount=i;
				}
			}
			file++;
		}
//ɾ������ļ��������ļ�
		if(FileCount<=1)
		{//��С����1
			return 0;//����:0=ûɾ��,1=��ɾ��
		}
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				i=file->FILECOUNT;
				FileID=file->FILEID;
				//if((FileID>>20)==0x136)
				if((FileID>>24)==0x13)
				{//�����ļ�
				#if (USER/100)==9//����ÿ5����1����
					if(i<=(3*96*3))//����3��
					{
						i=0;
					}
				#else
					if(i<=(3*96))//����3��
					{
						i=0;
					}
				#endif
				}
				if(i>=FileCount)
				{
					ms=Get_ms();
					ms->msfile_Delete(filename);
					return 1;//����:0=ûɾ��,1=��ɾ��
				}
			}
			file++;
		}
		return 0;//����:0=ûɾ��,1=��ɾ��
	}
	}
	else
	{//RAM
//�鵱ǰ�����ļ�����
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		for(filename=0;filename<recordFILENAMEmax_RAM;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				i=file->FILECOUNT;
				if(i>FileCount)
				{
					FileCount=i;
				}
			}
			file++;
		}
//ɾ������ļ��������ļ�
		if(FileCount<=1)
		{//��С����1
			return 0;//����:0=ûɾ��,1=��ɾ��
		}
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
		for(filename=0;filename<recordFILENAMEmax_RAM;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				i=file->FILECOUNT;
				if(i>=FileCount)
				{
					filename+=FILENAMEmax_FLASH;
					ms=Get_ms();
					ms->msfile_Delete(filename);
					return 1;//����:0=ûɾ��,1=��ɾ��
				}
			}
			file++;
		}
		return 0;//����:0=ûɾ��,1=��ɾ��
	}
}

void log_Flash(void)
{
	u32 filename;
	u32 FileID;
	msFILE_TypeDef *file;
	u32 SLFDATA=0;
	u32 RealMIN=0;
	u32 Frz1Min=0;
	u32 Frz15Min=0;
	u32 FrzDAY=0;
	u32 FrzMON=0;
	u32 NotFrz=0;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:�ļ����洢�ռ�recordFILENAMEmax_FLASH=%d\n",(u32)&__func__,(u32)__LINE__,recordFILENAMEmax_FLASH);
	#endif
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			switch(FileID)
			{
				case 0x13f05000://��������
					SLFDATA++;
				break;
				case 0x13f16000://�ն���
				case 0x13f11000:
					FrzDAY++;
				break;
				case 0x13f18000://�¶���
				case 0x13f13000:
					FrzMON++;
				break;
				case 0x13f19000://15���Ӷ���
					Frz15Min++;
				break;
				case 0x13f22000://1���Ӷ���
					Frz1Min++;
				break;
				case 0x13f12000:
				case 0x13f17000:
					RealMIN++;
					break;
				default:
					NotFrz++;//������ʲô�����Ӱ˵Ķ�ɾ��
				break;
			}
		}
		file++;
	}
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:��������=%d \n\n",(u32)&__func__,(u32)__LINE__,SLFDATA);
	myprintf("[%s:%d]:�ն���=%d \n\n",(u32)&__func__,(u32)__LINE__,FrzDAY);
	myprintf("[%s:%d]:�¶���=%d \n\n",(u32)&__func__,(u32)__LINE__,FrzMON);
	myprintf("[%s:%d]:15����=%d \n\n",(u32)&__func__,(u32)__LINE__,Frz15Min);
	myprintf("[%s:%d]:1����=%d \n\n",(u32)&__func__,(u32)__LINE__,Frz1Min);
	myprintf("[%s:%d]:ʵʱ��������=%d \n\n ",(u32)&__func__,(u32)__LINE__,RealMIN);
	myprintf("[%s:%d]:���ǽ�����Ҫ������=%d \n\n ",(u32)&__func__,(u32)__LINE__,NotFrz);
	#endif
}


#if(USER/100)==15//����
u8* GetROAD(u32 ROAD,u8* pDATA)
{
	u32 i;
	u32 x;
	u32 j;
	u32 n;
	u32 Offset;
	
	Offset=29;
	n=pDATA[Offset+1];
	Offset+=2;
	for(i=0;i<n;i++)
	{
		if(pDATA[Offset+1]==0)
		{//OAD
			Offset+=6;
		}
		else
		{//ROAD
			x=pDATA[Offset+6];//����OAD����
			j=(pDATA[Offset+2]<<24)+(pDATA[Offset+3]<<16)+(pDATA[Offset+4]<<8)+pDATA[Offset+5];
			if(ROAD==j)
			{
				return pDATA;
			}
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	return 0;
}
u8* Get_RMOADDATA(u32 OAD,u8* pDATA,bool bInsertData)//�õ�����������ָ����OAD����ָ��,���:pDATA=��������;����:0=û�ҵ�
{
	u32 i;
	u32 x;
	u32 Offset;
	u32 Len;
	u32 n;
	u32 OADcount;
	u8* pData;
	u32 buff;
	u32 l;


	//�����¼���ݿ�ʼָ��
	Offset=29;
	n=pDATA[Offset+1];
	Offset+=2;
	for(i=0;i<n;i++)
	{
		if(pDATA[Offset+1]==0)
		{//OAD
			Offset+=6;
		}
		else
		{//ROAD
			x=pDATA[Offset+6];//����OAD����
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=pDATA+Offset;	
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)
	//����ͬOAD�ڼ�¼�е�OAD����
	Offset=29+2+5;
	OADcount=0;//��ͬOAD�ڼ�¼�е�OAD����
	n=pDATA[Offset+1];
	for(i=0;i<n;i++)
	{
		x=(pDATA[Offset+2]<<24)+(pDATA[Offset+3]<<16)+(pDATA[Offset+4]<<8)+pDATA[Offset+5];
		if(OAD==x)
		{
			break;
		}

		OADcount++;
		Offset+=4;
	}
	if(i>=n)
	{//û�ҵ�,��OAD��ӵ�pDATA
		if(bInsertData)
		{
			pDATA[29+2+5+1]++;
			OADcount++;

			buff=(u32)ADDR_4KWRITE_BUFF;
			MC(0,buff,1023);

			Offset+=2;
			l=pDATA[6]|(pDATA[7]<<8);
			l-=Offset;
			l+=8;
			MR((u32)buff,(u32)pDATA+Offset,l);
			OAD=R_Inverse(OAD,4);
			MWR(OAD,(u32)pDATA+Offset,4);//���OAD
			if(l>4096)
			{
				l=4096;
			}
			MR((u32)pDATA+Offset+4,(u32)buff,l);//���ԭ�ȵ�����
			
			//pDATA[Offset+5+l]=0;//�յ�ѹ�ϸ����ݶ�ΪNULL
			pDATA[6]=(l+4+Offset-8)&0xff;
			pDATA[7]=(l+4+Offset-8)>>8;
		}
		else
			return 0;

	}
	Len=0;//����ƫ��
	while(OADcount--)
	{
		Len+=Get_DL698DataLen_S(pData+Len,0);//�õ�DL698�������ͳ���(����:�����ֽں�����);���:LENmax��ȡNAND����ʱ�ȶ���ADDR_128KDATABUFF��,����:����
	}
	return pData+Len;
}


void CALC_MeterVoltage_Qualification_Rate(u8 *pbuf)
{
	u32 i;
	u32 iAddrLen;
	u32 iSame;
	u32 iHasUse;
	u32 j;
	u32 a;
	u32 b;
	u32 c;
	u32 f;//��ǰԽ�ޱ�־
	u32 t;
	u32 x;
	u8 *p8;
	u8 *p8Data;
	u8 *p8Addr;
	u32 limitH;//��ѹ��������
	u32 limitL;//��ѹ��������
	u32 standardH;//��ѹ�ϸ�����
	u32 standardL;//��ѹ�ϸ�����

	t=Comm_Ram->DL698YMDHMS[3];//��
	t--;
	MR(ADDR_DATABUFF,ADDR_4030,LENmax_4030);
	p8=(u8*)ADDR_DATABUFF;
	limitH=(p8[3]<<8)|p8[4];//��ѹ��������
	limitL=(p8[6]<<8)|p8[7];//��ѹ��������
	standardH=(p8[9]<<8)|p8[10];//��ѹ�ϸ�����
	standardL=(p8[12]<<8)|p8[13];//��ѹ�ϸ�����
	
	
	
	for(i=0;i<3;i++)
	{
//��ʼ�����ݽṹ
		p8=(u8*)ADDR_2131_OneDay+(LENmax_2131_OneDay*i);
		MR(ADDR_DATABUFF,(u32)pbuf,6);//��ȡ���ͨ�ŵ�ַ;
		p8Addr=(u8*)ADDR_DATABUFF;
		iSame=0;
		for(j=0;j<NUM_RMmax;j++)
		{
			for(iAddrLen=0;iAddrLen<6;iAddrLen++)
			{
				if(p8Addr[iAddrLen]!=p8[6+iAddrLen+j*48])
				{
					iSame=0;
					break;
				}
				iSame=1;
			}
			if(iSame==1)
				break;
		}
		if(iSame==0)
		{
			for(j=0;j<NUM_RMmax;j++)
			{
				iHasUse=0;
				for(iAddrLen=0;iAddrLen<6;iAddrLen++)
				{
					if(p8[6+iAddrLen+j*48]==0)
					{
						iHasUse++;
					}
				}
				if(iHasUse==6)
						break;
			}
			p8=(u8*)ADDR_2131_OneDay+(i*LENmax_2131_OneDay)+(j*48);
			MR((u32)p8+6,(u32)p8Addr,6);
		}
		else
		{
			if(p8[39]==0)
			{
				MC(0,ADDR_2131_OneDay+(i*LENmax_2131_OneDay)+(j*48),48);
				p8[39]=t;
			}
			else if(p8[39]!=t)
			{
				x=p8[39];
				MW(ADDR_2131_OneDay+(i*LENmax_2131_OneDay)+(j*48),ADDR_2131_All+(((i*31)+x)*LENmax_2131_All)+(j*48),48);
				MC(0,ADDR_2131_OneDay+12+(i*LENmax_2131_OneDay)+(j*48),36);
				p8[39]=t;
			}
		}
		p8[5]=5;//ͨ�ŵ�ַ����
			
//����2����ѹ�ϸ������ݣ���=structure
		p8[0]=DataType_structure;
		p8[1]=2;
//{
//  ���յ�ѹ�ϸ���         ��ѹ�ϸ��ʣ�
//��ѹ�ϸ��ʡ�=structure
		p8[2]=DataType_structure;
		p8[3]=7;
//{
// �洢�ɼ������ݵĵ��ͨ�ŵ�ַ  octet-string
		p8[4]=DataType_octet_string;
		p8[5]=6;
// ��ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
		p8[12]=DataType_double_long_unsigned;
//��ѹ�ϸ���   long-unsigned����λ��%�����㣺-2����
		p8[17]=DataType_long_unsigned;
//��ѹ������   long-unsigned����λ��%�����㣺-2����
		p8[20]=DataType_long_unsigned;
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
		p8[23]=DataType_double_long_unsigned;
//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩
		p8[28]=DataType_double_long_unsigned;
		// �ϴε�ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
		p8[33]=DataType_double_long_unsigned;
//}
//  ���µ�ѹ�ϸ���         ��ѹ�ϸ���
//��ѹ�ϸ��ʡ�=structure
		p8[38]=0;//���µ�ѹ�ϸ���Ϊ��
		
		MR(ADDR_DATABUFF,(u32)pbuf+29+11+3*i,2);//��ȡ��Ӧ��ѹ����
		p8Data = (u8*)ADDR_DATABUFF;//ȡ�����ȡ�ĵ�ѹ
		a=(p8Data[0]<<8)|p8Data[1];
		if(a>=limitH)//��ѹ��������
		{
			f=1;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
		}
		else
		{
			if(a>=standardH)//��ѹ�ϸ�����
			{
				f=2;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
			}
			else
			{
				if(a<=limitL)//��ѹ��������
				{
					f=5;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
				}
				else
				{
					if(a<=standardL)//��ѹ�ϸ�����
					{
						f=4;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
					}
					else
					{
						f=3;//��ѹ��ǰԽ�ޱ�־:0=��,1=Խ��������,2=Խ�ϸ�����,3=�ϸ�,4=Խ�ϸ�����,5=Խ��������
					}
				}
			}
		}
		a=Comm_Ram->DL698YMDHMS[4];//ʱ
		a*=60;
		a+=Comm_Ram->DL698YMDHMS[5];//��
		//���յ�ѹ���ʱ��
		
		MR(ADDR_DATABUFF,(u32)ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48,48);//��ȡ��Ӧ��ѹ����
		//p8t=(u8*)ADDR_DATABUFF;
		//b=R_Inverse(a,4);
		MR((u32)p8Data,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+34,4);
		b=(p8Data[0]<<24)|(p8Data[1]<<16)|(p8Data[2]<<8)|(p8Data[3]);
		if(b==0)//��ʾ�յ�ѹ�ϸ��ʼ�������պ�
		{
			b=1;
		}
		else	
		{
//			if(a<b)
//			{
//				c=0;
//				c=R_Inverse(c,4);
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+3+8,4);//��ѹ���ʱ������
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);//��ѹ������ʱ������
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);//��ѹ������ʱ������
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+8+8,2);//��ѹ�ϸ�������
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+11+8,2);//��ѹ����������
//				b=1;
//			}
//			else
			if(a>=b)
			{
				x=a-b;
				if(x<=5)//��������ڵĲ�࣬Ĭ��Ϊ�м�������Ƕ��ˣ���������ӣ���ʾ������ϵͳʱ��
					b=x;
				else
					b=1;
			}
			else
				b=1;
		}
		a=R_Inverse(a,4);
		MWR(a,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+34,4);
		
		c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+3+8,4);//��ѹ���ʱ��
		c=R_Inverse(c,4);
		c+=b;
		c=R_Inverse(c,4);
		MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+3+8,4);
		switch(f)
		{
			case 1://1=Խ��������
			case 2://2=Խ�ϸ�����,
				//���յ�ѹ������ʱ��
				c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);
				c=R_Inverse(c,4);
				c+=b;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);
				break;
			case 3://3=�ϸ�
				break;
			case 4://4=Խ�ϸ�����,
			case 5://5=Խ��������
				//���յ�ѹ������ʱ��
				c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);
				c=R_Inverse(c,4);
				c+=b;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);
				break;
		}
		//���յ�ѹ������
		//a=MRR(ADDR_2131_1+((i*31+t)*LENmax_2131_1+j*48)+2+3+8,4);//��ѹ���ʱ��
		b=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);//��ѹ������ʱ��
		c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);//��ѹ������ʱ��
		a=1440;
		b=R_Inverse(b,4);
		c=R_Inverse(c,4);
		b+=c;
		if(b>a)
		{
			b=a;
		}
		b*=10000;//%��λС��
		c=b/a;//������
		b=10000-c;//�ϸ���
		b=R_Inverse(b,2);
		c=R_Inverse(c,2);
		MWR(b,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+8+8,2);//��ѹ�ϸ���
		MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+11+8,2);//��ѹ������
		
		
	}
}
void Addr_Voltage_Qualification_Rate(u8 *pbuf)
{
	u32 a;
	u32 i;
	u32 j;
	u32 day;
	u32 iAddrLen;
	u32 iSame;
	u8* p8;
	u32 len;
	u8* p8Data;
	u8* pDATA;
	for(i=0;i<3;i++)
	{
		MR(ADDR_DATABUFF,(u32)pbuf,6);//��ȡ���ͨ�ŵ�ַ;
		MR(ADDR_DATABUFF+6,(u32)pbuf+22,7);//�洢ʱ�꣬�����ն��ᵱ�����һ���ӵ�ʱ�伴23:59;
		p8=(u8*)ADDR_DATABUFF;
		day=p8[6+3];//��ʾ������һ��Ķ���
		day--;
		p8Data=(u8*)ADDR_4KWRITE_BUFF;
		MR((u32)p8Data,ADDR_2131_All+(((i*31)+day)*LENmax_2131_All),LENmax_2131_All);
//		if(p8Data[0]==0xff)
//		{
//			MC(0,(u32)p8Data,LENmax_2131_All);
//		}
		iSame=0;
		for(j=0;j<NUM_RMmax;j++)
		{
			for(iAddrLen=0;iAddrLen<6;iAddrLen++)
			{
				if(p8[iAddrLen]!=p8Data[6+iAddrLen+j*48])
				{
					iSame=0;
					break;
				}
				iSame=1;
			}
			if(iSame==1)
				break;
		}
		MR((u32)p8,(u32)p8Data+12+j*48,21);
		if(i==0)
			pDATA=Get_RMOADDATA(0x21310201,pbuf,true);
		if(i==1)
			pDATA=Get_RMOADDATA(0x21320201,pbuf,true);
		if(i==2)
			pDATA=Get_RMOADDATA(0x21330201,pbuf,true);
		
		if(iSame==0||p8[0]==0)//��ѹ�ϸ���δ������¼,����������ݼ�¼Ϊȫ���ϸ�
		{
			p8[0]=DataType_structure;
			p8[1]=2;
		//{
		//  ���յ�ѹ�ϸ���         ��ѹ�ϸ��ʣ�
		//��ѹ�ϸ��ʡ�=structure
			p8[2]=DataType_structure;
			p8[3]=5;

		// ��ѹ���ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
			p8[4]=DataType_double_long_unsigned;
			a=0;
			a=R_Inverse(a,4);
			MWR(a,(u32)p8+5,4);
			
		//��ѹ�ϸ���   long-unsigned����λ��%�����㣺-2����
			p8[9]=DataType_long_unsigned;
			a=10000;
			a=R_Inverse(a,2);
			MWR(a,(u32)p8+10,2);
		//��ѹ������   long-unsigned����λ��%�����㣺-2����
			p8[12]=DataType_long_unsigned;
			a=0;
			a=R_Inverse(a,2);
			MWR(a,(u32)p8+13,2);
		//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩��
			p8[15]=DataType_double_long_unsigned;
			a=0;
			a=R_Inverse(a,4);
			MWR(a,(u32)p8+16,4);
		//��ѹ������ʱ��  double-long-unsigned����λ�����ӣ��޻��㣩
			p8[20]=DataType_double_long_unsigned;
			a=0;
			a=R_Inverse(a,4);
			MWR(a,(u32)p8+21,4);
			p8[25]=0;//���µ�ѹ�ϸ���Ϊ��
			
			MW((u32)p8+2,(u32)pDATA,23);
		}
		else
		{
			pDATA[0]=2;
			pDATA[1]=5;
			
			MW((u32)p8,(u32)pDATA+2,21);
		}

		len=pbuf[6]|(pbuf[7]<<8);
		len+=23;
		pbuf[6]=(len>>0)&0xff;
		pbuf[7]=(len>>8)&0xff;

	}
}
#endif



#if(USER/100)==17//???
u32 InsertRecord(u32 PORTn,u32 FileID,u8* pbuf,u32 len)
{
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	u32 n;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u32 i;
	u32 offset;
	u8 *p8;
	u32 lenAddr;
	u32 m;
	
	ms=Get_ms();
	filename=Get_RecordFileName(FileID,0xffffffff,pbuf+22);
	if(filename==0xffffffff)//???????????????
		return 0;
	if(filename<FILENAMEmax_FLASH)
	{
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
	}
	else
	{
		file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
	}
	
	lenAddr=6;
	n=file->NUM_DATA;
	ms->msfile_read(filename,0,(u8*)ADDR_AddrList,n*sizeof(RecordFileHead_TypeDef));
	RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
	offset=0;
	Addr0=pbuf[0]|(pbuf[1]<<8);
	Addr1=pbuf[2]|(pbuf[3]<<8);
	Addr2=pbuf[4]|(pbuf[5]<<8);
	for(i=0;i<n;i++)
	{
		if(RecordFileHead->Addr0==Addr0)
		{
			if(RecordFileHead->Addr1==Addr1)
			{
				if(RecordFileHead->Addr2==Addr2)
				{
					break;
				}
			}
		}
		offset+=RecordFileHead->LEN_DATA;
		RecordFileHead++;
	}
	if(i>=n)
	{//???
		return 0;
	}
	i=RecordFileHead->LEN_DATA;
	ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_4KWRITE_BUFF,i);
	
	p8=(u8*)ADDR_4KWRITE_BUFF;
	m=p8[29];
	n=37+1+4*pbuf[31+lenAddr];
	pbuf[31+lenAddr]+=(m-1);
	MW((u32)pbuf+n,(u32)pbuf+1024,len-n);
	m=(p8[29]-1)*4;
	MW((u32)p8+34,(u32)pbuf+n,m);
	MW((u32)pbuf+1024,(u32)pbuf+n+m,len-n);
	MW((u32)p8+34+m+4,(u32)pbuf+len+m,i-38-m);
//	p8+=(pbuf[31+lenAddr]-1)*4;
//	MW(ADDR_4KWRITE_BUFF,(u32)p8,n);
//	p8+=n;
//	n=pbuf[31+lenAddr];
//	MW((u32)pbuf+31+n*4+lenAddr,(u32)p8,len-31-n*4);
//	
//	n=RecordFileHead->LEN_DATA-i*4-31;
//	
//	MW(ADDR_DATABUFF,(u32)pbuf+lenAddr+2,len+(i-1)*4+n);
	
	pbuf[lenAddr]=(len+i-30)&0xff;
	pbuf[lenAddr+1]=(len+i-30)>>8;
	
	return len+i-30;
	
//	y=file->NUM_DATA;//??????????(???????)
//	if(y>NUM_RMmax)
//	{
//		y=NUM_RMmax;
//	}
//	ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
//	RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
//	Addr0=pbuf[0]|(pbuf[1]<<8);
//	Addr1=pbuf[2]|(pbuf[3]<<8);
//	Addr2=pbuf[4]|(pbuf[5]<<8);
//	for(i=0;i<y;i++)
//	{
//		if(RecordFileHead->Addr0==Addr0)
//		{
//			if(RecordFileHead->Addr1==Addr1)
//			{
//				if(RecordFileHead->Addr2==Addr2)
//				{
//					break;
//				}
//			}
//		}
//		RecordFileHead++;
//	}
	
}
#endif
void ReSave_ACSAMPLE_Data(u32 PORTn,u8 *pbuf,u32 FileID)
{
	u32 i;
	u32 x;
	u32 y;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	//u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	ms=Get_ms();
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;//�����

	if(PORTn!=ACSAMPLEPORT)
	{
		return ;
	}
	Comm_Ram->msFILESearchCount=0;

	if((FileID&0x0f)==1)//����15���ӽ�������
	{
		FileID=0x13f12000;//1���Ӵ�����Ǹ��ļ�  15��������	
		filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+8);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
	}else
	{
		filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+22);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
	}
	if(filename!=0xffffffff)
	{
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		y=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
		if(y>NUM_RMmax)
		{
			y=NUM_RMmax;
		}
		ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
		RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
		Addr0=pbuf[0]|(pbuf[1]<<8);
		Addr1=pbuf[2]|(pbuf[3]<<8);
		Addr2=pbuf[4]|(pbuf[5]<<8);
		for(i=0;i<y;i++)
		{
			if(RecordFileHead->Addr0==Addr0)
			{
				if(RecordFileHead->Addr1==Addr1)
				{
					if(RecordFileHead->Addr2==Addr2)
					{
						break;
					}
				}
			}
			RecordFileHead++;
		}
		if(i<y)
		{//��ַ�Ѵ���
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				;
			}else
			{
				return;//�������ߵ�����ʱ���ܻ��ظ�,�ʲ���ɾ���ļ�
			}
		}
	}
	if(filename==0xffffffff)
	{//û�ҵ�
		while(1)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
			{
				if(file->BLOCK_ENTRY==0)
				{
					break;
				}
				file++;
			}
			if(filename>=recordFILENAMEmax_FLASH)
			{//û��
				i=AutoDeleteFile(0);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
				if(i)
				{//��ɾ��
					continue;
				}
				return;
			}
			break;
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//���ļ�
		ms->msfile_lenclear(filename);//��0�ļ���صĳ��ȴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;
		file->LEN_DATA=0;
		file->NUM_DATA=0;
		if((FileID==0x13f12000)||(FileID==0x13f17000))
		{
			MW((u32)pbuf+8,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
			MW((u32)pbuf+15,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
		}else
		{
			MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
			MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
		}
	}
//д�ļ�
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//��
		ms->msfile_Delete(filename);
		return;
	}
	//д����
	if(filename>=FILENAMEmax_FLASH)
	{//RAM���ļ�
		i=ms->ms_malloc_sizemax();//���ɷ���ĳߴ�(�ֽ���)
		if(i<(1024*1024))
		{//�ռ�<1M,��������1M�ռ����ڼ�¼���ݶ�ȡ
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y==0)
			{//ûɾ��
				return;
			}
		}
	}
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	}
	RecordFileHead=(RecordFileHead_TypeDef*)pbuf;
	while(1)
	{
		y=RecordFileHead->LEN_DATA;
		y=ms->msfile_write(filename,i,pbuf+sizeof(RecordFileHead_TypeDef),y);
		if(y!=RecordFileHead->LEN_DATA)
		{//д����
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y)
			{//��ɾ��
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//д��ַ�б�
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//������+1
	x++;
	file->NUM_DATA=x;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:д��ɹ� \n",(u32)&__func__,(u32)__LINE__,0);
	#endif
}

//UART�������ݻ��嶨��
//#define LEN_RM_DI_CSD     512
//�������ݻ���1,����ܳ���=LEN_RM_DI_CSD
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)

//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29  �ɼ���CSD  array CSD(����������)

//�������ݻ���2,����ܳ���=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte ����=����д��buff����û����һ��CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+2 2byte ����=����д��buff����CSD�ĳ���(����������2�ֽ�)
//LEN_RM_DI_CSD+4 �ɼ�������(����������)


//���������ϲ������ݻ���1
//0 6byte ���ܱ��ַ
//6 2byte ����=ʱ��+�Ѳɼ���CSD(����������2�ֽ�)
//8 7byte �ɼ�����ʱ��  date_time_s(����������)
//15 7byte �ɼ��ɹ�ʱ��  date_time_s(����������)
//22 7byte �ɼ��洢ʱ��  date_time_s(����������)
//29 �ɼ���CSD  array CSD(����������)
//n  �ɼ�������(����������,ȫ������Ϊ����˳��������������)
void RM_RecordFile(u32 PORTn,u8 *pbuf)//�������ݴ���(����,�¼�)
{
	u32 i;
	u32 x;
	u32 y;
	u32 FileID;
	u32 EventID;//�¼�������ID
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	#if (USER/100)==15//����
	u8* pDATA;
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//i=UARTCtrl->TaskID;
	//i>>=8;
	//i&=0xff;//�����
#if(USER/100)==17//����ϵ
	if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
	{
		switch(UARTCtrl->res23)
		{
			case 3:
			case 4:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff22ff;
				break;
			case 1:
			case 6:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff16ff;
				break;
			case 2:
			case 7:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff18ff;
				break;
			case 8:
			case 9:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff19ff;
				break;
			case 10:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff11ff;
				break;
			case 11:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff13ff;
				break;
			case 12:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff12ff;//һ����
				break;
			case 13:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff17ff;//15����
			default:
				break;	
		}	
	}
	
	if((PORTn==ACSAMPLEPORT)&&(Terminal_Router->MinuteCollect==1))
	//if((PORTn==ACSAMPLEPORT))//����
	{
		x=21;
		x+=8;
		x+=4;
		#ifdef DEBUG_JSON	
		myprintf("[%s:%d]:���ɽ���\n",(u32)__func__,(u32)__LINE__,0);
		for(i=0;i<300;i++)
		{	
			myprintf("%02x ",pbuf[i],0,0);
		}
		myprintf("\n",0,0,0);
		#endif
		i=(pbuf[x]<<24)|(pbuf[x+1]<<16)|(pbuf[x+2]<<8)|pbuf[x+3];
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:���ɼ�¼��OAD=%08x\n",(u32)&__func__,(u32)__LINE__,i);
		#endif
		switch(i)
		{
			case 0x50040200:
				FileID=0x13f16000;
				break;
			case 0x50060200:
				FileID=0x13f18000;
				break;
			case 0x50020200:
				FileID=0x13f19000;//15��������
				break;
			default:
				return;
		}
		ReSave_ACSAMPLE_Data(PORTn,pbuf,FileID);
		if(FileID==0x13f19000)
		{
			FileID|=1;
			ReSave_ACSAMPLE_Data(PORTn,pbuf,FileID);
		}
		return;//�����֧�ַ��Ӳɼ�,ֱ���˳�
	}
	
#endif
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;//�����

	switch(i)
	{
		case RMTASKnoPOWER://ͣ�ϵ��¼��̶������
			#if (USER/100)==15//����
			if(UARTCtrl->RMprotocol==3)
				//PowerEventTimerComp(pbuf+37);
				PowerEventTimerComp(pbuf+56);
			if(UARTCtrl->RMprotocol==2)
				PowerEventTimerComp(pbuf+58);
			#else
			PowerEventTimerComp(pbuf+37);//ͣ�ϵ��¼�ʱ��Ƚ�,���:pEVENTָ��698��ʽ�¼��Ŀ�ʼ,����:PowerEvent->bit
			#endif
			return;
		#if (USER/100)==15//����
		case 49://�յ�ѹ�ϸ��ʲɼ�����
			if(UARTCtrl->RMprotocol==2)
				CALC_MeterVoltage_Qualification_Rate(pbuf);
			return;
		case 1://�ն���ɼ�������
			pDATA=GetROAD(0x50040200,pbuf);
			if(pDATA!=0&&UARTCtrl->RMprotocol==2)
			{
				MR(ADDR_DATABUFF,(u32)pbuf+15,7);//�ɹ�ʱ��
				p8=(u8*)ADDR_DATABUFF;
				pDATA=Get_RMOADDATA(0x20210200,pbuf,false);
				x=Compare_DL698DataString(pDATA+1,p8,4);//�������ݶ���ʱ��Ͳɼ��ɹ�ʱ������ڲ������������������
				if(x!=0)
					return;
				Addr_Voltage_Qualification_Rate(pbuf);
			}
			else
			{
				EventID=Event_RMDATA(PORTn,pbuf+sizeof(RecordFileHead_TypeDef));//����������ص��¼�
				EventID=EventID;
				Update_Group_Total_Data_6000(pbuf);//�����ܼ��������,���ñ���6000���
			}
			break;
		case 5://������ն���ɼ�������
			pDATA=GetROAD(0x50040200,pbuf);
			if(pDATA!=0&&UARTCtrl->RMprotocol==2)
			{
				MR(ADDR_DATABUFF,(u32)pbuf+15,7);//�ɹ�ʱ��
				p8=(u8*)ADDR_DATABUFF;
				pDATA=Get_RMOADDATA(0x20210200,pbuf,false);
				x=Compare_DL698DataString(pDATA+1,p8,4);//�������ݶ���ʱ��Ͳɼ��ɹ�ʱ������ڲ������������������
				if(x!=0)
					return;
			}
			EventID=Event_RMDATA(PORTn,pbuf+sizeof(RecordFileHead_TypeDef));//����������ص��¼�
			EventID=EventID;
			Update_Group_Total_Data_6000(pbuf);//�����ܼ��������,���ñ���6000���
		#endif
		default:
		#if(USER/100)==17//����ϵ
			if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
				break;
		#endif
			EventID=Event_RMDATA(PORTn,pbuf+sizeof(RecordFileHead_TypeDef));//����������ص��¼�
			EventID=EventID;
			Update_Group_Total_Data_6000(pbuf);//�����ܼ��������,���ñ���6000���
			break;
	}

//TaskID=b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
//���ȼ�:1=��Ҫ,2=��Ҫ,3=��Ҫ,4=����
//��������:1=��ͨ�ɼ�����,2=�¼��ɼ�����,3=͸������,4=�ϱ�����,5=�ű�����
//��ͨ�ɼ������Ĳɼ�����:0=�ɼ���ǰ����,1=�ɼ��ϵ�N��,2=������ʱ��ɼ�,3=��ʱ�����ɼ�,4=����
//��ͨ�ɼ������Ĵ洢ʱ��:0=δ����,1=����ʼʱ��,2=��Ե���0��0��,3=�������23��59��,4=�������0��0��,5=��Ե���1��0��0��,6=���ݶ���ʱ��
	Comm_Ram->msFILEchange=0;//�ļ��������ݱ仯:0=�仯,!=û�仯
	ms=Get_ms();
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	FileID=Get_FileID(PORTn);//UART�������FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
	Comm_Ram->msFILESearchCount=0;

	if(Terminal_Router->MinuteCollect==1)
	{
		if((FileID==0x13f12000)||(FileID==0x13f17000))
		{	
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+8);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
		}
		else
		{
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+22);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
		}
	}
	else
	{
		filename=Get_RecordFileName(FileID,0xffffffff,pbuf+22);//̷̑τݾĻ:ɫ�?FileID=b31-b28׽и`э,b27-b24Ӊܯ`э,b23-b20զԢʱҪ`э,b19-b12Ɏϱۅ,b11-b8׋ࠚ�?b7-b0,РӦԲֵΪ0xFҭʾһژע,pTime˽ߝզԢʱҪ=0ҭʾһژע;SearchCount=ߪʼ̷̑քτݾۅ0xFFFFFFFFҭʾՓʏՎޡ˸ۅߪʼ̷̑;׵ܘ:0--(FILENAMEmax_FLASH-1)ҭʾτݾ՚FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)ҭʾτݾ՚RAM,0xffffffffҭʾû֒ս
	}
	
	if(filename!=0xffffffff)
	{
		if(filename<FILENAMEmax_FLASH)
		{
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags+(filename*sizeof(msFILE_TypeDef)));
		}
		else
		{
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage+((filename-FILENAMEmax_FLASH)*sizeof(msFILE_TypeDef)));
		}
		y=file->NUM_DATA;//�ļ��������ݸ�������(ͬ����ɹ�����)
		if(y>NUM_RMmax)
		{
			y=NUM_RMmax;
		}
		ms->msfile_read(filename,0,(u8*)ADDR_AddrList,y*sizeof(RecordFileHead_TypeDef));
		RecordFileHead=(RecordFileHead_TypeDef*)(ADDR_AddrList);
		Addr0=pbuf[0]|(pbuf[1]<<8);
		Addr1=pbuf[2]|(pbuf[3]<<8);
		Addr2=pbuf[4]|(pbuf[5]<<8);
		for(i=0;i<y;i++)
		{
			if(RecordFileHead->Addr0==Addr0)
			{
				if(RecordFileHead->Addr1==Addr1)
				{
					if(RecordFileHead->Addr2==Addr2)
					{
						break;
					}
				}
			}
			RecordFileHead++;
		}
		if(i<y)
		{//��ַ�Ѵ���
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				;
			}else
			{
				return;//�������ߵ�����ʱ���ܻ��ظ�,�ʲ���ɾ���ļ�
			}
		}
	}
	if(filename==0xffffffff)
	{//û�ҵ�
	#if(USER/100)==17//����ϵ
		switch(UARTCtrl->res23)
		{
			case 1:
			case 6:
			case 2:
			case 7:
			case 10:
			case 11:
			case 12:
			case 13:
			case 3:
			case 4:
			case 8:
			case 9:
				break;
			default:
				//CountInc_RecordFile(PORTn);//�����ļ�����=0ʱ,ȫ����¼�ļ��ļ�����+1,ɾ���ﵽ��¼��ȵ��ļ�
				//AutoDelete_ErrID_RecordFile();//�Զ�ɾ�������Ѳ����ڵļ�¼�ļ�;����:0=ûɾ��,1=��ɾ��
				break;
		}
	#else
		CountInc_RecordFile(PORTn);//�����ļ�����=0ʱ,ȫ����¼�ļ��ļ�����+1,ɾ���ﵽ��¼��ȵ��ļ�
		AutoDelete_ErrID_RecordFile();//�Զ�ɾ�������Ѳ����ڵļ�¼�ļ�;����:0=ûɾ��,1=��ɾ��
	#endif
		//�ҿ��ļ�
#if RecordFILEallinFLASH==0//ȫ����ͨ���������ݱ�����nand flash:0=��(ֻ��OADʱ����RAM),1=��
	#if (USER/100)==0//���Ժ����
		i=UARTCtrl->RMOADROAD;//���������г�����OAD����:b0=��OAD,b1=��ROAD,b3-b7����;���ڳ������ݴ���ʱ�Ǵ���RAM����NAND
		if(i==1)//ȫ��ֻ��OAD,RAM�ļ�
	#else
		//FileID=b31-b28��������,b27-b24�ɼ�����,b23-b20�洢ʱ������,b19-b12�����,b11-b8�˿ں�,b7-b0�ļ�����
		if(((FileID>>24)&0xff)==0x10)//1=��ͨ�ɼ�����,�ɼ�����=0=�ɼ���ǰ����
	#endif
		{
			//myprintf("[%s:%d]:ɾ��((FileID>>24)&0xff)==0x10\n",(u32)&__func__,(u32)__LINE__,0);
			while(1)
			{
				file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
				for(filename=0;filename<recordFILENAMEmax_RAM;filename++)
				{
					if(file->BLOCK_ENTRY==0)
					{
						break;
					}
					file++;
				}
				if(filename>=recordFILENAMEmax_RAM)
				{//û��
					i=AutoDeleteFile(FILENAMEmax_FLASH);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
					if(i)
					{//��ɾ��
						continue;
					}
					return;
				}
				break;
			}
			filename+=FILENAMEmax_FLASH;
		}
		else
#endif
		{//FLASH�ļ�	
			while(1)
			{
				file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
				for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
				{
					if(file->BLOCK_ENTRY==0)
					{
						break;
					}
					file++;
				}
				if(filename>=recordFILENAMEmax_FLASH)
				{//û��
					i=AutoDeleteFile(0);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
					if(i)
					{//��ɾ��
						continue;
					}
					return;
				}
				break;
			}
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//���ļ�
		ms->msfile_lenclear(filename);//��0�ļ���صĳ��ȴ���,0-(FILENAMEmax_FLASH-1)��ʾ�ļ���FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)��ʾ�ļ���RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;
		if(Terminal_Router->MinuteCollect==1)
		{
			file->LEN_DATA=0;
			file->NUM_DATA=0;
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				MW((u32)pbuf+8,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
				MW((u32)pbuf+15,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
			}else
			{
				MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
				MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
			}
		}
		else
		{
			MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//�ɼ��洢ʱ��
			MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//�ɼ�����ʱ��
		}
	}
//д�ļ�
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//��
		ms->msfile_Delete(filename);
		return;
	}
	//д����
	if(filename>=FILENAMEmax_FLASH)
	{//RAM���ļ�
		i=ms->ms_malloc_sizemax();//���ɷ���ĳߴ�(�ֽ���)
		if(i<(1024*1024))
		{//�ռ�<1M,��������1M�ռ����ڼ�¼���ݶ�ȡ
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y==0)
			{//ûɾ��
				return;
			}
		}
	}
	i=file->LEN_DATA;
	if(i==0)
	{
		i=(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
	}
	RecordFileHead=(RecordFileHead_TypeDef*)pbuf;
	while(1)
	{
		y=RecordFileHead->LEN_DATA;
		y=ms->msfile_write(filename,i,pbuf+sizeof(RecordFileHead_TypeDef),y);
		if(y!=RecordFileHead->LEN_DATA)
		{//д����
			y=AutoDeleteFile(filename);//�Զ�ɾ����¼�ļ�(�Զ����ٴ洢���);���:д���ļ�ʱ�ռ䲻�����ļ���,���б���RAM����FLASH�ռ䲻��;����:0=ûɾ��,1=��ɾ��
			if(y)
			{//��ɾ��
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//д��ַ�б�
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//������+1
	x++;
	file->NUM_DATA=x;
	
	//�����ϱ�
	i=MRR(ADDR_4300_8+1,1);
	if(i==0)
	{//�����������ϱ�
		for(i=0;i<NUMmax_601C;i++)
		{
		#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
			file->NUM_GPRSreport[i]=x;
			file->NUM_ENETreport[i]=x;
		#else
			file->NUM_GPRSorENETreport[i]=x;
		#endif
		}
	}
	else
	{//���������ϱ�
		i=UARTCtrl->TaskID;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
		if(((i>>24)&0xf)==2)
		{//��������=�¼��ɼ�����
			i&=0xff;//������
			i*=LENper_6016;
			i+=ADDR_6016_SDRAM;
			p8=Get_Element((u8 *)i,4,0,0);//����Ԫ�ص�ַ,ʹ��ADDR_128KDATABUFF
			if(p8)
			{
				if(p8[1]==0)//�ϱ���ʶ        bool��True�������ϱ���False�����ϱ�����
				{//���ϱ�
					for(i=0;i<NUMmax_601C;i++)
					{
					#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
						file->NUM_GPRSreport[i]=x;
						file->NUM_ENETreport[i]=x;
					#else
						file->NUM_GPRSorENETreport[i]=x;
					#endif
					}
					return;
				}
			}
		}
		
	#if ((USER/100)==15)//��΢ϵ
		i=UARTCtrl->TaskID;//��ǰ�ɼ�����ID;ֵ0��ʾ�޲ɼ�����;b31-b28���ȼ�,b27-b24��������,b23-b20�ɼ�����,b19-b16�洢ʱ������,b15-b8�����,b7-b0�������
		i>>=8;
		i&=0xff;
		if(i==49||i==28||i==54)
		{//�����49��������ڵ�ѹ�ϹC�ʼ��㲻���ϱ�
			for(i=0;i<NUMmax_601C;i++)
			{
			#if GPRS_ENET_REPORT==0//GPRS/ENET�����ϱ�:0=�ϱ��ֿ�,1=�ϱ��ϲ�
				file->NUM_GPRSreport[i]=x;
				file->NUM_ENETreport[i]=x;
			#else
				file->NUM_GPRSorENETreport[i]=x;
			#endif
			}
		}
	#endif		
		//�ϱ�ͨ��
		REPORT_PORTn(file);//ͨ�������ϱ�ʱ�����ϱ����ݸ���=��ǰ���ݸ�����ʹ��ADDR_DATABUFF
	}
	
}










