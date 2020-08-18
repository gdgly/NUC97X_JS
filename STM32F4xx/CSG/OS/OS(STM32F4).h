

#ifndef OS_H
#define OS_H

#include "../Hcsg/ProjectCSG.h"
//os
#include "cmsis_os.h"
#include "../OS/My_Heap.h"
#include "../Lzma/LzmaApp.h"
#include "stdio.h"
#include "rl_fs.h"
//usb
#include "usb_def.h"
#include "usb_cdc.h"
#include "usb_hid.h"
#include "usb_msc.h"
#include "rl_usb.h"
//eth
#include "rl_net_ds.h"
//mp3
#include "../mp3codec/mp3dec.h"


#ifdef __cplusplus
 extern "C" {
#endif

void ReSTART_OS(void);//重启OS
void Disable_os(void);//暂停os
void Enable_os(void);//允许os


typedef struct
{
//Lzma
u32 (*RAM_LzmaEncode) (u32 ADDR_Sour,u32 LEN_Sour,u32 ADDR_Dest,u32 DictSize);//RAM->RAM的Lzma压缩算法:入口：DictSize=字典长度;返回:压缩后的长度(包括开始的13字节为lzma格式头,0=ERR)
u32 (*RAM_LzmaDecode) (u32 ADDR_Sour,u32 LEN_Sour,u32 ADDR_Dest,u32 MaxLEN_Dest);//RAM->RAM的Lzma解压缩算法;入口:LEN_Sour包括开始的13字节为lzma格式头;返回:解压缩后的长度(0=ERR)	
	
//osKernel
//  ==== Generic Wait Functions ====
osStatus (*osDelay) (uint32_t millisec);
//======Thread Management==========
osThreadId 	(*osThreadCreate) (const osThreadDef_t *thread_def, void *argument);
osThreadId 	(*osThreadGetId) (void);
osStatus 	(*osThreadTerminate) (osThreadId thread_id);
osStatus 	(*osThreadYield) (void);
	

//FS
//  ==== System Routines ====
fsStatus (*finit) (const char *drive);
fsStatus (*funinit) (const char *drive);
fsStatus (*fmount) (const char *drive);
fsStatus (*funmount) (const char *drive);
//  ==== File Maintenance Routines ====	
fsStatus (*fdelete) (const char *path, const char *options);
fsStatus (*ffind) (const char *pattern, fsFileInfo *info);	
fsStatus (*frename) (const char *path, const char *newname);
fsStatus (*fattrib) (const char *path, const char *attr);
//  ==== Directory Maintenance Routines ====
fsStatus (*fpwd) (const char *drive, char *buf, uint32_t len);
fsStatus (*fchdir) (const char *path);
fsStatus (*fmkdir) (const char *path);
fsStatus (*frmdir) (const char *path, const char *options);
//  ==== Utility Routines ====
fsStatus (*fchdrive) (const char *drive);	
int64_t (*ffree) (const char *drive);
fsStatus (*fformat) (const char *drive, const char *options);
int32_t (*fanalyse) (const char *drive);
fsStatus (*fcheck) (const char *drive);
fsStatus (*fdefrag) (const char *drive);
fsStatus (*fmedia) (const char *drive);
fsStatus (*finfo) (const char *drive, fsDriveInfo *info);
fsStatus (*fvol) (const char *drive, char *label, uint32_t *serial);
//  ==== File Time Support Routines ====
fsStatus (*fs_get_time) (fsTime *time);
//  ==== Media Support Routines ====
//int32_t (*fs_mc_read_cd) (uint32_t drive_num);
//int32_t (*fs_mc_read_wp) (uint32_t drive_num);
//int32_t (*fs_mc_spi_control_ss) (uint32_t drive_num, uint32_t ss);
//  ==== I/O Control Interface Routines ====
//int32_t (*fs_ioc_get_id) (const char *drive);
//fsStatus (*fs_ioc_lock) (int32_t drv_id);
//fsStatus (*fs_ioc_unlock) (int32_t drv_id);
//fsStatus (*fs_ioc_get_cache) (int32_t drv_id, fsIOC_Cache *cache_info);
//fsStatus (*fs_ioc_read_sector) (int32_t drv_id, uint32_t sect, uint8_t *buf, uint32_t cnt);
//fsStatus (*fs_ioc_write_sector) (int32_t drv_id, uint32_t sect, const uint8_t *buf, uint32_t cnt);
//fsStatus (*fs_ioc_read_info) (int32_t drv_id, fsMediaInfo *info);
//fsStatus (*fs_ioc_device_ctrl) (int32_t drv_id, fsDevCtrlCode code, void *p);
	
//STDIO
 int (*remove)(const char * /*filename*/) __attribute__((__nonnull__(1)));
 int (*rename)(const char * /*old*/, const char * /*new*/) __attribute__((__nonnull__(1,2)));
 FILE *(*tmpfile)(void);
 char *(*tmpnam)(char * /*s*/);
 int (*fclose)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 int (*fflush)(FILE * /*stream*/);
 FILE *(*fopen)(const char * __restrict /*filename*/,const char * __restrict /*mode*/) __attribute__((__nonnull__(1,2)));	
 FILE *(*freopen)(const char * __restrict /*filename*/,const char * __restrict /*mode*/,FILE * __restrict /*stream*/) __attribute__((__nonnull__(2,3)));	
 void (*setbuf)(FILE * __restrict /*stream*/,char * __restrict /*buf*/) __attribute__((__nonnull__(1)));
 int (*setvbuf)(FILE * __restrict /*stream*/,char * __restrict /*buf*/,int /*mode*/, size_t /*size*/) __attribute__((__nonnull__(1)));
	
 int (*fgetc)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 char *(*fgets)(char * __restrict /*s*/, int /*n*/,FILE * __restrict /*stream*/) __attribute__((__nonnull__(1,3)));
 int (*fputc)(int /*c*/, FILE * /*stream*/) __attribute__((__nonnull__(2)));
 int (*fputs)(const char * __restrict /*s*/, FILE * __restrict /*stream*/) __attribute__((__nonnull__(1,2)));
 int (*getc)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 char *(*gets)(char * /*s*/) __attribute__((__nonnull__(1)));
 int (*putc)(int /*c*/, FILE * /*stream*/) __attribute__((__nonnull__(2)));
 int (*puts)(const char * /*s*/) __attribute__((__nonnull__(1)));
 int (*ungetc)(int /*c*/, FILE * /*stream*/) __attribute__((__nonnull__(2)));
 size_t (*fread)(void * __restrict /*ptr*/,size_t /*size*/, size_t /*nmemb*/, FILE * __restrict /*stream*/) __attribute__((__nonnull__(1,4)));
 size_t (*__fread_bytes_avail)(void * __restrict /*ptr*/,size_t /*count*/, FILE * __restrict /*stream*/) __attribute__((__nonnull__(1,3)));
 size_t (*fwrite)(const void * __restrict /*ptr*/,size_t /*size*/, size_t /*nmemb*/, FILE * __restrict /*stream*/) __attribute__((__nonnull__(1,4)));
 int (*fgetpos)(FILE * __restrict /*stream*/, fpos_t * __restrict /*pos*/) __attribute__((__nonnull__(1,2)));
 int (*fseek)(FILE * /*stream*/, long int /*offset*/, int /*whence*/) __attribute__((__nonnull__(1)));
 int (*fsetpos)(FILE * __restrict /*stream*/, const fpos_t * __restrict /*pos*/) __attribute__((__nonnull__(1,2)));
 long int (*ftell)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 void (*rewind)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 void (*clearerr)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 int (*feof)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 int (*ferror)(FILE * /*stream*/) __attribute__((__nonnull__(1)));
 void (*perror)(const char * /*s*/);


//USB
//  ==== USB Device Functions ====
 usbStatus (*USBD_Initialize) (uint8_t device);
 usbStatus (*USBD_Uninitialize) (uint8_t device);
 usbStatus (*USBD_Connect) (uint8_t device);
 usbStatus (*USBD_Disconnect) (uint8_t device);
 bool (*USBD_Configured) (uint8_t device);
 
//  ==== USB Host Functions ====
 usbStatus (*USBH_Initialize) (uint8_t ctrl);
 usbStatus (*USBH_Uninitialize) (uint8_t ctrl);
 usbStatus (*USBH_GetDeviceStatus) (uint8_t device);
//  ==== USB Host Mass Storage Functions ====
 usbStatus (*USBH_MSC_GetDeviceStatus) (uint8_t instance);


//ETH
//  ==== Network System API ====
netStatus (*netInitialize) (void);
const char *(*netSYS_GetHostName) (void);
netStatus (*netSYS_SetHostName) (const char *hostname);
//  ==== UDP Socket API ====
int32_t   (*netUDP_GetSocket) (netUDP_cb_t cb_func);
netStatus (*netUDP_ReleaseSocket) (int32_t socket);
netStatus (*netUDP_Open) (int32_t socket, uint16_t port);
netStatus (*netUDP_Close) (int32_t socket);
uint8_t  *(*netUDP_GetBuffer) (uint32_t size);
netStatus (*netUDP_Send) (int32_t socket, const NET_ADDR *addr, uint8_t *buf, uint32_t len);
netStatus (*netUDP_SetOption) (int32_t socket, netUDP_Option option, uint32_t val);
uint16_t  (*netUDP_GetLocalPort) (int32_t socket);
//  ==== TCP Socket API ====
int32_t   (*netTCP_GetSocket) (netTCP_cb_t cb_func);
netStatus (*netTCP_ReleaseSocket) (int32_t socket);
netStatus (*netTCP_Listen) (int32_t socket, uint16_t port);
netStatus (*netTCP_Connect) (int32_t socket, const NET_ADDR *addr, uint16_t local_port);
netStatus (*netTCP_Close) (int32_t socket);
netStatus (*netTCP_Abort) (int32_t socket);
uint32_t  (*netTCP_GetMaxSegmentSize) (int32_t socket);
uint8_t  *(*netTCP_GetBuffer) (uint32_t size);
bool      (*netTCP_SendReady) (int32_t socket);
netStatus (*netTCP_Send) (int32_t socket, uint8_t *buf, uint32_t len);
netTCP_State (*netTCP_GetState) (int32_t socket);
netStatus (*netTCP_ResetReceiveWindow) (int32_t socket);
netStatus (*netTCP_SetOption )(int32_t socket, netTCP_Option option, uint32_t val);
uint16_t  (*netTCP_GetLocalPort) (int32_t socket);
netStatus (*netTCP_GetPeer) (int32_t socket, NET_ADDR *addr, uint32_t addr_len);
uint32_t  (*netTCP_GetTimer) (int32_t socket);
//  ==== BSD Socket API ====
#ifdef RTE_Network_Socket_BSD	
int (*socket) (int family, int type, int protocol);
int (*bind) (int sock, const SOCKADDR *addr, int addrlen);
int (*listen) (int sock, int backlog);
int (*accept) (int sock, SOCKADDR *addr, int *addrlen);
int (*connect) (int sock, const SOCKADDR *addr, int addrlen);
int (*send) (int sock, const char *buf, int len, int flags);
int (*sendto) (int sock, const char *buf, int len, int flags, const SOCKADDR *to, int tolen);
int (*recv) (int sock, char *buf, int len, int flags);
int (*recvfrom) (int sock, char *buf, int len, int flags, SOCKADDR *from, int *fromlen);
int (*closesocket) (int sock);
int (*getpeername) (int sock, SOCKADDR *name, int *namelen);
int (*getsockname) (int sock, SOCKADDR *name, int *namelen);
int (*setsockopt) (int sock, int level, int optname, const char *optval, int optlen);
int (*ioctlsocket) (int sock, long cmd, unsigned long *argp);
HOSTENT *(*gethostbyname) (const char *name, int *err);
#endif
//  ==== Interface User API ====
//netStatus (*netIF_GetOption) (uint32_t if_id, netIF_Option option, uint8_t *buf, uint32_t buf_len);
//netStatus (*netIF_SetOption) (uint32_t if_id, netIF_Option option, const uint8_t *buf, uint32_t buf_len);
//  ==== Ethernet Interface User API ====
//netStatus (*netARP_CacheIP) (uint32_t if_num, const uint8_t *ip4_addr, netARP_CacheType type);
//netStatus (*netARP_CacheMAC) (uint32_t if_num, const uint8_t *mac_addr);
//netStatus (*netARP_GetIP) (uint32_t if_num, const uint8_t *mac_addr, uint8_t *ip4_addr);
//netStatus (*netARP_GetMAC) (uint32_t if_num, const uint8_t *ip4_addr, uint8_t *mac_addr);
//netStatus (*netNDP_CacheIP) (uint32_t if_num, const uint8_t *ip6_addr);
//netStatus (*netNDP_GetIP) (uint32_t if_num, const uint8_t *mac_addr, uint8_t *ip6_addr);
//netStatus (*netNDP_GetMAC) (uint32_t if_num, const uint8_t *ip6_addr, uint8_t *mac_addr);
//netStatus (*netIGMP_Join) (uint32_t if_num, const uint8_t *ip4_addr);
//netStatus (*netIGMP_Leave) (uint32_t if_num, const uint8_t *ip4_addr);
netStatus (*netDHCP_Enable) (uint32_t if_num);
netStatus (*netDHCP_Disable) (uint32_t if_num);
//  ==== Ethernet Interface Callbacks ====
//void      (*netETH_Notify) (uint32_t if_num, netETH_Event event, uint32_t val);
//void      (*netDHCP_Notify) (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);
//  ==== Ping User API ====
netStatus (*netPing_Echo) (const NET_ADDR *addr, netPing_cb_t cb_func);

//  ==== DNS Client User API ====
//netStatus (*netDNSc_GetHostByName) (const char *name, netDNSc_cb_t cb_func);

//  ==== FTP Client User API ====
uint32_t  (*netFTPc_Process) (netFTPc_Request request, char *buf, uint32_t buf_len);
void      (*netFTPc_Notify) (netFTPc_Event event);
//  ==== FTP Client File System Interface ====
void     *(*netFTPc_fopen) (const char *fname, const char *mode);
void      (*netFTPc_fclose) (void *file);
uint32_t  (*netFTPc_fread) (void *file, uint8_t *buf, uint32_t len);
uint32_t  (*netFTPc_fwrite) (void *file, const uint8_t *buf, uint32_t len);

//  ==== FTP Server User API ====

//  ==== TFTP Server User API ====

//  ==== PPP Interface User API ====

//  ==== SLIP Interface User API ====



//mp3codec
//HMP3Decoder (*MP3InitDecoder) (void);
//int (*MP3Decode) (HMP3Decoder hMP3Decoder, unsigned char **inbuf, int *bytesLeft, short *outbuf, int useSize);
//void (*MP3GetLastFrameInfo) (HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo);
//int (*MP3GetNextFrameInfo) (HMP3Decoder hMP3Decoder, MP3FrameInfo *mp3FrameInfo, unsigned char *buf);
//int (*MP3FindSyncWord) (unsigned char *buf, int nBytes);

}const os_Type;

os_Type *Get_os(void);//得到os表地址

/*
//MAC
#include "Driver_ETH_MAC.h"
typedef ARM_DRIVER_ETH_MAC     osMAC_Type;
osMAC_Type *Get_osMAC(void);//得到osMAC表地址

//PHY(LAN8720)
#include "Driver_ETH_PHY.h"
typedef ARM_DRIVER_ETH_PHY     osPHY_Type;
osPHY_Type *Get_osPHY(void);//得到osPHY表地址
*/



#ifdef __cplusplus
 }
#endif 
#endif


