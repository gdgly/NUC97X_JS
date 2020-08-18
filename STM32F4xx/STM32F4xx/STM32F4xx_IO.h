

#ifndef STM32F4xx_IO_H
#define STM32F4xx_IO_H
#ifdef __cplusplus
 extern "C" {
#endif

void Pin_CLK_ENABLE(u32 PinName);//�������ſ�ʱ��
void Pin_Configure(u32 PinName,u32 Configure);//��������
void MulPin_Configure(u32 * List);//�����б�����,����б�ָ��
void Pin_OutH(u32 PinName);//���������,�������ʹ����
void Pin_OutL(u32 PinName);//���������,�������ʹ����
u32 Pin_Read(u32 PinName);//������,�������ʹ����;����0��1
void Init_PIN_SPACE(void);//��ʼ��δʹ������

void OnIO3V3(void);//��IO3V3��Դ
void OffIO3V3(void);//��IO3V3��Դ
void OnEMU(void);//��EMU(��λ���)
void OffEMU(void);//��EMU(��λ)
void OnA5V0(void);//��A5V0��Դ
void OffA5V0(void);//��A5V0��Դ

void OnBACKLIGHT(void);//��BACKLIGHT��Դ
void OffBACKLIGHT(void);//��BACKLIGHT��Դ
void OnLED3V3(void);//��LED3V3��Դ
void OffLED3V3(void);//��LED3V3��Դ
void OnE5V0(void);//��E5V0��Դ
void OffE5V0(void);//��E5V0��Դ
void On2E5V0(void);//��E5V0��Դ
void Off2E5V0(void);//��E5V0��Դ
void OnR5V0(void);//��R5V0��Դ
void OffR5V0(void);//��R5V0��Դ
void OnUSB5V0(void);//��USB5V0��Դ
void OffUSB5V0(void);//��USB5V0��Դ




#ifdef __cplusplus
 }
#endif 
#endif


