

#ifndef IC_MESAM_H
#define IC_MESAM_H


void MESAM_RXTX(void);//MESAM接收发送
u32 MESAM_CMD_TX(u32 ClaInsP1P2,u32 Lc,u32 RxByte);//发送ESAM命令,入口参数说明见原函数;返回:0=正确发收且返回9000或过程字符,1=错误
u32 MESAM_DATA_TX(u32 ADDR_DATA,u32 Lc,u32 RxByte);//发送ESAM数据,入口参数说明见原函数;返回:0=正确发收且返回9000或0x61xx,1=错误
void MESAM_MACErrorCount(void);//MAC和密文校验错误计数加1次
void Write_MESAM_Run_File(void);//写ESAM运行信息文件
void MESAM_PAY(void);//ESAM钱包立即扣款
void MESAM_TIMER_PAY(void);//ESAM钱包定时扣款
void MESAM_DATA_COPY(void);//ESAM数据COPY到EFLASH
u32 MESAMKeyUpdata(void);//MESAM密钥更新,入口:密钥已存在FLASH;返回0=没更新,1=正确更新,2=密文错,3=ESAM返回错误
u32 MESAMKeyState(void);//按密钥总条数判断当前密钥状态;返回0=测试密钥,1=正式密钥

#endif

