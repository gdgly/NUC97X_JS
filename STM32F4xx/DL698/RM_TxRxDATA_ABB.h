#ifndef RM_TxRxDATA_ABB_H
#define RM_TxRxDATA_ABB_H
#ifdef __cplusplus
extern "C" {
#endif

	
u32 ABB_CRC(u8* p8, u32 nByte); //ABB表CRC计算
	
#define  ABB_SUB_OFFSET     LEN_UART2Rx+LEN_UART2Tx //ABB分帧响应开始相对于接收缓存的的偏移
#define  ABB_RM_LINK        0                       //用于ABB预连接的标识，减少王总那边的程序改动
#define  ABB_RM_CLASS11     ABB_RM_LINK+2           //暂时没用
#define  ABB_KEY            ABB_RM_CLASS11+2        //存储ABB的远程连接密钥
#define  ABB_POWER_POINT    ABB_KEY+4               //电量小数位
#define  ABB_DEMAND_POINT   ABB_POWER_POINT+1       //需量小数位
#define  ABB_CLASS11_BLOCK1 ABB_DEMAND_POINT+1      //类11的数据含义
#define  ABB_CLASS11_BLOCK2 ABB_CLASS11_BLOCK1+1    //
#define  ABB_CLASS11_BLOCK3 ABB_CLASS11_BLOCK2+1    //
#define  ABB_CLASS11_BLOCK4 ABB_CLASS11_BLOCK3+1    //
#define  ABB_LEN_CLASS9     ABB_CLASS11_BLOCK4+1    //抄类9数据长度
#define  ABB_LEN_CLASS11    ABB_LEN_CLASS9+2        //抄类11数据长度

#define  ABB_LENMAX_PARA    32                      //前面的ABB抄表流程参数和抄表数据的最大长度
#define  ABB_LENMAX_RX_CLASS9    128                //接收ABB表数据类型9的最大长度
#define  ABB_LENMAX_RX_CLASS11   1024               //允许接收ABB表数据类型11的最大长度，
//1.实际最长帧用不到1024；2.预留的接收缓冲的长度不止1024
#define  ABB_LENMAX_POWER_CLASS11   0x016E
#if(ABB_LEN_CLASS11-ABB_RM_LINK > ABB_LENMAX_PARA)
#error "ABB抄表流程参数长度大于最大的允许值"
#endif

#define  ABB_RM_CLASS9_OFFSET      ABB_RM_LINK+ABB_LENMAX_PARA                  //抄类9数据
#define  ABB_RM_CLASS11_OFFSET     ABB_RM_CLASS9_OFFSET+ABB_LENMAX_RX_CLASS9      //抄类11数据

#if(ABB_RM_CLASS11_OFFSET+ABB_LENMAX_RX_CLASS11-ABB_RM_LINK > LEN_UART2Rx)
#error "ABB抄表数据长度大于最大的允许值"
#endif


#ifdef __cplusplus
}
#endif
#endif
