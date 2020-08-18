
#ifndef IRAM_Meter_H
#define IRAM_Meter_H

typedef __packed struct
{
#define ADDR_I1Task      ADDR_Meter_Ram_Start
  u8 I1Task;//I1分时任务
  u8 I1SubTask1;//I1子分时任务1,用于计量计算:电压,电流,功率,...等
  u8 res1;
	u8 I1MonthTask;//结算日(月)冻结任务
#define ADDR_SSP1     (ADDR_I1Task+4)//SSP1,计量IDT90E36读写
  u8 EMUParaCount;//计量芯片参数初始化计数
  u8 EMUInit;//计量芯片初始化标志:0=关电源(放电),1=开电源,2=复位,3=复位解除,4=参数初始化,5=完成
  u8 SSP1ErrorCount;//SSP1通信错误计数
  u8 SSP1Sched;//SSP1任务调度:0=任务0,1=任务1
  u8 SSP1Task0;//SSP1任务0:0=空,0x1=等待读,0x2=读开始,0x3=读比较,0x4=读完成,0x5=读错误;0x81=等待写,0x82=写开始,0x83=写比较,0x84=写完成,0x85=写错误
  u8 SSP1Task1;//SSP1任务1:0=空,0x1=等待读,0x2=读开始,0x3=读比较,0x4=读完成,0x5=读错误;0x81=等待写,0x82=写开始,0x83=写比较,0x84=写完成,0x85=写错误
	u8 SSP1Task1Count;//SSP1任务1多个数据连续读计数
	u8 TxCount;//发送计数
	u16 SPI2TxData;//在接收中断中要连续发送的数据
	u16 res2;
#define ADDR_SSP1Data     ADDR_SSP1+12
  u16 SSP1Addr0;//SSP1读写数据地址0
  u16 SSP1Addr1;//SSP1读写数据地址1	
  u32 SSP1Data0;//SSP1读写数据0
  u32 SSP1Data1;//SSP1读写数据1
  u32 SSP1Data2;//SSP1读写数据2
#define ADDR_Measure     ADDR_SSP1Data+16//计量
	u8 PW;//接线方式;0=三相四线,1=三相三线
	u8 NOPLoad;//位为1表示空载;b2=有功C,b1=有功B,b0=有功A;(地址固定为ADDR_Measure+1,通信库中要用)
	u8 NOQLoad;//位为1表示空载;b2=无功C,b1=无功B,b0=无功A
	u8 NOSLoad;//位为1表示空载;b2=视在C,b1=视在B,b0=视在A
	u8 TQ;//总当前象限 1-4分别表示1-4象限,0=掉电或不确定
	u8 AQ;//A相当前象限 1-4分别表示1-4象限,0=掉电或不确定
	u8 BQ;//B相当前象限 1-4分别表示1-4象限,0=掉电或不确定
	u8 CQ;//C相当前象限 1-4分别表示1-4象限,0=掉电或不确定
	u8 DQ;//B0-B3无功需量运算用原当前象限1-4分别表示1-4象限,0=掉电或不确定(是TQ复制),B7=有功需量运算原方向0正1反
	u8 UARTFlags;//b7=设置键,b6=安全认证,b5=报警,b4=载波模块STA脚原状态寄存,b3=载波通信,b2=RS485-1通信,b1=RS485-2通信,b0=红外通信
#define ADDR_METER07_CTRL_SRAM     (ADDR_Measure+10)
	u8 Tariff;//1BYTE 电能表当前执行费率(HEX 费率1,2,3,4 分别对应尖峰,峰,平,谷等费率)
	u8 Ctrl1;//1BYTE 两套时区表切换状态0=1套,1=2套(同步FLASH中值)
	u8 Ctrl2;//1BYTE 两套日时段表切换状态0=1套,1=2套(同步FLASH中值)
	u8 Ctrl3;//1BYTE 两套梯度切换状态0=1套,1=2套(同步FLASH中值)
	u8 Ctrl4;//1BYTE 两套费率电价切换0=1套,1=2套(同步FLASH中值)
	u8 Ctrl5;//1BYTE 当前运行第n阶梯电价(HEX)
	u8 Ctrl6;//1BYTE 1=继电器测试卡插入
	u8 Ctrl7;//1BYTE 继电器原合跳闸状态,0=合闸,1=跳闸
	u8 Ctrl8;//1BYTE 继电器补跳次数计数器
	u8 Ctrl9;//1BYTE 声音报警允许禁止,0=允许,其他值禁止
#define ADDR_Meter_MS_Timer     ADDR_METER07_CTRL_SRAM+10//仪表10MS定时器
	u16 DisplayScanTimer;//1 显示扫描 每10MS减1定时器
	u16 PTrip_MS_Timer;//2 负荷开关跳闸脉冲10MS定时器
	u16 NTrip_MS_Timer;//3 负荷开关合闸脉冲10MS定时器
	u16 MT_MS_Timer;//4 复用端子脉冲输出10MS定时
	u16 LVTest_MS_Timer;//5 全失压检测打开90E36工作10MS定时器
	u16 Read90E36Timer;//6 循环读90E36测量数据每10MS减1定时器
#define Meter_MS_Timer_Num     6//仪表10MS定时器数
  
#define ADDR_Meter_S_Timer     (ADDR_Meter_MS_Timer+(2*Meter_MS_Timer_Num))//S减定时器
	u16 S_PR;//1 60秒分频产生分
	u16 DemandCYC_S_Timer;//2 需量周期秒定时器
	u16 SlipCYC_S_Timer;//3 需量滑差秒定时器
	u16 TariffCYC_S_Timer;//4 需量周期费率切换秒定时器
	u16 PowerCYC_S_Timer;//5 需量周期有功方向改变秒定时器
	u16 MeanPower_S_Timer;//6 一分钟有功总平均功率秒定时器
	u16 ALP_S_Timer;//7 A断相秒定时器
	u16 BLP_S_Timer;//8 B断相秒定时器
	u16 CLP_S_Timer;//9 C断相秒定时器
	u16 ALLLV_S_Timer;//10 全失压和掉电秒定时器
	u16 ALV_S_Timer;//11 A失压秒定时器
	u16 BLV_S_Timer;//12 B失压秒定时器
	u16 CLV_S_Timer;//13 C失压秒定时器
	u16 ALC_S_Timer;//14 A失流秒定时器
	u16 BLC_S_Timer;//15 B失流秒定时器
	u16 CLC_S_Timer;//16 C失流秒定时器
	u16 VIP_S_Timer;//17 电压逆相序秒定时器
	u16 IIP_S_Timer;//18 电流逆相序秒定时器
	u16 ALO_S_Timer;//19 A过载秒定时器
	u16 BLO_S_Timer;//20 B过载秒定时器
	u16 CLO_S_Timer;//21 C过载秒定时器
	u16 UNI_S_Timer;//22 电流不平衡秒定时器
	u16 UNN_S_Timer;//23 电流严重不平衡秒定时器
	u16 AVO_S_Timer;//24 A过压秒定时器
	u16 BVO_S_Timer;//25 B过压秒定时器
	u16 CVO_S_Timer;//26 C过压秒定时器
	u16 TFO_S_Timer;//27 总功率因数超限秒定时器
	u16 PDO_S_Timer;//28 正有功需量超限秒定时器
	u16 NDO_S_Timer;//29 反有功需量超限秒定时器
	u16 ReTrip_S_Timer;//30 负荷开关补跳秒定时器
	u16 ESAMPAY_S_Timer;//31 ESAM钱包付款秒定时器
	u16 ExitTrip_S_Timer;//32 外部负荷开关预跳闸保持时间秒定时器
	u16 TripAttr_S_Timer;//33 跳闸前告警延时秒定时器
  u16 DisplayLoopTimer;//34 循显秒定时器
  u16 BackLightTimer;//35 背光灯显示秒定时器
	u16 UNU_S_Timer;//36 电压不平衡秒定时器
	
#define Meter_S_Timer_Num     36//秒减定时器数
#define ADDR_Meter_SADD_Timer     ADDR_Meter_S_Timer+(2*Meter_S_Timer_Num)//S秒加定时器
	u16 AH_S_Timer;//安时计算用秒加计数
	u16 GP_SAdd_Timer;//非中断程序用秒加计数
	
#define Meter_SADD_Timer_Num     2//秒加定时器数
	
	
#define ADDR_Meter_T_Flags     ADDR_Meter_SADD_Timer+(2*Meter_SADD_Timer_Num)
	u8 TFlags;//秒分时间更新标志
			//B0=秒更新标志,用于RTC实时费率控制(值0表示更新,使用后置1)
			//B1=分更新标志,用于FM分定时器(值1表示更新,使用后置0)(当前不用此标志计时)
			//B2=LCD闪动控制(秒更新取反)
			//...
			//B7=
	u8 res3;
	u16 res4;
#define ADDR_EventFlags     ADDR_Meter_T_Flags+4//事件标志
	u8 LPFlags;//b0-b2分别为ABC断相瞬时,b3=,b4-b6分别为ABC断相确定,b7=
	u8 LPFlagsOld;//
	u8 LVFlags;//b0-b2分别为ABC失压瞬时,b3=,b4-b6分别为ABC失压确定,b7=
	u8 LVFlagsOld;//
	u8 LIFlags;//b0-b2分别为ABC失流瞬时,b3=,b4-b6分别为ABC失流确定,b7=
	u8 LIFlagsOld;//
	u8 AllLVFlags;//b0=全失压瞬时,b1=掉电瞬时;b3=全失压确定,b4=掉电确定
	u8 AllLVFlagsOld;//
	u8 IPFlags;//b7=电压逆相序,b6=电流逆相序,b5=电压逆相序确定,b4=电流逆相序确定,b3=电压逆相序瞬时,b2=电流逆相序瞬时,b1=, b0=
	u8 IPFlagsOld;//
	u8 LOFlags;//b0-b2分别为ABC过载瞬时,b3=,b4-b6分别为ABC过载确定,b7=
	u8 LOFlagsOld;//
	u8 UNFlags;//b0=电流不平衡瞬时b1=电流严重不平衡瞬时b2=电压不平衡瞬时;b4=电流不平衡确定b5=电流严重不平衡确定b6=电压不平衡确定
	u8 UNFlagsOld;//
	u8 VOFlags;//b0-b2分别为ABC过压瞬时,b3=,b4-b6分别为ABC过压确定,b7=
	u8 VOFlagsOld;//
	u8 FOFlags;///b0=总功率因数超限瞬时, b4=总功率因数超限确定
	u8 FOFlagsOld;//
	u8 DOFlags;//b0=正有功需量超限瞬时,b1=反有功需量超限瞬时, b4=有功需量超限确定,B5=反有功需量超限确定
	u8 DOFlagsOld;//
	u8 DIRFlags;//b0-b2分别为ABC潮流反向瞬时,b3=,b4-b6分别为ABC潮流反向确定,b7=
	u8 DIRFlagsOld;//
#define ADDR_SEGLCDCtrl     ADDR_EventFlags+22//循显控制(字对齐)
	u8 LcdLoop;// 0 循显计数
	u8 LcdRound;// 1 循显轮回计数
	u8 LcdNext;// 2 B0=NEXTI按下,B1=NEXTD按下,B2=原键显,B3-B7保留
	u8 reserve1;// 3 
	u8 LcdErr1;// 4 ERR1电表故障类异常循显计数
	u8 LcdErr2;// 5 ERR2事件类异常循显计数
	u8 LcdErr3;//	6 ERR3 IC卡异常循显计数
	u8 ErrFlags;// 7 电表故障标志;B7=,B6=时钟,B5=存储器,B4=内部程序,B3=时钟电池,B2=内卡初始化,B1=ESAM,B0=控制回路,
	u8 reserve2;// 8 
	u8 CardErr1;// 9 IC卡异常标志;B7=未按铅封键,B6=修改密钥错误,B5=现场参数设置卡对本表已经失效,B4=购电超囤积,B3=充值次数错误,B2=客户编号不匹配,B1=ESAM验证失败,B0=认证错误
	u8 CardErr2;// 10 IC卡异常标志;B7=卡文件格式不合法,B6=表计电压过低,B5=卡损坏或不明类型卡,B4=表计未开户,B3=表计已开户,B2=修改密钥卡次数为0,B1=修改表号卡满,B0=提前拔卡
	u8 CardErr3;// 11 IC卡异常标志;B7=本屏正在显IC卡异常，B6-B3=保留,B2=其他错误,B1=已经开过户的新开户卡,B0=卡类型错
	u8 ButtonVal;// 12 按键状态(使用后清0);
	u8 ButtonKeep;// 13 按下保持3秒状态(使用后清0)
	u8 LcdCard;// 14 B7-B0='空''读卡''中''成功''失败'请购电''透支''拉闸' 
	u8 LcdCardFlash;// 15 闪动B7-B0='空''读卡''中''成功''失败'请购电''透支''拉闸' 
#define ADDR_04000501     ADDR_SEGLCDCtrl+16//电表运行状态字,要求字对齐
	u16 RunFlag1;//2BYTE 状态字1	B7=保留,B6=保留,B5=无功方向,B4=有功方向,B3=抄表电池,B2=时钟电池,B1=需量积算方式,B0=保留
			// B15-B8=保留
	u16 RunFlag2;//2BYTE 状态字2 B7=保留,B6=C无功方向,B5=B无功方向,B4=A无功方向,B3=保留,B2=C有功方向,B1=B有功方向,B0=A有功方向
			// B15-B8=保留
	u16 RunFlag3;//2BYTE 状态字3 B7=预跳闸报警,B6=继电器命令,B5=当前运行时区,B4=继电器状态,B3=编程允许,B2B1=供电方式,B0=当前运行时段套
			// B15-B12=保留,B11=当前阶梯套,B10=当前运行费率电价套,B9B8=电能表类型
	u16 RunFlag4;//2BYTE 状态字4(A相故障状态) B7=断相,B6=潮流反向,B5=过载,B4=过流,B3=失流,B2=过压,B1=欠压,B0=失压
			//(A相故障状态)B15-B9=保留,B8=断流
	u16 RunFlag5;//2BYTE 状态字5(B相故障状态) B7=断相,B6=潮流反向,B5=过载,B4=过流,B3=失流,B2=过压,B1=欠压,B0=失压
			//(B相故障状态)B15-B9=保留,B8=断流
	u16 RunFlag6;//2BYTE 状态字6(C相故障状态) B7=断相,B6=潮流反向,B5=过载,B4=过流,B3=失流,B2=过压,B1=欠压,B0=失压
			//(C相故障状态)B15-B9=保留,B8=断流
	u16 RunFlag7;//2BYTE 状态字7(合相故障状态) B7=总功率因数超下限,B6=潮流反向,B5=过载,B4=过流,B3=失流,B2=过压,B1=欠压,B0=失压
			//(合相故障状态)B15-B9=保留,B8=电流严重不平衡
	
	#if MeterMaxDemandCalculate==0//最大需量计算：0=脉冲法,1=功率平均法
	//脉冲法最大需量计算
#define ADDR_PARKW_IMP     (ADDR_04000501+14)//一分钟有功总平均功率用脉冲
	u16 MeanImp;
	u8 DemandCYC;//当前计算用需量周期
	u8 SlipCYC;//当前计算用滑差时间
#define ADDR_MAXKW_IMP     (ADDR_PARKW_IMP+4)//有功需量脉冲
	u16 ActiveImpShift[60];//120BYTE
	u16 ActiveImp;//2BYTE 0.01CF计数
#define ADDR_MAXKVAR_IMP     (ADDR_MAXKW_IMP+120+2)//无功需量脉冲
	u16 ReactiveImpShift[60];//120BYTE
	u16 ReactiveImp;//2BYTE 0.01CF计数
#define ADDR_FactoryCal     ADDR_MAXKVAR_IMP+122
	#else
	//功率平均法最大需量计算
#define ADDR_PARKW_IMP     (ADDR_04000501+14)//一分钟有功总平均功率用脉冲
	u8 DemandCYC;//当前计算用需量周期
	u8 SlipCYC;//当前计算用滑差时间
	u8 kWParCount;//hex平均次数计数
	u8 res5;
	u32 kWParAdd;//4byte hex有功功率平均加
#define ADDR_MAXKW_PAR     (ADDR_PARKW_IMP+8)//有功需量平均
	u8 ActiveParShift[3*60];//180BYTE hex
	u32 ActiveParAdd;//4byte hex平均加
	u16 ActiveParCount;//2BYTE hex平均次数计数
#define ADDR_MAXKVAR_PAR     (ADDR_MAXKW_PAR+180+6)//无功需量平均
	u8 ReactiveParShift[3*60];//180BYTE hex
	u16 ReactiveParCount;//2BYTE hex平均次数计数
	u32 ReactiveParAdd;//4byte hex平均加
#define ADDR_FactoryCal     ADDR_MAXKVAR_PAR+180+6
	#endif
	u32 CS;//校表数据校验和


}Meter_Ram_TypeDef;
#define Meter_Ram     ((Meter_Ram_TypeDef *)(ADDR_Meter_Ram_Start))

#define ADDR_07LIB_REG     (((ADDR_FactoryCal+4)+3)&0xfffffffc)//仪表07库相关数据寄存(字对齐)
//4BYTE 0 通信标识
//1BYTE 1.0 显示用扩展标识
//1BYTE 1.1 B7-B4保留,B3=液晶查看标志,B2=1显示需秒刷新标志,B1=进入菜单标志,B0=1显示DI更新标志
//1BYTE 1.2 B7-B1保留,B0=1无标识数据(如没设置的冻结项)
//1BYTE 1.3
//4BYTE 2 内部数据编号
//4BYTE 3 通信标识库指针(结构数组号)
//4BYTE 4 数据开始地址
//4BYTE 5 数据指针库指针(结构数组号)
//4BYTE 6 LCD库指针(结构数组号)
//1BYTE 7.0 上月号
//1BYTE 7.1 费率号
//1BYTE 7.2 电量显示小数位数
//1BYTE 7.3 功率显示小数位数
#define ADDR_LCDBUFF1     (ADDR_07LIB_REG+32)//段式LCD显示缓冲区1(字对齐)
//8BYTE 0 字符数据N0-N7(小端对齐)
//1BYTE 8 B7-B1=数据小数点P7-P1,B0=空
//2BYTE 9 字符数据n0-n1(小端对齐)
//1BYTE 11 B7-B0='当前''上''月''组合''反''正''向''无'
//1BYTE 12 B7-B0='有''功''Ⅰ''Ⅰ''Ⅰ''Ⅴ''总''尖'
//1BYTE 13 B7-B0='峰''平''谷''Ａ''Ｂ''Ｃ''Ｎ''COS'
//1BYTE 14 B7-B0='Φ''阶梯''剩余'需''电''量''费''价'
//1BYTE 15 B7-B0='空''失''压''流''功率''时''间''段'
//1BYTE 16 B7-B0='△1''△2''万''元''Ｋ''V''V''Ａ'
//1BYTE 17 B7-B0='ｈ''ｋ''var''ｈ''囤积''时点''分点''正负号'

//1BYTE 18 闪动B7-B0=数7-数0
//1BYTE 19 闪动B7-B1=小数点P7-P1,B0=保留
//1BYTE 20 闪动B7-B2=保留,B1-B0=月数1-月数0
//1BYTE 21 闪动B7-B0='当前''上''月''组合''反''正''向''无'
//1BYTE 22 闪动B-B0='有''功''Ⅰ''Ⅰ''Ⅰ''Ⅴ''总''尖'
//1BYTE 23 闪动B7-B0='峰''平''谷''Ａ''Ｂ''Ｃ''Ｎ''COS'
//1BYTE 24 闪动B7-B0='Φ''阶梯''剩余'需''电''量''费''价'
//1BYTE 25 闪动B7-B0='空''失''压''流''功率''时''间''段'
//1BYTE 26 闪动B7-B0='△1''△2''万''元''Ｋ''V''V''Ａ'
//1BYTE 27 闪动B7-B0='ｈ''ｋ''var''ｈ''囤积''时点''分点''正负号'
//4BYTE 保留
#define ADDR_LCDBUFF2     (ADDR_LCDBUFF1+32)//段式LCD显示缓冲区2(字对齐)
//8BYTE 0 字符数据N0-N7(小端对齐)
//1BYTE 8 B7-B1=数据小数点P7-P1,B0=空
//2BYTE 9 字符数据n0-n1(小端对齐)
//1BYTE 11 B7-B0='●Ⅰ''●Ⅱ''●Ⅲ''●Ⅳ''□1''□2''□3''□4'
//1BYTE 12 B7-B0='○尖''○峰''○平''○谷''Ua''Ub''Uc''逆相序'
//1BYTE 13 B7-B0='-''Ia''-''Ib''-''Ic''○1''○2'
//1BYTE 14 B7-B0='电池X1''电池X2''天线''信号1''信号2''信号3''信号4''载波'
//1BYTE 15 B7-B0='座标''话筒''通信1''通信2''开关''锁合''家''铃'
//1BYTE 16 B7-B0='空''读卡''中''成功''失败'请购电''透支''拉闸'
//1BYTE 17 保留

//1BYTE 18 闪动B7-B0=数7-数0
//1BYTE 19 闪动B7-B1=小数点P7-P1,B0=保留
//1BYTE 20 闪动B7-B2=保留,B1-B0=月数1-月数0
//1BYTE 21 闪动B7-B0='Ⅰ''Ⅱ''Ⅲ''Ⅳ''□1''□2''□3''□4'
//1BYTE 22 闪动B7-B0='○尖''○峰''○平''○谷''Ua''Ub''Uc''逆相序'
//1BYTE 23 闪动B7-B0='-''Ia''-''Ib''-''Ic''○1''○2'
//1BYTE 24 闪动B7-B0='电池X1''电池X2''天线''信号1''信号2''信号3''信号4''载波'
//1BYTE 25 闪动B7-B0='座标''话筒''通信1''通信2''开关''锁合''家''铃'
//1BYTE 26 闪动B7-B0='空''读卡''中''成功''失败'请购电''透支''拉闸'
//1BYTE 27 保留
//1BYTE 28 保留
//3BYTE 保留

#define ADDR_MT     (ADDR_LCDBUFF2+32)
//1BYTE 复用端子;0=时钟秒脉冲,1=需量周期,2=时段投切
#define ADDR_04001301     ADDR_MT+1
//1BYTE 无线通信在线及信号强弱指示
#define ADDR_FREQUENCY     ADDR_04001301+1//电网频率
//2BYTE_BCD 2位小数
#define ADDR_PARPT     ADDR_FREQUENCY+2//一分钟有功总平均功率
//3BYTE_BCD 4位小数
#define ADDR_PMAX     ADDR_PARPT+3//当前有功需量
//3BYTE_BCD 4位小数
#define ADDR_QMAX     ADDR_PMAX+3//当前无功需量
//3BYTE_BCD 4位小数
#define ADDR_SMAX     ADDR_QMAX+3//当前视在需量
//3BYTE_BCD 4位小数
#define ADDR_IBPH     ADDR_SMAX+3//电流不平衡率
//3BYTE_BCD 2位小数
#define ADDR_UNPH     ADDR_IBPH+3//电压不平衡率
//3BYTE_BCD 2位小数
#define ADDR_0280000B     ADDR_UNPH+3//当前阶梯电价
//4BYTE BCD 4位小数(元/kWh)
#define ADDR_UA     ADDR_0280000B+4//A电压
//4BYTE_BCD 5位小数
#define ADDR_UB     ADDR_UA+4//B电压
//4BYTE_BCD 5位小数
#define ADDR_UC     ADDR_UB+4//C电压
//4BYTE_BCD 5位小数
#define ADDR_IA     ADDR_UC+4//A电流
//4BYTE_SBCD 5位小数
#define ADDR_IB     ADDR_IA+4//B电流
//4BYTE_SBCD 5位小数
#define ADDR_IC     ADDR_IB+4//C电流
//4BYTE_SBCD 5位小数
#define ADDR_IN     ADDR_IC+4//零线电流
//4BYTE_SBCD 5位小数
#define ADDR_PT     ADDR_IN+4//总有功功率
//4BYTE_SBCD 6位小数
#define ADDR_PA     ADDR_PT+4//A有功功率
//4BYTE_SBCD 6位小数
#define ADDR_PB     ADDR_PA+4//B有功功率
//4BYTE_SBCD 6位小数
#define ADDR_PC     ADDR_PB+4//C有功功率
//4BYTE_SBCD 6位小数
#define ADDR_QT     ADDR_PC+4//总无功功率
//4BYTE_SBCD 6位小数
#define ADDR_QA     ADDR_QT+4//A无功功率
//4BYTE_SBCD 6位小数
#define ADDR_QB     ADDR_QA+4//B无功功率
//4BYTE_SBCD 6位小数
#define ADDR_QC     ADDR_QB+4//C无功功率
//4BYTE_SBCD 6位小数
#define ADDR_ST     ADDR_QC+4//总视在功率
//4BYTE_SBCD 6位小数
#define ADDR_SA     ADDR_ST+4//A视在功率
//4BYTE_SBCD 6位小数
#define ADDR_SB     ADDR_SA+4//B视在功率
//4BYTE_SBCD 6位小数
#define ADDR_SC     ADDR_SB+4//C视在功率
//4BYTE_SBCD 6位小数
#define ADDR_COST     ADDR_SC+4//总功率因数
//2BYTE_SBCD 3位小数
#define ADDR_COSA     ADDR_COST+2//A功率因数
//2BYTE_SBCD 3位小数
#define ADDR_COSB     ADDR_COSA+2//B功率因数
//2BYTE_SBCD 3位小数
#define ADDR_COSC     ADDR_COSB+2//C功率因数
//2BYTE_SBCD 3位小数
#define ADDR_ANGLE_UIA     ADDR_COSC+2//A电压电流夹角
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_UIB     ADDR_ANGLE_UIA+2//B电压电流夹角
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_UIC     ADDR_ANGLE_UIB+2//C电压电流夹角
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_UA     ADDR_ANGLE_UIC+2//UA角度(总是0)
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_UB     ADDR_ANGLE_UA+2//UB角度(对UA)
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_UC     ADDR_ANGLE_UB+2//UC角度(对UA)
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_IA     ADDR_ANGLE_UC+2//IA角度(对UA)
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_IB     ADDR_ANGLE_IA+2//IB角度(对UA)
//2BYTE_BCD 1位小数
#define ADDR_ANGLE_IC     ADDR_ANGLE_IB+2//IC角度(对UA)
//2BYTE_BCD 1位小数

#define ADDR_UAA     ADDR_ANGLE_IC+2//A电压总畸变
//2BYTE 00.00%
#define ADDR_UBA     ADDR_UAA+2//B电压总畸变
//2BYTE 00.00%
#define ADDR_UCA     ADDR_UBA+2//C电压总畸变
//2BYTE 00.00%
#define ADDR_IAA     ADDR_UCA+2//A电流总畸变
//2BYTE 00.00%
#define ADDR_IBA     ADDR_IAA+2//B电流总畸变
//2BYTE 00.00%
#define ADDR_ICA     ADDR_IBA+2//C电流总畸变
//2BYTE 00.00%
#define ADDR_FFT_UA     ADDR_ICA+2//UA谐波含量
//2BYTE 0次
//2BYTE 1次
//...
//2BYTE 21次
#define ADDR_FFT_UB     ADDR_FFT_UA+(2*22)//UB谐波含量
//2BYTE 0次
//2BYTE 1次
//...
//2BYTE 21次
#define ADDR_FFT_UC     ADDR_FFT_UB+(2*22)//UC谐波含量
//2BYTE 0次
//2BYTE 1次
//...
//2BYTE 21次
#define ADDR_FFT_IA     ADDR_FFT_UC+(2*22)//IA谐波含量
//2BYTE 0次
//2BYTE 1次
//...
//2BYTE 21次
#define ADDR_FFT_IB     ADDR_FFT_IA+(2*22)//IB谐波含量
//2BYTE 0次
//2BYTE 1次
//...
//2BYTE 21次
#define ADDR_FFT_IC     ADDR_FFT_IB+(2*22)//IC谐波含量
//2BYTE 0次
//2BYTE 1次
//...
//2BYTE 21次
#define ADDR_DFT_SCALE     ADDR_FFT_IC+(2*22)//DFT信号输入GAIN
//2BYTE 
#define ADDR_UA_FUND     ADDR_DFT_SCALE+2//A电压基波
//3BYTE_BCD 3位小数
#define ADDR_UB_FUND     ADDR_UA_FUND+3//B电压基波
//3BYTE_BCD 3位小数
#define ADDR_UC_FUND     ADDR_UB_FUND+3//C电压基波
//3BYTE_BCD 3位小数
#define ADDR_IA_FUND     ADDR_UC_FUND+3//A电流基波
//3BYTE_BCD 3位小数(无符号)
#define ADDR_IB_FUND     ADDR_IA_FUND+3//B电流基波
//3BYTE_BCD 3位小数(无符号)
#define ADDR_IC_FUND     ADDR_IB_FUND+3//C电流基波
//3BYTE_BCD 3位小数(无符号)

#define ADDR_02800020     ADDR_IC_FUND+3//当前电价
//4BYTE_BCD XXXX.XXXX元/kWh
#define ADDR_02800021     ADDR_02800020+4//当前费率电价
//4BYTE_BCD XXXX.XXXX元/kWh
#define ADDR_02800022     ADDR_02800021+4//身份认证时效剩余时间
//1BYTE_HEX秒分频
//2BYTE_BCD 分
#define ADDR_02800023     ADDR_02800022+3//红外认证时效剩余时间
//1BYTE_HEX秒分频
//1BYTE_BCD 分
#define ADDR_04001502     ADDR_02800023+2//插卡状态字(读取和上电后置0)
//2BYTE   
#define ADDR_04040300     ADDR_04001502+2//液晶查看
//5BYTE NNNNNNNN,NN(FFFFFFFF,FF时,表示全屏显示)
#define ADDR_LUAB     ADDR_04040300+5//线电压UAB
//2BYTE_BCD 1位小数
#define ADDR_LUBC     ADDR_LUAB+2//线电压UBC
//2BYTE_BCD 1位小数
#define ADDR_LUCA     ADDR_LUBC+2//线电压UCA
//2BYTE_BCD 1位小数


#define ADDR_Meter_Ram_End     (ADDR_LUCA+2)


#endif


