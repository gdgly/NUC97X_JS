

#ifndef MS_H
#define MS_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "../OS/OS.h"
#include "../OS/My_Heap.h"
#include "../MS/msFILE.h"
#include "../Device/NAND.h"
#include "../Device/MEMRW.h"
	 

	 
	 
#ifdef LINUX 
#define O_ACCMODE	   0003
#define O_RDONLY	     00
#define O_WRONLY	     01
#define O_RDWR		     02
#define O_CREAT		   0100	/* not fcntl */
#define O_EXCL		   0200	/* not fcntl */
#define O_NOCTTY	   0400	/* not fcntl */
#define O_TRUNC		  01000	/* not fcntl */
#define O_APPEND	  02000
#define O_NONBLOCK	  04000
#define O_NDELAY	O_NONBLOCK
#define O_SYNC		 010000
#define O_FSYNC		 O_SYNC
#define O_ASYNC		 020000

#define SEEK_SET	0	/* Seek from beginning of file.  */
#define SEEK_CUR	1	/* Seek from current position.  */
#define SEEK_END	2	/* Seek from end of file.  */

#define fsid_t		struct { int __val[2]; }
struct statfs
{
	int f_type;    // 文件系统类型 (see below)
	int f_bsize;   // 经过优化的传输块大小
	unsigned long int   f_blocks;  // 文件系统数据块总数
	unsigned long int   f_bfree;   // 可用块数
	unsigned long int   f_bavail;  // 非超级用户可获取的块数
	unsigned long int   f_files;   // 文件节点总数
	unsigned long int   f_ffree;   // 可用文件节点总数
	fsid_t       f_fsid;    // 文件系统标识
	int f_namelen; // 文件名的最大长度
	int f_frsize;  // 碎片大小 (since Linux 2.6)
	int f_spare[5];
};

// Protocol families.
#define	PF_UNSPEC	0	/* Unspecified.  */
#define	PF_LOCAL	1	/* Local to host (pipes and file-domain).  */
#define	PF_UNIX		PF_LOCAL /* Old BSD name for PF_LOCAL.  */
#define	PF_FILE		PF_LOCAL /* Another non-standard name for PF_LOCAL.  */
#define	PF_INET		2	/* IP protocol family.  */
#define	PF_AX25		3	/* Amateur Radio AX.25.  */
#define	PF_IPX		4	/* Novell Internet Protocol.  */
#define	PF_APPLETALK	5	/* Appletalk DDP.  */
#define	PF_NETROM	6	/* Amateur radio NetROM.  */
#define	PF_BRIDGE	7	/* Multiprotocol bridge.  */
#define	PF_ATMPVC	8	/* ATM PVCs.  */
#define	PF_X25		9	/* Reserved for X.25 project.  */
#define	PF_INET6	10	/* IP version 6.  */
#define	PF_ROSE		11	/* Amateur Radio X.25 PLP.  */
#define	PF_DECnet	12	/* Reserved for DECnet project.  */
#define	PF_NETBEUI	13	/* Reserved for 802.2LLC project.  */
#define	PF_SECURITY	14	/* Security callback pseudo AF.  */
#define	PF_KEY		15	/* PF_KEY key management API.  */
#define	PF_NETLINK	16
#define	PF_ROUTE	PF_NETLINK /* Alias to emulate 4.4BSD.  */
#define	PF_PACKET	17	/* Packet family.  */
#define	PF_ASH		18	/* Ash.  */
#define	PF_ECONET	19	/* Acorn Econet.  */
#define	PF_ATMSVC	20	/* ATM SVCs.  */
#define PF_RDS		21	/* RDS sockets.  */
#define	PF_SNA		22	/* Linux SNA Project */
#define	PF_IRDA		23	/* IRDA sockets.  */
#define	PF_PPPOX	24	/* PPPoX sockets.  */
#define	PF_WANPIPE	25	/* Wanpipe API sockets.  */
#define PF_LLC		26	/* Linux LLC.  */
#define PF_CAN		29	/* Controller Area Network.  */
#define PF_TIPC		30	/* TIPC sockets.  */
#define	PF_BLUETOOTH	31	/* Bluetooth sockets.  */
#define	PF_IUCV		32	/* IUCV sockets.  */
#define PF_RXRPC	33	/* RxRPC sockets.  */
#define PF_ISDN		34	/* mISDN sockets.  */
#define PF_PHONET	35	/* Phonet sockets.  */
#define PF_IEEE802154	36	/* IEEE 802.15.4 sockets.  */
#define PF_CAIF		37	/* CAIF sockets.  */
#define PF_ALG		38	/* Algorithm sockets.  */
#define PF_NFC		39	/* NFC sockets.  */
#define	PF_MAX		40	/* For now..  */

// Address families.
#define	AF_UNSPEC	PF_UNSPEC
#define	AF_LOCAL	PF_LOCAL
#define	AF_UNIX		PF_UNIX
#define	AF_FILE		PF_FILE
#define	AF_INET		PF_INET
#define	AF_AX25		PF_AX25
#define	AF_IPX		PF_IPX
#define	AF_APPLETALK	PF_APPLETALK
#define	AF_NETROM	PF_NETROM
#define	AF_BRIDGE	PF_BRIDGE
#define	AF_ATMPVC	PF_ATMPVC
#define	AF_X25		PF_X25
#define	AF_INET6	PF_INET6
#define	AF_ROSE		PF_ROSE
#define	AF_DECnet	PF_DECnet
#define	AF_NETBEUI	PF_NETBEUI
#define	AF_SECURITY	PF_SECURITY
#define	AF_KEY		PF_KEY
#define	AF_NETLINK	PF_NETLINK
#define	AF_ROUTE	PF_ROUTE
#define	AF_PACKET	PF_PACKET
#define	AF_ASH		PF_ASH
#define	AF_ECONET	PF_ECONET
#define	AF_ATMSVC	PF_ATMSVC
#define AF_RDS		PF_RDS
#define	AF_SNA		PF_SNA
#define	AF_IRDA		PF_IRDA
#define	AF_PPPOX	PF_PPPOX
#define	AF_WANPIPE	PF_WANPIPE
#define AF_LLC		PF_LLC
#define AF_CAN		PF_CAN
#define AF_TIPC		PF_TIPC
#define	AF_BLUETOOTH	PF_BLUETOOTH
#define AF_IUCV		PF_IUCV
#define AF_RXRPC	PF_RXRPC
#define AF_ISDN		PF_ISDN
#define AF_PHONET	PF_PHONET
#define AF_IEEE802154	PF_IEEE802154
#define AF_CAIF		PF_CAIF
#define AF_ALG		PF_ALG
#define AF_NFC		PF_NFC
#define	AF_MAX		PF_MAX

/* Socket level values.  Others are defined in the appropriate headers.

   XXX These definitions also should go into the appropriate headers as
   far as they are available.  */
#define SOL_RAW		255
#define SOL_DECNET      261
#define SOL_X25         262
#define SOL_PACKET	263
#define SOL_ATM		264	/* ATM layer (cell level).  */
#define SOL_AAL		265	/* ATM Adaption Layer (packet level).  */
#define SOL_IRDA	266

// Maximum queue length specifiable by listen.
#define SOMAXCONN	128

/* Bits in the FLAGS argument to `send', `recv', et al.  */
enum
{
    MSG_OOB		= 0x01,	/* Process out-of-band data.  */
#define MSG_OOB		MSG_OOB
    MSG_PEEK		= 0x02,	/* Peek at incoming messages.  */
#define MSG_PEEK	MSG_PEEK
    MSG_DONTROUTE	= 0x04,	/* Don't use local routing.  */
#define MSG_DONTROUTE	MSG_DONTROUTE
#ifdef __USE_GNU
    /* DECnet uses a different name.  */
    MSG_TRYHARD		= MSG_DONTROUTE,
# define MSG_TRYHARD	MSG_DONTROUTE
#endif
    MSG_CTRUNC		= 0x08,	/* Control data lost before delivery.  */
#define MSG_CTRUNC	MSG_CTRUNC
    MSG_PROXY		= 0x10,	/* Supply or ask second address.  */
#define MSG_PROXY	MSG_PROXY
    MSG_TRUNC		= 0x20,
#define	MSG_TRUNC	MSG_TRUNC
    MSG_DONTWAIT	= 0x40, /* Nonblocking IO.  */
#define	MSG_DONTWAIT	MSG_DONTWAIT
    MSG_EOR		= 0x80, /* End of record.  */
#define	MSG_EOR		MSG_EOR
    MSG_WAITALL		= 0x100, /* Wait for a full request.  */
#define	MSG_WAITALL	MSG_WAITALL
    MSG_FIN		= 0x200,
#define	MSG_FIN		MSG_FIN
    MSG_SYN		= 0x400,
#define	MSG_SYN		MSG_SYN
    MSG_CONFIRM		= 0x800, /* Confirm path validity.  */
#define	MSG_CONFIRM	MSG_CONFIRM
    MSG_RST		= 0x1000,
#define	MSG_RST		MSG_RST
    MSG_ERRQUEUE	= 0x2000, /* Fetch message from error queue.  */
#define	MSG_ERRQUEUE	MSG_ERRQUEUE
    MSG_NOSIGNAL	= 0x4000, /* Do not generate SIGPIPE.  */
#define	MSG_NOSIGNAL	MSG_NOSIGNAL
    MSG_MORE		= 0x8000,  /* Sender will send more.  */
#define	MSG_MORE	MSG_MORE
    MSG_WAITFORONE	= 0x10000, /* Wait for at least one packet to return.*/
#define MSG_WAITFORONE	MSG_WAITFORONE
    MSG_CMSG_CLOEXEC	= 0x40000000	/* Set close_on_exit for file
					   descriptor received through
					   SCM_RIGHTS.  */
#define MSG_CMSG_CLOEXEC MSG_CMSG_CLOEXEC
};

/* Types of sockets.  */
enum __socket_type
{
  SOCK_STREAM = 1,		/* Sequenced, reliable, connection-based
				   byte streams.  */
#define SOCK_STREAM SOCK_STREAM
  SOCK_DGRAM = 2,		/* Connectionless, unreliable datagrams
				   of fixed maximum length.  */
#define SOCK_DGRAM SOCK_DGRAM
  SOCK_RAW = 3,			/* Raw protocol interface.  */
#define SOCK_RAW SOCK_RAW
  SOCK_RDM = 4,			/* Reliably-delivered messages.  */
#define SOCK_RDM SOCK_RDM
  SOCK_SEQPACKET = 5,		/* Sequenced, reliable, connection-based,
				   datagrams of fixed maximum length.  */
#define SOCK_SEQPACKET SOCK_SEQPACKET
  SOCK_DCCP = 6,		/* Datagram Congestion Control Protocol.  */
#define SOCK_DCCP SOCK_DCCP
  SOCK_PACKET = 10,		/* Linux specific way of getting packets
				   at the dev level.  For writing rarp and
				   other similar things on the user level. */
#define SOCK_PACKET SOCK_PACKET

  /* Flags to be ORed into the type parameter of socket and socketpair.  */

  SOCK_CLOEXEC = 02000000,	/* Atomically set close-on-exec flag for the
				   new descriptor(s).  */
#define SOCK_CLOEXEC SOCK_CLOEXEC
  SOCK_NONBLOCK = 00004000	/* Atomically mark descriptor(s) as
				   non-blocking.  */
#define SOCK_NONBLOCK SOCK_NONBLOCK
};

enum
{
    IPPROTO_IP = 0,	   /* Dummy protocol for TCP.  */
#define IPPROTO_IP		IPPROTO_IP
    IPPROTO_HOPOPTS = 0,   /* IPv6 Hop-by-Hop options.  */
#define IPPROTO_HOPOPTS		IPPROTO_HOPOPTS
    IPPROTO_ICMP = 1,	   /* Internet Control Message Protocol.  */
#define IPPROTO_ICMP		IPPROTO_ICMP
    IPPROTO_IGMP = 2,	   /* Internet Group Management Protocol. */
#define IPPROTO_IGMP		IPPROTO_IGMP
    IPPROTO_IPIP = 4,	   /* IPIP tunnels (older KA9Q tunnels use 94).  */
#define IPPROTO_IPIP		IPPROTO_IPIP
    IPPROTO_TCP = 6,	   /* Transmission Control Protocol.  */
#define IPPROTO_TCP		IPPROTO_TCP
    IPPROTO_EGP = 8,	   /* Exterior Gateway Protocol.  */
#define IPPROTO_EGP		IPPROTO_EGP
    IPPROTO_PUP = 12,	   /* PUP protocol.  */
#define IPPROTO_PUP		IPPROTO_PUP
    IPPROTO_UDP = 17,	   /* User Datagram Protocol.  */
#define IPPROTO_UDP		IPPROTO_UDP
    IPPROTO_IDP = 22,	   /* XNS IDP protocol.  */
#define IPPROTO_IDP		IPPROTO_IDP
    IPPROTO_TP = 29,	   /* SO Transport Protocol Class 4.  */
#define IPPROTO_TP		IPPROTO_TP
    IPPROTO_DCCP = 33,	   /* Datagram Congestion Control Protocol.  */
#define IPPROTO_DCCP		IPPROTO_DCCP
    IPPROTO_IPV6 = 41,     /* IPv6 header.  */
#define IPPROTO_IPV6		IPPROTO_IPV6
    IPPROTO_ROUTING = 43,  /* IPv6 routing header.  */
#define IPPROTO_ROUTING		IPPROTO_ROUTING
    IPPROTO_FRAGMENT = 44, /* IPv6 fragmentation header.  */
#define IPPROTO_FRAGMENT	IPPROTO_FRAGMENT
    IPPROTO_RSVP = 46,	   /* Reservation Protocol.  */
#define IPPROTO_RSVP		IPPROTO_RSVP
    IPPROTO_GRE = 47,	   /* General Routing Encapsulation.  */
#define IPPROTO_GRE		IPPROTO_GRE
    IPPROTO_ESP = 50,      /* encapsulating security payload.  */
#define IPPROTO_ESP		IPPROTO_ESP
    IPPROTO_AH = 51,       /* authentication header.  */
#define IPPROTO_AH		IPPROTO_AH
    IPPROTO_ICMPV6 = 58,   /* ICMPv6.  */
#define IPPROTO_ICMPV6		IPPROTO_ICMPV6
    IPPROTO_NONE = 59,     /* IPv6 no next header.  */
#define IPPROTO_NONE		IPPROTO_NONE
    IPPROTO_DSTOPTS = 60,  /* IPv6 destination options.  */
#define IPPROTO_DSTOPTS		IPPROTO_DSTOPTS
    IPPROTO_MTP = 92,	   /* Multicast Transport Protocol.  */
#define IPPROTO_MTP		IPPROTO_MTP
    IPPROTO_ENCAP = 98,	   /* Encapsulation Header.  */
#define IPPROTO_ENCAP		IPPROTO_ENCAP
    IPPROTO_PIM = 103,	   /* Protocol Independent Multicast.  */
#define IPPROTO_PIM		IPPROTO_PIM
    IPPROTO_COMP = 108,	   /* Compression Header Protocol.  */
#define IPPROTO_COMP		IPPROTO_COMP
    IPPROTO_SCTP = 132,	   /* Stream Control Transmission Protocol.  */
#define IPPROTO_SCTP		IPPROTO_SCTP
    IPPROTO_UDPLITE = 136, /* UDP-Lite protocol.  */
#define IPPROTO_UDPLITE		IPPROTO_UDPLITE
    IPPROTO_RAW = 255,	   /* Raw IP packets.  */
#define IPPROTO_RAW		IPPROTO_RAW
    IPPROTO_MAX
};

typedef unsigned short int sa_family_t;
#define	__SOCKADDR_COMMON(sa_prefix)    sa_family_t sa_prefix##family
struct sockaddr
{
	__SOCKADDR_COMMON (sa_);	// Common data: address family and length.
	char sa_data[14];		// Address data.
}; 
typedef unsigned short int in_port_t;
typedef unsigned int in_addr_t;
struct in_addr
{
	in_addr_t s_addr;
};
#define __SOCKADDR_COMMON_SIZE	(sizeof (unsigned short int))
struct sockaddr_in
{
	__SOCKADDR_COMMON (sin_);
	in_port_t sin_port;			//端口号 字节顺序：倒序
	struct in_addr sin_addr;	//IP地址，注:字节顺序 "192.168.0.1" = 0x0100A8C0

	/* Pad to size of `struct sockaddr'.  */
	unsigned char sin_zero[sizeof (struct sockaddr) -
			 __SOCKADDR_COMMON_SIZE -
			 sizeof (in_port_t) -
			 sizeof (struct in_addr)];
};

typedef unsigned long int pthread_t;
#define __SIZEOF_PTHREAD_ATTR_T 36
typedef union
{
  char __size[__SIZEOF_PTHREAD_ATTR_T];
  long int __align_;
} pthread_attr_t;

#endif

typedef long int __fd_mask;
#define __FD_SETSIZE        1024
#define __NFDBITS	(8 * (int) sizeof (__fd_mask))
/* fd_set for select and pselect.  */
typedef struct
  {
    /* XPG4.2 requires this member name.  Otherwise avoid the name
       from the global namespace.  */
#ifdef __USE_XOPEN
    __fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->fds_bits)
#else
    __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->__fds_bits)
#endif
  } fd_set;
struct timeval{
long int tv_sec; 
long int tv_usec; 
};
typedef int socklen_t;


typedef struct
{
//重启OS
void (*ReSTART_OS)(void);//重启OS(经实测直到MS在OS启动后的初始化完成需37ms)
//暂停os
void (*Disable_os)(void);//暂停os
//允许os
void (*Enable_os)(void);//允许os
	
//动态内存分配
void *(*ms_malloc) (size_t size);//动态分配我的内存堆
void *(*ms_calloc)(size_t size);//动态分配我的内存堆,并清0
void *(*ms_realloc)(void *pold,size_t newsize);//动态再分配我的内存堆,并copy原数据
void (*ms_free) (void * p);//释放我的内存堆分配
u32 (*ms_malloc_sizemax)(void);//最大可分配的尺寸(字节数)
u32 (*ms_realloc_sizemax)(void *pold);//最大可再分配的尺寸(字节数)
	
//msFILE
void (*msfile_init)(void);//msfile初始化
void (*msfile_format)(void);//msfile格式化化
u32 (*msfile_free)(void);//msfile返回文件自由空间
u32 (*msfile_write)(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile写;当offset>=文件总长度时自动加在文件结尾;返回:写入字节数
u32 (*msfile_read)(u32 filename,u32 offset,u8 *buf,u32 byte);//msfile读;返回:读出字节数
u32 (*msfile_Delete)(u32 filename);//msfile删除;返回:0=失败,1=成功
u32 (*msfile_len)(u32 filename);//msfile得到文件总长度
u32 (*msfile_set_lenmin)(u32 filename,u32 lenmin);//msfile设置预留最小文件长度(原文件有效数据总长度及内容不变只预占用最小存储空间,;返回:0=失败,1=成功
u32 (*msfile_check)(u32 filename);//msfile文件检查;返回:0=空,1=有效文件
u32 (*msfile_rename)(u32 filename,u32 newfilename);//文件名重命名:返回:0=错误,1=成功

#if MainProtocol==698
void (*msfile_lenclear)(u32 filename);//清0文件相关的长度次数等
u32 (*msfile_getfilename)(u32 FileID);//搜索符合任务FileID的文件名:返回:0-(FILENAMEmax_FLASH+FILENAMEmax_RAM-1),0xffffffff表示没找
#endif

//MRW
#if NAND128kBuff==0//定义128k nand写缓冲;0=4k BKPSRAM,1=128k SDRAM
void (*NAND4kBuff_Write_NAND)(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//连同4K(BKPSRAM)缓冲数据写到NAND FLASH
#else
void (*NAND128kBuff_Write_NAND)(u32 ADDR_Sour0,u32 ADDR_Dest0,u32 Byte0);//连同128K(BKPSDRAM)缓冲数据写到NAND FLASH
#endif
void (*MR)(u32 ADDR_Dest,u32 ADDR_Sour,u32 Byte);//存储器读
void (*MW)(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//存储器写
void (*MC)(u32 Data,u32 ADDR_Dest,u32 Byte);//存储器清0
void (*ME)(u32 ADDR_Dest,u32 Byte);//FLASH块擦除
u64 (*MRR)(u32 ADDR_Sour,u32 Byte);//存储器读到寄存器(<=8字节)
void (*MWR)(u64 Data,u32 ADDR_Dest,u32 Byte);//寄存器写到存储器(<=8字节)

//LCD
void (*LCDDataOut)(void);//点阵数据输出到LCD屏


//ext add
void (*Init_my_heap)(void);//初始化与内核RTOS无关的我的内存堆
void (*Main_Thread)(void);//主线程

//---LINUX add---
#ifdef LINUX
void (*DMW)(u32 ADDR_Sour,u32 ADDR_Dest,u32 Byte);//直接存储器写(无擦除)
void (*DMC)(u8 Data,u32 ADDR_Dest,u32 Byte);//直接存储器清除(无擦除)
void (*DMWR)(u64 Data,u32 ADDR_Dest,u8 Byte);//寄存器直接写到存储器(<=8字节)(无擦除)
void (*PowerDown_Data_Save)(void);//电源掉电数据保存(实测NUC97x需0.5秒)
void (*Pin_Configure)(u32 PinName,u32 Configure);//NUC97x引脚配置
void (*Pin_OutH)(u32 PinName);//引脚输出高,入口引脚使用名
void (*Pin_OutL)(u32 PinName);//引脚输出低,入口引脚使用名
u32 (*Pin_Read)(u32 PinName);//读引脚,入口引脚使用名;返回0或1
u32 (*off_irq)(void);
void (*destore_irq)(u32 irq);
unsigned long (*EnableInterrupt)(unsigned long eIntNo);
void (*save_kernel_reg)(void);
void (*destore_kernel_reg)(void);
void (*UARTx_Init) (u32 NUC97xUARTn,u32 BpsCtrl,u32 SrcCLK);//入口:URn=0-10,BpsCtrl=速率控制,SrcCLK 0=XIN 2=ACLK 3=UCLK
void (*Start_UARTn_Tx)(u32 PORTn);//启动UART口发送
u32 (*Wait_UARTn_Tx)(u32 PORTn);//等待UART口发送,返回0=发送完成,1=发送没完成
void (*SoftDelayUS)(u32 us);//软件us延时(TMR4)
void (*SoftDelayMS)(u32 ms);//软件ms延时(TMR4)
void (*WWDT_RESET)(void);//立即使能看门狗复位
void (*Battery_Data_Save)(void);//电池工作时关机前数据保存


//link linux
int (*link_printf) (char* str,int x,int y,int z);
int (*link_system) (char *command);
int (*link_open)(char *filename, int flags);
u32 (*link_lseek)(int filedes, u32 offset, int whence);
u32 (*link_read) (int filedes, void *buffer, u32 size);
u32 (*link_write) (int filedes, void *buffer, u32 size);
int (*link_close) (int filedes);
int (*link_remove) (char *filename);
int (*link_statfs)(char *path, struct statfs *diskinfo);

int (*link_setsockopt)(int sock, int level, int optname, const void *optval, u32 optlen);// #include <sys/socket.h>
int (*link_socket)(int protofamily, int type, int protocol);//#include<sys/socket.h>
int (*link_bind)(int sockfd,const struct sockaddr* myaddr,u32 addrlen);//#include<sys/socket.h>
int (*link_connect)(int sockfd, const struct sockaddr *addr, u32 addrlen);//#include<sys/socket.h>
int (*link_accept)(int sockfd, struct sockaddr *addr, u32 *addrlen);//#include<sys/socket.h>
int (*link_listen)(int sockfd, int backlog);//#include<sys/socket.h>
u32 (*link_send) (int fd,  void *buf, u32 n, int flags);
u32 (*link_recv) (int fd, void *buf, u32 n, int flags);
u32 (*link_sendto) (int __fd,  void *__buf, u32 __n, int __flags, struct sockaddr * __addr,u32 __addr_len);
u32 (*link_recvfrom) (int __fd, void * __buf, u32 __n,int __flags, struct sockaddr * __addr,u32 * __addr_len);
int (*link_pthread_create) (pthread_t *__newthread, pthread_attr_t * __attr,void *(*__start_routine) (void *), void * __arg);
u32 (*link_sleep) (u32 seconds);
u32 (*link_usleep) (u32 useconds);
int (*ioctl) (int fd, unsigned long int request, unsigned long int arg);
u32 (*linux_uart_receive)(u32 PORTn);//UART接收;返回：接收到的字节数
void (*linux_Init_UARTn)(u32 PORTn);//初始化UART口,入口通信控制字已填入
int (*link_errno)(void);//返回错误号
int (*link_fcntl)(int fd, int cmd, int arg);//获得/设置文件描述符
//int (*link_select) (int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);
//int (*getsockopt)(int sock, int level, int optname, void *optval, socklen_t *optlen);
//int (*setsockopt)(int sock, int level, int optname, const void *optval, socklen_t optlen);
#endif

}const ms_Type;

ms_Type *Get_ms(void);//得到ms表地址





#ifdef __cplusplus
 }
#endif 
#endif

