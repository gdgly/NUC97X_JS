

#ifndef FLASH_class8_H
#define FLASH_class8_H
#ifdef __cplusplus
 extern "C" {
#endif

	 

//---通信类---
#define ADDR_4001     ADDR_DL698_SET_START//通信地址
//属性2∷=octet-string
#define LENmax_4001     18
#define ADDR_4002     ADDR_4001+LENmax_4001//表号
//属性2∷=octet-string
#define LENmax_4002     18
#define ADDR_4003     ADDR_4002+LENmax_4002//客户编号
//属性2∷=octet-string
#define LENmax_4003     18
#define ADDR_4001_Factory     ADDR_4003+LENmax_4003//通信地址,表号,客户编号的出厂设置
//LENmax_4001+LENmax_4002+LENmax_4003
#define LENmax_4001_Factory     (LENmax_4001+LENmax_4002+LENmax_4003)
	 
#define ADDR_4005     ADDR_4001_Factory+LENmax_4001_Factory//组地址
//属性2∷=array octet-string
#define LENmax_4005     (2+(16*18))
#define  ADDR_4300_7     ADDR_4005+LENmax_4005//允许跟随上报
//属性7∷=bool
#define LENmax_4300_7     2
#define  ADDR_4300_8     ADDR_4300_7+LENmax_4300_7//允许主动上报
//属性8∷=bool
#define LENmax_4300_8     2
#define  ADDR_4300_9     ADDR_4300_8+LENmax_4300_8//允许与主站通话
//属性9∷=bool
#define LENmax_4300_9     2
#define  ADDR_4300_10     ADDR_4300_9+LENmax_4300_9//上报通道
//属性10∷=array OAD
#define LENmax_4300_10     32
#define ADDR_4401     ADDR_4300_10+LENmax_4300_10//应用连接认证密码
//属性2（只写）∷=visible-string
#define LENmax_4401     128
#define ADDR_4500_2     ADDR_4401+LENmax_4401//公网通信模块1;2通信配置
//通信配置∷=structure
//{
//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
//在线方式      enum{永久在线（0），被动激活（1）}，
//连接方式      enum{TCP（0），UDP（1）}，
//连接应用方式  enum{主备模式（0），多连接模式（1）}，
//侦听端口列表  array long-unsigned，
//APN            visible-string，
//用户名         visible-string，
//密码           visible-string，
//代理服务器地址 octet-string，
//代理端口            long-unsigned，
//超时时间及重发次数  unsigned
//（
//bit7~bit2：超时时间（秒），
//bit1~bit0：重发次数
//），
//心跳周期(秒)  long-unsigned
//}
#define LENmax_4500_2     128
#define ADDR_4500_3     ADDR_4500_2+LENmax_4500_2//公网通信模块1;3主站通信参数表
//主站通信参数表∷=array 主站通信参数
//主站通信参数：：=structure
//{
//IP地址   octet-string，
//端口     long-unsigned，
//}
#define LENmax_4500_3     64
#define ADDR_4500_4     ADDR_4500_3+LENmax_4500_3//公网通信模块1;4短信通信参数
//短信通信参数：：=structure
//{
//短信中心号码     visible-string(SIZE(16))，
//主站号码         array visible-string(SIZE(16))
//短信通知目的号码 array visible-string(SIZE(16))
//}
#define LENmax_4500_4     64
#define ADDR_4500_11     ADDR_4500_4+LENmax_4500_4//公网通信模块1;11 拨号IP
//octet-string
#define LENmax_4500_11     22
#define ADDR_4510_2     ADDR_4500_11+LENmax_4500_11//以太网通信模块1;2通信配置
//通信配置∷=structure
//{
//工作模式  enum{混合模式（0），客户机模式（1），服务器模式（2）}，
//连接方式  enum{TCP（0），UDP（1）}，
//连接应用方式enum{主备模式（0），多连接模式（1）}
//侦听端口列表array long-unsigned，
//代理服务器地址  octet-string，
//代理端口  long-unsigned，
//超时时间及重发次数  unsigned
//（
//bit7~bit2：超时时间（秒），
//bit1~bit0：重发次数
//），
//心跳周期(秒)  long-unsigned
//}
#define LENmax_4510_2     128
#define ADDR_4510_3     ADDR_4510_2+LENmax_4510_2//以太网通信模块1;3主站通信参数表
//主站通信参数表∷=array 主站通信参数
//主站通信参数：：=structure
//{
//IP地址  octet-string，
//端口     long-unsigned
//}
#define LENmax_4510_3     64
#define ADDR_4510_4     ADDR_4510_3+LENmax_4510_3//以太网通信模块1;4网络配置
//网络配置∷=structure
//{
//IP配置方式enum{DHCP（0），静态（1），PPPoE（2）}，
//IP地址       octet-string，
//子网掩码     octet-string，
//网关地址     octet-string，
//PPPoE用户名  octet-string，
//PPPoE密码    octet-string
//}
#define LENmax_4510_4     128
#define ADDR_4510_5     ADDR_4510_4+LENmax_4510_4// 以太网通信模块1;5 MAC地址
//MAC地址∷=octet-string 
#define LENmax_4510_5     16
	
#define ADDR_F101_2     ADDR_4510_5+LENmax_4510_5//安全模式参数(初始化值0x00)
//属性2（安全模式选择）∷=enum
//{
//不启用安全模式参数（0），
//启用安全模式参数  （1）
//}
//安全模式选择意义：0—不启用安全模式参数、默认安全模式参数，1—启用安全模式参数、默认安全模式参数。
#define LENmax_F101_2     2
#define ADDR_F101_3     ADDR_F101_2+LENmax_F101_2//显式安全模式参数(初始化值0x00)
//属性3（显式安全模式参数）∷=array 安全模式参数
//安全模式参数∷=structure
//{
//  对象标识   OI，
//  安全模式   long-unsigned
//}
//属性3为显式安全模式参数（设置值），如果对象安全性不在属性3中，则按默认安全模式参数。如果同一对象安全性在安全模式参数、默认安全模式参中均有说明，则按显式安全模式参数执行。安全模式参数、默认安全参数具体定义见附录F，安全模式定义见附录F中表F.1。
#define LENper_F101_3     8//实长度
#define NUMmax_F101_3     128
#define LENmax_F101_3     (4+(LENper_F101_3*NUMmax_F101_3))
#if LENmax_F101_3>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_F200_2     ADDR_F101_3+LENmax_F101_3//RS232
//端口∷=structure
//{
//端口号     OAD
//端口参数   COMDCB，
//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
//}
#define LENper_F200_2     15
#define LENmax_F200_2     LENper_F200_2
#define ADDR_F201_2     ADDR_F200_2+LENmax_F200_2//RS485
//端口∷=structure
//{
//端口号     OAD
//端口参数   COMDCB，
//端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）}
//}
#define LENper_F201_2     15
#define NUMmax_F201_2     3
#define LENmax_F201_2     (LENper_F201_2*NUMmax_F201_2)
#define ADDR_F202_2     ADDR_F201_2+LENmax_F201_2//红外
//端口∷=structure
//{
//端口号     OAD
//端口参数   COMDCB，
//}
#define LENper_F202_2     15
#define LENmax_F202_2     LENper_F202_2
#define ADDR_F002_4     ADDR_F202_2+LENmax_F202_2//FTP文件
//属性4（服务器信息）∷=structure
//{
//IP地址  octet-string，
//端口    long-unsigned，
//用户名  visible-string，
//密码    visible-string
//}
#define LENmax_F002_4     64
#define ADDR_F002_7     ADDR_F002_4+LENmax_F002_4
//方法7：从服务器下载（文件信息，协议类型）
//参数∷=structure
//{
//	文件信息 structure
//	协议类型 enum
//	{
//	telnet+zmodem协议 （0），
//	ftp协议 （1），
//	sftp协议 （2），
//	http 协议 （3），
//	https协议 （4）
//	}
//}
#define LENmax_F002_7     128
#define ADDR_F002_8      ADDR_F002_7+LENmax_F002_7
//方法8：上传到服务器（文件信息，协议类型）
//参数∷ =structure
//{
//文件 信息 structure
//协议类型  enum
//}
//以客户机模式主动连接指定远程服务器上传文件，并通过“命令结果”反馈执行情况。源文件不存在则返回错误，目标文件不存在则创建。
#define LENmax_F002_8     120//128

//4BYTE 参数有效标志固定为0x12345678
//4BYTE CRC32
#define ADDR_DL698_COMSET_END     (ADDR_F002_8+LENmax_F002_8+8)//ADDR_F002_8+LENmax_F002_8
//---通信类结束---


//以下参数在初始化时会先全部清0
#define ADDR_MenuKey     ADDR_DL698_COMSET_END//进入菜单密码
//6byte  初始值为0




//---事件参数---
#define NUMmax_EventOAD     16//最大事件关联OAD最大设置个数
#define LENmax_EventOAD     82//2+(5*NUMmax_EventOAD)
#if (LENmax_EventOAD!=(2+(5*NUMmax_EventOAD)))
	#error
#endif
#define ADDR_3000     ADDR_MenuKey+6//电能表失压事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3000_5     ADDR_3000+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//电压触发上限  long-unsigned（单位：V，换算：-1），
//  电压恢复下限  long-unsigned（单位：V，换算：-1），
//  电流触发下限  double-long（单位：A，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3000_5     15
#define ADDR_3001     ADDR_3000_5+LENmax_3000_5//电能表欠压事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3001_5     ADDR_3001+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  电压触发上限  long-unsigned（单位：V，换算：-1），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3001_5     7
#define ADDR_3002     ADDR_3001_5+LENmax_3001_5//电能表过压事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3002_5     ADDR_3002+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3002_5     7
#define ADDR_3003     ADDR_3002_5+LENmax_3002_5//电能表断相事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3003_5     ADDR_3003+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  电压触发上限  long-unsigned（单位：V，换算：-1），
//  电流触发上限  double-long（单位：A，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3003_5     12
#define ADDR_3004     ADDR_3003_5+LENmax_3003_5//电能表失流事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3004_5     ADDR_3004+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
//  电流触发上限  double-long（单位：A，换算：-4），
//  电流恢复下限  double-long（单位：A，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3004_5     17
#define ADDR_3005     ADDR_3004_5+LENmax_3004_5//电能表过流事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3005_5     ADDR_3005+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  电流触发下限  double-long（单位：A，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3005_5     9
#define ADDR_3006     ADDR_3005_5+LENmax_3005_5//电能表断流事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3006_5     ADDR_3006+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  电压触发下限  long-unsigned（单位：V，换算：-1），
//  电流触发上限  double-long（单位：A，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3006_5     12
#define ADDR_3007     ADDR_3006_5+LENmax_3006_5//电能表潮流反向事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3007_5     ADDR_3007+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  有功功率触发下限  double-long（单位：W，换算：-1），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3007_5     9
#define ADDR_3008     ADDR_3007_5+LENmax_3007_5//电能表过载事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3008_5     ADDR_3008+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  有功功率触发下限  double-long（单位：W，换算：-1），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3008_5     9
#define ADDR_3009     ADDR_3008_5+LENmax_3008_5//电能表正向有功需量超限事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3009_6     ADDR_3009+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  触发下限  double-long-unsigned（单位：kW，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_3009_6     9
#define ADDR_300A     ADDR_3009_6+LENmax_3009_6//电能表反向有功需量超限事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_300A_6     ADDR_300A+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  触发下限  double-long-unsigned（单位：kW，换算：-4），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_300A_6     9
#define ADDR_300B     ADDR_300A_6+LENmax_300A_6//电能表无功需量超限事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_300B_5     ADDR_300B+4+LENmax_EventOAD
//属性5（配置参数）∷=structure
//{
//  触发下限 double-long-unsigned（单位：kvar，换算：-4），
//  判定延时时间 unsigned（单位：s，换算：0）
//}
#define LENmax_300B_5     9
#define ADDR_300C     ADDR_300B_5+LENmax_300B_5//电能表功率因数超下限事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_300C_6     ADDR_300C+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  下限阀值  long（单位：%，换算：-1），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_300C_6     7
#define ADDR_300D     ADDR_300C_6+LENmax_300C_6//电能表全失压事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_300D_6     ADDR_300D+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_300D_6     2
#define ADDR_300E     ADDR_300D_6+LENmax_300D_6//电能表辅助电源掉电事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_300E_6     ADDR_300E+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_300E_6     4
#define ADDR_300F     ADDR_300E_6+LENmax_300E_6//电能表电压逆相序事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_300F_6     ADDR_300F+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_300F_6     4
#define ADDR_3010     ADDR_300F_6+LENmax_300F_6//电能表电流逆相序事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3010_6     ADDR_3010+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_3010_6     4
#define ADDR_3011     ADDR_3010_6+LENmax_3010_6//电能表掉电事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3011_6     ADDR_3011+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_3011_6     4
#define ADDR_3012     ADDR_3011_6+LENmax_3011_6//电能表编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3012_6     ADDR_3012+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3012_6     2
#define ADDR_3013     ADDR_3012_6+LENmax_3012_6//电能表清零事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3013_6     ADDR_3013+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3013_6     2
#define ADDR_3014     ADDR_3013_6+LENmax_3013_6//电能表需量清零事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3014_6     ADDR_3014+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3014_6     2
#define ADDR_3015     ADDR_3014_6+LENmax_3014_6//电能表事件清零事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3015_6     ADDR_3015+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3015_6     2
#define ADDR_3016     ADDR_3015_6+LENmax_3015_6//电能表校时事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3016_6     ADDR_3016+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3016_6     2
#define ADDR_3017     ADDR_3016_6+LENmax_3016_6//电能表时段表编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3017_6     ADDR_3017+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3017_6     2
#define ADDR_3018     ADDR_3017_6+LENmax_3017_6//电能表时区表编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3018_6     ADDR_3018+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3018_6     2
#define ADDR_3019     ADDR_3018_6+LENmax_3018_6//电能表周休日编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3019_6     ADDR_3019+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3019_6     2
#define ADDR_301A     ADDR_3019_6+LENmax_3019_6//电能表结算日编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_301A_6     ADDR_301A+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_301A_6     2
#define ADDR_301B     ADDR_301A_6+LENmax_301A_6//电能表开盖事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_301B_6     ADDR_301B+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_301B_6     2
#define ADDR_301C     ADDR_301B_6+LENmax_301B_6//电能表开端钮盒事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_301C_6     ADDR_301C+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_301C_6     2
#define ADDR_301D     ADDR_301C_6+LENmax_301C_6//电能表电压不平衡事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_301D_6     ADDR_301D+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  限值  long（单位：%，换算：-2），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_301D_6     7
#define ADDR_301E     ADDR_301D_6+LENmax_301D_6//电能表电流不平衡事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_301E_6     ADDR_301E+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  限值  long（单位：%，换算：-2），
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_301E_6     7
#define ADDR_301F     ADDR_301E_6+LENmax_301E_6//电能表跳闸事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_301F_6     ADDR_301F+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_301F_6     2
#define ADDR_3020     ADDR_301F_6+LENmax_301F_6//电能表合闸事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3020_6     ADDR_3020+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3020_6     2
#define ADDR_3021     ADDR_3020_6+LENmax_3020_6//电能表节假日编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3021_6     ADDR_3021+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3021_6     2
#define ADDR_3022     ADDR_3021_6+LENmax_3021_6//电能表有功组合方式编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3022_6     ADDR_3022+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3022_6     2
#define ADDR_3023     ADDR_3022_6+LENmax_3022_6//电能表无功组合方式编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3023_6     ADDR_3023+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
//事件发生源∷=enum
//{
//无功组合方式1特征字（0），
//无功组合方式2特征字（1）
//}
#define LENmax_3023_6     2
#define ADDR_3024     ADDR_3023_6+LENmax_3023_6//电能表费率参数表编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3024_6     ADDR_3024+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3024_6     2
#define ADDR_3025     ADDR_3024_6+LENmax_3024_6//电能表阶梯表编程事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3025_6     ADDR_3025+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3025_6     2
#define ADDR_3026     ADDR_3025_6+LENmax_3025_6//电能表密钥更新事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3026_6     ADDR_3026+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3026_6     2
#define ADDR_3027     ADDR_3026_6+LENmax_3026_6//电能表异常插卡事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3027_6     ADDR_3027+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3027_6     2
#define ADDR_3028     ADDR_3027_6+LENmax_3027_6//电能表购电记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3028_6     ADDR_3028+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3028_6     2
#define ADDR_3029     ADDR_3028_6+LENmax_3028_6//电能表退费记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3029_6     ADDR_3029+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3029_6     2
#define ADDR_302A     ADDR_3029_6+LENmax_3029_6//电能表恒定磁场干扰事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_302A_6     ADDR_302A+4+LENmax_EventOAD//新修订中已删除属性6
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_302A_6     4
#define ADDR_302B     ADDR_302A_6+LENmax_302A_6//电能表负荷开关误动作事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_302B_6     ADDR_302B+4+LENmax_EventOAD//新修订中已删除属性6
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_302B_6     4
#define ADDR_302C     ADDR_302B_6+LENmax_302B_6//电能表电源异常事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_302C_6     ADDR_302C+4+LENmax_EventOAD//新修订中已删除属性6
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_302C_6     4
#define ADDR_302D     ADDR_302C_6+LENmax_302C_6//电能表电流严重不平衡事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_302D_6     ADDR_302D+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  限值  long（单位：%，换算：-2）
//  判定延时时间  unsigned（单位：s，换算：0）
//}
#define LENmax_302D_6     7
#define ADDR_302E     ADDR_302D_6+LENmax_302D_6//电能表时钟故障事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_302E_6     ADDR_302E+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_302E_6     4
#define ADDR_302F     ADDR_302E_6+LENmax_302E_6//电能表计量芯片故障事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_302F_6     ADDR_302F+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
#define LENmax_302F_6     4
#define ADDR_3030     ADDR_302F_6+LENmax_302F_6//通信模块变更事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3030_6     ADDR_3030+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  判定延时  unsigned（单位：s，换算：0）
//}
//事件发生源∷=OAD
//事件发生源为通信模块OAD。
#define LENmax_3030_6     4


#define ADDR_3100     ADDR_3030_6+LENmax_3030_6//终端初始化事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3100_6     ADDR_3100+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3100_6     2
#define ADDR_3101     ADDR_3100_6+LENmax_3100_6//终端版本变更事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3101_6     ADDR_3101+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3101_6     2
#define ADDR_3104     ADDR_3101_6+LENmax_3101_6//终端状态量变位事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3104_6     ADDR_3104+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3104_6     2
#define ADDR_3105     ADDR_3104_6+LENmax_3104_6//电能表时钟超差事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3105_6     ADDR_3105+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//异常判别阈值  long-unsigned（单位：秒），
//关联采集任务号 unsigned
//}
//事件发生源∷=TSA
//采集监控任务中需要配置相关OAD的采集任务。
#define LENmax_3105_6     7
#define ADDR_3106     ADDR_3105_6+LENmax_3105_6//终端停/上电事件	
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3106_6     ADDR_3106+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//停电数据采集配置参数  structure
//{
//采集标志  bit-string(SIZE(8))，
//停电事件抄读时间间隔（小时） unsigned ，
//停电事件抄读时间限值（分钟） unsigned，
//需要读取停电事件电能表       array TSA
//}，
//停电事件甄别限值参数  structure
//{
//停电时间最小有效间隔（分钟） long-unsigned，
//停电时间最大有效间隔（分钟） long-unsigned，
//停电事件起止时间偏差限值（分钟） long-unsigned，
//停电事件时间区段偏差限值（分钟） long-unsigned，
//停电发生电压限值  long-unsigned（单位：V，换算：-1），
//停电恢复电压限值  long-unsigned（单位：V，换算：-1）
//}
//}
//采集标志：
//bit0：置“1”有效，置“0”无效；
//bit1：置“1”随机选择测量点，置“0”只采集设置的测量点。
#define LENmax_3106_6     84
#define ADDR_3107     ADDR_3106_6+LENmax_3106_6//终端直流模拟量越上限事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3107_6     ADDR_3107+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  直流模拟量上限  double-long
//}
//事件发生源∷=OAD（直流模拟量号）
#define LENmax_3107_6     7
#define ADDR_3108     ADDR_3107_6+LENmax_3107_6//终端直流模拟量越下限事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3108_6     ADDR_3108+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  直流模拟量下限  double-long
//}
//事件发生源∷=OAD（直流模拟量号）
#define LENmax_3108_6     7
#define ADDR_3109     ADDR_3108_6+LENmax_3108_6//终端消息认证错误事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3109_6     ADDR_3109+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3109_6     2
#define ADDR_310A     ADDR_3109_6+LENmax_3109_6//设备故障记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_310A_6     ADDR_310A+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
//事件发生源∷=enum
//{
//  终端主板内存故障（0），
//  时钟故障        （1），
//  主板通信故障    （2），
//  485抄表故障    （3），
//  显示板故障      （4），
//  载波通道异常    （5）
//}
#define LENmax_310A_6     2
#define ADDR_310B     ADDR_310A_6+LENmax_310A_6//电能表示度下降事件;ADDR_310B-ADDR_310E_6不能分开存放,计算事件时1次读入
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_310B_6     ADDR_310B+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  关联采集任务号 unsigned
//}
//事件发生源∷=TSA
//采集监控任务中需要配置相关OAD的采集任务。
#define LENmax_310B_6     4
#define ADDR_310C     ADDR_310B_6+LENmax_310B_6//电能量超差事件;ADDR_310B-ADDR_310E_6不能分开存放,计算事件时1次读入
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_310C_6     ADDR_310C+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  阈值  double-long-unsigned（单位：%，无换算）,
//  关联采集任务号 unsigned
//}
//事件发生源∷=TSA
//采集监控任务中需要配置相关OAD的采集任务。
#define LENmax_310C_6     9
#define ADDR_310D     ADDR_310C_6+LENmax_310C_6//电能表飞走事件;ADDR_310B-ADDR_310E_6不能分开存放,计算事件时1次读入
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_310D_6     ADDR_310D+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  阈值  double-long-unsigned（单位：%，无换算）,
//  关联采集任务号 unsigned
//}
//事件发生源∷=TSA
//采集监控任务中需要配置相关OAD的采集任务。
#define LENmax_310D_6     9
#define ADDR_310E     ADDR_310D_6+LENmax_310D_6//电能表停走事件;ADDR_310B-ADDR_310E_6不能分开存放,计算事件时1次读入
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_310E_6     ADDR_310E+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  阈值           TI,
//  关联采集任务号 unsigned
//}
//事件发生源∷=TSA
//采集监控任务中需要配置相关OAD的采集任务。
#define LENmax_310E_6     8
#define ADDR_310F     ADDR_310E_6+LENmax_310E_6//终端抄表失败事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_310F_6     ADDR_310F+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  重试轮次      unsigned,
//  关联采集任务号 unsigned
//}
//事件发生源∷=TSA
#define LENmax_310F_6     6
#define ADDR_3110     ADDR_310F_6+LENmax_310F_6//月通信流量超限事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3110_6     ADDR_3110+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  通信流量门限  double-long-unsigned（单位：byte）
//}
#define LENmax_3110_6     7
#define ADDR_3111     ADDR_3110_6+LENmax_3110_6//发现未知电能表事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3111_6     ADDR_3111+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3111_6     2
#define ADDR_3112     ADDR_3111_6+LENmax_3111_6//跨台区电能表事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3112_6     ADDR_3112+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3112_6     2
#define ADDR_3114     ADDR_3112_6+LENmax_3112_6//终端对时事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3114_6     ADDR_3114+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3114_6     2
#define ADDR_3115     ADDR_3114_6+LENmax_3114_6//遥控跳闸记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3115_6     ADDR_3115+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
//事件发生源∷=unsigned（继电器序号）
#define LENmax_3115_6     2
#define ADDR_3116     ADDR_3115_6+LENmax_3115_6//有功总电能量差动越限事件记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3116_6     ADDR_3116+4+LENmax_EventOAD
//属性6（配置参数）∷=array 有功总电能量差动组配置
//有功总电能量差动组配置∷=structure
//{
//  有功总电能量差动组序号 unsigned，
//  对比的总加组           OI，
//  参照的总加组           OI，
//  参与差动的电能量的时间区间及对比方法标志 bit-string（SIZE（8）），
//  差动越限相对偏差值 integer（单位：%，换算：0），
//  差动越限绝对偏差值 long64（单位：kWh，换算：-4）
//}
//参与差动的电能量的时间区间及对比方法标志：
//bit0～bit1编码表示电能量的时间跨度，取值范围0～2依次表示60分钟电量、30分钟电量、15分钟电量，其他值无效。
//bit7表示对比方法标志，置“0”：相对对比，公式见公式（1）；置“1”：绝对对比，公式见公式（2）。
//bit2～bit6备用。
//	 ％  .....	(1)
//	  .............	(2)
//式中：
//    Q——对比的总加组总电能量；
//    q——参照的总加组总电能量。
#define LENmax_3116_6     194
#define ADDR_3117     ADDR_3116_6+LENmax_3116_6//输出回路接入状态变位事件记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3117_6     ADDR_3117+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3117_6     2
#define ADDR_3118     ADDR_3117_6+LENmax_3117_6//终端编程记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3118_6     ADDR_3118+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3118_6     2
#define ADDR_3119     ADDR_3118_6+LENmax_3118_6//终端电流回路异常事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3119_6     ADDR_3119+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
//事件发生源∷=enum{短路(0)，开路(1)}
#define LENmax_3119_6     2
#define ADDR_311A     ADDR_3119_6+LENmax_3119_6//电能表在网状态切换事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_311A_6     ADDR_311A+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//   判定延时时间  long-unsigned（单位：s，换算：0）
//}
//事件发生源∷=NULL
//此事件只记录电表在网状态变迁。
#define LENmax_311A_6     5
#define ADDR_311B     ADDR_311A_6+LENmax_311A_6//终端对电表校时记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_311B_6     ADDR_311B+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_311B_6     5
#define ADDR_311C     ADDR_311B_6+LENmax_311B_6//电能表数据变更监控记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_311C_6     ADDR_311C+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  关联采集任务序号    unsigned
//}
//事件发生源∷=TSA
#define LENmax_311C_6     4

#define ADDR_311F     ADDR_311C_6+LENmax_311C_6//拒绝从节点入网记录 
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_311F_6     ADDR_311F+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//  关联采集任务序号    unsigned
//}
//事件发生源∷=TSA
#define LENmax_311F_6     3

#define ADDR_3200     ADDR_311F_6+LENmax_311F_6//功控跳闸记录
//#define ADDR_3200     ADDR_311C_6+LENmax_311C_6//功控跳闸记录

//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3200_6     ADDR_3200+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3200_6     2
#define ADDR_3201     ADDR_3200_6+LENmax_3200_6//电控跳闸记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3201_6     ADDR_3201+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3201_6     2
#define ADDR_3202     ADDR_3201_6+LENmax_3201_6//购电参数设置记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3202_6     ADDR_3202+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
//事件发生源∷=OI
#define LENmax_3202_6     2
#define ADDR_3203     ADDR_3202_6+LENmax_3202_6//电控告警事件记录
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3203_6     ADDR_3203+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3203_6     2

//---事件参数结束---

	 
//---通用类---
#define ADDR_4000_3     ADDR_3203_6+LENmax_3203_6//校时模式
//属性3（校时模式）∷=enum
//{
//主站授时（0），
//终端精确校时（1），
//北斗/GPS（2），
//其它（255）
//}
#define LENmax_4000_3     2
#define ADDR_4000_4     ADDR_4000_3+LENmax_4000_3//精准校时参数
//属性4（精准校时参数）∷=structure
//{
//最近心跳时间总个数   unsigned，
//最大值剔除个数       unsigned，
//最小值剔除个数       unsigned，
//通讯延时阈值         unsigned（单位：秒）， 
//最少有效个数         unsigned
//}
#define LENmax_4000_4     12
#define ADDR_4004     ADDR_4000_4+LENmax_4000_4//设备地理位置
//属性2∷=structure
//{
	//经度  structure
	//{
		//方位   enum{E（0），W（1）}，
		//度     unsigned，
		//分     unsigned，
		//秒     unsigned
	//}，
	//纬度  structure
	//{
		//方位   enum{S（0），N（1）}，
		//度     unsigned，
		//分     unsigned，
		//秒     unsigned
	//}，
	//高度（cm）  double-long-unsigned
//}
#define LENmax_4004     30
#define ADDR_4006     ADDR_4004+LENmax_4004//时钟源
//属性2（只读）∷=structure
//{
//	时钟源  enum
//	{
//		内部（0），时钟芯片（1），互联网时钟（2），卫星时钟（3），长波时钟（4）
//	}，
//	状态  enum
//	{
//		可用（0），不可用（1）
//  }
//}
#define LENmax_4006     16
#define ADDR_4007     ADDR_4006+LENmax_4006//LCD参数
//属性2::=structure
//{
//  上电全显时间  unsigned，
//  背光点亮时间  long-unsigned(按键时背光点亮时间)，
//  显示查看背光点亮时间  long-unsigned，
//  有电按键屏幕驻留最大时间  long-unsigned，
//  无电按键屏幕驻留最大时间  long-unsigned，
//  显示电能小数位数  unsigned，
//  显示功率（最大需量）小数位数  unsigned
//}
//以上时间的单位均为：秒。
#define LENmax_4007     20


//---电能表类---
#define ADDR_iMeterControlData     ADDR_4007+LENmax_4007
//1byte
//1BYTE 两套时区表切换状态0=1套,1=2套
//1BYTE 两套日时段表切换态0=1套,1=2套
//1BYTE 两套梯度切换状态0=1套,1=2套
//1BYTE 两套费率电价切换0=1套,1=2套
//1byte 
//7byte YYMDHMS hex两套时区表切换时间
//1byte 
//7byte YYMDHMS hex两套日时段表切换时间
//1byte 
//7byte YYMDHMS hex两套梯度切换时间
//1byte 
//7byte YYMDHMS hex两套费率电价切换时间
//1byte 
#define ADDR_4008     ADDR_iMeterControlData+38//备用套时区表切换时间,紧接上不分开1次写入
//属性2∷=date_time_s
//秒=FFH
#define LENmax_4008     8
#define ADDR_4009     ADDR_4008+LENmax_4008//备用套日时段切换时间
//属性2∷=date_time_s
//秒=FFH
#define LENmax_4009     8
#define ADDR_400A     ADDR_4009+LENmax_4009//备用套分时费率切换时间
//属性2∷=date_time_s
//秒=FFH
#define LENmax_400A     8
#define ADDR_400B     ADDR_400A+LENmax_400A//备用套阶梯电价切换时间
//属性2∷=date_time_s
//秒=FFH
#define LENmax_400B     8
#define ADDR_400C     ADDR_400B+LENmax_400B//时区时段数
//属性2∷=structure
//{
//年时区数(p≤14)                 unsigned，
//日时段表数（q≤8）              unsigned，
//日时段数(每日切换数)（m≤14）   unsigned，
//费率数（k≤63）                 unsigned，
//公共假日数（n≤254）            unsigned
//}
#define LENmax_400C     12
#define ADDR_400D     ADDR_400C+LENmax_400C//阶梯数
//属性2∷=unsigned，无单位，无换算
#define LENmax_400D     2
#define ADDR_400E     ADDR_400D+LENmax_400D//谐波分析次数
//属性2∷=unsigned，无单位，无换算
#define LENmax_400E     2
#define ADDR_400F     ADDR_400E+LENmax_400E//密钥总条数
//属性2∷=unsigned，无单位，无换算
#define LENmax_400F     2
#define ADDR_4010     ADDR_400F+LENmax_400F//计量元件数
//属性2∷=unsigned，无单位，无换算
#define LENmax_4010     2
#define ADDR_4011     ADDR_4010+LENmax_4010//公共假日表
//属性2∷=array 公共假日
//公共假日∷=structure
//{
//  日期        date，
//  日时段表号  unsigned
//}
//电能表实际使用多少个公共假日取决于公共假日数，建议电能表内至少可保存20个公共假日。
#define LENper_4011     8
#define NUMmax_4011     30
#define LENmax_4011     (2+(LENper_4011*NUMmax_4011))
#define ADDR_4012     ADDR_4011+LENmax_4011//周休日特征字
//属性2∷=bit-string(SIZE(8))
#define LENmax_4012     3
#define ADDR_4013     ADDR_4012+LENmax_4012//周休日釆用的日时段表号
//属性2∷=unsigned，无单位，无换算
#define LENmax_4013     2
#define ADDR_4014     ADDR_4013+LENmax_4013//当前套时区表
//属性2∷=array
//时区∷=structure
//{
//月          unsigned,
//日          unsigned,
//  日时段表号  unsigned
//}
#define LENper_4014     8
#define NUMmax_4014     14
#define LENmax_4014     (2+(LENper_4014*NUMmax_4014))
#define ADDR_4015     ADDR_4014+LENmax_4014//备用套时区表
//属性2∷=array
//时区∷=structure
//{
//月          unsigned,
//日          unsigned,
//  日时段表号  unsigned
//}
#define LENper_4015     8
#define NUMmax_4015     14
#define LENmax_4015     (2+(LENper_4015*NUMmax_4015))
#define ADDR_4016     ADDR_4015+LENmax_4015//当前套日时段表
//属性2∷=array 日时段表
//日时段表∷=array 时段
//时段∷=structure
//{
//  时     unsigned,
//  分     unsigned,
//  费率号 unsigned
//}
//费率号：该时段采用的费率号
#define LENper_4016     (2+(8*14))
#define NUMmax_4016     8
#define LENmax_4016     (2+(LENper_4016*NUMmax_4016))
#define ADDR_4017     ADDR_4016+LENmax_4016//备用套日时段表
//属性2∷=array 日时段表
//日时段表∷=array 时段
//时段∷=structure
//{
//  时     unsigned,
//  分     unsigned,
//  费率号 unsigned
//}
//费率号：该时段采用的费率号
#define LENper_4017     (2+(8*14))
#define NUMmax_4017     8
#define LENmax_4017     (2+(LENper_4017*NUMmax_4017))
#define ADDR_4018     ADDR_4017+LENmax_4017//当前套费率电价
//属性2∷=array 费率电价
//费率电价∷=double-long-unsigned
//单位：元/kWh，换算：-4，电能表最多支持32个费率电价，当前套费率电价协议只读。
#define LENper_4018     5
#define NUMmax_4018     8
#define LENmax_4018     (2+(LENper_4018*NUMmax_4018))
#define ADDR_4019     ADDR_4018+LENmax_4018//备用套费率电价
//属性2∷=array 费率电价
//费率电价∷=double-long-unsigned
//单位：元/kWh，换算：-4，电能表最多支持32个费率电价，当前套费率电价协议只读。
#define LENper_4019     5
#define NUMmax_4019     8
#define LENmax_4019     (2+(LENper_4019*NUMmax_4019))
#define ADDR_401A     ADDR_4019+LENmax_4019//当前套阶梯电价
//阶梯参数∷=structure
//{
//  阶梯值数组    array 阶梯值，
//  阶梯电价数组  array 阶梯电价，
//  阶梯结算日数组  array 阶梯结算日
//}
//阶梯值∷=double-long-unsigned
//阶梯电价∷=double-long-unsigned
//阶梯结算日∷=structure
//{
//月  unsigned，
//  日  unsigned，
//  时  unsigned
//}
//阶梯值：单位：kWh，换算：-2
//阶梯电价：单位：元/kWh，换算：-4
//阶梯结算日中月、日、时均有效时则按结算日执行阶梯冻结，如果第1阶梯结算日中仅月、日有效则以此每月该日时执行阶梯冻结，当全部无效时不执行阶梯冻结。
//当前套阶梯参数协议只读。
#define LENmax_401A     512
#define ADDR_401B     ADDR_401A+LENmax_401A//备用套阶梯电价
//同401A,备用套阶梯参数可读可写
#define LENmax_401B     512
#define ADDR_401C     ADDR_401B+LENmax_401B//电流互感器变比
//属性2∷=double-long-unsigned，无单位，换算:-2
#define LENmax_401C     8
#define ADDR_401D     ADDR_401C+LENmax_401C//电压互感器变比
//属性2∷=double-long-unsigned，无单位，换算:-2
#define LENmax_401D     8
#define ADDR_401E     ADDR_401D+LENmax_401D//报警金额限值
//属性2（参数）∷=structure
//{
//报警金额限值1    double-long-unsigned，
//报警金额限值2    double-long-unsigned，
//}
//单位：元，换算：-2
#define LENmax_401E     12
#define ADDR_401F     ADDR_401E+LENmax_401E//其它金额限值
//属性2（参数）∷=structure
//{
//透支金额限值     double-long-unsigned，
//囤积金额限值     double-long-unsigned，
//合闸允许金额限值 double-long-unsigned，
//}
//单位：元，换算：-2
#define LENmax_401F     18
#define ADDR_4020     ADDR_401F+LENmax_401F//报警电量限值
//属性2∷=structure
//{
//报警电量限值1    double-long-unsigned，
//报警电量限值2    double-long-unsigned，
//}
//单位：kWh，换算：-2
#define LENmax_4020     12
#define ADDR_4021     ADDR_4020+LENmax_4020//其它电量限值
//属性2∷=structure
//{
//囤积电量限值     double-long-unsigned，
//透支电量限值     double-long-unsigned，
//合闸允许电量限值 double-long-unsigned，
//}
//单位：kWh，换算：-2
#define LENmax_4021     18
#define ADDR_4023     ADDR_4021+LENmax_4021//认证有效时长
//属性2∷=long-unsigned ，单位：分钟，无换算
#define LENmax_4023     3
#define ADDR_4024     ADDR_4023+LENmax_4023//剔除
//属性2∷=enum {剔除投入（1），剔除解除（2）}
#define LENmax_4024     2
#define ADDR_4030     ADDR_4024+LENmax_4024//电压合格率参数
//属性2∷=structure
//{
//	电压考核上限  long-unsigned（单位：V，换算：-1），
//  电压考核下限  long-unsigned（单位：V，换算：-1），
//	电压合格上限  long-unsigned（单位：V，换算：-1），
//  电压合格下限  long-unsigned（单位：V，换算：-1）
//}
#define LENmax_4030     14
#define ADDR_4100     ADDR_4030+LENmax_4030//最大需量周期
//属性2∷=unsigned，单位：分钟，换算：0
#define LENmax_4100     2
#define ADDR_4101     ADDR_4100+LENmax_4100//滑差时间
//属性2∷=unsigned，单位：分钟，换算：0
#define LENmax_4101     2
#define ADDR_4102     ADDR_4101+LENmax_4101//校表脉冲宽度
//属性2∷=unsigned，单位：毫秒，换算：0
#define LENmax_4102     2
#define ADDR_4103     ADDR_4102+LENmax_4102//资产管理编码
//属性2∷=visible-string(SIZE(32))
#define LENmax_4103     34

#define ADDR_410C     ADDR_4103+LENmax_4103//ABC各相电导系数
//属性2∷=structure
//{
//A 相电导  long，单位：无，换算：-3
//B 相电导  long，单位：无，换算：-3
//C 相电导  long，单位：无，换算：-3
// }
#define LENmax_410C     12
#define ADDR_410D     ADDR_410C+LENmax_410C//ABC各相电抗系数
//属性2∷=structure
//{
//A 相电抗  long，单位：无，换算：-3
//B 相电抗  long，单位：无，换算：-3
//C 相电抗  long，单位：无，换算：-3
// }
#define LENmax_410D     12
#define ADDR_410E     ADDR_410D+LENmax_410D//ABC各相电阻系数
//属性2∷=structure
//{
//A 相电阻  long，单位：无，换算：-3
//B 相电阻  long，单位：无，换算：-3
//C 相电阻  long，单位：无，换算：-3
// }
#define LENmax_410E     12
#define ADDR_410F     ADDR_410E+LENmax_410E//ABC各相电纳系数
//属性2∷=structure
//{
//A 相电纳  long，单位：无，换算：-3
//B 相电纳  long，单位：无，换算：-3
//C 相电纳  long，单位：无，换算：-3
// }
#define LENmax_410F     12
#define ADDR_4112     ADDR_410F+LENmax_410F//有功组合方式特征字
//属性2∷=bit-string(SIZE(8))，见附录G
#define LENmax_4112     3
#define ADDR_4113     ADDR_4112+LENmax_4112//无功组合方式1特征字
//属性2∷=bit-string(SIZE(8))，见附录G
#define LENmax_4113     3
#define ADDR_4114     ADDR_4113+LENmax_4113//无功组合方式2特征字
//属性2∷=bit-string(SIZE(8))，见附录G
#define LENmax_4114     3
#define ADDR_4115     ADDR_4114+LENmax_4114//IC卡
//属性2∷=structure
//{
//IC卡类型  enum
//{
//参数预制卡（0），用户开户卡（1），
//用户购电卡（2），用户补卡（3）
//}，
//}
#define LENmax_4115     4
#define ADDR_4116     ADDR_4115+LENmax_4115//结算日
//属性2（配置参数）∷=array 结算日日期
//结算日日期∷=structure
//{
//日  unsigned,
//时  unsigned
//}
#define LENper_4116     6
#define NUMmax_4116     31
#define LENmax_4116     (2+(LENper_4116*NUMmax_4116))
#define ADDR_4117     ADDR_4116+LENmax_4116//期间需量冻结周期
//属性2（配置参数）∷= TI
#define LENmax_4117     5




//---终端类---

//4200	11	路由表
//4201	8	路由信息单元

#define ADDR_4202     ADDR_4117+LENmax_4117//级联通信参数
//属性2∷=structure
//{
//级联标志            bool，
//级联通信端口号      OAD，
//总等待超时（10ms）  long-unsigned，
//字节超时（10ms）    long-unsigned，
//重发次数            unsigned，
//巡测周期（min）     unsigned，
//级联（被）端口数    unsigned，
//级联（被）终端地址  array TSA
//}
#define LENmax_4202     128
#define ADDR_4204_2     ADDR_4202+LENmax_4202//终端广播校时-2
//属性2（终端广播校时参数）∷=structure
//{
//     终端广播校时启动时间  time，
//     是否启用              bool
//}
#define LENmax_4204_2     16
#define ADDR_4204_3     ADDR_4204_2+LENmax_4204_2//终端广播校时-3
//属性3（终端单地址广播校时参数）∷=structure
//{
//     时钟误差阈值          unsigned（单位：秒），
//     终端广播校时启动时间  time，
//     是否启用              bool
//}
//电表时钟误差由终端自动计算获得，前提是采集任务中配置有采集电表时钟。执行单地址广播校时后生成对应的终端对电表校时事件
#define LENmax_4204_3     16



#define ADDR_5000     ADDR_4204_3+LENmax_4204_3//瞬时冻结关联对象属性表
//关联对象属性表∷=array 一个关联的对象属性//删除时array的数据单元清为0(NULL)
//一个关联的对象属性∷=structure
//{
//  冻结周期  long-unsigned，
//  关联对象属性描述符  OAD，
//  存储深度  long-unsigned
//}
#define LENmax_50xx     (2+(NUMmax_50xx*13))
#define ADDR_5001     ADDR_5000+LENmax_50xx//秒冻结关联对象属性表
//同上
#define ADDR_5002     ADDR_5001+LENmax_50xx//分钟冻结关联对象属性表
//同上
#define ADDR_5003     ADDR_5002+LENmax_50xx//小时冻结关联对象属性表
//同上
#define ADDR_5004     ADDR_5003+LENmax_50xx//日冻结关联对象属性表
//同上
#define ADDR_5005     ADDR_5004+LENmax_50xx//结算日冻结关联对象属性表
//同上
#define ADDR_5006     ADDR_5005+LENmax_50xx//月冻结关联对象属性表
//同上
#define ADDR_5007     ADDR_5006+LENmax_50xx//年冻结关联对象属性表
//同上
#define ADDR_5008     ADDR_5007+LENmax_50xx//时区表切换冻结关联对象属性表
//同上
#define ADDR_5009     ADDR_5008+LENmax_50xx//日时段表切换冻结关联对象属性表
//同上
#define ADDR_500A     ADDR_5009+LENmax_50xx//费率电价切换冻结关联对象属性表
//同上
#define ADDR_500B     ADDR_500A+LENmax_50xx//阶梯切换冻结关联对象属性表
//同上
#define ADDR_5011     ADDR_500B+LENmax_50xx//阶梯结算冻结关联对象属性表
//同上





#define ADDR_6000     ADDR_5011+LENmax_50xx//采集档案配置表(初始化值0x00)
//属性2（配置表，只读）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	配置序号  long-unsigned,
//	基本信息  Basic_object，
//	扩展信息  Extended_object，
//	附属信息  Annex_object
//}

//Basic_object∷=structure
//{
//	通信地址  TSA，
//	波特率    enum
//	{
//		300bps（0），  600bps（1），    1200bps（2），
//		2400bps（3）， 4800bps（4），   7200bps（5），
//		9600bps（6）， 19200bps（7），  38400bps（8），
//		57600bps（9），115200bps（10），自适应（255）
//	}，
//	规约类型  enum
//	{
//  	未知 (0)，
//		DL/T645-1997（1），
//		DL/T645-2007（2），
//		DL/T698.45（3），
//		CJ/T 188-2004（4）
//	}，
//	端口      OAD，
//	通信密码  octet-string，
//	费率个数  unsigned，
//	用户类型  unsigned，
//	接线方式  enum
//	{
//   未知（0），
//   单相（1），
//   三相三线（2），
//   三相四线（3）
//	}
//额定电压    long-unsigned(换算-1，单位V),
//额定电流    long-unsigned(换算-1，单位A)
//}

//Extended_object∷=structure
//{
//	采集器地址  TSA，
//	资产号      octet-string，
//	PT          long-unsigned，
//	CT          long-unsigned
//}
//Annex_object∷=array structure
//{
//	对象属性描述  OAD，
//	属性值        Data
//}
#define LENper_6000     128
#define NUMmax_6000     NUM_RMmax
#define LENmax_6000     (LENper_6000*NUMmax_6000)

#define ADDR_6002_8     ADDR_6000+LENmax_6000//搜表参数
//属性8∷=structure
//{
//	是否启用每天周期搜表   bool，
//	自动更新采集档案       bool，
//	是否产生搜表相关事件   bool，
//	清空搜表结果选项       enum
//	{
//		不清空             （0），
//		每天周期搜表前清空 （1），
//		每次搜表前清空     （2）
//	}
//}
#define LENmax_6002_8     10
#define ADDR_6002_9     ADDR_6002_8+LENmax_6002_8//每天周期搜表参数配置
//属性9（每天周期搜表参数配置）∷=array 定时搜表参数
//定时搜表参数∷=structure
//{
//   开始时间         time，
//   搜表时长（min）  long-unsigned
//}
#define LENmax_6002_9     (2+(9*12))
#define ADDR_6002_127     ADDR_6002_9+LENmax_6002_9//实时启动搜表(初始化值0xFF)
//搜表时长∷=long-unsigned，单位：分钟，表示搜表持续时间，0表示不限时间直至搜表结束。
#define LENmax_6002_127     3
#define ADDR_6012_2     ADDR_6002_127+LENmax_6002_127//任务配置表(初始化值0x00)
//属性2（任务配置单元）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	任务ID    unsigned，
//	执行频率   TI，
//	方案类型   enum
//	{
//		普通采集方案 （1），   事件采集方案 （2），
//		透明方案     （3），   上报方案     （4），
//		脚本方案     （5）
//	}，
//	方案编号    unsigned，
//	开始时间    date_time_s，
//	结束时间    date_time_s，
//	延时        TI，
//	执行优先级  unsigned{首要（1），必要（2），需要（3），可能（4）}，
//	状态        enum{正常（1），停用（2）}，
//	任务开始前脚本id   long-unsigned，
//	任务完成后脚本id   long-unsigned，
//	任务运行时段       structure，
//}
//当方案类型为脚本时，方案编号为脚本id
//任务运行时段∷=structrue
//{
//	类型  enum
//	{
//		前闭后开    （0），
//		前开后闭    （1），
//		前闭后闭    （2），
//		前开后开    （3）
//	}，
//	时段表  array 时段
//}
//时段∷=structure
//{
//	起始小时  unsigned，
//	起始分钟  unsigned，
//	结束小时  unsigned，
//	结束分钟  unsigned
//}
#define LENper_6012_2     128
#define LENmax_6012_2     (LENper_6012_2*NUMmax_6012_2)
#define ADDR_6014     ADDR_6012_2+LENmax_6012_2//普通采集方案
//属性2（普通采集方案）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	方案编号  unsigned，
//	存储深度  long-unsigned，
//	采集方式  structure
//	{
//  	采集类型  unsigned，
//  	采集内容  Data
//	}，
//	记录列选择  array CSD，
//	电能表集合  MS，
//	存储时标选择 enum
//	{
//		未定义              （0），
//		任务开始时间        （1），
//		相对当日0点0分    （2），
//		相对上日23点59分  （3），
//		相对上日0点0分    （4），
//		相对当月1日0点0分（5），
//		数据冻结时标        （6）
//    相对上月月末23点59分 (7)
//	}
//}
//采集方式数据格式如下：
//采集类型	 采集内容	 表示
//0	       NULL	     采集当前数据
//1	       unsigned	 采集上第N次
//2				 NULL	     按冻结时标采集
//3	       TI	       按时标间隔采集
//4        RetryMetering 补抄

//RetryMetering::=structure
//{
//	数据时标间隔 TI
//	补抄周期(上N次) long-unsigned
//}
#define LENper_6014     512
#define LENmax_6014     (LENper_6014*NUMmax_6014)
#define ADDR_6016     ADDR_6014+LENmax_6014//事件采集方案集(初始化值0x00)
//属性2（事件采集方案Event acq plan）∷=structure
//{
//方案编号        unsigned，
//采集方式        structure
//{
//  采集类型      unsigned，
//  采集内容      Data
//}
//电能表集合      MS，
//上报标识        bool（True：立即上报，False：不上报），
//存储深度        long-unsigned
//}
//采集方式数据格式如下：
//采集类型	 采集内容	表示
//0	array ROAD	周期采集事件数据
//1	NULL	根据通知采集所有事件数据
//2	array ROAD	根据通知采集指定事件数据
//3	NULL 根据通知存储生成的事件数据
#define LENper_6016     1024
#define LENmax_6016     (LENper_6016*NUMmax_6016)
#define ADDR_6018     ADDR_6016+LENmax_6016//透明方案集(初始化值0x00)
//属性2（透明方案）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	方案编号     unsigned，
//	方案内容集   array 方案内容，
//	方案内容∷=structure
//	{
//		通信地址        	TSA，
//		开始前脚本id   	long-unsigned，
//		完成后脚本id   	long-unsigned，
//		方案控制标志  	structure，
//		{
//			上报透明方案结果并等待后续报文  bool，
//			等待后续报文超时时间（秒）      long-unsigned，
//			结果比对标识    enum{不比对（0），比（1），比对上报（2）}，
//			结果比对参数    structure
//			{
//				特征字节      unsigned，
//				截取开始      long-unsigned，
//				截取长度      long-unsigned
//			}
//		}
//		方案报文集 	 	array 方案报文
//		方案报文∷=structure
//		{
//			报文序号  unsigned，
//			报文内容  octet-string
//		}
//	}
//	存储深度     long-unsigned
//}
#define LENper_6018     8192
#define LENmax_6018     (LENper_6018*NUMmax_6018)
#define ADDR_601C     ADDR_6018+LENmax_6018//上报方案集(初始化值0x00)
//属性2（上报方案 report plan）∷=structure//删除时structure的数据单元清为0(NULL)
//{
//方案编号          unsigned，
//上报通道          array OAD，
//上报响应超时时间  TI，
//最大上报次数      unsigned，
//上报内容          strcuture
//{
//  类型  unsigned，
//  数据  Data
//}
//}
//上报内容：
//上报类型	  上报内容	表示
// 0	      OAD	对象属性数据
// 1	      RecordData	上报记录型对象属性
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//	记录型对象属性描述符  RCSD，
//	行选择                RSD
//}
#define LENper_601C     512
#define LENmax_601C     (LENper_601C*NUMmax_601C)
#define ADDR_601E     ADDR_601C+LENmax_601C//采集规则库(初始化值0x00)
//属性2∷=structure//删除时structure的数据单元清为0(NULL)
//{
//	数据列选择描述符  CSD，
//	规则描述  structrue
//	{
//		AcqCmd_2007  structure
//		{
//			主用DI  array octet-string(SIZE(4))，
//			备用DI  array octet-string(SIZE(4))
//		}，
//		AcqCmd_1997  structure
//		{
//			主用DI  array octet-string(SIZE(2))，
//			备用DI  array octet-string(SIZE(2))
//		}，
//		AcqCmd_Trans  structure
//		{
//			Frame  octet-string
//		}
//	}
//}
#define LENper_601E     256//160
#define NUMmax_601E     512
#define LENmax_601E     (LENper_601E*NUMmax_601E)


#define ADDR_8000_2     ADDR_601E+LENmax_601E//遥控-属性2（配置参数)
//属性2（配置参数)∷=structure//;//删除时structure类型单元=0
//{
//  继电器拉闸电流门限值    double-long-unsigned（单位：A，换算-4），
//  超电流门限保护延时时间  long-unsigned（单位：分钟，换算0）
//}
#define LENmax_8000_2     10
#define ADDR_8000_127     ADDR_8000_2+LENmax_8000_2//遥控-方法127：触发告警（参数）
//方法127：触发告警（参数）参数∷=NULL
//方法128：解除报警（参数）参数∷=NULL
//触发=0x55,解除=0x0
#define LENmax_8000_127     1
#define ADDR_8000_129     ADDR_8000_127+LENmax_8000_127//遥控-方法129：跳闸（参数）
//方法129：跳闸（参数）
//参数∷=array structure;//删除时数组数=0
//{
//继电器      OAD，合闸时相应OAD为0x00000000
//告警延时    unsigned（单位：分钟，换算：0），
//限电时间    long-unsigned（单位：分钟，换算：0；值为0表示永久限电），
//自动合闸    bool（True：自动合闸；False：非自动合闸）
//}
#define LENper_8000_129     14
#define LENmax_8000_129     (2+(LENper_8000_129*NUMmax_CR))
#define ADDR_8000_130     ADDR_8000_129+LENmax_8000_129//遥控-方法130：合闸（参数）
//方法130：合闸（参数）
//参数∷=array structure;//删除时数组数=0
//{
//继电器    OAD，跳闸相应OAD为0x00000000
//命令      enum{合闸允许（0），直接合闸（1）}
//密码      visible-string
//}
#define LENmax_8000_130     (2+(32*NUMmax_CR))

#define ADDR_8001_127     ADDR_8000_130+LENmax_8000_130//方法127：投入保电
//方法127：投入保电（参数）参数∷=NULL
//用于投入保电状态，即禁止一切执行跳闸的继电器输出控制，且恢复已跳闸的继电器输出控制。
//方法128：解除保电（参数）参数∷=NULL
//用于解除保电状态。
//投入=0x55,解除=0x0
#define LENmax_8001_127     1
#define ADDR_8001_3     ADDR_8001_127+LENmax_8001_127//保电-允许与主站最大无通信时长（分钟）
//属性3∷=long-unsigned
//允许与主站最大无通信时长（分钟），0表示不自动保电。
#define LENmax_8001_3     3
#define ADDR_8001_4     ADDR_8001_3+LENmax_8001_3//保电-上电自动保电时长（分钟）
//属性4∷=long-unsigned
//上电自动保电时长（分钟），0表示上电不自动保电。
#define LENmax_8001_4     3
#define ADDR_8001_5     ADDR_8001_4+LENmax_8001_4//保电-自动保电时段
//属性5∷=array 自动保电时段;//删除时数组数=0
//自动保电时段∷=structure
//{
//起始时间（时）  unsigned，
//结束时间（时）  unsigned
//}
//时间段区间规则为前闭后开。
#define LENmax_8001_5     (8*24)
#define ADDR_8002_127     ADDR_8001_5+LENmax_8001_5//方法127：催费告警投入
//方法127：催费告警投入（参数）
//参数∷=structure//删除时structure的数据单元清为0(NULL)
//{
//  告警时段：octet-string(SIZE(3))，
//  告警信息：visible-string(SIZE(200))
//}
//方法128：取消催费告警（参数）参数∷=NULL
#define LENmax_8002_127     256


#define ADDR_8100_2     ADDR_8002_127+LENmax_8002_127//终端保安定值
//终端保安定值∷=long64（单位：W，换算：-1）
#define LENmax_8100_2     9
#define ADDR_8101_2     ADDR_8100_2+LENmax_8100_2//终端功控时段
//属性2（配置参数）∷=array unsigned;//删除时数组数=0
//D7	D6	D5	D4	D3	D2	D1	D0		
//1:30-2:00	1:00-1:30	0:30-1:00	0:00-0:30	第l字节
//3:30-4:00	3:00-3:30	2:30-3:00	2:00-2:30	第2字节
//...	
//23:30-24:00	23:00-23:30	22:30-23:00	22:00-22:30	第12字节
#define LENmax_8101_2     (2+(2*12))
#define ADDR_8102_2     ADDR_8101_2+LENmax_8101_2//功控告警时间
//属性2（配置参数）∷=array unsigned
//告警时间按顺序表示1-n轮次的功控告警时间（单位：分钟）
#define LENmax_8102_2     18
#define ADDR_8103_2     ADDR_8102_2+LENmax_8102_2//时段功控-控制方案集(初始化值0x00)
//属性2（控制方案集）∷=array 时段功控配置单元;//删除时数组数=0
//属性2∷=structure//
//{
//总加组对象  OI，
//方案标识    bit-string(SIZE(8))，
//第一套定值  PowerCtrlParam，
//第二套定值  PowerCtrlParam，
//第三套定值  PowerCtrlParam，
//时段功控定值浮动系数  integer（单位：%）
//}
//PowerCtrlParam∷=structure
//{
//	时段号          bit-string(SIZE(8))，
//	时段1功控定值  long64（单位：W，换算：-1），
//	时段2功控定值  long64（单位：W，换算：-1），
//	时段3功控定值  long64（单位：W，换算：-1），
//	时段4功控定值  long64（单位：W，换算：-1），
//	时段5功控定值  long64（单位：W，换算：-1），
//	时段6功控定值  long64（单位：W，换算：-1），
//	时段7功控定值  long64（单位：W，换算：-1），
//	时段8功控定值  long64（单位：W，换算：-1）
//}
#define LENper_8103_2     241//实长度
#define LENmax_8103_2     (2+( LENper_8103_2*NUMmax_TAG))
#if LENmax_8103_2>(LEN_128KDATABUFF/2)
	#error
#endif
#define ADDR_8103_127     ADDR_8103_2+LENmax_8103_2//时段功控-方案切换(初始化值0x00)
//array //删除时数组数=0
//时段功控方案切换∷=structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//时段功控投入标识    bit-string(SIZE(8))，
//时段功控定值方案号  unsigned
//}
//时段功控投入标识：D0～D7按顺序对位表示第1～第8时段，置“1”：有效，置“0”：无效。
//时段功控定值方案号：数值范围：0～2依次表示第1～第3套方案，其他值无效。
#define LENper_8103_127     12//实长度
#define LENmax_8103_127     (2+(LENper_8103_127*NUMmax_TAG))
#define ADDR_8104_2     ADDR_8103_127+LENmax_8103_127//厂休控配置单元
//属性2（控制方案集）∷=array 厂休控配置单元;//删除时数组数=0
//属性2∷=structure
//{
//总加组对象    OI，
//厂休控定值    long64（单位：W，换算：-1），
//限电起始时间  date_time_s（年=FFFFH，月=FFH，日=FFH），
//限电延续时间  long-unsigned（单位：分钟），
//每周限电日    bit-string(SIZE(8)) 
//}
//每周限电日：D1～D7表示星期一～星期日，D0=0。
#define LENper_8104_2     28//实长度
#define LENmax_8104_2     (2+(LENper_8104_2*NUMmax_TAG))
#if LENmax_8104_2>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8105_2     ADDR_8104_2+LENmax_8104_2//营业报停控配置单元(初始化值0x00)
//属性2（控制方案集）∷=array 营业报停控配置单元;//删除时数组数=0
//属性2∷=structure
//{
//总加组对象      OI，
//报停起始时间    date_time_s（时=FFH，分=FFH），
//报停结束时间    date_time_s（时=FFH，分=FFH），
//报停控功率定值  long64（单位：W，换算：-1）
//}
#define LENper_8105_2     30//实长度
#define LENmax_8105_2     (2+(LENper_8105_2*NUMmax_TAG))
#if LENmax_8105_2>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8106_127     ADDR_8105_2+LENmax_8105_2//当前功率下浮控-方法127 投入（总加组对象，控制方案）(初始化值0x00)
//array;//删除时数组数=0
//structure
//总加组对象∷=OI
//控制方案∷=structure
//{
//当前功率下浮控定值滑差时间    unsigned（单位：分钟），
//当前功率下浮控定值浮动系数    integer（单位：%），
//控后总加有功功率冻结延时时间  unsigned（单位：分钟），
//当前功率下浮控的控制时间      unsigned（单位：0.5小时），
//当前功率下浮控第1轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第2轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第3轮告警时间  unsigned（单位：分钟），
//当前功率下浮控第4轮告警时间  unsigned（单位：分钟）
//}
#define LENper_8106_127     23//实长度
#define LENmax_8106_127     (2+(LENper_8106_127*NUMmax_TAG))
#if LENmax_8106_127>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8107_2     ADDR_8106_127+LENmax_8106_127//购电控配置单元
//属性2（控制方案集）∷=array 购电控配置单元;//删除时数组数=0
//属性2∷=structure
//{
//	总加组对象      OI，
//购电单号        double-long-unsigned，
//追加/刷新标识   enum{追加（0），刷新（1）}，
//购电类型        enum{电量（0），电费（1）}，
//购电量（费）值  long64（单位：kWh/元， 换算：-4），
//	报警门限值      long64（单位：kWh/元，换算：-4），
//	跳闸门限值      long64（单位：kWh/元，换算：-4）
//	购电控模式      enum{本地模式（0），远程模式（1）}
//}
#define LENper_8107_2     43//实长度
#define LENmax_8107_2     (2+(LENper_8107_2*NUMmax_TAG))
#if LENmax_8107_2>(LEN_128KDATABUFF/2)
#error
#endif
#define ADDR_8108_2     ADDR_8107_2+LENmax_8107_2//月电控配置单元
//属性2（控制方案集）∷=array 月电控配置单元;//删除时数组数=0
//属性2∷=structure
//{
//	总加组对象      OI，
//	月电量控定值    long64（单位：kWh，换算：-4），
//	报警门限值系数  unsigned（单位：%），
//	月电量控定值浮动系数  integer（单位：%）
//}
#define LENper_8108_2     18//实长度
#define LENmax_8108_2     (2+(LENper_8108_2*NUMmax_TAG))
#if LENmax_8108_2>(LEN_128KDATABUFF/2)
#error
#endif

#define ADDR_8103_3     ADDR_8108_2+LENmax_8108_2//时段功控-总加组控制投入状态(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态;//删除时数组数=0
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
#define LENper_8103_3     7//实长度
#define LENmax_8103_3     (2+(LENper_8103_3*NUMmax_TAG))
#define ADDR_8104_3     ADDR_8103_3+LENmax_8103_3//厂休控-总加组控制投入状态(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态;//删除时数组数=0
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
#define LENper_8104_3     7//实长度
#define LENmax_8104_3     (2+(LENper_8104_3*NUMmax_TAG))
#define ADDR_8105_3     ADDR_8104_3+LENmax_8104_3//营业报停控-总加组控制投入状态(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态;//删除时数组数=0
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
#define LENper_8105_3     7//实长度
#define LENmax_8105_3     (2+(LENper_8105_3*NUMmax_TAG))
#define ADDR_8106_3     ADDR_8105_3+LENmax_8105_3//当前功率下浮控-总加组控制投入状态(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态;//删除时数组数=0
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
#define LENper_8106_3     7//实长度
#define LENmax_8106_3     (2+(LENper_8106_3*NUMmax_TAG))
#define ADDR_8107_3     ADDR_8106_3+LENmax_8106_3//购电控-总加组控制投入状态(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态;//删除时数组数=0
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
#define LENper_8107_3     7//实长度
#define LENmax_8107_3     (2+(LENper_8107_3*NUMmax_TAG))
#define ADDR_8108_3     ADDR_8107_3+LENmax_8107_3//月电控-总加组控制投入状态(初始化值0x00)
//控制投入状态∷=array 一个总加组控制投入状态;//删除时数组数=0
//一个总加组控制投入状态∷=structure
//{
//  总加组对象  OI，
//投入状态  enum{未投入（0），投入（1）}
//}
#define LENper_8108_3     7//实长度
#define LENmax_8108_3     (2+(LENper_8108_3*NUMmax_TAG))


#define ADDR_F203_4     ADDR_8108_3+LENmax_8108_3
//属性4∷=structure
//{
//开关量接入标志bit-string(SIZE（8）)
//（
//bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”：接入，置“0”：未接入。
//），
//开关量属性标志bit-string(SIZE（8）)
//（
//  bit0～bit7按顺序对位表示第1～8路状态量输入，置“1”常开触点。置“0”：常闭触点。
//）
//}
#define LENmax_F203_4     8
#define ADDR_F204_4     ADDR_F203_4+LENmax_F203_4
//属性4∷=array 直流模拟量配置
//直流模拟量配置∷=structure
//{
//  量程起始值  double-long，
//  量程结束值  double-long，
//  换算及单位  Scaler_Unit
//}
#define NUMmax_F204_4     NUMmax_DC
#define LENmax_F204_4     (2+(15*NUMmax_F204_4))
#define ADDR_F205_127     ADDR_F204_4+LENmax_F204_4
//方法127：修改开关属性（继电器号，开关属性）
//∷=structure
//{
//继电器号∷=OAD
//开关属性∷=enum{脉冲式（0），保持式（1）}，
//}
#define LENper_F205_127     9
#define NUMmax_F205_127     NUMmax_CR
#define LENmax_F205_127     (LENper_F205_127*NUMmax_F205_127)
#define ADDR_F206_4     ADDR_F205_127+LENmax_F205_127//告警输出
//属性4∷=array 允许告警时段
//允许告警时段∷=structure
//{
//  起始时间  Time，
//  结束时间  Time
//}
#define NUMmax_F206_4     12
#define LENmax_F206_4     (2+(10*NUMmax_F206_4))
#define ADDR_F207_127     ADDR_F206_4+LENmax_F206_4
//方法127：修改工作模式（路号，工作模式）
//∷=structure
//{
//路号∷=OAD
//端子功能∷=enum
//{
//     秒脉冲输出（0），
//     需量周期  （1），
//     时段投切  （2）
//}
//}
#define LENper_F207_127     9
#define NUMmax_F207_127     1
#define LENmax_F207_127     (LENper_F207_127*NUMmax_F207_127)
#define ADDR_F209_6     ADDR_F207_127+LENmax_F207_127//载波/微功率无线接口
//属性6（从节点对象列表更新周期）∷= TI
//更新周期：指从节点对象列表、网络拓扑信息、多网信息等更新周期
#define LENmax_F209_6     4
#define ADDR_F209_9     ADDR_F209_6+LENmax_F209_6//载波/微功率无线接口
//属性 9（宽带载波频段序号）∷ =unsigned
#define LENmax_F209_9     2

#define ADDR_F209_11 			ADDR_F209_9+LENmax_F209_9
//属性 9（台区识别启停标记）∷ =unsigned
#define LENmax_F209_11     2

#define ADDR_F209_22 			ADDR_F209_11+LENmax_F209_11
//属性 9（拒绝节点上报使能标志）∷ =enum
//enum{禁止（0），使能（1）}
#define LENmax_F209_22     2


#define ADDR_F209_128     ADDR_F209_22+LENmax_F209_22//载波/微功率无线接口
//方法128：配置端口参数（端口号，通信参数）
//∷=structure
//{
//端口号∷=OAD
//端口参数   COMDCB，
//}
#define LENper_F209_128     13
#define NUMmax_F209_128     1
#define LENmax_F209_128     (LENper_F209_128*NUMmax_F209_128)
#define ADDR_F20B_127     ADDR_F209_128+LENmax_F209_128//蓝牙模块
//方法127：配置端口（端口号，通信参数）
//∷=structure
//{
//端口号∷=OAD
//端口参数   COMDCB，
//}
#define LENper_F20B_127     13
#define NUMmax_F20B_127     1
#define LENmax_F20B_127     (LENper_F20B_127*NUMmax_F20B_127)




#define ADDR_F001_4     ADDR_F20B_127+LENmax_F20B_127//文件传输块状态字(仅在工厂模式时会清0)
//属性4（传输块状态字，只读）∷=bit-string
//按bit位标识每个数据块的传输状态，bit0代表第一个数据块，依次类推。bitN=0，表示未传输，bitN=1，表示传输成功（N从0~总传输块数-1）。

#define LENmax_F001_4     256
#define ADDR_F001_7     ADDR_F001_4+LENmax_F001_4//文件分块传输管理(仅在工厂模式时会清0)
//方法7：启动传输（参数）
//参数∷=structure
//{
//	文件信息∷=structure
//	{
//		源文件    visible-string，
//		目标文件  visible-string，//目标文件：文件路径及名称；文件路径及名称格式：“[/路径/]文件名” ，如果设备不支持文件管理，可设置为空串。
//		文件大小  double-long-unsigned，
//		文件属性  bit-string(SIZE(3))，
							//bit0：读（1：可读，0：不可读）
							//bit1：写（1：可写，0：不可写）
							//bit2：执行（1：可执行，0：不可执行）
//		文件版本  visible-string
//		文件类别  enum
//		{
//			终端文件          （0），
//			本地通信模块文件  （1），
//			远程通信模块文件  （2），
//			采集器文件        （3），
//			从节点通信模块文件（4），
//			其它文件          (255)
//		}
//	}
//	传输块大小  long-unsigned，
//	校验∷=structure
//	{
//		校验类型  enum
//		{
//    	CRC校验（默认）	（0），
//			md5校验			（1），
//			SHA1校验			（2），
//			其他				（255）
//		}，
//  	校验值  octet-string
//	}
//}
//文件信息数据结构见表173　属性2；
//校验数据结构见表174　方法4。
#define LENmax_F001_7     128
#define ADDR_F001_END     ADDR_F001_7+LENmax_F001_7


//总加组
#define ADDR_230x_2     ADDR_F001_END
//总加配置表∷=array 总加组配置单元//清空时数组数=0
//总加配置单元∷=structure
//{
//参与总加的分路通信地址  TSA，
//总加标志	            enum{正向（0），反向（1）}，
//运算符标志	            enum{加（0），减（1）}
//}
#define LENper_230x_2     (2+(15*8))
#define LENmax_230x_2     (LENper_230x_2*NUMmax_TAG)

#define ADDR_230x_13     ADDR_230x_2+LENmax_230x_2
//总加组滑差时间周期	unsigned（单位：分）
#define LENper_230x_13     2
#define LENmax_230x_13     (LENper_230x_13*NUMmax_TAG)

#define ADDR_230x_14     ADDR_230x_13+LENmax_230x_13
//总加组功控轮次配置
//功控轮次标识位∷=bit-string（SIZE(8)）
//功控轮次标识位：bit0～bit7按顺序对位表示第1～第8轮次开关的受控设置，置“1”表示该轮次开关受控，置“0”表示不受控
#define LENper_230x_14     3
#define LENmax_230x_14     (LENper_230x_14*NUMmax_TAG)

#define ADDR_230x_15     ADDR_230x_14+LENmax_230x_14
//总加组电控轮次配置
//电控轮次标识位∷=bit-string（SIZE(8)）
//电控轮次标识位：bit0～bit7按顺序对位表示第1～第8轮次开关的受控设置，置“1”表示该轮次开关受控，置“0”表示不受控
#define LENper_230x_15     3
#define LENmax_230x_15     (LENper_230x_15*NUMmax_TAG)


//脉冲计量
#define ADDR_240x_2     ADDR_230x_15+LENmax_230x_15
//通信地址∷=octet-string
#define LENper_240x_2     18
#define LENmax_240x_2     (LENper_240x_2*NUMmax_IMPIN)
#define ADDR_240x_3     ADDR_240x_2+LENmax_240x_2
//互感器倍率∷=structure
//{
//PT  long-unsigned，
//CT  long-unsigned
//}
#define LENper_240x_3     8
#define LENmax_240x_3     (LENper_240x_3*NUMmax_IMPIN)
#define ADDR_240x_4     ADDR_240x_3+LENmax_240x_3
//脉冲配置∷=array 脉冲单元//删除时数组数=0
//脉冲单元∷=structure
//{
//脉冲输入端口号	OAD，
//脉冲属性		enum
//{
//	 正向有功（0），
//	 正向无功（1），
//	 反向有功（2），
//	 反向无功（3）
//}，
//脉冲常数k     long-unsigned
//}
#define LENper_240x_4     (2+12)
#define LENmax_240x_4     (LENper_240x_4*NUMmax_IMPIN)


//参数扩展
#define ADDR_4520_2     ADDR_240x_4+LENmax_240x_4//公网远程通信多接入点备用通道
//属性2∷=array 备用通道 //数组数=0表示无效
//备用通道∷=structure 
//{ 
//	运营商 enum
//	{ 
//		CMCC（移动） = 0， 
//		CTCC（电信） = 1， 
//		CUCC（联通） = 2， 
//		未知 = 255 
//	}， 
//	网络类型 enum 
//	 2G = 0，3G = 1， 4G = 2， 未知 = 255 ， 
//	APN visible-string， 
//	用户名 visible-string， 
//	密码 visible-string， 
//	代理服务器地址 octet-string， 
//	代理端口 long-unsigned， 
//	主站通信参数 array structure 
//	{ 
//		IP地址 octet-string， 
//		端口 long-unsigned 
//	} 
//} 
#define LENmax_4520_2     1024
#define ADDR_3411     ADDR_4520_2+LENmax_4520_2//电表接线错误事件
//有效标识∷=bool
//上报标识∷=enum
//{
//不上报（0），
//事件发生上报（1），
//事件恢复上报（2），
//事件发生恢复均上报（3）
//}
//关联对象属性表∷=array OAD
#define ADDR_3411_6     ADDR_3411+4+LENmax_EventOAD
//属性6（配置参数）∷=structure
//{
//}
#define LENmax_3411_6     2

#define ADDR_8002_3     ADDR_3411_6+LENmax_3411_6//催费告警参数
//属性3（催费告警参数）∷=structure 
//{
//告警时段：octet-string(SIZE(3))， 
//告警信息：visible-string(SIZE(1...200)) 
//} 
#define LENmax_8002_3     216


//---福建增补---
#if (USER/100)==14
#define ADDR_6E00     ADDR_8002_3+LENmax_8002_3//(福建)外部数据指的是除交采和终端本体类数据通过端口交换的数据
//属性2（外部设备数据采集模式）∷= enum
//{
//国网协议（0），
//福建增补（1）
//}
#define LENmax_6E00     2
#define ADDR_6E01     ADDR_6E00+LENmax_6E00//(福建)交采应能支持原终端的采集模式（原终端为376的应能支持测量点模式或698.45协议模式，698.45的终端应能支持原698.45协议方式采集）
//属性2（交采和终端本体数据采集模式）∷= enum
//{
//376模式      （0），
//698.45模式   （1）
//}
//支持两种模式的可进行切换，只支持一种的该参数应为只读
#define ADDR_6E02     ADDR_6E01+2//(福建)加密芯片类型,只读
//属性2（加密芯片类型,只读）∷= enum
//{
//2013版  （0），
//698.45版（1）
//}
#define LENmax_6E01     2
#define ADDR_6E11     ADDR_6E02+LENmax_6E01//(福建)全局透明任务方案集
//属性2（配置表）∷=array全局透明任务方案单元(对应任务方案参数有变更的应立即按新参数开始任务调度，没有变更的不应重新执行任务)
//6E12 属性2（任务配置单元）∷=structure
//{ 
//	透明任务方案号  long-unsigned，
//	端口            OAD，
//	任务类别        structure
//	{
//    任务类别   unsigned
//		附加参数   data
//	}，
//	执行优先级      unsigned，
//	执行独占性      bool
//	{
//		不独占（0），独占（1）
//	}，
//	开始时间             date_time_s，
//	结束时间             date_time_s，
//	任务周期             TI，
//	失败重试周期         TI，
//	每轮次失败重试次数   unsigned，
//	最大失败重试轮次     unsigned，
//	协议封拆包方式       enum
//	{
//	定义见说明
//	}，
//	多帧关联任务         bool
//	{不关联（0），关联（1）}，
//	通信对象类型          unsigned，
//	数据存储次数          long-unsigned，
//	上报方案编号          unsigned，
//	数据加工方式         structure
//	{
//		加工方式           unsigned
//		数据加工附加参数   data
//	}，
//	任务开始前脚本id    long-unsigned，
//	任务完成后脚本id    long-unsigned
//}
//数据加工方式格式：
//加工方式	加工处理方式	表示
//0	NULL	不加工
//其他方式：待后续扩展
//属性3（配置统计表，只读）∷=array 配置统计单元
//配置统计单元）∷=structure
//{
//	透明任务方案号       long-unsigned，
//	端口                 OAD，
//	执行优先级           unsigned，
//	执行独占性           bool，
//	已配置具体任务总数量 long-unsigned，
//	最小具体任务序号     long-unsigned，
//	最大具体任务序号     long-unsigned
//}
//方法127：Add（array 透明任务方案单元）添加或更新一组透明任务方案单元，方案参数有变化的应清除其数据记录，重新开始任务调度。
//方法128：Delete（array 透明任务方案号）按任务方案号删除一组透明任务方案单元、具体任务配置及其数据记录。
//方法129：Clear (参数) 参数∷=NULL 清空透明任务方案配置表、具体任务配置及其数据记录。
//方法130：Update（array 参数）
//参数∷=structure 
//{
//透明任务方案号  long-unsigned，
//执行优先级      unsigned
//}
//按新的策略更新任务状态。0删除该方案及其具体任务和数据，254为暂停任务，255为重启任务（任务方案优先级不变，执行清除当前周期任务记录，重新开始执行）或触发一次冻结。
#define LENper_6E11     256
#define LENmax_6E11     (LENper_6E11*RMTASKmax)
#define ADDR_6E13     ADDR_6E11+LENmax_6E11//(福建)全局透明具体任务集
//属性2∷=array 全局透明具体任务单元
//6E14 属性2（透明具体任务单元）∷=structure
//{
//	透明任务方案号    long-unsigned，
//	具体任务序号      long-unsigned，
//	通信地址集合      MS，
//	端口通信控制块    COMDCB，
//	请求报文格式字    enum
//	{ 
//	定义见附录
//	}，
//	请求报文内容      octet-string，
//	响应报文格式字    enum
//	{ 
//	定义见附录
//	}，
//	响应报文预计最长长度     long-unsigned，
//	成功响应报文最短有效长度 unsigned，
//	返回报文验证方式  structure
//	{
//		验证方式        unsigned，
//	验证参数        data，
//	触发任务方案号  long-unsigned，
//	}
//}
//返回报文验证方式：
//验证处理方式	验证参数	表示
//0	NULL	不验证
//1	报文比对参数	报文比对不一致验证
//2	报文比对参数	报文比对比对一致验证
//报文比对参数∷=structure
//{
//    特征字节      octet-string，
//    截取开始      long-unsigned，
//    截取长度      long-unsigned
//}
//截取开始字节在特征字节后开始算，最小值为1

//方法127：Add（array 全局透明具体任务单元）
//添加更新一个或一组透明具体任务, 请求报文参数（除通信地址集合MS外）有变更的应删除原有执行记录重新执行，没有变更的不应重复执行，有增加表计的新表计任务应自动增加执行。
//方法128：Delete（array 参数）
//参数∷=structure
//{
//  透明任务方案号long-unsigned，
//  通信地址集合   MS
//}
//按透明任务方案号和地址删除一组透明具体任务。
//方法129：Delete（array透明任务方案号）
//按透明任务方案号删除一组透明具体任务。
//方法130：Clear(参数)
//参数∷=NULL
//清空透明具体任务集。
#define LENper_6E13     256
#define LENmax_6E13     (LENper_6E13*NUMmax_6014)
#define ADDR_6E17     ADDR_6E13+LENmax_6E13//(福建)上报方案集
//属性2∷=array 上报方案单元
//6E18 属性2（上报方案 report plan）∷=structure
//{
//上报方案编号      unsigned，
//上报通道          array OAD，
//上报优先级        unsigned，
//上报响应超时时间  TI，
//最大上报次数      unsigned，
//结果验证标识      enum
//{
//直接上报（0），验证通过上报（1），验证不通过上报（2）
//}，
//上报时刻       unsigned，   
//{
//N秒内数据合并上报（N），
//等待主站补报命令（255）
//}，
//上报内容          strcuture
//{
//  类型  unsigned，
//  数据  Data
//}
//}
//上报内容：
//上报类型	上报内容	表示
//0	OAD	对象属性数据
//1	RecordData	上报记录型对象属性
//RecordData∷=structure
//{
//  主对象属性描述符      OAD，
//记录型对象属性描述符  RCSD，
//行选择                RSD
//}
//方法127：Add（array 上报方案单元）
//添加或更新一组上报方案单元。
//方法128：Delete（array 上报方案编号）
//删除一组上报方案单元。
//方法129：Clear(参数)
//参数∷=NULL
//清空上报方案集。
#define LENper_6E17     512
#define LENmax_6E17     (LENper_6E17*NUMmax_601C)
#define ADDR_6E30     ADDR_6E17+LENmax_6E17//(福建)主站穿透命令优先级
//属性2（主站穿透命令优先级）∷=array穿透任务优先级
//穿透任务优先级∷=structure
//{ 
//端口            OAD， 
//穿透优先级      unsigned，
//}
#define LENmax_6E30     64
#define ADDR_6E33     ADDR_6E30+LENmax_6E30//(福建)主动上报事件上报策略
//属性2（事件上报方案 report plan）∷=structure
//{
//上报通道          array OAD，
//上报优先级        unsigned，
//上报响应超时时间  TI，
//最大上报次数      unsigned
//}
#define LENmax_6E33     32

#define ADDR_DL698_SET_END     ADDR_6E33+LENmax_6E33
#endif//#if (USER/100)==14
//---福建增补 END---

//---安徽增补---
#if (USER/100)==15//安徽
	#define ADDR_3106_11     ADDR_8002_3+LENmax_8002_3
	#define LENmax_3106_11		4
	#define ADDR_DL698_SET_END     ADDR_3106_11+LENmax_3106_11
#endif
//---安徽增补 END---

//------------
	#define ADDR_4048_127		 ADDR_8002_3+LENmax_8002_3
	#define LENmax_4048_127		8
	#define ADDR_DL698_SET_END     ADDR_4048_127+LENmax_4048_127
//END---

#ifndef ADDR_DL698_SET_END
#define ADDR_DL698_SET_END     ADDR_8002_3+LENmax_8002_3
#endif


#if ((ADDR_DL698_SET_END)-(ADDR_DL698_SET_START))>LEN_FLASH_DL698_SET
	#error LEN_FLASH_DL698_SET
#endif




#ifdef __cplusplus
 }
#endif
#endif










