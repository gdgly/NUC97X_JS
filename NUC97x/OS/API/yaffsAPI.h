

#ifndef yaffsAPI_H
#define yaffsAPI_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
	 
#ifndef NAME_MAX
#define NAME_MAX	256
#endif
#ifndef YCHAR 
#define YCHAR char
#endif
#ifndef YUCHAR
#define YUCHAR unsigned char
#endif
#ifndef loff_t
typedef long long		loff_t;
#endif
#ifndef mode_t
typedef unsigned short	mode_t;
#endif
#ifndef off_t
typedef long		off_t;
#endif


struct yaffs_dirent {
	long d_ino;			/* inode number */
	off_t d_off;			/* offset to this dirent */
	unsigned short d_reclen;	/* length of this dirent */
	YUCHAR d_type;			/* type of this record */
	YCHAR d_name[NAME_MAX+1];	/* file name (null-terminated) */
	unsigned d_dont_use;		/* debug: not for public consumption */
};
typedef struct opaque_structure yaffs_DIR;

struct yaffs_stat {
	int		st_dev;		/* device */
	int		st_ino;		/* inode */
	unsigned	st_mode;	/* protection */
	int		st_nlink;	/* number of hard links */
	int		st_uid;		/* user ID of owner */
	int		st_gid;		/* group ID of owner */
	unsigned	st_rdev;	/* device type (if inode device) */
	loff_t		st_size;	/* total size, in bytes */
	unsigned long	st_blksize;	/* blocksize for filesystem I/O */
	unsigned long	st_blocks;	/* number of blocks allocated */
#ifdef CONFIG_YAFFS_WINCE
	/* Special 64-bit times for WinCE */
	unsigned long	yst_wince_atime[2];
	unsigned long	yst_wince_mtime[2];
	unsigned long	yst_wince_ctime[2];
#else
	unsigned long	yst_atime;	/* time of last access */
	unsigned long	yst_mtime;	/* time of last modification */
	unsigned long	yst_ctime;	/* time of last change */
#endif
};

void yaffs_Init(void);
int yaffs_mount(const YCHAR *path) ;
int yaffs_unmount(const YCHAR *path) ;
int yaffs_open(const YCHAR *path, int oflag, int mode) ;
int yaffs_close(int fd) ;
yaffs_DIR* yaffs_opendir(const YCHAR *dirname) ;
int yaffs_closedir(yaffs_DIR *dirp) ;
int yaffs_mkdir(const YCHAR *path, mode_t mode) ;
int yaffs_rmdir(const YCHAR *path) ;
int yaffs_read(int fd, void *buf, unsigned int nbyte) ;
int yaffs_write(int fd, const void *buf, unsigned int nbyte) ;
int yaffs_pread(int fd, void *buf, unsigned int nbyte, loff_t offset);
int yaffs_pwrite(int fd, const void *buf, unsigned int nbyte, loff_t offset);
int yaffs_unlink(const YCHAR *path) ;
int yaffs_rename(const YCHAR *oldPath, const YCHAR *newPath) ;
loff_t yaffs_freespace(const YCHAR *path);
loff_t yaffs_totalspace(const YCHAR *path);
int yaffs_stat(const YCHAR *path, struct yaffs_stat *buf);
int yaffs_fstat(int fd, struct yaffs_stat *buf);
int yaffs_chmod(const YCHAR *path, mode_t mode);
int yaffs_fchmod(int fd, mode_t mode);
struct yaffs_dirent *yaffs_readdir(yaffs_DIR * dirp);
u32 yaffs_READ(int fd, void *buf, u32 byte);
u32 yaffs_WRITE(int fd, const void *buf,u32 byte);
int yaffs_unlinkdir(const YCHAR *path) ;//删除目录下的全部文件
int yaffs_fsync(int fd) ;
int yaffs_sync(const YCHAR *path) ;




#ifndef O_RDONLY
#define O_RDONLY	00//只读模式
#endif

#ifndef O_WRONLY
#define O_WRONLY	01//只写模式
#endif

#ifndef O_RDWR
#define O_RDWR		02//读写模式
#endif

#ifndef O_CREAT
#define O_CREAT		0100//如果指定文件不存在，则创建这个文件
#endif

#ifndef O_EXCL
#define O_EXCL		0200//如果要创建的文件已存在，则返回-1，并且修改errno的值
#endif

#ifndef O_TRUNC
#define O_TRUNC		01000//如果文件存在，并且以只写/读写方式打开，则清空文件全部内容(即将其长度截短为0)
#endif

#ifndef O_APPEND
#define O_APPEND	02000//每次写操作都写入文件的末尾
#endif
//O_NOCTTY如果路径名指向终端设备，不要把这个设备用作控制终端
//O_NONBLOCK如果路径名指向FIFO/块文件/字符文件，则把文件的打开和后继I/O
//O_DSYNC等待物理I/O结束后再write。在不影响读取新写入的数据的前提下，不等待文件属性更新。
//O_RSYNCread等待所有写入同一区域的写操作完成后再进行
//O_SYNC等待物理I/O结束后再write，包括更新文件属性的I/O

#ifndef SEEK_SET
#define SEEK_SET	0
#endif

#ifndef SEEK_CUR
#define SEEK_CUR	1
#endif

#ifndef SEEK_END
#define SEEK_END	2
#endif

#ifndef EBUSY
#define EBUSY	16
#endif

#ifndef ENODEV
#define ENODEV	19
#endif

#ifndef EINVAL
#define EINVAL	22
#endif

#ifndef ENFILE
#define ENFILE	23
#endif

#ifndef EBADF
#define EBADF	9
#endif

#ifndef EACCES
#define EACCES	13
#endif

#ifndef EXDEV
#define EXDEV	18
#endif

#ifndef ENOENT
#define ENOENT	2
#endif

#ifndef ENOSPC
#define ENOSPC	28
#endif

#ifndef EROFS
#define EROFS	30
#endif

#ifndef ERANGE
#define ERANGE 34
#endif

#ifndef ENODATA
#define ENODATA 61
#endif

#ifndef ENOTEMPTY
#define ENOTEMPTY 39
#endif

#ifndef ENAMETOOLONG
#define ENAMETOOLONG 36
#endif

#ifndef ENOMEM
#define ENOMEM 12
#endif

#ifndef EFAULT
#define EFAULT 14
#endif

#ifndef EEXIST
#define EEXIST 17
#endif

#ifndef ENOTDIR
#define ENOTDIR 20
#endif

#ifndef EISDIR
#define EISDIR 21
#endif

#ifndef ELOOP
#define ELOOP	40
#endif


/* Mode flags */

#ifndef S_IFMT
#define S_IFMT		0170000
#endif

#ifndef S_IFSOCK
#define S_IFSOCK	0140000
#endif

#ifndef S_IFIFO
#define S_IFIFO		0010000
#endif

#ifndef S_IFCHR
#define S_IFCHR		0020000
#endif

#ifndef S_IFBLK
#define S_IFBLK		0060000
#endif

#ifndef S_IFLNK
#define S_IFLNK		0120000
#endif

#ifndef S_IFDIR
#define S_IFDIR		0040000
#endif

#ifndef S_IFREG
#define S_IFREG		0100000
#endif

#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)
#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)


#ifndef S_IREAD
#define S_IREAD		0000400
#endif

#ifndef S_IWRITE
#define	S_IWRITE	0000200
#endif

#ifndef S_IEXEC
#define	S_IEXEC	0000100
#endif

#ifndef XATTR_CREATE
#define XATTR_CREATE 1
#endif

#ifndef XATTR_REPLACE
#define XATTR_REPLACE 2
#endif

#ifndef R_OK
#define R_OK	4
#define W_OK	2
#define X_OK	1
#define F_OK	0
#endif



#ifdef __cplusplus
 }
#endif 
#endif





