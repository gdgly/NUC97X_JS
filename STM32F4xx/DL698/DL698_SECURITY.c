

#include "Project698.h"
#include "../DL698/DL698_DataDef.h"
#include "../Device/IC_TESAM.h"
#include "../DL698/DL698_SECURITY.h"

#include "../DL698/DL698_Uart.h"
#include "../STM32F4xx/STM32F4xx_UART.h"
#include "../Device/MEMRW.h"
#include "../TEST/LOG.h"



//SECURITY-Request∷=SEQUENCE
//{
//应用数据单元  CHOICE
//{
//明文应用数据单元   [0]  octet-string，
//密文应用数据单元   [1]  octet-string
//}，
	
//数据验证信息  CHOICE
//{
//数据验证码      [0]  SID_MAC，
//随机数          [1]  RN，
//随机数+数据MAC  [2]  RN_MAC，
//安全标识        [3]  SID
//}
//}
	
//RN_MAC∷=SEQUENCE
//{
//   随机数  	RN，
//   数据MAC 	MAC
//}

void SECURITY_Request(u32 PORTn)//安全请求
{
	u32 i;
	u32 LEN_SA;//服务器地址SA的长度
	u32 LENmax_TxSPACE;//最大还可发送的字节数
	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
	u8 * p8tx;
	u32 LEN_TypeByte1;
	u32 LEN_Data1;
	u32 LEN_TypeByte2;
	u32 LEN_Data2;
	u32 DI;
	u8 *p8esamrx;
//	ms_Type *ms;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	
	p8esamrx=(u8 *)(ADDR_UART7Rx);
	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);

	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	
	i=p8rx[LEN_SA+8+1];
	LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+LEN_SA+8+2);//得到DL698字节类型用来表示长度单元的字节数
	LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+LEN_SA+8+2);//得到DL698字节类型用来表示长度单元的字节数
	switch(i)
	{
		case 0://明文应用数据单元   [0]  octet-string
			i=p8rx[LEN_SA+8+2+LEN_TypeByte1+LEN_Data1];//数据验证信息
			switch(i)
			{
				case 0://数据验证码      [0]  SID_MAC
					UARTCtrl->SECURITY_Request=0x80;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
//68 2F 00 43 05 01 00 00 00 00 00 10 43 D9 
//10
//00 明文应用数据单元 [0] 密文应用数据单元 [1] octet-string
//10 明文字节数
//06 01 06 40 03 02 00 09 06 00 00 00 00 00 01 00 
//00 80 1C 31 10 02 00 14 04 73 8D EE C5 
//DF 59 16 
//发送： 4字节安全标识+附加数据AttachData+Data+MAC
//返回： 9000+LEN+Data2
					DI=MRR((u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+1,4);
					DI=R_Inverse(DI,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
					//取附加数据
					LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
					LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
					if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					MR(ADDR_UART7Tx+7,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
					//取Data
					MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1,LEN_Data1);
					//取MAC
					MR(ADDR_UART7Tx+7+LEN_Data2-2+LEN_Data1,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2+1,4);
					i=LEN_Data2-2+LEN_Data1+4;
					i=TESAM_CMD_TxRx(DI,i,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
					if(i)
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					i=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
					if(i>LEN_UART7Rx)
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					//组接收帧
					MR((u32)p8rx+LEN_SA+8,(u32)p8esamrx+5,i);
					//修改接收帧长度域
					i+=LEN_SA+8+3;
					i-=2;//除起始字符和结束字符之外的帧字节数
					p8rx[1]=i;
					p8rx[2]=i>>8;
					i-=1;
					p8rx[i]=0xAB;//校验码，无用仅查看帧方便用
					p8rx[i+1]=0xCD;//校验码，无用仅查看帧方便用
					p8rx[i+2]=0x16;//无用仅查看帧方便用
					Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
					
					if(UARTCtrl->Task==2)
					{//等待发送
					#if LOG_698==1//0=没,1=有
						LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
						LEN_Data1=p8tx[1]+(p8tx[2]<<8);
						LEN_Data1+=2;
						if(LEN_Data1<(LEN_SA+8+3))
						{
							LEN_Data1=LEN_SA+8+3;
						}
						LEN_Data1-=LEN_SA+8+3;//APUD长度
						i=TESAM_CMD_TxRx(0x801C0011,LEN_Data1,(u32)p8tx+LEN_SA+8);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
						if(i)
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						//返回格式例:明文+MAC=11
						//68 29 00 C3 05 LEN_SA 10 8D 40 (LEN_SA+8)
						//90	安全响应   		[144]	SECURITY-Response
						//00	明文应用数据单元   [0]  octet-string
						//10	29 D5 B1 44 88 D2 45 3C F2 69 32 F4 C9 1C 99 06	APDU明文
						//01 数据验证信息  CHOICE   OPTIONAL=1
						//00 数据MAC         [0]  MAC
						//04 40 E9 34 07 MAC
						//A1 88 16 
						LEN_Data2=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
						if((LEN_Data2<4)||((LEN_SA+8+10+LEN_Data2)>LENmax_TxSPACE))
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						LEN_Data2-=4;
						p8tx[3]|=0x80;
						i=LEN_SA+8;
						p8tx[i]=144;//安全响应   		[144]	SECURITY-Response
						p8tx[i+1]=0;//明文应用数据单元   [0]  octet-string
						i+=2;
						if(LEN_Data2<128)
						{
							p8tx[i]=LEN_Data2;
							i++;
							MR((u32)p8tx+i,(u32)p8esamrx+5,LEN_Data2);
							i+=LEN_Data2;
						}
						else
						{
							p8tx[i]=0x82;
							p8tx[i+1]=LEN_Data2>>8;
							p8tx[i+2]=LEN_Data2;
							i+=3;
							MR((u32)p8tx+i,(u32)p8esamrx+5,LEN_Data2);
							i+=LEN_Data2;
						}
						p8tx[i]=1;//数据验证信息  CHOICE   OPTIONAL=1
						p8tx[i+1]=0;//数据MAC         [0]  MAC
						p8tx[i+2]=4;
						i+=3;
						MR((u32)p8tx+i,(u32)p8esamrx+5+LEN_Data2,4);
						i+=4;
						
						i+=3;
						UARTCtrl->TxByte=i;
						i-=2;
						p8tx[1]=i;
						p8tx[2]=i>>8;
						Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
						return;
					}
					break;
				case 1://随机数          [1]  RN
					UARTCtrl->SECURITY_Request=0x81;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
//68 2C 00 43 05 01 00 00 00 00 00 10 F0 27
//10
//00 明文应用数据单元 [0] 密文应用数据单元 [1] octet-string
//08 05 01 05 43 00 03 00 00 
//01 10 32 E7 82 31 8F 64 8B CE 32 E7 82 31 8F 64 8B CE 63 03 16
					LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+1);//得到DL698字节类型用来表示长度单元的字节数
					LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+1);//得到DL698字节类型用来表示长度单元的字节数
					//组接收帧
					MR((u32)p8rx+LEN_SA+8,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1,LEN_Data1);
					//修改接收帧长度域
					i=LEN_SA+8+LEN_Data1+3;
					i-=2;//除起始字符和结束字符之外的帧字节数
					p8rx[1]=i;
					p8rx[2]=i>>8;
					Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
				
//68 51 00 C3 05 01 00 00 00 00 00 10 83 0A 
//90 00 
//38 85 01 06 43 00 03 00 01 02 06 0A 04 54 4F 50 53 0A 04 56 31 2E 31 0A 06 31 36 31 30 30 39 0A 04 31 2E 30 30 0A 06 31 35 30 36 30 31 0A 08 20 20 20 20 20 20 20 20 00 00 
//01 数据验证信息  CHOICE   OPTIONAL=1
//00 数据MAC         [0]  MAC
//04 1F 97 49 0A MAC
//E9 57 16 	
					if(UARTCtrl->Task==2)
					{//等待发送
					#if LOG_698==1//0=没,1=有
						LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
						//取主站下发的随机数
						if(LEN_Data2>(LEN_UART7Tx-7))
						{
							break;//错误
						}
						MR(ADDR_UART7Tx+7,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+1+LEN_TypeByte2,LEN_Data2);
						//明文APUD长度
						LEN_Data1=p8tx[1]+(p8tx[2]<<8);
						LEN_Data1+=2;
						if(LEN_Data1<(LEN_SA+8+3))
						{
							LEN_Data1=LEN_SA+8+3;
						}
						LEN_Data1-=LEN_SA+8+3;//明文APUD长度
						if((LEN_Data1+LEN_Data2)>(LEN_UART7Tx-7))
						{
							break;//错误
						}
						//取明文APUD
						MR(ADDR_UART7Tx+7+LEN_Data2,(u32)p8tx+LEN_SA+8,LEN_Data1);
						i=LEN_Data1+LEN_Data2;
						i=TESAM_CMD_TxRx(0x800E4002,i,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
						if(i)
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						i=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
						if((i<4)||((LEN_SA+8+10+LEN_Data1+4)>LENmax_TxSPACE))
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						p8tx[3]|=0x80;
						i=LEN_SA+8;
						p8tx[i]=144;//安全响应   		[144]	SECURITY-Response
						p8tx[i+1]=0;//明文应用数据单元   [0]  octet-string
						i+=2;
						if(LEN_Data1<128)
						{
							p8tx[i]=LEN_Data1;
							i++;
							MR((u32)p8tx+i,ADDR_UART7Tx+7+LEN_Data2,LEN_Data1);
							i+=LEN_Data1;
						}
						else
						{
							p8tx[i]=0x82;
							p8tx[i+1]=LEN_Data1>>8;
							p8tx[i+2]=LEN_Data1;
							i+=3;
							MR((u32)p8tx+i,ADDR_UART7Tx+7+LEN_Data2,LEN_Data1);
							i+=LEN_Data1;
						}
						p8tx[i]=1;//数据验证信息  CHOICE   OPTIONAL=1
						p8tx[i+1]=0;//数据MAC         [0]  MAC
						p8tx[i+2]=4;
						i+=3;
						MR((u32)p8tx+i,(u32)p8esamrx+5,4);
						i+=4;
						
						i+=3;
						UARTCtrl->TxByte=i;
						i-=2;
						p8tx[1]=i;
						p8tx[2]=i>>8;
						Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
						return;
					}
					break;
				case 2://随机数+数据MAC  [2]  RN_MAC
					UARTCtrl->SECURITY_Request=0x82;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
					break;
				case 3://安全标识        [3]  SID
					UARTCtrl->SECURITY_Request=0x83;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
					break;
				default:
					break;
			}
			break;
		case 1://密文应用数据单元   [1]  octet-string
//68 71 07 43 05 01 00 00 00 00 00 10 2B E8 
//10
//01 明文应用数据单元 [0] 密文应用数据单元 [1] octet-string
//82 07 50 F8 64 71 28 BA 84 18 B4 94 66 60 DA 84 C6 39 4F 67 E9 86 C5 54 25 94 21 F0 D5 82 F5 63 5D 7B E0 E9 27 0D 86 94 39 CB 16 1A 65 62 D2 37 42 35 C8 4A A2 FF 40 1A 10 3A 76 73 23 47 1F 9E 42 FB 7C E7 42 6F 1A D5 D9 EE 98 21 5E 6F 28 30 58 15 BA BA 19 57 6B 4A 62 F9 8E 99 A5 2A A3 DF 8B AF 13 BA 0C 0B 18 21 43 FE 19 E9 79 B2 24 8A 0F C3 11 D6 12 D7 60 E0 D3 5A 86 3E 35 E6 0C B6 EB 70 CF 39 53 01 F8 03 A1 11 7F 90 B9 FD A5 B6 68 88 2B CB F2 38 D6 D9 24 74 AC 4E 92 61 89 57 2C CE F9 6A E1 A7 6E BF 9C 51 18 E2 AC 78 E4 1A 45 E1 01 AA A8 D3 26 02 2C 3F 51 5D 1C F2 EB 82 45 61 B9 CD E5 1A 78 6D 09 D7 E8 2B 48 93 2C F2 3F 9B 68 0D 11 39 02 95 11 C2 CA 54 39 8A 11 84 0E 32 DF C1 21 CA 4F 6B CE E8 61 6C 37 50 5A E8 44 C2 88 22 66 1D 59 DD 3E 92 E6 73 85 B1 65 33 94 F2 1F AF 45 E6 13 7C 78 D6 95 65 3E 5D E4 46 94 67 B7 E9 3C D2 89 3E A9 6D AF C7 08 8D AF 05 78 DA 95 6D C1 79 EA F0 27 EF 78 EA 7F 3B 38 9E 98 7C 63 38 45 7C 38 02 71 15 F3 5C 31 22 4E B0 96 1C FC AA 84 D7 C6 F9 2B C3 7A B0 34 13 28 C1 DF 8C 3A 7D F3 AF 3F 58 5A 28 80 B5 D4 FF 4E D1 80 5D 89 5F ED B7 E3 51 CA 26 B1 16 78 13 ED 7A 65 2A BC 5E D8 D9 40 3C 7E F5 2E DB 41 BA 9E D9 59 4D EE 9C 45 4B FA 1F BE 0F CB 5C E5 0E 82 A9 18 37 87 0F CB 23 8B 8D 32 0A E3 05 9D A0 B4 55 41 12 78 02 3A 4C B8 5B E4 49 98 A3 A2 7D 76 93 7C 83 95 9E DB 2E 66 45 8E C6 BF 05 B5 2D E7 9D 30 CB 1B 8B 7F BD 6F 22 F6 7F 61 42 CD BA 80 4A 53 C0 0D 84 D3 0B 5B 1B 49 4C 76 8A E8 EC AC 8B FE DE 94 D6 D6 31 4E 2A CD DF 11 F9 79 49 B0 A8 09 63 64 55 60 2C 66 11 06 FE 5A 28 D1 C1 BC 0B 63 03 5E 6E 89 2F 35 E1 E8 35 B3 F3 15 FB 97 D3 66 65 5A 72 30 82 4F CD 2E 4B 88 F4 67 DE 4A EE EB 07 D3 3A 6D 35 FF 3A C7 74 72 12 FA DD 74 CB D7 5F 41 DB DC 77 66 E0 C8 B6 D0 EC 11 80 DA 06 37 D5 83 34 61 46 FB 04 83 3D DF B7 C2 A7 CB 7B 8B 03 8D 4A FE 2E 74 D4 C2 77 69 E0 95 A1 B3 0A 35 A2 40 C2 65 E8 DD D3 02 5E 42 71 5B 25 FD 44 0A 0C 81 C5 09 13 FE 6F C2 32 6C 03 C4 BC 44 1D 62 83 B0 15 C0 76 19 59 5F D7 A0 DE BC F3 52 4D 25 E1 5B 5C E3 3F 04 D3 53 06 74 14 D9 59 F0 20 CA A1 92 95 46 92 E7 61 CE 5E 27 D1 2A 9C 2C 22 40 85 A3 6F EC 66 B4 2F 32 9A 29 A3 BD C6 D3 0F F5 47 02 D6 25 CF FD 76 C7 0D 71 16 1F 22 95 C3 0F 1F 64 4E AC 3B 95 98 1C 3B 93 6B A0 85 97 28 A0 C4 F8 9A 87 4F A1 C1 DD 1A AC EA F3 DD B5 83 28 A8 66 47 BE 84 35 01 92 A4 37 2D 96 D0 0D 1B DA BD 61 13 8F 22 D4 EE 36 CB 5D 4C FC 3D 13 11 FA 6A EA 4E DF 61 E4 57 89 4D A8 87 8C BB 16 87 B6 84 79 D8 75 C2 CE 65 DC 8F 6A 89 AF 2C 69 1A D0 38 3B 7F E5 50 32 33 7A F5 FD D5 C2 E5 A7 53 4F 74 8B 72 35 80 BF 23 48 6B 13 EA A4 E2 D3 B7 75 47 8E F9 5E A7 2F 14 8E 47 25 2F 29 1A 35 38 75 06 F7 7C E5 10 89 7B A1 4D 30 26 F3 EC B4 88 E8 C7 A0 1E 7E 6C 2A BF 10 15 67 CA 49 91 F0 A7 3F CD 68 D4 D9 0D 2C B8 AA 7B 3F 67 4D 51 45 D5 21 B6 E6 F2 E8 D9 5A A7 5A 54 D3 3E C2 29 EE 2D B5 B2 B0 27 A8 17 21 3D E6 AB 47 0B CC 5D 4E CC 40 D1 8B 89 7E C7 82 16 15 4D 5E 9C 57 CC D2 7A 3D 7A 32 AA 31 F8 13 38 27 E6 D7 FD C2 F3 CC B0 71 6B D3 78 C2 9A F2 51 73 9B 1A A7 37 2D E5 26 76 13 F5 98 F8 36 ED E5 DB 0E AC 65 A7 84 B4 B9 B4 FF 7E E7 43 82 22 E6 B7 08 2B 9F F9 32 F9 3F DD 5C A4 4B F0 47 A7 68 94 4B 8F FB 05 D4 9E A0 41 48 16 CE 9F 91 1B 43 3E 0D 4B 7B 46 78 08 84 36 FC 93 83 74 DD EB 22 8A 4F F2 E6 77 07 0D B5 6F 19 53 AF B0 03 9A C5 9E 3C 5D 92 2F F4 BD 5F 47 0E FF 36 92 53 62 81 DD 47 06 00 CE 60 CA 6D 5E EF F0 FB 88 09 9D 48 64 9D FA 1D 45 54 56 49 22 49 6D 16 92 DD A1 8C 7D E2 CF 43 70 01 56 B2 12 C4 D2 B7 AD E0 A8 E6 1E EB 09 4D F8 11 6D C5 D3 E8 A5 E5 09 B9 CA 94 8D A8 C9 23 F5 6E 3D F2 45 A2 9F E1 6A C0 EC 18 35 C4 2C D1 B3 76 6E 93 48 37 04 8F BD C2 67 94 8B F5 8F 94 69 7D 9A DE 69 CF 12 EB 7C 45 F8 3C 90 72 7C 19 91 65 6B E9 48 91 D1 57 88 5B FA D4 1C F8 5B 3D 00 B0 E3 7F CC 90 CB 20 7E 26 36 26 CC 3D 84 8E 4E 1C F2 B9 83 9A EE F2 81 0C A2 01 6D A3 FC CB 05 B5 78 8A 74 EE 30 BC E4 B8 74 06 02 26 97 DE C8 54 F2 CD 29 8A CF D7 CA 74 FD DA E0 3D B0 F0 4F AB 1F 08 1C 58 76 9D BA AA D1 21 1C CE D4 D7 EE 35 AC 52 EA 65 6E 7F 69 86 B6 AB 46 C7 A0 44 3D 08 13 74 9C B4 1D 24 7A DC A2 48 CD 21 EA C8 AF DA B6 7C F5 E2 5C E9 F9 59 CC 30 A8 3D 0E AA 3B CE 09 B8 4A 70 87 DC 36 F3 76 A9 A8 69 32 AD 68 C7 E3 52 1E 1A 10 48 92 07 3B E1 6E 5C AC 14 DD 7A EA 8A 53 74 67 41 63 67 95 32 F7 F0 C3 14 6E 4A 27 73 A2 C5 AE E4 02 FD A6 22 02 80 D3 C6 0E 6D 4E EC 48 7A A2 8C C1 21 77 12 98 AA 2C 13 BE 9A C1 B2 20 83 F8 B8 EF 4E 45 7E 06 AC DE 4B 69 41 E9 64 1D 8F 03 CE 59 44 A2 DE 02 C1 93 53 6E CE F7 BB 17 84 8F 6F 21 8A 78 95 1F 47 A8 8B 85 96 8E AC 91 C5 21 06 C5 F7 1C D0 6A 71 3D D9 BE 7E 19 B2 D4 53 CA 1E D8 0C 8D 7B 6B DE 3C 58 EE 28 D3 B2 CD 3A D2 81 71 BD 54 E6 68 00 92 98 CB 19 97 A6 7B 8D 86 E8 FC 74 55 26 AA 5D C9 14 3A 6E 04 6C 31 A3 C8 91 2D 87 E5 D6 C9 CC CA 65 88 F8 5D 85 93 D4 6D AE 5D D3 24 45 17 46 0F A8 14 C5 CD F5 D1 87 44 4B 52 17 38 25 1F 38 BD 79 55 76 AA 90 68 9B BA 97 47 B3 89 3B 7D 5D 3A 6A F7 94 0D E2 4D 79 F1 CB 68 A3 8C B1 9C 95 A2 91 62 EA F6 14 06 1A ED FD EE A6 12 76 66 50 76 15 68 4D 33 4E 9F C8 D5 0A DA 53 11 1A 66 DC 7C B2 8A 68 BC 61 B9 87 AC 81 AA C5 CA ED FA 7B B1 44 94 98 24 DA EA A8 2C FF 70 0D 4A DE 10 4B 38 32 D8 D5 3C 58 28 FA 9E 50 15 EB 5F 84 83 C3 42 FD CB FE 1B FA 69 F2 96 96 EC 60 B1 DA 3E D7 46 3E E1 67 22 01 FC 28 DE 2B D9 A7 6D 7B 73 3D A4 2A 72 F5 E7 72 D5 A8 30 4C 58 77 DA FB 78 D9 69 31 2D 12 1E 24 E1 39 35 4D 3D E9 C7 6C C3 E9 54 84 3D 26 94 1E C6 30 AB 73 D2 09 83 7E 2C 44 88 AA A4 0E DF 9E 59 68 C7 DD C4 33 05 CE DF 2A 81 28 A0 C5 ED 12 AD 85 56 06 78 7A A1 B6 45 D3 7E 74 93 DF F2 FD 5B F0 D3 13 2C A9 FC F3 EC 95 8F 0D 68 C2 C2 30 0D AE BB 6E 02 BC 01 67 80 CC 95 27 B2 A3 DE 5A F0 D7 FE 85 09 49 EC 7C DC 36 2E 2D 39 D1 72 09 27 70 37 
//00 80 1C 33 10 02 07 54 
//04 C9 8D 66 16 
//7F 96 16 	
			DI=MRR((u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+1,4);
			DI=R_Inverse(DI,4);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
/*
			if(UARTCtrl->AddrType!=1)//接收帧的地址类型:0=空,1=单地址,2=组地址,3=广播地址
			{
				if((DI>>4)!=0x8016480)
				{
					UARTCtrl->DAR=22;//ESAM验证失败        （22）
					break;//错误
				}
				if((DI&0xf)!=(p8rx[5]&0xf))
				{
					UARTCtrl->DAR=22;//ESAM验证失败        （22）
					break;//错误
				}
			}
*/
			i=p8rx[LEN_SA+8+2+LEN_TypeByte1+LEN_Data1];//数据验证信息
			switch(i)
			{
				case 0://数据验证码      [0]  SID_MAC
					UARTCtrl->SECURITY_Request=0xC0;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
					//发送TESAM:安全标识+附加数据AttachData+密文Endata1+MAC
					//取附加数据
					LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
					LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
					if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					MR(ADDR_UART7Tx+7,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
					//取密钥密文Endata1
					MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1,LEN_Data1);
					//取MAC
					MR(ADDR_UART7Tx+7+LEN_Data2-2+LEN_Data1,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2+1,4);
					i=LEN_Data2-2+LEN_Data1+4;
					i=TESAM_CMD_TxRx(DI,i,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
					if(i)
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					i=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
					if(i>LEN_UART7Rx)
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					//组接收帧
					MR((u32)p8rx+LEN_SA+8,(u32)p8esamrx+5,i);
					//修改接收帧长度域
					i+=LEN_SA+8+3;
					i-=2;//除起始字符和结束字符之外的帧字节数
					p8rx[1]=i;
					p8rx[2]=i>>8;
					i-=1;
					p8rx[i]=0xAB;//校验码，无用仅查看帧方便用
					p8rx[i+1]=0xCD;//校验码，无用仅查看帧方便用
					p8rx[i+2]=0x16;//无用仅查看帧方便用
				#if LOG_698==1//0=没,1=有
					LOG(698,2,PORTn,p8rx,i+3,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
					Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
					
					if(UARTCtrl->Task==2)
					{//等待发送
					#if LOG_698==1//0=没,1=有
						LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
						LEN_Data1=p8tx[1]+(p8tx[2]<<8);
						LEN_Data1+=2;
						if(LEN_Data1<(LEN_SA+8+3))
						{
							LEN_Data1=LEN_SA+8+3;
						}
						LEN_Data1-=LEN_SA+8+3;//APUD长度
						i=TESAM_CMD_TxRx(0x801C0097,LEN_Data1,(u32)p8tx+LEN_SA+8);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
						if(i)
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						//返回格式例:密文+MAC=97
						//68 29 00 C3 05 LEN_SA 10 8D 40 (LEN_SA+8)
						//90	安全响应   		[144]	SECURITY-Response
						//01	密文应用数据单元   [1]  octet-string
						//10	29 D5 B1 44 88 D2 45 3C F2 69 32 F4 C9 1C 99 06	APDU明文的密文
						//01 数据验证信息  CHOICE   OPTIONAL=1
						//00 数据MAC         [0]  MAC
						//04 40 E9 34 07 MAC
						//A1 88 16 
						LEN_Data2=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
						if((LEN_Data2<4)||((LEN_SA+8+10+LEN_Data2)>LENmax_TxSPACE))
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						LEN_Data2-=4;
						p8tx[3]|=0x80;
						i=LEN_SA+8;
						p8tx[i]=144;//安全响应   		[144]	SECURITY-Response
						p8tx[i+1]=1;//密文应用数据单元   [1]  octet-string
						i+=2;
						if(LEN_Data2<128)
						{
							p8tx[i]=LEN_Data2;
							i++;
							MR((u32)p8tx+i,(u32)p8esamrx+5,LEN_Data2);
							i+=LEN_Data2;
						}
						else
						{
							p8tx[i]=0x82;
							p8tx[i+1]=LEN_Data2>>8;
							p8tx[i+2]=LEN_Data2;
							i+=3;
							MR((u32)p8tx+i,(u32)p8esamrx+5,LEN_Data2);
							i+=LEN_Data2;
						}
						p8tx[i]=1;//数据验证信息  CHOICE   OPTIONAL=1
						p8tx[i+1]=0;//数据MAC         [0]  MAC
						p8tx[i+2]=4;
						i+=3;
						MR((u32)p8tx+i,(u32)p8esamrx+5+LEN_Data2,4);
						i+=4;
						
						i+=3;
						UARTCtrl->TxByte=i;
						i-=2;
						p8tx[1]=i;
						p8tx[2]=i>>8;
						Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
						return;
					}
					break;
				case 1://随机数          [1]  RN
					UARTCtrl->SECURITY_Request=0xC1;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
					break;
				case 2://随机数+数据MAC  [2]  RN_MAC
					UARTCtrl->SECURITY_Request=0xC2;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
					break;
				case 3://安全标识        [3]  SID
					UARTCtrl->SECURITY_Request=0xC3;//安全请求:b7=0无安全=1安全传输,b6=0明文=1密文,b5=,b4=,b3-b0=0数据验证码[0]SID_MAC =1随机数[1]RN =2随机数+数据MAC[2]RN_MAC =3安全标识[3]SID
					//发送TESAM:4字节安全标识+附加数据AttachData+Data
					//返回：9000+LEN+Data2
					//取附加数据
					LEN_TypeByte2=Get_DL698ByteTypeLenByte(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
					LEN_Data2=Get_DL698ByteTypeLenValue(p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5);//得到DL698字节类型用来表示长度单元的字节数
					if((LEN_Data2<2)||((LEN_Data1+LEN_Data2-2+4)>(LEN_UART7Tx-7)))
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					MR(ADDR_UART7Tx+7,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+2,LEN_Data2-2);
					//取密钥密文Endata1
					MR(ADDR_UART7Tx+7+LEN_Data2-2,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1,LEN_Data1);
					//取MAC
					//MR(ADDR_UART7Tx+7+LEN_Data2-2+LEN_Data1,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1+LEN_Data1+5+LEN_TypeByte2+LEN_Data2+1,4);
					i=LEN_Data2-2+LEN_Data1;//+4;
					i=TESAM_CMD_TxRx(DI,i,ADDR_UART7Tx+7);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
					if(i)
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					i=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
					if(i>LEN_UART7Rx)
					{
						UARTCtrl->DAR=22;//ESAM验证失败        （22）
						break;//错误
					}
					//组接收帧
					MR((u32)p8rx+LEN_SA+8,(u32)p8esamrx+5,i);
					//修改接收帧长度域
					i+=LEN_SA+8+3;
					i-=2;//除起始字符和结束字符之外的帧字节数
					p8rx[1]=i;
					p8rx[2]=i>>8;
					i-=1;
					p8rx[i]=0xAB;//校验码，无用仅查看帧方便用
					p8rx[i+1]=0xCD;//校验码，无用仅查看帧方便用
					p8rx[i+2]=0x16;//无用仅查看帧方便用
				#if LOG_698==1//0=没,1=有
					LOG(698,2,PORTn,p8rx,i+3,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
				#endif
					Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
					
					if(UARTCtrl->Task==2)
					{//等待发送
					#if LOG_698==1//0=没,1=有
						LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
					#endif
						LEN_Data1=p8tx[1]+(p8tx[2]<<8);
						LEN_Data1+=2;
						if(LEN_Data1<(LEN_SA+8+3))
						{
							LEN_Data1=LEN_SA+8+3;
						}
						LEN_Data1-=LEN_SA+8+3;//APUD长度
						i=TESAM_CMD_TxRx(0x801C0097,LEN_Data1,(u32)p8tx+LEN_SA+8);//发送ESAM命令;返回:0=正确发收且返回9000,1=错误
						if(i)
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						//返回格式例:密文+MAC=97
						//68 29 00 C3 05 LEN_SA 10 8D 40 (LEN_SA+8)
						//90	安全响应   		[144]	SECURITY-Response
						//01	密文应用数据单元   [1]  octet-string
						//10	29 D5 B1 44 88 D2 45 3C F2 69 32 F4 C9 1C 99 06	APDU明文的密文
						//01 数据验证信息  CHOICE   OPTIONAL=1
						//00 数据MAC         [0]  MAC
						//04 40 E9 34 07 MAC
						//A1 88 16 
						LEN_Data2=(p8esamrx[3]<<8)+p8esamrx[4];//返回数据长度
						if((LEN_Data2<4)||((LEN_SA+8+10+LEN_Data2)>LENmax_TxSPACE))
						{
							UARTCtrl->DAR=22;//ESAM验证失败        （22）
							break;//错误
						}
						LEN_Data2-=4;
						p8tx[3]|=0x80;
						i=LEN_SA+8;
						p8tx[i]=144;//安全响应   		[144]	SECURITY-Response
						p8tx[i+1]=1;//密文应用数据单元   [1]  octet-string
						i+=2;
						if(LEN_Data2<128)
						{
							p8tx[i]=LEN_Data2;
							i++;
							MR((u32)p8tx+i,(u32)p8esamrx+5,LEN_Data2);
							i+=LEN_Data2;
						}
						else
						{
							p8tx[i]=0x82;
							p8tx[i+1]=LEN_Data2>>8;
							p8tx[i+2]=LEN_Data2;
							i+=3;
							MR((u32)p8tx+i,(u32)p8esamrx+5,LEN_Data2);
							i+=LEN_Data2;
						}
						p8tx[i]=1;//数据验证信息  CHOICE   OPTIONAL=1
						p8tx[i+1]=0;//数据MAC         [0]  MAC
						p8tx[i+2]=4;
						i+=3;
						MR((u32)p8tx+i,(u32)p8esamrx+5+LEN_Data2,4);
						i+=4;
						
						i+=3;
						UARTCtrl->TxByte=i;
						i-=2;
						p8tx[1]=i;
						p8tx[2]=i>>8;
						Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
						return;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
//异常错误返回
//68 29 00 C3 05 LEN_SA 10 8D 40 (LEN_SA+8)
//90	安全响应   		[144]	SECURITY-Response
//02	异常错误           [2]  DAR
//DAR
//A1 88 16 
	MR((u32)p8tx,(u32)p8rx,LEN_SA+8);//复制接收头
	p8tx[3]|=0x80;
	i=LEN_SA+8;
	p8tx[i]=144;//安全响应   		[144]	SECURITY-Response
	p8tx[i+1]=2;//异常错误           [2]  DAR
	p8tx[i+2]=UARTCtrl->DAR;
	i+=3;
	
	i+=3;
	UARTCtrl->TxByte=i;
	i-=2;
	p8tx[1]=i;
	p8tx[2]=i>>8;
	Uart_698_HCSFCS(p8tx);//整帧校验计算(HCS和FCS),同时加结束0x16;返回0=正确，1=错误
	UARTCtrl->Task=2;
	
#if LOG_698==1//0=没,1=有
	LOG(698,1,PORTn,(u8 *)Get_ADDR_UARTnTx(PORTn),UARTCtrl->TxByte,0);//入口:TR=0接收=1发送;protocol=13761,13762,13763,6451997,6452007;ascii=0为hex码=1本身为ascii码
#endif
}



//安全响应的数据类型（SECURITY-Response）
//SECURITY-Response∷=SEQUENCE
//{
//应用数据单元  CHOICE
//{
//明文应用数据单元   [0]  octet-string，
//密文应用数据单元   [1]  octet-string，
//异常错误           [2]  DAR
//}，
//数据验证信息  CHOICE   OPTIONAL
//{
//数据MAC         [0]  MAC
//}
//}
void SECURITY_Response(u32 PORTn)//安全响应
{
	u32 i;
	u32 LEN_SA;//服务器地址SA的长度
	u32 LENmax_TxSPACE;//最大还可发送的字节数
//	UARTCtrl_TypeDef * UARTCtrl;
	u8 * p8rx;
//	u8 * p8tx;
	u32 LEN_TypeByte1;
	u32 LEN_Data1;
//	u32 LEN_TypeByte2;
//	u32 LEN_Data2;
//	u32 DI;
//	u8 *p8esamrx;
//	ms_Type *ms;
	ClientConnectInfo_TypeDef *ClientConnectInfo;//客户机连接信息
	
//	p8esamrx=(u8 *)(ADDR_UART7Rx);
//	UARTCtrl=(UARTCtrl_TypeDef *)Get_ADDR_UARTnCtrl(PORTn);
	LENmax_TxSPACE=Get_LEN_UARTnTx(PORTn);//最大还可发送的字节数
	LENmax_TxSPACE-=TxAPDUOffset+1;//1byte上报信息FollowReport OPTIONAL=0 表示没有
#if USE_ClientConnectInfo==0//使用客户机连接信息的客户机最大接收字节:0=不使用,1=使用
	ClientConnectInfo=ClientConnectInfo;
#else
	//客户机接收缓冲长度
	ClientConnectInfo=(ClientConnectInfo_TypeDef*)UARTCtrl->ConnectInfo;//客户机连接信息
	i=ClientConnectInfo->LEN_Rx;
	i=R_Inverse(i,2);//寄存器数据倒序(高低字节调换),入口:Data=寄存器中数据,Byte=需倒序的字节数;返回:倒序后的数据
	if(i<2048)
	{
		i=2048;
	}
	if(i>(TxAPDUOffset+1))
	{//不为0
		i-=(TxAPDUOffset+1);
		if(LENmax_TxSPACE>i)
		{
			LENmax_TxSPACE=i;
		}
	}
#endif
	p8rx=(u8 *)Get_ADDR_UARTnRx(PORTn);
//	p8tx=(u8 *)Get_ADDR_UARTnTx(PORTn);

	LEN_SA=p8rx[4];//取地址字节数
	LEN_SA&=0x0f;
	LEN_SA++;
	
	i=p8rx[LEN_SA+8+1];
	LEN_TypeByte1=Get_DL698ByteTypeLenByte(p8rx+LEN_SA+8+2);//得到DL698字节类型用来表示长度单元的字节数
	LEN_Data1=Get_DL698ByteTypeLenValue(p8rx+LEN_SA+8+2);//得到DL698字节类型用来表示长度单元的字节数
	switch(i)
	{
		case 0://明文应用数据单元   [0]  octet-string
//68 2c 00 03 05 01 00 00 00 00 00 00 12 d1 
//90 00 13 08 02 01 01 31 01 02 00 01 07 e2 08 01 09 2e 27 00 00 0a 01 
//00 04 a3 64 42 c4 
//e6 f4 16
			//组接收帧
			MR((u32)p8rx+LEN_SA+8,(u32)p8rx+LEN_SA+8+2+LEN_TypeByte1,LEN_Data1);
			//修改接收帧长度域
			i=LEN_SA+8+LEN_Data1+3;
			i-=2;//除起始字符和结束字符之外的帧字节数
			p8rx[1]=i;
			p8rx[2]=i>>8;
			Uart_698_Data(PORTn,0);//UART_698数据;入口:安全模式SECURITY=0不启用=1启用
			break;
		case 1://密文应用数据单元   [1]  octet-string，
			
			break;
		case 2://异常错误           [2]  DAR
			break;
	}
}








