

#ifndef YAFFSconfig
#define YAFFSconfig

#ifndef CONFIG_YAFFS_DIRECT
#define CONFIG_YAFFS_DIRECT//ʹ��YAFFSֱ�ӽӿ�
#endif
#ifndef CONFIG_YAFFS_SHORT_NAMES_IN_RAM
#define CONFIG_YAFFS_SHORT_NAMES_IN_RAM
#endif
#ifndef CONFIG_YAFFS_YAFFS2
#define CONFIG_YAFFS_YAFFS2//ʹ��YAFFS2
#endif
#ifndef NO_Y_INLINE
#define NO_Y_INLINE
#endif
#ifndef CONFIG_YAFFS_PROVIDE_DEFS
#define CONFIG_YAFFS_PROVIDE_DEFS//�ṩһЩ�ļ����͵Ķ����Լ� #includes linux/types.h, linux/fs.h, linux/stat.h
#endif
#ifndef CONFIG_YAFFSFS_PROVIDE_VALUES
#define CONFIG_YAFFSFS_PROVIDE_VALUES// ����ʹ�ô�ѡ�����������Զ������ֵ���Լ�һЩ����������O_RDONLY��ϵͳ��ֵƥ��, ���û�ж������ѡ���ôyaffs�����errno.h,sys/stat.h and fcntl.h 
#endif


#endif
