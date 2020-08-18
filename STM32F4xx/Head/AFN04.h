

#ifndef AFN04_H
#define AFN04_H

#define ADDR_AFN0CF1     ADDR_AFN04_Start//终端版本信息
//厂商代号	ASCII	4
//设备编号	ASCII	8
//终端软件版本号	ASCII	4
//终端软件发布日期：日月年	见附录A.20	3
//终端配置容量信息码	ASCII	11
#define ADDR_AFN01Fx     ADDR_AFN0CF1+30//终端AFN01复位命令
//4BYTE 命令下发的端口控制缓冲首地址,当为菜单操作时为0x55555555
//1BYTE 复位命令的Fn值:1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
//1BYTE 集中器路由器复位命令的Fn值:在执行复位命令时复制上字节值(路由初始化AFN05F52复制到本字节)
#define ADDR_Protocol_No     ADDR_AFN01Fx+6//抄表规约编号
//1BYTE 交流采样装置(2)
//1BYTE DL/T645-1997(1)
//1BYTE DL/T645-2007(30)
//1BYTE 串行接口连接窄带低压载波(31)
//...

//终端AFN04设置数据开始(最前为通信参数)
//4BYTE CRC32
//4BYTE 校表参数有效标志固定为0x12345678
//通信参数----------------------------------------------------------------------------------------
#define ADDR_AFN04F89     ADDR_Protocol_No+64
#define ADDR_AREACODE     ADDR_AFN04F89//行政区域码
//2字节BCD
#define ADDR_TERMINAL     ADDR_AREACODE+2//终端地址
//2字节BIN
//8BYTE 设备编号ASCII
//6BYTE MAC(MAC1-MAC6,高-低)
#define ADDR_AFN04F1     ADDR_TERMINAL+16//终端上行通信口通信参数设置
//终端数传机延时时间RTS	              BIN 20ms  1字节
//终端作为启动站允许发送传输延时时间	  BIN min   1字节
//终端等待从动站响应的超时时间和重发次数 BIN秒、次	2字节
//需要主站确认的通信服务(CON=1)的标志	  BS8       1字节
//心跳周期                               BIN min  1字节
#define ADDR_AFN04F2     ADDR_AFN04F1+6//终端上行通信口无线中继转发设置
//    D7	         D6～DO	
//允许／禁止	  被转发的终端地址数n	    l字节
//    被转发终端地址i(i=l～n)	  2*n字节=2*16=32字节(MAX)
#define ADDR_AFN04F3     ADDR_AFN04F2+33//主站IP地址和端口
//IP地址l段	    BIN	l	
//IP地址2段	    BIN	1	
//IP地址3段	    BIN	1	  主用
//IP地址4段	    BIN	1	
//端口地址	        BIN	2	
//IP地址1段	    BIN	l	备用
//IP地址2段	    BIN	1	
//IP地址3段	    BIN	1	
//IP地址4段	    BIN	l	
//端口地址	        BIN	2	
//APN	   ASCII	16	
#define ADDR_AFN04F4     ADDR_AFN04F3+28//主站电话号码和短信中心号码
//Z0	Z1	BIN	1	主站电话号码或主站手机号码
//Z2	Z3	BIN	1	
//Z4	Z5	BIN	l	
//Z6	Z7	BIN	l	
//Z8	Z9	BIN	1	
//Z10	Z11	BIN	l	
//Z12	Z13	BIN	1	
//Z14	Z15	BIN	l	
//SM0	SMl	BIN	l	短信中心号码
//SM2	SM3	BIN	l	
//SM4	SM5	BIN	l	
//SM6	SM7	BIN	1	
//SM8	SM9	BIN	1	
//SMl0	SMll    BIN	l
//SMl2	SMl3	BIN	l
//SMl4	SMl5	BIN	1
#define ADDR_AFN04F5     ADDR_AFN04F4+16//终端上行通信消息认证参数
//消息认证方案号	    BIN	    1
//消息认证方案参数	    BIN	    2
#define ADDR_AFN04F6     ADDR_AFN04F5+3//终端组地址
//终端组地址l	    BIN	    2
//终端组地址2	    BIN	    2
//终端组地址3	    BIN	    2
//终端组地址4	    BIN	    2
//终端组地址5	    BIN	    2
//终端组地址6	    BIN	    2
//终端组地址7	    BIN	    2
//终端组地址8	    BIN	    2
#if (USER/100)==11//吉林系
#define ADDR_AFN04F7     ADDR_AFN04F6+32//终端IP地址和端口
#else
#define ADDR_AFN04F7     ADDR_AFN04F6+16//终端IP地址和端口
#endif
//终端IP地址1段 BIN 1 
//终端IP地址2段 BIN 1 
//终端IP地址3段 BIN 1 
//终端P地址4段 BIN 1 
//子网掩码地址1段 BIN 1 
//子网掩码地址2段 BIN 1 
//子网掩码地址3段 BIN 1 
//子网掩码地址4段 BIN 1 
//网关地址1段 BIN 1 
//网关地址2段 BIN 1 
//网关地址3段 BIN 1 
//网关地址4段 BIN 1 
//代理类型 BIN 1 
//代理服务器地址1段 BIN 1 
//代理服务器地址2段 BIN 1 
//代理服务器地址3段 BIN 1 
//代理服务器地址4段 BIN 1 
//代理服务器端口 BIN 2 
//代理服务器连接方式 BIN 1 
//用户名长度m(0～20) BIN 1 
//用户名 ASCII m 
//密码长度n(0～20) BIN 1 
//密码 ASCII n 
//终端侦听端口 BIN 2 
//1BYTE自扩展终端IP类型,0=固定,1=自动
#define ADDR_AFN04F8     ADDR_AFN04F7+64+1//终端上行通信工作方式（以太专网或虚拟专网）
//工作模式 BS8  1 
//永久在线、时段在线模式重拨间隔 BIN 秒 2 
//激活模式重拨次数 BIN 次 1 
//连续无通信自动断线时间 BIN min 1 
//模式允许在线时段标志 BS24  3 
#define ADDR_AFN04F16     ADDR_AFN04F8+8//虚拟专网用户名、密码
//虚拟专网用户名 ASCII 32
//密码  ASCII 32
#define ADDR_AFN04F90     ADDR_AFN04F16+64//无线通信参数（信道、发射功率）
//无线信道组 BIN 1
//无线网络发射功率 BIN 1
#define ADDR_AFN04F91     ADDR_AFN04F90+2//终端地理位置信息
//5BYTE
//5BYTE
#define ADDR_IPFILTER     (ADDR_AFN04F91+10)//作为服务器时远端滤波IP
//4BYTE IP1
//4BYTE IP2
//4BYTE IP3
//4BYTE IP4
//4BYTE IP5
#define ADDR_MenuKey     (ADDR_IPFILTER+20)
//6BYTE 菜单密码,仅在恢复至出厂配置参数初始化时清0


//通信参数结束------------------------------------------------------------------------------------
#define ADDR_AFN04F9     (ADDR_MenuKey+6)//终端事件记录配置设置
//事件记录有效标志位 BS64 8 
//事件重要性等级标志位 BS64 8 
#define ADDR_AFN04F10     (ADDR_AFN04F9+16)//终端电能表／交流采样装置配置参数
//电能表/交流采样装置序号 BIN 2 
//所属测量点号 BIN 2 
//通信速率及端口号 BIN 1 (D4～D0编码表示电能表、交流采样装置与终端连接所对应的终端通信端口号，数值范围1～31，其他值无效)
//通信协议类型 BIN 1 (其中0：表示终端无需对本序号的电能表/交流采样装置进行抄表；1：DL/T 645-1997.；2：交流采样装置通信协议；30：DL/T 645-2007；31："串行接口连接窄带低压载波通信模块"接口协议；其他：备用)
//通信地址 见附录A.12 6 
//通信密码 BIN 6 
//电能费率个数 BS8 1 
//有功电能示值整数位及小数位个数 BS8 1 
//所属采集器通信地址 见附录A.12 6 
//用户大类号及用户小类号 BS8 1 
//.....
//电能表/交流采样装置序号 BIN 2 
//所属测量点号 BIN 2 
//通信速率及端口号 BIN 1 
//通信协议类型 BIN 1 
//通信地址 见附录A.12 6 
//通信密码 BIN 6 
//电能费率个数 BS8 1 
//有功电能示值整数位及小数位个数 BS8 1 
//所属采集器通信地址 见附录A.12 6 
//用户大类号及用户小类号 BS8 1 	
#define LEN_AFN04F10_Pn     27
#define ADDR_AFN04F11     ADDR_AFN04F10+(LEN_AFN04F10_Pn*AFN04F10MaxPn)//终端脉冲配置参数
//脉冲输入端口号	BIN	1	
//所属测量点号	BIN	1	本次配置第l路脉冲
//脉冲属性	BS8	1	
//电表常数k	BIN	2	
//...
//脉冲输入端口号	BIN	l	
//所属测量点号	BIN	1	本次配置第n路脉冲
//脉冲属性	BS8	1	
//电表常数k	BIN	2	
#define ADDR_AFN04F12     ADDR_AFN04F11+(5*AFN04F11MaxPn)//终端状态量输入参数
//状态量接入标志位(对应1～8路状态量)	    BS8	    1
//状态量属性标志位(对应1～8路状态量)	    BS8	    1
#define ADDR_AFN04F13     ADDR_AFN04F12+2//终端电压／电流模拟量配置参数
//电压／电流模拟量输入端口号	BIN	l	
//所属测量点号	BIN	1	第1路电压／电流模拟量
//电压／电流模拟量属性	BS8	l	
//...
//电压／电流模拟量输入端口号	BIN	1	本次配置
//所属测量点号	BIN	1	第n路电压／
//电压／电流模拟量属性	BS8	l	电流模拟量
#define ADDR_AFN04F14     ADDR_AFN04F13+(3*AFN04F13MaxPn)//终端总加组配置参数(固定空间相隔)
//总加组序号(1～8)	BIN	1	
//总加组的测量点数量m1(0～64)	BIN	1	如为"0"，表示本总加组没有要参与总加的测量点，即被"删除"。
//第1个测量点号及总加标志	BS8	l	
//...
//第m1个测量点号及总加标志	BS8	l	
//....
//总加组序号			本次配置第n总加组
//总加组的测量点数量mn(0～64)	BIN	1	
//第1个测量点号及总加标志	BS8	1	
//			
//第mn个测量点号及总加标志	BS8	l	
#define LEN_AFN04F14_Pn     (2+64)
#define ADDR_AFN04F15     ADDR_AFN04F14+(LEN_AFN04F14_Pn*AFN04F14MaxPn)//有功总电能量差动越限事件参数设置
//有功总电能量差动组号	BIN		1	
//对比总加组	BIN		l	
//参照总加组号	BIN		l	
//参与差动的电能量的时间区间及对比方法标志	BS8		l
//差动越限相对偏差值	BIN	％	1	
//差动越限绝对偏差值	数据格式03	kWh	4	
//...
//有功总电能量差动组号	BIN		1	
//对比总加组号	BIN		1	第n组
//参与差动的电能量的时间区间及对比方法标志	BS8		1	
//差动越限相对偏差值	    BIN	  ％	    l	
//差动越限绝对偏差值	数据格式03	kWh	4	
#define ADDR_AFN04F17     ADDR_AFN04F15+(9*AFN04F15MaxPn)//终端保安定值
//保安定值	数据格式02	kW	2

#define ADDR_AFN04F18     ADDR_AFN04F17+2//终端功控时段
//D7	D6	D5	D4	D3	D2	D1	D0		
//1:30-2:00	1:00-1:30	0:30-1:00	0:00-0:30	BS8	第l字节
//3:30-4:00	3:00-3:30	2:30-3:00	2:00-2:30	BS8	第2字节
//...	
//23:30-24:00	23:00-23:30	22:30-23:00	22:00-22:30	BS8	第12字节
#define ADDR_AFN04F19     ADDR_AFN04F18+12//终端时段功控定值浮动系数
//时段功控定值浮动系数	数据格式04	％	1
#define ADDR_AFN04F20     ADDR_AFN04F19+1//终端月电能量控定值浮动系数
//月电能量控定值浮动系数	数据格式04	％	1
#define ADDR_AFN04F21     ADDR_AFN04F20+1//终端电能量费率时段和费率
//0：00～0：30时段费率号 BIN 1 
//0：30～1：00时段费率号 BIN 1 
//…… …… ……
//23：00～23：30时段费率号 BIN 1 
//23：30～0：00时段费率号 BIN 1 
//费率数 BIN 1 
#define ADDR_AFN04F22     ADDR_AFN04F21+49//终端电能量费率
//费率数M BIN 个 1 
//第1费率	数据格式03	厘／元	4
//第2费率	数据格式03	厘／元	4
//
//第M费率	数据格式03	厘／元	4
#define ADDR_AFN04F23     ADDR_AFN04F22+1+(48*4)//终端催费告警参数
//催费告警允许／禁止标志位	BS24	3
#define ADDR_AFN04F25     ADDR_AFN04F23+3//测量点基本参数(测量点号)
//测量点号1
//电压互感器倍率 BIN  2 
//电流互感器倍率 BIN  2 
//额定电压 见附录A.7 V 2 
//额定电流 见附录A.22 A 1 
//额定负荷 见附录A.23 kVA 3 
//电源接线方式 BS8  1 
//...
//测量点号N
#define ADDR_AFN04F26     ADDR_AFN04F25+(11*AFN04F25MaxPn)//测量点限值参数(测量点号)
//测量点号1
//0 电压合格上限 见附录A.7 V 2 
//2 电压合格下限 见附录A.7 V 2 
//4 电压断相门限 见附录A.7 V 2 
//6 电压上上限（过压门限） 见附录A.7 V 2 
//8 越限持续时间 BIN min 1 
//9 越限恢复系数 见附录A.5 ％ 2 
//11 电压下下限（欠压门限） 见附录A.7 V 2 
//13 越限持续时间 BIN min 1 
//14 越限恢复系数 见附录A.5 ％ 2 
//16 相电流上上限（过流门限） 见附录A.25 A 3 
//19 越限持续时间 BIN min 1 
//20 越限恢复系数 见附录A.5 ％ 2 
//22 相电流上限（额定电流门限） 见附录A.25 A 3 
//25 越限持续时间 BIN min 1 
//26 越限恢复系数 见附录A.5 ％ 2 
//28 零序电流上限 见附录A.25 A 3 
//31 越限持续时间 BIN min 1 
//32 越限恢复系数 见附录A.5 ％ 2 
//34 视在功率上上限 见附录A.23 kVA 3 
//37 越限持续时间 BIN min 1 
//38 越限恢复系数 见附录A.5 ％ 2 
//40 视在功率上限 见附录A.23 kVA 3 
//43 越限持续时间 BIN min 1 
//44 越限恢复系数 见附录A.5 ％ 2 
//46 三相电压不平衡限值 见附录A.5 ％ 2 
//48 越限持续时间 BIN min 1 
//49 越限恢复系数 见附录A.5 ％ 2 
//51 三相电流不平衡限值 见附录A.5 ％ 2 
//53 越限持续时间 BIN min 1 
//54 越限恢复系数 见附录A.5 ％ 2 
//56 连续失压时间限值 BIN min 1 
//...
//测量点号N
#define ADDR_AFN04F27     ADDR_AFN04F26+(57*AFN04F26MaxPn)//测量点铜损、铁损参数(测量点号)
//测量点号1
//A相电阻RA 见附录A.26 Ω 2 
//A相电抗XA 见附录A.26 Ω 2 
//A相电导GA 见附录A.26 S 2 
//A相电纳BA 见附录A.26 S 2 
//B相电阻RB 见附录A.26 Ω 2 
//B相电抗XB 见附录A.26 Ω 2 
//B相电导GB 见附录A.26 S 2 
//B相电纳BB 见附录A.26 S 2 
//C相电阻RC 见附录A.26 Ω 2 
//C相电抗XC 见附录A.26 Ω 2 
//C相电导GC 见附录A.26 S 2 
//C相电纳BC 见附录A.26 S 2 
//...
//测量点号N
#define ADDR_AFN04F28     ADDR_AFN04F27+(24*AFN04F27MaxPn)//测量点功率因数分段限值(测量点号)
//测量点号1
//功率因数分段限值l	数据格式05	％	2
//功率因数分段限值2	数据格式05	％	2
//...
//测量点号N
#define ADDR_AFN04F29     ADDR_AFN04F28+(4*AFN04F28MaxPn)//终端当地电能表显示号(测量点号)
//终端当地电能表显示号 ASCII 12 
//...
//测量点号N
#define ADDR_AFN04F30     ADDR_AFN04F29+(12*AFN04F29MaxPn)//台区集中抄表停抄/投抄设置(测量点号)
//台区集中抄表停抄/投抄设置 BIN 1 
//...
//测量点号N
#define ADDR_AFN04F31     ADDR_AFN04F30+(1*AFN04F30MaxPn)//载波从节点附属节点地址(测量点号)
//测量点号1
//第1个载波从节点附属节点地址 见附录A.12 6 
//…… …… …… 
//第20个载波从节点附属节点地址 见附录A.12 6 
//...
//测量点号N
#define ADDR_AFN04F33     ADDR_AFN04F31+(120*AFN04F31MaxPn)//终端抄表运行参数设置
//终端通信端口号n(n=1～31) BIN 1 
//台区集中抄表运行控制字 BS16 2 
//抄表日-日期 BS32 4 
//抄表日-时间 见附录A.19 2 
//抄表间隔时间 BIN 1 
//对电表广播校时定时时间 见附录A.18 3 
//允许抄表时段数m（0≤m≤24） BIN 1 
//1个允许抄表时段开始时间 见附录A.19 2 
//1个允许抄表时段结束时间 见附录A.19 2 
//…… …… …… 
//m个允许抄表时段开始时间 见附录A.19 2 
//m个允许抄表时段结束时间 见附录A.19 2 
#define ADDR_AFN04F34     ADDR_AFN04F33+((14+(4*24))*31)//集中器下行通信模块的参数设置
//终端通信端口号(n=1～31) BIN 1 
//端接口端的通信控制字 BS8 1 
//口对应端的通信速率（bps） BIN 4 
#define ADDR_AFN04F35     ADDR_AFN04F34+(6*31)//台区集中抄表重点户设置
//台区集中抄表重点户个数n=0～10 BIN 1 
//重点户的电能表/交流采样装置序号 BIN 2 
//…… …… …… 
//重点户的电能表/交流采样装置序号 BIN 2 
#define ADDR_AFN04F36     ADDR_AFN04F35+1+(2*MaxKeyUser)//终端上行通信流量门限设置
//月通信流量门限 BIN 字节 4 
#define ADDR_AFN04F37     ADDR_AFN04F36+4//终端级联通信参数
//终端级联通信端口号 BIN  1 
//终端级联通信控制字 BS8  1 
//接收等待报文超时时间 BIN 100ms 1 
//接收等待字节超时时间 BIN 10ms 1 
//级联方（主动站）接收失败重发次数 BIN 次 1 
//级联巡测周期 BIN min 1 
//级联/被级联标志、及其对应的被级联/级联的终端个数n(0-3) BIN  1 
//第1个被级联的终端行政区划码/级联方的终端行政区划码 BCD  2 
//第1个被级联的终端地址/级联方的终端地址 BIN  2 
//…… ……  ……
//第n个被个被级联的终端行政区划码 BCD  2 
//第n个被级联的终端地址 BIN  2 
#define ADDR_AFN04F38     ADDR_AFN04F37+7+(4*3)//1类数据配置设置(存贮时按大类和小类固定空间相隔)
//大类号0
//用户小类号0 
//信息类组数n(0～31) BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 
//...
//用户小类号15 
//信息类组数n BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 

//.....

//大类号15
//用户小类号0 
//信息类组数n(0～31) BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 
//...
//用户小类号15 
//信息类组数n BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 

#define ADDR_AFN04F39     ADDR_AFN04F38+(32*16*16)//2类数据配置设置(存贮时按大类和小类固定空间相隔)
//大类号0
//用户小类号0 
//信息类组数n(0～31) BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 
//...
//用户小类号15 
//信息类组数n BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 

//.....

//大类号15
//用户小类号0 
//信息类组数n(0～31) BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 
//...
//用户小类号15 
//信息类组数n BIN 1 
//第1组信息类组所对应的信息类元标志位 BS8 1 
//...
//第n组信息类组所对应的信息类元标志位 BS8 1 
	
#define ADDR_AFN04F41     ADDR_AFN04F39+(32*16*16)//时段功控定值(总加组号)(存贮时3个方案的时段固定空间相隔)
//总加组号1-N
//方案标志	BS8		1	方案标志
//时段号   BS8     1
//时段l功控定值	数据格式02	kW	2
//...
//时段n功控定值（n≤8）	数据格式02	kW	2
//...
//时段号	BS8		1	第i套定值(i=l～3)
//时段l功控定值	数据格式02	kW	2	
//...
//时段n功控定值（n≤8）	数据格式02	kW	2
#define ADDR_AFN04F42     ADDR_AFN04F41+((1+(17*3))*AFN04F41MaxPn)//厂休功控参数(总加组号)
//总加组号1-N
//厂休控定值	数据格式02	kW	2
//限电起始时间	数据格式19	分时	2
//限电延续时间(范围l～48)	BIN	0.5h	1
//每周限电日：D1～D7表示星期一～星期日，D0=0	BS8		1
#define ADDR_AFN04F43     ADDR_AFN04F42+(6*AFN04F42MaxPn)//功率控制的功率计算滑差时间(总加组号)
//总加组号1-N
//功率控制的功率计算滑差时间(范围：1～60)	BIN	min	1
#define ADDR_AFN04F44     ADDR_AFN04F43+(1*AFN04F43MaxPn)//营业报停控参数(总加组号)
//总加组号1-N
//报停起始时间	数据格式20	日月年	3
//报停结束时间	数据格式20	日月年	3
//报停控功率定值	数据格式02	kW	2
#define ADDR_AFN04F45     ADDR_AFN04F44+(8*AFN04F44MaxPn)//功控轮次设定(总加组号)
//总加组号1-N
//功控轮次标志位	    BS8	    l
#define ADDR_AFN04F46     ADDR_AFN04F45+(1*AFN04F45MaxPn)//月电量控定值(总加组号)
//总加组号1-N
//月电量控定值	数据格式03	kWh／MWh	4
//报警门限系数  %     1
#define ADDR_AFN04F47     ADDR_AFN04F46+(5*AFN04F46MaxPn)//购电量(费)控参数(总加组号)
//总加组号1-N
//购电单号	BIN		4
//追加／刷新标志	BIN		1
//购电量(费)值	数据格式03	kWh／厘	4
//报警门限值	数据格式03	kWh／厘	4
//跳闸门限值	数据格式03	kWh／厘	4
#define ADDR_AFN04F48     ADDR_AFN04F47+(17*AFN04F47MaxPn)//电控轮次设定(总加组号)
//总加组号1-N
//电控轮次标志位	BS8	1
#define ADDR_AFN04F49     ADDR_AFN04F48+(1*AFN04F48MaxPn)//功控告警时间(控制轮次)
//控制轮次1
//功控告警时间	BIN	min	1
//...
//控制轮次N
#define ADDR_AFN04F57     ADDR_AFN04F49+(1*AFN04F49MaxPn)//终端声音告警允许／禁止设置
//声音告警允许／禁止标志位	BS24	3
#define ADDR_AFN04F58     ADDR_AFN04F57+3//终端自动保电参数
//允许与主站连续无通信时间	BIN	h	1
#define ADDR_AFN04F59     ADDR_AFN04F58+1//电能表异常判别阈值设定
//电能量超差阈值	数据格式22		l
//电能表飞走阈值	数据格式22		l
//电能表停走阈值	BIN	15min	l
//电能表校时阈值	BIN	s	2
#define ADDR_AFN04F60     ADDR_AFN04F59+5//谐波限值
//总畸变电压含有率上限	数据格式05	％	2
//奇次谐波电压含有率上限	数据格式05	％	2
//偶次谐波电压含有率上限	数据格式05	％	2
//2次谐波电压含有率上限 见附录A.5 ％ 2 
//4次谐波电压含有率上限 见附录A.5 ％ 2 
//...
//18次谐波电压含有率上限 见附录A.5 ％ 2 
//3次谐波电压含有率上限 见附录A.5 ％ 2 
//5次谐波电压含有率上限 见附录A.5 ％ 2 
//...
//19次谐波电压含有率上限 见附录A.5 ％ 2 
//总畸变电流有效值上限	数据格式06	A	2
//2次谐波电流有效值上限	数据格式06	A	2
//4次谐波电流有效值上限	数据格式06	A	2
//...
//18次谐波电流有效值上限	数据格式06	A	2
//3次谐电流波有效值上限	数据格式06	A	2
//5次谐波电流有效值上限	数据格式06	A	2
//...
//19次谐波电流有效值上限	数据格式06	A	2
#define ADDR_AFN04F61     ADDR_AFN04F60+6+36+2+36//直流模拟量接入参数
//直流模拟量接入标志位	BS8	1

#define ADDR_AFN04F65     ADDR_AFN04F61+1//定时发送1类数据任务设置(任务号)
//任务号1
//周期单位	定时发送周期	BIN	1
//发送基准时间：秒分时日月年	数据格式l	6
//曲线数据抽取倍率R	BIN	l	
//数据单元标识个数n	BIN	1	
//数据单元标识l		4	在请求l类数据报文规定的数据单元标识中选取
//...
//数据单元标识n		4	
//...
//任务号N
#define ADDR_AFN04F66     ADDR_AFN04F65+((9+(4*MaxTimerTaskDI))*AFN04F65MaxPn)//定时发送2类数据任务设置(任务号)
//任务号1
//周期单位	定时发送周期	BIN	1
//发送基准时间：秒分时日月年	数据格式l	6
//曲线数据抽取倍率R	BIN	l	
//数据单元标识个数n	BIN	1	
//数据单元标识l		4	在请求2类数据报文规定的数据单元标识中选取
//...
//数据单元标识n		4	
//...
//任务号N
#define ADDR_AFN04F67     ADDR_AFN04F66+((9+(4*MaxTimerTaskDI))*AFN04F66MaxPn)//定时发送1类数据任务启动／停止设置(任务号)
//任务号1
//启动/停止标志	BIN	1
//...
//任务号N
#define ADDR_AFN04F68     ADDR_AFN04F67+(1*AFN04F67MaxPn)//定时发送2类数据任务启动／停止设置(任务号)
//任务号1
//启动/停止标志	BIN	1
//...
//任务号N

#define ADDR_AFN04F73     ADDR_AFN04F68+(1*AFN04F68MaxPn)//电容器参数(测量点号)
//测量点号1
//共补/分补	备用=O	分补相A、B、C	BS8		1	 第l组
//电容装见容量	数据格式02	kVA	2	
//...	
//共补／分补	备用=0	分补相A、B、C	BS8		l	 第16组
//电容装见容量	数据格式02	kVA	2	
//...
//测量点号N
#define ADDR_AFN04F74     ADDR_AFN04F73+(48*AFN04F73MaxPn)//电容器投切运行参数(测量点号)
//测量点号1
//目标功率因数	数据格式05	％	2
//投入无功功率门限	数据格式23	kvar	3
//切除无功功率门限	数据格式23	kvar	3
//延时时间	BIN	Sec	1
//动作时间间隔	BIN	min	1
//...
//测量点号N
#define ADDR_AFN04F75     ADDR_AFN04F74+(10*AFN04F74MaxPn)//电容器保护参数(测量点号)
//测量点号1
//过电压	数据格式07	    V	    2
//过电压回差值	数据格式07	    V	    2
//欠电压	数据格式07	    V	    2
//欠电压回差值	数据格式07	    V	    2
//总畸变电流含有率上限	数据格式05	  ％	  2
//总畸变电流含有率越限回差值	数据格式05	  ％	  2
//总畸变电压含有率上限	数据格式05	  ％	  2
//总畸变电压含有率越限回差值	数据格式05	  ％	  2
//...
//测量点号N
#define ADDR_AFN04F76     ADDR_AFN04F75+(16*AFN04F75MaxPn)//电容器投切控制方式(测量点号)
//测量点号1
//控制方式	BIN	1
//...
//测量点号N
	
#define ADDR_AFN04F81     ADDR_AFN04F76+(1*AFN04F76MaxPn)//直流模拟量输入变比(直流模拟量点号)
//直流模拟量点号1
//直流模拟量量程起始值 见附录A.2 2 
//直流模拟量量程终止值 见附录A.2 2 
//...
//直流模拟量点号N
#define ADDR_AFN04F82     ADDR_AFN04F81+(4*AFN04F81MaxPn)//直流模拟量限值(直流模拟量点号)
//直流模拟量点号1
//    直流模拟量上限	数据格式02	    2
//    直流模拟量下限	数据格式02	    2
//...
//直流模拟量点号N
#define ADDR_AFN04F83     ADDR_AFN04F82+(4*AFN04F82MaxPn)//直流模拟量冻结参数(直流模拟量点号)
//直流模拟量点号1
//直流模拟量冻结密度	    BIN	    l
//...
//直流模拟量点号N
#define ADDR_AFN04F84     ADDR_AFN04F83+(1*AFN04F83MaxPn)//终端通信端口配置参数
//配置序号BIN 1
//通信速率及端口号BIN 1
//通信协议类型BIN 1
//通信地址见附录A.12 6
//通信密码BIN 6
#define ADDR_AFN04F150     ADDR_AFN04F84+(15*AFN04F84MaxPn)//测量点状态(只读参数)
//256byte 测量点有效标志
//256byte 测量点参数变更标志
#define ADDR_AFN0CF3     ADDR_AFN04F150+(256*2)//AFN04终端参数状态
//31字节(对应AFN04F1-F248)
//1字节保留

#define ADDR_AFN04F97     ADDR_AFN0CF3+32//停电数据采集配置参数P0
//停电数据采集标志	BS8	1	
//停电事件抄读时间间隔	BIN	1	单位为小时
//停电事件抄读时间限值	BIN	1	单位为分钟
//需要读取停电事件电能表个数n	BIN	2	
//测量点号1	BIN	2	第1块需要抄读的电能表
//……	……	……	……
//测量点号n	BIN	2	第n块需要抄读的电能表
#define ADDR_AFN04F98     ADDR_AFN04F97+5+(2*MaxRS485AddCarrierPn)//停电事件甄别限值参数p0
//停电时间最小有效间隔	BIN	2	单位为分钟
//停电时间最大有效间隔	BIN	2	单位为分钟
//停电事件起止时间偏差限值	BIN	2	单位为分钟
//停电事件时间区段偏差限值	BIN	2	单位为秒
//终端停电发生电压限值	见附录A.7	2	V
//终端停电恢复电压限值	见附录A.7	2	V
//停电事件主动上报标志位	BS8	1	

#define ADDR_AFN0CF13     ((ADDR_AFN04F98+13+0xfff)&0xfffff000)//搜索到的电表信息
//2BYTE 总数量
//6BYTE 第1个结果的电能表地址
//1BYTE 第1个结果的通信协议类型
//6BYTE 第1个结果的所属采集器通信地址
//1BYTE 第1个结果的端口号
//1BYTE 第1个结果的电表相位(湖南需要)0:未知,1：A相,2：B相,3：C相
//...
//6BYTE 第n个结果的电能表地址
//1BYTE 第n个结果的通信协议类型
//6BYTE 第n个结果的所属采集器通信地址
//1BYTE 第n个结果的端口号
//1BYTE 第1个结果的电表相位(湖南需要)0:未知,1：A相,2：B相,3：C相
#define LEN_PerMeterAFN0CF13     15
#define ADDR_RMDateFlags     ADDR_AFN0CF13+2+(LEN_PerMeterAFN0CF13*MaxRS485AddCarrierPn)//测量点日抄表成功失败标志;B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
//4BYTE 测量点1,B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)
//4BYTE ...
//4BYTE 测量点n,B0=保留,B1-B31分别表示1-31日抄表成功失败标志,位0=成功1=失败(初始化时为0)


#define ADDR_PnCongealCount     ((ADDR_RMDateFlags+(4*MaxRS485AddCarrierPn)+0xfff)&0xfffff000)//测量点电能表配置变更时的冻结计数值
//测量点1
//1BYTE B0小时冻结0=无效1=有效,B1-B7保留
//2BYTE B0-B14曲线冻结计数,b15=1表示曲线冻结计数满(初始化值0)
//1BYTE B0-B6日冻结计数,B7=1表示日冻结计数满(初始化值0)
//1BYTE B0-B6日冻结计数,B7=1表示抄表日冻结计数满(初始化值0)
//1BYTE B0-B6抄表月冻结计数,B7=1表示月冻结计数满(初始化值0)
//...
//测量点n
#define LEN_PnCongealCount     6//测量点配置变更时的冻结计数值每点长度


#define ADDR_AFN04_BaseEnd     ADDR_PnCongealCount+(LEN_PnCongealCount*MaxRS485AddCarrierPn)




#if (USER/100)==5//用户标识:上海
#define ADDR_AFN04F241     ((ADDR_AFN04_BaseEnd+0xfff)&0xfffff000)//终端对电能表时钟核对的参数p0
//核对频率	BIN	1
//核对开始日期	BIN	1
//核对开始小时	BIN	1
//核对开始分钟	BIN	1
//报警误差间隔分钟	BIN	1
#define ADDR_AFN04F242     ADDR_AFN04F241+5//终端对电能表抄表机制参数p0
//抄表延时分钟数	BIN	1
#define ADDR_AFN04F243       ADDR_AFN04F242+1//终端（SIM卡）手机号码p0
//手机号码	BCD	8
#define ADDR_AFN04F47_Ext     ADDR_AFN04F243+8//显示用购电量参数(总加组号)
//总加组号1
//4BYTE 购前剩余电量(费),数据格式03
//4BYTE 购后剩余电量(费),数据格式03
//...
//总加组号n
#define ADDR_AFN05F201     (ADDR_AFN04F47_Ext+(8*MaxTotalAddGroup))//上海扩展F201终端对表广播校时（AFN=05H）
//1byte 按位依次表示4851、4852、4853、4854，!=0收到此命令,执行后清0
//6byte 主站当前时间（秒、分、时、日、星期-月、年）
//2byte 调整
#define ADDR_AFN04_UserExtEnd     (ADDR_AFN05F201+9)
#else
	#if (USER/100)==2//用户标识:山东版
	#define ADDR_AFN04F88     ((ADDR_AFN04_BaseEnd+0xfff)&0xfffff000)//山东增补协议电能表资产编号
	//15BYTE 电能表资产编号	BCD
#define ADDR_AFN04_UserExtEnd     (ADDR_AFN04F88+(15*AFN04F88MaxPn))//测量点状态(只读参数)
	#else
		#if ((USER/100)==8)||((USER/100)==7)//用户表示：通用、浙江
		#define ADDR_AFN04F297_Ext     ((ADDR_AFN04_BaseEnd+0xfff)&0xfffff000)//辽宁锦州、浙江终端对时参数
		//1byte BIN 对时开关 是否允许终端自动对时 0：关闭1：开启
		//1byte BIN 对时周期 天
		//2byte 对时开始时间 1376.1-2013附录A.19
		//2byte 对时结束时间 1376.1-2013附录A.19
		//1byte BIN 对时最短间隔 分
		//1byte BIN 允许对时次数 
		//1byte BIN 时钟误差阈值 秒
		//1byte BIN 上电事件生成最大延时时间 分
		#define ADDR_AFN04F298_Ext		ADDR_AFN04F297_Ext+10//辽宁锦州、浙江电表对时参数
		//1byte BIN 对时开关 是否对电表自动对时及对时的方式
		//1byte BIN 对时周期 天
		//2byte 对时开始时间 1376.1-2013附录A.19
		//2byte 对时结束时间 1376.1-2013附录A.19
		//1byte BIN 允许对时次数 
		//1byte BIN 时钟误差阈值 秒
		#define ADDR_AFN04_UserExtEnd     ADDR_AFN04F298_Ext+8
		#else
			#if (USER/100)==12//用户标识:四川
#define ADDR_AFN04F152     ((ADDR_AFN04_BaseEnd+0xfff)&0xfffff000)//终端（SIM卡）手机号码p0
//手机号码	BCD	8
#define ADDR_AFN04F245     ADDR_AFN04F152+8//设置电表时间采集日期(由4字节的D0～D30按顺序对位表示每月1日～31日，置"1"为有效，置"0"为无效)//20161128四川扩展
//采集日期	BS32	4
#define ADDR_AFN0CF305     ADDR_AFN04F245+4//透明任务执行状态  0表示测量点任务未完成，1表示测量点任务已完成。默认状态为已完成。D0位代表测量点1。201703四川增加透明任务
//透明任务执行状态0=保留,1=有高于全部冻结的透传任务,2=有低于日冻结的透传任务,0xFE=清0,0xFF=完成

#define LEN_AFN0CF305     MaxRS485AddCarrierPn
#define ADDR_AFN04_UserExtEnd     ADDR_AFN0CF305+LEN_AFN0CF305//透明任务执行状态
			#else
				#if (USER/100)==13//黑龙江
#define ADDR_AFN04F47_Ext     ((ADDR_AFN04_BaseEnd+0xfff)&0xfffff000)//显示用购电量参数(总加组号)
//总加组号1
//4BYTE 购前剩余电量(费),数据格式03
//4BYTE 购后剩余电量(费),数据格式03
//...
//总加组号n
#define ADDR_AFN04_UserExtEnd     ADDR_AFN04F47_Ext+(8*MaxTotalAddGroup)
				#else
#define ADDR_AFN04_UserExtEnd     ADDR_AFN04_BaseEnd
				#endif
			#endif
		#endif
	#endif
#endif


//AFN04扩展
#define ADDR_AFN04Ext_START     ((ADDR_AFN04_UserExtEnd+0xfff)&0xfffff000)
#define ADDR_AFN04F104     ADDR_AFN04Ext_START//曲线冻结密度参数(测量点号)(同步const u8 CurveSetFnList[])
//测量点1
	//AFN0DF73 总加组有功功率曲线冻结密度参数 1
	//AFN0DF74 总加组无功功率曲线冻结密度参数 2
	//AFN0DF75 总加组有功电能量曲线冻结密度参数 3
	//AFN0DF76 总加组无功电能量曲线冻结密度参数 4
	//AFN0DF81 测量点有功功率曲线冻结密度参数 5
	//AFN0DF82 测量点A相有功功率曲线冻结密度参数 6
	//AFN0DF83 测量点B相有功功率曲线冻结密度参数 7
	//AFN0DF84 测量点C相有功功率曲线冻结密度参数 8
	//AFN0DF85 测量点无功功率曲线冻结密度参数 9
	//AFN0DF86 测量点A无功功率曲线冻结密度参数 10
	//AFN0DF87 测量点B无功功率曲线冻结密度参数 11
	//AFN0DF88 测量点C无功功率曲线冻结密度参数 12
	//AFN0DF89 测量点A相电压曲线冻结密度参数 13
	//AFN0DF90 测量点B相电压曲线冻结密度参数 14
	//AFN0DF91 测量点C相电压曲线冻结密度参数 15
	//AFN0DF92 测量点A相电流曲线冻结密度参数 16
	//AFN0DF93 测量点B相电流曲线冻结密度参数 17
	//AFN0DF94 测量点C相电流曲线冻结密度参数 18
	//AFN0DF95 测量点零序电流曲线冻结密度参数 19
	//AFN0DF97 测量点正向有功总电能量曲线冻结密度参数 20
	//AFN0DF98 测量点正向无功总电能量曲线冻结密度参数 21
	//AFN0DF99 测量点反向有功总电能量曲线冻结密度参数 22
	//AFN0DF100 测量点反向无功总电能量曲线冻结密度参数 23
	//AFN0DF101 测量点正向有功总电能示值冻结密度参数 24
	//AFN0DF102 测量点正向无功总电能示值冻结密度参数 25
	//AFN0DF103 测量点反向有功总电能示值冻结密度参数 26
	//AFN0DF104 测量点反向无功总电能示值冻结密度参数 27
	//AFN0DF105 测量点总功率因数冻结密度参数 28
	//AFN0DF106 测量点A相功率因数冻结密度参数 29
	//AFN0DF107 测量点B相功率因数冻结密度参数 30
	//AFN0DF108 测量点C相功率因数冻结密度参数 31
	//AFN0DF109 测量点电压相位角曲线冻结密度参数 32
	//AFN0DF110 测量点电流相位角曲线冻结密度参数 33
	//AFN0DF138 直流模拟量数据曲线冻结密度参数 34
	//AFN0DF145 测量点1象限无功总电能示值曲线冻结密度参数 35
	//AFN0DF146 测量点4象限无功总电能示值曲线冻结密度参数 36
	//AFN0DF147 测量点2象限无功总电能示值曲线冻结密度参数 37
	//AFN0DF148 测量点3象限无功总电能示值曲线冻结密度参数 38
//...
//测量点n
#define LEN_AFN04F104_PerPn     38
#define ADDR_AFN04F105     ADDR_AFN04F104+(LEN_AFN04F104_PerPn*AFN04F104MaxPn)//电能表数据分级归类参数(测量点号)
//测量点1
//数据分级类号BIN 1 (1-MaxMeterEventClass)
//...
//测量点n
#define ADDR_AFN04F106     ADDR_AFN04F105+(1*AFN04F105MaxPn)//电能表数据分级参数(数据分级类号)
//数据分级类号1
//第1 个电能表数据标识BIN 4 对应DL/T 645
//第1 个电能表数据等级BIN 1 (0表示删除)
//……
//第n 个电能表数据标识BIN 4 对应DL/T 645
//第n 个电能表数据等级BIN 1 (0表示删除)
//...
//数据分级类号N
//第1 个电能表数据标识BIN 4 对应DL/T 645
//第1 个电能表数据等级BIN 1 (0表示删除)
//……
//第n 个电能表数据标识BIN 4 对应DL/T 645
//第n 个电能表数据等级BIN 1 (0表示删除)
#define NUM_AFN04F106_PerC     96//每分级类存储的电能表数据标识个数(最大个数=LEN_DATABUFF/5)
#define ADDR_AFN04F107     ADDR_AFN04F106+((5*NUM_AFN04F106_PerC)*AFN04F106MaxPn)//电能表数据分级周期参数(数据等级号)
//数据等级号1
//周期数值BIN 1
//周期单位BIN 1
//..
//数据等级号N

	#if ((USER/100)==8)//用户标识：通用版辽宁
#define ADDR_AFN04Ext1_END     ((ADDR_AFN04F107+(2*AFN04F107MaxPn)+0xfff)&0xfffff000)

#define ADDR_AFN0DExt_Start     ADDR_AFN04Ext1_END
//3byte 日冻结类数据时标 Td_d 日月年
#define MaxDate_AFN0DF321F322     (31)//AFN0DF321、F322最大存储天数
#define ADDR_AFN0DF321_Ext     (ADDR_AFN0DExt_Start+3)//电表对时后误差情况统计数量P0
//2byte 时间超差 5min 测量点数量
//2byte 超过时间阈值未超差5min测量点数量 
#define ADDR_AFN0DF322_Ext     (ADDR_AFN0DF321_Ext+4)//电表对时后误差情况统计明细Pn
//3byte 测量点 n 误差时间
		#if (3+4+(MaxRS485AddCarrierPn*3))*MaxDate_AFN0DF321F322>0x00020000
			#error "(3+4+MaxRS485AddCarrierPn*3)*MaxDate_AFN0DF321F322>0x00020000"
		#endif
#define ADDR_AFN0DExt_END     (ADDR_AFN0DExt_Start+0x00020000)
#define ADDR_AFN04Ext_END     (ADDR_AFN0DExt_END)
	#else
#define ADDR_AFN04Ext_END     ((ADDR_AFN04F107+(2*AFN04F107MaxPn)+0xfff)&0xfffff000)
	#endif


#define ADDR_AFN04_End     ADDR_AFN04Ext_END








#endif



