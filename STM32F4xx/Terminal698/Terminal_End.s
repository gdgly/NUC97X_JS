
;//I0_system���ģ�����


  export ADDR_Terminal_END



  AREA Terminal_END,DATA,READONLY
  ALIGN 4
 
  DCD 0;//CRC32
  DCD 0x5A5A5A5A;//�̶���ʶ(���)
ADDR_Terminal_END


  END