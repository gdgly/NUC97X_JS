
#ifndef netAPI_H
#define netAPI_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdbool.h>
	 
// Network status code values returned by Network library functions.
typedef enum {
  netOK                       = 0,      // Operation succeeded
  netBusy,                              // Process is busy
  netError,                             // Unspecified error
  netInvalidParameter,                  // Invalid parameter specified
  netWrongState,                        // Wrong state error
  netDriverError,                       // Driver error
  netServerError                        // Server error
} netStatus;

// General definitions.
#define NET_ADDR_ETH_LEN        6       // Ethernet MAC Address Length in bytes
#define NET_ADDR_IP4_LEN        4       // IPv4 Address Length in bytes
#define NET_ADDR_IP6_LEN        16      // IPv6 Address Length in bytes
#define NET_HOSTNAME_LEN        16      // Hostname Buffer Length in bytes
// Network Address types.
#define NET_ADDR_IP4            0       // IPv4 Address
#define NET_ADDR_IP6            1       // IPv6 Address

// DHCP Option Codes.
#define NET_DHCP_OPTION_IP_ADDRESS     0 // IP address change event
#define NET_DHCP_OPTION_NTP_SERVERS   42 // NTP Servers option
#define NET_DHCP_OPTION_BOOTFILE_NAME 67 // Bootfile name option

// FTP Client Requests.
typedef enum {
  netFTPc_RequestUsername,              // Username to login to FTP server
  netFTPc_RequestPassword,              // Password to login to FTP server
  netFTPc_RequestDirectory,             // Working directory path on server for all commands
  netFTPc_RequestName,                  // File or Directory name for FTP commands 
  netFTPc_RequestNewName,               // New File or Directory name for RENAME command
  netFTPc_RequestListMask,              // File filter/mask for LIST command (wildcards allowed)
  netFTPc_RequestList,                  // Received data if LIST command is given
  netFTPc_RequestLocalFilename          // Local filename (including path)
} netFTPc_Request;

// FTP Client Events.
typedef enum {
  netFTPc_EventSuccess        = 0,      // File operation successful
  netFTPc_EventTimeout,                 // Timeout on file operation
  netFTPc_EventLoginFailed,             // Login error, username/password invalid
  netFTPc_EventAccessDenied,            // File access not allowed
  netFTPc_EventFileNotFound,            // File not found
  netFTPc_EventInvalidDirectory,        // Working directory path not found
  netFTPc_EventLocalFileError,          // Local file read/write error
  netFTPc_EventError                    // Generic FTP client error 
} netFTPc_Event;

// Network Address IPv4/IPv6 capable.
typedef struct net_addr {
  int16_t  addr_type;                   // IP address type: \ref NET_ADDR_IP4 or \ref NET_ADDR_IP6
  uint16_t port;                        // Internet socket port number
  uint8_t  addr[NET_ADDR_IP6_LEN];      // IPv4 or IPv6 address (array 16 bytes, MSB first)
} NET_ADDR;
// UDP Socket Options.
typedef enum {
  netUDP_OptionTOS            = 0,      // IPv4 Type of Service; val=TOS
  netUDP_OptionTTL,                     // IPv4 Multi-cast Time to Live; val=TTL
  netUDP_OptionTrafficClass,            // IPv6 Traffic Class; val=TrafficClass
  netUDP_OptionHopLimit,                // IPv6 Multi-cast Hop Limit; val=HopLimit
  netUDP_OptionChecksum                 // UDP Checksum Options
} netUDP_Option;
// TCP Socket Events.
typedef enum {
  netTCP_EventConnect         = 0,      // Connect request received event
  netTCP_EventEstablished,              // Connection established event
  netTCP_EventClosed,                   // Connection was properly closed
  netTCP_EventAborted,                  // Connection is for some reason aborted
  netTCP_EventACK,                      // Previously send data acknowledged
  netTCP_EventData                      // Data received event
} netTCP_Event;

// TCP Socket States.
typedef enum {
  netTCP_StateINVALID         =-1,      // Invalid Socket
  netTCP_StateUNUSED,                   // Entry is free and unused
  netTCP_StateCLOSED,                   // Entry allocated, socket still closed
  netTCP_StateLISTEN,                   // Socket waiting for incoming connection
  netTCP_StateSYN_RECEIVED,             // SYN frame received
  netTCP_StateSYN_SENT,                 // SYN packet sent to establish a connection
  netTCP_StateFIN_WAIT_1,               // Close started FIN packet was sent
  netTCP_StateFIN_WAIT_2,               // Our FIN ACK-ed, waiting for remote FIN
  netTCP_StateCLOSING,                  // Received FIN independently of our FIN
  netTCP_StateLAST_ACK,                 // Waiting for last ACK for our FIN
  netTCP_StateTIME_WAIT,                // Timed waiting for 2MSL
  netTCP_StateESTABLISHED               // TCP Connection established
} netTCP_State;

// TCP Socket Options.
typedef enum {
  netTCP_OptionTOS            = 0,      // IPv4 Type of Service; val=TOS
  netTCP_OptionTrafficClass,            // IPv6 Traffic Class; val=TrafficClass
  netTCP_OptionTimeout,                 // TCP Idle Timeout; val=timeout (in seconds)
  netTCP_OptionKeepAlive,               // TCP Keep Alive; val: 0=disabled (default), 1=enabled
  netTCP_OptionFlowControl,             // TCP Flow Control; val: 0=disabled (default), 1=enabled
  netTCP_OptionDelayedACK               // TCP Delayed Acknowledgment; val: 0=disabled (default), 1=enabled 
} netTCP_Option;



typedef uint32_t (*netUDP_cb_t)(int32_t socket, const NET_ADDR *addr, const uint8_t *buf, uint32_t len);
typedef uint32_t (*netTCP_cb_t)(int32_t socket, netTCP_Event event, const NET_ADDR *addr, const uint8_t *buf, uint32_t len);

//ETH0
netStatus netInitialize(void);//eth0 init
void netifInitialize(void);//netif_init
int32_t netTCP_GetSocket(netTCP_cb_t cb_func);
netStatus netTCP_ReleaseSocket(int32_t socket);
netStatus netTCP_Listen(int32_t socket, uint16_t port);
netStatus netTCP_Connect(int32_t socket, const NET_ADDR *addr, uint16_t local_port);
netStatus netTCP_Close(int32_t socket);
netStatus netTCP_Abort(int32_t socket);
uint32_t  netTCP_GetMaxSegmentSize(int32_t socket);
bool netTCP_SendReady(int32_t socket);
netStatus netTCP_Send(int32_t socket, uint8_t *buf, uint32_t len);
	 
int32_t netUDP_GetSocket(netUDP_cb_t cb_func); 
netStatus netUDP_ReleaseSocket(int32_t socket);
netStatus netUDP_Open(int32_t socket, uint16_t port);
netStatus netUDP_Close(int32_t socket);
netStatus netUDP_Send(int32_t socket, const NET_ADDR *addr, uint8_t *buf, uint32_t len);

netStatus netDHCP_Enable(uint32_t if_num);
netStatus netDHCP_Disable(uint32_t if_num);
	 
//ETH1
	#if RJ45SELECT==2//以太网RJ45通道选择:0=eth0(IP101),1=eth1(LAN8720),2=eth0和eth1二个独立口,eth0用于以太网RJ45通道
netStatus netInitialize1(void);//eth0 init
void netifInitialize1(void);//netif_init
int32_t netTCP_GetSocket1(netTCP_cb_t cb_func);
netStatus netTCP_ReleaseSocket1(int32_t socket);
netStatus netTCP_Listen1(int32_t socket, uint16_t port);
netStatus netTCP_Connect1(int32_t socket, const NET_ADDR *addr, uint16_t local_port);
netStatus netTCP_Close1(int32_t socket);
netStatus netTCP_Abort1(int32_t socket);
uint32_t  netTCP_GetMaxSegmentSize1(int32_t socket);
bool netTCP_SendReady1(int32_t socket);
netStatus netTCP_Send1(int32_t socket, uint8_t *buf, uint32_t len);
	 
int32_t netUDP_GetSocket1(netUDP_cb_t cb_func); 
netStatus netUDP_ReleaseSocket1(int32_t socket);
netStatus netUDP_Open1(int32_t socket, uint16_t port);
netStatus netUDP_Close1(int32_t socket);
netStatus netUDP_Send1(int32_t socket, const NET_ADDR *addr, uint8_t *buf, uint32_t len);

netStatus netDHCP_Enable1(uint32_t if_num);
netStatus netDHCP_Disable1(uint32_t if_num);
	#endif

	 
#ifdef __cplusplus
 }
#endif 
#endif
