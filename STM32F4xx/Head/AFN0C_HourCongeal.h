

#ifndef ADDR_AFN0C_HourCongeal_H
#define ADDR_AFN0C_HourCongeal_H





//类1数据
#define ADDR_HOURCONGEAL_START     (ADDR_AFN0C_HourCongeal_Start)//小时冻结开始地址
//1BYTE小时冻结类数据时标冻结时的时
//1BYTE 小时冻结密度,值=0时表示无效冻结
#define ADDR_AFN0CF81     ADDR_HOURCONGEAL_START+2//小时冻结总加有功功率
//小时冻结总加有功功率1	见附录A.2	kW	2
//……	……	……	……
//小时冻结总加有功功率i	见附录A.2	kW	2
#define ADDR_AFN0CF82     ADDR_AFN0CF81+(2*MaxHourCongeal*MaxTotalAddGroup)//小时冻结总加无功功率
//小时冻结总加无功功率1	见附录A.2	kvar	2
//……	……	……	……
//小时冻结总加无功功率i	见附录A.2	kvar	2
#define ADDR_AFN0CF83     ADDR_AFN0CF82+(2*MaxHourCongeal*MaxTotalAddGroup)//小时冻结总加有功总电能量
//小时冻结总加有功总电能量1	见附录A.3	kWh	4
//……	……	……	……
//小时冻结总加有功总电能量i	见附录A.3	kWh	4
#define ADDR_AFN0CF84     ADDR_AFN0CF83+(4*MaxHourCongeal*MaxTotalAddGroup)//小时冻结总加无功总电能量
//小时冻结总加无功总电能量1	见附录A.3	kvarh	4
//……	……	……	……
//小时冻结总加无功总电能量i	见附录A.3	kvarh	4
#define ADDR_AFN0CF89     ADDR_AFN0CF84+(4*MaxHourCongeal*MaxTotalAddGroup)//小时冻结有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF90     ADDR_AFN0CF89+(3*MaxHourCongeal*AFN0CF89MaxPn)//小时冻结A相有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF91     ADDR_AFN0CF90+(3*MaxHourCongeal*AFN0CF90MaxPn)//小时冻结B相有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF92     ADDR_AFN0CF91+(3*MaxHourCongeal*AFN0CF91MaxPn)//小时冻结C相有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF93     ADDR_AFN0CF92+(3*MaxHourCongeal*AFN0CF92MaxPn)//小时冻结无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF94     ADDR_AFN0CF93+(3*MaxHourCongeal*AFN0CF93MaxPn)//小时冻结A无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF95     ADDR_AFN0CF94+(3*MaxHourCongeal*AFN0CF94MaxPn)//小时冻结B无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF96     ADDR_AFN0CF95+(3*MaxHourCongeal*AFN0CF95MaxPn)//小时冻结C无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF97     ADDR_AFN0CF96+(3*MaxHourCongeal*AFN0CF96MaxPn)//小时冻结A相电压
//小时冻结A相电压1	见附录A.7	V	2
//……	……	……	……
//小时冻结A相电压i	见附录A.7	V	2
#define ADDR_AFN0CF98     ADDR_AFN0CF97+(2*MaxHourCongeal*AFN0CF97MaxPn)//小时冻结B相电压
//小时冻结B相电压1	见附录A.7	V	2
//……	……	……	……
//小时冻结B相电压i	见附录A.7	V	2
#define ADDR_AFN0CF99     ADDR_AFN0CF98+(2*MaxHourCongeal*AFN0CF98MaxPn)//小时冻结C相电压
//小时冻结C相电压1	见附录A.7	V	2
//……	……	……	……
//小时冻结C相电压i	见附录A.7	V	2
#define ADDR_AFN0CF100     ADDR_AFN0CF99+(2*MaxHourCongeal*AFN0CF99MaxPn)//小时冻结A相电流
//小时冻结A相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结A相电流i	见附录A.25	A	3
#define ADDR_AFN0CF101     ADDR_AFN0CF100+(3*MaxHourCongeal*AFN0CF100MaxPn)//小时冻结B相电流
//小时冻结B相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结B相电流i	见附录A.25	A	3
#define ADDR_AFN0CF102     ADDR_AFN0CF101+(3*MaxHourCongeal*AFN0CF101MaxPn)//小时冻结C相电流
//小时冻结C相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结C相电流i	见附录A.25	A	3
#define ADDR_AFN0CF103     ADDR_AFN0CF102+(3*MaxHourCongeal*AFN0CF102MaxPn)//小时冻结零序电流
//小时冻结N相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结N相电流i	见附录A.25	A	3
#define ADDR_AFN0CF105     ADDR_AFN0CF103+(3*MaxHourCongeal*AFN0CF103MaxPn)//小时冻结正向有功总电能量
//小时冻结正向有功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF106     ADDR_AFN0CF105+(4*MaxHourCongeal*AFN0CF105MaxPn)//小时冻结正向无功总电能量
//小时冻结正向无功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向无功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF107     ADDR_AFN0CF106+(4*MaxHourCongeal*AFN0CF106MaxPn)//小时冻结反向有功总电能量
//小时冻结反向有功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结反向有功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF108     ADDR_AFN0CF107+(4*MaxHourCongeal*AFN0CF107MaxPn)//小时冻结反向无功总电能量
//小时冻结反向无功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结反向无功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF109     ADDR_AFN0CF108+(4*MaxHourCongeal*AFN0CF108MaxPn)//小时冻结正向有功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF110     ADDR_AFN0CF109+(4*MaxHourCongeal*AFN0CF109MaxPn)//小时冻结正向无功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF111     ADDR_AFN0CF110+(4*MaxHourCongeal*AFN0CF110MaxPn)//小时冻结反向有功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF112     ADDR_AFN0CF111+(4*MaxHourCongeal*AFN0CF111MaxPn)//小时冻结反向无功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF113     ADDR_AFN0CF112+(4*MaxHourCongeal*AFN0CF112MaxPn)//小时冻结总功率因数
//小时冻结总功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结总功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF114     ADDR_AFN0CF113+(2*MaxHourCongeal*AFN0CF113MaxPn)//小时冻结A功率因数
//小时冻结A功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结A功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF115     ADDR_AFN0CF114+(2*MaxHourCongeal*AFN0CF114MaxPn)//小时冻结B功率因数
//小时冻结B功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结B功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF116     ADDR_AFN0CF115+(2*MaxHourCongeal*AFN0CF115MaxPn)//小时冻结C功率因数
//小时冻结C功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结C功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF121     ADDR_AFN0CF116+(2*MaxHourCongeal*AFN0CF116MaxPn)//小时冻结直流模拟量
//小时冻结直流模拟量1	见附录A.2	2
//……		
//小时冻结直流模拟量i	见附录A.2	2
#define ADDR_HOURCONGEAL_END     ADDR_AFN0CF121+(2*MaxHourCongeal*AFN0CF121MaxPn)//小时冻结缓冲结束地址


#define ADDR_AFN0C_HourCongeal_End     (ADDR_HOURCONGEAL_END)//类1数据结束地址





#endif


