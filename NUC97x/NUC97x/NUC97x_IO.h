
#ifndef NUC97x_IO_H
#define NUC97x_IO_H
#ifdef __cplusplus
 extern "C" {
#endif


	 
typedef struct
{
	u32 GPIO_DIR;//GPIO portA direction control register
	u32 GPIO_DATAOUT;//GPIO portA data output register
	u32 GPIO_DATAIN;//GPIO portA data input register
	u32 GPIO_IMD;//GPIO Port A Interrupt Mode Register
	u32 GPIO_IREN;//GPIO Port A Interrupt Rising-Edge or Level-High Enable Register */
	u32 GPIO_IFEN;//GPIO Port A Interrupt Falling-Edge or Level-Low Enable Register */
	u32 GPIO_ISR;//GPIO Port A Interrupt Status Register */
	u32 GPIO_DBEN;//GPIO Port A De-bounce Enable Register */
	u32 GPIO_PUEN;//GPIO Port A Pull-Up Enable Register */
	u32 GPIO_PDEN;//GPIO Port A Pull-Down Enable Register */
	u32 GPIO_ICEN;//GPIO Port A CMOS Input Enable Register */
	u32 GPIO_ISEN;//GPIO Port A Schmitt-Trigger Input Enable Register */
}GPIO_TypeDef; 
	 
	 

	 
	 


#ifdef __cplusplus
 }
#endif 
#endif
