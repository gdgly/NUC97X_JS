
//小时冻结数据缓冲
#ifndef AFN0C_HourCongeal_Buff_H
#define AFN0C_HourCongeal_Buff_H





//小时冻结类数据时标Td_h	见图		2
#define ADDR_AFN0CF81_BUFF     ADDR_HOURCONGEAL_BUFF_START+2//小时冻结总加有功功率
//小时冻结总加有功功率1	见附录A.2	kW	2
//……	……	……	……
//小时冻结总加有功功率i	见附录A.2	kW	2
#define ADDR_AFN0CF82_BUFF     ADDR_AFN0CF81_BUFF+(2*MaxHourCongeal*AFN0CF81MaxPn)//小时冻结总加无功功率
//小时冻结总加无功功率1	见附录A.2	kvar	2
//……	……	……	……
//小时冻结总加无功功率i	见附录A.2	kvar	2
#define ADDR_AFN0CF83_BUFF     ADDR_AFN0CF82_BUFF+(2*MaxHourCongeal*AFN0CF82MaxPn)//小时冻结总加有功总电能量
//小时冻结总加有功总电能量1	见附录A.3	kWh	4
//……	……	……	……
//小时冻结总加有功总电能量i	见附录A.3	kWh	4
#define ADDR_AFN0CF84_BUFF     ADDR_AFN0CF83_BUFF+(4*MaxHourCongeal*AFN0CF83MaxPn)//小时冻结总加无功总电能量
//小时冻结总加无功总电能量1	见附录A.3	kvarh	4
//……	……	……	……
//小时冻结总加无功总电能量i	见附录A.3	kvarh	4
#define ADDR_AFN0CF89_BUFF     ADDR_AFN0CF84_BUFF+(4*MaxHourCongeal*AFN0CF84MaxPn)//小时冻结有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF90_BUFF     ADDR_AFN0CF89_BUFF+(3*MaxHourCongeal*AFN0CF89MaxPn)//小时冻结A相有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF91_BUFF     ADDR_AFN0CF90_BUFF+(3*MaxHourCongeal*AFN0CF90MaxPn)//小时冻结B相有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF92_BUFF     ADDR_AFN0CF91_BUFF+(3*MaxHourCongeal*AFN0CF91MaxPn)//小时冻结C相有功功率
//小时冻结有功功率1	见附录A.9	kW	3
//……	……	……	……
//小时冻结有功功率i	见附录A.9	kW	3
#define ADDR_AFN0CF93_BUFF     ADDR_AFN0CF92_BUFF+(3*MaxHourCongeal*AFN0CF92MaxPn)//小时冻结无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF94_BUFF     ADDR_AFN0CF93_BUFF+(3*MaxHourCongeal*AFN0CF93MaxPn)//小时冻结A无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF95_BUFF     ADDR_AFN0CF94_BUFF+(3*MaxHourCongeal*AFN0CF94MaxPn)//小时冻结B无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF96_BUFF     ADDR_AFN0CF95_BUFF+(3*MaxHourCongeal*AFN0CF95MaxPn)//小时冻结C无功功率
//小时冻结无功功率1	见附录A.9	kvar	3
//……	……	……	……
//小时冻结无功功率i	见附录A.9	kvar	3
#define ADDR_AFN0CF97_BUFF     ADDR_AFN0CF96_BUFF+(3*MaxHourCongeal*AFN0CF96MaxPn)//小时冻结A相电压
//小时冻结A相电压1	见附录A.7	V	2
//……	……	……	……
//小时冻结A相电压i	见附录A.7	V	2
#define ADDR_AFN0CF98_BUFF     ADDR_AFN0CF97_BUFF+(2*MaxHourCongeal*AFN0CF97MaxPn)//小时冻结B相电压
//小时冻结B相电压1	见附录A.7	V	2
//……	……	……	……
//小时冻结B相电压i	见附录A.7	V	2
#define ADDR_AFN0CF99_BUFF     ADDR_AFN0CF98_BUFF+(2*MaxHourCongeal*AFN0CF98MaxPn)//小时冻结C相电压
//小时冻结C相电压1	见附录A.7	V	2
//……	……	……	……
//小时冻结C相电压i	见附录A.7	V	2
#define ADDR_AFN0CF100_BUFF     ADDR_AFN0CF99_BUFF+(2*MaxHourCongeal*AFN0CF99MaxPn)//小时冻结A相电流
//小时冻结A相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结A相电流i	见附录A.25	A	3
#define ADDR_AFN0CF101_BUFF     ADDR_AFN0CF100_BUFF+(3*MaxHourCongeal*AFN0CF100MaxPn)//小时冻结B相电流
//小时冻结B相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结B相电流i	见附录A.25	A	3
#define ADDR_AFN0CF102_BUFF     ADDR_AFN0CF101_BUFF+(3*MaxHourCongeal*AFN0CF101MaxPn)//小时冻结C相电流
//小时冻结C相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结C相电流i	见附录A.25	A	3
#define ADDR_AFN0CF103_BUFF     ADDR_AFN0CF102_BUFF+(3*MaxHourCongeal*AFN0CF102MaxPn)//小时冻结零序电流
//小时冻结N相电流1	见附录A.25	A	3
//……	……	……	……
//小时冻结N相电流i	见附录A.25	A	3
#define ADDR_AFN0CF105_BUFF     ADDR_AFN0CF103_BUFF+(3*MaxHourCongeal*AFN0CF103MaxPn)//小时冻结正向有功总电能量
//小时冻结正向有功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF106_BUFF     ADDR_AFN0CF105_BUFF+(4*MaxHourCongeal*AFN0CF105MaxPn)//小时冻结正向无功总电能量
//小时冻结正向无功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向无功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF107_BUFF     ADDR_AFN0CF106_BUFF+(4*MaxHourCongeal*AFN0CF106MaxPn)//小时冻结反向有功总电能量
//小时冻结反向有功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结反向有功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF108_BUFF     ADDR_AFN0CF107_BUFF+(4*MaxHourCongeal*AFN0CF107MaxPn)//小时冻结反向无功总电能量
//小时冻结反向无功总电能量1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结反向无功总电能量i	见附录A.13	kWh	4
#define ADDR_AFN0CF109_BUFF     ADDR_AFN0CF108_BUFF+(4*MaxHourCongeal*AFN0CF108MaxPn)//小时冻结正向有功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF110_BUFF     ADDR_AFN0CF109_BUFF+(4*MaxHourCongeal*AFN0CF109MaxPn)//小时冻结正向无功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF111_BUFF     ADDR_AFN0CF110_BUFF+(4*MaxHourCongeal*AFN0CF110MaxPn)//小时冻结反向有功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF112_BUFF     ADDR_AFN0CF111_BUFF+(4*MaxHourCongeal*AFN0CF111MaxPn)//小时冻结反向无功总电能示值
//小时冻结正向有功总电能示值1	见附录A.13	kWh	4
//……	……	……	……
//小时冻结正向有功总电能示值i	见附录A.13	kWh	4
#define ADDR_AFN0CF113_BUFF     ADDR_AFN0CF112_BUFF+(4*MaxHourCongeal*AFN0CF112MaxPn)//小时冻结总功率因数
//小时冻结总功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结总功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF114_BUFF     ADDR_AFN0CF113_BUFF+(2*MaxHourCongeal*AFN0CF113MaxPn)//小时冻结A功率因数
//小时冻结A功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结A功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF115_BUFF     ADDR_AFN0CF114_BUFF+(2*MaxHourCongeal*AFN0CF114MaxPn)//小时冻结B功率因数
//小时冻结B功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结B功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF116_BUFF     ADDR_AFN0CF115_BUFF+(2*MaxHourCongeal*AFN0CF115MaxPn)//小时冻结C功率因数
//小时冻结C功率因数1	见附录A.5	％	2
//……	……	……	……
//小时冻结C功率因数i	见附录A.5	％	2
#define ADDR_AFN0CF121_BUFF     ADDR_AFN0CF116_BUFF+(2*MaxHourCongeal*AFN0CF116MaxPn)//小时冻结直流模拟量
//小时冻结直流模拟量1	见附录A.2	2
//……		
//小时冻结直流模拟量i	见附录A.2	2
#define ADDR_HOURCONGEAL_BUFF_END     (ADDR_AFN0CF121_BUFF+(2*MaxHourCongeal*AFN0CF121MaxPn))//小时冻结缓冲结束地址


#define LEN_HOURCONGEAL_BUFF     ((ADDR_HOURCONGEAL_BUFF_END)-(ADDR_HOURCONGEAL_BUFF_START))







#endif

