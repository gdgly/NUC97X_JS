
//显示动画
#if MainProtocol==376
#include "../Head/Project.h"
#endif
#if MainProtocol==698
#include "../H698/Project698.h"
#endif
#include "../Display/DisplayFlash.h"



void DisplayFlash(void)//显示动画
{
	u32 i;
	u32 x;
	u32 *p32s;
	u32 *p32d;
	
	switch(DOTMIXPMODE->FlashTask)//动画任务:0=无,1=开始,2=播放
	{
		case 1://开始
			if(DOTMIXPMODE->FlashMode!=0)
			{
				
				DOTMIXPMODE->FlashTask=2;//动画任务:0=无,1=开始,2=播放
				DOTMIXPMODE->FlashCount=0;//动画播放计数
				Comm_Ram->DisplayScanTimer=DOTMIXPMODE->FlashDelay;//1 显示扫描 每10MS减1定时器
				p32s=(u32*)(ADDR_DOTDATABUFF);
				p32d=(u32*)(ADDR_DOTDATABUFF2);
				for(i=0;i<((160*20)/4);i++)
				{
					p32d[i]=p32s[i];
				}
			}
			else
			{
				DOTMIXPMODE->FlashTask=0;//动画任务:0=无,1=开始,2=播放
			}
			break;
		case 2://播放
			if(DOTMIXPMODE->Task==4)//当前显示任务:0=上电屏幕,1=USB下载显示,2=错误显示,3=菜单显示,4=循环显示
			{//循显
				if(DOTMIXPMODE->FlashCount>=(160-32))
				{
					DOTMIXPMODE->FlashTask=0;//动画任务:0=无,1=开始,2=播放
					DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
					Comm_Ram->DisplayScanTimer=0;//1 显示扫描 每10MS减1定时器
				}
				else
				{
					DOTMIXPMODE->FlashCount+=4;//动画播放计数
					Comm_Ram->DisplayScanTimer=DOTMIXPMODE->FlashDelay;//1 显示扫描 每10MS减1定时器
					DOTMIXPMODE->Mix=1;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
					switch(DOTMIXPMODE->FlashMode)//动画模式:0=无,1=下移,2=上移,3=左移,4=右移,5=...
					{
						case 1://下移
							p32d=(u32*)(ADDR_DOTDATABUFF2+(160*20)-(16*20)-4);
							p32s=(u32*)(ADDR_DOTDATABUFF2+(160*20)-(16*20)-4-(4*20));
							x=(160-32-4)*20;
							x/=4;
							for(i=0;i<x;i++)
							{
								p32d[0]=p32s[0];
								p32s--;
								p32d--;
							}
							x=(4*20)/4;
							for(i=0;i<x;i++)
							{
								p32d[0]=0;
								p32d--;
							}
							break;
						case 2://上移
							p32d=(u32*)(ADDR_DOTDATABUFF2+(16*20));
							p32s=(u32*)(ADDR_DOTDATABUFF2+(16*20)+(4*20));
							x=(160-32-4)*20;
							x/=4;
							for(i=0;i<x;i++)
							{
								p32d[0]=p32s[0];
								p32s++;
								p32d++;
							}
							x=(4*20)/4;
							for(i=0;i<x;i++)
							{
								p32d[0]=0;
								p32d++;
							}
							break;
						case 3://左移
						case 4://右移
							break;
						default:
							DOTMIXPMODE->FlashTask=0;//动画任务:0=无,1=开始,2=播放
							DOTMIXPMODE->Mix=0;//点阵缓冲A和B合成方式:0=A,1=B,2=A|B,3=A^B,4=A&B
							break;
					}
				}
			}
			else
			{//非循显
			
			
			
			}
			break;
	}
}

/*
Terminal_Ram->DisplayFlashTimer=0;//2 显示动画 每10MS减1定时器
DOTMIXPMODE->FlashMode;//动画模式:0=无,1=下移,2=上移,3=左移,4=右移,5=...
DOTMIXPMODE->FlashDelay;//动画变化延时(单位10ms),0最快,255最慢
DOTMIXPMODE->FlashTask;//动画任务:0=无,1=开始,2=播放
	u8 FlashCount;//动画播放计数
*/

