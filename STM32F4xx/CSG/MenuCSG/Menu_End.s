
;//软件模块结束


  export ADDR_Menu_END



  AREA Menu_END,DATA,READONLY
  ALIGN 4
 
  DCD 0;//CRC32
  DCD 0x5A5A5A5A;//固定标识(最后)
ADDR_Menu_END


  END