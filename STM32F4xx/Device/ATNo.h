

#ifndef ATNo_H
#define ATNo_H


//定义AT命令号
#define AT_AT     0//波特率检查																				
	//模块信息
#define AT_ATE0     (AT_AT+1)//关回显且串口检查
#define AT_MYTYPE    (AT_ATE0+1)//查询远程通信模块类型
#define AT_MYGMR     (AT_MYTYPE+1)//获取模块软件版本
#define AT_CPIN     (AT_MYGMR+1)//卡检查
#define AT_MYNETURC     (AT_CPIN+1)//关主动上报
#define AT_MYCCID    (AT_MYNETURC+1)//获取SIM卡序列号ICCID
#define AT_CIMI     (AT_MYCCID+1)//查国际移动台标识请求IMSI
//#define AT_CSPN     (AT_CIMI+1)//读取SIM卡服务运营商名称

	//连接设置
#define AT_CSQ_NOLINK     (AT_CIMI+1)//读信号没连接前
#define AT_CREG     (AT_CSQ_NOLINK+1)//获取当前网络注册状态9
	#if (USER/100)!=0//不是电科院测试(1376.3标准无此命令)
#define AT_COPS2     (AT_CREG+1)
#define AT_COPS     (AT_COPS2+1)//读取SIM卡服务运营商名称
#define AT_CNUM     (AT_COPS+1)//取本机号码
	#else
#define AT_CNUM     (AT_CREG+1)//取本机号码
	#endif
#define AT_SYSINFO	(AT_CNUM+1)//查询注册网络类型：3G/4G
#define AT_MYNETCON_APN     (AT_SYSINFO+1)//设置APN(上紧接AT_SYSINFO不分开)
#define AT_MYNETCON_USERPWD     (AT_MYNETCON_APN+1)//设置USERPWD:用户名和密码

#define AT_MYNETACT     (AT_MYNETCON_USERPWD+1)//激活网络连接
#define AT_MYIPFILTER     (AT_MYNETACT+1)//IP访问控制配置
#define AT_CMFG     (AT_MYIPFILTER+1)//设置短消息模式
#define AT_ZCVF     (AT_CMFG+1)//打开短信功能(仅中兴CDMA(MC8332)时需要)
#define AT_CTA     (AT_ZCVF+1)//休眠等待时间(仅CDMA时需要)

	//没连接循环开始
#define AT_MYNETSRV     (AT_CTA+1)//设置服务参数
#define AT_MYNETOPEN     (AT_MYNETSRV+1)//开启服务 22
	//没连接秒循环开始
#define AT_SMS_NOLINK     (AT_MYNETOPEN+1)//短信服务没连接前
#define AT_CSQ_NOLINK2     (AT_SMS_NOLINK+1)//读信号没连接2
#define AT_LOOP_NOLINK     (AT_CSQ_NOLINK2+1)//AT循环,没连接

	//连接后循环开始
#define AT_CSQ_LINK     (AT_LOOP_NOLINK+1)//读信号连接后
#define AT_SYSINFO_LINK     (AT_CSQ_LINK+1)//查询注册网络类型：3G/4G
#define AT_MYNETOPEN_LINK     (AT_SYSINFO_LINK+1)//连接检查
	//数据收发
#define AT_MYNETREAD     (AT_MYNETOPEN_LINK+1)//读取数据
#define AT_MYNETWRITE     (AT_MYNETREAD+1)//发送数据
#define AT_MYFTP     (AT_MYNETWRITE+1)//FTP文件下载
#define AT_SMS_LINK     (AT_MYFTP+1)//短信服务
#define AT_LOOP     (AT_SMS_LINK+1)//AT循环,在线控制
	//停关机
#define AT_HOLD     (AT_LOOP+1)//HOLD暂停
#define AT_MYNETCLOSE     (AT_HOLD+1)//关闭连接
#define AT_MYPOWEROFF     (AT_MYNETCLOSE+1)//关机

#define AT_MYNETACTOFF     (AT_MYPOWEROFF+1)//激活网络连接
#define AT_ACCEPT     (AT_MYNETACTOFF+1)//集中器做服务器时，处理模块上报的主站IP
#define AT_CGDCONT    (AT_ACCEPT+1)    //设置isp接入网关
#define AT_ATDT     (AT_CGDCONT+1)     //接入号码
#define AT_Max     (AT_ATDT+1)


#endif
