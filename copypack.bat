::@echo off
set  s_path=D:\obj\BIN
set  d_path=D:\VMsharefile\pack
if exist "%d_path%" ( 
goto start 
)
set d_path=D:\Virtual Machines\VMsharefile\pack
if exist "%d_path%" ( 
goto start 
)

:start
copy  "%s_path%\Terminal698N.bin" "%d_path%\Terminal698N.bin"

::pause