

#include "../H698/Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../DL698/DL698TEST.h"
#include "../Device/MEMRW.h"
#include "../Device/RTC.h"
#include "../Calculate/Calculate.h"
#include "../Terminal698/Terminal_Init.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../DL698/TASK.h"
#include "../DL698/RM_TASK.h"
#include "../MS/MS.h"
#include "../Display/Display.h"
#include "../Display/Warning.h"







#if RUN_DL698TEST_1==1//0无效,1=运行;全部档案设为内部交采测试数据存储最大深度

const u8 InitData_6000_2_TEST[]=//采集档案配置
{
//属性2（Acquisition document definition）∷=structure//删除时structure的数据单元清为0(NULL)
	DataType_structure,
	4,
//{
//配置序号  long-unsigned，
	DataType_long_unsigned,
	0,1,
//基本信息  Basic_object，
	
//扩展信息  Extended_object，
	
//附属信息  Annex_object
//}

//Basic_object∷=structure
	DataType_structure,
	10,
//{
//通信地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//波特率    enum
//{
//300bps（0），  600bps（1），    1200bps（2），
//2400bps（3）， 4800bps（4），   7200bps（5），
//9600bps（6）， 19200bps（7），  38400bps（8），
//57600bps（9），115200bps（10），自适应（255）
//}，
	DataType_enum,
	255,
//规约类型  enum
//{
//  未知 (0)，
//DL/T645-1997（1），
//DL/T645-2007（2），
//DL/T698.45（3），
//CJ/T 188-2004（4）
//}，
	DataType_enum,
	3,
//端口      OAD，
	DataType_OAD,
	0xF2,0x08,0x02,0x01, //交流采样通信口号(虚拟)
//通信密码  octet-string，
	DataType_octet_string,
	6,
	0x00,0x00,0x00,0x00,0x00,0x00,
//费率个数  unsigned，
	DataType_unsigned,
	4,
//用户类型  unsigned，
	DataType_unsigned,
	101,
//接线方式  enum
//{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//}
	DataType_enum,
	3,
//额定电压    long-unsigned(换算-1，单位V),
	DataType_long_unsigned,
	(2200>>8)&0xff,2200&0xff,
//额定电流    long-unsigned(换算-1，单位A)
	DataType_long_unsigned,
	(15>>8)&0xff,15&0xff,
//}

//Extended_object∷=structure
	DataType_structure,
	4,
//{
//采集器地址  TSA，
	DataType_TSA,0x07,
	0x05,0x00,0x00,0x00,0x00,0x00,0x00,
//资产号      octet-string，
	DataType_octet_string,
	0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//PT          long-unsigned，
	DataType_long_unsigned,
	0,0,
//CT          long-unsigned
	DataType_long_unsigned,
	0,0,
//}
//Annex_object∷=array structure
	DataType_array,
	0,
//{
//对象属性描述  OAD，
//属性值        Data
//}
};


void DL698TEST_1(void)//全部档案设为内部交采测试数据存储最大深度
{
	u32 i;
	u32 x;
	u64 d64;
	u8* p8;
	u32 filename;
	msFILE_TypeDef *file;
//	ms_Type *ms;
	UARTCtrl_TypeDef *UARTCtrl;
	
	
	p8=(u8*)ADDR_TEST;
	i=p8[0];
	switch(i)
	{
		case 0:
			d64=d64;
/*
			//初始化参数
			DL698_INIT(3);//初始化,入口Command:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4/5=参数(除与系统主站通信有关的)及全体数据区初始化,6=仅部份参数初始化
			//清档案
			MC(0,ADDR_6000,LENper_6000*NUMmax_6000);
			//初始化档案
			MR(ADDR_DATABUFF,(u32)InitData_6000_2_TEST,sizeof(InitData_6000_2_TEST));
			d64=0;//通信地址
			//for(i=0;i<NUMmax_6000;i++)
			for(i=0;i<10;i++)
			{
				d64=hex_bcd16(d64);//HEX数转为16位压缩BCD数			
				d64=R_Inverse(d64,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
				MWR(d64,ADDR_DATABUFF+10,6);
				MW(ADDR_DATABUFF,ADDR_6000+(i*LENper_6000),LENper_6000);
				d64=R_Inverse(d64,6);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
				d64=bcd16_hex(d64);//16位压缩BCD数转为HEX
				d64++;
			}
			FLASHtoSDRAM();//参数FLASHtoSDRAM
*/
			p8[0]++;
			Comm_Ram->DisplayAllErrorTimer=0;//上电全显或系统错误显示秒定时器

			break;
		case 1:
			//NAND中文件总数和当前数
			p8=(u8*)ADDR_DATABUFF;
			CopyString((u8 *)"NAND               ",p8);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			p8+=6;
			i=FILENAMEmax_FLASH;
			p8+=HEX_ASCII(i,(u32)p8);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
			p8++;
			i=0;
			file=(msFILE_TypeDef*)(ADDR_msFILEmanage+LEN_msFILEunltFlags);
			for(filename=0;filename<recordFILENAMEmax_FLASH;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					i++;
				}
				file++;
			}
			p8+=HEX_ASCII(i,(u32)p8);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
			p8[0]=0;
			WarningOccur(RS485_1PORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			
			//RAM中文件总数和当前数
			p8=(u8*)ADDR_DATABUFF;
			CopyString((u8 *)"RAM               ",p8);//拷贝字符串;返回拷贝字符串字节数(不计结束0)
			p8+=6;
			i=FILENAMEmax_RAM;
			p8+=HEX_ASCII(i,(u32)p8);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
			p8++;
			i=0;
			file=(msFILE_TypeDef*)(ADDR_RAMmsFILEmanage);
			for(filename=0;filename<recordFILENAMEmax_RAM;filename++)
			{
				if(file->BLOCK_ENTRY)
				{
					i++;
				}
				file++;
			}
			p8+=HEX_ASCII(i,(u32)p8);//HEX转为ASCII码存在Addr存贮单元,不显无效0,返回有效ASCII字符数
			p8[0]=0;
			WarningOccur(RS485_2PORT,(u8 *)ADDR_DATABUFF);//告警发生,入口告警代码和告警字符串
			
			//调时间
			UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(ACSAMPLEPORT);
			if(UARTCtrl->TaskID!=0)
			{
				break;
			}
			for(i=0;i<60;i++)
			{
				TASK(ACSAMPLEPORT,1);//任务;入口:RM=0上报任务,RM=1采集任务
				if(UARTCtrl->TaskID!=0)
				{
					break;
				}
			}
			if(i<60)
			{
				return;
			}
					//u8 DL698YMDHMS[10];//年年(先高字节后低字节),月,日,时,分,秒,百分秒,周
					x=Comm_Ram->RTCBuff[1];//分
					x=bcd_hex(x);
					i=(Comm_Ram->RTCBuff[1]<<8)+Comm_Ram->RTCBuff[0];//分秒
					
					if(Comm_Ram->RTCBuff[0]<0x58)
					{
						x/=5;
						x*=5;
						x+=4;
						x=hex_bcd(x);
						Comm_Ram->RTCBuff[1]=x;//分
						Comm_Ram->RTCBuff[0]=0x58;//秒
						WriteRTC(ADDR_RTCBUFF);//写实时钟
					}
					
/*
						if(i<0x5958)
						{
							Comm_Ram->RTCBuff[2]=0x23;//时
							Comm_Ram->RTCBuff[1]=0x59;//分
							Comm_Ram->RTCBuff[0]=0x58;//秒
							WriteRTC(ADDR_RTCBUFF);//写实时钟
						}
*/
/*
					switch(i/15)
					{
						case 0:
							if(i<0x1458)
							{
								Comm_Ram->RTCBuff[1]=0x14;//分
								Comm_Ram->RTCBuff[0]=0x58;//秒
								WriteRTC(ADDR_RTCBUFF);//写实时钟
							}
							break;
						case 1:
							if(i<0x2958)
							{
								Comm_Ram->RTCBuff[1]=0x29;//分
								Comm_Ram->RTCBuff[0]=0x58;//秒
								WriteRTC(ADDR_RTCBUFF);//写实时钟
							}
							break;
						case 2:
							if(i<0x4458)
							{
								Comm_Ram->RTCBuff[1]=0x44;//分
								Comm_Ram->RTCBuff[0]=0x58;//秒
								WriteRTC(ADDR_RTCBUFF);//写实时钟
							}
							break;
						case 3:
							if(i<0x5958)
							{
								Comm_Ram->RTCBuff[1]=0x59;//分
								Comm_Ram->RTCBuff[0]=0x58;//秒
								WriteRTC(ADDR_RTCBUFF);//写实时钟
							}
							break;
					}
*/
					
					
			break;
		
			
			
		default:
			break;
	}
	
	
	
}
#endif



























