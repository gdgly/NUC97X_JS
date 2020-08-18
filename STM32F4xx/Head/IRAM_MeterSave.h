
#ifndef IRAM_MeterSave_H
#define IRAM_MeterSave_H


//电表电能类当前数据--------------------------------------------------------------------------
#define ADDR_MeterEnergy_IRAM_Start     ADDR_Meter_SaveRam_Start//仪表电能类数据在IRAM开始地址
#define ADDR_MFM_YMDHMS     ADDR_MeterEnergy_IRAM_Start//掉电前时钟
//秒,分,时,日,月,年
#define ADDR_BATTERY_TIMER     ADDR_MFM_YMDHMS+6//电池工作计时累计(分)
//4BYTE BCD分
#define ADDR_MeterFMTimer0     ADDR_BATTERY_TIMER+4//分定时器
//6BYTE 上次计算的秒分时日月年
//2BYTE 秒加计数计算分
#define ADDR_MeterFMTimer     ADDR_MeterFMTimer0+8//分减定时器(紧接上不分开)
//2BYTE  设置键按下分定时器hex
//2BYTE  智能表2级密码闭锁分定时器hex
//2BYTE  智能表4级密码闭锁分定时器hex
//2BYTE  
//2BYTE  
//2BYTE  
//2BYTE  
//2BYTE  工厂模式有效分定时器hex
#define MeterFMTimer_Num     8//分定时器数
#define ADDR_AH     ADDR_MeterFMTimer+(2*MeterFMTimer_Num)//安时
//4BYTE HEX A相000000.00安时
//4BYTE HEX B相000000.00安时
//4BYTE HEX C相000000.00安时
//2BYTE HEX A相毫安秒数,1安时=1000*3600毫安秒,0.01安时=36000毫安秒
//2BYTE HEX B相毫安秒数,1安时=1000*3600毫安秒,0.01安时=36000毫安秒
//2BYTE HEX C相毫安秒数,1安时=1000*3600毫安秒,0.01安时=36000毫安秒
#define ADDR_00000000     ADDR_AH+18//当前组合有功电能
//5*(1+M)BYTE BCD NNNNNN.NNNN 当前组合有功总,费率1-M电能
#define ADDR_00010000     ADDR_00000000+(5*(1+MMaxTariff))//正向有功电能
//5*(1+M)BYTE BCD NNNNNN.NNNN总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_00020000     ADDR_00010000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//反向有功电能
//5*(1+M)BYTE BCD NNNNNN.NNNN总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_00030000     ADDR_00020000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//组合无功1电能
//5*(1+M)BYTE BCD NNNNNN.NNNN 当前组合无功总,费率1-M电能
#define ADDR_00040000     ADDR_00030000+(5*(1+MMaxTariff))//组合无功2电能
//5*(1+M)BYTE BCD NNNNNN.NNNN 当前组合无功总,费率1-M分频
#define ADDR_00050000     ADDR_00040000+(5*(1+MMaxTariff))//第1象限无功电能
//5*(1+M)BYTE BCD NNNNNN.NNNN 总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_00060000     ADDR_00050000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//第2象限无功电能
//5*(1+M)BYTE BCD NNNNNN.NNNN总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_00070000     ADDR_00060000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//第3象限无功电能
//5*(1+M)BYTE BCD NNNNNN.NNNN总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_00080000     ADDR_00070000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//第4象限无功电能
//5*(1+M)BYTE BCD NNNNNN.NNNN总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_00090000     ADDR_00080000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//正向视在电能
//5*(1+M)BYTE BCD NNNNNN.NNNN总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_000A0000     ADDR_00090000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//反向视在电能
//5*(1+M)BYTE BCD NNNNNN.NNNN总,费率1-M电能
//1*(1+M)BYTE HEX 总,费率1-M分频
//4BYTE CRC32
#define ADDR_000C0000     ADDR_000A0000+((5*(1+MMaxTariff))+(1*(1+MMaxTariff))+4)//当前月度组合有功总累计用电量
//4BYTE BCD NNNNNN.NN
#define ADDR_000D0000     ADDR_000C0000+4//当前年结算周期组合有功总累计用电量
//4BYTE BCD YMDH当前日期时间年月日时
//4BYTE BCD NNNNNN.NN当前年结算周期组合有功总累计用电量

#define ADDR_00800000     ADDR_000D0000+8//关联等总电能
//5BYTE BCD  NNNNNN.NNNN 关联总电能
//5BYTE BCD  NNNNNN.NNNN 正有功基波总电能
//5BYTE BCD  NNNNNN.NNNN 反有功基波总电能
//5BYTE BCD  NNNNNN.NNNN 正有功谐波总电能
//5BYTE BCD  NNNNNN.NNNN 反有功谐波总电能
//5BYTE BCD  NNNNNN.NNNN 铜损有功电能总电能
//5BYTE BCD  NNNNNN.NNNN 铁损有功电能总电能

//1BYTE HEX 关联电能分频
//1BYTE HEX 正有功基波电能分频
//1BYTE HEX 反有功基波电能分频
//1BYTE HEX 正有功谐波电能分频
//1BYTE HEX 反有功谐波电能分频
//1BYTE HEX 铜损有功电能分频
//1BYTE HEX 铁损有功电能分频
//4BYTE CRC32
#define ADDR_00150000     ADDR_00800000+35+7+4//A电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功电能
//5BYTE BCD  NNNNNN.NNNN 相组合无功 1 电能
//5BYTE BCD  NNNNNN.NNNN 相组合无功 2 电能
//5BYTE BCD  NNNNNN.NNNN 相第一象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第二象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第三象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第四象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相正向视在电能
//5BYTE BCD  NNNNNN.NNNN 相反向视在电能
//5BYTE BCD  NNNNNN.NNNN 相关联电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功基波电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功基波电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功谐波电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功谐波电能
//5BYTE BCD  NNNNNN.NNNN 相铜损有功电能补偿量
//5BYTE BCD  NNNNNN.NNNN 相铁损有功电能补偿量

//1BYTE HEX 相正向有功电能分频
//1BYTE HEX 相反向有功电能分频
//1BYTE HEX 相组合无功 1 电能分频
//1BYTE HEX 相组合无功 2 电能分频
//1BYTE HEX 相第一象限无功电能分频
//1BYTE HEX 相第二象限无功电能分频
//1BYTE HEX 相第三象限无功电能分频
//1BYTE HEX 相第四象限无功电能分频
//1BYTE HEX 相正向视在电能分频
//1BYTE HEX 相反向视在电能分频
//1BYTE HEX 相关联电能分频
//1BYTE HEX 相正向有功基波电能分频
//1BYTE HEX 相反向有功基波电能分频
//1BYTE HEX 相正向有功谐波电能分频
//1BYTE HEX 相反向有功谐波电能分频
//1BYTE HEX 相铜损有功电能补偿量分频
//1BYTE HEX 相铁损有功电能补偿量分频
//4BYTE CRC32
#define ADDR_00290000     ADDR_00150000+85+17+4//B电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功电能
//5BYTE BCD  NNNNNN.NNNN 相组合无功 1 电能
//5BYTE BCD  NNNNNN.NNNN 相组合无功 2 电能
//5BYTE BCD  NNNNNN.NNNN 相第一象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第二象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第三象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第四象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相正向视在电能
//5BYTE BCD  NNNNNN.NNNN 相反向视在电能
//5BYTE BCD  NNNNNN.NNNN 相关联电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功基波电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功基波电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功谐波电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功谐波电能
//5BYTE BCD  NNNNNN.NNNN 相铜损有功电能补偿量
//5BYTE BCD  NNNNNN.NNNN 相铁损有功电能补偿量

//1BYTE HEX 相正向有功电能分频
//1BYTE HEX 相反向有功电能分频
//1BYTE HEX 相组合无功 1 电能分频
//1BYTE HEX 相组合无功 2 电能分频
//1BYTE HEX 相第一象限无功电能分频
//1BYTE HEX 相第二象限无功电能分频
//1BYTE HEX 相第三象限无功电能分频
//1BYTE HEX 相第四象限无功电能分频
//1BYTE HEX 相正向视在电能分频
//1BYTE HEX 相反向视在电能分频
//1BYTE HEX 相关联电能分频
//1BYTE HEX 相正向有功基波电能分频
//1BYTE HEX 相反向有功基波电能分频
//1BYTE HEX 相正向有功谐波电能分频
//1BYTE HEX 相反向有功谐波电能分频
//1BYTE HEX 相铜损有功电能补偿量分频
//1BYTE HEX 相铁损有功电能补偿量分频
//4BYTE CRC32
#define ADDR_003D0000     ADDR_00290000+85+17+4//C电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功电能
//5BYTE BCD  NNNNNN.NNNN 相组合无功 1 电能
//5BYTE BCD  NNNNNN.NNNN 相组合无功 2 电能
//5BYTE BCD  NNNNNN.NNNN 相第一象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第二象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第三象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相第四象限无功电能
//5BYTE BCD  NNNNNN.NNNN 相正向视在电能
//5BYTE BCD  NNNNNN.NNNN 相反向视在电能
//5BYTE BCD  NNNNNN.NNNN 相关联电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功基波电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功基波电能
//5BYTE BCD  NNNNNN.NNNN 相正向有功谐波电能
//5BYTE BCD  NNNNNN.NNNN 相反向有功谐波电能
//5BYTE BCD  NNNNNN.NNNN 相铜损有功电能补偿量
//5BYTE BCD  NNNNNN.NNNN 相铁损有功电能补偿量

//1BYTE HEX 相正向有功电能分频
//1BYTE HEX 相反向有功电能分频
//1BYTE HEX 相组合无功 1 电能分频
//1BYTE HEX 相组合无功 2 电能分频
//1BYTE HEX 相第一象限无功电能分频
//1BYTE HEX 相第二象限无功电能分频
//1BYTE HEX 相第三象限无功电能分频
//1BYTE HEX 相第四象限无功电能分频
//1BYTE HEX 相正向视在电能分频
//1BYTE HEX 相反向视在电能分频
//1BYTE HEX 相关联电能分频
//1BYTE HEX 相正向有功基波电能分频
//1BYTE HEX 相反向有功基波电能分频
//1BYTE HEX 相正向有功谐波电能分频
//1BYTE HEX 相反向有功谐波电能分频
//1BYTE HEX 相铜损有功电能补偿量分频
//1BYTE HEX 相铁损有功电能补偿量分频
//4BYTE CRC32
#define ADDR_00900100     ADDR_003D0000+85+17+4
//4BYTE 剩余电量NNNNNN.NN kWh(BCD,实时）
//4BYTE 透支电量NNNNNN.NN kWh(BCD,实时）
//4BYTE 未扣减电量NNNNNN.NN kWh(HEX)
//4BYTE 原用于扣减计算的正有功总电量值(HEX)
//4BYTE (暂没)ESAM扣减后应剩余电量NNNNNN.NN kWh(HEX)
//4BYTE CRC32
#define ADDR_00900200     ADDR_00900100+16+4
//4BYTE 剩余金额NNNNNN.NN元(BCD,实时）
//4BYTE 透支金额NNNNNN.NN元(BCD,实时）
//4BYTE 未扣减金额NNNNNN.NN元(HEX)
//4BYTE 未扣减金额小数位0.00NNNN元(HEX)
//4BYTE 结算周期内累计用电量(HEX)
//4*(1+MMaxTariff)BYTE 原用于计算的总,费率1-费率M电能示值(HEX)
//4BYTE 上结算清0结算周期内累计用电量的年月日时(BCD)
//4BYTE (暂没)ESAM扣减后应剩余金额NNNNNN.NN元(HEX)
//4BYTE CRC32
#define ADDR_ESAM_NOTE     ADDR_00900200+20+(4*(1+MMaxTariff))+4+4//ESAM中记录的剩余金额或电量
//4BYTE ESAM中记录的剩余金额(或电量)HHHHHH.HH(HEX)
#define ADDR_000B0000     ADDR_ESAM_NOTE+4//结算周期组合有功总累计用电量
//5BYTE BCD 上次已参于计算的组合有功总电量
//5BYTE BCD 当前结算周期组合有功总累计用电量

#define ADDR_MeterCurveTime     ADDR_000B0000+10//仪表07曲线存贮记录,电表清0时清为值0
//5BYTE 分时日月年,曲线记录开始时间
//5BYTE 分时日月年,最近次曲线记录时间(比较开始时间按分换算成存储偏移点数)
//4BYTE 空
#define ADDR_CONGEAL_COUNT     ADDR_MeterCurveTime+14//冻结计数
//1BYTE 定时冻结(上60次)计数HEX
//5BYTE 保留
//1BYTE 日冻结(上62次)计数HEX
//5BYTE 上次日冻结的分时日月年
//1BYTE 整点冻结(上254次)计数HEX
//5BYTE 上次整点冻结的分时日月年

#define ADDR_02800024     ADDR_CONGEAL_COUNT+18//本月总电压合格时间
//3BYTE_BCD 分
#define ADDR_CLRYMD     ADDR_02800024+3//上次每日清0时的年月日
//3BYTE_BCD 年月日
#define ADDR_BroadcasTimeFlag     ADDR_CLRYMD+3//智能表日广播校时标志
//1BYTE 广播校时标志,0=没广播校时，1=已广播校时
#define ADDR_MESAMMACErrorCount     ADDR_BroadcasTimeFlag+1//MESAM的MAC和密文校验错误计数,在每日的零点清0
//1BYTE_HEX MAC校验和密文解密错误计数,每天收到的数据帧MAC校验失败、密文校验失败总累计达到200次，则要求电能表远程设臵参数、远程控制、清零功能挂起。在每日的零点，清除挂起状态、累计次数
#define ADDR_ClrAutoReport_S_Time     ADDR_MESAMMACErrorCount+1//清0主动上报状态字秒定时器(停电时间不计)
//2byte 秒定时器




#define ADDR_Meter_SaveRam_CRC     ADDR_ClrAutoReport_S_Time+2
//4byte 0x12345678
//4byte CRC
#define ADDR_Meter_SaveRam_End     (ADDR_Meter_SaveRam_CRC+4)
#define ADDR_MeterEnergy_IRAM_End     ADDR_Meter_SaveRam_End//仪表电能类数据在IRAM结束地址
//电表电能类当前数据结束--------------------------------------------------------------------------



#endif
