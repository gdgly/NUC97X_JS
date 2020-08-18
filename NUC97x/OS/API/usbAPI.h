
#ifndef usbAPI_H
#define usbAPI_H
#ifdef __cplusplus
 extern "C" {
#endif


//#define SD0_DRIVE		0        // for SD0          
//#define SD1_DRIVE		1        // for SD1          
//#define EMMC_DRIVE		2        // for eMMC/NAND    
//#define USBH_DRIVE_0    3        // USB Mass Storage 
//#define USBH_DRIVE_1    4        // USB Mass Storage 
//#define USBH_DRIVE_2    5        // USB Mass Storage 
//#define USBH_DRIVE_3    6        // USB Mass Storage 
//#define USBH_DRIVE_4    7        // USB Mass Storage 	 
#include "../../OS/ThirdParty\FATFS\src\ffconf.h"

	 
typedef unsigned char	BYTE;
// These types MUST be 16 bit
typedef short			SHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;
//These types MUST be 16 bit or 32 bit
typedef int				INT;
typedef unsigned int	UINT;
// These types MUST be 32 bit
typedef long			LONG;
typedef unsigned long	DWORD;


// File access control and file status flags (FIL.flag) 
#define	FA_READ				0x01
#define	FA_OPEN_EXISTING	0x00
#if !_FS_READONLY
#define	FA_WRITE			0x02
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define FA__WRITTEN			0x20
#define FA__DIRTY			0x40
#endif

//
#if _LFN_UNICODE//Unicode string
#if !_USE_LFN
#error _LFN_UNICODE must be 0 at non-LFN cfg.
#endif
#ifndef _INC_TCHAR
typedef WCHAR TCHAR;
#define _T(x) L ## x
#define _TEXT(x) L ## x
#endif

#else	//ANSI/OEM string
#ifndef _INC_TCHAR
typedef char TCHAR;
#define _T(x) x
#define _TEXT(x) x
#endif
#endif

typedef struct {
	BYTE	fs_type;		// FAT sub-type (0:Not mounted) 
	BYTE	drv;			// Physical drive number 
	BYTE	csize;			// Sectors per cluster (1,2,4...128) 
	BYTE	n_fats;			// Number of FAT copies (1 or 2) 
	BYTE	wflag;			// win[] flag (b0:dirty) 
	BYTE	fsi_flag;		// FSINFO flags (b7:disabled, b0:dirty) 
	WORD	id;				// File system mount ID 
	WORD	n_rootdir;		// Number of root directory entries (FAT12/16) 
#if _MAX_SS != _MIN_SS
	WORD	ssize;			// Bytes per sector (512, 1024, 2048 or 4096) 
#endif
#if _FS_REENTRANT
	_SYNC_t	sobj;			// Identifier of sync object 
#endif
#if !_FS_READONLY
	DWORD	last_clust;		// Last allocated cluster 
	DWORD	free_clust;		// Number of free clusters 
#endif
#if _FS_RPATH
	DWORD	cdir;			// Current directory start cluster (0:root) 
#endif
	DWORD	n_fatent;		// Number of FAT entries, = number of clusters + 2 
	DWORD	fsize;			// Sectors per FAT 
	DWORD	volbase;		// Volume start sector 
	DWORD	fatbase;		// FAT start sector 
	DWORD	dirbase;		// Root directory start sector (FAT32:Cluster#) 
	DWORD	database;		// Data start sector 
	DWORD	winsect;		// Current sector appearing in the win[] 
	BYTE	win[_MAX_SS];	// Disk access window for Directory, FAT (and file data at tiny cfg) 
} FATFS;

typedef struct {
	FATFS*	fs;				// Pointer to the related file system object (**do not change order**) 
	WORD	id;				// Owner file system mount ID (**do not change order**) 
	BYTE	flag;			// Status flags 
	BYTE	err;			// Abort flag (error code) 
	DWORD	fptr;			// File read/write pointer (Zeroed on file open) 
	DWORD	fsize;			// File size 
	DWORD	sclust;			// File start cluster (0:no cluster chain, always 0 when fsize is 0) 
	DWORD	clust;			// Current cluster of fpter (not valid when fprt is 0) 
	DWORD	dsect;			// Sector number appearing in buf[] (0:invalid) 
#if !_FS_READONLY
	DWORD	dir_sect;		// Sector number containing the directory entry 
	BYTE*	dir_ptr;		// Pointer to the directory entry in the win[] 
#endif
#if _USE_FASTSEEK
	DWORD*	cltbl;			// Pointer to the cluster link map table (Nulled on file open) 
#endif
#if _FS_LOCK
	UINT	lockid;			// File lock ID origin from 1 (index of file semaphore table Files[]) 
#endif
#if !_FS_TINY
	BYTE	buf[_MAX_SS];	// File private data read/write window 
#endif
} FIL;
	 
typedef struct {
	FATFS*	fs;				// Pointer to the owner file system object (**do not change order**) 
	WORD	id;				// Owner file system mount ID (**do not change order**) 
	WORD	index;			// Current read/write index number 
	DWORD	sclust;			// Table start cluster (0:Root dir) 
	DWORD	clust;			// Current cluster 
	DWORD	sect;			// Current sector 
	BYTE*	dir;			// Pointer to the current SFN entry in the win[] 
	BYTE*	fn;				// Pointer to the SFN (in/out) {file[8],ext[3],status[1]} 
#if _FS_LOCK
	UINT	lockid;			// File lock ID (index of file semaphore table Files[]) 
#endif
#if _USE_LFN
	WCHAR*	lfn;			// Pointer to the LFN working buffer 
	WORD	lfn_idx;		// Last matched LFN index number (0xFFFF:No LFN) 
#endif
#if _USE_FIND
	const TCHAR*	pat;	// Pointer to the name matching pattern 
#endif
} DIR;	 
	 
typedef struct {
	DWORD	fsize;			// File size 
	WORD	fdate;			// Last modified date 
	WORD	ftime;			// Last modified time 
	BYTE	fattrib;		// Attribute 
	TCHAR	fname[13];		// Short file name (8.3 format) 
#if _USE_LFN
	TCHAR*	lfname;			// Pointer to the LFN buffer 
	UINT 	lfsize;			// Size of LFN buffer in TCHAR 
#endif
} FILINFO;

typedef enum {
	FR_OK = 0,				// (0) Succeeded 
	FR_DISK_ERR,			// (1) A hard error occurred in the low level disk I/O layer 
	FR_INT_ERR,				// (2) Assertion failed 
	FR_NOT_READY,			// (3) The physical drive cannot work 
	FR_NO_FILE,				// (4) Could not find the file 
	FR_NO_PATH,				// (5) Could not find the path 
	FR_INVALID_NAME,		// (6) The path name format is invalid 
	FR_DENIED,				// (7) Access denied due to prohibited access or directory full 
	FR_EXIST,				// (8) Access denied due to prohibited access 
	FR_INVALID_OBJECT,		// (9) The file/directory object is invalid 
	FR_WRITE_PROTECTED,		// (10) The physical drive is write protected 
	FR_INVALID_DRIVE,		// (11) The logical drive number is invalid 
	FR_NOT_ENABLED,			// (12) The volume has no work area 
	FR_NO_FILESYSTEM,		// (13) There is no valid FAT volume 
	FR_MKFS_ABORTED,		// (14) The f_mkfs() aborted due to any parameter error 
	FR_TIMEOUT,				// (15) Could not get a grant to access the volume within defined period 
	FR_LOCKED,				// (16) The operation is rejected according to the file sharing policy 
	FR_NOT_ENOUGH_CORE,		// (17) LFN working buffer could not be allocated 
	FR_TOO_MANY_OPEN_FILES,	// (18) Number of open files > _FS_SHARE 
	FR_INVALID_PARAMETER	// (19) Given parameter is invalid 
} FRESULT;


	 
void USBH_Initialize(void);
int  usbh_pooling_hubs(void);
//FatFs module application interface
FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);				// Open or create a file 
FRESULT f_close (FIL* fp);											// Close an open file object 
FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			// Read data from a file 
FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	// Write data to a file 
FRESULT f_lseek (FIL* fp, DWORD ofs);								// Move file pointer of a file object 
FRESULT f_opendir (DIR* dp, const TCHAR* path);						// Open a directory 
FRESULT f_closedir (DIR* dp);										// Close an open directory 
FRESULT f_readdir (DIR* dp, FILINFO* fno);							// Read a directory item 
FRESULT f_findfirst (DIR* dp, FILINFO* fno, const TCHAR* path, const TCHAR* pattern);	// Find first file 
FRESULT f_findnext (DIR* dp, FILINFO* fno);							// Find next file 
FRESULT f_mkdir (const TCHAR* path);								// Create a sub directory 
FRESULT f_unlink (const TCHAR* path);								// Delete an existing file or directory 
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);	// Rename/Move a file or directory 
FRESULT f_stat (const TCHAR* path, FILINFO* fno);					// Get file status 
FRESULT f_chmod (const TCHAR* path, BYTE attr, BYTE mask);			// Change attribute of the file/dir 
FRESULT f_utime (const TCHAR* path, const FILINFO* fno);			// Change times-tamp of the file/dir 
FRESULT f_chdir (const TCHAR* path);								// Change current directory 
FRESULT f_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);	// Get number of free clusters on the drive 
FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);			// Mount/Unmount a logical drive 
u32 f_READ(FIL* fp, void* buff,u32 BTR);// Read data from a file 
u32 f_WRITE(FIL* fp, const void* buff, UINT BTW);// Write data to a file 





#ifdef __cplusplus
 }
#endif 
#endif





