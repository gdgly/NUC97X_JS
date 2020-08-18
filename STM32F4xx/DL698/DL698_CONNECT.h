

#ifndef Terminal698_CONNECT_H
#define Terminal698_CONNECT_H
#ifdef __cplusplus
 extern "C" {
#endif

	 
void CONNECT_Request(u32 PORTn);
void RELEASE_Request(u32 PORTn);
void RELEASE_Notification(u32 PORTn);
u32 GET_OAD_ConnectApp(u32 PORTn,u32 OAD,u8* p8tx,u32 LENmax_TxSPACE);//读应用连接可访问对象列表;返回:本对象的数据长度,数据访问结果UARTCtrl->DAR


//协议一致性块
#define Protocol0     1//应用连接协商Application Association			（0），
#define Protocol1     1//	请求对象属性Get Normal						（1），
#define Protocol2     1//	批量请求基本对象属性Get With List			（2），
#define Protocol3     1//	请求记录型对象属性Get Record				（3），
#define Protocol4     1//	代理请求对象属性Get Proxy					（4），
#define Protocol5     1//	代理请求记录型对象属性Get Proxy Record		（5），
#define Protocol6     1//	请求分帧后续帧Get Subsequent Frame			（6），
#define Protocol7     1//	设置基本对象属性Set Normal					（7），
#define Protocol8     1//	批量设置基本对象属性Set With List			（8），
#define Protocol9     1//	设置后读取Set With Get						（9），
#define Protocol10     1//	代理设置对象属性Set Proxy					（10），
#define Protocol11     1//	代理设置后读取对象属性Set Proxy With Get		（11），
#define Protocol12     1//	执行对象方法Action Normal					（12），
#define Protocol13     1//	批量执行对象方法Action With List				（13），
#define Protocol14     1//	执行方法后读取Action With List				（14），
#define Protocol15     1//	代理执行对象方法Action Proxy				（15），
#define Protocol16     1//	代理执行后读取Action Proxy With Get			（16），
#define Protocol17     1//	事件主动上报Active Event Report				（17），
#define Protocol18     1//     事件尾随上报                              	（18)，
#define Protocol19     1//事件请求访问位ACD上报                    	（19)，
#define Protocol20     1//     分帧数据传输 Slicing Service              	（20），
#define Protocol21     1//     Get-request分帧                           	（21），
#define Protocol22     1//Get-response分帧                          	（22），
#define Protocol23     1//     Set-request分帧                         		（23），
#define Protocol24     1//Set-response分帧                        		（24），
#define Protocol25     1//     Action-request分帧                      		（25），
#define Protocol26     1//Action-response分帧                     		（26），
#define Protocol27     1//Proxy-request 分帧                       		（27），
#define Protocol28     1//Proxy-response分帧                      		（28），
#define Protocol29     1//事件上报分帧                            		（29），
#define Protocol30     1//DL/T645-2007                            		（30），
#define Protocol31     1//安全方式传输                            		（31），
#define Protocol32     0//对象属性ID为0的读取访问                     （32），
#define Protocol33     0//对象属性ID为0的设置访问                     （33）
#define Protocol34     0
#define Protocol35     0
#define Protocol36     0
#define Protocol37     0
#define Protocol38     0
#define Protocol39     0


//功能一致性块
#define Function0     1//	电能量计量							（0），
#define Function1     1//	双向有功计量							（1），
#define Function2     1//	无功电能计量							（2），
#define Function3     1//	视在电能计量							（3），
#define Function4     1//	有功需量						    		（4），
#define Function5     1//	无功需量						    		（5），
#define Function6     1//	视在需量						    		（6），
#define Function7     1//	复费率					     		（7），
#define Function8     1//	阀控									（8），
#define Function9     1//	本地费控								（9），
#define Function10     1//	远程费控								（10），
#define Function11     1//	基波电能							（11），
#define Function12     1//	谐波电能								（12），
#define Function13     1//	谐波含量								（13），
#define Function14     1//	波形失真度							（14），
#define Function15     1//	多功能端子输出						（15），
#define Function16     1//	事件记录						    		（16），
#define Function17     1//	事件上报								（17），
#define Function18     1//	温度测量								（18），
#define Function19     1//	状态量监测（如：开表盖/开端钮盖）		（19），
#define Function20     1//	以太网通信							（20），
#define Function21     1//	红外通信								（21），
#define Function22     0//	蓝牙通信								（22），
#define Function23     1//	多媒体采集							（23），
#define Function24     0//	级联									（24），
#define Function25     1//	直流模拟量							（25），
#define Function26     1//	弱电端子12V输出						（26），
#define Function27     1//	搜表									（27），
#define Function28     0//	三相负载平衡							（28），
#define Function29     1//	升级									（29），
#define Function30     0//	比对									（30）
#define Function31     0
#define Function32     0
#define Function33     0
#define Function34     0
#define Function35     0
#define Function36     0
#define Function37     0
#define Function38     0
#define Function39     0





#ifdef __cplusplus
 }
#endif
#endif

