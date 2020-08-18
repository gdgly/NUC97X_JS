
#ifndef SoftwareCheck_H
#define SoftwareCheck_H

u32 SoftwareCheck(u32 *p32);//软件检查;入口:软件入口地址,返回:0=错误,1=正确
void SoftwareCheck_All(void);//除MS外其他全部软件模块检查

#endif
