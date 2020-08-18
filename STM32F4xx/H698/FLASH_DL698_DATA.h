

#ifndef FLASH_DL698_DATA_H
#define FLASH_DL698_DATA_H
#ifdef __cplusplus
 extern "C" {
#endif


//数据
	#if CompareMeterEventCount==1//全事件采集抄表是否比较发生次数:0=不比较(全部抄表数据写入文件),1=比较(只有次数变化的事件写入文件)
#define ADDR_SourMeterEventCount     ADDR_FLASH_DL698_DATA_START//全事件采集抄表电能表原事件发生次数(档案配置顺序排列,初始化值全部为0)
//对应档案配置号0
//6byte 电能表通信地址(当通信地址变化时清0列表个数)
//2byte 列表个数
//4byte 事件OAD
//4byte 原事件发生次数
//...
//对应档案配置号1
//6byte 电能表通信地址(当通信地址变化时清0列表个数)
//2byte 列表个数
//4byte 事件OAD
//4byte 原事件发生次数
//...

//...
//对应档案配置号n
//6byte 电能表通信地址(当通信地址变化时清0列表个数)
//2byte 列表个数
//4byte 事件OAD
//4byte 原事件发生次数
//...
#define LENper_SourMeterEventCount     512//至少大于8
#define ADDR_FLASH_DL698_DATA_END1     ADDR_SourMeterEventCount+(LENper_SourMeterEventCount*NUM_RMmax)
	#endif

	#ifndef ADDR_FLASH_DL698_DATA_END1
#define ADDR_FLASH_DL698_DATA_END1     ADDR_FLASH_DL698_DATA_START
	#endif


#if (USER/100)==15//安徽
#define ADDR_2131_All    		ADDR_FLASH_DL698_DATA_END1//存储每次收到ABC相电压合格率存储的数据,保存一个月31天的数据
#define LENmax_2131_All     (48*NUM_RMmax)
#define ADDR_FLASH_DL698_DATA_END     ADDR_2131_All+(LENmax_2131_All*31*3)
#endif




#ifndef ADDR_FLASH_DL698_DATA_END
#define ADDR_FLASH_DL698_DATA_END     ADDR_FLASH_DL698_DATA_END1
#endif



#if ((ADDR_FLASH_DL698_DATA_END)-(ADDR_FLASH_DL698_DATA_START))>LEN_FLASH_DL698_DATA
	#error LEN_FLASH_DL698_DATA
#endif
	 
	 

#ifdef __cplusplus
}
#endif
#endif


