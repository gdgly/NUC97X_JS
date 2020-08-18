

#ifndef YAFFSconfig
#define YAFFSconfig

#ifndef CONFIG_YAFFS_DIRECT
#define CONFIG_YAFFS_DIRECT//使用YAFFS直接接口
#endif
#ifndef CONFIG_YAFFS_SHORT_NAMES_IN_RAM
#define CONFIG_YAFFS_SHORT_NAMES_IN_RAM
#endif
#ifndef CONFIG_YAFFS_YAFFS2
#define CONFIG_YAFFS_YAFFS2//使用YAFFS2
#endif
#ifndef NO_Y_INLINE
#define NO_Y_INLINE
#endif
#ifndef CONFIG_YAFFS_PROVIDE_DEFS
#define CONFIG_YAFFS_PROVIDE_DEFS//提供一些文件类型的定义以及 #includes linux/types.h, linux/fs.h, linux/stat.h
#endif
#ifndef CONFIG_YAFFSFS_PROVIDE_VALUES
#define CONFIG_YAFFSFS_PROVIDE_VALUES// 可能使用此选项，这样你可以自定义错误值，以及一些常数，比如O_RDONLY和系统数值匹配, 如果没有定义这个选项，那么yaffs会包含errno.h,sys/stat.h and fcntl.h 
#endif


#endif
