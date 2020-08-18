
;KEIL
;EQU 给数字常量取一个符号名，相当于C语言中的define
;AREA 用于定义一个代码段或数据段，段名若以数字开头，则该段名需用“|”括起来，如|1_test| 。
;SPACE 分配内存空间
;PRESERVE8 当前文件堆栈需按照8字节对齐
;REQUIRE8 指定当前文件要求堆栈八字节对齐
;THUMB 表示后指令兼容THUMB。THUMB是ARM以前指令集，16bit,现在Cortex-M系列的都使用THUMB-2指令集
;EXPORT 声明一个标号具有全局属性，可被外部的文件使用
;DCD 以字为单位分配内存，要求4字节对齐，并要求初始化这些内存
;PROC 定义子程序，与ENDP成对使用，表示子程序结束
;WEAK 弱定义，如果外部文件声明了一个标号，则优先使用外部文件定义的标号，如果外部文件没有定义也不出错。要注意的是：这个不是ARM的指令，是编译器的，这里放在一起只是为了方便。
;IMPORT 声明标号来自外部文件，跟C语言中的EXTERN关键字类似
;B 跳转到一个标号
;ALIGN 对齐，通常需要跟一个立即数(如:ALIGN=3 2^3=8BYTE)，缺省表示4对齐，要注意的是：这个不是ARM的指令，是编译器的，这里放在一起只是为了方便。
;END 文件结束

;IAR 
;关于段控制指令：SECTION section :type [flag] [(align)]
;section 段名
;type 存储类型CODE CONST DATA
;flag
	;	ROOT, NOROOT
	;ROOT (the default mode) indicates that the section fragment must not be discarded. 
	;NOROOT means that the section fragment is discarded by the linker if no symbols in this section fragment are referred to. Normally, all section fragments except startup code and interrupt vectors should set this flag.  
	;	REORDER, NOREORDER
	;NOREORDER (the default mode) starts a new fragment in the section with the given name, or a new section if no such section exists.
	;REORDER starts a new section with the given name. 
;(align) 对齐



;//程序入口
	extern Terminal_Main;
		
                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     0;__initial_sp               ;0 Top of Stack
                DCD     Terminal_Main;//Reset_Handler              ;1 Reset Handler
                DCD     NMI_Handler                ;2 NMI Handler
                DCD     HardFault_Handler          ;3 Hard Fault Handler
                DCD     MemManage_Handler          ;4 MPU Fault Handler
                DCD     BusFault_Handler           ;5 Bus Fault Handler
                DCD     UsageFault_Handler         ;6 Usage Fault Handler
                DCD     0                          ;7 Reserved
                DCD     0                          ;8 Reserved
                DCD     0                          ;9 Reserved
                DCD     0                          ;10 Reserved
                DCD     SVC_Handler                ;11 SVCall Handler
                DCD     DebugMon_Handler           ;12 Debug Monitor Handler
                DCD     0                          ;13 Reserved
                DCD     PendSV_Handler             ;14 PendSV Handler
                DCD     SysTick_Handler            ;15 SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler                   ;16 Window WatchDog                                        
                DCD     PVD_IRQHandler                    ;17 PVD through EXTI Line detection                        
                DCD     TAMP_STAMP_IRQHandler             ;18 Tamper and TimeStamps through the EXTI line            
                DCD     RTC_WKUP_IRQHandler               ;19 RTC Wakeup through the EXTI line                       
                DCD     FLASH_IRQHandler                  ;20 FLASH                                           
                DCD     RCC_IRQHandler                    ;21 RCC                                             
                DCD     EXTI0_IRQHandler                  ;22 EXTI Line0                                             
                DCD     EXTI1_IRQHandler                  ;23 EXTI Line1                                             
                DCD     EXTI2_IRQHandler                  ;24 EXTI Line2                                             
                DCD     EXTI3_IRQHandler                  ;25 EXTI Line3                                             
                DCD     EXTI4_IRQHandler                  ;26 EXTI Line4                                             
                DCD     DMA1_Stream0_IRQHandler           ;27 DMA1 Stream 0                                   
                DCD     DMA1_Stream1_IRQHandler           ;28 DMA1 Stream 1                                   
                DCD     DMA1_Stream2_IRQHandler           ;29 DMA1 Stream 2                                   
                DCD     DMA1_Stream3_IRQHandler           ;30 DMA1 Stream 3                                   
                DCD     DMA1_Stream4_IRQHandler           ;31 DMA1 Stream 4                                   
                DCD     DMA1_Stream5_IRQHandler           ;32 DMA1 Stream 5                                   
                DCD     DMA1_Stream6_IRQHandler           ;33 DMA1 Stream 6                                   
                DCD     ADC_IRQHandler                    ;34 ADC1, ADC2 and ADC3s                            
                DCD     CAN1_TX_IRQHandler                ;35 CAN1 TX                                                
                DCD     CAN1_RX0_IRQHandler               ;36 CAN1 RX0                                               
                DCD     CAN1_RX1_IRQHandler               ;37 CAN1 RX1                                               
                DCD     CAN1_SCE_IRQHandler               ;38 CAN1 SCE                                               
                DCD     EXTI9_5_IRQHandler                ;39 External Line[9:5]s                                    
                DCD     TIM1_BRK_TIM9_IRQHandler          ;40 TIM1 Break and TIM9                   
                DCD     TIM1_UP_TIM10_IRQHandler          ;41 TIM1 Update and TIM10                 
                DCD     TIM1_TRG_COM_TIM11_IRQHandler     ;42 TIM1 Trigger and Commutation and TIM11
                DCD     TIM1_CC_IRQHandler                ;43 TIM1 Capture Compare                                   
                DCD     TIM2_IRQHandler                   ;44 TIM2                                            
                DCD     TIM3_IRQHandler                   ;45 TIM3                                            
                DCD     TIM4_IRQHandler                   ;46 TIM4                                            
                DCD     I2C1_EV_IRQHandler                ;47 I2C1 Event                                             
                DCD     I2C1_ER_IRQHandler                ;48 I2C1 Error                                             
                DCD     I2C2_EV_IRQHandler                ;49 I2C2 Event                                             
                DCD     I2C2_ER_IRQHandler                ;50 I2C2 Error                                               
                DCD     SPI1_IRQHandler                   ;51 SPI1                                            
                DCD     SPI2_IRQHandler                   ;52 SPI2                                            
                DCD     USART1_IRQHandler                 ;53 USART1                                          
                DCD     USART2_IRQHandler                 ;54 USART2                                          
                DCD     USART3_IRQHandler                 ;55 USART3                                          
                DCD     EXTI15_10_IRQHandler              ;56 External Line[15:10]s                                  
                DCD     RTC_Alarm_IRQHandler              ;57 RTC Alarm (A and B) through EXTI Line                  
                DCD     OTG_FS_WKUP_IRQHandler            ;58 USB OTG FS Wakeup through EXTI line                        
                DCD     TIM8_BRK_TIM12_IRQHandler         ;59 TIM8 Break and TIM12                  
                DCD     TIM8_UP_TIM13_IRQHandler          ;60 TIM8 Update and TIM13                 
                DCD     TIM8_TRG_COM_TIM14_IRQHandler     ;61 TIM8 Trigger and Commutation and TIM14
                DCD     TIM8_CC_IRQHandler                ;62 TIM8 Capture Compare                                   
                DCD     DMA1_Stream7_IRQHandler           ;63 DMA1 Stream7                                           
                DCD     FMC_IRQHandler                    ;64 FMC                                             
                DCD     SDIO_IRQHandler                   ;65 SDIO                                            
                DCD     TIM5_IRQHandler                   ;66 TIM5                                            
                DCD     SPI3_IRQHandler                   ;67 SPI3                                            
                DCD     UART4_IRQHandler                  ;68 UART4                                           
                DCD     UART5_IRQHandler                  ;69 UART5                                           
                DCD     TIM6_DAC_IRQHandler               ;70 TIM6 and DAC1&2 underrun errors                   
                DCD     TIM7_IRQHandler                   ;71 TIM7                   
                DCD     DMA2_Stream0_IRQHandler           ;72 DMA2 Stream 0                                   
                DCD     DMA2_Stream1_IRQHandler           ;73 DMA2 Stream 1                                   
                DCD     DMA2_Stream2_IRQHandler           ;74 DMA2 Stream 2                                   
                DCD     DMA2_Stream3_IRQHandler           ;75 DMA2 Stream 3                                   
                DCD     DMA2_Stream4_IRQHandler           ;76 DMA2 Stream 4                                   
                DCD     ETH_IRQHandler                    ;77 Ethernet                                        
                DCD     ETH_WKUP_IRQHandler               ;78 Ethernet Wakeup through EXTI line                      
                DCD     CAN2_TX_IRQHandler                ;79 CAN2 TX                                                
                DCD     CAN2_RX0_IRQHandler               ;80 CAN2 RX0                                               
                DCD     CAN2_RX1_IRQHandler               ;81 CAN2 RX1                                               
                DCD     CAN2_SCE_IRQHandler               ;82 CAN2 SCE                                               
                DCD     OTG_FS_IRQHandler                 ;83 USB OTG FS                                      
                DCD     DMA2_Stream5_IRQHandler           ;84 DMA2 Stream 5                                   
                DCD     DMA2_Stream6_IRQHandler           ;85 DMA2 Stream 6                                   
                DCD     DMA2_Stream7_IRQHandler           ;86 DMA2 Stream 7                                   
                DCD     USART6_IRQHandler                 ;87 USART6                                           
                DCD     I2C3_EV_IRQHandler                ;88 I2C3 event                                             
                DCD     I2C3_ER_IRQHandler                ;89 I2C3 error                                             
                DCD     OTG_HS_EP1_OUT_IRQHandler         ;90 USB OTG HS End Point 1 Out                      
                DCD     OTG_HS_EP1_IN_IRQHandler          ;91 USB OTG HS End Point 1 In                       
                DCD     OTG_HS_WKUP_IRQHandler            ;92 USB OTG HS Wakeup through EXTI                         
                DCD     OTG_HS_IRQHandler                 ;93 USB OTG HS                                      
                DCD     DCMI_IRQHandler                   ;94 DCMI  
                DCD     0                                 ;95 Reserved				                              
                DCD     HASH_RNG_IRQHandler               ;96 Hash and Rng
                DCD     FPU_IRQHandler                    ;97 FPU
                DCD     UART7_IRQHandler                  ;98 UART7
                DCD     UART8_IRQHandler                  ;99 UART8
                DCD     SPI4_IRQHandler                   ;100 SPI4
                DCD     SPI5_IRQHandler                   ;101 SPI5
                DCD     SPI6_IRQHandler                   ;102 SPI6
                DCD     SAI1_IRQHandler                   ;103 SAI1
                DCD     LTDC_IRQHandler                   ;104 LTDC
                DCD     LTDC_ER_IRQHandler                ;105 LTDC error
                DCD     DMA2D_IRQHandler                  ;106 DMA2D
                DCD     QUADSPI_IRQHandler                ;107 QUADSPI
                DCD     DSI_IRQHandler                    ;108 DSI

	DCD 698;109 //协议:0=协议无关,376=1376.1协议,698=698.45协议,645=645-2007协议
	extern _0
	extern Init_Terminal;//启动初始化
	extern Init_Terminal_SoftwareUpdate;//软件更新初始化
	extern ADDR_Terminal_END
	extern ADDR_Terminal_VER;//版本信息
	extern DL698_INIT;//初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
	extern Init_ALLDATA;//终端全部数据初始化(清0)
	extern Event_31010200;//终端版本变更事件
		
  DCD __Vectors ; 110: 软件模块开始地址
  DCD ADDR_Terminal_END ; 111: 软件模块结束地址
	DCD Init_Terminal ; 112: 软件模块起动初始化入口
  DCD Init_Terminal_SoftwareUpdate; 113: 软件模块更新初始化入口
  DCD ADDR_Terminal_VER ; 114: 版本信息
  DCD DL698_INIT ; 115 菜单用参数初始化；初始化命令:0=否,1=硬件初始化,2=数据区初始化,3=参数及全体数据区初始化(即恢复至出厂配置),4=参数(除与系统主站通信有关的)及全体数据区初始化,5=参数及全体数据区初始化(即恢复至出厂配置)后硬件复位
  DCD Init_ALLDATA;//116 菜单用终端全部数据初始化(清0)
  DCD Event_31010200;/117 终端版本变更事件
  DCD 0;//118
  DCD 0;//119

;//在PC中进行替换
	DCD 0;//120//EnProject+(EnHV<<1)+(EnUSER<<2)+(EnSV<<3)+(EnMeter<<4);//软件更新时是否判别
	DCD 0;//121//Project;//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=,500=集中器Ⅱ型,600=,700=
	DCD 0;//122//HVER;//硬件版本号
	DCD 0;//123//USER;//用户标识
	DCD 0;//124//SVER;//软件版本号
	DCD 0;//125//SAME_MS+(SAME_OS<<1)+(SAME_Meter<<2)+(SAME_Terminal<<3)+(SAME_Menu<<4)+(SAME_Font<<5),//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Terminal,5=Font,6=,7=Router
	DCD 0;//126//DEBUG,// 0=发行,1=调试
	DCD 0;//127//MeterCtrl+(MeterType<<8)+(MeterSpec<<16);//电能表常数接线方式等电能表规格
	DCD 3;//128//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Test,5=Font,6=PACK,7=Router,8=TEST
	DCD 0;//129//KEIL直接写入=0,在PC中进行替换后下载=1


	extern netDHCP_Notify
	extern netFTPc_Process
	extern netFTPc_Notify
	extern TestReport;//生成测试报告文件
		
	DCD netDHCP_Notify;//130
	DCD netFTPc_Process;//131
	DCD netFTPc_Notify;//132
	DCD _0;//TestReport;//133 生成测试报告文件
	DCD _0;//134
	DCD _0;//135
	DCD _0;//136
	DCD _0;//137
	DCD _0;//138
	DCD _0;//139
		
  DCD _0 ;140
  DCD _0 ;141
  DCD _0 ;142
  DCD _0 ;143
  DCD _0 ;144
  DCD _0 ;145
  DCD _0 ;146
  DCD _0 ;147
  DCD _0 ;148
  DCD _0 ;149
			
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY



; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler                   [WEAK]                                        
                EXPORT  PVD_IRQHandler                    [WEAK]                      
                EXPORT  TAMP_STAMP_IRQHandler             [WEAK]         
                EXPORT  RTC_WKUP_IRQHandler               [WEAK]                     
                EXPORT  FLASH_IRQHandler                  [WEAK]                                         
                EXPORT  RCC_IRQHandler                    [WEAK]                                            
                EXPORT  EXTI0_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI1_IRQHandler                  [WEAK]                                             
                EXPORT  EXTI2_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI3_IRQHandler                  [WEAK]                                           
                EXPORT  EXTI4_IRQHandler                  [WEAK]                                            
                EXPORT  DMA1_Stream0_IRQHandler           [WEAK]                                
                EXPORT  DMA1_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream2_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream3_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream4_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  ADC_IRQHandler                    [WEAK]                         
                EXPORT  CAN1_TX_IRQHandler                [WEAK]                                                
                EXPORT  CAN1_RX0_IRQHandler               [WEAK]                                               
                EXPORT  CAN1_RX1_IRQHandler               [WEAK]                                                
                EXPORT  CAN1_SCE_IRQHandler               [WEAK]                                                
                EXPORT  EXTI9_5_IRQHandler                [WEAK]                                    
                EXPORT  TIM1_BRK_TIM9_IRQHandler          [WEAK]                  
                EXPORT  TIM1_UP_TIM10_IRQHandler          [WEAK]                
                EXPORT  TIM1_TRG_COM_TIM11_IRQHandler     [WEAK] 
                EXPORT  TIM1_CC_IRQHandler                [WEAK]                                   
                EXPORT  TIM2_IRQHandler                   [WEAK]                                            
                EXPORT  TIM3_IRQHandler                   [WEAK]                                            
                EXPORT  TIM4_IRQHandler                   [WEAK]                                            
                EXPORT  I2C1_EV_IRQHandler                [WEAK]                                             
                EXPORT  I2C1_ER_IRQHandler                [WEAK]                                             
                EXPORT  I2C2_EV_IRQHandler                [WEAK]                                            
                EXPORT  I2C2_ER_IRQHandler                [WEAK]                                               
                EXPORT  SPI1_IRQHandler                   [WEAK]                                           
                EXPORT  SPI2_IRQHandler                   [WEAK]                                            
                EXPORT  USART1_IRQHandler                 [WEAK]                                          
                EXPORT  USART2_IRQHandler                 [WEAK]                                          
                EXPORT  USART3_IRQHandler                 [WEAK]                                         
                EXPORT  EXTI15_10_IRQHandler              [WEAK]                                  
                EXPORT  RTC_Alarm_IRQHandler              [WEAK]                  
                EXPORT  OTG_FS_WKUP_IRQHandler            [WEAK]                        
                EXPORT  TIM8_BRK_TIM12_IRQHandler         [WEAK]                 
                EXPORT  TIM8_UP_TIM13_IRQHandler          [WEAK]                 
                EXPORT  TIM8_TRG_COM_TIM14_IRQHandler     [WEAK] 
                EXPORT  TIM8_CC_IRQHandler                [WEAK]                                   
                EXPORT  DMA1_Stream7_IRQHandler           [WEAK]                                          
                EXPORT  FMC_IRQHandler                    [WEAK]                                             
                EXPORT  SDIO_IRQHandler                   [WEAK]                                             
                EXPORT  TIM5_IRQHandler                   [WEAK]                                             
                EXPORT  SPI3_IRQHandler                   [WEAK]                                             
                EXPORT  UART4_IRQHandler                  [WEAK]                                            
                EXPORT  UART5_IRQHandler                  [WEAK]                                            
                EXPORT  TIM6_DAC_IRQHandler               [WEAK]                   
                EXPORT  TIM7_IRQHandler                   [WEAK]                    
                EXPORT  DMA2_Stream0_IRQHandler           [WEAK]                                  
                EXPORT  DMA2_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream2_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream3_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream4_IRQHandler           [WEAK]                                 
                EXPORT  ETH_IRQHandler                    [WEAK]                                         
                EXPORT  ETH_WKUP_IRQHandler               [WEAK]                     
                EXPORT  CAN2_TX_IRQHandler                [WEAK]                                               
                EXPORT  CAN2_RX0_IRQHandler               [WEAK]                                               
                EXPORT  CAN2_RX1_IRQHandler               [WEAK]                                               
                EXPORT  CAN2_SCE_IRQHandler               [WEAK]                                               
                EXPORT  OTG_FS_IRQHandler                 [WEAK]                                       
                EXPORT  DMA2_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream7_IRQHandler           [WEAK]                                   
                EXPORT  USART6_IRQHandler                 [WEAK]                                           
                EXPORT  I2C3_EV_IRQHandler                [WEAK]                                              
                EXPORT  I2C3_ER_IRQHandler                [WEAK]                                              
                EXPORT  OTG_HS_EP1_OUT_IRQHandler         [WEAK]                      
                EXPORT  OTG_HS_EP1_IN_IRQHandler          [WEAK]                      
                EXPORT  OTG_HS_WKUP_IRQHandler            [WEAK]                        
                EXPORT  OTG_HS_IRQHandler                 [WEAK]                                      
                EXPORT  DCMI_IRQHandler                   [WEAK]
                EXPORT  HASH_RNG_IRQHandler               [WEAK]
                EXPORT  FPU_IRQHandler                    [WEAK]
                EXPORT  UART7_IRQHandler                  [WEAK]
                EXPORT  UART8_IRQHandler                  [WEAK]
                EXPORT  SPI4_IRQHandler                   [WEAK]
                EXPORT  SPI5_IRQHandler                   [WEAK]
                EXPORT  SPI6_IRQHandler                   [WEAK]
                EXPORT  SAI1_IRQHandler                   [WEAK]
                EXPORT  LTDC_IRQHandler                   [WEAK]
                EXPORT  LTDC_ER_IRQHandler                [WEAK]
                EXPORT  DMA2D_IRQHandler                  [WEAK]
                EXPORT  QUADSPI_IRQHandler                [WEAK]
                EXPORT  DSI_IRQHandler                    [WEAK]

WWDG_IRQHandler                                                       
PVD_IRQHandler                                      
TAMP_STAMP_IRQHandler                  
RTC_WKUP_IRQHandler                                
FLASH_IRQHandler                                                       
RCC_IRQHandler                                                            
EXTI0_IRQHandler                                                          
EXTI1_IRQHandler                                                           
EXTI2_IRQHandler                                                          
EXTI3_IRQHandler                                                         
EXTI4_IRQHandler                                                          
DMA1_Stream0_IRQHandler                                       
DMA1_Stream1_IRQHandler                                          
DMA1_Stream2_IRQHandler                                          
DMA1_Stream3_IRQHandler                                          
DMA1_Stream4_IRQHandler                                          
DMA1_Stream5_IRQHandler                                          
DMA1_Stream6_IRQHandler                                          
ADC_IRQHandler                                         
CAN1_TX_IRQHandler                                                            
CAN1_RX0_IRQHandler                                                          
CAN1_RX1_IRQHandler                                                           
CAN1_SCE_IRQHandler                                                           
EXTI9_5_IRQHandler                                                
TIM1_BRK_TIM9_IRQHandler                        
TIM1_UP_TIM10_IRQHandler                      
TIM1_TRG_COM_TIM11_IRQHandler  
TIM1_CC_IRQHandler                                               
TIM2_IRQHandler                                                           
TIM3_IRQHandler                                                           
TIM4_IRQHandler                                                           
I2C1_EV_IRQHandler                                                         
I2C1_ER_IRQHandler                                                         
I2C2_EV_IRQHandler                                                        
I2C2_ER_IRQHandler                                                           
SPI1_IRQHandler                                                          
SPI2_IRQHandler                                                           
USART1_IRQHandler                                                       
USART2_IRQHandler                                                       
USART3_IRQHandler                                                      
EXTI15_10_IRQHandler                                            
RTC_Alarm_IRQHandler                            
OTG_FS_WKUP_IRQHandler                                
TIM8_BRK_TIM12_IRQHandler                      
TIM8_UP_TIM13_IRQHandler                       
TIM8_TRG_COM_TIM14_IRQHandler  
TIM8_CC_IRQHandler                                               
DMA1_Stream7_IRQHandler                                                 
FMC_IRQHandler                                                            
SDIO_IRQHandler                                                            
TIM5_IRQHandler                                                            
SPI3_IRQHandler                                                            
UART4_IRQHandler                                                          
UART5_IRQHandler                                                          
TIM6_DAC_IRQHandler                            
TIM7_IRQHandler                              
DMA2_Stream0_IRQHandler                                         
DMA2_Stream1_IRQHandler                                          
DMA2_Stream2_IRQHandler                                           
DMA2_Stream3_IRQHandler                                           
DMA2_Stream4_IRQHandler                                        
ETH_IRQHandler                                                         
ETH_WKUP_IRQHandler                                
CAN2_TX_IRQHandler                                                           
CAN2_RX0_IRQHandler                                                          
CAN2_RX1_IRQHandler                                                          
CAN2_SCE_IRQHandler                                                          
OTG_FS_IRQHandler                                                    
DMA2_Stream5_IRQHandler                                          
DMA2_Stream6_IRQHandler                                          
DMA2_Stream7_IRQHandler                                          
USART6_IRQHandler                                                        
I2C3_EV_IRQHandler                                                          
I2C3_ER_IRQHandler                                                          
OTG_HS_EP1_OUT_IRQHandler                           
OTG_HS_EP1_IN_IRQHandler                            
OTG_HS_WKUP_IRQHandler                                
OTG_HS_IRQHandler                                                   
DCMI_IRQHandler 
HASH_RNG_IRQHandler
FPU_IRQHandler  
UART7_IRQHandler                  
UART8_IRQHandler                  
SPI4_IRQHandler                   
SPI5_IRQHandler                   
SPI6_IRQHandler                   
SAI1_IRQHandler                   
LTDC_IRQHandler                   
LTDC_ER_IRQHandler                 
DMA2D_IRQHandler
QUADSPI_IRQHandler                 
DSI_IRQHandler                  
                B       .

                ENDP

                ALIGN

 
  ENTRY
Temp_Main
  BX LR
  ALIGN
								END

