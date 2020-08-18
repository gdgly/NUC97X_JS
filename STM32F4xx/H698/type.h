

#ifndef TYPE_H
#define TYPE_H


typedef unsigned char  uchar; /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  schar;  /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short ushort;/* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short sshort; /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint;/* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   sint; /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
#ifndef _LINUX_TYPES_H
//typedef unsigned long long  ulong;
#endif
typedef float          f32;  /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         f64;  /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

#ifndef __ASM_ARM_TYPES_H
typedef signed long long s64;
typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;
#endif

typedef signed long long const sc64;  /* Read Only */
typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long long vs64;
typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

#ifndef __ASM_ARM_TYPES_H
typedef unsigned long long u64;
typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#endif

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long long vu64;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */












//仪表07通信标识库数据结构
typedef __packed struct
{
  vu32 DI;//通信标识
  vu8 Inum;//(1-255)组内条数
  vu8 Cnum;//(1-255)组数
  vu8 num;//每标识占有编号数(1-255);编号数=组内条数*组数*每命令占有编号数
  vu8 inf;//B1B0组内标识变化字节号(0-3),B3B2组命令变化字节号(0-3);B5B4=0通信和显示,=1仅通信,=2仅显示,3=否;B7B6支持表计0=支持,1=远程,2=本地
#ifndef LINUX_CROSS_GCC
}Meter07Lib_DI_TypeDef;
#else
}__attribute__ ((packed)) Meter07Lib_DI_TypeDef;
#endif
//仪表07数据指针库数据结构
typedef __packed struct
{
  vu32 addr;//第1条数据在存贮体中绝对地址
  vu16 Tnum;//本指针包含数据总条数(1-65535)
  vu16 Cbyte;//组与组间或外循环体绝对地址相隔字节数(正相隔<65535)
  vu16 num;//本指针包含组内数据条数(编号数)
  vu16 byte;//组内每条数据绝对地址相隔字节数(正相隔<65535)
  vu16 rwbyte;//每条数据要读写数据字节数(1-65535)
  vu16 uartbyte;//每条数据RS485通信时字节数(1-65535)
  vu8 type;//B0-B3表示存贮时数据类型0=压BCD,1=单BCD,2=HEX,3=ASCII,4=有符号压BCD,5=HEX补码,(数据显示或通信时转为压BCD方式若有符号则为有符号的压BCD);B4-B7保留
  vu8 dp;//B0-B3表示每条数据存贮时小数位数,B4-B7表示通信时小数位数
  vu8 lcdbitnum;//B0-B3表示需显示整数位数,B4-B7表示需显示小数位数(0-9,10=按电量设定,11=按功率设定,12-15保留)
  vu8 unit;//数据单位0=kWh,1=kvarh,2=kW,3=kvar,4=V,5=A,6=月日时分,7=年月日周,8=时分秒,9=分,10=8状态字,11=表号,12=日时,13=日,14=毫秒,15=位数,16=p/kWh,17=p/kvarh,18=温度,19=秒,20=次,21=无单位,22=角度,23=%,24=费率,25=频率,26=相标志,27=日时分,28=时段,29=剩余电量(式03),30=kVAh,31=有功需时,32=kVA,33=YMDHM,34=YMDHMS,35=ASCII,36=16状态字,37=无功需时,38=视在需时,39=YMDN,40=DI,41=时区,42=Ah,43=%UB,44=%IB
  vu8 loop;//外循环体开始(!=0值1-253)结束(2级0xFE,1级0xFF)标志,1级2级同点结束用0xFF,最多只有2级
  vu8 key;//B0-B1表示读写0=只读,1=读写,2=只写;B2-B3保留;B4-B7表示写时密码权限2,4,8(98),9(99)
#ifndef LINUX_CROSS_GCC
}Meter07Lib_POINT_TypeDef;
#else
}__attribute__ ((packed)) Meter07Lib_POINT_TypeDef;
#endif
//仪表07SEGLCD库数据结构
typedef __packed struct
{
  vu16 Tnum;//本标题格式包含数据总条数
  vu16 num;//组内包含数据条数(编号数)
  vu8 month;//月数(组顺序值)=0xFF表示本指针不包含月份,不等0xFF值表示开始月份(0,1,2,...分别表示当前,上1月,上2月,...)
  vu8 tariff;//费率(组内顺序值)=0xFF表示本指针不包含费率,不等0xFF值表示开始费率(0,1,2,3,4分别表示总,尖,峰,平,谷)
  vu8 add;//下有附加说明数,附加说明中其包含数据总条数用0,当有费率时为每费率中条数
  vu8 bin1;//B7-B0='当前''上''月''组合''反''正''向''无'
  vu8 bin2;//B7-B0='有''功''Ⅰ''Ⅰ''Ⅰ''Ⅴ''总''尖'
  vu8 bin3;//B7-B0='峰''平''谷''Ａ''Ｂ''Ｃ''Ｎ''COS'
  vu8 bin4;//B7-B0='Φ''阶梯''剩余'需''电''量''费''价'
  vu8 bin5;//B7-B0='空''失''压''流''功率''时''间''段'
  vu8 bin6;//B7-B0='△1''△2''万''元''Ｋ''V''V''Ａ'
  vu8 bin7;//B7-B0='ｈ''ｋ''var''ｈ''囤积''时点''分点''正负号'
#ifndef LINUX_CROSS_GCC
}Meter07Lib_SEGLCD_TypeDef;
#else
}__attribute__ ((packed)) Meter07Lib_SEGLCD_TypeDef;
#endif

#endif  /* __TYPE_H__ */
