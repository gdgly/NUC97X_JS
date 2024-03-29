

#ifndef STM32F4xx_IO_H
#define STM32F4xx_IO_H
#ifdef __cplusplus
 extern "C" {
#endif

void Pin_CLK_ENABLE(u32 PinName);//允许引脚口时钟
void Pin_Configure(u32 PinName,u32 Configure);//引脚配置
void MulPin_Configure(u32 * List);//引脚列表配置,入口列表指针
void Pin_OutH(u32 PinName);//引脚输出高,入口引脚使用名
void Pin_OutL(u32 PinName);//引脚输出低,入口引脚使用名
u32 Pin_Read(u32 PinName);//读引脚,入口引脚使用名;返回0或1
void Init_PIN_SPACE(void);//初始化未使用引脚

void OnIO3V3(void);//打开IO3V3电源
void OffIO3V3(void);//关IO3V3电源
void OnEMU(void);//打开EMU(复位解除)
void OffEMU(void);//关EMU(复位)
void OnA5V0(void);//打开A5V0电源
void OffA5V0(void);//关A5V0电源

void OnBACKLIGHT(void);//打开BACKLIGHT电源
void OffBACKLIGHT(void);//关BACKLIGHT电源
void OnLED3V3(void);//打开LED3V3电源
void OffLED3V3(void);//关LED3V3电源
void OnE5V0(void);//打开E5V0电源
void OffE5V0(void);//关E5V0电源
void On2E5V0(void);//打开E5V0电源
void Off2E5V0(void);//关E5V0电源
void OnR5V0(void);//打开R5V0电源
void OffR5V0(void);//关R5V0电源
void OnUSB5V0(void);//打开USB5V0电源
void OffUSB5V0(void);//关USB5V0电源




#ifdef __cplusplus
 }
#endif 
#endif



