
//5xx=上海系


#ifndef Terminal_H
#define Terminal_H




//版本-------------------------------------------------------------------------------------
#define I0_UpDataInit     0//3//I0软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化
#define I1_UpDataInit     0//3//I1软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化
#define I2_UpDataInit     0//3//I2软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化
#define E1_UpDataMeterInit     0//3//E1软件更新仪表初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化
#define E1_UpDataTerminalInit     0//3//E1软件更新终端初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化
//#define E2_UpDataInit     0//E2软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化
//#define E3_UpDataInit     0//E3软件更新初始化:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位,6=仅部份参数初始化


#define EnProject     1//0=否,1=是;软件更新时是否判别项目类型
#define EnHV     1//0=否,1=是;软件更新时是否判别硬件版本
#define EnUSER     0//1//0=否,1=是;软件更新时是否判别用户标识
#define EnSV     0//0=否,1=是;软件更新时是否判别软件版本号,当允许时只有>=当前版本号才有效
#define EnMeter     0//0=否,1=是;I1电能表软件更新时是否判别电表常数接线方式等电表规格

#define SVER     0x0002//0x0001//软件版本号:ZZ01
#define SVDATE     0x150906//0x150211//软件发行日期:年月日
#define HVER     0x200//硬件版本号:2.00
#define HVDATE     0x130301//硬件发行日期:年月日

//版本结束---------------------------------------------------------------------------------





#define LEN_DATABUFF     4096//读写数据缓冲长度,最小4096




//UART------------------------------------------------------------------------------------------
#define RS232PORT     0//RS232通信口号
#define GPRSPORT     1//GPRS通信口号
#define RS485_1PORT     2//RS485_1通信口号
#define RS485_2PORT     3//RS485-2通信口号
#define RS485_4PORT     4//RS485_4通信口号(载波)
#define RS485_3PORT     5//RS485_3通信口号
#define IRPORT     6//红外通信口号
#define TESAMPORT     7//终端ESAM通信口号
#define MESAMPORT     8//电表ESAM通信口号
#define CARDPORT     9//电表CPUCARD通信口号
#define ENETPORT     10//以太网通信口号
#define SMSPORT     11//短信通信口号
#define ACSAMPLEPORT     12//交流采样通信口号(虚拟)
#define EFTPCMDPORT     13//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define EFTPDATAPORT     14//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define GFTPCMDPORT     15//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define GFTPDATAPORT     16//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)

//UART0(RS232)
#define LEN_UART0IntRx     8192//UART0中断接收缓冲长度
#define LEN_UART0Ctrl     200//UART0控制数据长度
#define LEN_UART0Rx     8192//UART0接收缓冲长度
#define LEN_UART0Tx     2048//UART0发送缓冲长度
#define LEN_UART0FnDataBuff     0//UART0抄表Fn数据缓冲长度
//UART1(GPRS)
#define LEN_UART1IntATRx     (LEN_DATABUFF+1024)//UART1中断接收缓冲长度AT命令
#define LEN_UART1IntRx     (LEN_DATABUFF+1024)//UART1中断接收缓冲长度
#define LEN_UART1Ctrl     200//UART1控制数据长度
#define LEN_UART1Rx     8192//UART1接收缓冲长度
#define LEN_ATTx     64//AT发送缓冲长度
#define LEN_UART1Tx     2048//UART1发送缓冲长度
#define LEN_UART1FnDataBuff     0//UART1抄表Fn数据缓冲长度
//UART2(RS485-1)
#define LEN_UART2IntRx     8192//UART2中断接收缓冲长度
#define LEN_UART2Ctrl     200//UART2控制数据长度
#define LEN_UART2Rx     8192//UART2接收缓冲长度
#define LEN_UART2Tx     8192//UART2发送缓冲长度
#define LEN_UART2FnDataBuff     256//UART2抄表Fn数据缓冲长度
//UART3(RS485-2)
#define LEN_UART3IntRx     8192//UART3中断接收缓冲长度
#define LEN_UART3Ctrl     200//UART3控制数据长度
#define LEN_UART3Rx     8192//UART3接收缓冲长度
#define LEN_UART3Tx     8192//UART3发送缓冲长度
#define LEN_UART3FnDataBuff     256//UART3抄表Fn数据缓冲长度
//UART4(RS485-4)(载波)
#define LEN_UART4IntRx     256//UART4中断接收缓冲长度
#define LEN_UART4Ctrl     200//UART4控制数据长度
#define LEN_UART4Rx     2048//UART4接收缓冲长度
#define LEN_UART4Tx     256//UART4发送缓冲长度
#define LEN_UART4FnDataBuff     256//UART4抄表Fn数据缓冲长度
//UART5(RS485-3)
#define LEN_UART5IntRx     8192//UART5中断接收缓冲长度
#define LEN_UART5Ctrl     200//UART5控制数据长度
#define LEN_UART5Rx     8192//UART5接收缓冲长度
#define LEN_UART5Tx     8192//UART5发送缓冲长度
#define LEN_UART5FnDataBuff     256//UART5抄表Fn数据缓冲长度
//UART6(红外)
#define LEN_UART6IntRx     2048//UART6中断接收缓冲长度
#define LEN_UART6Ctrl     200//UART6控制数据长度
#define LEN_UART6Rx     8192//UART6接收缓冲长度
#define LEN_UART6Tx     2048//UART6发送缓冲长度
#define LEN_UART6FnDataBuff     0//UART6抄表Fn数据缓冲长度
//UART7(终端ESAM)
#define LEN_UART7IntRx     8//UART7中断接收缓冲长度
#define LEN_UART7Ctrl     200//UART7控制数据长度
#define LEN_UART7Rx     2048//UART7接收缓冲长度
#define LEN_UART7Tx     2048//UART7发送缓冲长度
#define LEN_UART7FnDataBuff     0//UART7抄表Fn数据缓冲长度
//UART8(电表ESAM)
#define LEN_UART8IntRx     8//UART8中断接收缓冲长度
#define LEN_UART8Ctrl     0//UART8控制数据长度
#define LEN_UART8Rx     0//UART8接收缓冲长度
#define LEN_UART8Tx     0//UART8发送缓冲长度
#define LEN_UART8FnDataBuff     0//UART8抄表Fn数据缓冲长度
//UART9(电表CPUCARD)
#define LEN_UART9IntRx     8//UART9中断接收缓冲长度
#define LEN_UART9Ctrl     0//UART9控制数据长度
#define LEN_UART9Rx     0//UART9接收缓冲长度
#define LEN_UART9Tx     0//UART9发送缓冲长度
#define LEN_UART9FnDataBuff     0//UART9抄表Fn数据缓冲长度
//UART10(Ethernet)
#define LEN_UART10IntRx     8192//UART10中断接收缓冲长度
#define LEN_UART10Ctrl     200//UART10控制数据长度
#define LEN_UART10Rx     8192//UART10接收缓冲长度
#define LEN_UART10Tx     2048//UART10发送缓冲长度
#define LEN_UART10FnDataBuff     0//UART10抄表Fn数据缓冲长度
//UART11(SMS)
#define LEN_UART11IntRx     1024//UART11中断接收缓冲长度
#define LEN_UART11Ctrl     200//UART11控制数据长度
#define LEN_UART11Rx     1024//UART11接收缓冲长度
#define LEN_UART11Tx     1024//UART11发送缓冲长度
#define LEN_UART11FnDataBuff     0//UART11抄表Fn数据缓冲长度
//UART12(虚拟口)
#define LEN_UART12IntRx     8//UART12中断接收缓冲长度
#define LEN_UART12Ctrl     200//UART12控制数据长度
#define LEN_UART12Rx     2048//UART12接收缓冲长度
#define LEN_UART12Tx     256//UART12发送缓冲长度
#define LEN_UART12FnDataBuff     256//UART12抄表Fn数据缓冲长度

//UART13(以太网FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART13IntRx     (6+1024+2)//UART13中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART13Ctrl     200//UART13控制数据长度
#define LEN_UART13Rx     0//UART13接收缓冲长度
#define LEN_UART13Tx     1024//UART13发送缓冲长度
#define LEN_UART13FnDataBuff     0//UART13抄表Fn数据缓冲长度
//UART14(以太网FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART14IntRx     (6+4096+2)//UART14中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART14Ctrl     200//UART14控制数据长度
#define LEN_UART14Rx     0//UART14接收缓冲长度
#define LEN_UART14Tx     2048//UART14发送缓冲长度
#define LEN_UART14FnDataBuff     0//UART14抄表Fn数据缓冲长度
//UART15(无线FTP命令,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART15IntRx     8//UART15中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART15Ctrl     0//UART15控制数据长度
#define LEN_UART15Rx     0//UART15接收缓冲长度
#define LEN_UART15Tx     0//UART15发送缓冲长度
#define LEN_UART15FnDataBuff     0//UART15抄表Fn数据缓冲长度
//UART16(无线FTP数据,使用ADDR_DATABUFF和ADDR_4KWRITE_BUFF共至少8K缓冲)
#define LEN_UART16IntRx     8//UART16中断接收缓冲长度(接收缓冲比W5200内接收缓冲至少多1,免回0)
#define LEN_UART16Ctrl     0//UART16控制数据长度
#define LEN_UART16Rx     0//UART16接收缓冲长度
#define LEN_UART16Tx     0//UART16发送缓冲长度
#define LEN_UART16FnDataBuff     0//UART16抄表Fn数据缓冲长度

#define SocketPortnMin     1025//本机端口号最小值
#define SocketPortnMax     65535//本机端口号最大值
//UARTEND---------------------------------------------------------------------------------------




//电能表----------------------------------------------------------------------------------------
#define MMaxTariff     4//费率数
#define MeterHarmonic     0//谐波计算0=没,1=有
#define MeterMaxLadder     6//阶梯数
#define MaxMonth     12//月冻结数
#define MaxMeterCurveCongeal     (96*10)//最大曲线点数,60的整数倍(搜索时跳越用)
#define MeterMaxDemandCalculate     1//最大需量计算：0=脉冲法,1=功率平均法

#define MeterCtrl     0//计费控制:0无控制,1仅远程,2本地和远程
#define MeterType     0//表计类型:0非预付费表,1电量型预付费表,2电费型预付费表
#define MeterSpec     3//电能表规格
//电能表规格
//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#if MeterSpec==0
//MeterSpec=0 0.5S 3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     6400//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400//电表有功常数imp/kWh
#define MeterReactiveConst     6400//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x6400//电表无功常数imp/kvarh
#define MeterUBhex     2200//参比电压xxx.x(V)
#define MeterUBbcd     0x2200//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==1
//MeterSpec=1 0.5S 3P3W 50Hz 6400imp/kWh 100V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     1//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     6400//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400//电表有功常数imp/kWh
#define MeterReactiveConst     6400//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x6400//电表无功常数imp/kvarh
#define MeterUBhex     1000//参比电压xxx.x(V)
#define MeterUBbcd     0x1000//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==2
//MeterSpec=2 0.5S 3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     0x05//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     577//参比电压xxx.x(V)
#define MeterUBbcd     0x577//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==3
//MeterSpec=3 1.0  3P4W 50Hz 6400imp/kWh 220V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     6400//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x6400//电表有功常数imp/kWh
#define MeterReactiveConst     6400//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x6400//电表无功常数imp/kvarh
#define MeterUBhex     2200//参比电压xxx.x(V)
#define MeterUBbcd     0x2200//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==4
//MeterSpec=4 1.0  3P3W 50Hz 20000imp/kWh 100V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     1//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     1000//参比电压xxx.x(V)
#define MeterUBbcd     0x1000//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif
#if MeterSpec==5
//MeterSpec=5 1.0  3P4W 50Hz 20000imp/kWh 57.7V 1.5(6)A
#define MeterClass     0x10//0x02=0.2s级,0x05=0.5s级,0x10=1.0级,0x20=2.0级
#define MeterPW     0//初始接线方式;0=三相四线,1=三相三线
#define MeterFreq     0//额定频率0=50Hz,1=60Hz
#define MeterActiveConst     20000//电表有功常数imp/kWh
#define MeterActiveConstbcd     0x20000//电表有功常数imp/kWh
#define MeterReactiveConst     20000//电表无功常数imp/kvarh
#define MeterReactiveConstbcd     0x20000//电表无功常数imp/kvarh
#define MeterUBhex     577//参比电压xxx.x(V)
#define MeterUBbcd     0x577//参比电压xxx.x(V)
#define MeterIBhex     150//参比电流xx.xx(A)
#define MeterIBbcd     0x150//参比电流xx.xx(A)
#define MeterIMaxhex     600//最大电流xx.xx(A)
#define MeterIMaxbcd     0x600//最大电流xx.xx(A)
#define MeterPStartupHex     (MeterUBhex*MeterIBhex*0.00066)//有功起动功率xxxxx.xxx(Watt)
#define MeterQStartupHex     (MeterUBhex*MeterIBhex*0.00066)//无功起动功率xxxxx.xxx(var)
#endif

//电能表结束------------------------------------------------------------------------------------





//终端------------------------------------------------------------------------------------------
#define TMaxTariff     4//终端最大费率数
#define DefaultTariff     1//无数据时的默认费率数:0=费率数为0,1=用AFN04F10等设置的费率数,2=最大费率TMaxTariff
#define USERTEST     0//0=正常功能,1=各省级用户电科院送检
#define UartDateTI     0//日冻结的召读时标0=该日的24时数据,1=该日的0时数据
#define UartMonthTI     0//月冻结的召读时标0=该月未的24时数据,1=该月初1日的0时数据
#define RMM_RS485_PassageZero     1//RS485通道抄表过0点;0=不暂停,1=暂停
#define RMM_StartHM     0x0010//起动抄表时分
#define RMM_EndHM     0x2359//结束抄表时分
#define RMM_CompCongealTime     1//读电能表冻结数据比较冻结时间:0=不比较,1=要比较
#define RMM_RS485_FailData_0xEE     0//RS485抄表失败数据填0xee;0=否,1=是
#define RMM_DateCongeal_0xEE     1//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
#define RMM_MonthCongeal_0xEE     1//2类数据月冻结没抄到电表月冻结数据前填0xff;0=否(用实时数据冻结),1=是
	#if RMM_CompCongealTime==1//读电能表冻结数据比较冻结时间:0=不比较,1=要比较
		#if RMM_DateCongeal_0xEE==0//2类数据日冻结没抄到电表日冻结数据前填0xff;0=否(用实时数据冻结),1=是
			#error RMM_CompCongealTime==1,RMM_DateCongeal_0xEE==0
		#endif
	#endif
#define RMM_MonthCongealUserDate1     1//2类数据当月冻结使用电表每月1日冻结的数据;0=否,1=是;
	#if(RMM_MonthCongealUserDate1==1)
		#if(RMM_MonthCongeal_0xEE==0)
			#error RMM_MonthCongealUserDate1==1,RMM_MonthCongeal_0xEE==0
		#endif
	#endif
	
#define MaxRMTask     8//最大抄表任务数(>=2)
#define PowerOnRMTask     1//电源开抄表任务;0=没有,1=有
#define DateRMTask        1//每日抄表任务;0=没有,1=有
#define WeekRMTask        1//每周抄表任务;0=没有,1=有(替代 周期抄读载波口07表的曲线)(载波修改为当抄表失败时为加快任务退出不抄后续数据项)
#define TimerRMTask        1//定时抄读停电记录任务;0=没有,1=有
#define HourTask     1//重点户曲线抄读(仅对无曲线冻结的单相表)
#define MeterEventRMTask     1//周期抄读电能表事件任务;0=没有,1=有

#define RMM_RS485     1//RS485通道抄表模式:0=循抄全部数据,1=按大小类号设置抄读数据
#define RMM_RS485_DateCongeal     1//RS485通道抄表抄日冻结;0=否,1=是
#define RMM_Carrier_DateCongeal     1//载波通道抄表抄日冻结;0=否,1=是
#define RMM_RS485_IntegralCongeal     2//RS485通道抄表,抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
#define RMM_Carrier_IntegralCongeal     2//载波通道抄表,抄电表上1-24整点或曲线冻结;0=否,1=上1-24整点,2=曲线
#define RMM_Carrier_CurveTime     60//载波通道读电表曲线的整冻结间隔时间分;15分或60分
#define RMM_RS485_Congeal     1//RS485通道抄表补抄上2-3日冻结;0=否,1=是
#define RMM_Carrier_Congeal     1//载波通道抄表补抄上2-3日冻结;0=否,1=是
#define RMM_RS485_AddMonth     0//RS485通道抄表补抄上2月冻结;0=否,1=是
#define RMM_Carrier_AddMonth     0//载波通道抄表补抄上2月冻结;0=否,1=是
	#if((RMM_RS485_AddMonth|RMM_Carrier_AddMonth)!=0)
		#if(RMM_MonthCongeal_0xEE==0)
			#error (RMM_RS485_AddMonth|RMM_Carrier_AddMonth)!=0,RMM_MonthCongeal_0xEE==0
		#endif
	#endif
#define RMDateCongealSour 0//抄表日冻结原始数据:0=当前实时值,1=上结算日冻结值

#define MaxRS485User     8//最大RS485抄表数(4的整数倍)
#define MaxKeyUser     10//最大集中抄表重点用户数
#define MaxCarrierUser     1492//最大载波抄表数(4的整数倍)
#define MaxRS485Pn     MaxRS485User//最大RS485抄表测量点数
#define MaxRS485AddKeyUserPn     (MaxRS485User+MaxKeyUser)//最大RS485抄表+重点用户数
#define MaxRS485AddCarrierPn     (MaxRS485User+MaxCarrierUser)//最大RS485抄表+载波抄表数
#define MaxDCPn     0//最大直流模拟量测量点数
#define MaxCPn     0//最大电容器补偿用测量点数
#define MaxCCR     0//最大控制电容器投切数
#define MaxTotalAddGroup     8//最大总加组数
#define MaxControlRound     0//4//最大控制轮次
#define MaxTimerTask     64//最大定时任务数
#define MaxTimerTaskDI     8//最大定时任务数据标识个数
#define MaxImpInPort     0//最大脉冲输入端口数(<=8)
#define MaxSwitchInPort     5//最大开关量输入端口数(<=8,开关量输入端口数+门接点)
#define MaxAnalogPort     6//最大电压/电流模拟量配置数量(电压电流输入口目前最大只有6)
#define MaxDiffGroup     8//最大有功总电能量差动组数量
#define MaxHourCongeal     4//最大小时冻结数据个数(密度)0=不冻结;4=15,30,45,0;2=30,0;1=0;12=5,10,15...0;60=1,2,3...0
#define MaxDateCongeal     62//31//最大日冻结数
#define MaxRMDateCongeal     12//31//最大抄表日冻结数
#define MaxMonthCongeal     12//最大月冻结数
#define CurveCongealTime    60//15//曲线冻结间隔时间(密度)0=不冻结;15=15,30,45,0;30=30,0;60=0;5=5,10,15...0;1=1,2,3...0
#define MaxCurveCongeal     (96*10)//最大曲线点数
#define MaxACSampleCurveCongeal     0//最大交采1分钟冻结曲线点数(可设为0表示不用)
#define MaxCurveCongeal_Ext     (288*11)//最大线电压曲线点数(上海扩展)
#define MaxERC     64//最大事件ERC号
#define MaxClassEventNum     15//最大每个分类事件记录条数
#define F246MaxPn     4//最大日冻结掉电记录数据测量点数
#define NewERC14     1//ERC14停电事件记录方法0=原标准,1=国网营销部2014.1.15通知
#define EventProject     1//事件记录采集方案:0=原能走台体标准,1=按新智能电能表及采集终端事件记录采集规则
#define EnMeterAutoAutoReport     0//允许电能表主动上报事件:0=不允许,1=允许
#define MaxMeterEventClass     8//最大电能表事件分级类号
#define MaxMeterEventLevel     4//最大电能表事件分级数

#define GPRSENETAutoRePort     1//GPRS和以太网口主动上报0=分开独立上报,1=合并上报
#define RS232AutoRePort     0//RS232口主动上报0=禁止,1=允许
#define WaitHeartberatACKMinTimer     30//心跳发出等待主站确认最小定时秒
#define WaitHeartberatACKMaxTimer     180//600//心跳发出等待主站确认最大定时秒
#define WaitHostACKMinTimer     5//主动上报等待主站确认最小定时秒
#define WaitHostACKMaxTimer     60//主动上报等待主站确认最大定时秒
#define RouterCheckTimerVal     (120*60)//载波路由器死机秒定时值
#define ReadDL645OverTimer     3000//DL645读表超时定时值ms
#define SMSEQ2Byte     1//0=没,1=有;短信传输通道的数据单元中2字节短信传输接口(标准376.1中有)
#define SM_CMGF     0//短信模式:0=PDU模式,1=文本模式
#define MulRealRead     0//多帧下发实时抄读或数据转发:0=每帧抄读响应,1=放弃前面帧只抄读最后帧不检查最后帧可能是心跳等的确认帧

#define SoftBug1     0//0=正常功能,1=376.1功能测试软件BUG.小时冻结密度
#define SoftBug3     1//抄表上月最大需量0=比较需量产生的月,1=不比较需量产生的月
#define SoftBug4     0//0=正常功能,1=电科院送检(鼎信东软模块自动搜表为送检模式)
#define RMM_Type     0//集中抄表类型:0=载波表,1=RS485表通过采集器,2=混合
#define ErrorAutoDisplay     0//0=允许,1=禁止;系统错误自动立即显示
#define TESAMSPI     0//TESAM的SPI接口,0=标准SPI0口,1=IO仿
#define AFN04F150Dn     1//AFN04F150测量点有效标志Dn位代表的测量点号;1号测量点0=Dn-1,1=Dn
#define ReadF129Above    1//召读1类F129以上数据:0=读原存储数据,1=实时读取后再上传
#define RMM_AFN0CF26     0//AFN0CF26断相数据抄表;0=抄电表断相记录数据,1=抄电表失压记录数据
#define Uart3761Check0xEE   0//0xEE数据检查:0=不检查;1=检查,若全0xEE返回否认
#define AFN0CF41F42F43F44     0//当日电能量AFN0CF41F42F43F44计算;0=示值实时计算,1=当前示值-当日0点冻结
#define AFN0CF45F46F47F48     0//当月电能量AFN0CF45F46F47F48计算;0=示值实时计算,1=当前示值-当月0点冻结
#define RS232DefaultBPS     (0xb+(6<<5))//RS232口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define RS485DefaultBPS     (0xb+(4<<5))//RS485口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define IRDefaultBPS     (0xb+(3<<5))//红外口默认的速率控制码0=300,1=600,2=1200,3=2400,4=4800,5=7200,6=9600,7=19200,8=38400,9=57600,10=115200,11=230400,12=...
#define DefaultClassNo     0x50//自动搜表默认的用户大类号小类号

#define Extern_Uart_AFN255     1//外部程序:0=I2,1=E1,2=E2,3=;工厂测试(扩展)
#define Extern_Terminal_ReadMeter_DL645_1997     1//外部程序:0=I2,1=E1,2=E2,3=;终端抄表DL645-1997
#define Extern_Terminal_ReadMeter_DL645_1997_Fn     1//外部程序:0=I2,1=E1,2=E2,3=;仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
#define Extern_BatchTask_RS485     1//外部程序:0=I2,1=E1,2=E2,3=;数据转发批量任务-RS485-时钟对时
#define Extern_BatchTask_3762     1//外部程序:0=I2,1=E1,2=E2,3=;数据转发批量任务-3762-时钟对时
#define Extern_W5200_FTP_RxTx     1//外部程序:0=I2,1=E1,2=E2,3=;W5200 FTP文件下载
#define Extern_GPRS_FTP_RxTx     1//外部程序:0=I2,1=E1,2=E2,3=;GPRS FTP文件下载
//#define Extern_CDMA_FTP_RxTx     1//外部程序:0=I2,1=E1,2=E2,3=;CDMA FTP文件下载
#define Extern_Uart_AFN04     1//外部程序:0=I2,1=E1,2=E2,3=;参数设置和查询
#define Extern_Uart_AFN05     1//外部程序:0=I2,1=E1,2=E2,3=;控制命令
#define Extern_Uart_AFN09     1//外部程序:0=I2,1=E1,2=E2,3=;请求终端配置
#define Extern_Terminal_ReadMeter_ShH     1//外部程序:0=I2,1=E1,2=E2,3=;终端抄表 上海表规约
#define Extern_Terminal_ReadMeter_ShH_Fn     1//外部程序:0=I2,1=E1,2=E2,3=;仅抄1个Fn数据填写到FnDataBuff;Task=0=空闲(本Fn抄读完成)
#define Extern_ReadMeter_Fn_YesNo     1//外部程序:0=I2,1=E1,按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)

//终端结束--------------------------------------------------------------------------------------








//硬件版本--------------------------------------------------------------------------------------
#if(HVER==0x100)//硬件版本号1.00
#define CPUCLK     96//CPU时钟cclk:12,24,48,96(MHz)
#define LCDPixels     3//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
#define IC_LPC177x8x     1//0=没,1=有
#define IC_LPC176x     0//0=没,1=有
#define IC_SST25VF64C     0//0=没,1=有
#define IC_MX25L128     0//0=没,1=有
#define IC_MX25L256     0//0=没,1=有
#define IC_SerialFlash     (IC_SST25VF64C||IC_MX25L128||IC_MX25L256)//0=没,1=有
#define IC_SST39VF6401B     1//0=没,1=有
#define IC_S29GL01G     0//0=没,1=有
#define IC_S29GL512     1//0=没,1=有
#define IC_S29GL256     0//0=没,1=有
#define IC_S29GL128     0//0=没,1=有
#define IC_S29GLxxx     (IC_S29GL01G||IC_S29GL512||IC_S29GL256||IC_S29GL128)
#define IC_IS61LV25616     0//0=没,1=有
#define IC_MT48LC4M16     1//0=没,1=有
#define IC_DS3231     1//0=没,1=有
#define IC_W5200     1//0=没,1=有
#define IC_90E36     1//0=没,1=有
#define IC_ATT7022E     0//0=没,1=有
#define IC_UC1698     1//0=没,1=有(点阵显示)
#define IC_PCF8562     0//0=没,1=有(段式显示)
#define IC_CPUCARD     0//0=没,1=有
#define IC_MESAM     0//0=没,1=有
#define IC_TESAM     1//0=没,1=有
#define IC_24AAxxx     0//0=没,1=有
#define LOADSWITCH     0//智能表项目的负荷开关0=内部,1=外部
#define IC_BGS2     1//GPRS模块BGS2;0=没,1=有
#define IC_GPRS3763     0//GPRS模块376.3标准;0=没,1=有
#define IC_MC323     0//CDMA华为MC323模块;0=没,1=有
#define KEY_SET     0//设置按键0=没,1=有
#endif
#if(HVER==0x200)//硬件版本号2.00
#define CPUCLK     96//CPU时钟cclk:12,24,48,96(MHz)
#define LCDPixels     3//点阵LCD像素 3=160160,2=320240,1=19264,0=SEGMODE
#define IC_LPC177x8x     1//0=没,1=有
#define IC_LPC176x     0//0=没,1=有
#define IC_SST25VF64C     0//0=没,1=有
#define IC_MX25L128     0//0=没,1=有
#define IC_MX25L256     0//0=没,1=有
#define IC_SerialFlash     (IC_SST25VF64C||IC_MX25L128||IC_MX25L256)//0=没,1=有
#define IC_SST39VF6401B     1//0=没,1=有
#define IC_S29GL01G     0//0=没,1=有
#define IC_S29GL512     1//0=没,1=有
#define IC_S29GL256     0//0=没,1=有
#define IC_S29GL128     0//0=没,1=有
#define IC_S29GLxxx     (IC_S29GL01G||IC_S29GL512||IC_S29GL256||IC_S29GL128)
#define IC_IS61LV25616     0//0=没,1=有
#define IC_MT48LC4M16     1//0=没,1=有
#define IC_DS3231     1//0=没,1=有
#define IC_W5200     1//0=没,1=有
#define IC_90E36     1//0=没,1=有
#define IC_ATT7022E     0//0=没,1=有
#define IC_UC1698     1//0=没,1=有(点阵显示)
#define IC_PCF8562     0//0=没,1=有(段式显示)
#define IC_CPUCARD     0//0=没,1=有
#define IC_MESAM     0//0=没,1=有
#define IC_TESAM     1//0=没,1=有
#define IC_24AAxxx     1//0=没,1=有
#define LOADSWITCH     0//智能表项目的负荷开关0=内部,1=外部
#define IC_BGS2     0//GPRS模块BGS2;0=没,1=有
#define IC_GPRS3763     1//GPRS模块376.3标准;0=没,1=有
#define IC_MC323     1//CDMA华为MC323模块;0=没,1=有
#define KEY_SET     0//设置按键0=没,1=有
#endif
//硬件版本结束----------------------------------------------------------------------------------










#endif
