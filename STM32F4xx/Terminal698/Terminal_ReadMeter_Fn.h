

#ifndef Terminal_ReadMeter_Fn_H
#define Terminal_ReadMeter_Fn_H

u32 ReadMeter_Fn_YesNo(u32 PORTn,u32 ClassNo,u32 Pn,u8 * p8RMLib,u32 RMCount,u32 Protocol);//按大小类号抄表的Fn是否要抄;返回0=不抄,1=要抄;入口:RMFLAGS抄读标志,ClassNo的D7-D4大类号D3-D0小类号,Pn,p8RMLib抄表库指针,RMCount抄表计数,Protocol(1=1997,其他=2007)

#endif


