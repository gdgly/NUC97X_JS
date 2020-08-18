
#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"

#include "../DL698/DL698_Uart.h"
#include "../DL698/DL698_SETLIB.h"
#include "../DL698/DL698_GET.h"
#include "../DL698/DL698_GET_F000.h"
#include "../Calculate/Calculate.h"
#include "../DL698/Info_3762.h"

u32 Get_Request_4500(u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u8 *p8s;
	u32 Len;
	UARTCtrl_TypeDef *UARTCtrl;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	if(LENmax_TxSPACE<2)
	{
		GET_Request_Frame(PORTn);//分帧标志
		return 0;
	}
	
	p8s=(u8*)ADDR_45000C00;
	p8tx[0]=DataType_visible_string;
	p8tx[1]=0x1A;
	
	//type=
	p8tx[2] = 0x74;
	p8tx[3] = 0x79;
	p8tx[4] = 0x70;
	p8tx[5] = 0x65;
	p8tx[6] = 0x3d;
	
	//模块类型 3f写死
	p8tx[7] = 0x33;
	p8tx[8] = 0x66;
	
	p8tx[9] = 0x3b;
	
	//id=
	p8tx[10] = 0x69;
	p8tx[11] = 0x64;
	p8tx[12] = 0x3d;
	
	MR((u32)p8tx+13,(u32)p8s+2,15);
	Len=0x1C;
	return Len;
}

u32 GET_Request_F200(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 Len;
	u8* p8s;
	u8* p8s1;
	u8* p8s2;
	u8* p8s3;
	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 Array;
	u32 NUMmax;
	u32 n;
	u32 m,M;
	u32 j,k,L;
//	u32 data;

	Array=Array;
	NUMmax=NUMmax;
	n=n;
	m=m;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	if(LENmax_TxSPACE<2)
	{
		GET_Request_Frame(PORTn);//分帧标志
		return 0;
	}
	p8tx[0]=DataType_array;
	p8tx[1]=0;
	Len=2;
	switch(OAD>>16)//OI
	{
		case 0xF200://设备对象-RS232
			switch((OAD>>8)&0xff)//属性
			{
				case 2:
					//属性2（设备对象列表，只读）∷=array 端口
					//端口∷=structure
					//{
					//  端口描述符 visible-string，
					//端口参数   COMDCB，
					//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
					//}
					if(LENmax_TxSPACE<19)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8tx[1]=1;//数组数
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=5;
					p8tx[Len+4]='R';
					p8tx[Len+5]='S';
					p8tx[Len+6]='2';
					p8tx[Len+7]='3';
					p8tx[Len+8]='2';
					p8tx[Len+9]=DataType_COMDCB;
					Len+=10;
					MR((u32)p8tx+Len,ADDR_F200_2+8,5);
					Len+=5;
					p8tx[Len+0]=DataType_enum;
					Len+=1;
					MR((u32)p8tx+Len,ADDR_F200_2+14,1);
					Len+=1;
					return Len;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
			//return 0;
		case 0xF201://设备对象-RS485
			switch((OAD>>8)&0xff)//属性
			{
				case 2:
					//属性2（设备对象列表，只读）∷=array 端口
					//端口∷=structure
					//{
					//  端口描述符 visible-string，
					//端口参数   COMDCB，
					//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
					//}
					if(LENmax_TxSPACE<40)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8tx[1]=2;//数组数
					//485-1
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=7;
					p8tx[Len+4]='R';
					p8tx[Len+5]='S';
					p8tx[Len+6]='4';
					p8tx[Len+7]='8';
					p8tx[Len+8]='5';
					p8tx[Len+9]='-';
					p8tx[Len+10]='1';
					p8tx[Len+11]=DataType_COMDCB;
					Len+=12;
					MR((u32)p8tx+Len,ADDR_F201_2+8,5);
					Len+=5;
					p8tx[Len+0]=DataType_enum;
					Len+=1;
					MR((u32)p8tx+Len,ADDR_F201_2+14,1);
					Len+=1;
					//485-2
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=7;
					p8tx[Len+4]='R';
					p8tx[Len+5]='S';
					p8tx[Len+6]='4';
					p8tx[Len+7]='8';
					p8tx[Len+8]='5';
					p8tx[Len+9]='-';
					p8tx[Len+10]='2';
					p8tx[Len+11]=DataType_COMDCB;
					Len+=12;
					MR((u32)p8tx+Len,ADDR_F201_2+LENper_F201_2+8,5);
					Len+=5;
					p8tx[Len+0]=DataType_enum;
					Len+=1;
					MR((u32)p8tx+Len,ADDR_F201_2+LENper_F201_2+14,1);
					Len+=1;
					return Len;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
			//return 0;
		case 0xF202://设备对象-红外
			switch((OAD>>8)&0xff)//属性
			{
				case 2:
					//属性2（设备对象列表，只读）∷=array 端口
					//端口∷=structure
					//{
					//  端口描述符 visible-string，
					//端口参数   COMDCB，
					//}
					if(LENmax_TxSPACE<17)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8tx[1]=1;//数组数
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=3;
					p8tx[Len+2]=DataType_visible_string;
					p8tx[Len+3]=5;
					p8tx[Len+4]='I';
					p8tx[Len+5]='R';
					p8tx[Len+6]=' ';
					p8tx[Len+7]=' ';
					p8tx[Len+8]=' ';
					p8tx[Len+9]=DataType_COMDCB;
					Len+=10;
					MR((u32)p8tx+Len,ADDR_F202_2+8,5);
					Len+=5;
					return Len;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
			//return 0;
		case 0xF203://开关量输入
			switch((OAD>>8)&0xff)//属性
			{
				case 0x42://状态量变位发生后状态
					i=OAD&0xff;
					if((i>NUMmax_SWITCHIN)||(i==0))
					{
						return 0;
					}
					if(LENmax_TxSPACE<6)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					i--;
					i*=6;
					i+=ADDR_F203_2+2;
					MR((u32)p8tx,i,6);
					return 6;
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
			//reutrn 0;
		case 0xF204://直流模拟量
			return 0;
		case 0xF205://继电器输出
			//属性2（设备对象列表，只读）∷=array 继电器单元(在读通信时填写)
			//继电器单元∷=structure
			//{
			//  描述符    visible-string，
			//当前状态  enum{未输出（0），输出（1）}，
			//开关属性  enum{脉冲式（0），保持式（1）}，
			//接线状态  enum{接入（0），未接入（1) }
			//}
			p8d=(u8*)ADDR_F205_2;
			p8d[0]=DataType_array;
			p8d[1]=2;
			p8d+=2;
		//1
			p8d[0]=DataType_structure;
			p8d[1]=4;
			p8d[2]=DataType_visible_string;
			p8d[3]=0;
			p8d[4]=DataType_enum;//当前状态  enum{未输出（0），输出（1）}
			p8s=(u8*)ADDR_Terminal_Remote_State;//控制状态,0=上电或不控制,1=投入,2=...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
			if(p8s[0]<102)
			{
				p8d[5]=0;//{未输出（0），输出（1）}
			}
			else
			{
				p8d[5]=1;//{未输出（0），输出（1）}
			}
			p8d[6]=DataType_enum;//开关属性  enum{脉冲式（0），保持式（1）}
			p8d[7]=0;
			p8d[8]=DataType_enum;//接线状态  enum{接入（0），未接入（1) }
			p8s=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
			if(p8s[2]&1)
			{
				p8d[9]=1;
			}
			else
			{
				p8d[9]=0;
			}
			p8d+=10;
		//2
			p8d[0]=DataType_structure;
			p8d[1]=4;
			p8d[2]=DataType_visible_string;
			p8d[3]=0;
			p8d[4]=DataType_enum;//当前状态  enum{未输出（0），输出（1）}
			p8s=(u8*)ADDR_Terminal_Remote_State+Terminal_Ctrl_Pn_Len;//控制状态,0=上电或不控制,1=投入,2=...,100=报警,101=跳闸准备,102=跳闸,103=跳闸2分钟后
			if(p8s[0]<102)
			{
				p8d[5]=0;//{未输出（0），输出（1）}
			}
			else
			{
				p8d[5]=1;//{未输出（0），输出（1）}
			}
			p8d[6]=DataType_enum;//开关属性  enum{脉冲式（0），保持式（1）}
			p8d[7]=0;
			p8d[8]=DataType_enum;//接线状态  enum{接入（0），未接入（1) }
			p8s=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
			if(p8s[2]&4)
			{
				p8d[9]=1;
			}
			else
			{
				p8d[9]=0;
			}
			return 0;
		case 0xF206://告警输出
			//属性2（设备对象列表，只读）∷=array 告警输出(在读通信时填写)
			//告警输出∷=enum
			//{
			//未输出（0），输出（1）
			//}
	#if (Project/100)==2//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			p8d=(u8*)ADDR_F206_2;
			p8d[0]=DataType_array;
			p8d[1]=0;
	#endif
	#if (Project/100)==3//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			p8d=(u8*)ADDR_F206_2;
			p8d[0]=DataType_array;
			p8d[1]=1;
			p8d+=2;
			p8d[0]=DataType_enum;
			p8s=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
//#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//控制模块输出输入数据
//1BYTE	要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
//1BYTE 要输出给控制模块的控制1:b0=告警继电器动作,b1=保电指示灯,b2-b6,b7=轮次继电器动作方式(0=脉冲1=内部自动测试时电平)
			if(p8s[1]&1)//b0=告警继电器动作
			{
				p8d[1]=1;
			}
			else
			{
				p8d[1]=0;
			}
	#endif
	#if (Project/100)==4//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
			if(Comm_Ram->MODE3in1==1)//三合一终端当前运行模式:0=专变,1=集中器
			{
				p8d=(u8*)ADDR_F206_2;
				p8d[0]=DataType_array;
				p8d[1]=0;
			}
			else
			{
				p8d=(u8*)ADDR_F206_2;
				p8d[0]=DataType_array;
				p8d[1]=1;
				p8d+=2;
				p8d[0]=DataType_enum;
				p8s=(u8*)ADDR_ContrlUARTData;//控制模块输出输入数据
//#define ADDR_ContrlUARTData     ADDR_Terminal_Total_State+4//控制模块输出输入数据
//1BYTE	要输出给控制模块的控制0:b0=轮次1继电器动作,b1=轮次1指示灯点红灯,b2=轮次1指示灯点绿灯,b3=轮次2继电器动作,b4=轮次2指示灯点红灯,b5=轮次2指示灯点绿灯,b6=功控指示灯点亮,b7=电控指示灯点亮
//1BYTE 要输出给控制模块的控制1:b0=告警继电器动作,b1=保电指示灯,b2-b6,b7=轮次继电器动作方式(0=脉冲1=内部自动测试时电平)
			if(p8s[1]&1)//b0=告警继电器动作
				{
					p8d[1]=1;
				}
				else
				{
					p8d[1]=0;
				}
			}
	#endif
			return 0;
			
		case 0xF207://多功能端子
			return 0;
		
		case 0xF209://载波/微功率无线接口
			switch((OAD>>8)&0xff)//属性
			{
			#if (((Project/100)==2)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
				case 2:
					//属性2（设备对象列表，只读）∷=array 本地通信模块单元
					//本地通信模块单元∷=structure
					//{
					//端口描述符   visible-string，
					//通信参数     COMDCB，
					//版本信息     VersionInfo
					//}
					//VersionInfo∷=structure
					//{
					//  厂商代码    visible-string(SIZE(2))，
					//  芯片代码    visible-string(SIZE(2))，
					//  版本日期     Date，
					//  软件版本     long-unsigned
					//}
					Len=Get_DL698DataLen_S((u8*)ADDR_F209_2,0);//得到DL698数据类型长度(包括:类型字节和数据);
					if(LENmax_TxSPACE<Len)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					// 端口描述符
					p8s=UART_3762_AFNxFn(0x03,12);
					M=p8s[2];
					MR((u32)p8tx,ADDR_F209_2,Len);
		
					//端口描述符	visible-string
					p8tx[4] = DataType_visible_string;
					p8tx[5] = 129; 
					p8tx[6] = 105+2*M; 
					//type=
					p8tx[7] = 0x74;
					p8tx[8] = 0x79;
					p8tx[9] = 0x70;
					p8tx[10] = 0x65;
					p8tx[11] = 0x3d;
					
					//模块类型 PLC写死
					p8tx[12] = 0x50;
					p8tx[13] = 0x4c;
					p8tx[14] = 0x43;

					// ;
					p8tx[15] = 0x3b;

					//mfrs=
					p8tx[16] = 0x6d;
					p8tx[17] = 0x66;
					p8tx[18] = 0x72;
					p8tx[19] = 0x73;
					p8tx[20] = 0x3d;

					// 厂商代码
					i=p8s[0]+(p8s[1]<<8);
					p8tx[21]=i>>8;
					p8tx[22]=i;

					//;
					p8tx[23] = 0x3b; 
					
					//idformat=
					p8tx[24] = 0x69; 
					p8tx[25] = 0x64; 
					p8tx[26] = 0x66; 
					p8tx[27] = 0x6f; 
					p8tx[28] = 0x72; 
					p8tx[29] = 0x6d; 
					p8tx[30] = 0x61; 
					p8tx[31] = 0x74; 
					p8tx[32] = 0x3d; 

					// bcd bin ascll
					p8tx[33] = 0x32;

					// ；
					p8tx[34] = 0x3b;

					// id =
					p8tx[35] = 0x69;
					p8tx[36] = 0x64;
					p8tx[37] = 0x3d;
					MR((u32)(p8tx + 38 + 49),(ADDR_AFN10F112 + 9),24);
					Data_Inverse((u32)p8tx+87,24);
					HexToASCII(&p8tx[87],&p8tx[38],24);

					// ；
					p8tx[62 + 24] = 0x3b;	

					// mmfrs=
					p8tx[63+ 24] = 0x6d;
					p8tx[64+ 24] = 0x6d;
					p8tx[65+ 24] = 0x66;
					p8tx[66+ 24] = 0x72;
					p8tx[67+ 24] = 0x73;
					p8tx[68+ 24] = 0x3d;

					//  模块厂商代码
					i=p8s[0]+(p8s[1]<<8);
					p8tx[69+ 24]=i>>8;
					p8tx[70+ 24]=i;

					// ；
					p8tx[71+ 24] = 0x3b;

					// midformat=
					p8tx[72+ 24] = 0x6d;
					p8tx[73+ 24] = 0x69;
					p8tx[74+ 24] = 0x64;
					p8tx[75+ 24] = 0x66;
					p8tx[76+ 24] = 0x6f;
					p8tx[77+ 24] = 0x72;
					p8tx[78+ 24] = 0x6d;
					p8tx[79+ 24] = 0x61;
					p8tx[80+ 24] = 0x74;
					p8tx[81+ 24] = 0x3d;

					// 02
					p8tx[82+ 24] = 0x31;

					// ;
					p8tx[83+ 24] = 0x3b;
					
					//mid =
					p8tx[84+ 24] = 0x6d;
					p8tx[85+ 24] = 0x69;
					p8tx[86+ 24] = 0x64;
					p8tx[87+ 24] = 0x3d;

					// 模块ID信息	
					Data_Inverse((u32)p8s+4,M);	
					HexToASCII(&p8s[4],&p8tx[112],M);
//					p8s2 = (u8*)ADDR_F209_2;
					MR((u32)(p8tx+6+128),(ADDR_F209_2 + 6),Len-6);

					// 芯片厂商代码
					p8tx[21] = p8tx[6+128+14];
					p8tx[22] = p8tx[6+128+15];
					Len+=128;
					return Len;
				case 5:
					//属性5（从节点对象列表，只读）∷=array 从节点单元
					//从节点单元	属性2∷=structure
					//{
					//  从节点序号      long-unsigned,
					//  从节点通信地址  octet-string,
					//  从节点描述符    visible-string
					//}
					//每节点字节数=2+3+8+2=15
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//填入配置组数
#if(USER/100)==17//江苏系
					p8s1=(u8*)ADDR_AFN10F7;
					NUMmax = p8s1[0];
					NUMmax |=p8s1[1]<<8;

#else
					NUMmax=Terminal_Router->NoteNum;//当前路由器中已知总从节点数(半字对齐)
#endif	
					n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
					while(1)
					{
						if(n>=NUMmax)
						{
							break;
						}
						if((Array>=127)||(LENmax_TxSPACE<171))
						{//发送缓冲不够,分帧
							if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
							{
								UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
							}
							else
							{
								UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
							}
							UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
							p8tx[1]=Array;//填入配置组数
							return Len;
						}
					//F210从节点单元	属性2∷=structure
						p8tx[Len+0]=DataType_structure;
						p8tx[Len+1]=3;
					//{
					//  从节点序号      long-unsigned,
						p8tx[Len+2]=DataType_long_unsigned;
						p8tx[Len+3]=(n+1)>>8;
						p8tx[Len+4]=n+1;
					//  从节点通信地址  octet-string,
						p8tx[Len+5]=DataType_octet_string;
						p8tx[Len+6]=6;
						MR((u32)p8tx+Len+7,ADDR_AFN10F7+2+(n*61),6);
						p8s1=(u8*)ADDR_AFN10F7;
						for(j=0;j<NUM_RMmax;j++)
						{
							if(Compare_DL698DataString(p8s1 + 2 + 61 * j ,p8tx+Len+7, 6) == 0)
							{
								break;
							}
						}
						p8s2= (u8*)ADDR_AFN10F104;
						for(L=0;L<NUM_RMmax;L++)
						{
							if(Compare_DL698DataString(p8s2 + 2 + 15 * L ,p8tx+Len+7, 6) == 0)
							{
								break;
							}
						}
						p8s3=(u8*)ADDR_AFN10F112;
						for(k=0;k<NUM_RMmax;k++)
						{
							if(Compare_DL698DataString(p8s3 + 2 + 33 * k ,p8tx+Len+7, 6) == 0)
							{
								break;
							}
						}

						
						Data_Inverse((u32)p8tx+Len+7,6);//数据倒序(高低字节调换),入口Le>=2

						M = p8s1[(61*j) + 9 +2];
	
					//  从节点描述符    visible-string
						p8tx[Len+13]=DataType_visible_string;
#if 0					
						if(n==0)
						{
							p8tx[Len+14]=108;
						}
						else
						{
							p8tx[Len+14]=129;
							p8tx[Len+15]=128;
						}
#else						
						p8tx[Len+14]=129;
						p8tx[Len+15]=103+2*M;
#endif

						//128 字节模块ID信息
						//type=
						p8tx[Len+16] = 0x74;
						p8tx[Len+17] = 0x79;
						p8tx[Len+18] = 0x70;
						p8tx[Len+19] = 0x65;
						p8tx[Len+20] = 0x3d;

#if 0
						//模块类型 PLC写死
						p8tx[Len+21] = 0x50;
						p8tx[Len+22] = 0x4c;
						p8tx[Len+23] = 0x43;
#else
						//HexToASCII(p8s3 + (33 * k) + 2 + 6,&p8tx[Len + 21],1);
						p8tx[Len+21]=(p8s3[(33 * k) + 2 + 6] + 0x30); 
#endif

						// ;
						p8tx[Len+22] = 0x3b;

						//mfrs=
						p8tx[Len+23] = 0x6d;
						p8tx[Len+24] = 0x66;
						p8tx[Len+25] = 0x72;
						p8tx[Len+26] = 0x73;
						p8tx[Len+27] = 0x3d;

						// 芯片厂商代码
#if 0
						if(n==0)
						{
							MR((u32)(p8tx+Len+30),(ADDR_F209_2 + 20),2);
						}
						else
						{
							MR((u32)(p8tx + Len + 30),(ADDR_AFN10F112 + 2 + 7 + 10 +(33 * n)) ,2);
						}
#else

						//节点总数量 BIN 2
						//节点1地址 BIN 6
						//节点1软件版本 BIN 2
						//节点1软件版本日期 BIN 3
						//节点1模块厂商代码 BIN 2
						//节点1芯片代码 BIN 2
						//...
						//节点n地址 BIN 6
						//节点n软件版本 BIN 2
						//节点n软件版本日期 BIN 3
						//节点n模块厂商代码 BIN 2
						//节点n芯片代码 BIN 2

						p8tx[Len+29]=p8s2[13+ 2 + 15*L];
						p8tx[Len+28]=p8s2[14+ 2 + 15*L];
//						Data_Inverse((u32)p8tx+Len+29,2);//数据倒序(高低字节调换),入口Le>=2 								
#endif
						//;
						p8tx[Len+30] = 0x3b; 
						
						//idformat=
						p8tx[Len+31] = 0x69; 
						p8tx[Len+32] = 0x64; 
						p8tx[Len+33] = 0x66; 
						p8tx[Len+34] = 0x6f; 
						p8tx[Len+35] = 0x72; 
						p8tx[Len+36] = 0x6d; 
						p8tx[Len+37] = 0x61; 
						p8tx[Len+38] = 0x74; 
						p8tx[Len+39] = 0x3d; 

						// bcd bin ascll
						p8tx[Len+40] = 0x32;

						// ；
						p8tx[Len+41] = 0x3b;

						// id =
						p8tx[Len+42] = 0x69;
						p8tx[Len+43] = 0x64;
						p8tx[Len+44] = 0x3d;

						//p8s2 = (u8*)ADDR_AFN10F112;
						MR((u32)(p8tx + Len + 45 + 49),(ADDR_AFN10F112 + 2 + 7 + (33 * k)) ,24);
						Data_Inverse((u32)p8tx+Len+94,24);
						HexToASCII(&p8tx[Len + 94],&p8tx[Len+45],24);

						// ；
						p8tx[Len + 93] = 0x3b;	

						// mmfrs=
						p8tx[Len + 94] = 0x6d;
						p8tx[Len + 95] = 0x6d;
						p8tx[Len + 96] = 0x66;
						p8tx[Len + 97] = 0x72;
						p8tx[Len + 98] = 0x73;
						p8tx[Len + 99] = 0x3d;

						//	模块厂商代码
#if	0
						if(n==0)
						{
							p8s=UART_3762_AFNxFn(0x03,12);
							i=p8s[0]+(p8s[1]<<8);
							p8tx[Len + 104]=i>>8;
							p8tx[Len + 105]=i;

						}
						else
						{
							p8s1=(u8*)ADDR_AFN10F7;
							i=p8s1[2+7+61*m]+(p8s1[2+7+1+61*m]<<8);
							p8tx[Len + 104]=i>>8;
							p8tx[Len + 105]=i;
						
}
#else
						p8tx[Len + 101]=p8s2[11 + 2 + 15*L];;
						p8tx[Len + 100]=p8s2[12 + 2 + 15*L];;
#endif

						// ；
						p8tx[Len + 102] = 0x3b;

						// midformat=
						p8tx[Len + 103] = 0x6d;
						p8tx[Len + 104] = 0x69;
						p8tx[Len + 105] = 0x64;
						p8tx[Len + 106] = 0x66;
						p8tx[Len + 107] = 0x6f;
						p8tx[Len + 108] = 0x72;
						p8tx[Len + 109] = 0x6d;
						p8tx[Len + 110] = 0x61;
						p8tx[Len + 111] = 0x74;
						p8tx[Len + 112] = 0x3d;

						// 02
						p8tx[Len + 113] = 0x31;

						// ;
						p8tx[Len + 114] = 0x3b;
						
						//mid =
						p8tx[Len + 115] = 0x6d;
						p8tx[Len + 116] = 0x69;
						p8tx[Len + 117] = 0x64;
						p8tx[Len + 118] = 0x3d;

						// 模块ID信息		
#if 0
						if(n==0)
						{
							HexToASCII(&p8s[4],&p8tx[Len + 124],11);
						}
						else
						{
							HexToASCII(&p8s1[2+11+n*61],&p8tx[Len + 124],11);
						}
#else
						Data_Inverse((u32)p8s1+2+11+j*61,M);
						HexToASCII(&p8s1[2+11+j*61],&p8tx[Len + 119],M);
						Data_Inverse((u32)p8s1+2+11+j*61,M);	// 对 ram 直接操作，需要转回来，否则改变原有数据
#endif						
					//  从节点响应时长
//						p8tx[Len+16+p8tx[Len+15]]=DataType_long_unsigned;
//						p8tx[Len+17+p8tx[Len+15]]=0;
//						p8tx[Len+18+p8tx[Len+15]]=3;
						
					//从节点最近一次通信成功时间 6032
//						p8tx[Len+19+p8tx[Len+15]]=DataType_date_time_s;
//						// 注意7 字节
//						p8s = (u8*)ADDR_6032;
//						for(i=0;i<NUM_RMmax;i++)
//						{
//							if(Compare_DL698DataString(&p8tx[Len+7],(u8*)(ADDR_6032 +  5 + (64 * i)), 6) == 0)
//							{
//								break;
//							}
//						}
//						MR((u32)(p8tx + Len + 20 + p8tx[Len+15]),(u32)(ADDR_6032 + 20 + (64 * i)) ,7);
						
					//}
#if 0
						if(n==0)
						{
							Len+=(16+108);
						}
						else
						{
							Len+=(16+128);
						}		
#else
						Len+=(16+125);
#endif						
						LENmax_TxSPACE-=(16+125);
						Array++;//填入配置组数
						n++;
					}
					if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					{
						UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
					p8tx[1]=Array;//填入配置组数
					return Len;
				case 6:
					//属性6（从节点对象列表更新周期）∷= TI
					//更新周期：指从节点对象列表、网络拓扑信息、多网信息等更新周期
					if(LENmax_TxSPACE<4)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					//TI∷=SEQUENCE
					//{
					//   单位  ENUMERATED
					//   {
					//       秒      （0），
					//       分      （1），
					//       时      （2），
					//       日      （3），
					//       月      （4），
					//       年      （5）
					//    }，
					//    间隔值  long-unsigned
					//}
					if(LENmax_TxSPACE<4)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					MR((u32)p8tx,ADDR_F209_6,4);
					Len=4;
					return Len;
				case 7:
					//属性7（网络拓补信息，只读）::=array网络拓补单元
				#if (((USER/100)==9)||((USER/100)==17))//河南和江苏
					i=0;
					n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
					p8s=(u8*)ADDR_DATABUFF;
					//节点起始序号：每次查询必须从序号1起始查询
					while(i<n)
					{//不是从0开始
						m=n-i;
						if(m>15)
						{
							m=15;
						}
						UART_3762_AFN03F101(PORTn,i+1,m,(u8*)ADDR_DATABUFF);//(河南)查询节点台区信息(只暂停路由,没恢复);入口:StartNo=节点起始序号首帧从0开始,Num=节点数量(最大支持15个),p8Buff=数据放置缓冲
						NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
						if(n>=NUMmax)
						{
							break;
						}
						m=p8s[4];//本次应答的节点数量
						if(m==0)
						{
							break;
						}
						i+=m;
					}
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//填入配置组数
					while(1)
					{
						UART_3762_AFN10F21(PORTn,n+1,32,(u8*)ADDR_DATABUFF);//查询网络拓扑信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
						p8s=(u8*)ADDR_DATABUFF;
					//上行：
						//节点总数量	BIN	2
						//节点起始序号	BIN	2
						//本次应答的节点数量n	BIN	1
						//节点1地址	BIN	6
						//节点1网络节点信息	BIN	5
						//……	……	……
						//节点n地址	BIN	6
						//节点n网络节点信息	BIN	5
						NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
						m=p8s[4];//本次应答的节点数量
						if(m==0)
						{
							break;
						}
						p8s+=5;
						while(m)
						{
							if(n>=NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<20))//每节点字节数=2+8+2+3+3+2=20
							{//发送缓冲不够,分帧
								if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								{
									UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
								p8tx[1]=Array;//填入配置组数
								//路由恢复
								Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
								Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
								return Len;
							}
						//F211网络拓补单元  属性2::=structure
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=5;
						//{
						//节点地址   octet-string(SIZE(6)),
							p8tx[Len+2]=DataType_octet_string;
							p8tx[Len+3]=6;
							MR((u32)p8tx+Len+4,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+4,6);//数据倒序(高低字节调换),入口Le>=2
						//节点类型 enum{主节点(0),从节点(1)},
							p8tx[Len+10]=DataType_enum;
							if(n==0)
							{
								p8tx[Len+11]=0;
							}
							else
							{
								p8tx[Len+11]=1;
							}
						//节点标识 long-unsingned,
							p8tx[Len+12]=DataType_long_unsigned;
							p8tx[Len+13]=p8s[7];
							p8tx[Len+14]=p8s[6];
						//代理节点标识 long-unsigned,
							p8tx[Len+15]=DataType_long_unsigned;
							p8tx[Len+16]=p8s[9];
							p8tx[Len+17]=p8s[8];
						//节点信息unsigned
							p8tx[Len+18]=DataType_unsigned;
							p8tx[Len+19]=p8s[10];
						//}
							p8s+=11;
							Len+=20;
							LENmax_TxSPACE-=20;
							Array++;//填入配置组数
							n++;
							m--;
						}
						if(m)
						{//完
							break;
						}
					}
					if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					{
						UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
					p8tx[1]=Array;//填入数组数
					//路由恢复
					Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
					Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				#else
					if(UART4Ctrl->Lock_InfoTask==0x55)//读路由网络拓扑信息等任务:0x55=锁定
					{
						UARTCtrl->Lock_InfoTask=0x55;//读路由网络拓扑信息等任务:0x55=锁定
						return 1;//表示有数据即可
					}
					else
					{
						UARTCtrl->Lock_InfoTask=0;//读路由网络拓扑信息等任务:0x55=锁定
					}
					i=0;
					n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//填入配置组数
					while(1)
					{
						p8s=(u8*)ADDR_AFN10F21;//从路由读取的网络拓扑信息
						//2byte 节点总数量
						//6byte 节点1地址
						//5byte 节点1网络拓扑信息
						//...
						//6byte 节点n地址
						//5byte 节点n网络拓扑信息
						NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
						m=NUMmax-n;//本次应答的节点数量
						if(m==0)
						{
							break;
						}
						p8s+=2+(11*n);
						while(m)
						{
							if(n>=NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<20))//每节点字节数=2+8+2+3+3+2=20
							{//发送缓冲不够,分帧
								if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								{
									UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
								p8tx[1]=Array;//填入配置组数
								return Len;
							}
						//F211网络拓补单元  属性2::=structure
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=5;
						//{
						//节点地址   octet-string(SIZE(6)),
							p8tx[Len+2]=DataType_octet_string;
							p8tx[Len+3]=6;
							MR((u32)p8tx+Len+4,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+4,6);//数据倒序(高低字节调换),入口Le>=2
						//节点类型 enum{主节点(0),从节点(1)},
							p8tx[Len+10]=DataType_enum;
							if(n==0)
							{
								p8tx[Len+11]=0;
							}
							else
							{
								p8tx[Len+11]=1;
							}
						//节点标识 long-unsingned,
							p8tx[Len+12]=DataType_long_unsigned;
							p8tx[Len+13]=p8s[7];
							p8tx[Len+14]=p8s[6];
						//代理节点标识 long-unsigned,
							p8tx[Len+15]=DataType_long_unsigned;
							p8tx[Len+16]=p8s[9];
							p8tx[Len+17]=p8s[8];
						//节点信息unsigned
							p8tx[Len+18]=DataType_unsigned;
							p8tx[Len+19]=p8s[10];
						//}
							p8s+=11;
							Len+=20;
							LENmax_TxSPACE-=20;
							Array++;//填入配置组数
							n++;
							m--;
						}
						break;
					}
					if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					{
						UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
					p8tx[1]=Array;//填入数组数
				#endif
					return Len;
				case 8:
					//属性 8（多网信息，只读）∷ =structure
				//#if (USER/100)==9//河南
				#if (((USER/100)==9)||((USER/100)==17))//河南和江苏
					p8s=UART_3762_AFNxFn(0x10,111);//查询多网络信息,宽带载波频段序号等(自动暂停和恢复路由);入口:p8Buff=数据放置缓冲
				//376.2上行报文数据单元格式
					//多网络节点总数量n BIN 1
					//本节点网络标识号  BIN 3
					//本节点主节点地址  BIN 6
					//邻居节点1网络标识号  BIN 3
					//……
					//邻居节点n网络标识号  BIN 3
				#else
					if(UART4Ctrl->Lock_InfoTask==0x55)//读路由网络拓扑信息等任务:0x55=锁定
					{
						UARTCtrl->Lock_InfoTask=0x55;//读路由网络拓扑信息等任务:0x55=锁定
						return 1;//表示有数据即可
					}
					else
					{
						UARTCtrl->Lock_InfoTask=0;//读路由网络拓扑信息等任务:0x55=锁定
					}
					p8s=(u8*)ADDR_AFN10F111;
				#endif
					n=p8s[0];
					n*=3;
					n+=18;
					if(LENmax_TxSPACE<n)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
				//属性 8（多网信息，只读）∷ =structure
				//{
					p8tx[0]=DataType_structure;
					p8tx[1]=3;
				//本节点网络标识号 octet -string(SIZE(3)) ，
					p8tx[2]=DataType_octet_string;
					p8tx[3]=3;
					p8tx[4]=p8s[3];
					p8tx[5]=p8s[2];
					p8tx[6]=p8s[1];
				//本节点主地址 octet -string(SIZE(6)) ，
					p8tx[7]=DataType_octet_string;
					p8tx[8]=6;
					MR((u32)p8tx+9,(u32)p8s+4,6);
					Data_Inverse((u32)p8tx+9,6);//数据倒序(高低字节调换),入口Le>=2
				//邻居节点网络标识号 array octet -string(SIZE(3))
					p8tx[15]=DataType_array;
					n=p8s[0];
					if(n<=127)
					{
						p8tx[16]=n;
						Len=17;
					}
					else
					{
						p8tx[16]=0x81;
						p8tx[17]=n;
						Len=18;
					}
					p8s+=10;
					for(i=0;i<n;i++)
					{
						p8tx[Len+0]=DataType_octet_string;
						p8tx[Len+1]=0x03;
						p8tx[Len+2]=p8s[2];
						p8tx[Len+3]=p8s[1];
						p8tx[Len+4]=p8s[0];
						Len+=5;
						p8s+=5;
					}
				//}
					return Len;
				case 9:
					//属性 9（宽带载波频段序号）∷ =unsigned
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8s=UART_3762_AFNxFn(0x03,16);//查询多网络信息,宽带载波频段序号等(自动暂停和恢复路由);入口:p8Buff=数据放置缓冲
					//376.2上行报文数据单元格式
					//宽带载波频段  BIN 1
					//宽带载波频段：0：1.953～11.96MHz；1： 2.441～5.615 MHz；2～255表示保留
					p8tx[0]=DataType_unsigned;
					p8tx[1]=p8s[0];
					Len=2;
					return Len;

				case 10:
					n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
					p8s=(u8*)ADDR_DATABUFF;
					//节点起始序号：每次查询必须从序号1起始查询
					p8tx[0]=DataType_array;
					Len=2;
					Array=0;//填入配置组数
					while(1)
					{
						UART_3762_AFN10F31(PORTn,n+1,15,(u8*)ADDR_DATABUFF);//查询相位信息(只暂停路由,没恢复);入口:StartNo=节点序号从1开始,其中1为主节点,后续为从节点,Num=节点数量,p8Buff=数据放置缓冲
						p8s=(u8*)ADDR_DATABUFF;
						NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
						m=p8s[4];//本次应答的节点数量
						if(m==0)
						{
							break;
						}
						p8s+=5;
						while(m)
						{
							if(n>=NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<13))//每节点字节数
							{//发送缓冲不够,分帧
								if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								{
									UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
								p8tx[1]=Array;//填入配置组数
								//路由恢复
								Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
								Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
								return Len;
							}
						//节点相位信息单元  属性2::=structure
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=3;
						//{

							//节点序号
							p8tx[Len+2]=DataType_long_unsigned;
							p8tx[Len+3]=(n + 1)>>8;
							p8tx[Len+4]=(n + 1);

							//节点地址   octet-string(SIZE(6)),
							p8tx[Len+5]=DataType_octet_string;
							p8tx[Len+6]=6;
							MR((u32)p8tx+Len+7,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+7,6);//数据倒序(高低字节调换),入口Le>=2

							//节点相位信息},
							p8tx[Len+13]=DataType_long_unsigned;
							p8tx[Len+14]=p8s[7];
							p8tx[Len+15]=p8s[6];
						//}
							p8s+=8;
							Len+=16;
							LENmax_TxSPACE-=16;
							Array++;//填入配置组数
							n++;
							m--;
						}
						if(m)
						{//完
							break;
						}
					}
					if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					{
						UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					}
					UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
					p8tx[1]=Array;//填入数组数
					//路由恢复
					Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
					Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
					return Len;
					
				case 11:
					//台区识别标志 enum{主节点(0),从节点(1)},
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8s=UART_3762_AFNxFn(0x10,4);//查询台区识别状态读取,暂无协议读取，只支持AFN05_F6
					p8tx[0]=DataType_enum;
					if(p8s[7] & 0x08)
						p8tx[1]=1;
					else
						p8tx[1]=0;
					Len=2;
					return Len;

				case 12:
					//属性  12（节点版本信息，只读）∷=array  节点版本信息单元
#if 0
					节点版本信息单元∷=structure
					{
						节点地址  octet-string(SIZE(6))，
						节点类型  enum{主节点(0)，从节点(1)}，
						软件版本号  long-unsigned，
						软件版本日期  Date，
						模块厂商代码  visible-string(SIZE(2))，
						芯片代码  visible-string(SIZE(2))
					}				
#endif
					n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
					if(LENmax_TxSPACE<29)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					// 主节点版本信息
					p8tx[0]=DataType_array;
					p8s1=(u8*)ADDR_DATABUFF;
					MR((u32)p8s1,ADDR_F209_2,Get_DL698DataLen_S((u8*)ADDR_F209_2,0));
					
					Len=2;
					p8tx[Len+0]=DataType_structure;
					p8tx[Len+1]=6;	
					
					//节点地址	 octet-string(SIZE(6)),
					p8tx[Len+2]=DataType_octet_string;
					p8tx[Len+3]=6;
					MR((u32)p8tx+Len+4,ADDR_4001+2,6);
					Data_Inverse((u32)p8tx+Len+4,6);//数据倒序(高低字节调换),入口Le>=2

					//节点类型 enum{主节点(0),从节点(1),
					p8tx[Len+10]=DataType_enum;
					p8tx[Len+11]=0;

					//软件版本号  ,
					p8tx[Len+12]=DataType_long_unsigned;
					p8tx[Len+13]=p8s1[29];
					p8tx[Len+14]=p8s1[30];

					// 软件版本日期  Date
					p8tx[Len+15]=DataType_date;
					p8tx[Len+16]=p8s1[23];
					p8tx[Len+17]=p8s1[24];
					p8tx[Len+18]=p8s1[25];
					p8tx[Len+19]=p8s1[26];
					p8tx[Len+20]=0;
					
					//  厂商代码    visible-string(SIZE(2))，
					p8tx[Len+21]=DataType_visible_string;
					p8tx[Len+22]=2;
					p8tx[Len+23]=p8s1[16];
					p8tx[Len+24]=p8s1[17];

					//  芯片代码    visible-string(SIZE(2))，
					p8tx[Len+25]=DataType_visible_string;
					p8tx[Len+26]=2;
					p8tx[Len+27]=p8s1[20];
					p8tx[Len+28]=p8s1[21];

					Len+=29;
					LENmax_TxSPACE-=29;

					// 从节点信息
					p8s=(u8*)ADDR_AFN10F104;
					n=p8s[0];
					n|=p8s[1]<<8;
					p8s+=2;
					Array=1;//填入配置组数
					while(1)
					{
						if(n==0)
						{
							break;
						}
						while(n)
						{
							if(n >= NUMmax)
							{
								break;
							}
							if((Array>=127)||(LENmax_TxSPACE<29))//每节点字节数
							{//发送缓冲不够,分帧
								if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								{
									UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								else
								{
									UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
								}
								UARTCtrl->NEXTOADSubNum=m;//DL698分帧传输OAD内子计数
								p8tx[1]=Array;//填入配置组数
								return Len;
							}
							p8tx[Len+0]=DataType_structure;
							p8tx[Len+1]=6;	
							
							//节点地址	 octet-string(SIZE(6)),
							p8tx[Len+2]=DataType_octet_string;
							p8tx[Len+3]=6;
							MR((u32)p8tx+Len+4,(u32)p8s,6);
							Data_Inverse((u32)p8tx+Len+4,6);//数据倒序(高低字节调换),入口Le>=2

							//节点类型 enum{主节点(0),从节点(1),
							p8tx[Len+10]=DataType_enum;
							p8tx[Len+11]=1;

							//软件版本号  ,
							p8tx[Len+12]=DataType_long_unsigned;
							i=p8s[6]+(p8s[7]<<8);
							i=bcd_hex(i);
							p8tx[Len+13]=i>>8;;
							p8tx[Len+14]=i;

							// 软件版本日期  Date
							p8tx[Len+15]=DataType_date;
							i=p8s[10];	//年
							i=bcd_hex(i);
							i+=2000;
							p8tx[Len+16]=i>>8;
							p8tx[Len+17]=i;
							i=p8s[9];	//月
							i=bcd_hex(i);
							p8tx[Len+18]=i;
							i=p8s[8];	//日
							i=bcd_hex(i);
							p8tx[Len+19]=i;
							p8tx[Len+20]=0;	//周

							//  厂商代码    visible-string(SIZE(2))，
							p8tx[Len+21]=DataType_visible_string;
							p8tx[Len+22]=2;
							i=p8s[11]+(p8s[12]<<8);
							p8tx[Len+23]=i>>8;
							p8tx[Len+24]=i;
							
							//  芯片代码    visible-string(SIZE(2))，
							p8tx[Len+25]=DataType_visible_string;
							p8tx[Len+26]=2;
							i=p8s[13]+(p8s[14]<<8);
							p8tx[Len+27]=i>>8;
							p8tx[Len+28]=i;

							p8s+=15;
							Len+=29;
							LENmax_TxSPACE-=29;
							Array++;//填入配置组数
							m++;
							n--;
						}
						if(n)
						{//完
							break;
						}
					}
					if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					{
						UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
					}
					UARTCtrl->NEXTOADSubNum=m;//DL698分帧传输OAD内子计数
					p8tx[1]=Array;//填入数组数
					return Len;

				case 20:
					//属性 20（网络规模）∷ =unsigned
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8s=UART_3762_AFNxFn(0x10,9);//查询多网络信息,宽带载波频段序号等(自动暂停和恢复路由);入口:p8Buff=数据放置缓冲
					p8tx[0]=DataType_long_unsigned;
					p8tx[1]=p8s[1];
					p8tx[2]=p8s[0];
					Len=3;
					return Len;
					
				case 22:
					//拒绝节点上报使能标志
					if(LENmax_TxSPACE<2)
					{
						GET_Request_Frame(PORTn);//分帧标志
						return 0;
					}
					p8tx[0]=DataType_enum;
					i=MRR(ADDR_F209_22+1,1);
					p8tx[1]=(i&0x01);
					Len=2;
					return Len;
			#endif
				default:
					UARTCtrl->DAR=6;//对象不存在
					return 0;
			}
			//return 0;
			
		default:
			UARTCtrl->DAR=6;//对象不存在
			return 0;
	}
}


#if (USER/100)==9//河南
#if (((Project/100)==2)||((Project/100)==4))//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
//河南
//属性2电表接线错误信息表∷= array电表接线错误信息
//电表接线错误信息∷=structure
//{
//台区节点总数 unsigned，
//节点地址 octet-string，
//节点信息 bit-string(SIZE(8))，
//节点台区地址 octet-string
//}
//节点信息：b0为表档案，b1-2为台区状态，b3-5为相序，b6为线路异常，b7为保留
//a)	表档案：0表示不是集中器下发的表档案，1表示是集中器下发的表档案。
//b)	台区状态：0表示节点台区地址不确定；1表示节点台区地址和本集中器相同，属于本台区；2表示节点台区地址和本集中器地址不相同；
//c)	相序：节点所在电源相别，置0表示不确定；置1依次表示第1、2、3相（001代表A相；010代表B相；100代表C相；111代表三相）；
//d)	线路异常：从节点线路异常标记：置“1”时，表示从节点线路有异常。当电能表为单相供电时，线路异常表示零火线反接；当电能表为三相供电时，线路异常表示逆相序。
//e)	节点台区地址：节点地址归属的台区地址（包括跨台区地址），不确定全填F；
u32 GET_Request_FF10(DL698_SETLIB_TypeDef *pLib,u32 PORTn,u32 OAD,u8 *p8tx,u32 LENmax_TxSPACE)//读一个对象属性;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR
{
	u32 i;
	u32 x;
	u32 Len;
	u8* p8s;
//	u8* p8d;
	UARTCtrl_TypeDef *UARTCtrl;
	u32 Array;
	u32 NUMmax;
	u32 n;
	u32 m;
	
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	UARTCtrl->DAR=0;//数据访问结果;成功
	if(LENmax_TxSPACE<2)
	{
		GET_Request_Frame(PORTn);//分帧标志
		return 0;
	}
	
	i=0;
	n=UARTCtrl->NEXTOADSubNum;//DL698分帧传输OAD内子计数
	p8s=(u8*)ADDR_DATABUFF;
	//节点起始序号：下行报文中首帧从0开始
	while(i<n)
	{//不是从0开始
		m=n-i;
		if(m>15)
		{
			m=15;
		}
		UART_3762_AFN03F101(PORTn,i,m,(u8*)ADDR_DATABUFF);//(河南)查询节点台区信息(只暂停路由,没恢复);入口:StartNo=节点起始序号首帧从0开始,Num=节点数量(最大支持15个),p8Buff=数据放置缓冲
		NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
		if(n>=NUMmax)
		{
			break;
		}
		m=p8s[2];//本次应答的节点数量
		if(m==0)
		{
			break;
		}
		i+=m;
	}
	
	p8tx[0]=DataType_array;
	p8tx[1]=0;
	Len=2;
	Array=0;//填入配置组数
	while(1)
	{
		UART_3762_AFN03F101(PORTn,n,15,(u8*)ADDR_DATABUFF);//(河南)查询节点台区信息(只暂停路由,没恢复);入口:StartNo=节点起始序号首帧从0开始,Num=节点数量(最大支持15个),p8Buff=数据放置缓冲
		p8s=(u8*)ADDR_DATABUFF;
//上行：
	//台区节点总数量	BIN	2
	//本次上报节点数量n(n<=15)	BIN	1
		
	//节点1地址	BCD	6
	// b7   	 b6	    b5-b3	  b2-b1	     b0	     BS	1
	//保留	线路异常	  相序	 台区状态	  表档案		
	//节点1台区地址	BCD	6
	//…	…	…
	//a)	表档案：0表示不是集中器下发的表档案，1表示是集中器下发的表档案。
	//b)	台区状态：0表示节点台区地址不确定；1表示节点台区地址和本集中器相同，属于本台区；2表示节点台区地址和本集中器地址不相同；
	//c)	相序：节点所在电源相别，置0表示不确定；置1依次表示第1、2、3相（001代表A相；010代表B相；100该表C相；111代表三相）；
	//d)	线路异常：从节点线路异常标记：置“1”时，表示从节点线路有异常。当电能表为单相供电时，线路异常表示零火线反接；当电能表为三相供电时，线路异常表示逆相序。
	//e)	节点台区地址：节点地址归属的台区地址（包括跨台区地址），不确定全填F；
		NUMmax=p8s[0]|(p8s[1]<<8);//节点总数量
		m=p8s[2];//本次应答的节点数量
		if(m==0)
		{
			break;
		}
		p8s+=3;
		while(m)
		{
			if(n>=NUMmax)
			{
				break;
			}
			if((Array>=127)||(LENmax_TxSPACE<24))//每节点字节数=
			{//发送缓冲不够,分帧
				if(UARTCtrl->NEXT==0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
				{
					UARTCtrl->NEXT=1;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
				}
				else
				{
					UARTCtrl->NEXT=2;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
				}
				UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
				p8tx[1]=Array;//填入配置组数
				//路由恢复
				Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
				Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
				return Len;
			}
			
		//电表接线错误信息∷=structure
			p8tx[Len+0]=DataType_structure;
			p8tx[Len+1]=4;
		//{
		//台区节点总数 unsigned，
			p8tx[Len+2]=DataType_long_unsigned;
			p8tx[Len+3]=NUMmax>>8;
			p8tx[Len+4]=NUMmax;
		//节点地址 octet-string，
			Data_Inverse((u32)p8s,6);//数据倒序(高低字节调换),入口Le>=2
			p8tx[Len+5]=DataType_octet_string;
			p8tx[Len+6]=6;
			MR((u32)p8tx+Len+7,(u32)p8s,6);
		//节点信息 bit-string(SIZE(8))，
	// b7   	 b6	    b5-b3	  b2-b1	     b0	     BS	1
	//保留	线路异常	  相序	 台区状态	  表档案		
			x=p8s[6];
			i=Bit_Inverse(x,8);//寄存器数据位倒序(高低位调换),入口:Data=寄存器数据,bit=需倒序的位个数;返回:位倒序后的数据
//			i&=0x83;
//			i|=(x&0x06)<<4;
//			i|=(x&0x38)>>1;
			p8tx[Len+13]=DataType_bit_string;
			p8tx[Len+14]=8;
			p8tx[Len+15]=i;
		//节点台区地址 octet-string
			Data_Inverse((u32)p8s+7,6);//数据倒序(高低字节调换),入口Le>=2
			p8tx[Len+16]=DataType_octet_string;
			p8tx[Len+17]=6;
			MR((u32)p8tx+Len+18,(u32)p8s+7,6);
		//}
			p8s+=13;
			Len+=24;
			LENmax_TxSPACE-=24;
			Array++;//填入配置组数
			n++;
			m--;
		}
		if(m)
		{//完
			break;
		}
	}
	if(UARTCtrl->NEXT!=0)//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	{
		UARTCtrl->NEXT=3;//DL698分帧传输标志:0=不分帧,1=分帧第1帧,2=分帧中间帧,3=分帧最后帧;B7位=1表示已在发送区的APDU再分帧传输
	}
	UARTCtrl->NEXTOADSubNum=n;//DL698分帧传输OAD内子计数
	p8tx[1]=Array;//填入数组数
	//路由恢复
	Terminal_Router->StopFlags&=~(1<<3);//b3=UART读路由信息
	Terminal_Router->RouterCtrl=3;//路由器控制0=空,1=重启,2=暂停,3=恢复,4=读状态;完成后自动回到0
	return Len;
	
}
#endif
#endif










