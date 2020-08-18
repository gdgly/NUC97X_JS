

;//程序入口
	extern Test_Main;
		
		
		
		
                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     0;__initial_sp               ;0 Top of Stack
                DCD     Test_Main;//Reset_Handler  ;1 Reset Handler
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

	DCD 0;109 //协议:0=协议无关,376=1376.1协议,698=698.45协议,645=645-2007协议

;入口
	extern _0
	extern Init_Test;//启动初始化
	extern Init_Test_SoftwareUpdate;//软件更新初始化
	extern ADDR_Test_END
	extern ADDR_Test_VER;//版本信息
	extern Test_ReStartOS;// 115 电气性能测试重启OS前
	extern Test;//电气性能测试	
		
	extern ADDR_Test_SetInitLib_Start
	extern ADDR_Test_SetInitLib_End
  DCD __Vectors ; 110: 软件模块开始地址
  DCD ADDR_Test_END ; 111: 软件模块结束地址
	DCD Init_Test ; 112: 软件模块起动初始化入口
  DCD Init_Test_SoftwareUpdate; 113: 软件模块更新初始化入口
  DCD ADDR_Test_VER ; 114: 版本信息
  DCD Test_ReStartOS ; 115 电气性能测试重启OS前
  DCD Test ; 116:电气性能测试
  DCD _0 ; 117:
  DCD 0;ADDR_Test_SetInitLib_Start ; 118://初始化库开始地址
  DCD 0;ADDR_Test_SetInitLib_End ; 119://初始化库结束地址

;//在PC中进行替换
	DCD 0;//120
	DCD 0;//121
	DCD 0;//122
	DCD 0;//123
	DCD 0;//124
	DCD 0;//125
	DCD 0;//126
	DCD 0;//127
	DCD 8;//128
	DCD 0;//129

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

