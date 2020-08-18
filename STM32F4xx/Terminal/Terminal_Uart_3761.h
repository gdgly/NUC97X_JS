

#ifndef UART3761_H
#define UART3761_H



//export
void Uart_3761_RxTx(u32 PORTn);//UART_3761接收发送
void Uart_3761_Data(u32 PORTn);//UART_3761数据


//extern
u32 Uart_376_PW(u32 PORTn);//消息认证码字段PW;出口0有效,1无效,4已返错误帧

void Uart_AFN00(u32 PORTn);//确认/否认
void Uart_AFN01(u32 PORTn);//复位
void Uart_AFN02(u32 PORTn);//链路接口检测
void Uart_AFN04(u32 PORTn);//参数设置
void Uart_AFN05(u32 PORTn);//控制命令
void Uart_AFN06(u32 PORTn);//身份认证及密钥协商
void Uart_AFN08(u32 PORTn);//请求被级联终端主动上报
void Uart_AFN09(u32 PORTn);//请求终端配置
void Uart_AFN0A(u32 PORTn);//参数查询
void Uart_AFN0B(u32 PORTn);//请求任务数据
void Uart_AFN0C(u32 PORTn);//类1数据
void Uart_AFN0D(u32 PORTn);//类2数据
void Uart_AFN0E(u32 PORTn);//类3数据(事件)
void Uart_AFN0F(u32 PORTn);//文件传输
void Uart_AFN10(u32 PORTn);//数据转发
void Uart_AFN255(u32 PORTn);//工厂测试(扩展)
void Uart_AFN255(u32 PORTn);//所有版本信息

void Forwarding(u32 PORTn);//数据转发处理过程
void Cascading(u32 PORTn);//级联处理过程
void Cascading_LoopChack(u32 PORTn);//主动级联巡测
void AutoCascadingData(u32 PORTn);//自动上传级联巡测到的数据


void AFN04_Configure(void);//终端支持的参数配置,出口:数据在ADDR_DATABUFF
void AFN05_Configure(void);//终端支持的控制配置,出口:数据在ADDR_DATABUFF
void AFN0C_Configure(void);//终端支持的1类数据配置,出口:数据在ADDR_DATABUFF
void AFN0D_Configure(void);//终端支持的2类数据配置,出口:数据在ADDR_DATABUFF

#if (USER/100)==12//四川用户
void Uart_AFN0CF15(u32 PORTn);//四川扩展读取SIM卡信号强度
void Uart_AFN0CF2040(u32 PORTn);//2016年第三批中标四川专变扩展远程模块信号强度召测
void Uart_AFN0CF305(u32 PORTn);//透明任务执行状态
void Uart_AFN0CF306(u32 PORTn);//透明任务执行结果
#endif
#if (((Project/100)==2)&&((USER/100)==5))//上海集中器
void Uart_AFN0CF201(u32 PORTn);//上海集中器扩展AFN0CF201读取最近一次抄读失败表个数P0
void Uart_AFN0CF202(u32 PORTn);//上海集中器扩展AFN0CF202读取最近一次抄读电能表失败表信息P0
#endif
#if (USER/100)==8//用户标识：通用版
void Uart_AFN0DF321(u32 PORTn);//电表对时后误差情况统计数量
void Uart_AFN0DF322(u32 PORTn);//辽宁电表对时后误差情况统计明细
#endif
#if (USER/100)==6//用户标识：湖南
void Uart_AFN0DF40x(u32 PORTn);//AFN0DF400、F401、F402
#endif
#endif


