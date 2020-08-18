

#ifndef TYPE_H
#define TYPE_H


typedef unsigned char  uchar; /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  schar;  /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short ushort;/* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short sshort; /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   uint;/* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   sint; /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
#ifndef _LINUX_TYPES_H
//typedef unsigned long long  ulong;
#endif
typedef float          f32;  /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         f64;  /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */

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












//�Ǳ�07ͨ�ű�ʶ�����ݽṹ
typedef __packed struct
{
  vu32 DI;//ͨ�ű�ʶ
  vu8 Inum;//(1-255)��������
  vu8 Cnum;//(1-255)����
  vu8 num;//ÿ��ʶռ�б����(1-255);�����=��������*����*ÿ����ռ�б����
  vu8 inf;//B1B0���ڱ�ʶ�仯�ֽں�(0-3),B3B2������仯�ֽں�(0-3);B5B4=0ͨ�ź���ʾ,=1��ͨ��,=2����ʾ,3=��;B7B6֧�ֱ��0=֧��,1=Զ��,2=����
#ifndef LINUX_CROSS_GCC
}Meter07Lib_DI_TypeDef;
#else
}__attribute__ ((packed)) Meter07Lib_DI_TypeDef;
#endif
//�Ǳ�07����ָ������ݽṹ
typedef __packed struct
{
  vu32 addr;//��1�������ڴ������о��Ե�ַ
  vu16 Tnum;//��ָ���������������(1-65535)
  vu16 Cbyte;//����������ѭ������Ե�ַ����ֽ���(�����<65535)
  vu16 num;//��ָ�����������������(�����)
  vu16 byte;//����ÿ�����ݾ��Ե�ַ����ֽ���(�����<65535)
  vu16 rwbyte;//ÿ������Ҫ��д�����ֽ���(1-65535)
  vu16 uartbyte;//ÿ������RS485ͨ��ʱ�ֽ���(1-65535)
  vu8 type;//B0-B3��ʾ����ʱ��������0=ѹBCD,1=��BCD,2=HEX,3=ASCII,4=�з���ѹBCD,5=HEX����,(������ʾ��ͨ��ʱתΪѹBCD��ʽ���з�����Ϊ�з��ŵ�ѹBCD);B4-B7����
  vu8 dp;//B0-B3��ʾÿ�����ݴ���ʱС��λ��,B4-B7��ʾͨ��ʱС��λ��
  vu8 lcdbitnum;//B0-B3��ʾ����ʾ����λ��,B4-B7��ʾ����ʾС��λ��(0-9,10=�������趨,11=�������趨,12-15����)
  vu8 unit;//���ݵ�λ0=kWh,1=kvarh,2=kW,3=kvar,4=V,5=A,6=����ʱ��,7=��������,8=ʱ����,9=��,10=8״̬��,11=���,12=��ʱ,13=��,14=����,15=λ��,16=p/kWh,17=p/kvarh,18=�¶�,19=��,20=��,21=�޵�λ,22=�Ƕ�,23=%,24=����,25=Ƶ��,26=���־,27=��ʱ��,28=ʱ��,29=ʣ�����(ʽ03),30=kVAh,31=�й���ʱ,32=kVA,33=YMDHM,34=YMDHMS,35=ASCII,36=16״̬��,37=�޹���ʱ,38=������ʱ,39=YMDN,40=DI,41=ʱ��,42=Ah,43=%UB,44=%IB
  vu8 loop;//��ѭ���忪ʼ(!=0ֵ1-253)����(2��0xFE,1��0xFF)��־,1��2��ͬ�������0xFF,���ֻ��2��
  vu8 key;//B0-B1��ʾ��д0=ֻ��,1=��д,2=ֻд;B2-B3����;B4-B7��ʾдʱ����Ȩ��2,4,8(98),9(99)
#ifndef LINUX_CROSS_GCC
}Meter07Lib_POINT_TypeDef;
#else
}__attribute__ ((packed)) Meter07Lib_POINT_TypeDef;
#endif
//�Ǳ�07SEGLCD�����ݽṹ
typedef __packed struct
{
  vu16 Tnum;//�������ʽ��������������
  vu16 num;//���ڰ�����������(�����)
  vu8 month;//����(��˳��ֵ)=0xFF��ʾ��ָ�벻�����·�,����0xFFֵ��ʾ��ʼ�·�(0,1,2,...�ֱ��ʾ��ǰ,��1��,��2��,...)
  vu8 tariff;//����(����˳��ֵ)=0xFF��ʾ��ָ�벻��������,����0xFFֵ��ʾ��ʼ����(0,1,2,3,4�ֱ��ʾ��,��,��,ƽ,��)
  vu8 add;//���и���˵����,����˵���������������������0,���з���ʱΪÿ����������
  vu8 bin1;//B7-B0='��ǰ''��''��''���''��''��''��''��'
  vu8 bin2;//B7-B0='��''��''��''��''��''��''��''��'
  vu8 bin3;//B7-B0='��''ƽ''��''��''��''��''��''COS'
  vu8 bin4;//B7-B0='��''����''ʣ��'��''��''��''��''��'
  vu8 bin5;//B7-B0='��''ʧ''ѹ''��''����''ʱ''��''��'
  vu8 bin6;//B7-B0='��1''��2''��''Ԫ''��''V''V''��'
  vu8 bin7;//B7-B0='��''��''var''��''�ڻ�''ʱ��''�ֵ�''������'
#ifndef LINUX_CROSS_GCC
}Meter07Lib_SEGLCD_TypeDef;
#else
}__attribute__ ((packed)) Meter07Lib_SEGLCD_TypeDef;
#endif

#endif  /* __TYPE_H__ */
