
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

u32 RM_Meter(u32 PORTn)//当前电能表计数是否要抄;入口:UARTCtrl->MeterCount[],UARTCtrl->TaskID;返回:0=不抄,1=要抄,2=完成,同时填写通信地址,波特率,抄表规约类型
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
	i=(id>>8)&0xff;//任务号
	if(i>=(RMTASKmax+ExtRMTASKmax))
	{
		return 0;
	}
	switch(i)//任务号
	{
		case RMTASKnoPOWER://停上电事件固定任务号
			id>>=8;
			id&=0xff;
			i=UARTCtrl->MeterCount[id];
			i=PowerEvent->setno[i];
			p8s=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
			p8s+=5;
			break;
	
		default:
			i=id&0xff;//方案编号
			x=id>>24;
			x&=0x0f;
			switch(x)//方案类型
			{
				case 1://普通采集方案
					i*=LENper_6014;
					i+=ADDR_6014_SDRAM;
					p8s=(u8*)i;
					if(p8s[0]!=DataType_structure)
					{
						return 0;
					}
					pMS=Get_Element(p8s,5,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(pMS==0)
					{
						return 0;
					}
					id>>=8;
					id&=0xff;
					pMS++;//指向数据类型后的字节
					i=Compare_SETNO_MS((u8*)ADDR_6000_SDRAM,UARTCtrl->MeterCount[id],pMS);//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
					if(i==0)
					{
						return 0;
					}
					p8s=(u8*)ADDR_6000_SDRAM+(UARTCtrl->MeterCount[id]*LENper_6000);
					p8s+=5;
					break;
				case 2://事件采集方案
					i*=LENper_6016;
					i+=ADDR_6016_SDRAM;
					p8s=(u8*)i;
					if(p8s[0]!=DataType_structure)
					{
						return 0;
					}
					pMS=Get_Element(p8s,3,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(pMS==0)
					{
						return 0;
					}
					id>>=8;
					id&=0xff;
					pMS++;//指向数据类型后的字节
					i=Compare_SETNO_MS((u8*)ADDR_6000_SDRAM,UARTCtrl->MeterCount[id],pMS);//比较配置序号是否符合MS集合,入口:pADDR_6000已在RAM,pMS已在RAM并指向数据类型后的字节;出口:0=不符合,1=符合
					if(i==0)
					{
						return 0;
					}
					p8s=(u8*)ADDR_6000_SDRAM+(UARTCtrl->MeterCount[id]*LENper_6000);
					p8s+=5;
					break;
				case 3://透明方案
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
					i=Get_DL698ByteTypeLenValue(p8s+5);//得到DL698字节类型长度单元的值
					if(UARTCtrl->MeterCount[id]>=i)
					{
						UARTCtrl->MeterCount[id]=NUM_RMmax;
						return 0;
					}
					p8s=Get_Element(p8s+4,UARTCtrl->MeterCount[id]+1,0,0);//计算元素地址,使用ADDR_128KDATABUFF
					if(p8s==0)
					{
						return 0;//返回:0=不抄,1=要抄,2=完成
					}
					//透明方案地址
					p8s+=1;
					//按通信地址找配置序号
					p8d=(u8*)ADDR_6000_SDRAM;
					for(i=0;i<NUMmax_6000;i++)
					{
						if(p8d[0]==DataType_structure)
						{
							//x=Compare_octet_string(p8s,p8d+8);//比较字节串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
							x=Compare_TSA_string(p8s,p8d+8);//比较TSA串,p1和p2已在RAM中,返回:0=相同,1=不相同p1>p2,2=不相同p1<p2
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
		//			UARTCtrl->MeterSetNo=i;//配置序号
					break;
				case 4://上报方案
					
				case 5://脚本方案
					break;
				default:
					break;
			}
			break;
	}
	//端口
	p8d=Get_Element(p8s,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
	if(p8d==0)
	{
		return 0;
	}
	x=(p8d[1]<<24)+(p8d[2]<<16)+(p8d[3]<<8)+p8d[4];//端口OAD
	x=OADtoPORTn(x);
	if(x!=PORTn)
	{
		return 0;
	}
	//通信地址
	p8s+=3;
	i=p8s[1];
	i++;//通信地址字节数
	if(i>=6)
	{
		i-=6;//只取最后字节
		Addr0=p8s[i+2]|(p8s[i+3]<<8);
		Addr1=p8s[i+4]|(p8s[i+5]<<8);
		Addr2=p8s[i+6]|(p8s[i+7]<<8);
	}
	else
	{
		switch(i)//通信地址字节数
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
	if(UARTCtrl->RMMeterCountAuto==0)//抄表电能表自动加1计数,0=自动,1=不自动(路由模块主动)
	{
		UARTCtrl->RMAddr[0]=Addr0;
		UARTCtrl->RMAddr[1]=Addr1;
		UARTCtrl->RMAddr[2]=Addr2;
	}
	x=UARTCtrl->NUMrmOK[id];//任务已采集完成数
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
					return 2;//返回:0=不抄,1=要抄,2=完成
				}
			}
		}
		rmOKFAILlist++;
	}
	x=p8s[0];
	x++;
	//波特率
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
			{//不是载波口
				UARTCtrl->BpsCtrl=(0xb+(i<<5));
			}
			break;
		case 255://自适应
		default:
			if(PORTn!=RS485_4PORT)
			{//不是载波口
				UARTCtrl->BpsCtrl=RS485DefaultBPS;//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
			}
			break;
	}
	//规约类型
	if(PORTn==ACSAMPLEPORT)
	{//内部交采
	#if iMeterProtocol==2//内部交采抄表规约:2=DL/T645-2007，3=DL/T698.45
		UARTCtrl->RMprotocol=2;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	#else
		UARTCtrl->RMprotocol=3;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	#endif
		return 1;//返回:0=不抄,1=要抄,2=完成
	}
	p8s+=2;
	i=p8s[0];
	UARTCtrl->RMprotocol=i;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
	return 1;//返回:0=不抄,1=要抄,2=完成
}

u8* RM_CSD(u32 PORTn)//取抄表数据标识CSD;返回:0=无效,1=CSD在URATCtrl,其他值=透明抄读帧数据指针
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
	i=(id>>8)&0xff;//任务号
	switch(i)
	{
		case RMTASKnoPOWER://停上电事件固定任务号
			UARTCtrl->RMCSDCountMax=1;//抄表CSD最大个数
			if(UARTCtrl->RMCSDSubCount==0)//抄表CSD子计数
			{
				UARTCtrl->RMCSD[0]=DataType_CSD;
			#if (USER/100)==15
				UARTCtrl->RMCSD[1]=1;//ROAD
				UARTCtrl->RMCSD[2]=0x30;
				UARTCtrl->RMCSD[3]=0x11;
				UARTCtrl->RMCSD[4]=0x02;
				UARTCtrl->RMCSD[5]=0x00;
				UARTCtrl->RMCSD[6]=5;//RCSD，SEQUENCE OF个数
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
				UARTCtrl->RMCSD[6]=0;//RCSD，SEQUENCE OF个数
			#endif
				
//				UARTCtrl->RMCSD[0]=DataType_CSD;
//				UARTCtrl->RMCSD[1]=1;//ROAD
//				UARTCtrl->RMCSD[2]=0x30;
//				UARTCtrl->RMCSD[3]=0x11;
//				UARTCtrl->RMCSD[4]=0x02;
//				UARTCtrl->RMCSD[5]=0x00
//				UARTCtrl->RMCSD[6]=0;//RCSD，SEQUENCE OF个数
				switch(UARTCtrl->RMprotocol)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
				{
					case 1://DL/T645-1997
						return 0;
					case 2://DL/T645-2007
						UARTCtrl->RM_DI=0x03110001;//(上1次)掉电发生时刻，结束时刻
						break;
					case 3://DL/T698.45（3）
						break;
					default://ABB,Landis
						return 0;
				}
				return (u8*)1;
			}
			return 0;
	}
	
	i=id&0xff;//方案编号
	switch((id>>24)&0xf)//方案类型
	{
		case 1://普通采集方案
			i*=LENper_6014;
			i+=ADDR_6014_SDRAM;
		#if (USER/100)==17//江苏系
			if(Terminal_Router->MinuteCollect==1&&PORTn!=ACSAMPLEPORT)
			{
				p8s=Get_Element((u8*)i,3,1,0);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8s)
				{
					if((p8s[0]!=0)&&(p8s[1]==0xf0))//按采集器点名方式采集
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
				p8s=Get_Element((u8*)i,3,1,0);//计算元素地址,使用ADDR_128KDATABUFF
				if(p8s)
				{
					if((p8s[0]!=0)&&(p8s[1]==0xf0))//按采集器点名方式采集
						return 0;
				}
			}
//				p8s=Get_Element((u8*)i,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
//				if(p8s==0)
//				{
//					return 0;//0=无效,1=CSD在URATCtrl,其他值=透明抄读帧数据指针
//				}
//				else
//				{
//					if(p8s[3]==1)//历史数据，只要用点抄即可
//					{
//						p16=(u16*)(p8s+4);
//						if(p16[0]!=0x0550)
//						 return 0;
//					}
//				}
//			}
//			else
//			{
//				p8s=Get_Element((u8*)i,3,1,0);//计算元素地址,使用ADDR_128KDATABUFF
//				if(p8s)
//				{
//					if((p8s[0]!=0)&&(p8s[1]==0xf0))//按采集器点名方式采集
//						return 0;
//				}
//			}
		#endif
			p8s=Get_Element((u8*)i,4,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;//0=无效,1=CSD在URATCtrl,其他值=透明抄读帧数据指针
			}
			i=UARTCtrl->RMCSDCount;//抄表CSD计数
			x=Get_DL698ByteTypeLenValue(p8s+1);//得到DL698字节类型长度单元的值
			UARTCtrl->RMCSDCountMax=x;//抄表CSD最大个数
			if(i>=x)
			{//抄表CSD计数>=数组数
				return 0;//0=无效,1=CSD在URATCtrl,其他值=透明抄读帧数据指针
			}
			p8s=Get_Element(p8s,i+1,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			LEN_CSD=Get_DL698DataLen(p8s,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
			LEN_CSD>>=8;
			if((LEN_CSD>sizeof(UARTCtrl->RMCSD))||(LEN_CSD<6))
			{
				return 0;
			}
			if(p8s[1]==0)//当抄表数据是OAD还是ROAD
			{//OAD
				UARTCtrl->RMOADROAD|=1;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
			}
			else
			{//ROAD
				UARTCtrl->RMOADROAD|=2;//抄表任务有抄过的OAD类型:b0=有OAD,b1=有ROAD,b3-b7保留;用于抄表数据存贮时是存在RAM还是NAND
			}
			x=(UARTCtrl->RMCSD[2]<<8)|UARTCtrl->RMCSD[3];
			MR((u32)&UARTCtrl->RMCSD,(u32)p8s,LEN_CSD);
			n=(p8s[2]<<8)|p8s[3];
			switch(n)
			{
				case 0x5002://分钟冻结,采曲线数据,因会补抄上96点故用先用无效时间没相同文件名使已抄数为0,在RM_TxDATA.c中会每一点检查是否已抄
				case 0x5004://日冻结
				case 0x5006://月冻结
//					if(x!=n)
//					{//OI变化
//						NUMrm(PORTn);//抄表数;出口:任务需抄数，完成数，非法完成数,读记录文件中已完成的地址表
//						//重装,NUMrm(PORTn)会修改为0xff
//						MR((u32)&UARTCtrl->RMCSD,(u32)p8s,LEN_CSD);
//					}
					//交流采样通信口先冻结后抄表
					if(PORTn==ACSAMPLEPORT)//交流采样通信口号(虚拟)
					{
						CONGEAL_Record();//冻结
					}
					break;
			}
			
			x=UARTCtrl->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
			switch(x)
			{
				default://未知
					return 0;
				case 1://DL/T645-1997
					//曲线抄读
					if(UARTCtrl->RMCSD[1]==1)
					{//ROAD
						OAD=UARTCtrl->RMCSD[2+0]<<24;
						OAD|=UARTCtrl->RMCSD[2+1]<<16;
						OAD|=UARTCtrl->RMCSD[2+2]<<8;
						OAD|=UARTCtrl->RMCSD[2+3];
						if(OAD==0x50020200)
						{//分钟冻结
							i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
							x=UARTCtrl->RMCSD[6];
							UARTCtrl->RMCSDSubCountMax=1;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
							UARTCtrl->RMROADCountMax=x;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
							if(i>=x)
							{//抄表CSD子计数>=数组数
								return 0;//抄完
							}
							OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
							switch(OAD)
							{
								case 0x20210200://1 冻结时标
									OAD=0xC011;//DI电能表日历时钟Hhmmss	
									break;
								case 0x00100201://2 正向有功总电能
									OAD=0x9010;//DL645-1997标识 返回数据格式：XXXXXX.XX
									break;
								case 0x00200201://3 反向有功总电能  
									OAD=0x9020;//DL645-1997标识 返回数据格式：XXXXXX.XX
									break;
								case 0x00300200://4 组合无功1电能
									OAD=0x911F;//DL645-1997标识 返回数据格式：XXXXXX.XX
									break;
								case 0x00400200://5 组合无功2电能
									OAD=0x912F;//DL645-1997标识 返回数据格式：XXXXXX.XX
									break;
								case 0x00500201://6 一象限无功总电能
									OAD=0x9130;//DL645-1997标识 返回数据格式：XXXXXX.XX
									break;
								case 0x00600201://7 二象限无功总电能 返回数据格式：XXXXXX.XX
									OAD=0x9150;//DL645-1997标识
									break;
								case 0x00700201://8 三象限无功总电能 返回数据格式：XXXXXX.XX
									OAD=0x9160;//DL645-1997标识
									break;
								case 0x00800201://9 四象限无功总电能 返回数据格式：XXXXXX.XX
									OAD=0x9140;//DL645-1997标识
									break;
								case 0x20170200://10 有功需量
									return 0;
								case 0x20180200://11 无功需量
									return 0;
								case 0x20000200://12 电压
									UARTCtrl->RMCSDSubCountMax=3;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
									switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
									{
										case 0://A相电压
											OAD=0xB611;//DL645-1997标识 返回数据格式：XXX
											break;
										case 1://B相电压
											OAD=0xB612;//DL645-1997标识 返回数据格式：XXX
											break;
										case 2://C相电压
											OAD=0xB613;//DL645-1997标识 返回数据格式：XXX
											break;										
									}
									break;
								case 0x20000201://12 A相电压
									OAD=0xB611;//DL645-1997标识 返回数据格式：XXX
									break;
								case 0x20010200://13 电流
									UARTCtrl->RMCSDSubCountMax=3;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
									switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
									{
										case 0://A相电流
											OAD=0xB621;//DL645-1997标识 返回数据格式：XX.XX
											break;
										case 1://B相电流
											OAD=0xB622;//DL645-1997标识 返回数据格式：XX.XX
											break;
										case 2://C相电流
											OAD=0xB623;//DL645-1997标识 返回数据格式：XX.XX
											break;										
									}
									break;
								case 0x20010201://13 A相电流
									OAD=0xB621;//DL645-1997标识 返回数据格式：XX.XX
									break;
								case 0x20010400://14 零线电流
									return 0;
								case 0x20040200://15 有功功率
									UARTCtrl->RMCSDSubCountMax=4;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
									switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
									{
										case 0://总有功功率
											OAD=0xB630;//DL645-1997标识 返回数据格式：XX.XXXX
											break;
										case 1://A相有功功率
											OAD=0xB631;//DL645-1997标识 返回数据格式：XX.XXXX
											break;
										case 2://B相有功功率
											OAD=0xB632;//DL645-1997标识 返回数据格式：XX.XXXX
											break;
										case 3://C相有功功率
											OAD=0xB633;//DL645-1997标识 返回数据格式：XX.XXXX
											break;										
									}
									break;									
								case 0x20040202://15 A相有功功率
									OAD=0xB631;//DL645-1997标识 返回数据格式：XX.XXXX
									break;
								case 0x20050200://16 无功功率
									UARTCtrl->RMCSDSubCountMax=4;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
									switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
									{
										case 0://总无功功率
											OAD=0xB640;//DL645-1997标识 返回数据格式：XX.XX
											break;
										case 1://A相无功功率
											OAD=0xB641;//DL645-1997标识 返回数据格式：XX.XX
											break;
										case 2://B相无功功率
											OAD=0xB642;//DL645-1997标识 返回数据格式：XX.XX
											break;
										case 3://C相无功功率
											OAD=0xB643;//DL645-1997标识 返回数据格式：XX.XX
											break;										
									}
									break;									
								case 0x20050202://16 A相无功功率
									OAD=0xB641;//DL645-1997标识 返回数据格式：XX.XX
									break;
								case 0x200A0200://17 功率因数
									UARTCtrl->RMCSDSubCountMax=4;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
									switch(UARTCtrl->RMCSDSubCount)//抄表CSD子计数(非DL698抄表时)
									{
										case 0://总功率因数
											OAD=0xB650;//DL645-1997标识 返回数据格式：X.XXX
											break;
										case 1://A相功率因数
											OAD=0xB651;//DL645-1997标识 返回数据格式：X.XXX
											break;
										case 2://B相功率因数
											OAD=0xB652;//DL645-1997标识 返回数据格式：X.XXX
											break;
										case 3://C相功率因数
											OAD=0xB653;//DL645-1997标识 返回数据格式：X.XXX
											break;										
									}
									break;																		
								case 0x200A0202://17 A相功率因数
									OAD=0xB651;//DL645-1997标识 返回数据格式：X.XXX
									break;
								case 0x11100201://18 冻结周期内正向有功最大需量 
									return 0;
								default:
									return 0;
							}
							i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
							if(i>=1)
							{//DL645-1997表曲线抄实时数据，只抄当前1个点
								return 0;//抄完
							}
							i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
							if(i>=UARTCtrl->RMCSDSubCountMax)
							{
								return 0;
							}
							UARTCtrl->RM_DI=OAD;
							return (u8*)1;
						}
					}
				
					//先找全匹配
		#if PARAMETER_601E==0//设置参数601E采集规则库使用:0=内部默认,1=可设置
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//得到采集规则库初始设置库列表个数
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
					{//采集规则库中找到全匹配
						Len=Get_DL698DataLen(p8d+2,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
						Len>>=8;
						p8d+=2+Len+4;
						//2007 主用DI
						x=p8d[1];//数组数
						x*=6;
						x+=2;
						p8d+=x;
						//2007 备用DI
						x=p8d[1];//数组数
						x*=6;
						x+=2;
						p8d+=x;
						p8d+=2;
						if(UARTCtrl->RM_DI_CHOICE!=0)//抄表DI选择:0=主DI,1=备用DI
						{//备用DI
							x=p8d[1];//数组数
							x*=4;
							x+=2;
							p8d+=x;
						}
						i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
						x=p8d[1];
						UARTCtrl->RMCSDSubCountMax=x;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
						UARTCtrl->RMROADCountMax=0;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
						if(i>=x)
						{//抄表CSD子计数>=数组数
							return 0;
						}
						p8d+=4+(i*4);
						i=(p8d[0]<<8)|p8d[1];
						UARTCtrl->RM_DI=i;
						return (u8*)1;
					}
				//再找单个OAD匹配
					if(UARTCtrl->RMCSD[1]==0)//当前正在抄的CSD(有数据类型)
					{//不是ROAD错
						return 0;
					}
					i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
					x=UARTCtrl->RMCSD[6];//ROAD个数
					UARTCtrl->RMROADCountMax=x;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
					if(i>=x)
					{//抄表ROAD计数>=数组数
						return 0;//抄完
					}
					OAD=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|UARTCtrl->RMCSD[5];
					i*=4;
					i+=7;
					ROAD=(UARTCtrl->RMCSD[i]<<24)|(UARTCtrl->RMCSD[i+1]<<16)|(UARTCtrl->RMCSD[i+2]<<8)|UARTCtrl->RMCSD[i+3];
		#if PARAMETER_601E==0//设置参数601E采集规则库使用:0=内部默认,1=可设置
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//得到采集规则库初始设置库列表个数
					for(i=0;i<n;i++)
					{
						p8d=(u8*)p32[0];
						if(p8d[0]==DataType_structure)
						{
							if(p8d[3]==1)
							{//是ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//是单个ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//找到
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
							{//是ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//是单个ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//找到
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
					Len=Get_DL698DataLen(p8d+2,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
					Len>>=8;
					p8d+=2+Len+4;
					//2007 主用DI
					x=p8d[1];//数组数
					x*=6;
					x+=2;
					p8d+=x;
					//2007 备用DI
					x=p8d[1];//数组数
					x*=6;
					x+=2;
					p8d+=x;
					p8d+=2;
					if(UARTCtrl->RM_DI_CHOICE!=0)//抄表DI选择:0=主DI,1=备用DI
					{//备用DI
						x=p8d[1];//数组数
						x*=4;
						x+=2;
						p8d+=x;
					}
					i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
					x=p8d[1];
					UARTCtrl->RMCSDSubCountMax=x;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
					if(i>=x)
					{//抄表CSD子计数>=数组数
						return 0;
					}
					p8d+=4+(i*4);
					i=(p8d[0]<<8)|p8d[1];
					UARTCtrl->RM_DI=i;
					return (u8*)1;
				case 2://DL/T645-2007
				case ProtocolNO_Landis://Landis(IEC1107 模式C)
//属性2∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	数据列选择描述符  CSD，
//	规则描述  structrue
//	{
//		AcqCmd_2007  structure
//		{
//			主用DI  array octet-string(SIZE(4))，
//			备用DI  array octet-string(SIZE(4))
//		}，
//		AcqCmd_1997  structure
//		{
//			主用DI  array octet-string(SIZE(2))，
//			备用DI  array octet-string(SIZE(2))
//		}，
//		AcqCmd_Trans  structure
//		{
//			Frame  octet-string
//		}
//	}
//}
				//曲线抄读
					if(UARTCtrl->RMCSD[1]==1)
					{//ROAD
						OAD=UARTCtrl->RMCSD[2+0]<<24;
						OAD|=UARTCtrl->RMCSD[2+1]<<16;
						OAD|=UARTCtrl->RMCSD[2+2]<<8;
						OAD|=UARTCtrl->RMCSD[2+3];
					#if(USER/100)==17//江苏系
						if((OAD==0x50020200)||(OAD==0x50050200))
					#else
						if(OAD==0x50020200)
					#endif
						{//分钟冻结
							i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
							if(i>=1)
							{
								return 0;
							}
							id=UARTCtrl->TaskID;
							id=(id>>8)&0xff;//任务号
							i=UARTCtrl->MeterCount[id];//取档案号
							p8d=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							p8d=Get_Element(p8d,2,8,0);//计算元素地址,使用ADDR_128KDATABUFF
							if(p8d==0)
							{
								return 0;
							}

							i=p8d[1];//取接线方式
#if (((USER/100)==9)||((USER/100)==17)) //如果是河南市场部分考核表没有分钟冻结数据，用抄实时数据转
              if(UARTCtrl->RMprotocol==2)
              {//如果是645-07协议
                x=UARTCtrl->MeterCount[id];//取档案号
                p8d=(u8*)ADDR_6000_SDRAM+(x*LENper_6000);
                p8d=Get_Element(p8d,2,4,0);//计算元素地址,使用ADDR_128KDATABUFF
                x=(p8d[1]<<24)|(p8d[2]<<16)|(p8d[3]<<8)|(p8d[4]);
                if(x==0xF2010201)
                {//RS485通道用抄实时数据替代抄曲线数据
                  i=1;
                }
              }
#endif
							if(UARTCtrl->RMprotocol==ProtocolNO_Landis)//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
							{//Landis表
								i=1;//Landis表看成单相表用实时数据代
							}
							if(i==1)//未知（0），单相（1），三相三线（2），三相四线（3）
							{//单相
								i=UARTCtrl->RM_DI_TI_Count;//按时标间隔采集时的间隔计数
								if(i>=1)
								{//单相表曲线抄实时数据，只抄当前1个点
									return 0;//抄完
								}
								i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
								x=UARTCtrl->RMCSD[6];
								UARTCtrl->RMCSDSubCountMax=1;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
								UARTCtrl->RMROADCountMax=x;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
								if(i>=x)
								{//抄表CSD子计数>=数组数
									return 0;//抄完
								}
								OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
								switch(OAD)
								{
									case 0x20210200://1 冻结时标
										OAD=0x04000102;//DI电能表日历时钟Hhmmss
										break;
									case 0x00100201://2 正向有功总电能
										OAD=0x00010000;//DL645-2007标识 返回数据格式：XXXXXX.XX
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
									case 0x00300200://4 组合无功1电能
										OAD=0x0003ff00;//DL645-2007标识 返回数据格式：XXXXXX.XX(总及分费率)
										break;
									case 0x00400200://5 组合无功2电能
										OAD=0x0004ff00;//DL645-2007标识 返回数据格式：XXXXXX.XX(总及分费率)
										break;
									case 0x00500201://6 一象限无功总电能
										OAD=0x00050000;//DL645-2007标识 返回数据格式：XXXXXX.XX
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
									case 0x20170200://10 有功需量
						        OAD=0x02800004;//DL645-2007标识 返回数据格式：XX.XXXX
										break;
									case 0x20180200://11 无功需量
										OAD=0x02800005;//DL645-2007标识 返回数据格式：XX.XXXX
										break;
									case 0x20000200://12 电压
										OAD=0x0201ff00;//DL645-2007标识 返回数据格式：A相XXX.X B相XXX.X C相XXX.X
										break;										
									case 0x20000201://12 A相电压
										OAD=0x02010100;//DL645-2007标识 返回数据格式：A相XXX.X 
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
									case 0x20010400://14 零线电流
										OAD=0x02800001;//DL645-2007标识 返回数据格式：XXX.XXX
										break;
									case 0x20040200://15 有功功率
									case 0x20040202://15 A相有功功率
										OAD=0x0203ff00;//DL645-2007标识 返回数据格式：总XX.XXXX A相XX.XXXX B相XX.XXXX C相XX.XXXX
										break;
									case 0x20050200://16 无功功率
									case 0x20050202://16 A相无功功率
										OAD=0x0204ff00;//DL645-2007标识 返回数据格式：总XX.XXXX A相XX.XXXX B相XX.XXXX C相XX.XXXX
										break;
									case 0x200A0200://17 功率因数
									case 0x200A0202://17 A相功率因数
										OAD=0x0206ff00;//DL645-2007标识 返回数据格式：总X.XXX A相X.XXX B相X.XXX C相X.XXX
										break;
									case 0x11100201://18 冻结周期内正向有功最大需量 
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
							{//三相
								i=0;
								OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
								switch(OAD)
								{
									case 0x20000200://12 电压
									case 0x20000201://12 A相电压
									case 0x20010200://13 电流
									case 0x20010201://13 A相电流
									case 0x20010400://14 零线电流
										OAD=0x06010001;//DL645标识 1类负荷曲线(电压、电流、频率)
										break;
									case 0x20040200://15 有功功率
									case 0x20040202://15 A相有功功率
									case 0x20050200://16 无功功率
									case 0x20050202://16 A相无功功率
										OAD=0x06020001;//DL645标识 2类负荷曲线(有无功功率)
										break;
									case 0x200A0200://17 功率因数
									case 0x200A0202://17 A相功率因数
										OAD=0x06030001;//DL645标识 3类负荷曲线(功率因数)
										break;
									case 0x20210200://1 冻结时标
									case 0x00100201://2 正向有功总电能
									case 0x00200201://3 反向有功总电能  
									case 0x00300200://4 组合无功1电能
									case 0x00400200://5 组合无功2电能
										OAD=0x06040001;//DL645标识 4类负荷曲线(正反有功,組合无功1和2总电能)
										break;
									case 0x00500201://6 一象限无功总电能
									case 0x00600201://7 二象限无功总电能 返回数据格式：XXXXXX.XX
									case 0x00700201://8 三象限无功总电能 返回数据格式：XXXXXX.XX
									case 0x00800201://9 四象限无功总电能 返回数据格式：XXXXXX.XX
										OAD=0x06050001;//DL645标识 5类负荷曲线(1-4象限无功总电能)
										break;
									case 0x20170200://10 有功需量
									case 0x20180200://11 无功需量
										OAD=0x06060001;//DL645标识 6类负荷曲线(当前有功需量,当前无功需量)
										break;
									case 0x11100201://18 冻结周期内正向有功最大需量 
										return 0;
									default:
										return 0;
								}
								UARTCtrl->RM_DI=OAD;//DL645标识 负荷曲线
								UARTCtrl->RMCSDSubCountMax=1;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
								UARTCtrl->RMROADCountMax=0;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
								return (u8*)1;
							}
						}
					}

				//先找全匹配
		#if PARAMETER_601E==0//设置参数601E采集规则库使用:0=内部默认,1=可设置
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//得到采集规则库初始设置库列表个数
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
					{//采集规则库中找到全匹配
						Len=Get_DL698DataLen(p8d+2,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
						Len>>=8;
						p8d+=2+Len+4;
						if(UARTCtrl->RM_DI_CHOICE!=0)//抄表DI选择:0=主DI,1=备用DI
						{//备用DI
							x=p8d[1];//数组数
							x*=6;
							x+=2;
							p8d+=x;
						}
						i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
						x=p8d[1];
						UARTCtrl->RMCSDSubCountMax=x;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
						UARTCtrl->RMROADCountMax=0;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
						if(i>=x)
						{//抄表CSD子计数>=数组数
							return 0;//抄完
						}
						p8d+=4+(i*6);
						i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
						UARTCtrl->RM_DI=i;
						return (u8*)1;
					}
				//再找单个OAD匹配
					if(UARTCtrl->RMCSD[1]==0)//当前正在抄的CSD(有数据类型)
					{//不是ROAD错
						return 0;
					}
					i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
					x=UARTCtrl->RMCSD[6];//ROAD个数
					UARTCtrl->RMROADCountMax=x;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
					if(i>=x)
					{//抄表ROAD计数>=数组数
						return 0;//抄完
					}
					OAD=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|UARTCtrl->RMCSD[5];
					i*=4;
					i+=7;
					ROAD=(UARTCtrl->RMCSD[i]<<24)|(UARTCtrl->RMCSD[i+1]<<16)|(UARTCtrl->RMCSD[i+2]<<8)|UARTCtrl->RMCSD[i+3];
		#if PARAMETER_601E==0//设置参数601E采集规则库使用:0=内部默认,1=可设置
					p32=(u32*)SETINIT_601E_List;
					n=Get_SETINIT_601E_List_NUM();//得到采集规则库初始设置库列表个数
					for(i=0;i<n;i++)
					{
						p8d=(u8*)p32[0];
						if(p8d[0]==DataType_structure)
						{
							if(p8d[3]==1)
							{//是ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//是单个ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//找到
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
							{//是ROAD
								Len=(p8d[4]<<24)|(p8d[5]<<16)|(p8d[6]<<8)|p8d[7];
								if(OAD==Len)
								{
									if(p8d[8]==1)
									{//是单个ROAD
										Len=(p8d[9]<<24)|(p8d[10]<<16)|(p8d[11]<<8)|p8d[12];
										if(ROAD==Len)
										{//找到
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
					Len=Get_DL698DataLen(p8d+2,0,0,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:b31-b8长度,b7-b0元素计数
					Len>>=8;
					p8d+=2+Len+4;
					if(UARTCtrl->RM_DI_CHOICE!=0)//抄表DI选择:0=主DI,1=备用DI
					{//备用DI
						x=p8d[1];//数组数
						x*=6;
						x+=2;
						p8d+=x;
					}
					i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
					x=p8d[1];
					UARTCtrl->RMCSDSubCountMax=x;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
					if(i>=x)
					{//抄表CSD子计数>=数组数
						return 0;//抄完
					}
					p8d+=4+(i*6);
					i=(p8d[0]<<24)|(p8d[1]<<16)|(p8d[2]<<8)|p8d[3];
					UARTCtrl->RM_DI=i;
					return (u8*)1;
					
				case 3://DL/T698.45
				case ProtocolNO_ABB://ABB
/*
				//单相曲线抄读
					if(UARTCtrl->RMCSD[1]==1)
					{//ROAD
						OAD=UARTCtrl->RMCSD[2+0]<<24;
						OAD|=UARTCtrl->RMCSD[2+1]<<16;
						OAD|=UARTCtrl->RMCSD[2+2]<<8;
						OAD|=UARTCtrl->RMCSD[2+3];
						if(OAD==0x50020200)
						{//分钟冻结
							i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
							if(i>=1)
							{
								return 0;
							}
							id=UARTCtrl->TaskID;
							id=(id>>8)&0xff;//任务号
							i=UARTCtrl->MeterCount[id];//取档案号
							p8d=(u8*)ADDR_6000_SDRAM+(i*LENper_6000);
							p8d=Get_Element(p8d,2,8,0);//计算元素地址,使用ADDR_128KDATABUFF
							if(p8d==0)
							{
								return 0;
							}
							i=p8d[1];//取接线方式
							if(i==1)//未知（0），单相（1），三相三线（2），三相四线（3）
							{//单相,抄实时数据
								i=UARTCtrl->RMROADCount;//抄表CSD的ROAD内OAD计数(非DL698抄表时)
								x=UARTCtrl->RMCSD[6];
								UARTCtrl->RMCSDSubCountMax=1;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
								UARTCtrl->RMROADCountMax=x;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
								if(i>=x)
								{//抄表CSD子计数>=数组数
									return 0;//抄完
								}
								OAD=(UARTCtrl->RMCSD[7+(4*i)+0]<<24)+(UARTCtrl->RMCSD[7+(4*i)+1]<<16)+(UARTCtrl->RMCSD[7+(4*i)+2]<<8)+(UARTCtrl->RMCSD[7+(4*i)+3]<<0);
								switch(OAD)
								{
									case 0x20210200://1 冻结时标
										return 0;
									case 0x00100201://2 正向有功总电能
										break;
									case 0x00200201://3 反向有功总电能  
										break;
									case 0x00300200://4 组合无功1电能
										break;
									case 0x00400200://5 组合无功2电能
										break;
									case 0x00500201://6 一象限无功总电能
										break;
									case 0x00600201://7 二象限无功总电能
										break;
									case 0x00700201://8 三象限无功总电能
										break;
									case 0x00800201://9 四象限无功总电能
										break;
									case 0x20170200://10 有功需量
										break;
									case 0x20180200://11 无功需量
										break;
									case 0x20000200://12 电压
										break;										
									case 0x20000201://13 A相电压
										break;
									case 0x20010200://14 电流
										break;										
									case 0x20010201://15 A相电流
										break;
									case 0x20010400://16 零线电流
										break;
									case 0x20040200://17 有功功率
									case 0x20040202://18 A相有功功率
										break;
									case 0x20050200://19 无功功率
									case 0x20050202://20 A相无功功率
										break;
									case 0x200A0200://21 功率因数
									case 0x200A0202://22 A相功率因数
										break;
									case 0x11100201://23 冻结周期内正向有功最大需量 
										return 0;
									default:
										return 0;
								}
								UARTCtrl->RM_DI=OAD;
								return (u8*)1;
							}
							else
							{//三相，抄曲线冻结数据
								
							}
						}
					}
*/
				//
					x=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
					if(x)
					{//子计数!=0
						return 0;
					}
					UARTCtrl->RMROADCountMax=0;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
					UARTCtrl->RMCSDSubCountMax=0;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
					return (u8*)1;
				case 4://CJ/T 188-2004
					return 0;
			}
		case 2://事件采集方案
			
		#if (USER/100)==17//江苏系
			if(Terminal_Router->MinuteCollect==1)
				return 0;
		#endif
			i*=LENper_6016;
			i+=ADDR_6016_SDRAM;
			p8s=Get_Element((u8 *)i,2,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			i=p8s[3];//采集类型
			switch(i)
			{
				case 0://0	array ROAD	周期采集事件数据
					p8s+=4;
					break;
				case 1://1	NULL	根据通知采集所有事件数据
					return 0;
				case 2://2	array ROAD	根据通知采集指定事件数据
					p8s+=4;
					break;
				case 3://3  NULL 根据通知存储生成的事件数据
					return 0;
				default:
					return 0;
			}
			i=UARTCtrl->RMCSDCount;//抄表CSD计数
			x=Get_DL698ByteTypeLenValue(p8s+1);//得到DL698字节类型长度单元的值
			UARTCtrl->RMCSDCountMax=x;//抄表CSD最大个数
			if(i>=x)
			{//抄表CSD计数>=数组数
				return 0;
			}
			p8s=Get_Element(p8s,i+1,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			LEN_CSD=Get_DL698DataLen_S(p8s,0);//得到DL698数据类型长度(包括:类型字节和数据);入口:LENmax仅取NAND数据时先读到ADDR_128KDATABUFF用,返回:长度
			if(((LEN_CSD+1)>sizeof(UARTCtrl->RMCSD))||(LEN_CSD<6))
			{
				return 0;
			}
			//MR((u32)&UARTCtrl->RMCSD,(u32)p8s,LEN_CSD);
			//ROAD转为普通方案的RCSD格式，
			if(LEN_CSD)
			{
				LEN_CSD--;
			}
			UARTCtrl->RMCSD[0]=DataType_CSD;
			UARTCtrl->RMCSD[1]=1;
			MR(((u32)&UARTCtrl->RMCSD)+2,(u32)p8s+1,LEN_CSD);
		#if CompareMeterEventCount==1//全事件采集抄表是否比较发生次数:0=不比较(全部抄表数据写入文件),1=比较(只有次数变化的事件写入文件)
			RM_MeterEventAutoInc0x20220200(PORTn);//电能表事件抄读列选择若无发生次数(事件记录序号)则自动加入
		#endif
			x=UARTCtrl->RMprotocol;//抄表规约类型=未知 (0)，DL/T645-1997（1），DL/T645-2007（2），DL/T698.45（3），CJ/T 188-2004（4）
			switch(x)
			{
				case 1://DL/T645-1997
					return 0;
				case 2://DL/T645-2007
					OAD=(UARTCtrl->RMCSD[2]<<24)|(UARTCtrl->RMCSD[3]<<16)|(UARTCtrl->RMCSD[4]<<8)|UARTCtrl->RMCSD[5];
					i=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
					UARTCtrl->RMCSDSubCountMax=2;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
					if(i>=2)
					{//抄表CSD子计数>=2
						return 0;
					}
					OAD=Get_Event645_DI(OAD,i);//获取当前的抄645表的数据项，OAD=事件OAD，index=当前OAD对应的第几个数据表示，从0开始，返回0=没有要抄读的数据标识
					if(OAD==0)
					{
						return 0;
					}
					UARTCtrl->RM_DI=OAD;
					UARTCtrl->RM_DI_N=0;
					return (u8*)1;
				case 3://DL/T698.45
					x=UARTCtrl->RMCSDSubCount;//抄表CSD子计数
					if(x)
					{//子计数!=0
						return 0;
					}
					UARTCtrl->RMROADCountMax=0;//抄表CSD的ROAD内有OAD最大个数(非DL698抄表规约库设定)
					UARTCtrl->RMCSDSubCountMax=0;//抄表CSD有子抄表个数(非DL698抄表规约库设定)
					return (u8*)1;
				case 4://CJ/T 188-2004
					return 0;
				default://ABB,Landis等
					return 0;
			}
		case 3://透明方案
//属性2（透明方案）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	方案编号     unsigned，
//	方案内容集   array 方案内容，
//	方案内容∷=structure
//	{
//		通信地址        	TSA，
//		开始前脚本id   	long-unsigned，
//		完成后脚本id   	long-unsigned，
//		方案控制标志  	structure，
//		{
//			上报透明方案结果并等待后续报文  bool，
//			等待后续报文超时时间（秒）      long-unsigned，
//			结果比对标识    enum{不比对（0），比（1），比对上报（2）}，
//			结果比对参数    structure
//			{
//				特征字节      unsigned，
//				截取开始      long-unsigned，
//				截取长度      long-unsigned
//			}
//		}
//		方案报文集 	 	array 方案报文
//		方案报文∷=structure
//		{
//			报文序号  unsigned，
//			报文内容  octet-string
//		}
//	}
//	存储深度     long-unsigned
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
			x=Get_DL698ByteTypeLenValue(p8s+5);//得到DL698字节类型长度单元的值
			if(UARTCtrl->MeterCount[id]>=x)
			{
				return 0;
			}
			p8s=Get_Element((u8*)(i+4),UARTCtrl->MeterCount[id]+1,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			//方案报文集
			p8s=Get_Element(p8s,5,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			i=UARTCtrl->RMCSDCount;
			x=Get_DL698ByteTypeLenValue(p8s+1);//得到DL698字节类型长度单元的值
			UARTCtrl->RMCSDCountMax=x;//抄表CSD最大个数
			if(i>=x)
			{
				return 0;
			}
			p8s=Get_Element(p8s,UARTCtrl->RMCSDCount+1,0,0);//计算元素地址,使用ADDR_128KDATABUFF
			if(p8s==0)
			{
				return 0;
			}
			return p8s+4;
		case 4://上报方案
		case 5://脚本方案
		default:
			return 0;
	}
}















