
//普通采集方案记录文件
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


//方案类型:1=普通采集方案,3=透明方案  文件存储格式
//电能表地址和数据块长度列表区,列表区长度固定 8*NUM_RMmax byte
//6byte 第1个电能表地址
//2byte 第1个电能表数据块长度
//...
//6byte 第NUM_RMmax个电能表地址
//2byte 第NUM_RMmax个电能表数据块长度

//电能表数据块区,长度按实际写入数
//0 7byte 采集启动时标  date_time_s(无数据类型)
//7 7byte 采集成功时标  date_time_s(无数据类型)
//14 7byte 采集存储时标  date_time_s(无数据类型)
//21  采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型)


//方案类型:2=事件采集方案  文件存储格式

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
	RecordFileHead_TypeDef* RecordFileHead;//迎噎輫录蟿菥头蟿菥
	ms=(ms_Type *)Get_ms();
	if(0x13f16000==FileID)
	{
		FileID=0x13f11000;
	}else if(0x13f18000==FileID)
	{
		FileID=0x13f13000;
	}
	filename=Get_RecordFileName_JS(FileID,0xFFFFFFFF,0);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
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
	//时荬謩筛选
	//YYMD=YYMD_Sub1D_hex(YYMD);//弄弄諅蓵莸1蓵,椎軜莸1鄢謩弄弄諅蓵
	i=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
	if(YYMD!=i)
	{
		return;
	}
	//骚诨謷战詯私邼,袌褢原4謩私邼呓汛战颅微一俣BUFFo摩
	if(Len>4096)
	{
		return;
	}
	//拷贝一开始的数据到临时空间里面
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
	{//没找到
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
	//袀摩擢始蹚尧私邼挟
	pda=(u8 *)ADDR_DATABUFF;//詯}俣艢蓾,蹚尧詨一俣698謩帧
	if((datalen+Len)>4096)
	{
		MW((u32)pda,(u32)p8,Len);
		return;
	}
	//褢褌謩私邼挟蹚諝O謩私邼挟鄢摩
	
	MW((u32)p8,(u32)pda+Len,datalen);//两个数据长度连接起来
	
	p8=(u8 *)pda+Len;//p8指胁鄢摩一俣私邼挟
	//袌褢21俣貣逇謩时要呓汛战
	offset=0;
	//袌褢21俣时荬诮脣远去
	MW((u32)pda,(u32)pout,21);
	pda+=21;//没
	p8+=21;
	offset=21;
	Len-=21;
	datalen-=21;
	//6俣貣逇鸭(ROAD
	MW((u32)pda,(u32)pout+offset,8);
	offset+=8;
	pda+=8;
	p8+=8;
	Len-=8;
	datalen-=8;
	//私邼挟謩俣私
	i=pda[0];
	OADNUM1=i;
	n=p8[0];
	OADNUM2=n;
	if((i+n)>0)//私邼挟俣私
	{
		pout[offset++]=i+n-1;//
	}
	Len--;
	datalen--;
	pda++;
	//前摩一俣私邼挟謩私邼呓汛战pout
	MW((u32)pda,(u32)pout+offset,i*4);
	pda+=(i*4);
	offset+=(i*4);
	Len-=(i*4);
	p8++;
	if(n>0)
	{
		p8+=4;//诮脣执私邼殖蕖时荬症俣
		MW((u32)p8,(u32)pout+offset,(n-1)*4);
		p8+=((n-1))*4;
		offset+=((n-1)*4);
		datalen-=(n*4);
	}

	//计算数据域的长度
	datalen=0;
	for(i=0;i<OADNUM1;i++)
	{
		n=Get_DL698DataLen_S(pda+datalen,0);//算出第一个数据的长度
		datalen+=n;
	}
	MW((u32)pda,(u32)pout+offset,datalen);
	offset+=datalen;

	p8+=8;//诮脣执殖蕖时荬私邼挟
	datalen-=8;
	//计算数据域的长度
	datalen=0;
	for(i=0;i<OADNUM2;i++)
	{
		n=Get_DL698DataLen_S(p8+datalen,0);//算出第一个数据的长度
		datalen+=n;
	}
	MW((u32)p8,(u32)pout+offset,datalen);
	
	offset+=datalen;
	//for(i=0;i<offset;i++)
		//myprintf("%02x ", pout[i], 0, 0);
	//myprintf("\n",0,0,0);
}

u32 Get_RecordFileData_Fixed(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//得到记录文件数据;入口:pRSD、pRCSD指向接收的RSD、RCSD(无数据类型)，=0表示不关注;返回:读出的数据长度,0=无数据;使用ADDR_128KDATABUFF
{
	u32 i;
	u32 n;
	u32 offset;
	u32 filename;
	u32 YYMD;
	u32 YYMD1;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;//抄表记录文件头文件
	u8* p8;
	YYMD=0;
	YYMD1=0;
	ms=(ms_Type *)Get_ms();
#if(USER/100)==17//江苏系
	if(Terminal_Router->MinuteCollect==1)
	//if(1)
		filename=Get_RecordFileName_JS(FileID,FileCount,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
	else
		filename=Get_RecordFileName(FileID,FileCount,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
	#else
		filename=Get_RecordFileName(FileID,FileCount,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0,相应域值为0xF表示不关注,pTime数据存储时标=0表示不关注;SearchCount=开始搜索的文件号0xFFFF表示从上次结束号开始搜索;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
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
//不选择       [0]  NULL
			break;
		case 1:
//Selector1∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  数值            Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://数据冻结时间
					pRSD+=4;
					if(pRSD[0]!=DataType_date_time_s)
					{//数值Data=空
						return 0;
					}
					pRSD+=1;
					//数值Data=date_time_s
					for(i=0;i<6;i++)
					{//秒不比较
						if(pRSD[i]!=file->save_date_time_s[i])
						{
							return 0;
						}
					}
					break;
				//case 0x40000200://日期时间
				default:
					return 0;
			}
			break;
		case 2:
//Selector2∷=SEQUENCE
//{
//  对象属性描述符  OAD，
//  起始值          Data，
//  结束值          Data，
//  数据间隔        Data
//}
			pRSD++;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://数据冻结时间
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=0)
					{//不相同
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=2)
					{//时间>=结束值
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 3:
//Selector3∷=SEQUENCE OF Selector2
			pRSD+=2;
			i=(pRSD[0]<<24)|(pRSD[1]<<16)|(pRSD[2]<<8)|pRSD[3];
			switch(i)
			{
				case 0x20210200://数据冻结时间
					pRSD+=5;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=0)
					{//不相同
						return 0;
					}
					pRSD+=8;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=2)
					{//时间>=结束值
						return 0;
					}
					break;
				default:
					return 0;
			}
			break;
		case 4:
//Selector4∷=SEQUENCE
//{
//  采集启动时间  date_time_s，
//  电能表集合    MS
//}
			if((file->FILEID&0xff)!=0)
			{//文件分类!=0
				return 0;
			}
			//取7byte 采集启动时标  date_time_s(无数据类型)
//			ms->msfile_read(filename,NUM_RMmax*sizeof(RecordFileHead_TypeDef),(u8*)ADDR_DATABUFF,7);
//			p8=(u8*)ADDR_DATABUFF;
//			pRSD++;
//			for(i=0;i<6;i++)//秒不比较
//			{
//				if(pRSD[i]!=p8[i])
//				{
//					return 0;
//				}
//			}
			pRSD++;
			for(i=0;i<6;i++)//秒不比较
			{
				if(pRSD[i]!=file->start_date_time_s[i])
				{
					return 0;
				}
			}
			break;
		case 5:
//Selector5∷=SEQUENCE
//{
//  采集存储时间  date_time_s，
//  电能表集合    MS
//}
#if (USER/100==17)	
			if(((FileID>>24)!=0x1E)&&(Terminal_Router->MinuteCollect==1))//月冻结数据
			{
				pRSD++;
				i=(pRSD[0]<<16)|(pRSD[1]<<24)|(pRSD[2]<<8)|(pRSD[3]);
				i=YYMD_Sub1D_hex(i);//年年月日减1日,返回减1后的年年月日
				n=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
				if(i!=n)
				{
					return 0;
				}
			}
			else
			{
				pRSD++;
				for(i=0;i<6;i++)//秒不比较
				{
					if(pRSD[i]!=file->save_date_time_s[i])
					{
						return 0;
					}
				}
			}
#else
			pRSD++;
			for(i=0;i<6;i++)//秒不比较
			{
				if(pRSD[i]!=file->save_date_time_s[i])
				{
					return 0;
				}
			}
#endif
			break;
		case 6:
//Selector6∷=SEQUENCE
//{
//  采集启动时间起始值  date_time_s，
//  采集启动时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			if((file->FILEID&0xff)!=0)
			{//文件分类!=0
				return 0;
			}
			//取7byte 采集启动时标  date_time_s(无数据类型)
//			ms->msfile_read(filename,NUM_RMmax*sizeof(RecordFileHead_TypeDef),(u8*)ADDR_DATABUFF,7);
//			p8=(u8*)ADDR_DATABUFF;
//			pRSD++;
//			i=Compare_DL698DataString(p8,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//			if(i!=0)
//			{//不相同
//				return 0;
//			}
//			pRSD+=7;
//			i=Compare_DL698DataString(p8,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//			if(i!=2)
//			{//时间>=结束值
//				return 0;
//			}
			pRSD++;
			i=Compare_DL698DataString(file->start_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i!=0)
			{//不相同
				return 0;
			}
			pRSD+=7;
			i=Compare_DL698DataString(file->start_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i!=2)
			{//时间>=结束值
				return 0;
			}
			break;
		case 7:
//Selector7∷=SEQUENCE
//{
//  采集存储时间起始值  date_time_s，
//  采集存储时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
//		#if (USER%100)!=0//不是过台体测试
			//if(((FileID>>20)==0x136)||((FileID>>20)==0x101))
			if(((FileID>>24)==0x13)||((FileID>>24)==0x10))
			{//曲线或实时数据
				pRSD++;
				
			#if(USER/100)==17//
			if(Terminal_Router->MinuteCollect==1)
			//if(1)
			{
				YYMD=(pRSD[0]<<16)|(pRSD[1]<<24)|(pRSD[2]<<8)|(pRSD[3]);
				YYMD1=YYMD;
				//i=YYMD_Sub1D_hex(YYMD);//年年月日减1日,返回减1后的年年月日
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
				//符合召测日冻结的要求
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
				{//不相同
					return 0;
				}
				pRSD+=7;
				i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(i!=2)
				{//时间>=结束值
					return 0;
				}
			}
			}
			else
			{//日或月冻结
				//if((FileID>>20)==0x123)
				if((FileID>>24)==0x12)
				{//日冻结
					pRSD++;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,4);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=0)
					{//年月日不同
						return 0;
					}
				}
			#if(USER/100)==17//江苏系
				else if((FileID>>24)==0x1F)//结算日冻结
				{
					pRSD++;
					i=(pRSD[0]<<16)|(pRSD[1]<<24)|(pRSD[2]<<8)|(pRSD[3]);
					i=YYMD_Add1D_hex(i);//年年月日减1日,返回减1后的年年月日
					n=(file->save_date_time_s[0]<<16)|(file->save_date_time_s[1]<<24)|(file->save_date_time_s[2]<<8)|(file->save_date_time_s[3]);
					if(i!=n)
					{
						return 0;
					}
				}
			#endif
				else
				{//月冻结
					pRSD++;
					i=Compare_DL698DataString(file->save_date_time_s,pRSD,3);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
					if(i!=0)
					{//年月不同
						return 0;
					}
				}
			}
//		#else
//			pRSD++;
//			i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//			if(i!=0)
//			{//不相同
//				return 0;
//			}
//			pRSD+=7;
//			i=Compare_DL698DataString(file->save_date_time_s,pRSD,6);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//			if(i!=2)
//			{//时间>=结束值
//				return 0;
//			}
//		#endif
			break;
		case 8:
//Selector8∷=SEQUENCE
//{
//  采集成功时间起始值  date_time_s，
//  采集成功时间结束值  date_time_s，
//  时间间隔            TI，
//  电能表集合          MS
//}
			if((file->FILEID&0xff)!=0)
			{//文件分类!=0
				return 0;
			}
			//取7byte 采集成功时间  date_time_s(无数据类型)
//			ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+7,(u8*)ADDR_DATABUFF,7);
//			p8=(u8*)ADDR_DATABUFF;
//			pRSD++;
//			i=(pRSD[7+7+1]<<8)+pRSD[7+7+2];
//			if(i==0)
//			{//特例:TI=0;忇议一致性测试时需至少返回1个数据
//				i=Compare_DL698DataString(p8,pRSD,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//				if(i==2)
//				{//采集成功时间<起始值
//					return 0;
//				}
//				i=Compare_DL698DataString(p8,pRSD+7,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//				if(i!=2)
//				{//采集成功时间>=结束值
//					return 0;
//				}
//				break;
//			}
//			switch(pRSD[7+7])
//			{
//				case 0://  秒      （0），
//					n=7;
//					break;
//				case 1://分      （1），
//					n=6;
//					break;
//				case 2://时      （2），
//					n=5;
//					break;
//				case 3://日      （3），
//					n=4;
//					break;
//				case 4://月      （4），
//					n=3;
//					break;
//				case 5://年      （5）
//					n=2;
//					break;
//				default:
//					return 0;
//			}
//			i=Compare_DL698DataString(p8,pRSD,n);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//			if(i!=0)
//			{//不相同
//				return 0;
//			}
//			i=Compare_DL698DataString(p8,pRSD+7,n);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
//			if(i!=2)
//			{//采集成功时间>=结束值
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
			{//没找到
				return 0;
			}
			i=RecordFileHead->LEN_DATA;
			if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
			{
				return 0;
			}
			ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);
			p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+7;//7byte 采集成功时间
		
			pRSD++;
			i=(pRSD[7+7+1]<<8)+pRSD[7+7+2];
			if(i==0)
			{//特例:TI=0;忇议一致性测试时需至少返回1个数据
				i=Compare_DL698DataString(p8,pRSD,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(i==2)
				{//采集成功时间<起始值
					return 0;
				}
				i=Compare_DL698DataString(p8,pRSD+7,7);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
				if(i!=2)
				{//采集成功时间>=结束值
					return 0;
				}
				i=Get_RecordFileData_RCSD(pRCSD,(u8*)&RecordFileHead->Addr0,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//记录表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pTSA,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
				return i;
			}
			switch(pRSD[7+7])
			{
				case 0://  秒      （0），
					n=7;
					break;
				case 1://分      （1），
					n=6;
					break;
				case 2://时      （2），
					n=5;
					break;
				case 3://日      （3），
					n=4;
					break;
				case 4://月      （4），
					n=3;
					break;
				case 5://年      （5）
					n=2;
					break;
				default:
					return 0;
			}
			i=Compare_DL698DataString(p8,pRSD,n);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i!=0)
			{//不相同
				return 0;
			}
			i=Compare_DL698DataString(p8,pRSD+7,n);//数据串比较(高位在低字节),p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
			if(i!=2)
			{//采集成功时间>=结束值
				return 0;
			}
			i=Get_RecordFileData_RCSD(pRCSD,(u8*)&RecordFileHead->Addr0,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//记录表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pTSA,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
			return i;
			
		case 9:
//Selector9∷=SEQUENCE
//{
//  上第n次记录  unsigned
//}
			return 0;
		case 10:
//Selector10∷=SEQUENCE
//{
//上n条记录  unsigned，
//电能表集合  MS
//}
			break;//查找先用RCSDtoID，只要找到文件就有效
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
	{//没找到
		return 0;
	}
	i=RecordFileHead->LEN_DATA;
	if(i>(LEN_128KDATABUFF-(2+NUM_RMmax*sizeof(RecordFileHead_TypeDef))))
	{
		return 0;
	}
	ms->msfile_read(filename,(NUM_RMmax*sizeof(RecordFileHead_TypeDef))+offset,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i);
	if(YYMD1&&Terminal_Router->MinuteCollect==1)
	//if(YYMD1)//江苏会用到这个变量
	{
		p8=(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef));
		#ifdef DEBUG_JSON
		for(i=0;i<600;i++)
			myprintf("%02x ", p8[i], 0, 0);
		myprintf("\n",0,0,0);
		#endif
		Find_And_CreateNextData((u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),i,FileID,YYMD1,pAddr);	
		i=Get_RecordFileData_RCSD(pRCSD,(u8*)pAddr,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//记录表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pTSA,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
		return i;
	}
	i=Get_RecordFileData_RCSD(pRCSD,(u8*)&RecordFileHead->Addr0,(u8*)ADDR_128KDATABUFF+2+(NUM_RMmax*sizeof(RecordFileHead_TypeDef)),pOUT,LEN_OUTmax,ALLDATANULLSEL);//记录表的列选择;入口:pRCSD指向行选择的RCSD(无数据类型),pTSA,pRecord指向每只表记录地址和数据块开始,p8out指向数据发送缓冲;出口;记录选择数据的长度,0=没记录或错误
	return i;
}

u32 GET_RecordFileData(u32 FileID,u32 FileCount,u16* pAddr,u8* pRSD,u8* pRCSD,u8* pOUT,u32 LEN_OUTmax)//謨战輫录蟿菥私邼;色酄?pRSDbpRCSD指胁迵藭謩RSDbRCSD(蠟私邼`褝)矛=0噎示一跇注;椎軜:讈远謩私邼婴讏,0=蠟私邼;使詢ADDR_128KDATABUFF
{
	u32 LEN;

	WWDT_Enable_Feed(WDTTimerOutVal);//諍循叽膮俜蹗喂俜
	Comm_Ram->msFILESearchCount=0;
	if(Terminal_Router->MinuteCollect==1)
	//if(1)
	{
		while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH))
		{
			LEN=Get_RecordFileData_Fixed(FileID,FileCount,pAddr,pRSD,pRCSD,pOUT,LEN_OUTmax);//謨战輫录蟿菥私邼
			if(LEN)
			{
				return LEN;
			}
		}
	}else
	{
		while(Comm_Ram->msFILESearchCount<(recordFILENAMEmax_FLASH+recordFILENAMEmax_RAM))
		{
			LEN=Get_RecordFileData_Fixed(FileID,FileCount,pAddr,pRSD,pRCSD,pOUT,LEN_OUTmax);//謨战輫录蟿菥私邼
			if(LEN)
			{
				return LEN;
			}
		}
	}	
	
	return 0;
}



u32 DEPTH_RecordFile(u32 PORTn)//记录文件存储深度;返回:存储深度
{
	u32 no;
	u8 *p8;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	no=UARTCtrl->TaskID&0xff;//方案号
	switch((UARTCtrl->TaskID>>24)&0xf)//方案类型
	{
		case 1://普通采集方案 （1）
			if(no>=NUMmax_6014)
			{
				no=0;
			}
			p8=(u8*)ADDR_6014_SDRAM+(no*LENper_6014);
			p8=Get_Element(p8,2,0,LENper_6014);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8)
			{
				return (p8[1]<<8)+p8[2];
			}
			return 0;
		case 2://事件采集方案 （2）
			if(no>=NUMmax_6016)
			{
				no=0;
			}
			p8=(u8*)ADDR_6016_SDRAM+(no*LENper_6016);
			p8=Get_Element(p8,5,0,LENper_6016);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8)
			{
				return (p8[1]<<8)+p8[2];
			}
			return 0;
		case 3://透明方案     （3）
			if(no>=NUMmax_6018)
			{
				no=0;
			}
			p8=(u8*)ADDR_6018+(no*LENper_6018);
			p8=Get_Element(p8,4,0,LENper_6018);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8)
			{
				return (p8[1]<<8)+p8[2];
			}
			return 0;
		case 4://上报方案     （4）
		case 5://脚本方案     （5）
			return 0;
		default:
			return 0;
	}
}

void CountInc_RecordFile(u32 PORTn)//当有文件计数=0时,全部记录文件文件计数+1,删除达到记录深度的文件
{
	u32 i;
	u32 FileID;
	u32 set_depth;
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	
	FileID=Get_FileID(PORTn);//UART各任务的FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
	ms=(ms_Type *)Get_ms();
	set_depth=DEPTH_RecordFile(PORTn);//记录文件存储深度;入口:TaskID=UARTCtrl->TaskID;返回:存储深度
	if(set_depth<=1)
	{
		set_depth=2;//最小值
	}
	if(set_depth>0xfffe)
	{
		set_depth=0xfffe;//最大值
	}
	set_depth--;
	Comm_Ram->msFILESearchCount=0;
	while(1)
	{
		filename=Get_RecordFileName(FileID,0,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
		if(filename==0xffffffff)
		{//没找到
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
	//有计数=0的文件
	Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
	Comm_Ram->msFILESearchCount=0;
	while(1)
	{
		filename=Get_RecordFileName(FileID,0xffffffff,0);//搜索文件名:入口:FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类,相应域值为0xFF表示不关注,FileCount文件计数=0xFFFFFFFF表示不关注,pTime数据存储时标=0表示不关注;返回:0--(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)表示文件在RAM,0xffffffff表示没找到
		if(filename==0xffffffff)
		{//没找到
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

u32 AutoDelete_ErrID_RecordFile(void)//自动删除任务已不存在的记录文件;返回:0=没删除,1=有删除
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
			switch(FileID&0xff)//文件分类
			{
				case 1://1.数据冻结
				case 2://2.事件记录
				case 3://3.所有搜表结果
				case 4://4.跨台区搜表结果
				case 5://5.DL645电能表主动上报
				default:
					file++;
					continue;
				case 0://普通采集方案记录,全事件采集方案记录
					i=FileID>>28;
					if(i==1)
					{//1=普通采集方案
						FileID>>=12;
						for(id=0;id<NUMmax_6012_2;id++)
						{
							i=IDtoFileID(id);//入口:任务号id,返回:文件存储FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类;无效id返回=0
							i>>=12;
							if(i==FileID)
							{
								break;
							}
						}
						if(id>=NUMmax_6012_2)
						{//没相关任务
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
					{//全事件采集方案记录,任务号相关
					#if FileID_2xx==0//0=否,1=是;方案类型_采集类型_存储时标类型=2xx(事件采集方案)文件标识区分任务ID号
						#error
					#endif
//#define MeterAutoReportTaskID     0x02300000;//电表主动上报约定任务ID:b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//FILEID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
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
						FileID&=0xff;//任务号
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
//交采数据,1分钟,15分钟曲线数据就保存一天,日冻结保存3天,月冻结2个月

//一共有1683个时间点 1分钟1440,15分钟96,日冻结3,月冻结2
//1440+96+3+2=1541	//一天一共需要那么多个点保存分钟冻结 //因为要过第二天打包 再加70个点 1541+70=1611
//1683-1611 =72    //一天剩下那么多个点
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
	//根据FileID来删除不同的文件
	switch(FileID)
	{
		case 0x13f12000://实时的文件打包完之后会删除
		case 0x13f17000:
			break;
		case 0x13f16000://日冻结
		case 0x13f11000:
			YYMD1=0;
			YYMD1=(JSON->Lastrealtime[0]<<8)|(JSON->Lastrealtime[1]<<16)|(JSON->Lastrealtime[2]);
			YYMD2=0;
			YYMD2=(ptime[0]<<8)|(ptime[1]<<16)|(ptime[2]);
			if(YYMD1>YYMD2)//前面的时间比后面的时间大就说明已经过了一个月了,这个文件要删除
			{
				ret=1;
			}
		break;
		case 0x13f18000://月冻结
		case 0x13f13000:
			//YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<8)|JSON->Lastrealtime[2];
			//YYMD1=YYM_SubM_hex(YYMD1,12);//年年月日减1日,返回减1后的年年月日
			//YYMD2=(ptime[0]<<16)|(ptime[1]<<8)|ptime[2];
			YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<8);
			//YYMD1=YYM_SubM_hex(YYMD1,12);//年年月日减1日,返回减1后的年年月日
			YYMD2=(ptime[0]<<16)|(ptime[1]<<8);
			if(YYMD1>YYMD2)//年份大于文件年份就删除
			{
				ret=1;
			}
		break;
		case 0x13f19000://15分钟冻结
			YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|JSON->Lastrealtime[3];
			YYMD2=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|ptime[3];
			
			if((JSON->Lastrealtime[4]*60+JSON->Lastrealtime[5])<=(60+10))//如果是在1点10分之后那就删除,不然不删除因为还没有打包
			{
				YYMD1=YYMD_Sub1D_hex(YYMD1);//年年月日减1日,返回减1后的年年月日
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
		case 0x13f22000://1分钟冻结
			YYMD1=(JSON->Lastrealtime[0]<<16)|(JSON->Lastrealtime[1]<<24)|(JSON->Lastrealtime[2]<<8)|JSON->Lastrealtime[3];
			YYMD2=(ptime[0]<<16)|(ptime[1]<<24)|(ptime[2]<<8)|ptime[3];
			
			if((JSON->Lastrealtime[4]*60+JSON->Lastrealtime[5])<=(60+10))//如果是在1点10分之后那就删除,不然不删除因为还没有打包
			{
				YYMD1=YYMD_Sub1D_hex(YYMD1);//年年月日减1日,返回减1后的年年月日
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
		case 3://搜表结果不删除
		case 4:
			break;
		default:
			ret=1;//其他的什么杂七杂八的都删除
		break;
	}
	return ret;
}

u32 AutoDeleteFile_SeletFileID(u32 FileID)
{
	u32 ret;
	ret=0;
	//根据FileID来删除不同的文件
	switch(FileID)
	{
		case 0x13f12000:
		case 0x13f17000:
		case 0x13f16000://日冻结
		case 0x13f11000:
		case 0x13f18000://月冻结
		case 0x13f13000:
		case 0x13f19000://15分钟冻结
		case 0x13f22000://1分钟冻结
		case 3://搜表结果不删除
		case 4:
			break;
		default:
			ret=1;//其他的什么杂七杂八的都删除
		break;
	}
	return ret;
}

u32 AutoDeleteFile_RealData(u32 FileID)
{
	u32 ret;
	ret=0;
	//根据FileID来删除不同的文件
	switch(FileID)
	{
		case 0x13f12000:
		case 0x13f17000:
			ret=1;//其他的什么杂七杂八的都删除
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
		case 0x13f16000://日冻结
		case 0x13f11000:
		case 0x13f18000://月冻结
		case 0x13f13000:
		case 0x13f19000://15分钟冻结
		case 0x13f22000://1分钟冻结
		case 0x13f12000:
		case 0x13f17000:
			ret=1;
			break;
	}
	return ret;
}

u32 AutoDeleteFile(u32 filename)//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
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
	if(Terminal_Router->MinuteCollect==1)//载波那边决定这个值
	{
		//先删除不是江苏需要的数据
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
		//再删除按照时间排序数据
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
		//再删除实时数据
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
	
		return 0;//返回:0=没删除,1=有删除
	}else
	{
		//查当前最大的文件计数
		file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
		for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
		{
			if(file->BLOCK_ENTRY)
			{
				i=file->FILECOUNT;
				FileID=file->FILEID;
				#if(USER/100)==17//江苏
				if(AutoDelet_JSData(FileID))//Flash里面还有没有原来江苏的数据
				{
					ms->msfile_Delete(filename);
					return 1;//有删除
				}
				#endif
				//if((FileID>>20)==0x136)
				if((FileID>>24)==0x13)
				{//曲线文件
				#if (USER/100)==9//河南每5分钟1个点
					if(i<=(3*96*3))//至少3天
					{
						i=0;
					}
				#else
					if(i<=(3*96))//至少3天
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
//删除最大文件计数的文件
		if(FileCount<=1)
		{//最小计数1
			return 0;//返回:0=没删除,1=有删除
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
				{//曲线文件
				#if (USER/100)==9//河南每5分钟1个点
					if(i<=(3*96*3))//至少3天
					{
						i=0;
					}
				#else
					if(i<=(3*96))//至少3天
					{
						i=0;
					}
				#endif
				}
				if(i>=FileCount)
				{
					ms=Get_ms();
					ms->msfile_Delete(filename);
					return 1;//返回:0=没删除,1=有删除
				}
			}
			file++;
		}
		return 0;//返回:0=没删除,1=有删除
	}
	}
	else
	{//RAM
//查当前最大的文件计数
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
//删除最大文件计数的文件
		if(FileCount<=1)
		{//最小计数1
			return 0;//返回:0=没删除,1=有删除
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
					return 1;//返回:0=没删除,1=有删除
				}
			}
			file++;
		}
		return 0;//返回:0=没删除,1=有删除
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
	myprintf("[%s:%d]:文件最大存储空间recordFILENAMEmax_FLASH=%d\n",(u32)&__func__,(u32)__LINE__,recordFILENAMEmax_FLASH);
	#endif
	file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
	for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
	{
		if(file->BLOCK_ENTRY)
		{
			FileID=file->FILEID;
			switch(FileID)
			{
				case 0x13f05000://交采数据
					SLFDATA++;
				break;
				case 0x13f16000://日冻结
				case 0x13f11000:
					FrzDAY++;
				break;
				case 0x13f18000://月冻结
				case 0x13f13000:
					FrzMON++;
				break;
				case 0x13f19000://15分钟冻结
					Frz15Min++;
				break;
				case 0x13f22000://1分钟冻结
					Frz1Min++;
				break;
				case 0x13f12000:
				case 0x13f17000:
					RealMIN++;
					break;
				default:
					NotFrz++;//其他的什么杂七杂八的都删除
				break;
			}
		}
		file++;
	}
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:交采数据=%d \n\n",(u32)&__func__,(u32)__LINE__,SLFDATA);
	myprintf("[%s:%d]:日冻结=%d \n\n",(u32)&__func__,(u32)__LINE__,FrzDAY);
	myprintf("[%s:%d]:月冻结=%d \n\n",(u32)&__func__,(u32)__LINE__,FrzMON);
	myprintf("[%s:%d]:15分钟=%d \n\n",(u32)&__func__,(u32)__LINE__,Frz15Min);
	myprintf("[%s:%d]:1分钟=%d \n\n",(u32)&__func__,(u32)__LINE__,Frz1Min);
	myprintf("[%s:%d]:实时冻结数据=%d \n\n ",(u32)&__func__,(u32)__LINE__,RealMIN);
	myprintf("[%s:%d]:不是江苏需要的数据=%d \n\n ",(u32)&__func__,(u32)__LINE__,NotFrz);
	#endif
}


#if(USER/100)==15//安徽
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
			x=pDATA[Offset+6];//关联OAD个数
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
u8* Get_RMOADDATA(u32 OAD,u8* pDATA,bool bInsertData)//得到抄表数据中指定的OAD数据指针,入口:pDATA=抄表数据;返回:0=没找到
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


	//计算记录数据开始指针
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
			x=pDATA[Offset+6];//关联OAD个数
			x*=4;
			x+=7;
			Offset+=x;
		}
	}
	pData=pDATA+Offset;	
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)

//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)
	//找相同OAD在记录中的OAD计数
	Offset=29+2+5;
	OADcount=0;//相同OAD在记录中的OAD计数
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
	{//没找到,将OAD添加到pDATA
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
			MWR(OAD,(u32)pDATA+Offset,4);//添加OAD
			if(l>4096)
			{
				l=4096;
			}
			MR((u32)pDATA+Offset+4,(u32)buff,l);//添加原先的内容
			
			//pDATA[Offset+5+l]=0;//日电压合格率暂定为NULL
			pDATA[6]=(l+4+Offset-8)&0xff;
			pDATA[7]=(l+4+Offset-8)>>8;
		}
		else
			return 0;

	}
	Len=0;//数据偏移
	while(OADcount--)
	{
		Len+=Get_DL698DataLen_S(pData+Len,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
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
	u32 f;//当前越限标志
	u32 t;
	u32 x;
	u8 *p8;
	u8 *p8Data;
	u8 *p8Addr;
	u32 limitH;//电压考核上限
	u32 limitL;//电压考核下限
	u32 standardH;//电压合格上限
	u32 standardL;//电压合格下限

	t=Comm_Ram->DL698YMDHMS[3];//日
	t--;
	MR(ADDR_DATABUFF,ADDR_4030,LENmax_4030);
	p8=(u8*)ADDR_DATABUFF;
	limitH=(p8[3]<<8)|p8[4];//电压考核上限
	limitL=(p8[6]<<8)|p8[7];//电压考核下限
	standardH=(p8[9]<<8)|p8[10];//电压合格上限
	standardL=(p8[12]<<8)|p8[13];//电压合格下限
	
	
	
	for(i=0;i<3;i++)
	{
//初始化数据结构
		p8=(u8*)ADDR_2131_OneDay+(LENmax_2131_OneDay*i);
		MR(ADDR_DATABUFF,(u32)pbuf,6);//获取电表通信地址;
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
		p8[5]=5;//通信地址长度
			
//属性2（电压合格率数据）∷=structure
		p8[0]=DataType_structure;
		p8[1]=2;
//{
//  当日电压合格率         电压合格率，
//电压合格率∷=structure
		p8[2]=DataType_structure;
		p8[3]=7;
//{
// 存储采集到数据的电表通信地址  octet-string
		p8[4]=DataType_octet_string;
		p8[5]=6;
// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
		p8[12]=DataType_double_long_unsigned;
//电压合格率   long-unsigned（单位：%，换算：-2），
		p8[17]=DataType_long_unsigned;
//电压超限率   long-unsigned（单位：%，换算：-2），
		p8[20]=DataType_long_unsigned;
//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
		p8[23]=DataType_double_long_unsigned;
//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
		p8[28]=DataType_double_long_unsigned;
		// 上次电压监测时间  double-long-unsigned（单位：分钟，无换算），
		p8[33]=DataType_double_long_unsigned;
//}
//  当月电压合格率         电压合格率
//电压合格率∷=structure
		p8[38]=0;//当月电压合格率为空
		
		MR(ADDR_DATABUFF,(u32)pbuf+29+11+3*i,2);//获取对应电压数据
		p8Data = (u8*)ADDR_DATABUFF;//取任务读取的电压
		a=(p8Data[0]<<8)|p8Data[1];
		if(a>=limitH)//电压考核上限
		{
			f=1;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
		}
		else
		{
			if(a>=standardH)//电压合格上限
			{
				f=2;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
			}
			else
			{
				if(a<=limitL)//电压考核下限
				{
					f=5;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
				}
				else
				{
					if(a<=standardL)//电压合格下限
					{
						f=4;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
					}
					else
					{
						f=3;//电压当前越限标志:0=空,1=越考核上限,2=越合格上限,3=合格,4=越合格下限,5=越考核下限
					}
				}
			}
		}
		a=Comm_Ram->DL698YMDHMS[4];//时
		a*=60;
		a+=Comm_Ram->DL698YMDHMS[5];//分
		//当日电压监测时间
		
		MR(ADDR_DATABUFF,(u32)ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48,48);//获取对应电压数据
		//p8t=(u8*)ADDR_DATABUFF;
		//b=R_Inverse(a,4);
		MR((u32)p8Data,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+34,4);
		b=(p8Data[0]<<24)|(p8Data[1]<<16)|(p8Data[2]<<8)|(p8Data[3]);
		if(b==0)//表示日电压合格率计算结果清空后
		{
			b=1;
		}
		else	
		{
//			if(a<b)
//			{
//				c=0;
//				c=R_Inverse(c,4);
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+3+8,4);//电压监测时间清零
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);//电压超上限时间清零
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);//电压超下限时间清零
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+8+8,2);//电压合格率清零
//				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+11+8,2);//电压超限率清零
//				b=1;
//			}
//			else
			if(a>=b)
			{
				x=a-b;
				if(x<=5)//五分钟以内的差距，默认为中间的数据是丢了，超过五分钟，表示调节了系统时间
					b=x;
				else
					b=1;
			}
			else
				b=1;
		}
		a=R_Inverse(a,4);
		MWR(a,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+34,4);
		
		c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+3+8,4);//电压监测时间
		c=R_Inverse(c,4);
		c+=b;
		c=R_Inverse(c,4);
		MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+3+8,4);
		switch(f)
		{
			case 1://1=越考核上限
			case 2://2=越合格上限,
				//当日电压超上限时间
				c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);
				c=R_Inverse(c,4);
				c+=b;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);
				break;
			case 3://3=合格
				break;
			case 4://4=越合格下限,
			case 5://5=越考核下限
				//当日电压超下限时间
				c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);
				c=R_Inverse(c,4);
				c+=b;
				c=R_Inverse(c,4);
				MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);
				break;
		}
		//当日电压超限率
		//a=MRR(ADDR_2131_1+((i*31+t)*LENmax_2131_1+j*48)+2+3+8,4);//电压监测时间
		b=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+14+8,4);//电压超上限时间
		c=MRR(ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+19+8,4);//电压超下限时间
		a=1440;
		b=R_Inverse(b,4);
		c=R_Inverse(c,4);
		b+=c;
		if(b>a)
		{
			b=a;
		}
		b*=10000;//%二位小数
		c=b/a;//超限率
		b=10000-c;//合格率
		b=R_Inverse(b,2);
		c=R_Inverse(c,2);
		MWR(b,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+8+8,2);//电压合格率
		MWR(c,ADDR_2131_OneDay+i*LENmax_2131_OneDay+j*48+2+11+8,2);//电压超限率
		
		
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
		MR(ADDR_DATABUFF,(u32)pbuf,6);//获取电表通信地址;
		MR(ADDR_DATABUFF+6,(u32)pbuf+22,7);//存储时标，保存日冻结当天最后一分钟的时间即23:59;
		p8=(u8*)ADDR_DATABUFF;
		day=p8[6+3];//表示具体哪一天的冻结
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
		
		if(iSame==0||p8[0]==0)//电压合格率未产生记录,将当天的数据记录为全部合格
		{
			p8[0]=DataType_structure;
			p8[1]=2;
		//{
		//  当日电压合格率         电压合格率，
		//电压合格率∷=structure
			p8[2]=DataType_structure;
			p8[3]=5;

		// 电压监测时间  double-long-unsigned（单位：分钟，无换算），
			p8[4]=DataType_double_long_unsigned;
			a=0;
			a=R_Inverse(a,4);
			MWR(a,(u32)p8+5,4);
			
		//电压合格率   long-unsigned（单位：%，换算：-2），
			p8[9]=DataType_long_unsigned;
			a=10000;
			a=R_Inverse(a,2);
			MWR(a,(u32)p8+10,2);
		//电压超限率   long-unsigned（单位：%，换算：-2），
			p8[12]=DataType_long_unsigned;
			a=0;
			a=R_Inverse(a,2);
			MWR(a,(u32)p8+13,2);
		//电压超上限时间  double-long-unsigned（单位：分钟，无换算），
			p8[15]=DataType_double_long_unsigned;
			a=0;
			a=R_Inverse(a,4);
			MWR(a,(u32)p8+16,4);
		//电压超下限时间  double-long-unsigned（单位：分钟，无换算）
			p8[20]=DataType_double_long_unsigned;
			a=0;
			a=R_Inverse(a,4);
			MWR(a,(u32)p8+21,4);
			p8[25]=0;//当月电压合格率为空
			
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
	i&=0xff;//任务号

	if(PORTn!=ACSAMPLEPORT)
	{
		return ;
	}
	Comm_Ram->msFILESearchCount=0;

	if((FileID&0x0f)==1)//不是15分钟交采数据
	{
		FileID=0x13f12000;//1分钟打包的那个文件  15分钟数据	
		filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+8);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFFFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
	}else
	{
		filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+22);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFFFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
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
		y=file->NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
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
		{//地址已存在
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				;
			}else
			{
				return;//补抄曲线等数据时可能会重复,故不能删除文件
			}
		}
	}
	if(filename==0xffffffff)
	{//没找到
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
			{//没空
				i=AutoDeleteFile(0);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
				if(i)
				{//有删除
					continue;
				}
				return;
			}
			break;
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//空文件
		ms->msfile_lenclear(filename);//清0文件相关的长度次数,0-(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)表示文件在RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;
		file->LEN_DATA=0;
		file->NUM_DATA=0;
		if((FileID==0x13f12000)||(FileID==0x13f17000))
		{
			MW((u32)pbuf+8,(u32)&file->save_date_time_s,7);//采集存储时标
			MW((u32)pbuf+15,(u32)&file->start_date_time_s,7);//采集启动时间
		}else
		{
			MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//采集存储时标
			MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//采集启动时间
		}
	}
//写文件
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//错
		ms->msfile_Delete(filename);
		return;
	}
	//写数据
	if(filename>=FILENAMEmax_FLASH)
	{//RAM中文件
		i=ms->ms_malloc_sizemax();//最大可分配的尺寸(字节数)
		if(i<(1024*1024))
		{//空间<1M,保留至少1M空间用于记录数据读取
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y==0)
			{//没删除
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
		{//写错误
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y)
			{//有删除
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//写地址列表
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//数据数+1
	x++;
	file->NUM_DATA=x;
	#ifdef DEBUG_JSON
	myprintf("[%s:%d]:写入成功 \n",(u32)&__func__,(u32)__LINE__,0);
	#endif
}

//UART抄表数据缓冲定义
//#define LEN_RM_DI_CSD     512
//抄表数据缓冲1,最大总长度=LEN_RM_DI_CSD
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)

//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29  采集的CSD  array CSD(有数据类型)

//抄表数据缓冲2,最大总长度=LEN_UARTnFnDataBuff-LEN_RM_DI_CSD
//LEN_RM_DI_CSD   2byte 长度=已填写入buff但还没完整一个CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+2 2byte 长度=已填写入buff完整CSD的长度(不包括本身2字节)
//LEN_RM_DI_CSD+4 采集的数据(有数据类型)


//抄表结束后合并在数据缓冲1
//0 6byte 电能表地址
//6 2byte 长度=时标+已采集的CSD(不包括本身2字节)
//8 7byte 采集启动时标  date_time_s(无数据类型)
//15 7byte 采集成功时标  date_time_s(无数据类型)
//22 7byte 采集存储时标  date_time_s(无数据类型)
//29 采集的CSD  array CSD(有数据类型)
//n  采集的数据(有数据类型,全部数据为单独顺序排列无总数组)
void RM_RecordFile(u32 PORTn,u8 *pbuf)//抄表数据处理(存贮,事件)
{
	u32 i;
	u32 x;
	u32 y;
	u32 FileID;
	u32 EventID;//事件关联的ID
	u32 filename;
	msFILE_TypeDef *file;
	ms_Type *ms;
	RecordFileHead_TypeDef* RecordFileHead;
	u32 Addr0;
	u32 Addr1;
	u32 Addr2;
	u8* p8;
	UARTCtrl_TypeDef *UARTCtrl;
	#if (USER/100)==15//安徽
	u8* pDATA;
	#endif
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	//i=UARTCtrl->TaskID;
	//i>>=8;
	//i&=0xff;//任务号
#if(USER/100)==17//江苏系
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
				UARTCtrl->TaskID&=0xffff12ff;//一分钟
				break;
			case 13:
				UARTCtrl->TaskID|=0x0000ff00;
				UARTCtrl->TaskID&=0xffff17ff;//15分钟
			default:
				break;	
		}	
	}
	
	if((PORTn==ACSAMPLEPORT)&&(Terminal_Router->MinuteCollect==1))
	//if((PORTn==ACSAMPLEPORT))//交采
	{
		x=21;
		x+=8;
		x+=4;
		#ifdef DEBUG_JSON	
		myprintf("[%s:%d]:交采接受\n",(u32)__func__,(u32)__LINE__,0);
		for(i=0;i<300;i++)
		{	
			myprintf("%02x ",pbuf[i],0,0);
		}
		myprintf("\n",0,0,0);
		#endif
		i=(pbuf[x]<<24)|(pbuf[x+1]<<16)|(pbuf[x+2]<<8)|pbuf[x+3];
		#ifdef DEBUG_JSON
		myprintf("[%s:%d]:交采记录性OAD=%08x\n",(u32)&__func__,(u32)__LINE__,i);
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
				FileID=0x13f19000;//15分钟数据
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
		return;//如果是支持分钟采集,直接退出
	}
	
#endif
	i=UARTCtrl->TaskID;
	i>>=8;
	i&=0xff;//任务号

	switch(i)
	{
		case RMTASKnoPOWER://停上电事件固定任务号
			#if (USER/100)==15//安徽
			if(UARTCtrl->RMprotocol==3)
				//PowerEventTimerComp(pbuf+37);
				PowerEventTimerComp(pbuf+56);
			if(UARTCtrl->RMprotocol==2)
				PowerEventTimerComp(pbuf+58);
			#else
			PowerEventTimerComp(pbuf+37);//停上电事件时间比较,入口:pEVENT指向698格式事件的开始,出口:PowerEvent->bit
			#endif
			return;
		#if (USER/100)==15//安徽
		case 49://日电压合格率采集方案
			if(UARTCtrl->RMprotocol==2)
				CALC_MeterVoltage_Qualification_Rate(pbuf);
			return;
		case 1://日冻结采集数据项
			pDATA=GetROAD(0x50040200,pbuf);
			if(pDATA!=0&&UARTCtrl->RMprotocol==2)
			{
				MR(ADDR_DATABUFF,(u32)pbuf+15,7);//成功时标
				p8=(u8*)ADDR_DATABUFF;
				pDATA=Get_RMOADDATA(0x20210200,pbuf,false);
				x=Compare_DL698DataString(pDATA+1,p8,4);//电表的数据冻结时间和采集成功时间的日期不符，不保存电表的数据
				if(x!=0)
					return;
				Addr_Voltage_Qualification_Rate(pbuf);
			}
			else
			{
				EventID=Event_RMDATA(PORTn,pbuf+sizeof(RecordFileHead_TypeDef));//抄表数据相关的事件
				EventID=EventID;
				Update_Group_Total_Data_6000(pbuf);//更新总加组的数据,配置表是6000里的
			}
			break;
		case 5://单相表日冻结采集数据项
			pDATA=GetROAD(0x50040200,pbuf);
			if(pDATA!=0&&UARTCtrl->RMprotocol==2)
			{
				MR(ADDR_DATABUFF,(u32)pbuf+15,7);//成功时标
				p8=(u8*)ADDR_DATABUFF;
				pDATA=Get_RMOADDATA(0x20210200,pbuf,false);
				x=Compare_DL698DataString(pDATA+1,p8,4);//电表的数据冻结时间和采集成功时间的日期不符，不保存电表的数据
				if(x!=0)
					return;
			}
			EventID=Event_RMDATA(PORTn,pbuf+sizeof(RecordFileHead_TypeDef));//抄表数据相关的事件
			EventID=EventID;
			Update_Group_Total_Data_6000(pbuf);//更新总加组的数据,配置表是6000里的
		#endif
		default:
		#if(USER/100)==17//江苏系
			if((((UARTCtrl->TaskID>>20)&0xf)==5)&&(Terminal_Router->MinuteCollect==1))
				break;
		#endif
			EventID=Event_RMDATA(PORTn,pbuf+sizeof(RecordFileHead_TypeDef));//抄表数据相关的事件
			EventID=EventID;
			Update_Group_Total_Data_6000(pbuf);//更新总加组的数据,配置表是6000里的
			break;
	}

//TaskID=b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
//优先级:1=首要,2=必要,3=需要,4=可能
//方案类型:1=普通采集方案,2=事件采集方案,3=透明方案,4=上报方案,5=脚本方案
//普通采集方案的采集类型:0=采集当前数据,1=采集上第N次,2=按冻结时标采集,3=按时标间隔采集,4=补抄
//普通采集方案的存储时标:0=未定义,1=任务开始时间,2=相对当日0点0分,3=相对上日23点59分,4=相对上日0点0分,5=相对当月1日0点0分,6=数据冻结时标
	Comm_Ram->msFILEchange=0;//文件管理数据变化:0=变化,!=没变化
	ms=Get_ms();
	//UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	FileID=Get_FileID(PORTn);//UART各任务的FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
	Comm_Ram->msFILESearchCount=0;

	if(Terminal_Router->MinuteCollect==1)
	{
		if((FileID==0x13f12000)||(FileID==0x13f17000))
		{	
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+8);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFFFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
		}
		else
		{
			filename=Get_RecordFileName_JS(FileID,0xffffffff,pbuf+22);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFFFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
		}
	}
	else
	{
		filename=Get_RecordFileName(FileID,0xffffffff,pbuf+22);//虘谭蟿菥幕:色酄?FileID=b31-b28捉懈`褝,b27-b24訅墀`褝,b23-b20咋寓时要`褝,b19-b12蓭媳蹍,b11-b8讒酄氎?b7-b0,袪应圆值为0xF噎示一跇注,pTime私邼咋寓时要=0噎示一跇注;SearchCount=擢始虘谭謩蟿菥蹍0xFFFFFFFF噎示論蕪諑蕖烁蹍擢始虘谭;椎軜:0--(FILENAMEmax_FLASH-1)噎示蟿菥諝FLASH,FILENAMEmax_FLASH--(FILENAMEmax_RAM-1)噎示蟿菥諝RAM,0xffffffff噎示没謷战
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
		y=file->NUM_DATA;//文件存入数据个数计数(同抄表成功个数)
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
		{//地址已存在
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				;
			}else
			{
				return;//补抄曲线等数据时可能会重复,故不能删除文件
			}
		}
	}
	if(filename==0xffffffff)
	{//没找到
	#if(USER/100)==17//江苏系
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
				//CountInc_RecordFile(PORTn);//当有文件计数=0时,全部记录文件文件计数+1,删除达到记录深度的文件
				//AutoDelete_ErrID_RecordFile();//自动删除任务已不存在的记录文件;返回:0=没删除,1=有删除
				break;
		}
	#else
		CountInc_RecordFile(PORTn);//当有文件计数=0时,全部记录文件文件计数+1,删除达到记录深度的文件
		AutoDelete_ErrID_RecordFile();//自动删除任务已不存在的记录文件;返回:0=没删除,1=有删除
	#endif
		//找空文件
#if RecordFILEallinFLASH==0//全部普通抄表方案数据保存在nand flash:0=否(只抄OAD时存在RAM),1=是
	#if (USER/100)==0//电科院测试
		i=UARTCtrl->RMOADROAD;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
		if(i==1)//全部只抄OAD,RAM文件
	#else
		//FileID=b31-b28方案类型,b27-b24采集类型,b23-b20存储时标类型,b19-b12任务号,b11-b8端口号,b7-b0文件分类
		if(((FileID>>24)&0xff)==0x10)//1=普通采集方案,采集类型=0=采集当前数据
	#endif
		{
			//myprintf("[%s:%d]:删除((FileID>>24)&0xff)==0x10\n",(u32)&__func__,(u32)__LINE__,0);
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
				{//没空
					i=AutoDeleteFile(FILENAMEmax_FLASH);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
					if(i)
					{//有删除
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
		{//FLASH文件	
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
				{//没空
					i=AutoDeleteFile(0);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
					if(i)
					{//有删除
						continue;
					}
					return;
				}
				break;
			}
		}
	}
	if(file->BLOCK_ENTRY==0)
	{//空文件
		ms->msfile_lenclear(filename);//清0文件相关的长度次数,0-(FILENAMEmax_FLASH-1)表示文件在FLASH,FILENAMEmax_FLASH-(FILENAMEmax_RAM-1)表示文件在RAM
		file->FILEID=FileID;
		file->FILECOUNT=0;
		if(Terminal_Router->MinuteCollect==1)
		{
			file->LEN_DATA=0;
			file->NUM_DATA=0;
			if((FileID==0x13f12000)||(FileID==0x13f17000))
			{
				MW((u32)pbuf+8,(u32)&file->save_date_time_s,7);//采集存储时标
				MW((u32)pbuf+15,(u32)&file->start_date_time_s,7);//采集启动时间
			}else
			{
				MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//采集存储时标
				MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//采集启动时间
			}
		}
		else
		{
			MW((u32)pbuf+22,(u32)&file->save_date_time_s,7);//采集存储时标
			MW((u32)pbuf+8,(u32)&file->start_date_time_s,7);//采集启动时间
		}
	}
//写文件
	x=file->NUM_DATA;
	if(x>=NUM_RMmax)
	{//错
		ms->msfile_Delete(filename);
		return;
	}
	//写数据
	if(filename>=FILENAMEmax_FLASH)
	{//RAM中文件
		i=ms->ms_malloc_sizemax();//最大可分配的尺寸(字节数)
		if(i<(1024*1024))
		{//空间<1M,保留至少1M空间用于记录数据读取
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y==0)
			{//没删除
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
		{//写错误
			y=AutoDeleteFile(filename);//自动删除记录文件(自动减少存储深度);入口:写入文件时空间不够的文件名,以判别是RAM还是FLASH空间不够;返回:0=没删除,1=有删除
			if(y)
			{//有删除
				continue;
			}
			return;
		}
		break;
	}
	i+=y;
	file->LEN_DATA=i;
	
	//写地址列表
	ms->msfile_write(filename,x*sizeof(RecordFileHead_TypeDef),pbuf,sizeof(RecordFileHead_TypeDef));
	//数据数+1
	x++;
	file->NUM_DATA=x;
	
	//主动上报
	i=MRR(ADDR_4300_8+1,1);
	if(i==0)
	{//不允许主动上报
		for(i=0;i<NUMmax_601C;i++)
		{
		#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
			file->NUM_GPRSreport[i]=x;
			file->NUM_ENETreport[i]=x;
		#else
			file->NUM_GPRSorENETreport[i]=x;
		#endif
		}
	}
	else
	{//允许主动上报
		i=UARTCtrl->TaskID;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
		if(((i>>24)&0xf)==2)
		{//方案类型=事件采集方案
			i&=0xff;//方案号
			i*=LENper_6016;
			i+=ADDR_6016_SDRAM;
			p8=Get_Element((u8 *)i,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8)
			{
				if(p8[1]==0)//上报标识        bool（True：立即上报，False：不上报），
				{//不上报
					for(i=0;i<NUMmax_601C;i++)
					{
					#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
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
		
	#if ((USER/100)==15)//安微系
		i=UARTCtrl->TaskID;//当前采集任务ID;值0表示无采集任务;b31-b28优先级,b27-b24方案类型,b23-b20采集类型,b19-b16存储时标类型,b15-b8任务号,b7-b0方案编号
		i>>=8;
		i&=0xff;
		if(i==49||i==28||i==54)
		{//任务号49抄表仅用于电压合笴率计算不需上报
			for(i=0;i<NUMmax_601C;i++)
			{
			#if GPRS_ENET_REPORT==0//GPRS/ENET主动上报:0=上报分开,1=上报合并
				file->NUM_GPRSreport[i]=x;
				file->NUM_ENETreport[i]=x;
			#else
				file->NUM_GPRSorENETreport[i]=x;
			#endif
			}
		}
	#endif		
		//上报通道
		REPORT_PORTn(file);//通道不需上报时置已上报数据个数=当前数据个数；使用ADDR_DATABUFF
	}
	
}










