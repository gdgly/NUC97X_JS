
//UART设置参数
#include "Project.h"
#include "Terminal_Uart_AFN04_Paramter.h"
#include "../Terminal/Terminal_AFN04_Paramter_Check.h"
#include "../Terminal/Terminal_Uart_3761.h"
#include "../Terminal/Terminal_Uart_3761_COM.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../Device/IC_TESAM.h"
#include "../Device/IC_BUZZ.h"
#include "../Calculate/Calculate.h"
#include "../Calculate/DataConversion.h"
#include "Terminal_AFN0E_Event.h"
#include "Terminal_AFN0C_RealTimeData.h"
#include "Terminal_Init.h"
#include "Terminal_AFN0D_SourceData.h"
#include "../STM32F4xx/STM32F4xx_WDG.h"
#include "Terminal_AFN0C_RealTimeData_Init.h"
#include "Terminal_AFN0D_CongealData_Init.h"
#include "../Device/RTC.h"

	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
#include "../STM32F4xx/STM32F4xx_RTC.h"
#include "Terminal_Statistic.h"
	#endif


extern TerminalParamterLib_TypeDef     TerminalParamterLib[];


void Uart_AFN04F10_ERR_READDR(u32 PORTn)//AFN04F10电能表配置地址重复错误返回帧
{
	u32 DIlen;
	u8 * p8rxbuff;
	u8 * p8txbuff;
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)(Get_ADDR_UARTnTx(PORTn));
	p8txbuff[12]=0;//AFN=0
	
	p8txbuff[14]=0;//Pn=0
	p8txbuff[15]=0;
	p8txbuff[16]=4;//Fn=3(对收到报文中的全部数据单元标识进行逐个确认/否认),Pn=0
	p8txbuff[17]=0;
	p8txbuff[18]=0x04;//要被确认的报文的AFN=04H
	p8txbuff[19]=0;//Pn=0
	p8txbuff[20]=0;
	p8txbuff[21]=2;//Fn=10
	p8txbuff[22]=1;
	p8txbuff[23]=2;//ERR码=2(表地址重复)

	p8txbuff[1]=18;//数据长度
	p8txbuff[2]=0;
	p8txbuff[6]&=0x0f;
	p8txbuff[6]|=0x80;//控制码DIR=1上行,PRM=0表示此帧报文来自从动站,要求访问位ACD暂0,功能码=0
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}


//参数设置
void Uart_AFN04(u32 PORTn)//参数设置
{
	u32 i;
	u32 x;
	u32 y;
	u32 z;
	u32 w;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 f;
	u64 a64;
	u64 b64;
	
	u8 * p8;
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
//	u32 *p32;
#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
	u32 *p32;
	u64 d64a;
	UARTCtrl_TypeDef * UARTCtrl;
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
#endif
	
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	if(DIlen<(12+16))
	{
		goto err;
	}
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+16+6))
		{
			goto err;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
//PW认证
	i=Uart_376_PW(PORTn);//消息认证码字段PW;入口PW首址,出口0有效,1无效,4已返错误帧
	if(i==4)
	{
		return;
	}
	if(i!=0x0)
	{
err:
		//全部否认
		Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		return;
	}
/*
//若红外口需设置开关
	if(PORTn==IRPORT)
	{
    if((Comm_Ram->Factory!=0x55)&&(MRR(ADDR_MeterFMTimer,2)==0))
		{
			goto err;
		}
	}
*/
	f=0;//f=0检查全部数据是否全部有效,f=1写入
loop:
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识和参数数据长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+16+6);
	}
	else
	{
		DIlen-=(8+16);
	}
	if(DIlen<=4)
	{
		goto err;
	}
//数据(启动站功能码10（请求1级数据）用于应用层请求确认（CON=1）的链路传输)
	DIaddr=((u32)p8rxbuff)+14;
	RXaddr=((u32)p8rxbuff)+14+4;
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	p8rxbuff[0]=1;//用于ERC3参数变更记录的DI标识个数计数
	while(DIlen>4)
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
			i=p8rxbuff[0]*4;
			if(f!=0x0)
			{
				MR(((u32)p8rxbuff)+14+(p8rxbuff[0]*4),DIaddr,4);//用于ERC3参数变更记录的DI标识连接
				p8rxbuff[0]+=1;//用于ERC3参数变更记录的DI标识个数计数+1
			}
		}
		else
		{
			Fn=(i>>16);
			Pn=i&0xffff;
			p8=(u8 *)(RXaddr);
		#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
			if((Fn==297)&&(Pn==0))
			{
				goto AFN04F297;	
			}
			if((Fn==298)&&(Pn==0))
			{
				goto AFN04F298;	
			}
			if((Fn==299)&&(Pn==0))
			{
				goto AFN04F299;	
			}
		#endif
		#if (USER/100)==12//四川用户
			if(Fn==151)
			{
				if((DIlen-4)<1)
				{
					goto err;
				}
				goto AFN04F151;
			}
		#endif
			if(Fn>Get_AFN04ParamterLib_MaxFn())
			{
				goto err;
			}
			if(Fn==0x0)
			{
				goto err;
			}
			if(Pn>TerminalParamterLib[Fn-1].PnMax)
			{
				goto err;
			}
			if(TerminalParamterLib[Fn-1].PnType!=0x0)
			{
				if(Pn==0x0)
				{
					goto err;
				}
			}
			if((TerminalParamterLib[Fn-1].project&(1<<(Project/100)))==0x0)
			{//本方案无此Fn功能
				goto err;
			}
			switch(Fn)//Fn
			{
				case 0:
					goto err;
				case 2://终端上行通信口无线中继转发设置
					i=(p8[0]&0x7f);
					if(i>16)
					{
						goto err;
					}
					i=(i*2)+1;
					break;
				case 7://终端IP地址和端口设置
					i=p8[20];
					if(i>20)//用户名长度m(0～20)
					{
						goto err;
					}
					i=20+1+i;
					if(p8[i]>20)//密码长度n(0～20)
					{
						goto err;
					}
					i+=(1+p8[i]+2);
					break;
				case 10://终端电能表／交流采样装置配置参数设置
					if(f==0x0)
					{
					#if ((USER/100)==5)//上海用户
						for(i=0;i<(p8[0]+(p8[1]<<8));i++)
						{
							if(p8[7+(i*LEN_AFN04F10_Pn)]==21)
							{//上海规约
								//第3字节：制造厂代码，1字节ASCII码，如P，则该抄表帧里为50H，主站下发给终端为80H(BCD码)
								p8[11+(i*LEN_AFN04F10_Pn)]=bcd_hex(p8[11+(i*LEN_AFN04F10_Pn)]);//8位压缩BCD数转为HEX	
							}
						}
					#endif
						i=p8[0]+(p8[1]<<8);
						if((DIlen-4)<((i*LEN_AFN04F10_Pn)+2))
						{
							goto err;
						}
						//同时配置多个时测量点号重复检查
						for(x=0;x<i;x++)
						{
							p8=(u8 *)(RXaddr+(x*LEN_AFN04F10_Pn)+4);
							a64=p8[0]+(p8[1]<<8);
							if(a64==0)
							{//测量点号=0不比较
								continue;
							}
							z=0;
							p8=(u8 *)(RXaddr+4);
							for(y=0;y<i;y++)
							{
								b64=p8[0]+(p8[1]<<8);
								if(a64==b64)
								{
									z++;
								}
								p8+=LEN_AFN04F10_Pn;
							}
							if(z>1)
							{
								goto err;
							}
						}
						//同时配置多个时通信地址重复检查
						for(x=0;x<i;x++)
						{
							p8=(u8 *)(RXaddr+(x*LEN_AFN04F10_Pn)+4);
							w=p8[0]+(p8[1]<<8);
							if(w==0)
							{//测量点号=0
								continue;
							}
							w=p8[2];//端口号
							w&=0x1f;
							a64=MRR(((u32)p8)+4,6);
							a64=(p8[4+5]<<24)+(p8[4+4]<<16)+(p8[4+3]<<8)+p8[4+2];
							a64<<=16;
							a64+=(p8[4+1]<<8)+p8[4];
							z=0;
							for(y=0;y<i;y++)
							{
								p8=(u8 *)(RXaddr+(y*LEN_AFN04F10_Pn)+4);
								if((p8[0]+(p8[1]<<8))==0)
								{//测量点号=0
									continue;
								}
								if(w!=(p8[2]&0x1f))
								{//端口号不同
									continue;
								}
								b64=(p8[4+5]<<24)+(p8[4+4]<<16)+(p8[4+3]<<8)+p8[4+2];
								b64<<=16;
								b64+=(p8[4+1]<<8)+p8[4];
								if(a64==b64)
								{
									z++;
								}
							}
							if(z>1)
							{
								Uart_AFN04F10_ERR_READDR(PORTn);//AFN04F10电能表配置地址重复错误返回帧
								return;
							}
						}
						//
						p8=(u8 *)(RXaddr);
						i=p8[0]+(p8[1]<<8);
						DIlen-=((i*LEN_AFN04F10_Pn)+2);
						RXaddr+=((i*LEN_AFN04F10_Pn)+2);
						p8+=2;
						for(;i!=0x0;i--)
						{
							y=p8[0]+(p8[1]<<8);
							if(y==0x0)
							{//序号=0
								goto err;
							}
							if(y>AFN04F10MaxPn)
							{//序号>Max
								goto err;
							}
							x=p8[2]+(p8[3]<<8);
							if(x>MaxRS485AddCarrierPn)
							{//测试点号超值错
								goto err;
							}
							if(x!=0)
							{
								z=p8[4];
								z&=0x1f;
								a64=MRR(((u32)p8)+6,6);
								z=Check_AFN04F10_Addr(z,a64);//检查终端电能表/交流采样装置配置的通信地址;返回0无相同,!=0有相同地址
								if(z!=0)
								{//有相同通信地址
									if(z!=y)
									{//配置序号不同
										//goto err;
										Uart_AFN04F10_ERR_READDR(PORTn);//AFN04F10电能表配置地址重复错误返回帧
										return;
									}
								}
							}

							x=Check_AFN04F10_Pn(x);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
							if(x==y)
							{//配置序号相同
								x=0;
							}
							x|=Check_AFN04F11_Pn(p8[2]+(p8[3]<<8));//检查终端脉冲配置参数指定的测量点号;返回0无,1有
							x|=Check_AFN04F13_Pn(p8[2]+(p8[3]<<8));//检查终端电压/电流模拟量配置参数指定的测量点号;返回0无,1有
							if(x!=0)
							{//测量点号重复错
								goto err;
							}
							p8+=LEN_AFN04F10_Pn;
						}
					}
					else
					{
						RXaddr+=2;
						DIlen-=2;
						for(i=0;i<(p8[0]+(p8[1]<<8));i++)
						{
							p8d=(u8 *)(ADDR_AFN04F10+(((p8[2+(i*LEN_AFN04F10_Pn)]+(p8[1+2+(i*LEN_AFN04F10_Pn)]<<8))-1)*LEN_AFN04F10_Pn));
							x=MRR(((u32)p8d)+2,2);//原测量点号
							y=MRR(RXaddr+2,2);//设置的测量点号
						#if (USER/100)==13//黑龙江
							if(x==1)
							{//原测量点号
								z=MRR(((u32)p8d)+4,1);//原端口号
								z&=0x1f;
								if(z==1)
								{//原端口号
									if(MRR(((u32)p8d)+5,1)==2)
									{//原通信协议类型 BIN 1 (其中0：表示终端无需对本序号的电能表/交流采样装置进行抄表；1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议；其他：备用)
										w=MRR(RXaddr+4,1);//设置的端口号
										w&=0x1f;
										if((y!=1)||(w!=1)||(MRR(RXaddr+5,1)!=2))
										{//设置的测量点号!=1或设置的端口号!=1或设置的通信协议类型不是交采
											goto err;
										}
									}
								}
							}
						#endif
					#if ((Project/100)==2)//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=公变终端,500=集中器Ⅱ型,600=通信模块,700=
							//通信地址变化即同步路由档案
						#if ((USER/100)==11)||((USER/100)==12)//用户表示：吉林//吉林流水线台体测载波数据转发需立即档案同步 四川透明任务测试要立即档案同步
							Terminal_Ram->RouterSynDelay_S_Timer=0;//29 路由档案同步延时执行秒定时器
						#else
							Terminal_Ram->RouterSynDelay_S_Timer=30;//29 路由档案同步延时执行秒定时器
						#endif			
							if(Terminal_Router->AFN04F10==0)//AFN04F10设置变化标志0=没,1=变化;用于档案同步
							{
								z=MRR(((u32)p8d)+4,1);//原端口号
								z&=0x1f;
								w=MRR(RXaddr+4,1);//设置的端口号
								w&=0x1f;
								if((z==31)||(w==31))
								{//与载波口相关
									if(x!=y)
									{//测量点号变化
										Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
									}
									if((x!=0)||(y!=0))
									{//与测量点号相关
										if(z!=w)
										{//端口号变化
											Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
										}
									}
									if((y!=0)&&(x!=0))
									{//原和新设置的测量点号均有效
										a64=MRR(((u32)p8d)+6,6);//原通信地址
										b64=MRR(RXaddr+6,6);//设置的通信地址
										if(a64!=b64)
										{//通信地址变化
											Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
										}
										a64=MRR(((u32)p8d)+20,6);//原采集器地址
										b64=MRR(RXaddr+20,6);//设置的采集器地址
										if(a64!=b64)
										{//采集器地址变化
											Terminal_Router->AFN04F10=1;//AFN04F10设置变化标志0=没,1=变化;用于档案同步
										}
									}
								}
							}
							//新设置将1-31日抄表设为全成功,免删除
							z=MRR(ADDR_AFN05F149+1,1);//自动维护状态：00－关闭，01－启用并更新F10，02－启用但不更新F10，缺省为关闭
							if(z==0x1)
							{	
								if(y!=0)
								{
									MC(0,ADDR_RMDateFlags+(4*(y-1)),4);//测量点日抄表成功失败标志;B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
								}
							}
					#endif
							if(x!=y)
							{//测量点号变化
								Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
								//Init_Class1Data(pnType,x);//类1数据初始化(指定Pn数据)
								Init_Class1Data(pnType,y);//类1数据初始化(指定Pn数据)
								//Init_Class2Data(x);//类2数据初始化(指定的测量点数据,Pn=1~n)
								Init_Class2Data(y);//类2数据初始化(指定的测量点数据,Pn=1~n)
								Init_Class3Data(y);//类3数据(事件)初始化(指定的测量点数据,Pn=1~n)
								//Init_EnergyInc_Souser_Pn(x);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
								Init_EnergyInc_Souser_Pn(y);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
								//Init_Energy_Pn(x);//测量点日月电能量清为0(指定的Pn数据)
								Init_Energy_Pn(y);//测量点日月电能量清为0(指定的Pn数据)
							}
							//MW(RXaddr,TerminalParamterLib[Fn-1].Addr+(((p8[2+(i*LEN_AFN04F10_Pn)]+(p8[1+2+(i*LEN_AFN04F10_Pn)]<<8))-1)*LEN_AFN04F10_Pn),LEN_AFN04F10_Pn);
							RXaddr+=LEN_AFN04F10_Pn;
							DIlen-=LEN_AFN04F10_Pn;
						}
						//写入
						x=p8[2]+(p8[3]<<8);//写入开始配置序号
						y=0;//连续序号个数
						z=((u32)p8)+2;//写入的缓冲开始
						for(i=0;i<(p8[0]+(p8[1]<<8));i++)
						{
							w=p8[2+(i*LEN_AFN04F10_Pn)]+(p8[1+2+(i*LEN_AFN04F10_Pn)]<<8);//当前配置序号
							if(w!=(x+y))
							{//不连续
								MW(z,ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn),y*LEN_AFN04F10_Pn);
								x=w;//写入开始配置序号
								y=0;//连续序号个数
								z=((u32)p8)+2+(i*LEN_AFN04F10_Pn);//写入的缓冲开始
							}
							y++;
						}
						MW(z,ADDR_AFN04F10+((x-1)*LEN_AFN04F10_Pn),y*LEN_AFN04F10_Pn);
					#if ((USER/100)==5)//上海     //批量检测发现测试其中1路485后，直接修改测量点端口为另一个485，发F129去抄读电表数据，返回无效值，需发送端口重新抄表命令才可以
            if(MRR(ADDR_AFN05F50+1,1)!=0x55)
						{
                MC(0x55,ADDR_AFN05F51+2-1,1);
            }
            if(MRR(ADDR_AFN05F50+2,1)!=0x55)
						{
                MC(0x55,ADDR_AFN05F51+3-1,1);
            }
        #endif							
						PnVSAFN04F10SetNoList();//测量点号Pn对应AFN04F10配置序号列表
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
						AFN04F150_Computer();//测量点有效标志计算
						Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
						Terminal_Ram->F246PnListUpDate=0;//要抄读的掉电记录数据Pn列表已更新标志,0=要更新,!=0已更新
					}
					continue;
				case 11://终端脉冲配置参数设置
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*5)+1))
						{
							goto err;
						}
						//本次配置内相同测量点号和属性检查
						for(x=0;x<i;x++)
						{
							p8=(u8 *)(RXaddr+(x*5)+2);
							a64=p8[0];
							if(a64==0)
							{//测量点号=0不比较
								continue;
							}
							a64+=(p8[1]<<8);
							z=0;
							p8=(u8 *)(RXaddr+2);
							for(y=0;y<i;y++)
							{
								b64=MRR((u32)p8,2);
								if(a64==b64)
								{
									z++;
								}
								p8+=5;
							}
							if(z>1)
							{
								goto err;
							}
						}
						//
						p8=(u8 *)(RXaddr);
						i=p8[0];
						DIlen-=((i*5)+1);
						RXaddr+=((i*5)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							y=p8[0];
							if(y==0x0)
							{//序号=0
								goto err;
							}
							if(y>AFN04F11MaxPn)
							{//序号>Max
								goto err;
							}
							y=p8[1];
							if(y>MaxRS485AddCarrierPn)
							{//测试点号超值错
								goto err;
							}
							x=Check_AFN04F10_Pn(y);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
							//x|=Check_AFN04F11_Pn(y);//检查终端脉冲配置参数指定的测量点号;返回0无,1有
							x|=Check_AFN04F13_Pn(y);//检查终端电压/电流模拟量配置参数指定的测量点号;返回0无,1有
							if(x!=0)
							{//测量点号重复错
								goto err;
							}
							if(y!=0)
							{//测量点号!=0
								y+=(p8[2]<<8);
								z=AFN04F11MaxPn;
								for(x=0;x<z;x++)
								{
									if(y==MRR(ADDR_AFN04F11+(5*x)+1,2))
									{//相同测量点号和属性
										if(p8[0]!=(x+1))
										{//不同端口号
											goto err;
										}
									}
								}
							}
							p8+=5;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							x=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//原测量点号和脉冲属性
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5),5);
							y=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*5)]-1)*5)+1,2);//设置后测量点号和脉冲属性
							if(x!=y)
							{//测量点号或脉冲属性变化
								//清端口原记录的脉冲输入端口电能
								MC(0,ADDR_Terminal_impIn_Energy+((4+(5*(TMaxTariff+1)))*(p8[1+(i*5)]-1)),4+(5*(TMaxTariff+1)));
								//清端口原记录的脉冲端口当月最大需量和时间
								MC(0,ADDR_Terminal_impIn_Demand+((7*(TMaxTariff+1))*(p8[1+(i*5)]-1)),7*(TMaxTariff+1));
								//
								x&=0xff;
								y&=0xff;
								Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
								Init_Class1Data(pnType,x);//类1数据初始化(指定Pn数据)
								Init_Class1Data(pnType,y);//类1数据初始化(指定Pn数据)
								Init_Class2Data(x);//类2数据初始化(指定的测量点数据,Pn=1~n)
								Init_Class2Data(y);//类2数据初始化(指定的测量点数据,Pn=1~n)
								Init_EnergyInc_Souser_Pn(x);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
								Init_EnergyInc_Souser_Pn(y);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
								Init_Energy_Pn(x);//测量点日月电能量清为0(指定的Pn数据)
								Init_Energy_Pn(y);//测量点日月电能量清为0(指定的Pn数据)
							}
							RXaddr+=5;
							DIlen-=5;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
						AFN04F150_Computer();//测量点有效标志计算
					}
					continue;
				case 13://终端电压/电流模拟量配置参数设置
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*3)+1))
						{
							goto err;
						}
						DIlen-=((i*3)+1);
						RXaddr+=((i*3)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							y=p8[0];
							if(y==0x0)
							{//序号=0
								goto err;
							}
							if(y>AFN04F13MaxPn)
							{//序号>Max
								goto err;
							}
							y=p8[1];
							if(y>MaxRS485AddCarrierPn)
							{//测试点号超值错
								goto err;
							}
							x=Check_AFN04F10_Pn(y);//检查终端电能表/交流采样装置配置参数指定的测量点号;返回0无,!=0为相同测量点号的配置序号(1开始)
							x|=Check_AFN04F11_Pn(y);//检查终端脉冲配置参数指定的测量点号;返回0无,1有
							//x|=Check_AFN04F13_Pn(y);//检查终端电压/电流模拟量配置参数指定的测量点号;返回0无,1有
							if(x!=0)
							{//测量点号重复错
								goto err;
							}
							p8+=3;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							x=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*3)]-1)*3)+1,1);//原测量点号
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*3)]-1)*3),3);
							y=MRR(TerminalParamterLib[Fn-1].Addr+((p8[1+(i*3)]-1)*3)+1,1);//设置后测量点号
							if(x!=y)
							{//测量点号变化
								Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
								Init_Class1Data(pnType,x);//类1数据初始化(指定Pn数据)
								Init_Class1Data(pnType,y);//类1数据初始化(指定Pn数据)
								Init_Class2Data(x);//类2数据初始化(指定的测量点数据,Pn=1~n)
								Init_Class2Data(y);//类2数据初始化(指定的测量点数据,Pn=1~n)
								Init_EnergyInc_Souser_Pn(x);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
								Init_EnergyInc_Souser_Pn(y);//测量点电能增量计算用原始数据清为0xee(指定的Pn数据)
								Init_Energy_Pn(x);//测量点日月电能量清为0(指定的Pn数据)
								Init_Energy_Pn(y);//测量点日月电能量清为0(指定的Pn数据)
							}
							RXaddr+=3;
							DIlen-=3;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
						AFN04F150_Computer();//测量点有效标志计算
					}
					continue;
				case 14://终端总加组配置参数设置
					if(f==0x0)
					{
						x=1;
						for(i=0;i<p8[0];i++)
						{
							x+=p8[x+1]+2;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							if(p8[x]==0x0)
							{
								goto err;
							}
							if(p8[x]>AFN04F14MaxPn)
							{
								goto err;
							}
							for(y=0;y<p8[x+1];y++)
							{//参与总加的测量点号检查
								if((p8[x+2+y]&0x3f)>=MaxRS485User)
								{
									goto err;
								}
							}
							x+=p8[x+1]+2;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[x]-1)*66),p8[x+1]+2);
							RXaddr+=p8[x+1]+2;
							DIlen-=p8[x+1]+2;
							x+=p8[x+1]+2;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;	
				case 15://有功总电能量差动越限事件参数设置
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*9)+1))
						{
							goto err;
						}
						DIlen-=((i*9)+1);
						RXaddr+=((i*9)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							if(p8[0]==0x0)
							{
								goto err;
							}
							if(p8[0]>AFN04F15MaxPn)
							{
								goto err;
							}
							p8+=9;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*9)]-1)*9),9);
							RXaddr+=9;
							DIlen-=9;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;
				case 22://终端电能量费率设置
					i=p8[0];
					if(i>48)
					{
						goto err;
					}
					i=(i*4)+1;
					break;
				case 31://载波从节点附属节点地址设置
					i=p8[0];
					if(i>20)
					{
						goto err;
					}
					i=(i*6)+1;
					break;
				case 33://终端抄表运行参数设置
					if(f==0x0)
					{
						x=1;
						for(i=0;i<p8[0];i++)
						{
							x+=(p8[x+13]*4)+14;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							if(p8[x]==0x0)
							{
								goto err;
							}
							if(p8[x]>31)
							{
								goto err;
							}
							x+=(p8[x+13]*4)+14;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						x=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[x]-1)*(14+(4*24))),(p8[x+13]*4)+14);
							RXaddr+=(p8[x+13]*4)+14;
							DIlen-=(p8[x+13]*4)+14;
							x+=(p8[x+13]*4)+14;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;
				case 34://集中器下行通信模块的参数设置
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*6)+1))
						{
							goto err;
						}
						DIlen-=((i*6)+1);
						RXaddr+=((i*6)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							if(p8[0]==0x0)
							{
								goto err;
							}
							if(p8[0]>31)
							{
								goto err;
							}
							p8+=6;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*6)]-1)*6),6);
							RXaddr+=6;
							DIlen-=6;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;
				case 35://台区集中抄表重点户设置
					i=p8[0];
				#if (MaxKeyUser<=20)
					if(i>MaxKeyUser)
					{
						goto err;
					}
				#else
					if(i>20)
					{
						goto err;
					}
				#endif
					i=(i*2)+1;
					Terminal_Ram->AFN04F10F11F13F35UpDate=0;//0=AFN04F10,F11,F13,F35配置参数更新标志,!=0没更新
					Terminal_Ram->AFN04F10F35Update=0;//0=AFN04F10,F35配置参数更新标志,!=0没更新;用于集中抄表重点户曲线冻结Pn列表
					break;
				case 37://终端级联通信参数设置
					i=p8[6]&0x0f;
					if(i>3)
					{
						goto err;
					}
					i=(i*4)+7;
					//若是被级联口则初始化速率
//					if(p8[0]==2)
//					{//RS485-1
//						if((p8[6]&0x80)!=0x0)
//						{//被级联
//							UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_1PORT);
//							UARTCtrl->BpsCtrl=p8[1];
//							Init_UARTn(RS485_1PORT);//初始化UART口,入口通信控制字已填入
//						}
//					}
//					if(p8[0]==3)
//					{//RS485-2
//						if((p8[6]&0x80)!=0x0)
//						{//被级联
//							UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_2PORT);
//							UARTCtrl->BpsCtrl=p8[1];
//							Init_UARTn(RS485_2PORT);//初始化UART口,入口通信控制字已填入
//						}
//					}
//					if(p8[0]==4)
//					{//RS485-3
//						if((p8[6]&0x80)!=0x0)
//						{//被级联
//							UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(RS485_3PORT);
//							UARTCtrl->BpsCtrl=p8[1];
//							Init_UARTn(RS485_3PORT);//初始化UART口,入口通信控制字已填入
//						}
//					}
					break;
				case 38://1类数据配置设置
				case 39://2类数据配置设置
					if(f==0x0)
					{
//						if(p8[0]==0)
//						{//大类号0:缺省配置,禁止配置设置
//							goto err;
//						}
						if(p8[0]>15)
						{//大类号0-15
							goto err;
						}
						if(p8[1]==0x0)
						{//本次设置数=0
							goto err;
						}
						if(p8[1]>16)
						{//本次设置数>16
							goto err;
						}
						x=2;
						for(i=0;i<p8[1];i++)
						{
							if(p8[x]>15)
							{//用户小类号0-15
								goto err;
							}
							if(p8[x+1]>31)
							{//信息类组数0-31
								goto err;
							}
							x+=p8[x+1]+2;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
					}
					else
					{
						x=2;
						RXaddr+=2;
						DIlen-=2;
						for(i=0;i<p8[1];i++)
						{
							MW(RXaddr+1,TerminalParamterLib[Fn-1].Addr+(p8[0]*32*16)+(p8[x]*32),p8[x+1]+1);
							RXaddr+=p8[x+1]+2;
							DIlen-=p8[x+1]+2;
							x+=p8[x+1]+2;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;
				case 41://时段功控定值设置
					if(f==0x0)
					{
						if(p8[0]>7)
						{//方案标志D0-D2
							goto err;
						}
						x=1;
						for(i=1;i<7;)
						{
							if((p8[0]&i)!=0x0)
							{
								x+=(BitNum(p8[x])*2)+1;//计算置位为1的个数(0-32)
							}
							i<<=1;
						}
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
					}
					else
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3))),1);
						x=1;
						RXaddr+=1;
						DIlen-=1;
						for(i=1;i<7;)
						{
							if((p8[0]&i)!=0x0)
							{
								switch(i)
								{
									case 1:
										MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1,(BitNum(p8[x])*2)+1);
										break;
									case 2:
										MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+17,(BitNum(p8[x])*2)+1);
										break;
									case 4:
										MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+34,(BitNum(p8[x])*2)+1);
										break;
								}
								RXaddr+=(BitNum(p8[x])*2)+1;//计算置位为1的个数(0-32)
								DIlen-=(BitNum(p8[x])*2)+1;//计算置位为1的个数(0-32)
								x+=(BitNum(p8[x])*2)+1;//计算置位为1的个数(0-32)
							}
							i<<=1;
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;
				case 47://购电量（费）控参数
					if(f==0x0)
					{
						if((DIlen-4)<17)
						{
							goto err;
						}
						p8d=(u8 *)(TerminalParamterLib[Fn-1].Addr+((Pn-1)*17));
						for(i=0;i<4;i++)
						{
							if(p8[i]!=p8d[i])
							{
								break;
							}
						}
						if(i==4)
						{
//#if SoftBug4==0//0=正常功能,1==376主站测试软件BUG.购电单号	
						#if ( ((USER/100)==0)||((USER/100)==10) )//电科院送检,河北系							
							if(p8[4]==0x55)//河北141226要求刷新时不比较单号
							{
								goto err;//购电单号相同
							}	
						#else  //其他系	
							goto err;//购电单号相同
						#endif							
//#endif
						}
						if(p8[4]!=0x55)
						{
							if(p8[4]!=0xaa)
							{
								goto err;
							}
						}
					}
					else
					{
						x=MRR(ADDR_AFN0CF23+(4*(Pn-1)),4);//本次购电前剩余电能量（费）
						if(p8[4]==0x55)
						{//追加
							i=MRR(ADDR_AFN0CF23+(4*(Pn-1)),4);//读当前剩余电量(费)数据格式03
							a64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
							b64=MRR(ADDR_AFN0CF23_DP+(2*(Pn-1)),2);//读当前剩余电量单位G=1时(MWH)小数点后数据,格式hex=0.000
							if((i&0x10000000)!=0x0)
							{//数据格式03为负值
								b64=(~b64)+1;
							}
							a64+=b64;
							i=MRR(RXaddr+5,4);
							b64=Data03_kWh(i);//数据格式03转为单位统一为kwh的hex补码
							a64+=b64;
							a64=kWh_Data03(a64);//单位统一为kwh的hex补码转为数据格式03,当转换后单位为MWh时小数部份(0.000MWh)hex格式在返回的高4字节
							MWR(a64,ADDR_AFN0CF23+(4*(Pn-1)),4);//写寄存器数据到存贮器,Byte<=8
							a64>>=32;
							MWR(a64,ADDR_AFN0CF23_DP+(2*(Pn-1)),2);//写寄存器数据到存贮器,Byte<=8
						}
						else
						{//刷新
							MW(RXaddr+5,ADDR_AFN0CF23+(4*(Pn-1)),4);//刷新终端当前剩余电量(费)(总加组号)
							MC(0x0,ADDR_AFN0CF23_DP+(2*(Pn-1)),2);//清当前剩余电量单位G=1时(MWH)小数点后数据,格式BCD=0.000
						}
						i=MRR(ADDR_AFN0CF23+(4*(Pn-1)),4);//本次购电后剩余电能量（费）
				#if ERC19==1
						ERC19_Event(Pn,RXaddr,x,i);//购电参数设置记录
				#endif
				#if (((USER/100)==5) || ((USER/100)==13))//上海、黑龙江
						//显示用购电前后剩余电能量（费）
						a64=i;
						a64<<=32;
						a64|=x;
						MWR(a64,ADDR_AFN04F47_Ext+(8*(Pn-1)),8);
				#endif
					}
					i=TerminalParamterLib[Fn-1].Len;
					break;
				case 65://定时上报1类数据任务设置
				case 66://定时上报2类数据任务设置		
					if(f==0x0)
					{
						if(p8[8]>MaxTimerTaskDI)//最大定时任务数据标识个数
						{
							goto err;
						}
						x=(p8[8]*4)+9;
						if((DIlen-4)<x)
						{
							goto err;
						}
						DIlen-=x;
						RXaddr+=x;
					}
					else
					{
						x=(p8[8]*4)+9;
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(9+(4*MaxTimerTaskDI))),x);
						DIlen-=x;
						RXaddr+=x;
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;
				case 84://终端通信端口配置参数
					if(f==0x0)
					{
						i=p8[0];
						if((DIlen-4)<((i*15)+1))
						{
							goto err;
						}
						DIlen-=((i*15)+1);
						RXaddr+=((i*15)+1);
						p8+=1;
						for(;i!=0x0;i--)
						{
							if(p8[0]==0x0)
							{//配置序号=0
								goto err;
							}
							if(p8[0]>AFN04F84MaxPn)
							{
								goto err;
							}
							p8+=15;
						}
					}
					else
					{
						RXaddr+=1;
						DIlen-=1;
						for(i=0;i<p8[0];i++)
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((p8[1+(i*15)]-1)*15),15);
							RXaddr+=15;
							DIlen-=15;
							//通信速率同步
							p8d=(u8 *)(RXaddr);
							x=p8d[1]&0x1f;
							y=p8d[1]>>5;//D7~D5 编码表示通信端口的通信波特率，单位为bps，1~7 依次表示600、1200、2400、4800、7200、9600、19200；0：表示无需设置或使用默认值
							if(x==2)
							{//RS485-1
								if(y==0x0)
								{
									y=3;//默认值3=2400
								}
//								MWR(y,UART_PORT_SET+4+1,1);//写寄存器数据到存贮器,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET+4);
//								if(p8d[0]!=0x0)
//								{//归终端用
//									p8d=(u8 *)(ADDR_AFN04F37);//终端级联通信参数
//									if(p8d[0]==2)
//									{//级联通信端口
//										if((p8d[6]&0x80)!=0x0)
//										{//被级联
//											uart2bps((y<<5)|0x0b);//uart2波特率,入口通信控制字同负荷管理2005规约(扩展1位D8速率表示)
//										}
//									}
//								}
							}
							if(x==3)
							{//RS485-2
								if(y==0x0)
								{
									y=3;//默认值3=2400
								}
//								MWR(y,UART_PORT_SET+8+1,1);//写寄存器数据到存贮器,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET+8);
//								if(p8d[0]!=0x0)
//								{//归终端用
//									p8d=(u8 *)(ADDR_AFN04F37);//终端级联通信参数
//									if(p8d[0]==3)
//									{//级联通信端口
//										if((p8d[6]&0x80)!=0x0)
//										{//被级联
//											uart3bps((y<<5)|0x0b);//uart3波特率,入口通信控制字同负荷管理2005规约(扩展1位D8速率表示)
//										}
//									}
//								}
							}
							if(x==4)
							{//RS485-3
								if(y==0x0)
								{
									y=3;//默认值3=2400
								}
//								MWR(y,UART_PORT_SET+12+1,1);//写寄存器数据到存贮器,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET+12);
//								if(p8d[0]!=0x0)
//								{//归终端用
//									p8d=(u8 *)(ADDR_AFN04F37);//终端级联通信参数
//									if(p8d[0]==4)
//									{//级联通信端口
//										if((p8d[6]&0x80)!=0x0)
//										{//被级联
//											uart4bps((y<<5)|0x0b);//uart4波特率,入口通信控制字同负荷管理2005规约(扩展1位D8速率表示)
//										}
//									}
//								}
							}
							if(x==5)
							{//rs232
								if(y==0x0)
								{
									y=10;//默认值10=115200
								}
//								MWR(y,UART_PORT_SET+1,1);//写寄存器数据到存贮器,Byte<=8
//								p8d=(u8 *)(UART_PORT_SET);
//								if(p8d[0]!=0x0)
//								{//归终端用
//									uart0bps((y<<5)|0x0b);//uart0波特率,入口通信控制字同负荷管理2005规约(扩展1位D8速率表示)
//								}
							}
							
						}
						Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					}
					continue;
				case 97://F97停电数据采集配置参数
					i=p8[3]+(p8[4]<<8);
					if(i>MaxRS485AddCarrierPn)
					{//最大测量点数检查
						goto err;
					}
					if((p8[0]&0x02)==0)
					{//D1：置"1"随机选择测量点，置"0"只采集设置的测量点；置"1"时不填写测量点号1至测量点号n，按电能表个数n随机选择n个测量点
						for(x=0;x<i;x++)
						{//测量点号检查
							y=p8[5+(2*x)]+(p8[6+(2*x)]<<8);
							if(y>MaxRS485AddCarrierPn)
							{
								goto err;
							}
						}
					}
					i=5+(i*2);
					break;
				case 104://曲线冻结密度参数(测量点号)
					if((DIlen-4)<2)
					{
						goto err;
					}
					i=p8[0];//曲线Fn代号
					x=p8[1];
					if(f==0x0)
					{
						if(i!=0)
						{
							i=Get_CurveSetFnListNo(i);//得到曲线冻结密度参数Fn列表号;返回:列表号0-n,0xffffffff表示无
							if(i==0xffffffff)
							{
								goto err;
							}
						}
						RXaddr+=2;
						DIlen-=2;
						continue;
					}
					if(i==0)
					{//当曲线Fn 代号为0 时，表示该设置对所有曲线生效
						MC(x,ADDR_AFN04F104,LEN_AFN04F104_PerPn*AFN04F104MaxPn);
					}
					else
					{
						i=Get_CurveSetFnListNo(i);//得到曲线冻结密度参数Fn列表号;返回:列表号0-n,0xffffffff表示无
						if(i!=0xffffffff)
						{
							MWR(x,ADDR_AFN04F104+((Pn-1)*LEN_AFN04F104_PerPn)+i,1);
						}
					}
					Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
					continue;
				case 106://电能表数据分级参数(数据分级类号)
					x=p8[0];//本次设置的电能表数据个数n
					if((DIlen-4)<(1+(x*5)))
					{
						goto err;
					}
					if(f==0x0)
					{
						RXaddr+=(1+(x*5));
						DIlen-=(1+(x*5));
						continue;
					}
					MR(ADDR_DATABUFF,ADDR_AFN04F106+((Pn-1)*5*NUM_AFN04F106_PerC),5*NUM_AFN04F106_PerC);
					if(x>NUM_AFN04F106_PerC)
					{
						x=NUM_AFN04F106_PerC;//每分级类存储的电能表数据标识个数
					}
					for(i=0;i<x;i++)
					{
						y=MRR(RXaddr+1+(i*5),4);
						for(z=0;z<NUM_AFN04F106_PerC;z++)
						{
							f=MRR(ADDR_DATABUFF+(z*5),4);
							if(y==f)
							{//找到原设置位置
								p8=(u8*)(RXaddr+1+(i*5)+4);
								p8d=(u8*)(ADDR_DATABUFF+(z*5)+4);
								p8d[0]=p8[0];
								break;
							}
						}
						if(z==NUM_AFN04F106_PerC)
						{//没找到原设置位置,找1个空位置存储
							for(z=0;z<NUM_AFN04F106_PerC;z++)
							{
								p8d=(u8*)(ADDR_DATABUFF+4+(z*5));
								if(p8d[0]==0)
								{//找到空位置存储
									MW(RXaddr+1+(i*5),ADDR_DATABUFF+(z*5),5);
									break;
								}
							}
						}
					}
					MW(ADDR_DATABUFF,ADDR_AFN04F106+((Pn-1)*5*NUM_AFN04F106_PerC),5*NUM_AFN04F106_PerC);
					f=1;
					RXaddr+=(1+(x*5));
					DIlen-=(1+(x*5));
					continue;
				case 107://分级事件采集周期参数
					//为了电科院台体测试分级事件上报顺序需求,当参数设置时表示测试开始,清抄读定时
					//原时间用有效时间01年01月01日00时00分
					for(i=0;i<4;i++)
					{
						MWR(0x0101010000,((u32)&UARTRMEvent_4851->YMDHMS)+(6*i)+1,5);//485-1
					}
					for(i=0;i<4;i++)
					{
						MWR(0x0101010000,((u32)&UARTRMEvent_4852->YMDHMS)+(6*i)+1,5);//485-2
					}
					for(i=0;i<4;i++)
					{
						MWR(0x0101010000,((u32)&UARTRMEvent_4854->YMDHMS)+(6*i)+1,5);//485-4(载波)
					}
					i=TerminalParamterLib[Fn-1].Len;
					break;
				case 150://测量点状态(只读参数)
					goto err;
			#if (USER/100)==12//四川用户
/*
参数设置AFN04 扩展F151 485口功能配置。数据1字节，D0~D7y依次表示485I、485II、…
置1，表示485本地通信口。置0表示为485抄表口。
*/
				case 151:
				AFN04F151:
					if(Comm_Ram->Factory!=0x55)
					{//0x55=工厂模式
						MWR((~p8[0])&0x1,ADDR_UART_PORT_SET+LEN_UART_PORT_SET,1);
					}
					MWR(((~p8[0])>>1)&0x1,ADDR_UART_PORT_SET+2*LEN_UART_PORT_SET,1);
					RXaddr+=1;
					DIlen-=1;
					continue;
			#endif
			#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
				case 297://辽宁锦州、浙江终端对时参数
			AFN04F297:
					i=10;
					break;	
				case 298://辽宁锦州、浙江电表对时参数
			AFN04F298:
					i=8;
					break;	
				case 299://辽宁锦州、浙江终端请求主站时钟
			AFN04F299:
					if(UARTCtrl->HostACK==7)//主动发出需主站确认帧标记:0=无,1=登录帧,2=退出登录帧,3=心跳帧,4=重要事件,5=定时任务
					{
						UARTCtrl->HostACK=0;//需主站确认帧标记:0=无
						UARTCtrl->HostCount=0;//主动上报重发计数(心跳发出计数另)
						if(Terminal_Ram->PowerTask==0)//停电事件任务
						{
							Terminal_Ram->RMDelay_S_Timer=0;
							Terminal_Ram->PowerTask=1;//停电事件任务

							p8rxbuff[21+4]&=0x1f;//去加在月字节上的星期
							p8rxbuff[30+4]&=0x1f;//去加在月字节上的星期
							i=Check_YMDHMS(MRR((u32)p8rxbuff+21,6));//年月日时分秒合法检查,返回0合法,1非法
							if(i==0)
							{//时钟合法
								i=Check_YMDHMS(MRR((u32)p8rxbuff+30,6));//年月日时分秒合法检查,返回0合法,1非法
								if(i==0)
								{//时钟合法
									i=TimeDifference((u32)p8rxbuff+21,(u32)p8rxbuff+30);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
						 			if((i>>31)!=0x0)
									{//负
										i=~i;
										i++;
										if(i>=300)
										{
											d64a=MRR(ADDR_ERC14SOUR+22,6);
											d64a=YMDHMS_SubS(d64a,i);//年月日时分秒减S秒,返回减S秒后的年月日时分秒
											MWR(d64a,ADDR_ERC14SOUR+22,6);
		
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_SubS(d64a,i);//年月日时分秒减S秒,返回减S秒后的年月日时分秒
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									else
									{
										if(i>=300)
										{
											d64a=MRR(ADDR_ERC14SOUR+22,6);
											d64a=YMDHMS_AddS(d64a,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
											MWR(d64a,ADDR_ERC14SOUR+22,6);
		
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_AddS(d64a,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									if(i>=300)
									{
//										WriteLPCRTC(ADDR_RTCBUFF);//写内部RTC
//										SyncLPCRTC();//同步内部RTC(读内部写外部)
										WriteRTC(ADDR_RTCBUFF);//写外部硬实时钟
										Comm_Ram->MS_Task=0;//下步即读RTC
										Comm_Ram->ReadRTCTimer=0;//25 读ERTC每10MS减1定时器
										Terminal_Ram->I2SUBTask1=0;//下步即算实时数据RTC
										//对时事件记录
										MWR(0,ADDR_DATABUFF+2,2);//测量点号
										MR(ADDR_DATABUFF+4,ADDR_AFN0CF2,6);//对时前时间
										MR(ADDR_DATABUFF+10,(u32)p8rxbuff+30,6);//对时后时间
									WriteTerminalTime:
										ERC41_Event(ADDR_DATABUFF);//对时事件记录;入口:ADDR_BUFF的第2字节开始分别为2BYTE测量点号,6BYTE对时前时间,6BYTE对时后时间
										
										//主站下发对时命令给终端时，终端应将跳过的时间段的电压统计为合格；对时跳过的时间段应计入监测时间
										MR(ADDR_DATABUFF,ADDR_DATABUFF+4,12);//对时前时间,对时后时间
										p8d=(u8 *)(ADDR_DATABUFF);
										p8d[4]&=0x1f;//对时前时间去月上的星期
										p8d[6+4]&=0x1f;//对时后时间去月上的星期
										x=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
										if((x>>31)==0)
										{//正值
											i=MRR(ADDR_DATABUFF+3,3);
											i=YMD_Add1D(i);//对时前时间年月日加1日,返回加1后的年月日
											MC(0,ADDR_DATABUFF+12,3);
											MWR(i,ADDR_DATABUFF+15,3);
											i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+12);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
											if(x>i)
											{
												x=i;//用当日到24点差
											}
											x/=60;//秒变为分
											for(i=0;i<AFN0DF27MaxPn;i++)
											{
											//A相电压合格日累计时间
												y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+8,2);
												y+=x;
												if(y>0xffff)
												{
													y=0xffff;
												}
												MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+8,2);
											//B相电压合格日累计时间
												y=MRR(ADDR_AFN04F25+(i*11)+10,1);//取测量点基本参数的电源接线方式
												if(y!=1)
												{//不是三相三线
													y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+18,2);
													y+=x;
													if(y>0xffff)
													{
														y=0xffff;
													}
													MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+18,2);
												}
											//C相电压合格日累计时间
												y=MRR(ADDR_AFN0DF27_SOUR+(i*(66+27))+28,2);
												y+=x;
												if(y>0xffff)
												{
													y=0xffff;
												}
												MWR(y,ADDR_AFN0DF27_SOUR+(i*(66+27))+28,2);
											}
											x=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+6);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
											i=MRR(ADDR_DATABUFF+4,2);
											i=YM_Add1M(i);//年月加1月,返回加1后的年月
											MC(0,ADDR_DATABUFF+12,3);
											MWR(1,ADDR_DATABUFF+15,1);
											MWR(i,ADDR_DATABUFF+16,2);
											i=TimeDifference(ADDR_DATABUFF,ADDR_DATABUFF+12);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
											if(x>i)
											{
												x=i;//用当月未(30或31日)24点差
											}
											x/=60;//秒变为分
											for(i=0;i<AFN0DF27MaxPn;i++)
											{
											//A相电压合格月累计时间
												y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+8,2);
												y+=x;
												if(y>0xffff)
												{
													y=0xffff;
												}
												MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+8,2);
											//B相电压合格月累计时间
												y=MRR(ADDR_AFN04F25+(i*11)+10,1);//取测量点基本参数的电源接线方式
												if(y!=1)
												{//不是三相三线
													y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+18,2);
													y+=x;
													if(y>0xffff)
													{
														y=0xffff;
													}
													MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+18,2);
												}
											//C相电压合格月累计时间
												y=MRR(ADDR_AFN0DF35_SOUR+(i*(66+27))+28,2);
												y+=x;
												if(y>0xffff)
												{
													y=0xffff;
												}
												MWR(y,ADDR_AFN0DF35_SOUR+(i*(66+27))+28,2);
											}
											for(i=0;i<AFN0DF27MaxPn;i++)
											{
												AFN0DF27F35_Computer();//终端统计.电压统计数据
											}
										}
									}
								}
							}
						}
						else
						{
							p8rxbuff[21+4]&=0x1f;//去加在月字节上的星期
							p8rxbuff[30+4]&=0x1f;//去加在月字节上的星期
							p8rxbuff[39+4]&=0x1f;//去加在月字节上的星期
							i=Check_YMDHMS(MRR((u32)p8rxbuff+21,6));//年月日时分秒合法检查,返回0合法,1非法
							x=Check_YMDHMS(MRR((u32)p8rxbuff+30,6));//年月日时分秒合法检查,返回0合法,1非法
							y=Check_YMDHMS(MRR((u32)p8rxbuff+39,6));//年月日时分秒合法检查,返回0合法,1非法
							if((i==0)&&(x==0)&&(y==0))
							{
								i=MRR(ADDR_AFN04F299_Ext+28,1);
								i&=0x7f;
								i++;
								MWR(i|0x80,ADDR_AFN04F299_Ext+28,1);
								//V=T4-T3
								MR(ADDR_DATABUFF,ADDR_TYMDHMS,6);
								p8=(u8 *)ADDR_DATABUFF;
								p8[4]&=0x1f;//去加在月字节上的星期
								y=TimeDifference((u32)p8rxbuff+39,(u32)ADDR_DATABUFF);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
								//U=T2-T1
								x=TimeDifference((u32)p8rxbuff+21,(u32)p8rxbuff+30);//时钟差值计算,入口R0=时钟1(秒分时日月年)地址;R1=时钟2(秒分时日月年)地址;出口R0=HEX补码时钟快或慢的差值(秒),时钟1时钟2非法R0=0,时钟2>=时钟1差值为正,时钟2<时钟1差值为负
								//K=(U-V)/2
					 			if(((y>>31)!=0x0)&&((x>>31)!=0x0))
								{//负、负
									y=~y;
									y++;
									x=~x;
									x++;
									if(x>y)
									{
										z=(x-y);
										z/=2;
										z=~z;
										z++;
									}
									else
									{
										z=(y-x);
										z/=2;
									}
								}
								else
								{
						 			if(((y>>31)!=0x0)&&((x>>31)==0x0))
									{//负、正
										y=~y;
										y++;
										z=(x+y);
										z/=2;
									}
									else
									{
							 			if(((y>>31)==0x0)&&((x>>31)!=0x0))
										{//正、负
											x=~x;
											x++;
											z=(x+y);
											z/=2;
											z=~z;
											z++;
										}
										else
										{
								 			if(((y>>31)==0x0)&&((x>>31)==0x0))
											{//正、正
												if(x>y)
												{
													z=(x-y);
													z/=2;
												}
												else
												{
													z=(y-x);
													z/=2;
													z=~z;
													z++;
												}
											}
										}
									}
								}
								MWR(z,ADDR_AFN04F299_Ext+28+1+(i-1)*4,4);
								if(i==24)
								{
									MR(ADDR_AFN04F299_Ext+28+1+4*25+4+1,ADDR_TYMDHMS,6);
									MR(ADDR_DATABUFF,ADDR_AFN04F299_Ext+28+1,96);
									p32=(u32 *)ADDR_DATABUFF;
	
									//区分正负
									x=0;
									z=0;
									for(y=0;y<24;y++)
									{
										
										if((p32[y]>>31)!=0)
										{//负
											p32[47-z]=p32[y];
											p32[47-z]=~p32[47-z];
											p32[47-z]++;
											z++;
										}
										else
										{
											p32[x+24]=p32[y];
											x++;
										}
									}
									//正数个数
									p32[48]=x;
									//正数从大到小排列
									if(p32[48]!=0)
									{
										for(x=0;x<p32[48]-1;x++) 
										{ 
											for (i=0;i<p32[48]-1-x;i++)
											{
												if (p32[24+i]<p32[24+i+1]) 
												{ 
													y=p32[24+i]; 
													p32[24+i]=p32[24+i+1]; 
													p32[24+i+1]=y;
												} 
											} 
										} 
									}
									//负数从大到小排列
									if(p32[48]!=24)
									{
										for(x=0;x<(23-p32[48]);x++) 
										{ 
											for (i=0;i<(23-p32[48])-x;i++)
											{
												if (p32[47-i]<p32[47-(i+1)]) 
												{ 
													y=p32[47-i]; 
													p32[47-i]=p32[47-(i+1)]; 
													p32[47-(i+1)]=y;
												} 
											} 
										}
									}
									//选取20组K值最小的数据，求取延时平均值
									if(p32[48]<=4)
									{
										i=0;
										for(x=0;x<20;x++)
										{
											i+=p32[24+4+x];
										}
										i/=20;
										i=~i;
										i++;
									}
									else
									{
										i=0;
										for(x=0;x<(p32[48]-4);x++)
										{
											i+=p32[24+4+x];
										}
										y=0;
										for(x=0;x<(24-p32[48]);x++)
										{
											y+=p32[47-x];
										}
										if(i>y)
										{
											i-=y;
											i/=20;
										}
										else
										{
											i=(y-i);
											i/=20;
											i=~i;
											i++;
										}
									}
									//对时前误差
									MWR(i,ADDR_AFN04F299_Ext+28+1+4*25,4);
									if((i>>31)!=0)
									{//负
										i=~i;
										i++;
										if(i>MRR(ADDR_AFN04F297_Ext+8,1))
										{
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_SubS(d64a,i);//年月日时分秒减S秒,返回减S秒后的年月日时分秒
											MWR(d64a,ADDR_TYMDHMS,6);
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									else
									{
										if(i>MRR(ADDR_AFN04F297_Ext+8,1))
										{
											d64a=MRR(ADDR_TYMDHMS,6);
											d64a=YMDHMS_AddS(d64a,i);//年月日时分秒加S秒,返回加S秒后的年月日时分秒
											MWR(d64a,ADDR_TYMDHMS,6);
											MWR(d64a,ADDR_RTCBUFF,3);
											MWR(d64a>>24,ADDR_RTCBUFF+4,3);
										}
									}
									if(i>MRR(ADDR_AFN04F297_Ext+8,1))
									{
										WriteRTC(ADDR_RTCBUFF);//写外部硬实时钟
										Comm_Ram->MS_Task=0;//下步即读RTC
										Comm_Ram->ReadRTCTimer=0;//25 读ERTC每10MS减1定时器
										Terminal_Ram->I2SUBTask1=0;//下步即算实时数据RTC
										//对时事件记录
										MWR(0,ADDR_DATABUFF+2,2);//测量点号
										MR(ADDR_DATABUFF+4,ADDR_AFN0CF2,6);//对时前时间
										MR(ADDR_DATABUFF+10,ADDR_TYMDHMS,6);//对时后时间
										goto WriteTerminalTime;
									}
									else
									{
										MWR(0,ADDR_AFN04F299_Ext+28,1);
										MWR(0x80,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										//F249
										MR(ADDR_AFN0CF249_Ext,ADDR_TYMDHMS+1,5);
										MWR(i,ADDR_AFN0CF249_Ext+5,0);
										MWR(0,ADDR_AFN0CF249_Ext+7,0);
										MWR(2,ADDR_AFN0CF249_Ext+9,1);
									}
								}
								else
								{
									if(i==25)
									{
										//F249
										MR(ADDR_AFN0CF249_Ext,ADDR_TYMDHMS+1,5);
										//ERC51
										p8=(u8 *)ADDR_DATABUFF;
										i=MRR(ADDR_AFN04F299_Ext+28+1+25*4,4);//校时前误差
										if((i>>31)!=0)
										{
											i=~i;
											i++;
										}
										MWR(i,ADDR_AFN0CF249_Ext+5,2);
										MWR(i,ADDR_DATABUFF+9,2);
										if(MRR(ADDR_AFN0CF250_Ext1+10*(Pn-1)+5+2+2,1)==0xff)//电能表对时后的结果
										{
											i&=0xffff;
											y=MRR(ADDR_AFN0CF250_Ext1+10*x+5,2);//电能表对时前的误差
											y-=i;
											MWR(y,ADDR_AFN0CF250_Ext1+10*x+5,2);//电能表对时前的误差
										}
										i=MRR(ADDR_AFN04F299_Ext+28+1+24*4,4);//校时后误差
										if((i>>31)!=0)
										{
											i=~i;
											i++;
										}
										MWR(i,ADDR_AFN0CF249_Ext+7,2);
										MWR(i,ADDR_DATABUFF+11,2);
										if(i>MRR(ADDR_AFN04F297_Ext+8,1))
										{//校时失败
											p8[13]=0;
											MWR(0,ADDR_AFN0CF249_Ext+9,1);
										}
										else
										{//校时成功
											p8[13]=1;
											MWR(1,ADDR_AFN0CF249_Ext+9,1);
										}
										ERC51_Event(0+(1<<15),ADDR_DATABUFF);//校时记录事件;入口:Pn测量点号(从0开始)
										//记录第几次对时
										i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										i&=0x7f;
										i++;
										i|=(p8[13]<<7);
										MWR(i,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										if(DataComp(ADDR_AFN04F297_Ext+4,ADDR_TYMDHMS+1,2)==2)//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
										{
											i=MRR(ADDR_AFN04F299_Ext+28+1+4*25+4,1);
											MWR(i&0x80,ADDR_AFN04F299_Ext+28+1+4*25+4,1);
										}
									}
								}
							}
							}
					}
					i=28;
					break;	
			#endif
			#if (USER/100)==11//吉林系
				case 6://
					if(Comm_Ram->Factory==0x55)//0x55=工厂状态
					{
						i=16;
					}
					else
					{
						i=TerminalParamterLib[Fn-1].Len;
					}
					break;
			#endif
				default:
					i=TerminalParamterLib[Fn-1].Len;
					break;
			}
			if(i==0x0)
			{
				goto err;
			}
			if(Fn!=97)
			{//F97停电数据采集配置参数时不判帧长度
				if((DIlen-4)<i)
				{
					goto err;
				}
			}
			if(f==0x1)
			{
			#if (USER/100)==7//浙江用户
				if((PORTn==GPRSPORT)||(PORTn==ENETPORT))
				{
					if((UARTCtrl->LinkTask>=100)&&(UARTCtrl->LinkTask<=101))//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
					{//连接
						if(Fn==3)
						{
							if(DataComp(RXaddr,TerminalParamterLib[Fn-1].Addr,i))//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							{
								UARTCtrl->LinkTask=102;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
								if(PORTn==GPRSPORT)
								{
									Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS心跳秒定时器
								}
								if(PORTn==ENETPORT)
								{
									Terminal_Ram->ENETHeartBeat_S_Timer=0;//以太网心跳秒定时器
								}
							}
						}
						if(Fn==8)
						{
							if(DataComp(RXaddr,TerminalParamterLib[Fn-1].Addr,1))//数据比较,返回0=相等;当Byte>8时返回1=不相等;当Byte<=8时1=Data1>Data2,2=Data1<Data2
							{
								UARTCtrl->LinkTask=102;//链接任务0-99初始过程,100=建立连接,101=登录,102=发退出登录,103=退出登录,104=关机
								if(PORTn==GPRSPORT)
								{
									Terminal_Ram->GPRSHeartBeat_S_Timer=0;//GPRS心跳秒定时器
								}
								if(PORTn==ENETPORT)
								{
									Terminal_Ram->ENETHeartBeat_S_Timer=0;//以太网心跳秒定时器
								}
							}
						}
					}
				}
			#endif
			#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
				if(Pn==0x0)
				{
					if((Fn!=297)||(Fn!=298))
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
					if(Fn==297)
					{
						MW(RXaddr,ADDR_AFN04F297_Ext,i);
					}
					if(Fn==298)
					{
						MW(RXaddr,ADDR_AFN04F298_Ext,i);
					}
				}
			#else
				#if (USER/100)==11//吉林
				if(Pn==0x0)
				{
					if(Fn==89)
					{
						if(Comm_Ram->Factory==0x55)//0x55=工厂状态
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
						else
						{// 非工厂模式下设置F89返回否认
							goto err;
						}
					}
					else
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
				}
				#else
					#if (USER/100)==12//四川
					if(Pn==0x0)
					{
						if(Fn==245)
						{
							x=MRR(RXaddr,4);
							y=BitNum(x);//计算置位为1的个数(0-32)
							if(y<=5)
							{//推荐允许最大采集日期为5天，即前面5个置"1"有效
								MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
							}
							else
							{
								y=0;
								for(i=0;i<32;i++)
								{
									if((x&(1<<i))!=0x0)
									{
										y+=1;
									}
									if(y==5)
									{
										break;
									}
								}
								for(z=0;z<(31-i);z++)
								{
									x&=(~(1<<(31-z)));
								}
								MWR(x,TerminalParamterLib[Fn-1].Addr,4);
							}
						}
						else
						{
							MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
					}
					#else
					if(Pn==0x0)
					{
						MW(RXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
					#endif					
				#endif	
			#endif	
				else
				{
					MW(RXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*i),i);
				}
				Terminal_AFN0CF3_Computer(Fn);//终端类1数据F3,终端参数状态
				if(Fn==97)
				{//F97停电数据采集配置参数
					Terminal_Ram->F246PnListUpDate=0;//要抄读的掉电记录数据Pn列表已更新标志,0=要更新,!=0已更新
				}
			}
			RXaddr+=i;
			if(DIlen>=i)
			{
				DIlen-=i;
			}
			else
			{
				DIlen=0;
			}
		}
	}
	if(f==0x0)
	{
		f=1;
		goto loop;
	}
	else
	{
		//全部确认
	#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
		if(Fn!=299)
		{
			ERC3_Event((u32)p8rxbuff);//参数变更记录,入口接收缓冲首址
		}
	#else
		ERC3_Event((u32)p8rxbuff);//参数变更记录,入口接收缓冲首址
	#endif
		Uart_376_ACK(PORTn,1);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
		DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
		Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
		Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
		BUZZ_Generate(2);//蜂鸣产生,入口蜂鸣号
	}
}

//参数查询
void Uart_AFN0A(u32 PORTn)//参数查询
{
	u32 i;
	u32 x;
	u32 y;
	u32 DIlen;
	u32 Fn;
	u32 Pn;
	u32 DIaddr;
	u32 RXaddr;
	u32 TXDIaddr;
	u32 TXaddr;
	u32 TXlen;
	u32 f;
  u32 LEN_UARTnTx;
	u8 * p8s;
	u8 * p8d;	
	u8 * p8rxbuff;
	u8 * p8txbuff;
#if (USER/100)==5//用户标识:上海系
	u16 *p16;
	u32 crc;
#endif
#if ((USER/100)==13)&&((Project/100)==3)//黑龙江专变
	u32 z;
#endif
#if (USER/100)==6//用户标识:湖南
	u32 k;
#endif


  LEN_UARTnTx=Get_LEN_UARTnTx(PORTn);
  p8rxbuff=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);

	p8txbuff[6]=(p8txbuff[6]&0xf0)+8;//响应功能码 8
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//Tp时间标签有效判定
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		if(DIlen<(12+6))
		{
err:
			//全部否认
			Uart_376_ACK(PORTn,2);//全部确认／否认帧;ACK=1确认=2否认;出口填写数据长度,控制码,AFN,帧序列域,数据单元标识
			p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
			DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
			Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
			Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
			return;
		}
		i=((u32)p8rxbuff)+6+DIlen-6;
		i=Uart_376_Tp(i);//时间标签有效判定,入口时间标签首址,出口0有效,1无效
		if(i!=0x0)
		{
			return;//舍弃该报文
		}
	}
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
//用户区数据标识长度
	if((p8rxbuff[13]&0x80)!=0x0)
	{//有Tp
		DIlen-=(8+6);
	}
	else
	{
		DIlen-=(8);
	}
	if(DIlen<4)
	{
		goto err;
	}
//数据(启动站功能码11（请求2级数据）用于应用层请求数据的链路传输)
	DIaddr=Get_ADDR_UARTnRx(PORTn)+14;
	RXaddr=Get_ADDR_UARTnRx(PORTn)+14+4;
	TXDIaddr=Get_ADDR_UARTnTx(PORTn)+14;
	TXaddr=Get_ADDR_UARTnTx(PORTn)+14+4;
	TXlen=0;
	f=0;//有无有效数据填入标志
	CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
	p8d=(u8 *)(TXDIaddr);
	p8d[0]=0;
	p8d[1]=0;
	p8d[2]=0;
	p8d[3]=0;
	while(DIlen>=4)
	{
		i=DItoFnPn(PORTn);//数据标识分解为FnPn
		if(i==0x0)//数据标识分解为FnPn
		{
DIerr:
			DIaddr=RXaddr;
			RXaddr+=4;
			DIlen-=4;
			if(f!=0x0)
			{
				TXDIaddr=TXaddr;
				TXaddr+=4;
				TXlen+=4;
				if((TXlen+14+8+2)>Get_LEN_UARTnRx(PORTn))
				{
					goto err;
				}
				f=0;
			}
			CopyDI(PORTn,DIaddr);//COPY数据标识到控制缓冲准备分解
			p8d=(u8 *)(TXDIaddr);
			p8d[0]=0;
			p8d[1]=0;
			p8d[2]=0;
			p8d[3]=0;
		}
		else
		{
			Fn=(i>>16);
			Pn=i&0xffff;
		#if (USER/100)==12//四川用户
			if(Fn==151)
			{
				goto AFN0AF151;
			}
		#endif
		#if (USER/100)==6//湖南
			if(Fn==200)
			{
				goto AFN0AF200;
			}
		#endif
		#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
			p8s=(u8 *)(ADDR_DATABUFF);
			if((Fn==296)&&(Pn==0))
			{
				if(DIlen<6)
				{
					goto err;
				}
				else
				{
					goto AFN0AF296;	
				}
			}
			if((Fn==297)&&(Pn==0))
			{
				goto AFN0AF297;	
			}
			if((Fn==298)&&(Pn==0))
			{
				goto AFN0AF298;	
			}
		#endif
			if(Fn>Get_AFN04ParamterLib_MaxFn())
			{
				goto DIerr;
			}
			if(Fn==0x0)
			{
				goto DIerr;
			}
			if(Pn>TerminalParamterLib[Fn-1].PnMax)
			{
				goto err;
			}
			if(TerminalParamterLib[Fn-1].PnType!=0x0)
			{
				if(Pn==0x0)
				{
					goto err;
				}
			}
			if((TerminalParamterLib[Fn-1].project&(1<<(Project/100)))==0x0)
			{//本方案无此Fn功能
				goto err;
			}
			p8s=(u8 *)(ADDR_DATABUFF);
			switch(Fn)//Fn
			{
				case 0:
					goto DIerr;
				case 2://终端上行通信口无线中继转发查询
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0]&0x7f;
					if(i>16)
					{
						i=16;
					}
					i*=2;
					i+=1;
					break;
				case 7://终端IP地址和端口查询
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,64);
					i=20+1+p8s[20];
					i+=(1+p8s[i]+2);
					break;
				case 10://终端电能表／交流采样装置配置参数查询
					if((TXlen+4+14+8+2+2)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0]+(p8s[1]<<8);
					if((DIlen-4)<(2+(i*2)))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					p8txbuff[1]=0;
					TXlen+=2;
					TXaddr+=2;
					for(i=0;i<(p8s[0]+(p8s[1]<<8));i++)
					{
						if((TXlen+4+14+8+2+LEN_AFN04F10_Pn)>LEN_UARTnTx)
						{
							goto err;
						}
						x=p8s[2+(i*2)]+(p8s[2+(i*2)+1]<<8);
						if(x==0x0)
						{
							continue;	
						}
						if(x>AFN04F10MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((x-1)*LEN_AFN04F10_Pn),LEN_AFN04F10_Pn);
						p8d=(u8 *)(TXaddr);
						if(x==(p8d[0]+(p8d[1]<<8)))
						{//序号相同
						#if ((USER/100)==5)//上海用户
							if(p8d[5]==21)
							{//上海规约
								//第3字节：制造厂代码，1字节ASCII码，如P，则该抄表帧里为50H，主站下发给终端为80H(BCD码)
								p8d[9]=hex_bcd(p8d[9]);	
							}
						#endif
							TXlen+=LEN_AFN04F10_Pn;
							TXaddr+=LEN_AFN04F10_Pn;
							f=p8txbuff[0]+(p8txbuff[1]<<8);//本次查询数量+1
							f+=1;
							p8txbuff[0]=f;
							p8txbuff[1]=f>>8;
						}
					}
					RXaddr+=2+(i*2);
					DIlen-=2+(i*2);
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 11://终端脉冲配置参数查询
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+5)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F11MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*5),5);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//序号相同
							TXlen+=5;
							TXaddr+=5;
							p8txbuff[0]+=1;//本次查询数量+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 13://终端电压/电流模拟量配置参数查询
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+3)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F13MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*3),3);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//序号相同
							TXlen+=3;
							TXaddr+=3;
							p8txbuff[0]+=1;//本次查询数量+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 14://终端总加组配置参数查询
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F14MaxPn)
						{
							continue;
						}
						MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*66)+1,1);
						p8d=(u8 *)(ADDR_DATABUFF);
						x=p8d[0]+2;
						if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
						{
							goto err;
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*66),x);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//序号相同
							TXlen+=x;
							TXaddr+=x;
							p8txbuff[0]+=1;//本次查询数量+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 15://有功总电能量差动越限事件参数查询
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+9)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F15MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*9),9);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//序号相同
							TXlen+=9;
							TXaddr+=9;
							p8txbuff[0]+=1;//本次查询数量+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 22://终端电能量费率查询
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0];
					if(i>48)
					{
						i=48;
					}
					i*=4;
					i+=1;
					break;
				case 31://载波从节点附属节点地址查询
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0];
					if(i>20)
					{
						i=20;
					}
					i*=6;
					i+=1;
					break;
				case 33://终端抄表运行参数查询
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>31)
						{
							continue;
						}
						MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*(14+(4*24)))+13,1);
						p8d=(u8 *)(ADDR_DATABUFF);
						x=(p8d[0]*4)+14;
						if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
						{
							goto err;
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*(14+(4*24))),x);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//序号相同
							TXlen+=x;
							TXaddr+=x;
							p8txbuff[0]+=1;//本次查询数量+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;	
				case 34://集中器下行通信模块的参数查询
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+6)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>31)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*6),6);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//序号相同
							TXlen+=6;
							TXaddr+=6;
							p8txbuff[0]+=1;//本次查询数量+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;	
				case 35://台区集中抄表重点户查询
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr,1);
					i=p8s[0];
				#if (MaxKeyUser<=20)
					if(i>MaxKeyUser)
					{
						i=MaxKeyUser;
					}
				#else
					if(i>20)
					{
						i=20;
					}
				#endif
					i*=2;
					i+=1;
					break;	
				case 37://终端级联通信参数查询
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+6,1);
					i=p8s[0]&0x0f;
					if(i>3)
					{
						i=3;
					}
					i*=4;
					i+=7;
					break;	
				case 38://1类数据配置查询
				case 39://2类数据配置查询
					if((TXlen+4+14+8+2+2)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[1];
					if((DIlen-4)<(2+i))
					{
						goto err;
					}
					if(p8s[0]>15)
					{//本次查询的用户大类号>15
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=p8s[0];//本次查询的用户大类号
					p8txbuff[1]=0;//本次查询数量=0
					TXlen+=2;
					TXaddr+=2;
					for(i=0;i<p8s[1];i++)
					{
						if(p8s[2+i]>15)
						{//小类号>15
							goto err;
						}
						p8d=(u8 *)(ADDR_DATABUFF);
						MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+(p8s[0]*32*16)+(p8s[2+i]*32),1);
						x=p8d[0];
						if(x>31)
						{
							x=31;
						}
						if((TXlen+4+14+8+2+x+2)>LEN_UARTnTx)
						{
							goto err;
						}
						p8d=(u8 *)(TXaddr);
						p8d[0]=p8s[2+i];//小类号
						MR(TXaddr+1,TerminalParamterLib[Fn-1].Addr+(p8s[0]*32*16)+(p8s[2+i]*32),x+1);
						TXlen+=x+2;
						TXaddr+=x+2;
						p8txbuff[1]+=1;//本次查询数量+1
					}
					RXaddr+=2+i;
					DIlen-=2+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 41://时段功控定值查询
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3))),1);
					p8s[0]&=0x7;
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=p8s[0];//方案标志
					TXlen+=1;
					TXaddr+=1;
					for(i=1;i<7;)
					{
						if((p8s[0]&i)!=0x0)
						{
							switch(i)
							{
								case 1:
									MR(ADDR_DATABUFF+1,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1,1);
									x=(BitNum(p8s[1])*2)+1;//计算置位为1的个数(0-32)
									if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
									{
										goto err;
									}
									MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1,x);
									break;
								case 2:
									MR(ADDR_DATABUFF+1,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+17,1);
									x=(BitNum(p8s[1])*2)+1;//计算置位为1的个数(0-32)
									if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
									{
										goto err;
									}
									MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+17,x);
									break;
								case 4:
									MR(ADDR_DATABUFF+1,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+34,1);
									x=(BitNum(p8s[1])*2)+1;//计算置位为1的个数(0-32)
									if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
									{
										goto err;
									}
									MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(1+(17*3)))+1+34,x);
									break;
							}
							TXlen+=x;
							TXaddr+=x;
						}
						i<<=1;
					}
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 65://定时上报1类数据任务设置
				case 66://定时上报2类数据任务设置				
					MR(ADDR_DATABUFF,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(9+(4*MaxTimerTaskDI)))+8,1);
					x=(p8s[0]*4)+9;
					if((TXlen+4+14+8+2+x)>LEN_UARTnTx)
					{
						goto err;
					}
					MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*(9+(4*MaxTimerTaskDI))),x);
					TXlen+=x;
					TXaddr+=x;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 84://终端通信端口配置参数
					if((TXlen+4+14+8+2+1)>LEN_UARTnTx)
					{
						goto err;
					}
					p8s=(u8 *)(RXaddr);
					i=p8s[0];
					if((DIlen-4)<(1+i))
					{
						goto err;
					}
					p8txbuff=(u8 *)(TXaddr);
					p8txbuff[0]=0;//本次查询数量=0
					TXlen+=1;
					TXaddr+=1;
					for(i=0;i<p8s[0];i++)
					{
						if((TXlen+4+14+8+2+15)>LEN_UARTnTx)
						{
							goto err;
						}
						if(p8s[1+i]==0x0)
						{
							continue;	
						}
						if(p8s[1+i]>AFN04F84MaxPn)
						{
							continue;	
						}
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((p8s[1+i]-1)*15),15);
						p8d=(u8 *)(TXaddr);
						if(p8s[1+i]==p8d[0])
						{//序号相同
							TXlen+=15;
							TXaddr+=15;
							p8txbuff[0]++;//本次查询数量+1
						}
					}
					RXaddr+=1+i;
					DIlen-=1+i;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 97://F97停电数据采集配置参数
					i=MRR(ADDR_AFN04F97+3,2);
					if(i>MaxRS485AddCarrierPn)
					{
						i=MaxRS485AddCarrierPn;
					}
					i=5+(i*2);
					break;
				case 104://曲线冻结密度参数(测量点号)
					y=0;//冻结密度非默认值的曲线个数=0
					for(i=1;i<=248;i++)
					{
						x=Get_CurveSetFnListNo(i);//得到曲线冻结密度参数Fn列表号;返回:列表号0-n,0xffffffff表示无
						if(x!=0xffffffff)
						{
							x=MRR(ADDR_AFN04F104+((Pn-1)*LEN_AFN04F104_PerPn)+x,1);
							if(x!=127)
							{//只返回更改过冻结密度的曲线，即冻结密度内容非“127”的曲线
								if((TXlen+4+14+8+2+(1+((y+1)*2)))>LEN_UARTnTx)
								{
									goto err;
								}
								x<<=8;
								x|=i;
								MWR(x,TXaddr+1+(y*2),2);
								y++;//
							}
						}
					}
					MWR(y,TXaddr,1);
					TXlen+=(1+(y*2));
					TXaddr+=(1+(y*2));
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
				case 106://电能表数据分级参数(数据分级类号)
					y=0;//已配置的电能表数据个数n
					x=ADDR_AFN04F106+((5*NUM_AFN04F106_PerC)*(Pn-1));
					for(i=0;i<NUM_AFN04F106_PerC;i++)
					{
						f=MRR(x+4,1);
						if(f!=0)
						{
							if((TXlen+4+14+8+2+(1+((y+1)*5)))>LEN_UARTnTx)
							{
								goto err;
							}
							MR(TXaddr+1+(y*5),x,5);
							y++;
						}
						x+=5;
					}
					MWR(y,TXaddr,1);
					TXlen+=(1+(y*5));
					TXaddr+=(1+(y*5));
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
					
				case 150://查询测量点状态
					//集中器在收到查询测量点状态命令（F150）n分钟（n=0~20，一般取10）内不做本地F10终端电能表/交流采样装置配置参数的自动更新
					Terminal_Ram->AutoUpdataAFN04F10_S_Timer=10*60;//25 自动更新F10秒定时器;集中器在收到查询测量点状态命令(F150)n分钟(n=0~20,一般取10)内不做本地F10终端电能表/交流采样装置配置参数的自动更新
					i=TerminalParamterLib[Fn-1].Len;
					break;
			#if (USER/100)==12//四川用户
/*
参数设置AFN04 扩展F151 485口功能配置。数据1字节，D0~D7y依次表示485I、485II、…
置1，表示485本地通信口。置0表示为485抄表口。
*/
				case 151:
				AFN0AF151:
					if(Comm_Ram->Factory==0x55)
					{//0x55=工厂模式
						i=0;
					}
					else
					{
						i=MRR(ADDR_UART_PORT_SET+LEN_UART_PORT_SET,1);
						if(i!=0)
						{
							i=1;
						}
					}
					x=MRR(ADDR_UART_PORT_SET+2*LEN_UART_PORT_SET,1);
					if(x!=0)
					{
						x=1;
					}
					i|=(x<<1);
					i=((~i)&0x3);
					MWR(i,TXaddr,1);
					TXlen+=1;
					TXaddr+=1;
					f=1;
					FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
					continue;
			#endif
			#if (USER/100)==5//用户标识:上海系
				case 160://档案完整性比对
					crc = 0;
					p8d=(u8*)(ADDR_DATABUFF);
					p16=(u16*)(ADDR_PnVSAFN04F10SetNo);//测量点号Pn对应AFN04F10配置序号,0为无效
					for(x=2;x<=AFN04F10MaxPn;x++)
			    {
						y=p16[x];//pn对应AFN04F10配置序号,0为无效
						if(y!=0)
						{
							y--;
							p8d[0]=x;
							p8d[1]=x>>8;
							MR(ADDR_DATABUFF+2,ADDR_AFN04F10+6+(LEN_AFN04F10_Pn*y),6);
						#if ((Project/100)==2)//集中器
							if(MRR(ADDR_AFN04F10+5+(LEN_AFN04F10_Pn*y),1)==21)
							{//上海规约
								//第3字节：制造厂代码，1字节ASCII码，如P，则该抄表帧里为50H，主站下发给终端为80H(BCD码)
								p8d[5]=hex_bcd(p8d[5]);	
							}
						#endif
							for(y=0;y<8;y++)
							{
				        crc = crc ^ (p8d[y] << 8);
				        for (i = 0; i < 8; i++)
				        {
				            if (crc & 0x8000)
				                crc = (crc << 1) ^ 0x1021;
				            else
				                crc = crc << 1;
				        }
							}
						}
			    }
					p8d[0]=crc;
					p8d[1]=crc>>8;
					MR(ADDR_DATABUFF+2,ADDR_AFN04F150,256);
					i=2+256;
					break;
			#endif
			#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
				case 296://辽宁锦州、浙江电表对时误差状态
			AFN0AF296:
					i=256;
					break;	
				case 297://辽宁锦州、浙江终端对时参数
			AFN0AF297:
					i=10;
					break;	
				case 298://辽宁锦州、浙江电表对时参数
			AFN0AF298:
					i=8;
					break;
			#endif
			#if (USER/100)==11//吉林系
				case 6://
					if(Comm_Ram->Factory==0x55)//0x55=工厂状态
					{
						i=16;
					}
					else
					{
						i=TerminalParamterLib[Fn-1].Len;
					}
					break;
				case 89://
					if(Comm_Ram->Factory==0x55)//0x55=工厂状态
					{
						i=TerminalParamterLib[Fn-1].Len;
					}
					else
					{
						i=6;
					}
					break;
			#endif
			#if (USER/100)==6//湖南系
				case 200://终端电话号码和SIM卡卡号
			AFN0AF200:				
					i=29;
					break;
			#endif
				default:
					i=TerminalParamterLib[Fn-1].Len;
					break;
			}
			if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
			{
				goto err;
			}
		#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
			if(Pn==0x0)
			{
				if((Fn!=296)||(Fn!=297)||(Fn!=298))
				{
					MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
				}
				if(Fn==296)
				{
					MC(0,ADDR_AFN0AF296_Ext,i);
					p8s=(u8 *)(RXaddr);
					for(x=1;x<=MaxRS485AddCarrierPn;x++)
					{
						if(PnSet(x)!=0)//有效测量点号检查;返回0=无效,1=有效
						{
							y=MRR(ADDR_AFN0CF250_Ext1+10*(x-1)+5,2);
							if((y>>15)!=0x0)
							{//负
								y=~y;
								y++;
							}
							if(y>=(p8s[0]+(p8s[1]<<8)))
							{
								y=MRR(ADDR_AFN0AF296_Ext+(x/8),1);
								y|=(1<<(x%8));
								MWR(y,ADDR_AFN0AF296_Ext+(x/8),1);
							}
						}
					}
					MR(TXaddr,ADDR_AFN0AF296_Ext,i);
					RXaddr+=2;
				}
				if(Fn==297)
				{
					MR(TXaddr,ADDR_AFN04F297_Ext,i);
				}
				if(Fn==298)
				{
					MR(TXaddr,ADDR_AFN04F298_Ext,i);
				}
			}
		#else
			#if ((USER/100)==13)&&((Project/100)==3)//黑龙江专变
			if(Pn==0x0)
			{
				MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
				if(Fn==150)
				{//F150不计算脉冲测量点
					//终端脉冲配置参数
					p8txbuff=(u8 *)(TXaddr);
					x=ADDR_AFN04F11+1;
					for(y=1;y<=AFN04F11MaxPn;y++)
					{
						z=MRR(x,1);
						if(z!=0)
						{
					#if AFN04F150Dn==0//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
							z--;
					#endif
							p8txbuff[z/8]&=(~(1<<(z%8)));
						}
						x+=5;
					}
				}
			}
			#else
				#if (USER/100)==11//吉林
				if(Pn==0x0)
				{
					if(Fn==89)
					{
						if(Comm_Ram->Factory==0x55)//0x55=工厂状态
						{
							MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
						else
						{
							MR(TXaddr,TerminalParamterLib[Fn-1].Addr,2);
							MR(TXaddr+2,ADDR_04000401,4);
						}
					}
					else
					{
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
				}
				#else
					#if (USER/100)==6//湖南系
					if(Pn==0x0)
					{
						if(Fn==200)
						{
							if((TXlen+4+14+8+2+i)>LEN_UARTnTx)
							{
								goto err;
							}
							//8个字节终端电话号码，P0～P15对应电话号码第一位至最末位，余下未填满部分填FH
							MC(0xFF,TXaddr,8);
							p8s=(u8 *)ADDR_MobilePhoneNumber;
							p8d=(u8 *)TXaddr;
							for(x=0;x<20;x++)
							{
								if(p8s[x]==0)
								{
									break;
								}
							}
							if(x!=0)
							{
							 	if(x>=16)
								{
									y=ASCII_BCD(ADDR_MobilePhoneNumber+8,8);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
									MWR(y,ADDR_DATABUFF,4);//写终端电话号码
									y=ASCII_BCD(ADDR_MobilePhoneNumber,8);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
									MWR(y,ADDR_DATABUFF+4,4);//写终端电话号码
									Data_Inverse(ADDR_DATABUFF,8);//数据倒序(高低字节调换),入口Le>=2
									MW(ADDR_DATABUFF,TXaddr,8);
								}
								else
								{//x<16
									if(x>=8)
									{
										if(x&1)
										{//奇数
											y=ASCII_BCD(ADDR_MobilePhoneNumber+(x-7),7);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
											y<<=4;
											MWR(y,ADDR_DATABUFF,4);//写终端电话号码
											y=ASCII_BCD(ADDR_MobilePhoneNumber,(x-7));//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
											MWR(y,ADDR_DATABUFF+4,(x-7)/2);//写终端电话号码
											Data_Inverse(ADDR_DATABUFF,(x+1)/2);//数据倒序(高低字节调换),入口Le>=2
											MW(ADDR_DATABUFF,TXaddr,(x+1)/2);
											p8d[(x-1)/2]|=0x0F;
										}
										else
										{//偶数
											y=ASCII_BCD(ADDR_MobilePhoneNumber+(x-8),8);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
											MWR(y,ADDR_DATABUFF,4);//写终端电话号码
											y=ASCII_BCD(ADDR_MobilePhoneNumber,(x-8));//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
											MWR(y,ADDR_DATABUFF+4,(x-8)/2);//写终端电话号码
											Data_Inverse(ADDR_DATABUFF,x/2);//数据倒序(高低字节调换),入口Le>=2
											MW(ADDR_DATABUFF,TXaddr,x/2);
										}
									}
									else
									{//x<8
										if(x&1)
										{//奇数
											y=ASCII_BCD(ADDR_MobilePhoneNumber,x);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
											y<<=((8-x)*4);
											MWR(y,ADDR_DATABUFF,(x+1)/2);//写终端电话号码
											if(((x+1)/2)>=2)
											{
												Data_Inverse(ADDR_DATABUFF,(x+1)/2);//数据倒序(高低字节调换),入口Le>=2
											}
											MW(ADDR_DATABUFF,TXaddr,(x+1)/2);
											p8d[(x-1)/2]|=0x0F;
										}
										else
										{//偶数
											y=ASCII_BCD(ADDR_MobilePhoneNumber,x);//16进制的ASCII码转为BCD,例"12AB"->0x12AB,入口ASCII码开始地址和ASCII码字节数,返回=0xFFFFFFFF表示错误ASCII码非0-9,A-F值
											MWR(y,ADDR_DATABUFF,x/2);//写终端电话号码
											if((x/2)>=2)
											{
												Data_Inverse(ADDR_DATABUFF,x/2);//数据倒序(高低字节调换),入口Le>=2
											}
											MW(ADDR_DATABUFF,TXaddr,x/2);
										}
									}
								}
							}
							//SIM卡卡号(SM0~SM19对应SIM卡卡号第一位至最末位，填ASCII码，字母要大写)
							MR(TXaddr+8,ADDR_AFN09F9+26,20);
							p8d=(u8 *)(TXaddr+8);
							for(x=0;x<20;x++)
							{
								if((p8d[x]>=0x61)&&(p8d[x]<=0x7a))
								{//0x61~0x7a对应小写字母a~z，0x41~0x5a对应大写字母A~Z
									p8d[x]-=0x20;
								}
							}
							k=Terminal_Ram->SignaldBm;
							if(k==0x99)
							{
								k=0;
							}
							k=bcd_hex(k);//8位压缩BCD数转为HEX
							MWR(k,TXaddr+8+20,1);
 						}
						else
						{
							MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
						}
					}				
					#else
					if(Pn==0x0)
					{
						MR(TXaddr,TerminalParamterLib[Fn-1].Addr,i);
					}
					#endif
				#endif
			#endif
		#endif	
			else
			{
				MR(TXaddr,TerminalParamterLib[Fn-1].Addr+((Pn-1)*i),i);
			}
			FnPntoDI(TXDIaddr,(Fn<<16)+Pn);//FnPn组合为数据标识
			TXlen+=i;
			TXaddr+=i;
			if(i!=0x0)
			{
				f=1;
			}
		}
	}
	if(TXlen==0x0)
	{
		goto err;
	}
	TXlen+=8;
	p8txbuff=(u8 *)Get_ADDR_UARTnTx(PORTn);
	p8txbuff[1]=TXlen;
	p8txbuff[2]=TXlen>>8;
	DIlen=p8rxbuff[1]+(p8rxbuff[2]<<8);
	Uart_376_AUX(PORTn,((u32)p8rxbuff)+6+DIlen-6);//附加信息
	Uart_376_CS((u32)p8txbuff);//和校验计算,同时加结束0x16;修改长度字节的标志位
}













