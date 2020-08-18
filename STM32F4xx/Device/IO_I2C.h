
//I2C
#ifndef I2C_H
#define I2C_H


u32 IOI2C_Start(u32 PIN_SCL,u32 PIN_SDA);//IO仿I2C启动,返回0=正确,1=错误
u32 IOI2C_ReStart(u32 PIN_SCL,u32 PIN_SDA);//IO仿I2C重启动,返回0=正确,1=错误
void IOI2C_Stop(u32 PIN_SCL,u32 PIN_SDA);//IO仿I2C停止
u32 IOI2C_Write(u32 PIN_SCL,u32 PIN_SDA,u32 DATA);//IO仿I2C字节写,返回0=正确,1=错误
u32 IOI2C_ReadAck(u32 PIN_SCL,u32 PIN_SDA);//IO仿I2C字节读ACK
u32 IOI2C_ReadNoAck(u32 PIN_SCL,u32 PIN_SDA);//IO仿I2C字节读NOACK






#endif
