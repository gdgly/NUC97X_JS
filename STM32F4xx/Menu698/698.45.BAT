SET PATH=E:\Keil\ARM\ARMCC\Bin;E:\Klocwork\Command Line 19.2\bin;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\PuTTY\;C:\Program Files\Git\cmd;C:\Program Files\TortoiseGit\bin;C:\Program Files\TortoiseSVN\bin
SET CPU_TYPE=STM32F469BITx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=698.45
SET CPU_CLOCK=0x00B71B00
"E:\Keil\ARM\ARMCC\Bin\ArmAsm" --Via "d:\obj\menu_entry._ia"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\get_rm_record.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\display.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\draw.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\calculate.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\dataconversion.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\ascii.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_main.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_init.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_ver.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_1_realtimedata.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_2_parameters.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_2_parameterssetting.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_3_controlstate.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_3_systemmanagement.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_4_meterdata.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_5_chinesenews.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_6_powerpurchase .__i"
"E:\Keil\ARM\ARMCC\Bin\ArmCC" --Via "d:\obj\menu_7_terminalinformation.__i"
"E:\Keil\ARM\ARMCC\Bin\ArmAsm" --Via "d:\obj\menu_end._ia"
"E:\Keil\ARM\ARMCC\Bin\ArmLink" --Via "D:\obj\Menu698.lnp"
"E:\Keil\ARM\ARMCC\Bin\fromelf.exe" "D:\obj\Menu698.axf" --i32combined --output "D:\obj\Menu698.hex"
E:\Keil\ARM\ARMCC\bin\fromelf.exe --bin -o D:\obj\BIN\ D:\obj\Menu698.axf
