

		PRESERVE8
    AREA RESET, CODE, READONLY
			
;--------------------------------------------
; Mode bits and interrupt flag (I&F) defines
;--------------------------------------------
USR_MODE    EQU     0x10
FIQ_MODE    EQU     0x11
IRQ_MODE    EQU     0x12
SVC_MODE    EQU     0x13
ABT_MODE    EQU     0x17
UDF_MODE    EQU     0x1B
SYS_MODE    EQU     0x1F

I_BIT       EQU     0x80
F_BIT       EQU     0x40

;----------------------------
; System / User Stack Memory
;----------------------------
RAM_Limit       EQU     0x00C00000            ; For unexpanded hardware board

UND_Stack       EQU     RAM_Limit
Abort_Stack     EQU     RAM_Limit-256
FIQ_Stack       EQU     RAM_Limit-512       ; followed by IRQ stack
SVC_Stack       EQU     RAM_Limit-1024      ; SVC stack at top of memory
IRQ_Stack       EQU     RAM_Limit-10240     ; followed by IRQ stack
USR_Stack       EQU     RAM_Limit-20480

REG_AIC_MDCR    EQU     0xB8002138  ; Mask disable command register
REG_AIC_MDCRH   EQU     0xB800213C  ; Mask disable command register (High)

;    IMPORT  vPortYieldProcessor
    EXPORT  Reset_Go
			
		ENTRY
    EXPORT  Vector_Table
Vector_Table
        B       Reset_Go    			;0 Modified to be relative jumb for external boot
        LDR     PC, Undefined_Addr;1
        LDR     PC, SWI_Addr			;2
        LDR     PC, Prefetch_Addr	;3
        LDR     PC, Abort_Addr		;4
        DCD     0x0								;5
        LDR     PC, IRQ_Addr			;6
        LDR     PC, FIQ_Addr			;7
Reset_Addr      DCD     Reset_Go					;8
Undefined_Addr  DCD     Undefined_Handler	;9
SWI_Addr        DCD     0;vPortYieldProcessor;10
Prefetch_Addr   DCD     Prefetch_Handler	;11
Abort_Addr      DCD     Abort_Handler			;12
                DCD     0									;13
IRQ_Addr        DCD     IRQ_Handler				;14
FIQ_Addr        DCD     FIQ_Handler				;15

					
                ; External Interrupts
                DCD     0;16 Watch Dog Timer Interrupt
                DCD     0;17 Windowed-WDT Interrupt 
                DCD     0;18 Low Voltage Detect Interrupt
                DCD     0;19 External Interrupt 0
                DCD     0;20 External Interrupt 1
                DCD     0;21 External Interrupt 2           
                DCD     0;22 External Interrupt 3                   
                DCD     0;23 External Interrupt 4       
                DCD     0;24 External Interrupt 5    
                DCD     0;25 External Interrupt 6
                DCD     0;26 External Interrupt 7             
                DCD     0;27 I2S Controller Interrupt             
                DCD     0;28 LCD Controller Interrupt                    
                DCD     0;29 Sensor Interface Controller Interrupt        
                DCD     0;30 RTC Interrupt               
                DCD     0;31 Timer 0 Interrupt                 
                DCD     0;32 Timer 1 Interrupt
                DCD     0;33 ADC Interrupt                     
                DCD     0;34 EMC 0 RX Interrupt             
                DCD     0;35 EMC 1 RX Interrupt
                DCD     0;36 EMC 0 TX Interrupt
                DCD     0;37 EMC 1 TX Interrupt
                DCD     0;38 USB 2.0 Host Controller Interrupt
                DCD     0;39 USB 1.1 Host Controller Interrupt
                DCD     0;40 GDMA Channel 0 Interrupt
                DCD     0;41 GDMA Channel 1 Interrupt
                DCD     0;42 SD/SDIO Host Interrupt
                DCD     0;43 SIC Interrupt
                DCD     0;44 USB Device Controller Interrupt                              
                DCD     0;45 Timer 2 Interrupt                                   
                DCD     0;46 Timer 3 Interrupt                                
                DCD     0;47 Timer 4 Interrupt                                    
                DCD     0;48 JPEG Engine Interrupt                          
                DCD     0;49 2D Graphic Engine Interrupt                        
                DCD     0;50 CRYPTO Engine Interrupt                                 
                DCD     0;51 UART 0 Interrupt                            
                DCD     0;52 UART 1 Interrupt                           
                DCD     0;53 UART 2 Interrupt                              
                DCD     0;54 UART 4 Interrupt                            
                DCD     0;55 UART 6 Interrupt                              
                DCD     0;56 UART 8 Interrupt                          
                DCD     0;57 UART 10 Interrupt             
                DCD     0;58 UART 3 Interrupt                   
                DCD     0;59 UART 5 Interrupt       
                DCD     0;60 UART 7 Interrupt          
                DCD     0;61 UART 9 Interrupt
                DCD     0;62 Enhanced Timer 0 Interrupt                             
                DCD     0;63 Enhanced Timer 1 Interrupt                                     
                DCD     0;64 Enhanced Timer 2 Interrupt                                       
                DCD     0;65 Enhanced Timer 3 Interrupt                                    
                DCD     0;66 SPI 0 Interrupt                                      
                DCD     0;67 SPI 1 Interrupt                                    
                DCD     0;68 I2C 0 Interrupt                                    
                DCD     0;69 I2C 1 Interrupt                                 
                DCD     0;70 SmartCard 0 Interrupt          
                DCD     0;71 SmartCard 1 Interrupt          
                DCD     0;72 GPIO Interrupt                          
                DCD     0;73 CAN 0 Interrupt                        
                DCD     0;74 CAN 1 Interrupt                          
                DCD     0;75 PWM Interrupt                            
                DCD     0;76                                 
                DCD     0;77                                    
                DCD     0;78                  
                DCD     0;79                                           
                DCD     0;80                                       
                DCD     0;81                                        
                DCD     0;82                                      
                DCD     0;83                             
                DCD     0;84                           
                DCD     0;85                      
                DCD     0;86                           
                DCD     0;87                                
                DCD     0;88                                   
                DCD     0;89                                       
                DCD     0;90                
                DCD     0;91                  
                DCD     0;92                    
                DCD     0;93                              
                DCD     0;94
                DCD     0;95	                              
                DCD     0;96
                DCD     0;97
                DCD     0;98
                DCD     0;99
                DCD     0;100
                DCD     0;101
                DCD     0;102
                DCD     0;103
                DCD     0;104
                DCD     0;105
                DCD     0;106
                DCD     0;107
                DCD     0;108

	DCD 0;109 //协议:0=协议无关,376=1376.1协议，698=698.45协议,645=645-2007协议

;入口
	extern _0
	;extern Init_ETH1_main;//启动初始化
	;extern Init_OS_SoftwareUpdate;//软件更新初始化
	;extern ADDR_OS_END
	;extern ADDR_OS_VER;//版本信息
		
  DCD 0;Vector_Table ; 110: 软件模块开始地址
  DCD 0;ADDR_OS_END ; 111: 软件模块结束地址
	DCD 0; 112: 软件模块起动初始化入口
  DCD 0;Init_OS_SoftwareUpdate; 113: 软件模块更新初始化入口
  DCD 0;ADDR_OS_VER ; 114: 版本信息
  DCD _0 ; 115
  DCD _0 ; 116:
  DCD _0 ; 117:
  DCD 0 ; 118://初始化库开始地址
  DCD 0 ; 119://初始化库结束地址

;//在PC中进行替换
	DCD 0;//120//EnProject+(EnHV<<1)+(EnUSER<<2)+(EnSV<<3)+(EnMeter<<4);//软件更新时是否判别
	DCD 0;//121//Project;//方案0=智能表,100=网络表,200=集中器,300=专变终端,400=,500=集中器Ⅱ型,600=,700=
	DCD 0;//122//HVER;//硬件版本号
	DCD 0;//123//USER;//用户标识
	DCD 0;//124//SVER;//软件版本号
	DCD 0;//125//SAME_MS+(SAME_OS<<1)+(SAME_Meter<<2)+(SAME_Terminal<<3)+(SAME_Menu<<4)+(SAME_Font<<5),//同时更新标志:B0=MS,B1=OS,B2=Meter,B3=Terminal,B4=Menu,5=Font,6=,7=Router
	DCD 0;//126//DEBUG,// 0=发行,1=调试
	DCD 0;//127//MeterCtrl+(MeterType<<8)+(MeterSpec<<16);//电能表常数接线方式等电能表规格
	DCD 6;//128//软件号:0=MS,1=OS,2=Meter,3=Terminal,4=Menu,5=Font,6=,7=Router,8=Test
	DCD 0;//129//KEIL直接写入=0,在PC中进行替换后下载=1

;//OS
	extern eth1_Lable

	DCD eth1_Lable;130
	DCD 0;emWin_Lable;131
	DCD 0;GB2312vsUNICODE_Lable;132
  DCD _0 ;133
  DCD _0 ;134
  DCD _0 ;135
  DCD _0 ;136
  DCD _0 ;137
  DCD _0 ;138
  DCD _0 ;139
	
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
__Vectors_Size  EQU  __Vectors_End - Vector_Table
	

; Exception Handlers
; The following dummy handlers do not do anything useful in this example.
; They are set up here for completeness.
;	AREA |.text|, CODE, READONLY
	
Undefined_Handler
        B       Undefined_Handler
SWI_Handler1
        B       SWI_Handler1
Prefetch_Handler
        B       Prefetch_Handler
Abort_Handler
        B       Abort_Handler
IRQ_Handler
        B       IRQ_Handler
FIQ_Handler
        B       FIQ_Handler


Reset_Go
	IMPORT  ETH1_main
	;-----------------------------
	;   enter the C code
	;-----------------------------
	B   ETH1_main

		
	END




