
#if MainProtocol==376
#include "../../../STM32F4xx/Head/Project.h"
#endif
#if MainProtocol==698
#include "../../../STM32F4xx/H698/Project698.h"
#endif
#if MainProtocol==CSG
#include "../../../STM32F4xx/Hcsg/ProjectCSG.h"
#endif
#include "../../../NUc97x/OS/API/usbAPI.h"




//typedef struct 
//{
//    FFOBJID obj;          /* Object identifier */
//    BYTE    flag;         /* File object status flags */
//    BYTE    err;          /* Abort flag (error code) */
//    FSIZE_t fptr;         /* File read/write pointer (Byte offset origin from top of the file) */
//    DWORD   clust;        /* Current cluster of fptr (One cluster behind if fptr is on the cluster boundary. Invalid if fptr == 0.) */
//    DWORD   sect;         /* Current data sector (Can be invalid if fptr is on the cluster boundary.)*/
//#if !FF_FS_READONLY
//    DWORD   dir_sect;     /* Sector number containing the directory entry */
//    BYTE*   dir_ptr;      /* Ponter to the directory entry in the window */
//#endif
//#if FF_USE_FASTSEEK
//    DWORD*  cltbl;        /* Pointer to the cluster link map table (Nulled on file open. Set by application.) */
//#endif
//#if !FF_FS_TINY
//    BYTE    buf[FF_MAX_SS]; /* File private data transfer buffer (Always valid if fptr is not on the sector boundary but can be invalid if fptr is on the sector boundary.) */
//#endif
//} FIL;

#define f_READ_WRITE_BYTE     512//困有定义 #define	_MAX_SS		512

u32 f_READ(FIL* fp, void* buff,u32 BTR)// Read data from a file 
{
	u32 i;
	u32 x;
	UINT btr;
	UINT br[1];
	u32* p32;
	u32 MECR;
	u32 MECRH;
	
//原中断允许保存
	p32=(u32 *)(REG_AIC_IMR);//AIC Interrupt Mask Register
	MECR=p32[0];
	MECRH=p32[1];
//禁止中断,除下USB外
	//TMR0_IRQn               = 16,//Timer 0 Interrupt
	//EHCI_IRQn               = 23,// USB 2.0 Host Controller Interrupt
	//OHCI_IRQn               = 24,//USB 1.1 Host Controller Interrupt
	//USBD_IRQn               = 29,//USB Device Interrupt
	p32=(u32 *)(REG_AIC_MDCR);//AIC Mask Disable Command Register
	p32[0]=0xffffffff&(~((1<<TMR0_IRQn)|(1<<EHCI_IRQn)|(1<<OHCI_IRQn)|(1<<USBD_IRQn)));
	p32[1]=0xffffffff;
	
	i=0;
	while(i<BTR)
	{
		x=BTR-i;
		if(x>f_READ_WRITE_BYTE)
		{
			btr=f_READ_WRITE_BYTE;
		}
		else
		{
			btr=x;
		}
		f_read(fp,(void*)((u32)buff+i),btr,br);// Read data from a file 
		if(btr!=br[0])
		{
			i+=br[0];
		//恢复关闭的中断
			p32=(u32 *)(REG_AIC_MECR);//SET
			p32[0]=MECR;
			p32[1]=MECRH;
			return i;
		}
		i+=btr;
		//fp->fptr=i;
	}
	sysFlushCache(D_CACHE);//I_CACHE,D_CACHE,I_D_CACHE
//恢复关闭的中断
	p32=(u32 *)(REG_AIC_MECR);//SET
	p32[0]=MECR;
	p32[1]=MECRH;
	return i;
}

u32 f_WRITE(FIL* fp, const void* buff, UINT BTW)// Write data to a file 
{
	u32 i;
	u32 x;
	UINT btw;
	UINT bw[1];
	u32* p32;
	u32 MECR;
	u32 MECRH;
	
//原中断允许保存
	p32=(u32 *)(REG_AIC_IMR);//AIC Interrupt Mask Register
	MECR=p32[0];
	MECRH=p32[1];
//禁止中断,除下USB外
	//EHCI_IRQn               = 23,// USB 2.0 Host Controller Interrupt
	//OHCI_IRQn               = 24,//USB 1.1 Host Controller Interrupt
	//USBD_IRQn               = 29,//USB Device Interrupt
	p32=(u32 *)(REG_AIC_MDCR);//AIC Mask Disable Command Register
	p32[0]=0xffffffff&(~((1<<TMR0_IRQn)|(1<<EHCI_IRQn)|(1<<OHCI_IRQn)|(1<<USBD_IRQn)));
	p32[1]=0xffffffff;
	
	i=0;
	while(i<BTW)
	{
		x=BTW-i;
		if(x>f_READ_WRITE_BYTE)
		{
			btw=f_READ_WRITE_BYTE;
		}
		else
		{
			btw=x;
		}
		f_write (fp,(void*)((u32)buff+i), btw,bw);// Write data to a file 
		if(btw!=bw[0])
		{
			i+=bw[0];
		//恢复关闭的中断
			p32=(u32 *)(REG_AIC_MECR);//SET
			p32[0]=MECR;
			p32[1]=MECRH;
			return i;
		}
		i+=btw;
		//fp->fptr=i;
	}
//恢复关闭的中断
	p32=(u32 *)(REG_AIC_MECR);//SET
	p32[0]=MECR;
	p32[1]=MECRH;
	return i;
}












